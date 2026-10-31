// Functions of several original files, told apart by address: main.cpp (0x4DAxxx), video.cpp
// (0x5D8E00-0x5D98E0), winmain.cpp (0x5E4DE0-0x5E5D60) and a few from elsewhere.
#include "winmain.hpp"
#include "hud_message_priority.hpp"
#include "Bink.hpp"
#include "InputRecorder_67F8B0.hpp"
#include "Char_Pool.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "NetPlay.hpp"
#include "Particle_4C.hpp"
#include "debug.hpp"
#include "dma_video.hpp"
#include "error.hpp"
#include "gbh_graphics.hpp"
#include "input.hpp"
#include "PlyDat.hpp"
#include "KeyboardLayout.hpp"
#include "GameSession.hpp"
#include "crt_stubs.hpp"
#include "registry.hpp"
#include "rng.hpp"
#include "resource.h"
#include "root_sound.hpp"
#include "network_data.hpp"
#include "FpsCounter.hpp"
#include <ddraw.h>
#include <direct.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
//#include <dmusics.h>

// for force links
#include "3rdParty/GTA2Hax/d3ddll/d3ddll.hpp"
#include "AmbulanceService.hpp"
#include "InputRecorder_67F8B0.hpp"
#include "Camera.hpp"
#include "Car.hpp"
#include "Crushers.hpp"
#include "Firefighters.hpp"
#include "Fix16_Rect.hpp"
#include "Generators.hpp"
#include "CarChaseTaskTable.hpp"
#include "Hud.hpp"
#include "ImGuiDebug.hpp"
#include "MapRenderer.hpp"
#include "FrameProfiler.hpp"
#include "SpriteRenderer.hpp"
#include "NetUi.hpp"
#include "Ped.hpp"
#include "ObjectDefinitions.hpp"
#include "Player.hpp"
#include "CollisionSoundQueue.hpp"
#include "CrimeReportQueue.hpp"
#include "cSampleManager.hpp"
#include "char.hpp"
#include "collide.hpp"
#include "ScriptGosubStack.hpp"
#include "Light.hpp"
#include "sound_obj.hpp"
#include "sprite.hpp"
#include "TextTable.hpp"

DEFINE_STATIC_GLOBAL(u8, sNetDisconnectPlayersBits, 0x6F56B8);
DEFINE_STATIC_GLOBAL(u32, sNetworkPlayerIdx, 0x6F56C8);
DEFINE_STATIC_GLOBAL(u32, sPlayerQuit, 0x6F5AEC);
DEFINE_STATIC_GLOBAL(u32, sMatchStartTime, 0x6F5A28);
DEFINE_STATIC_GLOBAL(u32, sNetworkFrameCounter, 0x6F5868);
DEFINE_STATIC_GLOBAL(u8, sbRecordStartTime, 0x6F593C);
DEFINE_STATIC_GLOBAL(Network_PlayerInputs, sCurrentNetInputs, 0x6F57D8);
DEFINE_STATIC_GLOBAL(Network_Buffer, sInputSendData, 0x6F5B18);
DEFINE_STATIC_GLOBAL(Network_PlayerInputs, sPrevNetInputs, 0x6F5B28);
DEFINE_STATIC_GLOBAL(ReceivedPacket*, spInputBuffer, 0x6F58C0);
DEFINE_STATIC_GLOBAL(u32, sCurrentInputsBufferSize, 0x6F58C4);
DEFINE_GLOBAL_ARRAY(u8, gSyncCheckData, 0x20, 0x6F58E0);
DEFINE_STATIC_GLOBAL(u32, sTotalNetworkTime, 0x6F5980);
DEFINE_GLOBAL(s32, gNetTimeLimit, 0x6F573C);
DEFINE_STATIC_GLOBAL(s32, sHudTimerIdx, 0x6F5860);
EXTERN_GLOBAL(s32, gNetTimeLimitEnabled);
DEFINE_STATIC_GLOBAL(s32, sNetTimeSecondsOfCurrMin, 0x6F58A0);
DEFINE_GLOBAL_ARRAY(u8, gNet_Unused_6F5AC8, 14, 0x6F5AC8);
DEFINE_GLOBAL(u32, gNet_Unused_6F5B00, 0x6F5B00);
DEFINE_GLOBAL(u32, gNet_Unused_6F580C, 0x6F580C);
DEFINE_GLOBAL(u32, gNet_Unused_6F5AC0, 0x6F5AC0);
DEFINE_STATIC_GLOBAL(s32, sFramesOfCurrSecond, 0x6F5858);
DEFINE_STATIC_GLOBAL(u8, sNetTimerFlashing, 0x6F59C0);
DEFINE_GLOBAL(NetworkGameSettings, gNetworkGameSettings, 0x707098);

static T_gbh_SetBeginSceneCB pBeginSceneCB = NULL;

class DllRaii
{
  public:
    explicit DllRaii(const char* pDllName) : mDll(NULL)
    {
        mDll = ::LoadLibrary(pDllName);
    }

    ~DllRaii()
    {
        if (mDll)
        {
            ::FreeLibrary(mDll);
        }
    }

    FARPROC GetProcAddress(const char* funcName) const
    {
        if (!mDll)
        {
            printf("DLL not loaded\n");
            return NULL;
        }
        return ::GetProcAddress(mDll, funcName);
    }

  private:
    HMODULE mDll;
};

// Not in the original: hooks the ImGui debug draw into d3ddll. Called by our entry points before WinMain_5E53F0
void LoadBeginSceneCBPtr()
{
    static DllRaii hD3Ddll("d3ddll.dll"); // freed after WinMain return, only if this func is called

    pBeginSceneCB = reinterpret_cast<T_gbh_SetBeginSceneCB>(hD3Ddll.GetProcAddress("gbh_SetBeginSceneCB"));
    if (!pBeginSceneCB)
    {
        printf("Using OG Render - no ImGui debug will function/display\n");
    }
    else
    {
        printf("ImGui debug enabled\n");
        pBeginSceneCB(ImGuiDebugDraw);
    }
}

void force_link()
{
    ObjectDefinitions phi_8ca8;
    phi_8ca8.AllocDefinitionWithSprite(0, 0, 0, 0);

    AmbulanceTask jaw_20;
    jaw_20.mPatientCount = 1;

    CollisionSoundQueue collision_sound_queue;
    collision_sound_queue.mpSoundObj = NULL;

    GeneratorPool maccies;
    maccies.FinalizeAfterScriptLoad();

    Sprite car_3c;
    car_3c.mInstanceId = 5;

    CrusherPool snooky;
    snooky.IsCarBeingCrushed(NULL);

    NetUi network;
    network.cb_FillSessionList(NULL, NULL);

    Light light;
    light.AddToGrid();

    PedManager PedManager;
    PedManager.DoIanTest(0);

    Player lewin(1);
    u8 zero_u8 = 0;
    lewin.GetIdx();
    lewin.AddCarToHistory(NULL);
    lewin.PromoteCarInHistory(NULL, false);
    lewin.RemoveCarFromHistory(NULL);
    lewin.SetKFCarWeapon(NULL, 0);
    lewin.SetKFWeapon(0);
    lewin.ClearKFWeapon();
    lewin.GetCurrPlayerWeapon();
    lewin.SetWeapon(NULL);
    lewin.HasAnyAmmo();
    lewin.AddWeaponWithAmmo(0, 0);
    lewin.SelectNextOrPrevWeapon(false, false);
    lewin.LoadCarWeapons(NULL);
    lewin.ClearCarWeapons();
    lewin.CleanupEmptyAmmoWeapons();
    lewin.UnloadCarWeapons();
    lewin.RemovePlayerWeapons();
    lewin.ClearPowerUps();
    lewin.ClearPowerUpsExceptJailCard();
    lewin.CollectPowerUp(0);
    lewin.tick_down_powerups();
    lewin.RestorePowerUpsFromSave(NULL);
    lewin.TeleportToDebugCam();
    lewin.DebugWatchNearestCar();
    lewin.InitPlayerPed(NULL);
    lewin.SetInputs(0);
    lewin.IncrementGangRespectFromDebugKeys(0);
    lewin.IncreaseWantedLevelFromDebugKeys();
    lewin.Hud_Controls(0);
    lewin.HandleKeyRelease(0);
    lewin.CharacterControls();
    lewin.ControlInputs();
    lewin.HandleControls(NULL);
    lewin.DoCarControlInputs(NULL);
    lewin.DoPedControlInputs(NULL);
    lewin.ShowDebugInfo(false);
    lewin.RespawnPlayer();
    lewin.Wasted();
    lewin.UpdateAuxPedDeath();
    lewin.Busted();
    lewin.UpdateCurrentZones();
    lewin.UpdateSoundListener();
    lewin.HandleDebugZoom();
    lewin.UpdateCamera(NULL);
    lewin.Disconnect();
    lewin.Service();
    lewin.UpdatePaused();
    lewin.EndRemoteControl();
    lewin.ResetAuxCamera();
    lewin.StartRemoteControl(NULL);
    lewin.WatchCar(NULL);
    lewin.GetPosU8(zero_u8, zero_u8, zero_u8);
    lewin.GetPlayerCar();
    lewin.get_pos(NULL, NULL, NULL);
    lewin.ChangeLifeCountByAmount(0);
    lewin.ColorScoreFromRemap();
    lewin.SetScoreTextColour();
    lewin.InitializePlayerState();
    lewin.DebugToggleRemoteControl();
    lewin.GetDeathText();
    lewin.DisableInputs();
    lewin.DisableAllControls();
    lewin.EnableAllControls();
    lewin.EnableKFMode();
    lewin.DisableKFMode();
    lewin.DisableEnterVehicles();
    lewin.EnableEnterVehicles();
    lewin.RestoreCarsFromSave();
    lewin.CopyPlayerDataToSave(NULL);
    lewin.UpdateGameFromSave(NULL);
    lewin.ApplyCheats();
    lewin.ClearInputs();

    Camera camera;
    camera.SavePrevCamPos();
    camera.IncreaseElevation();
    camera.DecreaseElevation();
    camera.ResetElevation();
    camera.ClampTargetZ();
    camera.ApplyMovementDeltaFrom(NULL);
    camera.MoveTarget(true, true, true, true, true, true);
    camera.ResetCameraSmoothing();
    camera.RefreshBoundaries();
    camera.SetShake(1);
    camera.ResetPanning();
    camera.SetTarget(Fix16(), Fix16(), Fix16(), Fix16());
    camera.IsPointInBoundaries(1, 1);

    Ped cn;
    cn.SetRecentCrimeTimer();
    cn.SetPlayer(NULL, false);
    cn.IsEmergencyOccupation();

    ScriptGosubStack miss2;
    miss2.add_503160(NULL);
    miss2.PopFrame();
    miss2.PopToDepth(0);

    InputRecorder_67F8B0 input_recorder;
    input_recorder.StaticShutdown();
    input_recorder.clear_inputs();
    input_recorder.set_input(0);
    input_recorder.clear_input(0);
    input_recorder.should_ignore_input(0);
    input_recorder.should_record_input(0);
    input_recorder.save_replay_record(0);
    input_recorder.SaveReplay();
    input_recorder.modify_inputs(0);
    input_recorder.AppendReplayHeader();
    input_recorder.LoadReplayHeader(false);
    input_recorder.VerifyAttractFilesExist();
    input_recorder.GetNextAttrReplay(NULL);
    input_recorder.input_init_replay(NULL);
    input_recorder.input_init_live(NULL);
    input_recorder.replay_save(NULL);
    input_recorder.get_input_bits();
    input_recorder.save_replay_inputs(0, 0);
    input_recorder.DisplayInputBits();
    input_recorder.IsPlayingBack();
    input_recorder.ShowInput();

    Hud garox;

    MapRenderer nano;
    u16 zero = 0;
    nano.draw_bottom(zero);

    SpriteRenderer sprite_renderer;
    sprite_renderer.ResetAll();

    FrameProfiler mike;
    mike.DebugDrawProfiling();

    CrimeReport crime_report_14;
    CrimeReportQueue crime_report_queue_CC;

    Firefighter tango_28;
    tango_28.Clear();

    FirefighterPool tango_54;

    CarChaseTaskTable car_chase_task_table;
    car_chase_task_table.FreeEntry(NULL);

    CollisionCounters collide_C;

    Fix16_Rect car14_18;
    car14_18.DoSetCurrentRect();

    Particle_4C particle;
    particle.PoolAllocate();
}

