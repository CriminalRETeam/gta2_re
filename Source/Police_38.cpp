#include "Police_38.hpp"
#include "Globals.hpp"
#include "Kfc_1E0.hpp"
#include "Ped.hpp"
#include "Object_5C.hpp"
#include "Car_BC.hpp"
#include "Game_0x40.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"

DEFINE_GLOBAL(s32, dword_6FECE8, 0x6FECE8);
DEFINE_GLOBAL(Fix16, dword_6FED54, 0x6FED54);
DEFINE_GLOBAL(Ped*, pPed_6FEDDC, 0x6FEDDC);

DEFINE_GLOBAL_INIT(Fix16, dword_6FEB68, Fix16(13107, 0), 0x6FEB68);
DEFINE_GLOBAL_INIT(Fix16, dword_6FECA0, Fix16(256, 0), 0x6FECA0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEB88, dword_6FECA0, 0x6FEB88);
DEFINE_GLOBAL_INIT(Fix16, dword_6FECF8, Fix16(4), 0x6FECF8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FEB0C, dword_6FECF8 * dword_6FEB88, 0x6FEB0C);

MATCH_FUNC(0x4beb30)
Police_38::Police_38()
{
    sub_5709C0();
}

MATCH_FUNC(0x4beb40)
Police_38::~Police_38()
{
}

MATCH_FUNC(0x570790)
bool Police_38::sub_570790(Police_38* a1, Police_7C* a2)
{
    a1->field_14_pObj = a2;
    a1->field_24_state = 5;
    a1->sub_570A10();
    return true;
}

MATCH_FUNC(0x5709c0)
void Police_38::sub_5709C0()
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
void Police_38::sub_570A10()
{
    if (field_14_pObj->field_75_count < 6)
    {
        for (u8 v2 = 0; v2 < 6; v2++)
        {
            if (field_14_pObj->field_20[v2] == this)
            {
                return;
            }
        }
        field_14_pObj->field_20[field_14_pObj->field_75_count] = this;
        ++field_14_pObj->field_75_count;
        switch (field_10_subObj->field_20)
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
void Police_38::sub_570AB0()
{
    if (!field_1C_used || (field_24_state != 0 && field_24_state != 1 && field_24_state != 6))
    {
        u8 last_idx = field_14_pObj->field_75_count - 1;
        Police_38* pPolice38_last = field_14_pObj->field_20[last_idx];
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
void Police_38::sub_570BF0()
{
    PedGroup* pGroup = PedGroup::sub_4CB0D0();
    Ped* pCopLeader = gChar_C_6787BC->sub_470F30();
    pCopLeader->field_238 = 4;
    pCopLeader->field_240_occupation = ped_ocupation_enum::police;
    pCopLeader->sub_45C730(field_10_subObj->field_0);
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
            pCopLeader->field_1F0 = dword_6FEB0C * dword_6FEB68;
            break;
        case 2:
            pCopLeader->GiveWeapon_46F650(weapon_type::pistol);
            pCopLeader->set_health_4039A0(100);
            pCopLeader->field_1F0 = dword_6FEB0C * dword_6FEB68;
            break;
        default:
            pCopLeader->GiveWeapon_46F650(weapon_type::pistol);
            pCopLeader->set_health_4039A0(100);
            break;
    }

    pCopLeader->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pCopLeader->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;

    Ped* pCopSupporter = gChar_C_6787BC->sub_470F30();
    pCopSupporter->sub_45C7F0(field_10_subObj->field_0);
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
            pCopSupporter->field_1F0 = dword_6FEB0C * dword_6FEB68;
            break;
        case 2:
            pCopSupporter->GiveWeapon_46F650(weapon_type::pistol);
            pCopSupporter->field_216_health = 100;
            pCopSupporter->field_1F0 = dword_6FEB0C * dword_6FEB68;
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
    field_10_subObj->field_4 = pCopLeader;
    field_10_subObj->field_28 = 6;
    field_10_subObj->field_0->sub_421560(5);
    field_10_subObj->field_0->sub_440590();
    field_10_subObj->field_0->sub_43AF40();
    field_10_subObj->field_0->sub_43C920();
    field_10_subObj->field_8 = pGroup;
}

MATCH_FUNC(0x570e30)
void Police_38::sub_570E30()
{
    PedGroup* pSwatGroup = PedGroup::sub_4CB0D0();
    Ped* pSwatLeader = gChar_C_6787BC->sub_470F30();
    pSwatLeader->field_238 = 4;
    pSwatLeader->field_240_occupation = ped_ocupation_enum::swat;
    pSwatLeader->sub_45C730(field_10_subObj->field_0);
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
        Ped* pSwatMember = gChar_C_6787BC->sub_470F30();
        pSwatMember->sub_45C7F0(field_10_subObj->field_0);
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
    field_10_subObj->field_4 = pSwatLeader;
    field_10_subObj->field_28 = 6;
    field_10_subObj->field_0->sub_421560(5);
    field_10_subObj->field_0->sub_440590();
    field_10_subObj->field_0->sub_43AF40();
    field_10_subObj->field_0->sub_43C920();
    field_10_subObj->field_8 = pSwatGroup;
}

STUB_FUNC(0x571150)
s32 Police_38::sub_571150()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x571350)
u32* Police_38::sub_571350()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x571540)
char_type Police_38::sub_571540()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x571a30)
char_type Police_38::sub_571A30()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5720c0)
char_type Police_38::sub_5720C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x572210)
bool Police_38::sub_572210()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x572340)
char_type Police_38::sub_572340()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x572920)
void Police_38::sub_572920()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x574720)
void Police_38::sub_574720()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x574f10)
char_type Police_38::sub_574F10()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x575200)
void Police_38::sub_575200()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x575210)
void Police_38::sub_575210()
{
    NOT_IMPLEMENTED;
    Kfc_30* pKfc30 = this->field_10_subObj;
    if (!pKfc30->field_24 || pPed_6FEDDC->field_225)
    {
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->SetObjective(0, 9999);
    }
    else
    {
        pKfc30->field_0->field_A6 |= 0x20u;
    }
}

