#include "KanjiFont.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "chunk.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include "Game.hpp"
#include "gbh_graphics.hpp"
#include "enums.hpp"
#include "TextureCache.hpp"

DEFINE_GLOBAL(KanjiFont*, gKanjiFont_6F5168, 0x6F5168);

MATCH_FUNC(0x4D1FC0)
void KanjiFont::LoadChunks(const char_type* pChunkId, u32 chunk_len)
{
    if (!strncmp(pChunkId, "KIDX", 4u))
    {
        mKidxSizeWords = chunk_len >> 1;
        mpKidX = new u16[mKidxSizeWords];
        if (!mpKidX)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\kanji.cpp", 142);
        }
        File::Global_Read_4A71C0(mpKidX, chunk_len);
    }
    else if (!strncmp(pChunkId, "KBIT", 4u))
    {
        mpKBIT = new u8[chunk_len];
        if (!mpKBIT)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\kanji.cpp", 148);
        }
        File::Global_Read_4A71C0(mpKBIT, chunk_len);
    }
    else
    {
        File::Global_Seek(&chunk_len);
    }
}

MATCH_FUNC(0x4D2090)
void KanjiFont::Load_kanji_dat()
{
    File::Global_Open("data\\kanji.dat");

    file_header header;
    u32 readSize = sizeof(file_header);
    File::Global_Read_4A71C0(&header, readSize);

    header.verify_type("KANJ");
    header.verify_version(100);

    chunk_header chunkHeader;
    for (readSize = sizeof(chunkHeader); File::Global_Read_4A7210(&chunkHeader, &readSize); readSize = sizeof(chunkHeader))
    {
        if (chunkHeader.mSize)
        {
            LoadChunks(chunkHeader.mType, chunkHeader.mSize);
        }
    }
    File::Global_Close();
}

// Outlines the pixels of colour mColour with colour mOutlineColour in a 256 wide sprite
MATCH_FUNC(0x4D2150)
void KanjiFont::OutlineGlyph(u8* pPixelData, u16 width, u16 height)
{
    u8(*pPixels)[256] = reinterpret_cast<u8(*)[256]>(pPixelData);
    for (s32 y = 0; y < height; y++)
    {
        for (s32 x = 0; x < width; x++)
        {
            if (pPixels[y][x] == mColour)
            {
                if (x > 0 && pPixels[y][x - 1] != mColour)
                {
                    pPixels[y][x - 1] = mOutlineColour;
                }
                if (x < width - 1 && pPixels[y][x + 1] != mColour)
                {
                    pPixels[y][x + 1] = mOutlineColour;
                }
                if (y > 0 && pPixels[y - 1][x] != mColour)
                {
                    pPixels[y - 1][x] = mOutlineColour;
                }
                if (y < height - 1 && pPixels[y + 1][x] != mColour)
                {
                    pPixels[y + 1][x] = mOutlineColour;
                }
            }
        }
    }
}

// Expands a 16x16 1bpp glyph into the 256 pixel wide 8bpp sprite data
MATCH_FUNC(0x4D2240)
u8* KanjiFont::ExpandSmallGlyph(const u8* pGlyph)
{
    u8* pDst = mpSprtData8D0;
    const u8* pBits = pGlyph;
    u8 bits;
    for (s32 row = 0; row < 16; row++)
    {
        bits = *pBits++;
        *pDst++ = (bits & 0x80) ? mColour : 0;
        *pDst++ = (bits & 0x40) ? mColour : 0;
        *pDst++ = (bits & 0x20) ? mColour : 0;
        *pDst++ = (bits & 0x10) ? mColour : 0;
        *pDst++ = (bits & 0x8) ? mColour : 0;
        *pDst++ = (bits & 0x4) ? mColour : 0;
        *pDst++ = (bits & 0x2) ? mColour : 0;
        *pDst++ = (bits & 0x1) ? mColour : 0;
        bits = *pBits++;
        *pDst++ = (bits & 0x80) ? mColour : 0;
        *pDst++ = (bits & 0x40) ? mColour : 0;
        *pDst++ = (bits & 0x20) ? mColour : 0;
        *pDst++ = (bits & 0x10) ? mColour : 0;
        *pDst++ = (bits & 0x8) ? mColour : 0;
        *pDst++ = (bits & 0x4) ? mColour : 0;
        *pDst++ = (bits & 0x2) ? mColour : 0;
        *pDst++ = (bits & 0x1) ? mColour : 0;
        pDst += 256 - 16;
    }
    return pDst;
}

// Expands a 16x16 1bpp glyph at double size (2x2 pixels per bit) into the 256 pixel wide 8bpp sprite data
MATCH_FUNC(0x4D23B0)
u8* KanjiFont::ExpandLargeGlyph(const u8* pGlyph)
{
    u8* pDst = mpSprtData8DC;
    const u8* pBits = pGlyph;
    u8 bits;
    u8 colour;
    for (s32 row = 0; row < 16; row++)
    {
        bits = *pBits++;
        colour = (bits & 0x80) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x40) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x20) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x10) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x8) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x4) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x2) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x1) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        bits = *pBits++;
        colour = (bits & 0x80) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x40) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x20) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x10) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x8) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x4) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x2) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x1) ? mColour : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        pDst += 2 * 256 - 32;
    }
    return pDst;
}

