#include "PlayerScoreTracker_36C.hpp"
#include "zealous_borg.hpp"
#include "ped_death_cause.hpp"
#include "car_model_flag.hpp"
#include "bonus_event_type.hpp"
#include "CarInfo_808.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "ExplodingScore_100.hpp"
#include "Frontend.hpp"
#include "Game_0x40.hpp"
#include "Hud.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "CrimeReportQueue_CC.hpp"
#include "Gang.hpp"
#include "debug.hpp"
#include "gtx_0x106C.hpp"
#include "lucid_hamilton.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "text_0x14.hpp"
#include "Ped.inl"
#include <string.h>

// TODO: move
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);
EXTERN_GLOBAL(CrimeReportQueue_CC*, gCrimeReportQueue_67A4B8);

DEFINE_GLOBAL_INIT(Fix16, kFlyCarMinVelocity_7028BC, Fix16(0x666, 0), 0x7028BC);


MATCH_FUNC(0x591bd0)
PlayerScoreTracker_36C::PlayerScoreTracker_36C()
{
    field_368_player = NULL;

    field_74_car_kill_combo = 1;
    field_75_score_mult = 1;

    field_18C_one_second_timer = 0;
    field_190_fly_car_count = 0;
    field_70_last_car_kill_time = 0;
    field_78_last_kill_time = 0;
    field_7C_execution_count = 0;
    field_86_total_kills = 0;
    field_88_killed_cops = 0;
    field_8A_cars_stolen_count = 0;
    field_80_last_elvis_kill_time = 0;
    field_84_num_elvis_killed = 0;
    field_194_last_shot_result = 0;
    field_198_accuracy_count = 0;
    field_19C_reverse_count = 0;
    field_1A0_last_emergency_car_kill_time = 0;
    field_1A4_killed_cars_flags = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_8C_car_model_flags); i++)
    {
        field_8C_car_model_flags[i] = 0;
    }
}

