#include "dma_video.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <stdio.h>

EXPORT_VAR SVideo* gVidSys_7071D0;
GLOBAL(gVidSys_7071D0, 0x7071D0);

EXPORT_VAR T_Vid_GetVersion Vid_GetVersion;
GLOBAL(Vid_GetVersion, 0x626CF0);

EXPORT_VAR T_Vid_Init_SYS Vid_Init_SYS;
GLOBAL(Vid_Init_SYS, 0x626CF4);

EXPORT_VAR T_Vid_CheckMode Vid_CheckMode;
GLOBAL(Vid_CheckMode, 0x626CF8);

EXPORT_VAR T_Vid_FindMode Vid_FindMode;
GLOBAL(Vid_FindMode, 0x626CFC);

EXPORT_VAR T_Vid_FindFirstMode Vid_FindFirstMode;
GLOBAL(Vid_FindFirstMode, 0x626D00);

EXPORT_VAR T_Vid_FindNextMode Vid_FindNextMode;
GLOBAL(Vid_FindNextMode, 0x626D04);

EXPORT_VAR T_Vid_FindDevice Vid_FindDevice;
GLOBAL(Vid_FindDevice, 0x626D08);

EXPORT_VAR T_Vid_SetDevice Vid_SetDevice;
GLOBAL(Vid_SetDevice, 0x626D0C);

EXPORT_VAR T_Vid_CloseScreen Vid_CloseScreen;
GLOBAL(Vid_CloseScreen, 0x626D10);

EXPORT_VAR T_Vid_SetMode Vid_SetMode;
GLOBAL(Vid_SetMode, 0x626D14);

EXPORT_VAR T_Vid_FlipBuffers Vid_FlipBuffers;
GLOBAL(Vid_FlipBuffers, 0x626D18);

EXPORT_VAR T_Vid_ReleaseSurface Vid_ReleaseSurface;
GLOBAL(Vid_ReleaseSurface, 0x626D1C);

EXPORT_VAR T_Vid_GrabSurface Vid_GrabSurface;
GLOBAL(Vid_GrabSurface, 0x626D20);

EXPORT_VAR T_Vid_ShutDown_SYS Vid_ShutDown_SYS;
GLOBAL(Vid_ShutDown_SYS, 0x626D24);

EXPORT_VAR T_Vid_EnableWrites Vid_EnableWrites;
GLOBAL(Vid_EnableWrites, 0x626D28);

EXPORT_VAR T_Vid_DisableWrites Vid_DisableWrites;
GLOBAL(Vid_DisableWrites, 0x626D2C);

EXPORT_VAR T_Vid_GetSurface Vid_GetSurface;
GLOBAL(Vid_GetSurface, 0x626D30);

EXPORT_VAR T_Vid_FreeSurface Vid_FreeSurface;
GLOBAL(Vid_FreeSurface, 0x626D34);

EXPORT_VAR T_Vid_ClearScreen Vid_ClearScreen;
GLOBAL(Vid_ClearScreen, 0x626D38);

EXPORT_VAR T_Vid_WindowProc Vid_WindowProc;
GLOBAL(Vid_WindowProc, 0x626D3C);

EXPORT_VAR T_Vid_InitDLL Vid_InitDLL;
GLOBAL(Vid_InitDLL, 0x626D40);

EXPORT_VAR T_Vid_SetGamma Vid_SetGamma;
GLOBAL(Vid_SetGamma, 0x626D44);

EXPORT_VAR HMODULE hDmaVideoDll_7085E8;
GLOBAL(hDmaVideoDll_7085E8, 0x7085E8);

EXPORT_VAR SVideoFunctions gVidFuncs_708600;
GLOBAL(gVidFuncs_708600, 0x708600);

MATCH_FUNC(0x5EB810)
EXPORT void __stdcall DMA_Video_Stub_5EB810()
{
    MessageBoxA(0, "Error: A DMA Video Function was called without the DLL being loaded", "DMA Video Error", MB_OK);
}

