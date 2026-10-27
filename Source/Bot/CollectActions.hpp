#pragma once

#include "BotAction.hpp"
#include "WorldModel.hpp"

class WalkAction;

// Walk onto a world object (secret token, kill frenzy icon, pickup) until it is gone
class PickUpAction : public Action
{
  public:
    PickUpAction(s32 object_id, float x, float y);
    ~PickUpAction();

    virtual const char* Name() const
    {
        return "pick_up";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    s32 mId;
    float mX, mY;
    WalkAction* mWalk;
    bool mWiggle;
    s32 mWiggleStart;
};

// After touching a kill frenzy icon: kill pedestrians until the frenzy is over
class KillFrenzyAction : public Action
{
  public:
    KillFrenzyAction();
    ~KillFrenzyAction();

    virtual const char* Name() const
    {
        return "kill_frenzy";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    s32 Kills() const
    {
        return mKills;
    }

  private:
    s32 mStart;
    bool mSeenActive;
    s32 mTargetId;
    WalkAction* mWalk;
    s32 mWalkTargetFrame;
    s32 mKills;
    s32 mStartCash;
    s32 mRetries;
    s32 mStartPassed;
    s32 mStartFailed;
    s32 mRuleTarget;
    enum
    {
        kBlack = 8
    };
    s32 mBlackId[kBlack];
    s32 mBlackUntil[kBlack];
    s32 mBlackNext;
    s32 mTargetSince;
    s32 mLastProgress;
    s32 mProgressFrame;
    bool IsBlacklisted(s32 id, s32 frame) const;
    s32 mPickFrame;
    s32 mPickedId;
    bool mBarrier;       // a wall or fence between the player and the target: walk round
    s32 mBarrierCheck;
    WalkAction* mRepo;
    s32 mRepoUntil;
    s32 mRepoAngle;
    s32 mBackFrames;
    float mBackX, mBackY;
    WalkAction* mWander;
    s32 mWanderUntil;
    bool mCharging;
    s32 mChargeStart;
    s32 mCooldownUntil;
};
