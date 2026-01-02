#include "zealous_borg.hpp"
#include "Function.hpp"

MATCH_FUNC(0x484ED0)
zealous_borg::zealous_borg() // 0x484ED0
{
    sub_484EF0();
}

MATCH_FUNC(0x484EE0)
zealous_borg::~zealous_borg() // 0x484EE0
{
    // Empty
}

MATCH_FUNC(0x484EF0)
void zealous_borg::sub_484EF0()
{
    u8* pByteIter = field_28;
    u32* pIntIter = field_0_crime_count_list;
    for (s32 i = GTA2_COUNTOF(field_28) - 1; i >= 0; i--)
    {
        *pIntIter = 0;
        *pByteIter = 1;
        ++pIntIter;
        ++pByteIter;
    }
    field_34 = 0;
    field_38 = 0;
}

MATCH_FUNC(0x484F20)
void zealous_borg::sub_484F20()
{
    sub_484F30();
}

MATCH_FUNC(0x484F30)
void zealous_borg::sub_484F30()
{
    for (u8 i = 1; i < 10; i++)
    {
        field_28[i] = 1;
    }
}

MATCH_FUNC(0x484F50)
void zealous_borg::IncrementCrimeCount_484F50(int crime_type)
{
    switch (crime_type)
    {
        case crime_stats_type::Unknown_2:
            if (field_28[crime_type])
            {
                field_0_crime_count_list[crime_type]++;
                field_28[crime_type] = 0;
            }
            break;
        case crime_stats_type::Unknown_1:
        case crime_stats_type::Unknown_3:
        case crime_stats_type::Unknown_4:
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
void zealous_borg::sub_484FA0(int a2)
{
    field_34 += a2;
}

MATCH_FUNC(0x484FB0)
void zealous_borg::sub_484FB0(int amount)
{
    field_38 += amount;
}