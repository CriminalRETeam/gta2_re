// gbh_* render API on top of IRenderer. Logic ported from GTA2Hax d3ddll.cpp; portable C++.
#include "gbh_api.hpp"
#include "cmdlog.hpp"
#include "renderer.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace
{
struct SGlobals
{
    u32 mNumPolysDrawn;
    u32 mNumTextureSwaps;
    u32 mNumBatchFlushes;
    u32 mSceneTime;
    u32 mCacheSizes[12];
    u32 mCacheSizes2[12];
    u32 mCacheHitRates[12];
    u32 mCacheUnknown[12];
    void* mCaches[12];
};

const u32 kSGlobalsValues[] = {10, 62, 62, 88, 1, 0, 110, 126, 126, 40, 10, 0};

// Texture variants, as the original keeps two palettes per palette id: opaque draws use the
// opaque texture format (RGB565 in the reference), draws with alpha flags the alpha format (ARGB4444).
enum
{
    kOpaque = 0,
    kAlpha = 1,
};

// field_18_pPaltData of a texture points at m565, the start of this struct.
struct PalEntry
{
    u16 m565[512];
    u32 mRgba[2][256];
    u32 mRaw[256]; // game palette colours, 0x00RRGGBB
};

// field_1C_ptr of a texture.
struct TexData
{
    RTexture* mTex[2];
    bool mDirty[2];
    bool mImage; // 16-bit TGA image, already converted to mPixels
    u32 mEpoch; // gTextureEpoch of the last conversion
    std::vector<u32> mPixels;
};

struct ImageEntry
{
    s32 mLoaded;
    u32 mWidth;
    u32 mHeight;
    STexture* mTexture;
};

struct LightInternal
{
    u32 mFlags;
    f32 mBrightness;
    f32 mRadius;
    f32 mRadiusSquared;
    f32 mInvRadius;
    f32 mX, mY, mZ;
    f32 mR, mG, mB;
};

IRenderer* gRenderer;
SGlobals gGlobals;
PalEntry* gPals[16384];
std::vector<ImageEntry> gImages;
LightInternal gLights[256];
u32 gNumLights;
f32 gAmbient = 255.0f;
f32 gWindow[4];
s32 gScreenTable[1700];
u32 gTextureId;
CmdLog gCmd;
const u32* gFlipCounter;

void LogCall(const char* func, u32 flags, STexture* t, u32 colour)
{
    if (gFlipCounter)
    {
        gCmd.Update(*gFlipCounter);
    }
    gCmd.Call(func, flags, t ? t->field_0_id : 0, t ? t->field_E_width : 0, t ? t->field_10_height : 0, colour & 0xFF);
}

// Same fields as the reference log of the D3D calls: D3D render state values, texel units.
void LogPrim(const Vert* pVerts, s32 count, u32 state, STexture* t, bool blit)
{
    if (!gCmd.Active())
    {
        return;
    }
    u32 st[5] = {0, 0, 0, 2, 2};
    if (state & RS_BLEND_ADD)
    {
        st[0] = 1, st[1] = 2, st[2] = 2;
    }
    else if (state & RS_BLEND_ALPHA)
    {
        st[0] = 1, st[1] = 5, st[2] = 6;
    }
    if (!(state & RS_FILTER_LINEAR))
    {
        st[3] = st[4] = 1;
    }
    gCmd.Prim(blit ? "blit" : (count == 4 ? "fan" : "list"),
              count,
              st,
              t ? t->field_0_id : 0,
              t ? t->field_E_width : 0,
              t ? t->field_10_height : 0);
    for (s32 i = 0; i < count; i++)
    {
        gCmd.Vertex(pVerts[i].x, pVerts[i].y, pVerts[i].z, pVerts[i].w, pVerts[i].diff, pVerts[i].spec, pVerts[i].u, pVerts[i].v);
    }
}
// Enhancement state (gta2gl.ini).
bool gTrueColour; // 8-bit palette channels instead of the original's 16-bit textures
u32 gOptionsGeneration = ~0u;
u32 gTextureEpoch; // bumped when every texture has to be converted again
bool gLightsDirty = true;
std::vector<u32> gConvertBuffer;

// The original keeps textures in square cache slots of 8..256 texels (0xe107e0), with the last column
// and row repeated once; UVs are normalised by the slot size. Same here, so UVs match bit for bit.
s32 SlotSize(s32 w, s32 h)
{
    const s32 m = w > h ? w : h;
    s32 slot = 8;
    while (slot < m && slot < 256)
    {
        slot <<= 1;
    }
    return slot < m ? m : slot;
}

const u32 FLAG_FIT_TO_TEXTURE = 0x10000;
const u32 FLAG_NO_FILTER = 0x20000;
const u32 FLAG_LIGHTING = 0x8000;
const u32 FLAG_TILE_UV = 0x4000;
const u32 FLAG_VERTEX_DIFFUSE = 0x2000;
const u32 FLAG_BLEND_ADD = 0x200;
const u32 FLAG_BLEND_ALPHA = 0x100;
const u32 FLAG_COLOUR_KEY = 0x80;

void InitGlobals()
{
    memset(&gGlobals, 0, sizeof(gGlobals));
    for (int i = 0; i < 12; i++)
    {
        gGlobals.mCacheUnknown[i] = kSGlobalsValues[i];
    }
}

u16 To565(u32 rgb)
{
    const u32 r = (rgb >> 16) & 0xFF, g = (rgb >> 8) & 0xFF, b = rgb & 0xFF;
    return (u16)(((r * 31 / 255) << 11) | ((g * 63 / 255) << 5) | (b * 31 / 255));
}

TexData* Data(STexture* pTexture)
{
    return pTexture ? (TexData*)pTexture->field_1C_ptr : 0;
}

STexture* NewTexture(s32 width, s32 height, u8* pData)
{
    STexture* pTexture = (STexture*)malloc(sizeof(STexture));
    if (!pTexture)
    {
        return 0;
    }
    memset(pTexture, 0, sizeof(STexture));
    pTexture->field_0_id = (u16)gTextureId++;
    pTexture->field_E_width = (u16)width;
    pTexture->field_10_height = (u16)height;
    pTexture->field_14_original_pixel_data_ptr = pData;
    TexData* pData2 = new TexData();
    pData2->mTex[kOpaque] = 0;
    pData2->mTex[kAlpha] = 0;
    pData2->mDirty[kOpaque] = pData2->mDirty[kAlpha] = true;
    pData2->mImage = false;
    pData2->mEpoch = gTextureEpoch;
    pTexture->field_1C_ptr = pData2;
    return pTexture;
}

// Converts on first use: the game registers textures whose pixels are only valid later (or never).
RTexture* Resolve(STexture* pTexture, int variant)
{
    TexData* pData = Data(pTexture);
    if (!pData || !gRenderer)
    {
        return 0;
    }
    if (pData->mImage)
    {
        variant = kOpaque;
    }
    if (!pData->mTex[variant])
    {
        pData->mTex[variant] = gRenderer->CreateTexture();
    }
    if (pData->mEpoch != gTextureEpoch)
    {
        pData->mEpoch = gTextureEpoch;
        pData->mDirty[kOpaque] = pData->mDirty[kAlpha] = true;
    }
    if (pData->mDirty[variant])
    {
        const s32 w = pTexture->field_E_width;
        const s32 h = pTexture->field_10_height;
        if (pData->mImage)
        {
            gRenderer->UploadTexture(pData->mTex[variant], w, h, &pData->mPixels[0]);
        }
        else if (pTexture->field_14_original_pixel_data_ptr && pTexture->field_18_pPaltData && w > 0 && h > 0)
        {
            const u32* pal = ((const PalEntry*)pTexture->field_18_pPaltData)->mRgba[variant];
            const s32 slot = SlotSize(w, h);
            gConvertBuffer.assign((size_t)slot * slot, 0);
            for (s32 y = 0; y < h; y++)
            {
                // Sprite and tile pages are 256 pixels wide.
                const u8* src = pTexture->field_14_original_pixel_data_ptr + (size_t)y * 256;
                u32* dst = &gConvertBuffer[(size_t)y * slot];
                for (s32 x = 0; x < w; x++)
                {
                    dst[x] = pal[src[x]];
                }
                if (w < slot)
                {
                    dst[w] = dst[w - 1];
                }
            }
            if (h < slot)
            {
                memcpy(&gConvertBuffer[(size_t)h * slot], &gConvertBuffer[(size_t)(h - 1) * slot], (size_t)slot * 4);
            }
            gRenderer->UploadTexture(pData->mTex[variant], slot, slot, &gConvertBuffer[0]);
        }
        else
        {
            return 0;
        }
        pData->mDirty[variant] = false;
    }
    return pData->mTex[variant];
}

void ConvertPalette(PalEntry* pPal)
{
    for (s32 i = 0; i < 256; i++)
    {
        const u32 c = pPal->mRaw[i];
        const u32 r = (c >> 16) & 0xFF, g = (c >> 8) & 0xFF, b = c & 0xFF;
        const u32 a = i == 0 ? 0 : 0xFF000000u;
        if (gTrueColour) // enhancement
        {
            pPal->mRgba[kOpaque][i] = pPal->mRgba[kAlpha][i] = r | (g << 8) | (b << 16) | a;
            continue;
        }
        // 16-bit textures: bits truncated, expanded by replication when sampled.
        pPal->mRgba[kOpaque][i] = ((r & 0xF8) | (r >> 5)) | (((g & 0xFC) | (g >> 6)) << 8) | (((b & 0xF8) | (b >> 5)) << 16) | a;
        pPal->mRgba[kAlpha][i] = ((r >> 4) * 17) | (((g >> 4) * 17) << 8) | (((b >> 4) * 17) << 16) | a;
    }
}

// Enhancement: follows option changes (live toggle, ini edits); palettes and textures are converted again.
void SyncOptions()
{
    if (!gRenderer || gRenderer->Options().mGeneration == gOptionsGeneration)
    {
        return;
    }
    gOptionsGeneration = gRenderer->Options().mGeneration;
    const bool trueColour = gRenderer->Options().mTrueColour;
    if (trueColour != gTrueColour)
    {
        gTrueColour = trueColour;
        for (s32 i = 0; i < 16384; i++)
        {
            if (gPals[i])
            {
                ConvertPalette(gPals[i]);
            }
        }
        gTextureEpoch++;
    }
    gLightsDirty = true;
}

// Enhancement: the active lights for per pixel lighting.
void UploadLights()
{
    static RLight sLights[256];
    s32 n = 0;
    for (u32 j = 0; j < gNumLights; j++)
    {
        const LightInternal& l = gLights[j];
        if ((l.mFlags & 0x30000) != 0x10000)
        {
            continue;
        }
        RLight& o = sLights[n++];
        o.x = l.mX, o.y = l.mY, o.z = l.mZ;
        o.radius = l.mRadius;
        o.invRadius = l.mInvRadius;
        o.r = l.mR * l.mBrightness, o.g = l.mG * l.mBrightness, o.b = l.mB * l.mBrightness;
    }
    gRenderer->SetLights(sLights, n, gAmbient);
    gLightsDirty = false;
}

// The original's table driven square root (12 bit mantissa index, 4096 entries).
u32 gSqrtTable[4096];

void InitSqrtTable()
{
    for (u32 i = 0; i < 2048; i++)
    {
        union
        {
            u32 u;
            f32 f;
        } a, b;
        a.u = (i << 12) | 0x3F800000u; // [1, 2)
        b.u = (i << 12) | 0x40000000u; // [2, 4)
        a.f = (f32)sqrt((double)a.f);
        b.f = (f32)sqrt((double)b.f);
        gSqrtTable[i] = a.u & 0x7FFFFF;
        gSqrtTable[2048 + i] = b.u & 0x7FFFFF;
    }
}

f32 TableSqrt(f32 x)
{
    union
    {
        u32 u;
        f32 f;
    } v;
    v.f = x;
    u32 mant = v.u & 0x7FFFFF;
    u32 exp = (v.u & 0x7F800000u) >> 1;
    if (exp & 0x400000)
    {
        exp += 0x20000000u;
    }
    else
    {
        exp += 0x1F800000u;
        mant |= 0x800000;
    }
    v.u = gSqrtTable[mant >> 12] | (exp & 0x7F800000u);
    return v.f;
}

u32 LightVertex(const Vert& vert, u8 shade, u32 colour)
{
    f32 lr = 0.0f, lg = 0.0f, lb = 0.0f;
    for (u32 j = 0; j < gNumLights; j++)
    {
        const LightInternal& light = gLights[j];
        if ((light.mFlags & 0x30000) != 0x10000)
        {
            continue;
        }
        const f32 dx = vert.x - light.mX;
        const f32 dy = vert.y - light.mY;
        const f32 dz = vert.z - light.mZ;
        const f32 d2 = dx * dx + dy * dy + dz * dz;
        if (d2 > light.mRadiusSquared)
        {
            continue;
        }
        const f32 att = (light.mRadius - TableSqrt(d2)) * light.mInvRadius;
        if (att > 0.0f)
        {
            const f32 k = att * light.mBrightness;
            lr += light.mR * k;
            lg += light.mG * k;
            lb += light.mB * k;
        }
    }
    const f32 n = shade / 255.0f;
    f32 r = n * ((colour >> 16) & 0xFF) * lr + gAmbient;
    f32 g = n * ((colour >> 8) & 0xFF) * lg + gAmbient;
    f32 b = n * (colour & 0xFF) * lb + gAmbient;
    const u32 ri = r > 255.0f ? 255 : (u32)r;
    const u32 gi = g > 255.0f ? 255 : (u32)g;
    const u32 bi = b > 255.0f ? 255 : (u32)b;
    return (colour & 0xFF000000u) | (ri << 16) | (gi << 8) | bi;
}

// D3D ARGB to the renderer's RGBA byte order.
u32 ArgbToRgba(u32 c)
{
    return ((c >> 16) & 0xFF) | (c & 0xFF00) | ((c & 0xFF) << 16) | (c & 0xFF000000u);
}

const u32 FLAG_INTERNAL_BLIT = 0x80000000u;

void RenderVerts(u32 flags, STexture* pTexture, Vert* pVerts, s32 count, u32 baseColour)
{
    if (count < 3 || count > 4 || pVerts[0].z <= 0.0f || !gRenderer)
    {
        return;
    }
    // As the original: skip primitives entirely outside the gbh_SetWindow rectangle.
    f32 minX = pVerts[0].x, maxX = minX, minY = pVerts[0].y, maxY = minY;
    for (s32 i = 1; i < count; i++)
    {
        minX = pVerts[i].x < minX ? pVerts[i].x : minX;
        maxX = pVerts[i].x > maxX ? pVerts[i].x : maxX;
        minY = pVerts[i].y < minY ? pVerts[i].y : minY;
        maxY = pVerts[i].y > maxY ? pVerts[i].y : maxY;
    }
    if (!(flags & FLAG_INTERNAL_BLIT) && (minX > gWindow[2] || maxX < gWindow[0] || minY > gWindow[3] || maxY < gWindow[1]))
    {
        return;
    }
    if (flags & FLAG_FIT_TO_TEXTURE)
    {
        flags |= FLAG_NO_FILTER;
    }

    RTexture* pTex = pTexture ? Resolve(pTexture, (flags & (FLAG_BLEND_ADD | FLAG_BLEND_ALPHA | FLAG_COLOUR_KEY)) ? kAlpha : kOpaque) : 0;
    const f32 texW = pTexture && pTexture->field_E_width ? (f32)pTexture->field_E_width : 1.0f;
    const f32 texH = pTexture && pTexture->field_10_height ? (f32)pTexture->field_10_height : 1.0f;
    const bool image = pTexture && Data(pTexture) && Data(pTexture)->mImage;
    const f32 slotScale = image ? 1.0f : 1.0f / (f32)SlotSize((s32)texW, (s32)texH);
    const f32 scaleU = image ? 1.0f / texW : slotScale;
    const f32 scaleV = image ? 1.0f / texH : slotScale;

    if (flags & FLAG_FIT_TO_TEXTURE)
    {
        // The original's quad: (x0, y0) .. (x0 + w - 0.001, y0 + h - 0.001), UVs 0 .. w - 0.001.
        const f32 x1 = pVerts[0].x + texW - 0.001f;
        const f32 y1 = pVerts[0].y + texH - 0.001f;
        const f32 u1 = texW - 0.001f;
        const f32 v1 = texH - 0.001f;
        pVerts[1].x = x1;
        pVerts[1].y = pVerts[0].y;
        pVerts[2].x = x1;
        pVerts[2].y = y1;
        pVerts[3].x = pVerts[0].x;
        pVerts[3].y = y1;
        pVerts[1].z = pVerts[2].z = pVerts[3].z = pVerts[0].z;
        pVerts[0].u = 0.0f;
        pVerts[0].v = 0.0f;
        pVerts[1].u = u1;
        pVerts[1].v = 0.0f;
        pVerts[2].u = u1;
        pVerts[2].v = v1;
        pVerts[3].u = 0.0f;
        pVerts[3].v = v1;
    }
    for (s32 i = 0; i < count; i++)
    {
        pVerts[i].w = pVerts[i].z;
        pVerts[i].spec = 0;
    }

    const u8 shade = (u8)baseColour;
    if (!(flags & FLAG_VERTEX_DIFFUSE))
    {
        const u32 diffuse = shade | (shade << 8) | (shade << 16) | 0xFF000000u;
        for (s32 i = 0; i < count; i++)
        {
            pVerts[i].diff = diffuse;
        }
    }
    const bool lit = (flags & FLAG_LIGHTING) && gAmbient != 255.0f;
    const bool perPixel = lit && gRenderer->Options().mPerPixelLighting; // enhancement
    u32 unlit[4];
    if (lit)
    {
        for (s32 i = 0; i < count; i++)
        {
            unlit[i] = pVerts[i].diff;
            pVerts[i].diff = LightVertex(pVerts[i + 4], shade, pVerts[i].diff);
        }
        if (perPixel && gLightsDirty)
        {
            UploadLights();
        }
    }

    // As the original: additive is ONE/ONE, the colour key and alpha flags both use alpha blending
    // with the (filtered) texture alpha times the vertex alpha. No alpha test.
    u32 state = 0;
    if (flags & FLAG_BLEND_ADD)
    {
        state |= RS_BLEND_ADD;
    }
    else if (flags & (FLAG_BLEND_ALPHA | FLAG_COLOUR_KEY))
    {
        state |= RS_BLEND_ALPHA;
    }
    if (!(flags & FLAG_NO_FILTER))
    {
        state |= RS_FILTER_LINEAR;
    }
    if (flags & FLAG_INTERNAL_BLIT)
    {
        state |= RS_PIXEL_EXACT;
    }

    LogPrim(pVerts, count, state, pTexture, (flags & FLAG_INTERNAL_BLIT) != 0);

    RVertex rv[4];
    for (s32 i = 0; i < count; i++)
    {
        rv[i].x = pVerts[i].x;
        rv[i].y = pVerts[i].y;
        rv[i].rhw = pVerts[i].z;
        rv[i].rgba = ArgbToRgba(pVerts[i].diff);
        rv[i].u = pVerts[i].u * scaleU;
        rv[i].v = pVerts[i].v * scaleV;
        if (perPixel)
        {
            rv[i].wx = pVerts[i + 4].x;
            rv[i].wy = pVerts[i + 4].y;
            rv[i].wz = pVerts[i + 4].z;
            rv[i].unlit = ArgbToRgba(unlit[i]);
            rv[i].shade = shade / 255.0f;
            rv[i].lit = 1.0f;
        }
        else
        {
            rv[i].wx = rv[i].wy = rv[i].wz = 0.0f;
            rv[i].unlit = 0;
            rv[i].shade = 0.0f;
            rv[i].lit = 0.0f;
        }
    }
    RVertex tris[6];
    s32 n = 3;
    tris[0] = rv[0];
    tris[1] = rv[1];
    tris[2] = rv[2];
    if (count == 4)
    {
        tris[3] = rv[0];
        tris[4] = rv[2];
        tris[5] = rv[3];
        n = 6;
    }
    gRenderer->DrawTriangles(tris, n, pTex, state);
    gGlobals.mNumPolysDrawn += count - 2;
}

void DrawColouredQuad(f32 l, f32 t, f32 r, f32 b, u32 argb)
{
    if (!gRenderer)
    {
        return;
    }
    RVertex v[6];
    const f32 xs[6] = {l, r, r, l, r, l};
    const f32 ys[6] = {t, t, b, t, b, b};
    for (int i = 0; i < 6; i++)
    {
        v[i].x = xs[i];
        v[i].y = ys[i];
        v[i].rhw = 1.0f;
        v[i].rgba = ArgbToRgba(argb);
        v[i].u = v[i].v = 0.0f;
        v[i].wx = v[i].wy = v[i].wz = 0.0f;
        v[i].unlit = 0;
        v[i].shade = 0.0f;
        v[i].lit = 0.0f;
    }
    gRenderer->DrawTriangles(v, 6, 0, 0);
}
} // namespace

