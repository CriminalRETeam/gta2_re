#include <stdio.h>
#include "winmain.hpp"

extern "C"
{
    void __declspec(dllexport) __cdecl GameMain()
    {
        printf("Welcome to... the GAME MAIN!\n");

        LoadBeginSceneCBPtr();
        GameWinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);

        // TODO: we probably need to call the og exit/static de-inits too
        exit(0);
    }
}