MATCH_FUNC(0x4D2610)
void KanjiFont::RenderSmallGlyph(wchar_t text_char)
{
    u16 kidx = text_char;
    if (text_char < 0x100u)
    {
        kidx = text_char << 8;
    }

    u16 glyph_idx = mpKidX[kidx];
    if (glyph_idx == 0xFFFF)
    {
        glyph_idx = mpKidX[8448];
    }
    pgbh_LockTexture(mpTexture8D8);
    ExpandSmallGlyph(&mpKBIT[32 * glyph_idx]);
    OutlineGlyph(mpSprtData8D0, 16, 17);
    pgbh_UnlockTexture(mpTexture8D8);
}

MATCH_FUNC(0x4D2690)
void KanjiFont::RenderLargeGlyph(wchar_t text_char)
{
    u16 kidx = text_char;
    if (text_char < 0x100u)
    {
        kidx = text_char << 8;
    }

    u16 glyph_idx = mpKidX[kidx];
    if (glyph_idx == 0xFFFF)
    {
        glyph_idx = mpKidX[8448];
    }
    pgbh_LockTexture(mpTexture8E4);
    ExpandLargeGlyph(&mpKBIT[32 * glyph_idx]);
    OutlineGlyph(mpSprtData8DC, 32, 34);
    pgbh_UnlockTexture(mpTexture8E4);
}

MATCH_FUNC(0x4D2710)
STexture* KanjiFont::GetSmallGlyphTexture(wchar_t text_char)
{
    KanjiGlyph* pOldest;
    KanjiGlyph* pCurrent;
    u32 oldest_last_used = -1;

    for (s32 i = 0; i < k_num_small_glyphs; i++)
    {
        pCurrent = &mSmallGlyphs[i];
        if (pCurrent->mTextChar == text_char && pCurrent->mColour == mColour && pCurrent->mOutlineColour == mOutlineColour)
        {
            pCurrent->mLastUsed = mUseCounter++;
            return pCurrent->mpTexture;
        }

        if (pCurrent->mLastUsed < oldest_last_used)
        {
            oldest_last_used = pCurrent->mLastUsed;
            pOldest = pCurrent;
        }
    }

    mpSprtData8D0 = pOldest->mSprtIndex->mpData;
    mpTexture8D8 = pOldest->mpTexture;

    pOldest->mColour = mColour;
    pOldest->mOutlineColour = mOutlineColour;
    pOldest->mTextChar = text_char;
    pOldest->mLastUsed = mUseCounter++;

    RenderSmallGlyph(text_char);

    return mpTexture8D8;
}

MATCH_FUNC(0x4D27D0)
STexture* KanjiFont::GetLargeGlyphTexture(wchar_t text_char)
{
    KanjiGlyph* pOldest;
    KanjiGlyph* pCurrent;
    u32 oldest_last_used = -1;

    for (s32 i = 0; i < k_num_large_glyphs; i++)
    {
        pCurrent = &mLargeGlyphs[i];
        if (pCurrent->mTextChar == text_char && pCurrent->mColour == mColour && pCurrent->mOutlineColour == mOutlineColour)
        {
            pCurrent->mLastUsed = mUseCounter++;
            return pCurrent->mpTexture;
        }

        if (pCurrent->mLastUsed < oldest_last_used)
        {
            oldest_last_used = pCurrent->mLastUsed;
            pOldest = pCurrent;
        }
    }

    mpSprtData8DC = pOldest->mSprtIndex->mpData;
    mpTexture8E4 = pOldest->mpTexture;

    pOldest->mColour = mColour;
    pOldest->mOutlineColour = mOutlineColour;
    pOldest->mTextChar = text_char;
    pOldest->mLastUsed = mUseCounter++;

    RenderLargeGlyph(text_char);

    return mpTexture8E4;
}

