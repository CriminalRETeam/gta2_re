#include "WalkAction.hpp"
#include "WorldModel.hpp"
#include "NavFoot.hpp"
#include "CarActions.hpp"
#include "Snapshot.hpp"
#include "BotLog.hpp"
#include "BotOptions.hpp"
#include "input_bits.hpp"
#include <math.h>
#include <stdio.h>
#include "BotCompat.hpp"

WalkAction::WalkAction(float x, float y, bool follow_arrow, float stop_dist)
    : mTx(x)
    , mTy(y)
    , mFollowArrow(follow_arrow)
    , mStopDist(stop_dist)
    , mPlanned(false)
    , mTargetIsArrow(false)
    , mPathIdx(0)
    , mProgressFrame(0)
    , mRetries(0)
    , mWaitUntil(0)
    , mCkNext(0)
    , mCkSlot(0)
    , mTrafficWait(0)
    , mYieldUntil(0)
    , mYields(0)
    , mJumpUntil(0)
    , mJumpStart(0)
    , mJumps(0)
    , mNudgeUntil(0)
    , mNudges(0)
    , mNudgeX(0)
    , mNudgeY(0)
    , mCalibrating(false)
    , mExitCar(NULL)
{
    mCk[0] = mCk[1] = NULL;
    mCkFrame[0] = mCkFrame[1] = -1;
}

WalkAction::~WalkAction()
{
    delete mCk[0];
    delete mCk[1];
    delete mExitCar;
}

void WalkAction::Start(BotContext& ctx)
{
    mCkFrame[0] = mCkFrame[1] = -1;
    mCkNext = ctx.frame + 300;
    mPlanned = false;
    mRetries = 0;
    mCalibrating = !ctx.foot.valid;
    if (mCalibrating)
    {
        BotDecision("f=%d walk: calibrating the ped's steering", ctx.frame);
        mCalib.Start(false);
    }
}

bool WalkAction::Plan(BotContext& ctx, const char* why)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    const u8 sx = static_cast<u8>(pl.x);
    const u8 sy = static_cast<u8>(pl.y);
    const u8 sz = static_cast<u8>(pl.z + 0.5f);
    mPlanned = ctx.nav->FindPath(sx, sy, sz, static_cast<u8>(mTx), static_cast<u8>(mTy));
    mPathIdx = 0;
    mProgressFrame = ctx.frame;
    mYields = 0;
    mTrafficWait = 0;
    mNudges = 0;
    mNudgeUntil = 0;
    mJumps = 0;
    mJumpUntil = 0;
    BotDecision("f=%d walk plan (%s) (%d,%d,%d)->(%d,%d): %s, %u tiles, %u explored, %u banned",
                ctx.frame, why, sx, sy, sz, static_cast<u8>(mTx), static_cast<u8>(mTy), mPlanned ? "found" : "NO PATH",
                ctx.nav->PathLength(), ctx.nav->Expanded(), ctx.nav->BanCount());
    if (ctx.nav_dump)
    {
        char path[MAX_PATH];
        char title[120];
        _snprintf(title, sizeof(title), "nav f=%d %s", ctx.frame, why);
        BotPath_Join(path, sizeof(path), ctx.out_dir, "bot_nav.txt");
        ctx.nav->Dump(path, title);
    }
    return mPlanned;
}

