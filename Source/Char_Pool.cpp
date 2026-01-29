#include "Char_Pool.hpp"
#include "Car_BC.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Hud.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Ped_List_4.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "Weapon_30.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "rng.hpp"

#include "Frontend.hpp" // For the cheat bools

#include <DINPUT.H>

DEFINE_GLOBAL(PedManager*, gPedManager_6787BC, 0x6787BC);
DEFINE_GLOBAL(PedPool*, gPedPool_6787B8, 0x6787B8);
DEFINE_GLOBAL(Char_B4_Pool*, gChar_B4_Pool_6FDB44, 0x6FDB44);
DEFINE_GLOBAL(Char_8_Pool*, gChar_8_Pool_678b50, 0x678b50);

DEFINE_GLOBAL(u16, word_6787F0, 0x6787F0);
DEFINE_GLOBAL(u8, byte_61A8A1, 0x61A8A1);
DEFINE_GLOBAL(u8, byte_61A8A2, 0x61A8A2);
DEFINE_GLOBAL(u8, unk_6787EE, 0x6787EE);
DEFINE_GLOBAL(u8, unk_6787EF, 0x6787EF);
DEFINE_GLOBAL(u8, byte_6787DA, 0x6787DA);

DEFINE_GLOBAL(u8, spawnSideLocked_6787D5, 0x6787D5);
DEFINE_GLOBAL(u8, spawnCountLimit_6787D6, 0x6787D6);
DEFINE_GLOBAL(Ang16, cameraFacingAng_678760, 0x678760);
DEFINE_GLOBAL(u8, gSpawnSide_6787C8, 0x6787C8);
DEFINE_GLOBAL(u8, gSpawnIndex_6787C9, 0x6787C9);

DEFINE_GLOBAL(Fix16, gDummyH_678584, 0x678584);
DEFINE_GLOBAL(Fix16, dword_678414, 0x678414);

EXTERN_GLOBAL(u16, word_6787E0);
EXTERN_GLOBAL(u8, byte_6787E2);
EXTERN_GLOBAL(u8, byte_6787E4);
EXTERN_GLOBAL(u8, byte_6787E3);
EXTERN_GLOBAL(u8, byte_6787D8);
EXTERN_GLOBAL(u8, byte_6787D9);
EXTERN_GLOBAL(u8, byte_6787D2);

EXTERN_GLOBAL(Fix16, k_dword_678438);
EXTERN_GLOBAL(Fix16, k_dword_67853C);
EXTERN_GLOBAL(Ang16, gDummyPedAng_6787A8);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

EXTERN_GLOBAL(Fix16, gDummyW_678530);
EXTERN_GLOBAL(Fix16, gDummyZ_67841C);

EXTERN_GLOBAL(Fix16, gSpawnJitterScale_678618);
EXTERN_GLOBAL(Fix16, k_dword_678664);

EXTERN_GLOBAL(Fix16, dword_6784A0);
EXTERN_GLOBAL(Fix16, dword_678480);

DEFINE_GLOBAL(Ang16, gSpawnRotationLeft_6786E0, 0x6786E0);
DEFINE_GLOBAL(Ang16, gSpawnRotationTop_6787B0, 0x6787B0);
DEFINE_GLOBAL(Ang16, gSpawnRotationRight_678578, 0x678578);
DEFINE_GLOBAL(Ang16, gSpawnRotationBottom_678540, 0x678540);
DEFINE_GLOBAL(s16, gSpawnCounter_6787C6, 0x6787C6);
EXTERN_GLOBAL(char_type, byte_6787CE);

EXPORT Ped* __stdcall SpawnPedChainGroupAt_46DB90(char_type remap, u8 number_followers, Fix16 xpos, Fix16 ypos, Fix16 zpos);

