#include "PlayerCrimeStats.hpp"
#include "Function.hpp"

MATCH_FUNC(0x484ED0)
PlayerCrimeStats::PlayerCrimeStats() // 0x484ED0
{
    Reset();
}

MATCH_FUNC(0x484EE0)
PlayerCrimeStats::~PlayerCrimeStats() // 0x484EE0
{
    // Empty
}

MATCH_FUNC(0x484EF0)
void PlayerCrimeStats::Reset()
{
    bool* pByteIter = mbCountAllowed;
    u32* pIntIter = mCrimeCountList;
    for (s32 i = GTA2_COUNTOF(mbCountAllowed) - 1; i >= 0; i--)
    {
        *pIntIter = 0;
        *pByteIter = true;
        ++pIntIter;
        ++pByteIter;
    }
    mCarDamageCost = 0;
    mEvasionRating = 0;
}

MATCH_FUNC(0x484F20)
void PlayerCrimeStats::Service()
{
    ResetCrimeCountFlags();
}

MATCH_FUNC(0x484F30)
void PlayerCrimeStats::ResetCrimeCountFlags()
{
    for (u8 i = crime_stats_type::car_damaged_1; i < crime_stats_type::count_10; i++)
    {
        mbCountAllowed[i] = true;
    }
}

MATCH_FUNC(0x484F50)
void PlayerCrimeStats::IncrementCrimeCount(s32 crime_type)
{
    switch (crime_type)
    {
        case crime_stats_type::weapon_fired_2:
            if (mbCountAllowed[crime_type])
            {
                mCrimeCountList[crime_type]++;
                mbCountAllowed[crime_type] = false;
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
            mCrimeCountList[crime_type]++;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x484FA0)
void PlayerCrimeStats::AddCarDamageCost(s32 cost)
{
    mCarDamageCost += cost;
}

MATCH_FUNC(0x484FB0)
void PlayerCrimeStats::AddEvasionRating(s32 amount)
{
    mEvasionRating += amount;
}