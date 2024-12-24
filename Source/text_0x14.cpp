#include "text_0x14.hpp"
#include "Function.hpp"
#include "chunk.hpp"
#include "error.hpp"
#include "file.hpp"
#include "registry.hpp"
#include "Globals.hpp"

EXPORT_VAR text_0x14* gText_0x14_704DFC;
GLOBAL(gText_0x14_704DFC, 0x704DFC);

EXPORT_VAR text_0x14* gtext_0x14_6F87F0;
GLOBAL(gtext_0x14_6F87F0, 0x6F87F0);

EXPORT_VAR wchar_t tmpAscii2WideStr_70488C[640];
GLOBAL(tmpAscii2WideStr_70488C, 0x70488C);

EXPORT_VAR char_type tmpWide2AsciiStr_70462C[80];
GLOBAL(tmpWide2AsciiStr_70462C, 0x70462C);

const wchar_t word_626590[256] = {
    128u, 129u, 130u, 131u, 132u, 133u, 134u, 135u, 136u, 137u, 138u, 139u, 140u, 141u, 142u, 143u, 144u, 145u, 146u, 147u, 148u, 149u,
    150u, 128u, 129u, 130u, 131u, 132u, 133u, 134u, 135u, 136u, 137u, 138u, 139u, 140u, 141u, 142u, 143u, 144u, 145u, 146u, 147u, 148u,
    149u, 173u, 173u, 175u, 176u, 177u, 178u, 179u, 180u, 181u, 182u, 183u, 184u, 185u, 186u, 187u, 188u, 189u, 190u, 191u, 192u, 193u,
    194u, 195u, 196u, 197u, 198u, 199u, 200u, 201u, 202u, 203u, 204u, 205u, 206u, 207u, 208u, 209u, 177u, 178u, 179u, 180u, 181u, 182u,
    183u, 184u, 185u, 186u, 187u, 188u, 189u, 190u, 191u, 192u, 193u, 194u, 195u, 196u, 197u, 198u, 199u, 200u, 201u, 202u, 203u, 204u,
    205u, 206u, 207u, 208u, 209u, 243u, 244u, 245u, 246u, 247u, 248u, 249u, 250u, 251u, 252u, 253u, 254u, 255u, 128u, 129u, 130u, 131u,
    132u, 133u, 134u, 135u, 136u, 137u, 138u, 139u, 140u, 141u, 142u, 143u, 144u, 145u, 146u, 147u, 148u, 149u, 150u, 65u,  65u,  65u,
    65u,  132u, 133u, 69u,  69u,  69u,  69u,  73u,  73u,  73u,  73u,  79u,  79u,  79u,  79u,  85u,  85u,  85u,  85u,  173u, 173u, 175u,
    176u, 177u, 178u, 179u, 180u, 181u, 182u, 183u, 184u, 185u, 186u, 187u, 188u, 189u, 190u, 191u, 192u, 193u, 194u, 195u, 196u, 197u,
    198u, 199u, 200u, 201u, 202u, 203u, 204u, 205u, 206u, 207u, 208u, 209u, 210u, 211u, 212u, 213u, 214u, 215u, 216u, 217u, 218u, 219u,
    220u, 221u, 222u, 223u, 224u, 225u, 226u, 227u, 228u, 229u, 230u, 231u, 232u, 233u, 234u, 235u, 236u, 237u, 238u, 239u, 240u, 241u,
    242u, 243u, 244u, 245u, 246u, 247u, 248u, 249u, 250u, 251u, 252u, 253u, 254u, 255u};

const s16 word_626490[128] = {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   177, 0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   210, 0,   0,   0,   0,   0,   0,   0,   193, 208,
                        192, 184, 198, 182, 199, 187, 205, 178, 181, 197, 204, 183, 196, 194, 195, 203, 206, 180, 190, 188, 179, 202,
                        185, 186, 189, 191, 207, 200, 209, 201, 226, 241, 225, 217, 231, 215, 232, 220, 238, 211, 214, 230, 237, 216,
                        229, 227, 228, 236, 239, 213, 223, 221, 212, 235, 218, 219, 222, 224, 240, 233, 242, 234};

STUB_FUNC(0x5B58D0)
u16 text_0x14::sub_5B58D0(u16 a2)
{
    // todo
    /*
    u16 result; // ax

    result = a2;
    if (a2 >= 128u)
    {
        if (a2 >= 256u)
        {
            return 63;
        }

        result = this->field_10_lang_code == 'r' ? word_626390[a2] : *(u16 *)&aCSplittingGta2_27[2 * a2 + 24];
        if (!result)
        {
            return 63;
        }
    }
    return result;
    */
    return 0;
}

