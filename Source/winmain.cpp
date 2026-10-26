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

DEFINE_GLOBAL(u8, gNetDisconnectPlayersBits_6F56B8, 0x6F56B8); // TODO: move
DEFINE_GLOBAL(u32, gNetworkPlayerIdx_6F56C8, 0x6F56C8); // TODO: move
DEFINE_GLOBAL(u32, gPlayerQuit_6F5AEC, 0x6F5AEC); // TODO: move
DEFINE_GLOBAL(u32, gMatchStartTime_6F5A28, 0x6F5A28); // TODO: move
DEFINE_GLOBAL(u32, gNetworkFrameCounter_6F5868, 0x6F5868); // TODO: move
DEFINE_GLOBAL(u8, bRecordStartTime_6F593C, 0x6F593C); // TODO: move
DEFINE_GLOBAL(Network_PlayerInputs, gCurrentNetInputs_6F57D8, 0x6F57D8); // TODO: move
DEFINE_GLOBAL(Network_Buffer, gInputSendData_6F5B18, 0x6F5B18); // TODO: move
DEFINE_GLOBAL(Network_PlayerInputs, gPrevNetInputs_6F5B28, 0x6F5B28); // TODO: move
DEFINE_GLOBAL(ReceivedPacket*, gpInputBuffer_6F58C0, 0x6F58C0); // TODO: move
DEFINE_GLOBAL(u32, gCurrentInputsBufferSize_6F58C4, 0x6F58C4); // TODO: move
DEFINE_GLOBAL_ARRAY(u8, gSyncCheckData_6F58E0, 0x20, 0x6F58E0); // TODO: move
DEFINE_GLOBAL(u32, gTotalNetworkTime_6F5980, 0x6F5980); // TODO: move
DEFINE_GLOBAL(s32, gNetTimeLimit_6F573C, 0x6F573C); // TODO: move
DEFINE_GLOBAL(s32, gHudTimerIdx_6F5860, 0x6F5860); // TODO: move
EXTERN_GLOBAL(s32, gNetTimeLimitEnabled_6F58A4); // TODO: move
DEFINE_GLOBAL(s32, gNetTimeSecondsOfCurrMin_6F58A0, 0x6F58A0); // TODO: move
DEFINE_GLOBAL_ARRAY(u8, byte_6F5AC8, 14, 0x6F5AC8); // TODO: move
DEFINE_GLOBAL(u32, dword_6F5B00, 0x6F5B00); // TODO: move
DEFINE_GLOBAL(u32, dword_6F580C, 0x6F580C); // TODO: move
DEFINE_GLOBAL(u32, dword_6F5AC0, 0x6F5AC0); // TODO: move
DEFINE_GLOBAL(s32, gFramesOfCurrSecond_6F5858, 0x6F5858); // TODO: move
DEFINE_GLOBAL(s32, gNetTimerBlinkCounter_6F5B74, 0x6F5B74);
DEFINE_GLOBAL(s32, gNetTimerSeconds_6F5944, 0x6F5944);
DEFINE_GLOBAL(u8, gNetTimerFlashing_6F59C0, 0x6F59C0); // TODO: move
DEFINE_GLOBAL(NetworkGameSettings, gNetworkGameSettings_707098, 0x707098);

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
    phi_8ca8.AllocDefinitionWithSprite_5332D0(0, 0, 0, 0);

    AmbulanceTask jaw_20;
    jaw_20.field_14_patient_count = 1;

    CollisionSoundQueue collision_sound_queue;
    collision_sound_queue.field_0_pSoundObj = 0;

    GeneratorPool maccies;
    maccies.FinalizeAfterScriptLoad_4C1CD0();

    Sprite car_3c;
    car_3c.field_20_instance_id = 5;

    CrusherPool snooky;
    snooky.IsCarBeingCrushed_4887A0(0);

    NetUi network;
    network.cb_FillSessionList_519D30(0, 0);

    Light light;
    light.AddToGrid_4D6D70();

    PedManager PedManager;
    PedManager.DoIanTest_471060(0);

    Player lewin(1);
    u8 zero_u8 = 0;
    lewin.GetIdx_4881E0();
    lewin.AddCarToHistory_5645B0(0);
    lewin.PromoteCarInHistory_564610(0, 0);
    lewin.RemoveCarFromHistory_564680(0);
    lewin.SetKFCarWeapon_564710(0, 0);
    lewin.SetKFWeapon_564790(0);
    lewin.ClearKFWeapon_5647D0();
    lewin.GetCurrPlayerWeapon_5648F0();
    lewin.SetWeapon_564910(0);
    lewin.HasAnyAmmo_564940();
    lewin.AddWeaponWithAmmo_564960(0, 0);
    lewin.SelectNextOrPrevWeapon_5649D0(0, 0);
    lewin.LoadCarWeapons_564AD0(0);
    lewin.ClearCarWeapons_564B60();
    lewin.CleanupEmptyAmmoWeapons_564B80();
    lewin.UnloadCarWeapons_564C00();
    lewin.RemovePlayerWeapons_564C50();
    lewin.ClearPowerUps_564CC0();
    lewin.ClearPowerUpsExceptJailCard_564CF0();
    lewin.CollectPowerUp_564D60(0);
    lewin.tick_down_powerups_565070();
    lewin.RestorePowerUpsFromSave_5651F0(0);
    lewin.TeleportToDebugCam_565310();
    lewin.DebugWatchNearestCar_5653E0();
    lewin.InitPlayerPed_565490(0);
    lewin.SetInputs_565740(0);
    lewin.IncrementGangRespectFromDebugKeys_565770(0);
    lewin.IncreaseWantedLevelFromDebugKeys_565860();
    lewin.Hud_Controls_565890(0);
    lewin.HandleKeyRelease_566380(0);
    lewin.CharacterControls_566520();
    lewin.ControlInputs_566820();
    lewin.HandleControls_5668D0(0);
    lewin.DoCarControlInputs_566C30(0);
    lewin.DoPedControlInputs_566C80(0);
    lewin.ShowDebugInfo_566EE0(0);
    lewin.RespawnPlayer_5670B0();
    lewin.Wasted_567130();
    lewin.UpdateAuxPedDeath_567850();
    lewin.Busted_5679E0();
    lewin.UpdateCurrentZones_568520();
    lewin.UpdateSoundListener_568630();
    lewin.HandleDebugZoom_568670();
    lewin.UpdateCamera_5686D0(0);
    lewin.Disconnect_568730();
    lewin.Service_5687F0();
    lewin.UpdatePaused_569410();
    lewin.EndRemoteControl_569530();
    lewin.ResetAuxCamera_5695A0();
    lewin.StartRemoteControl_569600(0);
    lewin.WatchCar_5696D0(0);
    lewin.GetPosU8_569840(zero_u8, zero_u8, zero_u8);
    lewin.GetPlayerCar_5698E0();
    lewin.get_pos_569920(0, 0, 0);
    lewin.ChangeLifeCountByAmount_5699F0(0);
    lewin.ColorScoreFromRemap_569A10();
    lewin.SetScoreTextColour_569C20();
    lewin.InitializePlayerState_569CB0();
    lewin.DebugToggleRemoteControl_569E70();
    lewin.GetDeathText_569F00();
    lewin.DisableInputs_569F40();
    lewin.DisableAllControls_569FF0();
    lewin.EnableAllControls_56A000();
    lewin.EnableKFMode_56A010();
    lewin.DisableKFMode_56A020();
    lewin.DisableEnterVehicles_56A030();
    lewin.EnableEnterVehicles_56A040();
    lewin.RestoreCarsFromSave_56A0F0();
    lewin.CopyPlayerDataToSave_56A1A0(0);
    lewin.UpdateGameFromSave_56A310(0);
    lewin.ApplyCheats_56A490();
    lewin.ClearInputs_56A6D0();

    Camera camera;
    camera.SavePrevCamPos_4357B0();
    camera.IncreaseElevation_4357F0();
    camera.DecreaseElevation_435810();
    camera.ResetElevation_435830();
    camera.ClampTargetZ_435840();
    camera.ApplyMovementDeltaFrom_435860(0);
    camera.MoveTarget_435D20(1, 1, 1, 1, 1, 1);
    camera.ResetCameraSmoothing_435DD0();
    camera.RefreshBoundaries_436110();
    camera.SetShake_436120(1);
    camera.ResetPanning_436830();
    camera.SetTarget_4397D0(Fix16(), Fix16(), Fix16(), Fix16());
    camera.IsPointInBoundaries_58CF10(1, 1);

    Ped cn;
    cn.SetRecentCrimeTimer_45B550();
    cn.SetPlayer_45B560(NULL, 0);
    cn.IsEmergencyOccupation_45B590();

    ScriptGosubStack miss2;
    miss2.add_503160(0);
    miss2.remove_503180();
    miss2.remove_5031E0(0);

    InputRecorder_67F8B0 input_recorder;
    input_recorder.StaticShutdown_4CDCD0();
    input_recorder.clear_inputs_4CDCE0();
    input_recorder.set_input_4CDCF0(0);
    input_recorder.clear_input_4CDD10(0);
    input_recorder.should_ignore_input_4CDD80(0);
    input_recorder.should_record_input_4CDDF0(0);
    input_recorder.save_replay_record_4CDE20(0);
    input_recorder.SaveReplay_4CDED0();
    input_recorder.modify_inputs_4CDF30(0);
    input_recorder.AppendReplayHeader_4CDF70();
    input_recorder.LoadReplayHeader_4CE380(false);
    input_recorder.VerifyAttractFilesExist_4CE650();
    input_recorder.GetNextAttrReplay_4CE6E0(0);
    input_recorder.input_init_replay_4CE740(0);
    input_recorder.input_init_live_4CE880(0);
    input_recorder.replay_save_4CEA40(0);
    input_recorder.get_input_bits_4CEAC0();
    input_recorder.save_replay_inputs_4CED00(0, 0);
    input_recorder.DisplayInputBits_4CED90();
    input_recorder.IsPlayingBack_4CEDF0();
    input_recorder.ShowInput_4CEE10();

    Hud garox;

    MapRenderer nano;
    u16 zero = 0;
    nano.draw_bottom_4ED290(zero);

    SpriteRenderer sprite_renderer;
    sprite_renderer.ResetAll_4954F0();

    FrameProfiler mike;
    mike.DebugDrawProfiling_4FF250();

    CrimeReport crime_report_14;
    CrimeReportQueue crime_report_queue_CC;

    Firefighter tango_28;
    tango_28.Clear_450C10();

    FirefighterPool tango_54;

    CarChaseTaskTable car_chase_task_table;
    car_chase_task_table.FreeEntry_474CC0(0);

    CollisionCounters collide_C;

    Fix16_Rect car14_18;
    car14_18.DoSetCurrentRect_59DD60();

    Particle_4C particle;
    particle.PoolAllocate();
}

