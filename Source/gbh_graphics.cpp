#include "gbh_graphics.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <stdio.h>

// GLOBAL: 105 0x626C54
DEFINE_GLOBAL(T_gbh_InitDLL, pgbh_InitDLL);
// GLOBAL: 105 0x626C58
DEFINE_GLOBAL(T_gbh_Init, pgbh_Init);
// GLOBAL: 105 0x626C5C
DEFINE_GLOBAL(T_gbh_CloseDLL, pgbh_CloseDLL);
// GLOBAL: 105 0x626C60
DEFINE_GLOBAL(T_gbh_SetWindow, pgbh_SetWindow);
// GLOBAL: 105 0x626C64
DEFINE_GLOBAL(T_gbh_GetGlobals, pgbh_GetGlobals);
// GLOBAL: 105 0x626C68
DEFINE_GLOBAL(T_gbh_DrawTile, pgbh_DrawTile);
// GLOBAL: 105 0x626C6C
DEFINE_GLOBAL(T_gbh_DrawTilePart, pgbh_DrawTilePart);
// GLOBAL: 105 0x626C70
DEFINE_GLOBAL(T_gbh_DrawQuad, pgbh_DrawQuad);
// GLOBAL: 105 0x626C74
DEFINE_GLOBAL(T_gbh_DrawQuadClipped, pgbh_DrawQuadClipped);
// GLOBAL: 105 0x626C78
DEFINE_GLOBAL(T_gbh_DrawFlatRect, pgbh_DrawFlatRect);
// GLOBAL: 105 0x626C7C
DEFINE_GLOBAL(T_gbh_DrawTriangle, pgbh_DrawTriangle);
// GLOBAL: 105 0x626C80
DEFINE_GLOBAL(T_gbh_PrintBitmap, pgbh_PrintBitmap);
// GLOBAL: 105 0x626C84
DEFINE_GLOBAL(T_gbh_SetColourDepth, pgbh_SetColourDepth);
// GLOBAL: 105 0x626C88
DEFINE_GLOBAL(T_gbh_Plot, pgbh_Plot);
// GLOBAL: 105 0x626C8C
DEFINE_GLOBAL(T_gbh_ConvertColour, pgbh_ConvertColour);
// GLOBAL: 105 0x626C90
DEFINE_GLOBAL(T_gbh_RegisterTexture, pgbh_RegisterTexture);
// GLOBAL: 105 0x626C94
DEFINE_GLOBAL(T_gbh_SetCamera, pgbh_SetCamera);
// GLOBAL: 105 0x626C98
DEFINE_GLOBAL(T_gbh_ResetLights, pgbh_ResetLights);
// GLOBAL: 105 0x626C9C
DEFINE_GLOBAL(T_gbh_AddLight, pgbh_AddLight);
// GLOBAL: 105 0x626CA0
DEFINE_GLOBAL(T_gbh_SetAmbient, pgbh_SetAmbient);
// GLOBAL: 105 0x626CA4
DEFINE_GLOBAL(T_gbh_GetUsedCache, pgbh_GetUsedCache);
// GLOBAL: 105 0x626CA8
DEFINE_GLOBAL(T_gbh_FreeTexture, pgbh_FreeTexture);
// GLOBAL: 105 0x626CAC
DEFINE_GLOBAL(T_gbh_RegisterPalette, pgbh_RegisterPalette);
// GLOBAL: 105 0x626CB0
DEFINE_GLOBAL(T_gbh_FreePalette, pgbh_FreePalette);
// GLOBAL: 105 0x626CB4
DEFINE_GLOBAL(T_gbh_AssignPalette, pgbh_AssignPalette);
// GLOBAL: 105 0x626CB8
DEFINE_GLOBAL(T_gbh_LockTexture, pgbh_LockTexture);
// GLOBAL: 105 0x626CBC
DEFINE_GLOBAL(T_gbh_UnlockTexture, pgbh_UnlockTexture);
// GLOBAL: 105 0x626CC0
DEFINE_GLOBAL(T_gbh_BeginScene, pgbh_BeginScene);
// GLOBAL: 105 0x626CC4
DEFINE_GLOBAL(T_gbh_EndScene, pgbh_EndScene);
// GLOBAL: 105 0x626CC8
DEFINE_GLOBAL(T_gbh_BeginLevel, pgbh_BeginLevel);
// GLOBAL: 105 0x626CCC
DEFINE_GLOBAL(T_gbh_EndLevel, pgbh_EndLevel);
// GLOBAL: 105 0x626CD0
DEFINE_GLOBAL(T_MakeScreenTable, pMakeScreenTable);
// GLOBAL: 105 0x626CD4
DEFINE_GLOBAL(T_ConvertColourBank, pConvertColourBank);
// GLOBAL: 105 0x626CD8
DEFINE_GLOBAL(T_SetShadeTableA, pSetShadeTableA);
// GLOBAL: 105 0x626CDC
DEFINE_GLOBAL(T_gbh_InitImageTable, pgbh_InitImageTable);
// GLOBAL: 105 0x626CE0
DEFINE_GLOBAL(T_gbh_FreeImageTable, pgbh_FreeImageTable);
// GLOBAL: 105 0x626CE4
DEFINE_GLOBAL(T_gbh_LoadImage, pgbh_LoadImage);
// GLOBAL: 105 0x626CE8
DEFINE_GLOBAL(T_gbh_BlitImage, pgbh_BlitImage);
// GLOBAL: 105 0x626CEC
DEFINE_GLOBAL(T_gbh_BlitBuffer, pgbh_BlitBuffer);
// GLOBAL: 105 0x7085D8
DEFINE_GLOBAL(HMODULE, gGbhDllHandle_7085D8);
// GLOBAL: 105 0x7085DC
DEFINE_GLOBAL(FARPROC, pDrawLine);
// GLOBAL: 105 0x7085E0
DEFINE_GLOBAL(s32, gGBH_GraphicsLoaded_7085E0);
// GLOBAL: 105 0x7085E4
DEFINE_GLOBAL(u32*, gpGBH_Globals_7085E4);