DEFINE_GLOBAL(HINSTANCE, gHInstance, 0x708220);
DEFINE_STATIC_GLOBAL(HANDLE, sMutex, 0x707078);
DEFINE_GLOBAL(s32, gGTA2VersionMajor, 0x708280);
DEFINE_GLOBAL(s32, gGTA2VersionMinor, 0x708284);
DEFINE_GLOBAL_ARRAY(char_type, gWorkingDir, 256, 0x707F64);
DEFINE_GLOBAL_INIT(bool, bDoFrontEnd, true, 0x626B68);

DEFINE_GLOBAL(s32, window_width, 0x706630);
DEFINE_GLOBAL(s32, window_height, 0x706B50);
DEFINE_STATIC_GLOBAL(s32, sFullWidth, 0x706B5C);
DEFINE_STATIC_GLOBAL(s32, sFullHeight, 0x706798);
DEFINE_STATIC_GLOBAL(s32, sWindowX, 0x706B60);
DEFINE_STATIC_GLOBAL(s32, sWindowY, 0x706B64);
DEFINE_GLOBAL(s32, bStartNetworkGame, 0x7081F0);
DEFINE_STATIC_GLOBAL(BYTE, sMinimizedState, 0x70827C);
DEFINE_STATIC_GLOBAL(BYTE, sbVideoModeResetPending, 0x706C5D);
DEFINE_STATIC_GLOBAL(BYTE, sbReplayMode, 0x6F5B71);
DEFINE_STATIC_GLOBAL_INIT(s32, sStartMode, 2, 0x626A0C);

DEFINE_STATIC_GLOBAL(s32, sbTrippleBuffer, 0x706C54);
DEFINE_STATIC_GLOBAL(char_type, sbWindowedModeAvailable, 0x706C5C);
DEFINE_GLOBAL(u32, gBufferMode, 0x706B34);
DEFINE_STATIC_GLOBAL_ARRAY(char_type, sRenderDllName, 256, 0x7067F0);
DEFINE_STATIC_GLOBAL_ARRAY(char_type, sVideoDllName, 256, 0x706654);
DEFINE_STATIC_GLOBAL(s32, sVideodevice, 0x70694C);
DEFINE_STATIC_GLOBAL(u16, sDMusicVer, 0x67BD32);

DEFINE_GLOBAL_INIT(char_type, gLighting, 1, 0x626A09);

DEFINE_GLOBAL(u32, gLightingDrawFlag, 0x7068F4);
DEFINE_STATIC_GLOBAL(LPDIRECTINPUTDEVICEA, sMouseDevice, 0x706C60);
// The original mouse format uses the seven object records already recovered in Frontend.cpp.
extern DIOBJECTDATAFORMAT gMouseObjectDataFormats_5EA3D0[7];
DEFINE_STATIC_GLOBAL_INIT(DIDATAFORMAT, sMouseDataFormat, {24u COMMA 16u COMMA DIDF_RELAXIS COMMA 16u COMMA 7u COMMA gMouseObjectDataFormats_5EA3D0}, 0x601A84);

DEFINE_STATIC_GLOBAL_INIT(u8, sMaxFrameRate, 1, 0x626A08);
DEFINE_STATIC_GLOBAL(u8, sMinFrameRate, 0x706C50);

DEFINE_STATIC_GLOBAL(u8, sFramesSinceDraw, 0x6F58D8);
DEFINE_STATIC_GLOBAL(u8, sbInputsPending, 0x6F5760);
DEFINE_STATIC_GLOBAL(u8, sbForceGameUpdate, 0x6F5880);

MATCH_FUNC(0x4DA820)
EXPORT void ForceGameUpdate()
{
    sbForceGameUpdate = true;
}

MATCH_FUNC(0x4DA830)
EXPORT void __stdcall ResetFrameTimer()
{
    sMatchStartTime = timeGetTime();
    sFramesSinceDraw = 0;
}

MATCH_FUNC(0x5D8E00)
void UpdateGameScreenSize()
{
    if (gGame_0x40)
    {
        if (gVidSys->field_40_full_screen == -2)
        {
            if (gCachedScreenDimension != window_width)
            {
                gGame_0x40->SetScreenSize(window_width, window_height);
            }
            gCachedScreenDimension = window_width;
        }
        else
        {
            if (static_cast<u32>(gCachedScreenDimension) != gVidSys->field_4C_rect_bottom)
            {
                gGame_0x40->SetScreenSize(gVidSys->field_48_rect_right, gVidSys->field_4C_rect_bottom);
            }
            gCachedScreenDimension = gVidSys->field_4C_rect_bottom;
        }
        ForceGameUpdate();
        ResetFrameTimer();
    }
}

MATCH_FUNC(0x5D8EB0)
EXPORT void __stdcall Init_FrameRateLightingAndExplodingScores()
{
    if (bStartNetworkGame)
    {
        sMaxFrameRate = 1;
        sMinFrameRate = 1;
    }
    else
    {
        sMaxFrameRate = gRegistry.Get_Screen_Setting("max_frame_rate", 1) != 0;
        sMinFrameRate = gRegistry.Get_Screen_Setting("min_frame_rate", 0) != 0;
    }

    if (gRegistry.Get_Screen_Setting("lighting", 1))
    {
        gLighting = 1;
        gLightingDrawFlag = 0x8000;
    }
    else
    {
        gLighting = 0;
        gLightingDrawFlag = 0;
    }

    if (gSpriteMaskCache)
    {
        gSpriteMaskCache->InvalidateAllMasks();
    }

    bExplodingScoresOff = gRegistry.Get_Screen_Setting("exploding_on", 1) != 1;
}

MATCH_FUNC(0x5D7C60)
EXPORT void __stdcall Input_MouseAcquire()
{
    if (sMouseDevice)
    {
        sMouseDevice->Acquire();
    }
}

MATCH_FUNC(0x5D7C70)
EXPORT void __stdcall Input_ReleaseMouse()
{
    if (sMouseDevice)
    {
        sMouseDevice->Unacquire();
        sMouseDevice->Release();
        sMouseDevice = NULL;
    }
}

MATCH_FUNC(0x5D94F0)
EXPORT void __stdcall HideCursor()
{
    do
    {

    } while (ShowCursor(FALSE) >= 0);
}

MATCH_FUNC(0x5D7BF0)
EXPORT bool Input_InitMouse()
{
    if (!gpDInput || sMouseDevice || gpDInput->CreateDevice(GUID_SysMouse, &sMouseDevice, NULL) ||
        sMouseDevice->SetDataFormat(&sMouseDataFormat) || sMouseDevice->SetCooperativeLevel(gHwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND))
    {
        return false;
    }
    sMouseDevice->Acquire();
    return true;
}

MATCH_FUNC(0x5D93A0)
EXPORT void __stdcall SetFullScreenMode()
{
    bool bModeChanged = false;
    s32 bcheckModeRet = pVid_CheckMode(gVidSys, sFullWidth, sFullHeight, 16);
    if (!bcheckModeRet)
    {
        if (sFullWidth == 640 ||
            (sFullWidth = 640,
             bModeChanged = true,
             sFullHeight = 480,
             (bcheckModeRet = pVid_CheckMode(gVidSys, 640, 480, 16)) == 0))
        {
            FatalError_4A38C0(Gta2Error::VideomodeNotAvailable,
                              "C:\\Splitting\\Gta2\\Source\\video.cpp",
                              1359,
                              sFullWidth,
                              sFullHeight,
                              16);
        }
    }

    if (gVidSys)
    {
        gVidSys->field_4_flags |= 0x100;
    }

    if (pVid_SetMode(gVidSys, gHwnd, bcheckModeRet))
    {
        FatalError_4A38C0(Gta2Error::FailedToSetFullscreenVideomode, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1365, bcheckModeRet);
    }

    HideCursor();

    SetWindowLongA(gHwnd, GWL_STYLE, WS_VISIBLE);
    SetWindowPos(gHwnd, NULL, 0, 0, 0, 0, 0x63Bu);
    UpdateWindow(gHwnd);
    ShowWindow(gHwnd, SW_SHOWNORMAL);

    Input_InitMouse();

    if (bModeChanged)
    {
        gRegistry.Set_Screen_Setting("full_width", sFullWidth);
        gRegistry.Set_Screen_Setting("full_height", sFullHeight);
    }
}

MATCH_FUNC(0x5D92C0)
EXPORT char_type IsWindowedModeAvailable()
{
    return sbWindowedModeAvailable;
}

MATCH_FUNC(0x5D9660)
EXPORT void __stdcall ShowCursor_5D9660()
{
    s32 refCount;
    do
    {
        refCount = ShowCursor(TRUE);
    } while (refCount < 0);
}

