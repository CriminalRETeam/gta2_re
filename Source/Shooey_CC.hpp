#pragma once

#include "Function.hpp"

class cool_nash_0x294;
class angry_lewin_0x85C;
class Car_BC;

class Shooey_14
{
  public:
    EXPORT Shooey_14();
    EXPORT ~Shooey_14();
    EXPORT s32 sub_484CF0(s32 a2, s32 a3);
    EXPORT void sub_484D50(s32* a2, s32* a3, s32* a4, u32* a5);

    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
};

class Shooey_CC
{
  public:
    EXPORT Shooey_CC* ctor_484D80();
    EXPORT void dtor_484DB0();
    EXPORT void sub_484DD0(s32 a2, s32 a3);
    EXPORT bool sub_484E20(s32* a2, s32* a3, s32* a4, u32* a5);
    EXPORT char_type sub_484E90(s32 a2);
    EXPORT Shooey_CC* ctor_484FC0();
    EXPORT void dtor_484FD0();
    EXPORT void sub_484FE0(u32 a2, cool_nash_0x294* pPed);
    EXPORT bool sub_485090(Car_BC* a2, angry_lewin_0x85C* a3);
    EXPORT char_type sub_4850F0(Shooey_CC* a1, s32 a2, angry_lewin_0x85C* a3);
    EXPORT char_type sub_485140(cool_nash_0x294* a2, angry_lewin_0x85C* a3);

    u16 field_0;
    u16 field_2;
    Shooey_14 field_4[10];
};
