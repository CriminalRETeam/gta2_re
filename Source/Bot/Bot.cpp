#include "Bot.hpp"
#include "Hooks.hpp"
#include "HeapTracker.hpp"
#include "Snapshot.hpp"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "Game.hpp"
#include "GameSession.hpp"
#include "Player.hpp"
#include "Ped.hpp"
#include "char.hpp"
#include "sprite.hpp"
#include "debug.hpp"
#include "rng.hpp"
#include "input_bits.hpp"
#include "FpsCounter.hpp"
#include "InputRecorder_67F8B0.hpp"

// winmain.cpp globals without a header declaration
EXTERN_GLOBAL(bool, bDoFrontEnd_626B68);
EXTERN_GLOBAL(u32, gMatchStartTime_6F5A28);

static Bot* gBot = NULL;

Bot* Bot_Get()
{
    return gBot;
}

bool Bot_Init(const char* cmd_line)
{
    BotOptions opts;
    if (!BotOptions_Parse(cmd_line, &opts))
    {
        return false;
    }

    BotHeap_Init();
    BotLog_Open(opts.out_dir);
    BotLog("bot: profile=%s map=%s style=%s script=%s seed=%u out=%s budget=%d",
           opts.profile,
           opts.map,
           opts.style,
           opts.script,
           opts.seed,
           opts.out_dir,
           opts.budget_frames);

    gBot = new Bot(opts);
    if (!Hooks::Attach())
    {
        BotLog("bot: hooks failed, running without the bot");
        delete gBot;
        gBot = NULL;
        return false;
    }
    return true;
}

Bot::Bot(const BotOptions& opts)
    : mOpts(opts)
    , mProfile(Profile_Idle)
    , mInputBits(0)
    , mFrame(0)
    , mRunFrame(0)
    , mRunStartTick(0)
    , mSeenRunning(false)
    , mExitRequested(false)
    , mMap(NULL)
    , mRcState(RC_Warmup)
    , mRcCounter(0)
    , mRcSnapA(NULL)
    , mRcSnapB(NULL)
    , mRcHash0(0)
    , mRcHash1(0)
{
    if (_stricmp(opts.profile, "walk_test") == 0)
    {
        mProfile = Profile_WalkTest;
    }
    else if (_stricmp(opts.profile, "restore_check") == 0)
    {
        mProfile = Profile_RestoreCheck;
    }
    else if (_stricmp(opts.profile, "replay") == 0)
    {
        mProfile = Profile_Replay;
    }
    else if (_stricmp(opts.profile, "idle") != 0)
    {
        BotLog("bot: unknown profile '%s', running idle", opts.profile);
    }

    char path[MAX_PATH];
    BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_trace.txt");
    mTrace.Open(path);
    if (mOpts.hash_log)
    {
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_hash.txt");
        mHashLog.Open(path);
    }
}

Bot::~Bot()
{
    delete mRcSnapA;
    delete mRcSnapB;
    delete mMap;
}

void Bot::ApplyBootOverrides()
{
    char path[300];

    bSkip_frontend_67D53B = true;
    bDoFrontEnd_626B68 = false;
    bSkip_draw_67D4EA = !mOpts.draw;
    bSkip_audio_67D6BE = !mOpts.audio;
    bSkip_quit_confirm_67D4E8 = true;
    // The game writes test\replay.rep (header at init, the records at shutdown) only with this on
    bDo_release_replay_67D4EB = true;

    if (mProfile == Profile_Replay)
    {
        bPlay_replay_67D4F4 = true;
        bDo_exit_after_replay_67D6E4 = true;
    }

    _snprintf(path, sizeof(path), "data\\%s", mOpts.map);
    gGameSession_67E8E0.SetMapName_4C5870(path);
    _snprintf(path, sizeof(path), "data\\%s", mOpts.style);
    gGameSession_67E8E0.SetStyleName_4C5890(path);
    _snprintf(path, sizeof(path), "data\\%s", mOpts.script);
    gGameSession_67E8E0.SetScriptName_4C58B0(path);
    gGameSession_67E8E0.SetSaveFileName_4C58D0("");

    BotLog("bot: boot overrides applied (skip_frontend, skip_draw=%d, skip_audio=%d, replay on, %s)",
           bSkip_draw_67D4EA,
           bSkip_audio_67D6BE,
           gGameSession_67E8E0.GetMapName_4C5940());
}

