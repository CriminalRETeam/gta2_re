#include "Draw.hpp"
#include "Camera.hpp"
#include "dma_video.hpp"
#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "magical_germain_0x8EC.hpp"
#include "sharp_pare_0x15D8.hpp"

DEFINE_GLOBAL_INIT(Fix16, kFpOne_706A6C, Fix16(1), 0x706A6C);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_706C3C, Ang16(0), 0x706C3C);
DEFINE_GLOBAL(QuadVerts, gQuadVerts_706B88, 0x706B88);
EXTERN_GLOBAL(u32, gLightingDrawFlag_7068F4);

EXTERN_GLOBAL(s32, window_width_706630);
EXTERN_GLOBAL(s32, window_height_706B50);

DEFINE_GLOBAL(DWORD, gWindowRight_70675C, 0x70675C);
DEFINE_GLOBAL(DWORD, gWindowBottom_70679C, 0x70679C);

//u16 word_703BAA; //DEFINE_GLOBAL(u16, word_703BAA, 0x703BAA);

MATCH_FUNC(0x495470)
void __stdcall DrawTextureScaled_495470(STexture* pTexture, Fix16 x_pos, Fix16 y_pos, u8 width, u8 height, Ang16 rotation, s32 a7, u8 a8)
{
    DrawTexture_5D8470(pTexture,
               x_pos * gViewCamera_676978->field_A8_ui_scale,
               y_pos * gViewCamera_676978->field_A8_ui_scale,
               width,
               height,
               rotation,
               gViewCamera_676978->field_A8_ui_scale,
               a7,
               a8);
}

MATCH_FUNC(0x4B87A0)
void __stdcall DrawText_4B87A0(const wchar_t* pBuffer, Fix16 xpos_fp, Fix16 ypos_fp, s16 fontType, Fix16 scale)
{
    DrawText_5D8A10(pBuffer, xpos_fp, ypos_fp, fontType, scale, 2, 0, 0, 0);
}

MATCH_FUNC(0x5D7670)
void __stdcall DrawFigureScaled_5D7670(s32 sprite_type, s16 sprite_idx, Fix16 x_pos, Fix16 y_pos, Ang16 rotation, const s32& palette_type, s16 palette, s32 alpha_value, u8 flags)
{
    DrawFigure_5D7EC0(sprite_type,
               sprite_idx,
               x_pos * gViewCamera_676978->field_A8_ui_scale,
               y_pos * gViewCamera_676978->field_A8_ui_scale,
               rotation,
               gViewCamera_676978->field_A8_ui_scale,
               palette_type,
               palette,
               alpha_value,
               flags,
               0);
}

MATCH_FUNC(0x5D7700)
s32 __stdcall GetLineSpacingFromFontType_5D7700(u16 font_type)
{
    return (u16)gGtx_0x106C_703DD4->GetLineSpacing_5AA800(&font_type);
}

MATCH_FUNC(0x5D8940)
s32 __stdcall CountLineSpacing_5D8940(wchar_t* pStr, u16 font_type)
{
    s32 line_spacing = GetLineSpacingFromFontType_5D7700_inlined(font_type);

    s32 result = line_spacing;
    for (wchar_t* i = pStr; *i; ++i)
    {
        if (*i == '\n')
        {
            result += line_spacing;
        }
    }
    return result;
}

MATCH_FUNC(0x5D7720)
void __stdcall DrawText_5D7720(const wchar_t* pStr, Fix16 xoff, Fix16 yoff, u16 fontType, const s32& palette_type, u16 palette, s32 alpha, u8 alpha_flag)
{
    DrawText_5D8A10(pStr,
                    xoff * gViewCamera_676978->field_A8_ui_scale,
                    yoff * gViewCamera_676978->field_A8_ui_scale,
                    fontType,
                    gViewCamera_676978->field_A8_ui_scale,
                    palette_type,
                    palette,
                    alpha,
                    alpha_flag);
}

