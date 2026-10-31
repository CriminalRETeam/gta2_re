#include "BonusTracker.hpp"
#include "Function.hpp"
#include "Player.hpp" // PlayerScoreTracker
#include "rng.hpp"
#include "bonus_event_type.hpp"
#include "bonus_result.hpp"
#include "ped_death_cause.hpp"

// Wildcard values of the BonusRule gang and remap filters
enum
{
    k_any_gang = -1,
    k_any_remap = -2,
};

MATCH_FUNC(0x431D30);
BonusRule::BonusRule()
{
    Init();
}

MATCH_FUNC(0x431D40);
BonusRule::~BonusRule()
{
}

MATCH_FUNC(0x431D50);
void BonusRule::Init()
{
    mpZone = NULL;
    mEventType = bonus_event_type::any_event;
    mCarModel = car_model_enum::none;
    mOccupation = ped_ocupation_enum::no_occupation;
    mGangIdx = k_any_gang;
    mRemap = k_any_remap;
    mDeathCause = ped_death_cause::none_0;
    mAltCarModel = car_model_enum::none;
    mTimeLimit = 0;
    mStartTime = 0;
    mCheckMode = 0;
    mTargetCount = 0;
    mProgressCount = 0;
    mReward = 0;
    mbUsed = false;
    mbActive = false;
}

MATCH_FUNC(0x431DA0);
void BonusRule::Reset()
{
    Init();
}

MATCH_FUNC(0x431DB0);
void BonusRule::Deactivate()
{
    mbActive = false;
}

// ============

MATCH_FUNC(0x431DC0);
BonusTracker::BonusTracker()
{
    mpScores = NULL;
    mCurTime = 0;
}

MATCH_FUNC(0x431DF0);
BonusTracker::~BonusTracker()
{
    mpScores = NULL;
}

MATCH_FUNC(0x431E10);
void BonusTracker::Init(PlayerScoreTracker* pScores)
{
    mCurTime = 0;
    mpScores = pScores;
}

MATCH_FUNC(0x431E30);
void BonusTracker::Service()
{
    mCurTime = gpRng->get_cur_rng_41CFE0();

    BonusRule* pIter = &mBonuses[0];
    for (s32 i = GTA2_COUNTOF(mBonuses) - 1; i >= 0; i--)
    {
        if (pIter->mbUsed)
        {
            if (pIter->mbActive)
            {
                const u32 time_limit = pIter->mTimeLimit;
                if (time_limit != -1 && static_cast<u32>(mCurTime - pIter->mStartTime) > time_limit)
                {
                    pIter->mProgressCount = 0;
                    pIter->Deactivate();
                }
            }
        }
        pIter++;
    }
}

MATCH_FUNC(0x431E90);
u16 BonusTracker::FindFreeRule() const
{
    for (u16 i = 0; i < GTA2_COUNTOF(mBonuses); i++)
    {
        if (!mBonuses[i].mbUsed)
        {
            return i;
        }
    }
    return GTA2_COUNTOF(mBonuses);
}

MATCH_FUNC(0x431EC0);
u16 BonusTracker::FindMatchingRule(u16 start_idx,
                                s16 event_type,
                                s32 car_model,
                                s32 occupation,
                                s16 gang_idx,
                                s16 remap,
                                s32 death_cause,
                                s32 alt_car_model,
                                gmp_map_zone* pZone)
{
    for (u16 i = start_idx; i < k_max_rules; i++)
    {
        BonusRule* pBonus = &mBonuses[i];
        if (!pBonus->mbUsed)
        {
            continue;
        }
        if (!pBonus->mbActive)
        {
            continue;
        }
        const s16 bonus_type = pBonus->mEventType;
        if (bonus_type != event_type && bonus_type != bonus_event_type::any_event)
        {
            continue;
        }
        const s32 bonus_occupation = pBonus->mOccupation;
        if (bonus_occupation == occupation || bonus_occupation == ped_ocupation_enum::no_occupation || IsOccupationInGroup(occupation, bonus_occupation))
        {
            const s32 bonus_car_model = pBonus->mCarModel;
            if (bonus_car_model == car_model || bonus_car_model == car_model_enum::none || AreCarModelsEquivalent(car_model, bonus_car_model))
            {
                const s16 bonus_gang_idx = pBonus->mGangIdx;
                if (bonus_gang_idx == gang_idx || bonus_gang_idx == k_any_gang)
                {
                    const s16 bonus_remap = pBonus->mRemap;
                    if (bonus_remap == remap || bonus_remap == k_any_remap)
                    {
                        const s32 bonus_death_cause = pBonus->mDeathCause;
                        if (bonus_death_cause == death_cause || bonus_death_cause == ped_death_cause::any_23 || IsDeathCauseInGroup(death_cause, bonus_death_cause))
                        {
                            const s32 bonus_alt_car_model = pBonus->mAltCarModel;
                            if ((bonus_alt_car_model == alt_car_model || bonus_alt_car_model == car_model_enum::none) && (pBonus->mpZone == pZone || !pBonus->mpZone))
                            {
                                return i;
                            }
                        }
                    }
                }
            }
        }
        if (pBonus->mCheckMode == 1)
        {
            pBonus->Deactivate();
        }
    }
    return k_max_rules;
}

