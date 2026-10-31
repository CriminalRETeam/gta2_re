#pragma once

#include "Car.hpp"
#include "Function.hpp"
#include "garage_state.hpp"
#include <cstdio>

class Car;
class Ped;
class Door;

class Garage
{
  public:
    EXPORT ~Garage();
    EXPORT void ValidateParkCommand();
    EXPORT u8 ParkCarAtDoor(Car* pCar, Door* pDoor);
    EXPORT void GaragesService();
    EXPORT Garage();

    // inlined 0x44C870
    bool IsParkedCar_44C870(const Car* pCar) const
    {
        if (pCar == this->mParkedCar && pCar != NULL && pCar->mCarId == this->mParkedCarId)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x434AF0
    inline bool IsParkingCarInPosition_434AF0(const Car* pCar) const
    {
        return pCar == mParkingCar && mInPosition;
    }

    // 9.6f 0x489650
    inline void Reset_489650()
    {
        mDoor = NULL;
        mState = garage_state::idle_0;
        mPushDir.x = 0;
        mPushDir.y = 0;
        mParkXMin = 0;
        mParkYMin = 0;
        mParkXMax = 0;
        mParkYMax = 0;
        mTargetX = 0;
        mTargetY = 0;
    }

    // 9.6f 0x493540
    bool IsParkingCar_493540(const Car* pCar) const
    {
        if (pCar == mParkingCar)
        {
            return true;
        }
        return false;
    }

    Car* mParkingCar; // 0x0
    Car* mParkedCar; // 0x4
    s32 mParkedCarId; // 0x8
    s32 mState; // 0xC
    Door* mDoor; // 0x10
    Ped* mDriver; // 0x14
    Fix16 mParkXMin; // 0x18
    Fix16 mParkYMin; // 0x1C
    Fix16 mParkXMax; // 0x20
    Fix16 mParkYMax; // 0x24
    Fix16_Point mPushDir; // 0x28
    Fix16 mTargetX; // 0x30
    Fix16 mTargetY; // 0x34
    s32 mDoorFace; // 0x38
    char_type mSettleTicks; // 0x3C
    bool mInPosition; // 0x3D
    u8 mParkTicket; // 0x3E
    bool mNoRespawn; // 0x3F
    bool mLongCar; // 0x40
    char_type mUnused41; // 0x41
    char_type mUnused42; // 0x42
    char_type mUnused43; // 0x43
    u32 mWaitTicks; // 0x44
};
GTA2_ASSERT_SIZEOF_ALWAYS(Garage, 0x48);

EXTERN_GLOBAL(Garage*, gGarage_48);
