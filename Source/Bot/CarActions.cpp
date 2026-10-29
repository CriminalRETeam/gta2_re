#include "CarActions.hpp"
#include "CarIntercept.hpp"
#include "DropOff.hpp"
#include "Horn.hpp"
#include "JamSwitch.hpp"
#include "NavFoot.hpp"
#include "WalkCarJump.hpp"
#include "DriveSteer.hpp"
#include "LocalPlan.hpp"
#include "BotDebugDraw.hpp"
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
    , mStand(NULL)
    , mStandStart(-1)
    , mStandRetarget(0)
    , mStandThere(false)
    , mDriveOffs(0)
    , mSampleFrame(-1)
    , mSampleX(0.0f)
    , mSampleY(0.0f)
    , mCarVx(0.0f)
    , mCarVy(0.0f)
    , mPressing(false)
    , mHopped(false)
    , mHopStart(-1)
    , mHopJump(-1)
    , mPressStart(0)
    , mStable(0)
{
}

EnterCarAction::~EnterCarAction()
{
    delete mWalk;
    delete mStand;
}

void EnterCarAction::Start(BotContext& /*ctx*/)
{
    delete mWalk;
    mWalk = NULL;
    delete mStand;
    mStand = NULL;
    mStandStart = -1;
    mStandThere = false;
    mPressing = false;
    mHopped = false;
    mHopStart = -1;
    mHopJump = -1;
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
    if ((mPressing || mHopStart >= 0) && car.has_driver && car.speed > 0.05f && mCarVx * mCarVx + mCarVy * mCarVy > 0.0009f)
    {
        if (mDriveOffs >= 2)
        {
            return Fail("action_failed", "enter_car: car #%d drove off (speed %.2f)", mCarId, car.speed);
        }
        // it pulled away while we were at its door: get ahead of it and stop it
        mDriveOffs++;
        BotDecision("f=%d enter_car: car #%d drove off (speed %.2f): getting ahead of it (%d)", ctx.frame, mCarId, car.speed, mDriveOffs);
        mPressing = false;
        mHopStart = -1;
        mHopJump = -1;
        mHopped = false;
        delete mWalk;
        mWalk = NULL;
        delete mStand;
        mStand = NULL;
        mStandStart = -1;
        mStandThere = false;
        return Running;
    }

    // The car's velocity from where it was a few frames ago (the world's one-frame difference is 0 when it was not observed last frame)
    if (mSampleFrame < 0 || ctx.frame - mSampleFrame >= 4)
    {
        if (mSampleFrame >= 0)
        {
            mCarVx = (car.x - mSampleX) / static_cast<float>(ctx.frame - mSampleFrame);
            mCarVy = (car.y - mSampleY) / static_cast<float>(ctx.frame - mSampleFrame);
        }
        mSampleX = car.x;
        mSampleY = car.y;
        mSampleFrame = ctx.frame;
    }
    // A moving car with a driver drives off from anyone walking at it. Traffic stops for a pedestrian in its lane: get ahead of it
    // on its line, stand there facing it, and go up to it when it has stopped
    if (!mPressing && mHopStart < 0 && !mHopped && car.has_driver)
    {
        InterceptIn in;
        in.car_x = car.x;
        in.car_y = car.y;
        in.car_vx = mCarVx;
        in.car_vy = mCarVy;
        in.ped_x = pl.x;
        in.ped_y = pl.y;
        in.ped_speed = 0.06f;
        float sx = 0.0f;
        float sy = 0.0f;
        const int plan = CarIntercept_Plan(in, &sx, &sy);
        if (plan != kInterceptStopped && mWalk)
        {
            // it drove off while we walked to it: not after it, ahead of it
            delete mWalk;
            mWalk = NULL;
        }
        if (plan == kInterceptNever && mStandStart < 0)
        {
            return Fail("action_failed", "enter_car: car #%d is moving too fast (%.2f) to get ahead of it", mCarId, car.speed);
        }
        if (plan == kInterceptAhead || mStand)
        {
            if (!mStand)
            {
                mStand = new WalkAction(sx, sy, false, 0.7f);
                mStand->Start(ctx);
                mStandRetarget = ctx.frame;
                mStandStart = ctx.frame;
                BotDecision("f=%d enter_car: car #%d is moving (%.2f): standing in its lane ahead at (%.1f,%.1f) to stop it", ctx.frame, mCarId, car.speed, sx, sy);
            }
            else if (plan == kInterceptAhead && ctx.frame - mStandRetarget >= 15)
            {
                mStand->SetTarget(sx, sy);
                mStandRetarget = ctx.frame;
            }
            if (ctx.frame - mStandStart > 420)
            {
                return Fail("action_failed", "enter_car: car #%d did not stop for 420 frames", mCarId);
            }
            if (plan == kInterceptStopped)
            {
                BotDecision("f=%d enter_car: car #%d has stopped (dist %.1f): going up to it", ctx.frame, mCarId, car.dist);
                delete mStand;
                mStand = NULL;
                mStandStart = ctx.frame; // (so a car that drives off again is not walked after for ever either)
            }
            else
            {
                ctx.no_evade_frame = ctx.frame; // standing in the lane on purpose
                if (!mStandThere)
                {
                    const Status ws = mStand->Step(ctx);
                    if (ws == Done)
                    {
                        mStandThere = true;
                    }
                    else if (ws == Failed)
                    {
                        return Fail(mStand->fail_condition, "enter_car: stand ahead: %s", mStand->fail_detail);
                    }
                }
                else
                {
                    // there: face the car and wait (turning only, no walking). If the car's line has moved off, walk to the new spot
                    ctx.input = 0;
                    ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, car.x, car.y, true);
                    ctx.input &= ~static_cast<u32>(input_bit::Forward);
                    if (plan == kInterceptAhead && (pl.x - sx) * (pl.x - sx) + (pl.y - sy) * (pl.y - sy) > 2.5f * 2.5f)
                    {
                        mStandThere = false;
                        mStand->SetTarget(sx, sy);
                        mStand->Start(ctx);
                    }
                }
                return Running;
            }
        }
    }

    if (!mPressing && mHopStart < 0)
    {
        if (!mWalk)
        {
            mWalk = new WalkAction(car.x, car.y, false, 2.2f);
            mWalk->SetStealOnJump(true); // a car jumped over on the way to this one is as good
            mWalk->Start(ctx);
        }
        const Status s = mWalk->Step(ctx);
        if (s == Failed)
        {
            return Fail(mWalk->fail_condition, "enter_car: %s", mWalk->fail_detail);
        }
        if (s == Done)
        {
            if (!mHopped)
            {
                // Standing on the car and pressing enter takes it nearly every time (a driver is pulled out, a parked
                // car just opened): hop onto it first, and fall back to pressing at its side if that did not do it
                mHopped = true;
                mHopStart = ctx.frame;
                BotDecision("f=%d enter_car: next to car #%d at (%.1f,%.1f), hopping onto it", ctx.frame, mCarId, car.x, car.y);
                delete mWalk;
                mWalk = NULL;
            }
            else
            {
                mPressing = true;
                mPressStart = ctx.frame;
                BotDecision("f=%d enter_car: next to car #%d at (%.1f,%.1f), pressing enter", ctx.frame, mCarId, car.x, car.y);
            }
        }
        if (mHopStart < 0)
        {
            return Running;
        }
    }
    if (mHopStart >= 0 && !mPressing)
    {
        ctx.input = 0;
        const s32 t = ctx.frame - mHopStart;
        // not while the ped is in the middle of climbing in (state other than 0): that takes about 40 frames and any key cancels it
        if (t > 260 || (pl.ped_state == 0 && (t > 70 || (mHopJump >= 0 && ctx.frame - mHopJump > 26))))
        {
            mHopStart = -1;
            mPressing = true;
            mPressStart = ctx.frame;
            BotDecision("f=%d enter_car: the hop did not take car #%d, pressing enter at its side", ctx.frame, mCarId);
            return Running;
        }
        // face the car's middle, walk up to it, jump from 1.7 tiles off and press enter while above it
        const float herr = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, car.x, car.y, true);
        if (mHopJump < 0)
        {
            if (fabsf(herr) < 30.0f)
            {
                ctx.input |= input_bit::Forward;
            }
            if (car.dist < 1.7f && fabsf(herr) < 30.0f)
            {
                mHopJump = ctx.frame;
                ctx.input |= input_bit::HandbrakeJump;
            }
            return Running;
        }
        const s32 j = ctx.frame - mHopJump;
        if (j % 6 == 0)
        {
            BotDecision("f=%d hop: j=%d dist to car %.2f err %.0f in_car %d ped_state %d z %.2f", ctx.frame, j, car.dist, herr, pl.in_car, pl.ped_state, pl.z);
        }
        if (j >= 4 && pl.ped_state != 0)
        {
            ctx.input = 0; // the enter began: climbing in takes its time, and any key cancels it
            return Running;
        }
        if (j >= 4 && car.dist < 1.3f && !car.police && !car.locked)
        {
            // over the car: only the enter key, in pulses; any other key (walking on, turning) cancels the steal
            ctx.input = WalkCarJump_StealNow(j, car.dist, car.police, car.locked) ? input_bit::EnterExit : 0;
            return Running;
        }
        if (fabsf(herr) < 60.0f)
        {
            ctx.input |= input_bit::Forward;
        }
        if (j < 3)
        {
            ctx.input |= input_bit::HandbrakeJump;
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
    , mJamSwitch(false)
    , mJamStill(0)
    , mProgWpIdx(-1)
    , mProgWpDist(0.0f)
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
    , mLocalNext(0)
    , mPrevSteer(0)
    , mTraceFrame(-1000)
    , mRouteAheadN(0)
    , mLocalFallback(false)
    , mOvertakeUntil(0)
    , mOvertakeX(0)
    , mOvertakeY(0)
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
    , mChase(false)
{
    mCk[0] = mCk[1] = NULL;
    mCkFrame[0] = mCkFrame[1] = -1;
}

void DriveAction::Detail(char* buf, int size) const
{
    _snprintf(buf, size, "driving to (%.0f,%.0f), stop within %.1f%s", mTx, mTy, mStopDist, mChase ? ", chasing" : "");
    buf[size - 1] = 0;
}

DriveAction::~DriveAction()
{
    delete mCk[0];
    delete mCk[1];
}

void DriveAction::Start(BotContext& ctx)
{
    TurnAround_Reset(&mTurn);
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
    mOvertakeUntil = 0;
    mLocal.found = false;
    mLocalNext = 0;
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

// the room kept round our own car's box when steering past buildings and cars, in tiles
static const float kOwnMargin = 0.25f;

// The other cars as the steering and the planner see them: the game's collision boxes (a plain box where it has none)
static int Drive_CollectCars(BotContext& ctx, SteerCar* out)
{
    int n = 0;
    for (int k = 0; k < ctx.world->car_count; k++)
    {
        const WorldModel::CarInfo& oc = ctx.world->cars[k];
        if (DriveSteer_IsOwnCar(oc.idx, ctx.world->player.car_id, ctx.world->player.in_car))
        {
            continue;
        }
        SteerCar& sc = out[n++];
        sc.x = oc.x;
        sc.y = oc.y;
        sc.vx = oc.vx;
        sc.vy = oc.vy;
        sc.speed = oc.speed;
        sc.police = oc.police;
        sc.box.cx = oc.x;
        sc.box.cy = oc.y;
        sc.box.ax = oc.box.valid ? oc.box.ax : 1.0f;
        sc.box.ay = oc.box.valid ? oc.box.ay : 0.0f;
        sc.box.hl = oc.box.valid ? oc.box.hl : 1.0f;
        sc.box.hw = oc.box.valid ? oc.box.hw : 0.5f;
    }
    return n;
}

static int sRampZ = 2;
// 0 none, else the axis of the ramp the tile is on or one of the two tiles before it (1 along x, 2 along y), plus 4 on the ramp itself
static int Drive_RampZone(void*, float x, float y)
{
    const int tx = static_cast<int>(x);
    const int ty = static_cast<int>(y);
    const int here = WorldModel::SlopeAxisAt(tx, ty, sRampZ);
    if (here)
    {
        return here | 4;
    }
    for (int k = -2; k <= 2; k++)
    {
        if (k != 0 && WorldModel::SlopeAxisAt(tx, ty + k, sRampZ) == 2)
        {
            return 2;
        }
        if (k != 0 && WorldModel::SlopeAxisAt(tx + k, ty, sRampZ) == 1)
        {
            return 1;
        }
    }
    return 0;
}

static bool Drive_EdgeOk(void* user, int x, int y, int nx, int ny)
{
    return static_cast<NavCar*>(user)->EdgeOpen(x, y, nx, ny);
}

// The pedestrians near the car as obstacles for the detailed route: each one's collision box (a small square if the game gave none), where it
// is now. A pedestrian on the road is gone round when there is room, as a standing car is; with no way round the route goes through
static int Drive_CollectPeds(BotContext& ctx, SteerCar* out, int room)
{
    int n = 0;
    for (int k = 0; k < ctx.world->ped_count && n < room; k++)
    {
        const WorldModel::PedInfo& p = ctx.world->peds[k];
        if (p.dead || p.in_car || p.dist > 20.0f || p.x <= 0.0f)
        {
            continue;
        }
        out[n++] = DriveSteer_PedAsCar(p.x, p.y, p.box.valid, p.box.ax, p.box.ay, p.box.hl, p.box.hw);
    }
    return n;
}

// Against the traffic of a lane costs the planner extra
static float Drive_LaneCost(void* user, float x, float y, int dx, int dy)
{
    return static_cast<NavCar*>(user)->LaneAgainst(x, y, dx, dy) ? 3.5f : 0.0f;
}

// the room the look-ahead keeps round a pedestrian's box, in tiles
static const float kPedMargin = 0.3f;

static SteerBox Drive_ToSteerBox(const WorldModel::BoxInfo& b)
{
    SteerBox s;
    s.cx = b.cx;
    s.cy = b.cy;
    s.ax = b.ax;
    s.ay = b.ay;
    s.hl = b.hl;
    s.hw = b.hw;
    return s;
}

// Free tiles beyond the nose (dir 1) or the tail (dir -1) of the car along its heading: buildings, walls and other cars,
// on the centre line and the two sides of the car
static float Drive_RoomAlong(float x, float y, float heading, float dir, float hl, float hw, const SteerCar* cars, int car_count, char* what = NULL)
{
    const float kLook = 3.0f;
    const float kStep = 0.15f;
    const float hx = sinf(heading) * dir;
    const float hy = -cosf(heading) * dir;
    float best = kLook;
    for (int lat = -1; lat <= 1; lat++)
    {
        for (float d = kStep; d < kLook; d += kStep)
        {
            const float px = x + hx * (hl + d) - hy * hw * lat * dir;
            const float py = y + hy * (hl + d) + hx * hw * lat * dir;
            char kind = 0;
            if (WorldModel::BuildingAt(px, py))
            {
                kind = 'B';
            }
            else if (WorldModel::LowWallAt(static_cast<int>(px), static_cast<int>(py)))
            {
                kind = 'W';
            }
            for (int i = 0; i < car_count && !kind; i++)
            {
                kind = SteerBox_Contains(cars[i].box, px, py, 0.1f) ? 'C' : 0;
            }
            if (kind)
            {
                if (d < best && what)
                {
                    _snprintf(what, 24, "%c at (%.1f,%.1f)", kind, px, py);
                }
                best = d < best ? d : best;
                break;
            }
        }
    }
    return best;
}

static bool Drive_Solid(void* /*user*/, float x, float y)
{
    // a building, water, or a tile with a fence or low wall standing on it (cars cannot cross those either)
    return WorldModel::BuildingAt(x, y) || WorldModel::LowWallAt(static_cast<int>(x), static_cast<int>(y));
}

// Length of the way from (x, y) through the waypoints
static float Drive_RouteLength(float x, float y, const float* wx, const float* wy, int n)
{
    float len = 0.0f;
    for (int i = 0; i < n; i++)
    {
        len += sqrtf((wx[i] - x) * (wx[i] - x) + (wy[i] - y) * (wy[i] - y));
        x = wx[i];
        y = wy[i];
    }
    return len;
}

static const float kApproxStop = 2.5f;    // tiles: how near to the nearest road point to a goal the car cannot reach the drive goes
// Length of the router's last route from (x, y)
static float Drive_RouteLength(float x, float y, const NavCar* road)
{
    float len = 0.0f;
    for (u32 i = 1; i < road->Count(); i++)
    {
        len += sqrtf((road->X(i) - x) * (road->X(i) - x) + (road->Y(i) - y) * (road->Y(i) - y));
        x = road->X(i);
        y = road->Y(i);
    }
    return len;
}

// Tiles of walking from a road point to the goal; -1 when there is no way on foot. A route that only gets near the goal (its spot is
// on a tile the walker cannot enter, a phone in an alcove) counts: every candidate has the same goal, so they compare with each other
static float Drive_WalkLength(BotContext& ctx, float x, float y, float gx, float gy)
{
    if (!ctx.nav->FindPath(static_cast<u8>(x), static_cast<u8>(y), static_cast<u8>(ctx.world->player.z + 0.5f), static_cast<u8>(gx), static_cast<u8>(gy)))
    {
        return -1.0f;
    }
    return static_cast<float>(ctx.nav->PathLength());
}

// Candidate drop-off points: rings round the goal of this many points, from this far out, this far apart
static const int kDropCandidates = 14;
static const int kDropRings = 4;
static const int kDropAngles = 12;
static const float kDropFirstRing = 8.0f;
static const float kDropRingStep = 7.0f;
static const float kDropSeparate = 4.0f;

static const float kSolidGoalStop = 6.0f; // tiles: how near a goal inside a building is reached
static const float kPartialEndNear = 2.0f; // tiles from the end of a partial local route that count as being there
static const float kProgressTiles = 2.0f; // getting this much nearer the waypoint counts as progress
static const float kOnLegTiles = 1.2f; // within this of a later leg of the route: the earlier corners are behind us
static const int kReplanFrames = 15; // the detailed route is planned afresh this often: the other cars move
static const float kBehindCos = -0.17f; // the first waypoint is more than 100 degrees off the nose
static const float kAheadLead = 4.0f;   // tiles ahead of the nose where the route from the front is started
static const float kUTurnCost = 12.0f;  // tiles a three point turn is worth

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
    // The route starts behind the nose: turning round costs a three point turn, so a route that goes on ahead and comes back
    // round at the next junction is taken when it is not much longer
    if (found && !ctx.road->Approximate() && mWpCount > 0 && ctx.car.valid && pl.in_car)
    {
        const float th = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        const float hx = sinf(th);
        const float hy = -cosf(th);
        const float wx = mWpX[0] - pl.x;
        const float wy = mWpY[0] - pl.y;
        const float wl = sqrtf(wx * wx + wy * wy);
        if (wl > 0.5f && (wx * hx + wy * hy) / wl < kBehindCos)
        {
            float base = Drive_RouteLength(pl.x, pl.y, mWpX, mWpY, mWpCount);
            const float ax = pl.x + hx * kAheadLead;
            const float ay = pl.y + hy * kAheadLead;
            float alt_x[kMaxWp];
            float alt_y[kMaxWp];
            int alt_n = 0;
            if (!WorldModel::BuildingAt(ax, ay) && ctx.road->FindPath(ax, ay, mTx, mTy) && !ctx.road->Approximate())
            {
                alt_x[alt_n] = ax;
                alt_y[alt_n] = ay;
                alt_n++;
                for (u32 i = 1; i < ctx.road->Count() && alt_n < kMaxWp - 1; i++)
                {
                    alt_x[alt_n] = ctx.road->X(i);
                    alt_y[alt_n] = ctx.road->Y(i);
                    alt_n++;
                }
            }
            const float alt = alt_n > 0 ? Drive_RouteLength(pl.x, pl.y, alt_x, alt_y, alt_n) : 1e9f;
            if (alt < base + kUTurnCost)
            {
                for (int i = 0; i < alt_n; i++)
                {
                    mWpX[i] = alt_x[i];
                    mWpY[i] = alt_y[i];
                }
                mWpCount = alt_n;
                BotDecision("f=%d drive plan: the route starts behind the nose; going on ahead and round instead (%.0f tiles against %.0f and a U-turn)", ctx.frame, alt, base);
            }
            else
            {
                ctx.road->FindPath(pl.x, pl.y, mTx, mTy); // back to the route that was kept
            }
        }
    }
    bool approximated = false;
    if (found && ctx.road->Approximate() && mWpCount > 0)
    {
        // the target cannot be reached by car: drive to a road point and walk the rest. Not the point nearest in a straight line, which
        // may be on the wrong side of a block: the point that makes the whole trip shortest (DropOff.hpp)
        const float goal_x = mTx;
        const float goal_y = mTy;
        DropOffCand cands[kDropCandidates];
        int cand_count = 0;
        cands[cand_count].x = mWpX[mWpCount - 1];
        cands[cand_count].y = mWpY[mWpCount - 1];
        cands[cand_count].car_len = Drive_RouteLength(pl.x, pl.y, mWpX, mWpY, mWpCount);
        cands[cand_count].walk_len = Drive_WalkLength(ctx, cands[cand_count].x, cands[cand_count].y, goal_x, goal_y);
        cand_count++;
        for (int ring = 0; ring < kDropRings && cand_count < kDropCandidates; ring++)
        {
            for (int a = 0; a < kDropAngles && cand_count < kDropCandidates; a++)
            {
                const float ang = a * 6.2831853f / kDropAngles;
                const float r = kDropFirstRing + ring * kDropRingStep;
                float rx = 0.0f;
                float ry = 0.0f;
                if (!ctx.road->NearestLane(goal_x + sinf(ang) * r, goal_y - cosf(ang) * r, goal_x, goal_y, 3.0f, &rx, &ry))
                {
                    continue;
                }
                bool dup = false;
                for (int k = 0; k < cand_count; k++)
                {
                    dup = dup || (cands[k].x - rx) * (cands[k].x - rx) + (cands[k].y - ry) * (cands[k].y - ry) < kDropSeparate * kDropSeparate;
                }
                if (dup)
                {
                    continue;
                }
                cands[cand_count].x = rx;
                cands[cand_count].y = ry;
                cands[cand_count].walk_len = Drive_WalkLength(ctx, rx, ry, goal_x, goal_y);
                cands[cand_count].car_len = -1.0f;
                const bool walkable = cands[cand_count].walk_len >= 0.0f && cands[cand_count].walk_len < cands[0].walk_len;
                const bool drivable = walkable && ctx.road->FindPath(pl.x, pl.y, rx, ry) && !ctx.road->Approximate();
                BotDecision("f=%d drive plan: drop-off candidate (%.0f,%.0f): %.0f tiles of walking to the goal%s", ctx.frame, rx, ry, cands[cand_count].walk_len, !walkable ? " (no better than the nearest road point)" : (drivable ? "" : ", the car cannot get there"));
                if (drivable)
                {
                    cands[cand_count].car_len = Drive_RouteLength(pl.x, pl.y, ctx.road);
                    cand_count++;
                }
            }
        }
        int pick = DropOff_Choose(cands, cand_count);
        if (pick > 0)
        {
            BotDecision("f=%d drive plan: leaving the car at (%.0f,%.0f) (%.0f tiles of road, %.0f of walking) not at the nearest road point (%.0f,%.0f) (%.0f of road, %.0f of walking); %d points tried", ctx.frame, cands[pick].x,
                        cands[pick].y, cands[pick].car_len, cands[pick].walk_len, cands[0].x, cands[0].y, cands[0].car_len, cands[0].walk_len, cand_count);
            ctx.road->FindPath(pl.x, pl.y, cands[pick].x, cands[pick].y);
            mWpCount = 0;
            for (u32 i = 1; i < ctx.road->Count() && mWpCount < kMaxWp - 1; i++)
            {
                mWpX[mWpCount] = ctx.road->X(i);
                mWpY[mWpCount] = ctx.road->Y(i);
                mWpCount++;
            }
        }
        else
        {
            pick = 0;
            ctx.road->FindPath(pl.x, pl.y, goal_x, goal_y); // the router's state is that of the route that is kept
        }
        mTx = pick > 0 ? cands[pick].x : mWpX[mWpCount - 1];
        mTy = pick > 0 ? cands[pick].y : mWpY[mWpCount - 1];
        approximated = true;
        if (mStopDist > kApproxStop)
        {
            BotDecision("f=%d drive plan: the goal cannot be reached by car; driving to the nearest road point (%.1f,%.1f) and up to it (within %.1f, not %.1f)", ctx.frame, mTx, mTy, kApproxStop, mStopDist);
            mStopDist = kApproxStop;
        }
    }
    if (mWpCount == 0 || mWpX[mWpCount - 1] != mTx || mWpY[mWpCount - 1] != mTy)
    {
        mWpX[mWpCount] = mTx;
        mWpY[mWpCount] = mTy;
        mWpCount++;
    }
    // A goal inside a building cannot be reached: it is reached when the car is as near as the roads go
    if (!approximated && Drive_Solid(NULL, mTx, mTy) && mStopDist < kSolidGoalStop)
    {
        BotDecision("f=%d drive plan: the goal (%.0f,%.0f) is inside something solid: reached within %.0f tiles", ctx.frame, mTx, mTy, kSolidGoalStop);
        mStopDist = kSolidGoalStop;
    }
    mWpIdx = 0;
    mPlanX = pl.x;
    mPlanY = pl.y;
    mProgressFrame = ctx.frame;
    mProgWpIdx = -1;
    mStillFrames = 0;
    mReverseUntil = 0;
    mManoeuvres = 0;
    mBlockedFrames = 0;
    mHavePrev = false;
    mBestWpDist = 1e9f;
    mLastWpIdx = -1;
    BotDecision("f=%d drive plan (%s) (%.1f,%.1f)->(%.0f,%.0f): %s, %d waypoints, %u tiles explored, %s (cap %.2f, radius %.1f)", ctx.frame, why, pl.x, pl.y, mTx, mTy,
                direct ? "direct line" : (found ? "path" : "NO PATH, driving straight"), mWpCount, ctx.road->Expanded(), ctx.road->Clearance() ? "room each side" : "SQUEEZED (no route with room each side)", mSpeedCap, mWpRadius);
    BotDecision("f=%d boxes: ours %s half length %.2f half width %.2f; %d cars about", ctx.frame, pl.box.valid ? "valid" : "MISSING", pl.box.hl, pl.box.hw, ctx.world->car_count);
    for (int k = 0; k < ctx.world->car_count && k < 3; k++)
    {
        const WorldModel::CarInfo& oc = ctx.world->cars[k];
        BotDecision("f=%d boxes: car #%d model %d %s half length %.2f half width %.2f centre (%.2f,%.2f) sprite (%.2f,%.2f)", ctx.frame, oc.idx, oc.model, oc.box.valid ? "valid" : "MISSING", oc.box.hl, oc.box.hw, oc.box.cx, oc.box.cy, oc.x, oc.y);
    }
    for (int k = 0; k < ctx.world->ped_count && k < 2; k++)
    {
        const WorldModel::PedInfo& op = ctx.world->peds[k];
        BotDecision("f=%d boxes: ped #%d %s half length %.2f half width %.2f", ctx.frame, op.id, op.box.valid ? "valid" : "MISSING", op.box.hl, op.box.hw);
    }
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

void DriveAction::Retarget(BotContext& ctx, float x, float y)
{
    mTx = x;
    mTy = y;
    Plan(ctx, "retarget");
    mProgressFrame = ctx.frame;
    mProgWpIdx = -1;
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
    // our car's collision box: the game's own, the same size the steering tests
    // plus a margin (the steering is not exact and a scrape costs damage): the yellow box on the overlay is this one
    const float own_hl = (pl.box.valid ? pl.box.hl : 0.6f) + kOwnMargin;
    const float own_hw = (pl.box.valid ? pl.box.hw : 0.35f) + kOwnMargin;
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

    // Waypoints: the car may already be on a later leg (the route jogged aside and back, the car went straight on)
    {
        const int skip = LocalPlan_RouteProgress(mWpX, mWpY, mWpCount, mWpIdx, pl.x, pl.y, kOnLegTiles);
        if (skip != mWpIdx && skip < mWpCount)
        {
            BotDecision("f=%d drive: already on the leg to waypoint %d (%.1f,%.1f): dropping waypoints %d to %d", ctx.frame, skip, mWpX[skip], mWpY[skip], mWpIdx, skip - 1);
            mWpIdx = skip;
            mProgressFrame = ctx.frame;
        }
    }
    // -bot_drive_pure: a corner is reached before the next leg is taken, not 3.5 tiles short of it
    const float wp_radius = ctx.opts && ctx.opts->drive_pure ? 0.7f : mWpRadius;
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
            passed = (-wx * nx + -wy * ny) > 0 && d < wp_radius * 2.0f;
        }
        if (d < wp_radius || passed)
        {
            mWpIdx++;
            mProgressFrame = ctx.frame;
            continue;
        }
        break;
    }

    // Progress is getting nearer the waypoint ahead, not only reaching it: the legs of the square route are long and in traffic a leg can
    // take more than the 600 frames the stuck rule allows
    {
        const float wx = mWpX[mWpIdx] - pl.x;
        const float wy = mWpY[mWpIdx] - pl.y;
        const float d = sqrtf(wx * wx + wy * wy);
        if (mProgWpIdx != mWpIdx)
        {
            mProgWpIdx = mWpIdx;
            mProgWpDist = d;
        }
        else if (d < mProgWpDist - kProgressTiles)
        {
            mProgWpDist = d;
            mProgressFrame = ctx.frame;
        }
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
        if (blocker && mSpeed < 0.02f)
        {
            mJamStill++;
        }
        else if (mSpeed > 0.05f)
        {
            mJamStill = 0;
        }
        if (mJamSwitch && !mChase && mJamStill >= 90 && ctx.frame % 10 == 0)
        {
            // a better car close ahead (faster, less damaged): the jam is made of them, take one
            WorldModel::CarInfo me;
            JamSelf self;
            self.damage = pl.car_damage;
            self.top_speed = ctx.world->FindCar(pl.car_id, &me) ? me.top_speed : 0.0f;
            self.still_frames = mJamStill;
            JamCar cands[WorldModel::kMaxCars];
            s32 ids[WorldModel::kMaxCars];
            int cn = 0;
            for (int k = 0; k < ctx.world->car_count && cn < WorldModel::kMaxCars; k++)
            {
                const WorldModel::CarInfo& oc = ctx.world->cars[k];
                if (oc.idx == pl.car_id)
                {
                    continue;
                }
                cands[cn].along = (oc.x - pl.x) * fx + (oc.y - pl.y) * fy;
                cands[cn].side = (oc.x - pl.x) * -fy + (oc.y - pl.y) * fx;
                cands[cn].damage = oc.damage;
                cands[cn].top_speed = oc.top_speed;
                cands[cn].has_driver = oc.has_driver;
                cands[cn].locked = oc.locked;
                cands[cn].mission = oc.mission;
                cands[cn].police = oc.police;
                ids[cn] = oc.idx;
                cn++;
            }
            const int pick = JamSwitch_Choose(self, cands, cn);
            if (pick >= 0)
            {
                return Fail("jam_switch", "stuck in a jam for %d frames: taking car=%d, %.1f tiles ahead (damage %d, top speed %.0f; ours %d, %.0f)", mJamStill, ids[pick], cands[pick].along, cands[pick].damage,
                            cands[pick].top_speed, self.damage, self.top_speed);
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
        // a bump in sight of the police is a wanted level (the cars that were pushed about that way ended in an arrest): go round
        if (mBlockedFrames > 45 && (!blocker->police || pl.wanted_stars > 0) && (pl.wanted_stars > 0 || !ctx.world->CopsNear(15.0f)) && mShoves <= mBlockReplans)
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
        if (mChase || mRetries >= 3 || !ctx.action_snap || (from_start < 5.0f && mManoeuvres >= 4))
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
        BotDecision("f=%d RESET drive STUCK %s; retry %d: speed cap %.2f, waypoint radius %.1f, restore f=%d (%s)", ctx.frame, detail, mRetries, mSpeedCap, mWpRadius, restore_frame, restore_from == ctx.action_snap ? "the start" : "a checkpoint");
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

    // -bot_drive_pure: the aim point stays on the global route's current leg and never goes past its corner: with the look ahead
    // longer than the way to the corner the pursuit aims along the next leg and the car cuts the corner through whatever stands there
    const bool pure = ctx.opts && ctx.opts->drive_pure;
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
            // further ahead when faster: a short look steers at every little error and the car swings from side to side
            float look = 4.0f + mSpeed * 40.0f;
            look = look > 12.0f ? 12.0f : look;
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
                else if (!pure && mWpIdx + 1 < mWpCount)
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

    // what the driver is up to, drawn over the game view (BotUi.cpp)
    {
        // the whole global route, start to goal: square legs, blue where behind us and cyan from the next waypoint on
        for (int i = 1; i < mWpCount; i++)
        {
            BotDebugDraw::Line(mWpX[i - 1], mWpY[i - 1], mWpX[i], mWpY[i], i > mWpIdx ? kDebugColCyan : kDebugColBlue);
        }
        if (mWpIdx < mWpCount)
        {
            BotDebugDraw::Line(pl.x, pl.y, mWpX[mWpIdx], mWpY[mWpIdx], kDebugColBlue);
        }
        char label[20];
        _snprintf(label, sizeof(label), "wp %d/%d", mWpIdx, mWpCount);
        BotDebugDraw::Mark(mWpX[mWpIdx], mWpY[mWpIdx], 0.6f, kDebugColYellow, label);
        BotDebugDraw::Mark(aim_x, aim_y, 0.4f, kDebugColMagenta, "aim");
        BotDebugDraw::Mark(mTx, mTy, 1.0f, kDebugColWhite, "goal");
        {
            // our car: the game's collision box (white) and the box the steering tests, at the car's centre pointing the way the
            // model says it faces (yellow): they should lie on top of each other
            if (pl.box.valid)
            {
                BotDebugDraw::Box(pl.box.cx, pl.box.cy, pl.box.ax, pl.box.ay, pl.box.hl, pl.box.hw, kDebugColWhite);
            }
            const float hd = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
            BotDebugDraw::Box(pl.x, pl.y, sinf(hd), -cosf(hd), own_hl, own_hw, kDebugColYellow);
        }
        // what is about: cars (red standing, orange moving with where it will be in 45 frames, blue police) and pedestrians
        for (int k = 0; k < ctx.world->car_count; k++)
        {
            const WorldModel::CarInfo& oc = ctx.world->cars[k];
            if (DriveSteer_IsOwnCar(oc.idx, pl.car_id, pl.in_car) || oc.dist > 30.0f)
            {
                continue;
            }
            const bool still = oc.speed < 0.03f;
            // the game's collision box, as the steering tests it
            const unsigned ccol = oc.police ? kDebugColBlue : (still ? kDebugColRed : kDebugColOrange);
            if (oc.box.valid)
            {
                BotDebugDraw::Box(oc.box.cx, oc.box.cy, oc.box.ax, oc.box.ay, oc.box.hl, oc.box.hw, ccol);
            }
            if (oc.predicted.valid && !still)
            {
                // where its box is going to be over the next few frames
                BotDebugDraw::Box(oc.predicted.cx, oc.predicted.cy, oc.predicted.ax, oc.predicted.ay, oc.predicted.hl, oc.predicted.hw, kDebugColMagenta);
            }
            BotDebugDraw::Mark(oc.x, oc.y, 0.15f, ccol, oc.police ? "cop" : (still ? "stop" : ""));
            if (!still)
            {
                BotDebugDraw::Line(oc.x, oc.y, oc.x + oc.vx * 45.0f, oc.y + oc.vy * 45.0f, kDebugColOrange);
            }
        }
        for (int k = 0; k < ctx.world->ped_count; k++)
        {
            const WorldModel::PedInfo& op = ctx.world->peds[k];
            if (!op.dead && !op.in_car && op.dist < 20.0f)
            {
                // the pedestrian's collision box, and the margin the look-ahead keeps round it (kPedMargin)
                if (op.box.valid)
                {
                    BotDebugDraw::Box(op.box.cx, op.box.cy, op.box.ax, op.box.ay, op.box.hl + kPedMargin, op.box.hw + kPedMargin, op.cop ? kDebugColBlue : kDebugColGrey);
                    BotDebugDraw::Box(op.box.cx, op.box.cy, op.box.ax, op.box.ay, op.box.hl, op.box.hw, op.cop ? kDebugColBlue : kDebugColWhite);
                }
            }
        }
        // the solid tiles the steering sees round the car (buildings, water, fences): the outline of each
        for (int ty = static_cast<int>(pl.y) - 9; ty <= static_cast<int>(pl.y) + 9; ty++)
        {
            for (int tx = static_cast<int>(pl.x) - 9; tx <= static_cast<int>(pl.x) + 9; tx++)
            {
                if (Drive_Solid(NULL, tx + 0.5f, ty + 0.5f))
                {
                    BotDebugDraw::TileOutline(tx + 0.5f, ty + 0.5f, 0xFF2020B0);
                }
            }
        }
        // the detailed route (bright green with a cross at each node) and the tiles it closed because of standing cars
        if (mLocal.found)
        {
            for (int i = 1; i < mLocal.count; i++)
            {
                BotDebugDraw::Line(mLocal.x[i - 1], mLocal.y[i - 1], mLocal.x[i], mLocal.y[i], kDebugColLime);
                BotDebugDraw::Line(mLocal.x[i] - 0.12f, mLocal.y[i], mLocal.x[i] + 0.12f, mLocal.y[i], kDebugColLime);
                BotDebugDraw::Line(mLocal.x[i], mLocal.y[i] - 0.12f, mLocal.x[i], mLocal.y[i] + 0.12f, kDebugColLime);
            }
            for (int i = 0; i < mLocal.tile_count && i < 50; i++)
            {
                BotDebugDraw::TileOutline(mLocal.tile_x[i] + 0.5f, mLocal.tile_y[i] + 0.5f, kDebugColCyan);
            }
            for (int i = 0; i < mLocal.moving_count && i < 40; i++)
            {
                BotDebugDraw::TileOutline(mLocal.moving_x[i] + 0.5f, mLocal.moving_y[i] + 0.5f, kDebugColRed);
            }
            for (int i = 0; i < mLocal.car_closed_count; i++)
            {
                BotDebugDraw::TileOutline(mLocal.car_closed_x[i] + 0.5f, mLocal.car_closed_y[i] + 0.5f, kDebugColOrange);
            }
        }
        // tiles the route planner charges extra for, beside or under a car that stands
        for (int ty = static_cast<int>(pl.y) - 12; ty <= static_cast<int>(pl.y) + 12; ty++)
        {
            for (int tx = static_cast<int>(pl.x) - 12; tx <= static_cast<int>(pl.x) + 12; tx++)
            {
                if (ctx.road->JamAt(tx, ty) == 2)
                {
                    BotDebugDraw::Line(tx + 0.2f, ty + 0.2f, tx + 0.8f, ty + 0.8f, kDebugColRed);
                    BotDebugDraw::Line(tx + 0.8f, ty + 0.2f, tx + 0.2f, ty + 0.8f, kDebugColRed);
                }
            }
        }
        // the overtake: the car it sees in the way, and the places it tried (green taken, red turned down: W building or
        // water, B building on the way, C a car on the way)
        if (ctx.frame - mTraceFrame < 30 && mTrace.has_blocker)
        {
            BotDebugDraw::Mark(mTrace.blocker_x, mTrace.blocker_y, 1.2f, kDebugColYellow, "in the way");
            for (int k = 0; k < mTrace.count; k++)
            {
                // the strip as it was tested: the footprint (half width each side) carried from the car to the place
                const float sdx = mTrace.cand_x[k] - pl.x;
                const float sdy = mTrace.cand_y[k] - pl.y;
                const float sl = sqrtf(sdx * sdx + sdy * sdy);
                if (sl > 0.1f)
                {
                    const float sux = sdx / sl;
                    const float suy = sdy / sl;
                    const unsigned scol = mTrace.cand_why[k] == 0 ? kDebugColGreen : kDebugColRed;
                    BotDebugDraw::Line(pl.x - suy * own_hw, pl.y + sux * own_hw, mTrace.cand_x[k] - suy * own_hw, mTrace.cand_y[k] + sux * own_hw, scol);
                    BotDebugDraw::Line(pl.x + suy * own_hw, pl.y - sux * own_hw, mTrace.cand_x[k] + suy * own_hw, mTrace.cand_y[k] - sux * own_hw, scol);
                }
                static const char* const why[4] = {"ok", "W", "B", "C"};
                BotDebugDraw::Mark(mTrace.cand_x[k], mTrace.cand_y[k], 0.4f, mTrace.cand_why[k] == 0 ? kDebugColGreen : kDebugColRed, why[mTrace.cand_why[k]]);
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

    // The detailed route: an A* with a node per map tile from the car to a point 18 tiles along the global route. It keeps to the
    // direction of the line, goes round standing cars and the closed tiles, and the car follows it by pure pursuit
    bool local_ok = false;
    if (!pure && (dist > 8.0f || (mChase && dist > 2.5f)) && ctx.frame >= mReverseUntil) // (a chase ends beside things: the last stretch is routed round them too)
    {
        SteerCar lcars[WorldModel::kMaxCars + WorldModel::kMaxPeds];
        LocalPlanWorld lw;
        lw.solid = Drive_Solid;
        lw.edge_ok = Drive_EdgeOk;
        sRampZ = static_cast<int>(pl.z + 0.5f);
        lw.ramp_zone = Drive_RampZone;
        lw.step_cost = Drive_LaneCost;
        lw.user = ctx.road;
        lw.cars = lcars;
        lw.car_count = Drive_CollectCars(ctx, lcars);
        lw.car_count += Drive_CollectPeds(ctx, lcars + lw.car_count, WorldModel::kMaxPeds);
        if (mChase)
        {
            lw.car_count = DriveSteer_DropCarsNear(lcars, lw.car_count, mTx, mTy, 2.0f); // the car (or pedestrian) being chased is where the route ends
        }
        lw.own_hl = own_hl;
        lw.own_hw = own_hw;
        lw.own_speed = mSpeed;
        lw.route_x = mRouteAheadX;
        lw.route_y = mRouteAheadY;
        lw.route_n = mRouteAheadN;
        // keep the path while it is good; plan again when it is not, and every 60 frames (moving cars change)
        if (ctx.frame >= mLocalNext || (!mLocalFallback && !LocalPlan_StillGood(lw, mLocal, pl.x, pl.y)))
        {
            // the target: 24 tiles on along the global route from where the car is on its leg; the stretch is also what the
            // detailed route keeps to
            int rn = 0;
            LocalPlan_RouteAhead(mWpX, mWpY, mWpCount, mWpIdx, pl.x, pl.y, 24.0f, mRouteAheadX, mRouteAheadY, &rn);
            mRouteAheadN = rn;
            mLocalFallback = false;
            float tx = mRouteAheadX[rn - 1];
            float ty = mRouteAheadY[rn - 1];
            const float heading0 = ctx.car.sign * pl.rot_deg + ctx.car.offset;
            LocalPlan_Find(lw, pl.x, pl.y, heading0, tx, ty, &mLocal);
            // a goal that cannot be reached (walled off in the window, or the end of the stretch is on a closed tile): try nearer
            // points of the same route before giving up
            // (a partial route to the very end of the drive means the destination is not on the roads: its end, as near as they go, is the
            // arrival, which the code below acts on; only a partial route to a point mid-route is a pocket to be got out of)
            const float to_goal_x = tx - mTx;
            const float to_goal_y = ty - mTy;
            const bool goal_is_end = to_goal_x * to_goal_x + to_goal_y * to_goal_y < 3.0f * 3.0f;
            for (int shorter = 0; shorter < 2 && (!mLocal.found || (mLocal.partial && !goal_is_end)); shorter++)
            {
                const float reach = shorter == 0 ? 12.0f : 6.0f;
                LocalPlan_RouteAhead(mWpX, mWpY, mWpCount, mWpIdx, pl.x, pl.y, reach, mRouteAheadX, mRouteAheadY, &rn);
                mRouteAheadN = rn;
                tx = mRouteAheadX[rn - 1];
                ty = mRouteAheadY[rn - 1];
                LocalPlan_Find(lw, pl.x, pl.y, heading0, tx, ty, &mLocal);
            }
            if (!mLocal.found || (mLocal.partial && !goal_is_end))
            {
                // a partial route ends at the tile nearest the goal in a straight line, which is often a pocket against a wall: not
                // taken while the global route's own legs are there
                // always a full route: the global route's own square legs, from the car along the stretch ahead
                LocalPlan_RouteAhead(mWpX, mWpY, mWpCount, mWpIdx, pl.x, pl.y, 24.0f, mRouteAheadX, mRouteAheadY, &rn);
                mRouteAheadN = rn;
                mLocalFallback = true;
                mLocal.found = rn > 1;
                mLocal.partial = false;
                mLocal.count = rn < kLocalPlanMax ? rn : kLocalPlanMax;
                mLocal.length = 0.0f;
                mLocal.tile_count = 0;
                mLocal.moving_count = 0;
                mLocal.car_closed_count = 0;
                for (int i = 0; i < mLocal.count; i++)
                {
                    mLocal.x[i] = mRouteAheadX[i];
                    mLocal.y[i] = mRouteAheadY[i];
                }
                if (ctx.frame % 40 == 0)
                {
                    BotDecision("f=%d drive: no detailed route from (%.1f,%.1f) to (%.1f,%.1f): driving the global route's own legs", ctx.frame, pl.x, pl.y, tx, ty);
                }
            }
            mLocalNext = ctx.frame + kReplanFrames;
        }
        if (mLocal.found && mLocal.partial && mLocal.count > 0)
        {
            // the route ends at the reachable tile nearest the goal: at its end the car is as near as it gets
            const float ex = mLocal.x[mLocal.count - 1] - pl.x;
            const float ey = mLocal.y[mLocal.count - 1] - pl.y;
            if (ex * ex + ey * ey < kPartialEndNear * kPartialEndNear)
            {
                BotDecision("f=%d drive arrived as near as the roads go to (%.1f,%.1f), %.1f tiles off", ctx.frame, mTx, mTy, dist);
                return Done;
            }
        }
        if (mLocal.found)
        {
            float look = 4.0f + mSpeed * 40.0f;
            look = look > 12.0f ? 12.0f : look;
            sRampZ = static_cast<int>(pl.z + 0.5f);
            look = LocalPlan_RampLook(mLocal, pl.x, pl.y, look, Drive_RampZone, NULL); // up a ramp straight, not cutting its top corner
            LocalPlan_AimPoint(mLocal, pl.x, pl.y, look, &aim_x, &aim_y);
            local_ok = true;
        }
    }
    else
    {
        mLocal.found = false;
    }
    // Police after us: dodge a police car that is closing on the car, and keep the speed up
    if (pl.wanted_stars > 0 && !pure)
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
                    // the side away from it first, then the other; a side with a building or another car on the way is no way out
                    SteerCar swc[WorldModel::kMaxCars];
                    const int swn = Drive_CollectCars(ctx, swc);
                    int police_slot = -1;
                    for (int q = 0; q < swn; q++)
                    {
                        if ((swc[q].x - oc.x) * (swc[q].x - oc.x) + (swc[q].y - oc.y) * (swc[q].y - oc.y) < 0.01f)
                        {
                            police_slot = q;
                        }
                    }
                    const float first = across >= 0.0f ? 1.0f : -1.0f;
                    for (int tryside = 0; tryside < 2; tryside++)
                    {
                        const float sgn = tryside == 0 ? first : -first;
                        const float sx = pl.x + (-uy) * 4.0f * sgn + ux * 1.5f;
                        const float sy = pl.y + ux * 4.0f * sgn + uy * 1.5f;
                        if (WorldModel::BuildingAt(sx, sy) || !Drive_CorridorClear(pl.x, pl.y, sx, sy, 0.75f) ||
                            !DriveSteer_WayFreeOfCars(swc, swn, pl.x, pl.y, sx, sy, own_hw + 0.15f, police_slot))
                        {
                            continue;
                        }
                        mSwerveX = sx;
                        mSwerveY = sy;
                        mSwerveUntil = ctx.frame + 18;
                        aim_x = mSwerveX;
                        aim_y = mSwerveY;
                        BotDecision("f=%d drive: police car #%d closing on us (%.1f tiles, speed %.2f): swerving to the %s", ctx.frame, oc.idx, oc.dist, v, tryside == 0 ? "side away from it" : "other side");
                        break;
                    }
                    break;
                }
            }
        }
    }
    // Driving against the traffic of a lane: move over to the lane that goes our way
    if (!pure && !local_ok && dist > 8.0f && ctx.frame >= mSwerveUntil)
    {
        float lx = 0.0f;
        float ly = 0.0f;
        if (ctx.road->LaneSwitch(pl.x, pl.y, aim_x - pl.x, aim_y - pl.y, &lx, &ly) && !WorldModel::BuildingAt(lx, ly) && Drive_CorridorClear(pl.x, pl.y, lx, ly, 0.75f))
        {
            aim_x = lx;
            aim_y = ly;
        }
    }
    // A pedestrian in the way that is easy to avoid (room beside him) is steered round, whoever is watching: only when there is no
    // detailed route, which already goes round pedestrians
    {
        const float hd = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        float px_[WorldModel::kMaxPeds];
        float py_[WorldModel::kMaxPeds];
        int pn_ = 0;
        for (int k = 0; k < ctx.world->ped_count; k++)
        {
            if (!ctx.world->peds[k].dead && !ctx.world->peds[k].in_car)
            {
                px_[pn_] = ctx.world->peds[k].x;
                py_[pn_] = ctx.world->peds[k].y;
                pn_++;
            }
        }
        DriveSteer_PedDodgeAim(local_ok || pure, pl.x, pl.y, sinf(hd), -cosf(hd), aim_x - pl.x, aim_y - pl.y, px_, py_, pn_, WorldModel::BuildingAt, &aim_x, &aim_y);
    }
    const float err = ctx.Steer(ctx.car, pl.rot_deg, pl.x, pl.y, aim_x, aim_y, true);
    // Which way to hold the wheel: try five arcs ahead and keep the one that ends nearest the aim point without
    // running into a building (or a car coming at us). Steering straight at the aim point is the first choice
    float wall_ahead = 1e3f;
    if (pure)
    {
        // the wheel at the aim point, as gently as the arcs would hold it (DriveSteer's Gentle: a tap every fourth frame from 5 degrees,
        // every other from 8, held from 20): a key that is either down or up swings the car into the wall of a one tile gap
        ctx.input &= ~(input_bit::Left | input_bit::Right);
        const float ea = fabsf(err);
        const float lvl = ea > 20.0f ? 1.0f : (ea > 8.0f ? 0.5f : (ea > 5.0f ? 0.25f : 0.0f));
        if (lvl > 0.0f && ea <= kTurnEngageDeg && (lvl > 0.9f || (lvl > 0.4f && ctx.frame % 2 == 0) || ctx.frame % 4 == 0))
        {
            ctx.input |= err > 0.0f ? input_bit::Right : input_bit::Left; // (err is in the sense of the Right key)
        }
    }
    else if (fabsf(err) <= kTurnEngageDeg && !mTurn.active)
    {
        SteerCar others[WorldModel::kMaxCars];
        const int other_count = Drive_CollectCars(ctx, others);
        SteerWorld world = {Drive_Solid, NULL, others, other_count, own_hl, own_hw, local_ok};
        // A slower car in the way: go round it by the pavement or the next lane. The side is kept for a while so
        // that the choice does not flip from frame to frame
        if (ctx.frame < mOvertakeUntil)
        {
            aim_x = mOvertakeX;
            aim_y = mOvertakeY;
        }
        else if (!local_ok && dist > 12.0f && (mTraceFrame = ctx.frame, true) && DriveSteer_Overtake(world, pl.x, pl.y, mSpeed, aim_x, aim_y, &mOvertakeX, &mOvertakeY, &mTrace))
        {
            mOvertakeUntil = ctx.frame + 45;
            aim_x = mOvertakeX;
            aim_y = mOvertakeY;
            if (ctx.frame % 20 == 0)
            {
                BotDecision("f=%d drive: slower car ahead: going round it towards (%.1f,%.1f)", ctx.frame, aim_x, aim_y);
            }
        }
        const float heading_deg = ctx.car.sign * pl.rot_deg + ctx.car.offset;
        const float hdir = ctx.car.sign * ctx.car.turn_sign;
        const SteerChoice choice = DriveSteer_Choose(world, pl.x, pl.y, heading_deg, hdir, mSpeed, aim_x, aim_y, mPrevSteer, &mSteerDebug);
        mPrevSteer = choice.s;
        wall_ahead = choice.wall_d;
        {
            // the five rollouts that were really run: the track of the car's centre (green the one chosen, grey the others), and
            // what stopped each: a red cross at the footprint point that met a building, or a ring of the hit radius round the
            // place the other car will be
            for (int a = 0; a < mSteerDebug.arc_count; a++)
            {
                const SteerArcDebug& arc = mSteerDebug.arcs[a];
                const unsigned col = arc.chosen ? kDebugColGreen : kDebugColGrey;
                float lx = pl.x;
                float ly = pl.y;
                for (int i = 0; i < arc.n; i++)
                {
                    BotDebugDraw::Line(lx, ly, arc.px[i], arc.py[i], col);
                    lx = arc.px[i];
                    ly = arc.py[i];
                }
                if (arc.hit == 1)
                {
                    // the map tile that was solid at the point of the box that met it (buildings are tile squares)
                    BotDebugDraw::TileOutline(arc.hit_x, arc.hit_y, kDebugColRed);
                }
                else if (arc.hit == 2)
                {
                    // our box and the other car's box at the moment they met
                    BotDebugDraw::Box(arc.own_box.cx, arc.own_box.cy, arc.own_box.ax, arc.own_box.ay, arc.own_box.hl, arc.own_box.hw, arc.chosen ? kDebugColYellow : kDebugColGrey);
                    BotDebugDraw::Box(arc.car_box.cx, arc.car_box.cy, arc.car_box.ax, arc.car_box.ay, arc.car_box.hl, arc.car_box.hw, kDebugColOrange);
                }
            }
            if (ctx.frame < mOvertakeUntil)
            {
                BotDebugDraw::Mark(mOvertakeX, mOvertakeY, 0.5f, kDebugColOrange, "overtake");
            }
        }
        ctx.input &= ~(input_bit::Left | input_bit::Right);
        if (choice.s != 0.0f && (fabsf(choice.s) > 0.9f || (fabsf(choice.s) > 0.4f && ctx.frame % 2 == 0) || ctx.frame % 4 == 0))
        {
            ctx.input |= choice.s > 0.0f ? input_bit::Right : input_bit::Left;
        }
        if (choice.changed && ctx.frame % 20 == 0)
        {
            BotDecision("f=%d drive: steering %+.1f to keep clear of what is ahead (wall in %.1f tiles)", ctx.frame, choice.s, wall_ahead);
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
    if (!pure)
    {
        const float heading = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        const float hx = sinf(heading);
        const float hy = -cosf(heading);
        const float reach = 3.0f + mSpeed * 50.0f;
        SteerCar route_cars[WorldModel::kMaxCars];
        const int route_car_n = Drive_CollectCars(ctx, route_cars);
        float first_obstacle = -1.0f;
        bool obstacle_is_ped = false;
        bool obstacle_is_car = false;
        for (float d = 1.2f; d <= reach && first_obstacle < 0.0f; d += 0.5f)
        {
            // along the route being driven when there is one (the heading ray ran into a police car the route goes round: the car
            // stopped for it), else along the heading
            float sx = pl.x + hx * d;
            float sy = pl.y + hy * d;
            if (local_ok)
            {
                LocalPlan_AimPoint(mLocal, pl.x, pl.y, d, &sx, &sy);
            }
            bool hit = false; // buildings are the arcs' business (above)
            // Other cars are not obstacles: stopping dead behind every parked or queuing car is what makes a
            // drive crawl, and a bump costs a few points of damage. Walls and pedestrians are.
            for (int i = 0; i < ctx.world->ped_count && !hit; i++)
            {
                const WorldModel::PedInfo& p = ctx.world->peds[i];
                if (!p.dead && !p.in_car && (p.box.valid ? SteerBox_Contains(Drive_ToSteerBox(p.box), sx, sy, kPedMargin) : ((p.x - sx) * (p.x - sx) + (p.y - sy) * (p.y - sy) < 0.8f * 0.8f)))
                {
                    hit = true;
                    obstacle_is_ped = true;
                }
            }
            // a car that will be on the route when we get there (moving across or along it, or standing on it): slow down to let it by
            // or go behind it, not into it
            for (int i = 0; i < route_car_n && !hit && !mChase; i++) // (not in a chase: ramming the car is the point)
            {
                if (LocalPlan_CarInWayAt(route_cars[i], sx, sy, d / (mSpeed > 0.05f ? mSpeed : 0.05f), own_hw))
                {
                    hit = true;
                    obstacle_is_car = true;
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
            if ((first_obstacle > 1.9f || obstacle_is_car) && allowed < 0.03f)
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
        const float allowed = DriveSteer_SpeedFor(wall_ahead);
        cap = allowed < cap ? allowed : cap;
    }
    if (last && dist < 14.0f)
    {
        cap *= 0.5f;
    }
    if (pure && !last)
    {
        // the one slowing the pure drive keeps: a right angle at the end of the leg is taken slowly, or the car runs on past it
        const float lx = mWpX[mWpIdx] - (mWpIdx > 0 ? mWpX[mWpIdx - 1] : mPlanX);
        const float ly = mWpY[mWpIdx] - (mWpIdx > 0 ? mWpY[mWpIdx - 1] : mPlanY);
        const float nx = mWpX[mWpIdx + 1] - mWpX[mWpIdx];
        const float ny = mWpY[mWpIdx + 1] - mWpY[mWpIdx];
        const float ll = sqrtf(lx * lx + ly * ly);
        const float nl = sqrtf(nx * nx + ny * ny);
        if (ll > 0.1f && nl > 0.1f && (lx * nx + ly * ny) / (ll * nl) < 0.7f)
        {
            const float rx = mWpX[mWpIdx] - pl.x;
            const float ry = mWpY[mWpIdx] - pl.y;
            const float allowed = 0.04f + sqrtf(rx * rx + ry * ry) * 0.02f;
            cap = allowed < cap ? allowed : cap;
        }
    }
    if (ctx.opts && ctx.opts->draw && (ctx.frame - ctx.action_start_frame) % 40 == 0 || ctx.frame - ctx.action_start_frame < 100 && (ctx.frame - ctx.action_start_frame) % 10 == 0)
    {
        BotDecision("f=%d drive state: at (%.1f,%.1f) speed %.3f cap %.2f, heading error %.0f to aim (%.1f,%.1f) on the way to waypoint %d/%d (%.1f,%.1f), %.0f tiles to go", ctx.frame, pl.x, pl.y, mSpeed, cap, err, aim_x, aim_y, mWpIdx, mWpCount, mWpX[mWpIdx], mWpY[mWpIdx], dist);
        if (mLocal.found)
        {
            char pts[300];
            int n = 0;
            for (int k = 0; k < mLocal.count && k < 10 && n < 270; k++)
            {
                n += _snprintf(pts + n, sizeof(pts) - n, " (%.1f,%.1f%s)", mLocal.x[k], mLocal.y[k], Drive_Solid(NULL, mLocal.x[k], mLocal.y[k]) ? " SOLID" : (WorldModel::SlopeAxisAt(static_cast<int>(mLocal.x[k]), static_cast<int>(mLocal.y[k]), static_cast<int>(pl.z + 0.5f)) == 1 ? " SLOPE-x" : (WorldModel::SlopeAxisAt(static_cast<int>(mLocal.x[k]), static_cast<int>(mLocal.y[k]), static_cast<int>(pl.z + 0.5f)) == 2 ? " SLOPE-y" : "")));
            }
            pts[n < 299 ? n : 299] = 0;
            BotDecision("f=%d drive route (%s, %d points):%s", ctx.frame, mLocalFallback ? "global legs" : (mLocal.partial ? "partial" : "planned"), mLocal.count, pts);
        }
    }
    {
        // The way to go is behind the car: turn it round (TurnAround.hpp)
        SteerCar around[WorldModel::kMaxCars];
        const int around_count = Drive_CollectCars(ctx, around);
        const float th = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        char ahead_what[24] = "";
        char behind_what[24] = "";
        const float box_hl = pl.box.valid ? pl.box.hl : 0.5f;
        const float box_hw = pl.box.valid ? pl.box.hw : 0.27f;
        const TurnIn tin = {err * ctx.car.turn_sign, Drive_RoomAlong(pl.x, pl.y, th, 1.0f, box_hl, box_hw, around, around_count, ahead_what),
                            Drive_RoomAlong(pl.x, pl.y, th, -1.0f, box_hl, box_hw, around, around_count, behind_what)};
        const TurnOut tout = TurnAround_Step(&mTurn, tin);
        if (tout.active)
        {
            if (mTurn.phase == 1)
            {
                BotDecision("f=%d drive: turning round (%.0f degrees off): %s, room ahead %.1f [%s] behind %.1f [%s]", ctx.frame, err, tout.gas > 0 ? "forward" : "backing up", tin.fwd_clear, ahead_what, tin.rear_clear, behind_what);
            }
            ctx.input &= ~(input_bit::Left | input_bit::Right | input_bit::Forward | input_bit::Backward);
            ctx.input |= tout.right ? input_bit::Right : input_bit::Left; // (err above is already in the sense of the Right key)
            if (tout.spin)
            {
                ctx.input |= input_bit::Forward | input_bit::Backward;
            }
            else if (tout.gas != 0)
            {
                ctx.input |= tout.gas > 0 ? input_bit::Forward : input_bit::Backward;
            }
            return Running;
        }
    }
    mBehindSince = 0;
    if (fabsf(err) > 60.0f && mSpeed > 0.14f && ctx.frame >= mHandbrakeNext)
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

    // A pedestrian in the way ahead flees from the horn: bursts, unless the aim is to run them down
    {
        const float hh = (ctx.car.sign * pl.rot_deg + ctx.car.offset) * 3.14159265f / 180.0f;
        const float hfx = sinf(hh);
        const float hfy = -cosf(hh);
        HornIn horn;
        horn.ped_ahead = false;
        horn.ped_dist = 0.0f;
        horn.running_over = mChase;
        for (int i = 0; i < ctx.world->ped_count; i++)
        {
            const WorldModel::PedInfo& p = ctx.world->peds[i];
            const float along = (p.x - pl.x) * hfx + (p.y - pl.y) * hfy;
            const float side = (p.x - pl.x) * -hfy + (p.y - pl.y) * hfx;
            if (!p.dead && !p.in_car && !p.cop && along > 0.0f && along < kHornFar && side > -1.6f && side < 1.6f && (!horn.ped_ahead || along < horn.ped_dist))
            {
                horn.ped_ahead = true;
                horn.ped_dist = along;
            }
        }
        if (Horn_Press(horn, ctx.frame))
        {
            ctx.input |= input_bit::Special1;
        }
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
