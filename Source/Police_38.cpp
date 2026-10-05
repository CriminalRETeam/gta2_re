#include "Police_38.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Fix16_Rect.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Kfc_1E0.hpp"
#include "Object_5C.hpp"
#include "Orca_2FD4.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "PurpleDoom.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "RouteFinder.hpp"
#include "CarAI_78.hpp"

DEFINE_GLOBAL(Fix16, dword_6FECE8, 0x6FECE8);
DEFINE_GLOBAL_INIT(Fix16, kFpTen_6FED54, Fix16(0x28000, 0), 0x6FED54);
DEFINE_GLOBAL(Ped*, gCurrentCrewPed_6FEDDC, 0x6FEDDC);
DEFINE_GLOBAL(u8, byte_6FEB48, 0x6FEB48);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_6FED48, kFpOne64th_6FEB88 * 512, 0x6FED48);

MATCH_FUNC(0x4beb30)
PoliceCrew_38::PoliceCrew_38()
{
    Init_5709C0();
}

MATCH_FUNC(0x4beb40)
PoliceCrew_38::~PoliceCrew_38()
{
}

MATCH_FUNC(0x5709c0)
void PoliceCrew_38::Init_5709C0()
{
    field_2_targ_x = 0;
    field_3_targ_y = 0;
    field_4_targ_z = 0;
    field_18 = 0;
    field_1C_used = 0;
    field_1A = 0;
    field_10_subObj = 0;
    field_24_state = police_crew_state::none_0;
    field_28 = 0;
    field_8 = dword_6FECE8;
    field_C = dword_6FECE8;
    field_14_pService = 0;
    field_1A = 0;
    field_29_bCountedInPoliceCount = 0;
    field_2A = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
}