MATCH_FUNC(0x5D77A0)
void __stdcall DrawTextScaled_5D77A0(wchar_t* pText, Fix16 xpos, Fix16 ypos, u16 font_type)
{
    DrawText_5D8A10(pText,
                    xpos * gViewCamera_676978->field_A8_ui_scale,
                    ypos * gViewCamera_676978->field_A8_ui_scale,
                    font_type,
                    gViewCamera_676978->field_A8_ui_scale,
                    2,
                    0,
                    0,
                    0);
}

// The original is a tail-call thunk into the body at 0x5D7CC0 (9.6f: 0x4CAEB0 -> 0x4CADE0)
MATCH_FUNC(0x5D7CB0)
void __stdcall ConvertColourBanks_5D7CB0()
{
    pgbh_SetColourDepth();
    ConvertColourBanks_5D7CC0();
}

// https://decomp.me/scratch/zpWhI
MATCH_FUNC(0x5D7CC0)
void __stdcall ConvertColourBanks_5D7CC0()
{
    if (gGtx_0x106C_703DD4 && gGtx_0x106C_703DD4->field_6A_palettes_converted == 0)
    {
        s32 phys_pal_len = gGtx_0x106C_703DD4->get_physical_palettes_len_5AA900();
        u32 max_idx = (u32)phys_pal_len / 64;

        if (phys_pal_len % 64 != 0)
        {
            ++max_idx;
        }
        for (u16 i = 0; i < max_idx; i++)
        {
            pConvertColourBank(gGtx_0x106C_703DD4->GetPalData_5AA6A0(i * 64));
        }
        gGtx_0x106C_703DD4->field_6A_palettes_converted = 1;
    }
}

MATCH_FUNC(0x5D7D30)
void __stdcall MakeScreenTableAndSetWindow_5D7D30()
{
    pVid_GetSurface(gVidSys_7071D0);
    pMakeScreenTable((int)gVidSys_7071D0->field_50_surface_pixels_ptr,
                     gVidSys_7071D0->field_54_surface_pixels_pitch,
                     gVidSys_7071D0->field_4C_rect_bottom);

    if (gVidSys_7071D0->field_40_full_screen == -2)
    {
        gWindowRight_70675C = window_width_706630 - 1;
        gWindowBottom_70679C = window_height_706B50 - 1;
    }
    else
    {
        gWindowRight_70675C = gVidSys_7071D0->field_48_rect_right - 1;
        gWindowBottom_70679C = gVidSys_7071D0->field_4C_rect_bottom - 1;
    }

    pgbh_SetWindow(0, 0, (f32)gWindowRight_70675C, (f32)gWindowBottom_70679C);
}

