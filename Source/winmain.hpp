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
EXTERN_GLOBAL(s32, gGTA2VersionMajor);
EXTERN_GLOBAL(s32, gGTA2VersionMinor);

EXPORT void __stdcall ResetFrameTimer();

EXPORT void UpdateGameScreenSize();

EXTERN_GLOBAL(s32, bStartNetworkGame);
EXTERN_GLOBAL_ARRAY(u8, gSyncCheckData, 0x20);
EXPORT void __stdcall MakeInitialSyncData(u8* pSyncData);
EXPORT void __stdcall CompareRemotePlayers(u8* pLocalSyncData, u8* pRemoteSyncData);

EXTERN_GLOBAL_ARRAY(char_type, gWorkingDir, 256);

EXTERN_GLOBAL(u32, gLightingDrawFlag);

EXTERN_GLOBAL(u32, gBufferMode);

EXTERN_GLOBAL(char_type, gLighting);

// 9.6f 0x434B10
static inline u8 IsNetworkGame()
{
    return bStartNetworkGame;
}
