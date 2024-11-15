#pragma once

#include <windows.h>
#include "Function.hpp"

struct SDisplayMode
{
    u32 field_0_display_mode_idx;
    u32 field_4_deviceId;
    u32 field_8_width;
    u32 field_C_height;
    u32 field_10_pitch;
    u32 field_14_rgb_bit_count;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    u32 field_34;
    struct SDisplayMode* field_38_pnext;
    u32 field_3C;
};

struct SDevice
{
    u32 field_0_id;
    u32 field_4_flags;
    char* field_8_driver_name;
    char* field_C_driver_desc;
    SDevice* field_10_next_ptr;
    GUID* field_14_pDeviceGuid;
    GUID field_18_guid;
    u32 field_28_dwVidMemTotal;
    u32 field_2C_end_buffer; // TODO: Actually a u16 and SDevice is 0x2E ?
};

struct SVideo;

EXPORT s32 __stdcall GBH_GraphicsLoadDll_5EA680(const char_type* lpLibFileName);

struct STexture;
struct Vert;
struct SLight;
struct SImage;

typedef int(__stdcall *T_gbh_InitDLL)(SVideo*);
typedef int(__stdcall *T_gbh_Init)(int);
typedef int(__stdcall *T_gbh_CloseDLL)();
typedef f32(__stdcall *T_gbh_SetWindow)(f32 left, f32 top, f32 right, f32 bottom);
typedef int*(__stdcall *T_gbh_GetGlobals)(void);
typedef int(__stdcall *T_gbh_DrawTile)(u32 flags, STexture* pTexture, Vert* pData, s32 diffuseColour);
typedef int(__stdcall *T_gbh_DrawTilePart)(u32 flags, STexture* pTexture, Vert* pData, s32 diffuseColour);
typedef int(__stdcall *T_gbh_DrawQuad)(s32 flags, STexture* pTexture, Vert* pVerts, s32 baseColour);
typedef int(__stdcall *T_gbh_DrawQuadClipped)(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
typedef int(__stdcall *T_gbh_DrawFlatRect)(s32 a1, s32 a2);
typedef int(__stdcall *T_gbh_DrawTriangle)(s32 triFlags, STexture* pTexture, Vert* pVerts, s32 diffuseColour);
typedef int(__stdcall *T_gbh_PrintBitmap)(s32 a1, s32 a2);
typedef int(__stdcall *T_gbh_SetColourDepth)(void);
typedef int(__stdcall *T_gbh_Plot)(s32 a1, s32 a2, s32 a3, s32 a4);
typedef int(__stdcall *T_gbh_ConvertColour)(u8 a1, u8 a2, u8 a3);
typedef STexture*(__stdcall *T_gbh_RegisterTexture)(s32 width, s32 height, BYTE* pData, s32 pal_idx, char_type flag);
typedef int(__stdcall *T_gbh_SetCamera)(f32 a1, f32 a2, f32 a3, f32 a4);
typedef int(__stdcall *T_gbh_ResetLights)();
typedef int(__stdcall *T_gbh_AddLight)(SLight* pLight);
typedef int(__stdcall *T_gbh_SetAmbient)(f32 a1);
typedef int(__stdcall *T_gbh_GetUsedCache)(s32 a1);
typedef int(__stdcall *T_gbh_FreeTexture)(STexture* pTexture);
typedef int(__stdcall *T_gbh_RegisterPalette)(s32 palId, BYTE *pData);
typedef int(__stdcall *T_gbh_FreePalette)(s32 a1);
typedef int(__stdcall *T_gbh_AssignPalette)(STexture* pTexture, s32 palId);
typedef int(__stdcall *T_gbh_LockTexture)(STexture *);
typedef int(__stdcall *T_gbh_UnlockTexture)(STexture *);
typedef int(__stdcall *T_gbh_BeginScene)();
typedef f64(__stdcall *T_gbh_EndScene)();
typedef int(__stdcall *T_gbh_BeginLevel)();
typedef int(__stdcall *T_gbh_EndLevel)();
typedef int(__stdcall *T_MakeScreenTable)(s32 result, s32 a2, u32 a3);
typedef int(__stdcall *T_ConvertColourBank)(s32 a1);
typedef int(__stdcall *T_SetShadeTableA)(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
typedef int(__stdcall *T_gbh_InitImageTable)(s32 tableSize);
typedef int(__stdcall *T_gbh_FreeImageTable)();
typedef int(__stdcall *T_gbh_LoadImage)(SImage* pImage);
typedef int(__stdcall *T_gbh_BlitImage)(s32 imageIndex, s32 srcLeft, s32 srcTop, s32 srcRight, s32 srcBottom, s32 dstX, s32 dstY);
typedef int(__stdcall *T_gbh_BlitBuffer)(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

extern T_gbh_InitDLL gbh_InitDLL;
extern T_gbh_Init gbh_Init;
extern T_gbh_CloseDLL gbh_CloseDLL;
extern T_gbh_SetWindow gbh_SetWindow;
extern T_gbh_GetGlobals gbh_GetGlobals;
extern T_gbh_DrawTile gbh_DrawTile;
extern T_gbh_DrawTilePart gbh_DrawTilePart;
extern T_gbh_DrawQuad gbh_DrawQuad;
extern T_gbh_DrawQuadClipped gbh_DrawQuadClipped;
extern T_gbh_DrawFlatRect gbh_DrawFlatRect;
extern T_gbh_DrawTriangle gbh_DrawTriangle;
extern T_gbh_PrintBitmap gbh_PrintBitmap;
extern T_gbh_SetColourDepth gbh_SetColourDepth;
extern T_gbh_Plot gbh_Plot;
extern T_gbh_ConvertColour gbh_ConvertColour;
extern T_gbh_RegisterTexture gbh_RegisterTexture;
extern T_gbh_SetCamera gbh_SetCamera;
extern T_gbh_ResetLights gbh_ResetLights;
extern T_gbh_AddLight gbh_AddLight;
extern T_gbh_SetAmbient gbh_SetAmbient;
extern T_gbh_GetUsedCache gbh_GetUsedCache;
extern T_gbh_FreeTexture gbh_FreeTexture;
extern T_gbh_RegisterPalette gbh_RegisterPalette;
extern T_gbh_FreePalette gbh_FreePalette;
extern T_gbh_AssignPalette gbh_AssignPalette;
extern T_gbh_LockTexture gbh_LockTexture;
extern T_gbh_UnlockTexture gbh_UnlockTexture;
extern T_gbh_BeginScene gbh_BeginScene;
extern T_gbh_EndScene gbh_EndScene;
extern T_gbh_BeginLevel gbh_BeginLevel;
extern T_gbh_EndLevel gbh_EndLevel;
extern T_MakeScreenTable MakeScreenTable;
extern T_ConvertColourBank ConvertColourBank;
extern T_SetShadeTableA SetShadeTableA;
extern T_gbh_InitImageTable gbh_InitImageTable;
extern T_gbh_FreeImageTable gbh_FreeImageTable;
extern T_gbh_LoadImage gbh_LoadImage;
extern T_gbh_BlitImage gbh_BlitImage;
extern T_gbh_BlitBuffer gbh_BlitBuffer;

extern s32 gGBH_GraphicsLoaded_7085E0;

EXPORT s32 __stdcall GBH_GraphicsLoad_5EB680(const char_type* lpLibFileName, SVideo* pVidSys);
EXPORT void __stdcall GBH_Graphics_Unload_5EA500();
EXPORT s32 GBH_Graphics_Free_5EA640();