MATCH_FUNC(0x591c70)
void PlayerScoreTracker_36C::Service_591C70()
{
    field_1A8_bonuses.Service_431E30();
    Ped* player_ped = field_368_player->GetPlayerPed_41D020();
    field_18C_one_second_timer += gGame_0x40_67E008->GetFrameDurationMs_4B8BB0();

    if (field_18C_one_second_timer >= 1000)
    {
        field_18C_one_second_timer -= 1000;
        Car_BC* pCar = player_ped->get_car_416B60();

        if (pCar)
        {
            if (player_ped->not_enter_car_as_passenger_4A5040() && pCar->inline_check_0x20_info_4216C0())
            {
                if (pCar->field_4_passengers_list.field_0_pFirstPed)
                {
                    field_368_player->AddScore_41DC40(1);
                }
            }
        }
        if (player_ped->get_wanted_points_433DC0() >= 5000)
        {
            field_368_player->AddScore_41DC40(1);
        }

        field_368_player->field_644_crime_stats.AddEvasionRating_484FB0(player_ped->get_wanted_star_count_46EF00());
    }

    if (field_7C_execution_count >= 20u)
    {
        field_7C_execution_count = 0;
        field_368_player->AddScore_41DC40(100000);

        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("excutin"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(4);
        }
    }

    if (field_84_num_elvis_killed >= 6u)
    {
        field_84_num_elvis_killed = 0;
        field_368_player->AddScore_41DC40(30000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("elvis_d"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(8);
        }
    }

    if (field_1A4_killed_cars_flags == 7)
    {
        field_1A4_killed_cars_flags = 0;
        field_368_player->AddScore_41DC40(10000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("em_dest"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(11);
        }
    }

    if (field_86_total_kills >= 1000)
    {
        field_86_total_kills = 0;
        field_368_player->AddScore_41DC40(30000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("gencide"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(5);
        }
    }

    if (field_88_killed_cops >= 20u)
    {
        field_88_killed_cops = 0;
        field_368_player->AddScore_41DC40(5000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("copkill"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(6);
        }
    }

    if (field_8A_cars_stolen_count >= 100)
    {
        field_8A_cars_stolen_count = 0;
        field_368_player->AddScore_41DC40(10000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("carjaka"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(7);
        }
    }

    if (field_198_accuracy_count >= 25)
    {
        field_198_accuracy_count = 0;
        field_368_player->AddScore_41DC40(5000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("accurcy"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(9);
        }
    }

    Car_BC* pReversingCar;
    if (player_ped->get_wanted_points_433DC0() > 3000 && (player_ped->has_car_403B80()) &&
        player_ped->not_enter_car_as_passenger_4A5040() &&
        (pReversingCar = player_ped->get_car_416B60()) != 0 && // null check optimized away ??
        (pReversingCar->field_58_physics) != 0 &&

        pReversingCar->field_58_physics->is_backward_gas_on_411810())
    {
        field_19C_reverse_count += gGame_0x40_67E008->GetFrameDurationMs_4B8BB0();
    }
    else
    {
        field_19C_reverse_count = 0;
    }

    if (field_19C_reverse_count >= 60000u)
    {
        field_19C_reverse_count = 0;
        field_368_player->AddScore_41DC40(1000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("wrngway"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(10);
        }
    }

    Car_BC* pFlyingCar = player_ped->get_car_416B60();
    if (pFlyingCar && player_ped->not_enter_car_as_passenger_4A5040() && (pFlyingCar->field_58_physics) != 0 &&
        pFlyingCar->field_58_physics->IsInAir_55A0B0() // TODO: Wrong stack
        && pFlyingCar->GetVelocity_43A4C0() > kFlyCarMinVelocity_7028BC)
    {
        field_190_fly_car_count += gGame_0x40_67E008->GetFrameDurationMs_4B8BB0();
    }
    else
    {
        field_190_fly_car_count = 0;
    }

    if (field_190_fly_car_count >= 1250 && field_190_fly_car_count < 2250)
    {
        field_368_player->AddScore_41DC40(1000);
        field_190_fly_car_count = 2250;
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("fly_car"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(1);
        }
    }

    if (bStartNetworkGame_7081F0)
    {
        s32 frags_or_points; // edi
        u8 player_idx = field_368_player->get_idx_4219D0();
        u8 gamemode = gLucid_hamilton_67E8E0.GetMultiplayerGamemode_4C5BC0();
        s32 points_limit = gLucid_hamilton_67E8E0.GetMultiplayerPointsLimit_4C5BD0();

        if (gamemode == FRAG_GAME_1) // di vs bl
        {
            s16 frags = gLucid_hamilton_67E8E0.GetFragsForPlayerIdx_4C5D60(player_idx);
            frags_or_points = frags;
            GetFrags_5935C0();
        }
        else if (gamemode == POINTS_GAME_2)
        {
            frags_or_points = gLucid_hamilton_67E8E0.GetPointsForPlayerIdx_4C5CB0(player_idx);
            GetScore_592370();
        }

        if (gamemode != TAG_GAME_3)
        {
            if (frags_or_points >= points_limit) // TODO: di vs edi
            {
                gLucid_hamilton_67E8E0.SetWinnerIdx_4C5C00(player_idx);
                if (gGame_0x40_67E008->field_28_timer == -1)
                {
                    gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("g_over"), 3);
                }
                gGame_0x40_67E008->ExitGameNoBonus_4B8C00(2, GameExitType::MultiplayerExit_5);
            }
        }
    }

    // Handle the previous LABEL_63 section
    const s32 cur_rng = gpRng_67AB34->get_cur_rng_41CFE0();

    if ((u32)(gpRng_67AB34->get_cur_rng_41CFE0() - field_70_last_car_kill_time) > 15)
    {
        field_74_car_kill_combo = 1;
    }

    if ((u32)(cur_rng - field_78_last_kill_time) > 15)
    {
        field_75_score_mult = 1;
    }
}

MATCH_FUNC(0x5922f0)
void PlayerScoreTracker_36C::Init_5922F0(Player* pPlayer, s16 digit_transition_speed, s32 max_score_value, s16 palette, u16 max_frag_value)
{
    field_368_player = pPlayer;
    field_0_money.SetupDigitsParams_492110(digit_transition_speed, max_score_value, palette);
    field_38_multiplayer_frags.SetupDigitsParams_492110(digit_transition_speed, max_frag_value, palette);
}

MATCH_FUNC(0x592330)
void PlayerScoreTracker_36C::Reset_592330()
{
    field_0_money.InitDigitSprites_492150();
    field_38_multiplayer_frags.InitDigitSprites_492150();
    field_1A8_bonuses.Init_431E10(this);
    ResetCarModelFlags_592380(car_model_flag::both_3);
}

MATCH_FUNC(0x592360)
thirsty_lamarr* PlayerScoreTracker_36C::GetScoreDigits_592360()
{
    return &field_0_money;
}

MATCH_FUNC(0x592370)
s32 PlayerScoreTracker_36C::GetScore_592370()
{
    return field_0_money.field_0_value;
}

MATCH_FUNC(0x592380)
void PlayerScoreTracker_36C::ResetCarModelFlags_592380(char_type bits)
{
    if ((bits & car_model_flag::stolen_1) != 0)
    {
        for (u16 i = 0; i < GTA2_COUNTOF(field_8C_car_model_flags); i++)
        {
            if (gGtx_0x106C_703DD4->does_car_exist(i) && gGtx_0x106C_703DD4->IsCarModelInRecycleList_5AB380(i))
            {
                field_8C_car_model_flags[i] &= ~car_model_flag::stolen_1;
            }
            else
            {
                field_8C_car_model_flags[i] |= car_model_flag::stolen_1;
            }
        }
    }

    if ((bits & car_model_flag::destroyed_2) != 0)
    {
        for (u16 i = 0; i < GTA2_COUNTOF(field_8C_car_model_flags); i++)
        {
            if (gGtx_0x106C_703DD4->does_car_exist(i) && gGtx_0x106C_703DD4->IsCarModelInRecycleList_5AB380(i))
            {
                const u8 wreck = gGtx_0x106C_703DD4->get_car_info_5AA3B0(i)->wreck;

                if (wreck == 99)
                {
                    field_8C_car_model_flags[i] |= car_model_flag::destroyed_2;
                }
                else
                {
                    field_8C_car_model_flags[i] &= ~car_model_flag::destroyed_2;
                }
            }
            else
            {
                field_8C_car_model_flags[i] |= car_model_flag::destroyed_2;
            }
        }
    }
}

MATCH_FUNC(0x592430)
void PlayerScoreTracker_36C::CheckAllCarModelsFlagged_592430(char_type bits)
{
    u16 i;

    if ((bits & car_model_flag::stolen_1) != 0)
    {
        for (i = 0; i < GTA2_COUNTOF(field_8C_car_model_flags); i++)
        {
            if ((field_8C_car_model_flags[i] & car_model_flag::stolen_1) == 0)
            {
                return;
            }
        }

        field_368_player->AddScore_41DC40(30000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("stl_all"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(2);
        }
        ResetCarModelFlags_592380(car_model_flag::stolen_1);
    }
    else if ((bits & car_model_flag::destroyed_2) != 0)
    {
        for (i = 0; i < GTA2_COUNTOF(field_8C_car_model_flags); i++)
        {
            if ((field_8C_car_model_flags[i] & car_model_flag::destroyed_2) == 0)
            {
                return;
            }
        }

        field_368_player->AddScore_41DC40(50000);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("dst_all"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(3);
        }
        ResetCarModelFlags_592380(car_model_flag::destroyed_2);
    }
}

MATCH_FUNC(0x592570)
void PlayerScoreTracker_36C::SetCarModelFlag_592570(char_type flag, s32 car_model)
{
    field_8C_car_model_flags[car_model] |= flag;
    CheckAllCarModelsFlagged_592430(flag);
}

MATCH_FUNC(0x5925b0)
s32 PlayerScoreTracker_36C::GetCarScoreValue_5925B0(u32 car_model, u8 reward_tier)
{
    u32 result = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(car_model)->field_2_value;

    switch (reward_tier)
    {
        case 0:
            return result;
        case 1:
            return 2 * result;
        case 2:
            return 5 * result;
        default:
            return 0;
    }
}

MATCH_FUNC(0x592620)
void PlayerScoreTracker_36C::AddCash_592620(s32 cash)
{
    field_0_money.ChangeStatByAmount_4921B0(cash);

    if (bStartNetworkGame_7081F0)
    {
        gLucid_hamilton_67E8E0.ChangePointsForPlayerIdxByAmount_4C5C80(field_368_player->get_idx_4219D0(), cash);
    }
}

// Scores the player killing pVictim (pKiller is the killer's ped): points by occupation and kill
// type, exploding score, cash, and reports the crime
MATCH_FUNC(0x592660)
void PlayerScoreTracker_36C::AwardPedKilledScore_592660(Ped* pVictim, Ped* pKiller)
{
    const s32 multiplier = field_368_player->get_multiplier_4766A0();
    gmp_map_zone* pZone = gMap_0x370_6F6268->first_zone_by_pos_4DF6A0(pKiller->get_cam_x().ToInt(), pKiller->get_cam_y().ToInt());

    s16 gang_idx;
    if (pVictim->field_17C_pGang)
    {
        gang_idx = pVictim->field_17C_pGang->field_1_gang_idx;
    }
    else if (pVictim->field_19C_dummy_gang)
    {
        gang_idx = pVictim->field_19C_dummy_gang->field_1_gang_idx;
    }
    else
    {
        gang_idx = -1;
    }

    s32 killer_car_model;
    Car_BC* pKillerCar = pKiller->get_car_416B60();
    if (pKillerCar)
    {
        killer_car_model = pKillerCar->field_84_car_info_idx;
    }
    else
    {
        killer_car_model = car_model_enum::none;
    }
    field_1A8_bonuses.ProcessBonusEvent_4320D0(bonus_event_type::ped_killed_0,
                             car_model_enum::none,
                             pVictim->get_occupation_403980(),
                             gang_idx,
                             pVictim->get_remap_433BA0(),
                             pVictim->field_290_death_cause,
                             killer_car_model,
                             pZone);

    s32 rng = gpRng_67AB34->get_cur_rng_41CFE0();
    if ((u32)(rng - field_78_last_kill_time) > 15)
    {
        field_7C_execution_count = 1;
    }
    else
    {
        field_7C_execution_count++;
    }
    field_86_total_kills++;
    field_78_last_kill_time = rng;

    u32 score = 0;
    char_type bOtherGang = 0;
    char_type bHasGameObject = pVictim->field_168_game_object != 0;
    char_type bCop;
    char_type bArmy;
    char_type bSwat;
    char_type bFbi;
    char_type bGangA;
    char_type bGangB;

    if (bStartNetworkGame_7081F0 && pVictim->PedTypeIs_45EDE0(ped_type::player_2) && pVictim->field_15C_player)
    {
        switch (pVictim->field_290_death_cause)
        {
            case ped_death_cause::unknown_4:
            case ped_death_cause::projectile_default_9:
            case ped_death_cause::punched_10:
            case ped_death_cause::bullet_11:
            case ped_death_cause::bomb_12:
            case ped_death_cause::fire_13:
            case ped_death_cause::fire_hit_14:
            case ped_death_cause::grenade_15:
            case ped_death_cause::molotov_16:
            case ped_death_cause::rocket_bullet_17:
            case ped_death_cause::rocket_18:
            case ped_death_cause::shotgun_19:
            case ped_death_cause::burning_20:
                score = 2000;
                break;
            case ped_death_cause::run_over_1:
                score = 1000;
                break;
            case ped_death_cause::run_over_by_stolen_car_3:
                score = 10000;
                break;
            case ped_death_cause::electrocuted_2:
            case ped_death_cause::unknown_5:
                score = 5000;
                break;
        }
    }
    else
    {
        if (pVictim->field_17C_pGang && (!pKiller->field_17C_pGang || pKiller->field_17C_pGang != pVictim->field_17C_pGang))
        {
            bOtherGang = 1;
        }
        if (pVictim->field_19C_dummy_gang && (!pKiller->field_17C_pGang || pKiller->field_17C_pGang != pVictim->field_19C_dummy_gang))
        {
            bOtherGang = 1;
        }

        switch (pVictim->get_occupation_403980())
        {
            case ped_ocupation_enum::paramedic_23:
            case ped_ocupation_enum::police:
            case ped_ocupation_enum::walking_guard_29:
            case ped_ocupation_enum::roadblock_cop_37:
            case ped_ocupation_enum::fireman:
                bCop = 1;
                bSwat = 0;
                bArmy = 0;
                bFbi = 0;
                bGangA = 0;
                bGangB = 0;
                break;
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::unknown_cop_occu_30:
                bArmy = 1;
                bSwat = 0;
                bFbi = 0;
                bCop = 0;
                bGangA = 0;
                bGangB = 0;
                break;
            case ped_ocupation_enum::fbi:
                bSwat = 1;
                bArmy = 0;
                bFbi = 0;
                bCop = 0;
                bGangA = 0;
                bGangB = 0;
                break;
            case ped_ocupation_enum::army_army:
            case ped_ocupation_enum::unknown_cop_occu_31:
            case ped_ocupation_enum::tank_driver:
            case ped_ocupation_enum::road_block_tank_man:
                bFbi = 1;
                bSwat = 0;
                bArmy = 0;
                bCop = 0;
                bGangA = 0;
                bGangB = 0;
                break;
            case ped_ocupation_enum::elvis:
            case ped_ocupation_enum::elvis_leader:
                bCop = 0;
                bSwat = 0;
                bArmy = 0;
                bFbi = 0;
                score = 100;
                break;
            case ped_ocupation_enum::mugger:
                bGangA = 1;
                bCop = 0;
                bSwat = 0;
                bArmy = 0;
                bFbi = 0;
                bGangB = 0;
                break;
            case ped_ocupation_enum::car_thief:
                bGangB = 1;
                bCop = 0;
                bSwat = 0;
                bArmy = 0;
                bFbi = 0;
                bGangA = 0;
                break;
            default:
                bCop = 0;
                bSwat = 0;
                bArmy = 0;
                bFbi = 0;
                bGangA = 0;
                bGangB = 0;
                break;
        }

        // Only the Elvis case has a score by now
        if (score)
        {
            if ((u32)(rng - field_80_last_elvis_kill_time) > 15)
            {
                field_84_num_elvis_killed = 1;
            }
            else
            {
                field_84_num_elvis_killed++;
            }
            field_80_last_elvis_kill_time = rng;
        }
        else
        {
            switch (pVictim->field_290_death_cause)
            {
                case ped_death_cause::projectile_default_9:
                case ped_death_cause::punched_10:
                case ped_death_cause::bullet_11:
                case ped_death_cause::bomb_12:
                case ped_death_cause::fire_13:
                case ped_death_cause::fire_hit_14:
                case ped_death_cause::grenade_15:
                case ped_death_cause::molotov_16:
                case ped_death_cause::rocket_bullet_17:
                case ped_death_cause::rocket_18:
                case ped_death_cause::shotgun_19:
                case ped_death_cause::burning_20:
                    if (bOtherGang)
                        score = 50;
                    else if (bCop)
                        score = 200;
                    else if (bFbi)
                        score = 500;
                    else if (bArmy)
                        score = 300;
                    else if (bSwat)
                        score = 400;
                    else if (bGangA)
                        score = 40;
                    else
                        score = bGangB ? 40 : 20;
                    break;
                case ped_death_cause::run_over_1:
                    if (bOtherGang)
                        score = 20;
                    else if (bCop)
                        score = 100;
                    else if (bFbi)
                        score = 250;
                    else if (bArmy)
                        score = 150;
                    else if (bSwat)
                        score = 200;
                    else if (bGangA)
                        score = 20;
                    else
                        score = bGangB ? 20 : 10;
                    break;
                case ped_death_cause::electrocuted_2:
                    if (bOtherGang)
                        score = 200;
                    else if (bCop)
                        score = 500;
                    else if (bFbi)
                        score = 1250;
                    else if (bArmy)
                        score = 750;
                    else if (bSwat)
                        score = 1000;
                    else if (bGangA)
                        score = 100;
                    else
                        score = bGangB ? 100 : 50;
                    break;
                case ped_death_cause::run_over_by_stolen_car_3:
                    if (bOtherGang)
                        score = 200;
                    else if (bCop)
                        score = 1000;
                    else if (bFbi)
                        score = 2500;
                    else if (bArmy)
                        score = 1500;
                    else if (bSwat)
                        score = 2000;
                    else if (bGangA)
                        score = 200;
                    else
                        score = bGangB ? 200 : 100;
                    break;
                case ped_death_cause::unknown_4:
                    score = 20;
                    break;
                case ped_death_cause::unknown_5:
                    score = 50;
                    break;
            }
        }
    }

    char_type bAwardScore = 1;
    if (bIsFrench_67D53C)
    {
        s32 occupation = pVictim->get_occupation_403980();
        if (occupation == ped_ocupation_enum::police || occupation == ped_ocupation_enum::walking_guard_29 || occupation == ped_ocupation_enum::roadblock_cop_37 || bSwat || bArmy || bFbi)
        {
            bAwardScore = 0;
        }
    }

    if (score > 0)
    {
        u32 total = (u8)field_75_score_mult * score;
        if (!bExplodingScoresOff_67D4FB && bHasGameObject && bAwardScore && field_368_player->IsUser_41DC70())
        {
            gExplodingScorePool->PushScore_596890(pVictim->get_cam_x(), pVictim->get_cam_y(), pVictim->get_cam_z(), total * multiplier);
        }
        if (bAwardScore)
        {
            field_368_player->AddScore_41DC40(total);
        }
        if ((u8)field_75_score_mult < 5)
        {
            field_75_score_mult++;
        }
    }

    if (gCrimeReportQueue_67A4B8->ShouldReportPedCrime_485140(pVictim, field_368_player))
    {
        if (bOtherGang)
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::Gang_members_killed_9, field_368_player->GetPlayerPed_4A5130());
        }
        else if (bCop || bFbi || bArmy || bSwat)
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::Lawmen_killed_8, field_368_player->GetPlayerPed_4A5130());
        }
        else if (pVictim->field_290_death_cause == ped_death_cause::run_over_1 || pVictim->field_290_death_cause == ped_death_cause::run_over_by_stolen_car_3)
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::Civilians_run_down_6, field_368_player->GetPlayerPed_4A5130());
        }
        else
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::Civilians_murdered_7, field_368_player->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x592dd0)
void PlayerScoreTracker_36C::AwardCarDestroyedScore_592DD0(Car_BC* pCar, Ped* pKiller)
{
    const s32 multiplier = field_368_player->get_multiplier_4766A0();
    gmp_map_zone* pZone = gMap_0x370_6F6268->first_zone_by_pos_4DF6A0(pKiller->get_cam_x().ToInt(), pKiller->get_cam_y().ToInt());

    u32 killer_car_model = pKiller->get_car_model();

    u16 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pCar->field_84_car_info_idx);

    field_1A8_bonuses.ProcessBonusEvent_4320D0(bonus_event_type::car_destroyed_1,
                             pCar->field_84_car_info_idx,
                             ped_ocupation_enum::no_occupation,
                             gang_idx,
                             pCar->field_50_car_sprite->get_remap_41C1F0(),
                             pCar->field_90,
                             killer_car_model,
                             pZone);

    u8 bAwardScore = 1;
    if (bIsFrench_67D53C)
    {
        if (pCar->IsPoliceCar_439EC0())
        {
            bAwardScore = 0;
        }
    }

    s32 cur_rng = gpRng_67AB34->get_cur_rng_41CFE0();
    if (pCar->IsFireTruck_4118F0() || pCar->IsCopCar_421790() ||
        pCar->IsMediCar() || pCar->IsSwatVan_4217A0() ||
        pCar->is_FBI_car_411920())
    {

        if ((unsigned int)(gpRng_67AB34->get_cur_rng_41CFE0() - field_1A0_last_emergency_car_kill_time) > 150)
        {
            field_1A4_killed_cars_flags = 0;
        }

        u32 destroyed_car_model = pCar->field_84_car_info_idx;

        if (destroyed_car_model == car_model_enum::MEDICAR)
        {
            field_1A4_killed_cars_flags |= 1;
        }
        else if (destroyed_car_model == car_model_enum::COPCAR || destroyed_car_model == car_model_enum::SWATVAN || destroyed_car_model == car_model_enum::EDSELFBI)
        {
            field_1A4_killed_cars_flags |= 2;
        }
        else if (destroyed_car_model == car_model_enum::FIRETRUK)
        {
            field_1A4_killed_cars_flags |= 4;
        }

        field_1A0_last_emergency_car_kill_time = cur_rng;
    }
    if (pCar->IsCopCar_421790() && bAwardScore)
    {
        field_88_killed_cops++;
    }

    u32 car_score_value = GetCarScoreValue_5925B0(pCar->field_84_car_info_idx, 2);
    u8 combo = field_74_car_kill_combo;

    u32 kill_car_score = car_score_value * combo;
    if (!bExplodingScoresOff_67D4FB)
    {
        if (bAwardScore)
        {
            if (field_368_player->IsUser_41DC70())
            {
                gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                                       pCar->get_y_41E440(),
                                                       pCar->get_z_41E450(),
                                                       multiplier * kill_car_score);
            }
        }
    }

    if (bAwardScore)
    {
        field_368_player->AddScore_41DC40(kill_car_score);
    }

    field_70_last_car_kill_time = cur_rng;
    if (field_74_car_kill_combo < 5u)
    {
        field_74_car_kill_combo++;
    }

    if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime_485090(pCar, this->field_368_player))
    {
        gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::car_destroyed_3, field_368_player->GetPlayerPed_4A5130());
    }
    field_368_player->field_644_crime_stats.AddCarDamageCost_484FA0(multiplier * kill_car_score);
    SetCarModelFlag_592570(car_model_flag::destroyed_2, pCar->field_84_car_info_idx);
}

