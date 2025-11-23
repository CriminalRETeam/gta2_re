#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Car_BC;
class infallible_turing;
class Object_2C;

class Crusher_30
{
  public:
    EXPORT Crusher_30(); // 4882D0
    EXPORT ~Crusher_30(); // 4882E0
    EXPORT void CrushCar_488310(Car_BC* pCar);
    EXPORT void Service_488350();
    EXPORT void InitCrusher_4885A0(Fix16 xpos, Fix16 ypos, char_type crusher_idx);

    // Each side of the crusher?
    Object_2C* field_0;
    Object_2C* field_4;
    Object_2C* field_8;
    Object_2C* field_C;

    s32 field_10;

    // Car getting crunched?
    Car_BC* field_14_pObj;

    infallible_turing* field_18_sound;

    s32 field_1C;
    s32 field_20;

    s32 field_24_xpos;
    s32 field_28_ypos;

    s32 field_2C_state;
};

class CrusherPool_94
{
  public:
    EXPORT bool IsCarBeingCrushed_4887A0(Car_BC* pCar);
    EXPORT void CrushCarWithCrusher_4887D0(Car_BC* pCar, u8 crusher_idx);
    EXPORT void CrushersService_4887F0();
    EXPORT Crusher_30* CreateCrusher_488820(Fix16 xpos, Fix16 ypos);
    EXPORT CrusherPool_94();
    EXPORT ~CrusherPool_94();

    Crusher_30 field_0[3];
    s32 field_90_count;
};
EXTERN_GLOBAL(CrusherPool_94*, gCrusherPool_94_67A830);
