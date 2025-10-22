#include "winmain.hpp"
#include "Bink.hpp"
#include "BurgerKing_67F8B0.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "NetPlay.hpp"
#include "debug.hpp"
#include "dma_video.hpp"
#include "error.hpp"
#include "gbh_graphics.hpp"
#include "input.hpp"
#include "jolly_poitras_0x2BC0.hpp"
#include "keybrd_0x204.hpp"
#include "lucid_hamilton.hpp"
#include "registry.hpp"
#include "resource.h"
#include "root_sound.hpp"
#include "sharp_bose_0x54.hpp"
#include <ddraw.h>
#include <direct.h>
#include <stdio.h>
#include <windows.h>
//#include <dmusics.h>

// for force links
#include "3rdParty/GTA2Hax/d3ddll/d3ddll.hpp"
#include "Ambulance_110.hpp"
#include "BurgerKing_67F8B0.hpp"
#include "Car_BC.hpp"
#include "sprite.hpp"
#include "DrawUnk_0xBC.hpp"
#include "Fix16_Rect.hpp"
#include "Garox_2B00.hpp"
#include "Hamburger_500.hpp"
#include "ImGuiDebug.hpp"
#include "Maccies_14AC.hpp"
#include "MapRenderer.hpp"
#include "Mike_A80.hpp"
#include "Montana.hpp"
#include "Network_20324.hpp"
#include "Ped.hpp"
#include "Phi_8CA8.hpp"
#include "Player.hpp"
#include "Rozza_C88.hpp"
#include "Shooey_CC.hpp"
#include "Snooky_94.hpp"
#include "Tango_28.hpp"
#include "char.hpp"
#include "collide.hpp"
#include "miss2_8.hpp"
#include "nostalgic_ellis_0x28.hpp"
#include "sound_obj.hpp"
#include "cSampleManager.hpp"


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

static void LoadBeginSceneCBPtr()
{
    static DllRaii hD3Ddll("d3ddll.dll"); // freed after WinMain return, only if this func is called

    pBeginSceneCB = (T_gbh_SetBeginSceneCB)hD3Ddll.GetProcAddress("gbh_SetBeginSceneCB");
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
    Phi_8CA8 phi_8ca8;
    phi_8ca8.sub_5332D0(0, 0, 0, 0);

    Ambulance_20 jaw_20;
    jaw_20.field_14_count = 1;

    Rozza_C88 rozza;
    rozza.field_0_pSoundObj = 0;

    Maccies_14AC maccies;
    maccies.sub_4C1CD0(0);

    Sprite car_3c;
    car_3c.field_20_id = 5;

    Snooky_94 snooky;
    snooky.sub_4887A0(0);

    Network_20324 network;
    network.cb_sub_519D30(0, 0);

    nostalgic_ellis_0x28 nostalgic;
    nostalgic.sub_4D6D70();

    Char_C char_c;
    char_c.DoIanTest_471060(0);

    Player lewin(1);
    u8 zero_u8 = 0;
    lewin.GetIdx_4881E0();
    lewin.sub_5645B0(0);
    lewin.sub_564610(0, 0);
    lewin.sub_564680(0);
    lewin.sub_564710(0, 0);
    lewin.sub_564790(0);
    lewin.sub_5647D0();
    lewin.sub_5648F0();
    lewin.sub_564910(0);
    lewin.HasAnyAmmo_564940();
    lewin.sub_564960(0, 0);
    lewin.sub_5649D0(0, 0);
    lewin.sub_564AD0(0);
    lewin.sub_564B60();
    lewin.sub_564B80();
    lewin.sub_564C00();
    lewin.RemovePlayerWeapons_564C50();
    lewin.sub_564CC0();
    lewin.sub_564CF0();
    lewin.sub_564D60(0);
    lewin.tick_down_powerups_565070();
    lewin.sub_5651F0(0);
    lewin.sub_565310();
    lewin.sub_5653E0();
    lewin.sub_565490(0);
    lewin.SetInputs_565740(0);
    lewin.sub_565770(0);
    lewin.sub_565860();
    lewin.sub_565890(0);
    lewin.sub_566380(0);
    lewin.sub_566520();
    lewin.sub_566820();
    lewin.sub_5668D0(0);
    lewin.sub_566C30(0);
    lewin.sub_566C80(0);
    lewin.sub_566EE0(0);
    lewin.sub_5670B0();
    lewin.sub_567130();
    lewin.sub_567850();
    lewin.sub_5679E0();
    lewin.sub_568520();
    lewin.sub_568630();
    lewin.sub_568670();
    lewin.sub_5686D0(0);
    lewin.sub_568730();
    lewin.sub_5687F0();
    lewin.sub_569410();
    lewin.sub_569530();
    lewin.sub_5695A0();
    lewin.sub_569600(0);
    lewin.sub_5696D0(0);
    lewin.sub_569840(zero_u8, zero_u8, zero_u8);
    lewin.sub_5698E0();
    lewin.sub_569920(0, 0, 0);
    lewin.sub_5699F0(0);
    lewin.sub_569A10();
    lewin.sub_569C20();
    lewin.sub_569CB0();
    lewin.sub_569E70();
    lewin.GetDeathText_569F00();
    lewin.sub_569F40();
    lewin.sub_569FF0();
    lewin.SetUnknown_56A000();
    lewin.sub_56A010();
    lewin.sub_56A020();
    lewin.sub_56A030();
    lewin.sub_56A040();
    lewin.sub_56A0F0();
    lewin.sub_56A1A0(0);
    lewin.sub_56A310(0);
    lewin.sub_56A490();
    lewin.sub_56A6D0();

    DrawUnk_0xBC drawUnk;
    drawUnk.sub_4357B0();
    drawUnk.sub_4357F0();
    drawUnk.sub_435810();
    drawUnk.sub_435830();
    drawUnk.sub_435840();
    drawUnk.sub_435860(0);
    drawUnk.sub_435D20(1, 1, 1, 1, 1, 1);
    drawUnk.sub_435DD0();
    drawUnk.sub_436110();
    drawUnk.sub_436120(1);
    drawUnk.sub_436830();
    drawUnk.sub_4397D0(Fix16(), Fix16(), Fix16(), Fix16());
    drawUnk.sub_58CF10(1, 1);

    Ped cn;
    cn.sub_45B550();
    cn.sub_45B560(NULL, 0);
    cn.sub_45B590();

    miss2_8 miss2;
    miss2.add_503160(0);
    miss2.remove_503180();
    miss2.remove_5031E0(0);

    BurgerKing_67F8B0 burgerking;
    burgerking.sub_4CDCD0();
    burgerking.sub_4CDCE0();
    burgerking.sub_4CDCF0(0);
    burgerking.sub_4CDD10(0);
    burgerking.sub_4CDD80(0);
    burgerking.sub_4CDDF0(0);
    burgerking.sub_4CDE20(0);
    burgerking.SaveReplay_4CDED0();
    burgerking.sub_4CDF30(0);
    burgerking.AppendReplayHeader_4CDF70();
    burgerking.LoadReplayHeader_4CE380(0);
    burgerking.sub_4CE650();
    burgerking.GetNextAttrReplay_4CE6E0(0);
    burgerking.sub_4CE740(0);
    burgerking.sub_4CE880(0);
    burgerking.sub_4CEA40(0);
    burgerking.sub_4CEAC0();
    burgerking.sub_4CED00(0, 0);
    burgerking.sub_4CED90();
    burgerking.RecOrPlayBackState_4CEDF0();
    burgerking.ShowInput_4CEE10();

    Hud_2B00 garox;

    MapRenderer nano;
    nano.draw_bottom_4ED290(0);

    Montana montana;
    montana.ResetAll_4954F0();

    Mike_A80 mike;
    mike.DebugDrawProfiling_4FF250();

    Shooey_14 shooey_14;
    Shooey_CC shooey_CC;

    Tango_28 tango_28;
    tango_28.sub_450C10();

    Tango_54 tango_54;

    Hamburger_500 hamburger_500;
    hamburger_500.sub_474CC0(0);

    Collide_C collide_C;

    Fix16_Rect car14_18;
    car14_18.DoSetCurrentRect_59DD60();
}

