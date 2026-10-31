#pragma once

#include "Function.hpp"
#include <windows.h>

struct STexture;
struct sprite_index;

// A cached glyph texture: the character it currently shows, drawn with colour/outline_colour
struct KanjiGlyph
{
    u8 mColour; // 0x0
    u8 mOutlineColour; // 0x1
    wchar_t mTextChar; // 0x2
    STexture* mpTexture; // 0x4
    sprite_index* mSprtIndex; // 0x8
    u32 mLastUsed; // 0xC
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

    KanjiGlyph mSmallGlyphs[k_num_small_glyphs]; // 0x0
    KanjiGlyph mLargeGlyphs[k_num_large_glyphs]; // 0x780
    s32 mUseCounter; // 0x8C0
    u16* mpKidX; // 0x8C4
    u8* mpKBIT; // 0x8C8
    s32 mKidxSizeWords; // 0x8CC
    u8* mpSprtData8D0; // 0x8D0
    sprite_index* mSpriteIndex8D4; // 0x8D4
    STexture* mpTexture8D8; // 0x8D8
    u8* mpSprtData8DC; // 0x8DC
    sprite_index* mSpriteIndex8E0; // 0x8E0
    STexture* mpTexture8E4; // 0x8E4
    u8 mColour; // 0x8E8
    u8 mOutlineColour; // 0x8E9

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