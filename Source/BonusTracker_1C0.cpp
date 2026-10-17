#include "BonusTracker_1C0.hpp"
#include "Function.hpp"
#include "Player.hpp" // PlayerScoreTracker_36C
#include "rng.hpp"
#include "bonus_event_type.hpp"
#include "ped_death_cause.hpp"

MATCH_FUNC(0x431D30);
BonusRule_2C::BonusRule_2C()
{
    Init_431D50();
}

MATCH_FUNC(0x431D40);
BonusRule_2C::~BonusRule_2C()
{
}

MATCH_FUNC(0x431D50);
void BonusRule_2C::Init_431D50()
{
    field_0_pZone = 0;
    field_4_event_type = bonus_event_type::any_event;
    field_8_car_model = car_model_enum::none;
    field_C_occupation = ped_ocupation_enum::no_occupation;
    field_10_gang_idx = -1;
    field_12_remap = -2;
    field_14_death_cause = ped_death_cause::none_0;
    field_18_alt_car_model = car_model_enum::none;
    field_1C_time_limit = 0;
    field_20_start_time = 0;
    field_24_check_mode = 0;
    field_25_target_count = 0;
    field_26_progress_count = 0;
    field_28_reward = 0;
    field_2A_bUsed = 0;
    field_2B_bActive = 0;
}

MATCH_FUNC(0x431DA0);
void BonusRule_2C::Reset_431DA0()
{
    Init_431D50();
}

MATCH_FUNC(0x431DB0);
void BonusRule_2C::Deactivate_431DB0()
{
    field_2B_bActive = 0;
}

// ============

MATCH_FUNC(0x431DC0);
BonusTracker_1C0::BonusTracker_1C0()
{
    field_1B8_pScores = 0;
    field_1BC_cur_time = 0;
}

MATCH_FUNC(0x431DF0);
BonusTracker_1C0::~BonusTracker_1C0()
{
    field_1B8_pScores = 0;
}

MATCH_FUNC(0x431E10);
void BonusTracker_1C0::Init_431E10(PlayerScoreTracker_36C* pScores)
{
    field_1BC_cur_time = 0;
    field_1B8_pScores = pScores;
}

MATCH_FUNC(0x431E30);
void BonusTracker_1C0::Service_431E30()
{
    field_1BC_cur_time = gpRng_67AB34->get_cur_rng_41CFE0();

    BonusRule_2C* pIter = &field_0_bonuses[0];
    for (s32 i = GTA2_COUNTOF(field_0_bonuses) - 1; i >= 0; i--)
    {
        if (pIter->field_2A_bUsed)
        {
            if (pIter->field_2B_bActive)
            {
                const u32 f1c = pIter->field_1C_time_limit;
                if (f1c != -1 && field_1BC_cur_time - pIter->field_20_start_time > f1c)
                {
                    pIter->field_26_progress_count = 0;
                    pIter->Deactivate_431DB0();
                }
            }
        }
        pIter++;
    }
}

MATCH_FUNC(0x431E90);
u16 BonusTracker_1C0::FindFreeRule_431E90()
{
    for (u16 i = 0; i < GTA2_COUNTOF(field_0_bonuses); i++)
    {
        if (!field_0_bonuses[i].field_2A_bUsed)
        {
            return i;
        }
    }
    return GTA2_COUNTOF(field_0_bonuses);
}