DEFINE_GLOBAL(HINSTANCE, gHInstance_708220, 0x708220);
DEFINE_GLOBAL(HANDLE, gMutex_707078, 0x707078);
DEFINE_GLOBAL(s32, gGTA2VersionMajor_708280, 0x708280);
DEFINE_GLOBAL(s32, gGTA2VersionMajor_708284, 0x708284);
DEFINE_GLOBAL_ARRAY(char_type, gWorkingDir_707F64, 652, 0x707F64);
DEFINE_GLOBAL_INIT(bool, bDoFrontEnd_626B68, true, 0x626B68);

DEFINE_GLOBAL(s32, window_width_706630, 0x706630);
DEFINE_GLOBAL(s32, window_height_706B50, 0x706B50);
DEFINE_GLOBAL(s32, full_width_706B5C, 0x706B5C);
DEFINE_GLOBAL(s32, full_height_706798, 0x706798);
DEFINE_GLOBAL(s32, gWindowX_706B60, 0x706B60);
DEFINE_GLOBAL(s32, gWindowY_706B64, 0x706B64);
DEFINE_GLOBAL(s32, bStartNetworkGame_7081F0, 0x7081F0);
DEFINE_GLOBAL(BYTE, byte_70827C, 0x70827C);
DEFINE_GLOBAL(BYTE, byte_706C5D, 0x706C5D);
DEFINE_GLOBAL(BYTE, byte_6F5B71, 0x6F5B71);
DEFINE_GLOBAL_INIT(s32, gStartMode_626A0C, 2, 0x626A0C);

DEFINE_GLOBAL(s32, bTrippleBuffer_706C54, 0x706C54);
DEFINE_GLOBAL(char_type, byte_706C5C, 0x706C5C);
DEFINE_GLOBAL(u32, gBufferMode_706B34, 0x706B34);
DEFINE_GLOBAL_ARRAY(char_type, gRenderDllName_7067F0, 256, 0x7067F0);
DEFINE_GLOBAL_ARRAY(char_type, gVideoDllName_706654, 256, 0x706654);
DEFINE_GLOBAL(s32, gVideodevice_70694C, 0x70694C);
DEFINE_GLOBAL(s32, gRenderdevice_706998, 0x706998);
DEFINE_GLOBAL(u16, gDMusicVer_67BD32, 0x67BD32);

DEFINE_GLOBAL_INIT(char_type, gLighting_626A09, 1, 0x626A09);

DEFINE_GLOBAL(u32, gLightingDrawFlag_7068F4, 0x7068F4);
DEFINE_GLOBAL(LPDIRECTINPUTDEVICEA, gMouseDevice_706C60, 0x706C60);
DEFINE_GLOBAL(DIDATAFORMAT, gMouseDataFormat_601A84, 0x601A84);

DEFINE_GLOBAL(u8, max_frame_rate_626A08, 0x626A08);
DEFINE_GLOBAL(u8, min_frame_rate_706C50, 0x706C50);

// todo move to another file for ordering
MATCH_FUNC(0x5D8EB0)
EXPORT void __stdcall Init_FrameRateLightAndUnknown_5D8EB0()
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

    if (gSprite_3CC_67AF1C)
    {
        gSprite_3CC_67AF1C->sub_48F710();
    }

    bExplodingOff_67D4FB = gRegistry_6FF968.Get_Screen_Setting_5870D0("exploding_on", 1) != 1;
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
        gMouseDevice_706C60 = 0;
    }
}

// todo: move
MATCH_FUNC(0x5D94F0)
EXPORT void __stdcall HideCursor_5D94F0()
{
    do
    {

    } while (ShowCursor(0) >= 0);
}

// todo: move
MATCH_FUNC(0x5D7BF0)
EXPORT char_type Input_InitMouse_5D7BF0()
{
    // TODO: using the HR error checking macros doesn't match here, what did they do??
    if (!gpDInput_67B804 || gMouseDevice_706C60 || gpDInput_67B804->CreateDevice(GUID_SysMouse, &gMouseDevice_706C60, 0) ||
        gMouseDevice_706C60->SetDataFormat(&gMouseDataFormat_601A84) || gMouseDevice_706C60->SetCooperativeLevel(gHwnd_707F04, 5))
    {
        return 0;
    }
    gMouseDevice_706C60->Acquire();
    return 1;
}

