#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Car_BC;
class infallible_turing;
class Object_2C;

class Crusher_30
{
  public:
    enum CrusherStates
    {
        Idle_0 = 0,
        CrushW_1 = 1,
        UnCrushW_2 = 2,
        CrushH_3 = 3,
        UnCrushH_4 = 4,
    };

    EXPORT Crusher_30();
    EXPORT ~Crusher_30();
    EXPORT void CrushCar_488310(Car_BC* pCar);
    EXPORT void Service_488350();
    EXPORT void InitCrusher_4885A0(Fix16 xpos, Fix16 ypos, char_type crusher_idx);

    Car_BC* GetCarBeingCrushed() const 
    {
      return field_14_pCarBeingCrushed;
    }

  private:
    // Each side of the crusher walls and some unknown object
    Object_2C* field_0;
    Object_2C* field_4;
    Object_2C* field_8;
    Object_2C* field_C;
    Object_2C* field_10;

    Car_BC* field_14_pCarBeingCrushed;

    infallible_turing* field_18_sound;

    // Target of crusher walls
    Fix16 field_1C_w;
    Fix16 field_20_h;

    // Location of the crusher
    Fix16 field_24_xpos;
    Fix16 field_28_ypos;

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

  private:
    Crusher_30 field_0[3];
    s32 field_90_count;
};
EXTERN_GLOBAL(CrusherPool_94*, gCrusherPool_94_67A830);
