#pragma once

#include "BitSet32.hpp"
#include "Fix16.hpp"
#include "Function.hpp"
#include "Pool.hpp"
#include "ang16.hpp"

class Car_BC;
class Sprite;
class CarChaseTask_40;
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

class CarAI_78
{
  public:
    EXPORT void MakeAgressiveSirensAndLights_4476F0();
    EXPORT void PlanTurnAtNextJunction_447710();
    EXPORT void DoShortcutsUsingJunctions_447970();
    inline void TurnAround_447970();
    EXPORT bool GoToBlock_447CA0(u8 x, u8 y, u8 z, s32 maybe_direction);
    EXPORT char_type TrySetTargetDirectionFromArrows_447D40(gmp_block_info* a2);
    EXPORT bool IsClockwiseTurning_448270();
    EXPORT void BrakeForBlockedRoadAhead_4482C0();
    EXPORT void CheckRoadAhead_448770();
    EXPORT void ManageTrafficCarDirection_448CE0();
    EXPORT void FollowRoadDirection_44A1F0();
    EXPORT void AlignToLaneCenter_44AF00();
    EXPORT void DetectCarAhead_44D1D0();
    EXPORT void Init_AI_Chase_44E0C0();
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
    EXPORT void RaiseSpeedTo_453990(Fix16 a2);
    EXPORT void ClearA6Bits2And3_4539B0();
    EXPORT void UpdateSpeedTowardTarget_4539D0();
    EXPORT void UpdateTrainMovement_453A40();
    EXPORT void AI_Service_453BB0();
    EXPORT void SetCar_453BF0(Car_BC* a2);
    EXPORT void ReverseOrNeutral_453C00();
    EXPORT void PoolAllocate();
    EXPORT CarAI_78();

    void PoolDeallocate()
    {
        field_0_car = 0;
    }

    Car_BC* field_0_car;
    s32 field_4_unused;
    char_type field_8_maneuver_active;
    char_type field_9_probe_x;
    char_type field_A_probe_y;
    char_type field_B_pad;
    CarAI_78* mpNext;
    Ang16 field_10_angle;
    s16 field_12_pad;
    Fix16 field_14_speed;
    Fix16 field_18_target_speed;
    Fix16 field_1C_acceleration;
    s32 field_20_unused;

    union
    {
        u32 field_24_flags;
        CompilerBitField32 field_24_bf;
    };

    char_type field_28_junc_idx;
    char_type field_29_pad;
    u8 field_2A_stopped_timer;
    u8 field_2B_ticks_since_alloc;
    u8 field_2C_yield_timer;
    char_type field_2D_arrow_start;
    char_type field_2E_arrow_cur;
    char_type field_2F_arrow_end;
    u8 field_30_forced_stop_timer;
    char_type field_31_pad;
    char_type field_32_pad;
    char_type field_33_pad;
    s32 field_34;
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
    Fix16 field_60;
    Fix16 field_64_unused;
    Car_BC* field_68_car_in_collision; // Car collided
    Car_BC* field_6C_yield_to_car;
    Sprite* field_70_nearest_entity;
    Fix16 field_74_max_speed;
};

class CarAI_78_Pool
{
  public:
    //Inlined in Car_6C constructor 9.6f -> 0x420eb0
    CarAI_78_Pool()
    {
    }

    ~CarAI_78_Pool()
    {
        field_0_pool.field_0_pHead = 0;
    }

    CarAI_78* Allocate()
    {
        return field_0_pool.Allocate();
    }

    // TODO: get 9.6f inline addr
    void DeAllocate(CarAI_78* p78)
    {
        field_0_pool.DeAllocate(p78);
    }

    PoolBasic<CarAI_78, 306> field_0_pool;
};

EXTERN_GLOBAL(CarAI_78_Pool*, gCarAI_78_Pool_677CF8);
