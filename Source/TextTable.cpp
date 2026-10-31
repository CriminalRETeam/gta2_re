#include "TextTable.hpp"
#include "Function.hpp"
#include "chunk.hpp"
#include "error.hpp"
#include "file.hpp"
#include "registry.hpp"
#include "Globals.hpp"
#include "gtx_0x106C.hpp"
#include "enums.hpp"

DEFINE_GLOBAL(TextTable*, gText_0x14, 0x704DFC);
DEFINE_GLOBAL(TextTable*, gtext_0x14, 0x6F87F0);
DEFINE_GLOBAL_ARRAY(wchar_t, tmpAscii2WideStr, 640, 0x70488C);

const wchar_t kToUpperTable_626590[256] = {
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

const s16 kExtendedRussianCharMap_626490[128] = {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   177, 0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   210, 0,   0,   0,   0,   0,   0,   0,   193, 208,
                        192, 184, 198, 182, 199, 187, 205, 178, 181, 197, 204, 183, 196, 194, 195, 203, 206, 180, 190, 188, 179, 202,
                        185, 186, 189, 191, 207, 200, 209, 201, 226, 241, 225, 217, 231, 215, 232, 220, 238, 211, 214, 230, 237, 216,
                        229, 227, 228, 236, 239, 213, 223, 221, 212, 235, 218, 219, 222, 224, 240, 233, 242, 234};

// Extended characters 128-255, corresponding to original data at 0x626390.
static const u16 kExtendedWesternCharMap[128] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   176, 0,   0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   175, 128, 129, 130, 0, 131, 0,   132, 133, 134, 135, 136, 137, 138, 139,
    140, 141, 0,   173, 142, 143, 144, 0,   145, 0,   0,   146, 147, 148, 149, 0, 0,   150, 151, 152, 153, 0,   154, 0,   155, 156,
    157, 158, 159, 160, 161, 162, 163, 164, 0,   174, 165, 166, 167, 0,   168, 0, 0,   169, 170, 171, 172, 0,   0,   0};

MATCH_FUNC(0x5B58D0)
u16 TextTable::RemapExtendedChar(u16 ch) const
{
    u16 result = ch;
    if (ch >= 128u)
    {
        if (ch >= 256u)
        {
            result = '?';
        }
        else
        {
            result = mLangCode == 'r' ? kExtendedRussianCharMap_626490[ch - 128] : kExtendedWesternCharMap[ch - 128];
            if (!result)
            {
                result = '?';
            }
        }
    }
    return result;
}

MATCH_FUNC(0x5B5910)
void TextTable::RemapExtendedCharsIfNotJapanese(wchar_t* pText)
{
    if (!LangIsJapanese())
    {
        RemapExtendedChars(pText);
    }
}

MATCH_FUNC(0x5B5930)
void TextTable::RemapExtendedChars(wchar_t* pStr)
{
    for (wchar_t* pStrIter = pStr; *pStrIter; ++pStrIter)
    {
        *pStrIter = RemapExtendedChar(*pStrIter);
    }
}

MATCH_FUNC(0x5B5960)
void TextTable::TKEY_Load(u32 chunk_size)
{
    mTKey.mTKeyCount = chunk_size / sizeof(TextKeyEntry);
    mTKey.mTKey = new TextKeyEntry[mTKey.mTKeyCount];
    if (!mTKey.mTKey)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\text.cpp", 218);
    }

    File::Global_Read(mTKey.mTKey, chunk_size);
}

MATCH_FUNC(0x5B59B0)
void TextTable::TDAT_Offsets2Strings(u8* pDat)
{
    TextKeyEntry* ptKeyIter = mTKey.mTKey;

    for (u32 idx = 0; idx < static_cast<u32>(mTKey.mTKeyCount); idx++)
    {
        // The raw file record is an offset, convert it to a pointer to the string data in the TDAT
        const s32 offsetInBytes = reinterpret_cast<s32>(ptKeyIter->mPwstr);
        ptKeyIter->mPwstr = reinterpret_cast<wchar_t*>(&pDat[offsetInBytes]);
        ptKeyIter++;
    }
}

MATCH_FUNC(0x5B59E0)
bool TextTable::TKeyExists(const char_type* pIdStr) const
{
    return bsearch(pIdStr, // search key
                   mTKey.mTKey, // array to search
                   mTKey.mTKeyCount, // array count
                   sizeof(TextKeyEntry), // element size
                   TKeySearch) ?
        true :
        false;
}