MATCH_FUNC(0x4D28A0)
void KanjiFont::SetGlyphParamsFromFont(u16 font_type)
{
    switch (font_type)
    {
        case 0x65u:
        case 0xC9u:
            mColour = 0xFC;
            mOutlineColour = 0xFA;
            break;
        case 0x66u:
            mColour = 0x27;
            mOutlineColour = 0x2C;
            break;
        case 0x67u:
            mColour = 0xCB;
            mOutlineColour = 0xCF;
            break;
        case 0x68u:
            mColour = 0x1C;
            mOutlineColour = 0x18;
            break;
        case 0x69u:
        case 0xCAu:
            mColour = 0xE9;
            mOutlineColour = 0xEF;
            break;
        case 0x6Au:
            mColour = 0x89;
            mOutlineColour = 0x8D;
            break;
        case 0x6Bu:
        case 0xCBu:
            mColour = 0x48;
            mOutlineColour = 0x4C;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4D29D0)
void KanjiFont::SetGlyphParamsFromRemap(u16 remap)
{
    if (gGame_0x40_67E008)
    {
        switch (remap)
        {
            case 0:
                mColour = 0x89;
                mOutlineColour = 0x8D;
                break;
            case 1:
                mColour = 0x99;
                mOutlineColour = 0x9D;
                break;
            case 2:
                mColour = 0x69;
                mOutlineColour = 0x6D;
                break;
            case 3:
                mColour = 0x79;
                mOutlineColour = 0x7D;
                break;
            case 4:
                mColour = 0x35;
                mOutlineColour = 0x3A;
                break;
            case 5:
                mColour = 0x27;
                mOutlineColour = 0x2C;
                break;
            case 6:
                mColour = 0x1C;
                mOutlineColour = 0x18;
                break;
            case 7:
                mColour = 0x48;
                mOutlineColour = 0x4C;
                break;
            case 8:
                mColour = 0xFC;
                mOutlineColour = 0xFA;
                break;
        }
    }
    else
    {
        switch (remap)
        {
            case 0:
            case 1:
            case 2:
                mColour = 0xE9;
                mOutlineColour = 0xEF;
                break;
            case 3:
                mColour = 0x89;
                mOutlineColour = 0x8D;
                break;
            case 4:
                mColour = 0x27;
                mOutlineColour = 0x2C;
                break;
            case 5:
                mColour = 0xFC;
                mOutlineColour = 0xFA;
                break;
            case 8:
                mColour = 0xF8;
                mOutlineColour = 0xF5;
                break;
            case 10:
                mColour = 0x27;
                mOutlineColour = 0x23;
                break;
            case 13:
                mColour = 0x89;
                mOutlineColour = 0x8D;
                break;
            case 15:
                mColour = 0x48;
                mOutlineColour = 0x4C;
                break;
            case 14:
                mColour = 0x69;
                mOutlineColour = 0x6D;
                break;
        }
    }
}

MATCH_FUNC(0x4D2B40)
void KanjiFont::InitGlyphCaches()
{
    u16 base_font_idx = gGtx_0x106C_703DD4->GetSpriteIdxFromFont(gKanjiSmallFont_703C3E, 0);
    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex(sprite_types_enum::font_7, base_font_idx);
    mSpriteIndex8D4 = gGtx_0x106C_703DD4->get_sprite_index(sprite_idx);

    for (s32 i = 0; i < GTA2_COUNTOF_S(mSmallGlyphs); i++)
    {
        KanjiGlyph* pGlyph = &mSmallGlyphs[i];
        pGlyph->mTextChar = 0;
        pGlyph->mColour = 0;
        pGlyph->mOutlineColour = 0;
        pGlyph->mLastUsed = 0;

        u16 font_idx = gGtx_0x106C_703DD4->GetSpriteIdxFromFont(gKanjiSmallFont_703C3E, i);
        u16 true_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex(sprite_types_enum::font_7, font_idx);
        pGlyph->mSprtIndex = gGtx_0x106C_703DD4->get_sprite_index(true_idx);
        pGlyph->mpTexture = gTextureCache_15D8_705064->GetSpriteTexture(sprite_types_enum::font_7, font_idx, palette_types_enum::sprites_2, 0);
    }

    u16 large_base_font_idx = gGtx_0x106C_703DD4->GetSpriteIdxFromFont(gKanjiLargeFont_703D9A, 0);
    u16 large_sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex(sprite_types_enum::font_7, large_base_font_idx);

    mSpriteIndex8E0 = gGtx_0x106C_703DD4->get_sprite_index(large_sprite_idx);

    for (s32 j = 0; j < GTA2_COUNTOF_S(mLargeGlyphs); j++)
    {
        KanjiGlyph* pGlyph = &mLargeGlyphs[j];
        pGlyph->mTextChar = 0;
        pGlyph->mColour = 0;
        pGlyph->mOutlineColour = 0;
        pGlyph->mLastUsed = 0;

        u16 font_idx = gGtx_0x106C_703DD4->GetSpriteIdxFromFont(gKanjiLargeFont_703D9A, j);
        u16 true_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex(sprite_types_enum::font_7, font_idx);
        pGlyph->mSprtIndex = gGtx_0x106C_703DD4->get_sprite_index(true_idx);
        pGlyph->mpTexture = gTextureCache_15D8_705064->GetSpriteTexture(sprite_types_enum::font_7, font_idx, palette_types_enum::sprites_2, 0);
    }
}

MATCH_FUNC(0x4D2C80)
KanjiFont::KanjiFont()
{
    mColour = 0xFE;
    mOutlineColour = 0xF7;
    mpKBIT = NULL;
    mpKidX = NULL;
    mKidxSizeWords = 0;
    mUseCounter = 0;
    Load_kanji_dat();
}

MATCH_FUNC(0x4D2CC0)
KanjiFont::~KanjiFont()
{
    if (mpKBIT)
    {
        delete[] mpKBIT;
    }

    if (mpKidX)
    {
        delete[] mpKidX;
    }

    mKidxSizeWords = 0;
}