MATCH_FUNC(0x431FE0);
s16 BonusTracker::AllocRule(s16 event_type,
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
                                      gmp_map_zone* pZone)
{
    const s16 idx = FindFreeRule();
    if (idx == k_max_rules)
    {
        return idx;
    }

    mBonuses[idx].mEventType = event_type;
    mBonuses[idx].mCarModel = car_model;
    mBonuses[idx].mOccupation = occupation;
    mBonuses[idx].mGangIdx = gang_idx;
    mBonuses[idx].mRemap = remap;
    mBonuses[idx].mDeathCause = death_cause;
    mBonuses[idx].mAltCarModel = alt_car_model;
    mBonuses[idx].mpZone = pZone;
    mBonuses[idx].mStartTime = gpRng->get_cur_rng_41CFE0();
    mBonuses[idx].mTimeLimit = time_limit;
    mBonuses[idx].mCheckMode = check_mode;
    mBonuses[idx].mTargetCount = target_count;
    mBonuses[idx].mProgressCount = 0;
    mBonuses[idx].mReward = reward;
    mBonuses[idx].mbUsed = true;
    mBonuses[idx].mbActive = true;

    return idx;
}

MATCH_FUNC(0x432080);
s16 BonusTracker::GetBonusResult(u16 idx)
{
    BonusRule* pRule = &mBonuses[idx];
    if (!pRule->mbUsed)
    {
        return bonus_result::unused_rule;
    }

    if (pRule->mbActive)
    {
        return bonus_result::in_progress;
    }

    if (pRule->mProgressCount == pRule->mTargetCount)
    {
        pRule->Reset();
        return bonus_result::passed;
    }
    else
    {
        pRule->Reset();
        return bonus_result::failed;
    }
}

MATCH_FUNC(0x4320D0);
void BonusTracker::ProcessBonusEvent(s16 event_type, s32 car_model, s32 occupation, s16 gang_idx, s16 remap, s32 death_cause, s32 alt_car_model, gmp_map_zone* pZone)
{
    for (u16 i = 0; i < k_max_rules; i++)
    {
        i = FindMatchingRule(i, event_type, car_model, occupation, gang_idx, remap, death_cause, alt_car_model, pZone);
        if (i >= k_max_rules)
        {
            break;
        }
        BonusRule* pFound = &mBonuses[i];
        mBonuses[i].mProgressCount++;
        if (GetProgressCount_476660(i) == pFound->mTargetCount)
        {
            mpScores->mPlayer->AddScore_41DC40(pFound->mReward);
            pFound->Deactivate();
        }
    }
}