// __stdcall, not __cdecl: see GetLayout (the `push 0x316` goes before the rect loads)
MATCH_FUNC(0x5D9510)
EXPORT bool __stdcall SetWindowedMode()
{

    if (!IsWindowedModeAvailable())
    {
        return false;
    }

    Input_ReleaseMouse();
    SetWindowLongA(gHwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
    SetWindowPos(gHwnd, NULL, 0, 0, 0, 0, 0x63Bu);
    UpdateWindow(gHwnd);
    ShowWindow(gHwnd, SW_SHOW);

    RECT windowRect;
    GetWindowRect(gHwnd, &windowRect);

    RECT clientRect;
    GetClientRect(gHwnd, &clientRect);

    if (!SetWindowPos(gHwnd,
                      NULL,
                      sWindowX,
                      sWindowY,
                      windowRect.right + clientRect.left + window_width - windowRect.left - clientRect.right,
                      windowRect.bottom + clientRect.top + window_height - windowRect.top - clientRect.bottom,
                      0x316))
    {
        return false;
    }

    UpdateWindow(gHwnd);
    ShowWindow(gHwnd, SW_SHOW);
    if (gVidSys)
    {
        gVidSys->field_4_flags |= 0x100;
    }

    if (pVid_SetMode(gVidSys, gHwnd, -2) == 1)
    {
        return false;
    }
    else
    {
        ShowCursor_5D9660();
        return true;
    }
}

MATCH_FUNC(0x5D8F70)
EXPORT s32 ReadScreenSettings()
{
    const s32 full_width_old = sFullWidth;
    const s32 window_width_old = window_width;
    const s32 full_height_old = sFullHeight;
    const s32 startMode_old = sStartMode;
    const s32 trippleBuffer_old = sbTrippleBuffer;
    const s32 window_height_old = window_height;

    if (bDoFrontEnd)
    {
        window_width = 640;
        window_height = 480;
        sFullWidth = 640;
        sFullHeight = 480;
    }
    else
    {
        window_width = gRegistry.Get_Screen_Setting("window_width", 640);
        window_height = gRegistry.Get_Screen_Setting("window_height", 480);
        sFullWidth = gRegistry.Get_Screen_Setting("full_width", 640);
        sFullHeight = gRegistry.Get_Screen_Setting("full_height", 480);
    }

    sStartMode = gRegistry.Get_Screen_Setting("start_mode", 1);

    if (gBufferMode == 0)
    {
        sbTrippleBuffer = 1;
    }
    else
    {
        sbTrippleBuffer = gRegistry.Get_Screen_Setting("tripple_buffer", 0);
    }

    if (sStartMode == startMode_old && sbTrippleBuffer == trippleBuffer_old)
    {
        if (sStartMode == 1)
        {
            if (sFullWidth == full_width_old && sFullHeight == full_height_old)
            {
                return 0;
            }
        }
        else if (sStartMode || window_width == window_width_old && window_height == window_height_old)
        {
            return 0;
        }
    }

    return 1;
}

MATCH_FUNC(0x5D98E0)
EXPORT void __stdcall SetSavedGamma()
{
    DWORD gamma = gRegistry.Get_Screen_Setting("gamma", 10);
    if (SetGamma(gamma))
    {
        counter_706C4C = 30;
    }
}

MATCH_FUNC(0x5D92D0)
EXPORT void __stdcall SetVideoModeFromSettings()
{
    Init_FrameRateLightingAndExplodingScores();
    ReadScreenSettings();
    Input_MouseAcquire();

    pVid_CloseScreen(gVidSys);

    if (sbTrippleBuffer)
    {
        gVidSys->field_4_flags |= 0x10;
    }
    else
    {
        gVidSys->field_4_flags &= ~0x10u;
    }

    if (!sStartMode)
    {
        if (!SetWindowedMode())
        {
            if (window_width == 640 || (window_width = 640, window_height = 480, !SetWindowedMode()))
            {
                SetFullScreenMode();
                sStartMode = 1;
                gRegistry.Set_Screen_Setting("start_mode", 1u);
            }
            else
            {
                gRegistry.Set_Screen_Setting("window_width", window_width);
                gRegistry.Set_Screen_Setting("window_height", window_height);
            }
        }
    }
    else
    {
        SetFullScreenMode();
    }
    SetSavedGamma();
}

MATCH_FUNC(0x5D9860)
EXPORT s32 jGBH_Graphics_Free()
{
    return GBH_Graphics_Free();
}

MATCH_FUNC(0x5D9830)
EXPORT void __stdcall GBH_Graphis_DMA_Video_Free()
{
    jGBH_Graphics_Free();
    if (gVidSys)
    {
        DMA_Video_Unload(gVidSys);
        gVidSys = NULL;
    }
    ShowCursor_5D9660();
}

MATCH_FUNC(0x5D9290)
EXPORT bool Vid_FindDevice_5D9290()
{
    SDevice* pDevice = pVid_FindDevice(gVidSys, sVideodevice);
    return pDevice && (pDevice->field_4_flags & 1) == 1;
}

MATCH_FUNC(0x5D9800)
EXPORT s32 GetHwndDCDeviceCaps()
{
    HDC hdc = GetDC(gHwnd);
    s32 deviceCaps = GetDeviceCaps(hdc, BITSPIXEL);
    ReleaseDC(gHwnd, hdc);
    return deviceCaps;
}

MATCH_FUNC(0x5D97C0)
EXPORT void __stdcall GBH_GraphicsInit()
{
    if (GBH_GraphicsLoad(sRenderDllName, gVidSys))
    {
        FatalError_4A38C0(Gta2Error::DllLoadError, "C:\\Splitting\\Gta2\\Source\\video.cpp", 206, gTmpBuffer);
    }
}

MATCH_FUNC(0x5D96C0)
EXPORT void InitVideo()
{
    s32 ddraw_flags = 0;
    if (DMA_Video_LoadDll(sVideoDllName))
    {
        FatalError_4A38C0(Gta2Error::DllLoadError, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1647, sVideoDllName);
    }

    gVidSys = pVid_Init_SYS(gHInstance, 0);

    pVid_SetDevice(gVidSys, sVideodevice);

    if (!gVidSys)
    {
        FatalError_4A38C0(Gta2Error::DMAVideoInitFail, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1656);
    }

    if (gBufferMode == 1)
    {
        ddraw_flags = 64;
    }

    gVidSys->field_4_flags |= ddraw_flags;

    if (GetHwndDCDeviceCaps() == 16)
    {
        bool bDeviceFound = Vid_FindDevice_5D9290();
        sbWindowedModeAvailable = true;
        if (!bDeviceFound)
        {
            sbWindowedModeAvailable = false;
        }
    }
    else
    {
        sbWindowedModeAvailable = false;
    }

    SetVideoModeFromSettings();

    GBH_GraphicsInit();

    SetSavedGamma();

    pVid_ClearScreen(gVidSys, 0, 0, 0, 0, 0, gVidSys->field_48_rect_right, gVidSys->field_4C_rect_bottom);

    pVid_FlipBuffers(gVidSys);

    pVid_ClearScreen(gVidSys, 0, 0, 0, 0, 0, gVidSys->field_48_rect_right, gVidSys->field_4C_rect_bottom);
}

MATCH_FUNC(0x4DA440)
EXPORT void __stdcall Init_keybrd_jolly_and_sound()
{
    KeyboardLayout::create();
    PlyDat::Create();

    if (!bSkip_audio)
    {
        gRoot_sound.Init();
        gRoot_sound.SetCDVol(gRegistry.Get_Sound_Setting("CDVol", 127));
        gRoot_sound.SetSfxVol(gRegistry.Get_Sound_Setting("SFXVol", 127));
        gRoot_sound.Set3DSound(gRegistry.Sound_Setting_Exists("do_3d_sound"));
        gRegistry.Clear_Or_Delete_Sound_Setting("do_3d_sound", gRoot_sound.Get3DSound());
    }
}

MATCH_FUNC(0x4DB170)
void __stdcall ClearDebugFlags()
{
    bShow_all_arrows = false;
    bDo_kill_phones_on_answer = false;
    bSkip_dummies = false;
    bSkip_tiles = false;
    bDo_test = false;
    bSkip_mission = false;
    bDo_brian_test = false;
    bDo_iain_test = false;
    bSkip_user = false;
    bSkip_traffic_lights = false;
    bSkip_recycling = false;
    bLog_collisions = false;
    bDo_show_traffic_lights_info = false;
    bLimit_recycling = false;
    bNo_annoying_chars = false;
    bSkip_slopes = false;
    bSkip_left = false;
    bSkip_right = false;
    bSkip_top = false;
    bSkip_bottom = false;
    bSkip_lid = false;
    bLog_routefinder = false;
    bDo_mike = false;
    bSkip_particles = false;
    bShow_hidden_faces = false;
    bGet_all_weapons = false;
    bDo_exit_after_replay = false;
    bDont_get_car_back = false;
    bSkip_ambulance = false;
    bSkip_frontend = false;
    bDo_invulnerable = false;
    bKeep_weapons_after_death = false;
    bSkip_skidmarks = false;
    gDo_infinite_lives = false;
    bDo_load_savegame = false;
    bSkip_audio = false;
    bDo_debug_keys = false;
    bLog_input = false;
    bLog_directinput = false;
    bIgnore_replay_header = false;
    bSkip_trains = false;
    bSkip_buses = false;
    bSkip_quit_confirm = false;
    bSkip_fire_engines = false;
    bSkip_window_check = false;
    gDo_miss_logging = false;
    gDebugFlag_Unused = false;
    bDo_police_1 = false;
    bDo_police_2 = false;
    bDo_police_3 = false;
    bSkip_draw = false;
    bDo_free_shopping = false;
    gCheatMiniCars = false;
    gCheatGetPlayerPoints = false;
    gCheatGet10MillionMoney = false;
    gCheatUnlimitedElectroGun = false;
    gCheatAllGangMaxRespect = false;
    gCheatGet99Lives = false;
    gCheat10xMultiplier = false;
    gCheatUnlockThreeLevels = false;
    gCheatUnlockLevelsOneAndTwo = false;
    gCheatUnlockAllLevels = false;
    gCheatGetBasicWeaponsMaxAmmo = false;
    gCheatOnlyElvisPeds = false;
    gCheatOnlyMuggerPeds = false;
    gCheatNakedPeds = false;
    gCheatUnlimitedJailCard = false;
    gCheatInvisibility = false;
    gCheatUnlimitedDoubleDamage = false;
    gCheatJailCardSmgRespect = false;
    gCheatUnlimitedFlameThrower = false;
}

MATCH_FUNC(0x4DB070)
EXPORT void __stdcall ShowPlayerLeftMessage(u8 idx)
{
    Player* pPlayer = gGame_0x40->GetPlayer(idx);
    pPlayer->Disconnect();
    swprintf(tmpBuff, gText_0x14->Find("comms1"), pPlayer->mPlayerName);
    gHud_2B00->mMpMessage.ShowText(tmpBuff);
}

MATCH_FUNC(0x4DB0D0)
void __stdcall ExitGameCallback(Game* pGame, s32 reason)
{
    char_type Buffer[260];
    sprintf(Buffer, "(ExitGameCallback)Called with reason - %d", reason);
    gErrorLog_67C530.Write(Buffer);
    pGame->ExitGameNoBonus(0, GameExitType::CloseGameImmediately_1);
}

// https://decomp.me/scratch/VazoB
MATCH_FUNC(0x4DA4D0)
EXPORT void __stdcall InitializeGame()
{
    if (sbReplayMode)
    {
        gInputRecorder.input_init_replay(gHInstance);
        sbReplayMode = false;
    }
    else
    {
        gInputRecorder.input_init_live(gHInstance);
    }

    gRoot_sound.Set3DSound(gRegistry.Sound_Setting_Exists("do_3d_sound"));
    gRegistry.Clear_Or_Delete_Sound_Setting("do_3d_sound", gRoot_sound.Get3DSound());

    if (bStartNetworkGame)
    {
        gTagGame.Init();

        ClearDebugFlags();

        gGame_0x40 = new Game(gNetPlay.CountActivePlayers(), gNetPlay.GetPlayerIdx());
        gNetPlay.SetExitGameCallBack(reinterpret_cast<int>(ExitGameCallback), gGame_0x40);

        memset(&sCurrentNetInputs, 0, sizeof(sCurrentNetInputs));
        memset(&sPrevNetInputs, 0, sizeof(sPrevNetInputs));

        memset(gNet_Unused_6F5AC8, 0, sizeof(gNet_Unused_6F5AC8));
        sNetworkPlayerIdx = gNetPlay.GetPlayerIdx();
        spInputBuffer = reinterpret_cast<ReceivedPacket*>(&sCurrentNetInputs.mInputs[0]);
        sCurrentInputsBufferSize = 48;
        gNet_Unused_6F5B00 = 0;
        sbRecordStartTime = true;
        sPlayerQuit = false;
        sNetDisconnectPlayersBits = 0;
        sNetworkFrameCounter = 0;
        gNet_Unused_6F580C = 0;
        sTotalNetworkTime = 0;
        gNet_Unused_6F5AC0 = 0;
        gNetTimeLimit = gGameSession.GetTimeLimit();
        if (gNetTimeLimit > 60)
        {
            gNetTimeLimit = 60;
        }
        sHudTimerIdx = -1;
        gNetTimeLimitEnabled = (gNetTimeLimit != 0);
        sNetTimeSecondsOfCurrMin = 0;
        sFramesOfCurrSecond = 0;
        sNetTimerFlashing = 0;
    }
    else
    {
        gGame_0x40 = new Game(1, 0);
    }

    gGame_0x40->LoadGameFiles();
    gGame_0x40->BootGame();

    sMatchStartTime = timeGetTime();
    sFramesSinceDraw = 0;
    sbForceGameUpdate = false;
    sbInputsPending = true;
}

MATCH_FUNC(0x5D9680)
EXPORT void OnVideoModeChanged()
{
    ConvertColourBanks_5D7CB0();
    UpdateGameScreenSize();
}

MATCH_FUNC(0x5D9690)
EXPORT void __stdcall ApplyChangedScreenSettings()
{
    Init_FrameRateLightingAndExplodingScores();
    if (static_cast<u8>(ReadScreenSettings()) || sbVideoModeResetPending)
    {
        SetVideoModeFromSettings();
        OnVideoModeChanged();
        sbVideoModeResetPending = false;
    }
    Input_MouseAcquire();
}

MATCH_FUNC(0x4DA780)
EXPORT bool ExecuteGame_4DA780()
{
    bool exit_requested = gGame_0x40->ExecuteGame();
    if (gFpsCounter)
    {
        gFpsCounter->mExecuteGameTimer.AccumulateElapsed();
    }
    return exit_requested;
}

MATCH_FUNC(0x4DA7B0)
void __stdcall Draw_4DA7B0()
{
    gGame_0x40->Draw();
    pVid_FlipBuffers(gVidSys);
    if (bSkip_lid || bSkip_top || bSkip_bottom || bSkip_left || bSkip_right)
    {
        pVid_ClearScreen(gVidSys, 0, 0, 0, 0, 0, gVidSys->field_48_rect_right, gVidSys->field_4C_rect_bottom);
    }
}

// Re-sends our inputs of a previous frame to one player.
MATCH_FUNC(0x4DA9B0)
EXPORT void __stdcall NetSendOldInputs(Network_InputData* pInputs, s32 delayed_ticks, u8 player_idx)
{
    sInputSendData.mData = pInputs;
    sInputSendData.mLen = bDo_sync_check ? sizeof(Network_InputData) : sizeof(u32);
    gNetPlay.SendToPlayer(&sInputSendData, player_idx, delayed_ticks);
}

MATCH_FUNC(0x4DA9F0)
EXPORT void Net_Receive_Players_Inputs()
{
    LOCAL_STATIC(Network_PlayerInputs, sNetInputsHistory1, 0x6F56E0);
    LOCAL_STATIC(Network_PlayerInputs, sNetInputsHistory2, 0x6F5798);
    s32 retries = 0;
    s32 max_retries = sbRecordStartTime ? 25 : 6;

    // Update previous stored inputs
    memcpy(&sNetInputsHistory2, &sNetInputsHistory1, sizeof(sNetInputsHistory2));
    memcpy(&sNetInputsHistory1, &sCurrentNetInputs, sizeof(sNetInputsHistory1));

    // Check which players we are waiting for
    u32 waiting_bits = 0;
    for (u32 i = 0; i < GTA2_COUNTOF(gNetPlay.mN2.mPlayers); i++)
    {
        if (gNetPlay.mN2.mPlayers[i].mInUse && i != sNetworkPlayerIdx)
        {
            waiting_bits |= 1 << i;
        }
    }

    u32 start_time = timeGetTime();
    sTotalNetworkTime = 0;

    // Do not execute next game tick until we receive all players inputs
    while (waiting_bits)
    {
        // Leave a window to escape the loop if player is trying to exit the game
        if (gGame_0x40->UpdateExitTimer())
        {
            return;
        }

        // Wait a packet. If one was received, analyse its data and do something
        s32 player_idx;
        u32 packet_subtype;
        if (gNetPlay.ReceiveGameMessage(reinterpret_cast<Network_Buffer*>(&spInputBuffer), &player_idx, &packet_subtype))
        {
            switch (packet_subtype)
            {
                case packet_subtype::StandardPacket_2:
                {
                    switch (spInputBuffer->s_packet.mSubtype) // std_packet_subtype
                    {
                        case std_packet_subtype::GiveUp_1:
                            waiting_bits &= ~(1 << reinterpret_cast<s32>(spInputBuffer->s_packet.mpData));
                            sNetDisconnectPlayersBits |= 1 << reinterpret_cast<s32>(spInputBuffer->s_packet.mpData);
                            break;
                        case std_packet_subtype::PlayerQuit_2:
                            waiting_bits &= ~(1 << player_idx);
                            sNetDisconnectPlayersBits |= 1 << player_idx;
                            break;
                        case std_packet_subtype::KickFromConnection_3:
                            waiting_bits = 0;
                            sPlayerQuit = true;
                            break;
                        default:
                            FatalError_4A38C0(Gta2Error::InvalidLine, "C:\\Splitting\\Gta2\\Source\\main.cpp", 853, 0);
                            break;
                    }
                    break;
                }
                case packet_subtype::PlayerInputs_3: // store player input for the latest/current tick
                    memcpy(&sCurrentNetInputs.mInputs[player_idx], spInputBuffer, sCurrentInputsBufferSize);
                    waiting_bits &= ~(1 << player_idx);

                    // Now check RNG if sync_check is activated. If it doesnt match, then do FatalError
                    if (bDo_sync_check &&
                        sCurrentNetInputs.mInputs[player_idx].mRng !=
                            sCurrentNetInputs.mInputs[sNetworkPlayerIdx].mRng)
                    {
                        FatalError_4A38C0(Gta2Error::SyncErrorRandom,
                                          "C:\\Splitting\\Gta2\\Source\\main.cpp",
                                          822,
                                          sCurrentNetInputs.mInputs[player_idx].mRng,
                                          sCurrentNetInputs.mInputs[sNetworkPlayerIdx].mRng);
                    }
                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidLine, "C:\\Splitting\\Gta2\\Source\\main.cpp", 860, 0);
                    break;
            }
        }

        sTotalNetworkTime += gNetPlay.mTimeDiff;

        // Now manage those players we didn't received packets

        if (timeGetTime() - start_time > 1000 && retries >= max_retries && waiting_bits)
        {
            // Give up on the players that didn't answer
            for (u32 j = 0; j < 6 && waiting_bits; j++)
            {
                if (waiting_bits & (1 << j))
                {
                    waiting_bits &= ~(1 << j);
                    sNetDisconnectPlayersBits |= 1 << j;
                    gNetPlay.SendGiveUp(j);
                    gTagGame.SetQuit(j);
                }
            }
        }
        else if (timeGetTime() - start_time > 1000 && waiting_bits)
        {
            // Send previous inputs in case of lost packets
            start_time = timeGetTime();
            retries++;
            for (u8 k = 0; k < 6; k++)
            {
                if (waiting_bits & (1 << k))
                {
                    NetSendOldInputs(&sNetInputsHistory1.mInputs[sNetworkPlayerIdx], 1, k);
                    NetSendOldInputs(&sNetInputsHistory2.mInputs[sNetworkPlayerIdx], 2, k);
                }
            }
        }
    }
}

