#pragma once

#include "Function.hpp"
#include "types.hpp"

class Marz_3
{
public:
    EXPORT void ctor_463F90();
    EXPORT void dtor_463FA0();
    char_type field_0;
    char_type field_1;
    char_type field_2;
};

class Marz_96
{
public:
    EXPORT void dtor_4BDF90();
    EXPORT void sub_543EC0();
    EXPORT Marz_96* ctor_544BF0();
    Marz_3 field_0[50];
};

class Marz_1D7E
{
public:
    EXPORT void dtor_4BDF70();
    EXPORT Marz_1D7E* ctor_543ED0();
    EXPORT Marz_96* sub_543F10(u8* a2);
    Marz_96 field_0[50];
    char_type field_1D4C[48];
    s16 field_1D7C;
};
