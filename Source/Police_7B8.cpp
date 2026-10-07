#include "Police_7B8.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Kfc_1E0.hpp"
#include "Object_5C.hpp"
#include "Orca_2FD4.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "winmain.hpp"

DEFINE_GLOBAL(Police_7B8*, gPolice_7B8_6FEE40, 0x6FEE40);
DEFINE_GLOBAL(s32, gRoadblockGuardType_6FEDB8, 0x6FEDB8);
DEFINE_GLOBAL(u8, bHaveCriminals_6FEE44, 0x6FEE44);
DEFINE_GLOBAL(u16, id_counter_6FEE46, 0x6FEE46);
DEFINE_GLOBAL(s32, dword_6FEDCC, 0x6FEDCC);
DEFINE_GLOBAL(u32, dword_6FEE18, 0x6FEE18);
DEFINE_GLOBAL(s16, word_6FEAC8, 0x6FEAC8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6FEB68, Fix16(13107, 0), 0x6FEB68);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FECA0, Fix16(256, 0), 0x6FECA0);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FEB88, kFpOne64th_6FECA0, 0x6FEB88);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_6FECF8, Fix16(4), 0x6FECF8);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixteenth_6FEB0C, kFpFour_6FECF8* kFpOne64th_6FEB88, 0x6FEB0C);

EXTERN_GLOBAL(Fix16, dword_6FECE8);

MATCH_FUNC(0x4BEB50)
Police_7B8::~Police_7B8()
{
}

MATCH_FUNC(0x56f400)
void Police_7B8::Init_56F400()
{
    field_0 = 1;
    for (s32 i = 0; i < 4; i++)
    {
        field_464_services[i].field_0_criminal_ped = NULL;
        field_464_services[i].field_C_timer = 0;
        field_464_services[i].field_1C = 0;

        field_464_services[i].field_70_num_police_crews = 0;
        field_464_services[i].field_71_num_unknown = 0;
        field_464_services[i].field_72_num_swat_crews = 0;
        field_464_services[i].field_73_num_fbi_crews = 0;
        field_464_services[i].field_74_num_army_crews = 0;
        field_464_services[i].field_76 = 0;

        field_464_services[i].field_4_wanted_level = 0;
        field_464_services[i].field_8_state = 0;

        field_464_services[i].field_10_x = dword_6FECE8;
        field_464_services[i].field_14_y = dword_6FECE8;
        field_464_services[i].field_18_z = dword_6FECE8;

        field_464_services[i].field_1C = 0;
        field_464_services[i].field_E = 0;

        field_464_services[i].field_75_count = 0;
        field_464_services[i].field_78 = 0;
        field_464_services[i].field_7A_wanted_timer = 0;

        memset(field_464_services[i].field_20_crews, 0, 0x18);
    }
    field_654_max_wanted_level = 0;
    field_658_police_car_count = 0;
    field_659_max_police_cars = 1;
    field_65C_highest_crew_type_on_service = crew_type::police_3;
    bHaveCriminals_6FEE44 = 0;
    if (bStartNetworkGame_7081F0)
    {
        field_660_max_wanted_stars = 1;
    }
    else
    {
        field_660_max_wanted_stars = 6;
    }
    field_7AC_roadblock_cooldown = 100;
    field_7AD_police_peds_in_range_screen = 0;
    field_7B0_last_firing_emergency_ped = 0;
    field_7B4_crew_ped_onscreen = 0;
}

