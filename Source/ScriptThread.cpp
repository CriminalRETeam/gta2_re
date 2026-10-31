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

// GetEntityTypeOfCommand
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

// GetOperationType
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
    kDecidePowerupGenTypes,
    19,
    0x6212F0,
    0x41u COMMA 0x42u COMMA 0x43u COMMA 0x44u COMMA 0x45u COMMA 0x46u COMMA 0x47u COMMA 0x48u COMMA 0x49u COMMA 0x5Eu COMMA 0x5Fu COMMA 0x60u COMMA 0x61u COMMA 0x62u COMMA 0x63u COMMA 0x64u COMMA 0x65u COMMA 0x66u COMMA 0x67u);

DEFINE_GLOBAL(ScriptThreadPool*, miss2_0x11C_Pool, 0x6F8064);
// TODO: the current command, of any SCR_* type, typed SCR_POINTER only because many commands read
// mIndex; most uses cast it to their command struct
DEFINE_GLOBAL(SCR_POINTER*, gBasePtr, 0x6F8070);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneAndHalf, Fix16(0x6000, 0), 0x6F76DC);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6F75F0, Fix16(0x2000, 0), 0x6F75F0);
DEFINE_GLOBAL(Fix16, kFpZero_6F77C0, 0x6F77C0);
DEFINE_GLOBAL_INIT(Ang16, kDegreesToAng16, Ang16(0x4), 0x6F8044);
DEFINE_STATIC_MEMBER_INIT(Fix16, ScriptThread, kFp255, Fix16(255), 0x6F7570);
EXTERN_GLOBAL(Ang16, kAngZero_6F804C);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6F77C4, Fix16(1), 0x6F77C4);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6F77C8, Fix16(2), 0x6F77C8);

static inline bool is_car_weapon(s32& weapon_idx)
{
    return weapon_idx >= weapon_type::car_bomb && weapon_idx <= 27;
}

MATCH_FUNC(0x503200)
void ScriptThread::MissionFailOnDeathOrArrest()
{
    if (gScriptManager->mDeathArrState && gScriptManager->mMissionFlag &&
        *gScriptManager->mMissionFlag == 1)
    {
        if (!gGame_0x40->mLocalPlayer->IsWastedOrBustedNotObjective54() &&
            !gGame_0x40->mLocalPlayer->IsBustedObjective54())
        {
            return;
        }

        mGosubStack->remove_5031E0(2);
        Handle_RETURN();

        s32 msg_offset;
        if (gGame_0x40->mLocalPlayer->IsWastedOrBustedNotObjective54())
        {
            msg_offset = 0;
        }
        else
        {
            msg_offset = 5;
        }

        u16 msg_id = gRng.get_uint8(5);

        if (gScriptManager->mGang1MissionFlag && *gScriptManager->mGang1MissionFlag == 1)
        {
            msg_id += gScriptManager->mGang1DeathBase + msg_offset;
        }
        else if (gScriptManager->mGang2MissionFlag && *gScriptManager->mGang2MissionFlag == 1)
        {
            msg_id += gScriptManager->mGang2DeathBase + msg_offset;
        }
        else if (gScriptManager->mGang3MissionFlag && *gScriptManager->mGang3MissionFlag == 1)
        {
            msg_id += gScriptManager->mGang3DeathBase + msg_offset;
        }
        else
        {
            msg_id = 8001;
        }

        sprintf(gTmpBuffer, "%d", msg_id);

        gHud_2B00->mBrief.ClearAllBriefsWithPriority(hud_brief_priority::normal_1);
        gHud_2B00->mBrief.ClearAllBriefsWithPriority(hud_brief_priority::now_3);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        gHud_2B00->mMessage.ShowMessage(gText_0x14->Find("mfail"), hud_message_priority::important_3);
        gRoot_sound.PlayVoice(voice_line::jobfail_23);

        *gScriptManager->mMissionFlag = 0;
        gScriptManager->mDeathArrestFlag = true;
        gGame_0x40->mLocalPlayer->mPlayerPed->mGraphicType = ped_graphic_type::character_1;
    }
}

MATCH_FUNC(0x503410)
u8 ScriptThread::GetEntityTypeOfCommand(u32 opcode)
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
void ScriptThread::Log()
{
    if (gDo_miss_logging)
    {
        sprintf(gTmpBuffer, "%d:\t\t%d ", gpRng->get_cur_rng(), static_cast<s16>(mNextCmd));
        gMiss2Log.Write_Log(gTmpBuffer);
    }
}

MATCH_FUNC(0x503610)
void ScriptThread::StopThread()
{
    mStopped = 1;
}

MATCH_FUNC(0x503620)
void ScriptThread::Next(SCR_CMD_HEADER* pCmd)
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
        ScriptThread::EndCmd();
    }
}

MATCH_FUNC(0x503650)
void ScriptThread::JumpToCmd(u16 cmdIdx)
{
    if (cmdIdx != 0xFFFF)
    {
        mNextCmd = cmdIdx;
    }
    else
    {
        ScriptThread::EndCmd();
    }
}

MATCH_FUNC(0x503670)
void ScriptThread::EndCmd()
{
    mNextCmd = 0xFFFFu;
}

MATCH_FUNC(0x503680)
void ScriptThread::SCRCMD_OBJ_DECSET_2D_3D(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370->FindGroundZForCoord(pCmd->mPos.x, pCmd->mPos.y);
    }

    if (pCmd->mObjId < 0xC8u || pCmd->mObjId > 0xF4u)
    {
        Ang16 rotation = kDegreesToAng16.MultiplyByFix16(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

        pPointer->mObj = gObject_5C->NewPhysicsObj(pCmd->mObjId,
                                                                        pCmd->mPos.x,
                                                                        pCmd->mPos.y,
                                                                        pCmd->mPos.z,
                                                                        rotation);
    }
    else
    {
        Ang16 rotation = kDegreesToAng16.MultiplyByFix16(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

        pPointer->mObj = gObject_5C->NewPhysicsObjUnlessBlocked(pCmd->mObjId,
                                                              pCmd->mPos.x,
                                                              pCmd->mPos.y,
                                                              pCmd->mPos.z,
                                                              rotation);
    }

    if (pPointer->mObj != NULL)
    {
        if (pPointer->mObj->IsModel176To181())
        {
            gHud_2B00->mArrows.place_gang_phone(pPointer->mObj);
            for (u8 i = 0; i < 0x1Fu; i++)
            {
                if (gScriptManager->mPhoneIds[i] == 0)
                {
                    gScriptManager->mPhoneIds[i] = pPointer->mCmdThis;
                    break;
                }
            }
        }
    }
}

MATCH_FUNC(0x5038d0)
void ScriptThread::SCRCMD_OBJ_DECSET(SCR_OBJ_DATA* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370->FindGroundZForCoord(pCmd->mPos.x, pCmd->mPos.y);
    }

    if (pCmd->mObjId < 0xC8u || pCmd->mObjId > 0xF4u)
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16, &pCmd->mRot);
        rotation.Normalize_406C20();

        pPointer->mObj = gObject_5C->NewPhysicsObj(pCmd->mObjId,
                                                                        pCmd->mPos.x,
                                                                        pCmd->mPos.y,
                                                                        pCmd->mPos.z,
                                                                        rotation);
    }
    else
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16, &pCmd->mRot);
        rotation.Normalize_406C20();

        pPointer->mObj = gObject_5C->NewPhysicsObjUnlessBlocked(pCmd->mObjId,
                                                              pCmd->mPos.x,
                                                              pCmd->mPos.y,
                                                              pCmd->mPos.z,
                                                              rotation);
    }

    if (pPointer->mObj != NULL)
    {
        Object* pObj;

        if (pPointer->mObj->IsStaticCollisionCheck())
        {
            pPointer->mObj->SetParam(pCmd->mValueShopType);
        }
        else if (pObj = pPointer->mObj, pObj->IsPickup())
        {
            pObj->SetParam(pCmd->mValueShopType);
        }
        else
        {
            if (pPointer->mObj->IsShop())
            {
                pPointer->mObj->SetParam(pCmd->mValueShopType);
            }
        }
    }
}

MATCH_FUNC(0x503a20)
void ScriptThread::Handle_PLAYER_PED(SCR_PLAYER_PED* pCmd)
{
    Player* pPlayer = gGame_0x40->GetFirstPlayerWithoutPed();

    if (pPlayer)
    {
        Ped* pPed;
        if (gScriptManager->IsLoadedFromSave())
        {
            Fix16 saved_y = kFpOneAndHalf + gGameSave.mPlayerAndWorldStats.mY.GetFloorValue();
            Fix16 saved_x = kFpHalf_6F75F0 + gGameSave.mPlayerAndWorldStats.mX.GetFloorValue();

            pPed = gPedManager->SpawnPedAt(saved_x,
                                                  saved_y,
                                                  gGameSave.mPlayerAndWorldStats.mZ,
                                                  gGameSave.mPlayerAndWorldStats.mPlayerPedRemap,
                                                  kAngZero_6F804C);
        }
        else
        {
            if (pCmd->mPos.z == kFp255) //  kFp255 is 255.0
            {
                //  Calculate the real Z position at (X,Y) based on the map
                pCmd->mPos.z =
                    gMap_0x370->FindGroundZForCoord(pCmd->mPos.x, pCmd->mPos.y);
            }

            Ang16 rotation = kDegreesToAng16.MultiplyByFix16(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

            pPed = gPedManager->SpawnPedAt(pCmd->mPos.x,
                                                  pCmd->mPos.y,
                                                  pCmd->mPos.z,
                                                  static_cast<u8>(pCmd->mRemap),
                                                  rotation);
        }

        if (pPed != NULL)
        {
            pPed->SetPedType(ped_type::player_2);
            if (!gScriptManager->IsLoadedFromSave())
            {
                pPed->set_health(100);
            }

            pPlayer->InitPlayerPed(pPed);
            pPed->mGraphicType = ped_graphic_type::character_1;
            pCmd->mPed = pPed;

            Sprite* pSprite = pPed->GetSprite();
            pSprite->ResolveCollisionWithCarPedOrObject();
        }
    }
}

MATCH_FUNC(0x503bc0)
void ScriptThread::SCRCMD_CAR_DECSET(SCR_CAR_DATA_DEC* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255) //  255.0
    {
        pCmd->mPos.z =
            gMap_0x370->FindGroundZForCoord(pCmd->mPos.x, pCmd->mPos.y);
    }
    if (pCmd->mTrailerId == -1) //  No trailers
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16, &pCmd->mRot);
        rotation.Normalize_406C20();
        pPointer->mCar = gCarManager_6C->SpawnCar_426E10(pCmd->mPos.x,
                                                                pCmd->mPos.y,
                                                                pCmd->mPos.z,
                                                                rotation,
                                                                pCmd->mCarId);
    }
    else if (pCmd->mTrailerId == -2) //  Mini car
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16, &pCmd->mRot);
        rotation.Normalize_406C20();
        pPointer->mCar = gCarManager_6C->SpawnCar(pCmd->mPos.x,
                                                                pCmd->mPos.y,
                                                                pCmd->mPos.z,
                                                                rotation,
                                                                pCmd->mCarId);
        if (pPointer->mCar != NULL)
        {
            pPointer->mCar->LockPermanently();
            pPointer->mCar->SetMiniCar();
            pPointer->mCar->UpdateDrawLayer();
        }
    }
    else //  There is a trailer
    {
        if (pCmd->mCarId == car_model_enum::TRUKTRNS) // 66 = TRUKTRNS Truck Trailer, Flatbed
        {
            Car* pTrailerCar = gCarManager_6C->SpawnCar_shortened(pCmd->mTrailerId);
            Ang16 rotation;
            rotation.ConvertAndMultiply(&kDegreesToAng16, &pCmd->mRot);
            rotation.Normalize();

            pPointer->mCar = gCarManager_6C->SpawnCar_426E10(pCmd->mPos.x,
                                                                    pCmd->mPos.y,
                                                                    pCmd->mPos.z,
                                                                    rotation,
                                                                    pCmd->mCarId);
            pPointer->mCar->mCarSprite->AttachSprite(pTrailerCar->mCarSprite,
                                                                                      kFpZero_6F77C0,
                                                                                      kFpZero_6F77C0,
                                                                                      kAng180_6F771E);
            pTrailerCar->IncrementCarStats(car_kind::mission_car_8);
        }
        else
        {
            Ang16 rotation;
            rotation.ConvertAndMultiply(&kDegreesToAng16, &pCmd->mRot);
            rotation.Normalize();

            Trailer* pTrailer = gCarManager_6C->SpawnCabAndTrailer(pCmd->mPos.x,
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
        pCar->SetDriverPedType(ped_type::script_created_5);

        pPointer->mCar->LockoutThiefUnlessPermanent();
        pPointer->mCar->IncrementCarStats(car_kind::mission_car_8);
        pPointer->mCar->mCarSprite->ResolveCollisionWithCarPedOrObject();

        if (pCmd->mType >= static_cast<u32>(SCRCMD_CREATE_GANG_CAR1) && pCmd->mType <= static_cast<u32>(SCRCMD_CREATE_GANG_CAR4))
        {
            s8 gang_idx = gGangPool_CA8->FindGangByCarModel(pPointer->mCar->mCarInfoIdx);
            if (gang_idx > -1)
            {
                Gang* pGang = gGangPool_CA8->GangByIdx(gang_idx);
                pPointer->mCar->AttachGangIcon(pGang->mArrowColour);
            }
        }
    }
}

MATCH_FUNC(0x503f80)
void ScriptThread::SCRCMD_PARKED_CAR_DECSET(SCR_POINTER* pCmd)
{
    ScriptThread::SCRCMD_CAR_DECSET(reinterpret_cast<SCR_CAR_DATA_DEC*>(pCmd), pCmd);
    (pCmd->mCar)->ReassignCarStats(car_kind::parked_car_9);
    Car* pCar = pCmd->mCar;
    pCar->SetDriverPedType(ped_type::special_ped_4);
}

MATCH_FUNC(0x503fb0)
void ScriptThread::SCRCMD_CHAR_DECSET_2D_3D(SCR_CHAR_DATA_DEC* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370->FindGroundZForCoord(pCmd->mPos.x, pCmd->mPos.y);
    }

    Ang16 rotation = kDegreesToAng16.MultiplyByFix16(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

    Ped* pPed = gPedManager->SpawnPedAt(pCmd->mPos.x,
                                          pCmd->mPos.y,
                                          pCmd->mPos.z,
                                          static_cast<u8>(pCmd->mRemap),
                                          rotation);
    pPointer->mChar = pPed;

    if (pPed)
    {
        pPointer->mChar->SetPedType(ped_type::script_created_5);
        pPointer->mChar->set_occupation(pCmd->mOccupation);
        pPointer->mChar->mGraphicType = ped_graphic_type::character_1;
        pPointer->mChar->SetObjective(objectives_enum::wait_on_foot_26);
        pPointer->mChar->set_health(100);
        Sprite* pSprite = pPointer->mChar->GetSprite();
        pSprite->ResolveCollisionWithCarPedOrObject();
    }
}

MATCH_FUNC(0x504110)
void ScriptThread::SetCharObjective(SCR_CHAR_OBJECTIVE* pCmd, SCR_POINTER* pPointer)
{
    pPointer->mChar->SetObjective(pCmd->mObjective);
    pPointer->mChar->mBf21C.bScheduledForRemoval = false;
}

MATCH_FUNC(0x504150)
void ScriptThread::Handle_MAP_ZONE_SET(SCR_MAP_ZONE_SET* pCmd)
{
    str_table_entry* pZoneStr = gScriptManager->FindStringById(pCmd->mCmdThis);
    gMap_0x370->SetZoneInfo(pZoneStr->mZoneIdx,
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
void ScriptThread::Handle_ARROW_DEC(SCR_TWO_PARAMS* pCmd)
{
    pCmd->mU32 = 0;
}

MATCH_FUNC(0x5041c0)
void ScriptThread::SCRCMD_CRANE(SCR_CRANE_TARGET_DEC* pTargetCmd, SCR_CRANE_BASIC_DEC* pBasicCmd)
{
    pBasicCmd->mCrane =
        gCranePool_D9C->NewCrane(pTargetCmd->mPos.mX, pTargetCmd->mPos.mY);

    if (pTargetCmd->mType == SCRCMD_CRANE_TARGET_DEC)
    {
        if (pTargetCmd->mHomeCranetype == crane_home_target::first_0)
        {
            Fix16 fix_rot = Fix16(kDegreesToAng16.rValue) * Fix16(pTargetCmd->mTargetRotation);
            Ang16 rotation(&fix_rot, 0);

            pTargetCmd->mCrane->SetTarget1(pTargetCmd->mTargetPos.mX,
                                                                     pTargetCmd->mTargetPos.mY,
                                                                     rotation);
        }
        else if (pTargetCmd->mHomeCranetype == crane_home_target::second_1)
        {
            Fix16 fix_rot = Fix16(kDegreesToAng16.rValue) * Fix16(pTargetCmd->mTargetRotation);
            Ang16 rotation(&fix_rot, 0);

            pTargetCmd->mCrane->SetTarget2(pTargetCmd->mTargetPos.mX,
                                                                     pTargetCmd->mTargetPos.mY,
                                                                     rotation);
        }
    }
    else if (pTargetCmd->mType == SCRCMD_CRANE_BASIC_DEC)
    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16, reinterpret_cast<Ang16*>(&pBasicCmd->mTargetRotation));
        rotation.Normalize_406C20();
        pBasicCmd->mCrane->SetTarget1(pBasicCmd->mTargetPos.mX,
                                                                pBasicCmd->mTargetPos.mY,
                                                                rotation);

        rotation.ConvertAndMultiply(&kDegreesToAng16, reinterpret_cast<Ang16*>(&pBasicCmd->mSecondRotation));
        rotation.Normalize_406C20();
        pBasicCmd->mCrane->SetTarget2(pBasicCmd->mSecondPos.mX,
                                                                pBasicCmd->mSecondPos.mY,
                                                                rotation);
    }

    {
        Ang16 rotation;
        rotation.ConvertAndMultiply(&kDegreesToAng16, reinterpret_cast<Ang16*>(&pTargetCmd->mHomeRotation));
        rotation.Normalize_406C20();
        pBasicCmd->mCrane->SetHomeRotation(rotation);
    }

    if (pTargetCmd->mHomecrane)
    {
        SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pTargetCmd->mHomecrane));
        pBasicCmd->mCrane->SetHomeCrane(pPointer->mCrane);
    }
}

MATCH_FUNC(0x5043a0)
void ScriptThread::SCRCMD_CONVEYOR_DECSET1_2(SCR_CONVEYOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mRect.mPos.z == kFp255)
    {
        pCmd->mRect.mPos.z = gMap_0x370->FindGroundZForCoord(pCmd->mRect.mPos.x,
                                                                                                 pCmd->mRect.mPos.y);
    }
    pPointer->mObj = gObject_5C->NewTouchPoint(objects::conveyor_139,
                                                                    pCmd->mRect.mPos.x,
                                                                    pCmd->mRect.mPos.y,
                                                                    pCmd->mRect.mPos.z,
                                                                    kAngZero_6F804C,
                                                                    pCmd->mRect.mSize.mX,
                                                                    pCmd->mRect.mSize.mY,
                                                                    kFpOne_6F77C4);

    pPointer->mObj->SetConveyorSpeed(pCmd->mSpeed.mX, pCmd->mSpeed.mY);
}

MATCH_FUNC(0x504420)
void ScriptThread::SCRCMD_GENERATOR_DECSET(SCR_GENERATOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370->FindGroundZForCoord(pCmd->mPos.x, pCmd->mPos.y);
    }

    Ang16 rotation = kDegreesToAng16.MultiplyByFix16(Fix16(static_cast<u16>(pCmd->mRot.rValue)));

    pPointer->mGenerator = gGeneratorPool_14AC->CreateGenerator(pCmd->mPos.x,
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
void ScriptThread::SCRCMD_DESTRUCTOR_DECSET(SCR_DESTRUCTOR* pCmd, SCR_POINTER* pPointer)
{
    if (pCmd->mRect.mPos.z == kFp255)
    {
        pCmd->mRect.mPos.z = gMap_0x370->FindGroundZForCoord(pCmd->mRect.mPos.x,
                                                                                                 pCmd->mRect.mPos.y);
    }

    pPointer->mObj = gObject_5C->NewTouchPoint(objects::destructor_141,
                                                                    pCmd->mRect.mPos.x,
                                                                    pCmd->mRect.mPos.y,
                                                                    pCmd->mRect.mPos.z,
                                                                    kAngZero_6F804C,
                                                                    pCmd->mRect.mSize.mX,
                                                                    pCmd->mRect.mSize.mY,
                                                                    kFpOne_6F77C4);
}

MATCH_FUNC(0x5045a0)
void ScriptThread::Handle_CRUSHER_BASIC(SCR_CRUSHER_BASIC* pCmd, SCR_POINTER* pPointer)
{
    pPointer->mCrusher = gCrusherPool_94->CreateCrusher(pCmd->mPos.mX, pCmd->mPos.mY);
}

MATCH_FUNC(0x5045d0)
void ScriptThread::Handle_THREAD_DECLARE2(SCR_THREAD* pCmd, s16* pThreadIdx)
{
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCharVarIdx));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCarOrPhoneIdx));

    if (pCharPointer->mChar && pCarPointer->mCar)
    {
        gScriptManager->AddCarThread(pCharPointer->mChar->get_id(), pCarPointer->mCar->mCarId, *pThreadIdx);
        pCarPointer->mCar->SetCarThreadPending();
    }
}

MATCH_FUNC(0x504660)
void ScriptThread::Handle_THREAD_DECLARE3(SCR_THREAD* pCmd)
{
    if (static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCharIdx10))->mChar)
    {
        pCmd->mTriggerIdx = gCarThreadTriggers_214->AddThreadTrigger(car_thread_trigger_type::char_on_foot_4,
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
void ScriptThread::Handle_THREAD_DECLARE5(SCR_CHAR_AREA_ANY* pCmd)
{
    if (static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCharIdx24))->mChar)
    {
        pCmd->mTriggerAdded = 1;

        switch (pCmd->mType)
        {
            case SCRCMD_THREAD_DECLARE5:
                pCmd->mTriggerIdx = gCarThreadTriggers_214->AddThreadTrigger(car_thread_trigger_type::char_on_foot_4,
                                                                    car_thread_trigger_action::spawn_thread_2,
                                                                    reinterpret_cast<SCR_THREAD*>(pCmd),
                                                                    pCmd->mX,
                                                                    pCmd->mY,
                                                                    pCmd->mZ,
                                                                    pCmd->mW,
                                                                    pCmd->mH);

                break;
            case SCRCMD_CHAR_AREA_ANY_MEANS:
                pCmd->mTriggerIdx = gCarThreadTriggers_214->AddThreadTrigger(car_thread_trigger_type::char_any_means_3,
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
void ScriptThread::Handle_THREAD_DECLARE4(SCR_THREAD* pCmd, s16* pThreadIdx)
{
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCharVarIdx));
    SCR_POINTER* pPhonePointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCarOrPhoneIdx));

    if (pCharPointer->mChar && pPhonePointer->mCar)
    {
        pPhonePointer->mObj->PoolGiveAndMarkDone();
        gScriptManager->AddPhoneThread(pCharPointer->mChar->get_id(), pPhonePointer->mObj->mId, *pThreadIdx);
    }
}

MATCH_FUNC(0x504830)
void ScriptThread::ApplyGangInfo(SCR_SET_GANG_INFO* pCmd)
{
    str_table_entry* pGangStr = gScriptManager->FindStringById(pCmd->mGangname);

    Gang* pGang = gGangPool_CA8->FindGangByName(pGangStr->get_name());
    pGang->SetRemap(pCmd->mRemap);

    pGang->SetBasicWeapon(pCmd->mWeapon1);
    pGang->SetAngryWeapon(pCmd->mWeapon2);
    pGang->SetHateWeapon(pCmd->mWeapon3);

    pGang->SetArrowColour(pCmd->mArrowId);
    pGang->SetGangCarModel(pCmd->mCarModel);
    pGang->SetGangCarRemap(static_cast<char_type>(pCmd->mCarRemap));

    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370->FindGroundZForCoord(pCmd->mPos.x, pCmd->mPos.y);
    }

    pGang->SetInfoPhonePos(pCmd->mPos.x, pCmd->mPos.y, pCmd->mPos.z);

    pGang->SetKillRespectChange(pCmd->mKillRespectChange);
    gGangPool_CA8->SwapGangSlots(pGang, gScriptManager->mNextGangIdx);
    ++gScriptManager->mNextGangIdx;
    Gang* pMovedGang = gGangPool_CA8->FindGangByName(pGangStr->get_name());

    if (pCmd->mKillRespectChange > 0)
    {
        gHud_2B00->mArrows.SetNewGangArrow(pMovedGang);
    }
}