bool Bot::GameRunning() const
{
    return gGame_0x40_67E008 != NULL && gGame_0x40_67E008->field_0_game_state == GameState::Running_1 && PlayerPed() != NULL;
}

Player* Bot::LocalPlayer() const
{
    return gGame_0x40_67E008 ? gGame_0x40_67E008->field_38_local_player : NULL;
}

Ped* Bot::PlayerPed() const
{
    Player* pPlayer = LocalPlayer();
    return pPlayer ? pPlayer->field_2C4_player_ped : NULL;
}

Sprite* Bot::PlayerSprite() const
{
    Ped* pPed = PlayerPed();
    if (!pPed || !pPed->field_168_game_object)
    {
        return NULL;
    }
    return pPed->field_168_game_object->field_80_sprite_ptr;
}

void Bot::LogPlayer(const char* tag) const
{
    Sprite* pSprite = PlayerSprite();
    Ped* pPed = PlayerPed();
    if (!pSprite || !pPed)
    {
        BotLog("%s: f=%d no player sprite", tag, mFrame);
        return;
    }
    BotLog("%s: f=%d rng=%d x=%.3f y=%.3f z=%.3f hp=%d incar=%d",
           tag,
           mFrame,
           gpRng_67AB34->get_cur_rng_41CFE0(),
           pSprite->field_14_xy.x.ToFloat(),
           pSprite->field_14_xy.y.ToFloat(),
           pSprite->field_1C_zpos.ToFloat(),
           pPed->field_216_health,
           pPed->field_16C_car != NULL);
}

void Bot::TraceFrame()
{
    Sprite* pSprite = PlayerSprite();
    Ped* pPed = PlayerPed();
    if (!pSprite || !pPed)
    {
        return;
    }
    mTrace.Printf("%d %d %d %d %d %d %u\n",
                  mFrame,
                  gpRng_67AB34->get_cur_rng_41CFE0(),
                  pSprite->field_14_xy.x.mValue,
                  pSprite->field_14_xy.y.mValue,
                  pSprite->field_1C_zpos.mValue,
                  pPed->field_216_health,
                  gInputRecorder_67F8B0.field_75340_rec_buf_idx);
    if (mOpts.hash_log && mMap)
    {
        mHashLog.Printf("%d %d %08x\n", mFrame, gpRng_67AB34->get_cur_rng_41CFE0(), mMap->Hash());
    }
}

void Bot::RequestExit(const char* why)
{
    if (mExitRequested)
    {
        return;
    }
    mExitRequested = true;
    mInputBits = 0;
    const u32 ms = GetTickCount() - mRunStartTick;
    BotLog("bot: %d frames in %u ms (%u frames/s)", mRunFrame, ms, ms ? mRunFrame * 1000 / ms : 0);
    BotLog("bot: exit requested at f=%d: %s (heap: %u live blocks, %u bytes; during play %u allocs/%u bytes, %u frees)",
           mFrame,
           why,
           HeapTracker::LiveCount(),
           HeapTracker::LiveBytes(),
           HeapTracker::PlayAllocs(),
           HeapTracker::PlayAllocBytes(),
           HeapTracker::PlayFrees());
    if (gGame_0x40_67E008)
    {
        gGame_0x40_67E008->ExitGameNoBonus_4B8C00(0, GameExitType::CloseGameImmediately_1);
    }
}

void Bot::EnsureMap()
{
    if (mMap)
    {
        return;
    }
    mMap = new MemoryMap();

    char path[MAX_PATH];
    BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_exclude.txt");
    mMap->LoadExclusionFile(path);

    // Ranges only known at run time: heap blocks that hold wall clock state
    mMap->AddExclusion(gFpsCounter_7055D4, sizeof(FpsCounter), "gFpsCounter_7055D4 block (timeGetTime)");
    mMap->AddExclusion(&gMatchStartTime_6F5A28, sizeof(gMatchStartTime_6F5A28), "gMatchStartTime_6F5A28 (frame pacing)");

    mMap->Build();
    BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_blocks.txt");
    mMap->WriteBlockIndex(path);
}