DEFINE_GLOBAL(HINSTANCE, gHInstance_708220, 0x708220);
DEFINE_GLOBAL(HANDLE, gMutex_707078, 0x707078);
DEFINE_GLOBAL(s32, gGTA2VersionMajor_708280, 0x708280);
DEFINE_GLOBAL(s32, gGTA2VersionMinor_708284, 0x708284);
DEFINE_GLOBAL_ARRAY(char_type, gWorkingDir_707F64, 256, 0x707F64);
DEFINE_GLOBAL_INIT(bool, bDoFrontEnd_626B68, true, 0x626B68);

DEFINE_GLOBAL(s32, window_width_706630, 0x706630);
DEFINE_GLOBAL(s32, window_height_706B50, 0x706B50);
DEFINE_GLOBAL(s32, full_width_706B5C, 0x706B5C);
DEFINE_GLOBAL(s32, full_height_706798, 0x706798);
DEFINE_GLOBAL(s32, gWindowX_706B60, 0x706B60);
DEFINE_GLOBAL(s32, gWindowY_706B64, 0x706B64);
DEFINE_GLOBAL(s32, bStartNetworkGame_7081F0, 0x7081F0);
DEFINE_GLOBAL(BYTE, gMinimizedState_70827C, 0x70827C);
DEFINE_GLOBAL(BYTE, bVideoModeResetPending_706C5D, 0x706C5D);
DEFINE_GLOBAL(BYTE, bReplayMode_6F5B71, 0x6F5B71);
DEFINE_GLOBAL_INIT(s32, gStartMode_626A0C, 2, 0x626A0C);

DEFINE_GLOBAL(s32, bTrippleBuffer_706C54, 0x706C54);
DEFINE_GLOBAL(char_type, bWindowedModeAvailable_706C5C, 0x706C5C);
DEFINE_GLOBAL(u32, gBufferMode_706B34, 0x706B34);
DEFINE_GLOBAL_ARRAY(char_type, gRenderDllName_7067F0, 256, 0x7067F0);
DEFINE_GLOBAL_ARRAY(char_type, gVideoDllName_706654, 256, 0x706654);
DEFINE_GLOBAL(s32, gVideodevice_70694C, 0x70694C);
DEFINE_GLOBAL(s32, gRenderdevice_706998, 0x706998);
DEFINE_GLOBAL(u16, gDMusicVer_67BD32, 0x67BD32);

DEFINE_GLOBAL_INIT(char_type, gLighting_626A09, 1, 0x626A09);

DEFINE_GLOBAL(u32, gLightingDrawFlag_7068F4, 0x7068F4);
DEFINE_GLOBAL(LPDIRECTINPUTDEVICEA, gMouseDevice_706C60, 0x706C60);
// The original mouse format uses the seven object records already recovered in Frontend.cpp.
extern DIOBJECTDATAFORMAT gMouseObjectDataFormats_5EA3D0[7];
DEFINE_GLOBAL_INIT(DIDATAFORMAT, gMouseDataFormat_601A84, {24u COMMA 16u COMMA DIDF_RELAXIS COMMA 16u COMMA 7u COMMA gMouseObjectDataFormats_5EA3D0}, 0x601A84);

DEFINE_GLOBAL_INIT(u8, max_frame_rate_626A08, 1, 0x626A08);
DEFINE_GLOBAL(u8, min_frame_rate_706C50, 0x706C50);

DEFINE_GLOBAL(u8, gFramesSinceDraw_6F58D8, 0x6F58D8);
DEFINE_GLOBAL(u8, bInputsPending_6F5760, 0x6F5760);
DEFINE_GLOBAL(u8, bForceGameUpdate_6F5880, 0x6F5880);

// TODO: move
MATCH_FUNC(0x4DA820)
EXPORT void ForceGameUpdate_4DA820()
{
    bForceGameUpdate_6F5880 = true;
}

// TODO: Other missing stubs here
MATCH_FUNC(0x4DA830)
EXPORT void __stdcall ResetFrameTimer_4DA830()
{
    gMatchStartTime_6F5A28 = timeGetTime();
    gFramesSinceDraw_6F58D8 = 0;
}

// TODO: move
MATCH_FUNC(0x5D8E00)
void UpdateGameScreenSize_5D8E00()
{
    if (gGame_0x40_67E008)
    {
        if (gVidSys_7071D0->field_40_full_screen == -2)
        {
            if (gCachedScreenDimension_706C58 != window_width_706630)
            {
                gGame_0x40_67E008->SetScreenSize_4B8E00(window_width_706630, window_height_706B50);
            }
            gCachedScreenDimension_706C58 = window_width_706630;
        }
        else
        {
            if (static_cast<u32>(gCachedScreenDimension_706C58) != gVidSys_7071D0->field_4C_rect_bottom)
            {
                gGame_0x40_67E008->SetScreenSize_4B8E00(gVidSys_7071D0->field_48_rect_right, gVidSys_7071D0->field_4C_rect_bottom);
            }
            gCachedScreenDimension_706C58 = gVidSys_7071D0->field_4C_rect_bottom;
        }
        ForceGameUpdate_4DA820();
        ResetFrameTimer_4DA830();
    }
}

// todo move to another file for ordering
MATCH_FUNC(0x5D8EB0)
EXPORT void __stdcall Init_FrameRateLightingAndExplodingScores_5D8EB0()
{
    if (bStartNetworkGame_7081F0)
    {
        max_frame_rate_626A08 = 1;
        min_frame_rate_706C50 = 1;
    }
    else
    {
        max_frame_rate_626A08 = gRegistry_6FF968.Get_Screen_Setting_5870D0("max_frame_rate", 1) != 0;
        min_frame_rate_706C50 = gRegistry_6FF968.Get_Screen_Setting_5870D0("min_frame_rate", 0) != 0;
    }

    if (gRegistry_6FF968.Get_Screen_Setting_5870D0("lighting", 1))
    {
        gLighting_626A09 = 1;
        gLightingDrawFlag_7068F4 = 0x8000;
    }
    else
    {
        gLighting_626A09 = 0;
        gLightingDrawFlag_7068F4 = 0;
    }

    if (gSpriteMaskCache_67AF1C)
    {
        gSpriteMaskCache_67AF1C->InvalidateAllMasks_48F710();
    }

    bExplodingScoresOff_67D4FB = gRegistry_6FF968.Get_Screen_Setting_5870D0("exploding_on", 1) != 1;
}

// todo: move
MATCH_FUNC(0x5D7C60)
EXPORT void __stdcall Input_MouseAcquire_5D7C60()
{
    if (gMouseDevice_706C60)
    {
        gMouseDevice_706C60->Acquire();
    }
}

// todo: move
MATCH_FUNC(0x5D7C70)
EXPORT void __stdcall Input_ReleaseMouse_5D7C70()
{
    if (gMouseDevice_706C60)
    {
        gMouseDevice_706C60->Unacquire();
        gMouseDevice_706C60->Release();
        gMouseDevice_706C60 = NULL;
    }
}

// todo: move
MATCH_FUNC(0x5D94F0)
EXPORT void __stdcall HideCursor_5D94F0()
{
    do
    {

    } while (ShowCursor(FALSE) >= 0);
}

// todo: move
MATCH_FUNC(0x5D7BF0)
EXPORT bool Input_InitMouse_5D7BF0()
{
    // TODO: using the HR error checking macros doesn't match here, what did they do??
    if (!gpDInput_67B804 || gMouseDevice_706C60 || gpDInput_67B804->CreateDevice(GUID_SysMouse, &gMouseDevice_706C60, NULL) ||
        gMouseDevice_706C60->SetDataFormat(&gMouseDataFormat_601A84) || gMouseDevice_706C60->SetCooperativeLevel(gHwnd_707F04, DISCL_EXCLUSIVE | DISCL_FOREGROUND))
    {
        return false;
    }
    gMouseDevice_706C60->Acquire();
    return true;
}

// todo: move
MATCH_FUNC(0x5D93A0)
EXPORT void __stdcall SetFullScreenMode_5D93A0()
{
    bool bModeChanged = false;
    s32 bcheckModeRet = pVid_CheckMode(gVidSys_7071D0, full_width_706B5C, full_height_706798, 16);
    if (!bcheckModeRet)
    {
        if (full_width_706B5C == 640 ||
            (full_width_706B5C = 640,
             bModeChanged = true,
             full_height_706798 = 480,
             (bcheckModeRet = pVid_CheckMode(gVidSys_7071D0, 640, 480, 16)) == 0))
        {
            FatalError_4A38C0(Gta2Error::VideomodeNotAvailable,
                              "C:\\Splitting\\Gta2\\Source\\video.cpp",
                              1359,
                              full_width_706B5C,
                              full_height_706798,
                              16);
        }
    }

    if (gVidSys_7071D0)
    {
        gVidSys_7071D0->field_4_flags |= 0x100;
    }

    if (pVid_SetMode(gVidSys_7071D0, gHwnd_707F04, bcheckModeRet))
    {
        // Gta2Error::FailedToSetFullscreenVideomode
        FatalError_4A38C0(0x40D, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1365, bcheckModeRet);
    }

    HideCursor_5D94F0();

    SetWindowLongA(gHwnd_707F04, GWL_STYLE, WS_VISIBLE);
    SetWindowPos(gHwnd_707F04, NULL, 0, 0, 0, 0, 0x63Bu);
    UpdateWindow(gHwnd_707F04);
    ShowWindow(gHwnd_707F04, SW_SHOWNORMAL);

    Input_InitMouse_5D7BF0();

    if (bModeChanged)
    {
        gRegistry_6FF968.Set_Screen_Setting_587170("full_width", full_width_706B5C);
        gRegistry_6FF968.Set_Screen_Setting_587170("full_height", full_height_706798);
    }
}

