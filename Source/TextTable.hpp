#pragma once

#include "Function.hpp"
#include <windows.h>

struct TextKeyEntry
{
    wchar_t* field_0_pwstr;
    // The TKEY record's 8 char key is stored inline in field_4_key and field_8_key_tail, not through a pointer
    const char_type* field_4_key;
    s32 field_8_key_tail;
};
GTA2_ASSERT_SIZEOF_ALWAYS(TextKeyEntry, 0xC);

class text_tdat
{
  public:
    text_tdat() // inline 0x4C2420
    {
        field_0_data = NULL;
        field_4_len = 0;
    }

    ~text_tdat() // inline 0x4C2430
    {
        if (field_0_data)
        {
            delete[] field_0_data;
        }
        field_0_data = NULL;
    }
    u8* field_0_data;
    s32 field_4_len;
    EXPORT void TDAT_Load_5B5A80(u32 size);
};

class text_tkey
{
public:
    text_tkey() // inline 4C23F0
    {
        field_0_tKey = NULL;
        field_4_tKey_count = 0;
    }

    ~text_tkey() // inline 0x4C2400
    {
        if (field_0_tKey)
        {
            delete[] field_0_tKey;
        }
        field_0_tKey = NULL;
    }
    TextKeyEntry* field_0_tKey;
    s32 field_4_tKey_count;
};


class TextTable
{
  public:
    // 9.6f 0x452E60
    inline bool LangIsJapanese_452E60() const
    {
        return field_10_lang_code == 'j';
    }

    text_tkey field_0_tKey;
    text_tdat field_8_tDat;
    char_type field_10_lang_code;

    EXPORT u16 RemapExtendedChar_5B58D0(u16 ch) const;

    EXPORT void RemapExtendedCharsIfNotJapanese_5B5910(wchar_t* pText);

    EXPORT void RemapExtendedChars_5B5930(wchar_t* pStr);

    EXPORT void TKEY_Load_5B5960(u32 chunk_size);

    EXPORT void TDAT_Offsets2Strings_5B59B0(u8* pDat);

    EXPORT bool TKeyFind_5B59E0(const char_type* pIdStr) const;

    EXPORT static s32 __cdecl TKeySearch_5B5A10(const void* a, const void* b);

    EXPORT wchar_t* TKeyFind_5B5A50(const char_type* pIdStr) const;

    EXPORT u16 ToUpper_5B5AD0(wchar_t ch) const;

    EXPORT wchar_t* StrToUpper_5B5B80(wchar_t* pWideStr);

    EXPORT static s32 __stdcall InsertLineBreaksAndGetNumLines_5B5BC0(wchar_t* pDestStr, const wchar_t* pSrcStr, s32 max_line_width, u16 font_type);

    EXPORT static char_type* __stdcall Wide2PesudoAscii_5B5D10(const wchar_t* pText);

    EXPORT static wchar_t* __stdcall Ascii2Wide_5B5DF0(const char_type* pStr);

    EXPORT void load_chunk_5B5E20(const char_type* chunk_type, u32 chunk_len);

    EXPORT void Load_5B5E90();

    EXPORT wchar_t* Find_5B5F90(const char_type* pIdStr) const;

    EXPORT bool KeyExists_5B5FA0(const char_type* pIdStr) const;

    // 0x5B5FB0
    EXPORT TextTable();

    // 0x5B6050
    EXPORT ~TextTable();
};
GTA2_ASSERT_SIZEOF_ALWAYS(TextTable, 0x14);

EXTERN_GLOBAL(TextTable*, gText_0x14_704DFC);

EXTERN_GLOBAL(TextTable*, gtext_0x14_6F87F0);
