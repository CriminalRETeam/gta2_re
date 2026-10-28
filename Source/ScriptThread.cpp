// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO kFpZero_6F77C0

// Force inline off
//#define INLINE_MODE inline
#include "bonus_event_type.hpp"
#include "car_thread_trigger.hpp"
#include "bonus_result.hpp"
#include "car_despawn_status.hpp"
#include "crane_home_target.hpp"
#include "explosion_type.hpp"
#include "hud_message_priority.hpp"
#include "hud_brief_priority.hpp"
#include "voice_line.hpp"
#include "objects_more.hpp"
#include "ScriptThread.hpp"
#include "CarAI.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Cranes.hpp"
#include "Crushers.hpp"
#include "DoorPool.hpp"
#include "ScriptStackFramePool.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Garage.hpp"
#include "garage_state.hpp"
#include "Generators.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "LightPool.hpp"
#include "MapRenderer.hpp"
#include "MissionCleanupList.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "PublicTransport.hpp"
#include "RouteFinder.hpp"
#include "WeaponManager.hpp"
#include "ang16.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "fix16.hpp"
#include "ScriptManager.hpp"
#include "gtx_0x106C.hpp"
#include "GameSession.hpp"
#include "map_0x370.hpp"
#include "ScriptGosubStack.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "TextTable.hpp"
#include "ped_death_cause.hpp"
#include "ped_graphic_type.hpp"
// Back to force inline
#define INLINE_MODE __forceinline

// GetEntityTypeOfCommand_503410
namespace script_entity_type
{
enum
{
    none_0 = 0,
    char_1 = 1,
    car_2 = 2,
    object_3 = 3,
    crane_4 = 4,
    conveyor_5 = 5,
    destructor_6 = 6,
    generator_7 = 7,
    light_8 = 8,
    radio_station_9 = 9,
    sound_10 = 10,
};
} // namespace script_entity_type

// GetOperationType_506BC0
namespace script_operation
{
enum
{
    add_0 = 0,
    subtract_1 = 1,
    greater_2 = 2,
    greater_or_equal_3 = 3,
    less_4 = 4,
    less_or_equal_5 = 5,
    equal_6 = 6,
    divide_7 = 7,
    multiply_8 = 8,
    modulo_9 = 9,
};
} // namespace script_operation

DEFINE_GLOBAL_INIT(s16, gNextThreadId_6212EE, 1, 0x6212EE);

DEFINE_GLOBAL_ARRAY_INIT(
    u8,
    kDecidePowerupGenTypes_6212F0,
    19,
    0x6212F0,
    0x41u COMMA 0x42u COMMA 0x43u COMMA 0x44u COMMA 0x45u COMMA 0x46u COMMA 0x47u COMMA 0x48u COMMA 0x49u COMMA 0x5Eu COMMA 0x5Fu COMMA 0x60u COMMA 0x61u COMMA 0x62u COMMA 0x63u COMMA 0x64u COMMA 0x65u COMMA 0x66u COMMA 0x67u);

DEFINE_GLOBAL(ScriptThreadPool*, miss2_0x11C_Pool_6F8064, 0x6F8064);
DEFINE_GLOBAL(SCR_POINTER*, gBasePtr_6F8070, 0x6F8070);
DEFINE_GLOBAL_INIT(Fix16, kFpOneAndHalf_6F76DC, Fix16(0x6000, 0), 0x6F76DC);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6F75F0, Fix16(0x2000, 0), 0x6F75F0);
DEFINE_GLOBAL(Fix16, kFpZero_6F77C0, 0x6F77C0);
DEFINE_GLOBAL_INIT(Ang16, kDegreesToAng16_6F8044, Ang16(0x4), 0x6F8044);
DEFINE_GLOBAL_INIT(Fix16, kFp255_6F7570, Fix16(255), 0x6F7570);
EXTERN_GLOBAL(Ang16, kAngZero_6F804C);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6F77C4, Fix16(1), 0x6F77C4);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6F77C8, Fix16(2), 0x6F77C8);

static inline bool is_car_weapon(s32& weapon_idx)
{
    return weapon_idx >= weapon_type::car_bomb && weapon_idx <= 27;
}

MATCH_FUNC(0x503200)
void ScriptThread::MissionFailOnDeathOrArrest_503200()
{
    if (gScriptManager_6F8060->field_355_death_arr_state && gScriptManager_6F8060->field_344_mission_flag &&
        *gScriptManager_6F8060->field_344_mission_flag == 1)
    {
        if (!gGame_0x40_67E008->field_38_local_player->IsWastedOrBustedNotObjective54_476700() &&
            !gGame_0x40_67E008->field_38_local_player->IsBustedObjective54_476730())
        {
            return;
        }

        field_114_gosub_stack->remove_5031E0(2);
        SCRCMD_RETURN_506B80();

        s32 msg_offset;
        if (gGame_0x40_67E008->field_38_local_player->IsWastedOrBustedNotObjective54_476700())
        {
            msg_offset = 0;
        }
        else
        {
            msg_offset = 5;
        }

        u16 msg_id = gRng_6F6784.get_uint8_4F7B70(5);

        if (gScriptManager_6F8060->field_348_gang_1_mission_flag && *gScriptManager_6F8060->field_348_gang_1_mission_flag == 1)
        {
            msg_id += gScriptManager_6F8060->field_356_gang_1_death_base + msg_offset;
        }
        else if (gScriptManager_6F8060->field_34C_gang_2_mission_flag && *gScriptManager_6F8060->field_34C_gang_2_mission_flag == 1)
        {
            msg_id += gScriptManager_6F8060->field_358_gang_2_death_base + msg_offset;
        }
        else if (gScriptManager_6F8060->field_350_gang_3_mission_flag && *gScriptManager_6F8060->field_350_gang_3_mission_flag == 1)
        {
            msg_id += gScriptManager_6F8060->field_35A_gang_3_death_base + msg_offset;
        }
        else
        {
            msg_id = 8001;
        }

        sprintf(gTmpBuffer_67C598, "%d", msg_id);

        gHud_2B00_706620->field_DC_brief.ClearAllBriefsWithPriority_5D4890(hud_brief_priority::normal_1);
        gHud_2B00_706620->field_DC_brief.ClearAllBriefsWithPriority_5D4890(hud_brief_priority::now_3);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("mfail"), hud_message_priority::important_3);
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::jobfail_23);

        *gScriptManager_6F8060->field_344_mission_flag = 0;
        gScriptManager_6F8060->field_C1E2E_death_arrest_flag = true;
        gGame_0x40_67E008->field_38_local_player->field_2C4_player_ped->field_26C_graphic_type = ped_graphic_type::character_1;
    }
}

MATCH_FUNC(0x503410)
u8 ScriptThread::GetEntityTypeOfCommand_503410(u32 opcode)
{
    switch (opcode)
    {
        case SCRCMD_CONVEYOR_DEC:
        case SCRCMD_CONVEYOR_DECSET1:
        case SCRCMD_CONVEYOR_DECSET2:
            return script_entity_type::conveyor_5;
        case SCRCMD_GENERATOR_DEC:
        case SCRCMD_GENERATOR_DECSET3:
        case SCRCMD_GENERATOR_DECSET4:
            return script_entity_type::generator_7;
        case SCRCMD_DESTRUCTOR_DEC:
        case SCRCMD_DESTRUCTOR_DECSET1:
        case SCRCMD_DESTRUCTOR_DECSET2:
            return script_entity_type::destructor_6;
        case SCRCMD_CRANE_DEC:
        case SCRCMD_CRANE_BASIC_DEC:
        case SCRCMD_CRANE_TARGET_DEC:
        case SCRCMD_CRANE2TARGET_DEC:
            return script_entity_type::crane_4;

        case SCRCMD_PLAYER_PED:
        case SCRCMD_CHAR_DEC:
        case SCRCMD_CHAR_DECSET_2D:
        case SCRCMD_CHAR_DECSET_3D:
        case SCRCMD_CREATE_CHAR_2D:
        case SCRCMD_CREATE_CHAR_3D:
            return script_entity_type::char_1;

        case SCRCMD_OBJ_DEC:
        case SCRCMD_OBJ_DECSET_2D:
        case SCRCMD_OBJ_DECSET_3D:
        case SCRCMD_OBJ_DECSET_2D_INT:
        case SCRCMD_OBJ_DECSET_3D_INT:
        case SCRCMD_OBJ_DECSET_2D_STR:
        case SCRCMD_OBJ_DECSET_3D_STR:
        case SCRCMD_CREATE_OBJ_2D:
        case SCRCMD_CREATE_OBJ_3D:
        case SCRCMD_CREATE_OBJ_3D_INT:
        case SCRCMD_CREATE_OBJ_2D_INT:
        case SCRCMD_CREATE_OBJ_3D_STR:
        case SCRCMD_CREATE_OBJ_2D_STR:
            return script_entity_type::object_3;

        case SCRCMD_LIGHT_DEC:
        case SCRCMD_LIGHT_DECSET1:
            return script_entity_type::light_8;

        case SCRCMD_CAR_DEC:
        case SCRCMD_CAR_DECSET_2D:
        case SCRCMD_CAR_DECSET_3D:
        case SCRCMD_PARKED_CAR_DECSET_2D:
        case SCRCMD_PARKED_CAR_DECSET_3D:
        case SCRCMD_PARKED_CAR_DECSET_2D_STR:
        case SCRCMD_PARKED_CAR_DECSET_3D_STR:
        case SCRCMD_CREATE_CAR_2D:
        case SCRCMD_CREATE_CAR_3D:
            return script_entity_type::car_2;

        case SCRCMD_RADIOSTATION_DEC:
            return script_entity_type::radio_station_9;

        case SCRCMD_SOUND:
        case SCRCMD_SOUND_DECSET:
        case SCRCMD_CREATE_SOUND:
            return script_entity_type::sound_10;
        default:
            return script_entity_type::none_0;
    }
}

MATCH_FUNC(0x5035b0)
void ScriptThread::PoolAllocate_5035B0()
{
    field_118_in_level_start = false;
    field_11A_thread_id = gNextThreadId_6212EE++;
}

MATCH_FUNC(0x5035d0)
void ScriptThread::Log_5035D0()
{
    if (gDo_miss_logging_67D6BC)
    {
        sprintf(gTmpBuffer_67C598, "%d:\t\t%d ", gpRng_67AB34->get_cur_rng_41CFE0(), static_cast<s16>(field_4_next_cmd));
        gMiss2Log_6F7698.Write_Log_4D9650(gTmpBuffer_67C598);
    }
}

DEFINE_GLOBAL(s32, gPrevCmdIdx_6F806C, 0x6F806C);

MATCH_FUNC(0x503610)
void ScriptThread::StopThread_503610()
{
    field_10_stopped = 1;
}

MATCH_FUNC(0x503620)
void ScriptThread::Next_503620(SCR_CMD_HEADER* pCmd)
{
    if (static_cast<u16>(pCmd->field_4_cmd_next) != 0xFFFF)
    { // FF FF (low endian) is the script terminator
        gPrevCmdIdx_6F806C = field_4_next_cmd;
        field_4_next_cmd = pCmd->field_4_cmd_next;
        field_C_cmd_initialised = 0;
    }
    else
    {
        ScriptThread::EndCmd_503670();
    }
}

MATCH_FUNC(0x503650)
void ScriptThread::JumpToCmd_503650(u16 cmdIdx)
{
    if (cmdIdx != 0xFFFF)
    {
        field_4_next_cmd = cmdIdx;
    }
    else
    {
        ScriptThread::EndCmd_503670();
    }
}

MATCH_FUNC(0x503670)
void ScriptThread::EndCmd_503670()
{
    field_4_next_cmd = 0xFFFFu;
}

MATCH_FUNC(0x503680)
void ScriptThread::SCRCMD_OBJ_DECSET_2D_3D_503680(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->field_C_pos.z == kFp255_6F7570)
    {
        pCmd->field_C_pos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_pos.x, pCmd->field_C_pos.y);
    }

    if (pCmd->field_18_obj_id < 0xC8u || pCmd->field_18_obj_id > 0xF4u)
    {
        Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->field_1A_rot.rValue)));

        pPointer->field_8_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(pCmd->field_18_obj_id,
                                                                        pCmd->field_C_pos.x,
                                                                        pCmd->field_C_pos.y,
                                                                        pCmd->field_C_pos.z,
                                                                        rotation);
    }
    else
    {
        Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->field_1A_rot.rValue)));

        pPointer->field_8_obj = gObject_5C_6F8F84->NewPhysicsObjUnlessBlocked_529BC0(pCmd->field_18_obj_id,
                                                              pCmd->field_C_pos.x,
                                                              pCmd->field_C_pos.y,
                                                              pCmd->field_C_pos.z,
                                                              rotation);
    }

    if (pPointer->field_8_obj != NULL)
    {
        if (pPointer->field_8_obj->IsModel176To181_475AA0())
        {
            gHud_2B00_706620->field_1F18_arrows.place_gang_phone_5D1110(pPointer->field_8_obj);
            for (u8 i = 0; i < 0x1Fu; i++)
            {
                if (gScriptManager_6F8060->field_C1E32_phone_ids[i] == 0)
                {
                    gScriptManager_6F8060->field_C1E32_phone_ids[i] = pPointer->field_0_cmd_this;
                    break;
                }
            }
        }
    }
}

MATCH_FUNC(0x5038d0)
void ScriptThread::SCRCMD_OBJ_DECSET_5038D0(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->field_C_pos.z == kFp255_6F7570)
    {
        pCmd->field_C_pos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_pos.x, pCmd->field_C_pos.y);
    }

    if (pCmd->field_18_obj_id < 0xC8u || pCmd->field_18_obj_id > 0xF4u)
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->field_1A_rot);
        rotation.Normalize_406C20();

        pPointer->field_8_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(pCmd->field_18_obj_id,
                                                                        pCmd->field_C_pos.x,
                                                                        pCmd->field_C_pos.y,
                                                                        pCmd->field_C_pos.z,
                                                                        rotation);
    }
    else
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->field_1A_rot);
        rotation.Normalize_406C20();

        pPointer->field_8_obj = gObject_5C_6F8F84->NewPhysicsObjUnlessBlocked_529BC0(pCmd->field_18_obj_id,
                                                              pCmd->field_C_pos.x,
                                                              pCmd->field_C_pos.y,
                                                              pCmd->field_C_pos.z,
                                                              rotation);
    }

    if (pPointer->field_8_obj != NULL)
    {
        Object* pObj;

        if (pPointer->field_8_obj->IsStaticCollisionCheck_475A70())
        {
            pPointer->field_8_obj->set_field_26(pCmd->field_1C_value_shop_type);
        }
        else if (pObj = pPointer->field_8_obj, pObj->IsPickup_421060())
        {
            pObj->set_field_26(pCmd->field_1C_value_shop_type);
        }
        else
        {
            if (pPointer->field_8_obj->IsShop_475A60())
            {
                pPointer->field_8_obj->set_field_26(pCmd->field_1C_value_shop_type);
            }
        }
    }
}

MATCH_FUNC(0x503a20)
void ScriptThread::SCRCMD_PLAYER_PED_503A20(SCR_PLAYER_PED* pCmd)
{
    Player* pPlayer = gGame_0x40_67E008->GetFirstPlayerWithoutPed_4B9750();

    if (pPlayer)
    {
        Ped* pPed;
        if (gScriptManager_6F8060->get_field_C1E2C_475A20())
        {
            Fix16 saved_y = kFpOneAndHalf_6F76DC + gGameSave_6F78C8.field_54_player_and_world_stats.field_4_y.GetFloorValue();
            Fix16 saved_x = kFpHalf_6F75F0 + gGameSave_6F78C8.field_54_player_and_world_stats.field_0_x.GetFloorValue();

            pPed = gPedManager_6787BC->SpawnPedAt(saved_x,
                                                  saved_y,
                                                  gGameSave_6F78C8.field_54_player_and_world_stats.field_8_z,
                                                  gGameSave_6F78C8.field_54_player_and_world_stats.field_7F_player_ped_remap,
                                                  kAngZero_6F804C);
        }
        else
        {
            if (pCmd->field_C_pos.z == kFp255_6F7570) //  kFp255_6F7570 is 255.0
            {
                //  Calculate the real Z position at (X,Y) based on the map
                pCmd->field_C_pos.z =
                    gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_pos.x, pCmd->field_C_pos.y);
            }

            Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->field_18_rot.rValue)));

            pPed = gPedManager_6787BC->SpawnPedAt(pCmd->field_C_pos.x,
                                                  pCmd->field_C_pos.y,
                                                  pCmd->field_C_pos.z,
                                                  static_cast<u8>(pCmd->field_1A_remap),
                                                  rotation);
        }

        if (pPed != NULL)
        {
            pPed->SetPedType_403920(ped_type::player_2);
            if (!gScriptManager_6F8060->get_field_C1E2C_475A20())
            {
                pPed->set_health_4039A0(100);
            }

            pPlayer->InitPlayerPed_565490(pPed);
            pPed->field_26C_graphic_type = ped_graphic_type::character_1;
            pCmd->field_8_ped = pPed;

            Sprite* pSprite = pPed->GetSprite_46DF50();
            pSprite->ResolveCollisionWithCarPedOrObject_5A2A30();
        }
    }
}

MATCH_FUNC(0x503bc0)
void ScriptThread::SCRCMD_CAR_DECSET_503BC0(SCR_CAR_DATA_DEC* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->field_C_pos.z == kFp255_6F7570) //  255.0
    {
        pCmd->field_C_pos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_pos.x, pCmd->field_C_pos.y);
    }
    if (pCmd->field_1E_trailer_id == -1) //  No trailers
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->field_18_rot);
        rotation.Normalize_406C20();
        pPointer->field_8_car = gCarManager_6C_677930->SpawnCar_426E10(pCmd->field_C_pos.x,
                                                                pCmd->field_C_pos.y,
                                                                pCmd->field_C_pos.z,
                                                                rotation,
                                                                pCmd->field_1C_car_id);
    }
    else if (pCmd->field_1E_trailer_id == -2) //  Mini car
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->field_18_rot);
        rotation.Normalize_406C20();
        pPointer->field_8_car = gCarManager_6C_677930->SpawnCar_4764A0(pCmd->field_C_pos.x,
                                                                pCmd->field_C_pos.y,
                                                                pCmd->field_C_pos.z,
                                                                rotation,
                                                                pCmd->field_1C_car_id);
        if (pPointer->field_8_car != NULL)
        {
            pPointer->field_8_car->LockPermanently_475C30();
            pPointer->field_8_car->SetFlag4_476270();
            pPointer->field_8_car->UpdateDrawLayer_4435F0();
        }
    }
    else //  There is a trailer
    {
        if (pCmd->field_1C_car_id == car_model_enum::TRUKTRNS) // 66 = TRUKTRNS Truck Trailer, Flatbed
        {
            Car* pTrailerCar = gCarManager_6C_677930->SpawnCar_shortened(pCmd->field_1E_trailer_id);
            Ang16 rotation;
            rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->field_18_rot);
            rotation.Normalize();

            pPointer->field_8_car = gCarManager_6C_677930->SpawnCar_426E10(pCmd->field_C_pos.x,
                                                                    pCmd->field_C_pos.y,
                                                                    pCmd->field_C_pos.z,
                                                                    rotation,
                                                                    pCmd->field_1C_car_id);
            pPointer->field_8_car->field_50_car_sprite->AttachSprite_5A3100(pTrailerCar->field_50_car_sprite,
                                                                                      kFpZero_6F77C0,
                                                                                      kFpZero_6F77C0,
                                                                                      kAng180_6F771E);
            pTrailerCar->IncrementCarStats_443D70(car_kind::mission_car_8);
        }
        else
        {
            Ang16 rotation;
            rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->field_18_rot);
            rotation.Normalize();

            Trailer* pTrailer = gCarManager_6C_677930->SpawnCabAndTrailer_446530(pCmd->field_C_pos.x,
                                                                          pCmd->field_C_pos.y,
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
            pPointer->field_8_car->SetCarRemap(static_cast<u8>(pCmd->field_1A_remap));
        }

        Car* pCar = pPointer->field_8_car;
        pCar->SetDriverPedType_421560(ped_type::script_created_5);

        pPointer->field_8_car->LockoutThiefUnlessPermanent_475C40();
        pPointer->field_8_car->IncrementCarStats_443D70(car_kind::mission_car_8);
        pPointer->field_8_car->field_50_car_sprite->ResolveCollisionWithCarPedOrObject_5A2A30();

        if (pCmd->field_2_type >= static_cast<u32>(SCRCMD_CREATE_GANG_CAR1) && pCmd->field_2_type <= static_cast<u32>(SCRCMD_CREATE_GANG_CAR4))
        {
            s8 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pPointer->field_8_car->field_84_car_info_idx);
            if (gang_idx > -1)
            {
                Gang* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(gang_idx);
                pPointer->field_8_car->AttachGangIcon_440660(pGang->field_138_arrow_colour);
            }
        }
    }
}

MATCH_FUNC(0x503f80)
void ScriptThread::SCRCMD_PARKED_CAR_DECSET_503F80(SCR_POINTER* pCmd)
{
    ScriptThread::SCRCMD_CAR_DECSET_503BC0(reinterpret_cast<SCR_CAR_DATA_DEC*>(pCmd), pCmd);
    (pCmd->field_8_car)->ReassignCarStats_443EB0(car_kind::parked_car_9);
    Car* pCar = pCmd->field_8_car;
    pCar->field_7C_ped_type = ped_type::special_ped_4;
    pCar->field_76_last_seen_timer = 0;
}

MATCH_FUNC(0x503fb0)
void ScriptThread::SCRCMD_CHAR_DECSET_2D_3D_503FB0(SCR_CHAR_DATA_DEC* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->field_C_pos.z == kFp255_6F7570)
    {
        pCmd->field_C_pos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_pos.x, pCmd->field_C_pos.y);
    }

    Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->field_18_rot.rValue)));

    Ped* pPed = gPedManager_6787BC->SpawnPedAt(pCmd->field_C_pos.x,
                                          pCmd->field_C_pos.y,
                                          pCmd->field_C_pos.z,
                                          static_cast<u8>(pCmd->field_1A_remap),
                                          rotation);
    pPointer->field_8_char = pPed;

    if (pPed)
    {
        pPointer->field_8_char->SetPedType_403920(ped_type::script_created_5);
        pPointer->field_8_char->set_occupation_403970(pCmd->field_1C_occupation);
        pPointer->field_8_char->field_26C_graphic_type = ped_graphic_type::character_1;
        pPointer->field_8_char->SetObjective(objectives_enum::wait_on_foot_26);
        pPointer->field_8_char->set_health_4039A0(100);
        Sprite* pSprite = pPointer->field_8_char->GetSprite_46DF50();
        pSprite->ResolveCollisionWithCarPedOrObject_5A2A30();
    }
}

MATCH_FUNC(0x504110)
void ScriptThread::SetCharObjective_504110(SCR_CHAR_OBJECTIVE* pCmd, SCR_POINTER* pPointer)
{
    pPointer->field_8_char->SetObjective(pCmd->field_A_objective);
    pPointer->field_8_char->field_21C_bf.bScheduledForRemoval = false;
}

MATCH_FUNC(0x504150)
void ScriptThread::SCRCMD_MAP_ZONE_SET_504150(SCR_MAP_ZONE_SET* pCmd)
{
    str_table_entry* pZoneStr = gScriptManager_6F8060->FindStringById_503080(pCmd->field_0_cmd_this);
    gMap_0x370_6F6268->SetZoneInfo_4DF8C0(pZoneStr->field_2_zone_idx,
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
void ScriptThread::SCRCMD_ARROW_DEC_5041B0(SCR_TWO_PARAMS* pCmd)
{
    pCmd->field_8_u32 = 0;
}

MATCH_FUNC(0x5041c0)
void ScriptThread::SCRCMD_CRANE_5041C0(SCR_CRANE_TARGET_DEC* pTargetCmd, SCR_CRANE_BASIC_DEC* pBasicCmd)
{
    pBasicCmd->field_8_crane =
        gCranePool_D9C_679FD4->NewCrane_480EC0(pTargetCmd->field_10_pos.field_0_x, pTargetCmd->field_10_pos.field_4_y);

    if (pTargetCmd->field_2_type == SCRCMD_CRANE_TARGET_DEC)
    {
        if (pTargetCmd->field_1A_home_cranetype == crane_home_target::first_0)
        {
            Fix16 fix_rot = Fix16(kDegreesToAng16_6F8044.rValue) * Fix16(pTargetCmd->field_24_target_rotation);
            Ang16 rotation(&fix_rot, 0);

            pTargetCmd->field_8_crane->SetTarget1_480900(pTargetCmd->field_1C_target_pos.field_0_x,
                                                                     pTargetCmd->field_1C_target_pos.field_4_y,
                                                                     rotation);
        }
        else if (pTargetCmd->field_1A_home_cranetype == crane_home_target::second_1)
        {
            Fix16 fix_rot = Fix16(kDegreesToAng16_6F8044.rValue) * Fix16(pTargetCmd->field_24_target_rotation);
            Ang16 rotation(&fix_rot, 0);

            pTargetCmd->field_8_crane->SetTarget2_480B60(pTargetCmd->field_1C_target_pos.field_0_x,
                                                                     pTargetCmd->field_1C_target_pos.field_4_y,
                                                                     rotation);
        }
    }
    else if (pTargetCmd->field_2_type == SCRCMD_CRANE_BASIC_DEC)
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, reinterpret_cast<Ang16*>(&pBasicCmd->field_24_target_rotation));
        rotation.Normalize_406C20();
        pBasicCmd->field_8_crane->SetTarget1_480900(pBasicCmd->field_1C_target_pos.field_0_x,
                                                                pBasicCmd->field_1C_target_pos.field_4_y,
                                                                rotation);

        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, reinterpret_cast<Ang16*>(&pBasicCmd->field_26_second_rotation));
        rotation.Normalize_406C20();
        pBasicCmd->field_8_crane->SetTarget2_480B60(pBasicCmd->field_28_second_pos.field_0_x,
                                                                pBasicCmd->field_28_second_pos.field_4_y,
                                                                rotation);
    }

    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, reinterpret_cast<Ang16*>(&pTargetCmd->field_18_home_rotation));
        rotation.Normalize_406C20();
        pBasicCmd->field_8_crane->SetHomeRotation_4768E0(rotation);
    }

    if (pTargetCmd->field_C_homecrane)
    {
        SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pTargetCmd->field_C_homecrane));
        pBasicCmd->field_8_crane->SetHomeCrane_476900(pPointer->field_8_crane);
    }
}

MATCH_FUNC(0x5043a0)
void ScriptThread::SCRCMD_CONVEYOR_DECSET1_2_5043A0(SCR_CONVEYOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->field_C_rect.field_0_pos.z == kFp255_6F7570)
    {
        pCmd->field_C_rect.field_0_pos.z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_rect.field_0_pos.x,
                                                                                                 pCmd->field_C_rect.field_0_pos.y);
    }
    pPointer->field_8_obj = gObject_5C_6F8F84->NewTouchPoint_529950(objects::conveyor_139,
                                                                    pCmd->field_C_rect.field_0_pos.x,
                                                                    pCmd->field_C_rect.field_0_pos.y,
                                                                    pCmd->field_C_rect.field_0_pos.z,
                                                                    kAngZero_6F804C,
                                                                    pCmd->field_C_rect.field_C_size.field_0_x,
                                                                    pCmd->field_C_rect.field_C_size.field_4_y,
                                                                    kFpOne_6F77C4);

    pPointer->field_8_obj->SetConveyorSpeed_529030(pCmd->field_18_speed.field_0_x, pCmd->field_18_speed.field_1_y);
}

MATCH_FUNC(0x504420)
void ScriptThread::SCRCMD_GENERATOR_DECSET_504420(SCR_GENERATOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->field_C_pos.z == kFp255_6F7570)
    {
        pCmd->field_C_pos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_pos.x, pCmd->field_C_pos.y);
    }

    Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->field_18_rot.rValue)));

    pPointer->field_8_generator = gGeneratorPool_14AC_67E5D0->CreateGenerator_4C1DC0(pCmd->field_C_pos.x,
                                                                                     pCmd->field_C_pos.y,
                                                                                     pCmd->field_C_pos.z,
                                                                                     rotation,
                                                                                     pCmd->field_1A_obj_id,
                                                                                     pCmd->field_1C_mindelay,
                                                                                     pCmd->field_1E_maxdelay);
    if (pCmd->field_20_ammo > 0)
    {
        pPointer->field_8_generator->field_1C_ammo = pCmd->field_20_ammo;
    }
}

MATCH_FUNC(0x504530)
void ScriptThread::SCRCMD_DESTRUCTOR_DECSET_504530(SCR_DESTRUCTOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->field_C_rect.field_0_pos.z == kFp255_6F7570)
    {
        pCmd->field_C_rect.field_0_pos.z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_rect.field_0_pos.x,
                                                                                                 pCmd->field_C_rect.field_0_pos.y);
    }

    pPointer->field_8_obj = gObject_5C_6F8F84->NewTouchPoint_529950(objects::destructor_141,
                                                                    pCmd->field_C_rect.field_0_pos.x,
                                                                    pCmd->field_C_rect.field_0_pos.y,
                                                                    pCmd->field_C_rect.field_0_pos.z,
                                                                    kAngZero_6F804C,
                                                                    pCmd->field_C_rect.field_C_size.field_0_x,
                                                                    pCmd->field_C_rect.field_C_size.field_4_y,
                                                                    kFpOne_6F77C4);
}

MATCH_FUNC(0x5045a0)
void ScriptThread::SCRCMD_CRUSHER_BASIC_5045A0(SCR_CRUSHER_BASIC* pCmd, SCR_POINTER* pPointer)
{
    pPointer->field_8_crusher = gCrusherPool_94_67A830->CreateCrusher_488820(pCmd->field_C_pos.field_0_x, pCmd->field_C_pos.field_4_y);
}