// clang-format off
// STRING: 105 0x601A9C
#define GBH_GRAPHICS_ERROR_MSG_STRING "Error: A GBH Graphic function was called without the DLL being loaded, or the function has not been fixed up."
// clang-format on

// STRING: 105 0x601B0C
#define GBH_GRAPHICS_ERROR_TITLE_STRING "GBH Graphics Error"

// FUNCTION: 105 0x5EA4D0
EXPORT void __stdcall GBH_Graphics_Stub_5EA4D0()
{
    MessageBoxA(0, GBH_GRAPHICS_ERROR_MSG_STRING, GBH_GRAPHICS_ERROR_TITLE_STRING, MB_OK);
}

// FUNCTION: 105 0x5EA500
EXPORT void __stdcall GBH_Graphics_Unload_5EA500()
{
    FreeLibrary(gGbhDllHandle_7085D8);
    pgbh_InitDLL = (T_gbh_InitDLL)GBH_Graphics_Stub_5EA4D0;
    pgbh_CloseDLL = (T_gbh_CloseDLL)GBH_Graphics_Stub_5EA4D0;
    pgbh_Init = (T_gbh_Init)GBH_Graphics_Stub_5EA4D0;
    pgbh_DrawTile = (T_gbh_DrawTile)GBH_Graphics_Stub_5EA4D0;
    pgbh_DrawQuad = (T_gbh_DrawQuad)GBH_Graphics_Stub_5EA4D0;
    pgbh_DrawQuadClipped = (T_gbh_DrawQuadClipped)GBH_Graphics_Stub_5EA4D0;
    pgbh_DrawTriangle = (T_gbh_DrawTriangle)GBH_Graphics_Stub_5EA4D0;
    pgbh_Plot = (T_gbh_Plot)GBH_Graphics_Stub_5EA4D0;
    pgbh_SetWindow = (T_gbh_SetWindow)GBH_Graphics_Stub_5EA4D0;
    pgbh_PrintBitmap = (T_gbh_PrintBitmap)GBH_Graphics_Stub_5EA4D0;
    pgbh_SetColourDepth = (T_gbh_SetColourDepth)GBH_Graphics_Stub_5EA4D0;
    pgbh_GetGlobals = (T_gbh_GetGlobals)GBH_Graphics_Stub_5EA4D0;
    pgbh_ConvertColour = (T_gbh_ConvertColour)GBH_Graphics_Stub_5EA4D0;
    pgbh_RegisterTexture = (T_gbh_RegisterTexture)GBH_Graphics_Stub_5EA4D0;
    pConvertColourBank = (T_ConvertColourBank)GBH_Graphics_Stub_5EA4D0;
    pDrawLine = (FARPROC)GBH_Graphics_Stub_5EA4D0;
    pMakeScreenTable = (T_MakeScreenTable)GBH_Graphics_Stub_5EA4D0;
    pSetShadeTableA = (T_SetShadeTableA)GBH_Graphics_Stub_5EA4D0;
    pgbh_GetUsedCache = (T_gbh_GetUsedCache)GBH_Graphics_Stub_5EA4D0;
    pgbh_SetCamera = (T_gbh_SetCamera)GBH_Graphics_Stub_5EA4D0;
    pgbh_ResetLights = (T_gbh_ResetLights)GBH_Graphics_Stub_5EA4D0;
    pgbh_AddLight = (T_gbh_AddLight)GBH_Graphics_Stub_5EA4D0;
    pgbh_SetAmbient = (T_gbh_SetAmbient)GBH_Graphics_Stub_5EA4D0;
    pgbh_InitImageTable = (T_gbh_InitImageTable)GBH_Graphics_Stub_5EA4D0;
    pgbh_FreeImageTable = (T_gbh_FreeImageTable)GBH_Graphics_Stub_5EA4D0;
    pgbh_LoadImage = (T_gbh_LoadImage)GBH_Graphics_Stub_5EA4D0;
    pgbh_BlitImage = (T_gbh_BlitImage)GBH_Graphics_Stub_5EA4D0;
    pgbh_BlitBuffer = (T_gbh_BlitBuffer)GBH_Graphics_Stub_5EA4D0;
    pgbh_DrawFlatRect = (T_gbh_DrawFlatRect)GBH_Graphics_Stub_5EA4D0;
}

