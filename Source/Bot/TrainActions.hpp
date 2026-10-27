#pragma once

#include "BotAction.hpp"
#include "WorldModel.hpp"

class WalkAction;

// Experiment: walk to a platform, wait for a train to stop beside it, try to board it, ride it and
// log what happens (does the enter control work on a train? does the player move with it?).
class TrainTestAction : public Action
{
  public:
    TrainTestAction(float x, float y);
    ~TrainTestAction();

    virtual const char* Name() const
    {
        return "train_test";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    float mX, mY;
    WalkAction* mWalk;
    bool mArrived;
    s32 mArrivedFrame;
    s32 mPressStart;
    float mPrevTx, mPrevTy;
    s32 mLastLog;
};

// Gets from the nearest station towards (x, y) by train: walk to a platform beside the track, wait
// for a train to stop there, board it the moment it stops (it waits only about 80 frames), ride, and
// get off at a station that is nearer the target, when the train stands still. The tracks are
// long loops across the map and the trains run about three times as fast as a walker.
class RideTrainGoal : public Action
{
  public:
    // force: ride even when walking would be as quick (for testing)
    RideTrainGoal(float dest_x, float dest_y, bool force = false);
    ~RideTrainGoal();

    virtual const char* Name() const
    {
        return "ride_train";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    // The cheapest way by train from (px, py) to (x, y): estimated frames, or -1 when there is no use
    // in it. Fills the boarding and the alighting station ids.
    static float Estimate(const WorldModel& w, float px, float py, float x, float y, s32* board_id, s32* alight_id, bool force = false);

  private:
    enum Stage
    {
        ToPlatform,
        Waiting,
        Riding,
        Leaving
    };
    const WorldModel::Station* FindStation(const WorldModel& w, s32 id) const;
    bool OnPlatform(const WorldModel& w, float x, float y, s32* station_id) const;

    float mDx, mDy;
    Stage mStage;
    WalkAction* mWalk;
    s32 mBoardId;
    s32 mAlightId;
    s32 mStageFrame;
    float mStartX, mStartY;
    float mPrevX, mPrevY;
    s32 mStillFrames;
    s32 mPressFrame;
    s32 mTrainId;
    float mTrainX, mTrainY;
    s32 mTrainStill; // frames the watched train car has not moved
    s32 mCabId;
    float mCabX, mCabY, mCabVx, mCabVy;
    float mSpotX, mSpotY; // where a carriage will stop: beside the platform, behind the cab
    bool mHaveSpot;
    float mMyX, mMyY;
    s32 mBoardings;
    bool mForce;
};
