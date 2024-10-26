#include "Function.hpp"
#include "magical_germain_0x8EC.hpp"
#include "error.hpp"
#include "file.hpp"
#include "chunk.hpp"
#include "gtx_0x106C.hpp"

magical_germain_0x8EC *gMagical_germain_0x8EC_6F5168;

MATCH_FUNC(0x4D1FC0)
void magical_germain_0x8EC::LoadChunks_4D1FC0(const char *pChunkId, unsigned int chunk_len)
{
    if (!strncmp(pChunkId, "KIDX", 4u))
    {
        field_8CC_kidx_size_words = chunk_len >> 1;
        field_8C4_pKidX = new WORD[field_8CC_kidx_size_words];
        if (!field_8C4_pKidX)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\kanji.cpp", 142);
        }
        File::Global_Read_4A71C0(this->field_8C4_pKidX, &chunk_len);
    }
    else if (!strncmp(pChunkId, "KBIT", 4u))
    {
        field_8C8_pKBIT = new BYTE[chunk_len];
        if (!field_8C8_pKBIT)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\kanji.cpp", 148);
        }
        File::Global_Read_4A71C0(field_8C8_pKBIT, &chunk_len);
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

    kanji_header header;
    unsigned int readSize = sizeof(kanji_header);
    File::Global_Read_4A71C0(&header, &readSize);
    if (strncmp("KANJ", header.field_0_id, sizeof(header.field_0_id)))
    {
        FatalError_4A38C0(93, "C:\\Splitting\\Gta2\\Source\\chunk.h", 37);
    }

    if (header.field_4_version != 100)
    {
        FatalError_4A38C0(94, "C:\\Splitting\\Gta2\\Source\\chunk.h", 33);
    }

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

// nomatch 0x4D2C80
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

// stub
void magical_germain_0x8EC::sub_4D2B40()
{
    // todo
}

// stub
int magical_germain_0x8EC::sub_4D29D0(unsigned __int16 a2)
{
    return 0;
}

// stub
int magical_germain_0x8EC::sub_4D28A0(unsigned __int16 a2)
{
    return 0;
}

// stub
STexture * magical_germain_0x8EC::sub_4D27D0(unsigned int *a2)
{
    return 0;
}

// nomatch
STexture * magical_germain_0x8EC::sub_4D2710(wchar_t text_char)
{
    kanji_0x10 * pFound;
    unsigned int nearestId = -1;

    int i = 0;
    while (i < 120)
    {
        if (field_0[i].field_2_text_char == text_char &&
            field_0[i].field_0_v1 == field_8E8_v1 &&
            field_0[i].field_1_v2 == field_8E9_v2)
        {
            field_0[i].field_C_id = field_8C0_count++;
            return field_0[i].field_4_pTexture;
        }

        if (field_0[i].field_C_id < nearestId)
        {
            nearestId = field_0[i].field_C_id;
            pFound = &field_0[i];
        }
        i++;
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

// stub
void magical_germain_0x8EC::sub_4D2610(wchar_t text_char)
{

}