// todo: move
STUB_FUNC(0x5D93A0)
EXPORT void __stdcall sub_5D93A0()
{
    NOT_IMPLEMENTED;
    char_type v0; // bl
    s32 bcheckModeRet; // esi
    // SVideo *v2; // eax
    BYTE local_field_4_flags; // ecx

    v0 = 0;
    bcheckModeRet = pVid_CheckMode(gVidSys_7071D0, full_width_706B5C, full_height_706798, 16);
    if (!bcheckModeRet)
    {
        if (full_width_706B5C == 640 ||
            (full_width_706B5C = 640,
             v0 = 1,
             full_height_706798 = 480,
             (bcheckModeRet = pVid_CheckMode(gVidSys_7071D0, 640, 480, 16)) == 0))
        {
            FatalError_4A38C0(3003, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1359, full_width_706B5C, full_height_706798, 16);
        }
    }

    //v2 = gVidSys_7071D0;
    if (gVidSys_7071D0)
    {
        local_field_4_flags = gVidSys_7071D0->field_4_flags;
        //BYTE1(local_field_4_flags) |= 1u;
        local_field_4_flags |= 1u;
        gVidSys_7071D0->field_4_flags = local_field_4_flags;
        // v2 = gVidSys_7071D0;
    }

    if (pVid_SetMode(gVidSys_7071D0, gHwnd_707F04, bcheckModeRet))
    {
        FatalError_4A38C0(1037, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1365, bcheckModeRet);
    }

    HideCursor_5D94F0();

    SetWindowLongA(gHwnd_707F04, -16, 0x10000000);
    SetWindowPos(gHwnd_707F04, 0, 0, 0, 0, 0, 0x63Bu);
    UpdateWindow(gHwnd_707F04);
    ShowWindow(gHwnd_707F04, 1);

    Input_InitMouse_5D7BF0();

    if (v0)
    {
        gRegistry_6FF968.Set_Screen_Setting_587170("full_width", full_width_706B5C);
        gRegistry_6FF968.Set_Screen_Setting_587170("full_height", full_height_706798);
    }
}

// todo: move
MATCH_FUNC(0x5D92C0)
EXPORT char_type sub_5D92C0()
{
    return byte_706C5C;
}

MATCH_FUNC(0x5D9660)
EXPORT void __stdcall ShowCursor_5D9660()
{
    s32 refCount;
    do
    {
        refCount = ShowCursor(1);
    } while (refCount < 0);
}

// todo: move
STUB_FUNC(0x5D9510)
EXPORT char_type sub_5D9510()
{
    NOT_IMPLEMENTED;

    if (!sub_5D92C0())
    {
        return 0;
    }

    Input_ReleaseMouse_5D7C70();
    SetWindowLongA(gHwnd_707F04, -16, 0x10CF0000);
    SetWindowPos(gHwnd_707F04, 0, 0, 0, 0, 0, 0x63Bu);
    UpdateWindow(gHwnd_707F04);
    ShowWindow(gHwnd_707F04, SW_SHOW);

    struct tagRECT windowRect; // [esp+18h] [ebp-10h] BYREF
    GetWindowRect(gHwnd_707F04, &windowRect);

    struct tagRECT clientRect; // [esp+8h] [ebp-20h] BYREF
    GetClientRect(gHwnd_707F04, &clientRect);

    if (!SetWindowPos(gHwnd_707F04,
                      0,
                      gWindowX_706B60,
                      gWindowY_706B64,
                      windowRect.right + clientRect.left + window_width_706630 - clientRect.right - windowRect.left,
                      windowRect.bottom + clientRect.top + window_height_706B50 - clientRect.bottom - windowRect.top,
                      0x316u))
    {
        return 0;
    }

    UpdateWindow(gHwnd_707F04);
    ShowWindow(gHwnd_707F04, 5);
    if (gVidSys_7071D0)
    {
        //BYTE field_4_flags = gVidSys_7071D0->field_4_flags;
        //field_4_flags |= 1u;
        gVidSys_7071D0->field_4_flags |= 1u;
    }

    if (pVid_SetMode(gVidSys_7071D0, gHwnd_707F04, -2) == 1)
    {
        return 0;
    }
    else
    {
        ShowCursor_5D9660();
        return 1;
    }
}

// todo move to another file for ordering
MATCH_FUNC(0x5D8F70)
EXPORT s32 ReadScreenSettings_5D8F70()
{
    const u32 full_width_old = full_width_706B5C;
    const u32 window_width_old = window_width_706630;
    const u32 full_height_old = full_height_706798;
    const s32 startMode_old = gStartMode_626A0C;
    const s32 trippleBuffer_old = bTrippleBuffer_706C54;
    const u32 window_height_old = window_height_706B50;

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
STUB_FUNC(0x5D98E0)
EXPORT void __stdcall SetSavedGamma_5D98E0()
{
    NOT_IMPLEMENTED;
    // todo
}

// todo: move
STUB_FUNC(0x5D92D0)
EXPORT void __stdcall sub_5D92D0()
{
    NOT_IMPLEMENTED;
    u32 field_4_flags; // ecx
    u32 v1; // ecx

    Init_FrameRateLightAndUnknown_5D8EB0();
    ReadScreenSettings_5D8F70();
    Input_MouseAcquire_5D7C60();

    pVid_CloseScreen(gVidSys_7071D0);
    field_4_flags = gVidSys_7071D0->field_4_flags;

    if (bTrippleBuffer_706C54)
    {
        v1 = field_4_flags | 0x10;
    }
    else
    {
        v1 = field_4_flags & ~0x10u;
    }

    gVidSys_7071D0->field_4_flags = v1;

    if (gStartMode_626A0C)
    {
        sub_5D93A0();
    }
    else if (!sub_5D9510())
    {
        if (window_width_706630 == 640 || (window_width_706630 = 640, window_height_706B50 = 480, !sub_5D9510()))
        {
            sub_5D93A0();
            gStartMode_626A0C = 1;
            gRegistry_6FF968.Set_Screen_Setting_587170("start_mode", 1u);
        }
        else
        {
            gRegistry_6FF968.Set_Screen_Setting_587170("window_width", window_width_706630);
            gRegistry_6FF968.Set_Screen_Setting_587170("window_height", window_height_706B50);
        }
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
        gVidSys_7071D0 = 0;
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
        FatalError_4A38C0(1011, "C:\\Splitting\\Gta2\\Source\\video.cpp", 206, gTmpBuffer_67C598);
    }
}

// todo move to another file for ordering
MATCH_FUNC(0x5D96C0)
EXPORT void sub_5D96C0()
{
    s32 v1 = 0;
    if (DMA_Video_LoadDll_5EB970(gVideoDllName_706654))
    {
        FatalError_4A38C0(1011, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1647, gVideoDllName_706654);
    }

    gVidSys_7071D0 = pVid_Init_SYS(gHInstance_708220, 0); // flags param ??

    pVid_SetDevice(gVidSys_7071D0, gVideodevice_70694C);

    if (!gVidSys_7071D0)
    {
        FatalError_4A38C0(2, "C:\\Splitting\\Gta2\\Source\\video.cpp", 1656);
    }

    if (gBufferMode_706B34 == 1)
    {
        v1 = 64;
    }

    gVidSys_7071D0->field_4_flags |= v1;

    if (GetHwndDCDeviceCaps_5D9800() == 16) // 16 bpp?
    {
        bool cVar1 = Vid_FindDevice_5D9290();
        byte_706C5C = 1;
        if (!cVar1)
        {
            byte_706C5C = 0;
        }
    }
    else
    {
        byte_706C5C = 0;
    }

    sub_5D92D0();

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
    keybrd_0x204::create_4D5F50();
    jolly_poitras_0x2BC0::create_56C2C0();

    if (!bSkip_audio_67D6BE)
    {
        gRoot_sound_66B038.sub_40EF80();
        gRoot_sound_66B038.SetCDVol_40F0F0(gRegistry_6FF968.Set_Sound_Setting_586AE0("CDVol", 127));
        gRoot_sound_66B038.SetSfxVol_40F0B0(gRegistry_6FF968.Set_Sound_Setting_586AE0("SFXVol", 127));
        gRoot_sound_66B038.Set3DSound_40F160(gRegistry_6FF968.Get_Sound_Settting_586A70("do_3d_sound"));
        gRegistry_6FF968.Clear_Or_Delete_Sound_Setting_586BF0("do_3d_sound", gRoot_sound_66B038.Get3DSound_40F180());
    }
}

STUB_FUNC(0x4DB170)
void __stdcall sub_4DB170()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4DB0D0)
void __stdcall ExitGameCallback_4DB0D0(Game_0x40* pGame, int reason)
{
    NOT_IMPLEMENTED;
}

