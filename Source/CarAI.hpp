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
        field_0_car = NULL;
    }

    Car* field_0_car; // 0x0
    s32 field_4_unused; // 0x4
    char_type field_8_maneuver_active; // 0x8
    char_type field_9_probe_x; // 0x9
    char_type field_A_probe_y; // 0xA
    CarAI* mpNext;
    Ang16 field_10_angle; // 0x10
    Fix16 field_14_speed; // 0x14
    Fix16 field_18_target_speed; // 0x18
    Fix16 field_1C_acceleration; // 0x1C
    s32 field_20_unused; // 0x20

    union
    {
        u32 field_24_flags; // 0x24
        CarAIFlags32 field_24_bf; // 0x24
    };

    char_type field_28_route_idx; // 0x28
    char_type field_29_pad; // 0x29
    u8 field_2A_stopped_timer; // 0x2A
    u8 field_2B_ticks_since_alloc; // 0x2B
    u8 field_2C_yield_timer; // 0x2C
    char_type field_2D_arrow_start; // 0x2D
    char_type field_2E_arrow_cur; // 0x2E
    char_type field_2F_arrow_end; // 0x2F
    u8 field_30_forced_stop_timer; // 0x30
    s32 field_34_unused; // 0x34
    s32 field_38_junction_turn_direction; // 0x38
    s32 field_3C_seeking_road; // 0x3C
    s32 field_40_prev_target_direction; // 0x40
    s32 field_44_target_direction; // 0x44
    s32 field_48_probe_direction; // 0x48
    s32 field_4C_curr_direction; // 0x4C
    s32 field_50_seek_road_turn; // 0x50
    u16 field_54_accel_cooldown; // 0x54
    s16 field_56_route_pos; // 0x56
    u16 field_58_off_road_timer; // 0x58
    s16 field_5A_obstacle_timer; // 0x5A
    Fix16 field_5C_unused; // 0x5C
    Fix16 field_60_unused; // 0x60
    Fix16 field_64_unused; // 0x64
    Car* field_68_car_in_collision; // 0x68
    Car* field_6C_yield_to_car; // 0x6C
    Sprite* field_70_nearest_entity; // 0x70
    Fix16 field_74_max_speed; // 0x74
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
        field_0_pool.field_0_pHead = NULL;
    }

    CarAI* Allocate()
    {
        return field_0_pool.Allocate();
    }

    // 9.6f 0x425460
    void DeAllocate(CarAI* pCarAI)
    {
        field_0_pool.DeAllocate(pCarAI);
    }

    PoolBasic<CarAI, 306> field_0_pool; // 0x0
};

EXTERN_GLOBAL(CarAIPool*, gCarAI_78_Pool_677CF8);
