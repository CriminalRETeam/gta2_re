#include "Draw.hpp"
#include "DrawUnk_0xBC.hpp"
#include "dma_video.hpp"
#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "magical_germain_0x8EC.hpp"
#include "sharp_pare_0x15D8.hpp"

DEFINE_GLOBAL_INIT(Fix16, dword_706A6C, Fix16(1), 0x706A6C);
DEFINE_GLOBAL_INIT(Ang16, word_706C3C, Ang16(0), 0x706C3C);
DEFINE_GLOBAL(QuadVerts, gQuadVerts_706B88, 0x706B88);
EXTERN_GLOBAL(u32, gLightingDrawFlag_7068F4);

EXTERN_GLOBAL(s32, window_width_706630);
EXTERN_GLOBAL(s32, window_height_706B50);

DEFINE_GLOBAL(DWORD, dword_70675C, 0x70675C);
DEFINE_GLOBAL(DWORD, dword_70679C, 0x70679C);

MATCH_FUNC(0x4B87A0)
void __stdcall DrawText_4B87A0(const wchar_t* pBuffer, Fix16 xpos_fp, Fix16 ypos_fp, s16 fontType, Fix16 scale)
{
    DrawText_5D8A10(pBuffer, xpos_fp, ypos_fp, fontType, scale, DrawKind(2), 0, 0, 0);
}

MATCH_FUNC(0x5D7670)
void __stdcall sub_5D7670(s32 type, s16 pal, Fix16 x_pos, Fix16 y_pos, Ang16 rotation, DrawKind& drawkind, s16 a8, s32 a9, u8 a10)
{
    sub_5D7EC0(type,
               pal,
               x_pos * gViewCamera_676978->field_A8_ui_scale,
               y_pos * gViewCamera_676978->field_A8_ui_scale,
               rotation,
               gViewCamera_676978->field_A8_ui_scale,
               drawkind,
               a8,
               a9,
               a10,
               0);
}

// https://decomp.me/scratch/WTcl7
STUB_FUNC(0x5D7700)
s32 __stdcall sub_5D7700(u16* a1)
{
    NOT_IMPLEMENTED;
    return (u16)gGtx_0x106C_703DD4->sub_5AA800(a1);
}

MATCH_FUNC(0x5D7720)
void __stdcall DrawText_5D7720(const wchar_t* pStr, Fix16 xoff, Fix16 yoff, u16 fontType, DrawKind& a5, s32 a6, s32 a7, s32 a8)
{
    DrawText_5D8A10(pStr,
                    xoff * gViewCamera_676978->field_A8_ui_scale,
                    yoff * gViewCamera_676978->field_A8_ui_scale,
                    fontType,
                    gViewCamera_676978->field_A8_ui_scale,
                    a5,
                    a6,
                    a7,
                    a8);
}

MATCH_FUNC(0x5D77A0)
void __stdcall sub_5D77A0(wchar_t* pText, Fix16 xpos, Fix16 ypos, u16 font_type)
{
    DrawText_5D8A10(pText,
                    xpos * gViewCamera_676978->field_A8_ui_scale,
                    ypos * gViewCamera_676978->field_A8_ui_scale,
                    font_type,
                    gViewCamera_676978->field_A8_ui_scale,
                    DrawKind(2),
                    0,
                    0,
                    0);
}

STUB_FUNC(0x5D7CB0)
void __stdcall sub_5D7CB0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5D7D30)
void __stdcall sub_5D7D30()
{
    NOT_IMPLEMENTED;
    pVid_GetSurface(gVidSys_7071D0);
    pMakeScreenTable((int)gVidSys_7071D0->field_50_surface_pixels_ptr,
                     gVidSys_7071D0->field_54_surface_pixels_pitch,
                     gVidSys_7071D0->field_4C_rect_bottom);

    if (gVidSys_7071D0->field_40_full_screen == -2)
    {
        dword_70679C = window_height_706B50 - 1;
        dword_70675C = window_width_706630 - 1;
    }
    else
    {
        dword_70675C = gVidSys_7071D0->field_48_rect_right - 1;
        dword_70679C = gVidSys_7071D0->field_4C_rect_bottom - 1;
    }

    pgbh_SetWindow(0, 0, dword_70675C, dword_70679C);
}

