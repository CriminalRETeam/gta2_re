// glvideo.dll: Vid_* exports on top of VidSystem, with a WGL context on the game's window.
#include "../core/vid_system.hpp"
#include "../platform/gl_context.hpp"
#include "dinput_hook.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define EXPORT extern "C" __declspec(dllexport)

static HINSTANCE gDllInstance;
static void* gVidFuncs;

static void Log(const char* fmt, ...)
{
    char buf[512];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    OutputDebugStringA(buf);
}

static VidSystem* Sys(SVideo* pVideo)
{
    return VidSystem::FromVideo(pVideo);
}

EXPORT SVidVersion* __stdcall Vid_GetVersion()
{
    static SVidVersion version = {0x3FE7AE14, "OpenGL 3.3 video"};
    return &version;
}

EXPORT s32 __stdcall Vid_InitDLL(HINSTANCE hInstance, void* pFuncs)
{
    gDllInstance = hInstance;
    gVidFuncs = pFuncs;
    return 0;
}

EXPORT SVideo* __stdcall Vid_Init_SYS(HINSTANCE hInstance, u16 flags)
{
    // gta2gl.ini lives next to this DLL.
    char iniPath[MAX_PATH] = "gta2gl.ini";
    if (gDllInstance && GetModuleFileNameA(gDllInstance, iniPath, MAX_PATH))
    {
        char* slash = strrchr(iniPath, '\\');
        strcpy(slash ? slash + 1 : iniPath, "gta2gl.ini");
    }
    VidSystem* pSys = new VidSystem();
    pSys->Init(gDllInstance ? gDllInstance : hInstance, flags, gVidFuncs, iniPath);
    Log("glvideo: Vid_Init_SYS flags=%x\n", flags);
    InstallDInputHook(pSys->FrameCounter());
    return pSys->Video();
}

EXPORT s32 __stdcall Vid_CheckMode(SVideo* pVideo, s32 width, s32 height, s32 bpp)
{
    VidSystem* pSys = Sys(pVideo);
    return pSys ? pSys->CheckMode(width, height, bpp) : 0;
}

EXPORT SDisplayMode* __stdcall Vid_FindMode(SVideo* pVideo, s32 modeId)
{
    VidSystem* pSys = Sys(pVideo);
    return pSys ? pSys->FindMode(modeId) : 0;
}

EXPORT s32 __stdcall Vid_FindFirstMode(SVideo* pVideo, s32 bpp)
{
    VidSystem* pSys = Sys(pVideo);
    return pSys ? pSys->FindFirstMode(bpp) : 0;
}

EXPORT s32 __stdcall Vid_FindNextMode(SVideo* pVideo)
{
    VidSystem* pSys = Sys(pVideo);
    return pSys ? pSys->FindNextMode() : 0;
}

EXPORT SDevice* __stdcall Vid_FindDevice(SVideo* pVideo, s32 deviceId)
{
    VidSystem* pSys = Sys(pVideo);
    return pSys ? pSys->FindDevice(deviceId) : 0;
}

EXPORT s32 __stdcall Vid_SetDevice(SVideo*, s32)
{
    return 0;
}

EXPORT void __stdcall Vid_CloseScreen(SVideo*)
{
}