MATCH_FUNC(0x570a10)
void PoliceCrew_38::AddToService_570A10()
{
    if (field_14_pService->field_75_count < 6)
    {
        // check if it's already active
        for (u8 i = 0; i < 6; i++)
        {
            if (field_14_pService->field_20_crews[i] == this)
            {
                return;
            }
        }
        // if not found on the list, then
        field_14_pService->field_20_crews[field_14_pService->field_75_count] = this;
        ++field_14_pService->field_75_count;
        switch (field_10_subObj->field_20_crew_type)
        {
            case crew_type::police_3:
                ++field_14_pService->field_70_num_police_crews;
                break;
            case crew_type::swat_5:
                ++field_14_pService->field_72_num_swat_crews;
                break;
            case crew_type::fbi_4:
                ++field_14_pService->field_73_num_fbi_crews;
                break;
            case crew_type::army_6:
                ++field_14_pService->field_74_num_army_crews;
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x570ab0)
void PoliceCrew_38::RemoveFromService_570AB0()
{
    if (!field_1C_used || (field_24_state != police_crew_state::none_0 
        && field_24_state != police_crew_state::patrol_1 
        && field_24_state != police_crew_state::shutdown_6))
    {
        u8 last_idx = field_14_pService->field_75_count - 1;
        PoliceCrew_38* pPolice38_last = field_14_pService->field_20_crews[last_idx];
        if (pPolice38_last == this)
        {
            field_14_pService->field_20_crews[last_idx] = NULL;
        }
        else
        {
            for (u8 i = 0; i < last_idx; i++)
            {
                if (field_14_pService->field_20_crews[i] == this)
                {
                    field_14_pService->field_20_crews[i] = pPolice38_last;
                    field_14_pService->field_20_crews[last_idx] = NULL;
                    break;
                }
            }
        }
        if (field_1C_used)
        {
            switch (field_20_crew_kind)
            {
                case 1:
                    --field_14_pService->field_70_num_police_crews;
                    break;
                case 2:
                    --field_14_pService->field_72_num_swat_crews;
                    break;
                case 3:
                    --field_14_pService->field_73_num_fbi_crews;
                    break;
                case 4:
                    --field_14_pService->field_74_num_army_crews;
                    break;
                default:
                    break;
            }
        }
        --field_14_pService->field_75_count;
        field_24_state = police_crew_state::shutdown_6;
    }
}

MATCH_FUNC(0x570bf0)
void PoliceCrew_38::SpawnPoliceInCar_570BF0()
{
    PedGroup* pGroup = PedGroup::New_4CB0D0();
    Ped* pCopLeader = gPedManager_6787BC->AllocatePed_470F30();
    pCopLeader->SetField238_403920(ped_type::special_ped_4);
    pCopLeader->set_occupation_403970(ped_ocupation_enum::police);
    pCopLeader->SpawnPedInCar_45C730(field_10_subObj->field_0_car);
    pCopLeader->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pCopLeader->field_1DC_objective_target_x = Fix16(field_2_targ_x);
    pCopLeader->field_1E0_objective_target_y = Fix16(field_3_targ_y);
    pCopLeader->field_1E4_objective_target_z = Fix16(field_4_targ_z);
    pCopLeader->set_remap_433B90(0);
    pCopLeader->field_26C_graphic_type = 2;

    s32 wanted_level = gPolice_7B8_6FEE40->field_654_wanted_level;

    switch (wanted_level)
    {
        case 0:
        case 1:
            pCopLeader->field_170_selected_weapon = 0;
            pCopLeader->GiveWeapon_46F650(weapon_type::pistol);
            pCopLeader->set_health_4039A0(50);
            pCopLeader->field_1F0_maybe_max_speed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
            break;
        case 2:
            pCopLeader->GiveWeapon_46F650(weapon_type::pistol);
            pCopLeader->set_health_4039A0(100);
            pCopLeader->field_1F0_maybe_max_speed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
            break;
        default:
            pCopLeader->GiveWeapon_46F650(weapon_type::pistol);
            pCopLeader->set_health_4039A0(100);
            break;
    }

    pCopLeader->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pCopLeader->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;

    Ped* pCopSupporter = gPedManager_6787BC->AllocatePed_470F30();
    pCopSupporter->EnterCarAsPassenger_45C7F0(field_10_subObj->field_0_car);
    pCopSupporter->SetField238_403920(ped_type::special_ped_4);
    pCopSupporter->set_occupation_403970(ped_ocupation_enum::police);
    pCopSupporter->SetObjective(objectives_enum::no_obj_0, 9999);
    pCopSupporter->set_remap_433B90(0);

    switch (field_14_pService->field_4_wanted_level)
    {
        case 1:
            pCopSupporter->field_170_selected_weapon = 0;
            pCopSupporter->GiveWeapon_46F650(weapon_type::pistol);
            pCopSupporter->field_216_health = 50;
            pCopSupporter->field_1F0_maybe_max_speed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
            break;
        case 2:
            pCopSupporter->GiveWeapon_46F650(weapon_type::pistol);
            pCopSupporter->field_216_health = 100;
            pCopSupporter->field_1F0_maybe_max_speed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
            break;
        default:
            pCopSupporter->GiveWeapon_46F650(weapon_type::pistol);
            pCopSupporter->field_216_health = 100;
            break;
    }

    pCopSupporter->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pCopSupporter->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pCopSupporter->field_26C_graphic_type = 2;
    pGroup->add_ped_leader_4C9B10(pCopLeader);
    pGroup->SetCounts_433360(1);
    pGroup->add_ped_to_list_4C9B30(pCopSupporter, 0);
    pGroup->field_0 = 0;
    field_10_subObj->field_4_ped = pCopLeader;
    field_10_subObj->field_28_state = 6;
    field_10_subObj->field_0_car->SetUniNum_421560(5);
    field_10_subObj->field_0_car->InitCarAIControl_440590();
    field_10_subObj->field_0_car->sub_43AF40();
    field_10_subObj->field_0_car->ActivateEmergencyLights_43C920();
    field_10_subObj->field_8_group = pGroup;
}

MATCH_FUNC(0x570e30)
void PoliceCrew_38::SpawnSWAT_570E30()
{
    PedGroup* pSwatGroup = PedGroup::New_4CB0D0();
    Ped* pSwatLeader = gPedManager_6787BC->AllocatePed_470F30();
    pSwatLeader->SetField238_403920(ped_type::special_ped_4);
    pSwatLeader->set_occupation_403970(ped_ocupation_enum::swat);
    pSwatLeader->SpawnPedInCar_45C730(field_10_subObj->field_0_car);
    pSwatLeader->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pSwatLeader->field_1DC_objective_target_x = Fix16(field_2_targ_x);
    pSwatLeader->field_1E0_objective_target_y = Fix16(field_3_targ_y);
    pSwatLeader->field_1E4_objective_target_z = Fix16(field_4_targ_z);
    pSwatLeader->set_remap_433B90(-1);
    pSwatLeader->ForceWeapon_46F600(weapon_type::pistol);
    pSwatLeader->set_health_4039A0(400);
    pSwatLeader->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pSwatLeader->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pSwatLeader->field_26C_graphic_type = 2;
    pSwatGroup->add_ped_leader_4C9B10(pSwatLeader);
    pSwatGroup->SetCounts_433360(3);
    for (u8 i = 0; i < 3; ++i)
    {
        Ped* pSwatMember = gPedManager_6787BC->AllocatePed_470F30();
        pSwatMember->EnterCarAsPassenger_45C7F0(field_10_subObj->field_0_car);
        pSwatMember->SetField238_403920(ped_type::special_ped_4);
        pSwatMember->set_occupation_403970(ped_ocupation_enum::swat);
        pSwatMember->SetObjective(objectives_enum::no_obj_0, 9999);
        pSwatMember->set_remap_433B90(-1);
        pSwatMember->ForceWeapon_46F600(weapon_type::pistol);
        pSwatMember->set_health_4039A0(400);
        pSwatMember->field_288_threat_search = threat_search_enum::line_of_sight_1;
        pSwatMember->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
        pSwatMember->field_26C_graphic_type = 2;

        pSwatGroup->add_ped_to_list_4C9B30(pSwatMember, i);
    }
    pSwatGroup->field_0 = 0;
    field_10_subObj->field_4_ped = pSwatLeader;
    field_10_subObj->field_28_state = 6;
    field_10_subObj->field_0_car->SetUniNum_421560(5);
    field_10_subObj->field_0_car->InitCarAIControl_440590();
    field_10_subObj->field_0_car->sub_43AF40();
    field_10_subObj->field_0_car->ActivateEmergencyLights_43C920();
    field_10_subObj->field_8_group = pSwatGroup;
}

MATCH_FUNC(0x571150)
void PoliceCrew_38::SpawnFBI_nonused_571150()
{
    Ped* pFBI = gPedManager_6787BC->AllocatePed_470F30();
    pFBI->SetField238_403920(ped_type::special_ped_4);
    pFBI->set_occupation_403970(ped_ocupation_enum::fbi);
    pFBI->SpawnPedInCar_45C730(field_10_subObj->field_0_car);
    pFBI->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pFBI->field_1DC_objective_target_x = Fix16(field_2_targ_x);
    pFBI->field_1E0_objective_target_y = Fix16(field_3_targ_y);
    pFBI->field_1E4_objective_target_z = Fix16(field_4_targ_z);
    pFBI->set_remap_433B90(0);
    pFBI->ForceWeapon_46F600(weapon_type::smg);
    pFBI->set_health_4039A0(200);
    pFBI->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pFBI->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pFBI->field_26C_graphic_type = 1;
    field_10_subObj->field_4_ped = pFBI;
    field_10_subObj->field_28_state = 6;
    field_10_subObj->field_0_car->SetUniNum_421560(5);
    field_10_subObj->field_0_car->InitCarAIControl_440590();
    field_10_subObj->field_0_car->sub_43AF40();
    field_10_subObj->field_0_car->ActivateEmergencyLights_43C920();
    field_10_subObj->field_8_group = 0;
}

MATCH_FUNC(0x571350)
void PoliceCrew_38::TryDespawnOffscreenCrew_571350()
{
    field_24_state = police_crew_state::unknown_2;
    PedGroup* pGroup = field_10_subObj->field_8_group;
    if (pGroup)
    {
        if (pGroup->AreAllMembersOffScreen_4C9150())
        {
            u8 v7 = 0;
            for (Ped* pPedIter = field_10_subObj->field_4_ped; pPedIter; pPedIter = field_10_subObj->field_8_group->field_4_ped_list[v7++])
            {
                pPedIter->ClearGroupAndGroupIdx_403A30();
                pPedIter->Deallocate_45EB60();
                if (!field_10_subObj->field_8_group)
                {
                    break;
                }
            }
            field_10_subObj->field_8_group->ClearGroupData_4C8E90();
            field_10_subObj->field_28_state = 5;
            field_10_subObj->field_2C = 1;
        }
    }
    else
    {
        Ped* v6 = field_10_subObj->field_4_ped;
        if (v6)
        {
            if (v6->Get_F20E_4039F0() >= 0x1Eu)
            {
                v6->Deallocate_45EB60();
                field_10_subObj->field_28_state = 5;
                field_10_subObj->field_2C = 1;
            }
        }
        else
        {
            field_10_subObj->field_28_state = 5;
            field_10_subObj->field_2C = 1;
        }
    }
}

MATCH_FUNC(0x571540)
void PoliceCrew_38::sub_571540()
{
    Car_BC* pCar = field_10_subObj->field_0_car;
    PedGroup* pGroup = field_10_subObj->field_8_group;
    if (pCar)
    {
        if (pGroup)
        {
            if (pCar->Get_F76_4A9AD0() > 200 && pGroup->AreAllMembersOffScreen_4C9150())
            {
                u8 v7 = 0;
                for (Ped* pPedIter = field_10_subObj->field_4_ped; pPedIter; pPedIter = field_10_subObj->field_8_group->field_4_ped_list[v7++])
                {
                    pPedIter->ClearGroupAndGroupIdx_403A30();
                    pPedIter->Deallocate_45EB60();
                    if (!field_10_subObj->field_8_group)
                    {
                        break;
                    }
                }
                field_10_subObj->field_8_group->ClearGroupData_4C8E90();
                field_10_subObj->field_0_car->MarkForDespawn_421470();
                field_10_subObj->field_28_state = 5;
                field_10_subObj->field_2C = 1;
            }
        }
        else
        {
            Ped* pPed = field_10_subObj->field_4_ped;
            if (pPed)
            {
                if (pPed->get_field_20e() > 30 && pCar->Get_F76_4A9AD0() > 200)
                {
                    pPed->Deallocate_45EB60();
                    field_10_subObj->field_0_car->MarkForDespawn_421470();
                    field_10_subObj->field_28_state = 5;
                    field_10_subObj->field_2C = 1;
                }
            }
            else if (pCar->Get_F76_4A9AD0() > 200)
            {
                pCar->MarkForDespawn_421470();
                field_10_subObj->field_28_state = 5;
                field_10_subObj->field_2C = 1;
            }
        }
    }
    else if (pGroup)
    {
        if (pGroup->AreAllMembersOffScreen_4C9150())
        {
            u8 v7 = 0;
            for (Ped* pPedIter = field_10_subObj->field_4_ped; pPedIter; pPedIter = field_10_subObj->field_8_group->field_4_ped_list[v7++])
            {
                pPedIter->ClearGroupAndGroupIdx_403A30();
                pPedIter->Deallocate_45EB60();
                if (!field_10_subObj->field_8_group)
                {
                    break;
                }
            }
            field_10_subObj->field_8_group->ClearGroupData_4C8E90();
            field_10_subObj->field_28_state = 5;
            field_10_subObj->field_2C = 1;
        }
    }
    else
    {
        Ped* pPed = field_10_subObj->field_4_ped;
        if (pPed)
        {
            if (pPed->Get_F20E_4039F0() >= 30)
            {
                pPed->Deallocate_45EB60();
                field_10_subObj->field_28_state = 5;
                field_10_subObj->field_2C = 1;
            }
            else if (pPed->field_16C_car)
            {
                pPed->Deallocate_45EB60();
                field_10_subObj->field_28_state = 5;
                field_10_subObj->field_2C = 1;
            }
            else if (pPed->field_28C_threat_reaction != 1)
            {
                field_10_subObj->field_28_state = 5;
                field_10_subObj->field_2C = 1;
            }
        }
        else
        {
            field_10_subObj->field_28_state = 5;
            field_10_subObj->field_2C = 1;
        }
    }
}

WIP_FUNC(0x571a30)
void PoliceCrew_38::sub_571A30()
{
    Car_BC* pPlayerCar = gCurrentCrewPed_6FEDDC->field_16C_car;
    if (pPlayerCar && gCurrentCrewPed_6FEDDC == pPlayerCar->field_54_driver)
    {
        PedGroup* pGroup = field_10_subObj->field_8_group;
        Car_BC* pCar = field_10_subObj->field_0_car;
        if (pGroup)
        {
            if (pCar->Get_F76_4A9AD0() <= 200)
            {
                return;
            }

            if (pGroup->IsAllMembersInSomeCar_4CAA20())
            {
                u8 i = 0;
                for (Ped* pPedIter = field_10_subObj->field_4_ped; pPedIter; pPedIter = field_10_subObj->field_8_group->field_4_ped_list[i++])
                {
                    pPedIter->SetField238_403920(3);
                    pPedIter->ClearGroupAndGroupIdx_403A30();
                    if (!field_10_subObj->field_8_group)
                    {
                        break;
                    }
                }
                field_10_subObj->field_8_group->ClearGroupData_4C8E90();
                field_10_subObj->field_0_car->MarkForDespawn_421470();
                field_10_subObj->field_28_state = 5;
                field_10_subObj->field_2C = 1;
            }
            else
            {
                u8 bAllReady = 1;
                field_10_subObj->field_0_car->sub_43AF60();

                PedGroup* pGroup2 = field_10_subObj->field_8_group;
                u8 j = 0;
                for (Ped* pMember = pGroup2->field_4_ped_list[0]; pMember; pMember = pGroup2->field_4_ped_list[++j])
                {
                    if (pMember->field_168_game_object && pMember->Get_F20E_4039F0() < 10)
                    {
                        bAllReady = 0;
                    }
                }

                if (!bAllReady)
                {
                    return;
                }

                field_10_subObj->field_4_ped->ClearGroupAndGroupIdx_403A30();
                field_10_subObj->field_0_car->MarkForDespawn_421470();

                u8 k = 0;
                for (Ped* pPed = field_10_subObj->field_8_group->field_4_ped_list[0]; pPed;
                     pPed = field_10_subObj->field_8_group->field_4_ped_list[++k])
                {
                    if (pPed->field_168_game_object)
                    {
                        pPed->ClearGroupAndGroupIdx_403A30();
                        pPed->Deallocate_45EB60();
                    }
                }
                field_10_subObj->field_8_group->ClearGroupData_4C8E90();
                field_10_subObj->field_28_state = 5;
                field_10_subObj->field_2C = 1;
            }
        }
        else if (pCar->Get_F76_4A9AD0() > 80)
        {
            pCar->MarkForDespawn_421470();
            field_10_subObj->field_28_state = 5;
            field_10_subObj->field_2C = 1;
        }
    }
    else
    {
        PedGroup* pGroup = field_10_subObj->field_8_group;
        if (pGroup)
        {
            if (!pGroup->AreAllMembersOffScreen_4C9150())
            {
                return;
            }

            Car_BC* pCar = field_10_subObj->field_0_car;
            if (pCar->Get_F76_4A9AD0() <= 200 && pCar->field_7C_uni_num != 2)
            {
                return;
            }

            u8 i = 0;
            for (Ped* pPedIter = field_10_subObj->field_4_ped; pPedIter; pPedIter = field_10_subObj->field_8_group->field_4_ped_list[i++])
            {
                pPedIter->ClearGroupAndGroupIdx_403A30();
                pPedIter->Deallocate_45EB60();
                if (!field_10_subObj->field_8_group)
                {
                    break;
                }
            }
            field_10_subObj->field_8_group->ClearGroupData_4C8E90();

            pCar = field_10_subObj->field_0_car;
            if (pCar->field_7C_uni_num != 2)
            {
                pCar->MarkForDespawn_421470();
            }
            field_10_subObj->field_28_state = 5;
            field_10_subObj->field_2C = 1;
        }
        else
        {
            Car_BC* pCar = field_10_subObj->field_0_car;
            if (pCar->Get_F76_4A9AD0() <= 200 && pCar->field_7C_uni_num != 2)
            {
                return;
            }
            if (field_10_subObj->field_4_ped->Get_F20E_4039F0() <= 30)
            {
                return;
            }
            if (pCar->field_7C_uni_num != 2)
            {
                pCar->MarkForDespawn_421470();
            }
            field_10_subObj->field_4_ped->Deallocate_45EB60();
            field_10_subObj->field_28_state = 5;
            field_10_subObj->field_2C = 1;
        }
    }
}

MATCH_FUNC(0x5720c0)
void PoliceCrew_38::sub_5720C0()
{
    if (field_10_subObj->field_24 == 2)
    {
        if (field_29_bCountedInPoliceCount)
        {
            if (gPolice_7B8_6FEE40->field_658_count > 0)
            {
                gPolice_7B8_6FEE40->field_658_count--;
            }
            field_29_bCountedInPoliceCount = 0;
        }

        if (field_24_state != police_crew_state::shutdown_6)
        {
            sub_575650();
            field_24_state = police_crew_state::shutdown_6;
            return;
        }
    }
    else if (field_24_state != police_crew_state::shutdown_6)
    {
        return;
    }

    if (field_10_subObj->field_24 == 0)
    {
        TryDespawnOffscreenCrew_571350();
    }
    else if (field_10_subObj->field_24 == 2)
    {
        sub_571540();
    }
    else
    {
        sub_571A30();
    }
}

// https://decomp.me/scratch/p2NiN
// MaxAbsDistance_42A6B0 with a by-value max, which keeps the compare in registers
static inline Fix16 MaxAbsDistance_572210(Fix16 x1, Fix16 y1, Fix16 x2, Fix16 y2)
{
    Fix16 diff_x = x2 - x1;
    Fix16 diff_y = y2 - y1;
    Fix16 abs_y = Fix16::Abs(diff_y);
    Fix16 abs_x = Fix16::Abs(diff_x);
    if (!(abs_x > abs_y))
    {
        abs_x = abs_y;
    }
    return abs_x;
}

MATCH_FUNC(0x572210)
bool PoliceCrew_38::sub_572210()
{
    if (field_14_pService->field_0_criminal_ped && !field_34 && field_14_pService->field_C_timer > 0)
    {
        if (!field_10_subObj->field_24)
        {
            return MaxAbsDistance_572210(gCurrentCrewPed_6FEDDC->get_cam_x(),
                                        gCurrentCrewPed_6FEDDC->get_cam_y(),
                                        field_14_pService->field_0_criminal_ped->get_cam_x(),
                                        field_14_pService->field_0_criminal_ped->get_cam_y()) < kFpEight_6FED48 ? true : false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x572340)
void PoliceCrew_38::State3_AlertedSearch_572340()
{
    u8 bUnk = true;
    u8 idx = 0;
    byte_6FEB48 = 1;

    if (field_10_subObj->field_28_state != 3)
    {
        if (field_10_subObj->field_28_state == 6)
        {
            PoliceCrew_38::sub_5720C0();
        }
    }
    else if (field_10_subObj->field_2C)
    {
        if (gPedManager_6787BC->field_5_fbi_army_count < 0x1Au)
        {
            switch (field_10_subObj->field_20_crew_type)
            {
                case crew_type::police_3:
                    PoliceCrew_38::SpawnPoliceInCar_570BF0();
                    break;
                case crew_type::swat_5:
                    PoliceCrew_38::SpawnSWAT_570E30();
                default:
                    break;
            }
            gCurrentCrewPed_6FEDDC = field_10_subObj->field_4_ped;
            field_1A = 0;
        }
        else
        {
            Car_BC* pCar = field_10_subObj->field_0_car;
            if (pCar)
            {
                pCar->MarkForDespawn_421470();
                field_10_subObj->field_0_car = 0;
                field_24_state = police_crew_state::shutdown_6;
                PoliceCrew_38::sub_575650();
            }
        }
    }
    else
    {
        bUnk = false;
        if (field_10_subObj->field_18 == -80)
        {
            field_24_state = police_crew_state::shutdown_6;
            PoliceCrew_38::sub_575650();
        }
    }

    if (field_24_state != police_crew_state::shutdown_6)
    {
        if (bUnk)
        {
            Ped* pPed = field_10_subObj->field_4_ped;
            gCurrentCrewPed_6FEDDC = pPed;
            if (field_14_pService->field_C_timer == 250)
            {
                field_24_state = police_crew_state::pursue_or_chase_5;
            }
            else
            {
                for (; pPed; ++idx)
                {
                    if (pPed->field_278_ped_state_1 != ped_state_1::dead_9 
                        && pPed->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        Fix16 xpos_f;
                        switch (pPed->get_objective_403A80())
                        {
                            case objectives_enum::objective_52:
                                pPed->SetObjective(objectives_enum::goto_area_in_car_14, 9999);
                                gCurrentCrewPed_6FEDDC->field_1DC_objective_target_x = Fix16(field_14_pService->field_10_x.ToUInt8());
                                gCurrentCrewPed_6FEDDC->field_1E0_objective_target_y = Fix16(field_14_pService->field_14_y.ToUInt8());
                                gCurrentCrewPed_6FEDDC->field_1E4_objective_target_z = Fix16(field_14_pService->field_18_z.ToUInt8());
                                break;
                            case objectives_enum::goto_area_in_car_14:
                                PoliceCrew_38::sub_5752C0();
                                xpos_f = field_14_pService->field_10_x;
                                if (gCurrentCrewPed_6FEDDC->field_1DC_objective_target_x != field_14_pService->field_10_x ||
                                    gCurrentCrewPed_6FEDDC->field_1E0_objective_target_y != field_14_pService->field_14_y)
                                {
                                    u8 xpos = xpos_f.ToInt();
                                    u8 ypos = field_14_pService->field_14_y.ToInt();
                                    u8 zpos = field_14_pService->field_18_z.ToInt();
                                    if (gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &xpos, &ypos, &zpos, 0))
                                    {
                                        gCurrentCrewPed_6FEDDC->field_1DC_objective_target_x = Fix16(xpos);
                                        field_14_pService->field_10_x = gCurrentCrewPed_6FEDDC->field_1DC_objective_target_x;
                                        gCurrentCrewPed_6FEDDC->field_1E0_objective_target_y = Fix16(ypos);
                                        field_14_pService->field_14_y = gCurrentCrewPed_6FEDDC->field_1E0_objective_target_y;
                                        gCurrentCrewPed_6FEDDC->field_1E4_objective_target_z = Fix16(zpos);
                                        field_14_pService->field_18_z = gCurrentCrewPed_6FEDDC->field_1E4_objective_target_z;
                                    }
                                }
                                field_28 = 1;
                                break;
                            case objectives_enum::no_obj_0:
                                if (pPed->IsGroupLeader_472FD0())
                                {
                                    if (pPed->field_16C_car)
                                    {
                                        pPed->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::leave_car_36, 9999);
                                        gCurrentCrewPed_6FEDDC->set_field_150_target_objective_car(field_10_subObj->field_0_car);
                                        break;
                                    }
                                    else
                                    {
                                        pPed->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::goto_area_on_foot_12, 9999);
                                        gCurrentCrewPed_6FEDDC->field_1DC_objective_target_x = field_14_pService->field_10_x;
                                        gCurrentCrewPed_6FEDDC->field_1E0_objective_target_y = field_14_pService->field_14_y;
                                        gCurrentCrewPed_6FEDDC->field_1E4_objective_target_z = field_14_pService->field_18_z;
                                    }
                                }
                                break;
                            case objectives_enum::leave_car_36:
                                PoliceCrew_38::sub_575270();
                                break;
                            case objectives_enum::enter_car_as_driver_35:
                                PoliceCrew_38::sub_575210();
                                break;
                            case objectives_enum::objective_51:
                                if (pPed->GetObjectiveStatus_450CB0() == objective_status::failed_2)
                                {
                                    field_24_state = police_crew_state::shutdown_6;
                                    field_14_pService->field_1C = 1;
                                }
                                break;
                            case objectives_enum::kill_char_on_foot_20:
                                pPed->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::goto_area_on_foot_12, 9999);
                                gCurrentCrewPed_6FEDDC->field_1DC_objective_target_x = field_14_pService->field_10_x;
                                gCurrentCrewPed_6FEDDC->field_1E0_objective_target_y = field_14_pService->field_14_y;
                                gCurrentCrewPed_6FEDDC->field_1E4_objective_target_z = field_14_pService->field_18_z;
                                break;
                            case objectives_enum::goto_area_on_foot_12:
                                field_28 = 0;
                                if (gCurrentCrewPed_6FEDDC->GetObjectiveStatus_450CB0() == objective_status::passed_1)
                                {
                                    gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::objective_51, 200);
                                }
                                break;
                            case objectives_enum::objective_28:
                                if (pPed->GetObjectiveStatus_450CB0() != objective_status::not_finished_0)
                                {
                                    pPed->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                                }
                                break;
                            case objectives_enum::objective_32:
                                pPed->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                                break;
                            default:
                                break;
                        }
                    }
                    PedGroup* pGroup = field_10_subObj->field_8_group;
                    if (pGroup)
                    {
                        pPed = pGroup->field_4_ped_list[idx];
                    }
                    else
                    {
                        pPed = 0;
                    }
                    gCurrentCrewPed_6FEDDC = pPed;
                }
            }
        }
    }
}