void Bot::WriteReport(const char* result, const char* detail)
{
    char text[2048];
    _snprintf(text,
              sizeof(text),
              "result %s\nprofile %s\nmap %s\nframe %d\nrun_frame %d\ndetail %s\n"
              "map_ranges %u\nmap_bytes %u\nmap_section_bytes %u\nmap_heap_bytes %u\nmap_heap_blocks %u\nmap_exclusions %u\n"
              "heap_live_blocks %u\nheap_live_bytes %u\nplay_allocs %u\nplay_alloc_bytes %u\nplay_frees %u\n",
              result,
              mOpts.profile,
              mOpts.map,
              mFrame,
              mRunFrame,
              detail,
              mMap ? mMap->Count() : 0,
              mMap ? mMap->TotalBytes() : 0,
              mMap ? mMap->SectionBytes() : 0,
              mMap ? mMap->HeapBytes() : 0,
              mMap ? mMap->HeapBlocks() : 0,
              mMap ? mMap->ExclusionCount() : 0,
              HeapTracker::LiveCount(),
              HeapTracker::LiveBytes(),
              HeapTracker::PlayAllocs(),
              HeapTracker::PlayAllocBytes(),
              HeapTracker::PlayFrees());
    text[sizeof(text) - 1] = 0;
    char path[MAX_PATH];
    BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_report.txt");
    BotFile_WriteAll(path, text, strlen(text));
    BotLog("bot: report %s: %s", result, detail);
}

void Bot::OnFrame()
{
    if (!mSeenRunning)
    {
        if (!GameRunning())
        {
            if (mFrame < 5)
            {
                BotLog("bot: f=%d game not running yet (game=%p state=%d)",
                       mFrame,
                       gGame_0x40_67E008,
                       gGame_0x40_67E008 ? gGame_0x40_67E008->field_0_game_state : -1);
            }
            mFrame++;
            return;
        }
        mSeenRunning = true;
        mRunStartTick = GetTickCount();
        HeapTracker::BeginPlay();
        BotLog("bot: game running from f=%d, replay state=%d input_enabled=%d, heap %u blocks %u bytes",
               mFrame,
               gInputRecorder_67F8B0.field_38_replay_state,
               gInputRecorder_67F8B0.field_75344_bInputEnabled,
               HeapTracker::LiveCount(),
               HeapTracker::LiveBytes());
        LogPlayer("start");
        if (mOpts.hash_log)
        {
            EnsureMap();
        }
    }

    if (!mExitRequested)
    {
        switch (mProfile)
        {
            case Profile_WalkTest:
                StepWalkTest();
                break;
            case Profile_RestoreCheck:
                StepRestoreCheck();
                break;
            case Profile_Replay:
                StepReplay();
                break;
            default:
                StepIdle();
                break;
        }

        if (!mExitRequested && mOpts.budget_frames > 0 && mRunFrame >= mOpts.budget_frames)
        {
            LogPlayer("budget");
            RequestExit("frame budget reached");
        }
    }

    TraceFrame();
    mFrame++;
    mRunFrame++;
}

void Bot::StepIdle()
{
    mInputBits = 0;
    if (mOpts.budget_frames <= 0 && mRunFrame >= 1000)
    {
        LogPlayer("end");
        RequestExit("idle run done");
    }
}

void Bot::StepWalkTest()
{
    if (mRunFrame < mOpts.walk_frames)
    {
        mInputBits = input_bit::Forward;
    }
    else
    {
        mInputBits = 0;
    }
    if (mRunFrame == mOpts.walk_frames)
    {
        LogPlayer("walked");
    }
    if (mRunFrame == mOpts.walk_frames + 30)
    {
        LogPlayer("end");
        RequestExit("walk test done");
    }
}

void Bot::StepReplay()
{
    mInputBits = 0;
    if (gInputRecorder_67F8B0.field_38_replay_state == Live_0 && mRunFrame > 0)
    {
        // Playback ended and the game did not exit by itself (bDo_exit_after_replay_67D6E4 unset?)
        LogPlayer("end");
        RequestExit("replay finished");
    }
}

namespace
{
struct DiffCtx
{
    BotTextFile* file;
    u32 runs;
    u32 bytes;
};
} // namespace

void Bot::DiffToFileCb(void* ctx, u8* addr, u32 size, const u8* old_bytes, const u8* new_bytes)
{
    DiffCtx* d = static_cast<DiffCtx*>(ctx);
    char old_hex[40];
    char new_hex[40];
    const u32 shown = size < 16 ? size : 16;
    for (u32 i = 0; i < shown; i++)
    {
        sprintf(old_hex + i * 2, "%02x", old_bytes[i]);
        sprintf(new_hex + i * 2, "%02x", new_bytes[i]);
    }
    d->file->Printf("%08x %u %s %s\n", reinterpret_cast<u32>(addr), size, old_hex, new_hex);
    d->runs++;
    d->bytes += size;
}

