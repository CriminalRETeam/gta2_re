// Reference command log for capvideo.dll (see d3dlog.cpp). No-op without GTA2GL_CMDLOG.
#pragma once

#include "../core/gta2_api.hpp"

void InstallD3DLog(const u32* pFlipCounter);
void D3DLogFlip();
