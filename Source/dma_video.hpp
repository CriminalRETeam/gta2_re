#pragma once

#include <Windows.h>
#include "types.hpp"

struct SVideo
{
    u32 field_0;
    u32 field_4_flags;
    u32 field_8_found_width;
    u32 field_C_found_height;
    u32 field_10_found_rgb_bit_count;
    u32 field_14_display_mode_count_2_q;
    u32 field_18_num_guids;
    u32 field_1C_num_display_modes;
    u32 field_20_num_enums;
    struct SDisplayMode* field_24_head_ptr;
    struct SDisplayMode* field_28_display_mode_array;
    struct SDevice* field_2C_device_info_head_ptr;
    struct SDevice* field_30_device_info_last;
    u32 field_34_active_device_id;
    u32 field_38;
    struct SDisplayMode* field_3C_current_enum_ptr;
    u32 field_40_full_screen;
    u32 field_44;
    u32 field_48_rect_right;
    u32 field_4C_rect_bottom;
    LPVOID field_50_surface_pixels_ptr;
    u32 field_54_surface_pixels_pitch;
    u32 field_58;
    u32 field_5C;
    u32 field_60_g;
    u32 field_64_r;
    u32 field_68_b;
    u32 field_6C;
    u32 field_70;
    u32 field_74;
    u32 field_78;
    HINSTANCE field_7C_self_dll_handle;
    u32 field_80_active_mode_q;
    /*
    SPtrVideoFunctions* field_84_from_initDLL;
    u32 field_88_last_error;
    IDirectDraw7* field_8C_DirectDraw7;
    char_type buffer[144];
    IDirectDraw4* field_120_IDDraw4;
    GUID field_124; // TODO: Check
    IDirectDrawSurface4* field_134_SurfacePrimary;
    IDirectDrawSurface4* field_138_Surface;
    DDSURFACEDESC2 field_13C_DDSurfaceDesc7;
    LPDIRECTDRAWCLIPPER field_1B8_clipper;
    u32 field_1BC;
    u32 field_1C0;
    u32 field_1C4;
    DDCAPS field_1C8_device_caps;
    DDCAPS field_344_hel_caps;
    HWND field_4C0_hwnd;
    */
};

extern SVideo *gVidSys_7071D0;

struct SVidVersion
{
    u32 mVersion;
    char_type mVersionString[255];
};

struct SVideoFunctions;

typedef SVidVersion*(__stdcall *T_Vid_GetVersion)();
typedef SVideo*(__stdcall *T_Vid_Init_SYS)(HINSTANCE param1, u16 param2_flags);
typedef s32(__stdcall *T_Vid_CheckMode)(SVideo* pVideoDriver, s32 width, s32 height, s32 rgbBitCount);
typedef SDisplayMode*(__stdcall*T_Vid_FindMode)(SVideo* pVideoDriver, s32 modeId);
typedef s32(__stdcall*T_Vid_FindFirstMode)(SVideo* pVideoDriver, s32 rgbBitCountFilter);
typedef s32(__stdcall*T_Vid_FindNextMode)(SVideo* pVideoDriver);
typedef SDevice*(__stdcall *T_Vid_FindDevice)(SVideo* pVideoDriver, s32 deviceId);
typedef s32(__stdcall *T_Vid_SetDevice)(SVideo* pVideoDriver, s32 deviceId);
typedef void(__stdcall *T_Vid_CloseScreen)(SVideo* pVideo);
typedef s32(__stdcall *T_Vid_SetMode)(SVideo* pVideoDriver, HWND hWnd, s32 modeId);
typedef void(__stdcall *T_Vid_FlipBuffers)(SVideo* pVideo);
typedef void(__stdcall*T_Vid_ReleaseSurface)(SVideo* pVideoDriver);
typedef void(__stdcall*T_Vid_GrabSurface)(SVideo* pVideoDriver);
typedef void(__stdcall *T_Vid_ShutDown_SYS)(SVideo* pVideoDriver);
typedef s32(__stdcall*T_Vid_EnableWrites)(SVideo* pVideoDriver);
typedef s32(__stdcall*T_Vid_DisableWrites)(SVideo* pVideoDriver);
typedef s32(__stdcall *T_Vid_GetSurface)(SVideo* pVideoDriver);
typedef s32(__stdcall *T_Vid_FreeSurface)(SVideo* pVideoDriver);
typedef s32(__stdcall *T_Vid_ClearScreen)(SVideo* pVideoDriver, u8 aR, u8 aG, u8 aB, s32 aLeft, s32 aTop, s32 aRight, s32 aBottom);
typedef s32(__stdcall *T_Vid_WindowProc)(SVideo* pVideoDriver, HWND hwnd, u32 uMsg, WPARAM wParam, LPARAM lParam);
typedef s32(__stdcall *T_Vid_InitDLL)(HINSTANCE hInstance, SVideoFunctions* a2);
typedef s32(__stdcall *T_Vid_SetGamma)(SVideo* pVideoDriver, f32 a2, f32 a3, f32 a4);


