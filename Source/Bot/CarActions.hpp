#pragma once

#include "BotAction.hpp"

class WalkAction;

// Walk to a parked car and get in as the driver
class EnterCarAction : public Action
{
  public:
    // With hijack the car may have a driver (a car waiting in traffic): the enter control pulls them out
    explicit EnterCarAction(s32 car_id, bool hijack = false);
    ~EnterCarAction();

    virtual const char* Name() const
    {
        return "enter_car";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    s32 mCarId;
    bool mHijack;
    WalkAction* mWalk;
    bool mPressing;
    s32 mPressStart;
    s32 mStable;
};

class ExitCarAction : public Action
{
  public:
    ExitCarAction();

    virtual const char* Name() const
    {
        return "exit_car";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    s32 mStart;
};

// Drive the car the player is in to (x, y): the RouteFinder junction path, then straight at the
// target. Steers with a measured heading model, backs out of obstacles, and when it stays stuck
// restores the action start and tries another speed / waypoint radius.
class DriveAction : public Action
{
  public:
    DriveAction(float x, float y, float stop_dist);

    virtual const char* Name() const
    {
        return "drive";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    enum
    {
        kMaxWp = 258
    };

    bool Plan(BotContext& ctx, const char* why);
    void ChooseVariant(BotContext& ctx);

    float mTx, mTy, mStopDist;
    bool mCalibrating;
    Calibrator mCalib;
    bool mPlanned;
    float mWpX[kMaxWp];
    float mWpY[kMaxWp];
    s32 mWpCount;
    s32 mWpIdx;
    s32 mProgressFrame;
    s32 mRetries;
    float mSpeedCap;  // tiles per frame
    float mWpRadius;  // tiles
    bool mHavePrev;
    float mPrevX, mPrevY;
    float mSpeed;
    s32 mStillFrames;
    s32 mReverseUntil;
    float mReverseSteer;
    s32 mManoeuvres;
    float mMaxSpeedSeen;
    float mBestWpDist;
    s32 mBestWpFrame;
    s32 mLastWpIdx;
    s32 mTurnUntil;
    s32 mTurnEnd;
    bool mTurnReverse;
    s32 mBehindSince;
    s32 mFwdUntil;
    float mStartX, mStartY;
    float mPlanX, mPlanY; // where the route was planned from (the start of its first leg)
};

// Creeps the car the player is in onto (x, y) to within tol tiles and stops there: a mission
// "locate" test is a small box round the arrow (the script wants the car on the spot, not near it).
class ParkAction : public Action
{
  public:
    ParkAction(float x, float y, float tol);

    virtual const char* Name() const
    {
        return "park";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    float mTx, mTy, mTol;
    s32 mStart;
    bool mHavePrev;
    float mPrevX, mPrevY;
    float mSpeed;
    s32 mSettled;
};
