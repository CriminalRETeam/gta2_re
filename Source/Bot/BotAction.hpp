#pragma once

#include "types.hpp"
#include "BotHeap.hpp"
#include "BotRng.hpp"

class WorldModel;
class NavFoot;
class NavCar;
class Snapshot;
class MemoryMap;
struct BotOptions;

// How a rotation maps to a world heading for one way of moving (on foot, in a car), measured at
// run time: heading = sign * rotation + offset, and the Right bit changes the rotation by turn_sign
struct Steering
{
    bool valid;
    float sign;
    float offset;
    float turn_sign;
};

float Bot_WrapDeg(float d);
float Bot_HeadingDeg(float dx, float dy); // 0 = towards -Y, clockwise

// No building / wall tile on the straight line between two points (sampled every half tile): a clear line of fire
bool Bot_LineClear(float x0, float y0, float x1, float y1);

// Shared by the actions. Owned by the Bot; nothing here points into game memory.
struct BotContext
{
    BotContext();

    WorldModel* world;
    NavFoot* nav;
    NavCar* road;
    BotRng* rng;
    Snapshot* action_snap; // the world at the start of the running action (or of the running step of a mission)
    MemoryMap* map;        // for actions that keep snapshots of their own
    const BotOptions* opts;
    s32 frame;             // logic frames seen, never rewound
    s32 action_start_frame;
    u32 input;             // control bits for the next frame
    Steering foot;
    Steering car;
    bool nav_dump;
    const char* out_dir;

    // Turns towards aim (world tile coordinates) and returns the heading error in degrees
    float Steer(const Steering& st, float rot, float px, float py, float aim_x, float aim_y, bool turn_only_when_far);
};

// One action = one state machine, Step() once per logic frame, setting ctx.input
class Action : public BotObject
{
  public:
    enum Status
    {
        Running,
        Done,
        Failed
    };

    Action();
    virtual ~Action()
    {
    }

    virtual const char* Name() const = 0;
    virtual void Start(BotContext& ctx) = 0;
    virtual Status Step(BotContext& ctx) = 0;

    // Valid when Step returned Failed
    char fail_condition[24]; // no_plan, no_progress, action_failed
    char fail_detail[240];

  protected:
    Status Fail(const char* condition, const char* fmt, ...);
};

// Measures a Steering by moving: forward, then a turn while moving forward (cars) or standing
// (feet), then forward again
class Calibrator
{
  public:
    Calibrator();
    void Start(bool in_car);
    // Running until the Steering is filled in
    Action::Status Step(BotContext& ctx, Steering* out, char* fail, u32 fail_size);

  private:
    enum State
    {
        Move1,
        Turn,
        Move2,
        Finished
    };
    bool mInCar;
    State mState;
    s32 mStateFrame;
    float mX, mY, mRot;
    float mH1, mRot1;
    float mTurnSign;
};
