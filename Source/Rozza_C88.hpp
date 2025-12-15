#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class infallible_turing;
class Sprite;
class Car_BC;

class Rozza_28
{
  public:
    s32 field_0_type;
    Fix16 field_4_mapx_t1;
    s32 field_8; // prob Fix16
    Fix16 field_C_mapy_t2;
    s32 field_10; // prob Fix16
    Fix16 field_14_mapx_t2;
    Fix16 field_18_mapy_t1;
    Fix16 field_1C_mapz;
    Sprite* field_20_pSprite;
    s32 field_24;
};

class Rozza_A
{
  public:
  EXPORT void set_xyz_40B870(Fix16 x, Fix16 y, Fix16 z);

    s32 field_0;
    Fix16 field_4_x;
    Fix16 field_8_y;
    Fix16 field_C_z;
    Car_BC* field_10;
    Car_BC* field_14;
    s32 field_18_model_copy;
    s32 field_1C;
    s32 field_20_map_block_spec;
    s32 field_24_car_physics_value;
};

class Rozza_C88
{
  public:
    EXPORT void Reset_40BB90();
    EXPORT void sub_40BBA0(Sprite* a2, s32 a3);
    EXPORT void sub_40BC40(Sprite* a2);
    EXPORT void sub_40BD10(Sprite* a2);
    EXPORT void sub_40BDD0(Sprite* a2, Sprite* a3);
    EXPORT Rozza_C88();
    EXPORT ~Rozza_C88();

    infallible_turing* field_0_pSoundObj;
    Rozza_A field_4_pool[80];
    s32 field_C84_count;
};

EXTERN_GLOBAL(Rozza_C88*, gRozza_C88_66AFE0);
