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

DEFINE_GLOBAL_ARRAY_INIT(
    u8,
    kDecidePowerupGenTypes_6212F0,
    19,
    0x6212F0,
    0x41u COMMA 0x42u COMMA 0x43u COMMA 0x44u COMMA 0x45u COMMA 0x46u COMMA 0x47u COMMA 0x48u COMMA 0x49u COMMA 0x5Eu COMMA 0x5Fu COMMA 0x60u COMMA 0x61u COMMA 0x62u COMMA 0x63u COMMA 0x64u COMMA 0x65u COMMA 0x66u COMMA 0x67u);

DEFINE_GLOBAL(ScriptThreadPool*, miss2_0x11C_Pool_6F8064, 0x6F8064);
// TODO: the current command, of any SCR_* type, typed SCR_POINTER only because many commands read
// mIndex; most uses cast it to their command struct
DEFINE_GLOBAL(SCR_POINTER*, gBasePtr_6F8070, 0x6F8070);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneAndHalf, Fix16(0x6000, 0), 0x6F76DC);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6F75F0, Fix16(0x2000, 0), 0x6F75F0);
DEFINE_GLOBAL(Fix16, kFpZero_6F77C0, 0x6F77C0);
DEFINE_GLOBAL_INIT(Ang16, kDegreesToAng16_6F8044, Ang16(0x4), 0x6F8044);
DEFINE_STATIC_MEMBER_INIT(Fix16, ScriptThread, kFp255, Fix16(255), 0x6F7570);
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
    if (gScriptManager_6F8060->mDeathArrState && gScriptManager_6F8060->mMissionFlag &&
        *gScriptManager_6F8060->mMissionFlag == 1)
    {
        if (!gGame_0x40_67E008->mLocalPlayer->IsWastedOrBustedNotObjective54_476700() &&
            !gGame_0x40_67E008->mLocalPlayer->IsBustedObjective54_476730())
        {
            return;
        }

        mGosubStack->remove_5031E0(2);
        SCRCMD_RETURN_506B80();

        s32 msg_offset;
        if (gGame_0x40_67E008->mLocalPlayer->IsWastedOrBustedNotObjective54_476700())
        {
            msg_offset = 0;
        }
        else
        {
            msg_offset = 5;
        }

        u16 msg_id = gRng_6F6784.get_uint8_4F7B70(5);

        if (gScriptManager_6F8060->mGang1MissionFlag && *gScriptManager_6F8060->mGang1MissionFlag == 1)
        {
            msg_id += gScriptManager_6F8060->mGang1DeathBase + msg_offset;
        }
        else if (gScriptManager_6F8060->mGang2MissionFlag && *gScriptManager_6F8060->mGang2MissionFlag == 1)
        {
            msg_id += gScriptManager_6F8060->mGang2DeathBase + msg_offset;
        }
        else if (gScriptManager_6F8060->mGang3MissionFlag && *gScriptManager_6F8060->mGang3MissionFlag == 1)
        {
            msg_id += gScriptManager_6F8060->mGang3DeathBase + msg_offset;
        }
        else
        {
            msg_id = 8001;
        }

        sprintf(gTmpBuffer_67C598, "%d", msg_id);

        gHud_2B00_706620->mBrief.ClearAllBriefsWithPriority_5D4890(hud_brief_priority::normal_1);
        gHud_2B00_706620->mBrief.ClearAllBriefsWithPriority_5D4890(hud_brief_priority::now_3);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("mfail"), hud_message_priority::important_3);
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::jobfail_23);

        *gScriptManager_6F8060->mMissionFlag = 0;
        gScriptManager_6F8060->mDeathArrestFlag = true;
        gGame_0x40_67E008->mLocalPlayer->mPlayerPed->mGraphicType = ped_graphic_type::character_1;
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
void ScriptThread::PoolAllocate()
{
    LOCAL_STATIC_INIT(s16, sNextThreadId, 1, 0x6212EE);
    mInLevelStart = false;
    mThreadId = sNextThreadId++;
}

MATCH_FUNC(0x5035d0)
void ScriptThread::Log_5035D0()
{
    if (gDo_miss_logging_67D6BC)
    {
        sprintf(gTmpBuffer_67C598, "%d:\t\t%d ", gpRng_67AB34->get_cur_rng_41CFE0(), static_cast<s16>(mNextCmd));
        gMiss2Log_6F7698.Write_Log_4D9650(gTmpBuffer_67C598);
    }
}

MATCH_FUNC(0x503610)
void ScriptThread::StopThread_503610()
{
    mStopped = 1;
}

MATCH_FUNC(0x503620)
void ScriptThread::Next_503620(SCR_CMD_HEADER* pCmd)
{
    LOCAL_STATIC(s32, sPrevCmdIdx, 0x6F806C);
    if (static_cast<u16>(pCmd->mCmdNext) != 0xFFFF)
    { // FF FF (low endian) is the script terminator
        sPrevCmdIdx = mNextCmd;
        mNextCmd = pCmd->mCmdNext;
        mCmdInitialised = 0;
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
        mNextCmd = cmdIdx;
    }
    else
    {
        ScriptThread::EndCmd_503670();
    }
}

MATCH_FUNC(0x503670)
void ScriptThread::EndCmd_503670()
{
    mNextCmd = 0xFFFFu;
}

MATCH_FUNC(0x503680)
void ScriptThread::SCRCMD_OBJ_DECSET_2D_3D_503680(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mPos.x, pCmd->mPos.y);
    }

    if (pCmd->mObjId < 0xC8u || pCmd->mObjId > 0xF4u)
    {
        Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

        pPointer->mObj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(pCmd->mObjId,
                                                                        pCmd->mPos.x,
                                                                        pCmd->mPos.y,
                                                                        pCmd->mPos.z,
                                                                        rotation);
    }
    else
    {
        Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

        pPointer->mObj = gObject_5C_6F8F84->NewPhysicsObjUnlessBlocked_529BC0(pCmd->mObjId,
                                                              pCmd->mPos.x,
                                                              pCmd->mPos.y,
                                                              pCmd->mPos.z,
                                                              rotation);
    }

    if (pPointer->mObj != NULL)
    {
        if (pPointer->mObj->IsModel176To181_475AA0())
        {
            gHud_2B00_706620->mArrows.place_gang_phone_5D1110(pPointer->mObj);
            for (u8 i = 0; i < 0x1Fu; i++)
            {
                if (gScriptManager_6F8060->mPhoneIds[i] == 0)
                {
                    gScriptManager_6F8060->mPhoneIds[i] = pPointer->mCmdThis;
                    break;
                }
            }
        }
    }
}

MATCH_FUNC(0x5038d0)
void ScriptThread::SCRCMD_OBJ_DECSET_5038D0(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mPos.x, pCmd->mPos.y);
    }

    if (pCmd->mObjId < 0xC8u || pCmd->mObjId > 0xF4u)
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->mRot);
        rotation.Normalize_406C20();

        pPointer->mObj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(pCmd->mObjId,
                                                                        pCmd->mPos.x,
                                                                        pCmd->mPos.y,
                                                                        pCmd->mPos.z,
                                                                        rotation);
    }
    else
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->mRot);
        rotation.Normalize_406C20();

        pPointer->mObj = gObject_5C_6F8F84->NewPhysicsObjUnlessBlocked_529BC0(pCmd->mObjId,
                                                              pCmd->mPos.x,
                                                              pCmd->mPos.y,
                                                              pCmd->mPos.z,
                                                              rotation);
    }

    if (pPointer->mObj != NULL)
    {
        Object* pObj;

        if (pPointer->mObj->IsStaticCollisionCheck_475A70())
        {
            pPointer->mObj->SetParam_447E90(pCmd->mValueShopType);
        }
        else if (pObj = pPointer->mObj, pObj->IsPickup_421060())
        {
            pObj->SetParam_447E90(pCmd->mValueShopType);
        }
        else
        {
            if (pPointer->mObj->IsShop_475A60())
            {
                pPointer->mObj->SetParam_447E90(pCmd->mValueShopType);
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
        if (gScriptManager_6F8060->IsLoadedFromSave_475A20())
        {
            Fix16 saved_y = kFpOneAndHalf + gGameSave_6F78C8.mPlayerAndWorldStats.mY.GetFloorValue();
            Fix16 saved_x = kFpHalf_6F75F0 + gGameSave_6F78C8.mPlayerAndWorldStats.mX.GetFloorValue();

            pPed = gPedManager_6787BC->SpawnPedAt(saved_x,
                                                  saved_y,
                                                  gGameSave_6F78C8.mPlayerAndWorldStats.mZ,
                                                  gGameSave_6F78C8.mPlayerAndWorldStats.mPlayerPedRemap,
                                                  kAngZero_6F804C);
        }
        else
        {
            if (pCmd->mPos.z == kFp255) //  kFp255 is 255.0
            {
                //  Calculate the real Z position at (X,Y) based on the map
                pCmd->mPos.z =
                    gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mPos.x, pCmd->mPos.y);
            }

            Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

            pPed = gPedManager_6787BC->SpawnPedAt(pCmd->mPos.x,
                                                  pCmd->mPos.y,
                                                  pCmd->mPos.z,
                                                  static_cast<u8>(pCmd->mRemap),
                                                  rotation);
        }

        if (pPed != NULL)
        {
            pPed->SetPedType_403920(ped_type::player_2);
            if (!gScriptManager_6F8060->IsLoadedFromSave_475A20())
            {
                pPed->set_health_4039A0(100);
            }

            pPlayer->InitPlayerPed_565490(pPed);
            pPed->mGraphicType = ped_graphic_type::character_1;
            pCmd->mPed = pPed;

            Sprite* pSprite = pPed->GetSprite_46DF50();
            pSprite->ResolveCollisionWithCarPedOrObject_5A2A30();
        }
    }
}

MATCH_FUNC(0x503bc0)
void ScriptThread::SCRCMD_CAR_DECSET_503BC0(SCR_CAR_DATA_DEC* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255) //  255.0
    {
        pCmd->mPos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mPos.x, pCmd->mPos.y);
    }
    if (pCmd->mTrailerId == -1) //  No trailers
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->mRot);
        rotation.Normalize_406C20();
        pPointer->mCar = gCarManager_6C_677930->SpawnCar_426E10(pCmd->mPos.x,
                                                                pCmd->mPos.y,
                                                                pCmd->mPos.z,
                                                                rotation,
                                                                pCmd->mCarId);
    }
    else if (pCmd->mTrailerId == -2) //  Mini car
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->mRot);
        rotation.Normalize_406C20();
        pPointer->mCar = gCarManager_6C_677930->SpawnCar_4764A0(pCmd->mPos.x,
                                                                pCmd->mPos.y,
                                                                pCmd->mPos.z,
                                                                rotation,
                                                                pCmd->mCarId);
        if (pPointer->mCar != NULL)
        {
            pPointer->mCar->LockPermanently_475C30();
            pPointer->mCar->SetMiniCar_476270();
            pPointer->mCar->UpdateDrawLayer_4435F0();
        }
    }
    else //  There is a trailer
    {
        if (pCmd->mCarId == car_model_enum::TRUKTRNS) // 66 = TRUKTRNS Truck Trailer, Flatbed
        {
            Car* pTrailerCar = gCarManager_6C_677930->SpawnCar_shortened(pCmd->mTrailerId);
            Ang16 rotation;
            rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->mRot);
            rotation.Normalize();

            pPointer->mCar = gCarManager_6C_677930->SpawnCar_426E10(pCmd->mPos.x,
                                                                    pCmd->mPos.y,
                                                                    pCmd->mPos.z,
                                                                    rotation,
                                                                    pCmd->mCarId);
            pPointer->mCar->mCarSprite->AttachSprite_5A3100(pTrailerCar->mCarSprite,
                                                                                      kFpZero_6F77C0,
                                                                                      kFpZero_6F77C0,
                                                                                      kAng180_6F771E);
            pTrailerCar->IncrementCarStats_443D70(car_kind::mission_car_8);
        }
        else
        {
            Ang16 rotation;
            rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, &pCmd->mRot);
            rotation.Normalize();

            Trailer* pTrailer = gCarManager_6C_677930->SpawnCabAndTrailer_446530(pCmd->mPos.x,
                                                                          pCmd->mPos.y,
                                                                          rotation,
                                                                          pCmd->mCarId,
                                                                          pCmd->mTrailerId);
            if (pTrailer != NULL)
            {
                pPointer->mCar = pTrailer->mTruckCab;
            }
            else
            {
                pPointer->mCar = NULL;
            }
        }
    }
    if (pPointer->mCar != NULL)
    {
        if (pCmd->mRemap != -1)
        {
            pPointer->mCar->SetCarRemap(static_cast<u8>(pCmd->mRemap));
        }

        Car* pCar = pPointer->mCar;
        pCar->SetDriverPedType_421560(ped_type::script_created_5);

        pPointer->mCar->LockoutThiefUnlessPermanent_475C40();
        pPointer->mCar->IncrementCarStats_443D70(car_kind::mission_car_8);
        pPointer->mCar->mCarSprite->ResolveCollisionWithCarPedOrObject_5A2A30();

        if (pCmd->mType >= static_cast<u32>(SCRCMD_CREATE_GANG_CAR1) && pCmd->mType <= static_cast<u32>(SCRCMD_CREATE_GANG_CAR4))
        {
            s8 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pPointer->mCar->mCarInfoIdx);
            if (gang_idx > -1)
            {
                Gang* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(gang_idx);
                pPointer->mCar->AttachGangIcon_440660(pGang->mArrowColour);
            }
        }
    }
}

MATCH_FUNC(0x503f80)
void ScriptThread::SCRCMD_PARKED_CAR_DECSET_503F80(SCR_POINTER* pCmd)
{
    ScriptThread::SCRCMD_CAR_DECSET_503BC0(reinterpret_cast<SCR_CAR_DATA_DEC*>(pCmd), pCmd);
    (pCmd->mCar)->ReassignCarStats_443EB0(car_kind::parked_car_9);
    Car* pCar = pCmd->mCar;
    pCar->SetDriverPedType_421560(ped_type::special_ped_4);
}

MATCH_FUNC(0x503fb0)
void ScriptThread::SCRCMD_CHAR_DECSET_2D_3D_503FB0(SCR_CHAR_DATA_DEC* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mPos.x, pCmd->mPos.y);
    }

    Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

    Ped* pPed = gPedManager_6787BC->SpawnPedAt(pCmd->mPos.x,
                                          pCmd->mPos.y,
                                          pCmd->mPos.z,
                                          static_cast<u8>(pCmd->mRemap),
                                          rotation);
    pPointer->mChar = pPed;

    if (pPed)
    {
        pPointer->mChar->SetPedType_403920(ped_type::script_created_5);
        pPointer->mChar->set_occupation_403970(pCmd->mOccupation);
        pPointer->mChar->mGraphicType = ped_graphic_type::character_1;
        pPointer->mChar->SetObjective(objectives_enum::wait_on_foot_26);
        pPointer->mChar->set_health_4039A0(100);
        Sprite* pSprite = pPointer->mChar->GetSprite_46DF50();
        pSprite->ResolveCollisionWithCarPedOrObject_5A2A30();
    }
}

MATCH_FUNC(0x504110)
void ScriptThread::SetCharObjective_504110(SCR_CHAR_OBJECTIVE* pCmd, SCR_POINTER* pPointer)
{
    pPointer->mChar->SetObjective(pCmd->mObjective);
    pPointer->mChar->mBf21C.bScheduledForRemoval = false;
}

MATCH_FUNC(0x504150)
void ScriptThread::SCRCMD_MAP_ZONE_SET_504150(SCR_MAP_ZONE_SET* pCmd)
{
    str_table_entry* pZoneStr = gScriptManager_6F8060->FindStringById_503080(pCmd->mCmdThis);
    gMap_0x370_6F6268->SetZoneInfo_4DF8C0(pZoneStr->mZoneIdx,
                                                   pCmd->mCarDensity,
                                                   pCmd->mGoodcarRatio,
                                                   pCmd->mBadcarRatio,
                                                   pCmd->mPolicecarRatio,
                                                   pCmd->mGangcarRatio,
                                                   pCmd->mPedDensity,
                                                   pCmd->mMuggerRatio,
                                                   pCmd->mCarthiefRatio,
                                                   pCmd->mElvisRatio,
                                                   pCmd->mGangcharRatio,
                                                   pCmd->mPolicepedRatio);
}

MATCH_FUNC(0x5041b0)
void ScriptThread::SCRCMD_ARROW_DEC_5041B0(SCR_TWO_PARAMS* pCmd)
{
    pCmd->mU32 = 0;
}

MATCH_FUNC(0x5041c0)
void ScriptThread::SCRCMD_CRANE_5041C0(SCR_CRANE_TARGET_DEC* pTargetCmd, SCR_CRANE_BASIC_DEC* pBasicCmd)
{
    pBasicCmd->mCrane =
        gCranePool_D9C_679FD4->NewCrane_480EC0(pTargetCmd->mPos.mX, pTargetCmd->mPos.mY);

    if (pTargetCmd->mType == SCRCMD_CRANE_TARGET_DEC)
    {
        if (pTargetCmd->mHomeCranetype == crane_home_target::first_0)
        {
            Fix16 fix_rot = Fix16(kDegreesToAng16_6F8044.rValue) * Fix16(pTargetCmd->mTargetRotation);
            Ang16 rotation(&fix_rot, 0);

            pTargetCmd->mCrane->SetTarget1_480900(pTargetCmd->mTargetPos.mX,
                                                                     pTargetCmd->mTargetPos.mY,
                                                                     rotation);
        }
        else if (pTargetCmd->mHomeCranetype == crane_home_target::second_1)
        {
            Fix16 fix_rot = Fix16(kDegreesToAng16_6F8044.rValue) * Fix16(pTargetCmd->mTargetRotation);
            Ang16 rotation(&fix_rot, 0);

            pTargetCmd->mCrane->SetTarget2_480B60(pTargetCmd->mTargetPos.mX,
                                                                     pTargetCmd->mTargetPos.mY,
                                                                     rotation);
        }
    }
    else if (pTargetCmd->mType == SCRCMD_CRANE_BASIC_DEC)
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, reinterpret_cast<Ang16*>(&pBasicCmd->mTargetRotation));
        rotation.Normalize_406C20();
        pBasicCmd->mCrane->SetTarget1_480900(pBasicCmd->mTargetPos.mX,
                                                                pBasicCmd->mTargetPos.mY,
                                                                rotation);

        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, reinterpret_cast<Ang16*>(&pBasicCmd->mSecondRotation));
        rotation.Normalize_406C20();
        pBasicCmd->mCrane->SetTarget2_480B60(pBasicCmd->mSecondPos.mX,
                                                                pBasicCmd->mSecondPos.mY,
                                                                rotation);
    }

    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16_6F8044, reinterpret_cast<Ang16*>(&pTargetCmd->mHomeRotation));
        rotation.Normalize_406C20();
        pBasicCmd->mCrane->SetHomeRotation_4768E0(rotation);
    }

    if (pTargetCmd->mHomecrane)
    {
        SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pTargetCmd->mHomecrane));
        pBasicCmd->mCrane->SetHomeCrane_476900(pPointer->mCrane);
    }
}

MATCH_FUNC(0x5043a0)
void ScriptThread::SCRCMD_CONVEYOR_DECSET1_2_5043A0(SCR_CONVEYOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mRect.mPos.z == kFp255)
    {
        pCmd->mRect.mPos.z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mRect.mPos.x,
                                                                                                 pCmd->mRect.mPos.y);
    }
    pPointer->mObj = gObject_5C_6F8F84->NewTouchPoint_529950(objects::conveyor_139,
                                                                    pCmd->mRect.mPos.x,
                                                                    pCmd->mRect.mPos.y,
                                                                    pCmd->mRect.mPos.z,
                                                                    kAngZero_6F804C,
                                                                    pCmd->mRect.mSize.mX,
                                                                    pCmd->mRect.mSize.mY,
                                                                    kFpOne_6F77C4);

    pPointer->mObj->SetConveyorSpeed_529030(pCmd->mSpeed.mX, pCmd->mSpeed.mY);
}

MATCH_FUNC(0x504420)
void ScriptThread::SCRCMD_GENERATOR_DECSET_504420(SCR_GENERATOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mPos.x, pCmd->mPos.y);
    }

    Ang16 rotation = kDegreesToAng16_6F8044.MultiplyByFix16_401CB0(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

    pPointer->mGenerator = gGeneratorPool_14AC_67E5D0->CreateGenerator_4C1DC0(pCmd->mPos.x,
                                                                                     pCmd->mPos.y,
                                                                                     pCmd->mPos.z,
                                                                                     rotation,
                                                                                     pCmd->mObjId,
                                                                                     pCmd->mMindelay,
                                                                                     pCmd->mMaxdelay);
    if (pCmd->mAmmo > 0)
    {
        pPointer->mGenerator->mAmmo = pCmd->mAmmo;
    }
}

MATCH_FUNC(0x504530)
void ScriptThread::SCRCMD_DESTRUCTOR_DECSET_504530(SCR_DESTRUCTOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mRect.mPos.z == kFp255)
    {
        pCmd->mRect.mPos.z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mRect.mPos.x,
                                                                                                 pCmd->mRect.mPos.y);
    }

    pPointer->mObj = gObject_5C_6F8F84->NewTouchPoint_529950(objects::destructor_141,
                                                                    pCmd->mRect.mPos.x,
                                                                    pCmd->mRect.mPos.y,
                                                                    pCmd->mRect.mPos.z,
                                                                    kAngZero_6F804C,
                                                                    pCmd->mRect.mSize.mX,
                                                                    pCmd->mRect.mSize.mY,
                                                                    kFpOne_6F77C4);
}

MATCH_FUNC(0x5045a0)
void ScriptThread::SCRCMD_CRUSHER_BASIC_5045A0(SCR_CRUSHER_BASIC* pCmd, SCR_POINTER* pPointer)
{
    pPointer->mCrusher = gCrusherPool_94_67A830->CreateCrusher_488820(pCmd->mPos.mX, pCmd->mPos.mY);
}

MATCH_FUNC(0x5045d0)
void ScriptThread::SCRCMD_THREAD_DECLARE2_5045D0(SCR_THREAD* pCmd, s16* pThreadIdx)
{
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCharVarIdx));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCarOrPhoneIdx));

    if (pCharPointer->mChar && pCarPointer->mCar)
    {
        gScriptManager_6F8060->AddCarThread_5128D0(pCharPointer->mChar->get_id(), pCarPointer->mCar->mCarId, *pThreadIdx);
        pCarPointer->mCar->SetCarThreadPending_476360();
    }
}

