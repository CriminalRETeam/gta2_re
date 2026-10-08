#include "zealous_borg.hpp"
#include "Function.hpp"

MATCH_FUNC(0x484ED0)
zealous_borg::zealous_borg() // 0x484ED0
{
    Reset_484EF0();
}

MATCH_FUNC(0x484EE0)
zealous_borg::~zealous_borg() // 0x484EE0
{
    // Empty
}

MATCH_FUNC(0x484EF0)
void zealous_borg::Reset_484EF0()
{
    u8* pByteIter = field_28_bCountAllowed;
    u32* pIntIter = field_0_crime_count_list;
    for (s32 i = GTA2_COUNTOF(field_28_bCountAllowed) - 1; i >= 0; i--)
    {
        *pIntIter = 0;
        *pByteIter = 1;
        ++pIntIter;
        ++pByteIter;
    }
    field_34_car_damage_cost = 0;
    field_38_evasion_rating = 0;
}

MATCH_FUNC(0x484F20)
void zealous_borg::Service_484F20()
{
    ResetCrimeCountFlags_484F30();
}

MATCH_FUNC(0x484F30)
void zealous_borg::ResetCrimeCountFlags_484F30()
{
    for (u8 i = 1; i < 10; i++)
    {
        field_28_bCountAllowed[i] = 1;
    }
}

MATCH_FUNC(0x484F50)
void zealous_borg::IncrementCrimeCount_484F50(int crime_type)
{
    switch (crime_type)
    {
        case crime_stats_type::weapon_fired_2:
            if (field_28_bCountAllowed[crime_type])
            {
                field_0_crime_count_list[crime_type]++;
                field_28_bCountAllowed[crime_type] = 0;
            }
            break;
        case crime_stats_type::car_damaged_1:
        case crime_stats_type::car_destroyed_3:
        case crime_stats_type::bus_stolen_4:
        case crime_stats_type::Vehicles_Hijacked_5:
        case crime_stats_type::Civilians_run_down_6:
        case crime_stats_type::Civilians_murdered_7:
        case crime_stats_type::Lawmen_killed_8:
        case crime_stats_type::Gang_members_killed_9:
            field_0_crime_count_list[crime_type]++;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x484FA0)
void zealous_borg::AddCarDamageCost_484FA0(int a2)
{
    field_34_car_damage_cost += a2;
}

MATCH_FUNC(0x484FB0)
void zealous_borg::AddEvasionRating_484FB0(int amount)
{
    field_38_evasion_rating += amount;
}