GBH_EXPORT void GTA2_CC ConvertColourBank(void*)
{
}

GBH_EXPORT s32 GTA2_CC DrawLine(s32, s32 a2, s32, s32 a4, s32)
{
    return abs(a4 - a2);
}

GBH_EXPORT void GTA2_CC SetShadeTableA(s32, s32, s32, s32, s32)
{
}

GBH_EXPORT s32* GTA2_CC MakeScreenTable(s32 value, s32 elementSize, u32 size)
{
    if (size > sizeof(gScreenTable) / sizeof(gScreenTable[0]))
    {
        size = sizeof(gScreenTable) / sizeof(gScreenTable[0]);
    }
    s32* p = gScreenTable;
    for (u32 i = 0; i < size; i++)
    {
        *p++ = value;
        value += elementSize;
    }
    return p;
}

GBH_EXPORT u32 GTA2_CC gbh_InitDLL(SVideo* pVideo)
{
    InitGlobals();
    InitSqrtTable();
#if defined(__i386__) || defined(_M_IX86)
    // As DMA's d3ddll: x87 single precision, rounding down, for the whole process (game code included).
    unsigned short cw;
    __asm__ __volatile__("fnstcw %0" : "=m"(cw));
    cw = (unsigned short)((cw & ~0x0F00) | 0x0400);
    __asm__ __volatile__("fldcw %0" : : "m"(cw));
#endif
    GLVideoHeader* pHeader = (GLVideoHeader*)pVideo;
    if (!pHeader || pHeader->mMagic != kGLVideoMagic)
    {
        gRenderer = 0;
        return 0;
    }
    gRenderer = pHeader->mRenderer;
    gFlipCounter = pHeader->mFlipCounter;
    gCmd.Init("gl");
    return 1;
}