MATCH_FUNC(0x5045d0)
void ScriptThread::SCRCMD_THREAD_DECLARE2_5045D0(SCR_THREAD* pCmd, s16* pThreadIdx)
{
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_10_char_var_idx));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_12_car_or_phone_idx));

    if (pCharPointer->field_8_char && pCarPointer->field_8_car)
    {
        gScriptManager_6F8060->AddCarThread_5128D0(pCharPointer->field_8_char->get_id(), pCarPointer->field_8_car->field_6C_car_id, *pThreadIdx);
        pCarPointer->field_8_car->SetCarThreadPending_476360();
    }
}

MATCH_FUNC(0x504660)
void ScriptThread::SCRCMD_THREAD_DECLARE3_504660(SCR_THREAD* pCmd)
{
    if (static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_10_char_idx))->field_8_char)
    {
        pCmd->field_15_trigger_idx = gCarThreadTriggers_214_705F20->AddThreadTrigger_5C86C0(car_thread_trigger_type::char_on_foot_4,
                                                     car_thread_trigger_action::spawn_thread_2,
                                                     pCmd,
                                                     Fix16(pCmd->field_12_x) + kFpHalf_6F75F0,
                                                     Fix16(pCmd->field_13_y) + kFpHalf_6F75F0,
                                                     pCmd->field_14_z,
                                                     kFpHalf_6F75F0,
                                                     kFpHalf_6F75F0);
        pCmd->field_16_trigger_added = 1;
    }
}

MATCH_FUNC(0x504710)
void ScriptThread::SCRCMD_THREAD_DECLARE5_504710(SCR_CHAR_AREA_ANY* pCmd)
{
    if (static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_24_char_idx))->field_8_char)
    {
        pCmd->field_27_trigger_added = 1;

        switch (pCmd->field_2_type)
        {
            case SCRCMD_THREAD_DECLARE5:
                pCmd->field_26_trigger_idx = gCarThreadTriggers_214_705F20->AddThreadTrigger_5C86C0(car_thread_trigger_type::char_on_foot_4,
                                                                    car_thread_trigger_action::spawn_thread_2,
                                                                    reinterpret_cast<SCR_THREAD*>(pCmd),
                                                                    pCmd->field_10_x,
                                                                    pCmd->field_14_y,
                                                                    pCmd->field_18_z,
                                                                    pCmd->field_1C_w,
                                                                    pCmd->field_20_h);

                break;
            case SCRCMD_CHAR_AREA_ANY_MEANS:
                pCmd->field_26_trigger_idx = gCarThreadTriggers_214_705F20->AddThreadTrigger_5C86C0(car_thread_trigger_type::char_any_means_3,
                                                                    car_thread_trigger_action::spawn_thread_2,
                                                                    reinterpret_cast<SCR_THREAD*>(pCmd),
                                                                    pCmd->field_10_x,
                                                                    pCmd->field_14_y,
                                                                    pCmd->field_18_z,
                                                                    pCmd->field_1C_w,
                                                                    pCmd->field_20_h);

                break;
        }
    }
}

MATCH_FUNC(0x5047c0)
void ScriptThread::SCRCMD_THREAD_DECLARE4_5047C0(SCR_THREAD* pCmd, s16* pThreadIdx)
{
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_10_char_var_idx));
    SCR_POINTER* pPhonePointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_12_car_or_phone_idx));

    if (pCharPointer->field_8_char && pPhonePointer->field_8_car)
    {
        pPhonePointer->field_8_obj->PoolGiveAndMarkDone_5291D0();
        gScriptManager_6F8060->AddPhoneThread_5129B0(pCharPointer->field_8_char->get_id(), pPhonePointer->field_8_obj->field_14_id, *pThreadIdx);
    }
}

MATCH_FUNC(0x504830)
void ScriptThread::SCRCMD_SET_GANG_INFO1_504830(SCR_SET_GANG_INFO* pCmd)
{
    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(pCmd->field_8_gangname);

    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pGangStr->get_name());
    pGang->SetRemap_4758A0(pCmd->field_A_remap);

    pGang->SetBasicWeapon_4758B0(pCmd->field_B_weapon1);
    pGang->SetAngryWeapon_4758C0(pCmd->field_C_weapon2);
    pGang->SetHateWeapon_4758D0(pCmd->field_D_weapon3);

    pGang->SetArrowColour_475940(pCmd->field_E_arrow_id);
    pGang->SetGangCarModel_4758E0(pCmd->field_1C_car_model);
    pGang->SetGangCarRemap_4758F0(static_cast<char_type>(pCmd->field_1E_car_remap));

    if (pCmd->field_10_pos.z == kFp255_6F7570)
    {
        pCmd->field_10_pos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_10_pos.x, pCmd->field_10_pos.y);
    }

    pGang->SetInfoPhonePos_475910(pCmd->field_10_pos.x, pCmd->field_10_pos.y, pCmd->field_10_pos.z);

    pGang->SetKillRespectChange_475950(pCmd->field_F_kill_respect_change);
    gGangPool_CA8_67E274->SwapGangSlots_4BF230(pGang, gScriptManager_6F8060->field_354_next_gang_idx);
    ++gScriptManager_6F8060->field_354_next_gang_idx;
    Gang* pMovedGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pGangStr->get_name());

    if (pCmd->field_F_kill_respect_change > 0)
    {
        gHud_2B00_706620->field_1F18_arrows.SetNewGangArrow_5D1310(pMovedGang);
    }
}

MATCH_FUNC(0x504950)
void ScriptThread::SCRCMD_SET_DOOR_INFO_504950(SCR_FOUR_PARAMS* pCmd)
{
    gDoor_4D4_67BD2C->RegisterDoorInfo_49D2D0(pCmd->field_8_unsigned_1, pCmd->field_A_unsigned_2, static_cast<char_type>(pCmd->field_C_unsigned_3));
}

MATCH_FUNC(0x504970)
void ScriptThread::SCRCMD_DOOR_DECLARE_D1_S1_504970(SCR_DOOR_DATA_DEC* pCmd)
{
    if (pCmd->field_2_type == SCRCMD_DOOR_DECLARE_S1)
    {
        if (pCmd->field_14_check.field_C_size.field_0_x == kFpZero_6F77C0 && pCmd->field_14_check.field_C_size.field_4_y == kFpZero_6F77C0)
        {
            pCmd->field_8_door = gDoor_4D4_67BD2C->RegisterSingleDoorNoCheck_49CF50(pCmd->field_10_gr_id,
                                                                               pCmd->field_C_block.field_0_x,
                                                                               pCmd->field_C_block.field_1_y,
                                                                               pCmd->field_C_block.field_2_z,
                                                                               pCmd->field_F_face,
                                                                               pCmd->field_28_flip,
                                                                               pCmd->field_29_reversed);
        }
        else
        {
            pCmd->field_8_door = gDoor_4D4_67BD2C->RegisterSingleDoor_49D170(pCmd->field_10_gr_id,
                                                                        pCmd->field_C_block.field_0_x,
                                                                        pCmd->field_C_block.field_1_y,
                                                                        pCmd->field_C_block.field_2_z,
                                                                        pCmd->field_F_face,
                                                                        pCmd->field_14_check.field_0_pos.x,
                                                                        pCmd->field_14_check.field_0_pos.y,
                                                                        pCmd->field_14_check.field_0_pos.z,
                                                                        pCmd->field_14_check.field_C_size.field_0_x,
                                                                        pCmd->field_14_check.field_C_size.field_4_y,
                                                                        pCmd->field_28_flip,
                                                                        pCmd->field_29_reversed);
        }
    }
    else
    {
        if (pCmd->field_14_check.field_C_size.field_0_x == kFpZero_6F77C0 && pCmd->field_14_check.field_C_size.field_4_y == kFpZero_6F77C0)
        {
            pCmd->field_8_door = gDoor_4D4_67BD2C->RegisterDoubleDoorNoCheck_49CFA0(pCmd->field_10_gr_id,
                                                                               pCmd->field_C_block.field_0_x,
                                                                               pCmd->field_C_block.field_1_y,
                                                                               pCmd->field_C_block.field_2_z,
                                                                               pCmd->field_F_face,
                                                                               pCmd->field_28_flip,
                                                                               pCmd->field_29_reversed);
        }
        else
        {
            pCmd->field_8_door = gDoor_4D4_67BD2C->RegisterDoubleDoor_49D1F0(pCmd->field_10_gr_id,
                                                                        pCmd->field_C_block.field_0_x,
                                                                        pCmd->field_C_block.field_1_y,
                                                                        pCmd->field_C_block.field_2_z,
                                                                        pCmd->field_F_face,
                                                                        pCmd->field_14_check.field_0_pos.x,
                                                                        pCmd->field_14_check.field_0_pos.y,
                                                                        pCmd->field_14_check.field_0_pos.z,
                                                                        pCmd->field_14_check.field_C_size.field_0_x,
                                                                        pCmd->field_14_check.field_C_size.field_4_y,
                                                                        pCmd->field_28_flip,
                                                                        pCmd->field_29_reversed);
        }
    }
    switch (pCmd->field_11_open_type)
    {
        case SCR_DOOR_OPENTYPES::any_player:
            pCmd->field_8_door->set_field_20(door_open_type::any_player);
            break;
        case SCR_DOOR_OPENTYPES::any_car:
            pCmd->field_8_door->set_field_20(door_open_type::any_car);
            break;
        case SCR_DOOR_OPENTYPES::none:
            pCmd->field_8_door->set_field_20(door_open_type::none);
            break;
        case SCR_DOOR_OPENTYPES::one_car:
            pCmd->field_8_door->set_field_20(door_open_type::one_car);
            break;
        case SCR_DOOR_OPENTYPES::one_char_on_foot:
            pCmd->field_8_door->set_field_20(door_open_type::one_char_on_foot);
            break;
        case SCR_DOOR_OPENTYPES::any_player_one_car:
            pCmd->field_8_door->set_field_20(door_open_type::any_player_one_car);
            break;
    }

    Door* pDoor;
    u16 close_delay;
    switch (pCmd->field_12_close_type)
    {
        case SCR_DOOR_CLOSETYPES::close_never:
            pCmd->field_8_door->set_close_type(door_close_type::close_never);
            pDoor = pCmd->field_8_door;
            close_delay = pCmd->field_13_delay;
            pDoor->field_1C_close_delay = close_delay;
            pDoor->field_1E_close_timer = close_delay;
            return;
        case SCR_DOOR_CLOSETYPES::close_time_delay:
            pCmd->field_8_door->set_close_type(door_close_type::close_time_delay);
            pDoor = pCmd->field_8_door;
            close_delay = pCmd->field_13_delay;
            pDoor->field_1C_close_delay = close_delay;
            pDoor->field_1E_close_timer = close_delay;
            return;
        case SCR_DOOR_CLOSETYPES::close_when_clear:
            pCmd->field_8_door->set_close_type(door_close_type::close_when_clear);
            pDoor = pCmd->field_8_door;
            close_delay = pCmd->field_13_delay;
            pDoor->field_1C_close_delay = close_delay;
            pDoor->field_1E_close_timer = close_delay;
            return;
        case SCR_DOOR_CLOSETYPES::close_when_open_rule_fails:
            pCmd->field_8_door->set_close_type(door_close_type::close_when_open_rule_fails);
            pDoor = pCmd->field_8_door;
            close_delay = pCmd->field_13_delay;
            pDoor->field_1C_close_delay = close_delay;
            pDoor->field_1E_close_timer = close_delay;
            return;
    }
    pDoor = pCmd->field_8_door;
    close_delay = pCmd->field_13_delay;
    pDoor->field_1C_close_delay = close_delay;
    pDoor->field_1E_close_timer = close_delay;
    // 9.6f: Door::set_close_delay (0x476A20, inlined, using it changes the code)
}

MATCH_FUNC(0x504b80)
void ScriptThread::SCRCMD_DOOR_DECLARE_D2_S2_504B80(SCR_DOOR_DATA_DEC* pCmd)
{
    if (pCmd->field_2_type == SCRCMD_DOOR_DECLARE_S2)
    {
        if (pCmd->field_14_check.field_C_size.field_0_x == kFpZero_6F77C0 && pCmd->field_14_check.field_C_size.field_4_y == kFpZero_6F77C0)
        {
            pCmd->field_8_door = gDoor_4D4_67BD2C->RegisterSingleDoorNoCheck_49CF50(pCmd->field_10_gr_id,
                                                                               pCmd->field_C_block.field_0_x,
                                                                               pCmd->field_C_block.field_1_y,
                                                                               pCmd->field_C_block.field_2_z,
                                                                               pCmd->field_F_face,
                                                                               pCmd->field_28_flip,
                                                                               pCmd->field_29_reversed);
        }
        else
        {
            pCmd->field_8_door = gDoor_4D4_67BD2C->RegisterSingleDoor_49D170(pCmd->field_10_gr_id,
                                                                        pCmd->field_C_block.field_0_x,
                                                                        pCmd->field_C_block.field_1_y,
                                                                        pCmd->field_C_block.field_2_z,
                                                                        pCmd->field_F_face,
                                                                        pCmd->field_14_check.field_0_pos.x,
                                                                        pCmd->field_14_check.field_0_pos.y,
                                                                        pCmd->field_14_check.field_0_pos.z,
                                                                        pCmd->field_14_check.field_C_size.field_0_x,
                                                                        pCmd->field_14_check.field_C_size.field_4_y,
                                                                        pCmd->field_28_flip,
                                                                        pCmd->field_29_reversed);
        }
    }
    else
    {
        if (pCmd->field_14_check.field_C_size.field_0_x == kFpZero_6F77C0 && pCmd->field_14_check.field_C_size.field_4_y == kFpZero_6F77C0)
        {
            pCmd->field_8_door = gDoor_4D4_67BD2C->RegisterDoubleDoorNoCheck_49CFA0(pCmd->field_10_gr_id,
                                                                               pCmd->field_C_block.field_0_x,
                                                                               pCmd->field_C_block.field_1_y,
                                                                               pCmd->field_C_block.field_2_z,
                                                                               pCmd->field_F_face,
                                                                               pCmd->field_28_flip,
                                                                               pCmd->field_29_reversed);
        }
        else
        {
            pCmd->field_8_door = gDoor_4D4_67BD2C->RegisterDoubleDoor_49D1F0(pCmd->field_10_gr_id,
                                                                        pCmd->field_C_block.field_0_x,
                                                                        pCmd->field_C_block.field_1_y,
                                                                        pCmd->field_C_block.field_2_z,
                                                                        pCmd->field_F_face,
                                                                        pCmd->field_14_check.field_0_pos.x,
                                                                        pCmd->field_14_check.field_0_pos.y,
                                                                        pCmd->field_14_check.field_0_pos.z,
                                                                        pCmd->field_14_check.field_C_size.field_0_x,
                                                                        pCmd->field_14_check.field_C_size.field_4_y,
                                                                        pCmd->field_28_flip,
                                                                        pCmd->field_29_reversed);
        }
    }

    switch (pCmd->field_11_open_type)
    {
        case SCR_DOOR_OPENTYPES::any_player:
            pCmd->field_8_door->set_field_20(door_open_type::any_player);
            break;
        case SCR_DOOR_OPENTYPES::any_car:
            pCmd->field_8_door->set_field_20(door_open_type::any_car);
            break;
        case SCR_DOOR_OPENTYPES::one_car:
        case SCR_DOOR_OPENTYPES::any_player_one_car:
        {
            SCR_POINTER* pTargetPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_2A_open_target));
            Car* pCar = pTargetPointer->field_8_car;
            if (pCar != NULL)
            {
                pCmd->field_8_door->set_open_details_car_bc(door_open_type::one_car, pCar);
                pCmd->field_8_door->set_target_id(pTargetPointer->field_8_car->field_6C_car_id);
            }
            break;
        }

        case SCR_DOOR_OPENTYPES::one_model:
            pCmd->field_8_door->set_open_details_model_id(door_open_type::one_model, pCmd->field_2A_open_target);
            break;

        case SCR_DOOR_OPENTYPES::one_char_on_foot:
        {
            SCR_POINTER* pTargetPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_2A_open_target));
            Ped* pPed = pTargetPointer->field_8_char;
            if (pPed != NULL)
            {
                pCmd->field_8_door->set_open_details_ped(door_open_type::one_char_on_foot, pPed);
                pCmd->field_8_door->set_target_id(pTargetPointer->field_8_char->field_200_id);
            }
        }
        break;
        case SCR_DOOR_OPENTYPES::none:
            pCmd->field_8_door->set_field_20(door_open_type::none);
            break;
    }

    switch (pCmd->field_12_close_type)
    {
        case SCR_DOOR_CLOSETYPES::close_never:
            pCmd->field_8_door->set_close_type(door_close_type::close_never);
            break;
        case SCR_DOOR_CLOSETYPES::close_time_delay:
            pCmd->field_8_door->set_close_type(door_close_type::close_time_delay);
            break;
        case SCR_DOOR_CLOSETYPES::close_when_clear:
            pCmd->field_8_door->set_close_type(door_close_type::close_when_clear);
            break;
        case SCR_DOOR_CLOSETYPES::close_when_open_rule_fails:
            pCmd->field_8_door->set_close_type(door_close_type::close_when_open_rule_fails);
            break;
    }
    Door* pDoor = pCmd->field_8_door;
    u16 close_delay = pCmd->field_13_delay;
    pDoor->field_1C_close_delay = close_delay;
    pDoor->field_1E_close_timer = close_delay;
    // 9.6f: Door::set_close_delay (0x476A20, inlined, using it changes the code)
}

MATCH_FUNC(0x504dd0)
void ScriptThread::SCRCMD_DECLARE_MISSION_504DD0(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_unsigned_1); //  player pointer not used because missions are singleplayer
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));
    if (pCmd->field_2_type == SCRCMD_DEC_GANG_1_FLAG)
    {
        gScriptManager_6F8060->field_348_gang_1_mission_flag = &pPointer->field_8_counter;
    }
    else if (pCmd->field_2_type == SCRCMD_DEC_GANG_2_FLAG)
    {
        gScriptManager_6F8060->field_34C_gang_2_mission_flag = &pPointer->field_8_counter;
    }
    else if (pCmd->field_2_type == SCRCMD_DEC_GANG_3_FLAG)
    {
        gScriptManager_6F8060->field_350_gang_3_mission_flag = &pPointer->field_8_counter;
    }
    else // pCmd->field_2_type == SCRCMD_DECLARE_MISSION
    {
        gScriptManager_6F8060->field_344_mission_flag = &pPointer->field_8_counter;
    }
}

MATCH_FUNC(0x504ee0)
void ScriptThread::CreateLight_504EE0(SCR_CREATE_LIGHT* pCmd, SCR_POINTER* pPointer)
{
    Light* pNewLight = gLight_1D4CC_6F5520->Init_469010(pCmd->field_C_xpos,
                                                                       pCmd->field_10_ypos,
                                                                       pCmd->field_14_zpos,
                                                                       pCmd->field_18_argb,
                                                                       pCmd->field_1C_radius,
                                                                       pCmd->field_20_intensity);
    pPointer->field_8_light = pNewLight;
    if (pCmd->field_21_on_time > 0)
    {
        gLight_1D4CC_6F5520->SetFlashing_469070(pNewLight, pCmd->field_21_on_time, pCmd->field_22_off_time, pCmd->field_23_shape);
    }
}

MATCH_FUNC(0x505030)
void ScriptThread::SCRCMD_SET_STATION_EMPTY_STATION_505030(SCR_SET_STATION* pCmd)
{
    if (!bSkip_trains_67D550)
    {
        str_table_entry* pStationStr = gScriptManager_6F8060->FindStringById_503080(pCmd->field_8_platform);
        gmp_map_zone* station_zone = gMap_0x370_6F6268->FindZoneByName_4DEFD0(pStationStr->get_name());
        TrainStation* pStation = gPublicTransport_181C_6FF1D4->TrainStationForZone_57B4B0(station_zone);

        for (u8 wagon_idx = 0; wagon_idx < 10; wagon_idx++)
        {
            if (wagon_idx < pCmd->field_A_num_passenger)
            {
                pStation->field_24_train_wagons[wagon_idx] = 1; // passenger wagon
            }
            else
            {
                const s32 num_freight = pCmd->field_B_num_freight;
                const s32 num_passenger = pCmd->field_A_num_passenger;

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
void ScriptThread::SCRCMD_RADIOSTATION_DEC_5051D0(SCR_RADIOSTATION_DEC* pCmd)
{
    gRoot_sound_66B038.DeclareRadioStation_40F030(pCmd->field_10_station_type, pCmd->field_8_pos.field_0_x, pCmd->field_8_pos.field_4_y);
}

MATCH_FUNC(0x505210)
void ScriptThread::SCRCMD_SET_TRAIN_STATIONS_505210(SCR_TWO_PARAMS* pCmd)
{
    if (!bSkip_trains_67D550)
    {
        str_table_entry* pStationStr = gScriptManager_6F8060->FindStringById_503080(pCmd->field_8_unsigned_1);
        const char_type* station_zone_name = pStationStr->get_name();
        gmp_map_zone* station_zone = gMap_0x370_6F6268->FindZoneByName_4DEFD0(station_zone_name);
        TrainStation* pTrainStation = gPublicTransport_181C_6FF1D4->TrainStationForZone_57B4B0(station_zone);
        pTrainStation->CalculateWagonCount_578820(reinterpret_cast<u8*>(&pCmd->field_A_unsigned_2)); //  The parameter may be the train wagon array, not sure
    }
}

MATCH_FUNC(0x5052c0)
void ScriptThread::SCRCMD_DECLARE_POLICE_5052C0(SCR_DECLARE_POLICELEVEL* pCmd)
{
    u8 max_wanted_level = static_cast<u8>(pCmd->field_A_wanted_level);
    if (max_wanted_level == 0)
    {
        bSkip_police_67D4F9 = false;
    }
    gPolice_7B8_6FEE40->field_660_max_wanted_stars = static_cast<u8>(pCmd->field_A_wanted_level); // max_wanted_level
}

MATCH_FUNC(0x505340)
void ScriptThread::SCRCMD_SOUND_DECSET_505340(SCR_SOUND_DECSET* pCmd, SCR_POINTER* pPointer)
{
    if (!pCmd->field_19_play_type)
    {
        pPointer->field_8_obj = gObject_5C_6F8F84->NewWithPedRefIdx_5299F0(objects::sound_object_type_1_278,
                                                              pCmd->field_18_sound_id,
                                                              pCmd->field_C_pos.x,
                                                              pCmd->field_C_pos.y,
                                                              pCmd->field_C_pos.z);
    }
    else
    {
        pPointer->field_8_obj = gObject_5C_6F8F84->NewWithPedRefIdx_5299F0(objects::sound_object_type_2_279,
                                                              pCmd->field_18_sound_id,
                                                              pCmd->field_C_pos.x,
                                                              pCmd->field_C_pos.y,
                                                              pCmd->field_C_pos.z);
    }
}

MATCH_FUNC(0x505430)
void ScriptThread::SCRCMD_SET_MISSIONS_TOTAL_505430(SCR_TWO_PARAMS* pCmd)
{
    switch (pCmd->field_2_type)
    {
        case SCRCMD_TOTAL_MISSIONS:
            gScriptManager_6F8060->field_314_total_missions = pCmd->field_A_signed_2;
            break;
        case SCRCMD_TOTAL_SECRETS:
            gScriptManager_6F8060->field_318_total_secrets = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_1_MISSION_TOTAL:
            gScriptManager_6F8060->field_31C_gang_1_missions_total = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_2_MISSION_TOTAL:
            gScriptManager_6F8060->field_320_gang_2_missions_total = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_3_MISSION_TOTAL:
            gScriptManager_6F8060->field_324_gang_3_missions_total = pCmd->field_A_signed_2;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505580)
void ScriptThread::SCRCMD_PASSED_FAILED_FLAGS_505580(SCR_TWO_PARAMS* pCmd)
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_unsigned_1));

    switch (pCmd->field_2_type)
    {
        case SCRCMD_PASSED_FLAG:
            gScriptManager_6F8060->field_328_passed_flag = &pPointer->field_8_counter;
            break;
        case SCRCMD_1_PASSED_FLAG:
            gScriptManager_6F8060->field_32C_1_passed_flag = &pPointer->field_8_counter;
            break;
        case SCRCMD_2_PASSED_FLAG:
            gScriptManager_6F8060->field_330_2_passed_flag = &pPointer->field_8_counter;
            break;
        case SCRCMD_3_PASSED_FLAG:
            gScriptManager_6F8060->field_334_3_passed_flag = &pPointer->field_8_counter;
            break;
        case SCRCMD_SECRETS_PASSED:
            gScriptManager_6F8060->field_338_secrets_passed = &pPointer->field_8_counter;
            break;
        case SCRCMD_SECRETS_FAILED:
            gScriptManager_6F8060->field_33C_secrets_failed = &pPointer->field_8_counter;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505710)
void ScriptThread::SCRCMD_FINISH_SCORE_505710(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager_6F8060->field_310_finish_score = pCmd->field_8_s32;
}

MATCH_FUNC(0x505750)
void ScriptThread::SCRCMD_DECLARE_CARLIST_505750(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager_6F8060->field_340_car_list = &pCmd->field_8_u32;
}

MATCH_FUNC(0x505790)
void ScriptThread::DisableThread_505790(u16 idx)
{
    SCR_CMD_HEADER* pCmd = gScriptManager_6F8060->GetBasePointer_512770(idx);

    switch (pCmd->field_2_type)
    {
        case SCRCMD_THREAD_DECLARE4:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);
            SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pThread->field_10_char_var_idx));
            SCR_POINTER* pPhonePointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pThread->field_12_car_or_phone_idx));

            if (pPhonePointer->field_8_obj->IsAnimatedBehavior_475A80() && pPhonePointer->field_8_obj->field_24_touched_result != objects::dead_phone_174)
            {
                pPhonePointer->field_8_obj->PoolGiveAndMarkDone_5291D0();
            }

            gScriptManager_6F8060->RemovePhoneThread_512A70(pCharPointer->field_8_char->get_id(), pPhonePointer->field_8_obj->field_14_id);
            break;
        }
        case SCRCMD_THREAD_DECLARE3:
            gCarThreadTriggers_214_705F20->SetTriggerEnableState_4768C0(static_cast<SCR_THREAD*>(pCmd)->field_15_trigger_idx, car_thread_trigger_state::disabled_2);
            break;
        case SCRCMD_THREAD_DECLARE5:
        case SCRCMD_CHAR_AREA_ANY_MEANS:
            gCarThreadTriggers_214_705F20->SetTriggerEnableState_4768C0(static_cast<SCR_CHAR_AREA_ANY*>(pCmd)->field_26_trigger_idx, car_thread_trigger_state::disabled_2);
            break;
        case SCRCMD_THREAD_DECLARE2:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);
            SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pThread->field_10_char_var_idx));
            SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pThread->field_12_car_or_phone_idx));

            s32 char_id = pCharPointer->field_8_char->get_id();
            ThreadTrigger* pTrigger = gScriptManager_6F8060->FindCarThread_5128A0(char_id, pCarPointer->field_8_car->field_6C_car_id);

            if (pTrigger)
            {
                pTrigger->field_0_ped_id = 0;
                pTrigger->field_4_target_id = 0;
                pTrigger->field_8_cmd_line = 0;
                --gScriptManager_6F8060->field_0_car_trigger_count;
            }

            pCarPointer->field_8_car->ClearCarThreadPending_4218E0();
            break;
        }
    }
}

MATCH_FUNC(0x505b10)
void ScriptThread::DeallocOrDeleteItem_505B10(u16 idx)
{
    SCR_POINTER* pCarCmdPointer;
    SCR_POINTER* pPedCmdPointer;
    SCR_POINTER* pObjCmdPointer;
    SCR_POINTER* pLightCmdPointer;
    SCR_DELETE_SOUND* pSoundCmdPointer;

    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
    switch (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->field_2_type))
    {
        case script_entity_type::car_2:
            pCarCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pCarCmdPointer->field_8_car)
            {
                gScriptManager_6F8060->ClearWeaponHitFlag_512BA0(pCarCmdPointer->field_8_car->field_6C_car_id, false);
                pCarCmdPointer->field_8_car->MarkForDespawn_421470();
                pCarCmdPointer->field_8_car = NULL;
            }
            break;

        case script_entity_type::char_1:
            pPedCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pPedCmdPointer->field_8_char)
            {
                pPedCmdPointer->field_8_char->Deallocate_45EB60();
                pPedCmdPointer->field_8_char = NULL;
            }
            break;

        case script_entity_type::object_3:
            pObjCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pObjCmdPointer->field_8_obj)
            {
                pObjCmdPointer->field_8_obj->Dealloc_5291B0();
                pObjCmdPointer->field_8_obj = NULL;
            }
            break;
        case script_entity_type::light_8:
            pLightCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pLightCmdPointer->field_8_light)
            {
                gLight_1D4CC_6F5520->DeallocLight_47F4F0(pLightCmdPointer->field_8_light);
                pLightCmdPointer->field_8_light = NULL;
            }
            break;

        case script_entity_type::radio_station_9:
            pSoundCmdPointer = static_cast<SCR_DELETE_SOUND*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            gRoot_sound_66B038.RemoveSound_40F050(pSoundCmdPointer->field_8_xpos, pSoundCmdPointer->field_C_ypos);
            break;

        case script_entity_type::sound_10:
            pObjCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pObjCmdPointer->field_8_obj)
            {
                pObjCmdPointer->field_8_obj->Dealloc_5291B0();
                pObjCmdPointer->field_8_obj = NULL;
            }
            return;
        default:
            return;
    }
}

MATCH_FUNC(0x505ea0)
s16 ScriptThread::GetBonusResult_505EA0(u16 idx)
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
    s16 result = pPointer->field_8_counter;
    if (result != bonus_result::passed && result != bonus_result::failed)
    {
        result = gGame_0x40_67E008->field_38_local_player->field_2D4_scores.field_1A8_bonuses.GetBonusResult_432080(result);
        if (result == bonus_result::passed || result == bonus_result::failed)
        {
            pPointer->field_8_counter = result;
        }
    }
    return result;
}

