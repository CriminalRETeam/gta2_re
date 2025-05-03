#include "gbh_graphics.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <stdio.h>

EXPORT_VAR T_gbh_InitDLL pgbh_InitDLL;
GLOBAL(pgbh_InitDLL, 0x626C54);

EXPORT_VAR T_gbh_Init pgbh_Init;
GLOBAL(pgbh_Init, 0x626C58);

EXPORT_VAR T_gbh_CloseDLL pgbh_CloseDLL;
GLOBAL(pgbh_CloseDLL, 0x626C5C);

EXPORT_VAR T_gbh_SetWindow pgbh_SetWindow;
GLOBAL(pgbh_SetWindow, 0x626C60);

EXPORT_VAR T_gbh_GetGlobals pgbh_GetGlobals;
GLOBAL(pgbh_GetGlobals, 0x626C64);

EXPORT_VAR T_gbh_DrawTile pgbh_DrawTile;
GLOBAL(pgbh_DrawTile, 0x626C68);

EXPORT_VAR T_gbh_DrawTilePart pgbh_DrawTilePart;
GLOBAL(pgbh_DrawTilePart, 0x626C6C);

EXPORT_VAR T_gbh_DrawQuad pgbh_DrawQuad;
GLOBAL(pgbh_DrawQuad, 0x626C70);

EXPORT_VAR T_gbh_DrawQuadClipped pgbh_DrawQuadClipped;
GLOBAL(pgbh_DrawQuadClipped, 0x626C74);

EXPORT_VAR T_gbh_DrawFlatRect pgbh_DrawFlatRect;
GLOBAL(pgbh_DrawFlatRect, 0x626C78);

EXPORT_VAR T_gbh_DrawTriangle pgbh_DrawTriangle;
GLOBAL(pgbh_DrawTriangle, 0x626C7C);

EXPORT_VAR T_gbh_PrintBitmap pgbh_PrintBitmap;
GLOBAL(pgbh_PrintBitmap, 0x626C80);

EXPORT_VAR T_gbh_SetColourDepth pgbh_SetColourDepth;
GLOBAL(pgbh_SetColourDepth, 0x626C84);

EXPORT_VAR T_gbh_Plot pgbh_Plot;
GLOBAL(pgbh_Plot, 0x626C88);

EXPORT_VAR T_gbh_ConvertColour pgbh_ConvertColour;
GLOBAL(pgbh_ConvertColour, 0x626C8C);

EXPORT_VAR T_gbh_RegisterTexture pgbh_RegisterTexture;
GLOBAL(pgbh_RegisterTexture, 0x626C90);

EXPORT_VAR T_gbh_SetCamera pgbh_SetCamera;
GLOBAL(pgbh_SetCamera, 0x626C94);

EXPORT_VAR T_gbh_ResetLights pgbh_ResetLights;
GLOBAL(pgbh_ResetLights, 0x626C98);

EXPORT_VAR T_gbh_AddLight pgbh_AddLight;
GLOBAL(pgbh_AddLight, 0x626C9C);

EXPORT_VAR T_gbh_SetAmbient pgbh_SetAmbient;
GLOBAL(pgbh_SetAmbient, 0x626CA0);

EXPORT_VAR T_gbh_GetUsedCache pgbh_GetUsedCache;
GLOBAL(pgbh_GetUsedCache, 0x626CA4);

EXPORT_VAR T_gbh_FreeTexture pgbh_FreeTexture;
GLOBAL(pgbh_FreeTexture, 0x626CA8);

EXPORT_VAR T_gbh_RegisterPalette pgbh_RegisterPalette;
GLOBAL(pgbh_RegisterPalette, 0x626CAC);

EXPORT_VAR T_gbh_FreePalette pgbh_FreePalette;
GLOBAL(pgbh_FreePalette, 0x626CB0);

EXPORT_VAR T_gbh_AssignPalette pgbh_AssignPalette;
GLOBAL(pgbh_AssignPalette, 0x626CB4);

EXPORT_VAR T_gbh_LockTexture pgbh_LockTexture;
GLOBAL(pgbh_LockTexture, 0x626CB8);

EXPORT_VAR T_gbh_UnlockTexture pgbh_UnlockTexture;
GLOBAL(pgbh_UnlockTexture, 0x626CBC);

