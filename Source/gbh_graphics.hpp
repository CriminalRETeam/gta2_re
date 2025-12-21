#pragma once

#include "Function.hpp"
#include "3rdParty/GTA2Hax/d3ddll/d3ddll.hpp"
#include <windows.h>

EXPORT s32 __stdcall GBH_GraphicsLoadDll_5EA680(const char_type* lpLibFileName);

struct Vert;
struct SLight;
struct SImage;

typedef void (CC* T_ConvertColourBank)(BYTE* unknown);
typedef s32 (CC* T_DrawLine)(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
typedef void (CC* T_SetShadeTableA)(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
typedef s32* (CC* T_MakeScreenTable)(s32 result, s32 a2, u32 a3);
typedef s32 (CC* T_gbh_AddLight)(SLight* pLight);
typedef void (CC* T_gbh_AssignPalette)(STexture* pTexture, s32 palId);
typedef void (CC* T_gbh_BeginLevel)();
typedef void (CC* T_gbh_SetBeginSceneCB)(TBeginSceneCB pBeginSceneCB);
typedef void (CC* T_gbh_BeginScene)();
typedef s32 (CC* T_gbh_BlitBuffer)(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
typedef s32 (CC* T_gbh_BlitImage)(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
typedef void (CC* T_gbh_CloseDLL)();
typedef void (CC* T_gbh_CloseScreen)(SVideo* pVideo);
typedef u32 (CC* T_gbh_Convert16BitGraphic)(s32 a1, u32 a2, WORD *a3, s32 a4);
typedef u32 (CC* T_gbh_ConvertColour)(u8 a1, u8 a2, u8 a3);
typedef s32 (CC* T_gbh_DrawFlatRect)(s32 a1, s32 a2);
typedef void (CC* T_gbh_DrawQuad)(s32 flags, STexture* pTexture, Vert* pVerts, s32 baseColour);
typedef void (CC* T_gbh_DrawQuadClipped)(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
typedef int(CC* T_gbh_DrawTile)(u32 flags, STexture* pTexture, Vert* pData, s32 diffuseColour);
typedef s32 (CC* T_gbh_DrawTilePart)(u32 flags, STexture* pTexture, Vert* pData, s32 diffuseColour);
typedef void (CC* T_gbh_DrawTriangle)(s32 triFlags, STexture* pTexture, Vert* pVerts, s32 diffuseColour);
typedef void (CC* T_gbh_EndLevel)();
typedef void (CC* T_gbh_EndScene)();
typedef s32 (CC* T_gbh_FreeImageTable)();
typedef void (CC* T_gbh_FreePalette)(s32 a1);
typedef void (CC* T_gbh_FreeTexture)(STexture* pTexture);
typedef u32* (CC* T_gbh_GetGlobals)();
typedef s32 (CC* T_gbh_GetUsedCache)(s32 a1);
typedef s32 (CC* T_gbh_Init)(s32 a1);
typedef u32 (CC* T_gbh_InitDLL)(SVideo* pVideoDriver);
typedef s32 (CC* T_gbh_InitImageTable)(s32 tableSize);
typedef s32 (CC* T_gbh_LoadImage)(SImage* pImage);
typedef STexture* (CC* T_gbh_LockTexture)(STexture* pTexture);
typedef void (CC* T_gbh_Plot)(s32 a1, s32 a2, s32 a3, s32 a4);
typedef s32 (CC* T_gbh_PrintBitmap)(s32 a1, s32 a2);
typedef u32 (CC* T_gbh_RegisterPalette)(s32 a1, DWORD *a2);
typedef STexture* (CC* T_gbh_RegisterTexture)(s32 width, s32 height, BYTE* pData, s32 pal_idx, char_type flag);
typedef void (CC* T_gbh_ResetLights)();
typedef void (CC* T_gbh_SetAmbient)(float a1);
typedef s32 (CC* T_gbh_SetCamera)(float a1, float a2, float a3, float a4);
typedef s32 (CC* T_gbh_SetColourDepth)();
typedef void (CC* T_gbh_SetWindow)(float left, float top, float right, float bottom);
typedef STexture* (CC* T_gbh_UnlockTexture)(STexture* pTexture);

EXTERN_GLOBAL(T_gbh_InitDLL, pgbh_InitDLL);

EXTERN_GLOBAL(T_gbh_Init, pgbh_Init);

EXTERN_GLOBAL(T_gbh_CloseDLL, pgbh_CloseDLL);

EXTERN_GLOBAL(T_gbh_SetWindow, pgbh_SetWindow);

EXTERN_GLOBAL(T_gbh_GetGlobals, pgbh_GetGlobals);

EXTERN_GLOBAL(T_gbh_DrawTile, pgbh_DrawTile);

EXTERN_GLOBAL(T_gbh_DrawTilePart, pgbh_DrawTilePart);

EXTERN_GLOBAL(T_gbh_DrawQuad, pgbh_DrawQuad);

EXTERN_GLOBAL(T_gbh_DrawQuadClipped, pgbh_DrawQuadClipped);

EXTERN_GLOBAL(T_gbh_DrawFlatRect, pgbh_DrawFlatRect);

EXTERN_GLOBAL(T_gbh_DrawTriangle, pgbh_DrawTriangle);

EXTERN_GLOBAL(T_gbh_PrintBitmap, pgbh_PrintBitmap);

EXTERN_GLOBAL(T_gbh_SetColourDepth, pgbh_SetColourDepth);

EXTERN_GLOBAL(T_gbh_Plot, pgbh_Plot);

EXTERN_GLOBAL(T_gbh_ConvertColour, pgbh_ConvertColour);

EXTERN_GLOBAL(T_gbh_RegisterTexture, pgbh_RegisterTexture);

EXTERN_GLOBAL(T_gbh_SetCamera, pgbh_SetCamera);

EXTERN_GLOBAL(T_gbh_ResetLights, pgbh_ResetLights);

EXTERN_GLOBAL(T_gbh_AddLight, pgbh_AddLight);

EXTERN_GLOBAL(T_gbh_SetAmbient, pgbh_SetAmbient);

EXTERN_GLOBAL(T_gbh_GetUsedCache, pgbh_GetUsedCache);

EXTERN_GLOBAL(T_gbh_FreeTexture, pgbh_FreeTexture);

EXTERN_GLOBAL(T_gbh_RegisterPalette, pgbh_RegisterPalette);

EXTERN_GLOBAL(T_gbh_FreePalette, pgbh_FreePalette);

EXTERN_GLOBAL(T_gbh_AssignPalette, pgbh_AssignPalette);

EXTERN_GLOBAL(T_gbh_LockTexture, pgbh_LockTexture);

EXTERN_GLOBAL(T_gbh_UnlockTexture, pgbh_UnlockTexture);

EXTERN_GLOBAL(T_gbh_BeginScene, pgbh_BeginScene);

EXTERN_GLOBAL(T_gbh_EndScene, pgbh_EndScene);

EXTERN_GLOBAL(T_gbh_BeginLevel, pgbh_BeginLevel);

EXTERN_GLOBAL(T_gbh_EndLevel, pgbh_EndLevel);

EXTERN_GLOBAL(T_MakeScreenTable, pMakeScreenTable);

EXTERN_GLOBAL(T_ConvertColourBank, pConvertColourBank);

EXTERN_GLOBAL(T_SetShadeTableA, pSetShadeTableA);

EXTERN_GLOBAL(T_gbh_InitImageTable, pgbh_InitImageTable);

EXTERN_GLOBAL(T_gbh_FreeImageTable, pgbh_FreeImageTable);

EXTERN_GLOBAL(T_gbh_LoadImage, pgbh_LoadImage);

EXTERN_GLOBAL(T_gbh_BlitImage, pgbh_BlitImage);

EXTERN_GLOBAL(T_gbh_BlitBuffer, pgbh_BlitBuffer);


EXTERN_GLOBAL(s32, gGBH_GraphicsLoaded_7085E0);


EXPORT void __stdcall GBH_Graphics_Stub_5EA4D0();
EXPORT s32 __stdcall GBH_GraphicsLoad_5EB680(const char_type* lpLibFileName, SVideo* pVidSys);
EXPORT void __stdcall GBH_Graphics_Unload_5EA500();
EXPORT s32 GBH_Graphics_Free_5EA640();