// Sum of all the bytes of a file
MATCH_FUNC(0x4DB120)
EXPORT s32 __stdcall FileByteSum(FILE* hFile)
{
    s32 sum = 0;
    u8 c;
    while (crt::fread(&c, 1, 1, hFile) > 0)
    {
        sum += c;
    }
    return sum;
}

// Fills in the sync check data that is compared between players at the start of a network game
MATCH_FUNC(0x4DB2E0)
EXPORT void __stdcall MakeInitialSyncData(u8* pSyncData)
{
    SyncCheckData* pData = reinterpret_cast<SyncCheckData*>(pSyncData);
    struct _stat st;

    memset(pData, 0, sizeof(SyncCheckData));
    pData->mType = std_packet_subtype::SyncCheckData_5;
    if (gCarManager_6C)
    {
        gCarManager_6C->mDoFreeShopping = bDo_free_shopping;
    }

    pData->mFlags = 0;
    pData->mFlags = bLog_random_extra != 0;
    pData->mFlags |= bDo_sync_check ? 2 : 0;
    pData->mFlags |= bLog_random ? 4 : 0;
    pData->mExeA = -1;
    pData->mExeB = 0;

    if (_stat(gGameSession.GetMapName(), &st) == 0)
    {
        pData->mMapSize = st.st_size;
    }

    FILE* hFile = crt::fopen(gGameSession.GetScriptName(), "rb");
    if (hFile)
    {
        pData->mScriptSum = FileByteSum(hFile);
        crt::fclose(hFile);
    }

    hFile = crt::fopen("data\\nyc.gci", "rb");
    if (hFile)
    {
        pData->mGciSum = FileByteSum(hFile);
        crt::fclose(hFile);
    }

    if (bIsFrench)
    {
        pData->mbFrench = 1;
    }
    else
    {
        pData->mbFrench = 0;
    }
    pData->mPlayerIdx = sNetworkPlayerIdx;
}