MATCH_FUNC(0x504950)
void ScriptThread::Handle_SET_DOOR_INFO(SCR_FOUR_PARAMS* pCmd)
{
    gDoor_4D4->RegisterDoorInfo(pCmd->mUnsigned1, pCmd->mUnsigned2, static_cast<char_type>(pCmd->mUnsigned3));
}

MATCH_FUNC(0x504970)
void ScriptThread::SCRCMD_DOOR_DECLARE_D1_S1(SCR_DOOR_DATA_DEC* pCmd)
{
    if (pCmd->mType == SCRCMD_DOOR_DECLARE_S1)
    {
        if (pCmd->mCheck.mSize.mX == kFpZero_6F77C0 && pCmd->mCheck.mSize.mY == kFpZero_6F77C0)
        {
            pCmd->mDoor = gDoor_4D4->RegisterSingleDoorNoCheck(pCmd->mGrId,
                                                                               pCmd->mBlock.mX,
                                                                               pCmd->mBlock.mY,
                                                                               pCmd->mBlock.mZ,
                                                                               pCmd->mFace,
                                                                               pCmd->mFlip,
                                                                               pCmd->mReversed);
        }
        else
        {
            pCmd->mDoor = gDoor_4D4->RegisterSingleDoor(pCmd->mGrId,
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
            pCmd->mDoor = gDoor_4D4->RegisterDoubleDoorNoCheck(pCmd->mGrId,
                                                                               pCmd->mBlock.mX,
                                                                               pCmd->mBlock.mY,
                                                                               pCmd->mBlock.mZ,
                                                                               pCmd->mFace,
                                                                               pCmd->mFlip,
                                                                               pCmd->mReversed);
        }
        else
        {
            pCmd->mDoor = gDoor_4D4->RegisterDoubleDoor(pCmd->mGrId,
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
            pCmd->mDoor->SetOpenType(door_open_type::any_player);
            break;
        case SCR_DOOR_OPENTYPES::any_car:
            pCmd->mDoor->SetOpenType(door_open_type::any_car);
            break;
        case SCR_DOOR_OPENTYPES::none:
            pCmd->mDoor->SetOpenType(door_open_type::none);
            break;
        case SCR_DOOR_OPENTYPES::one_car:
            pCmd->mDoor->SetOpenType(door_open_type::one_car);
            break;
        case SCR_DOOR_OPENTYPES::one_char_on_foot:
            pCmd->mDoor->SetOpenType(door_open_type::one_char_on_foot);
            break;
        case SCR_DOOR_OPENTYPES::any_player_one_car:
            pCmd->mDoor->SetOpenType(door_open_type::any_player_one_car);
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
void ScriptThread::SCRCMD_DOOR_DECLARE_D2_S2(SCR_DOOR_DATA_DEC* pCmd)
{
    if (pCmd->mType == SCRCMD_DOOR_DECLARE_S2)
    {
        if (pCmd->mCheck.mSize.mX == kFpZero_6F77C0 && pCmd->mCheck.mSize.mY == kFpZero_6F77C0)
        {
            pCmd->mDoor = gDoor_4D4->RegisterSingleDoorNoCheck(pCmd->mGrId,
                                                                               pCmd->mBlock.mX,
                                                                               pCmd->mBlock.mY,
                                                                               pCmd->mBlock.mZ,
                                                                               pCmd->mFace,
                                                                               pCmd->mFlip,
                                                                               pCmd->mReversed);
        }
        else
        {
            pCmd->mDoor = gDoor_4D4->RegisterSingleDoor(pCmd->mGrId,
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
            pCmd->mDoor = gDoor_4D4->RegisterDoubleDoorNoCheck(pCmd->mGrId,
                                                                               pCmd->mBlock.mX,
                                                                               pCmd->mBlock.mY,
                                                                               pCmd->mBlock.mZ,
                                                                               pCmd->mFace,
                                                                               pCmd->mFlip,
                                                                               pCmd->mReversed);
        }
        else
        {
            pCmd->mDoor = gDoor_4D4->RegisterDoubleDoor(pCmd->mGrId,
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
            pCmd->mDoor->SetOpenType(door_open_type::any_player);
            break;
        case SCR_DOOR_OPENTYPES::any_car:
            pCmd->mDoor->SetOpenType(door_open_type::any_car);
            break;
        case SCR_DOOR_OPENTYPES::one_car:
        case SCR_DOOR_OPENTYPES::any_player_one_car:
        {
            SCR_POINTER* pTargetPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mOpenTarget));
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
            SCR_POINTER* pTargetPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mOpenTarget));
            Ped* pPed = pTargetPointer->mChar;
            if (pPed != NULL)
            {
                pCmd->mDoor->set_open_details_ped(door_open_type::one_char_on_foot, pPed);
                pCmd->mDoor->set_target_id(pTargetPointer->mChar->mId);
            }
        }
        break;
        case SCR_DOOR_OPENTYPES::none:
            pCmd->mDoor->SetOpenType(door_open_type::none);
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
void ScriptThread::Handle_DECLARE_MISSION(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager->GetBasePointer(pCmd->mUnsigned1); //  player pointer not used because missions are singleplayer
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));
    if (pCmd->mType == SCRCMD_DEC_GANG_1_FLAG)
    {
        gScriptManager->mGang1MissionFlag = &pPointer->mCounter;
    }
    else if (pCmd->mType == SCRCMD_DEC_GANG_2_FLAG)
    {
        gScriptManager->mGang2MissionFlag = &pPointer->mCounter;
    }
    else if (pCmd->mType == SCRCMD_DEC_GANG_3_FLAG)
    {
        gScriptManager->mGang3MissionFlag = &pPointer->mCounter;
    }
    else // pCmd->mType == SCRCMD_DECLARE_MISSION
    {
        gScriptManager->mMissionFlag = &pPointer->mCounter;
    }
}

MATCH_FUNC(0x504ee0)
void ScriptThread::CreateLight(SCR_CREATE_LIGHT* pCmd, SCR_POINTER* pPointer)
{
    Light* pNewLight = gLight_1D4CC->Init_469010(pCmd->mXpos,
                                                                       pCmd->mYpos,
                                                                       pCmd->mZpos,
                                                                       pCmd->mArgb,
                                                                       pCmd->mRadius,
                                                                       pCmd->mIntensity);
    pPointer->mLight = pNewLight;
    if (pCmd->mOnTime > 0)
    {
        gLight_1D4CC->SetFlashing(pNewLight, pCmd->mOnTime, pCmd->mOffTime, pCmd->mShape);
    }
}

MATCH_FUNC(0x505030)
void ScriptThread::SCRCMD_SET_STATION_EMPTY_STATION(SCR_SET_STATION* pCmd)
{
    if (!bSkip_trains)
    {
        str_table_entry* pStationStr = gScriptManager->FindStringById(pCmd->mPlatform);
        gmp_map_zone* station_zone = gMap_0x370->FindZoneByName(pStationStr->get_name());
        TrainStation* pStation = gPublicTransport_181C->TrainStationForZone(station_zone);

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
void ScriptThread::Handle_RADIOSTATION_DEC(SCR_RADIOSTATION_DEC* pCmd)
{
    gRoot_sound.DeclareRadioStation(pCmd->mStationType, pCmd->mPos.mX, pCmd->mPos.mY);
}

MATCH_FUNC(0x505210)
void ScriptThread::SCRCMD_SET_TRAIN_STATIONS(SCR_TWO_PARAMS* pCmd)
{
    if (!bSkip_trains)
    {
        str_table_entry* pStationStr = gScriptManager->FindStringById(pCmd->mUnsigned1);
        const char_type* station_zone_name = pStationStr->get_name();
        gmp_map_zone* station_zone = gMap_0x370->FindZoneByName(station_zone_name);
        TrainStation* pTrainStation = gPublicTransport_181C->TrainStationForZone(station_zone);
        pTrainStation->CalculateWagonCount(reinterpret_cast<u8*>(&pCmd->mUnsigned2)); //  The parameter may be the train wagon array, not sure
    }
}

MATCH_FUNC(0x5052c0)
void ScriptThread::Handle_DECLARE_POLICE(SCR_DECLARE_POLICELEVEL* pCmd)
{
    u8 max_wanted_level = static_cast<u8>(pCmd->mWantedLevel);
    if (max_wanted_level == 0)
    {
        bSkip_police = false;
    }
    gPolice_7B8->mMaxWantedStars = static_cast<u8>(pCmd->mWantedLevel); // max_wanted_level
}

MATCH_FUNC(0x505340)
void ScriptThread::Handle_SOUND_DECSET(SCR_SOUND_DECSET* pCmd, SCR_POINTER* pPointer)
{
    if (!pCmd->mPlayType)
    {
        pPointer->mObj = gObject_5C->NewWithPedRefIdx(objects::sound_object_type_1_278,
                                                              pCmd->mSoundId,
                                                              pCmd->mPos.x,
                                                              pCmd->mPos.y,
                                                              pCmd->mPos.z);
    }
    else
    {
        pPointer->mObj = gObject_5C->NewWithPedRefIdx(objects::sound_object_type_2_279,
                                                              pCmd->mSoundId,
                                                              pCmd->mPos.x,
                                                              pCmd->mPos.y,
                                                              pCmd->mPos.z);
    }
}

MATCH_FUNC(0x505430)
void ScriptThread::SCRCMD_SET_MISSIONS_TOTAL(SCR_TWO_PARAMS* pCmd)
{
    switch (pCmd->mType)
    {
        case SCRCMD_TOTAL_MISSIONS:
            gScriptManager->mTotalMissions = pCmd->mSigned2;
            break;
        case SCRCMD_TOTAL_SECRETS:
            gScriptManager->mTotalSecrets = pCmd->mSigned2;
            break;
        case SCRCMD_GANG_1_MISSION_TOTAL:
            gScriptManager->mGang1MissionsTotal = pCmd->mSigned2;
            break;
        case SCRCMD_GANG_2_MISSION_TOTAL:
            gScriptManager->mGang2MissionsTotal = pCmd->mSigned2;
            break;
        case SCRCMD_GANG_3_MISSION_TOTAL:
            gScriptManager->mGang3MissionsTotal = pCmd->mSigned2;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505580)
void ScriptThread::SCRCMD_PASSED_FAILED_FLAGS(SCR_TWO_PARAMS* pCmd)
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned1));

    switch (pCmd->mType)
    {
        case SCRCMD_PASSED_FLAG:
            gScriptManager->mPassedFlag = &pPointer->mCounter;
            break;
        case SCRCMD_1_PASSED_FLAG:
            gScriptManager->m1PassedFlag = &pPointer->mCounter;
            break;
        case SCRCMD_2_PASSED_FLAG:
            gScriptManager->m2PassedFlag = &pPointer->mCounter;
            break;
        case SCRCMD_3_PASSED_FLAG:
            gScriptManager->m3PassedFlag = &pPointer->mCounter;
            break;
        case SCRCMD_SECRETS_PASSED:
            gScriptManager->mSecretsPassed = &pPointer->mCounter;
            break;
        case SCRCMD_SECRETS_FAILED:
            gScriptManager->mSecretsFailed = &pPointer->mCounter;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505710)
void ScriptThread::Handle_FINISH_SCORE(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager->mFinishScore = pCmd->mS32;
}

MATCH_FUNC(0x505750)
void ScriptThread::Handle_DECLARE_CARLIST(SCR_TWO_PARAMS* pCmd)
{
    gScriptManager->mCarList = &pCmd->mU32;
}

MATCH_FUNC(0x505790)
void ScriptThread::DisableThread(u16 idx)
{
    SCR_CMD_HEADER* pCmd = gScriptManager->GetBasePointer(idx);

    switch (pCmd->mType)
    {
        case SCRCMD_THREAD_DECLARE4:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);
            SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pThread->mCharVarIdx));
            SCR_POINTER* pPhonePointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pThread->mCarOrPhoneIdx));

            if (pPhonePointer->mObj->IsAnimatedBehavior() && pPhonePointer->mObj->mTouchedResult != objects::dead_phone_174)
            {
                pPhonePointer->mObj->PoolGiveAndMarkDone();
            }

            gScriptManager->RemovePhoneThread(pCharPointer->mChar->get_id(), pPhonePointer->mObj->mId);
            break;
        }
        case SCRCMD_THREAD_DECLARE3:
            gCarThreadTriggers_214->SetTriggerEnableState(static_cast<SCR_THREAD*>(pCmd)->mTriggerIdx, car_thread_trigger_state::disabled_2);
            break;
        case SCRCMD_THREAD_DECLARE5:
        case SCRCMD_CHAR_AREA_ANY_MEANS:
            gCarThreadTriggers_214->SetTriggerEnableState(static_cast<SCR_CHAR_AREA_ANY*>(pCmd)->mTriggerIdx, car_thread_trigger_state::disabled_2);
            break;
        case SCRCMD_THREAD_DECLARE2:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);
            SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pThread->mCharVarIdx));
            SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pThread->mCarOrPhoneIdx));

            s32 char_id = pCharPointer->mChar->get_id();
            ThreadTrigger* pTrigger = gScriptManager->FindCarThread(char_id, pCarPointer->mCar->mCarId);

            if (pTrigger)
            {
                pTrigger->mPedId = 0;
                pTrigger->mTargetId = 0;
                pTrigger->mCmdLine = 0;
                --gScriptManager->mCarTriggerCount;
            }

            pCarPointer->mCar->ClearCarThreadPending();
            break;
        }
    }
}

MATCH_FUNC(0x505b10)
void ScriptThread::DeallocOrDeleteItem(u16 idx)
{
    SCR_POINTER* pCarCmdPointer;
    SCR_POINTER* pPedCmdPointer;
    SCR_POINTER* pObjCmdPointer;
    SCR_POINTER* pLightCmdPointer;
    SCR_DELETE_SOUND* pSoundCmdPointer;

    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(idx));
    switch (ScriptThread::GetEntityTypeOfCommand(pPointer->mType))
    {
        case script_entity_type::car_2:
            pCarCmdPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(idx));
            if (pCarCmdPointer->mCar)
            {
                gScriptManager->ClearWeaponHitFlag(pCarCmdPointer->mCar->mCarId, false);
                pCarCmdPointer->mCar->MarkForDespawn();
                pCarCmdPointer->mCar = NULL;
            }
            break;

        case script_entity_type::char_1:
            pPedCmdPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(idx));
            if (pPedCmdPointer->mChar)
            {
                pPedCmdPointer->mChar->Deallocate();
                pPedCmdPointer->mChar = NULL;
            }
            break;

        case script_entity_type::object_3:
            pObjCmdPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(idx));
            if (pObjCmdPointer->mObj)
            {
                pObjCmdPointer->mObj->Dealloc();
                pObjCmdPointer->mObj = NULL;
            }
            break;
        case script_entity_type::light_8:
            pLightCmdPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(idx));
            if (pLightCmdPointer->mLight)
            {
                gLight_1D4CC->DeallocLight(pLightCmdPointer->mLight);
                pLightCmdPointer->mLight = NULL;
            }
            break;

        case script_entity_type::radio_station_9:
            pSoundCmdPointer = static_cast<SCR_DELETE_SOUND*>(gScriptManager->GetBasePointer(idx));
            gRoot_sound.RemoveSound(pSoundCmdPointer->mXpos, pSoundCmdPointer->mYpos);
            break;

        case script_entity_type::sound_10:
            pObjCmdPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(idx));
            if (pObjCmdPointer->mObj)
            {
                pObjCmdPointer->mObj->Dealloc();
                pObjCmdPointer->mObj = NULL;
            }
            return;
        default:
            return;
    }
}

MATCH_FUNC(0x505ea0)
s16 ScriptThread::GetBonusResult(u16 idx)
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(idx));
    s16 result = pPointer->mCounter;
    if (result != bonus_result::passed && result != bonus_result::failed)
    {
        result = gGame_0x40->mLocalPlayer->mScores.mBonuses.GetBonusResult(result);
        if (result == bonus_result::passed || result == bonus_result::failed)
        {
            pPointer->mCounter = result;
        }
    }
    return result;
}

MATCH_FUNC(0x505f50)
void ScriptThread::SCRCMD_DISPLAY_MESSAGES(SCR_TWO_PARAMS* pCmd)
{
    if (gDo_text_id_test)
    {
        sprintf(gTmpBuffer, "%d", pCmd->mUnsigned1);
        if (!gText_0x14->Find(gTmpBuffer))
        {
            FatalError_4A38C0(Gta2Error::MissionScriptIsReferringToTextIdThatDoesNotExist,
                              "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                              2460,
                              pCmd->mUnsigned1); // Mission script is referring to a 'text id' that doesn't exist. ID
        }
    }
}

MATCH_FUNC(0x506010)
void ScriptThread::SCRCMD_DEC_DEATH_BASE(SCR_FOUR_PARAMS* pCmd)
{
    if (pCmd->mType == SCRCMD_DEC_DEATH_BASE_1)
    {
        gScriptManager->mGang1DeathBase = pCmd->mUnsigned3;
    }
    else if (pCmd->mType == SCRCMD_DEC_DEATH_BASE_2)
    {
        gScriptManager->mGang2DeathBase = pCmd->mUnsigned3;
    }
    else if (pCmd->mType == SCRCMD_DEC_DEATH_BASE_3)
    {
        gScriptManager->mGang3DeathBase = pCmd->mUnsigned3;
    }
}

MATCH_FUNC(0x5060d0)
void ScriptThread::Handle_DO_CRANE_POWERUP(SCR_DECLARE_CRANE_POWERUP* pCmd)
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCraneIdx));
    pPointer->mCrane->mPowerupCmd = pCmd->mCmdThis;
}

MATCH_FUNC(0x506140)
void ScriptThread::RegisterBasicKf(u16* pKfValue)
{
    for (u16 idx = 0; idx < 25; idx++)
    {
        if (gScriptManager->mBasicKf[idx] == 0)
        {
            gScriptManager->mBasicKf[idx] = *pKfValue;
            break;
        }
    }
}