struct SVideoFunctions
{
    T_Vid_GetVersion* pVid_GetVersion;
    T_Vid_Init_SYS* pVid_Init_SYS;
    T_Vid_CheckMode* pVid_CheckMode;
    T_Vid_FindMode* pVid_FindMode;
    T_Vid_FindFirstMode* pVid_FindFirstMode;
    T_Vid_FindNextMode* pVid_FindNextMode;
    T_Vid_FindDevice* pVid_FindDevice;
    T_Vid_SetDevice* pVid_SetDevice;
    T_Vid_CloseScreen* pVid_CloseScreen;
    T_Vid_SetMode* pVid_SetMode;
    T_Vid_FlipBuffers* pVid_FlipBuffers;
    T_Vid_ReleaseSurface* pVid_ReleaseSurface;
    T_Vid_GrabSurface* pVid_GrabSurface;
    T_Vid_ShutDown_SYS* pVid_ShutDown_SYS;
    T_Vid_EnableWrites* pVid_EnableWrites;
    T_Vid_DisableWrites* pVid_DisableWrites;
    T_Vid_GetSurface* pVid_GetSurface;
    T_Vid_FreeSurface* pVid_FreeSurface;
    T_Vid_ClearScreen* pVid_ClearScreen;
    T_Vid_WindowProc* pVid_WindowProc;
    T_Vid_InitDLL* pVid_InitDLL;
    T_Vid_SetGamma* pVid_SetGamma;
};

extern T_Vid_GetVersion Vid_GetVersion;
extern T_Vid_Init_SYS Vid_Init_SYS;
extern T_Vid_CheckMode Vid_CheckMode;
extern T_Vid_FindMode Vid_FindMode;
extern T_Vid_FindFirstMode Vid_FindFirstMode;
extern T_Vid_FindNextMode Vid_FindNextMode;
extern T_Vid_FindDevice Vid_FindDevice;
extern T_Vid_SetDevice Vid_SetDevice;
extern T_Vid_CloseScreen Vid_CloseScreen;
extern T_Vid_SetMode Vid_SetMode;
extern T_Vid_FlipBuffers Vid_FlipBuffers;
extern T_Vid_ReleaseSurface Vid_ReleaseSurface;
extern T_Vid_GrabSurface Vid_GrabSurface;
extern T_Vid_ShutDown_SYS Vid_ShutDown_SYS;
extern T_Vid_EnableWrites Vid_EnableWrites;
extern T_Vid_DisableWrites Vid_DisableWrites;
extern T_Vid_GetSurface Vid_GetSurface;
extern T_Vid_FreeSurface Vid_FreeSurface;
extern T_Vid_ClearScreen Vid_ClearScreen;
extern T_Vid_WindowProc Vid_WindowProc;
extern T_Vid_InitDLL Vid_InitDLL;
extern T_Vid_SetGamma Vid_SetGamma;

EXPORT void __stdcall DMA_Video_Unload_5EB8C0(SVideo *pVidSys);
EXPORT s32 __stdcall DMA_Video_LoadDll_5EB970(const char_type* lpLibFileName);
