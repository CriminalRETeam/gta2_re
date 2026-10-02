#include "magical_germain_0x8EC.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "chunk.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include "Game_0x40.hpp"
#include "gbh_graphics.hpp"
#include "enums.hpp"
#include "sharp_pare_0x15D8.hpp"

DEFINE_GLOBAL(magical_germain_0x8EC*, gMagical_germain_0x8EC_6F5168, 0x6F5168);

MATCH_FUNC(0x4D1FC0)
void magical_germain_0x8EC::LoadChunks_4D1FC0(const char_type* pChunkId, u32 chunk_len)
{
    if (!strncmp(pChunkId, "KIDX", 4u))
    {
        field_8CC_kidx_size_words = chunk_len >> 1;
        field_8C4_pKidX = new WORD[field_8CC_kidx_size_words];
        if (!field_8C4_pKidX)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\kanji.cpp", 142);
        }
        File::Global_Read_4A71C0(field_8C4_pKidX, chunk_len);
    }
    else if (!strncmp(pChunkId, "KBIT", 4u))
    {
        field_8C8_pKBIT = new BYTE[chunk_len];
        if (!field_8C8_pKBIT)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\kanji.cpp", 148);
        }
        File::Global_Read_4A71C0(field_8C8_pKBIT, chunk_len);
    }
    else
    {
        File::Global_Seek_4A7140(&chunk_len);
    }
}

MATCH_FUNC(0x4D2090)
void magical_germain_0x8EC::Load_kanji_dat_4D2090()
{
    File::Global_Open_4A7060("data\\kanji.dat");

    file_header header;
    u32 readSize = sizeof(file_header);
    File::Global_Read_4A71C0(&header, readSize);

    header.verify_type("KANJ");
    header.verify_version(100);

    chunk_header chunkHeader; // [esp+10h] [ebp-8h] BYREF
    for (readSize = sizeof(chunkHeader); File::Global_Read_4A7210(&chunkHeader, &readSize); readSize = sizeof(chunkHeader))
    {
        if (chunkHeader.field_4_size)
        {
            LoadChunks_4D1FC0(chunkHeader.field_0_type, chunkHeader.field_4_size);
        }
    }
    File::Global_Close_4A70C0();
}

// Outlines the pixels of colour field_8E8_v1 with colour field_8E9_v2 in a 256 wide sprite
MATCH_FUNC(0x4D2150)
void magical_germain_0x8EC::sub_4D2150(s32 a2, u16 width, u16 height)
{
    u8(*pPixels)[256] = reinterpret_cast<u8(*)[256]>(a2);
    for (s32 y = 0; y < height; y++)
    {
        for (s32 x = 0; x < width; x++)
        {
            if (pPixels[y][x] == field_8E8_v1)
            {
                if (x > 0 && pPixels[y][x - 1] != field_8E8_v1)
                {
                    pPixels[y][x - 1] = field_8E9_v2;
                }
                if (x < width - 1 && pPixels[y][x + 1] != field_8E8_v1)
                {
                    pPixels[y][x + 1] = field_8E9_v2;
                }
                if (y > 0 && pPixels[y - 1][x] != field_8E8_v1)
                {
                    pPixels[y - 1][x] = field_8E9_v2;
                }
                if (y < height - 1 && pPixels[y + 1][x] != field_8E8_v1)
                {
                    pPixels[y + 1][x] = field_8E9_v2;
                }
            }
        }
    }
}

// Expands a 16x16 1bpp glyph into the 256 pixel wide 8bpp sprite data
MATCH_FUNC(0x4D2240)
u8* magical_germain_0x8EC::sub_4D2240(char_type* pGlyph)
{
    u8* pDst = field_8D0_pSprtData;
    u8* pBits = (u8*)pGlyph;
    u8 bits;
    for (s32 row = 0; row < 16; row++)
    {
        bits = *pBits++;
        *pDst++ = (bits & 0x80) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x40) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x20) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x10) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x8) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x4) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x2) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x1) ? field_8E8_v1 : 0;
        bits = *pBits++;
        *pDst++ = (bits & 0x80) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x40) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x20) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x10) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x8) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x4) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x2) ? field_8E8_v1 : 0;
        *pDst++ = (bits & 0x1) ? field_8E8_v1 : 0;
        pDst += 256 - 16;
    }
    return pDst;
}

