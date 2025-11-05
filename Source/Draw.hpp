#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "gbh_graphics.hpp"
#include "fix16.hpp"

struct DrawKind
{
    DrawKind()
    {
    }

    DrawKind(s32 v_arg)
    {
        value = v_arg;
    }
    s32 value;
};

struct QuadVerts // TODO: Same as Verts in gbh header (d3ddll)
{
    Vert field_0_verts[4];
};

EXPORT void __stdcall DrawText_4B87A0(const wchar_t* pBuffer, Fix16 xpos_fp, Fix16 ypos_fp, s16 fontType, Fix16 scale);

EXPORT void __stdcall sub_5D7670(s32 type, s16 pal, Fix16 x_pos, Fix16 y_pos, Ang16 rotation, DrawKind& drawkind, s16 a8, s32 a9, u8 a10);

EXPORT s32 __stdcall sub_5D7700(u16* a1);

EXPORT void __stdcall DrawText_5D7720(const wchar_t* pStr, Fix16 xoff, Fix16 yoff, u16 fontType, DrawKind& a5, u16 a6, s32 alpha, u8 alpha_flag);

EXPORT void __stdcall sub_5D77A0(wchar_t* pText, Fix16 xpos, Fix16 ypos, u16 font_type);

EXPORT void __stdcall sub_5D7CB0();

EXPORT void __stdcall sub_5D7D30();

EXPORT void __stdcall sub_5D7EC0(s32 type,
                                 s16 pal,
                                 Fix16 x_pos,
                                 Fix16 y_pos,
                                 Ang16 rotation,
                                 Fix16 scale,
                                 DrawKind& drawkind,
                                 s16 a8,
                                 s32 a9,
                                 u8 a10,
                                 char_type a11);

EXPORT s32 __stdcall CalcQuadFlags_5D83E0(s32 mode, u8 a2);

EXPORT void __stdcall DrawText_5D8A10(const wchar_t* pText,
                                      Fix16 xpos_fp,
                                      Fix16 ypos_fp,
                                      u16 font_type,
                                      Fix16 scale_fp,
                                      DrawKind& drawkind,
                                      u16 unknown1,
                                      s32 unknown2,
                                      u8 flags);

extern u16 word_703BAA; //EXTERN_GLOBAL(u16, word_703BAA);
