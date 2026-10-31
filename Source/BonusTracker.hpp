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

    gmp_map_zone* mpZone; // 0x0
    s16 mEventType; // 0x4
    s32 mCarModel; // 0x8
    s32 mOccupation; // 0xC
    s16 mGangIdx; // 0x10
    s16 mRemap; // 0x12
    s32 mDeathCause; // 0x14
    s32 mAltCarModel; // 0x18
    s32 mTimeLimit; // 0x1C: -1: no limit
    s32 mStartTime; // 0x20
    char_type mCheckMode; // 0x24: 1: a non matching event cancels the rule, otherwise it is ignored
    u8 mTargetCount; // 0x25
    u8 mProgressCount; // 0x26
    u16 mReward; // 0x28
    bool mbUsed; // 0x2A
    bool mbActive; // 0x2B
};
GTA2_ASSERT_SIZEOF_ALWAYS(BonusRule, 0x2C);

// The rules of one player's score tracker (PlayerScoreTracker::mBonuses)
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
        return mBonuses[idx].mProgressCount;
    }

    // 9.6f 0x476680
    inline void DeactivateBonus_476680(u16 idx)
    {
        mBonuses[idx].Deactivate_431DB0();
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

    BonusRule mBonuses[k_max_rules]; // 0x0
    class PlayerScoreTracker* mpScores; // 0x1B8
    s32 mCurTime; // 0x1BC
};
GTA2_ASSERT_SIZEOF_ALWAYS(BonusTracker, 0x1C0);