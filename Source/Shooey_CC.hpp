#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Ped;
class Player;
class Car_BC;

class Shooey_14
{
  public:
    EXPORT Shooey_14();
    EXPORT ~Shooey_14();
    EXPORT void sub_484CF0(s32 a2, s32 a3);
    EXPORT void sub_484D50(s32* a2, Fix16* a3, Fix16* a4, Fix16* a5);

    s32 field_0;
    s32 field_4;
    Fix16_Vec field_8;
};

class Shooey_CC
{
  public:
    EXPORT Shooey_CC();
    EXPORT ~Shooey_CC();
    EXPORT void sub_484DD0(s32 a2, s32 a3);
    EXPORT bool sub_484E20(s32* a2, s32* a3, s32* a4, u32* a5);
    EXPORT char_type sub_484E90(s32 a2);
    EXPORT Shooey_CC* ctor_484FC0();
    EXPORT void dtor_484FD0();
    EXPORT void sub_484FE0(u32 a2, Ped* pPed);
    EXPORT bool sub_485090(Car_BC* a2, Player* a3);
    EXPORT char_type sub_4850F0(Shooey_CC* a1, s32 a2, Player* a3);
    EXPORT char_type sub_485140(Ped* a2, Player* a3);

    u16 field_0;
    u16 field_2;
    Shooey_14 field_4[10];
};