// FUNCTION: 105 0x5EA640
EXPORT s32 GBH_Graphics_Free_5EA640()
{
    if (!gGBH_GraphicsLoaded_7085E0)
    {
        return 0;
    }

    gGBH_GraphicsLoaded_7085E0 = 0;
    pgbh_CloseDLL();
    GBH_Graphics_Unload_5EA500();
    return 0;
}

#define load_gbh_func(varName, varType, funcName)                      \
    varName = (varType)GetProcAddress(gGbhDllHandle_7085D8, funcName); \
    if (!varName)                                                      \
    {                                                                  \
        char_type Text[120];                                           \
        sprintf(Text, "Can't Find Function Called %s", funcName);      \
        MessageBoxA(0, Text, "Error Patching DLL Function", MB_OK);    \
        FreeLibrary(gGbhDllHandle_7085D8);                             \
        return -1;                                                     \
    }

// STRING: 105 0x601e2c
#define GBH_INITDLL_STRING "gbh_InitDLL"
// STRING: 105 0x601de0
#define GBH_CLOSEDLL_STRING "gbh_CloseDLL"
// STRING: 105 0x601dd4
#define GBH_INIT_STRING "gbh_Init"
// STRING: 105 0x601dc4
#define GBH_DRAWTILE_STRING "gbh_DrawTile"
// STRING: 105 0x601db0
#define GBH_DRAWTILEPART_STRING "gbh_DrawTilePart"
// STRING: 105 0x601da0
#define GBH_DRAWQUAD_STRING "gbh_DrawQuad"
// STRING: 105 0x601d8c
#define GBH_DRAWQUADCLIPPED_STRING "gbh_DrawQuadClipped"
// STRING: 105 0x601d78
#define GBH_DRAWTRIANGLE_STRING "gbh_DrawTriangle"
// STRING: 105 0x601d6c
#define GBH_PLOT_STRING "gbh_Plot"
// STRING: 105 0x601d5c
#define GBH_SETWINDOW_STRING "gbh_SetWindow"
// STRING: 105 0x601d4c
#define GBH_PRINTBITMAP_STRING "gbh_PrintBitmap"
// STRING: 105 0x601d38
#define GBH_SETCOLOURDEPTH_STRING "gbh_SetColourDepth"
// STRING: 105 0x601d28
#define GBH_GETGLOBALS_STRING "gbh_GetGlobals"
// STRING: 105 0x601d14
#define GBH_CONVERTCOLOUR_STRING "gbh_ConvertColour"
// STRING: 105 0x601d00
#define GBH_REGISTERTEXTURE_STRING "gbh_RegisterTexture"
// STRING: 105 0x601cf0
#define GBH_BEGINSCENE_STRING "gbh_BeginScene"
// STRING: 105 0x601ce0
#define GBH_ENDSCENE_STRING "gbh_EndScene"
// STRING: 105 0x601cd0
#define GBH_BEGINLEVEL_STRING "gbh_BeginLevel"
// STRING: 105 0x601cc0
#define GBH_ENDLEVEL_STRING "gbh_EndLevel"
// STRING: 105 0x601cac
#define CONVERTCOLOURBANK_STRING "ConvertColourBank"
// STRING: 105 0x601ca0
#define DRAWLINE_STRING "DrawLine"
// STRING: 105 0x601c90
#define MAKESCREENTABLE_STRING "MakeScreenTable"
// STRING: 105 0x601c80
#define SETSHADETABLEA_STRING "SetShadeTableA"
// STRING: 105 0x601c6c
#define GBH_UNLOCKTEXTURE_STRING "gbh_UnlockTexture"
// STRING: 105 0x601c58
#define GBH_REGISTERPALETTE_STRING "gbh_RegisterPalette"
// STRING: 105 0x601c48
#define GBH_FREEPALETTE_STRING "gbh_FreePalette"
// STRING: 105 0x601c38
#define GBH_FREETEXTURE_STRING "gbh_FreeTexture"
// STRING: 105 0x601c24
#define GBH_ASSIGNPALETTE_STRING "gbh_AssignPalette"
// STRING: 105 0x601c14
#define GBH_LOCKTEXTURE_STRING "gbh_LockTexture"
// STRING: 105 0x601c00
#define GBH_GETUSEDCACHE_STRING "gbh_GetUsedCache"
// STRING: 105 0x601bf0
#define GBH_SETCAMERA_STRING "gbh_SetCamera"
// STRING: 105 0x601be0
#define GBH_RESETLIGHTS_STRING "gbh_ResetLights"
// STRING: 105 0x601bd0
#define GBH_ADDLIGHT_STRING "gbh_AddLight"
// STRING: 105 0x601bc0
#define GBH_SETAMBIENT_STRING "gbh_SetAmbient"
// STRING: 105 0x601bac
#define GBH_INITIMAGETABLE_STRING "gbh_InitImageTable"
// STRING: 105 0x601b98
#define GBH_FREEIMAGETABLE_STRING "gbh_FreeImageTable"
// STRING: 105 0x601b88
#define GBH_LOADIMAGE_STRING "gbh_LoadImage"
// STRING: 105 0x601b78
#define GBH_BLITIMAGE_STRING "gbh_BlitImage"
// STRING: 105 0x601b68
#define GBH_BLITBUFFER_STRING "gbh_BlitBuffer"
// STRING: 105 0x601b54
#define GBH_DRAWFLATRECT_STRING "gbh_DrawFlatRect"

