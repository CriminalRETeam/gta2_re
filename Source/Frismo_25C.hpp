#pragma once

#include "Function.hpp"
#include "types.hpp"

class Frismo_C
{
public:
    EXPORT Frismo_C();
    EXPORT ~Frismo_C();
    EXPORT s32 sub_503110();
    s32 field_0;
    s16 field_4;
    s16 field_6;
    Frismo_C* field_8_next;
};

class Frismo_25C
{
public:
    EXPORT ~Frismo_25C();
    Frismo_C* field_0;
    Frismo_C field_4[50];
};