// Expands a 16x16 1bpp glyph at double size (2x2 pixels per bit) into the 256 pixel wide 8bpp sprite data
MATCH_FUNC(0x4D23B0)
u8* magical_germain_0x8EC::sub_4D23B0(char_type* pGlyph)
{
    u8* pDst = field_8DC_pSprtData;
    u8* pBits = (u8*)pGlyph;
    u8 bits;
    u8 colour;
    for (s32 row = 0; row < 16; row++)
    {
        bits = *pBits++;
        colour = (bits & 0x80) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x40) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x20) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x10) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x8) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x4) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x2) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x1) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        bits = *pBits++;
        colour = (bits & 0x80) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x40) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x20) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x10) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x8) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x4) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x2) ? field_8E8_v1 : 0;
        pDst[0] = colour;
        pDst[1] = colour;
        pDst[256] = colour;
        pDst[257] = colour;
        pDst += 2;
        colour = (bits & 0x1) ? field_8E8_v1 : 0;
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
void magical_germain_0x8EC::RenderSmallGlyph_4D2610(wchar_t text_char)
{
    u16 v2 = text_char;
    if (text_char < 0x100u)
    {
        v2 = text_char << 8;
    }

    u16 v6 = field_8C4_pKidX[v2];
    if (v6 == 0xFFFF)
    {
        v6 = field_8C4_pKidX[8448];
    }
    pgbh_LockTexture(field_8D8_pTexture);
    magical_germain_0x8EC::sub_4D2240((char_type*)&field_8C8_pKBIT[32 * v6]); // OBS: probably a pointer
    magical_germain_0x8EC::sub_4D2150((s32)field_8D0_pSprtData, 16, 17);
    pgbh_UnlockTexture(field_8D8_pTexture);
}

MATCH_FUNC(0x4D2690)
void magical_germain_0x8EC::RenderLargeGlyph_4D2690(wchar_t text_char)
{
    u16 v2 = text_char;
    if (text_char < 0x100u)
    {
        v2 = text_char << 8;
    }

    u16 v6 = field_8C4_pKidX[v2];
    if (v6 == 0xFFFF)
    {
        v6 = field_8C4_pKidX[8448];
    }
    pgbh_LockTexture(field_8E4_pTexture);
    magical_germain_0x8EC::sub_4D23B0((char_type*)&field_8C8_pKBIT[32 * v6]); // OBS: probably a pointer
    magical_germain_0x8EC::sub_4D2150((s32)field_8DC_pSprtData, 32, 34);
    pgbh_UnlockTexture(field_8E4_pTexture);
}

MATCH_FUNC(0x4D2710)
STexture* magical_germain_0x8EC::GetSmallGlyphTexture_4D2710(wchar_t text_char)
{
    kanji_0x10* pFound;
    kanji_0x10* pCurrent;
    u32 nearestId = -1;

    for (s32 i = 0; i < 120; i++)
    {
        pCurrent = &field_0_small_glyphs[i];
        if (pCurrent->field_2_text_char == text_char && pCurrent->field_0_v1 == field_8E8_v1 && pCurrent->field_1_v2 == field_8E9_v2)
        {
            pCurrent->field_C_id = field_8C0_count++;
            return pCurrent->field_4_pTexture;
        }

        if (pCurrent->field_C_id < nearestId)
        {
            nearestId = pCurrent->field_C_id;
            pFound = pCurrent;
        }
    }

    field_8D0_pSprtData = pFound->field_8_sprt_index->field_0_pData;
    field_8D8_pTexture = pFound->field_4_pTexture;

    pFound->field_0_v1 = field_8E8_v1;
    pFound->field_1_v2 = field_8E9_v2;
    pFound->field_2_text_char = text_char;
    pFound->field_C_id = field_8C0_count++;

    RenderSmallGlyph_4D2610(text_char);

    return field_8D8_pTexture;
}