// todo: move
MATCH_FUNC(0x5D92C0)
EXPORT char_type IsWindowedModeAvailable_5D92C0()
{
    return bWindowedModeAvailable_706C5C;
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

// todo: move
// __stdcall, not __cdecl: see GetLayout_4D6000 (the `push 0x316` goes before the rect loads)
MATCH_FUNC(0x5D9510)
EXPORT bool __stdcall SetWindowedMode_5D9510()
{

    if (!IsWindowedModeAvailable_5D92C0())
    {
        return false;
    }

    Input_ReleaseMouse_5D7C70();
    SetWindowLongA(gHwnd_707F04, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
    SetWindowPos(gHwnd_707F04, NULL, 0, 0, 0, 0, 0x63Bu);
    UpdateWindow(gHwnd_707F04);
    ShowWindow(gHwnd_707F04, SW_SHOW);

    RECT windowRect;
    GetWindowRect(gHwnd_707F04, &windowRect);

    RECT clientRect;
    GetClientRect(gHwnd_707F04, &clientRect);

    if (!SetWindowPos(gHwnd_707F04,
                      NULL,
                      gWindowX_706B60,
                      gWindowY_706B64,
                      windowRect.right + clientRect.left + window_width_706630 - windowRect.left - clientRect.right,
                      windowRect.bottom + clientRect.top + window_height_706B50 - windowRect.top - clientRect.bottom,
                      0x316))
    {
        return false;
    }

    UpdateWindow(gHwnd_707F04);
    ShowWindow(gHwnd_707F04, SW_SHOW);
    if (gVidSys_7071D0)
    {
        gVidSys_7071D0->field_4_flags |= 0x100;
    }

    if (pVid_SetMode(gVidSys_7071D0, gHwnd_707F04, -2) == 1)
    {
        return false;
    }
    else
    {
        ShowCursor_5D9660();
        return true;
    }
}

// todo move to another file for ordering
MATCH_FUNC(0x5D8F70)
EXPORT s32 ReadScreenSettings_5D8F70()
{
    const s32 full_width_old = full_width_706B5C;
    const s32 window_width_old = window_width_706630;
    const s32 full_height_old = full_height_706798;
    const s32 startMode_old = gStartMode_626A0C;
    const s32 trippleBuffer_old = bTrippleBuffer_706C54;
    const s32 window_height_old = window_height_706B50;

    if (bDoFrontEnd_626B68)
    {
        window_width_706630 = 640;
        window_height_706B50 = 480;
        full_width_706B5C = 640;
        full_height_706798 = 480;
    }
    else
    {
        window_width_706630 = gRegistry_6FF968.Get_Screen_Setting_5870D0("window_width", 640);
        window_height_706B50 = gRegistry_6FF968.Get_Screen_Setting_5870D0("window_height", 480);
        full_width_706B5C = gRegistry_6FF968.Get_Screen_Setting_5870D0("full_width", 640);
        full_height_706798 = gRegistry_6FF968.Get_Screen_Setting_5870D0("full_height", 480);
    }

    gStartMode_626A0C = gRegistry_6FF968.Get_Screen_Setting_5870D0("start_mode", 1);

    if (gBufferMode_706B34 == 0)
    {
        bTrippleBuffer_706C54 = 1;
    }
    else
    {
        bTrippleBuffer_706C54 = gRegistry_6FF968.Get_Screen_Setting_5870D0("tripple_buffer", 0);
    }

    if (gStartMode_626A0C == startMode_old && bTrippleBuffer_706C54 == trippleBuffer_old)
    {
        if (gStartMode_626A0C == 1)
        {
            if (full_width_706B5C == full_width_old && full_height_706798 == full_height_old)
            {
                return 0;
            }
        }
        else if (gStartMode_626A0C || window_width_706630 == window_width_old && window_height_706B50 == window_height_old)
        {
            return 0;
        }
    }

    return 1;
}

// todo: move
MATCH_FUNC(0x5D98E0)
EXPORT void __stdcall SetSavedGamma_5D98E0()
{
    DWORD gamma = gRegistry_6FF968.Get_Screen_Setting_5870D0("gamma", 10);
    if (SetGamma_5D9910(gamma))
    {
        counter_706C4C = 30;
    }
}

// todo: move
MATCH_FUNC(0x5D92D0)
EXPORT void __stdcall SetVideoModeFromSettings_5D92D0()
{
    Init_FrameRateLightingAndExplodingScores_5D8EB0();
    ReadScreenSettings_5D8F70();
    Input_MouseAcquire_5D7C60();

    pVid_CloseScreen(gVidSys_7071D0);

    if (bTrippleBuffer_706C54)
    {
        gVidSys_7071D0->field_4_flags |= 0x10;
    }
    else
    {
        gVidSys_7071D0->field_4_flags &= ~0x10u;
    }

    if (!gStartMode_626A0C)
    {
        if (!SetWindowedMode_5D9510())
        {
            if (window_width_706630 == 640 || (window_width_706630 = 640, window_height_706B50 = 480, !SetWindowedMode_5D9510()))
            {
                SetFullScreenMode_5D93A0();
                gStartMode_626A0C = 1;
                gRegistry_6FF968.Set_Screen_Setting_587170("start_mode", 1u);
            }
            else
            {
                gRegistry_6FF968.Set_Screen_Setting_587170("window_width", window_width_706630);
                gRegistry_6FF968.Set_Screen_Setting_587170("window_height", window_height_706B50);
            }
        }
    }
    else
    {
        SetFullScreenMode_5D93A0();
    }
    SetSavedGamma_5D98E0();
}

MATCH_FUNC(0x5D9860)
EXPORT s32 jGBH_Graphics_Free_5D9860()
{
    return GBH_Graphics_Free_5EA640();
}

MATCH_FUNC(0x5D9830)
EXPORT void __stdcall GBH_Graphis_DMA_Video_Free_5D9830()
{
    jGBH_Graphics_Free_5D9860();
    if (gVidSys_7071D0)
    {
        DMA_Video_Unload_5EB8C0(gVidSys_7071D0);
        gVidSys_7071D0 = NULL;
    }
    ShowCursor_5D9660();
}

// todo: move
MATCH_FUNC(0x5D9290)
EXPORT bool Vid_FindDevice_5D9290()
{
    SDevice* pDevice = pVid_FindDevice(gVidSys_7071D0, gVideodevice_70694C);
    return pDevice && (pDevice->field_4_flags & 1) == 1;
}

// todo move to another file for ordering
MATCH_FUNC(0x5D9800)
EXPORT s32 GetHwndDCDeviceCaps_5D9800()
{
    HDC hdc = GetDC(gHwnd_707F04);
    s32 deviceCaps = GetDeviceCaps(hdc, BITSPIXEL);
    ReleaseDC(gHwnd_707F04, hdc);
    return deviceCaps;
}

// todo move to another file for ordering
MATCH_FUNC(0x5D97C0)
EXPORT void __stdcall GBH_GraphicsInit_5D97C0()
{
    if (GBH_GraphicsLoad_5EB680(gRenderDllName_7067F0, gVidSys_7071D0))
    {
        FatalError_4A38C0(Gta2Error::DllLoadError, "C:\\Splitting\\Gta2\\Source\\video.cpp", 206, gTmpBuffer_67C598);
    }
}

// todo move to another file for ordering
MATCH_FUNC(0x5D96C0)
EXPORT void InitVideo_5D96C0()
{
    s32 ddraw_flags = 0;
    if (DMA_Video_LoadDll_5EB970(gVideoDllName_706654))
    {
        FatalError_4A38C0(Gta2Error::DllLoadError, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1647, gVideoDllName_706654);
    }

    gVidSys_7071D0 = pVid_Init_SYS(gHInstance_708220, 0); // flags param ??

    pVid_SetDevice(gVidSys_7071D0, gVideodevice_70694C);

    if (!gVidSys_7071D0)
    {
        FatalError_4A38C0(Gta2Error::DMAVideoInitFail, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1656);
    }

    if (gBufferMode_706B34 == 1)
    {
        ddraw_flags = 64;
    }

    gVidSys_7071D0->field_4_flags |= ddraw_flags;

    if (GetHwndDCDeviceCaps_5D9800() == 16)
    {
        bool bDeviceFound = Vid_FindDevice_5D9290();
        bWindowedModeAvailable_706C5C = true;
        if (!bDeviceFound)
        {
            bWindowedModeAvailable_706C5C = false;
        }
    }
    else
    {
        bWindowedModeAvailable_706C5C = false;
    }

    SetVideoModeFromSettings_5D92D0();

    GBH_GraphicsInit_5D97C0();

    SetSavedGamma_5D98E0();

    pVid_ClearScreen(gVidSys_7071D0, 0, 0, 0, 0, 0, gVidSys_7071D0->field_48_rect_right, gVidSys_7071D0->field_4C_rect_bottom);

    pVid_FlipBuffers(gVidSys_7071D0);

    pVid_ClearScreen(gVidSys_7071D0, 0, 0, 0, 0, 0, gVidSys_7071D0->field_48_rect_right, gVidSys_7071D0->field_4C_rect_bottom);
}

// todo move to another file for ordering
MATCH_FUNC(0x4DA440)
EXPORT void __stdcall Init_keybrd_jolly_and_sound_4DA440()
{
    KeyboardLayout::create_4D5F50();
    PlyDat::Create_56C2C0();

    if (!bSkip_audio_67D6BE)
    {
        gRoot_sound_66B038.Init_40EF80();
        gRoot_sound_66B038.SetCDVol_40F0F0(gRegistry_6FF968.Get_Sound_Setting_586AE0("CDVol", 127));
        gRoot_sound_66B038.SetSfxVol_40F0B0(gRegistry_6FF968.Get_Sound_Setting_586AE0("SFXVol", 127));
        gRoot_sound_66B038.Set3DSound_40F160(gRegistry_6FF968.Sound_Setting_Exists_586A70("do_3d_sound"));
        gRegistry_6FF968.Clear_Or_Delete_Sound_Setting_586BF0("do_3d_sound", gRoot_sound_66B038.Get3DSound_40F180());
    }
}

MATCH_FUNC(0x4DB170)
void __stdcall ClearDebugFlags_4DB170()
{
    bShow_all_arrows_67D6E7 = false;
    bDo_kill_phones_on_answer_67D6E8 = false;
    bSkip_dummies_67D4EF = false;
    bSkip_tiles_67D655 = false;
    bDo_test_67D4F8 = false;
    bSkip_mission_67D4E5 = false;
    bDo_brian_test_67D544 = false;
    bDo_iain_test_67D4E9 = false;
    bSkip_user_67D506 = false;
    bSkip_traffic_lights_67D4EC = false;
    bSkip_recycling_67D575 = false;
    bLog_collisions_67D4E6 = false;
    bDo_show_traffic_lights_info_67D4FA = false;
    bLimit_recycling_67D4CA = false;
    bNo_annoying_chars_67D586 = false;
    bSkip_slopes_67D505 = false;
    bSkip_left_67D6BF = false;
    bSkip_right_67D4E4 = false;
    bSkip_top_67D574 = false;
    bSkip_bottom_67D4E7 = false;
    bSkip_lid_67D546 = false;
    bLog_routefinder_67D6D1 = false;
    bDo_mike_67D5CC = false;
    bSkip_particles_67D64D = false;
    bShow_hidden_faces_67D5CD = false;
    bGet_all_weapons_67D684 = false;
    bDo_exit_after_replay_67D6E4 = false;
    bDont_get_car_back_67D4F5 = false;
    bSkip_ambulance_67D6C9 = false;
    bSkip_frontend_67D53B = false;
    bDo_invulnerable_67D4CB = false;
    bKeep_weapons_after_death_67D54D = false;
    bSkip_skidmarks_67D585 = false;
    gDo_infinite_lives_67D4C9 = false;
    bDo_load_savegame_67D4F0 = false;
    bSkip_audio_67D6BE = false;
    bDo_debug_keys_67D6CF = false;
    bLog_input_67D4CC = false;
    bLog_directinput_67D6C0 = false;
    bIgnore_replay_header_67D4F3 = false;
    bSkip_trains_67D550 = false;
    bSkip_buses_67D558 = false;
    bSkip_quit_confirm_67D4E8 = false;
    bSkip_fire_engines_67D53A = false;
    bSkip_window_check_67D54E = false;
    gDo_miss_logging_67D6BC = false;
    byte_67D654 = false;
    bDo_police_1_67D568 = false;
    bDo_police_2_67D569 = false;
    bDo_police_3_67D56A = false;
    bSkip_draw_67D4EA = false;
    bDo_free_shopping_67D6CD = false;
    gCheatMiniCars_67D6C8 = false;
    gCheatGetPlayerPoints_67D4C8 = false;
    gCheatGet10MillionMoney_67D6CE = false;
    gCheatUnlimitedElectroGun_67D4F7 = false;
    gCheatAllGangMaxRespect_67D587 = false;
    gCheatGet99Lives_67D4F1 = false;
    gCheat10xMultiplier_67D589 = false;
    gCheatUnlockThreeLevels_67D6CB = false;
    gCheatUnlockLevelsOneAndTwo_67D584 = false;
    gCheatUnlockAllLevels_67D538 = false;
    gCheatGetBasicWeaponsMaxAmmo_67D545 = false;
    gCheatOnlyElvisPeds_67D4ED = false;
    gCheatOnlyMuggerPeds_67D5A4 = false;
    gCheatNakedPeds_67D5E8 = false;
    gCheatUnlimitedJailCard_67D4F6 = false;
    gCheatInvisibility_67D539 = false;
    gCheatUnlimitedDoubleDamage_67D57C = false;
    byte_67D56B = false;
    gCheatUnlimitedFlameThrower_67D6CC = false;
}

MATCH_FUNC(0x4DB070)
EXPORT void __stdcall ShowPlayerLeftMessage_4DB070(u8 idx)
{
    Player* pPlayer = gGame_0x40_67E008->GetPlayer_4219E0(idx);
    pPlayer->Disconnect_568730();
    swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("comms1"), pPlayer->field_83C_player_name);
    gHud_2B00_706620->field_12F0_mp_message.ShowText_5D5730(tmpBuff_67BD9C);
}

MATCH_FUNC(0x4DB0D0)
void __stdcall ExitGameCallback_4DB0D0(Game* pGame, s32 reason)
{
    char_type Buffer[260];
    sprintf(Buffer, "(ExitGameCallback)Called with reason - %d", reason);
    gErrorLog_67C530.Write_4D9620(Buffer);
    pGame->ExitGameNoBonus_4B8C00(0, GameExitType::CloseGameImmediately_1);
}

// todo move to another file for ordering
// https://decomp.me/scratch/VazoB
MATCH_FUNC(0x4DA4D0)
EXPORT void __stdcall InitializeGame_4DA4D0()
{
    if (bReplayMode_6F5B71)
    {
        gInputRecorder_67F8B0.input_init_replay_4CE740(gHInstance_708220);
        bReplayMode_6F5B71 = false;
    }
    else
    {
        gInputRecorder_67F8B0.input_init_live_4CE880(gHInstance_708220);
    }

    gRoot_sound_66B038.Set3DSound_40F160(gRegistry_6FF968.Sound_Setting_Exists_586A70("do_3d_sound"));
    gRegistry_6FF968.Clear_Or_Delete_Sound_Setting_586BF0("do_3d_sound", gRoot_sound_66B038.Get3DSound_40F180());

    if (bStartNetworkGame_7081F0)
    {
        gTagGame_6F8450.Init_516560();

        ClearDebugFlags_4DB170();

        gGame_0x40_67E008 = new Game(gNetPlay_7071E8.CountActivePlayers_521350(), gNetPlay_7071E8.GetPlayerIdx_409C40());
        gNetPlay_7071E8.SetExitGameCallBack_521330(reinterpret_cast<int>(ExitGameCallback_4DB0D0), gGame_0x40_67E008);

        memset(&gCurrentNetInputs_6F57D8, 0, sizeof(gCurrentNetInputs_6F57D8));
        memset(&gPrevNetInputs_6F5B28, 0, sizeof(gPrevNetInputs_6F5B28));

        memset(byte_6F5AC8, 0, sizeof(byte_6F5AC8));
        gNetworkPlayerIdx_6F56C8 = gNetPlay_7071E8.GetPlayerIdx_409C40();
        gpInputBuffer_6F58C0 = reinterpret_cast<ReceivedPacket*>(&gCurrentNetInputs_6F57D8.field_0_inputs[0]);
        gCurrentInputsBufferSize_6F58C4 = 48;
        dword_6F5B00 = 0;
        bRecordStartTime_6F593C = true;
        gPlayerQuit_6F5AEC = false;
        gNetDisconnectPlayersBits_6F56B8 = 0;
        gNetworkFrameCounter_6F5868 = 0;
        dword_6F580C = 0;
        gTotalNetworkTime_6F5980 = 0;
        dword_6F5AC0 = 0;
        gNetTimeLimit_6F573C = gGameSession_67E8E0.GetTimeLimit_461DC0();
        if (gNetTimeLimit_6F573C > 60)
        {
            gNetTimeLimit_6F573C = 60;
        }
        gHudTimerIdx_6F5860 = -1;
        gNetTimeLimitEnabled_6F58A4 = (gNetTimeLimit_6F573C != 0);
        gNetTimeSecondsOfCurrMin_6F58A0 = 0;
        gFramesOfCurrSecond_6F5858 = 0;
        gNetTimerFlashing_6F59C0 = 0;
    }
    else
    {
        gGame_0x40_67E008 = new Game(1, 0);
    }

    gGame_0x40_67E008->LoadGameFiles_4B8C40();
    gGame_0x40_67E008->BootGame_4B8EB0();

    gMatchStartTime_6F5A28 = timeGetTime();
    gFramesSinceDraw_6F58D8 = 0;
    bForceGameUpdate_6F5880 = false;
    bInputsPending_6F5760 = true;
}

// todo: move
MATCH_FUNC(0x5D9680)
EXPORT void OnVideoModeChanged_5D9680()
{
    ConvertColourBanks_5D7CB0();
    UpdateGameScreenSize_5D8E00();
}

// todo move to another file for ordering
MATCH_FUNC(0x5D9690)
EXPORT void __stdcall ApplyChangedScreenSettings_5D9690()
{
    Init_FrameRateLightingAndExplodingScores_5D8EB0();
    if (static_cast<u8>(ReadScreenSettings_5D8F70()) || bVideoModeResetPending_706C5D)
    {
        SetVideoModeFromSettings_5D92D0();
        OnVideoModeChanged_5D9680();
        bVideoModeResetPending_706C5D = false;
    }
    Input_MouseAcquire_5D7C60();
}

// TODO: order
MATCH_FUNC(0x4DA780)
EXPORT bool ExecuteGame_4DA780()
{
    bool exit_requested = gGame_0x40_67E008->ExecuteGame_4B9640();
    if (gFpsCounter_7055D4)
    {
        gFpsCounter_7055D4->field_18_execute_game_timer.AccumulateElapsed_5BEBF0();
    }
    return exit_requested;
}

MATCH_FUNC(0x4DA7B0)
void __stdcall Draw_4DA7B0()
{
    gGame_0x40_67E008->Draw_4B92D0();
    pVid_FlipBuffers(gVidSys_7071D0);
    if (bSkip_lid_67D546 || bSkip_top_67D574 || bSkip_bottom_67D4E7 || bSkip_left_67D6BF || bSkip_right_67D4E4)
    {
        pVid_ClearScreen(gVidSys_7071D0, 0, 0, 0, 0, 0, gVidSys_7071D0->field_48_rect_right, gVidSys_7071D0->field_4C_rect_bottom);
    }
}

DEFINE_GLOBAL(Network_PlayerInputs, gNetInputsHistory1_6F56E0, 0x6F56E0);
DEFINE_GLOBAL(Network_PlayerInputs, gNetInputsHistory2_6F5798, 0x6F5798);

// Re-sends our inputs of a previous frame to one player.
MATCH_FUNC(0x4DA9B0)
EXPORT void __stdcall NetSendOldInputs_4DA9B0(Network_InputData* pInputs, s32 delayed_ticks, u8 player_idx)
{
    gInputSendData_6F5B18.field_0_data = pInputs;
    gInputSendData_6F5B18.field_4_len = bDo_sync_check_67D6C1 ? sizeof(Network_InputData) : sizeof(u32);
    gNetPlay_7071E8.SendToPlayer_521630(&gInputSendData_6F5B18, player_idx, delayed_ticks);
}

MATCH_FUNC(0x4DA9F0)
EXPORT void Net_Receive_Players_Inputs_4DA9F0()
{
    s32 retries = 0;
    s32 max_retries = bRecordStartTime_6F593C ? 25 : 6;

    // Update previous stored inputs
    memcpy(&gNetInputsHistory2_6F5798, &gNetInputsHistory1_6F56E0, sizeof(gNetInputsHistory2_6F5798));
    memcpy(&gNetInputsHistory1_6F56E0, &gCurrentNetInputs_6F57D8, sizeof(gNetInputsHistory1_6F56E0));

    // Check which players we are waiting for
    u32 waiting_bits = 0;
    for (u32 i = 0; i < GTA2_COUNTOF(gNetPlay_7071E8.field_758_n2.field_10_players); i++)
    {
        if (gNetPlay_7071E8.field_758_n2.field_10_players[i].field_0_in_use && i != gNetworkPlayerIdx_6F56C8)
        {
            waiting_bits |= 1 << i;
        }
    }

    u32 start_time = timeGetTime();
    gTotalNetworkTime_6F5980 = 0;

    // Do not execute next game tick until we receive all players inputs
    while (waiting_bits)
    {
        // Leave a window to escape the loop if player is trying to exit the game
        if (gGame_0x40_67E008->UpdateExitTimer_4B8C20())
        {
            return;
        }

        // Wait a packet. If one was received, analyse its data and do something
        s32 player_idx;
        u32 packet_subtype;
        if (gNetPlay_7071E8.ReceiveGameMessage_521890(reinterpret_cast<Network_Buffer*>(&gpInputBuffer_6F58C0), &player_idx, &packet_subtype))
        {
            switch (packet_subtype)
            {
                case packet_subtype::StandardPacket_2:
                {
                    switch (gpInputBuffer_6F58C0->s_packet.field_0_subtype) // std_packet_subtype
                    {
                        case std_packet_subtype::GiveUp_1:
                            waiting_bits &= ~(1 << reinterpret_cast<s32>(gpInputBuffer_6F58C0->s_packet.field_1_pData));
                            gNetDisconnectPlayersBits_6F56B8 |= 1 << reinterpret_cast<s32>(gpInputBuffer_6F58C0->s_packet.field_1_pData);
                            break;
                        case std_packet_subtype::PlayerQuit_2:
                            waiting_bits &= ~(1 << player_idx);
                            gNetDisconnectPlayersBits_6F56B8 |= 1 << player_idx;
                            break;
                        case std_packet_subtype::KickFromConnection_3:
                            waiting_bits = 0;
                            gPlayerQuit_6F5AEC = true;
                            break;
                        default:
                            FatalError_4A38C0(Gta2Error::InvalidLine, "C:\\Splitting\\Gta2\\Source\\main.cpp", 853, 0);
                            break;
                    }
                    break;
                }
                case packet_subtype::PlayerInputs_3: // store player input for the latest/current tick
                    memcpy(&gCurrentNetInputs_6F57D8.field_0_inputs[player_idx], gpInputBuffer_6F58C0, gCurrentInputsBufferSize_6F58C4);
                    waiting_bits &= ~(1 << player_idx);

                    // Now check RNG if sync_check is activated. If it doesnt match, then do FatalError
                    if (bDo_sync_check_67D6C1 &&
                        gCurrentNetInputs_6F57D8.field_0_inputs[player_idx].field_4_rng !=
                            gCurrentNetInputs_6F57D8.field_0_inputs[gNetworkPlayerIdx_6F56C8].field_4_rng)
                    {
                        FatalError_4A38C0(Gta2Error::SyncErrorRandom,
                                          "C:\\Splitting\\Gta2\\Source\\main.cpp",
                                          822,
                                          gCurrentNetInputs_6F57D8.field_0_inputs[player_idx].field_4_rng,
                                          gCurrentNetInputs_6F57D8.field_0_inputs[gNetworkPlayerIdx_6F56C8].field_4_rng);
                    }
                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidLine, "C:\\Splitting\\Gta2\\Source\\main.cpp", 860, 0);
                    break;
            }
        }

        gTotalNetworkTime_6F5980 += gNetPlay_7071E8.field_8F4_time_diff;

        // Now manage those players we didn't received packets

        if (timeGetTime() - start_time > 1000 && retries >= max_retries && waiting_bits)
        {
            // Give up on the players that didn't answer
            for (u32 j = 0; j < 6 && waiting_bits; j++)
            {
                if (waiting_bits & (1 << j))
                {
                    waiting_bits &= ~(1 << j);
                    gNetDisconnectPlayersBits_6F56B8 |= 1 << j;
                    gNetPlay_7071E8.SendGiveUp_521DB0(j);
                    gTagGame_6F8450.field_20_bHasQuit[j] = 1;
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
                    NetSendOldInputs_4DA9B0(&gNetInputsHistory1_6F56E0.field_0_inputs[gNetworkPlayerIdx_6F56C8], 1, k);
                    NetSendOldInputs_4DA9B0(&gNetInputsHistory2_6F5798.field_0_inputs[gNetworkPlayerIdx_6F56C8], 2, k);
                }
            }
        }
    }
}