MATCH_FUNC(0x504660)
void ScriptThread::SCRCMD_THREAD_DECLARE3_504660(SCR_THREAD* pCmd)
{
    if (static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCharIdx10))->mChar)
    {
        pCmd->mTriggerIdx = gCarThreadTriggers_214_705F20->AddThreadTrigger_5C86C0(car_thread_trigger_type::char_on_foot_4,
                                                     car_thread_trigger_action::spawn_thread_2,
                                                     pCmd,
                                                     Fix16(pCmd->mX) + kFpHalf_6F75F0,
                                                     Fix16(pCmd->mY) + kFpHalf_6F75F0,
                                                     pCmd->mZ,
                                                     kFpHalf_6F75F0,
                                                     kFpHalf_6F75F0);
        pCmd->mTriggerAdded = 1;
    }
}

MATCH_FUNC(0x504710)
void ScriptThread::SCRCMD_THREAD_DECLARE5_504710(SCR_CHAR_AREA_ANY* pCmd)
{
    if (static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCharIdx24))->mChar)
    {
        pCmd->mTriggerAdded = 1;

        switch (pCmd->mType)
        {
            case SCRCMD_THREAD_DECLARE5:
                pCmd->mTriggerIdx = gCarThreadTriggers_214_705F20->AddThreadTrigger_5C86C0(car_thread_trigger_type::char_on_foot_4,
                                                                    car_thread_trigger_action::spawn_thread_2,
                                                                    reinterpret_cast<SCR_THREAD*>(pCmd),
                                                                    pCmd->mX,
                                                                    pCmd->mY,
                                                                    pCmd->mZ,
                                                                    pCmd->mW,
                                                                    pCmd->mH);

                break;
            case SCRCMD_CHAR_AREA_ANY_MEANS:
                pCmd->mTriggerIdx = gCarThreadTriggers_214_705F20->AddThreadTrigger_5C86C0(car_thread_trigger_type::char_any_means_3,
                                                                    car_thread_trigger_action::spawn_thread_2,
                                                                    reinterpret_cast<SCR_THREAD*>(pCmd),
                                                                    pCmd->mX,
                                                                    pCmd->mY,
                                                                    pCmd->mZ,
                                                                    pCmd->mW,
                                                                    pCmd->mH);

                break;
        }
    }
}

MATCH_FUNC(0x5047c0)
void ScriptThread::SCRCMD_THREAD_DECLARE4_5047C0(SCR_THREAD* pCmd, s16* pThreadIdx)
{
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCharVarIdx));
    SCR_POINTER* pPhonePointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCarOrPhoneIdx));

    if (pCharPointer->mChar && pPhonePointer->mCar)
    {
        pPhonePointer->mObj->PoolGiveAndMarkDone_5291D0();
        gScriptManager_6F8060->AddPhoneThread_5129B0(pCharPointer->mChar->get_id(), pPhonePointer->mObj->mId, *pThreadIdx);
    }
}

MATCH_FUNC(0x504830)
void ScriptThread::SCRCMD_SET_GANG_INFO1_504830(SCR_SET_GANG_INFO* pCmd)
{
    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(pCmd->mGangname);

    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pGangStr->get_name());
    pGang->SetRemap_4758A0(pCmd->mRemap);

    pGang->SetBasicWeapon_4758B0(pCmd->mWeapon1);
    pGang->SetAngryWeapon_4758C0(pCmd->mWeapon2);
    pGang->SetHateWeapon_4758D0(pCmd->mWeapon3);

    pGang->SetArrowColour_475940(pCmd->mArrowId);
    pGang->SetGangCarModel_4758E0(pCmd->mCarModel);
    pGang->SetGangCarRemap_4758F0(static_cast<char_type>(pCmd->mCarRemap));

    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mPos.x, pCmd->mPos.y);
    }

    pGang->SetInfoPhonePos_475910(pCmd->mPos.x, pCmd->mPos.y, pCmd->mPos.z);

    pGang->SetKillRespectChange_475950(pCmd->mKillRespectChange);
    gGangPool_CA8_67E274->SwapGangSlots_4BF230(pGang, gScriptManager_6F8060->mNextGangIdx);
    ++gScriptManager_6F8060->mNextGangIdx;
    Gang* pMovedGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pGangStr->get_name());

    if (pCmd->mKillRespectChange > 0)
    {
        gHud_2B00_706620->mArrows.SetNewGangArrow_5D1310(pMovedGang);
    }
}

MATCH_FUNC(0x504950)
void ScriptThread::SCRCMD_SET_DOOR_INFO_504950(SCR_FOUR_PARAMS* pCmd)
{
    gDoor_4D4_67BD2C->RegisterDoorInfo_49D2D0(pCmd->mUnsigned1, pCmd->mUnsigned2, static_cast<char_type>(pCmd->mUnsigned3));
}

MATCH_FUNC(0x504970)
void ScriptThread::SCRCMD_DOOR_DECLARE_D1_S1_504970(SCR_DOOR_DATA_DEC* pCmd)
{
    if (pCmd->mType == SCRCMD_DOOR_DECLARE_S1)
    {
        if (pCmd->mCheck.mSize.mX == kFpZero_6F77C0 && pCmd->mCheck.mSize.mY == kFpZero_6F77C0)
        {
            pCmd->mDoor = gDoor_4D4_67BD2C->RegisterSingleDoorNoCheck_49CF50(pCmd->mGrId,
                                                                               pCmd->mBlock.mX,
                                                                               pCmd->mBlock.mY,
                                                                               pCmd->mBlock.mZ,
                                                                               pCmd->mFace,
                                                                               pCmd->mFlip,
                                                                               pCmd->mReversed);
        }
        else
        {
            pCmd->mDoor = gDoor_4D4_67BD2C->RegisterSingleDoor_49D170(pCmd->mGrId,
                                                                        pCmd->mBlock.mX,
                                                                        pCmd->mBlock.mY,
                                                                        pCmd->mBlock.mZ,
                                                                        pCmd->mFace,
                                                                        pCmd->mCheck.mPos.x,
                                                                        pCmd->mCheck.mPos.y,
                                                                        pCmd->mCheck.mPos.z,
                                                                        pCmd->mCheck.mSize.mX,
                                                                        pCmd->mCheck.mSize.mY,
                                                                        pCmd->mFlip,
                                                                        pCmd->mReversed);
        }
    }
    else
    {
        if (pCmd->mCheck.mSize.mX == kFpZero_6F77C0 && pCmd->mCheck.mSize.mY == kFpZero_6F77C0)
        {
            pCmd->mDoor = gDoor_4D4_67BD2C->RegisterDoubleDoorNoCheck_49CFA0(pCmd->mGrId,
                                                                               pCmd->mBlock.mX,
                                                                               pCmd->mBlock.mY,
                                                                               pCmd->mBlock.mZ,
                                                                               pCmd->mFace,
                                                                               pCmd->mFlip,
                                                                               pCmd->mReversed);
        }
        else
        {
            pCmd->mDoor = gDoor_4D4_67BD2C->RegisterDoubleDoor_49D1F0(pCmd->mGrId,
                                                                        pCmd->mBlock.mX,
                                                                        pCmd->mBlock.mY,
                                                                        pCmd->mBlock.mZ,
                                                                        pCmd->mFace,
                                                                        pCmd->mCheck.mPos.x,
                                                                        pCmd->mCheck.mPos.y,
                                                                        pCmd->mCheck.mPos.z,
                                                                        pCmd->mCheck.mSize.mX,
                                                                        pCmd->mCheck.mSize.mY,
                                                                        pCmd->mFlip,
                                                                        pCmd->mReversed);
        }
    }
    switch (pCmd->mOpenType)
    {
        case SCR_DOOR_OPENTYPES::any_player:
            pCmd->mDoor->SetOpenType_476990(door_open_type::any_player);
            break;
        case SCR_DOOR_OPENTYPES::any_car:
            pCmd->mDoor->SetOpenType_476990(door_open_type::any_car);
            break;
        case SCR_DOOR_OPENTYPES::none:
            pCmd->mDoor->SetOpenType_476990(door_open_type::none);
            break;
        case SCR_DOOR_OPENTYPES::one_car:
            pCmd->mDoor->SetOpenType_476990(door_open_type::one_car);
            break;
        case SCR_DOOR_OPENTYPES::one_char_on_foot:
            pCmd->mDoor->SetOpenType_476990(door_open_type::one_char_on_foot);
            break;
        case SCR_DOOR_OPENTYPES::any_player_one_car:
            pCmd->mDoor->SetOpenType_476990(door_open_type::any_player_one_car);
            break;
    }

    Door* pDoor;
    u16 close_delay;
    switch (pCmd->mCloseType)
    {
        case SCR_DOOR_CLOSETYPES::close_never:
            pCmd->mDoor->set_close_type(door_close_type::close_never);
            pDoor = pCmd->mDoor;
            close_delay = pCmd->mDelay;
            pDoor->mCloseDelay = close_delay;
            pDoor->mCloseTimer = close_delay;
            return;
        case SCR_DOOR_CLOSETYPES::close_time_delay:
            pCmd->mDoor->set_close_type(door_close_type::close_time_delay);
            pDoor = pCmd->mDoor;
            close_delay = pCmd->mDelay;
            pDoor->mCloseDelay = close_delay;
            pDoor->mCloseTimer = close_delay;
            return;
        case SCR_DOOR_CLOSETYPES::close_when_clear:
            pCmd->mDoor->set_close_type(door_close_type::close_when_clear);
            pDoor = pCmd->mDoor;
            close_delay = pCmd->mDelay;
            pDoor->mCloseDelay = close_delay;
            pDoor->mCloseTimer = close_delay;
            return;
        case SCR_DOOR_CLOSETYPES::close_when_open_rule_fails:
            pCmd->mDoor->set_close_type(door_close_type::close_when_open_rule_fails);
            pDoor = pCmd->mDoor;
            close_delay = pCmd->mDelay;
            pDoor->mCloseDelay = close_delay;
            pDoor->mCloseTimer = close_delay;
            return;
    }
    pDoor = pCmd->mDoor;
    close_delay = pCmd->mDelay;
    pDoor->mCloseDelay = close_delay;
    pDoor->mCloseTimer = close_delay;
    // 9.6f: Door::set_close_delay (0x476A20, inlined, using it changes the code)
}

MATCH_FUNC(0x504b80)
void ScriptThread::SCRCMD_DOOR_DECLARE_D2_S2_504B80(SCR_DOOR_DATA_DEC* pCmd)
{
    if (pCmd->mType == SCRCMD_DOOR_DECLARE_S2)
    {
        if (pCmd->mCheck.mSize.mX == kFpZero_6F77C0 && pCmd->mCheck.mSize.mY == kFpZero_6F77C0)
        {
            pCmd->mDoor = gDoor_4D4_67BD2C->RegisterSingleDoorNoCheck_49CF50(pCmd->mGrId,
                                                                               pCmd->mBlock.mX,
                                                                               pCmd->mBlock.mY,
                                                                               pCmd->mBlock.mZ,
                                                                               pCmd->mFace,
                                                                               pCmd->mFlip,
                                                                               pCmd->mReversed);
        }
        else
        {
            pCmd->mDoor = gDoor_4D4_67BD2C->RegisterSingleDoor_49D170(pCmd->mGrId,
                                                                        pCmd->mBlock.mX,
                                                                        pCmd->mBlock.mY,
                                                                        pCmd->mBlock.mZ,
                                                                        pCmd->mFace,
                                                                        pCmd->mCheck.mPos.x,
                                                                        pCmd->mCheck.mPos.y,
                                                                        pCmd->mCheck.mPos.z,
                                                                        pCmd->mCheck.mSize.mX,
                                                                        pCmd->mCheck.mSize.mY,
                                                                        pCmd->mFlip,
                                                                        pCmd->mReversed);
        }
    }
    else
    {
        if (pCmd->mCheck.mSize.mX == kFpZero_6F77C0 && pCmd->mCheck.mSize.mY == kFpZero_6F77C0)
        {
            pCmd->mDoor = gDoor_4D4_67BD2C->RegisterDoubleDoorNoCheck_49CFA0(pCmd->mGrId,
                                                                               pCmd->mBlock.mX,
                                                                               pCmd->mBlock.mY,
                                                                               pCmd->mBlock.mZ,
                                                                               pCmd->mFace,
                                                                               pCmd->mFlip,
                                                                               pCmd->mReversed);
        }
        else
        {
            pCmd->mDoor = gDoor_4D4_67BD2C->RegisterDoubleDoor_49D1F0(pCmd->mGrId,
                                                                        pCmd->mBlock.mX,
                                                                        pCmd->mBlock.mY,
                                                                        pCmd->mBlock.mZ,
                                                                        pCmd->mFace,
                                                                        pCmd->mCheck.mPos.x,
                                                                        pCmd->mCheck.mPos.y,
                                                                        pCmd->mCheck.mPos.z,
                                                                        pCmd->mCheck.mSize.mX,
                                                                        pCmd->mCheck.mSize.mY,
                                                                        pCmd->mFlip,
                                                                        pCmd->mReversed);
        }
    }

    switch (pCmd->mOpenType)
    {
        case SCR_DOOR_OPENTYPES::any_player:
            pCmd->mDoor->SetOpenType_476990(door_open_type::any_player);
            break;
        case SCR_DOOR_OPENTYPES::any_car:
            pCmd->mDoor->SetOpenType_476990(door_open_type::any_car);
            break;
        case SCR_DOOR_OPENTYPES::one_car:
        case SCR_DOOR_OPENTYPES::any_player_one_car:
        {
            SCR_POINTER* pTargetPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mOpenTarget));
            Car* pCar = pTargetPointer->mCar;
            if (pCar != NULL)
            {
                pCmd->mDoor->set_open_details_car_bc(door_open_type::one_car, pCar);
                pCmd->mDoor->set_target_id(pTargetPointer->mCar->mCarId);
            }
            break;
        }

        case SCR_DOOR_OPENTYPES::one_model:
            pCmd->mDoor->set_open_details_model_id(door_open_type::one_model, pCmd->mOpenTarget);
            break;

        case SCR_DOOR_OPENTYPES::one_char_on_foot:
        {
            SCR_POINTER* pTargetPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mOpenTarget));
            Ped* pPed = pTargetPointer->mChar;
            if (pPed != NULL)
            {
                pCmd->mDoor->set_open_details_ped(door_open_type::one_char_on_foot, pPed);
                pCmd->mDoor->set_target_id(pTargetPointer->mChar->mId);
            }
        }
        break;
        case SCR_DOOR_OPENTYPES::none:
            pCmd->mDoor->SetOpenType_476990(door_open_type::none);
            break;
    }

    switch (pCmd->mCloseType)
    {
        case SCR_DOOR_CLOSETYPES::close_never:
            pCmd->mDoor->set_close_type(door_close_type::close_never);
            break;
        case SCR_DOOR_CLOSETYPES::close_time_delay:
            pCmd->mDoor->set_close_type(door_close_type::close_time_delay);
            break;
        case SCR_DOOR_CLOSETYPES::close_when_clear:
            pCmd->mDoor->set_close_type(door_close_type::close_when_clear);
            break;
        case SCR_DOOR_CLOSETYPES::close_when_open_rule_fails:
            pCmd->mDoor->set_close_type(door_close_type::close_when_open_rule_fails);
            break;
    }
    Door* pDoor = pCmd->mDoor;
    u16 close_delay = pCmd->mDelay;
    pDoor->mCloseDelay = close_delay;
    pDoor->mCloseTimer = close_delay;
    // 9.6f: Door::set_close_delay (0x476A20, inlined, using it changes the code)
}

MATCH_FUNC(0x504dd0)
void ScriptThread::SCRCMD_DECLARE_MISSION_504DD0(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned1); //  player pointer not used because missions are singleplayer
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));
    if (pCmd->mType == SCRCMD_DEC_GANG_1_FLAG)
    {
        gScriptManager_6F8060->mGang1MissionFlag = &pPointer->mCounter;
    }
    else if (pCmd->mType == SCRCMD_DEC_GANG_2_FLAG)
    {
        gScriptManager_6F8060->mGang2MissionFlag = &pPointer->mCounter;
    }
    else if (pCmd->mType == SCRCMD_DEC_GANG_3_FLAG)
    {
        gScriptManager_6F8060->mGang3MissionFlag = &pPointer->mCounter;
    }
    else // pCmd->mType == SCRCMD_DECLARE_MISSION
    {
        gScriptManager_6F8060->mMissionFlag = &pPointer->mCounter;
    }
}

MATCH_FUNC(0x504ee0)
void ScriptThread::CreateLight_504EE0(SCR_CREATE_LIGHT* pCmd, SCR_POINTER* pPointer)
{
    Light* pNewLight = gLight_1D4CC_6F5520->Init_469010(pCmd->mXpos,
                                                                       pCmd->mYpos,
                                                                       pCmd->mZpos,
                                                                       pCmd->mArgb,
                                                                       pCmd->mRadius,
                                                                       pCmd->mIntensity);
    pPointer->mLight = pNewLight;
    if (pCmd->mOnTime > 0)
    {
        gLight_1D4CC_6F5520->SetFlashing_469070(pNewLight, pCmd->mOnTime, pCmd->mOffTime, pCmd->mShape);
    }
}

MATCH_FUNC(0x505030)
void ScriptThread::SCRCMD_SET_STATION_EMPTY_STATION_505030(SCR_SET_STATION* pCmd)
{
    if (!bSkip_trains_67D550)
    {
        str_table_entry* pStationStr = gScriptManager_6F8060->FindStringById_503080(pCmd->mPlatform);
        gmp_map_zone* station_zone = gMap_0x370_6F6268->FindZoneByName_4DEFD0(pStationStr->get_name());
        TrainStation* pStation = gPublicTransport_181C_6FF1D4->TrainStationForZone_57B4B0(station_zone);

        for (u8 wagon_idx = 0; wagon_idx < 10; wagon_idx++)
        {
            if (wagon_idx < pCmd->mNumPassenger)
            {
                pStation->mTrainWagons[wagon_idx] = 1; // passenger wagon
            }
            else
            {
                const s32 num_freight = pCmd->mNumFreight;
                const s32 num_passenger = pCmd->mNumPassenger;

                if (wagon_idx < num_passenger + num_freight)
                {
                    pStation->mTrainWagons[wagon_idx] = 2; // freight wagon
                }
                else
                {
                    if (wagon_idx < num_passenger + num_freight + pCmd->mNumBoxcar)
                    {
                        pStation->mTrainWagons[wagon_idx] = 3; // boxcar wagon
                    }
                }
            }
        }
        pStation->mWagonsNumber = pCmd->mNumPassenger + pCmd->mNumFreight + pCmd->mNumBoxcar;
    }
}

MATCH_FUNC(0x5051d0)
void ScriptThread::SCRCMD_RADIOSTATION_DEC_5051D0(SCR_RADIOSTATION_DEC* pCmd)
{
    gRoot_sound_66B038.DeclareRadioStation_40F030(pCmd->mStationType, pCmd->mPos.mX, pCmd->mPos.mY);
}

MATCH_FUNC(0x505210)
void ScriptThread::SCRCMD_SET_TRAIN_STATIONS_505210(SCR_TWO_PARAMS* pCmd)
{
    if (!bSkip_trains_67D550)
    {
        str_table_entry* pStationStr = gScriptManager_6F8060->FindStringById_503080(pCmd->mUnsigned1);
        const char_type* station_zone_name = pStationStr->get_name();
        gmp_map_zone* station_zone = gMap_0x370_6F6268->FindZoneByName_4DEFD0(station_zone_name);
        TrainStation* pTrainStation = gPublicTransport_181C_6FF1D4->TrainStationForZone_57B4B0(station_zone);
        pTrainStation->CalculateWagonCount_578820(reinterpret_cast<u8*>(&pCmd->mUnsigned2)); //  The parameter may be the train wagon array, not sure
    }
}

MATCH_FUNC(0x5052c0)
void ScriptThread::SCRCMD_DECLARE_POLICE_5052C0(SCR_DECLARE_POLICELEVEL* pCmd)
{
    u8 max_wanted_level = static_cast<u8>(pCmd->mWantedLevel);
    if (max_wanted_level == 0)
    {
        bSkip_police_67D4F9 = false;
    }
    gPolice_7B8_6FEE40->mMaxWantedStars = static_cast<u8>(pCmd->mWantedLevel); // max_wanted_level
}

MATCH_FUNC(0x505340)
void ScriptThread::SCRCMD_SOUND_DECSET_505340(SCR_SOUND_DECSET* pCmd, SCR_POINTER* pPointer)
{
    if (!pCmd->mPlayType)
    {
        pPointer->mObj = gObject_5C_6F8F84->NewWithPedRefIdx_5299F0(objects::sound_object_type_1_278,
                                                              pCmd->mSoundId,
                                                              pCmd->mPos.x,
                                                              pCmd->mPos.y,
                                                              pCmd->mPos.z);
    }
    else
    {
        pPointer->mObj = gObject_5C_6F8F84->NewWithPedRefIdx_5299F0(objects::sound_object_type_2_279,
                                                              pCmd->mSoundId,
                                                              pCmd->mPos.x,
                                                              pCmd->mPos.y,
                                                              pCmd->mPos.z);
    }
}

MATCH_FUNC(0x505430)
void ScriptThread::SCRCMD_SET_MISSIONS_TOTAL_505430(SCR_TWO_PARAMS* pCmd)
{
    switch (pCmd->mType)
    {
        case SCRCMD_TOTAL_MISSIONS:
            gScriptManager_6F8060->mTotalMissions = pCmd->mSigned2;
            break;
        case SCRCMD_TOTAL_SECRETS:
            gScriptManager_6F8060->mTotalSecrets = pCmd->mSigned2;
            break;
        case SCRCMD_GANG_1_MISSION_TOTAL:
            gScriptManager_6F8060->mGang1MissionsTotal = pCmd->mSigned2;
            break;
        case SCRCMD_GANG_2_MISSION_TOTAL:
            gScriptManager_6F8060->mGang2MissionsTotal = pCmd->mSigned2;
            break;
        case SCRCMD_GANG_3_MISSION_TOTAL:
            gScriptManager_6F8060->mGang3MissionsTotal = pCmd->mSigned2;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505580)
void ScriptThread::SCRCMD_PASSED_FAILED_FLAGS_505580(SCR_TWO_PARAMS* pCmd)
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned1));

    switch (pCmd->mType)
    {
        case SCRCMD_PASSED_FLAG:
            gScriptManager_6F8060->mPassedFlag = &pPointer->mCounter;
            break;
        case SCRCMD_1_PASSED_FLAG:
            gScriptManager_6F8060->m1PassedFlag = &pPointer->mCounter;
            break;
        case SCRCMD_2_PASSED_FLAG:
            gScriptManager_6F8060->m2PassedFlag = &pPointer->mCounter;
            break;
        case SCRCMD_3_PASSED_FLAG:
            gScriptManager_6F8060->m3PassedFlag = &pPointer->mCounter;
            break;
        case SCRCMD_SECRETS_PASSED:
            gScriptManager_6F8060->mSecretsPassed = &pPointer->mCounter;
            break;
        case SCRCMD_SECRETS_FAILED:
            gScriptManager_6F8060->mSecretsFailed = &pPointer->mCounter;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505710)
