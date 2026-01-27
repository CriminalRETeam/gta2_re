#include "miss2_0x11c.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Cranes.hpp"
#include "Crushers.hpp"
#include "Door_4D4.hpp"
#include "Frismo_25C.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Garage_48.hpp"
#include "Generators.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Light_1D4CC.hpp"
#include "MapRenderer.hpp"
#include "Miss2_25C.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PublicTransport.hpp"
#include "Weapon_8.hpp"
#include "ang16.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "fix16.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "gtx_0x106C.hpp"
#include "lucid_hamilton.hpp"
#include "map_0x370.hpp"
#include "miss2_8.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "text_0x14.hpp"

DEFINE_GLOBAL_INIT(s16, word_6212EE, 1, 0x6212EE);

DEFINE_GLOBAL(miss2_0x11C_Pool*, miss2_0x11C_Pool_6F8064, 0x6F8064);
DEFINE_GLOBAL(SCR_CMD_HEADER*, gBasePtr_6F8070, 0x6F8070);
DEFINE_GLOBAL(Fix16, dword_6F76DC, 0x6F76DC);
DEFINE_GLOBAL(Fix16, dword_6F75F0, 0x6F75F0);
DEFINE_GLOBAL(Fix16, dword_6F77C0, 0x6F77C0);
DEFINE_GLOBAL(Ang16, word_6F8044, 0x6F8044);
DEFINE_GLOBAL(Fix16, dword_6F7570, 0x6F7570);
EXTERN_GLOBAL(Ang16, dword_6F804C);
DEFINE_GLOBAL(Fix16, dword_6F77C4, 0x6F77C4);

static inline bool is_car_weapon(s32& weapon_idx)
{
    return weapon_idx >= 15 && weapon_idx <= 27;
}

STUB_FUNC(0x503200)
void miss2_0x11C::sub_503200()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x503410)
char_type miss2_0x11C::sub_503410(u32 a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5035b0)
void miss2_0x11C::sub_5035B0()
{
    field_118 = 0;
    field_11A = word_6212EE++;
}

MATCH_FUNC(0x5035d0)
void miss2_0x11C::sub_5035D0()
{
    if (gDo_miss_logging_67D6BC)
    {
        sprintf(gTmpBuffer_67C598, "%d:\t\t%d ", rng_dword_67AB34->field_0_rng, (s16)field_4_next_cmd);
        gMiss2Log_6F7698.Write_Log_4D9650(gTmpBuffer_67C598);
    }
}

DEFINE_GLOBAL(s32, dword_6F806C, 0x6F806C);

MATCH_FUNC(0x503610)
void miss2_0x11C::sub_503610()
{
    field_10 = 1;
}

MATCH_FUNC(0x503620)
void miss2_0x11C::Next_503620(SCR_CMD_HEADER* a2)
{
    if ((u16)a2->field_4_cmd_next != 0xFFFF)
    { // FF FF (low endian) is the script terminator
        dword_6F806C = field_4_next_cmd;
        field_4_next_cmd = a2->field_4_cmd_next;
        field_C = 0;
    }
    else
    {
        miss2_0x11C::EndCmd_503670();
    }
}

MATCH_FUNC(0x503650)
void miss2_0x11C::JumpToCmd_503650(u16 a2)
{
    if (a2 != 0xFFFF)
    {
        field_4_next_cmd = a2;
    }
    else
    {
        miss2_0x11C::EndCmd_503670();
    }
}

MATCH_FUNC(0x503670)
void miss2_0x11C::EndCmd_503670()
{
    field_4_next_cmd = 0xFFFFu;
}

MATCH_FUNC(0x503680)
void miss2_0x11C::SCRCMD_OBJ_DECSET_2D_3D_503680(SCR_OBJ_DATA* pCmd, SCR_POINTER* a2)
{
    if (pCmd->field_C_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        pCmd->field_C_pos.field_8_z =
            *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y);
    }

    if (pCmd->field_18_obj_id < 0xC8u || pCmd->field_18_obj_id > 0xF4u)
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_1A_rot);
        rotation.Normalize();

        a2->field_8_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(pCmd->field_18_obj_id,
                                                        pCmd->field_C_pos.field_0_x,
                                                        pCmd->field_C_pos.field_4_y,
                                                        pCmd->field_C_pos.field_8_z,
                                                        rotation);
    }
    else
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_1A_rot);
        rotation.Normalize();

        a2->field_8_obj = gObject_5C_6F8F84->sub_529BC0(pCmd->field_18_obj_id,
                                                        pCmd->field_C_pos.field_0_x,
                                                        pCmd->field_C_pos.field_4_y,
                                                        pCmd->field_C_pos.field_8_z,
                                                        rotation);
    }

    if (a2->field_8_obj != NULL)
    {
        s32 v8 = a2->field_8_obj->field_18_model;
        if (v8 == 176 || v8 == 177 || v8 == 178 || v8 == 179 || v8 == 180 || v8 == 181)
        {
            gHud_2B00_706620->field_1F18.place_gang_phone_5D1110(a2->field_8_obj);
            for (u8 v9 = 0; v9 < 0x1Fu; v9++)
            {
                if (gfrosty_pasteur_6F8060->field_C1E32[v9] == 0)
                {
                    gfrosty_pasteur_6F8060->field_C1E32[v9] = a2->field_0_cmd_this;
                    break;
                }
            }
        }
    }
}

MATCH_FUNC(0x5038d0)
void miss2_0x11C::SCRCMD_OBJ_DECSET_5038D0(SCR_OBJ_DATA* pCmd, SCR_POINTER* a2)
{
    if (pCmd->field_C_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        pCmd->field_C_pos.field_8_z =
            *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y);
    }

    if (pCmd->field_18_obj_id < 0xC8u || pCmd->field_18_obj_id > 0xF4u)
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_1A_rot);
        rotation.sub_406C20();

        a2->field_8_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(pCmd->field_18_obj_id,
                                                        pCmd->field_C_pos.field_0_x,
                                                        pCmd->field_C_pos.field_4_y,
                                                        pCmd->field_C_pos.field_8_z,
                                                        rotation);
    }
    else
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_1A_rot);
        rotation.sub_406C20();

        a2->field_8_obj = gObject_5C_6F8F84->sub_529BC0(pCmd->field_18_obj_id,
                                                        pCmd->field_C_pos.field_0_x,
                                                        pCmd->field_C_pos.field_4_y,
                                                        pCmd->field_C_pos.field_8_z,
                                                        rotation);
    }

    if (a2->field_8_obj != NULL)
    {
        Object_2C* pObj;

        if (a2->field_8_obj->field_8->field_34_behavior_type == object_behavior_type::behavior_1)
        {
            a2->field_8_obj->field_26_varrok_idx = pCmd->field_1C_value_shop_type;
        }
        else if (pObj = a2->field_8_obj, pObj->check_is_shop_421060())
        {
            pObj->field_26_varrok_idx = pCmd->field_1C_value_shop_type;
        }
        else
        {
            if (a2->field_8_obj->check_is_busy_shop())
            {
                a2->field_8_obj->field_26_varrok_idx = pCmd->field_1C_value_shop_type;
            }
        }
    }
}

MATCH_FUNC(0x503a20)
void miss2_0x11C::SCRCMD_PLAYER_PED_503A20(SCR_PLAYER_PED* pCmd)
{

    Player* v1 = gGame_0x40_67E008->sub_4B9750();

    if (v1)
    {
        Ped* pPed;
        if (gfrosty_pasteur_6F8060->field_C1E2C)
        {
            Fix16 weird_y = dword_6F76DC + gGameSave_6F78C8.field_54_player_and_world_stats.field_4_y.GetRoundValue();
            Fix16 weird_x = dword_6F75F0 + gGameSave_6F78C8.field_54_player_and_world_stats.field_0_x.GetRoundValue();

            pPed = gPedManager_6787BC->SpawnPedAt(weird_x,
                                                  weird_y,
                                                  gGameSave_6F78C8.field_54_player_and_world_stats.field_8_z,
                                                  gGameSave_6F78C8.field_54_player_and_world_stats.field_7F_player_ped_remap,
                                                  dword_6F804C);
        }
        else
        {
            if (pCmd->field_C_pos.field_8_z == dword_6F7570) //  dword_6F7570 is 255.0
            {
                Fix16 temp_z;
                //  Calculate the real Z position at (X,Y) based on the map
                pCmd->field_C_pos.field_8_z =
                    *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y);
            }

            Ang16 rotation;
            rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
            rotation.Normalize();

            pPed = gPedManager_6787BC->SpawnPedAt(pCmd->field_C_pos.field_0_x,
                                                  pCmd->field_C_pos.field_4_y,
                                                  pCmd->field_C_pos.field_8_z,
                                                  pCmd->field_1A_remap,
                                                  rotation);
        }

        if (pPed != NULL)
        {
            pPed->field_238 = 2;
            if (!gfrosty_pasteur_6F8060->field_C1E2C)
            {
                pPed->field_216_health = 100;
            }

            v1->InitPlayerPed_565490(pPed);
            pPed->field_26C_graphic_type = 1;
            pCmd->field_8_ped = pPed;

            Sprite* v6 = pPed->GetSprite_46DF50();
            v6->ResolveCollisionWithCarPedOrObject_5A2A30();
        }
    }
}

MATCH_FUNC(0x503bc0)
void miss2_0x11C::SCRCMD_CAR_DECSET_503BC0(SCR_CAR_DATA_DEC* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->field_C_pos.field_8_z == dword_6F7570) //  255.0
    {
        Fix16 temp_z;
        pCmd->field_C_pos.field_8_z =
            *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y);
    }
    if (pCmd->field_1E_trailer_id == -1) //  No trailers
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
        rotation.sub_406C20();
        pPointer->field_8_car = gCar_6C_677930->SpawnCar_426E10(pCmd->field_C_pos.field_0_x,
                                                           pCmd->field_C_pos.field_4_y,
                                                           pCmd->field_C_pos.field_8_z,
                                                           rotation,
                                                           pCmd->field_1C_car_id);
    }
    else if (pCmd->field_1E_trailer_id == -2) //  Mini car
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
        rotation.sub_406C20();
        pPointer->field_8_car = gCar_6C_677930->SpawnCar_4764A0(pCmd->field_C_pos.field_0_x,
                                                           pCmd->field_C_pos.field_4_y,
                                                           pCmd->field_C_pos.field_8_z,
                                                           rotation,
                                                           pCmd->field_1C_car_id);
        if (pPointer->field_8_car != NULL)
        {
            pPointer->field_8_car->field_98 = 4;
            pPointer->field_8_car->field_78_flags |= 0x10u;
            pPointer->field_8_car->sub_4435F0();
        }
    }
    else //  There is a trailer
    {
        if (pCmd->field_1C_car_id == car_model_enum::TRUKTRNS) // 66 = TRUKTRNS Truck Trailer, Flatbed
        {
            Car_BC* v7 = gCar_6C_677930->SpawnCar_shortened(pCmd->field_1E_trailer_id);
            Ang16 rotation;
            rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
            rotation.Normalize();

            pPointer->field_8_car = gCar_6C_677930->SpawnCar_426E10(pCmd->field_C_pos.field_0_x,
                                                               pCmd->field_C_pos.field_4_y,
                                                               pCmd->field_C_pos.field_8_z,
                                                               rotation,
                                                               pCmd->field_1C_car_id);
            pPointer->field_8_car->field_50_car_sprite->DispatchCollisionEvent_5A3100(v7->field_50_car_sprite, dword_6F77C0, dword_6F77C0, word_6F771E);
            v7->IncrementCarStats_443D70(8);
        }
        else
        {
            Ang16 rotation;
            rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
            rotation.Normalize();

            Trailer* pTrailer = gCar_6C_677930->sub_446530(pCmd->field_C_pos.field_0_x,
                                                           pCmd->field_C_pos.field_4_y,
                                                           rotation,
                                                           pCmd->field_1C_car_id,
                                                           pCmd->field_1E_trailer_id);
            if (pTrailer != NULL)
            {
                pPointer->field_8_car = pTrailer->field_8_truck_cab;
            }
            else
            {
                pPointer->field_8_car = NULL;
            }
        }
    }
    if (pPointer->field_8_car != NULL)
    {
        if (pCmd->field_1A_remap != -1)
        {
            pPointer->field_8_car->SetCarRemap(pCmd->field_1A_remap);
        }

        Car_BC* v12 = pPointer->field_8_car;
        v12->field_7C_uni_num = 5;
        v12->field_76 = 0;

        if (pPointer->field_8_car->field_98 != 4)
        {
            pPointer->field_8_car->field_98 = 2;
        }
        pPointer->field_8_car->IncrementCarStats_443D70(8);
        pPointer->field_8_car->field_50_car_sprite->ResolveCollisionWithCarPedOrObject_5A2A30();

        if (pCmd->field_2_type >= 0x18Au //  create gang car
            && pCmd->field_2_type <= 0x18Du)
        {
            s8 zone_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pPointer->field_8_car->field_84_car_info_idx);
            if (zone_idx > -1)
            {
                Gang_144* pZone = gGangPool_CA8_67E274->GangByIdx_4BF1C0(zone_idx);
                pPointer->field_8_car->AttachGangIcon_440660(pZone->field_138_arrow_colour);
            }
        }
    }
}

MATCH_FUNC(0x503f80)
void miss2_0x11C::SCRCMD_PARKED_CAR_DECSET_503F80(SCR_POINTER* pCmd)
{
    miss2_0x11C::SCRCMD_CAR_DECSET_503BC0((SCR_CAR_DATA_DEC*)pCmd, pCmd);
    (pCmd->field_8_car)->sub_443EB0(9);
    Car_BC* v1 = pCmd->field_8_car;
    v1->field_7C_uni_num = 4;
    v1->field_76 = 0;
}

MATCH_FUNC(0x503fb0)
void miss2_0x11C::SCRCMD_CHAR_DECSET_2D_3D_503FB0(SCR_CHAR_DATA_DEC* pCmd, SCR_POINTER* a2)
{
    Ped* pPed;

    if (pCmd->field_C_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        pCmd->field_C_pos.field_8_z =
            *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y);
    }

    Ang16 rotation;
    rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
    rotation.Normalize();

    pPed = gPedManager_6787BC->SpawnPedAt(pCmd->field_C_pos.field_0_x,
                                          pCmd->field_C_pos.field_4_y,
                                          pCmd->field_C_pos.field_8_z,
                                          (u8)pCmd->field_1A_remap,
                                          rotation);
    a2->field_8_char = pPed;

    if (pPed)
    {
        a2->field_8_char->field_238 = 5;
        a2->field_8_char->field_240_occupation = pCmd->field_1C_occupation;
        a2->field_8_char->field_26C_graphic_type = 1;
        a2->field_8_char->SetObjective(objectives_enum::wait_on_foot_26, 9999);
        a2->field_8_char->field_216_health = 100;
        Sprite* v6 = a2->field_8_char->GetSprite_46DF50();
        v6->ResolveCollisionWithCarPedOrObject_5A2A30();
    }
}

MATCH_FUNC(0x504110)
Ped* miss2_0x11C::sub_504110(SCR_CHAR_OBJECTIVE* a1, SCR_POINTER* a2)
{
    (a2->field_8_char)->SetObjective(a1->field_A_objective, 9999);
    Ped* v2 = a2->field_8_char;
    BitSet32 flag = v2->field_21C;
    flag.clear_bit(10);
    v2->field_21C = flag.m_var;
    return v2;
}

MATCH_FUNC(0x504150)
void miss2_0x11C::SCRCMD_MAP_ZONE_SET_504150(SCR_MAP_ZONE_SET* pCmd)
{
    str_table_entry* StringById_503080;

    StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_0_cmd_this);
    gMap_0x370_6F6268->set_nav_unknown_data_4DF8C0(StringById_503080->field_2_zone_idx,
                                                   pCmd->field_8_car_density,
                                                   pCmd->field_A_goodcar_ratio,
                                                   pCmd->field_C_badcar_ratio,
                                                   pCmd->field_E_policecar_ratio,
                                                   pCmd->field_1C_gangcar_ratio,
                                                   pCmd->field_10_ped_density,
                                                   pCmd->field_12_mugger_ratio,
                                                   pCmd->field_14_carthief_ratio,
                                                   pCmd->field_16_elvis_ratio,
                                                   pCmd->field_18_gangchar_ratio,
                                                   pCmd->field_1A_policeped_ratio);
}

MATCH_FUNC(0x5041b0)
void miss2_0x11C::SCRCMD_ARROW_DEC_5041B0(SCR_TWO_PARAMS* pCmd)
{
    pCmd->field_8_u32 = 0;
}

STUB_FUNC(0x5041c0)
s32 miss2_0x11C::SCRCMD_CRANE_5041C0(s32 a1, s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5043a0)
void miss2_0x11C::SCRCMD_CONVEYOR_DECSET1_2_5043A0(SCR_CONVEYOR* a1, SCR_POINTER* a2)
{
    if (a1->field_C_rect.field_0_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        a1->field_C_rect.field_0_pos.field_8_z = *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z,
                                                                                                a1->field_C_rect.field_0_pos.field_0_x,
                                                                                                a1->field_C_rect.field_0_pos.field_4_y);
    }
    a2->field_8_obj = gObject_5C_6F8F84->NewTouchPoint_529950(139,
                                                    a1->field_C_rect.field_0_pos.field_0_x,
                                                    a1->field_C_rect.field_0_pos.field_4_y,
                                                    a1->field_C_rect.field_0_pos.field_8_z,
                                                    dword_6F804C,
                                                    a1->field_C_rect.field_C_size.field_0_x,
                                                    a1->field_C_rect.field_C_size.field_4_y,
                                                    dword_6F77C4);

    a2->field_8_obj->sub_529030(a1->field_18_speed.field_0_x, a1->field_18_speed.field_1_y);
}

MATCH_FUNC(0x504420)
void miss2_0x11C::SCRCMD_GENERATOR_DECSET_504420(SCR_GENERATOR* pCmd, SCR_POINTER* a2)
{
    if (pCmd->field_C_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        pCmd->field_C_pos.field_8_z =
            *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y);
    }

    Ang16 rotation;
    rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
    rotation.Normalize();

    a2->field_8_generator = gGeneratorPool_14AC_67E5D0->CreateGenerator_4C1DC0(pCmd->field_C_pos.field_0_x,
                                                                               pCmd->field_C_pos.field_4_y,
                                                                               pCmd->field_C_pos.field_8_z,
                                                                               rotation,
                                                                               pCmd->field_1A_obj_id,
                                                                               pCmd->field_1C_mindelay,
                                                                               pCmd->field_1E_maxdelay);
    if (pCmd->field_20_ammo > 0)
    {
        a2->field_8_generator->field_1C_ammo = pCmd->field_20_ammo;
    }
}

MATCH_FUNC(0x504530)
void miss2_0x11C::SCRCMD_DESTRUCTOR_DECSET_504530(SCR_DESTRUCTOR* a1, SCR_POINTER* a2)
{
    if (a1->field_C_rect.field_0_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        a1->field_C_rect.field_0_pos.field_8_z = *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z,
                                                                                                a1->field_C_rect.field_0_pos.field_0_x,
                                                                                                a1->field_C_rect.field_0_pos.field_4_y);
    }

    a2->field_8_obj = gObject_5C_6F8F84->NewTouchPoint_529950(141,
                                                    a1->field_C_rect.field_0_pos.field_0_x,
                                                    a1->field_C_rect.field_0_pos.field_4_y,
                                                    a1->field_C_rect.field_0_pos.field_8_z,
                                                    dword_6F804C,
                                                    a1->field_C_rect.field_C_size.field_0_x,
                                                    a1->field_C_rect.field_C_size.field_4_y,
                                                    dword_6F77C4);
}

MATCH_FUNC(0x5045a0)
void miss2_0x11C::SCRCMD_CRUSHER_BASIC_5045A0(SCR_CRUSHER_BASIC* a1, SCR_POINTER* a2)
{
    a2->field_8_crusher = gCrusherPool_94_67A830->CreateCrusher_488820(a1->field_C_pos.field_0_x, a1->field_C_pos.field_4_y);
}

STUB_FUNC(0x5045d0)
s32 miss2_0x11C::SCRCMD_THREAD_DECLARE2_5045D0(s32 a1, s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x504660)
void miss2_0x11C::SCRCMD_THREAD_DECLARE3_504660(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x504710)
void miss2_0x11C::sub_504710(s32 a1)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5047c0)
s32* miss2_0x11C::SCRCMD_THREAD_DECLARE4_5047C0(s32 a1, s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x504830)
void miss2_0x11C::SCRCMD_SET_GANG_INFO1_504830(SCR_SET_GANG_INFO* pCmd)
{
    str_table_entry* string_entry;

    string_entry = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_8_gangname);

    Gang_144* pZone = gGangPool_CA8_67E274->gang_by_name_4BF100((char*)&string_entry[1]);
    pZone->field_101 = pCmd->field_A_remap;

    pZone->field_104_basic_weapon = pCmd->field_B_weapon1;
    pZone->field_108_angry_weapon = pCmd->field_C_weapon2;
    pZone->field_10C_hate_weapon = pCmd->field_D_weapon3;

    pZone->field_138_arrow_colour = pCmd->field_E_arrow_id;
    pZone->field_13C_gang_car_model = pCmd->field_1C_car_model;
    pZone->field_140_gang_car_remap = pCmd->field_1E_car_remap;

    if (pCmd->field_10_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        pCmd->field_10_pos.field_8_z =
            *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, pCmd->field_10_pos.field_0_x, pCmd->field_10_pos.field_4_y);
    }

    Fix16 z = pCmd->field_10_pos.field_8_z;
    Fix16 y = pCmd->field_10_pos.field_4_y;
    Fix16 x = pCmd->field_10_pos.field_0_x;

    pZone->field_130_info_phone_y = y;
    pZone->field_12C_info_phone_x = x;
    pZone->field_134_info_phone_z = z;

    pZone->field_139_kill_respect_change = pCmd->field_F_kill_respect_change;
    gGangPool_CA8_67E274->sub_4BF230(pZone, gfrosty_pasteur_6F8060->field_354);
    ++gfrosty_pasteur_6F8060->field_354;
    Gang_144* v7 = gGangPool_CA8_67E274->gang_by_name_4BF100((char*)&string_entry[1]);
    if ((u8)pCmd->field_F_kill_respect_change > 0)
    {
        gHud_2B00_706620->field_1F18.SetNewGangArrow_5D1310(v7);
    }
}

MATCH_FUNC(0x504950)
void miss2_0x11C::SCRCMD_SET_DOOR_INFO_504950(SCR_FOUR_PARAMS* a1)
{
    gDoor_4D4_67BD2C->RegisterDoorInfo_49D2D0(a1->field_8_unsigned_1, a1->field_A_unsigned_2, a1->field_C_unsigned_3);
}

