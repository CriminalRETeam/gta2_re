#pragma once

#include "Function.hpp"
#include "Weapon_30.hpp"

class Ped;
class Car_BC;
class infallible_turing;

// TODO: Move this
class struct_4
{
  public:
    struct_4() // 424620 inline
    {
        sub_4207E0();
    }

    void sub_4207E0() // inline
    {
        field_0 = 0;
    }

  private:
    s32 field_0;
};

class Weapon_8
{
  public:
    EXPORT Weapon_30* sub_5E3C10(s32 a2, Ped* a3, u8 a4);
    EXPORT s32 sub_5E3CB0(s32 a2);
    EXPORT Weapon_30* sub_5E3CE0(s32 a1, Car_BC* a2, u8 a3);
    EXPORT Weapon_30* sub_5E3D20(Car_BC* a2, s32 a3);
    EXPORT char_type sub_5E3D50(s32 a2, u8 a3, Car_BC* a4);
    EXPORT void sub_5E3DF0(u32* a1);
    EXPORT char_type sub_5E3E70(s32 a2);
    EXPORT char_type sub_5E3E80(s32 a2);
    EXPORT Weapon_8();
    EXPORT ~Weapon_8();
    struct_4 field_0;
    s16 field_4_ref_count;
    s16 field_6;
};

class Weapon_2FDC
{
  public:
    Weapon_2FDC() // 4CDA20 inline
    {
        Weapon_30* pF8 = field_8;
        Weapon_30* pIter = field_8;

        for (s32 i = 0; i < 254; i++)
        {
            pIter->field_18_pNext = pIter + 1;
            pIter++;
        }
        field_0 = pF8;
        field_8[254].field_18_pNext = 0;
        field_4 = 0;
        field_2FD8 = 0;
    }

    EXPORT ~Weapon_2FDC();

    Weapon_30* field_0;
    Weapon_30* field_4;
    Weapon_30 field_8[255];
    s16 field_2FD8;
    s16 field_2FDA;
};

extern EXPORT_VAR Weapon_2FDC* gWeapon_2FDC_707014;
extern EXPORT_VAR Weapon_8* gWeapon_8_707018;