void ScriptThread::SCRCMD_FINISH_SCORE_505710(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager_6F8060->mFinishScore = pCmd->mS32;
}

MATCH_FUNC(0x505750)
void ScriptThread::SCRCMD_DECLARE_CARLIST_505750(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager_6F8060->mCarList = &pCmd->mU32;
}

MATCH_FUNC(0x505790)
void ScriptThread::DisableThread_505790(u16 idx)
{
    SCR_CMD_HEADER* pCmd = gScriptManager_6F8060->GetBasePointer_512770(idx);

    switch (pCmd->mType)
    {
        case SCRCMD_THREAD_DECLARE4:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);
            SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pThread->mCharVarIdx));
            SCR_POINTER* pPhonePointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pThread->mCarOrPhoneIdx));

            if (pPhonePointer->mObj->IsAnimatedBehavior_475A80() && pPhonePointer->mObj->mTouchedResult != objects::dead_phone_174)
            {
                pPhonePointer->mObj->PoolGiveAndMarkDone_5291D0();
            }

            gScriptManager_6F8060->RemovePhoneThread_512A70(pCharPointer->mChar->get_id(), pPhonePointer->mObj->mId);
            break;
        }
        case SCRCMD_THREAD_DECLARE3:
            gCarThreadTriggers_214_705F20->SetTriggerEnableState_4768C0(static_cast<SCR_THREAD*>(pCmd)->mTriggerIdx, car_thread_trigger_state::disabled_2);
            break;
        case SCRCMD_THREAD_DECLARE5:
        case SCRCMD_CHAR_AREA_ANY_MEANS:
            gCarThreadTriggers_214_705F20->SetTriggerEnableState_4768C0(static_cast<SCR_CHAR_AREA_ANY*>(pCmd)->mTriggerIdx, car_thread_trigger_state::disabled_2);
            break;
        case SCRCMD_THREAD_DECLARE2:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);
            SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pThread->mCharVarIdx));
            SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pThread->mCarOrPhoneIdx));

            s32 char_id = pCharPointer->mChar->get_id();
            ThreadTrigger* pTrigger = gScriptManager_6F8060->FindCarThread_5128A0(char_id, pCarPointer->mCar->mCarId);

            if (pTrigger)
            {
                pTrigger->mPedId = 0;
                pTrigger->mTargetId = 0;
                pTrigger->mCmdLine = 0;
                --gScriptManager_6F8060->mCarTriggerCount;
            }

            pCarPointer->mCar->ClearCarThreadPending_4218E0();
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
    switch (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->mType))
    {
        case script_entity_type::car_2:
            pCarCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pCarCmdPointer->mCar)
            {
                gScriptManager_6F8060->ClearWeaponHitFlag_512BA0(pCarCmdPointer->mCar->mCarId, false);
                pCarCmdPointer->mCar->MarkForDespawn_421470();
                pCarCmdPointer->mCar = NULL;
            }
            break;

        case script_entity_type::char_1:
            pPedCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pPedCmdPointer->mChar)
            {
                pPedCmdPointer->mChar->Deallocate_45EB60();
                pPedCmdPointer->mChar = NULL;
            }
            break;

        case script_entity_type::object_3:
            pObjCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pObjCmdPointer->mObj)
            {
                pObjCmdPointer->mObj->Dealloc_5291B0();
                pObjCmdPointer->mObj = NULL;
            }
            break;
        case script_entity_type::light_8:
            pLightCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pLightCmdPointer->mLight)
            {
                gLight_1D4CC_6F5520->DeallocLight_47F4F0(pLightCmdPointer->mLight);
                pLightCmdPointer->mLight = NULL;
            }
            break;

        case script_entity_type::radio_station_9:
            pSoundCmdPointer = static_cast<SCR_DELETE_SOUND*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            gRoot_sound_66B038.RemoveSound_40F050(pSoundCmdPointer->mXpos, pSoundCmdPointer->mYpos);
            break;

        case script_entity_type::sound_10:
            pObjCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(idx));
            if (pObjCmdPointer->mObj)
            {
                pObjCmdPointer->mObj->Dealloc_5291B0();
                pObjCmdPointer->mObj = NULL;
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
    s16 result = pPointer->mCounter;
    if (result != bonus_result::passed && result != bonus_result::failed)
    {
        result = gGame_0x40_67E008->mLocalPlayer->mScores.mBonuses.GetBonusResult_432080(result);
        if (result == bonus_result::passed || result == bonus_result::failed)
        {
            pPointer->mCounter = result;
        }
    }
    return result;
}

MATCH_FUNC(0x505f50)
void ScriptThread::SCRCMD_DISPLAY_MESSAGES_505F50(SCR_TWO_PARAMS* pCmd)
{
    if (gDo_text_id_test_67D6D0)
    {
        sprintf(gTmpBuffer_67C598, "%d", pCmd->mUnsigned1);
        if (!gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598))
        {
            FatalError_4A38C0(Gta2Error::MissionScriptIsReferringToTextIdThatDoesNotExist,
                              "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                              2460,
                              pCmd->mUnsigned1); // Mission script is referring to a 'text id' that doesn't exist. ID
        }
    }
}

MATCH_FUNC(0x506010)
void ScriptThread::SCRCMD_DEC_DEATH_BASE_506010(SCR_FOUR_PARAMS* pCmd)
{
    if (pCmd->mType == SCRCMD_DEC_DEATH_BASE_1)
    {
        gScriptManager_6F8060->mGang1DeathBase = pCmd->mUnsigned3;
    }
    else if (pCmd->mType == SCRCMD_DEC_DEATH_BASE_2)
    {
        gScriptManager_6F8060->mGang2DeathBase = pCmd->mUnsigned3;
    }
    else if (pCmd->mType == SCRCMD_DEC_DEATH_BASE_3)
    {
        gScriptManager_6F8060->mGang3DeathBase = pCmd->mUnsigned3;
    }
}

MATCH_FUNC(0x5060d0)
void ScriptThread::SCRCMD_DO_CRANE_POWERUP_5060D0(SCR_DECLARE_CRANE_POWERUP* pCmd)
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCraneIdx));
    pPointer->mCrane->mPowerupCmd = pCmd->mCmdThis;
}

MATCH_FUNC(0x506140)
void ScriptThread::SCRCMD_START_BASIC_KF_506140(u16* pKfValue)
{
    for (u16 idx = 0; idx < 25; idx++)
    {
        if (gScriptManager_6F8060->mBasicKf[idx] == 0)
        {
            gScriptManager_6F8060->mBasicKf[idx] = *pKfValue;
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
            switch (pBasePtr->mType)
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
                    ScriptThread::SCRCMD_THREAD_DECLARE2_5045D0(static_cast<SCR_THREAD*>(pBasePtr), &pBasePtr->mCmdThis);
                    break;
                case SCRCMD_THREAD_DECLARE4:
                    ScriptThread::SCRCMD_THREAD_DECLARE4_5047C0(static_cast<SCR_THREAD*>(pBasePtr), &pBasePtr->mCmdThis);
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
                    ScriptThread::SCRCMD_START_BASIC_KF_506140(reinterpret_cast<u16*>(&pBasePtr->mCmdThis));
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
    mInLevelStart = true;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5069f0)
void ScriptThread::SCRCMD_LEVELEND_5069F0()
{
    mInLevelStart = false;
    ScriptThread::EndCmd_503670();
}

MATCH_FUNC(0x506a00)
void ScriptThread::SCRCMD_CREATE_THREAD_506A00()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_THREAD* pThread = static_cast<SCR_THREAD*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    ScriptThread* pScriptThread = ScriptThread::SpawnThread_511960(pCmd->mUnsigned1);
    pThread->mScriptThread = pScriptThread;
    pThread->mThreadId = pScriptThread->mThreadId;
    pScriptThread->InitThread_511930(mFlags, pCmd->mUnsigned1);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x506a60)
void ScriptThread::SCRCMD_STOP_THREAD_506A60()
{
    SCR_THREAD* pThread = static_cast<SCR_THREAD*>(
        gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mUnsigned2));
    ScriptThread* pScriptThread;
    s16 threadId;

    s32 opcode_idx = pThread->mType;
    if (opcode_idx != SCRCMD_THREAD_ID)
    {
        if (opcode_idx > SCRCMD_THREAD_DECLARE1 && opcode_idx <= SCRCMD_THREAD_DECLARE5)
        {
            pScriptThread = pThread->mScriptThread;
            threadId = pThread->mThreadId;

            pThread->mScriptThread = NULL;
            pThread->mThreadId = 0;
        }
    }
    else
    {
        pScriptThread = pThread->mScriptThread;
        threadId = pThread->mThreadId;

        pThread->mThreadId = 0;
        pThread->mScriptThread = NULL;
    }

    if (pScriptThread->mThreadId == threadId)
    {
        pScriptThread->StopThread_503610();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x506af0)
void ScriptThread::SCRCMD_IF_JUMP_506AF0()
{
    //  mCondResult = 1  means the boolean opcode has returned true
    //  mCondResult = 0  means the boolean opcode has returned false

    //  If it's a OR and boolean is true, or if it's a AND and boolean is false, jump
    SCR_IF_JUMP* pCmd = reinterpret_cast<SCR_IF_JUMP*>(gBasePtr_6F8070);

    if ((pCmd->is_or == 1 && mCondResult) || (!pCmd->is_or && !mCondResult))
    {

        JumpToCmd_503650(pCmd->else_endif_pointer); //  Jump to the last IF_JUMP or go to ELSE section
        return;
    }

    Next_503620(gBasePtr_6F8070); // go to mCmdNext
}

MATCH_FUNC(0x506b30)
void ScriptThread::SCRCMD_GOSUB_506B30()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    ScriptStackFrame* pFrame = mGosubStack->AllocFrame_5031A0();
    pFrame->mCondResult = mCondResult;
    pFrame->mReturnCmd = gBasePtr_6F8070->mCmdNext;
    mGosubStack->add_503160(pFrame);
    ScriptThread::JumpToCmd_503650(pCmd->mUnsigned1);
}

MATCH_FUNC(0x506b80)
void ScriptThread::SCRCMD_RETURN_506B80() // MISSIONEND
{
    ScriptStackFrame* pFrame = mGosubStack->remove_503180();

    if (pFrame == NULL)
    {
        ScriptThread::EndCmd_503670();
    }
    else
    {
        ScriptThread::JumpToCmd_503650(pFrame->mReturnCmd);
        mCondResult = pFrame->mCondResult;
        mGosubStack->FreeFrame_5031C0(pFrame);
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
    u8 operation_type = ScriptThread::GetOperationType_506BC0(pCmd->mType);
    SCR_POINTER* pLeftOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounterIdx));

    switch (operation_type)
    {
        case script_operation::add_0:
            this->mCondResult = pLeftOperand->mCounter + pCmd->mValue;
            break;
        case script_operation::subtract_1:
            this->mCondResult = pLeftOperand->mCounter - pCmd->mValue;
            break;
        case script_operation::less_4:
            if (pLeftOperand->mCounter < pCmd->mValue)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }

            break;
        case script_operation::less_or_equal_5:
            if (pLeftOperand->mCounter <= pCmd->mValue)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }

            break;
        case script_operation::greater_2:
            if (pLeftOperand->mCounter > pCmd->mValue)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }

            break;
        case script_operation::greater_or_equal_3:
            if (pLeftOperand->mCounter >= pCmd->mValue)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }

            break;
        case script_operation::equal_6:
            if (pLeftOperand->mCounter == pCmd->mValue)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
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
    u8 operation_type = ScriptThread::GetOperationType_506BC0(gBasePtr_6F8070->mType);
    SCR_OPERATE_COUNTER_AND_COUNTER_2* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_COUNTER_2*>(gBasePtr_6F8070);
    SCR_POINTER* pLeftOperand =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex)); // pCmd->mLeftCounterIdx
    SCR_POINTER* pRightOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mRightCounterIdx));
    switch (operation_type)
    {
        case script_operation::add_0:
            this->mCondResult = pLeftOperand->mCounter + pRightOperand->mCounter;
            break;
        case script_operation::subtract_1:
            this->mCondResult = pLeftOperand->mCounter - pRightOperand->mCounter;
            break;
        case script_operation::less_4:
            if (pLeftOperand->mCounter < pRightOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }
            break;
        case script_operation::less_or_equal_5:
            if (pLeftOperand->mCounter <= pRightOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }
            break;
        case script_operation::greater_2:
            if (pLeftOperand->mCounter > pRightOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }
            break;
        case script_operation::greater_or_equal_3:
            if (pLeftOperand->mCounter >= pRightOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }
            break;
        case script_operation::equal_6:
            if (pLeftOperand->mCounter == pRightOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
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
    u8 operation_type = ScriptThread::GetOperationType_506BC0(gBasePtr_6F8070->mType);
    SCR_OPERATE_INT_AND_COUNTER* pCmd = reinterpret_cast<SCR_OPERATE_INT_AND_COUNTER*>(gBasePtr_6F8070);
    SCR_POINTER* pOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(
        reinterpret_cast<SCR_OPERATE_INT_AND_COUNTER*>(gBasePtr_6F8070)->mCounterIdx));

    switch (operation_type)
    {
        case script_operation::add_0:
            this->mCondResult = pCmd->mValue + pOperand->mCounter;
            break;
        case script_operation::subtract_1:
            this->mCondResult = pCmd->mValue - pOperand->mCounter;
            break;
        case script_operation::less_4:
            if (pCmd->mValue < pOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }
            break;
        case script_operation::less_or_equal_5:
            if (pCmd->mValue <= pOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }
            break;
        case script_operation::greater_2:
            if (pCmd->mValue > pOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }
            break;
        case script_operation::greater_or_equal_3:
            if (pCmd->mValue >= pOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }
            break;
        case script_operation::equal_6:
            if (pCmd->mValue == pOperand->mCounter)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
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
    mCondResult = mCondResult == 0; //  just toggle the boolean value
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5078d0)
void ScriptThread::SCRCMD_START_EXEC_5078D0()
{
    s16 exec_flag = mExecFlag;

    //  Only increase EXEC flag if it wasn't set before
    if (exec_flag <= 0)
    {
        mExecFlag = exec_flag + 1; //  increase EXEC flag
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5079a0)
void ScriptThread::SCRCMD_STOP_EXEC_5079A0()
{
    s16 exec_flag = mExecFlag;

    //  Only decrease EXEC flag if it was set before
    if (exec_flag >= 0)
    {
        mExecFlag = exec_flag - 1; //  decrease EXEC flag
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507a70)
void ScriptThread::SCRCMD_INCREMENT_507A70()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    ++pPointer->mCounter; //  Increment Counter
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507b50)
void ScriptThread::SCRCMD_DECREMENT_507B50()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    --pPointer->mCounter; //  Decrement Counter
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507ce0)
void ScriptThread::SCRCMD_CREATE_OBJ_507CE0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_OBJ_DATA* pObjData = reinterpret_cast<SCR_OBJ_DATA*>(gBasePtr_6F8070);

    switch (pObjData->mType)
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
    if (!mInLevelStart)
    {
        gMiss2_25C_6F805C->push_type_2_non_pickup_object_502FF0(pPointer->mObj, false);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507f80)
void ScriptThread::SCRCMD_CREATE_CAR_507F80()
{
    SCR_CAR_DATA_DEC* pCmd = reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070);

    Fix16* z_coord = &reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070)->mPos.z;

    if (*z_coord == kFp255)
    {
        *z_coord = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070)->mPos.x,
                                                                 reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070)->mPos.y);
    }
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCarIdx));
    ScriptThread::SCRCMD_CAR_DECSET_503BC0(reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070), pPointer);
    if (!mInLevelStart)
    {
        gMiss2_25C_6F805C->push_type_1_car_502F80(pPointer->mCar);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508220)
void ScriptThread::SCRCMD_MAKE_CAR_DUMMY_508220()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (pPointer->mCar != NULL)
    {
        if (!pPointer->mCar->GetDriver_4118B0())
        {
            pPointer->mCar->SpawnDriverPed();
        }
        pPointer->mCar->InitCarAIControl_440590();

        pPointer->mCar->SetDriverPedType_421560(ped_type::script_created_5);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508280)
void ScriptThread::SCRCMD_SET_DENSITY_508280()
{
    SCR_MAP_SET_ZONE_INFO* pCmd = reinterpret_cast<SCR_MAP_SET_ZONE_INFO*>(gBasePtr_6F8070);
    str_table_entry* pStrTable = gScriptManager_6F8060->FindStringById_503080(pCmd->mZoneIdx);

    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_SET_CAR_DENSITY:
            gMap_0x370_6F6268->SetZoneCarDensity_4DF9D0(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_GOOD_CAR:
            gMap_0x370_6F6268->SetZoneGoodCarRatio_4DF9F0(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_BAD_CAR:
            gMap_0x370_6F6268->SetZoneBadCarRatio_4DFA10(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_POLICE_CAR:
            gMap_0x370_6F6268->SetZonePoliceCarRatio_4DFA30(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_PED_DENSITY:
            gMap_0x370_6F6268->SetZonePedDensity_4DFA70(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_MUGGER:
            gMap_0x370_6F6268->SetZoneMuggerRatio_4DFA90(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_CARTHIEF:
            gMap_0x370_6F6268->SetZoneCarThiefRatio_4DFAB0(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_ELVIS:
            gMap_0x370_6F6268->SetZoneElvisRatio_4DFAD0(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_GANG:
            gMap_0x370_6F6268->SetZoneGangCharRatio_4DFAF0(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_POLICE_PED:
            gMap_0x370_6F6268->SetZonePolicePedRatio_4DFB10(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_GANGCARRATIO:
            gMap_0x370_6F6268->SetZoneGangCarRatio_4DFA50(pStrTable->mZoneIdx, pCmd->mValue);
        default:
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508550)
void ScriptThread::SCRCMD_POINT_ARROW_3D_508550() //  SCRCMD_POINT_ARROW_3D and SCRCMD_LEVEL_END_ARROW2
{
    SCR_LEVEL_END_ARROW2* pCmd = reinterpret_cast<SCR_LEVEL_END_ARROW2*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mArrowname));
    if (pPointer->mArrow == NULL)
    {
        pPointer->mArrow = gHud_2B00_706620->mArrows.AllocArrow_5D1050();
    }

    pPointer->mArrow->SetArrowAim_476840(pCmd->mPos.x, pCmd->mPos.y, pCmd->mPos.z);

    if (gBasePtr_6F8070->mType == SCRCMD_LEVEL_END_ARROW2)
    {
        pPointer->mArrow->SetArrowColour_5D0510(5);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5086f0)
void ScriptThread::SCRCMD_POINT_ARROW_AT_5086F0()
{
    SCR_POINT_ARROW_AT* pParams = reinterpret_cast<SCR_POINT_ARROW_AT*>(gBasePtr_6F8070);
    SCR_POINT_ARROW_AT* pCmd = reinterpret_cast<SCR_POINT_ARROW_AT*>(gBasePtr_6F8070);

    SCR_POINTER* pArrowPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pParams->mArrowIdx));
    SCR_ARROW_ENTITY* pEntityPtr = static_cast<SCR_ARROW_ENTITY*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mTargetEntityIdx));

    if (pArrowPtr->mArrow == NULL)
    {
        pArrowPtr->mArrow = gHud_2B00_706620->mArrows.AllocArrow_5D1050();
    }

    switch (ScriptThread::GetEntityTypeOfCommand_503410(pEntityPtr->mType))
    {
        case script_entity_type::car_2:
        {
            pArrowPtr->mArrow->SetArrowTargetCar_476860(pEntityPtr->mCar);
            break;
        }

        case script_entity_type::char_1:
        {
            Ped* pChar = pEntityPtr->mChar;

            if (!pChar->is_player_41B0A0() || pChar->mPlayer->get_idx_4219D0() != gGame_0x40_67E008->GetLocalPlayerIdx_476790())
            {
                pArrowPtr->mArrow->SetArrowTargetPed_476850(pChar);
            }

            break;
        }

        case script_entity_type::object_3:
        {
            pArrowPtr->mArrow->SetArrowTargetObject_476870(pEntityPtr->mObj);
            break;
        }

        case script_entity_type::crane_4:
            pArrowPtr->mArrow->SetArrowAim_476840(reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pEntityPtr)->mPos.mX,
                                                         reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pEntityPtr)->mPos.mY,
                                                         kFpOne_6F77C4);
            break;

        case script_entity_type::conveyor_5:
            pArrowPtr->mArrow->SetArrowAim_476840(reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->mRect.mPos.x,
                                                         reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->mRect.mPos.y,
                                                         reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->mRect.mPos.z);
            break;

        default:
            break;
    }

    if (gBasePtr_6F8070->mType == SCRCMD_LEVEL_END_ARROW1)
    {
        pArrowPtr->mArrow->SetArrowColour_5D0510(5);
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508dc0)
void ScriptThread::SCRCMD_ARROW_COLOUR_508DC0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pPointer->mArrow == NULL)
    {
        pPointer->mArrow = gHud_2B00_706620->mArrows.AllocArrow_5D1050();
    }
    pPointer->mArrow->SetArrowColour_5D0510(pCmd->mUnsigned2);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508e80)
void ScriptThread::SCRCMD_REMOVE_ARROW_508E80()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Hud_Arrow* pArrow = pPointer->mArrow;

    if (pArrow != NULL)
    {
        pArrow->ClearTargets_476880();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x508f00)
void ScriptThread::SCRCMD_CHECK_CAR_DAMAGE_508F00()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Car* pCar = pPointer->mCar;
    if (pCar->mDamage >= 320 * pCmd->mSigned2)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509030)
void ScriptThread::SCRCMD_CHECK_HEALTH_509030()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Ped* pPed = pPointer->mChar;
    s16 health_param = pCmd->mSigned2;

    if (pPed != NULL && pPed->get_health_433B70() >= health_param)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5093c0)
