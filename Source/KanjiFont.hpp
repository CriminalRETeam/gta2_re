#pragma once

#include "Function.hpp"
#include <windows.h>

struct STexture;
struct sprite_index;

// A cached glyph texture: the character it currently shows, drawn with colour/outline_colour
struct KanjiGlyph
{
    u8 field_0_colour;
    u8 field_1_outline_colour;
    wchar_t field_2_text_char;
    STexture* field_4_pTexture;
    sprite_index* field_8_sprt_index;
    u32 field_C_last_used;
};
GTA2_ASSERT_SIZEOF_ALWAYS(KanjiGlyph, 0x10);

// Kanji font: glyph bitmaps loaded from data\kanji.dat (KIDX: character to glyph index, KBIT: 16x16 1bpp glyphs),
// rendered on demand into the textures of two least-recently-used glyph caches (small 16x16 and large 32x32).
class KanjiFont
{
  public:
    enum
    {
        k_num_small_glyphs = 120,
        k_num_large_glyphs = 20
    };

    KanjiGlyph field_0_small_glyphs[k_num_small_glyphs];
    KanjiGlyph field_780_large_glyphs[k_num_large_glyphs];
    s32 field_8C0_use_counter;
    u16* field_8C4_pKidX;
    u8* field_8C8_pKBIT;
    s32 field_8CC_kidx_size_words;
    u8* field_8D0_pSprtData;
    sprite_index* field_8D4_sprite_index;
    STexture* field_8D8_pTexture;
    u8* field_8DC_pSprtData;
    sprite_index* field_8E0_sprite_index;
    STexture* field_8E4_pTexture;
    u8 field_8E8_colour;
    u8 field_8E9_outline_colour;

    EXPORT void LoadChunks_4D1FC0(const char_type* pChunkId, u32 chunk_len);
    EXPORT void Load_kanji_dat_4D2090();
    EXPORT void OutlineGlyph_4D2150(u8* pPixelData, u16 width, u16 height);
    EXPORT u8* ExpandSmallGlyph_4D2240(const u8* pGlyph);
    EXPORT u8* ExpandLargeGlyph_4D23B0(const u8* pGlyph);
    EXPORT void RenderSmallGlyph_4D2610(wchar_t text_char);
    EXPORT void RenderLargeGlyph_4D2690(wchar_t text_char);
    EXPORT STexture* GetSmallGlyphTexture_4D2710(wchar_t text_char);
    EXPORT STexture* GetLargeGlyphTexture_4D27D0(wchar_t text_char);
    EXPORT void SetGlyphParamsFromFont_4D28A0(u16 font_type);
    EXPORT void SetGlyphParamsFromRemap_4D29D0(u16 remap);
    EXPORT void InitGlyphCaches_4D2B40();
    EXPORT KanjiFont();
    EXPORT ~KanjiFont();
};
GTA2_ASSERT_SIZEOF_ALWAYS(KanjiFont, 0x8EC);

EXTERN_GLOBAL(KanjiFont*, gKanjiFont_6F5168);