MATCH_FUNC(0x56f4d0)
bool Police_7B8::HandlePedDeath_56F4D0(Ped* pPed)
{
    for (u8 idx = 0; idx < 20; idx++)
    {
        PoliceCrew_38* pCrew = &this->field_4_cop_crew[idx];
        if (pCrew->field_1C_used)
        {
            if (pCrew->field_10_subObj->field_4_ped == pPed)
            {
                char_type bLeaderReplaced = pCrew->field_10_subObj->ReplaceLeaderIfNeeded_5CBC90();
                Kfc_30* pKfc = pCrew->field_10_subObj;
                if (pCrew->field_10_subObj->field_8_group)
                {
                    pKfc->field_4_ped = pKfc->field_8_group->field_2C_ped_leader;
                }
                if (bLeaderReplaced != 0)
                {
                    return true;
                }
                else
                {
                    pCrew->field_10_subObj->field_4_ped = NULL;
                    return false;
                }
            }
            else
            {
                if (pPed->field_164_ped_group)
                {
                    pPed->field_164_ped_group->RemovePed_4C9970(pPed);
                }
                return false;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x56f560)
PoliceCrew_38* Police_7B8::New_56F560()
{
    for (u8 i = 0; i < 20; i++)
    {
        if (!field_4_cop_crew[i].field_1C_used)
        {
            PoliceCrew_38* pNew = &field_4_cop_crew[i];
            pNew->Init_5709C0();
            return pNew;
        }
    }
    return NULL;
}

MATCH_FUNC(0x56f5c0)
Ped* Police_7B8::SpawnRoadblockGuard_56F5C0(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Ped* pCop = NULL;

    if (gPedManager_6787BC->field_5_fbi_army_count >= 30)
    {
        return NULL;
    }

    switch (gRoadblockGuardType_6FEDB8)
    {
        case 3:
            pCop = gPedManager_6787BC->SpawnPedAt(xpos, ypos, zpos, 0, rotation);
            pCop->SetField238_403920(ped_type::special_ped_4);
            pCop->set_occupation_403970(ped_ocupation_enum::roadblock_cop_37);
            pCop->SetObjective(objectives_enum::guard_spot_24, 0);
            pCop->set_remap_433B90(8);
            pCop->field_26C_graphic_type = 1;
            pCop->ForceWeapon_46F600(weapon_type::silence_smg);
            pCop->set_health_4039A0(200);
            pCop->field_288_threat_search = threat_search_enum::area_2;
            pCop->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
            break;
        case 1:
            pCop = gPedManager_6787BC->SpawnPedAt(xpos, ypos, zpos, 0, rotation);
            pCop->SetField238_403920(ped_type::special_ped_4);
            pCop->set_occupation_403970(ped_ocupation_enum::roadblock_cop_37);
            pCop->SetObjective(objectives_enum::guard_spot_24, 0);
            pCop->set_remap_433B90(0);
            pCop->field_26C_graphic_type = 2;
            pCop->field_170_selected_weapon = 0;
            pCop->GiveWeapon_46F650(weapon_type::pistol);
            pCop->set_health_4039A0(200);
            pCop->field_288_threat_search = threat_search_enum::area_2;
            pCop->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
            break;
    }
    return pCop;
}

MATCH_FUNC(0x56f6d0)
void Police_7B8::DespawnCrewInCar_56F6D0(Car_BC* pCar)
{
    u8 bUnknown = 0;

    for (u8 idx = 0; idx < 20; idx++)
    {
        if (field_4_cop_crew[idx].field_1C_used)
        {
            PoliceCrew_38* pCrew = &field_4_cop_crew[idx];
            if (field_4_cop_crew[idx].field_10_subObj && field_4_cop_crew[idx].field_10_subObj->field_0_car == pCar)
            {
                PedGroup* pPedGroup = pCrew->field_10_subObj->field_8_group;

                if (!pPedGroup || pPedGroup->IsAllMembersInSomeCar_4CAA20())
                {
                    switch (pCrew->field_14_pService->field_4_wanted_level)
                    {
                        case 6:

                            if (pCrew->field_20_crew_kind == 1)
                            {
                                bUnknown = 1;
                            }
                            if (pCrew->field_20_crew_kind == 2)
                            {
                                bUnknown = 1;
                            }
                            if (pCrew->field_20_crew_kind == 3)
                            {
                                bUnknown = 1;
                            }

                            break;

                        case 5:

                            if (pCrew->field_20_crew_kind == 1)
                            {
                                bUnknown = 1;
                            }
                            if (pCrew->field_20_crew_kind == 2)
                            {
                                bUnknown = 1;
                            }
                            break;

                        default:
                            return;
                    }
                    if (!bUnknown)
                    {
                        return;
                    }
                }

                idx = 0;
                if (pCrew->field_10_subObj->field_8_group)
                {
                    for (Ped* pPedIter = pCrew->field_10_subObj->field_8_group->field_4_ped_list[idx]; pPedIter;
                         pPedIter = pCrew->field_10_subObj->field_8_group->field_4_ped_list[++idx])
                    {
                        if (pPedIter->field_168_game_object)
                        {
                            pPedIter->Deallocate_45EB60();
                        }
                    }
                }

                pCrew->RemoveFromService_570AB0();
                Car_BC* pCrewCar = pCrew->field_10_subObj->field_0_car;

                pCrewCar->MarkForDespawn_421470();
                pCrew->field_10_subObj->field_28_state = 5;
                pCrew->field_10_subObj->field_2C = 1;
                pCrew->field_24_state = police_crew_state::shutdown_6;
                return;
            }
        }
    }
}

MATCH_FUNC(0x56f800)
bool Police_7B8::HasCriminalBeenFound_56F800(Ped* pCriminal)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464_services[i].field_0_criminal_ped == pCriminal)
        {
            if (field_464_services[i].field_75_count > 0 && (field_464_services[i].field_8_state == 3 || field_464_services[i].field_C_timer != 0))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x56f880)
bool Police_7B8::IsPedActiveCriminal_56F880(Ped* pCriminal)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464_services[i].field_0_criminal_ped == pCriminal)
        {
            if (field_464_services[i].field_78)
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

MATCH_FUNC(0x56f8e0)
void Police_7B8::SetArrestedPed_56F8E0(Ped* pCriminal, Ped* pUnusedPed)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464_services[i].field_0_criminal_ped == pCriminal)
        {
            field_464_services[i].field_0_criminal_ped = pCriminal;
            return;
        }
    }
}