EXPORT_VAR T_gbh_BeginScene pgbh_BeginScene;
GLOBAL(pgbh_BeginScene, 0x626CC0);

EXPORT_VAR T_gbh_EndScene pgbh_EndScene;
GLOBAL(pgbh_EndScene, 0x626CC4);

EXPORT_VAR T_gbh_BeginLevel pgbh_BeginLevel;
GLOBAL(pgbh_BeginLevel, 0x626CC8);

EXPORT_VAR T_gbh_EndLevel pgbh_EndLevel;
GLOBAL(pgbh_EndLevel, 0x626CCC);

EXPORT_VAR T_MakeScreenTable pMakeScreenTable;
GLOBAL(pMakeScreenTable, 0x626CD0);

EXPORT_VAR T_ConvertColourBank pConvertColourBank;
GLOBAL(pConvertColourBank, 0x0);

EXPORT_VAR T_SetShadeTableA pSetShadeTableA;
GLOBAL(pSetShadeTableA, 0x626CD8);

EXPORT_VAR T_gbh_InitImageTable pgbh_InitImageTable;
GLOBAL(pgbh_InitImageTable, 0x626CDC);

EXPORT_VAR T_gbh_FreeImageTable pgbh_FreeImageTable;
GLOBAL(pgbh_FreeImageTable, 0x626CE0);

EXPORT_VAR T_gbh_LoadImage pgbh_LoadImage;
GLOBAL(pgbh_LoadImage, 0x626CE4);

EXPORT_VAR T_gbh_BlitImage pgbh_BlitImage;
GLOBAL(pgbh_BlitImage, 0x626CE8);

EXPORT_VAR T_gbh_BlitBuffer pgbh_BlitBuffer;
GLOBAL(pgbh_BlitBuffer, 0x626CEC);

EXPORT_VAR HMODULE gGbhDllHandle_7085D8;
GLOBAL(gGbhDllHandle_7085D8, 0x7085D8);

EXPORT_VAR FARPROC pDrawLine;
GLOBAL(pDrawLine, 0x7085DC);

EXPORT_VAR s32 gGBH_GraphicsLoaded_7085E0;
GLOBAL(gGBH_GraphicsLoaded_7085E0, 0x7085E0);

EXPORT_VAR u32* gpGBH_Globals_7085E4;
GLOBAL(gpGBH_Globals_7085E4, 0x7085E4);

#define load_gbh_func(varName, varType, funcName)                      \
    varName = (varType)GetProcAddress(gGbhDllHandle_7085D8, funcName); \
    if (!varName)                                                      \
    {                                                                  \
        sprintf(Text, "Can't Find Function Called %s", funcName);      \
        MessageBoxA(0, Text, "Error Patching DLL Function", MB_OK);    \
        FreeLibrary(gGbhDllHandle_7085D8);                             \
        return -1;                                                     \
    }

MATCH_FUNC(0x5EA4D0)
EXPORT void __stdcall GBH_Graphics_Stub_5EA4D0()
{
    MessageBoxA(0,
                "Error: A GBH Graphic function was called without the DLL being loaded, or the function has not been fixed up.",
                "GBH Graphics Error",
                MB_OK);
}

MATCH_FUNC(0x5EA500)
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

MATCH_FUNC(0x5EA640)
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