// Sum of all the bytes of a file
MATCH_FUNC(0x4DB120)
EXPORT s32 __stdcall FileByteSum_4DB120(FILE* hFile)
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
EXPORT void __stdcall MakeInitialSyncData_4DB2E0(u8* pSyncData)
{
    SyncCheckData* pData = reinterpret_cast<SyncCheckData*>(pSyncData);
    struct _stat st;

    memset(pData, 0, sizeof(SyncCheckData));
    pData->field_0_type = std_packet_subtype::SyncCheckData_5;
    if (gCarManager_6C_677930)
    {
        gCarManager_6C_677930->field_69_do_free_shopping = bDo_free_shopping_67D6CD;
    }

    pData->field_1_flags = 0;
    pData->field_1_flags = bLog_random_extra_67D5BC != 0;
    pData->field_1_flags |= bDo_sync_check_67D6C1 ? 2 : 0;
    pData->field_1_flags |= bLog_random_67D5FC ? 4 : 0;
    pData->field_11_exe_a = -1;
    pData->field_15_exe_b = 0;

    if (_stat(gGameSession_67E8E0.GetMapName_4C5940(), &st) == 0)
    {
        pData->field_5_map_size = st.st_size;
    }

    FILE* hFile = crt::fopen(gGameSession_67E8E0.GetScriptName_4C5960(), "rb");
    if (hFile)
    {
        pData->field_D_script_sum = FileByteSum_4DB120(hFile);
        crt::fclose(hFile);
    }

    hFile = crt::fopen("data\\nyc.gci", "rb");
    if (hFile)
    {
        pData->field_19_gci_sum = FileByteSum_4DB120(hFile);
        crt::fclose(hFile);
    }

    if (bIsFrench_67D53C)
    {
        pData->field_1D_bFrench = 1;
    }
    else
    {
        pData->field_1D_bFrench = 0;
    }
    pData->field_1E_player_idx = gNetworkPlayerIdx_6F56C8;
}