MATCH_FUNC(0x504970)
void miss2_0x11C::SCRCMD_DOOR_DECLARE_D1_S1_504970(SCR_DOOR_DATA_DEC* a1)
{
    if (a1->field_2_type == SCRCMD_DOOR_DECLARE_S1)
    {
        if (a1->field_14_check.field_C_size.field_0_x == dword_6F77C0 && a1->field_14_check.field_C_size.field_4_y == dword_6F77C0)
        {
            a1->field_8 = gDoor_4D4_67BD2C->RegisterSingleDoorNoCheck_49CF50(a1->field_10_gr_id,
                                                       a1->field_C_block.field_0_x,
                                                       a1->field_C_block.field_1_y,
                                                       a1->field_C_block.field_2_z,
                                                       a1->field_F_face,
                                                       a1->field_28_flip,
                                                       a1->field_29_reversed);
        }
        else
        {
            a1->field_8 = gDoor_4D4_67BD2C->RegisterSingleDoor_49D170(a1->field_10_gr_id,
                                                       a1->field_C_block.field_0_x,
                                                       a1->field_C_block.field_1_y,
                                                       a1->field_C_block.field_2_z,
                                                       a1->field_F_face,
                                                       a1->field_14_check.field_0_pos.field_0_x,
                                                       a1->field_14_check.field_0_pos.field_4_y,
                                                       a1->field_14_check.field_0_pos.field_8_z,
                                                       a1->field_14_check.field_C_size.field_0_x,
                                                       a1->field_14_check.field_C_size.field_4_y,
                                                       a1->field_28_flip,
                                                       a1->field_29_reversed);
        }
    }
    else
    {
        if (a1->field_14_check.field_C_size.field_0_x == dword_6F77C0 && a1->field_14_check.field_C_size.field_4_y == dword_6F77C0)
        {
            a1->field_8 = gDoor_4D4_67BD2C->RegisterDoubleDoorNoCheck_49CFA0(a1->field_10_gr_id,
                                                       a1->field_C_block.field_0_x,
                                                       a1->field_C_block.field_1_y,
                                                       a1->field_C_block.field_2_z,
                                                       a1->field_F_face,
                                                       a1->field_28_flip,
                                                       a1->field_29_reversed);
        }
        else
        {
            a1->field_8 = gDoor_4D4_67BD2C->RegisterDoubleDoor_49D1F0(a1->field_10_gr_id,
                                                       a1->field_C_block.field_0_x,
                                                       a1->field_C_block.field_1_y,
                                                       a1->field_C_block.field_2_z,
                                                       a1->field_F_face,
                                                       a1->field_14_check.field_0_pos.field_0_x,
                                                       a1->field_14_check.field_0_pos.field_4_y,
                                                       a1->field_14_check.field_0_pos.field_8_z,
                                                       a1->field_14_check.field_C_size.field_0_x,
                                                       a1->field_14_check.field_C_size.field_4_y,
                                                       a1->field_28_flip,
                                                       a1->field_29_reversed);
        }
    }
    switch (a1->field_11_open_type)
    {
        case SCR_DOOR_OPENTYPES::any_player:
            a1->field_8->set_field_20(door_open_type::any_player);
            break;
        case SCR_DOOR_OPENTYPES::any_car:
            a1->field_8->set_field_20(door_open_type::any_car);
            break;
        case SCR_DOOR_OPENTYPES::unknown1:
            a1->field_8->set_field_20(door_open_type::unknown1);
            break;
        case SCR_DOOR_OPENTYPES::one_car:
            a1->field_8->set_field_20(door_open_type::one_car);
            break;
        case SCR_DOOR_OPENTYPES::one_char_on_foot:
            a1->field_8->set_field_20(door_open_type::one_char_on_foot);
            break;
        case SCR_DOOR_OPENTYPES::any_player_one_car:
            a1->field_8->set_field_20(door_open_type::any_player_one_car);
            break;
    }

    Door_38* tmp;
    u16 tmp1;
    switch (a1->field_12_close_type)
    {
        case SCR_DOOR_CLOSETYPES::close_never:
            // a1->field_8->set_field_24(3);
            a1->field_8->field_24 = door_close_type::close_never;
            tmp = a1->field_8;
            tmp1 = a1->field_13_delay;
            tmp->field_1C = tmp1;
            tmp->field_1E = tmp1;
            return;
            break;
        case SCR_DOOR_CLOSETYPES::close_time_delay:
            // a1->field_8->set_field_24(1);
            a1->field_8->field_24 = door_close_type::close_time_delay;
            tmp = a1->field_8;
            tmp1 = a1->field_13_delay;
            tmp->field_1C = tmp1;
            tmp->field_1E = tmp1;
            return;
            break;
        case SCR_DOOR_CLOSETYPES::close_when_clear:
            // a1->field_8->set_field_24(2);
            a1->field_8->field_24 = door_close_type::close_when_clear;
            tmp = a1->field_8;
            tmp1 = a1->field_13_delay;
            tmp->field_1C = tmp1;
            tmp->field_1E = tmp1;
            return;
            break;
        case SCR_DOOR_CLOSETYPES::close_when_open_rule_fails:
            // a1->field_8->set_field_24(4);
            a1->field_8->field_24 = door_close_type::close_when_open_rule_fails;
            tmp = a1->field_8;
            tmp1 = a1->field_13_delay;
            tmp->field_1C = tmp1;
            tmp->field_1E = tmp1;
            return;
            break;
    }
    tmp = a1->field_8;
    tmp1 = a1->field_13_delay;
    tmp->field_1C = tmp1;
    tmp->field_1E = tmp1;
    // a1->field_8->set_field_1c_1e(a1->field_13_delay);
}

MATCH_FUNC(0x504b80)
void miss2_0x11C::SCRCMD_DOOR_DECLARE_D2_S2_504B80(SCR_DOOR_DATA_DEC* a3)
{
    if (a3->field_2_type == SCRCMD_DOOR_DECLARE_S2)
    {
        if (a3->field_14_check.field_C_size.field_0_x == dword_6F77C0 && a3->field_14_check.field_C_size.field_4_y == dword_6F77C0)
        {
            a3->field_8 = gDoor_4D4_67BD2C->RegisterSingleDoorNoCheck_49CF50(a3->field_10_gr_id,
                                                       a3->field_C_block.field_0_x,
                                                       a3->field_C_block.field_1_y,
                                                       a3->field_C_block.field_2_z,
                                                       a3->field_F_face,
                                                       a3->field_28_flip,
                                                       a3->field_29_reversed);
        }
        else
        {
            a3->field_8 = gDoor_4D4_67BD2C->RegisterSingleDoor_49D170(a3->field_10_gr_id,
                                                       a3->field_C_block.field_0_x,
                                                       a3->field_C_block.field_1_y,
                                                       a3->field_C_block.field_2_z,
                                                       a3->field_F_face,
                                                       a3->field_14_check.field_0_pos.field_0_x,
                                                       a3->field_14_check.field_0_pos.field_4_y,
                                                       a3->field_14_check.field_0_pos.field_8_z,
                                                       a3->field_14_check.field_C_size.field_0_x,
                                                       a3->field_14_check.field_C_size.field_4_y,
                                                       a3->field_28_flip,
                                                       a3->field_29_reversed);
        }
    }
    else
    {
        if (a3->field_14_check.field_C_size.field_0_x == dword_6F77C0 && a3->field_14_check.field_C_size.field_4_y == dword_6F77C0)
        {
            a3->field_8 = gDoor_4D4_67BD2C->RegisterDoubleDoorNoCheck_49CFA0(a3->field_10_gr_id,
                                                       a3->field_C_block.field_0_x,
                                                       a3->field_C_block.field_1_y,
                                                       a3->field_C_block.field_2_z,
                                                       a3->field_F_face,
                                                       a3->field_28_flip,
                                                       a3->field_29_reversed);
        }
        else
        {
            a3->field_8 = gDoor_4D4_67BD2C->RegisterDoubleDoor_49D1F0(a3->field_10_gr_id,
                                                       a3->field_C_block.field_0_x,
                                                       a3->field_C_block.field_1_y,
                                                       a3->field_C_block.field_2_z,
                                                       a3->field_F_face,
                                                       a3->field_14_check.field_0_pos.field_0_x,
                                                       a3->field_14_check.field_0_pos.field_4_y,
                                                       a3->field_14_check.field_0_pos.field_8_z,
                                                       a3->field_14_check.field_C_size.field_0_x,
                                                       a3->field_14_check.field_C_size.field_4_y,
                                                       a3->field_28_flip,
                                                       a3->field_29_reversed);
        }
    }

    switch (a3->field_11_open_type)
    {
        case SCR_DOOR_OPENTYPES::any_player:
            //a3->field_8->field_20_state = 1;
            a3->field_8->set_field_20(door_open_type::any_player);
            break;
        case SCR_DOOR_OPENTYPES::any_car:
            //a3->field_8->field_20_state = 2;
            a3->field_8->set_field_20(door_open_type::any_car);
            break;
        case SCR_DOOR_OPENTYPES::one_car:
        case SCR_DOOR_OPENTYPES::any_player_one_car:
        {
            SCR_POINTER* tmp = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(a3->field_30_somename);
            Car_BC* ped_ptr = tmp->field_8_car;
            if (ped_ptr != NULL)
            {
                a3->field_8->set_open_details_car_bc(door_open_type::one_car, ped_ptr);
                a3->field_8->set_field_14_id(tmp->field_8_car->field_6C_maybe_id);
            }
            break;
        }

        case SCR_DOOR_OPENTYPES::one_model:
            a3->field_8->set_open_details_model_id(door_open_type::one_model, a3->field_30_somename);
            break;

        case SCR_DOOR_OPENTYPES::one_char_on_foot:
        {
            SCR_POINTER* tmp = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(a3->field_30_somename);
            Ped* ped_ptr = tmp->field_8_char;
            if (ped_ptr != NULL)
            {
                a3->field_8->set_open_details_ped(door_open_type::one_char_on_foot, ped_ptr);
                a3->field_8->set_field_14_id(tmp->field_8_char->field_200_id);
            }
        }
        break;
        case SCR_DOOR_OPENTYPES::unknown1:
            a3->field_8->set_field_20(door_open_type::unknown1);
            break;
    }

    switch (a3->field_12_close_type)
    {
        case SCR_DOOR_CLOSETYPES::close_never:
            a3->field_8->set_field_24(door_close_type::close_never);
            break;
        case SCR_DOOR_CLOSETYPES::close_time_delay:
            a3->field_8->set_field_24(door_close_type::close_time_delay);
            break;
        case SCR_DOOR_CLOSETYPES::close_when_clear:
            a3->field_8->set_field_24(door_close_type::close_when_clear);
            break;
        case SCR_DOOR_CLOSETYPES::close_when_open_rule_fails:
            a3->field_8->set_field_24(door_close_type::close_when_open_rule_fails);
            break;
    }
    Door_38* tmp;
    u16 tmp1;
    tmp = a3->field_8;
    tmp1 = a3->field_13_delay;
    tmp->field_1C = tmp1;
    tmp->field_1E = tmp1;
    // a3->field_8->set_field_1c_1e(a3->field_13_delay);
}

MATCH_FUNC(0x504dd0)
void miss2_0x11C::SCRCMD_DECLARE_MISSION_504DD0(SCR_TWO_PARAMS* a1)
{
    SCR_POINTER* pPointer;

    gfrosty_pasteur_6F8060->GetBasePointer_512770(a1->field_8_unsigned_1); //  player pointer not used because missions are singleplayer
    pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(a1->field_A_unsigned_2);
    if (a1->field_2_type == SCRCMD_DEC_GANG_1_FLAG)
    {
        gfrosty_pasteur_6F8060->field_348_gang_1_mission_flag = &pPointer->field_8_counter;
    }
    else if (a1->field_2_type == SCRCMD_DEC_GANG_2_FLAG)
    {
        gfrosty_pasteur_6F8060->field_34C_gang_2_mission_flag = &pPointer->field_8_counter;
    }
    else if (a1->field_2_type == SCRCMD_DEC_GANG_3_FLAG)
    {
        gfrosty_pasteur_6F8060->field_350_gang_3_mission_flag = &pPointer->field_8_counter;
    }
    else // a1->field_2_type == SCRCMD_DECLARE_MISSION
    {
        gfrosty_pasteur_6F8060->field_344_mission_flag = &pPointer->field_8_counter;
    }
}

MATCH_FUNC(0x504ee0)
void miss2_0x11C::CreateLight_504EE0(SCR_CREATE_LIGHT* pCmd, SCR_POINTER* pPointer)
{
    nostalgic_ellis_0x28* pNewLight = gLight_1D4CC_6F5520->Init_469010(pCmd->field_C_xpos,
                                                                       pCmd->field_10_ypos,
                                                                       pCmd->field_14_zpos,
                                                                       pCmd->field_18_argb,
                                                                       pCmd->field_1C_radius,
                                                                       pCmd->field_20_intensity);
    pPointer->field_8_light = pNewLight;
    if (pCmd->field_21_on_time > 0)
    {
        gLight_1D4CC_6F5520->sub_469070(pNewLight, pCmd->field_21_on_time, pCmd->field_22_off_time, pCmd->field_23_shape);
    }
}

MATCH_FUNC(0x505030)
void miss2_0x11C::SCRCMD_SET_STATION_EMPTY_STATION_505030(SCR_SET_STATION* pCmd)
{
    s32 num_passenger;
    s32 num_freight;

    if (!bSkip_trains_67D550)
    {
        str_table_entry* StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_8_platform);
        gmp_map_zone* station_zone = gMap_0x370_6F6268->zone_by_name_4DEFD0((const char*)&StringById_503080[1]);
        TrainStation_34* pStation = gPublicTransport_181C_6FF1D4->TrainStationForZone_57B4B0(station_zone);

        for (u8 wagon_idx = 0; wagon_idx < 10; wagon_idx++)
        {
            if (wagon_idx < pCmd->field_A_num_passenger)
            {
                pStation->field_24_train_wagons[wagon_idx] = 1; // passenger wagon
            }
            else
            {
                num_freight = pCmd->field_B_num_freight;
                num_passenger = pCmd->field_A_num_passenger;

                if (wagon_idx < num_passenger + num_freight)
                {
                    pStation->field_24_train_wagons[wagon_idx] = 2; // freight wagon
                }
                else
                {
                    if (wagon_idx < num_passenger + num_freight + pCmd->field_C_num_boxcar)
                    {
                        pStation->field_24_train_wagons[wagon_idx] = 3; // boxcar wagon
                    }
                }
            }
        }
        pStation->field_2E_wagons_number = pCmd->field_A_num_passenger + pCmd->field_B_num_freight + pCmd->field_C_num_boxcar;
    }
}

MATCH_FUNC(0x5051d0)
void miss2_0x11C::SCRCMD_RADIOSTATION_DEC_5051D0(SCR_RADIOSTATION_DEC* a1)
{
    gRoot_sound_66B038.DeclareRadioStation_40F030(a1->field_10_station_type, a1->field_8_pos.field_0_x, a1->field_8_pos.field_4_y);
}

MATCH_FUNC(0x505210)
void miss2_0x11C::CRCMD_SET_TRAIN_STATIONS_505210(SCR_TWO_PARAMS* pCmd)
{
    str_table_entry* StringById;

    if (!bSkip_trains_67D550)
    {
        StringById = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_8_unsigned_1);
        const char* station_zone_name = (const char*)&StringById[1];
        gmp_map_zone* station_zone = gMap_0x370_6F6268->zone_by_name_4DEFD0(station_zone_name);
        TrainStation_34* pTrainStation = gPublicTransport_181C_6FF1D4->TrainStationForZone_57B4B0(station_zone);
        pTrainStation->CalculateWagonCount_578820((u8*)&pCmd->field_A_unsigned_2); //  The parameter may be the train wagon array, not sure
    }
}

MATCH_FUNC(0x5052c0)
void miss2_0x11C::SCRCMD_DECLARE_POLICE_5052C0(SCR_DECLARE_POLICELEVEL* pCmd)
{
    u8 max_wanted_level = pCmd->field_A_wanted_level;
    if (max_wanted_level == 0)
    {
        bSkip_police_67D4F9 = 0;
    }
    gPolice_7B8_6FEE40->field_660_wanted_star_count = pCmd->field_A_wanted_level; // max_wanted_level
}

MATCH_FUNC(0x505340)
void miss2_0x11C::SCRCMD_SOUND_DECSET_505340(SCR_SOUND_DECSET* a1, SCR_POINTER* a2)
{
    if (!a1->field_19_play_type)
    {
        a2->field_8_obj = gObject_5C_6F8F84->sub_5299F0(278,
                                                        a1->field_18_sound_id,
                                                        a1->field_C_pos.field_0_x,
                                                        a1->field_C_pos.field_4_y,
                                                        a1->field_C_pos.field_8_z);
    }
    else
    {
        a2->field_8_obj = gObject_5C_6F8F84->sub_5299F0(279,
                                                        a1->field_18_sound_id,
                                                        a1->field_C_pos.field_0_x,
                                                        a1->field_C_pos.field_4_y,
                                                        a1->field_C_pos.field_8_z);
    }
}

MATCH_FUNC(0x505430)
void miss2_0x11C::SCRCMD_SET_MISSIONS_TOTAL_505430(SCR_TWO_PARAMS* pCmd)
{
    switch (pCmd->field_2_type)
    {
        case SCRCMD_TOTAL_MISSIONS:
            gfrosty_pasteur_6F8060->field_314_total_missions = pCmd->field_A_signed_2;
            break;
        case SCRCMD_TOTAL_SECRETS:
            gfrosty_pasteur_6F8060->field_318_total_secrets = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_1_MISSION_TOTAL:
            gfrosty_pasteur_6F8060->field_31C_gang_1_missions_total = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_2_MISSION_TOTAL:
            gfrosty_pasteur_6F8060->field_320_gang_2_missions_total = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_3_MISSION_TOTAL:
            gfrosty_pasteur_6F8060->field_324_gang_3_missions_total = pCmd->field_A_signed_2;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505580)
void miss2_0x11C::SCRCMD_PASSED_FAILED_FLAGS_505580(SCR_TWO_PARAMS* a1)
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(a1->field_8_unsigned_1);

    switch (a1->field_2_type)
    {
        case SCRCMD_PASSED_FLAG:
            gfrosty_pasteur_6F8060->field_328_passed_flag = &pCmd->field_8_counter;
            break;
        case SCRCMD_1_PASSED_FLAG:
            gfrosty_pasteur_6F8060->field_32C_1_passed_flag = &pCmd->field_8_counter;
            break;
        case SCRCMD_2_PASSED_FLAG:
            gfrosty_pasteur_6F8060->field_330_2_passed_flag = &pCmd->field_8_counter;
            break;
        case SCRCMD_3_PASSED_FLAG:
            gfrosty_pasteur_6F8060->field_334_3_passed_flag = &pCmd->field_8_counter;
            break;
        case SCRCMD_SECRETS_PASSED:
            gfrosty_pasteur_6F8060->field_338_secrets_passed = &pCmd->field_8_counter;
            break;
        case SCRCMD_SECRETS_FAILED:
            gfrosty_pasteur_6F8060->field_33C_secrets_failed = &pCmd->field_8_counter;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505710)
void miss2_0x11C::SCRCMD_FINISH_SCORE_505710(SCR_TWO_PARAMS* pCmd)
{
    gfrosty_pasteur_6F8060->field_310_finish_score = pCmd->field_8_s32;
}

MATCH_FUNC(0x505750)
void miss2_0x11C::SCRCMD_DECLARE_CARLIST_505750(SCR_TWO_PARAMS* pCmd)
{
    gfrosty_pasteur_6F8060->field_340_car_list = &pCmd->field_8_u32;
}

STUB_FUNC(0x505790)
s32 miss2_0x11C::DisableThread_505790(u16 a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x505b10)
void miss2_0x11C::DeallocOrDeleteItem_505B10(u16 idx)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x505ea0)
s16 miss2_0x11C::sub_505EA0(u16 idx)
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(idx);
    s16 result = pPointer->field_8_counter;
    if (result != -3 && result != -4)
    {
        result = gGame_0x40_67E008->field_38_orf1->field_2D4_scores.field_1A8_unk.sub_432080(result);
        if (result == -3 || result == -4)
        {
            pPointer->field_8_counter = result;
        }
    }
    return result;
}

MATCH_FUNC(0x505f50)
void miss2_0x11C::SCRCMD_DISPLAY_MESSAGES_505F50(SCR_TWO_PARAMS* a1)
{
    if (gDo_text_id_test_67D6D0)
    {
        sprintf(gTmpBuffer_67C598, "%d", a1->field_8_unsigned_1);
        if (!gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598))
        {
            FatalError_4A38C0(Gta2Error::MissionScriptIsReferringToTextIdThatDoesNotExist,
                              "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                              2460,
                              a1->field_8_unsigned_1); // Mission script is referring to a 'text id' that doesn't exist. ID
        }
    }
}

MATCH_FUNC(0x506010)
void miss2_0x11C::SCRCMD_DEC_DEATH_BASE_506010(SCR_FOUR_PARAMS* pCmd)
{
    if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_1)
    {
        gfrosty_pasteur_6F8060->field_356 = pCmd->field_C_unsigned_3;
    }
    else if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_2)
    {
        gfrosty_pasteur_6F8060->field_358 = pCmd->field_C_unsigned_3;
    }
    else if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_3)
    {
        gfrosty_pasteur_6F8060->field_35A = pCmd->field_C_unsigned_3;
    }
}

MATCH_FUNC(0x5060d0)
void miss2_0x11C::SCRCMD_DO_CRANE_POWERUP_5060D0(SCR_DECLARE_CRANE_POWERUP* a1)
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(a1->field_8_crane_idx);
    pPointer->field_8_crane->field_140 = a1->field_0_cmd_this;
}

MATCH_FUNC(0x506140)
void miss2_0x11C::SCRCMD_START_BASIC_KF_506140(u16* a1)
{
    for (u16 idx = 0; idx < 25; idx++)
    {
        if (gfrosty_pasteur_6F8060->field_C1E74_basic_kf[idx] == 0)
        {
            gfrosty_pasteur_6F8060->field_C1E74_basic_kf[idx] = *a1;
            break;
        }
    }
}

