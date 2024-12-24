#pragma once

#include "Function.hpp"
#include <windows.h>

struct STexture;
struct sprite_index;

struct kanji_0x10
{
    char_type field_0_v1;
    char_type field_1_v2;
    s16 field_2_text_char;
    STexture* field_4_pTexture;
    sprite_index* field_8_sprt_index;
    u32 field_C_id;
};

/*
struct kanji_0x2
{
    char_type field_0;
    char_type field_1;
};

struct kanji_0x20
{
    kanji_0x2 field_0[16];
};
*/

class magical_germain_0x8EC
{
  public:
    kanji_0x10 field_0[120];
    kanji_0x10 field_780[20];
    s32 field_8C0_count;
    u16* field_8C4_pKidX;
    BYTE* field_8C8_pKBIT;
    s32 field_8CC_kidx_size_words;
    BYTE* field_8D0_pSprtData;
    sprite_index* field_8D4_sprite_index;
    STexture* field_8D8_pTexture;
    BYTE* field_8DC_pSprtData;
    sprite_index* field_8E0_sprite_index;
    STexture* field_8E4_pTexture;
    u8 field_8E8_v1;
    char_type field_8E9_v2;
    s16 field_8EA_pad;

    EXPORT void LoadChunks_4D1FC0(const char_type* pChunkId, u32 chunk_len);
    EXPORT void Load_kanji_dat_4D2090();
    EXPORT void sub_4D2150(s32 a2, u16 a3, u16 a4);
    EXPORT u8* sub_4D2240(char_type* a2);
    EXPORT u8* sub_4D23B0(char_type* a2);
    EXPORT void sub_4D2610(wchar_t text_char);
    EXPORT s32 sub_4D2690(u16 a2);
    EXPORT STexture* sub_4D2710(wchar_t text_char);
    EXPORT STexture* sub_4D27D0(u32* a2);
    EXPORT s32 sub_4D28A0(u16 a2);
    EXPORT s32 sub_4D29D0(u16 a2);
    EXPORT void sub_4D2B40();
    EXPORT magical_germain_0x8EC();
    EXPORT ~magical_germain_0x8EC();
};

EXPORT_VAR extern magical_germain_0x8EC* gMagical_germain_0x8EC_6F5168;