MATCH_FUNC(0x593030)
void PlayerScoreTracker_36C::AwardCarDamageScore_593030(Car_BC* pCar, s16 damage)
{
    bool bAddScore = true;
    s32 multiplier = field_368_player->get_multiplier_4766A0();

    if (bIsFrench_67D53C)
    {
        if (pCar->IsPoliceCar_439EC0())
        {
            bAddScore = false;
        }
    }

    u32 damage_u = damage;
    if (damage_u > 0)
    {
        int base_score;
        if (damage_u < 300)
        {
            base_score = 1;
        }
        else
        {
            base_score = damage_u < 400 ? 10 : 100;
        }

        if (!bExplodingScoresOff_67D4FB)
        {
            if (bAddScore)
            {
                if (this->field_368_player->IsUser_41DC70())
                {
                    gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                                           pCar->get_y_41E440(),
                                                           pCar->get_z_41E450(),
                                                           multiplier * base_score);
                }
            }
        }

        if (bAddScore)
        {
            field_368_player->AddScore_41DC40(base_score);
        }

        field_368_player->field_644_crime_stats.AddCarDamageCost_484FA0(multiplier * base_score);
        if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime_485090(pCar, field_368_player))
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::car_damaged_1, field_368_player->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x593150)
void PlayerScoreTracker_36C::AwardCarDamageScoreHit_593150(Car_BC* pCar, s16 damage)
{
    if (!pCar->IsMaxDamage_40F890())
    {
        const s32 multiplier = field_368_player->get_multiplier_4766A0();
        u32 damage_u = damage;
        if (damage_u > 0)
        {
            s32 base_score;
            if (damage_u < 300)
            {
                base_score = 1;
            }
            else
            {
                base_score = damage_u < 400 ? 10 : 100;
            }
            if (!bIsFrench_67D53C || !pCar->IsPoliceCar_439EC0())
            {
                field_368_player->AddScore_41DC40(base_score);
            }
            field_368_player->field_644_crime_stats.AddCarDamageCost_484FA0(multiplier * base_score);

            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::car_damaged_1, field_368_player->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x593220)
void PlayerScoreTracker_36C::AddCashWithMultiplier_593220()
{
    field_368_player->field_2D4_scores.AddCash_592620(field_368_player->field_6BC_multpliers.field_0_value * 20);
}

MATCH_FUNC(0x593240)
void PlayerScoreTracker_36C::AwardCarHijackedScore_593240(Car_BC* pCar)
{
    const s32 multiplier = field_368_player->get_multiplier_4766A0();
    gmp_map_zone* pZone = gMap_0x370_6F6268->first_zone_by_pos_4DF6A0(field_368_player->field_2C4_player_ped->get_cam_x().ToInt(),
                                                           field_368_player->field_2C4_player_ped->get_cam_y().ToInt());

    const u16 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pCar->field_84_car_info_idx);
    field_1A8_bonuses.ProcessBonusEvent_4320D0(bonus_event_type::car_hijacked_2, pCar->field_84_car_info_idx, ped_ocupation_enum::no_occupation, gang_idx, pCar->field_50_car_sprite->get_remap_41C1F0(), ped_death_cause::any_23, car_model_enum::none, pZone);

    field_8A_cars_stolen_count++;

    const s32 base_score = GetCarScoreValue_5925B0(pCar->field_84_car_info_idx, 0);
    if (!bExplodingScoresOff_67D4FB && field_368_player->IsUser_41DC70())
    {
        gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                               pCar->get_y_41E440(),
                                               pCar->get_z_41E450(),
                                               multiplier * base_score);
    }
    field_368_player->AddScore_41DC40(base_score);

    gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::Vehicles_Hijacked_5, field_368_player->GetPlayerPed_4A5130());
    SetCarModelFlag_592570(car_model_flag::stolen_1, pCar->field_84_car_info_idx);
}

