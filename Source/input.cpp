#include "input.hpp"
#include "Function.hpp"
#include "Globals.hpp"

#include "error.hpp"

#if _MSC_VER <= 1200
    #pragma comment(lib, "DInput.lib")
#endif
//#pragma comment(lib, "DInput8.lib")

EXPORT_VAR LPDIRECTINPUTA gpDInput_67B804;
GLOBAL(gpDInput_67B804, 0x67B804);

EXPORT_VAR LPDIRECTINPUTDEVICEA gKeyboardDevice_67B5C0;
GLOBAL(gKeyboardDevice_67B5C0, 0x67B5C0);

EXPORT_VAR LPDIRECTINPUTDEVICEA gGamePadDevice_67B6C0;
GLOBAL(gGamePadDevice_67B6C0, 0x67B6C0);

#if _MSC_VER > 1200
HRESULT WINAPI Fn_DirectInputCreateA(HINSTANCE hinst, u32 dwVersion, LPDIRECTINPUTA* ppDI, LPUNKNOWN punkOuter);
#endif

MATCH_FUNC(0x4986D0)
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

MATCH_FUNC(0x498710)
void __stdcall Input::DInputRelease_498710()
{
    if (gpDInput_67B804)
    {
        gpDInput_67B804->Release();
        gpDInput_67B804 = 0;
    }
}
