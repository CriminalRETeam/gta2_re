#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "Police_38.hpp"

class Ped;
class Car_BC;
class Kfc_30;
class Object_2C;
class Police_7C;
class Ang16;

class Police_7B8
{
  public:
    Police_7B8()
    {
        sub_56F400();
    }

    EXPORT ~Police_7B8();
    EXPORT void sub_56F400();
    EXPORT char_type sub_56F4D0(Ped* a2);
    EXPORT Police_38* New_56F560();
    EXPORT Ped* SpawnRoadblockGuard_56F5C0(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT void sub_56F6D0(Car_BC* a2);
    EXPORT bool sub_56F800(Ped* a2);
    EXPORT bool sub_56F880(Ped* a2);
    EXPORT void sub_56F8E0(Ped* a2, Ped* a3);
    EXPORT s32 sub_56F940(Ped* a2);
    EXPORT s16 sub_56FA40();
    EXPORT char_type sub_56FAA0(Police_7C* a2);
    EXPORT void sub_56FBD0();
    EXPORT void sub_570270();
    EXPORT void SpawnWalkingGuard_570320(Ped* pPed, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT char_type sub_5703E0(Car_BC* a2);
    EXPORT bool sub_570790(Police_38* a1, Police_7C* a2);
    EXPORT char_type sub_5707B0(s32 a2, s32 a3);
    EXPORT s32 sub_5708C0(Police_7B8* a1, s32 a2, u32* a3);
    EXPORT void sub_570940(Ped* a3);
    EXPORT char_type sub_577320();
    EXPORT void sub_577370(u8 a2, s32 a3, s32 a4);

    u8 field_0;
    u8 field_1;
    u8 field_2;
    u8 field_3;
    Police_38 field_4[20];
    Police_7C field_464[4];
    s32 field_654_wanted_level;
    u8 field_658;
    char_type field_659;
    char_type field_65A;
    char_type field_65B;
    s32 field_65C;
    u8 field_660_wanted_star_count;
    char_type field_661;
    char_type field_662;
    char_type field_663;
    PoliceRoadblock_A4 field_664_obj;
    PoliceRoadblock_A4 field_708_obj;
    u8 field_7AC;
    char_type field_7AD_police_peds_in_range_screen;
    char_type field_7AE;
    char_type field_7AF;
    Ped* field_7B0;
    char_type field_7B4;
    char_type field_7B5;
    char_type field_7B6;
    char_type field_7B7;
};

EXTERN_GLOBAL(class Police_7B8*, gPolice_7B8_6FEE40);

EXTERN_GLOBAL(s32, gRoadblockGuardType_6FEDB8);
