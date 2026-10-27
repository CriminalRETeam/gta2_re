#include "WalkAction.hpp"
#include "WorldModel.hpp"
#include "NavFoot.hpp"
#include "Snapshot.hpp"
#include "BotLog.hpp"
#include "BotOptions.hpp"
#include "input_bits.hpp"
#include <math.h>
#include <stdio.h>

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
    , mCalibrating(false)
{
}

void WalkAction::Start(BotContext& ctx)
{
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
        return Fail("action_failed", "walk: the player is in a car");
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
        ctx.nav->ClearBans();
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
    if (dist < mStopDist)
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

    if (ctx.frame - mProgressFrame > 120)
    {
        const NavNode blocked = ctx.nav->PathAt(mPathIdx);
        char detail[200];
        _snprintf(detail, sizeof(detail), "no new waypoint for 120 frames at (%.1f,%.1f), waypoint %d/%d tile (%d,%d), dist %.1f to (%.1f,%.1f)",
                  pl.x, pl.y, mPathIdx, len, blocked.x, blocked.y, dist, mTx, mTy);
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
        ctx.action_snap->Restore();
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
    if (fabsf(err) < 40.0f)
    {
        ctx.input |= input_bit::Forward;
    }
    return Running;
}
