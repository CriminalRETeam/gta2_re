#include "CollectActions.hpp"
#include "HoseRule.hpp"
#include <stdio.h>
#include "ObjectTouch.hpp"
#include "BlastAvoid.hpp"
#include "FrenzyRange.hpp"
#include "WalkAction.hpp"
#include "WorldModel.hpp"
#include "BotLog.hpp"
#include "Snapshot.hpp"
#include "BotOptions.hpp"
#include "NavFoot.hpp"
#include "CarActions.hpp"
#include "input_bits.hpp"
#include <math.h>
#include <string.h>
#include "BotCompat.hpp"

PickUpAction::PickUpAction(s32 object_id, float x, float y)
    : mId(object_id)
    , mX(x)
    , mY(y)
    , mWalk(NULL)
    , mWiggle(false)
    , mWiggleStart(0)
{
}

// The spot and reach for touching object id (the middle of its collision box, ObjectTouch.hpp); (fx, fy) where it was last seen
static TouchBox Collect_TouchBox(const WorldModel::BoxInfo& b)
{
    TouchBox t;
    t.valid = b.valid;
    t.cx = b.cx;
    t.cy = b.cy;
    t.ax = b.ax;
    t.ay = b.ay;
    t.hl = b.hl;
    t.hw = b.hw;
    return t;
}

static TouchAim Collect_TouchAim(const WorldModel& w, s32 id, float fx, float fy, bool* inside_now)
{
    WorldModel::ObjInfo obj;
    if (!w.FindObject(id, &obj))
    {
        TouchBox none;
        none.valid = false;
        none.cx = none.cy = none.ax = none.ay = none.hl = none.hw = 0.0f;
        if (inside_now)
        {
            *inside_now = false;
        }
        return ObjectTouch_Aim(none, fx, fy);
    }
    const TouchBox box = Collect_TouchBox(obj.box);
    if (inside_now)
    {
        *inside_now = ObjectTouch_Inside(box, obj.x, obj.y, w.player.x, w.player.y);
    }
    return ObjectTouch_Aim(box, obj.x, obj.y);
}

PickUpAction::~PickUpAction()
{
    delete mWalk;
}

void PickUpAction::Start(BotContext& /*ctx*/)
{
    delete mWalk;
    mWalk = NULL;
    mWiggle = false;
}