// FUNCTION: 105 0x5EA680
EXPORT s32 __stdcall GBH_GraphicsLoadDll_5EA680(const char_type* lpLibFileName)
{
    char_type Text[126];
    if (!lpLibFileName)
    {
        return 0;
    }

    gGbhDllHandle_7085D8 = LoadLibraryA(lpLibFileName);
    if (gGbhDllHandle_7085D8)
    {
        load_gbh_func(pgbh_InitDLL, T_gbh_InitDLL, GBH_INITDLL_STRING);
        load_gbh_func(pgbh_CloseDLL, T_gbh_CloseDLL, GBH_CLOSEDLL_STRING);
        load_gbh_func(pgbh_Init, T_gbh_Init, GBH_INIT_STRING);
        load_gbh_func(pgbh_DrawTile, T_gbh_DrawTile, GBH_DRAWTILE_STRING);
        load_gbh_func(pgbh_DrawTilePart, T_gbh_DrawTilePart, GBH_DRAWTILEPART_STRING);
        load_gbh_func(pgbh_DrawQuad, T_gbh_DrawQuad, GBH_DRAWQUAD_STRING);
        load_gbh_func(pgbh_DrawQuadClipped, T_gbh_DrawQuadClipped, GBH_DRAWQUADCLIPPED_STRING);
        load_gbh_func(pgbh_DrawTriangle, T_gbh_DrawTriangle, GBH_DRAWTRIANGLE_STRING);
        load_gbh_func(pgbh_Plot, T_gbh_Plot, GBH_PLOT_STRING);
        load_gbh_func(pgbh_SetWindow, T_gbh_SetWindow, GBH_SETWINDOW_STRING);
        load_gbh_func(pgbh_PrintBitmap, T_gbh_PrintBitmap, GBH_PRINTBITMAP_STRING);
        load_gbh_func(pgbh_SetColourDepth, T_gbh_SetColourDepth, GBH_SETCOLOURDEPTH_STRING);
        load_gbh_func(pgbh_GetGlobals, T_gbh_GetGlobals, GBH_GETGLOBALS_STRING);
        load_gbh_func(pgbh_ConvertColour, T_gbh_ConvertColour, GBH_CONVERTCOLOUR_STRING);
        load_gbh_func(pgbh_RegisterTexture, T_gbh_RegisterTexture, GBH_REGISTERTEXTURE_STRING);
        load_gbh_func(pgbh_BeginScene, T_gbh_BeginScene, GBH_BEGINSCENE_STRING);
        load_gbh_func(pgbh_EndScene, T_gbh_EndScene, GBH_ENDSCENE_STRING);
        load_gbh_func(pgbh_BeginLevel, T_gbh_BeginLevel, GBH_BEGINLEVEL_STRING);
        load_gbh_func(pgbh_EndLevel, T_gbh_EndLevel, GBH_ENDLEVEL_STRING);
        load_gbh_func(pConvertColourBank, T_ConvertColourBank, CONVERTCOLOURBANK_STRING);
        load_gbh_func(pDrawLine, FARPROC, DRAWLINE_STRING);
        load_gbh_func(pMakeScreenTable, T_MakeScreenTable, MAKESCREENTABLE_STRING);
        load_gbh_func(pSetShadeTableA, T_SetShadeTableA, SETSHADETABLEA_STRING);
        load_gbh_func(pgbh_UnlockTexture, T_gbh_UnlockTexture, GBH_UNLOCKTEXTURE_STRING);
        load_gbh_func(pgbh_RegisterPalette, T_gbh_RegisterPalette, GBH_REGISTERPALETTE_STRING);
        load_gbh_func(pgbh_FreePalette, T_gbh_FreePalette, GBH_FREEPALETTE_STRING);
        load_gbh_func(pgbh_FreeTexture, T_gbh_FreeTexture, GBH_FREETEXTURE_STRING);
        load_gbh_func(pgbh_AssignPalette, T_gbh_AssignPalette, GBH_ASSIGNPALETTE_STRING);
        load_gbh_func(pgbh_LockTexture, T_gbh_LockTexture, GBH_LOCKTEXTURE_STRING);
        load_gbh_func(pgbh_GetUsedCache, T_gbh_GetUsedCache, GBH_GETUSEDCACHE_STRING);
        load_gbh_func(pgbh_SetCamera, T_gbh_SetCamera, GBH_SETCAMERA_STRING);
        load_gbh_func(pgbh_ResetLights, T_gbh_ResetLights, GBH_RESETLIGHTS_STRING);
        load_gbh_func(pgbh_AddLight, T_gbh_AddLight, GBH_ADDLIGHT_STRING);
        load_gbh_func(pgbh_SetAmbient, T_gbh_SetAmbient, GBH_SETAMBIENT_STRING);
        load_gbh_func(pgbh_InitImageTable, T_gbh_InitImageTable, GBH_INITIMAGETABLE_STRING);
        load_gbh_func(pgbh_FreeImageTable, T_gbh_FreeImageTable, GBH_FREEIMAGETABLE_STRING);
        load_gbh_func(pgbh_LoadImage, T_gbh_LoadImage, GBH_LOADIMAGE_STRING);
        load_gbh_func(pgbh_BlitImage, T_gbh_BlitImage, GBH_BLITIMAGE_STRING);
        load_gbh_func(pgbh_BlitBuffer, T_gbh_BlitBuffer, GBH_BLITBUFFER_STRING);
        load_gbh_func(pgbh_DrawFlatRect, T_gbh_DrawFlatRect, GBH_DRAWFLATRECT_STRING);
    }
    else
    {
        sprintf(Text, "Error can't load DLL file: %s", lpLibFileName);
        MessageBoxA(0, Text, "Error Loading DLL.", MB_OK);
        return -1;
    }
    return 0;
}

// FUNCTION: 105 0x5EB680
EXPORT s32 __stdcall GBH_GraphicsLoad_5EB680(const char_type* lpLibFileName, SVideo* pVidSys)
{
    const s32 error = GBH_GraphicsLoadDll_5EA680(lpLibFileName);
    if (error)
    {
        return error;
    }

    pgbh_InitDLL(pVidSys);
    gpGBH_Globals_7085E4 = pgbh_GetGlobals();

    gGBH_GraphicsLoaded_7085E0 = 1;

    return 0;
}
