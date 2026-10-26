#pragma once

#include "types.hpp"
#include "BotHeap.hpp"
#include "BotOptions.hpp"
#include "BotLog.hpp"

class MemoryMap;
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

    void StepIdle();
    void StepWalkTest();
    void StepRestoreCheck();
    void StepReplay();

    static void DiffToFileCb(void* ctx, u8* addr, u32 size, const u8* old_bytes, const u8* new_bytes);
    u32 WriteDiff(const Snapshot* pSnap, const char* name);

    BotOptions mOpts;
    Profile mProfile;
    u32 mInputBits;
    s32 mFrame;        // logic frames seen by the hook, never rewound
    s32 mRunFrame;     // frames since the game was first seen running, never rewound
    bool mSeenRunning;
    bool mExitRequested;

    MemoryMap* mMap;
    BotTextFile mTrace;
    BotTextFile mHashLog;

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