MATCH_FUNC(0x593370)
void PlayerScoreTracker_36C::AwardBusStolenScore_593370(Car_BC* pCar)
{
    if (!bExplodingScoresOff_67D4FB && field_368_player->IsUser_41DC70())
    {
        gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                               pCar->get_y_41E440(),
                                               pCar->get_z_41E450(),
                                               field_368_player->get_multiplier_4766A0() * 10);
    }

    field_368_player->AddScore_41DC40(10);
    gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::bus_stolen_4, field_368_player->GetPlayerPed_4A5130());
}

MATCH_FUNC(0x593410)
void PlayerScoreTracker_36C::AwardFullBusDestroyedScore_593410(Car_BC* pCar)
{
    const s32 multiplier = field_368_player->get_multiplier_4766A0();
    if (!bExplodingScoresOff_67D4FB)
    {
        if (field_368_player->IsUser_41DC70())
        {
            gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                                   pCar->get_y_41E440(),
                                                   pCar->get_z_41E450(),
                                                   100 * multiplier);
        }
    }

    field_368_player->AddScore_41DC40(100);
    field_368_player->field_644_crime_stats.AddCarDamageCost_484FA0(100 * multiplier);

    if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime_485090(pCar, field_368_player))
    {
        gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::car_destroyed_3, field_368_player->GetPlayerPed_4A5130());
    }
}