// TODO: Prob a method of PedManager?
WIP_FUNC(0x46E380)
EXPORT void __stdcall SpawnPedestrianAt_46E380(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    //WIP_IMPLEMENTED;

    char_type rng_kind; // bl
    s16 rng_val; // di
    Ped* pPed; // esi
    gmp_zone_info* pZone; // eax
    s32 mugger_ratio; // ecx
    s32 v11; // edx
    s32 field_E_carthief_ratio; // edi
    gmp_map_zone* v14; // eax
    Gang_144* pGang; // edi
    char_type v16; // al
    Gang_144* field_17C_pZone; // ecx
    s32 GangCurrWeapon_4BF0C0; // eax
    Weapon_30* field_170_selected_weapon; // eax
    s32 occupation_; // eax
    char_type v21; // al
    s32 int_4F7AE0; // eax
    s32 v23; // eax
    s32 wanted_level_; // eax
    u8 v25; // al
    char_type v26; // al
    Char_B4* game_object; // ecx
    u8 remap; // al
    char_type kind; // [esp+10h] [ebp-18h]
    s32 rng_max; // [esp+14h] [ebp-14h] BYREF
    s32 v31; // [esp+18h] [ebp-10h] BYREF
    s32 field_12_gangchar_ratio; // [esp+1Ch] [ebp-Ch] BYREF
    s32 y_int; // [esp+20h] [ebp-8h] BYREF
    s32 x_int; // [esp+24h] [ebp-4h] BYREF

    rng_kind = 0;
    kind = 0;
    rng_max = 1000;
    rng_val = stru_6F6784.get_int_4F7AE0((s16*)&rng_max);

    /*
    gPoolRoot = gPed_Pool_6787B8;
    pPed = gPed_Pool_6787B8->field_0_pStart;
    field_4_pPrev = gPed_Pool_6787B8->field_4_pPrev;
    gPed_Pool_6787B8->field_0_pStart = gPed_Pool_6787B8->field_0_pStart->mpNext;
    pPed->mpNext = field_4_pPrev;
    gPoolRoot->field_4_pPrev = pPed;
    Ped::PoolAllocate(pPed);
    */
    pPed = gPedPool_6787B8->Allocate();

    ++gSpawnCounter_6787C6;

    x_int = xpos.ToInt();
    y_int = ypos.ToInt();

    pZone = gMap_0x370_6F6268->get_nav_zone_unknown_4DF890(xpos.ToInt(), ypos.ToInt());
    mugger_ratio = (u16)pZone->field_C_mugger_ratio;

    v11 = rng_val;

    if (rng_val < (s32)(u16)mugger_ratio)
    {
        rng_kind = 1;
    }
    else
    {
        field_E_carthief_ratio = (u16)pZone->field_E_carthief_ratio;
        v31 = (u16)field_E_carthief_ratio;
        if (v11 >= (u16)field_E_carthief_ratio + mugger_ratio)
        {
            rng_max = (u16)pZone->field_10_elvis_ratio;
            if (v11 < mugger_ratio + field_E_carthief_ratio + rng_max)
            {
                rng_max = 50;
                rng_kind = stru_6F6784.get_int_4F7AE0((s16*)&rng_max) != 25 ? 0 : 3;
                kind = rng_kind;
                goto LABEL_12;
            }
            field_12_gangchar_ratio = (u16)pZone->field_12_gangchar_ratio;
            if (v11 < mugger_ratio + v31 + field_12_gangchar_ratio + rng_max)
            {
                rng_kind = (u8)byte_6787CE < 8u ? 4 : 0;
            }
            else
            {
                if (v11 >= mugger_ratio + v31 + rng_max + field_12_gangchar_ratio + (u16)pZone->field_14_policeped_ratio)
                {
                    goto LABEL_12;
                }
                rng_kind = 5;
            }
        }
        else
        {
            rng_kind = 2;
        }
    }

    kind = rng_kind;

LABEL_12:

    if (gCheatOnlyElvisPeds_67D4ED)
    {
        rng_kind = 3;
        kind = 3;
    }

    if (bNo_annoying_chars_67D586)
    {
        if (rng_kind != 5)
        {
            kind = 0;
        }
    }

    if (pPed)
    {
        pPed->ChangeNextPedState1_45C500(0);
        pPed->ChangeNextPedState2_45C540(0);
        pPed->field_238 = 3;
        pPed->field_20e = 1;
        pPed->field_244_remap = 3;

        switch (kind)
        {
            case 1:
                if (gNumberMuggersSpawned_6787CA)
                {
                    goto delloc_ret_2;
                }
                gNumberMuggersSpawned_6787CA = 1;
                pPed->field_240_occupation = ped_ocupation_enum::mugger;
                pPed->field_22C = 2;
                pPed->field_218_objective_timer = 40;
                pPed->field_238 = 4;
                pPed->field_244_remap = 17;
                pPed->field_288_threat_search = threat_search_enum::area_2;
                pPed->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                pPed->field_26C_graphic_type = 0;
                goto LABEL_34;

            case 2:
                if (gNumberCarThiefsSpawned_6787CB)
                {
                    goto delloc_ret_2;
                }
                gNumberCarThiefsSpawned_6787CB = 1;
                pPed->field_22C = 2;
                pPed->field_288_threat_search = threat_search_enum::area_2;
                //v13 = pPed->field_21C;
                pPed->field_218_objective_timer = 40;
                //LOBYTE(v13) = v13 | 8;
                pPed->field_240_occupation = ped_ocupation_enum::car_thief;
                pPed->field_238 = 6;
                pPed->field_244_remap = 15;
                pPed->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                pPed->field_26C_graphic_type = 0;
                //pPed->field_21C = v13;
                pPed->field_21C |= 8;
                pPed->field_1F8 = dword_678670;
                goto LABEL_34;

            case 3:
                if (gNumberElvisLeadersSpawned_6787CC)
                {
                    goto delloc_ret_2;
                }
                SpawnPedChainGroupAt_46DB90(12, 5u, xpos, ypos, zpos);
                pPed->Deallocate_45EB60();
                ++gNumberElvisLeadersSpawned_6787CC;
                goto LABEL_34;

            case 4:
                v14 = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(x_int, y_int, 14u);
                if (v14)
                {
                    pGang = gGangPool_CA8_67E274->gang_by_name_4BF100(v14->field_6_name);
                    if ((u8)byte_6787CE >= 4u)
                    {
                        pPed->field_19C = pGang;
                        pPed->field_240_occupation = ped_ocupation_enum::dummy;
                        pPed->field_22C = 0;
                        pPed->field_238 = 3;
                        pPed->field_288_threat_search = threat_search_enum::area_2;
                        pPed->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                        v21 = pGang->field_101;
                        pPed->field_26C_graphic_type = 1;
                        pPed->field_244_remap = v21;
                        if (v21 == 5)
                        {
                            y_int = 2;
                            if (!stru_6F6784.get_int_4F7AE0((s16*)&y_int))
                            {
                                pPed->field_244_remap = 6;
                            }
                        }
                    }
                    else
                    {
                        ++byte_6787CE;
                        pPed->field_238 = 4;
                        pPed->field_240_occupation = ped_ocupation_enum::unknown_10;
                        pPed->field_17C_pZone = pGang;
                        v16 = pGang->field_101;
                        pPed->field_244_remap = v16;
                        if (v16 == 5)
                        {
                            x_int = 2;
                            if (!stru_6F6784.get_int_4F7AE0((s16*)&x_int))
                            {
                                pPed->field_244_remap = 6;
                            }
                        }
                        field_17C_pZone = pPed->field_17C_pZone;
                        pPed->field_26C_graphic_type = 1;
                        pPed->field_22C = 1;
                        GangCurrWeapon_4BF0C0 = field_17C_pZone->GetGangCurrWeapon_4BF0C0();
                        pPed->ForceWeapon_46F600(GangCurrWeapon_4BF0C0);
                        field_170_selected_weapon = pPed->field_170_selected_weapon;
                        if (field_170_selected_weapon)
                        {
                            if (field_170_selected_weapon->field_1C_idx)
                            {
                                pPed->field_21C |= 0x10000000u;
                                pGang->field_141 = 1;
                            }
                        }
                        pPed->GiveWeapon_46F650(weapon_type::pistol);
                        pPed->field_270 = 0;
                        pPed->field_288_threat_search = threat_search_enum::line_of_sight_1;
                        pPed->field_28C_threat_reaction = threat_reaction_enum::react_as_normal_2;
                    }
                }
                else
                {
                    pPed->field_240_occupation = ped_ocupation_enum::dummy;
                    pPed->field_22C = 0;
                    pPed->field_238 = 3;
                    pPed->field_288_threat_search = threat_search_enum::area_2;
                    pPed->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                    pPed->field_26C_graphic_type = 0;
                    field_12_gangchar_ratio = 4;
                    int_4F7AE0 = stru_6F6784.get_int_4F7AE0((s16*)&field_12_gangchar_ratio);
                    if ((u16)int_4F7AE0)
                    {
                        v23 = int_4F7AE0 - 1;
                        if (v23)
                        {
                            if (v23 == 1)
                            {
                                pPed->field_244_remap = 20;
                            }
                            else
                            {
                                pPed->field_244_remap = 21;
                            }
                        }
                        else
                        {
                            pPed->field_244_remap = 19;
                        }
                    }
                    else
                    {
                        pPed->field_244_remap = 18;
                    }
                }
                goto LABEL_34;

            case 5:
                if (gNumberWalkingCopsSpawned_6787CD || bSkip_police_67D4F9 || gPolice_7B8_6FEE40->field_65C == 6)
                {
                delloc_ret_2:
                    pPed->Deallocate_45EB60();
                    return;
                }
                gPolice_7B8_6FEE40->SpawnWalkingGuard_570320(pPed, xpos, ypos, zpos, rotation);
                gNumberWalkingCopsSpawned_6787CD = 1;
                pPed->field_288_threat_search = threat_search_enum::line_of_sight_1;
                pPed->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
                wanted_level_ = gPolice_7B8_6FEE40->field_654_wanted_level;
                if (wanted_level_ < 0)
                {
                    goto LABEL_58;
                }

                if (wanted_level_ <= 1)
                {
                    pPed->field_170_selected_weapon = 0;
                    pPed->GiveWeapon_46F650(weapon_type::pistol);
                    pPed->field_216_health = 50;
                    goto LABEL_56;
                }
                else if (wanted_level_ == 2)
                {
                    pPed->GiveWeapon_46F650(weapon_type::pistol);
                    pPed->field_216_health = 100;
                LABEL_56:
                    pPed->field_1F0_maybe_max_speed = (dword_678448 * dword_6784A0);
                    pPed->field_26C_graphic_type = 2;
                    goto LABEL_34;
                }

                {
                LABEL_58:
                    pPed->GiveWeapon_46F650(weapon_type::pistol);
                    pPed->field_216_health = 100;
                    pPed->field_26C_graphic_type = 2;
                }

            LABEL_34:
                occupation_ = pPed->field_240_occupation;
                if (occupation_ != ped_ocupation_enum::unknown_14 && occupation_ != ped_ocupation_enum::unknown_16)
                {
                    pPed->AllocCharB4_45C830(xpos, ypos, zpos);
                }

                gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(pPed->field_168_game_object->field_80_sprite_ptr);

                if (gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(pPed->field_168_game_object->field_80_sprite_ptr, 0))
                {
                    pPed->ChangeNextPedState1_45C500(9);
                    pPed->ChangeNextPedState2_45C540(15);
                    pPed->Deallocate_45EB60();
                    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(pPed->field_168_game_object->field_80_sprite_ptr);
                    return;
                }

                gPurpleDoom_1_679208->AddToRegionBuckets_477B20(pPed->field_168_game_object->field_80_sprite_ptr);
                if (pPed->field_240_occupation != ped_ocupation_enum::unknown_14)
                {
                    game_object = pPed->field_168_game_object;
                    remap = pPed->field_244_remap;
                    game_object->field_5_remap = remap;
                    if (remap != 0xFF)
                    {
                        game_object->field_80_sprite_ptr->SetRemap(remap);
                    }
                    pPed->field_168_game_object->field_40_rotation = rotation;
                    pPed->sub_467280();
                }

                if (gPedManager_6787BC->field_7_make_all_muggers)
                {
                    ypos = 2;
                    if (!stru_6F6784.get_int_4F7AE0((s16*)&ypos))
                    {
                        if (pPed->field_240_occupation == ped_ocupation_enum::unknown_10)
                        {
                            --byte_6787CE;
                        }
                        pPed->field_22C = 2;
                        pPed->field_218_objective_timer = 40;
                        pPed->field_240_occupation = ped_ocupation_enum::unknown_18;
                        pPed->field_238 = 4;
                        pPed->field_288_threat_search = threat_search_enum::area_2;
                        pPed->field_28C_threat_reaction = threat_reaction_enum::react_as_normal_2;
                    }
                }
                break;

            default:
                if (gPolice_7B8_6FEE40->field_65C == 6)
                {
                    gPolice_7B8_6FEE40->SpawnWalkingGuard_570320(pPed, xpos, ypos, zpos, rotation);
                    pPed->field_288_threat_search = threat_search_enum::line_of_sight_1;
                    pPed->field_28C_threat_reaction = threat_reaction_enum::react_as_normal_2;
                    pPed->ForceWeapon_46F600(weapon_type::smg);
                    pPed->field_26C_graphic_type = 2;
                }
                else
                {
                    pPed->field_240_occupation = ped_ocupation_enum::dummy;
                    pPed->field_22C = 0;
                    pPed->field_238 = 3;
                    pPed->field_288_threat_search = threat_search_enum::area_2;
                    pPed->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                    pPed->field_26C_graphic_type = 0;

                    if (gCheatNakedPeds_67D5E8)
                    {
                        pPed->field_244_remap = 26;
                    }
                    else
                    {
                        v31 = 25;
                        v25 = stru_6F6784.get_int_4F7AE0((s16*)&v31);
                        if (v25 < 4u)
                        {
                            v26 = v25 + 18;
                        }
                        else
                        {
                            v26 = v25 + 27;
                        }
                        pPed->field_244_remap = v26;
                    }
                }
                goto LABEL_34;
        }
    }

    if ((u16)gSpawnCounter_6787C6 > 200u)
    {
        gSpawnCounter_6787C6 = 0;
    }
}

