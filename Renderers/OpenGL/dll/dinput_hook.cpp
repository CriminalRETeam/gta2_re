// Hooks GetDeviceData and GetDeviceState in the DirectInput keyboard vtable (shared by the game's device).
// Always: drops NumLock events (see gDropNumLock). Test tools: GTA2GL_INPUT=<frame>:<dik>[:<frames held>],...
// feeds keys at those Vid_FlipBuffers counts (dik in hex or decimal); GTA2GL_KEYLOG=<file> logs what the game reads.
#define DIRECTINPUT_VERSION 0x0700
#include "dinput_hook.hpp"
#include <dinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <windows.h>

namespace
{
struct KeyEvent
{
    unsigned mFrame;
    unsigned mDik;
    bool mDown;
};

typedef HRESULT(__stdcall* TGetDeviceData)(IDirectInputDeviceA*, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD);
typedef HRESULT(__stdcall* TGetCapabilities)(IDirectInputDeviceA*, LPDIDEVCAPS);

typedef HRESULT(__stdcall* TGetDeviceState)(IDirectInputDeviceA*, DWORD, LPVOID);

TGetDeviceData gRealGetDeviceData;
TGetDeviceState gRealGetDeviceState;
std::vector<KeyEvent> gEvents; // sorted by frame
size_t gNext;
const unsigned* gFrameCounter;
FILE* gKeyLog; // GTA2GL_KEYLOG=<file>: every keyboard event the game reads, with the backlog left
// Wine on XWayland can flood the keyboard buffer with NumLock pairs; GTA2 reads one event per frame, so
// they delay every real key. GTA2 has no use for NumLock. Opt-out: GTA2GL_KEEP_NUMLOCK=1.
bool gDropNumLock;
DWORD gSequence = 1;

HRESULT ReadKeyboard(IDirectInputDeviceA* pDev, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);

bool IsKeyboard(IDirectInputDeviceA* pDev)
{
    static IDirectInputDeviceA* sKnown[8];
    static bool sIsKb[8];
    for (int i = 0; i < 8; i++)
    {
        if (sKnown[i] == pDev)
        {
            return sIsKb[i];
        }
    }
    DIDEVCAPS caps;
    memset(&caps, 0, sizeof(caps));
    caps.dwSize = sizeof(DIDEVCAPS_DX3);
    const bool kb = SUCCEEDED(pDev->GetCapabilities(&caps)) && (caps.dwDevType & 0xFF) == DIDEVTYPE_KEYBOARD;
    for (int i = 0; i < 8; i++)
    {
        if (!sKnown[i])
        {
            sKnown[i] = pDev;
            sIsKb[i] = kb;
            break;
        }
    }
    return kb;
}

DWORD DueCount()
{
    DWORD n = 0;
    for (size_t i = gNext; i < gEvents.size() && gEvents[i].mFrame <= *gFrameCounter; i++)
    {
        n++;
    }
    return n;
}

HRESULT __stdcall HookGetDeviceData(IDirectInputDeviceA* pDev,
                                    DWORD cbObjectData,
                                    LPDIDEVICEOBJECTDATA rgdod,
                                    LPDWORD pdwInOut,
                                    DWORD dwFlags)
{
    if (!IsKeyboard(pDev))
    {
        return gRealGetDeviceData(pDev, cbObjectData, rgdod, pdwInOut, dwFlags);
    }
    if (gEvents.empty())
    {
        return ReadKeyboard(pDev, cbObjectData, rgdod, pdwInOut, dwFlags);
    }
    const DWORD due = DueCount();
    if (!rgdod || (dwFlags & DIGDD_PEEK))
    {
        HRESULT hr = gRealGetDeviceData(pDev, cbObjectData, rgdod, pdwInOut, dwFlags);
        if (FAILED(hr) && pdwInOut)
        {
            *pdwInOut = 0;
        }
        if (pdwInOut && (dwFlags & DIGDD_PEEK))
        {
            *pdwInOut += due;
        }
        return DI_OK;
    }
    if (due && pdwInOut && *pdwInOut >= 1)
    {
        const KeyEvent& e = gEvents[gNext++];
        memset(rgdod, 0, cbObjectData);
        rgdod->dwOfs = e.mDik;
        rgdod->dwData = e.mDown ? 0x80 : 0;
        rgdod->dwTimeStamp = GetTickCount();
        rgdod->dwSequence = gSequence++;
        *pdwInOut = 1;
        return DI_OK;
    }
    HRESULT hr = ReadKeyboard(pDev, cbObjectData, rgdod, pdwInOut, dwFlags);
    if (FAILED(hr) && pdwInOut)
    {
        *pdwInOut = 0;
        return DI_OK;
    }
    return hr;
}

HRESULT ReadKeyboard(IDirectInputDeviceA* pDev, DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
    const DWORD requested = (pdwInOut && rgdod && !(dwFlags & DIGDD_PEEK)) ? *pdwInOut : 0;
    HRESULT hr = gRealGetDeviceData(pDev, cbObjectData, rgdod, pdwInOut, dwFlags);
    for (int i = 0; i < 256 && gDropNumLock && requested == 1 && SUCCEEDED(hr) && *pdwInOut == 1 && rgdod->dwOfs == DIK_NUMLOCK; i++)
    {
        *pdwInOut = 1;
        hr = gRealGetDeviceData(pDev, cbObjectData, rgdod, pdwInOut, dwFlags);
    }
    if (gKeyLog && rgdod && SUCCEEDED(hr) && pdwInOut && *pdwInOut > 0)
    {
        DWORD left = INFINITE;
        gRealGetDeviceData(pDev, cbObjectData, 0, &left, DIGDD_PEEK);
        fprintf(gKeyLog,
                "%u frame %u dik %02x %s left %u\n",
                (unsigned)GetTickCount(),
                gFrameCounter ? *gFrameCounter : 0,
                (unsigned)rgdod->dwOfs,
                (rgdod->dwData & 0x80) ? "down" : "up",
                (unsigned)left);
        fflush(gKeyLog);
    }
    return hr;
}

// Immediate mode (the frontend polls GetDeviceState): keys held between their down and up frames.
HRESULT __stdcall HookGetDeviceState(IDirectInputDeviceA* pDev, DWORD size, LPVOID data)
{
    HRESULT hr = gRealGetDeviceState(pDev, size, data);
    if (gEvents.empty() || size != 256 || !data || !IsKeyboard(pDev))
    {
        return hr;
    }
    if (FAILED(hr))
    {
        memset(data, 0, size);
        hr = DI_OK;
    }
    BYTE* keys = (BYTE*)data;
    for (size_t i = 0; i < gEvents.size(); i++)
    {
        const KeyEvent& e = gEvents[i];
        if (e.mDown && e.mFrame <= *gFrameCounter)
        {
            // Held until its matching up event.
            for (size_t j = i + 1; j < gEvents.size(); j++)
            {
                if (!gEvents[j].mDown && gEvents[j].mDik == e.mDik)
                {
                    if (gEvents[j].mFrame > *gFrameCounter)
                    {
                        keys[e.mDik & 0xFF] = 0x80;
                    }
                    break;
                }
            }
        }
    }
    return hr;
}

void Parse(const char* s)
{
    while (s && *s)
    {
        char* end = 0;
        KeyEvent down;
        down.mFrame = (unsigned)strtoul(s, &end, 10);
        if (*end != ':')
        {
            break;
        }
        down.mDik = (unsigned)strtoul(end + 1, &end, 0);
        down.mDown = true;
        unsigned held = 2;
        if (*end == ':')
        {
            held = (unsigned)strtoul(end + 1, &end, 10);
        }
        KeyEvent up = down;
        up.mFrame += held;
        up.mDown = false;
        gEvents.push_back(down);
        gEvents.push_back(up);
        s = (*end == ',') ? end + 1 : 0;
    }
    // Stable insertion sort by frame keeps the down/up order of each key.
    for (size_t i = 1; i < gEvents.size(); i++)
    {
        KeyEvent e = gEvents[i];
        size_t j = i;
        while (j > 0 && gEvents[j - 1].mFrame > e.mFrame)
        {
            gEvents[j] = gEvents[j - 1];
            j--;
        }
        gEvents[j] = e;
    }
}
} // namespace

