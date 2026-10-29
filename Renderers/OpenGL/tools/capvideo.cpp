// Proxy for DMA's Dmavideo.dll that captures the back buffer before each flip (reference frames).
#include "../core/capture.hpp"
#include "../core/gta2_api.hpp"
#include "../dll/dinput_hook.hpp"
#include "d3dlog.hpp"
#include <ddraw.h>
#include <stdio.h>
#include <vector>
#include <windows.h>

#define EXPORT extern "C" __declspec(dllexport)

static HMODULE gReal;
static FrameCapture gCapture;
static u32 gFrame;

static FARPROC Real(const char* name)
{
    if (!gReal)
    {
        gReal = LoadLibraryA("Dmavideo.dll");
        gCapture.Init("ref");
        InstallDInputHook(&gFrame);
        InstallD3DLog(&gFrame);
    }
    return GetProcAddress(gReal, name);
}

#define FWD(ret, name, params, args)       \
    EXPORT ret __stdcall name params       \
    {                                      \
        typedef ret(__stdcall * T) params; \
        static T p = (T)Real(#name);       \
        return p args;                     \
    }

FWD(SVidVersion*, Vid_GetVersion, (), ())
FWD(SVideo*, Vid_Init_SYS, (HINSTANCE a, u16 b), (a, b))
FWD(s32, Vid_CheckMode, (SVideo * v, s32 w, s32 h, s32 bpp), (v, w, h, bpp))
FWD(SDisplayMode*, Vid_FindMode, (SVideo * v, s32 m), (v, m))
FWD(s32, Vid_FindFirstMode, (SVideo * v, s32 m), (v, m))
FWD(s32, Vid_FindNextMode, (SVideo * v), (v))
FWD(SDevice*, Vid_FindDevice, (SVideo * v, s32 d), (v, d))
FWD(s32, Vid_SetDevice, (SVideo * v, s32 d), (v, d))
FWD(void, Vid_CloseScreen, (SVideo * v), (v))
FWD(s32, Vid_SetMode, (SVideo * v, HWND h, s32 m), (v, h, m))
FWD(void, Vid_ReleaseSurface, (SVideo * v), (v))
FWD(void, Vid_GrabSurface, (SVideo * v), (v))
FWD(void, Vid_ShutDown_SYS, (SVideo * v), (v))
FWD(s32, Vid_EnableWrites, (SVideo * v), (v))
FWD(s32, Vid_DisableWrites, (SVideo * v), (v))
FWD(s32, Vid_GetSurface, (SVideo * v), (v))
FWD(s32, Vid_FreeSurface, (SVideo * v), (v))
FWD(s32, Vid_ClearScreen, (SVideo * v, u8 r, u8 g, u8 b, s32 l, s32 t, s32 rr, s32 bb), (v, r, g, b, l, t, rr, bb))
FWD(s32, Vid_SetGamma, (SVideo * v, f32 a, f32 b, f32 c), (v, a, b, c))
FWD(s32, Vid_WindowProc, (SVideo * v, HWND h, DWORD m, WPARAM w, LPARAM l), (v, h, m, w, l))
FWD(s32, Vid_InitDLL, (HINSTANCE h, void* f), (h, f))

static u32 Expand(u32 v, u32 mask)
{
    if (!mask)
    {
        return 0;
    }
    u32 shift = 0;
    while (!((mask >> shift) & 1))
    {
        shift++;
    }
    u32 bits = 0;
    while ((mask >> (shift + bits)) & 1)
    {
        bits++;
    }
    u32 c = (v & mask) >> shift;
    return bits >= 8 ? (c >> (bits - 8)) : ((c << (8 - bits)) | (c >> (2 * bits > 8 ? 2 * bits - 8 : 0)));
}

static void CaptureSurface(void* pSurf, const char* tag)
{
    if (!pSurf)
    {
        return;
    }
    IDirectDrawSurface7* s = (IDirectDrawSurface7*)pSurf;
    DDSURFACEDESC2 d;
    memset(&d, 0, sizeof(d));
    d.dwSize = sizeof(d);
    HRESULT hr = s->Lock(NULL, &d, DDLOCK_WAIT | DDLOCK_READONLY, NULL);
    char msg[256];
    snprintf(msg,
             sizeof(msg),
             "capvideo: %s lock hr=%08lx %lux%lu bpp=%lu pitch=%ld masks %lx %lx %lx\n",
             tag,
             hr,
             d.dwWidth,
             d.dwHeight,
             d.ddpfPixelFormat.dwRGBBitCount,
             d.lPitch,
             d.ddpfPixelFormat.dwRBitMask,
             d.ddpfPixelFormat.dwGBitMask,
             d.ddpfPixelFormat.dwBBitMask);
    OutputDebugStringA(msg);
    if (FAILED(hr))
    {
        return;
    }
    const s32 w = d.dwWidth, h = d.dwHeight;
    std::vector<u8> rgba((size_t)w * h * 4);
    const u32 bpp = d.ddpfPixelFormat.dwRGBBitCount;
    for (s32 y = 0; y < h; y++)
    {
        const u8* row = (const u8*)d.lpSurface + (size_t)y * d.lPitch;
        for (s32 x = 0; x < w; x++)
        {
            u32 v = bpp == 16 ? ((const u16*)row)[x] : ((const u32*)row)[x];
            u8* o = &rgba[((size_t)y * w + x) * 4];
            o[0] = (u8)Expand(v, d.ddpfPixelFormat.dwRBitMask);
            o[1] = (u8)Expand(v, d.ddpfPixelFormat.dwGBitMask);
            o[2] = (u8)Expand(v, d.ddpfPixelFormat.dwBBitMask);
            o[3] = 255;
        }
    }
    s->Unlock(NULL);
    std::string path = gCapture.PathFor(gFrame);
    if (tag[0] == 'p')
    {
        path.insert(path.size() - 4, "_primary");
    }
    FrameCapture::WriteBmp(path, &rgba[0], w, h, false);
}

EXPORT void __stdcall Vid_FlipBuffers(SVideo* v)
{
    typedef void(__stdcall * T)(SVideo*);
    static T p = (T)Real("Vid_FlipBuffers");
    D3DLogFlip();
    gFrame++;
    static u32 sStats = getenv("GTA2GL_STATS") ? (u32)atoi(getenv("GTA2GL_STATS")) : 0;
    static DWORD sStart;
    if (sStats && gFrame % sStats == 0)
    {
        const DWORD now = GetTickCount();
        if (sStart)
        {
            fprintf(stderr, "refstats frame %u: %.1f fps\n", gFrame, sStats * 1000.0 / (now - sStart));
        }
        sStart = now;
    }
    const bool want = gCapture.WantFrame(gFrame);
    if (want)
    {
        CaptureSurface(v->field_138_Surface, "back");
    }
    p(v);
}
