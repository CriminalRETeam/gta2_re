#pragma once

// 0xRRGGBB colour as used by lights (Light::field_10_argb has no alpha byte set)
#define MAKE_RGB(r, g, b) ((((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF))
