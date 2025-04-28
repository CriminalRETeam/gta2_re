#pragma once

#include "Function.hpp"

class Sprite;

class Collide_8
{
  public:
    Sprite* field_0_sprt;
    Collide_8* field_4_pNext;
};


class Collide_C
{
  public:
    EXPORT void sub_478A20();
    EXPORT Collide_C();
    EXPORT ~Collide_C();
    s32 field_0_count;
    s32 field_4_count;
    s32 field_8_bUnknown;
};
