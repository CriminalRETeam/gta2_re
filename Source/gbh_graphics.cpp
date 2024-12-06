#include "gbh_graphics.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <stdio.h>

EXPORT_VAR T_gbh_InitDLL gbh_InitDLL;
GLOBAL(gbh_InitDLL, 0x626C54);

EXPORT_VAR T_gbh_Init gbh_Init;
GLOBAL(gbh_Init, 0x626C58);

EXPORT_VAR T_gbh_CloseDLL gbh_CloseDLL;
GLOBAL(gbh_CloseDLL, 0x626C5C);

EXPORT_VAR T_gbh_SetWindow gbh_SetWindow;
GLOBAL(gbh_SetWindow, 0x626C60);

EXPORT_VAR T_gbh_GetGlobals gbh_GetGlobals;
GLOBAL(gbh_GetGlobals, 0x626C64);

EXPORT_VAR T_gbh_DrawTile gbh_DrawTile;
GLOBAL(gbh_DrawTile, 0x626C68);

EXPORT_VAR T_gbh_DrawTilePart gbh_DrawTilePart;
GLOBAL(gbh_DrawTilePart, 0x626C6C);

EXPORT_VAR T_gbh_DrawQuad gbh_DrawQuad;
GLOBAL(gbh_DrawQuad, 0x626C70);

EXPORT_VAR T_gbh_DrawQuadClipped gbh_DrawQuadClipped;
GLOBAL(gbh_DrawQuadClipped, 0x626C74);

EXPORT_VAR T_gbh_DrawFlatRect gbh_DrawFlatRect;
GLOBAL(gbh_DrawFlatRect, 0x626C78);

EXPORT_VAR T_gbh_DrawTriangle gbh_DrawTriangle;
GLOBAL(gbh_DrawTriangle, 0x626C7C);

EXPORT_VAR T_gbh_PrintBitmap gbh_PrintBitmap;
GLOBAL(gbh_PrintBitmap, 0x626C80);

EXPORT_VAR T_gbh_SetColourDepth gbh_SetColourDepth;
GLOBAL(gbh_SetColourDepth, 0x626C84);

EXPORT_VAR T_gbh_Plot gbh_Plot;
GLOBAL(gbh_Plot, 0x626C88);

EXPORT_VAR T_gbh_ConvertColour gbh_ConvertColour;
GLOBAL(gbh_ConvertColour, 0x626C8C);

EXPORT_VAR T_gbh_RegisterTexture gbh_RegisterTexture;
GLOBAL(gbh_RegisterTexture, 0x626C90);

EXPORT_VAR T_gbh_SetCamera gbh_SetCamera;
GLOBAL(gbh_SetCamera, 0x626C94);

EXPORT_VAR T_gbh_ResetLights gbh_ResetLights;
GLOBAL(gbh_ResetLights, 0x626C98);

EXPORT_VAR T_gbh_AddLight gbh_AddLight;
GLOBAL(gbh_AddLight, 0x626C9C);

EXPORT_VAR T_gbh_SetAmbient gbh_SetAmbient;
GLOBAL(gbh_SetAmbient, 0x626CA0);

EXPORT_VAR T_gbh_GetUsedCache gbh_GetUsedCache;
GLOBAL(gbh_GetUsedCache, 0x626CA4);

EXPORT_VAR T_gbh_FreeTexture gbh_FreeTexture;
GLOBAL(gbh_FreeTexture, 0x626CA8);

EXPORT_VAR T_gbh_RegisterPalette gbh_RegisterPalette;
GLOBAL(gbh_RegisterPalette, 0x626CAC);

EXPORT_VAR T_gbh_FreePalette gbh_FreePalette;
GLOBAL(gbh_FreePalette, 0x626CB0);

EXPORT_VAR T_gbh_AssignPalette gbh_AssignPalette;
GLOBAL(gbh_AssignPalette, 0x626CB4);

