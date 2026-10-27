#include "TrainActions.hpp"
#include "WalkAction.hpp"
#include "WorldModel.hpp"
#include "BotLog.hpp"
#include "input_bits.hpp"
#include <math.h>
#include "BotCompat.hpp"

TrainTestAction::TrainTestAction(float x, float y)
    : mX(x)
    , mY(y)
    , mWalk(NULL)
    , mArrived(false)
    , mArrivedFrame(0)
    , mPressStart(-1000)
    , mPrevTx(0)
    , mPrevTy(0)
    , mLastLog(0)
{
}

TrainTestAction::~TrainTestAction()
{
    delete mWalk;
}

void TrainTestAction::Start(BotContext& ctx)
{
    delete mWalk;
    mWalk = NULL;
    mArrived = false;
}

Action::Status TrainTestAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (!mArrived)
    {
        if (!mWalk)
        {
            mWalk = new WalkAction(mX, mY, false, 1.0f);
            mWalk->Start(ctx);
        }
        const Status s = mWalk->Step(ctx);
        if (s == Failed)
        {
            return Fail(mWalk->fail_condition, "train_test: %s", mWalk->fail_detail);
        }
        if (s == Done)
        {
            mArrived = true;
            mArrivedFrame = ctx.frame;
            BotDecision("f=%d train_test: on the platform at (%.1f,%.1f)", ctx.frame, pl.x, pl.y);
        }
        return Running;
    }
    // the nearest train car
    const WorldModel::CarInfo* train = NULL;
    for (int i = 0; i < ctx.world->car_count; i++)
    {
        const WorldModel::CarInfo& c = ctx.world->cars[i];
        if (c.model >= 59 && c.model <= 61)
        {
            train = &c;
            break;
        }
    }
    if (ctx.frame - mLastLog >= 30)
    {
        mLastLog = ctx.frame;
        if (train)
        {
            BotDecision("f=%d train_test: train car #%d model %d at (%.1f,%.1f) %.1f tiles away, moved %.2f since last; me (%.1f,%.1f) incar=%d car=%d state=%d obj=%d/%d rot=%.0f", ctx.frame, train->idx, train->model, train->x, train->y, train->dist,
                        sqrtf((train->x - mPrevTx) * (train->x - mPrevTx) + (train->y - mPrevTy) * (train->y - mPrevTy)), pl.x, pl.y, pl.in_car, pl.car_id, pl.ped_state, pl.objective, pl.internal_objective, pl.rot_deg);
            mPrevTx = train->x;
            mPrevTy = train->y;
        }
        else
        {
            BotDecision("f=%d train_test: no train near; me (%.1f,%.1f) incar=%d", ctx.frame, pl.x, pl.y, pl.in_car);
        }
    }
    if (ctx.frame - mArrivedFrame > 6000)
    {
        return Done;
    }
    if (pl.in_car)
    {
        return Running; // riding: just log
    }
    if (train && train->dist < 4.0f)
    {
        // next to a train: walk to it and press enter
        if (ctx.frame - mPressStart > 300)
        {
            mPressStart = ctx.frame;
            BotDecision("f=%d train_test: train #%d is %.1f tiles away; trying to board", ctx.frame, train->idx, train->dist);
        }
        const float err = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, train->x, train->y, true);
        if (fabsf(err) < 40.0f && train->dist > 2.6f)
        {
            ctx.input |= input_bit::Forward; // the track is at the train's x: stay off it
        }
        if (((ctx.frame - mPressStart) / 4) % 2 == 0)
        {
            ctx.input |= input_bit::EnterExit;
        }
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
RideTrainGoal::RideTrainGoal(float dest_x, float dest_y, bool force)
    : mDx(dest_x)
    , mDy(dest_y)
    , mStage(ToPlatform)
    , mWalk(NULL)
    , mBoardId(-1)
    , mAlightId(-1)
    , mStageFrame(0)
    , mStartX(0)
    , mStartY(0)
    , mPrevX(0)
    , mPrevY(0)
    , mStillFrames(0)
    , mPressFrame(0)
    , mTrainId(0)
    , mTrainX(0)
    , mTrainY(0)
    , mTrainStill(0)
    , mCabId(0)
    , mCabX(0)
    , mCabY(0)
    , mCabVx(0)
    , mCabVy(0)
    , mSpotX(0)
    , mSpotY(0)
    , mHaveSpot(false)
    , mMyX(0)
    , mMyY(0)
    , mBoardings(0)
    , mForce(force)
{
}