void InstallDInputHook(const unsigned* pFrameCounter)
{
    const char* env = getenv("GTA2GL_INPUT");
    const char* keyLog = getenv("GTA2GL_KEYLOG");
    gDropNumLock = getenv("GTA2GL_KEEP_NUMLOCK") == NULL;
    if (((!env || !*env) && (!keyLog || !*keyLog) && !gDropNumLock) || gRealGetDeviceData)
    {
        return;
    }
    if (keyLog && *keyLog)
    {
        gKeyLog = fopen(keyLog, "w");
    }
    Parse(env);
    gFrameCounter = pFrameCounter;

    // Any keyboard device created here shares its vtable with the game's.
    typedef HRESULT(WINAPI * TCreate)(HINSTANCE, DWORD, LPDIRECTINPUTA*, LPUNKNOWN);
    HMODULE hDi = LoadLibraryA("dinput.dll");
    TCreate pCreate = hDi ? (TCreate)GetProcAddress(hDi, "DirectInputCreateA") : 0;
    LPDIRECTINPUTA pDi = 0;
    LPDIRECTINPUTDEVICEA pDev = 0;
    if (!pCreate || FAILED(pCreate(GetModuleHandleA(0), 0x700, &pDi, 0)) || FAILED(pDi->CreateDevice(GUID_SysKeyboard, &pDev, 0)))
    {
        OutputDebugStringA("dinput_hook: DirectInput unavailable\n");
        return;
    }
    void** vtbl = *(void***)pDev;
    DWORD old;
    VirtualProtect(&vtbl[10], sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
    gRealGetDeviceData = (TGetDeviceData)vtbl[10];
    vtbl[10] = (void*)HookGetDeviceData;
    VirtualProtect(&vtbl[10], sizeof(void*), old, &old);
    VirtualProtect(&vtbl[9], sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
    gRealGetDeviceState = (TGetDeviceState)vtbl[9];
    vtbl[9] = (void*)HookGetDeviceState;
    VirtualProtect(&vtbl[9], sizeof(void*), old, &old);
    // pDev and pDi stay alive so the vtable stays valid.
    char msg[128];
    snprintf(msg, sizeof(msg), "dinput_hook: %u scheduled key events\n", (unsigned)gEvents.size());
    OutputDebugStringA(msg);
}
