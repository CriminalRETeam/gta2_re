#include "ChaseActions.hpp"
#include "NavFoot.hpp"
#include "NavCar.hpp"
#include "BotLog.hpp"
#include "input_bits.hpp"
#include <math.h>
#include <string.h>
#include "BotCompat.hpp"

// Models the bot never takes: emergency and public vehicles (the driver is a service worker who comes after the
// player, an ambulance cannot be driven off), trains, articulated lorries and slow vans
bool Cars_BadModel(s32 model)
{
    switch (model)
    {
        case 6:  // boxcar
        case 11: // bus
        case 12: // police car
        case 17: // fire truck
        case 23: case 24: case 25: case 26: // hot dog vans
        case 27: // ice cream van
        case 34: // ambulance
        case 52: // swat van
        case 54: // tank
        case 55: // tanker
        case 58: // tow truck
        case 59: case 60: case 61: // trains
        case 63: case 64: case 65: case 66: // lorry cabs and trailers
            return true;
        default:
            return false;
    }
}

bool Cars_Pick(BotContext& ctx, const CarPick& ban, float max_dist, bool prefer_fast, s32* out_id, bool* out_hijack, float* out_path)
{
    const WorldModel& w = *ctx.world;
    const WorldModel::PlayerInfo& pl = w.player;
    float best = 1e9f;
    bool found = false;
    const bool cops_near = w.CopsNear(16.0f);
    if (cops_near && !w.TimeLimited() && pl.wanted_stars == 0)
    {
        return false; // a car taken in front of the police is a wanted level: move away from them first, unless the clock is running.
                      // With the wanted level up already they are after the player: a car is the way out, not something to avoid
    }
    for (int i = 0; i < w.car_count; i++)
    {
        const WorldModel::CarInfo& c = w.cars[i];
        bool banned = false;
        for (int b = 0; b < ban.banned_count; b++)
        {
            banned = banned || ban.banned[b] == c.idx;
        }
        if (banned || c.locked || c.police || c.mission || c.damage > 1500 || c.dist > max_dist || Cars_BadModel(c.model))
        {
            continue;
        }
        if (c.has_driver && (c.speed > 0.03f || (pl.wanted_stars == 0 && cops_near) || c.dist > 10.0f))
        {
            continue; // moving traffic cannot be boarded, a hijack in front of the police ends in an arrest, and a car
                      // waiting at a light drives off before we are there unless it is right beside us
        }
        if (!ctx.nav->FindPath(static_cast<u8>(pl.x), static_cast<u8>(pl.y), static_cast<u8>(pl.z + 0.5f), static_cast<u8>(c.x), static_cast<u8>(c.y)) || ctx.nav->Approximate())
        {
            continue;
        }
        if (c.has_driver && ctx.nav->PathLength() > 14)
        {
            continue; // by the time we have walked there it has driven off
        }
        // a hijack is dearer than a parked car: the driver fights back
        float cost = static_cast<float>(ctx.nav->PathLength()) + (c.has_driver ? 25.0f : 0.0f) + static_cast<float>(c.damage) * 0.01f;
        cost -= c.top_speed * (prefer_fast ? 6.0f : 2.0f); // a fast car is worth a walk
        if (cost < best)
        {
            best = cost;
            *out_id = c.idx;
            *out_hijack = c.has_driver;
            *out_path = static_cast<float>(ctx.nav->PathLength());
            found = true;
        }
    }
    return found;
}

// ---------------------------------------------------------------------------------------------
RamCarAction::RamCarAction(s32 target_car_id)
    : mTarget(target_car_id)
    , mStart(0)
    , mHavePrev(false)
    , mPrevTx(0)
    , mPrevTy(0)
    , mVx(0)
    , mVy(0)
    , mPrevX(0)
    , mPrevY(0)
    , mSpeed(0)
    , mStill(0)
    , mReverseUntil(0)
    , mReverseSteer(1.0f)
    , mPlanFrame(-1000)
    , mWpIdx(0)
    , mHavePlan(false)
    , mHits(0)
    , mLastTargetDamage(0)
    , mLastDamageFrame(0)
    , mStoppedFrames(0)
    , mStay(false)
    , mWpCount(0)
{
}