GBH_EXPORT void GTA2_CC gbh_CloseDLL()
{
}

GBH_EXPORT s32 GTA2_CC gbh_Init(s32)
{
    return 0;
}

GBH_EXPORT void GTA2_CC gbh_CloseScreen(SVideo*)
{
}

GBH_EXPORT void GTA2_CC gbh_BeginLevel()
{
}

GBH_EXPORT void GTA2_CC gbh_EndLevel()
{
}

GBH_EXPORT void GTA2_CC gbh_SetBeginSceneCB(void*)
{
}

GBH_EXPORT void GTA2_CC gbh_BeginScene()
{
    gGlobals.mNumPolysDrawn = 0;
    SyncOptions();
    if (gRenderer)
    {
        gRenderer->BeginScene();
    }
}

GBH_EXPORT void GTA2_CC gbh_EndScene()
{
    if (gRenderer)
    {
        gRenderer->EndScene();
        gGlobals.mNumBatchFlushes = gRenderer->GetDrawCallCount();
    }
}

GBH_EXPORT u32* GTA2_CC gbh_GetGlobals()
{
    return (u32*)&gGlobals;
}

GBH_EXPORT s32 GTA2_CC gbh_GetUsedCache(s32)
{
    return 0;
}

GBH_EXPORT s32 GTA2_CC gbh_SetColourDepth()
{
    return 1;
}

