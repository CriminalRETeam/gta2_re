#pragma once

#include "Function.hpp"

class Car_BC;
class Ped;

class Tango_28
{
  public:
    // inline
    void sub_450C10()
    {
        field_C_target_car = 0;
        field_8_state = 0;
        field_4_bActive = 0;
        field_1C_car = 0;
        field_20_ped = 0;
        field_24 = 0;
    }

    EXPORT char_type sub_4A7FC0();
    EXPORT void deinit_4A81A0();
    EXPORT void sub_4A81F0();
    EXPORT void init_4A85C0();
    EXPORT s32 sub_4A85E0();

    s16 field_0;
    s16 field_2;
    s32 field_4_bActive;
    s32 field_8_state;
    Car_BC* field_C_target_car;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    Car_BC* field_1C_car;
    Ped* field_20_ped;
    s16 field_24;
    s16 field_26;
};

class Tango_54
{
  public:
    Tango_54()
    {
        for (s32 i = 0; i < 2; i++)
        {
            field_0[i].init_4A85C0();
        }
        sub_4A88D0();
    }

    ~Tango_54()
    {
        // TODO: Should this be empty?
    }

    EXPORT void sub_4A85F0();
    EXPORT Tango_28* sub_4A8620(Car_BC* a2, s32 a3, s32 a4, s32 a5);
    EXPORT Tango_28* New28_4A8800();
    EXPORT char_type sub_4A8820(Car_BC* a2);
    EXPORT void sub_4A88D0();

    Tango_28 field_0[2];
    s16 field_50;
    s16 field_52;
};