void ScriptThread::SCRCMD_DISPLAY_MESSAGE_5093C0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    u8 type = static_cast<u8>(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mUnsigned2);
    if (type == 1)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::jobcomplete_22);
    }
    else if (type == 2)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::jobfail_23);
    }

    if (pCmd->mUnsigned1 == 5501)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::raceover_33);
    }
    else if (pCmd->mUnsigned1 == 5502)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::second_lap_34);
    }
    else if (pCmd->mUnsigned1 == 5503)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::finallap_35);
    }
    else if (pCmd->mUnsigned1 == 5504)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::raceon_36);
    }
    else if (pCmd->mUnsigned1 == 5505)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_30_37);
    }
    else if (pCmd->mUnsigned1 == 5506)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_60_38);
    }
    else if (pCmd->mUnsigned1 == 5507)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_90_39);
    }
    else if (pCmd->mUnsigned1 == 5508)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_120_40);
    }
    else if (pCmd->mUnsigned1 == 5509)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::people_down_150_41);
    }
    else if (pCmd->mUnsigned1 == 5510)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::timeout_42);
    }
    else if (pCmd->mUnsigned1 == 5000)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::timeextended_43);
    }
    else if (pCmd->mUnsigned1 == 5015)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::timesup_44);
    }
    else if (pCmd->mUnsigned1 == 5031)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::sorrydidthathurt_45);
    }
    else if (pCmd->mUnsigned1 == 5032)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::nicework_46);
    }
    else if (pCmd->mUnsigned1 == 5050)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::choctastic_47);
    }
    else if (pCmd->mUnsigned1 == 5051)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::raspberryripple_48);
    }
    else if (pCmd->mUnsigned1 == 5052)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::youshotyourload_49);
    }
    else if (pCmd->mUnsigned1 == 5053)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::oohdidthathurt_50);
    }
    else if (pCmd->mUnsigned1 == 5054)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::deathtoicvans_51);
    }
    else if (pCmd->mUnsigned1 == 5055)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::crispycritter_52);
    }
    else if (pCmd->mUnsigned1 == 5056)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::youretoastbuddy_53);
    }
    else if (pCmd->mUnsigned1 == 5057)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::eatleaddeath_54);
    }
    else if (pCmd->mUnsigned1 == 5058)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::thatsgottahurt_55);
    }
    else if (pCmd->mUnsigned1 == 5059)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::sorryaboutthat_56);
    }
    else if (pCmd->mUnsigned1 == 5060)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::xinloimyman_57);
    }
    else if (pCmd->mUnsigned1 == 5061)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::damnsundaydrivers_58);
    }
    else if (pCmd->mUnsigned1 == 5062)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::suckitandsee_59);
    }
    else if (pCmd->mUnsigned1 == 5063)
    {
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::tastemywrath_60);
    }

    sprintf(gTmpBuffer_67C598, "%d", pCmd->mUnsigned1);

    switch (pCmd->mType)
    {
        case SCRCMD_DISPLAY_MESSAGE:
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598), hud_message_priority::important_3);
            break;
        case SCRCMD_DISPLAY_BRIEF:
            gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
            break;
        case SCRCMD_DISPLAY_BRIEF_NOW:
            gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::now_3, gTmpBuffer_67C598);
            break;
        case SCRCMD_DISPLAY_BRIEF_SOON:
            gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::soon_2, gTmpBuffer_67C598);
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    pPointer->mIndex = gHud_2B00_706620->mPagers.CreateTimer_5D31F0(pCmd->mSigned2);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509730)
void ScriptThread::SCRCMD_CREATE_CHAR_509730()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    ScriptThread::SCRCMD_CHAR_DECSET_2D_3D_503FB0(reinterpret_cast<SCR_CHAR_DATA_DEC*>(gBasePtr_6F8070), pPointer);
    if (!mInLevelStart)
    {
        gMiss2_25C_6F805C->push_type_3_ped_502FB0(pPointer->mChar);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509790)
void ScriptThread::SCRCMD_SET_CHAR_OCCUPATION_509790()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    pPointer->mChar->set_occupation_403970(pCmd->mU32);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5097d0)
void ScriptThread::SCRCMD_SET_CHAR_OBJ1_5097D0()
{
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    ScriptThread::SetCharObjective_504110(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070), pPointer);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509810)
void ScriptThread::SCRCMD_IS_CHAR_OBJ_PASS_509810()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        switch (pPed->mObjectiveStatus)
        {
            case objective_status::failed_2:
            case objective_status::not_finished_0:
                mCondResult = false;
                break;
            case objective_status::passed_1:
                mCondResult = true;
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        switch (pPed->mObjectiveStatus)
        {
            case objective_status::failed_2:
                mCondResult = true;
                break;
            default:
                mCondResult = false;
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->mType) == script_entity_type::char_1)
    {
        if (gBasePtr_6F8070->mType == SCRCMD_GIVE_WEAPON1)
        {
            pPointer->mChar->AddWeaponWithAmmo_45DD30(pCmd->mSigned2, 100);
        }
        else
        {
            pPointer->mChar->AddWeaponWithAmmo_45DD30(pCmd4->mSigned2, static_cast<char_type>(pCmd4->mSigned3));
        }
    }
    else
    {
        // Always 50 ammo for cars
        gWeapon_8_707018->allocate_5E3D50(pCmd->mSigned2, 50, pPointer->mCar);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509990)
void ScriptThread::SCRCMD_IS_CAR_IN_BLOCK_509990()
{
    SCR_IS_CAR_IN_BLOCK* pCmd = reinterpret_cast<SCR_IS_CAR_IN_BLOCK*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCmd->mPos.x, pCmd->mPos.y);
    }

    Car* pCar = pPointer->mCar;

    if (pCar != NULL && pCar->IsWithinBlock(&pCmd->mPos))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509a70)
void ScriptThread::SCRCMD_CAR_IN_AREA_509A70()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Fix16 height = pCmd->mRect.mSize.mY;
    Fix16 width = pCmd->mRect.mSize.mX;
    Fix16 top = pCmd->mRect.mPos.y - height / kFpTwo_6F77C8;
    Fix16 left = pCmd->mRect.mPos.x - width / kFpTwo_6F77C8;

    gScriptManager_6F8060->mAreaRect.SetFromPosSize_463710(left, top, width, height);

    Fix16 hz = kFpHalf_6F75F0 + pCmd->mRect.mPos.z;
    Fix16 lz = pCmd->mRect.mPos.z - kFpHalf_6F75F0;
    gScriptManager_6F8060->mAreaRect.SetHiLowZ_41E370(lz, hz);

    if (pPointer->mCar != NULL && pPointer->mCar->mDespawnStatus != car_despawn_status::despawned_6 &&
        (pPointer->mCar->mCarSprite->IntersectsRectSAT_59FB10(&gScriptManager_6F8060->mAreaRect) ||
         gScriptManager_6F8060->mAreaRect.IntersectsSpriteRenderingRect_59DDF0(pPointer->mCar->mCarSprite)))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509bb0)
void ScriptThread::SCRCMD_HAS_CHAR_DIED_509BB0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (pPointer->mChar && pPointer->mChar->GetPedState_403990() == ped_state_1::dead_9)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509c10)
void ScriptThread::SCRCMD_IS_CHAR_IN_CAR_509C10()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    Ped* pPed = pCharPointer->mChar;

    Car* pCarTarget;

    if (pPed != NULL && (pCarTarget = pCarPointer->mCar) != NULL && pPed->has_car_403B80() &&
        pPed->get_car_416B60() == pCarTarget)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509c90)
void ScriptThread::SCRCMD_IS_CHAR_IN_MODEL_509C90()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Ped* pPed = pCharPointer->mChar;

    if (pPed != NULL && pPed->has_car_403B80() && pPed->get_car_416B60()->GetCarInfoIdx_411940() == pCmd->mSigned2)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d00)
void ScriptThread::SCRCMD_IS_CHAR_IN_ANY_CAR_509D00()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL && pPed->has_car_403B80())
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d60)
void ScriptThread::SCRCMD_DELETE_ITEM_509D60()
{
    ScriptThread::DeallocOrDeleteItem_505B10(gBasePtr_6F8070->mIndex); //  delete item
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509d90)
void ScriptThread::SCRCMD_ADD_SCORE_509D90()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        if (pPed->is_player_41B0A0())
        {
            Player* pPlayer = pPed->mPlayer;
            if (gBasePtr_6F8070->mType == SCRCMD_ADD_SCORE1)
            {
                pPlayer->AddScore_41DC40(pCmd->mS32);
            }
            else
            {
                pPlayer->AddCash_421990(pCmd->mS32); //  no multiplier
            }
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509e00)
void ScriptThread::SCRCMD_ADD_SCORE2_509E00()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mS32));

    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        if (pPed->is_player_41B0A0())
        {
            pPed->mPlayer->AddScore_41DC40(pCounter->mCounter);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509e70)
void ScriptThread::SCRCMD_IS_CHAR_STOPPED_509E70()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL && pPed->GetGameObjectVelocity_433C20() == kFpZero_6F77C0)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x509ed0)
void ScriptThread::SCRCMD_EXPLODE_509ED0()
{
    SCR_EXPLODE* pCmd = reinterpret_cast<SCR_EXPLODE*>(gBasePtr_6F8070);
    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_EXPLODE_NO_RING2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::no_ring_32,
                                                      0);
            break;
        case SCRCMD_EXPLODE_SMALL2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::small_18,
                                                      0);
            break;
        case SCRCMD_EXPLODE:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::item_19,
                                                      0);
            break;
        case SCRCMD_EXPLODE_LARGE2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
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
    switch (pCmd->mFace)
    {
        case 1:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_225_23,
                                                      0);
            break;
        case 2:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_45_22,
                                                      0);
            break;
        case 3:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_135_24,
                                                      0);
            break;
        case 4:
            gObject_5C_6F8F84->CreateExplosion_52A3D0(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mVarname));

    if (pPointer->mChar)
    {
        Fix16 pedX = pPointer->mChar->get_cam_x();
        Fix16 pedY = pPointer->mChar->get_cam_y();
        Fix16 pedZ = pPointer->mChar->get_cam_z();
        Fix16 width = pCmd->mRect.mSize.mX / kFpTwo_6F77C8;
        Fix16 height = pCmd->mRect.mSize.mY / kFpTwo_6F77C8;
        mCondResult = false;

        if (pedX >= pCmd->mRect.mPos.x - width && pedX <= pCmd->mRect.mPos.x + width &&
            pedY >= pCmd->mRect.mPos.y - height && pedY <= pCmd->mRect.mPos.y + height &&
            pedZ.ToUInt8() == pCmd->mRect.mPos.z.ToUInt8())
        {
            switch (gBasePtr_6F8070->mType)
            {
                case SCRCMD_LOCATE_CHAR_ANY:
                    mCondResult = true;
                    break;
                case SCRCMD_LOCATE_CHAR_ONFOOT:
                    if (pPointer->mChar->bHasGameObject_403B70())
                    {
                        mCondResult = true;
                    }

                    break;
                case SCRCMD_LOCATE_CHAR_BY_CAR:
                    if (pPointer->mChar->has_car_403B80())
                    {
                        mCondResult = true;
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_ANY:
                    if (pPointer->mChar->bHasGameObject_403B70() &&
                        pPointer->mChar->GetGameObjectVelocity_433C20() == kFpZero_6F77C0)
                    {
                        mCondResult = true;
                    }
                    else if (pPointer->mChar->has_car_403B80() && pPointer->mChar->GetPedVelocity_45C920() == kFpZero_6F77C0)
                    {
                        mCondResult = true;
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_FOOT:
                    if (pPointer->mChar->bHasGameObject_403B70())
                    {
                        if (pPointer->mChar->GetGameObjectVelocity_433C20() == kFpZero_6F77C0)
                        {
                            mCondResult = true;
                        }
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_CAR:
                    if (pPointer->mChar->has_car_403B80() && pPointer->mChar->GetPedVelocity_45C920() == kFpZero_6F77C0)
                    {
                        mCondResult = true;
                    }

                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidCase,
                                      "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                                      4622,
                                      gBasePtr_6F8070->mType);
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    ScriptThread::SetCharObjective_504110(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070), pPointer);

    SCR_POINTER* pParam;
    switch (pCmd->mObjective)
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
            pParam = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mSecondItemIdx));
            pPointer->mChar->set_objective_target_ped_403AC0(pParam->mChar);
            break;
        case objectives_enum::enter_car_as_driver_35:
            pParam = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mSecondItemIdx));
            pPointer->mChar->SetTargetObjectiveCar(pParam->mCar);
            if (pCmd->mVariant == 1)
            {
                pPointer->mChar->set_enter_car_as_passenger_4039B0(true);
            }
            else
            {
                pPointer->mChar->set_enter_car_as_passenger_4039B0(false);
            }
            break;
        case objectives_enum::kill_car_21:
        case objectives_enum::wait_in_car_27:
        case objectives_enum::leave_car_36:
        case objectives_enum::follow_car_in_car_55:
        case objectives_enum::destroy_car_59:
            pParam = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mSecondItemIdx));
            pPointer->mChar->SetTargetObjectiveCar(pParam->mCar);
            break;
        case objectives_enum::fire_at_object_from_vehicle_57:
        case objectives_enum::destroy_object_58:
            pParam = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mSecondItemIdx));
            pPointer->mChar->mObjectiveTargetObject = pParam->mObj;
            break;
        default:
            break;
    }

    Ped* pPed = pPointer->mChar;
    pPed->mBf21C.bScheduledForRemoval = false;

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a3e0)
void ScriptThread::SCRCMD_SET_CHAR_OBJ3_50A3E0()
{
    SCR_CHAR_OBJ3* pCmd = reinterpret_cast<SCR_CHAR_OBJ3*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    ScriptThread::SetCharObjective_504110(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070), pPointer);

    if (pPointer->mChar)
    {
        (pPointer->mChar)->mObjectiveTargetX = pCmd->mPos.x;
        (pPointer->mChar)->mObjectiveTargetY = pCmd->mPos.y;
        (pPointer->mChar)->mObjectiveTargetZ = pCmd->mPos.z;

        (pPointer->mChar)->mBf21C.bScheduledForRemoval = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a460)
void ScriptThread::SCRCMD_SET_CHAR_OBJ_FOLLOW_50A460()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    ScriptThread::SetCharObjective_504110(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070), pPointer);

    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCarIdx));

    if (pPointer->mChar)
    {
        (pPointer->mChar)->SetTargetObjectiveCar(pCarPointer->mCar);

        Ang16 cmd_rotation;
        cmd_rotation.rValue = pCmd->mRotation;

        Fix16 fix_1;
        fix_1.mValue = cmd_rotation.FromUnsignedToFloat();
        Fix16 fix_2;
        fix_2.mValue = kDegreesToAng16_6F8044.ToFloat();
        Fix16 fix_3;
        fix_3.mValue = fix_2.MultiplyInt64(fix_1);

        Ang16 rotation;
        rotation.rValue = fix_3.ToInt();
        rotation.Normalize();

        (pPointer->mChar)->mFollowCarOffsetAngle = rotation.rValue;
        (pPointer->mChar)->mFollowCarOffsetDistance = pCmd->mOffset;
        (pPointer->mChar)->mBf21C.bScheduledForRemoval = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a570)
void ScriptThread::SCRCMD_CHANGE_CAR_REMAP_50A570()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pCmd->mUnsigned2 != 0xFFu) //  mUnsigned2 = remap
    {
        pPointer->mCar->SetCarRemap(static_cast<u8>(pCmd->mUnsigned2));
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a5b0)
void ScriptThread::SCRCMD_CHANGE_CHAR_REMAP_50A5B0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (pPointer->mChar)
    {
        pPointer->mChar->SetRemap_433C10(static_cast<u8>(pCmd->mUnsigned2));
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a610)
void ScriptThread::SCRCMD_CHECK_CAR_MODEL_50A610()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Car* pCar = pPointer->mCar;
    s16 car_model_index = pCmd->mSigned2;

    if (pCar != NULL && static_cast<s8>(pCar->mCarInfoIdx) == car_model_index)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a670)
void ScriptThread::SCRCMD_CHECK_CAR_REMAP_50A670()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Car* pCar = pPointer->mCar;
    s16 remap = pCmd->mSigned2;

    if (pCar != NULL && pCar->mDespawnStatus != car_despawn_status::despawned_6 && static_cast<u16>(pCar->mCarSprite->mRemap) == remap)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a6e0)
void ScriptThread::SCRCMD_CHECK_CAR_BOTH_50A6E0()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Car* pCar = pPointer->mCar;
    s16 car_model_idx = pCmd->mSigned2;
    s16 remap = pCmd->mSigned3;

    if (pCar != NULL && pCar->mDespawnStatus != car_despawn_status::despawned_6 && static_cast<s8>(pCar->mCarInfoIdx) == car_model_idx &&
        static_cast<u16>(pCar->mCarSprite->mRemap) == remap)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a760)
void ScriptThread::IsOnScreen_50A760()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    bool onScreen;

    switch (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->mType))
    {
        case script_entity_type::car_2:
        {
            if (!pPointer->mCar)
            {
                FatalError_4A38C0(Gta2Error::LineAccessingCarThatHasntBeenCreated, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1376, static_cast<u16>(gBasePtr_6F8070->mCmdThis));
            }

            Car* pCar = pPointer->mCar;

            if (pCar->mDespawnStatus == car_despawn_status::despawned_6)
            {
                onScreen = false;
                break;
            }

            if (gBasePtr_6F8070->mType == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pCar->mCarSprite, 1);
            }
            else
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pCar->mCarSprite, 0);
            }

            break;
        }
        case script_entity_type::char_1:
            if (!pPointer->mChar)
            {
                FatalError_4A38C0(Gta2Error::LineAccessingCharacterThatHasntBeenCreated, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1383, static_cast<u16>(gBasePtr_6F8070->mCmdThis));
            }

            if (gBasePtr_6F8070->mType == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pPointer->mChar->GetSprite_46DF50(), 1);
            }
            else
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pPointer->mChar->GetSprite_46DF50(), 0);
            }

            break;
        case script_entity_type::object_3:
            if (!pPointer->mObj)
            {
                FatalError_4A38C0(Gta2Error::LineAccessingObjectThatHasntBeenCreated, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x138E, static_cast<u16>(gBasePtr_6F8070->mCmdThis));
            }

            if (gBasePtr_6F8070->mType == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pPointer->mObj->mSprite, 1);
            }
            else
            {
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pPointer->mObj->mSprite, 0);
            }

            break;
        case script_entity_type::crane_4:
            if (!pPointer->mCrane)
            {
                FatalError_4A38C0(Gta2Error::LineAccessingCraneThatHasntBeenCreated, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1399, static_cast<u16>(gBasePtr_6F8070->mCmdThis));
            }

            if (gBasePtr_6F8070->mType == SCRCMD_IS_ITEM_ONSCREEN)
            {
                Sprite* pSprite = pPointer->mCrane->GetRotorSprite_411A00();
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pSprite, 1);
            }
            else
            {
                Sprite* pSprite = pPointer->mCrane->GetRotorSprite_411A00();
                onScreen = gGame_0x40_67E008->IsSpriteInAnyPlayerView_4B9890(pSprite, 0);
            }

            break;
        default:
            onScreen = false;
            break;
    }

    if (onScreen)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a940)
void ScriptThread::SCRCMD_DELAY_HERE_50A940()
{
    s16* pDelay = &reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mSigned1;

    if (static_cast<s16>(gBasePtr_6F8070->mIndex) == -1)
    {
        *pDelay = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mUnsigned2;
    }
    else
    {
        *pDelay = gBasePtr_6F8070->mIndex - 1;
        if (!*pDelay)
        {
            *pDelay = -1;
            ScriptThread::Next_503620(gBasePtr_6F8070);
        }
    }
}

