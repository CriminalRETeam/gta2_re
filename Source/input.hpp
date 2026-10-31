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
    EXPORT static void __stdcall CreateDirectInput(HINSTANCE hInstance);

    EXPORT static void __stdcall DInputRelease();
};

EXTERN_GLOBAL(LPDIRECTINPUTA, gpDInput);

EXTERN_GLOBAL(LPDIRECTINPUTDEVICEA, gKeyboardDevice);

EXTERN_GLOBAL(LPDIRECTINPUTDEVICEA, gGamePadDevice);