MATCH_FUNC(0x4DB410)
EXPORT void __stdcall FatalErrorMsg_4DB410(const char_type* pMsg)
{
    Shutdown_4DA740();
    GBH_Graphis_DMA_Video_Free_5D9830();
    ErrorMsgBox_5E4EC0(pMsg);
    DestroyWindow(gHwnd_707F04);
    exit(1);
}

// Quits with an error when another player's sync check data differs from ours
MATCH_FUNC(0x4DB440)
EXPORT void __stdcall CompareRemotePlayers_4DB440(u8* pLocalSyncData, u8* pRemoteSyncData)
{
    SyncCheckData* pLocal = reinterpret_cast<SyncCheckData*>(pLocalSyncData);
    SyncCheckData* pRemote = reinterpret_cast<SyncCheckData*>(pRemoteSyncData);
    char_type msg[260];
    char_type name[260];
    wchar_t wname[260];

    gNetPlay_7071E8.GetPlayerName_521100(wname, pRemote->field_1E_player_idx);
    WideCharToMultiByte(CP_ACP, 0, wname, -1, name, sizeof(name), NULL, NULL);

    if (pLocal->field_1D_bFrench != pRemote->field_1D_bFrench)
    {
        sprintf(msg, "Player %s: Language version is different", name);
        FatalErrorMsg_4DB410(msg);
    }
    if (pLocal->field_1_flags != pRemote->field_1_flags)
    {
        sprintf(msg, "Player %s: Debug flags are different", name);
        FatalErrorMsg_4DB410(msg);
    }
    if (pLocal->field_5_map_size != pRemote->field_5_map_size)
    {
        sprintf(msg, "Player %s: Level file '%s' is different", name, gGameSession_67E8E0.GetMapName_4C5940());
        FatalErrorMsg_4DB410(msg);
    }
    if (pLocal->field_D_script_sum != pRemote->field_D_script_sum)
    {
        sprintf(msg, "Player %s: Script file '%s' is different", name, gGameSession_67E8E0.GetScriptName_4C5960());
        FatalErrorMsg_4DB410(msg);
    }
    if (pLocal->field_11_exe_a != pRemote->field_11_exe_a || pLocal->field_15_exe_b != pRemote->field_15_exe_b)
    {
        sprintf(msg, "Player %s: Game file 'GTA2.EXE' is different", name);
        FatalErrorMsg_4DB410(msg);
    }
    if (pLocal->field_19_gci_sum != pRemote->field_19_gci_sum)
    {
        // The original passes the script name too, which the format doesn't use
        sprintf(msg, "Player %s: Car handling file 'nyc.gci' is different", name, gGameSession_67E8E0.GetScriptName_4C5960());
        FatalErrorMsg_4DB410(msg);
    }
}

MATCH_FUNC(0x4DACB0)
EXPORT void Net_Send_Our_Inputs_4DACB0()
{
    if (gNetPlay_7071E8.GetPlayerCount_461DA0() > 1)
    {
        if (bDo_sync_check_67D6C1)
        {
            // Send the inputs plus an rng value the other players check against
            s16 max = 100;
            gCurrentNetInputs_6F57D8.field_0_inputs[gNetworkPlayerIdx_6F56C8].field_4_rng = gRng_6F6784.get_int_4F7AE0(max);
            gInputSendData_6F5B18.field_0_data = &gCurrentNetInputs_6F57D8.field_0_inputs[gNetworkPlayerIdx_6F56C8];
            gInputSendData_6F5B18.field_4_len = sizeof(Network_InputData);
        }
        else
        {
            gInputSendData_6F5B18.field_0_data = &gCurrentNetInputs_6F57D8.field_0_inputs[gNetworkPlayerIdx_6F56C8];
            gInputSendData_6F5B18.field_4_len = sizeof(u32);
        }
        gNetPlay_7071E8.SendToAll_521B20(&gInputSendData_6F5B18);
    }
}

MATCH_FUNC(0x4DAD50)
EXPORT void Net_Set_Local_Player_Inputs_4DAD50()
{
    for (u32 player_idx = 0; player_idx < MAX_PLAYERS; player_idx++)
    {
        Player* pPlayer = gGame_0x40_67E008->GetPlayer_4219E0(player_idx);
        if (pPlayer && pPlayer->GetInUse_461DB0())
        {
            pPlayer->SetInputs_565740(gPrevNetInputs_6F5B28.field_0_inputs[player_idx].field_0_Inputs);
            gPrevNetInputs_6F5B28.field_0_inputs[player_idx].field_0_Inputs &= ~0x3FF000;
        }
    }
}

MATCH_FUNC(0x4DADA0)
EXPORT void ManageNetworkTimeAndHudPager_4DADA0()
{
    if (gNetTimeLimitEnabled_6F58A4)
    {
        if (++gFramesOfCurrSecond_6F5858 >= 30) // If a second has passed
        {
            gFramesOfCurrSecond_6F5858 = 0;
            if (--gNetTimeSecondsOfCurrMin_6F58A0 < 0) // If a minute has passed
            {
                gNetTimeSecondsOfCurrMin_6F58A0 = 59;
                if (--gNetTimeLimit_6F573C < 0) // If time is out
                {
                    gNetTimeLimitEnabled_6F58A4 = 0;
                    gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("g_over"), hud_message_priority::important_3);
                    gGame_0x40_67E008->ExitGameNoBonus_4B8C00(2, GameExitType::MultiplayerExit_5);
                }
            }
        }

        // Now do HUD Pager stuff showing the remaining time

        s32 seconds = gNetTimeSecondsOfCurrMin_6F58A0;
        s32 minutes = gNetTimeLimit_6F573C;
        s32 rem = minutes % 5;
        bool bShow = false;
        if (minutes == 0 || (rem == 4 && seconds >= 50) || (rem == 0 && seconds == 0) ||
            (minutes == gGameSession_67E8E0.GetTimeLimit_461DC0() && seconds == 0) ||
            (minutes == gGameSession_67E8E0.GetTimeLimit_461DC0() - 1 && seconds >= 50))
        {
            // bShow is set in both arms (not once before the if): with a single store, VC6 sees bl == 1 at the
            // blink test and uses "test dl, bl" there instead of "test dl, 1"
            if (!gNetTimerFlashing_6F59C0)
            {
                bShow = true;
                gNetTimerBlinkCounter_6F5B74 = 59;
                gNetTimerFlashing_6F59C0 = 1;
            }
            else
            {
                bShow = true;
            }
        }
        if (!bShow)
        {
            gNetTimerFlashing_6F59C0 = 0;
            gNetTimerBlinkCounter_6F5B74 = 0;
        }
        else
        {
            if (gNetTimerBlinkCounter_6F5B74 > 0)
            {
                if (((gNetTimerBlinkCounter_6F5B74 / 5) & 1) == 0)
                {
                    bShow = false;
                }
                gNetTimerBlinkCounter_6F5B74--;
            }

            if (bShow)
            {
                gNetTimerSeconds_6F5944 = minutes * 60 + seconds;
                if (gHudTimerIdx_6F5860 == -1)
                {
                    gHudTimerIdx_6F5860 = gHud_2B00_706620->field_620_pagers.CreateTimer_5D31F0(gNetTimerSeconds_6F5944);
                }
                return;
            }
        }

        if (gHudTimerIdx_6F5860 != -1)
        {
            gHud_2B00_706620->field_620_pagers.ClearPager_5D3280(gHudTimerIdx_6F5860);
            gHudTimerIdx_6F5860 = -1;
        }
    }
}

