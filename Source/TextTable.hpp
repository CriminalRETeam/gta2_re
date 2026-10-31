#pragma once

#include "Function.hpp"
#include <windows.h>

struct TextKeyEntry
{
    wchar_t* mPwstr; // 0x0
    // The TKEY record's 8 char key is stored inline in mKey and mKeyTail, not through a pointer
    const char_type* mKey; // 0x4
    s32 mKeyTail; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(TextKeyEntry, 0xC);

class text_tdat
{
  public:
    text_tdat() // inline 0x4C2420
    {
        mData = NULL;
        mLen = 0;
    }

    ~text_tdat() // inline 0x4C2430
    {
        if (mData)
        {
            delete[] mData;
        }
        mData = NULL;
    }
    u8* mData; // 0x0
    s32 mLen; // 0x4
    EXPORT void TDAT_Load(u32 size);
};

class text_tkey
{
public:
    text_tkey() // inline 4C23F0
    {
        mTKey = NULL;
        mTKeyCount = 0;
    }

    ~text_tkey() // inline 0x4C2400
    {
        if (mTKey)
        {
            delete[] mTKey;
        }
        mTKey = NULL;
    }
    TextKeyEntry* mTKey; // 0x0
    s32 mTKeyCount; // 0x4
};


class TextTable
{
  public:
    // 9.6f 0x452E60
    inline bool LangIsJapanese() const
    {
        return mLangCode == 'j';
    }

    text_tkey mTKey; // 0x0
    text_tdat mTDat; // 0x8
    char_type mLangCode; // 0x10

    EXPORT u16 RemapExtendedChar(u16 ch) const;

    EXPORT void RemapExtendedCharsIfNotJapanese(wchar_t* pText);

    EXPORT void RemapExtendedChars(wchar_t* pStr);

    EXPORT void TKEY_Load(u32 chunk_size);

    EXPORT void TDAT_Offsets2Strings(u8* pDat);

    EXPORT bool TKeyFind_5B59E0(const char_type* pIdStr) const;

    EXPORT static s32 __cdecl TKeySearch(const void* a, const void* b);

    EXPORT wchar_t* TKeyFind_5B5A50(const char_type* pIdStr) const;

    EXPORT u16 ToUpper(wchar_t ch) const;

    EXPORT wchar_t* StrToUpper(wchar_t* pWideStr);

    EXPORT static s32 __stdcall InsertLineBreaksAndGetNumLines(wchar_t* pDestStr, const wchar_t* pSrcStr, s32 max_line_width, u16 font_type);

    EXPORT static char_type* __stdcall Wide2PesudoAscii(const wchar_t* pText);

    EXPORT static wchar_t* __stdcall Ascii2Wide(const char_type* pStr);

    EXPORT void load_chunk(const char_type* chunk_type, u32 chunk_len);

    EXPORT void Load();

    EXPORT wchar_t* Find(const char_type* pIdStr) const;

    EXPORT bool KeyExists(const char_type* pIdStr) const;

    // 0x5B5FB0
    EXPORT TextTable();

    // 0x5B6050
    EXPORT ~TextTable();
};
GTA2_ASSERT_SIZEOF_ALWAYS(TextTable, 0x14);

EXTERN_GLOBAL(TextTable*, gText_0x14);

EXTERN_GLOBAL(TextTable*, gtext_0x14);
