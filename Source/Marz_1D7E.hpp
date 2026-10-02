#pragma once

#include "Function.hpp"

#pragma pack(push)
#pragma pack(1)
class Marz_3
{
  public:
    // ctor 463F90
    // dtor 463FA0
    EXPORT Marz_3();
    EXPORT ~Marz_3();

    u8 field_0_x;
    u8 field_1_y;
    u8 field_2_z;
};
#pragma pack(pop)

class Marz_96
{
  public:
    EXPORT ~Marz_96();
    EXPORT void sub_543EC0();
    EXPORT Marz_96();
    Marz_3 field_0_points[50];
};

class Marz_1D7E
{
  public:
    EXPORT ~Marz_1D7E();
    EXPORT Marz_1D7E();
    EXPORT Marz_96* AllocPatrolList_543F10(u8* a2);
    Marz_96 field_0_lists[50];
    u8 field_1D4C_used[50];
};

EXTERN_GLOBAL(Marz_1D7E*, gMarz_1D7E_6FD784);
