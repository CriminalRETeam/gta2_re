#include "MissionActions.hpp"
#include "WalkAction.hpp"
#include "CarActions.hpp"
#include "NavCar.hpp"
#include "CollectActions.hpp"
#include "Goals.hpp"
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

static const float kFootMarkerStopDist = 2.0f; // tiles: where the car stops for a marker that wants the player on foot
static const float kFarOnFoot = 30.0f;        // tiles: further than this a step on foot goes by car when it can
static const float kMarkerReach = 0.25f;      // tiles: how near the middle of a marker the player walks (its hit box is small)
static const s32 kMarkerSettle = 60;         // frames on a marker on foot before it is walked onto again
static const s32 kFootMarkerWait = 30;         // frames the script gets to react to the car before the player gets out

MissionAction::MissionAction()
    : mMissionSnap(NULL)
    , mStepSnap(NULL)
    , mChild(NULL)
    , mChildDone(false)
    , mStarted(false)
    , mStartPassed(0)
    , mStart(0)
    , mProgX(0.0f)
    , mProgY(0.0f)
    , mLastChange(0)
    , mLastArrowType(-1)
    , mRetries(0)
    , mNoArrowFrames(0)
    , mHornUntil(0)
    , mDoneFrame(0)
    , mFootTried(false)
    , mTouchTries(0)
    , mTutorial(false)
    , mStartTokens(0)
    , mHornParked(false)
    , mLastCarId(0)
    , mRemedy(NULL)
    , mRemedyUsed(false)
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
    delete mRemedy;
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
    mProgX = ctx.world->player.x;
    mProgY = ctx.world->player.y;
    mStartPassed = ctx.world->missions_passed;
    mStartTokens = ctx.world->token_count;
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
    mProgX = ctx.world->player.x;
    mProgY = ctx.world->player.y;
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
        // the tutorial is not counted as a passed mission by the script: it ends by handing out a Special Token (one more on the map)
        const bool passed = ctx.world->missions_passed > mStartPassed || (mTutorial && ctx.world->token_count > mStartTokens);
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
    {
        // travelling counts as progress: a long drive through traffic changes neither brief nor arrow
        const float mx = pl.x - mProgX;
        const float my = pl.y - mProgY;
        if (mx * mx + my * my > 15.0f * 15.0f)
        {
            mProgX = pl.x;
            mProgY = pl.y;
            mLastChange = ctx.frame;
        }
    }
    if (ctx.frame - mLastChange > 5000)
    {
        return Fail("no_progress", "mission: nothing changed for 5000 frames (brief \"%s\", arrow type %d at %.0f,%.0f)", ctx.world->brief_key, arrow.type, arrow.x, arrow.y);
    }

    if (!arrow.valid)
    {
        mNoArrowFrames++;
        if (mChild && !mChildDone && strncmp(mChildKey, "horn", 4) == 0)
        {
            // the arrow went while we were still settling on the horn spot: finish the parking
            return RunChild(ctx);
        }
        if (mHornParked && pl.in_car)
        {
            // parked on the horn spot and the arrow went: the script is waiting for the horn, or for whoever it called
            ctx.input = ((ctx.frame / 12) % 2 == 0) ? input_bit::Special1 : 0;
            if (mNoArrowFrames > 6000)
            {
                return Fail("no_plan", "mission: no arrow for 6000 frames after the horn, brief \"%s\"", ctx.world->brief_key);
            }
            return Running;
        }
        DropChild();
        if (mNoArrowFrames > 1500)
        {
            return Fail("no_plan", "mission: no arrow for 1500 frames, brief \"%s\"", ctx.world->brief_key);
        }
        return Running; // the script is busy (talking, spawning): wait
    }
    mNoArrowFrames = 0;

    if (pl.in_car)
    {
        mLastCarId = pl.car_id;
    }
    if (mRemedy)
    {
        // moving the car we left out of the way (it blocks a doorway): then the failed step is tried again
        const Status rs = mRemedy->Step(ctx);
        if (rs != Running)
        {
            BotDecision("f=%d mission: car moved (%s)", ctx.frame, rs == Done ? "done" : mRemedy->fail_detail);
            delete mRemedy;
            mRemedy = NULL;
            DropChild();
        }
        return Running;
    }

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
        if (strncmp(key, "horn", 4) != 0)
        {
            mHornParked = false;
        }
        mRemedyUsed = false;
        mFootTried = false;
        mTouchTries = 0;
        DropChild();
        strncpy(mChildKey, key, kKeyLen - 1);
        mChildKey[kKeyLen - 1] = 0;
        if (strncmp(key, "wheels", 6) == 0)
        {
            mChild = new EnterCarAction(mWheelsId);
        }
        else if (strncmp(key, "wreck", 5) == 0)
        {
            mChild = new DestroyCarGoal(arrow.car_idx);
        }
        else if (arrow.type == 3)
        {
            mChild = new EnterCarAction(arrow.car_idx);
        }
        else if (arrow.type == 2)
        {
            // hunt it: out of the car, up to it, shoot with the weapon the mission gave
            SequenceGoal* hunt = new SequenceGoal("hunt");
            hunt->Add(new PrepareGoal(Weapons::VsPed, 35.0f, 900), true);
            hunt->Add(new KillPedAction(arrow.ped_id, arrow.x, arrow.y), false);
            mChild = hunt;
        }
        else if (pl.in_car)
        {
            // drive near, then park on the spot: a locate test is a small box round the arrow
            SequenceGoal* go = new SequenceGoal("drive_park");
            // (a tutorial's markers, like its phone, are for the player on foot: stop short of it, no parking on top)
            go->Add(new DriveAction(arrow.x, arrow.y, mTutorial ? kFootMarkerStopDist : 2.5f), false);
            if (!mTutorial)
            {
                go->Add(new ParkAction(arrow.x, arrow.y, 0.3f), true);
            }
            mChild = go;
        }
        else
        {
            // an object (a pickup) has to be walked onto, a place needs only to be reached. A long way is for the car: travel takes
            // one parked close by or stopped in traffic, drives, and walks the rest
            const float adx = arrow.x - pl.x;
            const float ady = arrow.y - pl.y;
            if (arrow.type != 4 && adx * adx + ady * ady > kFarOnFoot * kFarOnFoot)
            {
                mChild = new TravelGoal(arrow.x, arrow.y, kMarkerReach, false);
            }
            else
            {
                mChild = new WalkAction(arrow.x, arrow.y, false, arrow.type == 4 ? 0.35f : kMarkerReach);
            }
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
    return RunChild(ctx);
}