MATCH_FUNC(0x56f940)
void Police_7B8::RegisterCriminal_56F940(Ped* pPed)
{
    bHaveCriminals_6FEE44 = 0;
    if (pPed->is_player_41B0A0())
    {
        bool bFound = false;
        for (u8 idx = 0; idx < GTA2_COUNTOF(field_464_services); idx++)
        {
            if (field_464_services[idx].field_0_criminal_ped == pPed)
            {
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            for (u8 i = 0; i < GTA2_COUNTOF(field_464_services); i++)
            {
                if (field_464_services[i].field_0_criminal_ped == NULL)
                {
                    field_464_services[i].field_0_criminal_ped = pPed;
                    field_464_services[i].field_8_state = 0;
                    field_464_services[i].field_10_x = pPed->get_cam_x();
                    field_464_services[i].field_14_y = pPed->get_cam_y();
                    field_464_services[i].field_18_z = pPed->get_cam_z();
                    break;
                }
            }
        }

        for (u8 j = 0; j < GTA2_COUNTOF(field_464_services); j++)
        {
            if (field_464_services[j].field_0_criminal_ped != NULL)
            {
                bHaveCriminals_6FEE44 = 1;
                return;
            }
        }
    }
}

MATCH_FUNC(0x56fa40)
void Police_7B8::UpdatePlayerServiceTimer_56FA40()
{
    if (field_464_services[0].field_0_criminal_ped)
    {
        if (!field_464_services[0].field_0_criminal_ped->CheckBit0_433B40() 
            || field_464_services[0].field_0_criminal_ped->isDead_403B60())
        {
            field_464_services[0].field_8_state = 4;
        }
        else
        {
            if (field_464_services[0].field_C_timer > 0)
            {
                field_464_services[0].field_C_timer--;
            }

            if (field_464_services[0].field_8_state == 3 && field_464_services[0].field_C_timer == 0)
            {
                field_464_services[0].field_8_state = 5;
            }
        }
    }
}

MATCH_FUNC(0x56faa0)
char_type Police_7B8::DispatchNewCrewToService_56FAA0(Police_7C* pService)
{
    u8 tileX = pService->field_10_x.ToInt();
    u8 tileY = pService->field_14_y.ToInt();
    u8 tileZ = pService->field_18_z.ToInt();

    if (gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &tileX, &tileY, &tileZ, 0))
    {
        PoliceCrew_38* pNewPoliceCrew = Police_7B8::New_56F560();
        pNewPoliceCrew->field_1C_used = 1;
        pNewPoliceCrew->field_2_targ_x = tileX;
        pNewPoliceCrew->field_3_targ_y = tileY;
        pNewPoliceCrew->field_4_targ_z = tileZ;
        Kfc_30* pNewKfc = gKfc_1E0_706280->New_5CBB80();
        pNewPoliceCrew->field_10_subObj = pNewKfc;
        if (!pNewPoliceCrew->field_10_subObj)
        {
            pNewPoliceCrew->Init_5709C0();
            return 0;
        }
        pNewPoliceCrew->field_0_id = id_counter_6FEE46++; // TODO: types
        Kfc_30* pKfc = pNewPoliceCrew->field_10_subObj;
        pNewPoliceCrew->field_14_pService = pService;
        pNewPoliceCrew->field_24_state = dword_6FEDCC;
        pNewPoliceCrew->field_20_crew_kind = gRoadblockGuardType_6FEDB8;
        pKfc->field_1E_is_used = 1;
        pKfc->field_20_crew_type = dword_6FEE18; // field_20_crew_type
        pKfc->field_24 = 1;
        pKfc->field_28_state = 3;
        pKfc->field_18 = word_6FEAC8;
        pKfc->field_C_x = Fix16(tileX);
        pKfc->field_10_y = Fix16(tileY);
        pKfc->field_14_z = Fix16(tileZ);
        pNewPoliceCrew->AddToService_570A10();
        return 1;
    }
    return 0;
}

DEFINE_GLOBAL_INIT(Fix16, dword_6FECFC, Fix16(5), 0x6FECFC);

