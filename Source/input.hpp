#pragma once

#include "Function.hpp"
#include <windows.h>

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0700
#endif

#include <Dinput.h>


class Input
{
  public:
    EXPORT static void __stdcall DirectInputCreate_4986D0(HINSTANCE hInstance);

    EXPORT static void __stdcall DInputRelease_498710();
};

EXPORT_VAR extern LPDIRECTINPUTA gpDInput_67B804;
EXPORT_VAR extern LPDIRECTINPUTDEVICEA gKeyboardDevice_67B5C0;
EXPORT_VAR extern LPDIRECTINPUTDEVICEA gGamePadDevice_67B6C0;
