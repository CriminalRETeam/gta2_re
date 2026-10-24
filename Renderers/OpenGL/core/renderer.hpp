// Backend interface between the gbh_* layer (render DLL) and the OpenGL core (video DLL).
// Both sides are built by the same compiler, so a plain virtual interface is safe across the DLLs.
#pragma once

#include "gta2_api.hpp"

struct RVertex
{
    f32 x, y; // logical screen pixels
    f32 rhw; // 1/w, used for perspective correct interpolation
    u32 rgba; // bytes in memory: r, g, b, a
    f32 u, v; // normalised texture coordinates
    // Per pixel lighting only (lit != 0): world position, colour before lighting, shade 0..1.
    f32 wx, wy, wz;
    u32 unlit;
    f32 shade;
    f32 lit;
};

// Optional enhancements; all off is the faithful renderer.
struct RenderOptions
{
    bool mTrueColour = false; // 8-bit palette colours and an RGBA8 target
    f32 mBloom = 0.0f; // 0..1, glow from additive draws
    bool mPerPixelLighting = false; // gbh_AddLight lights evaluated per fragment
    u32 mGeneration = 0; // bumped on every change
};

// A light as the original's LightVertex uses it (already filtered to active lights).
struct RLight
{
    f32 x, y, z;
    f32 radius;
    f32 invRadius;
    f32 r, g, b; // colour times brightness
};

enum RenderStateBits
{
    RS_BLEND_ALPHA = 1 << 1, // src_alpha, 1 - src_alpha
    RS_BLEND_ADD = 1 << 2, // additive
    RS_FILTER_LINEAR = 1 << 3,
    RS_NO_TEXTURE = 1 << 4,
    RS_PIXEL_EXACT = 1 << 5, // no D3D pixel centre offset: integer quads copy texels 1:1 (DirectDraw blits)
};

struct RTexture;

class IRenderer
{
  public:
    virtual ~IRenderer()
    {
    }

    // Texture contents are given as RGBA8, top row first. The backend may defer GL work.
    virtual RTexture* CreateTexture() = 0;
    virtual void UploadTexture(RTexture* pTex, s32 width, s32 height, const u32* pRgba) = 0;
    virtual void FreeTexture(RTexture* pTex) = 0;

    virtual void BeginScene() = 0;
    virtual void EndScene() = 0;

    // Triangles (count multiple of 3).
    virtual void DrawTriangles(const RVertex* pVerts, s32 count, RTexture* pTex, u32 state) = 0;

    virtual const RenderOptions& Options() const = 0;
    // Lights for per pixel lighting; ambient in 0..255 units as gbh_SetAmbient stores it.
    virtual void SetLights(const RLight* pLights, s32 count, f32 ambient) = 0;

    // Stats exposed through gbh_GetGlobals.
    virtual u32 GetDrawCallCount() const = 0;
};

// Lets the render DLL find the backend from the SVideo pointer the game hands to gbh_InitDLL.
static const u32 kGLVideoMagic = 0x4C47324Cu; // "L2GL"

struct GLVideoHeader
{
    SVideo mVideo; // must stay first: the game only sees an SVideo*
    u32 mMagic;
    IRenderer* mRenderer;
    const u32* mFlipCounter; // number of Vid_FlipBuffers calls so far
};