// todo move to another file for ordering
STUB_FUNC(0x4DA4D0)
EXPORT void __stdcall sub_4DA4D0()
{
    NOT_IMPLEMENTED;

    if (byte_6F5B71)
    {
        gBurgerKing_67F8B0.sub_4CE740(gHInstance_708220);
    }
    else
    {
        gBurgerKing_67F8B0.sub_4CE880(gHInstance_708220);
    }

    gRoot_sound_66B038.Set3DSound_40F160(gRegistry_6FF968.Get_Sound_Settting_586A70("do_3d_sound"));
    gRegistry_6FF968.Clear_Or_Delete_Sound_Setting_586BF0("do_3d_sound", gRoot_sound_66B038.Get3DSound_40F180());

    if (bStartNetworkGame_7081F0)
    {
        gYouthful_einstein_6F8450.ctor_516560();

        sub_4DB170();

        gGame_0x40_67E008 = new Game_0x40(gNetPlay_7071E8.GetMaxPlayers_521350(), gNetPlay_7071E8.field_5D4_player_idx);

        gNetPlay_7071E8.SetExitGameCallBack_521330((int)ExitGameCallback_4DB0D0, gGame_0x40_67E008);

        // TODO missing code here
    }
    else
    {
        gGame_0x40_67E008 = new Game_0x40(1, 0);
    }

    gGame_0x40_67E008->sub_4B8C40();
    gGame_0x40_67E008->sub_4B8EB0();

    // TODO
    //byte_6F58D8 = 0;
    //byte_6F5880 = 0;
    //dword_6F5A28 = timeGetTime();
    //byte_6F5760 = 1;
}

// todo move to another file for ordering
STUB_FUNC(0x5D9690)
EXPORT void __stdcall sub_5D9690()
{
    NOT_IMPLEMENTED;
    // todo
}

// todo move to another file for ordering
STUB_FUNC(0x4DA850)
EXPORT char_type sub_4DA850()
{
    NOT_IMPLEMENTED;
    // todo
    return 0;
}

// TODO: Other missing stubs here
STUB_FUNC(0x4DA830)
EXPORT void __stdcall sub_4DA830()
{
    NOT_IMPLEMENTED;
}

GUID IID_DirectMusic = {1667997456u, 3197u, 4561u, {149u, 178u, 0u, 32u, 175u, 220u, 116u, 33u}};
GUID IDD_IDirectMusic = {1698042202u, 31533u, 4562u, {186u, 24u, 0u, 0u, 248u, 117u, 172u, 18u}};

// todo move to another file for ordering
MATCH_FUNC(0x4A0650)
EXPORT void __stdcall GetDirectMusicVer_4A0650()
{
    // todo
    HMODULE hDMusic; // esi
    IUnknown* pDMusic; // [esp+4h] [ebp-4h] BYREF

    gDMusicVer_67BD32 = 0;
    hDMusic = LoadLibraryA("DMUSIC.DLL");
    if (hDMusic)
    {
        if (CoCreateInstance(IID_DirectMusic, 0, 1u, IDD_IDirectMusic, (LPVOID*)&pDMusic) >= 0)
        {
            gDMusicVer_67BD32 = 0x601;
            pDMusic->Release();
        }
        FreeLibrary(hDMusic);
    }
}

GUID IID_IDirectDrawSurface4_ = {0x0B2B8630, 0xAD35, 0x11D0, 0x8E, 0xA6, 0x00, 0x60, 0x97, 0x97, 0xEA, 0x5B};