MATCH_FUNC(0x505f50)
void ScriptThread::SCRCMD_DISPLAY_MESSAGES_505F50(SCR_TWO_PARAMS* pCmd)
{
    if (gDo_text_id_test_67D6D0)
    {
        sprintf(gTmpBuffer_67C598, "%d", pCmd->field_8_unsigned_1);
        if (!gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598))
        {
            FatalError_4A38C0(Gta2Error::MissionScriptIsReferringToTextIdThatDoesNotExist,
                              "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                              2460,
                              pCmd->field_8_unsigned_1); // Mission script is referring to a 'text id' that doesn't exist. ID
        }
    }
}

MATCH_FUNC(0x506010)
void ScriptThread::SCRCMD_DEC_DEATH_BASE_506010(SCR_FOUR_PARAMS* pCmd)
{
    if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_1)
    {
        gScriptManager_6F8060->field_356_gang_1_death_base = pCmd->field_C_unsigned_3;
    }
    else if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_2)
    {
        gScriptManager_6F8060->field_358_gang_2_death_base = pCmd->field_C_unsigned_3;
    }
    else if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_3)
    {
        gScriptManager_6F8060->field_35A_gang_3_death_base = pCmd->field_C_unsigned_3;
    }
}

MATCH_FUNC(0x5060d0)
void ScriptThread::SCRCMD_DO_CRANE_POWERUP_5060D0(SCR_DECLARE_CRANE_POWERUP* pCmd)
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_crane_idx));
    pPointer->field_8_crane->field_140_powerup_cmd = pCmd->field_0_cmd_this;
}

MATCH_FUNC(0x506140)
void ScriptThread::SCRCMD_START_BASIC_KF_506140(u16* pKfValue)
{
    for (u16 idx = 0; idx < 25; idx++)
    {
        if (gScriptManager_6F8060->field_C1E74_basic_kf[idx] == 0)
        {
            gScriptManager_6F8060->field_C1E74_basic_kf[idx] = *pKfValue;
            break;
        }
    }
}

MATCH_FUNC(0x5061c0)
void ScriptThread::ExecOpCode_5061C0()
{
    for (u16 i = 1; i < 6000u; ++i)
    {
        SCR_CMD_HEADER* pBasePtr = gScriptManager_6F8060->GetBasePointer_512770(i);
        if (pBasePtr)
        {
            switch (pBasePtr->field_2_type)
            {
                case SCRCMD_PLAYER_PED:
                    ScriptThread::SCRCMD_PLAYER_PED_503A20(reinterpret_cast<SCR_PLAYER_PED*>(pBasePtr));
                    break;
                case SCRCMD_CAR_DECSET_2D:
                case SCRCMD_CAR_DECSET_3D:
                case SCRCMD_CAR_DECSET_2D_STR:
                case SCRCMD_CAR_DECSET_3D_STR:
                    ScriptThread::SCRCMD_CAR_DECSET_503BC0(static_cast<SCR_CAR_DATA_DEC*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_CHAR_DECSET_2D:
                case SCRCMD_CHAR_DECSET_3D:
                    ScriptThread::SCRCMD_CHAR_DECSET_2D_3D_503FB0(static_cast<SCR_CHAR_DATA_DEC*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_OBJ_DECSET_2D:
                case SCRCMD_OBJ_DECSET_3D:
                    ScriptThread::SCRCMD_OBJ_DECSET_2D_3D_503680(static_cast<SCR_OBJ_DATA*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_OBJ_DECSET_2D_INT:
                case SCRCMD_OBJ_DECSET_3D_INT:
                case SCRCMD_OBJ_DECSET_2D_STR:
                case SCRCMD_OBJ_DECSET_3D_STR:
                    ScriptThread::SCRCMD_OBJ_DECSET_5038D0(static_cast<SCR_OBJ_DATA*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_MAP_ZONE_SET:
                    ScriptThread::SCRCMD_MAP_ZONE_SET_504150(static_cast<SCR_MAP_ZONE_SET*>(pBasePtr));
                    break;
                case SCRCMD_ARROW_DEC:
                    ScriptThread::SCRCMD_ARROW_DEC_5041B0(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_CRANE_BASIC_DEC:
                case SCRCMD_CRANE_TARGET_DEC:
                case SCRCMD_CRANE2TARGET_DEC:
                    ScriptThread::SCRCMD_CRANE_5041C0(static_cast<SCR_CRANE_TARGET_DEC*>(pBasePtr), static_cast<SCR_CRANE_BASIC_DEC*>(pBasePtr));
                    break;
                case SCRCMD_CONVEYOR_DECSET1:
                case SCRCMD_CONVEYOR_DECSET2:
                    ScriptThread::SCRCMD_CONVEYOR_DECSET1_2_5043A0(static_cast<SCR_CONVEYOR*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_GENERATOR_DECSET1:
                case SCRCMD_GENERATOR_DECSET2:
                case SCRCMD_GENERATOR_DECSET3:
                case SCRCMD_GENERATOR_DECSET4:
                    ScriptThread::SCRCMD_GENERATOR_DECSET_504420(static_cast<SCR_GENERATOR*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_DESTRUCTOR_DECSET1:
                case SCRCMD_DESTRUCTOR_DECSET2:
                    ScriptThread::SCRCMD_DESTRUCTOR_DECSET_504530(static_cast<SCR_DESTRUCTOR*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_THREAD_DECLARE2:
                    ScriptThread::SCRCMD_THREAD_DECLARE2_5045D0(static_cast<SCR_THREAD*>(pBasePtr), &pBasePtr->field_0_cmd_this);
                    break;
                case SCRCMD_THREAD_DECLARE4:
                    ScriptThread::SCRCMD_THREAD_DECLARE4_5047C0(static_cast<SCR_THREAD*>(pBasePtr), &pBasePtr->field_0_cmd_this);
                    break;
                case SCRCMD_THREAD_DECLARE3:
                    ScriptThread::SCRCMD_THREAD_DECLARE3_504660(static_cast<SCR_THREAD*>(pBasePtr));
                    break;
                case SCRCMD_SET_GANG_INFO1:
                    ScriptThread::SCRCMD_SET_GANG_INFO1_504830(static_cast<SCR_SET_GANG_INFO*>(pBasePtr));
                    break;
                case SCRCMD_SET_DOOR_INFO:
                    ScriptThread::SCRCMD_SET_DOOR_INFO_504950(static_cast<SCR_FOUR_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_CRUSHER_BASIC:
                    ScriptThread::SCRCMD_CRUSHER_BASIC_5045A0(static_cast<SCR_CRUSHER_BASIC*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_CHAR_AREA_ANY_MEANS:
                case SCRCMD_THREAD_DECLARE5:
                    ScriptThread::SCRCMD_THREAD_DECLARE5_504710(static_cast<SCR_CHAR_AREA_ANY*>(pBasePtr));
                    break;
                case SCRCMD_DOOR_DECLARE_S1:
                case SCRCMD_DOOR_DECLARE_D1:
                    ScriptThread::SCRCMD_DOOR_DECLARE_D1_S1_504970(static_cast<SCR_DOOR_DATA_DEC*>(pBasePtr));
                    break;
                case SCRCMD_DOOR_DECLARE_S2:
                case SCRCMD_DOOR_DECLARE_D2:
                    ScriptThread::SCRCMD_DOOR_DECLARE_D2_S2_504B80(static_cast<SCR_DOOR_DATA_DEC*>(pBasePtr));
                    break;
                case SCRCMD_DEC_GANG_1_FLAG:
                case SCRCMD_DEC_GANG_2_FLAG:
                case SCRCMD_DEC_GANG_3_FLAG:
                case SCRCMD_DECLARE_MISSION:
                    ScriptThread::SCRCMD_DECLARE_MISSION_504DD0(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_CREATE_LIGHT2:
                case SCRCMD_LIGHT_DEC:
                    ScriptThread::CreateLight_504EE0(static_cast<SCR_CREATE_LIGHT*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_SET_STATION:
                case SCRCMD_SET_EMPTY_STATION:
                    ScriptThread::SCRCMD_SET_STATION_EMPTY_STATION_505030(static_cast<SCR_SET_STATION*>(pBasePtr));
                    break;
                case SCRCMD_RADIOSTATION_DEC:
                    ScriptThread::SCRCMD_RADIOSTATION_DEC_5051D0(static_cast<SCR_RADIOSTATION_DEC*>(pBasePtr));
                    break;
                case SCRCMD_SET_STATION_4:
                case SCRCMD_SET_STATION_3:
                case SCRCMD_SET_STATION_2:
                case SCRCMD_SET_STATION_1:
                    ScriptThread::SCRCMD_SET_TRAIN_STATIONS_505210(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_DECLARE_POLICE:
                    ScriptThread::SCRCMD_DECLARE_POLICE_5052C0(static_cast<SCR_DECLARE_POLICELEVEL*>(pBasePtr));
                    break;
                case SCRCMD_SOUND_DECSET:
                    ScriptThread::SCRCMD_SOUND_DECSET_505340(static_cast<SCR_SOUND_DECSET*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_FINISH_SCORE:
                    ScriptThread::SCRCMD_FINISH_SCORE_505710(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_TOTAL_MISSIONS:
                case SCRCMD_TOTAL_SECRETS:
                case SCRCMD_GANG_1_MISSION_TOTAL:
                case SCRCMD_GANG_2_MISSION_TOTAL:
                case SCRCMD_GANG_3_MISSION_TOTAL:
                    ScriptThread::SCRCMD_SET_MISSIONS_TOTAL_505430(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_PASSED_FLAG:
                case SCRCMD_1_PASSED_FLAG:
                case SCRCMD_2_PASSED_FLAG:
                case SCRCMD_3_PASSED_FLAG:
                case SCRCMD_SECRETS_PASSED:
                case SCRCMD_SECRETS_FAILED:
                    ScriptThread::SCRCMD_PASSED_FAILED_FLAGS_505580(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_DECLARE_CARLIST:
                    ScriptThread::SCRCMD_DECLARE_CARLIST_505750(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_DISPLAY_MESSAGE:
                case SCRCMD_DISPLAY_BRIEF:
                case SCRCMD_DISPLAY_BRIEF_NOW:
                case SCRCMD_DISPLAY_BRIEF_SOON:
                    ScriptThread::SCRCMD_DISPLAY_MESSAGES_505F50(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_PARKED_CAR_DECSET_2D:
                case SCRCMD_PARKED_CAR_DECSET_3D:
                case SCRCMD_PARKED_CAR_DECSET_2D_STR:
                case SCRCMD_PARKED_CAR_DECSET_3D_STR:
                    ScriptThread::SCRCMD_PARKED_CAR_DECSET_503F80(static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_DEC_DEATH_BASE_1:
                case SCRCMD_DEC_DEATH_BASE_2:
                case SCRCMD_DEC_DEATH_BASE_3:
                    ScriptThread::SCRCMD_DEC_DEATH_BASE_506010(static_cast<SCR_FOUR_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_DO_CRANE_POWERUP:
                    ScriptThread::SCRCMD_DO_CRANE_POWERUP_5060D0(static_cast<SCR_DECLARE_CRANE_POWERUP*>(pBasePtr));
                    break;
                case SCRCMD_START_BASIC_KF:
                    ScriptThread::SCRCMD_START_BASIC_KF_506140(reinterpret_cast<u16*>(&pBasePtr->field_0_cmd_this));
                    break;
                case SCRCMD_OBJ_DEC:
                case SCRCMD_COUNTER:
                case SCRCMD_COUNTER_SET:
                case SCRCMD_CREATE_OBJ_3D_INT:
                case SCRCMD_CREATE_OBJ_2D_STR:
                case SCRCMD_LEVELSTART:
                case SCRCMD_LEVELEND:
                case SCRCMD_THREAD_DECLARE1:
                case SCRCMD_COUNTER_SAVE:
                case SCRCMD_COUNTER_SET_SAVE:
                    break;

                default:
                    continue;
            }
        }
    }
    gScriptManager_6F8060->RestoreAnsweredPhones_511C60();
    ScriptThread::DisableBasicKfsFromSave_511CD0();
    gGeneratorPool_14AC_67E5D0->FinalizeAfterScriptLoad_4C1CD0();
}

MATCH_FUNC(0x5069c0)
void ScriptThread::SCRCMD_LEVELSTART_5069C0()
{
    gRoot_sound_66B038.PlayVoice_40F090(voice_line::respectis_24); //  "And remember, respect is everything!"
    field_118_in_level_start = true;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5069f0)
void ScriptThread::SCRCMD_LEVELEND_5069F0()
{
    field_118_in_level_start = false;
    ScriptThread::EndCmd_503670();
}

MATCH_FUNC(0x506a00)
void ScriptThread::SCRCMD_CREATE_THREAD_506A00()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_THREAD* pThread = static_cast<SCR_THREAD*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    ScriptThread* pScriptThread = ScriptThread::SpawnThread_511960(pCmd->field_8_unsigned_1);
    pThread->field_8_script_thread = pScriptThread;
    pThread->field_C_thread_id = pScriptThread->field_11A_thread_id;
    pScriptThread->InitThread_511930(field_6_flags, pCmd->field_8_unsigned_1);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x506a60)
void ScriptThread::SCRCMD_STOP_THREAD_506A60()
{
    SCR_THREAD* pThread = static_cast<SCR_THREAD*>(gScriptManager_6F8060->GetBasePointer_512770(static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[1].field_2_type));
    ScriptThread* pScriptThread;
    s16 threadId;

    s32 opcode_idx = pThread->field_2_type;
    if (opcode_idx != SCRCMD_THREAD_ID)
    {
        if (opcode_idx > SCRCMD_THREAD_DECLARE1 && opcode_idx <= SCRCMD_THREAD_DECLARE5)
        {
            pScriptThread = pThread->field_8_script_thread;
            threadId = pThread->field_C_thread_id;

            pThread->field_8_script_thread = NULL;
            pThread->field_C_thread_id = 0;
        }
    }
    else
    {
        pScriptThread = pThread->field_8_script_thread;
        threadId = pThread->field_C_thread_id;

        pThread->field_C_thread_id = 0;
        pThread->field_8_script_thread = NULL;
    }

    if (pScriptThread->field_11A_thread_id == threadId)
    {
        pScriptThread->StopThread_503610();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x506af0)
void ScriptThread::SCRCMD_IF_JUMP_506AF0()
{
    //  field_8_cond_result = 1  means the boolean opcode has returned true
    //  field_8_cond_result = 0  means the boolean opcode has returned false

    //  If it's a OR and boolean is true, or if it's a AND and boolean is false, jump
    SCR_IF_JUMP* pCmd = reinterpret_cast<SCR_IF_JUMP*>(gBasePtr_6F8070);

    if ((pCmd->is_or == 1 && field_8_cond_result) || (!pCmd->is_or && !field_8_cond_result))
    {

        JumpToCmd_503650(pCmd->else_endif_pointer); //  Jump to the last IF_JUMP or go to ELSE section
        return;
    }

    Next_503620(gBasePtr_6F8070); // go to field_4_cmd_next
}

MATCH_FUNC(0x506b30)
void ScriptThread::SCRCMD_GOSUB_506B30()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    ScriptStackFrame* pFrame = field_114_gosub_stack->AllocFrame_5031A0();
    pFrame->field_0_cond_result = field_8_cond_result;
    pFrame->field_4_return_cmd = gBasePtr_6F8070->field_4_cmd_next;
    field_114_gosub_stack->add_503160(pFrame);
    ScriptThread::JumpToCmd_503650(pCmd->field_8_unsigned_1);
}

MATCH_FUNC(0x506b80)
void ScriptThread::SCRCMD_RETURN_506B80() // MISSIONEND
{
    ScriptStackFrame* pFrame = field_114_gosub_stack->remove_503180();

    if (pFrame == NULL)
    {
        ScriptThread::EndCmd_503670();
    }
    else
    {
        ScriptThread::JumpToCmd_503650(pFrame->field_4_return_cmd);
        field_8_cond_result = pFrame->field_0_cond_result;
        field_114_gosub_stack->FreeFrame_5031C0(pFrame);
    }
}

MATCH_FUNC(0x506bc0)
u8 ScriptThread::GetOperationType_506BC0(u32 opcode)
{
    switch (opcode)
    {
        case SCRCMD_S_PLUS_I:
        case SCRCMD_I_PLUS_S:
        case SCRCMD_S_PLUS_S:
        case SCRCMD_S_IS_S_PLUS_S:
        case SCRCMD_S_IS_S_PLUS_I:
            return script_operation::add_0;
        case SCRCMD_S_MINUS_I:
        case SCRCMD_I_MINUS_S:
        case SCRCMD_S_MINUS_S:
        case SCRCMD_S_IS_S_MINUS_I:
        case SCRCMD_S_IS_S_MINUS_S:
            return script_operation::subtract_1;
        case SCRCMD_S_LESS_I:
        case SCRCMD_S_LESS_S:
            return script_operation::less_4;
        case SCRCMD_S_LEQUAL_I:
        case SCRCMD_S_LEQUAL_S:
            return script_operation::less_or_equal_5;
        case SCRCMD_S_GREATER_I:
        case SCRCMD_S_GREATER_S:
            return script_operation::greater_2;
        case SCRCMD_S_GEQUAL_I:
        case SCRCMD_S_GEQUAL_S:
            return script_operation::greater_or_equal_3;
        case SCRCMD_S_EQUAL_I:
        case SCRCMD_S_EQUAL_S:
            return script_operation::equal_6;
        case SCRCMD_S_IS_S_DIV_I:
        case SCRCMD_S_IS_S_DIV_S:
            return script_operation::divide_7;
        case SCRCMD_S_IS_S_MULT_I:
        case SCRCMD_S_IS_S_MULT_S:
            return script_operation::multiply_8;
        case SCRCMD_S_IS_S_MOD_I:
        case SCRCMD_S_IS_S_MOD_S:
            return script_operation::modulo_9;
        default:
            return script_operation::add_0;
    }
}

MATCH_FUNC(0x506d60)
void ScriptThread::SCRCMD_S_OP_I_506D60()
{
    SCR_OPERATE_COUNTER_AND_INT* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_INT*>(gBasePtr_6F8070);
    u8 operation_type = ScriptThread::GetOperationType_506BC0(pCmd->field_2_type);
    SCR_POINTER* pLeftOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_counter_idx));

    switch (operation_type)
    {
        case script_operation::add_0:
            this->field_8_cond_result = pLeftOperand->field_8_counter + pCmd->field_A_value;
            break;
        case script_operation::subtract_1:
            this->field_8_cond_result = pLeftOperand->field_8_counter - pCmd->field_A_value;
            break;
        case script_operation::less_4:
            if (pLeftOperand->field_8_counter < pCmd->field_A_value)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            break;
        case script_operation::less_or_equal_5:
            if (pLeftOperand->field_8_counter <= pCmd->field_A_value)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            break;
        case script_operation::greater_2:
            if (pLeftOperand->field_8_counter > pCmd->field_A_value)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            break;
        case script_operation::greater_or_equal_3:
            if (pLeftOperand->field_8_counter >= pCmd->field_A_value)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            break;
        case script_operation::equal_6:
            if (pLeftOperand->field_8_counter == pCmd->field_A_value)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            break;
        default:
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x506ed0)
void ScriptThread::SCRCMD_S_OP_S_506ED0()
{
    u8 operation_type = ScriptThread::GetOperationType_506BC0(gBasePtr_6F8070->field_2_type);
    SCR_OPERATE_COUNTER_AND_COUNTER_2* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_COUNTER_2*>(gBasePtr_6F8070);
    SCR_POINTER* pLeftOperand =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index)); // pCmd->field_8_left_counter_idx
    SCR_POINTER* pRightOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_right_counter_idx));
    switch (operation_type)
    {
        case script_operation::add_0:
            this->field_8_cond_result = pLeftOperand->field_8_counter + pRightOperand->field_8_counter;
            break;
        case script_operation::subtract_1:
            this->field_8_cond_result = pLeftOperand->field_8_counter - pRightOperand->field_8_counter;
            break;
        case script_operation::less_4:
            if (pLeftOperand->field_8_counter < pRightOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        case script_operation::less_or_equal_5:
            if (pLeftOperand->field_8_counter <= pRightOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        case script_operation::greater_2:
            if (pLeftOperand->field_8_counter > pRightOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        case script_operation::greater_or_equal_3:
            if (pLeftOperand->field_8_counter >= pRightOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        case script_operation::equal_6:
            if (pLeftOperand->field_8_counter == pRightOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507110)
void ScriptThread::SCRCMD_I_OP_S_507110()
{
    u8 operation_type = ScriptThread::GetOperationType_506BC0(gBasePtr_6F8070->field_2_type);
    SCR_OPERATE_INT_AND_COUNTER* pCmd = reinterpret_cast<SCR_OPERATE_INT_AND_COUNTER*>(gBasePtr_6F8070);
    SCR_POINTER* pOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(
        static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[1].field_2_type)); // pCmd->field_A_counter_idx

    switch (operation_type)
    {
        case script_operation::add_0:
            this->field_8_cond_result = pCmd->field_8_value + pOperand->field_8_counter;
            break;
        case script_operation::subtract_1:
            this->field_8_cond_result = pCmd->field_8_value - pOperand->field_8_counter;
            break;
        case script_operation::less_4:
            if (pCmd->field_8_value < pOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        case script_operation::less_or_equal_5:
            if (pCmd->field_8_value <= pOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        case script_operation::greater_2:
            if (pCmd->field_8_value > pOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        case script_operation::greater_or_equal_3:
            if (pCmd->field_8_value >= pOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        case script_operation::equal_6:
            if (pCmd->field_8_value == pOperand->field_8_counter)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }
            break;
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507750)
void ScriptThread::SCRCMD_NOT_507750()
{
    field_8_cond_result = field_8_cond_result == 0; //  just toggle the boolean value
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5078d0)
void ScriptThread::SCRCMD_START_EXEC_5078D0()
{
    s16 exec_flag = field_12_exec_flag;

    //  Only increase EXEC flag if it wasn't set before
    if (exec_flag <= 0)
    {
        field_12_exec_flag = exec_flag + 1; //  increase EXEC flag
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5079a0)
void ScriptThread::SCRCMD_STOP_EXEC_5079A0()
{
    s16 exec_flag = field_12_exec_flag;

    //  Only decrease EXEC flag if it was set before
    if (exec_flag >= 0)
    {
        field_12_exec_flag = exec_flag - 1; //  decrease EXEC flag
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507a70)
void ScriptThread::SCRCMD_INCREMENT_507A70()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    ++pPointer->field_8_counter; //  Increment Counter
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507b50)
void ScriptThread::SCRCMD_DECREMENT_507B50()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    --pPointer->field_8_counter; //  Decrement Counter
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507ce0)
void ScriptThread::SCRCMD_CREATE_OBJ_507CE0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_OBJ_DATA* pObjData = reinterpret_cast<SCR_OBJ_DATA*>(gBasePtr_6F8070);

    switch (pObjData->field_2_type)
    {
        case SCRCMD_CREATE_OBJ_3D_INT:
        case SCRCMD_CREATE_OBJ_2D_STR:
            ScriptThread::SCRCMD_OBJ_DECSET_2D_3D_503680(pObjData, pPointer);
            break;
        case SCRCMD_CREATE_OBJ_2D:
        case SCRCMD_CREATE_OBJ_3D:
        case SCRCMD_CREATE_OBJ_2D_INT:
        case SCRCMD_CREATE_OBJ_3D_STR:
            ScriptThread::SCRCMD_OBJ_DECSET_5038D0(pObjData, pPointer);
            break;
        default:
            break;
    }
    if (!field_118_in_level_start)
    {
        gMiss2_25C_6F805C->push_type_2_502FF0(pPointer->field_8_obj, false);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507f80)
void ScriptThread::SCRCMD_CREATE_CAR_507F80()
{
    SCR_CAR_DATA_DEC* pCmd = reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070);

    //  TODO: gBasePtr_6F8070 may be an extended union of SCR_CMD_HEADER
    Fix16* z_coord = reinterpret_cast<Fix16*>(&static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[2].field_4_cmd_next);

    if (*z_coord == kFp255_6F7570)
    {
        *z_coord = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(*reinterpret_cast<Fix16*>(&static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[1].field_4_cmd_next),
                                                                 *reinterpret_cast<Fix16*>(&static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[2].field_0_cmd_this));
    }
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_car_idx));
    ScriptThread::SCRCMD_CAR_DECSET_503BC0(reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070), pPointer);
    if (!field_118_in_level_start)
    {
        gMiss2_25C_6F805C->push_type_1_car_502F80(pPointer->field_8_car);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508220)
void ScriptThread::SCRCMD_MAKE_CAR_DUMMY_508220()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (pPointer->field_8_car != NULL)
    {
        if (!pPointer->field_8_car->GetDriver_4118B0())
        {
            pPointer->field_8_car->SpawnDriverPed();
        }
        pPointer->field_8_car->InitCarAIControl_440590();

        pPointer->field_8_car->SetDriverPedType_421560(ped_type::script_created_5);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508280)
void ScriptThread::SCRCMD_SET_DENSITY_508280()
{
    SCR_MAP_SET_ZONE_INFO* pCmd = reinterpret_cast<SCR_MAP_SET_ZONE_INFO*>(gBasePtr_6F8070);
    str_table_entry* pStrTable = gScriptManager_6F8060->FindStringById_503080(pCmd->field_8_zone_idx);

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_SET_CAR_DENSITY:
            gMap_0x370_6F6268->SetZoneCarDensity_4DF9D0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_GOOD_CAR:
            gMap_0x370_6F6268->SetZoneGoodCarRatio_4DF9F0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_BAD_CAR:
            gMap_0x370_6F6268->SetZoneBadCarRatio_4DFA10(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_POLICE_CAR:
            gMap_0x370_6F6268->SetZonePoliceCarRatio_4DFA30(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_PED_DENSITY:
            gMap_0x370_6F6268->SetZonePedDensity_4DFA70(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_MUGGER:
            gMap_0x370_6F6268->SetZoneMuggerRatio_4DFA90(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_CARTHIEF:
            gMap_0x370_6F6268->SetZoneCarThiefRatio_4DFAB0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_ELVIS:
            gMap_0x370_6F6268->SetZoneElvisRatio_4DFAD0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_GANG:
            gMap_0x370_6F6268->SetZoneGangCharRatio_4DFAF0(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_POLICE_PED:
            gMap_0x370_6F6268->SetZonePolicePedRatio_4DFB10(pStrTable->field_2_zone_idx, pCmd->field_C_value);
            break;
        case SCRCMD_SET_GANGCARRATIO:
            gMap_0x370_6F6268->SetZoneGangCarRatio_4DFA50(pStrTable->field_2_zone_idx, pCmd->field_C_value);
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508550)
void ScriptThread::SCRCMD_POINT_ARROW_3D_508550() //  SCRCMD_POINT_ARROW_3D and SCRCMD_LEVEL_END_ARROW2
{
    SCR_LEVEL_END_ARROW2* pCmd = reinterpret_cast<SCR_LEVEL_END_ARROW2*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[2].field_4_cmd_next));
    if (pPointer->field_8_arrow == NULL)
    {
        pPointer->field_8_arrow = gHud_2B00_706620->field_1F18_arrows.AllocArrow_5D1050();
    }

    pPointer->field_8_arrow->SetArrowAim_476840(pCmd->field_8_pos.x, pCmd->field_8_pos.y, pCmd->field_8_pos.z);

    if (gBasePtr_6F8070->field_2_type == SCRCMD_LEVEL_END_ARROW2)
    {
        pPointer->field_8_arrow->SetArrowColour_5D0510(5);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5086f0)
void ScriptThread::SCRCMD_POINT_ARROW_AT_5086F0()
{
    SCR_POINT_ARROW_AT* pParams = reinterpret_cast<SCR_POINT_ARROW_AT*>(gBasePtr_6F8070);
    SCR_POINT_ARROW_AT* pCmd = reinterpret_cast<SCR_POINT_ARROW_AT*>(gBasePtr_6F8070);

    SCR_POINTER* pArrowPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pParams->field_C_arrow_idx));
    SCR_ARROW_ENTITY* pEntityPtr = static_cast<SCR_ARROW_ENTITY*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_target_entity_idx));

    if (pArrowPtr->field_8_arrow == NULL)
    {
        pArrowPtr->field_8_arrow = gHud_2B00_706620->field_1F18_arrows.AllocArrow_5D1050();
    }

    switch (ScriptThread::GetEntityTypeOfCommand_503410(pEntityPtr->field_2_type))
    {
        case script_entity_type::car_2:
        {
            pArrowPtr->field_8_arrow->SetArrowTargetCar_476860(pEntityPtr->field_8_car);
            break;
        }

        case script_entity_type::char_1:
        {
            Ped* pChar = pEntityPtr->field_8_char;

            if (!pChar->is_player_41B0A0() || pChar->field_15C_player->get_idx_4219D0() != gGame_0x40_67E008->GetLocalPlayerIdx_476790())
            {
                pArrowPtr->field_8_arrow->SetArrowTargetPed_476850(pChar);
            }

            break;
        }

        case script_entity_type::object_3:
        {
            pArrowPtr->field_8_arrow->SetArrowTargetObject_476870(pEntityPtr->field_8_obj);
            break;
        }

        case script_entity_type::crane_4:
            pArrowPtr->field_8_arrow->SetArrowAim_476840(reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pEntityPtr)->field_10_pos.field_0_x,
                                                         reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pEntityPtr)->field_10_pos.field_4_y,
                                                         kFpOne_6F77C4);
            break;

        case script_entity_type::conveyor_5:
            pArrowPtr->field_8_arrow->SetArrowAim_476840(reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->field_C_rect.field_0_pos.x,
                                                         reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->field_C_rect.field_0_pos.y,
                                                         reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->field_C_rect.field_0_pos.z);
            break;

        default:
            break;
    }

    if (gBasePtr_6F8070->field_2_type == SCRCMD_LEVEL_END_ARROW1)
    {
        pArrowPtr->field_8_arrow->SetArrowColour_5D0510(5);
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508dc0)
void ScriptThread::SCRCMD_ARROW_COLOUR_508DC0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pPointer->field_8_arrow == NULL)
    {
        pPointer->field_8_arrow = gHud_2B00_706620->field_1F18_arrows.AllocArrow_5D1050();
    }
    pPointer->field_8_arrow->SetArrowColour_5D0510(pCmd->field_A_unsigned_2);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508e80)
void ScriptThread::SCRCMD_REMOVE_ARROW_508E80()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Hud_Arrow* pArrow = pPointer->field_8_arrow;

    if (pArrow != NULL)
    {
        pArrow->field_18_targets.field_18_primary_target.field_10_target_type = ArrowTargetType::Nothing_0;
        pArrow->field_18_targets.field_3C_secondary_target.field_10_target_type = ArrowTargetType::Nothing_0;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508f00)
void ScriptThread::SCRCMD_CHECK_CAR_DAMAGE_508F00()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Car* pCar = pPointer->field_8_car;
    if (pCar->field_74_damage >= 320 * pCmd->field_A_signed_2)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509030)
void ScriptThread::SCRCMD_CHECK_HEALTH_509030()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Ped* pPed = pPointer->field_8_char;
    s16 health_param = pCmd->field_A_signed_2;

    if (pPed != NULL && pPed->field_216_health >= health_param)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5093c0)
void ScriptThread::SCRCMD_DISPLAY_MESSAGE_5093C0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    u8 type = static_cast<u8>(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->field_A_unsigned_2);
    if (type == 1)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::jobcomplete_22);
    }
    else if (type == 2)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::jobfail_23);
    }

    if (pCmd->field_8_unsigned_1 == 5501)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::raceover_33);
    }
    else if (pCmd->field_8_unsigned_1 == 5502)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::second_lap_34);
    }
    else if (pCmd->field_8_unsigned_1 == 5503)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::finallap_35);
    }
    else if (pCmd->field_8_unsigned_1 == 5504)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::raceon_36);
    }
    else if (pCmd->field_8_unsigned_1 == 5505)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_30_37);
    }
    else if (pCmd->field_8_unsigned_1 == 5506)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_60_38);
    }
    else if (pCmd->field_8_unsigned_1 == 5507)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_90_39);
    }
    else if (pCmd->field_8_unsigned_1 == 5508)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_120_40);
    }
    else if (pCmd->field_8_unsigned_1 == 5509)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_150_41);
    }
    else if (pCmd->field_8_unsigned_1 == 5510)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::timeout_42);
    }
    else if (pCmd->field_8_unsigned_1 == 5000)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::timeextended_43);
    }
    else if (pCmd->field_8_unsigned_1 == 5015)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::timesup_44);
    }
    else if (pCmd->field_8_unsigned_1 == 5031)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::sorrydidthathurt_45);
    }
    else if (pCmd->field_8_unsigned_1 == 5032)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::nicework_46);
    }
    else if (pCmd->field_8_unsigned_1 == 5050)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::choctastic_47);
    }
    else if (pCmd->field_8_unsigned_1 == 5051)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::raspberryripple_48);
    }
    else if (pCmd->field_8_unsigned_1 == 5052)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::youshotyourload_49);
    }
    else if (pCmd->field_8_unsigned_1 == 5053)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::oohdidthathurt_50);
    }
    else if (pCmd->field_8_unsigned_1 == 5054)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::deathtoicvans_51);
    }
    else if (pCmd->field_8_unsigned_1 == 5055)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::crispycritter_52);
    }
    else if (pCmd->field_8_unsigned_1 == 5056)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::youretoastbuddy_53);
    }
    else if (pCmd->field_8_unsigned_1 == 5057)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::eatleaddeath_54);
    }
    else if (pCmd->field_8_unsigned_1 == 5058)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::thatsgottahurt_55);
    }
    else if (pCmd->field_8_unsigned_1 == 5059)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::sorryaboutthat_56);
    }
    else if (pCmd->field_8_unsigned_1 == 5060)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::xinloimyman_57);
    }
    else if (pCmd->field_8_unsigned_1 == 5061)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::damnsundaydrivers_58);
    }
    else if (pCmd->field_8_unsigned_1 == 5062)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::suckitandsee_59);
    }
    else if (pCmd->field_8_unsigned_1 == 5063)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::tastemywrath_60);
    }

    sprintf(gTmpBuffer_67C598, "%d", pCmd->field_8_unsigned_1);

    switch (pCmd->field_2_type)
    {
        case SCRCMD_DISPLAY_MESSAGE:
            gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598), hud_message_priority::important_3);
            break;
        case SCRCMD_DISPLAY_BRIEF:
            gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
            break;
        case SCRCMD_DISPLAY_BRIEF_NOW:
            gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::now_3, gTmpBuffer_67C598);
            break;
        case SCRCMD_DISPLAY_BRIEF_SOON:
            gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::soon_2, gTmpBuffer_67C598);
            break;
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5096e0)
void ScriptThread::SCRCMD_DISPLAY_TIMER_5096E0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    pPointer->field_8_index = gHud_2B00_706620->field_620_pagers.CreateTimer_5D31F0(pCmd->field_A_signed_2);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509730)
void ScriptThread::SCRCMD_CREATE_CHAR_509730()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    ScriptThread::SCRCMD_CHAR_DECSET_2D_3D_503FB0(reinterpret_cast<SCR_CHAR_DATA_DEC*>(gBasePtr_6F8070), pPointer);
    if (!field_118_in_level_start)
    {
        gMiss2_25C_6F805C->push_type_3_ped_502FB0(pPointer->field_8_char);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509790)
