#pragma once

#include "types.hpp"
#include "BotHeap.hpp"
#include "BotOptions.hpp"
#include "BotLog.hpp"
#include "BotRng.hpp"
#include "BotAction.hpp"

class MemoryMap;
class WorldModel;
class NavFoot;
class NavCar;
class Action;
class Snapshot;
class Player;
class Ped;
class Sprite;

namespace input_bit
{
enum
{
    ControlBits = 0xFFF // the 12 game controls, see input_bits.hpp
};
}

// The hook target. Phase 0: the drivers that prove the seams (walk_test, restore_check, replay)
// and the idle run. The planner, executors and world model come in later phases.
class Bot : public BotObject
{
  public:
    Bot(const BotOptions& opts);
    ~Bot();

    // From the ParseCommandLine_4DA320 detour, after the game read its registry debug settings
    void ApplyBootOverrides();

    // From the ExecuteGame_4DA780 detour, once per logic frame before the frame runs
    void OnFrame();

    // The control bits the input detour hands to the game at the next input read
    u32 InputBits() const
    {
        return mInputBits;
    }

    // Asked once per logic frame by the game loop: true to hold this frame back (paused, between steps, or the slow speed)
    bool HoldFrame();
    static void RoutePlannedStatic(const char* why, class NavCar* road, float sx, float sy, float tx, float ty);

    const BotOptions& Options() const
    {
        return mOpts;
    }

  private:
    enum Profile
    {
        Profile_Idle,
        Profile_WalkTest,
        Profile_RestoreCheck,
        Profile_Replay,
        Profile_WalkArrow,
        Profile_DriveTest,
        Profile_RocketTest,
        Profile_Collect,
        Profile_PhoneTest,
        Profile_Missions,
        Profile_Travel,
        Profile_KillGang,
        Profile_TrainTest,
    };

    enum RestoreCheckState
    {
        RC_Warmup,
        RC_RunA,
        RC_RunB,
        RC_Done,
    };

    bool GameRunning() const;
    Player* LocalPlayer() const;
    Ped* PlayerPed() const;
    Sprite* PlayerSprite() const;
    void LogPlayer(const char* tag) const;
    void TraceFrame();
    void RequestExit(const char* why);
    void EnsureMap();
    void WriteReport(const char* result, const char* detail);
    bool LoadSnapshotFile(const char* path);
    bool SaveSnapshotFile(const char* name);

    void StepIdle();
    void StepWalkTest();
    void StepRestoreCheck();
    void StepReplay();
    void StepActions();
    void BuildActions();
    void StartAction(int idx);
    void ClearActions();
    void CollectNext();
    bool SelfDefense();
    bool Evade();
    bool AvoidBlast(); // a burning car close by, or our own about to: get out of the car, run away from the blast
    bool ShootPoliceCrowd(); // a crowd of police on foot: face them, shoot, walk backwards away from them
    bool CopCrush();

    // The overlay (BotUi.cpp): the goal chain, the state and the last decisions drawn with ImGui into the game window, and
    // a WTF button that saves the state from several seconds ago with everything needed to run that part again
    static Bot* sInstance;
    void OnRoutePlanned(const char* why, NavCar* road, float sx, float sy, float tx, float ty);
    static void __stdcall UiCallback();
    void RegisterUi();
    void DrawUi();
    void UiFrame();   // per logic frame: the rolling snapshots and a pending WTF
    void SaveWtf();
    enum
    {
        kRing = 5
    };
    bool mUiOn;
    int mSpeed;          // 0 slow (one logic frame in four), 1 normal, 2 fast (as many as can be run)
    int mSlowTick;
    int mStepFrames;     // paused: logic frames still to run
    float mStepTiles;    // paused: tiles of travel still to run (0 none)
    float mStepX, mStepY;
    int mStepRun;
    bool mPaused;        // the game's logic is held: the world stands still, the picture and the window go on
    bool mDrawPath;      // draw the route, aim point and steering on the game view
    bool mPauseOnRoute;  // pause whenever a drive route has been planned, to look at its picture
    s32 mRouteCount;
    char mRouteLast[260];
    bool mWtfRequested;
    s32 mWtfCount;
    char mWtfLast[260];
    Snapshot* mRing[kRing];
    s32 mRingFrame[kRing];
    u32 mRingTick;
    s32 mRingNext;
    s32 mCrushLogFrame;
    s32 mEvadeUntil;
    s32 mEvades;
    float mEvadeTx, mEvadeTy;
    void OnCollectFailure(Action* pAction);
    void StuckReport(const char* condition, const char* detail);

    static void DiffToFileCb(void* ctx, u8* addr, u32 size, const u8* old_bytes, const u8* new_bytes);
    u32 WriteDiff(const Snapshot* pSnap, const char* name);

    BotOptions mOpts;
    BotRng mRng;
    Profile mProfile;
    u32 mInputBits;
    s32 mFrame;        // logic frames seen by the hook, never rewound
    s32 mRunFrame;     // frames since the game was first seen running, never rewound
    u32 mRunStartTick; // GetTickCount at the first running frame
    bool mSeenRunning;
    bool mExitRequested;

    MemoryMap* mMap;
    BotTextFile mTrace;
    BotTextFile mHashLog;

    WorldModel* mWorld;
    NavFoot* mNav;
    NavCar* mRoad;
    Snapshot* mActionSnap;
    BotContext mCtx;
    enum
    {
        kMaxActions = 8
    };
    Action* mActions[kMaxActions];
    int mActionCount;
    int mActionIdx;
    // collect profile
    enum
    {
        kMaxBanned = 64
    };
    s32 mCollectTokens0;
    bool mCollectInit;
    s32 mFrenzyDone;
    s32 mCollectFailures;
    s32 mCurrentEntity;
    s32 mCurrentKind;
    s32 mBanned[kMaxBanned];
    s32 mBannedCount;
    s32 mStartCash;
    s32 mLastHurtFrame;
    s32 mDefenseFrames;
    s32 mShootCrowdFrames;
    s32 mBlastFrames;
    s32 mPrevHp;
    s32 mPrevWanted;
    // The last 128 frames of the player, for finding out how a death or an arrest came about
    struct HistFrame
    {
        s32 hp;
        s32 wanted;
        s32 car_damage;
        bool in_car;
        bool busted;
    };
    HistFrame mHist[128];
    s32 mWantedSince;
    bool mPrevDead;
    s32 mDeaths;
    s32 mArrests;
    void LogDeath();
    s32 mDamageLogs;
    bool mMapDumped;
    bool mObjectsDumped;
    bool mScriptDumped;
    bool mZonesDumped;

    // restore_check
    RestoreCheckState mRcState;
    s32 mRcCounter;
    Snapshot* mRcSnapA;
    Snapshot* mRcSnapB;
    u32 mRcHash0;
    u32 mRcHash1;
};

// Entry point: parses the command line, and when -bot is present creates the bot heap, the log,
// the Bot and attaches the hooks. Returns false when the game should run untouched.
bool Bot_Init(const char* cmd_line);
Bot* Bot_Get();
