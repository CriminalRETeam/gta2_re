#include "dma_video.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <stdio.h>

EXPORT_VAR SVideo* gVidSys_7071D0;
GLOBAL(gVidSys_7071D0, 0x7071D0);

EXPORT_VAR T_Vid_GetVersion pVid_GetVersion;
GLOBAL(pVid_GetVersion, 0x626CF0);

EXPORT_VAR T_Vid_Init_SYS pVid_Init_SYS;
GLOBAL(pVid_Init_SYS, 0x626CF4);

EXPORT_VAR T_Vid_CheckMode pVid_CheckMode;
GLOBAL(pVid_CheckMode, 0x626CF8);

EXPORT_VAR T_Vid_FindMode pVid_FindMode;
GLOBAL(pVid_FindMode, 0x626CFC);

EXPORT_VAR T_Vid_FindFirstMode pVid_FindFirstMode;
GLOBAL(pVid_FindFirstMode, 0x626D00);

EXPORT_VAR T_Vid_FindNextMode pVid_FindNextMode;
GLOBAL(pVid_FindNextMode, 0x626D04);

EXPORT_VAR T_Vid_FindDevice pVid_FindDevice;
GLOBAL(pVid_FindDevice, 0x626D08);

EXPORT_VAR T_Vid_SetDevice pVid_SetDevice;
GLOBAL(pVid_SetDevice, 0x626D0C);

EXPORT_VAR T_Vid_CloseScreen pVid_CloseScreen;
GLOBAL(pVid_CloseScreen, 0x626D10);

EXPORT_VAR T_Vid_SetMode pVid_SetMode;
GLOBAL(pVid_SetMode, 0x626D14);

EXPORT_VAR T_Vid_FlipBuffers pVid_FlipBuffers;
GLOBAL(pVid_FlipBuffers, 0x626D18);

EXPORT_VAR T_Vid_ReleaseSurface pVid_ReleaseSurface;
GLOBAL(pVid_ReleaseSurface, 0x626D1C);

EXPORT_VAR T_Vid_GrabSurface pVid_GrabSurface;
GLOBAL(pVid_GrabSurface, 0x626D20);

EXPORT_VAR T_Vid_ShutDown_SYS pVid_ShutDown_SYS;
GLOBAL(pVid_ShutDown_SYS, 0x626D24);

EXPORT_VAR T_Vid_EnableWrites pVid_EnableWrites;
GLOBAL(pVid_EnableWrites, 0x626D28);

EXPORT_VAR T_Vid_DisableWrites pVid_DisableWrites;
GLOBAL(pVid_DisableWrites, 0x626D2C);

EXPORT_VAR T_Vid_GetSurface pVid_GetSurface;
GLOBAL(pVid_GetSurface, 0x626D30);

EXPORT_VAR T_Vid_FreeSurface pVid_FreeSurface;
GLOBAL(pVid_FreeSurface, 0x626D34);

EXPORT_VAR T_Vid_ClearScreen pVid_ClearScreen;
GLOBAL(pVid_ClearScreen, 0x626D38);

EXPORT_VAR T_Vid_WindowProc pVid_WindowProc;
GLOBAL(pVid_WindowProc, 0x626D3C);

EXPORT_VAR T_Vid_InitDLL pVid_InitDLL;
GLOBAL(pVid_InitDLL, 0x626D40);