MATCH_FUNC(0x5B5910)
void text_0x14::sub_5B5910(u16* a2)
{
    if (field_10_lang_code != 'j')
    {
        sub_5B5930(a2);
    }
}

MATCH_FUNC(0x5B5930)
void text_0x14::sub_5B5930(u16* pStr)
{
    for (u16* pStrIter = pStr; *pStrIter; ++pStrIter)
    {
        *pStrIter = sub_5B58D0(*pStrIter);
    }
}

MATCH_FUNC(0x5B5960)
void text_0x14::TKEY_Load_5B5960(u32 chunkSize)
{
    // TODO: probably an inline
    field_0_tKey.field_4_tKey_count = chunkSize / sizeof(text_0xC);
    field_0_tKey.field_0_tKey = new text_0xC[field_0_tKey.field_4_tKey_count];
    if (!field_0_tKey.field_0_tKey)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\text.cpp", 218);
    }

    File::Global_Read_4A71C0(field_0_tKey.field_0_tKey, &chunkSize);
}

MATCH_FUNC(0x5B59B0)
void text_0x14::TDAT_Offsets2Strings_5B59B0(BYTE* pDat)
{
    text_0xC* ptKeyIter = field_0_tKey.field_0_tKey;

    for (u32 idx = 0; idx < field_0_tKey.field_4_tKey_count; idx++)
    {
        // The raw file record is an offset, convert it to pos32 to the string data in the TDAT
        const s32 offsetInBytes = reinterpret_cast<int>(ptKeyIter->field_0_pwstr);
        ptKeyIter->field_0_pwstr = reinterpret_cast<wchar_t*>(&pDat[offsetInBytes]);
        ptKeyIter++;
    }
}

MATCH_FUNC(0x5B59E0)
bool text_0x14::TKeyFind_5B59E0(const char_type* pIdStr)
{
    return bsearch(pIdStr, // search key
                   field_0_tKey.field_0_tKey, // array to search
                   field_0_tKey.field_4_tKey_count, // array count
                   sizeof(text_0xC), // element size
                   TKeySearch_5B5A10) ?
        true :
        false;
}

MATCH_FUNC(0x5B5A10)
s32 __cdecl text_0x14::TKeySearch_5B5A10(const void* a, const void* b)
{
    const char_type* pIdStr = (const char_type*)a;
    const text_0xC* a2 = (const text_0xC*)b;
    return strcmp(pIdStr, (const char_type*)&a2->field_4_pKey); // TODO: ???
}

MATCH_FUNC(0x5B5A50)
wchar_t* text_0x14::TKeyFind_5B5A50(const char_type* pIdStr)
{
    text_0xC* pFound = (text_0xC*)bsearch(pIdStr, // key
                                          field_0_tKey.field_0_tKey, // array
                                          field_0_tKey.field_4_tKey_count, // array count
                                          sizeof(text_0xC), // element size
                                          TKeySearch_5B5A10);
    if (!pFound)
    {
        return L" ";
    }
    else
    {
        return pFound->field_0_pwstr;
    }
}

MATCH_FUNC(0x5B5A80)
void text_tdat::TDAT_Load_5B5A80(u32 size)
{
    field_4_len = size / sizeof(wchar_t);
    field_0_data = new BYTE[field_4_len * sizeof(wchar_t)];

    if (!field_0_data)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\text.cpp", 378);
    }

    File::Global_Read_4A71C0(field_0_data, &size);
}

STUB_FUNC(0x5B5AD0)
u16 text_0x14::sub_5B5AD0(wchar_t a2)
{
    u16 result; // ax

    result = a2;
    switch (this->field_10_lang_code)
    {
        case 'e':
            if ((u32)a2 >= 'a' && (u32)a2 <= 'z')
            {
                result = a2 - ' ';
            }
            break;

        case 'f':
            if (a2 < 0x61u || a2 > 0x7Au)
            {
                if (a2 >= 0x80u && a2 <= 0xFFu)
                {
                    // todo: these arrays are very wrong
                    result = word_626590[a2];
                }
            }
            else
            {
                result = a2 - 32;
            }
            break;

        case 'g':
        case 'i':
        case 'r':
        case 's':
            if (a2 < 0x61u || a2 > 0x7Au)
            {
                if (a2 >= 0x80u && a2 <= 0xFFu)
                {
                    result = word_626490[a2];
                }
            }
            else
            {
                result = a2 - 32;
            }
            break;

        default:
            return result;
    }

    return result;
}

MATCH_FUNC(0x5B5B80)
wchar_t* text_0x14::sub_5B5B80(wchar_t* pWideStr)
{
    for (wchar_t* pStrIter = pWideStr; *pStrIter; ++pStrIter)
    {
        *pStrIter = sub_5B5AD0(*pStrIter);
    }
    return pWideStr;
}