WIP_FUNC(0x46eb60)
void PedManager::SpawnDummies_46EB60(Camera_0xBC* pCam)
{
    WIP_IMPLEMENTED;

    // regs
    char_type spawn_side; // dl
    Fix16 xpos; // esi
    Fix16 ypos; // edi
    char_type last_count; // al
    s16 bound_max; // cx
    s32 last_count_; // edi
    Ang16 rot; // ax
    s32 last_count__; // esi
    Fix16 x_rng; // ebx
    gmp_zone_info* pZoneInfo; // eax
    u32 slope_type; // eax

    // stack
    s32 zpos;
    s32 rng_max;

    Ang16 rotation;

    Sprite* pSprite = this->field_8;

    Fix16 left = pCam->field_78_boundaries_non_neg.field_0_left - k_dword_67853C;
    Fix16 right = pCam->field_78_boundaries_non_neg.field_4_right + k_dword_67853C;
    Fix16 top = pCam->field_78_boundaries_non_neg.field_8_top - k_dword_67853C;
    s32 bottom = (pCam->field_78_boundaries_non_neg.field_C_bottom + k_dword_67853C).ToInt(); // >> 14 to int

    s32 tileLeft = left.ToInt();
    s32 tileTop = top.ToInt();

    //s32 tileLeft_ = tileLeft;
    s32 tileRight = right.ToInt();
    //s32 tileTop_ = tileTop;
    s16 bound_max_ = 0;

    if (spawnSideLocked_6787D5)
    {
        switch (Ang16::GetAngleFace_4F78F0(cameraFacingAng_678760))
        {
            case 1:
                spawn_side = 1;
                gSpawnSide_6787C8 = 1;
                break;
            case 2:
                spawn_side = 3;
                gSpawnSide_6787C8 = 3;
                break;
            case 3:
                spawn_side = 2;
                gSpawnSide_6787C8 = 2;
                break;
            case 4:
                spawn_side = 0;
                gSpawnSide_6787C8 = 0;
                break;
            default:
                goto LABEL_7;
        }
    }
    else
    {
    LABEL_7:
        spawn_side = gSpawnSide_6787C8;
    }

    u8 pCam_v = 0;
    if (spawnCountLimit_6787D6)
    {
        xpos = pCam_v; // s32 cast?
        ypos = pCam_v; // s32 cast?
        last_count = gSpawnIndex_6787C9;

        while (1)
        {
            switch (spawn_side)
            {
                case 0:
                case 2:
                    bound_max = bottom - tileTop;
                    //goto LABEL_14;
                    bound_max_ = bound_max;
                    break;
                case 1:
                case 3:
                    bound_max = tileRight - tileLeft;
                    //LABEL_14:
                    bound_max_ = bound_max;
                    break;
                default:
                    break;
            }

            if (last_count > bound_max_)
            {
                last_count = 0;
                gSpawnIndex_6787C9 = 0;
                if (!spawnSideLocked_6787D5)
                {
                    gSpawnSide_6787C8 = ++spawn_side;
                    if (spawn_side > 3)
                    {
                        spawn_side = 0;
                        gSpawnSide_6787C8 = 0;
                    }
                }
            }

            switch (spawn_side)
            {
                case 0:
                    last_count_ = last_count;
                    rot = gSpawnRotationLeft_6786E0;
                    xpos = Fix16((s16)tileLeft); // ToFix16
                    ypos = Fix16(((s16)tileTop + last_count_)); // ToFix16
                    goto LABEL_24;

                case 1:
                    rotation = gSpawnRotationTop_6787B0;
                    ypos = Fix16((s16)tileTop); // ToFix16
                    xpos = Fix16(((s16)tileLeft + last_count)); // ToFix16
                    break;

                case 2:
                    rotation = gSpawnRotationRight_678578;
                    xpos = Fix16((s16)tileRight); // ToFix16
                    ypos = Fix16(((s16)tileTop + last_count)); // ToFix16
                    break;

                case 3:
                    last_count__ = last_count;
                    rot = gSpawnRotationBottom_678540;
                    ypos = Fix16((s16)bottom); // ToFix16
                    xpos = Fix16(((s16)tileLeft + last_count__)); // ToFix16
                LABEL_24:
                    rotation = rot;
                    break;

                default:
                    break;
            }

            rng_max = 32;
            x_rng = gSpawnJitterScale_678618 * (stru_6F6784.get_int_4F7AE0((s16*)&rng_max) + 8);

            rng_max = 32;
            xpos += x_rng;
            ypos += gSpawnJitterScale_678618 * (stru_6F6784.get_int_4F7AE0((s16*)&rng_max) + 8);

            if (xpos > k_dword_678664 && xpos < dword_678414 - k_dword_678664 && ypos > k_dword_678664 &&
                ypos < dword_678414 - k_dword_678664)
            {
                pZoneInfo = gMap_0x370_6F6268->get_nav_zone_unknown_4DF890(xpos.ToInt(), ypos.ToInt());
                if ((u8)field_6_num_peds_on_screen < (u16)pZoneInfo->field_A_ped_density / 25)
                {
                    if (gMap_0x370_6F6268->FindPavementBlockForCoord_4E4BB0(xpos.ToInt(), ypos.ToInt(), zpos))
                    {
                        pSprite->set_xyz_lazy_451950(xpos, ypos, (zpos + 1));
                        pSprite->set_ang_lazy_420690(gDummyPedAng_6787A8);
                        pSprite->AllocInternal_59F950(gDummyW_678530, gDummyH_678584, gDummyZ_67841C);
                        if (!gGame_0x40_67E008->is_point_on_screen_4B9A80(pSprite->field_14_xy.x, pSprite->field_14_xy.y))
                        {
                            gmp_block_info* pBlock =
                                gMap_0x370_6F6268->get_block_4DFE10((u8)(xpos.ToInt()), (u8)(ypos.ToInt()), (u8)zpos + 1);
                            if (!pBlock || (slope_type = pBlock->field_B_slope_type & 0xFC, slope_type < 0xB4) || slope_type > 0xD0)
                            {
                                SpawnPedestrianAt_46E380(xpos, ypos, (zpos + 1) << 14, rotation);
                            }
                        }
                    }
                }
                //LOWORD(tileLeft) = tileLeft_;
            }
            last_count = ++gSpawnIndex_6787C9;
            pCam_v++;
            if ((u8)pCam_v >= (u8)spawnCountLimit_6787D6)
            {
                break;
            }
            spawn_side = gSpawnSide_6787C8;
            //LOWORD(tileTop) = tileTop_;
        }
    }
}

