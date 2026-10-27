#pragma once

#include "BotAction.hpp"
#include "WorldModel.hpp"

class ExitCarAction;

// A car worth getting into: parked and unlocked, or waiting in traffic (hijacked); never a police
// car, a mission car or a wreck. path is the walking distance. With prefer_fast the model's top
// speed counts for a lot (a chase is won by speed).
struct CarPick
{
    s32 banned[8];
    s32 banned_count;
};
bool Cars_Pick(BotContext& ctx, const CarPick& ban, float max_dist, bool prefer_fast, s32* out_id, bool* out_hijack, float* out_path);

// Drives the car the player is in at another car and rams it: aims where the target will be,
// uses the hand brake to swing round, backs out when stuck. Done when the target is on fire,
// stopped next to us (ready to be shot) or gone; fails when our own car is about to burn.
class RamCarAction : public Action
{
  public:
    explicit RamCarAction(s32 target_car_id);

    virtual const char* Name() const
    {
        return "ram_car";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    bool LineClear(float x0, float y0, float x1, float y1) const;

    s32 mTarget;
    s32 mStart;
    bool mHavePrev;
    float mPrevTx, mPrevTy;
    float mVx, mVy;       // target velocity, tiles per frame, smoothed
    float mPrevX, mPrevY; // own position last frame
    float mSpeed;
    s32 mStill;
    s32 mReverseUntil;
    float mReverseSteer;
    s32 mPlanFrame;
    s32 mWpIdx;
    bool mHavePlan;
    s32 mHits;
    s32 mLastTargetDamage;
    s32 mLastDamageFrame;
    s32 mStoppedFrames;
    enum
    {
        kMaxWp = 64
    };
    float mWpX[kMaxWp];
    float mWpY[kMaxWp];
    s32 mWpCount;
};