// https://decomp.me/scratch/fuDf2
STUB_FUNC(0x5061c0)
void miss2_0x11C::ExecOpCode_5061C0()
{
    NOT_IMPLEMENTED;
    for (u16 i = 1; i < 6000u; ++i)
    {
        SCR_CMD_HEADER* pBasePtr = gfrosty_pasteur_6F8060->GetBasePointer_512770(i);
        if (pBasePtr)
        {
            switch (pBasePtr->field_2_type)
            {
                case SCRCMD_PLAYER_PED:
                    miss2_0x11C::SCRCMD_PLAYER_PED_503A20((SCR_PLAYER_PED*)pBasePtr);
                    break;
                case SCRCMD_CAR_DECSET_2D:
                case SCRCMD_CAR_DECSET_3D:
                case SCRCMD_CAR_DECSET_2D_STR:
                case SCRCMD_CAR_DECSET_3D_STR:
                    miss2_0x11C::SCRCMD_CAR_DECSET_503BC0((SCR_CAR_DATA_DEC*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_CHAR_DECSET_2D:
                case SCRCMD_CHAR_DECSET_3D:
                    miss2_0x11C::SCRCMD_CHAR_DECSET_2D_3D_503FB0((SCR_CHAR_DATA_DEC*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_OBJ_DECSET_2D:
                case SCRCMD_OBJ_DECSET_3D:
                    miss2_0x11C::SCRCMD_OBJ_DECSET_2D_3D_503680((SCR_OBJ_DATA*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_OBJ_DECSET_2D_s32:
                case SCRCMD_OBJ_DECSET_3D_s32:
                case SCRCMD_OBJ_DECSET_2D_STR:
                case SCRCMD_OBJ_DECSET_3D_STR:
                    miss2_0x11C::SCRCMD_OBJ_DECSET_5038D0((SCR_OBJ_DATA*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_MAP_ZONE_SET:
                    miss2_0x11C::SCRCMD_MAP_ZONE_SET_504150((SCR_MAP_ZONE_SET*)pBasePtr);
                    break;
                case SCRCMD_ARROW_DEC:
                    miss2_0x11C::SCRCMD_ARROW_DEC_5041B0((SCR_TWO_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_CRANE_BASIC_DEC:
                case SCRCMD_CRANE_TARGET_DEC:
                case SCRCMD_CRANE2TARGET_DEC:
                    miss2_0x11C::SCRCMD_CRANE_5041C0((s32)pBasePtr, (s32)pBasePtr); // TODO: correct type after matching this func
                    break;
                case SCRCMD_CONVEYOR_DECSET1:
                case SCRCMD_CONVEYOR_DECSET2:
                    miss2_0x11C::SCRCMD_CONVEYOR_DECSET1_2_5043A0((SCR_CONVEYOR*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_GENERATOR_DECSET1:
                case SCRCMD_GENERATOR_DECSET2:
                case SCRCMD_GENERATOR_DECSET3:
                case SCRCMD_GENERATOR_DECSET4:
                    miss2_0x11C::SCRCMD_GENERATOR_DECSET_504420((SCR_GENERATOR*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_DESTRUCTOR_DECSET1:
                case SCRCMD_DESTRUCTOR_DECSET2:
                    miss2_0x11C::SCRCMD_DESTRUCTOR_DECSET_504530((SCR_DESTRUCTOR*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_THREAD_DECLARE2:
                    miss2_0x11C::SCRCMD_THREAD_DECLARE2_5045D0((s32)pBasePtr,
                                                               &pBasePtr->field_0_cmd_this); // TODO: correct type after matching this func
                    break;
                case SCRCMD_THREAD_DECLARE3:
                    miss2_0x11C::SCRCMD_THREAD_DECLARE3_504660((s32)pBasePtr); // TODO: correct type after matching this func
                    break;
                case SCRCMD_THREAD_DECLARE4:
                    miss2_0x11C::SCRCMD_THREAD_DECLARE4_5047C0((s32)pBasePtr,
                                                               &pBasePtr->field_0_cmd_this); // TODO: correct type after matching this func
                    break;
                case SCRCMD_SET_GANG_INFO1:
                    miss2_0x11C::SCRCMD_SET_GANG_INFO1_504830((SCR_SET_GANG_INFO*)pBasePtr); // SCRCMD_SET_GANG_INFO1
                    break;
                case SCRCMD_SET_DOOR_INFO: // SCRCMD_SET_DOOR_INFO
                    miss2_0x11C::SCRCMD_SET_DOOR_INFO_504950((SCR_FOUR_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_CRUSHER_BASIC: // SCRCMD_CRUSHER_BASIC ??
                    miss2_0x11C::SCRCMD_CRUSHER_BASIC_5045A0((SCR_CRUSHER_BASIC*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_SET_STATION:
                case SCRCMD_SET_EMPTY_STATION:
                    miss2_0x11C::SCRCMD_SET_STATION_EMPTY_STATION_505030((SCR_SET_STATION*)pBasePtr);
                    break;
                case SCRCMD_RADIOSTATION_DEC:
                    miss2_0x11C::SCRCMD_RADIOSTATION_DEC_5051D0((SCR_RADIOSTATION_DEC*)pBasePtr);
                    break;
                case SCRCMD_SET_STATION_4:
                case SCRCMD_SET_STATION_3:
                case SCRCMD_SET_STATION_2:
                case SCRCMD_SET_STATION_1:
                    miss2_0x11C::CRCMD_SET_TRAIN_STATIONS_505210((SCR_TWO_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_CREATE_LIGHT2:
                case SCRCMD_LIGHT_DEC:
                    miss2_0x11C::CreateLight_504EE0((SCR_CREATE_LIGHT*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_DECLARE_POLICE:
                    miss2_0x11C::SCRCMD_DECLARE_POLICE_5052C0((SCR_DECLARE_POLICELEVEL*)pBasePtr);
                    break;
                case SCRCMD_SOUND_DECSET:
                    miss2_0x11C::SCRCMD_SOUND_DECSET_505340((SCR_SOUND_DECSET*)pBasePtr, (SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_FINISH_SCORE:
                    miss2_0x11C::SCRCMD_FINISH_SCORE_505710((SCR_TWO_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_TOTAL_MISSIONS:
                case SCRCMD_TOTAL_SECRETS:
                case SCRCMD_GANG_1_MISSION_TOTAL:
                case SCRCMD_GANG_2_MISSION_TOTAL:
                case SCRCMD_GANG_3_MISSION_TOTAL:
                    miss2_0x11C::SCRCMD_SET_MISSIONS_TOTAL_505430((SCR_TWO_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_PASSED_FLAG:
                case SCRCMD_1_PASSED_FLAG:
                case SCRCMD_2_PASSED_FLAG:
                case SCRCMD_3_PASSED_FLAG:
                case SCRCMD_SECRETS_PASSED:
                case SCRCMD_SECRETS_FAILED:
                    miss2_0x11C::SCRCMD_PASSED_FAILED_FLAGS_505580((SCR_TWO_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_DOOR_DECLARE_S1:
                case SCRCMD_DOOR_DECLARE_D1:
                    miss2_0x11C::SCRCMD_DOOR_DECLARE_D1_S1_504970((SCR_DOOR_DATA_DEC*)pBasePtr);
                    break;
                case SCRCMD_DOOR_DECLARE_S2:
                case SCRCMD_DOOR_DECLARE_D2:
                    miss2_0x11C::SCRCMD_DOOR_DECLARE_D2_S2_504B80((SCR_DOOR_DATA_DEC*)pBasePtr);
                    break;
                case SCRCMD_DEC_GANG_1_FLAG:
                case SCRCMD_DEC_GANG_2_FLAG:
                case SCRCMD_DEC_GANG_3_FLAG:
                case SCRCMD_DECLARE_MISSION:
                    miss2_0x11C::SCRCMD_DECLARE_MISSION_504DD0((SCR_TWO_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_CHAR_AREA_ANY_MEANS:
                case SCRCMD_THREAD_DECLARE5: // ?????
                    miss2_0x11C::sub_504710((s32)pBasePtr); // TODO: correct type after matching this func
                    break;
                case SCRCMD_DECLARE_CARLIST:
                    miss2_0x11C::SCRCMD_DECLARE_CARLIST_505750((SCR_TWO_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_DISPLAY_MESSAGE:
                case SCRCMD_DISPLAY_BRIEF:
                case SCRCMD_DISPLAY_BRIEF_NOW:
                case SCRCMD_DISPLAY_BRIEF_SOON:
                    miss2_0x11C::SCRCMD_DISPLAY_MESSAGES_505F50((SCR_TWO_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_PARKED_CAR_DECSET_2D:
                case SCRCMD_PARKED_CAR_DECSET_3D:
                case SCRCMD_PARKED_CAR_DECSET_2D_STR:
                case SCRCMD_PARKED_CAR_DECSET_3D_STR:
                    miss2_0x11C::SCRCMD_PARKED_CAR_DECSET_503F80((SCR_POINTER*)pBasePtr);
                    break;
                case SCRCMD_DEC_DEATH_BASE_1:
                case SCRCMD_DEC_DEATH_BASE_2:
                case SCRCMD_DEC_DEATH_BASE_3:
                    miss2_0x11C::SCRCMD_DEC_DEATH_BASE_506010((SCR_FOUR_PARAMS*)pBasePtr);
                    break;
                case SCRCMD_DO_CRANE_POWERUP:
                    miss2_0x11C::SCRCMD_DO_CRANE_POWERUP_5060D0((SCR_DECLARE_CRANE_POWERUP*)pBasePtr);
                    break;
                case SCRCMD_START_BASIC_KF:
                    miss2_0x11C::SCRCMD_START_BASIC_KF_506140((u16*)&pBasePtr->field_0_cmd_this);
                    break;

                default:
                    continue;
            }
        }
    }
    gfrosty_pasteur_6F8060->sub_511C60();
    miss2_0x11C::sub_511CD0();
    gGeneratorPool_14AC_67E5D0->sub_4C1CD0();
}

MATCH_FUNC(0x5069c0)
void miss2_0x11C::SCRCMD_LEVELSTART_5069C0()
{
    gRoot_sound_66B038.PlayVoice_40F090(24); //  "And remember, respect is everything!"
    field_118 = 1;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5069f0)
void miss2_0x11C::SCRCMD_LEVELEND_5069F0()
{
    field_118 = 0;
    miss2_0x11C::EndCmd_503670();
}

MATCH_FUNC(0x506a00)
void miss2_0x11C::SCRCMD_CREATE_THREAD_506A00()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_THREAD* pCmd = (SCR_THREAD*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_A_unsigned_2);

    miss2_0x11C* v4 = miss2_0x11C::sub_511960(v1->field_8_unsigned_1);
    pCmd->field_8_script_thread = v4;
    pCmd->field_C_unknown = v4->field_11A;
    v4->sub_511930(field_6, v1->field_8_unsigned_1);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x506a60)
void miss2_0x11C::SCRCMD_STOP_THREAD_506A60()
{
    SCR_THREAD* pCmd = (SCR_THREAD*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_2_type);
    miss2_0x11C* v3;
    s16 v4;

    s32 opcode_idx = pCmd->field_2_type;
    if (opcode_idx != 24)
    {
        if (opcode_idx > 210 && opcode_idx <= 214)
        {
            v3 = pCmd->field_8_script_thread;
            v4 = pCmd->field_C_unknown;

            pCmd->field_8_script_thread = NULL;
            pCmd->field_C_unknown = 0;
        }
    }
    else
    {
        v3 = pCmd->field_8_script_thread;
        v4 = pCmd->field_C_unknown;

        pCmd->field_C_unknown = 0;
        pCmd->field_8_script_thread = NULL;
    }

    if (v3->field_11A == v4)
    {
        v3->sub_503610();
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x506af0)
void miss2_0x11C::SCRCMD_IF_JUMP_506AF0()
{
    //  field_8 = 1  means the boolean opcode has returned true
    //  field_8 = 0  means the boolean opcode has returned false

    //  If it's a OR and boolean is true, or if it's a AND and boolean is false, jump
    SCR_IF_JUMP* base_pointer = (SCR_IF_JUMP*)gBasePtr_6F8070;

    if ((base_pointer->is_or == 1 && field_8) || (!base_pointer->is_or && !field_8))
    {

        JumpToCmd_503650(base_pointer->else_endif_pointer); //  Jump to the last IF_JUMP or go to ELSE section
        return;
    }

    Next_503620(gBasePtr_6F8070); // go to field_4_cmd_next
}

MATCH_FUNC(0x506b30)
void miss2_0x11C::sub_506B30() // GOSUB
{
    SCR_TWO_PARAMS* v2 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    Frismo_C* v3 = field_114->sub_5031A0();
    v3->field_0 = field_8;
    v3->field_4 = gBasePtr_6F8070->field_4_cmd_next;
    field_114->add_503160(v3);
    miss2_0x11C::JumpToCmd_503650(v2->field_8_unsigned_1);
}

MATCH_FUNC(0x506b80)
void miss2_0x11C::sub_506B80() // MISSIONEND
{
    Frismo_C* v2 = field_114->remove_503180();

    if (v2 == NULL)
    {
        miss2_0x11C::EndCmd_503670();
    }
    else
    {
        miss2_0x11C::JumpToCmd_503650(v2->field_4);
        field_8 = v2->field_0;
        field_114->sub_5031C0(v2);
    }
}

MATCH_FUNC(0x506bc0)
u8 miss2_0x11C::sub_506BC0(u32 a1)
{
    switch (a1)
    {
        case SCRCMD_S_PLUS_I:
        case SCRCMD_I_PLUS_S:
        case SCRCMD_S_PLUS_S:
        case SCRCMD_S_IS_S_PLUS_S:
        case SCRCMD_S_IS_S_PLUS_I:
            return 0;
        case SCRCMD_S_MINUS_I:
        case SCRCMD_I_MINUS_S:
        case SCRCMD_S_MINUS_S:
        case SCRCMD_S_IS_S_MINUS_I:
        case SCRCMD_S_IS_S_MINUS_S:
            return 1;
        case SCRCMD_S_LESS_I:
        case SCRCMD_S_LESS_S:
            return 4;
        case SCRCMD_S_LEQUAL_I:
        case SCRCMD_S_LEQUAL_S:
            return 5;
        case SCRCMD_S_GREATER_I:
        case SCRCMD_S_GREATER_S:
            return 2;
        case SCRCMD_S_GEQUAL_I:
        case SCRCMD_S_GEQUAL_S:
            return 3;
        case SCRCMD_S_EQUAL_I:
        case SCRCMD_S_EQUAL_S:
            return 6;
        case SCRCMD_S_IS_S_DIV_I:
        case SCRCMD_S_IS_S_DIV_S:
            return 7;
        case SCRCMD_S_IS_S_MULT_I:
        case SCRCMD_S_IS_S_MULT_S:
            return 8;
        case SCRCMD_S_IS_S_MOD_I:
        case SCRCMD_S_IS_S_MOD_S:
            return 9;
        default:
            return 0;
    }
}

STUB_FUNC(0x506d60)
void miss2_0x11C::sub_506D60()
{
    NOT_IMPLEMENTED;

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x506ed0)
void miss2_0x11C::sub_506ED0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x507110)
void miss2_0x11C::sub_507110()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x507750)
void miss2_0x11C::SCRCMD_NOT_507750()
{
    field_8 = field_8 == 0; //  just toggle the boolean value
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5078d0)
void miss2_0x11C::SCRCMD_START_EXEC_5078D0()
{
    s16 exec_flag;

    exec_flag = field_12;

    //  Only increase EXEC flag if it wasn't set before
    if (exec_flag <= 0)
    {
        field_12 = exec_flag + 1; //  increase EXEC flag
    }

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5079a0)
void miss2_0x11C::SCRCMD_STOP_EXEC_5079A0()
{
    s16 exec_flag;

    exec_flag = field_12;

    //  Only decrease EXEC flag if it was set before
    if (exec_flag >= 0)
    {
        field_12 = exec_flag - 1; //  decrease EXEC flag
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507a70)
void miss2_0x11C::SCRCMD_INCREMENT_507A70()
{
    SCR_POINTER* pCmd;

    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    ++pCmd->field_8_counter; //  Increment Counter
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507b50)
void miss2_0x11C::SCRCMD_DECREMENT_507B50()
{
    SCR_POINTER* pCmd;

    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    --pCmd->field_8_counter; //  Decrement Counter
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507ce0)
void miss2_0x11C::sub_507CE0()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_OBJ_DATA* pObjData = (SCR_OBJ_DATA*)gBasePtr_6F8070;

    switch (pObjData->field_2_type)
    {
        case 0x31:
        case 0x34:
            miss2_0x11C::SCRCMD_OBJ_DECSET_2D_3D_503680(pObjData, pPointer);
            break;
        case 0x2F:
        case 0x30:
        case 0x32:
        case 0x33:
            miss2_0x11C::SCRCMD_OBJ_DECSET_5038D0(pObjData, pPointer);
            break;
        default:
            break;
    }
    if (!field_118)
    {
        gMiss2_25C_6F805C->push_type_2_502FF0(pPointer->field_8_obj, 0);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507f80)
void miss2_0x11C::sub_507F80()
{
    SCR_CAR_DATA_DEC* v3 = (SCR_CAR_DATA_DEC*)gBasePtr_6F8070;

    //  TODO: gBasePtr_6F8070 may be an extended union of SCR_CMD_HEADER
    Fix16* z_coord = (Fix16*)&gBasePtr_6F8070[2].field_4_cmd_next;

    if (*z_coord == dword_6F7570)
    {
        Fix16 temp_z;
        *z_coord = *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z,
                                                                  *(Fix16*)&gBasePtr_6F8070[1].field_4_cmd_next,
                                                                  *(Fix16*)&gBasePtr_6F8070[2].field_0_cmd_this);
    }
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v3->field_8_car_idx);
    miss2_0x11C::SCRCMD_CAR_DECSET_503BC0((SCR_CAR_DATA_DEC*)gBasePtr_6F8070, (SCR_POINTER*)pPointer);
    if (this->field_118 == 0)
    {
        gMiss2_25C_6F805C->push_type_1_car_502F80(pPointer->field_8_car);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508220)
void miss2_0x11C::SCRCMD_MAKE_CAR_DUMMY_508220()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (pPointer->field_8_car != NULL)
    {
        if (!pPointer->field_8_car->field_54_driver)
        {
            pPointer->field_8_car->SpawnDriverPed();
        }
        pPointer->field_8_car->InitCarAIControl_440590();

        Car_BC* pCar = pPointer->field_8_car;
        pCar->field_7C_uni_num = 5;
        pCar->field_76 = 0;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508280)
void miss2_0x11C::sub_508280()
{
    SCR_MAP_SET_ZONE_INFO* pCmd = (SCR_MAP_SET_ZONE_INFO*)gBasePtr_6F8070;
    str_table_entry* pStrTable = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_8_zone_idx);

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_SET_CAR_DENSITY:
            gMap_0x370_6F6268->set_nav_unknown_f0_4DF9D0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_GOOD_CAR:
            gMap_0x370_6F6268->set_nav_unknown_f2_4DF9F0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_BAD_CAR:
            gMap_0x370_6F6268->set_nav_unknown_f4_4DFA10(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_POLICE_CAR:
            gMap_0x370_6F6268->set_nav_unknown_f6_4DFA30(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_PED_DENSITY:
            gMap_0x370_6F6268->set_nav_unknown_fA_4DFA70(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_MUGGER:
            gMap_0x370_6F6268->set_nav_unknown_fC_4DFA90(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_CARTHIEF:
            gMap_0x370_6F6268->set_nav_unknown_fE_4DFAB0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_ELVIS:
            gMap_0x370_6F6268->set_nav_unknown_f10_4DFAD0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_GANG:
            gMap_0x370_6F6268->set_nav_unknown_f12_4DFAF0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_POLICE_PED:
            gMap_0x370_6F6268->set_nav_unknown_f14_4DFB10(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_GANGCARRATIO:
            gMap_0x370_6F6268->set_nav_unknown_f8_4DFA50(pStrTable->field_2_zone_idx, pCmd->field_C_value);
        default:
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508550)
void miss2_0x11C::sub_508550() //  SCRCMD_POINT_ARROW_3D and SCRCMD_LEVEL_END_ARROW2
{
    SCR_LEVEL_END_ARROW2* v1 = (SCR_LEVEL_END_ARROW2*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[2].field_4_cmd_next);
    if (pPointer->field_8_arrow == NULL)
    {
        pPointer->field_8_arrow = gHud_2B00_706620->field_1F18.AllocArrow_5D1050();
    }

    ArrowTrace_24* pArrow_trace = &pPointer->field_8_arrow->field_18.field_18_primary_target;

    pArrow_trace->set_arrow_aim_from_pos_4767C0(v1->field_8_pos.field_0_x, v1->field_8_pos.field_4_y, v1->field_8_pos.field_8_z);

    pArrow_trace->field_10_target_type = 1;

    if (gBasePtr_6F8070->field_2_type == SCRCMD_LEVEL_END_ARROW2)
    {
        pPointer->field_8_arrow->SetArrowColour_5D0510(5);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x5086f0)
void miss2_0x11C::sub_5086F0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x508dc0)
void miss2_0x11C::SCRCMD_ARROW_COLOUR_508DC0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (pPointer->field_8_arrow == NULL)
    {
        pPointer->field_8_arrow = gHud_2B00_706620->field_1F18.AllocArrow_5D1050();
    }
    pPointer->field_8_arrow->SetArrowColour_5D0510(v1->field_A_unsigned_2);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508e80)
void miss2_0x11C::SCRCMD_REMOVE_ARROW_508E80()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Hud_Arrow_7C* pArrow = pPointer->field_8_arrow;

    if (pArrow != NULL)
    {
        pArrow->field_18.field_18_primary_target.field_10_target_type = 0;
        pArrow->field_18.field_3C_secondary_target.field_10_target_type = 0;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508f00)
void miss2_0x11C::SCRCMD_CHECK_CAR_DAMAGE_508F00()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Car_BC* pCar = pPointer->field_8_car;
    if (pCar->field_74_damage >= 320 * v1->field_A_signed_2)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509030)
void miss2_0x11C::SCRCMD_CHECK_HEALTH_509030()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Ped* v3 = pPointer->field_8_char;
    s16 health_param = v1->field_A_signed_2;

    if (v3 != NULL && v3->field_216_health >= health_param)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x509180)
void miss2_0x11C::SCRCMD_STORE_CAR_INFO_509180()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5093c0)
void miss2_0x11C::sub_5093C0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    u8 type = ((SCR_TWO_PARAMS*)gBasePtr_6F8070)->field_A_unsigned_2;
    if (type == 1)
    {
        gRoot_sound_66B038.PlayVoice_40F090(22);
    }
    else if (type == 2)
    {
        gRoot_sound_66B038.PlayVoice_40F090(23);
    }

    if (v1->field_8_unsigned_1 == 5501)
    {
        gRoot_sound_66B038.PlayVoice_40F090(33);
    }
    else if (v1->field_8_unsigned_1 == 5502)
    {
        gRoot_sound_66B038.PlayVoice_40F090(34);
    }
    else if (v1->field_8_unsigned_1 == 5503)
    {
        gRoot_sound_66B038.PlayVoice_40F090(35);
    }
    else if (v1->field_8_unsigned_1 == 5504)
    {
        gRoot_sound_66B038.PlayVoice_40F090(36);
    }
    else if (v1->field_8_unsigned_1 == 5505)
    {
        gRoot_sound_66B038.PlayVoice_40F090(37);
    }
    else if (v1->field_8_unsigned_1 == 5506)
    {
        gRoot_sound_66B038.PlayVoice_40F090(38);
    }
    else if (v1->field_8_unsigned_1 == 5507)
    {
        gRoot_sound_66B038.PlayVoice_40F090(39);
    }
    else if (v1->field_8_unsigned_1 == 5508)
    {
        gRoot_sound_66B038.PlayVoice_40F090(40);
    }
    else if (v1->field_8_unsigned_1 == 5509)
    {
        gRoot_sound_66B038.PlayVoice_40F090(41);
    }
    else if (v1->field_8_unsigned_1 == 5510)
    {
        gRoot_sound_66B038.PlayVoice_40F090(42);
    }
    else if (v1->field_8_unsigned_1 == 5000)
    {
        gRoot_sound_66B038.PlayVoice_40F090(43);
    }
    else if (v1->field_8_unsigned_1 == 5015)
    {
        gRoot_sound_66B038.PlayVoice_40F090(44);
    }
    else if (v1->field_8_unsigned_1 == 5031)
    {
        gRoot_sound_66B038.PlayVoice_40F090(45);
    }
    else if (v1->field_8_unsigned_1 == 5032)
    {
        gRoot_sound_66B038.PlayVoice_40F090(46);
    }
    else if (v1->field_8_unsigned_1 == 5050)
    {
        gRoot_sound_66B038.PlayVoice_40F090(47);
    }
    else if (v1->field_8_unsigned_1 == 5051)
    {
        gRoot_sound_66B038.PlayVoice_40F090(48);
    }
    else if (v1->field_8_unsigned_1 == 5052)
    {
        gRoot_sound_66B038.PlayVoice_40F090(49);
    }
    else if (v1->field_8_unsigned_1 == 5053)
    {
        gRoot_sound_66B038.PlayVoice_40F090(50);
    }
    else if (v1->field_8_unsigned_1 == 5054)
    {
        gRoot_sound_66B038.PlayVoice_40F090(51);
    }
    else if (v1->field_8_unsigned_1 == 5055)
    {
        gRoot_sound_66B038.PlayVoice_40F090(52);
    }
    else if (v1->field_8_unsigned_1 == 5056)
    {
        gRoot_sound_66B038.PlayVoice_40F090(53);
    }
    else if (v1->field_8_unsigned_1 == 5057)
    {
        gRoot_sound_66B038.PlayVoice_40F090(54);
    }
    else if (v1->field_8_unsigned_1 == 5058)
    {
        gRoot_sound_66B038.PlayVoice_40F090(55);
    }
    else if (v1->field_8_unsigned_1 == 5059)
    {
        gRoot_sound_66B038.PlayVoice_40F090(56);
    }
    else if (v1->field_8_unsigned_1 == 5060)
    {
        gRoot_sound_66B038.PlayVoice_40F090(57);
    }
    else if (v1->field_8_unsigned_1 == 5061)
    {
        gRoot_sound_66B038.PlayVoice_40F090(58);
    }
    else if (v1->field_8_unsigned_1 == 5062)
    {
        gRoot_sound_66B038.PlayVoice_40F090(59);
    }
    else if (v1->field_8_unsigned_1 == 5063)
    {
        gRoot_sound_66B038.PlayVoice_40F090(60);
    }

    sprintf(gTmpBuffer_67C598, "%d", v1->field_8_unsigned_1);

    switch (v1->field_2_type)
    {
        case SCRCMD_DISPLAY_MESSAGE:
            gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598), 3);
            break;
        case SCRCMD_DISPLAY_BRIEF:
            gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
            break;
        case SCRCMD_DISPLAY_BRIEF_NOW:
            gHud_2B00_706620->field_DC.sub_5D4400(3, gTmpBuffer_67C598);
            break;
        case SCRCMD_DISPLAY_BRIEF_SOON:
            gHud_2B00_706620->field_DC.sub_5D4400(2, gTmpBuffer_67C598);
            break;
        default:
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5096e0)
void miss2_0x11C::SCRCMD_DISPLAY_TIMER_5096E0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    pPointer->field_8_index = gHud_2B00_706620->field_620.CreateTimer_5D31F0(v1->field_A_signed_2);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509730)
void miss2_0x11C::SCR_CMD_CREATE_CHAR_509730()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::SCRCMD_CHAR_DECSET_2D_3D_503FB0((SCR_CHAR_DATA_DEC*)gBasePtr_6F8070, pPointer);
    if (!field_118)
    {
        gMiss2_25C_6F805C->push_type_3_ped_502FB0(pPointer->field_8_char);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509790)
void miss2_0x11C::sub_509790()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    pPointer->field_8_char->field_240_occupation = v1->field_C_u32;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5097d0)
void miss2_0x11C::sub_5097D0()
{
    SCR_POINTER* BasePointer_512770;

    BasePointer_512770 =
        (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this); //  TODO: fix gBasePtr_6F8070
    miss2_0x11C::sub_504110((SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070, BasePointer_512770);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509810)
void miss2_0x11C::sub_509810()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        switch (pPed->field_225_objective_status)
        {
            case objective_status::failed_2:
            case objective_status::not_finished_0:
                field_8 = false;
                break;
            case objective_status::passed_1:
                field_8 = true;
                break;
            default:
                break;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509880)
void miss2_0x11C::sub_509880()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        switch (pPed->field_225_objective_status)
        {
            case objective_status::failed_2:
                field_8 = true;
                break;
            default:
                field_8 = false;
                break;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5098e0)
void miss2_0x11C::sub_5098E0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_FOUR_PARAMS* v2 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (miss2_0x11C::sub_503410(pPointer->field_2_type) == 1)
    {
        if (gBasePtr_6F8070->field_2_type == SCRCMD_GIVE_WEAPON1)
        {
            pPointer->field_8_char->AddWeaponWithAmmo_45DD30(v1->field_A_signed_2, 100);
        }
        else
        {
            pPointer->field_8_char->AddWeaponWithAmmo_45DD30(v2->field_A_signed_2, v2->field_C_signed_3);
        }
    }
    else
    {
        // Always 50 ammo for cars
        gWeapon_8_707018->allocate_5E3D50(v1->field_A_signed_2, 50, pPointer->field_8_car);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509990)
void miss2_0x11C::sub_509990()
{
    SCR_IS_CAR_IN_BLOCK* pCmd = (SCR_IS_CAR_IN_BLOCK*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (pCmd->field_C_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        pCmd->field_C_pos.field_8_z =
            *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y);
    }

    Car_BC* pCar = pPointer->field_8_car;

    if (pCar != NULL && pCar->IsWithinBlock(&pCmd->field_C_pos))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x509a70)
void miss2_0x11C::SCRCMD_CAR_IN_AREA_509A70()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x509bb0)
void miss2_0x11C::SCRCMD_HAS_CHAR_DIED_509BB0()
{
    SCR_POINTER* v2;

    v2 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (v2->field_8_char && v2->field_8_char->field_278_ped_state_1 == ped_state_1::dead_9)
    {
        field_8 = 1;
    }
    else
    {
        field_8 = 0;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509c10)
void miss2_0x11C::sub_509C10()
{
    SCR_TWO_PARAMS* pCmd = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pParam1 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pParam2 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2);

    Ped* pPed = pParam1->field_8_char;

    Car_BC* pCarTarget;
    Car_BC* pCarCurrent;

    if (pPed != NULL && (pCarTarget = pParam2->field_8_car) != NULL && (pCarCurrent = pPed->field_16C_car) != NULL &&
        pCarCurrent == pCarTarget)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509c90)
void miss2_0x11C::sub_509C90()
{
    SCR_TWO_PARAMS* pCmd = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pParam1 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Ped* pPed = pParam1->field_8_char;
    Car_BC* pCarCurrent;

    if (pPed != NULL && (pCarCurrent = pPed->field_16C_car) != NULL && pCarCurrent->field_84_car_info_idx == pCmd->field_A_signed_2)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d00)
void miss2_0x11C::sub_509D00()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL && pPed->field_16C_car != NULL)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d60)
void miss2_0x11C::sub_509D60()
{
    miss2_0x11C::DeallocOrDeleteItem_505B10(gBasePtr_6F8070[1].field_0_cmd_this); //  delete item
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d90)
void miss2_0x11C::sub_509D90()
{
    SCR_FOUR_PARAMS* pCmd = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        Player* pPlayer = pPed->field_15C_player;
        if (pPlayer != NULL)
        {
            if (gBasePtr_6F8070->field_2_type == SCRCMD_ADD_SCORE1)
            {
                pPlayer->field_2D4_scores.AddCash_592620(pCmd->field_C_s32 * pPlayer->field_6BC_multpliers.field_0);
            }
            else
            {
                pPlayer->field_2D4_scores.AddCash_592620(pCmd->field_C_s32); //  no multiplier
            }
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509e00)
void miss2_0x11C::SCRCMD_ADD_SCORE2_509E00()
{
    SCR_FOUR_PARAMS* pCmd = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_C_s32);

    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        Player* pPlayer = pPed->field_15C_player;
        if (pPlayer != NULL)
        {
            pPlayer->field_2D4_scores.AddCash_592620(pCounter->field_8_counter * pPlayer->field_6BC_multpliers.field_0);
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509e70)
void miss2_0x11C::sub_509E70()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    Fix16 temp;

    if (pPed != NULL && (temp = pPed->field_168_game_object->field_38_velocity, temp == dword_6F77C0))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509ed0)
void miss2_0x11C::sub_509ED0()
{
    SCR_EXPLODE* pCmd = (SCR_EXPLODE*)gBasePtr_6F8070;
    switch (gBasePtr_6F8070->field_2_type)
    {
        case 406:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.field_0_x,
                                                      pCmd->field_8_pos.field_4_y,
                                                      pCmd->field_8_pos.field_8_z,
                                                      dword_6F804C,
                                                      32,
                                                      0);
            break;
        case 404:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.field_0_x,
                                                      pCmd->field_8_pos.field_4_y,
                                                      pCmd->field_8_pos.field_8_z,
                                                      dword_6F804C,
                                                      18,
                                                      0);
            break;
        case 142:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.field_0_x,
                                                      pCmd->field_8_pos.field_4_y,
                                                      pCmd->field_8_pos.field_8_z,
                                                      dword_6F804C,
                                                      19,
                                                      0);
            break;
        case 399:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.field_0_x,
                                                      pCmd->field_8_pos.field_4_y,
                                                      pCmd->field_8_pos.field_8_z,
                                                      dword_6F804C,
                                                      20,
                                                      0);
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509f60)
void miss2_0x11C::SCRCMD_EXPLODE_BUILDING_509F60()
{
    SCR_EXPLODE_WALL* pCmd = (SCR_EXPLODE_WALL*)gBasePtr_6F8070;
    switch (pCmd->field_14_face)
    {
        case 1:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.field_0_x,
                                                      pCmd->field_8_pos.field_4_y,
                                                      pCmd->field_8_pos.field_8_z,
                                                      dword_6F804C,
                                                      23,
                                                      0);
            break;
        case 2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.field_0_x,
                                                      pCmd->field_8_pos.field_4_y,
                                                      pCmd->field_8_pos.field_8_z,
                                                      dword_6F804C,
                                                      22,
                                                      0);
            break;
        case 3:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.field_0_x,
                                                      pCmd->field_8_pos.field_4_y,
                                                      pCmd->field_8_pos.field_8_z,
                                                      dword_6F804C,
                                                      24,
                                                      0);
            break;
        case 4:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.field_0_x,
                                                      pCmd->field_8_pos.field_4_y,
                                                      pCmd->field_8_pos.field_8_z,
                                                      dword_6F804C,
                                                      25,
                                                      0);
            break;
        default:
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x509fd0)
void miss2_0x11C::sub_509FD0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50a200)
void miss2_0x11C::sub_50A200()
{
    SCR_CHAR_OBJECTIVE* v1 = (SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::sub_504110((SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070, pPointer);

    SCR_POINTER* pParam;
    switch (v1->field_A_objective)
    {
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 0x10:
        case 0x13:
        case 0x14:
        case 0x17:
            pParam = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_C_second_item_idx);
            pPointer->field_8_char->field_148_objective_target_ped = pParam->field_8_char;
            break;
        case 0x23:
            pParam = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_C_second_item_idx);
            pPointer->field_8_char->field_150_target_objective_car = pParam->field_8_car;
            if (v1->field_E_variant == 1)
            {
                pPointer->field_8_char->field_248_enter_car_as_passenger = true;
            }
            else
            {
                pPointer->field_8_char->field_248_enter_car_as_passenger = false;
            }
            break;
        case 0x15:
        case 0x1B:
        case 0x24:
        case 0x37:
        case 0x3B:
            pParam = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_C_second_item_idx);
            pPointer->field_8_char->field_150_target_objective_car = pParam->field_8_car;
            break;
        case 0x39:
        case 0x3A:
            pParam = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_C_second_item_idx);
            pPointer->field_8_char->field_1A0_objective_target_object = pParam->field_8_obj;
            break;
        default:
            break;
    }

    Ped* v4 = pPointer->field_8_char;
    v4->field_21C &= ~0x400;

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a3e0)
void miss2_0x11C::sub_50A3E0()
{
    SCR_CHAR_OBJ3* v1;
    SCR_POINTER* pCmd;

    v1 = (SCR_CHAR_OBJ3*)gBasePtr_6F8070;
    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::sub_504110((SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070, pCmd);

    if (pCmd->field_8_char)
    {
        (pCmd->field_8_char)->field_1DC_objective_target_x = v1->field_C_pos.field_0_x.mValue; //  TODO: May be Fix16 in cool_nash
        (pCmd->field_8_char)->field_1E0_objective_target_y = v1->field_C_pos.field_4_y.mValue;
        (pCmd->field_8_char)->field_1E4_objective_target_z = v1->field_C_pos.field_8_z.mValue;

        (pCmd->field_8_char)->field_21C &= ~0x400u; // TODO: Maybe BitSet32
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a460)
void miss2_0x11C::sub_50A460()
{
    SCR_CHAR_OBJECTIVE* v1;
    SCR_POINTER* pCmd;

    v1 = (SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070;
    pCmd =
        (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this); //  TODO: fix gBasePtr_6F8070 type
    miss2_0x11C::sub_504110((SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070, pCmd);

    SCR_POINTER* v4 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_C_car_idx);

    if (pCmd->field_8_char)
    {
        (pCmd->field_8_char)->field_150_target_objective_car = v4->field_8_car;

        Ang16 CmdRotation;
        CmdRotation.rValue = v1->field_E_rotation;

        Fix16 fix_1;
        fix_1.mValue = CmdRotation.FromUnsignedToFloat();
        Fix16 fix_2;
        fix_2.mValue = word_6F8044.ToFloat();
        Fix16 fix_3;
        fix_3.mValue = fix_2.MultiplyInt64(fix_1);

        Ang16 rotation;
        rotation.rValue = fix_3.ToInt();
        rotation.Normalize();

        (pCmd->field_8_char)->field_132 = rotation.rValue;
        (pCmd->field_8_char)->field_1FC = v1->field_12_offset;
        (pCmd->field_8_char)->field_21C &= ~0x400u; // TODO: Maybe BitSet32
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a570)
void miss2_0x11C::SCRCMD_CHANGE_CAR_REMAP_50A570()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (v1->field_A_unsigned_2 != 0xFFu) //  field_A_unsigned_2 = remap
    {
        pPointer->field_8_car->SetCarRemap(v1->field_A_unsigned_2);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a5b0)
void miss2_0x11C::SCRCMD_CHANGE_CHAR_REMAP_50A5B0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (pPointer->field_8_char)
    {
        Char_B4* pChar_b4 = pPointer->field_8_char->field_168_game_object;
        u8 remap = v1->field_A_unsigned_2;
        pChar_b4->field_5_remap = remap;
        if (pChar_b4->field_5_remap != 0xFFu)
        {
            pChar_b4->field_80_sprite_ptr->SetRemap(remap);
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a610)
void miss2_0x11C::SCRCMD_CHECK_CAR_MODEL_50A610()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar = pPointer->field_8_car;
    s16 car_model_index = v1->field_A_signed_2;

    if (pCar != NULL && (s8)pCar->field_84_car_info_idx == car_model_index)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a670)
void miss2_0x11C::SCRCMD_CHECK_CAR_REMAP_50A670()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar = pPointer->field_8_car;
    s16 remap = v1->field_A_signed_2;

    if (pCar != NULL && pCar->field_88 != 6 && (u16)pCar->field_50_car_sprite->field_24_remap == remap)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a6e0)
void miss2_0x11C::SCRCMD_CHECK_CAR_BOTH_50A6E0()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar = pPointer->field_8_car;
    s16 car_model_idx = v1->field_A_signed_2;
    s16 remap = v1->field_C_signed_3;

    if (pCar != NULL && pCar->field_88 != 6 && (s8)pCar->field_84_car_info_idx == car_model_idx &&
        (u16)pCar->field_50_car_sprite->field_24_remap == remap)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50a760)
void miss2_0x11C::sub_50A760()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50a940)
void miss2_0x11C::SCRCMD_DELAY_HERE_50A940()
{
    SCR_CMD_HEADER* v2 = gBasePtr_6F8070 + 1; //  TODO: remove this hack

    if (gBasePtr_6F8070[1].field_0_cmd_this == -1)
    {
        v2->field_0_cmd_this = gBasePtr_6F8070[1].field_2_type;
    }
    else
    {
        v2->field_0_cmd_this = gBasePtr_6F8070[1].field_0_cmd_this - 1;
        if (!v2->field_0_cmd_this)
        {
            v2->field_0_cmd_this = -1;
            miss2_0x11C::Next_503620(gBasePtr_6F8070);
        }
    }
}

MATCH_FUNC(0x50a980)
void miss2_0x11C::sub_50A980() //  DELAY
{
    s16 param_1 = gBasePtr_6F8070[1].field_0_cmd_this;
    SCR_CMD_HEADER* v2 = gBasePtr_6F8070 + 1; //  TODO: remove this hack

    if (param_1 == -1)
    {
        v2->field_0_cmd_this = gBasePtr_6F8070[1].field_2_type;
        field_8 = true;
    }
    else
    {
        v2->field_0_cmd_this = param_1 - 1;
        if (!v2->field_0_cmd_this)
        {
            v2->field_0_cmd_this = -1;
            field_8 = false;
        }
        else
        {
            field_8 = true;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50a9e0)
void miss2_0x11C::EnableThread_50A9E0(u16 idx)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50abc0)
void miss2_0x11C::SCRCMD_DISABLE_THREAD_50ABC0()
{
    miss2_0x11C::DisableThread_505790(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50abf0)
void miss2_0x11C::SCRCMD_ENABLE_THREAD_50ABF0()
{
    miss2_0x11C::EnableThread_50A9E0(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ac20)
void miss2_0x11C::SCRCMD_SET_GANG_KILL_REACTION_50AC20() // SET_GANG_KILL_REACTION
{
    str_table_entry* gang_1_str;

    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    gang_1_str = gfrosty_pasteur_6F8060->FindStringById_503080(gBasePtr_6F8070[1].field_0_cmd_this);
    if (gang_1_str == NULL)
    {
        FatalError_4A38C0(Gta2Error::UnableToFindGangMatchingNameInSetGangRespect,
                          "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                          5217,
                          (u16)gBasePtr_6F8070->field_0_cmd_this);
    }
    Gang_144* pGang_1 = gGangPool_CA8_67E274->gang_by_name_4BF100(gang_1_str->get_name());
    str_table_entry* gang_2_str = gfrosty_pasteur_6F8060->FindStringById_503080(v1->field_C_unsigned_3);
    if (gang_2_str == NULL)
    {
        FatalError_4A38C0(Gta2Error::UnableToFindGangMatchingNameInSetGangRespect2,
                          "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                          5223,
                          (u16)gBasePtr_6F8070->field_0_cmd_this);
    }
    Gang_144* pGang_2 = gGangPool_CA8_67E274->gang_by_name_4BF100(gang_2_str->get_name());
    pGang_1->SetGangKillReaction_4BEF50(pGang_2->field_1_gang_idx, v1->field_A_unsigned_2);
    pGang_1->field_111 = 1;
    pGang_2->field_111 = 1;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50acf0)
void miss2_0x11C::sub_50ACF0()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;

    str_table_entry* StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(gBasePtr_6F8070[1].field_0_cmd_this);

    char* gang_zone_name = (char*)&StringById_503080[1];
    Gang_144* v4 = gGangPool_CA8_67E274->gang_by_name_4BF100(gang_zone_name);
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_C_unsigned_3);

    // v1->field_A_signed_2 = respect parameter

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_SET_CHAR_RESPECT:

            v4->SetRespect_4BEE30(pPointer->field_8_char->field_15C_player->field_2E_idx, 20 * ((u8)v1->field_A_signed_2));
            v4->field_111 = 1;
            break;
        case SCRCMD_CHANGE_RESPECT:
            if (v1->field_A_signed_2 > 0)
            {
                v4->IncrementRespect_4BEE50(pPointer->field_8_char->field_15C_player->field_2E_idx, 20 * (v1->field_A_signed_2));
            }
            else
            {
                v4->DecrementRespect_4BEEA0(pPointer->field_8_char->field_15C_player->field_2E_idx, 20 * abs(v1->field_A_signed_2));
            }
            break;
        case SCRCMD_CHANGE_GANG_RESP:
            v4->ChangeRespectAndUpdate_4BF000(pPointer->field_8_char->field_15C_player->field_2E_idx, 20 * ((u8)v1->field_A_signed_2));
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50aef0)
void miss2_0x11C::sub_50AEF0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50b0e0)
void miss2_0x11C::sub_50B0E0()
{
    SCR_ADD_PATROL_POINT* pCmd = (SCR_ADD_PATROL_POINT*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;
    if (pPed)
    {
        if (pPed->field_258_objective != objectives_enum::patrol_on_foot_42)
        {
            pPed->SetObjective(objectives_enum::patrol_on_foot_42, 9999);
        }
        pPointer->field_8_char->PushPatrolPoint_4702A0(pCmd->field_C_pos.field_0_x.ToInt(),
                                                       pCmd->field_C_pos.field_4_y.ToInt(),
                                                       pCmd->field_C_pos.field_8_z.ToInt());
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b150)
void miss2_0x11C::SCRCMD_SET_AMBIENT_50B150()
{
    SCR_SET_AMBIENT* pCmd = (SCR_SET_AMBIENT*)gBasePtr_6F8070;
    gpMapRenderer_6F66E4->sub_4E9D50(pCmd->field_8_level, pCmd->field_C_time);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b180)
void miss2_0x11C::sub_50B180()
{
    SCR_ANSWER_PHONE* pCmd = (SCR_ANSWER_PHONE*)gBasePtr_6F8070;
    SCR_POINTER* pPtrPed = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_C_ped_idx);
    SCR_POINTER* pPtrObj = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_8_obj_idx);
    Object_2C* pPhoneObj = pPtrObj->field_8_obj;
    if (pPhoneObj && pPtrPed->field_8_char)
    {
        pPhoneObj->sub_5291D0();
        gfrosty_pasteur_6F8060->sub_5129B0(pPtrPed->field_8_char->field_200_id,
                                           pPtrObj->field_8_obj->field_14_id,
                                           gBasePtr_6F8070->field_0_cmd_this);
        field_8 = false;
        pCmd->field_12 = 0;
        if (pCmd->field_10 > 0)
        {
            field_E = pCmd->field_10;
        }
        else
        {
            field_E = -1;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b230)
void miss2_0x11C::SCRCMD_CHECK_PHONE_50B230()
{
    SCR_POINTER* pPtr = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    thread_C* pHeader = gfrosty_pasteur_6F8060->sub_512AD0(pPtr->field_8_obj->field_14_id);
    SCR_ANSWER_PHONE* pThreadPtr = (SCR_ANSWER_PHONE*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pHeader->field_8_cmd_line);
    if (pThreadPtr->field_12 == 1)
    {
        gfrosty_pasteur_6F8060->sub_512A70(pHeader->field_0_unk, pHeader->field_4_obj_f14);
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

// https://decomp.me/scratch/Fu170 matches on decompme, wrong instr size?
STUB_FUNC(0x50b2c0)
void miss2_0x11C::SCRCMD_CHECK_PHONETIMER_50B2C0()
{
    SCR_POINTER* pPtr = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    thread_C* pHeader =
        gfrosty_pasteur_6F8060->sub_512AD0(pPtr->field_8_obj->field_14_id);
    if (pHeader)
    {
        SCR_ANSWER_PHONE* pAnswer = (SCR_ANSWER_PHONE*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pHeader->field_8_cmd_line);
        if (field_E != -1)
        {
            if (field_E == 0 && pAnswer->field_12 == 0)
            {
                field_8 = true;
                pPtr->field_8_obj->sub_5291D0();
            }
            else
            {
                --field_E;
                field_8 = false;
            }
        }
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b360)
void miss2_0x11C::SCRCMD_STOP_PHONE_RING_50B360()
{
    SCR_POINTER* pPtr = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Object_2C* pObj = pPtr->field_8_obj;
    if (pObj)
    {
        thread_C* pHeader = gfrosty_pasteur_6F8060->sub_512AD0(pObj->field_14_id);
        if (pHeader)
        {
            gfrosty_pasteur_6F8060->sub_512A70(pHeader->field_0_unk, pHeader->field_4_obj_f14);
        }
        pPtr->field_8_obj->sub_5291E0(163);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b3d0)
void miss2_0x11C::sub_50B3D0()
{
    SCR_POINTER* pPtr = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* v2 = pPtr->field_8_char;
    BitSet32 flag = v2->field_21C;
    if (flag.check_bit(11) && v2->field_20e == 0 && v2->field_170_selected_weapon)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b440)
void miss2_0x11C::SCRCMD_CAR_DRIVE_AWAY_50B440()
{
    SCR_POINTER* pPtr = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    pPtr->field_8_car->InitCarAIControl_440590();
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b470)
void miss2_0x11C::SCRCMD_GET_CAR_FROM_CRANE_50B470()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_2_type);

    SCR_POINTER* pPointer_2 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_8_unsigned_1);
    Crane_15C* pCrane = pPointer->field_8_crane;

    if (pCrane != NULL)
    {
        pPointer_2->field_8_car = pCrane->GetCarFromCrane_480DA0();
        if (pPointer_2->field_8_car)
        {
            field_8 = true;
        }
        else
        {
            field_8 = false;
        }
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b4f0)
void miss2_0x11C::sub_50B4F0()
{
    SCR_CHAR_ENTER_CAR* pCmd = (SCR_CHAR_ENTER_CAR*)gBasePtr_6F8070;
    SCR_POINTER* pPtrPed = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pPtrCar = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_car_idx);

    if (pPtrPed->field_8_char)
    {
        if (gBasePtr_6F8070->field_2_type != SCRCMD_CHAR_TO_BACKDOOR)
        {
            pPtrPed->field_8_char->sub_463830(0, 9999);
            pPtrPed->field_8_char->SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
            pPtrPed->field_8_char->field_150_target_objective_car = pPtrCar->field_8_car;
            pPtrPed->field_8_char->field_248_enter_car_as_passenger = false;
        }
        else
        {
            pPtrPed->field_8_char->EnterCarAsDriver(pPtrCar->field_8_car);
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b5a0)
void miss2_0x11C::sub_50B5A0() // SCRCMD_DRIVER_OUT_CAR
{
    SCR_POINTER* pPtr = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (pPtr->field_8_char)
    {
        pPtr->field_8_char->sub_463830(0, 9999);
        pPtr->field_8_char->SetObjective(objectives_enum::leave_car_36, 9999);
        pPtr->field_8_char->field_150_target_objective_car = pPtr->field_8_char->field_16C_car;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b600)
void miss2_0x11C::sub_50B600() // SCRCMD_GIVE_DRIVER_BRAKE
{
    SCR_POINTER* pPtr = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (pPtr->field_8_car)
    {
        if (pPtr->field_8_car->field_54_driver == NULL)
        {
            pPtr->field_8_car->SpawnDriverPed();
        }
        pPtr->field_8_car->InitCarAIControl_440590();
        pPtr->field_8_car->field_A6 |= 0x20;
        pPtr->field_8_car->sub_421560(5);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b670)
void miss2_0x11C::SCRCMD_SET_GANG_INFO1_50B670()
{
    // mid-game change
    miss2_0x11C::SCRCMD_SET_GANG_INFO1_504830((SCR_SET_GANG_INFO*)gBasePtr_6F8070);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b690)
void miss2_0x11C::sub_50B690() // SCRCMD_CLEAR_TIMERS
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (pPointer->field_8_index != 0xFFFF)
    {
        gHud_2B00_706620->field_620.ClearPager_5D3280(pPointer->field_8_index);
    }
    pPointer->field_8_index = 0;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b6f0)
void miss2_0x11C::SCRCMD_CHECK_SCORE_50B6F0()
{
    SCR_CHECK_SCORE_GREATER* pCmd = (SCR_CHECK_SCORE_GREATER*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        Player* pPlayer = pPed->field_15C_player;
        if (pPlayer != NULL && pPlayer->field_2D4_scores.GetScore_592370() > pCmd->field_C_target_score)
        {
            field_8 = true;
        }
        else
        {
            field_8 = false;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b760)
void miss2_0x11C::SCRCMD_GET_SCORE_50B760()
{
    SCR_GET_SCORE* pCmd = (SCR_GET_SCORE*)gBasePtr_6F8070;
    SCR_POINTER* pPtrPed = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pPtrCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_counter_idx);

    if (pPtrPed->field_8_char)
    {
        Player* pPlayer = pPtrPed->field_8_char->field_15C_player;
        if (pPlayer)
        {
            pPtrCounter->field_8_counter = (u16)pPlayer->field_2D4_scores.GetScore_592370();
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b7d0)
void miss2_0x11C::SCRCMD_IS_CHAR_IN_GANG_50B7D0()
{
    SCR_IS_CHAR_IN_GANG_ZONE* pCmd = (SCR_IS_CHAR_IN_GANG_ZONE*)gBasePtr_6F8070;
    SCR_POINTER* BasePointer_512770 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    str_table_entry* StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_A_gang_idx);
    const char_type* GangName = (char*)&StringById_503080[1];
    Gang_144* pGang = gGangPool_CA8_67E274->gang_by_name_4BF100(GangName);
    Ped* pPed = BasePointer_512770->field_8_char;
    Player* pPlayer = pPed->field_15C_player;
    if (pPlayer)
    {
        if (pPlayer->field_34_gang_curr_location == pGang)
        {
            field_8 = true;
        }
        else
        {
            field_8 = false;
        }
    }
    else
    {
        gmp_map_zone* pZone = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(pPed->get_cam_x().ToInt(), pPed->get_cam_y().ToInt(), Gang_14);
        if (pZone && !_strnicmp(pZone->field_6_name, pGang->field_2_name, pZone->field_5_name_length))
        {
            field_8 = true;
        }
        else
        {
            field_8 = false;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50b8b0)
void miss2_0x11C::sub_50B8B0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50b910)
void miss2_0x11C::SCRCMD_IS_CHAR_FIRING_AREA_50B910()
{
    SCR_ONEVAR_RECT* v1 = (SCR_ONEVAR_RECT*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Ped* pPed = pPointer->field_8_char;

    BitSet32 flag = pPointer->field_8_char->field_21C;

    if (flag.check_bit(11) && pPed->IsWithinArea(&v1->field_C_rect))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b9c0)
void miss2_0x11C::SCRCMD_GET_PASSENGER_NUM_50B9C0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar = pPointer->field_8_car;

    if (pCar != NULL && pCar->field_4_passengers_list.GetPedsCount_4716B0() >= v1->field_A_signed_2)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50ba30)
void miss2_0x11C::sub_50BA30()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50ba70)
void miss2_0x11C::sub_50BA70()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50bad0)
void miss2_0x11C::SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0()
{
    SCR_ONEVAR_RECT* v1 = (SCR_ONEVAR_RECT*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Car_BC* pCar = pPointer->field_8_car;

    if ((pCar->field_74_damage >= 32000 || pCar->field_88 == 5) && pCar->IsWithinArea(&v1->field_C_rect))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50bb80)
void miss2_0x11C::sub_50BB80()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50bbd0)
void miss2_0x11C::sub_50BBD0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50bc60)
void miss2_0x11C::SCRCMD_CHECK_NUM_ALIVE_50BC60()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50bcd0)
void miss2_0x11C::SCRCMD_SET_MIN_ALIVE_50BCD0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50bd10)
void miss2_0x11C::sub_50BD10()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50bdc0)
void miss2_0x11C::sub_50BDC0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50be00)
void miss2_0x11C::SCRCMD_HAS_CAR_WEAPON_50BE00()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    s16 weapon_idx = v1->field_A_signed_2;

    if (gWeapon_8_707018->find_5E3D20(pPointer->field_8_car, weapon_idx))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50be70)
void miss2_0x11C::SCRCMD_IS_CHAR_HORN_50BE70()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;
    Car_BC* pCar = pPed->field_16C_car;

    if (pCar != NULL && pCar->field_A7_horn > 0 && pCar->field_A7_horn <= 0xF8u)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bed0)
void miss2_0x11C::SCRCMD_CHECK_MAX_PASS_50BED0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Car_BC* v3 = pPointer->field_8_char->field_16C_car;

    if (v3 && (u8)v3->GetPassengersCount_440570() >= v1->field_A_signed_2)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50bf40)
void miss2_0x11C::SCRCMD_IS_CHAR_IN_ZONE_50BF40()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50c040)
void miss2_0x11C::SCRCMD_SET_PHONE_DEAD_50C040()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50c0e0)
void miss2_0x11C::SCRCMD_IS_TRAILER_ATT_50C0E0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pParam1 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pParam2 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_A_unsigned_2);

    Car_BC* v5;

    if (pParam2 == pParam1)
    {
        v5 = pParam1->field_8_car;

        if (v5->is_on_trailer_421720() || pParam1->field_8_car->is_trailer_cab_41E460())
        {
            field_8 = true;
        }
        else
        {
            field_8 = false;
        }
    }
    else
    {
        v5 = pParam2->field_8_car;

        if (pParam1->field_8_car->HasOtherCarOnTrailer_475E60(v5))
        {
            field_8 = true;
        }
        else
        {
            field_8 = false;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c1b0)
void miss2_0x11C::SCRCMD_IS_CAR_ON_TRAIL_50C1B0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pCarPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pTrailerPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_A_unsigned_2);

    //Sprite* v5 = pTrailerPointer->field_8_sprite->sub_5A6CA0(2);
    Sprite* v5 = pTrailerPointer->field_8_car->field_0_qq.FirstSpriteOfType_5A6CA0(2);
    Car_BC* v6;

    if (v5 != NULL && (v5->field_30_sprite_type_enum == 2 ? (v6 = v5->field_8_car_bc_ptr) : (v6 = NULL), v6 == pCarPointer->field_8_car))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c230)
void miss2_0x11C::SCRCMD_ENABLE_DISABLE_CRANE_50C230()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    s16 type = gBasePtr_6F8070->field_2_type;

    if (type == SCRCMD_ENABLE_CRANE)
    {
        pPointer->field_8_crane->field_148 = 0;
    }
    else if (type == SCRCMD_DISABLE_CRANE)
    {
        pPointer->field_8_crane->field_148 = 1;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c2a0)
void miss2_0x11C::SCRCMD_CAR_GOT_DRIVER_50C2A0()
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (pCmd->field_8_car->field_54_driver)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c2f0)
void miss2_0x11C::SCRCMD_SPOTTED_PLAYER_50C2F0()
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    BitSet32 flag;
    flag.m_var = pCmd->field_8_char->field_21C;
    if (flag.check_bit(23))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c350)
void miss2_0x11C::SCRCMD_GET_LAST_PUNCHED_50C350()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pCharTargetPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pCharPunchedPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_A_unsigned_2);

    pCharPunchedPointer->field_8_char = pCharTargetPointer->field_8_char->field_188_last_char_punched;
    pCharTargetPointer->field_8_char->field_188_last_char_punched = 0; //  reset

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c3b0)
void miss2_0x11C::SCRCMD_IS_CHAR_STUNNED_50C3B0()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed->field_27C_ped_state_2 == ped_state_2::lying_on_floor_22 || pPed->field_216_health <= 25)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c410)
void miss2_0x11C::SCRCMD_KILL_ALL_PASSENG_50C410()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    pPointer->field_8_car->field_4_passengers_list.KillAllPedsFromList_4715A0();

    if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(pPointer->field_8_car))
    {
        gPublicTransport_181C_6FF1D4->KillAllPassengers_579B20();
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c470)
void miss2_0x11C::SCRCMD_IS_GROUP_IN_CAR_50C470()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pGroupLeader = pPointer->field_8_char;
    PedGroup* pGroup = pGroupLeader->field_164_ped_group;

    bool v1;
    if (pGroup != NULL)
    {
        v1 = pGroup->IsAllMembersInSomeCar_4CAA20();
    }
    else
    {
        v1 = (pGroupLeader->field_16C_car != NULL);
    }
    if (v1)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c4e0)
void miss2_0x11C::SCRCMD_PUNCHED_SOMEONE_50C4E0()
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (pCmd->field_8_char->field_188_last_char_punched != NULL)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c540)
void miss2_0x11C::SCRCMD_ADD_CHAR_TO_GANG_50C540()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    str_table_entry* StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(v1->field_A_unsigned_2);

    char* gang_zone_name = (char*)&StringById_503080[1];

    pPointer->field_8_char->field_17C_pZone = gGangPool_CA8_67E274->gang_by_name_4BF100(gang_zone_name);

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50c5a0)
void miss2_0x11C::sub_50C5A0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50c6f0)
void miss2_0x11C::sub_50C6F0() // PARK and PARK_NO_RESPAWN
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pParam1 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pParam2 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_A_unsigned_2);

    gGarage_48_6FD26C->ParkCarAtDoor_534700(pParam1->field_8_car, pParam2->field_8_door);
    if (gBasePtr_6F8070->field_2_type == SCRCMD_PARK_NO_RESPAWN)
    {
        gGarage_48_6FD26C->field_3F_no_respawn = true;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c760)
void miss2_0x11C::SCRCMD_BEEN_PUNCHED_BY_50C760()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointerVictim = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pPointerAttacker = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_A_unsigned_2);

    if (pPointerAttacker->field_8_char->field_188_last_char_punched == pPointerVictim->field_8_char)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c7d0)
void miss2_0x11C::SCRCMD_UPDATE_DOOR_50C7D0()
{
    SCR_UPDATE_DOOR* pCmd = (SCR_UPDATE_DOOR*)gBasePtr_6F8070;
    SCR_POINTER* pDoorPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_8_door_idx);

    SCR_POINTER* pTarget;

    switch (pDoorPointer->field_8_door->field_20_state)
    {
        case 5:
            pTarget = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_target_idx);

            pDoorPointer->field_8_door->set_open_details_ped(5, pTarget->field_8_char);
            pDoorPointer->field_8_door->set_field_14_id(pTarget->field_8_char->field_200_id);
            break;
        case 3:
            pTarget = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_target_idx);

            pDoorPointer->field_8_door->set_open_details_car_bc(3, pTarget->field_8_car);
            pDoorPointer->field_8_door->set_field_14_id(pTarget->field_8_car->field_6C_maybe_id);
            break;
        case 6:
            pTarget = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_target_idx);

            pDoorPointer->field_8_door->set_open_details_car_bc(6, pTarget->field_8_car);
            pDoorPointer->field_8_door->set_field_14_id(pTarget->field_8_car->field_6C_maybe_id);
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50c8a0)
void miss2_0x11C::SCRCMD_DOOR_50C8A0()
{
    NOT_IMPLEMENTED;

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c990)
void miss2_0x11C::SCRCMD_REMOVE_WEAPON_50C990()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Ped* pPed = pPointer->field_8_char;
    Player* player = pPed->field_15C_player;

    if (player != NULL)
    {
        player->RemovePlayerWeapons_564C50();
    }
    else
    {
        pPed->RemovePedWeapons_462510();
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c9f0)
void miss2_0x11C::SCRCMD_REMOVE_BLOCK_50C9F0()
{
    SCR_REMOVE_BLOCK* pCmd = (SCR_REMOVE_BLOCK*)gBasePtr_6F8070;

    gMap_0x370_6F6268->RemoveBlock_4E8940(pCmd->field_8_pos.field_0_x,
                                          pCmd->field_8_pos.field_1_y,
                                          pCmd->field_8_pos.field_2_z,
                                          pCmd->field_B_do_drop);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ca30)
