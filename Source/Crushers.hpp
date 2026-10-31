#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Car;
class SoundObject;
class Object;

class Crusher
{
  public:
    STATIC_MEMBER(Fix16, kCrusherOpenDist);
    // 9.6f 0x411A20
    inline Fix16 get_xpos() const
    {
        return mXpos;
    }

    // 9.6f 0x411A30
    inline Fix16 get_ypos() const
    {
        return mYpos;
    }

    // 9.6f 0x411A10
    inline s32 get_state() const
    {
        return mState;
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
    EXPORT void CrushCar(Car* pCar);
    EXPORT void Service();
    EXPORT void InitCrusher(Fix16 xpos, Fix16 ypos, char_type crusher_idx);

    Car* GetCarBeingCrushed() const 
    {
      return mpCarBeingCrushed;
    }

  private:
    // Each side of the crusher walls and some unknown object
    Object* mPistonTop; // 0x0
    Object* mPistonRight; // 0x4
    Object* mPistonBottom; // 0x8
    Object* mPistonLeft; // 0xC
    Object* mCentralSpot; // 0x10

    Car* mpCarBeingCrushed; // 0x14

    SoundObject* mSound; // 0x18

    // Target of crusher walls
    Fix16 mW; // 0x1C
    Fix16 mH; // 0x20
public:
    // Location of the crusher
    Fix16 mXpos; // 0x24
    Fix16 mYpos; // 0x28

    s32 mState; // 0x2C
};
GTA2_ASSERT_SIZEOF_ALWAYS(Crusher, 0x30);

class CrusherPool
{
  public:
    EXPORT bool IsCarBeingCrushed(Car* pCar);
    EXPORT void CrushCarWithCrusher(Car* pCar, u8 crusher_idx);
    EXPORT void CrushersService();
    EXPORT Crusher* CreateCrusher(Fix16 xpos, Fix16 ypos);
    EXPORT CrusherPool();
    EXPORT ~CrusherPool();

  private:
    Crusher mCrushers[3]; // 0x0
    s32 mCount; // 0x90
};
GTA2_ASSERT_SIZEOF_ALWAYS(CrusherPool, 0x94);
EXTERN_GLOBAL(CrusherPool*, gCrusherPool_94);
