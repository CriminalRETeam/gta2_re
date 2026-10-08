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
    Vehicles_Hijacked_5 = 5,
    Civilians_run_down_6 = 6,
    Civilians_murdered_7 = 7,
    Lawmen_killed_8 = 8,
    Gang_members_killed_9 = 9,
};
} // namespace crime_stats_type

class zealous_borg
{
  public:
    EXPORT zealous_borg(); // 0x484ED0
    EXPORT ~zealous_borg(); // 0x484EE0
    EXPORT void Reset_484EF0();
    EXPORT void Service_484F20();
    EXPORT void ResetCrimeCountFlags_484F30();
    EXPORT void IncrementCrimeCount_484F50(int crime_type);
    EXPORT void AddCarDamageCost_484FA0(int a2);
    EXPORT void AddEvasionRating_484FB0(int amount);

  public:
    u32 field_0_crime_count_list[10];
    u8 field_28_bCountAllowed[10];
    s16 field_32;
    s32 field_34_car_damage_cost;
    s32 field_38_evasion_rating;
};