GBH_EXPORT s32 GTA2_CC gbh_SetCamera(f32, f32, f32, f32)
{
    return 0;
}

GBH_EXPORT void GTA2_CC gbh_SetWindow(f32 left, f32 top, f32 right, f32 bottom)
{
    gWindow[0] = left;
    gWindow[1] = top;
    gWindow[2] = right;
    gWindow[3] = bottom;
}

GBH_EXPORT u32 GTA2_CC gbh_ConvertColour(u8 r, u8 g, u8 b)
{
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

GBH_EXPORT u32 GTA2_CC gbh_Convert16BitGraphic(s32, u32, u16*, s32)
{
    return 0;
}

GBH_EXPORT u32 GTA2_CC gbh_RegisterPalette(s32 palId, u32* pData)
{
    if (palId < 0 || palId >= 16384 || !pData)
    {
        return 0;
    }
    // Entries are stored in columns: entry i of a palette is 64 dwords after entry i - 1.
    u32* p = pData;
    for (s32 i = 0; i < 256; i++, p += 64)
    {
        if (i == 0)
        {
            *p = 0;
        }
        else if (*p == 0)
        {
            *p = 0x10000;
        }
    }
    PalEntry* pPal = gPals[palId];
    if (!pPal)
    {
        pPal = new PalEntry();
        gPals[palId] = pPal;
    }
    p = pData;
    for (s32 i = 0; i < 256; i++, p += 64)
    {
        pPal->mRaw[i] = *p & 0xFFFFFF;
        pPal->m565[i] = To565(*p);
        pPal->m565[i + 256] = pPal->m565[i];
    }
    ConvertPalette(pPal);
    return palId;
}

GBH_EXPORT void GTA2_CC gbh_FreePalette(s32 palId)
{
    if (palId >= 0 && palId < 16384)
    {
        delete gPals[palId];
        gPals[palId] = 0;
    }
}

GBH_EXPORT STexture* GTA2_CC gbh_LockTexture(STexture* pTexture)
{
    pTexture->field_8_locked_pixels_ptr = pTexture->field_14_original_pixel_data_ptr;
    pTexture->field_13_flags |= 1;
    pTexture->field_6_pal_size = 256;
    return pTexture;
}

GBH_EXPORT STexture* GTA2_CC gbh_UnlockTexture(STexture* pTexture)
{
    pTexture->field_6_pal_size = 0;
    pTexture->field_8_locked_pixels_ptr = 0;
    pTexture->field_13_flags &= 0xFE;
    TexData* pData = Data(pTexture);
    if (pData)
    {
        pData->mDirty[kOpaque] = pData->mDirty[kAlpha] = true;
    }
    return pTexture;
}

GBH_EXPORT void GTA2_CC gbh_AssignPalette(STexture* pTexture, s32 palId)
{
    const bool needUnlock = !(pTexture->field_13_flags & 1);
    if (needUnlock)
    {
        gbh_LockTexture(pTexture);
    }
    PalEntry* pPal = (palId >= 0 && palId < 16384) ? gPals[palId] : 0;
    pTexture->field_18_pPaltData = pPal ? pPal->m565 : 0;
    pTexture->field_12_bPalIsValid = pPal ? 1 : 0;
    if (needUnlock)
    {
        gbh_UnlockTexture(pTexture);
    }
}

GBH_EXPORT STexture* GTA2_CC gbh_RegisterTexture(s32 width, s32 height, u8* pData, s32 palId, char)
{
    STexture* pTexture = NewTexture(width, height, pData);
    if (pTexture)
    {
        PalEntry* pPal = (palId >= 0 && palId < 16384) ? gPals[palId] : 0;
        pTexture->field_18_pPaltData = pPal ? pPal->m565 : 0;
        pTexture->field_12_bPalIsValid = pPal ? 1 : 0;
    }
    return pTexture;
}

GBH_EXPORT void GTA2_CC gbh_FreeTexture(STexture* pTexture)
{
    if (!pTexture)
    {
        return;
    }
    TexData* pData = Data(pTexture);
    if (pData)
    {
        for (int v = 0; v < 2; v++)
        {
            if (gRenderer && pData->mTex[v])
            {
                gRenderer->FreeTexture(pData->mTex[v]);
            }
        }
        delete pData;
    }
    free(pTexture);
}

GBH_EXPORT void GTA2_CC gbh_ResetLights()
{
    gNumLights = 0;
    gLightsDirty = true;
}

GBH_EXPORT s32 GTA2_CC gbh_AddLight(SLight* pLight)
{
    if (gNumLights >= 256)
    {
        return 0;
    }
    const u32 idx = gNumLights++;
    gLightsDirty = true;
    LightInternal& l = gLights[idx];
    const f32 k = 1.0f / 255.0f;
    l.mFlags = pLight->field_0;
    l.mBrightness = (pLight->field_0 & 0xFF) * k;
    l.mX = pLight->field_4_x;
    l.mY = pLight->field_8_y;
    l.mZ = pLight->field_C_z;
    l.mR = ((pLight->field_10_colour >> 16) & 0xFF) * k;
    l.mG = ((pLight->field_10_colour >> 8) & 0xFF) * k;
    l.mB = (pLight->field_10_colour & 0xFF) * k;
    l.mRadius = ((pLight->field_0 >> 8) & 0xFF) * k * 8.0f;
    l.mRadiusSquared = l.mRadius * l.mRadius;
    l.mInvRadius = l.mRadius > 0.0f ? 1.0f / l.mRadius : 0.0f;
    return idx * 0x2C;
}

GBH_EXPORT void GTA2_CC gbh_SetAmbient(f32 ambient)
{
    gAmbient = ambient * 255.0f;
    gLightsDirty = true;
}

GBH_EXPORT void GTA2_CC gbh_DrawQuad(s32 flags, STexture* pTexture, Vert* pVerts, s32 baseColour)
{
    LogCall("DrawQuad", flags, pTexture, baseColour);
    RenderVerts((u32)flags, pTexture, pVerts, 4, (u32)baseColour & 0xFF);
}

GBH_EXPORT void GTA2_CC gbh_DrawTriangle(s32 flags, STexture* pTexture, Vert* pVerts, s32 diffuseColour)
{
    LogCall("DrawTriangle", flags, pTexture, diffuseColour);
    RenderVerts((u32)flags, pTexture, pVerts, 3, (u32)diffuseColour & 0xFF);
}

static s32 DrawTileImpl(u32 flags, STexture* pTexture, Vert* pData, s32 diffuseColour)
{
    u32 drawFlags = flags;
    if (!(flags & FLAG_TILE_UV))
    {
        // Half texel inset against bleeding at the tile edges.
        pData[0].u = 0.5f;
        pData[0].v = 0.5f;
        pData[1].u = 63.499901f;
        pData[1].v = 0.5f;
        pData[2].u = 63.499901f;
        pData[2].v = 63.499901f;
        pData[3].u = 0.5f;
        pData[3].v = 63.499901f;
    }

    f32 uvs[4][2];
    for (int i = 0; i < 4; i++)
    {
        uvs[i][0] = pData[i].u;
        uvs[i][1] = pData[i].v;
    }

    // Rotation in 90 degree steps (0x40 is a 180 degree turn, done as two flips).
    static const int kRot90[4] = {3, 0, 1, 2};
    static const int kRot270[4] = {1, 2, 3, 0};
    const int* pRot = 0;
    switch (flags & 0x60)
    {
        case 0x20:
            pRot = kRot90;
            break;
        case 0x40:
            drawFlags = flags ^ 0x18;
            break;
        case 0x60:
            pRot = kRot270;
            break;
    }
    if (pRot)
    {
        for (int i = 0; i < 4; i++)
        {
            pData[i].u = uvs[pRot[i]][0];
            pData[i].v = uvs[pRot[i]][1];
        }
        for (int i = 0; i < 4; i++)
        {
            uvs[i][0] = pData[i].u;
            uvs[i][1] = pData[i].v;
        }
    }

    if (drawFlags & 8)
    {
        static const int kFlip[4] = {1, 0, 3, 2};
        for (int i = 0; i < 4; i++)
        {
            pData[i].u = uvs[kFlip[i]][0];
            pData[i].v = uvs[kFlip[i]][1];
        }
        if (drawFlags & 0x10)
        {
            for (int i = 0; i < 4; i++)
            {
                uvs[i][0] = pData[i].u;
                uvs[i][1] = pData[i].v;
            }
        }
    }
    if (drawFlags & 0x10)
    {
        static const int kFlip[4] = {3, 2, 1, 0};
        for (int i = 0; i < 4; i++)
        {
            pData[i].u = uvs[kFlip[i]][0];
            pData[i].v = uvs[kFlip[i]][1];
        }
    }

    RenderVerts(drawFlags, pTexture, pData, 4, (u32)diffuseColour & 0xFF);
    return 0;
}

GBH_EXPORT s32 GTA2_CC gbh_DrawTilePart(u32 flags, STexture* pTexture, Vert* pData, s32 diffuseColour)
{
    LogCall("DrawTilePart", flags, pTexture, diffuseColour);
    return DrawTileImpl(flags, pTexture, pData, diffuseColour);
}

GBH_EXPORT void GTA2_CC gbh_DrawQuadClipped(s32, s32, s32, s32, s32)
{
}

GBH_EXPORT s32 GTA2_CC gbh_DrawFlatRect(Vert* pVerts, s32 colour)
{
    LogCall("DrawFlatRect", 0, 0, colour);
    if (!pVerts)
    {
        return 0;
    }
    // As the original: untextured, no blending, colour as given.
    DrawColouredQuad(pVerts[0].x, pVerts[0].y, pVerts[2].x, pVerts[2].y, (u32)colour);
    return 0;
}

// A no-op in DMA's d3ddll.dll as well.
GBH_EXPORT void GTA2_CC gbh_Plot(f32, f32, s32, s32)
{
}

GBH_EXPORT s32 GTA2_CC gbh_PrintBitmap(s32, s32)
{
    return 0;
}

GBH_EXPORT s32 GTA2_CC gbh_BlitBuffer(s32, s32, s32, s32, s32, s32)
{
    return 0;
}

GBH_EXPORT s32 GTA2_CC gbh_InitImageTable(s32 tableSize)
{
    if (tableSize < 0)
    {
        return -1;
    }
    gImages.assign(tableSize, ImageEntry());
    for (size_t i = 0; i < gImages.size(); i++)
    {
        memset(&gImages[i], 0, sizeof(ImageEntry));
    }
    return 0;
}

GBH_EXPORT s32 GTA2_CC gbh_FreeImageTable()
{
    for (size_t i = 0; i < gImages.size(); i++)
    {
        if (gImages[i].mLoaded)
        {
            gbh_FreeTexture(gImages[i].mTexture);
        }
    }
    gImages.clear();
    return 0;
}

// The buffer is a whole TGA file (type 2, 16 bpp X1R5G5B5); the game frees it right after the call.
GBH_EXPORT s32 GTA2_CC gbh_LoadImage(SImage* pImage)
{
    s32 slot = -1;
    for (size_t i = 0; i < gImages.size(); i++)
    {
        if (!gImages[i].mLoaded)
        {
            slot = (s32)i;
            break;
        }
    }
    if (slot < 0)
    {
        return -1;
    }
    if (!pImage || pImage->field_1_cmap_type || pImage->field_2_image_type != 2 || pImage->field_10_bpp != 16)
    {
        return -2;
    }
    const s32 w = pImage->field_C_width;
    const s32 h = pImage->field_E_height;
    STexture* pTexture = NewTexture(w, h, 0);
    if (!pTexture)
    {
        return -3;
    }
    TexData* pData = Data(pTexture);
    pData->mImage = true;
    pData->mPixels.resize((size_t)w * h);
    const u16* src = (const u16*)((const u8*)pImage + offsetof(SImage, field_12) + pImage->field_0_id_len);
    const bool topDown = (pImage->field_11_descriptor & 0x20) != 0;
    for (s32 y = 0; y < h; y++)
    {
        const u16* row = src + (size_t)(topDown ? y : (h - 1 - y)) * w;
        u32* dst = &pData->mPixels[(size_t)y * w];
        for (s32 x = 0; x < w; x++)
        {
            const u32 p = row[x];
            // Green goes 5 -> 6 bits by a shift, as the original's 555 to 565 conversion.
            const u32 r = (p >> 10) & 31, g = ((p >> 5) & 31) << 1, b = p & 31;
            dst[x] = ((r << 3) | (r >> 2)) | (((g << 2) | (g >> 4)) << 8) | (((b << 3) | (b >> 2)) << 16) | 0xFF000000u;
        }
    }
    gImages[slot].mLoaded = 1;
    gImages[slot].mWidth = w;
    gImages[slot].mHeight = h;
    gImages[slot].mTexture = pTexture;
    return slot;
}

GBH_EXPORT s32 GTA2_CC gbh_BlitImage(s32 imageIndex, s32 srcLeft, s32 srcTop, s32 srcRight, s32 srcBottom, s32 dstX, s32 dstY)
{
    LogCall("BlitImage", imageIndex, 0, 0);
    if (imageIndex < 0 || imageIndex >= (s32)gImages.size() || !gImages[imageIndex].mLoaded)
    {
        return -1;
    }
    const ImageEntry& img = gImages[imageIndex];
    if (srcLeft < 0 || srcTop < 0 || srcRight <= srcLeft || srcBottom <= srcTop || srcRight > (s32)img.mWidth ||
        srcBottom > (s32)img.mHeight)
    {
        return -2;
    }
    Vert verts[4];
    memset(verts, 0, sizeof(verts));
    const f32 w = (f32)(srcRight - srcLeft);
    const f32 h = (f32)(srcBottom - srcTop);
    const f32 xs[4] = {0, w, w, 0};
    const f32 ys[4] = {0, 0, h, h};
    for (int i = 0; i < 4; i++)
    {
        verts[i].x = dstX + xs[i];
        verts[i].y = dstY + ys[i];
        verts[i].z = 1.0f;
        verts[i].w = 1.0f;
        verts[i].u = srcLeft + xs[i];
        verts[i].v = srcTop + ys[i];
    }
    // The original does a DirectDraw Blt here: an exact copy.
    RenderVerts(FLAG_NO_FILTER | FLAG_INTERNAL_BLIT, img.mTexture, verts, 4, 255);
    return 0;
}

GBH_EXPORT s32 GTA2_CC gbh_DrawTile(u32 flags, STexture* pTexture, Vert* pData, s32 diffuseColour)
{
    LogCall("DrawTile", flags, pTexture, diffuseColour);
    return DrawTileImpl(flags, pTexture, pData, diffuseColour);
}