EXPORT_VAR T_gbh_LockTexture gbh_LockTexture;
GLOBAL(gbh_LockTexture, 0x626CB8);

EXPORT_VAR T_gbh_UnlockTexture gbh_UnlockTexture;
GLOBAL(gbh_UnlockTexture, 0x626CBC);

EXPORT_VAR T_gbh_BeginScene gbh_BeginScene;
GLOBAL(gbh_BeginScene, 0x626CC0);

EXPORT_VAR T_gbh_EndScene gbh_EndScene;
GLOBAL(gbh_EndScene, 0x626CC4);

EXPORT_VAR T_gbh_BeginLevel gbh_BeginLevel;
GLOBAL(gbh_BeginLevel, 0x626CC8);

EXPORT_VAR T_gbh_EndLevel gbh_EndLevel;
GLOBAL(gbh_EndLevel, 0x626CCC);

EXPORT_VAR T_MakeScreenTable MakeScreenTable;
GLOBAL(MakeScreenTable, 0x626CD0);

EXPORT_VAR T_ConvertColourBank ConvertColourBank;
GLOBAL(ConvertColourBank, 0x0);

EXPORT_VAR T_SetShadeTableA SetShadeTableA;
GLOBAL(SetShadeTableA, 0x626CD8);

EXPORT_VAR T_gbh_InitImageTable gbh_InitImageTable;
GLOBAL(gbh_InitImageTable, 0x626CDC);

EXPORT_VAR T_gbh_FreeImageTable gbh_FreeImageTable;
GLOBAL(gbh_FreeImageTable, 0x626CE0);

EXPORT_VAR T_gbh_LoadImage gbh_LoadImage;
GLOBAL(gbh_LoadImage, 0x626CE4);

EXPORT_VAR T_gbh_BlitImage gbh_BlitImage;
GLOBAL(gbh_BlitImage, 0x626CE8);

EXPORT_VAR T_gbh_BlitBuffer gbh_BlitBuffer;
GLOBAL(gbh_BlitBuffer, 0x626CEC);

EXPORT_VAR HMODULE gGbhDllHandle_7085D8;
GLOBAL(gGbhDllHandle_7085D8, 0x7085D8);

EXPORT_VAR FARPROC DrawLine;
GLOBAL(DrawLine, 0x7085DC);

EXPORT_VAR s32 gGBH_GraphicsLoaded_7085E0;
GLOBAL(gGBH_GraphicsLoaded_7085E0, 0x7085E0);

EXPORT_VAR int* gpGBH_Globals_7085E4;
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

STUB_FUNC(0x5EA4D0)
EXPORT void __stdcall GBH_Graphics_Stub_5EA4D0()
{
    MessageBoxA(0,
                "Error: A GBH Graphic function was called without the DLL being loaded, or the function has not been fixed up.",
                "GBH Graphics Error",
                MB_OK);
}

