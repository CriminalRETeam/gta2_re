#include "MissionActions.hpp"
#include "WalkAction.hpp"
#include "CarActions.hpp"
#include "CollectActions.hpp"
#include "WorldModel.hpp"
#include "NavFoot.hpp"
#include "Snapshot.hpp"
#include "BotLog.hpp"
#include "BotOptions.hpp"
#include "input_bits.hpp"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "BotCompat.hpp"

MissionAction::MissionAction()
    : mMissionSnap(NULL)
    , mStepSnap(NULL)
    , mChild(NULL)
    , mChildDone(false)
    , mStarted(false)
    , mStartPassed(0)
    , mStart(0)
    , mLastChange(0)
    , mLastArrowType(-1)
    , mRetries(0)
    , mNoArrowFrames(0)
    , mHornUntil(0)
    , mWheelsId(0)
    , mWheelsTried(0)
    , mWheelsSearchFrame(0)
{
    mChildKey[0] = 0;
    mLastBrief[0] = 0;
    mBriefMemory[0] = 0;
}

MissionAction::~MissionAction()
{
    delete mChild;
    delete mStepSnap;
}

void MissionAction::DropChild()
{
    delete mChild;
    mChild = NULL;
    mChildKey[0] = 0;
    mChildDone = false;
}

void MissionAction::Start(BotContext& ctx)
{
    mMissionSnap = ctx.action_snap;
    mBriefMemory[0] = 0;
    DropChild();
    mStarted = false;
    mStart = ctx.frame;
    mLastChange = ctx.frame;
    mStartPassed = ctx.world->missions_passed;
    mRetries = 0;
    mNoArrowFrames = 0;
    strncpy(mLastBrief, ctx.world->brief_key, sizeof(mLastBrief) - 1);
    mLastBrief[sizeof(mLastBrief) - 1] = 0;
}

bool MissionAction::Restart(BotContext& ctx, const char* why)
{
    if (mRetries >= 3 || !mMissionSnap)
    {
        return false;
    }
    mRetries++;
    BotDecision("f=%d mission: %s; retry %d from the mission start (f=%d)", ctx.frame, why, mRetries, ctx.action_start_frame);
    mMissionSnap->Restore();
    ctx.action_snap = mMissionSnap;
    ctx.world->Observe();
    DropChild();
    mStarted = false;
    mLastChange = ctx.frame;
    mNoArrowFrames = 0;
    mLastArrowType = -1;
    return true;
}

// A parked, unlocked, undamaged car within 30 tiles that can be reached on foot
bool MissionAction::FindNearbyCar(BotContext& ctx, s32* out_id)
{
    if (mWheelsTried >= 3)
    {
        return false;
    }
    if (mWheelsId != 0)
    {
        WorldModel::CarInfo c;
        if (ctx.world->FindCar(mWheelsId, &c) && !c.has_driver && !c.locked)
        {
            *out_id = mWheelsId;
            return true;
        }
    }
    if (ctx.frame < mWheelsSearchFrame)
    {
        return false;
    }
    mWheelsSearchFrame = ctx.frame + 60;
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    for (int i = 0; i < ctx.world->car_count; i++)
    {
        const WorldModel::CarInfo& c = ctx.world->cars[i];
        if (c.has_driver || c.locked || c.police || c.mission || c.damage > 2000 || c.dist > 30.0f)
        {
            continue;
        }
        if (ctx.nav->FindPath(static_cast<u8>(pl.x), static_cast<u8>(pl.y), static_cast<u8>(pl.z + 0.5f), static_cast<u8>(c.x), static_cast<u8>(c.y)) && ctx.nav->PathLength() < 60)
        {
            *out_id = c.idx;
            mWheelsId = c.idx;
            mWheelsTried++;
            BotDecision("f=%d mission: the target is far; taking car #%d (model %d) %.0f tiles away first", ctx.frame, c.idx, c.model, c.dist);
            return true;
        }
    }
    return false;
}