void ScriptThread::SCRCMD_SET_CHAR_OCCUPATION_509790()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    pPointer->field_8_char->field_240_occupation = pCmd->field_C_u32;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5097d0)
void ScriptThread::SCRCMD_SET_CHAR_OBJ1_5097D0()
{
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index)); //  TODO: fix gBasePtr_6F8070
    ScriptThread::SetCharObjective_504110(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070), pPointer);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509810)
void ScriptThread::SCRCMD_IS_CHAR_OBJ_PASS_509810()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        switch (pPed->field_225_objective_status)
        {
            case objective_status::failed_2:
            case objective_status::not_finished_0:
                field_8_cond_result = false;
                break;
            case objective_status::passed_1:
                field_8_cond_result = true;
                break;
            default:
                break;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509880)
void ScriptThread::SCRCMD_IS_CHAR_OBJ_FAIL_509880()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        switch (pPed->field_225_objective_status)
        {
            case objective_status::failed_2:
                field_8_cond_result = true;
                break;
            default:
                field_8_cond_result = false;
                break;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5098e0)
void ScriptThread::SCRCMD_GIVE_WEAPON_5098E0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_FOUR_PARAMS* pCmd4 = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->field_2_type) == script_entity_type::char_1)
    {
        if (gBasePtr_6F8070->field_2_type == SCRCMD_GIVE_WEAPON1)
        {
            pPointer->field_8_char->AddWeaponWithAmmo_45DD30(pCmd->field_A_signed_2, 100);
        }
        else
        {
            pPointer->field_8_char->AddWeaponWithAmmo_45DD30(pCmd4->field_A_signed_2, static_cast<char_type>(pCmd4->field_C_signed_3));
        }
    }
    else
    {
        // Always 50 ammo for cars
        gWeapon_8_707018->allocate_5E3D50(pCmd->field_A_signed_2, 50, pPointer->field_8_car);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509990)
void ScriptThread::SCRCMD_IS_CAR_IN_BLOCK_509990()
{
    SCR_IS_CAR_IN_BLOCK* pCmd = reinterpret_cast<SCR_IS_CAR_IN_BLOCK*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pCmd->field_C_pos.z == kFp255_6F7570)
    {
        pCmd->field_C_pos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->field_C_pos.x, pCmd->field_C_pos.y);
    }

    Car* pCar = pPointer->field_8_car;

    if (pCar != NULL && pCar->IsWithinBlock(&pCmd->field_C_pos))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509a70)
void ScriptThread::SCRCMD_CAR_IN_AREA_509A70()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Fix16 height = pCmd->field_C_rect.field_C_size.field_4_y;
    Fix16 width = pCmd->field_C_rect.field_C_size.field_0_x;
    Fix16 top = pCmd->field_C_rect.field_0_pos.y - height / kFpTwo_6F77C8;
    Fix16 left = pCmd->field_C_rect.field_0_pos.x - width / kFpTwo_6F77C8;

    gScriptManager_6F8060->field_2F8_area_rect.SetFromPosSize_463710(left, top, width, height);

    Fix16 hz = kFpHalf_6F75F0 + pCmd->field_C_rect.field_0_pos.z;
    Fix16 lz = pCmd->field_C_rect.field_0_pos.z - kFpHalf_6F75F0;
    gScriptManager_6F8060->field_2F8_area_rect.SetHiLowZ_41E370(lz, hz);

    if (pPointer->field_8_car != NULL && pPointer->field_8_car->field_88_despawn_status != car_despawn_status::despawned_6 &&
        (pPointer->field_8_car->field_50_car_sprite->IntersectsRectSAT_59FB10(&gScriptManager_6F8060->field_2F8_area_rect) ||
         gScriptManager_6F8060->field_2F8_area_rect.IntersectsSpriteRenderingRect_59DDF0(pPointer->field_8_car->field_50_car_sprite)))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509bb0)
void ScriptThread::SCRCMD_HAS_CHAR_DIED_509BB0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (pPointer->field_8_char && pPointer->field_8_char->field_278_ped_state_1 == ped_state_1::dead_9)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509c10)
void ScriptThread::SCRCMD_IS_CHAR_IN_CAR_509C10()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    Ped* pPed = pCharPointer->field_8_char;

    Car* pCarTarget;

    if (pPed != NULL && (pCarTarget = pCarPointer->field_8_car) != NULL && pPed->has_car_403B80() &&
        pPed->get_car_416B60() == pCarTarget)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509c90)
void ScriptThread::SCRCMD_IS_CHAR_IN_MODEL_509C90()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Ped* pPed = pCharPointer->field_8_char;

    if (pPed != NULL && pPed->has_car_403B80() && pPed->get_car_416B60()->GetCarInfoIdx_411940() == pCmd->field_A_signed_2)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d00)
void ScriptThread::SCRCMD_IS_CHAR_IN_ANY_CAR_509D00()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL && pPed->has_car_403B80())
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d60)
void ScriptThread::SCRCMD_DELETE_ITEM_509D60()
{
    ScriptThread::DeallocOrDeleteItem_505B10(gBasePtr_6F8070->field_8_index); //  delete item
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d90)
void ScriptThread::SCRCMD_ADD_SCORE_509D90()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        if (pPed->is_player_41B0A0())
        {
            Player* pPlayer = pPed->field_15C_player;
            if (gBasePtr_6F8070->field_2_type == SCRCMD_ADD_SCORE1)
            {
                pPlayer->AddScore_41DC40(pCmd->field_C_s32);
            }
            else
            {
                pPlayer->AddCash_421990(pCmd->field_C_s32); //  no multiplier
            }
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509e00)
void ScriptThread::SCRCMD_ADD_SCORE2_509E00()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_s32));

    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        if (pPed->is_player_41B0A0())
        {
            pPed->field_15C_player->AddScore_41DC40(pCounter->field_8_counter);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509e70)
void ScriptThread::SCRCMD_IS_CHAR_STOPPED_509E70()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    Fix16 velocity;

    if (pPed != NULL && (velocity = pPed->field_168_game_object->field_38_velocity, velocity == kFpZero_6F77C0))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509ed0)
void ScriptThread::SCRCMD_EXPLODE_509ED0()
{
    SCR_EXPLODE* pCmd = reinterpret_cast<SCR_EXPLODE*>(gBasePtr_6F8070);
    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_EXPLODE_NO_RING2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.x,
                                                      pCmd->field_8_pos.y,
                                                      pCmd->field_8_pos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::no_ring_32,
                                                      0);
            break;
        case SCRCMD_EXPLODE_SMALL2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.x,
                                                      pCmd->field_8_pos.y,
                                                      pCmd->field_8_pos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::small_18,
                                                      0);
            break;
        case SCRCMD_EXPLODE:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.x,
                                                      pCmd->field_8_pos.y,
                                                      pCmd->field_8_pos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::item_19,
                                                      0);
            break;
        case SCRCMD_EXPLODE_LARGE2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.x,
                                                      pCmd->field_8_pos.y,
                                                      pCmd->field_8_pos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::large_20,
                                                      0);
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509f60)
void ScriptThread::SCRCMD_EXPLODE_BUILDING_509F60()
{
    SCR_EXPLODE_WALL* pCmd = reinterpret_cast<SCR_EXPLODE_WALL*>(gBasePtr_6F8070);
    switch (pCmd->field_14_face)
    {
        case 1:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.x,
                                                      pCmd->field_8_pos.y,
                                                      pCmd->field_8_pos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_225_23,
                                                      0);
            break;
        case 2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.x,
                                                      pCmd->field_8_pos.y,
                                                      pCmd->field_8_pos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_45_22,
                                                      0);
            break;
        case 3:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.x,
                                                      pCmd->field_8_pos.y,
                                                      pCmd->field_8_pos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_135_24,
                                                      0);
            break;
        case 4:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->field_8_pos.x,
                                                      pCmd->field_8_pos.y,
                                                      pCmd->field_8_pos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_315_25,
                                                      0);
            break;
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509fd0)
void ScriptThread::Locate_509FD0()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_varname));
    void* pObj;
    Fix16 vel;

    if (pPointer->field_8_char)
    {
        Fix16 pedX = pPointer->field_8_char->field_1AC_cam.x;
        Fix16 pedY = pPointer->field_8_char->field_1AC_cam.y;
        Fix16 pedZ = pPointer->field_8_char->field_1AC_cam.z;
        Fix16 width = pCmd->field_C_rect.field_C_size.field_0_x / kFpTwo_6F77C8;
        Fix16 height = pCmd->field_C_rect.field_C_size.field_4_y / kFpTwo_6F77C8;
        field_8_cond_result = false;

        if (pedX >= pCmd->field_C_rect.field_0_pos.x - width && pedX <= pCmd->field_C_rect.field_0_pos.x + width &&
            pedY >= pCmd->field_C_rect.field_0_pos.y - height && pedY <= pCmd->field_C_rect.field_0_pos.y + height &&
            pedZ.ToUInt8() == pCmd->field_C_rect.field_0_pos.z.ToUInt8())
        {
            switch (gBasePtr_6F8070->field_2_type)
            {
                case SCRCMD_LOCATE_CHAR_ANY:
                    field_8_cond_result = true;
                    break;
                case SCRCMD_LOCATE_CHAR_ONFOOT:
                    if ((pObj = pPointer->field_8_char->field_168_game_object) != NULL)
                    {
                        field_8_cond_result = true;
                    }

                    break;
                case SCRCMD_LOCATE_CHAR_BY_CAR:
                    if ((pObj = pPointer->field_8_char->field_16C_car) != NULL)
                    {
                        field_8_cond_result = true;
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_ANY:
                    if (pPointer->field_8_char->field_168_game_object &&
                        (vel = pPointer->field_8_char->field_168_game_object->field_38_velocity, vel == kFpZero_6F77C0))
                    {
                        field_8_cond_result = true;
                    }
                    else if (pPointer->field_8_char->field_16C_car && pPointer->field_8_char->GetPedVelocity_45C920() == kFpZero_6F77C0)
                    {
                        field_8_cond_result = true;
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_FOOT:
                    if (pPointer->field_8_char->field_168_game_object)
                    {
                        vel = pPointer->field_8_char->field_168_game_object->field_38_velocity;
                        if (vel == kFpZero_6F77C0)
                        {
                            field_8_cond_result = true;
                        }
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_CAR:
                    if (pPointer->field_8_char->field_16C_car && pPointer->field_8_char->GetPedVelocity_45C920() == kFpZero_6F77C0)
                    {
                        field_8_cond_result = true;
                    }

                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidCase,
                                      "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                                      4622,
                                      gBasePtr_6F8070->field_2_type);
                    break;
            }
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a200)
void ScriptThread::SCRCMD_SET_CHAR_OBJ2_50A200()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    ScriptThread::SetCharObjective_504110(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070), pPointer);

    SCR_POINTER* pParam;
    switch (pCmd->field_A_objective)
    {
        case objectives_enum::flee_char_on_foot_till_safe_2:
        case objectives_enum::flee_char_on_foot_always_3:
        case objectives_enum::flee_char_any_means_till_safe_4:
        case objectives_enum::flee_char_any_means_always_5:
        case objectives_enum::flee_char_always_once_car_stopped_6:
        case objectives_enum::goto_char_on_foot_16:
        case objectives_enum::kill_char_any_means_19:
        case objectives_enum::kill_char_on_foot_20:
        case objectives_enum::punch_char_23:
            pParam = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_second_item_idx));
            pPointer->field_8_char->set_objective_target_ped_403AC0(pParam->field_8_char);
            break;
        case objectives_enum::enter_car_as_driver_35:
            pParam = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_second_item_idx));
            pPointer->field_8_char->SetTargetObjectiveCar(pParam->field_8_car);
            if (pCmd->field_E_variant == 1)
            {
                pPointer->field_8_char->set_enter_car_as_passenger_4039B0(true);
            }
            else
            {
                pPointer->field_8_char->set_enter_car_as_passenger_4039B0(false);
            }
            break;
        case objectives_enum::kill_car_21:
        case objectives_enum::wait_in_car_27:
        case objectives_enum::leave_car_36:
        case objectives_enum::follow_car_in_car_55:
        case objectives_enum::destroy_car_59:
            pParam = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_second_item_idx));
            pPointer->field_8_char->SetTargetObjectiveCar(pParam->field_8_car);
            break;
        case objectives_enum::fire_at_object_from_vehicle_57:
        case objectives_enum::destroy_object_58:
            pParam = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_second_item_idx));
            pPointer->field_8_char->field_1A0_objective_target_object = pParam->field_8_obj;
            break;
        default:
            break;
    }

    Ped* pPed = pPointer->field_8_char;
    pPed->field_21C_bf.bScheduledForRemoval = false;

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a3e0)
void ScriptThread::SCRCMD_SET_CHAR_OBJ3_50A3E0()
{
    SCR_CHAR_OBJ3* pCmd = reinterpret_cast<SCR_CHAR_OBJ3*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    ScriptThread::SetCharObjective_504110(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070), pPointer);

    if (pPointer->field_8_char)
    {
        (pPointer->field_8_char)->field_1DC_objective_target_x = pCmd->field_C_pos.x.mValue; //  TODO: May be Fix16 in the original
        (pPointer->field_8_char)->field_1E0_objective_target_y = pCmd->field_C_pos.y.mValue;
        (pPointer->field_8_char)->field_1E4_objective_target_z = pCmd->field_C_pos.z.mValue;

        (pPointer->field_8_char)->field_21C_bf.bScheduledForRemoval = false; // TODO: Maybe BitSet32
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a460)
void ScriptThread::SCRCMD_SET_CHAR_OBJ_FOLLOW_50A460()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index)); //  TODO: fix gBasePtr_6F8070 type
    ScriptThread::SetCharObjective_504110(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070), pPointer);

    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_car_idx));

    if (pPointer->field_8_char)
    {
        (pPointer->field_8_char)->SetTargetObjectiveCar(pCarPointer->field_8_car);

        Ang16 cmd_rotation;
        cmd_rotation.rValue = pCmd->field_E_rotation;

        Fix16 fix_1;
        fix_1.mValue = cmd_rotation.FromUnsignedToFloat();
        Fix16 fix_2;
        fix_2.mValue = kDegreesToAng16_6F8044.ToFloat();
        Fix16 fix_3;
        fix_3.mValue = fix_2.MultiplyInt64(fix_1);

        Ang16 rotation;
        rotation.rValue = fix_3.ToInt();
        rotation.Normalize();

        (pPointer->field_8_char)->field_132_follow_car_offset_angle = rotation.rValue;
        (pPointer->field_8_char)->field_1FC_follow_car_offset_distance = pCmd->field_12_offset;
        (pPointer->field_8_char)->field_21C_bf.bScheduledForRemoval = false; // TODO: Maybe BitSet32
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a570)
void ScriptThread::SCRCMD_CHANGE_CAR_REMAP_50A570()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pCmd->field_A_unsigned_2 != 0xFFu) //  field_A_unsigned_2 = remap
    {
        pPointer->field_8_car->SetCarRemap(static_cast<u8>(pCmd->field_A_unsigned_2));
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a5b0)
void ScriptThread::SCRCMD_CHANGE_CHAR_REMAP_50A5B0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (pPointer->field_8_char)
    {
        pPointer->field_8_char->SetRemap_433C10(static_cast<u8>(pCmd->field_A_unsigned_2));
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a610)
void ScriptThread::SCRCMD_CHECK_CAR_MODEL_50A610()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Car* pCar = pPointer->field_8_car;
    s16 car_model_index = pCmd->field_A_signed_2;

    if (pCar != NULL && static_cast<s8>(pCar->field_84_car_info_idx) == car_model_index)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a670)
void ScriptThread::SCRCMD_CHECK_CAR_REMAP_50A670()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Car* pCar = pPointer->field_8_car;
    s16 remap = pCmd->field_A_signed_2;

    if (pCar != NULL && pCar->field_88_despawn_status != car_despawn_status::despawned_6 && static_cast<u16>(pCar->field_50_car_sprite->field_24_remap) == remap)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a6e0)
void ScriptThread::SCRCMD_CHECK_CAR_BOTH_50A6E0()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Car* pCar = pPointer->field_8_car;
    s16 car_model_idx = pCmd->field_A_signed_2;
    s16 remap = pCmd->field_C_signed_3;

    if (pCar != NULL && pCar->field_88_despawn_status != car_despawn_status::despawned_6 && static_cast<s8>(pCar->field_84_car_info_idx) == car_model_idx &&
        static_cast<u16>(pCar->field_50_car_sprite->field_24_remap) == remap)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a760)
void ScriptThread::IsOnScreen_50A760()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    bool onScreen;

    switch (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->field_2_type))
    {
        case script_entity_type::car_2:
        {
            if (!pPointer->field_8_car)
            {
                FatalError_4A38C0(1102, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1376, static_cast<u16>(gBasePtr_6F8070->field_0_cmd_this));
            }

            Car* pCar = pPointer->field_8_car;

            if (pCar->field_88_despawn_status == car_despawn_status::despawned_6)
            {
                onScreen = false;
                break;
            }

            if (gBasePtr_6F8070->field_2_type == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pCar->field_50_car_sprite, 1);
            }
            else
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pCar->field_50_car_sprite, 0);
            }

            break;
        }
        case script_entity_type::char_1:
            if (!pPointer->field_8_char)
            {
                FatalError_4A38C0(1103, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1383, static_cast<u16>(gBasePtr_6F8070->field_0_cmd_this));
            }

            if (gBasePtr_6F8070->field_2_type == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pPointer->field_8_char->GetSprite_46DF50(), 1);
            }
            else
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pPointer->field_8_char->GetSprite_46DF50(), 0);
            }

            break;
        case script_entity_type::object_3:
            if (!pPointer->field_8_obj)
            {
                FatalError_4A38C0(1104, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x138E, static_cast<u16>(gBasePtr_6F8070->field_0_cmd_this));
            }

            if (gBasePtr_6F8070->field_2_type == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pPointer->field_8_obj->field_4_sprite, 1);
            }
            else
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pPointer->field_8_obj->field_4_sprite, 0);
            }

            break;
        case script_entity_type::crane_4:
            if (!pPointer->field_8_crane)
            {
                FatalError_4A38C0(1108, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1399, static_cast<u16>(gBasePtr_6F8070->field_0_cmd_this));
            }

            if (gBasePtr_6F8070->field_2_type == SCRCMD_IS_ITEM_ONSCREEN)
            {
                Sprite* pSprite = pPointer->field_8_crane->GetRotorSprite_411A00();
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pSprite, 1);
            }
            else
            {
                Sprite* pSprite = pPointer->field_8_crane->GetRotorSprite_411A00();
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pSprite, 0);
            }

            break;
        default:
            onScreen = false;
            break;
    }

    if (onScreen)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a940)
void ScriptThread::SCRCMD_DELAY_HERE_50A940()
{
    SCR_CMD_HEADER* pNextCmd = static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070) + 1; //  TODO: remove this hack

    if (static_cast<s16>(gBasePtr_6F8070->field_8_index) == -1)
    {
        pNextCmd->field_0_cmd_this = static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[1].field_2_type;
    }
    else
    {
        pNextCmd->field_0_cmd_this = gBasePtr_6F8070->field_8_index - 1;
        if (!pNextCmd->field_0_cmd_this)
        {
            pNextCmd->field_0_cmd_this = -1;
            ScriptThread::Next_503620(gBasePtr_6F8070);
        }
    }
}

MATCH_FUNC(0x50a980)
void ScriptThread::SCRCMD_DELAY_50A980()
{
    s16 delay = gBasePtr_6F8070->field_8_index;
    SCR_CMD_HEADER* pNextCmd = static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070) + 1; //  TODO: remove this hack

    if (delay == -1)
    {
        pNextCmd->field_0_cmd_this = static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[1].field_2_type;
        field_8_cond_result = true;
    }
    else
    {
        pNextCmd->field_0_cmd_this = delay - 1;
        if (!pNextCmd->field_0_cmd_this)
        {
            pNextCmd->field_0_cmd_this = -1;
            field_8_cond_result = false;
        }
        else
        {
            field_8_cond_result = true;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a9e0)
void ScriptThread::EnableThread_50A9E0(u16 idx)
{
    SCR_CMD_HEADER* pCmd = gScriptManager_6F8060->GetBasePointer_512770(idx);

    switch (pCmd->field_2_type)
    {
        case SCRCMD_THREAD_DECLARE2:
            SCRCMD_THREAD_DECLARE2_5045D0(static_cast<SCR_THREAD*>(pCmd), reinterpret_cast<s16*>(pCmd));
            break;
        case SCRCMD_THREAD_DECLARE3:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);

            if (pThread->field_16_trigger_added)
            {
                gCarThreadTriggers_214_705F20->SetTriggerEnableState_4768C0(pThread->field_15_trigger_idx, car_thread_trigger_state::armed_1);
            }
            else
            {
                SCRCMD_THREAD_DECLARE3_504660(static_cast<SCR_THREAD*>(pCmd));
            }

            break;
        }
        case SCRCMD_THREAD_DECLARE4:
            SCRCMD_THREAD_DECLARE4_5047C0(static_cast<SCR_THREAD*>(pCmd), reinterpret_cast<s16*>(pCmd));
            break;
        case SCRCMD_THREAD_DECLARE5:
        case SCRCMD_CHAR_AREA_ANY_MEANS:
        {
            SCR_CHAR_AREA_ANY* pAny = static_cast<SCR_CHAR_AREA_ANY*>(pCmd);

            if (pAny->field_27_trigger_added)
            {
                gCarThreadTriggers_214_705F20->SetTriggerEnableState_4768C0(pAny->field_26_trigger_idx, car_thread_trigger_state::armed_1);
            }
            else
            {
                SCRCMD_THREAD_DECLARE5_504710(pAny);
            }

            break;
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50abc0)
void ScriptThread::SCRCMD_DISABLE_THREAD_50ABC0()
{
    ScriptThread::DisableThread_505790(gBasePtr_6F8070->field_8_index);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50abf0)
void ScriptThread::SCRCMD_ENABLE_THREAD_50ABF0()
{
    ScriptThread::EnableThread_50A9E0(gBasePtr_6F8070->field_8_index);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ac20)
void ScriptThread::SCRCMD_SET_GANG_KILL_REACTION_50AC20()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    str_table_entry* gang_1_str = gScriptManager_6F8060->FindStringById_503080(gBasePtr_6F8070->field_8_index);
    if (gang_1_str == NULL)
    {
        FatalError_4A38C0(Gta2Error::UnableToFindGangMatchingNameInSetGangRespect,
                          "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                          5217,
                          static_cast<u16>(gBasePtr_6F8070->field_0_cmd_this));
    }
    Gang* pGang_1 = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_1_str->get_name());
    str_table_entry* gang_2_str = gScriptManager_6F8060->FindStringById_503080(pCmd->field_C_unsigned_3);
    if (gang_2_str == NULL)
    {
        FatalError_4A38C0(Gta2Error::UnableToFindGangMatchingNameInSetGangRespect2,
                          "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                          5223,
                          static_cast<u16>(gBasePtr_6F8070->field_0_cmd_this));
    }
    Gang* pGang_2 = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_2_str->get_name());
    pGang_1->SetGangKillReaction_4BEF50(pGang_2->field_1_gang_idx, static_cast<char_type>(pCmd->field_A_unsigned_2));
    pGang_1->SetKillAffectsRespect_475900(true);
    pGang_2->SetKillAffectsRespect_475900(true);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50acf0)
