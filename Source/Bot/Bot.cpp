#include "Bot.hpp"
#include "BlastAvoid.hpp"
#include "Hud.hpp"
#include "ShootPoliceCrowd.hpp"
#include "BotDebugDraw.hpp"
#include "Hooks.hpp"
#include "HeapTracker.hpp"
#include "Snapshot.hpp"
#include "WorldModel.hpp"
#include "NavFoot.hpp"
#include "NavCar.hpp"
#include "BotAction.hpp"
#include "WalkAction.hpp"
#include "CarActions.hpp"
#include "CollectActions.hpp"
#include "MissionActions.hpp"
#include "Goals.hpp"
#include "TrainActions.hpp"
#include "Goap.hpp"
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "Game.hpp"
#include "GameSession.hpp"
#include "Player.hpp"
#include "Car.hpp"
#include "Ped.hpp"
#include "char.hpp"
#include "sprite.hpp"
#include "debug.hpp"
#include "rng.hpp"
#include "input_bits.hpp"
#include <dinput.h>
#include <stdlib.h>
#include "FpsCounter.hpp"
#include "InputRecorder_67F8B0.hpp"
#include "BotCompat.hpp"

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

static const s32 kBotTextSpeed = 1; // frames a character of a message stays up

Bot::Bot(const BotOptions& opts)
    : mOpts(opts)
    , mRng(opts.seed)
    , mProfile(Profile_Idle)
    , mInputBits(0)
    , mFrame(0)
    , mRunFrame(0)
    , mRunStartTick(0)
    , mSeenRunning(false)
    , mExitRequested(false)
    , mMap(NULL)
    , mWorld(NULL)
    , mNav(NULL)
    , mRoad(NULL)
    , mActionSnap(NULL)
    , mActionCount(0)
    , mActionIdx(-1)
    , mCollectTokens0(0)
    , mCollectInit(false)
    , mFrenzyDone(0)
    , mCollectFailures(0)
    , mCurrentEntity(0)
    , mCurrentKind(0)
    , mBannedCount(0)
    , mStartCash(0)
    , mLastHurtFrame(-1000)
    , mDefenseFrames(0)
    , mShootCrowdFrames(0)
    , mBlastFrames(0)
    , mCrushLogFrame(-1000)
    , mUiOn(false)
    , mPaused(false)
    , mDrawPath(true)
    , mPauseOnRoute(opts.pause_route)
    , mRouteCount(0)
    , mWtfRequested(false)
    , mWtfCount(0)
    , mRingTick(0)
    , mRingNext(0)
    , mEvadeUntil(0)
    , mEvades(0)
    , mEvadeTx(0.0f)
    , mEvadeTy(0.0f)
    , mPrevHp(100)
    , mPrevWanted(0)
    , mWantedSince(0)
    , mPrevDead(false)
    , mDeaths(0)
    , mArrests(0)
    , mDamageLogs(0)
    , mMapDumped(false)
    , mObjectsDumped(false)
    , mScriptDumped(false)
    , mZonesDumped(false)
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
    else if (_stricmp(opts.profile, "walk_arrow") == 0)
    {
        mProfile = Profile_WalkArrow;
    }
    else if (_stricmp(opts.profile, "phone_test") == 0)
    {
        mProfile = Profile_PhoneTest;
    }
    else if (_stricmp(opts.profile, "collect") == 0)
    {
        mProfile = Profile_Collect;
    }
    else if (_stricmp(opts.profile, "train_test") == 0)
    {
        mProfile = Profile_TrainTest;
    }
    else if (_stricmp(opts.profile, "kill_gang") == 0)
    {
        mProfile = Profile_KillGang;
    }
    else if (_stricmp(opts.profile, "travel") == 0)
    {
        mProfile = Profile_Travel;
    }
    else if (_stricmp(opts.profile, "missions") == 0)
    {
        mProfile = Profile_Missions;
    }
    else if (_stricmp(opts.profile, "drive_test") == 0)
    {
        mProfile = Profile_DriveTest;
    }
    else if (_stricmp(opts.profile, "rocket_test") == 0)
    {
        mProfile = Profile_RocketTest;
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
    delete mWorld;
    for (int i = 0; i < mActionCount; i++)
    {
        delete mActions[i];
    }
    delete mActionSnap;
    delete mNav;
    delete mRoad;
}