// https://decomp.me/scratch/Zmms7
// 9.6f 0x4CBA50: v12/v13 are assigned after their declaration (the multiply result is copied into
// them), the flags test is one equality condition and the widths go through the Fix16(u8) ctor.
// The ternary, the field reads in u/v and `u32 flags` set the front-end size that gives the
// original's inline cut-offs: the second rotation's Negate_4086A0 inline, the others out of line.
MATCH_FUNC(0x5D7EC0)
void __stdcall DrawFigure_5D7EC0(s32 sprite_type,
                          s16 sprite_idx,
                          Fix16 x_pos,
                          Fix16 y_pos,
                          Ang16 rotation,
                          Fix16 scale,
                          const s32& palette_type,
                          s16 palette,
                          s32 alpha_value,
                          u8 og_flags,
                          char_type a11)
{
    sprite_index* sprite_index_5AA440 =
        gGtx_0x106C_703DD4->get_sprite_index_5AA440(gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_type, sprite_idx));
    Fix16_Point point;

    Fix16 v12;
    Fix16 v13;
    v12 = (Fix16(sprite_index_5AA440->field_4_width) / 2) * scale;
    v13 = (Fix16(sprite_index_5AA440->field_5_height) / 2) * scale;

    u32 flags;

    flags = (scale == kFpOne_706A6C && rotation == kAngZero_706C3C) ? 0x10000 : 0;
    if (!a11)
    {
        flags |= 0x20000u;
    }

    point.SetXY_432860(-v12, -v13);
    point.RotateByAngle_40F6B0(rotation);

    gQuadVerts_706B88.field_0_verts[0].x = ((x_pos + point.x).ToFloat());
    gQuadVerts_706B88.field_0_verts[0].y = ((y_pos + point.y).ToFloat());
    gQuadVerts_706B88.field_0_verts[0].z = 0.000099999997f;

    point.SetXY_432860(v12, -v13);
    point.RotateByAngle_40F6B0(rotation);

    gQuadVerts_706B88.field_0_verts[1].x = ((x_pos + point.x).ToFloat());
    gQuadVerts_706B88.field_0_verts[1].y = ((y_pos + point.y).ToFloat());
    gQuadVerts_706B88.field_0_verts[1].z = 0.000099999997f;

    point.SetXY_432860(v12, v13);
    point.RotateByAngle_40F6B0(rotation);

    gQuadVerts_706B88.field_0_verts[2].x = ((x_pos + point.x).ToFloat());
    gQuadVerts_706B88.field_0_verts[2].y = ((y_pos + point.y).ToFloat());
    gQuadVerts_706B88.field_0_verts[2].z = 0.000099999997f;

    point.SetXY_432860(-v12, v13);
    point.RotateByAngle_40F6B0(rotation);

    gQuadVerts_706B88.field_0_verts[3].x = ((x_pos + point.x).ToFloat());
    gQuadVerts_706B88.field_0_verts[3].y = ((y_pos + point.y).ToFloat());
    gQuadVerts_706B88.field_0_verts[3].z = 0.000099999997f;


    f32 u = sprite_index_5AA440->field_4_width - 0.000099999997f;
    f32 v = sprite_index_5AA440->field_5_height - 0.000099999997f;
    gQuadVerts_706B88.field_0_verts[0].u = 0.0;
    gQuadVerts_706B88.field_0_verts[0].v = 0.0;
    gQuadVerts_706B88.field_0_verts[1].v = 0.0;
    gQuadVerts_706B88.field_0_verts[3].u = 0.0;
    gQuadVerts_706B88.field_0_verts[1].u = u;
    gQuadVerts_706B88.field_0_verts[2].u = u;
    gQuadVerts_706B88.field_0_verts[2].v = v;
    gQuadVerts_706B88.field_0_verts[3].v = v;

    pgbh_DrawQuad(flags | CalcQuadFlags_5D83E0(alpha_value, og_flags),
                  gSharp_pare_0x15D8_705064->GetSpriteTexture_5B94F0(sprite_type, sprite_idx, palette_type, palette),
                  gQuadVerts_706B88.field_0_verts,
                  255);
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