Action::Status PickUpAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.dead)
    {
        return Fail("action_failed", "pick_up: the player died");
    }
    WorldModel::ObjInfo obj;
    if (!ctx.world->FindObject(mId, &obj))
    {
        BotDecision("f=%d pick_up: object #%d gone", ctx.frame, mId);
        return Done;
    }
    bool inside = false;
    const TouchAim aim = Collect_TouchAim(*ctx.world, mId, mX, mY, &inside);
    if (!mWalk)
    {
        BotDecision("f=%d pick_up: object #%d (model %d) box %s centre (%.2f,%.2f) half (%.2f,%.2f): walking to (%.2f,%.2f) within %.2f", ctx.frame, mId, obj.model, obj.box.valid ? "valid" : "MISSING", obj.box.cx, obj.box.cy, obj.box.hl, obj.box.hw, aim.x, aim.y, aim.reach);
        mWalk = new WalkAction(aim.x, aim.y, false, aim.reach);
        mWalk->Start(ctx);
    }
    if (!mWiggle)
    {
        const Status s = mWalk->Step(ctx);
        if (s == Failed)
        {
            return Fail(mWalk->fail_condition, "pick_up #%d: %s", mId, mWalk->fail_detail);
        }
        if (s == Done)
        {
            mWiggle = true;
            mWiggleStart = ctx.frame;
        }
        return Running;
    }
    // On the spot and the object is still there: walk at its exact position
    if (ctx.frame - mWiggleStart > 90)
    {
        return Fail("action_failed", "pick_up: standing at (%.1f,%.1f) for 90 frames, object #%d (model %d at %.1f,%.1f z %.0f) is still there", pl.x, pl.y, mId, obj.model, obj.x, obj.y, obj.z);
    }
    const float err = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, aim.x, aim.y, true);
    if (fabsf(err) < 40.0f && !inside)
    {
        ctx.input |= input_bit::Forward;
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
KillFrenzyAction::KillFrenzyAction()
    : mStart(0)
    , mSeenActive(false)
    , mTargetId(-1)
    , mWalk(NULL)
    , mWalkTargetFrame(0)
    , mKills(0)
    , mStartCash(0)
    , mRetries(0)
    , mStartPassed(0)
    , mStartFailed(0)
    , mRuleTarget(0)
    , mBlackNext(0)
    , mTargetSince(0)
    , mLastProgress(0)
    , mProgressFrame(0)
    , mPickFrame(-100)
    , mPickedId(-1)
    , mBarrier(false)
    , mBarrierCheck(0)
    , mRepo(NULL)
    , mRepoUntil(0)
    , mRepoAngle(0)
    , mBackFrames(0)
    , mBackX(0)
    , mBackY(0)
    , mWander(NULL)
    , mWanderUntil(0)
    , mCharging(false)
    , mChargeStart(0)
    , mCooldownUntil(0)
{
}

bool KillFrenzyAction::IsBlacklisted(s32 id, s32 frame) const
{
    for (int i = 0; i < kBlack; i++)
    {
        if (mBlackId[i] == id && frame < mBlackUntil[i])
        {
            return true;
        }
    }
    return false;
}

static const s32 kFireTruckModel = 17; // car_model_enum::FIRETRUK (Cars_BadModel's 'fire truck')

KillFrenzyAction::~KillFrenzyAction()
{
    delete mRepo;
    delete mWander;
    delete mWalk;
}

void KillFrenzyAction::Start(BotContext& ctx)
{
    mStart = ctx.frame;
    mSeenActive = false;
    mTargetId = -1;
    delete mWalk;
    mWalk = NULL;
    mKills = 0;
    mStartCash = ctx.world->player.cash;
    mRetries = 0;
    memset(mBlackId, 0, sizeof(mBlackId));
    memset(mBlackUntil, 0, sizeof(mBlackUntil));
    mTargetSince = ctx.frame;
    mLastProgress = 0;
    mProgressFrame = ctx.frame;
}

Action::Status KillFrenzyAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.dead)
    {
        if (mRetries >= 3 || !ctx.action_snap)
        {
            return Fail("action_failed", "kill_frenzy: the player died (%d retries)", mRetries);
        }
        mRetries++;
        BotDecision("f=%d kill_frenzy: the player died (hp %d, wanted %d); retry %d: restore the action start (f=%d)", ctx.frame, pl.hp, pl.wanted_stars, mRetries, ctx.action_start_frame);
        ctx.action_snap->Restore();
        ctx.world->Observe();
        mTargetId = -1;
        delete mWalk;
        mWalk = NULL;
        mSeenActive = false;
        mStart = ctx.frame;
        return Running;
    }
    if (ctx.world->kill_frenzy_active)
    {
        if (!mSeenActive)
        {
            mSeenActive = true;
            mStartPassed = ctx.world->secrets_passed;
            mStartFailed = ctx.world->secrets_failed;
            mRuleTarget = ctx.world->bonus.target;
            BotDecision("f=%d kill_frenzy: started after %d frames; rule: event %d occupation %d gang %d remap %d death_cause %d, %d needed; weapon %d", ctx.frame, ctx.frame - mStart,
                        ctx.world->bonus.event_type, ctx.world->bonus.occupation, ctx.world->bonus.gang_idx, ctx.world->bonus.remap, ctx.world->bonus.death_cause, ctx.world->bonus.target, pl.weapon);
        }
    }
    else
    {
        if (mSeenActive)
        {
            const bool passed = ctx.world->secrets_passed > mStartPassed;
            BotDecision("f=%d kill_frenzy: over after %d frames: %s (passed %d -> %d, failed %d -> %d), cash %d -> %d", ctx.frame, ctx.frame - mStart, passed ? "PASSED" : "failed",
                        mStartPassed, ctx.world->secrets_passed, mStartFailed, ctx.world->secrets_failed, mStartCash, pl.cash);
            if (passed)
            {
                return Done;
            }
            if (mRetries >= 3 || !ctx.action_snap)
            {
                return Fail("action_failed", "kill_frenzy: failed (%d retries), the rule wanted %d kills", mRetries, mRuleTarget);
            }
            mRetries++;
            BotDecision("f=%d kill_frenzy: retry %d from the action start (f=%d)", ctx.frame, mRetries, ctx.action_start_frame);
            ctx.action_snap->Restore();
            ctx.world->Observe();
            mTargetId = -1;
            delete mWalk;
            mWalk = NULL;
            mSeenActive = false;
            mStart = ctx.frame;
            return Running;
        }
        if (ctx.frame - mStart > 90)
        {
            return Fail("action_failed", "kill_frenzy: no frenzy became active within 90 frames");
        }
        return Running;
    }
    if (ctx.frame - mStart > 4000)
    {
        return Fail("action_failed", "kill_frenzy: still going after 4000 frames");
    }
    if ((ctx.frame - mStart) % 40 == 0 && mTargetId >= 0)
    {
        for (int i = 0; i < ctx.world->ped_count; i++)
        {
            if (ctx.world->peds[i].id == mTargetId)
            {
                const WorldModel::PedInfo& t = ctx.world->peds[i];
                BotDecision("f=%d   target #%d hp %d state %d/%d d=%.1f at (%.1f,%.1f,z%.1f) me (%.1f,%.1f,z%.1f) rot %.0f | reload %d shot %d pressed %d attacking %d my state %d", ctx.frame, t.id, t.hp, t.state1, t.state2, t.dist, t.x, t.y, t.z, pl.x, pl.y, pl.z, pl.rot_deg, pl.reload, pl.shot_fired, pl.attack_pressed, pl.ped_attacking, pl.ped_state);
            }
        }
    }
    if ((ctx.frame - mStart) % 60 == 0)
    {
        BotDecision("f=%d kill_frenzy: hp %d wanted %d weapon %d pagers t=%d,%d v=%d,%d cash %d", ctx.frame, pl.hp, pl.wanted_stars, pl.weapon, ctx.world->pager_timer[0], ctx.world->pager_timer[1],
                    ctx.world->pager_value[0], ctx.world->pager_value[1], pl.cash);
    }

    // In a fire truck (the frenzy of the water jet): drive at the pedestrians and hold the fire button when the nose is on one within reach.
    // The jet fires straight ahead from the cab, so it is the truck that is turned, not a gun that is aimed
    if (pl.in_car)
    {
        WorldModel::CarInfo mine;
        if (ctx.world->FindCar(pl.car_id, &mine) && mine.model == kFireTruckModel)
        {
            const WorldModel::PedInfo* target = NULL;
            for (int i = 0; i < ctx.world->match_count && !target; i++)
            {
                if (!ctx.world->match_peds[i].dead && !ctx.world->match_peds[i].in_car && !IsBlacklisted(ctx.world->match_peds[i].id, ctx.frame))
                {
                    target = &ctx.world->match_peds[i];
                }
            }
            for (int i = 0; i < ctx.world->ped_count && !target; i++)
            {
                if (!ctx.world->peds[i].dead && !ctx.world->peds[i].in_car && !ctx.world->peds[i].cop)
                {
                    target = &ctx.world->peds[i];
                }
            }
            if (target)
            {
                if (ctx.car.valid)
                {
                    const float herr = ctx.Steer(ctx.car, pl.rot_deg, pl.x, pl.y, target->x, target->y, false);
                    const float hdist = sqrtf((target->x - pl.x) * (target->x - pl.x) + (target->y - pl.y) * (target->y - pl.y));
                    const HoseDecision hd = HoseRule_Decide(hdist, herr);
                    ctx.input |= (hd.forward ? input_bit::Forward : 0) | (hd.backward ? input_bit::Backward : 0) | (hd.attack ? input_bit::Attack : 0);
                    if (hd.backward)
                    {
                        // reversing turns the other way
                        const bool l = (ctx.input & input_bit::Left) != 0;
                        const bool rt = (ctx.input & input_bit::Right) != 0;
                        ctx.input &= ~(input_bit::Left | input_bit::Right);
                        ctx.input |= (l ? input_bit::Right : 0) | (rt ? input_bit::Left : 0);
                    }
                    if ((ctx.frame - mStart) % 60 == 0)
                    {
                        BotDecision("f=%d kill_frenzy: fire truck, hose on ped #%d %.1f tiles off, %.0f degrees off the nose: %s", ctx.frame, target->id, hdist, herr, hd.attack ? "firing" : "closing");
                    }
                }
                else
                {
                    ctx.input |= input_bit::Forward;
                }
            }
            return Running;
        }
    }

    // Engagement range by weapon: explosives need distance, flames and shocks need closeness
    const bool car_mode = ctx.world->bonus.active && ctx.world->bonus.event_type == 1;
    const FrenzyRange range = FrenzyRange_Of(pl.weapon, car_mode, mRetries);
    const float min_d = range.min_d;
    const float max_d = range.max_d;

    // Target. A bonus rule says what counts: cars destroyed, or pedestrians of a kind killed (and
    // killing other pedestrians can cancel the rule). Without a rule: the nearest civilian.
    // After a failed try the choice among the nearest three is random.
    s32 tid = -1;
    float tx = 0, ty = 0, tdist = 0;
    bool have_target = false;
    const WorldModel::PedInfo* ped_target = NULL;
    if (car_mode)
    {
        const int n = ctx.world->car_count;
        BlastHazard hot[WorldModel::kMaxCars];
        int hot_count = 0;
        for (int i = 0; i < n; i++)
        {
            if (BlastAvoid_IsHazard(ctx.world->cars[i].damage))
            {
                hot[hot_count].x = ctx.world->cars[i].x;
                hot[hot_count].y = ctx.world->cars[i].y;
                hot[hot_count].damage = ctx.world->cars[i].damage;
                hot_count++;
            }
        }
        const BlastWorld blast = {pl.x, pl.y, hot, hot_count, NULL, NULL};
        int usable[WorldModel::kMaxCars];
        int usable_count = 0;
        for (int i = 0; i < n; i++)
        {
            const WorldModel::CarInfo& c = ctx.world->cars[i];
            if (IsBlacklisted(c.idx, ctx.frame) || c.police || c.damage >= 32000 || c.dist > 80.0f || (ctx.world->bonus.car_model != 87 && c.model != ctx.world->bonus.car_model))
            {
                continue;
            }
            // A car that is about to blow, and the ones next to it that go up with it, need no more shooting: move on
            if (BlastAvoid_IsHazard(c.damage) || BlastAvoid_WillChain(blast, c.x, c.y))
            {
                continue;
            }
            if (c.idx == mTargetId)
            {
                tid = c.idx;
                tx = c.x;
                ty = c.y;
                tdist = c.dist;
                have_target = true;
                break;
            }
            usable[usable_count++] = i;
        }
        if (!have_target && usable_count > 0)
        {
            const int pick = mRetries > 0 ? static_cast<int>(ctx.rng->Below(usable_count < 3 ? usable_count : 3)) : 0;
            const WorldModel::CarInfo& c = ctx.world->cars[usable[pick]];
            tid = c.idx;
            tx = c.x;
            ty = c.y;
            tdist = c.dist;
            have_target = true;
        }
    }
    else
    {
        if (ctx.world->bonus.active && ctx.world->bonus.event_type == 0)
        {
            const int n = ctx.world->match_count;
            for (int i = 0; i < n; i++)
            {
                if (ctx.world->match_peds[i].id == mTargetId && !IsBlacklisted(mTargetId, ctx.frame))
                {
                    ped_target = &ctx.world->match_peds[i];
                    break;
                }
            }
            if (!ped_target && n > 0)
            {
                int usable[WorldModel::kMaxPeds];
                int uc = 0;
                for (int i = 0; i < n; i++)
                {
                    if (!IsBlacklisted(ctx.world->match_peds[i].id, ctx.frame))
                    {
                        usable[uc++] = i;
                    }
                }
                if (uc > 0)
                {
                    int pick = 0;
                    if (mRetries > 0)
                    {
                        pick = static_cast<int>(ctx.rng->Below(uc < 3 ? uc : 3));
                    }
                    else if (!mWalk && !mRepo)
                    {
                        // The one with the shortest way to it: pedestrians behind a fence cost the way round
                        float best = 1e9f;
                        for (int k = 0; k < uc; k++)
                        {
                            const WorldModel::PedInfo& c = ctx.world->match_peds[usable[k]];
                            float score = c.dist * 2.0f + 40.0f;
                            if (c.dist < 16.0f &&
                                ctx.nav->FindPath(static_cast<u8>(pl.x), static_cast<u8>(pl.y), static_cast<u8>(pl.z + 0.5f), static_cast<u8>(c.x), static_cast<u8>(c.y)))
                            {
                                score = static_cast<float>(ctx.nav->PathLength()) + c.dist * 0.3f;
                            }
                            if (score < best)
                            {
                                best = score;
                                pick = k;
                            }
                        }
                    }
                    ped_target = &ctx.world->match_peds[usable[pick]];
                }
            }
        }
        else
        {
            for (int i = 0; i < ctx.world->ped_count; i++)
            {
                const WorldModel::PedInfo& p = ctx.world->peds[i];
                if (IsBlacklisted(p.id, ctx.frame) || p.cop || p.in_car || p.dead || p.dist > 60.0f || p.x <= 0 || p.y <= 0)
                {
                    continue;
                }
                if (p.id == mTargetId)
                {
                    ped_target = &p;
                    break;
                }
                if (!ped_target)
                {
                    ped_target = &p;
                }
            }
        }
        if (ped_target)
        {
            tid = ped_target->id;
            tx = ped_target->x;
            ty = ped_target->y;
            tdist = ped_target->dist;
            have_target = true;
        }
    }
    if (!have_target && ctx.world->bonus.active && !ctx.world->bonus.cancel_on_mismatch && ctx.world->bonus.event_type == 0)
    {
        // Nobody of the kind the rule wants is around. Other pedestrians do not cancel the rule:
        // attacking them stirs things up (the police come for a rule about cops) and the kind we
        // want turns up
        for (int i = 0; i < ctx.world->ped_count; i++)
        {
            const WorldModel::PedInfo& p = ctx.world->peds[i];
            if (!IsBlacklisted(p.id, ctx.frame) && !p.in_car && !p.dead && !p.matches_bonus && p.dist < 40.0f && p.x > 0 && p.y > 0)
            {
                ped_target = &p;
                tid = p.id;
                tx = p.x;
                ty = p.y;
                tdist = p.dist;
                have_target = true;
                break;
            }
        }
    }
    if (!have_target)
    {
        // Nothing to shoot at: wander to find them
        if (!mWander || ctx.frame >= mWanderUntil)
        {
            delete mWander;
            const float ang = static_cast<float>(ctx.rng->Below(360)) * 3.14159265f / 180.0f;
            const float r = 8.0f + static_cast<float>(ctx.rng->Below(20));
            mWander = new WalkAction(pl.x + sinf(ang) * r, pl.y - cosf(ang) * r, false, 2.0f);
            mWander->Start(ctx);
            mWanderUntil = ctx.frame + 300;
        }
        const Status ws = mWander->Step(ctx);
        if (ws != Running)
        {
            delete mWander;
            mWander = NULL;
        }
        return Running;
    }
    delete mWander;
    mWander = NULL;
    // A target that has not led to progress for 200 frames is given up for a while
    if (ctx.world->bonus.progress != mLastProgress)
    {
        mLastProgress = ctx.world->bonus.progress;
        mProgressFrame = ctx.frame;
    }
    if (tid == mTargetId && ctx.frame - mTargetSince > 100 && ctx.frame - mProgressFrame > 100)
    {
        mBlackId[mBlackNext % kBlack] = tid;
        mBlackUntil[mBlackNext % kBlack] = ctx.frame + 300;
        mBlackNext++;
        BotDecision("f=%d kill_frenzy: no progress on target #%d for 100 frames, leaving it for 300 frames", ctx.frame, tid);
        mTargetId = -1;
        mTargetSince = ctx.frame;
        mProgressFrame = ctx.frame;
        return Running;
    }
    if (tid != mTargetId)
    {
        mTargetSince = ctx.frame;
        mTargetId = tid;
        if (ped_target)
        {
            BotDecision("f=%d kill_frenzy: target ped #%d (occupation %s, gang %d, remap %d) at (%.0f,%.0f) dist %.0f; rule progress %d/%d", ctx.frame, tid,
                        WorldModel_OccupationName(ped_target->occupation), ped_target->gang_idx, ped_target->remap, tx, ty, tdist, ctx.world->bonus.progress, ctx.world->bonus.target);
        }
        else
        {
            BotDecision("f=%d kill_frenzy: target car #%d at (%.0f,%.0f) dist %.0f; rule progress %d/%d", ctx.frame, tid, tx, ty, tdist, ctx.world->bonus.progress, ctx.world->bonus.target);
        }
    }

    // Is there a wall or fence in between? The walking path is much longer than the straight line
    if (!ped_target || tdist > max_d + 2.0f)
    {
        mBarrier = false;
    }
    else if (!mWalk && !mRepo && ctx.frame - mBarrierCheck > 20)
    {
        mBarrierCheck = ctx.frame;
        const bool found = ctx.nav->FindPath(static_cast<u8>(pl.x), static_cast<u8>(pl.y), static_cast<u8>(pl.z + 0.5f), static_cast<u8>(tx), static_cast<u8>(ty));
        const bool was = mBarrier;
        mBarrier = !found || ctx.nav->PathLength() > tdist * 1.8f + 3.0f;
        if (mBarrier && !was)
        {
            BotDecision("f=%d kill_frenzy: target #%d at %.1f is behind something (%s, %u tiles of walking): going round", ctx.frame, tid, tdist, found ? "path" : "no path", ctx.nav->PathLength());
        }
    }
    if (tdist > max_d + 2.0f || mBarrier)
    {
        // Chase with a path, re-made every 45 frames because the target moves
        if (!mWalk || ctx.frame - mWalkTargetFrame > 45)
        {
            delete mWalk;
            mWalk = new WalkAction(tx, ty, false, mBarrier ? 2.0f : max_d - 1.0f);
            mWalk->Start(ctx);
            mWalkTargetFrame = ctx.frame;
        }
        const Status s = mWalk->Step(ctx);
        if (s != Running)
        {
            delete mWalk;
            mWalk = NULL;
        }
        return Running;
    }
    delete mWalk;
    mWalk = NULL;

    // Repositioning: a walk to a point at firing distance from the target
    if (mRepo)
    {
        const Status rs = mRepo->Step(ctx);
        if (rs != Running || ctx.frame >= mRepoUntil)
        {
            delete mRepo;
            mRepo = NULL;
        }
        return Running;
    }
    const float err = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, tx, ty, true);
    if (tdist < min_d)
    {
        // too close for this weapon: back off, and when the back is against a wall walk round
        if (mBackFrames == 0)
        {
            mBackX = pl.x;
            mBackY = pl.y;
        }
        mBackFrames++;
        if (mBackFrames >= 20)
        {
            const float moved = sqrtf((pl.x - mBackX) * (pl.x - mBackX) + (pl.y - mBackY) * (pl.y - mBackY));
            mBackFrames = 0;
            if (moved < 0.4f)
            {
                const float ring = (min_d + max_d) * 0.5f;
                const float ang = (mRepoAngle++ * 45.0f) * 3.14159265f / 180.0f;
                mRepo = new WalkAction(tx + sinf(ang) * ring, ty - cosf(ang) * ring, false, 1.5f);
                mRepo->Start(ctx);
                mRepoUntil = ctx.frame + 150;
                BotDecision("f=%d kill_frenzy: stuck against something at %.1f from the target; repositioning to %.0f degrees", ctx.frame, tdist, mRepoAngle * 45.0f);
                return Running;
            }
        }
        ctx.input |= input_bit::Backward;
    }
    else if (tdist > max_d && fabsf(err) < 40.0f)
    {
        mBackFrames = 0;
        ctx.input |= input_bit::Forward;
    }
    else
    {
        mBackFrames = 0;
    }
    const bool explosive = pl.weapon == 2 || pl.weapon == 4 || pl.weapon == 5;
    // A shot that bursts within min_d of the player burns the player: look along the line of fire
    // for anything that is not the target (pedestrians, cars, walls) closer than the blast range
    bool clear_shot = true;
    if (explosive && tdist > 0.5f)
    {
        const float ux = (tx - pl.x) / tdist;
        const float uy = (ty - pl.y) / tdist;
        const float reach = min_d < tdist ? min_d : tdist;
        for (int i = 0; i < ctx.world->ped_count && clear_shot; i++)
        {
            const WorldModel::PedInfo& p = ctx.world->peds[i];
            if (p.id == tid || p.dead)
            {
                continue;
            }
            const float along = (p.x - pl.x) * ux + (p.y - pl.y) * uy;
            const float side = (p.x - pl.x) * uy - (p.y - pl.y) * ux;
            if (along > -0.3f && along < reach + 1.0f && fabsf(side) < 1.1f)
            {
                clear_shot = false;
            }
        }
        for (int i = 0; i < ctx.world->car_count && clear_shot; i++)
        {
            const WorldModel::CarInfo& c = ctx.world->cars[i];
            if (c.idx == tid)
            {
                continue;
            }
            const float along = (c.x - pl.x) * ux + (c.y - pl.y) * uy;
            const float side = (c.x - pl.x) * uy - (c.y - pl.y) * ux;
            if (along > -0.3f && along < reach + 1.5f && fabsf(side) < 1.6f)
            {
                clear_shot = false;
            }
        }
        for (float d = 0.5f; d < reach + 1.0f && clear_shot; d += 0.5f)
        {
            if (WorldModel::BuildingAt(pl.x + ux * d, pl.y + uy * d))
            {
                clear_shot = false;
            }
        }
    }
    if (!clear_shot)
    {
        ctx.input |= input_bit::Backward; // something is in the way: get some room
    }
    const bool fire_ok = FrenzyRange_FireOk(range, err, tdist, clear_shot);
    if (pl.weapon == 4 || pl.weapon == 5)
    {
        // Thrown weapons fly when the button is let go; the range is the time it was held
        if (fire_ok && ctx.frame >= mCooldownUntil)
        {
            if (!mCharging)
            {
                mCharging = true;
                mChargeStart = ctx.frame;
            }
            s32 want = static_cast<s32>(tdist * 5.0f);
            want = want < 8 ? 8 : (want > 70 ? 70 : want);
            if (ctx.frame - mChargeStart >= want)
            {
                mCharging = false;
                mCooldownUntil = ctx.frame + 10;
            }
            else
            {
                ctx.input |= input_bit::Attack;
            }
        }
        else if (mCharging && !fire_ok)
        {
            if (ctx.frame - mChargeStart > 40)
            {
                mCharging = false; // lined up too slowly: let it go
                mCooldownUntil = ctx.frame + 10;
            }
            else
            {
                ctx.input |= input_bit::Attack; // keep the charge while lining up
            }
        }
        return Running;
    }
    if (fire_ok)
    {
        // pulse the button: the game acts on presses, a button held forever stops firing
        const s32 mode = ctx.opts->fire_mode;
        bool press = true;
        if (mode == 0)
        {
            press = (ctx.frame % 8) < 6;
        }
        else if (mode == 2)
        {
            press = (ctx.frame % 4) < 2;
        }
        else if (mode == 3)
        {
            press = (ctx.frame % 2) == 0;
        }
        else if (mode == 4)
        {
            press = false; // control experiment: never attack
        }
        if (press)
        {
            ctx.input |= input_bit::Attack;
        }
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
// Then it steps on towards the middle of its collision box
static const s32 kPhoneSettleFrames = 40; // frames to get inside the box after the walk is over

PhoneAction::PhoneAction(s32 object_id, float x, float y, bool tutorial)
    : mId(object_id)
    , mX(x)
    , mY(y)
    , mWalk(NULL)
    , mOnIt(false)
    , mWalkDoneFrame(0)
    , mOnItFrame(0)
    , mWasOnMission(false)
    , mRefused(false)
    , mBriefFrame(0)
    , mTutorial(tutorial)
    , mDoneFrame(0)
    , mLastLogged(-1000)
{
    mBrief0[0] = 0;
    mBrief[0] = 0;
}

PhoneAction::~PhoneAction()
{
    delete mWalk;
}

void PhoneAction::Start(BotContext& ctx)
{
    mLastLogged = -1000;
    mDoneFrame = 0;
    delete mWalk;
    mWalk = NULL;
    mOnIt = false;
    mRefused = false;
    mBriefFrame = 0;
    mBrief[0] = 0;
    mWasOnMission = ctx.world->on_mission;
    strncpy(mBrief0, ctx.world->brief_key, sizeof(mBrief0) - 1);
    mBrief0[sizeof(mBrief0) - 1] = 0;
}

Action::Status PhoneAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.dead)
    {
        return Fail("action_failed", "phone: the player died");
    }
    if (!mOnIt)
    {
        bool inside = false;
        const TouchAim aim = Collect_TouchAim(*ctx.world, mId, mX, mY, &inside);
        if (!mWalk)
        {
            WorldModel::ObjInfo obj;
            if (ctx.world->FindObject(mId, &obj))
            {
                BotDecision("f=%d phone #%d (model %d): collision box %s centre (%.2f,%.2f) half (%.2f,%.2f), the object's spot (%.2f,%.2f): walking to (%.2f,%.2f) within %.2f", ctx.frame, mId, obj.model, obj.box.valid ? "valid" : "MISSING", obj.box.cx, obj.box.cy, obj.box.hl, obj.box.hw, obj.x, obj.y, aim.x, aim.y, aim.reach);
            }
            mWalk = new WalkAction(aim.x, aim.y, false, aim.reach);
            mWalk->Start(ctx);
            mWalkDoneFrame = 0;
        }
        if (mWalkDoneFrame == 0)
        {
            const Status s = mWalk->Step(ctx);
            if (s == Failed)
            {
                return Fail(mWalk->fail_condition, "phone #%d: %s", mId, mWalk->fail_detail);
            }
            if (s == Done)
            {
                mWalkDoneFrame = ctx.frame;
            }
            return Running;
        }
        // the walk is over: stand well inside the box (not at its edge), a few steps at most
        if (!inside && ctx.frame - mWalkDoneFrame < kPhoneSettleFrames)
        {
            const float err = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, aim.x, aim.y, true);
            if (fabsf(err) < 40.0f)
            {
                ctx.input |= input_bit::Forward;
            }
            return Running;
        }
        mOnIt = true;
        mOnItFrame = ctx.frame;
        BotDecision("f=%d phone: on phone #%d at (%.2f,%.2f) %s the middle of its box (%.2f,%.2f)", ctx.frame, mId, pl.x, pl.y, inside ? "inside" : "NOT inside", aim.x, aim.y);
        return Running;
    }
    if (ctx.world->on_mission && !mWasOnMission)
    {
        BotDecision("f=%d phone: mission started (brief \"%s\")", ctx.frame, ctx.world->brief_key);
        return Done;
    }
    if (strcmp(mBrief0, ctx.world->brief_key) != 0 && ctx.world->brief_key[0])
    {
        // A brief without a mission is the phone's refusal ("kill some Zaibatsu before you return
        // for work"): give the script a few seconds to start the mission anyway
        if (mBriefFrame == 0)
        {
            mBriefFrame = ctx.frame;
            strncpy(mBrief, ctx.world->brief_text, sizeof(mBrief) - 1);
            mBrief[sizeof(mBrief) - 1] = 0;
            BotDecision("f=%d phone: a brief came up (\"%s\" [%s]) on_mission=%d", ctx.frame, ctx.world->brief_key, ctx.world->brief_text, ctx.world->on_mission);
        }
        if (mTutorial)
        {
            // the walk-through: watch what the script does for a while (text, the arrow, tokens) instead of calling it a refusal
            if (ctx.frame - mLastLogged >= 120)
            {
                mLastLogged = ctx.frame;
                BotDecision("f=%d tutorial: brief \"%s\" [%s] on_mission %d arrow type %d at (%.0f,%.0f) tokens left %d secrets passed %d cash %d", ctx.frame, ctx.world->brief_key, ctx.world->brief_text, ctx.world->on_mission,
                            ctx.world->arrow.type, ctx.world->arrow.x, ctx.world->arrow.y, ctx.world->token_count, ctx.world->secrets_passed, pl.cash);
            }
            if (ctx.frame - mBriefFrame > 3000)
            {
                return Done;
            }
            return Running;
        }
        if (ctx.frame - mBriefFrame > 200)
        {
            mRefused = true;
            return Fail("action_failed", "phone #%d: refused: \"%s\"", mId, mBrief);
        }
        return Running;
    }
    if (mTutorial && mBriefFrame != 0)
    {
        // the walk-through's last message is gone: it is over. Give the script a moment to hand out what it hands out
        if (mDoneFrame == 0)
        {
            mDoneFrame = ctx.frame;
            BotDecision("f=%d tutorial: finished (the last message is gone); tokens left %d secrets passed %d secrets failed %d cash %d", ctx.frame, ctx.world->token_count, ctx.world->secrets_passed, ctx.world->secrets_failed, pl.cash);
        }
        if (ctx.frame - mDoneFrame > 240)
        {
            BotDecision("f=%d tutorial: 240 frames after; tokens left %d secrets passed %d secrets failed %d cash %d", ctx.frame, ctx.world->token_count, ctx.world->secrets_passed, ctx.world->secrets_failed, pl.cash);
            return Done;
        }
        return Running;
    }
    if (ctx.frame - mOnItFrame > 120)
    {
        mRefused = true;
        return Fail("action_failed", "phone #%d: standing on it for 120 frames, no mission or brief (on_mission=%d, brief \"%s\")", mId, ctx.world->on_mission, ctx.world->brief_key);
    }
    // step on and off the phone to touch it again
    const float err = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, mX, mY, true);
    if (((ctx.frame - mOnItFrame) / 20) % 2 == 0 && fabsf(err) < 40.0f)
    {
        ctx.input |= input_bit::Forward;
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
MissionWatchAction::MissionWatchAction(s32 frames)
    : mFrames(frames)
    , mStart(0)
{
}

void MissionWatchAction::Start(BotContext& ctx)
{
    mStart = ctx.frame;
}

Action::Status MissionWatchAction::Step(BotContext& ctx)
{
    ctx.input = 0;
    if ((ctx.frame - mStart) % 120 == 0)
    {
        char view[1500];
        ctx.world->Text(view, sizeof(view), "mission_watch");
        BotLog("%s", view);
    }
    return ctx.frame - mStart >= mFrames ? Done : Running;
}

// ---------------------------------------------------------------------------------------------
KillPedAction::KillPedAction(s32 ped_id, float x, float y)
    : mId(ped_id)
    , mX(x)
    , mY(y)
    , mWalk(NULL)
    , mWalkFrame(0)
    , mWalkTx(0)
    , mWalkTy(0)
    , mStart(0)
    , mGoneFrames(0)
    , mExit(NULL)
{
}

KillPedAction::~KillPedAction()
{
    delete mWalk;
    delete mExit;
}

void KillPedAction::Start(BotContext& ctx)
{
    delete mWalk;
    mWalk = NULL;
    delete mExit;
    mExit = NULL;
    mStart = ctx.frame;
    mGoneFrames = 0;
    ctx.world->track_ped_id = mId;
}

Action::Status KillPedAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    ctx.world->track_ped_id = mId;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.dead)
    {
        return Fail("action_failed", "kill_ped: the player died");
    }
    if (ctx.frame - mStart > 3000)
    {
        return Fail("action_failed", "kill_ped: ped #%d still alive after 3000 frames", mId);
    }
    if (pl.in_car)
    {
        if (!mExit)
        {
            mExit = new ExitCarAction();
            mExit->Start(ctx);
        }
        const Status es = mExit->Step(ctx);
        if (es == Failed)
        {
            return Fail(mExit->fail_condition, "kill_ped: %s", mExit->fail_detail);
        }
        return Running;
    }
    delete mExit;
    mExit = NULL;

    if (!ctx.world->tracked_valid || ctx.world->tracked.dead)
    {
        // not in the nearest list or dead: dead for a while means done, far away means go there
        mGoneFrames++;
        if (ctx.world->tracked_valid && ctx.world->tracked.dead)
        {
            BotDecision("f=%d kill_ped: ped #%d is dead", ctx.frame, mId);
            return Done;
        }
        if (mGoneFrames > 40)
        {
            BotDecision("f=%d kill_ped: ped #%d is gone", ctx.frame, mId);
            return Done;
        }
    }
    else
    {
        mGoneFrames = 0;
        mX = ctx.world->tracked.x;
        mY = ctx.world->tracked.y;
    }

    const float dx = mX - pl.x;
    const float dy = mY - pl.y;
    const float dist = sqrtf(dx * dx + dy * dy);
    const bool explosive = pl.weapon == 2 || pl.weapon == 4 || pl.weapon == 5;
    const float stand = explosive ? 7.0f : 4.5f;

    // is there a wall or fence between? Then no standing and shooting here: go round to where the shot is clear
    const bool clear = Bot_LineClear(pl.x, pl.y, mX, mY);
    if (dist > stand + 1.5f || mWalk || !clear)
    {
        // one walk per place the target stands at: rebuilding it each time would hide that it is stuck
        if (mWalk && (mX - mWalkTx) * (mX - mWalkTx) + (mY - mWalkTy) * (mY - mWalkTy) > 3.0f * 3.0f)
        {
            delete mWalk;
            mWalk = NULL;
        }
        if (!mWalk)
        {
            mWalk = new WalkAction(mX, mY, false, clear ? stand : 1.2f);
            mWalk->Start(ctx);
            mWalkFrame = ctx.frame;
            mWalkTx = mX;
            mWalkTy = mY;
        }
        const Status s = mWalk->Step(ctx);
        if (s == Failed)
        {
            delete mWalk;
            mWalk = NULL;
            return Fail("action_failed", "kill_ped #%d: cannot get to it", mId);
        }
        if (s == Done)
        {
            delete mWalk;
            mWalk = NULL;
        }
        return Running;
    }
    delete mWalk;
    mWalk = NULL;
    const float err = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, mX, mY, true);
    if (dist < (explosive ? 4.0f : 1.2f))
    {
        ctx.input |= input_bit::Backward;
    }
    if (fabsf(err) < 30.0f && (ctx.frame % 8) < 6)
    {
        ctx.input |= input_bit::Attack;
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
DestroyCarAction::DestroyCarAction(s32 car_id)
    : mId(car_id)
    , mWalk(NULL)
    , mWalkTx(0)
    , mWalkTy(0)
    , mStart(0)
    , mExit(NULL)
    , mLastX(0)
    , mLastY(0)
    , mLastDamage(0)
    , mLastDamageFrame(0)
    , mLastAmmo(-1)
{
}

DestroyCarAction::~DestroyCarAction()
{
    delete mWalk;
    delete mExit;
}

void DestroyCarAction::Start(BotContext& ctx)
{
    delete mWalk;
    mWalk = NULL;
    delete mExit;
    mExit = NULL;
    mStart = ctx.frame;
    mLastDamage = 0;
    mLastDamageFrame = ctx.frame;
}

Action::Status DestroyCarAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (pl.dead)
    {
        return Fail("action_failed", "destroy_car: the player died");
    }
    if (ctx.frame - mStart > 12000)
    {
        return Fail("action_failed", "destroy_car: car #%d still not destroyed after 12000 frames", mId);
    }
    WorldModel::CarInfo car;
    if (!ctx.world->FindCar(mId, &car))
    {
        BotDecision("f=%d destroy_car: car #%d is gone", ctx.frame, mId);
        return Done;
    }
    if (car.damage >= 32000)
    {
        BotDecision("f=%d destroy_car: car #%d is wrecked (damage %d)", ctx.frame, mId, car.damage);
        return Done;
    }
    if (car.damage != mLastDamage)
    {
        mLastDamage = car.damage;
        mLastDamageFrame = ctx.frame;
    }
    if (pl.in_car)
    {
        if (!mExit)
        {
            mExit = new ExitCarAction();
            mExit->Start(ctx);
        }
        mExit->Step(ctx);
        return Running;
    }
    delete mExit;
    mExit = NULL;

    const float dist = car.dist;
    const bool explosive = pl.weapon == 2 || pl.weapon == 4 || pl.weapon == 5;
    const float stand = explosive ? 9.0f : 6.0f;
    if (ctx.frame % 120 == 0)
    {
        BotDecision("f=%d destroy_car: car #%d model %d at (%.0f,%.0f) dist %.1f damage %d, my weapon %d/%d", ctx.frame, mId, car.model, car.x, car.y, dist, car.damage, pl.weapon, pl.ammo);
    }
    if (dist > stand + 2.0f || mWalk)
    {
        if (mWalk && (car.x - mWalkTx) * (car.x - mWalkTx) + (car.y - mWalkTy) * (car.y - mWalkTy) > 4.0f * 4.0f)
        {
            delete mWalk;
            mWalk = NULL;
        }
        if (!mWalk)
        {
            mWalk = new WalkAction(car.x, car.y, false, stand);
            mWalk->Start(ctx);
            mWalkTx = car.x;
            mWalkTy = car.y;
        }
        const Status s = mWalk->Step(ctx);
        if (s != Running)
        {
            delete mWalk;
            mWalk = NULL;
        }
        if (dist > stand + 2.0f)
        {
            return Running;
        }
    }
    const float err = ctx.Steer(ctx.foot, pl.rot_deg, pl.x, pl.y, car.x, car.y, true);
    // A rocket, grenade or molotov blows up at the target: from closer than about 5 tiles it hurts the one who threw it
    const float too_close = explosive ? 5.5f : 3.0f;
    if (dist < too_close)
    {
        ctx.input |= input_bit::Backward; // back off (a wrecked car burns and blows up too)
    }
    // A shot goes straight ahead: fire only when the car is within the angle that its width covers at this distance
    // (30 degrees was 5 tiles wide at 10 tiles away, so the rockets flew past), and nothing solid is in between
    float tol = atanf(1.3f / (dist > 1.0f ? dist : 1.0f)) * 57.2958f;
    tol = tol < 3.0f ? 3.0f : (tol > 25.0f ? 25.0f : tol);
    const bool line = Bot_LineClear(pl.x, pl.y, car.x, car.y);
    if (fabsf(err) < tol && line && dist >= too_close && dist < stand + 4.0f && (ctx.frame % 8) < 6)
    {
        ctx.input |= input_bit::Attack;
    }
    if (mLastAmmo >= 0 && pl.ammo < mLastAmmo)
    {
        BotDecision("f=%d destroy_car: shot (weapon %d, ammo %d -> %d): aim error %.1f deg (limit %.1f), %.1f tiles, line %s, target speed %.2f damage %d", ctx.frame, pl.weapon, mLastAmmo, pl.ammo, err, tol, dist, line ? "clear" : "BLOCKED", car.speed, car.damage);
    }
    mLastAmmo = pl.ammo;
    return Running;
}

void PickUpAction::Detail(char* buf, int size) const
{
    _snprintf(buf, size, "walking onto pickup #%d at (%.1f,%.1f)", mId, mX, mY);
    buf[size - 1] = 0;
}
