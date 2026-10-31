#pragma once

#include "Function.hpp"

namespace crime_stats_type
{
enum
{
    none_0 = 0,
    car_damaged_1 = 1,
    weapon_fired_2 = 2,
    car_destroyed_3 = 3,
    bus_stolen_4 = 4,
    vehicles_hijacked_5 = 5,
    civilians_run_down_6 = 6,
    civilians_murdered_7 = 7,
    lawmen_killed_8 = 8,
    gang_members_killed_9 = 9,
    count_10 = 10,
};
} // namespace crime_stats_type

class PlayerCrimeStats
{
  public:
    EXPORT PlayerCrimeStats(); // 0x484ED0
    EXPORT ~PlayerCrimeStats(); // 0x484EE0
    EXPORT void Reset();
    EXPORT void Service();
    EXPORT void ResetCrimeCountFlags();
    EXPORT void IncrementCrimeCount(s32 crime_type);
    EXPORT void AddCarDamageCost(s32 cost);
    EXPORT void AddEvasionRating(s32 amount);

  public:
    u32 mCrimeCountList[crime_stats_type::count_10]; // 0x0: indexed by crime_stats_type
    bool mbCountAllowed[crime_stats_type::count_10]; // 0x28
    s32 mCarDamageCost; // 0x34
    s32 mEvasionRating; // 0x38
};
GTA2_ASSERT_SIZEOF_ALWAYS(PlayerCrimeStats, 0x3C);