MATCH_FUNC(0x5EB8C0)
EXPORT void __stdcall DMA_Video_Unload_5EB8C0(SVideo* pVidSys)
{
    if (Vid_ShutDown_SYS != (T_Vid_ShutDown_SYS)DMA_Video_Stub_5EB810)
    {
        Vid_ShutDown_SYS(pVidSys);
    }

    FreeLibrary(hDmaVideoDll_7085E8);

    Vid_GetVersion = (T_Vid_GetVersion)DMA_Video_Stub_5EB810;
    Vid_Init_SYS = (T_Vid_Init_SYS)DMA_Video_Stub_5EB810;
    Vid_CheckMode = (T_Vid_CheckMode)DMA_Video_Stub_5EB810;
    Vid_FindDevice = (T_Vid_FindDevice)DMA_Video_Stub_5EB810;
    Vid_SetDevice = (T_Vid_SetDevice)DMA_Video_Stub_5EB810;
    Vid_FindMode = (T_Vid_FindMode)DMA_Video_Stub_5EB810;
    Vid_FindFirstMode = (T_Vid_FindFirstMode)DMA_Video_Stub_5EB810;
    Vid_FindNextMode = (T_Vid_FindNextMode)DMA_Video_Stub_5EB810;
    Vid_CloseScreen = (T_Vid_CloseScreen)DMA_Video_Stub_5EB810;
    Vid_SetMode = (T_Vid_SetMode)DMA_Video_Stub_5EB810;
    Vid_FlipBuffers = (T_Vid_FlipBuffers)DMA_Video_Stub_5EB810;
    Vid_ReleaseSurface = (T_Vid_ReleaseSurface)DMA_Video_Stub_5EB810;
    Vid_GrabSurface = (T_Vid_GrabSurface)DMA_Video_Stub_5EB810;
    Vid_ShutDown_SYS = (T_Vid_ShutDown_SYS)DMA_Video_Stub_5EB810;
    Vid_EnableWrites = (T_Vid_EnableWrites)DMA_Video_Stub_5EB810;
    Vid_DisableWrites = (T_Vid_DisableWrites)DMA_Video_Stub_5EB810;
    Vid_GetSurface = (T_Vid_GetSurface)DMA_Video_Stub_5EB810;
    Vid_FreeSurface = (T_Vid_FreeSurface)DMA_Video_Stub_5EB810;
    Vid_ClearScreen = (T_Vid_ClearScreen)DMA_Video_Stub_5EB810;
    Vid_SetGamma = (T_Vid_SetGamma)DMA_Video_Stub_5EB810;
    Vid_WindowProc = (T_Vid_WindowProc)DMA_Video_Stub_5EB810;
    Vid_InitDLL = (T_Vid_InitDLL)DMA_Video_Stub_5EB810;
}