STUB_FUNC(0x5B5BC0)
s32 __stdcall text_0x14::sub_5B5BC0(u16* a1, s32 a2, s32 a3, s32 a4)
{
    // todo
    return 0;
}

MATCH_FUNC(0x5B5D10)
char* text_0x14::Wide2PesudoAscii_5B5D10(u16* a1)
{
    u16* pSrc = a1;
    // jnb forced to jge by changing u32 loop index to int
    s32 dstIdx = 0;
    while (pSrc) // bug: should be *pSrc
    {
        // jnb also used if comparing to sizeof() value instead of
        // numerical literal! cast to s32 fixes (sizeof returns unsigned?)
        if (dstIdx >= static_cast<int>(sizeof(tmpWide2AsciiStr_70462C) - 1))
        {
            break;
        }

        if (*pSrc >= 0x80u)
        {
            tmpWide2AsciiStr_70462C[dstIdx] = '#';
        }
        else
        {
            tmpWide2AsciiStr_70462C[dstIdx] = static_cast<char>(*pSrc);
        }

        pSrc++;
        dstIdx++;
    }

    // add null terminator
    tmpWide2AsciiStr_70462C[dstIdx] = 0;
    return tmpWide2AsciiStr_70462C;
}

MATCH_FUNC(0x5B5DF0)
wchar_t* text_0x14::Ascii2Wide_5B5DF0(char_type* pStr)
{
    // movsx vs movzx caused by unsigned vs s8
    wchar_t* pDst = tmpAscii2WideStr_70488C;
    while (*pStr)
    {
        *pDst = *pStr;
        pDst++;
        pStr++;
    }

    // null terminate
    *pDst = 0;
    return tmpAscii2WideStr_70488C;
}

MATCH_FUNC(0x5B5E20)
void text_0x14::load_chunk_5B5E20(const char_type* chunk_type, u32 chunk_len)
{
    if (!strncmp(chunk_type, "TKEY", 4u))
    {
        TKEY_Load_5B5960(chunk_len);
    }
    else if (!strncmp(chunk_type, "TDAT", 4u))
    {
        field_8_tDat.TDAT_Load_5B5A80(chunk_len);
    }
    else
    {
        File::Global_Seek_4A7140(&chunk_len);
    }
}

MATCH_FUNC(0x5B5E90)
void text_0x14::Load_5B5E90()
{
    char_type expected_code[5];

    chunk_header chunkHeader;
    char_type gxtFileName[20];
    sprintf(gxtFileName, "data\\%c.gxt", this->field_10_lang_code);

    sprintf(expected_code, "GBL%c", this->field_10_lang_code - ' ');

    File::Global_Open_4A7060(gxtFileName);

    file_header gtxHeader;
    u32 len = sizeof(file_header);
    File::Global_Read_4A71C0(&gtxHeader, &len);

    gtxHeader.verify_type(expected_code);
    gtxHeader.verify_version(100);

    for (len = sizeof(chunk_header); File::Global_Read_4A7210(&chunkHeader, &len); len = sizeof(chunk_header))
    {
        if (chunkHeader.field_4_size != 0)
        {
            load_chunk_5B5E20(chunkHeader.field_0_type, chunkHeader.field_4_size);
        }
    }
    File::Global_Close_4A70C0();
    TDAT_Offsets2Strings_5B59B0(field_8_tDat.field_0_data);
}

MATCH_FUNC(0x5B5F90)
wchar_t* text_0x14::Find_5B5F90(const char_type* pIdStr)
{
    return TKeyFind_5B5A50(pIdStr);
}

MATCH_FUNC(0x5B5FA0)
bool text_0x14::sub_5B5FA0(const char_type* pIdStr)
{
    return TKeyFind_5B59E0(pIdStr);
}

MATCH_FUNC(0x5B5FB0)
text_0x14::text_0x14()
{
    gRegistry_6FF968.Get_Option_586F00("language", reinterpret_cast<BYTE*>(gTmpBuffer_67C598), sizeof(gTmpBuffer_67C598));

    if (gTmpBuffer_67C598[0] == 'e' || gTmpBuffer_67C598[0] == 'f' || gTmpBuffer_67C598[0] == 'g' || gTmpBuffer_67C598[0] == 'i' ||
        gTmpBuffer_67C598[0] == 's' || gTmpBuffer_67C598[0] == 'j' || gTmpBuffer_67C598[0] == 'r')
    {
        field_10_lang_code = gTmpBuffer_67C598[0];
    }
    else
    {
        field_10_lang_code = 'e';
    }
}

// https://decomp.me/scratch/ZNzsG
// TODO: This should match but doesn't, maybe a problem in the comparison scripts ??
// seems like the new func we dism is the wrong addr or something
STUB_FUNC(0x5B6050)
text_0x14::~text_0x14()
{

}