RideTrainGoal::~RideTrainGoal()
{
    delete mWalk;
}

static float Dist(float ax, float ay, float bx, float by)
{
    return sqrtf((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

float RideTrainGoal::Estimate(const WorldModel& w, float px, float py, float x, float y, s32* board_id, s32* alight_id, bool force)
{
    float best = -1.0f;
    for (int b = 0; b < w.station_count; b++)
    {
        for (int a = 0; a < w.station_count; a++)
        {
            if (a == b)
            {
                continue;
            }
            const WorldModel::Station& sb = w.stations[b];
            const WorldModel::Station& sa = w.stations[a];
            const float walk1 = Dist(px, py, sb.board_x, sb.board_y) * 1.3f;
            const float walk2 = Dist(sa.board_x, sa.board_y, x, y) * 1.3f;
            const float ride = Dist(sb.stop_x, sb.stop_y, sa.stop_x, sa.stop_y) * 1.6f; // the loops wind
            const float frames = (walk1 + walk2) * 28.0f + ride * 7.0f + 900.0f; // 28 frames a tile with the traffic; 900: waiting for a train
            if (best < 0.0f || frames < best)
            {
                best = frames;
                *board_id = sb.id;
                *alight_id = sa.id;
            }
        }
    }
    return best;
}

const WorldModel::Station* RideTrainGoal::FindStation(const WorldModel& w, s32 id) const
{
    for (int i = 0; i < w.station_count; i++)
    {
        if (w.stations[i].id == id)
        {
            return &w.stations[i];
        }
    }
    return NULL;
}

bool RideTrainGoal::OnPlatform(const WorldModel& w, float x, float y, s32* station_id) const
{
    for (int i = 0; i < w.station_count; i++)
    {
        const WorldModel::Station& s = w.stations[i];
        if (x >= s.px - 1.5f && x <= s.px + s.pw + 1.5f && y >= s.py - 1.5f && y <= s.py + s.ph + 1.5f)
        {
            *station_id = s.id;
            return true;
        }
    }
    return false;
}

void RideTrainGoal::Start(BotContext& ctx)
{
    delete mWalk;
    mWalk = NULL;
    mStage = ToPlatform;
    mStageFrame = ctx.frame;
    mStartX = ctx.world->player.x;
    mStartY = ctx.world->player.y;
    mBoardings = 0;
    mBoardId = -2; // not decided yet: the first Step does, once the stations are read
}

Action::Status RideTrainGoal::Step(BotContext& ctx)
{
    const WorldModel& w = *ctx.world;
    const WorldModel::PlayerInfo& pl = w.player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (mBoardId == -2)
    {
        if (w.station_count == 0)
        {
            return ctx.frame - ctx.action_start_frame > 60 ? Fail("no_plan", "ride_train: the map has no stations") : Running;
        }
        mStartX = pl.x;
        mStartY = pl.y;
        if (Estimate(w, mStartX, mStartY, mDx, mDy, &mBoardId, &mAlightId, mForce) < 0.0f)
        {
            mBoardId = -1;
        }
    }
    if (mBoardId < 0)
    {
        return Fail("no_plan", "ride_train: no station pair is worth it");
    }
    const WorldModel::Station* sb = FindStation(w, mBoardId);
    if (!sb)
    {
        return Fail("no_plan", "ride_train: no station %d", mBoardId);
    }
    switch (mStage)
    {
        case ToPlatform:
        {
            if (!mWalk)
            {
                BotDecision("f=%d ride_train: to the platform of station %d at (%.1f,%.1f); getting off at station %d", ctx.frame, mBoardId, sb->board_x, sb->board_y, mAlightId);
                mWalk = new WalkAction(sb->board_x, sb->board_y, false, 0.7f);
                mWalk->Start(ctx);
            }
            const Status s = mWalk->Step(ctx);
            if (s == Failed)
            {
                return Fail(mWalk->fail_condition, "ride_train: %s", mWalk->fail_detail);
            }
            if (s == Done)
            {
                delete mWalk;
                mWalk = NULL;
                mStage = Waiting;
                mStageFrame = ctx.frame;
                mStillFrames = 0;
                BotDecision("f=%d ride_train: waiting for a train at (%.1f,%.1f)", ctx.frame, pl.x, pl.y);
            }
            return Running;
        }
        case Waiting:
        {
            if (pl.in_car)
            {
                mStage = Riding;
                mStageFrame = ctx.frame;
                mBoardings++;
                mPrevX = pl.x;
                mPrevY = pl.y;
                mStillFrames = 0;
                BotDecision("f=%d ride_train: aboard car #%d at (%.1f,%.1f)", ctx.frame, pl.car_id, pl.x, pl.y);
                return Running;
            }
            if (ctx.frame - mStageFrame > 5000)
            {
                return Fail("no_progress", "ride_train: no train stopped for 5000 frames");
            }
            // The lead car (cab, model 60) cannot be boarded: a carriage behind it can. The cab stops at the
            // stop point, the carriages stand behind it, so find which way the train comes and wait beside
            // the second car.
            for (int i = 0; i < w.car_count; i++)
            {
                const WorldModel::CarInfo& c = w.cars[i];
                if (c.model == 60 && c.dist < 45.0f)
                {
                    if (c.idx == mCabId)
                    {
                        mCabVx = mCabVx * 0.8f + (c.x - mCabX) * 0.2f;
                        mCabVy = mCabVy * 0.8f + (c.y - mCabY) * 0.2f;
                    }
                    else
                    {
                        mCabId = c.idx;
                        mCabVx = mCabVy = 0.0f;
                    }
                    mCabX = c.x;
                    mCabY = c.y;
                    const float sp = sqrtf(mCabVx * mCabVx + mCabVy * mCabVy);
                    if (sp > 0.05f)
                    {
                        // along the track axis only: the platform side stays as it is
                        const bool vertical = fabsf(mCabVy) > fabsf(mCabVx);
                        const float dir = vertical ? (mCabVy > 0 ? 1.0f : -1.0f) : (mCabVx > 0 ? 1.0f : -1.0f);
                        mSpotX = sb->board_x + (vertical ? 0.0f : -dir * 4.5f);
                        mSpotY = sb->board_y + (vertical ? -dir * 4.5f : 0.0f);
                        mHaveSpot = true;
                    }
                    break;
                }
            }
            if (mHaveSpot && Dist(pl.x, pl.y, mSpotX, mSpotY) > 0.5f && mTrainStill < 3)
            {
                const float serr = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, mSpotX, mSpotY, true);
                if (fabsf(serr) < 40.0f)
                {
                    ctx.input |= input_bit::Forward;
                }
            }
            // a train car beside us that has stopped: press enter at once, it leaves in about 80 frames
            const WorldModel::CarInfo* train = NULL;
            for (int i = 0; i < w.car_count; i++)
            {
                const WorldModel::CarInfo& c = w.cars[i];
                if (c.model == 59 && c.dist < 3.2f && (!train || c.idx == mTrainId))
                {
                    train = &c; // a carriage (the cab cannot be boarded), the nearest, but keep the one being watched
                }
            }
            // The game's own velocity reads zero for trains: watch how far the car moves instead
            if (train)
            {
                if (train->idx == mTrainId && Dist(train->x, train->y, mTrainX, mTrainY) < 0.004f)
                {
                    mTrainStill++;
                }
                else
                {
                    mTrainStill = 0;
                }
                mTrainId = train->idx;
                mTrainX = train->x;
                mTrainY = train->y;
            }
            else
            {
                mTrainId = 0;
                mTrainStill = 0;
            }
            const float my_move = Dist(pl.x, pl.y, mMyX, mMyY);
            mMyX = pl.x;
            mMyY = pl.y;
            if (mPressFrame != 0 && ctx.frame - mPressFrame < 90 && ctx.frame % 3 == 0)
            {
                BotDecision("f=%d ride_train: after the press +%d: state %d obj %d/%d incar %d; still-frames of the watched car %d; cab #%d at (%.1f,%.1f) v=(%.3f,%.3f)", ctx.frame, ctx.frame - mPressFrame, pl.ped_state, pl.objective,
                            pl.internal_objective, pl.in_car, mTrainStill, mCabId, mCabX, mCabY, mCabVx, mCabVy);
            }
            if (train && mTrainStill >= 3)
            {
                // It has stopped. Get squarely beside the carriage (the game looks half a tile ahead of
                // the ped for the nearest car), face it, and press enter once: that starts the walk to
                // the door (the "enter train" objective). The train leaves in about 80 frames.
                const float ax = sb->board_x;
                const float ay = train->y;
                const float d = Dist(pl.x, pl.y, ax, ay);
                if (d > 0.45f)
                {
                    const float serr = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, ax, ay, true);
                    if (fabsf(serr) < 40.0f)
                    {
                        ctx.input |= input_bit::Forward;
                    }
                }
                else if (my_move < 0.01f)
                {
                    const float ferr = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, train->x, train->y, true);
                    if (fabsf(ferr) < 15.0f)
                    {
                        if (mPressFrame == 0 || ctx.frame - mPressFrame > 40)
                        {
                            mPressFrame = ctx.frame;
                            BotDecision("f=%d ride_train: train car #%d stopped beside me (%.1f tiles), boarding (me %.1f,%.1f state %d objective %d)", ctx.frame, train->idx, train->dist, pl.x, pl.y, pl.ped_state, pl.objective);
                        }
                        if (ctx.frame - mPressFrame < 3)
                        {
                            ctx.input |= input_bit::EnterExit;
                        }
                    }
                }
            }
            return Running;
        }
        case Riding:
        {
            if (!pl.in_car)
            {
                // thrown out, or got off: carry on from here
                BotDecision("f=%d ride_train: out of the train at (%.1f,%.1f)", ctx.frame, pl.x, pl.y);
                return Done;
            }
            const float moved = Dist(pl.x, pl.y, mPrevX, mPrevY);
            mPrevX = pl.x;
            mPrevY = pl.y;
            mStillFrames = moved < 0.005f ? mStillFrames + 1 : 0;
            if (ctx.frame % 120 == 0)
            {
                BotDecision("f=%d ride_train: riding at (%.1f,%.1f), %.0f tiles from the target", ctx.frame, pl.x, pl.y, Dist(pl.x, pl.y, mDx, mDy));
            }
            // stopped at a platform: get off if this is much nearer the target than where we got on
            s32 here = -1;
            if (mStillFrames > 12 && OnPlatform(w, pl.x, pl.y, &here))
            {
                const float near_now = Dist(pl.x, pl.y, mDx, mDy);
                const float near_start = Dist(mStartX, mStartY, mDx, mDy);
                if (here == mAlightId || near_now < near_start * 0.5f || ctx.frame - mStageFrame > 3500)
                {
                    mStage = Leaving;
                    mStageFrame = ctx.frame;
                    BotDecision("f=%d ride_train: stopped at station %d, %.0f tiles from the target (%.0f at the start); getting off", ctx.frame, here, near_now, near_start);
                }
            }
            if (ctx.frame - mStageFrame > 9000)
            {
                return Fail("no_progress", "ride_train: riding for 9000 frames");
            }
            return Running;
        }
        case Leaving:
        default:
        {
            if (!pl.in_car)
            {
                BotDecision("f=%d ride_train: off at (%.1f,%.1f), %.0f tiles from the target", ctx.frame, pl.x, pl.y, Dist(pl.x, pl.y, mDx, mDy));
                return Done;
            }
            if (ctx.frame - mStageFrame > 600)
            {
                return Fail("action_failed", "ride_train: cannot get off the train");
            }
            if ((ctx.frame - mStageFrame) % 8 < 3)
            {
                ctx.input |= input_bit::EnterExit;
            }
            return Running;
        }
    }
}