// Updates every call for service: its wanted level from the criminal's stars, then its state
// (send crews, escalate, give up, clean up when the criminal is gone).
MATCH_FUNC(0x56fbd0)
void Police_7B8::UpdateServices_56FBD0()
{
    u8 numCrews;
    u8 crewIdx;
    u8 serviceIdx = 0;
    Police_7C* pService = &field_464_services[0];
    while (pService->field_0_criminal_ped && serviceIdx < 4)
    {
        pService->field_78 = 0;
        Ped* pCriminal = pService->field_0_criminal_ped;
        if (pCriminal->GetPedType_420B70() == 2 && (pCriminal->field_21C & 0x20) == 0x20)
        {
            pCriminal->field_15C_player->field_640_busted = 1;
        }

        switch (pService->field_0_criminal_ped->get_wanted_star_count_46EF00())
        {
            case 0:
                if (pService->field_8_state)
                {
                    pService->field_8_state = 4;
                    field_659_max_police_cars = 2;
                    pService->field_4_wanted_level = 0;
                    pService->field_71_num_unknown = 0;
                }
                break;
            case 1:
                pService->field_71_num_unknown = 1;
                pService->field_4_wanted_level = 1;
                if (pService->field_70_num_police_crews < 1)
                {
                    field_659_max_police_cars = 1;
                }
                else
                {
                    field_659_max_police_cars = 0;
                }
                break;
            case 2:
                if (pService->field_71_num_unknown == 1)
                {
                    pService->field_E = 1;
                }
                pService->field_71_num_unknown = 2;
                pService->field_4_wanted_level = 2;
                if (pService->field_70_num_police_crews <= 1)
                {
                    field_659_max_police_cars = 2;
                }
                else
                {
                    field_659_max_police_cars = 0;
                }
                break;
            case 3:
                pService->field_71_num_unknown = 2;
                pService->field_4_wanted_level = 3;
                if (pService->field_70_num_police_crews <= 1)
                {
                    field_659_max_police_cars = 2;
                }
                else
                {
                    field_659_max_police_cars = 0;
                }
                break;
            case 4:
                pService->field_71_num_unknown = 2;
                pService->field_4_wanted_level = 4;
                if (pService->field_70_num_police_crews <= 1)
                {
                    field_659_max_police_cars = 2;
                }
                else
                {
                    field_659_max_police_cars = 0;
                }
                break;
            case 5:
                pService->field_4_wanted_level = 5;
                gPolice_7B8_6FEE40->field_65C_highest_crew_type_on_service = 4;
                if (!pService->field_70_num_police_crews && !pService->field_72_num_swat_crews && pService->field_73_num_fbi_crews <= 1)
                {
                    field_659_max_police_cars = 2;
                }
                else
                {
                    field_659_max_police_cars = 0;
                }
                break;
            case 6:
                pService->field_71_num_unknown = 0;
                pService->field_4_wanted_level = 6;
                gPolice_7B8_6FEE40->field_65C_highest_crew_type_on_service = 6;
                break;
        }

        if (pService->field_4_wanted_level > field_654_max_wanted_level)
        {
            field_654_max_wanted_level = pService->field_4_wanted_level;
        }

        pCriminal = pService->field_0_criminal_ped;
        if (!pCriminal->CheckBit0_433B40() || pCriminal->isDead_403B60())
        {
            pService->field_8_state = 4;
        }

        if (pService->field_4_wanted_level == 1)
        {
            if (!HasCriminalBeenFound_56F800(pService->field_0_criminal_ped))
            {
                if ((u16)pService->field_7A_wanted_timer >= 900)
                {
                    pService->field_0_criminal_ped->ClearWantedPoints_420B80();
                    pService->field_0_criminal_ped->field_20A_wanted_points = 0;
                    pService->field_7A_wanted_timer = 0;
                    return;
                }
                pService->field_7A_wanted_timer++;
            }
            else
            {
                pService->field_7A_wanted_timer = 0;
            }
        }

        switch (pService->field_8_state)
        {
            case 0:
                if (pService->field_4_wanted_level > 0)
                {
                    if (!pService->field_70_num_police_crews)
                    {
                        if (!field_658_police_car_count)
                        {
                            word_6FEAC8 = 200;
                            dword_6FEE18 = 3;
                            dword_6FEDCC = 3;
                            gRoadblockGuardType_6FEDB8 = 1;
                            if (gPolice_7B8_6FEE40->DispatchNewCrewToService_56FAA0(pService))
                            {
                                pService->field_8_state = 1;
                                field_659_max_police_cars = 0;
                            }
                        }
                    }
                    else
                    {
                        pService->field_8_state = 3;
                    }
                }
                break;

            case 1:
                field_659_max_police_cars = 0;
                if (pService->field_C_timer == 250)
                {
                    pService->field_8_state = 3;
                    for (crewIdx = 0; crewIdx < pService->field_75_count; crewIdx++)
                    {
                        pService->field_20_crews[crewIdx]->field_24_state = 5;
                    }
                }
                break;

            case 3:
                pService->field_1C = 0;
                switch (pService->field_4_wanted_level)
                {
                    case 3:
                        field_659_max_police_cars = 0;
                        if (pService->field_70_num_police_crews < pService->field_71_num_unknown)
                        {
                            dword_6FEE18 = 3;
                            word_6FEAC8 = 50;
                            dword_6FEDCC = 5;
                            gRoadblockGuardType_6FEDB8 = 1;
                            gPolice_7B8_6FEE40->DispatchNewCrewToService_56FAA0(pService);
                        }
                        break;
                    case 4:
                        if (pService->field_70_num_police_crews < pService->field_71_num_unknown)
                        {
                            dword_6FEE18 = 3;
                            word_6FEAC8 = 50;
                            dword_6FEDCC = 5;
                            gRoadblockGuardType_6FEDB8 = 1;
                            gPolice_7B8_6FEE40->DispatchNewCrewToService_56FAA0(pService);
                        }
                        if (!pService->field_72_num_swat_crews)
                        {
                            word_6FEAC8 = 50;
                            dword_6FEE18 = 5;
                            dword_6FEDCC = 5;
                            gRoadblockGuardType_6FEDB8 = 2;
                            if (gPolice_7B8_6FEE40->DispatchNewCrewToService_56FAA0(pService))
                            {
                                pService->field_72_num_swat_crews = 1;
                            }
                        }
                        break;
                    case 5:
                        if (pService->field_70_num_police_crews > 0)
                        {
                            numCrews = pService->field_75_count;
                            for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                            {
                                pService->field_20_crews[0]->field_34 = 1;
                                pService->field_20_crews[0]->RemoveFromService_570AB0();
                            }
                            pService->field_70_num_police_crews = 0;
                            pService->field_71_num_unknown = 0;
                            pService->field_72_num_swat_crews = 0;
                            gPolice_7B8_6FEE40->field_65C_highest_crew_type_on_service = 4;
                        }
                        break;
                    case 6:
                        if (pService->field_70_num_police_crews > 0 || pService->field_72_num_swat_crews || pService->field_73_num_fbi_crews)
                        {
                            numCrews = pService->field_75_count;
                            for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                            {
                                pService->field_20_crews[0]->RemoveFromService_570AB0();
                            }
                            pService->field_70_num_police_crews = 0;
                            pService->field_71_num_unknown = 0;
                            bHaveCriminals_6FEE44--;
                            gPolice_7B8_6FEE40->field_65C_highest_crew_type_on_service = 6;
                        }
                        break;
                }

                if (pService->field_70_num_police_crews > pService->field_71_num_unknown)
                {
                    numCrews = pService->field_75_count;
                    for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                    {
                        PoliceCrew_38* pCrew = pService->field_20_crews[crewIdx];
                        if (pCrew && pCrew->field_1C_used == 1 && pCrew->field_20_crew_kind == 1 && pCrew->field_10_subObj->field_0_car)
                        {
                            pService->field_20_crews[crewIdx]->field_34 = 1;
                            pService->field_20_crews[crewIdx]->RemoveFromService_570AB0();
                            break;
                        }
                    }
                }
                break;

            case 5:
                if (pService->field_C_timer > 0)
                {
                    pService->field_8_state = 3;
                }
                else if (pService->field_4_wanted_level == 5)
                {
                    pService->field_8_state = 3;
                }
                else if (!pService->field_1C)
                {
                    u8 bNoneSearching = 1;
                    numCrews = pService->field_75_count;
                    for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                    {
                        PoliceCrew_38* pCrew = pService->field_20_crews[crewIdx];
                        if (pCrew && pCrew->field_24_state == 3)
                        {
                            pCriminal = pService->field_0_criminal_ped;
                            if (pCriminal)
                            {
                                Fix16 dy = pCriminal->field_1AC_cam.y - pService->field_14_y;
                                Fix16 dx = pCriminal->field_1AC_cam.x - pService->field_10_x;
                                dy = Fix16::Abs(dy);
                                dx = Fix16::Abs(dx);
                                if (!(dx > dy))
                                {
                                    dx = dy;
                                }
                                if (dx > dword_6FECFC)
                                {
                                    pCrew->RemoveFromService_570AB0();
                                    pService->field_1C = 1;
                                }
                            }
                            bNoneSearching = 0;
                        }
                    }
                    if (bNoneSearching && !field_658_police_car_count)
                    {
                        word_6FEAC8 = 200;
                        dword_6FEE18 = 3;
                        dword_6FEDCC = 3;
                        gRoadblockGuardType_6FEDB8 = 1;
                        gPolice_7B8_6FEE40->DispatchNewCrewToService_56FAA0(pService);
                    }
                }
                break;

            case 4:
            {
                numCrews = pService->field_75_count;
                for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                {
                    PoliceCrew_38* pCrew = pService->field_20_crews[0];
                    if (!pCrew->field_1C_used)
                    {
                        if (pService->field_75_count > 0)
                        {
                            pService->field_20_crews[0] = pService->field_20_crews[pService->field_75_count - 1];
                            pService->field_20_crews[pService->field_75_count - 1] = NULL;
                            pService->field_75_count--;
                        }
                        else
                        {
                            pService->field_20_crews[0] = NULL;
                        }
                    }
                    else
                    {
                        pCrew->RemoveFromService_570AB0();
                    }
                }
                pService->field_0_criminal_ped = NULL;
                pService->field_70_num_police_crews = 0;
                pService->field_71_num_unknown = 0;
                pService->field_75_count = 0;
                pService->field_76 = 0;
                pService->field_E = 0;
                bHaveCriminals_6FEE44--;
                gPolice_7B8_6FEE40->field_65C_highest_crew_type_on_service = 3;
                break;
            }
        }

        if (++serviceIdx < 4)
        {
            pService = &field_464_services[serviceIdx];
        }
    }
}

