// Reference command log (capvideo.dll): wraps the gbh_* draw entry points the game resolves from
// DMA's d3ddll.dll (through the exe's GetProcAddress import) and hooks the IDirect3DDevice3 that
// d3ddll draws with, so each gbh call is followed by the D3D primitives it produced.
#include "d3dlog.hpp"
#include "../core/cmdlog.hpp"
#include "../core/gta2_api.hpp"
#include <ddraw.h>
#include <string.h>
#include <windows.h>

namespace
{
CmdLog gLog;
const u32* gFlips;
u32 gState[256];
u32 gCurTex;
s32 gCurTexW, gCurTexH;
HMODULE gD3dDll;

// IDirect3DDevice3 vtable slots.
const int kSetRenderState = 22;
const int kDrawPrimitive = 28;
const int kDrawIndexedPrimitive = 29;
const int kSetTexture = 38;
const int kBegin = 16;
const int kDrawPrimitiveStrided = 32;
const int kDrawPrimitiveVB = 34;

typedef HRESULT(__stdcall* TSetRenderState)(void*, DWORD, DWORD);
typedef HRESULT(__stdcall* TDrawPrimitive)(void*, DWORD, DWORD, void*, DWORD, DWORD);
typedef HRESULT(__stdcall* TDrawIndexedPrimitive)(void*, DWORD, DWORD, void*, DWORD, WORD*, DWORD, DWORD);
typedef HRESULT(__stdcall* TSetTexture)(void*, DWORD, IUnknown*);
typedef HRESULT(__stdcall* TGeneric5)(void*, DWORD, DWORD, DWORD, DWORD, DWORD);

TSetRenderState gRealSetRenderState;
TDrawPrimitive gRealDrawPrimitive;
TDrawIndexedPrimitive gRealDrawIndexedPrimitive;
TSetTexture gRealSetTexture;
void* gRealOther[64];

const char* PrimName(DWORD t)
{
    switch (t)
    {
        case 4:
            return "list";
        case 5:
            return "strip";
        case 6:
            return "fan";
        default:
            return "other";
    }
}

void LogVerts(DWORD fvf, const u8* p, DWORD count)
{
    if (fvf != 0x1C4)
    {
        char note[64];
        snprintf(note, sizeof(note), "fvf %lx not logged", (unsigned long)fvf);
        gLog.Note(note);
        return;
    }
    for (DWORD i = 0; i < count; i++)
    {
        const f32* f = (const f32*)(p + i * 32);
        const u32* d = (const u32*)(p + i * 32);
        gLog.Vertex(f[0], f[1], f[2], f[3], d[4], d[5], f[6] * gCurTexW, f[7] * gCurTexH);
    }
}

void PrimHeader(DWORD type, DWORD count)
{
    const u32 st[5] = {gState[27], gState[19], gState[20], gState[17], gState[18]};
    gLog.Prim(PrimName(type), (s32)count, st, gCurTex, gCurTexW, gCurTexH);
}

HRESULT __stdcall HookSetRenderState(void* dev, DWORD st, DWORD v)
{
    if (st < 256)
    {
        gState[st] = v;
    }
    return gRealSetRenderState(dev, st, v);
}

HRESULT __stdcall HookDrawPrimitive(void* dev, DWORD type, DWORD fvf, void* verts, DWORD count, DWORD flags)
{
    gLog.Update(*gFlips);
    if (gLog.Active())
    {
        PrimHeader(type, count);
        LogVerts(fvf, (const u8*)verts, count);
    }
    return gRealDrawPrimitive(dev, type, fvf, verts, count, flags);
}

HRESULT __stdcall HookDrawIndexedPrimitive(void* dev, DWORD type, DWORD fvf, void* verts, DWORD count, WORD* idx, DWORD nIdx, DWORD flags)
{
    gLog.Update(*gFlips);
    if (gLog.Active())
    {
        PrimHeader(type, nIdx);
        for (DWORD i = 0; i < nIdx; i++)
        {
            LogVerts(fvf, (const u8*)verts + idx[i] * 32, 1);
        }
    }
    return gRealDrawIndexedPrimitive(dev, type, fvf, verts, count, idx, nIdx, flags);
}

HRESULT __stdcall HookSetTexture(void* dev, DWORD stage, IUnknown* tex)
{
    if (stage == 0)
    {
        gCurTex = (u32)(DWORD_PTR)tex;
        gCurTexW = gCurTexH = 1;
        IDirectDrawSurface4* surf = 0;
        if (tex && SUCCEEDED(tex->QueryInterface(IID_IDirectDrawSurface4, (void**)&surf)) && surf)
        {
            DDSURFACEDESC2 d;
            memset(&d, 0, sizeof(d));
            d.dwSize = sizeof(d);
            if (SUCCEEDED(surf->GetSurfaceDesc(&d)))
            {
                gCurTexW = d.dwWidth;
                gCurTexH = d.dwHeight;
            }
            surf->Release();
        }
    }
    return gRealSetTexture(dev, stage, tex);
}

// Draw paths d3ddll is not expected to use: noted so a silent gap cannot hide in the diff.
template<int Slot>
HRESULT __stdcall HookUnexpected(void* dev, DWORD a, DWORD b, DWORD c, DWORD d, DWORD e)
{
    gLog.Update(*gFlips);
    char note[64];
    snprintf(note, sizeof(note), "unexpected device call slot %d", Slot);
    gLog.Note(note);
    return ((TGeneric5)gRealOther[Slot])(dev, a, b, c, d, e);
}

void Patch(void** vtbl, int slot, void* hook, void** real)
{
    DWORD old;
    VirtualProtect(&vtbl[slot], sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
    *real = vtbl[slot];
    vtbl[slot] = hook;
    VirtualProtect(&vtbl[slot], sizeof(void*), old, &old);
}

void TryHookDevice()
{
    if (gRealDrawPrimitive || !gLog.Enabled())
    {
        return;
    }
    if (!gD3dDll)
    {
        gD3dDll = GetModuleHandleA("d3ddll.dll");
    }
    if (!gD3dDll)
    {
        return;
    }
    // d3ddll's global device holder (0xe485e0 with the DLL at its 0xe00000 base), device at +0x28.
    u8* holder = *(u8**)((u8*)gD3dDll + 0x485e0);
    void* dev = holder ? *(void**)(holder + 0x28) : 0;
    if (!dev)
    {
        return;
    }
    void** vtbl = *(void***)dev;
    Patch(vtbl, kSetRenderState, (void*)HookSetRenderState, (void**)&gRealSetRenderState);
    Patch(vtbl, kSetTexture, (void*)HookSetTexture, (void**)&gRealSetTexture);
    Patch(vtbl, kDrawIndexedPrimitive, (void*)HookDrawIndexedPrimitive, (void**)&gRealDrawIndexedPrimitive);
    Patch(vtbl, kBegin, (void*)HookUnexpected<kBegin>, &gRealOther[kBegin]);
    Patch(vtbl, kDrawPrimitiveStrided, (void*)HookUnexpected<kDrawPrimitiveStrided>, &gRealOther[kDrawPrimitiveStrided]);
    Patch(vtbl, kDrawPrimitiveVB, (void*)HookUnexpected<kDrawPrimitiveVB>, &gRealOther[kDrawPrimitiveVB]);
    Patch(vtbl, kDrawPrimitive, (void*)HookDrawPrimitive, (void**)&gRealDrawPrimitive);
    // Render states set before the hook are unknown; d3ddll's init values (0xe01a90).
    gState[27] = 0;
    gState[19] = 5;
    gState[20] = 6;
    gState[17] = 2;
    gState[18] = 2;
}

// gbh wrappers.
typedef void(__stdcall* TDrawQuad)(s32, STexture*, Vert*, s32);
typedef s32(__stdcall* TDrawTile)(u32, STexture*, Vert*, s32);
typedef s32(__stdcall* TBlitImage)(s32, s32, s32, s32, s32, s32, s32);
typedef s32(__stdcall* TDrawFlatRect)(Vert*, s32);
TDrawQuad gRealDrawQuad, gRealDrawTriangle;
TDrawTile gRealDrawTile, gRealDrawTilePart;
TBlitImage gRealBlitImage;
TDrawFlatRect gRealDrawFlatRect;

void LogCall(const char* name, u32 flags, STexture* t, u32 colour)
{
    TryHookDevice();
    gLog.Update(*gFlips);
    gLog.Call(name, flags, t ? t->field_0_id : 0, t ? t->field_E_width : 0, t ? t->field_10_height : 0, colour & 0xFF);
}

void __stdcall WrapDrawQuad(s32 f, STexture* t, Vert* v, s32 c)
{
    LogCall("DrawQuad", f, t, c);
    gRealDrawQuad(f, t, v, c);
}
void __stdcall WrapDrawTriangle(s32 f, STexture* t, Vert* v, s32 c)
{
    LogCall("DrawTriangle", f, t, c);
    gRealDrawTriangle(f, t, v, c);
}
s32 __stdcall WrapDrawTile(u32 f, STexture* t, Vert* v, s32 c)
{
    LogCall("DrawTile", f, t, c);
    return gRealDrawTile(f, t, v, c);
}
s32 __stdcall WrapDrawTilePart(u32 f, STexture* t, Vert* v, s32 c)
{
    LogCall("DrawTilePart", f, t, c);
    return gRealDrawTilePart(f, t, v, c);
}
s32 __stdcall WrapBlitImage(s32 i, s32 a, s32 b, s32 c, s32 d, s32 x, s32 y)
{
    LogCall("BlitImage", i, 0, 0);
    return gRealBlitImage(i, a, b, c, d, x, y);
}
s32 __stdcall WrapDrawFlatRect(Vert* v, s32 c)
{
    LogCall("DrawFlatRect", 0, 0, c);
    return gRealDrawFlatRect(v, c);
}

typedef FARPROC(WINAPI* TGetProcAddress)(HMODULE, LPCSTR);
TGetProcAddress gRealGetProcAddress;

FARPROC WINAPI HookGetProcAddress(HMODULE h, LPCSTR name)
{
    FARPROC p = gRealGetProcAddress(h, name);
    if (!p || ((DWORD_PTR)name >> 16) == 0)
    {
        return p;
    }
    struct Entry
    {
        const char* mName;
        void** mReal;
        void* mWrap;
    };
    static const Entry kEntries[] = {
        {"gbh_DrawQuad", (void**)&gRealDrawQuad, (void*)WrapDrawQuad},
        {"gbh_DrawTriangle", (void**)&gRealDrawTriangle, (void*)WrapDrawTriangle},
        {"gbh_DrawTile", (void**)&gRealDrawTile, (void*)WrapDrawTile},
        {"gbh_DrawTilePart", (void**)&gRealDrawTilePart, (void*)WrapDrawTilePart},
        {"gbh_BlitImage", (void**)&gRealBlitImage, (void*)WrapBlitImage},
        {"gbh_DrawFlatRect", (void**)&gRealDrawFlatRect, (void*)WrapDrawFlatRect},
    };
    for (size_t i = 0; i < sizeof(kEntries) / sizeof(kEntries[0]); i++)
    {
        if (strcmp(name, kEntries[i].mName) == 0)
        {
            *kEntries[i].mReal = (void*)p;
            return (FARPROC)kEntries[i].mWrap;
        }
    }
    return p;
}
} // namespace

void InstallD3DLog(const u32* pFlipCounter)
{
    gLog.Init("d3d");
    if (!gLog.Enabled())
    {
        return;
    }
    gFlips = pFlipCounter;
    u8* base = (u8*)GetModuleHandleA(0);
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + ((IMAGE_DOS_HEADER*)base)->e_lfanew);
    const IMAGE_DATA_DIRECTORY& dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    for (IMAGE_IMPORT_DESCRIPTOR* imp = (IMAGE_IMPORT_DESCRIPTOR*)(base + dir.VirtualAddress); dir.VirtualAddress && imp->Name; imp++)
    {
        if (_stricmp((const char*)(base + imp->Name), "kernel32.dll") != 0 || !imp->OriginalFirstThunk)
        {
            continue;
        }
        IMAGE_THUNK_DATA* names = (IMAGE_THUNK_DATA*)(base + imp->OriginalFirstThunk);
        IMAGE_THUNK_DATA* iat = (IMAGE_THUNK_DATA*)(base + imp->FirstThunk);
        for (; names->u1.AddressOfData; names++, iat++)
        {
            if (!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal) &&
                strcmp((const char*)((IMAGE_IMPORT_BY_NAME*)(base + names->u1.AddressOfData))->Name, "GetProcAddress") == 0)
            {
                DWORD old;
                VirtualProtect(&iat->u1.Function, sizeof(void*), PAGE_READWRITE, &old);
                gRealGetProcAddress = (TGetProcAddress)iat->u1.Function;
                iat->u1.Function = (DWORD_PTR)HookGetProcAddress;
                VirtualProtect(&iat->u1.Function, sizeof(void*), old, &old);
            }
        }
    }
}

void D3DLogFlip()
{
    TryHookDevice();
}
