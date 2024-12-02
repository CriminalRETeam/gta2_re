#include <stdio.h>
#include "winmain.hpp"

extern "C"
{
    void __declspec(dllexport) __cdecl GameMain()
    {
        printf("Welcome to... the GAME MAIN!\n");

        WinMain_5E53F0(GetModuleHandle(NULL), 0, GetCommandLineA(), SW_SHOW);

    }
}