void ScriptThread::SCRCMD_CHANGE_RESPECT_50ACF0()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);

    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(gBasePtr_6F8070->field_8_index);

    char_type* gang_name = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_name);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_unsigned_3));

    // pCmd->field_A_signed_2 = respect parameter

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_SET_CHAR_RESPECT:

            pGang->SetRespect_4BEE30(pPointer->field_8_char->field_15C_player->get_idx_4219D0(), 20 * static_cast<u8>(pCmd->field_A_signed_2));
            pGang->SetKillAffectsRespect_475900(true);
            break;
        case SCRCMD_CHANGE_RESPECT:
            if (pCmd->field_A_signed_2 > 0)
            {
                pGang->IncrementRespect_4BEE50(pPointer->field_8_char->field_15C_player->get_idx_4219D0(), 20 * (pCmd->field_A_signed_2));
            }
            else
            {
                pGang->DecrementRespect_4BEEA0(pPointer->field_8_char->field_15C_player->get_idx_4219D0(), 20 * abs(pCmd->field_A_signed_2));
            }
            break;
        case SCRCMD_CHANGE_GANG_RESP:
            pGang->ChangeRespectAndUpdate_4BF000(pPointer->field_8_char->field_15C_player->get_idx_4219D0(), 20 * static_cast<u8>(pCmd->field_A_signed_2));
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50aef0)
void ScriptThread::RespectOperator_50AEF0()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_unsigned_1));
    str_table_entry* pStr = gScriptManager_6F8060->FindStringById_503080(pCmd->field_C_unsigned_3);
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pStr->get_name());

    switch (pCmd->field_2_type)
    {
        case SCRCMD_CHECK_RESPECT_IS:
        {
            u8 player_idx = pPointer->field_8_char->field_15C_player->get_idx_4219D0();

            if (pGang->GetRespectForPlayer_4BEEF0(player_idx) / 20 == pCmd->field_A_signed_2)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            break;
        }
        case SCRCMD_CHECK_RESPECT_LESS:
        {
            u8 player_idx = pPointer->field_8_char->field_15C_player->get_idx_4219D0();

            if (pGang->GetRespectForPlayer_4BEEF0(player_idx) / 20 < pCmd->field_A_signed_2)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            break;
        }
        case SCRCMD_CHECK_RESPECT_GREATER:
        {
            u8 player_idx = pPointer->field_8_char->field_15C_player->get_idx_4219D0();

            if (pGang->GetRespectForPlayer_4BEEF0(player_idx) / 20 > pCmd->field_A_signed_2)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            break;
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b0e0)
void ScriptThread::SCRCMD_ADD_PATROL_POINT_50B0E0()
{
    SCR_ADD_PATROL_POINT* pCmd = reinterpret_cast<SCR_ADD_PATROL_POINT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;
    if (pPed)
    {
        if (pPed->get_objective_403A80() != objectives_enum::patrol_on_foot_42)
        {
            pPed->SetObjective(objectives_enum::patrol_on_foot_42);
        }
        pPointer->field_8_char->PushPatrolPoint_4702A0(pCmd->field_C_pos.x.ToInt(),
                                                       pCmd->field_C_pos.y.ToInt(),
                                                       pCmd->field_C_pos.z.ToInt());
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b150)
void ScriptThread::SCRCMD_SET_AMBIENT_50B150()
{
    SCR_SET_AMBIENT* pCmd = reinterpret_cast<SCR_SET_AMBIENT*>(gBasePtr_6F8070);
    gpMapRenderer_6F66E4->SetAmbientLevel_4E9D50(pCmd->field_8_level, pCmd->field_C_time);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b180)
void ScriptThread::SCRCMD_ANSWER_PHONE_50B180()
{
    SCR_ANSWER_PHONE* pCmd = reinterpret_cast<SCR_ANSWER_PHONE*>(gBasePtr_6F8070);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_ped_idx));
    SCR_POINTER* pPtrObj = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_obj_idx));
    Object* pPhoneObj = pPtrObj->field_8_obj;
    if (pPhoneObj && pPtrPed->field_8_char)
    {
        pPhoneObj->PoolGiveAndMarkDone_5291D0();
        gScriptManager_6F8060->AddPhoneThread_5129B0(pPtrPed->field_8_char->get_id(),
                                           pPtrObj->field_8_obj->field_14_id,
                                           gBasePtr_6F8070->field_0_cmd_this);
        field_8_cond_result = false;
        pCmd->field_12_answered = 0;
        if (pCmd->field_10_timer > 0)
        {
            field_E_phone_timer = pCmd->field_10_timer;
        }
        else
        {
            field_E_phone_timer = -1;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b230)
void ScriptThread::SCRCMD_CHECK_PHONE_50B230()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    ThreadTrigger* pTrigger = gScriptManager_6F8060->FindPhoneThreadByObjId_512AD0(pPointer->field_8_obj->field_14_id);
    SCR_ANSWER_PHONE* pAnswer = static_cast<SCR_ANSWER_PHONE*>(gScriptManager_6F8060->GetBasePointer_512770(pTrigger->field_8_cmd_line));
    if (pAnswer->field_12_answered == 1)
    {
        gScriptManager_6F8060->RemovePhoneThread_512A70(pTrigger->field_0_ped_id, pTrigger->field_4_target_id);
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b2c0)
void ScriptThread::SCRCMD_CHECK_PHONETIMER_50B2C0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    ThreadTrigger* pTrigger = gScriptManager_6F8060->FindPhoneThreadByObjId_512AD0(pPointer->field_8_obj->field_14_id);
    if (pTrigger)
    {
        SCR_ANSWER_PHONE* pAnswer = static_cast<SCR_ANSWER_PHONE*>(gScriptManager_6F8060->GetBasePointer_512770(pTrigger->field_8_cmd_line));
        if (static_cast<u16>(field_E_phone_timer) != 0xFFFF)
        {
            if (field_E_phone_timer == 0 && pAnswer->field_12_answered == 0)
            {
                field_8_cond_result = true;
                pPointer->field_8_obj->PoolGiveAndMarkDone_5291D0();
            }
            else
            {
                --field_E_phone_timer;
                field_8_cond_result = false;
            }
        }
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b360)
void ScriptThread::SCRCMD_STOP_PHONE_RING_50B360()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Object* pObj = pPointer->field_8_obj;
    if (pObj)
    {
        ThreadTrigger* pTrigger = gScriptManager_6F8060->FindPhoneThreadByObjId_512AD0(pObj->field_14_id);
        if (pTrigger)
        {
            gScriptManager_6F8060->RemovePhoneThread_512A70(pTrigger->field_0_ped_id, pTrigger->field_4_target_id);
        }
        pPointer->field_8_obj->PoolGiveAndQueueObjectType_5291E0(163);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b3d0)
void ScriptThread::SCRCMD_IS_CHAR_FIRE_ONSCREEN_50B3D0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;
    if (pPed->GetAttacking_433CA0() && pPed->GetOffscreenCounter_4039F0() == 0 && pPed->field_170_selected_weapon)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b440)
void ScriptThread::SCRCMD_CAR_DRIVE_AWAY_50B440()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    pPointer->field_8_car->InitCarAIControl_440590();
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b470)
void ScriptThread::SCRCMD_GET_CAR_FROM_CRANE_50B470()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer1 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[1].field_2_type));
    SCR_POINTER* pPointer2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_unsigned_1));
    Crane* pCrane = pPointer1->field_8_crane;

    if (pCrane != NULL)
    {
        pPointer2->field_8_car = pCrane->GetCarFromCrane_480DA0();
        if (pPointer2->field_8_car)
        {
            field_8_cond_result = true;
        }
        else
        {
            field_8_cond_result = false;
        }
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b4f0)
void ScriptThread::SCRCMD_CHAR_TO_DRIVE_CAR_50B4F0()
{
    SCR_CHAR_ENTER_CAR* pCmd = reinterpret_cast<SCR_CHAR_ENTER_CAR*>(gBasePtr_6F8070);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pPtrCar = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_car_idx));

    if (pPtrPed->field_8_char)
    {
        if (gBasePtr_6F8070->field_2_type != SCRCMD_CHAR_TO_BACKDOOR)
        {
            pPtrPed->field_8_char->SetObjective2_463830(objectives_enum::no_obj_0);
            pPtrPed->field_8_char->SetObjective(objectives_enum::enter_car_as_driver_35);
            pPtrPed->field_8_char->SetTargetObjectiveCar(pPtrCar->field_8_car);
            pPtrPed->field_8_char->set_enter_car_as_passenger_4039B0(false);
        }
        else
        {
            pPtrPed->field_8_char->EnterCarByBackDoor_45C7A0(pPtrCar->field_8_car);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b5a0)
void ScriptThread::SCRCMD_DRIVER_OUT_CAR_50B5A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pPointer->field_8_char)
    {
        pPointer->field_8_char->SetObjective2_463830(objectives_enum::no_obj_0);
        pPointer->field_8_char->SetObjective(objectives_enum::leave_car_36);
        pPointer->field_8_char->field_150_target_objective_car = pPointer->field_8_char->field_16C_car;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b600)
void ScriptThread::SCRCMD_GIVE_DRIVER_BRAKE_50B600()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pPointer->field_8_car)
    {
        if (pPointer->field_8_car->GetDriver_4118B0() == NULL)
        {
            pPointer->field_8_car->SpawnDriverPed();
        }
        pPointer->field_8_car->InitCarAIControl_440590();
        pPointer->field_8_car->SetAIHalted_421540();
        pPointer->field_8_car->SetDriverPedType_421560(ped_type::script_created_5);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b670)
void ScriptThread::SCRCMD_SET_GANG_INFO1_50B670()
{
    // mid-game change
    ScriptThread::SCRCMD_SET_GANG_INFO1_504830(reinterpret_cast<SCR_SET_GANG_INFO*>(gBasePtr_6F8070));
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b690)
void ScriptThread::SCRCMD_CLEAR_TIMERS_50B690()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (pPointer->field_8_index != 0xFFFF)
    {
        gHud_2B00_706620->field_620_pagers.ClearPager_5D3280(pPointer->field_8_index);
    }
    pPointer->field_8_index = 0;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b6f0)
void ScriptThread::SCRCMD_CHECK_SCORE_50B6F0()
{
    SCR_CHECK_SCORE_GREATER* pCmd = reinterpret_cast<SCR_CHECK_SCORE_GREATER*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        if (pPed->is_player_41B0A0() && pPed->field_15C_player->GetScore_421980() > pCmd->field_C_target_score)
        {
            field_8_cond_result = true;
        }
        else
        {
            field_8_cond_result = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b760)
void ScriptThread::SCRCMD_GET_SCORE_50B760()
{
    SCR_GET_SCORE* pCmd = reinterpret_cast<SCR_GET_SCORE*>(gBasePtr_6F8070);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pPtrCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_counter_idx));

    if (pPtrPed->field_8_char)
    {
        if (pPtrPed->field_8_char->is_player_41B0A0())
        {
            pPtrCounter->field_8_counter = static_cast<u16>(pPtrPed->field_8_char->field_15C_player->GetScore_421980());
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b7d0)
void ScriptThread::SCRCMD_IS_CHAR_IN_GANG_50B7D0()
{
    SCR_IS_CHAR_IN_GANG_ZONE* pCmd = reinterpret_cast<SCR_IS_CHAR_IN_GANG_ZONE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(pCmd->field_A_gang_idx);
    const char_type* gang_name = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_name);
    Ped* pPed = pPointer->field_8_char;
    if (pPed->is_player_41B0A0())
    {
        if (pPed->field_15C_player->get_gang_curr_location_4766D0() == pGang)
        {
            field_8_cond_result = true;
        }
        else
        {
            field_8_cond_result = false;
        }
    }
    else
    {
        gmp_map_zone* pZone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(pPed->get_cam_x().ToInt(), pPed->get_cam_y().ToInt(), Gang_14);
        if (pZone && !_strnicmp(pZone->field_6_name, pGang->field_2_name, pZone->field_5_name_length))
        {
            field_8_cond_result = true;
        }
        else
        {
            field_8_cond_result = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b8b0)
void ScriptThread::SCRCMD_SET_NO_COLLIDE_50B8B0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Car* pCar = pPointer->field_8_car;

    if (pCar)
    {
        if (gBasePtr_6F8070->field_2_type == SCRCMD_SET_NO_COLLIDE)
        {
            pCar->AddFlags_421890(car_flag::NoCollide);
        }
        else if (gBasePtr_6F8070->field_2_type == SCRCMD_CLEAR_NO_COLLIDE)
        {
            pCar->field_78_flags &= static_cast<u16>(~car_flag::NoCollide);
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b910)
void ScriptThread::SCRCMD_IS_CHAR_FIRING_AREA_50B910()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Ped* pPed = pPointer->field_8_char;

    if (pPed->GetAttacking_433CA0() && pPed->IsWithinArea(&pCmd->field_C_rect))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b9c0)
void ScriptThread::SCRCMD_GET_PASSENGER_NUM_50B9C0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Car* pCar = pPointer->field_8_car;

    if (pCar != NULL && pCar->field_4_passengers_list.GetPedsCount_4716B0() >= pCmd->field_A_signed_2)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ba30)
void ScriptThread::SCRCMD_CLEAR_WANTED_LEVEL_50BA30()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    pPointer->field_8_char->ClearWantedPoints_420B80();
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ba70)
void ScriptThread::SCRCMD_ALT_WANTED_LEVEL_50BA70()
{
    SCR_CMD_ALTER_WANTED_LEVEL* pCmd = reinterpret_cast<SCR_CMD_ALTER_WANTED_LEVEL*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (gBasePtr_6F8070->field_2_type == SCRCMD_ALTER_WANTED_LEVEL)
    {
        if (!pPointer->field_8_char->WantedStarCountGreaterThan_46F100(pCmd->field_C_wanted_level))
        {
            pPointer->field_8_char->set_wanted_star_count_46F070(pCmd->field_C_wanted_level);
        }
    }
    else
    {
        pPointer->field_8_char->set_wanted_star_count_46F070(pCmd->field_C_wanted_level);
    }

    Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bad0)
void ScriptThread::SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Car* pCar = pPointer->field_8_car;

    if ((pCar->field_74_damage >= 32000 || pCar->field_88_despawn_status == car_despawn_status::despawning_5) && pCar->IsWithinArea(&pCmd->field_C_rect))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bb80)
void ScriptThread::SCRCMD_SEND_CAR_TO_BLOCK_50BB80()
{
    SCR_CAR_DATA_DEC* pCmd = reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070);
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_car_idx));
    pScrPtr->field_8_car->GotoBlock_441080(pCmd->field_C_pos.x.ToUInt8(),
                                           pCmd->field_C_pos.y.ToUInt8(),
                                           pCmd->field_C_pos.z.ToUInt8(),
                                           1);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bbd0)
void ScriptThread::SCRCMD_THREAT_SEARCH_REACT_ADD_GROUP_50BBD0()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_char_idx));

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_ADD_GROUP:
            pScrPtr->field_8_char->SpawnPedGroupFollowers_46E200(static_cast<u8>(pCmd->field_A_objective));
            break;
        case SCRCMD_SET_THREAT_REACT:
            pScrPtr->field_8_char->field_28C_threat_reaction = pCmd->field_A_objective;
            break;
        case SCRCMD_SET_THREAT_SEARCH:
            pScrPtr->field_8_char->field_288_threat_search = pCmd->field_A_objective;
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bc60)
void ScriptThread::SCRCMD_CHECK_NUM_ALIVE_50BC60()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pScrPtr =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070)->field_8_char_idx));
    PedGroup* pPedGroup = pScrPtr->field_8_char->field_164_ped_group;

    if (pPedGroup != NULL && static_cast<s16>(pPedGroup->field_34_count) >= pCmd->field_A_objective)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bcd0)
void ScriptThread::SCRCMD_SET_MIN_ALIVE_50BCD0()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070)->field_8_char_idx));
    pPointer->field_8_char->field_164_ped_group->field_35_unused = static_cast<char_type>(pCmd->field_A_objective);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

// 9.6f 0x47BAC0
MATCH_FUNC(0x50bd10)
void ScriptThread::SCRCMD_CHAR_GROUP_50BD10()
{
    SCR_TWO_PARAMS* pParams = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);

    SCR_POINTER* pPtr1 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pParams->field_8_unsigned_1));
    SCR_POINTER* pPtr2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    Ped* pChar = pPtr1->field_8_char;
    PedGroup* pGroup = pChar->GetGroup_475AF0();

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_MAKE_LEADER:
            pPtr2->field_8_char->BecomeLeaderOfGroup_4702D0(pChar);
            break;

        case SCRCMD_REMOVE_CHAR:
            pGroup->RemovePed_4C9970(pPtr2->field_8_char);
            break;

        case SCRCMD_ADD_CHAR_TO_GROUP:
            pGroup->add_ped_to_end_of_list_4C8F90(pPtr2->field_8_char);
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bdc0)
void ScriptThread::SCRCMD_SET_COUNTER_50BDC0() // Set counter to a given constant value
{
    SCR_SET_COUNTER* pCmd = reinterpret_cast<SCR_SET_COUNTER*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    pPointer->field_8_counter = pCmd->field_A_value;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50be00)
void ScriptThread::SCRCMD_HAS_CAR_WEAPON_50BE00()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    s16 weapon_idx = pCmd->field_A_signed_2;

    if (gWeapon_8_707018->find_5E3D20(pPointer->field_8_car, weapon_idx))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50be70)
void ScriptThread::SCRCMD_IS_CHAR_HORN_50BE70()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;
    Car* pCar = pPed->field_16C_car;

    if (pCar != NULL && pCar->field_A7_horn > 0 && pCar->field_A7_horn <= 0xF8u)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bed0)
void ScriptThread::SCRCMD_CHECK_MAX_PASS_50BED0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Car* pCar = pPointer->field_8_char->get_car_416B60();

    if (pCar && static_cast<u8>(pCar->GetMaxPassengers_440570()) >= pCmd->field_A_signed_2)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bf40)
void ScriptThread::SCRCMD_IS_CHAR_IN_ZONE_50BF40()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    str_table_entry* pStrEntry = gScriptManager_6F8060->FindStringById_503080(pCmd->field_A_signed_2);
    Player* pPlayer = pPointer->field_8_char->field_15C_player;

    gmp_map_zone* pZone1;
    gmp_map_zone* pZone2;

    if (pPlayer)
    {
        pZone1 = pPlayer->field_38_local_navigation_zone;
        pZone2 = pPlayer->field_3C_navigation_zone;
    }
    else
    {
        pZone1 = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(pPointer->field_8_char->get_cam_x().ToInt(),
                                                              pPointer->field_8_char->get_cam_y().ToInt(),
                                                              Local_Navigation_15);
        pZone2 = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(pPointer->field_8_char->get_cam_x().ToInt(),
                                                              pPointer->field_8_char->get_cam_y().ToInt(),
                                                              Navigation_1);
    }

    if ((pZone1 && !_strnicmp(pZone1->field_6_name, pStrEntry->get_name(), pZone1->field_5_name_length)) ||
        (pZone2 && !_strnicmp(pZone2->field_6_name, pStrEntry->get_name(), pZone2->field_5_name_length)))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c040)
void ScriptThread::SCRCMD_SET_PHONE_DEAD_50C040()
{
    u16* pIndex = &gBasePtr_6F8070->field_8_index;
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(*pIndex));

    pScrPtr->field_8_obj->PoolGiveAndQueueObjectType_5291E0(objects::dead_phone_174);
    gScriptManager_6F8060->RemovePhoneThreadByObjId_512AA0(pScrPtr->field_8_obj->field_14_id);
    u16* pPhoneIds = &gScriptManager_6F8060->field_C1E32_phone_ids[0];

    for (u8 i = 0; i < 31; i++)
    {
        if (*pPhoneIds == *pIndex)
        {
            gGameSave_6F78C8.field_E4_car_and_script_data.field_48_flags |= 1 << i;
            break;
        }

        pPhoneIds++;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c0e0)
void ScriptThread::SCRCMD_IS_TRAILER_ATT_50C0E0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pTrailerPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    Car* pCar;

    if (pTrailerPointer == pCarPointer)
    {
        pCar = pCarPointer->field_8_car;

        if (pCar->IsTrailerCar_421720() || pCarPointer->field_8_car->IsTrailerCab_41E460())
        {
            field_8_cond_result = true;
        }
        else
        {
            field_8_cond_result = false;
        }
    }
    else
    {
        pCar = pTrailerPointer->field_8_car;

        if (pCarPointer->field_8_car->HasAttachedTrailer_475E60(pCar))
        {
            field_8_cond_result = true;
        }
        else
        {
            field_8_cond_result = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c1b0)
void ScriptThread::SCRCMD_IS_CAR_ON_TRAIL_50C1B0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pTrailerPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    Sprite* pSprite = pTrailerPointer->field_8_car->field_0_attachments.FirstSpriteOfType_5A6CA0(sprite_types_enum::car_2);
    Car* pCarOnTrailer;

    if (pSprite != NULL &&
        (pSprite->field_30_sprite_type_enum == sprite_types_enum::car_2 ? (pCarOnTrailer = pSprite->field_8_car_bc_ptr) : (pCarOnTrailer = NULL),
         pCarOnTrailer == pCarPointer->field_8_car))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c230)
void ScriptThread::SCRCMD_ENABLE_DISABLE_CRANE_50C230()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    s16 type = gBasePtr_6F8070->field_2_type;

    if (type == SCRCMD_ENABLE_CRANE)
    {
        pPointer->field_8_crane->field_148_disabled = 0;
    }
    else if (type == SCRCMD_DISABLE_CRANE)
    {
        pPointer->field_8_crane->field_148_disabled = 1;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c2a0)
void ScriptThread::SCRCMD_CAR_GOT_DRIVER_50C2A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pPointer->field_8_car->field_54_driver)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c2f0)
void ScriptThread::SCRCMD_SPOTTED_PLAYER_50C2F0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pPointer->field_8_char->HasSpottedPlayer())
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c350)
void ScriptThread::SCRCMD_GET_LAST_PUNCHED_50C350()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCharTargetPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pCharPunchedPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    pCharPunchedPointer->field_8_char = pCharTargetPointer->field_8_char->get_last_char_punched_475B10();
    pCharTargetPointer->field_8_char->clear_last_char_punched_475B40();

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c3b0)
void ScriptThread::SCRCMD_IS_CHAR_STUNNED_50C3B0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed->field_27C_ped_state_2 == ped_state_2::lying_on_floor_22 || pPed->field_216_health <= 25)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c410)
void ScriptThread::SCRCMD_KILL_ALL_PASSENG_50C410()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    pPointer->field_8_car->field_4_passengers_list.KillAndClearAllPeds_4715A0();

    if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(pPointer->field_8_car))
    {
        gPublicTransport_181C_6FF1D4->KillAllPassengers_579B20();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c470)
void ScriptThread::SCRCMD_IS_GROUP_IN_CAR_50C470()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pGroupLeader = pPointer->field_8_char;
    PedGroup* pGroup = pGroupLeader->field_164_ped_group;

    bool allInCar;
    if (pGroup != NULL)
    {
        allInCar = pGroup->IsAllMembersInSomeCar_4CAA20();
    }
    else
    {
        allInCar = (pGroupLeader->field_16C_car != NULL);
    }
    if (allInCar)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c4e0)
void ScriptThread::SCRCMD_PUNCHED_SOMEONE_50C4E0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pPointer->field_8_char->field_188_last_char_punched != NULL)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c540)
void ScriptThread::SCRCMD_ADD_CHAR_TO_GANG_50C540()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(pCmd->field_A_unsigned_2);

    char_type* gang_name = pGangStr->get_name();

    pPointer->field_8_char->field_17C_pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_name);

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c5a0)
void ScriptThread::SCRCMD_EXPLODE_50C5A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    s32 type;

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_EXPLODE_LARGE1:
            type = explosion_type::large_20;
            break;
        case SCRCMD_EXPLODE_ITEM:
            type = explosion_type::item_19;
            break;
        case SCRCMD_EXPLODE_SMALL1:
            type = explosion_type::small_18;
            break;
        case SCRCMD_EXPLODE_NO_RING1:
            type = explosion_type::no_ring_32;
            break;
    }

    switch (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->field_2_type))
    {
        case script_entity_type::car_2:
            pPointer->field_8_car->HandleCarExplosion_43D840(explosion_type::item_19);
            break;
        case script_entity_type::char_1:
        {
            Ped* pChar = pPointer->field_8_char;

            gObject_5C_6F8F84
                ->CreateExplosion_52A3D0(pChar->get_cam_x(), pChar->get_cam_y(), pChar->get_cam_z(), kAngZero_6F804C, type, 0);

            break;
        }
        case script_entity_type::object_3:
        {
            // 9.6f: Object::GetX_4340D0/GetY_4340E0/GetZ_4340F0 (inlined, using them changes the code)
            Sprite* pSprite = pPointer->field_8_obj->field_4_sprite;

            gObject_5C_6F8F84
                ->CreateExplosion_52A3D0(pSprite->GetXPos(), pSprite->GetYPos(), pSprite->GetZPos(), kAngZero_6F804C, type, 0);

            break;
        }
        case script_entity_type::crane_4:
        {
            SCR_CRANE_TARGET_DEC* pCrane = reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pPointer);

            gObject_5C_6F8F84->CreateExplosion_52A3D0(
                pCrane->field_10_pos.field_0_x,
                pCrane->field_10_pos.field_4_y,
                gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCrane->field_10_pos.field_0_x, pCrane->field_10_pos.field_4_y),
                kAngZero_6F804C,
                type,
                0);

            break;
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c6f0)
void ScriptThread::SCRCMD_PARK_50C6F0() // PARK and PARK_NO_RESPAWN
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pDoorPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    gGarage_48_6FD26C->ParkCarAtDoor_534700(pCarPointer->field_8_car, pDoorPointer->field_8_door);
    if (gBasePtr_6F8070->field_2_type == SCRCMD_PARK_NO_RESPAWN)
    {
        gGarage_48_6FD26C->field_3F_no_respawn = true;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c760)
void ScriptThread::SCRCMD_BEEN_PUNCHED_BY_50C760()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointerVictim = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pPointerAttacker = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    if (pPointerAttacker->field_8_char->field_188_last_char_punched == pPointerVictim->field_8_char)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c7d0)
void ScriptThread::SCRCMD_UPDATE_DOOR_50C7D0()
{
    SCR_UPDATE_DOOR* pCmd = reinterpret_cast<SCR_UPDATE_DOOR*>(gBasePtr_6F8070);
    SCR_POINTER* pDoorPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_door_idx));

    SCR_POINTER* pTarget;

    switch (pDoorPointer->field_8_door->get_open_type_476A90())
    {
        case door_open_type::one_char_on_foot:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_target_idx));

            pDoorPointer->field_8_door->set_open_details_ped(door_open_type::one_char_on_foot, pTarget->field_8_char);
            pDoorPointer->field_8_door->set_target_id(pTarget->field_8_char->field_200_id);
            break;
        case door_open_type::one_car:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_target_idx));

            pDoorPointer->field_8_door->set_open_details_car_bc(door_open_type::one_car, pTarget->field_8_car);
            pDoorPointer->field_8_door->set_target_id(pTarget->field_8_car->field_6C_car_id);
            break;
        case door_open_type::any_player_one_car:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_target_idx));

            pDoorPointer->field_8_door->set_open_details_car_bc(door_open_type::any_player_one_car, pTarget->field_8_car);
            pDoorPointer->field_8_door->set_target_id(pTarget->field_8_car->field_6C_car_id);
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c8a0)
void ScriptThread::SCRCMD_DOOR_50C8A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_OPEN_DOOR:
        {
            Door* pDoor = pPointer->field_8_door;

            if (pDoor->field_0_primary_door_data)
            {
                pDoor->field_0_primary_door_data->Open_49C4E0(0);
            }

            if (pDoor->field_4_secondary_door_data)
            {
                pDoor->field_4_secondary_door_data->Open_49C4E0(pDoor->field_2A_bDoFlip);
            }

            break;
        }
        case SCRCMD_CLOSE_DOOR:
        {
            Door* pDoor = pPointer->field_8_door;

            if (!pDoor->field_28_bClosing)
            {
                break;
            }

            if (pDoor->field_0_primary_door_data)
            {
                pDoor->field_0_primary_door_data->Close_49C590(0);
            }

            if (pDoor->field_4_secondary_door_data)
            {
                pDoor->field_4_secondary_door_data->Close_49C590(pDoor->field_2A_bDoFlip);
            }

            break;
        }
        case SCRCMD_SET_DOOR_AUTO:
            pPointer->field_8_door->field_29_bAuto = true;
            break;
        case SCRCMD_SET_DOOR_MANUAL:
            pPointer->field_8_door->field_29_bAuto = false;
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c990)
void ScriptThread::SCRCMD_REMOVE_WEAPON_50C990()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Ped* pPed = pPointer->field_8_char;
    Player* pPlayer = pPed->field_15C_player;

    if (pPlayer != NULL)
    {
        pPlayer->RemovePlayerWeapons_564C50();
    }
    else
    {
        pPed->RemovePedWeapons_462510();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c9f0)
void ScriptThread::SCRCMD_REMOVE_BLOCK_50C9F0()
{
    SCR_REMOVE_BLOCK* pCmd = reinterpret_cast<SCR_REMOVE_BLOCK*>(gBasePtr_6F8070);

    gMap_0x370_6F6268->RemoveBlock_4E8940(pCmd->field_8_pos.field_0_x,
                                          pCmd->field_8_pos.field_1_y,
                                          pCmd->field_8_pos.field_2_z,
                                          pCmd->field_B_do_drop);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ca30)
void ScriptThread::SCRCMD_LOWER_LEVEL_50CA30()
{
    SCR_LOWER_LEVEL* pCmd = reinterpret_cast<SCR_LOWER_LEVEL*>(gBasePtr_6F8070);

    gMap_0x370_6F6268->LowerLevel_4E8B70(pCmd->field_8_min_pos.field_0_x,
                                         pCmd->field_A_max_pos.field_0_x,
                                         pCmd->field_8_min_pos.field_1_y,
                                         pCmd->field_A_max_pos.field_1_y);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ca70)
void ScriptThread::SCRCMD_CHANGE_BLOCK_50CA70()
{
    SCR_CHANGE_BLOCK* pCmd = reinterpret_cast<SCR_CHANGE_BLOCK*>(gBasePtr_6F8070);
    gMap_0x370_6F6268->ChangeBlock_4E8620(pCmd->field_8_x,
                                          pCmd->field_9_y,
                                          pCmd->field_A_z,
                                          pCmd->field_B_change_type,
                                          pCmd->field_C_info_word);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cab0)
void ScriptThread::SCRCMD_ADD_NEW_BLOCK_50CAB0()
{
    gmp_block_info block;

    block.init_44C840();

    gMap_0x370_6F6268->AddNewBlock_4E87C0(reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr_6F8070)->field_8_xpos,
                                          reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr_6F8070)->field_9_ypos,
                                          reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr_6F8070)->field_A_zpos,
                                          &block);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cb20)
