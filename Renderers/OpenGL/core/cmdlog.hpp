// Draw command log shared by the reference proxy (D3D calls of DMA's d3ddll) and glrender (pre-GL),
// so both streams can be diffed draw by draw. GTA2GL_CMDLOG=<dir>,<frame>,<a>-<b>,...
// Frame N holds the calls made before the Nth Vid_FlipBuffers.
#pragma once

#include "capture.hpp"
#include <stdio.h>

class CmdLog
{
  public:
    void Init(const char* prefix);
    // Called with the number of flips done so far; opens/closes the per frame file.
    void Update(u32 flipsDone);
    bool Active() const
    {
        return mFile != 0;
    }
    bool Enabled() const
    {
        return mFrames.Enabled();
    }

    // A gbh_* call made by the game.
    void Call(const char* func, u32 flags, u32 texId, s32 texW, s32 texH, u32 colour);
    // A primitive as given to the rasteriser (D3D space: pixel centres on integers, texel units).
    // state: blend enable, src, dst, mag filter, min filter (D3D render state values).
    void Prim(const char* type, s32 count, const u32 state[5], u32 texKey, s32 texW, s32 texH);
    void Vertex(f32 x, f32 y, f32 z, f32 rhw, u32 diffuse, u32 specular, f32 u, f32 v);
    void Note(const char* text);

  private:
    FrameCapture mFrames;
    FILE* mFile = 0;
    u32 mOpenFrame = 0;
    char mPrefix[16];
};
