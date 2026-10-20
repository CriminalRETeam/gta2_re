#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Car;
class SoundObject;
class Object;

class Crusher
{
  public:
    // 9.6f 0x411A20
    inline Fix16 get_xpos_411A20()
    {
        return field_24_xpos;
    }

    // 9.6f 0x411A30
    inline Fix16 get_ypos_411A30()
    {
        return field_28_ypos;
    }

    // 9.6f 0x411A10
    inline s32 get_state_411A10()
    {
        return field_2C_state;
    }

    enum CrusherStates
    {
        Idle_0 = 0,
        CrushW_1 = 1,
        UnCrushW_2 = 2,
        CrushH_3 = 3,
        UnCrushH_4 = 4,
    };

    EXPORT Crusher();
    EXPORT ~Crusher();
    EXPORT void CrushCar_488310(Car* pCar);
    EXPORT void Service_488350();
    EXPORT void InitCrusher_4885A0(Fix16 xpos, Fix16 ypos, char_type crusher_idx);

    Car* GetCarBeingCrushed() const 
    {
      return field_14_pCarBeingCrushed;
    }

  private:
    // Each side of the crusher walls and some unknown object
    Object* field_0_piston_top;
    Object* field_4_piston_right;
    Object* field_8_piston_bottom;
    Object* field_C_piston_left;
    Object* field_10_central_spot;

    Car* field_14_pCarBeingCrushed;

    SoundObject* field_18_sound;

    // Target of crusher walls
    Fix16 field_1C_w;
    Fix16 field_20_h;
public:
    // Location of the crusher
    Fix16 field_24_xpos;
    Fix16 field_28_ypos;

    s32 field_2C_state;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Crusher, 0x30);

class CrusherPool
{
  public:
    EXPORT bool IsCarBeingCrushed_4887A0(Car* pCar);
    EXPORT void CrushCarWithCrusher_4887D0(Car* pCar, u8 crusher_idx);
    EXPORT void CrushersService_4887F0();
    EXPORT Crusher* CreateCrusher_488820(Fix16 xpos, Fix16 ypos);
    EXPORT CrusherPool();
    EXPORT ~CrusherPool();

  private:
    Crusher field_0_crushers[3];
    s32 field_90_count;
};
GTA2_ASSERT_SIZEOF_ALWAYS(CrusherPool, 0x94);
EXTERN_GLOBAL(CrusherPool*, gCrusherPool_94_67A830);