u32 Bot::WriteDiff(const Snapshot* pSnap, const char* name)
{
    char path[MAX_PATH];
    BotPath_Join(path, sizeof(path), mOpts.out_dir, name);
    BotTextFile file;
    file.Open(path);
    DiffCtx ctx;
    ctx.file = &file;
    ctx.runs = 0;
    ctx.bytes = 0;
    const u32 total_runs = pSnap->Diff(DiffToFileCb, &ctx, 20000);
    BotLog("bot: %s: %u differing runs (%u written, %u bytes)", name, total_runs, ctx.runs, ctx.bytes);
    return total_runs;
}

// Snapshot at W, run k frames, snapshot again, restore W, run k frames: the second run must end
// in the same memory as the first. A mismatch means something that matters is outside the
// snapshot or in the exclusion list; bot_diff.txt lists it for Scripts/bot/restore_check.py.
void Bot::StepRestoreCheck()
{
    char detail[256];
    mInputBits = 0;

    switch (mRcState)
    {
        case RC_Warmup:
            if (mRunFrame >= mOpts.warmup_frames)
            {
                EnsureMap();
                mRcSnapA = new Snapshot(mMap);
                mRcSnapB = new Snapshot(mMap);
                mRcSnapA->Capture();
                mRcHash0 = mRcSnapA->Hash();
                const u32 live_hash = mMap->Hash();
                BotLog("restore_check: snapshot A at f=%d rng=%d hash %08x (live %08x), %u bytes",
                       mFrame,
                       gpRng_67AB34->get_cur_rng_41CFE0(),
                       mRcHash0,
                       live_hash,
                       mMap->TotalBytes());
                LogPlayer("rc A");
                mRcState = RC_RunA;
                mRcCounter = 0;
            }
            break;

        case RC_RunA:
            if (mRcCounter >= mOpts.check_frames)
            {
                mRcHash1 = mMap->Hash();
                mRcSnapB->Capture();
                LogPlayer("rc end of run 1");
                BotLog("restore_check: after %d frames hash %08x, restoring A", mRcCounter, mRcHash1);
                const DWORD t0 = GetTickCount();
                mRcSnapA->Restore();
                const DWORD t1 = GetTickCount();
                const u32 back = mMap->Hash();
                BotLog("restore_check: restored in %lu ms, hash %08x (expected %08x)", t1 - t0, back, mRcHash0);
                LogPlayer("rc after restore");
                if (back != mRcHash0)
                {
                    WriteDiff(mRcSnapA, "bot_diff.txt");
                    _snprintf(detail, sizeof(detail), "restore did not reproduce the snapshot hash (%08x vs %08x)", back, mRcHash0);
                    WriteReport("FAIL", detail);
                    mRcState = RC_Done;
                    RequestExit("restore_check failed");
                    break;
                }
                if (mOpts.perturb)
                {
                    // Verifier sanity check: a one byte change after the restore has to show up
                    PlayerPed()->field_216_health -= 1;
                    BotLog("restore_check: perturbed the player's health, run 2 must diverge");
                }
                mRcState = RC_RunB;
                mRcCounter = 0;
            }
            break;

        case RC_RunB:
            if (mRcCounter >= mOpts.check_frames)
            {
                const u32 h2 = mMap->Hash();
                LogPlayer("rc end of run 2");
                if (h2 != mRcHash1)
                {
                    const u32 runs = WriteDiff(mRcSnapB, "bot_diff.txt");
                    _snprintf(detail,
                              sizeof(detail),
                              "run 2 diverged from run 1 (%08x vs %08x), %u differing runs in bot_diff.txt",
                              h2,
                              mRcHash1,
                              runs);
                    WriteReport("FAIL", detail);
                }
                else
                {
                    _snprintf(detail, sizeof(detail), "run 2 reproduced run 1 (%08x) over %d frames", h2, mOpts.check_frames);
                    WriteReport("PASS", detail);
                }
                mRcState = RC_Done;
                RequestExit("restore_check done");
            }
            break;

        case RC_Done:
            break;
    }

    if (mRcState == RC_RunA || mRcState == RC_RunB)
    {
        mRcCounter++;
    }
}