MATCH_FUNC(0x5061c0)
void ScriptThread::ExecOpCode()
{
    for (u16 i = 1; i < 6000u; ++i)
    {
        SCR_CMD_HEADER* pBasePtr = gScriptManager->GetBasePointer(i);
        if (pBasePtr)
        {
            switch (pBasePtr->mType)
            {
                case SCRCMD_PLAYER_PED:
                    ScriptThread::Handle_PLAYER_PED(reinterpret_cast<SCR_PLAYER_PED*>(pBasePtr));
                    break;
                case SCRCMD_CAR_DECSET_2D:
                case SCRCMD_CAR_DECSET_3D:
                case SCRCMD_CAR_DECSET_2D_STR:
                case SCRCMD_CAR_DECSET_3D_STR:
                    ScriptThread::SCRCMD_CAR_DECSET(static_cast<SCR_CAR_DATA_DEC*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_CHAR_DECSET_2D:
                case SCRCMD_CHAR_DECSET_3D:
                    ScriptThread::SCRCMD_CHAR_DECSET_2D_3D(static_cast<SCR_CHAR_DATA_DEC*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_OBJ_DECSET_2D:
                case SCRCMD_OBJ_DECSET_3D:
                    ScriptThread::SCRCMD_OBJ_DECSET_2D_3D(static_cast<SCR_OBJ_DATA*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_OBJ_DECSET_2D_INT:
                case SCRCMD_OBJ_DECSET_3D_INT:
                case SCRCMD_OBJ_DECSET_2D_STR:
                case SCRCMD_OBJ_DECSET_3D_STR:
                    ScriptThread::SCRCMD_OBJ_DECSET(static_cast<SCR_OBJ_DATA*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_MAP_ZONE_SET:
                    ScriptThread::Handle_MAP_ZONE_SET(static_cast<SCR_MAP_ZONE_SET*>(pBasePtr));
                    break;
                case SCRCMD_ARROW_DEC:
                    ScriptThread::Handle_ARROW_DEC(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_CRANE_BASIC_DEC:
                case SCRCMD_CRANE_TARGET_DEC:
                case SCRCMD_CRANE2TARGET_DEC:
                    ScriptThread::SCRCMD_CRANE(static_cast<SCR_CRANE_TARGET_DEC*>(pBasePtr), static_cast<SCR_CRANE_BASIC_DEC*>(pBasePtr));
                    break;
                case SCRCMD_CONVEYOR_DECSET1:
                case SCRCMD_CONVEYOR_DECSET2:
                    ScriptThread::SCRCMD_CONVEYOR_DECSET1_2(static_cast<SCR_CONVEYOR*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_GENERATOR_DECSET1:
                case SCRCMD_GENERATOR_DECSET2:
                case SCRCMD_GENERATOR_DECSET3:
                case SCRCMD_GENERATOR_DECSET4:
                    ScriptThread::SCRCMD_GENERATOR_DECSET(static_cast<SCR_GENERATOR*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_DESTRUCTOR_DECSET1:
                case SCRCMD_DESTRUCTOR_DECSET2:
                    ScriptThread::SCRCMD_DESTRUCTOR_DECSET(static_cast<SCR_DESTRUCTOR*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_THREAD_DECLARE2:
                    ScriptThread::Handle_THREAD_DECLARE2(static_cast<SCR_THREAD*>(pBasePtr), &pBasePtr->mCmdThis);
                    break;
                case SCRCMD_THREAD_DECLARE4:
                    ScriptThread::Handle_THREAD_DECLARE4(static_cast<SCR_THREAD*>(pBasePtr), &pBasePtr->mCmdThis);
                    break;
                case SCRCMD_THREAD_DECLARE3:
                    ScriptThread::Handle_THREAD_DECLARE3(static_cast<SCR_THREAD*>(pBasePtr));
                    break;
                case SCRCMD_SET_GANG_INFO1:
                    ScriptThread::ApplyGangInfo(static_cast<SCR_SET_GANG_INFO*>(pBasePtr));
                    break;
                case SCRCMD_SET_DOOR_INFO:
                    ScriptThread::Handle_SET_DOOR_INFO(static_cast<SCR_FOUR_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_CRUSHER_BASIC:
                    ScriptThread::Handle_CRUSHER_BASIC(static_cast<SCR_CRUSHER_BASIC*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_CHAR_AREA_ANY_MEANS:
                case SCRCMD_THREAD_DECLARE5:
                    ScriptThread::Handle_THREAD_DECLARE5(static_cast<SCR_CHAR_AREA_ANY*>(pBasePtr));
                    break;
                case SCRCMD_DOOR_DECLARE_S1:
                case SCRCMD_DOOR_DECLARE_D1:
                    ScriptThread::SCRCMD_DOOR_DECLARE_D1_S1(static_cast<SCR_DOOR_DATA_DEC*>(pBasePtr));
                    break;
                case SCRCMD_DOOR_DECLARE_S2:
                case SCRCMD_DOOR_DECLARE_D2:
                    ScriptThread::SCRCMD_DOOR_DECLARE_D2_S2(static_cast<SCR_DOOR_DATA_DEC*>(pBasePtr));
                    break;
                case SCRCMD_DEC_GANG_1_FLAG:
                case SCRCMD_DEC_GANG_2_FLAG:
                case SCRCMD_DEC_GANG_3_FLAG:
                case SCRCMD_DECLARE_MISSION:
                    ScriptThread::Handle_DECLARE_MISSION(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_CREATE_LIGHT2:
                case SCRCMD_LIGHT_DEC:
                    ScriptThread::CreateLight(static_cast<SCR_CREATE_LIGHT*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_SET_STATION:
                case SCRCMD_SET_EMPTY_STATION:
                    ScriptThread::SCRCMD_SET_STATION_EMPTY_STATION(static_cast<SCR_SET_STATION*>(pBasePtr));
                    break;
                case SCRCMD_RADIOSTATION_DEC:
                    ScriptThread::Handle_RADIOSTATION_DEC(static_cast<SCR_RADIOSTATION_DEC*>(pBasePtr));
                    break;
                case SCRCMD_SET_STATION_4:
                case SCRCMD_SET_STATION_3:
                case SCRCMD_SET_STATION_2:
                case SCRCMD_SET_STATION_1:
                    ScriptThread::SCRCMD_SET_TRAIN_STATIONS(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_DECLARE_POLICE:
                    ScriptThread::Handle_DECLARE_POLICE(static_cast<SCR_DECLARE_POLICELEVEL*>(pBasePtr));
                    break;
                case SCRCMD_SOUND_DECSET:
                    ScriptThread::Handle_SOUND_DECSET(static_cast<SCR_SOUND_DECSET*>(pBasePtr), static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_FINISH_SCORE:
                    ScriptThread::Handle_FINISH_SCORE(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_TOTAL_MISSIONS:
                case SCRCMD_TOTAL_SECRETS:
                case SCRCMD_GANG_1_MISSION_TOTAL:
                case SCRCMD_GANG_2_MISSION_TOTAL:
                case SCRCMD_GANG_3_MISSION_TOTAL:
                    ScriptThread::SCRCMD_SET_MISSIONS_TOTAL(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_PASSED_FLAG:
                case SCRCMD_1_PASSED_FLAG:
                case SCRCMD_2_PASSED_FLAG:
                case SCRCMD_3_PASSED_FLAG:
                case SCRCMD_SECRETS_PASSED:
                case SCRCMD_SECRETS_FAILED:
                    ScriptThread::SCRCMD_PASSED_FAILED_FLAGS(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_DECLARE_CARLIST:
                    ScriptThread::Handle_DECLARE_CARLIST(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_DISPLAY_MESSAGE:
                case SCRCMD_DISPLAY_BRIEF:
                case SCRCMD_DISPLAY_BRIEF_NOW:
                case SCRCMD_DISPLAY_BRIEF_SOON:
                    ScriptThread::SCRCMD_DISPLAY_MESSAGES(static_cast<SCR_TWO_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_PARKED_CAR_DECSET_2D:
                case SCRCMD_PARKED_CAR_DECSET_3D:
                case SCRCMD_PARKED_CAR_DECSET_2D_STR:
                case SCRCMD_PARKED_CAR_DECSET_3D_STR:
                    ScriptThread::SCRCMD_PARKED_CAR_DECSET(static_cast<SCR_POINTER*>(pBasePtr));
                    break;
                case SCRCMD_DEC_DEATH_BASE_1:
                case SCRCMD_DEC_DEATH_BASE_2:
                case SCRCMD_DEC_DEATH_BASE_3:
                    ScriptThread::SCRCMD_DEC_DEATH_BASE(static_cast<SCR_FOUR_PARAMS*>(pBasePtr));
                    break;
                case SCRCMD_DO_CRANE_POWERUP:
                    ScriptThread::Handle_DO_CRANE_POWERUP(static_cast<SCR_DECLARE_CRANE_POWERUP*>(pBasePtr));
                    break;
                case SCRCMD_START_BASIC_KF:
                    ScriptThread::RegisterBasicKf(reinterpret_cast<u16*>(&pBasePtr->mCmdThis));
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
    gScriptManager->RestoreAnsweredPhones();
    ScriptThread::DisableBasicKfsFromSave();
    gGeneratorPool_14AC->FinalizeAfterScriptLoad();
}

MATCH_FUNC(0x5069c0)
void ScriptThread::Handle_LEVELSTART()
{
    gRoot_sound.PlayVoice(voice_line::respectis_24); //  "And remember, respect is everything!"
    mInLevelStart = true;
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5069f0)
void ScriptThread::Handle_LEVELEND()
{
    mInLevelStart = false;
    ScriptThread::EndCmd();
}

MATCH_FUNC(0x506a00)
void ScriptThread::Handle_CREATE_THREAD()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_THREAD* pThread = static_cast<SCR_THREAD*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    ScriptThread* pScriptThread = ScriptThread::SpawnThread(pCmd->mUnsigned1);
    pThread->mScriptThread = pScriptThread;
    pThread->mThreadId = pScriptThread->mThreadId;
    pScriptThread->InitThread(mFlags, pCmd->mUnsigned1);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x506a60)
void ScriptThread::Handle_STOP_THREAD()
{
    SCR_THREAD* pThread = static_cast<SCR_THREAD*>(
        gScriptManager->GetBasePointer(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mUnsigned2));
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
        pScriptThread->StopThread();
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x506af0)
void ScriptThread::Handle_IF_JUMP()
{
    //  mCondResult = 1  means the boolean opcode has returned true
    //  mCondResult = 0  means the boolean opcode has returned false

    //  If it's a OR and boolean is true, or if it's a AND and boolean is false, jump
    SCR_IF_JUMP* pCmd = reinterpret_cast<SCR_IF_JUMP*>(gBasePtr);

    if ((pCmd->is_or == 1 && mCondResult) || (!pCmd->is_or && !mCondResult))
    {

        JumpToCmd(pCmd->else_endif_pointer); //  Jump to the last IF_JUMP or go to ELSE section
        return;
    }

    Next(gBasePtr); // go to mCmdNext
}

MATCH_FUNC(0x506b30)
void ScriptThread::Handle_GOSUB()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    ScriptStackFrame* pFrame = mGosubStack->AllocFrame();
    pFrame->mCondResult = mCondResult;
    pFrame->mReturnCmd = gBasePtr->mCmdNext;
    mGosubStack->add_503160(pFrame);
    ScriptThread::JumpToCmd(pCmd->mUnsigned1);
}

MATCH_FUNC(0x506b80)
void ScriptThread::Handle_RETURN() // MISSIONEND
{
    ScriptStackFrame* pFrame = mGosubStack->remove_503180();

    if (pFrame == NULL)
    {
        ScriptThread::EndCmd();
    }
    else
    {
        ScriptThread::JumpToCmd(pFrame->mReturnCmd);
        mCondResult = pFrame->mCondResult;
        mGosubStack->FreeFrame(pFrame);
    }
}

MATCH_FUNC(0x506bc0)
u8 ScriptThread::GetOperationType(u32 opcode)
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
void ScriptThread::SCRCMD_S_OP_I()
{
    SCR_OPERATE_COUNTER_AND_INT* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_INT*>(gBasePtr);
    u8 operation_type = ScriptThread::GetOperationType(pCmd->mType);
    SCR_POINTER* pLeftOperand = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounterIdx));

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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x506ed0)
void ScriptThread::SCRCMD_S_OP_S()
{
    u8 operation_type = ScriptThread::GetOperationType(gBasePtr->mType);
    SCR_OPERATE_COUNTER_AND_COUNTER_2* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_COUNTER_2*>(gBasePtr);
    SCR_POINTER* pLeftOperand =
        static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex)); // pCmd->mLeftCounterIdx
    SCR_POINTER* pRightOperand = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mRightCounterIdx));
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x507110)
void ScriptThread::SCRCMD_I_OP_S()
{
    u8 operation_type = ScriptThread::GetOperationType(gBasePtr->mType);
    SCR_OPERATE_INT_AND_COUNTER* pCmd = reinterpret_cast<SCR_OPERATE_INT_AND_COUNTER*>(gBasePtr);
    SCR_POINTER* pOperand = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(
        reinterpret_cast<SCR_OPERATE_INT_AND_COUNTER*>(gBasePtr)->mCounterIdx));

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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x507750)
void ScriptThread::Handle_NOT()
{
    mCondResult = mCondResult == 0; //  just toggle the boolean value
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5078d0)
void ScriptThread::Handle_START_EXEC()
{
    s16 exec_flag = mExecFlag;

    //  Only increase EXEC flag if it wasn't set before
    if (exec_flag <= 0)
    {
        mExecFlag = exec_flag + 1; //  increase EXEC flag
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5079a0)
void ScriptThread::Handle_STOP_EXEC()
{
    s16 exec_flag = mExecFlag;

    //  Only decrease EXEC flag if it was set before
    if (exec_flag >= 0)
    {
        mExecFlag = exec_flag - 1; //  decrease EXEC flag
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x507a70)
void ScriptThread::Handle_INCREMENT()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    ++pPointer->mCounter; //  Increment Counter
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x507b50)
void ScriptThread::Handle_DECREMENT()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    --pPointer->mCounter; //  Decrement Counter
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x507ce0)
void ScriptThread::SCRCMD_CREATE_OBJ()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_OBJ_DATA* pObjData = reinterpret_cast<SCR_OBJ_DATA*>(gBasePtr);

    switch (pObjData->mType)
    {
        case SCRCMD_CREATE_OBJ_3D_INT:
        case SCRCMD_CREATE_OBJ_2D_STR:
            ScriptThread::SCRCMD_OBJ_DECSET_2D_3D(pObjData, pPointer);
            break;
        case SCRCMD_CREATE_OBJ_2D:
        case SCRCMD_CREATE_OBJ_3D:
        case SCRCMD_CREATE_OBJ_2D_INT:
        case SCRCMD_CREATE_OBJ_3D_STR:
            ScriptThread::SCRCMD_OBJ_DECSET(pObjData, pPointer);
            break;
        default:
            break;
    }
    if (!mInLevelStart)
    {
        gMiss2_25C->push_type_2_non_pickup_object(pPointer->mObj, false);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x507f80)
void ScriptThread::SCRCMD_CREATE_CAR()
{
    SCR_CAR_DATA_DEC* pCmd = reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr);

    Fix16* z_coord = &reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr)->mPos.z;

    if (*z_coord == kFp255)
    {
        *z_coord = gMap_0x370->FindGroundZForCoord(reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr)->mPos.x,
                                                                 reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr)->mPos.y);
    }
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCarIdx));
    ScriptThread::SCRCMD_CAR_DECSET(reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr), pPointer);
    if (!mInLevelStart)
    {
        gMiss2_25C->push_type_1_car(pPointer->mCar);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x508220)
void ScriptThread::Handle_MAKE_CAR_DUMMY()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (pPointer->mCar != NULL)
    {
        if (!pPointer->mCar->GetDriver())
        {
            pPointer->mCar->SpawnDriverPed();
        }
        pPointer->mCar->InitCarAIControl();

        pPointer->mCar->SetDriverPedType(ped_type::script_created_5);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x508280)
void ScriptThread::SCRCMD_SET_DENSITY()
{
    SCR_MAP_SET_ZONE_INFO* pCmd = reinterpret_cast<SCR_MAP_SET_ZONE_INFO*>(gBasePtr);
    str_table_entry* pStrTable = gScriptManager->FindStringById(pCmd->mZoneIdx);

    switch (gBasePtr->mType)
    {
        case SCRCMD_SET_CAR_DENSITY:
            gMap_0x370->SetZoneCarDensity(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_GOOD_CAR:
            gMap_0x370->SetZoneGoodCarRatio(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_BAD_CAR:
            gMap_0x370->SetZoneBadCarRatio(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_POLICE_CAR:
            gMap_0x370->SetZonePoliceCarRatio(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_PED_DENSITY:
            gMap_0x370->SetZonePedDensity(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_MUGGER:
            gMap_0x370->SetZoneMuggerRatio(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_CARTHIEF:
            gMap_0x370->SetZoneCarThiefRatio(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_ELVIS:
            gMap_0x370->SetZoneElvisRatio(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_GANG:
            gMap_0x370->SetZoneGangCharRatio(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_POLICE_PED:
            gMap_0x370->SetZonePolicePedRatio(pStrTable->mZoneIdx, pCmd->mValue);
            break;
        case SCRCMD_SET_GANGCARRATIO:
            gMap_0x370->SetZoneGangCarRatio(pStrTable->mZoneIdx, pCmd->mValue);
        default:
            break;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x508550)
void ScriptThread::Handle_POINT_ARROW_3D() //  SCRCMD_POINT_ARROW_3D and SCRCMD_LEVEL_END_ARROW2
{
    SCR_LEVEL_END_ARROW2* pCmd = reinterpret_cast<SCR_LEVEL_END_ARROW2*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mArrowname));
    if (pPointer->mArrow == NULL)
    {
        pPointer->mArrow = gHud_2B00->mArrows.AllocArrow();
    }

    pPointer->mArrow->SetArrowAim(pCmd->mPos.x, pCmd->mPos.y, pCmd->mPos.z);

    if (gBasePtr->mType == SCRCMD_LEVEL_END_ARROW2)
    {
        pPointer->mArrow->SetArrowColour(5);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5086f0)
void ScriptThread::Handle_POINT_ARROW_AT()
{
    SCR_POINT_ARROW_AT* pParams = reinterpret_cast<SCR_POINT_ARROW_AT*>(gBasePtr);
    SCR_POINT_ARROW_AT* pCmd = reinterpret_cast<SCR_POINT_ARROW_AT*>(gBasePtr);

    SCR_POINTER* pArrowPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pParams->mArrowIdx));
    SCR_ARROW_ENTITY* pEntityPtr = static_cast<SCR_ARROW_ENTITY*>(gScriptManager->GetBasePointer(pCmd->mTargetEntityIdx));

    if (pArrowPtr->mArrow == NULL)
    {
        pArrowPtr->mArrow = gHud_2B00->mArrows.AllocArrow();
    }

    switch (ScriptThread::GetEntityTypeOfCommand(pEntityPtr->mType))
    {
        case script_entity_type::car_2:
        {
            pArrowPtr->mArrow->SetArrowTargetCar(pEntityPtr->mCar);
            break;
        }

        case script_entity_type::char_1:
        {
            Ped* pChar = pEntityPtr->mChar;

            if (!pChar->is_player() || pChar->mPlayer->get_idx() != gGame_0x40->GetLocalPlayerIdx())
            {
                pArrowPtr->mArrow->SetArrowTargetPed(pChar);
            }

            break;
        }

        case script_entity_type::object_3:
        {
            pArrowPtr->mArrow->SetArrowTargetObject(pEntityPtr->mObj);
            break;
        }

        case script_entity_type::crane_4:
            pArrowPtr->mArrow->SetArrowAim(reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pEntityPtr)->mPos.mX,
                                                         reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pEntityPtr)->mPos.mY,
                                                         kFpOne_6F77C4);
            break;

        case script_entity_type::conveyor_5:
            pArrowPtr->mArrow->SetArrowAim(reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->mRect.mPos.x,
                                                         reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->mRect.mPos.y,
                                                         reinterpret_cast<SCR_CONVEYOR*>(pEntityPtr)->mRect.mPos.z);
            break;

        default:
            break;
    }

    if (gBasePtr->mType == SCRCMD_LEVEL_END_ARROW1)
    {
        pArrowPtr->mArrow->SetArrowColour(5);
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x508dc0)
void ScriptThread::Handle_ARROW_COLOUR()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pPointer->mArrow == NULL)
    {
        pPointer->mArrow = gHud_2B00->mArrows.AllocArrow();
    }
    pPointer->mArrow->SetArrowColour(pCmd->mUnsigned2);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x508e80)
void ScriptThread::Handle_REMOVE_ARROW()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Hud_Arrow* pArrow = pPointer->mArrow;

    if (pArrow != NULL)
    {
        pArrow->ClearTargets();
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x508f00)
void ScriptThread::Handle_CHECK_CAR_DAMAGE()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Car* pCar = pPointer->mCar;
    if (pCar->mDamage >= 320 * pCmd->mSigned2)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509030)
void ScriptThread::Handle_CHECK_HEALTH()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Ped* pPed = pPointer->mChar;
    s16 health_param = pCmd->mSigned2;

    if (pPed != NULL && pPed->get_health() >= health_param)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5093c0)
void ScriptThread::Handle_DISPLAY_MESSAGE()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    u8 type = static_cast<u8>(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mUnsigned2);
    if (type == 1)
    {
        gRoot_sound.PlayVoice(voice_line::jobcomplete_22);
    }
    else if (type == 2)
    {
        gRoot_sound.PlayVoice(voice_line::jobfail_23);
    }

    if (pCmd->mUnsigned1 == 5501)
    {
        gRoot_sound.PlayVoice(voice_line::raceover_33);
    }
    else if (pCmd->mUnsigned1 == 5502)
    {
        gRoot_sound.PlayVoice(voice_line::second_lap_34);
    }
    else if (pCmd->mUnsigned1 == 5503)
    {
        gRoot_sound.PlayVoice(voice_line::finallap_35);
    }
    else if (pCmd->mUnsigned1 == 5504)
    {
        gRoot_sound.PlayVoice(voice_line::raceon_36);
    }
    else if (pCmd->mUnsigned1 == 5505)
    {
        gRoot_sound.PlayVoice(voice_line::people_down_30_37);
    }
    else if (pCmd->mUnsigned1 == 5506)
    {
        gRoot_sound.PlayVoice(voice_line::people_down_60_38);
    }
    else if (pCmd->mUnsigned1 == 5507)
    {
        gRoot_sound.PlayVoice(voice_line::people_down_90_39);
    }
    else if (pCmd->mUnsigned1 == 5508)
    {
        gRoot_sound.PlayVoice(voice_line::people_down_120_40);
    }
    else if (pCmd->mUnsigned1 == 5509)
    {
        gRoot_sound.PlayVoice(voice_line::people_down_150_41);
    }
    else if (pCmd->mUnsigned1 == 5510)
    {
        gRoot_sound.PlayVoice(voice_line::timeout_42);
    }
    else if (pCmd->mUnsigned1 == 5000)
    {
        gRoot_sound.PlayVoice(voice_line::timeextended_43);
    }
    else if (pCmd->mUnsigned1 == 5015)
    {
        gRoot_sound.PlayVoice(voice_line::timesup_44);
    }
    else if (pCmd->mUnsigned1 == 5031)
    {
        gRoot_sound.PlayVoice(voice_line::sorrydidthathurt_45);
    }
    else if (pCmd->mUnsigned1 == 5032)
    {
        gRoot_sound.PlayVoice(voice_line::nicework_46);
    }
    else if (pCmd->mUnsigned1 == 5050)
    {
        gRoot_sound.PlayVoice(voice_line::choctastic_47);
    }
    else if (pCmd->mUnsigned1 == 5051)
    {
        gRoot_sound.PlayVoice(voice_line::raspberryripple_48);
    }
    else if (pCmd->mUnsigned1 == 5052)
    {
        gRoot_sound.PlayVoice(voice_line::youshotyourload_49);
    }
    else if (pCmd->mUnsigned1 == 5053)
    {
        gRoot_sound.PlayVoice(voice_line::oohdidthathurt_50);
    }
    else if (pCmd->mUnsigned1 == 5054)
    {
        gRoot_sound.PlayVoice(voice_line::deathtoicvans_51);
    }
    else if (pCmd->mUnsigned1 == 5055)
    {
        gRoot_sound.PlayVoice(voice_line::crispycritter_52);
    }
    else if (pCmd->mUnsigned1 == 5056)
    {
        gRoot_sound.PlayVoice(voice_line::youretoastbuddy_53);
    }
    else if (pCmd->mUnsigned1 == 5057)
    {
        gRoot_sound.PlayVoice(voice_line::eatleaddeath_54);
    }
    else if (pCmd->mUnsigned1 == 5058)
    {
        gRoot_sound.PlayVoice(voice_line::thatsgottahurt_55);
    }
    else if (pCmd->mUnsigned1 == 5059)
    {
        gRoot_sound.PlayVoice(voice_line::sorryaboutthat_56);
    }
    else if (pCmd->mUnsigned1 == 5060)
    {
        gRoot_sound.PlayVoice(voice_line::xinloimyman_57);
    }
    else if (pCmd->mUnsigned1 == 5061)
    {
        gRoot_sound.PlayVoice(voice_line::damnsundaydrivers_58);
    }
    else if (pCmd->mUnsigned1 == 5062)
    {
        gRoot_sound.PlayVoice(voice_line::suckitandsee_59);
    }
    else if (pCmd->mUnsigned1 == 5063)
    {
        gRoot_sound.PlayVoice(voice_line::tastemywrath_60);
    }

    sprintf(gTmpBuffer, "%d", pCmd->mUnsigned1);

    switch (pCmd->mType)
    {
        case SCRCMD_DISPLAY_MESSAGE:
            gHud_2B00->mMessage.ShowMessage(gText_0x14->Find(gTmpBuffer), hud_message_priority::important_3);
            break;
        case SCRCMD_DISPLAY_BRIEF:
            gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
            break;
        case SCRCMD_DISPLAY_BRIEF_NOW:
            gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::now_3, gTmpBuffer);
            break;
        case SCRCMD_DISPLAY_BRIEF_SOON:
            gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::soon_2, gTmpBuffer);
            break;
        default:
            break;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5096e0)
void ScriptThread::Handle_DISPLAY_TIMER()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    pPointer->mIndex = gHud_2B00->mPagers.CreateTimer(pCmd->mSigned2);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509730)
void ScriptThread::SCRCMD_CREATE_CHAR()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    ScriptThread::SCRCMD_CHAR_DECSET_2D_3D(reinterpret_cast<SCR_CHAR_DATA_DEC*>(gBasePtr), pPointer);
    if (!mInLevelStart)
    {
        gMiss2_25C->push_type_3_ped(pPointer->mChar);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509790)
void ScriptThread::Handle_SET_CHAR_OCCUPATION()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    pPointer->mChar->set_occupation(pCmd->mU32);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5097d0)
void ScriptThread::Handle_SET_CHAR_OBJ1()
{
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    ScriptThread::SetCharObjective(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr), pPointer);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509810)
void ScriptThread::Handle_IS_CHAR_OBJ_PASS()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509880)
void ScriptThread::Handle_IS_CHAR_OBJ_FAIL()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5098e0)
void ScriptThread::SCRCMD_GIVE_WEAPON()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_FOUR_PARAMS* pCmd4 = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (ScriptThread::GetEntityTypeOfCommand(pPointer->mType) == script_entity_type::char_1)
    {
        if (gBasePtr->mType == SCRCMD_GIVE_WEAPON1)
        {
            pPointer->mChar->AddWeaponWithAmmo(pCmd->mSigned2, 100);
        }
        else
        {
            pPointer->mChar->AddWeaponWithAmmo(pCmd4->mSigned2, static_cast<char_type>(pCmd4->mSigned3));
        }
    }
    else
    {
        // Always 50 ammo for cars
        gWeapon_8->allocate_5E3D50(pCmd->mSigned2, 50, pPointer->mCar);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509990)
void ScriptThread::Handle_IS_CAR_IN_BLOCK()
{
    SCR_IS_CAR_IN_BLOCK* pCmd = reinterpret_cast<SCR_IS_CAR_IN_BLOCK*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pCmd->mPos.z == kFp255)
    {
        pCmd->mPos.z =
            gMap_0x370->FindGroundZForCoord(pCmd->mPos.x, pCmd->mPos.y);
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509a70)
void ScriptThread::Handle_CAR_IN_AREA()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Fix16 height = pCmd->mRect.mSize.mY;
    Fix16 width = pCmd->mRect.mSize.mX;
    Fix16 top = pCmd->mRect.mPos.y - height / kFpTwo_6F77C8;
    Fix16 left = pCmd->mRect.mPos.x - width / kFpTwo_6F77C8;

    gScriptManager->mAreaRect.SetFromPosSize(left, top, width, height);

    Fix16 hz = kFpHalf_6F75F0 + pCmd->mRect.mPos.z;
    Fix16 lz = pCmd->mRect.mPos.z - kFpHalf_6F75F0;
    gScriptManager->mAreaRect.SetHiLowZ(lz, hz);

    if (pPointer->mCar != NULL && pPointer->mCar->mDespawnStatus != car_despawn_status::despawned_6 &&
        (pPointer->mCar->mCarSprite->IntersectsRectSAT(&gScriptManager->mAreaRect) ||
         gScriptManager->mAreaRect.IntersectsSpriteRenderingRect(pPointer->mCar->mCarSprite)))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509bb0)
void ScriptThread::Handle_HAS_CHAR_DIED()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (pPointer->mChar && pPointer->mChar->GetPedState() == ped_state_1::dead_9)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509c10)
void ScriptThread::Handle_IS_CHAR_IN_CAR()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    Ped* pPed = pCharPointer->mChar;

    Car* pCarTarget;

    if (pPed != NULL && (pCarTarget = pCarPointer->mCar) != NULL && pPed->has_car() &&
        pPed->get_car() == pCarTarget)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509c90)