MATCH_FUNC(0x4DB410)
EXPORT void __stdcall FatalErrorMsg(const char_type* pMsg)
{
    Shutdown_4DA740();
    GBH_Graphis_DMA_Video_Free();
    ErrorMsgBox(pMsg);
    DestroyWindow(gHwnd);
    exit(1);
}

// Quits with an error when another player's sync check data differs from ours
MATCH_FUNC(0x4DB440)
EXPORT void __stdcall CompareRemotePlayers(u8* pLocalSyncData, u8* pRemoteSyncData)
{
    SyncCheckData* pLocal = reinterpret_cast<SyncCheckData*>(pLocalSyncData);
    SyncCheckData* pRemote = reinterpret_cast<SyncCheckData*>(pRemoteSyncData);
    char_type msg[260];
    char_type name[260];
    wchar_t wname[260];

    gNetPlay.GetPlayerName(wname, pRemote->mPlayerIdx);
    WideCharToMultiByte(CP_ACP, 0, wname, -1, name, sizeof(name), NULL, NULL);

    if (pLocal->mbFrench != pRemote->mbFrench)
    {
        sprintf(msg, "Player %s: Language version is different", name);
        FatalErrorMsg(msg);
    }
    if (pLocal->mFlags != pRemote->mFlags)
    {
        sprintf(msg, "Player %s: Debug flags are different", name);
        FatalErrorMsg(msg);
    }
    if (pLocal->mMapSize != pRemote->mMapSize)
    {
        sprintf(msg, "Player %s: Level file '%s' is different", name, gGameSession.GetMapName());
        FatalErrorMsg(msg);
    }
    if (pLocal->mScriptSum != pRemote->mScriptSum)
    {
        sprintf(msg, "Player %s: Script file '%s' is different", name, gGameSession.GetScriptName());
        FatalErrorMsg(msg);
    }
    if (pLocal->mExeA != pRemote->mExeA || pLocal->mExeB != pRemote->mExeB)
    {
        sprintf(msg, "Player %s: Game file 'GTA2.EXE' is different", name);
        FatalErrorMsg(msg);
    }
    if (pLocal->mGciSum != pRemote->mGciSum)
    {
        // The original passes the script name too, which the format doesn't use
        sprintf(msg, "Player %s: Car handling file 'nyc.gci' is different", name, gGameSession.GetScriptName());
        FatalErrorMsg(msg);
    }
}

MATCH_FUNC(0x4DACB0)
EXPORT void Net_Send_Our_Inputs()
{
    if (gNetPlay.GetPlayerCount() > 1)
    {
        if (bDo_sync_check)
        {
            // Send the inputs plus an rng value the other players check against
            s16 max = 100;
            sCurrentNetInputs.mInputs[sNetworkPlayerIdx].mRng = gRng.get_int(max);
            sInputSendData.mData = &sCurrentNetInputs.mInputs[sNetworkPlayerIdx];
            sInputSendData.mLen = sizeof(Network_InputData);
        }
        else
        {
            sInputSendData.mData = &sCurrentNetInputs.mInputs[sNetworkPlayerIdx];
            sInputSendData.mLen = sizeof(u32);
        }
        gNetPlay.SendToAll(&sInputSendData);
    }
}

MATCH_FUNC(0x4DAD50)
EXPORT void Net_Set_Local_Player_Inputs()
{
    for (u32 player_idx = 0; player_idx < MAX_PLAYERS; player_idx++)
    {
        Player* pPlayer = gGame_0x40->GetPlayer(player_idx);
        if (pPlayer && pPlayer->GetInUse())
        {
            pPlayer->SetInputs(sPrevNetInputs.mInputs[player_idx].mInputs);
            sPrevNetInputs.mInputs[player_idx].mInputs &= ~0x3FF000;
        }
    }
}

MATCH_FUNC(0x4DADA0)
EXPORT void ManageNetworkTimeAndHudPager()
{
    LOCAL_STATIC(s32, sNetTimerBlinkCounter, 0x6F5B74);
    LOCAL_STATIC(s32, sNetTimerSeconds, 0x6F5944);
    if (gNetTimeLimitEnabled)
    {
        if (++sFramesOfCurrSecond >= 30) // If a second has passed
        {
            sFramesOfCurrSecond = 0;
            if (--sNetTimeSecondsOfCurrMin < 0) // If a minute has passed
            {
                sNetTimeSecondsOfCurrMin = 59;
                if (--gNetTimeLimit < 0) // If time is out
                {
                    gNetTimeLimitEnabled = 0;
                    gHud_2B00->mMessage.ShowMessage(gText_0x14->Find("g_over"), hud_message_priority::important_3);
                    gGame_0x40->ExitGameNoBonus(2, GameExitType::MultiplayerExit_5);
                }
            }
        }

        // Now do HUD Pager stuff showing the remaining time

        s32 seconds = sNetTimeSecondsOfCurrMin;
        s32 minutes = gNetTimeLimit;
        s32 rem = minutes % 5;
        bool bShow = false;
        if (minutes == 0 || (rem == 4 && seconds >= 50) || (rem == 0 && seconds == 0) ||
            (minutes == gGameSession.GetTimeLimit() && seconds == 0) ||
            (minutes == gGameSession.GetTimeLimit() - 1 && seconds >= 50))
        {
            // bShow is set in both arms (not once before the if): with a single store, VC6 sees bl == 1 at the
            // blink test and uses "test dl, bl" there instead of "test dl, 1"
            if (!sNetTimerFlashing)
            {
                bShow = true;
                sNetTimerBlinkCounter = 59;
                sNetTimerFlashing = 1;
            }
            else
            {
                bShow = true;
            }
        }
        if (!bShow)
        {
            sNetTimerFlashing = 0;
            sNetTimerBlinkCounter = 0;
        }
        else
        {
            if (sNetTimerBlinkCounter > 0)
            {
                if (((sNetTimerBlinkCounter / 5) & 1) == 0)
                {
                    bShow = false;
                }
                sNetTimerBlinkCounter--;
            }

            if (bShow)
            {
                sNetTimerSeconds = minutes * 60 + seconds;
                if (sHudTimerIdx == -1)
                {
                    sHudTimerIdx = gHud_2B00->mPagers.CreateTimer(sNetTimerSeconds);
                }
                return;
            }
        }

        if (sHudTimerIdx != -1)
        {
            gHud_2B00->mPagers.ClearPager(sHudTimerIdx);
            sHudTimerIdx = -1;
        }
    }
}

MATCH_FUNC(0x4DAF30)
EXPORT void __stdcall do_network_and_local_inputs()
{
    if (bStartNetworkGame)
    {
        ManageNetworkTimeAndHudPager();
        gTagGame.ExecuteGamemodeTick();

        if (!gGame_0x40->UpdateExitTimer())
        {
            if (sNetDisconnectPlayersBits)
            {
                u32 player_idx = 0;
                do
                {
                    if ((sNetDisconnectPlayersBits & 1) != 0)
                    {
                        Player* pPlayer = gGame_0x40->GetPlayer(player_idx);
                        if (pPlayer && pPlayer->GetInUse())
                        {
                            gNetPlay.DeletePlayerFromGroup(player_idx);
                            ShowPlayerLeftMessage(player_idx);
                            if (!player_idx || player_idx == sNetworkPlayerIdx)
                            {
                                sPlayerQuit = true;
                            }
                        }
                    }
                    player_idx++;
                    sNetDisconnectPlayersBits >>= 1;
                } while (sNetDisconnectPlayersBits);
            }

            sCurrentNetInputs.mInputs[sNetworkPlayerIdx].mInputs = gInputRecorder.get_input_bits();
            Net_Send_Our_Inputs();
            Draw_4DA7B0();
            Net_Receive_Players_Inputs();
            memcpy(&sPrevNetInputs, &sCurrentNetInputs, sizeof(sPrevNetInputs));
        }
        else
        {
            gNetPlay.SendPlayerIsQuitting();
        }

        if (sbRecordStartTime)
        {
            sMatchStartTime = timeGetTime();
            sbRecordStartTime = false;
        }
        if (sPlayerQuit)
        {
            gGame_0x40->ExitGameNoBonus(0, GameExitType::PlayerQuit_2);
        }
        else
        {
            Net_Set_Local_Player_Inputs();
        }
        ++sNetworkFrameCounter;
    }
    else
    {
        const u32 inputs = gInputRecorder.get_input_bits();
        gGame_0x40->mLocalPlayer->SetInputs(inputs);
    }
}

MATCH_FUNC(0x4DA850)
EXPORT bool RunGameFrame()
{
    bool exit_requested = false;
    bool drew_frame = false;
    bool updated_logic = false;
    DWORD Time = timeGetTime();

    if (bStartNetworkGame)
    {
        if (Time >= sMatchStartTime)
        {
            exit_requested = ExecuteGame_4DA780();
            do_network_and_local_inputs();
            sMatchStartTime = Time + gGame_0x40->GetFrameDurationMs();
        }
    }
    else
    {
        if (!sMaxFrameRate && !sFramesSinceDraw || Time >= sMatchStartTime)
        {
            updated_logic = true;
        }
        if (sFramesSinceDraw >= 8u || !sMinFrameRate && sFramesSinceDraw)
        {
            updated_logic = false;
        }

        if (sbForceGameUpdate || bSkip_draw)
        {
            updated_logic = true;
        }

        if (updated_logic)
        {
            if (!sMinFrameRate)
            {
                sMatchStartTime = Time;
            }
            if (sbInputsPending)
            {
                do_network_and_local_inputs();
                sbInputsPending = false;
            }
            exit_requested = ExecuteGame_4DA780();
            s32 frame_ms = gGame_0x40->GetFrameDurationMs();
            sbForceGameUpdate = false;
            sMatchStartTime += frame_ms;
            ++sFramesSinceDraw;
            sbInputsPending = true;
        }
        else if (sFramesSinceDraw)
        {
            Draw_4DA7B0();
            sFramesSinceDraw = 0;
            drew_frame = true;
        }
        else if (sbInputsPending)
        {
            do_network_and_local_inputs();
            sbInputsPending = false;
        }
    }

    if (gFpsCounter)
    {
        gFpsCounter->UpdateFpsCounters(drew_frame, updated_logic);
    }
    return exit_requested;
}