// https://decomp.me/scratch/Zmms7
STUB_FUNC(0x5D7EC0)
void __stdcall sub_5D7EC0(s32 type,
                          s16 pal,
                          Fix16 x_pos,
                          Fix16 y_pos,
                          Ang16 rotation,
                          Fix16 scale,
                          DrawKind& drawkind,
                          s16 a8,
                          s32 a9,
                          u8 a10,
                          char_type a11)
{

    u16 v11 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(type, pal);
    sprite_index* sprite_index_5AA440 = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v11);

    Fix16 v12 = (Fix16(sprite_index_5AA440->field_4_width) / 2) * scale;
    Fix16 v13 = (Fix16(sprite_index_5AA440->field_5_height) / 2) * scale;

    s32 flags;

    if (scale != dword_706A6C || (flags = 0x10000, rotation != word_706C3C))
    {
        flags = 0;
    }
    if (!a11)
    {
        flags |= 0x20000u;
    }

    Fix16_Point point(-v12, -v13);
    point.RotateByAngle_40F6B0(rotation);
    point.x += x_pos;
    point.y += y_pos;

    gQuadVerts_706B88.field_0_verts[0].x = point.x.ToFloat();
    gQuadVerts_706B88.field_0_verts[0].y = point.y.ToFloat();
    gQuadVerts_706B88.field_0_verts[0].z = 0.000099999997;

    Fix16_Point point2(v12, -v13);
    point2.RotateByAngle_40F6B0(rotation);
    point2.x += x_pos;
    point2.y += y_pos;

    gQuadVerts_706B88.field_0_verts[1].x = point2.x.ToFloat();
    gQuadVerts_706B88.field_0_verts[1].y = point2.y.ToFloat();
    gQuadVerts_706B88.field_0_verts[1].z = 0.000099999997;

    Fix16_Point point3(v12, v13);
    point3.RotateByAngle_40F6B0(rotation);
    point3.x += x_pos;
    point3.y += y_pos;

    gQuadVerts_706B88.field_0_verts[2].x = point3.x.ToFloat();
    gQuadVerts_706B88.field_0_verts[2].y = point3.y.ToFloat();
    gQuadVerts_706B88.field_0_verts[2].z = 0.000099999997;

    Fix16_Point point4(-v12, v13);
    point4.RotateByAngle_40F6B0(rotation);
    point4.x += x_pos;
    point4.y += y_pos;

    gQuadVerts_706B88.field_0_verts[3].x = point4.x.ToFloat();
    gQuadVerts_706B88.field_0_verts[3].y = point4.y.ToFloat();
    gQuadVerts_706B88.field_0_verts[3].z = 0.000099999997;

    //  u & v

    u32 field_5_height = sprite_index_5AA440->field_5_height;
    u32 field_4_width = sprite_index_5AA440->field_4_width;

    gQuadVerts_706B88.field_0_verts[0].u = 0.0;
    gQuadVerts_706B88.field_0_verts[0].v = 0.0;
    gQuadVerts_706B88.field_0_verts[1].v = 0.0;
    gQuadVerts_706B88.field_0_verts[3].u = 0.0;

    gQuadVerts_706B88.field_0_verts[1].u = field_4_width - 0.000099999997;
    gQuadVerts_706B88.field_0_verts[2].u = field_4_width - 0.000099999997;
    gQuadVerts_706B88.field_0_verts[2].v = field_5_height - 0.000099999997;
    gQuadVerts_706B88.field_0_verts[3].v = field_5_height - 0.000099999997;

    STexture* pTexture = gSharp_pare_0x15D8_705064->sub_5B94F0(type, pal, drawkind.value, a8);
    s32 v44 = CalcQuadFlags_5D83E0(a9, a10);
    pgbh_DrawQuad(flags | v44, pTexture, &gQuadVerts_706B88.field_0_verts[0], 255);
}

MATCH_FUNC(0x5D83E0);
s32 __stdcall CalcQuadFlags_5D83E0(s32 mode, u8 a2)
{
    switch (mode)
    {
        case 0:
            return gLightingDrawFlag_7068F4 | 0x80;
        case 1:
            gQuadVerts_706B88.field_0_verts[0].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[1].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[2].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[3].diff = (a2 << 27) | 0xFFFFFF;
            return gLightingDrawFlag_7068F4 | 0x2180;
        case 2:
            gQuadVerts_706B88.field_0_verts[0].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[1].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[2].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[3].diff = (a2 << 27) | 0xFFFFFF;
            return gLightingDrawFlag_7068F4 | 0x2280;
        default:
            return 0;
    }
}