STUB_FUNC(0x5EB970)
EXPORT s32 __stdcall DMA_Video_LoadDll_5EB970(const char_type* lpLibFileName)
{
    char_type Text[120];
    char_type Buffer[128];

    gVidFuncs_708600.pVid_GetVersion = &Vid_GetVersion;
    gVidFuncs_708600.pVid_Init_SYS = &Vid_Init_SYS;
    gVidFuncs_708600.pVid_CheckMode = &Vid_CheckMode;
    gVidFuncs_708600.pVid_FindDevice = &Vid_FindDevice;
    gVidFuncs_708600.pVid_SetDevice = &Vid_SetDevice;
    gVidFuncs_708600.pVid_FindMode = &Vid_FindMode;
    gVidFuncs_708600.pVid_FindFirstMode = &Vid_FindFirstMode;
    gVidFuncs_708600.pVid_FindNextMode = &Vid_FindNextMode;
    gVidFuncs_708600.pVid_CloseScreen = &Vid_CloseScreen;
    gVidFuncs_708600.pVid_SetMode = &Vid_SetMode;
    gVidFuncs_708600.pVid_FlipBuffers = &Vid_FlipBuffers;
    gVidFuncs_708600.pVid_ReleaseSurface = &Vid_ReleaseSurface;
    gVidFuncs_708600.pVid_GrabSurface = &Vid_GrabSurface;
    gVidFuncs_708600.pVid_ShutDown_SYS = &Vid_ShutDown_SYS;
    gVidFuncs_708600.pVid_EnableWrites = &Vid_EnableWrites;
    gVidFuncs_708600.pVid_DisableWrites = &Vid_DisableWrites;
    gVidFuncs_708600.pVid_GetSurface = &Vid_GetSurface;
    gVidFuncs_708600.pVid_FreeSurface = &Vid_FreeSurface;
    gVidFuncs_708600.pVid_ClearScreen = &Vid_ClearScreen;
    gVidFuncs_708600.pVid_SetGamma = &Vid_SetGamma;
    gVidFuncs_708600.pVid_WindowProc = &Vid_WindowProc;
    gVidFuncs_708600.pVid_InitDLL = &Vid_InitDLL;

    hDmaVideoDll_7085E8 = LoadLibraryA(lpLibFileName);
    if (hDmaVideoDll_7085E8)
    {
        Vid_GetVersion = (T_Vid_GetVersion)GetProcAddress(hDmaVideoDll_7085E8, "Vid_GetVersion");
        if (!Vid_GetVersion)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_GetVersion");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_Init_SYS = (T_Vid_Init_SYS)GetProcAddress(hDmaVideoDll_7085E8, "Vid_Init_SYS");
        if (!Vid_Init_SYS)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_Init_SYS");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_CheckMode = (T_Vid_CheckMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_CheckMode");
        if (!Vid_CheckMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_CheckMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_FindMode = (T_Vid_FindMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FindMode");
        if (!Vid_FindMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FindMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_FindFirstMode = (T_Vid_FindFirstMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FindFirstMode");
        if (!Vid_FindFirstMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FindFirstMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_FindNextMode = (T_Vid_FindNextMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FindNextMode");
        if (!Vid_FindNextMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FindNextMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_CloseScreen = (T_Vid_CloseScreen)GetProcAddress(hDmaVideoDll_7085E8, "Vid_CloseScreen");
        if (!Vid_CloseScreen)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_CloseScreen");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_SetMode = (T_Vid_SetMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_SetMode");
        if (!Vid_SetMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_SetMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_FlipBuffers = (T_Vid_FlipBuffers)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FlipBuffers");
        if (!Vid_FlipBuffers)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FlipBuffers");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_ReleaseSurface = (T_Vid_ReleaseSurface)GetProcAddress(hDmaVideoDll_7085E8, "Vid_ReleaseSurface");
        if (!Vid_ReleaseSurface)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_ReleaseSurface");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_GrabSurface = (T_Vid_GrabSurface)GetProcAddress(hDmaVideoDll_7085E8, "Vid_GrabSurface");
        if (!Vid_GrabSurface)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_GrabSurface");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_ShutDown_SYS = (T_Vid_ShutDown_SYS)GetProcAddress(hDmaVideoDll_7085E8, "Vid_ShutDown_SYS");
        if (!Vid_ShutDown_SYS)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_ShutDown_SYS");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_EnableWrites = (T_Vid_EnableWrites)GetProcAddress(hDmaVideoDll_7085E8, "Vid_EnableWrites");
        if (!Vid_EnableWrites)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_EnableWrites");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_DisableWrites = (T_Vid_DisableWrites)GetProcAddress(hDmaVideoDll_7085E8, "Vid_DisableWrites");
        if (!Vid_DisableWrites)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_DisableWrites");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_GetSurface = (T_Vid_GetSurface)GetProcAddress(hDmaVideoDll_7085E8, "Vid_GetSurface");
        if (!Vid_GetSurface)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_GetSurface");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_FreeSurface = (T_Vid_FreeSurface)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FreeSurface");
        if (!Vid_FreeSurface)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FreeSurface");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_ClearScreen = (T_Vid_ClearScreen)GetProcAddress(hDmaVideoDll_7085E8, "Vid_ClearScreen");
        if (!Vid_ClearScreen)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_ClearScreen");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_InitDLL = (T_Vid_InitDLL)GetProcAddress(hDmaVideoDll_7085E8, "Vid_InitDLL");
        if (!Vid_InitDLL)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_InitDLL");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_FindDevice = (T_Vid_FindDevice)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FindDevice");
        if (!Vid_FindDevice)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FindDevice");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_SetDevice = (T_Vid_SetDevice)GetProcAddress(hDmaVideoDll_7085E8, "Vid_SetDevice");
        if (!Vid_SetDevice)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_SetDevice");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_WindowProc = (T_Vid_WindowProc)GetProcAddress(hDmaVideoDll_7085E8, "Vid_WindowProc");
        if (!Vid_WindowProc)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_WindowProc");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_SetGamma = (T_Vid_SetGamma)GetProcAddress(hDmaVideoDll_7085E8, "Vid_SetGamma");
        if (!Vid_SetGamma)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_SetGamma");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        Vid_InitDLL(hDmaVideoDll_7085E8, &gVidFuncs_708600);
        return 0;
    }
    else
    {
        sprintf(Buffer, "Error can't load DLL file: %s", lpLibFileName);
        MessageBoxA(0, Buffer, "Error Loading DLL.", MB_OK);
        return -1;
    }
}
