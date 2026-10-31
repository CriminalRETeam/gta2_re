#pragma once

#include "Function.hpp"

class gmp_map_zone;

// One bonus challenge defined by the mission script: count the events that match the filters and pay the reward
// when target_count is reached (within time_limit). Filters set to their wildcard value match everything.
class BonusRule
{
  public:
    EXPORT BonusRule(); // 0x431D30
    EXPORT ~BonusRule(); // 0x431D40
    EXPORT void Init_431D50();
    EXPORT void Reset_431DA0();
    EXPORT void Deactivate_431DB0();

    gmp_map_zone* field_0_pZone; // 0x0
    s16 field_4_event_type; // 0x4
    s32 field_8_car_model; // 0x8
    s32 field_C_occupation; // 0xC
    s16 field_10_gang_idx; // 0x10
    s16 field_12_remap; // 0x12
    s32 field_14_death_cause; // 0x14
    s32 field_18_alt_car_model; // 0x18
    s32 field_1C_time_limit; // 0x1C: -1: no limit
    s32 field_20_start_time; // 0x20
    char_type field_24_check_mode; // 0x24: 1: a non matching event cancels the rule, otherwise it is ignored
    u8 field_25_target_count; // 0x25
    u8 field_26_progress_count; // 0x26
    u16 field_28_reward; // 0x28
    bool field_2A_bUsed; // 0x2A
    bool field_2B_bActive; // 0x2B
};
GTA2_ASSERT_SIZEOF_ALWAYS(BonusRule, 0x2C);

// The rules of one player's score tracker (PlayerScoreTracker::field_1A8_bonuses)
class BonusTracker
{
  public:
    enum
    {
        k_max_rules = 10
    };

    // 9.6f 0x476660
    inline u8 GetProgressCount_476660(u16 idx) const
    {
        return field_0_bonuses[idx].field_26_progress_count;
    }

    // 9.6f 0x476680
    inline void DeactivateBonus_476680(u16 idx)
    {
        field_0_bonuses[idx].Deactivate_431DB0();
    }

    EXPORT BonusTracker(); // 0x431DC0
    EXPORT ~BonusTracker(); // 0x431DF0
    EXPORT void Init_431E10(class PlayerScoreTracker* pScores);
    EXPORT void Service_431E30();

  private:
    EXPORT u16 FindFreeRule_431E90() const;
    EXPORT u16 FindMatchingRule_431EC0(u16 start_idx, s16 event_type, s32 car_model, s32 occupation, s16 gang_idx, s16 remap, s32 death_cause, s32 alt_car_model, gmp_map_zone* pZone);

  public:
    EXPORT s16 AllocRule_431FE0(s16 event_type,
                                 s32 car_model,
                                 s32 occupation,
                                 s16 gang_idx,
                                 s16 remap,
                                 s32 death_cause,
                                 s32 alt_car_model,
                                 s32 time_limit,
                                 s8 check_mode,
                                 u8 target_count,
                                 u16 reward,
                                 gmp_map_zone* pZone);
    EXPORT s16 GetBonusResult_432080(u16 idx);
    EXPORT void ProcessBonusEvent_4320D0(s16 event_type, s32 car_model, s32 occupation, s16 gang_idx, s16 remap, s32 death_cause, s32 alt_car_model, gmp_map_zone* pZone);

  private:
    EXPORT bool IsDeathCauseInGroup_432170(s32 death_cause, s32 rule_death_cause) const;
    EXPORT bool IsOccupationInGroup_432240(s32 occupation, s32 rule_occupation) const;
    EXPORT bool AreCarModelsEquivalent_432300(s32 car_model_1, s32 car_model_2) const;

    BonusRule field_0_bonuses[k_max_rules]; // 0x0
    class PlayerScoreTracker* field_1B8_pScores; // 0x1B8
    s32 field_1BC_cur_time; // 0x1BC
};
GTA2_ASSERT_SIZEOF_ALWAYS(BonusTracker, 0x1C0);