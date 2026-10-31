#pragma once

#include "Function.hpp"
#include <windows.h>

EXPORT void Start_GTA2Manager();

EXPORT void __stdcall ErrorMsgBox(LPCSTR lpText);

EXPORT LRESULT __stdcall WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void LoadBeginSceneCBPtr();

s32 __stdcall WinMain_5E53F0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd);

EXPORT bool __stdcall Start_NetworkGame(HINSTANCE hInstance);

EXPORT void __stdcall GetGTA2Version(s32* pVerMinor, s32* pVerMajor);
EXTERN_GLOBAL(s32, gGTA2VersionMajor_708280);
EXTERN_GLOBAL(s32, gGTA2VersionMinor_708284);

EXPORT void __stdcall ResetFrameTimer();

EXPORT void UpdateGameScreenSize();

EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);
EXTERN_GLOBAL_ARRAY(u8, gSyncCheckData_6F58E0, 0x20);
EXPORT void __stdcall MakeInitialSyncData(u8* pSyncData);
EXPORT void __stdcall CompareRemotePlayers(u8* pLocalSyncData, u8* pRemoteSyncData);

EXTERN_GLOBAL_ARRAY(char_type, gWorkingDir_707F64, 256);

EXTERN_GLOBAL(u32, gLightingDrawFlag_7068F4);

EXTERN_GLOBAL(u32, gBufferMode_706B34);

EXTERN_GLOBAL(char_type, gLighting_626A09);

static inline u8 IsNetworkGame_434B10()
{
    return bStartNetworkGame_7081F0;
}
