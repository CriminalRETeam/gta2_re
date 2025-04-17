#pragma once

#include "Function.hpp"
#include "3rdParty/GTA2Hax/DmaVideo/DmaVideo.hpp"
#include <Windows.h>


EXPORT_VAR extern SVideo* gVidSys_7071D0;

EXPORT_VAR extern T_Vid_GetVersion pVid_GetVersion;
EXPORT_VAR extern T_Vid_Init_SYS pVid_Init_SYS;
EXPORT_VAR extern T_Vid_CheckMode pVid_CheckMode;
EXPORT_VAR extern T_Vid_FindMode pVid_FindMode;
EXPORT_VAR extern T_Vid_FindFirstMode pVid_FindFirstMode;
EXPORT_VAR extern T_Vid_FindNextMode pVid_FindNextMode;
EXPORT_VAR extern T_Vid_FindDevice pVid_FindDevice;
EXPORT_VAR extern T_Vid_SetDevice pVid_SetDevice;
EXPORT_VAR extern T_Vid_CloseScreen pVid_CloseScreen;
EXPORT_VAR extern T_Vid_SetMode pVid_SetMode;
EXPORT_VAR extern T_Vid_FlipBuffers pVid_FlipBuffers;
EXPORT_VAR extern T_Vid_ReleaseSurface pVid_ReleaseSurface;
EXPORT_VAR extern T_Vid_GrabSurface pVid_GrabSurface;
EXPORT_VAR extern T_Vid_ShutDown_SYS pVid_ShutDown_SYS;
EXPORT_VAR extern T_Vid_EnableWrites pVid_EnableWrites;
EXPORT_VAR extern T_Vid_DisableWrites pVid_DisableWrites;
EXPORT_VAR extern T_Vid_GetSurface pVid_GetSurface;
EXPORT_VAR extern T_Vid_FreeSurface pVid_FreeSurface;
EXPORT_VAR extern T_Vid_ClearScreen pVid_ClearScreen;
EXPORT_VAR extern T_Vid_WindowProc pVid_WindowProc;
EXPORT_VAR extern T_Vid_InitDLL pVid_InitDLL;
EXPORT_VAR extern T_Vid_SetGamma pVid_SetGamma;

EXPORT void __stdcall DMA_Video_Unload_5EB8C0(SVideo* pVidSys);
EXPORT s32 __stdcall DMA_Video_LoadDll_5EB970(const char_type* lpLibFileName);