MATCH_FUNC(0x50a980)
void ScriptThread::SCRCMD_DELAY_50A980()
{
    s16 delay = gBasePtr_6F8070->mIndex;
    s16* pDelay = &reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mSigned1;

    if (delay == -1)
    {
        *pDelay = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mUnsigned2;
        mCondResult = true;
    }
    else
    {
        *pDelay = delay - 1;
        if (!*pDelay)
        {
            *pDelay = -1;
            mCondResult = false;
        }
        else
        {
            mCondResult = true;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a9e0)
void ScriptThread::EnableThread_50A9E0(u16 idx)
{
    SCR_CMD_HEADER* pCmd = gScriptManager_6F8060->GetBasePointer_512770(idx);

    switch (pCmd->mType)
    {
        case SCRCMD_THREAD_DECLARE2:
            SCRCMD_THREAD_DECLARE2_5045D0(static_cast<SCR_THREAD*>(pCmd), reinterpret_cast<s16*>(pCmd));
            break;
        case SCRCMD_THREAD_DECLARE3:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);

            if (pThread->mTriggerAdded)
            {
                gCarThreadTriggers_214_705F20->SetTriggerEnableState_4768C0(pThread->mTriggerIdx, car_thread_trigger_state::armed_1);
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

            if (pAny->mTriggerAdded)
            {
                gCarThreadTriggers_214_705F20->SetTriggerEnableState_4768C0(pAny->mTriggerIdx, car_thread_trigger_state::armed_1);
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
    ScriptThread::DisableThread_505790(gBasePtr_6F8070->mIndex);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50abf0)
void ScriptThread::SCRCMD_ENABLE_THREAD_50ABF0()
{
    ScriptThread::EnableThread_50A9E0(gBasePtr_6F8070->mIndex);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ac20)
void ScriptThread::SCRCMD_SET_GANG_KILL_REACTION_50AC20()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    str_table_entry* gang_1_str = gScriptManager_6F8060->FindStringById_503080(gBasePtr_6F8070->mIndex);
    if (gang_1_str == NULL)
    {
        FatalError_4A38C0(Gta2Error::UnableToFindGangMatchingNameInSetGangRespect,
                          "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                          5217,
                          static_cast<u16>(gBasePtr_6F8070->mCmdThis));
    }
    Gang* pGang_1 = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_1_str->get_name());
    str_table_entry* gang_2_str = gScriptManager_6F8060->FindStringById_503080(pCmd->mUnsigned3);
    if (gang_2_str == NULL)
    {
        FatalError_4A38C0(Gta2Error::UnableToFindGangMatchingNameInSetGangRespect2,
                          "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                          5223,
                          static_cast<u16>(gBasePtr_6F8070->mCmdThis));
    }
    Gang* pGang_2 = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_2_str->get_name());
    pGang_1->SetGangKillReaction_4BEF50(pGang_2->mGangIdx, static_cast<char_type>(pCmd->mUnsigned2));
    pGang_1->SetKillAffectsRespect_475900(true);
    pGang_2->SetKillAffectsRespect_475900(true);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50acf0)
void ScriptThread::SCRCMD_CHANGE_RESPECT_50ACF0()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);

    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(gBasePtr_6F8070->mIndex);

    char_type* gang_name = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_name);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned3));

    // pCmd->mSigned2 = respect parameter

    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_SET_CHAR_RESPECT:

            pGang->SetRespect_4BEE30(pPointer->mChar->mPlayer->get_idx_4219D0(), 20 * static_cast<u8>(pCmd->mSigned2));
            pGang->SetKillAffectsRespect_475900(true);
            break;
        case SCRCMD_CHANGE_RESPECT:
            if (pCmd->mSigned2 > 0)
            {
                pGang->IncrementRespect_4BEE50(pPointer->mChar->mPlayer->get_idx_4219D0(), 20 * (pCmd->mSigned2));
            }
            else
            {
                pGang->DecrementRespect_4BEEA0(pPointer->mChar->mPlayer->get_idx_4219D0(), 20 * abs(pCmd->mSigned2));
            }
            break;
        case SCRCMD_CHANGE_GANG_RESP:
            pGang->ChangeRespectAndUpdate_4BF000(pPointer->mChar->mPlayer->get_idx_4219D0(), 20 * static_cast<u8>(pCmd->mSigned2));
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50aef0)
void ScriptThread::RespectOperator_50AEF0()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned1));
    str_table_entry* pStr = gScriptManager_6F8060->FindStringById_503080(pCmd->mUnsigned3);
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pStr->get_name());

    switch (pCmd->mType)
    {
        case SCRCMD_CHECK_RESPECT_IS:
        {
            u8 player_idx = pPointer->mChar->mPlayer->get_idx_4219D0();

            if (pGang->GetRespectForPlayer_4BEEF0(player_idx) / 20 == pCmd->mSigned2)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }

            break;
        }
        case SCRCMD_CHECK_RESPECT_LESS:
        {
            u8 player_idx = pPointer->mChar->mPlayer->get_idx_4219D0();

            if (pGang->GetRespectForPlayer_4BEEF0(player_idx) / 20 < pCmd->mSigned2)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }

            break;
        }
        case SCRCMD_CHECK_RESPECT_GREATER:
        {
            u8 player_idx = pPointer->mChar->mPlayer->get_idx_4219D0();

            if (pGang->GetRespectForPlayer_4BEEF0(player_idx) / 20 > pCmd->mSigned2)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;
    if (pPed)
    {
        if (pPed->get_objective_403A80() != objectives_enum::patrol_on_foot_42)
        {
            pPed->SetObjective(objectives_enum::patrol_on_foot_42);
        }
        pPointer->mChar->PushPatrolPoint_4702A0(pCmd->mPos.x.ToInt(),
                                                       pCmd->mPos.y.ToInt(),
                                                       pCmd->mPos.z.ToInt());
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b150)
void ScriptThread::SCRCMD_SET_AMBIENT_50B150()
{
    SCR_SET_AMBIENT* pCmd = reinterpret_cast<SCR_SET_AMBIENT*>(gBasePtr_6F8070);
    gpMapRenderer_6F66E4->SetAmbientLevel_4E9D50(pCmd->mLevel, pCmd->mTime);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b180)
void ScriptThread::SCRCMD_ANSWER_PHONE_50B180()
{
    SCR_ANSWER_PHONE* pCmd = reinterpret_cast<SCR_ANSWER_PHONE*>(gBasePtr_6F8070);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mPedIdx));
    SCR_POINTER* pPtrObj = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mObjIdx));
    Object* pPhoneObj = pPtrObj->mObj;
    if (pPhoneObj && pPtrPed->mChar)
    {
        pPhoneObj->PoolGiveAndMarkDone_5291D0();
        gScriptManager_6F8060->AddPhoneThread_5129B0(pPtrPed->mChar->get_id(),
                                           pPtrObj->mObj->mId,
                                           gBasePtr_6F8070->mCmdThis);
        mCondResult = false;
        pCmd->mAnswered = 0;
        if (pCmd->mTimer > 0)
        {
            mPhoneTimer = pCmd->mTimer;
        }
        else
        {
            mPhoneTimer = -1;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b230)
void ScriptThread::SCRCMD_CHECK_PHONE_50B230()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    ThreadTrigger* pTrigger = gScriptManager_6F8060->FindPhoneThreadByObjId_512AD0(pPointer->mObj->mId);
    SCR_ANSWER_PHONE* pAnswer = static_cast<SCR_ANSWER_PHONE*>(gScriptManager_6F8060->GetBasePointer_512770(pTrigger->mCmdLine));
    if (pAnswer->mAnswered == 1)
    {
        gScriptManager_6F8060->RemovePhoneThread_512A70(pTrigger->mPedId, pTrigger->mTargetId);
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b2c0)
void ScriptThread::SCRCMD_CHECK_PHONETIMER_50B2C0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    ThreadTrigger* pTrigger = gScriptManager_6F8060->FindPhoneThreadByObjId_512AD0(pPointer->mObj->mId);
    if (pTrigger)
    {
        SCR_ANSWER_PHONE* pAnswer = static_cast<SCR_ANSWER_PHONE*>(gScriptManager_6F8060->GetBasePointer_512770(pTrigger->mCmdLine));
        if (static_cast<u16>(mPhoneTimer) != 0xFFFF)
        {
            if (mPhoneTimer == 0 && pAnswer->mAnswered == 0)
            {
                mCondResult = true;
                pPointer->mObj->PoolGiveAndMarkDone_5291D0();
            }
            else
            {
                --mPhoneTimer;
                mCondResult = false;
            }
        }
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b360)
void ScriptThread::SCRCMD_STOP_PHONE_RING_50B360()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Object* pObj = pPointer->mObj;
    if (pObj)
    {
        ThreadTrigger* pTrigger = gScriptManager_6F8060->FindPhoneThreadByObjId_512AD0(pObj->mId);
        if (pTrigger)
        {
            gScriptManager_6F8060->RemovePhoneThread_512A70(pTrigger->mPedId, pTrigger->mTargetId);
        }
        pPointer->mObj->PoolGiveAndQueueObjectType_5291E0(163);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b3d0)
void ScriptThread::SCRCMD_IS_CHAR_FIRE_ONSCREEN_50B3D0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;
    if (pPed->GetAttacking_433CA0() && pPed->GetOffscreenCounter_4039F0() == 0 && pPed->mSelectedWeapon)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b440)
void ScriptThread::SCRCMD_CAR_DRIVE_AWAY_50B440()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    pPointer->mCar->InitCarAIControl_440590();
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b470)
void ScriptThread::SCRCMD_GET_CAR_FROM_CRANE_50B470()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer1 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(
        reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mUnsigned2));
    SCR_POINTER* pPointer2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned1));
    Crane* pCrane = pPointer1->mCrane;

    if (pCrane != NULL)
    {
        pPointer2->mCar = pCrane->GetCarFromCrane_480DA0();
        if (pPointer2->mCar)
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b4f0)
void ScriptThread::SCRCMD_CHAR_TO_DRIVE_CAR_50B4F0()
{
    SCR_CHAR_ENTER_CAR* pCmd = reinterpret_cast<SCR_CHAR_ENTER_CAR*>(gBasePtr_6F8070);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pPtrCar = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCarIdx));

    if (pPtrPed->mChar)
    {
        if (gBasePtr_6F8070->mType != SCRCMD_CHAR_TO_BACKDOOR)
        {
            pPtrPed->mChar->SetObjective2_463830(objectives_enum::no_obj_0);
            pPtrPed->mChar->SetObjective(objectives_enum::enter_car_as_driver_35);
            pPtrPed->mChar->SetTargetObjectiveCar(pPtrCar->mCar);
            pPtrPed->mChar->set_enter_car_as_passenger_4039B0(false);
        }
        else
        {
            pPtrPed->mChar->EnterCarByBackDoor_45C7A0(pPtrCar->mCar);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b5a0)
void ScriptThread::SCRCMD_DRIVER_OUT_CAR_50B5A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pPointer->mChar)
    {
        pPointer->mChar->SetObjective2_463830(objectives_enum::no_obj_0);
        pPointer->mChar->SetObjective(objectives_enum::leave_car_36);
        pPointer->mChar->mTargetObjectiveCar = pPointer->mChar->mCar;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b600)
void ScriptThread::SCRCMD_GIVE_DRIVER_BRAKE_50B600()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pPointer->mCar)
    {
        if (pPointer->mCar->GetDriver_4118B0() == NULL)
        {
            pPointer->mCar->SpawnDriverPed();
        }
        pPointer->mCar->InitCarAIControl_440590();
        pPointer->mCar->SetAIHalted_421540();
        pPointer->mCar->SetDriverPedType_421560(ped_type::script_created_5);
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (pPointer->mIndex != 0xFFFF)
    {
        gHud_2B00_706620->mPagers.ClearPager_5D3280(pPointer->mIndex);
    }
    pPointer->mIndex = 0;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b6f0)
void ScriptThread::SCRCMD_CHECK_SCORE_50B6F0()
{
    SCR_CHECK_SCORE_GREATER* pCmd = reinterpret_cast<SCR_CHECK_SCORE_GREATER*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        if (pPed->is_player_41B0A0() && pPed->mPlayer->GetScore_421980() > pCmd->mTargetScore)
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b760)
void ScriptThread::SCRCMD_GET_SCORE_50B760()
{
    SCR_GET_SCORE* pCmd = reinterpret_cast<SCR_GET_SCORE*>(gBasePtr_6F8070);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pPtrCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounterIdx));

    if (pPtrPed->mChar)
    {
        if (pPtrPed->mChar->is_player_41B0A0())
        {
            pPtrCounter->mCounter = static_cast<u16>(pPtrPed->mChar->mPlayer->GetScore_421980());
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b7d0)
void ScriptThread::SCRCMD_IS_CHAR_IN_GANG_50B7D0()
{
    SCR_IS_CHAR_IN_GANG_ZONE* pCmd = reinterpret_cast<SCR_IS_CHAR_IN_GANG_ZONE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(pCmd->mGangIdx);
    const char_type* gang_name = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_name);
    Ped* pPed = pPointer->mChar;
    if (pPed->is_player_41B0A0())
    {
        if (pPed->mPlayer->get_gang_curr_location_4766D0() == pGang)
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    else
    {
        gmp_map_zone* pZone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(pPed->get_cam_x().ToInt(), pPed->get_cam_y().ToInt(), Gang_14);
        if (pZone && !_strnicmp(pZone->mName, pGang->mName, pZone->mNameLength))
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b8b0)
void ScriptThread::SCRCMD_SET_NO_COLLIDE_50B8B0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Car* pCar = pPointer->mCar;

    if (pCar)
    {
        if (gBasePtr_6F8070->mType == SCRCMD_SET_NO_COLLIDE)
        {
            pCar->AddFlags_421890(car_flag::NoCollide);
        }
        else if (gBasePtr_6F8070->mType == SCRCMD_CLEAR_NO_COLLIDE)
        {
            pCar->mFlags &= static_cast<u16>(~car_flag::NoCollide);
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b910)
void ScriptThread::SCRCMD_IS_CHAR_FIRING_AREA_50B910()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Ped* pPed = pPointer->mChar;

    if (pPed->GetAttacking_433CA0() && pPed->IsWithinArea(&pCmd->mRect))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50b9c0)
void ScriptThread::SCRCMD_GET_PASSENGER_NUM_50B9C0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Car* pCar = pPointer->mCar;

    if (pCar != NULL && pCar->mPassengersList.GetPedsCount_4716B0() >= pCmd->mSigned2)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ba30)
void ScriptThread::SCRCMD_CLEAR_WANTED_LEVEL_50BA30()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    pPointer->mChar->ClearWantedPoints_420B80();
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ba70)
void ScriptThread::SCRCMD_ALT_WANTED_LEVEL_50BA70()
{
    SCR_CMD_ALTER_WANTED_LEVEL* pCmd = reinterpret_cast<SCR_CMD_ALTER_WANTED_LEVEL*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (gBasePtr_6F8070->mType == SCRCMD_ALTER_WANTED_LEVEL)
    {
        if (!pPointer->mChar->WantedStarCountGreaterThan_46F100(pCmd->mWantedLevel))
        {
            pPointer->mChar->set_wanted_star_count_46F070(pCmd->mWantedLevel);
        }
    }
    else
    {
        pPointer->mChar->set_wanted_star_count_46F070(pCmd->mWantedLevel);
    }

    Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bad0)
void ScriptThread::SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Car* pCar = pPointer->mCar;

    if ((pCar->mDamage >= 32000 || pCar->mDespawnStatus == car_despawn_status::despawning_5) && pCar->IsWithinArea(&pCmd->mRect))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bb80)
void ScriptThread::SCRCMD_SEND_CAR_TO_BLOCK_50BB80()
{
    SCR_CAR_DATA_DEC* pCmd = reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr_6F8070);
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCarIdx));
    pScrPtr->mCar->GotoBlock_441080(pCmd->mPos.x.ToUInt8(),
                                           pCmd->mPos.y.ToUInt8(),
                                           pCmd->mPos.z.ToUInt8(),
                                           1);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bbd0)
void ScriptThread::SCRCMD_THREAT_SEARCH_REACT_ADD_GROUP_50BBD0()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCharIdx));

    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_ADD_GROUP:
            pScrPtr->mChar->SpawnPedGroupFollowers_46E200(static_cast<u8>(pCmd->mObjective));
            break;
        case SCRCMD_SET_THREAT_REACT:
            pScrPtr->mChar->mThreatReaction = pCmd->mObjective;
            break;
        case SCRCMD_SET_THREAT_SEARCH:
            pScrPtr->mChar->mThreatSearch = pCmd->mObjective;
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bc60)
void ScriptThread::SCRCMD_CHECK_NUM_ALIVE_50BC60()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pScrPtr =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070)->mCharIdx));
    PedGroup* pPedGroup = pScrPtr->mChar->GetGroup_475AF0();

    if (pPedGroup != NULL && static_cast<s16>(pPedGroup->mCount34) >= pCmd->mObjective)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bcd0)
void ScriptThread::SCRCMD_SET_MIN_ALIVE_50BCD0()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr_6F8070)->mCharIdx));
    pPointer->mChar->GetGroup_475AF0()->mUnused35 = static_cast<char_type>(pCmd->mObjective);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

// 9.6f 0x47BAC0
MATCH_FUNC(0x50bd10)
void ScriptThread::SCRCMD_CHAR_GROUP_50BD10()
{
    SCR_TWO_PARAMS* pParams = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);

    SCR_POINTER* pPtr1 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pParams->mUnsigned1));
    SCR_POINTER* pPtr2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    Ped* pChar = pPtr1->mChar;
    PedGroup* pGroup = pChar->GetGroup_475AF0();

    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_MAKE_LEADER:
            pPtr2->mChar->BecomeLeaderOfGroup_4702D0(pChar);
            break;

        case SCRCMD_REMOVE_CHAR:
            pGroup->RemovePed_4C9970(pPtr2->mChar);
            break;

        case SCRCMD_ADD_CHAR_TO_GROUP:
            pGroup->add_ped_to_end_of_list_4C8F90(pPtr2->mChar);
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bdc0)
void ScriptThread::SCRCMD_SET_COUNTER_50BDC0() // Set counter to a given constant value
{
    SCR_SET_COUNTER* pCmd = reinterpret_cast<SCR_SET_COUNTER*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    pPointer->mCounter = pCmd->mValue;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50be00)
void ScriptThread::SCRCMD_HAS_CAR_WEAPON_50BE00()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    s16 weapon_idx = pCmd->mSigned2;

    if (gWeapon_8_707018->find_5E3D20(pPointer->mCar, weapon_idx))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50be70)
void ScriptThread::SCRCMD_IS_CHAR_HORN_50BE70()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;
    Car* pCar = pPed->get_car_416B60();

    if (pCar != NULL && pCar->IsEmittingHorn_411970())
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bed0)
void ScriptThread::SCRCMD_CHECK_MAX_PASS_50BED0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Car* pCar = pPointer->mChar->get_car_416B60();

    if (pCar && static_cast<u8>(pCar->GetMaxPassengers_440570()) >= pCmd->mSigned2)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50bf40)
void ScriptThread::SCRCMD_IS_CHAR_IN_ZONE_50BF40()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    str_table_entry* pStrEntry = gScriptManager_6F8060->FindStringById_503080(pCmd->mSigned2);
    Player* pPlayer = pPointer->mChar->mPlayer;

    gmp_map_zone* pZone1;
    gmp_map_zone* pZone2;

    if (pPlayer)
    {
        pZone1 = pPlayer->mLocalNavigationZone;
        pZone2 = pPlayer->mNavigationZone;
    }
    else
    {
        pZone1 = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(pPointer->mChar->get_cam_x().ToInt(),
                                                              pPointer->mChar->get_cam_y().ToInt(),
                                                              Local_Navigation_15);
        pZone2 = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(pPointer->mChar->get_cam_x().ToInt(),
                                                              pPointer->mChar->get_cam_y().ToInt(),
                                                              Navigation_1);
    }

    if ((pZone1 && !_strnicmp(pZone1->mName, pStrEntry->get_name(), pZone1->mNameLength)) ||
        (pZone2 && !_strnicmp(pZone2->mName, pStrEntry->get_name(), pZone2->mNameLength)))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c040)
void ScriptThread::SCRCMD_SET_PHONE_DEAD_50C040()
{
    u16* pIndex = &gBasePtr_6F8070->mIndex;
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(*pIndex));

    pScrPtr->mObj->PoolGiveAndQueueObjectType_5291E0(objects::dead_phone_174);
    gScriptManager_6F8060->RemovePhoneThreadByObjId_512AA0(pScrPtr->mObj->mId);
    u16* pPhoneIds = &gScriptManager_6F8060->mPhoneIds[0];

    for (u8 i = 0; i < 31; i++)
    {
        if (*pPhoneIds == *pIndex)
        {
            gGameSave_6F78C8.mCarAndScriptData.mFlags |= 1 << i;
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
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pTrailerPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    Car* pCar;

    if (pTrailerPointer == pCarPointer)
    {
        pCar = pCarPointer->mCar;

        if (pCar->IsTrailerCar_421720() || pCarPointer->mCar->IsTrailerCab_41E460())
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    else
    {
        pCar = pTrailerPointer->mCar;

        if (pCarPointer->mCar->HasAttachedTrailer_475E60(pCar))
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c1b0)
void ScriptThread::SCRCMD_IS_CAR_ON_TRAIL_50C1B0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pTrailerPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    Sprite* pSprite = pTrailerPointer->mCar->mAttachments.FirstSpriteOfType_5A6CA0(sprite_types_enum::car_2);
    Car* pCarOnTrailer;

    if (pSprite != NULL &&
        (pSprite->mSpriteTypeEnum == sprite_types_enum::car_2 ? (pCarOnTrailer = pSprite->mCarBcPtr) : (pCarOnTrailer = NULL),
         pCarOnTrailer == pCarPointer->mCar))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c230)
void ScriptThread::SCRCMD_ENABLE_DISABLE_CRANE_50C230()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    s16 type = gBasePtr_6F8070->mType;

    if (type == SCRCMD_ENABLE_CRANE)
    {
        pPointer->mCrane->mDisabled = 0;
    }
    else if (type == SCRCMD_DISABLE_CRANE)
    {
        pPointer->mCrane->mDisabled = 1;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c2a0)
void ScriptThread::SCRCMD_CAR_GOT_DRIVER_50C2A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pPointer->mCar->mDriver)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c2f0)
void ScriptThread::SCRCMD_SPOTTED_PLAYER_50C2F0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pPointer->mChar->HasSpottedPlayer())
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c350)
void ScriptThread::SCRCMD_GET_LAST_PUNCHED_50C350()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCharTargetPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pCharPunchedPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    pCharPunchedPointer->mChar = pCharTargetPointer->mChar->get_last_char_punched_475B10();
    pCharTargetPointer->mChar->clear_last_char_punched_475B40();

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c3b0)
void ScriptThread::SCRCMD_IS_CHAR_STUNNED_50C3B0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed->GetPedState2_433B60() == ped_state_2::lying_on_floor_22 || pPed->get_health_433B70() <= 25)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c410)
void ScriptThread::SCRCMD_KILL_ALL_PASSENG_50C410()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    pPointer->mCar->mPassengersList.KillAndClearAllPeds_4715A0();

    if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(pPointer->mCar))
    {
        gPublicTransport_181C_6FF1D4->KillAllPassengers_579B20();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c470)
void ScriptThread::SCRCMD_IS_GROUP_IN_CAR_50C470()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pGroupLeader = pPointer->mChar;
    PedGroup* pGroup = pGroupLeader->mPedGroup;

    bool allInCar;
    if (pGroup != NULL)
    {
        allInCar = pGroup->IsAllMembersInSomeCar_4CAA20();
    }
    else
    {
        allInCar = (pGroupLeader->mCar != NULL);
    }
    if (allInCar)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c4e0)