MATCH_FUNC(0x5B5A10)
s32 __cdecl TextTable::TKeySearch(const void* a, const void* b)
{
    const char_type* pIdStr = static_cast<const char_type*>(a);
    const TextKeyEntry* pEntry = static_cast<const TextKeyEntry*>(b);
    return strcmp(pIdStr, reinterpret_cast<const char_type*>(&pEntry->mKey));
}

MATCH_FUNC(0x5B5A50)
wchar_t* TextTable::TKeyFind(const char_type* pIdStr) const
{
    TextKeyEntry* pFound = static_cast<TextKeyEntry*>(bsearch(pIdStr, // key
                                          mTKey.mTKey, // array
                                          mTKey.mTKeyCount, // array count
                                          sizeof(TextKeyEntry), // element size
                                          TKeySearch));
    if (!pFound)
    {
        return L" ";
    }
    else
    {
        return pFound->mPwstr;
    }
}

MATCH_FUNC(0x5B5A80)
void text_tdat::TDAT_Load(u32 size)
{
    mLen = size / sizeof(wchar_t);
    mData = new u8[mLen * sizeof(wchar_t)];

    if (!mData)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\text.cpp", 378);
    }

    File::Global_Read(mData, size);
}

MATCH_FUNC(0x5B5AD0)
u16 TextTable::ToUpper(wchar_t ch) const
{
    // The original uppercase data uses kToUpperTable_626590[0..127] for 'g'/'i'/'r'/'s'
    // and kToUpperTable_626590[128..255] for French. Index both halves explicitly.
    switch (mLangCode)
    {
        case 'e':
            if (ch >= 'a' && ch <= 'z')
            {
                return ch - ' ';
            }
            break;

        case 'f':
            if (ch >= 'a' && ch <= 'z')
            {
                return ch - ' ';
            }
            if (ch >= 0x80 && ch <= 0xFF)
            {
                return kToUpperTable_626590[ch];
            }
            break;

        case 'g':
        case 'i':
        case 'r':
        case 's':
            if (ch >= 'a' && ch <= 'z')
            {
                return ch - ' ';
            }
            if (ch >= 0x80 && ch <= 0xFF)
            {
                return kToUpperTable_626590[ch - 128];
            }
            break;
    }
    return ch;
}

MATCH_FUNC(0x5B5B80)
wchar_t* TextTable::StrToUpper(wchar_t* pWideStr)
{
    for (wchar_t* pStrIter = pWideStr; *pStrIter; ++pStrIter)
    {
        *pStrIter = ToUpper(*pStrIter);
    }
    return pWideStr;
}

// 9.6f 0x4539D0, file-local copy of GetCharWidthFromFontType (Draw.hpp): including Draw.hpp changes this file's static init code
static inline s32 __stdcall GetCharWidth_local_4539D0(u16 font_type, wchar_t chr)
{
    return static_cast<u16>(gGtx_0x106C->GetFontWidth(&font_type, &chr));
}

// https://decomp.me/scratch/JzIDq
MATCH_FUNC(0x5B5BC0)
s32 __stdcall TextTable::InsertLineBreaksAndGetNumLines(wchar_t* pDestStr, const wchar_t* pSrcStr, s32 max_line_width, u16 font_type)
{
    const wchar_t* pSrc = pSrcStr;
    wchar_t* pDest = pDestStr;
    s32 num_of_lines = 1;
    s32 space_width = GetSpaceWidth(font_type);
    s32 current_width = 0;
    const wchar_t* pPrevSrcCheckpoint = NULL;
    wchar_t* pPrevDestCheckpoint = NULL;

    for (; *pSrc; pSrc++, pDest++)
    {
        *pDest = *pSrc;

        switch (*pSrc)
        {
            case '\n':
                pPrevSrcCheckpoint = NULL;
                pPrevDestCheckpoint = NULL;
                current_width = 0;
                num_of_lines += 1;
                break;
            case ' ':
                current_width += space_width;
                pPrevSrcCheckpoint = pSrc;
                pPrevDestCheckpoint = pDest;
                break;
            case '#':
                break;
            default:
                current_width += GetCharWidth_local_4539D0(font_type, *pSrc);
                break;
        }

        if (current_width > max_line_width)
        {
            if (pPrevSrcCheckpoint && pPrevDestCheckpoint)
            {
                current_width = 0;
                pDest = pPrevDestCheckpoint; // retrocede
                pSrc = pPrevSrcCheckpoint;
                *pDest = '\n';

                pPrevSrcCheckpoint = NULL;
                pPrevDestCheckpoint = NULL;
            }
            else
            {
                *pDest = '\n';
                pDest++;

                current_width = GetCharWidth_local_4539D0(font_type, *pSrc);
                *pDest = *pSrc;
            }
            num_of_lines += 1;
        }
    }
    *pDest = 0; // terminate string

    return num_of_lines;
}