void ScriptThread::Handle_IS_CHAR_IN_MODEL()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Ped* pPed = pCharPointer->mChar;

    if (pPed != NULL && pPed->has_car() && pPed->get_car()->GetCarInfoIdx() == pCmd->mSigned2)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509d00)
void ScriptThread::Handle_IS_CHAR_IN_ANY_CAR()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL && pPed->has_car())
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509d60)
void ScriptThread::Handle_DELETE_ITEM()
{
    ScriptThread::DeallocOrDeleteItem(gBasePtr->mIndex); //  delete item
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509d90)
void ScriptThread::SCRCMD_ADD_SCORE()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        if (pPed->is_player())
        {
            Player* pPlayer = pPed->mPlayer;
            if (gBasePtr->mType == SCRCMD_ADD_SCORE1)
            {
                pPlayer->AddScore(pCmd->mS32);
            }
            else
            {
                pPlayer->AddCash(pCmd->mS32); //  no multiplier
            }
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509e00)
void ScriptThread::Handle_ADD_SCORE2()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mS32));

    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        if (pPed->is_player())
        {
            pPed->mPlayer->AddScore(pCounter->mCounter);
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509e70)
void ScriptThread::Handle_IS_CHAR_STOPPED()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL && pPed->GetGameObjectVelocity() == kFpZero_6F77C0)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509ed0)
void ScriptThread::Handle_EXPLODE()
{
    SCR_EXPLODE* pCmd = reinterpret_cast<SCR_EXPLODE*>(gBasePtr);
    switch (gBasePtr->mType)
    {
        case SCRCMD_EXPLODE_NO_RING2:
            gObject_5C->CreateExplosion(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::no_ring_32,
                                                      0);
            break;
        case SCRCMD_EXPLODE_SMALL2:
            gObject_5C->CreateExplosion(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::small_18,
                                                      0);
            break;
        case SCRCMD_EXPLODE:
            gObject_5C->CreateExplosion(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::item_19,
                                                      0);
            break;
        case SCRCMD_EXPLODE_LARGE2:
            gObject_5C->CreateExplosion(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::large_20,
                                                      0);
            break;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509f60)
void ScriptThread::Handle_EXPLODE_BUILDING()
{
    SCR_EXPLODE_WALL* pCmd = reinterpret_cast<SCR_EXPLODE_WALL*>(gBasePtr);
    switch (pCmd->mFace)
    {
        case 1:
            gObject_5C->CreateExplosion(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_225_23,
                                                      0);
            break;
        case 2:
            gObject_5C->CreateExplosion(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_45_22,
                                                      0);
            break;
        case 3:
            gObject_5C->CreateExplosion(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_135_24,
                                                      0);
            break;
        case 4:
            gObject_5C->CreateExplosion(pCmd->mPos.x,
                                                      pCmd->mPos.y,
                                                      pCmd->mPos.z,
                                                      kAngZero_6F804C,
                                                      explosion_type::building_315_25,
                                                      0);
            break;
        default:
            break;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x509fd0)
void ScriptThread::Locate()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mVarname));

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
            switch (gBasePtr->mType)
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
                    if (pPointer->mChar->has_car())
                    {
                        mCondResult = true;
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_ANY:
                    if (pPointer->mChar->bHasGameObject_403B70() &&
                        pPointer->mChar->GetGameObjectVelocity() == kFpZero_6F77C0)
                    {
                        mCondResult = true;
                    }
                    else if (pPointer->mChar->has_car() && pPointer->mChar->GetPedVelocity() == kFpZero_6F77C0)
                    {
                        mCondResult = true;
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_FOOT:
                    if (pPointer->mChar->bHasGameObject_403B70())
                    {
                        if (pPointer->mChar->GetGameObjectVelocity() == kFpZero_6F77C0)
                        {
                            mCondResult = true;
                        }
                    }

                    break;
                case SCRCMD_STOP_LOCATE_CHAR_CAR:
                    if (pPointer->mChar->has_car() && pPointer->mChar->GetPedVelocity() == kFpZero_6F77C0)
                    {
                        mCondResult = true;
                    }

                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidCase,
                                      "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                                      4622,
                                      gBasePtr->mType);
                    break;
            }
        }
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a200)
void ScriptThread::Handle_SET_CHAR_OBJ2()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    ScriptThread::SetCharObjective(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr), pPointer);

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
            pParam = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mSecondItemIdx));
            pPointer->mChar->set_objective_target_ped(pParam->mChar);
            break;
        case objectives_enum::enter_car_as_driver_35:
            pParam = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mSecondItemIdx));
            pPointer->mChar->SetTargetObjectiveCar(pParam->mCar);
            if (pCmd->mVariant == 1)
            {
                pPointer->mChar->set_enter_car_as_passenger(true);
            }
            else
            {
                pPointer->mChar->set_enter_car_as_passenger(false);
            }
            break;
        case objectives_enum::kill_car_21:
        case objectives_enum::wait_in_car_27:
        case objectives_enum::leave_car_36:
        case objectives_enum::follow_car_in_car_55:
        case objectives_enum::destroy_car_59:
            pParam = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mSecondItemIdx));
            pPointer->mChar->SetTargetObjectiveCar(pParam->mCar);
            break;
        case objectives_enum::fire_at_object_from_vehicle_57:
        case objectives_enum::destroy_object_58:
            pParam = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mSecondItemIdx));
            pPointer->mChar->mObjectiveTargetObject = pParam->mObj;
            break;
        default:
            break;
    }

    Ped* pPed = pPointer->mChar;
    pPed->mBf21C.bScheduledForRemoval = false;

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a3e0)
void ScriptThread::Handle_SET_CHAR_OBJ3()
{
    SCR_CHAR_OBJ3* pCmd = reinterpret_cast<SCR_CHAR_OBJ3*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    ScriptThread::SetCharObjective(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr), pPointer);

    if (pPointer->mChar)
    {
        (pPointer->mChar)->mObjectiveTargetX = pCmd->mPos.x;
        (pPointer->mChar)->mObjectiveTargetY = pCmd->mPos.y;
        (pPointer->mChar)->mObjectiveTargetZ = pCmd->mPos.z;

        (pPointer->mChar)->mBf21C.bScheduledForRemoval = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a460)
void ScriptThread::Handle_SET_CHAR_OBJ_FOLLOW()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr);
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    ScriptThread::SetCharObjective(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr), pPointer);

    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCarIdx));

    if (pPointer->mChar)
    {
        (pPointer->mChar)->SetTargetObjectiveCar(pCarPointer->mCar);

        Ang16 cmd_rotation;
        cmd_rotation.rValue = pCmd->mRotation;

        Fix16 fix_1;
        fix_1.mValue = cmd_rotation.FromUnsignedToFloat();
        Fix16 fix_2;
        fix_2.mValue = kDegreesToAng16.ToFloat();
        Fix16 fix_3;
        fix_3.mValue = fix_2.MultiplyInt64(fix_1);

        Ang16 rotation;
        rotation.rValue = fix_3.ToInt();
        rotation.Normalize();

        (pPointer->mChar)->mFollowCarOffsetAngle = rotation.rValue;
        (pPointer->mChar)->mFollowCarOffsetDistance = pCmd->mOffset;
        (pPointer->mChar)->mBf21C.bScheduledForRemoval = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a570)
void ScriptThread::Handle_CHANGE_CAR_REMAP()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pCmd->mUnsigned2 != 0xFFu) //  mUnsigned2 = remap
    {
        pPointer->mCar->SetCarRemap(static_cast<u8>(pCmd->mUnsigned2));
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a5b0)
void ScriptThread::Handle_CHANGE_CHAR_REMAP()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (pPointer->mChar)
    {
        pPointer->mChar->SetRemap(static_cast<u8>(pCmd->mUnsigned2));
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a610)
void ScriptThread::Handle_CHECK_CAR_MODEL()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a670)
void ScriptThread::Handle_CHECK_CAR_REMAP()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a6e0)
void ScriptThread::Handle_CHECK_CAR_BOTH()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a760)
void ScriptThread::IsOnScreen()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    bool onScreen;

    switch (ScriptThread::GetEntityTypeOfCommand(pPointer->mType))
    {
        case script_entity_type::car_2:
        {
            if (!pPointer->mCar)
            {
                FatalError_4A38C0(Gta2Error::LineAccessingCarThatHasntBeenCreated, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1376, static_cast<u16>(gBasePtr->mCmdThis));
            }

            Car* pCar = pPointer->mCar;

            if (pCar->mDespawnStatus == car_despawn_status::despawned_6)
            {
                onScreen = false;
                break;
            }

            if (gBasePtr->mType == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40->IsSpriteInAnyPlayerView(pCar->mCarSprite, 1);
            }
            else
            {
                onScreen = gGame_0x40->IsSpriteInAnyPlayerView(pCar->mCarSprite, 0);
            }

            break;
        }
        case script_entity_type::char_1:
            if (!pPointer->mChar)
            {
                FatalError_4A38C0(Gta2Error::LineAccessingCharacterThatHasntBeenCreated, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1383, static_cast<u16>(gBasePtr->mCmdThis));
            }

            if (gBasePtr->mType == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40->IsSpriteInAnyPlayerView(pPointer->mChar->GetSprite(), 1);
            }
            else
            {
                onScreen = gGame_0x40->IsSpriteInAnyPlayerView(pPointer->mChar->GetSprite(), 0);
            }

            break;
        case script_entity_type::object_3:
            if (!pPointer->mObj)
            {
                FatalError_4A38C0(Gta2Error::LineAccessingObjectThatHasntBeenCreated, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x138E, static_cast<u16>(gBasePtr->mCmdThis));
            }

            if (gBasePtr->mType == SCRCMD_IS_ITEM_ONSCREEN)
            {
                onScreen = gGame_0x40->IsSpriteInAnyPlayerView(pPointer->mObj->mSprite, 1);
            }
            else
            {
                onScreen = gGame_0x40->IsSpriteInAnyPlayerView(pPointer->mObj->mSprite, 0);
            }

            break;
        case script_entity_type::crane_4:
            if (!pPointer->mCrane)
            {
                FatalError_4A38C0(Gta2Error::LineAccessingCraneThatHasntBeenCreated, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 0x1399, static_cast<u16>(gBasePtr->mCmdThis));
            }

            if (gBasePtr->mType == SCRCMD_IS_ITEM_ONSCREEN)
            {
                Sprite* pSprite = pPointer->mCrane->GetRotorSprite();
                onScreen = gGame_0x40->IsSpriteInAnyPlayerView(pSprite, 1);
            }
            else
            {
                Sprite* pSprite = pPointer->mCrane->GetRotorSprite();
                onScreen = gGame_0x40->IsSpriteInAnyPlayerView(pSprite, 0);
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a940)
void ScriptThread::Handle_DELAY_HERE()
{
    s16* pDelay = &reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mSigned1;

    if (static_cast<s16>(gBasePtr->mIndex) == -1)
    {
        *pDelay = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mUnsigned2;
    }
    else
    {
        *pDelay = gBasePtr->mIndex - 1;
        if (!*pDelay)
        {
            *pDelay = -1;
            ScriptThread::Next(gBasePtr);
        }
    }
}

MATCH_FUNC(0x50a980)
void ScriptThread::Handle_DELAY()
{
    s16 delay = gBasePtr->mIndex;
    s16* pDelay = &reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mSigned1;

    if (delay == -1)
    {
        *pDelay = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mUnsigned2;
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50a9e0)
void ScriptThread::EnableThread(u16 idx)
{
    SCR_CMD_HEADER* pCmd = gScriptManager->GetBasePointer(idx);

    switch (pCmd->mType)
    {
        case SCRCMD_THREAD_DECLARE2:
            Handle_THREAD_DECLARE2(static_cast<SCR_THREAD*>(pCmd), reinterpret_cast<s16*>(pCmd));
            break;
        case SCRCMD_THREAD_DECLARE3:
        {
            SCR_THREAD* pThread = static_cast<SCR_THREAD*>(pCmd);

            if (pThread->mTriggerAdded)
            {
                gCarThreadTriggers_214->SetTriggerEnableState(pThread->mTriggerIdx, car_thread_trigger_state::armed_1);
            }
            else
            {
                Handle_THREAD_DECLARE3(static_cast<SCR_THREAD*>(pCmd));
            }

            break;
        }
        case SCRCMD_THREAD_DECLARE4:
            Handle_THREAD_DECLARE4(static_cast<SCR_THREAD*>(pCmd), reinterpret_cast<s16*>(pCmd));
            break;
        case SCRCMD_THREAD_DECLARE5:
        case SCRCMD_CHAR_AREA_ANY_MEANS:
        {
            SCR_CHAR_AREA_ANY* pAny = static_cast<SCR_CHAR_AREA_ANY*>(pCmd);

            if (pAny->mTriggerAdded)
            {
                gCarThreadTriggers_214->SetTriggerEnableState(pAny->mTriggerIdx, car_thread_trigger_state::armed_1);
            }
            else
            {
                Handle_THREAD_DECLARE5(pAny);
            }

            break;
        }
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50abc0)
void ScriptThread::Handle_DISABLE_THREAD()
{
    ScriptThread::DisableThread(gBasePtr->mIndex);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50abf0)
void ScriptThread::Handle_ENABLE_THREAD()
{
    ScriptThread::EnableThread(gBasePtr->mIndex);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ac20)
void ScriptThread::SCRCMD_SET_GANG_KILL_REACTION()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    str_table_entry* gang_1_str = gScriptManager->FindStringById(gBasePtr->mIndex);
    if (gang_1_str == NULL)
    {
        FatalError_4A38C0(Gta2Error::UnableToFindGangMatchingNameInSetGangRespect,
                          "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                          5217,
                          static_cast<u16>(gBasePtr->mCmdThis));
    }
    Gang* pGang_1 = gGangPool_CA8->FindGangByName(gang_1_str->get_name());
    str_table_entry* gang_2_str = gScriptManager->FindStringById(pCmd->mUnsigned3);
    if (gang_2_str == NULL)
    {
        FatalError_4A38C0(Gta2Error::UnableToFindGangMatchingNameInSetGangRespect2,
                          "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                          5223,
                          static_cast<u16>(gBasePtr->mCmdThis));
    }
    Gang* pGang_2 = gGangPool_CA8->FindGangByName(gang_2_str->get_name());
    pGang_1->SetGangKillReaction(pGang_2->mGangIdx, static_cast<char_type>(pCmd->mUnsigned2));
    pGang_1->SetKillAffectsRespect(true);
    pGang_2->SetKillAffectsRespect(true);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50acf0)
void ScriptThread::Handle_CHANGE_RESPECT()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);

    str_table_entry* pGangStr = gScriptManager->FindStringById(gBasePtr->mIndex);

    char_type* gang_name = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8->FindGangByName(gang_name);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned3));

    // pCmd->mSigned2 = respect parameter

    switch (gBasePtr->mType)
    {
        case SCRCMD_SET_CHAR_RESPECT:

            pGang->SetRespect(pPointer->mChar->mPlayer->get_idx(), 20 * static_cast<u8>(pCmd->mSigned2));
            pGang->SetKillAffectsRespect(true);
            break;
        case SCRCMD_CHANGE_RESPECT:
            if (pCmd->mSigned2 > 0)
            {
                pGang->IncrementRespect(pPointer->mChar->mPlayer->get_idx(), 20 * (pCmd->mSigned2));
            }
            else
            {
                pGang->DecrementRespect(pPointer->mChar->mPlayer->get_idx(), 20 * abs(pCmd->mSigned2));
            }
            break;
        case SCRCMD_CHANGE_GANG_RESP:
            pGang->ChangeRespectAndUpdate(pPointer->mChar->mPlayer->get_idx(), 20 * static_cast<u8>(pCmd->mSigned2));
            break;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50aef0)
void ScriptThread::RespectOperator()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned1));
    str_table_entry* pStr = gScriptManager->FindStringById(pCmd->mUnsigned3);
    Gang* pGang = gGangPool_CA8->FindGangByName(pStr->get_name());

    switch (pCmd->mType)
    {
        case SCRCMD_CHECK_RESPECT_IS:
        {
            u8 player_idx = pPointer->mChar->mPlayer->get_idx();

            if (pGang->GetRespectForPlayer(player_idx) / 20 == pCmd->mSigned2)
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
            u8 player_idx = pPointer->mChar->mPlayer->get_idx();

            if (pGang->GetRespectForPlayer(player_idx) / 20 < pCmd->mSigned2)
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
            u8 player_idx = pPointer->mChar->mPlayer->get_idx();

            if (pGang->GetRespectForPlayer(player_idx) / 20 > pCmd->mSigned2)
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b0e0)
void ScriptThread::Handle_ADD_PATROL_POINT()
{
    SCR_ADD_PATROL_POINT* pCmd = reinterpret_cast<SCR_ADD_PATROL_POINT*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;
    if (pPed)
    {
        if (pPed->get_objective() != objectives_enum::patrol_on_foot_42)
        {
            pPed->SetObjective(objectives_enum::patrol_on_foot_42);
        }
        pPointer->mChar->PushPatrolPoint(pCmd->mPos.x.ToInt(),
                                                       pCmd->mPos.y.ToInt(),
                                                       pCmd->mPos.z.ToInt());
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b150)
void ScriptThread::Handle_SET_AMBIENT()
{
    SCR_SET_AMBIENT* pCmd = reinterpret_cast<SCR_SET_AMBIENT*>(gBasePtr);
    gpMapRenderer->SetAmbientLevel(pCmd->mLevel, pCmd->mTime);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b180)
void ScriptThread::Handle_ANSWER_PHONE()
{
    SCR_ANSWER_PHONE* pCmd = reinterpret_cast<SCR_ANSWER_PHONE*>(gBasePtr);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mPedIdx));
    SCR_POINTER* pPtrObj = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mObjIdx));
    Object* pPhoneObj = pPtrObj->mObj;
    if (pPhoneObj && pPtrPed->mChar)
    {
        pPhoneObj->PoolGiveAndMarkDone();
        gScriptManager->AddPhoneThread(pPtrPed->mChar->get_id(),
                                           pPtrObj->mObj->mId,
                                           gBasePtr->mCmdThis);
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b230)
void ScriptThread::Handle_CHECK_PHONE()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    ThreadTrigger* pTrigger = gScriptManager->FindPhoneThreadByObjId(pPointer->mObj->mId);
    SCR_ANSWER_PHONE* pAnswer = static_cast<SCR_ANSWER_PHONE*>(gScriptManager->GetBasePointer(pTrigger->mCmdLine));
    if (pAnswer->mAnswered == 1)
    {
        gScriptManager->RemovePhoneThread(pTrigger->mPedId, pTrigger->mTargetId);
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b2c0)
void ScriptThread::Handle_CHECK_PHONETIMER()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    ThreadTrigger* pTrigger = gScriptManager->FindPhoneThreadByObjId(pPointer->mObj->mId);
    if (pTrigger)
    {
        SCR_ANSWER_PHONE* pAnswer = static_cast<SCR_ANSWER_PHONE*>(gScriptManager->GetBasePointer(pTrigger->mCmdLine));
        if (static_cast<u16>(mPhoneTimer) != 0xFFFF)
        {
            if (mPhoneTimer == 0 && pAnswer->mAnswered == 0)
            {
                mCondResult = true;
                pPointer->mObj->PoolGiveAndMarkDone();
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b360)
void ScriptThread::Handle_STOP_PHONE_RING()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Object* pObj = pPointer->mObj;
    if (pObj)
    {
        ThreadTrigger* pTrigger = gScriptManager->FindPhoneThreadByObjId(pObj->mId);
        if (pTrigger)
        {
            gScriptManager->RemovePhoneThread(pTrigger->mPedId, pTrigger->mTargetId);
        }
        pPointer->mObj->PoolGiveAndQueueObjectType(163);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b3d0)
void ScriptThread::Handle_IS_CHAR_FIRE_ONSCREEN()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;
    if (pPed->GetAttacking() && pPed->GetOffscreenCounter() == 0 && pPed->mSelectedWeapon)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b440)
void ScriptThread::Handle_CAR_DRIVE_AWAY()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    pPointer->mCar->InitCarAIControl();
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b470)
void ScriptThread::Handle_GET_CAR_FROM_CRANE()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer1 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(
        reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mUnsigned2));
    SCR_POINTER* pPointer2 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned1));
    Crane* pCrane = pPointer1->mCrane;

    if (pCrane != NULL)
    {
        pPointer2->mCar = pCrane->GetCarFromCrane();
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b4f0)
void ScriptThread::Handle_CHAR_TO_DRIVE_CAR()
{
    SCR_CHAR_ENTER_CAR* pCmd = reinterpret_cast<SCR_CHAR_ENTER_CAR*>(gBasePtr);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pPtrCar = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCarIdx));

    if (pPtrPed->mChar)
    {
        if (gBasePtr->mType != SCRCMD_CHAR_TO_BACKDOOR)
        {
            pPtrPed->mChar->SetObjective2(objectives_enum::no_obj_0);
            pPtrPed->mChar->SetObjective(objectives_enum::enter_car_as_driver_35);
            pPtrPed->mChar->SetTargetObjectiveCar(pPtrCar->mCar);
            pPtrPed->mChar->set_enter_car_as_passenger(false);
        }
        else
        {
            pPtrPed->mChar->EnterCarByBackDoor(pPtrCar->mCar);
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b5a0)
void ScriptThread::Handle_DRIVER_OUT_CAR()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pPointer->mChar)
    {
        pPointer->mChar->SetObjective2(objectives_enum::no_obj_0);
        pPointer->mChar->SetObjective(objectives_enum::leave_car_36);
        pPointer->mChar->mTargetObjectiveCar = pPointer->mChar->mCar;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b600)
void ScriptThread::Handle_GIVE_DRIVER_BRAKE()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pPointer->mCar)
    {
        if (pPointer->mCar->GetDriver() == NULL)
        {
            pPointer->mCar->SpawnDriverPed();
        }
        pPointer->mCar->InitCarAIControl();
        pPointer->mCar->SetAIHalted();
        pPointer->mCar->SetDriverPedType(ped_type::script_created_5);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b670)
void ScriptThread::Handle_SET_GANG_INFO1()
{
    // mid-game change
    ScriptThread::ApplyGangInfo(reinterpret_cast<SCR_SET_GANG_INFO*>(gBasePtr));
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b690)
void ScriptThread::Handle_CLEAR_TIMERS()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (pPointer->mIndex != 0xFFFF)
    {
        gHud_2B00->mPagers.ClearPager(pPointer->mIndex);
    }
    pPointer->mIndex = 0;
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b6f0)
void ScriptThread::Handle_CHECK_SCORE()
{
    SCR_CHECK_SCORE_GREATER* pCmd = reinterpret_cast<SCR_CHECK_SCORE_GREATER*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        if (pPed->is_player() && pPed->mPlayer->GetScore() > pCmd->mTargetScore)
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b760)
void ScriptThread::Handle_GET_SCORE()
{
    SCR_GET_SCORE* pCmd = reinterpret_cast<SCR_GET_SCORE*>(gBasePtr);
    SCR_POINTER* pPtrPed = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pPtrCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounterIdx));

    if (pPtrPed->mChar)
    {
        if (pPtrPed->mChar->is_player())
        {
            pPtrCounter->mCounter = static_cast<u16>(pPtrPed->mChar->mPlayer->GetScore());
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b7d0)
void ScriptThread::Handle_IS_CHAR_IN_GANG()
{
    SCR_IS_CHAR_IN_GANG_ZONE* pCmd = reinterpret_cast<SCR_IS_CHAR_IN_GANG_ZONE*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    str_table_entry* pGangStr = gScriptManager->FindStringById(pCmd->mGangIdx);
    const char_type* gang_name = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8->FindGangByName(gang_name);
    Ped* pPed = pPointer->mChar;
    if (pPed->is_player())
    {
        if (pPed->mPlayer->get_gang_curr_location() == pGang)
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
        gmp_map_zone* pZone = gMap_0x370->GetZoneAtPosOfType(pPed->get_cam_x().ToInt(), pPed->get_cam_y().ToInt(), Gang_14);
        if (pZone && !_strnicmp(pZone->mName, pGang->mName, pZone->mNameLength))
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b8b0)
void ScriptThread::Handle_SET_NO_COLLIDE()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Car* pCar = pPointer->mCar;

    if (pCar)
    {
        if (gBasePtr->mType == SCRCMD_SET_NO_COLLIDE)
        {
            pCar->AddFlags(car_flag::NoCollide);
        }
        else if (gBasePtr->mType == SCRCMD_CLEAR_NO_COLLIDE)
        {
            pCar->mFlags &= static_cast<u16>(~car_flag::NoCollide);
        }
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b910)
void ScriptThread::Handle_IS_CHAR_FIRING_AREA()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Ped* pPed = pPointer->mChar;

    if (pPed->GetAttacking() && pPed->IsWithinArea(&pCmd->mRect))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50b9c0)
void ScriptThread::Handle_GET_PASSENGER_NUM()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Car* pCar = pPointer->mCar;

    if (pCar != NULL && pCar->mPassengersList.GetPedsCount() >= pCmd->mSigned2)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ba30)