Action::Status MissionAction::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }

    if (ctx.world->on_mission)
    {
        mStarted = true;
    }
    else if (mStarted)
    {
        // The script ended the mission: passed or failed?
        const bool passed = ctx.world->missions_passed > mStartPassed;
        BotDecision("f=%d mission: over after %d frames: %s (passed %d -> %d), cash %d", ctx.frame, ctx.frame - mStart, passed ? "PASSED" : "failed", mStartPassed, ctx.world->missions_passed, pl.cash);
        if (passed)
        {
            return Done;
        }
        if (!Restart(ctx, "the mission failed"))
        {
            return Fail("action_failed", "mission: failed (%d retries)", mRetries);
        }
        return Running;
    }
    else if (ctx.frame - mStart > 600)
    {
        return Fail("action_failed", "mission: no mission became active within 600 frames");
    }

    if (pl.dead)
    {
        if (!Restart(ctx, "the player died"))
        {
            return Fail("action_failed", "mission: the player died (%d retries)", mRetries);
        }
        return Running;
    }

    if (ctx.world->brief_text[0])
    {
        strncpy(mBriefMemory, ctx.world->brief_text, sizeof(mBriefMemory) - 1);
        mBriefMemory[sizeof(mBriefMemory) - 1] = 0;
    }

    // Progress: any change of arrow type, brief or the arrow target resets the timeout
    const WorldModel::ArrowInfo& arrow = ctx.world->arrow;
    const s32 effective_type = arrow.valid ? arrow.type : -1;
    if (effective_type != mLastArrowType || strcmp(mLastBrief, ctx.world->brief_key) != 0)
    {
        mLastArrowType = effective_type;
        strncpy(mLastBrief, ctx.world->brief_key, sizeof(mLastBrief) - 1);
        mLastBrief[sizeof(mLastBrief) - 1] = 0;
        mLastChange = ctx.frame;
        BotDecision("f=%d mission: brief \"%s\" [%.390s], arrow %s type %d at (%.0f,%.0f), I am at (%.0f,%.0f) hp %d %s", ctx.frame, ctx.world->brief_key, ctx.world->brief_text, arrow.valid ? "" : "none", arrow.type, arrow.x, arrow.y, pl.x, pl.y,
                    pl.hp, pl.in_car ? "in a car" : "on foot");
    }
    if (ctx.frame - mLastChange > 5000)
    {
        return Fail("no_progress", "mission: nothing changed for 5000 frames (brief \"%s\", arrow type %d at %.0f,%.0f)", ctx.world->brief_key, arrow.type, arrow.x, arrow.y);
    }

    if (!arrow.valid)
    {
        mNoArrowFrames++;
        DropChild();
        if (mNoArrowFrames > 1500)
        {
            return Fail("no_plan", "mission: no arrow for 1500 frames, brief \"%s\"", ctx.world->brief_key);
        }
        return Running; // the script is busy (talking, spawning): wait
    }
    mNoArrowFrames = 0;

    // What does the arrow ask for? Make a child action for it
    char key[kKeyLen];
    bool horn = false;
    if (pl.in_car && strstr(mBriefMemory, "horn") != NULL && arrow.type == 1)
    {
        horn = true; // "sound the horn": go to the spot and honk
        _snprintf(key, sizeof(key), "horn%s,%d,%d", ctx.world->brief_key, static_cast<int>(arrow.x), static_cast<int>(arrow.y));
    }
    else if (arrow.type == 1 && !pl.in_car && (arrow.x - pl.x) * (arrow.x - pl.x) + (arrow.y - pl.y) * (arrow.y - pl.y) > 40.0f * 40.0f && FindNearbyCar(ctx, &mWheelsId))
    {
        // a long way on foot: take a nearby parked car first
        _snprintf(key, sizeof(key), "wheels%d", mWheelsId);
    }
    else switch (arrow.type)
    {
        case 3: // a car: get in it, unless already; "destroy that" means shoot it
            if (pl.in_car && pl.car_id == arrow.car_idx)
            {
                DropChild();
                return Running;
            }
            if (strstr(mBriefMemory, "estroy") != NULL || strstr(mBriefMemory, "blow up") != NULL || strstr(mBriefMemory, "wreck") != NULL)
            {
                _snprintf(key, sizeof(key), "wreck%d", arrow.car_idx);
            }
            else
            {
                _snprintf(key, sizeof(key), "car%d", arrow.car_idx);
            }
            break;
        case 2: // a pedestrian
            _snprintf(key, sizeof(key), "ped%d", arrow.ped_id);
            break;
        default: // a place (also objects and phones): go there
            _snprintf(key, sizeof(key), "go%d,%d,%s", static_cast<int>(arrow.x), static_cast<int>(arrow.y), pl.in_car ? "c" : "f");
            break;
    }
    key[kKeyLen - 1] = 0;

    if (strcmp(key, mChildKey) != 0)
    {
        DropChild();
        strncpy(mChildKey, key, kKeyLen - 1);
        mChildKey[kKeyLen - 1] = 0;
        if (strncmp(key, "wheels", 6) == 0)
        {
            mChild = new EnterCarAction(mWheelsId);
        }
        else if (strncmp(key, "wreck", 5) == 0)
        {
            mChild = new DestroyCarAction(arrow.car_idx);
        }
        else if (arrow.type == 3)
        {
            mChild = new EnterCarAction(arrow.car_idx);
        }
        else if (arrow.type == 2)
        {
            // hunt it: out of the car, up to it, shoot with the weapon the mission gave
            mChild = new KillPedAction(arrow.ped_id, arrow.x, arrow.y);
        }
        else if (pl.in_car)
        {
            mChild = new DriveAction(arrow.x, arrow.y, 1.5f);
        }
        else
        {
            // an object (a pickup) has to be walked onto, a place needs only to be reached
            mChild = new WalkAction(arrow.x, arrow.y, false, arrow.type == 4 ? 0.35f : 0.5f);
        }
        BotDecision("f=%d mission: new step %s (%s)", ctx.frame, mChildKey, mChild->Name());
        if (ctx.map)
        {
            if (!mStepSnap)
            {
                mStepSnap = new Snapshot(ctx.map);
            }
            mStepSnap->Capture();
            ctx.action_snap = mStepSnap; // the step's own retries rewind to here, not to the phone
        }
        mChild->Start(ctx);
    }
    if (mChildDone)
    {
        // arrived: wait for the script to react, honking if it asked for that
        if (ctx.frame < mHornUntil)
        {
            ctx.input = ((ctx.frame / 10) % 2 == 0) ? input_bit::Special1 : 0;
        }
        return Running;
    }

    ctx.input = 0;
    const Status s = mChild->Step(ctx);
    if (s == Done)
    {
        mChildDone = true;
        BotDecision("f=%d mission: step %s done", ctx.frame, mChildKey);
        if (strncmp(mChildKey, "horn", 4) == 0)
        {
            mHornUntil = ctx.frame + 80;
            BotDecision("f=%d mission: honking", ctx.frame);
        }
    }
    else if (s == Failed)
    {
        BotDecision("f=%d mission: step %s failed (%s): %s", ctx.frame, mChildKey, mChild->fail_condition, mChild->fail_detail);
        if (!Restart(ctx, "a step failed"))
        {
            return Fail(mChild->fail_condition, "mission step %s: %s", mChildKey, mChild->fail_detail);
        }
    }
    return Running;
}
