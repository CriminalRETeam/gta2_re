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
DEFINE_GLOBAL(u8, byte_6FEE44, 0x6FEE44);
DEFINE_GLOBAL(u16, id_counter_6FEE46, 0x6FEE46);
DEFINE_GLOBAL(s32, dword_6FEDCC, 0x6FEDCC);
DEFINE_GLOBAL(u32, dword_6FEE18, 0x6FEE18);
DEFINE_GLOBAL(s16, word_6FEAC8, 0x6FEAC8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEB68, Fix16(13107, 0), 0x6FEB68);
DEFINE_GLOBAL_INIT(Fix16, dword_6FECA0, Fix16(256, 0), 0x6FECA0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEB88, dword_6FECA0, 0x6FEB88);
DEFINE_GLOBAL_INIT(Fix16, dword_6FECF8, Fix16(4), 0x6FECF8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEB0C, dword_6FECF8* dword_6FEB88, 0x6FEB0C);

EXTERN_GLOBAL(Fix16, dword_6FECE8);

MATCH_FUNC(0x4BEB50)
Police_7B8::~Police_7B8()
{
}

MATCH_FUNC(0x56f400)
void Police_7B8::sub_56F400()
{
    field_0 = 1;
    for (s32 i = 0; i < 4; i++)
    {
        field_464[i].field_0_criminal_ped = 0;
        field_464[i].field_C = 0;
        field_464[i].field_1C = 0;

        field_464[i].field_70 = 0;
        field_464[i].field_71 = 0;
        field_464[i].field_72 = 0;
        field_464[i].field_73 = 0;
        field_464[i].field_74 = 0;
        field_464[i].field_76 = 0;

        field_464[i].field_4 = 0;
        field_464[i].field_8 = 0;

        field_464[i].field_10_x = dword_6FECE8;
        field_464[i].field_14_y = dword_6FECE8;
        field_464[i].field_18_z = dword_6FECE8;

        field_464[i].field_1C = 0;
        field_464[i].field_E = 0;

        field_464[i].field_75_count = 0;
        field_464[i].field_78 = 0;
        field_464[i].field_7A_wanted_timer = 0;

        memset(field_464[i].field_20, 0, 0x18);
    }
    field_654_wanted_level = 0;
    field_658_count = 0;
    field_659 = 1;
    field_65C = 3;
    byte_6FEE44 = 0;
    if (bStartNetworkGame_7081F0)
    {
        field_660_wanted_star_count = 1;
    }
    else
    {
        field_660_wanted_star_count = 6;
    }
    field_7AC = 100;
    field_7AD_police_peds_in_range_screen = 0;
    field_7B0 = 0;
    field_7B4 = 0;
}

MATCH_FUNC(0x56f4d0)
bool Police_7B8::sub_56F4D0(Ped* a2)
{
    for (u8 v10 = 0; v10 < 20; v10++)
    {
        PoliceCrew_38* v3 = &this->field_4_cop_crew[v10];
        if (v3->field_1C_used)
        {
            if (v3->field_10_subObj->field_4_ped == a2)
            {
                char_type v6 = v3->field_10_subObj->sub_5CBC90();
                Kfc_30* v7 = v3->field_10_subObj;
                if (v3->field_10_subObj->field_8_group)
                {
                    v7->field_4_ped = v7->field_8_group->field_2C_ped_leader;
                }
                if (v6 != 0)
                {
                    return true;
                }
                else
                {
                    v3->field_10_subObj->field_4_ped = NULL;
                    return false;
                }
            }
            else
            {
                if (a2->field_164_ped_group)
                {
                    a2->field_164_ped_group->sub_4C9970(a2);
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
            pCop->field_238 = 4;
            pCop->field_240_occupation = ped_ocupation_enum::unknown_17;
            pCop->SetObjective(objectives_enum::guard_spot_24, 0);
            pCop->field_244_remap = 8;
            pCop->field_26C_graphic_type = 1;
            pCop->ForceWeapon_46F600(weapon_type::silence_smg);
            pCop->field_216_health = 200;
            pCop->field_288_threat_search = threat_search_enum::area_2;
            pCop->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
            break;
        case 1:
            pCop = gPedManager_6787BC->SpawnPedAt(xpos, ypos, zpos, 0, rotation);
            pCop->field_238 = 4;
            pCop->field_240_occupation = ped_ocupation_enum::unknown_17;
            pCop->SetObjective(objectives_enum::guard_spot_24, 0);
            pCop->field_244_remap = 0;
            pCop->field_26C_graphic_type = 2;
            pCop->field_170_selected_weapon = 0;
            pCop->GiveWeapon_46F650(weapon_type::pistol);
            pCop->field_216_health = 200;
            pCop->field_288_threat_search = threat_search_enum::area_2;
            pCop->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
            break;
    }
    return pCop;
}

MATCH_FUNC(0x56f6d0)
void Police_7B8::sub_56F6D0(Car_BC* pCar)
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
                    switch (pCrew->field_14_pObj->field_4)
                    {
                        case 6:

                            if (pCrew->field_20 == 1)
                            {
                                bUnknown = 1;
                            }
                            if (pCrew->field_20 == 2)
                            {
                                bUnknown = 1;
                            }
                            if (pCrew->field_20 == 3)
                            {
                                bUnknown = 1;
                            }

                            break;

                        case 5:

                            if (pCrew->field_20 == 1)
                            {
                                bUnknown = 1;
                            }
                            if (pCrew->field_20 == 2)
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

                pCrew->sub_570AB0();
                Car_BC* pCrewCar = pCrew->field_10_subObj->field_0_car;

                if (pCrewCar->field_88 != 5 && pCrewCar->field_88 != 2 && pCrewCar->field_88 != 3)
                {
                    pCrewCar->field_88 = 4;
                }
                pCrew->field_10_subObj->field_28 = 5;
                pCrew->field_10_subObj->field_2C = 1;
                pCrew->field_24_state = 6;
                return;
            }
        }
    }
}

