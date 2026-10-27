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

// Walk onto a phone and see whether the script starts a mission
class PhoneAction : public Action
{
  public:
    PhoneAction(s32 object_id, float x, float y);
    ~PhoneAction();

    virtual const char* Name() const
    {
        return "phone";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    // After Failed: the script gave neither a mission nor a brief
    bool Refused() const
    {
        return mRefused;
    }

    // The brief text shown when the phone refused (empty if none)
    const char* RefusalBrief() const
    {
        return mBrief;
    }

  private:
    s32 mId;
    float mX, mY;
    WalkAction* mWalk;
    bool mOnIt;
    s32 mOnItFrame;
    bool mWasOnMission;
    bool mRefused;
    s32 mBriefFrame;
    char mBrief[400];
    char mBrief0[12];
};

// Watches a mission: logs the view now and then (the mission loop comes later)
class MissionWatchAction : public Action
{
  public:
    explicit MissionWatchAction(s32 frames);

    virtual const char* Name() const
    {
        return "mission_watch";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    s32 mFrames;
    s32 mStart;
};

// Kill one pedestrian, found by its id: get out of any car, walk up to it, shoot until it is dead
class KillPedAction : public Action
{
  public:
    KillPedAction(s32 ped_id, float x, float y);
    ~KillPedAction();

    virtual const char* Name() const
    {
        return "kill_ped";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    s32 mId;
    float mX, mY;       // last known position
    WalkAction* mWalk;
    s32 mWalkFrame;
    float mWalkTx, mWalkTy;
    s32 mStart;
    s32 mGoneFrames;
    Action* mExit;
};

// Destroy one car, found by its id: chase it on foot, keep clear of the blast, shoot until it is wrecked
class DestroyCarAction : public Action
{
  public:
    explicit DestroyCarAction(s32 car_id);
    ~DestroyCarAction();

    virtual const char* Name() const
    {
        return "destroy_car";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    s32 mId;
    WalkAction* mWalk;
    float mWalkTx, mWalkTy;
    s32 mStart;
    Action* mExit;
    float mLastX, mLastY;
    s32 mLastDamage;
    s32 mLastDamageFrame;
    s32 mLastAmmo;
};