EXPORT_VAR T_Vid_SetGamma pVid_SetGamma;
GLOBAL(pVid_SetGamma, 0x626D44);

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
    if (pVid_ShutDown_SYS != (T_Vid_ShutDown_SYS)DMA_Video_Stub_5EB810)
    {
        pVid_ShutDown_SYS(pVidSys);
    }

    FreeLibrary(hDmaVideoDll_7085E8);

    pVid_GetVersion = (T_Vid_GetVersion)DMA_Video_Stub_5EB810;
    pVid_Init_SYS = (T_Vid_Init_SYS)DMA_Video_Stub_5EB810;
    pVid_CheckMode = (T_Vid_CheckMode)DMA_Video_Stub_5EB810;
    pVid_FindDevice = (T_Vid_FindDevice)DMA_Video_Stub_5EB810;
    pVid_SetDevice = (T_Vid_SetDevice)DMA_Video_Stub_5EB810;
    pVid_FindMode = (T_Vid_FindMode)DMA_Video_Stub_5EB810;
    pVid_FindFirstMode = (T_Vid_FindFirstMode)DMA_Video_Stub_5EB810;
    pVid_FindNextMode = (T_Vid_FindNextMode)DMA_Video_Stub_5EB810;
    pVid_CloseScreen = (T_Vid_CloseScreen)DMA_Video_Stub_5EB810;
    pVid_SetMode = (T_Vid_SetMode)DMA_Video_Stub_5EB810;
    pVid_FlipBuffers = (T_Vid_FlipBuffers)DMA_Video_Stub_5EB810;
    pVid_ReleaseSurface = (T_Vid_ReleaseSurface)DMA_Video_Stub_5EB810;
    pVid_GrabSurface = (T_Vid_GrabSurface)DMA_Video_Stub_5EB810;
    pVid_ShutDown_SYS = (T_Vid_ShutDown_SYS)DMA_Video_Stub_5EB810;
    pVid_EnableWrites = (T_Vid_EnableWrites)DMA_Video_Stub_5EB810;
    pVid_DisableWrites = (T_Vid_DisableWrites)DMA_Video_Stub_5EB810;
    pVid_GetSurface = (T_Vid_GetSurface)DMA_Video_Stub_5EB810;
    pVid_FreeSurface = (T_Vid_FreeSurface)DMA_Video_Stub_5EB810;
    pVid_ClearScreen = (T_Vid_ClearScreen)DMA_Video_Stub_5EB810;
    pVid_SetGamma = (T_Vid_SetGamma)DMA_Video_Stub_5EB810;
    pVid_WindowProc = (T_Vid_WindowProc)DMA_Video_Stub_5EB810;
    pVid_InitDLL = (T_Vid_InitDLL)DMA_Video_Stub_5EB810;
}