EXTERN_GLOBAL(Fix16, kFpFour_6FECF8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEB44, Fix16(0x666, 0), 0x6FEB44);
DEFINE_GLOBAL_INIT(Fix16, dword_6FED08, Fix16(4), 0x6FED08);
DEFINE_GLOBAL_INIT(Fix16, dword_6FECB8, Fix16(0x147, 0), 0x6FECB8);

// The crew chasing the criminal: like State3_AlertedSearch_572340, then each member follows the criminal on foot
// or in the car depending on how far away and how fast the criminal is
WIP_FUNC(0x572920)
void PoliceCrew_38::State5_PursueOrChase_572920()
{
    u8 bUnk = true;
    u8 bChaseOnFoot = false;
    u8 idx = 0;

    if (!field_14_pService->field_0_criminal_ped)
    {
        field_24_state = police_crew_state::shutdown_6;
        PoliceCrew_38::sub_575650();
        return;
    }

    if (field_10_subObj->field_28_state != 3)
    {
        if (field_10_subObj->field_28_state == 6)
        {
            PoliceCrew_38::sub_5720C0();
            if (field_10_subObj->field_28_state == 5 || field_24_state == police_crew_state::shutdown_6)
            {
                field_24_state = police_crew_state::shutdown_6;
                PoliceCrew_38::sub_575650();
                return;
            }
        }
    }
    else
    {
        if (field_10_subObj->field_2C)
        {
            if (gPedManager_6787BC->field_5_fbi_army_count < 0x1Au)
            {
                switch (field_10_subObj->field_20_crew_type)
                {
                    case crew_type::police_3:
                        PoliceCrew_38::SpawnPoliceInCar_570BF0();
                        break;
                    case crew_type::swat_5:
                        PoliceCrew_38::SpawnSWAT_570E30();
                    default:
                        break;
                }
                gCurrentCrewPed_6FEDDC = field_10_subObj->field_4_ped;
                field_1A = 0;
            }
            else
            {
                Car_BC* pCar = field_10_subObj->field_0_car;
                if (pCar)
                {
                    pCar->MarkForDespawn_421470();
                    field_24_state = police_crew_state::shutdown_6;
                    field_10_subObj->field_0_car = 0;
                    PoliceCrew_38::sub_575650();
                }
            }
        }
        else
        {
            bUnk = false;
            if (field_10_subObj->field_18 == -80)
            {
                field_24_state = police_crew_state::shutdown_6;
                PoliceCrew_38::sub_575650();
            }
        }
    }
    if (field_24_state == police_crew_state::shutdown_6 || !bUnk)
    {
        return;
    }

    gCurrentCrewPed_6FEDDC = field_10_subObj->field_4_ped;
    if (PoliceCrew_38::sub_572210())
    {
        field_14_pService->field_10_x = field_14_pService->field_0_criminal_ped->get_cam_x();
        field_14_pService->field_14_y = field_14_pService->field_0_criminal_ped->get_cam_y();
        field_14_pService->field_18_z = field_14_pService->field_0_criminal_ped->get_cam_z();
    }
    else if (field_14_pService->field_C_timer > 0)
    {
        if (!field_10_subObj->field_24)
        {
            field_24_state = police_crew_state::shutdown_6;
            PoliceCrew_38::sub_575650();
            return;
        }
        if (field_14_pService->field_0_criminal_ped->field_16C_car)
        {
            field_14_pService->field_10_x = field_14_pService->field_0_criminal_ped->get_cam_x();
            field_14_pService->field_14_y = field_14_pService->field_0_criminal_ped->field_1AC_cam.y;
            field_14_pService->field_18_z = field_14_pService->field_0_criminal_ped->get_cam_z();
        }
    }
    else
    {
        if (!field_10_subObj->field_24)
        {
            field_24_state = police_crew_state::shutdown_6;
            PoliceCrew_38::sub_575650();
            return;
        }
        field_24_state = police_crew_state::alerted_search_3;
        return;
    }

    if (field_14_pService->field_C_timer > 0)
    {
        while (gCurrentCrewPed_6FEDDC)
        {
            Ped* pPed = gCurrentCrewPed_6FEDDC;
            if (pPed->GetPedState_403990() != ped_state_1::dead_9 && pPed->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
            {
                switch (pPed->get_objective_403A80())
                {
                    case objectives_enum::goto_area_in_car_14:
                        if (pPed == field_10_subObj->field_4_ped)
                        {
                            if (field_10_subObj->field_8_group)
                            {
                                if (field_10_subObj->field_8_group->IsAllMembersInSomeCar_4CAA20())
                                {
                                    gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::objective_52, 9999);
                                    gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);
                                    field_28 = 1;
                                }
                                else
                                {
                                    gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                                    field_28 = 1;
                                    break;
                                }
                            }
                            else
                            {
                                gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::objective_52, 9999);
                                gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);
                                field_28 = 1;
                            }
                        }
                        field_28 = 1;
                        break;

                    case objectives_enum::objective_52:
                        field_28 = 1;
                        if (field_10_subObj->field_0_car &&
                            (!field_10_subObj->field_8_group || field_10_subObj->field_8_group->HasNoActiveMembers_4CAAE0()))
                        {
                            PoliceCrew_38::sub_575310();
                            field_28 = 1;
                        }
                        else
                        {
                            gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                        }
                        break;

                    case objectives_enum::no_obj_0:
                        if (pPed == field_10_subObj->field_4_ped)
                        {
                            if (pPed->field_16C_car)
                            {
                                if (field_14_pService->field_0_criminal_ped->field_16C_car)
                                {
                                    if (field_10_subObj->field_8_group && !field_10_subObj->field_8_group->HasNoActiveMembers_4CAAE0())
                                    {
                                        break;
                                    }
                                    gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::objective_52, 9999);
                                    gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);
                                    field_28 = 1;
                                }
                                else
                                {
                                    gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::leave_car_36, 9999);
                                    gCurrentCrewPed_6FEDDC->set_field_150_target_objective_car(field_10_subObj->field_0_car);
                                    field_28 = 0;
                                }
                            }
                            else if (field_28 && field_10_subObj->field_24 == 1)
                            {
                                if (!pPed->field_21C_bf.b27)
                                {
                                    gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                                    gCurrentCrewPed_6FEDDC->set_field_150_target_objective_car(field_10_subObj->field_0_car);
                                    gCurrentCrewPed_6FEDDC->unset_bitset_0x04();
                                }
                            }
                            else if (!pPed->field_21C_bf.b27)
                            {
                                gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::kill_char_on_foot_20, 9999);
                                gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);
                                gCurrentCrewPed_6FEDDC->unset_bitset_0x04();
                                field_28 = 0;
                                field_35 = 0;
                            }
                        }
                        else if (pPed->Get_F14C_403AF0() == (Ped*)field_30 && pPed->Get_F14C_403AF0())
                        {
                            pPed->set_field_14C_403AE0(field_14_pService->field_0_criminal_ped);
                        }
                        break;

                    case objectives_enum::leave_car_36:
                        PoliceCrew_38::sub_575270();
                        field_28 = 0;
                        break;

                    case objectives_enum::enter_car_as_driver_35:
                        PoliceCrew_38::sub_575210();
                        field_28 = 1;
                        break;

                    case objectives_enum::kill_char_on_foot_20:
                    case objectives_enum::objective_32:
                    {
                        u8 status;
                        if (pPed->get_objective_target_ped_403AD0() == pPed->Get_F14C_403AF0())
                        {
                            field_30 = (s32)pPed->get_objective_target_ped_403AD0();
                            gCurrentCrewPed_6FEDDC->set_field_14C_403AE0(field_14_pService->field_0_criminal_ped);
                        }
                        gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);
                        status = gCurrentCrewPed_6FEDDC->GetObjectiveStatus_450CB0();
                        if (status == 1)
                        {
                            byte_6FEB48 = 0;
                            break;
                        }
                        if (status == 2)
                        {
                            gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                            break;
                        }

                        Ped* pCriminal = field_14_pService->field_0_criminal_ped;
                        if (pCriminal)
                        {
                            // 9.6f: MaxAbsDistance_42A6B0
                            field_8 = Fix16::MaxAbsDistance_42A6B0(gCurrentCrewPed_6FEDDC->get_cam_x(),
                                                                      gCurrentCrewPed_6FEDDC->field_1AC_cam.y,
                                                                      pCriminal->field_1AC_cam.x,
                                                                      pCriminal->get_cam_y());
                        }
                        else
                        {
                            field_8 = kFpEight_6FED48 * kFpFour_6FECF8;
                        }

                        if (field_10_subObj->field_24 == 1)
                        {
                            u8 bEnterCar = false;
                            if (field_8 > kFpEight_6FED48)
                            {
                                bEnterCar = true;
                            }
                            else if (field_14_pService->field_0_criminal_ped->field_16C_car)
                            {
                                if (field_14_pService->field_0_criminal_ped->field_16C_car->GetCarLinearSpeed_43A240() > dword_6FEB44)
                                {
                                    if (++field_35 > 30)
                                    {
                                        bEnterCar = true;
                                    }
                                }
                                else if (field_8 > dword_6FED08)
                                {
                                    bEnterCar = true;
                                }
                                else
                                {
                                    bChaseOnFoot = true;
                                }
                            }

                            if (bEnterCar)
                            {
                                // The criminal is too far or too fast to chase on foot
                                if (gCurrentCrewPed_6FEDDC->get_objective_403A80() == objectives_enum::objective_32 && gCurrentCrewPed_6FEDDC->field_278_ped_state_1 != 1)
                                {
                                    break;
                                }
                                if (!gCurrentCrewPed_6FEDDC->field_21C_bf.b27)
                                {
                                    gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                                    gCurrentCrewPed_6FEDDC->set_field_150_target_objective_car(field_10_subObj->field_0_car);
                                    gCurrentCrewPed_6FEDDC->unset_bitset_0x04();
                                    field_28 = 1;
                                }
                                break;
                            }
                        }

                        if (gCurrentCrewPed_6FEDDC->get_objective_403A80() == objectives_enum::objective_32)
                        {
                            if (field_14_pService->field_0_criminal_ped->field_168_game_object && gCurrentCrewPed_6FEDDC->field_21C_bf.b27)
                            {
                                gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::kill_char_on_foot_20, 9999);
                                gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);
                                gCurrentCrewPed_6FEDDC->unset_bitset_0x04();
                                field_28 = 0;
                            }
                        }
                        else if (bChaseOnFoot)
                        {
                            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::objective_32, 9999);
                            gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);
                            gCurrentCrewPed_6FEDDC->unset_bitset_0x04();
                        }
                        break;
                    }

                    case objectives_enum::goto_area_on_foot_12:
                        gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                        break;
                    case objectives_enum::objective_51:
                        gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                        break;

                    case objectives_enum::objective_28:
                        if (pPed->GetObjectiveStatus_450CB0())
                        {
                            gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                        }
                        break;

                    case objectives_enum::flee_char_on_foot_till_safe_2:
                        byte_6FEB48 = 0;
                        PoliceCrew_38::RemoveFromService_570AB0();
                        break;

                    case objectives_enum::objective_43:
                        pPed->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        if (field_10_subObj->field_8_group && !field_10_subObj->field_8_group->IsAllMembersInSomeCar_4CAA20())
                        {
                            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                        }
                        else
                        {
                            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::objective_52, 9999);
                            gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);
                            field_28 = 1;
                        }
                        break;

                    case objectives_enum::wait_in_car_27:
                        if (field_10_subObj->field_0_car->GetCarLinearSpeed_43A240() <= dword_6FECB8)
                        {
                            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::leave_car_36, 9999);
                            gCurrentCrewPed_6FEDDC->set_field_150_target_objective_car(gCurrentCrewPed_6FEDDC->field_16C_car);
                        }
                        break;
                }
            }

            PedGroup* pGroup = field_10_subObj->field_8_group;
            if (pGroup)
            {
                gCurrentCrewPed_6FEDDC = pGroup->field_4_ped_list[idx];
            }
            else
            {
                gCurrentCrewPed_6FEDDC = 0;
            }
            idx++;
        }
    }
    else
    {
        gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
        field_24_state = police_crew_state::shutdown_6;
        PoliceCrew_38::sub_575650();
    }
}

