#pragma once

#include "BotAction.hpp"

// The generic mission loop (docs/BOT_DESIGN.md, "Missions"): the script tells the player what to do
// through the HUD arrow and the brief. Follow the arrow, whatever it points at, until the script
// ends the mission, and read the outcome from the script's passed counter.
class MissionAction : public Action
{
  public:
    // The tutorial ends by handing out a Special Token: that, not the passed counter, is how it is told from a failure
    void SetTutorial(bool tutorial)
    {
        mTutorial = tutorial;
    }
    MissionAction();
    ~MissionAction();

    virtual Action* Active() const
    {
        return mRemedy ? mRemedy : mChild;
    }

    virtual const char* Name() const
    {
        return "mission";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    enum
    {
        kKeyLen = 48
    };

    void DropChild();
    Status RunChild(BotContext& ctx);
    Status StepChild(BotContext& ctx, bool* child_done);
    bool Restart(BotContext& ctx, const char* why);

    Snapshot* mMissionSnap; // the world when the mission began: where a failed mission restarts
    Snapshot* mStepSnap;    // the world when the current step began: where a step's own retries go
    Action* mChild;
    char mChildKey[kKeyLen];
    bool mChildDone;       // the child finished: wait for the arrow to change
    bool mStarted;
    s32 mStartPassed;
    bool mTutorial;   // the starting phone's walk-through (not counted as a passed mission by the script)
    s32 mStartTokens; // secret tokens on the map when the mission began
    s32 mStart;
    float mProgX, mProgY;  // where the player was when the progress timer last restarted
    s32 mLastChange;       // frame of the last arrow / brief / progress change
    char mLastBrief[12];
    s32 mLastArrowType;
    s32 mRetries;
    s32 mNoArrowFrames;
    char mBriefMemory[400]; // the last brief text seen: the on-screen brief goes away after a few seconds
    s32 mHornUntil;
    s32 mDoneFrame;  // the frame the current step finished
    bool mFootTried; // the on-foot try at a marker has been made
    s32 mPhoneTries; // walks onto a phone beside the last marker that rings while the script waits
    s32 mTouchTries; // walks onto the exact spot of a marker that did nothing
    bool mHornParked; // parked on the spot a horn mission asked for
    s32 mLastCarId;   // the last car the player was in
    Action* mRemedy;  // moving that car out of the way
    bool mRemedyUsed;
    s32 mWheelsId;
    s32 mWheelsTried;
    s32 mWheelsSearchFrame;
    bool FindNearbyCar(BotContext& ctx, s32* out_id);
};
