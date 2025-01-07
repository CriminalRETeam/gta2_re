#pragma once

#include "Function.hpp"

class Car_3C;
class Object_3C;

class PurpleDoom
{
  public:
    EXPORT s32 sub_477A40();
    EXPORT char_type sub_477AE0(Car_3C* a1);
    EXPORT void sub_477B00(Car_3C* a1);
    EXPORT s32 sub_477B20(Car_3C* a1a);
    EXPORT s32 sub_477B60(Car_3C* a1);
    EXPORT void sub_477BA0();
    EXPORT char_type sub_477BD0(Car_3C* a2);
    EXPORT char_type sub_477C30(Car_3C* a2);
    EXPORT s32 sub_477C90(s32 a1, s32 a2, s32* a3, u8 a4, s32 a5, char_type a6);
    EXPORT s32 sub_477E60(Car_3C* a2, s32 a3);
    EXPORT char_type sub_477F30(s32* a2, char_type a3, s32 a4, Car_3C* a5, Object_3C* a6);
    EXPORT char_type sub_477F60(s32* a2, char_type a3, s32 a4, Car_3C* a5);
    EXPORT PurpleDoom();
    EXPORT ~PurpleDoom();
    EXPORT u32 sub_478160(u8 a2);
    EXPORT s32 sub_478240(s32 a2, s32 a3, s32 a4, s32 a5);
    EXPORT void sub_4782C0(s32 a2, s32 idx, Car_3C* a4);
    EXPORT u8* sub_478370(s32 a2, s32 a3);
    EXPORT char_type sub_478440(s32 a2, s32 idx, Car_3C* a4);
    EXPORT void sub_4784D0(s32 idx, Car_3C* a3);
    EXPORT u8* sub_478590(u32 a2);
    EXPORT char_type sub_4785D0(u32 a2, s32* a3);
    EXPORT char_type sub_478750(u32 a2, u32* a3);
    EXPORT char_type sub_4787E0(u32 a2, u32* a3);
    EXPORT s32 sub_478880(u32 a2, u32* a3);
    EXPORT void sub_478950(s32 a2, s32 a3);
    EXPORT PurpleDoom* sub_4789F0();

    void* field_0[256];
};

EXPORT_VAR extern PurpleDoom* gPurpleDoom_1_679208;
EXPORT_VAR extern PurpleDoom* gPurpleDoom_2_67920C;
EXPORT_VAR extern PurpleDoom* gPurpleDoom_3_679210;
EXPORT_VAR extern s32 dword_6F6108;
EXPORT_VAR extern s32 dword_6F5F38;