void ScriptThread::SCRCMD_PUNCHED_SOMEONE_50C4E0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pPointer->mChar->get_last_char_punched_475B10() != NULL)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c540)
void ScriptThread::SCRCMD_ADD_CHAR_TO_GANG_50C540()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(pCmd->mUnsigned2);

    char_type* gang_name = pGangStr->get_name();

    pPointer->mChar->mpGang = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_name);

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c5a0)
void ScriptThread::SCRCMD_EXPLODE_50C5A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    s32 type;

    switch (gBasePtr_6F8070->mType)
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

    switch (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->mType))
    {
        case script_entity_type::car_2:
            pPointer->mCar->HandleCarExplosion_43D840(explosion_type::item_19);
            break;
        case script_entity_type::char_1:
        {
            Ped* pChar = pPointer->mChar;

            gObject_5C_6F8F84
                ->CreateExplosion_52A3D0(pChar->get_cam_x(), pChar->get_cam_y(), pChar->get_cam_z(), kAngZero_6F804C, type, 0);

            break;
        }
        case script_entity_type::object_3:
        {
            // 9.6f: Object::GetX_4340D0/GetY_4340E0/GetZ_4340F0 (inlined, using them changes the code)
            Sprite* pSprite = pPointer->mObj->mSprite;

            gObject_5C_6F8F84
                ->CreateExplosion_52A3D0(pSprite->GetXPos(), pSprite->GetYPos(), pSprite->GetZPos(), kAngZero_6F804C, type, 0);

            break;
        }
        case script_entity_type::crane_4:
        {
            SCR_CRANE_TARGET_DEC* pCrane = reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pPointer);

            gObject_5C_6F8F84->CreateExplosion_52A3D0(
                pCrane->mPos.mX,
                pCrane->mPos.mY,
                gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(pCrane->mPos.mX, pCrane->mPos.mY),
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
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pDoorPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    gGarage_48_6FD26C->ParkCarAtDoor_534700(pCarPointer->mCar, pDoorPointer->mDoor);
    if (gBasePtr_6F8070->mType == SCRCMD_PARK_NO_RESPAWN)
    {
        gGarage_48_6FD26C->mNoRespawn = true;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c760)
void ScriptThread::SCRCMD_BEEN_PUNCHED_BY_50C760()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointerVictim = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pPointerAttacker = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    if (pPointerAttacker->mChar->get_last_char_punched_475B10() == pPointerVictim->mChar)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c7d0)
void ScriptThread::SCRCMD_UPDATE_DOOR_50C7D0()
{
    SCR_UPDATE_DOOR* pCmd = reinterpret_cast<SCR_UPDATE_DOOR*>(gBasePtr_6F8070);
    SCR_POINTER* pDoorPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mDoorIdx));

    SCR_POINTER* pTarget;

    switch (pDoorPointer->mDoor->get_open_type_476A90())
    {
        case door_open_type::one_char_on_foot:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mTargetIdx));

            pDoorPointer->mDoor->set_open_details_ped(door_open_type::one_char_on_foot, pTarget->mChar);
            pDoorPointer->mDoor->set_target_id(pTarget->mChar->mId);
            break;
        case door_open_type::one_car:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mTargetIdx));

            pDoorPointer->mDoor->set_open_details_car_bc(door_open_type::one_car, pTarget->mCar);
            pDoorPointer->mDoor->set_target_id(pTarget->mCar->mCarId);
            break;
        case door_open_type::any_player_one_car:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mTargetIdx));

            pDoorPointer->mDoor->set_open_details_car_bc(door_open_type::any_player_one_car, pTarget->mCar);
            pDoorPointer->mDoor->set_target_id(pTarget->mCar->mCarId);
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c8a0)
void ScriptThread::SCRCMD_DOOR_50C8A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_OPEN_DOOR:
        {
            Door* pDoor = pPointer->mDoor;

            if (pDoor->mPrimaryDoorData)
            {
                pDoor->mPrimaryDoorData->Open_49C4E0(0);
            }

            if (pDoor->mSecondaryDoorData)
            {
                pDoor->mSecondaryDoorData->Open_49C4E0(pDoor->mbDoFlip);
            }

            break;
        }
        case SCRCMD_CLOSE_DOOR:
        {
            Door* pDoor = pPointer->mDoor;

            if (!pDoor->mbClosing)
            {
                break;
            }

            if (pDoor->mPrimaryDoorData)
            {
                pDoor->mPrimaryDoorData->Close_49C590(0);
            }

            if (pDoor->mSecondaryDoorData)
            {
                pDoor->mSecondaryDoorData->Close_49C590(pDoor->mbDoFlip);
            }

            break;
        }
        case SCRCMD_SET_DOOR_AUTO:
            pPointer->mDoor->mbAuto = true;
            break;
        case SCRCMD_SET_DOOR_MANUAL:
            pPointer->mDoor->mbAuto = false;
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c990)
void ScriptThread::SCRCMD_REMOVE_WEAPON_50C990()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Ped* pPed = pPointer->mChar;
    Player* pPlayer = pPed->mPlayer;

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

    gMap_0x370_6F6268->RemoveBlock_4E8940(pCmd->mPos.mX,
                                          pCmd->mPos.mY,
                                          pCmd->mPos.mZ,
                                          pCmd->mDoDrop);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ca30)
void ScriptThread::SCRCMD_LOWER_LEVEL_50CA30()
{
    SCR_LOWER_LEVEL* pCmd = reinterpret_cast<SCR_LOWER_LEVEL*>(gBasePtr_6F8070);

    gMap_0x370_6F6268->LowerLevel_4E8B70(pCmd->mMinPos.mX,
                                         pCmd->mMaxPos.mX,
                                         pCmd->mMinPos.mY,
                                         pCmd->mMaxPos.mY);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ca70)
void ScriptThread::SCRCMD_CHANGE_BLOCK_50CA70()
{
    SCR_CHANGE_BLOCK* pCmd = reinterpret_cast<SCR_CHANGE_BLOCK*>(gBasePtr_6F8070);
    gMap_0x370_6F6268->ChangeBlock_4E8620(pCmd->mX,
                                          pCmd->mY,
                                          pCmd->mZ,
                                          pCmd->mChangeType,
                                          pCmd->mInfoWord);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cab0)
void ScriptThread::SCRCMD_ADD_NEW_BLOCK_50CAB0()
{
    gmp_block_info block;

    block.init_44C840();

    gMap_0x370_6F6268->AddNewBlock_4E87C0(reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr_6F8070)->mXpos,
                                          reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr_6F8070)->mYpos,
                                          reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr_6F8070)->mZpos,
                                          &block);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cb20)
void ScriptThread::SCRCMD_ROAD_ON_OFF_50CB20()
{
    SCR_SWITCH_ROAD_ON_OFF* pCmd = reinterpret_cast<SCR_SWITCH_ROAD_ON_OFF*>(gBasePtr_6F8070);

    if (pCmd->mStatus != 0)
    {
        gRouteFinder_6FFDC8->RoadOn_588950(pCmd->mXpos, pCmd->mYpos, pCmd->mZpos);
    }
    else
    {
        gRouteFinder_6FFDC8->RoadOff_588810(pCmd->mXpos, pCmd->mYpos, pCmd->mZpos);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cb70)
void ScriptThread::SCRCMD_CHECK_CAR_DRIVER_50CB70()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    mCondResult = false;

    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_CHECK_CAR_DRIVER:
            if (pPointer->mCar->mDriver != NULL)
            {
                mCondResult = true;
            }

            break;
        case SCRCMD_IS_CAR_WRECKED:
        {
            Car* pCar = pPointer->mCar;

            if (pCar->mDespawnStatus == car_despawn_status::despawned_6 || pCar->mDespawnStatus == car_despawn_status::despawning_5 || pCar->mDamage >= 32000 || pCar->HasSpriteZoom_43A230())
            {
                mCondResult = true;
            }

            break;
        }
        case SCRCMD_CAR_SUNK:
            if (pPointer->mCar->HasSpriteZoom_43A230())
            {
                mCondResult = true;
            }

            break;
        case SCRCMD_CAR_IN_AIR:
            if (pPointer->mCar->IsCarInAir_43A3C0())
            {
                mCondResult = true;
            }

            break;
        case SCRCMD_IS_CAR_CRUSHED:
            if (pPointer->mCar->IsBeingCrushed_43DD50())
            {
                mCondResult = true;
            }

            break;
        case SCRCMD_CARBOMB_ACTIVE:
        {
            Car* pCar = pPointer->mCar;
            if (pCar->mAttachments.GetSpriteForModel_5A6A50(objects::car_bomb_132) != NULL)
            {
                mCondResult = true;
            }

            break;
        }
        case SCRCMD_EMERG_LIGHTS_ON:
        {
            Car* pCar = pPointer->mCar;
            car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(pCar->mCarInfoIdx);

            if ((pInfo->info_flags & car_info_flag::emergency) == car_info_flag::emergency || pCar->mCarInfoIdx == car_model_enum::EDSELFBI)
            {
                if (pCar->mBfA4.bEmergencyLights)
                {
                    mCondResult = true;
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    u16 spawn_count = pCmd->mUnsigned2;
    Generator* pGen = pPointer->mGenerator;
    pGen->Activate_4C1A70();
    pGen->mSpawnsLeft = spawn_count;

    s32 type = pPointer->mGenerator->mGenType;

    if ((type >= 200 && type <= 244) || (type >= 64 && type <= 108))
    {
        pPointer->mGenerator->mSpawnMode = generator_spawn_mode::one_at_a_time_2;
    }
    else
    {
        pPointer->mGenerator->mSpawnMode = generator_spawn_mode::every_cycle_1;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cd30)
void ScriptThread::SCRCMD_SWITCH_GENERATOR3_50CD30()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    pPointer->mGenerator->mGenType = pCmd->mSigned3;

    Generator* pGen = pPointer->mGenerator;
    u16 spawn_count = pCmd->mUnsigned2;
    pGen->Activate_4C1A70();
    pGen->mSpawnsLeft = spawn_count;

    s16 type = pCmd->mSigned3;

    if ((type >= 200 && type <= 244) || (type >= 64 && type <= 108))
    {
        pPointer->mGenerator->mSpawnMode = generator_spawn_mode::one_at_a_time_2;
    }
    else
    {
        pPointer->mGenerator->mSpawnMode = generator_spawn_mode::every_cycle_1;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50cdb0)
void ScriptThread::SCRCMD_CAR_DAMAGE_POS_50CDB0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if ((pPointer->mCar)->IsAreaDamaged_43D1C0(pCmd->mSigned2)) // mSigned2 = damage pos
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ce10)
void ScriptThread::SCRCMD_PARK_FINISHED_50CE10()
{
    if (gGarage_48_6FD26C->mState == garage_state::park_finished_3)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ce50)
void ScriptThread::Gosub_50CE50(SCR_CMD_HEADER* pCmd, u16 cmd_idx)
{
    ScriptStackFrame* pFrame = mGosubStack->AllocFrame_5031A0();
    pFrame->mCondResult = mCondResult;
    pFrame->mReturnCmd = pCmd->mCmdNext;
    mGosubStack->add_503160(pFrame);
    ScriptThread::JumpToCmd_503650(cmd_idx);
}

MATCH_FUNC(0x50ce90)
void ScriptThread::SCRCMD_PHONE_TEMPLATE_50CE90()
{
    SCR_PHONE_TEMPLATE* pCmd = reinterpret_cast<SCR_PHONE_TEMPLATE*>(gBasePtr_6F8070);

    SCR_POINTER* pPassedFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter1));
    SCR_POINTER* pFailedFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter2));
    SCR_POINTER* pOnMission0 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter3b));
    SCR_POINTER* pOnMission1 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter3));
    SCR_POINTER* pOnMission2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter4));
    SCR_POINTER* pOnMission3 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter5));

    str_table_entry* pGangNameEntry = gScriptManager_6F8060->FindStringById_503080(pCmd->mGangname);
    char_type* gang_name = pGangNameEntry->get_name();
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(gang_name);

    s8 player_idx1;

    // If passed previous mission.
    if (pPassedFlag->mCounter >= 1 && pOnMission0->mCounter == 0 && pOnMission1->mCounter == 0 &&
        pOnMission2->mCounter == 0 && pOnMission3->mCounter == 0)
    {
        // If enough respect.
        if ((player_idx1 = gGame_0x40_67E008->mLocalPlayer->get_idx_4219D0(),
             pGang->GetRespectForPlayer_4BEEF0(player_idx1) / 20 >= pCmd->mRespect))
        {
            gScriptManager_6F8060->mDeathArrestFlag = false;
            str_table_entry* mission2_str_table = gScriptManager_6F8060->FindStringById_503080(pCmd->mMission2);
            char_type* mission2_name = mission2_str_table->get_name();
            ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, mission2_name);
            return;
        }

        // Not enough respect.
        s32 brief_id = pCmd->mBriefId - 7;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    s8 player_idx2;

    // If failed previous mission.
    if (pFailedFlag->mCounter >= 1 && pOnMission0->mCounter == 0 && pOnMission1->mCounter == 0 &&
        pOnMission2->mCounter == 0 && pOnMission3->mCounter == 0)
    {
        // If enough respect.
        if ((player_idx2 = gGame_0x40_67E008->mLocalPlayer->get_idx_4219D0(),
             pGang->GetRespectForPlayer_4BEEF0(player_idx2) / 20 >= pCmd->mRespect))
        {
            gScriptManager_6F8060->mDeathArrestFlag = false;
            str_table_entry* mission2_str_table = gScriptManager_6F8060->FindStringById_503080(pCmd->mMission2);
            char_type* mission2_name = mission2_str_table->get_name();
            ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, mission2_name);
            return;
        }

        // Not enough respect.
        s32 brief_id = pCmd->mBriefId - 7;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If on mission gang 1.
    if (pOnMission1->mCounter >= 1)
    {
        s32 brief_id = pCmd->mBriefId - 5;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If on mission gang 2.
    if (pOnMission2->mCounter == 1)
    {
        s32 brief_id = pCmd->mBriefId - 2;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If on mission gang 3.
    if (pOnMission3->mCounter == 1)
    {
        s32 brief_id = pCmd->mBriefId - 1;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If already passed.
    if (pPassedFlag->mCounter == 1)
    {
        s32 brief_id = pCmd->mBriefId;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    // If already failed.
    if (pFailedFlag->mCounter == 1)
    {
        s32 brief_id = pCmd->mBriefId + 1;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    s8 player_idx3;

    // Start mission 1 if enough respect.
    if ((player_idx3 = gGame_0x40_67E008->mLocalPlayer->get_idx_4219D0(),
         pGang->GetRespectForPlayer_4BEEF0(player_idx3) / 20 >= pCmd->mRespect))
    {
        gScriptManager_6F8060->mDeathArrestFlag = false;
        str_table_entry* mission1_str_table = gScriptManager_6F8060->FindStringById_503080(pCmd->mMission1);
        char_type* mission1_name = mission1_str_table->get_name();
        ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, mission1_name);
        return;
    }

    s8 player_idx4;

    // If zero respect.
    if ((player_idx4 = gGame_0x40_67E008->mLocalPlayer->get_idx_4219D0(), (pGang->GetRespectForPlayer_4BEEF0(player_idx4) / 20) == 0))
    {
        s32 brief_id = pCmd->mBriefId - 6;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }

    s8 player_idx5;

    // If negative respect.
    if ((player_idx5 = gGame_0x40_67E008->mLocalPlayer->get_idx_4219D0(), pGang->GetRespectForPlayer_4BEEF0(player_idx5) / 20 < 0))
    {
        s32 brief_id = pCmd->mBriefId - 4;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
    else // Positive respect, but not enough.
    {
        s32 brief_id = pCmd->mBriefId - 7;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
}

MATCH_FUNC(0x50d200)
void ScriptThread::SCRCMD_REMOTE_CONTROL_50D200()
{
    SCR_REMOTE_CONTROL* pCmd = reinterpret_cast<SCR_REMOTE_CONTROL*>(gBasePtr_6F8070);
    SCR_POINTER* pPlayerPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pCarPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCarIdx));
    pPlayerPtr->mChar->mPlayer->DisableInputs_569F40();
    pPlayerPtr->mChar->mPlayer->ResetAuxCamera_5695A0();
    pPlayerPtr->mChar->mPlayer->StartRemoteControl_569600(pCarPtr->mCar);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50d2e0)
void ScriptThread::SCRCMD_LAUNCH_MISSION_50D2E0()
{
    str_table_entry* pMissionStr = gScriptManager_6F8060->FindStringById_503080(gBasePtr_6F8070->mIndex);
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
    if (!mCmdInitialised)
    {
        SCR_DO_SAVE_GAME* pCmd = reinterpret_cast<SCR_DO_SAVE_GAME*>(gBasePtr_6F8070);
        ScriptThread::DisableThread_505790(pCmd->mTriggername);

        if (static_cast<u32>(gpRng_67AB34->get_cur_rng_41CFE0()) > 0)
        {
            u32* mission_flag = reinterpret_cast<u32*>(gScriptManager_6F8060->mMissionFlag);
            if (!mission_flag || *mission_flag)
            {
                // It's in mission, so the player cannot save the game
                sprintf(gTmpBuffer_67C598, "svmiss");
            }
            else
            {
                Player* pPlayer = gGame_0x40_67E008->mLocalPlayer;

                if (pPlayer->GetScore_421980() >= 50000)
                {
                    // Hallelluya! Another soul saved!
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::hallelujah_61);
                    gGame_0x40_67E008->mLocalPlayer->AddCash_421990(-50000);
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

            gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        }

        mCmdInitialised = true;
    }
    else
    {
        Ped* pPlayerPed = gGame_0x40_67E008->mLocalPlayer->mPlayerPed;
        SCR_DO_SAVE_GAME* pCmd = reinterpret_cast<SCR_DO_SAVE_GAME*>(gBasePtr_6F8070);

        // This doesn't look right, but it matches. Might be a missing parens bug in the OG game?
        if (pPlayerPed->get_cam_x() < pCmd->mRect.mPos.x - pCmd->mRect.mSize.mX ||
            pPlayerPed->get_cam_x() > pCmd->mRect.mPos.x + pCmd->mRect.mSize.mX &&
                pPlayerPed->get_cam_y() < pCmd->mRect.mPos.y - pCmd->mRect.mSize.mY ||
            pPlayerPed->get_cam_y() > pCmd->mRect.mPos.y + pCmd->mRect.mSize.mY)
        {
            ScriptThread::EnableThread_50A9E0(pCmd->mTriggername);
            ScriptThread::Next_503620(gBasePtr_6F8070);
        }
    }
}

MATCH_FUNC(0x50d680)
void ScriptThread::SCRCMD_CHANGE_CAR_LOCK_50D680()
{
    SCR_CHANGE_CAR_LOCK* pCmd = reinterpret_cast<SCR_CHANGE_CAR_LOCK*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Car* pCar;

    switch (pCmd->mLockType)
    {
        case Car_Door_Lock::locked_1:
            pCar = pPointer->mCar;
            if (pCar->mDoorLock != Car_Door_Lock::locked_permanently_4)
            {
                pCar->mDoorLock = Car_Door_Lock::locked_1;
            }

            break;
        case Car_Door_Lock::lockout_thief_2:
            pCar = pPointer->mCar;
            if (pCar->mDoorLock != Car_Door_Lock::locked_permanently_4)
            {
                pCar->mDoorLock = Car_Door_Lock::lockout_thief_2;
            }

            break;
        case Car_Door_Lock::unlocked_3:
            pCar = pPointer->mCar;
            if (pCar->mDoorLock != Car_Door_Lock::locked_permanently_4)
            {
                pCar->mDoorLock = Car_Door_Lock::unlocked_3;
            }
            break;
        case Car_Door_Lock::locked_permanently_4:
            pPointer->mCar->mDoorLock = Car_Door_Lock::locked_permanently_4;
            break;
        case Car_Door_Lock::lockout_player_5:
            pCar = pPointer->mCar;
            if (pCar->mDoorLock != Car_Door_Lock::locked_permanently_4)
            {
                pCar->mDoorLock = Car_Door_Lock::lockout_player_5;
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    pPointer->mLight->SetIntensity_476AE0(static_cast<u8>(pCmd->mIntensity));

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50d900)
void ScriptThread::SCRCMD_CHANGE_COLOUR_50D900()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    u32 rgb = pCmd->mU32;
    pPointer->mLight->mArgb = rgb;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50d9a0)
void ScriptThread::SCRCMD_CHANGE_RADIUS_50D9A0()
{
    SCR_CHANGE_LIGHT_RADIUS* pCmd = reinterpret_cast<SCR_CHANGE_LIGHT_RADIUS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    pPointer->mLight->mIntensityRadius.SetRadius_463F10(pCmd->mRadius);

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50da50)
void ScriptThread::SCRCMD_CREATE_LIGHT_50DA50() // CREATE_LIGHT1 or LIGHT_DECSET2
{
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex)); // SCR_CREATE_LIGHT::mLightIdx
    ScriptThread::CreateLight_504EE0(reinterpret_cast<SCR_CREATE_LIGHT*>(gBasePtr_6F8070), pPointer);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50db70)
void ScriptThread::GetSetLivesAndMultiplier_50DB70()
{
    SCR_GET_NUM_LIVES_MULTIPLIERS* pCmd = reinterpret_cast<SCR_GET_NUM_LIVES_MULTIPLIERS*>(gBasePtr_6F8070);
    SCR_POINTER* pPlayerPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mPlayerPedIdx));

    switch (pCmd->mType)
    {
        case SCRCMD_ADD_LIVES:
            if (pPlayerPtr->mChar->mPlayer)
            {
                pPlayerPtr->mChar->mPlayer->ChangeLifeCountByAmount_5699F0(pCmd->mCounterIdx);
            }

            break;
        case SCRCMD_CHECK_NUM_LIVES:
            if (pPlayerPtr->mChar->mPlayer)
            {
                if (pPlayerPtr->mChar->mPlayer->mLives.mValue > pCmd->mCounterIdx)
                {
                    mCondResult = true;
                }
                else
                {
                    mCondResult = false;
                }
            }

            break;
        case SCRCMD_ADD_MULTIPLIER:
            if (pPlayerPtr->mChar->mPlayer)
            {
                pPlayerPtr->mChar->mPlayer->mMultipliers.ChangeStatByAmount_4921B0(pCmd->mCounterIdx);
            }

            break;
        case SCRCMD_CHECK_MULT:
            if (pPlayerPtr->mChar->mPlayer)
            {
                if (pPlayerPtr->mChar->mPlayer->mMultipliers.mValue > pCmd->mCounterIdx)
                {
                    mCondResult = true;
                }
                else
                {
                    mCondResult = false;
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
        static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex)); // pCmd->mPlayerPedIdx
    SCR_POINTER* pCounterPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounterIdx));
    if (gBasePtr_6F8070->mType == SCRCMD_GET_NUM_LIVES)
    {
        pCounterPtr->mCounter = pPlayerPtr->mChar->mPlayer->get_lives_4766C0();
    }
    else
    {
        pCounterPtr->mCounter = pPlayerPtr->mChar->mPlayer->get_multiplier_4766A0();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50dd90)
void ScriptThread::SCRCMD_SET_DIR_OF_TVVAN_50DD90()
{
    SCR_SET_DIR_OF_TVVAN* pCmd = reinterpret_cast<SCR_SET_DIR_OF_TVVAN*>(gBasePtr_6F8070);

    Fix16_Point point(pCmd->mPos.mX, pCmd->mPos.mY);
    gCarManager_6C_677930->mTvVanDir = point;

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50de00)
void ScriptThread::SCRCMD_POINT_ONSCREEN_50DE00()
{
    SCR_IS_POINT_ON_SCREEN* pCmd = reinterpret_cast<SCR_IS_POINT_ON_SCREEN*>(gBasePtr_6F8070);
    if (gGame_0x40_67E008->IsPointOnScreen_4B9A80(pCmd->mXpos, pCmd->mYpos))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50de50)
void ScriptThread::SCRCMD_CHAR_IN_AIR_50DE50()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed->GetPedState2_433B60() == ped_state_2::falling_19 && pPed->GetPedState_403990() == ped_state_1::immobilized_8)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50deb0)
void ScriptThread::SCRCMD_CHAR_SUNK_50DEB0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed->GetPedState2_433B60() == ped_state_2::sinking_20 && pPed->GetPedState_403990() == ped_state_1::immobilized_8)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
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

    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mPtr));

    s32 event_type;

    if (pCmd->mSubtype == 0 || pCmd->mSubtype == 2)
    {
        event_type = bonus_event_type::car_destroyed_1;
    }
    else
    {
        event_type = bonus_event_type::ped_killed_0;
    }

    switch (pCmd->mSubtype)
    {
        case 2:
        case 3:
            gang_idx = gGangPool_CA8_67E274->GetGangIdxByName_4BF210(
                gScriptManager_6F8060->FindStringById_503080(pCmd->mId)->get_name());
            break;
        case 0:
            if (pCmd->mId != 0xFFFF)
            {
                car_model = pCmd->mId;

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
            occupation = pCmd->mId;

            // French version neutralizes bonuses against law enforcement.
            if (bIsFrench_67D53C && (occupation == ped_ocupation_enum::any_law_enforcement || occupation == ped_ocupation_enum::police))
            {
                occupation = ped_ocupation_enum::no_occupation;
            }

            break;
    }

    s8 check_mode = pCmd->mbExclusive != 1 ? 2 : 1;

    if (pCmd->mAltCarModel >= 0)
    {
        alt_car_model = pCmd->mAltCarModel;
    }

    // u16 > 0xFFFF is never true, left over dead code?
    if (pCmd->mZoneStrId > 0xFFFF)
    {
        gMap_0x370_6F6268->FindZoneByName_4DEFD0(gScriptManager_6F8060->FindStringById_503080(pCmd->mZoneStrId)->get_name());
    }

    pPointer->mCounter = gGame_0x40_67E008->mLocalPlayer->mScores.mBonuses.AllocRule_431FE0(event_type,
                                                                                                                   car_model,
                                                                                                                   occupation,
                                                                                                                   gang_idx,
                                                                                                                   pCmd->mRemap,
                                                                                                                   pCmd->mDeathCause,
                                                                                                                   alt_car_model,
                                                                                                                   pCmd->mTimeLimit,
                                                                                                                   check_mode,
                                                                                                                   pCmd->mTargetCount,
                                                                                                                   pCmd->mReward,
                                                                                                                   NULL);

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e0b0)
void ScriptThread::SCRCMD_CHECK_BONUS_50E0B0()
{
    s16 result = ScriptThread::GetBonusResult_505EA0(gBasePtr_6F8070->mIndex);
    mCondResult = false;

    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_CHECK_BONUS1:
            if (result == bonus_result::passed)
            {
                mCondResult = true;
            }
            break;
        case SCRCMD_CHECK_BONUS2:
            if (result == bonus_result::failed)
            {
                mCondResult = true;
            }
            break;
        case SCRCMD_CHECK_BONUS3:
            if (result != bonus_result::in_progress)
            {
                mCondResult = true;
            }
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e120)
void ScriptThread::SCRCMD_SETUP_MODEL_CHECK_50E120()
{
    gCarManager_6C_677930->mModelToCheckDestroy = gBasePtr_6F8070->mIndex;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e150)