STUB_FUNC(0x5EA680)
EXPORT s32 __stdcall GBH_GraphicsLoadDll_5EA680(const char_type* lpLibFileName)
{
    char_type Text[120];

    if (!lpLibFileName)
    {
        return 0;
    }

    gGbhDllHandle_7085D8 = LoadLibraryA(lpLibFileName);
    if (!gGbhDllHandle_7085D8)
    {
        sprintf(Text, "Error can't load DLL file: %s", lpLibFileName);
        MessageBoxA(0, Text, "Error Loading DLL.", MB_OK);
        return -1;
    }

    load_gbh_func(pgbh_InitDLL, T_gbh_InitDLL, "gbh_InitDLL");
    load_gbh_func(pgbh_CloseDLL, T_gbh_CloseDLL, "gbh_CloseDLL");
    load_gbh_func(pgbh_Init, T_gbh_Init, "gbh_Init");
    load_gbh_func(pgbh_DrawTile, T_gbh_DrawTile, "gbh_DrawTile");
    load_gbh_func(pgbh_DrawTilePart, T_gbh_DrawTilePart, "gbh_DrawTilePart");
    load_gbh_func(pgbh_DrawQuad, T_gbh_DrawQuad, "gbh_DrawQuad");
    load_gbh_func(pgbh_DrawQuadClipped, T_gbh_DrawQuadClipped, "gbh_DrawQuadClipped");
    load_gbh_func(pgbh_DrawTriangle, T_gbh_DrawTriangle, "gbh_DrawTriangle");
    load_gbh_func(pgbh_Plot, T_gbh_Plot, "gbh_Plot");
    load_gbh_func(pgbh_SetWindow, T_gbh_SetWindow, "gbh_SetWindow");
    load_gbh_func(pgbh_PrintBitmap, T_gbh_PrintBitmap, "gbh_PrintBitmap");
    load_gbh_func(pgbh_SetColourDepth, T_gbh_SetColourDepth, "gbh_SetColourDepth");
    load_gbh_func(pgbh_GetGlobals, T_gbh_GetGlobals, "gbh_GetGlobals");
    load_gbh_func(pgbh_ConvertColour, T_gbh_ConvertColour, "gbh_ConvertColour");
    load_gbh_func(pgbh_RegisterTexture, T_gbh_RegisterTexture, "gbh_RegisterTexture");
    load_gbh_func(pgbh_BeginScene, T_gbh_BeginScene, "gbh_BeginScene");
    load_gbh_func(pgbh_EndScene, T_gbh_EndScene, "gbh_EndScene");
    load_gbh_func(pgbh_BeginLevel, T_gbh_BeginLevel, "gbh_BeginLevel");
    load_gbh_func(pgbh_EndLevel, T_gbh_EndLevel, "gbh_EndLevel");
    load_gbh_func(pConvertColourBank, T_ConvertColourBank, "ConvertColourBank");
    load_gbh_func(pDrawLine, FARPROC, "DrawLine");
    load_gbh_func(pMakeScreenTable, T_MakeScreenTable, "MakeScreenTable");
    load_gbh_func(pSetShadeTableA, T_SetShadeTableA, "SetShadeTableA");
    load_gbh_func(pgbh_UnlockTexture, T_gbh_UnlockTexture, "gbh_UnlockTexture");
    load_gbh_func(pgbh_RegisterPalette, T_gbh_RegisterPalette, "gbh_RegisterPalette");
    load_gbh_func(pgbh_FreePalette, T_gbh_FreePalette, "gbh_FreePalette");
    load_gbh_func(pgbh_FreeTexture, T_gbh_FreeTexture, "gbh_FreeTexture");
    load_gbh_func(pgbh_AssignPalette, T_gbh_AssignPalette, "gbh_AssignPalette");
    load_gbh_func(pgbh_LockTexture, T_gbh_LockTexture, "gbh_LockTexture");
    load_gbh_func(pgbh_GetUsedCache, T_gbh_GetUsedCache, "gbh_GetUsedCache");
    load_gbh_func(pgbh_SetCamera, T_gbh_SetCamera, "gbh_SetCamera");
    load_gbh_func(pgbh_ResetLights, T_gbh_ResetLights, "gbh_ResetLights");
    load_gbh_func(pgbh_AddLight, T_gbh_AddLight, "gbh_AddLight");
    load_gbh_func(pgbh_SetAmbient, T_gbh_SetAmbient, "gbh_SetAmbient");
    load_gbh_func(pgbh_InitImageTable, T_gbh_InitImageTable, "gbh_InitImageTable");
    load_gbh_func(pgbh_FreeImageTable, T_gbh_FreeImageTable, "gbh_FreeImageTable");
    load_gbh_func(pgbh_LoadImage, T_gbh_LoadImage, "gbh_LoadImage");
    load_gbh_func(pgbh_BlitImage, T_gbh_BlitImage, "gbh_BlitImage");
    load_gbh_func(pgbh_BlitBuffer, T_gbh_BlitBuffer, "gbh_BlitBuffer");
    load_gbh_func(pgbh_DrawFlatRect, T_gbh_DrawFlatRect, "gbh_DrawFlatRect");

    return 0;
}

STUB_FUNC(0x5EB680)
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
