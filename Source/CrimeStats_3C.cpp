#include "CrimeStats_3C.hpp"
#include "Function.hpp"

MATCH_FUNC(0x484ED0)
CrimeStats_3C::CrimeStats_3C() // 0x484ED0
{
    Reset_484EF0();
}

MATCH_FUNC(0x484EE0)
CrimeStats_3C::~CrimeStats_3C() // 0x484EE0
{
    // Empty
}

MATCH_FUNC(0x484EF0)
void CrimeStats_3C::Reset_484EF0()
{
    bool* pByteIter = field_28_bCountAllowed;
    u32* pIntIter = field_0_crime_count_list;
    for (s32 i = GTA2_COUNTOF(field_28_bCountAllowed) - 1; i >= 0; i--)
    {
        *pIntIter = 0;
        *pByteIter = true;
        ++pIntIter;
        ++pByteIter;
    }
    field_34_car_damage_cost = 0;
    field_38_evasion_rating = 0;
}

MATCH_FUNC(0x484F20)
void CrimeStats_3C::Service_484F20()
{
    ResetCrimeCountFlags_484F30();
}

MATCH_FUNC(0x484F30)
void CrimeStats_3C::ResetCrimeCountFlags_484F30()
{
    for (u8 i = crime_stats_type::car_damaged_1; i < crime_stats_type::count_10; i++)
    {
        field_28_bCountAllowed[i] = true;
    }
}

MATCH_FUNC(0x484F50)
void CrimeStats_3C::IncrementCrimeCount_484F50(int crime_type)
{
    switch (crime_type)
    {
        case crime_stats_type::weapon_fired_2:
            if (field_28_bCountAllowed[crime_type])
            {
                field_0_crime_count_list[crime_type]++;
                field_28_bCountAllowed[crime_type] = false;
            }
            break;
        case crime_stats_type::car_damaged_1:
        case crime_stats_type::car_destroyed_3:
        case crime_stats_type::bus_stolen_4:
        case crime_stats_type::vehicles_hijacked_5:
        case crime_stats_type::civilians_run_down_6:
        case crime_stats_type::civilians_murdered_7:
        case crime_stats_type::lawmen_killed_8:
        case crime_stats_type::gang_members_killed_9:
            field_0_crime_count_list[crime_type]++;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x484FA0)
void CrimeStats_3C::AddCarDamageCost_484FA0(int cost)
{
    field_34_car_damage_cost += cost;
}

MATCH_FUNC(0x484FB0)
void CrimeStats_3C::AddEvasionRating_484FB0(int amount)
{
    field_38_evasion_rating += amount;
}