// todo move to another file for ordering
STUB_FUNC(0x4C4EC0)
EXPORT void __stdcall GetDirectXVersion_4C4EC0(u32* pDXVer, u32* osKind)
{
    NOT_IMPLEMENTED;
    u32 dwMajorVersion; // eax
    HMODULE hDInput; // eax
    HMODULE hDInput_; // esi
    FARPROC pDirectInputCreateA; // ebx
    HMODULE hDDraw; // eax
    HMODULE hDDraw_; // esi
    HRESULT(__stdcall * pDirectDrawCreate)(GUID*, LPDIRECTDRAW*, IUnknown*); // eax
    LPDIRECTDRAW pDDraw_; // eax
    HMODULE hDInput2; // eax
    HMODULE hDInput2_; // ebp
    FARPROC pDirectInputCreateA_; // edi
    LPDIRECTDRAWSURFACE pDSurface_; // eax
    LPDIRECTDRAWSURFACE pDSurface__; // eax
    LPDIRECTDRAW pDDraw; // [esp+54h] [ebp-114h] BYREF
    LPDIRECTDRAWSURFACE pDSurface; // [esp+58h] [ebp-110h] BYREF
    LPDIRECTDRAW pDraw2; // [esp+5Ch] [ebp-10Ch] BYREF
    s32 v18; // [esp+60h] [ebp-108h] BYREF
    LPDIRECTDRAWSURFACE pDSurface4; // [esp+64h] [ebp-104h] BYREF
    DDSURFACEDESC surfaceDesc; // [esp+68h] [ebp-100h] BYREF
    struct _OSVERSIONINFOA osVersionInfo; // [esp+D4h] [ebp-94h] BYREF

    pDDraw = 0;
    pDraw2 = 0;
    pDSurface = 0;
    v18 = 0;
    pDSurface4 = 0;
    osVersionInfo.dwOSVersionInfoSize = 148;

    if (!GetVersionExA(&osVersionInfo))
    {
        *pDXVer = 0;
        *osKind = 0;
        return;
    }

    if (osVersionInfo.dwPlatformId == 2) // VER_PLATFORM_WIN32_NT
    {
        dwMajorVersion = osVersionInfo.dwMajorVersion;
        *osKind = 2;
        if (dwMajorVersion < 4)
        {
            *osKind = 0; // less than NT4
            return;
        }
        if (dwMajorVersion == 4) // exactly NT4
        {
            *pDXVer = 0x200;
            hDInput = LoadLibraryA("DINPUT.DLL");
            hDInput_ = hDInput;
            if (hDInput)
            {
                pDirectInputCreateA = GetProcAddress(hDInput, "DirectInputCreateA");
                FreeLibrary(hDInput_);
                if (pDirectInputCreateA)
                {
                    *pDXVer = 0x300;
                }
                else
                {
                    OutputDebugStringA("Couldn't GetProcAddress DInputCreate\r\n");
                }
            }
            else
            {
                OutputDebugStringA("Couldn't LoadLibrary DInput\r\n");
            }
            return;
        }
    }
    else
    {
        *osKind = 1;
    }

    hDDraw = LoadLibraryA("DDRAW.DLL");
    hDDraw_ = hDDraw;
    if (hDDraw)
    {
        pDirectDrawCreate = (HRESULT(__stdcall*)(GUID*, LPDIRECTDRAW*, IUnknown*))GetProcAddress(hDDraw, "DirectDrawCreate");
        if (pDirectDrawCreate)
        {
            if (pDirectDrawCreate(0, &pDDraw, 0) >= 0)
            {
                pDDraw_ = pDDraw;
                *pDXVer = 0x100;
                if (pDDraw_->QueryInterface(IID_IDirectDraw2, (LPVOID*)&pDraw2) >= 0)
                {
                    pDraw2->Release();
                    *pDXVer = 0x200;
                    hDInput2 = LoadLibraryA("DINPUT.DLL");
                    hDInput2_ = hDInput2;
                    if (hDInput2)
                    {
                        pDirectInputCreateA_ = GetProcAddress(hDInput2, "DirectInputCreateA");
                        FreeLibrary(hDInput2_);
                        if (pDirectInputCreateA_)
                        {
                            *pDXVer = 0x300;
                            memset(&surfaceDesc, 0, sizeof(surfaceDesc));
                            surfaceDesc.dwSize = 108;
                            surfaceDesc.dwFlags = 1;
                            surfaceDesc.ddsCaps.dwCaps = 512;
                            if (pDDraw->SetCooperativeLevel(0, 8) >= 0)
                            {
                                if (pDDraw->CreateSurface(&surfaceDesc, &pDSurface, 0) >= 0)
                                {
                                    if (pDSurface->QueryInterface(IID_IDirectDrawSurface2, (LPVOID*)&v18) < 0 ||
                                        (pDSurface_ = pDSurface,
                                         *pDXVer = 0x500,
                                         pDSurface_->QueryInterface(IID_IDirectDrawSurface4_, (LPVOID*)&pDSurface4) < 0))
                                    {
                                        pDDraw->Release();
                                        FreeLibrary(hDDraw_);
                                    }
                                    else
                                    {
                                        pDSurface__ = pDSurface;
                                        *pDXVer = 0x600;
                                        pDSurface__->Release();
                                        pDDraw->Release();
                                        FreeLibrary(hDDraw_);
                                        GetDirectMusicVer_4A0650();
                                        if (gDMusicVer_67BD32 == 0x601)
                                        {
                                            *pDXVer = 0x601;
                                        }
                                    }
                                }
                                else
                                {
                                    pDDraw->Release();
                                    FreeLibrary(hDDraw_);
                                    *pDXVer = 0;
                                    OutputDebugStringA("Couldn't CreateSurface\r\n");
                                }
                            }
                            else
                            {
                                pDDraw->Release();
                                FreeLibrary(hDDraw_);
                                *pDXVer = 0;
                                OutputDebugStringA("Couldn't Set coop level\r\n");
                            }
                        }
                        else
                        {
                            FreeLibrary(hDDraw_);
                            pDDraw->Release();
                            OutputDebugStringA("Couldn't GetProcAddress DInputCreate\r\n");
                        }
                    }
                    else
                    {
                        OutputDebugStringA("Couldn't LoadLibrary DInput\r\n");
                        pDDraw->Release();
                        FreeLibrary(hDDraw_);
                    }
                }
                else
                {
                    pDDraw->Release();
                    FreeLibrary(hDDraw_);
                    OutputDebugStringA("Couldn't QI DDraw2\r\n");
                }
            }
            else
            {
                *pDXVer = 0;
                *osKind = 0;
                FreeLibrary(hDDraw_);
                OutputDebugStringA("Couldn't create DDraw\r\n");
            }
        }
        else
        {
            *pDXVer = 0;
            *osKind = 0;
            FreeLibrary(hDDraw_);
            OutputDebugStringA("Couldn't LoadLibrary DDraw\r\n");
        }
    }
    else
    {
        *pDXVer = 0;
        *osKind = 0;
        FreeLibrary(0);
    }
}

const char_type* off_626A00[2] = {"d3ddll.dll", "dmavideo.dll"};

// todo move to another file for ordering
MATCH_FUNC(0x5D90E0)
EXPORT void __stdcall Video_Render_Inits_5D90E0()
{
    gRenderdevice_706998 = gRegistry_6FF968.Get_Screen_Setting_5870D0("renderdevice", 1);
    gVideodevice_70694C = gRegistry_6FF968.Get_Screen_Setting_5870D0("videodevice", 1);
    strcpy(gRenderDllName_7067F0, off_626A00[0]);
    strcpy(gVideoDllName_706654, off_626A00[1]);
    gRegistry_6FF968.Set_Screen_Setting_5871E0("rendername", (BYTE*)gRenderDllName_7067F0, 0xFFu);
    gRegistry_6FF968.Set_Screen_Setting_5871E0("videoname", (BYTE*)gVideoDllName_706654, 0xFFu);

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

    Init_FrameRateLightAndUnknown_5D8EB0();
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
    gBurgerKing_67F8B0.Shutdown_4CEA00();
    if (gGame_0x40_67E008)
    {
        GTA2_DELETE_AND_NULL(gGame_0x40_67E008);
    }
    gRoot_sound_66B038.Set3DSound_40F160(0);
}

