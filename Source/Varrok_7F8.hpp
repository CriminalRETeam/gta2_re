#pragma once

#include "Function.hpp"

class Varrok_8
{
  public:
    s32 field_0_ped_id;
    u8 field_4;
    char_type field_5;
    char_type field_6;
    char_type field_7;
};

class Varrok_7F8
{
  public:
    EXPORT u8 sub_59B060(s32 a2);
    EXPORT void IncrementRefCount_59B0B0(u8 a2);
    EXPORT void sub_59B0D0(u8 idx);
    EXPORT Varrok_7F8();
    EXPORT ~Varrok_7F8();
    Varrok_8 field_0[255];

    inline void Clear_434070(u8 idx)
    {
        field_0[idx].field_0_ped_id = 0;
        field_0[idx].field_4 = 0;
    }

    s32 GetPedId_420F10(u8 idx)
    {
        return field_0[idx].field_0_ped_id;
    }
};

EXTERN_GLOBAL(Varrok_7F8*, gVarrok_7F8_703398);