MATCH_FUNC(0x432170);
bool BonusTracker::IsDeathCauseInGroup(s32 death_cause, s32 rule_death_cause) const
{
    if (rule_death_cause == ped_death_cause::run_over_1)
    {
        if (death_cause == ped_death_cause::run_over_by_stolen_car_3)
        {
            return true;
        }
    }
    else if (rule_death_cause == ped_death_cause::bomb_12)
    {
        if (death_cause == ped_death_cause::explosion_4)
        {
            return true;
        }
    }
    else if (rule_death_cause == ped_death_cause::mine_21)
    {
        if (death_cause == ped_death_cause::explosion_4)
        {
            return true;
        }
    }
    else if (rule_death_cause == ped_death_cause::grenade_15)
    {
        if (death_cause == ped_death_cause::explosion_4)
        {
            return true;
        }
    }
    else if (rule_death_cause == ped_death_cause::molotov_16)
    {
        if (death_cause == ped_death_cause::explosion_4)
        {
            return true;
        }
    }
    else if (rule_death_cause == ped_death_cause::rocket_bullet_17)
    {
        if (death_cause == ped_death_cause::explosion_4)
        {
            return true;
        }
    }
    else if (rule_death_cause == ped_death_cause::fire_hit_14)
    {
        if (death_cause == ped_death_cause::fire_13)
        {
            return true;
        }
    }
    else if (rule_death_cause == ped_death_cause::any_weapon_22)
    {
        switch (death_cause)
        {
            case ped_death_cause::explosion_4:
            case ped_death_cause::punched_10:
            case ped_death_cause::bullet_11:
            case ped_death_cause::fire_13:
            case ped_death_cause::fire_hit_14:
            case ped_death_cause::grenade_15:
            case ped_death_cause::molotov_16:
            case ped_death_cause::rocket_bullet_17:
            case ped_death_cause::rocket_18:
            case ped_death_cause::shotgun_19:
                return true;
        }
    }
    return false;
}

MATCH_FUNC(0x432240);
bool BonusTracker::IsOccupationInGroup(s32 occupation, s32 rule_occupation) const
{
    if (rule_occupation == ped_ocupation_enum::any_law_enforcement)
    {
        switch (occupation)
        {
            case ped_ocupation_enum::police:
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::fbi:
            case ped_ocupation_enum::army_army:
            case ped_ocupation_enum::walking_guard_29:
            case ped_ocupation_enum::walking_swat_guard_30:
            case ped_ocupation_enum::walking_army_guard_31:
            case ped_ocupation_enum::tank_driver:
            case ped_ocupation_enum::roadblock_cop_37:
            case ped_ocupation_enum::road_block_tank_man:
                return true;
            default:
                return false;
        }
    }
    else if (rule_occupation == ped_ocupation_enum::any_emergency_service_man)
    {
        switch (occupation)
        {
            case ped_ocupation_enum::paramedic_23:
            case ped_ocupation_enum::police:
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::fbi:
            case ped_ocupation_enum::army_army:
            case ped_ocupation_enum::walking_guard_29:
            case ped_ocupation_enum::walking_swat_guard_30:
            case ped_ocupation_enum::walking_army_guard_31:
            case ped_ocupation_enum::tank_driver:
            case ped_ocupation_enum::roadblock_cop_37:
            case ped_ocupation_enum::fireman:
            case ped_ocupation_enum::road_block_tank_man:
                return true;
            default:
                return false;
        }
    }
    else if (rule_occupation == ped_ocupation_enum::any_gang_member)
    {
        if (occupation == ped_ocupation_enum::armed_gang_member_19 || occupation == ped_ocupation_enum::guard || occupation == ped_ocupation_enum::gang_driver_42)
        {
            return true;
        }
    }
    else if (rule_occupation == ped_ocupation_enum::any_elvis)
    {
        if (occupation == ped_ocupation_enum::elvis || occupation == ped_ocupation_enum::elvis_leader)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x432300);
bool BonusTracker::AreCarModelsEquivalent(s32 car_model_1, s32 car_model_2) const
{
    bool is_fed_car_1;
    bool is_fed_car_2;

    if ((car_model_1 == car_model_enum::TAXI || car_model_1 == car_model_enum::STYPECAB) &&
        (car_model_2 == car_model_enum::TAXI || car_model_2 == car_model_enum::STYPECAB))
    {
        return true;
    }
    switch (car_model_1)
    {
        case car_model_enum::apc:
        case car_model_enum::COPCAR:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::SWATVAN:
        case car_model_enum::TANK:
        case car_model_enum::EDSELFBI:
            is_fed_car_1 = true;
            break;
        default:
            is_fed_car_1 = false;
            break;
    }
    switch (car_model_2)
    {
        case car_model_enum::apc:
        case car_model_enum::COPCAR:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::SWATVAN:
        case car_model_enum::TANK:
        case car_model_enum::EDSELFBI:
            is_fed_car_2 = true;
            break;
        default:
            is_fed_car_2 = false;
            break;
    }
    if (is_fed_car_1 && is_fed_car_2)
    {
        return true;
    }
    return false;
}