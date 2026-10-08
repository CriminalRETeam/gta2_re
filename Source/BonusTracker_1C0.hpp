#pragma once

#include "Function.hpp"

class gmp_map_zone;

// One bonus challenge defined by the mission script: count the events that match the filters and pay the reward
// when target_count is reached (within time_limit). Filters set to their wildcard value match everything.
class BonusRule_2C
{
  public:
    EXPORT BonusRule_2C(); // 0x431D30
    EXPORT ~BonusRule_2C(); // 0x431D40
    EXPORT void Init_431D50();
    EXPORT void Reset_431DA0();
    EXPORT void Deactivate_431DB0();

    gmp_map_zone* field_0_pZone;
    s16 field_4_event_type;
    s32 field_8_car_model;
    s32 field_C_occupation;
    s16 field_10_gang_idx;
    s16 field_12_remap;
    s32 field_14_death_cause;
    s32 field_18_alt_car_model;
    s32 field_1C_time_limit; // -1: no limit
    s32 field_20_start_time;
    char_type field_24_check_mode; // 1: a non matching event cancels the rule, otherwise it is ignored
    u8 field_25_target_count;
    u8 field_26_progress_count;
    u16 field_28_reward;
    char_type field_2A_bUsed;
    char_type field_2B_bActive;
};

// The rules of one player's score tracker (PlayerScoreTracker_36C::field_1A8_bonuses)
class BonusTracker_1C0
{
  public:
    enum
    {
        k_max_rules = 10
    };

    // 9.6f 0x476660
    inline u8 GetProgressCount_476660(u16 idx)
    {
        return field_0_bonuses[idx].field_26_progress_count;
    }

    // 9.6f 0x476680
    inline void DeactivateBonus_476680(u16 idx)
    {
        field_0_bonuses[idx].Deactivate_431DB0();
    }

    EXPORT BonusTracker_1C0(); // 0x431DC0
    EXPORT ~BonusTracker_1C0(); // 0x431DF0
    EXPORT void Init_431E10(class PlayerScoreTracker_36C* pScores);
    EXPORT void Service_431E30();
    EXPORT u16 FindFreeRule_431E90();
    EXPORT u16 FindMatchingRule_431EC0(u16 start_idx, s16 event_type, s32 car_model, s32 occupation, s16 gang_idx, s16 remap, s32 death_cause, s32 alt_car_model, gmp_map_zone* pZone);
    EXPORT s16 AllocRule_431FE0(s16 event_type,
                                 s32 car_model,
                                 s32 occupation,
                                 s16 gang_idx,
                                 s16 remap,
                                 s32 death_cause,
                                 s32 alt_car_model,
                                 s32 time_limit,
                                 s8 check_mode,
                                 s8 target_count,
                                 u16 reward,
                                 gmp_map_zone* pZone);
    EXPORT s16 GetBonusResult_432080(u16 idx);
    EXPORT void ProcessBonusEvent_4320D0(s16 event_type, s32 car_model, s32 occupation, s16 gang_idx, s16 remap, s32 death_cause, s32 alt_car_model, gmp_map_zone* pZone);
    EXPORT s8 IsDeathCauseInGroup_432170(int death_cause, int rule_death_cause);
    EXPORT s8 IsOccupationInGroup_432240(int occupation, int rule_occupation);
    EXPORT bool AreCarModelsEquivalent_432300(int car_model_1, int car_model_2);

    BonusRule_2C field_0_bonuses[k_max_rules];
    class PlayerScoreTracker_36C* field_1B8_pScores;
    s32 field_1BC_cur_time;
};