#include "input.hpp"
#include "Function.hpp"
#include "Globals.hpp"

#include "error.hpp"

// GLOBAL: 105 0x67B804
DEFINE_GLOBAL(LPDIRECTINPUTA, gpDInput_67B804);
// GLOBAL: 105 0x67B5C0
DEFINE_GLOBAL(LPDIRECTINPUTDEVICEA, gKeyboardDevice_67B5C0);
// GLOBAL: 105 0x67B6C0
DEFINE_GLOBAL(LPDIRECTINPUTDEVICEA, gGamePadDevice_67B6C0);

#if _MSC_VER > 1200
HRESULT WINAPI Fn_DirectInputCreateA(HINSTANCE hinst, u32 dwVersion, LPDIRECTINPUTA* ppDI, LPUNKNOWN punkOuter);
#endif

// FUNCTION: 105 0x4986D0
void __stdcall Input::DirectInputCreate_4986D0(HINSTANCE hInstance)
{
    #if defined(__clang__) || (_MSC_VER <= 1200)
    #pragma comment(lib, "DInput.lib")
    // VC6-compatible path
    if (DirectInputCreateA(hInstance, 1792, &gpDInput_67B804, 0) < 0)
    {
        FatalError_4A38C0(8, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }
    #else
    #pragma comment(lib, "DInput8.lib")
    // Runtime dynamic loading path
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

// FUNCTION: 105 0x498710
void __stdcall Input::DInputRelease_498710()
{
    if (gpDInput_67B804)
    {
        gpDInput_67B804->Release();
        gpDInput_67B804 = 0;
    }
}