// todo move to another file for ordering
MATCH_FUNC(0x4DA390)
EXPORT s32 __stdcall SkipWhiteSpace_4DA390(char_type* pStr)
{
    char* v1 = pStr;
    s32 result = 0;
    if (pStr == 0)
    {
        return 0;
    }
    else
    {
        if (*pStr)
        {
            do
            {
                while (1)
                {
                    char v3 = *v1;
                    if (v3 != ' ' && v3 != '\n' && v3 != '\r' && v3 != '\t')
                    {
                        break;
                    }
                    if (v3 == 0)
                    {
                        break;
                    }
                    ++v1;
                }
                char v4 = *v1;
                if (!*v1)
                {
                    break;
                }
                ++result;
                for (; v4 != ' '; v4 = *++v1)
                {
                    if (v4 == '\n' || v4 == '\r' || v4 == '\t' || v4 == 0)
                    {
                        break;
                    }
                }
            } while (*v1);
        }
    }
    return result;
}

// todo move to another file for ordering
STUB_FUNC(0x4DA3F0)
EXPORT char_type* __stdcall sub_4DA3F0(char_type* pStr)
{
    NOT_IMPLEMENTED;
    // todo
    return 0;
}

// todo move to another file for ordering
MATCH_FUNC(0x4DA320)
EXPORT void __stdcall ParseCommandLine_4DA320(char_type* pCommandLine)
{
    char* pIter = pCommandLine;
    s32 len = SkipWhiteSpace_4DA390(pCommandLine);
    _strupr(pCommandLine);
    while (len > 0)
    {
        if (*pIter == '-' || *pIter == '/')
        {
            const char_type cmd_char = *(++pIter);
            switch (cmd_char)
            {
                case 'R':
                    bPlay_replay_67D4F4 = 1;
                    break;

                case 'Q':
                    bConstant_replay_save_67D5C4 = 1;
                    break;

                case 'N':
                    bStartNetworkGame_7081F0 = 1;
                    break;

                default:
                    break;
            }
        }

        pIter = sub_4DA3F0(pIter);
        --len;
    }
}

MATCH_FUNC(0x5E4DE0)
EXPORT void Start_GTA2Manager_5E4DE0()
{
    ShowWindow(gHwnd_707F04, SW_SHOWMINNOACTIVE);
    HANDLE hMutex = OpenMutexA(MUTEX_ALL_ACCESS, 0, "GBH_BOB_MUTEX");
    if (hMutex)
    {
        if (!CloseHandle(hMutex))
        {
            FatalError_4A38C0(126, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 118);
        }

        HWND hwndGta2Manager = FindWindowExA(0, 0, 0, "GTA2 Manager");
        if (hwndGta2Manager)
        {
            PostMessageA(gHwnd_707F04, WM_ERASEBKGND, 0, (LPARAM)hwndGta2Manager);
            if (!SetForegroundWindow(hwndGta2Manager))
            {
                FatalError_4A38C0(126, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 125);
            }
        }
    }
    else
    {
        LPARAM lParam; // [esp+4h] [ebp-4h]
        PostMessageA(gHwnd_707F04, WM_ERASEBKGND, 0, lParam);
        if ((u32)ShellExecuteA(0, 0, "GTA2 Manager.EXE", 0, gWorkingDir_707F64, 1) <= 0x20)
        {
            FatalError_4A38C0(126, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 136);
        }
    }
}

MATCH_FUNC(0x5E4EC0)
EXPORT void __stdcall ErrorMsgBox_5E4EC0(LPCSTR lpText)
{
    MessageBoxA(gHwnd_707F04, lpText, "Error!", MB_OK);
}

// todo: move
STUB_FUNC(0x5D9230)
EXPORT void __stdcall sub_5D9230(s32 startMode)
{
    NOT_IMPLEMENTED;
    gStartMode_626A0C = startMode;
    gRegistry_6FF968.Set_Screen_Setting_587170("start_mode", startMode);
}

// todo: move
STUB_FUNC(0x5D9680)
EXPORT void sub_5D9680()
{
    NOT_IMPLEMENTED;
    // todo
}

// todo: move
STUB_FUNC(0x498D10)
EXPORT void Input_Read_498D10()
{
    NOT_IMPLEMENTED;
    // todo
}

// todo: move
STUB_FUNC(0x5D9250)
EXPORT void __stdcall sub_5D9250()
{
    NOT_IMPLEMENTED;
    // todo
}