MATCH_FUNC(0x4D27D0)
STexture* magical_germain_0x8EC::GetLargeGlyphTexture_4D27D0(wchar_t text_char)
{
    kanji_0x10* pFound;
    kanji_0x10* pCurrent;
    u32 nearestId = -1;

    for (s32 i = 0; i < 20; i++)
    {
        pCurrent = &field_780_large_glyphs[i];
        if (pCurrent->field_2_text_char == text_char && pCurrent->field_0_v1 == field_8E8_v1 && pCurrent->field_1_v2 == field_8E9_v2)
        {
            pCurrent->field_C_id = field_8C0_count++;
            return pCurrent->field_4_pTexture;
        }

        if (pCurrent->field_C_id < nearestId)
        {
            nearestId = pCurrent->field_C_id;
            pFound = pCurrent;
        }
    }

    field_8DC_pSprtData = pFound->field_8_sprt_index->field_0_pData;
    field_8E4_pTexture = pFound->field_4_pTexture;

    pFound->field_0_v1 = field_8E8_v1;
    pFound->field_1_v2 = field_8E9_v2;
    pFound->field_2_text_char = text_char;
    pFound->field_C_id = field_8C0_count++;

    RenderLargeGlyph_4D2690(text_char);

    return field_8E4_pTexture;
}

