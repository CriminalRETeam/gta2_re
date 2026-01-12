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
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "RouteFinder.hpp"

DEFINE_GLOBAL(Fix16, dword_6FECE8, 0x6FECE8);
DEFINE_GLOBAL(Fix16, dword_6FED54, 0x6FED54);
DEFINE_GLOBAL(Ped*, pPed_6FEDDC, 0x6FEDDC);
DEFINE_GLOBAL(u8, byte_6FEB48, 0x6FEB48);

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
    field_24_state = 0;
    field_28 = 0;
    field_8 = dword_6FECE8;
    field_C = dword_6FECE8;
    field_14_pObj = 0;
    field_1A = 0;
    field_29 = 0;
    field_2A = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
}

MATCH_FUNC(0x570a10)
void PoliceCrew_38::sub_570A10()
{
    if (field_14_pObj->field_75_count < 6)
    {
        for (u8 i = 0; i < 6; i++)
        {
            if (field_14_pObj->field_20[i] == this)
            {
                return;
            }
        }
        field_14_pObj->field_20[field_14_pObj->field_75_count] = this;
        ++field_14_pObj->field_75_count;
        switch (field_10_subObj->field_20_maybe_type)
        {
            case 3:
                ++field_14_pObj->field_70;
                break;
            case 5:
                ++field_14_pObj->field_72;
                break;
            case 4:
                ++field_14_pObj->field_73;
                break;
            case 6:
                ++field_14_pObj->field_74;
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x570ab0)
void PoliceCrew_38::sub_570AB0()
{
    if (!field_1C_used || (field_24_state != 0 && field_24_state != 1 && field_24_state != 6))
    {
        u8 last_idx = field_14_pObj->field_75_count - 1;
        PoliceCrew_38* pPolice38_last = field_14_pObj->field_20[last_idx];
        if (pPolice38_last == this)
        {
            field_14_pObj->field_20[last_idx] = NULL;
        }
        else
        {
            for (u8 i = 0; i < last_idx; i++)
            {
                if (field_14_pObj->field_20[i] == this)
                {
                    field_14_pObj->field_20[i] = pPolice38_last;
                    field_14_pObj->field_20[last_idx] = NULL;
                    break;
                }
            }
        }
        if (field_1C_used)
        {
            switch (field_20)
            {
                case 1:
                    --field_14_pObj->field_70;
                    break;
                case 2:
                    --field_14_pObj->field_72;
                    break;
                case 3:
                    --field_14_pObj->field_73;
                    break;
                case 4:
                    --field_14_pObj->field_74;
                    break;
                default:
                    break;
            }
        }
        --field_14_pObj->field_75_count;
        field_24_state = 6;
    }
}

MATCH_FUNC(0x570bf0)
void PoliceCrew_38::SpawnPoliceInCar_570BF0()
{
    PedGroup* pGroup = PedGroup::New_4CB0D0();
    Ped* pCopLeader = gPedManager_6787BC->sub_470F30();
    pCopLeader->field_238 = 4;
    pCopLeader->field_240_occupation = ped_ocupation_enum::police;
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
            pCopLeader->field_1F0_maybe_max_speed = dword_6FEB0C * dword_6FEB68;
            break;
        case 2:
            pCopLeader->GiveWeapon_46F650(weapon_type::pistol);
            pCopLeader->set_health_4039A0(100);
            pCopLeader->field_1F0_maybe_max_speed = dword_6FEB0C * dword_6FEB68;
            break;
        default:
            pCopLeader->GiveWeapon_46F650(weapon_type::pistol);
            pCopLeader->set_health_4039A0(100);
            break;
    }

    pCopLeader->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pCopLeader->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;

    Ped* pCopSupporter = gPedManager_6787BC->sub_470F30();
    pCopSupporter->sub_45C7F0(field_10_subObj->field_0_car);
    pCopSupporter->field_238 = 4;
    pCopSupporter->field_240_occupation = ped_ocupation_enum::police;
    pCopSupporter->SetObjective(objectives_enum::no_obj_0, 9999);
    pCopSupporter->field_244_remap = 0;

    switch (field_14_pObj->field_4)
    {
        case 1:
            pCopSupporter->field_170_selected_weapon = 0;
            pCopSupporter->GiveWeapon_46F650(weapon_type::pistol);
            pCopSupporter->field_216_health = 50;
            pCopSupporter->field_1F0_maybe_max_speed = dword_6FEB0C * dword_6FEB68;
            break;
        case 2:
            pCopSupporter->GiveWeapon_46F650(weapon_type::pistol);
            pCopSupporter->field_216_health = 100;
            pCopSupporter->field_1F0_maybe_max_speed = dword_6FEB0C * dword_6FEB68;
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
    pGroup->field_36_count = 1;
    pGroup->field_34_count = 1;
    pGroup->add_ped_to_list_4C9B30(pCopSupporter, 0);
    pGroup->field_0 = 0;
    field_10_subObj->field_4_ped = pCopLeader;
    field_10_subObj->field_28 = 6;
    field_10_subObj->field_0_car->sub_421560(5);
    field_10_subObj->field_0_car->InitCarAIControl_440590();
    field_10_subObj->field_0_car->sub_43AF40();
    field_10_subObj->field_0_car->ActivateEmergencyLights_43C920();
    field_10_subObj->field_8_group = pGroup;
}

MATCH_FUNC(0x570e30)
void PoliceCrew_38::SpawnSWAT_570E30()
{
    PedGroup* pSwatGroup = PedGroup::New_4CB0D0();
    Ped* pSwatLeader = gPedManager_6787BC->sub_470F30();
    pSwatLeader->field_238 = 4;
    pSwatLeader->field_240_occupation = ped_ocupation_enum::swat;
    pSwatLeader->SpawnPedInCar_45C730(field_10_subObj->field_0_car);
    pSwatLeader->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pSwatLeader->field_1DC_objective_target_x = Fix16(field_2_targ_x);
    pSwatLeader->field_1E0_objective_target_y = Fix16(field_3_targ_y);
    pSwatLeader->field_1E4_objective_target_z = Fix16(field_4_targ_z);
    pSwatLeader->field_244_remap = -1;
    pSwatLeader->ForceWeapon_46F600(weapon_type::pistol);
    pSwatLeader->field_216_health = 400;
    pSwatLeader->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pSwatLeader->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pSwatLeader->field_26C_graphic_type = 2;
    pSwatGroup->add_ped_leader_4C9B10(pSwatLeader);
    pSwatGroup->field_36_count = 3;
    pSwatGroup->field_34_count = 3;
    for (u8 i = 0; i < 3; ++i)
    {
        Ped* pSwatMember = gPedManager_6787BC->sub_470F30();
        pSwatMember->sub_45C7F0(field_10_subObj->field_0_car);
        pSwatMember->field_238 = 4;
        pSwatMember->field_240_occupation = ped_ocupation_enum::swat;
        pSwatMember->SetObjective(objectives_enum::no_obj_0, 9999);
        pSwatMember->field_244_remap = -1;
        pSwatMember->ForceWeapon_46F600(weapon_type::pistol);
        pSwatMember->field_216_health = 400;
        pSwatMember->field_288_threat_search = threat_search_enum::line_of_sight_1;
        pSwatMember->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
        pSwatMember->field_26C_graphic_type = 2;

        pSwatGroup->add_ped_to_list_4C9B30(pSwatMember, i);
    }
    pSwatGroup->field_0 = 0;
    field_10_subObj->field_4_ped = pSwatLeader;
    field_10_subObj->field_28 = 6;
    field_10_subObj->field_0_car->sub_421560(5);
    field_10_subObj->field_0_car->InitCarAIControl_440590();
    field_10_subObj->field_0_car->sub_43AF40();
    field_10_subObj->field_0_car->ActivateEmergencyLights_43C920();
    field_10_subObj->field_8_group = pSwatGroup;
}

MATCH_FUNC(0x571150)
void PoliceCrew_38::SpawnFBI_nonused_571150()
{
    Ped* pFBI = gPedManager_6787BC->sub_470F30();
    pFBI->field_238 = 4;
    pFBI->field_240_occupation = ped_ocupation_enum::fbi;
    pFBI->SpawnPedInCar_45C730(field_10_subObj->field_0_car);
    pFBI->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pFBI->field_1DC_objective_target_x = Fix16(field_2_targ_x);
    pFBI->field_1E0_objective_target_y = Fix16(field_3_targ_y);
    pFBI->field_1E4_objective_target_z = Fix16(field_4_targ_z);
    pFBI->field_244_remap = 0;
    pFBI->ForceWeapon_46F600(weapon_type::smg);
    pFBI->field_216_health = 200;
    pFBI->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pFBI->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pFBI->field_26C_graphic_type = 1;
    field_10_subObj->field_4_ped = pFBI;
    field_10_subObj->field_28 = 6;
    field_10_subObj->field_0_car->sub_421560(5);
    field_10_subObj->field_0_car->InitCarAIControl_440590();
    field_10_subObj->field_0_car->sub_43AF40();
    field_10_subObj->field_0_car->ActivateEmergencyLights_43C920();
    field_10_subObj->field_8_group = 0;
}

MATCH_FUNC(0x571350)
void PoliceCrew_38::sub_571350()
{
    field_24_state = 2;
    PedGroup* pGroup = field_10_subObj->field_8_group;
    if (pGroup)
    {
        if (pGroup->sub_4C9150())
        {
            u8 v7 = 0;
            for (Ped* pPedIter = field_10_subObj->field_4_ped; pPedIter; pPedIter = field_10_subObj->field_8_group->field_4_ped_list[v7++])
            {
                pPedIter->field_164_ped_group = 0;
                pPedIter->field_23C = 0;
                pPedIter->Deallocate_45EB60();
                if (!field_10_subObj->field_8_group)
                {
                    break;
                }
            }
            field_10_subObj->field_8_group->sub_4C8E90();
            field_10_subObj->field_28 = 5;
            field_10_subObj->field_2C = 1;
        }
    }
    else
    {
        Ped* v6 = field_10_subObj->field_4_ped;
        if (v6)
        {
            if (v6->field_20e >= 0x1Eu)
            {
                v6->Deallocate_45EB60();
                field_10_subObj->field_28 = 5;
                field_10_subObj->field_2C = 1;
            }
        }
        else
        {
            field_10_subObj->field_28 = 5;
            field_10_subObj->field_2C = 1;
        }
    }
}

STUB_FUNC(0x571540)
char_type PoliceCrew_38::sub_571540()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x571a30)
char_type PoliceCrew_38::sub_571A30()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5720c0)
void PoliceCrew_38::sub_5720C0()
{
    if (field_10_subObj->field_24 == 2)
    {
        if (field_29)
        {
            if (gPolice_7B8_6FEE40->field_658_count > 0)
            {
                gPolice_7B8_6FEE40->field_658_count--;
            }
            field_29 = 0;
        }

        if (field_24_state != 6)
        {
            sub_575650();
            field_24_state = 6;
            return;
        }
    }
    else if (field_24_state != 6)
    {
        return;
    }

    if (field_10_subObj->field_24 == 0)
    {
        sub_571350();
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

STUB_FUNC(0x572210)
bool PoliceCrew_38::sub_572210()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x572340)
void PoliceCrew_38::sub_572340()
{
    u8 bUnk = true;
    u8 idx = 0;
    byte_6FEB48 = 1;

    if (field_10_subObj->field_28 != 3)
    {
        if (field_10_subObj->field_28 == 6)
        {
            PoliceCrew_38::sub_5720C0();
        }
    }
    else if (field_10_subObj->field_2C)
    {
        if (gPedManager_6787BC->field_5_fbi_army_count < 0x1Au)
        {
            switch (field_10_subObj->field_20_maybe_type)
            {
                case 3:
                    PoliceCrew_38::SpawnPoliceInCar_570BF0();
                    break;
                case 5:
                    PoliceCrew_38::SpawnSWAT_570E30();
                default:
                    break;
            }
            pPed_6FEDDC = field_10_subObj->field_4_ped;
            field_1A = 0;
        }
        else
        {
            Car_BC* pCar = field_10_subObj->field_0_car;
            if (pCar)
            {
                s32 v7 = pCar->field_88;
                if (v7 != 5 && v7 != 2 && v7 != 3)
                {
                    pCar->field_88 = 4;
                }
                field_10_subObj->field_0_car = 0;
                field_24_state = 6;
                PoliceCrew_38::sub_575650();
            }
        }
    }
    else
    {
        bUnk = false;
        if (field_10_subObj->field_18 == -80)
        {
            field_24_state = 6;
            PoliceCrew_38::sub_575650();
        }
    }

    if (field_24_state != 6)
    {
        if (bUnk)
        {
            Ped* pPed = field_10_subObj->field_4_ped;
            pPed_6FEDDC = pPed;
            if (field_14_pObj->field_C == 250)
            {
                field_24_state = 5;
            }
            else
            {
                for (; pPed; ++idx)
                {
                    if (pPed->field_278 != 9 && pPed->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        Fix16 xpos_f;
                        switch (pPed->field_258_objective)
                        {
                            case objectives_enum::objective_52:
                                pPed->SetObjective(objectives_enum::goto_area_in_car_14, 9999);
                                pPed_6FEDDC->field_1DC_objective_target_x = Fix16(field_14_pObj->field_10_x.ToUInt8());
                                pPed_6FEDDC->field_1E0_objective_target_y = Fix16(field_14_pObj->field_14_y.ToUInt8());
                                pPed_6FEDDC->field_1E4_objective_target_z = Fix16(field_14_pObj->field_18_z.ToUInt8());
                                break;
                            case objectives_enum::goto_area_in_car_14:
                                PoliceCrew_38::sub_5752C0();
                                xpos_f = field_14_pObj->field_10_x;
                                if (pPed_6FEDDC->field_1DC_objective_target_x != field_14_pObj->field_10_x ||
                                    pPed_6FEDDC->field_1E0_objective_target_y != field_14_pObj->field_14_y)
                                {
                                    u8 xpos = xpos_f.ToInt();
                                    u8 ypos = field_14_pObj->field_14_y.ToInt();
                                    u8 zpos = field_14_pObj->field_18_z.ToInt();
                                    if (gOrca_2FD4_6FDEF0->sub_5552B0(1, &xpos, &ypos, &zpos, 0))
                                    {
                                        pPed_6FEDDC->field_1DC_objective_target_x = Fix16(xpos);
                                        field_14_pObj->field_10_x = pPed_6FEDDC->field_1DC_objective_target_x;
                                        pPed_6FEDDC->field_1E0_objective_target_y = Fix16(ypos);
                                        field_14_pObj->field_14_y = pPed_6FEDDC->field_1E0_objective_target_y;
                                        pPed_6FEDDC->field_1E4_objective_target_z = Fix16(zpos);
                                        field_14_pObj->field_18_z = pPed_6FEDDC->field_1E4_objective_target_z;
                                    }
                                }
                                field_28 = 1;
                                break;
                            case objectives_enum::no_obj_0:
                                if (pPed->field_23C == 99)
                                {
                                    if (pPed->field_16C_car)
                                    {
                                        pPed->sub_463830(0, 9999);
                                        pPed_6FEDDC->SetObjective(objectives_enum::leave_car_36, 9999);
                                        pPed_6FEDDC->field_150_target_objective_car = field_10_subObj->field_0_car;
                                        break;
                                    }
                                    else
                                    {
                                        pPed->sub_463830(0, 9999);
                                        pPed_6FEDDC->SetObjective(objectives_enum::goto_area_on_foot_12, 9999);
                                        pPed_6FEDDC->field_1DC_objective_target_x = field_14_pObj->field_10_x;
                                        pPed_6FEDDC->field_1E0_objective_target_y = field_14_pObj->field_14_y;
                                        pPed_6FEDDC->field_1E4_objective_target_z = field_14_pObj->field_18_z;
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
                                if (pPed->field_225 == 2)
                                {
                                    field_24_state = 6;
                                    field_14_pObj->field_1C = 1;
                                }
                                break;
                            case objectives_enum::kill_char_on_foot_20:
                                pPed->sub_463830(0, 9999);
                                pPed_6FEDDC->SetObjective(objectives_enum::goto_area_on_foot_12, 9999);
                                pPed_6FEDDC->field_1DC_objective_target_x = field_14_pObj->field_10_x;
                                pPed_6FEDDC->field_1E0_objective_target_y = field_14_pObj->field_14_y;
                                pPed_6FEDDC->field_1E4_objective_target_z = field_14_pObj->field_18_z;
                                break;
                            case objectives_enum::goto_area_on_foot_12:
                                field_28 = 0;
                                if (pPed_6FEDDC->field_225 == 1)
                                {
                                    pPed_6FEDDC->sub_463830(0, 9999);
                                    pPed_6FEDDC->SetObjective(objectives_enum::objective_51, 200);
                                }
                                break;
                            case objectives_enum::objective_28:
                                if (pPed->field_225)
                                {
                                    pPed->sub_463830(0, 9999);
                                    pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                                }
                                break;
                            case objectives_enum::objective_32:
                                pPed->sub_463830(0, 9999);
                                pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
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
                    pPed_6FEDDC = pPed;
                }
            }
        }
    }
}

STUB_FUNC(0x572920)
void PoliceCrew_38::sub_572920()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x574720)
void PoliceCrew_38::sub_574720()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x574f10)
void PoliceCrew_38::sub_574F10()
{
    Car_BC* pCarUnk;

    byte_6FEB48 = 1;
    u8 idx = 0;

    if (field_10_subObj->field_24 == 2 || !field_10_subObj->field_24 || (pCarUnk = field_10_subObj->field_0_car, pCarUnk->field_76 > 80))
    {
        field_24_state = 6;
    }
    else
    {
        pCarUnk->field_A6 &= ~0x20u;
        if (field_10_subObj->field_28 == 6)
        {
            PoliceCrew_38::sub_5720C0();
        }

        if (field_10_subObj->field_28 == 5)
        {
            Car_BC* pCar = field_10_subObj->field_0_car;
            if (pCar)
            {
                Car_78* v7 = pCar->field_5C;
                if (v7)
                {
                    char field_28_junc_idx = v7->field_28_junc_idx;
                    if (field_28_junc_idx > 0)
                    {
                        gRouteFinder_6FFDC8->CancelRoute_589930(field_28_junc_idx);
                        field_10_subObj->field_0_car->field_5C->field_28_junc_idx = -1;
                    }
                }
            }
            PoliceCrew_38::sub_575650();
            if (field_29)
            {
                if (gPolice_7B8_6FEE40->field_658_count > 0)
                {
                    gPolice_7B8_6FEE40->field_658_count--;
                }
            }
            field_10_subObj->field_28 = 0;
            field_10_subObj->sub_5CBC30();
            PoliceCrew_38::Init_5709C0();
        }
        else
        {
            if (field_14_pObj && field_14_pObj->field_0_criminal_ped && PoliceCrew_38::sub_572210() &&
                (field_10_subObj->field_20_maybe_type == 6 || field_14_pObj->field_4 != 6))
            {
                field_14_pObj->field_10_x = field_14_pObj->field_0_criminal_ped->get_cam_x();
                field_14_pObj->field_14_y = field_14_pObj->field_0_criminal_ped->get_cam_y();
                field_14_pObj->field_18_z = field_14_pObj->field_0_criminal_ped->get_cam_z();
                gPolice_7B8_6FEE40->sub_570790(this, field_14_pObj);
            }
            else
            {
                pPed_6FEDDC = field_10_subObj->field_4_ped;
                pPed_6FEDDC->field_21C_bf.b11 = 0;
                Hamburger_40* v13;
                for (Ped* pPedIter = pPed_6FEDDC; pPedIter; ++idx)
                {
                    switch (pPedIter->field_258_objective)
                    {
                        case objectives_enum::goto_area_in_car_14:
                        case objectives_enum::objective_52:
                            byte_6FEB48 = 0;
                            v13 = field_10_subObj->field_0_car->field_60;
                            if (v13)
                            {
                                gHamburger_500_678E30->Cancel_474CC0(v13);
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
                                    pPedIter->sub_463830(0, 9999);
                                    pPed_6FEDDC->SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                                    pPed_6FEDDC->field_150_target_objective_car = field_10_subObj->field_0_car;
                                    pPed_6FEDDC->field_21C &= ~4u;
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
                            pPedIter->sub_463830(0, 9999);
                            pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
                            break;
                        case objectives_enum::objective_28:
                            if (pPedIter->field_225)
                            {
                                pPedIter->sub_463830(0, 9999);
                                pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
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
                    pPed_6FEDDC = pPedIter;
                }
            }
        }
    }
}

MATCH_FUNC(0x575200)
void PoliceCrew_38::sub_575200()
{
    byte_6FEB48 = 1;
    field_24_state = 6;
}

MATCH_FUNC(0x575210)
void PoliceCrew_38::sub_575210()
{
    if (field_10_subObj->field_24)
    {
        if (pPed_6FEDDC->sub_450CB0())
        {
            pPed_6FEDDC->sub_463830(0, 9999);
            pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
        }
        else
        {
            field_10_subObj->field_0_car->field_A6 |= 0x20u;
        }
    }
    else
    {
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
    }
}

MATCH_FUNC(0x575270)
void PoliceCrew_38::sub_575270()
{
    if (field_10_subObj->field_24)
    {
        if (pPed_6FEDDC->sub_450CB0())
        {
            pPed_6FEDDC->sub_463830(0, 9999);
            pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
        }
    }
    else
    {
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
    }
}

MATCH_FUNC(0x5752c0)
void PoliceCrew_38::sub_5752C0()
{
    if (pPed_6FEDDC->field_225)
    {
        field_10_subObj->field_28 = 6;
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->SetObjective(objectives_enum::no_obj_0, 9999);
    }
    byte_6FEB48 = 1;
}

STUB_FUNC(0x575310)
void PoliceCrew_38::sub_575310()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x575590)
void PoliceCrew_38::Service_575590()
{
    if (field_10_subObj)
    {
        Ped* pPed = field_10_subObj->field_4_ped;
        if (pPed)
        {
            if (!pPed->field_20e && pPed->field_278 != 9 && (pPed->field_21C & 1) != 0)
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
        case 3:
            PoliceCrew_38::sub_572340();
            break;
        case 5:
            PoliceCrew_38::sub_572920();
            break;
        case 6:
            PoliceCrew_38::sub_574720();
            break;
        case 1:
            PoliceCrew_38::sub_574F10();
            break;
        case 2:
            PoliceCrew_38::sub_575200();
            break;
        default:
            return;
    }
}

STUB_FUNC(0x575650)
s32 PoliceCrew_38::sub_575650()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x575710)
void PoliceRoadblock_A4::sub_575710()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_9 = 0;
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
    field_58 = 0;
    field_5C = 0;
    field_60 = 0;
    field_64 = 0;
    field_68 = 0;
    field_6C = 0;
    field_70 = 0;
    field_74 = 0;
    field_78 = 0;
    field_7C = 0;
    field_80 = 0;
    field_84 = 0;
    field_88_guard_1 = 0;
    field_8C_guard_2 = 0;
    field_90_guard_3 = 0;
    field_94_guard_4 = 0;
    field_98_guard_5 = 0;
    field_9C_guard_6 = 0;
}

MATCH_FUNC(0x5757b0)
void PoliceRoadblock_A4::sub_5757B0()
{
    u8 v31 = 1;

    if (field_0)
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
                    if (!field_10_car_1->field_76)
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
                    if (!field_14_car_2->field_76)
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
                    if (!field_18_car_3->field_76)
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
                    if (!field_1C_car_4->field_76)
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
                    if (!field_20_car_5->field_76)
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
                    if (!field_24_car_6->field_76)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_28_barrier_1)
            {
                if (field_28_barrier_1->field_14_id == field_58)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_28_barrier_1->field_4->field_14_xpos.x,
                                                                     field_28_barrier_1->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_2C_barrier_2)
            {
                if (field_2C_barrier_2->field_14_id == field_5C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_2C_barrier_2->field_4->field_14_xpos.x,
                                                                     field_2C_barrier_2->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_30_barrier_3)
            {
                if (field_30_barrier_3->field_14_id == field_60)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_30_barrier_3->field_4->field_14_xpos.x,
                                                                     field_30_barrier_3->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_34_barrier_4)
            {
                if (field_34_barrier_4->field_14_id == field_64)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_34_barrier_4->field_4->field_14_xpos.x,
                                                                     field_34_barrier_4->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_38_barrier_5)
            {
                if (field_38_barrier_5->field_14_id == field_68)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_38_barrier_5->field_4->field_14_xpos.x,
                                                                     field_38_barrier_5->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_3C_barrier_6)
            {
                if (field_3C_barrier_6->field_14_id == field_6C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_3C_barrier_6->field_4->field_14_xpos.x,
                                                                     field_3C_barrier_6->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_40_barrier_7)
            {
                if (field_40_barrier_7->field_14_id == field_70)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_40_barrier_7->field_4->field_14_xpos.x,
                                                                     field_40_barrier_7->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_44_barrier_8)
            {
                if (field_44_barrier_8->field_14_id == field_74)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_44_barrier_8->field_4->field_14_xpos.x,
                                                                     field_44_barrier_8->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_48_barrier_9)
            {
                if (field_48_barrier_9->field_14_id == field_78)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_48_barrier_9->field_4->field_14_xpos.x,
                                                                     field_48_barrier_9->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_4C_barrier_10)
            {
                if (field_4C_barrier_10->field_14_id == field_7C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_4C_barrier_10->field_4->field_14_xpos.x,
                                                                     field_4C_barrier_10->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_50_barrier_11)
            {
                if (field_50_barrier_11->field_14_id == field_80)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_50_barrier_11->field_4->field_14_xpos.x,
                                                                     field_50_barrier_11->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_54_barrier_12)
            {
                if (field_54_barrier_12->field_14_id == field_84)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_54_barrier_12->field_4->field_14_xpos.x,
                                                                     field_54_barrier_12->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_88_guard_1)
            {
                if ((field_88_guard_1->field_21C & 1) == 0)
                {
                    field_88_guard_1 = 0;
                }
                else
                {
                    if (field_88_guard_1->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_8C_guard_2)
            {
                if ((field_8C_guard_2->field_21C & 1) == 0)
                {
                    field_8C_guard_2 = 0;
                }
                else
                {
                    if (field_8C_guard_2->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_90_guard_3)
            {
                if ((field_90_guard_3->field_21C & 1) == 0)
                {
                    field_90_guard_3 = 0;
                }
                else
                {
                    if (field_90_guard_3->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_94_guard_4)
            {
                if ((field_94_guard_4->field_21C & 1) == 0)
                {
                    field_94_guard_4 = 0;
                }
                else
                {
                    if (field_94_guard_4->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_98_guard_5)
            {
                if ((field_98_guard_5->field_21C & 1) == 0)
                {
                    field_98_guard_5 = 0;
                }
                else
                {
                    if (field_98_guard_5->field_20e < 0x50u)
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_9C_guard_6)
            {
                if ((field_9C_guard_6->field_21C & 1) == 0)
                {
                    field_9C_guard_6 = 0;
                }
                else
                {
                    if (field_9C_guard_6->field_20e < 0x50u)
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

                Fix16 fix_y = field_2C4_player_ped->field_1AC_cam.y;
                Fix16 fix_x = field_2C4_player_ped->field_1AC_cam.x;

                Fix16 v29 = Fix16(field_8) - fix_x;
                Fix16 v30 = Fix16(field_9) - fix_y;

                v30 = Fix16::Abs(v30);
                v29 = Fix16::Abs(v29);

                if (v29 <= v30)
                {
                    v29 = v30;
                }
                if (v29 > dword_6FED54)
                {
                    PoliceRoadblock_A4::sub_575CA0();
                    return;
                }
            }
            field_E = 0;
        }
    }
}

MATCH_FUNC(0x575ca0)
void PoliceRoadblock_A4::sub_575CA0()
{
    if (field_10_car_1)
    {
        s32 v3 = field_10_car_1->field_88;
        if (v3 != 5 && v3 != 2 && v3 != 3)
        {
            field_10_car_1->field_88 = 4;
        }
        Car_BC* v4 = field_10_car_1;
        v4->field_7C_uni_num = 3;
        v4->field_76 = 0;
        field_10_car_1 = 0;
    }

    if (field_14_car_2)
    {
        s32 v6 = field_14_car_2->field_88;
        if (v6 != 5 && v6 != 2 && v6 != 3)
        {
            field_14_car_2->field_88 = 4;
        }
        Car_BC* v7 = field_14_car_2;
        v7->field_7C_uni_num = 3;
        v7->field_76 = 0;
        field_14_car_2 = 0;
    }
    if (field_18_car_3)
    {
        s32 v9 = field_18_car_3->field_88;
        if (v9 != 5 && v9 != 2 && v9 != 3)
        {
            field_18_car_3->field_88 = 4;
        }
        Car_BC* v10 = field_18_car_3;
        v10->field_7C_uni_num = 3;
        v10->field_76 = 0;
        field_18_car_3 = 0;
    }

    if (field_1C_car_4)
    {
        s32 v12 = field_1C_car_4->field_88;
        if (v12 != 5 && v12 != 2 && v12 != 3)
        {
            field_1C_car_4->field_88 = 4;
        }
        Car_BC* v13 = field_1C_car_4;
        v13->field_7C_uni_num = 3;
        v13->field_76 = 0;
        field_1C_car_4 = 0;
    }

    if (field_20_car_5)
    {
        s32 v15 = field_20_car_5->field_88;
        if (v15 != 5 && v15 != 2 && v15 != 3)
        {
            field_20_car_5->field_88 = 4;
        }
        Car_BC* v16 = field_20_car_5;
        v16->field_7C_uni_num = 3;
        v16->field_76 = 0;
        field_20_car_5 = 0;
    }

    if (field_24_car_6)
    {
        s32 v18 = field_24_car_6->field_88;
        if (v18 != 5 && v18 != 2 && v18 != 3)
        {
            field_24_car_6->field_88 = 4;
        }
        Car_BC* v19 = field_24_car_6;
        v19->field_7C_uni_num = 3;
        v19->field_76 = 0;
        field_24_car_6 = 0;
    }

    if (field_28_barrier_1)
    {
        if (field_28_barrier_1->field_14_id == field_58)
        {
            field_28_barrier_1->Dealloc_5291B0();
        }
        field_28_barrier_1 = 0;
    }

    if (field_2C_barrier_2)
    {
        if (field_2C_barrier_2->field_14_id == field_5C)
        {
            field_2C_barrier_2->Dealloc_5291B0();
        }
        field_2C_barrier_2 = 0;
    }

    if (field_30_barrier_3)
    {
        if (field_30_barrier_3->field_14_id == field_60)
        {
            field_30_barrier_3->Dealloc_5291B0();
        }
        field_30_barrier_3 = 0;
    }

    if (field_34_barrier_4)
    {
        if (field_34_barrier_4->field_14_id == field_64)
        {
            field_34_barrier_4->Dealloc_5291B0();
        }
        field_34_barrier_4 = 0;
    }

    if (field_38_barrier_5)
    {
        if (field_38_barrier_5->field_14_id == field_68)
        {
            field_38_barrier_5->Dealloc_5291B0();
        }
        field_38_barrier_5 = 0;
    }

    if (field_3C_barrier_6)
    {
        if (field_3C_barrier_6->field_14_id == field_6C)
        {
            field_3C_barrier_6->Dealloc_5291B0();
        }
        field_3C_barrier_6 = 0;
    }

    if (field_40_barrier_7)
    {
        if (field_40_barrier_7->field_14_id == field_70)
        {
            field_40_barrier_7->Dealloc_5291B0();
        }
        field_40_barrier_7 = 0;
    }

    if (field_44_barrier_8)
    {
        if (field_44_barrier_8->field_14_id == field_74)
        {
            field_44_barrier_8->Dealloc_5291B0();
        }
        field_44_barrier_8 = 0;
    }

    if (field_48_barrier_9)
    {
        if (field_48_barrier_9->field_14_id == field_78)
        {
            field_48_barrier_9->Dealloc_5291B0();
        }
        field_48_barrier_9 = 0;
    }

    if (field_4C_barrier_10)
    {
        if (field_4C_barrier_10->field_14_id == field_7C)
        {
            field_4C_barrier_10->Dealloc_5291B0();
        }
        field_4C_barrier_10 = 0;
    }

    if (field_50_barrier_11)
    {
        if (field_50_barrier_11->field_14_id == field_80)
        {
            field_50_barrier_11->Dealloc_5291B0();
        }
        field_50_barrier_11 = 0;
    }

    if (field_54_barrier_12)
    {
        if (field_54_barrier_12->field_14_id == field_84)
        {
            field_54_barrier_12->Dealloc_5291B0();
        }
        field_54_barrier_12 = 0;
    }

    if (field_88_guard_1)
    {
        if (field_88_guard_1->field_20e)
        {
            field_88_guard_1->Deallocate_45EB60();
        }
        else
        {
            field_88_guard_1->field_238 = 3;
        }
        field_88_guard_1 = 0;
    }

    if (field_8C_guard_2)
    {
        if (field_8C_guard_2->field_20e)
        {
            field_8C_guard_2->Deallocate_45EB60();
        }
        else
        {
            field_8C_guard_2->field_238 = 3;
        }
        field_8C_guard_2 = 0;
    }

    if (field_90_guard_3)
    {
        if (field_90_guard_3->field_20e)
        {
            field_90_guard_3->Deallocate_45EB60();
        }
        else
        {
            field_90_guard_3->field_238 = 3;
        }
        field_90_guard_3 = 0;
    }

    if (field_94_guard_4)
    {
        if (field_94_guard_4->field_20e)
        {
            field_94_guard_4->Deallocate_45EB60();
        }
        else
        {
            field_94_guard_4->field_238 = 3;
        }
        field_94_guard_4 = 0;
    }

    if (field_98_guard_5)
    {
        if (field_98_guard_5->field_20e)
        {
            field_98_guard_5->Deallocate_45EB60();
        }
        else
        {
            field_98_guard_5->field_238 = 3;
        }
        field_98_guard_5 = 0;
    }

    if (field_9C_guard_6)
    {
        if (field_9C_guard_6->field_20e)
        {
            field_9C_guard_6->Deallocate_45EB60();
            field_9C_guard_6 = 0;
            field_0 = 0;
            return;
        }
        field_9C_guard_6->field_238 = 3;
        field_9C_guard_6 = 0;
    }
    field_0 = 0;
}

STUB_FUNC(0x575ff0)
char_type PoliceRoadblock_A4::CreateRoadblock_575FF0(u8 a2, s32 a3, u8 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x577480)
PoliceRoadblock_A4::PoliceRoadblock_A4()
{
    sub_575710();
    field_A0_rect = new Fix16_Rect();
}

MATCH_FUNC(0x5774a0)
PoliceRoadblock_A4::~PoliceRoadblock_A4()
{
    delete[] field_A0_rect;
}