MATCH_FUNC(0x5B5D10)
char_type* TextTable::Wide2PesudoAscii(const wchar_t* pText)
{
    LOCAL_STATIC_ARRAY(char_type, sTmpWide2AsciiStr, 80, 0x70462C);
    const wchar_t* pSrc = pText;
    // Signed index and limit: the original compares them with jge
    s32 dstIdx = 0;
    while (pSrc) // bug: should be *pSrc
    {
        if (dstIdx >= static_cast<s32>(sizeof(sTmpWide2AsciiStr) - 1))
        {
            break;
        }

        if (*pSrc >= 0x80u)
        {
            sTmpWide2AsciiStr[dstIdx] = '#';
        }
        else
        {
            sTmpWide2AsciiStr[dstIdx] = static_cast<char_type>(*pSrc);
        }

        pSrc++;
        dstIdx++;
    }

    // add null terminator
    sTmpWide2AsciiStr[dstIdx] = 0;
    return sTmpWide2AsciiStr;
}

MATCH_FUNC(0x5B5DF0)
wchar_t* __stdcall TextTable::Ascii2Wide(const char_type* pStr)
{
    // char_type is signed, so the widening sign-extends (movsx) as in the original
    wchar_t* pDst = tmpAscii2WideStr;
    while (*pStr)
    {
        *pDst = *pStr;
        pDst++;
        pStr++;
    }

    // null terminate
    *pDst = 0;
    return tmpAscii2WideStr;
}

MATCH_FUNC(0x5B5E20)
void TextTable::load_chunk(const char_type* chunk_type, u32 chunk_len)
{
    if (!strncmp(chunk_type, "TKEY", 4u))
    {
        TKEY_Load(chunk_len);
    }
    else if (!strncmp(chunk_type, "TDAT", 4u))
    {
        mTDat.TDAT_Load(chunk_len);
    }
    else
    {
        File::Global_Seek(&chunk_len);
    }
}

MATCH_FUNC(0x5B5E90)
void TextTable::Load()
{
    char_type expected_code[5];

    chunk_header chunkHeader;
    char_type gxtFileName[20];
    sprintf(gxtFileName, "data\\%c.gxt", mLangCode);

    sprintf(expected_code, "GBL%c", mLangCode - ' ');

    File::Global_Open(gxtFileName);

    file_header gxtHeader;
    u32 len = sizeof(file_header);
    File::Global_Read(&gxtHeader, len);

    gxtHeader.verify_type(expected_code);
    gxtHeader.verify_version(100);

    for (len = sizeof(chunk_header); File::Global_TryRead(&chunkHeader, &len); len = sizeof(chunk_header))
    {
        if (chunkHeader.mSize != 0)
        {
            load_chunk(chunkHeader.mType, chunkHeader.mSize);
        }
    }
    File::Global_Close();
    TDAT_Offsets2Strings(mTDat.mData);
}

MATCH_FUNC(0x5B5F90)
wchar_t* TextTable::Find(const char_type* pIdStr) const
{
    return TKeyFind(pIdStr);
}

MATCH_FUNC(0x5B5FA0)
bool TextTable::KeyExists(const char_type* pIdStr) const
{
    return TKeyExists(pIdStr);
}

MATCH_FUNC(0x5B5FB0)
TextTable::TextTable()
{
    gRegistry.Get_Option("language", reinterpret_cast<BYTE*>(gTmpBuffer), sizeof(gTmpBuffer));

    if (gTmpBuffer[0] == 'e' || gTmpBuffer[0] == 'f' || gTmpBuffer[0] == 'g' || gTmpBuffer[0] == 'i' ||
        gTmpBuffer[0] == 's' || gTmpBuffer[0] == 'j' || gTmpBuffer[0] == 'r')
    {
        mLangCode = gTmpBuffer[0];
    }
    else
    {
        mLangCode = 'e';
    }
}

// Only gets its EH frame when <new> isn't included, see Globals.hpp
MATCH_FUNC(0x5B6050)
TextTable::~TextTable()
{
}