// https://decomp.me/scratch/dQf8H
WIP_FUNC(0x4703f0)
void PedManager::PedsService_4703F0()
{
    WIP_IMPLEMENTED;

    ++word_6787F0;
    word_6787E0 = 0;
    byte_6787E2 = 0;
    byte_6787E4 = 0;
    byte_6787E3 = 0;
    gNumPedsOnScreen_6787EC = 0;
    byte_61A8A1 = 1;
    byte_61A8A2 = 1;
    byte_6787D2 = 0;
    unk_6787EE = 0;

    gPedPool_6787B8->field_0_pool.UpdatePool();

    if (unk_6787EF) // 11d: je 128
    {
        byte_6787DA = 1;
    }
    else
    {
        byte_6787DA = 0;
        gThreateningPedsList_678468.ClearPeds_4712F0();
    }

    byte_6787D8 = byte_61A8A1 == 1;
    byte_6787D9 = byte_61A8A2 == 1;

    if (!bSkip_dummies_67D4EF)
    {
        Dummies_470330();
    }
    field_3 = word_6787E0;
    field_2 = byte_6787E2;
    field_4 = byte_6787E3;
    field_5_fbi_army_count = byte_6787E4;
    field_6_num_peds_on_screen = gNumPedsOnScreen_6787EC;
    if (gPolice_7B8_6FEE40)
    {
        gPolice_7B8_6FEE40->field_7AD_police_peds_in_range_screen = unk_6787EE;
    }
    unk_6787EF = 0;
    if (bDo_iain_test_67D4E9)
    {
        u16 num_peds = *(u32*)&gNumPedsOnScreen_6787EC; //  TODO: fix me
        swprintf(tmpBuff_67BD9C, L"num peds on screen : %d",
                 num_peds); // num peds on screen : %d
        gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 64, word_706600, 1);

        if (gPolice_7B8_6FEE40)
        {
            swprintf(tmpBuff_67BD9C,
                     L"num police peds in range screen : %d", // num police peds in range screen : %d
                     (u8)gPolice_7B8_6FEE40->field_7AD_police_peds_in_range_screen);
            gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 80, word_706600, 1);
        }
    }
}