GUID IID_DirectMusic = {1667997456u, 3197u, 4561u, {149u, 178u, 0u, 32u, 175u, 220u, 116u, 33u}};
GUID IDD_IDirectMusic = {1698042202u, 31533u, 4562u, {186u, 24u, 0u, 0u, 248u, 117u, 172u, 18u}};

MATCH_FUNC(0x4A0650)
EXPORT void __stdcall GetDirectMusicVer()
{
    HMODULE hDMusic;
    IUnknown* pDMusic;

    sDMusicVer = 0;
    hDMusic = LoadLibraryA("DMUSIC.DLL");
    if (hDMusic)
    {
        if (CoCreateInstance(IID_DirectMusic, NULL, CLSCTX_INPROC_SERVER, IDD_IDirectMusic, reinterpret_cast<LPVOID*>(&pDMusic)) >= 0)
        {
            sDMusicVer = 0x601;
            pDMusic->Release();
        }
        FreeLibrary(hDMusic);
    }
}

GUID IID_IDirectDrawSurface4_ = {0x0B2B8630, 0xAD35, 0x11D0, 0x8E, 0xA6, 0x00, 0x60, 0x97, 0x97, 0xEA, 0x5B};

MATCH_FUNC(0x4C4EC0)
EXPORT void __stdcall GetDirectXVersion(u32* pDXVer, u32* osKind)
{
    HMODULE hDInput;
    FARPROC pDirectInputCreateA;
    HMODULE hDDraw;
    HRESULT(__stdcall * pDirectDrawCreate)(GUID*, LPDIRECTDRAW*, IUnknown*);
    HMODULE hDInput2;
    FARPROC pDirectInputCreateA_;
    LPDIRECTDRAW pDDraw;
    LPDIRECTDRAWSURFACE pDSurface;
    LPDIRECTDRAW pDraw2;
    s32 pSurface3;
    LPDIRECTDRAWSURFACE pDSurface4;
    DDSURFACEDESC surfaceDesc;
    struct _OSVERSIONINFOA osVersionInfo;

    pDDraw = NULL;
    pDraw2 = NULL;
    pDSurface = NULL;
    pSurface3 = 0;
    pDSurface4 = NULL;
    osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);

    if (!GetVersionExA(&osVersionInfo))
    {
        *pDXVer = 0;
        *osKind = 0;
        return;
    }

    if (osVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        *osKind = VER_PLATFORM_WIN32_NT;
        if (osVersionInfo.dwMajorVersion < 4)
        {
            *osKind = 0; // less than NT4
            return;
        }
        if (osVersionInfo.dwMajorVersion == 4) // exactly NT4
        {
            *pDXVer = 0x200;
            hDInput = LoadLibraryA("DINPUT.DLL");
            if (!hDInput)
            {
                OutputDebugStringA("Couldn't LoadLibrary DInput\r\n");
                return;
            }

            pDirectInputCreateA = GetProcAddress(hDInput, "DirectInputCreateA");
            FreeLibrary(hDInput);
            if (!pDirectInputCreateA)
            {
                OutputDebugStringA("Couldn't GetProcAddress DInputCreate\r\n");
                return;
            }
            *pDXVer = 0x300;
            return;
        }
    }
    else
    {
        *osKind = VER_PLATFORM_WIN32_WINDOWS;
    }

    hDDraw = LoadLibraryA("DDRAW.DLL");
    if (!hDDraw)
    {
        *pDXVer = 0;
        *osKind = 0;
        FreeLibrary(hDDraw);
        return;
    }

    pDirectDrawCreate = reinterpret_cast<HRESULT(__stdcall*)(GUID*, LPDIRECTDRAW*, IUnknown*)>(GetProcAddress(hDDraw, "DirectDrawCreate"));
    if (!pDirectDrawCreate)
    {
        *pDXVer = 0;
        *osKind = 0;
        FreeLibrary(hDDraw);
        OutputDebugStringA("Couldn't LoadLibrary DDraw\r\n");
        return;
    }

    if (pDirectDrawCreate(NULL, &pDDraw, NULL) < 0)
    {
        *pDXVer = 0;
        *osKind = 0;
        FreeLibrary(hDDraw);
        OutputDebugStringA("Couldn't create DDraw\r\n");
        return;
    }

    *pDXVer = 0x100;
    if (pDDraw->QueryInterface(IID_IDirectDraw2, reinterpret_cast<LPVOID*>(&pDraw2)) < 0)
    {
        pDDraw->Release();
        FreeLibrary(hDDraw);
        OutputDebugStringA("Couldn't QI DDraw2\r\n");
        return;
    }

    pDraw2->Release();
    *pDXVer = 0x200;
    hDInput2 = LoadLibraryA("DINPUT.DLL");
    if (!hDInput2)
    {
        OutputDebugStringA("Couldn't LoadLibrary DInput\r\n");
        pDDraw->Release();
        FreeLibrary(hDDraw);
        return;
    }

    pDirectInputCreateA_ = GetProcAddress(hDInput2, "DirectInputCreateA");
    FreeLibrary(hDInput2);
    if (!pDirectInputCreateA_)
    {
        FreeLibrary(hDDraw);
        pDDraw->Release();
        OutputDebugStringA("Couldn't GetProcAddress DInputCreate\r\n");
        return;
    }

    *pDXVer = 0x300;
    memset(&surfaceDesc, 0, sizeof(surfaceDesc));
    surfaceDesc.dwSize = sizeof(surfaceDesc);
    surfaceDesc.dwFlags = DDSD_CAPS;
    surfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    if (pDDraw->SetCooperativeLevel(NULL, DDSCL_NORMAL) < 0)
    {
        pDDraw->Release();
        FreeLibrary(hDDraw);
        *pDXVer = 0;
        OutputDebugStringA("Couldn't Set coop level\r\n");
        return;
    }

    if (pDDraw->CreateSurface(&surfaceDesc, &pDSurface, NULL) < 0)
    {
        pDDraw->Release();
        FreeLibrary(hDDraw);
        *pDXVer = 0;
        OutputDebugStringA("Couldn't CreateSurface\r\n");
        return;
    }

    if (pDSurface->QueryInterface(IID_IDirectDrawSurface3, reinterpret_cast<LPVOID*>(&pSurface3)) < 0)
    {
        pDDraw->Release();
        FreeLibrary(hDDraw);
        return;
    }

    *pDXVer = 0x500;

    if (pDSurface->QueryInterface(IID_IDirectDrawSurface4_, reinterpret_cast<LPVOID*>(&pDSurface4)) < 0)
    {
        pDDraw->Release();
        FreeLibrary(hDDraw);
        return;
    }

    *pDXVer = 0x600;
    pDSurface->Release();
    pDDraw->Release();
    FreeLibrary(hDDraw);
    GetDirectMusicVer();
    if (sDMusicVer == 0x601)
    {
        *pDXVer = 0x601;
    }
}

const char_type* kDefaultDllNames_626A00[2] = {"d3ddll.dll", "dmavideo.dll"};

MATCH_FUNC(0x5D90E0)
EXPORT void __stdcall Video_Render_Inits()
{
    LOCAL_STATIC(s32, sRenderdevice, 0x706998);
    sRenderdevice = gRegistry.Get_Screen_Setting("renderdevice", 1);
    sVideodevice = gRegistry.Get_Screen_Setting("videodevice", 1);
    strcpy(sRenderDllName, kDefaultDllNames_626A00[0]);
    strcpy(sVideoDllName, kDefaultDllNames_626A00[1]);
    gRegistry.Get_Screen_String("rendername", reinterpret_cast<BYTE*>(sRenderDllName), 0xFFu);
    gRegistry.Get_Screen_String("videoname", reinterpret_cast<BYTE*>(sVideoDllName), 0xFFu);

    if (strcmp(sRenderDllName, "softdll.dll") == 0)
    {
        gBufferMode = 0;
    }
    else if (strcmp(sRenderDllName, "3dfx.dll") == 0)
    {
        gBufferMode = 2;
    }
    else
    {
        gBufferMode = 1;
    }

    Init_FrameRateLightingAndExplodingScores();
    ReadScreenSettings();
}

MATCH_FUNC(0x5D9280)
EXPORT bool IsFullScreen_5D9280()
{
    return sStartMode == 1;
}

MATCH_FUNC(0x5D8E70)
EXPORT void __stdcall UpdateWinXY()
{
    if (!IsFullScreen_5D9280())
    {
        RECT winRec;
        GetWindowRect(gHwnd, &winRec);
        sWindowX = winRec.left;
        sWindowY = winRec.top;
    }
}

MATCH_FUNC(0x5D7CA0)
EXPORT void __stdcall j_gbh_init()
{
    pgbh_Init(0);
}

MATCH_FUNC(0x4DA700)
EXPORT void __stdcall CleanUpInputAndOthers()
{
    gInputRecorder.Shutdown();
    if (gGame_0x40)
    {
        GTA2_DELETE_AND_NULL(gGame_0x40);
    }
    gRoot_sound.Set3DSound(0);
}

MATCH_FUNC(0x4DA390)
EXPORT s32 __stdcall CountTokens(char_type* pStr)
{
    char_type* pCur = pStr;
    s32 result = 0;
    if (pStr == NULL)
    {
        return 0;
    }
    else
    {
        if (*pStr)
        {
            do
            {
                while (true)
                {
                    char_type ch = *pCur;
                    if (ch != ' ' && ch != '\n' && ch != '\r' && ch != '\t')
                    {
                        break;
                    }
                    if (ch == 0)
                    {
                        break;
                    }
                    ++pCur;
                }
                char_type ch2 = *pCur;
                if (!*pCur)
                {
                    break;
                }
                ++result;
                for (; ch2 != ' '; ch2 = *++pCur)
                {
                    if (ch2 == '\n' || ch2 == '\r' || ch2 == '\t' || ch2 == 0)
                    {
                        break;
                    }
                }
            } while (*pCur);
        }
    }
    return result;
}

MATCH_FUNC(0x4DA3F0)
EXPORT char_type* __stdcall SkipToNextToken(char_type* pStr)
{
    char_type* result = pStr;
    for (char_type i = *pStr; i; i = *++result)
    {
        if (i == ' ')
        {
            break;
        }
        if (i == '\n')
        {
            break;
        }
        if (i == '\r')
        {
            break;
        }
        if (i == '\t')
        {
            break;
        }
    }
    for (char_type j = *result; j; j = *++result)
    {
        if (j != ' ' && j != '\n' && j != '\r' && j != '\t')
        {
            break;
        }
    }
    return result;
}