void ScriptThread::SCRCMD_MODEL_CHECK_50E150()
{
    if (gCarManager_6C_677930->mModelCheckDestroyed)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e190)
void ScriptThread::GetSpeed_50E190()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pCounterPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    // 9.6f 0x47D070. The cases are in ascending order (the layout doesn't follow it): that order gives
    // the original's inline cut-offs, GetLength_41E260 inlined (with one Abs) for the char's car and
    // called out of line (0x453590) for the car, behind the get_car_velocity_4211C0 wrappers.
    switch (gBasePtr_6F8070->mType)
    {
        case SCRCMD_GET_CAR_SPEED:
            if (pPointer->mCar->mPhysics)
            {
                pCounterPointer->mCounter = pPointer->mCar->mPhysics->get_car_velocity_4211C0().get_value_4754D0();
            }
            else
            {
                pCounterPointer->mCounter = kFpZero_6F77C0.ToInt();
            }
            break;

        case SCRCMD_GET_CHAR_CAR_SPEED:
            if (pPointer->mChar->has_car_403B80())
            {
                if (pPointer->mChar->get_car_416B60()->mPhysics)
                {
                    pCounterPointer->mCounter =
                        pPointer->mChar->get_car_416B60()->mPhysics->get_car_velocity_4211C0().get_value_4754D0();
                }
                else
                {
                    pCounterPointer->mCounter = kFpZero_6F77C0.ToInt();
                }
            }
            break;

        case SCRCMD_GET_MAX_SPEED:
            pCounterPointer->mCounter = pPointer->mCar->GetMaxSpeed_439F30().get_value_4754D0();
            break;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e360)
void ScriptThread::SCRCMD_CHECK_CAR_SPEED_50E360()
{
    SCR_CHECK_CAR_SPEED* pCmd = reinterpret_cast<SCR_CHECK_CAR_SPEED*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (pPointer->mCar->mPhysics &&
        pPointer->mCar->mPhysics->get_car_velocity_4211C0().get_value_4754D0() >
            pCmd->mValue)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e460)
void ScriptThread::SCRCMD_SET_CAR_GRAPHIC_50E460()
{
    SCR_SET_CAR_GRAPHIC* pCmd = reinterpret_cast<SCR_SET_CAR_GRAPHIC*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    pPointer->mCar->SetCarGraphic_43CDF0(static_cast<char_type>(pCmd->mNumber)); // set the number on the top of the car
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e4a0)
void ScriptThread::SCRCMD_CHAR_ARRESTED_50E4A0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pPointer->mChar->mBf21C.bBusted)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e4f0)
void ScriptThread::SCRCMD_S_IS_S_OP_S_50E4F0()
{
    SCR_OPERATE_COUNTER_AND_COUNTER* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_COUNTER*>(gBasePtr_6F8070);
    u8 type = ScriptThread::GetOperationType_506BC0(gBasePtr_6F8070->mType);

    SCR_POINTER* pDestCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mDestinationCounterIdx));
    SCR_POINTER* pFirstOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mFirstOperandCounterIdx));
    SCR_POINTER* pSecondOperand = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mSecondOperandCounterIdx));
    switch (type)
    {
        case script_operation::add_0:
            pDestCounter->mCounter = pFirstOperand->mCounter + pSecondOperand->mCounter;
            break;
        case script_operation::subtract_1:
            pDestCounter->mCounter = pFirstOperand->mCounter - pSecondOperand->mCounter;
            break;
        case script_operation::multiply_8:
            pDestCounter->mCounter = pFirstOperand->mCounter * pSecondOperand->mCounter;
            break;
        case script_operation::divide_7:
            pDestCounter->mCounter = pFirstOperand->mCounter / pSecondOperand->mCounter;
            break;
        case script_operation::modulo_9:
            pDestCounter->mCounter = pFirstOperand->mCounter % pSecondOperand->mCounter;
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
    SCR_POINTER* pDestCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mDestinationCounterIdx));
    SCR_POINTER* pOperandCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mOperandCounterIdx));

    switch (ScriptThread::GetOperationType_506BC0(gBasePtr_6F8070->mType))
    {
        case script_operation::add_0:
            pDestCounter->mCounter = pOperandCounter->mCounter + pCmd->mValue;
            break;
        case script_operation::subtract_1:
            pDestCounter->mCounter = pOperandCounter->mCounter - pCmd->mValue;
            break;
        case script_operation::multiply_8:
            pDestCounter->mCounter = pOperandCounter->mCounter * pCmd->mValue;
            break;
        case script_operation::divide_7:
            pDestCounter->mCounter = pOperandCounter->mCounter / pCmd->mValue;
            break;
        case script_operation::modulo_9:
            pDestCounter->mCounter = pOperandCounter->mCounter % pCmd->mValue;
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    pPointer->mChar->mBf21C.bDriveAggressively = pCmd->mStatus;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e780)
void ScriptThread::SCRCMD_SET_SPEED_50E780()
{
    SCR_SET_RUN_SPEED* pCmd = reinterpret_cast<SCR_SET_RUN_SPEED*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (gBasePtr_6F8070->mType == SCRCMD_SET_RUN_SPEED)
    {
        pPointer->mChar->mMaxSpeed = pCmd->mValue;
    }
    else
    {
        pPointer->mChar->mRunSpeed = pCmd->mValue;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e7f0)
void ScriptThread::SCRCMD_GIVE_CAR_ALARM_50E7F0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    pPointer->mCar->mFlags |= car_flag::CarAlarm;
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e820)
void ScriptThread::SCRCMD_CAR_WEAPON_KIND_PROOF_50E820()
{
    SCR_SET_CAR_DAMAGE_PROOF* pCmd = reinterpret_cast<SCR_SET_CAR_DAMAGE_PROOF*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Car* pCar;

    switch (pCmd->mType)
    {
        case SCRCMD_CAR_BULLETPROOF:
            pCar = pPointer->mCar;
            if (pCmd->mStatus == 1)
            {
                pCar->mFlags |= car_flag::Bulletproof;
            }
            else
            {
                pCar->mFlags &= static_cast<u16>(~car_flag::Bulletproof);
            }
            break;
        case SCRCMD_CAR_ROCKETPROOF:
            pCar = pPointer->mCar;
            if (pCmd->mStatus == 1)
            {
                pCar->mFlags |= car_flag::Rocketproof;
            }
            else
            {
                pCar->mFlags &= static_cast<u16>(~car_flag::Rocketproof);
            }
            break;
        case SCRCMD_CAR_FLAMEPROOF:
            pCar = pPointer->mCar;
            if (pCmd->mStatus == 1)
            {
                pCar->mFlags |= car_flag::Flameproof;
            }
            else
            {
                pCar->mFlags &= static_cast<u16>(~car_flag::Flameproof);
            }
            break;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e900)
void ScriptThread::SCRCMD_PUT_CAR_ON_TRAILER_50E900()
{
    SCR_PUT_CAR_ON_TRAILER* pCmd = reinterpret_cast<SCR_PUT_CAR_ON_TRAILER*>(gBasePtr_6F8070);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pDstCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mTrailerCarIdx));

    Car* pDstCar = pDstCarPointer->mCar;
    if (pDstCar->mDespawnStatus != car_despawn_status::despawned_6)
    {
        if (pDstCar->IsTrailerCab_41E460())
        {
            // put car on the trailer attached to the truck cab
            pDstCar->GetSprite_440840()->AttachSprite_5A3100(pCarPointer->mCar->mCarSprite,
                                                                       kFpZero_6F77C0,
                                                                       kFpZero_6F77C0,
                                                                       kAng180_6F771E);
        }
        else
        {
            // put car directly on the trailer
            pDstCar->mCarSprite->AttachSprite_5A3100(pCarPointer->mCar->mCarSprite,
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
    gHud_2B00_706620->mBrief.ClearAllBriefsWithPriority_5D4890(hud_brief_priority::normal_1);
    gHud_2B00_706620->mBrief.ClearAllBriefsWithPriority_5D4890(hud_brief_priority::now_3);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50e9e0)
void ScriptThread::SCRCMD_CHECK_HEADS_50E9E0()
{
    SCR_CHECK_HEADS_GREATER* pCmd = reinterpret_cast<SCR_CHECK_HEADS_GREATER*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (pPointer->mChar->get_wanted_star_count_46EF00() > pCmd->mWantedLevel)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ea40)
void ScriptThread::SCRCMD_FINISH_LEVEL_50EA40()
{
    SCR_FINISH_LEVEL* pCmd = reinterpret_cast<SCR_FINISH_LEVEL*>(gBasePtr_6F8070);
    char_type bonus_type;
    switch (pCmd->mBonusType)
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

    s32* num_passed_flag = gScriptManager_6F8060->mPassedFlag;
    if (num_passed_flag != NULL && gScriptManager_6F8060->mTotalMissions == *num_passed_flag)
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (!pCmd->mStatus)
    {
        gScriptManager_6F8060->TestAndRecordWeaponHit_512AF0(pPointer->mChar->get_id(), static_cast<char_type>(pCmd->mWeapon), true);
        mCondResult = false;
        pCmd->mStatus = 1;
    }
    else
    {
        if (gScriptManager_6F8060->IsWeaponHitRecorded_512C70(pPointer->mChar->get_id(), static_cast<char_type>(pCmd->mWeapon), true))
        {
            mCondResult = true;
            gScriptManager_6F8060->ClearWeaponHitFlag_512BA0(pPointer->mChar->get_id(), true);
            pCmd->mStatus = 0;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ebd0)
void ScriptThread::SCRCMD_WEAP_HIT_CAR_50EBD0()
{
    SCR_CHECK_WEAPONHIT* pCmd = reinterpret_cast<SCR_CHECK_WEAPONHIT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (!pCmd->mStatus)
    {
        bool hit;
        if (gBasePtr_6F8070->mType == SCRCMD_WEAP_HIT_CAR)
        {
            hit = gScriptManager_6F8060->TestAndRecordWeaponHit_512AF0(pPointer->mCar->mCarId, static_cast<char_type>(pCmd->mWeapon), false);
        }
        else
        {
            hit = gScriptManager_6F8060->TestAndRecordWeaponHit_512AF0(pPointer->mCar->mCarId, ped_death_cause::any_23, false);
        }
        if (hit && gScriptManager_6F8060->IsWeaponHitRecorded_512C70(pPointer->mCar->mCarId, static_cast<char_type>(pCmd->mWeapon), false))
        {
            mCondResult = true;
            pCmd->mStatus = 0;
        }
        else
        {
            mCondResult = false;
            pCmd->mStatus = 1;
        }
    }
    else
    {
        if (gScriptManager_6F8060->IsWeaponHitRecorded_512C70(pPointer->mCar->mCarId, static_cast<char_type>(pCmd->mWeapon), false))
        {
            mCondResult = true;
            pCmd->mStatus = 0;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ece0)
void ScriptThread::SCRCMD_IS_CHAR_ON_FIRE_50ECE0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    if (pPointer->mChar->IsOnFire_475B50())
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ed40)
void ScriptThread::SCRCMD_BRIEF_ONSCREEN_50ED40()
{
    Hud_BriefEntry* pCurrBrief = gHud_2B00_706620->mBrief.mCurrBriefs;
    if (pCurrBrief != NULL && pCurrBrief->mBriefPriority != 0)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ed80)
void ScriptThread::SCRCMD_CREATE_SOUND_50ED80()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
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

    pPassedFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter1));
    pFailedFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter2));
    pGang1MissionFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter3));
    pGang2MissionFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter4));
    pGang3MissionFlag = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounter5));
    str_table_entry* pGangStr = gScriptManager_6F8060->FindStringById_503080(pCmd->mGangname);
    char_type* pGangName = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pGangName);
    if (pGang1MissionFlag->mCounter == 1) //  flag_on_[gang1]_mission == 1
    {
        s32 brief_id = pCmd->mBriefId - 5;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pGang2MissionFlag->mCounter == 1) //  flag_on_[gang2]_mission == 1
    {
        s32 brief_id = pCmd->mBriefId - 2;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pGang3MissionFlag->mCounter == 1) //  flag_on_[gang3]_mission == 1
    {
        s32 brief_id = pCmd->mBriefId - 1;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pPassedFlag->mCounter == 1) //  flag_passed_[gang1]_easy_phone_m1 == 1
    {
        //  not used in original levels, since phones are disabled after finishing mission
        s32 brief_id = pCmd->mBriefId;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    if (pFailedFlag->mCounter == 1) //  flag_failed_[this_gang]_easy_phone_m1 == 1
    {
        //  not used in the game as you can repeat a failed mission in GTA2
        s32 brief_id = pCmd->mBriefId + 1;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    s8 player_idx1;
    //  Check if the player has respect enough to launch the mission
    if ((player_idx1 = gGame_0x40_67E008->mLocalPlayer->get_idx_4219D0(),
         pGang->GetRespectForPlayer_4BEEF0(player_idx1) / 20 >= pCmd->mRespect))
    {
        str_table_entry* mission1_str_table = gScriptManager_6F8060->FindStringById_503080(pCmd->mMission1);
        char_type* mission1_name = mission1_str_table->get_name();
        ScriptThread::launch_mission_5119A0(gBasePtr_6F8070, mission1_name);
        return;
    }
    s8 player_idx2;
    //  Check if the player has respect = 0
    if ((player_idx2 = gGame_0x40_67E008->mLocalPlayer->get_idx_4219D0(), (pGang->GetRespectForPlayer_4BEEF0(player_idx2) / 20) == 0))
    {
        s32 brief_id = pCmd->mBriefId - 6;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
        return;
    }
    s8 player_idx3;
    //  Check if the player has negative respect
    if ((player_idx3 = gGame_0x40_67E008->mLocalPlayer->get_idx_4219D0(), pGang->GetRespectForPlayer_4BEEF0(player_idx3) / 20 < 0))
    {
        s32 brief_id = pCmd->mBriefId - 4;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
    else
    {
        //  In this case, the player has positive respect but not enough to launch the mission
        s32 brief_id = pCmd->mBriefId - 7;
        sprintf(gTmpBuffer_67C598, "%d", brief_id);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        ScriptThread::Next_503620(gBasePtr_6F8070);
    }
}

MATCH_FUNC(0x50f060)
void ScriptThread::SCRCMD_CHAR_INTO_CAR_50F060()
{
    SCR_CHAR_INTO_CAR* pCmd = reinterpret_cast<SCR_CHAR_INTO_CAR*>(gBasePtr_6F8070);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCharIdx));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCarIdx));

    Ped* pNewPed = gPedManager_6787BC->AllocatePed_470F30();
    pCharPointer->mChar = pNewPed;

    if (pNewPed)
    {
        pNewPed->SetPedType_403920(ped_type::script_created_5);
        pCharPointer->mChar->SpawnPedInCar_45C730(pCarPointer->mCar);
        pCharPointer->mChar->set_remap_433B90(pCmd->mRemap);
        pCharPointer->mChar->set_occupation_403970(pCmd->mOccupation);

        if (!mInLevelStart)
        {
            gMiss2_25C_6F805C->push_type_3_ped_502FB0(pCharPointer->mChar);
        }
    }

    Car* pCarTest = pCarPointer->mCar;

    if (pCarTest)
    {
        pCarTest->SetDriverPedType_421560(ped_type::script_created_5);

        Car* pCar = pCarPointer->mCar;

        pCar->AllocateAI_421510();

        pCarPointer->mCar->mAI->SetCar_453BF0(pCarPointer->mCar);
        pCarPointer->mCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f150)
void ScriptThread::SCRCMD_DECIDE_POWERUP_50F150()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pGeneratorPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));

    Car* pCar = pCarPointer->mCar;
    u8* pCarList = reinterpret_cast<u8*>(gScriptManager_6F8060->mCarList);

    u16 i;

    for (i = 0; i < 19; i++)
    {
        if (pCar != NULL && pCar->mCarInfoIdx == *pCarList)
        {
            pGeneratorPointer->mGenerator->mGenType = kDecidePowerupGenTypes_6212F0[i];
            Generator* pGen = pGeneratorPointer->mGenerator;

            if (kDecidePowerupGenTypes_6212F0[i] < 91)
            {
                pGen->Activate_4C1A70();
                pGen->mSpawnsLeft = 3;
            }
            else
            {
                pGen->Activate_4C1A70();
                pGen->mSpawnsLeft = 1;
            }

            break;
        }

        pCarList++;
    }

    if (i == 19)
    {
        Generator* pGen = pGeneratorPointer->mGenerator;
        pGen->Activate_4C1A70();
        pGen->mSpawnsLeft = 3;
        pGeneratorPointer->mGenerator->mGenType = 65;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f220)