Action::Status WalkAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.in_car)
    {
        // a walk is on foot: get out first
        if (!mExitCar)
        {
            mExitCar = new ExitCarAction();
            mExitCar->Start(ctx);
        }
        if (mExitCar->Step(ctx) == Failed)
        {
            return Fail("action_failed", "walk: the player is in a car and cannot get out: %s", mExitCar->fail_detail);
        }
        return Running;
    }
    if (mExitCar)
    {
        delete mExitCar;
        mExitCar = NULL;
        mProgressFrame = ctx.frame;
    }

    if (mCalibrating)
    {
        char why[200];
        const Status s = mCalib.Step(ctx, &ctx.foot, why, sizeof(why));
        if (s == Failed)
        {
            return Fail("action_failed", "%s", why);
        }
        if (s == Running)
        {
            return Running;
        }
        mCalibrating = false;
        BotDecision("f=%d walk: heading = %+.0f*rot %+.0f, Right turns %+.0f", ctx.frame, ctx.foot.sign, ctx.foot.offset, ctx.foot.turn_sign);
    }

    // Target: the arrow if asked and there is one
    float tx = mTx;
    float ty = mTy;
    bool is_arrow = false;
    if (mFollowArrow && ctx.world->arrow.valid)
    {
        tx = ctx.world->arrow.x;
        ty = ctx.world->arrow.y;
        is_arrow = true;
    }
    else if (mFollowArrow && !ctx.opts->has_goto)
    {
        return Fail("no_plan", "no arrow target and no -bot_goto");
    }
    if (!mPlanned || tx != mTx || ty != mTy || is_arrow != mTargetIsArrow)
    {
        BotDecision("f=%d walk target %s (%.1f,%.1f) from (%.1f,%.1f)", ctx.frame, is_arrow ? "arrow" : "point", tx, ty, pl.x, pl.y);
        mTx = tx;
        mTy = ty;
        mTargetIsArrow = is_arrow;
        ctx.nav->ClearBansUnlessSameTarget(mTx, mTy);
        mRetries = 0;
        if (!Plan(ctx, "new target"))
        {
            return Fail("no_plan", "no path on foot from (%.0f,%.0f) to (%.0f,%.0f)", pl.x, pl.y, mTx, mTy);
        }
    }

    if (ctx.frame < mWaitUntil)
    {
        mProgressFrame = ctx.frame;
        return Running;
    }

    const float dx = mTx - pl.x;
    const float dy = mTy - pl.y;
    const float dist = sqrtf(dx * dx + dy * dy);
    const s32 last = static_cast<s32>(ctx.nav->PathLength()) - 1;
    if (dist < mStopDist || (ctx.nav->Approximate() && last >= 0 && mPathIdx >= last &&
                             (pl.x - (ctx.nav->PathAt(last).x + 0.5f)) * (pl.x - (ctx.nav->PathAt(last).x + 0.5f)) + (pl.y - (ctx.nav->PathAt(last).y + 0.5f)) * (pl.y - (ctx.nav->PathAt(last).y + 0.5f)) < 0.5f * 0.5f))
    {
        BotDecision("f=%d walk arrived at (%.1f,%.1f), dist %.2f, %d retries", ctx.frame, mTx, mTy, dist, mRetries);
        return Done;
    }

    // Follow the path: jump to the node after the one the player stands on, advance when close
    const s32 len = static_cast<s32>(ctx.nav->PathLength());
    const s32 px = static_cast<s32>(pl.x);
    const s32 py = static_cast<s32>(pl.y);
    for (s32 j = len - 1; j > mPathIdx; j--)
    {
        if (ctx.nav->PathAt(j).x == px && ctx.nav->PathAt(j).y == py)
        {
            mPathIdx = j + 1 < len ? j + 1 : len - 1;
            mProgressFrame = ctx.frame;
            break;
        }
    }
    while (mPathIdx < len - 1)
    {
        const float wx = ctx.nav->PathAt(mPathIdx).x + 0.5f - pl.x;
        const float wy = ctx.nav->PathAt(mPathIdx).y + 0.5f - pl.y;
        if (wx * wx + wy * wy > 0.6f * 0.6f)
        {
            break;
        }
        mPathIdx++;
        mProgressFrame = ctx.frame;
    }

    // Rolling checkpoints while things go well
    if (ctx.map && ctx.frame >= mCkNext && ctx.frame - mProgressFrame < 20 && !pl.dead)
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

    // About to step onto a road with traffic coming: wait on the pavement. Only when not already on
    // the road (once on it, the quickest way is across).
    if (WorldModel::TileType(static_cast<int>(pl.x), static_cast<int>(pl.y)) != 1)
    {
        bool road_ahead = false;
        float rx = 0, ry = 0;
        for (s32 k = mPathIdx; k < len && k < mPathIdx + 3; k++)
        {
            if (WorldModel::TileType(ctx.nav->PathAt(k).x, ctx.nav->PathAt(k).y) == 1)
            {
                road_ahead = true;
                rx = ctx.nav->PathAt(k).x + 0.5f;
                ry = ctx.nav->PathAt(k).y + 0.5f;
                break;
            }
        }
        if (road_ahead && mTrafficWait < 240)
        {
            bool danger = false;
            for (int c = 0; c < ctx.world->car_count; c++)
            {
                const WorldModel::CarInfo& car = ctx.world->cars[c];
                const float cdx = car.x - rx;
                const float cdy = car.y - ry;
                if (car.speed > 0.02f && cdx * cdx + cdy * cdy < 10.0f * 10.0f)
                {
                    danger = true;
                    break;
                }
            }
            if (danger)
            {
                mTrafficWait++;
                mProgressFrame = ctx.frame;
                if (mTrafficWait == 1)
                {
                    BotDecision("f=%d walk: waiting for traffic before the road at (%.0f,%.0f)", ctx.frame, rx, ry);
                }
                return Running;
            }
        }
    }

    // A parked car in the way (the one we just left, a wreck): slide round it, one side then the other
    if (ctx.frame < mNudgeUntil)
    {
        const float nerr = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, mNudgeX, mNudgeY, true);
        if (fabsf(nerr) < 40.0f)
        {
            ctx.input |= input_bit::Forward;
        }
        mProgressFrame = ctx.frame;
        return Running;
    }
    if (ctx.frame - mProgressFrame > 15 && mNudges < 10 && ctx.world->car_count > 0 && ctx.world->cars[0].dist < 1.9f && ctx.world->cars[0].speed < 0.03f)
    {
        const WorldModel::CarInfo& pc = ctx.world->cars[0];
        float ax = pl.x - pc.x;
        float ay = pl.y - pc.y;
        const float al = sqrtf(ax * ax + ay * ay);
        if (al < 0.01f)
        {
            ax = 1.0f;
            ay = 0.0f;
        }
        else
        {
            ax /= al;
            ay /= al;
        }
        // away from the car, turned 45 degrees to the left or the right of that
        const float turn = (mNudges % 2 == 0) ? 1.2f : -1.2f; // nearly sideways: round the bumper
        const float c = cosf(turn);
        const float s = sinf(turn);
        mNudgeX = pl.x + (ax * c - ay * s) * 2.5f;
        mNudgeY = pl.y + (ax * s + ay * c) * 2.5f;
        mNudgeUntil = ctx.frame + 36;
        BotDecision("f=%d walk: parked car #%d %.1f tiles away in the way at (%.1f,%.1f); sliding round it towards (%.1f,%.1f) (%d)", ctx.frame, pc.idx, pc.dist, pl.x, pl.y, mNudgeX, mNudgeY, mNudges + 1);
        mNudges++;
        return Running;
    }

    // Still no progress: jump at the next waypoint (the ped can hop low obstacles: kerbs, bollards, a bumper)
    if (ctx.frame < mJumpUntil)
    {
        const float jerr = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, ctx.nav->PathAt(mPathIdx).x + 0.5f, ctx.nav->PathAt(mPathIdx).y + 0.5f, true);
        if (fabsf(jerr) < 40.0f)
        {
            ctx.input |= input_bit::Forward;
        }
        if (ctx.frame - mJumpStart < 3)
        {
            ctx.input |= input_bit::HandbrakeJump;
        }
        mProgressFrame = ctx.frame;
        return Running;
    }
    if (ctx.frame - mProgressFrame > 30 && mJumps < 4 && mPathIdx < len)
    {
        mJumps++;
        mJumpStart = ctx.frame;
        mJumpUntil = ctx.frame + 25;
        BotDecision("f=%d walk: no progress at (%.1f,%.1f) towards tile (%d,%d); jumping (%d)", ctx.frame, pl.x, pl.y, ctx.nav->PathAt(mPathIdx).x, ctx.nav->PathAt(mPathIdx).y, mJumps);
        return Running;
    }

    // A car in the way (traffic at a crossing): step back and let it go before calling it stuck
    if (ctx.frame < mYieldUntil)
    {
        mProgressFrame = ctx.frame; // stand still while a moving car goes by
        return Running;
    }
    if (ctx.frame - mProgressFrame > 25 && mYields < 3 && ctx.world->car_count > 0 && ctx.world->cars[0].dist < 1.9f && ctx.world->cars[0].speed >= 0.03f)
    {
        mYields++;
        mYieldUntil = ctx.frame + 30;
        BotDecision("f=%d walk: car #%d (driver %d) %.1f tiles away in the way at (%.1f,%.1f); yielding (%d)", ctx.frame, ctx.world->cars[0].idx, ctx.world->cars[0].has_driver, ctx.world->cars[0].dist, pl.x, pl.y, mYields);
        return Running;
    }

    if (ctx.frame - mProgressFrame > 70)
    {
        const NavNode blocked = ctx.nav->PathAt(mPathIdx);
        char detail[200];
        char near_things[120];
        near_things[0] = 0;
        if (ctx.world->car_count > 0)
        {
            const WorldModel::CarInfo& c = ctx.world->cars[0];
            _snprintf(near_things, sizeof(near_things), "; nearest car #%d model %d at %.1f (driver %d)", c.idx, c.model, c.dist, c.has_driver);
        }
        _snprintf(detail, sizeof(detail), "no new waypoint for 70 frames at (%.1f,%.1f), waypoint %d/%d tile (%d,%d), dist %.1f to (%.1f,%.1f)%s",
                  pl.x, pl.y, mPathIdx, len, blocked.x, blocked.y, dist, mTx, mTy, near_things);
        if (mRetries >= 4 || !ctx.action_snap)
        {
            return Fail("no_progress", "%s", detail);
        }
        mRetries++;
        BotDecision("f=%d walk STUCK %s; retry %d: ban tile (%d,%d), restore the action start (f=%d) and replan", ctx.frame, detail, mRetries, blocked.x, blocked.y, ctx.action_start_frame);
        // Variants: ban more of the area around the blocked tile each time, and stand still for a
        // random time first so that traffic and pedestrians are elsewhere
        const s32 radius = mRetries >= 3 ? 2 : (mRetries >= 2 ? 1 : 0);
        for (s32 by = -radius; by <= radius; by++)
        {
            for (s32 bx = -radius; bx <= radius; bx++)
            {
                ctx.nav->Ban(static_cast<u8>(blocked.x + bx), static_cast<u8>(blocked.y + by));
            }
        }
        const s32 wait = static_cast<s32>(ctx.rng->Below(90));
        Snapshot* restore_from = ctx.action_snap;
        s32 restore_frame = ctx.action_start_frame;
        // the older of the two checkpoints; the other may already be inside the same trouble
        const s32 older = (mCkFrame[0] >= 0 && (mCkFrame[1] < 0 || mCkFrame[0] <= mCkFrame[1])) ? 0 : 1;
        if (mCkFrame[older] >= 0 && mCk[older])
        {
            restore_from = mCk[older];
            restore_frame = mCkFrame[older];
        }
        restore_from->Restore();
        BotDecision("f=%d walk: rewound to the checkpoint of f=%d", ctx.frame, restore_frame);
        ctx.world->Observe();
        mWaitUntil = ctx.frame + wait;
        BotDecision("f=%d walk retry %d: ban radius %d, wait %d frames", ctx.frame, mRetries, radius, wait);
        if (!Plan(ctx, "retry"))
        {
            return Fail("no_plan", "no path left after banning %u tiles", ctx.nav->BanCount());
        }
        return Running;
    }

    float aim_x = ctx.nav->PathAt(mPathIdx).x + 0.5f;
    float aim_y = ctx.nav->PathAt(mPathIdx).y + 0.5f;
    if (mPathIdx == len - 1)
    {
        aim_x = mTx;
        aim_y = mTy;
    }
    const float err = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, aim_x, aim_y, true);
    // Turn first, then walk: heading 40 degrees off drifts into building corners
    if (fabsf(err) < 22.0f)
    {
        ctx.input |= input_bit::Forward;
    }
    return Running;
}