MATCH_FUNC(0x570270)
void Police_7B8::Service_570270()
{
    field_7B4_crew_ped_onscreen = 0;
    field_654_max_wanted_level = 0;

    if (bHaveCriminals_6FEE44 == 1)
    {
        Police_7B8::UpdateServices_56FBD0();
    }

    for (s32 i = 0; i < GTA2_COUNTOF(field_4_cop_crew); i++)
    {
        if (field_4_cop_crew[i].field_1C_used == 1)
        {
            field_4_cop_crew[i].Service_575590();
        }
    }

    if (bHaveCriminals_6FEE44 == 1)
    {
        Police_7B8::UpdatePlayerServiceTimer_56FA40();
    }

    field_664_roadblock_1.Update_5757B0();
    field_708_roadblock_2.Update_5757B0();

    if (field_7AC_roadblock_cooldown > 0)
    {
        field_7AC_roadblock_cooldown--;
    }

    if (field_7B0_last_firing_emergency_ped != NULL)
    {
        if (field_7B0_last_firing_emergency_ped->GetPedState_403990() == 9)
        {
            field_7B0_last_firing_emergency_ped = NULL;
        }
        else if (!field_7B0_last_firing_emergency_ped->CheckBit0_433B40())
        {
            field_7B0_last_firing_emergency_ped = NULL;
        }
        else if (field_7B0_last_firing_emergency_ped->field_21C_bf.b11 == 0)
        {
            field_7B0_last_firing_emergency_ped = NULL;
        }
    }
}