void ScriptThread::Handle_CLEAR_WANTED_LEVEL()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    pPointer->mChar->ClearWantedPoints();
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ba70)
void ScriptThread::Handle_ALT_WANTED_LEVEL()
{
    SCR_CMD_ALTER_WANTED_LEVEL* pCmd = reinterpret_cast<SCR_CMD_ALTER_WANTED_LEVEL*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (gBasePtr->mType == SCRCMD_ALTER_WANTED_LEVEL)
    {
        if (!pPointer->mChar->WantedStarCountGreaterThan(pCmd->mWantedLevel))
        {
            pPointer->mChar->set_wanted_star_count(pCmd->mWantedLevel);
        }
    }
    else
    {
        pPointer->mChar->set_wanted_star_count(pCmd->mWantedLevel);
    }

    Next(gBasePtr);
}

MATCH_FUNC(0x50bad0)
void ScriptThread::Handle_CAR_WRECK_IN_LOCATION()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Car* pCar = pPointer->mCar;

    if ((pCar->mDamage >= 32000 || pCar->mDespawnStatus == car_despawn_status::despawning_5) && pCar->IsWithinArea(&pCmd->mRect))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50bb80)
void ScriptThread::Handle_SEND_CAR_TO_BLOCK()
{
    SCR_CAR_DATA_DEC* pCmd = reinterpret_cast<SCR_CAR_DATA_DEC*>(gBasePtr);
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCarIdx));
    pScrPtr->mCar->GotoBlock(pCmd->mPos.x.ToUInt8(),
                                           pCmd->mPos.y.ToUInt8(),
                                           pCmd->mPos.z.ToUInt8(),
                                           1);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50bbd0)
void ScriptThread::SCRCMD_THREAT_SEARCH_REACT_ADD_GROUP()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr);
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCharIdx));

    switch (gBasePtr->mType)
    {
        case SCRCMD_ADD_GROUP:
            pScrPtr->mChar->SpawnPedGroupFollowers(static_cast<u8>(pCmd->mObjective));
            break;
        case SCRCMD_SET_THREAT_REACT:
            pScrPtr->mChar->mThreatReaction = pCmd->mObjective;
            break;
        case SCRCMD_SET_THREAT_SEARCH:
            pScrPtr->mChar->mThreatSearch = pCmd->mObjective;
            break;
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50bc60)
void ScriptThread::Handle_CHECK_NUM_ALIVE()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr);
    SCR_POINTER* pScrPtr =
        static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr)->mCharIdx));
    PedGroup* pPedGroup = pScrPtr->mChar->GetGroup();

    if (pPedGroup != NULL && static_cast<s16>(pPedGroup->mCount34) >= pCmd->mObjective)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50bcd0)
void ScriptThread::Handle_SET_MIN_ALIVE()
{
    SCR_CHAR_OBJECTIVE* pCmd = reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr);
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(reinterpret_cast<SCR_CHAR_OBJECTIVE*>(gBasePtr)->mCharIdx));
    pPointer->mChar->GetGroup()->mUnused35 = static_cast<char_type>(pCmd->mObjective);
    ScriptThread::Next(gBasePtr);
}

// 9.6f 0x47BAC0
MATCH_FUNC(0x50bd10)
void ScriptThread::SCRCMD_CHAR_GROUP()
{
    SCR_TWO_PARAMS* pParams = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);

    SCR_POINTER* pPtr1 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pParams->mUnsigned1));
    SCR_POINTER* pPtr2 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    Ped* pChar = pPtr1->mChar;
    PedGroup* pGroup = pChar->GetGroup();

    switch (gBasePtr->mType)
    {
        case SCRCMD_MAKE_LEADER:
            pPtr2->mChar->BecomeLeaderOfGroup(pChar);
            break;

        case SCRCMD_REMOVE_CHAR:
            pGroup->RemovePed(pPtr2->mChar);
            break;

        case SCRCMD_ADD_CHAR_TO_GROUP:
            pGroup->add_ped_to_end_of_list(pPtr2->mChar);
            break;
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50bdc0)
void ScriptThread::SCRCMD_SET_COUNTER() // Set counter to a given constant value
{
    SCR_SET_COUNTER* pCmd = reinterpret_cast<SCR_SET_COUNTER*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    pPointer->mCounter = pCmd->mValue;
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50be00)
void ScriptThread::Handle_HAS_CAR_WEAPON()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    s16 weapon_idx = pCmd->mSigned2;

    if (gWeapon_8->find(pPointer->mCar, weapon_idx))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50be70)
void ScriptThread::Handle_IS_CHAR_HORN()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;
    Car* pCar = pPed->get_car();

    if (pCar != NULL && pCar->IsEmittingHorn_411970())
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50bed0)
void ScriptThread::Handle_CHECK_MAX_PASS()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Car* pCar = pPointer->mChar->get_car();

    if (pCar && static_cast<u8>(pCar->GetMaxPassengers()) >= pCmd->mSigned2)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50bf40)
void ScriptThread::Handle_IS_CHAR_IN_ZONE()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    str_table_entry* pStrEntry = gScriptManager->FindStringById(pCmd->mSigned2);
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
        pZone1 = gMap_0x370->GetZoneAtPosOfType(pPointer->mChar->get_cam_x().ToInt(),
                                                              pPointer->mChar->get_cam_y().ToInt(),
                                                              Local_Navigation_15);
        pZone2 = gMap_0x370->GetZoneAtPosOfType(pPointer->mChar->get_cam_x().ToInt(),
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c040)
void ScriptThread::Handle_SET_PHONE_DEAD()
{
    u16* pIndex = &gBasePtr->mIndex;
    SCR_POINTER* pScrPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(*pIndex));

    pScrPtr->mObj->PoolGiveAndQueueObjectType(objects::dead_phone_174);
    gScriptManager->RemovePhoneThreadByObjId(pScrPtr->mObj->mId);
    u16* pPhoneIds = &gScriptManager->mPhoneIds[0];

    for (u8 i = 0; i < 31; i++)
    {
        if (*pPhoneIds == *pIndex)
        {
            gGameSave.mCarAndScriptData.mFlags |= 1 << i;
            break;
        }

        pPhoneIds++;
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c0e0)
void ScriptThread::Handle_IS_TRAILER_ATT()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pTrailerPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    Car* pCar;

    if (pTrailerPointer == pCarPointer)
    {
        pCar = pCarPointer->mCar;

        if (pCar->IsTrailerCar() || pCarPointer->mCar->IsTrailerCab())
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

        if (pCarPointer->mCar->HasAttachedTrailer(pCar))
        {
            mCondResult = true;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c1b0)
void ScriptThread::Handle_IS_CAR_ON_TRAIL()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pTrailerPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    Sprite* pSprite = pTrailerPointer->mCar->mAttachments.FirstSpriteOfType(sprite_types_enum::car_2);
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c230)
void ScriptThread::SCRCMD_ENABLE_DISABLE_CRANE()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    s16 type = gBasePtr->mType;

    if (type == SCRCMD_ENABLE_CRANE)
    {
        pPointer->mCrane->mDisabled = 0;
    }
    else if (type == SCRCMD_DISABLE_CRANE)
    {
        pPointer->mCrane->mDisabled = 1;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c2a0)
void ScriptThread::Handle_CAR_GOT_DRIVER()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pPointer->mCar->mDriver)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c2f0)
void ScriptThread::Handle_SPOTTED_PLAYER()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pPointer->mChar->HasSpottedPlayer())
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c350)
void ScriptThread::Handle_GET_LAST_PUNCHED()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pCharTargetPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pCharPunchedPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    pCharPunchedPointer->mChar = pCharTargetPointer->mChar->get_last_char_punched();
    pCharTargetPointer->mChar->clear_last_char_punched();

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c3b0)
void ScriptThread::Handle_IS_CHAR_STUNNED()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed->GetPedState2() == ped_state_2::lying_on_floor_22 || pPed->get_health() <= 25)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c410)
void ScriptThread::Handle_KILL_ALL_PASSENG()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    pPointer->mCar->mPassengersList.KillAndClearAllPeds();

    if (gPublicTransport_181C->is_bus(pPointer->mCar))
    {
        gPublicTransport_181C->KillAllPassengers();
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c470)
void ScriptThread::Handle_IS_GROUP_IN_CAR()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pGroupLeader = pPointer->mChar;
    PedGroup* pGroup = pGroupLeader->mPedGroup;

    bool allInCar;
    if (pGroup != NULL)
    {
        allInCar = pGroup->IsAllMembersInSomeCar();
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c4e0)
void ScriptThread::Handle_PUNCHED_SOMEONE()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pPointer->mChar->get_last_char_punched() != NULL)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c540)
void ScriptThread::Handle_ADD_CHAR_TO_GANG()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    str_table_entry* pGangStr = gScriptManager->FindStringById(pCmd->mUnsigned2);

    char_type* gang_name = pGangStr->get_name();

    pPointer->mChar->mpGang = gGangPool_CA8->FindGangByName(gang_name);

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c5a0)
void ScriptThread::Handle_EXPLODE_ITEM()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    s32 type;

    switch (gBasePtr->mType)
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

    switch (ScriptThread::GetEntityTypeOfCommand(pPointer->mType))
    {
        case script_entity_type::car_2:
            pPointer->mCar->HandleCarExplosion(explosion_type::item_19);
            break;
        case script_entity_type::char_1:
        {
            Ped* pChar = pPointer->mChar;

            gObject_5C
                ->CreateExplosion(pChar->get_cam_x(), pChar->get_cam_y(), pChar->get_cam_z(), kAngZero_6F804C, type, 0);

            break;
        }
        case script_entity_type::object_3:
        {
            // 9.6f: Object::GetX/GetY/GetZ (inlined, using them changes the code)
            Sprite* pSprite = pPointer->mObj->mSprite;

            gObject_5C
                ->CreateExplosion(pSprite->GetXPos(), pSprite->GetYPos(), pSprite->GetZPos(), kAngZero_6F804C, type, 0);

            break;
        }
        case script_entity_type::crane_4:
        {
            SCR_CRANE_TARGET_DEC* pCrane = reinterpret_cast<SCR_CRANE_TARGET_DEC*>(pPointer);

            gObject_5C->CreateExplosion(
                pCrane->mPos.mX,
                pCrane->mPos.mY,
                gMap_0x370->FindGroundZForCoord(pCrane->mPos.mX, pCrane->mPos.mY),
                kAngZero_6F804C,
                type,
                0);

            break;
        }
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c6f0)
void ScriptThread::Handle_PARK() // PARK and PARK_NO_RESPAWN
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pDoorPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    gGarage_48->ParkCarAtDoor(pCarPointer->mCar, pDoorPointer->mDoor);
    if (gBasePtr->mType == SCRCMD_PARK_NO_RESPAWN)
    {
        gGarage_48->mNoRespawn = true;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c760)
void ScriptThread::Handle_BEEN_PUNCHED_BY()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointerVictim = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pPointerAttacker = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    if (pPointerAttacker->mChar->get_last_char_punched() == pPointerVictim->mChar)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c7d0)
void ScriptThread::Handle_UPDATE_DOOR()
{
    SCR_UPDATE_DOOR* pCmd = reinterpret_cast<SCR_UPDATE_DOOR*>(gBasePtr);
    SCR_POINTER* pDoorPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mDoorIdx));

    SCR_POINTER* pTarget;

    switch (pDoorPointer->mDoor->get_open_type())
    {
        case door_open_type::one_char_on_foot:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mTargetIdx));

            pDoorPointer->mDoor->set_open_details_ped(door_open_type::one_char_on_foot, pTarget->mChar);
            pDoorPointer->mDoor->set_target_id(pTarget->mChar->mId);
            break;
        case door_open_type::one_car:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mTargetIdx));

            pDoorPointer->mDoor->set_open_details_car_bc(door_open_type::one_car, pTarget->mCar);
            pDoorPointer->mDoor->set_target_id(pTarget->mCar->mCarId);
            break;
        case door_open_type::any_player_one_car:
            pTarget = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mTargetIdx));

            pDoorPointer->mDoor->set_open_details_car_bc(door_open_type::any_player_one_car, pTarget->mCar);
            pDoorPointer->mDoor->set_target_id(pTarget->mCar->mCarId);
            break;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c8a0)
void ScriptThread::SCRCMD_DOOR()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    switch (gBasePtr->mType)
    {
        case SCRCMD_OPEN_DOOR:
        {
            Door* pDoor = pPointer->mDoor;

            if (pDoor->mPrimaryDoorData)
            {
                pDoor->mPrimaryDoorData->Open(0);
            }

            if (pDoor->mSecondaryDoorData)
            {
                pDoor->mSecondaryDoorData->Open(pDoor->mbDoFlip);
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
                pDoor->mPrimaryDoorData->Close(0);
            }

            if (pDoor->mSecondaryDoorData)
            {
                pDoor->mSecondaryDoorData->Close(pDoor->mbDoFlip);
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c990)
void ScriptThread::Handle_REMOVE_WEAPON()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Ped* pPed = pPointer->mChar;
    Player* pPlayer = pPed->mPlayer;

    if (pPlayer != NULL)
    {
        pPlayer->RemovePlayerWeapons();
    }
    else
    {
        pPed->RemovePedWeapons();
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50c9f0)
void ScriptThread::Handle_REMOVE_BLOCK()
{
    SCR_REMOVE_BLOCK* pCmd = reinterpret_cast<SCR_REMOVE_BLOCK*>(gBasePtr);

    gMap_0x370->RemoveBlock(pCmd->mPos.mX,
                                          pCmd->mPos.mY,
                                          pCmd->mPos.mZ,
                                          pCmd->mDoDrop);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ca30)
void ScriptThread::Handle_LOWER_LEVEL()
{
    SCR_LOWER_LEVEL* pCmd = reinterpret_cast<SCR_LOWER_LEVEL*>(gBasePtr);

    gMap_0x370->LowerLevel(pCmd->mMinPos.mX,
                                         pCmd->mMaxPos.mX,
                                         pCmd->mMinPos.mY,
                                         pCmd->mMaxPos.mY);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ca70)
void ScriptThread::SCRCMD_CHANGE_BLOCK()
{
    SCR_CHANGE_BLOCK* pCmd = reinterpret_cast<SCR_CHANGE_BLOCK*>(gBasePtr);
    gMap_0x370->ChangeBlock(pCmd->mX,
                                          pCmd->mY,
                                          pCmd->mZ,
                                          pCmd->mChangeType,
                                          pCmd->mInfoWord);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50cab0)
void ScriptThread::Handle_ADD_NEW_BLOCK()
{
    gmp_block_info block;

    block.init();

    gMap_0x370->AddNewBlock(reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr)->mXpos,
                                          reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr)->mYpos,
                                          reinterpret_cast<SCR_ADD_BLOCK*>(gBasePtr)->mZpos,
                                          &block);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50cb20)
void ScriptThread::Handle_ROAD_ON_OFF()
{
    SCR_SWITCH_ROAD_ON_OFF* pCmd = reinterpret_cast<SCR_SWITCH_ROAD_ON_OFF*>(gBasePtr);

    if (pCmd->mStatus != 0)
    {
        gRouteFinder->RoadOn(pCmd->mXpos, pCmd->mYpos, pCmd->mZpos);
    }
    else
    {
        gRouteFinder->RoadOff(pCmd->mXpos, pCmd->mYpos, pCmd->mZpos);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50cb70)
void ScriptThread::Handle_CHECK_CAR_DRIVER()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    mCondResult = false;

    switch (gBasePtr->mType)
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

            if (pCar->mDespawnStatus == car_despawn_status::despawned_6 || pCar->mDespawnStatus == car_despawn_status::despawning_5 || pCar->mDamage >= 32000 || pCar->HasSpriteZoom())
            {
                mCondResult = true;
            }

            break;
        }
        case SCRCMD_CAR_SUNK:
            if (pPointer->mCar->HasSpriteZoom())
            {
                mCondResult = true;
            }

            break;
        case SCRCMD_CAR_IN_AIR:
            if (pPointer->mCar->IsCarInAir())
            {
                mCondResult = true;
            }

            break;
        case SCRCMD_IS_CAR_CRUSHED:
            if (pPointer->mCar->IsBeingCrushed())
            {
                mCondResult = true;
            }

            break;
        case SCRCMD_CARBOMB_ACTIVE:
        {
            Car* pCar = pPointer->mCar;
            if (pCar->mAttachments.GetSpriteForModel(objects::car_bomb_132) != NULL)
            {
                mCondResult = true;
            }

            break;
        }
        case SCRCMD_EMERG_LIGHTS_ON:
        {
            Car* pCar = pPointer->mCar;
            car_info* pInfo = gGtx_0x106C->get_car_info_5AA3B0(pCar->mCarInfoIdx);

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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ccb0)
void ScriptThread::Handle_SWITCH_GENERATOR1()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    u16 spawn_count = pCmd->mUnsigned2;
    Generator* pGen = pPointer->mGenerator;
    pGen->Activate();
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50cd30)
void ScriptThread::Handle_SWITCH_GENERATOR3()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    pPointer->mGenerator->mGenType = pCmd->mSigned3;

    Generator* pGen = pPointer->mGenerator;
    u16 spawn_count = pCmd->mUnsigned2;
    pGen->Activate();
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50cdb0)
void ScriptThread::Handle_CAR_DAMAGE_POS()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if ((pPointer->mCar)->IsAreaDamaged(pCmd->mSigned2)) // mSigned2 = damage pos
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ce10)
void ScriptThread::Handle_PARK_FINISHED()
{
    if (gGarage_48->mState == garage_state::park_finished_3)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ce50)
void ScriptThread::Gosub(SCR_CMD_HEADER* pCmd, u16 cmd_idx)
{
    ScriptStackFrame* pFrame = mGosubStack->AllocFrame();
    pFrame->mCondResult = mCondResult;
    pFrame->mReturnCmd = pCmd->mCmdNext;
    mGosubStack->add_503160(pFrame);
    ScriptThread::JumpToCmd(cmd_idx);
}

MATCH_FUNC(0x50ce90)
void ScriptThread::Handle_PHONE_TEMPLATE()
{
    SCR_PHONE_TEMPLATE* pCmd = reinterpret_cast<SCR_PHONE_TEMPLATE*>(gBasePtr);

    SCR_POINTER* pPassedFlag = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter1));
    SCR_POINTER* pFailedFlag = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter2));
    SCR_POINTER* pOnMission0 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter3b));
    SCR_POINTER* pOnMission1 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter3));
    SCR_POINTER* pOnMission2 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter4));
    SCR_POINTER* pOnMission3 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter5));

    str_table_entry* pGangNameEntry = gScriptManager->FindStringById(pCmd->mGangname);
    char_type* gang_name = pGangNameEntry->get_name();
    Gang* pGang = gGangPool_CA8->FindGangByName(gang_name);

    s8 player_idx1;

    // If passed previous mission.
    if (pPassedFlag->mCounter >= 1 && pOnMission0->mCounter == 0 && pOnMission1->mCounter == 0 &&
        pOnMission2->mCounter == 0 && pOnMission3->mCounter == 0)
    {
        // If enough respect.
        if ((player_idx1 = gGame_0x40->mLocalPlayer->get_idx(),
             pGang->GetRespectForPlayer(player_idx1) / 20 >= pCmd->mRespect))
        {
            gScriptManager->mDeathArrestFlag = false;
            str_table_entry* mission2_str_table = gScriptManager->FindStringById(pCmd->mMission2);
            char_type* mission2_name = mission2_str_table->get_name();
            ScriptThread::launch_mission(gBasePtr, mission2_name);
            return;
        }

        // Not enough respect.
        s32 brief_id = pCmd->mBriefId - 7;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }

    s8 player_idx2;

    // If failed previous mission.
    if (pFailedFlag->mCounter >= 1 && pOnMission0->mCounter == 0 && pOnMission1->mCounter == 0 &&
        pOnMission2->mCounter == 0 && pOnMission3->mCounter == 0)
    {
        // If enough respect.
        if ((player_idx2 = gGame_0x40->mLocalPlayer->get_idx(),
             pGang->GetRespectForPlayer(player_idx2) / 20 >= pCmd->mRespect))
        {
            gScriptManager->mDeathArrestFlag = false;
            str_table_entry* mission2_str_table = gScriptManager->FindStringById(pCmd->mMission2);
            char_type* mission2_name = mission2_str_table->get_name();
            ScriptThread::launch_mission(gBasePtr, mission2_name);
            return;
        }

        // Not enough respect.
        s32 brief_id = pCmd->mBriefId - 7;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }

    // If on mission gang 1.
    if (pOnMission1->mCounter >= 1)
    {
        s32 brief_id = pCmd->mBriefId - 5;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }

    // If on mission gang 2.
    if (pOnMission2->mCounter == 1)
    {
        s32 brief_id = pCmd->mBriefId - 2;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }

    // If on mission gang 3.
    if (pOnMission3->mCounter == 1)
    {
        s32 brief_id = pCmd->mBriefId - 1;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }

    // If already passed.
    if (pPassedFlag->mCounter == 1)
    {
        s32 brief_id = pCmd->mBriefId;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }

    // If already failed.
    if (pFailedFlag->mCounter == 1)
    {
        s32 brief_id = pCmd->mBriefId + 1;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }

    s8 player_idx3;

    // Start mission 1 if enough respect.
    if ((player_idx3 = gGame_0x40->mLocalPlayer->get_idx(),
         pGang->GetRespectForPlayer(player_idx3) / 20 >= pCmd->mRespect))
    {
        gScriptManager->mDeathArrestFlag = false;
        str_table_entry* mission1_str_table = gScriptManager->FindStringById(pCmd->mMission1);
        char_type* mission1_name = mission1_str_table->get_name();
        ScriptThread::launch_mission(gBasePtr, mission1_name);
        return;
    }

    s8 player_idx4;

    // If zero respect.
    if ((player_idx4 = gGame_0x40->mLocalPlayer->get_idx(), (pGang->GetRespectForPlayer(player_idx4) / 20) == 0))
    {
        s32 brief_id = pCmd->mBriefId - 6;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }

    s8 player_idx5;

    // If negative respect.
    if ((player_idx5 = gGame_0x40->mLocalPlayer->get_idx(), pGang->GetRespectForPlayer(player_idx5) / 20 < 0))
    {
        s32 brief_id = pCmd->mBriefId - 4;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
    }
    else // Positive respect, but not enough.
    {
        s32 brief_id = pCmd->mBriefId - 7;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
    }
}

MATCH_FUNC(0x50d200)
void ScriptThread::Handle_REMOTE_CONTROL()
{
    SCR_REMOTE_CONTROL* pCmd = reinterpret_cast<SCR_REMOTE_CONTROL*>(gBasePtr);
    SCR_POINTER* pPlayerPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pCarPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCarIdx));
    pPlayerPtr->mChar->mPlayer->DisableInputs();
    pPlayerPtr->mChar->mPlayer->ResetAuxCamera();
    pPlayerPtr->mChar->mPlayer->StartRemoteControl(pCarPtr->mCar);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50d2e0)
void ScriptThread::Handle_LAUNCH_MISSION()
{
    str_table_entry* pMissionStr = gScriptManager->FindStringById(gBasePtr->mIndex);
    ScriptThread::launch_mission(gBasePtr, pMissionStr->get_name());
}

MATCH_FUNC(0x50d340)
void ScriptThread::Handle_SAVE_GAME()
{
    char_type* pSaveFileName = gGameSession.GetSaveFileName_4C5970();
    gScriptManager->SaveGame(pSaveFileName);
    Next(gBasePtr);
}