void ScriptThread::SCRCMD_SUPPRESS_MODEL_50F220()
{
    if (gBasePtr_6F8070->mIndex == 0xFFFFu)
    {
        gCarManager_6C_677930->mSuppressedModel = car_model_enum::none; //  No model suppressed
    }
    else
    {
        gCarManager_6C_677930->mSuppressedModel = gBasePtr_6F8070->mIndex;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f270)
void ScriptThread::SCRCMD_WARP_CHAR_50F270() // WARP_FROM_CAR_TO_POINT
{
    SCR_WARP_FROM_CAR* pCmd = reinterpret_cast<SCR_WARP_FROM_CAR*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;
    Car* pCar = pPed->get_car_416B60();

    if (pPed != NULL)
    {
        pPed->StartPedWalking_470200(pCmd->mPos.x, pCmd->mPos.y, pCmd->mPos.z);

        Ang16 rotation = kDegreesToAng16_6F8044 * pCmd->mRotation;
        rotation.Normalize();

        pPointer->mChar->SetRotation_433C00(rotation);
        pCar->ClearDriver_4407F0();
        pCar->mDriver = NULL;
        gGame_0x40_67E008->mLocalPlayer->DisableInputs_569F40();
        gGame_0x40_67E008->mLocalPlayer->mGameCamera.UpdateFollowPedCamera_436540(pPointer->mChar);

        Camera* pCamera = &(gGame_0x40_67E008->mLocalPlayer->mGameCamera);

        pCamera->CommitPendingTarget_41E410();

        gGame_0x40_67E008->mLocalPlayer->mGameCamera.ResetCameraSmoothing_435DD0();
        gGame_0x40_67E008->mLocalPlayer->mGameCamera.SetSmoothFollow_475B60();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f3d0)
void ScriptThread::SCRCMD_SET_GROUP_TYPE_50F3D0()
{
    SCR_SET_GROUP_TYPE* pCmd = reinterpret_cast<SCR_SET_GROUP_TYPE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;
    PedGroup* pGroup = pPed->GetGroup_475AF0();

    if (pGroup != NULL)
    {
        pGroup->mGroupType = pCmd->mGroupType;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f410)
void ScriptThread::SCRCMD_CHAR_DO_NOTHING_50F410()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (pPointer->mCar->HasEmergencyLights_414F20())
    {
        if (static_cast<u8>(pCmd->mUnsigned2) == 1)
        {
            pPointer->mCar->ActivateEmergencyLights_43C920(); //  activate sirens
        }
        else
        {
            pPointer->mCar->DeactivateEmergencyLights_43C9D0(); //  deactivate sirens
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f4d0)
void ScriptThread::SCRCMD_CHECK_OBJ_MODEL_50F4D0()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Object* pObj = pPointer->mObj;
    s32 model_idx = pCmd->mU32;

    if (pObj != NULL && pObj->mModel == model_idx)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f550)
void ScriptThread::SCRCMD_PED_GRAPHIC_50F550()
{
    SCR_SET_CHAR_GRAPHIC_TYPE* pCmd = reinterpret_cast<SCR_SET_CHAR_GRAPHIC_TYPE*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (pPointer->mChar != NULL)
    {
        pPointer->mChar->mGraphicType = pCmd->mGraphicType;
        pPointer->mChar->set_remap_433B90(static_cast<char_type>(pCmd->mRemap));
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f5e0)
void ScriptThread::SCRCMD_SET_CHAR_FLAGS_50F5E0()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr_6F8070);
    Ped* pPed = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<SCR_SET_STATE*>(gBasePtr_6F8070)->mVarname))->mChar;

    if (pPed)
    {
        switch (gBasePtr_6F8070->mType)
        {
            case SCRCMD_CHAR_INVINCIBLE:
                if (pCmd->mStatus == 1)
                {
                    pPed->mInvulnerability = 9999;
                }
                else
                {
                    pPed->mInvulnerability = 0;
                }

                break;
            case SCRCMD_SET_STAY_IN_CAR:
                if (pCmd->mStatus == 1)
                {
                    pPed->mBf21C.bStayInCar = true;
                }
                else
                {
                    pPed->mBf21C.bStayInCar = false;
                }

                break;
            case SCRCMD_SET_USE_CAR_WEAPON:
                if (pCmd->mStatus == 1)
                {
                    pPed->mBf21C.bUseCarWeapon = true;
                }
                else
                {
                    pPed->mBf21C.bUseCarWeapon = false;
                }

                break;
            case SCRCMD_DEATH_ARR_STATE:
                gScriptManager_6F8060->mDeathArrState = pCmd->mStatus;
                break;
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f770)
void ScriptThread::SCRCMD_MAKE_MUGGERS_50F770()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr_6F8070);
    if (pCmd->mStatus == 1)
    {
        gPedManager_6787BC->mMakeAllMuggers = true;
    }
    else
    {
        gPedManager_6787BC->mMakeAllMuggers = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f7b0)
void ScriptThread::LocSecChar_50F7B0()
{
    SCR_LOC_SEC_CHAR* pCmd = reinterpret_cast<SCR_LOC_SEC_CHAR*>(gBasePtr_6F8070);
    SCR_POINTER* pChar1 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mChar1Idx));
    SCR_POINTER* pChar2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mChar2Idx));

    Ped* pCenter = pChar2->mChar;
    Fix16 centerX = pCenter->get_cam_x();
    Fix16 centerY = pCenter->get_cam_y();
    Fix16 centerZ = pCenter->get_cam_z();

    Fix16 width = pCmd->mXRange / kFpTwo_6F77C8;
    Fix16 height = pCmd->mYRange / kFpTwo_6F77C8;

    Fix16 testX = pChar1->mChar->get_cam_x();
    Fix16 testY = pChar1->mChar->get_cam_y();
    Fix16 testZ = pChar1->mChar->get_cam_z();
    Fix16 maxX;
    mCondResult = false;

    if (testX >= centerX - width && (maxX = centerX + width, testX <= maxX) && testY >= centerY - height &&
        (centerY = centerY + height, testY <= centerY) && testZ.ToUInt8() == centerZ.ToUInt8())
    {
        switch (gBasePtr_6F8070->mType)
        {
            case SCRCMD_LOC_SECOND_CHAR:
                if (pChar1->mChar->bHasGameObject_403B70())
                {
                    mCondResult = true;
                }

                break;
            case SCRCMD_LOC_SEC_CHAR_CAR:
                if (pChar1->mChar->has_car_403B80())
                {
                    mCondResult = true;
                }

                break;
            case SCRCMD_LOC_SEC_CHAR_ANY:
                mCondResult = true;
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Car* pCar = pPointer->mChar->get_car_416B60();

    if (pCar)
    {
        pCar->SetAIHalted_421540();
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f940)
void ScriptThread::SCRCMD_IS_BUS_FULL_50F940()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    mCondResult = false;
    Ped* pPed = pPointer->mChar;

    if (pPed && pPed->has_car_403B80() && gPublicTransport_181C_6FF1D4->is_bus_579AA0(pPed->get_car_416B60()) &&
        gPublicTransport_181C_6FF1D4->is_bus_full_579AF0())
    {
        mCondResult = true;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50f9b0)
void ScriptThread::SCRCMD_NO_CHARS_OFF_BUS_50F9B0()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr_6F8070);
    if (pCmd->mStatus == 1)
    {
        gPublicTransport_181C_6FF1D4->mStopGettingOffBus = true;
    }
    else
    {
        gPublicTransport_181C_6FF1D4->mStopGettingOffBus = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa00)
void ScriptThread::SCRCMD_KILL_CHAR_50FA00()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        pPed->Kill_46F9D0(); // kill ped
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa40)
void ScriptThread::SCRCMD_SET_SHADING_LEV_50FA40()
{
    gpMapRenderer_6F66E4->set_shading_lev_4E9DB0(static_cast<u8>(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mUnsigned2));
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fa70)
void ScriptThread::SCRCMD_SET_CAR_JAMMED_50FA70()
{
    SCR_SET_CAR_JAMMED* pCmd = reinterpret_cast<SCR_SET_CAR_JAMMED*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Car* pCar = pPointer->mCar;

    if (pCar)
    {
        if (pCmd->mStatus == 1)
        {
            pCar->mFlags |= car_flag::JammedAccelerator;
        }
        else
        {
            pCar->mFlags &= static_cast<u16>(~car_flag::JammedAccelerator);
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
    SCR_POINTER* pPointerBonus = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pPointerCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounterIdx));

    s32 bonus_rule_idx = pPointerBonus->mCounter;

    if (bonus_rule_idx != bonus_result::passed && bonus_rule_idx != bonus_result::failed)
    {
        pPointerCounter->mCounter =
            gGame_0x40_67E008->mLocalPlayer->mScores.mBonuses.GetProgressCount_476660(pPointerBonus->mIndex);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fb60)
void ScriptThread::SCRCMD_SET_ENTER_STATUS_50FB60() // SCRCMD_SET_ENTER_STATUS and SCRCMD_SET_ALL_CONTROLS
{
    SCR_SET_PLAYER_CONTROLS* pCmd = reinterpret_cast<SCR_SET_PLAYER_CONTROLS*>(gBasePtr_6F8070);

    gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex);

    // The player pointer is not used, so the user controls are modified instead
    // This explains desync in multiplayer when SET_ENTER_CONTROL_STATUS are used

    if (pCmd->mNewState == 1)
    {
        if (gBasePtr_6F8070->mType == SCRCMD_SET_ENTER_STATUS)
        {
            gGame_0x40_67E008->mLocalPlayer->EnableEnterVehicles_56A040(); // SET_ENTER_STATUS = ON
        }
        else
        {
            gGame_0x40_67E008->mLocalPlayer->EnableAllControls_56A000(); // SET_ALL_CONTROLS = ON
        }
    }
    else if (gBasePtr_6F8070->mType == SCRCMD_SET_ENTER_STATUS)
    {
        gGame_0x40_67E008->mLocalPlayer->DisableEnterVehicles_56A030(); // SET_ENTER_STATUS = OFF
    }
    else
    {
        gGame_0x40_67E008->mLocalPlayer->DisableAllControls_569FF0(); // SET_ALL_CONTROLS = OFF
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fc20)
void ScriptThread::SCRCMD_SET_FAV_CAR_50FC20()
{
    SCR_SET_FAV_CAR* pCmd = reinterpret_cast<SCR_SET_FAV_CAR*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        pPed->mGangCarModel = pCmd->mFavouriteCar;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fc60)
void ScriptThread::GroupInArea_50FC60()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Fix16 halfwidth = pCmd->mRect.mSize.mX / kFpTwo_6F77C8;
    Fix16 halfheight = pCmd->mRect.mSize.mY / kFpTwo_6F77C8;

    PedGroup* pGroup = pPointer->mChar->GetGroup_475AF0();

    Ped* pMember = pGroup->mPedList[0];
    Ped** ppMember = pGroup->mPedList;

    if (pMember != NULL)
    {
        for (u16 i = 0; i < pGroup->mCount34; i++)
        {
            Fix16 charX = pMember->get_cam_x();
            Fix16 centerX = pCmd->mRect.mPos.x;
            Fix16 centerY, charY;

            if (charX >= centerX - halfwidth && charX <= centerX + halfwidth &&
                (centerY = pCmd->mRect.mPos.y, charY = pMember->get_cam_y(), charY >= centerY - halfheight) &&
                charY <= centerY + halfheight)
            {
                mCondResult = true;
            }
            else
            {
                mCondResult = false;
            }

            ++ppMember;
            pMember = *ppMember;
        }
    }

    Ped* pLeader = pGroup->mPedLeader;
    Fix16 leaderX = pLeader->get_cam_x();
    Fix16 centerX = pCmd->mRect.mPos.x;
    Fix16 centerY, leaderY;

    if (leaderX >= centerX - halfwidth && leaderX <= centerX + halfwidth &&
        (centerY = pCmd->mRect.mPos.y, leaderY = pLeader->get_cam_y(), leaderY >= centerY - halfheight) &&
        leaderY <= centerY + halfheight)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fe00)
void ScriptThread::SCRCMD_SET_KF_WEAPON_50FE00()
{
    SCR_SET_KF_WEAPON* pCmd = reinterpret_cast<SCR_SET_KF_WEAPON*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    s32 weapon_idx = pCmd->mWeaponIdx;

    if (is_car_weapon(weapon_idx))
    {
        pPointer->mChar->mPlayer->SetKFCarWeapon_564710(pPointer->mChar->get_car_416B60(), weapon_idx);
    }
    else
    {
        pPointer->mChar->mPlayer->SetKFWeapon_564790(weapon_idx);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50fed0)
void ScriptThread::SCRCMD_CLEAR_KF_WEAPON_50FED0()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Player* pPlayer = pPointer->mChar->mPlayer;

    pPlayer->ClearKFWeapon_5647D0(); // clear kill frenzy weapon
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ff50)
void ScriptThread::SCRCMD_ADD_ONSCREEN_COUNTER_50FF50()
{
    SCR_ADD_ONSCREEN_COUNTER* pCmd = reinterpret_cast<SCR_ADD_ONSCREEN_COUNTER*>(gBasePtr_6F8070);
    SCR_POINTER* pOnScreenCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCounterIdx));

    pOnScreenCounter->mIndex = gHud_2B00_706620->mPagers.AddOnScreenCounter_5D3220(&pCounter->mCounter);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ffb0)
void ScriptThread::SCRCMD_CLEAR_COUNTER_50FFB0() // SCRCMD_CLEAR_COUNTER and SCRCMD_CLEAR_CLOCK_ONLY
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    u16 idx = pPointer->mIndex;

    if (idx != 0xFFFF)
    {
        if (gBasePtr_6F8070->mType == SCRCMD_CLEAR_COUNTER)
        {
            gHud_2B00_706620->mPagers.ClearCounterOnly_5D3310(idx);
        }
        else // == SCRCMD_CLEAR_CLOCK_ONLY
        {
            gHud_2B00_706620->mPagers.ClearClockOnly_5D32D0(idx);
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    Ped* pGroupLeader = pPointer->mChar;
    PedGroup* pGroup = pGroupLeader->mPedGroup;

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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    Weapon* pWeapon = pPointer->mChar->mPlayer->GetCurrPlayerWeapon_5648F0();

    if (pWeapon != NULL && static_cast<u32>(pWeapon->mWeaponType) == pCmd->mWeaponIdx)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510100)
void ScriptThread::SCRCMD_START_BASIC_KF_510100() // START_BASIC_KF_TEMPLATE
{
    SCR_START_BASIC_KF_TEMPLATE* pCmd = reinterpret_cast<SCR_START_BASIC_KF_TEMPLATE*>(gBasePtr_6F8070);
    if (gScriptManager_6F8060->IsKillFrenzyActive_475A30())
    {
        ScriptThread::EnableThread_50A9E0(pCmd->mTriggername);
        ScriptThread::SCRCMD_RETURN_506B80();
    }
    else
    {
        gScriptManager_6F8060->SetKillFrenzyActive_475A40(true);
        ScriptThread::DisableThread_505790(pCmd->mTriggername);
        gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("kfstart"), hud_message_priority::important_3); // KILL FRENZY!
        gRoot_sound_66B038.PlayVoice_40F090(voice_line::killfrenzy_12); // KILL FRENZY voice
        SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mObjname)); // Skull icon?
        if (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->mType) == script_entity_type::object_3)
        {
            ScriptThread::DeallocOrDeleteItem_505B10(pCmd->mObjname); // Delete skull icon?
        }
        sprintf(gTmpBuffer_67C598, "%d", pCmd->mBriefId);
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer_67C598);
        SCR_POINTER* pPlayerPedPtr = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mPlayername));
        u8 weapon_idx = static_cast<u8>(pCmd->mWeapon);
        if (weapon_idx < weapon_type::car_bomb || weapon_idx > 27)
        {
            // Ped weapons
            if (weapon_idx != 28)
            {
                pPlayerPedPtr->mChar->mPlayer->SetKFWeapon_564790(static_cast<u8>(pCmd->mWeapon));
            }
        }
        else
        {
            // Car weapons
            pPlayerPedPtr->mChar->mPlayer->SetKFCarWeapon_564710(pPlayerPedPtr->mChar->get_car_416B60(),
                                                                                 static_cast<u8>(pCmd->mWeapon));
        }

        u16* pBasicKF = gScriptManager_6F8060->mBasicKf;
        for (u16 idx = 0; idx < GTA2_COUNTOF(gScriptManager_6F8060->mBasicKf); idx++)
        {
            if (*pBasicKF == static_cast<u16>(gBasePtr_6F8070->mCmdThis))
            {
                gGameSave_6F78C8.mCarAndScriptData.mStartedBasicKfMask |= 1 << idx;
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
    SCR_POINTER* pPlayerPedCmdPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mPlayername));

    if (!mCmdInitialised)
    {
        pTimerIDPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mTimername));
        pTimerIDPointer->mIndex = gHud_2B00_706620->mPagers.CreateTimer_5D31F0(pCmd->mTimeLimit);
        SCR_POINTER* pOnScreenCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mOnscreenname));
        SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCountername));
        pOnScreenCounter->mIndex = gHud_2B00_706620->mPagers.AddOnScreenCounter_5D3220(&pCounter->mCounter);
        ++mCmdInitialised;
    }
    else
    {
        if (ScriptThread::GetBonusResult_505EA0(pCmd->mBonusname) == bonus_result::in_progress)
        {
            SCR_POINTER* pBonusType = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mBonusname));
            SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mCountername));
            pCounter->mCounter = pCmd->mTargetTotal -
                gGame_0x40_67E008->mLocalPlayer->mScores.mBonuses.GetProgressCount_476660(pBonusType->mIndex);

            Ped* pPed = pPlayerPedCmdPointer->mChar;

            if (pPed->GetPedState_403990() == ped_state_1::dead_9 || pPed->mBf21C.bBusted != 0)
            {
                gGame_0x40_67E008->mLocalPlayer->mScores.mBonuses.DeactivateBonus_476680(pBonusType->mIndex);
                pPlayerPedCmdPointer->mChar->mPlayer->ClearKFWeapon_5647D0();
            }
        }
        else
        {
            pTimerIDPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mTimername));
            if (pTimerIDPointer->mIndex != 0xFFFF)
            {
                gHud_2B00_706620->mPagers.ClearPager_5D3280(pTimerIDPointer->mIndex);
            }
            pTimerIDPointer->mIndex = 0;
            pPlayerPedCmdPointer->mChar->mPlayer->ClearKFWeapon_5647D0();
            gScriptManager_6F8060->SetKillFrenzyActive_475A40(false);
            if (ScriptThread::GetBonusResult_505EA0(pCmd->mBonusname) == -3)
            {
                gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("kfpass"), hud_message_priority::important_3);
                pPlayerPedCmdPointer->mChar->ClearWantedPoints_420B80();
                ++*gScriptManager_6F8060->mSecretsPassed;

                if (pCmd->mRewardtype == 1)
                {
                    pPlayerPedCmdPointer->mChar->mPlayer->AddScore_41DC40(pCmd->mRewardvalue);
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::frenzypassed_19);
                }
                else if (pCmd->mRewardtype == 2)
                {
                    pPlayerPedCmdPointer->mChar->mPlayer->ChangeMultipliers_4766B0(pCmd->mRewardvalue);
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::frenzypassed_19);
                }
                else
                {
                    if (pCmd->mRewardtype == 3)
                    {
                        pPlayerPedCmdPointer->mChar->mPlayer->ChangeLifeCountByAmount_5699F0(pCmd->mRewardvalue);
                    }
                    gRoot_sound_66B038.PlayVoice_40F090(voice_line::frenzypassed_19);
                }
            }
            else
            {
                gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("kffail"), hud_message_priority::important_3);
                ++*gScriptManager_6F8060->mSecretsFailed;
                gRoot_sound_66B038.PlayVoice_40F090(voice_line::frenzyfail_18);
            }
            ScriptThread::Next_503620(gBasePtr_6F8070);
        }
    }
}

MATCH_FUNC(0x510530)
void ScriptThread::SCRCMD_SET_BONUS_RATING_510530()
{
    gGameSession_67E8E0.SetBonusRatingTextIdx_4C5AB0(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070)->mUnsigned2);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510560)
void ScriptThread::SCRCMD_ADD_TIME_510560()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    gHud_2B00_706620->mPagers.AddTime_5D32F0(pPointer->mIndex, 30 * pCmd->mU32);
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5105b0)
void ScriptThread::SCRCMD_SET_MODEL_WANTED_5105B0()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    s16 model_idx = pCmd->mSigned2;

    if (model_idx == -1)
    {
        gScriptManager_6F8060->mWantedCarModel = car_model_enum::none;
    }
    else
    {
        gScriptManager_6F8060->mWantedCarModel = model_idx;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510600)
void ScriptThread::SCRCMD_CHECK_DEATH_ARR_510600()
{
    if (gScriptManager_6F8060->mDeathArrestFlag)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510660)
void ScriptThread::SCRCMD_FORCE_CLEANUP_510660()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));

    if (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->mType) == script_entity_type::object_3)
    {
        gMiss2_25C_6F805C->push_type_2_non_pickup_object_502FF0(pPointer->mObj, true);
    }
    else if (ScriptThread::GetEntityTypeOfCommand_503410(pPointer->mType) == script_entity_type::sound_10)
    {
        gMiss2_25C_6F805C->push_type_2_object_503050(pPointer->mObj);
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x510780)
void ScriptThread::SCRCMD_SAVE_RESTORE_RESPECT_510780()
{
    u8 idx = 0;
    Gang* pGang = gGangPool_CA8_67E274->FirstGang_4BECA0();

    if (gBasePtr_6F8070->mType == SCRCMD_SAVE_RESPECT)
    {
        for (; pGang != NULL; pGang = gGangPool_CA8_67E274->NextGang_4BECE0())
        {
            gScriptManager_6F8060->mSavedGangRespect[idx++] = pGang->GetRespectForPlayer_4BEEF0(0);
        }
    }
    else
    {
        for (; pGang != NULL; pGang = gGangPool_CA8_67E274->NextGang_4BECE0())
        {
            pGang->SetRespect_4BEE30(0, gScriptManager_6F8060->mSavedGangRespect[idx++]);
        }
    }
    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5108d0)
void ScriptThread::PreExecOpCode_5108D0()
{
    if (mStopped != 1)
    {
        SCR_CMD_HEADER* pCmd = gScriptManager_6F8060->GetBasePointer_512770(mNextCmd);
        gBasePtr_6F8070 = static_cast<SCR_POINTER*>(pCmd);
        switch (pCmd->mType)
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

    if (mStopped == 1)
    {
        return true;
    }

    SCR_CMD_HEADER* pCmd = gScriptManager_6F8060->GetBasePointer_512770(mNextCmd);

    if (!pCmd)
    {
        sprintf(gTmpBuffer_67C598, "Miss2: accessing nonexistant mission line. Current uid: %d", mNextCmd);
    }

    ScriptThread::MissionFailOnDeathOrArrest_503200();

    if (pCmd->mType == SCRCMD_START_EXEC)
    {
        do
        {
            ScriptThread::PreExecOpCode_5108D0();
            gScriptManager_6F8060->GetBasePointer_512770(mNextCmd);
        } while (mExecFlag > 0); //  execute opcodes in the same frame until an ENDEXEC
    }
    else //  It isn't an EXEC opcode
    {
        // mReturnValue == 1  means it's in a WHILE_EXEC block
        if (pCmd->mReturnValue == 1)
        {
            while (true) // execute commands in the same frame
            {
                SCR_CMD_HEADER* pCurrentCmd = gScriptManager_6F8060->GetBasePointer_512770(mNextCmd);
                ScriptThread::PreExecOpCode_5108D0();

                //  If the script has reached a command out of the WHILE_EXEC block, or if it reaches LEVELEND
                if (pCurrentCmd->mReturnValue != 1 || mNextCmd == 0xFFFF)
                {
                    break;
                }

                //  Maybe if it reaches an ENDEXEC?
                if (mExecFlag)
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

    const bool reached_level_end = mNextCmd == 0xFFFF;
    mExecFlag = 0;
    return reached_level_end;
}

MATCH_FUNC(0x511930)
void ScriptThread::InitThread_511930(char_type flags, u16 start_cmd)
{
    mNextCmd = start_cmd;
    mFlags = flags;
    mPhoneTimer = 0;
    mCmdInitialised = 0;
    mExecFlag = 0;
    mCondResult = false;
    mStopped = 0;
}

MATCH_FUNC(0x511960)
ScriptThread* ScriptThread::SpawnThread_511960(u16 start_cmd)
{
    ScriptThread* pNewThread = miss2_0x11C_Pool_6F8064->Allocate_4767A0();
    pNewThread->InitThread_511930(mFlags, start_cmd);
    return pNewThread;
}

MATCH_FUNC(0x5119a0)
void ScriptThread::launch_mission_5119A0(SCR_CMD_HEADER* pCmd, const char_type* pMissionName)
{
    u16 cmd_next;
    gScriptManager_6F8060->LoadMissionScript_512400(pMissionName, &cmd_next);
    strcpy(mStr, pMissionName);
    ScriptThread::Gosub_50CE50(pCmd, cmd_next);
}

MATCH_FUNC(0x511a00)
ScriptThread::ScriptThread()
{
    mpNext = NULL;
    mGosubStack = new ScriptGosubStack();
    mNextCmd = 0;
    mFlags = 0;
    mCondResult = false;
    mCmdInitialised = 0;
    mPhoneTimer = 0;
    mStopped = 0;
    mExecFlag = 0;
    mThreadId = 0;

    memset(mStr, 0, sizeof(mStr));

    mInLevelStart = false;
}

// https://decomp.me/scratch/gmYBd
MATCH_FUNC(0x511cd0)
void ScriptThread::DisableBasicKfsFromSave_511CD0()
{
    for (u16 i = 0; i < 25; i++)
    {
        if ((1 << i) & gGameSave_6F78C8.mCarAndScriptData.mStartedBasicKfMask)
        {
            SCR_START_BASIC_KF_TEMPLATE* pCmd = static_cast<SCR_START_BASIC_KF_TEMPLATE*>(gScriptManager_6F8060->GetBasePointer_512770(
                gScriptManager_6F8060->mBasicKf[i]));
            ScriptThread::DisableThread_505790(pCmd->mTriggername);
            ScriptThread::DeallocOrDeleteItem_505B10(pCmd->mObjname);
        }
    }
}

MATCH_FUNC(0x512fd0)
ScriptThread::~ScriptThread()
{
    if (mGosubStack)
    {
        GTA2_DELETE_AND_NULL(mGosubStack);
    }
    mpNext = NULL;
}

MATCH_FUNC(0x5131C0)
ScriptThreadPool::~ScriptThreadPool()
{
}