MATCH_FUNC(0x56f800)
bool Police_7B8::HasCriminalBeenFound_56F800(Ped* a2)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464[i].field_0_criminal_ped == a2)
        {
            if (field_464[i].field_75_count > 0 && (field_464[i].field_8 == 3 || field_464[i].field_C != 0))
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
bool Police_7B8::sub_56F880(Ped* a2)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464[i].field_0_criminal_ped == a2)
        {
            if (field_464[i].field_78)
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

MATCH_FUNC(0x56f8e0)
void Police_7B8::SetArrestedPed_56F8E0(Ped* a2, Ped* a3)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464[i].field_0_criminal_ped == a2)
        {
            field_464[i].field_0_criminal_ped = a2;
            return;
        }
    }
}

MATCH_FUNC(0x56f940)
void Police_7B8::sub_56F940(Ped* pPed)
{
    byte_6FEE44 = 0;
    if (pPed->field_15C_player)
    {
        bool bFound = false;
        for (u8 idx = 0; idx < GTA2_COUNTOF(field_464); idx++)
        {
            if (field_464[idx].field_0_criminal_ped == pPed)
            {
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            for (u8 i = 0; i < GTA2_COUNTOF(field_464); i++)
            {
                if (field_464[i].field_0_criminal_ped == NULL)
                {
                    field_464[i].field_0_criminal_ped = pPed;
                    field_464[i].field_8 = 0;
                    field_464[i].field_10_x = pPed->get_cam_x();
                    field_464[i].field_14_y = pPed->get_cam_y();
                    field_464[i].field_18_z = pPed->get_cam_z();
                    break;
                }
            }
        }

        for (u8 j = 0; j < GTA2_COUNTOF(field_464); j++)
        {
            if (field_464[j].field_0_criminal_ped != NULL)
            {
                byte_6FEE44 = 1;
                return;
            }
        }
    }
}

MATCH_FUNC(0x56fa40)
void Police_7B8::sub_56FA40()
{
    if (field_464[0].field_0_criminal_ped)
    {
        if ((field_464[0].field_0_criminal_ped->field_21C & 1) == 0 || field_464[0].field_0_criminal_ped->field_278 == 9)
        {
            field_464[0].field_8 = 4;
        }
        else
        {
            if (field_464[0].field_C > 0)
            {
                field_464[0].field_C--;
            }

            if (field_464[0].field_8 == 3 && field_464[0].field_C == 0)
            {
                field_464[0].field_8 = 5;
            }
        }
    }
}

MATCH_FUNC(0x56faa0)
char_type Police_7B8::sub_56FAA0(Police_7C* p7C)
{
    u8 xval = p7C->field_10_x.ToInt();
    u8 yval = p7C->field_14_y.ToInt();
    u8 zval = p7C->field_18_z.ToInt();

    if (gOrca_2FD4_6FDEF0->sub_5552B0(1, &xval, &yval, &zval, 0))
    {
        PoliceCrew_38* pNewPoliceCrew = Police_7B8::New_56F560();
        pNewPoliceCrew->field_1C_used = 1;
        pNewPoliceCrew->field_2_targ_x = xval;
        pNewPoliceCrew->field_3_targ_y = yval;
        pNewPoliceCrew->field_4_targ_z = zval;
        Kfc_30* pNewKfc = gKfc_1E0_706280->New_5CBB80();
        pNewPoliceCrew->field_10_subObj = pNewKfc;
        if (!pNewPoliceCrew->field_10_subObj)
        {
            pNewPoliceCrew->Init_5709C0();
            return 0;
        }
        pNewPoliceCrew->field_0_id = id_counter_6FEE46++; // TODO: types
        Kfc_30* pKfc = pNewPoliceCrew->field_10_subObj;
        pNewPoliceCrew->field_14_pObj = p7C;
        pNewPoliceCrew->field_24_state = dword_6FEDCC;
        pNewPoliceCrew->field_20 = gRoadblockGuardType_6FEDB8;
        pKfc->field_1E_is_used = 1;
        pKfc->field_20_maybe_type = dword_6FEE18; // field_20_maybe_type
        pKfc->field_24 = 1;
        pKfc->field_28 = 3;
        pKfc->field_18 = word_6FEAC8;
        pKfc->field_C_x = Fix16(xval);
        pKfc->field_10_y = Fix16(yval);
        pKfc->field_14_z = Fix16(zval);
        pNewPoliceCrew->sub_570A10();
        return 1;
    }
    return 0;
}

STUB_FUNC(0x56fbd0)
void Police_7B8::sub_56FBD0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x570270)
void Police_7B8::Service_570270()
{
    field_7B4 = 0;
    field_654_wanted_level = 0;

    if (byte_6FEE44 == 1)
    {
        Police_7B8::sub_56FBD0();
    }

    for (s32 i = 0; i < GTA2_COUNTOF(field_4_cop_crew); i++)
    {
        if (field_4_cop_crew[i].field_1C_used == 1)
        {
            field_4_cop_crew[i].Service_575590();
        }
    }

    if (byte_6FEE44 == 1)
    {
        Police_7B8::sub_56FA40();
    }

    field_664_obj.sub_5757B0();
    field_708_obj.sub_5757B0();

    if (field_7AC > 0)
    {
        field_7AC--;
    }

    if (field_7B0 != NULL)
    {
        if (field_7B0->get_ped_state1() == 9)
        {
            field_7B0 = NULL;
        }
        else if (!field_7B0->check_bit_0())
        {
            field_7B0 = NULL;
        }
        else if (field_7B0->field_21C_bf.b11 == 0)
        {
            field_7B0 = NULL;
        }
    }
}