// https://decomp.me/scratch/P1OvR
WIP_FUNC(0x470650)
PedManager::PedManager()
{
    field_8 = 0;
    if (!gPedPool_6787B8)
    {
        gPedPool_6787B8 = new PedPool();
        if (!gPedPool_6787B8)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15827); // OutOfMemoryNewOperator
        }
    }

    if (!gChar_B4_Pool_6FDB44)
    {
        gChar_B4_Pool_6FDB44 = new Char_B4_Pool();
        if (!gChar_B4_Pool_6FDB44)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15834); // OutOfMemoryNewOperator
        }
    }

    if (!gChar_8_Pool_678b50)
    {
        gChar_8_Pool_678b50 = new Char_8_Pool();
        if (!gChar_8_Pool_678b50)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15841); // OutOfMemoryNewOperator
        }
    }

    field_8 = gSprite_Pool_703818->get_new_sprite();

    field_2 = 0;
    field_3 = 0;
    field_4 = 0;
    field_6_num_peds_on_screen = 0;
    field_0 = 50;
    field_7_make_all_muggers = false;
    /*
    gPedId_61A89C = 7;
    dword_6787C0 = 0;
    word_6787C6 = 0;
    byte_6787C8 = 0;
    byte_6787C9 = 0;
    gNumberMuggersSpawned_6787CA = 0;
    gNumberCarThiefsSpawned_6787CB = 0;
    gNumberElvisLeadersSpawned_6787CC = 0;
    gNumberWalkingCopsSpawned_6787CD = 0;
    byte_6787CE = 0;
    word_6787D0 = 0;
    this->field_5_fbi_army_count = 0;
    HIWORD(dword_678654) = word_61A898;
    word_6787F0 = 0;
    byte_6787D2 = 0;
    byte_6787D3 = 0;
    HIWORD(dword_6784EE) = gDummyPedAng_6787A8;
    byte_6787D4 = 0;
    spawnSideLocked_6787D5 = 0;
    spawnCountLimit_6787D6 = 0;
    byte_6787D7 = 0;
    byte_61A8A0 = 1;
    byte_61A8A1 = 1;
    byte_6787D8 = 0;
    byte_61A8A2 = 1;
    byte_6787D9 = 0;
    byte_6787DA = 0;
    gDistanceToTarget_678750 = k_dword_678660;
    dword_6787DC = 0;
    cameraFacingAng_678760 = gDummyPedAng_6787A8;
    byte_61A8A3 = 1;
    byte_61A8A4 = 1;
    word_6787E0 = 0;
    byte_6787E2 = 0;
    byte_6787E3 = 0;
    gNumPedsOnScreen_6787EC = 0;
    unk_6787EF = 0;
    */
    sub_553F90();
    gThreateningPedsList_678468.ClearList_420E90();
}