// https://decomp.me/scratch/SCz1D
WIP_FUNC(0x5D8470);
void __stdcall DrawTexture_5D8470(STexture* pTexture,
                                 Fix16 x_pos,
                                 Fix16 y_pos,
                                 u8 width,
                                 u8 height,
                                 Ang16 rotation,
                                 Fix16 scale,
                                 s32 a8,
                                 u8 a9)
{
    Fix16_Point point;
    u32 flags;
    Fix16 v12;
    Fix16 v13;
    if (scale == kFpOne_706A6C && rotation == kAngZero_706C3C)
    {
        flags = 0x10000;
    }
    else
    {
        flags = 0;
    }

    v12 = (Fix16(width) / 2) * scale;
    v13 = (Fix16(height) / 2) * scale;

    // point 1

    point.SetXY_432860(-v12, -v13);
    point.RotateByAngle_40F6B0(rotation);

    gQuadVerts_706B88.field_0_verts[0].x = ((x_pos + point.x).ToFloat());
    gQuadVerts_706B88.field_0_verts[0].y = ((y_pos + point.y).ToFloat());
    gQuadVerts_706B88.field_0_verts[0].z = 0.000099999997f;

    // point 2

    point.SetXY_432860(v12, -v13);
    point.RotateByAngle_40F6B0(rotation);

    gQuadVerts_706B88.field_0_verts[1].x = ((x_pos + point.x).ToFloat());
    gQuadVerts_706B88.field_0_verts[1].y = ((y_pos + point.y).ToFloat());
    gQuadVerts_706B88.field_0_verts[1].z = 0.000099999997f;

    // point 3

    point.SetXY_432860(v12, v13);
    point.RotateByAngle_40F6B0(rotation);

    gQuadVerts_706B88.field_0_verts[2].x = ((x_pos + point.x).ToFloat());
    gQuadVerts_706B88.field_0_verts[2].y = ((y_pos + point.y).ToFloat());
    gQuadVerts_706B88.field_0_verts[2].z = 0.000099999997f;

    // point 4

    point.SetXY_432860(-v12, v13);
    point.RotateByAngle_40F6B0(rotation);

    gQuadVerts_706B88.field_0_verts[3].x = ((x_pos + point.x).ToFloat());
    gQuadVerts_706B88.field_0_verts[3].y = ((y_pos + point.y).ToFloat());
    gQuadVerts_706B88.field_0_verts[3].z = 0.000099999997f;
    // u & v, with z interleaved (store order found by the permuter)
    gQuadVerts_706B88.field_0_verts[0].u = 0.0;
    gQuadVerts_706B88.field_0_verts[0].v = 0.0;
    gQuadVerts_706B88.field_0_verts[1].v = 0.0;
    gQuadVerts_706B88.field_0_verts[3].u = 0.0;

    f32 u = width - 0.000099999997f;
    f32 v = height - 0.000099999997f;
    gQuadVerts_706B88.field_0_verts[1].u = u;
    gQuadVerts_706B88.field_0_verts[2].u = u;
    gQuadVerts_706B88.field_0_verts[2].v = v;
    gQuadVerts_706B88.field_0_verts[3].v = v;

    pgbh_DrawQuad(flags | CalcQuadFlags_5D83E0(a8, a9) | 0x20000, pTexture, gQuadVerts_706B88.field_0_verts, 255);
}

