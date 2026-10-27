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
        Profile_Collect,
        Profile_PhoneTest,
        Profile_Missions,
        Profile_Travel,
        Profile_KillGang,
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
    s32 mPrevHp;
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
