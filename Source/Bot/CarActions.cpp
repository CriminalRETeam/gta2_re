#include "CarActions.hpp"
#include "ChaseActions.hpp"
#include "WalkAction.hpp"
#include "WorldModel.hpp"
#include "NavCar.hpp"
#include "Snapshot.hpp"
#include "BotLog.hpp"
#include "BotOptions.hpp"
#include "input_bits.hpp"
#include <math.h>
#include <stdio.h>
#include "BotCompat.hpp"

// ---------------------------------------------------------------------------------------------
EnterCarAction::EnterCarAction(s32 car_id, bool hijack)
    : mCarId(car_id)
    , mHijack(hijack)
    , mWalk(NULL)
    , mPressing(false)
    , mPressStart(0)
    , mStable(0)
{
}

EnterCarAction::~EnterCarAction()
{
    delete mWalk;
}

void EnterCarAction::Start(BotContext& /*ctx*/)
{
    delete mWalk;
    mWalk = NULL;
    mPressing = false;
    mStable = 0;
}

Action::Status EnterCarAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.in_car)
    {
        if (pl.car_id == mCarId)
        {
            // a hijack puts the ped in the car and may throw it out again at once: wait until it stays
            if (++mStable >= 12)
            {
                BotDecision("f=%d enter_car: in car #%d", ctx.frame, mCarId);
                return Done;
            }
            return Running;
        }
        return Fail("action_failed", "enter_car: in car #%d instead of #%d", pl.car_id, mCarId);
    }
    mStable = 0;

    WorldModel::CarInfo car;
    if (!ctx.world->FindCar(mCarId, &car))
    {
        return Fail("action_failed", "enter_car: car #%d is gone", mCarId);
    }
    if ((car.has_driver && !mHijack) || car.locked)
    {
        return Fail("action_failed", "enter_car: car #%d driver=%d locked=%d", mCarId, car.has_driver, car.locked);
    }
    if (mPressing && car.has_driver && car.speed > 0.05f)
    {
        return Fail("action_failed", "enter_car: car #%d drove off (speed %.2f)", mCarId, car.speed);
    }

    if (!mPressing)
    {
        if (!mWalk)
        {
            mWalk = new WalkAction(car.x, car.y, false, 2.2f);
            mWalk->Start(ctx);
        }
        const Status s = mWalk->Step(ctx);
        if (s == Failed)
        {
            return Fail(mWalk->fail_condition, "enter_car: %s", mWalk->fail_detail);
        }
        if (s == Done)
        {
            mPressing = true;
            mPressStart = ctx.frame;
            BotDecision("f=%d enter_car: next to car #%d at (%.1f,%.1f), pressing enter", ctx.frame, mCarId, car.x, car.y);
        }
        return Running;
    }

    if (ctx.frame - mPressStart > 90)
    {
        return Fail("action_failed", "enter_car: pressing enter for 160 frames next to car #%d (dist %.1f) did not get the ped in", mCarId, car.dist);
    }
    if (((ctx.frame - mPressStart) / 4) % 2 == 0)
    {
        ctx.input = input_bit::EnterExit;
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
ExitCarAction::ExitCarAction()
    : mStart(0)
{
}

void ExitCarAction::Start(BotContext& ctx)
{
    mStart = ctx.frame;
}

Action::Status ExitCarAction::Step(BotContext& ctx)
{
    ctx.input = 0;
    if (!ctx.world->player.in_car)
    {
        BotDecision("f=%d exit_car: out", ctx.frame);
        return Done;
    }
    if (ctx.frame - mStart > 200)
    {
        return Fail("action_failed", "exit_car: still in the car after 200 frames");
    }
    if (ctx.frame - mStart > 20 && ((ctx.frame - mStart) / 4) % 2 == 0)
    {
        ctx.input = input_bit::EnterExit; // after the car has had time to stop
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
DriveAction::DriveAction(float x, float y, float stop_dist)
    : mTx(x)
    , mTy(y)
    , mStopDist(stop_dist)
    , mCalibrating(false)
    , mPlanned(false)
    , mWpCount(0)
    , mWpIdx(0)
    , mProgressFrame(0)
    , mRetries(0)
    , mSpeedCap(0.20f)
    , mWpRadius(3.5f)
    , mHavePrev(false)
    , mPrevX(0)
    , mPrevY(0)
    , mSpeed(0)
    , mStillFrames(0)
    , mReverseUntil(0)
    , mReverseSteer(0)
    , mManoeuvres(0)
    , mShoveUntil(0)
    , mShoves(0)
    , mHandbrakeUntil(0)
    , mHandbrakeNext(0)
    , mSwerveUntil(0)
    , mSwerveX(0.0f)
    , mSwerveY(0.0f)
    , mCkNext(0)
    , mCkSlot(0)
    , mLeftStart(false)
    , mBlockedFrames(0)
    , mBlockReplans(0)
    , mMaxSpeedSeen(0)
    , mBestWpDist(1e9f)
    , mBestWpFrame(0)
    , mLastWpIdx(-1)
    , mTurnUntil(0)
    , mTurnEnd(0)
    , mTurnReverse(false)
    , mBehindSince(0)
    , mFwdUntil(0)
    , mStartX(0)
    , mStartY(0)
    , mPlanX(0)
    , mPlanY(0)
{
    mCk[0] = mCk[1] = NULL;
    mCkFrame[0] = mCkFrame[1] = -1;
}

DriveAction::~DriveAction()
{
    delete mCk[0];
    delete mCk[1];
}

void DriveAction::Start(BotContext& ctx)
{
    ctx.road->ClearBans();
    mTurnUntil = 0;
    mTurnEnd = 0;
    mTurnReverse = false;
    mBehindSince = 0;
    mFwdUntil = 0;
    mStartX = ctx.world->player.x;
    mLeftStart = false;
    mCkFrame[0] = mCkFrame[1] = -1;
    mCkNext = ctx.frame + 300;
    mStartY = ctx.world->player.y;
    mPlanned = false;
    mRetries = 0;
    mHavePrev = false;
    if (!ctx.car.valid && ctx.foot.valid)
    {
        // The rotation to heading relation measured for the ped and for cars came out the same
        // every time, and calibrating a car means crashing it about the place: reuse it
        ctx.car = ctx.foot;
        BotDecision("f=%d drive: using the pedestrian's steering model for the car", ctx.frame);
    }
    mCalibrating = !ctx.car.valid;
    if (mCalibrating)
    {
        BotDecision("f=%d drive: calibrating the car's steering", ctx.frame);
        mCalib.Start(true);
    }
}

// Is the strip of the given half width along the line free of buildings? (the first tile and a half is the car's own)
static bool Drive_CorridorClear(float x0, float y0, float x1, float y1, float half_width)
{
    const float ex = x1 - x0;
    const float ey = y1 - y0;
    const float el = sqrtf(ex * ex + ey * ey);
    if (el < 2.0f)
    {
        return true;
    }
    const float px = -ey / el * half_width;
    const float py = ex / el * half_width;
    for (float d = 1.5f; d <= el; d += 0.5f)
    {
        const float qx = x0 + ex / el * d;
        const float qy = y0 + ey / el * d;
        if (WorldModel::BuildingAt(qx + px, qy + py) || WorldModel::BuildingAt(qx - px, qy - py))
        {
            return false;
        }
    }
    return true;
}

// Where would the car end up if it held the wheel at s (-1 left, +1 right, 0.5 = every other frame) for the next few tiles?
// The car's footprint is tested against buildings (a wall ends the arc), and against other cars at the place they will
// have reached by then (a car coming the other way is hard to hit, one going our way or standing still is only a bump).
// The cost is how far from the aim point the arc ends, plus a penalty for what it hits.
struct DriveArc
{
    float cost;
    float wall_d; // tiles driven before the first building (the whole reach if none)
    bool car_hit;
};

static const float kTurnDegPerFrame = 1.8f; // measured from the trace: a car turns about this fast at any speed

static DriveArc Drive_Arc(const BotContext& ctx, float heading_deg, float hdir, float s, float speed, float aim_x, float aim_y)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    DriveArc arc;
    arc.wall_d = 0.0f;
    arc.car_hit = false;
    float hard_car = 0.0f;
    float x = pl.x;
    float y = pl.y;
    float h = heading_deg;
    const float v = speed < 0.10f ? 0.10f : speed; // it will pick up speed
    const float reach = 4.0f + speed * 45.0f;
    const int step = 3;
    float driven = 0.0f;
    float t = 0.0f;
    bool wall = false;
    while (driven < reach)
    {
        h += hdir * s * kTurnDegPerFrame * step;
        const float rad = h * 3.14159265f / 180.0f;
        const float fx = sinf(rad);
        const float fy = -cosf(rad);
        x += fx * v * step;
        y += fy * v * step;
        driven += v * step;
        t += step;
        const float px = -fy * 0.6f;
        const float py = fx * 0.6f;
        // a wall the car already touches is not one it is about to hit: only look beyond the first stretch, or an arc
        // that starts beside a wall (the corner of a road) is blocked at once and the car cannot move out of it
        if (driven > 0.8f && (WorldModel::BuildingAt(x + fx * 0.9f + px, y + fy * 0.9f + py) || WorldModel::BuildingAt(x + fx * 0.9f - px, y + fy * 0.9f - py) ||
            WorldModel::BuildingAt(x + px, y + py) || WorldModel::BuildingAt(x - px, y - py)))
        {
            wall = true;
            break;
        }
        if (!arc.car_hit && driven > 2.5f)
        {
            for (int k = 0; k < ctx.world->car_count; k++)
            {
                const WorldModel::CarInfo& oc = ctx.world->cars[k];
                if (oc.dist < 1.0f)
                {
                    continue; // our own
                }
                const float cx = oc.x + oc.vx * t;
                const float cy = oc.y + oc.vy * t;
                if ((cx - x) * (cx - x) + (cy - y) * (cy - y) < 1.4f * 1.4f)
                {
                    arc.car_hit = true;
                    const bool oncoming = oc.speed > 0.12f && oc.vx * fx + oc.vy * fy < -0.10f;
                    hard_car = (oncoming || oc.police) ? 25.0f : 4.0f;
                    break;
                }
            }
        }
    }
    arc.wall_d = driven;
    // an arc that ends pointing away from the aim point is no better than one that ends a little short of it
    const float bearing = Bot_HeadingDeg(aim_x - x, aim_y - y);
    arc.cost = sqrtf((aim_x - x) * (aim_x - x) + (aim_y - y) * (aim_y - y)) + fabsf(Bot_WrapDeg(bearing - h)) * (12.0f / 57.3f) + hard_car;
    if (wall)
    {
        arc.cost += 20.0f + (reach - driven) * 3.0f;
    }
    else
    {
        arc.wall_d = 1e3f; // nothing in the way: the speed is not limited by this arc
    }
    return arc;
}

bool DriveAction::Plan(BotContext& ctx, const char* why)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    // A clear straight line is the quickest way: no detour to the roads and their arrows
    const float ddx = mTx - pl.x;
    const float ddy = mTy - pl.y;
    // stopped cars close to the car cost extra to route through
    ctx.road->ClearJams();
    for (int k = 0; k < ctx.world->car_count; k++)
    {
        const WorldModel::CarInfo& oc = ctx.world->cars[k];
        if (oc.dist > 2.0f && oc.speed < 0.03f)
        {
            ctx.road->AddJam(oc.x, oc.y);
        }
        else if (oc.dist > 2.0f && oc.speed < 0.08f)
        {
            // a slow one is where it will be in a few seconds, not where it is: that tile is the one it clogs
            ctx.road->AddJam(oc.x + oc.vx * 60.0f, oc.y + oc.vy * 60.0f);
        }
    }
    const bool direct = ddx * ddx + ddy * ddy < 40.0f * 40.0f && ctx.road->DirectLine(pl.x, pl.y, mTx, mTy);
    const bool found = !direct && ctx.road->FindPath(pl.x, pl.y, mTx, mTy);
    mWpCount = 0;
    if (found)
    {
        for (u32 i = 1; i < ctx.road->Count() && mWpCount < kMaxWp - 1; i++) // 0 is where the car is
        {
            mWpX[mWpCount] = ctx.road->X(i);
            mWpY[mWpCount] = ctx.road->Y(i);
            mWpCount++;
        }
    }
    if (found && ctx.road->Approximate() && mWpCount > 0)
    {
        // the target cannot be reached by car: drive to the nearest tile that can
        mTx = mWpX[mWpCount - 1];
        mTy = mWpY[mWpCount - 1];
    }
    if (mWpCount == 0 || mWpX[mWpCount - 1] != mTx || mWpY[mWpCount - 1] != mTy)
    {
        mWpX[mWpCount] = mTx;
        mWpY[mWpCount] = mTy;
        mWpCount++;
    }
    mWpIdx = 0;
    mPlanX = pl.x;
    mPlanY = pl.y;
    mProgressFrame = ctx.frame;
    mStillFrames = 0;
    mReverseUntil = 0;
    mManoeuvres = 0;
    mBlockedFrames = 0;
    mHavePrev = false;
    mBestWpDist = 1e9f;
    mLastWpIdx = -1;
    BotDecision("f=%d drive plan (%s) (%.1f,%.1f)->(%.0f,%.0f): %s, %d waypoints, %u tiles explored, %s (cap %.2f, radius %.1f)", ctx.frame, why, pl.x, pl.y, mTx, mTy,
                direct ? "direct line" : (found ? "path" : "NO PATH, driving straight"), mWpCount, ctx.road->Expanded(), ctx.road->Clearance() ? "room each side" : "SQUEEZED (no route with room each side)", mSpeedCap, mWpRadius);
    Bot_RoutePlanned(why, ctx.road, pl.x, pl.y, mTx, mTy);
    if (ctx.nav_dump)
    {
        char path[MAX_PATH];
        BotPath_Join(path, sizeof(path), ctx.out_dir, "bot_road.txt");
        ctx.road->Dump(path, "drive route");
    }
    mPlanned = true;
    return found;
}

void DriveAction::ChooseVariant(BotContext& ctx)
{
    static const float caps[3] = {0.18f, 0.24f, 0.36f};
    static const float radii[3] = {2.5f, 3.5f, 5.0f};
    mSpeedCap = caps[ctx.rng->Below(3)];
    mWpRadius = radii[ctx.rng->Below(3)];
}

Action::Status DriveAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (!pl.in_car)
    {
        return Fail("action_failed", "drive: the player is not in a car");
    }

    if (mCalibrating)
    {
        char why[200];
        const Status s = mCalib.Step(ctx, &ctx.car, why, sizeof(why));
        if (s == Failed)
        {
            return Fail("action_failed", "%s", why);
        }
        if (s == Running)
        {
            return Running;
        }
        mCalibrating = false;
        BotDecision("f=%d drive: car heading = %+.0f*rot %+.0f, Right turns %+.0f", ctx.frame, ctx.car.sign, ctx.car.offset, ctx.car.turn_sign);
    }

    if (!mPlanned)
    {
        Plan(ctx, "start");
    }

    // Speed in tiles per frame
    if (mHavePrev)
    {
        const float mx = pl.x - mPrevX;
        const float my = pl.y - mPrevY;
        mSpeed = sqrtf(mx * mx + my * my);
        if (mSpeed > mMaxSpeedSeen)
        {
            mMaxSpeedSeen = mSpeed;
        }
    }
    mPrevX = pl.x;
    mPrevY = pl.y;
    mHavePrev = true;

    const float dx = mTx - pl.x;
    const float dy = mTy - pl.y;
    const float dist = sqrtf(dx * dx + dy * dy);
    if (dist < mStopDist)
    {
        BotDecision("f=%d drive arrived at (%.1f,%.1f), dist %.2f, %d retries, top speed %.2f tiles/frame", ctx.frame, mTx, mTy, dist, mRetries, mMaxSpeedSeen);
        return Done;
    }

    // Waypoints
    while (mWpIdx < mWpCount - 1)
    {
        const float wx = mWpX[mWpIdx] - pl.x;
        const float wy = mWpY[mWpIdx] - pl.y;
        const float d = sqrtf(wx * wx + wy * wy);
        bool passed = false;
        if (mWpIdx + 1 < mWpCount && d < 10.0f)
        {
            // past the waypoint when the next one is more than 90 degrees from the way we came
            const float nx = mWpX[mWpIdx + 1] - mWpX[mWpIdx];
            const float ny = mWpY[mWpIdx + 1] - mWpY[mWpIdx];
            passed = (-wx * nx + -wy * ny) > 0 && d < mWpRadius * 2.0f;
        }
        if (d < mWpRadius || passed)
        {
            mWpIdx++;
            mProgressFrame = ctx.frame;
            continue;
        }
        break;
    }

    // Rolling checkpoints while the drive goes well, so that a retry rewinds a few hundred frames and not to the start
    if (ctx.map && ctx.frame >= mCkNext && mSpeed > 0.04f && ctx.frame - mProgressFrame < 100 && !pl.dead)
    {
        if (!mCk[mCkSlot])
        {
            mCk[mCkSlot] = new Snapshot(ctx.map);
        }
        mCk[mCkSlot]->Capture();
        mCkFrame[mCkSlot] = ctx.frame;
        mCkSlot = 1 - mCkSlot;
        mCkNext = ctx.frame + 300;
    }

    // Stuck: not moving while asked to, or moving (sliding along a wall) without getting nearer
    {
        const float wdx = mWpX[mWpIdx] - pl.x;
        const float wdy = mWpY[mWpIdx] - pl.y;
        const float wd = sqrtf(wdx * wdx + wdy * wdy);
        if (mWpIdx != mLastWpIdx || wd < mBestWpDist - 1.0f)
        {
            mLastWpIdx = mWpIdx;
            mBestWpDist = wd;
            mBestWpFrame = ctx.frame;
        }
        else if (ctx.frame - mBestWpFrame > 100 && ctx.frame > mFwdUntil)
        {
            mStillFrames = 46;
            mBestWpFrame = ctx.frame;
        }
    }
    if (ctx.frame > ctx.action_start_frame + 40 && mSpeed < 0.004f && ctx.frame > mFwdUntil)
    {
        mStillFrames++;
    }
    else if (mSpeed >= 0.004f)
    {
        mStillFrames = 0;
    }
    // Stopped behind a car that is not going anywhere (a queue, a parked car in the lane, a broken down one):
    // close the tile it stands on to the route finder and plan again, round it by the next lane, the pavement
    // or another street, instead of waiting for it or shoving it
    {
        const float hd = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        const float fx = sinf(hd);
        const float fy = -cosf(hd);
        const WorldModel::CarInfo* blocker = NULL;
        for (int k = 0; k < ctx.world->car_count && !blocker; k++)
        {
            const WorldModel::CarInfo& oc = ctx.world->cars[k];
            const float along = (oc.x - pl.x) * fx + (oc.y - pl.y) * fy;
            const float side = (oc.x - pl.x) * -fy + (oc.y - pl.y) * fx;
            if (along > 0.3f && along < 4.0f && fabsf(side) < 1.5f && oc.speed < 0.03f)
            {
                blocker = &oc;
            }
        }
        if (blocker && mSpeed < 0.02f && ctx.frame > ctx.action_start_frame + 40 && ctx.frame > mReverseUntil && ctx.frame > mFwdUntil && ctx.frame > mShoveUntil)
        {
            mBlockedFrames++;
        }
        else if (mSpeed > 0.05f || !blocker)
        {
            mBlockedFrames = 0;
        }
        if (mBlockedFrames > 45 && (!blocker->police || pl.wanted_stars > 0) && mShoves <= mBlockReplans)
        {
            // a bump moves most cars (and costs a few points of damage): try that before going round
            mBlockedFrames = 0;
            mShoves++;
            mShoveUntil = ctx.frame + 45;
            BotDecision("f=%d drive: stopped behind car #%d (%.1f tiles ahead, %s): pushing it out of the way (%d)", ctx.frame, blocker->idx, (blocker->x - pl.x) * fx + (blocker->y - pl.y) * fy,
                        blocker->has_driver ? "driver" : "empty", mShoves);
        }
        else if (mBlockedFrames > 45 && mBlockReplans < 8)
        {
            mBlockedFrames = 0;
            mBlockReplans++;
            ctx.road->BanArea(static_cast<int>(blocker->x), static_cast<int>(blocker->y), 0);
            BotDecision("f=%d drive: stopped behind car #%d (%.1f tiles ahead, %s%s); closing its tile and planning round it (%d)", ctx.frame, blocker->idx,
                        (blocker->x - pl.x) * fx + (blocker->y - pl.y) * fy, blocker->has_driver ? "driver" : "empty", blocker->police ? ", police" : "", mBlockReplans);
            const s32 manoeuvres = mManoeuvres;
            Plan(ctx, "blocked");
            mManoeuvres = manoeuvres;
            // back off a little and swing round
            mReverseUntil = ctx.frame + 20 + static_cast<s32>(ctx.rng->Below(15));
            mFwdUntil = mReverseUntil + 40;
            mReverseSteer = (ctx.rng->Below(2) == 0) ? 1.0f : -1.0f;
            return Running;
        }
    }
    // Shut in: still within ten tiles of where it started, at the first waypoints, after 450 frames. Retrying the same drive from the same place
    // (three times 600 frames) only repeats it, so give the car up at once and remember the place
    {
        const float sdx = pl.x - mStartX;
        const float sdy = pl.y - mStartY;
        if (sdx * sdx + sdy * sdy > 10.0f * 10.0f || mWpIdx > 1)
        {
            mLeftStart = true;
        }
        if (!mLeftStart && dist > 15.0f && ctx.frame - ctx.action_start_frame > 600 && mRetries == 0 && mManoeuvres >= 2)
        {
            return Fail("no_progress", "boxed in: still within 10 tiles of (%.1f,%.1f) and at the first waypoints after 600 frames, %d reverse manoeuvres", mStartX, mStartY, mManoeuvres);
        }
    }
    const bool timed_out = ctx.frame - mProgressFrame > 600;
    if ((mStillFrames > 45 && mManoeuvres >= 4) || timed_out)
    {
        char detail[220];
        _snprintf(detail, sizeof(detail), "%s at (%.1f,%.1f), waypoint %d/%d (%.1f,%.1f), dist %.1f to (%.1f,%.1f), %d reverse manoeuvres",
                  timed_out ? "no waypoint reached for 600 frames" : "not moving", pl.x, pl.y, mWpIdx, mWpCount, mWpX[mWpIdx], mWpY[mWpIdx], dist, mTx, mTy, mManoeuvres);
        const float from_start = sqrtf((pl.x - mStartX) * (pl.x - mStartX) + (pl.y - mStartY) * (pl.y - mStartY));
        if (mRetries >= 3 || !ctx.action_snap || (from_start < 5.0f && mManoeuvres >= 4))
        {
            // wedged where it began: a restore would only repeat it, give the car up (the caller walks or takes another)
            return Fail("no_progress", "%s", detail);
        }
        mRetries++;
        ChooseVariant(ctx);
        {
            // avoid the place that held us: a tile or two ahead of the car, and the waypoint it was heading for
            const float heading = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
            ctx.road->BanArea(static_cast<int>(pl.x + sinf(heading) * 2.0f), static_cast<int>(pl.y - cosf(heading) * 2.0f), 1);
        }
        // the older checkpoint (the newer may be inside the same trouble), else the start of the action
        const s32 older = (mCkFrame[0] >= 0 && (mCkFrame[1] < 0 || mCkFrame[0] <= mCkFrame[1])) ? 0 : 1;
        Snapshot* restore_from = ctx.action_snap;
        s32 restore_frame = ctx.action_start_frame;
        if (mCkFrame[older] >= 0 && mCk[older])
        {
            restore_from = mCk[older];
            restore_frame = mCkFrame[older];
        }
        BotDecision("f=%d drive STUCK %s; retry %d: speed cap %.2f, waypoint radius %.1f, restore f=%d (%s)", ctx.frame, detail, mRetries, mSpeedCap, mWpRadius, restore_frame, restore_from == ctx.action_snap ? "the start" : "a checkpoint");
        restore_from->Restore();
        ctx.world->Observe();
        Plan(ctx, "retry");
        return Running;
    }
    if (mStillFrames > 45)
    {
        // back out and turn the other way
        mManoeuvres++;
        mStillFrames = 0;
        // a three point turn: back up with the wheel one way, then go forward with it the other way; the
        // lengths and the side vary from try to try
        mReverseUntil = ctx.frame + 25 + static_cast<s32>(ctx.rng->Below(45));
        mFwdUntil = mReverseUntil + 35 + static_cast<s32>(ctx.rng->Below(45));
        mReverseSteer = (ctx.rng->Below(2) == 0) ? 1.0f : -1.0f;
        {
            // what is in front: a wall, a pedestrian or a car
            const float hd = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
            const float fx = sinf(hd);
            const float fy = -cosf(hd);
            char what[160];
            what[0] = 0;
            for (int k = 0; k < ctx.world->car_count; k++)
            {
                const WorldModel::CarInfo& oc = ctx.world->cars[k];
                const float along = (oc.x - pl.x) * fx + (oc.y - pl.y) * fy;
                const float side = (oc.x - pl.x) * -fy + (oc.y - pl.y) * fx;
                if (along > 0.0f && along < 5.0f && fabsf(side) < 1.5f && strlen(what) < 100)
                {
                    _snprintf(what + strlen(what), 40, " car#%d(%.1f,%.1f%s%s)", oc.idx, along, side, oc.has_driver ? ",drv" : ",empty", oc.police ? ",cop" : "");
                }
            }
            BotDecision("f=%d drive: not moving at (%.1f,%.1f), reverse manoeuvre %d; ahead: wall %d/%d/%d tiles%s", ctx.frame, pl.x, pl.y, mManoeuvres,
                        WorldModel::BuildingAt(pl.x + fx * 1.2f, pl.y + fy * 1.2f), WorldModel::BuildingAt(pl.x + fx * 2.5f, pl.y + fy * 2.5f), WorldModel::BuildingAt(pl.x + fx * 4.0f, pl.y + fy * 4.0f), what);
        }
    }

    const bool last = mWpIdx == mWpCount - 1;
    float aim_x = mWpX[mWpIdx];
    float aim_y = mWpY[mWpIdx];
    {
        // Pure pursuit: aim a little way ahead on the route (further when faster) instead of at the waypoint
        // itself, which keeps the car on the leg between two waypoints and rounds the corners. The line from the
        // car to the aim point must have room for the car's width; if it clips the corner of a building the
        // look ahead is shortened (the aim moves back along the route, not along the line) until it does not
        const float ax0 = mWpIdx > 0 ? mWpX[mWpIdx - 1] : mPlanX;
        const float ay0 = mWpIdx > 0 ? mWpY[mWpIdx - 1] : mPlanY;
        const float sx = mWpX[mWpIdx] - ax0;
        const float sy = mWpY[mWpIdx] - ay0;
        const float sl2 = sx * sx + sy * sy;
        if (sl2 > 1.0f)
        {
            float t = ((pl.x - ax0) * sx + (pl.y - ay0) * sy) / sl2;
            t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
            const float qx = ax0 + sx * t;
            const float qy = ay0 + sy * t;
            const float sl = sqrtf(sl2);
            float look = 3.5f + mSpeed * 22.0f;
            look = look > 8.0f ? 8.0f : look;
            const float rem = sqrtf((mWpX[mWpIdx] - qx) * (mWpX[mWpIdx] - qx) + (mWpY[mWpIdx] - qy) * (mWpY[mWpIdx] - qy));
            for (int k = 0; k < 6; k++)
            {
                float lx = mWpX[mWpIdx];
                float ly = mWpY[mWpIdx];
                if (rem > look)
                {
                    lx = qx + sx / sl * look;
                    ly = qy + sy / sl * look;
                }
                else if (mWpIdx + 1 < mWpCount)
                {
                    const float nx = mWpX[mWpIdx + 1] - mWpX[mWpIdx];
                    const float ny = mWpY[mWpIdx + 1] - mWpY[mWpIdx];
                    const float nl = sqrtf(nx * nx + ny * ny);
                    if (nl > 0.01f)
                    {
                        const float rest = look - rem;
                        const float use = rest < nl ? rest : nl;
                        lx = mWpX[mWpIdx] + nx / nl * use;
                        ly = mWpY[mWpIdx] + ny / nl * use;
                    }
                }
                aim_x = lx;
                aim_y = ly;
                if (Drive_CorridorClear(pl.x, pl.y, lx, ly, 0.75f))
                {
                    break;
                }
                look *= 0.6f;
            }
        }
    }

    if (ctx.frame < mReverseUntil)
    {
        ctx.input = input_bit::Backward | (mReverseSteer > 0 ? input_bit::Right : input_bit::Left);
        return Running;
    }
    if (ctx.frame < mFwdUntil)
    {
        ctx.input = input_bit::Forward | (mReverseSteer > 0 ? input_bit::Left : input_bit::Right);
        return Running;
    }

    // Police after us: dodge a police car that is closing on the car, and keep the speed up
    if (pl.wanted_stars > 0)
    {
        if (ctx.frame < mSwerveUntil)
        {
            aim_x = mSwerveX;
            aim_y = mSwerveY;
        }
        else
        {
            for (int k = 0; k < ctx.world->car_count; k++)
            {
                const WorldModel::CarInfo& oc = ctx.world->cars[k];
                const float v = sqrtf(oc.vx * oc.vx + oc.vy * oc.vy);
                if (!oc.police || oc.dist < 1.5f || oc.dist > 8.0f || v < 0.08f)
                {
                    continue;
                }
                const float ux = oc.vx / v;
                const float uy = oc.vy / v;
                const float rx = pl.x - oc.x;
                const float ry = pl.y - oc.y;
                const float along = rx * ux + ry * uy;
                const float across = rx * -uy + ry * ux;
                if (along > 0.0f && along < 2.0f + v * 30.0f && fabsf(across) < 1.8f)
                {
                    const float sgn = across >= 0.0f ? 1.0f : -1.0f;
                    mSwerveX = pl.x + (-uy) * 4.0f * sgn + ux * 1.5f;
                    mSwerveY = pl.y + ux * 4.0f * sgn + uy * 1.5f;
                    if (!WorldModel::BuildingAt(mSwerveX, mSwerveY))
                    {
                        mSwerveUntil = ctx.frame + 18;
                        aim_x = mSwerveX;
                        aim_y = mSwerveY;
                        BotDecision("f=%d drive: police car #%d closing on us (%.1f tiles, speed %.2f): swerving", ctx.frame, oc.idx, oc.dist, v);
                    }
                    break;
                }
            }
        }
    }
    // Driving against the traffic of a lane: move over to the lane that goes our way
    if (dist > 8.0f && ctx.frame >= mSwerveUntil)
    {
        float lx = 0.0f;
        float ly = 0.0f;
        if (ctx.road->LaneSwitch(pl.x, pl.y, aim_x - pl.x, aim_y - pl.y, &lx, &ly) && !WorldModel::BuildingAt(lx, ly) && Drive_CorridorClear(pl.x, pl.y, lx, ly, 0.75f))
        {
            aim_x = lx;
            aim_y = ly;
        }
    }
    // A pedestrian in the way that is easy to avoid (room beside him) is steered round, whoever is watching
    {
        const float hd = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        const float fx = sinf(hd);
        const float fy = -cosf(hd);
        for (int k = 0; k < ctx.world->ped_count; k++)
        {
            const WorldModel::PedInfo& p = ctx.world->peds[k];
            if (p.dead || p.in_car)
            {
                continue;
            }
            const float along = (p.x - pl.x) * fx + (p.y - pl.y) * fy;
            const float across = (p.x - pl.x) * -fy + (p.y - pl.y) * fx;
            if (along > 1.0f && along < 6.0f && fabsf(across) < 0.9f)
            {
                const float side = across >= 0.0f ? -1.0f : 1.0f; // away from the side he stands on
                const float ax = p.x + (-fy) * side * 1.7f + fx * 1.5f;
                const float ay = p.y + fx * side * 1.7f + fy * 1.5f;
                const float mx = p.x + (-fy) * side * 1.7f;
                const float my = p.y + fx * side * 1.7f;
                // only when going round him still heads the way of the route: a plaza full of pedestrians must not
                // turn the car away from where it is going every few frames
                const float route_h = Bot_HeadingDeg(aim_x - pl.x, aim_y - pl.y);
                const float round_h = Bot_HeadingDeg(ax - pl.x, ay - pl.y);
                if (!WorldModel::BuildingAt(ax, ay) && !WorldModel::BuildingAt(mx, my) && fabsf(Bot_WrapDeg(round_h - route_h)) < 40.0f)
                {
                    aim_x = ax;
                    aim_y = ay;
                }
                break;
            }
        }
    }
    const float err = ctx.Steer(ctx.car, pl.rot_deg, pl.x, pl.y, aim_x, aim_y, true);
    // Which way to hold the wheel: try five arcs ahead and keep the one that ends nearest the aim point without
    // running into a building (or a car coming at us). Steering straight at the aim point is the first choice
    float wall_ahead = 1e3f;
    if (fabsf(err) <= 100.0f)
    {
        static const float options[5] = {0.0f, -1.0f, 1.0f, -0.5f, 0.5f};
        const float heading_deg = ctx.car.sign * pl.rot_deg + ctx.car.offset;
        const float hdir = ctx.car.sign * ctx.car.turn_sign;
        const float natural = fabsf(err) > 8.0f ? ((err * ctx.car.turn_sign > 0) ? 1.0f : -1.0f) : 0.0f;
        float best_s = natural;
        float best_cost = 1e9f;
        for (int o = 0; o < 5; o++)
        {
            const DriveArc arc = Drive_Arc(ctx, heading_deg, hdir, options[o], mSpeed, aim_x, aim_y);
            const float cost = arc.cost - (options[o] == natural ? 3.0f : 0.0f);
            if (cost < best_cost)
            {
                best_cost = cost;
                best_s = options[o];
                wall_ahead = arc.wall_d;
            }
        }
        ctx.input &= ~(input_bit::Left | input_bit::Right);
        if (best_s != 0.0f && (fabsf(best_s) > 0.9f || ctx.frame % 2 == 0))
        {
            ctx.input |= best_s > 0.0f ? input_bit::Right : input_bit::Left;
        }
        if (best_s != natural && ctx.frame % 20 == 0)
        {
            BotDecision("f=%d drive: steering %+.1f instead of %+.1f to keep clear of what is ahead (wall in %.1f tiles)", ctx.frame, best_s, natural, wall_ahead);
        }
    }
    if (ctx.frame < mShoveUntil)
    {
        ctx.input |= input_bit::Forward; // full throttle into the car in the way
        return Running;
    }
    float cap = mSpeedCap;
    if (pl.wanted_stars > 0 && cap < 0.30f)
    {
        cap = 0.30f; // running from the police: no dawdling
    }

    // Look ahead along the car's heading for things to hit: buildings, other cars, pedestrians
    {
        const float heading = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        const float hx = sinf(heading);
        const float hy = -cosf(heading);
        const float reach = 3.0f + mSpeed * 50.0f;
        float first_obstacle = -1.0f;
        bool obstacle_is_ped = false;
        for (float d = 1.2f; d <= reach && first_obstacle < 0.0f; d += 0.5f)
        {
            const float sx = pl.x + hx * d;
            const float sy = pl.y + hy * d;
            bool hit = false; // buildings are the arcs' business (above)
            // Other cars are not obstacles: stopping dead behind every parked or queuing car is what makes a
            // drive crawl, and a bump costs a few points of damage. Walls and pedestrians are.
            for (int i = 0; i < ctx.world->ped_count && !hit; i++)
            {
                const WorldModel::PedInfo& p = ctx.world->peds[i];
                if (!p.dead && !p.in_car && (p.x - sx) * (p.x - sx) + (p.y - sy) * (p.y - sy) < 0.8f * 0.8f)
                {
                    hit = true;
                    obstacle_is_ped = true;
                }
            }
            // a police car is no use to bump: it brings the wanted level
            for (int i = 0; i < ctx.world->car_count && !hit; i++)
            {
                const WorldModel::CarInfo& oc = ctx.world->cars[i];
                if (oc.police && (oc.x - sx) * (oc.x - sx) + (oc.y - sy) * (oc.y - sy) < 1.1f * 1.1f)
                {
                    hit = true;
                }
            }
            if (hit)
            {
                first_obstacle = d;
            }
        }
        if (first_obstacle > 0.0f)
        {
            // the speed that still lets the car stop before the obstacle: none inside 2.2 tiles
            float allowed = (first_obstacle - 1.4f) * 0.06f;
            allowed = allowed < 0.0f ? 0.0f : allowed;
            if (first_obstacle > 1.9f && allowed < 0.03f)
            {
                allowed = 0.03f; // keep creeping: stopping dead in front of every kerb and corner is what makes it crawl
            }

            if (obstacle_is_ped)
            {
                // Pedestrians: with no police about (or when already wanted, which a death cannot make worse) the car
                // goes through at full speed, after trying to steer round them; in sight of the police it only bumps
                // them at a crawl, since a death they see is a wanted level
                const bool police_see = pl.wanted_stars == 0 && ctx.world->CopsNear(25.0f);
                allowed = police_see ? 0.05f : cap;
            }
            cap = allowed < cap ? allowed : cap;
            if (ctx.frame % 40 == 0)
            {
                BotDecision("f=%d drive: obstacle %.1f tiles ahead at (%.1f,%.1f), speed %.2f -> allowed %.2f", ctx.frame, first_obstacle, pl.x, pl.y, mSpeed, cap);
            }
        }
    }
    if (wall_ahead < 1e3f)
    {
        // the speed that still lets the car stop before the building at the end of the best arc
        float allowed = (wall_ahead - 1.4f) * 0.06f;
        allowed = allowed < 0.0f ? 0.0f : allowed;
        if (wall_ahead > 2.5f && allowed < 0.03f)
        {
            allowed = 0.03f;
        }
        cap = allowed < cap ? allowed : cap;
    }
    if (last && dist < 14.0f)
    {
        cap *= 0.5f;
    }
    if (ctx.opts && ctx.opts->draw && (ctx.frame - ctx.action_start_frame) % 40 == 0 || ctx.frame - ctx.action_start_frame < 100 && (ctx.frame - ctx.action_start_frame) % 10 == 0)
    {
        BotDecision("f=%d drive state: at (%.1f,%.1f) speed %.3f cap %.2f, heading error %.0f to aim (%.1f,%.1f) on the way to waypoint %d/%d (%.1f,%.1f), %.0f tiles to go", ctx.frame, pl.x, pl.y, mSpeed, cap, err, aim_x, aim_y, mWpIdx, mWpCount, mWpX[mWpIdx], mWpY[mWpIdx], dist);
    }
    if (fabsf(err) <= 100.0f)
    {
        mBehindSince = 0;
    }
    if (fabsf(err) > 100.0f)
    {
        // The target is behind. Backing towards it works for a while (it is what a parked car does to
        // leave a kerb); if that has not turned the car in 120 frames, go forward with the wheel over
        // instead of backing off for ever.
        if (mBehindSince == 0)
        {
            mBehindSince = ctx.frame;
        }
        const s32 behind = ctx.frame - mBehindSince;
        const float bh = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        const bool room_behind = !WorldModel::BuildingAt(pl.x - sinf(bh) * 1.6f, pl.y + cosf(bh) * 1.6f) && !WorldModel::BuildingAt(pl.x - sinf(bh) * 2.8f, pl.y + cosf(bh) * 2.8f);
        if (room_behind && behind % 160 < 80)
        {
            ctx.input |= input_bit::Backward; // back towards it for 80 frames ...
        }
        else if (mSpeed < 0.10f)
        {
            ctx.input |= input_bit::Forward;  // ... then forward with the wheel over for 80, and again
        }
    }
    else if (fabsf(err) > 60.0f && mSpeed > 0.14f && ctx.frame >= mHandbrakeNext)
    {
        // a sharp turn at speed: handbrake and the wheel over swing the tail round
        mHandbrakeUntil = ctx.frame + 10;
        mHandbrakeNext = ctx.frame + 70;
        ctx.input |= input_bit::HandbrakeJump;
    }
    else if (ctx.frame < mHandbrakeUntil)
    {
        ctx.input |= input_bit::HandbrakeJump;
        if (mSpeed < 0.10f)
        {
            ctx.input |= input_bit::Forward;
        }
    }
    else if (fabsf(err) > 55.0f)
    {
        if (mSpeed > cap * 0.6f)
        {
            ctx.input |= input_bit::Backward; // brake for the turn
        }
        else
        {
            ctx.input |= input_bit::Forward;
        }
    }
    else if (mSpeed < cap)
    {
        ctx.input |= input_bit::Forward;
    }
    else if (mSpeed > cap * 1.5f + 0.01f)
    {
        ctx.input |= input_bit::Backward;
    }

    if ((ctx.frame - ctx.action_start_frame) % 10 == 0)
    {
        ctx.world->frame = ctx.frame;
        // goes to bot_trace.txt via the Bot (it prints ctx.last_trace)
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
ParkAction::ParkAction(float x, float y, float tol)
    : mTx(x)
    , mTy(y)
    , mTol(tol)
    , mStart(0)
    , mHavePrev(false)
    , mPrevX(0)
    , mPrevY(0)
    , mSpeed(0)
    , mSettled(0)
{
}

void ParkAction::Start(BotContext& ctx)
{
    mStart = ctx.frame;
    mHavePrev = false;
    mSettled = 0;
    if (!ctx.car.valid && ctx.foot.valid)
    {
        ctx.car = ctx.foot;
    }
}

Action::Status ParkAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (!pl.in_car)
    {
        return Fail("action_failed", "park: the player is not in a car");
    }
    if (mHavePrev)
    {
        const float mx = pl.x - mPrevX;
        const float my = pl.y - mPrevY;
        mSpeed = sqrtf(mx * mx + my * my);
    }
    mPrevX = pl.x;
    mPrevY = pl.y;
    mHavePrev = true;

    const float dx = mTx - pl.x;
    const float dy = mTy - pl.y;
    const float dist = sqrtf(dx * dx + dy * dy);
    if (dist <= mTol)
    {
        // on the spot: hold the brake until the car has stopped
        if (mSpeed < 0.004f && ++mSettled > 8)
        {
            BotDecision("f=%d park: on the spot (%.2f,%.2f), %.2f tiles from (%.2f,%.2f)", ctx.frame, pl.x, pl.y, dist, mTx, mTy);
            return Done;
        }
        ctx.input = mSpeed > 0.004f ? input_bit::Backward : 0;
        return Running;
    }
    mSettled = 0;
    if (ctx.frame - mStart > 900)
    {
        BotDecision("f=%d park: giving up %.2f tiles from (%.2f,%.2f)", ctx.frame, dist, mTx, mTy);
        return Done; // as close as it will get: the caller carries on
    }

    // heading error to the spot; the car turns only while it rolls, so creep in pulses
    const float err = ctx.Steer(ctx.car, pl.rot_deg, pl.x, pl.y, mTx, mTy, false);
    const bool ahead = fabsf(err) < 90.0f;
    const float want_speed = dist > 3.0f ? 0.08f : (dist > 1.0f ? 0.03f : 0.012f);
    if (ahead)
    {
        if (mSpeed < want_speed && (ctx.frame % 3) == 0)
        {
            ctx.input |= input_bit::Forward;
        }
        else if (mSpeed > want_speed * 1.6f)
        {
            ctx.input |= input_bit::Backward;
        }
    }
    else
    {
        // it is behind us: back up towards it (the steering turns the other way in reverse)
        ctx.input &= ~(input_bit::Left | input_bit::Right);
        ctx.input |= (ctx.frame % 3) == 0 ? input_bit::Backward : 0;
        if (mSpeed > 0.05f)
        {
            ctx.input = input_bit::Forward; // brake
        }
    }
    return Running;
}