STUB_FUNC(0x5EA500)
EXPORT void __stdcall GBH_Graphics_Unload_5EA500()
{
    FreeLibrary(gGbhDllHandle_7085D8);
    gbh_InitDLL = (T_gbh_InitDLL)GBH_Graphics_Stub_5EA4D0;
    gbh_CloseDLL = (T_gbh_CloseDLL)GBH_Graphics_Stub_5EA4D0;
    gbh_Init = (T_gbh_Init)GBH_Graphics_Stub_5EA4D0;
    gbh_DrawTile = (T_gbh_DrawTile)GBH_Graphics_Stub_5EA4D0;
    gbh_DrawQuad = (T_gbh_DrawQuad)GBH_Graphics_Stub_5EA4D0;
    gbh_DrawQuadClipped = (T_gbh_DrawQuadClipped)GBH_Graphics_Stub_5EA4D0;
    gbh_DrawTriangle = (T_gbh_DrawTriangle)GBH_Graphics_Stub_5EA4D0;
    gbh_Plot = (T_gbh_Plot)GBH_Graphics_Stub_5EA4D0;
    gbh_SetWindow = (T_gbh_SetWindow)GBH_Graphics_Stub_5EA4D0;
    gbh_PrintBitmap = (T_gbh_PrintBitmap)GBH_Graphics_Stub_5EA4D0;
    gbh_SetColourDepth = (T_gbh_SetColourDepth)GBH_Graphics_Stub_5EA4D0;
    gbh_GetGlobals = (T_gbh_GetGlobals)GBH_Graphics_Stub_5EA4D0;
    gbh_ConvertColour = (T_gbh_ConvertColour)GBH_Graphics_Stub_5EA4D0;
    gbh_RegisterTexture = (T_gbh_RegisterTexture)GBH_Graphics_Stub_5EA4D0;
    ConvertColourBank = (T_ConvertColourBank)GBH_Graphics_Stub_5EA4D0;
    DrawLine = (FARPROC)GBH_Graphics_Stub_5EA4D0;
    MakeScreenTable = (T_MakeScreenTable)GBH_Graphics_Stub_5EA4D0;
    SetShadeTableA = (T_SetShadeTableA)GBH_Graphics_Stub_5EA4D0;
    gbh_GetUsedCache = (T_gbh_GetUsedCache)GBH_Graphics_Stub_5EA4D0;
    gbh_SetCamera = (T_gbh_SetCamera)GBH_Graphics_Stub_5EA4D0;
    gbh_ResetLights = (T_gbh_ResetLights)GBH_Graphics_Stub_5EA4D0;
    gbh_AddLight = (T_gbh_AddLight)GBH_Graphics_Stub_5EA4D0;
    gbh_SetAmbient = (T_gbh_SetAmbient)GBH_Graphics_Stub_5EA4D0;
    gbh_InitImageTable = (T_gbh_InitImageTable)GBH_Graphics_Stub_5EA4D0;
    gbh_FreeImageTable = (T_gbh_FreeImageTable)GBH_Graphics_Stub_5EA4D0;
    gbh_LoadImage = (T_gbh_LoadImage)GBH_Graphics_Stub_5EA4D0;
    gbh_BlitImage = (T_gbh_BlitImage)GBH_Graphics_Stub_5EA4D0;
    gbh_BlitBuffer = (T_gbh_BlitBuffer)GBH_Graphics_Stub_5EA4D0;
    gbh_DrawFlatRect = (T_gbh_DrawFlatRect)GBH_Graphics_Stub_5EA4D0;
}