MATCH_FUNC(0x50d3c0)
void ScriptThread::Handle_DO_SAVE_GAME()
{
    if (!mCmdInitialised)
    {
        SCR_DO_SAVE_GAME* pCmd = reinterpret_cast<SCR_DO_SAVE_GAME*>(gBasePtr);
        ScriptThread::DisableThread(pCmd->mTriggername);

        if (static_cast<u32>(gpRng->get_cur_rng()) > 0)
        {
            u32* mission_flag = reinterpret_cast<u32*>(gScriptManager->mMissionFlag);
            if (!mission_flag || *mission_flag)
            {
                // It's in mission, so the player cannot save the game
                sprintf(gTmpBuffer, "svmiss");
            }
            else
            {
                Player* pPlayer = gGame_0x40->mLocalPlayer;

                if (pPlayer->GetScore() >= 50000)
                {
                    // Hallelluya! Another soul saved!
                    gRoot_sound.PlayVoice(voice_line::hallelujah_61);
                    gGame_0x40->mLocalPlayer->AddCash(-50000);
                    gScriptManager->SaveGame(gGameSession.GetSaveFileName_4C5970());
                    sprintf(gTmpBuffer, "svdone");
                }
                else
                {
                    // No donation, no salvation!
                    gRoot_sound.PlayVoice(voice_line::damnation_62);
                    sprintf(gTmpBuffer, "svscore");
                }
            }

            gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        }

        mCmdInitialised = true;
    }
    else
    {
        Ped* pPlayerPed = gGame_0x40->mLocalPlayer->mPlayerPed;
        SCR_DO_SAVE_GAME* pCmd = reinterpret_cast<SCR_DO_SAVE_GAME*>(gBasePtr);

        // This doesn't look right, but it matches. Might be a missing parens bug in the OG game?
        if (pPlayerPed->get_cam_x() < pCmd->mRect.mPos.x - pCmd->mRect.mSize.mX ||
            pPlayerPed->get_cam_x() > pCmd->mRect.mPos.x + pCmd->mRect.mSize.mX &&
                pPlayerPed->get_cam_y() < pCmd->mRect.mPos.y - pCmd->mRect.mSize.mY ||
            pPlayerPed->get_cam_y() > pCmd->mRect.mPos.y + pCmd->mRect.mSize.mY)
        {
            ScriptThread::EnableThread(pCmd->mTriggername);
            ScriptThread::Next(gBasePtr);
        }
    }
}

MATCH_FUNC(0x50d680)
void ScriptThread::Handle_CHANGE_CAR_LOCK()
{
    SCR_CHANGE_CAR_LOCK* pCmd = reinterpret_cast<SCR_CHANGE_CAR_LOCK*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50d870)
void ScriptThread::Handle_CHANGE_INTENSITY()
{
    SCR_CHANGE_LIGHT_INTENSITY* pCmd = reinterpret_cast<SCR_CHANGE_LIGHT_INTENSITY*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    pPointer->mLight->SetIntensity(static_cast<u8>(pCmd->mIntensity));

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50d900)
void ScriptThread::Handle_CHANGE_COLOUR()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    u32 rgb = pCmd->mU32;
    pPointer->mLight->mArgb = rgb;
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50d9a0)
void ScriptThread::Handle_CHANGE_RADIUS()
{
    SCR_CHANGE_LIGHT_RADIUS* pCmd = reinterpret_cast<SCR_CHANGE_LIGHT_RADIUS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    pPointer->mLight->mIntensityRadius.SetRadius_463F10(pCmd->mRadius);

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50da50)
void ScriptThread::SCRCMD_CREATE_LIGHT() // CREATE_LIGHT1 or LIGHT_DECSET2
{
    SCR_POINTER* pPointer =
        static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex)); // SCR_CREATE_LIGHT::mLightIdx
    ScriptThread::CreateLight(reinterpret_cast<SCR_CREATE_LIGHT*>(gBasePtr), pPointer);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50db70)
void ScriptThread::GetSetLivesAndMultiplier()
{
    SCR_GET_NUM_LIVES_MULTIPLIERS* pCmd = reinterpret_cast<SCR_GET_NUM_LIVES_MULTIPLIERS*>(gBasePtr);
    SCR_POINTER* pPlayerPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mPlayerPedIdx));

    switch (pCmd->mType)
    {
        case SCRCMD_ADD_LIVES:
            if (pPlayerPtr->mChar->mPlayer)
            {
                pPlayerPtr->mChar->mPlayer->ChangeLifeCountByAmount(pCmd->mCounterIdx);
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
                pPlayerPtr->mChar->mPlayer->mMultipliers.ChangeStatByAmount(pCmd->mCounterIdx);
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50dd00)
void ScriptThread::SCRCMD_GET_LIVES_MULT() // GET_NUM_LIVES or GET_NUM_MULT
{
    SCR_GET_NUM_LIVES_MULTIPLIERS* pCmd = reinterpret_cast<SCR_GET_NUM_LIVES_MULTIPLIERS*>(gBasePtr);
    SCR_POINTER* pPlayerPtr =
        static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex)); // pCmd->mPlayerPedIdx
    SCR_POINTER* pCounterPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounterIdx));
    if (gBasePtr->mType == SCRCMD_GET_NUM_LIVES)
    {
        pCounterPtr->mCounter = pPlayerPtr->mChar->mPlayer->get_lives();
    }
    else
    {
        pCounterPtr->mCounter = pPlayerPtr->mChar->mPlayer->get_multiplier();
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50dd90)
void ScriptThread::Handle_SET_DIR_OF_TVVAN()
{
    SCR_SET_DIR_OF_TVVAN* pCmd = reinterpret_cast<SCR_SET_DIR_OF_TVVAN*>(gBasePtr);

    Fix16_Point point(pCmd->mPos.mX, pCmd->mPos.mY);
    gCarManager_6C->mTvVanDir = point;

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50de00)
void ScriptThread::Handle_POINT_ONSCREEN()
{
    SCR_IS_POINT_ON_SCREEN* pCmd = reinterpret_cast<SCR_IS_POINT_ON_SCREEN*>(gBasePtr);
    if (gGame_0x40->IsPointOnScreen(pCmd->mXpos, pCmd->mYpos))
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50de50)
void ScriptThread::Handle_CHAR_IN_AIR()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed->GetPedState2() == ped_state_2::falling_19 && pPed->GetPedState() == ped_state_1::immobilized_8)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50deb0)
void ScriptThread::Handle_CHAR_SUNK()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed->GetPedState2() == ped_state_2::sinking_20 && pPed->GetPedState() == ped_state_1::immobilized_8)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50df10)
void ScriptThread::StartBonus()
{
    SCR_START_BONUS* pCmd = reinterpret_cast<SCR_START_BONUS*>(gBasePtr);

    s32 car_model = car_model_enum::none;
    s16 gang_idx = -1;
    s32 alt_car_model = car_model;
    s32 occupation = ped_ocupation_enum::no_occupation;

    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mPtr));

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
            gang_idx = gGangPool_CA8->GetGangIdxByName(
                gScriptManager->FindStringById(pCmd->mId)->get_name());
            break;
        case 0:
            if (pCmd->mId != 0xFFFF)
            {
                car_model = pCmd->mId;

                // French version neutralizes bonuses against law enforcement.
                if (bIsFrench &&
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
            if (bIsFrench && (occupation == ped_ocupation_enum::any_law_enforcement || occupation == ped_ocupation_enum::police))
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
        gMap_0x370->FindZoneByName(gScriptManager->FindStringById(pCmd->mZoneStrId)->get_name());
    }

    pPointer->mCounter = gGame_0x40->mLocalPlayer->mScores.mBonuses.AllocRule(event_type,
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e0b0)
void ScriptThread::SCRCMD_CHECK_BONUS()
{
    s16 result = ScriptThread::GetBonusResult(gBasePtr->mIndex);
    mCondResult = false;

    switch (gBasePtr->mType)
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e120)
void ScriptThread::Handle_SETUP_MODEL_CHECK()
{
    gCarManager_6C->mModelToCheckDestroy = gBasePtr->mIndex;
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e150)
void ScriptThread::Handle_MODEL_CHECK()
{
    if (gCarManager_6C->mModelCheckDestroyed)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e190)
void ScriptThread::GetSpeed()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pCounterPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    // 9.6f 0x47D070. The cases are in ascending order (the layout doesn't follow it): that order gives
    // the original's inline cut-offs, GetLength_41E260 inlined (with one Abs) for the char's car and
    // called out of line (0x453590) for the car, behind the get_car_velocity wrappers.
    switch (gBasePtr->mType)
    {
        case SCRCMD_GET_CAR_SPEED:
            if (pPointer->mCar->mPhysics)
            {
                pCounterPointer->mCounter = pPointer->mCar->mPhysics->get_car_velocity().get_value();
            }
            else
            {
                pCounterPointer->mCounter = kFpZero_6F77C0.ToInt();
            }
            break;

        case SCRCMD_GET_CHAR_CAR_SPEED:
            if (pPointer->mChar->has_car())
            {
                if (pPointer->mChar->get_car()->mPhysics)
                {
                    pCounterPointer->mCounter =
                        pPointer->mChar->get_car()->mPhysics->get_car_velocity().get_value();
                }
                else
                {
                    pCounterPointer->mCounter = kFpZero_6F77C0.ToInt();
                }
            }
            break;

        case SCRCMD_GET_MAX_SPEED:
            pCounterPointer->mCounter = pPointer->mCar->GetMaxSpeed().get_value();
            break;
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e360)
void ScriptThread::Handle_CHECK_CAR_SPEED()
{
    SCR_CHECK_CAR_SPEED* pCmd = reinterpret_cast<SCR_CHECK_CAR_SPEED*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (pPointer->mCar->mPhysics &&
        pPointer->mCar->mPhysics->get_car_velocity().get_value() >
            pCmd->mValue)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e460)
void ScriptThread::Handle_SET_CAR_GRAPHIC()
{
    SCR_SET_CAR_GRAPHIC* pCmd = reinterpret_cast<SCR_SET_CAR_GRAPHIC*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    pPointer->mCar->SetCarGraphic(static_cast<char_type>(pCmd->mNumber)); // set the number on the top of the car
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e4a0)
void ScriptThread::Handle_CHAR_ARRESTED()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pPointer->mChar->mBf21C.bBusted)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e4f0)
void ScriptThread::SCRCMD_S_IS_S_OP_S()
{
    SCR_OPERATE_COUNTER_AND_COUNTER* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_COUNTER*>(gBasePtr);
    u8 type = ScriptThread::GetOperationType(gBasePtr->mType);

    SCR_POINTER* pDestCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mDestinationCounterIdx));
    SCR_POINTER* pFirstOperand = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mFirstOperandCounterIdx));
    SCR_POINTER* pSecondOperand = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mSecondOperandCounterIdx));
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e610)
void ScriptThread::SCRCMD_S_IS_S_OP_I()
{
    SCR_OPERATE_COUNTER_AND_OPERAND_INT* pCmd = reinterpret_cast<SCR_OPERATE_COUNTER_AND_OPERAND_INT*>(gBasePtr);
    SCR_POINTER* pDestCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mDestinationCounterIdx));
    SCR_POINTER* pOperandCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mOperandCounterIdx));

    switch (ScriptThread::GetOperationType(gBasePtr->mType))
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e730)
void ScriptThread::Handle_CHAR_DRIVE_AGGR()
{
    SCR_SET_CHAR_DRIVE_AGGR* pCmd = reinterpret_cast<SCR_SET_CHAR_DRIVE_AGGR*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    pPointer->mChar->mBf21C.bDriveAggressively = pCmd->mStatus;
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e780)
void ScriptThread::SCRCMD_SET_SPEED()
{
    SCR_SET_RUN_SPEED* pCmd = reinterpret_cast<SCR_SET_RUN_SPEED*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (gBasePtr->mType == SCRCMD_SET_RUN_SPEED)
    {
        pPointer->mChar->mMaxSpeed = pCmd->mValue;
    }
    else
    {
        pPointer->mChar->mRunSpeed = pCmd->mValue;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e7f0)
void ScriptThread::Handle_GIVE_CAR_ALARM()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    pPointer->mCar->mFlags |= car_flag::CarAlarm;
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e820)
void ScriptThread::SCRCMD_CAR_WEAPON_KIND_PROOF()
{
    SCR_SET_CAR_DAMAGE_PROOF* pCmd = reinterpret_cast<SCR_SET_CAR_DAMAGE_PROOF*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e900)
void ScriptThread::Handle_PUT_CAR_ON_TRAILER()
{
    SCR_PUT_CAR_ON_TRAILER* pCmd = reinterpret_cast<SCR_PUT_CAR_ON_TRAILER*>(gBasePtr);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pDstCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mTrailerCarIdx));

    Car* pDstCar = pDstCarPointer->mCar;
    if (pDstCar->mDespawnStatus != car_despawn_status::despawned_6)
    {
        if (pDstCar->IsTrailerCab())
        {
            // put car on the trailer attached to the truck cab
            pDstCar->GetSprite()->AttachSprite(pCarPointer->mCar->mCarSprite,
                                                                       kFpZero_6F77C0,
                                                                       kFpZero_6F77C0,
                                                                       kAng180_6F771E);
        }
        else
        {
            // put car directly on the trailer
            pDstCar->mCarSprite->AttachSprite(pCarPointer->mCar->mCarSprite,
                                                                        kFpZero_6F77C0,
                                                                        kFpZero_6F77C0,
                                                                        kAng180_6F771E);
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e9a0)
void ScriptThread::Handle_CLEAR_BRIEFS()
{
    gHud_2B00->mBrief.ClearAllBriefsWithPriority(hud_brief_priority::normal_1);
    gHud_2B00->mBrief.ClearAllBriefsWithPriority(hud_brief_priority::now_3);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50e9e0)
void ScriptThread::Handle_CHECK_HEADS()
{
    SCR_CHECK_HEADS_GREATER* pCmd = reinterpret_cast<SCR_CHECK_HEADS_GREATER*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (pPointer->mChar->get_wanted_star_count() > pCmd->mWantedLevel)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ea40)
void ScriptThread::Handle_FINISH_LEVEL()
{
    SCR_FINISH_LEVEL* pCmd = reinterpret_cast<SCR_FINISH_LEVEL*>(gBasePtr);
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

    s32* num_passed_flag = gScriptManager->mPassedFlag;
    if (num_passed_flag != NULL && gScriptManager->mTotalMissions == *num_passed_flag)
    {
        bonus_type = 2;
    }
    if (bonus_type)
    {
        gGame_0x40->ExitGame(0, GameExitType::AreaCompleted_4, bonus_type);
    }
    else
    {
        gGame_0x40->ExitGameNoBonus(0, GameExitType::AreaCompleted_4);
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50eb00)
void ScriptThread::Handle_CHECK_WEAPONHIT()
{
    SCR_CHECK_WEAPONHIT* pCmd = reinterpret_cast<SCR_CHECK_WEAPONHIT*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (!pCmd->mStatus)
    {
        gScriptManager->TestAndRecordWeaponHit(pPointer->mChar->get_id(), static_cast<char_type>(pCmd->mWeapon), true);
        mCondResult = false;
        pCmd->mStatus = 1;
    }
    else
    {
        if (gScriptManager->IsWeaponHitRecorded(pPointer->mChar->get_id(), static_cast<char_type>(pCmd->mWeapon), true))
        {
            mCondResult = true;
            gScriptManager->ClearWeaponHitFlag(pPointer->mChar->get_id(), true);
            pCmd->mStatus = 0;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ebd0)
void ScriptThread::Handle_WEAP_HIT_CAR()
{
    SCR_CHECK_WEAPONHIT* pCmd = reinterpret_cast<SCR_CHECK_WEAPONHIT*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (!pCmd->mStatus)
    {
        bool hit;
        if (gBasePtr->mType == SCRCMD_WEAP_HIT_CAR)
        {
            hit = gScriptManager->TestAndRecordWeaponHit(pPointer->mCar->mCarId, static_cast<char_type>(pCmd->mWeapon), false);
        }
        else
        {
            hit = gScriptManager->TestAndRecordWeaponHit(pPointer->mCar->mCarId, ped_death_cause::any_23, false);
        }
        if (hit && gScriptManager->IsWeaponHitRecorded(pPointer->mCar->mCarId, static_cast<char_type>(pCmd->mWeapon), false))
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
        if (gScriptManager->IsWeaponHitRecorded(pPointer->mCar->mCarId, static_cast<char_type>(pCmd->mWeapon), false))
        {
            mCondResult = true;
            pCmd->mStatus = 0;
        }
        else
        {
            mCondResult = false;
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ece0)
void ScriptThread::Handle_IS_CHAR_ON_FIRE()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    if (pPointer->mChar->IsOnFire())
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ed40)
void ScriptThread::Handle_BRIEF_ONSCREEN()
{
    Hud_BriefEntry* pCurrBrief = gHud_2B00->mBrief.mCurrBriefs;
    if (pCurrBrief != NULL && pCurrBrief->mBriefPriority != 0)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ed80)
void ScriptThread::Handle_CREATE_SOUND()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    ScriptThread::Handle_SOUND_DECSET(reinterpret_cast<SCR_SOUND_DECSET*>(gBasePtr), pPointer);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50edc0)
void ScriptThread::Handle_DO_EASY_PHONE() //  EASY_PHONE_TEMPLATE
{
    SCR_PHONE_TEMPLATE* pCmd = reinterpret_cast<SCR_PHONE_TEMPLATE*>(gBasePtr);

    SCR_POINTER* pGang1MissionFlag;
    SCR_POINTER* pGang2MissionFlag;
    SCR_POINTER* pGang3MissionFlag;
    SCR_POINTER* pPassedFlag;
    SCR_POINTER* pFailedFlag;

    pPassedFlag = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter1));
    pFailedFlag = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter2));
    pGang1MissionFlag = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter3));
    pGang2MissionFlag = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter4));
    pGang3MissionFlag = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounter5));
    str_table_entry* pGangStr = gScriptManager->FindStringById(pCmd->mGangname);
    char_type* pGangName = pGangStr->get_name();
    Gang* pGang = gGangPool_CA8->FindGangByName(pGangName);
    if (pGang1MissionFlag->mCounter == 1) //  flag_on_[gang1]_mission == 1
    {
        s32 brief_id = pCmd->mBriefId - 5;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }
    if (pGang2MissionFlag->mCounter == 1) //  flag_on_[gang2]_mission == 1
    {
        s32 brief_id = pCmd->mBriefId - 2;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }
    if (pGang3MissionFlag->mCounter == 1) //  flag_on_[gang3]_mission == 1
    {
        s32 brief_id = pCmd->mBriefId - 1;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }
    if (pPassedFlag->mCounter == 1) //  flag_passed_[gang1]_easy_phone_m1 == 1
    {
        //  not used in original levels, since phones are disabled after finishing mission
        s32 brief_id = pCmd->mBriefId;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }
    if (pFailedFlag->mCounter == 1) //  flag_failed_[this_gang]_easy_phone_m1 == 1
    {
        //  not used in the game as you can repeat a failed mission in GTA2
        s32 brief_id = pCmd->mBriefId + 1;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }
    s8 player_idx1;
    //  Check if the player has respect enough to launch the mission
    if ((player_idx1 = gGame_0x40->mLocalPlayer->get_idx(),
         pGang->GetRespectForPlayer(player_idx1) / 20 >= pCmd->mRespect))
    {
        str_table_entry* mission1_str_table = gScriptManager->FindStringById(pCmd->mMission1);
        char_type* mission1_name = mission1_str_table->get_name();
        ScriptThread::launch_mission(gBasePtr, mission1_name);
        return;
    }
    s8 player_idx2;
    //  Check if the player has respect = 0
    if ((player_idx2 = gGame_0x40->mLocalPlayer->get_idx(), (pGang->GetRespectForPlayer(player_idx2) / 20) == 0))
    {
        s32 brief_id = pCmd->mBriefId - 6;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
        return;
    }
    s8 player_idx3;
    //  Check if the player has negative respect
    if ((player_idx3 = gGame_0x40->mLocalPlayer->get_idx(), pGang->GetRespectForPlayer(player_idx3) / 20 < 0))
    {
        s32 brief_id = pCmd->mBriefId - 4;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
    }
    else
    {
        //  In this case, the player has positive respect but not enough to launch the mission
        s32 brief_id = pCmd->mBriefId - 7;
        sprintf(gTmpBuffer, "%d", brief_id);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        ScriptThread::Next(gBasePtr);
    }
}

MATCH_FUNC(0x50f060)
void ScriptThread::Handle_CHAR_INTO_CAR()
{
    SCR_CHAR_INTO_CAR* pCmd = reinterpret_cast<SCR_CHAR_INTO_CAR*>(gBasePtr);
    SCR_POINTER* pCharPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCharIdx));
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCarIdx));

    Ped* pNewPed = gPedManager->AllocatePed();
    pCharPointer->mChar = pNewPed;

    if (pNewPed)
    {
        pNewPed->SetPedType(ped_type::script_created_5);
        pCharPointer->mChar->SpawnPedInCar(pCarPointer->mCar);
        pCharPointer->mChar->set_remap(pCmd->mRemap);
        pCharPointer->mChar->set_occupation(pCmd->mOccupation);

        if (!mInLevelStart)
        {
            gMiss2_25C->push_type_3_ped(pCharPointer->mChar);
        }
    }

    Car* pCarTest = pCarPointer->mCar;

    if (pCarTest)
    {
        pCarTest->SetDriverPedType(ped_type::script_created_5);

        Car* pCar = pCarPointer->mCar;

        pCar->AllocateAI_421510();

        pCarPointer->mCar->mAI->SetCar(pCarPointer->mCar);
        pCarPointer->mCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f150)
void ScriptThread::Handle_DECIDE_POWERUP()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pCarPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pGeneratorPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));

    Car* pCar = pCarPointer->mCar;
    u8* pCarList = reinterpret_cast<u8*>(gScriptManager->mCarList);

    u16 i;

    for (i = 0; i < 19; i++)
    {
        if (pCar != NULL && pCar->mCarInfoIdx == *pCarList)
        {
            pGeneratorPointer->mGenerator->mGenType = kDecidePowerupGenTypes[i];
            Generator* pGen = pGeneratorPointer->mGenerator;

            if (kDecidePowerupGenTypes[i] < 91)
            {
                pGen->Activate();
                pGen->mSpawnsLeft = 3;
            }
            else
            {
                pGen->Activate();
                pGen->mSpawnsLeft = 1;
            }

            break;
        }

        pCarList++;
    }

    if (i == 19)
    {
        Generator* pGen = pGeneratorPointer->mGenerator;
        pGen->Activate();
        pGen->mSpawnsLeft = 3;
        pGeneratorPointer->mGenerator->mGenType = 65;
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f220)
void ScriptThread::Handle_SUPPRESS_MODEL()
{
    if (gBasePtr->mIndex == 0xFFFFu)
    {
        gCarManager_6C->mSuppressedModel = car_model_enum::none; //  No model suppressed
    }
    else
    {
        gCarManager_6C->mSuppressedModel = gBasePtr->mIndex;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f270)
void ScriptThread::Handle_WARP_CHAR() // WARP_FROM_CAR_TO_POINT
{
    SCR_WARP_FROM_CAR* pCmd = reinterpret_cast<SCR_WARP_FROM_CAR*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;
    Car* pCar = pPed->get_car();

    if (pPed != NULL)
    {
        pPed->StartPedWalking(pCmd->mPos.x, pCmd->mPos.y, pCmd->mPos.z);

        Ang16 rotation = kDegreesToAng16 * pCmd->mRotation;
        rotation.Normalize();

        pPointer->mChar->SetRotation(rotation);
        pCar->ClearDriver();
        pCar->mDriver = NULL;
        gGame_0x40->mLocalPlayer->DisableInputs();
        gGame_0x40->mLocalPlayer->mGameCamera.UpdateFollowPedCamera(pPointer->mChar);

        Camera* pCamera = &(gGame_0x40->mLocalPlayer->mGameCamera);

        pCamera->CommitPendingTarget();

        gGame_0x40->mLocalPlayer->mGameCamera.ResetCameraSmoothing();
        gGame_0x40->mLocalPlayer->mGameCamera.SetSmoothFollow();
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f3d0)
void ScriptThread::Handle_SET_GROUP_TYPE()
{
    SCR_SET_GROUP_TYPE* pCmd = reinterpret_cast<SCR_SET_GROUP_TYPE*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;
    PedGroup* pGroup = pPed->GetGroup();

    if (pGroup != NULL)
    {
        pGroup->mGroupType = pCmd->mGroupType;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f410)
void ScriptThread::Handle_CHAR_DO_NOTHING()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        pPed->ForceDoNothing();
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f450)
void ScriptThread::Handle_EMERG_LIGHTS()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (pPointer->mCar->HasEmergencyLights())
    {
        if (static_cast<u8>(pCmd->mUnsigned2) == 1)
        {
            pPointer->mCar->ActivateEmergencyLights(); //  activate sirens
        }
        else
        {
            pPointer->mCar->DeactivateEmergencyLights(); //  deactivate sirens
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f4d0)
void ScriptThread::Handle_CHECK_OBJ_MODEL()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f550)
void ScriptThread::Handle_PED_GRAPHIC()
{
    SCR_SET_CHAR_GRAPHIC_TYPE* pCmd = reinterpret_cast<SCR_SET_CHAR_GRAPHIC_TYPE*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (pPointer->mChar != NULL)
    {
        pPointer->mChar->mGraphicType = pCmd->mGraphicType;
        pPointer->mChar->set_remap(static_cast<char_type>(pCmd->mRemap));
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f5e0)
void ScriptThread::SCRCMD_SET_CHAR_FLAGS()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr);
    Ped* pPed = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(reinterpret_cast<SCR_SET_STATE*>(gBasePtr)->mVarname))->mChar;

    if (pPed)
    {
        switch (gBasePtr->mType)
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
                gScriptManager->mDeathArrState = pCmd->mStatus;
                break;
        }
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f770)
void ScriptThread::Handle_MAKE_MUGGERS()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr);
    if (pCmd->mStatus == 1)
    {
        gPedManager->mMakeAllMuggers = true;
    }
    else
    {
        gPedManager->mMakeAllMuggers = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f7b0)
