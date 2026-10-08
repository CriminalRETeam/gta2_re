#pragma once

#include "Function.hpp"
#include <windows.h>

struct STexture;
struct sprite_index;

// A cached glyph texture: the character it currently shows, drawn with colour/outline_colour
struct KanjiGlyph_10
{
    u8 field_0_colour;
    u8 field_1_outline_colour;
    wchar_t field_2_text_char;
    STexture* field_4_pTexture;
    sprite_index* field_8_sprt_index;
    u32 field_C_last_used;
};

/*
struct kanji_0x2
{
    char_type field_0;
    char_type field_1;
};

struct kanji_0x20
{
    kanji_0x2 field_0[16];
};
*/

// Kanji font: glyph bitmaps loaded from data\kanji.dat (KIDX: character to glyph index, KBIT: 16x16 1bpp glyphs),
// rendered on demand into the textures of two least-recently-used glyph caches (small 16x16 and large 32x32).
class KanjiFont_8EC
{
  public:
    enum
    {
        k_num_small_glyphs = 120,
        k_num_large_glyphs = 20
    };

    KanjiGlyph_10 field_0_small_glyphs[k_num_small_glyphs];
    KanjiGlyph_10 field_780_large_glyphs[k_num_large_glyphs];
    s32 field_8C0_use_counter;
    u16* field_8C4_pKidX;
    BYTE* field_8C8_pKBIT;
    s32 field_8CC_kidx_size_words;
    BYTE* field_8D0_pSprtData;
    sprite_index* field_8D4_sprite_index;
    STexture* field_8D8_pTexture;
    BYTE* field_8DC_pSprtData;
    sprite_index* field_8E0_sprite_index;
    STexture* field_8E4_pTexture;
    u8 field_8E8_colour;
    u8 field_8E9_outline_colour;

    EXPORT void LoadChunks_4D1FC0(const char_type* pChunkId, u32 chunk_len);
    EXPORT void Load_kanji_dat_4D2090();
    EXPORT void OutlineGlyph_4D2150(s32 pPixels, u16 width, u16 height);
    EXPORT u8* ExpandSmallGlyph_4D2240(char_type* pGlyph);
    EXPORT u8* ExpandLargeGlyph_4D23B0(char_type* pGlyph);
    EXPORT void RenderSmallGlyph_4D2610(wchar_t text_char);
    EXPORT void RenderLargeGlyph_4D2690(wchar_t text_char);
    EXPORT STexture* GetSmallGlyphTexture_4D2710(wchar_t text_char);
    EXPORT STexture* GetLargeGlyphTexture_4D27D0(wchar_t text_char);
    EXPORT void SetGlyphParamsFromFont_4D28A0(u16 font_type);
    EXPORT void SetGlyphParamsFromRemap_4D29D0(u16 remap);
    EXPORT void InitGlyphCaches_4D2B40();
    EXPORT KanjiFont_8EC();
    EXPORT ~KanjiFont_8EC();
};

EXTERN_GLOBAL(KanjiFont_8EC*, gKanjiFont_6F5168);