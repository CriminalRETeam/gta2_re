#pragma once

#include <windows.h>

void Start_GTA2Manager_5E4DE0();

void __stdcall ErrorMsgBox_5E4EC0(LPCSTR lpText);

LRESULT __stdcall WindowProc_5E4EE0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

void __stdcall laughing_blackwell_0x1EB54_sub_5E53C0(BYTE *a1);

int __stdcall WinMain_5E53F0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

char __stdcall Start_NetworkGame_5E5A30(HINSTANCE hInstance);

void __stdcall GetGTA2Version_5E5D60(int *pVerMinor, int *pVerMajor);

void __stdcall sub_4DA830();
