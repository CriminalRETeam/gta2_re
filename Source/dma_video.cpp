#include "dma_video.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include <stdio.h>

// GLOBAL: 105 0x7071D0
DEFINE_GLOBAL(SVideo*, gVidSys_7071D0);

// GLOBAL: 105 0x626CF0
DEFINE_GLOBAL(T_Vid_GetVersion, pVid_GetVersion);
// GLOBAL: 105 0x626CF4
DEFINE_GLOBAL(T_Vid_Init_SYS, pVid_Init_SYS);
// GLOBAL: 105 0x626CF8
DEFINE_GLOBAL(T_Vid_CheckMode, pVid_CheckMode);
// GLOBAL: 105 0x626CFC
DEFINE_GLOBAL(T_Vid_FindMode, pVid_FindMode);
// GLOBAL: 105 0x626D00
DEFINE_GLOBAL(T_Vid_FindFirstMode, pVid_FindFirstMode);
// GLOBAL: 105 0x626D04
DEFINE_GLOBAL(T_Vid_FindNextMode, pVid_FindNextMode);
// GLOBAL: 105 0x626D08
DEFINE_GLOBAL(T_Vid_FindDevice, pVid_FindDevice);
// GLOBAL: 105 0x626D0C
DEFINE_GLOBAL(T_Vid_SetDevice, pVid_SetDevice);
// GLOBAL: 105 0x626D10
DEFINE_GLOBAL(T_Vid_CloseScreen, pVid_CloseScreen);
// GLOBAL: 105 0x626D14
DEFINE_GLOBAL(T_Vid_SetMode, pVid_SetMode);
// GLOBAL: 105 0x626D18
DEFINE_GLOBAL(T_Vid_FlipBuffers, pVid_FlipBuffers);
// GLOBAL: 105 0x626D1C
DEFINE_GLOBAL(T_Vid_ReleaseSurface, pVid_ReleaseSurface);
// GLOBAL: 105 0x626D20
DEFINE_GLOBAL(T_Vid_GrabSurface, pVid_GrabSurface);
// GLOBAL: 105 0x626D24
DEFINE_GLOBAL(T_Vid_ShutDown_SYS, pVid_ShutDown_SYS);
// GLOBAL: 105 0x626D28
DEFINE_GLOBAL(T_Vid_EnableWrites, pVid_EnableWrites);
// GLOBAL: 105 0x626D2C
DEFINE_GLOBAL(T_Vid_DisableWrites, pVid_DisableWrites);
// GLOBAL: 105 0x626D30
DEFINE_GLOBAL(T_Vid_GetSurface, pVid_GetSurface);
// GLOBAL: 105 0x626D34
DEFINE_GLOBAL(T_Vid_FreeSurface, pVid_FreeSurface);
// GLOBAL: 105 0x626D38
DEFINE_GLOBAL(T_Vid_ClearScreen, pVid_ClearScreen);
// GLOBAL: 105 0x626D3C
DEFINE_GLOBAL(T_Vid_WindowProc, pVid_WindowProc);
// GLOBAL: 105 0x626D40
DEFINE_GLOBAL(T_Vid_InitDLL, pVid_InitDLL);
// GLOBAL: 105 0x626D44
DEFINE_GLOBAL(T_Vid_SetGamma, pVid_SetGamma);

// GLOBAL: 105 0x7085E8
DEFINE_GLOBAL(HMODULE, hDmaVideoDll_7085E8);
// GLOBAL: 105 0x708600
DEFINE_GLOBAL(SVideoFunctions, gVidFuncs_708600);

// STRING: 105 0x626D48
#define DMA_ERROR_MSG "Error: A DMA Video Function was called without the DLL being loaded"

// STRING: 105 0x626d8c
#define DMA_ERROR_MSG_TITLE "DMA Video Error"

// FUNCTION: 105 0x5EB810
EXPORT void __stdcall DMA_Video_Stub_5EB810()
{
    MessageBoxA(0, DMA_ERROR_MSG, DMA_ERROR_MSG_TITLE, MB_OK);
}

// FUNCTION: 105 0x5EB8C0
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

// STRING: 105 0x601e0c
#define CANT_FIND_FUNCTION_STRING "Can't Find Function Called %s"

// STRING: 105 0x601df0
#define ERROR_PATCHING_DLL_FUNCTION "Error Patching DLL Function"

#define load_gbh_func(varName, varType, funcName)                      \
    varName = (varType)GetProcAddress(hDmaVideoDll_7085E8, funcName); \
    if (!varName)                                                      \
    {                                                                  \
        char_type Text[120];                                           \
        sprintf(Text, CANT_FIND_FUNCTION_STRING, funcName);      \
        MessageBoxA(0, Text, ERROR_PATCHING_DLL_FUNCTION, MB_OK);    \
        FreeLibrary(hDmaVideoDll_7085E8);                             \
        return -1;                                                     \
    }


// STRING: 105 0x601b34
#define CANT_LOAD_DLL_FILE_STRNG "Error can't load DLL file: %s"
// STRING: 105 0x601b20
#define ERROR_LOADING_DLL_STRING "Error Loading DLL."