MATCH_FUNC(0x4DA320)
EXPORT void __stdcall ParseCommandLine(char_type* pCommandLine)
{
    char_type* pIter = pCommandLine;
    s32 len = CountTokens(pCommandLine);
    _strupr(pCommandLine);
    while (len > 0)
    {
        if (*pIter == '-' || *pIter == '/')
        {
            const char_type cmd_char = *(++pIter);
            switch (cmd_char)
            {
                case 'R':
                    bPlay_replay = true;
                    break;

                case 'Q':
                    bConstant_replay_save = true;
                    break;

                case 'N':
                    bStartNetworkGame = 1;
                    break;

                default:
                    break;
            }
        }

        pIter = SkipToNextToken(pIter);
        --len;
    }
}

// The original passes an uninitialised lParam to PostMessageA, keep it for matching
#pragma warning(push)
#pragma warning(disable : 4700)
MATCH_FUNC(0x5E4DE0)
EXPORT void Start_GTA2Manager()
{
    ShowWindow(gHwnd, SW_SHOWMINNOACTIVE);
    HANDLE hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "GBH_BOB_MUTEX");
    if (hMutex)
    {
        if (!CloseHandle(hMutex))
        {
            FatalError_4A38C0(Gta2Error::Gta2ManagerLaunchError, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 118);
        }

        HWND hwndGta2Manager = FindWindowExA(NULL, NULL, NULL, "GTA2 Manager");
        if (hwndGta2Manager)
        {
            PostMessageA(gHwnd, WM_ACTIVATE, 0, reinterpret_cast<LPARAM>(hwndGta2Manager));
            if (!SetForegroundWindow(hwndGta2Manager))
            {
                FatalError_4A38C0(Gta2Error::Gta2ManagerLaunchError, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 125);
            }
        }
    }
    else
    {
        LPARAM lParam;
        PostMessageA(gHwnd, WM_ACTIVATE, 0, lParam);
        if (reinterpret_cast<u32>(ShellExecuteA(NULL, NULL, "GTA2 Manager.EXE", NULL, gWorkingDir, SW_SHOWNORMAL)) <= 0x20)
        {
            FatalError_4A38C0(Gta2Error::Gta2ManagerLaunchError, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 136);
        }
    }
}
#pragma warning(pop)

MATCH_FUNC(0x5E4EC0)
EXPORT void __stdcall ErrorMsgBox(LPCSTR lpText)
{
    MessageBoxA(gHwnd, lpText, "Error!", MB_OK);
}

MATCH_FUNC(0x5D9230)
EXPORT void __stdcall SetStartMode(s32 startMode)
{
    sStartMode = startMode;
    gRegistry.Set_Screen_Setting("start_mode", startMode);
}

MATCH_FUNC(0x498D10)
EXPORT void Input_Read()
{
    if (gInputDevices)
    {
        gInputDevices->flush_keyboard_and_gamepad();
    }
}

MATCH_FUNC(0x5D9250)
EXPORT void __stdcall ToggleStartMode()
{
    sStartMode = (sStartMode != 1);
    gRegistry.Set_Screen_Setting("start_mode", sStartMode);
}

MATCH_FUNC(0x5E4EE0)
EXPORT LRESULT __stdcall WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    BYTE bSetFocusInput;
    BYTE bKillFocusInput;
    BYTE bActiveInput;
    BYTE bInactiveInput;
    s32 newX;
    s32 newY;
    RECT winRec;

    switch (Msg)
    {

        case WM_WINDOWPOSCHANGED:
        {
            if (gFrontend)
            {
                Bink::ResetBufferOffset();
            }
            break;
        }

        case WM_SETFOCUS:
            gRoot_sound.Reacquire();
            gRoot_sound.SetCDVol(gRegistry.Get_Sound_Setting("CDVol", 127));
            gRoot_sound.SetSfxVol(gRegistry.Get_Sound_Setting("SFXVol", 127));

            if (bDoFrontEnd)
            {
                gRoot_sound.Set3DSound(0);
            }
            else
            {
                gRoot_sound.Set3DSound(gRegistry.Sound_Setting_Exists("do_3d_sound"));
                gRegistry.Clear_Or_Delete_Sound_Setting("do_3d_sound", gRoot_sound.Get3DSound());
            }

            if (!bDestroyed)
            {
                bSetFocusInput = 1;
                Frontend::SetInputEnabled(&bSetFocusInput);
                if (!bDoFrontEnd)
                {
                    Input_Read();
                }

                if (gVidSys && !Bink::IsUsingDDBuffer())
                {
                    SetVideoModeFromSettings();
                    OnVideoModeChanged();
                    sbVideoModeResetPending = false;
                }

                if (gGame_0x40)
                {
                    gGame_0x40->ReinitAfterVideoModeChange();
                }

                SetSavedGamma();
            }
            break;

        case WM_KILLFOCUS:
            {
                bKillFocusInput = 0;
                Frontend::SetInputEnabled(&bKillFocusInput);
                Input_ReleaseMouse();
                gRoot_sound.Set3DSound(0);
                gRoot_sound.Release();

                if (gFrontend && Bink::IsDDBufferOpen())
                {
                    Bink::CloseSlot1();
                    Bink::CloseSlot2();
                    gFrontend->ChangeMenuPage(MENUPAGE_START_MENU);
                }

                if (gVidSys)
                {
                    if (!Vid_FindDevice_5D9290())
                    {
                        pVid_CloseScreen(gVidSys);
                        sbVideoModeResetPending = true;
                        ShowWindow(gHwnd, SW_SHOWMINNOACTIVE);
                    }
                }
                break;
            }

        case WM_ACTIVATE:
            switch (static_cast<u8>(wParam))
            {
                case WA_ACTIVE:
                case WA_CLICKACTIVE:
                {
                    bActiveInput = 1;
                    Frontend::SetInputEnabled(&bActiveInput);
                    Input_MouseAcquire();
                }
                break;

                case WA_INACTIVE:
                {
                    bInactiveInput = 0;
                    Frontend::SetInputEnabled(&bInactiveInput);
                    Input_ReleaseMouse();
                }
                break;
            }
            break;

        case WM_SIZE:
            switch (wParam)
            {
                case SIZE_RESTORED:
                    sMinimizedState = 0;
                    gRoot_sound.Reacquire();
                    break;

                case SIZE_MINIMIZED:
                    sMinimizedState = 2;
                    gRoot_sound.Release();
                    break;

                case SIZE_MAXIMIZED:
                    sMinimizedState = 0;
                    gRoot_sound.Reacquire();
                    break;
            }
            break;

        case WM_DESTROY:
            if (bStartNetworkGame)
            {
                gNetPlay.Disconnect();
            }

            ReleaseMutex(sMutex);
            CloseHandle(sMutex);
            sMutex = NULL;

            Shutdown_4DA740();
            GBH_Graphis_DMA_Video_Free();
            PostQuitMessage(0);
            break;

        case WM_WINDOWPOSCHANGING:
        {
            WINDOWPOS* pPos = reinterpret_cast<WINDOWPOS*>(lParam);
            if (gFrontend && (pPos->flags & SWP_NOMOVE) == 0)
            {
                newX = pPos->x;
                newY = pPos->y;
                Bink::CheckWindowPos(&newX, &newY);
                pPos->x = newX;
                pPos->y = newY;
            }
            break;
        }

        case WM_SYSKEYDOWN:
            switch (wParam)
            {
                case VK_RETURN:
                    if (IsWindowedModeAvailable())
                    {
                        UpdateWinXY();
                        // The previous key state. The value is 1 if the key is down before the message is sent, or it is 0 if the key is up.
                        if ((lParam & 0x20000000) != 0)
                        {
                            ToggleStartMode();
                            SetVideoModeFromSettings();
                            OnVideoModeChanged();
                        }
                    }
                    break;

                case VK_MENU:
                case VK_F10:
                case VK_LMENU:
                    return 0;

                default:
                    break;
            }
            break;

        case WM_SYSCOMMAND:
            switch (wParam & 0xFFF0)
            {
                case SC_MONITORPOWER:
                case SC_SCREENSAVE:
                    return 0;

                case SC_MAXIMIZE:
                    UpdateWinXY();
                    if (sMinimizedState != 2)
                    {
                        SetStartMode(1u);
                        SetVideoModeFromSettings();
                        OnVideoModeChanged();
                    }
                    break;

                case SC_MINIMIZE:
                    UpdateWinXY();
                    break;

                default:
                    break;
            }
            break;

        case WM_SIZING:
        {
            RECT* pDragRect = reinterpret_cast<RECT*>(lParam);
            GetWindowRect(gHwnd, &winRec);
            pDragRect->left = winRec.left;
            pDragRect->top = winRec.top;
            pDragRect->bottom = winRec.bottom;
            pDragRect->right = winRec.right;
            return 0;
        }

        default:
            break;
    }

    if (gVidSys)
    {
        pVid_WindowProc(gVidSys, hWnd, Msg, wParam, lParam);
    }

    return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