void Bot::ApplyBootOverrides()
{
    char path[300];

    bSkip_frontend_67D53B = true;
    bDoFrontEnd_626B68 = false;
    bSkip_draw_67D4EA = !mOpts.draw;
    bSkip_audio_67D6BE = !mOpts.audio;
    bSkip_quit_confirm_67D4E8 = true;
    if (mOpts.miss_log)
    {
        gDo_miss_logging_67D6BC = true; // test\\MISS_LOG.TXT
    }
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

bool Bot::LoadSnapshotFile(const char* path)
{
    EnsureMap();
    Snapshot* pSnap = new Snapshot(mMap);
    const bool ok = pSnap->LoadFromFile(path);
    if (ok)
    {
        pSnap->Restore();
        BotLog("bot: restored %s (%u bytes), rng=%d", path, mMap->TotalBytes(), gpRng_67AB34->get_cur_rng_41CFE0());
        LogPlayer("loaded");
    }
    delete pSnap;
    return ok;
}

bool Bot::SaveSnapshotFile(const char* name)
{
    EnsureMap();
    char path[MAX_PATH];
    BotPath_Join(path, sizeof(path), mOpts.out_dir, name);
    Snapshot* pSnap = new Snapshot(mMap);
    pSnap->Capture();
    const bool ok = pSnap->SaveToFile(path);
    BotLog("bot: %s %s at f=%d rng=%d (%u bytes, hash %08x)",
           ok ? "saved" : "could not save",
           path,
           mFrame,
           gpRng_67AB34->get_cur_rng_41CFE0(),
           mMap->TotalBytes(),
           pSnap->Hash());
    delete pSnap;
    return ok;
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

    const u32 count = BotDecision_Count();
    u32 first = count > 50 ? count - 50 : 0;
    if (count)
    {
        BotFile_AppendText(path, "decisions\n");
    }
    for (u32 i = first; i < count; i++)
    {
        char line[260];
        _snprintf(line, sizeof(line), "  %s\n", BotDecision_At(i));
        line[sizeof(line) - 1] = 0;
        BotFile_AppendText(path, line);
    }
    BotLog("bot: report %s: %s", result, detail);
}

EXTERN_GLOBAL(LPDIRECTINPUTDEVICEA, gMouseDevice_706C60);

static void LogTrains(s32 frame)
{
    for (int i = 0; i < 306; i++)
    {
        Car* c = &gCar_BC_Pool_67792C->field_0_pool.field_8_pool[i];
        if (!c->field_50_car_sprite || !c->field_6C_car_id || !c->IsTrainModel_403BA0())
        {
            continue;
        }
        BotLog("train f=%d car#%d model=%d x=%.1f y=%.1f vel=%.3f", frame, c->field_6C_car_id, c->field_84_car_info_idx, c->field_50_car_sprite->field_14_xy.x.mValue / 16384.0f,
               c->field_50_car_sprite->field_14_xy.y.mValue / 16384.0f, c->field_58_physics ? c->GetVelocity_43A4C0().mValue / 16384.0f : 0.0f);
    }
}
EXTERN_GLOBAL(u32, gMatchStartTime_6F5A28);

void Bot::OnFrame()
{
    if (mOpts.trains_log && mFrame % 60 == 0)
    {
        LogTrains(mFrame);
    }
    if (mOpts.draw)
    {
        if (!sInstance)
        {
            memset(mRing, 0, sizeof(mRing));
            memset(mRingFrame, 0, sizeof(mRingFrame));
            mWtfLast[0] = 0;
            mRouteLast[0] = 0;
            RegisterUi();
        }
        UiFrame();
        // Watching on the user's own screen: the game grabs the mouse exclusively and clips it to its
        // window. The bot plays by itself, so give the mouse back every frame.
        ClipCursor(NULL);
        if (mOpts.fast)
        {
            gMatchStartTime_6F5A28 = 0; // the next logic frame is always due: up to 8 per drawn frame
        }
        if (gMouseDevice_706C60)
        {
            gMouseDevice_706C60->Unacquire();
        }
    }
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
        if (mOpts.load_snap[0])
        {
            if (!LoadSnapshotFile(mOpts.load_snap))
            {
                RequestExit("-bot_load failed");
            }
        }
    }

    if (!mExitRequested && mOpts.save_at_frame >= 0 && mRunFrame == mOpts.save_at_frame)
    {
        SaveSnapshotFile("bot_save.snap");
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
            case Profile_WalkArrow:
            case Profile_DriveTest:
            case Profile_RocketTest:
            case Profile_Collect:
            case Profile_PhoneTest:
            case Profile_Missions:
            case Profile_Travel:
            case Profile_KillGang:
            case Profile_TrainTest:
                StepActions();
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


// ---------------------------------------------------------------------------------------------
// Profiles built from actions: walk_arrow, drive_test

void Bot::StuckReport(const char* condition, const char* detail)
{
    char view[3000];
    mWorld->frame = mFrame;
    mWorld->Observe();
    mWorld->Text(view, sizeof(view), mOpts.profile);

    char head[500];
    _snprintf(head, sizeof(head), "result FAIL\ncondition %s\ndetail %s\nprofile %s\nmap %s\nseed %u\nframe %d\naction %s\naction_start_frame %d\n--- view\n",
              condition, detail, mOpts.profile, mOpts.map, mOpts.seed, mFrame,
              (mActionIdx >= 0 && mActionIdx < mActionCount) ? mActions[mActionIdx]->Name() : "-", mCtx.action_start_frame);
    head[sizeof(head) - 1] = 0;

    char path[MAX_PATH];
    BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_report.txt");
    BotFile_WriteAll(path, head, strlen(head));
    BotFile_AppendText(path, view);
    {
        char threads[4000];
        WorldModel::ScriptThreads(threads, sizeof(threads));
        BotFile_AppendText(path, "--- script threads (what each is about to run)\n");
        BotFile_AppendText(path, threads);
        char peds[3500];
        WorldModel::PedTable(peds, sizeof(peds), 30.0f);
        BotFile_AppendText(path, "--- peds within 30 tiles\n");
        BotFile_AppendText(path, peds);
    }
    BotFile_AppendText(path, "--- last decisions\n");
    const u32 count = BotDecision_Count();
    for (u32 i = count > 50 ? count - 50 : 0; i < count; i++)
    {
        BotFile_AppendText(path, BotDecision_At(i));
        BotFile_AppendText(path, "\n");
    }
    if (mActionSnap)
    {
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_stuck.snap");
        const bool ok = mActionSnap->SaveToFile(path);
        BotLog("bot: stuck snapshot (state at action start, f=%d) %s: %s", mCtx.action_start_frame, ok ? "saved" : "NOT saved", path);
    }
    BotLog("bot: STUCK %s: %s", condition, detail);
    BotLog("%s", view);
    if (mOpts.script_dump)
    {
        // the mission script is loaded over the level script when a mission starts: dump what is there now
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_script_stuck.txt");
        WorldModel::DumpScript(path);
    }
}

void Bot::BuildActions()
{
    if (!mNav)
    {
        mNav = new NavFoot();
        mRoad = new NavCar();
        EnsureMap();
        mActionSnap = new Snapshot(mMap);
        mCtx.world = mWorld;
        mCtx.nav = mNav;
        mCtx.road = mRoad;
        mCtx.rng = &mRng;
        mCtx.action_snap = mActionSnap;
        mCtx.map = mMap;
        mCtx.opts = &mOpts;
        mCtx.nav_dump = mOpts.nav_dump;
        mCtx.out_dir = mOpts.out_dir;
    }

    if (mProfile == Profile_WalkArrow)
    {
        mActions[mActionCount++] = new WalkAction(mOpts.goto_x, mOpts.goto_y, true, 1.5f);
        return;
    }
    if (mProfile == Profile_Collect)
    {
        return; // CollectNext plans
    }
    if (mProfile == Profile_TrainTest)
    {
        mActions[mActionCount++] = new RideTrainGoal(mOpts.goto_x, mOpts.goto_y, true);
        return;
    }
    if (mProfile == Profile_KillGang)
    {
        SequenceGoal* seq = new SequenceGoal("kill_gang_test");
        seq->Add(new PrepareGoal(Weapons::VsPed, 40.0f, 1500), true);
        seq->Add(new KillGangGoal(mOpts.gang, mOpts.kills, 12000), false);
        mActions[mActionCount++] = seq;
        return;
    }
    if (mProfile == Profile_Travel)
    {
        mActions[mActionCount++] = new TravelGoal(mOpts.goto_x, mOpts.goto_y, 6.0f);
        return;
    }
    if (mProfile == Profile_Missions)
    {
        mActions[mActionCount++] = new MissionsGoal(mOpts.want_missions > 0 ? mOpts.want_missions : 3);
        return;
    }
    if (mProfile == Profile_PhoneTest && mOpts.skip_phone)
    {
        mActions[mActionCount++] = new MissionAction();
        return;
    }
    if (mProfile == Profile_PhoneTest)
    {
        // -bot_phone_id=N picks a phone, otherwise the nearest one
        const WorldModel::ObjInfo* phone = NULL;
        for (int i = 0; i < mWorld->phone_count; i++)
        {
            if (mOpts.phone_id == 0 || mWorld->phones[i].id == mOpts.phone_id)
            {
                phone = &mWorld->phones[i];
                break;
            }
        }
        if (!phone)
        {
            StuckReport("no_plan", "no such phone");
            RequestExit("no phone");
            return;
        }
        BotDecision("f=%d phone_test: phone #%d model %d at (%.0f,%.0f)", mFrame, phone->id, phone->model, phone->x, phone->y);
        mActions[mActionCount++] = new MissionGoal(*phone);
        return;
    }

    if (mProfile == Profile_RocketTest)
    {
        // rocket_test: a weapon (-bot_kills=ammo, -bot_gang=weapon type, 2 = rocket launcher) against the nearest parked car
        // that is 8 to 40 tiles away: what does it do to the car?
        const WorldModel::PlayerInfo& rp = mWorld->player;
        for (int i = 0; i < mWorld->car_count; i++)
        {
            const WorldModel::CarInfo& c = mWorld->cars[i];
            if (c.has_driver || c.police || c.mission || c.dist < 8.0f || c.dist > 90.0f || c.damage > 0)
            {
                continue;
            }
            BotDecision("f=%d rocket_test: car #%d model %d at (%.0f,%.0f), %.0f tiles from (%.0f,%.0f), proof bullets %d rockets %d flames %d", mFrame, c.idx, c.model, c.x, c.y, c.dist, rp.x, rp.y, c.bulletproof, c.rocketproof, c.flameproof);
            mActions[mActionCount++] = new DestroyCarAction(c.idx);
            return;
        }
        StuckReport("no_plan", "no parked car 8 to 40 tiles away");
        RequestExit("no car");
        return;
    }

    // drive_test: nearest parked, unlocked car on foot reach, drive it to -bot_goto, get out
    const WorldModel::PlayerInfo& pl = mWorld->player;
    s32 chosen = 0;
    for (int i = 0; i < mWorld->car_count; i++)
    {
        const WorldModel::CarInfo& c = mWorld->cars[i];
        if (c.has_driver || c.locked || c.police || c.mission || c.damage > 0 || c.dist > 150.0f)
        {
            continue;
        }
        if (mNav->FindPath(static_cast<u8>(pl.x), static_cast<u8>(pl.y), static_cast<u8>(pl.z + 0.5f), static_cast<u8>(c.x), static_cast<u8>(c.y)))
        {
            chosen = c.idx;
            BotDecision("f=%d drive_test: car #%d model %d at (%.0f,%.0f), %.0f tiles away, %u tiles on foot", mFrame, c.idx, c.model, c.x, c.y, c.dist, mNav->PathLength());
            break;
        }
    }
    if (!chosen)
    {
        StuckReport("no_plan", "no parked unlocked car within reach");
        RequestExit("no car");
        return;
    }
    mActions[mActionCount++] = new EnterCarAction(chosen);
    mActions[mActionCount++] = new DriveAction(mOpts.goto_x, mOpts.goto_y, 4.0f);
    mActions[mActionCount++] = new ExitCarAction();
}

void Bot::StartAction(int idx)
{
    mActionIdx = idx;
    mCtx.action_start_frame = mFrame;
    mActionSnap->Capture();
    mActions[idx]->Start(mCtx);
    BotDecision("f=%d start action %d/%d %s", mFrame, idx + 1, mActionCount, mActions[idx]->Name());
}

void Bot::StepActions()
{
    if (!mWorld)
    {
        mWorld = new WorldModel();
    }
    if (mProfile == Profile_RocketTest && mFrame == 60)
    {
        WorldModel::GiveWeapon(mOpts.gang > 0 ? mOpts.gang : 2, mOpts.kills > 0 ? mOpts.kills : 50);
        BotDecision("f=%d rocket_test: gave weapon %d ammo %d", mFrame, mOpts.gang > 0 ? mOpts.gang : 2, mOpts.kills > 0 ? mOpts.kills : 50);
    }
    mWorld->frame = mFrame;
    mWorld->Observe();
    mInputBits = 0;
    if (!mWorld->player.valid)
    {
        return;
    }
    mCtx.world = mWorld;
    mCtx.frame = mFrame;

    if (mActionIdx < 0)
    {
        BuildActions();
        if (mExitRequested)
        {
            return;
        }
        if (mProfile == Profile_Collect)
        {
            mCollectTokens0 = mWorld->token_count;
            mStartCash = mWorld->player.cash;
            BotDecision("f=%d collect: goal %d tokens (of %d in the world), %d kill frenzies", mFrame, mOpts.want_tokens, mCollectTokens0, mOpts.want_frenzies);
            CollectNext();
            return;
        }
        if (mActionCount == 0)
        {
            StuckReport("no_plan", "no actions");
            RequestExit("no plan");
            return;
        }
        StartAction(0);
    }

    if (mOpts.zones_dump && !mZonesDumped)
    {
        mZonesDumped = true;
        char path[MAX_PATH];
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_zones.txt");
        WorldModel::DumpZones(path);
    }
    if (mOpts.script_dump && !mScriptDumped)
    {
        mScriptDumped = true;
        char path[MAX_PATH];
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_script.txt");
        WorldModel::DumpScript(path);
    }
    if (mOpts.objects_dump && !mObjectsDumped)
    {
        mObjectsDumped = true;
        char path[MAX_PATH];
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_objects.txt");
        mWorld->DumpObjects(path);
    }
    if (mOpts.has_map_dump && !mMapDumped)
    {
        mMapDumped = true;
        char path[MAX_PATH];
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_map.txt");
        mWorld->DumpMap(path, mOpts.map_dump_x, mOpts.map_dump_y, 24);
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_slopes.txt");
        mWorld->DumpSlopes(path);
        BotPath_Join(path, sizeof(path), mOpts.out_dir, "bot_tiles.txt");
        mWorld->DumpTiles(path, static_cast<int>(mOpts.map_dump_x), static_cast<int>(mOpts.map_dump_y));
        if (mRoad && mOpts.has_goto)
        {
            // with -bot_goto too: the route the car router plans from the dumped point to the goto point
            const bool found = mRoad->FindPath(mOpts.map_dump_x, mOpts.map_dump_y, mOpts.goto_x, mOpts.goto_y);
            BotLog("car route (%.1f,%.1f)->(%.1f,%.1f): %s, %u waypoints, room %s", mOpts.map_dump_x, mOpts.map_dump_y, mOpts.goto_x, mOpts.goto_y, found ? "found" : "NONE", mRoad->Count(), mRoad->Clearance() ? "ok" : "squeezed");
            for (u32 wi = 0; wi < mRoad->Count(); wi++)
            {
                BotLog("route %u: %.1f %.1f", wi, mRoad->X(wi), mRoad->Y(wi));
            }
        }
        if (mRoad)
        {
            // the car router's whole view of the area: # closed, else a hex digit of the wall bits (1 left, 2 right, 4 top, 8 bottom)
            const int mcx = static_cast<int>(mOpts.map_dump_x);
            const int mcy = static_cast<int>(mOpts.map_dump_y);
            BotLog("road map x=%d..%d y=%d..%d (# closed, digit = wall bits 1 left 2 right 4 top 8 bottom):", mcx - 16, mcx + 16, mcy - 10, mcy + 10);
            for (int my = mcy - 10; my <= mcy + 10; my++)
            {
                char row[80];
                int n = 0;
                for (int mx = mcx - 16; mx <= mcx + 16 && n < 76; mx++)
                {
                    const u8 b = mRoad->WallBits(mx, my);
                    row[n++] = (b & 0x10) ? "0123456789abcdef"[b & 15] : '#';
                }
                row[n] = 0;
                BotLog("road %3d %s", my, row);
            }
        }
        if (mRoad)
        {
            // what the car router makes of the tiles: bit 0x10 open, 1 left, 2 right, 4 top, 8 bottom wall
            for (int ty = static_cast<int>(mOpts.map_dump_y) - 3; ty <= static_cast<int>(mOpts.map_dump_y) + 3; ty++)
            {
                char line[160];
                char* lp = line;
                lp += _snprintf(lp, sizeof(line), "road walls y=%d x=%d..:", ty, static_cast<int>(mOpts.map_dump_x) - 3);
                for (int tx = static_cast<int>(mOpts.map_dump_x) - 3; tx <= static_cast<int>(mOpts.map_dump_x) + 3; tx++)
                {
                    lp += _snprintf(lp, line + sizeof(line) - lp, " %02x", mRoad->WallBits(tx, ty));
                }
                BotDecision("%s", line);
            }
        }
    }

    // Damage log: who is around when health drops
    if (mWorld->player.hp < mPrevHp - 3 && mDamageLogs < 60)
    {
        mDamageLogs++;
        char line[400];
        char* p = line;
        p += _snprintf(p, sizeof(line), "f=%d hp %d -> %d, wanted %d at (%.1f,%.1f); near:", mFrame, mPrevHp, mWorld->player.hp, mWorld->player.wanted_stars, mWorld->player.x, mWorld->player.y);
        int k;
        for (k = 0; k < mWorld->ped_count && k < 3; k++)
        {
            const WorldModel::PedInfo& d = mWorld->peds[k];
            p += _snprintf(p, line + sizeof(line) - p, " ped#%d %s d=%.1f obj=%d", d.id, WorldModel_OccupationName(d.occupation), d.dist, d.objective);
        }
        for (k = 0; k < mWorld->car_count && k < 2; k++)
        {
            const WorldModel::CarInfo& d = mWorld->cars[k];
            p += _snprintf(p, line + sizeof(line) - p, " car#%d model %d d=%.1f dmg=%d drv=%d", d.idx, d.model, d.dist, d.damage, d.has_driver);
        }
        BotDecision("%s", line);
    }
    {
        HistFrame& h = mHist[mFrame & 127];
        h.hp = mWorld->player.hp;
        h.wanted = mWorld->player.wanted_stars;
        h.car_damage = mWorld->player.car_damage;
        h.in_car = mWorld->player.in_car;
        h.busted = mWorld->player.busted;
        if (mWorld->player.dead && !mPrevDead)
        {
            LogDeath();
        }
        mPrevDead = mWorld->player.dead;
        if (mWorld->player.wanted_stars > 0 && mPrevWanted == 0)
        {
            mWantedSince = mFrame;
        }
    }
    if (mWorld->player.wanted_stars != mPrevWanted)
    {
        // what the player was doing when the police took notice (or lost interest)
        const WorldModel::PlayerInfo& wp = mWorld->player;
        char line[400];
        char* p = line;
        p += _snprintf(p, sizeof(line), "f=%d wanted %d -> %d at (%.1f,%.1f) %s weapon %d ammo %d (fired %d) car damage %d, action %s; near:", mFrame, mPrevWanted, wp.wanted_stars, wp.x, wp.y, wp.in_car ? "in a car" : "on foot",
                       wp.weapon, wp.ammo, wp.shot_fired, wp.car_damage, mActions[mActionIdx] ? mActions[mActionIdx]->Name() : "-");
        int k;
        for (k = 0; k < mWorld->ped_count && k < 4; k++)
        {
            const WorldModel::PedInfo& d = mWorld->peds[k];
            p += _snprintf(p, line + sizeof(line) - p, " ped#%d %s%s d=%.1f", d.id, WorldModel_OccupationName(d.occupation), d.dead ? "(dead)" : "", d.dist);
        }
        for (k = 0; k < mWorld->car_count && k < 3; k++)
        {
            const WorldModel::CarInfo& d = mWorld->cars[k];
            p += _snprintf(p, line + sizeof(line) - p, " car#%d%s d=%.1f dmg=%d", d.idx, d.police ? "(police)" : "", d.dist, d.damage);
        }
        BotDecision("%s", line);
        mPrevWanted = wp.wanted_stars;
    }
    if (mWorld->player.hp < mPrevHp)
    {
        mLastHurtFrame = mFrame;
    }
    mPrevHp = mWorld->player.hp;
    // Messages (the tutorial's, the briefs) stay on screen for text_speed frames a character; the game's default of 3 makes a
    // tutorial take ten thousand frames. Set every frame: a snapshot restore puts the saved value back
    if (gHud_2B00_706620 && gHud_2B00_706620->field_13C4_text_speed > kBotTextSpeed)
    {
        gHud_2B00_706620->field_13C4_text_speed = kBotTextSpeed;
    }

    // Dead (wasted or busted): the game respawns the player after a countdown, nothing to do
    // meanwhile, except for actions that handle it themselves (a frenzy retries from a snapshot)
    if (mWorld->player.dead && mActions[mActionIdx] && strcmp(mActions[mActionIdx]->Name(), "kill_frenzy") != 0)
    {
        mInputBits = 0;
        return;
    }

    mCtx.input = 0;
    BotDebugDraw::Clear();
    Action* pAction = mActions[mActionIdx];
    const Action::Status status = pAction->Step(mCtx);
    mInputBits = mCtx.input & input_bit::ControlBits;
    const bool frenzy = strcmp(pAction->Name(), "kill_frenzy") == 0;
    bool shooting_crowd = false;
    const bool blast = AvoidBlast();
    if (blast)
    {
        mInputBits = mCtx.input & input_bit::ControlBits;
    }
    if (!frenzy && !blast)
    {
        shooting_crowd = ShootPoliceCrowd();
        if (!shooting_crowd)
        {
            SelfDefense(); // may replace the action's input, or add a weapon change to it
        }
        mInputBits = mCtx.input & input_bit::ControlBits;
    }
    if (!frenzy && !blast && !shooting_crowd && (CopCrush() || Evade()))
    {
        mInputBits = mCtx.input & input_bit::ControlBits;
    }
    if ((mFrame - mCtx.action_start_frame) % 5 == 0)
    {
        mTrace.Printf("act f=%d %s pos=%.2f,%.2f rot=%.0f in=%x incar=%d cdmg=%d hp=%d wanted=%d cops=%d weapon=%d/%d\n", mFrame, pAction->Name(), mWorld->player.x, mWorld->player.y, mWorld->player.rot_deg, mInputBits, mWorld->player.in_car, mWorld->player.car_damage, mWorld->player.hp, mWorld->player.wanted_stars, mWorld->cops_total, mWorld->player.weapon, mWorld->player.ammo);
    }

    if (status == Action::Failed && mProfile == Profile_Collect)
    {
        OnCollectFailure(pAction);
        return;
    }
    if (status == Action::Failed)
    {
        StuckReport(pAction->fail_condition, pAction->fail_detail);
        RequestExit("action failed");
        return;
    }
    if (status == Action::Done)
    {
        if (mActionIdx + 1 < mActionCount)
        {
            mInputBits = 0;
            StartAction(mActionIdx + 1);
            return;
        }
        if (mProfile == Profile_Collect)
        {
            if (mCurrentKind == 1)
            {
                mFrenzyDone++;
            }
            mInputBits = 0;
            CollectNext();
            return;
        }
        char detail[200];
        _snprintf(detail, sizeof(detail), "%d action(s) done in %d frames", mActionCount, mFrame);
        WriteReport("PASS", detail);
        LogPlayer("done");
        RequestExit("all actions done");
        mActionIdx = mActionCount; // keep the exit state
        mActionIdx = mActionCount - 1;
    }
}

// ---------------------------------------------------------------------------------------------
// collect: tokens and kill frenzies, planned with GOAP and replanned after every action

void Bot::ClearActions()
{
    for (int i = 0; i < mActionCount; i++)
    {
        delete mActions[i];
        mActions[i] = NULL;
    }
    mActionCount = 0;
    mActionIdx = -1;
}

void Bot::OnCollectFailure(Action* pAction)
{
    mCollectFailures++;
    BotDecision("f=%d collect: %s failed (%s): %s; entity %d banned", mFrame, pAction->Name(), pAction->fail_condition, pAction->fail_detail, mCurrentEntity);
    if (mBannedCount < kMaxBanned)
    {
        mBanned[mBannedCount++] = mCurrentEntity;
    }
    mInputBits = 0;
    if (mCollectFailures > 8)
    {
        StuckReport(pAction->fail_condition, pAction->fail_detail);
        RequestExit("too many failures");
        return;
    }
    CollectNext();
}

void Bot::CollectNext()
{
    ClearActions();
    const WorldModel::PlayerInfo& pl = mWorld->player;
    const s32 tokens_got = mCollectTokens0 - mWorld->token_count;
    s32 need_t = mOpts.want_tokens - tokens_got;
    s32 need_f = mOpts.want_frenzies - mFrenzyDone;
    if (need_t < 0)
    {
        need_t = 0;
    }
    if (need_f < 0)
    {
        need_f = 0;
    }
    if (need_t == 0 && need_f == 0)
    {
        char detail[240];
        _snprintf(detail, sizeof(detail), "collected %d secret tokens and %d kill frenzies in %d frames; cash %d -> %d; %d failures", tokens_got, mFrenzyDone, mFrame, mStartCash, pl.cash, mCollectFailures);
        WriteReport("PASS", detail);
        LogPlayer("collected");
        RequestExit("collect goal reached");
        return;
    }

    // Candidate entities: the nearest few of each kind that are not banned
    Goap::Entity ents[Goap::kMaxEntities];
    const WorldModel::ObjInfo* objs[Goap::kMaxEntities];
    s32 count = 0;
    s32 listed_t = 0;
    s32 listed_f = 0;
    int i;
    for (i = 0; i < mWorld->tokens_listed && count < 8; i++)
    {
        bool banned = false;
        for (int b = 0; b < mBannedCount; b++)
        {
            banned = banned || mBanned[b] == mWorld->tokens[i].id;
        }
        // by the walking route (not the straight line): a token across a river or on a roof is not near; here tokens are the goal, so
        // a long walk is fine
        if (banned || !Goals_PickupReachable(mCtx.nav, *mWorld, mWorld->tokens[i], true))
        {
            continue;
        }
        ents[count].id = mWorld->tokens[i].id;
        ents[count].kind = 0;
        ents[count].x = mWorld->tokens[i].x;
        ents[count].y = mWorld->tokens[i].y;
        ents[count].cost = 20.0f;
        objs[count] = &mWorld->tokens[i];
        count++;
        listed_t++;
    }
    for (i = 0; i < mWorld->frenzy_count && listed_f < 4; i++)
    {
        bool banned = false;
        for (int b = 0; b < mBannedCount; b++)
        {
            banned = banned || mBanned[b] == mWorld->frenzies[i].id;
        }
        bool allowed = mOpts.only_frenzy_count == 0;
        for (int a = 0; a < mOpts.only_frenzy_count; a++)
        {
            allowed = allowed || mWorld->frenzies[i].id == mOpts.only_frenzy_ids[a];
        }
        if (banned || !allowed)
        {
            continue;
        }
        ents[count].id = mWorld->frenzies[i].id;
        ents[count].kind = 1;
        ents[count].x = mWorld->frenzies[i].x;
        ents[count].y = mWorld->frenzies[i].y;
        ents[count].cost = 1500.0f;
        objs[count] = &mWorld->frenzies[i];
        count++;
        listed_f++;
    }
    Goap::Goal goal;
    memset(&goal, 0, sizeof(goal));
    goal.need[0] = need_t < listed_t ? need_t : listed_t;
    goal.need[1] = need_f < listed_f ? need_f : listed_f;
    if (goal.need[0] == 0 && goal.need[1] == 0)
    {
        StuckReport("no_plan", "no reachable candidates left for the goal");
        RequestExit("no plan");
        return;
    }
    Goap::Plan plan;
    Goap::MakePlan(ents, count, goal, pl.x, pl.y, 22.0f, mRng.Next(), &plan);
    if (!plan.found || plan.length == 0)
    {
        StuckReport("no_plan", "the planner found no plan for the goal");
        RequestExit("no plan");
        return;
    }
    char order[200];
    order[0] = 0;
    for (i = 0; i < plan.length && i < 8; i++)
    {
        char one[32];
        _snprintf(one, sizeof(one), " %s#%d", ents[plan.order[i]].kind == 0 ? "token" : "frenzy", ents[plan.order[i]].id);
        strncat(order, one, sizeof(order) - strlen(order) - 1);
    }
    BotDecision("f=%d collect plan (need %d tokens, %d frenzies; %d candidates, %u states): %s, cost %.0f", mFrame, need_t, need_f, count, plan.expanded, order, plan.total_cost);

    // Replan after each action: only the first step of the plan is started
    const Goap::Entity& first = ents[plan.order[0]];
    mCurrentEntity = first.id;
    mCurrentKind = first.kind;
    mActions[mActionCount++] = new PickUpAction(first.id, first.x, first.y);
    if (first.kind == 1)
    {
        mActions[mActionCount++] = new KillFrenzyAction();
    }
    StartAction(0);
}

// A reflex under every action: when something is hurting the player, turn on the nearest
// pedestrian that is close and fight until it stops. Not for kill frenzies (they fight by
// themselves) and not with explosive weapons (they would hurt the player more).
static bool Blast_Solid(void* /*user*/, float x, float y)
{
    return WorldModel::BuildingAt(x, y);
}

// Under every action, kill frenzies too: stay out of the reach of any car about to blow (BlastAvoid.hpp). Shooting a
// car until it is nearly gone and then stepping out is fine; this only moves the player once it is inside the stand-off.
bool Bot::AvoidBlast()
{
    const WorldModel::PlayerInfo& pl = mWorld->player;
    if (pl.dead || !mCtx.foot.valid)
    {
        return false;
    }
    if (pl.in_car)
    {
        // our own car is about to go: out of it, the next frames run
        if (BlastAvoid_IsHazard(pl.car_damage))
        {
            if (mBlastFrames == 0)
            {
                BotDecision("f=%d blast: our car has %d damage and is about to blow: getting out", mFrame, pl.car_damage);
            }
            mBlastFrames++;
            mCtx.input = (mFrame % 6 < 3) ? input_bit::EnterExit : 0;
            return true;
        }
        mBlastFrames = 0;
        return false;
    }
    BlastHazard hazards[WorldModel::kMaxCars];
    int count = 0;
    for (int i = 0; i < mWorld->car_count; i++)
    {
        const WorldModel::CarInfo& c = mWorld->cars[i];
        if (BlastAvoid_IsHazard(c.damage) && c.dist < 12.0f)
        {
            hazards[count].x = c.x;
            hazards[count].y = c.y;
            hazards[count].damage = c.damage;
            count++;
        }
    }
    BlastWorld world = {pl.x, pl.y, hazards, count, Blast_Solid, NULL};
    const BlastOut out = BlastAvoid_Decide(world);
    if (!out.active)
    {
        mBlastFrames = 0;
        return false;
    }
    if (mBlastFrames == 0)
    {
        BotDecision("f=%d blast: %d car(s) about to blow within %.1f tiles: stepping out to (%.1f,%.1f)", mFrame, count, kBlastStandOff, out.aim_x, out.aim_y);
    }
    mBlastFrames++;
    mCtx.input = 0;
    mCtx.Steer(mCtx.foot, pl.rot_deg, pl.x, pl.y, out.aim_x, out.aim_y, true);
    mCtx.input |= input_bit::Forward;
    return true;
}

static bool Crowd_Solid(void* /*user*/, float x, float y)
{
    return WorldModel::BuildingAt(x, y);
}

bool Bot::ShootPoliceCrowd()
{
    const WorldModel::PlayerInfo& pl = mWorld->player;
    const bool explosive_held = pl.weapon == 2 || pl.weapon == 4 || pl.weapon == 5 || pl.weapon == 8;
    if (pl.in_car || pl.dead || pl.wanted_stars == 0 || pl.ammo <= 0 || explosive_held || !mCtx.foot.valid)
    {
        return false;
    }
    CrowdCop cops[WorldModel::kMaxPeds];
    int count = 0;
    for (int i = 0; i < mWorld->ped_count; i++)
    {
        const WorldModel::PedInfo& p = mWorld->peds[i];
        if (p.cop && !p.dead && !p.in_car && p.dist < 14.0f)
        {
            cops[count].x = p.x;
            cops[count].y = p.y;
            count++;
        }
    }
    const float heading = (mCtx.foot.sign * pl.rot_deg + mCtx.foot.offset) * 3.14159265f / 180.0f;
    CrowdWorld world = {pl.x, pl.y, sinf(heading), -cosf(heading), cops, count, Crowd_Solid, NULL};
    const CrowdOut out = ShootPoliceCrowd_Decide(world);
    if (!out.active)
    {
        mShootCrowdFrames = 0;
        return false;
    }
    if (mShootCrowdFrames == 0)
    {
        BotDecision("f=%d police crowd: %d police on foot close by: facing them, shooting and walking %s", mFrame, count, out.move < 0 ? "backwards" : (out.move > 0 ? "away" : "nowhere (no room)"));
    }
    mShootCrowdFrames++;
    mCtx.input = 0;
    const float err = mCtx.Steer(mCtx.foot, pl.rot_deg, pl.x, pl.y, out.target_x, out.target_y, true);
    if (fabsf(err) < 35.0f && (mFrame % 8) < 6)
    {
        mCtx.input |= input_bit::Attack;
    }
    if (out.move < 0)
    {
        mCtx.input |= input_bit::Backward;
    }
    else if (out.move > 0)
    {
        mCtx.input |= input_bit::Forward;
    }
    return true;
}

bool Bot::SelfDefense()
{
    const WorldModel::PlayerInfo& pl = mWorld->player;
    if (pl.in_car || pl.dead || mFrame - mLastHurtFrame > 50)
    {
        return false;
    }
    const bool explosive_held = pl.weapon == 2 || pl.weapon == 4 || pl.weapon == 5 || pl.weapon == 8;
    const WorldModel::PedInfo* target = NULL;
    for (int i = 0; i < mWorld->ped_count; i++)
    {
        const WorldModel::PedInfo& p = mWorld->peds[i];
        // a cop that is beating the player is fought like anyone else (punched or shot); the nearest one first
        if (!p.dead && !p.in_car && p.dist < 3.6f && (!p.cop || pl.wanted_stars > 0) && (!target || p.dist < target->dist))
        {
            target = &p;
        }
    }
    // With the police on top of the player, they are the ones to fight (not the bystanders next to them)
    for (int i = 0; i < mWorld->ped_count && pl.wanted_stars > 0; i++)
    {
        const WorldModel::PedInfo& p = mWorld->peds[i];
        if (p.cop && !p.dead && !p.in_car && p.dist < 3.6f && (!target || !target->cop || p.dist < target->dist))
        {
            target = &p;
        }
    }
    if (!target)
    {
        return false;
    }
    if (mDefenseFrames == 0)
    {
        BotDecision("f=%d self defence: hp %d, fighting ped #%d (%s) at %.1f with weapon %d ammo %d", mFrame, pl.hp, target->id, WorldModel_OccupationName(target->occupation), target->dist, pl.weapon, pl.ammo);
    }
    mDefenseFrames++;
    if (explosive_held)
    {
        // a rocket, molotov, grenade or flamethrower at arm's length hurts the player more. (Changing weapon here, for the
        // police or for everyone, was tried: it made the player shoot bystanders, the wanted level rose and it died more.)
        return false;
    }
    mCtx.input = 0;
    const float err = mCtx.Steer(mCtx.foot, pl.rot_deg, pl.x, pl.y, target->x, target->y, true);
    if (fabsf(err) < 35.0f && (mFrame % 8) < 6)
    {
        mCtx.input |= input_bit::Attack;
    }
    return true;
}

// A reflex over every action: a car coming straight at the player on foot is dodged, sideways and
// with a jump when it is close, whatever the action was doing
bool Bot::Evade()
{
    if (mCtx.no_evade_frame >= mFrame - 1)
    {
        return false; // standing in the lane on purpose to stop a car
    }
    const WorldModel::PlayerInfo& pl = mWorld->player;
    if (pl.in_car || pl.dead)
    {
        return false;
    }
    const WorldModel::CarInfo* threat = NULL;
    float side = 0.0f;
    float along_t = 0.0f;
    for (int i = 0; i < mWorld->car_count; i++)
    {
        const WorldModel::CarInfo& c = mWorld->cars[i];
        const float v = sqrtf(c.vx * c.vx + c.vy * c.vy);
        if (v < 0.12f || c.dist > 6.0f)
        {
            continue;
        }
        const float ux = c.vx / v;
        const float uy = c.vy / v;
        const float rx = pl.x - c.x;
        const float ry = pl.y - c.y;
        const float along = rx * ux + ry * uy;      // ahead of the car
        const float across = rx * -uy + ry * ux;    // to its left (+) or right (-)
        // reaches us within about 25 frames, and we stand in its path (a car is about 1.3 tiles wide)
        if (along > 0.0f && along < 1.5f + v * 10.0f && fabsf(across) < 1.0f && (!threat || along < along_t))
        {
            threat = &c;
            along_t = along;
            side = across >= 0.0f ? 1.0f : -1.0f;
        }
    }
    if (threat && mFrame >= mEvadeUntil)
    {
        const float v = sqrtf(threat->vx * threat->vx + threat->vy * threat->vy);
        // sideways = perpendicular to the car's path, to the side we already stand on
        mEvadeTx = pl.x + (-threat->vy / v) * 3.0f * side;
        mEvadeTy = pl.y + (threat->vx / v) * 3.0f * side;
        mEvadeUntil = mFrame + 14;
        mEvades++;
        BotDecision("f=%d evade: car #%d (model %d) %.1f tiles away heading at us (speed %.2f), jumping aside (%d)", mFrame, threat->idx, threat->model, threat->dist, v, mEvades);
    }
    else if (mFrame >= mEvadeUntil)
    {
        return false;
    }
    const float tx = mEvadeTx;
    const float ty = mEvadeTy;
    mCtx.input = 0;
    const float err = mCtx.Steer(mCtx.foot, pl.rot_deg, pl.x, pl.y, tx, ty, true);
    if (fabsf(err) < 60.0f)
    {
        mCtx.input |= input_bit::Forward;
    }
    if (mEvadeUntil - mFrame > 10)
    {
        mCtx.input |= input_bit::HandbrakeJump;
    }
    return true;
}

// A reflex over every action: in a car with the police on foot beside it (they are there to pull the player out),
// run them down. In front: drive at them. Behind: back into them. Beside: reverse with the handbrake on to swing
// the tail round onto them.
bool Bot::CopCrush()
{
    const WorldModel::PlayerInfo& pl = mWorld->player;
    if (!pl.in_car || pl.dead || pl.wanted_stars == 0)
    {
        return false;
    }
    const WorldModel::PedInfo* cop = NULL;
    for (int i = 0; i < mWorld->ped_count; i++)
    {
        const WorldModel::PedInfo& p = mWorld->peds[i];
        if (p.cop && !p.dead && !p.in_car && p.dist < 3.2f && (!cop || p.dist < cop->dist))
        {
            cop = &p;
        }
    }
    if (!cop)
    {
        return false;
    }
    mCtx.input = 0;
    const float err = mCtx.Steer(mCtx.car, pl.rot_deg, pl.x, pl.y, cop->x, cop->y, false);
    const float a = fabsf(err);
    const char* how;
    if (a < 60.0f)
    {
        mCtx.input |= input_bit::Forward;
        how = "driving at";
    }
    else if (a > 120.0f)
    {
        mCtx.input |= input_bit::Backward;
        how = "backing into";
    }
    else
    {
        mCtx.input |= input_bit::Backward | input_bit::HandbrakeJump;
        how = "swinging the tail onto";
    }
    if (mFrame - mCrushLogFrame > 60)
    {
        mCrushLogFrame = mFrame;
        BotDecision("f=%d crush: cop #%d on foot %.1f tiles away (%.0f deg): %s him", mFrame, cop->id, cop->dist, err, how);
    }
    return true;
}

// Called the frame the player becomes wasted or busted: what led to it
void Bot::LogDeath()
{
    const WorldModel::PlayerInfo& pl = mWorld->player;
    const bool busted = pl.busted;
    if (busted)
    {
        mArrests++;
    }
    else
    {
        mDeaths++;
    }
    // the hp, car damage and place (in a car or not) at several frames back
    static const int back[6] = {120, 60, 30, 15, 5, 1};
    char hist[300];
    char* p = hist;
    p += _snprintf(p, sizeof(hist), "history (frames back: hp/car damage/%s/wanted):", "car");
    for (int i = 0; i < 6; i++)
    {
        if (mFrame - back[i] < 0 || back[i] > 127)
        {
            continue;
        }
        const HistFrame& h = mHist[(mFrame - back[i]) & 127];
        p += _snprintf(p, hist + sizeof(hist) - p, " -%d: %d/%d/%s/%d", back[i], h.hp, h.car_damage, h.in_car ? "car" : "foot", h.wanted);
    }
    char nearby[400];
    char* q = nearby;
    q += _snprintf(q, sizeof(nearby), "near:");
    int k;
    for (k = 0; k < mWorld->ped_count && k < 4; k++)
    {
        const WorldModel::PedInfo& d = mWorld->peds[k];
        q += _snprintf(q, nearby + sizeof(nearby) - q, " ped#%d %s%s d=%.1f", d.id, WorldModel_OccupationName(d.occupation), d.dead ? "(dead)" : "", d.dist);
    }
    for (k = 0; k < mWorld->car_count && k < 4; k++)
    {
        const WorldModel::CarInfo& d = mWorld->cars[k];
        q += _snprintf(q, nearby + sizeof(nearby) - q, " car#%d%s d=%.1f speed=%.2f drv=%d", d.idx, d.police ? "(police)" : "", d.dist, d.speed, d.has_driver);
    }
    BotDecision("f=%d %s #%d at (%.1f,%.1f) %s, wanted %d (for %d frames), hp %d, cause %d, killer %s%s d=%.1f, action %s; %s; %s", mFrame, busted ? "ARRESTED" : "KILLED", busted ? mArrests : mDeaths, pl.x, pl.y,
                pl.in_car ? "in a car" : "on foot", pl.wanted_stars, mWantedSince ? mFrame - mWantedSince : 0, pl.hp, pl.death_cause, pl.killer_occupation >= 0 ? WorldModel_OccupationName(pl.killer_occupation) : "none",
                pl.killer_cop ? " (cop)" : "", pl.killer_dist, mActions[mActionIdx] ? mActions[mActionIdx]->Name() : "-", hist, nearby);
}