// STRING: 105
#define VID_INIT_SYS_STRING "Vid_Init_SYS"
// STRING: 105
#define VID_CHECKMODE_STRING "Vid_CheckMode"
// STRING: 105
#define VID_FINDMODE_STRING "Vid_FindMode"
// STRING: 105
#define VID_FINDFIRSTMODE_STRING "Vid_FindFirstMode"
// STRING: 105
#define VID_FINDNEXTMODE_STRING "Vid_FindNextMode"
// STRING: 105
#define VID_CLOSESCREEN_STRING "Vid_CloseScreen"
// STRING: 105
#define VID_SETMODE_STRING "Vid_SetMode"
// STRING: 105
#define VID_FLIPBUFFERS_STRING "Vid_FlipBuffers"
// STRING: 105
#define VID_RELEASESURFACE_STRING "Vid_ReleaseSurface"
// STRING: 105
#define VID_GRABSURFACE_STRING "Vid_GrabSurface"
// STRING: 105
#define VID_SHUTDOWN_SYS_STRING "Vid_ShutDown_SYS"
// STRING: 105
#define VID_ENABLEWRITES_STRING "Vid_EnableWrites"
// STRING: 105
#define VID_DISABLEWRITES_STRING "Vid_DisableWrites"
// STRING: 105
#define VID_GETSURFACE_STRING "Vid_GetSurface"
// STRING: 105
#define VID_FREESURFACE_STRING "Vid_FreeSurface"
// STRING: 105
#define VID_CLEARSCREEN_STRING "Vid_ClearScreen"
// STRING: 105
#define VID_INITDLL_STRING "Vid_InitDLL"
// STRING: 105
#define VID_FINDDEVICE_STRING "Vid_FindDevice"
// STRING: 105
#define VID_SETDEVICE_STRING "Vid_SetDevice"
// STRING: 105
#define VID_WINDOWPROC_STRING "Vid_WindowProc"
// STRING: 105
#define VID_SETGAMMA_STRING "Vid_SetGamma"

// STUB: 105 0x5EB970
EXPORT s32 __stdcall DMA_Video_LoadDll_5EB970(const char_type* lpLibFileName)
{
    NOT_IMPLEMENTED;
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
        load_gbh_func(pVid_Init_SYS, T_Vid_Init_SYS, VID_INIT_SYS_STRING);
        load_gbh_func(pVid_CheckMode, T_Vid_CheckMode, VID_CHECKMODE_STRING);
        load_gbh_func(pVid_FindMode, T_Vid_FindMode, VID_FINDMODE_STRING);
        load_gbh_func(pVid_FindFirstMode, T_Vid_FindFirstMode, VID_FINDFIRSTMODE_STRING);
        load_gbh_func(pVid_FindNextMode, T_Vid_FindNextMode, VID_FINDNEXTMODE_STRING);
        load_gbh_func(pVid_CloseScreen, T_Vid_CloseScreen, VID_CLOSESCREEN_STRING);
        load_gbh_func(pVid_SetMode, T_Vid_SetMode, VID_SETMODE_STRING);
        load_gbh_func(pVid_FlipBuffers, T_Vid_FlipBuffers, VID_FLIPBUFFERS_STRING);
        load_gbh_func(pVid_ReleaseSurface, T_Vid_ReleaseSurface, VID_RELEASESURFACE_STRING);
        load_gbh_func(pVid_GrabSurface, T_Vid_GrabSurface, VID_GRABSURFACE_STRING);
        load_gbh_func(pVid_ShutDown_SYS, T_Vid_ShutDown_SYS, VID_SHUTDOWN_SYS_STRING);
        load_gbh_func(pVid_EnableWrites, T_Vid_EnableWrites, VID_ENABLEWRITES_STRING);
        load_gbh_func(pVid_DisableWrites, T_Vid_DisableWrites, VID_DISABLEWRITES_STRING);
        load_gbh_func(pVid_GetSurface, T_Vid_GetSurface, VID_GETSURFACE_STRING);
        load_gbh_func(pVid_FreeSurface, T_Vid_FreeSurface, VID_FREESURFACE_STRING);
        load_gbh_func(pVid_ClearScreen, T_Vid_ClearScreen, VID_CLEARSCREEN_STRING);
        load_gbh_func(pVid_InitDLL, T_Vid_InitDLL, VID_INITDLL_STRING);
        load_gbh_func(pVid_FindDevice, T_Vid_FindDevice, VID_FINDDEVICE_STRING);
        load_gbh_func(pVid_SetDevice, T_Vid_SetDevice, VID_SETDEVICE_STRING);
        load_gbh_func(pVid_WindowProc, T_Vid_WindowProc, VID_WINDOWPROC_STRING);
        load_gbh_func(pVid_SetGamma, T_Vid_SetGamma, VID_SETGAMMA_STRING);
            

        pVid_InitDLL(hDmaVideoDll_7085E8, &gVidFuncs_708600);
        return 0;
    }
    else
    {
        char_type Buffer[128];
        sprintf(Buffer, CANT_LOAD_DLL_FILE_STRNG, lpLibFileName);
        MessageBoxA(0, Buffer, ERROR_LOADING_DLL_STRING, MB_OK);
        return -1;
    }
}
