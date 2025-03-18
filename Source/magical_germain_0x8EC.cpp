#include "magical_germain_0x8EC.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "chunk.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include "Game_0x40.hpp"

EXPORT_VAR magical_germain_0x8EC* gMagical_germain_0x8EC_6F5168;
GLOBAL(gMagical_germain_0x8EC_6F5168, 0x6F5168);

MATCH_FUNC(0x4D1FC0)
void magical_germain_0x8EC::LoadChunks_4D1FC0(const char_type* pChunkId, u32 chunk_len)
{
    if (!strncmp(pChunkId, "KIDX", 4u))
    {
        field_8CC_kidx_size_words = chunk_len >> 1;
        field_8C4_pKidX = new WORD[field_8CC_kidx_size_words];
        if (!field_8C4_pKidX)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\kanji.cpp", 142);
        }
        File::Global_Read_4A71C0(field_8C4_pKidX, chunk_len);
    }
    else if (!strncmp(pChunkId, "KBIT", 4u))
    {
        field_8C8_pKBIT = new BYTE[chunk_len];
        if (!field_8C8_pKBIT)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\kanji.cpp", 148);
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

STUB_FUNC(0x4D2150)
void magical_germain_0x8EC::sub_4D2150(s32 a2, u16 a3, u16 a4)
{
}

STUB_FUNC(0x4D2240)
u8* magical_germain_0x8EC::sub_4D2240(char_type* a2)
{
    return 0;
}

STUB_FUNC(0x4D23B0)
u8* magical_germain_0x8EC::sub_4D23B0(char_type* a2)
{
    return 0;
}

STUB_FUNC(0x4D2610)
void magical_germain_0x8EC::sub_4D2610(wchar_t text_char)
{
}

STUB_FUNC(0x4D2690)
s32 magical_germain_0x8EC::sub_4D2690(u16 a2)
{
    return 0;
}

MATCH_FUNC(0x4D2710)
STexture* magical_germain_0x8EC::sub_4D2710(wchar_t text_char)
{
    kanji_0x10* pFound;
    kanji_0x10* pCurrent;
    u32 nearestId = -1;

    for (s32 i = 0; i < 120; i++)
    {
        pCurrent = &field_0[i];
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

    sub_4D2610(text_char);

    return field_8D8_pTexture;
}

STUB_FUNC(0x4D27D0)
STexture* magical_germain_0x8EC::sub_4D27D0(u16 a2)
{
    return 0;
}

MATCH_FUNC(0x4D28A0)
void magical_germain_0x8EC::sub_4D28A0(u16 font_type)
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

STUB_FUNC(0x4D29D0)
void magical_germain_0x8EC::sub_4D29D0(u16 a2)
{
    if (gGame_0x40_67E008)
    {
        switch (a2)
        {
            case 0u:
                goto LABEL_3;
            case 1u:
                field_8E8_v1 = -103;
                field_8E9_v2 = -99;
                break;
            case 2u:
                goto LABEL_15;
            case 3u:
                field_8E8_v1 = 121;
                field_8E9_v2 = 125;
                break;
            case 4u:
                field_8E8_v1 = 53;
                field_8E9_v2 = 58;
                break;
            case 5u:
                goto LABEL_7;
            case 6u:
                field_8E8_v1 = 28;
                field_8E9_v2 = 24;
                break;
            case 7u:
                goto LABEL_9;
            case 8u:
                goto LABEL_10;
            default:
                return;
        }
    }
    else
    {
        switch (a2)
        {
            case 0u:
            case 1u:
            case 2u:
                field_8E8_v1 = -23;
                field_8E9_v2 = -17;
                break;
            case 3u:
            case 0xDu:
            LABEL_3:
                field_8E8_v1 = -119;
                field_8E9_v2 = -115;
                break;
            case 4u:
            LABEL_7:
                field_8E8_v1 = 39;
                field_8E9_v2 = 44;
                break;
            case 5u:
            LABEL_10:
                field_8E8_v1 = -4;
                field_8E9_v2 = -6;
                break;
            case 8u:
                field_8E8_v1 = -8;
                field_8E9_v2 = -11;
                break;
            case 0xAu:
                field_8E8_v1 = 39;
                field_8E9_v2 = 35;
                break;
            case 0xEu:
            LABEL_15:
                field_8E8_v1 = 105;
                field_8E9_v2 = 109;
                break;
            case 0xFu:
            LABEL_9:
                field_8E8_v1 = 72;
                field_8E9_v2 = 76;
                break;
            default:
                return;
        }
    }
}

STUB_FUNC(0x4D2B40)
void magical_germain_0x8EC::sub_4D2B40()
{
    // todo
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