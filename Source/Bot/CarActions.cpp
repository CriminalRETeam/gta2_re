#include "CarActions.hpp"
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

bool DriveAction::Plan(BotContext& ctx, const char* why)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    // A clear straight line is the quickest way: no detour to the roads and their arrows
    const float ddx = mTx - pl.x;
    const float ddy = mTy - pl.y;
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
    mHavePrev = false;
    mBestWpDist = 1e9f;
    mLastWpIdx = -1;
    BotDecision("f=%d drive plan (%s) (%.1f,%.1f)->(%.0f,%.0f): %s, %d waypoints, %u tiles explored (cap %.2f, radius %.1f)", ctx.frame, why, pl.x, pl.y, mTx, mTy,
                direct ? "direct line" : (found ? "path" : "NO PATH, driving straight"), mWpCount, ctx.road->Expanded(), mSpeedCap, mWpRadius);
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
        BotDecision("f=%d drive STUCK %s; retry %d: speed cap %.2f, waypoint radius %.1f, restore the action start (f=%d)", ctx.frame, detail, mRetries, mSpeedCap, mWpRadius, ctx.action_start_frame);
        ctx.action_snap->Restore();
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
        BotDecision("f=%d drive: not moving at (%.1f,%.1f), reverse manoeuvre %d", ctx.frame, pl.x, pl.y, mManoeuvres);
    }

    const bool last = mWpIdx == mWpCount - 1;
    float aim_x = mWpX[mWpIdx];
    float aim_y = mWpY[mWpIdx];
    {
        // Pure pursuit: aim a little way ahead on the route (further when faster) instead of at the waypoint
        // itself, which keeps the car on the leg between two waypoints and rounds the corners
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
            if (rem > look)
            {
                aim_x = qx + sx / sl * look;
                aim_y = qy + sy / sl * look;
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
                    aim_x = mWpX[mWpIdx] + nx / nl * use;
                    aim_y = mWpY[mWpIdx] + ny / nl * use;
                }
            }
        }
    }

    {
        // The line to the aim point must be clear of buildings with room for the car's width, or the car clips a
        // corner: pull the aim point back towards the car until it is
        const float ex = aim_x - pl.x;
        const float ey = aim_y - pl.y;
        const float el = sqrtf(ex * ex + ey * ey);
        if (el > 2.0f)
        {
            const float px = -ey / el * 0.7f;
            const float py = ex / el * 0.7f;
            float f = 1.0f;
            for (int k = 0; k < 4; k++)
            {
                bool clear = true;
                for (float d = 1.5f; d <= el * f && clear; d += 0.5f)
                {
                    const float qx = pl.x + ex / el * d;
                    const float qy = pl.y + ey / el * d;
                    clear = !WorldModel::BuildingAt(qx + px, qy + py) && !WorldModel::BuildingAt(qx - px, qy - py);
                }
                if (clear)
                {
                    break;
                }
                f *= 0.6f;
            }
            if (f < 1.0f && el * f > 2.0f)
            {
                aim_x = pl.x + ex * f;
                aim_y = pl.y + ey * f;
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

    const float err = ctx.Steer(ctx.car, pl.rot_deg, pl.x, pl.y, aim_x, aim_y, true);
    float cap = mSpeedCap;

    // Look ahead along the car's heading for things to hit: buildings, other cars, pedestrians
    {
        const float heading = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        const float hx = sinf(heading);
        const float hy = -cosf(heading);
        const float reach = 3.0f + mSpeed * 50.0f;
        float first_obstacle = -1.0f;
        for (float d = 1.2f; d <= reach && first_obstacle < 0.0f; d += 0.5f)
        {
            const float sx = pl.x + hx * d;
            const float sy = pl.y + hy * d;
            bool hit = WorldModel::BuildingAt(sx, sy);
            // Other cars are not obstacles: stopping dead behind every parked or queuing car is what makes a
            // drive crawl, and a bump costs a few points of damage. Walls and pedestrians are.
            for (int i = 0; i < ctx.world->ped_count && !hit; i++)
            {
                const WorldModel::PedInfo& p = ctx.world->peds[i];
                if (!p.dead && !p.in_car && (p.x - sx) * (p.x - sx) + (p.y - sy) * (p.y - sy) < 0.8f * 0.8f)
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

            cap = allowed < cap ? allowed : cap;
            if (ctx.frame % 40 == 0)
            {
                BotDecision("f=%d drive: obstacle %.1f tiles ahead at (%.1f,%.1f), speed %.2f -> allowed %.2f", ctx.frame, first_obstacle, pl.x, pl.y, mSpeed, cap);
            }
        }
    }
    if (last && dist < 14.0f)
    {
        cap *= 0.5f;
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
        if (behind % 160 < 80)
        {
            ctx.input |= input_bit::Backward; // back towards it for 80 frames ...
        }
        else if (mSpeed < 0.10f)
        {
            ctx.input |= input_bit::Forward;  // ... then forward with the wheel over for 80, and again
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