STUB_FUNC(0x5EA640)
EXPORT s32 GBH_Graphics_Free_5EA640()
{
    if (gGBH_GraphicsLoaded_7085E0)
    {
        gGBH_GraphicsLoaded_7085E0 = 0;
        gbh_CloseDLL();
        GBH_Graphics_Unload_5EA500();
    }
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

    load_gbh_func(gbh_InitDLL, T_gbh_InitDLL, "gbh_InitDLL");
    load_gbh_func(gbh_CloseDLL, T_gbh_CloseDLL, "gbh_CloseDLL");
    load_gbh_func(gbh_Init, T_gbh_Init, "gbh_Init");
    load_gbh_func(gbh_DrawTile, T_gbh_DrawTile, "gbh_DrawTile");
    load_gbh_func(gbh_DrawTilePart, T_gbh_DrawTilePart, "gbh_DrawTilePart");
    load_gbh_func(gbh_DrawQuad, T_gbh_DrawQuad, "gbh_DrawQuad");
    load_gbh_func(gbh_DrawQuadClipped, T_gbh_DrawQuadClipped, "gbh_DrawQuadClipped");
    load_gbh_func(gbh_DrawTriangle, T_gbh_DrawTriangle, "gbh_DrawTriangle");
    load_gbh_func(gbh_Plot, T_gbh_Plot, "gbh_Plot");
    load_gbh_func(gbh_SetWindow, T_gbh_SetWindow, "gbh_SetWindow");
    load_gbh_func(gbh_PrintBitmap, T_gbh_PrintBitmap, "gbh_PrintBitmap");
    load_gbh_func(gbh_SetColourDepth, T_gbh_SetColourDepth, "gbh_SetColourDepth");
    load_gbh_func(gbh_GetGlobals, T_gbh_GetGlobals, "gbh_GetGlobals");
    load_gbh_func(gbh_ConvertColour, T_gbh_ConvertColour, "gbh_ConvertColour");
    load_gbh_func(gbh_RegisterTexture, T_gbh_RegisterTexture, "gbh_RegisterTexture");
    load_gbh_func(gbh_BeginScene, T_gbh_BeginScene, "gbh_BeginScene");
    load_gbh_func(gbh_EndScene, T_gbh_EndScene, "gbh_EndScene");
    load_gbh_func(gbh_BeginLevel, T_gbh_BeginLevel, "gbh_BeginLevel");
    load_gbh_func(gbh_EndLevel, T_gbh_EndLevel, "gbh_EndLevel");
    load_gbh_func(ConvertColourBank, T_ConvertColourBank, "ConvertColourBank");
    load_gbh_func(DrawLine, FARPROC, "DrawLine");
    load_gbh_func(MakeScreenTable, T_MakeScreenTable, "MakeScreenTable");
    load_gbh_func(SetShadeTableA, T_SetShadeTableA, "SetShadeTableA");
    load_gbh_func(gbh_UnlockTexture, T_gbh_UnlockTexture, "gbh_UnlockTexture");
    load_gbh_func(gbh_RegisterPalette, T_gbh_RegisterPalette, "gbh_RegisterPalette");
    load_gbh_func(gbh_FreePalette, T_gbh_FreePalette, "gbh_FreePalette");
    load_gbh_func(gbh_FreeTexture, T_gbh_FreeTexture, "gbh_FreeTexture");
    load_gbh_func(gbh_AssignPalette, T_gbh_AssignPalette, "gbh_AssignPalette");
    load_gbh_func(gbh_LockTexture, T_gbh_LockTexture, "gbh_LockTexture");
    load_gbh_func(gbh_GetUsedCache, T_gbh_GetUsedCache, "gbh_GetUsedCache");
    load_gbh_func(gbh_SetCamera, T_gbh_SetCamera, "gbh_SetCamera");
    load_gbh_func(gbh_ResetLights, T_gbh_ResetLights, "gbh_ResetLights");
    load_gbh_func(gbh_AddLight, T_gbh_AddLight, "gbh_AddLight");
    load_gbh_func(gbh_SetAmbient, T_gbh_SetAmbient, "gbh_SetAmbient");
    load_gbh_func(gbh_InitImageTable, T_gbh_InitImageTable, "gbh_InitImageTable");
    load_gbh_func(gbh_FreeImageTable, T_gbh_FreeImageTable, "gbh_FreeImageTable");
    load_gbh_func(gbh_LoadImage, T_gbh_LoadImage, "gbh_LoadImage");
    load_gbh_func(gbh_BlitImage, T_gbh_BlitImage, "gbh_BlitImage");
    load_gbh_func(gbh_BlitBuffer, T_gbh_BlitBuffer, "gbh_BlitBuffer");
    load_gbh_func(gbh_DrawFlatRect, T_gbh_DrawFlatRect, "gbh_DrawFlatRect");

    return 0;
}

class Bob
{
  public:
    Bob()
    {
    }
    s32 a, b, c, d, e;
};

STUB_FUNC(0x5EB680)
EXPORT s32 __stdcall GBH_GraphicsLoad_5EB680(const char_type* lpLibFileName, SVideo* pVidSys)
{
    Bob b;
    s32 error = GBH_GraphicsLoadDll_5EA680(lpLibFileName);
    if (error)
    {
        return error;
    }

    gbh_InitDLL(pVidSys);
    gpGBH_Globals_7085E4 = gbh_GetGlobals();

    gGBH_GraphicsLoaded_7085E0 = 1;

    return 0;
}
