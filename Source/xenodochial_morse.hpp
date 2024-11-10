#pragma once

#include "types.hpp"
#include "sleepy_stonebraker_0x6C.hpp"

#pragma pack(push)
#pragma pack(1)
class xenodochial_morse
{
public:

    EXPORT xenodochial_morse(); // 483E70
    EXPORT ~xenodochial_morse(); // 483EA0

    EXPORT s16 sub_483EC0(s32 a2, s32 a3, s32* a4, char_type a5);
    EXPORT void sub_483F20();

    s16 field_0;
    sleepy_stonebraker_0x6C field_2[600];
};
#pragma pack(pop)