MATCH_FUNC(0x4DAF30)
EXPORT void __stdcall do_network_and_local_inputs_4DAF30()
{
    if (bStartNetworkGame_7081F0)
    {
        ManageNetworkTimeAndHudPager_4DADA0();
        gTagGame_6F8450.ExecuteGamemodeTick_516660();

        if (!gGame_0x40_67E008->UpdateExitTimer_4B8C20())
        {
            if (gNetDisconnectPlayersBits_6F56B8)
            {
                u32 player_idx = 0;
                do
                {
                    if ((gNetDisconnectPlayersBits_6F56B8 & 1) != 0)
                    {
                        Player* pPlayer = gGame_0x40_67E008->GetPlayer_4219E0(player_idx);
                        if (pPlayer && pPlayer->GetInUse_461DB0())
                        {
                            gNetPlay_7071E8.DeletePlayerFromGroup_521000(player_idx);
                            ShowPlayerLeftMessage_4DB070(player_idx);
                            if (!player_idx || player_idx == gNetworkPlayerIdx_6F56C8)
                            {
                                gPlayerQuit_6F5AEC = true;
                            }
                        }
                    }
                    player_idx++;
                    gNetDisconnectPlayersBits_6F56B8 >>= 1;
                } while (gNetDisconnectPlayersBits_6F56B8);
            }

            gCurrentNetInputs_6F57D8.field_0_inputs[gNetworkPlayerIdx_6F56C8].field_0_Inputs = gInputRecorder_67F8B0.get_input_bits_4CEAC0();
            Net_Send_Our_Inputs_4DACB0();
            Draw_4DA7B0();
            Net_Receive_Players_Inputs_4DA9F0();
            memcpy(&gPrevNetInputs_6F5B28, &gCurrentNetInputs_6F57D8, sizeof(gPrevNetInputs_6F5B28));
        }
        else
        {
            gNetPlay_7071E8.SendPlayerIsQuitting_521D20();
        }

        if (bRecordStartTime_6F593C)
        {
            gMatchStartTime_6F5A28 = timeGetTime();
            bRecordStartTime_6F593C = false;
        }
        if (gPlayerQuit_6F5AEC)
        {
            gGame_0x40_67E008->ExitGameNoBonus_4B8C00(0, GameExitType::PlayerQuit_2);
        }
        else
        {
            Net_Set_Local_Player_Inputs_4DAD50();
        }
        ++gNetworkFrameCounter_6F5868;
    }
    else
    {
        const u32 inputs = gInputRecorder_67F8B0.get_input_bits_4CEAC0();
        gGame_0x40_67E008->field_38_local_player->SetInputs_565740(inputs);
    }
}

// todo move to another file for ordering
MATCH_FUNC(0x4DA850)
EXPORT bool RunGameFrame_4DA850()
{
    bool exit_requested = false;
    bool drew_frame = false;
    bool updated_logic = false;
    DWORD Time = timeGetTime();

    if (bStartNetworkGame_7081F0)
    {
        if (Time >= gMatchStartTime_6F5A28)
        {
            exit_requested = ExecuteGame_4DA780();
            do_network_and_local_inputs_4DAF30();
            gMatchStartTime_6F5A28 = Time + gGame_0x40_67E008->GetFrameDurationMs_4B8BB0();
        }
    }
    else
    {
        if (!max_frame_rate_626A08 && !gFramesSinceDraw_6F58D8 || Time >= gMatchStartTime_6F5A28)
        {
            updated_logic = true;
        }
        if (gFramesSinceDraw_6F58D8 >= 8u || !min_frame_rate_706C50 && gFramesSinceDraw_6F58D8)
        {
            updated_logic = false;
        }

        if (bForceGameUpdate_6F5880 || bSkip_draw_67D4EA)
        {
            updated_logic = true;
        }

        if (updated_logic)
        {
            if (!min_frame_rate_706C50)
            {
                gMatchStartTime_6F5A28 = Time;
            }
            if (bInputsPending_6F5760)
            {
                do_network_and_local_inputs_4DAF30();
                bInputsPending_6F5760 = false;
            }
            exit_requested = ExecuteGame_4DA780();
            s32 frame_ms = gGame_0x40_67E008->GetFrameDurationMs_4B8BB0();
            bForceGameUpdate_6F5880 = false;
            gMatchStartTime_6F5A28 += frame_ms;
            ++gFramesSinceDraw_6F58D8;
            bInputsPending_6F5760 = true;
        }
        else if (gFramesSinceDraw_6F58D8)
        {
            Draw_4DA7B0();
            gFramesSinceDraw_6F58D8 = 0;
            drew_frame = true;
        }
        else if (bInputsPending_6F5760)
        {
            do_network_and_local_inputs_4DAF30();
            bInputsPending_6F5760 = false;
        }
    }

    if (gFpsCounter_7055D4)
    {
        gFpsCounter_7055D4->UpdateFpsCounters_5BECF0(drew_frame, updated_logic);
    }
    return exit_requested;
}

GUID IID_DirectMusic = {1667997456u, 3197u, 4561u, {149u, 178u, 0u, 32u, 175u, 220u, 116u, 33u}};
GUID IDD_IDirectMusic = {1698042202u, 31533u, 4562u, {186u, 24u, 0u, 0u, 248u, 117u, 172u, 18u}};

// todo move to another file for ordering
MATCH_FUNC(0x4A0650)
EXPORT void __stdcall GetDirectMusicVer_4A0650()
{
    HMODULE hDMusic;
    IUnknown* pDMusic;

    gDMusicVer_67BD32 = 0;
    hDMusic = LoadLibraryA("DMUSIC.DLL");
    if (hDMusic)
    {
        if (CoCreateInstance(IID_DirectMusic, NULL, CLSCTX_INPROC_SERVER, IDD_IDirectMusic, reinterpret_cast<LPVOID*>(&pDMusic)) >= 0)
        {
            gDMusicVer_67BD32 = 0x601;
            pDMusic->Release();
        }
        FreeLibrary(hDMusic);
    }
}

GUID IID_IDirectDrawSurface4_ = {0x0B2B8630, 0xAD35, 0x11D0, 0x8E, 0xA6, 0x00, 0x60, 0x97, 0x97, 0xEA, 0x5B};

// todo move to another file for ordering
MATCH_FUNC(0x4C4EC0)
EXPORT void __stdcall GetDirectXVersion_4C4EC0(u32* pDXVer, u32* osKind)
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
    GetDirectMusicVer_4A0650();
    if (gDMusicVer_67BD32 == 0x601)
    {
        *pDXVer = 0x601;
    }
}

const char_type* kDefaultDllNames_626A00[2] = {"d3ddll.dll", "dmavideo.dll"};

// todo move to another file for ordering
MATCH_FUNC(0x5D90E0)
EXPORT void __stdcall Video_Render_Inits_5D90E0()
{
    gRenderdevice_706998 = gRegistry_6FF968.Get_Screen_Setting_5870D0("renderdevice", 1);
    gVideodevice_70694C = gRegistry_6FF968.Get_Screen_Setting_5870D0("videodevice", 1);
    strcpy(gRenderDllName_7067F0, kDefaultDllNames_626A00[0]);
    strcpy(gVideoDllName_706654, kDefaultDllNames_626A00[1]);
    gRegistry_6FF968.Get_Screen_String_5871E0("rendername", reinterpret_cast<BYTE*>(gRenderDllName_7067F0), 0xFFu);
    gRegistry_6FF968.Get_Screen_String_5871E0("videoname", reinterpret_cast<BYTE*>(gVideoDllName_706654), 0xFFu);

    if (strcmp(gRenderDllName_7067F0, "softdll.dll") == 0)
    {
        gBufferMode_706B34 = 0;
    }
    else if (strcmp(gRenderDllName_7067F0, "3dfx.dll") == 0)
    {
        gBufferMode_706B34 = 2;
    }
    else
    {
        gBufferMode_706B34 = 1;
    }

    Init_FrameRateLightingAndExplodingScores_5D8EB0();
    ReadScreenSettings_5D8F70();
}

// todo move to another file for ordering
MATCH_FUNC(0x5D9280)
EXPORT bool IsFullScreen_5D9280()
{
    return gStartMode_626A0C == 1;
}

// todo move to another file for ordering
MATCH_FUNC(0x5D8E70)
EXPORT void __stdcall UpdateWinXY_5D8E70()
{
    if (!IsFullScreen_5D9280())
    {
        RECT winRec;
        GetWindowRect(gHwnd_707F04, &winRec);
        gWindowX_706B60 = winRec.left;
        gWindowY_706B64 = winRec.top;
    }
}

// todo move to another file for ordering
MATCH_FUNC(0x5D7CA0)
EXPORT void __stdcall j_gbh_init_5D7CA0()
{
    pgbh_Init(0);
}

// todo move to another file for ordering
MATCH_FUNC(0x4DA700)
EXPORT void __stdcall CleanUpInputAndOthers_4DA700()
{
    gInputRecorder_67F8B0.Shutdown_4CEA00();
    if (gGame_0x40_67E008)
    {
        GTA2_DELETE_AND_NULL(gGame_0x40_67E008);
    }
    gRoot_sound_66B038.Set3DSound_40F160(0);
}

// todo move to another file for ordering
MATCH_FUNC(0x4DA390)
EXPORT s32 __stdcall CountTokens_4DA390(char_type* pStr)
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

// todo move to another file for ordering
MATCH_FUNC(0x4DA3F0)
EXPORT char_type* __stdcall SkipToNextToken_4DA3F0(char_type* pStr)
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

