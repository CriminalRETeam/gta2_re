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

class CrimeStats_3C
{
  public:
    EXPORT CrimeStats_3C(); // 0x484ED0
    EXPORT ~CrimeStats_3C(); // 0x484EE0
    EXPORT void Reset_484EF0();
    EXPORT void Service_484F20();
    EXPORT void ResetCrimeCountFlags_484F30();
    EXPORT void IncrementCrimeCount_484F50(int crime_type);
    EXPORT void AddCarDamageCost_484FA0(int cost);
    EXPORT void AddEvasionRating_484FB0(int amount);

  public:
    u32 field_0_crime_count_list[crime_stats_type::count_10]; // indexed by crime_stats_type
    bool field_28_bCountAllowed[crime_stats_type::count_10];
    s32 field_34_car_damage_cost;
    s32 field_38_evasion_rating;
};