void ScriptThread::LocSecChar()
{
    SCR_LOC_SEC_CHAR* pCmd = reinterpret_cast<SCR_LOC_SEC_CHAR*>(gBasePtr);
    SCR_POINTER* pChar1 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mChar1Idx));
    SCR_POINTER* pChar2 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mChar2Idx));

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
        switch (gBasePtr->mType)
        {
            case SCRCMD_LOC_SECOND_CHAR:
                if (pChar1->mChar->bHasGameObject_403B70())
                {
                    mCondResult = true;
                }

                break;
            case SCRCMD_LOC_SEC_CHAR_CAR:
                if (pChar1->mChar->has_car())
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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f900)
void ScriptThread::Handle_STOP_CAR_DRIVE()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Car* pCar = pPointer->mChar->get_car();

    if (pCar)
    {
        pCar->SetAIHalted();
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f940)
void ScriptThread::Handle_IS_BUS_FULL()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    mCondResult = false;
    Ped* pPed = pPointer->mChar;

    if (pPed && pPed->has_car() && gPublicTransport_181C->is_bus(pPed->get_car()) &&
        gPublicTransport_181C->is_bus_full())
    {
        mCondResult = true;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50f9b0)
void ScriptThread::Handle_NO_CHARS_OFF_BUS()
{
    SCR_SET_STATE* pCmd = reinterpret_cast<SCR_SET_STATE*>(gBasePtr);
    if (pCmd->mStatus == 1)
    {
        gPublicTransport_181C->mStopGettingOffBus = true;
    }
    else
    {
        gPublicTransport_181C->mStopGettingOffBus = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fa00)
void ScriptThread::Handle_KILL_CHAR()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        pPed->Kill(); // kill ped
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fa40)
void ScriptThread::Handle_SET_SHADING_LEV()
{
    gpMapRenderer->set_shading_lev(static_cast<u8>(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mUnsigned2));
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fa70)
void ScriptThread::Handle_SET_CAR_JAMMED()
{
    SCR_SET_CAR_JAMMED* pCmd = reinterpret_cast<SCR_SET_CAR_JAMMED*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
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
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fad0)
void ScriptThread::Handle_FINISH_MISSION()
{
    gMiss2_25C->MissionCleanUp();
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50faf0)
void ScriptThread::Handle_STORE_BONUS()
{
    SCR_STORE_BONUS_COUNT* pCmd = reinterpret_cast<SCR_STORE_BONUS_COUNT*>(gBasePtr);
    SCR_POINTER* pPointerBonus = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pPointerCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounterIdx));

    s32 bonus_rule_idx = pPointerBonus->mCounter;

    if (bonus_rule_idx != bonus_result::passed && bonus_rule_idx != bonus_result::failed)
    {
        pPointerCounter->mCounter =
            gGame_0x40->mLocalPlayer->mScores.mBonuses.GetProgressCount(pPointerBonus->mIndex);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fb60)
void ScriptThread::Handle_SET_ENTER_STATUS() // SCRCMD_SET_ENTER_STATUS and SCRCMD_SET_ALL_CONTROLS
{
    SCR_SET_PLAYER_CONTROLS* pCmd = reinterpret_cast<SCR_SET_PLAYER_CONTROLS*>(gBasePtr);

    gScriptManager->GetBasePointer(gBasePtr->mIndex);

    // The player pointer is not used, so the user controls are modified instead
    // This explains desync in multiplayer when SET_ENTER_CONTROL_STATUS are used

    if (pCmd->mNewState == 1)
    {
        if (gBasePtr->mType == SCRCMD_SET_ENTER_STATUS)
        {
            gGame_0x40->mLocalPlayer->EnableEnterVehicles(); // SET_ENTER_STATUS = ON
        }
        else
        {
            gGame_0x40->mLocalPlayer->EnableAllControls(); // SET_ALL_CONTROLS = ON
        }
    }
    else if (gBasePtr->mType == SCRCMD_SET_ENTER_STATUS)
    {
        gGame_0x40->mLocalPlayer->DisableEnterVehicles(); // SET_ENTER_STATUS = OFF
    }
    else
    {
        gGame_0x40->mLocalPlayer->DisableAllControls(); // SET_ALL_CONTROLS = OFF
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fc20)
void ScriptThread::Handle_SET_FAV_CAR()
{
    SCR_SET_FAV_CAR* pCmd = reinterpret_cast<SCR_SET_FAV_CAR*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pPed = pPointer->mChar;

    if (pPed != NULL)
    {
        pPed->mGangCarModel = pCmd->mFavouriteCar;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fc60)
void ScriptThread::GroupInArea()
{
    SCR_ONEVAR_RECT* pCmd = reinterpret_cast<SCR_ONEVAR_RECT*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Fix16 halfwidth = pCmd->mRect.mSize.mX / kFpTwo_6F77C8;
    Fix16 halfheight = pCmd->mRect.mSize.mY / kFpTwo_6F77C8;

    PedGroup* pGroup = pPointer->mChar->GetGroup();

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

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fe00)
void ScriptThread::Handle_SET_KF_WEAPON()
{
    SCR_SET_KF_WEAPON* pCmd = reinterpret_cast<SCR_SET_KF_WEAPON*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    s32 weapon_idx = pCmd->mWeaponIdx;

    if (is_car_weapon(weapon_idx))
    {
        pPointer->mChar->mPlayer->SetKFCarWeapon(pPointer->mChar->get_car(), weapon_idx);
    }
    else
    {
        pPointer->mChar->mPlayer->SetKFWeapon(weapon_idx);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50fed0)
void ScriptThread::Handle_CLEAR_KF_WEAPON()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Player* pPlayer = pPointer->mChar->mPlayer;

    pPlayer->ClearKFWeapon(); // clear kill frenzy weapon
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ff50)
void ScriptThread::Handle_ADD_ONSCREEN_COUNTER()
{
    SCR_ADD_ONSCREEN_COUNTER* pCmd = reinterpret_cast<SCR_ADD_ONSCREEN_COUNTER*>(gBasePtr);
    SCR_POINTER* pOnScreenCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCounterIdx));

    pOnScreenCounter->mIndex = gHud_2B00->mPagers.AddOnScreenCounter(&pCounter->mCounter);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x50ffb0)
void ScriptThread::Handle_CLEAR_COUNTER() // SCRCMD_CLEAR_COUNTER and SCRCMD_CLEAR_CLOCK_ONLY
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    u16 idx = pPointer->mIndex;

    if (idx != 0xFFFF)
    {
        if (gBasePtr->mType == SCRCMD_CLEAR_COUNTER)
        {
            gHud_2B00->mPagers.ClearCounterOnly(idx);
        }
        else // == SCRCMD_CLEAR_CLOCK_ONLY
        {
            gHud_2B00->mPagers.ClearClockOnly(idx);
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x510030)
void ScriptThread::Handle_CHANGE_POLICE()
{
    ScriptThread::Handle_DECLARE_POLICE(reinterpret_cast<SCR_DECLARE_POLICELEVEL*>(gBasePtr));
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x510050)
void ScriptThread::Handle_DESTROY_GROUP()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    Ped* pGroupLeader = pPointer->mChar;
    PedGroup* pGroup = pGroupLeader->mPedGroup;

    if (pGroup)
    {
        pGroup->DestroyGroup(); // destroy group
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x510090)
void ScriptThread::Handle_CHECK_CURRENT_WEAPON()
{
    SCR_CHECK_CURRENT_WEAPON* pCmd = reinterpret_cast<SCR_CHECK_CURRENT_WEAPON*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    Weapon* pWeapon = pPointer->mChar->mPlayer->GetCurrPlayerWeapon();

    if (pWeapon != NULL && static_cast<u32>(pWeapon->mWeaponType) == pCmd->mWeaponIdx)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x510100)
void ScriptThread::Handle_START_BASIC_KF() // START_BASIC_KF_TEMPLATE
{
    SCR_START_BASIC_KF_TEMPLATE* pCmd = reinterpret_cast<SCR_START_BASIC_KF_TEMPLATE*>(gBasePtr);
    if (gScriptManager->IsKillFrenzyActive())
    {
        ScriptThread::EnableThread(pCmd->mTriggername);
        ScriptThread::Handle_RETURN();
    }
    else
    {
        gScriptManager->SetKillFrenzyActive(true);
        ScriptThread::DisableThread(pCmd->mTriggername);
        gHud_2B00->mMessage.ShowMessage(gText_0x14->Find("kfstart"), hud_message_priority::important_3); // KILL FRENZY!
        gRoot_sound.PlayVoice(voice_line::killfrenzy_12); // KILL FRENZY voice
        SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mObjname)); // Skull icon?
        if (ScriptThread::GetEntityTypeOfCommand(pPointer->mType) == script_entity_type::object_3)
        {
            ScriptThread::DeallocOrDeleteItem(pCmd->mObjname); // Delete skull icon?
        }
        sprintf(gTmpBuffer, "%d", pCmd->mBriefId);
        gHud_2B00->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, gTmpBuffer);
        SCR_POINTER* pPlayerPedPtr = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mPlayername));
        u8 weapon_idx = static_cast<u8>(pCmd->mWeapon);
        if (weapon_idx < weapon_type::car_bomb || weapon_idx > 27)
        {
            // Ped weapons
            if (weapon_idx != 28)
            {
                pPlayerPedPtr->mChar->mPlayer->SetKFWeapon(static_cast<u8>(pCmd->mWeapon));
            }
        }
        else
        {
            // Car weapons
            pPlayerPedPtr->mChar->mPlayer->SetKFCarWeapon(pPlayerPedPtr->mChar->get_car(),
                                                                                 static_cast<u8>(pCmd->mWeapon));
        }

        u16* pBasicKF = gScriptManager->mBasicKf;
        for (u16 idx = 0; idx < GTA2_COUNTOF(gScriptManager->mBasicKf); idx++)
        {
            if (*pBasicKF == static_cast<u16>(gBasePtr->mCmdThis))
            {
                gGameSave.mCarAndScriptData.mStartedBasicKfMask |= 1 << idx;
                break;
            }
            pBasicKF++;
        }
        ScriptThread::Next(gBasePtr);
    }
}

MATCH_FUNC(0x510280)
void ScriptThread::Handle_DO_BASIC_KF()
{
    SCR_POINTER* pTimerIDPointer;

    SCR_DO_BASIC_KF_TEMPLATE* pCmd = reinterpret_cast<SCR_DO_BASIC_KF_TEMPLATE*>(gBasePtr);
    SCR_POINTER* pPlayerPedCmdPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mPlayername));

    if (!mCmdInitialised)
    {
        pTimerIDPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mTimername));
        pTimerIDPointer->mIndex = gHud_2B00->mPagers.CreateTimer(pCmd->mTimeLimit);
        SCR_POINTER* pOnScreenCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mOnscreenname));
        SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCountername));
        pOnScreenCounter->mIndex = gHud_2B00->mPagers.AddOnScreenCounter(&pCounter->mCounter);
        ++mCmdInitialised;
    }
    else
    {
        if (ScriptThread::GetBonusResult(pCmd->mBonusname) == bonus_result::in_progress)
        {
            SCR_POINTER* pBonusType = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mBonusname));
            SCR_POINTER* pCounter = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mCountername));
            pCounter->mCounter = pCmd->mTargetTotal -
                gGame_0x40->mLocalPlayer->mScores.mBonuses.GetProgressCount(pBonusType->mIndex);

            Ped* pPed = pPlayerPedCmdPointer->mChar;

            if (pPed->GetPedState() == ped_state_1::dead_9 || pPed->mBf21C.bBusted != 0)
            {
                gGame_0x40->mLocalPlayer->mScores.mBonuses.DeactivateBonus(pBonusType->mIndex);
                pPlayerPedCmdPointer->mChar->mPlayer->ClearKFWeapon();
            }
        }
        else
        {
            pTimerIDPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mTimername));
            if (pTimerIDPointer->mIndex != 0xFFFF)
            {
                gHud_2B00->mPagers.ClearPager(pTimerIDPointer->mIndex);
            }
            pTimerIDPointer->mIndex = 0;
            pPlayerPedCmdPointer->mChar->mPlayer->ClearKFWeapon();
            gScriptManager->SetKillFrenzyActive(false);
            if (ScriptThread::GetBonusResult(pCmd->mBonusname) == -3)
            {
                gHud_2B00->mMessage.ShowMessage(gText_0x14->Find("kfpass"), hud_message_priority::important_3);
                pPlayerPedCmdPointer->mChar->ClearWantedPoints();
                ++*gScriptManager->mSecretsPassed;

                if (pCmd->mRewardtype == 1)
                {
                    pPlayerPedCmdPointer->mChar->mPlayer->AddScore(pCmd->mRewardvalue);
                    gRoot_sound.PlayVoice(voice_line::frenzypassed_19);
                }
                else if (pCmd->mRewardtype == 2)
                {
                    pPlayerPedCmdPointer->mChar->mPlayer->ChangeMultipliers(pCmd->mRewardvalue);
                    gRoot_sound.PlayVoice(voice_line::frenzypassed_19);
                }
                else
                {
                    if (pCmd->mRewardtype == 3)
                    {
                        pPlayerPedCmdPointer->mChar->mPlayer->ChangeLifeCountByAmount(pCmd->mRewardvalue);
                    }
                    gRoot_sound.PlayVoice(voice_line::frenzypassed_19);
                }
            }
            else
            {
                gHud_2B00->mMessage.ShowMessage(gText_0x14->Find("kffail"), hud_message_priority::important_3);
                ++*gScriptManager->mSecretsFailed;
                gRoot_sound.PlayVoice(voice_line::frenzyfail_18);
            }
            ScriptThread::Next(gBasePtr);
        }
    }
}

MATCH_FUNC(0x510530)
void ScriptThread::Handle_SET_BONUS_RATING()
{
    gGameSession.SetBonusRatingTextIdx(reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr)->mUnsigned2);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x510560)
void ScriptThread::Handle_ADD_TIME()
{
    SCR_FOUR_PARAMS* pCmd = reinterpret_cast<SCR_FOUR_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    gHud_2B00->mPagers.AddTime(pPointer->mIndex, 30 * pCmd->mU32);
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5105b0)
void ScriptThread::Handle_SET_MODEL_WANTED()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    s16 model_idx = pCmd->mSigned2;

    if (model_idx == -1)
    {
        gScriptManager->mWantedCarModel = car_model_enum::none;
    }
    else
    {
        gScriptManager->mWantedCarModel = model_idx;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x510600)
void ScriptThread::Handle_CHECK_DEATH_ARR()
{
    if (gScriptManager->mDeathArrestFlag)
    {
        mCondResult = true;
    }
    else
    {
        mCondResult = false;
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x510660)
void ScriptThread::Handle_FORCE_CLEANUP()
{
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));

    if (ScriptThread::GetEntityTypeOfCommand(pPointer->mType) == script_entity_type::object_3)
    {
        gMiss2_25C->push_type_2_non_pickup_object(pPointer->mObj, true);
    }
    else if (ScriptThread::GetEntityTypeOfCommand(pPointer->mType) == script_entity_type::sound_10)
    {
        gMiss2_25C->push_type_2_object(pPointer->mObj);
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x510780)
void ScriptThread::SCRCMD_SAVE_RESTORE_RESPECT()
{
    u8 idx = 0;
    Gang* pGang = gGangPool_CA8->FirstGang();

    if (gBasePtr->mType == SCRCMD_SAVE_RESPECT)
    {
        for (; pGang != NULL; pGang = gGangPool_CA8->NextGang())
        {
            gScriptManager->mSavedGangRespect[idx++] = pGang->GetRespectForPlayer(0);
        }
    }
    else
    {
        for (; pGang != NULL; pGang = gGangPool_CA8->NextGang())
        {
            pGang->SetRespect(0, gScriptManager->mSavedGangRespect[idx++]);
        }
    }
    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x5108d0)