void ScriptThread::SCRCMD_ROAD_ON_OFF_50CB20()
{
    SCR_SWITCH_ROAD_ON_OFF* pCmd = reinterpret_cast<SCR_SWITCH_ROAD_ON_OFF*>(gBasePtr_6F8070);

    if (pCmd->field_B_status != 0)
    {
        gRouteFinder_6FFDC8->RoadOn_588950(pCmd->field_8_xpos, pCmd->field_9_ypos, pCmd->field_A_zpos);
    }
    else
    {
        gRouteFinder_6FFDC8->RoadOff_588810(pCmd->field_8_xpos, pCmd->field_9_ypos, pCmd->field_A_zpos);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cb70)
void ScriptThread::SCRCMD_CHECK_CAR_DRIVER_50CB70()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    field_8_cond_result = false;

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_CHECK_CAR_DRIVER:
            if (pPointer->field_8_car->field_54_driver != NULL)
            {
                field_8_cond_result = true;
            }

            break;
        case SCRCMD_IS_CAR_WRECKED:
        {
            Car* pCar = pPointer->field_8_car;

            if (pCar->field_88_despawn_status == car_despawn_status::despawned_6 || pCar->field_88_despawn_status == car_despawn_status::despawning_5 || pCar->field_74_damage >= 32000 || pCar->HasSpriteZoom_43A230())
            {
                field_8_cond_result = true;
            }

            break;
        }
        case SCRCMD_CAR_SUNK:
            if (pPointer->field_8_car->HasSpriteZoom_43A230())
            {
                field_8_cond_result = true;
            }

            break;
        case SCRCMD_CAR_IN_AIR:
            if (pPointer->field_8_car->IsCarInAir_43A3C0())
            {
                field_8_cond_result = true;
            }

            break;
        case SCRCMD_IS_CAR_CRUSHED:
            if (pPointer->field_8_car->IsBeingCrushed_43DD50())
            {
                field_8_cond_result = true;
            }

            break;
        case SCRCMD_CARBOMB_ACTIVE:
        {
            Car* pCar = pPointer->field_8_car;
            if (pCar->field_0_attachments.GetSpriteForModel_5A6A50(objects::car_bomb_132) != NULL)
            {
                field_8_cond_result = true;
            }

            break;
        }
        case SCRCMD_EMERG_LIGHTS_ON:
        {
            Car* pCar = pPointer->field_8_car;
            car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(pCar->field_84_car_info_idx);

            if ((pInfo->info_flags & car_info_flag::emergency) == car_info_flag::emergency || pCar->field_84_car_info_idx == car_model_enum::EDSELFBI)
            {
                if (pCar->field_A4_bf.bEmergencyLights)
                {
                    field_8_cond_result = true;
                }
            }

            break;
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ccb0)
void ScriptThread::SCRCMD_SWITCH_GENERATOR1_50CCB0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    u16 spawn_count = pCmd->field_A_unsigned_2;
    Generator* pGen = pPointer->field_8_generator;
    pGen->Activate_4C1A70();
    pGen->field_1E_spawns_left = spawn_count;

    s32 type = pPointer->field_8_generator->field_0_gen_type;

    if ((type >= 200 && type <= 244) || (type >= 64 && type <= 108))
    {
        pPointer->field_8_generator->field_21_spawn_mode = generator_spawn_mode::one_at_a_time_2;
    }
    else
    {
        pPointer->field_8_generator->field_21_spawn_mode = generator_spawn_mode::every_cycle_1;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cd30)
void ScriptThread::SCRCMD_SWITCH_GENERATOR3_50CD30()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    pPointer->field_8_generator->field_0_gen_type = pCmd->field_C_signed_3;

    Generator* pGen = pPointer->field_8_generator;
    u16 spawn_count = pCmd->field_A_unsigned_2;
    pGen->Activate_4C1A70();
    pGen->field_1E_spawns_left = spawn_count;

    s16 type = pCmd->field_C_signed_3;

    if ((type >= 200 && type <= 244) || (type >= 64 && type <= 108))
    {
        pPointer->field_8_generator->field_21_spawn_mode = generator_spawn_mode::one_at_a_time_2;
    }
    else
    {
        pPointer->field_8_generator->field_21_spawn_mode = generator_spawn_mode::every_cycle_1;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cdb0)
void ScriptThread::SCRCMD_CAR_DAMAGE_POS_50CDB0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if ((pPointer->field_8_car)->IsAreaDamaged_43D1C0(pCmd->field_A_signed_2)) // field_A_signed_2 = damage pos
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ce10)
void ScriptThread::SCRCMD_PARK_FINISHED_50CE10()
{
    if (gGarage_48_6FD26C->field_C_state == garage_state::park_finished_3)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ce50)
void ScriptThread::Gosub_50CE50(SCR_CMD_HEADER* pCmd, u16 cmd_idx)
{
    ScriptStackFrame* pFrame = field_114_gosub_stack->AllocFrame_5031A0();
    pFrame->field_0_cond_result = field_8_cond_result;
    pFrame->field_4_return_cmd = pCmd->field_4_cmd_next;
    field_114_gosub_stack->add_503160(pFrame);
    ScriptThread::JumpToCmd_503650(cmd_idx);
}

MATCH_FUNC(0x50ce90)
void ScriptThread::SCRCMD_PHONE_TEMPLATE_50CE90()
{
    SCR_PHONE_TEMPLATE* pCmd = reinterpret_cast<SCR_PHONE_TEMPLATE*>(gBasePtr_6F8070);

    SCR_POINTER* pPassedFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_counter1));
    SCR_POINTER* pFailedFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_10_counter2));
    SCR_POINTER* pOnMission0 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_counter3b));
    SCR_POINTER* pOnMission1 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_12_counter3));
    SCR_POINTER* pOnMission2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_14_counter4));
    SCR_POINTER* pOnMission3 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_16_counter5));

    str_table_entry* pGangNameEntry = gScriptManager_6F8060->FindStringById_503080(pCmd->field_18_gangname);
    char_type* gang_name = pGangNameEntry->get_name();
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_name);

    s8 player_idx1;

    // If passed previous mission.
    if (pPassedFlag->field_8_counter >= 1 && pOnMission0->field_8_counter == 0 && pOnMission1->field_8_counter == 0 &&
        pOnMission2->field_8_counter == 0 && pOnMission3->field_8_counter == 0)
    {
        // If enough respect.
        if ((player_idx1 = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0(),
             pGang->GetRespectForPlayer_4BEEF0(player_idx1) / 20 >= pCmd->field_1A_respect))
        {
            gScriptManager_6F8060->field_C1E2E_death_arrest_flag = false;
            str_table_entry* mission2_str_table = gScriptManager_6F8060->FindStringById_503080(pCmd->field_E_mission2);
            char_type* mission2_name = mission2_str_table->get_name();
            ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, mission2_name);
            return;
        }

        // Not enough respect.
        s32 brief_id = pCmd->field_8_brief_id - 7;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    s8 player_idx2;

    // If failed previous mission.
    if (pFailedFlag->field_8_counter >= 1 && pOnMission0->field_8_counter == 0 && pOnMission1->field_8_counter == 0 &&
        pOnMission2->field_8_counter == 0 && pOnMission3->field_8_counter == 0)
    {
        // If enough respect.
        if ((player_idx2 = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0(),
             pGang->GetRespectForPlayer_4BEEF0(player_idx2) / 20 >= pCmd->field_1A_respect))
        {
            gScriptManager_6F8060->field_C1E2E_death_arrest_flag = false;
            str_table_entry* mission2_str_table = gScriptManager_6F8060->FindStringById_503080(pCmd->field_E_mission2);
            char_type* mission2_name = mission2_str_table->get_name();
            ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, mission2_name);
            return;
        }

        // Not enough respect.
        s32 brief_id = pCmd->field_8_brief_id - 7;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If on mission gang 1.
    if (pOnMission1->field_8_counter >= 1)
    {
        s32 brief_id = pCmd->field_8_brief_id - 5;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If on mission gang 2.
    if (pOnMission2->field_8_counter == 1)
    {
        s32 brief_id = pCmd->field_8_brief_id - 2;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If on mission gang 3.
    if (pOnMission3->field_8_counter == 1)
    {
        s32 brief_id = pCmd->field_8_brief_id - 1;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If already passed.
    if (pPassedFlag->field_8_counter == 1)
    {
        s32 brief_id = pCmd->field_8_brief_id;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If already failed.
    if (pFailedFlag->field_8_counter == 1)
    {
        s32 brief_id = pCmd->field_8_brief_id + 1;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    s8 player_idx3;

    // Start mission 1 if enough respect.
    if ((player_idx3 = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0(),
         pGang->GetRespectForPlayer_4BEEF0(player_idx3) / 20 >= pCmd->field_1A_respect))
    {
        gScriptManager_6F8060->field_C1E2E_death_arrest_flag = false;
        str_table_entry* mission1_str_table = gScriptManager_6F8060->FindStringById_503080(pCmd->field_1C_mission1);
        char_type* mission1_name = mission1_str_table->get_name();
        ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, mission1_name);
        return;
    }

    s8 player_idx4;

    // If zero respect.
    if ((player_idx4 = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0(), (pGang->GetRespectForPlayer_4BEEF0(player_idx4) / 20) == 0))
    {
        s32 brief_id = pCmd->field_8_brief_id - 6;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    s8 player_idx5;

    // If negative respect.
    if ((player_idx5 = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0(), pGang->GetRespectForPlayer_4BEEF0(player_idx5) / 20 < 0))
    {
        s32 brief_id = pCmd->field_8_brief_id - 4;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
    else // Positive respect, but not enough.
    {
        s32 brief_id = pCmd->field_8_brief_id - 7;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
}

MATCH_FUNC(0x50d200)
void ScriptThread::SCRCMD_REMOTE_CONTROL_50D200()
{
    SCR_REMOTE_CONTROL* pCmd = reinterpret_cast<SCR_REMOTE_CONTROL*>(gBasePtr_6F8070);
    SCR_POINTER* pPlayerPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pCarPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_car_idx));
    pPlayerPtr->field_8_char->field_15C_player->DisableInputs_569F40();
    pPlayerPtr->field_8_char->field_15C_player->ResetAuxCamera_5695A0();
    pPlayerPtr->field_8_char->field_15C_player->StartRemoteControl_569600(pCarPtr->field_8_car);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50d2e0)
void ScriptThread::SCRCMD_LAUNCH_MISSION_50D2E0()
{
    str_table_entry* pMissionStr = gScriptManager_6F8060->FindStringById_503080(gBasePtr_6F8070->field_8_index);
    ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, pMissionStr->get_name());
}

MATCH_FUNC(0x50d340)
void ScriptThread::SCRCMD_SAVE_GAME_50D340()
{
    char_type* pSaveFileName = gGameSession_67E8E0.GetSaveFileName_4C5970();
    gScriptManager_6F8060->SaveGame_511E10(pSaveFileName);
    Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50d3c0)
void ScriptThread::SCRCMD_DO_SAVE_GAME_50D3C0()
{
    if (!field_C_cmd_initialised)
    {
        SCR_DO_SAVE_GAME* pCmd = reinterpret_cast<SCR_DO_SAVE_GAME*>(gBasePtr_6F8070);
        ScriptThread::DisableThread_505790(pCmd->field_8_triggername);

        if (static_cast<u32>(gpRng_67AB34->get_cur_rng_41CFE0()) > 0)
        {
            u32* mission_flag = reinterpret_cast<u32*>(gScriptManager_6F8060->field_344_mission_flag);
            if (!mission_flag || *mission_flag)
            {
                // It's in mission, so the player cannot save the game
                sprintf(gTmpBuffer_67C598, "svmiss");
            }
            else
            {
                Player* pPlayer = gGame_0x40_67E008->field_38_local_player;

                if (pPlayer->GetScore_421980() >= 50000)
                {
                    // Hallelluya! Another soul saved!
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::hallelujah_61);
                    gGame_0x40_67E008->field_38_local_player->AddCash_421990(-50000);
                    gScriptManager_6F8060->SaveGame_511E10(gGameSession_67E8E0.GetSaveFileName_4C5970());
                    sprintf(gTmpBuffer_67C598, "svdone");
                }
                else
                {
                    // No donation, no salvation!
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::damnation_62);
                    sprintf(gTmpBuffer_67C598, "svscore");
                }
            }

            gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        }

        field_C_cmd_initialised = true;
    }
    else
    {
        Ped* pPlayerPed = gGame_0x40_67E008->field_38_local_player->field_2C4_player_ped;
        SCR_DO_SAVE_GAME* pCmd = reinterpret_cast<SCR_DO_SAVE_GAME*>(gBasePtr_6F8070);

        // This doesn't look right, but it matches. Might be a missing parens bug in the OG game?
        if (pPlayerPed->get_cam_x() < pCmd->field_C_rect.field_0_pos.x - pCmd->field_C_rect.field_C_size.field_0_x ||
            pPlayerPed->get_cam_x() > pCmd->field_C_rect.field_0_pos.x + pCmd->field_C_rect.field_C_size.field_0_x &&
                pPlayerPed->get_cam_y() < pCmd->field_C_rect.field_0_pos.y - pCmd->field_C_rect.field_C_size.field_4_y ||
            pPlayerPed->get_cam_y() > pCmd->field_C_rect.field_0_pos.y + pCmd->field_C_rect.field_C_size.field_4_y)
        {
            ScriptThread::EnableThread_50A9E0(pCmd->field_8_triggername);
            ScriptThread::Next_503620(gBasePtr_6F8070);
        }
    }
}

MATCH_FUNC(0x50d680)
void ScriptThread::SCRCMD_CHANGE_CAR_LOCK_50D680()
{
    SCR_CHANGE_CAR_LOCK* pCmd = reinterpret_cast<SCR_CHANGE_CAR_LOCK*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Car* pCar;

    switch (pCmd->field_A_lock_type)
    {
        case Car_Door_Lock::locked_1:
            pCar = pPointer->field_8_car;
            if (pCar->field_98_door_lock != Car_Door_Lock::locked_permanently_4)
            {
                pCar->field_98_door_lock = Car_Door_Lock::locked_1;
            }

            break;
        case Car_Door_Lock::lockout_thief_2:
            pCar = pPointer->field_8_car;
            if (pCar->field_98_door_lock != Car_Door_Lock::locked_permanently_4)
            {
                pCar->field_98_door_lock = Car_Door_Lock::lockout_thief_2;
            }

            break;
        case Car_Door_Lock::unlocked_3:
            pCar = pPointer->field_8_car;
            if (pCar->field_98_door_lock != Car_Door_Lock::locked_permanently_4)
            {
                pCar->field_98_door_lock = Car_Door_Lock::unlocked_3;
            }
            break;
        case Car_Door_Lock::locked_permanently_4:
            pPointer->field_8_car->field_98_door_lock = Car_Door_Lock::locked_permanently_4;
            break;
        case Car_Door_Lock::lockout_player_5:
            pCar = pPointer->field_8_car;
            if (pCar->field_98_door_lock != Car_Door_Lock::locked_permanently_4)
            {
                pCar->field_98_door_lock = Car_Door_Lock::lockout_player_5;
            }
            break;
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50d870)
void ScriptThread::SCRCMD_CHANGE_INTENSITY_50D870()
{
    SCR_CHANGE_LIGHT_INTENSITY* pCmd = reinterpret_cast<SCR_CHANGE_LIGHT_INTENSITY*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    pPointer->field_8_light->SetIntensity_476AE0(static_cast<u8>(pCmd->field_A_intensity));

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50d900)
void ScriptThread::SCRCMD_CHANGE_COLOUR_50D900()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    u32 rgb = pCmd->field_C_u32;
    pPointer->field_8_light->field_10_argb = rgb;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50d9a0)
void ScriptThread::SCRCMD_CHANGE_RADIUS_50D9A0()
{
    SCR_CHANGE_LIGHT_RADIUS* pCmd = reinterpret_cast<SCR_CHANGE_LIGHT_RADIUS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    pPointer->field_8_light->field_0_intensity_radius.SetRadius_463F10(pCmd->field_A_radius);

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50da50)
void ScriptThread::SCRCMD_CREATE_LIGHT_50DA50() // CREATE_LIGHT1 or LIGHT_DECSET2
{
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index)); // SCR_CREATE_LIGHT::field_8_light_idx
    ScriptThread::CreateLight_504EE0(reinterpret_cast<SCR_CREATE_LIGHT*>(gBasePtr_6F8070), pPointer);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50db70)
void ScriptThread::GetSetLivesAndMultiplier_50DB70()
{
    SCR_GET_NUM_LIVES_MULTIPLIERS* pCmd = reinterpret_cast<SCR_GET_NUM_LIVES_MULTIPLIERS*>(gBasePtr_6F8070);
    SCR_POINTER* pPlayerPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_player_ped_idx));

    switch (pCmd->field_2_type)
    {
        case SCRCMD_ADD_LIVES:
            if (pPlayerPtr->field_8_char->field_15C_player)
            {
                pPlayerPtr->field_8_char->field_15C_player->ChangeLifeCountByAmount_5699F0(pCmd->field_A_counter_idx);
            }

            break;
        case SCRCMD_CHECK_NUM_LIVES:
            if (pPlayerPtr->field_8_char->field_15C_player)
            {
                if (pPlayerPtr->field_8_char->field_15C_player->field_684_lives.field_0_value > pCmd->field_A_counter_idx)
                {
                    field_8_cond_result = true;
                }
                else
                {
                    field_8_cond_result = false;
                }
            }

            break;
        case SCRCMD_ADD_MULTIPLIER:
            if (pPlayerPtr->field_8_char->field_15C_player)
            {
                pPlayerPtr->field_8_char->field_15C_player->field_6BC_multipliers.ChangeStatByAmount_4921B0(pCmd->field_A_counter_idx);
            }

            break;
        case SCRCMD_CHECK_MULT:
            if (pPlayerPtr->field_8_char->field_15C_player)
            {
                if (pPlayerPtr->field_8_char->field_15C_player->field_6BC_multipliers.field_0_value > pCmd->field_A_counter_idx)
                {
                    field_8_cond_result = true;
                }
                else
                {
                    field_8_cond_result = false;
                }
            }

            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50dd00)
void ScriptThread::SCRCMD_GET_LIVES_MULT_50DD00() // GET_NUM_LIVES or GET_NUM_MULT
{
    SCR_GET_NUM_LIVES_MULTIPLIERS* pCmd = reinterpret_cast<SCR_GET_NUM_LIVES_MULTIPLIERS*>(gBasePtr_6F8070);
    SCR_POINTER* pPlayerPtr =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index)); // pCmd->field_8_player_ped_idx
    SCR_POINTER* pCounterPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_counter_idx));
    if (gBasePtr_6F8070->field_2_type == SCRCMD_GET_NUM_LIVES)
    {
        pCounterPtr->field_8_counter = pPlayerPtr->field_8_char->field_15C_player->get_lives_4766C0();
    }
    else
    {
        pCounterPtr->field_8_counter = pPlayerPtr->field_8_char->field_15C_player->get_multiplier_4766A0();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50dd90)
void ScriptThread::SCRCMD_SET_DIR_OF_TVVAN_50DD90()
{
    SCR_SET_DIR_OF_TVVAN* pCmd = reinterpret_cast<SCR_SET_DIR_OF_TVVAN*>(gBasePtr_6F8070);

    Fix16_Point point(pCmd->field_8_pos.field_0_x, pCmd->field_8_pos.field_4_y);
    gCarManager_6C_677930->field_4C_tv_van_dir = point;

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50de00)
void ScriptThread::SCRCMD_POINT_ONSCREEN_50DE00()
{
    SCR_IS_POINT_ON_SCREEN* pCmd = reinterpret_cast<SCR_IS_POINT_ON_SCREEN*>(gBasePtr_6F8070);
    if (gGame_0x40_67E008->IsPointOnScreen_4B9A80(pCmd->field_8_xpos, pCmd->field_C_ypos))
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50de50)
void ScriptThread::SCRCMD_CHAR_IN_AIR_50DE50()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed->field_27C_ped_state_2 == ped_state_2::falling_19 && pPed->field_278_ped_state_1 == ped_state_1::immobilized_8)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50deb0)
void ScriptThread::SCRCMD_CHAR_SUNK_50DEB0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed->field_27C_ped_state_2 == ped_state_2::sinking_20 && pPed->field_278_ped_state_1 == ped_state_1::immobilized_8)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50df10)
void ScriptThread::StartBonus_50DF10()
{
    SCR_START_BONUS* pCmd = reinterpret_cast<SCR_START_BONUS*>(gBasePtr_6F8070);

    s32 car_model = car_model_enum::none;
    s16 gang_idx = -1;
    s32 alt_car_model = car_model;
    s32 occupation = ped_ocupation_enum::no_occupation;

    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_18_ptr));

    s32 event_type;

    if (pCmd->field_A_subtype == 0 || pCmd->field_A_subtype == 2)
    {
        event_type = bonus_event_type::car_destroyed_1;
    }
    else
    {
        event_type = bonus_event_type::ped_killed_0;
    }

    switch (pCmd->field_A_subtype)
    {
        case 2:
        case 3:
            gang_idx = gGangPool_CA8_67E274->GetGangIdxByName_4BF210(
                gScriptManager_6F8060->FindStringById_503080(pCmd->field_E_id)->get_name());
            break;
        case 0:
            if (pCmd->field_E_id != 0xFFFF)
            {
                car_model = pCmd->field_E_id;

                // French version neutralizes bonuses against law enforcement.
                if (bIsFrench_67D53C &&
                    (car_model == car_model_enum::COPCAR || car_model == car_model_enum::apc || car_model == car_model_enum::GUNJEEP ||
                     car_model == car_model_enum::JEEP || car_model == car_model_enum::SWATVAN || car_model == car_model_enum::EDSELFBI))
                {
                    car_model = car_model_enum::none;
                }
            }

            break;
        case 1:
            occupation = pCmd->field_E_id;

            // French version neutralizes bonuses against law enforcement.
            if (bIsFrench_67D53C && (occupation == ped_ocupation_enum::any_law_enforcement || occupation == ped_ocupation_enum::police))
            {
                occupation = ped_ocupation_enum::no_occupation;
            }

            break;
    }

    s8 check_mode = pCmd->field_D_bExclusive != 1 ? 2 : 1;

    if (pCmd->field_12_alt_car_model >= 0)
    {
        alt_car_model = pCmd->field_12_alt_car_model;
    }

    // u16 > 0xFFFF is never true, left over dead code?
    if (pCmd->field_8_zone_str_id > 0xFFFF)
    {
        gMap_0x370_6F6268->FindZoneByName_4DEFD0(gScriptManager_6F8060->FindStringById_503080(pCmd->field_8_zone_str_id)->get_name());
    }

    pPointer->field_8_counter = gGame_0x40_67E008->field_38_local_player->field_2D4_scores.field_1A8_bonuses.AllocRule_431FE0(event_type,
                                                                                                                   car_model,
                                                                                                                   occupation,
                                                                                                                   gang_idx,
                                                                                                                   pCmd->field_B_remap,
                                                                                                                   pCmd->field_1A_death_cause,
                                                                                                                   alt_car_model,
                                                                                                                   pCmd->field_10_time_limit,
                                                                                                                   check_mode,
                                                                                                                   pCmd->field_C_target_count,
                                                                                                                   pCmd->field_14_reward,
                                                                                                                   NULL);

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e0b0)
void ScriptThread::SCRCMD_CHECK_BONUS_50E0B0()
{
    s16 result = ScriptThread::GetBonusResult_505EA0(gBasePtr_6F8070->field_8_index);
    field_8_cond_result = false;

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_CHECK_BONUS1:
            if (result == bonus_result::passed)
            {
                field_8_cond_result = true;
            }
            break;
        case SCRCMD_CHECK_BONUS2:
            if (result == bonus_result::failed)
            {
                field_8_cond_result = true;
            }
            break;
        case SCRCMD_CHECK_BONUS3:
            if (result != bonus_result::in_progress)
            {
                field_8_cond_result = true;
            }
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e120)
void ScriptThread::SCRCMD_SETUP_MODEL_CHECK_50E120()
{
    gCarManager_6C_677930->field_58_model_to_check_destroy = gBasePtr_6F8070->field_8_index;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e150)
void ScriptThread::SCRCMD_MODEL_CHECK_50E150()
{
    if (gCarManager_6C_677930->field_5C_model_check_destroyed)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e190)
void ScriptThread::GetSpeed_50E190()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pCounterPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    // 9.6f 0x47D070. The cases are in ascending order (the layout doesn't follow it): that order gives
    // the original's inline cut-offs, GetLength_41E260 inlined (with one Abs) for the char's car and
    // called out of line (0x453590) for the car, behind the get_car_velocity_4211C0 wrappers.
    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_GET_CAR_SPEED:
            if (pPointer->field_8_car->field_58_physics)
            {
                pCounterPointer->field_8_counter = pPointer->field_8_car->field_58_physics->get_car_velocity_4211C0().get_value_4754D0();
            }
            else
            {
                pCounterPointer->field_8_counter = kFpZero_6F77C0.ToInt();
            }
            break;

        case SCRCMD_GET_CHAR_CAR_SPEED:
            if (pPointer->field_8_char->has_car_403B80())
            {
                if (pPointer->field_8_char->get_car_416B60()->field_58_physics)
                {
                    pCounterPointer->field_8_counter =
                        pPointer->field_8_char->get_car_416B60()->field_58_physics->get_car_velocity_4211C0().get_value_4754D0();
                }
                else
                {
                    pCounterPointer->field_8_counter = kFpZero_6F77C0.ToInt();
                }
            }
            break;

        case SCRCMD_GET_MAX_SPEED:
            pCounterPointer->field_8_counter = pPointer->field_8_car->GetMaxSpeed_439F30().get_value_4754D0();
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e360)
void ScriptThread::SCRCMD_CHECK_CAR_SPEED_50E360()
{
    SCR_CHECK_CAR_SPEED* pCmd = reinterpret_cast<SCR_CHECK_CAR_SPEED*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (pPointer->field_8_car->field_58_physics &&
        pPointer->field_8_car->field_58_physics->get_car_velocity_4211C0().get_value_4754D0() >
            pCmd->field_A_value)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e460)
void ScriptThread::SCRCMD_SET_CAR_GRAPHIC_50E460()
{
    SCR_SET_CAR_GRAPHIC* pCmd = reinterpret_cast<SCR_SET_CAR_GRAPHIC*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    pPointer->field_8_car->SetCarGraphic_43CDF0(static_cast<char_type>(pCmd->field_C_number)); // set the number on the top of the car
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e4a0)
void ScriptThread::SCRCMD_CHAR_ARRESTED_50E4A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pPointer->field_8_char->field_21C_bf.bBusted)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e4f0)
void ScriptThread::SCRCMD_S_IS_S_OP_S_50E4F0()
{
    SCR_OPERATE_COUNTER_AND_COUNTER* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_COUNTER*>(gBasePtr_6F8070);
    u8 type = ScriptThread::GetOperationType_506BC0(gBasePtr_6F8070->field_2_type);

    SCR_POINTER* pDestCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_destination_counter_idx));
    SCR_POINTER* pFirstOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_first_operand_counter_idx));
    SCR_POINTER* pSecondOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_second_operand_counter_idx));
    switch (type)
    {
        case script_operation::add_0:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter + pSecondOperand->field_8_counter;
            break;
        case script_operation::subtract_1:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter - pSecondOperand->field_8_counter;
            break;
        case script_operation::multiply_8:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter * pSecondOperand->field_8_counter;
            break;
        case script_operation::divide_7:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter / pSecondOperand->field_8_counter;
            break;
        case script_operation::modulo_9:
            pDestCounter->field_8_counter = pFirstOperand->field_8_counter % pSecondOperand->field_8_counter;
            break;
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e610)
void ScriptThread::SCRCMD_S_IS_S_OP_I_50E610()
{
    SCR_OPERATE_COUNTER_AND_OPERAND_INT* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_OPERAND_INT*>(gBasePtr_6F8070);
    SCR_POINTER* pDestCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_destination_counter_idx));
    SCR_POINTER* pOperandCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_operand_counter_idx));

    switch (ScriptThread::GetOperationType_506BC0(gBasePtr_6F8070->field_2_type))
    {
        case script_operation::add_0:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter + pCmd->field_C_value;
            break;
        case script_operation::subtract_1:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter - pCmd->field_C_value;
            break;
        case script_operation::multiply_8:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter * pCmd->field_C_value;
            break;
        case script_operation::divide_7:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter / pCmd->field_C_value;
            break;
        case script_operation::modulo_9:
            pDestCounter->field_8_counter = pOperandCounter->field_8_counter % pCmd->field_C_value;
            break;
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e730)
void ScriptThread::SCRCMD_CHAR_DRIVE_AGGR_50E730()
{
    SCR_SET_CHAR_DRIVE_AGGR* pCmd = reinterpret_cast<SCR_SET_CHAR_DRIVE_AGGR*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    pPointer->field_8_char->field_21C_bf.bDriveAggressively = pCmd->field_A_status;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e780)