STUB_FUNC(0x4709b0)
PedManager::~PedManager()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x470a50)
Ped* PedManager::SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation)
{
    Ped* pPed = gPedPool_6787B8->Allocate();

    if (!pPed->AllocCharB4_45C830(xpos, ypos, zpos))
    {
        return 0;
    }
    pPed->field_168_game_object->field_40_rotation.rValue = rotation.rValue;
    pPed->field_244_remap = remap;

    Char_B4* pB4 = pPed->field_168_game_object;
    pB4->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pB4->field_80_sprite_ptr->SetRemap(remap);
    }
    pPed->field_134_rotation = rotation;
    pPed->field_288_threat_search = 2;
    pPed->field_28C_threat_reaction = 3;
    pPed->field_216_health = 100;
    pPed->field_26C_graphic_type = 0;
    return pPed;
}

MATCH_FUNC(0x470b00)
Ped* PedManager::SpawnDriver_470B00(Car_BC* pCar)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_238 = 3;
    pNewPed->field_240_occupation = 4; //unknown_2;
    pNewPed->field_16C_car = pCar;
    pNewPed->field_168_game_object = 0;
    pNewPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2_45C540(10);
    pNewPed->field_248_enter_car_as_passenger = 0;
    pNewPed->field_24C_target_car_door = 0;
    pNewPed->field_288_threat_search = 2; //area_2;
    pNewPed->field_28C_threat_reaction = 3; //run_away_3;
    pNewPed->field_216_health = 100;
    pNewPed->field_26C_graphic_type = 0;
    pCar->SetDriver(pNewPed);
    return pNewPed;
}

