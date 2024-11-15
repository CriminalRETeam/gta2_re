#pragma once

#include "Function.hpp"

class Car_3C;

class Montana_C
{
public:
    Montana_C* field_0;
    Montana_C* field_4_pNext;
    Montana_C* field_8;
};

class Montana_4
{
public:
    EXPORT void sub_5C5CF0(s32 a2);
    EXPORT void Draw_5C5DF0();
    EXPORT void Reset_5C5E50();
    EXPORT Montana_4* ctor_5C5E70();
    EXPORT void dtor_5C5F10();
    Montana_C* field_0_pFirst;
};

class Montana
{
public:
    EXPORT void ResetAll_4954F0();
    EXPORT s32 sub_495510(Car_3C* a2);
    EXPORT void Draw_495560(s32 col_idx);
    EXPORT Montana* ctor_4955A0();
    EXPORT void dtor_495630();
    Montana_4* field_0_cols[7];
};