MATCH_FUNC(0x4D28A0)
void magical_germain_0x8EC::SetGlyphParamsFromFont_4D28A0(u16 font_type)
{
    switch (font_type)
    {
        case 0x65u:
        case 0xC9u:
            field_8E8_v1 = -4;
            field_8E9_v2 = -6;
            break;
        case 0x66u:
            field_8E8_v1 = 39;
            field_8E9_v2 = 44;
            break;
        case 0x67u:
            field_8E8_v1 = -53;
            field_8E9_v2 = -49;
            break;
        case 0x68u:
            field_8E8_v1 = 28;
            field_8E9_v2 = 24;
            break;
        case 0x69u:
        case 0xCAu:
            field_8E8_v1 = -23;
            field_8E9_v2 = -17;
            break;
        case 0x6Au:
            field_8E8_v1 = -119;
            field_8E9_v2 = -115;
            break;
        case 0x6Bu:
        case 0xCBu:
            field_8E8_v1 = 72;
            field_8E9_v2 = 76;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4D29D0)
void magical_germain_0x8EC::SetGlyphParamsFromRemap_4D29D0(u16 a2)
{
    if (gGame_0x40_67E008)
    {
        switch (a2)
        {
            case 0:
                field_8E8_v1 = 0x89;
                field_8E9_v2 = 0x8D;
                break;
            case 1:
                field_8E8_v1 = 0x99;
                field_8E9_v2 = 0x9D;
                break;
            case 2:
                field_8E8_v1 = 0x69;
                field_8E9_v2 = 0x6D;
                break;
            case 3:
                field_8E8_v1 = 0x79;
                field_8E9_v2 = 0x7D;
                break;
            case 4:
                field_8E8_v1 = 0x35;
                field_8E9_v2 = 0x3A;
                break;
            case 5:
                field_8E8_v1 = 0x27;
                field_8E9_v2 = 0x2C;
                break;
            case 6:
                field_8E8_v1 = 0x1C;
                field_8E9_v2 = 0x18;
                break;
            case 7:
                field_8E8_v1 = 0x48;
                field_8E9_v2 = 0x4C;
                break;
            case 8:
                field_8E8_v1 = 0xFC;
                field_8E9_v2 = 0xFA;
                break;
        }
    }
    else
    {
        switch (a2)
        {
            case 0:
            case 1:
            case 2:
                field_8E8_v1 = 0xE9;
                field_8E9_v2 = 0xEF;
                break;
            case 3:
                field_8E8_v1 = 0x89;
                field_8E9_v2 = 0x8D;
                break;
            case 4:
                field_8E8_v1 = 0x27;
                field_8E9_v2 = 0x2C;
                break;
            case 5:
                field_8E8_v1 = 0xFC;
                field_8E9_v2 = 0xFA;
                break;
            case 8:
                field_8E8_v1 = 0xF8;
                field_8E9_v2 = 0xF5;
                break;
            case 10:
                field_8E8_v1 = 0x27;
                field_8E9_v2 = 0x23;
                break;
            case 13:
                field_8E8_v1 = 0x89;
                field_8E9_v2 = 0x8D;
                break;
            case 15:
                field_8E8_v1 = 0x48;
                field_8E9_v2 = 0x4C;
                break;
            case 14:
                field_8E8_v1 = 0x69;
                field_8E9_v2 = 0x6D;
                break;
        }
    }
}

MATCH_FUNC(0x4D2B40)
void magical_germain_0x8EC::InitGlyphCaches_4D2B40()
{
    u16 v2 = gGtx_0x106C_703DD4->GetSpriteIdxFromFont_5AA710(word_703C3E, 0);
    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::font_7, v2);
    field_8D4_sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_idx);

    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_small_glyphs); i++)
    {
        kanji_0x10* pKanji = &field_0_small_glyphs[i];
        pKanji->field_2_text_char = 0;
        pKanji->field_0_v1 = 0;
        pKanji->field_1_v2 = 0;
        pKanji->field_C_id = 0;

        u16 v6 = gGtx_0x106C_703DD4->GetSpriteIdxFromFont_5AA710(word_703C3E, i);
        u16 v7 = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::font_7, v6);
        pKanji->field_8_sprt_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v7);
        pKanji->field_4_pTexture = gSharp_pare_0x15D8_705064->GetSpriteTexture_5B94F0(7, v6, 2, 0);
    }

    u16 v8 = gGtx_0x106C_703DD4->GetSpriteIdxFromFont_5AA710(word_703D9A, 0);
    u16 v9 = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::font_7, v8);

    field_8E0_sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v9);

    for (s32 j = 0; j < GTA2_COUNTOF_S(field_780_large_glyphs); j++)
    {
        kanji_0x10* pKanji_2 = &field_780_large_glyphs[j];
        pKanji_2->field_2_text_char = 0;
        pKanji_2->field_0_v1 = 0;
        pKanji_2->field_1_v2 = 0;
        pKanji_2->field_C_id = 0;

        u16 v6 = gGtx_0x106C_703DD4->GetSpriteIdxFromFont_5AA710(word_703D9A, j);
        u16 v7 = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::font_7, v6);
        pKanji_2->field_8_sprt_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v7);
        pKanji_2->field_4_pTexture = gSharp_pare_0x15D8_705064->GetSpriteTexture_5B94F0(7, v6, 2, 0);
    }
}

MATCH_FUNC(0x4D2C80)
magical_germain_0x8EC::magical_germain_0x8EC()
{
    field_8E8_v1 = -2;
    field_8E9_v2 = -9;
    field_8C8_pKBIT = 0;
    field_8C4_pKidX = 0;
    field_8CC_kidx_size_words = 0;
    field_8C0_count = 0;
    Load_kanji_dat_4D2090();
}

MATCH_FUNC(0x4D2CC0)
magical_germain_0x8EC::~magical_germain_0x8EC()
{
    if (field_8C8_pKBIT)
    {
        delete[] field_8C8_pKBIT;
    }

    if (field_8C4_pKidX)
    {
        delete[] field_8C4_pKidX;
    }

    field_8CC_kidx_size_words = 0;
}