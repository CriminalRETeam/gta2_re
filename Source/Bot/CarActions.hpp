#pragma once

#include "TurnAround.hpp"
#include "DriveSteer.hpp"
#include "LocalPlan.hpp"

#include "BotAction.hpp"

class WalkAction;
class Snapshot;

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
    WalkAction* mStand; // walking to the spot ahead of a moving car
    s32 mStandStart;    // the frame the intercept began (-1: not at it)
    s32 mStandRetarget;
    s32 mSampleFrame;   // the car's position now and a few frames ago give its velocity
    float mSampleX, mSampleY, mCarVx, mCarVy;
    s32 mDriveOffs;     // times the car pulled away while we were at its door
    bool mStandThere;   // at the spot, waiting for the car to stop
    bool mPressing;
    bool mHopped;   // the hop onto the car has been tried
    s32 mHopStart;  // -1 when not hopping
    s32 mHopJump;   // the frame the jump began (-1 before)
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
    ~DriveAction();

    virtual const char* Name() const
    {
        return "drive";
    }
    virtual void Detail(char* buf, int size) const;
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    // Chasing something that moves (run over a pedestrian): the goal is moved with Retarget, and a drive that gets
    // stuck fails instead of rewinding to a snapshot (that would undo what the chase has done)
    void SetChase()
    {
        mChase = true;
    }
    void Retarget(BotContext& ctx, float x, float y);
    // Stuck in a jam with a better car close ahead: fail with "jam_switch" and the car ("car=<id>") for the caller to take it
    void SetJamSwitch(bool on)
    {
        mJamSwitch = on;
    }

  private:
    enum
    {
        kMaxWp = 258
    };

    bool Plan(BotContext& ctx, const char* why);
    void ChooseVariant(BotContext& ctx);

    float mTx, mTy, mStopDist;
    float mRouteAheadX[24]; // the stretch of the global route the detailed route is planned along
    float mRouteAheadY[24];
    int mRouteAheadN;
    bool mLocalFallback; // the detailed route is the global route's own legs (the search found none)
    bool mCalibrating;
    Calibrator mCalib;
    bool mPlanned;
    float mWpX[kMaxWp];
    float mWpY[kMaxWp];
    s32 mWpCount;
    s32 mWpIdx;
    s32 mProgressFrame;
    bool mJamSwitch;
    s32 mJamStill;       // frames stood still behind a stopped car
    s32 mProgWpIdx;      // the waypoint mProgWpDist is for
    float mProgWpDist;   // the nearest the car has been to it
    s32 mRetries;
    float mSpeedCap;  // tiles per frame
    float mWpRadius;  // tiles
    bool mHavePrev;
    float mPrevX, mPrevY;
    float mSpeed;
    s32 mStillFrames;
    s32 mReverseUntil;
    float mReverseSteer;
    LocalPlanOut mTile;  // the tile route over the next 18 tiles, planned every few frames
    LocalPlanOut mLocal; // the detailed route: the tile route from the car on with the fine stage's way round what is in the way, every frame
    s32 mLocalNext;
    float mPrevSteer;
    SteerDebug mSteerDebug; // the rollouts of the last choice, for the debug draw
    OvertakeTrace mTrace; // what the last overtake look found, for the debug draw
    s32 mTraceFrame;
    s32 mOvertakeUntil;
    float mOvertakeX, mOvertakeY;
    s32 mManoeuvres;
    Snapshot* mCk[2];  // rolling checkpoints: a retry rewinds to the older one, not to the start of a long drive
    s32 mCkFrame[2];
    s32 mCkNext;
    s32 mCkSlot;
    bool mLeftStart;    // the car has been more than 5 tiles from where it started
    s32 mBlockedFrames; // frames spent stopped behind a car that is not moving
    s32 mBlockReplans;
    float mMaxSpeedSeen;
    float mBestWpDist;
    s32 mBestWpFrame;
    s32 mLastWpIdx;
    s32 mTurnUntil;
    s32 mTurnEnd;
    bool mTurnReverse;
    s32 mBehindSince;
    s32 mFwdUntil;
    s32 mShoveUntil;   // pushing a car out of the way with the throttle down
    s32 mShoves;
    s32 mHandbrakeUntil;
    s32 mHandbrakeNext;
    s32 mSwerveUntil;  // dodging a police car
    float mSwerveX, mSwerveY;
    float mStartX, mStartY;
    float mPlanX, mPlanY; // where the route was planned from (the start of its first leg)
    bool mChase;
    TurnState mTurn; // turning round: forward with the wheel over, backing up when the nose is blocked, and so on
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