STUB_FUNC(0x575270)
void Police_38::sub_575270()
{
    NOT_IMPLEMENTED;
    if (!this->field_10_subObj->field_24 || pPed_6FEDDC->field_225)
    {
        pPed_6FEDDC->sub_463830(0, 9999);
        pPed_6FEDDC->SetObjective(0, 9999);
    }
}

STUB_FUNC(0x5752c0)
void Police_38::sub_5752C0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x575310)
void Police_38::sub_575310()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x575590)
void Police_38::sub_575590()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x575650)
s32 Police_38::sub_575650()
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
                if (field_28_barrier_1->field_14 == field_58)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_28_barrier_1->field_4->field_14_xpos.x, field_28_barrier_1->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_2C_barrier_2)
            {
                if (field_2C_barrier_2->field_14 == field_5C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_2C_barrier_2->field_4->field_14_xpos.x, field_2C_barrier_2->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_30_barrier_3)
            {
                if (field_30_barrier_3->field_14 == field_60)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_30_barrier_3->field_4->field_14_xpos.x, field_30_barrier_3->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_34_barrier_4)
            {
                if (field_34_barrier_4->field_14 == field_64)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_34_barrier_4->field_4->field_14_xpos.x, field_34_barrier_4->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_38_barrier_5)
            {
                if (field_38_barrier_5->field_14 == field_68)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_38_barrier_5->field_4->field_14_xpos.x, field_38_barrier_5->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_3C_barrier_6)
            {
                if (field_3C_barrier_6->field_14 == field_6C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_3C_barrier_6->field_4->field_14_xpos.x, field_3C_barrier_6->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_40_barrier_7)
            {
                if (field_40_barrier_7->field_14 == field_70)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_40_barrier_7->field_4->field_14_xpos.x, field_40_barrier_7->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_44_barrier_8)
            {
                if (field_44_barrier_8->field_14 == field_74)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_44_barrier_8->field_4->field_14_xpos.x, field_44_barrier_8->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_48_barrier_9)
            {
                if (field_48_barrier_9->field_14 == field_78)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_48_barrier_9->field_4->field_14_xpos.x, field_48_barrier_9->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_4C_barrier_10)
            {
                if (field_4C_barrier_10->field_14 == field_7C)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_4C_barrier_10->field_4->field_14_xpos.x, field_4C_barrier_10->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_50_barrier_11)
            {
                if (field_50_barrier_11->field_14 == field_80)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_50_barrier_11->field_4->field_14_xpos.x, field_50_barrier_11->field_4->field_14_xpos.y))
                    {
                        v31 = 0;
                    }
                }
            }

            if (field_54_barrier_12)
            {
                if (field_54_barrier_12->field_14 == field_84)
                {
                    if (gGame_0x40_67E008->is_point_on_screen_4B9A80(field_54_barrier_12->field_4->field_14_xpos.x, field_54_barrier_12->field_4->field_14_xpos.y))
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

                v30.inline_abs_403840(v30);
                v29.inline_abs_403840(v29);

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
        if (field_28_barrier_1->field_14 == field_58)
        {
            field_28_barrier_1->sub_5291B0();
        }
        field_28_barrier_1 = 0;
    }

    if (field_2C_barrier_2)
    {
        if (field_2C_barrier_2->field_14 == field_5C)
        {
            field_2C_barrier_2->sub_5291B0();
        }
        field_2C_barrier_2 = 0;
    }

    if (field_30_barrier_3)
    {
        if (field_30_barrier_3->field_14 == field_60)
        {
            field_30_barrier_3->sub_5291B0();
        }
        field_30_barrier_3 = 0;
    }

    if (field_34_barrier_4)
    {
        if (field_34_barrier_4->field_14 == field_64)
        {
            field_34_barrier_4->sub_5291B0();
        }
        field_34_barrier_4 = 0;
    }

    if (field_38_barrier_5)
    {
        if (field_38_barrier_5->field_14 == field_68)
        {
            field_38_barrier_5->sub_5291B0();
        }
        field_38_barrier_5 = 0;
    }

    if (field_3C_barrier_6)
    {
        if (field_3C_barrier_6->field_14 == field_6C)
        {
            field_3C_barrier_6->sub_5291B0();
        }
        field_3C_barrier_6 = 0;
    }

    if (field_40_barrier_7)
    {
        if (field_40_barrier_7->field_14 == field_70)
        {
            field_40_barrier_7->sub_5291B0();
        }
        field_40_barrier_7 = 0;
    }

    if (field_44_barrier_8)
    {
        if (field_44_barrier_8->field_14 == field_74)
        {
            field_44_barrier_8->sub_5291B0();
        }
        field_44_barrier_8 = 0;
    }

    if (field_48_barrier_9)
    {
        if (field_48_barrier_9->field_14 == field_78)
        {
            field_48_barrier_9->sub_5291B0();
        }
        field_48_barrier_9 = 0;
    }

    if (field_4C_barrier_10)
    {
        if (field_4C_barrier_10->field_14 == field_7C)
        {
            field_4C_barrier_10->sub_5291B0();
        }
        field_4C_barrier_10 = 0;
    }

    if (field_50_barrier_11)
    {
        if (field_50_barrier_11->field_14 == field_80)
        {
            field_50_barrier_11->sub_5291B0();
        }
        field_50_barrier_11 = 0;
    }

    if (field_54_barrier_12)
    {
        if (field_54_barrier_12->field_14 == field_84)
        {
            field_54_barrier_12->sub_5291B0();
        }
        field_54_barrier_12 = 0;
    }

    if (field_88_guard_1)
    {
        if (field_88_guard_1->field_20e)
        {
            field_88_guard_1->sub_45EB60();
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
            field_8C_guard_2->sub_45EB60();
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
            field_90_guard_3->sub_45EB60();
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
            field_94_guard_4->sub_45EB60();
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
            field_98_guard_5->sub_45EB60();
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
            field_9C_guard_6->sub_45EB60();
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
    field_A0_pMem = new u8[0x18u]; // TODO: A class/struct?
}

MATCH_FUNC(0x5774a0)
PoliceRoadblock_A4::~PoliceRoadblock_A4()
{
    delete[] field_A0_pMem;
}