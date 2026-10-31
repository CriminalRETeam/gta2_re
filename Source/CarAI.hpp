#pragma once

#include "BitSet32.hpp"
#include "car_ai_flags.hpp"
#include "Fix16.hpp"
#include "Function.hpp"
#include "Pool.hpp"
#include "ang16.hpp"

class Car;
class Sprite;
class CarChaseTask;
struct gmp_block_info;

namespace car_ai_direction
{
enum
{
    none_0 = 0,
    north_1 = 1,
    south_2 = 2,
    east_3 = 3,
    west_4 = 4,
};
} // namespace car_ai_direction

namespace car_ai_target_direction
{
enum
{
    go_straight_0 = 0,
    northwards_1 = 1,
    southwards_2 = 2,
    eastwards_3 = 3,
    westwards_4 = 4,
};
} // namespace car_ai_target_direction

class CarAI
{
  private:
    EXPORT void MakeAgressiveSirensAndLights_4476F0();
    EXPORT void PlanTurnAtNextJunction_447710();
    EXPORT void DoShortcutsUsingJunctions_447970();
    inline void TurnAround_447970();

  public:
    EXPORT bool GoToBlock_447CA0(u8 x, u8 y, u8 z, s32 arrow_type);

  private:
    EXPORT bool TrySetTargetDirectionFromArrows_447D40(const gmp_block_info* pBlock);
    EXPORT bool IsClockwiseTurning_448270() const;
    EXPORT void BrakeForBlockedRoadAhead_4482C0();
    EXPORT void CheckRoadAhead_448770();
    EXPORT void ManageTrafficCarDirection_448CE0();
    EXPORT void FollowRoadDirection_44A1F0();
    EXPORT void AlignToLaneCenter_44AF00();
    EXPORT void DetectCarAhead_44D1D0();
    EXPORT void UpdateChase_44E0C0();
    EXPORT void UpdateStateMachine_44E560();
    EXPORT void ReactToNearbyCar_451980();
    EXPORT void ReactToNearbyObject_451FA0();
    EXPORT void ReactToNearbyPed_451FF0();
    EXPORT void ScanAheadForObstacles_452060();
    EXPORT void ManageCollisions_452A20();
    EXPORT void UpdateDrivingAI_452DF0();
    EXPORT void UpdateDriving_453470();
    EXPORT void ChooseRandomTurn_4537D0();
    EXPORT void ClearSteeringIfTurning_4538B0();
    EXPORT void RaiseSpeedTo_453990(Fix16 target_speed);
    EXPORT void ClearTurnReady_4539B0();
    EXPORT void UpdateSpeedTowardTarget_4539D0();

  public:
    EXPORT void UpdateTrainMovement_453A40();
    EXPORT void AI_Service_453BB0();
    EXPORT void SetCar_453BF0(Car* pCar);

  private:
    EXPORT void ReverseOrNeutral_453C00();

  public:
    EXPORT void PoolAllocate();
    EXPORT CarAI();

    void PoolDeallocate()
    {
        mCar = NULL;
    }

    Car* mCar; // 0x0
    s32 mUnused4; // 0x4
    char_type mManeuverActive; // 0x8
    char_type mProbeX; // 0x9
    char_type mProbeY; // 0xA
    CarAI* mpNext;
    Ang16 mAngle; // 0x10
    Fix16 mSpeed; // 0x14
    Fix16 mTargetSpeed; // 0x18
    Fix16 mAcceleration; // 0x1C
    s32 mUnused20; // 0x20

    union
    {
        u32 mFlags; // 0x24
        CarAIFlags32 mBf24; // 0x24
    };

    char_type mRouteIdx; // 0x28
    char_type mPad29; // 0x29
    u8 mStoppedTimer; // 0x2A
    u8 mTicksSinceAlloc; // 0x2B
    u8 mYieldTimer; // 0x2C
    char_type mArrowStart; // 0x2D
    char_type mArrowCur; // 0x2E
    char_type mArrowEnd; // 0x2F
    u8 mForcedStopTimer; // 0x30
    s32 mUnused34; // 0x34
    s32 mJunctionTurnDirection; // 0x38
    s32 mSeekingRoad; // 0x3C
    s32 mPrevTargetDirection; // 0x40
    s32 mTargetDirection; // 0x44
    s32 mProbeDirection; // 0x48
    s32 mCurrDirection; // 0x4C
    s32 mSeekRoadTurn; // 0x50
    u16 mAccelCooldown; // 0x54
    s16 mRoutePos; // 0x56
    u16 mOffRoadTimer; // 0x58
    s16 mObstacleTimer; // 0x5A
    Fix16 mUnused5C; // 0x5C
    Fix16 mUnused60; // 0x60
    Fix16 mUnused64; // 0x64
    Car* mCarInCollision; // 0x68
    Car* mYieldToCar; // 0x6C
    Sprite* mNearestEntity; // 0x70
    Fix16 mMaxSpeed; // 0x74
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarAI, 0x78);

class CarAIPool
{
  public:
    //Inlined in CarManager constructor 9.6f -> 0x420eb0
    CarAIPool()
    {
    }

    ~CarAIPool()
    {
        mPool.mpHead = NULL;
    }

    CarAI* Allocate()
    {
        return mPool.Allocate();
    }

    // 9.6f 0x425460
    void DeAllocate(CarAI* pCarAI)
    {
        mPool.DeAllocate(pCarAI);
    }

    PoolBasic<CarAI, 306> mPool; // 0x0
};

EXTERN_GLOBAL(CarAIPool*, gCarAI_78_Pool_677CF8);