MATCH_FUNC(0x570320)
void Police_7B8::SpawnWalkingGuard_570320(Ped* pPed, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    if (field_65C_highest_crew_type_on_service == crew_type::army_6)
    {
        pPed->set_occupation_403970(ped_ocupation_enum::unknown_cop_occu_31);
        pPed->SetField238_403920(3);
        pPed->set_remap_433B90(ped_remap_enum::ped_remap_army);
    }
    else
    {
        pPed->set_occupation_403970(ped_ocupation_enum::walking_guard_29);
        pPed->SetField238_403920(3);
        pPed->set_remap_433B90(ped_remap_enum::ped_remap_blue_police);
    }
    pPed->field_26C_graphic_type = 2;
    pPed->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pPed->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed->AllocCharB4_45C830(xpos, ypos, zpos);

    Char_B4* pCharObj = pPed->field_168_game_object;
    u8 remap = pPed->field_244_remap;
    pCharObj->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pCharObj->field_80_sprite_ptr->SetRemap(remap);
    }
    pPed->SetRotation_433C00(rotation);
    pPed->sub_467280();
}

MATCH_FUNC(0x5703e0)
bool Police_7B8::SpawnCrewInCar_5703E0(Car_BC* pCar)
{
    if (gPedManager_6787BC->field_5_fbi_army_count >= 30)
    {
        return false;
    }
    if (field_658_police_car_count > 2)
    {
        return false;
    }
    PoliceCrew_38* pNewCrew = Police_7B8::New_56F560();
    pNewCrew->field_1C_used = 1;
    Kfc_30* pNewKfc = gKfc_1E0_706280->New_5CBB80();
    pNewCrew->field_10_subObj = pNewKfc;
    if (!pNewKfc)
    {
        pNewCrew->Init_5709C0();
        return false;
    }
    pNewCrew->field_0_id = id_counter_6FEE46++; // u16 type
    Kfc_30* pKfc = pNewCrew->field_10_subObj;
    pNewCrew->field_24_state = police_crew_state::patrol_1;
    pNewCrew->field_29_bCountedInPoliceCount = 1;
    pKfc->field_1E_is_used = 1;
    pKfc->field_20_crew_type = gPolice_7B8_6FEE40->field_65C_highest_crew_type_on_service;
    pKfc->field_24 = 1;
    pKfc->field_0_car = pCar;
    PedGroup* pNewPedGroup = PedGroup::New_4CB0D0();
    Ped* pNewPed1 = gPedManager_6787BC->AllocatePed_470F30();
    pNewPed1->SetField238_403920(4);
    pNewPed1->set_occupation_403970(ped_ocupation_enum::police);
    pNewPed1->SpawnPedInCar_45C730(pKfc->field_0_car);
    pNewPed1->SetObjective(objectives_enum::objective_43, 9999);
    pNewPed1->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pNewPed1->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    Ped* pNewPed2 = gPedManager_6787BC->AllocatePed_470F30();
    pNewPed2->SetObjective(objectives_enum::no_obj_0, 9999);
    pNewPed2->EnterCarAsPassenger_45C7F0(pKfc->field_0_car);
    pNewPed2->SetField238_403920(4);
    pNewPed2->set_occupation_403970(ped_ocupation_enum::police);
    pNewPed2->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pNewPed2->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;

    switch (gPolice_7B8_6FEE40->field_65C_highest_crew_type_on_service)
    {
        case crew_type::fbi_4:
            // ok
            pNewPed1->set_health_4039A0(250);
            pNewPed1->ForceWeapon_46F600(weapon_type::shotgun);
            pNewPed1->GiveWeapon_46F650(weapon_type::silence_smg);
            pNewPed1->set_occupation_403970(ped_ocupation_enum::fbi);
            pNewPed1->set_remap_433B90(8);
            pNewPed1->field_26C_graphic_type = 1;
            pNewPed2->set_health_4039A0(250);
            pNewPed2->set_remap_433B90(8);
            pNewPed2->ForceWeapon_46F600(weapon_type::silence_smg);
            pNewPed2->field_26C_graphic_type = 1;
            pNewPed2->set_occupation_403970(ped_ocupation_enum::fbi);
            pNewCrew->field_20_crew_kind = 3;
            break;

        case crew_type::police_3:

            switch (field_654_max_wanted_level)
            {
                case 0:
                case 1:
                    pNewPed1->field_170_selected_weapon = 0;
                    pNewPed1->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed1->set_health_4039A0(50);
                    pNewPed1->field_1F0_maybe_max_speed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
                    pNewPed2->field_170_selected_weapon = 0;
                    pNewPed2->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed2->set_health_4039A0(50);
                    pNewPed2->field_1F0_maybe_max_speed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;

                    break;

                case 2:
                    // line 231
                    pNewPed1->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed1->set_health_4039A0(100);
                    pNewPed1->field_1F0_maybe_max_speed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
                    pNewPed2->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed2->set_health_4039A0(100);

                    pNewPed2->field_1F0_maybe_max_speed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;

                    break;

                default:
                    // ok
                    pNewPed1->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed1->set_health_4039A0(100);
                    pNewPed2->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed2->set_health_4039A0(100);

                    break;
            }

            pNewPed1->set_occupation_403970(ped_ocupation_enum::police);
            pNewPed1->set_remap_433B90(ped_remap_enum::ped_remap_blue_police);
            pNewPed1->field_26C_graphic_type = 2;
            pNewPed2->set_remap_433B90(ped_remap_enum::ped_remap_blue_police);
            pNewPed2->field_26C_graphic_type = 2;
            pNewCrew->field_20_crew_kind = 1;

            break;

        default:
            pNewPed1->set_health_4039A0(250);
            pNewPed1->ForceWeapon_46F600(weapon_type::smg);
            pNewPed1->set_remap_433B90(4);
            pNewPed1->set_occupation_403970(ped_ocupation_enum::army_army);
            pNewPed1->field_26C_graphic_type = 2;
            pNewPed2->set_health_4039A0(250);
            pNewPed2->ForceWeapon_46F600(weapon_type::smg);
            pNewPed2->set_remap_433B90(4);
            pNewPed2->set_occupation_403970(ped_ocupation_enum::army_army);
            pNewPed2->field_26C_graphic_type = 2;
            pNewCrew->field_20_crew_kind = 4;
            break;
    }

    pNewPedGroup->add_ped_leader_4C9B10(pNewPed1);
    pNewPedGroup->SetCounts_433360(1);
    pNewPedGroup->add_ped_to_list_4C9B30(pNewPed2, 0);
    pNewPedGroup->field_0 = 0;
    pKfc->field_4_ped = pNewPed1;
    pKfc->field_18 = 0;
    pKfc->field_28_state = 6;
    pKfc->field_0_car->SetUniNum_421560(5);
    pKfc->field_0_car->InitCarAIControl_440590();
    pKfc->field_0_car->sub_43AF40();
    ++field_658_police_car_count;
    return true;
}