// Steps the current step's child, or waits (honking if the mission asked for that) once it is done
Action::Status MissionAction::RunChild(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    if (mChildDone)
    {
        // arrived: wait for the script to react, honking if it asked for that
        const size_t kl = strlen(mChildKey);
        if (!mFootTried && !mHornParked && kl > 2 && strncmp(mChildKey, "go", 2) == 0 && mChildKey[kl - 1] == 'c' && pl.in_car && ctx.frame - mDoneFrame > (mTutorial ? kFootMarkerWait : 400))
        {
            // nothing happened for a while at the marker with the car on it: some markers want the player himself. Out of the
            // car and onto it on foot, once
            mFootTried = true;
            // the key stays: once the player is on foot the key of the arrow changes (",f") and the step becomes a walk onto it
            delete mChild;
            mChild = new ExitCarAction();
            mChildDone = false;
            BotDecision("f=%d mission: nothing at the marker with the car by it: out, then on foot onto it", ctx.frame);
            ctx.action_start_frame = ctx.frame;
            mChild->Start(ctx);
            return Running;
        }
        // on foot at a marker and the script is not reacting: the marker (a phone, a pickup) wants the player exactly on it, to the
        // last few steps: walk onto the very spot, and if that is not it, off it and on again
        if (!pl.in_car && !mHornParked && mTouchTries < 8 && strncmp(mChildKey, "go", 2) == 0 && ctx.frame - mDoneFrame > kMarkerSettle)
        {
            mTouchTries++;
            const float off = (mTouchTries % 2 == 0) ? 2.0f : 0.0f;
            delete mChild;
            mChild = new WalkAction(ctx.world->arrow.x + off, ctx.world->arrow.y, false, 0.12f);
            mChildDone = false;
            BotDecision("f=%d mission: at the marker %.1f tiles off and nothing happens: %s (%d)", ctx.frame,
                        sqrtf((ctx.world->arrow.x - pl.x) * (ctx.world->arrow.x - pl.x) + (ctx.world->arrow.y - pl.y) * (ctx.world->arrow.y - pl.y)), off > 0.0f ? "stepping off it" : "onto its exact spot", mTouchTries);
            ctx.action_start_frame = ctx.frame;
            mChild->Start(ctx);
            return Running;
        }
        if (ctx.frame < mHornUntil)
        {
            ctx.input = ((ctx.frame / 12) % 2 == 0) ? input_bit::Special1 : 0;
            if (ctx.frame % 300 == 0)
            {
                BotDecision("f=%d mission: honking at (%.1f,%.1f); %d peds near, nearest %.1f tiles", ctx.frame, pl.x, pl.y, ctx.world->ped_count, ctx.world->ped_count ? ctx.world->peds[0].dist : -1.0f);
            }
        }
        return Running;
    }

    ctx.input = 0;
    const Status s = mChild->Step(ctx);
    if (s == Done)
    {
        mChildDone = true;
        mDoneFrame = ctx.frame;
        BotDecision("f=%d mission: step %s done", ctx.frame, mChildKey);
        if (strncmp(mChildKey, "horn", 4) == 0)
        {
            mHornParked = true;
            // honk until the script reacts (the arrow or the brief changes, which makes a new step)
            mHornUntil = ctx.frame + 3000;
            BotDecision("f=%d mission: honking", ctx.frame);
        }
    }
    else if (s == Failed)
    {
        BotDecision("f=%d mission: step %s failed (%s): %s", ctx.frame, mChildKey, mChild->fail_condition, mChild->fail_detail);
        if (!mRemedyUsed && mLastCarId != 0 && !pl.in_car)
        {
            // The car we left may be parked in the way (a doorway, an alley): move it before giving up on the step
            WorldModel::CarInfo c;
            float sx = 0, sy = 0;
            if (ctx.world->FindCar(mLastCarId, &c) && c.dist < 8.0f && !c.has_driver && ctx.road->FindClearSpot(c.x, c.y, 6, 16, &sx, &sy))
            {
                mRemedyUsed = true;
                SequenceGoal* fix = new SequenceGoal("move_car");
                fix->Add(new EnterCarAction(mLastCarId), false);
                fix->Add(new DriveAction(sx, sy, 2.5f), false);
                fix->Add(new ExitCarAction(), false);
                mRemedy = fix;
                BotDecision("f=%d mission: car #%d is %.1f tiles away, maybe in the way; moving it to (%.0f,%.0f)", ctx.frame, mLastCarId, c.dist, sx, sy);
                ctx.action_start_frame = ctx.frame;
                mRemedy->Start(ctx);
                DropChild();
                return Running;
            }
        }
        if (!Restart(ctx, "a step failed"))
        {
            return Fail(mChild->fail_condition, "mission step %s: %s", mChildKey, mChild->fail_detail);
        }
    }
    return Running;
}