void ScriptThread::SCRCMD_SET_SPEED_50E780()
{
    SCR_SET_RUN_SPEED* pCmd = reinterpret_cast<SCR_SET_RUN_SPEED*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (gBasePtr_6F8070->field_2_type == SCRCMD_SET_RUN_SPEED)
    {
        pPointer->field_8_char->field_1F0_max_speed = pCmd->field_A_value;
    }
    else
    {
        pPointer->field_8_char->field_1F8_run_speed = pCmd->field_A_value;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e7f0)
void ScriptThread::SCRCMD_GIVE_CAR_ALARM_50E7F0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    pPointer->field_8_car->field_78_flags |= car_flag::CarAlarm;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e820)
void ScriptThread::SCRCMD_CAR_WEAPON_KIND_PROOF_50E820()
{
    SCR_SET_CAR_DAMAGE_PROOF* pCmd = reinterpret_cast<SCR_SET_CAR_DAMAGE_PROOF*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Car* pCar;

    switch (pCmd->field_2_type)
    {
        case SCRCMD_CAR_BULLETPROOF:
            pCar = pPointer->field_8_car;
            if (pCmd->field_A_status == 1)
            {
                pCar->field_78_flags |= car_flag::Bulletproof;
            }
            else
            {
                pCar->field_78_flags &= static_cast<u16>(~car_flag::Bulletproof);
            }
            break;
        case SCRCMD_CAR_ROCKETPROOF:
            pCar = pPointer->field_8_car;
            if (pCmd->field_A_status == 1)
            {
                pCar->field_78_flags |= car_flag::Rocketproof;
            }
            else
            {
                pCar->field_78_flags &= static_cast<u16>(~car_flag::Rocketproof);
            }
            break;
        case SCRCMD_CAR_FLAMEPROOF:
            pCar = pPointer->field_8_car;
            if (pCmd->field_A_status == 1)
            {
                pCar->field_78_flags |= car_flag::Flameproof;
            }
            else
            {
                pCar->field_78_flags &= static_cast<u16>(~car_flag::Flameproof);
            }
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e900)
void ScriptThread::SCRCMD_PUT_CAR_ON_TRAILER_50E900()
{
    SCR_PUT_CAR_ON_TRAILER* pCmd = reinterpret_cast<SCR_PUT_CAR_ON_TRAILER*>(gBasePtr_6F8070);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pDstCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_trailer_car_idx));

    Car* pDstCar = pDstCarPointer->field_8_car;
    if (pDstCar->field_88_despawn_status != car_despawn_status::despawned_6)
    {
        if (pDstCar->IsTrailerCab_41E460())
        {
            // put car on the trailer attached to the truck cab
            pDstCar->GetSprite_440840()->AttachSprite_5A3100(pCarPointer->field_8_car->field_50_car_sprite,
                                                                       kFpZero_6F77C0,
                                                                       kFpZero_6F77C0,
                                                                       kAng180_6F771E);
        }
        else
        {
            // put car directly on the trailer
            pDstCar->field_50_car_sprite->AttachSprite_5A3100(pCarPointer->field_8_car->field_50_car_sprite,
                                                                        kFpZero_6F77C0,
                                                                        kFpZero_6F77C0,
                                                                        kAng180_6F771E);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e9a0)
void ScriptThread::SCRCMD_CLEAR_BRIEFS_50E9A0()
{
    gHud_2B00_706620->field_DC_brief.ClearAllBriefsWithPriority_5D4890(hud_brief_priority::normal_1);
    gHud_2B00_706620->field_DC_brief.ClearAllBriefsWithPriority_5D4890(hud_brief_priority::now_3);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e9e0)
void ScriptThread::SCRCMD_CHECK_HEADS_50E9E0()
{
    SCR_CHECK_HEADS_GREATER* pCmd = reinterpret_cast<SCR_CHECK_HEADS_GREATER*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (pPointer->field_8_char->get_wanted_star_count_46EF00() > pCmd->field_A_wanted_level)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ea40)
void ScriptThread::SCRCMD_FINISH_LEVEL_50EA40()
{
    SCR_FINISH_LEVEL* pCmd = reinterpret_cast<SCR_FINISH_LEVEL*>(gBasePtr_6F8070);
    char_type bonus_type;
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

    s32* num_passed_flag = gScriptManager_6F8060->field_328_passed_flag;
    if (num_passed_flag != NULL && gScriptManager_6F8060->field_314_total_missions == *num_passed_flag)
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

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50eb00)
void ScriptThread::SCRCMD_CHECK_WEAPONHIT_50EB00()
{
    SCR_CHECK_WEAPONHIT* pCmd = reinterpret_cast<SCR_CHECK_WEAPONHIT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (!pCmd->field_A_status)
    {
        gScriptManager_6F8060->TestAndRecordWeaponHit_512AF0(pPointer->field_8_char->get_id(), static_cast<char_type>(pCmd->field_C_weapon), true);
        field_8_cond_result = false;
        pCmd->field_A_status = 1;
    }
    else
    {
        if (gScriptManager_6F8060->IsWeaponHitRecorded_512C70(pPointer->field_8_char->get_id(), static_cast<char_type>(pCmd->field_C_weapon), true))
        {
            field_8_cond_result = true;
            gScriptManager_6F8060->ClearWeaponHitFlag_512BA0(pPointer->field_8_char->get_id(), true);
            pCmd->field_A_status = 0;
        }
        else
        {
            field_8_cond_result = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ebd0)
void ScriptThread::SCRCMD_WEAP_HIT_CAR_50EBD0()
{
    SCR_CHECK_WEAPONHIT* pCmd = reinterpret_cast<SCR_CHECK_WEAPONHIT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (!pCmd->field_A_status)
    {
        bool hit;
        if (gBasePtr_6F8070->field_2_type == SCRCMD_WEAP_HIT_CAR)
        {
            hit = gScriptManager_6F8060->TestAndRecordWeaponHit_512AF0(pPointer->field_8_car->field_6C_car_id, static_cast<char_type>(pCmd->field_C_weapon), false);
        }
        else
        {
            hit = gScriptManager_6F8060->TestAndRecordWeaponHit_512AF0(pPointer->field_8_car->field_6C_car_id, ped_death_cause::any_23, false);
        }
        if (hit && gScriptManager_6F8060->IsWeaponHitRecorded_512C70(pPointer->field_8_car->field_6C_car_id, static_cast<char_type>(pCmd->field_C_weapon), false))
        {
            field_8_cond_result = true;
            pCmd->field_A_status = 0;
        }
        else
        {
            field_8_cond_result = false;
            pCmd->field_A_status = 1;
        }
    }
    else
    {
        if (gScriptManager_6F8060->IsWeaponHitRecorded_512C70(pPointer->field_8_car->field_6C_car_id, static_cast<char_type>(pCmd->field_C_weapon), false))
        {
            field_8_cond_result = true;
            pCmd->field_A_status = 0;
        }
        else
        {
            field_8_cond_result = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ece0)
void ScriptThread::SCRCMD_IS_CHAR_ON_FIRE_50ECE0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    if (pPointer->field_8_char->IsOnFire_475B50())
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ed40)
void ScriptThread::SCRCMD_BRIEF_ONSCREEN_50ED40()
{
    Hud_BriefEntry* pCurrBrief = gHud_2B00_706620->field_DC_brief.field_6F8_curr_briefs;
    if (pCurrBrief != NULL && pCurrBrief->field_8_brief_priority != 0)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ed80)
void ScriptThread::SCRCMD_CREATE_SOUND_50ED80()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    ScriptThread::SCRCMD_SOUND_DECSET_505340(reinterpret_cast<SCR_SOUND_DECSET*>(gBasePtr_6F8070), pPointer);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50edc0)
void ScriptThread::SCRCMD_DO_EASY_PHONE_50EDC0() //  EASY_PHONE_TEMPLATE
{
    SCR_PHONE_TEMPLATE* pCmd = reinterpret_cast<SCR_PHONE_TEMPLATE*>(gBasePtr_6F8070);

    SCR_POINTER* pGang1MissionFlag;
    SCR_POINTER* pGang2MissionFlag;
    SCR_POINTER* pGang3MissionFlag;
    SCR_POINTER* pPassedFlag;
    SCR_POINTER* pFailedFlag;

    pPassedFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_counter1));
    pFailedFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_10_counter2));
    pGang1MissionFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_12_counter3));
    pGang2MissionFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_14_counter4));
    pGang3MissionFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_16_counter5));
    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(pCmd->field_18_gangname);
    char_type* pGangName = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pGangName);
    if (pGang1MissionFlag->field_8_counter == 1) //  flag_on_[gang1]_mission == 1
    {
        s32 brief_id = pCmd->field_8_brief_id - 5;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pGang2MissionFlag->field_8_counter == 1) //  flag_on_[gang2]_mission == 1
    {
        s32 brief_id = pCmd->field_8_brief_id - 2;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pGang3MissionFlag->field_8_counter == 1) //  flag_on_[gang3]_mission == 1
    {
        s32 brief_id = pCmd->field_8_brief_id - 1;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pPassedFlag->field_8_counter == 1) //  flag_passed_[gang1]_easy_phone_m1 == 1
    {
        //  not used in original levels, since phones are disabled after finishing mission
        s32 brief_id = pCmd->field_8_brief_id;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pFailedFlag->field_8_counter == 1) //  flag_failed_[this_gang]_easy_phone_m1 == 1
    {
        //  not used in the game as you can repeat a failed mission in GTA2
        s32 brief_id = pCmd->field_8_brief_id + 1;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    s8 player_idx1;
    //  Check if the player has respect enough to launch the mission
    if ((player_idx1 = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0(),
         pGang->GetRespectForPlayer_4BEEF0(player_idx1) / 20 >= pCmd->field_1A_respect))
    {
        str_table_entry* mission1_str_table = gScriptManager_6F8060->FindStringById_503080(pCmd->field_1C_mission1);
        char_type* mission1_name = mission1_str_table->get_name();
        ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, mission1_name); //  TODO: fix gBasePtr_6F8070 type
        return;
    }
    s8 player_idx2;
    //  Check if the player has respect = 0
    if ((player_idx2 = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0(), (pGang->GetRespectForPlayer_4BEEF0(player_idx2) / 20) == 0))
    {
        s32 brief_id = pCmd->field_8_brief_id - 6;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    s8 player_idx3;
    //  Check if the player has negative respect
    if ((player_idx3 = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0(), pGang->GetRespectForPlayer_4BEEF0(player_idx3) / 20 < 0))
    {
        s32 brief_id = pCmd->field_8_brief_id - 4;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
    else
    {
        //  In this case, the player has positive respect but not enough to launch the mission
        s32 brief_id = pCmd->field_8_brief_id - 7;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
}

MATCH_FUNC(0x50f060)
void ScriptThread::SCRCMD_CHAR_INTO_CAR_50F060()
{
    SCR_CHAR_INTO_CAR* pCmd = reinterpret_cast<SCR_CHAR_INTO_CAR*>(gBasePtr_6F8070);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_char_idx));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_10_car_idx));

    Ped* pNewPed = gPedManager_6787BC->AllocatePed_470F30();
    pCharPointer->field_8_char = pNewPed;

    if (pNewPed)
    {
        pNewPed->field_238_ped_type = ped_type::script_created_5;
        pCharPointer->field_8_char->SpawnPedInCar_45C730(pCarPointer->field_8_car);
        pCharPointer->field_8_char->field_244_remap = pCmd->field_C_remap;
        pCharPointer->field_8_char->field_240_occupation = pCmd->field_E_occupation;

        if (!field_118_in_level_start)
        {
            gMiss2_25C_6F805C->push_type_3_ped_502FB0(pCharPointer->field_8_char);
        }
    }

    Car* pCarTest = pCarPointer->field_8_car;

    if (pCarTest)
    {
        pCarTest->field_7C_ped_type = ped_type::script_created_5;
        pCarTest->field_76_last_seen_timer = 0;

        Car* pCar = pCarPointer->field_8_car;

        if (!pCar->field_5C_AI)
        {
            pCar->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
        }

        pCarPointer->field_8_car->field_5C_AI->SetCar_453BF0(pCarPointer->field_8_car);
        pCarPointer->field_8_car->SetupCarPhysicsAndSpriteBinding_43BCA0();
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f150)
void ScriptThread::SCRCMD_DECIDE_POWERUP_50F150()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pGeneratorPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));

    Car* pCar = pCarPointer->field_8_car;
    u8* pCarList = reinterpret_cast<u8*>(gScriptManager_6F8060->field_340_car_list);

    u16 i;

    for (i = 0; i < 19; i++)
    {
        if (pCar != NULL && pCar->field_84_car_info_idx == *pCarList)
        {
            pGeneratorPointer->field_8_generator->field_0_gen_type = kDecidePowerupGenTypes_6212F0[i];
            Generator* pGen = pGeneratorPointer->field_8_generator;

            if (kDecidePowerupGenTypes_6212F0[i] < 91)
            {
                pGen->Activate_4C1A70();
                pGen->field_1E_spawns_left = 3;
            }
            else
            {
                pGen->Activate_4C1A70();
                pGen->field_1E_spawns_left = 1;
            }

            break;
        }

        pCarList++;
    }

    if (i == 19)
    {
        Generator* pGen = pGeneratorPointer->field_8_generator;
        pGen->Activate_4C1A70();
        pGen->field_1E_spawns_left = 3;
        pGeneratorPointer->field_8_generator->field_0_gen_type = 65;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f220)
void ScriptThread::SCRCMD_SUPPRESS_MODEL_50F220()
{
    if (gBasePtr_6F8070->field_8_index == 0xFFFFu) //  TODO: fix gBasePtr_6F8070
    {
        gCarManager_6C_677930->field_C_suppressed_model = car_model_enum::none; //  No model suppressed
    }
    else
    {
        gCarManager_6C_677930->field_C_suppressed_model = gBasePtr_6F8070->field_8_index;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f270)
void ScriptThread::SCRCMD_WARP_CHAR_50F270() // WARP_FROM_CAR_TO_POINT
{
    SCR_WARP_FROM_CAR* pCmd = reinterpret_cast<SCR_WARP_FROM_CAR*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;
    Car* pCar = pPed->get_car_416B60();

    if (pPed != NULL)
    {
        pPed->StartPedWalking_470200(pCmd->field_C_pos.x, pCmd->field_C_pos.y, pCmd->field_C_pos.z);

        Ang16 rotation = kDegreesToAng16_6F8044 * pCmd->field_A_rotation;
        rotation.Normalize();

        pPointer->field_8_char->SetRotation_433C00(rotation);
        pCar->ClearDriver_4407F0();
        pCar->field_54_driver = NULL;
        gGame_0x40_67E008->field_38_local_player->DisableInputs_569F40();
        gGame_0x40_67E008->field_38_local_player->field_90_game_camera.UpdateFollowPedCamera_436540(pPointer->field_8_char);

        Camera* pCamera = &(gGame_0x40_67E008->field_38_local_player->field_90_game_camera);

        pCamera->CommitPendingTarget_41E410();

        gGame_0x40_67E008->field_38_local_player->field_90_game_camera.ResetCameraSmoothing_435DD0();
        gGame_0x40_67E008->field_38_local_player->field_90_game_camera.SetSmoothFollow_475B60();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f3d0)
void ScriptThread::SCRCMD_SET_GROUP_TYPE_50F3D0()
{
    SCR_SET_GROUP_TYPE* pCmd = reinterpret_cast<SCR_SET_GROUP_TYPE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;
    PedGroup* pGroup = pPed->field_164_ped_group;

    if (pGroup != NULL)
    {
        pGroup->field_38_group_type = pCmd->field_C_group_type;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f410)
void ScriptThread::SCRCMD_CHAR_DO_NOTHING_50F410()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        pPed->ForceDoNothing_462590();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f450)
void ScriptThread::SCRCMD_EMERG_LIGHTS_50F450()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (pPointer->field_8_car->HasEmergencyLights_414F20())
    {
        if (static_cast<u8>(pCmd->field_A_unsigned_2) == 1)
        {
            pPointer->field_8_car->ActivateEmergencyLights_43C920(); //  activate sirens
        }
        else
        {
            pPointer->field_8_car->DeactivateEmergencyLights_43C9D0(); //  deactivate sirens
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f4d0)
void ScriptThread::SCRCMD_CHECK_OBJ_MODEL_50F4D0()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Object* pObj = pPointer->field_8_obj;
    s32 model_idx = pCmd->field_C_u32;

    if (pObj != NULL && pObj->field_18_model == model_idx)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f550)
void ScriptThread::SCRCMD_PED_GRAPHIC_50F550()
{
    SCR_SET_CHAR_GRAPHIC_TYPE* pCmd = reinterpret_cast<SCR_SET_CHAR_GRAPHIC_TYPE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (pPointer->field_8_char != NULL)
    {
        pPointer->field_8_char->field_26C_graphic_type = pCmd->field_C_graphic_type;
        pPointer->field_8_char->field_244_remap = static_cast<char_type>(pCmd->field_A_remap);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f5e0)
void ScriptThread::SCRCMD_SET_CHAR_FLAGS_50F5E0()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr_6F8070);
    Ped* pPed = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<SCR_SET_STATE*>(gBasePtr_6F8070)->field_8_varname))->field_8_char;

    if (pPed)
    {
        switch (gBasePtr_6F8070->field_2_type)
        {
            case SCRCMD_CHAR_INVINCIBLE:
                if (pCmd->field_A_status == 1)
                {
                    pPed->field_208_invulnerability = 9999;
                }
                else
                {
                    pPed->field_208_invulnerability = 0;
                }

                break;
            case SCRCMD_SET_STAY_IN_CAR:
                if (pCmd->field_A_status == 1)
                {
                    pPed->field_21C_bf.bStayInCar = true;
                }
                else
                {
                    pPed->field_21C_bf.bStayInCar = false;
                }

                break;
            case SCRCMD_SET_USE_CAR_WEAPON:
                if (pCmd->field_A_status == 1)
                {
                    pPed->field_21C_bf.bUseCarWeapon = true;
                }
                else
                {
                    pPed->field_21C_bf.bUseCarWeapon = false;
                }

                break;
            case SCRCMD_DEATH_ARR_STATE:
                gScriptManager_6F8060->field_355_death_arr_state = pCmd->field_A_status;
                break;
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f770)
void ScriptThread::SCRCMD_MAKE_MUGGERS_50F770()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr_6F8070);
    if (pCmd->field_A_status == 1)
    {
        gPedManager_6787BC->field_7_make_all_muggers = true;
    }
    else
    {
        gPedManager_6787BC->field_7_make_all_muggers = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f7b0)
void ScriptThread::LocSecChar_50F7B0()
{
    SCR_LOC_SEC_CHAR* pCmd = reinterpret_cast<SCR_LOC_SEC_CHAR*>(gBasePtr_6F8070);
    SCR_POINTER* pChar1 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_char1_idx));
    SCR_POINTER* pChar2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_char2_idx));

    Ped* pCenter = pChar2->field_8_char;
    Fix16 centerX = pCenter->field_1AC_cam.x;
    Fix16 centerY = pCenter->field_1AC_cam.y;
    Fix16 centerZ = pCenter->field_1AC_cam.z;

    Fix16 width = pCmd->field_C_x_range / kFpTwo_6F77C8;
    Fix16 height = pCmd->field_10_y_range / kFpTwo_6F77C8;

    Fix16 testX = pChar1->field_8_char->field_1AC_cam.x;
    Fix16 testY = pChar1->field_8_char->field_1AC_cam.y;
    Fix16 testZ = pChar1->field_8_char->field_1AC_cam.z;
    Fix16 maxX;
    field_8_cond_result = false;

    if (testX >= centerX - width && (maxX = centerX + width, testX <= maxX) && testY >= centerY - height &&
        (centerY = centerY + height, testY <= centerY) && testZ.ToUInt8() == centerZ.ToUInt8())
    {
        switch (gBasePtr_6F8070->field_2_type)
        {
            case SCRCMD_LOC_SECOND_CHAR:
                if (pChar1->field_8_char->field_168_game_object)
                {
                    field_8_cond_result = true;
                }

                break;
            case SCRCMD_LOC_SEC_CHAR_CAR:
                if (pChar1->field_8_char->field_16C_car)
                {
                    field_8_cond_result = true;
                }

                break;
            case SCRCMD_LOC_SEC_CHAR_ANY:
                field_8_cond_result = true;
                break;
            default:
                break;
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f900)
void ScriptThread::SCRCMD_STOP_CAR_DRIVE_50F900()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Car* pCar = pPointer->field_8_char->field_16C_car;

    if (pCar)
    {
        pCar->field_A6_bf.bAIHalted = true;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f940)
void ScriptThread::SCRCMD_IS_BUS_FULL_50F940()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    field_8_cond_result = false;
    Ped* pPed = pPointer->field_8_char;

    if (pPed)
    {
        Car* pCar = pPed->field_16C_car;
        if (pCar)
        {
            if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(pCar) && gPublicTransport_181C_6FF1D4->is_bus_full_579AF0())
            {
                field_8_cond_result = true;
            }
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f9b0)
void ScriptThread::SCRCMD_NO_CHARS_OFF_BUS_50F9B0()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr_6F8070);
    if (pCmd->field_A_status == 1)
    {
        gPublicTransport_181C_6FF1D4->field_1818_stop_getting_off_bus = true;
    }
    else
    {
        gPublicTransport_181C_6FF1D4->field_1818_stop_getting_off_bus = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa00)
void ScriptThread::SCRCMD_KILL_CHAR_50FA00()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        pPed->Kill_46F9D0(); // kill ped
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa40)
void ScriptThread::SCRCMD_SET_SHADING_LEV_50FA40()
{
    gpMapRenderer_6F66E4->set_shading_lev_4E9DB0(static_cast<u8>(static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[1].field_2_type));
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa70)
void ScriptThread::SCRCMD_SET_CAR_JAMMED_50FA70()
{
    SCR_SET_CAR_JAMMED* pCmd = reinterpret_cast<SCR_SET_CAR_JAMMED*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Car* pCar = pPointer->field_8_car;

    if (pCar)
    {
        if (pCmd->field_A_status == 1)
        {
            pCar->field_78_flags |= car_flag::Flag12;
        }
        else
        {
            pCar->field_78_flags &= static_cast<u16>(~car_flag::Flag12);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fad0)
void ScriptThread::SCRCMD_FINISH_MISSION_50FAD0()
{
    gMiss2_25C_6F805C->MissionCleanUp_502DC0();
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50faf0)
void ScriptThread::SCRCMD_STORE_BONUS_50FAF0()
{
    SCR_STORE_BONUS_COUNT* pCmd = reinterpret_cast<SCR_STORE_BONUS_COUNT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointerBonus = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pPointerCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_counter_idx));

    s32 bonus_rule_idx = pPointerBonus->field_8_counter;

    if (bonus_rule_idx != bonus_result::passed && bonus_rule_idx != bonus_result::failed)
    {
        pPointerCounter->field_8_counter =
            gGame_0x40_67E008->field_38_local_player->field_2D4_scores.field_1A8_bonuses.GetProgressCount_476660(pPointerBonus->field_8_index);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fb60)
void ScriptThread::SCRCMD_SET_ENTER_STATUS_50FB60() // SCRCMD_SET_ENTER_STATUS and SCRCMD_SET_ALL_CONTROLS
{
    SCR_SET_PLAYER_CONTROLS* pCmd = reinterpret_cast<SCR_SET_PLAYER_CONTROLS*>(gBasePtr_6F8070);

    gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index);

    // The player pointer is not used, so the user controls are modified instead
    // This explains desync in multiplayer when SET_ENTER_CONTROL_STATUS are used

    if (pCmd->field_A_new_state == 1)
    {
        if (gBasePtr_6F8070->field_2_type == SCRCMD_SET_ENTER_STATUS)
        {
            gGame_0x40_67E008->field_38_local_player->EnableEnterVehicles_56A040(); // SET_ENTER_STATUS = ON
        }
        else
        {
            gGame_0x40_67E008->field_38_local_player->EnableAllControls_56A000(); // SET_ALL_CONTROLS = ON
        }
    }
    else if (gBasePtr_6F8070->field_2_type == SCRCMD_SET_ENTER_STATUS)
    {
        gGame_0x40_67E008->field_38_local_player->DisableEnterVehicles_56A030(); // SET_ENTER_STATUS = OFF
    }
    else
    {
        gGame_0x40_67E008->field_38_local_player->DisableAllControls_569FF0(); // SET_ALL_CONTROLS = OFF
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fc20)
void ScriptThread::SCRCMD_SET_FAV_CAR_50FC20()
{
    SCR_SET_FAV_CAR* pCmd = reinterpret_cast<SCR_SET_FAV_CAR*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        pPed->field_274_gang_car_model = pCmd->field_C_favourite_car;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fc60)
void ScriptThread::GroupInArea_50FC60()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Fix16 halfwidth = pCmd->field_C_rect.field_C_size.field_0_x / kFpTwo_6F77C8;
    Fix16 halfheight = pCmd->field_C_rect.field_C_size.field_4_y / kFpTwo_6F77C8;

    PedGroup* pGroup = pPointer->field_8_char->field_164_ped_group;

    Ped* pMember = pGroup->field_4_ped_list[0];
    Ped** ppMember = pGroup->field_4_ped_list;

    if (pMember != NULL)
    {
        for (u16 i = 0; i < pGroup->field_34_count; i++)
        {
            Fix16 charX = pMember->field_1AC_cam.x;
            Fix16 centerX = pCmd->field_C_rect.field_0_pos.x;
            Fix16 centerY, charY;

            if (charX >= centerX - halfwidth && charX <= centerX + halfwidth &&
                (centerY = pCmd->field_C_rect.field_0_pos.y, charY = pMember->field_1AC_cam.y, charY >= centerY - halfheight) &&
                charY <= centerY + halfheight)
            {
                field_8_cond_result = true;
            }
            else
            {
                field_8_cond_result = false;
            }

            ++ppMember;
            pMember = *ppMember;
        }
    }

    Ped* pLeader = pGroup->field_2C_ped_leader;
    Fix16 leaderX = pLeader->field_1AC_cam.x;
    Fix16 centerX = pCmd->field_C_rect.field_0_pos.x;
    Fix16 centerY, leaderY;

    if (leaderX >= centerX - halfwidth && leaderX <= centerX + halfwidth &&
        (centerY = pCmd->field_C_rect.field_0_pos.y, leaderY = pLeader->field_1AC_cam.y, leaderY >= centerY - halfheight) &&
        leaderY <= centerY + halfheight)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fe00)
void ScriptThread::SCRCMD_SET_KF_WEAPON_50FE00()
{
    SCR_SET_KF_WEAPON* pCmd = reinterpret_cast<SCR_SET_KF_WEAPON*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    s32 weapon_idx = pCmd->field_C_weapon_idx;

    if (is_car_weapon(weapon_idx))
    {
        pPointer->field_8_char->field_15C_player->SetKFCarWeapon_564710(pPointer->field_8_char->field_16C_car, weapon_idx);
    }
    else
    {
        pPointer->field_8_char->field_15C_player->SetKFWeapon_564790(weapon_idx);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fed0)
void ScriptThread::SCRCMD_CLEAR_KF_WEAPON_50FED0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Player* pPlayer = pPointer->field_8_char->field_15C_player;

    pPlayer->ClearKFWeapon_5647D0(); // clear kill frenzy weapon
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ff50)
void ScriptThread::SCRCMD_ADD_ONSCREEN_COUNTER_50FF50()
{
    SCR_ADD_ONSCREEN_COUNTER* pCmd = reinterpret_cast<SCR_ADD_ONSCREEN_COUNTER*>(gBasePtr_6F8070);
    SCR_POINTER* pOnScreenCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_counter_idx));

    pOnScreenCounter->field_8_index = gHud_2B00_706620->field_620_pagers.AddOnScreenCounter_5D3220(&pCounter->field_8_counter);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ffb0)
void ScriptThread::SCRCMD_CLEAR_COUNTER_50FFB0() // SCRCMD_CLEAR_COUNTER and SCRCMD_CLEAR_CLOCK_ONLY
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    u16 idx = pPointer->field_8_index;

    if (idx != 0xFFFF)
    {
        if (gBasePtr_6F8070->field_2_type == SCRCMD_CLEAR_COUNTER)
        {
            gHud_2B00_706620->field_620_pagers.ClearCounterOnly_5D3310(idx);
        }
        else // == SCRCMD_CLEAR_CLOCK_ONLY
        {
            gHud_2B00_706620->field_620_pagers.ClearClockOnly_5D32D0(idx);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510030)
void ScriptThread::SCRCMD_CHANGE_POLICE_510030()
{
    ScriptThread::SCRCMD_DECLARE_POLICE_5052C0(reinterpret_cast<SCR_DECLARE_POLICELEVEL*>(gBasePtr_6F8070));
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510050)
void ScriptThread::SCRCMD_DESTROY_GROUP_510050()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    Ped* pGroupLeader = pPointer->field_8_char;
    PedGroup* pGroup = pGroupLeader->field_164_ped_group;

    if (pGroup)
    {
        pGroup->DestroyGroup_4C93A0(); // destroy group
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510090)
void ScriptThread::SCRCMD_CHECK_CURRENT_WEAPON_510090()
{
    SCR_CHECK_CURRENT_WEAPON* pCmd = reinterpret_cast<SCR_CHECK_CURRENT_WEAPON*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    Weapon* pWeapon = pPointer->field_8_char->field_15C_player->GetCurrPlayerWeapon_5648F0();

    if (pWeapon != NULL && static_cast<u32>(pWeapon->field_1C_weapon_type) == pCmd->field_C_weapon_idx)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510100)
void ScriptThread::SCRCMD_START_BASIC_KF_510100() // START_BASIC_KF_TEMPLATE
{
    SCR_START_BASIC_KF_TEMPLATE* pCmd = reinterpret_cast<SCR_START_BASIC_KF_TEMPLATE*>(gBasePtr_6F8070);
    if (gScriptManager_6F8060->get_field_C1E2D_475A30())
    {
        ScriptThread::EnableThread_50A9E0(pCmd->field_8_triggername);
        ScriptThread::SCRCMD_RETURN_506B80();
    }
    else
    {
        gScriptManager_6F8060->set_field_C1E2D_475A40(true);
        ScriptThread::DisableThread_505790(pCmd->field_8_triggername);
        gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("kfstart"), hud_message_priority::important_3); // KILL FRENZY!
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::killfrenzy_12); // KILL FRENZY voice
        SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_C_objname)); // Skull icon?
        if (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->field_2_type) == script_entity_type::object_3)
        {
            ScriptThread::DeallocOrDeleteItem_505B10(pCmd->field_C_objname); // Delete skull icon?
        }
        sprintf(gTmpBuffer_67C598, "%d", pCmd->field_A_brief_id);
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        SCR_POINTER* pPlayerPedPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_E_playername));
        u8 weapon_idx = static_cast<u8>(pCmd->field_10_weapon);
        if (weapon_idx < weapon_type::car_bomb || weapon_idx > 27)
        {
            // Ped weapons
            if (weapon_idx != 28)
            {
                pPlayerPedPtr->field_8_char->field_15C_player->SetKFWeapon_564790(static_cast<u8>(pCmd->field_10_weapon));
            }
        }
        else
        {
            // Car weapons
            pPlayerPedPtr->field_8_char->field_15C_player->SetKFCarWeapon_564710(pPlayerPedPtr->field_8_char->get_car_416B60(),
                                                                                 static_cast<u8>(pCmd->field_10_weapon));
        }

        u16* pBasicKF = gScriptManager_6F8060->field_C1E74_basic_kf;
        for (u16 idx = 0; idx < GTA2_COUNTOF(gScriptManager_6F8060->field_C1E74_basic_kf); idx++)
        {
            if (*pBasicKF == static_cast<u16>(gBasePtr_6F8070->field_0_cmd_this))
            {
                gGameSave_6F78C8.field_E4_car_and_script_data.field_4C_started_basic_kf_mask |= 1 << idx;
                break;
            }
            pBasicKF++;
        }
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
}

