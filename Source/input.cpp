#include "input.hpp"

#define DIRECTINPUT_VERSION 0x0700

#include <Dinput.h>
#include "error.hpp"

#if _MSC_VER <= 1200
#pragma comment(lib, "DInput.lib")
#endif
//#pragma comment(lib, "DInput8.lib")

LPDIRECTINPUTA gpDInput_67B804;

#if _MSC_VER > 1200
HRESULT WINAPI Fn_DirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA *ppDI, LPUNKNOWN punkOuter);
#endif

void __stdcall Input::DirectInputCreate_4986D0(HINSTANCE hInstance)
{
#if _MSC_VER <= 1200
    // todo
    if (DirectInputCreateA(hInstance, 1792, &gpDInput_67B804, 0) < 0)
    {
        FatalError_4A38C0(8, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }
#else
    HMODULE hDx = LoadLibrary("DInput.dll");
    if (!hDx)
    {
        FatalError_4A38C0(8, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }
    FARPROC p = GetProcAddress(hDx, "DirectInputCreateA");
    if (!p)
    {
        FatalError_4A38C0(8, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }
    
    if (((decltype(&Fn_DirectInputCreateA))p)(hInstance, 1792, &gpDInput_67B804, 0) < 0)
    {
        FatalError_4A38C0(8, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }


#endif
}

// match
void __stdcall Input::DInputRelease_498710()
{
    if (gpDInput_67B804)
    {
        gpDInput_67B804->Release();
        gpDInput_67B804 = 0;
    }
}
