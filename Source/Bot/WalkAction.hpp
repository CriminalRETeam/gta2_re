#pragma once
#include "WalkRoute.hpp"

#include "BotAction.hpp"

class WalkAction : public Action
{
  public:
    // Walk to (x, y) on foot. With follow_arrow the target is the HUD arrow when there is one.
    WalkAction(float x, float y, bool follow_arrow, float stop_dist);
    ~WalkAction();

    virtual const char* Name() const
    {
        return "walk";
    }
    virtual void Detail(char* buf, int size) const;
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    void SetTarget(float x, float y)
    {
        mTx = x;
        mTy = y;
    }

    // On a jump over a car, land on it and press the enter key: a car stood on is taken nearly every time
    void SetStealOnJump(bool steal)
    {
        mStealOnJump = steal;
    }

    s32 Retries() const
    {
        return mRetries;
    }

  private:
    bool Plan(BotContext& ctx, const char* why);

    WalkRoute mRoute; // our own copy of the planned path (the shared navigation object is re-planned by others)
    float mTx, mTy;
    bool mFollowArrow;
    float mStopDist;
    bool mPlanned;
    bool mTargetIsArrow;
    s32 mPathIdx;
    s32 mProgressFrame;
    s32 mRetries;
    s32 mWaitUntil;
    Snapshot* mCk[2];      // rolling checkpoints: a retry rewinds to the older one, not to the start of the walk
    s32 mCkFrame[2];
    s32 mCkNext;
    s32 mCkSlot;
    s32 mTrafficWait;
    s32 mYieldUntil;
    s32 mYields;
    s32 mJumpUntil;
    s32 mJumpStart;
    s32 mJumps;
    s32 mNudgeUntil;
    s32 mCarJumpUntil;
    s32 mCarJumpStart;
    s32 mCarJumpNext;
    bool mStealOnJump;
    s32 mStealTries; // steals tried in this walk: two and it stops trying
    s32 mNudges;
    float mNudgeX, mNudgeY;
    Calibrator mCalib;
    bool mCalibrating;
    Action* mExitCar; // when the walk starts in a car
};