void miss2_0x11C::SCRCMD_LOWER_LEVEL_50CA30()
{
    SCR_LOWER_LEVEL* pCmd = (SCR_LOWER_LEVEL*)gBasePtr_6F8070;

    gMap_0x370_6F6268->LowerLevel_4E8B70(pCmd->field_8_min_pos.field_0_x,
                                         pCmd->field_A_max_pos.field_0_x,
                                         pCmd->field_8_min_pos.field_1_y,
                                         pCmd->field_A_max_pos.field_1_y);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ca70)
void miss2_0x11C::sub_50CA70()
{
    SCR_CHANGE_BLOCK* pCmd = (SCR_CHANGE_BLOCK*)gBasePtr_6F8070;
    gMap_0x370_6F6268->ChangeBlock_4E8620(pCmd->field_8_x,
                                          pCmd->field_9_y,
                                          pCmd->field_A_z,
                                          pCmd->field_B_change_type,
                                          pCmd->field_C_info_word);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50cab0)
void miss2_0x11C::SCRCMD_ADD_NEW_BLOCK_50CAB0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50cb20)
void miss2_0x11C::SCRCMD_ROAD_ON_OFF_50CB20()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50cb70)
void miss2_0x11C::sub_50CB70()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50ccb0)
void miss2_0x11C::sub_50CCB0()
{
    NOT_IMPLEMENTED;

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50cd30)
void miss2_0x11C::sub_50CD30()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50cdb0)
void miss2_0x11C::SCRCMD_CAR_DAMAGE_POS_50CDB0()
{
    SCR_TWO_PARAMS* v1;
    SCR_POINTER* pCmd;

    v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if ((pCmd->field_8_car)->IsAreaDamaged_43D1C0(v1->field_A_signed_2)) // field_A_signed_2 = damage pos
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ce10)
void miss2_0x11C::SCRCMD_PARK_FINISHED_50CE10()
{
    if (gGarage_48_6FD26C->field_C == 3)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50ce50)