MATCH_FUNC(0x5E53F0)
s32 __stdcall WinMain_5E53F0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    gHInstance = hInstance;
    if (CoInitialize(NULL) < 0)
    {
        return 0;
    }

    if (OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "GBH_COOP_MUTEX"))
    {
        return 0;
    }

    sMutex = CreateMutexA(NULL, FALSE, "GBH_COOP_MUTEX");
    GetGTA2Version(&gGTA2VersionMajor, &gGTA2VersionMinor);

    u32 dxVer;
    u32 osKind;
    GetDirectXVersion(&dxVer, &osKind);

    if (dxVer < 0x601)
    {
        ErrorMsgBox("ERROR: Incorrect version of DirectX");
        ReleaseMutex(sMutex);
        CloseHandle(sMutex);
        return -1;
    }

    gDebug.Init();
    gGameSession.LoadDebugSettings();
    bDoFrontEnd = !bSkip_frontend;

    WNDCLASSA WndClass;
    WndClass.style = CS_VREDRAW | CS_HREDRAW;
    WndClass.lpfnWndProc = WindowProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(0x65));
    WndClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    WndClass.hbrBackground = NULL;
    WndClass.lpszMenuName = "WinMain";
    WndClass.lpszClassName = "WinMain";
    RegisterClassA(&WndClass);

    Video_Render_Inits();

    _getcwd(gWorkingDir, 256);

    ParseCommandLine(lpCmdLine);
    if (!Start_NetworkGame(hInstance)) // start NetUI if multiplayer. Stays here while players don't click "start", "cancel" etc
    {
        ReleaseMutex(sMutex);
        CloseHandle(sMutex);
        return 0;
    }

    // Starting game...

    sprintf(gTmpBuffer, "GTA2");
    gHwnd = CreateWindowExA(0, "WinMain", gTmpBuffer, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, hInstance, NULL);
    if (!gHwnd)
    {
        return 0;
    }

    ShowWindow(gHwnd, nShowCmd);
    UpdateWindow(gHwnd);
    RECT clientRec;
    GetClientRect(gHwnd, &clientRec);
    RECT windowRec;
    GetWindowRect(gHwnd, &windowRec);

    s32 window_w = window_width + (windowRec.right - windowRec.left) - (clientRec.right - clientRec.left);
    s32 window_h = window_height + (windowRec.bottom - windowRec.top) - (clientRec.bottom - clientRec.top);

    if (bDo_corner_window)
    {
        sWindowX = 0;
        sWindowY = 0;
    }
    else
    {
        sWindowX = static_cast<s32>(GetSystemMetrics(SM_CXSCREEN) - window_w) / 2;
        sWindowY = static_cast<s32>(GetSystemMetrics(SM_CYSCREEN) - window_h) / 2;
    }

    if (!SetWindowPos(gHwnd, NULL, sWindowX, sWindowY, window_w, window_h, 0x314u))
    {
        FatalError_4A38C0(Gta2Error::SetWindowPosFail, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 661);
    }

    Input::DirectInputCreate_4986D0(gHInstance);
    InitVideo();

    ShowWindow(gHwnd, nShowCmd);
    UpdateWindow(gHwnd);
    j_gbh_init();
    Init_keybrd_jolly_and_sound();

    s32 state;
    if (Frontend::intro_bik_exists())
    {
        state = gRegistry.Get_Screen_Setting("do_play_movie", 1) != 1 ? MENUPAGE_START_MENU : MENUPAGE_PLAY_INTRO;
    }
    else
    {
        state = MENUPAGE_START_MENU;
    }

    if (bIsFrench)
    {
        state = MENUPAGE_PARENTAL_CONTROL;
    }

    if (bStartNetworkGame)
    {
        bDoFrontEnd = false;
        gRoot_sound.Set3DSound(gRegistry.Sound_Setting_Exists("do_3d_sound"));
        gRegistry.Clear_Or_Delete_Sound_Setting("do_3d_sound", gRoot_sound.Get3DSound());
    }

    // Game loop

    bool bQuit = false;

    while (true)
    {
        bQuit = false;

        if (bDoFrontEnd)
        {
            Frontend::create();
            gFrontend->ChangeMenuPage(state);
        }
        else
        {
            InitializeGame();
            if (bStartNetworkGame && !gNetPlay.WaitForPlayersSync())
            {
                CoUninitialize();
                return 0;
            }
        }

        UpdateWinXY();
        ApplyChangedScreenSettings();

        while (true)
        {
            MSG msg;
            if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    Input::DInputRelease();
                    return msg.wParam;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            else
            {
                if (!bQuit && sMinimizedState != 2 && !sbVideoModeResetPending)
                {
                    if (bDoFrontEnd)
                    {
                        s32 t = gFrontend->Run();

                        if (t == Quit_1)
                        {
                            bQuit = true;
                            Frontend::destroy();
                            DestroyWindow(gHwnd);
                            continue; // go to PeekMessageA
                        }
                        else if (t == Start_Game_3)
                        {
                            Frontend::destroy();
                            bDoFrontEnd = false;
                            break; // go to the beginning
                        }
                        else if (t == Start_Replay_4)
                        {
                            Frontend::destroy();
                            bDoFrontEnd = false;
                            sbReplayMode = true;
                            break; // go to the beginning
                        }
                        else
                        {
                            continue; // go to PeekMessageA
                        }
                    }
                    bQuit = RunGameFrame();

                    if (bQuit)
                    {
                        if (bSkip_frontend)
                        {
                            DestroyWindow(gHwnd);
                        }
                        else
                        {
                            if (bStartNetworkGame)
                            {
                                switch (gGame_0x40->GetExitType())
                                {
                                    case GameExitType::CloseGameImmediately_1:
                                        DestroyWindow(gHwnd);
                                        break;

                                    default:
                                        state = MENUPAGE_MULTIPLAYER_RESULTS;
                                        CleanUpInputAndOthers();
                                        bDoFrontEnd = true;
                                        break;
                                }
                                break; // go to the beginning
                            }
                            else
                            {
                                switch (gGame_0x40->GetExitType())
                                {
                                    case GameExitType::CloseGameImmediately_1:
                                        DestroyWindow(gHwnd);
                                        break;

                                    case GameExitType::PlayerQuit_2:
                                        gGameSession.StoreCrimeStats(gGame_0x40->mLocalPlayer);
                                        gPlyDat->UpdateStageScore(gGame_0x40->mLocalPlayer);
                                        gPlyDat->UpdateHiScores();

                                        state = gGameSession.IsBonusStage() != 0 ? MENUPAGE_BONUS_AREA : MENUPAGE_RESULTS_PLAYER_QUIT;
                                        CleanUpInputAndOthers();
                                        bDoFrontEnd = true;
                                        break;

                                    case GameExitType::GameOverRIP_3:
                                        gGameSession.StoreCrimeStats(gGame_0x40->mLocalPlayer);
                                        gPlyDat->UpdateStageScore(gGame_0x40->mLocalPlayer);
                                        gPlyDat->UpdateHiScores();
                                        state = gGameSession.IsBonusStage() != 0 ? MENUPAGE_BONUS_AREA : MENUPAGE_DEAD;
                                        CleanUpInputAndOthers();
                                        bDoFrontEnd = true;
                                        break;

                                    case GameExitType::AreaCompleted_4:
                                        gGameSession.StoreCrimeStats(gGame_0x40->mLocalPlayer);
                                        gPlyDat->UpdateStageScore(gGame_0x40->mLocalPlayer);
                                        gPlyDat->UpdateHiScores();
                                        state = gGameSession.IsBonusStage() != 0 ? MENUPAGE_BONUS_AREA : MENUPAGE_AREA_COMPLETE;
                                        CleanUpInputAndOthers();
                                        bDoFrontEnd = true;
                                        break;

                                    case GameExitType::MultiplayerExit_5:
                                        state = MENUPAGE_MULTIPLAYER_RESULTS;
                                        CleanUpInputAndOthers();
                                        bDoFrontEnd = true;
                                        break;

                                    case GameExitType::ReplayExit_6:
                                        state = MENUPAGE_START_MENU;
                                        CleanUpInputAndOthers();
                                        bDoFrontEnd = true;
                                        break;

                                    default:
                                        break;
                                }
                                break; // go to the beginning
                            }
                        }
                    }
                }
            }
        }

    }
    return 0;
}

MATCH_FUNC(0x4DA740)
EXPORT void __stdcall Shutdown_4DA740()
{
    if (!bDestroyed)
    {
        bDestroyed = 1;
        KeyboardLayout::destroy();
        PlyDat::Destroy();
        CleanUpInputAndOthers();
        Frontend::destroy();

        if (!bSkip_audio)
        {
            gRoot_sound.DeinitializeAudio();
        }
    }
}

MATCH_FUNC(0x5E5A30)
EXPORT bool __stdcall Start_NetworkGame(HINSTANCE hInstance)
{
    bool bRet = true;
    gGameSession.init();
    if (bStartNetworkGame)
    {
        HKEY hKey;
        if (!gRegistry.CreateNetworkRoot(&hKey))
        {
            bRet = false;
        }

        if (gRegistry.Get_Binary_Size(hKey, "UseProtocol") != sizeof(GUID))
        {
            RegCloseKey(hKey);
            if (!gNetPlay.SetProtoAndConnection(NULL, NULL))
            {
                return false;
            }
        }
        else
        {
            GUID protocolGuid;
            if (!gRegistry.Get_Binary(hKey, "UseProtocol", sizeof(GUID), reinterpret_cast<LPBYTE>(&protocolGuid)))
            {
                RegCloseKey(hKey);
                return false;
            }

            Network_Buffer connection;
            connection.mLen = gRegistry.Get_Binary_Size(hKey, "UseConnection");
            connection.mData = operator new(connection.mLen);
            if (!gRegistry.Get_Binary(hKey, "UseConnection", connection.mLen, static_cast<LPBYTE>(connection.mData)))
            {
                operator delete(connection.mData);
                RegCloseKey(hKey);
                return false;
            }
            RegCloseKey(hKey);

            if (!gNetPlay.SetProtoAndConnection(&protocolGuid, &connection))
            {
                operator delete(connection.mData);
                return false;
            }

            if (gNetPlay.mbModem)
            {
                u16 len = gRegistry.Get_Binary_Size(hKey, "ModemNumber");
                if (len >= 128)
                {
                    return false;
                }
                wchar_t modemNumber[128];
                gRegistry.Get_Binary(hKey, "ModemNumber", len, reinterpret_cast<LPBYTE>(modemNumber));
                gNetPlay.mModemNum = modemNumber[0] != 0;
            }
        }

        NetUi networkUi;
        if (!gNetPlay.InitializeConnection())
        {
            return false;
        }

        if (!networkUi.ShowNetworkUiBlocking(hInstance))
        {
            return false;
        }

        networkUi.CopyGameSettings(&gNetworkGameSettings);

        char_type path[MAX_PATH];
        wsprintfA(path, "data\\%s", networkUi.GetMapName());
        gGameSession.SetMapName(path);
        wsprintfA(path, "data\\%s", networkUi.GetMapStyName());
        gGameSession.SetStyleName(path);
        wsprintfA(path, "data\\%s", networkUi.GetMapScrName());
        gGameSession.SetScriptName(path);

        gGameSession.SetMultiplayerParams(gNetworkGameSettings.mGameType,
                                                           gNetworkGameSettings.mFragLimit,
                                                           gNetPlay.GetPlayerIdx(),
                                                           gNetPlay.CountActivePlayers(),
                                                           gNetworkGameSettings.mGameTimeLimit);
    }
    return bRet;
}

#pragma comment(lib, "Version.lib")

MATCH_FUNC(0x5E5D60)
EXPORT void __stdcall GetGTA2Version(s32* pVerMinor, s32* pVerMajor)
{
    DWORD dwHandle;
    DWORD dwSize = GetFileVersionInfoSizeA("GTA2.EXE", &dwHandle);
    BYTE* pAlloc = new BYTE[dwSize];

    VS_FIXEDFILEINFO* pFileInfo;
    u32 puLen;
    if (pAlloc && GetFileVersionInfoA("GTA2.EXE", 0, dwSize, pAlloc) &&
        VerQueryValueA(pAlloc, "\\", reinterpret_cast<LPVOID*>(&pFileInfo), &puLen))
    {
        *pVerMinor = (pFileInfo->dwProductVersionMS >> 16);
        *pVerMajor = pFileInfo->dwProductVersionMS & 0xFFFF;
    }
    delete[] pAlloc;
}