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
  public:
    EXPORT void MakeAgressiveSirensAndLights_4476F0();
    EXPORT void PlanTurnAtNextJunction_447710();
    EXPORT void DoShortcutsUsingJunctions_447970();
    inline void TurnAround_447970();
    EXPORT bool GoToBlock_447CA0(u8 x, u8 y, u8 z, s32 arrow_type);
    EXPORT bool TrySetTargetDirectionFromArrows_447D40(gmp_block_info* pBlock);
    EXPORT bool IsClockwiseTurning_448270();
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
    EXPORT void UpdateTrainMovement_453A40();
    EXPORT void AI_Service_453BB0();
    EXPORT void SetCar_453BF0(Car* pCar);
    EXPORT void ReverseOrNeutral_453C00();
    EXPORT void PoolAllocate();
    EXPORT CarAI();

    void PoolDeallocate()
    {
        field_0_car = 0;
    }

    Car* field_0_car;
    s32 field_4_unused;
    char_type field_8_maneuver_active;
    char_type field_9_probe_x;
    char_type field_A_probe_y;
    CarAI* mpNext;
    Ang16 field_10_angle;
    Fix16 field_14_speed;
    Fix16 field_18_target_speed;
    Fix16 field_1C_acceleration;
    s32 field_20_unused;

    union
    {
        u32 field_24_flags;
        CarAIFlags32 field_24_bf;
    };

    char_type field_28_route_idx;
    char_type field_29_pad;
    u8 field_2A_stopped_timer;
    u8 field_2B_ticks_since_alloc;
    u8 field_2C_yield_timer;
    char_type field_2D_arrow_start;
    char_type field_2E_arrow_cur;
    char_type field_2F_arrow_end;
    u8 field_30_forced_stop_timer;
    s32 field_34_unused;
    s32 field_38_junction_turn_direction;
    s32 field_3C_seeking_road;
    s32 field_40_prev_target_direction;
    s32 field_44_target_direction;
    s32 field_48_probe_direction;
    s32 field_4C_curr_direction;
    s32 field_50_seek_road_turn;
    u16 field_54_accel_cooldown;
    s16 field_56_route_pos;
    u16 field_58_off_road_timer;
    s16 field_5A_obstacle_timer;
    Fix16 field_5C_unused;
    Fix16 field_60_unused;
    Fix16 field_64_unused;
    Car* field_68_car_in_collision; // Car collided
    Car* field_6C_yield_to_car;
    Sprite* field_70_nearest_entity;
    Fix16 field_74_max_speed;
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
        field_0_pool.field_0_pHead = 0;
    }

    CarAI* Allocate()
    {
        return field_0_pool.Allocate();
    }

    // TODO: get 9.6f inline addr
    void DeAllocate(CarAI* p78)
    {
        field_0_pool.DeAllocate(p78);
    }

    PoolBasic<CarAI, 306> field_0_pool;
};

EXTERN_GLOBAL(CarAIPool*, gCarAI_78_Pool_677CF8);