MATCH_FUNC(0x570320)
void Police_7B8::SpawnWalkingGuard_570320(Ped* pPed, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    if (field_65C == 6)
    {
        pPed->set_occupation_403970(ped_ocupation_enum::unknown_16);
        pPed->SetField238_403920(3);
        pPed->set_remap_433B90(4);
    }
    else
    {
        pPed->set_occupation_403970(ped_ocupation_enum::unknown_14);
        pPed->SetField238_403920(3);
        pPed->set_remap_433B90(0);
    }
    pPed->field_26C_graphic_type = 2;
    pPed->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pPed->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    pPed->sub_45C830(xpos, ypos, zpos);

    Char_B4* pCharObj = pPed->field_168_game_object;
    u8 remap = pPed->field_244_remap;
    pCharObj->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pCharObj->field_80_sprite_ptr->SetRemap(remap);
    }
    pPed->field_168_game_object->field_40_rotation = rotation;
    pPed->sub_467280();
}

MATCH_FUNC(0x5703e0)
bool Police_7B8::FBI_Army_5703E0(Car_BC* pCar)
{
    if (gPedManager_6787BC->field_5_fbi_army_count >= 30)
    {
        return false;
    }
    if (field_658_count > 2)
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
    pNewCrew->field_24_state = 1;
    pNewCrew->field_29 = 1;
    pKfc->field_1E_is_used = 1;
    pKfc->field_20_maybe_type = gPolice_7B8_6FEE40->field_65C;
    pKfc->field_24 = 1;
    pKfc->field_0_car = pCar;
    PedGroup* pNewPedGroup = PedGroup::New_4CB0D0();
    Ped* pNewPed1 = gPedManager_6787BC->sub_470F30();
    pNewPed1->SetField238_403920(4);
    pNewPed1->set_occupation_403970(ped_ocupation_enum::police);
    pNewPed1->SpawnPedInCar_45C730(pKfc->field_0_car);
    pNewPed1->SetObjective(objectives_enum::objective_43, 9999);
    pNewPed1->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pNewPed1->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
    Ped* pNewPed2 = gPedManager_6787BC->sub_470F30();
    pNewPed2->SetObjective(objectives_enum::no_obj_0, 9999);
    pNewPed2->sub_45C7F0(pKfc->field_0_car);
    pNewPed2->SetField238_403920(4);
    pNewPed2->set_occupation_403970(ped_ocupation_enum::police);
    pNewPed2->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pNewPed2->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;

    switch (gPolice_7B8_6FEE40->field_65C)
    {
        case 4:
            // ok
            pNewPed1->set_health_4039A0(250);
            pNewPed1->ForceWeapon_46F600(weapon_type::shotgun);
            pNewPed1->GiveWeapon_46F650(weapon_type::silence_smg);
            pNewPed1->set_occupation_403970(ped_ocupation_enum::fbi);
            pNewPed1->field_244_remap = 8;
            pNewPed1->field_26C_graphic_type = 1;
            pNewPed2->set_health_4039A0(250);
            pNewPed2->field_244_remap = 8;
            pNewPed2->ForceWeapon_46F600(weapon_type::silence_smg);
            pNewPed2->field_26C_graphic_type = 1;
            pNewPed2->set_occupation_403970(ped_ocupation_enum::fbi);
            pNewCrew->field_20 = 3;
            break;

        case 3:

            switch (field_654_wanted_level)
            {
                case 0:
                case 1:
                    pNewPed1->field_170_selected_weapon = 0;
                    pNewPed1->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed1->set_health_4039A0(50);
                    pNewPed1->field_1F0_maybe_max_speed = dword_6FEB0C * dword_6FEB68;
                    pNewPed2->field_170_selected_weapon = 0;
                    pNewPed2->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed2->set_health_4039A0(50);
                    pNewPed2->field_1F0_maybe_max_speed = dword_6FEB0C * dword_6FEB68;

                    break;

                case 2:
                    // line 231
                    pNewPed1->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed1->set_health_4039A0(100);
                    pNewPed1->field_1F0_maybe_max_speed = dword_6FEB0C * dword_6FEB68;
                    pNewPed2->GiveWeapon_46F650(weapon_type::pistol);
                    pNewPed2->set_health_4039A0(100);

                    pNewPed2->field_1F0_maybe_max_speed = dword_6FEB0C * dword_6FEB68;

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
            pNewCrew->field_20 = 1;

            break;

        default:
            pNewPed1->set_health_4039A0(250);
            pNewPed1->ForceWeapon_46F600(weapon_type::smg);
            pNewPed1->field_244_remap = 4;
            pNewPed1->set_occupation_403970(ped_ocupation_enum::army_army);
            pNewPed1->field_26C_graphic_type = 2;
            pNewPed2->set_health_4039A0(250);
            pNewPed2->ForceWeapon_46F600(weapon_type::smg);
            pNewPed2->field_244_remap = 4;
            pNewPed2->set_occupation_403970(ped_ocupation_enum::army_army);
            pNewPed2->field_26C_graphic_type = 2;
            pNewCrew->field_20 = 4;
            break;
    }

    pNewPedGroup->add_ped_leader_4C9B10(pNewPed1);
    pNewPedGroup->field_36_count = 1;
    pNewPedGroup->field_34_count = 1;
    pNewPedGroup->add_ped_to_list_4C9B30(pNewPed2, 0);
    pNewPedGroup->field_0 = 0;
    pKfc->field_4_ped = pNewPed1;
    pKfc->field_18 = 0;
    pKfc->field_28 = 6;
    pKfc->field_0_car->sub_421560(5);
    pKfc->field_0_car->InitCarAIControl_440590();
    pKfc->field_0_car->sub_43AF40();
    ++field_658_count;
    return true;
}

MATCH_FUNC(0x570790)
bool Police_7B8::sub_570790(PoliceCrew_38* a1, Police_7C* a2)
{
    a1->field_14_pObj = a2;
    a1->field_24_state = 5;
    a1->sub_570A10();
    return true;
}

STUB_FUNC(0x5707b0)
char_type Police_7B8::sub_5707B0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5708c0)
void Police_7B8::sub_5708C0(Ped* pPed)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464[i].field_0_criminal_ped == pPed)
        {
            field_464[i].field_10_x = pPed->get_cam_x();
            field_464[i].field_14_y = pPed->get_cam_y();
            field_464[i].field_18_z = pPed->get_cam_z();
            field_464[i].field_C = 250;
            return;
        }
    }
}

MATCH_FUNC(0x570940)
void Police_7B8::UpdateCriminalLatestPosition_570940(Ped* pPed)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464[i].field_0_criminal_ped == pPed)
        {
            field_464[i].field_10_x = pPed->get_cam_x();
            field_464[i].field_14_y = pPed->get_cam_y();
            field_464[i].field_18_z = pPed->get_cam_z();
            return;
        }
    }
}

MATCH_FUNC(0x577320)
char_type Police_7B8::sub_577320()
{
    if (this->field_654_wanted_level < 3 || this->field_664_obj.field_0 || this->field_7AC)
    {
        return 0;
    }
    this->field_7AC = 40;
    return 1;
}

STUB_FUNC(0x577370)
void Police_7B8::sub_577370(u8 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
}