// https://decomp.me/scratch/mAN9o
WIP_FUNC(0x574720)
void PoliceCrew_38::State6_ShutDown_574720()
{
    byte_6FEB48 = 1;
    gCurrentCrewPed_6FEDDC = field_10_subObj->field_4_ped;
    if (field_10_subObj->field_0_car)
    {
        if (field_10_subObj->field_0_car->field_60)
        {
            gHamburger_500_678E30->FreeEntry_474CC0(field_10_subObj->field_0_car->field_60);
            field_10_subObj->field_0_car->field_60 = 0;
        }
        if (field_10_subObj->field_0_car->HasEmergencyLights_414F20())
        {
            field_10_subObj->field_0_car->DeactivateEmergencyLights_43C9D0();
        }
    }

    if (field_10_subObj->field_28_state != 3)
    {
        if (field_10_subObj->field_28_state == 6)
        {
            PoliceCrew_38::sub_5720C0();
        }
        if (field_10_subObj->field_28_state == 5)
        {
            if (field_10_subObj->field_0_car)
            {
                if (field_10_subObj->field_0_car->field_5C_AI)
                {
                    if (field_10_subObj->field_0_car->field_5C_AI->field_28_junc_idx > 0)
                    {
                        gRouteFinder_6FFDC8->CancelRoute_589930(field_10_subObj->field_0_car->field_5C_AI->field_28_junc_idx);
                        field_10_subObj->field_0_car->field_5C_AI->field_28_junc_idx = -1;
                    }
                }
            }
            PoliceCrew_38::sub_575650();
            if (field_29_bCountedInPoliceCount)
            {
                if (gPolice_7B8_6FEE40->field_658_count > 0)
                {
                    --gPolice_7B8_6FEE40->field_658_count;
                }
                field_29_bCountedInPoliceCount = 0;
            }

            if (field_10_subObj->field_4_ped)
            {
                field_10_subObj->field_4_ped->ForceDoNothing_462590();
            }
            field_10_subObj->field_28_state = 0;
            field_10_subObj->ReInit_5CBC30();
            PoliceCrew_38::Init_5709C0();
            return;
        }

        if (field_14_pService)
        {
            if (field_10_subObj->field_24 != 2)
            {
                if (field_14_pService->field_0_criminal_ped)
                {
                    if (PoliceCrew_38::sub_572210())
                    {
                        if (field_10_subObj->field_20_crew_type == 6 ||
                            (field_14_pService->field_4_wanted_level != 6 && field_14_pService->field_4_wanted_level))
                        {
                            if (gCurrentCrewPed_6FEDDC->field_258_objective == objectives_enum::enter_car_as_driver_35 &&
                                !gCurrentCrewPed_6FEDDC->field_21C_bf.b27)
                            {
                                gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                            }
                            field_14_pService->field_10_x = field_14_pService->field_0_criminal_ped->get_cam_x();
                            field_14_pService->field_14_y = field_14_pService->field_0_criminal_ped->get_cam_y();
                            field_14_pService->field_18_z = field_14_pService->field_0_criminal_ped->get_cam_z();

                            gPolice_7B8_6FEE40->AssignCrewToService_570790(this, field_14_pService);
                            return;
                        }
                    }
                }
            }
        }
        if (field_10_subObj->field_24 != 2)
        {
            if (gCurrentCrewPed_6FEDDC)
            {
                gCurrentCrewPed_6FEDDC->ClearBit11_403A40();

                // Declared here: declared any earlier, VC6 treats the known zero as a value
                // and keeps it in ebp for every other zero in the function. The original
                // still stores it at the top of the function.
                u8 i = 0;
                for (; gCurrentCrewPed_6FEDDC; ++i)
                {
                    if (gCurrentCrewPed_6FEDDC->field_278_ped_state_1 != ped_state_1::dead_9 &&
                        gCurrentCrewPed_6FEDDC->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        switch (gCurrentCrewPed_6FEDDC->get_objective_403A80())
                        {
                            case objectives_enum::goto_area_in_car_14:
                            case objectives_enum::objective_52:
                                gCurrentCrewPed_6FEDDC->SetObjective2_463830(0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::objective_43, 9999);
                                byte_6FEB48 = 0;
                                break;
                            case objectives_enum::no_obj_0:
                                if (gCurrentCrewPed_6FEDDC != field_10_subObj->field_4_ped)
                                {
                                    break;
                                }
                                if (gCurrentCrewPed_6FEDDC->field_16C_car)
                                {
                                    if (!field_10_subObj->field_8_group || field_10_subObj->field_8_group->IsAllMembersInSomeCar_4CAA20())
                                    {
                                        gCurrentCrewPed_6FEDDC->SetObjective2_463830(0, 9999);
                                        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::objective_43, 9999);
                                        gCurrentCrewPed_6FEDDC->field_16C_car->InitCarAIControl_440590();
                                        gCurrentCrewPed_6FEDDC->field_16C_car->sub_43AF40();
                                        byte_6FEB48 = 0;
                                    }
                                }
                                else
                                {
                                    if (field_10_subObj->field_24 == 1 && !gCurrentCrewPed_6FEDDC->field_21C_bf.b27)
                                    {
                                        gCurrentCrewPed_6FEDDC->SetObjective2_463830(0, 9999);
                                        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                                        gCurrentCrewPed_6FEDDC->set_field_150_target_objective_car(field_10_subObj->field_0_car);
                                        gCurrentCrewPed_6FEDDC->unset_bitset_0x04();
                                    }
                                }
                                break;
                            case objectives_enum::objective_8:
                            case objectives_enum::wait_in_car_27:
                            case objectives_enum::objective_34:
                            case objectives_enum::objective_43:
                            case objectives_enum::objective_50:
                                break;
                            case objectives_enum::enter_car_as_driver_35:
                                PoliceCrew_38::sub_575210();
                                break;
                            case objectives_enum::leave_car_36:
                                PoliceCrew_38::sub_575270();
                                break;
                            case objectives_enum::objective_28:
                                if (gCurrentCrewPed_6FEDDC->GetObjectiveStatus_450CB0())
                                {
                                    gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                                }
                                break;
                            default:
                                gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                                break;
                        }
                    }

                    if (field_10_subObj->field_8_group)
                    {
                        gCurrentCrewPed_6FEDDC = field_10_subObj->field_8_group->field_4_ped_list[i];
                    }
                    else
                    {
                        gCurrentCrewPed_6FEDDC = NULL;
                    }
                } // end for loop
            }
        }
    }
    else
    {
        if (field_10_subObj->field_0_car)
        {
            if (field_10_subObj->field_0_car->field_5C_AI)
            {
                if (field_10_subObj->field_0_car->field_5C_AI->field_28_junc_idx > 0)
                {
                    gRouteFinder_6FFDC8->CancelRoute_589930(field_10_subObj->field_0_car->field_5C_AI->field_28_junc_idx);
                    field_10_subObj->field_0_car->field_5C_AI->field_28_junc_idx = -1;
                }
            }
        }
        field_10_subObj->field_28_state = 0;
        field_10_subObj->ReInit_5CBC30();
        if (field_29_bCountedInPoliceCount)
        {
            if (gPolice_7B8_6FEE40->field_658_count > 0)
            {
                --gPolice_7B8_6FEE40->field_658_count;
            }
        }
        PoliceCrew_38::Init_5709C0();
        PoliceCrew_38::sub_575650();
    }
}