void miss2_0x11C::sub_50CE50(SCR_CMD_HEADER* pCmd, u16 cmd_idx)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50ce90)
void miss2_0x11C::SCRCMD_PHONE_TEMPLATE_50CE90()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50d200)
void miss2_0x11C::SCRCMD_REMOTE_CONTROL_50D200()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50d2e0)
void miss2_0x11C::SCRCMD_LAUNCH_MISSION_50D2E0()
{
    str_table_entry* StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::launch_mission_5119A0(gBasePtr_6F8070, StringById_503080->get_name());
}

MATCH_FUNC(0x50d340)
void miss2_0x11C::SCRCMD_SAVE_GAME_50D340()
{
    char* pSaveFileName = gLucid_hamilton_67E8E0.GetDebugStr_4C5970();
    gfrosty_pasteur_6F8060->SaveGame_511E10(pSaveFileName);
    Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50d3c0)
void miss2_0x11C::sub_50D3C0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50d680)
void miss2_0x11C::SCRCMD_CHANGE_CAR_LOCK_50D680()
{
    SCR_CHANGE_CAR_LOCK* pCmd = (SCR_CHANGE_CAR_LOCK*)gBasePtr_6F8070;
    SCR_POINTER* pPtr = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar;

    switch (pCmd->field_A_lock_type)
    {
        case Car_Door_Lock::locked_1:
            pCar = pPtr->field_8_car;
            if (pCar->field_98 != Car_Door_Lock::locked_permanently_4)
            {
                pCar->field_98 = Car_Door_Lock::locked_1;
            }

            break;
        case Car_Door_Lock::lockout_thief_2:
            pCar = pPtr->field_8_car;
            if (pCar->field_98 != Car_Door_Lock::locked_permanently_4)
            {
                pCar->field_98 = Car_Door_Lock::lockout_thief_2;
            }

            break;
        case Car_Door_Lock::unlocked_3:
            pCar = pPtr->field_8_car;
            if (pCar->field_98 != Car_Door_Lock::locked_permanently_4)
            {
                pCar->field_98 = Car_Door_Lock::unlocked_3;
            }
            break;
        case Car_Door_Lock::locked_permanently_4:
            pPtr->field_8_car->field_98 = Car_Door_Lock::locked_permanently_4;
            break;
        case Car_Door_Lock::lockout_player_5:
            pCar = pPtr->field_8_car;
            if (pCar->field_98 != Car_Door_Lock::locked_permanently_4)
            {
                pCar->field_98 = Car_Door_Lock::lockout_player_5;
            }
            break;
        default:
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50d870)
void miss2_0x11C::SCRCMD_CHANGE_INTENSITY_50D870()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50d900)
void miss2_0x11C::SCRCMD_CHANGE_COLOUR_50D900()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    u32 rgb = v1->field_C_u32;
    pPointer->field_8_light->field_10_argb = rgb;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50d9a0)
void miss2_0x11C::SCRCMD_CHANGE_RADIUS_50D9A0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50da50)
void miss2_0x11C::sub_50DA50()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50db70)
void miss2_0x11C::sub_50DB70()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50dd00)
void miss2_0x11C::sub_50DD00()
{
    NOT_IMPLEMENTED;
}

