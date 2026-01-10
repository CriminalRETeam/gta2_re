#pragma once

#include "Function.hpp"
#include "3rdParty/GTA2Hax/DmaVideo/DmaVideo.hpp"
#include <windows.h>

EXTERN_GLOBAL(SVideo*, gVidSys_7071D0);

EXTERN_GLOBAL(T_Vid_GetVersion, pVid_GetVersion);
EXTERN_GLOBAL(T_Vid_Init_SYS, pVid_Init_SYS);
EXTERN_GLOBAL(T_Vid_CheckMode, pVid_CheckMode);
EXTERN_GLOBAL(T_Vid_FindMode, pVid_FindMode);
EXTERN_GLOBAL(T_Vid_FindFirstMode, pVid_FindFirstMode);
EXTERN_GLOBAL(T_Vid_FindNextMode, pVid_FindNextMode);
EXTERN_GLOBAL(T_Vid_FindDevice, pVid_FindDevice);
EXTERN_GLOBAL(T_Vid_SetDevice, pVid_SetDevice);
EXTERN_GLOBAL(T_Vid_CloseScreen, pVid_CloseScreen);
EXTERN_GLOBAL(T_Vid_SetMode, pVid_SetMode);
EXTERN_GLOBAL(T_Vid_FlipBuffers, pVid_FlipBuffers);
EXTERN_GLOBAL(T_Vid_ReleaseSurface, pVid_ReleaseSurface);
EXTERN_GLOBAL(T_Vid_GrabSurface, pVid_GrabSurface);
EXTERN_GLOBAL(T_Vid_ShutDown_SYS, pVid_ShutDown_SYS);
EXTERN_GLOBAL(T_Vid_EnableWrites, pVid_EnableWrites);
EXTERN_GLOBAL(T_Vid_DisableWrites, pVid_DisableWrites);
EXTERN_GLOBAL(T_Vid_GetSurface, pVid_GetSurface);
EXTERN_GLOBAL(T_Vid_FreeSurface, pVid_FreeSurface);
EXTERN_GLOBAL(T_Vid_ClearScreen, pVid_ClearScreen);
EXTERN_GLOBAL(T_Vid_WindowProc, pVid_WindowProc);
EXTERN_GLOBAL(T_Vid_InitDLL, pVid_InitDLL);
EXTERN_GLOBAL(T_Vid_SetGamma, pVid_SetGamma);

EXPORT void __stdcall DMA_Video_Stub_5EB810();
EXPORT void __stdcall DMA_Video_Unload_5EB8C0(SVideo* pVidSys);
EXPORT s32 __stdcall DMA_Video_LoadDll_5EB970(const char_type* lpLibFileName);