MATCH_FUNC(0x574f10)
void PoliceCrew_38::State1_Patrol_574F10()
{
    Car_BC* pCarUnk;

    byte_6FEB48 = 1;
    u8 idx = 0;

    if (field_10_subObj->field_24 == 2 || !field_10_subObj->field_24 || (pCarUnk = field_10_subObj->field_0_car, pCarUnk->field_76_last_seen_timer > 80))
    {
        field_24_state = police_crew_state::shutdown_6;
    }
    else
    {
        pCarUnk->ClearA6Bit5_421550();
        if (field_10_subObj->field_28_state == 6)
        {
            PoliceCrew_38::sub_5720C0();
        }

        if (field_10_subObj->field_28_state == 5)
        {
            Car_BC* pCar = field_10_subObj->field_0_car;
            if (pCar)
            {
                CarAI_78* v7 = pCar->field_5C_AI;
                if (v7)
                {
                    char field_28_junc_idx = v7->field_28_junc_idx;
                    if (field_28_junc_idx > 0)
                    {
                        gRouteFinder_6FFDC8->CancelRoute_589930(field_28_junc_idx);
                        field_10_subObj->field_0_car->field_5C_AI->field_28_junc_idx = -1;
                    }
                }
            }
            PoliceCrew_38::sub_575650();
            if (field_29_bCountedInPoliceCount)
            {
                if (gPolice_7B8_6FEE40->field_658_count > 0)
                {
                    gPolice_7B8_6FEE40->field_658_count--;
                }
            }
            field_10_subObj->field_28_state = 0;
            field_10_subObj->ReInit_5CBC30();
            PoliceCrew_38::Init_5709C0();
        }
        else
        {
            if (field_14_pService && field_14_pService->field_0_criminal_ped && PoliceCrew_38::sub_572210() &&
                (field_10_subObj->field_20_crew_type == crew_type::army_6 || field_14_pService->field_4_wanted_level != 6))
            {
                field_14_pService->field_10_x = field_14_pService->field_0_criminal_ped->get_cam_x();
                field_14_pService->field_14_y = field_14_pService->field_0_criminal_ped->get_cam_y();
                field_14_pService->field_18_z = field_14_pService->field_0_criminal_ped->get_cam_z();
                gPolice_7B8_6FEE40->AssignCrewToService_570790(this, field_14_pService);
            }
            else
            {
                gCurrentCrewPed_6FEDDC = field_10_subObj->field_4_ped;
                gCurrentCrewPed_6FEDDC->ClearBit11_403A40();
                Hamburger_40* v13;
                for (Ped* pPedIter = gCurrentCrewPed_6FEDDC; pPedIter; ++idx)
                {
                    switch (pPedIter->get_objective_403A80())
                    {
                        case objectives_enum::goto_area_in_car_14:
                        case objectives_enum::objective_52:
                            byte_6FEB48 = 0;
                            v13 = field_10_subObj->field_0_car->field_60;
                            if (v13)
                            {
                                gHamburger_500_678E30->FreeEntry_474CC0(v13);
                                field_10_subObj->field_0_car->field_60 = 0;
                            }
                            break;
                        case objectives_enum::no_obj_0:
                            if (pPedIter == field_10_subObj->field_4_ped)
                            {
                                if (pPedIter->field_16C_car)
                                {
                                    byte_6FEB48 = 0;
                                }
                                else if ((pPedIter->field_21C & 0x8000000) == 0)
                                {
                                    pPedIter->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                    gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                                    gCurrentCrewPed_6FEDDC->set_field_150_target_objective_car(field_10_subObj->field_0_car);
                                    gCurrentCrewPed_6FEDDC->unset_bitset_0x04();
                                }
                            }
                            break;
                        case objectives_enum::leave_car_36:
                            PoliceCrew_38::sub_575270();
                            break;
                        case objectives_enum::enter_car_as_driver_35:
                            PoliceCrew_38::sub_575210();
                            break;
                        case objectives_enum::flee_char_on_foot_till_safe_2:
                        case objectives_enum::goto_area_on_foot_12:
                        case objectives_enum::kill_char_on_foot_20:
                            pPedIter->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                            break;
                        case objectives_enum::objective_28:
                            if (pPedIter->GetObjectiveStatus_450CB0() != objective_status::not_finished_0)
                            {
                                pPedIter->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                                gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                            }
                            break;
                        default:
                            break;
                    }
                    PedGroup* pGroup = field_10_subObj->field_8_group;
                    if (pGroup)
                    {
                        pPedIter = pGroup->field_4_ped_list[idx];
                    }
                    else
                    {
                        pPedIter = 0;
                    }
                    gCurrentCrewPed_6FEDDC = pPedIter;
                }
            }
        }
    }
}

MATCH_FUNC(0x575200)
void PoliceCrew_38::sub_575200()
{
    byte_6FEB48 = 1;
    field_24_state = police_crew_state::shutdown_6;
}

MATCH_FUNC(0x575210)
void PoliceCrew_38::sub_575210()
{
    if (field_10_subObj->field_24)
    {
        if (gCurrentCrewPed_6FEDDC->GetObjectiveStatus_450CB0())
        {
            gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
        }
        else
        {
            field_10_subObj->field_0_car->SetA6Bit5_421540();
        }
    }
    else
    {
        gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
    }
}

MATCH_FUNC(0x575270)
void PoliceCrew_38::sub_575270()
{
    if (field_10_subObj->field_24)
    {
        if (gCurrentCrewPed_6FEDDC->GetObjectiveStatus_450CB0())
        {
            gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
        }
    }
    else
    {
        gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
    }
}

MATCH_FUNC(0x5752c0)
void PoliceCrew_38::sub_5752C0()
{
    if (gCurrentCrewPed_6FEDDC->GetObjectiveStatus_450CB0() != objective_status::not_finished_0)
    {
        field_10_subObj->field_28_state = 6;
        gCurrentCrewPed_6FEDDC->SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        gCurrentCrewPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
    }
    byte_6FEB48 = 1;
}

DEFINE_GLOBAL_INIT(Fix16, dword_6FECF0, Fix16(2), 0x6FECF0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEBF4, Fix16(0.5), 0x6FEBF4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FECF4, Fix16(3), 0x6FECF4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEDE0, Fix16(0x1EB, 0), 0x6FEDE0);

MATCH_FUNC(0x575310)
void PoliceCrew_38::sub_575310()
{
    byte_6FEB48 = 1;
    gCurrentCrewPed_6FEDDC->set_objective_target_ped_403AC0(field_14_pService->field_0_criminal_ped);

    if (field_14_pService->field_0_criminal_ped->field_168_game_object)
    {
        Fix16 dist;
        dist = Fix16::MaxAbsDistance_42A6B0(gCurrentCrewPed_6FEDDC->get_cam_x(),
                                            gCurrentCrewPed_6FEDDC->get_cam_y(),
                                            field_14_pService->field_0_criminal_ped->get_cam_x(),
                                            field_14_pService->field_0_criminal_ped->get_cam_y());
        if (dist < dword_6FECF0 + dword_6FEBF4)
        {
            gCurrentCrewPed_6FEDDC->SetObjective(27, 9999);
            if (field_10_subObj && field_10_subObj->field_0_car && field_10_subObj->field_0_car->field_60)
            {
                gHamburger_500_678E30->FreeEntry_474CC0(field_10_subObj->field_0_car->field_60);
                field_10_subObj->field_0_car->field_60 = 0;
            }
        }
        else
        {
            field_24_state = 3;
        }
    }
    else
    {
        Fix16 dist;
        dist = Fix16::MaxAbsDistance_42A6B0(gCurrentCrewPed_6FEDDC->get_cam_x(),
                                            gCurrentCrewPed_6FEDDC->get_cam_y(),
                                            field_14_pService->field_0_criminal_ped->get_cam_x(),
                                            field_14_pService->field_0_criminal_ped->get_cam_y());

        Car_BC* pCar = field_10_subObj->field_0_car;
        Hamburger_40* pHamburger = pCar->field_60;
        if (!pHamburger)
        {
            return;
        }

        if (pHamburger->field_C_relationship_code == 15)
        {
                pCar->field_5C_AI->field_24_flags |= 0x100000;
                gCurrentCrewPed_6FEDDC->SetObjective(27, 9999);
                field_14_pService->field_E += field_10_subObj->field_0_car->field_60->field_3C;
                if (field_10_subObj->field_0_car->field_60)
                {
                    gHamburger_500_678E30->FreeEntry_474CC0(field_10_subObj->field_0_car->field_60);
                    field_10_subObj->field_0_car->field_60 = 0;
                }
                return;
        }

        switch (pHamburger->field_C_relationship_code)
        {
            case 0:
            case 1:
            case 2:
            case 14:
            case 15:
                break;
            default:
                field_14_pService->field_78 = 1;
                break;
        }

        if ((u8)field_14_pService->field_E > 0)
        {
            field_10_subObj->field_0_car->field_60->field_3C = field_14_pService->field_E;
        }

        if (dist < dword_6FECF4 && field_10_subObj->field_0_car->GetVelocity_43A4C0() < dword_6FEDE0 &&
            field_14_pService->field_0_criminal_ped->field_16C_car->GetVelocity_43A4C0() < dword_6FEDE0)
        {
            gCurrentCrewPed_6FEDDC->SetObjective(27, 9999);
            if (field_10_subObj->field_0_car->field_60)
            {
                gHamburger_500_678E30->FreeEntry_474CC0(field_10_subObj->field_0_car->field_60);
                field_10_subObj->field_0_car->field_60 = 0;
            }
        }
    }
}

MATCH_FUNC(0x575590)
void PoliceCrew_38::Service_575590()
{
    if (field_10_subObj)
    {
        Ped* pPed = field_10_subObj->field_4_ped;
        if (pPed)
        {
            if (!pPed->field_20e_offscreen_counter && pPed->GetPedState_403990() != ped_state_1::dead_9 
                && pPed->CheckBit0_433B40())
            {
                gPolice_7B8_6FEE40->field_7B4 = 1;
            }
        }
        else
        {
            PedGroup* pGroup = field_10_subObj->field_8_group;
            if (pGroup)
            {
                Ped* pUnkPed = pGroup->field_4_ped_list[0];
                if (pUnkPed)
                {
                    pGroup->RemovePed_4C9970(pUnkPed);
                    field_10_subObj->field_8_group->add_ped_leader_4C9B10(pUnkPed);
                    field_10_subObj->field_4_ped = pUnkPed;
                }
            }
        }
    }
    switch (field_24_state)
    {
        case police_crew_state::alerted_search_3:
            PoliceCrew_38::State3_AlertedSearch_572340();
            break;
        case police_crew_state::pursue_or_chase_5:
            PoliceCrew_38::State5_PursueOrChase_572920();
            break;
        case police_crew_state::shutdown_6:
            PoliceCrew_38::State6_ShutDown_574720();
            break;
        case police_crew_state::patrol_1:
            PoliceCrew_38::State1_Patrol_574F10();
            break;
        case 2:
            PoliceCrew_38::sub_575200();
            break;
        default:
            return;
    }
}