// https://decomp.me/scratch/6AW5o
MATCH_FUNC(0x470ba0)
Ped* PedManager::SpawnGangDriver_470BA0(Car_BC* pCar, Gang_144* pGang)
{
    Ped* pNewPed = gPedPool_6787B8->field_0_pool.Allocate();

    pNewPed->field_238 = 6;
    pNewPed->field_240_occupation = ped_ocupation_enum::unknown_19;
    pNewPed->field_16C_car = pCar;
    pNewPed->field_168_game_object = 0;
    pNewPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2_45C540(10);
    pNewPed->field_248_enter_car_as_passenger = 0;
    pNewPed->field_24C_target_car_door = 0;
    pNewPed->field_288_threat_search = threat_search_enum::area_2;
    pNewPed->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
    pNewPed->field_216_health = 100;
    pNewPed->field_26C_graphic_type = 1;

    pCar->SetDriver(pNewPed);

    pNewPed->field_17C_pZone = pGang;
    pNewPed->field_244_remap = pGang->field_101;

    if (pNewPed->field_244_remap == 5)
    {
        s16 constant = 2;
        if (!stru_6F6784.get_int_4F7AE0(&constant))
        {
            pNewPed->field_244_remap = 6;
        }
    }
    pNewPed->field_26C_graphic_type = 1;
    pNewPed->field_22C = 1;

    pNewPed->ForceWeapon_46F600(pNewPed->field_17C_pZone->GetGangCurrWeapon_4BF0C0());
    pNewPed->GiveWeapon_46F650(weapon_type::pistol);

    pNewPed->field_270 = 0;
    pNewPed->field_288_threat_search = threat_search_enum::line_of_sight_1;
    pNewPed->field_28C_threat_reaction = threat_reaction_enum::react_as_normal_2;

    return pNewPed;
}

MATCH_FUNC(0x470cc0)
Ped* PedManager::sub_470CC0(Car_BC* pCar)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_244_remap = -1;
    pNewPed->field_26C_graphic_type = 0;
    pNewPed->field_238 = 3;
    pNewPed->field_240_occupation = 4; //unknown_2;
    pNewPed->field_16C_car = pCar;
    pNewPed->field_168_game_object = 0;
    pNewPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2_45C540(10);
    pNewPed->field_24C_target_car_door = 0;
    pNewPed->field_288_threat_search = 2; //area_2;
    pNewPed->field_28C_threat_reaction = 3; //run_away_3;
    pNewPed->field_216_health = 100;
    pNewPed->field_26C_graphic_type = 0;
    return pNewPed;
}

STUB_FUNC(0x470d60)
Ped* PedManager::sub_470D60()
{
    NOT_IMPLEMENTED;
    return 0;
}

// 9.6f 0x43DEB0
WIP_FUNC(0x470e30)
Ped* PedManager::SpawnTrainLeaver_470E30()
{
    WIP_IMPLEMENTED;

    Ped* pPed = gPedPool_6787B8->Allocate();

    // TODO: Instruction swap here
    pPed->field_26C_graphic_type = 0;

    s16 base_rng = 4;
    switch (stru_6F6784.get_int_4F7AE0(&base_rng))
    {
        case 0:
            pPed->field_244_remap = 18;
            break;
        case 1:
            pPed->field_244_remap = 19;
            break;
        case 2:
            pPed->field_244_remap = 20;
            break;
        default:
            pPed->field_244_remap = 21;
            break;
    }

    pPed->field_238 = 4;
    pPed->field_240_occupation = 9;
    pPed->field_168_game_object = 0;
    pPed->ChangeNextPedState1_45C500(10);
    pPed->ChangeNextPedState2_45C540(10);
    pPed->field_216_health = 50;
    pPed->field_288_threat_search = threat_search_enum::area_2;
    pPed->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
    return pPed;
}

MATCH_FUNC(0x470f30)
Ped* PedManager::sub_470F30()
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_216_health = 100;
    return pNewPed;
}

