#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "gbh_graphics.hpp"
#include "fix16.hpp"
#include "gtx_0x106C.hpp"

struct QuadVerts // same layout as Verts in GTA2Hax d3ddll.hpp
{
    Vert field_0_verts[4];
};

EXPORT void __stdcall DrawText_4B87A0(const wchar_t* pBuffer, Fix16 xpos_fp, Fix16 ypos_fp, s16 fontType, Fix16 scale);

EXPORT void __stdcall DrawFigureScaled_5D7670(s32 sprite_type, s16 sprite_idx, Fix16 x_pos, Fix16 y_pos, Ang16 rotation, const s32& palette_type, s16 palette, s32 alpha_value, u8 flags);

EXPORT s32 __stdcall GetLineSpacingFromFontType_5D7700(u16 font_type);

EXPORT void __stdcall DrawText_5D7720(const wchar_t* pStr, Fix16 xoff, Fix16 yoff, u16 fontType, const s32& palette_type, u16 palette, s32 alpha, u8 alpha_flag);

EXPORT void __stdcall DrawTextScaled_5D77A0(const wchar_t* pText, Fix16 xpos, Fix16 ypos, u16 font_type);

EXPORT void __stdcall ConvertColourBanks_5D7CB0();
EXPORT void __stdcall ConvertColourBanks_5D7CC0();

EXPORT void __stdcall MakeScreenTableAndSetWindow_5D7D30();

EXPORT void __stdcall DrawFigure_5D7EC0(s32 sprite_type,
                                 s16 sprite_idx,
                                 Fix16 x_pos,
                                 Fix16 y_pos,
                                 Ang16 rotation,
                                 Fix16 scale,
                                 const s32& palette_type,
                                 s16 palette,
                                 s32 alpha_value,
                                 u8 flags,
                                 bool bNoTextureFiltering);

EXPORT s32 __stdcall CalcQuadFlags_5D83E0(s32 mode, u8 alpha);

EXPORT void __stdcall DrawText_5D8A10(const wchar_t* pText,
                                      Fix16 xpos_fp,
                                      Fix16 ypos_fp,
                                      u16 font_type,
                                      Fix16 scale_fp,
                                      const s32& og_palette_type,
                                      u16 og_palette,
                                      s32 alpha_value,
                                      u8 flags);

EXPORT void __stdcall DrawTexture_5D8470(STexture* pTexture,
                                 Fix16 x_pos,
                                 Fix16 y_pos,
                                 u8 width,
                                 u8 height,
                                 Ang16 rotation,
                                 Fix16 scale,
                                 s32 alpha_value,
                                 u8 alpha_flag);

EXPORT void __stdcall DrawTextureScaled_495470(STexture* pTexture, Fix16 x_pos, Fix16 y_pos, u8 width, u8 height, Ang16 rotation, s32 alpha_value, u8 alpha_flag);

// 9.6f 0x4539B0, the inlined copy of GetLineSpacingFromFontType_5D7700
inline s32 __stdcall GetLineSpacingFromFontType_5D7700_inlined(u16 font_type)
{
    return static_cast<u16>(gGtx_0x106C_703DD4->GetLineSpacing_5AA800(&font_type));
}

// 9.6f 0x4539D0
inline s32 __stdcall GetCharWidth_4539D0(u16 font_type, wchar_t chr)
{
    return static_cast<u16>(gGtx_0x106C_703DD4->GetFontWidth_5AA760(&font_type, &chr));
}

EXPORT s32 __stdcall CountLineSpacing_5D8940(const wchar_t* pStr, u16 font_type);
