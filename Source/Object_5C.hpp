#pragma once

#include "Function.hpp"

class Object_2C;
class Car_3C;
class Sprite_4C;
class Car_BC;

class Object_5C
{
  public:
    EXPORT void sub_529300();
    EXPORT void sub_5293A0();
    EXPORT Object_5C* ctor_529430();
    EXPORT void dtor_529750();
    EXPORT void sub_5297F0();
    EXPORT s32 sub_5298E0(s32 a2);
    EXPORT Object_2C* sub_529950(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s32 a7, Sprite_4C* a8, Car_BC* a9);
    EXPORT Object_2C* sub_5299B0(s32 object_type, s32 a3, s32 a4, s32 a5, s16 a6);
    EXPORT Object_2C* sub_5299F0(s32 a2, u8 a3, s32 a4, s32 a5, s32 a6);
    EXPORT s32 sub_529A40(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u8 a7);
    EXPORT Object_2C* sub_529AB0(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, u8 a8);
    EXPORT Object_2C* sub_529B20(s32 obj_type,
                                 s32 a3,
                                 s32 a4,
                                 s32 argb,
                                 s32 a6,
                                 u8 intensity,
                                 char_type on_time,
                                 char_type off_time,
                                 char_type shape);
    EXPORT Object_2C* sub_529BC0(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6);
    EXPORT Object_2C* sub_529C00(s32 object_type, s32 a3, s32 a4, s32 a5, s16 a6, char_type a7);
    EXPORT char_type sub_52A210(char_type a2);
    EXPORT s32* sub_52A240(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s16 a7, s32 a8, s32 a9, s32 a10);
    EXPORT s32* sub_52A280(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s16 a7, s32 a8, s32 a9, s32 a10);
    EXPORT s32* sub_52A2C0(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s16 a7, s32 a8, s32 a9, s32 a10, char_type a11);
    EXPORT s32* sub_52A3D0(s32 a2, s32 a3, s32 a4, s16 a5, s32 a6, s32 a7);

    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    char_type field_18;
    char_type field_19;
    char_type field_1A;
    char_type field_1B;
    s32 field_1C;
    char_type field_20[48];
    s16 field_50;
    s16 field_52;
    s32 field_54;
    Car_3C* field_58;
};

extern Object_5C* gObject_5C_6F8F84;