// TODO: logic matches, but the original keeps field_75_count in bl and i in cl
MATCH_FUNC(0x575650)
void PoliceCrew_38::sub_575650()
{
    Police_7C* pService = field_14_pService;
    if (pService)
    {
        for (u8 i = 0; i < pService->field_75_count; i++)
        {
            if (this == pService->field_20_crews[i])
            {
                if (i == pService->field_75_count - 1)
                {
                    pService->field_20_crews[i] = NULL;
                    pService->field_75_count--;
                }
                else
                {
                    pService->field_20_crews[i] = pService->field_20_crews[pService->field_75_count - 1];
                    pService->field_20_crews[pService->field_75_count - 1] = NULL;
                    pService->field_75_count--;
                }

                switch (field_10_subObj->field_20_crew_type)
                {
                    case crew_type::police_3:
                        --field_14_pService->field_70_num_police_crews;
                        break;
                    case crew_type::swat_5:
                        --field_14_pService->field_72_num_swat_crews;
                        break;
                    case crew_type::fbi_4:
                        --field_14_pService->field_73_num_fbi_crews;
                        break;
                    case crew_type::army_6:
                        --field_14_pService->field_74_num_army_crews;
                        break;
                }
            }
        }
    }
}

MATCH_FUNC(0x575710)
void PoliceRoadblock_A4::Reset_575710()
{
    field_0_bActive = 0;
    field_4 = 0;
    field_8_x = 0;
    field_9_y = 0;
    field_A = 0;
    field_C = 0;
    field_10_car_1 = 0;
    field_14_car_2 = 0;
    field_18_car_3 = 0;
    field_1C_car_4 = 0;
    field_20_car_5 = 0;
    field_24_car_6 = 0;
    field_28_barrier_1 = 0;
    field_2C_barrier_2 = 0;
    field_30_barrier_3 = 0;
    field_34_barrier_4 = 0;
    field_38_barrier_5 = 0;
    field_3C_barrier_6 = 0;
    field_40_barrier_7 = 0;
    field_44_barrier_8 = 0;
    field_48_barrier_9 = 0;
    field_4C_barrier_10 = 0;
    field_50_barrier_11 = 0;
    field_54_barrier_12 = 0;
    field_58_barrier_1_id = 0;
    field_5C_barrier_2_id = 0;
    field_60_barrier_3_id = 0;
    field_64_barrier_4_id = 0;
    field_68_barrier_5_id = 0;
    field_6C_barrier_6_id = 0;
    field_70_barrier_7_id = 0;
    field_74_barrier_8_id = 0;
    field_78_barrier_9_id = 0;
    field_7C_barrier_10_id = 0;
    field_80_barrier_11_id = 0;
    field_84_barrier_12_id = 0;
    field_88_guard_1 = 0;
    field_8C_guard_2 = 0;
    field_90_guard_3 = 0;
    field_94_guard_4 = 0;
    field_98_guard_5 = 0;
    field_9C_guard_6 = 0;
}