MATCH_FUNC(0x570790)
bool Police_7B8::AssignCrewToService_570790(PoliceCrew_38* pCrew, Police_7C* pService)
{
    pCrew->field_14_pService = pService;
    pCrew->field_24_state = police_crew_state::pursue_or_chase_5;
    pCrew->AddToService_570A10();
    return true;
}

// https://decomp.me/scratch/pfRaI
// Inlined search helpers: their NULL results are tested by the caller
static inline Police_7C* FindServiceForCriminal_5707B0(Police_7B8* pThis, Ped* pCriminal)
{
    for (u8 i = 0; i < GTA2_COUNTOF(pThis->field_464_services); i++)
    {
        if (pThis->field_464_services[i].field_0_criminal_ped == pCriminal)
        {
            return &pThis->field_464_services[i];
        }
    }
    return NULL;
}

static inline PoliceCrew_38* FindCrewInCar_5707B0(Police_7B8* pThis, Car_BC* pCar)
{
    for (u8 j = 0; j < GTA2_COUNTOF(pThis->field_4_cop_crew); j++)
    {
        PoliceCrew_38* pCrew = &pThis->field_4_cop_crew[j];
        if (pCrew->field_1C_used && pCrew->field_10_subObj->field_0_car == pCar)
        {
            return pCrew;
        }
    }
    return NULL;
}