// https://decomp.me/scratch/HX0q9
MATCH_FUNC(0x5D8A10)
void __stdcall DrawText_5D8A10(const wchar_t* pText,
                               Fix16 xpos_fp,
                               Fix16 ypos_fp,
                               u16 font_type,
                               Fix16 scale_fp,
                               const s32& og_palette_type,
                               u16 og_palette,
                               s32 alpha_value,
                               u8 flags)
{
    s32 new_Flags = CalcQuadFlags_5D83E0(alpha_value, flags) | 0x20000;

    // The original walks a copy of pText (it reuses pText's stack slot as a float temp)
    const wchar_t* pIter = pText;
    // Declared ahead of cur_xpos: VC6's register tie-break then loads cur_xpos into ecx and
    // sprite_w into edx for the x + w sum, as the original does
    Fix16 sprite_w;
    Fix16 sprite_h;
    Fix16 cur_xpos = xpos_fp;

    Fix16 spaceWidth = scale_fp * gGtx_0x106C_703DD4->GetSpaceCharWidth_5AA7B0(&font_type);
    Fix16 lineHeight = scale_fp * (u16)gGtx_0x106C_703DD4->GetLineSpacing_5AA800(&font_type);

    u16 curr_palette = og_palette;
    u32 curr_palette_type = og_palette_type;

    if (scale_fp == kFpOne_706A6C)
    {
        new_Flags = new_Flags | 0x10000;
    }

    if (font_type >= 101u)
    {
        if (curr_palette_type == palette_types_enum::font_remaps_8)
        {
            gMagical_germain_0x8EC_6F5168->SetGlyphParamsFromRemap_4D29D0(og_palette);
        }
        else
        {
            gMagical_germain_0x8EC_6F5168->SetGlyphParamsFromFont_4D28A0(font_type);
        }
    }

    while (*pIter != 0)
    {
        wchar_t text_char = *pIter;

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
            // swap palettes
            if (curr_palette_type == og_palette_type && curr_palette == og_palette)
            {
                curr_palette_type = palette_types_enum::font_remaps_8;
                curr_palette = font_type < 0x65u ? 0 : 5;
            }
            else
            {
                curr_palette_type = og_palette_type;
                curr_palette = og_palette;
            }

            if (font_type >= 101u)
            {
                if (curr_palette_type == palette_types_enum::font_remaps_8)
                {
                    gMagical_germain_0x8EC_6F5168->SetGlyphParamsFromRemap_4D29D0(curr_palette);
                }
                else
                {
                    gMagical_germain_0x8EC_6F5168->SetGlyphParamsFromFont_4D28A0(font_type);
                }
            }
        }
        else
        {
            sprite_index* pSprIdx;
            STexture* pTexture;
            if (font_type < 0x65 || font_type > 107)
            {
                if (font_type < 0xC9 || font_type > 203)
                {
                    u16 sprt_relative_idx = gGtx_0x106C_703DD4->GetSpriteIdxFromFont_5AA710(font_type, text_char - 33);
                    u16 sprt_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::font_7, sprt_relative_idx);
                    pSprIdx = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprt_idx);
                    pTexture = gSharp_pare_0x15D8_705064->GetSpriteTexture_5B94F0(sprite_types_enum::font_7, sprt_relative_idx, curr_palette_type, curr_palette);
                }
                else
                {
                    pSprIdx = gMagical_germain_0x8EC_6F5168->field_8E0_sprite_index;
                    pTexture = gMagical_germain_0x8EC_6F5168->GetLargeGlyphTexture_4D27D0(text_char);
                }
            }
            else
            {
                pSprIdx = gMagical_germain_0x8EC_6F5168->field_8D4_sprite_index;
                pTexture = gMagical_germain_0x8EC_6F5168->GetSmallGlyphTexture_4D2710(text_char);
            }

            sprite_w = Fix16(pSprIdx->field_4_width) * scale_fp;
            sprite_h = Fix16(pSprIdx->field_5_height) * scale_fp;

            // Each corner converts its coordinate again: VC6 CSEs the repeated ToFloat()s
            // (x0/y0 stay on the FPU stack, x1/y1 go through a stack temp like the original)
            gQuadVerts_706B88.field_0_verts[0].x = cur_xpos.ToFloat();
            gQuadVerts_706B88.field_0_verts[0].y = ypos_fp.ToFloat();
            gQuadVerts_706B88.field_0_verts[0].z = 0.0001f;
            gQuadVerts_706B88.field_0_verts[1].x = (cur_xpos + sprite_w).ToFloat();
            gQuadVerts_706B88.field_0_verts[1].y = ypos_fp.ToFloat();
            gQuadVerts_706B88.field_0_verts[1].z = 0.0001f;
            gQuadVerts_706B88.field_0_verts[2].x = (cur_xpos + sprite_w).ToFloat();
            gQuadVerts_706B88.field_0_verts[2].y = (ypos_fp + sprite_h).ToFloat();
            gQuadVerts_706B88.field_0_verts[2].z = 0.0001f;
            gQuadVerts_706B88.field_0_verts[3].x = cur_xpos.ToFloat();
            gQuadVerts_706B88.field_0_verts[3].y = (ypos_fp + sprite_h).ToFloat();
            gQuadVerts_706B88.field_0_verts[3].z = 0.0001f;

            // The double parentheses are two no-op expression nodes each (see "x87 code: the scheduler
            // works in 81-node windows" in docs/matching_quirks.md). With these 4 nodes the window
            // breaks before the zero u/v stores, so they are issued after the DrawQuad pushes
            Fix16 u(((pSprIdx->field_4_width - 0.0001f)));
            Fix16 v(((pSprIdx->field_5_height - 0.0001f)));

            gQuadVerts_706B88.field_0_verts[0].u = 0.0f;
            gQuadVerts_706B88.field_0_verts[0].v = 0.0f;
            gQuadVerts_706B88.field_0_verts[1].v = 0.0f;
            gQuadVerts_706B88.field_0_verts[3].u = 0.0f;
            gQuadVerts_706B88.field_0_verts[1].u = u.ToFloat();
            gQuadVerts_706B88.field_0_verts[2].u = u.ToFloat();
            gQuadVerts_706B88.field_0_verts[2].v = v.ToFloat();
            gQuadVerts_706B88.field_0_verts[3].v = v.ToFloat();

            pgbh_DrawQuad(new_Flags, pTexture, &gQuadVerts_706B88.field_0_verts[0], 255);

            cur_xpos += sprite_w;
        }
        pIter++;
    }
}
