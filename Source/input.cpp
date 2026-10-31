#include "input.hpp"
#include "Function.hpp"
#include "Globals.hpp"

#include "error.hpp"
#include "enums.hpp"

DEFINE_GLOBAL(LPDIRECTINPUTA, gpDInput, 0x67B804);
DEFINE_GLOBAL(LPDIRECTINPUTDEVICEA, gKeyboardDevice, 0x67B5C0);
DEFINE_GLOBAL(LPDIRECTINPUTDEVICEA, gGamePadDevice, 0x67B6C0);

#if _MSC_VER > 1200
HRESULT WINAPI Fn_DirectInputCreateA(HINSTANCE hinst, u32 dwVersion, LPDIRECTINPUTA* ppDI, LPUNKNOWN punkOuter);
#endif

MATCH_FUNC(0x4986D0)
void __stdcall Input::CreateDirectInput(HINSTANCE hInstance)
{
    #if defined(__clang__) || (_MSC_VER <= 1200)
    #pragma comment(lib, "DInput.lib")
    // VC6-compatible path
    if (DirectInputCreateA(hInstance, 0x0700, &gpDInput, NULL) < 0)
    {
        FatalError(Gta2Error::DirectInputInitFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }
    #else
    #pragma comment(lib, "DInput8.lib")
    // Runtime dynamic loading path
    HMODULE hDx = LoadLibrary("DInput.dll");
    if (!hDx)
    {
        FatalError(Gta2Error::DirectInputInitFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }
    FARPROC p = GetProcAddress(hDx, "DirectInputCreateA");
    if (!p)
    {
        FatalError(Gta2Error::DirectInputInitFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }

    if (((decltype(&Fn_DirectInputCreateA))p)(hInstance, 0x0700, &gpDInput, NULL) < 0)
    {
        FatalError(Gta2Error::DirectInputInitFail, "C:\\Splitting\\Gta2\\Source\\diutil.cpp", 129);
    }
#endif

}

MATCH_FUNC(0x498710)
void __stdcall Input::DInputRelease()
{
    if (gpDInput)
    {
        gpDInput->Release();
        gpDInput = NULL;
    }
}