MATCH_FUNC(0x5707b0)
bool Police_7B8::PromptCrewAtCarToPurseCriminal_5707B0(Car_BC* pCar, Ped* pCriminal)
{
    if (!pCriminal->is_player_41B0A0())
    {
        return false;
    }

    Police_7C* pService = FindServiceForCriminal_5707B0(this, pCriminal);
    if (pService == NULL)
    {
        return false;
    }

    PoliceCrew_38* pCrew = FindCrewInCar_5707B0(this, pCar);
    if (pCrew == NULL)
    {
        return false;
    }

    if (pCrew->field_10_subObj->field_20_crew_type != crew_type::army_6 && pService->field_4_wanted_level == 6)
    {
        return false;
    }

    pService->field_8_state = 3;
    pCrew->field_14_pService = pService;
    pCrew->field_24_state = police_crew_state::pursue_or_chase_5;
    pCrew->AddToService_570A10();

    if (pCrew->field_10_subObj->field_20_crew_type != crew_type::army_6)
    {
        pCrew->field_10_subObj->field_0_car->ActivateEmergencyLights_43C920();
    }

    return true;
}

MATCH_FUNC(0x5708c0)
void Police_7B8::UpdateLastSeenCoordsForCriminal_5708C0(Ped* pPed)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464_services[i].field_0_criminal_ped == pPed)
        {
            field_464_services[i].field_10_x = pPed->get_cam_x();
            field_464_services[i].field_14_y = pPed->get_cam_y();
            field_464_services[i].field_18_z = pPed->get_cam_z();
            field_464_services[i].field_C_timer = 250;
            return;
        }
    }
}

MATCH_FUNC(0x570940)
void Police_7B8::UpdateCriminalLatestPosition_570940(Ped* pPed)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464_services[i].field_0_criminal_ped == pPed)
        {
            field_464_services[i].field_10_x = pPed->get_cam_x();
            field_464_services[i].field_14_y = pPed->get_cam_y();
            field_464_services[i].field_18_z = pPed->get_cam_z();
            return;
        }
    }
}

MATCH_FUNC(0x577320)
char_type Police_7B8::ShouldCreateRoadblock_577320()
{
    if (this->field_654_max_wanted_level < 3 || this->field_664_roadblock_1.field_0_bActive || this->field_7AC_roadblock_cooldown)
    {
        return 0;
    }
    this->field_7AC_roadblock_cooldown = 40;
    return 1;
}

MATCH_FUNC(0x577370)
void Police_7B8::TryCreateRoadblockAt_577370(u8 tileX, u8 tileY, s32 roadblock_type)
{
    bool bBothSides = false;
    switch (field_654_max_wanted_level)
    {
        case 3:
            gRoadblockGuardType_6FEDB8 = 1;
            break;
        case 4:
            gRoadblockGuardType_6FEDB8 = 1;
            break;
        case 5:
            gRoadblockGuardType_6FEDB8 = 3;
            break;
        case 6:
            gRoadblockGuardType_6FEDB8 = 4;
            break;
    }

    if (roadblock_type > 0 && roadblock_type <= 2)
    {
        bBothSides = true;
    }

    u8 tileZ = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(tileX, (s32)tileY).ToUInt8();

    if (bBothSides)
    {
        if (!field_664_roadblock_1.field_0_bActive)
        {
            field_664_roadblock_1.CreateRoadblock_575FF0(tileX, tileY, tileZ, 3);
        }
        else if (!field_708_roadblock_2.field_0_bActive)
        {
            field_708_roadblock_2.CreateRoadblock_575FF0(tileX, tileY, tileZ, 3);
        }
    }
    else
    {
        if (!field_664_roadblock_1.field_0_bActive)
        {
            field_664_roadblock_1.CreateRoadblock_575FF0(tileX, tileY, tileZ, 2);
        }
        else if (!field_708_roadblock_2.field_0_bActive)
        {
            field_708_roadblock_2.CreateRoadblock_575FF0(tileX, tileY, tileZ, 3);
        }
    }
}