// glrender.dll: the gbh_* exports live in core/gbh_api.cpp; this file only provides DllMain.
#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID)
{
    return TRUE;
}
