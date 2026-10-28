#include "CarActions.hpp"
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
}

void EnterCarAction::Start(BotContext& /*ctx*/)
{
    delete mWalk;
    mWalk = NULL;
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
    if (mPressing && car.has_driver && car.speed > 0.05f)
    {
        return Fail("action_failed", "enter_car: car #%d drove off (speed %.2f)", mCarId, car.speed);
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
        if (oc.dist < 1.0f) // our own
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
static const float kSolidGoalStop = 6.0f; // tiles: how near a goal inside a building is reached
static const float kPartialEndNear = 2.0f; // tiles from the end of a partial local route that count as being there
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
        // the target cannot be reached by car: drive to the nearest tile that can, and right up to it: that tile is as near as the roads
        // go, so the arrival radius of the trip (which counts for the real target) does not apply to it
        mTx = mWpX[mWpCount - 1];
        mTy = mWpY[mWpCount - 1];
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

    // what the driver is up to, drawn over the game view (BotUi.cpp)
    {
        float px = pl.x;
        float py = pl.y;
        for (int i = mWpIdx; i < mWpCount && i < mWpIdx + 30; i++)
        {
            BotDebugDraw::Line(px, py, mWpX[i], mWpY[i], kDebugColCyan);
            px = mWpX[i];
            py = mWpY[i];
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
            if (oc.dist < 1.0f || oc.dist > 30.0f)
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
    if (dist > 8.0f && ctx.frame >= mReverseUntil)
    {
        SteerCar lcars[WorldModel::kMaxCars];
        LocalPlanWorld lw;
        lw.solid = Drive_Solid;
        lw.step_cost = Drive_LaneCost;
        lw.user = ctx.road;
        lw.cars = lcars;
        lw.car_count = Drive_CollectCars(ctx, lcars);
        lw.own_hl = own_hl;
        lw.own_hw = own_hw;
        // keep the path while it is good; plan again when it is not, and every 60 frames (moving cars change)
        if (ctx.frame >= mLocalNext || !LocalPlan_StillGood(lw, mLocal, pl.x, pl.y))
        {
            // the target: 18 tiles on along the waypoints from where the car is on its leg
            float tx = mWpX[mWpCount - 1];
            float ty = mWpY[mWpCount - 1];
            float rest = 24.0f;
            float qx = pl.x;
            float qy = pl.y;
            for (int i = mWpIdx; i < mWpCount; i++)
            {
                const float ex = mWpX[i] - qx;
                const float ey = mWpY[i] - qy;
                const float el = sqrtf(ex * ex + ey * ey);
                if (el >= rest && el > 0.001f)
                {
                    tx = qx + ex / el * rest;
                    ty = qy + ey / el * rest;
                    break;
                }
                rest -= el;
                qx = mWpX[i];
                qy = mWpY[i];
            }
            const float heading0 = ctx.car.sign * pl.rot_deg + ctx.car.offset;
            LocalPlan_Find(lw, pl.x, pl.y, heading0, tx, ty, &mLocal);
            mLocalNext = ctx.frame + 60;
            if (!mLocal.found && ctx.frame % 40 == 0)
            {
                BotDecision("f=%d drive: no detailed route from (%.1f,%.1f) to (%.1f,%.1f): following the waypoints", ctx.frame, pl.x, pl.y, tx, ty);
            }
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
            LocalPlan_AimPoint(mLocal, pl.x, pl.y, look, &aim_x, &aim_y);
            local_ok = true;
        }
    }
    else
    {
        mLocal.found = false;
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
    if (!local_ok && dist > 8.0f && ctx.frame >= mSwerveUntil)
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
    if (fabsf(err) <= kTurnEngageDeg && !mTurn.active)
    {
        SteerCar others[WorldModel::kMaxCars];
        const int other_count = Drive_CollectCars(ctx, others);
        SteerWorld world = {Drive_Solid, NULL, others, other_count, own_hl, own_hw};
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
        if (choice.s != 0.0f && (fabsf(choice.s) > 0.9f || ctx.frame % 2 == 0))
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
                if (!p.dead && !p.in_car && (p.box.valid ? SteerBox_Contains(Drive_ToSteerBox(p.box), sx, sy, kPedMargin) : ((p.x - sx) * (p.x - sx) + (p.y - sy) * (p.y - sy) < 0.8f * 0.8f)))
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
        const float allowed = DriveSteer_SpeedFor(wall_ahead);
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
