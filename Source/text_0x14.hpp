#pragma once

#include "Function.hpp"
#include <windows.h>

struct text_0xC
{
    wchar_t* field_0_pwstr;
    const char_type* field_4_pKey;
    s32 field_8;
};

class text_tdat
{
  public:
    text_tdat() // inline 0x4C2420
    {
        field_0_data = 0;
        field_4_len = 0;
    }

    ~text_tdat() // inline 0x4C2430
    {
        if (field_0_data)
        {
            delete[] field_0_data;
        }
        field_0_data = 0;
    }
    BYTE* field_0_data;
    s32 field_4_len;
    EXPORT void TDAT_Load_5B5A80(u32 size);
};

class text_tkey
{
public:
    text_tkey() // inline 4C23F0
    {
        field_0_tKey = 0;
        field_4_tKey_count = 0;
    }

    ~text_tkey() // inline 0x4C2400
    {
        if (field_0_tKey)
        {
            delete[] field_0_tKey;
        }
        field_0_tKey = 0;
    }
    text_0xC* field_0_tKey;
    s32 field_4_tKey_count;
};


class text_0x14
{
  public:
    text_tkey field_0_tKey;
    text_tdat field_8_tDat;
    char_type field_10_lang_code;
    char_type field_11_pad[3];

    EXPORT u16 sub_5B58D0(u16 a2);

    EXPORT void sub_5B5910(wchar_t* a2);

    EXPORT void sub_5B5930(wchar_t* a1);

    EXPORT void TKEY_Load_5B5960(u32 size);

    EXPORT void TDAT_Offsets2Strings_5B59B0(BYTE* pDat);

    EXPORT bool TKeyFind_5B59E0(const char_type* pIdStr);

    EXPORT static s32 __cdecl TKeySearch_5B5A10(const void* a, const void* b);

    EXPORT wchar_t* TKeyFind_5B5A50(const char_type* pIdStr);

    EXPORT u16 sub_5B5AD0(wchar_t a2);

    EXPORT wchar_t* sub_5B5B80(wchar_t* pWideStr);

    EXPORT s32 __stdcall sub_5B5BC0(u16* a1, s32 a2, s32 a3, s32 a4);

    EXPORT static char_type* __stdcall Wide2PesudoAscii_5B5D10(const wchar_t* a1);

    EXPORT wchar_t* Ascii2Wide_5B5DF0(char_type* a1);

    EXPORT void load_chunk_5B5E20(const char_type* chunk_type, u32 chunk_len);

    EXPORT void Load_5B5E90();

    EXPORT wchar_t* Find_5B5F90(const char_type* pIdStr);

    EXPORT bool sub_5B5FA0(const char_type* pIdStr);

    // 0x5B5FB0
    EXPORT text_0x14();

    // 0x5B6050
    EXPORT ~text_0x14();
};

EXTERN_GLOBAL(text_0x14*, gText_0x14_704DFC);

EXTERN_GLOBAL(text_0x14*, gtext_0x14_6F87F0);