MATCH_FUNC(0x5757b0)
void PoliceRoadblock_A4::Update_5757B0()
{
    u8 v31 = 1;

    if (field_0_bActive)
    {
        if (field_C > 0)
        {
            field_C--;
        }
        if (!field_C)
        {
            if (field_10_car_1)
            {
                if (field_10_car_1->field_7C_uni_num != 4)
                {
                    field_10_car_1 = 0;
                }
                else
                {
                    if (!field_10_car_1->field_76_last_seen_timer)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_14_car_2)
            {
                if (field_14_car_2->field_7C_uni_num != 4)
                {
                    field_14_car_2 = 0;
                }
                else
                {
                    if (!field_14_car_2->field_76_last_seen_timer)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_18_car_3)
            {
                if (field_18_car_3->field_7C_uni_num != 4)
                {
                    field_18_car_3 = 0;
                }
                else
                {
                    if (!field_18_car_3->field_76_last_seen_timer)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_1C_car_4)
            {
                if (field_1C_car_4->field_7C_uni_num != 4)
                {
                    field_1C_car_4 = 0;
                }
                else
                {
                    if (!field_1C_car_4->field_76_last_seen_timer)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_20_car_5)
            {
                if (field_20_car_5->field_7C_uni_num != 4)
                {
                    field_20_car_5 = 0;
                }
                else
                {
                    if (!field_20_car_5->field_76_last_seen_timer)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_24_car_6)
            {
                if (field_24_car_6->field_7C_uni_num != 4)
                {
                    field_24_car_6 = 0;
                }
                else
                {
                    if (!field_24_car_6->field_76_last_seen_timer)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_28_barrier_1)
            {
                if (field_28_barrier_1->field_14_id == field_58_barrier_1_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_28_barrier_1->field_4->field_14_xy.x,
                                                                     field_28_barrier_1->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_2C_barrier_2)
            {
                if (field_2C_barrier_2->field_14_id == field_5C_barrier_2_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_2C_barrier_2->field_4->field_14_xy.x,
                                                                     field_2C_barrier_2->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_30_barrier_3)
            {
                if (field_30_barrier_3->field_14_id == field_60_barrier_3_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_30_barrier_3->field_4->field_14_xy.x,
                                                                     field_30_barrier_3->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_34_barrier_4)
            {
                if (field_34_barrier_4->field_14_id == field_64_barrier_4_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_34_barrier_4->field_4->field_14_xy.x,
                                                                     field_34_barrier_4->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_38_barrier_5)
            {
                if (field_38_barrier_5->field_14_id == field_68_barrier_5_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_38_barrier_5->field_4->field_14_xy.x,
                                                                     field_38_barrier_5->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_3C_barrier_6)
            {
                if (field_3C_barrier_6->field_14_id == field_6C_barrier_6_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_3C_barrier_6->field_4->field_14_xy.x,
                                                                     field_3C_barrier_6->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_40_barrier_7)
            {
                if (field_40_barrier_7->field_14_id == field_70_barrier_7_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_40_barrier_7->field_4->field_14_xy.x,
                                                                     field_40_barrier_7->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_44_barrier_8)
            {
                if (field_44_barrier_8->field_14_id == field_74_barrier_8_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_44_barrier_8->field_4->field_14_xy.x,
                                                                     field_44_barrier_8->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_48_barrier_9)
            {
                if (field_48_barrier_9->field_14_id == field_78_barrier_9_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_48_barrier_9->field_4->field_14_xy.x,
                                                                     field_48_barrier_9->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_4C_barrier_10)
            {
                if (field_4C_barrier_10->field_14_id == field_7C_barrier_10_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_4C_barrier_10->field_4->field_14_xy.x,
                                                                     field_4C_barrier_10->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_50_barrier_11)
            {
                if (field_50_barrier_11->field_14_id == field_80_barrier_11_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_50_barrier_11->field_4->field_14_xy.x,
                                                                     field_50_barrier_11->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_54_barrier_12)
            {
                if (field_54_barrier_12->field_14_id == field_84_barrier_12_id)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_54_barrier_12->field_4->field_14_xy.x,
                                                                     field_54_barrier_12->field_4->field_14_xy.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_88_guard_1)
            {
                if (!field_88_guard_1->CheckBit0_433B40())
                {
                    field_88_guard_1 = 0;
                }
                else
                {
                    if (field_88_guard_1->field_20e_offscreen_counter < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_8C_guard_2)
            {
                if (!field_8C_guard_2->CheckBit0_433B40())
                {
                    field_8C_guard_2 = 0;
                }
                else
                {
                    if (field_8C_guard_2->field_20e_offscreen_counter < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_90_guard_3)
            {
                if (!field_90_guard_3->CheckBit0_433B40())
                {
                    field_90_guard_3 = 0;
                }
                else
                {
                    if (field_90_guard_3->field_20e_offscreen_counter < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_94_guard_4)
            {
                if (!field_94_guard_4->CheckBit0_433B40())
                {
                    field_94_guard_4 = 0;
                }
                else
                {
                    if (field_94_guard_4->field_20e_offscreen_counter < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_98_guard_5)
            {
                if (!field_98_guard_5->CheckBit0_433B40())
                {
                    field_98_guard_5 = 0;
                }
                else
                {
                    if (field_98_guard_5->field_20e_offscreen_counter < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_9C_guard_6)
            {
                if (!field_9C_guard_6->CheckBit0_433B40())
                {
                    field_9C_guard_6 = 0;
                }
                else
                {
                    if (field_9C_guard_6->field_20e_offscreen_counter < 0x50u)
                    {
                        field_E = 0;
                        return;
                    }
                }
            }
            if (v31)
            {
                if (++field_E <= 200u)
                {
                    return;
                }
                Ped* field_2C4_player_ped = gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped;

                Fix16 fix_y = field_2C4_player_ped->get_cam_y();
                Fix16 fix_x = field_2C4_player_ped->get_cam_x();

                Fix16 v29 = Fix16(field_8_x) - fix_x;
                Fix16 v30 = Fix16(field_9_y) - fix_y;

                v30 = Fix16::Abs(v30);
                v29 = Fix16::Abs(v29);

                if (v29 <= v30)
                {
                    v29 = v30;
                }
                if (v29 > kFpTen_6FED54)
                {
                    PoliceRoadblock_A4::RemoveRoadblock_575CA0();
                    return;
                }
            }
            field_E = 0;
        }
    }
}

MATCH_FUNC(0x575ca0)
void PoliceRoadblock_A4::RemoveRoadblock_575CA0()
{
    if (field_10_car_1)
    {
        field_10_car_1->MarkForDespawn_421470();
        field_10_car_1->SetUniNum_421560(3);
        field_10_car_1 = 0;
    }

    if (field_14_car_2)
    {
        field_14_car_2->MarkForDespawn_421470();
        field_14_car_2->SetUniNum_421560(3);
        field_14_car_2 = 0;
    }
    if (field_18_car_3)
    {
        field_18_car_3->MarkForDespawn_421470();
        field_18_car_3->SetUniNum_421560(3);
        field_18_car_3 = 0;
    }

    if (field_1C_car_4)
    {
        field_1C_car_4->MarkForDespawn_421470();
        field_1C_car_4->SetUniNum_421560(3);
        field_1C_car_4 = 0;
    }

    if (field_20_car_5)
    {
        field_20_car_5->MarkForDespawn_421470();
        field_20_car_5->SetUniNum_421560(3);
        field_20_car_5 = 0;
    }

    if (field_24_car_6)
    {
        field_24_car_6->MarkForDespawn_421470();
        field_24_car_6->SetUniNum_421560(3);
        field_24_car_6 = 0;
    }

    if (field_28_barrier_1)
    {
        if (field_28_barrier_1->field_14_id == field_58_barrier_1_id)
        {
            field_28_barrier_1->Dealloc_5291B0();
        }
        field_28_barrier_1 = 0;
    }

    if (field_2C_barrier_2)
    {
        if (field_2C_barrier_2->field_14_id == field_5C_barrier_2_id)
        {
            field_2C_barrier_2->Dealloc_5291B0();
        }
        field_2C_barrier_2 = 0;
    }

    if (field_30_barrier_3)
    {
        if (field_30_barrier_3->field_14_id == field_60_barrier_3_id)
        {
            field_30_barrier_3->Dealloc_5291B0();
        }
        field_30_barrier_3 = 0;
    }

    if (field_34_barrier_4)
    {
        if (field_34_barrier_4->field_14_id == field_64_barrier_4_id)
        {
            field_34_barrier_4->Dealloc_5291B0();
        }
        field_34_barrier_4 = 0;
    }

    if (field_38_barrier_5)
    {
        if (field_38_barrier_5->field_14_id == field_68_barrier_5_id)
        {
            field_38_barrier_5->Dealloc_5291B0();
        }
        field_38_barrier_5 = 0;
    }

    if (field_3C_barrier_6)
    {
        if (field_3C_barrier_6->field_14_id == field_6C_barrier_6_id)
        {
            field_3C_barrier_6->Dealloc_5291B0();
        }
        field_3C_barrier_6 = 0;
    }

    if (field_40_barrier_7)
    {
        if (field_40_barrier_7->field_14_id == field_70_barrier_7_id)
        {
            field_40_barrier_7->Dealloc_5291B0();
        }
        field_40_barrier_7 = 0;
    }

    if (field_44_barrier_8)
    {
        if (field_44_barrier_8->field_14_id == field_74_barrier_8_id)
        {
            field_44_barrier_8->Dealloc_5291B0();
        }
        field_44_barrier_8 = 0;
    }

    if (field_48_barrier_9)
    {
        if (field_48_barrier_9->field_14_id == field_78_barrier_9_id)
        {
            field_48_barrier_9->Dealloc_5291B0();
        }
        field_48_barrier_9 = 0;
    }

    if (field_4C_barrier_10)
    {
        if (field_4C_barrier_10->field_14_id == field_7C_barrier_10_id)
        {
            field_4C_barrier_10->Dealloc_5291B0();
        }
        field_4C_barrier_10 = 0;
    }

    if (field_50_barrier_11)
    {
        if (field_50_barrier_11->field_14_id == field_80_barrier_11_id)
        {
            field_50_barrier_11->Dealloc_5291B0();
        }
        field_50_barrier_11 = 0;
    }

    if (field_54_barrier_12)
    {
        if (field_54_barrier_12->field_14_id == field_84_barrier_12_id)
        {
            field_54_barrier_12->Dealloc_5291B0();
        }
        field_54_barrier_12 = 0;
    }

    if (field_88_guard_1)
    {
        if (field_88_guard_1->field_20e_offscreen_counter)
        {
            field_88_guard_1->Deallocate_45EB60();
        }
        else
        {
            field_88_guard_1->SetField238_403920(ped_type::dummy_3);
        }
        field_88_guard_1 = 0;
    }

    if (field_8C_guard_2)
    {
        if (field_8C_guard_2->field_20e_offscreen_counter)
        {
            field_8C_guard_2->Deallocate_45EB60();
        }
        else
        {
            field_8C_guard_2->SetField238_403920(ped_type::dummy_3);
        }
        field_8C_guard_2 = 0;
    }

    if (field_90_guard_3)
    {
        if (field_90_guard_3->field_20e_offscreen_counter)
        {
            field_90_guard_3->Deallocate_45EB60();
        }
        else
        {
            field_90_guard_3->SetField238_403920(ped_type::dummy_3);
        }
        field_90_guard_3 = 0;
    }

    if (field_94_guard_4)
    {
        if (field_94_guard_4->field_20e_offscreen_counter)
        {
            field_94_guard_4->Deallocate_45EB60();
        }
        else
        {
            field_94_guard_4->SetField238_403920(ped_type::dummy_3);
        }
        field_94_guard_4 = 0;
    }

    if (field_98_guard_5)
    {
        if (field_98_guard_5->field_20e_offscreen_counter)
        {
            field_98_guard_5->Deallocate_45EB60();
        }
        else
        {
            field_98_guard_5->SetField238_403920(ped_type::dummy_3);
        }
        field_98_guard_5 = 0;
    }

    if (field_9C_guard_6)
    {
        if (field_9C_guard_6->field_20e_offscreen_counter)
        {
            field_9C_guard_6->Deallocate_45EB60();
            field_9C_guard_6 = 0;
            field_0_bActive = 0;
            return;
        }
        field_9C_guard_6->SetField238_403920(ped_type::dummy_3);
        field_9C_guard_6 = 0;
    }
    field_0_bActive = 0;
}

// Roadblock building: the values aren't known yet
DEFINE_GLOBAL_INIT(Fix16, dword_6FECEC, Fix16(1), 0x6FECEC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEDA0, Fix16(0x100, 0), 0x6FEDA0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FED80, Fix16(16), 0x6FED80);
DEFINE_GLOBAL_INIT(Fix16, dword_6FED0C, Fix16(8), 0x6FED0C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEBD0, Fix16(0.75), 0x6FEBD0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEB50, Fix16(0xCCC, 0), 0x6FEB50);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEB5C, Fix16(0.5), 0x6FEB5C);
DEFINE_GLOBAL(Ang16, word_6FEE30, 0x6FEE30);
DEFINE_GLOBAL_INIT(Ang16, word_6FEB74, Ang16(360), 0x6FEB74);
DEFINE_GLOBAL_INIT(u8, byte_624FBC, 1, 0x624FBC); // the next roadblock lane gets barriers
DEFINE_GLOBAL_INIT(u8, byte_624FBD, 1, 0x624FBD); // the next roadblock lane gets a guard

// The "first free slot" fills for the cars, barriers and guards are written out in the body, as in
// 9.6f (built with /Ob0, so they were never inline helpers). As inline helpers they used up VC6's
// inline budget and the x branch's Ang16 Normalize went out of line.
WIP_FUNC(0x575ff0)
char_type PoliceRoadblock_A4::CreateRoadblock_575FF0(u8 x, u8 y, u8 z, s32 orientation)
{
    Car_BC* pCar = 0;
    u8 bEdge = 0;
    u8 tries = 0;
    char_type bFound;
    u8 width;
    u8 lane;
    Ang16 angle;

    if (orientation == 2)
    {
        // Find the road's edges along y
        s32 z_below = z - 1;
        do
        {
            bFound = 0;
            y--;
            switch (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x, y, z_below))
            {
                case AIR:
                    bFound = 1;
                    y++;
                    break;
                case PAVEMENT:
                case FIELD:
                    if (!bEdge)
                    {
                        bEdge = 1;
                    }
                    else
                    {
                        bFound = 1;
                    }
                    break;
                case ROAD:
                    break;
                case 4:
                    return 0;
                default:
                    return 0;
            }
            if (++tries > 12)
            {
                return 0;
            }
        } while (!bFound);

        width = 0;
        if (bEdge == 1)
        {
            width = 1;
            y++;
        }
        u8 y_start = y;
        bEdge = 0;
        tries = 0;
        do
        {
            bFound = 0;
            y++;
            switch (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x, y, z_below))
            {
                case ROAD:
                    width++;
                    break;
                case PAVEMENT:
                case FIELD:
                    if (!bEdge)
                    {
                        width++;
                        bEdge = 1;
                    }
                    else
                    {
                        bFound = 1;
                    }
                    break;
                case AIR:
                    bFound = 1;
                    break;
                case 4:
                    return 0;
                default:
                    return 0;
            }
            if (++tries > 12)
            {
                return 0;
            }
        } while (!bFound);

        if (width > 12)
        {
            return 0;
        }

        Fix16 x_left = Fix16(x) + dword_6FEBF4;
        Fix16 y_top = Fix16(y_start);
        Fix16 x_right = Fix16(x) + dword_6FEBF4;
        Fix16 y_bottom = Fix16(y_start + width + 1);
        Fix16 zpos = Fix16(z);
        field_A0_rect->SetRect_41E350(x_left - dword_6FEBF4, x_right + dword_6FEBF4, y_top, y_bottom);
        field_A0_rect->SetHiLowZ_41E370(zpos - dword_6FECEC, zpos + dword_6FECEC);
        if (gPurpleDoom_1_679208->CheckRectForCollisions_477F60(field_A0_rect, 0, 0, 0))
        {
            return 0;
        }
        if (gMap_0x370_6F6268->HasWallInArea_4E18A0((x_left - dword_6FEBF4).ToInt(),
                                                    (x_right + dword_6FEBF4 - dword_6FEDA0).ToInt(),
                                                    y_top.ToInt(),
                                                    (y_bottom - dword_6FEDA0).ToInt(),
                                                    zpos.ToInt()))
        {
            return 0;
        }

        Reset_575710();
        for (lane = 0; lane < width; lane++)
        {
            if (lane % 2)
            {
                if (lane > 0 && lane < width - 1)
                {
                    s16 max = 32;
                    angle = Ang16::Fix16_To_Ang16_40F540((Fix16(gRng_6F6784.get_int_4F7AE0(max)) - dword_6FED80) * kFpOne64th_6FEB88) +
                        word_6FEE30;
                }
                else
                {
                    angle = word_6FEE30;
                }

                if (gCar_6C_677930->CanAllocateOfType_446930(7))
                {
                    switch (gRoadblockGuardType_6FEDB8)
                    {
                        default:
                            break;
                        case 1:
                            pCar = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(x) + dword_6FEBF4,
                                                                             Fix16(y_start + lane) + dword_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::COPCAR,
                                                                             dword_6FECEC);
                            byte_624FBC = 1;
                            byte_624FBD = 1;
                            break;
                        case 2:
                            pCar = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(x) + dword_6FEBF4,
                                                                             Fix16(y_start + lane) + dword_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::COPCAR,
                                                                             dword_6FECEC);
                            byte_624FBC = 1;
                            byte_624FBD = 1;
                            break;
                        case 3:
                            pCar = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(x) + dword_6FEBF4,
                                                                             Fix16(y_start + lane) + dword_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::EDSELFBI,
                                                                             dword_6FECEC);
                            byte_624FBC = 1;
                            byte_624FBD = 1;
                            break;
                        case 4:
                            if (lane != width - 1)
                            {
                                pCar = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(x) + dword_6FEBF4,
                                                                                 Fix16(y_start + lane) + dword_6FEBD0,
                                                                                 angle,
                                                                                 car_model_enum::TANK,
                                                                                 dword_6FECEC);
                                byte_624FBC = 0;
                                byte_624FBD = 0;
                                Ped* pDriver = gPedManager_6787BC->SpawnDriver_470B00(pCar);
                                pDriver->SetField238_403920(5);
                                pDriver->set_occupation_403970(0x27);
                                pDriver->field_28C_threat_reaction = 1;
                                pCar->field_0_qq.GetSpriteForModel_5A6A50(148)->field_10_rot = word_6FEB74;
                            }
                            break;
                    }
                    if (pCar)
                    {
                        pCar->SetUniNum_421560(4);
                        pCar->IncrementCarStats_443D70(7);
                        if (pCar->HasEmergencyLights_414F20())
                        {
                            pCar->ActivateEmergencyLights_43C920();
                        }
                        if (!field_10_car_1)
                        {
                            field_10_car_1 = pCar;
                        }
                        else if (!field_14_car_2)
                        {
                            field_14_car_2 = pCar;
                        }
                        else if (!field_18_car_3)
                        {
                            field_18_car_3 = pCar;
                        }
                        else if (!field_1C_car_4)
                        {
                            field_1C_car_4 = pCar;
                        }
                        else if (!field_20_car_5)
                        {
                            field_20_car_5 = pCar;
                        }
                        else
                        {
                            field_24_car_6 = pCar;
                        }
                    }
                }
            }
            else
            {
                if (byte_624FBC)
                {
                    Object_2C* pBarrier1 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(21,
                                                                                   Fix16(x) + dword_6FEB50,
                                                                                   Fix16(y_start + lane) + dword_6FEBF4,
                                                                                   z,
                                                                                   word_6FEB74);
                    Object_2C* pBarrier2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(21,
                                                                                   Fix16(x) + kFpPoint8_6FEB68,
                                                                                   Fix16(y_start + lane) + dword_6FEBF4,
                                                                                   z,
                                                                                   word_6FEB74);
                    if (!field_28_barrier_1)
                    {
                        field_28_barrier_1 = pBarrier1;
                        field_2C_barrier_2 = pBarrier2;
                        field_58_barrier_1_id = pBarrier1->field_14_id;
                        field_5C_barrier_2_id = pBarrier2->field_14_id;
                    }
                    else if (!field_30_barrier_3)
                    {
                        field_30_barrier_3 = pBarrier1;
                        field_34_barrier_4 = pBarrier2;
                        field_60_barrier_3_id = pBarrier1->field_14_id;
                        field_64_barrier_4_id = pBarrier2->field_14_id;
                    }
                    else if (!field_38_barrier_5)
                    {
                        field_38_barrier_5 = pBarrier1;
                        field_3C_barrier_6 = pBarrier2;
                        field_68_barrier_5_id = pBarrier1->field_14_id;
                        field_6C_barrier_6_id = pBarrier2->field_14_id;
                    }
                    else if (!field_40_barrier_7)
                    {
                        field_40_barrier_7 = pBarrier1;
                        field_44_barrier_8 = pBarrier2;
                        field_70_barrier_7_id = pBarrier1->field_14_id;
                        field_74_barrier_8_id = pBarrier2->field_14_id;
                    }
                    else if (!field_48_barrier_9)
                    {
                        field_48_barrier_9 = pBarrier1;
                        field_4C_barrier_10 = pBarrier2;
                        field_78_barrier_9_id = pBarrier1->field_14_id;
                        field_7C_barrier_10_id = pBarrier2->field_14_id;
                    }
                    else if (!field_50_barrier_11)
                    {
                        field_50_barrier_11 = pBarrier1;
                        field_54_barrier_12 = pBarrier2;
                        field_80_barrier_11_id = pBarrier1->field_14_id;
                        field_84_barrier_12_id = pBarrier2->field_14_id;
                    }
                }
                if (byte_624FBD)
                {
                    Ped* pGuard = gPolice_7B8_6FEE40->SpawnRoadblockGuard_56F5C0(Fix16(x) + dword_6FEB5C,
                                                                            Fix16(y_start + lane) + dword_6FEBF4,
                                                                            z,
                                                                            word_6FEB74);
                    if (!field_88_guard_1)
                    {
                        field_88_guard_1 = pGuard;
                    }
                    else if (!field_8C_guard_2)
                    {
                        field_8C_guard_2 = pGuard;
                    }
                    else if (!field_90_guard_3)
                    {
                        field_90_guard_3 = pGuard;
                    }
                    else if (!field_94_guard_4)
                    {
                        field_94_guard_4 = pGuard;
                    }
                    else if (!field_98_guard_5)
                    {
                        field_98_guard_5 = pGuard;
                    }
                    else if (!field_9C_guard_6)
                    {
                        field_9C_guard_6 = pGuard;
                    }
                }
            }
        }
    }
    else
    {
        // Find the road's edges along x
        s32 z_below = z - 1;
        do
        {
            bFound = 0;
            x--;
            switch (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x, y, z_below))
            {
                case AIR:
                    bFound = 1;
                    x++;
                    break;
                case PAVEMENT:
                case FIELD:
                    if (!bEdge)
                    {
                        bEdge = 1;
                    }
                    else
                    {
                        bFound = 1;
                    }
                    break;
                case ROAD:
                    break;
                case 4:
                    return 0;
                default:
                    return 0;
            }
            if (++tries > 12)
            {
                return 0;
            }
        } while (!bFound);

        width = 0;
        if (bEdge == 1)
        {
            width = 1;
            x++;
        }
        u8 x_start = x;
        bEdge = 0;
        tries = 0;
        do
        {
            bFound = 0;
            x++;
            switch (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x, y, z_below))
            {
                case ROAD:
                    width++;
                    break;
                case PAVEMENT:
                case FIELD:
                    if (!bEdge)
                    {
                        width++;
                        bEdge = 1;
                    }
                    else
                    {
                        bFound = 1;
                    }
                    break;
                case AIR:
                    bFound = 1;
                    break;
                case 4:
                    return 0;
                default:
                    return 0;
            }
            if (++tries > 12)
            {
                return 0;
            }
        } while (!bFound);

        if (width > 12)
        {
            return 0;
        }

        Fix16 x_left = Fix16(x_start);
        Fix16 y_top = Fix16(y) + dword_6FEBF4;
        Fix16 x_right = Fix16(x_start + width + 1);
        Fix16 y_bottom = Fix16(y) + dword_6FEBF4;
        Fix16 zpos = Fix16(z);
        field_A0_rect->SetRect_41E350(x_left, x_right, y_top - dword_6FEBF4, y_bottom + dword_6FEBF4);
        field_A0_rect->SetHiLowZ_41E370(zpos - dword_6FECEC, zpos + dword_6FECEC);
        if (gPurpleDoom_1_679208->CheckRectForCollisions_477F60(field_A0_rect, 0, 0, 0))
        {
            return 0;
        }
        if (gMap_0x370_6F6268->HasWallInArea_4E18A0(x_left.ToInt(),
                                                    (x_right - dword_6FEDA0).ToInt(),
                                                    (y_top - dword_6FEBF4).ToInt(),
                                                    (y_bottom + dword_6FEBF4 - dword_6FEDA0).ToInt(),
                                                    zpos.ToInt()))
        {
            return 0;
        }

        Reset_575710();
        for (lane = 0; lane < width; lane++)
        {
            if (lane % 2)
            {
                if (lane > 0 && lane < width - 1)
                {
                    s16 max = 16;
                    angle =
                        Ang16::Fix16_To_Ang16_40F540((Fix16(gRng_6F6784.get_int_4F7AE0(max)) - dword_6FED0C) * kFpOne64th_6FEB88) + word_6FEB74;
                }
                else
                {
                    angle = word_6FEB74;
                }

                if (gCar_6C_677930->CanAllocateOfType_446930(7))
                {
                    switch (gRoadblockGuardType_6FEDB8)
                    {
                        default:
                            break;
                        case 1:
                            pCar = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(x_start + lane) + dword_6FEBF4,
                                                                             Fix16(y) + dword_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::COPCAR,
                                                                             dword_6FECEC);
                            byte_624FBC = 1;
                            break;
                        case 2:
                            pCar = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(x_start + lane) + dword_6FEBF4,
                                                                             Fix16(y) + dword_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::COPCAR,
                                                                             dword_6FECEC);
                            byte_624FBC = 1;
                            break;
                        case 3:
                            pCar = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(x_start + lane) + dword_6FEBF4,
                                                                             Fix16(y) + dword_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::EDSELFBI,
                                                                             dword_6FECEC);
                            byte_624FBC = 1;
                            break;
                        case 4:
                            if (lane != width - 1)
                            {
                                pCar = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(x_start + lane) + dword_6FEBD0,
                                                                                 Fix16(y) + dword_6FEBF4,
                                                                                 angle,
                                                                                 car_model_enum::TANK,
                                                                                 dword_6FECEC);
                                byte_624FBC = 0;
                                byte_624FBD = 0;
                                Ped* pDriver = gPedManager_6787BC->SpawnDriver_470B00(pCar);
                                pDriver->SetField238_403920(5);
                                pDriver->set_occupation_403970(0x27);
                                pDriver->field_28C_threat_reaction = 1;
                            }
                            break;
                    }
                    if (pCar)
                    {
                        pCar->SetUniNum_421560(4);
                        pCar->IncrementCarStats_443D70(7);
                        if (pCar->HasEmergencyLights_414F20())
                        {
                            pCar->ActivateEmergencyLights_43C920();
                        }
                        if (!field_10_car_1)
                        {
                            field_10_car_1 = pCar;
                        }
                        else if (!field_14_car_2)
                        {
                            field_14_car_2 = pCar;
                        }
                        else if (!field_18_car_3)
                        {
                            field_18_car_3 = pCar;
                        }
                        else if (!field_1C_car_4)
                        {
                            field_1C_car_4 = pCar;
                        }
                        else if (!field_20_car_5)
                        {
                            field_20_car_5 = pCar;
                        }
                        else
                        {
                            field_24_car_6 = pCar;
                        }
                    }
                }
            }
            else
            {
                if (byte_624FBC)
                {
                    Object_2C* pBarrier1 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(21,
                                                                                   Fix16(x_start + lane) + dword_6FEBF4,
                                                                                   Fix16(y) + dword_6FEB50,
                                                                                   z,
                                                                                   word_6FEE30);
                    Object_2C* pBarrier2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(21,
                                                                                   Fix16(x_start + lane) + dword_6FEBF4,
                                                                                   Fix16(y) + kFpPoint8_6FEB68,
                                                                                   z,
                                                                                   word_6FEE30);
                    if (!field_28_barrier_1)
                    {
                        field_28_barrier_1 = pBarrier1;
                        field_2C_barrier_2 = pBarrier2;
                        field_58_barrier_1_id = pBarrier1->field_14_id;
                        field_5C_barrier_2_id = pBarrier2->field_14_id;
                    }
                    else if (!field_30_barrier_3)
                    {
                        field_30_barrier_3 = pBarrier1;
                        field_34_barrier_4 = pBarrier2;
                        field_60_barrier_3_id = pBarrier1->field_14_id;
                        field_64_barrier_4_id = pBarrier2->field_14_id;
                    }
                    else if (!field_38_barrier_5)
                    {
                        field_38_barrier_5 = pBarrier1;
                        field_3C_barrier_6 = pBarrier2;
                        field_68_barrier_5_id = pBarrier1->field_14_id;
                        field_6C_barrier_6_id = pBarrier2->field_14_id;
                    }
                    else if (!field_40_barrier_7)
                    {
                        field_40_barrier_7 = pBarrier1;
                        field_44_barrier_8 = pBarrier2;
                        field_70_barrier_7_id = pBarrier1->field_14_id;
                        field_74_barrier_8_id = pBarrier2->field_14_id;
                    }
                    else if (!field_48_barrier_9)
                    {
                        field_48_barrier_9 = pBarrier1;
                        field_4C_barrier_10 = pBarrier2;
                        field_78_barrier_9_id = pBarrier1->field_14_id;
                        field_7C_barrier_10_id = pBarrier2->field_14_id;
                    }
                    else if (!field_50_barrier_11)
                    {
                        field_50_barrier_11 = pBarrier1;
                        field_54_barrier_12 = pBarrier2;
                        field_80_barrier_11_id = pBarrier1->field_14_id;
                        field_84_barrier_12_id = pBarrier2->field_14_id;
                    }
                }
                if (byte_624FBD)
                {
                    Ped* pGuard = gPolice_7B8_6FEE40->SpawnRoadblockGuard_56F5C0(Fix16(x_start + lane) + dword_6FEBF4,
                                                                            Fix16(y) + dword_6FEB5C,
                                                                            z,
                                                                            word_6FEE30);
                    if (!field_88_guard_1)
                    {
                        field_88_guard_1 = pGuard;
                    }
                    else if (!field_8C_guard_2)
                    {
                        field_8C_guard_2 = pGuard;
                    }
                    else if (!field_90_guard_3)
                    {
                        field_90_guard_3 = pGuard;
                    }
                    else if (!field_94_guard_4)
                    {
                        field_94_guard_4 = pGuard;
                    }
                    else if (!field_98_guard_5)
                    {
                        field_98_guard_5 = pGuard;
                    }
                    else if (!field_9C_guard_6)
                    {
                        field_9C_guard_6 = pGuard;
                    }
                }
            }
        }
    }

    field_0_bActive = 1;
    field_C = 100;
    return 1;
}

MATCH_FUNC(0x577480)
PoliceRoadblock_A4::PoliceRoadblock_A4()
{
    Reset_575710();
    field_A0_rect = new Fix16_Rect();
}

MATCH_FUNC(0x5774a0)
PoliceRoadblock_A4::~PoliceRoadblock_A4()
{
    delete[] field_A0_rect;
}