MATCH_FUNC(0x5934f0)
void PlayerScoreTracker_36C::UpdateAccuracyCount_5934F0(u32 shot_result, s32 weapon_model, Ped* pTarget)
{
    field_194_last_shot_result = shot_result;
    if (pTarget && bIsFrench_67D53C)
    {
        switch (pTarget->get_occupation_403980())
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
                field_198_accuracy_count = 0;
                return;
            default:
                break;
        }
    }

    if (weapon_model == objects::fire_hitting_194 || weapon_model == objects::maybe_bullet_on_fire_198 || weapon_model == objects::flamethrower_fire_154 || weapon_model == objects::tanktop_193 || weapon_model == objects::object_195 || weapon_model == objects::object_159 || weapon_model == objects::object_199 || shot_result == 0 ||
        shot_result == 1 || shot_result == 3)
    {
        field_198_accuracy_count = 0;
    }
    else if (shot_result == 2)
    {
        field_198_accuracy_count++;
    }
}

MATCH_FUNC(0x5935b0)
thirsty_lamarr* PlayerScoreTracker_36C::GetMultiplayerFragDigits_5935B0()
{
    return &field_38_multiplayer_frags;
}

MATCH_FUNC(0x5935c0)
s32 PlayerScoreTracker_36C::GetFrags_5935C0()
{
    return field_38_multiplayer_frags.field_0_value;
}

MATCH_FUNC(0x5935d0)
void PlayerScoreTracker_36C::ChangeFragsByAmount_5935D0(s32 amount)
{
    field_38_multiplayer_frags.ChangeStatByAmount_4921B0(amount);
}