void ScriptThread::PreExecOpCode()
{
    if (mStopped != 1)
    {
        SCR_CMD_HEADER* pCmd = gScriptManager->GetBasePointer(mNextCmd);
        gBasePtr = static_cast<SCR_POINTER*>(pCmd);
        switch (pCmd->mType)
        {
            case SCRCMD_LEVELSTART:
                Handle_LEVELSTART();
                break;
            case SCRCMD_LEVELEND:
                Handle_LEVELEND();
                break;
            case SCRCMD_CREATE_THREAD:
                Handle_CREATE_THREAD();
                break;
            case SCRCMD_STOP_THREAD:
                Handle_STOP_THREAD();
                break;
            case SCRCMD_IF_JUMP:
                Handle_IF_JUMP();
                break;
            case SCRCMD_NOT:
                Handle_NOT();
                break;
            case SCRCMD_GOSUB:
                Handle_GOSUB();
                break;
            case SCRCMD_START_EXEC:
                Handle_START_EXEC();
                break;
            case SCRCMD_STOP_EXEC:
                Handle_STOP_EXEC();
                break;
            case SCRCMD_S_PLUS_S:
            case SCRCMD_S_MINUS_S:
            case SCRCMD_S_LESS_S:
            case SCRCMD_S_LEQUAL_S:
            case SCRCMD_S_GREATER_S:
            case SCRCMD_S_GEQUAL_S:
            case SCRCMD_S_EQUAL_S:
                SCRCMD_S_OP_S();
                break;
            case SCRCMD_I_PLUS_S:
            case SCRCMD_I_MINUS_S:
                SCRCMD_I_OP_S();
                break;
            case SCRCMD_S_PLUS_I:
            case SCRCMD_S_MINUS_I:
            case SCRCMD_S_LESS_I:
            case SCRCMD_S_LEQUAL_I:
            case SCRCMD_S_GREATER_I:
            case SCRCMD_S_GEQUAL_I:
            case SCRCMD_S_EQUAL_I:
                SCRCMD_S_OP_I();
                break;
            case SCRCMD_INCREMENT:
                Handle_INCREMENT();
                break;
            case SCRCMD_DECREMENT:
                Handle_DECREMENT();
                break;
            case SCRCMD_CREATE_OBJ_2D:
            case SCRCMD_CREATE_OBJ_3D:
            case SCRCMD_CREATE_OBJ_3D_INT:
            case SCRCMD_CREATE_OBJ_2D_INT:
            case SCRCMD_CREATE_OBJ_3D_STR:
            case SCRCMD_CREATE_OBJ_2D_STR:
                SCRCMD_CREATE_OBJ();
                break;
            case SCRCMD_CREATE_CAR_2D:
            case SCRCMD_CREATE_CAR_3D:
            case SCRCMD_CREATE_CAR_2D_STR:
            case SCRCMD_CREATE_CAR_3D_STR:
            case SCRCMD_CREATE_GANG_CAR1:
            case SCRCMD_CREATE_GANG_CAR2:
            case SCRCMD_CREATE_GANG_CAR3:
            case SCRCMD_CREATE_GANG_CAR4:
                SCRCMD_CREATE_CAR();
                break;
            case SCRCMD_MAKE_CAR_DUMMY:
                Handle_MAKE_CAR_DUMMY();
                break;
            case SCRCMD_CREATE_CHAR_2D:
            case SCRCMD_CREATE_CHAR_3D:
                SCRCMD_CREATE_CHAR();
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
                SCRCMD_SET_DENSITY();
                break;
            case SCRCMD_POINT_ARROW_3D:
            case SCRCMD_LEVEL_END_ARROW2:
                Handle_POINT_ARROW_3D();
                break;
            case SCRCMD_ARROW_COLOUR:
                Handle_ARROW_COLOUR();
                break;
            case SCRCMD_POINT_ARROW_AT:
            case SCRCMD_LEVEL_END_ARROW1:
                Handle_POINT_ARROW_AT();
                break;
            case SCRCMD_REMOVE_ARROW:
                Handle_REMOVE_ARROW();
                break;
            case SCRCMD_CHECK_CAR_DAMAGE:
                Handle_CHECK_CAR_DAMAGE();
                break;
            case SCRCMD_CHECK_HEALTH:
                Handle_CHECK_HEALTH();
                break;
            case SCRCMD_STORE_CAR_INFO:
                Handle_STORE_CAR_INFO();
                break;
            case SCRCMD_DISPLAY_MESSAGE:
            case SCRCMD_DISPLAY_BRIEF:
            case SCRCMD_DISPLAY_BRIEF_NOW:
            case SCRCMD_DISPLAY_BRIEF_SOON:
                Handle_DISPLAY_MESSAGE();
                break;
            case SCRCMD_DISPLAY_TIMER:
                Handle_DISPLAY_TIMER();
                break;
            case SCRCMD_SET_CHAR_OBJ1:
                Handle_SET_CHAR_OBJ1();
                break;
            case SCRCMD_IS_CHAR_OBJ_PASS:
                Handle_IS_CHAR_OBJ_PASS();
                break;
            case SCRCMD_IS_CHAR_OBJ_FAIL:
                Handle_IS_CHAR_OBJ_FAIL();
                break;
            case SCRCMD_GIVE_WEAPON1:
            case SCRCMD_GIVE_WEAPON2:
                SCRCMD_GIVE_WEAPON();
                break;
            case SCRCMD_IS_CAR_IN_BLOCK:
                Handle_IS_CAR_IN_BLOCK();
                break;
            case SCRCMD_HAS_CHAR_DIED:
                Handle_HAS_CHAR_DIED();
                break;
            case SCRCMD_IS_CHAR_IN_CAR:
                Handle_IS_CHAR_IN_CAR();
                break;
            case SCRCMD_IS_CHAR_IN_MODEL:
                Handle_IS_CHAR_IN_MODEL();
                break;
            case SCRCMD_IS_CHAR_IN_ANY_CAR:
                Handle_IS_CHAR_IN_ANY_CAR();
                break;
            case SCRCMD_DELETE_ITEM:
                Handle_DELETE_ITEM();
                break;
            case SCRCMD_ADD_SCORE1:
            case SCRCMD_ADDSCORE_NO_MULT:
                SCRCMD_ADD_SCORE();
                break;
            case SCRCMD_IS_CHAR_STOPPED:
                Handle_IS_CHAR_STOPPED();
                break;
            case SCRCMD_EXPLODE:
            case SCRCMD_EXPLODE_LARGE2:
            case SCRCMD_EXPLODE_SMALL2:
            case SCRCMD_EXPLODE_NO_RING2:
                Handle_EXPLODE();
                break;
            case SCRCMD_LOCATE_CHAR_ANY:
            case SCRCMD_LOCATE_CHAR_ONFOOT:
            case SCRCMD_LOCATE_CHAR_BY_CAR:
            case SCRCMD_STOP_LOCATE_CHAR_ANY:
            case SCRCMD_STOP_LOCATE_CHAR_FOOT:
            case SCRCMD_STOP_LOCATE_CHAR_CAR:
                Locate();
                break;
            case SCRCMD_SET_CHAR_OBJ2:
                Handle_SET_CHAR_OBJ2();
                break;
            case SCRCMD_SET_CHAR_OBJ3:
                Handle_SET_CHAR_OBJ3();
                break;
            case SCRCMD_SET_THREAT_SEARCH:
            case SCRCMD_SET_THREAT_REACT:
            case SCRCMD_ADD_GROUP:
            case SCRCMD_SET_CHAR_SHOOT:
            case SCRCMD_SET_CHAR_BRAVERY:
                SCRCMD_THREAT_SEARCH_REACT_ADD_GROUP();
                break;
            case SCRCMD_CHANGE_CAR_REMAP:
                Handle_CHANGE_CAR_REMAP();
                break;
            case SCRCMD_CHANGE_CHAR_REMAP:
                Handle_CHANGE_CHAR_REMAP();
                break;
            case SCRCMD_CHECK_CAR_MODEL:
                Handle_CHECK_CAR_MODEL();
                break;
            case SCRCMD_CHECK_CAR_REMAP:
                Handle_CHECK_CAR_REMAP();
                break;
            case SCRCMD_CHECK_CAR_BOTH:
                Handle_CHECK_CAR_BOTH();
                break;
            case SCRCMD_IS_ITEM_ONSCREEN:
            case SCRCMD_ONSCREEN_ACCURACY:
                IsOnScreen();
                break;
            case SCRCMD_DELAY_HERE:
                Handle_DELAY_HERE();
                break;
            case SCRCMD_DELAY:
                Handle_DELAY();
                break;
            case SCRCMD_ENABLE_THREAD:
                Handle_ENABLE_THREAD();
                break;
            case SCRCMD_DISABLE_THREAD:
                Handle_DISABLE_THREAD();
                break;
            case SCRCMD_SET_GANG_RESPECT:
                SCRCMD_SET_GANG_KILL_REACTION();
                break;
            case SCRCMD_SET_CHAR_RESPECT:
            case SCRCMD_CHANGE_RESPECT:
            case SCRCMD_CHANGE_GANG_RESP:
                Handle_CHANGE_RESPECT();
                break;
            case SCRCMD_CHECK_RESPECT_GREATER:
            case SCRCMD_CHECK_RESPECT_LESS:
            case SCRCMD_CHECK_RESPECT_IS:
                RespectOperator();
                break;
            case SCRCMD_ADD_PATROL_POINT:
                Handle_ADD_PATROL_POINT();
                break;
            case SCRCMD_SET_AMBIENT:
                Handle_SET_AMBIENT();
                break;
            case SCRCMD_ANSWER_PHONE:
                Handle_ANSWER_PHONE();
                break;
            case SCRCMD_CHECK_PHONE:
                Handle_CHECK_PHONE();
                break;
            case SCRCMD_CHECK_PHONETIMER:
                Handle_CHECK_PHONETIMER();
                break;
            case SCRCMD_STOP_PHONE_RING:
                Handle_STOP_PHONE_RING();
                break;
            case SCRCMD_EXPLODE_BUILDING:
                Handle_EXPLODE_BUILDING();
                break;
            case SCRCMD_IS_CHAR_FIRE_ONSCREEN:
                Handle_IS_CHAR_FIRE_ONSCREEN();
                break;
            case SCRCMD_CAR_DRIVE_AWAY:
                Handle_CAR_DRIVE_AWAY();
                break;
            case SCRCMD_CHAR_TO_DRIVE_CAR:
            case SCRCMD_CHAR_TO_BACKDOOR:
                Handle_CHAR_TO_DRIVE_CAR();
                break;
            case SCRCMD_DRIVER_OUT_CAR:
                Handle_DRIVER_OUT_CAR();
                break;
            case SCRCMD_GIVE_DRIVER_BRAKE:
                Handle_GIVE_DRIVER_BRAKE();
                break;
            case SCRCMD_SET_GANG_INFO1:
                Handle_SET_GANG_INFO1();
                break;
            case SCRCMD_CLEAR_TIMERS:
                Handle_CLEAR_TIMERS();
                break;
            case SCRCMD_CHECK_SCORE:
                Handle_CHECK_SCORE();
                break;
            case SCRCMD_GET_SCORE:
                Handle_GET_SCORE();
                break;
            case SCRCMD_IS_CHAR_IN_GANG:
                Handle_IS_CHAR_IN_GANG();
                break;
            case SCRCMD_SET_NO_COLLIDE:
                Handle_SET_NO_COLLIDE();
                break;
            case SCRCMD_IS_CHAR_FIRING_AREA:
                Handle_IS_CHAR_FIRING_AREA();
                break;
            case SCRCMD_GET_PASSENGER_NUM:
                Handle_GET_PASSENGER_NUM();
                break;
            case SCRCMD_CLEAR_WANTED_LEVEL:
                Handle_CLEAR_WANTED_LEVEL();
                break;
            case SCRCMD_CAR_WRECK_IN_LOCATION:
                Handle_CAR_WRECK_IN_LOCATION();
                break;
            case SCRCMD_SEND_CAR_TO_BLOCK:
                Handle_SEND_CAR_TO_BLOCK();
                break;
            case SCRCMD_CHECK_NUM_ALIVE:
                Handle_CHECK_NUM_ALIVE();
                break;
            case SCRCMD_ADD_CHAR_TO_GROUP:
            case SCRCMD_REMOVE_CHAR:
            case SCRCMD_MAKE_LEADER:
                SCRCMD_CHAR_GROUP();
                break;
            case SCRCMD_SET_COUNTER_INT:
                SCRCMD_SET_COUNTER();
                break;
            case SCRCMD_HAS_CAR_WEAPON:
                Handle_HAS_CAR_WEAPON();
                break;
            case SCRCMD_IS_CHAR_HORN:
                Handle_IS_CHAR_HORN();
                break;
            case SCRCMD_CHECK_MAX_PASS:
                Handle_CHECK_MAX_PASS();
                break;
            case SCRCMD_IS_CHAR_IN_ZONE:
                Handle_IS_CHAR_IN_ZONE();
                break;
            case SCRCMD_SET_PHONE_DEAD:
                Handle_SET_PHONE_DEAD();
                break;
            case SCRCMD_IS_TRAILER_ATT:
                Handle_IS_TRAILER_ATT();
                break;
            case SCRCMD_IS_CAR_ON_TRAIL:
                Handle_IS_CAR_ON_TRAIL();
                break;
            case SCRCMD_ENABLE_CRANE:
            case SCRCMD_DISABLE_CRANE:
                SCRCMD_ENABLE_DISABLE_CRANE();
                break;
            case SCRCMD_CAR_GOT_DRIVER:
                Handle_CAR_GOT_DRIVER();
                break;
            case SCRCMD_SPOTTED_PLAYER:
                Handle_SPOTTED_PLAYER();
                break;
            case SCRCMD_GET_LAST_PUNCHED:
                Handle_GET_LAST_PUNCHED();
                break;
            case SCRCMD_IS_CHAR_STUNNED:
                Handle_IS_CHAR_STUNNED();
                break;
            case SCRCMD_KILL_ALL_PASSENG:
                Handle_KILL_ALL_PASSENG();
                break;
            case SCRCMD_IS_GROUP_IN_CAR:
                Handle_IS_GROUP_IN_CAR();
                break;
            case SCRCMD_PUNCHED_SOMEONE:
                Handle_PUNCHED_SOMEONE();
                break;
            case SCRCMD_SET_MIN_ALIVE:
                Handle_SET_MIN_ALIVE();
                break;
            case SCRCMD_ADD_CHAR_TO_GANG:
                Handle_ADD_CHAR_TO_GANG();
                break;
            case SCRCMD_EXPLODE_ITEM:
            case SCRCMD_EXPLODE_LARGE1:
            case SCRCMD_EXPLODE_SMALL1:
            case SCRCMD_EXPLODE_NO_RING1:
                Handle_EXPLODE_ITEM();
                break;
            case SCRCMD_PARK:
            case SCRCMD_PARK_NO_RESPAWN:
                Handle_PARK();
                break;
            case SCRCMD_ALT_WANTED_LEVEL:
            case SCRCMD_ALTER_WANTED_LEVEL:
                Handle_ALT_WANTED_LEVEL();
                break;
            case SCRCMD_BEEN_PUNCHED_BY:
                Handle_BEEN_PUNCHED_BY();
                break;
            case SCRCMD_UPDATE_DOOR:
                Handle_UPDATE_DOOR();
                break;
            case SCRCMD_OPEN_DOOR:
            case SCRCMD_CLOSE_DOOR:
            case SCRCMD_SET_DOOR_AUTO:
            case SCRCMD_SET_DOOR_MANUAL:
                SCRCMD_DOOR();
                break;
            case SCRCMD_REMOVE_WEAPON:
                Handle_REMOVE_WEAPON();
                break;
            case SCRCMD_REMOVE_BLOCK:
                Handle_REMOVE_BLOCK();
                break;
            case SCRCMD_LOWER_LEVEL:
                Handle_LOWER_LEVEL();
                break;
            case SCRCMD_ADD_NEW_BLOCK:
                Handle_ADD_NEW_BLOCK();
                break;
            case SCRCMD_CHANGE_BLOCK_SIDE:
            case SCRCMD_CHANGE_BLOCK_LID:
            case SCRCMD_CHANGE_BLOCK_TYPE:
                SCRCMD_CHANGE_BLOCK();
                break;
            case SCRCMD_ROAD_ON_OFF:
                Handle_ROAD_ON_OFF();
                break;
            case SCRCMD_GET_CAR_FROM_CRANE:
                Handle_GET_CAR_FROM_CRANE();
                break;
            case SCRCMD_SWITCH_GENERATOR1:
            case SCRCMD_SWITCH_GENERATOR2:
                Handle_SWITCH_GENERATOR1();
                break;
            case SCRCMD_SWITCH_GENERATOR3:
            case SCRCMD_SWITCH_GENERATOR4:
                Handle_SWITCH_GENERATOR3();
                break;
            case SCRCMD_CAR_DAMAGE_POS:
                Handle_CAR_DAMAGE_POS();
                break;
            case SCRCMD_PARK_FINISHED:
                Handle_PARK_FINISHED();
                break;
            case SCRCMD_CAR_IN_AREA:
                Handle_CAR_IN_AREA();
                break;
            case SCRCMD_REMOTE_CONTROL:
                Handle_REMOTE_CONTROL();
                break;
            case SCRCMD_LAUNCH_MISSION:
                Handle_LAUNCH_MISSION();
                break;
            case SCRCMD_RETURN:
            case SCRCMD_MISSIONEND:
                Handle_RETURN();
                break;
            case SCRCMD_PHONE_TEMPLATE:
                Handle_PHONE_TEMPLATE();
                break;
            case SCRCMD_SAVE_GAME:
                Handle_SAVE_GAME();
                break;
            case SCRCMD_CHANGE_CAR_LOCK:
                Handle_CHANGE_CAR_LOCK();
                break;
            case SCRCMD_CHANGE_COLOUR:
                Handle_CHANGE_COLOUR();
                break;
            case SCRCMD_CHANGE_INTENSITY:
                Handle_CHANGE_INTENSITY();
                break;
            case SCRCMD_CHANGE_RADIUS:
                Handle_CHANGE_RADIUS();
                break;
            case SCRCMD_CREATE_LIGHT1:
            case SCRCMD_LIGHT_DECSET2:
                SCRCMD_CREATE_LIGHT();
                break;
            case SCRCMD_CHECK_NUM_LIVES:
            case SCRCMD_CHECK_MULT:
            case SCRCMD_ADD_LIVES:
            case SCRCMD_ADD_MULTIPLIER:
                GetSetLivesAndMultiplier();
                break;
            case SCRCMD_GET_NUM_LIVES:
            case SCRCMD_GET_MULT:
                SCRCMD_GET_LIVES_MULT();
                break;
            case SCRCMD_SET_DIR_OF_TVVAN:
                Handle_SET_DIR_OF_TVVAN();
                break;
            case SCRCMD_POINT_ONSCREEN:
                Handle_POINT_ONSCREEN();
                break;
            case SCRCMD_CHAR_IN_AIR:
                Handle_CHAR_IN_AIR();
                break;
            case SCRCMD_CHAR_SUNK:
                Handle_CHAR_SUNK();
                break;
            case SCRCMD_START_BONUS1:
            case SCRCMD_START_BONUS2:
            case SCRCMD_START_BONUS3:
            case SCRCMD_START_BONUS4:
                StartBonus();
                break;
            case SCRCMD_CHECK_BONUS1:
            case SCRCMD_CHECK_BONUS2:
            case SCRCMD_CHECK_BONUS3:
                SCRCMD_CHECK_BONUS();
                break;
            case SCRCMD_SETUP_MODEL_CHECK:
                Handle_SETUP_MODEL_CHECK();
                break;
            case SCRCMD_MODEL_CHECK:
                Handle_MODEL_CHECK();
                break;
            case SCRCMD_GET_CAR_SPEED:
            case SCRCMD_GET_CHAR_CAR_SPEED:
            case SCRCMD_GET_MAX_SPEED:
                GetSpeed();
                break;
            case SCRCMD_CHECK_CAR_SPEED:
                Handle_CHECK_CAR_SPEED();
                break;
            case SCRCMD_SET_CAR_GRAPHIC:
                Handle_SET_CAR_GRAPHIC();
                break;
            case SCRCMD_S_IS_S_MINUS_S:
            case SCRCMD_S_IS_S_PLUS_S:
            case SCRCMD_S_IS_S_DIV_S:
            case SCRCMD_S_IS_S_MULT_S:
            case SCRCMD_S_IS_S_MOD_S:
                SCRCMD_S_IS_S_OP_S();
                break;
            case SCRCMD_S_IS_S_MINUS_I:
            case SCRCMD_S_IS_S_PLUS_I:
            case SCRCMD_S_IS_S_DIV_I:
            case SCRCMD_S_IS_S_MULT_I:
            case SCRCMD_S_IS_S_MOD_I:
                SCRCMD_S_IS_S_OP_I();
                break;
            case SCRCMD_GIVE_CAR_ALARM:
                Handle_GIVE_CAR_ALARM();
                break;
            case SCRCMD_CAR_BULLETPROOF:
            case SCRCMD_CAR_ROCKETPROOF:
            case SCRCMD_CAR_FLAMEPROOF:
                SCRCMD_CAR_WEAPON_KIND_PROOF();
                break;
            case SCRCMD_CHAR_DRIVE_AGGR:
                Handle_CHAR_DRIVE_AGGR();
                break;
            case SCRCMD_CHAR_DRIVE_SPEED:
            case SCRCMD_SET_RUN_SPEED:
                SCRCMD_SET_SPEED();
                break;
            case SCRCMD_SET_CHAR_OBJ_FOLLOW:
                Handle_SET_CHAR_OBJ_FOLLOW();
                break;
            case SCRCMD_PUT_CAR_ON_TRAILER:
                Handle_PUT_CAR_ON_TRAILER();
                break;
            case SCRCMD_CLEAR_BRIEFS:
                Handle_CLEAR_BRIEFS();
                break;
            case SCRCMD_CHECK_HEADS:
                Handle_CHECK_HEADS();
                break;
            case SCRCMD_FINISH_LEVEL:
                Handle_FINISH_LEVEL();
                break;
            case SCRCMD_CHECK_WEAPONHIT:
                Handle_CHECK_WEAPONHIT();
                break;
            case SCRCMD_WEAP_HIT_CAR:
            case SCRCMD_ANY_WEAPON_HIT_CAR:
                Handle_WEAP_HIT_CAR();
                break;
            case SCRCMD_IS_CHAR_ON_FIRE:
                Handle_IS_CHAR_ON_FIRE();
                break;
            case SCRCMD_BRIEF_ONSCREEN:
                Handle_BRIEF_ONSCREEN();
                break;
            case SCRCMD_CREATE_SOUND:
                Handle_CREATE_SOUND();
                break;
            case SCRCMD_DO_EASY_PHONE:
                Handle_DO_EASY_PHONE();
                break;
            case SCRCMD_CHAR_INTO_CAR:
                Handle_CHAR_INTO_CAR();
                break;
            case SCRCMD_ADD_SCORE2:
                Handle_ADD_SCORE2();
                break;
            case SCRCMD_DECIDE_POWERUP:
                Handle_DECIDE_POWERUP();
                break;
            case SCRCMD_CHAR_ARRESTED:
                Handle_CHAR_ARRESTED();
                break;
            case SCRCMD_SUPPRESS_MODEL:
                Handle_SUPPRESS_MODEL();
                break;
            case SCRCMD_WARP_CHAR:
                Handle_WARP_CHAR();
                break;
            case SCRCMD_SET_GROUP_TYPE:
                Handle_SET_GROUP_TYPE();
                break;
            case SCRCMD_CHAR_DO_NOTHING:
                Handle_CHAR_DO_NOTHING();
                break;
            case SCRCMD_EMERG_LIGHTS:
                Handle_EMERG_LIGHTS();
                break;
            case SCRCMD_CHECK_OBJ_MODEL:
                Handle_CHECK_OBJ_MODEL();
                break;
            case SCRCMD_PED_GRAPHIC:
                Handle_PED_GRAPHIC();
                break;
            case SCRCMD_CHAR_INVINCIBLE:
            case SCRCMD_SET_STAY_IN_CAR:
            case SCRCMD_SET_USE_CAR_WEAPON:
            case SCRCMD_DEATH_ARR_STATE:
                SCRCMD_SET_CHAR_FLAGS();
                break;
            case SCRCMD_LOC_SECOND_CHAR:
            case SCRCMD_LOC_SEC_CHAR_CAR:
            case SCRCMD_LOC_SEC_CHAR_ANY:
                LocSecChar();
                break;
            case SCRCMD_STOP_CAR_DRIVE:
                Handle_STOP_CAR_DRIVE();
                break;
            case SCRCMD_IS_BUS_FULL:
                Handle_IS_BUS_FULL();
                break;
            case SCRCMD_NO_CHARS_OFF_BUS:
                Handle_NO_CHARS_OFF_BUS();
                break;
            case SCRCMD_KILL_CHAR:
                Handle_KILL_CHAR();
                break;
            case SCRCMD_SET_SHADING_LEV:
                Handle_SET_SHADING_LEV();
                break;
            case SCRCMD_SET_CAR_JAMMED:
                Handle_SET_CAR_JAMMED();
                break;
            case SCRCMD_FINISH_MISSION:
                Handle_FINISH_MISSION();
                break;
            case SCRCMD_STORE_BONUS:
                Handle_STORE_BONUS();
                break;
            case SCRCMD_SET_ENTER_STATUS:
            case SCRCMD_SET_ALL_CONTROLS:
                Handle_SET_ENTER_STATUS();
                break;
            case SCRCMD_SET_FAV_CAR:
                Handle_SET_FAV_CAR();
                break;
            case SCRCMD_GROUP_IN_AREA:
                GroupInArea();
                break;
            case SCRCMD_SET_CHAR_OCCUPATION:
                Handle_SET_CHAR_OCCUPATION();
                break;
            case SCRCMD_SET_KF_WEAPON:
                Handle_SET_KF_WEAPON();
                break;
            case SCRCMD_CLEAR_KF_WEAPON:
                Handle_CLEAR_KF_WEAPON();
                break;
            case SCRCMD_ADD_ONSCREEN_COUNTER:
                Handle_ADD_ONSCREEN_COUNTER();
                break;
            case SCRCMD_CLEAR_COUNTER:
            case SCRCMD_CLEAR_CLOCK_ONLY:
                Handle_CLEAR_COUNTER();
                break;
            case SCRCMD_CHECK_CAR_DRIVER:
            case SCRCMD_IS_CAR_WRECKED:
            case SCRCMD_CAR_SUNK:
            case SCRCMD_CAR_IN_AIR:
            case SCRCMD_IS_CAR_CRUSHED:
            case SCRCMD_CARBOMB_ACTIVE:
            case SCRCMD_EMERG_LIGHTS_ON:
                Handle_CHECK_CAR_DRIVER();
                break;
            case SCRCMD_CHANGE_POLICE:
                Handle_CHANGE_POLICE();
                break;
            case SCRCMD_DESTROY_GROUP:
                Handle_DESTROY_GROUP();
                break;
            case SCRCMD_CHECK_CURRENT_WEAPON:
                Handle_CHECK_CURRENT_WEAPON();
                break;
            case SCRCMD_MAKE_MUGGERS:
                Handle_MAKE_MUGGERS();
                break;
            case SCRCMD_START_BASIC_KF:
                Handle_START_BASIC_KF();
                break;
            case SCRCMD_DO_BASIC_KF:
                Handle_DO_BASIC_KF();
                break;
            case SCRCMD_SET_BONUS_RATING:
                Handle_SET_BONUS_RATING();
                break;
            case SCRCMD_ADD_TIME:
                Handle_ADD_TIME();
                break;
            case SCRCMD_DO_SAVE_GAME:
                Handle_DO_SAVE_GAME();
                break;
            case SCRCMD_SET_MODEL_WANTED:
                Handle_SET_MODEL_WANTED();
                break;
            case SCRCMD_CHECK_DEATH_ARR:
                Handle_CHECK_DEATH_ARR();
                break;
            case SCRCMD_FORCE_CLEANUP:
                Handle_FORCE_CLEANUP();
                break;
            case SCRCMD_SAVE_RESPECT:
            case SCRCMD_RESTORE_RESPECT:
                SCRCMD_SAVE_RESTORE_RESPECT();
                break;
            default:
                Next(pCmd);
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

    SCR_CMD_HEADER* pCmd = gScriptManager->GetBasePointer(mNextCmd);

    if (!pCmd)
    {
        sprintf(gTmpBuffer, "Miss2: accessing nonexistant mission line. Current uid: %d", mNextCmd);
    }

    ScriptThread::MissionFailOnDeathOrArrest();

    if (pCmd->mType == SCRCMD_START_EXEC)
    {
        do
        {
            ScriptThread::PreExecOpCode();
            gScriptManager->GetBasePointer(mNextCmd);
        } while (mExecFlag > 0); //  execute opcodes in the same frame until an ENDEXEC
    }
    else //  It isn't an EXEC opcode
    {
        // mReturnValue == 1  means it's in a WHILE_EXEC block
        if (pCmd->mReturnValue == 1)
        {
            while (true) // execute commands in the same frame
            {
                SCR_CMD_HEADER* pCurrentCmd = gScriptManager->GetBasePointer(mNextCmd);
                ScriptThread::PreExecOpCode();

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
            ScriptThread::PreExecOpCode();
        }
    }

    const bool reached_level_end = mNextCmd == 0xFFFF;
    mExecFlag = 0;
    return reached_level_end;
}

MATCH_FUNC(0x511930)
void ScriptThread::InitThread(char_type flags, u16 start_cmd)
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
ScriptThread* ScriptThread::SpawnThread(u16 start_cmd)
{
    ScriptThread* pNewThread = miss2_0x11C_Pool->Allocate_4767A0();
    pNewThread->InitThread(mFlags, start_cmd);
    return pNewThread;
}

MATCH_FUNC(0x5119a0)
void ScriptThread::launch_mission(SCR_CMD_HEADER* pCmd, const char_type* pMissionName)
{
    u16 cmd_next;
    gScriptManager->LoadMissionScript(pMissionName, &cmd_next);
    strcpy(mStr, pMissionName);
    ScriptThread::Gosub(pCmd, cmd_next);
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
void ScriptThread::DisableBasicKfsFromSave()
{
    for (u16 i = 0; i < 25; i++)
    {
        if ((1 << i) & gGameSave.mCarAndScriptData.mStartedBasicKfMask)
        {
            SCR_START_BASIC_KF_TEMPLATE* pCmd = static_cast<SCR_START_BASIC_KF_TEMPLATE*>(gScriptManager->GetBasePointer(
                gScriptManager->mBasicKf[i]));
            ScriptThread::DisableThread(pCmd->mTriggername);
            ScriptThread::DeallocOrDeleteItem(pCmd->mObjname);
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
