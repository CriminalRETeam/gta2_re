#pragma once

#include "Function.hpp"
#include <windows.h>

EXPORT void Start_GTA2Manager_5E4DE0();

EXPORT void __stdcall ErrorMsgBox_5E4EC0(LPCSTR lpText);

EXPORT LRESULT __stdcall WindowProc_5E4EE0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

EXPORT void __stdcall laughing_blackwell_0x1EB54_sub_5E53C0(BYTE* a1);

int __stdcall WinMain_5E53F0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

EXPORT char __stdcall Start_NetworkGame_5E5A30(HINSTANCE hInstance);

EXPORT void __stdcall GetGTA2Version_5E5D60(int* pVerMinor, int* pVerMajor);

EXPORT void __stdcall sub_4DA830();

EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);