// matches on decompme: https://decomp.me/scratch/y8gtV
WIP_FUNC(0x50dd90)
void miss2_0x11C::SCRCMD_SET_DIR_OF_TVVAN_50DD90()
{
    SCR_SET_DIR_OF_TVVAN* pCmd = (SCR_SET_DIR_OF_TVVAN*)gBasePtr_6F8070;

    Fix16_Point point(pCmd->field_8_pos.field_0_x, pCmd->field_8_pos.field_4_y);
    gCar_6C_677930->field_4C_tv_van_dir = point;

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50de00)
void miss2_0x11C::SCRCMD_POINT_ONSCREEN_50DE00()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50de50)
void miss2_0x11C::SCRCMD_CHAR_IN_AIR_50DE50()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed->field_27C_ped_state_2 == ped_state_2::falling_19 
        && pPed->field_278_ped_state_1 == ped_state_1::immobilized_8)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50deb0)
void miss2_0x11C::SCRCMD_CHAR_SUNK_50DEB0()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed->field_27C_ped_state_2 == ped_state_2::sinking_20 
        && pPed->field_278_ped_state_1 == ped_state_1::immobilized_8)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50df10)
void miss2_0x11C::sub_50DF10()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50e0b0)
void miss2_0x11C::sub_50E0B0()
{
    s16 result = miss2_0x11C::sub_505EA0(gBasePtr_6F8070[1].field_0_cmd_this);
    field_8 = false;

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_CHECK_BONUS1:
            if (result == -3)
            {
                field_8 = true;
            }
            break;
        case SCRCMD_CHECK_BONUS2:
            if (result == -4)
            {
                field_8 = true;
            }
            break;
        case SCRCMD_CHECK_BONUS3:
            if (result != -2)
            {
                field_8 = true;
            }
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e120)
void miss2_0x11C::sub_50E120()
{
    gCar_6C_677930->field_58_model_to_check_destroy = (u16)gBasePtr_6F8070[1].field_0_cmd_this;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e150)
void miss2_0x11C::sub_50E150() // SCRCMD_MODEL_CHECK
{
    if (gCar_6C_677930->field_5C)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50e190)