MATCH_FUNC(0x510280)
void ScriptThread::SCRCMD_DO_BASIC_KF_510280()
{
    SCR_POINTER* pTimerIDPointer;

    SCR_DO_BASIC_KF_TEMPLATE* pCmd = reinterpret_cast<SCR_DO_BASIC_KF_TEMPLATE*>(gBasePtr_6F8070);
    SCR_POINTER* pPlayerPedCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_18_playername));

    if (!field_C_cmd_initialised)
    {
        pTimerIDPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_timername));
        pTimerIDPointer->field_8_index = gHud_2B00_706620->field_620_pagers.CreateTimer_5D31F0(pCmd->field_C_time_limit);
        SCR_POINTER* pOnScreenCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_10_onscreenname));
        SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_12_countername));
        pOnScreenCounter->field_8_index = gHud_2B00_706620->field_620_pagers.AddOnScreenCounter_5D3220(&pCounter->field_8_counter);
        ++field_C_cmd_initialised;
    }
    else
    {
        if (ScriptThread::GetBonusResult_505EA0(pCmd->field_8_bonusname) == bonus_result::in_progress)
        {
            SCR_POINTER* pBonusType = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_bonusname));
            SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_12_countername));
            pCounter->field_8_counter = pCmd->field_14_target_total -
                gGame_0x40_67E008->field_38_local_player->field_2D4_scores.field_1A8_bonuses.GetProgressCount_476660(pBonusType->field_8_index);

            Ped* pPed = pPlayerPedCmdPointer->field_8_char;

            if (pPed->GetPedState_403990() == ped_state_1::dead_9 || pPed->field_21C_bf.bBusted != 0)
            {
                gGame_0x40_67E008->field_38_local_player->field_2D4_scores.field_1A8_bonuses.DeactivateBonus_476680(pBonusType->field_8_index);
                pPlayerPedCmdPointer->field_8_char->field_15C_player->ClearKFWeapon_5647D0();
            }
        }
        else
        {
            pTimerIDPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_timername));
            if (pTimerIDPointer->field_8_index != 0xFFFF)
            {
                gHud_2B00_706620->field_620_pagers.ClearPager_5D3280(pTimerIDPointer->field_8_index);
            }
            pTimerIDPointer->field_8_index = 0;
            pPlayerPedCmdPointer->field_8_char->field_15C_player->ClearKFWeapon_5647D0();
            gScriptManager_6F8060->set_field_C1E2D_475A40(false);
            if (ScriptThread::GetBonusResult_505EA0(pCmd->field_8_bonusname) == -3)
            {
                gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("kfpass"), hud_message_priority::important_3);
                pPlayerPedCmdPointer->field_8_char->ClearWantedPoints_420B80();
                ++*gScriptManager_6F8060->field_338_secrets_passed;

                if (pCmd->field_1A_rewardtype == 1)
                {
                    pPlayerPedCmdPointer->field_8_char->field_15C_player->AddScore_41DC40(pCmd->field_1C_rewardvalue);
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::frenzypassed_19);
                }
                else if (pCmd->field_1A_rewardtype == 2)
                {
                    pPlayerPedCmdPointer->field_8_char->field_15C_player->ChangeMultipliers_4766B0(pCmd->field_1C_rewardvalue);
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::frenzypassed_19);
                }
                else
                {
                    if (pCmd->field_1A_rewardtype == 3)
                    {
                        pPlayerPedCmdPointer->field_8_char->field_15C_player->ChangeLifeCountByAmount_5699F0(pCmd->field_1C_rewardvalue);
                    }
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::frenzypassed_19);
                }
            }
            else
            {
                gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("kffail"), hud_message_priority::important_3);
                ++*gScriptManager_6F8060->field_33C_secrets_failed;
                gRoot_sound_66B038.PlayVoice_40F090(voice_line::frenzyfail_18);
            }
            ScriptThread::Next_503620(gBasePtr_6F8070);
        }
    }
}

MATCH_FUNC(0x510530)
void ScriptThread::SCRCMD_SET_BONUS_RATING_510530()
{
    gGameSession_67E8E0.SetBonusRatingTextIdx_4C5AB0(static_cast<SCR_CMD_HEADER*>(gBasePtr_6F8070)[1].field_2_type);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510560)
void ScriptThread::SCRCMD_ADD_TIME_510560()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    gHud_2B00_706620->field_620_pagers.AddTime_5D32F0(pPointer->field_8_index, 30 * pCmd->field_C_u32);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5105b0)
void ScriptThread::SCRCMD_SET_MODEL_WANTED_5105B0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    s16 model_idx = pCmd->field_A_signed_2;

    if (model_idx == -1)
    {
        gScriptManager_6F8060->field_C1E70_wanted_car_model = car_model_enum::none;
    }
    else
    {
        gScriptManager_6F8060->field_C1E70_wanted_car_model = model_idx;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510600)
void ScriptThread::SCRCMD_CHECK_DEATH_ARR_510600()
{
    if (gScriptManager_6F8060->field_C1E2E_death_arrest_flag)
    {
        field_8_cond_result = true;
    }
    else
    {
        field_8_cond_result = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510660)
void ScriptThread::SCRCMD_FORCE_CLEANUP_510660()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));

    if (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->field_2_type) == script_entity_type::object_3)
    {
        gMiss2_25C_6F805C->push_type_2_502FF0(pPointer->field_8_obj, true);
    }
    else if (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->field_2_type) == script_entity_type::sound_10)
    {
        gMiss2_25C_6F805C->push_type_2_503050(pPointer->field_8_obj);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510780)
void ScriptThread::SCRCMD_SAVE_RESTORE_RESPECT_510780()
{
    u8 idx = 0;
    Gang* pGang = gGangPool_CA8_67E274->FirstGang_4BECA0();

    if (gBasePtr_6F8070->field_2_type == SCRCMD_SAVE_RESPECT)
    {
        for (; pGang != NULL; pGang = gGangPool_CA8_67E274->NextGang_4BECE0())
        {
            gScriptManager_6F8060->field_C1E2F_saved_gang_respect[idx++] = pGang->GetRespectForPlayer_4BEEF0(0);
        }
    }
    else
    {
        for (; pGang != NULL; pGang = gGangPool_CA8_67E274->NextGang_4BECE0())
        {
            pGang->SetRespect_4BEE30(0, gScriptManager_6F8060->field_C1E2F_saved_gang_respect[idx++]);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5108d0)
void ScriptThread::PreExecOpCode_5108D0()
{
    if (field_10_stopped != 1)
    {
        SCR_CMD_HEADER* pCmd = gScriptManager_6F8060->GetBasePointer_512770(field_4_next_cmd);
        gBasePtr_6F8070 = static_cast<SCR_POINTER*>(pCmd);
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
                SCRCMD_IF_JUMP_506AF0();
                break;
            case SCRCMD_NOT:
                SCRCMD_NOT_507750();
                break;
            case SCRCMD_GOSUB:
                SCRCMD_GOSUB_506B30();
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
                SCRCMD_S_OP_S_506ED0();
                break;
            case SCRCMD_I_PLUS_S:
            case SCRCMD_I_MINUS_S:
                SCRCMD_I_OP_S_507110();
                break;
            case SCRCMD_S_PLUS_I:
            case SCRCMD_S_MINUS_I:
            case SCRCMD_S_LESS_I:
            case SCRCMD_S_LEQUAL_I:
            case SCRCMD_S_GREATER_I:
            case SCRCMD_S_GEQUAL_I:
            case SCRCMD_S_EQUAL_I:
                SCRCMD_S_OP_I_506D60();
                break;
            case SCRCMD_INCREMENT:
                SCRCMD_INCREMENT_507A70();
                break;
            case SCRCMD_DECREMENT:
                SCRCMD_DECREMENT_507B50();
                break;
            case SCRCMD_CREATE_OBJ_2D:
            case SCRCMD_CREATE_OBJ_3D:
            case SCRCMD_CREATE_OBJ_3D_INT:
            case SCRCMD_CREATE_OBJ_2D_INT:
            case SCRCMD_CREATE_OBJ_3D_STR:
            case SCRCMD_CREATE_OBJ_2D_STR:
                SCRCMD_CREATE_OBJ_507CE0();
                break;
            case SCRCMD_CREATE_CAR_2D:
            case SCRCMD_CREATE_CAR_3D:
            case SCRCMD_CREATE_CAR_2D_STR:
            case SCRCMD_CREATE_CAR_3D_STR:
            case SCRCMD_CREATE_GANG_CAR1:
            case SCRCMD_CREATE_GANG_CAR2:
            case SCRCMD_CREATE_GANG_CAR3:
            case SCRCMD_CREATE_GANG_CAR4:
                SCRCMD_CREATE_CAR_507F80();
                break;
            case SCRCMD_MAKE_CAR_DUMMY:
                SCRCMD_MAKE_CAR_DUMMY_508220();
                break;
            case SCRCMD_CREATE_CHAR_2D:
            case SCRCMD_CREATE_CHAR_3D:
                SCRCMD_CREATE_CHAR_509730();
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
                SCRCMD_SET_DENSITY_508280();
                break;
            case SCRCMD_POINT_ARROW_3D:
            case SCRCMD_LEVEL_END_ARROW2:
                SCRCMD_POINT_ARROW_3D_508550();
                break;
            case SCRCMD_ARROW_COLOUR:
                SCRCMD_ARROW_COLOUR_508DC0();
                break;
            case SCRCMD_POINT_ARROW_AT:
            case SCRCMD_LEVEL_END_ARROW1:
                SCRCMD_POINT_ARROW_AT_5086F0();
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
                SCRCMD_DISPLAY_MESSAGE_5093C0();
                break;
            case SCRCMD_DISPLAY_TIMER:
                SCRCMD_DISPLAY_TIMER_5096E0();
                break;
            case SCRCMD_SET_CHAR_OBJ1:
                SCRCMD_SET_CHAR_OBJ1_5097D0();
                break;
            case SCRCMD_IS_CHAR_OBJ_PASS:
                SCRCMD_IS_CHAR_OBJ_PASS_509810();
                break;
            case SCRCMD_IS_CHAR_OBJ_FAIL:
                SCRCMD_IS_CHAR_OBJ_FAIL_509880();
                break;
            case SCRCMD_GIVE_WEAPON1:
            case SCRCMD_GIVE_WEAPON2:
                SCRCMD_GIVE_WEAPON_5098E0();
                break;
            case SCRCMD_IS_CAR_IN_BLOCK:
                SCRCMD_IS_CAR_IN_BLOCK_509990();
                break;
            case SCRCMD_HAS_CHAR_DIED:
                SCRCMD_HAS_CHAR_DIED_509BB0();
                break;
            case SCRCMD_IS_CHAR_IN_CAR:
                SCRCMD_IS_CHAR_IN_CAR_509C10();
                break;
            case SCRCMD_IS_CHAR_IN_MODEL:
                SCRCMD_IS_CHAR_IN_MODEL_509C90();
                break;
            case SCRCMD_IS_CHAR_IN_ANY_CAR:
                SCRCMD_IS_CHAR_IN_ANY_CAR_509D00();
                break;
            case SCRCMD_DELETE_ITEM:
                SCRCMD_DELETE_ITEM_509D60();
                break;
            case SCRCMD_ADD_SCORE1:
            case SCRCMD_ADDSCORE_NO_MULT:
                SCRCMD_ADD_SCORE_509D90();
                break;
            case SCRCMD_IS_CHAR_STOPPED:
                SCRCMD_IS_CHAR_STOPPED_509E70();
                break;
            case SCRCMD_EXPLODE:
            case SCRCMD_EXPLODE_LARGE2:
            case SCRCMD_EXPLODE_SMALL2:
            case SCRCMD_EXPLODE_NO_RING2:
                SCRCMD_EXPLODE_509ED0();
                break;
            case SCRCMD_LOCATE_CHAR_ANY:
            case SCRCMD_LOCATE_CHAR_ONFOOT:
            case SCRCMD_LOCATE_CHAR_BY_CAR:
            case SCRCMD_STOP_LOCATE_CHAR_ANY:
            case SCRCMD_STOP_LOCATE_CHAR_FOOT:
            case SCRCMD_STOP_LOCATE_CHAR_CAR:
                Locate_509FD0();
                break;
            case SCRCMD_SET_CHAR_OBJ2:
                SCRCMD_SET_CHAR_OBJ2_50A200();
                break;
            case SCRCMD_SET_CHAR_OBJ3:
                SCRCMD_SET_CHAR_OBJ3_50A3E0();
                break;
            case SCRCMD_SET_THREAT_SEARCH:
            case SCRCMD_SET_THREAT_REACT:
            case SCRCMD_ADD_GROUP:
            case SCRCMD_SET_CHAR_SHOOT:
            case SCRCMD_SET_CHAR_BRAVERY:
                SCRCMD_THREAT_SEARCH_REACT_ADD_GROUP_50BBD0();
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
                IsOnScreen_50A760();
                break;
            case SCRCMD_DELAY_HERE:
                SCRCMD_DELAY_HERE_50A940();
                break;
            case SCRCMD_DELAY:
                SCRCMD_DELAY_50A980();
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
                SCRCMD_CHANGE_RESPECT_50ACF0();
                break;
            case SCRCMD_CHECK_RESPECT_GREATER:
            case SCRCMD_CHECK_RESPECT_LESS:
            case SCRCMD_CHECK_RESPECT_IS:
                RespectOperator_50AEF0();
                break;
            case SCRCMD_ADD_PATROL_POINT:
                SCRCMD_ADD_PATROL_POINT_50B0E0();
                break;
            case SCRCMD_SET_AMBIENT:
                SCRCMD_SET_AMBIENT_50B150();
                break;
            case SCRCMD_ANSWER_PHONE:
                SCRCMD_ANSWER_PHONE_50B180();
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
                SCRCMD_IS_CHAR_FIRE_ONSCREEN_50B3D0();
                break;
            case SCRCMD_CAR_DRIVE_AWAY:
                SCRCMD_CAR_DRIVE_AWAY_50B440();
                break;
            case SCRCMD_CHAR_TO_DRIVE_CAR:
            case SCRCMD_CHAR_TO_BACKDOOR:
                SCRCMD_CHAR_TO_DRIVE_CAR_50B4F0();
                break;
            case SCRCMD_DRIVER_OUT_CAR:
                SCRCMD_DRIVER_OUT_CAR_50B5A0();
                break;
            case SCRCMD_GIVE_DRIVER_BRAKE:
                SCRCMD_GIVE_DRIVER_BRAKE_50B600();
                break;
            case SCRCMD_SET_GANG_INFO1:
                SCRCMD_SET_GANG_INFO1_50B670();
                break;
            case SCRCMD_CLEAR_TIMERS:
                SCRCMD_CLEAR_TIMERS_50B690();
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
                SCRCMD_SET_NO_COLLIDE_50B8B0();
                break;
            case SCRCMD_IS_CHAR_FIRING_AREA:
                SCRCMD_IS_CHAR_FIRING_AREA_50B910();
                break;
            case SCRCMD_GET_PASSENGER_NUM:
                SCRCMD_GET_PASSENGER_NUM_50B9C0();
                break;
            case SCRCMD_CLEAR_WANTED_LEVEL:
                SCRCMD_CLEAR_WANTED_LEVEL_50BA30();
                break;
            case SCRCMD_CAR_WRECK_IN_LOCATION:
                SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0();
                break;
            case SCRCMD_SEND_CAR_TO_BLOCK:
                SCRCMD_SEND_CAR_TO_BLOCK_50BB80();
                break;
            case SCRCMD_CHECK_NUM_ALIVE:
                SCRCMD_CHECK_NUM_ALIVE_50BC60();
                break;
            case SCRCMD_ADD_CHAR_TO_GROUP:
            case SCRCMD_REMOVE_CHAR:
            case SCRCMD_MAKE_LEADER:
                SCRCMD_CHAR_GROUP_50BD10();
                break;
            case SCRCMD_SET_COUNTER_INT:
                SCRCMD_SET_COUNTER_50BDC0();
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
                SCRCMD_EXPLODE_50C5A0();
                break;
            case SCRCMD_PARK:
            case SCRCMD_PARK_NO_RESPAWN:
                SCRCMD_PARK_50C6F0();
                break;
            case SCRCMD_ALT_WANTED_LEVEL:
            case SCRCMD_ALTER_WANTED_LEVEL:
                SCRCMD_ALT_WANTED_LEVEL_50BA70();
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
                SCRCMD_CHANGE_BLOCK_50CA70();
                break;
            case SCRCMD_ROAD_ON_OFF:
                SCRCMD_ROAD_ON_OFF_50CB20();
                break;
            case SCRCMD_GET_CAR_FROM_CRANE:
                SCRCMD_GET_CAR_FROM_CRANE_50B470();
                break;
            case SCRCMD_SWITCH_GENERATOR1:
            case SCRCMD_SWITCH_GENERATOR2:
                SCRCMD_SWITCH_GENERATOR1_50CCB0();
                break;
            case SCRCMD_SWITCH_GENERATOR3:
            case SCRCMD_SWITCH_GENERATOR4:
                SCRCMD_SWITCH_GENERATOR3_50CD30();
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
                SCRCMD_RETURN_506B80();
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
                SCRCMD_CREATE_LIGHT_50DA50();
                break;
            case SCRCMD_CHECK_NUM_LIVES:
            case SCRCMD_CHECK_MULT:
            case SCRCMD_ADD_LIVES:
            case SCRCMD_ADD_MULTIPLIER:
                GetSetLivesAndMultiplier_50DB70();
                break;
            case SCRCMD_GET_NUM_LIVES:
            case SCRCMD_GET_MULT:
                SCRCMD_GET_LIVES_MULT_50DD00();
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
                StartBonus_50DF10();
                break;
            case SCRCMD_CHECK_BONUS1:
            case SCRCMD_CHECK_BONUS2:
            case SCRCMD_CHECK_BONUS3:
                SCRCMD_CHECK_BONUS_50E0B0();
                break;
            case SCRCMD_SETUP_MODEL_CHECK:
                SCRCMD_SETUP_MODEL_CHECK_50E120();
                break;
            case SCRCMD_MODEL_CHECK:
                SCRCMD_MODEL_CHECK_50E150();
                break;
            case SCRCMD_GET_CAR_SPEED:
            case SCRCMD_GET_CHAR_CAR_SPEED:
            case SCRCMD_GET_MAX_SPEED:
                GetSpeed_50E190();
                break;
            case SCRCMD_CHECK_CAR_SPEED:
                SCRCMD_CHECK_CAR_SPEED_50E360();
                break;
            case SCRCMD_SET_CAR_GRAPHIC:
                SCRCMD_SET_CAR_GRAPHIC_50E460();
                break;
            case SCRCMD_S_IS_S_MINUS_S:
            case SCRCMD_S_IS_S_PLUS_S:
            case SCRCMD_S_IS_S_DIV_S:
            case SCRCMD_S_IS_S_MULT_S:
            case SCRCMD_S_IS_S_MOD_S:
                SCRCMD_S_IS_S_OP_S_50E4F0();
                break;
            case SCRCMD_S_IS_S_MINUS_I:
            case SCRCMD_S_IS_S_PLUS_I:
            case SCRCMD_S_IS_S_DIV_I:
            case SCRCMD_S_IS_S_MULT_I:
            case SCRCMD_S_IS_S_MOD_I:
                SCRCMD_S_IS_S_OP_I_50E610();
                break;
            case SCRCMD_GIVE_CAR_ALARM:
                SCRCMD_GIVE_CAR_ALARM_50E7F0();
                break;
            case SCRCMD_CAR_BULLETPROOF:
            case SCRCMD_CAR_ROCKETPROOF:
            case SCRCMD_CAR_FLAMEPROOF:
                SCRCMD_CAR_WEAPON_KIND_PROOF_50E820();
                break;
            case SCRCMD_CHAR_DRIVE_AGGR:
                SCRCMD_CHAR_DRIVE_AGGR_50E730();
                break;
            case SCRCMD_CHAR_DRIVE_SPEED:
            case SCRCMD_SET_RUN_SPEED:
                SCRCMD_SET_SPEED_50E780();
                break;
            case SCRCMD_SET_CHAR_OBJ_FOLLOW:
                SCRCMD_SET_CHAR_OBJ_FOLLOW_50A460();
                break;
            case SCRCMD_PUT_CAR_ON_TRAILER:
                SCRCMD_PUT_CAR_ON_TRAILER_50E900();
                break;
            case SCRCMD_CLEAR_BRIEFS:
                SCRCMD_CLEAR_BRIEFS_50E9A0();
                break;
            case SCRCMD_CHECK_HEADS:
                SCRCMD_CHECK_HEADS_50E9E0();
                break;
            case SCRCMD_FINISH_LEVEL:
                SCRCMD_FINISH_LEVEL_50EA40();
                break;
            case SCRCMD_CHECK_WEAPONHIT:
                SCRCMD_CHECK_WEAPONHIT_50EB00();
                break;
            case SCRCMD_WEAP_HIT_CAR:
            case SCRCMD_ANY_WEAPON_HIT_CAR:
                SCRCMD_WEAP_HIT_CAR_50EBD0();
                break;
            case SCRCMD_IS_CHAR_ON_FIRE:
                SCRCMD_IS_CHAR_ON_FIRE_50ECE0();
                break;
            case SCRCMD_BRIEF_ONSCREEN:
                SCRCMD_BRIEF_ONSCREEN_50ED40();
                break;
            case SCRCMD_CREATE_SOUND:
                SCRCMD_CREATE_SOUND_50ED80();
                break;
            case SCRCMD_DO_EASY_PHONE:
                SCRCMD_DO_EASY_PHONE_50EDC0();
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
            case SCRCMD_WARP_CHAR:
                SCRCMD_WARP_CHAR_50F270();
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
                SCRCMD_SET_CHAR_FLAGS_50F5E0();
                break;
            case SCRCMD_LOC_SECOND_CHAR:
            case SCRCMD_LOC_SEC_CHAR_CAR:
            case SCRCMD_LOC_SEC_CHAR_ANY:
                LocSecChar_50F7B0();
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
            case SCRCMD_KILL_CHAR:
                SCRCMD_KILL_CHAR_50FA00();
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
                SCRCMD_STORE_BONUS_50FAF0();
                break;
            case SCRCMD_SET_ENTER_STATUS:
            case SCRCMD_SET_ALL_CONTROLS:
                SCRCMD_SET_ENTER_STATUS_50FB60();
                break;
            case SCRCMD_SET_FAV_CAR:
                SCRCMD_SET_FAV_CAR_50FC20();
                break;
            case SCRCMD_GROUP_IN_AREA:
                GroupInArea_50FC60();
                break;
            case SCRCMD_SET_CHAR_OCCUPATION:
                SCRCMD_SET_CHAR_OCCUPATION_509790();
                break;
            case SCRCMD_SET_KF_WEAPON:
                SCRCMD_SET_KF_WEAPON_50FE00();
                break;
            case SCRCMD_CLEAR_KF_WEAPON:
                SCRCMD_CLEAR_KF_WEAPON_50FED0();
                break;
            case SCRCMD_ADD_ONSCREEN_COUNTER:
                SCRCMD_ADD_ONSCREEN_COUNTER_50FF50();
                break;
            case SCRCMD_CLEAR_COUNTER:
            case SCRCMD_CLEAR_CLOCK_ONLY:
                SCRCMD_CLEAR_COUNTER_50FFB0();
                break;
            case SCRCMD_CHECK_CAR_DRIVER:
            case SCRCMD_IS_CAR_WRECKED:
            case SCRCMD_CAR_SUNK:
            case SCRCMD_CAR_IN_AIR:
            case SCRCMD_IS_CAR_CRUSHED:
            case SCRCMD_CARBOMB_ACTIVE:
            case SCRCMD_EMERG_LIGHTS_ON:
                SCRCMD_CHECK_CAR_DRIVER_50CB70();
                break;
            case SCRCMD_CHANGE_POLICE:
                SCRCMD_CHANGE_POLICE_510030();
                break;
            case SCRCMD_DESTROY_GROUP:
                SCRCMD_DESTROY_GROUP_510050();
                break;
            case SCRCMD_CHECK_CURRENT_WEAPON:
                SCRCMD_CHECK_CURRENT_WEAPON_510090();
                break;
            case SCRCMD_MAKE_MUGGERS:
                SCRCMD_MAKE_MUGGERS_50F770();
                break;
            case SCRCMD_START_BASIC_KF:
                SCRCMD_START_BASIC_KF_510100();
                break;
            case SCRCMD_DO_BASIC_KF:
                SCRCMD_DO_BASIC_KF_510280();
                break;
            case SCRCMD_SET_BONUS_RATING:
                SCRCMD_SET_BONUS_RATING_510530();
                break;
            case SCRCMD_ADD_TIME:
                SCRCMD_ADD_TIME_510560();
                break;
            case SCRCMD_DO_SAVE_GAME:
                SCRCMD_DO_SAVE_GAME_50D3C0();
                break;
            case SCRCMD_SET_MODEL_WANTED:
                SCRCMD_SET_MODEL_WANTED_5105B0();
                break;
            case SCRCMD_CHECK_DEATH_ARR:
                SCRCMD_CHECK_DEATH_ARR_510600();
                break;
            case SCRCMD_FORCE_CLEANUP:
                SCRCMD_FORCE_CLEANUP_510660();
                break;
            case SCRCMD_SAVE_RESPECT:
            case SCRCMD_RESTORE_RESPECT:
                SCRCMD_SAVE_RESTORE_RESPECT_510780();
                break;
            default:
                Next_503620(pCmd);
                break;
        }
    }
}

MATCH_FUNC(0x511840)
bool ScriptThread::PoolUpdate()
{ //  return boolean: true if it has reached LEVELEND, false otherwise
    //  I guess it's the script executed in mid-game on the current frame
    //  Normally each opcode is executed in one frame, unless there are WHILE_EXECs or EXECs blocks

    if (field_10_stopped == 1)
    {
        return true;
    }

    SCR_CMD_HEADER* pCmd = gScriptManager_6F8060->GetBasePointer_512770(field_4_next_cmd);

    if (!pCmd)
    {
        sprintf(gTmpBuffer_67C598, "Miss2: accessing nonexistant mission line. Current uid: %d", field_4_next_cmd);
    }

    ScriptThread::MissionFailOnDeathOrArrest_503200();

    if (pCmd->field_2_type == SCRCMD_START_EXEC)
    {
        do
        {
            ScriptThread::PreExecOpCode_5108D0();
            gScriptManager_6F8060->GetBasePointer_512770(field_4_next_cmd);
        } while (field_12_exec_flag > 0); //  execute opcodes in the same frame until an ENDEXEC
    }
    else //  It isn't an EXEC opcode
    {
        // field_6_return_value == 1  means it's in a WHILE_EXEC block
        if (pCmd->field_6_return_value == 1)
        {
            while (true) // execute commands in the same frame
            {
                SCR_CMD_HEADER* pCurrentCmd = gScriptManager_6F8060->GetBasePointer_512770(field_4_next_cmd);
                ScriptThread::PreExecOpCode_5108D0();

                //  If the script has reached a command out of the WHILE_EXEC block, or if it reaches LEVELEND
                if (pCurrentCmd->field_6_return_value != 1 || field_4_next_cmd == 0xFFFF)
                {
                    break;
                }

                //  Maybe if it reaches an ENDEXEC?
                if (field_12_exec_flag)
                {
                    break;
                }
            }
        }
        else
        {
            //  or else, execute only one command on this frame
            ScriptThread::PreExecOpCode_5108D0();
        }
    }

    const bool reached_level_end = field_4_next_cmd == 0xFFFF;
    field_12_exec_flag = 0;
    return reached_level_end;
}

MATCH_FUNC(0x511930)
void ScriptThread::InitThread_511930(char_type flags, u16 start_cmd)
{
    field_4_next_cmd = start_cmd;
    field_6_flags = flags;
    field_E_phone_timer = 0;
    field_C_cmd_initialised = 0;
    field_12_exec_flag = 0;
    field_8_cond_result = false;
    field_10_stopped = 0;
}

MATCH_FUNC(0x511960)
ScriptThread* ScriptThread::SpawnThread_511960(u16 start_cmd)
{
    ScriptThread* pNewThread = miss2_0x11C_Pool_6F8064->Allocate_4767A0();
    pNewThread->InitThread_511930(field_6_flags, start_cmd);
    return pNewThread;
}

MATCH_FUNC(0x5119a0)
void ScriptThread::launch_mission_5119A0(SCR_CMD_HEADER* pCmd, const char_type* pMissionName)
{
    u16 cmd_next;
    gScriptManager_6F8060->LoadMissionScript_512400(pMissionName, &cmd_next);
    strcpy(field_14_str, pMissionName);
    ScriptThread::Gosub_50CE50(pCmd, cmd_next);
}

MATCH_FUNC(0x511a00)
ScriptThread::ScriptThread()
{
    mpNext = NULL;
    field_114_gosub_stack = new ScriptGosubStack();
    field_4_next_cmd = 0;
    field_6_flags = 0;
    field_8_cond_result = false;
    field_C_cmd_initialised = 0;
    field_E_phone_timer = 0;
    field_10_stopped = 0;
    field_12_exec_flag = 0;
    field_11A_thread_id = 0;

    memset(field_14_str, 0, sizeof(field_14_str));

    field_118_in_level_start = false;
}

// https://decomp.me/scratch/gmYBd
MATCH_FUNC(0x511cd0)
void ScriptThread::DisableBasicKfsFromSave_511CD0()
{
    for (u16 i = 0; i < 25; i++)
    {
        if ((1 << i) & gGameSave_6F78C8.field_E4_car_and_script_data.field_4C_started_basic_kf_mask)
        {
            SCR_START_BASIC_KF_TEMPLATE* pCmd = static_cast<SCR_START_BASIC_KF_TEMPLATE*>(gScriptManager_6F8060->GetBasePointer_512770(
                gScriptManager_6F8060->field_C1E74_basic_kf[i]));
            ScriptThread::DisableThread_505790(pCmd->field_8_triggername);
            ScriptThread::DeallocOrDeleteItem_505B10(pCmd->field_C_objname);
        }
    }
}

MATCH_FUNC(0x512fd0)
ScriptThread::~ScriptThread()
{
    if (field_114_gosub_stack)
    {
        GTA2_DELETE_AND_NULL(field_114_gosub_stack);
    }
    mpNext = NULL;
}

MATCH_FUNC(0x5131C0)
ScriptThreadPool::~ScriptThreadPool()
{
}