STUB_FUNC(0x5E4EE0)
EXPORT LRESULT __stdcall WindowProc_5E4EE0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    NOT_IMPLEMENTED;

    switch (Msg)
    {

        case WM_WINDOWPOSCHANGED:
        {
            if (gFrontend_67DC84)
            {
                Bink::sub_513720();
            }
            break;
        }

        case WM_SETFOCUS: // order ok
            gRoot_sound_66B038.sub_40F140();
            gRoot_sound_66B038.SetCDVol_40F0F0(gRegistry_6FF968.Set_Sound_Setting_586AE0("CDVol", 127));
            gRoot_sound_66B038.SetSfxVol_40F0B0(gRegistry_6FF968.Set_Sound_Setting_586AE0("SFXVol", 127));

            if (bDoFrontEnd_626B68)
            {
                gRoot_sound_66B038.Set3DSound_40F160(0);
            }
            else
            {
                gRoot_sound_66B038.Set3DSound_40F160(gRegistry_6FF968.Get_Sound_Settting_586A70("do_3d_sound"));
                gRegistry_6FF968.Clear_Or_Delete_Sound_Setting_586BF0("do_3d_sound", gRoot_sound_66B038.Get3DSound_40F180());
            }

            if (!bDestroyed_6F5B70)
            {
                //LOBYTE(Msg) = 1;
                BYTE tmp = 1;
                Frontend::sub_5E53C0(&tmp);
                if (!bDoFrontEnd_626B68)
                {
                    Input_Read_498D10();
                }

                if (gVidSys_7071D0 && !Bink::sub_513770())
                {
                    sub_5D92D0();
                    sub_5D9680();
                    byte_706C5D = 0;
                }

                if (gGame_0x40_67E008)
                {
                    gGame_0x40_67E008->sub_4B9720();
                }

                SetSavedGamma_5D98E0();
            }
            break;

        case WM_KILLFOCUS: // order ok
            //LOBYTE(hWnd) = 0;
            {
                BYTE tmp = 0;
                Frontend::sub_5E53C0(&tmp);
                Input_ReleaseMouse_5D7C70();
                gRoot_sound_66B038.Set3DSound_40F160(0);
                gRoot_sound_66B038.Release_40F130();

                if (gFrontend_67DC84 && Bink::sub_513760())
                {
                    Bink::Close1_513340();
                    Bink::Close2_513390();
                    gFrontend_67DC84->sub_4B3170(0);
                }

                if (gVidSys_7071D0)
                {
                    if (!Vid_FindDevice_5D9290())
                    {
                        pVid_CloseScreen(gVidSys_7071D0);
                        byte_706C5D = 1;
                        ShowWindow(gHwnd_707F04, 7);
                    }
                }
                break;
            }

        case WM_ACTIVATE: // order ok
            switch (wParam)
            {
                case WA_ACTIVE:
                case WA_CLICKACTIVE:
                {
                    BYTE tmp = 1;
                    Frontend::sub_5E53C0(&tmp);
                    Input_MouseAcquire_5D7C60();
                }
                break;

                case WA_INACTIVE:
                {
                    BYTE tmp = 0;
                    Frontend::sub_5E53C0(&tmp);
                    Input_ReleaseMouse_5D7C70();
                }
                break;
            }
            break;

        case WM_SIZE:
            switch (wParam)
            {
                case 0u:
                    // goto wm_size_case_2;
                    byte_70827C = 0;
                    gRoot_sound_66B038.sub_40F140();
                    break;

                case 1u:
                    byte_70827C = 2;
                    gRoot_sound_66B038.Release_40F130();
                    break;

                case 2u:
                    // wm_size_case_2:
                    byte_70827C = 0;
                    gRoot_sound_66B038.sub_40F140();
                    break;
            }
            break;

        case WM_DESTROY: // order ok
            if (bStartNetworkGame_7081F0)
            {
                gNetPlay_7071E8.sub_520D10();
            }

            ReleaseMutex(gMutex_707078);
            CloseHandle(gMutex_707078);
            gMutex_707078 = 0;

            sub_4DA740();
            GBH_Graphis_DMA_Video_Free_5D9830();
            PostQuitMessage(0);
            break;

        case WM_WINDOWPOSCHANGING:
        {
            WINDOWPOS* pPos = reinterpret_cast<WINDOWPOS*>(lParam);
            if (gFrontend_67DC84 && (pPos->flags & 2) == 0)
            {
                s32 newX = pPos->x;
                s32 newY = pPos->y;
                Bink::sub_5136D0(&newX, &newY);
                pPos->x = newX;
                pPos->y = newY;
            }
            break;
        }

        case WM_SYSKEYDOWN:
            switch (wParam)
            {
                case VK_RETURN:
                    if (sub_5D92C0())
                    {
                        UpdateWinXY_5D8E70();
                        // The previous key state. The value is 1 if the key is down before the message is sent, or it is 0 if the key is up.
                        if ((lParam & 0x20000000) != 0)
                        {
                            sub_5D9250();
                            sub_5D92D0();
                            sub_5D9680();
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
                    if (byte_70827C != 2)
                    {
                        sub_5D9230(1u);
                        sub_5D92D0();
                        sub_5D9680();
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
            RECT winRec;
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

//STUB_FUNC(0x5E53F0)
s32 __stdcall WinMain_5E53F0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    NOT_IMPLEMENTED;
    LoadBeginSceneCBPtr();

    //hInstance_ = hInstance;
    gHInstance_708220 = hInstance;
    if (CoInitialize(0) < 0)
    {
        return 0;
    }

    if (OpenMutexA(0x1F0001u, 0, "GBH_COOP_MUTEX"))
    {
        return 0;
    }

    gMutex_707078 = CreateMutexA(0, 0, "GBH_COOP_MUTEX");
    GetGTA2Version_5E5D60(&gGTA2VersionMajor_708280, &gGTA2VersionMajor_708284);

    // u32 v16;
    u32 dxVer;
    GetDirectXVersion_4C4EC0(&dxVer, &dxVer); // stack hack

    if ((u32)dxVer < 0x601)
    {
        ErrorMsgBox_5E4EC0("ERROR: Incorrect version of DirectX");
        ReleaseMutex(gMutex_707078);
        CloseHandle(gMutex_707078);
        return -1;
    }

    gDebug_67D52C.Init_4ABBD0();
    gLucid_hamilton_67E8E0.sub_4C53D0();
    bDoFrontEnd_626B68 = bSkip_frontend_67D53B == 0;

    WNDCLASSA WndClass;
    WndClass.style = 3; // CS_VREDRAW | CS_HREDRAW
    WndClass.lpfnWndProc = WindowProc_5E4EE0;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIconA(hInstance, (LPCSTR)0x65);
    WndClass.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
    WndClass.hbrBackground = 0;
    WndClass.lpszMenuName = "WinMain";
    WndClass.lpszClassName = "WinMain";
    RegisterClassA(&WndClass);

    Video_Render_Inits_5D90E0();

    _getcwd(gWorkingDir_707F64, 256);

    ParseCommandLine_4DA320(lpCmdLine);
    if (!Start_NetworkGame_5E5A30(hInstance))
    {
        ReleaseMutex(gMutex_707078);
        CloseHandle(gMutex_707078);
        return 0;
    }
    sprintf(gTmpBuffer_67C598, "GTA2");
    gHwnd_707F04 = CreateWindowExA(0, "WinMain", gTmpBuffer_67C598, 0xCF0000u, 0, 0, 100, 100, 0, 0, hInstance, 0);
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

    s32 v8 = window_width_706630 + windowRec.right + clientRec.left - clientRec.right - windowRec.left;
    s32 v9 = window_height_706B50 + windowRec.bottom + clientRec.top - clientRec.bottom - windowRec.top;

    if (bDo_corner_window_67D4EE)
    {
        gWindowX_706B60 = 0;
        gWindowY_706B64 = 0;
    }
    else
    {
        gWindowX_706B60 = (int)(GetSystemMetrics(0) - v8) / 2;
        gWindowY_706B64 = (int)(GetSystemMetrics(1) - v9) / 2;
    }

    if (!SetWindowPos(gHwnd_707F04, 0, gWindowX_706B60, gWindowY_706B64, v8, v9, 0x314u))
    {
        FatalError_4A38C0(7, "C:\\Splitting\\Gta2\\Source\\winmain.cpp", 661);
    }

    Input::DirectInputCreate_4986D0(gHInstance_708220);
    sub_5D96C0(); // todo: cc/arg?

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
        state = 0; //  xor     esi, esi
    }

    if (bIsFrench_67D53C)
    {
        state = 14; // mov     esi, 0Eh
    }

    if (bStartNetworkGame_7081F0)
    {
        bDoFrontEnd_626B68 = 0;
        gRoot_sound_66B038.Set3DSound_40F160(gRegistry_6FF968.Get_Sound_Settting_586A70("do_3d_sound"));
        gRegistry_6FF968.Clear_Or_Delete_Sound_Setting_586BF0("do_3d_sound", gRoot_sound_66B038.Get3DSound_40F180());
    }

    u8 bQuit = 0; //  %bl

    while (1)
    {
        bQuit = 0;
        
        if (bDoFrontEnd_626B68)
        {
            Frontend::create_4ACFA0();
            gFrontend_67DC84->sub_4B3170(state);
        }
        else
        {
            sub_4DA4D0();
            if (bStartNetworkGame_7081F0 && !gNetPlay_7071E8.sub_5213E0())
            {
                CoUninitialize();
                return 0;
            }
        }

        UpdateWinXY_5D8E70();
        sub_5D9690();

        while (1)
        {
            MSG msg;
            if (PeekMessageA(&msg, 0, 0, 0, 1u))
            {
                if (msg.message == 18) // WM_QUIT
                {
                    Input::DInputRelease_498710();
                    return msg.wParam;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            else
            {
                if (!bQuit && byte_70827C != 2 && !byte_706C5D) //  line 3e4
                {
                    if (bDoFrontEnd_626B68)
                    {
                        s32 t = gFrontend_67DC84->sub_4AEDB0();

                        if (t == 1)
                        {
                            bQuit = 1;
                            Frontend::destroy_4AD070();
                            DestroyWindow(gHwnd_707F04);
                            continue; // go to PeekMessageA
                        }
                        else if (t == 3)
                        {
                            Frontend::destroy_4AD070();
                            bDoFrontEnd_626B68 = 0;
                            break; // go to the beginning
                        }
                        else if (t == 4)
                        {
                            Frontend::destroy_4AD070();
                            bDoFrontEnd_626B68 = 0;
                            byte_6F5B71 = 1;
                            break; // go to the beginning
                        }
                        else
                        {
                            continue; // go to PeekMessageA
                        }
                    }
                    bQuit = sub_4DA850();

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
                                switch (gGame_0x40_67E008->field_2C_main_state)
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
                                switch (gGame_0x40_67E008->field_2C_main_state)
                                {
                                    case 1:
                                        DestroyWindow(gHwnd_707F04);
                                        break;

                                    case 2:
                                        gLucid_hamilton_67E8E0.sub_4C5A10(gGame_0x40_67E008->field_38_orf1);
                                        gJolly_poitras_0x2BC0_6FEAC0->sub_56BB10(gGame_0x40_67E008->field_38_orf1);
                                        gJolly_poitras_0x2BC0_6FEAC0->sub_56C010();

                                        state = gLucid_hamilton_67E8E0.sub_4C59A0() != 0 ? 6 : 11; // 11? prob 1
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    case 3:
                                        gLucid_hamilton_67E8E0.sub_4C5A10(gGame_0x40_67E008->field_38_orf1);
                                        gJolly_poitras_0x2BC0_6FEAC0->sub_56BB10(gGame_0x40_67E008->field_38_orf1);
                                        gJolly_poitras_0x2BC0_6FEAC0->sub_56C010();
                                        state = gLucid_hamilton_67E8E0.sub_4C59A0() != 0 ? 6 : 2;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    case 4:
                                        gLucid_hamilton_67E8E0.sub_4C5A10(gGame_0x40_67E008->field_38_orf1);
                                        gJolly_poitras_0x2BC0_6FEAC0->sub_56BB10(gGame_0x40_67E008->field_38_orf1);
                                        gJolly_poitras_0x2BC0_6FEAC0->sub_56C010();
                                        state = gLucid_hamilton_67E8E0.sub_4C59A0() != 0 ? 6 : 3;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    case 5:
                                        state = 7;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    case 6:
                                        state = 0;
                                        CleanUpInputAndOthers_4DA700();
                                        bDoFrontEnd_626B68 = 1;
                                        break;

                                    default:
                                        continue; // go to PeekMessageA
                                }
                                break; // go to the beginning
                            }
                            //  nothing here
                        }
                        //  nothing here
                    }
                    // nothing here
                }
            }
        }

        //  nothing here
    }
    return 0;
}

// TODO: order
MATCH_FUNC(0x4DA780)
EXPORT char sub_4DA780()
{
    char v0 = gGame_0x40_67E008->sub_4B9640();
    if (gsharp_bose_0x54_7055D4)
    {
        gsharp_bose_0x54_7055D4->field_18.sub_5BEBF0();
    }
    return v0;
}

// TODO: order
STUB_FUNC(0x4DA740)
EXPORT void __stdcall sub_4DA740()
{
    if (!bDestroyed_6F5B70)
    {
        bDestroyed_6F5B70 = 1;
        keybrd_0x204::destroy_4D5FA0();
        jolly_poitras_0x2BC0::destroy_56C340();
        CleanUpInputAndOthers_4DA700();
        Frontend::destroy_4AD070();

        // TODO: Function chunk
        if (!bSkip_audio_67D6BE)
        {
            if (gSound_obj_66F680.field_0)
            {
                gSound_obj_66F680.sub_57EA10();
                gSound_obj_66F680.sub_418C60();
                for (s32 i = 0; i < gSound_obj_66F680.field_10_nActiveSamples; ++i)
                {
                    if (gSound_obj_66F680.field_1D_b3d_sound)
                    {
                        gSampManager_6FFF00.StopChannel3D_58DFC0(i);
                    }
                    else
                    {
                        gSampManager_6FFF00.StopChannel_58DDD0(i);
                    }
                }

                for (s32 j = 0; j < gSound_obj_66F680.field_543C_444C_nAudioEntitiesCount; j++)
                {
                    gSound_obj_66F680.field_147C[gSound_obj_66F680.field_444C_AudioEntityOrderList[j]].field_0_bUsed = 0;
                    gSound_obj_66F680.field_444C_AudioEntityOrderList[j] = 0;
                }
                gSound_obj_66F680.field_543C_444C_nAudioEntitiesCount = 0;

                gSound_obj_66F680.null_412250();
                gSampManager_6FFF00.Shutdown_58DB30();
                gSound_obj_66F680.field_0 = 0;
            }
        }
    }
}

STUB_FUNC(0x5E5A30)
EXPORT char_type __stdcall Start_NetworkGame_5E5A30(HINSTANCE hInstance)
{
    NOT_IMPLEMENTED;
    return 1;
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