void miss2_0x11C::sub_50E190()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50e360)
void miss2_0x11C::SCRCMD_CHECK_CAR_SPEED_50E360()
{
    SCR_CHECK_CAR_SPEED* pCmd = (SCR_CHECK_CAR_SPEED*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (pPointer->field_8_car->field_58_physics &&
        pPointer->field_8_car->field_58_physics->get_car_velocity_4754D0().get_value_4754D0() > pCmd->field_A_value)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e460)
void miss2_0x11C::sub_50E460() //  SCRCMD_SET_CAR_GRAPHIC
{
    SCR_SET_CAR_GRAPHIC* pCmd = (SCR_SET_CAR_GRAPHIC*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    pPointer->field_8_car->sub_43CDF0(pCmd->field_C_number); // set the number on the top of the car
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e4a0)
void miss2_0x11C::SCRCMD_CHAR_ARRESTED_50E4A0()
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if ((pCmd->field_8_char->field_21C & 0x20) != 0)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e4f0)
void miss2_0x11C::sub_50E4F0()
{
    SCR_OPERATE_COUNTER_AND_COUNTER* pCmd = (SCR_OPERATE_COUNTER_AND_COUNTER*)gBasePtr_6F8070;
    u8 type = miss2_0x11C::sub_506BC0((u16)gBasePtr_6F8070->field_2_type);

    SCR_POINTER* pDestCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_8_destination_counter_idx);
    SCR_POINTER* pFirstOperand = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_first_operand_counter_idx);
    SCR_POINTER* pSecondOperand = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_C_second_operand_counter_idx);
    switch (type)
    {
        case 0:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter + pSecondOperand->field_8_counter;
            break;
        case 1:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter - pSecondOperand->field_8_counter;
            break;
        case 8:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter * pSecondOperand->field_8_counter;
            break;
        case 7:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter / pSecondOperand->field_8_counter;
            break;
        case 9:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter % pSecondOperand->field_8_counter;
            break;
        default:
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e610)
void miss2_0x11C::sub_50E610()
{
    SCR_OPERATE_COUNTER_AND_INT* pCmd = (SCR_OPERATE_COUNTER_AND_INT*)gBasePtr_6F8070;
    SCR_POINTER* pDestCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_8_destination_counter_idx);
    SCR_POINTER* pOperandCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_operand_counter_idx);

    switch (miss2_0x11C::sub_506BC0(gBasePtr_6F8070->field_2_type))
    {
        case 0:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter + pCmd->field_C_value;
            break;
        case 1:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter - pCmd->field_C_value;
            break;
        case 8:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter * pCmd->field_C_value;
            break;
        case 7:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter / pCmd->field_C_value;
            break;
        case 9:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter % pCmd->field_C_value;
            break;
        default:
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e730)
void miss2_0x11C::sub_50E730()
{
    SCR_SET_CHAR_DRIVE_AGGR* pCmd = (SCR_SET_CHAR_DRIVE_AGGR*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    pPointer->field_8_char->field_21C_bf.b3 = pCmd->field_A_status;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e780)
void miss2_0x11C::sub_50E780()
{
    SCR_SET_RUN_SPEED* pCmd = (SCR_SET_RUN_SPEED*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (gBasePtr_6F8070->field_2_type == SCRCMD_SET_RUN_SPEED)
    {
        pPointer->field_8_char->field_1F0_maybe_max_speed = pCmd->field_A_value;
    }
    else
    {
        pPointer->field_8_char->field_1F8 = pCmd->field_A_value;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50e7f0)
void miss2_0x11C::sub_50E7F0()
{
    NOT_IMPLEMENTED;

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e820)
void miss2_0x11C::sub_50E820()
{
    SCR_SET_CAR_DAMAGE_PROOF* pCmd = (SCR_SET_CAR_DAMAGE_PROOF*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar;

    switch (pCmd->field_2_type)
    {
        case SCRCMD_CAR_BULLETPROOF:
            pCar = pPointer->field_8_car;
            if (pCmd->field_A_status == 1)
            {
                pCar->field_78_flags |= 0x100;
            }
            else
            {
                pCar->field_78_flags &= ~0x100;
            }
            break;
        case SCRCMD_CAR_ROCKETPROOF:
            pCar = pPointer->field_8_car;
            if (pCmd->field_A_status == 1)
            {
                pCar->field_78_flags |= 0x200;
            }
            else
            {
                pCar->field_78_flags &= ~0x200;
            }
            break;
        case SCRCMD_CAR_FLAMEPROOF:
            pCar = pPointer->field_8_car;
            if (pCmd->field_A_status == 1)
            {
                pCar->field_78_flags |= 0x400;
            }
            else
            {
                pCar->field_78_flags &= ~0x400;
            }
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e900)
void miss2_0x11C::sub_50E900()
{
    SCR_PUT_CAR_ON_TRAILER* pCmd = (SCR_PUT_CAR_ON_TRAILER*)gBasePtr_6F8070;
    SCR_POINTER* pCarPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pDstCarPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_trailer_car_idx);

    Car_BC* pDstCar = pDstCarPointer->field_8_car;
    if (pDstCar->field_88 != 6)
    {
        Trailer* pTrailer = pDstCar->field_64_pTrailer;
        if (pTrailer != NULL && pTrailer->field_8_truck_cab == pDstCar)
        {
            // put car on the trailer attached to the truck cab
            pDstCar->GetSprite_440840()->DispatchCollisionEvent_5A3100(pCarPointer->field_8_car->field_50_car_sprite, dword_6F77C0, dword_6F77C0, word_6F771E);
        }
        else
        {
            // put car directly on the trailer
            pDstCar->field_50_car_sprite->DispatchCollisionEvent_5A3100(pCarPointer->field_8_car->field_50_car_sprite,
                                                     dword_6F77C0,
                                                     dword_6F77C0,
                                                     word_6F771E);
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e9a0)
void miss2_0x11C::sub_50E9A0()
{
    gHud_2B00_706620->field_DC.ClearAllBriefsWithPriority_5D4890(1); // clear lowest brief priority
    gHud_2B00_706620->field_DC.ClearAllBriefsWithPriority_5D4890(3); // clear highest brief priority
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e9e0)
void miss2_0x11C::sub_50E9E0() //  SCRCMD_CHECK_HEADS
{
    SCR_CHECK_HEADS_GREATER* pCmd = (SCR_CHECK_HEADS_GREATER*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (pPointer->field_8_char->get_wanted_star_count_46EF00() > pCmd->field_A_wanted_level)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ea40)
void miss2_0x11C::sub_50EA40() //  SCRCMD_FINISH_LEVEL
{
    SCR_FINISH_LEVEL* pCmd = (SCR_FINISH_LEVEL*)gBasePtr_6F8070;
    char bonus_type;
    switch (pCmd->field_A_bonus_type)
    {
        case SCR_BONUSES::NO_BONUS:
            bonus_type = 0;
            break;
        case SCR_BONUSES::BONUS_1:
            bonus_type = 1;
            break;
        case SCR_BONUSES::BONUS_2:
            bonus_type = 2;
            break;
        case SCR_BONUSES::BONUS_3:
            bonus_type = 3;
            break;
        default:
            break;
    }

    s32* num_passed_flag = gfrosty_pasteur_6F8060->field_328_passed_flag;
    if (num_passed_flag != NULL && gfrosty_pasteur_6F8060->field_314_total_missions == *num_passed_flag)
    {
        bonus_type = 2;
    }
    if (bonus_type)
    {
        gGame_0x40_67E008->ExitGame_4B8BD0(0, GameExitType::AreaCompleted_4, bonus_type);
    }
    else
    {
        gGame_0x40_67E008->ExitGameNoBonus_4B8C00(0, GameExitType::AreaCompleted_4);
    }

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50eb00)
void miss2_0x11C::sub_50EB00() //  SCRCMD_CHECK_WEAPONHIT
{
    SCR_CHECK_WEAPONHIT* pCmd = (SCR_CHECK_WEAPONHIT*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (!pCmd->field_A_status)
    {
        gfrosty_pasteur_6F8060->sub_512AF0(pPointer->field_8_char->get_id(), pCmd->field_C_weapon, 1);
        field_8 = false;
        pCmd->field_A_status = 1;
    }
    else
    {
        if (gfrosty_pasteur_6F8060->sub_512C70(pPointer->field_8_char->get_id(), pCmd->field_C_weapon, 1))
        {
            field_8 = true;
            gfrosty_pasteur_6F8060->sub_512BA0(pPointer->field_8_char->get_id(), 1);
            pCmd->field_A_status = 0;
        }
        else
        {
            field_8 = false;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ebd0)
void miss2_0x11C::sub_50EBD0() //  SCRCMD_WEAP_HIT_CAR
{
    SCR_CHECK_WEAPONHIT* pCmd = (SCR_CHECK_WEAPONHIT*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (!pCmd->field_A_status)
    {
        char v5;
        if (gBasePtr_6F8070->field_2_type == SCRCMD_WEAP_HIT_CAR)
        {
            v5 = gfrosty_pasteur_6F8060->sub_512AF0(pPointer->field_8_car->field_6C_maybe_id, pCmd->field_C_weapon, 0);
        }
        else
        {
            v5 = gfrosty_pasteur_6F8060->sub_512AF0(pPointer->field_8_car->field_6C_maybe_id, 23, 0);
        }
        if (v5 && gfrosty_pasteur_6F8060->sub_512C70(pPointer->field_8_car->field_6C_maybe_id, pCmd->field_C_weapon, 0))
        {
            field_8 = true;
            pCmd->field_A_status = 0;
        }
        else
        {
            field_8 = false;
            pCmd->field_A_status = 1;
        }
    }
    else
    {
        if (gfrosty_pasteur_6F8060->sub_512C70(pPointer->field_8_car->field_6C_maybe_id, pCmd->field_C_weapon, 0))
        {
            field_8 = true;
            pCmd->field_A_status = 0;
        }
        else
        {
            field_8 = false;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

// TODO: https://decomp.me/scratch/NMvtk trying to match this using CompilerBitField32 instead of BitSet32
MATCH_FUNC(0x50ece0)
void miss2_0x11C::sub_50ECE0() //  SCRCMD_IS_CHAR_ON_FIRE
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    BitSet32 flag = pPointer->field_8_char->field_21C;

    if (flag.check_bit(24))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ed40)
void miss2_0x11C::sub_50ED40() //  SCRCMD_BRIEF_ONSCREEN
{
    Garox_18* field_6F8_prev_brief = gHud_2B00_706620->field_DC.field_6F8_prev_brief;
    if (field_6F8_prev_brief != NULL && field_6F8_prev_brief->field_8_brief_priority != 0)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ed80)
void miss2_0x11C::sub_50ED80() //  SCRCMD_CREATE_SOUND
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::SCRCMD_SOUND_DECSET_505340((SCR_SOUND_DECSET*)gBasePtr_6F8070, (SCR_POINTER*)pPointer);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50edc0)
void miss2_0x11C::sub_50EDC0() //  EASY_PHONE_TEMPLATE
{
    SCR_PHONE_TEMPLATE* v1 = (SCR_PHONE_TEMPLATE*)gBasePtr_6F8070;

    SCR_POINTER* pParam3;
    SCR_POINTER* pParam4;
    SCR_POINTER* pParam5;
    SCR_POINTER* pParam1;
    SCR_POINTER* pParam2;

    pParam1 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_A_counter1);
    pParam2 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_10_counter2);
    pParam3 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_12_counter3);
    pParam4 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_14_counter4);
    pParam5 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_16_counter5);
    str_table_entry* StringById = gfrosty_pasteur_6F8060->FindStringById_503080(v1->field_18_gangname);
    char_type* gang_name = (char_type*)&StringById[1];
    Gang_144* pGang_zone = gGangPool_CA8_67E274->gang_by_name_4BF100(gang_name);
    if (pParam3->field_8_counter == 1) //  flag_on_[gang1]_mission == 1
    {
        s32 v9 = v1->field_8_brief_id - 5;
        sprintf(gTmpBuffer_67C598, "%d", v9);
        gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
        miss2_0x11C::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pParam4->field_8_counter == 1) //  flag_on_[gang2]_mission == 1
    {
        s32 v9 = v1->field_8_brief_id - 2;
        sprintf(gTmpBuffer_67C598, "%d", v9);
        gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
        miss2_0x11C::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pParam5->field_8_counter == 1) //  flag_on_[gang3]_mission == 1
    {
        s32 v9 = v1->field_8_brief_id - 1;
        sprintf(gTmpBuffer_67C598, "%d", v9);
        gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
        miss2_0x11C::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pParam1->field_8_counter == 1) //  flag_passed_[gang1]_easy_phone_m1 == 1
    {
        //  not used in original levels, since phones are disabled after finishing mission
        s32 v9 = v1->field_8_brief_id;
        sprintf(gTmpBuffer_67C598, "%d", v9);
        gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
        miss2_0x11C::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pParam2->field_8_counter == 1) //  flag_failed_[this_gang]_easy_phone_m1 == 1
    {
        //  not used in the game as you can repeat a failed mission in GTA2
        s32 v9 = v1->field_8_brief_id + 1;
        sprintf(gTmpBuffer_67C598, "%d", v9);
        gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
        miss2_0x11C::Next_503620(gBasePtr_6F8070);
        return;
    }
    s8 idx1;
    //  Check if the player has respect enough to launch the mission
    if ((idx1 = gGame_0x40_67E008->field_38_orf1->field_2E_idx, pGang_zone->GetRespectForPlayer_4BEEF0(idx1) / 20 >= v1->field_1A_respect))
    {
        str_table_entry* mission1_str_table = gfrosty_pasteur_6F8060->FindStringById_503080(v1->field_1C_mission1);
        char_type* mission1_name = (char_type*)&mission1_str_table[1];
        miss2_0x11C::launch_mission_5119A0(gBasePtr_6F8070, mission1_name); //  TODO: fix gBasePtr_6F8070 type
        return;
    }
    s8 idx2;
    //  Check if the player has respect = 0
    if ((idx2 = gGame_0x40_67E008->field_38_orf1->field_2E_idx, (pGang_zone->GetRespectForPlayer_4BEEF0(idx2) / 20) == 0))
    {
        s32 v9 = v1->field_8_brief_id - 6;
        sprintf(gTmpBuffer_67C598, "%d", v9);
        gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
        miss2_0x11C::Next_503620(gBasePtr_6F8070);
        return;
    }
    s8 idx3;
    //  Check if the player has negative respect
    if ((idx3 = gGame_0x40_67E008->field_38_orf1->field_2E_idx, pGang_zone->GetRespectForPlayer_4BEEF0(idx3) / 20 < 0))
    {
        s32 v9 = v1->field_8_brief_id - 4;
        sprintf(gTmpBuffer_67C598, "%d", v9);
        gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
        miss2_0x11C::Next_503620(gBasePtr_6F8070);
    }
    else
    {
        //  In this case, the player has positive respect but not enough to launch the mission
        s32 v9 = v1->field_8_brief_id - 7;
        sprintf(gTmpBuffer_67C598, "%d", v9);
        gHud_2B00_706620->field_DC.sub_5D4400(1, gTmpBuffer_67C598);
        miss2_0x11C::Next_503620(gBasePtr_6F8070);
    }
}

STUB_FUNC(0x50f060)
void miss2_0x11C::SCRCMD_CHAR_INTO_CAR_50F060()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x50f150)
void miss2_0x11C::SCRCMD_DECIDE_POWERUP_50F150()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50f220)
void miss2_0x11C::SCRCMD_SUPPRESS_MODEL_50F220()
{
    if ((u16)gBasePtr_6F8070[1].field_0_cmd_this == 0xFFFFu) //  TODO: fix gBasePtr_6F8070
    {
        gCar_6C_677930->field_C_model_unk = car_model_enum::none; //  No model suppressed
    }
    else
    {
        gCar_6C_677930->field_C_model_unk = (u16)gBasePtr_6F8070[1].field_0_cmd_this;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f270)
void miss2_0x11C::sub_50F270() // WARP_FROM_CAR_TO_POINT
{
    SCR_WARP_FROM_CAR* pCmd = (SCR_WARP_FROM_CAR*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;
    Car_BC* pCar = pPed->field_16C_car;

    if (pPed != NULL)
    {
        pPed->sub_470200(pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y, pCmd->field_C_pos.field_8_z);

        Ang16 rotation = word_6F8044 * pCmd->field_A_rotation;
        rotation.Normalize();

        pPointer->field_8_char->field_168_game_object->field_40_rotation = rotation;
        pCar->ClearDriver_4407F0();
        pCar->field_54_driver = 0;
        gGame_0x40_67E008->field_38_orf1->sub_569F40();
        gGame_0x40_67E008->field_38_orf1->field_90_game_camera.sub_436540(pPointer->field_8_char);

        Camera_0xBC* p_game_camera = &(gGame_0x40_67E008->field_38_orf1->field_90_game_camera);

        p_game_camera->sub_41E410();

        gGame_0x40_67E008->field_38_orf1->field_90_game_camera.sub_435DD0();
        gGame_0x40_67E008->field_38_orf1->field_90_game_camera.field_3C_followed_ped_id = 1;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f3d0)
void miss2_0x11C::SCRCMD_SET_GROUP_TYPE_50F3D0()
{
    SCR_SET_GROUP_TYPE* pCmd = (SCR_SET_GROUP_TYPE*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;
    PedGroup* pGroup = pPed->field_164_ped_group;

    if (pGroup != NULL)
    {
        pGroup->field_38_group_type = pCmd->field_C_group_type;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f410)
void miss2_0x11C::SCRCMD_CHAR_DO_NOTHING_50F410()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        pPed->ForceDoNothing_462590();
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f450)
void miss2_0x11C::SCRCMD_EMERG_LIGHTS_50F450()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* v4 = pPointer->field_8_car;
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(v4->field_84_car_info_idx)->info_flags & 2) == 2 || v4->field_84_car_info_idx == 84)
    {
        if ((u8)v1->field_A_unsigned_2 == 1)
        {
            pPointer->field_8_car->ActivateEmergencyLights_43C920(); //  activate sirens
        }
        else
        {
            pPointer->field_8_car->DeactivateEmergencyLights_43C9D0(); //  deactivate sirens
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f4d0)
void miss2_0x11C::SCRCMD_CHECK_OBJ_MODEL_50F4D0()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Object_2C* pObj = pPointer->field_8_obj;
    u32 model_idx = v1->field_C_u32;

    if (pObj != NULL && pObj->field_18_model == model_idx)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f550)
void miss2_0x11C::SCRCMD_PED_GRAPHIC_50F550()
{
    SCR_SET_CHAR_GRAPHIC_TYPE* pCmd = (SCR_SET_CHAR_GRAPHIC_TYPE*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (pPointer->field_8_char != NULL)
    {
        pPointer->field_8_char->field_26C_graphic_type = pCmd->field_C_graphic_type;
        pPointer->field_8_char->field_244_remap = pCmd->field_A_remap;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50f5e0)
void miss2_0x11C::sub_50F5E0()
{
    NOT_IMPLEMENTED;

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f770)
void miss2_0x11C::SCRCMD_MAKE_MUGGERS_50F770()
{
    SCR_SET_STATE* pCmd = (SCR_SET_STATE*)gBasePtr_6F8070;
    if (pCmd->field_A_status == 1)
    {
        gPedManager_6787BC->field_7_make_all_muggers = true;
    }
    else
    {
        gPedManager_6787BC->field_7_make_all_muggers = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50f7b0)
void miss2_0x11C::sub_50F7B0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50f900)
void miss2_0x11C::SCRCMD_STOP_CAR_DRIVE_50F900()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Car_BC* pCar = pPointer->field_8_char->field_16C_car;

    if (pCar)
    {
        pCar->field_A6 |= 0x20u;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f940)
void miss2_0x11C::SCRCMD_IS_BUS_FULL_50F940()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    field_8 = false;
    Ped* pPed = pPointer->field_8_char;

    if (pPed)
    {
        Car_BC* pCar = pPed->field_16C_car;
        if (pCar)
        {
            if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(pCar) && gPublicTransport_181C_6FF1D4->is_bus_full_579AF0())
            {
                field_8 = true;
            }
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f9b0)
void miss2_0x11C::SCRCMD_NO_CHARS_OFF_BUS_50F9B0()
{
    SCR_SET_STATE* pCmd = (SCR_SET_STATE*)gBasePtr_6F8070;
    if (pCmd->field_A_status == 1)
    {
        gPublicTransport_181C_6FF1D4->field_1818_stop_getting_off_bus = true;
    }
    else
    {
        gPublicTransport_181C_6FF1D4->field_1818_stop_getting_off_bus = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa00)
void miss2_0x11C::sub_50FA00() // SCRCMD_KILL_char_type
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        pPed->Kill_46F9D0(); // kill ped
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa40)
void miss2_0x11C::SCRCMD_SET_SHADING_LEV_50FA40()
{
    gpMapRenderer_6F66E4->set_shading_lev_4E9DB0(gBasePtr_6F8070[1].field_2_type);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa70)
void miss2_0x11C::SCRCMD_SET_CAR_JAMMED_50FA70()
{
    SCR_SET_CAR_JAMMED* pCmd = (SCR_SET_CAR_JAMMED*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Car_BC* pCar = pPointer->field_8_car;

    if (pCar)
    {
        if (pCmd->field_A_status == 1)
        {
            pCar->field_78_flags |= 0x1000;
        }
        else
        {
            pCar->field_78_flags &= ~0x1000u;
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fad0)
void miss2_0x11C::SCRCMD_FINISH_MISSION_50FAD0()
{
    gMiss2_25C_6F805C->MissionCleanUp_502DC0();
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50faf0)
void miss2_0x11C::sub_50FAF0() // SCRCMD_STORE_BONUS
{
    SCR_STORE_BONUS_COUNT* pCmd = (SCR_STORE_BONUS_COUNT*)gBasePtr_6F8070;
    SCR_POINTER* pPointerBonus = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pPointerCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_counter_idx);

    s32 bonus_unk = pPointerBonus->field_8_counter;

    if (bonus_unk != -3 && bonus_unk != -4)
    {
        pPointerCounter->field_8_counter =
            gGame_0x40_67E008->field_38_orf1->field_2D4_scores.field_1A8_unk.field_0[pPointerBonus->field_8_index].field_26;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fb60)
void miss2_0x11C::sub_50FB60() // SCRCMD_SET_ENTER_STATUS and SCRCMD_SET_ALL_CONTROLS
{
    SCR_SET_PLAYER_CONTROLS* pCmd = (SCR_SET_PLAYER_CONTROLS*)gBasePtr_6F8070;

    gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    // The player pointer is not used, so the user controls are modified instead
    // This explains desync in multiplayer when SET_ENTER_CONTROL_STATUS are used

    if (pCmd->field_A_new_state == 1)
    {
        if (gBasePtr_6F8070->field_2_type == SCRCMD_SET_ENTER_STATUS)
        {
            gGame_0x40_67E008->field_38_orf1->EnableEnterVehicles_56A040(); // SET_ENTER_STATUS = ON
        }
        else
        {
            gGame_0x40_67E008->field_38_orf1->EnableAllControls_56A000(); // SET_ALL_CONTROLS = ON
        }
    }
    else if (gBasePtr_6F8070->field_2_type == SCRCMD_SET_ENTER_STATUS)
    {
        gGame_0x40_67E008->field_38_orf1->DisableEnterVehicles_56A030(); // SET_ENTER_STATUS = OFF
    }
    else
    {
        gGame_0x40_67E008->field_38_orf1->DisableAllControls_569FF0(); // SET_ALL_CONTROLS = OFF
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fc20)
void miss2_0x11C::sub_50FC20() // SCRCMD_SET_FAV_CAR
{
    SCR_SET_FAV_CAR* pCmd = (SCR_SET_FAV_CAR*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        pPed->field_274_gang_car_model = pCmd->field_C_favourite_car;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50fc60)
void miss2_0x11C::sub_50FC60()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x50fe00)
void miss2_0x11C::sub_50FE00() // SCRCMD_SET_KF_WEAPON
{
    SCR_SET_KF_WEAPON* v1 = (SCR_SET_KF_WEAPON*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    s32 weapon_idx = v1->field_C_weapon_idx;

    if (is_car_weapon(weapon_idx))
    {
        pPointer->field_8_char->field_15C_player->sub_564710(pPointer->field_8_char->field_16C_car, weapon_idx);
    }
    else
    {
        pPointer->field_8_char->field_15C_player->sub_564790(weapon_idx);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fed0)
void miss2_0x11C::sub_50FED0() // SCRCMD_CLEAR_KF_WEAPON
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Player* pPlayer = pPointer->field_8_char->field_15C_player;

    pPlayer->ClearKFWeapon_5647D0(); // clear kill frenzy weapon
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ff50)
void miss2_0x11C::sub_50FF50() // SCRCMD_ADD_ONSCREEN_COUNTER
{
    SCR_ADD_ONSCREEN_COUNTER* v1 = (SCR_ADD_ONSCREEN_COUNTER*)gBasePtr_6F8070;
    SCR_POINTER* pOnScreenCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(v1->field_A_counter_idx);

    pOnScreenCounter->field_8_index = gHud_2B00_706620->field_620.AddOnScreenCounter_5D3220(&pCounter->field_8_counter);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ffb0)
void miss2_0x11C::sub_50FFB0() // SCRCMD_CLEAR_COUNTER and SCRCMD_CLEAR_CLOCK_ONLY
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    u16 idx = pPointer->field_8_index;

    if (idx != 0xFFFF)
    {
        if (gBasePtr_6F8070->field_2_type == SCRCMD_CLEAR_COUNTER)
        {
            gHud_2B00_706620->field_620.ClearCounterOnly_5D3310(idx);
        }
        else // == SCRCMD_CLEAR_CLOCK_ONLY
        {
            gHud_2B00_706620->field_620.ClearClockOnly_5D32D0(idx);
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510030)
void miss2_0x11C::sub_510030() // SCRCMD_CHANGE_POLICE
{
    miss2_0x11C::SCRCMD_DECLARE_POLICE_5052C0((SCR_DECLARE_POLICELEVEL*)gBasePtr_6F8070);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510050)
void miss2_0x11C::sub_510050() // SCRCMD_DESTROY_GROUP
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    Ped* pGroupLeader = pPointer->field_8_char;
    PedGroup* pGroup = pGroupLeader->field_164_ped_group;

    if (pGroup)
    {
        pGroup->DestroyGroup_4C93A0(); // destroy group
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510090)
void miss2_0x11C::sub_510090()
{
    SCR_CHECK_CURRENT_WEAPON* pCmd = (SCR_CHECK_CURRENT_WEAPON*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    Weapon_30* pWeapon = pPointer->field_8_char->field_15C_player->GetCurrPlayerWeapon_5648F0();

    if (pWeapon != NULL && pWeapon->field_1C_idx == pCmd->field_C_weapon_idx)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x510100)
void miss2_0x11C::sub_510100()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x510280)
void miss2_0x11C::sub_510280()
{
    SCR_POINTER* pTimerIDPointer;

    SCR_DO_BASIC_KF_TEMPLATE* pCmd = (SCR_DO_BASIC_KF_TEMPLATE*)gBasePtr_6F8070;
    SCR_POINTER* pPlayerPedCmdPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_18_playername);

    if (field_C == 0)
    {
        pTimerIDPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_timername);
        pTimerIDPointer->field_8_index = gHud_2B00_706620->field_620.CreateTimer_5D31F0(pCmd->field_C_time_limit);
        SCR_POINTER* pOnScreenCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_10_onscreenname);
        SCR_POINTER* pCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_12_countername);
        pOnScreenCounter->field_8_index = gHud_2B00_706620->field_620.AddOnScreenCounter_5D3220(&pCounter->field_8_counter);
        ++field_C;
    }
    else
    {
        if (miss2_0x11C::sub_505EA0(pCmd->field_8_bonusname) == -2)
        {
            SCR_POINTER* pBonusType = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_8_bonusname);
            SCR_POINTER* pCounter = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_12_countername);
            pCounter->field_8_counter = pCmd->field_14_target_total -
                (u8)gGame_0x40_67E008->field_38_orf1->field_2D4_scores.field_1A8_unk.field_0[pBonusType->field_8_index].field_26;

            Ped* pPed = pPlayerPedCmdPointer->field_8_char;

            if (pPed->field_278_ped_state_1 == ped_state_1::dead_9 || pPed->field_21C_bf.b5 != 0)
            {
                gGame_0x40_67E008->field_38_orf1->field_2D4_scores.field_1A8_unk.field_0[pBonusType->field_8_index].sub_431DB0();
                pPlayerPedCmdPointer->field_8_char->field_15C_player->ClearKFWeapon_5647D0();
            }
        }
        else
        {
            pTimerIDPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(pCmd->field_A_timername);
            if (pTimerIDPointer->field_8_index != 0xFFFF)
            {
                gHud_2B00_706620->field_620.ClearPager_5D3280(pTimerIDPointer->field_8_index);
            }
            pTimerIDPointer->field_8_index = 0;
            pPlayerPedCmdPointer->field_8_char->field_15C_player->ClearKFWeapon_5647D0();
            gfrosty_pasteur_6F8060->field_C1E2D = 0;
            if (miss2_0x11C::sub_505EA0(pCmd->field_8_bonusname) == -3)
            {
                gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("kfpass"), 3);
                pPlayerPedCmdPointer->field_8_char->field_20A_wanted_points = 0;
                ++*gfrosty_pasteur_6F8060->field_338_secrets_passed;

                if (pCmd->field_1A_rewardtype == 1)
                {
                    pPlayerPedCmdPointer->field_8_char->field_15C_player->Add_2D4(pCmd->field_1C_rewardvalue);
                    gRoot_sound_66B038.PlayVoice_40F090(19);
                }
                else if (pCmd->field_1A_rewardtype == 2)
                {
                    pPlayerPedCmdPointer->field_8_char->field_15C_player->field_6BC_multpliers.ChangeStatByAmount_4921B0(
                        pCmd->field_1C_rewardvalue);
                    gRoot_sound_66B038.PlayVoice_40F090(19);
                }
                else
                {
                    if (pCmd->field_1A_rewardtype == 3)
                    {
                        pPlayerPedCmdPointer->field_8_char->field_15C_player->ChangeLifeCountByAmount_5699F0(pCmd->field_1C_rewardvalue);
                    }
                    gRoot_sound_66B038.PlayVoice_40F090(19);
                }
            }
            else
            {
                gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("kffail"), 3);
                ++*gfrosty_pasteur_6F8060->field_33C_secrets_failed;
                gRoot_sound_66B038.PlayVoice_40F090(18);
            }
            miss2_0x11C::Next_503620(gBasePtr_6F8070);
        }
    }
}

MATCH_FUNC(0x510530)
void miss2_0x11C::sub_510530()
{
    gLucid_hamilton_67E8E0.sub_4C5AB0(gBasePtr_6F8070[1].field_2_type);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510560)
void miss2_0x11C::sub_510560()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    gHud_2B00_706620->field_620.AddTime_5D32F0(pPointer->field_8_index, 30 * v1->field_C_u32);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5105b0)
void miss2_0x11C::sub_5105B0()
{
    SCR_TWO_PARAMS* pCmd = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    s16 model_idx = pCmd->field_A_signed_2;

    if (model_idx == -1)
    {
        gfrosty_pasteur_6F8060->field_C1E70 = car_model_enum::none;
    }
    else
    {
        gfrosty_pasteur_6F8060->field_C1E70 = model_idx;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510600)
void miss2_0x11C::sub_510600()
{
    if (gfrosty_pasteur_6F8060->field_C1E2E_death_arrest_flag)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510660)
void miss2_0x11C::sub_510660()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (miss2_0x11C::sub_503410(pPointer->field_2_type) == 3)
    {
        gMiss2_25C_6F805C->push_type_2_502FF0(pPointer->field_8_obj, 1);
    }
    else if (miss2_0x11C::sub_503410(pPointer->field_2_type) == 10)
    {
        gMiss2_25C_6F805C->push_type_2_503050(pPointer->field_8_obj);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510780)
void miss2_0x11C::sub_510780()
{
    u8 v4 = 0;
    Gang_144* v2 = gGangPool_CA8_67E274->sub_4BECA0();

    if (gBasePtr_6F8070->field_2_type == 445)
    {
        for (; v2 != NULL; v2 = gGangPool_CA8_67E274->sub_4BECE0())
        {
            gfrosty_pasteur_6F8060->field_C1E2F[v4++] = v2->GetRespectForPlayer_4BEEF0(0);
        }
    }
    else
    {
        for (; v2 != NULL; v2 = gGangPool_CA8_67E274->sub_4BECE0())
        {
            v2->SetRespect_4BEE30(0, gfrosty_pasteur_6F8060->field_C1E2F[v4++]);
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5108d0)
void miss2_0x11C::PreExecOpCode_5108D0()
{
    if (field_10 != 1)
    {
        SCR_CMD_HEADER* pCmd = gfrosty_pasteur_6F8060->GetBasePointer_512770(field_4_next_cmd);
        gBasePtr_6F8070 = pCmd;
        switch (pCmd->field_2_type)
        {
            case SCRCMD_LEVELSTART:
                SCRCMD_LEVELSTART_5069C0();
                break;
            case SCRCMD_LEVELEND:
                SCRCMD_LEVELEND_5069F0();
                break;
            case SCRCMD_CREATE_THREAD:
                SCRCMD_CREATE_THREAD_506A00();
                break;
            case SCRCMD_STOP_THREAD:
                SCRCMD_STOP_THREAD_506A60();
                break;
            case SCRCMD_IF_JUMP:
                SCRCMD_IF_JUMP_506AF0(); // IF_JUMP?
                break;
            case SCRCMD_NOT:
                SCRCMD_NOT_507750();
                break;
            case SCRCMD_GOSUB:
                sub_506B30();
                break;
            case SCRCMD_START_EXEC:
                SCRCMD_START_EXEC_5078D0();
                break;
            case SCRCMD_STOP_EXEC:
                SCRCMD_STOP_EXEC_5079A0();
                break;
            case SCRCMD_S_PLUS_S:
            case SCRCMD_S_MINUS_S:
            case SCRCMD_S_LESS_S:
            case SCRCMD_S_LEQUAL_S:
            case SCRCMD_S_GREATER_S:
            case SCRCMD_S_GEQUAL_S:
            case SCRCMD_S_EQUAL_S:
                sub_506ED0();
                break;
            case SCRCMD_I_PLUS_S:
            case SCRCMD_I_MINUS_S:
                sub_507110();
                break;
            case SCRCMD_S_PLUS_I:
            case SCRCMD_S_MINUS_I:
            case SCRCMD_S_LESS_I:
            case SCRCMD_S_LEQUAL_I:
            case SCRCMD_S_GREATER_I:
            case SCRCMD_S_GEQUAL_I:
            case SCRCMD_S_EQUAL_I:
                sub_506D60();
                break;
            case SCRCMD_INCREMENT:
                SCRCMD_INCREMENT_507A70();
                break;
            case SCRCMD_DECREMENT:
                SCRCMD_DECREMENT_507B50();
                break;
            case SCRCMD_CREATE_OBJ_2D:
            case SCRCMD_CREATE_OBJ_3D:
            case SCRCMD_CREATE_OBJ_3D_s32:
            case SCRCMD_CREATE_OBJ_2D_s32:
            case SCRCMD_CREATE_OBJ_3D_STR:
            case SCRCMD_CREATE_OBJ_2D_STR:
                sub_507CE0();
                break;
            case SCRCMD_CREATE_CAR_2D:
            case SCRCMD_CREATE_CAR_3D:
            case SCRCMD_CREATE_CAR_2D_STR:
            case SCRCMD_CREATE_CAR_3D_STR:
            case SCRCMD_CREATE_GANG_CAR1:
            case SCRCMD_CREATE_GANG_CAR2:
            case SCRCMD_CREATE_GANG_CAR3:
            case SCRCMD_CREATE_GANG_CAR4:
                sub_507F80();
                break;
            case SCRCMD_MAKE_CAR_DUMMY:
                SCRCMD_MAKE_CAR_DUMMY_508220();
                break;
            case SCRCMD_CREATE_CHAR_2D:
            case SCRCMD_CREATE_CHAR_3D:
                SCR_CMD_CREATE_CHAR_509730();
                break;
            case SCRCMD_SET_CAR_DENSITY:
            case SCRCMD_SET_GOOD_CAR:
            case SCRCMD_SET_BAD_CAR:
            case SCRCMD_SET_POLICE_CAR:
            case SCRCMD_SET_PED_DENSITY:
            case SCRCMD_SET_MUGGER:
            case SCRCMD_SET_CARTHIEF:
            case SCRCMD_SET_ELVIS:
            case SCRCMD_SET_GANG:
            case SCRCMD_SET_POLICE_PED:
            case SCRCMD_SET_GANGCARRATIO:
                sub_508280();
                break;
            case SCRCMD_POINT_ARROW_3D:
            case SCRCMD_LEVEL_END_ARROW2:
                sub_508550();
                break;
            case SCRCMD_ARROW_COLOUR:
                SCRCMD_ARROW_COLOUR_508DC0();
                break;
            case SCRCMD_POINT_ARROW_AT:
            case SCRCMD_LEVEL_END_ARROW1:
                sub_5086F0();
                break;
            case SCRCMD_REMOVE_ARROW:
                SCRCMD_REMOVE_ARROW_508E80();
                break;
            case SCRCMD_CHECK_CAR_DAMAGE:
                SCRCMD_CHECK_CAR_DAMAGE_508F00();
                break;
            case SCRCMD_CHECK_HEALTH:
                SCRCMD_CHECK_HEALTH_509030();
                break;
            case SCRCMD_STORE_CAR_INFO:
                SCRCMD_STORE_CAR_INFO_509180();
                break;
            case SCRCMD_DISPLAY_MESSAGE:
            case SCRCMD_DISPLAY_BRIEF:
            case SCRCMD_DISPLAY_BRIEF_NOW:
            case SCRCMD_DISPLAY_BRIEF_SOON:
                sub_5093C0();
                break;
            case SCRCMD_DISPLAY_TIMER:
                SCRCMD_DISPLAY_TIMER_5096E0();
                break;
            case SCRCMD_SET_CHAR_OBJ1:
                sub_5097D0();
                break;
            case SCRCMD_IS_CHAR_OBJ_PASS:
                sub_509810();
                break;
            case SCRCMD_IS_CHAR_OBJ_FAIL:
                sub_509880();
                break;
            case SCRCMD_GIVE_WEAPON1:
            case SCRCMD_GIVE_WEAPON2:
                sub_5098E0();
                break;
            case SCRCMD_IS_CAR_IN_BLOCK:
                sub_509990();
                break;
            case SCRCMD_HAS_CHAR_DIED:
                SCRCMD_HAS_CHAR_DIED_509BB0();
                break;
            case SCRCMD_IS_CHAR_IN_CAR:
                sub_509C10();
                break;
            case SCRCMD_IS_CHAR_IN_MODEL:
                sub_509C90();
                break;
            case SCRCMD_IS_CHAR_IN_ANY_CAR:
                sub_509D00();
                break;
            case SCRCMD_DELETE_ITEM:
                sub_509D60();
                break;
            case SCRCMD_ADD_SCORE1:
            case SCRCMD_ADDSCORE_NO_MULT:
                sub_509D90();
                break;
            case SCRCMD_IS_CHAR_STOPPED:
                sub_509E70();
                break;
            case SCRCMD_EXPLODE:
            case SCRCMD_EXPLODE_LARGE2:
            case SCRCMD_EXPLODE_SMALL2:
            case SCRCMD_EXPLODE_NO_RING2:
                sub_509ED0();
                break;
            case SCRCMD_LOCATE_CHAR_ANY:
            case SCRCMD_LOCATE_CHAR_ONFOOT:
            case SCRCMD_LOCATE_CHAR_BY_CAR:
            case SCRCMD_STOP_LOCATE_CHAR_ANY:
            case SCRCMD_STOP_LOCATE_CHAR_FOOT:
            case SCRCMD_STOP_LOCATE_CHAR_CAR:
                sub_509FD0();
                break;
            case SCRCMD_SET_CHAR_OBJ2:
                sub_50A200();
                break;
            case SCRCMD_SET_CHAR_OBJ3:
                sub_50A3E0();
                break;
            case SCRCMD_SET_THREAT_SEARCH:
            case SCRCMD_SET_THREAT_REACT:
            case SCRCMD_ADD_GROUP:
            case SCRCMD_SET_CHAR_SHOOT:
            case SCRCMD_SET_CHAR_BRAVERY:
                sub_50BBD0();
                break;
            case SCRCMD_CHANGE_CAR_REMAP:
                SCRCMD_CHANGE_CAR_REMAP_50A570();
                break;
            case SCRCMD_CHANGE_CHAR_REMAP:
                SCRCMD_CHANGE_CHAR_REMAP_50A5B0();
                break;
            case SCRCMD_CHECK_CAR_MODEL:
                SCRCMD_CHECK_CAR_MODEL_50A610();
                break;
            case SCRCMD_CHECK_CAR_REMAP:
                SCRCMD_CHECK_CAR_REMAP_50A670();
                break;
            case SCRCMD_CHECK_CAR_BOTH:
                SCRCMD_CHECK_CAR_BOTH_50A6E0();
                break;
            case SCRCMD_IS_ITEM_ONSCREEN:
            case SCRCMD_ONSCREEN_ACCURACY:
                sub_50A760();
                break;
            case SCRCMD_DELAY_HERE:
                SCRCMD_DELAY_HERE_50A940();
                break;
            case SCRCMD_DELAY:
                sub_50A980();
                break;
            case SCRCMD_ENABLE_THREAD:
                SCRCMD_ENABLE_THREAD_50ABF0();
                break;
            case SCRCMD_DISABLE_THREAD:
                SCRCMD_DISABLE_THREAD_50ABC0();
                break;
            case SCRCMD_SET_GANG_RESPECT:
                SCRCMD_SET_GANG_KILL_REACTION_50AC20();
                break;
            case SCRCMD_SET_CHAR_RESPECT:
            case SCRCMD_CHANGE_RESPECT:
            case SCRCMD_CHANGE_GANG_RESP:
                sub_50ACF0();
                break;
            case SCRCMD_CHECK_RESPECT_GREATER:
            case SCRCMD_CHECK_RESPECT_LESS:
            case SCRCMD_CHECK_RESPECT_IS:
                sub_50AEF0();
                break;
            case SCRCMD_ADD_PATROL_POs32:
                sub_50B0E0();
                break;
            case SCRCMD_SET_AMBIENT:
                SCRCMD_SET_AMBIENT_50B150();
                break;
            case SCRCMD_ANSWER_PHONE:
                sub_50B180();
                break;
            case SCRCMD_CHECK_PHONE:
                SCRCMD_CHECK_PHONE_50B230();
                break;
            case SCRCMD_CHECK_PHONETIMER:
                SCRCMD_CHECK_PHONETIMER_50B2C0();
                break;
            case SCRCMD_STOP_PHONE_RING:
                SCRCMD_STOP_PHONE_RING_50B360();
                break;
            case SCRCMD_EXPLODE_BUILDING:
                SCRCMD_EXPLODE_BUILDING_509F60();
                break;
            case SCRCMD_IS_CHAR_FIRE_ONSCREEN:
                sub_50B3D0();
                break;
            case SCRCMD_CAR_DRIVE_AWAY:
                SCRCMD_CAR_DRIVE_AWAY_50B440();
                break;
            case SCRCMD_CHAR_TO_DRIVE_CAR:
            case SCRCMD_CHAR_TO_BACKDOOR:
                sub_50B4F0();
                break;
            case SCRCMD_DRIVER_OUT_CAR:
                sub_50B5A0();
                break;
            case SCRCMD_GIVE_DRIVER_BRAKE:
                sub_50B600();
                break;
            case SCRCMD_SET_GANG_INFO1:
                SCRCMD_SET_GANG_INFO1_50B670();
                break;
            case SCRCMD_CLEAR_TIMERS:
                sub_50B690();
                break;
            case SCRCMD_CHECK_SCORE:
                SCRCMD_CHECK_SCORE_50B6F0();
                break;
            case SCRCMD_GET_SCORE:
                SCRCMD_GET_SCORE_50B760();
                break;
            case SCRCMD_IS_CHAR_IN_GANG:
                SCRCMD_IS_CHAR_IN_GANG_50B7D0();
                break;
            case SCRCMD_SET_NO_COLLIDE:
                sub_50B8B0();
                break;
            case SCRCMD_IS_CHAR_FIRING_AREA:
                SCRCMD_IS_CHAR_FIRING_AREA_50B910();
                break;
            case SCRCMD_GET_PASSENGER_NUM:
                SCRCMD_GET_PASSENGER_NUM_50B9C0();
                break;
            case SCRCMD_CLEAR_WANTED_LEVEL:
                sub_50BA30();
                break;
            case SCRCMD_CAR_WRECK_IN_LOCATION:
                SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0();
                break;
            case SCRCMD_SEND_CAR_TO_BLOCK:
                sub_50BB80();
                break;
            case SCRCMD_CHECK_NUM_ALIVE:
                SCRCMD_CHECK_NUM_ALIVE_50BC60();
                break;
            case SCRCMD_ADD_CHAR_TO_GROUP:
            case SCRCMD_REMOVE_char_type:
            case SCRCMD_MAKE_LEADER:
                sub_50BD10();
                break;
            case SCRCMD_SET_COUNTER_s32:
                sub_50BDC0();
                break;
            case SCRCMD_HAS_CAR_WEAPON:
                SCRCMD_HAS_CAR_WEAPON_50BE00();
                break;
            case SCRCMD_IS_CHAR_HORN:
                SCRCMD_IS_CHAR_HORN_50BE70();
                break;
            case SCRCMD_CHECK_MAX_PASS:
                SCRCMD_CHECK_MAX_PASS_50BED0();
                break;
            case SCRCMD_IS_CHAR_IN_ZONE:
                SCRCMD_IS_CHAR_IN_ZONE_50BF40();
                break;
            case SCRCMD_SET_PHONE_DEAD:
                SCRCMD_SET_PHONE_DEAD_50C040();
                break;
            case SCRCMD_IS_TRAILER_ATT:
                SCRCMD_IS_TRAILER_ATT_50C0E0();
                break;
            case SCRCMD_IS_CAR_ON_TRAIL:
                SCRCMD_IS_CAR_ON_TRAIL_50C1B0();
                break;
            case SCRCMD_ENABLE_CRANE:
            case SCRCMD_DISABLE_CRANE:
                SCRCMD_ENABLE_DISABLE_CRANE_50C230();
                break;
            case SCRCMD_CAR_GOT_DRIVER:
                SCRCMD_CAR_GOT_DRIVER_50C2A0();
                break;
            case SCRCMD_SPOTTED_PLAYER:
                SCRCMD_SPOTTED_PLAYER_50C2F0();
                break;
            case SCRCMD_GET_LAST_PUNCHED:
                SCRCMD_GET_LAST_PUNCHED_50C350();
                break;
            case SCRCMD_IS_CHAR_STUNNED:
                SCRCMD_IS_CHAR_STUNNED_50C3B0();
                break;
            case SCRCMD_KILL_ALL_PASSENG:
                SCRCMD_KILL_ALL_PASSENG_50C410();
                break;
            case SCRCMD_IS_GROUP_IN_CAR:
                SCRCMD_IS_GROUP_IN_CAR_50C470();
                break;
            case SCRCMD_PUNCHED_SOMEONE:
                SCRCMD_PUNCHED_SOMEONE_50C4E0();
                break;
            case SCRCMD_SET_MIN_ALIVE:
                SCRCMD_SET_MIN_ALIVE_50BCD0();
                break;
            case SCRCMD_ADD_CHAR_TO_GANG:
                SCRCMD_ADD_CHAR_TO_GANG_50C540();
                break;
            case SCRCMD_EXPLODE_ITEM:
            case SCRCMD_EXPLODE_LARGE1:
            case SCRCMD_EXPLODE_SMALL1:
            case SCRCMD_EXPLODE_NO_RING1:
                sub_50C5A0();
                break;
            case SCRCMD_PARK:
            case SCRCMD_PARK_NO_RESPAWN:
                sub_50C6F0();
                break;
            case SCRCMD_ALT_WANTED_LEVEL:
            case SCRCMD_ALTER_WANTED_LEVEL:
                sub_50BA70();
                break;
            case SCRCMD_BEEN_PUNCHED_BY:
                SCRCMD_BEEN_PUNCHED_BY_50C760();
                break;
            case SCRCMD_UPDATE_DOOR:
                SCRCMD_UPDATE_DOOR_50C7D0();
                break;
            case SCRCMD_OPEN_DOOR:
            case SCRCMD_CLOSE_DOOR:
            case SCRCMD_SET_DOOR_AUTO:
            case SCRCMD_SET_DOOR_MANUAL:
                SCRCMD_DOOR_50C8A0();
                break;
            case SCRCMD_REMOVE_WEAPON:
                SCRCMD_REMOVE_WEAPON_50C990();
                break;
            case SCRCMD_REMOVE_BLOCK:
                SCRCMD_REMOVE_BLOCK_50C9F0();
                break;
            case SCRCMD_LOWER_LEVEL:
                SCRCMD_LOWER_LEVEL_50CA30();
                break;
            case SCRCMD_ADD_NEW_BLOCK:
                SCRCMD_ADD_NEW_BLOCK_50CAB0();
                break;
            case SCRCMD_CHANGE_BLOCK_SIDE:
            case SCRCMD_CHANGE_BLOCK_LID:
            case SCRCMD_CHANGE_BLOCK_TYPE:
                sub_50CA70();
                break;
            case SCRCMD_ROAD_ON_OFF:
                SCRCMD_ROAD_ON_OFF_50CB20();
                break;
            case SCRCMD_GET_CAR_FROM_CRANE:
                SCRCMD_GET_CAR_FROM_CRANE_50B470();
                break;
            case SCRCMD_SWITCH_GENERATOR1:
            case SCRCMD_SWITCH_GENERATOR2:
                sub_50CCB0();
                break;
            case SCRCMD_SWITCH_GENERATOR3:
            case SCRCMD_SWITCH_GENERATOR4:
                sub_50CD30();
                break;
            case SCRCMD_CAR_DAMAGE_POS:
                SCRCMD_CAR_DAMAGE_POS_50CDB0();
                break;
            case SCRCMD_PARK_FINISHED:
                SCRCMD_PARK_FINISHED_50CE10();
                break;
            case SCRCMD_CAR_IN_AREA:
                SCRCMD_CAR_IN_AREA_509A70();
                break;
            case SCRCMD_REMOTE_CONTROL:
                SCRCMD_REMOTE_CONTROL_50D200();
                break;
            case SCRCMD_LAUNCH_MISSION:
                SCRCMD_LAUNCH_MISSION_50D2E0();
                break;
            case SCRCMD_RETURN:
            case SCRCMD_MISSIONEND:
                sub_506B80();
                break;
            case SCRCMD_PHONE_TEMPLATE:
                SCRCMD_PHONE_TEMPLATE_50CE90();
                break;
            case SCRCMD_SAVE_GAME:
                SCRCMD_SAVE_GAME_50D340();
                break;
            case SCRCMD_CHANGE_CAR_LOCK:
                SCRCMD_CHANGE_CAR_LOCK_50D680();
                break;
            case SCRCMD_CHANGE_COLOUR:
                SCRCMD_CHANGE_COLOUR_50D900();
                break;
            case SCRCMD_CHANGE_INTENSITY:
                SCRCMD_CHANGE_INTENSITY_50D870();
                break;
            case SCRCMD_CHANGE_RADIUS:
                SCRCMD_CHANGE_RADIUS_50D9A0();
                break;
            case SCRCMD_CREATE_LIGHT1:
            case SCRCMD_LIGHT_DECSET2:
                sub_50DA50();
                break;
            case SCRCMD_CHECK_NUM_LIVES:
            case SCRCMD_CHECK_MULT:
            case SCRCMD_ADD_LIVES:
            case SCRCMD_ADD_MULTIPLIER:
                sub_50DB70();
                break;
            case SCRCMD_GET_NUM_LIVES:
            case SCRCMD_GET_MULT:
                sub_50DD00();
                break;
            case SCRCMD_SET_DIR_OF_TVVAN:
                SCRCMD_SET_DIR_OF_TVVAN_50DD90();
                break;
            case SCRCMD_POINT_ONSCREEN:
                SCRCMD_POINT_ONSCREEN_50DE00();
                break;
            case SCRCMD_CHAR_IN_AIR:
                SCRCMD_CHAR_IN_AIR_50DE50();
                break;
            case SCRCMD_CHAR_SUNK:
                SCRCMD_CHAR_SUNK_50DEB0();
                break;
            case SCRCMD_START_BONUS1:
            case SCRCMD_START_BONUS2:
            case SCRCMD_START_BONUS3:
            case SCRCMD_START_BONUS4:
                sub_50DF10();
                break;
            case SCRCMD_CHECK_BONUS1:
            case SCRCMD_CHECK_BONUS2:
            case SCRCMD_CHECK_BONUS3:
                sub_50E0B0();
                break;
            case SCRCMD_SETUP_MODEL_CHECK:
                sub_50E120();
                break;
            case SCRCMD_MODEL_CHECK:
                sub_50E150();
                break;
            case SCRCMD_GET_CAR_SPEED:
            case SCRCMD_GET_CHAR_CAR_SPEED:
            case SCRCMD_GET_MAX_SPEED:
                sub_50E190();
                break;
            case SCRCMD_CHECK_CAR_SPEED:
                SCRCMD_CHECK_CAR_SPEED_50E360();
                break;
            case SCRCMD_SET_CAR_GRAPHIC:
                sub_50E460();
                break;
            case SCRCMD_S_IS_S_MINUS_S:
            case SCRCMD_S_IS_S_PLUS_S:
            case SCRCMD_S_IS_S_DIV_S:
            case SCRCMD_S_IS_S_MULT_S:
            case SCRCMD_S_IS_S_MOD_S:
                sub_50E4F0();
                break;
            case SCRCMD_S_IS_S_MINUS_I:
            case SCRCMD_S_IS_S_PLUS_I:
            case SCRCMD_S_IS_S_DIV_I:
            case SCRCMD_S_IS_S_MULT_I:
            case SCRCMD_S_IS_S_MOD_I:
                sub_50E610();
                break;
            case SCRCMD_GIVE_CAR_ALARM:
                sub_50E7F0();
                break;
            case SCRCMD_CAR_BULLETPROOF:
            case SCRCMD_CAR_ROCKETPROOF:
            case SCRCMD_CAR_FLAMEPROOF:
                sub_50E820();
                break;
            case SCRCMD_CHAR_DRIVE_AGGR:
                sub_50E730();
                break;
            case SCRCMD_CHAR_DRIVE_SPEED:
            case SCRCMD_SET_RUN_SPEED:
                sub_50E780();
                break;
            case SCRCMD_SET_CHAR_OBJ_FOLLOW:
                sub_50A460();
                break;
            case SCRCMD_PUT_CAR_ON_TRAILER:
                sub_50E900();
                break;
            case SCRCMD_CLEAR_BRIEFS:
                sub_50E9A0();
                break;
            case SCRCMD_CHECK_HEADS:
                sub_50E9E0();
                break;
            case SCRCMD_FINISH_LEVEL:
                sub_50EA40();
                break;
            case SCRCMD_CHECK_WEAPONHIT:
                sub_50EB00();
                break;
            case SCRCMD_WEAP_HIT_CAR:
            case SCRCMD_ANY_WEAPON_HIT_CAR:
                sub_50EBD0();
                break;
            case SCRCMD_IS_CHAR_ON_FIRE:
                sub_50ECE0();
                break;
            case SCRCMD_BRIEF_ONSCREEN:
                sub_50ED40();
                break;
            case SCRCMD_CREATE_SOUND:
                sub_50ED80();
                break;
            case SCRCMD_DO_EASY_PHONE:
                sub_50EDC0();
                break;
            case SCRCMD_CHAR_INTO_CAR:
                SCRCMD_CHAR_INTO_CAR_50F060();
                break;
            case SCRCMD_ADD_SCORE2:
                SCRCMD_ADD_SCORE2_509E00();
                break;
            case SCRCMD_DECIDE_POWERUP:
                SCRCMD_DECIDE_POWERUP_50F150();
                break;
            case SCRCMD_CHAR_ARRESTED:
                SCRCMD_CHAR_ARRESTED_50E4A0();
                break;
            case SCRCMD_SUPPRESS_MODEL:
                SCRCMD_SUPPRESS_MODEL_50F220();
                break;
            case SCRCMD_WARP_char_type:
                sub_50F270();
                break;
            case SCRCMD_SET_GROUP_TYPE:
                SCRCMD_SET_GROUP_TYPE_50F3D0();
                break;
            case SCRCMD_CHAR_DO_NOTHING:
                SCRCMD_CHAR_DO_NOTHING_50F410();
                break;
            case SCRCMD_EMERG_LIGHTS:
                SCRCMD_EMERG_LIGHTS_50F450();
                break;
            case SCRCMD_CHECK_OBJ_MODEL:
                SCRCMD_CHECK_OBJ_MODEL_50F4D0();
                break;
            case SCRCMD_PED_GRAPHIC:
                SCRCMD_PED_GRAPHIC_50F550();
                break;
            case SCRCMD_CHAR_INVINCIBLE:
            case SCRCMD_SET_STAY_IN_CAR:
            case SCRCMD_SET_USE_CAR_WEAPON:
            case SCRCMD_DEATH_ARR_STATE:
                sub_50F5E0();
                break;
            case SCRCMD_LOC_SECOND_char_type:
            case SCRCMD_LOC_SEC_CHAR_CAR:
            case SCRCMD_LOC_SEC_CHAR_ANY:
                sub_50F7B0();
                break;
            case SCRCMD_STOP_CAR_DRIVE:
                SCRCMD_STOP_CAR_DRIVE_50F900();
                break;
            case SCRCMD_IS_BUS_FULL:
                SCRCMD_IS_BUS_FULL_50F940();
                break;
            case SCRCMD_NO_CHARS_OFF_BUS:
                SCRCMD_NO_CHARS_OFF_BUS_50F9B0();
                break;
            case SCRCMD_KILL_char_type:
                sub_50FA00();
                break;
            case SCRCMD_SET_SHADING_LEV:
                SCRCMD_SET_SHADING_LEV_50FA40();
                break;
            case SCRCMD_SET_CAR_JAMMED:
                SCRCMD_SET_CAR_JAMMED_50FA70();
                break;
            case SCRCMD_FINISH_MISSION:
                SCRCMD_FINISH_MISSION_50FAD0();
                break;
            case SCRCMD_STORE_BONUS:
                sub_50FAF0();
                break;
            case SCRCMD_SET_ENTER_STATUS:
            case SCRCMD_SET_ALL_CONTROLS:
                sub_50FB60();
                break;
            case SCRCMD_SET_FAV_CAR:
                sub_50FC20();
                break;
            case SCRCMD_GROUP_IN_AREA:
                sub_50FC60();
                break;
            case SCRCMD_SET_CHAR_OCCUPATION:
                sub_509790();
                break;
            case SCRCMD_SET_KF_WEAPON:
                sub_50FE00();
                break;
            case SCRCMD_CLEAR_KF_WEAPON:
                sub_50FED0();
                break;
            case SCRCMD_ADD_ONSCREEN_COUNTER:
                sub_50FF50();
                break;
            case SCRCMD_CLEAR_COUNTER:
            case SCRCMD_CLEAR_CLOCK_ONLY:
                sub_50FFB0();
                break;
            case SCRCMD_CHECK_CAR_DRIVER:
            case SCRCMD_IS_CAR_WRECKED:
            case SCRCMD_CAR_SUNK:
            case SCRCMD_CAR_IN_AIR:
            case SCRCMD_IS_CAR_CRUSHED:
            case SCRCMD_CARBOMB_ACTIVE:
            case SCRCMD_EMERG_LIGHTS_ON:
                sub_50CB70();
                break;
            case SCRCMD_CHANGE_POLICE:
                sub_510030();
                break;
            case SCRCMD_DESTROY_GROUP:
                sub_510050();
                break;
            case SCRCMD_CHECK_CURRENT_WEAPON:
                sub_510090();
                break;
            case SCRCMD_MAKE_MUGGERS:
                SCRCMD_MAKE_MUGGERS_50F770();
                break;
            case SCRCMD_START_BASIC_KF:
                sub_510100();
                break;
            case SCRCMD_DO_BASIC_KF:
                sub_510280();
                break;
            case SCRCMD_SET_BONUS_RATING:
                sub_510530();
                break;
            case SCRCMD_ADD_TIME:
                sub_510560();
                break;
            case SCRCMD_DO_SAVE_GAME:
                sub_50D3C0();
                break;
            case SCRCMD_SET_MODEL_WANTED:
                sub_5105B0();
                break;
            case SCRCMD_CHECK_DEATH_ARR:
                sub_510600();
                break;
            case SCRCMD_FORCE_CLEANUP:
                sub_510660();
                break;
            case SCRCMD_SAVE_RESPECT:
            case SCRCMD_RESTORE_RESPECT:
                sub_510780();
                break;
            default:
                Next_503620(pCmd);
                break;
        }
    }
}

MATCH_FUNC(0x511840)
char_type miss2_0x11C::PoolUpdate()
{ //  return boolean: true if it has reached LEVELEND, false otherwise
    SCR_CMD_HEADER* BasePointer_512770;
    bool v4;
    SCR_CMD_HEADER* v5;

    //  I guess it's the script executed in mid-game on the current frame
    //  Normally each opcode is executed in one frame, unless there are WHILE_EXECs or EXECs blocks

    if (field_10 == 1)
    {
        return true;
    }

    BasePointer_512770 = gfrosty_pasteur_6F8060->GetBasePointer_512770(field_4_next_cmd);

    if (!BasePointer_512770)
    {
        sprintf(gTmpBuffer_67C598, "Miss2: accessing nonexistant mission line. Current uid: %d", field_4_next_cmd);
    }

    miss2_0x11C::sub_503200();

    if (BasePointer_512770->field_2_type == 0x3F) //  3F 00 = EXEC opcode
    {
        do
        {
            miss2_0x11C::PreExecOpCode_5108D0();
            gfrosty_pasteur_6F8060->GetBasePointer_512770(field_4_next_cmd);
        } while (field_12 > 0); //  execute opcodes in the same frame until an ENDEXEC
    }
    else //  It isn't an EXEC opcode
    {
        // field_6_return_value == 1  means it's in a WHILE_EXEC block
        if (BasePointer_512770->field_6_return_value == 1)
        {
            while (1) // execute commands in the same frame
            {
                v5 = gfrosty_pasteur_6F8060->GetBasePointer_512770(field_4_next_cmd);
                miss2_0x11C::PreExecOpCode_5108D0();

                //  If the script has reached a command out of the WHILE_EXEC block, or if it reaches LEVELEND
                if (v5->field_6_return_value != 1 || field_4_next_cmd == 0xFFFF)
                {
                    break;
                }

                //  Maybe if it reaches an ENDEXEC?
                if (field_12)
                {
                    break;
                }
            }
        }
        else
        {
            //  or else, execute only one command on this frame
            miss2_0x11C::PreExecOpCode_5108D0();
        }
    }

    v4 = field_4_next_cmd == 0xFFFF; // Did it reached the LEVELEND?
    field_12 = 0; // Clear EXEC flag?
    return v4;
}

MATCH_FUNC(0x511930)
void miss2_0x11C::sub_511930(char_type a2, u16 levelStart)
{
    field_4_next_cmd = levelStart;
    field_6 = a2;
    field_E = 0;
    field_C = 0;
    field_12 = 0;
    field_8 = false;
    field_10 = 0;
}

MATCH_FUNC(0x511960)
miss2_0x11C* miss2_0x11C::sub_511960(u16 levelStart)
{
    miss2_0x11C* v4 = miss2_0x11C_Pool_6F8064->sub_4767A0();
    v4->sub_511930(field_6, levelStart);
    return v4;
}

MATCH_FUNC(0x5119a0)
void miss2_0x11C::launch_mission_5119A0(SCR_CMD_HEADER* BasePointer, char_type* string)
{
    u16 cmd_next;
    gfrosty_pasteur_6F8060->sub_512400(string, &cmd_next);
    strcpy(field_14_str, string);
    miss2_0x11C::sub_50CE50(BasePointer, cmd_next);
}

MATCH_FUNC(0x511a00)
miss2_0x11C::miss2_0x11C()
{
    mpNext = 0;
    field_114 = new miss2_8();
    field_4_next_cmd = 0;
    field_6 = 0;
    field_8 = false;
    field_C = 0;
    field_E = 0;
    field_10 = 0;
    field_12 = 0;
    field_11A = 0;

    memset(field_14_str, 0, sizeof(field_14_str));

    field_118 = 0;
}

// https://decomp.me/scratch/gmYBd 
MATCH_FUNC(0x511cd0)
void miss2_0x11C::sub_511CD0()
{
    for (u16 i = 0; i < 25; i++)
    {
        if ((1 << i) & gGameSave_6F78C8.field_E4_car_and_script_data.field_4C)
        {
            SCR_START_BASIC_KF_TEMPLATE* pPtr = (SCR_START_BASIC_KF_TEMPLATE*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                    gfrosty_pasteur_6F8060->field_C1E74_basic_kf[i]);
            miss2_0x11C::DisableThread_505790(pPtr->field_8_triggername);
            miss2_0x11C::DeallocOrDeleteItem_505B10(pPtr->field_C_objname);
        }
    }
}

MATCH_FUNC(0x512fd0)
miss2_0x11C::~miss2_0x11C()
{
    if (field_114)
    {
        GTA2_DELETE_AND_NULL(field_114);
    }
    mpNext = 0;
}

STUB_FUNC(0x5131C0)
miss2_0x11C_Pool::~miss2_0x11C_Pool()
{
}