void RamCarAction::Start(BotContext& ctx)
{
    mStart = ctx.frame;
    mHavePrev = false;
    mStill = 0;
    mReverseUntil = 0;
    mHavePlan = false;
    mPlanFrame = -1000;
    mHits = 0;
    mStoppedFrames = 0;
    mLastDamageFrame = ctx.frame;
    mLastTargetDamage = 0;
    if (!ctx.car.valid && ctx.foot.valid)
    {
        ctx.car = ctx.foot;
    }
}

// No building on the straight line between two points (sampled every half tile)
bool RamCarAction::LineClear(float x0, float y0, float x1, float y1) const
{
    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float len = sqrtf(dx * dx + dy * dy);
    const int steps = static_cast<int>(len * 2.0f) + 1;
    for (int i = 1; i < steps; i++)
    {
        const float t = static_cast<float>(i) / steps;
        if (WorldModel::BuildingAt(x0 + dx * t, y0 + dy * t))
        {
            return false;
        }
    }
    return true;
}

Action::Status RamCarAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.dead)
    {
        return Fail("action_failed", "ram_car: the player died");
    }
    if (!pl.in_car)
    {
        return Fail("action_failed", "ram_car: the player is not in a car");
    }
    if (ctx.frame - mStart > 3000)
    {
        return Fail("no_progress", "ram_car: car #%d still going after 3000 frames", mTarget);
    }
    if (pl.car_damage >= 14500)
    {
        return Fail("car_damaged", "ram_car: own car damage %d, about to burn", pl.car_damage);
    }
    WorldModel::CarInfo t;
    if (!ctx.world->FindCar(mTarget, &t))
    {
        BotDecision("f=%d ram_car: car #%d is gone", ctx.frame, mTarget);
        return Done;
    }
    if (t.damage >= 16000)
    {
        BotDecision("f=%d ram_car: car #%d is burning (damage %d)", ctx.frame, mTarget, t.damage);
        return Done;
    }
    if (t.damage > mLastTargetDamage + 200)
    {
        mHits++;
        mLastDamageFrame = ctx.frame;
    }
    mLastTargetDamage = t.damage;

    // own and the target's velocity
    if (mHavePrev)
    {
        const float mx = pl.x - mPrevX;
        const float my = pl.y - mPrevY;
        mSpeed = sqrtf(mx * mx + my * my);
        mVx = mVx * 0.8f + (t.x - mPrevTx) * 0.2f;
        mVy = mVy * 0.8f + (t.y - mPrevTy) * 0.2f;
    }
    mPrevX = pl.x;
    mPrevY = pl.y;
    mPrevTx = t.x;
    mPrevTy = t.y;
    mHavePrev = true;

    const float dx = t.x - pl.x;
    const float dy = t.y - pl.y;
    const float dist = sqrtf(dx * dx + dy * dy);
    const float tspeed = sqrtf(mVx * mVx + mVy * mVy);

    // The target has stopped (stuck, wrecked in the road, driver gone) and we are next to it:
    // that is the moment to get out and finish it with the gun
    if (dist < 7.0f && tspeed < 0.01f && !mStay)
    {
        mStoppedFrames++;
        if (mStoppedFrames > 30)
        {
            BotDecision("f=%d ram_car: car #%d has stopped %.1f tiles away (damage %d, %d hits)", ctx.frame, mTarget, dist, t.damage, mHits);
            return Done;
        }
    }
    else
    {
        mStoppedFrames = 0;
    }

    // stuck: back out and swing the other way
    if (ctx.frame < mReverseUntil)
    {
        ctx.input = input_bit::Backward | (mReverseSteer > 0 ? input_bit::Right : input_bit::Left);
        return Running;
    }
    if (mSpeed < 0.004f && ctx.frame - mStart > 30)
    {
        mStill++;
    }
    else
    {
        mStill = 0;
    }
    if (mStill > 45 && (mStay || !(dist < 4.0f && tspeed < 0.02f)))
    {
        mStill = 0;
        mReverseUntil = ctx.frame + 35;
        mReverseSteer = ctx.rng->Below(2) == 0 ? 1.0f : -1.0f;
        BotDecision("f=%d ram_car: stuck at (%.1f,%.1f), reversing", ctx.frame, pl.x, pl.y);
        return Running;
    }

    // Where to aim: the point the target will reach by the time we do, or a waypoint of the road
    // route when buildings are in the way
    const float closing = mSpeed > 0.08f ? mSpeed : 0.08f;
    float lead = dist / closing;
    lead = lead > 30.0f ? 30.0f : lead;
    float ax = t.x + mVx * lead;
    float ay = t.y + mVy * lead;
    if (!LineClear(pl.x, pl.y, ax, ay))
    {
        ax = t.x;
        ay = t.y;
    }
    if (!LineClear(pl.x, pl.y, ax, ay))
    {
        if (!mHavePlan || ctx.frame - mPlanFrame > 60)
        {
            mPlanFrame = ctx.frame;
            mWpCount = 0;
            if (ctx.road->FindPath(pl.x, pl.y, t.x, t.y))
            {
                for (u32 i = 1; i < ctx.road->Count() && mWpCount < kMaxWp; i++)
                {
                    mWpX[mWpCount] = ctx.road->X(i);
                    mWpY[mWpCount] = ctx.road->Y(i);
                    mWpCount++;
                }
            }
            mWpIdx = 0;
            mHavePlan = mWpCount > 0;
        }
        if (mHavePlan)
        {
            while (mWpIdx < mWpCount - 1 && (mWpX[mWpIdx] - pl.x) * (mWpX[mWpIdx] - pl.x) + (mWpY[mWpIdx] - pl.y) * (mWpY[mWpIdx] - pl.y) < 3.5f * 3.5f)
            {
                mWpIdx++;
            }
            ax = mWpX[mWpIdx];
            ay = mWpY[mWpIdx];
        }
    }
    else
    {
        mHavePlan = false;
    }

    const float err = ctx.Steer(ctx.car, pl.rot_deg, pl.x, pl.y, ax, ay, false);
    const float aerr = fabsf(err);
    if (aerr > 120.0f && mSpeed < 0.06f)
    {
        // it is behind us and we are slow: reverse round (the steering works the other way when backing)
        const bool l = (ctx.input & input_bit::Left) != 0;
        const bool rt = (ctx.input & input_bit::Right) != 0;
        ctx.input &= ~(input_bit::Left | input_bit::Right);
        ctx.input |= (l ? input_bit::Right : 0) | (rt ? input_bit::Left : 0) | input_bit::Backward;
    }
    else if (aerr > 55.0f)
    {
        // a sharp turn: ease off and swing the tail with the hand brake when fast
        ctx.input |= input_bit::Forward;
        if (mSpeed > 0.12f)
        {
            ctx.input |= input_bit::HandbrakeJump;
        }
    }
    else
    {
        ctx.input |= input_bit::Forward;
        // very close and nearly there: no brakes, drive through it
    }
    if (ctx.frame % 60 == 0)
    {
        BotDecision("f=%d ram_car: target #%d at %.1f tiles, speed %.2f (mine %.2f), err %.0f, damage %d (mine %d), %d hits", ctx.frame, mTarget, dist, tspeed, mSpeed, err, t.damage, pl.car_damage, mHits);
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
RunOverAction::RunOverAction(s32 gang_idx, s32 kills)
    : mGang(gang_idx)
    , mWant(kills)
    , mKilled(0)
    , mStart(0)
    , mLastSeen(0)
    , mTargetId(0)
    , mTargetAlive(false)
    , mHavePrev(false)
    , mPrevX(0)
    , mPrevY(0)
    , mTx(0)
    , mTy(0)
    , mVx(0)
    , mVy(0)
    , mSpeed(0)
    , mStill(0)
    , mReverseUntil(0)
    , mReverseSteer(1.0f)
{
}

void RunOverAction::Start(BotContext& ctx)
{
    mStart = ctx.frame;
    mLastSeen = ctx.frame;
    mKilled = 0;
    mTargetId = 0;
    mTargetAlive = false;
    mHavePrev = false;
    mStill = 0;
    mReverseUntil = 0;
    if (!ctx.car.valid && ctx.foot.valid)
    {
        ctx.car = ctx.foot;
    }
}

Action::Status RunOverAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.dead)
    {
        return Fail("action_failed", "run_over: the player died");
    }
    if (!pl.in_car)
    {
        return Fail("action_failed", "run_over: the player is not in a car");
    }
    if (pl.car_damage >= 14500)
    {
        return Fail("car_damaged", "run_over: own car damage %d, about to burn", pl.car_damage);
    }
    if (mKilled >= mWant)
    {
        return Done;
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

    // the nearest living gang ped on foot; a kill is a target seen dead
    const WorldModel& w = *ctx.world;
    const WorldModel::PedInfo* target = NULL;
    bool target_listed = false;
    for (int i = 0; i < w.ped_count; i++)
    {
        const WorldModel::PedInfo& p = w.peds[i];
        if (p.id == mTargetId)
        {
            target_listed = true;
        }
        if (!target && p.gang_idx == mGang && !p.dead && !p.cop && !p.in_car && p.dist < 40.0f)
        {
            target = &p;
        }
    }
    if (mTargetAlive && !target_listed)
    {
        // it left the list of living peds: dead (the pool drops the dead from the nearest list)
        mKilled++;
        mTargetAlive = false;
        BotDecision("f=%d run_over: ped #%d is down (%d of %d), speed %.2f", ctx.frame, mTargetId, mKilled, mWant, mSpeed);
    }
    if (!target)
    {
        mTargetAlive = false;
        mTargetId = 0;
        // none in sight: keep rolling, and give up after a while so the caller can look elsewhere
        ctx.input = input_bit::Forward;
        return ctx.frame - mLastSeen > 150 ? Done : Running;
    }
    mLastSeen = ctx.frame;
    if (target->id != mTargetId)
    {
        mTargetId = target->id;
        mTargetAlive = true;
        mVx = mVy = 0;
        mTx = target->x;
        mTy = target->y;
    }
    mVx = mVx * 0.7f + (target->x - mTx) * 0.3f;
    mVy = mVy * 0.7f + (target->y - mTy) * 0.3f;
    mTx = target->x;
    mTy = target->y;

    if (ctx.frame < mReverseUntil)
    {
        ctx.input = input_bit::Backward | (mReverseSteer > 0 ? input_bit::Right : input_bit::Left);
        return Running;
    }
    if (mSpeed < 0.004f && ctx.frame - mStart > 30)
    {
        mStill++;
    }
    else
    {
        mStill = 0;
    }
    if (mStill > 40)
    {
        mStill = 0;
        mReverseUntil = ctx.frame + 30;
        mReverseSteer = ctx.rng->Below(2) == 0 ? 1.0f : -1.0f;
        return Running;
    }
    const float lead = target->dist / (mSpeed > 0.06f ? mSpeed : 0.06f);
    const float ax = target->x + mVx * (lead > 20.0f ? 20.0f : lead);
    const float ay = target->y + mVy * (lead > 20.0f ? 20.0f : lead);
    const float err = ctx.Steer(ctx.car, pl.rot_deg, pl.x, pl.y, ax, ay, false);
    ctx.input |= input_bit::Forward;
    if (fabsf(err) > 60.0f && mSpeed > 0.12f)
    {
        ctx.input |= input_bit::HandbrakeJump; // swing the tail round
    }
    return Running;
}