// Returns 0 on success (the game treats 1 as failure for windowed mode, non zero for full screen).
EXPORT s32 __stdcall Vid_SetMode(SVideo* pVideo, HWND hWnd, s32 modeId)
{
    VidSystem* pSys = Sys(pVideo);
    if (!pSys || !hWnd)
    {
        return 1;
    }
    s32 w = 640, h = 480;
    if (modeId == VidSystem::kWindowedModeId)
    {
        RECT rc;
        GetClientRect(hWnd, &rc);
        w = rc.right - rc.left;
        h = rc.bottom - rc.top;
    }
    else
    {
        SDisplayMode* pMode = pSys->FindMode(modeId);
        if (!pMode)
        {
            return 1;
        }
        w = pMode->field_8_width;
        h = pMode->field_C_height;
        // Borderless full screen: the window covers the monitor, the frame is scaled on present.
        SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOACTIVATE);
    }
    if (w <= 0 || h <= 0)
    {
        return 1;
    }
    pVideo->field_4C0_hwnd = hWnd;
    if (!pSys->HasContext())
    {
        const char* pError = "";
        IGLContext* pCtx = CreateGLContextFromNativeWindow(hWnd, &pError);
        if (!pCtx || !pSys->AttachContext(pCtx, &pError))
        {
            Log("glvideo: GL init failed: %s\n", pError);
            MessageBoxA(hWnd, pError, "glvideo: OpenGL 3.3 initialisation failed", MB_OK | MB_ICONERROR);
            return 1;
        }
    }
    pSys->SetMode(modeId, w, h);
    Log("glvideo: Vid_SetMode mode=%d %dx%d\n", modeId, w, h);
    return 0;
}

// Scroll Lock (unused by the game) toggles faithful rendering and the ini options. Polled here so the
// game's DirectInput keyboard is left alone.
// Enhancement: Scroll Lock switches between the faithful renderer and the gta2gl.ini options.
static void PollToggleKey(VidSystem* pSys, HWND hWnd)
{
    static bool sWasDown;
    const bool down = (GetAsyncKeyState(VK_SCROLL) & 0x8000) != 0 && GetForegroundWindow() == hWnd;
    if (down && !sWasDown)
    {
        pSys->ToggleFaithful();
    }
    sWasDown = down;
}

EXPORT void __stdcall Vid_FlipBuffers(SVideo* pVideo)
{
    VidSystem* pSys = Sys(pVideo);
    if (pSys)
    {
        PollToggleKey(pSys, (HWND)pVideo->field_4C0_hwnd);
        pSys->Flip();
    }
}

EXPORT void __stdcall Vid_ReleaseSurface(SVideo*)
{
}

EXPORT void __stdcall Vid_GrabSurface(SVideo*)
{
}

EXPORT void __stdcall Vid_ShutDown_SYS(SVideo* pVideo)
{
    delete Sys(pVideo);
}

EXPORT s32 __stdcall Vid_EnableWrites(SVideo* pVideo)
{
    VidSystem* pSys = Sys(pVideo);
    if (pSys)
    {
        pSys->GetSurface();
    }
    return 0;
}

EXPORT s32 __stdcall Vid_DisableWrites(SVideo* pVideo)
{
    VidSystem* pSys = Sys(pVideo);
    if (pSys)
    {
        pSys->FreeSurface();
    }
    return 0;
}

EXPORT s32 __stdcall Vid_GetSurface(SVideo* pVideo)
{
    VidSystem* pSys = Sys(pVideo);
    if (pSys)
    {
        pSys->GetSurface();
    }
    return 0;
}

EXPORT s32 __stdcall Vid_FreeSurface(SVideo* pVideo)
{
    VidSystem* pSys = Sys(pVideo);
    if (pSys)
    {
        pSys->FreeSurface();
    }
    return 0;
}

EXPORT s32 __stdcall Vid_ClearScreen(SVideo* pVideo, u8 r, u8 g, u8 b, s32 left, s32 top, s32 right, s32 bottom)
{
    VidSystem* pSys = Sys(pVideo);
    if (pSys)
    {
        pSys->Clear(r, g, b, left, top, right, bottom);
    }
    return 0;
}

EXPORT s32 __stdcall Vid_SetGamma(SVideo* pVideo, f32 r, f32 g, f32 b)
{
    VidSystem* pSys = Sys(pVideo);
    return pSys ? pSys->SetGamma(r, g, b) : 0;
}

EXPORT s32 __stdcall Vid_WindowProc(SVideo*, HWND, DWORD, WPARAM, LPARAM)
{
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID)
{
    return TRUE;
}