MATCH_FUNC(0x470f90)
Ped* PedManager::sub_470F90(Ped* pSrc)
{
    Ped* pDst = gPedPool_6787B8->Allocate();
    Ped* pNext = pDst->mpNext;
    memcpy(pDst, pSrc, sizeof(Ped));
    pDst->mpNext = pNext;

    if (pSrc->field_168_game_object)
    {
        pDst->AllocCharB4_45C830(pSrc->field_1AC_cam.x, pSrc->field_1AC_cam.y, pSrc->field_1AC_cam.z);
        Char_B4* pCharObj = pDst->field_168_game_object;
        u8 remap = pSrc->field_244_remap;
        pCharObj->field_5_remap = remap;
        if (remap != 0xFF)
        {
            pCharObj->field_80_sprite_ptr->SetRemap(remap);
        }
        pDst->field_168_game_object->set_rotation_433A30(pSrc->GetRotation());
        pDst->field_168_game_object->field_16 = 1;
        pDst->field_168_game_object->field_84 = pSrc->field_168_game_object->field_84;
    }
    return pDst;
}

WIP_FUNC(0x471060)
void PedManager::DoIanTest_471060(s16 key)
{
    WIP_IMPLEMENTED;

    switch (key)
    {
        case DIK_9:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetOnFire();
            break;
        case DIK_8:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetVisible();
            break;

        case DIK_7:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetInvisible();
            break;
    }
}

MATCH_FUNC(0x4710c0)
Ped* PedManager::PedById(s32 pedId)
{
    for (Ped* pPedIter = gPedPool_6787B8->field_0_pool.field_4_pPrev; pPedIter; pPedIter = pPedIter->mpNext)
    {
        if (pPedIter->field_200_id == pedId)
        {
            return pPedIter;
        }
    }
    return NULL;
}

WIP_FUNC(0x470330)
void PedManager::Dummies_470330()
{
    WIP_IMPLEMENTED;

    s16 v1 = gPedManager_6787BC->field_0;
    if (gPolice_7B8_6FEE40->field_654_wanted_level > 3)
    {
        v1 = (u16)v1 >> 1;
    }
    if (byte_6787E2 < v1)
    {
        for (Camera_0xBC* pCam = gGame_0x40_67E008->IteratePlayerCamera_4B9BC0(); pCam; pCam = gGame_0x40_67E008->sub_4B9C50())
        {
            spawnSideLocked_6787D5 = 0;
            if (pCam->field_34_ped || pCam->field_38_car)
            {
                if (pCam->sub_435A20() > k_dword_678438)
                {
                    // TODO: BL register is reused to set these to 1 instead
                    // of a constant value :)
                    spawnSideLocked_6787D5 = 1;
                    cameraFacingAng_678760 = pCam->sub_4358D0();
                    spawnCountLimit_6787D6 = 1; // Set this to 3 and apart from the wrong constant it matches
                }
                else
                {
                    spawnCountLimit_6787D6 = 2;
                }
            }
            // Use global instance even tho we're already in an instance method :)
            gPedManager_6787BC->SpawnDummies_46EB60(pCam);
        }
    }
}

MATCH_FUNC(0x471110)
PedPool::~PedPool()
{
}

MATCH_FUNC(0x46DB90)
EXPORT Ped* __stdcall SpawnPedChainGroupAt_46DB90(char_type remap, u8 number_followers, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Fix16 xpos_adjusted = dword_6784A0 + Fix16((u8)(xpos.ToInt()));
    Fix16 ypos_adjusted = dword_6784A0 + Fix16((u8)(ypos.ToInt()));

    Ped* pLeader = gPedPool_6787B8->Allocate();
    pLeader->field_240_occupation = ped_ocupation_enum::elvis_leader;
    pLeader->field_244_remap = remap;
    pLeader->field_26C_graphic_type = 1;
    pLeader->field_238 = 4;
    pLeader->AllocCharB4_45C830(xpos_adjusted, ypos_adjusted, zpos);
    pLeader->field_168_game_object->SetRemap_46DD50(pLeader->field_244_remap);
    pLeader->field_216_health = 100;

    PedGroup* pGroup = PedGroup::New_4CB0D0();
    pGroup->add_ped_leader_4C9B10(pLeader);
    pGroup->field_38_group_type = 1;
    pGroup->field_36_count = number_followers;
    pGroup->field_34_count = number_followers;

    for (u8 ped_idx = 0; ped_idx < number_followers; ped_idx++)
    {
        Fix16 xy_off = Fix16(0x4000 * (ped_idx + 1), 0);

        Ped* pNewPed = gPedPool_6787B8->Allocate();

        pNewPed->field_240_occupation = ped_ocupation_enum::elvis;
        pNewPed->field_244_remap = remap;
        pNewPed->field_238 = 4;
        pNewPed->AllocCharB4_45C830(xpos_adjusted - ((dword_678480 * xy_off)), ypos_adjusted - ((dword_678480 * xy_off)), zpos);

        Char_B4* pB4 = pNewPed->field_168_game_object;
        const u8 cur_remap = pNewPed->field_244_remap;
        pB4->field_5_remap = cur_remap;
        if (cur_remap != 0xFF)
        {
            pB4->field_80_sprite_ptr->SetRemap(cur_remap);
        }

        pNewPed->field_216_health = 100;
        pNewPed->field_26C_graphic_type = 1;
        pGroup->add_ped_to_list_4C9B30(pNewPed, ped_idx);
    }
    return pLeader;
}