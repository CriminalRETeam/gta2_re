#include "eager_benz.hpp"
#include "CarInfo_808.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "ExplodingScore_100.hpp"
#include "Game_0x40.hpp"
#include "Hud.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "Shooey_CC.hpp"
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
EXTERN_GLOBAL(Shooey_CC*, gShooey_CC_67A4B8);

DEFINE_GLOBAL_INIT(Fix16, dword_7028BC, Fix16(0x666, 0), 0x7028BC);


MATCH_FUNC(0x591bd0)
eager_benz::eager_benz()
{
    field_368_player = 0;

    field_74 = 1;
    field_75_score_mult = 1;

    field_18C = 0;
    field_190_fly_car_count = 0;
    field_70 = 0;
    field_78 = 0;
    field_7C_e_execution_count = 0;
    field_86_total_kills = 0;
    field_88_killed_cops = 0;
    field_8A_cars_stolen_count = 0;
    field_80 = 0;
    field_84_num_elvis_killed = 0;
    field_194 = 0;
    field_198_accuracy_count = 0;
    field_19C_reverse_count = 0;
    field_1A0 = 0;
    field_1A4_killed_cars_flags = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_8C); i++)
    {
        field_8C[i] = 0;
    }
}

MATCH_FUNC(0x591c70)
void eager_benz::sub_591C70()
{
    field_1A8_unk.sub_431E30();
    Ped* player_ped = field_368_player->GetPlayerPed_41D020();
    field_18C += gGame_0x40_67E008->sub_4B8BB0();

    if (field_18C >= 1000)
    {
        field_18C -= 1000;
        Car_BC* field_16C_car = player_ped->field_16C_car;

        if (field_16C_car)
        {
            if (player_ped->field_248_enter_car_as_passenger != 1 &&
                (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_16C_car->field_84_car_info_idx)->info_flags & 0x20) == 0x20)
            {
                if (field_16C_car->field_4_passengers_list.field_0_pFirstPed)
                {
                    field_368_player->field_2D4_scores.AddCash_592620(field_368_player->field_6BC_multpliers.field_0_value);
                }
            }
        }
        if (player_ped->field_20A_wanted_points >= 5000)
        {
            field_368_player->field_2D4_scores.AddCash_592620(field_368_player->field_6BC_multpliers.field_0_value);
        }

        field_368_player->field_644_unk.sub_484FB0(player_ped->get_wanted_star_count_46EF00());
    }

    if (field_7C_e_execution_count >= 20u)
    {
        field_7C_e_execution_count = 0;
        field_368_player->field_2D4_scores.AddCash_592620(100000 * field_368_player->field_6BC_multpliers.field_0_value);

        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("excutin"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(4);
        }
    }

    if (field_84_num_elvis_killed >= 6u)
    {
        field_84_num_elvis_killed = 0;
        field_368_player->field_2D4_scores.AddCash_592620(30000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("elvis_d"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(8);
        }
    }

    if (field_1A4_killed_cars_flags == 7)
    {
        field_1A4_killed_cars_flags = 0;
        field_368_player->field_2D4_scores.AddCash_592620(10000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("em_dest"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(11);
        }
    }

    if (field_86_total_kills >= 1000)
    {
        field_86_total_kills = 0;
        field_368_player->field_2D4_scores.AddCash_592620(30000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("gencide"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(5);
        }
    }

    if (field_88_killed_cops >= 20u)
    {
        field_88_killed_cops = 0;
        field_368_player->field_2D4_scores.AddCash_592620(5000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("copkill"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(6);
        }
    }

    if (field_8A_cars_stolen_count >= 100)
    {
        field_8A_cars_stolen_count = 0;
        field_368_player->field_2D4_scores.AddCash_592620(10000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("carjaka"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(7);
        }
    }

    if (field_198_accuracy_count >= 25)
    {
        field_198_accuracy_count = 0;
        field_368_player->field_2D4_scores.AddCash_592620(5000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("accurcy"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(9);
        }
    }

    Car_BC* tmp3;
    if (player_ped->get_wanted_points_433DC0() > 3000 && (player_ped->has_car_403B80()) &&
        player_ped->not_enter_car_as_passenger_4A5040() &&
        (tmp3 = player_ped->get_car_416B60()) != 0 && // null check optimized away ??
        (tmp3->field_58_physics) != 0 &&

        tmp3->field_58_physics->is_backward_gas_on_411810())
    {
        field_19C_reverse_count += gGame_0x40_67E008->sub_4B8BB0();
    }
    else
    {
        field_19C_reverse_count = 0;
    }

    if (field_19C_reverse_count >= 60000u)
    {
        field_19C_reverse_count = 0;
        field_368_player->field_2D4_scores.AddCash_592620(1000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("wrngway"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(10);
        }
    }

    Car_BC* v24 = player_ped->get_car_416B60();
    if (v24 && player_ped->not_enter_car_as_passenger_4A5040() && (v24->field_58_physics) != 0 &&
        v24->field_58_physics->IsInAir_55A0B0() // TODO: Wrong stack
        && v24->GetVelocity_43A4C0() > dword_7028BC)
    {
        field_190_fly_car_count += gGame_0x40_67E008->sub_4B8BB0();
    }
    else
    {
        field_190_fly_car_count = 0;
    }

    if (field_190_fly_car_count >= 1250 && field_190_fly_car_count < 2250)
    {
        field_368_player->field_2D4_scores.AddCash_592620(1000 * field_368_player->field_6BC_multpliers.field_0_value);
        field_190_fly_car_count = 2250;
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("fly_car"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(1);
        }
    }

    if (bStartNetworkGame_7081F0)
    {
        s32 v30; // edi
        u8 player_idx = field_368_player->get_idx_4219D0();
        u8 v29 = gLucid_hamilton_67E8E0.GetMultiplayerGamemode_4C5BC0();
        s32 v34 = gLucid_hamilton_67E8E0.GetMultiplayerPointsLimit_4C5BD0();

        if (v29 == 1) // di vs bl
        {
            s16 t = gLucid_hamilton_67E8E0.GetFragsForPlayerIdx_4C5D60(player_idx);
            v30 = t;
            sub_5935C0();
        }
        else if (v29 == 2)
        {
            v30 = gLucid_hamilton_67E8E0.GetPointsForPlayerIdx_4C5CB0(player_idx);
            GetScore_592370();
        }

        if (v29 != 3)
        {
            if (v30 >= v34) // TODO: di vs edi
            {
                gLucid_hamilton_67E8E0.sub_4C5C00(player_idx);
                if (gGame_0x40_67E008->field_28_timer == -1)
                {
                    gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("g_over"), 3);
                }
                gGame_0x40_67E008->ExitGameNoBonus_4B8C00(2, GameExitType::MultiplayerExit_5);
            }
        }
    }

    // Handle the previous LABEL_63 section
    const s32 field_0_rng = rng_dword_67AB34->field_0_rng; // TODO: inline

    if ((u32)(rng_dword_67AB34->field_0_rng - field_70) > 15)
    {
        field_74 = 1;
    }

    if ((u32)(field_0_rng - field_78) > 15)
    {
        field_75_score_mult = 1;
    }
}

MATCH_FUNC(0x5922f0)
void eager_benz::sub_5922F0(Player* pPlayer, s16 digit_transition_speed, s32 max_score_value, s16 palette, u16 max_frag_value)
{
    field_368_player = pPlayer;
    field_0_money.SetupDigitsParams_492110(digit_transition_speed, max_score_value, palette);
    field_38_multiplayer_frags.SetupDigitsParams_492110(digit_transition_speed, max_frag_value, palette);
}

MATCH_FUNC(0x592330)
void eager_benz::sub_592330()
{
    field_0_money.sub_492150();
    field_38_multiplayer_frags.sub_492150();
    field_1A8_unk.sub_431E10(this);
    sub_592380(3);
}

MATCH_FUNC(0x592360)
thirsty_lamarr* eager_benz::GetScoreDigits_592360()
{
    return &field_0_money;
}

MATCH_FUNC(0x592370)
s32 eager_benz::GetScore_592370()
{
    return field_0_money.field_0_value;
}

MATCH_FUNC(0x592380)
void eager_benz::sub_592380(char_type bits)
{
    if ((bits & 1) != 0)
    {
        for (u16 i = 0; i < 256; i++)
        {
            if (gGtx_0x106C_703DD4->does_car_exist(i) && gGtx_0x106C_703DD4->IsCarModelInRecycleList_5AB380(i))
            {
                field_8C[i] &= ~1;
            }
            else
            {
                field_8C[i] |= 1;
            }
        }
    }

    if ((bits & 2) != 0)
    {
        for (u16 i = 0; i < 256; i++)
        {
            if (gGtx_0x106C_703DD4->does_car_exist(i) && gGtx_0x106C_703DD4->IsCarModelInRecycleList_5AB380(i))
            {
                const u8 wreck = gGtx_0x106C_703DD4->get_car_info_5AA3B0(i)->wreck;

                if (wreck == 99)
                {
                    field_8C[i] |= 2;
                }
                else
                {
                    field_8C[i] &= ~2;
                }
            }
            else
            {
                field_8C[i] |= 2;
            }
        }
    }
}

MATCH_FUNC(0x592430)
void eager_benz::sub_592430(char_type bits)
{
    u16 i;

    if ((bits & 1) != 0)
    {
        for (i = 0; i < GTA2_COUNTOF(field_8C); i++)
        {
            if ((field_8C[i] & 1) == 0)
            {
                return;
            }
        }

        field_368_player->field_2D4_scores.AddCash_592620(30000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("stl_all"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(2);
        }
        sub_592380(1);
    }
    else if ((bits & 2) != 0)
    {
        for (i = 0; i < GTA2_COUNTOF(field_8C); i++)
        {
            if ((field_8C[i] & 2) == 0)
            {
                return;
            }
        }

        field_368_player->field_2D4_scores.AddCash_592620(50000 * field_368_player->field_6BC_multpliers.field_0_value);
        if (field_368_player->IsUser_41DC70())
        {
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("dst_all"), 1);
            gRoot_sound_66B038.PlayVoice_40F090(3);
        }
        sub_592380(2);
    }
}

MATCH_FUNC(0x592570)
void eager_benz::sub_592570(char_type a2, s32 a3)
{
    field_8C[a3] |= a2;
    sub_592430(a2);
}

MATCH_FUNC(0x5925b0)
s32 eager_benz::sub_5925B0(u32 car_info_idx, u8 arg4)
{
    u32 result = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(car_info_idx)->field_2_value;

    switch (arg4)
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
void eager_benz::AddCash_592620(s32 cash)
{
    field_0_money.ChangeStatByAmount_4921B0(cash);

    if (bStartNetworkGame_7081F0)
    {
        gLucid_hamilton_67E8E0.ChangePointsForPlayerIdxByAmount_4C5C80(field_368_player->get_idx_4219D0(), cash);
    }
}

// Scores the player killing pPed1 (pPed2 is the killer's ped): points by occupation and kill
// type, exploding score, cash, and reports the crime
WIP_FUNC(0x592660)
void eager_benz::sub_592660(Ped* pPed1, Ped* pPed2)
{
    const s32 multipler = field_368_player->get_multiplier_4766A0();
    gmp_map_zone* pZone = gMap_0x370_6F6268->sub_4DF6A0(pPed2->get_cam_x().ToInt(), pPed2->get_cam_y().ToInt());

    s16 gang_idx;
    if (pPed1->field_17C_pGang)
    {
        gang_idx = pPed1->field_17C_pGang->field_1_gang_idx;
    }
    else if (pPed1->field_19C)
    {
        gang_idx = pPed1->field_19C->field_1_gang_idx;
    }
    else
    {
        gang_idx = -1;
    }

    field_1A8_unk.sub_4320D0(0,
                             87,
                             pPed1->get_occupation_403980(),
                             gang_idx,
                             pPed1->get_remap_433BA0(),
                             pPed1->field_290,
                             pPed2->get_car_model(),
                             pZone);

    s32 rng = rng_dword_67AB34->get_cur_rng_41CFE0();
    if ((u32)(rng - field_78) > 15)
    {
        field_7C_e_execution_count = 1;
    }
    else
    {
        field_7C_e_execution_count++;
    }
    field_86_total_kills++;
    field_78 = rng;

    u32 score = 0;
    char_type bOtherGang = 0;
    char_type bHasB4 = pPed1->field_168_game_object != 0;
    char_type bCop;
    char_type bArmy;
    char_type bSwat;
    char_type bFbi;
    char_type bGangA;
    char_type bGangB;

    if (bStartNetworkGame_7081F0 && pPed1->IsField238_45EDE0(2) && pPed1->field_15C_player)
    {
        switch (pPed1->field_290)
        {
            case 1:
                score = 1000;
                break;
            case 2:
            case 5:
                score = 5000;
                break;
            case 3:
                score = 10000;
                break;
            case 4:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
                score = 2000;
                break;
        }
    }
    else
    {
        if (pPed1->field_17C_pGang && (!pPed2->field_17C_pGang || pPed2->field_17C_pGang != pPed1->field_17C_pGang))
        {
            bOtherGang = 1;
        }
        if (pPed1->field_19C && (!pPed2->field_17C_pGang || pPed2->field_17C_pGang != pPed1->field_19C))
        {
            bOtherGang = 1;
        }

        switch (pPed1->get_occupation_403980())
        {
            case 23:
            case 24:
            case 29:
            case 37:
            case 38:
                bCop = 1;
                bSwat = 0;
                bArmy = 0;
                bFbi = 0;
                bGangA = 0;
                bGangB = 0;
                break;
            case 25:
            case 30:
                bArmy = 1;
                bSwat = 0;
                bFbi = 0;
                bCop = 0;
                bGangA = 0;
                bGangB = 0;
                break;
            case 26:
                bSwat = 1;
                bArmy = 0;
                bFbi = 0;
                bCop = 0;
                bGangA = 0;
                bGangB = 0;
                break;
            case 27:
            case 31:
            case 36:
            case 39:
                bFbi = 1;
                bSwat = 0;
                bArmy = 0;
                bCop = 0;
                bGangA = 0;
                bGangB = 0;
                break;
            case 22:
            case 44:
                bFbi = 0;
                bCop = 0;
                bSwat = 0;
                bArmy = 0;
                score = 100;
                if ((u32)(rng - field_80) > 15)
                {
                    field_84_num_elvis_killed = 1;
                }
                else
                {
                    field_84_num_elvis_killed++;
                }
                field_80 = rng;
                // The original jumps straight to the scoring below, past the kill type switch
                goto scored;
            case 15:
                bGangA = 1;
                bCop = 0;
                bSwat = 0;
                bArmy = 0;
                bFbi = 0;
                bGangB = 0;
                break;
            case 16:
                bFbi = 0;
                bGangB = 1;
                bCop = 0;
                bSwat = 0;
                bArmy = 0;
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

        switch (pPed1->field_290)
        {
            case 1:
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
            case 2:
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
            case 3:
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
            case 4:
                score = 20;
                break;
            case 5:
                score = 50;
                break;
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
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
        }
    }

scored:
    char_type bGiveScore = 1;
    if (bIsFrench_67D53C)
    {
        s32 occupation = pPed1->get_occupation_403980();
        if (occupation == 24 || occupation == 29 || occupation == 37 || bSwat || bArmy || bFbi)
        {
            bGiveScore = 0;
        }
    }

    if (score > 0)
    {
        u32 total = (u8)field_75_score_mult * score;
        if (!bExplodingScoresOff_67D4FB && bHasB4 && bGiveScore && field_368_player->IsUser_41DC70())
        {
            gExplodingScorePool->PushScore_596890(pPed1->get_cam_x(), pPed1->get_cam_y(), pPed1->get_cam_z(), total * multipler);
        }
        if (bGiveScore)
        {
            field_368_player->Add_2D4(total);
        }
        if ((u8)field_75_score_mult < 5)
        {
            field_75_score_mult++;
        }
    }

    if (gShooey_CC_67A4B8->sub_485140(pPed1, field_368_player))
    {
        if (bOtherGang)
        {
            gShooey_CC_67A4B8->ReportCrimeForPed(9, field_368_player->GetPlayerPed_4A5130());
        }
        else if (bCop || bFbi || bArmy || bSwat)
        {
            gShooey_CC_67A4B8->ReportCrimeForPed(8, field_368_player->GetPlayerPed_4A5130());
        }
        else if (pPed1->field_290 == 1 || pPed1->field_290 == 3)
        {
            gShooey_CC_67A4B8->ReportCrimeForPed(6, field_368_player->GetPlayerPed_4A5130());
        }
        else
        {
            gShooey_CC_67A4B8->ReportCrimeForPed(7, field_368_player->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x592dd0)
void eager_benz::sub_592DD0(Car_BC* pCar, Ped* pPed)
{
    const s32 multipler = field_368_player->get_multiplier_4766A0();
    gmp_map_zone* pZone = gMap_0x370_6F6268->sub_4DF6A0(pPed->get_cam_x().ToInt(), pPed->get_cam_y().ToInt());

    u32 car_info_idx = pPed->get_car_model();

    u16 bIsGangCar = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pCar->field_84_car_info_idx);

    field_1A8_unk.sub_4320D0(1,
                             pCar->field_84_car_info_idx,
                             51,
                             bIsGangCar,
                             pCar->field_50_car_sprite->get_remap_41C1F0(),
                             pCar->field_90,
                             car_info_idx,
                             pZone);

    u8 bCopSwatOrFbiCar = 1;
    if (bIsFrench_67D53C)
    {
        if (pCar->IsPoliceCar_439EC0())
        {
            bCopSwatOrFbiCar = 0;
        }
    }

    s32 cur_rng_2 = rng_dword_67AB34->get_cur_rng_41CFE0();
    if (pCar->IsFireTruck_4118F0() || pCar->IsCopCar_421790() ||
        pCar->IsMediCar() || pCar->IsSwatVan_4217A0() ||
        pCar->is_FBI_car_411920())
    {

        if ((unsigned int)(rng_dword_67AB34->get_cur_rng_41CFE0() - field_1A0) > 150)
        {
            field_1A4_killed_cars_flags = 0;
        }

        u32 car_type = pCar->field_84_car_info_idx;

        if (car_type == car_model_enum::MEDICAR)
        {
            field_1A4_killed_cars_flags |= 1;
        }
        else if (car_type == car_model_enum::COPCAR || car_type == car_model_enum::SWATVAN || car_type == car_model_enum::EDSELFBI)
        {
            field_1A4_killed_cars_flags |= 2;
        }
        else if (car_type == car_model_enum::FIRETRUK)
        {
            field_1A4_killed_cars_flags |= 4;
        }

        field_1A0 = cur_rng_2;
    }
    if (pCar->IsCopCar_421790() && bCopSwatOrFbiCar)
    {
        field_88_killed_cops++;
    }

    u32 tt = sub_5925B0(pCar->field_84_car_info_idx, 2);
    u8 t = field_74;

    u32 kill_car_score = tt * t;
    if (!bExplodingScoresOff_67D4FB)
    {
        if (bCopSwatOrFbiCar)
        {
            if (field_368_player->IsUser_41DC70())
            {
                gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                                       pCar->get_y_41E440(),
                                                       pCar->get_z_41E450(),
                                                       multipler * kill_car_score);
            }
        }
    }

    if (bCopSwatOrFbiCar)
    {
        field_368_player->Add_2D4(kill_car_score);
    }

    field_70 = cur_rng_2;
    if (field_74 < 5u)
    {
        field_74++;
    }

    if (gShooey_CC_67A4B8->sub_485090(pCar, this->field_368_player))
    {
        gShooey_CC_67A4B8->ReportCrimeForPed(3u, field_368_player->GetPlayerPed_4A5130());
    }
    field_368_player->field_644_unk.sub_484FA0(multipler * kill_car_score);
    sub_592570(2, pCar->field_84_car_info_idx);
}

MATCH_FUNC(0x593030)
void eager_benz::sub_593030(Car_BC* pCar, s16 score_default)
{
    bool bAddScore = true;
    s32 mutipler = field_368_player->get_multiplier_4766A0();

    if (bIsFrench_67D53C)
    {
        if (pCar->IsPoliceCar_439EC0())
        {
            bAddScore = false;
        }
    }

    u32 score_default_2 = score_default;
    if (score_default_2 > 0)
    {
        int base_score;
        if (score_default_2 < 300)
        {
            base_score = 1;
        }
        else
        {
            base_score = score_default_2 < 400 ? 10 : 100;
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
                                                           mutipler * base_score);
                }
            }
        }

        if (bAddScore)
        {
            field_368_player->Add_2D4(base_score);
        }

        field_368_player->field_644_unk.sub_484FA0(mutipler * base_score);
        if (gShooey_CC_67A4B8->sub_485090(pCar, field_368_player))
        {
            gShooey_CC_67A4B8->ReportCrimeForPed(1u, field_368_player->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x593150)
void eager_benz::sub_593150(Car_BC* pCar, s16 a3)
{
    if (!pCar->IsMaxDamage_40F890())
    {
        const s32 multipler = field_368_player->get_multiplier_4766A0();
        u32 t = a3;
        if (t > 0)
        {
            s32 base_score;
            if (t < 300)
            {
                base_score = 1;
            }
            else
            {
                base_score = t < 400 ? 10 : 100;
            }
            if (!bIsFrench_67D53C || !pCar->IsPoliceCar_439EC0())
            {
                field_368_player->Add_2D4(base_score);
            }
            field_368_player->field_644_unk.sub_484FA0(multipler * base_score);

            gShooey_CC_67A4B8->ReportCrimeForPed(1u, field_368_player->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x593220)
void eager_benz::sub_593220()
{
    field_368_player->field_2D4_scores.AddCash_592620(field_368_player->field_6BC_multpliers.field_0_value * 20);
}

MATCH_FUNC(0x593240)
void eager_benz::sub_593240(Car_BC* pCar)
{
    const s32 multipler = field_368_player->get_multiplier_4766A0();
    gmp_map_zone* pMapZone = gMap_0x370_6F6268->sub_4DF6A0(field_368_player->field_2C4_player_ped->get_cam_x().ToInt(),
                                                           field_368_player->field_2C4_player_ped->get_cam_y().ToInt());

    const u16 zone_ret = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pCar->field_84_car_info_idx);
    field_1A8_unk.sub_4320D0(2, pCar->field_84_car_info_idx, 51, zone_ret, pCar->field_50_car_sprite->get_remap_41C1F0(), 23, 87, pMapZone);

    field_8A_cars_stolen_count++;

    const s32 base_score = sub_5925B0(pCar->field_84_car_info_idx, 0);
    if (!bExplodingScoresOff_67D4FB && field_368_player->IsUser_41DC70())
    {
        gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                               pCar->get_y_41E440(),
                                               pCar->get_z_41E450(),
                                               multipler * base_score);
    }
    field_368_player->Add_2D4(base_score);

    gShooey_CC_67A4B8->ReportCrimeForPed(5u, field_368_player->GetPlayerPed_4A5130());
    sub_592570(1, pCar->field_84_car_info_idx);
}

MATCH_FUNC(0x593370)
void eager_benz::sub_593370(Car_BC* pCar)
{
    if (!bExplodingScoresOff_67D4FB && field_368_player->IsUser_41DC70())
    {
        gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                               pCar->get_y_41E440(),
                                               pCar->get_z_41E450(),
                                               field_368_player->field_6BC_multpliers.field_0_value * 10);
    }

    field_368_player->field_2D4_scores.AddCash_592620(field_368_player->field_6BC_multpliers.field_0_value * 10);
    gShooey_CC_67A4B8->ReportCrimeForPed(4u, field_368_player->GetPlayerPed_4A5130());
}

MATCH_FUNC(0x593410)
void eager_benz::sub_593410(Car_BC* pCar)
{
    const s32 multpliers = field_368_player->get_multiplier_4766A0();
    if (!bExplodingScoresOff_67D4FB)
    {
        if (field_368_player->IsUser_41DC70())
        {
            gExplodingScorePool->PushScore_596890(pCar->get_x_41E430(),
                                                   pCar->get_y_41E440(),
                                                   pCar->get_z_41E450(),
                                                   100 * multpliers);
        }
    }

    field_368_player->Add_2D4(100);
    field_368_player->field_644_unk.sub_484FA0(100 * multpliers);

    if (gShooey_CC_67A4B8->sub_485090(pCar, field_368_player))
    {
        gShooey_CC_67A4B8->ReportCrimeForPed(3u, field_368_player->GetPlayerPed_4A5130());
    }
}

MATCH_FUNC(0x5934f0)
void eager_benz::UpdateAccuracyCount_5934F0(u32 a2, s32 model, Ped* pPed)
{
    field_194 = a2;
    if (pPed && bIsFrench_67D53C)
    {
        switch (pPed->get_occupation_403980())
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

    if (model == objects::fire_hitting_194 || model == objects::maybe_bullet_on_fire_198 || model == objects::flamethrower_fire_154 || model == objects::tanktop_193 || model == objects::object_195 || model == objects::object_159 || model == objects::object_199 || a2 == 0 ||
        a2 == 1 || a2 == 3)
    {
        field_198_accuracy_count = 0;
    }
    else if (a2 == 2)
    {
        field_198_accuracy_count++;
    }
}

MATCH_FUNC(0x5935b0)
thirsty_lamarr* eager_benz::GetMultiplayerFragDigits_5935B0()
{
    return &field_38_multiplayer_frags;
}

MATCH_FUNC(0x5935c0)
s32 eager_benz::sub_5935C0()
{
    return field_38_multiplayer_frags.field_0_value;
}

MATCH_FUNC(0x5935d0)
void eager_benz::ChangeFragsByAmount_5935D0(s32 amount)
{
    field_38_multiplayer_frags.ChangeStatByAmount_4921B0(amount);
}