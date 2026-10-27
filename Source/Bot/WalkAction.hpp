#pragma once

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
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    void SetTarget(float x, float y)
    {
        mTx = x;
        mTy = y;
    }

    s32 Retries() const
    {
        return mRetries;
    }

  private:
    bool Plan(BotContext& ctx, const char* why);

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
    Calibrator mCalib;
    bool mCalibrating;
};