// todo move to another file for ordering
MATCH_FUNC(0x4DA320)
EXPORT void __stdcall ParseCommandLine_4DA320(char_type* pCommandLine)
{
    char_type* pIter = pCommandLine;
    s32 len = CountTokens_4DA390(pCommandLine);
    _strupr(pCommandLine);
    while (len > 0)
    {
        if (*pIter == '-' || *pIter == '/')
        {
            const char_type cmd_char = *(++pIter);
            switch (cmd_char)
            {
                case 'R':
                    bPlay_replay_67D4F4 = true;
                    break;

                case 'Q':
                    bConstant_replay_save_67D5C4 = true;
                    break;

                case 'N':
                    bStartNetworkGame_7081F0 = 1;
                    break;

                default:
                    break;
            }
        }

        pIter = SkipToNextToken_4DA3F0(pIter);
        --len;
    }
}

// The original passes an uninitialised lParam to PostMessageA, keep it for matching
#pragma warning(push)
#pragma warning(disable : 4700)
MATCH_FUNC(0x5E4DE0)
EXPORT void Start_GTA2Manager_5E4DE0()
{
    ShowWindow(gHwnd_707F04, SW_SHOWMINNOACTIVE);
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
            PostMessageA(gHwnd_707F04, WM_ACTIVATE, 0, reinterpret_cast<LPARAM>(hwndGta2Manager));
            if (!SetForegroundWindow(hwndGta2Manager))
            {
                FatalError_4A38C0(Gta2Error::Gta2ManagerLaunchError, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 125);
            }
        }
    }
    else
    {
        LPARAM lParam;
        PostMessageA(gHwnd_707F04, WM_ACTIVATE, 0, lParam);
        if (reinterpret_cast<u32>(ShellExecuteA(NULL, NULL, "GTA2 Manager.EXE", NULL, gWorkingDir_707F64, SW_SHOWNORMAL)) <= 0x20)
        {
            FatalError_4A38C0(Gta2Error::Gta2ManagerLaunchError, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 136);
        }
    }
}
#pragma warning(pop)

MATCH_FUNC(0x5E4EC0)
EXPORT void __stdcall ErrorMsgBox_5E4EC0(LPCSTR lpText)
{
    MessageBoxA(gHwnd_707F04, lpText, "Error!", MB_OK);
}

// todo: move
MATCH_FUNC(0x5D9230)
EXPORT void __stdcall SetStartMode_5D9230(s32 startMode)
{
    gStartMode_626A0C = startMode;
    gRegistry_6FF968.Set_Screen_Setting_587170("start_mode", startMode);
}

// todo: move
MATCH_FUNC(0x498D10)
EXPORT void Input_Read_498D10()
{
    if (gInputDevices_67B990)
    {
        gInputDevices_67B990->flush_keyboard_and_gamepad_498CC0();
    }
}

// todo: move
MATCH_FUNC(0x5D9250)
EXPORT void __stdcall ToggleStartMode_5D9250()
{
    gStartMode_626A0C = (gStartMode_626A0C != 1);
    gRegistry_6FF968.Set_Screen_Setting_587170("start_mode", gStartMode_626A0C);
}