MATCH_FUNC(0x431EC0);
u16 BonusTracker_1C0::FindMatchingRule_431EC0(u16 start_idx,
                                s16 event_type,
                                s32 car_model,
                                s32 occupation,
                                s16 gang_idx,
                                s16 remap,
                                s32 death_cause,
                                s32 alt_car_model,
                                gmp_map_zone* pZone)
{
    u16 i;
    BonusRule_2C* pBonus;
    s16 bonus_type;
    s32 bonus_occupation;
    s32 bonus_car_model;
    s16 bonus_gang_idx;
    s16 bonus_remap;
    s32 bonus_death_cause;
    s32 bonus_alt_car_model;

    for (i = start_idx; i < k_max_rules; i++)
    {
        pBonus = &this->field_0_bonuses[i];
        if (!pBonus->field_2A_bUsed)
        {
            continue;
        }
        if (!pBonus->field_2B_bActive)
        {
            continue;
        }
        bonus_type = pBonus->field_4_event_type;
        if (bonus_type != event_type && bonus_type != bonus_event_type::any_event)
        {
            continue;
        }
        bonus_occupation = pBonus->field_C_occupation;
        if (bonus_occupation == occupation || bonus_occupation == ped_ocupation_enum::no_occupation || IsOccupationInGroup_432240(occupation, pBonus->field_C_occupation))
        {
            bonus_car_model = pBonus->field_8_car_model;
            if (bonus_car_model == car_model || bonus_car_model == car_model_enum::none || AreCarModelsEquivalent_432300(car_model, pBonus->field_8_car_model))
            {
                bonus_gang_idx = pBonus->field_10_gang_idx;
                if (bonus_gang_idx == gang_idx || bonus_gang_idx == -1)
                {
                    bonus_remap = pBonus->field_12_remap;
                    if (bonus_remap == remap || bonus_remap == -2)
                    {
                        bonus_death_cause = pBonus->field_14_death_cause;
                        if (bonus_death_cause == death_cause || bonus_death_cause == ped_death_cause::any_23 || IsDeathCauseInGroup_432170(death_cause, pBonus->field_14_death_cause))
                        {
                            bonus_alt_car_model = pBonus->field_18_alt_car_model;
                            if ((bonus_alt_car_model == alt_car_model || bonus_alt_car_model == car_model_enum::none) && (pBonus->field_0_pZone == pZone || !pBonus->field_0_pZone))
                            {
                                return i;
                            }
                        }
                    }
                }
            }
        }
        if (pBonus->field_24_check_mode == 1)
        {
            field_0_bonuses[i].Deactivate_431DB0();
        }
    }
    return k_max_rules;
}

MATCH_FUNC(0x431FE0);
s16 BonusTracker_1C0::AllocRule_431FE0(s16 event_type,
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
                                      gmp_map_zone* pZone)
{
    const s16 idx = FindFreeRule_431E90();
    if (idx == k_max_rules)
    {
        return idx;
    }

    field_0_bonuses[idx].field_4_event_type = event_type;
    field_0_bonuses[idx].field_8_car_model = car_model;
    field_0_bonuses[idx].field_C_occupation = occupation;
    field_0_bonuses[idx].field_10_gang_idx = gang_idx;
    field_0_bonuses[idx].field_12_remap = remap;
    field_0_bonuses[idx].field_14_death_cause = death_cause;
    field_0_bonuses[idx].field_18_alt_car_model = alt_car_model;
    field_0_bonuses[idx].field_0_pZone = pZone;
    field_0_bonuses[idx].field_20_start_time = gpRng_67AB34->get_cur_rng_41CFE0();
    field_0_bonuses[idx].field_1C_time_limit = time_limit;
    field_0_bonuses[idx].field_24_check_mode = check_mode;
    field_0_bonuses[idx].field_25_target_count = target_count;
    field_0_bonuses[idx].field_26_progress_count = 0;
    field_0_bonuses[idx].field_28_reward = reward;
    field_0_bonuses[idx].field_2A_bUsed = 1;
    field_0_bonuses[idx].field_2B_bActive = 1;

    return idx;
}

MATCH_FUNC(0x432080);
s16 BonusTracker_1C0::GetBonusResult_432080(u16 idx)
{
    BonusRule_2C* pItem = &field_0_bonuses[idx];
    if (!pItem->field_2A_bUsed)
    {
        return -1;
    }

    if (pItem->field_2B_bActive)
    {
        return -2;
    }

    if (pItem->field_26_progress_count == pItem->field_25_target_count)
    {
        pItem->Reset_431DA0();
        return -3;
    }
    else
    {
        pItem->Reset_431DA0();
        return -4;
    }
}

MATCH_FUNC(0x4320D0);
void BonusTracker_1C0::ProcessBonusEvent_4320D0(s16 event_type, s32 car_model, s32 occupation, s16 gang_idx, s16 remap, s32 death_cause, s32 alt_car_model, gmp_map_zone* pZone)
{
    for (u16 i = 0; i < k_max_rules; i++)
    {
        i = FindMatchingRule_431EC0(i, event_type, car_model, occupation, gang_idx, remap, death_cause, alt_car_model, pZone);
        if (i >= k_max_rules)
        {
            break;
        }
        BonusRule_2C* pFound = &field_0_bonuses[i];
        field_0_bonuses[i].field_26_progress_count++;
        if (GetProgressCount_476660(i) == pFound->field_25_target_count)
        {
            field_1B8_pScores->field_368_player->AddScore_41DC40(pFound->field_28_reward);
            pFound->Deactivate_431DB0();
        }
    }
}