STUB_FUNC(0x5EB970)
EXPORT s32 __stdcall DMA_Video_LoadDll_5EB970(const char_type* lpLibFileName)
{
    char_type Text[120];
    char_type Buffer[128];

    gVidFuncs_708600.pVid_GetVersion = &pVid_GetVersion;
    gVidFuncs_708600.pVid_Init_SYS = &pVid_Init_SYS;
    gVidFuncs_708600.pVid_CheckMode = &pVid_CheckMode;
    gVidFuncs_708600.pVid_FindDevice = &pVid_FindDevice;
    gVidFuncs_708600.pVid_SetDevice = &pVid_SetDevice;
    gVidFuncs_708600.pVid_FindMode = &pVid_FindMode;
    gVidFuncs_708600.pVid_FindFirstMode = &pVid_FindFirstMode;
    gVidFuncs_708600.pVid_FindNextMode = &pVid_FindNextMode;
    gVidFuncs_708600.pVid_CloseScreen = &pVid_CloseScreen;
    gVidFuncs_708600.pVid_SetMode = &pVid_SetMode;
    gVidFuncs_708600.pVid_FlipBuffers = &pVid_FlipBuffers;
    gVidFuncs_708600.pVid_ReleaseSurface = &pVid_ReleaseSurface;
    gVidFuncs_708600.pVid_GrabSurface = &pVid_GrabSurface;
    gVidFuncs_708600.pVid_ShutDown_SYS = &pVid_ShutDown_SYS;
    gVidFuncs_708600.pVid_EnableWrites = &pVid_EnableWrites;
    gVidFuncs_708600.pVid_DisableWrites = &pVid_DisableWrites;
    gVidFuncs_708600.pVid_GetSurface = &pVid_GetSurface;
    gVidFuncs_708600.pVid_FreeSurface = &pVid_FreeSurface;
    gVidFuncs_708600.pVid_ClearScreen = &pVid_ClearScreen;
    gVidFuncs_708600.pVid_SetGamma = &pVid_SetGamma;
    gVidFuncs_708600.pVid_WindowProc = &pVid_WindowProc;
    gVidFuncs_708600.pVid_InitDLL = &pVid_InitDLL;

    hDmaVideoDll_7085E8 = LoadLibraryA(lpLibFileName);
    if (hDmaVideoDll_7085E8)
    {
        pVid_GetVersion = (T_Vid_GetVersion)GetProcAddress(hDmaVideoDll_7085E8, "Vid_GetVersion");
        if (!pVid_GetVersion)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_GetVersion");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_Init_SYS = (T_Vid_Init_SYS)GetProcAddress(hDmaVideoDll_7085E8, "Vid_Init_SYS");
        if (!pVid_Init_SYS)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_Init_SYS");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_CheckMode = (T_Vid_CheckMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_CheckMode");
        if (!pVid_CheckMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_CheckMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_FindMode = (T_Vid_FindMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FindMode");
        if (!pVid_FindMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FindMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_FindFirstMode = (T_Vid_FindFirstMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FindFirstMode");
        if (!pVid_FindFirstMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FindFirstMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_FindNextMode = (T_Vid_FindNextMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FindNextMode");
        if (!pVid_FindNextMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FindNextMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_CloseScreen = (T_Vid_CloseScreen)GetProcAddress(hDmaVideoDll_7085E8, "Vid_CloseScreen");
        if (!pVid_CloseScreen)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_CloseScreen");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_SetMode = (T_Vid_SetMode)GetProcAddress(hDmaVideoDll_7085E8, "Vid_SetMode");
        if (!pVid_SetMode)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_SetMode");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_FlipBuffers = (T_Vid_FlipBuffers)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FlipBuffers");
        if (!pVid_FlipBuffers)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FlipBuffers");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_ReleaseSurface = (T_Vid_ReleaseSurface)GetProcAddress(hDmaVideoDll_7085E8, "Vid_ReleaseSurface");
        if (!pVid_ReleaseSurface)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_ReleaseSurface");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_GrabSurface = (T_Vid_GrabSurface)GetProcAddress(hDmaVideoDll_7085E8, "Vid_GrabSurface");
        if (!pVid_GrabSurface)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_GrabSurface");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_ShutDown_SYS = (T_Vid_ShutDown_SYS)GetProcAddress(hDmaVideoDll_7085E8, "Vid_ShutDown_SYS");
        if (!pVid_ShutDown_SYS)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_ShutDown_SYS");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_EnableWrites = (T_Vid_EnableWrites)GetProcAddress(hDmaVideoDll_7085E8, "Vid_EnableWrites");
        if (!pVid_EnableWrites)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_EnableWrites");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_DisableWrites = (T_Vid_DisableWrites)GetProcAddress(hDmaVideoDll_7085E8, "Vid_DisableWrites");
        if (!pVid_DisableWrites)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_DisableWrites");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_GetSurface = (T_Vid_GetSurface)GetProcAddress(hDmaVideoDll_7085E8, "Vid_GetSurface");
        if (!pVid_GetSurface)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_GetSurface");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_FreeSurface = (T_Vid_FreeSurface)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FreeSurface");
        if (!pVid_FreeSurface)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FreeSurface");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_ClearScreen = (T_Vid_ClearScreen)GetProcAddress(hDmaVideoDll_7085E8, "Vid_ClearScreen");
        if (!pVid_ClearScreen)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_ClearScreen");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_InitDLL = (T_Vid_InitDLL)GetProcAddress(hDmaVideoDll_7085E8, "Vid_InitDLL");
        if (!pVid_InitDLL)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_InitDLL");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_FindDevice = (T_Vid_FindDevice)GetProcAddress(hDmaVideoDll_7085E8, "Vid_FindDevice");
        if (!pVid_FindDevice)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_FindDevice");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_SetDevice = (T_Vid_SetDevice)GetProcAddress(hDmaVideoDll_7085E8, "Vid_SetDevice");
        if (!pVid_SetDevice)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_SetDevice");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_WindowProc = (T_Vid_WindowProc)GetProcAddress(hDmaVideoDll_7085E8, "Vid_WindowProc");
        if (!pVid_WindowProc)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_WindowProc");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_SetGamma = (T_Vid_SetGamma)GetProcAddress(hDmaVideoDll_7085E8, "Vid_SetGamma");
        if (!pVid_SetGamma)
        {
            sprintf(Text, "Can't Find Function Called %s", "Vid_SetGamma");
            MessageBoxA(0, Text, "Error Patching DLL Function", 0);
            FreeLibrary(hDmaVideoDll_7085E8);
            return -1;
        }

        pVid_InitDLL(hDmaVideoDll_7085E8, &gVidFuncs_708600);
        return 0;
    }
    else
    {
        sprintf(Buffer, "Error can't load DLL file: %s", lpLibFileName);
        MessageBoxA(0, Buffer, "Error Loading DLL.", MB_OK);
        return -1;
    }
}