MATCH_FUNC(0x5E4EE0)
EXPORT LRESULT __stdcall WindowProc_5E4EE0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
            if (gFrontend_67DC84)
            {
                Bink::ResetBufferOffset_513720();
            }
            break;
        }

        case WM_SETFOCUS:
            gRoot_sound_66B038.Reacquire_40F140();
            gRoot_sound_66B038.SetCDVol_40F0F0(gRegistry_6FF968.Get_Sound_Setting_586AE0("CDVol", 127));
            gRoot_sound_66B038.SetSfxVol_40F0B0(gRegistry_6FF968.Get_Sound_Setting_586AE0("SFXVol", 127));

            if (bDoFrontEnd_626B68)
            {
                gRoot_sound_66B038.Set3DSound_40F160(0);
            }
            else
            {
                gRoot_sound_66B038.Set3DSound_40F160(gRegistry_6FF968.Sound_Setting_Exists_586A70("do_3d_sound"));
                gRegistry_6FF968.Clear_Or_Delete_Sound_Setting_586BF0("do_3d_sound", gRoot_sound_66B038.Get3DSound_40F180());
            }

            if (!bDestroyed_6F5B70)
            {
                bSetFocusInput = 1;
                Frontend::SetInputEnabled_5E53C0(&bSetFocusInput);
                if (!bDoFrontEnd_626B68)
                {
                    Input_Read_498D10();
                }

                if (gVidSys_7071D0 && !Bink::IsUsingDDBuffer_513770())
                {
                    SetVideoModeFromSettings_5D92D0();
                    OnVideoModeChanged_5D9680();
                    bVideoModeResetPending_706C5D = false;
                }

                if (gGame_0x40_67E008)
                {
                    gGame_0x40_67E008->ReinitAfterVideoModeChange_4B9720();
                }

                SetSavedGamma_5D98E0();
            }
            break;

        case WM_KILLFOCUS:
            {
                bKillFocusInput = 0;
                Frontend::SetInputEnabled_5E53C0(&bKillFocusInput);
                Input_ReleaseMouse_5D7C70();
                gRoot_sound_66B038.Set3DSound_40F160(0);
                gRoot_sound_66B038.Release_40F130();

                if (gFrontend_67DC84 && Bink::IsDDBufferOpen_513760())
                {
                    Bink::CloseSlot1_513340();
                    Bink::CloseSlot2_513390();
                    gFrontend_67DC84->ChangeMenuPage_4B3170(MENUPAGE_START_MENU);
                }

                if (gVidSys_7071D0)
                {
                    if (!Vid_FindDevice_5D9290())
                    {
                        pVid_CloseScreen(gVidSys_7071D0);
                        bVideoModeResetPending_706C5D = true;
                        ShowWindow(gHwnd_707F04, SW_SHOWMINNOACTIVE);
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
                    Frontend::SetInputEnabled_5E53C0(&bActiveInput);
                    Input_MouseAcquire_5D7C60();
                }
                break;

                case WA_INACTIVE:
                {
                    bInactiveInput = 0;
                    Frontend::SetInputEnabled_5E53C0(&bInactiveInput);
                    Input_ReleaseMouse_5D7C70();
                }
                break;
            }
            break;

        case WM_SIZE:
            switch (wParam)
            {
                case SIZE_RESTORED:
                    gMinimizedState_70827C = 0;
                    gRoot_sound_66B038.Reacquire_40F140();
                    break;

                case SIZE_MINIMIZED:
                    gMinimizedState_70827C = 2;
                    gRoot_sound_66B038.Release_40F130();
                    break;

                case SIZE_MAXIMIZED:
                    gMinimizedState_70827C = 0;
                    gRoot_sound_66B038.Reacquire_40F140();
                    break;
            }
            break;

        case WM_DESTROY:
            if (bStartNetworkGame_7081F0)
            {
                gNetPlay_7071E8.Disconnect_520D10();
            }

            ReleaseMutex(gMutex_707078);
            CloseHandle(gMutex_707078);
            gMutex_707078 = NULL;

            Shutdown_4DA740();
            GBH_Graphis_DMA_Video_Free_5D9830();
            PostQuitMessage(0);
            break;

        case WM_WINDOWPOSCHANGING:
        {
            WINDOWPOS* pPos = reinterpret_cast<WINDOWPOS*>(lParam);
            if (gFrontend_67DC84 && (pPos->flags & SWP_NOMOVE) == 0)
            {
                newX = pPos->x;
                newY = pPos->y;
                Bink::CheckWindowPos_5136D0(&newX, &newY);
                pPos->x = newX;
                pPos->y = newY;
            }
            break;
        }

        case WM_SYSKEYDOWN:
            switch (wParam)
            {
                case VK_RETURN:
                    if (IsWindowedModeAvailable_5D92C0())
                    {
                        UpdateWinXY_5D8E70();
                        // The previous key state. The value is 1 if the key is down before the message is sent, or it is 0 if the key is up.
                        if ((lParam & 0x20000000) != 0)
                        {
                            ToggleStartMode_5D9250();
                            SetVideoModeFromSettings_5D92D0();
                            OnVideoModeChanged_5D9680();
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
                    UpdateWinXY_5D8E70();
                    if (gMinimizedState_70827C != 2)
                    {
                        SetStartMode_5D9230(1u);
                        SetVideoModeFromSettings_5D92D0();
                        OnVideoModeChanged_5D9680();
                    }
                    break;

                case SC_MINIMIZE:
                    UpdateWinXY_5D8E70();
                    break;

                default:
                    break;
            }
            break;

        case WM_SIZING:
        {
            RECT* pDragRect = reinterpret_cast<RECT*>(lParam);
            GetWindowRect(gHwnd_707F04, &winRec);
            pDragRect->left = winRec.left;
            pDragRect->top = winRec.top;
            pDragRect->bottom = winRec.bottom;
            pDragRect->right = winRec.right;
            return 0;
        }

        default:
            break;
    }

    if (gVidSys_7071D0)
    {
        pVid_WindowProc(gVidSys_7071D0, hWnd, Msg, wParam, lParam);
    }

    return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

MATCH_FUNC(0x5E53F0)
s32 __stdcall WinMain_5E53F0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    gHInstance_708220 = hInstance;
    if (CoInitialize(NULL) < 0)
    {
        return 0;
    }

    if (OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "GBH_COOP_MUTEX"))
    {
        return 0;
    }

    gMutex_707078 = CreateMutexA(NULL, FALSE, "GBH_COOP_MUTEX");
    GetGTA2Version_5E5D60(&gGTA2VersionMajor_708280, &gGTA2VersionMinor_708284);

    u32 dxVer;
    u32 osKind;
    GetDirectXVersion_4C4EC0(&dxVer, &osKind);

    if (dxVer < 0x601)
    {
        ErrorMsgBox_5E4EC0("ERROR: Incorrect version of DirectX");
        ReleaseMutex(gMutex_707078);
        CloseHandle(gMutex_707078);
        return -1;
    }

    gDebug_67D52C.Init_4ABBD0();
    gGameSession_67E8E0.LoadDebugSettings_4C53D0();
    bDoFrontEnd_626B68 = bSkip_frontend_67D53B == 0;

    WNDCLASSA WndClass;
    WndClass.style = CS_VREDRAW | CS_HREDRAW;
    WndClass.lpfnWndProc = WindowProc_5E4EE0;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(0x65));
    WndClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    WndClass.hbrBackground = NULL;
    WndClass.lpszMenuName = "WinMain";
    WndClass.lpszClassName = "WinMain";
    RegisterClassA(&WndClass);

    Video_Render_Inits_5D90E0();

    _getcwd(gWorkingDir_707F64, 256);

    ParseCommandLine_4DA320(lpCmdLine);
    if (!Start_NetworkGame_5E5A30(hInstance)) // start NetUI if multiplayer. Stays here while players don't click "start", "cancel" etc
    {
        ReleaseMutex(gMutex_707078);
        CloseHandle(gMutex_707078);
        return 0;
    }

    // Starting game...

    sprintf(gTmpBuffer_67C598, "GTA2");
    gHwnd_707F04 = CreateWindowExA(0, "WinMain", gTmpBuffer_67C598, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, hInstance, NULL);
    if (!gHwnd_707F04)
    {
        return 0;
    }

    ShowWindow(gHwnd_707F04, nShowCmd);
    UpdateWindow(gHwnd_707F04);
    RECT clientRec;
    GetClientRect(gHwnd_707F04, &clientRec);
    RECT windowRec;
    GetWindowRect(gHwnd_707F04, &windowRec);

    s32 window_w = window_width_706630 + (windowRec.right - windowRec.left) - (clientRec.right - clientRec.left);
    s32 window_h = window_height_706B50 + (windowRec.bottom - windowRec.top) - (clientRec.bottom - clientRec.top);

    if (bDo_corner_window_67D4EE)
    {
        gWindowX_706B60 = 0;
        gWindowY_706B64 = 0;
    }
    else
    {
        gWindowX_706B60 = static_cast<s32>(GetSystemMetrics(SM_CXSCREEN) - window_w) / 2;
        gWindowY_706B64 = static_cast<s32>(GetSystemMetrics(SM_CYSCREEN) - window_h) / 2;
    }

    if (!SetWindowPos(gHwnd_707F04, NULL, gWindowX_706B60, gWindowY_706B64, window_w, window_h, 0x314u))
    {
        FatalError_4A38C0(Gta2Error::SetWindowPosFail, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 661);
    }

    Input::DirectInputCreate_4986D0(gHInstance_708220);
    InitVideo_5D96C0();

    ShowWindow(gHwnd_707F04, nShowCmd);
    UpdateWindow(gHwnd_707F04);
    j_gbh_init_5D7CA0();
    Init_keybrd_jolly_and_sound_4DA440();

    s32 state;
    if (Frontend::intro_bik_exists_4B5FF0())
    {
        state = gRegistry_6FF968.Get_Screen_Setting_5870D0("do_play_movie", 1) != 1 ? 0 : 8;
    }
    else
    {
        state = 0;
    }

    if (bIsFrench_67D53C)
    {
        state = 14;
    }

    if (bStartNetworkGame_7081F0)
    {
        bDoFrontEnd_626B68 = 0;
        gRoot_sound_66B038.Set3DSound_40F160(gRegistry_6FF968.Sound_Setting_Exists_586A70("do_3d_sound"));
        gRegistry_6FF968.Clear_Or_Delete_Sound_Setting_586BF0("do_3d_sound", gRoot_sound_66B038.Get3DSound_40F180());
    }

    // Game loop

    bool bQuit = false;

    while (true)
    {
        bQuit = false;

        if (bDoFrontEnd_626B68)
        {
            Frontend::create_4ACFA0();
            gFrontend_67DC84->ChangeMenuPage_4B3170(state);
        }
        else
        {
            InitializeGame_4DA4D0();
            if (bStartNetworkGame_7081F0 && !gNetPlay_7071E8.WaitForPlayersSync_5213E0())
            {
                CoUninitialize();
                return 0;
            }
        }

        UpdateWinXY_5D8E70();
        ApplyChangedScreenSettings_5D9690();

        while (true)
        {
            MSG msg;
            if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    Input::DInputRelease_498710();
                    return msg.wParam;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            else
            {
                if (!bQuit && gMinimizedState_70827C != 2 && !bVideoModeResetPending_706C5D)
                {
                    if (bDoFrontEnd_626B68)
                    {
                        s32 t = gFrontend_67DC84->Run_4AEDB0();

                        if (t == Quit_1)
                        {
                            bQuit = true;
                            Frontend::destroy_4AD070();
                            DestroyWindow(gHwnd_707F04);
                            continue; // go to PeekMessageA
                        }
                        else if (t == Start_Game_3)
                        {
                            Frontend::destroy_4AD070();
                            bDoFrontEnd_626B68 = false;
                            break; // go to the beginning
                        }
                        else if (t == Start_Replay_4)
                        {
                            Frontend::destroy_4AD070();
                            bDoFrontEnd_626B68 = false;
                            bReplayMode_6F5B71 = true;
                            break; // go to the beginning
                        }
                        else
                        {
                            continue; // go to PeekMessageA
                        }
                    }
                    bQuit = RunGameFrame_4DA850();

                    if (bQuit)
                    {
                        if (bSkip_frontend_67D53B)
                        {
                            DestroyWindow(gHwnd_707F04);
                        }
                        else
                        {
                            if (bStartNetworkGame_7081F0)
                            {
                                switch (gGame_0x40_67E008->GetExitType_4D09C0())
                                {
                                    case 1:
                                        DestroyWindow(gHwnd_707F04);
                                        break;

                                    default:
                                        state = 7;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;
                                }
                                break; // go to the beginning
                            }
                            else
                            {
                                switch (gGame_0x40_67E008->GetExitType_4D09C0())
                                {
                                    case GameExitType::CloseGameImmediately_1:
                                        DestroyWindow(gHwnd_707F04);
                                        break;

                                    case GameExitType::PlayerQuit_2:
                                        gGameSession_67E8E0.StoreCrimeStats_4C5A10(gGame_0x40_67E008->field_38_local_player);
                                        gPlyDat_6FEAC0->UpdateStageScore_56BB10(gGame_0x40_67E008->field_38_local_player);
                                        gPlyDat_6FEAC0->UpdateHiScores_56C010();

                                        state = gGameSession_67E8E0.IsBonusStage_4C59A0() != 0 ? 6 : 11; // 11? prob 1
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    case GameExitType::GameOverRIP_3:
                                        gGameSession_67E8E0.StoreCrimeStats_4C5A10(gGame_0x40_67E008->field_38_local_player);
                                        gPlyDat_6FEAC0->UpdateStageScore_56BB10(gGame_0x40_67E008->field_38_local_player);
                                        gPlyDat_6FEAC0->UpdateHiScores_56C010();
                                        state = gGameSession_67E8E0.IsBonusStage_4C59A0() != 0 ? 6 : 2;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    case GameExitType::AreaCompleted_4:
                                        gGameSession_67E8E0.StoreCrimeStats_4C5A10(gGame_0x40_67E008->field_38_local_player);
                                        gPlyDat_6FEAC0->UpdateStageScore_56BB10(gGame_0x40_67E008->field_38_local_player);
                                        gPlyDat_6FEAC0->UpdateHiScores_56C010();
                                        state = gGameSession_67E8E0.IsBonusStage_4C59A0() != 0 ? 6 : 3;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    case GameExitType::MultiplayerExit_5:
                                        state = 7;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    case GameExitType::ReplayExit_6:
                                        state = 0;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
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

// TODO: order
MATCH_FUNC(0x4DA740)
EXPORT void __stdcall Shutdown_4DA740()
{
    if (!bDestroyed_6F5B70)
    {
        bDestroyed_6F5B70 = 1;
        KeyboardLayout::destroy_4D5FA0();
        PlyDat::Destroy_56C340();
        CleanUpInputAndOthers_4DA700();
        Frontend::destroy_4AD070();

        if (!bSkip_audio_67D6BE)
        {
            gRoot_sound_66B038.DeinitializeAudio_40EF90();
        }
    }
}

MATCH_FUNC(0x5E5A30)
EXPORT bool __stdcall Start_NetworkGame_5E5A30(HINSTANCE hInstance)
{
    bool bRet = true;
    gGameSession_67E8E0.init_4C5AF0();
    if (bStartNetworkGame_7081F0)
    {
        HKEY hKey;
        if (!gRegistry_6FF968.CreateNetworkRoot_587420(&hKey))
        {
            bRet = false;
        }

        if (gRegistry_6FF968.Get_Binary_Size_5873E0(hKey, "UseProtocol") != sizeof(GUID))
        {
            RegCloseKey(hKey);
            if (!gNetPlay_7071E8.SetProtoAndConnection_51DAE0(NULL, NULL))
            {
                return false;
            }
        }
        else
        {
            GUID protocolGuid;
            if (!gRegistry_6FF968.Get_Binary_587340(hKey, "UseProtocol", sizeof(GUID), reinterpret_cast<LPBYTE>(&protocolGuid)))
            {
                RegCloseKey(hKey);
                return false;
            }

            Network_Buffer connection;
            connection.field_4_len = gRegistry_6FF968.Get_Binary_Size_5873E0(hKey, "UseConnection");
            connection.field_0_data = operator new(connection.field_4_len);
            if (!gRegistry_6FF968.Get_Binary_587340(hKey, "UseConnection", connection.field_4_len, static_cast<LPBYTE>(connection.field_0_data)))
            {
                operator delete(connection.field_0_data);
                RegCloseKey(hKey);
                return false;
            }
            RegCloseKey(hKey);

            if (!gNetPlay_7071E8.SetProtoAndConnection_51DAE0(&protocolGuid, &connection))
            {
                operator delete(connection.field_0_data);
                return false;
            }

            if (gNetPlay_7071E8.field_4_bModem)
            {
                u16 len = gRegistry_6FF968.Get_Binary_Size_5873E0(hKey, "ModemNumber");
                if (len >= 128)
                {
                    return false;
                }
                wchar_t modemNumber[128];
                gRegistry_6FF968.Get_Binary_587340(hKey, "ModemNumber", len, reinterpret_cast<LPBYTE>(modemNumber));
                gNetPlay_7071E8.field_5_modem_num = modemNumber[0] != 0;
            }
        }

        NetUi networkUi;
        if (!gNetPlay_7071E8.InitializeConnection_51E5C0())
        {
            return false;
        }

        if (!networkUi.ShowNetworkUiBlocking_519BD0(hInstance))
        {
            return false;
        }

        networkUi.CopyGameSettings_51C7F0(&gNetworkGameSettings_707098);

        char_type path[MAX_PATH];
        wsprintfA(path, "data\\%s", networkUi.GetMapName_51CA10());
        gGameSession_67E8E0.SetMapName_4C5870(path);
        wsprintfA(path, "data\\%s", networkUi.GetMapStyName_51CA50());
        gGameSession_67E8E0.SetStyleName_4C5890(path);
        wsprintfA(path, "data\\%s", networkUi.GetMapScrName_51CA90());
        gGameSession_67E8E0.SetScriptName_4C58B0(path);

        gGameSession_67E8E0.SetMultiplayerParams_4C5B80(gNetworkGameSettings_707098.field_20198_game_type,
                                                           gNetworkGameSettings_707098.field_20194_frag_limit,
                                                           gNetPlay_7071E8.GetPlayerIdx_409C40(),
                                                           gNetPlay_7071E8.CountActivePlayers_521350(),
                                                           gNetworkGameSettings_707098.field_201A4_game_time_limit);
    }
    return bRet;
}

#pragma comment(lib, "Version.lib")

MATCH_FUNC(0x5E5D60)
EXPORT void __stdcall GetGTA2Version_5E5D60(s32* pVerMinor, s32* pVerMajor)
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