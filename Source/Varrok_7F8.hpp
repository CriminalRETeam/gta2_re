#pragma once

#include "Function.hpp"

class Varrok_8
{
public:
    s32 field_0;
    char_type field_4;
    char_type field_5;
    char_type field_6;
    char_type field_7;
};

class Varrok_7F8
{
public:
    EXPORT u8 sub_59B060(s32 a2);
    EXPORT char_type* sub_59B0B0(u8 a2);
    EXPORT char_type sub_59B0D0(u8 idx);
    EXPORT Varrok_7F8* ctor_59B0F0();
    EXPORT void dtor_59B110();
    Varrok_8 field_0[255];
};