MATCH_FUNC(0x432170);
s8 BonusTracker_1C0::IsDeathCauseInGroup_432170(int death_cause, int rule_death_cause)
{
    if (rule_death_cause == ped_death_cause::run_over_1)
    {
        if (death_cause == ped_death_cause::run_over_by_stolen_car_3)
        {
            return 1;
        }
    }
    else if (rule_death_cause == ped_death_cause::bomb_12)
    {
        if (death_cause == ped_death_cause::unknown_4)
        {
            return 1;
        }
    }
    else if (rule_death_cause == ped_death_cause::unknown_21)
    {
        if (death_cause == ped_death_cause::unknown_4)
        {
            return 1;
        }
    }
    else if (rule_death_cause == ped_death_cause::grenade_15)
    {
        if (death_cause == ped_death_cause::unknown_4)
        {
            return 1;
        }
    }
    else if (rule_death_cause == ped_death_cause::molotov_16)
    {
        if (death_cause == ped_death_cause::unknown_4)
        {
            return 1;
        }
    }
    else if (rule_death_cause == ped_death_cause::rocket_bullet_17)
    {
        if (death_cause == ped_death_cause::unknown_4)
        {
            return 1;
        }
    }
    else if (rule_death_cause == ped_death_cause::fire_hit_14)
    {
        if (death_cause == ped_death_cause::fire_13)
        {
            return 1;
        }
    }
    else if (rule_death_cause == ped_death_cause::any_weapon_22)
    {
        switch (death_cause)
        {
            case ped_death_cause::unknown_4:
            case ped_death_cause::punched_10:
            case ped_death_cause::bullet_11:
            case ped_death_cause::fire_13:
            case ped_death_cause::fire_hit_14:
            case ped_death_cause::grenade_15:
            case ped_death_cause::molotov_16:
            case ped_death_cause::rocket_bullet_17:
            case ped_death_cause::rocket_18:
            case ped_death_cause::shotgun_19:
                return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x432240);
s8 BonusTracker_1C0::IsOccupationInGroup_432240(int occupation, int rule_occupation)
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
            case ped_ocupation_enum::unknown_cop_occu_30:
            case ped_ocupation_enum::unknown_cop_occu_31:
            case ped_ocupation_enum::tank_driver:
            case ped_ocupation_enum::roadblock_cop_37:
            case ped_ocupation_enum::road_block_tank_man:
                return 1;
            default:
                return 0;
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
            case ped_ocupation_enum::unknown_cop_occu_30:
            case ped_ocupation_enum::unknown_cop_occu_31:
            case ped_ocupation_enum::tank_driver:
            case ped_ocupation_enum::roadblock_cop_37:
            case ped_ocupation_enum::fireman:
            case ped_ocupation_enum::road_block_tank_man:
                return 1;
            default:
                return 0;
        }
    }
    else if (rule_occupation == ped_ocupation_enum::any_gang_member)
    {
        if (occupation == ped_ocupation_enum::armed_gang_member_19 || occupation == ped_ocupation_enum::guard || occupation == ped_ocupation_enum::gang_driver_42)
        {
            return 1;
        }
    }
    else if (rule_occupation == ped_ocupation_enum::any_elvis)
    {
        if (occupation == ped_ocupation_enum::elvis || occupation == ped_ocupation_enum::elvis_leader)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x432300);
bool BonusTracker_1C0::AreCarModelsEquivalent_432300(int car_model_1, int car_model_2)
{
    bool is_fed_car_1;
    bool is_fed_car_2;

    if ((car_model_1 == car_model_enum::TAXI || car_model_1 == car_model_enum::STYPECAB) &&
        (car_model_2 == car_model_enum::TAXI || car_model_2 == car_model_enum::STYPECAB))
    {
        return 1;
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
            is_fed_car_1 = 1;
            break;
        default:
            is_fed_car_1 = 0;
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
            is_fed_car_2 = 1;
            break;
        default:
            is_fed_car_2 = 0;
            break;
    }
    if (is_fed_car_1 && is_fed_car_2)
    {
        return true;
    }
    return false;
}