STUB_FUNC(0x5D8A10)
void __stdcall DrawText_5D8A10(const wchar_t* pText,
                               Fix16 xpos_fp,
                               Fix16 ypos_fp,
                               u16 font_type,
                               Fix16 scale_fp,
                               DrawKind& drawkind,
                               s32 unknown1, // seems to be related with palette
                               s32 unknown2, // alpha_value
                               s32 flags) // bool use_alpha
{
    NOT_IMPLEMENTED;

    s32 new_Flags = CalcQuadFlags_5D83E0(unknown2, flags) | 0x20000;

    Fix16 cur_xpos = xpos_fp; // note: new var

    Fix16 spaceWidth = scale_fp * gGtx_0x106C_703DD4->space_width_5AA7B0(&font_type);
    Fix16 lineHeight = scale_fp * gGtx_0x106C_703DD4->sub_5AA800(&font_type);

    unknown2 = unknown1;

    u32 kind = drawkind.value;
    if (scale_fp == dword_706A6C)
    {
        new_Flags = new_Flags | 0x10000;
    }

    if (font_type >= 101u)
    {
        if (kind == 8)
        {
            gMagical_germain_0x8EC_6F5168->sub_4D29D0(unknown1);
        }
        else
        {
            gMagical_germain_0x8EC_6F5168->sub_4D28A0(font_type);
        }
    }

    while (*pText != 0)
    {
        wchar_t text_char = *pText;

        // HIWORD(v16) =
        //HIWORD(v16) = HIWORD(pTextIter);

        // = LOWORD(v16)

        if (text_char == L'\n')
        {
            // reset xpos back to the start
            cur_xpos = xpos_fp;

            // move to the next line down
            ypos_fp += lineHeight;
        }
        else if (text_char == L' ')
        {
            // advance by size of space char
            cur_xpos += spaceWidth;
        }
        else if (text_char == L'#')
        {
            if (kind == drawkind.value && (WORD)unknown2 == (WORD)unknown1)
            {
                kind = 8;
                /*
                v17 = -((u16)font_type < 0x65u);
                // LOBYTE(v17) =
                v17 = v17 & 0xFB;
                v18 = v17 + 5;
                */
                unknown2 = font_type < 0x65u ? 10 : 5;
            }
            else
            {
                kind = drawkind.value;
                unknown2 = unknown1;
            }

            if ((u16)font_type >= 101u)
            {
                if (kind == 8)
                {
                    gMagical_germain_0x8EC_6F5168->sub_4D29D0(unknown2);
                }
                else
                {
                    gMagical_germain_0x8EC_6F5168->sub_4D28A0(font_type);
                }
            }
        }
        else
        {
            sprite_index* pSprIdx;
            STexture* pTextureToUse;
            if ((u16)font_type < 0x65u || (u16)font_type > 107u)
            {
                if ((u16)font_type < 0xC9u || (u16)font_type > 203u)
                {
                    // LOWORD(sprite_pal) =
                    u16 sprite_pal = gGtx_0x106C_703DD4->sub_5AA710(font_type, text_char - 33);
                    u16 sprt_idx = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(7, sprite_pal);
                    pSprIdx = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprt_idx);
                    pTextureToUse = gSharp_pare_0x15D8_705064->sub_5B94F0(7, sprite_pal, kind, unknown2);
                }
                else
                {
                    pSprIdx = gMagical_germain_0x8EC_6F5168->field_8E0_sprite_index;
                    pTextureToUse = gMagical_germain_0x8EC_6F5168->sub_4D27D0(text_char);
                }
            }
            else
            {
                pSprIdx = gMagical_germain_0x8EC_6F5168->field_8D4_sprite_index;
                pTextureToUse = gMagical_germain_0x8EC_6F5168->sub_4D2710(text_char);
            }

            STexture* pTexture = pTextureToUse;

            //v25 = v11.mValue;
            //v26 = v11.mValue >> 31;

            Fix16 sprite_xoff = Fix16(pSprIdx->field_4_width) * scale_fp;

            Fix16 sprite_yoff = Fix16(pSprIdx->field_5_height) * scale_fp;

            gQuadVerts_706B88.field_0_verts[0].x = cur_xpos.ToFloat();
            gQuadVerts_706B88.field_0_verts[0].y = ypos_fp.ToFloat();
            gQuadVerts_706B88.field_0_verts[0].z = 0.0001; // line 214

            f32 v_1_2_x = (sprite_xoff + cur_xpos).ToFloat();
            gQuadVerts_706B88.field_0_verts[1].x = v_1_2_x;
            gQuadVerts_706B88.field_0_verts[1].y = gQuadVerts_706B88.field_0_verts[0].y;
            gQuadVerts_706B88.field_0_verts[1].z = 0.0001;

            // f32 v1_u = (((f64)pSprIdx->field_4_width - 0.000099999997) * 16384.0);

            gQuadVerts_706B88.field_0_verts[2].x = v_1_2_x;
            gQuadVerts_706B88.field_0_verts[2].y = (ypos_fp + sprite_yoff).ToFloat();

            s32 v28 = sprite_xoff.mValue + cur_xpos.mValue;
            gQuadVerts_706B88.field_0_verts[2].z = 0.0001;

            gQuadVerts_706B88.field_0_verts[3].x = gQuadVerts_706B88.field_0_verts[0].x;
            gQuadVerts_706B88.field_0_verts[3].y = (ypos_fp + sprite_yoff).ToFloat();
            gQuadVerts_706B88.field_0_verts[3].z = 0.0001;

            Fix16 letterW((float)(pSprIdx->field_4_width - 0.0001));
            cur_xpos += letterW;
            Fix16 spriteH((float)(pSprIdx->field_5_height - 0.0001));

            gQuadVerts_706B88.field_0_verts[0].u = 0.0;
            gQuadVerts_706B88.field_0_verts[0].v = 0.0;

            gQuadVerts_706B88.field_0_verts[1].u = spriteH.ToFloat();
            gQuadVerts_706B88.field_0_verts[1].v = 0.0;

            gQuadVerts_706B88.field_0_verts[2].u = spriteH.ToFloat();
            gQuadVerts_706B88.field_0_verts[2].v = letterW.ToFloat();

            gQuadVerts_706B88.field_0_verts[3].u = 0.0;
            gQuadVerts_706B88.field_0_verts[3].v = letterW.ToFloat();

            pgbh_DrawQuad(new_Flags, pTexture, &gQuadVerts_706B88.field_0_verts[0], 255);
        }
        pText++;
    }
}
