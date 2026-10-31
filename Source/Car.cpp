#include "Car.hpp"
#include "car_shop_event.hpp"
#include "car_thread_trigger.hpp"
#include "car_impact_mode.hpp"
#include "explosion_type.hpp"
#include "ped_death_cause.hpp"
#include "car_shop_type.hpp"
#include "car_despawn_status.hpp"
#include "car_door_state.hpp"
#include "CarAI.hpp"
#include "CarInfoTable.hpp"
#include "CarPhysics.hpp"
#include "Char_Pool.hpp"
#include "Cranes.hpp"
#include "Crushers.hpp"
#include "DoorPool.hpp"
#include "Firefighters.hpp"
#include "Fix16_Rect.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "CarChaseTaskTable.hpp"
#include "Hud.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "PathFinder.hpp"
#include "ParticleManager.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "PublicTransport.hpp"
#include "SpriteGrid.hpp"
#include "RouteFinder.hpp"
#include "CrimeReportQueue.hpp"
#include "TaxiList.hpp"
#include "PedRefTable.hpp"
#include "WeaponManager.hpp"
#include "collide.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "ScriptManager.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "TextTable.hpp"
#include "winmain.hpp"

// mLightFlags bits of CarLightFlags8: bEmergencyLights, bLightFlashing and bHeadlightFlash
enum
{
    kEmergencyOrFlashingLightsMask = 0x1C
};

// Car model lists filled by CarManager::DistributeCarsByRating, from the tens of car_info::rating
enum
{
    k_rating_bad = 1,
    k_rating_average = 2,
    k_rating_good = 3,
};

// Traffic car category that CarManager::SelectTrafficCarModel writes to *pOutCategory
enum
{
    k_traffic_none = 0,
    k_traffic_good = 1,
    k_traffic_bad = 2,
    k_traffic_average = 3,
    k_traffic_police = 4,
    k_traffic_gang = 5,
};

// Speed band that TrafficSpawner::GetRandomTrafficSpeed writes to *pOutSpeedBand
enum
{
    k_traffic_speed_slow = 0,
    k_traffic_speed_medium = 1,
    k_traffic_speed_fast = 2,
};

// Trailer::Update results
enum
{
    k_trailer_update_ok = 0,
    k_trailer_update_despawned = 1,
    k_trailer_update_detach = 2,
};

EXTERN_GLOBAL(bool, gCheatMiniCars_67D6C8);

DEFINE_GLOBAL(CarThreadTriggers*, gCarThreadTriggers_214_705F20, 0x705F20);
DEFINE_GLOBAL(CarManager*, gCarManager_6C_677930, 0x677930);
DEFINE_GLOBAL(CarPool*, gCar_BC_Pool_67792C, 0x67792C);
DEFINE_GLOBAL(TrailerPool*, gTrailerPool_66AC80, 0x66AC80);
DEFINE_STATIC_GLOBAL_INIT(Fix16_Point, sTrailerHitchOffset, Fix16_Point(Fix16(0, 0), Fix16(-0x1E00, 0)), 0x66AAC8); // (0, -30/64)
DEFINE_STATIC_GLOBAL_INIT(Fix16_Point, sTrailerCabOffset, Fix16_Point(Fix16(0, 0), Fix16(0x2F00, 0)), 0x66AAE0); // (0, 47/64)
DEFINE_GLOBAL_INIT(Ang16, kAng90_66A9C8, Ang16(360), 0x66A9C8);
DEFINE_GLOBAL_INIT(Ang16, kAng270_66AABC, Ang16(1080), 0x66AABC);
DEFINE_GLOBAL(TrafficSpawner*, gTrafficSpawner_14_677934, 0x677934);
DEFINE_GLOBAL_INIT(Fix16, kFpOneAndHalf_6772AC, Fix16(0x6000, 0), 0x6772AC);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint4_6FF70C, Fix16(0x1999, 0), 0x6FF70C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint3, Fix16(0x1333, 0), 0x6FF85C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FF724, Fix16(0xCCC, 0), 0x6FF724);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint03, Fix16(0x1EB, 0), 0x6FF6A4);

// This is not used outside this file.
// In fact, it's only allocated and deallocated, it's never used.
DEFINE_STATIC_MEMBER(Sprite*, CarManager, sSpriteUnused, 0x677938);

DEFINE_GLOBAL_INIT(Fix16, gFix16_6777CC, Fix16(0), 0x6777CC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kTvAntennaOffsetY, Fix16(0x100, 0), 0x6778A0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMachineGunOffsetY, Fix16(0xFFFFEE00, 0), 0x6777FC);

EXTERN_GLOBAL(CarInfo_2C*, gCarInfo_2C_6FE0E4);
EXTERN_GLOBAL(ModelPhysics*, gCarInfo_48_6FE258);

DEFINE_GLOBAL(u16, gBlockedRoadBrakeTimer_677CFC, 0x677CFC);
DEFINE_STATIC_GLOBAL(SpriteList, sCrushedCarSprites, 0x67737c);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kWaterCannonOffsetY, Fix16(0x800, 0), 0x6771F0);
DEFINE_GLOBAL_INIT(Ang16, kAng180_677326, Ang16(0x2D0), 0x677326);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kTankCannonOffsetY, Fix16(0x800, 0), 0x6772A4);

DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_677208, Fix16(0x1000, 0), 0x677208);

// Indicates if CarRemapRng is initialised
DEFINE_GLOBAL(bool, gbRngRemapTableDone_679C0A, 0x679C0A);

// Array of values used by CarRemapRng.
DEFINE_GLOBAL_ARRAY(u16, gRngRemapTable_679320, 1000, 0x679320);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6777D0, Fix16(1), 0x6777D0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6772D0, Fix16(0x2000, 0), 0x6772D0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6771FC, Fix16(0x2000, 0), 0x6771FC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_677888, Fix16(0x100, 0), 0x677888);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_6778D0, Fix16(5), 0x6778D0);
DEFINE_GLOBAL_INIT(Fix16, kDefaultTrafficCarSpeed_6FF744, Fix16(0x147, 0), 0x6FF744);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6FF774, Fix16(0), 0x006FF774);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpZero, kFpZero_6FF774, 0x6FF580);
DEFINE_GLOBAL_INIT(Fix16, kFp255_6FF558, Fix16(255), 0x6FF558);
DEFINE_STATIC_GLOBAL_INIT(Fix16, sTrafficCarSpeed, Fix16(0x147, 0), 0x6FF570);
DEFINE_STATIC_GLOBAL_INIT(Fix16, sTrafficDensityScale, kFpZero_6FF774, 0x6FF7E8);
DEFINE_GLOBAL(s8, DAT_006FF8C4, 0x6FF8C4);
DEFINE_GLOBAL(s8, DAT_006FF8C5, 0x6FF8C5);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FF778, Fix16(1), 0x6ff778);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint8, Fix16(0x3333, 0), 0x6FF5E4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint6, Fix16(0x2666, 0), 0x6FF5DC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint4_6FF5D4, Fix16(0x1999, 0), 0x6FF5D4);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6F7690, Fix16(0x2000, 0), 0x6F7690);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_6F77D4, Fix16(5), 0x6F77D4);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_6F804C, Ang16(0), 0x6F804C);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6F771E, Ang16(0x2D0), 0x6F771E);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_67791C, Ang16(0), 0x67791C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_6777A0, Fix16(0x333, 0), 0x6777A0);

DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6FF77C, Fix16(2), 0x6FF77C);
DEFINE_GLOBAL_INIT(Fix16, kFpOneAndHalf_6FF680, Fix16(0x6000, 0), 0x6FF680);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_6FF6D4, Fix16(5), 0x6FF6D4);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FF674, Fix16(0x2000, 0), 0x6FF674);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FF5CC, Fix16(0xCCC, 0), 0x6FF5CC);

DEFINE_STATIC_GLOBAL_INIT(Fix16_Point, kZeroPoint, Fix16_Point(0, 0), 0x6778A8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne, Fix16(1), 0x677908);

DEFINE_GLOBAL_INIT(Fix16, kZero_705DD8, Fix16(0), 0x705DD8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_705DDC, Fix16(1), 0x705DDC);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_705F10, Ang16(0), 0x705F10);

DEFINE_GLOBAL_INIT(Fix16, kFpHalf_677218, Fix16(0x2000, 0), 0x677218);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint13_676984, Fix16(0x852, 0), 0x676984);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint26, Fix16(0x10A4, 0), 0x6778B4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint1, Fix16(0x666, 0), 0x6778E0);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6777D4, Fix16(2), 0x6777D4);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6772CC, Fix16(0xCCC, 0), 0x6772CC);
EXTERN_GLOBAL(bool, gDoorAccessCheckActive_6F8EDC);

DEFINE_GLOBAL_INIT(Fix16, kFpZero_66AB38, Fix16(0), 0x66AB38);

DEFINE_STATIC_GLOBAL_INIT(Fix16_Point, kTrailerHitchOffset, Fix16_Point(0, kFpOne64th_677888 * 47), 0x677370);
DEFINE_STATIC_GLOBAL_INIT(Fix16_Point, kCabHitchOffset, Fix16_Point(0, kFpOne64th_677888 * -30), 0x677358);
DEFINE_GLOBAL_INIT(Ang16, kAng90_677234, Ang16(0x168), 0x677234);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6778FC, Fix16(0x2000, 0), 0x6778FC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpEight, Fix16(8), 0x677918);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16OneDegree_677920, Fix16(0x11C, 0), 0x677920);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveEighths, Fix16(0x2800, 0), 0x6778C8);
DEFINE_GLOBAL_INIT(Ang16, kRoofTurretAutoRotSpeed_677910, Ang16(4), 0x677910);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6F67EA, Ang16(720), 0x6F67EA);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6F6754, Ang16(360), 0x6F6754);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6F6808, Ang16(1080), 0x6F6808);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_6F6D3C, Ang16(0), 0x6F6D3C);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6772BC, Fix16(0xCCC, 0), 0x6772BC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint4_677214, Fix16(0x1999, 0), 0x677214);

DEFINE_STATIC_GLOBAL_INIT(Ang16, kFireTruckCannonRotSpeed, Ang16(0x14), 0x6771C0);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kTankTurretRotSpeed, Ang16(0x14), 0x677352);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kGunJeepTurretRotSpeed, Ang16(0x14), 0x677810);

DEFINE_GLOBAL_INIT(Fix16, kFp255_676D98, Fix16(255), 0x676D98);
DEFINE_GLOBAL_INIT(Fix16, kDrowningMaxSpeed_677794, kFpPoint05_6777A0, 0x677794);

DEFINE_STATIC_MEMBER(s32, CarManager, sAverageCarModelsCount, 0x6772DC);
DEFINE_STATIC_MEMBER(s32, CarManager, sBadCarModelsCount, 0x6772EC);
DEFINE_STATIC_MEMBER(s32, CarManager, sGoodCarModelsCount, 0x677294);

DEFINE_GLOBAL_INIT(car_rng_list, gGoodCarModels_677384, car_rng_list(Fix16(256, 0)), 0x677384);
DEFINE_GLOBAL_INIT(car_rng_list, gAverageCarModels_676DB4, car_rng_list(Fix16(512, 0)), 0x676DB4);
DEFINE_GLOBAL_INIT(car_rng_list, gBadCarModels_676988, car_rng_list(Fix16(5)), 0x676988);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kHitchPullDist, kFpOne64th_677888 * 8, 0x6772C0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kHitchAttachDist, kFpOne64th_677888 * 3, 0x677900);

static inline void __stdcall SwapIf3or4_41FE40(s32 direction, Fix16& width, Fix16& height) 
{
    if (direction >= 3 && direction <= 4)
    {
        Fix16 tmp = width;
        width = height;
        height = tmp;
    }
}

MATCH_FUNC(0x5639c0)
void ClearCarInfoGlobals()
{
    gCarInfo_2C_6FE0E4 = NULL;
    gCarInfo_48_6FE258 = NULL;
}

MATCH_FUNC(0x447640)
void ResetBlockedRoadBrakeTimer()
{
    gBlockedRoadBrakeTimer_677CFC = 0;
}

MATCH_FUNC(0x5c8680)
void CarThreadTriggers::FreeTrigger(u8 idx)
{
    mTriggers[idx].mType = car_thread_trigger_type::free_0;
    mTriggers[idx].mAction = car_thread_trigger_action::none_0;
    mTriggers[idx].mpScriptCmd = 0;
    mTriggers[idx].mEnableState = car_thread_trigger_state::disabled_2;
    mTriggers[idx].mO2C->Dealloc();
    mTriggers[idx].mO2C = NULL;
}

MATCH_FUNC(0x5c86c0)
char_type CarThreadTriggers::AddThreadTrigger(const s32& type, const s32& param, SCR_THREAD* pScriptThread, Fix16 xpos, Fix16 ypos, Fix16 zpos, Fix16 width, Fix16 height)
{
    CarThreadTrigger* pIter = mTriggers;
    for (u8 idx = 0; idx < GTA2_COUNTOF(mTriggers); idx++)
    {
        if (pIter->mType == car_thread_trigger_type::free_0)
        {
            pIter->mType = type;
            pIter->mAction = param;
            pIter->mpScriptCmd = reinterpret_cast<s32>(pScriptThread);
            pIter->mEnableState = car_thread_trigger_state::armed_1;
            pIter->mO2C = gObject_5C_6F8F84->NewTouchPoint(objects::savepoint_161,
                                                                         xpos,
                                                                         ypos,
                                                                         zpos,
                                                                         kAngZero_705F10,
                                                                         width,
                                                                         height,
                                                                         kFpOne_705DDC);
            pIter->mO2C->SetThreadTriggerIdx_4C4F10(idx);
            mCount++;
            return idx;
        }
        pIter++;
    }
    return -1;
}

MATCH_FUNC(0x5c8750)
void CarThreadTriggers::Reset()
{
    mCount = 0;
    CarThreadTrigger* pOff = &mTriggers[0];
    for (u8 i = 0; i < GTA2_COUNTOF(mTriggers); i++)
    {
        pOff->mRemapRng = i;
        pOff->mType = car_thread_trigger_type::free_0;
        pOff->mAction = car_thread_trigger_action::none_0;
        pOff->mpScriptCmd = 0;
        pOff->mEnableState = car_thread_trigger_state::armed_1;
        pOff++;
    }
}

// Script command that a CarThreadTrigger touch point waits on (several command types share this layout)
struct CarThreadTriggerCmd : SCR_CMD_HEADER
{
    union
    {
        u16 mIdx; // 0x8
        ScriptThread* mThread; // 0x8
    };
    u16 mThreadId; // 0xC
    s16 mThreadStartCmd; // 0xE
    u16 mCharIdx10; // 0x10
    u8 mPad12[0x12]; // 0x12
    u16 mCharIdx24; // 0x24
};

MATCH_FUNC(0x5c8780)
void CarThreadTriggers::CheckThreadTrigger(u8 idx, Sprite* pSprite)
{
    CarThreadTrigger* pEntry = &mTriggers[idx];
    if (pEntry->mEnableState != car_thread_trigger_state::armed_1 || !pEntry->mO2C)
    {
        return;
    }

    CarThreadTriggerCmd* pCmd;
    Car* pCar;
    Ped* pPed;
    SCR_POINTER* pPointer;

    switch (pEntry->mType)
    {
        case car_thread_trigger_type::any_car_1:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pEntry->mEnableState = car_thread_trigger_state::fired_0;
                pCar = pSprite->AsCar_40FEB0();
            }
            break;

        case car_thread_trigger_type::script_car_2:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->mCarBcPtr;
                pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd)->mIdx));
                if (pCar->mCarId == pPointer->mCar->mCarId)
                {
                    pEntry->mEnableState = car_thread_trigger_state::fired_0;
                }
            }
            break;

        case car_thread_trigger_type::char_on_foot_4:
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                pPed = pSprite->AsCharB4_40FEA0()->get_ped_433A20();
                pCmd = reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd);
                if (pCmd->mType == SCRCMD_THREAD_DECLARE3)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(pCmd->mCharIdx10));
                }
                else if (pCmd->mType == SCRCMD_THREAD_DECLARE5)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(pCmd->mCharIdx24));
                }
                else
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(pCmd->mIdx));
                }
                if (pPed->get_id() == pPointer->mChar->get_id())
                {
                    pEntry->mEnableState = car_thread_trigger_state::fired_0;
                }
                else
                {
                    // The empty else makes the success path an else-skip jump, which VC6 retargets
                    // to the switch tail late; that list order lets case 4 and then case 7 merge
                    // into case 6's ped check, as in the original
                    break;
                }
            }
            break;

        case car_thread_trigger_type::char_any_means_3:
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                pPed = pSprite->AsCharB4_40FEA0()->get_ped_433A20();
                if (pPed)
                {
                    pCmd = reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd);
                    if (pCmd->mType == SCRCMD_CHAR_AREA_ANY_MEANS)
                    {
                        pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(pCmd->mCharIdx24));
                    }
                    else
                    {
                        pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(pCmd->mIdx));
                    }
                    if (pPed->get_id() == pPointer->mChar->get_id())
                    {
                        pEntry->mEnableState = car_thread_trigger_state::fired_0;
                    }
                }
            }
            else if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->mCarBcPtr;
                pPed = pCar->GetDriver_4118B0();
                if (pPed)
                {
                    pCmd = reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd);
                    if (pCmd->mType == SCRCMD_CHAR_AREA_ANY_MEANS)
                    {
                        pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(pCmd->mCharIdx24));
                    }
                    else
                    {
                        pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(pCmd->mIdx));
                    }
                    if (pPed->get_id() == pPointer->mChar->get_id())
                    {
                        pEntry->mEnableState = car_thread_trigger_state::fired_0;
                    }
                }
            }
            break;

        case car_thread_trigger_type::char_in_car_5:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->mCarBcPtr;
                pPed = pCar->GetDriver_4118B0();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd)->mIdx));
                    if (pPed->get_id() == pPointer->mChar->get_id())
                    {
                        pEntry->mEnableState = car_thread_trigger_state::fired_0;
                    }
                }
            }
            break;

        case car_thread_trigger_type::char_stopped_on_foot_7:
            // Same ped check as case 6 (written out in full as in 9.6f; VC6 cross-jumps it into case 6)
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                pPed = pSprite->AsCharB4_40FEA0()->get_ped_433A20();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd)->mIdx));
                    if (pPed->GetGameObjectVelocity_433C20() == kZero_705DD8 &&
                        pPed->get_id() == pPointer->mChar->get_id())
                    {
                        pEntry->mEnableState = car_thread_trigger_state::fired_0;
                    }
                }
            }
            break;

        case car_thread_trigger_type::char_stopped_any_means_6:
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                pPed = pSprite->AsCharB4_40FEA0()->get_ped_433A20();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd)->mIdx));
                    if (pPed->GetGameObjectVelocity_433C20() == kZero_705DD8 &&
                        pPed->get_id() == pPointer->mChar->get_id())
                    {
                        pEntry->mEnableState = car_thread_trigger_state::fired_0;
                    }
                }
            }
            else if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->mCarBcPtr;
                pPed = pCar->GetDriver_4118B0();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd)->mIdx));
                    if (pPed->GetPedVelocity() == kZero_705DD8 &&
                        pPed->get_id() == pPointer->mChar->get_id())
                    {
                        pEntry->mEnableState = car_thread_trigger_state::fired_0;
                    }
                }
            }
            break;

        case car_thread_trigger_type::char_stopped_in_car_8:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->mCarBcPtr;
                pPed = pCar->GetDriver_4118B0();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd)->mIdx));
                    if (pPed->GetPedVelocity() == kZero_705DD8 &&
                        pPed->get_id() == pPointer->mChar->get_id())
                    {
                        pEntry->mEnableState = car_thread_trigger_state::fired_0;
                    }
                }
            }
            break;
    }

    if (pEntry->mEnableState == car_thread_trigger_state::fired_0)
    {
        switch (pEntry->mAction)
        {
            case car_thread_trigger_action::spawn_thread_2:
                pCmd = reinterpret_cast<CarThreadTriggerCmd*>(pEntry->mpScriptCmd);
                pCmd->mThread = gScriptManager_6F8060->SpawnThread(pCmd->mThreadStartCmd, 0);
                break;
            case car_thread_trigger_action::set_generator_car_type_3:
                gScriptManager_6F8060->SetGeneratorTypeForCar(pCar->mCarInfoIdx, reinterpret_cast<Generator*>(pEntry->mpScriptCmd));
                FreeTrigger(pEntry->mRemapRng);
                break;
        }
    }
}

MATCH_FUNC(0x47bd00)
CarRemapRng::CarRemapRng()
{
    if (!gbRngRemapTableDone_679C0A)
    {
        gbRngRemapTableDone_679C0A = true;
        for (u16 i = 0; i < 1000; i++)
        {
            gRngRemapTable_679320[i] = i;
        }

        for (u16 j = 0; j < 1000; j++)
        {
            u16 idx = gRng_6F6784.get_int(1000);
            s16 old_value = gRngRemapTable_679320[j];
            gRngRemapTable_679320[j] = gRngRemapTable_679320[idx];
            gRngRemapTable_679320[idx] = old_value;
        }
    }
    mTableIdx = 0;
}

MATCH_FUNC(0x47bd90)
void CarRemapRng::IncNextRngRemapIdx()
{
    mTableIdx++;
    if (mTableIdx == 1000)
    {
        mTableIdx = 0;
    }
}

MATCH_FUNC(0x444980)
void CarManager::DistributeCarsByRating()
{
    u16 i = 0;
    sBadCarModelsCount = 0;
    sAverageCarModelsCount = 0;
    sGoodCarModelsCount = 0;

    for (; i < 256; i++)
    {
        if (gGtx_0x106C_703DD4->does_car_exist(static_cast<u8>(i)))
        {
            if (bStartNetworkGame_7081F0 || gGtx_0x106C_703DD4->IsCarModelInRecycleList(static_cast<u8>(i)))
            {
                car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(static_cast<u8>(i));

                if (pInfo->rating != 99)
                {
                    u16 r_mod_10 = pInfo->rating % 10;
                    u16 idx_type;

                    if (pInfo->rating < 10)
                    {
                        idx_type = k_rating_bad;
                    }
                    else if (pInfo->rating < 20)
                    {
                        idx_type = k_rating_average;
                    }
                    else if (pInfo->rating < 30)
                    {
                        idx_type = k_rating_good;
                    }

                    if (idx_type == k_rating_bad)
                    {
                        for (; r_mod_10 > 0; r_mod_10--)
                        {
                            gBadCarModels_676988.values[sBadCarModelsCount++] = pInfo->model;
                        }
                    }
                    else if (idx_type == k_rating_average)
                    {
                        for (; r_mod_10 > 0; r_mod_10--)
                        {
                            gAverageCarModels_676DB4.values[sAverageCarModelsCount++] = pInfo->model;
                        }
                    }
                    else if (idx_type == k_rating_good)
                    {
                        for (; r_mod_10 > 0; r_mod_10--)
                        {
                            gGoodCarModels_677384.values[sGoodCarModelsCount++] = pInfo->model;
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x444ab0)
u32 CarManager::SelectTrafficCarModel(Player* pPlayer, gmp_zone_info* pZoneInfo, Fix16 density_scale, u16* pOutCategory)
{
    s32 result;
    Fix16 density_calc = Fix16(pZoneInfo->mCarDensity) / Fix16(1000);
    if (density_calc == gFix16_6777CC)
    {
        *pOutCategory = k_traffic_none;
        return car_model_enum::none;
    }
    else
    {
        u16 density = (Fix16(100) * (density_scale * density_calc)).ToInt();
        if (density > 99)
        {
            density = 99;
        }

        pPlayer->mTrafficSpawnThreshold = 100 - density;
        if (pPlayer->mTrafficSpawnCounter < static_cast<u16>(100 - density))
        {
            *pOutCategory = k_traffic_none;
            return car_model_enum::none;
        }
        else
        {
            u16 goodcar_ratio;
            u16 badcar_ratio;
            u16 gangcar_ratio;
            u16 policecar_ratio;

            if (pZoneInfo)
            {
                goodcar_ratio = pZoneInfo->mGoodcarRatio;
                badcar_ratio = pZoneInfo->mBadcarRatio;
                policecar_ratio = pZoneInfo->mPolicecarRatio;
                gangcar_ratio = pZoneInfo->mGangcarRatio;
            }
            else
            {
                goodcar_ratio = 300;
                policecar_ratio = 100;
                badcar_ratio = 300;
                gangcar_ratio = 0;
            }

            u16 value = mModelRng.GetRngValue_4212D0();

            mModelRng.IncNextRngRemapIdx();

            if (value < goodcar_ratio)
            {
                do
                {
                    result = gGoodCarModels_677384.values[mGoodCarIdx++];
                    if (mGoodCarIdx == sGoodCarModelsCount)
                    {
                        mGoodCarIdx = 0;
                    }
                } while (result == mSuppressedModel);
                *pOutCategory = k_traffic_good;
            }
            else
            {
                if (value < goodcar_ratio + badcar_ratio)
                {
                    do
                    {
                        result = gBadCarModels_676988.values[mBadCarIdx++];
                        if (mBadCarIdx == sBadCarModelsCount)
                        {
                            mBadCarIdx = 0;
                        }
                    } while (result == mSuppressedModel);
                    *pOutCategory = k_traffic_bad;
                }
                else
                {
                    if (value < badcar_ratio + goodcar_ratio + policecar_ratio)
                    {
                        result = 12;
                        *pOutCategory = k_traffic_police;
                    }
                    else
                    {
                        if (value < badcar_ratio + goodcar_ratio + policecar_ratio + gangcar_ratio)
                        {
                            result = car_model_enum::none;
                            *pOutCategory = k_traffic_gang;
                        }
                        else
                        {
                            do
                            {
                                result = gAverageCarModels_676DB4.values[mAverageCarIdx++];
                                if (mAverageCarIdx == sAverageCarModelsCount)
                                {
                                    mAverageCarIdx = 0;
                                }
                            } while (result == mSuppressedModel);
                            *pOutCategory = k_traffic_average;
                        }
                    }
                }
            }
        }
    }
    return result;
}

// No callers. Five ignored arguments.
MATCH_FUNC(0x444CE0)
bool CarManager::NoRefs(s32, s32, s32, s32, s32)
{
    return true;
}

MATCH_FUNC(0x444cf0)
Car* CarManager::SpawnCarAtRoadDirection(s32 car_model_type, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{

    u8 tile_x = xpos.ToInt();
    u8 tile_y = ypos.ToInt();
    u8 z = zpos.ToInt();
    Car* pCar = NULL;

    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock(tile_x, tile_y, z - 1);

    if (gMap_0x370_6F6268->CheckGreenArrowDirection(4, pBlock))
    {
        pCar = gCarManager_6C_677930->SpawnCarOnRoadNetwork(Fix16(tile_x), kFpHalf_677218 + Fix16(tile_y), 4, car_model_type);
    }
    else if (gMap_0x370_6F6268->CheckGreenArrowDirection(2, pBlock))
    {
        pCar = gCarManager_6C_677930->SpawnCarOnRoadNetwork(kFpHalf_677218 + Fix16(tile_x), kFP16One_6777D0 + Fix16(tile_y), 2, car_model_type);
    }
    else if (gMap_0x370_6F6268->CheckGreenArrowDirection(3, pBlock))
    {
        pCar = gCarManager_6C_677930->SpawnCarOnRoadNetwork(Fix16(tile_x), kFpHalf_677218 + Fix16(tile_y), 3, car_model_type);
    }
    else if (gMap_0x370_6F6268->CheckGreenArrowDirection(1, pBlock))
    {
        pCar = gCarManager_6C_677930->SpawnCarOnRoadNetwork(kFpHalf_677218 + Fix16(tile_x), Fix16(tile_y), 1, car_model_type);
    }

    return pCar;
}

MATCH_FUNC(0x444E40)
bool Car::SnapCarToGreenArrow(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    const u8 x_int = xpos.ToInt();
    const u8 y_int = ypos.ToInt();
    const u8 z_int = zpos.ToInt();
    gmp_block_info* pBlockInfo = gMap_0x370_6F6268->GetBlock(x_int, y_int, z_int - 1);

    if (pBlockInfo)
    {

        if (gMap_0x370_6F6268->CheckGreenArrowDirection(4, pBlockInfo))
        {
            return Car::TrySnapCarToNearestDrivableRoadAndDriveForward(Fix16(x_int), (kFpHalf_677218 + (y_int)), 4);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection(2, pBlockInfo))
        {
            return Car::TrySnapCarToNearestDrivableRoadAndDriveForward((kFpHalf_677218 + (x_int)), (kFP16One_6777D0 + (y_int)), 2);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection(3, pBlockInfo))
        {
            return Car::TrySnapCarToNearestDrivableRoadAndDriveForward(Fix16(x_int), (kFpHalf_677218 + (y_int)), 3);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection(1, pBlockInfo))
        {
            return Car::TrySnapCarToNearestDrivableRoadAndDriveForward((kFpHalf_677218 + (x_int)), Fix16(y_int), 1);
        }
    }
    return false;
}

MATCH_FUNC(0x445EC0)
bool Car::TrySnapCarToNearestDrivableRoadAndDriveForward(Fix16 xpos, Fix16 ypos, s32 direction)
{

    s32 time_out_counter = 0;
    s32 search_z = gMap_0x370_6F6268->FindGroundZForCoord(xpos, ypos).ToInt();
    while (1)
    {
        gmp_block_info* pBlock = gMap_0x370_6F6268->FindNonAirBlockAtOrBelowZ(xpos.ToInt(), ypos.ToInt(), search_z);
        ++search_z;
        if (pBlock)
        {
            if (GET_BLOCK_TYPE(pBlock->mSlopeType) == ROAD)
            {
                if (!gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, direction))
                {
                    if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::up_1))
                    {
                        direction = road_direction::up_1;
                    }
                    else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::right_3))
                    {
                        direction = road_direction::right_3;
                    }
                    else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::down_2))
                    {
                        direction = road_direction::down_2;
                    }
                    else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::left_4))
                    {
                        direction = road_direction::left_4;
                    }
                    else
                    {
                        return false;
                    }
                }

                if (!gGame_0x40_67E008->IsPointOnScreen(xpos, ypos))
                {
                    Fix16 pos_z = gMap_0x370_6F6268->FindGroundZBelowCoord(xpos, ypos, search_z);
                    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
                    Fix16 w_val = gPixelsToFix16_6F6850.list[pInfo->w];
                    Fix16 h_val = gPixelsToFix16_6F6850.list[pInfo->h];
                    SwapIf3or4_41FE40(direction, w_val, h_val);
                    Fix16_Rect rect;
                    rect.SetRect_41E350(xpos - w_val, w_val + xpos, ypos - h_val, h_val + ypos);
                    rect.SetHiLowZ_41E370(pos_z - kFP16One_6777D0, kFP16One_6777D0 + pos_z);
                    if (!gSpriteGrid_1_679208->CheckRectForCollisions(&rect, false, sprite_types_enum::none_0, NULL) && !rect.IsRectBlockedByMap())
                    {
                        mCarSprite->set_xyz_lazy_420600(xpos, ypos, pos_z);

                        mCarSprite->set_ang_lazy_420690(ReturnAngleFromRoadDirection(direction));

                        if (mPhysics)
                        {
                            mPhysics->SnapVelocityToSpriteDirection(mCarSprite);
                            mPhysics->Accelerate_421210();
                            mPhysics->mIsHandBrakeOn = 0;
                            mPhysics->mTurnDirection = car_turn_direction::none_0;
                        }
                        return true;
                    }
                }
            }
        }
        switch (direction)
        {
            case 1:
                ypos += kFP16One_6777D0;
                break;
            case 2:
                ypos -= kFP16One_6777D0;
                break;
            case 3:
                xpos -= kFP16One_6777D0;
                break;
            case 4:
                xpos += kFP16One_6777D0;
                break;
            default:
                break;
        }

        if (xpos < gFix16_6777CC || xpos > kFp255_676D98 || ypos < gFix16_6777CC || ypos > kFp255_676D98 || search_z < 0 || search_z > 7)
        {
            time_out_counter = 200;
        }

        if (++time_out_counter >= 200)
        {
            return false;
        }
    }
}

MATCH_FUNC(0x444f80)
Car* CarManager::GetNearestCarFromCoord(Fix16 x, Fix16 y, Fix16 z, Ped* pPed)
{
    return DoGetNearestCarFromCoord(x, y, z, true, pPed, true);
}

MATCH_FUNC(0x444fa0)
Car* CarManager::GetNearestEnterableCarFromCoord(Fix16 x, Fix16 y, Fix16 z, Ped* pPed)
{
    return DoGetNearestCarFromCoord(x, y, z, false, pPed, false);
}

// 9.6f 0x424BD0
MATCH_FUNC(0x444FC0)
Car* CarManager::DoGetNearestCarFromCoord(Fix16 xpos,
                                                Fix16 ypos,
                                                Fix16 zpos,
                                                s32 bMatchDriverless,
                                                Ped* pPed,
                                                bool bIgnorePedRestrictions)
{
    // 9.6f calls HasSpriteZoom_421D80 and IsCarInAir_421DF0 here, 10.5 calls HasSpriteZoom and IsCarInAir (not inlined)
    Fix16 smallestDist = Fix16(0x300);
    Car* pNearestCar = NULL;
    for (Car* pCarIter = gCar_BC_Pool_67792C->GetFirstCar_420E50(); pCarIter; pCarIter = pCarIter->mpNext)
    {
        if (!pCarIter->IsMaxDamage_40F890() && !pCarIter->IsTrailerInfo_421640() &&
            (bIgnorePedRestrictions || !pCarIter->IsDoorLockedForPed(pPed)) && !pCarIter->HasSpriteZoom() && !pCarIter->IsDeactivated_4214D0() &&
            !pCarIter->IsCarInAir() && pCarIter->GetCarInfoIdx_411940() != car_model_enum::TRAINFB &&
            (!bMatchDriverless || !pCarIter->mDriver))
        {
            Fix16 currentDistance = Fix16::Abs(pCarIter->mCarSprite->mXy.x - xpos) +
                Fix16::Abs(pCarIter->mCarSprite->mXy.y - ypos) + Fix16::Abs(pCarIter->mCarSprite->mZpos - zpos);
            if (currentDistance < smallestDist)
            {
                smallestDist = currentDistance;
                pNearestCar = pCarIter;
            }

            if (pCarIter->IsTrainModel_403BA0())
            {
                u8 train_car_idx = 0;
                Car** pTrainCars = gPublicTransport_181C_6FF1D4->GetCarArrayFromLeadCar(pCarIter);
                for (Car* pTrainIter = *pTrainCars; pTrainIter; pTrainIter = pTrainCars[train_car_idx])
                {
                    if (train_car_idx >= 2)
                    {
                        break;
                    }

                    // The Abs negates and the last add go out of line by the inline budget. Add is the
                    // nothrow copy: the plain operator+ changes the register allocation of the whole loop
                    Fix16 trainDistance;
                    trainDistance = (Fix16::Abs(pTrainIter->mCarSprite->mXy.x - xpos) +
                                     Fix16::Abs(pTrainIter->mCarSprite->mXy.y - ypos))
                                        .Add(Fix16::Abs(pTrainIter->mCarSprite->mZpos - zpos));
                    if (trainDistance < smallestDist)
                    {
                        smallestDist = trainDistance;
                        pNearestCar = pTrainIter;
                    }
                    train_car_idx++;
                }
            }
        }
    }
    return pNearestCar;
}

MATCH_FUNC(0x445210)
Car* CarManager::GetNearestFrontVehicle(Sprite* pSprite, u8 max_rings)
{
    Fix16 oldx = pSprite->mXy.x;
    Fix16 oldy = pSprite->mXy.y;
    Fix16 oldz = pSprite->mZpos;

    gSpriteGrid_1_679208->RemoveFromRegionBuckets(pSprite);

    Fix16 new_x;
    Fix16 new_y;
    Ang16::PolarToCartesian_41FC20(pSprite->mRotation, kFpHalf_6772D0, new_x, new_y);

    pSprite->set_xyz_lazy_420600(pSprite->mXy.x + new_x, pSprite->mXy.y + new_y, pSprite->mZpos);

    Sprite* pNearest =
        gSpriteGrid_1_679208->FindNearestSprite_SpiralSearch(sprite_types_enum::car_2, sprite_types_enum::car_2, pSprite, max_rings, 3, 1u);

    pSprite->set_xyz_lazy_420600(oldx, oldy, oldz);

    gSpriteGrid_1_679208->AddToRegionBuckets(pSprite);

    if (pNearest)
    {
        Car* pCar = pNearest->mCarBcPtr;

        if (pCar->IsTrain_421610())
        {
            Car* pLeadCar = gPublicTransport_181C_6FF1D4->GetLeadTrainCar(pNearest->mCarBcPtr);
            if (pLeadCar->GetCarLinearSpeed() == gFix16_6777CC)
            {
                return pCar;
            }
        }
        else
        {
            return pCar;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4453E0)
Car* CarManager::SpawnBusAtValidRoadPosition(Fix16 x, Fix16 y, s32 road_direction, const s32& car_model)
{
    s32 found_z;
    s32 attempts = 0;
    gmp_block_info* pHighestBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
    do
    {
        gmp_block_info* pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
        if (pBlock)
        {
            if (GET_BLOCK_TYPE(pBlock->mSlopeType) == 1 && !gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction))
            {
                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::up_1))
                {
                    road_direction = road_direction::up_1;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::right_3))
                {
                    road_direction = road_direction::right_3;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::down_2))
                {
                    road_direction = road_direction::down_2;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::left_4))
                {
                    road_direction = road_direction::left_4;
                }
                else
                {
                    switch (road_direction)
                    {
                        case road_direction::up_1:
                            x += kFP16One_6777D0;
                            y -= kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::left_4))
                            {
                                road_direction = road_direction::left_4;
                            }
                            else
                            {
                                x -= kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::right_3))
                                {
                                    road_direction = road_direction::right_3;
                                }
                            }
                            break;
                        case road_direction::down_2:
                            x += kFP16One_6777D0;
                            y += kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::left_4))
                            {
                                road_direction = road_direction::left_4;
                            }
                            else
                            {
                                x -= kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::right_3))
                                {
                                    road_direction = road_direction::right_3;
                                }
                            }
                            break;
                        case road_direction::right_3:
                            x += kFP16One_6777D0;
                            y -= kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::down_2))
                            {
                                road_direction = road_direction::down_2;
                            }
                            else
                            {
                                y += kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::up_1))
                                {
                                    road_direction = road_direction::up_1;
                                }
                            }
                            break;
                        case road_direction::left_4:
                            x -= kFP16One_6777D0;
                            y -= kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);

                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::down_2))
                            {
                                road_direction = road_direction::down_2;
                            }
                            else
                            {
                                y += kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::up_1))
                                {
                                    road_direction = road_direction::up_1;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            if (GET_BLOCK_TYPE(pBlock->mSlopeType) == 1 && !gGame_0x40_67E008->IsPointOnScreen(x, y))
            {
                Fix16 ground_z = gMap_0x370_6F6268->FindGroundZForCoord(x, y);
                car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(car_model);

                Fix16 sprite_width = gPixelsToFix16_6F6850.list[pCarInfo->w];
                Fix16 sprite_height = gPixelsToFix16_6F6850.list[pCarInfo->h];

                SwapIf3or4_41FE40(road_direction, sprite_width, sprite_height);

                Fix16_Rect rect;
                rect.SetRect_41E350(x - sprite_width, sprite_width + x, y - sprite_height, sprite_height + y);
                rect.SetHiLowZ_41E370(ground_z - kFP16One_6777D0, ground_z + kFP16One_6777D0);

                if (!gSpriteGrid_1_679208->CheckRectForCollisions(&rect, false, sprite_types_enum::none_0, NULL) && !rect.IsRectBlockedByMap() &&
                    !gGame_0x40_67E008->IsRectVisibleToAnyPlayer(&rect))
                {
                    return CarManager::SpawnCar_426E10_v2(x, y, ground_z, ReturnAngleFromRoadDirection(road_direction), car_model);
                }
            }
        }
        switch (road_direction)
        {
            case road_direction::up_1:
                y += kFP16One_6777D0;
                break;
            case road_direction::down_2:
                y -= kFP16One_6777D0;
                break;
            case road_direction::right_3:
                x -= kFP16One_6777D0;
                break;
            case road_direction::left_4:
                x += kFP16One_6777D0;
                break;
            default:
                break;
        }
        if (x < gFix16_6777CC || x > kFp255_676D98 || y < gFix16_6777CC || y > kFp255_676D98 || found_z < 0 || found_z > 1)
        {
            attempts = 200;
        }
        ++attempts;
    } while (attempts < 200);

    return NULL;
}

// Finds a route junction near (xpos, ypos) and walks along the road from it until a free, off
// screen spot is found, then spawns a car there with an AI that follows the route.
MATCH_FUNC(0x4458b0)
Car* CarManager::SpawnCarOnRoadNetwork(Fix16 xpos, Fix16 ypos, s32 road_direction, s32 car_model_type)
{
    u8 unused;
    s32 found_z;
    s32 attempts = 0;
    u8 x8 = xpos.ToUInt8();
    u8 y8 = ypos.ToUInt8();
    s32 x_int = xpos.ToInt();
    s32 y_int = ypos.ToInt();
    gMap_0x370_6F6268->FindHighestBlockForCoord(x_int, y_int, &found_z);
    s16 route_idx = static_cast<char_type>(gRouteFinder_6FFDC8->FindOffScreenRoute(x_int, y_int, found_z, &unused, 1, road_direction));
    if (route_idx <= 0)
    {
        return NULL;
    }

    Junction* pJunction = gRouteFinder_6FFDC8->GetJunction(gRouteFinder_6FFDC8->mRoutes[route_idx].mJunctions[0]);
    u16 junction_idx = gRouteFinder_6FFDC8->mRoutes[route_idx].mJunctions[0];
    u16 next_idx = gRouteFinder_6FFDC8->mRoutes[route_idx].mJunctions[1];
    s32 dir;
    switch (static_cast<s16>(pJunction->GetDirectionToJunction(next_idx)))
    {
        case 1:
            dir = road_direction::up_1;
            break;
        case 2:
            dir = road_direction::down_2;
            break;
        case 3:
            dir = road_direction::left_4;
            break;
        case 4:
            dir = road_direction::right_3;
            break;
        default:
            dir = road_direction;
            break;
    }
    gRouteFinder_6FFDC8->FindArrowBlockInJunction(junction_idx, dir, &x8, &y8);
    Fix16 x = Fix16(x8) + kFpHalf_6772D0;
    Fix16 y = Fix16(y8) + kFpHalf_6772D0;
    road_direction = dir;

    do
    {
        gmp_block_info* pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
        if (pBlock)
        {
            if (!gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction) && GET_BLOCK_TYPE(pBlock->mSlopeType) == 1)
            {
                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::up_1))
                {
                    road_direction = road_direction::up_1;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::right_3))
                {
                    road_direction = road_direction::right_3;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::down_2))
                {
                    road_direction = road_direction::down_2;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::left_4))
                {
                    road_direction = road_direction::left_4;
                }
                else
                {
                    switch (road_direction)
                    {
                        case road_direction::up_1:
                            y = y - kFP16One_6777D0;
                            x = x + kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::left_4))
                            {
                                road_direction = road_direction::left_4;
                            }
                            else
                            {
                                x = x - kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::right_3))
                                {
                                    road_direction = road_direction::right_3;
                                }
                            }
                            break;
                        case road_direction::down_2:
                            y = y + kFP16One_6777D0;
                            x = x + kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::left_4))
                            {
                                road_direction = road_direction::left_4;
                            }
                            else
                            {
                                x = x - kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::right_3))
                                {
                                    road_direction = road_direction::right_3;
                                }
                            }
                            break;
                        case road_direction::right_3:
                            x = x + kFP16One_6777D0;
                            y = y - kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::down_2))
                            {
                                road_direction = road_direction::down_2;
                            }
                            else
                            {
                                y = y + kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::up_1))
                                {
                                    road_direction = road_direction::up_1;
                                }
                            }
                            break;
                        case road_direction::left_4:
                            x = x - kFP16One_6777D0;
                            y = y - kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::down_2))
                            {
                                road_direction = road_direction::down_2;
                            }
                            else
                            {
                                y = y + kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection(pBlock, green_1, road_direction::up_1))
                                {
                                    road_direction = road_direction::up_1;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            if (GET_BLOCK_TYPE(pBlock->mSlopeType) == 1 && !gGame_0x40_67E008->IsPointOnScreen(x, y))
            {
                Fix16 ground_z = gMap_0x370_6F6268->FindGroundZForCoord(x, y);
                car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(car_model_type);

                Fix16 sprite_width = gPixelsToFix16_6F6850.list[pCarInfo->w];
                Fix16 sprite_height = gPixelsToFix16_6F6850.list[pCarInfo->h];

                SwapIf3or4_41FE40(road_direction, sprite_width, sprite_height);

                Fix16_Rect rect;
                rect.SetRect_41E350(x - sprite_width, sprite_width + x, y - sprite_height, sprite_height + y);
                rect.SetHiLowZ_41E370(ground_z - kFP16One_6777D0, ground_z + kFP16One_6777D0);

                if (!gSpriteGrid_1_679208->CheckRectForCollisions(&rect, false, sprite_types_enum::none_0, NULL) && !rect.IsRectBlockedByMap() &&
                    !gGame_0x40_67E008->IsRectVisibleToAnyPlayer(&rect))
                {
                    Car* pCar =
                        SpawnCar_426E10_v2(x, y, ground_z, ReturnAngleFromRoadDirection(road_direction), car_model_type);
                    pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
                    pCar->AllocateAI_421510();
                    pCar->mAI->SetCar(pCar);
                    pCar->mAI->mRouteIdx = static_cast<char_type>(route_idx);
                    return pCar;
                }
            }
        }
        switch (road_direction)
        {
            case road_direction::up_1:
                y = y + kFP16One_6777D0;
                break;
            case road_direction::down_2:
                y = y - kFP16One_6777D0;
                break;
            case road_direction::right_3:
                x = x - kFP16One_6777D0;
                break;
            case road_direction::left_4:
                x = x + kFP16One_6777D0;
                break;
            default:
                break;
        }
        if (x < gFix16_6777CC || x > kFp255_676D98 || y < gFix16_6777CC || y > kFp255_676D98 || found_z < 0 || found_z > 7)
        {
            attempts = 200;
        }
        ++attempts;
    } while (attempts < 200);

    if (route_idx > -1)
    {
        gRouteFinder_6FFDC8->CancelRoute(route_idx);
    }
    return NULL;
}

// https://decomp.me/scratch/HFGKH
// 9.6f 0x426AC0
MATCH_FUNC(0x446230)
Car* CarManager::SpawnCarAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, Fix16 scale)
{
    if (gCheatMiniCars_67D6C8)
    {
        if (scale == kFP16One_6777D0)
        {
            scale = kFpHalf_6772D0;
        }
    }

    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(car_info_idx);

    Car* pCar;
    if (pCarInfo->IsTruckCab_41FEA0())
    {
        // https://decomp.me/scratch/evRsU
        pCar = gCar_BC_Pool_67792C->mPool.AllocateAppend();
    }
    else if (pCarInfo->IsHeavy_41FF00())
    {
        pCar = gCar_BC_Pool_67792C->mPool.AllocateAppend();
    }
    else
    {
        pCar = gCar_BC_Pool_67792C->mPool.Allocate();
    }

    pCar->mCarInfoIdx = car_info_idx;
    pCar->mDespawnStatus = car_despawn_status::active_1;

    pCar->mCarSprite = gSpritePool_703818->get_new_sprite();
    pCar->mCarSprite->set_xyz_lazy_420600(xpos, ypos, zpos);

    gMap_0x370_6F6268->UpdateZFromSlopeAtCoord(pCar->mCarSprite->mXy.x,
                                                      pCar->mCarSprite->mXy.y,
                                                      pCar->mCarSprite->mZpos);
    pCar->mCarSprite->set_ang_lazy_420690(rotation);
    pCar->mCarSprite->SetType_4206F0(sprite_types_enum::car_2);
    pCar->mCarSprite->set_id_lazy_4206C0(pCarInfo->sprite);
    pCar->mCarSprite->SetCar_420710(pCar);
    pCar->mCarSprite->AllocCollisionShape(gPixelsToFix16_6F6850.list[pCarInfo->w], gPixelsToFix16_6F6850.list[pCarInfo->h], kFpHalf_6771FC);

    pCar->mScale = scale;

    if (scale != kFP16One_6777D0)
    {
        pCar->mCarSprite->ApplyScaleToDimensions(pCar->mScale, 1);
    }

    gSpriteGrid_1_679208->AddToRegionBuckets(pCar->mCarSprite);

    pCar->InitDoors_4435A0();

    pCar->mAI = NULL;
    pCar->mpTrailer = NULL;

    if (pCar->HasBottomLeftRoofLight_421680() && !pCar->IsGt24640_4217D0())
    {
        pCar->SetBottomLeftRoofLightDelta_421810();
    }

    if (pCar->IsCopCar_421790())
    {
        pCar->AddFlags_421890(car_flag::ResistsPushWhenStopped);
    }
    else if (pCar->IsFireTruck_4118F0())
    {
        pCar->PutWaterCannonOnRoof();
        gWeapon_8_707018->allocate_5E3D50(weapon_type::fire_truck_gun, 50u, pCar);

        // shared flag set
        pCar->AddFlags_421890(car_flag::ResistsPushWhenStopped);
    }
    else if (pCar->IsTank_411900())
    {
        pCar->AddFlags_421890(car_flag::ResistsPushWhenStopped);
        pCar->PutTankCannonOnRoof();
        pCar->SetBulletproof_4218C0();
        pCar->SetFlameproof_4218D0();
        pCar->SetNoCollisionDamage_4218A0();
        gWeapon_8_707018->allocate_5E3D50(weapon_type::tank_main_gun, 20u, pCar);
    }
    else if (pCar->IsGunJeep_411910())
    {
        pCar->PutMachineGunOnRoof();
        gWeapon_8_707018->allocate_5E3D50(weapon_type::army_gun_jeep, 20u, pCar);
    }
    else if (pCar->IsTvVan_4217E0())
    {
        pCar->PutTV_Antenna();
    }
    else if (pCar->IsTrainModel_403BA0())
    {
        pCar->AddFlags_421890(car_flag::NoCollide);
        pCar->SetTrain_4218B0();
    }
    else if (pCar->IsGt24640_4217D0())
    {
        pCar->SetCarGraphic(mModelRng.GetRngValue_4212D0() % 11);
    }
    else if (pCar->IsFbiCar_411920())
    {
        pCar->mCarSprite->SetRemap(2);

        // shared flag set
        pCar->AddFlags_421890(car_flag::ResistsPushWhenStopped);
    }

    pCar->SetSirens();
    pCar->UpdateDrawLayer();
    pCar->mCarSprite->CreateSoundObj();
    return pCar;
}

MATCH_FUNC(0x408370)
EXPORT void __stdcall SpawnCabAndTrailerHelper(
        Fix16 xpos,
        Fix16 ypos,
        Fix16 zpos,
        Ang16 rot,
        Ang16 uknown_rot,
        Fix16 *pOutX,
        Fix16 *pOutY,
        Fix16 *pOutZ,
        Ang16 *pOutRot)
{
    Fix16_Point pos(xpos, ypos);
    Fix16_Point hitch_pos;
    Fix16_Point trailer_pos;
    Fix16_Point hitch = sTrailerHitchOffset;
    Fix16_Point cab_offset = sTrailerCabOffset;

    hitch.RotateByAngle_40F6B0_out_of_line(rot);
    hitch_pos = pos.Add(hitch);

    cab_offset.RotateByAngle_40F6B0_all_out_of_line(rot);
    // 9.6f 0x40F760: negate in place
    cab_offset.x = cab_offset.x.Negate_4086A0();
    cab_offset.y = cab_offset.y.Negate_4086A0();
    cab_offset.RotateByAngle_40F6B0_all_out_of_line(uknown_rot);
    trailer_pos = hitch_pos.Add(cab_offset);

    *pOutZ = zpos;
    *pOutX = trailer_pos.x;
    *pOutY = trailer_pos.y;
    // 9.6f 0x40F640 (unary -) then 0x40F790 (angle of the vector)
    *pOutRot = cab_offset.Negate_40ACB0().atan2_40F790();
}

// 9.6f 0x428EC0
MATCH_FUNC(0x446530)
Trailer* CarManager::SpawnCabAndTrailer(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_model, s32 trailer_model)
{
    Ang16 out_rot;

    Car* pCab = SpawnCarAtCorrectZ_426E40(xpos, ypos, rotation, car_model);

    // Separate output locals: VC6 puts them in the dead xpos/ypos arg slots and keeps the
    // parameters in registers across the first spawn
    Fix16 trailer_x;
    Fix16 trailer_y;
    Fix16 zpos;
    SpawnCabAndTrailerHelper(xpos,
                                    ypos,
                                    pCab->mCarSprite->mZpos,
                                    rotation,
                                    kAngZero_67791C,
                                    &trailer_x,
                                    &trailer_y,
                                    &zpos,
                                    &out_rot);

    Car* pTrailer = SpawnCarAtCorrectZ_426E40(trailer_x, trailer_y, out_rot, trailer_model);
    gCar_BC_Pool_67792C->mPool.UnlinkFromActiveList_420F30(pTrailer);

    Trailer* pNewTrailer = gTrailerPool_66AC80->mPool.Allocate();
    pNewTrailer->SetTruckCabAndTrailerCar(pCab, pTrailer);

    return pNewTrailer;
}

MATCH_FUNC(0x446730)
void CarManager::RemoveFromPoolAndCollision(Car* pCar)
{
    gSpriteGrid_3_679210->Remove(pCar->mCarSprite);
    gCar_BC_Pool_67792C->Remove(pCar);
}

MATCH_FUNC(0x4466c0)
void CarManager::DecrementAllocatedCarType(s32 kind)
{
    switch (kind)
    {
        case car_kind::recycled_1:
            --this->mRecycledCars;
            break;

        case car_kind::proto_recycled_2:
            --this->mProtoRecycledCars;
            break;

        case car_kind::call_police_car_10:
            --this->mKind10Cars;
            break;

        case car_kind::paramedic_car_4:
            --this->mParamedicCars;
            break;

        case car_kind::firefighter_5:
            --this->mFirefighterCars;
            break;

        case car_kind::police_6:
            --this->mUnitCars;
            break;

        case car_kind::roadblock_car_7:
            --this->mRoadblockCars;
            break;

        case car_kind::mission_car_8:
            --this->mMissionCars;
            break;
        case car_kind::parked_car_9:
            --this->mParkedCars;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x446760)
void CarManager::EnableFreeShoppingIfAllKFsPassed()
{
    // Enable free shopping once all KF's are passed
    s32* pSecretsPassed = gScriptManager_6F8060->mSecretsPassed;
    if (pSecretsPassed)
    {
        const u32 total_secrets = gScriptManager_6F8060->mTotalSecrets;
        if (total_secrets > 0)
        {
            if (!this->mDoFreeShopping && static_cast<u32>(*pSecretsPassed) >= total_secrets)
            {
                this->mDoFreeShopping = true;
            }
        }
    }
}

MATCH_FUNC(0x446790)
void CarManager::CarsService()
{
    EnableFreeShoppingIfAllKFsPassed();

    gTaxi_4_704130->PopAll();

    mVisibleCarsCount = 0;

    gCar_BC_Pool_67792C->mPool.UpdatePool();

    gTrafficSpawner_14_677934->GenerateTraffic();

    if (mSpawnBlock > 0)
    {
        mSpawnBlock--;
    }
}

MATCH_FUNC(0x446870)
bool CarManager::CanAllocateCarOfKindWithSpare(s32 type) const
{
    switch (type)
    {
        case car_kind::firefighter_5:
            return this->mFirefighterCars < 1;
        case car_kind::paramedic_car_4:
            return this->mParamedicCars < 1;
        case car_kind::police_6:
            return this->mUnitCars < 5;
        case car_kind::roadblock_car_7:
            return this->mRoadblockCars < 11;
        case car_kind::call_police_car_10:
            return this->mKind10Cars < 11;
        case car_kind::parked_car_9:
            return this->mParkedCars < 200;
        case car_kind::recycled_1:
            return gCarManager_6C_677930->mRecycledCars + gCarManager_6C_677930->mProtoRecycledCars < 15;
        case car_kind::mission_car_8:
            return this->mMissionCars < 23;
        default:
            return false;
    }
}

MATCH_FUNC(0x446930)
bool CarManager::CanAllocateCarOfKind(s32 type) const
{
    switch (type)
    {
        case car_kind::firefighter_5:
            return this->mFirefighterCars < 2;
        case car_kind::paramedic_car_4:
            return this->mParamedicCars < 2;
        case car_kind::police_6:
            return this->mUnitCars < 6;
        case car_kind::roadblock_car_7:
            return this->mRoadblockCars < 12;
        case car_kind::call_police_car_10:
            return this->mKind10Cars < 12;
        case car_kind::parked_car_9:
            return this->mParkedCars < 200;
        case car_kind::recycled_1:
            return gCarManager_6C_677930->mRecycledCars + gCarManager_6C_677930->mProtoRecycledCars < 16;
        case car_kind::mission_car_8:
            return this->mMissionCars < 24;
        default:
            return false;
    }
}

MATCH_FUNC(0x4469f0)
CarManager::CarManager()
{
    if (!gCar_BC_Pool_67792C)
    {
        gCar_BC_Pool_67792C = new CarPool();
        if (!gCar_BC_Pool_67792C)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8318);
        }
    }

    if (!gCarPhysicsPool_6FE3E0)
    {
        gCarPhysicsPool_6FE3E0 = new CarPhyisicsPool();
        if (!gCarPhysicsPool_6FE3E0)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8323);
        }
    }

    if (!gTrafficSpawner_14_677934)
    {
        gTrafficSpawner_14_677934 = new TrafficSpawner();
        if (!gTrafficSpawner_14_677934)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8329);
        }
    }

    if (!gCarAI_78_Pool_677CF8)
    {
        gCarAI_78_Pool_677CF8 = new CarAIPool();
        if (!gCarAI_78_Pool_677CF8)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8335);
        }
    }

    if (!gTrailerPool_66AC80)
    {
        gTrailerPool_66AC80 = new TrailerPool();
        if (!gTrailerPool_66AC80)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8341);
        }
    }

    mZpos = gFix16_6777CC;
    mImpactMode = car_impact_mode::none_0;
    mSpareDriver = NULL;
    mNextCarId = 1;
    mGoodCarIdx = 0;
    mUnused8 = 0;
    mBadCarIdx = 0;
    mAverageCarIdx = 0;
    mFireEffectCycle = 0;
    mExplosionOffsetCycle = 0;
    mRecycledCars = 0;
    mProtoRecycledCars = 0;
    mFirefighterCars = 0;
    mParamedicCars = 0;
    mUnitCars = 0;
    mRoadblockCars = 0;
    mMissionCars = 0;
    mParkedCars = 0;

    gTrailerHitchCandidates_67727C.ResetHead_4207E0();
    sCrushedCarSprites.ResetHead_4207E0();

    ClearCarInfoGlobals();
    ResetBlockedRoadBrakeTimer();

    mLightStateA = 4;
    mLightStateB = 1;

    if (!sSpriteUnused)
    {
        sSpriteUnused = new Sprite(kAngZero_67791C, gFix16_6777CC);
        if (!sSpriteUnused)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8375);
        }
    }

    // On version 9.6f this is call to a class method
    // that initialises the next two fields
    mTvVanDir.x = Fix16(0);
    mTvVanDir.y = Fix16(0);
    mSpawnBlock = 0;
    mVisibleCarsCount = 0;
    mModelToCheckDestroy = car_model_enum::none;
    mModelCheckDestroyed = false;
    mSuppressedModel = car_model_enum::none;
    mDoFreeShopping = bDo_free_shopping_67D6CD;
}

MATCH_FUNC(0x446dc0)
CarManager::~CarManager()
{
    if (gCar_BC_Pool_67792C)
    {
        GTA2_DELETE_AND_NULL(gCar_BC_Pool_67792C);
    }

    if (gCarPhysicsPool_6FE3E0)
    {
        GTA2_DELETE_AND_NULL(gCarPhysicsPool_6FE3E0);
    }

    if (gTrafficSpawner_14_677934)
    {
        GTA2_DELETE_AND_NULL(gTrafficSpawner_14_677934);
    }

    if (gCarAI_78_Pool_677CF8)
    {
        GTA2_DELETE_AND_NULL(gCarAI_78_Pool_677CF8);
    }

    if (gTrailerPool_66AC80)
    {
        GTA2_DELETE_AND_NULL(gTrailerPool_66AC80);
    }

    // An explicit dtor call through the global plus operator delete on a saved copy gives the original's
    // test in ecx with the copy moved to esi inside the if (as in SpriteTree::~SpriteTree)
    if (sSpriteUnused != NULL)
    {
        Sprite* p = sSpriteUnused;
        sSpriteUnused->~Sprite();
        operator delete(p);
        sSpriteUnused = NULL;
    }

    mSpareDriver = NULL;
}

MATCH_FUNC(0x4451E0)
bool Car::CanBeEnteredByPed(Ped* pPed) const
{
    Ped* pDriver = this->mDriver;
    if (pDriver && !this->IsMediCar_421780() &&
        pDriver->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
    {
        return false;
    }
    return IsDoorLockedForPed(pPed) ? false : true;
}

MATCH_FUNC(0x445360)
bool Car::IsEnterable() const
{
    if (!this->IsMaxDamage_40F890() && !IsTrailerInfo_421640() &&
        !IsDoorLockedForPed(gSpriteGrid_ped_678F64) && !HasSpriteZoom() && !IsDeactivated_4214D0() && !IsCarInAir())
    {
        // Not TRAIN (0x3B) itself, only the carriages
        if (mCarInfoIdx != car_model_enum::boxcar && mCarInfoIdx != car_model_enum::TRAINFB &&
            mCarInfoIdx != car_model_enum::TRAINCAB)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x43D5D0)
EXPORT s16 Car::ApplyImpactDamage(Fix16 damage)
{
    if ((mCarInfoIdx == car_model_enum::TAXI || mCarInfoIdx == car_model_enum::STYPECAB) && damage > kFpTwo_6777D4)
    {
        // Big taxi crash, time for the passengers to do a runner
        mPassengersList.ForceTaxiPassengersToExit();
    }

    if (!IsFlagSet_411930(car_flag::NoCollisionDamage))
    {

        return AccumulateDamage((damage * 200).ToInt(), &kZeroPoint);
    }

    return 0;
}

MATCH_FUNC(0x4403a0)
Ang16 Car::GetCornerAngle() const
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
    Fix16 w_fp = Fix16(pCarInfo->w) / 2;
    Fix16 h_fp = Fix16(pCarInfo->h) / 2;
    return Ang16(Fix16::atan2_fixed(h_fp, w_fp).rValue);
}

MATCH_FUNC(0x439ec0)
bool Car::IsPoliceCar() const
{
    bool b = IsCopCar_421790() || IsSwatVan_4217A0() || IsFbiCar_411920();
    return b;
}

// https://decomp.me/scratch/I0oG6
MATCH_FUNC(0x439ee0)
Fix16 Car::GetDamageFactorOnSpeed() const
{
    if (mDamage >= 16000)
    {
        return (Fix16((32001 - mDamage))) / Fix16(16000);
    }
    else
    {
        return Fix16(kFP16One_6777D0);
    }
}

MATCH_FUNC(0x439f30)
Fix16 Car::GetMaxSpeed() const
{
    ModelPhysics* pModelPhysics = gCarInfo_808_678098->GetModelPhysicsFromIdx(mCarInfoIdx);
    // Max speed is limited by how smashed up the car is
    return GetDamageFactorOnSpeed() * pModelPhysics->mMaxSpeed;
}

MATCH_FUNC(0x439f80)
wchar_t* Car::GetCarStr()
{
    sprintf(gTmpGxtKey_67CE50, "car%d%c%c", mCarInfoIdx, 0, 0);
    return gText_0x14_704DFC->Find(gTmpGxtKey_67CE50);
}

// 9.6f 0x421C40
MATCH_FUNC(0x439fb0)
Fix16_Point Car::GetHitchPoint() const
{
    Fix16_Point point;
    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
    if (pInfo->IsTrailer())
    {
        point = kTrailerHitchOffset;
    }
    else
    {
        point = kCabHitchOffset;
    }

    point.RotateByAngle_40F6B0(mCarSprite->mRotation);
    return point + mCarSprite->get_x_y_443580();
}

MATCH_FUNC(0x43a0e0)
Fix16_Point Car::GetRearWheelOffset() const
{
    CarInfo_2C* pInfo = gCarInfo_808_678098->GetInfoAtIdx(mCarInfoIdx);
    return Fix16_Point(Fix16(0), pInfo->mRearWheelOffset);
}

MATCH_FUNC(0x43a120)
Fix16 Car::GetMass() const
{
    ModelPhysics* pCarInfo = gCarInfo_808_678098->GetModelPhysicsFromIdx(mCarInfoIdx);

    if (HasPlayerDriver_4215C0())
    {
        return ApplyScale_421910(pCarInfo->mMass) * (kFP16One_6777D0 + kFpPoint05_6777A0);
    }
    else
    {
        return ApplyScale_421910(pCarInfo->mMass);
    }
}

MATCH_FUNC(0x43a1d0)
Fix16 Car::GetAntiStrength() const
{
    return gCarInfo_808_678098->GetModelPhysicsFromIdx(mCarInfoIdx)->mAntiStrngth;
}

MATCH_FUNC(0x43a1f0)
bool Car::IsBus()
{
    if (gPublicTransport_181C_6FF1D4->is_bus(this))
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x43a230)
bool Car::HasSpriteZoom() const
{
    return mCarSprite->mZoom != 0;
}

// CarPhysics::GetLinearSpeed_4211A0 with the square root forced inline (see
// Fix16_Point::GetLength_SqrtForced_43A240)
static inline Fix16 GetLinearSpeed_43A240(CarPhysics* pPhysics)
{
    return pPhysics->mLinvel1.GetLength_SqrtForced_43A240();
}

MATCH_FUNC(0x43a240)
Fix16 Car::GetCarLinearSpeed()
{
    CarPhysics* pPhysics;
    if (IsTrainModel_403BA0())
    {
        pPhysics = gPublicTransport_181C_6FF1D4->GetLeadTrainCar(this)->mPhysics;
        if (!pPhysics)
        {
            return gFix16_6777CC;
        }
        return GetLinearSpeed_43A240(pPhysics);
    }
    else
    {
        pPhysics = this->mPhysics;
        if (!pPhysics)
        {
            return gFix16_6777CC;
        }
        return GetLinearSpeed_43A240(pPhysics);
    }
}

MATCH_FUNC(0x43a3c0)
bool Car::IsCarInAir() const
{
    if (!mPhysics)
    {
        return false;
    }
    return mPhysics->mSurfaceType == car_surface_type::air_surface_6;
}

// 9.6f 0x421E00
MATCH_FUNC(0x43a3e0)
Ang16 Car::GetOrientationAngle() const
{
    if (!mPhysics)
    {
        return mCarSprite->mRotation;
    }
    else
    {
        return mPhysics->GetLinvelAngle_4211E0();
    }
}

MATCH_FUNC(0x43a450)
Fix16_Point Car::GetLinearVelocity()
{
    if (IsTrainModel_403BA0())
    {
        Car* pLeadCar = gPublicTransport_181C_6FF1D4->GetLeadTrainCar(this);
        if (!pLeadCar->mPhysics)
        {
            return kZeroPoint;
        }
        else
        {
            return pLeadCar->mPhysics->get_linvel();
        }
    }
    else
    {
        if (!this->mPhysics)
        {
            return kZeroPoint;
        }
        else
        {
            return this->mPhysics->get_linvel();
        }
    }
}

MATCH_FUNC(0x43a4c0)
Fix16 Car::GetVelocity() const
{
    if (!mPhysics)
    {
        return 0;
    }
    else
    {
        return mPhysics->mVelReadOnly.GetLength_41E260();
    }
}

MATCH_FUNC(0x43a590)
Fix16 Car::GetMomentOfInertia()
{
    return gCarInfo_808_678098->GetInfoAtIdx(GetCarModelForPhysics())->mMomentOfInertia;
}

MATCH_FUNC(0x43a5b0)
Fix16 Car::GetMinDimension() const
{
    const car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
    if (pInfo->w < pInfo->h)
    {
        Fix16 width = gPixelsToFix16_6F6850.list[pInfo->w];
        return width;
    }
    else
    {
        Fix16 height = gPixelsToFix16_6F6850.list[pInfo->h];
        return height;
    }
}

MATCH_FUNC(0x43a600)
void Car::RemoveAllDamage()
{
    RepairDamage();
    if (mpTrailer)
    {
        mpTrailer->mTrailerCar->RepairDamage();
    }
}

MATCH_FUNC(0x43a680)
bool Car::AllowResprayOrPlates()
{
    return (!IsTrainModel_403BA0() && !HasEmergencyInfoFlag() &&
            !IsFbiCar_411920() && !mAttachments.GetGangIdxFromSpriteIfAny() && !IsArmyModel(mCarInfoIdx));
}

MATCH_FUNC(0x43a6f0)
bool Car::IsNotCurrentRemap(u8 remap) const
{
    return gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx)->has_remaps() && mCarSprite->get_remap_41C1F0() != remap;
}

MATCH_FUNC(0x43a730)
bool Car::IsNotCurrentRemapOfCarAndTrailerCar(u8 remap) const
{
    if (mpTrailer)
    {
        // Check trailer car
        return IsNotCurrentRemap(remap) || mpTrailer->mTrailerCar->IsNotCurrentRemap(remap);
    }
    return IsNotCurrentRemap(remap);
}

MATCH_FUNC(0x43a780)
void Car::SetCarRemap(u8 remap)
{
    if (IsNotCurrentRemap(remap))
    {
        mCarSprite->SetRemap(remap);
    }

    if (mpTrailer)
    {
        if (mpTrailer->mTrailerCar->IsNotCurrentRemap(remap))
        {
            mpTrailer->mTrailerCar->mCarSprite->SetRemap(remap);
        }
    }
}

MATCH_FUNC(0x43a7d0)
void Car::AssignRandomRemap()
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
    if (pCarInfo->num_remaps)
    {
        const u32 remap_idx = gCarManager_6C_677930->mRemapRng.GetRngValue_4212D0() % (pCarInfo->num_remaps + 1);
        if (remap_idx == pCarInfo->num_remaps)
        {
            mCarSprite->SetPaletteSprites_420700();
        }
        else
        {
            mCarSprite->SetRemap(pCarInfo->remap[remap_idx]);
        }

        gCarManager_6C_677930->mRemapRng.IncNextRngRemapIdx();
    }
}

// https://decomp.me/scratch/zoRIL
MATCH_FUNC(0x43a850)
char_type Car::GetCarModelForPhysics()
{
    if (HasNonPlayerDriver_4214F0())
    {
        if (!IsTrainModel_403BA0())
        {
            if (mpChaseTask)
            {
                if (mpChaseTask->mFollowingRoute)
                {
                    if (IsTank_411900())
                    {
                        return car_model_enum::HOTDOG_D4;
                    }
                    return car_model_enum::HOTDOG_D1;
                }
            }
            else
            {
                if (IsTank_411900())
                {
                    return car_model_enum::HOTDOG_D4;
                }
                return car_model_enum::HOTDOG_D1;
            }

            switch (mCarInfoIdx)
            {
                case car_model_enum::SWATVAN:
                case car_model_enum::GUNJEEP:
                case car_model_enum::COPCAR:
                case car_model_enum::EDSELFBI:
                case car_model_enum::JEEP:
                    return car_model_enum::HOTDOG_D2;
                case car_model_enum::TANK:
                    return car_model_enum::HOTDOG_D4;
            }
        }
        return this->mCarInfoIdx;
    }

    if (this->mScale < kFP16One_6777D0)
    {
        return car_model_enum::HOTDOG_D3;
    }

    if (mCarInfoIdx == car_model_enum::TRUKCAB2 && !this->mpTrailer)
    {
        return car_model_enum::TRUKCAB1;
    }

    if (mCarInfoIdx == car_model_enum::TRUKCAB1 && this->mpTrailer)
    {
        return car_model_enum::TRUKCAB2;
    }

    return this->mCarInfoIdx;
}

MATCH_FUNC(0x43a950)
void Car::DoBrake()
{
    CarPhysics* pCarPhysics = mPhysics;
    pCarPhysics->mIsFootBrakeOn = 1;
    pCarPhysics->mIsForwardGasOn = 0;
    pCarPhysics->mIsBackwardGasOn = 0;
    pCarPhysics->mIsCoasting = false;
}

MATCH_FUNC(0x43a970)
void Car::DoBrakeAndHandbrake()
{
    mPhysics->mIsHandBrakeOn = 1;
    CarPhysics* pCarPhysics = mPhysics;
    pCarPhysics->mIsFootBrakeOn = 1;
    pCarPhysics->mIsForwardGasOn = 0;
    pCarPhysics->mIsBackwardGasOn = 0;
    pCarPhysics->mIsCoasting = false;
}

MATCH_FUNC(0x43a9a0)
void Car::SetDriver(Ped* pNewDriver)
{
    char_type hand_brake_on;
    if (!pNewDriver)
    {
        CarPhysics* pCarPhysics = this->mPhysics;
        if (pCarPhysics)
        {
            Ped* pOldDriver = this->mDriver;
            if (pOldDriver && pOldDriver->mPlayer)
            {
                hand_brake_on = pCarPhysics->get_is_hand_brake_on_421270();
            }
            else
            {
                hand_brake_on = 1;
            }
            pCarPhysics->HandleUserInputs(0, 0, 0, 0, hand_brake_on);
        }
    }
    this->mDriver = pNewDriver;
}

MATCH_FUNC(0x43a9f0)
void Car::TriggerCarAlarm()
{
    if (!mDriver && IsFlagSet_411930(car_flag::CarAlarm) && mPedType != ped_type::player_2 && !mBfA4.bLightFlashing && !IsMaxDamage_40F890())
    {
        StartLightFlashing();
    }
}

MATCH_FUNC(0x43CAC0)
void Car::StartLightFlashing()
{
    if ((mLightFlags & kEmergencyOrFlashingLightsMask) == 0)
    {
        mFlashPhaseCounter = 12;
        mBfA4.bLightFlashing = true;

        SetHeadlightDeltas_425590();
        SetBrakeLightDeltas_4213D0();
        mFlashCount = 50;
    }
}

// Not in IDA's function list: tail called from Ped::BecomeDummyOnPlayerDisconnect
MATCH_FUNC(0x43AA20)
void Car::SetDummyControl()
{
    const s32 info_idx = mCarInfoIdx;
    if (info_idx != car_model_enum::TRAIN && info_idx != car_model_enum::TRAINCAB && info_idx != car_model_enum::TRAINFB &&
        info_idx != car_model_enum::boxcar)
    {
        SetDriverPedType_421560(ped_type::dummy_3);
        CarPhysics* pCarPhysics = mPhysics;
        if (pCarPhysics)
        {
            pCarPhysics->SetField8C_to_1();
        }
    }
}

MATCH_FUNC(0x43aa60)
void Car::Deactivate()
{
    gSpriteGrid_1_679208->RemoveFromRegionBuckets(mCarSprite);
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(mCarSprite);
    gCar_BC_Pool_67792C->mPool.UnlinkFromActiveList_420F30(this);
    SetDeactivatedStatus_4214E0();
}

MATCH_FUNC(0x43AAE0)
bool Car::IsThreatToSearchingPed() const
{
    Ped* pDriver = this->mDriver;
    if (!pDriver)
    {
        return false;
    }
    return pDriver->IsThreatToSearchingPed();
}

MATCH_FUNC(0x43aaf0)
bool Car::CanCarCollideWithSprite(Sprite* pSprite)
{
    if (pSprite)
    {
        if (pSprite->mSpriteTypeEnum == sprite_types_enum::car_2)
        {
            Car* pCar = pSprite->mCarBcPtr;
            if (pCar)
            {
                if (gPublicTransport_181C_6FF1D4->AreTrainsOnDifferentTracks(pCar, this))
                {
                    return false;
                }

                const u16 our_flags = this->mFlags;
                const u16 their_flags = pCar->mFlags;
                if ((static_cast<u8>(their_flags) & car_flag::MiniCar) != (static_cast<u8>(our_flags) & car_flag::MiniCar))
                {
                    return false;
                }
                if ((their_flags & car_flag::Train) != 0 && this->IsMaxDamage_40F890() && (our_flags & car_flag::Train) == 0 ||
                    (our_flags & car_flag::Train) != 0 && pCar->IsMaxDamage_40F890() && (their_flags & car_flag::Train) == 0)
                {
                    return false;
                }
                if ((their_flags & car_flag::Crushing) != 0 && !gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->mCarInfoIdx)->IsHeavy_41FF00() ||
                    (this->mFlags & car_flag::Crushing) &&
                        !gGtx_0x106C_703DD4->get_car_info_5AA3B0(pCar->mCarInfoIdx)->IsHeavy_41FF00())
                {
                    return false;
                }

                if (this->mpTrailer)
                {
                    return true;
                }

                if (IsTruckCab_421620() && !pCar->mpTrailer && pCar->IsTrailerInfo_421640())
                {
                    return mCarSprite->CheckDirectionalSliceCollision(kFpFiveEighths, pSprite);
                }

                if (this->mpTrailer || !IsTrailerInfo_421640() || pCar->mpTrailer || !pCar->IsTruckCab_421620())
                {
                    return true;
                }

                const bool bCanCollide = pSprite->CheckDirectionalSliceCollision(kFpFiveEighths, this->mCarSprite);
                if (!HasSpriteZoom() && !this->IsMaxDamage_40F890())
                {
                    if (!IsMarkedForDespawn_4214B0() && !IsDespawning_4215B0())
                    {
                        gTrailerHitchCandidates_67727C.PushSprite(this->mCarSprite);
                    }
                }
                return bCanCollide;
            }
        }

        const s32 sprite_type = pSprite->mSpriteTypeEnum;
        Object* pObject;
        if ((sprite_type == sprite_types_enum::code_obj1_4 || sprite_type == sprite_types_enum::map_obj_5 || sprite_type == sprite_types_enum::object_1) && (pObject = pSprite->mObject2CPtr) != NULL)
        {
            ObjectDefinition* pDefinition = pObject->mDefinition;
            const s32 behavior_type = pDefinition->mBehaviorType;
            s32 model;
            if (behavior_type == object_behavior_type::pickup_6 || behavior_type == object_behavior_type::pickup_physics_7 || behavior_type == object_behavior_type::pickup_animated_8 ||
            behavior_type == object_behavior_type::pickup_physics_animated_9 || behavior_type == object_behavior_type::shop_10 || behavior_type == object_behavior_type::static_collision_check_1 || behavior_type == object_behavior_type::effect_only_12 ||
                (model = pObject->mModel, model == 182) || model == objects::grenade_obj_183)
            {
                if (pDefinition->mCollisionBucketCategory == collision_bucket_category::sprite_grid_2_region_3)
                {
                    return false;
                }
            }
            else
            {
                if (gPedRefTable_7F8_703398->mEntries[pObject->mPedRefIdx].mPedId)
                {
                    Ped* pPed = gPedManager_6787BC->PedById(gPedRefTable_7F8_703398->mEntries[pObject->mPedRefIdx].mPedId);
                    if (pPed)
                    {
                        if (pPed->mCar == this)
                        {
                            return false;
                        }
                    }
                }
            }
        }
        else if (sprite_type == sprite_types_enum::ped_3)
        {
            Char* pChar = pSprite->mCharB4Ptr;
            if (pChar)
            {
                Ped* pCharPed = pChar->mpPed;
                if (pCharPed->mPedState2 == ped_state_2::ped2_entering_a_car_6 && pCharPed->mTargetToEnter == this)
                {
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        if (gCollisionCounters_6791FC->mbDoorAccessCheck == 1)
        {
            Trailer* pTrailer = this->mpTrailer;
            if (pTrailer)
            {
                if (pTrailer->mTrailerCar == this)
                {
                    return false;
                }
            }
        }
        return true;
    }
}

MATCH_FUNC(0x43adc0)
void Car::ProcessCarToCarImpact(Sprite* pSprite)
{
    s32 score_base_multi = 0;
    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        switch (gCarManager_6C_677930->mImpactMode)
        {
            case car_impact_mode::crush_damage_1:
            {
                if (!this->IsMaxDamage_40F890())
                {
                    if (!Car::IsAreaDamaged(3))
                    {
                        Car::DamageArea(3);
                        score_base_multi = 1;
                    }
                    if (!Car::IsAreaDamaged(2))
                    {
                        Car::DamageArea(2);
                        ++score_base_multi;
                    }
                    if (!Car::IsAreaDamaged(0))
                    {
                        Car::DamageArea(0);
                        ++score_base_multi;
                    }
                    if (!Car::IsAreaDamaged(1))
                    {
                        Car::DamageArea(1);
                        ++score_base_multi;
                    }
                }

                s32 score = 1000 * score_base_multi;

                if (pCar->IsTank_411900())
                {
                    score += Car::ApplyImpactDamage(kFpEight);
                }

                if (score > 200)
                {
                    Ped* pDriver = pCar->GetDriver_4118B0();
                    if (pDriver && pDriver->is_player_41B0A0())
                    {
                        pDriver->mPlayer->mScores.AwardCarDamageScore(this, score);
                    }
                }

                mAttachments.CleanupSpriteList();
                if (mPhysics)
                {
                    mPhysics->set_linvel_421170(kZeroPoint);
                }
                sCrushedCarSprites.AddSprite(mCarSprite);
                break;
            }

            case car_impact_mode::crush_max_z_2:
                if (mCarSprite->mZpos > gCarManager_6C_677930->mZpos)
                {
                    gCarManager_6C_677930->mZpos = mCarSprite->mZpos;
                }
                break;
        }
    }
    mAttachments.DispatchCarImpactEvents(pSprite);
}

MATCH_FUNC(0x43af10)
bool Car::CanExitCar() const
{
    if (HasSpriteZoom())
    {
        return false;
    }

    if (mPhysics)
    {
        return mPhysics->IsNearlyStopped();
    }

    return true;
}

MATCH_FUNC(0x43af40)
void Car::ResumeAIDriving()
{
    if (mAI)
    {
        mAI->mTargetSpeed = kFpPoint1;
        ClearAIHalted_421550();
    }
}

MATCH_FUNC(0x43af60)
void Car::HaltAIDriving()
{
    if (mAI)
    {
        SetAIHalted_421540();
    }
}

MATCH_FUNC(0x43afe0)
bool Car::IsDoorAccessible(u8 target_door)
{
    Fix16 y;
    Fix16 x;
    Fix16_Rect fr;

    GetDoorWorldPosition(target_door, &x, &y);
    if (gPathFinder_6FDEF0->TestMoveToAdjacentTile(mCarSprite->mXy.x.ToInt(),
                                                   mCarSprite->mXy.y.ToInt(),
                                                   mCarSprite->mZpos.ToInt(),
                                                   x.ToInt(),
                                                   y.ToInt()))
    {
        gDoorAccessCheckActive_6F8EDC = true;
        fr.ComputeCollisionPrism_4204D0(x, y, kFpPoint2_6772CC, mCarSprite->mZpos);
        fr.ExpandToIncludePoint(mCarSprite->mXy.x, mCarSprite->mXy.y);
        gCollisionCounters_6791FC->mbDoorAccessCheck = 1;
        if (!fr.IsRectBlockedByMap() &&
            !gSpriteGrid_1_679208->CheckRectForCollisions(&fr, true, sprite_types_enum::ped_3, mCarSprite) && !gMap_0x370_6F6268->RectHitsDiagonalWall(&fr))
        {
            gCollisionCounters_6791FC->mbDoorAccessCheck = 0;
            gDoorAccessCheckActive_6F8EDC = false;
            return true;
        }
        gDoorAccessCheckActive_6F8EDC = false;
        gCollisionCounters_6791FC->mbDoorAccessCheck = 0;
    }
    return false;
}

// 9.6f 0x425B60
MATCH_FUNC(0x43b140)
bool Car::IsStoppedWithPavementAtDoor(u8 target_car_door)
{
    Ang16 angToUse;

    u8 door_count = GetDoorCount();
    if (GetCarLinearSpeed() > gFix16_6777CC)
    {
        return false;
    }

    Fix16 door_x;
    Fix16 door_y;
    GetDoorWorldPosition(target_car_door, &door_x, &door_y);

    if (door_count != 3)
    {
        switch (target_car_door)
        {
            case 0:
            case 2:
                angToUse = mCarSprite->mRotation + kAng90_677234;
                break;
            case 1:
            case 3:
                angToUse = mCarSprite->mRotation - kAng90_677234;
                break;
            default:
                break;
        }
    }
    else
    {
        angToUse = mCarSprite->mRotation - kAng90_677234;
    }

    Fix16 side_x;
    Fix16 side_y;
    Ang16::PolarToCartesian_41FC20(angToUse, kFpHalf_6778FC, side_x, side_y);

    side_x += door_x;
    side_y += door_y;
    if (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(side_x.ToInt(), side_y.ToInt(), mCarSprite->mZpos.ToInt() - 1) != 2)
    {
        return false;
    }
    return true;
}

MATCH_FUNC(0x43b2b0)
bool Car::IsDoorLockedForPed(Ped* pPed) const
{
    // Returning the comparison chains (not 0/1 constants) gives the original's 'xor %eax,%eax'/'mov $1,%eax'
    if (pPed)
    {
        if (mDoorLock == Car_Door_Lock::lockout_player_5)
        {
            return pPed->PedTypeIs(ped_type::player_2);
        }
        if (!pPed->PedTypeIs(ped_type::player_2) &&
            !(pPed->PedTypeIs(ped_type::script_created_5) && pPed->GetInternalObjective_403A90() == objectives_enum::enter_car_as_driver_35 && pPed->get_target_to_enter_403B10() == this))
        {
            return mDoorLock == Car_Door_Lock::locked_1 || mDoorLock == Car_Door_Lock::locked_permanently_4 || mDoorLock == Car_Door_Lock::lockout_thief_2;
        }
    }
    return mDoorLock == Car_Door_Lock::locked_1 || mDoorLock == Car_Door_Lock::locked_permanently_4;
}

MATCH_FUNC(0x43b340)
Car_Door* Car::GetDoor(u8 door_idx)
{
    return &mDoors[door_idx];
}

MATCH_FUNC(0x43b360)
char_type Car::GetDoorCount() const
{
    return *gGtx_0x106C_703DD4->get_car_door_data(mCarInfoIdx);
}

MATCH_FUNC(0x43b380)
void Car::OpenAllDoors()
{
    u8* pDoorCount = gGtx_0x106C_703DD4->get_car_door_data(mCarInfoIdx);
    for (u8 i = 0; i < *pDoorCount; i++)
    {
        Car_Door* pDoor = GetDoor(i);
        pDoor->Open();
    }
}

MATCH_FUNC(0x43b3d0)
void Car::CloseAllDoors()
{
    u8* pDoorCount = gGtx_0x106C_703DD4->get_car_door_data(mCarInfoIdx);
    for (u8 i = 0; i < *pDoorCount; i++)
    {
        Car_Door* pDoor = GetDoor(i);
        pDoor->Close();
    }
}

MATCH_FUNC(0x43b420)
void Car::GetDoorWorldPos(u8 door_idx, Fix16* pOutX, Fix16* pOutY)
{
    door_info* pDoor = reinterpret_cast<door_info*>(gGtx_0x106C_703DD4->get_car_door_data(mCarInfoIdx) + 1) + door_idx;
    Fix16 door_x = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pDoor->rx);
    Fix16 door_y = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pDoor->ry);

    // Ang16::RotateVector_41FC90 written out: old_xpos copy and an Ang16& to the sprite angle as in the
    // inline, the new x kept in x_off, and the y line through the out-of-line Multiply and
    // operator+ (0x408660) like Trailer::GetHitchPosition. An Ang16 copy of the angle swaps which of
    // sin/cos stays in a register.
    Fix16 old_xpos = door_x;
    Ang16& rotation = mCarSprite->mRotation;
    Fix16 x_off;
    x_off = door_x * Ang16::cosine_40F520(rotation) + door_y * Ang16::sine_40F500(rotation);
    door_y = (-old_xpos).Multiply(Ang16::sine_40F500(rotation)).Add(door_y.Multiply(Ang16::cosine_40F520(rotation)));

    *pOutX = mCarSprite->mXy.x + x_off;
    *pOutY = mCarSprite->mXy.y + door_y;
}

MATCH_FUNC(0x43B540)
bool Car::IsDoorOffsetLarge(u8 targetDoor) const
{
    s8* pDoorData = reinterpret_cast<s8*>(gGtx_0x106C_703DD4->get_car_door_data(mCarInfoIdx));
    if (Fix16::Abs(gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pDoorData[2 * targetDoor + 1])) >= kFpOne)
    {
        return true;
    }
    return false;
}

// Ang16::RotateVector_41FC90 with the new x built in a local and stored last: with the header
// version VC6 folds the new x into the caller's final add (GetDoorWorldPosition)
static inline void __stdcall RotateVector_41FC90_NewX(Fix16& xpos, Fix16& ypos, Ang16& rotation)
{
    Fix16 old_xpos = xpos;
    Fix16 x_new;
    x_new = xpos * Ang16::cosine_40F520(rotation) + ypos * Ang16::sine_40F500(rotation);
    ypos = (-old_xpos) * Ang16::sine_40F500(rotation) + ypos * Ang16::cosine_40F520(rotation);
    xpos = x_new;
}

// https://decomp.me/scratch/2BtJU
MATCH_FUNC(0x43b5a0)
void Car::GetDoorWorldPosition(u8 target_door, Fix16* pOutX, Fix16* pOutY)
{
    Fix16 door_relative_xpos;
    Fix16 door_relative_ypos;

    u8* pDoorOffset = &gGtx_0x106C_703DD4->get_car_door_data(mCarInfoIdx)[2 * target_door + 1];

    door_relative_xpos = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pDoorOffset[0]);
    door_relative_ypos = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pDoorOffset[1]);

    if (door_relative_xpos >= kFpOne)
    {
        door_relative_xpos -= kFpOne;
    }
    else
    {
        if (door_relative_xpos <= -kFpOne)
        {
            door_relative_xpos += kFpOne;
        }
    }

    switch (target_door)
    {
        case 0:
            door_relative_xpos += kFpPoint2_6772BC;
            break;
        case 1:
        case 3:
            door_relative_xpos -= kFpPoint2_6772BC;
            break;
        case 2:
            if (door_relative_xpos == gFix16_6777CC)
            {
                door_relative_ypos -= kFpPoint2_6772BC;
            }
            else if (door_relative_xpos < gFix16_6777CC)
            {
                door_relative_xpos -= kFpPoint2_6772BC;
            }
            else
            {
                door_relative_xpos += kFpPoint2_6772BC;
            }
            break;
        default:
            break;
    }

    RotateVector_41FC90_NewX(door_relative_xpos, door_relative_ypos, mCarSprite->mRotation);

    *pOutX = door_relative_xpos + mCarSprite->mXy.x;
    *pOutY = door_relative_ypos + mCarSprite->mXy.y;
}

MATCH_FUNC(0x43b730)
bool Car::IsOnScreenForAnyPlayer() const
{
    return gGame_0x40_67E008->IsSpriteInAnyPlayerBounds(mCarSprite, kFpFive_6778D0);
}

MATCH_FUNC(0x43b750)
bool Car::IsVisibleToAnyPlayer() const
{
    return gGame_0x40_67E008->IsSpriteInBoundsOrFollowedByAnyPlayer(mCarSprite, kFpFive_6778D0);
}

MATCH_FUNC(0x43b770)
void Car::AssignKillerToOccupants()
{
    Ped* pDriver = this->mDriver;
    if (pDriver)
    {
        if (pDriver->get_occupation_403980() != ped_ocupation_enum::dummy_driver_4)
        {
            pDriver->mKillerId = this->mExploderPedId;
            this->mDriver->mDeathCause = this->mDeathCause;
            this->mDriver->mKillerIdTimer = 50;
        }
    }
    mPassengersList.SyncPassengersWithCarState(this);
}

// 9.6f 0x420C30 (a method of an unknown global object in 9.6f that only looks at its argument)
static inline bool IsPlayerPedId_420C30(u32 ped_id)
{
    return ped_id <= 12;
}

MATCH_FUNC(0x43b7b0)
void Car::AssignDriverBlameForExplosion(Car* pCar)
{
    bool bBlameUs = false;
    bool bBlameThem = false;
    u32 our_id = GetEffectiveDriverPedId();
    u32 their_id = pCar->GetEffectiveDriverPedId();
    bool bOurIsPlayer = IsPlayerPedId_420C30(our_id);
    bool bTheirIsPlayer = IsPlayerPedId_420C30(their_id);

    if (bOurIsPlayer)
    {
        if (bTheirIsPlayer && our_id != their_id)
        {
            bBlameUs = true;
        }
    }
    else if (their_id > 0 && our_id != their_id)
    {
        bBlameUs = true;
    }

    if (bTheirIsPlayer)
    {
        if (bOurIsPlayer && their_id != our_id)
        {
            bBlameThem = true;
        }
    }
    else if (our_id > 0 && their_id != our_id)
    {
        bBlameThem = true;
    }

    if (bBlameUs)
    {
        this->mExploderPedId = their_id;
        this->mDeathCause = ped_death_cause::run_over_1;
        this->mExploderTimer = 50;
    }
    if (bBlameThem)
    {
        pCar->mExploderPedId = our_id;
        pCar->mDeathCause = ped_death_cause::run_over_1;
        pCar->mExploderTimer = 50;
    }
}

MATCH_FUNC(0x43b850)
bool Car::IsImmuneToExplosionType(s32 type) const
{
    // Two separate bit tests: VC6 loads the u16 once into cx and merges them into 'test $6,%ch'
    return ((mFlags & car_flag::Rocketproof) || (mFlags & car_flag::Flameproof)) && type != explosion_type::large_20 ? true : false;
}

// 9.6f 0x41FF40 (a car_info helper, inlined in 10.5): each inlined call gets its own return temp
static inline Fix16 Get_front_window_offset(car_info* p)
{
    return gPixelsToFix16_6F6850.SignedPixelsToFix16_440860(p->front_window_offset);
}
// 9.6f 0x41FF20
static inline Fix16 Get_rear_window_offset(car_info* p)
{
    return gPixelsToFix16_6F6850.SignedPixelsToFix16_440860(p->rear_window_offset);
}
// 9.6f 0x41FF60
static inline Fix16 Get_half_width(car_info* p)
{
    return kFpPoint4_677214 * gPixelsToFix16_6F6850.list[p->w];
}
// 9.6f 0x4226C0
MATCH_FUNC(0x43b870)
void Car::SpawnDamageFireEffect(s32 fire_size, const Fix16_Point* pPos)
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);

    Fix16 x_hit;
    Fix16 y_hit;
    Fix16_Point rot_point; // function scope: the original has its EH state 0 from entry

    if (pPos->IsNull_420360())
    {
        switch (static_cast<u8>(fire_size))
        {
            case 1u: // small fire
                y_hit = Get_rear_window_offset(pCarInfo);
                if (gCarManager_6C_677930->mFireEffectCycle && gCarManager_6C_677930->mFireEffectCycle != 2)
                {
                    x_hit = gFix16_6777CC;
                }
                else
                {
                    x_hit = Get_half_width(pCarInfo);
                }
                break;

            case 2u: // huge fire
                x_hit = -(Get_half_width(pCarInfo));
                if (gCarManager_6C_677930->mFireEffectCycle && gCarManager_6C_677930->mFireEffectCycle != 3)
                {
                    y_hit = Get_front_window_offset(pCarInfo);
                }
                else
                {
                    y_hit = Get_rear_window_offset(pCarInfo);
                }
                break;

            case 3u: // big fire, no caller in the game passes 3
                y_hit = Get_front_window_offset(pCarInfo);
                if (gCarManager_6C_677930->mFireEffectCycle && gCarManager_6C_677930->mFireEffectCycle != 2)
                {
                    x_hit = gFix16_6777CC;
                }
                else
                {
                    x_hit = Get_half_width(pCarInfo);
                }
                break;

            default:
                // x_hit is left uninitialised here, as in 9.6f (the original reads whatever its slot holds)
                break;
        }
        x_hit *= mScale;
        y_hit *= mScale;
    }
    else
    {
        rot_point = *pPos - mCarSprite->get_x_y();
        rot_point.RotateByAngle_40F6B0(mCarSprite->mRotation);
        x_hit = rot_point.x;
        y_hit = rot_point.y;
    }

    Object* pExplosion = gObject_5C_6F8F84->CreateExplosion(mCarSprite->mXy.x,
                                                                      mCarSprite->mXy.y,
                                                                      2, // implicit: the out-of-line Fix16(s32) ctor
                                                                      kAngZero_67791C,
                                                                      Car::GetFireExplosionType(fire_size),
                                                                      mExploderPedId);
    if (pExplosion)
    {
        mCarSprite->AttachSprite(pExplosion->mSprite, x_hit, y_hit, kAngZero_67791C);
        if (++gCarManager_6C_677930->mFireEffectCycle > 3u)
        {
            gCarManager_6C_677930->mFireEffectCycle = 0;
        }
    }
}

MATCH_FUNC(0x43bb90)
s32 __stdcall Car::GetFireExplosionType(u8 fire_level)
{
    switch (fire_level)
    {
        case 1:
            return explosion_type::car_fire_level1_12;

        case 2:
            return explosion_type::car_fire_level2_13;

        case 3:
            return explosion_type::car_fire_level3_14;

        default:
            return explosion_type::car_fire_level1_12;
    }
}

MATCH_FUNC(0x43bbc0)
void Car::SpawnFire()
{
    if (gFirefighterPool_54_67D4C0->TryDispatchFirefightersToCar(this))
    {
        Object* pExplosion =
            gObject_5C_6F8F84->CreateExplosion(mCarSprite->mXy.x, mCarSprite->mXy.y, 4, kAngZero_67791C, explosion_type::car_fire_4, mExploderPedId);
        if (pExplosion)
        {
            mCarSprite->AttachSprite(pExplosion->mSprite, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
        }
    }
}

MATCH_FUNC(0x43bc30)
void Car::SetupCarPhysicsAndSpriteBinding_43BC30()
{
    if (!mPhysics)
    {
        AllocCarPhysics();
        mPhysics->SetCar(this);
        mPhysics->SetSprite(this->mCarSprite);
    }
    else
    {
        if (IsTrainModel_403BA0())
        {
            mPhysics->SetSprite(this->mCarSprite);
        }
    }
}

MATCH_FUNC(0x43bca0)
void Car::SetupCarPhysicsAndSpriteBinding_43BCA0()
{
    if (mpTrailer)
    {
        mpTrailer->SetupCarPhysicsAndSpriteBinding();
    }
    else
    {
        SetupCarPhysicsAndSpriteBinding_43BC30();
    }
}

MATCH_FUNC(0x43bd00)
void Car::DeAllocateCarPhysics_43BD00()
{
    if (mpTrailer)
    {
        mpTrailer->DeAllocateCarPhysics();
    }
    else
    {
        DeAllocateCarPhysics_441A10();
    }
}

MATCH_FUNC(0x43bd40)
void Car::FlashHeadlights()
{
    if ((mLightFlags & kEmergencyOrFlashingLightsMask) == 0)
    {
        if (mBfA4.bHeadlights)
        {
            ClearHeadlightDeltas_425650();
        }
        else
        {
            SetHeadlightDeltas_425590();
        }

        this->mFlashPhaseCounter = 6;
        this->mBfA4.bHeadlightFlash = true;
    }
}

MATCH_FUNC(0x43bf10)
void Car::BrakeLightsOn()
{
    if (!this->mBfA4.bLightFlashing)
    {
        SetBrakeLightDeltas_4213D0();
    }
    this->mBfA4.bBrakeLights = true;
}

MATCH_FUNC(0x43bf70)
void Car::BrakeLightsOff()
{
    if (!this->mBfA4.bLightFlashing)
    {
        ClearBrakeLightDeltas_421430();
    }
    this->mBfA4.bBrakeLights = false;
}

MATCH_FUNC(0x43bfe0)
void Car::HeadlightsOn()
{
    if (!this->mBfA4.bLightFlashing)
    {
        SetHeadlightDeltas_425590();
    }
    this->mBfA4.bHeadlights = true;
}

MATCH_FUNC(0x43c0c0)
void Car::HeadlightsOff()
{
    if (!mBfA4.bLightFlashing)
    {
        ClearHeadlightDeltas_425650();
    }

    mBfA4.bHeadlights = false;
}

MATCH_FUNC(0x43c1c0)
void Car::PrepareForExplosion()
{
    if (this->mBfA4.bLightFlashing)
    {
        Car::StopLightFlashing();
    }

    this->mLightFlags = 0;

    if (HasEmergencyLights_414F20())
    {
        Car::DeactivateEmergencyLights();
    }

    Car::HeadlightsOff();
    Car::BrakeLightsOff();
}

MATCH_FUNC(0x43c260)
void Car::UpdateTopRightRoofLight()
{
    if (HasTopRoofLights_4216A0() && !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
    {
        this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightRoofLight_18);
        Object* pObject = mAttachments.FindObject2CByModel(172);
        pObject->UpdateLight();
    }
}

MATCH_FUNC(0x43c310)
void Car::ResetTopRightRoofLight()
{
    if (HasTopRoofLights_4216A0() && !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
    {
        this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightRoofLight_18);
        Object* pLight = mAttachments.FindObject2CByModel(172);
        if (pLight)
        {
            pLight->TurnOffLight();
        }
    }
}

MATCH_FUNC(0x43c3c0)
void Car::UpdateTopLeftRoofLight()
{
    if (HasTopRoofLights_4216A0() && !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
    {
        this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopLeftRoofLight_17);
        Object* pLight = mAttachments.FindObject2CByModel(173);
        pLight->UpdateLight();
    }
}

MATCH_FUNC(0x43c470)
void Car::ResetTopLeftRoofLight()
{
    if (HasTopRoofLights_4216A0() && !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
    {
        this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftRoofLight_17);
        Object* pLight = mAttachments.FindObject2CByModel(173);
        if (pLight)
        {
            pLight->TurnOffLight();
        }
    }
}

MATCH_FUNC(0x43c500)
void Car::UpdateRoofLights()
{
    if (!IsFbiCar_411920() ||
        !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11) &&
            !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12) &&
            !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13) &&
            !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
    {
        mDamagedAreas.set_bit(CarDeltaBitsEnum::BottomRightRoofLight_16);

        Object* pLight = mAttachments.FindObject2CByModel(objects::light_165);
        pLight->UpdateLight();
    }

    if (IsSwatVan_4217A0() || IsFireTruck_4118F0())
    {
        Object* pLight = mAttachments.FindObject2CByModel(172);
        pLight->UpdateLight();
    }
}

MATCH_FUNC(0x43c650)
void Car::ResetRoofLights()
{
    mDamagedAreas.clear_bit(CarDeltaBitsEnum::BottomRightRoofLight_16);

    Object* pLight = mAttachments.FindObject2CByModel(objects::light_165);
    if (pLight)
    {
        pLight->TurnOffLight();
    }

    if (IsSwatVan_4217A0() || IsFireTruck_4118F0())
    {
        Object* pSecondLight = mAttachments.FindObject2CByModel(172);
        if (pSecondLight)
        {
            pSecondLight->TurnOffLight();
        }
    }
}

MATCH_FUNC(0x43c700)
void Car::UpdateBottomLeftRoofLight()
{
    if (!IsFbiCar_411920() ||
        !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11) &&
            !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12) &&
            !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13) &&
            !mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
    {
        mDamagedAreas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }

    if (!IsFbiCar_411920())
    {
        Object* pLight = mAttachments.FindObject2CByModel(171);
        pLight->UpdateLight();
    }

    if (IsSwatVan_4217A0() || IsFireTruck_4118F0())
    {
        Object* pLight = mAttachments.FindObject2CByModel(173);
        pLight->UpdateLight();
    }
}

MATCH_FUNC(0x43c840)
void Car::ResetBottomLeftRoofLight()
{
    mDamagedAreas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);

    if (!IsFbiCar_411920())
    {
        Object* pObject = mAttachments.FindObject2CByModel(171);
        if (pObject)
        {
            pObject->TurnOffLight();
        }
    }

    if (IsSwatVan_4217A0() || IsFireTruck_4118F0())
    {
        Object* pObject = mAttachments.FindObject2CByModel(173);
        if (pObject)
        {
            pObject->TurnOffLight();
        }
    }
}

MATCH_FUNC(0x43c920)
void Car::ActivateEmergencyLights()
{
    if ((mLightFlags & kEmergencyOrFlashingLightsMask) == 0)
    {
        if (IsFbiCar_411920())
        {
            mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        }
        mBfA4.bEmergencyLights = true;
        if (IsFbiCar_411920())
        {
            mFlashPhaseCounter = 8;
        }
        else
        {
            mFlashPhaseCounter = 15;
        }
    }
}

MATCH_FUNC(0x43c9d0)
void Car::DeactivateEmergencyLights()
{
    Car::ResetRoofLights();
    Car::ResetBottomLeftRoofLight();
    Car::ResetTopRightRoofLight();
    Car::ResetTopLeftRoofLight();
    mBfA4.bEmergencyLights = false;
    if (IsFbiCar_411920() && !IsMaxDamage_40F890())
    {
        mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor4_14);
    }
}

MATCH_FUNC(0x43ca80)
void Car::ToggleEmergencyLights()
{
    if ((mLightFlags & kEmergencyOrFlashingLightsMask) != 0)
    {
        Car::DeactivateEmergencyLights();
    }
    else
    {
        Car::ActivateEmergencyLights();
    }
}

MATCH_FUNC(0x43cbe0)
void Car::StopLightFlashing()
{
    mBfA4.bLightFlashing = false;

    if (mBfA4.bHeadlights)
    {
        SetHeadlightDeltas_425590();
    }
    else
    {
        ClearHeadlightDeltas_425650();
    }

    if (this->mBfA4.bBrakeLights)
    {
        SetBrakeLightDeltas_4213D0();
    }
    else
    {
        ClearBrakeLightDeltas_421430();
    }

    this->mFlashCount = 0;
}

MATCH_FUNC(0x43cdf0)
void Car::SetCarGraphic(char_type graphic_idx)
{
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::BottomRightRoofLight_16);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftRoofLight_17);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightRoofLight_18);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::Bit19_19);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::Bit20_20);

    if (graphic_idx != 10)
    {
        this->mDamagedAreas.set_bit(graphic_idx + CarDeltaBitsEnum::TopRightDoor1_11);
    }
}

MATCH_FUNC(0x43cf30)
void Car::DamageArea(s32 damage_area)
{
    switch (damage_area)
    {
        case 0:
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
            if (HasPopupHeadlights_421700())
            {
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
            }
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::BottomRightDamage_2);
            break;

        case 1:
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::BottomLeftDamage_3);
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
            if (HasPopupHeadlights_421700())
            {
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
            }
            break;

        case 2:
            ResetTopRightRoofLight();
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDamage_1);
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
            break;

        case 3:
            ResetTopLeftRoofLight();
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopLeftDamage_0);
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
            break;

        case 4:
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::WindshieldDamage_4);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x43d1c0)
bool Car::IsAreaDamaged(s32 damage_area)
{
    // damage_area is the script's damage position (miss2_0x11C), not the bit: 10.5.exe's jump table sends
    // 0 to the BottomRight test, 1 to BottomLeft, 2 to TopRight, 3 to TopLeft and 4 to the windshield
    switch (damage_area)
    {
        case 0:
            return mDamagedAreas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2);

        case 1:
            return mDamagedAreas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3);

        case 2:
            return mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1);

        case 3:
            return mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0);

        case 4:
            return mDamagedAreas.mask_bit(CarDeltaBitsEnum::WindshieldDamage_4);

        default:
            return false;
    }
    return false;
}

// 9.6f: the mFlags test is Car::IsFlagSet_411930 (inlined, as a bool helper it changes the code)
MATCH_FUNC(0x43d2c0)
void Car::TryDamageArea(u8 damage_area, s32 damageAmount)
{
    if (!IsMaxDamage_40F890() && !(mFlags & car_flag::NoCollisionDamage) && (GetAntiStrength() * Fix16(damageAmount, 0)) >= kFP16One_6777D0)
    {
        switch (damage_area)
        {
            case 0:
                DamageArea(3);
                break;
            case 1:
                DamageArea(2);
                break;
            case 2:
                DamageArea(0);
                break;
            case 3:
                DamageArea(1);
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x43d400)
void Car::RepairDamage()
{
    this->mDamage = 0;
    this->mDamageLevel = 0;

    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDamage_0);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDamage_1);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::BottomRightDamage_2);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::BottomLeftDamage_3);
    this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::WindshieldDamage_4);

    if (this->mBfA4.bHeadlights)
    {
        SetHeadlightDeltas_425590();
    }

    if (this->mBfA4.bBrakeLights)
    {
        SetBrakeLightDeltas_4213D0();
    }

    mAttachments.CleanupSpriteList();
    ResetEngineStatus_421570();
}

MATCH_FUNC(0x43d690)
void Car::EmitExplosion(s32 type, Fix16 x, Fix16 y)
{
    Fix16_Point unused; // EH frame with state 0 before the first call: the original has a destructible local

    Object* pObject = gObject_5C_6F8F84->CreateExplosion(gFix16_6777CC, gFix16_6777CC, 2, kAngZero_67791C, type, mExploderPedId);
    if (pObject)
    {
        mCarSprite->AttachSprite(pObject->mSprite, x, y, kAngZero_67791C);
    }
}

MATCH_FUNC(0x43d7b0)
void Car::TriggerExplosion(s32 type)
{
    if (GetAntiStrength() != gFix16_6777CC && !this->IsMaxDamage_40F890())
    {
        HandleCarExplosion(type);
    }
    else
    {
        EmitExplosion(type, kZeroPoint.x, kZeroPoint.y);
    }
}

MATCH_FUNC(0x43d840)
void Car::HandleCarExplosion(s32 type)
{
    bool bOcc2 = false;
    if (mpChaseTask)
    {
        gCarChaseTaskTable_678E30->FreeEntry(mpChaseTask);
        this->mpChaseTask = NULL;
    }

    if (!this->IsMaxDamage_40F890())
    {
        StartWreckCountdown();
        EmitExplosion(type, kZeroPoint.x, kZeroPoint.y);
        AssignKillerToOccupants();
        mAttachments.ExpireAttachedFlames();

        if (mDriver)
        {
            if (mDriver->get_occupation_403980() == ped_ocupation_enum::dummy_driver_4)
            {
                bOcc2 = true;
            }
        }

        KillContainedPeds();

        ApplyExplosionImpulse(&mCarSprite->get_x_y());

        this->mDamage = 32001;

        mAttachments.DestroyAllAttached();

        const s32 model_to_check = gCarManager_6C_677930->mModelToCheckDestroy;
        if (model_to_check != car_model_enum::none && model_to_check == this->mCarInfoIdx)
        {
            gCarManager_6C_677930->mModelCheckDestroyed = true;
        }

        if (this->mExploderPedId)
        {
            Ped* pExploder = gPedManager_6787BC->PedById(this->mExploderPedId);
            if (pExploder)
            {
                if (pExploder->PedTypeIs(ped_type::player_2))
                {
                    if (gPublicTransport_181C_6FF1D4->is_bus(this) &&
                        gPublicTransport_181C_6FF1D4->mBus.mPassengerCount >= 10)
                    {
                        pExploder->mPlayer->mScores.AwardFullBusDestroyedScore(this);
                    }
                    else
                    {
                        pExploder->mPlayer->mScores.AwardCarDestroyedScore(this, pExploder);
                        if (bOcc2)
                        {
                            pExploder->mPlayer->mScores.AddCashWithMultiplier();
                        }
                    }
                    s16 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel(this->mCarInfoIdx);
                    if (gang_idx != -1)
                    {
                        Gang* pGang = gGangPool_CA8_67E274->GangByIdx(static_cast<u8>(gang_idx));
                        pGang->ApplyKillRespectChange(pExploder->mPlayer->get_idx_4219D0(), 1u);
                    }
                    if (pExploder->is_player_41B0A0())
                    {
                        if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime(this, pExploder->mPlayer))
                        {
                            if (pExploder->mWantedPoints < cop_level_ped_enum::cop_1_stars)
                            {
                                pExploder->mWantedPoints = cop_level_ped_enum::cop_1_stars;
                            }
                            else
                            {
                                pExploder->add_wanted_points(200);
                            }
                        }
                    }
                }
            }
        }
    }
}

// 9.6f 0x427180
MATCH_FUNC(0x43da90)
s16 Car::AccumulateDamage(s16 damage, const Fix16_Point* pVec)
{
    if (IsMaxDamage_40F890())
    {
        return 0;
    }

    Fix16 anti_strength;
    anti_strength = GetAntiStrength();
    Fix16 scaled_damage = (anti_strength * Fix16(damage));
    s16 applied_damage = scaled_damage.ToInt();

    if (applied_damage == 0)
    {
        if (anti_strength != gFix16_6777CC)
        {
            applied_damage = 1;
        }
    }

    this->mDamage += applied_damage;
    if (this->mDamage < 0 || this->mDamage > 32000)
    {
        this->mDamage = 32000;
    }

    if (this->mDamage >= 16000)
    {
        if (this->mDamageLevel < 3)
        {
            Car::SpawnDamageFireEffect(1, pVec);
            this->mDamageLevel = 3;
        }
        if (this->mDamage >= 25000)
        {
            if (this->mDamageLevel < 4)
            {
                mAttachments.ExpireAttachedFlames();
                Car::SpawnDamageFireEffect(2, pVec);
                this->mDamageLevel = 4;
            }
            if (this->mDamage >= 31500)
            {
                Car::PrepareForExplosion();
            }
            if (this->mDamage == 32000)
            {
                Car::HandleCarExplosion(explosion_type::item_19);
            }
        }
    }
    return applied_damage;
}

MATCH_FUNC(0x43db80)
void Car::KillContainedPeds()
{
    Ped* pDriver = this->mDriver;
    if (pDriver)
    {
        if (pDriver->get_occupation_403980() != ped_ocupation_enum::dummy_driver_4)
        {
            if (pDriver->is_player_41B0A0())
            {
                CarPhysics* pPhysics = this->mPhysics;
                if (pPhysics)
                {
                    pPhysics->SetField8C_to_1();
                }
            }
            mDriver->Kill();
        }
        this->mDriver = NULL;
    }
    mPassengersList.KillAndClearAllPedsResetCarRef();
}

MATCH_FUNC(0x43dbd0)
void Car::StopMovement()
{
    CarPhysics* pPhysics = this->mPhysics;
    if (pPhysics)
    {
        pPhysics->mLinvel1 = kZeroPoint;
        this->mPhysics->mAngVelRad = gFix16_6777CC;
    }
}

MATCH_FUNC(0x43dc00)
bool Car::IsSpriteShrunk() const
{
    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
    if (IsMaxDamage_40F890())
    {
        SpriteShape* pShape = mCarSprite->mpCollisionShape;
        SpriteShape* pVisualShape = mCarSprite->mpVisualShape;
        Fix16 shapeWidth = pShape->mWidth;
        Fix16 visualWidth = pVisualShape->mWidth;
        return (visualWidth != shapeWidth) != 0;
    }

    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex(sprite_types_enum::car_2, pInfo->sprite);
    sprite_index* pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index(sprite_idx);
    Fix16 visualWidth = mCarSprite->mpVisualShape->mWidth;
    return visualWidth.mValue != gPixelsToFix16_6F6850.list[pSpriteIndex->mWidth].mValue;
}

MATCH_FUNC(0x43dc80)
bool Car::CarShrinkSprite(s32 xoff, s32 yoff)
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
    Fix16 w_fp(pCarInfo->w << 14, 0);
    Fix16 h_fp(pCarInfo->h << 14, 0);
    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex(sprite_types_enum::car_2, pCarInfo->sprite);
    sprite_index* pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index(sprite_idx);
    Fix16 sprite_w = gPixelsToFix16_6F6850.list[pSpriteIndex->mWidth];
    Fix16 sprite_h = gPixelsToFix16_6F6850.list[pSpriteIndex->mHeight];
    return mCarSprite->ShrinkSprite((sprite_w * xoff) / w_fp, (sprite_h * yoff) / h_fp, 0);
}

MATCH_FUNC(0x43dd50)
bool Car::IsBeingCrushed()
{
    return gCrusherPool_94_67A830->IsCarBeingCrushed(this);
}

MATCH_FUNC(0x43dd60)
void Car::DestroyWhileSinking()
{
    bool bOcc2 = false;

    StopMovement();
    mCarSprite->IncreaseZoom(1);
    mAttachments.SinkAllAttachedCars();

    if (CarShrinkSprite(2, 2))
    {
        Ped* pDriver = this->mDriver;
        if (pDriver)
        {
            if (pDriver->get_occupation_403980() == ped_ocupation_enum::dummy_driver_4)
            {
                bOcc2 = true;
            }
        }
        const s32 exploder_ped_id = this->mExploderPedId;
        if (exploder_ped_id)
        {
            Ped* pExploderPed = gPedManager_6787BC->PedById(exploder_ped_id);
            if (pExploderPed)
            {
                if (pExploderPed->PedTypeIs(ped_type::player_2))
                {
                    if (gPublicTransport_181C_6FF1D4->is_bus(this) &&
                        gPublicTransport_181C_6FF1D4->mBus.mPassengerCount >= 10)
                    {
                        pExploderPed->mPlayer->mScores.AwardFullBusDestroyedScore(this);
                    }
                    else
                    {
                        pExploderPed->mPlayer->mScores.AwardCarDestroyedScore(this, pExploderPed);
                        if (bOcc2)
                        {
                            pExploderPed->mPlayer->mScores.AddCashWithMultiplier();
                        }
                    }
                    s16 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel(this->mCarInfoIdx);
                    if (gang_idx != -1)
                    {
                        Gang* pGang = gGangPool_CA8_67E274->GangByIdx(static_cast<u8>(gang_idx));
                        pGang->ApplyKillRespectChange(pExploderPed->mPlayer->get_idx_4219D0(), 1u);
                    }
                    if (pExploderPed->is_player_41B0A0())
                    {
                        if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime(this, pExploderPed->mPlayer))
                        {
                            if (pExploderPed->mWantedPoints < cop_level_ped_enum::cop_1_stars)
                            {
                                pExploderPed->mWantedPoints = cop_level_ped_enum::cop_1_stars;
                            }
                            else
                            {
                                pExploderPed->add_wanted_points(200);
                            }
                        }
                    }
                }
            }
        }

        const s32 model_to_check = gCarManager_6C_677930->mModelToCheckDestroy;
        if (model_to_check != car_model_enum::none && model_to_check == this->mCarInfoIdx)
        {
            gCarManager_6C_677930->mModelCheckDestroyed = true;
        }

        AssignKillerToOccupants();
        KillContainedPeds();

        SetEngineDamaged_421590();
        PrepareForExplosion();
        mAttachments.DestroyAllSprites();
        mCarSprite->set_z_lazy_420660(gFix16_6777CC);
        MarkForDespawn_421470();
        DeAllocateCarPhysics_43BD00();
    }
}

MATCH_FUNC(0x43e560)
bool Car::ManageDrowning()
{
    bool ret = mPhysics->IsDrowning_421100();
    if (ret)
    {
        if (this->mExploderTimer > 0)
        {
            this->mExploderTimer = 50;
        }

        DestroyWhileSinking();

        gParticle_8_6FD5E8->EmitWaterSplash(mCarSprite->mXy.x,
                                                   mCarSprite->mXy.y,
                                                   mCarSprite->mZpos,
                                                   mCarSprite->mRotation + kAng180_677326,
                                                   1);
    }
    return ret;
}

MATCH_FUNC(0x43e8d0)
Car* Car::GetCabOrSelf()
{
    if (mpTrailer)
    {
        return mpTrailer->mTruckCab;
    }
    return this;
}

MATCH_FUNC(0x43e990)
Ped* Car::GetEffectiveDriver() const
{
    if (IsTrailerCar_421720())
    {
        return mpTrailer->GetTruckCabDriver();
    }
    else
    {
        return this->mDriver;
    }
}

MATCH_FUNC(0x43ea60)
bool Car::OnObjectTouched(Object* pObj)
{
    if (pObj->IsPickup_421060() || pObj->mModel == objects::secret_token_266)
    {
        Ped* pPed = Car::GetEffectiveDriver();
        if (pPed)
        {
            return (pPed->HandlePickupCollision(pObj));
        }

        return false;
    }

    switch (pObj->mModel)
    {

        case objects::bus_stop_marker_129:
            gPublicTransport_181C_6FF1D4->OnBusStopMarkerHit(this);
            break;

        case objects::car_shop_130:
            GetCabOrSelf()->HandleShops(pObj);
            return true;

        case objects::crane_base_137:
            gCranePool_D9C_679FD4->PickUpCar(this, pObj->GetParam_421050());
            break;

        case objects::crusher_central_spot_143:
            gCrusherPool_94_67A830->CrushCarWithCrusher(this, pObj->GetParam_421050());
            break;

        case objects::animating_oil_8:
        case objects::oil_9:
            mPhysics->SpinOutOnOil();
            break;
        case objects::conveyor_139:
            mPhysics->ApplyConveyorPush(pObj); // push car into conveyor direction
            break;
        case objects::savepoint_161:
            gCarThreadTriggers_214_705F20->CheckThreadTrigger(pObj->mThreadTriggerIdx, this->mCarSprite);
            break;

        case objects::destructor_141:
            if (!mDriver)
            {
                DespawnSoon_421490();
            }
            break;

        case objects::door_trigger_167:
            gDoor_4D4_67BD2C->TryOpenDoorForCar(this, pObj->GetParam_421050());
            break;

        default:
            break;
    }
    return false;
}

MATCH_FUNC(0x48E720)
bool __stdcall IsNoKillCreditObject(s32 model)
{
    bool result;
    switch (model)
    {
        case objects::invisible_bullet_154:
        case objects::invisible_rocket_159:
        case objects::invisible_shotgun_bullet_193:
        case objects::flamethrower_bullet_195:
        case objects::fire_truck_gun_bullet_199:
            result = true;
            break;
        default:
            result = false;
            break;
    }
    return result;
}

MATCH_FUNC(0x48E780);
s32 __stdcall GetDeathCauseForObjectModel(s32 model)
{
    if (model <= 192)
    {
        if (model != objects::shotgun_bullet_192)
        {
            switch (model)
            {
                case objects::car_bomb_132:
                    return ped_death_cause::bomb_12;
                case 182:
                case objects::grenade_obj_183:
                    return ped_death_cause::grenade_15;
                case objects::moving_molotov_138:
                    return ped_death_cause::molotov_16;
                case objects::rocket_bullet_128:
                    return ped_death_cause::rocket_bullet_17;
                case objects::rocket_160:
                    return ped_death_cause::rocket_18;
                case objects::mine_10:
                    return ped_death_cause::mine_21;
            }
        }
        else
        {
            return ped_death_cause::shotgun_19;
        }
    }
    else
    {
        switch (model)
        {
            case objects::machine_gun_bullet_254:
            case objects::pistol_bullet_265:
                return ped_death_cause::bullet_11;
            case objects::fire_197:
                return ped_death_cause::fire_13;
            case objects::fire_hitting_194:
                return ped_death_cause::fire_hit_14;
            case objects::water_hitting_198:
            case objects::object_251:
                return ped_death_cause::burning_20;
        }
    }
    return ped_death_cause::projectile_default_9;
}

MATCH_FUNC(0x45CF90)
s32 __stdcall GetDamageMultiplier(Ped* pPed)
{
    if (pPed && pPed->mPlayer && pPed->mPlayer->mPowerUpTimers[power_up_indices::DoubleDamage_7] != 0)
    {
        return 2;
    }
    return 1;
}

// impact_vec is read uninitialised, as in the original
#pragma warning(push)
#pragma warning(disable : 4700)
// 9.6f 0x4274C0. The Fix16 and Fix16_Point operators are the out-of-line copies (Add,
// Multiply, operator- 0x40AC80, Multiply, operator+ 0x408660, Normalize_406C20).
// The 9.6f getters (IsMaxDamage_40F890, the flag tests, get_x_y 0x4207B0) set the inline budget: every
// RotateByAngle_40F6B0 keeps only the x_old negate inline. The original keeps EH state stores around the
// get_x_y/operator+/operator- calls, so their bodies weren't compiled yet: the *Late* aliases are defined at
// the end of this file (see "An inline defined after its callers" in docs/matching_quirks.md).
MATCH_FUNC(0x43f130)
bool Car::HandleCarHitByObject(Object* pObj)
{
    s16 damage = 0;
    Ped* pFoundPed = NULL;
    Fix16_Point offset;
    Fix16_Point impact_vec; // never set: 265 and default pass it uninitialised

    if (pObj->GetPedRefIdx_420FF0())
    {
        s32 pedId = gPedRefTable_7F8_703398->GetPedId_420F10(pObj->GetPedRefIdx_420FF0());
        pFoundPed = pedId ? gPedManager_6787BC->PedById(pedId) : NULL;
        if (pFoundPed)
        {
            pFoundPed->HandleShootingAtCar(this, pObj->mModel);
        }

        if (pObj->mModel == objects::invisible_bullet_154 || pObj->mModel == objects::invisible_shotgun_bullet_193 ||
            pObj->mModel == objects::flamethrower_bullet_195 || pObj->mModel == objects::invisible_rocket_159 ||
            pObj->mModel == objects::fire_truck_gun_bullet_199)
        {
            return true;
        }
    }

    if (mDriver && pObj->mModel != objects::mine_10 && pObj->GetPedRefIdx_420FF0() == mDriver->get_ped_ref_idx_420B50())
    {
        return false;
    }

    if (pObj->mModel != objects::water_hitting_198 && !IsNoKillCreditObject(pObj->mModel))
    {
        if (pFoundPed)
        {
            this->mExploderPedId = pFoundPed->mId;
            this->mDeathCause = GetDeathCauseForObjectModel(pObj->mModel);
            this->mExploderTimer = 50;
        }
    }

    gScriptManager_6F8060->RecordWeaponHit(mCarId, pObj->mModel, 0);

    switch (pObj->mModel)
    {
        case objects::rocket_bullet_128:
        case objects::moving_molotov_138:
            if (!IsFlagSet_411930(car_flag::Rocketproof) && !IsMaxDamage_40F890())
            {
                damage = AccumulateDamage(
                    32000,
                    &((pObj->mSprite->get_x_y_late_443580().AddLate_40AC50(mCarSprite->get_x_y_late_443580())) * kFpHalf_677218));
                if (!IsMaxDamage_40F890())
                {
                    offset = pObj->mSprite->get_x_y_late_443580().SubLate_40AC80(mCarSprite->get_x_y_late_443580());
                    offset.RotateByAngle_40F6B0(-mCarSprite->mRotation);
                    EmitExplosion(explosion_type::small_18, offset.x, offset.y);
                }
            }
            else
            {
                offset = pObj->mSprite->get_x_y_late_443580().SubLate_40AC80(mCarSprite->get_x_y_late_443580());
                offset.RotateByAngle_40F6B0(-mCarSprite->mRotation);
                EmitExplosion(explosion_type::small_18, offset.x, offset.y);
            }
            break;

        case objects::mine_10:
            if (!gCarManager_6C_677930->mObjectHitActive)
            {
                gCarManager_6C_677930->mObjectHitActive = true;

                if (!IsMaxDamage_40F890())
                {
                    if (!IsFlagSet_411930(car_flag::Rocketproof))
                    {
                        damage = AccumulateDamage(
                            32000,
                            &((pObj->mSprite->get_x_y_late_443580().AddLate_40AC50(mCarSprite->get_x_y_late_443580())) * kFpHalf_677218));
                    }

                    if (!IsMaxDamage_40F890())
                    {
                        offset = pObj->mSprite->get_x_y_late_443580().SubLate_40AC80(mCarSprite->get_x_y_late_443580());
                        offset.RotateByAngle_40F6B0(-mCarSprite->mRotation);
                        EmitExplosion(explosion_type::small_18, offset.x, offset.y);
                    }
                }
            }
            break;

        case objects::fire_hitting_194:
            if (!IsFlameproof_425770())
            {
                damage = AccumulateDamage(
                    100 * GetDamageMultiplier(pFoundPed),
                    &((pObj->mSprite->get_x_y_late_443580().AddLate_40AC50(mCarSprite->get_x_y_late_443580())) * kFpHalf_677218));
            }
            break;

        case objects::pistol_bullet_265:
            if (!IsFlagSet_411930(car_flag::Bulletproof))
            {
                damage = AccumulateDamage(
                    1600 * GetDamageMultiplier(pFoundPed),
                    &((pObj->mSprite->get_x_y_late_443580().AddLate_40AC50(mCarSprite->get_x_y_late_443580())) * kFpHalf_677218));
            }
            gParticle_8_6FD5E8->EmitImpactParticles(pObj->mSprite->mXy.x,
                                                           pObj->mSprite->mXy.y,
                                                           pObj->mSprite->mZpos,
                                                           impact_vec);
            break;

        default:
            if (!IsFlagSet_411930(car_flag::Bulletproof))
            {
                damage = AccumulateDamage(
                    800 * GetDamageMultiplier(pFoundPed),
                    &((pObj->mSprite->get_x_y_late_443580().AddLate_40AC50(mCarSprite->get_x_y_late_443580())) * kFpHalf_677218));
            }
            gParticle_8_6FD5E8->EmitImpactParticles(pObj->mSprite->mXy.x,
                                                           pObj->mSprite->mXy.y,
                                                           pObj->mSprite->mZpos,
                                                           impact_vec);
            break;

        case objects::water_hitting_198:
            mAttachments.CleanupSpriteList();
            break;

        case objects::electrobaton_bullet_277:
            break;
    }

    switch (pObj->mModel)
    {
        case objects::shotgun_bullet_192:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
            this->mSoundEvent = 3;
            break;
    }

    if (pObj->mModel != objects::water_hitting_198 && damage > 0)
    {
        if (pFoundPed && pFoundPed->PedTypeIs(ped_type::player_2))
        {
            pFoundPed->mPlayer->mScores.AwardCarDamageScoreHit(this, 1);
        }
    }

    return true;
}

#pragma warning(pop)

MATCH_FUNC(0x440510)
Fix16 Car::GetMaxTurnRate()
{
    if (gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx)->h <= 64u)
    {
        return kFpPoint26 * GetCarLinearSpeed();
    }
    else
    {
        return kFpPoint13_676984 * GetCarLinearSpeed();
    }
}

MATCH_FUNC(0x440570)
char_type Car::GetMaxPassengers() const
{
    if (IsFlagSet_411930(car_flag::NoPassengers))
    {
        return 0;
    }
    else
    {
        return gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx)->passengers;
    }
}

MATCH_FUNC(0x440590)
void Car::InitCarAIControl()
{
    SetupCarPhysicsAndSpriteBinding_43BCA0();

    mPhysics->SetHandBrakeOff_421260();

    Ped* pDriver = this->mDriver;
    if (pDriver)
    {
        if (pDriver->GetPedType_420B70() != ped_type::player_2)
        {
            this->AllocateAI_421510();
            this->mAI->SetCar(this);
            StartEngine_426E00();
        }
    }
}

MATCH_FUNC(0x4405f0)
void Car::SpawnDriverPed()
{
    Ped* pNextPed = gCarManager_6C_677930->GetSpareDriver_421960();
    if (!pNextPed)
    {
        pNextPed = gPedManager_6787BC->CreateDummyDriver(this);
        gCarManager_6C_677930->SetSpareDriver_421970(pNextPed);
    }
    SetDriver(pNextPed);
    this->mPedType = ped_type::dummy_3;
}

MATCH_FUNC(0x440630)
void Car::AddGangDriver(Gang* pGang)
{
    Ped* pNewPed = gPedManager_6787BC->SpawnGangDriver(this, pGang);
    SetDriver(pNewPed);
    mPedType = ped_type::dummy_3;
}

MATCH_FUNC(0x440660)
void Car::AttachGangIcon(u8 arrow_colour)
{
    Object* pIcon =
        gObject_5C_6F8F84->NewPhysicsObj(arrow_colour + 286, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    mCarSprite->AttachSprite(pIcon->mSprite, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
}

MATCH_FUNC(0x4406b0)
void Car::ShowCarName(Ped* pPed)
{
    Player* pPlayer = pPed->mPlayer;
    if (pPlayer)
    {
        if (pPlayer->mIsUser)
        {
            gHud_2B00_706620->ShowCarName(GetCarStr());
        }
    }
}

MATCH_FUNC(0x4406e0)
void Car::AssignDriver(Ped* pPed)
{
    Player* pPlayer = pPed->mPlayer;
    SetDriver(pPed);
    InitCarAIControl();
    SetDriverPedType_421560(pPed->GetPedType_420B70());
    if (pPed->PedTypeIs(ped_type::player_2))
    {
        DeAllocateAI();
    }

    CancelDespawn_4215F0();

    if (IsCarThreadPending_4218F0())
    {
        if (gScriptManager_6F8060->TriggerCarThread(pPed->get_id(), mCarId))
        {
            ClearCarThreadPending_4218E0();
        }
    }
    if (pPlayer)
    {
        if (pPlayer->IsUser_41DC70())
        {
            if (mDriver->get_occupation_403980() != ped_ocupation_enum::empty)
            {
                gHud_2B00_706620->ShowCarName(GetCarStr());
            }
        }
        MarkProtoRecycled();
        pPlayer->AddCarToHistory(this);
        if (mDriver->get_occupation_403980() != ped_ocupation_enum::empty)
        {
            pPlayer->LoadCarWeapons(this);
        }

        if (mPhysics)
        {
            mPhysics->SetField8C_to_2();
        }
        mPlayerPedId = pPed->mId;
    }
    else
    {
        mPlayerPedId = 0;
    }
}

MATCH_FUNC(0x4407f0)
void Car::ClearDriver()
{
    Player* pPlayer = mDriver->mPlayer;
    if (pPlayer)
    {
        if (mDriver->get_occupation_403980() != ped_ocupation_enum::empty)
        {
            if (pPlayer->IsUser_41DC70())
            {
                gHud_2B00_706620->HideCarName_4219F0();
            }
            pPlayer->UnloadCarWeapons();
        }

        if (mPhysics)
        {
            mPhysics->SetField8C_to_1();
        }
    }
    SetDriver(NULL);
    ClearHorn_421460();
}

MATCH_FUNC(0x440840)
Sprite* Car::GetSprite() const
{
    Trailer* pTrailer = this->mpTrailer;
    if (pTrailer)
    {
        return pTrailer->mTrailerCar->mCarSprite;
    }
    else
    {
        return this->mCarSprite;
    }
}

MATCH_FUNC(0x440ac0)
void Car::PutWaterCannonOnRoof()
{
    Object* pObject =
        gObject_5C_6F8F84->NewPhysicsObj(objects::water_cannon_114, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    mCarSprite->AttachSprite(pObject->mSprite, gFix16_6777CC, kWaterCannonOffsetY, kAng180_677326);
}

MATCH_FUNC(0x440b10)
void Car::PutTankCannonOnRoof()
{
    Object* pObject =
        gObject_5C_6F8F84->NewPhysicsObj(objects::tank_cannon_148, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    mCarSprite->AttachSprite(pObject->mSprite, gFix16_6777CC, kTankCannonOffsetY, kAngZero_67791C);
}

MATCH_FUNC(0x440b60)
void Car::PutMachineGunOnRoof()
{
    Object* pObj = gObject_5C_6F8F84->NewPhysicsObj(248, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    mCarSprite->AttachSprite(pObj->mSprite, gFix16_6777CC, kMachineGunOffsetY, kAngZero_67791C);
}

MATCH_FUNC(0x440bb0)
void Car::PutTV_Antenna()
{
    Object* pNewObj =
        gObject_5C_6F8F84->NewPhysicsObj(objects::tv_antenna_149, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    mCarSprite->AttachSprite(pNewObj->mSprite, gFix16_6777CC, kTvAntennaOffsetY, GetRadioTowerAngle());
}

MATCH_FUNC(0x440c10)
bool Car::RotateRoofObjectTowardTarget(Ang16 targetAngle)
{
    AttachedSprite* pFound = NULL;
    Ang16 ang = 0;

    if (IsFireTruck_4118F0())
    {
        pFound = mAttachments.GetSpriteForModel(objects::water_cannon_114);
        ang = pFound->mpSprite->mRotation + kAng180_677326;
    }
    else if (IsTank_411900())
    {
        pFound = mAttachments.GetSpriteForModel(objects::tank_cannon_148);
        ang = pFound->mpSprite->mRotation;
    }
    else if (IsGunJeep_411910())
    {
        pFound = mAttachments.GetSpriteForModel(248);
        ang = pFound->mpSprite->mRotation;
    }

    if (ComputeShortestAngleDelta(ang, targetAngle) <= kRoofTurretAutoRotSpeed_677910)
    {
        return true;
    }

    if (Ang16::IsAngleAhead(&ang, &targetAngle))
    {
        pFound->mOffsetRot += kRoofTurretAutoRotSpeed_677910;
    }
    else
    {
        pFound->mOffsetRot -= kRoofTurretAutoRotSpeed_677910;
    }
    return false;
}

// 9.6f 0x41FA70
static inline Ang16& compound_add_41FA70(Ang16& a, const Ang16& b)
{
    a.rValue += b.rValue;
    a.Normalize();
    return a;
}

// 9.6f 0x41FA90
static inline Ang16& compound_subtract_41FA90(Ang16& a, const Ang16& b)
{
    a.rValue -= b.rValue;
    a.Normalize();
    return a;
}

MATCH_FUNC(0x440d90)
bool Car::HandleRoofTurretRotation(char_type left_on)
{
    // 9.6f joins the three models into one +=/-= pair. Here field_B8 is set in both branches (a
    // single store after the if/else keeps the angle in %di across the second Normalize loop)
    if (IsFireTruck_4118F0())
    {
        AttachedSprite* pSprite = mAttachments.GetSpriteForModel(objects::water_cannon_114);
        if (left_on)
        {
            compound_add_41FA70(pSprite->mOffsetRot, kFireTruckCannonRotSpeed);
            this->mTurretRotated = true;
        }
        else
        {
            compound_subtract_41FA90(pSprite->mOffsetRot, kFireTruckCannonRotSpeed);
            this->mTurretRotated = true;
        }
        return true;
    }

    if (IsTank_411900())
    {
        AttachedSprite* pSprite = mAttachments.GetSpriteForModel(objects::tank_cannon_148);
        if (left_on)
        {
            compound_add_41FA70(pSprite->mOffsetRot, kTankTurretRotSpeed);
            this->mTurretRotated = true;
        }
        else
        {
            compound_subtract_41FA90(pSprite->mOffsetRot, kTankTurretRotSpeed);
            this->mTurretRotated = true;
        }
        return true;
    }

    if (IsGunJeep_411910())
    {
        AttachedSprite* pSprite = mAttachments.GetSpriteForModel(248);
        if (left_on)
        {
            compound_add_41FA70(pSprite->mOffsetRot, kGunJeepTurretRotSpeed);
            this->mTurretRotated = true;
        }
        else
        {
            compound_subtract_41FA90(pSprite->mOffsetRot, kGunJeepTurretRotSpeed);
            this->mTurretRotated = true;
        }
        return true;
    }

    return false;
}

MATCH_FUNC(0x440f90)
void Car::FireCarBomb(char_type instant_bomb)
{
    if (instant_bomb)
    {
        s32 ped_id = gPedRefTable_7F8_703398->GetPedId_420F10(mDriver->get_ped_ref_idx_420B50());
        if (ped_id)
        {
            this->mExploderPedId = ped_id;
            this->mDeathCause = ped_death_cause::bomb_12;
            this->mExploderTimer = 50;
        }
        Car::TriggerExplosion(explosion_type::large_20);
    }
    else
    {
        Object* pNewObject = gObject_5C_6F8F84->NewPhysicsObj(objects::car_bomb_132,
                                                                    gFix16_6777CC,
                                                                    gFix16_6777CC,
                                                                    gFix16_6777CC,
                                                                    kAngZero_67791C);
        Ped* pDriver = this->mDriver;
        if (pDriver)
        {
            pNewObject->SetDamageOwner(pDriver->get_ped_ref_idx_420B50());
        }
        mCarSprite->AttachSprite(pNewObject->mSprite, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    }
}

MATCH_FUNC(0x441030)
void Car::GoToBlockTest(u8 x, u8 y, u8 z, s32 direction)
{
    AllocateAI_421510();
    mAI->SetCar(this);
    mAI->GoToBlock(x, y, z, direction);
}

MATCH_FUNC(0x441080)
void Car::GotoBlock(u8 x, u8 y, u8 z, s32 direction)
{
    AllocateAI_421510();
    mAI->SetCar(this);
    mAI->GoToBlock(x, y, z, direction);
}

// 9.6f 0x41FF90
MATCH_FUNC(0x4410d0)
char_type Car::CountConsecutiveArrowBlocks(Ang16 ang, s8* pOutCount, Fix16 sprite_x, Fix16 sprite_y)
{
    u8 x_add = 0;
    u8 y_add = 0;
    u8 mask1 = 0;
    u8 mask2 = 0;
    s32 angleFace = Ang16::GetAngleFace(ang);
    u8 x = sprite_x.ToInt();
    u8 y = sprite_y.ToInt();
    u8 z = mCarSprite->mZpos.ToInt() - 1;

    switch (angleFace)
    {
        case road_direction::up_1:
            x_add = -1;
            mask1 = block_arrow::green_up_4;
            mask2 = block_arrow::red_up_40;
            break;
        case road_direction::down_2:
            x_add = 1;
            mask1 = block_arrow::green_down_8;
            mask2 = block_arrow::red_down_80;
            break;
        case road_direction::left_4:
            y_add = 1;
            mask2 = block_arrow::red_left_10;
            mask1 = block_arrow::green_left_1;
            break;
        case road_direction::right_3:
            y_add = -1;
            mask1 = block_arrow::green_right_2;
            mask2 = block_arrow::red_right_20;
            break;
        default:
            break;
    }

    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock(x, y, z);
    if (pBlock && ((pBlock->mArrows & mask1) || (pBlock->mArrows & mask2)))
    {
        x += x_add;
        y += y_add;

        u8 count = 0;
        for (pBlock = gMap_0x370_6F6268->GetBlock(x, y, z); pBlock; pBlock = gMap_0x370_6F6268->GetBlock(x, y, z))
        {
            if (!(pBlock->mArrows & mask1) && !(pBlock->mArrows & mask2))
            {
                break;
            }
            count++;
            x += x_add;
            y += y_add;
        }

        // Returns the count ahead; *pOutCount gets the count both ways
        char_type count_ahead = count;
        x = sprite_x.ToInt();
        y = sprite_y.ToInt();
        x_add = -x_add;
        y_add = -y_add;

        for (pBlock = gMap_0x370_6F6268->GetBlock(x, y, z); pBlock; pBlock = gMap_0x370_6F6268->GetBlock(x, y, z))
        {
            if (!(pBlock->mArrows & mask1) && !(pBlock->mArrows & mask2))
            {
                break;
            }
            x += x_add;
            y += y_add;
            count++;
        }
        *pOutCount = count;
        return count_ahead;
    }

    *pOutCount = -1;
    return -1;
}

MATCH_FUNC(0x441330)
Fix16 Car::GetZPos() const
{
    if (mPhysics)
    {
        return mPhysics->ComputeZPosition();
    }
    return mCarSprite->mZpos;
}

MATCH_FUNC(0x441360)
void Car::CountDownToWreck()
{
    if (mWreckCountdown)
    {
        mWreckCountdown--;
        if (mWreckCountdown == 0)
        {
            TurnToWreck();
        }
    }
}

MATCH_FUNC(0x4436A0)
void Car::TurnToWreck()
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
    s32 wreck_idx = pCarInfo->wreck + car_model_enum::WRECK0;
    mCarSprite->mTypeSpriteIdx = gGtx_0x106C_703DD4->get_car_info_5AA3B0(wreck_idx)->sprite;
    mCarSprite->CopyCollisionShapeToVisual();
    mCarSprite->mPaletteType = palette_types_enum::sprites_2;
    mDamagedAreas.m_var = 0;
    PrepareForExplosion();
    ClearHorn_421460();
    SpawnFire();
}

MATCH_FUNC(0x441380)
void Car::StartWreckCountdown()
{
    if (this->mWreckCountdown == 0)
    {
        SetEngineDamaged_421590();
        this->mWreckCountdown = 50;
    }
}

MATCH_FUNC(0x4F7940)
EXPORT Ang16 __stdcall ReturnAngleFromRoadDirection(s32& road_direction)
{
    switch (road_direction)
    {
        case road_direction::up_1:
            return kAng180_6F67EA;
        case road_direction::right_3:
            return kAng90_6F6754;
        case road_direction::down_2:
            return kAngZero_6F6D3C;
        case road_direction::left_4:
            return kAng270_6F6808;
        default:
            return kAngZero_6F6D3C;
    }
}

MATCH_FUNC(0x4413b0)
void Car::UpdateTrainCarriagesOnTrack(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{

    u8 idx = 0;
    Car** pTrainCars = gPublicTransport_181C_6FF1D4->GetCarArrayFromLeadCar(this);
    Train* pTrain = gPublicTransport_181C_6FF1D4->GetTrainFromCar(this);
    if (pTrain)
    {
        pTrainCars = &pTrain->mCarriages[1];
    }

    Fix16 newx;
    Fix16 newy;
    Fix16 newz;

    Fix16 x = xpos;
    Fix16 y = ypos;
    Fix16 z = zpos;

    Fix16 train_speed;
    bool bNotReversing = gPublicTransport_181C_6FF1D4->GetTrainSpeed(this, &train_speed);

    for (Car* pTrainCarIter = *pTrainCars; pTrainCarIter; pTrainCarIter = pTrainCars[idx])
    {

        newx = x;
        newy = y;
        newz = z;

        Ang16 carriage_angle;
        if (bNotReversing)
        {
            s32 track_dir_a = gMap_0x370_6F6268->AdvanceAlongRailwayChecked(&newx, &newy, &newz, kFpTwo_6777D4);
            carriage_angle = ReturnAngleFromRoadDirection(track_dir_a);
        }
        else
        {
            s32 track_dir_b = gMap_0x370_6F6268->AdvanceAlongRailway(&newx, &newy, &newz, kFpTwo_6777D4);
            carriage_angle = ReturnAngleFromRoadDirection(track_dir_b);
        }

        pTrainCarIter->mCarSprite->set_xyz_lazy_420600(newx, newy, newz);
        pTrainCarIter->mCarSprite->set_ang_lazy_420690(carriage_angle);

        // The next carriage follows on from this one
        x = newx;
        y = newy;
        z = newz;

        if (pTrainCarIter->mAttachments.mP18)
        {
            pTrainCarIter->mAttachments.PoolUpdate(pTrainCarIter->mCarSprite);
            pTrainCarIter->mAttachments.PropagateMaxZLayer(pTrainCarIter->mCarSprite, 0);
        }

        pTrainCarIter->ServiceDoorsLightsAndHorn();
        pTrainCarIter->CountDownToWreck();

        idx++;
    }
}

MATCH_FUNC(0x441520)
void Car::UpdateEngineStatus()
{
    CarPhysics* pCarPhysics;
    switch (mEngineStatus)
    {
        case car_engine_status::off_1:
            pCarPhysics = mPhysics;
            if (pCarPhysics)
            {
                if (pCarPhysics->IsAccelerationOrReverseOn())
                {
                    mEngineStatus = car_engine_status::starting_4;
                }
            }
            break;
        case car_engine_status::starting_4:
            HeadlightsOn();
            mEngineStatus = car_engine_status::on_3;
            break;
        case car_engine_status::stopping_2:
            HeadlightsOff();
            mEngineStatus = car_engine_status::off_1;
            break;
        case car_engine_status::destroying_5:
            HeadlightsOff();
            mEngineStatus = car_engine_status::destroyed_6;
            break;
        case car_engine_status::crushed_7:
            mCarSprite->CopyVisualShapeToCollision();
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4415c0)
void Car::UpdateBrakeLights()
{
    CarPhysics* pCarPhysics = this->mPhysics;
    if (pCarPhysics && pCarPhysics->IsFootBrakeOn())
    {
        BrakeLightsOn();
    }
    else
    {
        BrakeLightsOff();
    }
}

MATCH_FUNC(0x441600)
Fix16_Rect Car::GetOrientedHalfExtentsRect() const
{
    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx);
    Fix16 half_w = gPixelsToFix16_6F6850.list[pInfo->w] / kFpTwo_6777D4;
    Fix16 half_h = gPixelsToFix16_6F6850.list[pInfo->h] / kFpTwo_6777D4;

    SwapIf3or4_41FE40(Ang16::GetAngleFace(mCarSprite->mRotation), half_w, half_h);

    Fix16 z = mCarSprite->mZpos;
    return Fix16_Rect(-half_w, half_w, -half_h, half_h, z);
}

MATCH_FUNC(0x4416d0)
void Car::TryHonkHorn(s32 horn_mode)
{
    bool bHornStarted = false;
    if (mHornCooldown > 0)
    {
        mHornCooldown--;
    }

    if (horn_mode != 0)
    {
        if (!this->mHornCooldown)
        {
            if (!IsTrainModel_403BA0() && !IsTank_411900() && (this->mCarId & 7) == 0)
            {
                if (horn_mode == 2)
                {
                    this->mSoundEvent = 1;
                }
                else if (horn_mode == 1)
                {
                    this->mSoundEvent = 2;
                }
                bHornStarted = true;
                this->mHornCooldown = -1;
            }
        }
    }

    if (!this->mHorn && !IsMaxDamage_40F890())
    {
        if (this->mDriver->mPlayer)
        {
            this->mHorn = 45;
        }
        else if (!this->mHornCooldown || bHornStarted)
        {
            if (IsFireTruck_4118F0() || IsCopCar_421790() || IsMediCar_421780() || IsSwatVan_4217A0() || IsTank_411900() || IsFbiCar_411920() || IsTrainModel_403BA0() ||
                (this->mCarId & 3) == 0)
            {
                this->mHorn = 45;
                this->mHornCooldown = -1;
            }
        }
    }
}

MATCH_FUNC(0x4417d0)
void Car::UpdateHorn()
{
    if (mHorn <= 0)
    {
        return;
    }

    if (mHorn != 248)
    {
        mHorn--;
    }
}

MATCH_FUNC(0x4417f0)
void Car::HonkHorn()
{
    mHorn = 45;
}

MATCH_FUNC(0x441800)
void Car::HandleSpecialInput(char_type now_special_pressed)
{
    if (HasEmergencyLights_414F20() && !now_special_pressed && mHorn > 248u)
    {
        ToggleEmergencyLights(); // on/off depending on mLightFlags flags
    }

    if (!now_special_pressed || IsMaxDamage_40F890())
    {
        ClearHorn_421460();
        return;
    }

    if (HasEmergencyLights_414F20())
    {
        mHorn = -1;
    }
    else
    {
        mHorn = -8;
    }
}

MATCH_FUNC(0x4418a0)
void Car::DoDetachTrailer()
{
    if (mpTrailer)
    {
        FlashAndDetachTrailer();
    }
}

MATCH_FUNC(0x4418b0)
void Car::FlashAndDetachTrailer()
{
    FlashHeadlights();
    DetachTrailer();
    AddFlags_421890(car_flag::TrailerDetached);
}

// https://decomp.me/scratch/KU02C
MATCH_FUNC(0x4418d0)
void Car::HandleUserInput(char_type forward_gas_on,
                                    char_type foot_brake_on,
                                    char_type left_on,
                                    char_type right_on,
                                    char_type hand_brake_on,
                                    char_type now_special_pressed,
                                    char_type was_special_pressed,
                                    char_type attack)
{
    // A flag initialised to 0 (not a nested if) gives the original's 'cmp %bl,%al' on the call result
    bool bRotated = false;
    this->mTurretRotated = false;

    if (now_special_pressed && (left_on || right_on))
    {
        // NB: If left is off then right is on
        bRotated = Car::HandleRoofTurretRotation(left_on);
    }

    if (bRotated)
    {
        left_on = 0;
        right_on = 0;
        now_special_pressed = 0;
    }

    if (was_special_pressed)
    {
        Car::HandleSpecialInput(now_special_pressed);
    }

    if (attack)
    {
        Car::DoDetachTrailer();
    }

    // 9.6f: Car::IsAcceleratorJammed_425780 (mFlags & 0x1000 through IsFlagSet_411930), as a bool helper it changes the code
    if ((this->mFlags & car_flag::JammedAccelerator))
    {
        forward_gas_on = 1;
    }

    mPhysics->HandleUserInputs(forward_gas_on, foot_brake_on, left_on, right_on, hand_brake_on);
}

MATCH_FUNC(0x4419e0)
void Car::AllocCarPhysics()
{
    if (!mPhysics)
    {
        mPhysics = gCarPhysicsPool_6FE3E0->Allocate();
    }
}

MATCH_FUNC(0x441a10)
void Car::DeAllocateCarPhysics_441A10()
{
    if (mPhysics)
    {
        gCarPhysicsPool_6FE3E0->DeAllocate(mPhysics);
    }
    mPhysics = NULL;
}

MATCH_FUNC(0x441a40)
bool Car::AreAllDoorsClosed()
{
    u32 i = 0;
    do
    {
        if (mDoors[i].IsStateActive_421360())
        {
            return false;
        }
        i++;
    } while (i < GTA2_COUNTOF(mDoors));
    return true;
}

MATCH_FUNC(0x441a70)
void Car::InitDoors_441A70()
{
    const u8* pDoorCount = gGtx_0x106C_703DD4->get_car_door_data(mCarInfoIdx);
    u8 i;
    for (i = 0; i < *pDoorCount; i++)
    {
        mDoors[i].Init(i);
        mDoors[i].set_ped_421380(NULL);
    }

    for (i = *pDoorCount; i < 4; i++)
    {
        mDoors[i].ResetState_421340();
    }
}

MATCH_FUNC(0x441b00)
void Car::ServiceDoors()
{
    Car_Door* pDoor = mDoors;
    for (s32 i = 0; i < 4; i++)
    {
        pDoor->Service(&mDamagedAreas.m_var);
        pDoor++;
    }
}

MATCH_FUNC(0x441b20)
void Car::UpdateTaxiLight()
{
    if (mEngineStatus != car_engine_status::on_3 || mPassengersList.mpFirstPed)
    {
        mDamagedAreas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }
    else
    {
        mDamagedAreas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }
}

MATCH_FUNC(0x441b50)
void Car::UpdateRoofLightFlasher()
{
    char_type flashOffThreshold;
    char_type flashCycleAmplitude;

    if (IsFbiCar_411920())
    {
        flashOffThreshold = 0; // FBI cars flash faster
        flashCycleAmplitude = 8; // FBI flash amplitude
    }
    else
    {
        flashOffThreshold = 5; // normal cars flash slower
        flashCycleAmplitude = 15; // normal flash amplitude
    }

    // positive side of flash cycle
    if (mFlashPhaseCounter > 0)
    {
        mFlashPhaseCounter--;
        if (mFlashPhaseCounter == flashOffThreshold)
        {
            ResetBottomLeftRoofLight();
            ResetTopLeftRoofLight();
        }

        if (!mFlashPhaseCounter)
        {
            mFlashPhaseCounter = -flashCycleAmplitude;
            UpdateRoofLights();
            UpdateTopRightRoofLight();
        }
    }
    // negative side of flash cycle
    else
    {
        mFlashPhaseCounter++;
        if (mFlashPhaseCounter == -flashOffThreshold)
        {
            ResetRoofLights();
            ResetTopRightRoofLight();
        }

        if (!mFlashPhaseCounter)
        {
            mFlashPhaseCounter = flashCycleAmplitude;
            UpdateBottomLeftRoofLight();
            UpdateTopLeftRoofLight();
        }
    }
}

MATCH_FUNC(0x441c00)
void Car::UpdatePopupHeadlights()
{
    if (!(gpRng_67AB34->get_cur_rng_41CFE0() % 3u))
    {
        if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor2_12);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor3_13);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor4_14);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
        }

        if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopLeftDoor1_28))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopLeftDoor2_29))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopLeftDoor3_30))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopLeftDoor4_31))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
        }
    }
}

MATCH_FUNC(0x441d40)
void Car::UpdateFbiPopupSiren()
{
    if (!(gpRng_67AB34->get_cur_rng_41CFE0() % 3u))
    {
        if (this->mBfA4.bEmergencyLights)
        {
            if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11))
            {
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
                this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            }
            else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12))
            {
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
                this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            }
            else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13))
            {
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
                this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor4_14);
            }
            else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
            {
                this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
                this->mDamagedAreas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
            }
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor3_13);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor2_12);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            this->mDamagedAreas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        }
        else if (mDamagedAreas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11))
        {
            this->mDamagedAreas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        }
    }
}

MATCH_FUNC(0x441e70)
void Car::UpdateLights()
{
    if (mBfA4.bEmergencyLights)
    {
        Car::UpdateRoofLightFlasher();
    }
    else if (mBfA4.bLightFlashing)
    {
        mFlashPhaseCounter--;
        if (mFlashPhaseCounter == 0)
        {
            mFlashCount--;
            if (mFlashCount == 0)
            {
                Car::StopLightFlashing();
            }
            else
            {
                SetHeadlightDeltas_425590();
                SetBrakeLightDeltas_4213D0();
                mFlashPhaseCounter = 12;
            }
        }
        else
        {
            if (mFlashPhaseCounter == 6)
            {
                ClearHeadlightDeltas_425650();
                ClearBrakeLightDeltas_421430();
            }
        }
    }
    else if (mBfA4.bHeadlightFlash)
    {
        mFlashPhaseCounter--;
        if (mFlashPhaseCounter == 0)
        {
            if (mBfA4.bHeadlights)
            {
                SetHeadlightDeltas_425590();
            }
            else
            {
                Car::ClearHeadlightDeltas_447360();
            }
            mBfA4.bHeadlightFlash = false;
        }
    }

    if (HasPopupHeadlights_421700())
    {
        Car::UpdatePopupHeadlights();
    }
    if (HasTaxiLight_4216C0())
    {
        Car::UpdateTaxiLight();
    }
    if (IsFbiCar_411920())
    {
        Car::UpdateFbiPopupSiren();
    }
}

MATCH_FUNC(0x442170)
bool Car::IsCarInConditionsToDespawn() const
{
    Ped* pDriver = mDriver;
    if ((!pDriver || !pDriver->mPlayer) && !IsVisibleToAnyPlayer())
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x442190)
void Car::ServicePhysics()
{
    if (mPhysics->ProcessCarPhysicsStateMachine())
    {
        if (!this->mDriver)
        {
            DeAllocateCarPhysics_43BD00();
        }
    }
}

MATCH_FUNC(0x4421b0)
bool Car::IsMissionRelated()
{
    if (mCarKind != car_kind::mission_car_8 && mPedType != ped_type::script_created_5 && !mPassengersList.HasScriptCreatedPed())
    {
        return false;
    }

    return true;
}

MATCH_FUNC(0x442200)
bool Car::CanDespawn()
{
    if (mCarKind == car_kind::parked_car_9)
    {
        return IsMaxDamage_40F890() ? true : false;
    }

    if (!IsTrainModel_403BA0() && !gGame_0x40_67E008->IsCarInAnyPlayerHistory(this) && !Car::IsMissionRelated())
    {
        if (mPedType != ped_type::special_ped_4 && (mPedType != ped_type::dummy_with_occupation_6 || !mDriver))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x442310)
void Car::ManageDespawning()
{
    bool bOnScreenForAnyPlayer = false;
    bool bIsNotSeen = true;

    if (bSkip_recycling_67D575 && !IsMediCar_421780() && !IsCopCar_421790() && !IsFbiCar_411920())
    {
        return;
    }

    if (IsTrainModel_403BA0())
    {
        if (IsOnScreenForAnyPlayer())
        {
            mLastSeenTimer = 0;
            bIsNotSeen = false;
        }
    }
    else
    {
        for (u8 i = 0; i < 6; i++)
        {
            if (gGame_0x40_67E008->GetPlayer_4219E0(i))
            {
                if (gGame_0x40_67E008->IsSpriteInPlayerBounds(mCarSprite, i, kFpFive_6778D0))
                {
                    bOnScreenForAnyPlayer = true;
                }
            }
        }

        if (bOnScreenForAnyPlayer)
        {
            mLastSeenTimer = 0;
            bIsNotSeen = false;
        }
    }

    if (bIsNotSeen)
    {
        mLastSeenTimer++;
        if (mLastSeenTimer > 9999)
        {
            mLastSeenTimer = 9999;
        }
    }

    if (((IsCopCar_421790() || IsFireTruck_4118F0() || IsSwatVan_4217A0() || IsTank_411900() || IsGunJeep_411910() ||
          IsFbiCar_411920()) &&
         mLastSeenTimer == 300) ||
        mLastSeenTimer >= 130)
    {
        if (CanDespawn() && !IsMarkedForDespawn_4214B0() && mDespawnStatus != car_despawn_status::despawning_5)
        {
            MarkForDespawn_421470();
        }
    }
}

MATCH_FUNC(0x4424c0)
bool Car::UpdateCarDespawnStatus()
{
    switch (mDespawnStatus)
    {
        case car_despawn_status::despawn_soon_3:
            mDespawnStatus = car_despawn_status::despawning_5; // Despawn now
            return false;

        case car_despawn_status::marked_for_despawn_4:
            mDespawnStatus = car_despawn_status::despawn_pending_2;
            return false;

        case car_despawn_status::despawn_pending_2: // Mostly NPC cars
            if (!IsCarInConditionsToDespawn())
            {
                return false;
            }
            mDespawnStatus = car_despawn_status::despawning_5; // Despawn now
            return false;

        case car_despawn_status::despawning_5: // Despawn now
            mDespawnStatus = car_despawn_status::despawned_6; // Despawned
            return true;

        default:
            return false;
    }
}

// 9.6f 0x424220
// https://decomp.me/scratch/vhWKK
MATCH_FUNC(0x442520)
Ang16 Car::GetRadioTowerAngle() const
{
    Fix16_Point xy;
    xy = gCarManager_6C_677930->mTvVanDir - mCarSprite->get_x_y();
    return xy.atan2_40F790() - mCarSprite->mRotation;
}


EXTERN_GLOBAL(Fix16, kFPZero_6691B0);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16FullCircle_66A8E4, Fix16(0x18F60, 0), 0x66A8E4);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16HalfCircle_6691EC, Fix16(0xC7B0, 0), 0x6691EC);
inline Fix16 __stdcall WrapAngle_40E790(Fix16& angle) // 9.6f inlined func
{
    Fix16 result = angle;
    for (; result < kFPZero_6691B0; result += kAngFix16FullCircle_66A8E4)
    {
        ;
    }
    for (; result >= kAngFix16FullCircle_66A8E4; result -= kAngFix16FullCircle_66A8E4)
    {
        ;
    }
    return result;
}

// 9.6f 0x40ECB0
// https://decomp.me/scratch/GnD4O
MATCH_FUNC(0x405CE0)
EXPORT void __stdcall SmoothApproachAngle(Fix16& target_angle, Fix16& angle_speed, Fix16& angle, Fix16& accel, Fix16& max_speed)
{
    if (target_angle - angle > kAngFix16HalfCircle_6691EC)
    {
        angle += kAngFix16FullCircle_66A8E4;
    }
    else
    {
        if (target_angle - angle < -kAngFix16HalfCircle_6691EC)
        {
            angle -= kAngFix16FullCircle_66A8E4;
        }
    }

    Fix16 angle_diff = target_angle - angle;

    if (angle_diff > kFPZero_6691B0)
    {
        if (angle_speed >= kFPZero_6691B0)
        {
            if (angle_speed + accel <= angle_diff)
            {
                angle_speed += accel;
                if (angle_speed > max_speed)
                {
                    angle_speed = max_speed;
                }
            }
            else
            {
                angle_speed = angle_diff;
            }
        }
        else
        {
            angle_speed = kFPZero_6691B0;
        }
    }
    else
    {
        if (angle_diff >= kFPZero_6691B0 || angle_speed > kFPZero_6691B0)
        {
            angle_speed = kFPZero_6691B0;
        }
        else
        {
            if (angle_speed - accel >= angle_diff)
            {
                angle_speed -= accel;
                if (angle_speed < -max_speed)
                {
                    angle_speed = -max_speed;
                }
            }
            else
            {
                angle_speed = angle_diff;
            }
        }
    }

    angle = WrapAngle_40E790(angle + angle_speed);
}

// 9.6f 0x424280
MATCH_FUNC(0x4425d0)
void Car::ManageTVAntenna()
{
    // Fix16 locals declared at the top (not in the inner block) give the original stack slot layout
    Ang16 towerAng;
    Fix16 zero;
    Fix16 spriteAngFp;
    AttachedSprite* pSprite = mAttachments.GetSpriteForModel(objects::tv_antenna_149);
    if (pSprite)
    {
        towerAng = GetRadioTowerAngle();
        if (pSprite->mOffsetRot != towerAng)
        {
            zero = gFix16_6777CC;
            // A reference makes VC6 reload the rotation instead of reusing the compared value
            Ang16& rot = pSprite->mOffsetRot;
            spriteAngFp = Ang16::Ang16_to_Fix16(rot);
            SmoothApproachAngle(Ang16::Ang16_to_Fix16(towerAng), zero, spriteAngFp, kAngFix16OneDegree_677920, kAngFix16OneDegree_677920);
            rot = Ang16::Fix16_To_Ang16_40F540(spriteAngFp);
        }
    }
}

MATCH_FUNC(0x4426d0)
void Car::ServiceDoorsLightsAndHorn()
{
    ServiceDoors();
    UpdateEngineStatus();

    if (!IsMaxDamage_40F890())
    {
        UpdateBrakeLights();
        UpdateLights();
    }

    UpdateHorn();

    if (IsTvVan_4217E0() && IsEngineOn_4118C0())
    {
        ManageTVAntenna();
    }
}

MATCH_FUNC(0x442760)
void Car::DetachTrailer()
{
    Trailer* pTrailer = mpTrailer;
    gCar_BC_Pool_67792C->UpdateNextPrev(mpTrailer->mTrailerCar);
    mpTrailer->mTrailerCar->mpTrailer = NULL;
    mpTrailer->mTruckCab->mpTrailer = NULL;
    gTrailerPool_66AC80->Remove_425580(pTrailer);
}

MATCH_FUNC(0x4427a0)
void Car::AttachTrailer(Car* pTrailerCar)
{
    mpTrailer = gTrailerPool_66AC80->mPool.Allocate();
    mpTrailer->SetTruckCabAndTrailerCar(this, pTrailerCar);

    Car* pLast = NULL;
    for (Car* pIter = gCar_BC_Pool_67792C->mPool.mpPrev; pIter; pIter = pIter->mpNext)
    {
        if (pIter == pTrailerCar)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
            }
            else
            {
                gCar_BC_Pool_67792C->mPool.mpPrev = pIter->mpNext;
            }
            pIter->mpNext = NULL;
            return;
        }
        pLast = pIter;
    }
}

// 9.6f 0x4262E0
MATCH_FUNC(0x442810)
void Car::TryHitchTrailer()
{

    Fix16_Point hitch_this;
    Fix16_Point hitch_car;
    Fix16_Point offset;
    Fix16_Point hitch_point; // the original sets EH state 3 up front: 4 points constructed

    mCarSprite->SetDrawLayer_40F7B0(15);

    if (!HasSpriteZoom() && !IsMaxDamage_40F890() && !IsMarkedForDespawn_4214B0() && !IsDespawning_4215B0())
    {
        gTrailerHitchCandidates_67727C.PruneNonCollidingSprites(mCarSprite);
        Sprite* pHitchSprite =
            gTrailerHitchCandidates_67727C.FindClosestSprite(this->mCarSprite->mXy.x, this->mCarSprite->mXy.y);
        if (!pHitchSprite)
        {
            this->mFlags &= static_cast<u16>(~car_flag::TrailerDetached);
        }
        else
        {
            if (mCarSprite->CheckSlopeTopInFootprint() || gTrailerHitchCandidates_67727C.AnySpriteHasSlopeTopInFootprint())
            {
                this->mCarSprite->SetDrawLayer_40F7B0(8);
                gTrailerHitchCandidates_67727C.PropagateMaxZLayer(this->mCarSprite, 1);
            }

            Car* pCar = pHitchSprite->AsCar_40FEB0();
            hitch_car = pCar->GetHitchPoint();
            hitch_this = GetHitchPoint();
            offset = hitch_this - hitch_car;
            // Declared, then assigned: keeps the length in eax (Abs results reloaded from their temp)
            Fix16 hitch_dist;
            hitch_dist = offset.GetLength_41E260();

            Fix16 z_delta = Fix16::Abs(pHitchSprite->mZpos - mCarSprite->mZpos);

            if (hitch_dist < kHitchPullDist && z_delta < kHitchPullDist)
            {
                if ((this->mFlags & 1) == 0)
                {
                    if (hitch_dist < kHitchAttachDist)
                    {
                        AttachTrailer(pCar);
                        FlashHeadlights();
                    }
                    else
                    {
                        pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
                        if (!pCar->mPhysics->IsDamageWindowOpen_421130())
                        {
                            hitch_point = pCar->mCarSprite->get_x_y_443580().AddInl_40AC50(offset.NormalizeSafe().MultiplyInl_438FE0(kFpOne64th_677888));
                            {
                                s32 rotation_mode = 1;
                                pCar->mPhysics->SetVelocityTowardTarget(
                                    hitch_point.x,
                                    hitch_point.y,
                                    Ang16::Ang16_to_Fix16(pCar->mPhysics->get_theta_40F820()),
                                    &rotation_mode);
                            }
                        }
                    }
                }
            }
            else
            {
                this->mFlags &= ~1u;
            }
        }
    }
    gTrailerHitchCandidates_67727C.ClearList();
}

MATCH_FUNC(0x442d10)
void Car::LightUpdate()
{
    AttachedSprite* pAttached = mAttachments.GetSpriteForModel(objects::light_165);
    if (pAttached)
    {
        Sprite* pSprite = pAttached->mpSprite;
        Object* pObject = pSprite->AsObject2C_40FEC0();
        if (gMap_0x370_6F6268->CheckColumnHasSolidAbove(pSprite->mXy.x, pSprite->mXy.y, pSprite->mZpos))
        {
            pObject->TurnOffLight();
        }
        else
        {
            pObject->UpdateLight();
        }
    }
}

// Not in 9.6f (no pair for TrainUpdate). high_z calls the out-of-line
// Fix16::Add (operator+) as the original does; all the arguments are
// evaluated before any store, which gives the original's schedule.
static inline void SetPrism(Fix16_Rect& rect, Fix16 left, Fix16 right, Fix16 top, Fix16 bottom, const Fix16 z)
{
    rect.mLeft = left;
    rect.mRight = right;
    rect.mTop = top;
    rect.mBottom = bottom;
    rect.mLowZ = z - kCollisionPrismHalfHeight_6771E4;
    rect.mHighZ = z.Add(kCollisionPrismHalfHeight_6771E4);
}

MATCH_FUNC(0x442d70)
bool Car::TrainUpdate()
{
    u8 train_car_idx = 0;
    Fix16 player_x;
    Fix16 player_y;
    Fix16 player_z;
    if (GetCarLinearSpeed() > gFix16_6777CC)
    {
        gGame_0x40_67E008->mLocalPlayer->get_pos(&player_x, &player_y, &player_z);

        if (Fix16::Abs_negate_out_of_line(mCarSprite->mXy.x - player_x) >= Fix16(10) ||
            Fix16::Abs(mCarSprite->mXy.y - player_y) >= Fix16(10))
        {
            mHornCooldown = 0;
        }
        else
        {
            Fix16_Rect rect;
            switch (Ang16::GetAngleFace(mCarSprite->mRotation))
            {
                case road_direction::down_2:
                {
                    Fix16 y = mCarSprite->mXy.y;
                    Fix16 z = mCarSprite->mZpos;
                    Fix16 x = mCarSprite->mXy.x;
                    SetPrism(rect, x - Fix16(1), x + Fix16(1), y, y + Fix16(10), z);
                    break;
                }

                case road_direction::up_1:
                {
                    Fix16 z = mCarSprite->mZpos;
                    Fix16 y = mCarSprite->mXy.y;
                    Fix16 x = mCarSprite->mXy.x;
                    SetPrism(rect, x - Fix16(1), x + Fix16(1), y - Fix16(10), y, z);
                    break;
                }

                case road_direction::right_3:
                {
                    Fix16 y = mCarSprite->mXy.y;
                    Fix16 z = mCarSprite->mZpos;
                    Fix16 x = mCarSprite->mXy.x;
                    SetPrism(rect, x, x + Fix16(10), y - Fix16(1), y + Fix16(1), z);
                    break;
                }

                case road_direction::left_4:
                {
                    // Written out: here the original inlines operator+ for high_z
                    Fix16 z = mCarSprite->mZpos;
                    rect.SetRect_41E350(mCarSprite->mXy.x - Fix16(10),
                                        mCarSprite->mXy.x,
                                        mCarSprite->mXy.y - Fix16(1),
                                        mCarSprite->mXy.y + Fix16(1));
                    rect.mLowZ = z - kCollisionPrismHalfHeight_6771E4;
                    rect.mHighZ = z + kCollisionPrismHalfHeight_6771E4;
                    break;
                }

                default:
                    FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\car.cpp", 5458, 0);
            }

            if (gSpriteGrid_1_679208->CheckRectForCollisions(&rect, false, sprite_types_enum::none_0, this->mCarSprite))
            {
                TryHonkHorn(0);
            }
        }
    }

    Car** pTrainCarsArray = gPublicTransport_181C_6FF1D4->GetCarArrayFromLeadCar(this);

    if (mDespawnStatus != car_despawn_status::despawning_5)
    {
        gSpriteGrid_1_679208->RemoveFromRegionBuckets(mCarSprite);
    }

    for (Car* pTrainCarIter_ = *pTrainCarsArray; pTrainCarIter_; pTrainCarIter_ = pTrainCarsArray[train_car_idx])
    {
        if (pTrainCarIter_->mDespawnStatus != car_despawn_status::despawning_5)
        {
            gSpriteGrid_1_679208->RemoveFromRegionBuckets(pTrainCarIter_->mCarSprite);
        }
        ++train_car_idx;
    }

    if (UpdateCarDespawnStatus())
    {
        return true;
    }

    if (mPhysics)
    {
        if (!mAI)
        {
            mAI = gCarAI_78_Pool_677CF8->Allocate();
            mAI->SetCar(this);
        }

        if (mAI)
        {
            mAI->UpdateTrainMovement();
        }
        ServicePhysics();
    }

    UpdateTrainCarriagesOnTrack(this->mCarSprite->mXy.x,
                                       this->mCarSprite->mXy.y,
                                       this->mCarSprite->mZpos);
    ServiceDoorsLightsAndHorn();
    ManageDespawning();
    CountDownToWreck();

    if (this->mAttachments.mP18)
    {
        mAttachments.PoolUpdate(mCarSprite);
        mAttachments.PropagateMaxZLayer(mCarSprite, false);
    }

    if (this->mDespawnStatus != car_despawn_status::despawning_5)
    {
        gSpriteGrid_1_679208->AddToRegionBuckets(mCarSprite);
    }

    train_car_idx = 0;
    for (Car* pTrainCarIter = *pTrainCarsArray; pTrainCarIter; pTrainCarIter = pTrainCarsArray[train_car_idx])
    {
        if (pTrainCarIter->mDespawnStatus != car_despawn_status::despawning_5)
        {
            gSpriteGrid_1_679208->AddToRegionBuckets(pTrainCarIter->mCarSprite);
        }
        ++train_car_idx;
    }

    LightUpdate();
    return false;
}

MATCH_FUNC(0x443130)
bool Car::TrailerUpdate()
{
    const s32 state = mpTrailer->Update();
    switch (state)
    {
        case k_trailer_update_ok:
            return false;
        case k_trailer_update_despawned:
            gTrailerPool_66AC80->mPool.DeAllocate(mpTrailer);
            return true;
        case k_trailer_update_detach:
            Car::DetachTrailer();
            break;
    }
    return false;
}

MATCH_FUNC(0x443170)
bool Car::PoolUpdate()
{
    if (!this->mLastSeenTimer)
    {
        gCarManager_6C_677930->mVisibleCarsCount++;
    }

    UpdateExploderTimer();

    if (this->mpTrailer)
    {
        return TrailerUpdate();
    }

    if (IsTrainModel_403BA0())
    {
        return TrainUpdate();
    }

    if (this->mDespawnStatus != car_despawn_status::despawning_5)
    {
        gSpriteGrid_1_679208->RemoveFromRegionBuckets(this->mCarSprite);
    }

    if (UpdateCarDespawnStatus())
    {
        return true;
    }

    if (this->mPhysics)
    {
        CarAI* pAi = this->mAI;
        if (pAi)
        {
            Ped* pDriver = this->mDriver;
            if (pDriver)
            {
                if (pDriver->GetPedType_420B70() == ped_type::player_2 && this->mCarInfoIdx == car_model_enum::TRAINCAB || pDriver->GetPedType_420B70() != ped_type::player_2)
                {
                    pAi->AI_Service();
                }
            }
            mAI->mCarInCollision = NULL;
            mAI->mBf24.bCollidedWithCar = false;
        }
        ServicePhysics();
    }

    if (IsCrushing_414F70())
    {
        sCrushedCarSprites.PropagateMaxZLayer(this->mCarSprite, 1);
        sCrushedCarSprites.ClearList();
    }

    if (this->mAttachments.mP18)
    {
        this->mAttachments.PoolUpdate(this->mCarSprite);
        this->mAttachments.PropagateMaxZLayer(this->mCarSprite, 0);
    }

    if (this->mPhysics)
    {
        ManageDrowning();
    }

    if (!this->mpTrailer && IsTruckCab_421620())
    {
        TryHitchTrailer();
    }

    ServiceDoorsLightsAndHorn();
    ManageDespawning();
    CountDownToWreck();

    if (mDespawnStatus != car_despawn_status::despawning_5)
    {
        if (HasTaxiLight_4216C0())
        {
            if (mDriver)
            {
                if (mEngineStatus == car_engine_status::on_3 && !mPassengersList.mpFirstPed && mDamageLevel < 3u)
                {
                    gTaxi_4_704130->PushTaxi(this);
                }
            }
        }
        gSpriteGrid_1_679208->AddToRegionBuckets(mCarSprite);
    }
    return false;
}

MATCH_FUNC(0x443330)
void Car::UpdateOnCraneHook()
{
    Car::ServiceDoorsLightsAndHorn();
    if (mAttachments.mP18)
    {
        mAttachments.PoolUpdate(mCarSprite);
        mAttachments.PropagateMaxZLayer(mCarSprite, false);
    }
    Car::CountDownToWreck();
}

MATCH_FUNC(0x443360)
bool Car::UpdateAttachedToSprite(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot)
{
    gSpriteGrid_3_679210->Remove(mCarSprite);

    Fix16 sprite_x;
    Fix16 sprite_y;

    if (x != gFix16_6777CC || y != gFix16_6777CC)
    {
        sprite_x = x;
        sprite_y = y;
        Ang16::RotateVector_41FC90(sprite_x, sprite_y, pSprite->mRotation);

        sprite_x = pSprite->mXy.x + sprite_x;
        sprite_y = pSprite->mXy.y + sprite_y;
    }
    else
    {
        sprite_x = pSprite->mXy.x;
        sprite_y = pSprite->mXy.y;
    }

    mCarSprite->set_xyz_lazy_420600(sprite_x, sprite_y, pSprite->mZpos);
    Ang16 new_rotation = pSprite->mRotation;
    new_rotation += rot;
    mCarSprite->set_ang_lazy_420690(new_rotation);

    switch (pSprite->mSpriteTypeEnum)
    {
        case sprite_types_enum::car_2:
            mCarSprite->SetDrawLayer_40F7B0(18);
            break;

        case sprite_types_enum::object_1:
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::map_obj_5:
            mCarSprite->SetDrawLayer_40F7B0(17);
            break;

        default:
            break;
    }

    ServiceDoorsLightsAndHorn();
    CountDownToWreck();

    if (mAttachments.mP18)
    {
        mAttachments.PoolUpdate(mCarSprite);
        mAttachments.PropagateMaxZLayer(mCarSprite, false);
    }

    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(mCarSprite);
    return false;
}

MATCH_FUNC(0x4435a0)
void Car::InitDoors_4435A0()
{
    InitDoors_441A70();
}

MATCH_FUNC(0x4435b0)
s32 Car::GetAttachedDrawLayer() const
{
    if (IsFlagSet_411930(car_flag::MiniCar))
    {
        return 14;
    }

    if (IsFlagSet_411930(car_flag::Train))
    {
        return 20;
    }

    if (!gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx)->IsHeavy_41FF00())
    {
        return 17;
    }
    return 22;
}

MATCH_FUNC(0x4435f0)
void Car::UpdateDrawLayer()
{
    if (IsTrailerInfo_421640())
    {
        this->mCarSprite->SetDrawLayer_40F7B0(16);
    }
    else
    {
        if (IsFlagSet_411930(car_flag::MiniCar))
        {
            this->mCarSprite->SetDrawLayer_40F7B0(13);
        }
        else if (IsFlagSet_411930(car_flag::Train))
        {
            this->mCarSprite->SetDrawLayer_40F7B0(19);
        }
        else if (gGtx_0x106C_703DD4->get_car_info_5AA3B0(mCarInfoIdx)->IsHeavy_41FF00())
        {
            this->mCarSprite->SetDrawLayer_40F7B0(21);
        }
        else
        {
            this->mCarSprite->SetDrawLayer_40F7B0(15);
        }
    }
}

// 9.6f 0x426580
MATCH_FUNC(0x443710)
void Car::ApplyExplosionImpulse(Fix16_Point* xy)
{
    Fix16_Point hitch_point;
    Fix16_Point impulse_dir;
    Fix16_Point impulse_pos;

    if (!IsDeactivated_4214D0())
    {
        SetupCarPhysicsAndSpriteBinding_43BCA0();

        hitch_point = mCarSprite->get_x_y();
        hitch_point.x += kFpQuarter_677208 * gCarManager_6C_677930->mExplosionOffsetCycle - kFpHalf_6772D0;
        hitch_point.y += kFpQuarter_677208 * gCarManager_6C_677930->mExplosionOffsetCycle - kFpHalf_6772D0;

        impulse_dir = (hitch_point - *xy);
        // vecLen's scope has to end before the ApplyImpactForcesAndDamage call: its result temp
        // reuses vecLen's slot (the dead xy param slot), in 9.6f too. Hence the goto: an inline
        // helper returning bool runs out of inline budget for GetLength_41E260, and a bool
        // flag tested twice isn't merged by VC6.
        {
            Fix16 vecLen = impulse_dir.GetLength_41E260();
            if (vecLen == gFix16_6777CC)
            {
                goto done;
            }
            vecLen = vecLen * 4;
            impulse_pos = impulse_dir.NormalizeSafe().DivideInl_442CB0(vecLen);
        }
        mPhysics->SetCurrentCarInfoAndModelPhysics();
        mPhysics->ApplyImpactForcesAndDamage(hitch_point, impulse_pos, 10);
    }

done:
    gCarManager_6C_677930->mExplosionOffsetCycle++;
    if (gCarManager_6C_677930->mExplosionOffsetCycle > 4)
    {
        gCarManager_6C_677930->mExplosionOffsetCycle = 0;
    }
}

MATCH_FUNC(0x443A50)
s32 __stdcall Car::GetCarWeaponCost(s32 weapon_kind)
{
    if (gCarManager_6C_677930->mDoFreeShopping)
    {
        return 0;
    }

    switch (weapon_kind)
    {
        case weapon_type::car_bomb:
            return 5000;
        case weapon_type::oil_stain:
            return 10000;
        case weapon_type::car_smg:
            return 25000;
        case weapon_type::car_mines:
            return 50000;
        default:
            return 0;
    }
}

MATCH_FUNC(0x4438C0)
void Car::BuyCarWeapon(s32 weapon_kind)
{
    u8 ammo_capacity;
    Player* pPlayer = GetDriverPlayer_421870();
    Weapon* pWeapon = pPlayer->mWeapons[weapon_kind];
    if (pWeapon && pWeapon->is_max_capacity())
    {
        // Ammo full
        if (pPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mBrief.SetHudBrief_5D4400(1, "arig");
        }
    }
    else
    {
        s32 car_weapon_cost = Car::GetCarWeaponCost(weapon_kind);
        if (bStartNetworkGame_7081F0)
        {
            // Cheaper in multiplayer
            car_weapon_cost /= 10;
        }

        if (car_weapon_cost <= pPlayer->GetScore_421980())
        {
            if (pPlayer->IsUser_41DC70())
            {
                gHud_2B00_706620->mBrief.SetHudBrief_5D3F10(1, "bdone", car_weapon_cost);
            }

            pPlayer->AddCash_421990(-car_weapon_cost);
            if (gWeapon_8_707018->get_max_ammo_capacity(weapon_kind) < 10u)
            {
                ammo_capacity = gWeapon_8_707018->get_max_ammo_capacity(weapon_kind);
            }
            else
            {
                ammo_capacity = 10;
            }
            gWeapon_8_707018->allocate_5E3D50(weapon_kind, ammo_capacity, this);
            switch (weapon_kind)
            {
                case weapon_type::car_bomb:
                    this->SetShopEvent_421950(car_shop_event::car_bomb_4);
                    break;
                case weapon_type::car_mines:
                    this->SetShopEvent_421950(car_shop_event::car_mines_5);
                    break;
                case weapon_type::oil_stain:
                    this->SetShopEvent_421950(car_shop_event::oil_stain_6);
                    break;
                case weapon_type::car_smg:
                    this->SetShopEvent_421950(car_shop_event::car_smg_7);
                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\car.cpp", 6107, 0);
            }
        }
        else
        {
            // Can't afford weapon
            Car::ShowCantAffordMessage(pPlayer, car_weapon_cost);
            this->SetShopEvent_421950(car_shop_event::cant_afford_8);
        }
    }
}

MATCH_FUNC(0x443AB0)
void __stdcall Car::ShowCantAffordMessage(Player* pPlayer, s32 weapon_cost)
{
    if (pPlayer->IsUser_41DC70())
    {
        gHud_2B00_706620->mBrief.SetHudBrief_5D3F10(1, "nspraya", weapon_cost);
    }
}

MATCH_FUNC(0x443ae0)
void Car::ResprayOrChangePlates(u8 remap)
{
    Player* pPlayer = GetDriverPlayer_421870();
    const s32 cost = gCarManager_6C_677930->mDoFreeShopping ? 0 : 5000;
    if (cost <= pPlayer->GetScore_421980())
    {
        if (pPlayer->IsUser_41DC70())
        {
            if (remap == car_shop_type::change_plates_253) // clean plates only
            {
                gHud_2B00_706620->mBrief.SetHudBrief_5D3F10(1, "cdone", cost);
            }
            else
            {
                gHud_2B00_706620->mBrief.SetHudBrief_5D3F10(1, "sdone", cost);
            }
        }

        if (remap != car_shop_type::change_plates_253) // respray
        {
            SetCarRemap(remap);
            mAttachments.ClearGangIconSprite();
            this->SetShopEvent_421950(car_shop_event::respray_1);
        }
        else
        {
            this->SetShopEvent_421950(car_shop_event::change_plates_2);
        }

        pPlayer->AddCash_421990(-cost);
        this->mDriver->ClearWantedPoints_420B80();
        RemoveAllDamage();
    }
    else
    {
        ShowCantAffordMessage(pPlayer, cost);
        this->SetShopEvent_421950(car_shop_event::cant_afford_8);
    }
}

MATCH_FUNC(0x443bd0)
void Car::ResprayOrCleanPlates(u8 remap)
{
    if (AllowResprayOrPlates())
    {
        if (IsNotCurrentRemapOfCarAndTrailerCar(remap))
        {
            ResprayOrChangePlates(remap);
        }
        else
        {
            ResprayOrChangePlates(car_shop_type::change_plates_253);
        }
    }
    else if (GetDriverPlayer_421870()->IsUser_41DC70())
    {
        gHud_2B00_706620->mBrief.SetHudBrief_5D4400(1, "nespray");
    }
}

MATCH_FUNC(0x443c40)
void Car::HandleShops(Object* pObj)
{
    if (HasPlayerDriver_4215C0() && pObj->mModel == objects::car_shop_130)
    {
        const u8 idx = pObj->GetPedRefIdx_420FF0();
        if (idx >= car_shop_type::car_smg_250 && (idx <= car_shop_type::mines_252 || idx == car_shop_type::car_bomb_254))
        {
            Car::BuyCarWeapon(GetDriverPlayer_421870()->ObjectTypeToWeaponType(idx));
        }
        else
        {
            Car::ResprayOrCleanPlates(idx);
        }
    }
}

MATCH_FUNC(0x443d00)
void Car::SetPosition(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    gSpriteGrid_1_679208->RemoveFromRegionBuckets(mCarSprite);
    mCarSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
    if (mPhysics)
    {
        mPhysics->SetSprite(mCarSprite);
    }
    gSpriteGrid_1_679208->AddToRegionBuckets(mCarSprite);
}

MATCH_FUNC(0x443d70)
void Car::IncrementCarStats(s32 car_kind)
{
    IncrementAllocatedCarType(car_kind);

    if (IsTrailerCab_41E460())
    {
        mpTrailer->mTrailerCar->IncrementAllocatedCarType(car_kind);
    }
}

MATCH_FUNC(0x443da0)
void Car::IncrementAllocatedCarType(s32 car_kind)
{
    this->mCarKind = car_kind;
    switch (car_kind)
    {
        case car_kind::recycled_1:
            ++gCarManager_6C_677930->mRecycledCars;
            break;
        case car_kind::proto_recycled_2:
            ++gCarManager_6C_677930->mProtoRecycledCars;
            break;
        case car_kind::firefighter_5:
            ++gCarManager_6C_677930->mFirefighterCars;
            break;
        case car_kind::call_police_car_10:
            ++gCarManager_6C_677930->mKind10Cars;
            break;
        case car_kind::paramedic_car_4:
            ++gCarManager_6C_677930->mParamedicCars;
            break;
        case car_kind::police_6:
            ++gCarManager_6C_677930->mUnitCars;
            break;
        case car_kind::roadblock_car_7:
            ++gCarManager_6C_677930->mRoadblockCars;
            break;
        case car_kind::mission_car_8:
            ++gCarManager_6C_677930->mMissionCars;
            break;
        case car_kind::parked_car_9:
            ++gCarManager_6C_677930->mParkedCars;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x443e50)
void Car::MarkProtoRecycled()
{
    if (mCarKind == car_kind::recycled_1)
    {
        gCarManager_6C_677930->mRecycledCars--;
        gCarManager_6C_677930->mProtoRecycledCars++;
        mCarKind = car_kind::proto_recycled_2;
    }
}

MATCH_FUNC(0x443e80)
void Car::MarkRecycled()
{
    if (mCarKind == car_kind::proto_recycled_2)
    {
        gCarManager_6C_677930->mRecycledCars++;
        gCarManager_6C_677930->mProtoRecycledCars--;
        mCarKind = car_kind::recycled_1;
    }
}

MATCH_FUNC(0x443eb0)
void Car::ReassignCarStats(s32 car_kind)
{
    gCarManager_6C_677930->DecrementAllocatedCarType(mCarKind);
    IncrementCarStats(car_kind);
}

MATCH_FUNC(0x443ee0)
void Car::ReassignAllocatedCarType(s32 car_kind)
{
    gCarManager_6C_677930->DecrementAllocatedCarType(mCarKind);
    IncrementAllocatedCarType(car_kind);
}

MATCH_FUNC(0x443f30)
void Car::AttachLight(s32 object_type, s32 argb, s32 x_offset, s32 y_offset)
{
    Object* pObj = gObject_5C_6F8F84->NewLight_529AB0(object_type, 0, 0, 0, argb, kFpOneAndHalf_6772AC, 200);
    pObj->TurnOffLight();
    mCarSprite->AttachSprite(pObj->mSprite, (kFpOne64th_677888 * x_offset), (kFpOne64th_677888 * y_offset), kAngZero_67791C);
}

MATCH_FUNC(0x444020)
void Car::UpdateExploderTimer()
{
    if (mExploderTimer > 0)
    {
        mExploderTimer--;
        if (mExploderTimer == 0)
        {
            mExploderPedId = 0;
            mDeathCause = ped_death_cause::none_0;
        }
    }
}

MATCH_FUNC(0x444090)
u32 Car::GetEffectiveDriverPedId()
{
    Car* pCar = this;
    while (!pCar->mDriver)
    {
        if (!pCar->mpTrailer || pCar->mpTrailer->mTrailerCar != pCar)
        {
            return pCar->mExploderPedId;
        }
        pCar = pCar->mpTrailer->mTruckCab;
    }

    Ped* pDriver = pCar->mDriver;
    if (pDriver->get_occupation_403980() == ped_ocupation_enum::dummy_driver_4)
    {
        return pCar->mExploderPedId;
    }

    if (!IsPlayerPedId_420C30(pDriver->mId))
    {
        if (pCar->mExploderPedId)
        {
            return pCar->mExploderPedId;
        }
    }
    return pDriver->mId;
}

MATCH_FUNC(0x4441b0)
void Car::SetSirens()
{
    if (IsMediCar_421780())
    {
        Car::AttachLight(objects::light_165, 0xFF2010, 10, 16);
        Car::AttachLight(171, 0xFF2010, -10, 16);
        Car::AttachLight(172, 0xFF2010, 10, -32);
        Car::AttachLight(173, 0xFF2010, -10, -32);
    }
    else if (IsCopCar_421790())
    {
        Car::AttachLight(objects::light_165, 0xFF2010, 10, -16);
        Car::AttachLight(171, 255, -10, -16);
    }
    else if (IsSwatVan_4217A0())
    {
        Car::AttachLight(objects::light_165, 0xFF2010, 10, 32);
        Car::AttachLight(171, 255, -10, 32);
        Car::AttachLight(172, 0xFF2010, 10, -32);
        Car::AttachLight(173, 255, -10, -32);
    }
    else if (IsFireTruck_4118F0())
    {
        Car::AttachLight(objects::light_165, 0xFF2010, 16, 48);
        Car::AttachLight(171, 0xFF2010, -16, 48);
        Car::AttachLight(172, 0xFF2010, 16, -16);
        Car::AttachLight(173, 0xFF2010, -16, -26);
    }
    else if (IsFbiCar_411920())
    {
        Car::AttachLight(objects::light_165, 0xFF2010, 0, 16);
    }
}

MATCH_FUNC(0x444490)
void Car::PoolAllocate()
{
    this->mCarId = gCarManager_6C_677930->mNextCarId++;
    this->mDamage = 0;
    this->mDamageLevel = 0;
    this->mDamagedAreas.ClearAllBits_420D90();
    this->mPassengersList.ClearList_420E90();
    this->mDriver = NULL;
    this->mDoorLock = Car_Door_Lock::unlocked_3;
    this->mPhysics = NULL;
    this->mLightFlags = 0;
    this->mFlashPhaseCounter = 0;
    this->mLastSeenTimer = 0;
    this->mPedType = ped_type::dummy_3;
    this->mCarSprite = NULL;
    this->mEngineStatus = car_engine_status::off_1;
    this->mTurnFlags = 0;
    this->mAbandoned = false;
    this->mFlags = 0;

    this->mAttachments.DestroyAllSprites();

    ClearHorn_421460();
    IncrementCarStats(car_kind::none_0);
    this->mCarThreadFlags = 0;
    this->mpChaseTask = NULL;
    this->mExploderPedId = 0;
    this->mDeathCause = ped_death_cause::none_0;
    this->mExploderTimer = 0;
    this->mPlayerPedId = 0;
    this->mScale = kFP16One_6777D0;

    this->mFlashCount = 0;
    this->mHornCooldown = 0;
    this->mWreckCountdown = 0;
    this->mShopEvent = car_shop_event::none_0;
    this->mTurretRotated = false;
    this->mEmitterStatus = 0;
}

MATCH_FUNC(0x4446e0)
void Car::DeAllocateAI()
{
    if (mAI)
    {
        if (mAI->mRouteIdx > 0)
        {
            gRouteFinder_6FFDC8->CancelRoute(mAI->mRouteIdx);
        }

        gCarAI_78_Pool_677CF8->DeAllocate(mAI);
        mAI = NULL;
    }
}

MATCH_FUNC(0x4447d0)
void Car::PoolDeallocate()
{
    this->mAttachments.DestroyAllSprites();

    DeAllocateCarPhysics_441A10();

    if (mCarSprite)
    {
        gSpritePool_703818->remove(mCarSprite);
        mCarSprite = NULL;
    }

    DeAllocateAI();

    if (mpChaseTask)
    {
        gCarChaseTaskTable_678E30->FreeEntry(mpChaseTask);
        mpChaseTask = NULL;
    }

    KillContainedPeds();

    gWeapon_8_707018->dealloc_car_weapon(this);
    gCarManager_6C_677930->DecrementAllocatedCarType(mCarKind);

    mCarId = 0;
}

MATCH_FUNC(0x444860)
Car::Car()
{
    mDriver = NULL;
    mDamage = 0;
    mDamageLevel = 0;
    mDoorLock = Car_Door_Lock::no_lock_0;
    mEngineStatus = 0;
    mPedType = ped_type::none_0;
    mLastSeenTimer = 0;
    mLightFlags = 0;
    mFlashPhaseCounter = 0;
    mTurnFlags = 0;
    mpNext = NULL;
    mCarInfoIdx = car_model_enum::none;
    mCarSprite = NULL;
    mPhysics = NULL;
    mAI = NULL;
    mDespawnStatus = car_despawn_status::none_0;
    mCarId = 0xFFFF;
    mpTrailer = NULL;
    mFlags = 0;
    this->mAttachments.DestroyAllSprites();
    ClearHorn_421460();
    mAbandoned = false;
    mCarKind = car_kind::none_0;
    mCarThreadFlags = 0;
    mExploderPedId = 0;
    mDeathCause = ped_death_cause::none_0;
    mExploderTimer = 0;
    mPlayerPedId = 0;
    mScale.mValue = 0x4000;
    mFlashCount = 0;
}

MATCH_FUNC(0x444960)
Car::~Car()
{
    mpNext = NULL;
    mCarSprite = NULL;
    mAI = NULL;
}

MATCH_FUNC(0x447360)
void Car::ClearHeadlightDeltas_447360()
{
    ClearHeadlightDeltas_425650();
}

MATCH_FUNC(0x564300)
bool Car::IsDrivenByNonPlayer()
{
    if (mDriver)
    {
        bool result = !mDriver->PedTypeIs(ped_type::player_2);
        return result;
    }
    return false;
}

MATCH_FUNC(0x407B80)
Ped* Trailer::GetTruckCabDriver() const
{
    return mTruckCab->mDriver;
}

MATCH_FUNC(0x407b90)
Car* Trailer::GetCabOrTrailerCar(Car* pCar) const
{
    if (pCar == mTruckCab)
    {
        return mTrailerCar;
    }
    return mTruckCab;
}

MATCH_FUNC(0x407bb0)
void Trailer::SetTruckCabAndTrailerCar(Car* pTruckCab, Car* pTrailerCar)
{
    this->mTruckCab = pTruckCab;
    this->mTrailerCar = pTrailerCar;

    pTruckCab->mpTrailer = this;
    this->mTrailerCar->mpTrailer = this;
    this->mHitchAngleClamped = 0;
}

MATCH_FUNC(0x407bd0)
Fix16_Point Trailer::GetHitchPosition()
{
    // The y line calls the out-of-line Negate/Multiply/operator+ in the original, as if it
    // were written with them; with the inline RotateByAngle_40F6B0 the tail stops inlining.
    // Assigning a constructed point (not a copy) loads y before x as in the original.
    // The rotation's locals live in their own block (as if from an inline rotate): without it the
    // x_old store is scheduled after the angle load.
    Fix16_Point offset;
    offset = Fix16_Point(sTrailerHitchOffset.x, sTrailerHitchOffset.y);
    Ang16 angle = mTruckCab->mPhysics->mTheta;
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);
        Fix16 x_old = offset.x;
        offset.x = (offset.x * cos) + (offset.y * sin);
        offset.y = x_old.Negate_4086A0().Multiply(sin).Add(offset.y.Multiply(cos));
    }
    offset += mTruckCab->mPhysics->get_cp1();
    return offset;
}

// 9.6f 0x40F840 CarPhysics::IsVelocityAlignedWithHeading_40F840, inlined here with car.cpp's own
// static copies of the 90/270 degree constants
static inline bool IsVelocityAlignedWithHeading_40F840(CarPhysics* pPhys)
{
    Ang16 drift = pPhys->mLinvel1.atan2_40F790().SubtractNormalized(pPhys->mTheta);
    return drift <= kAng90_66A9C8 || drift >= kAng270_66AABC;
}

MATCH_FUNC(0x407ce0)
void Trailer::UpdateTrailerAlignment()
{
    // The four points are declared up front: the original enters with EH state 3. trailer_theta reads
    // mTheta and the Fix16 compares use mValue, otherwise the inline budget pushes these ctors out of line.
    Fix16_Point rear;
    Fix16_Point hitch;
    Fix16_Point delta;
    Fix16_Point offset;

    CarPhysics* pTrailerPhys = mTrailerCar->mPhysics;
    CarPhysics* pCabPhys = mTruckCab->mPhysics;
    Fix16 trailer_theta = Ang16::Ang16_to_Fix16(pTrailerPhys->mTheta);

    rear = mTrailerCar->GetRearWheelOffset();
    rear.RotateByAngle_40F6B0(pTrailerPhys->get_theta_40F820());
    rear += pTrailerPhys->get_cp1();

    hitch = sTrailerHitchOffset;
    Fix16 cab_theta = Ang16::Ang16_to_Fix16(pCabPhys->get_theta_40F820());
    hitch.RotateByAngle_40F6B0(Ang16::Fix16_To_Ang16_40F540(cab_theta));
    hitch += pCabPhys->get_cp1();

    // The trailer points along hitch - rear, kept within a window around the cab's angle
    delta = hitch.Sub(rear);
    Fix16 new_theta = Ang16::Ang16_to_Fix16(delta.atan2_40F790());
    mHitchAngleClamped = ClampAngleAroundTarget(&cab_theta, &new_theta);

    if (new_theta.mValue != cab_theta.mValue && new_theta.mValue == trailer_theta.mValue)
    {
        if (IsVelocityAlignedWithHeading_40F840(pCabPhys) && pCabPhys->IsGasPedalPressedEnough())
        {
            new_theta = TurnAngleToward(new_theta, &cab_theta, &mTruckCab->GetMaxTurnRate());
        }
    }

    pTrailerPhys->set_theta_40F830(Ang16::Fix16_To_Ang16_40F540(new_theta));

    offset = sTrailerCabOffset;
    offset.RotateByAngle_40F6B0(Ang16::Fix16_To_Ang16_40F540(new_theta));
    mTrailerCar->mPhysics->set_cp1_40F7E0(hitch.Sub(offset));
    pTrailerPhys->set_cp3_40F810(mTruckCab->mPhysics->get_cp3_40F800());
    pTrailerPhys->UpdateCenterOfMassPoint();
}

MATCH_FUNC(0x408140)
bool Trailer::ServicePhysics()
{
    if (!mTruckCab->mPhysics && !mTrailerCar->mPhysics)
    {
        return false;
    }

    SetupCarPhysicsAndSpriteBinding();

    if (!mTruckCab->mPhysics->ProcessCarPhysicsStateMachine() || mTruckCab->mDriver)
    {
        return mTruckCab->ManageDrowning();
    }
    DeAllocateCarPhysics();
    return false;
}

MATCH_FUNC(0x408190)
void Trailer::SetupCarPhysicsAndSpriteBinding()
{
    mTruckCab->SetupCarPhysicsAndSpriteBinding_43BC30();
    mTrailerCar->SetupCarPhysicsAndSpriteBinding_43BC30();
}

MATCH_FUNC(0x4081b0)
void Trailer::DeAllocateCarPhysics()
{
    mTruckCab->DeAllocateCarPhysics_441A10();
    mTrailerCar->DeAllocateCarPhysics_441A10();
}

MATCH_FUNC(0x4081d0)
bool Trailer::ExplodeBothIfOneDestroyed()
{
    if (mTruckCab->IsMaxDamage_40F890())
    {
        if (!mTrailerCar->IsMaxDamage_40F890())
        {
            mTrailerCar->mDamage = 32000;
            mTrailerCar->HandleCarExplosion(explosion_type::small_18);
        }
        return true;
    }
    else if (mTrailerCar->IsMaxDamage_40F890())
    {
        mTruckCab->mDamage = 32000;
        mTruckCab->HandleCarExplosion(explosion_type::small_18);
        return true;
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x408220)
s32 Trailer::Update()
{
    if (mTruckCab->mDespawnStatus != car_despawn_status::despawning_5)
    {
        gSpriteGrid_1_679208->RemoveFromRegionBuckets(mTruckCab->mCarSprite);
        gSpriteGrid_1_679208->RemoveFromRegionBuckets(mTrailerCar->mCarSprite);
    }

    const bool bDespawned = mTruckCab->UpdateCarDespawnStatus();
    mTrailerCar->mDespawnStatus = mTruckCab->mDespawnStatus;
    if (bDespawned)
    {
        gCar_BC_Pool_67792C->Remove(mTrailerCar);
        return k_trailer_update_despawned;
    }
    else
    {
        const bool bCabDrowning = ServicePhysics();
        if (mTruckCab->mCarSprite->mZpos != kFpZero_66AB38)
        {
            if (mTrailerCar->mCarSprite->mZpos != kFpZero_66AB38)
            {
                mTruckCab->mCarSprite->ResolveZOrder(mTrailerCar->mCarSprite);
            }
        }

        mTruckCab->mCarSprite->SetDrawLayer_40F7B0(15);

        mTruckCab->ServiceDoorsLightsAndHorn();
        mTrailerCar->ServiceDoorsLightsAndHorn();

        if (mTruckCab->mDespawnStatus != car_despawn_status::despawning_5)
        {
            gSpriteGrid_1_679208->AddToRegionBuckets(mTruckCab->mCarSprite);
            gSpriteGrid_1_679208->AddToRegionBuckets(mTrailerCar->mCarSprite);
        }

        if (mTruckCab->mAttachments.mP18)
        {
            mTruckCab->mAttachments.PoolUpdate(mTruckCab->mCarSprite);
            mTruckCab->mAttachments.PropagateMaxZLayer(mTruckCab->mCarSprite, 0);
        }

        if (mTrailerCar->mAttachments.mP18)
        {
            mTrailerCar->mAttachments.PoolUpdate(mTrailerCar->mCarSprite);
            mTrailerCar->mAttachments.PropagateMaxZLayer(mTrailerCar->mCarSprite, 0);
        }

        if (ExplodeBothIfOneDestroyed() || bCabDrowning)
        {
            return k_trailer_update_detach;
        }
        else
        {
            return k_trailer_update_ok;
        }
    }
}

MATCH_FUNC(0x5822E0)
void ResetTrafficSpawnGlobals()
{
    sTrafficDensityScale = kFpZero_6FF774;
    sTrafficCarSpeed = kDefaultTrafficCarSpeed_6FF744;
    DAT_006FF8C4 = 0;
    DAT_006FF8C5 = 0;
}

MATCH_FUNC(0x582310)
TrafficSpawner::TrafficSpawner()
{
    mTrafficCarModel = 0;
    mUnusedA = 0;
    mSpawnSide = 0;
    mCam = NULL;
    mPlayer = NULL;
    mZone = NULL;
    mRect = new Fix16_Rect();
    ResetTrafficSpawnGlobals();
}

MATCH_FUNC(0x582340)
TrafficSpawner::~TrafficSpawner()
{
    delete mRect;
    mRect = NULL;
    mCam = NULL;
    mPlayer = NULL;
    mZone = NULL;
}

MATCH_FUNC(0x582360)
bool TrafficSpawner::IsInsideCameraEdge(s32 direction, Fix16 x, Fix16 y)
{
    switch (direction)
    {
        case 1:
        case 2:
            if (mSpawnSide == 0)
            {
                if (x < (mCam->mViewBounds.mRight - kFpOne_6FF778))
                {
                    return true;
                }
            }
            else
            {
                if (x > (mCam->mViewBounds.mLeft + kFpOne_6FF778))
                {
                    return true;
                }
            }
            break;
        case 3:
        case 4:
            if (mSpawnSide == 0)
            {
                if (y < (mCam->mViewBounds.mBottom - kFpOne_6FF778))
                {
                    return true;
                }
            }
            else
            {
                if (y > (mCam->mViewBounds.mTop + kFpOne_6FF778))
                {
                    return true;
                }
            }
            break;
    }
    return false;
}

// 9.6f 0x4B4A60
MATCH_FUNC(0x5832C0)
void TrafficSpawner::MakeTrafficForCurrCamera()
{
    Fix16 w;

    Fix16 wanted_traffic_scale;
    if ((!bLimit_recycling_67D4CA || gCarManager_6C_677930->mRecycledCars < 2) &&
        gCarManager_6C_677930->mRecycledCars + gCarManager_6C_677930->mProtoRecycledCars != 16)
    {
        s32 angleFace = Ang16::GetAngleFace(mCam->ComputeTargetFacingAngle());

        switch (gPolice_7B8_6FEE40->mMaxWantedLevel)
        {
            case cop_level_enum::none_0:
            case cop_level_enum::police_1:
                wanted_traffic_scale = kFpOne_6FF778;
                break;
            case cop_level_enum::police_3:
                wanted_traffic_scale = kFpPoint6;
                break;
            case cop_level_enum::police_2:
            case cop_level_enum::fbi_5:
                wanted_traffic_scale = kFpPoint8;
                break;
            case cop_level_enum::swat_4:
                wanted_traffic_scale = kFpPoint4_6FF5D4;
                break;
            case cop_level_enum::army_6:
                wanted_traffic_scale = kFpPoint4_6FF5D4;
                break;
            default:
                break;
        }

        // 9.6f: width and height into locals first, then multiplied
        w = mCam->GetPaddedViewBoundsWidth_4B3110();
        Fix16 h = mCam->GetPaddedViewBoundsHeight_4B3130();
        Fix16 t = h * w;
        t = t / Fix16(86);
        // w is declared at the top of the function: with it declared here, the mTrafficCarModel store after
        // sTrafficDensityScale (as in 9.6f) made VC6 compute the height first
        sTrafficDensityScale = (t)*wanted_traffic_scale;
        this->mTrafficCarModel = 1;
        this->mUnusedA = 1;

        u8 rng_int = gRng_6F6784.get_uint8(5);
        bool bOwnerMoving = !!(mCam->ReturnOwnerVelocity() > kFpZero);

        switch (rng_int)
        {
            case 0:
                mSpawnSide = 1;
                if (!SpawnTrafficCar(1, 2, 0) && !SpawnTrafficCar(2, 1, 0) && !SpawnTrafficCar(4, 3, 0) &&
                    !SpawnTrafficCar(3, 4, 0) && gPolice_7B8_6FEE40->mMaxWantedLevel < cop_level_enum::police_3)
                {
                    if (static_cast<u8>(bOwnerMoving))
                    {
                        SpawnTrafficCarFacing(angleFace);
                    }
                }
                break;

            case 1:
                this->mSpawnSide = 0;
                if (!SpawnTrafficCar(2, 1, 0) && !SpawnTrafficCar(4, 3, 0) && !SpawnTrafficCar(3, 4, 0) &&
                    (gPolice_7B8_6FEE40->mMaxWantedLevel >= cop_level_enum::police_3 || !static_cast<u8>(bOwnerMoving) || !SpawnTrafficCarFacing(angleFace)))
                {
                    SpawnTrafficCar(1, 2, 0);
                }
                break;

            case 2:
                this->mSpawnSide = 1;
                if (!SpawnTrafficCar(4, 3, 0) && !SpawnTrafficCar(3, 4, 0) &&
                    (gPolice_7B8_6FEE40->mMaxWantedLevel >= cop_level_enum::police_3 || !static_cast<u8>(bOwnerMoving) || !SpawnTrafficCarFacing(angleFace)) &&
                    !SpawnTrafficCar(1, 2, 0))
                {
                    SpawnTrafficCar(2, 1, 0);
                }
                break;

            case 3:
                this->mSpawnSide = 0;
                if (!SpawnTrafficCar(3, 4, 0) &&
                    (gPolice_7B8_6FEE40->mMaxWantedLevel >= cop_level_enum::police_3 || !static_cast<u8>(bOwnerMoving) || !SpawnTrafficCarFacing(angleFace)) &&
                    !SpawnTrafficCar(1, 2, 0) && !TrafficSpawner::SpawnTrafficCar(2, 1, 0))
                {
                    SpawnTrafficCar(4, 3, 0);
                }
                break;

            case 4:
                mSpawnSide = 1;
                if ((gPolice_7B8_6FEE40->mMaxWantedLevel >= cop_level_enum::police_3 || !static_cast<u8>(bOwnerMoving) || !SpawnTrafficCarFacing(angleFace)) &&
                    !SpawnTrafficCar(1, 2, 0) && !SpawnTrafficCar(2, 1, 0) && !SpawnTrafficCar(4, 3, 0))
                {
                    SpawnTrafficCar(3, 4, 0);
                }
                break;

            default:
                return;
        }
    }
}

MATCH_FUNC(0x583260)
bool TrafficSpawner::SpawnTrafficCarFacing(s32 angle_face)
{
    switch (angle_face)
    {
        case 1:
            return SpawnTrafficCar(1, 1, 0);
        case 2:
            return SpawnTrafficCar(2, 2, 0);
        case 3:
            return SpawnTrafficCar(3, 3, 0);
        case 4:
            return SpawnTrafficCar(4, 4, 0);
    }
    return true;
}

// 9.6f: the y read is Ped::get_cam_y (0x403A10, inlined, using it changes the code)
MATCH_FUNC(0x583670)
void TrafficSpawner::GenerateTraffic()
{
    if (!bSkip_recycling_67D575)
    {
        mCam = gGame_0x40_67E008->IteratePlayerCamera();
        mPlayer = gGame_0x40_67E008->IterateFirstPlayer();
        if (mPlayer)
        {
            mZone = gMap_0x370_6F6268->GetZoneInfoAt(mPlayer->mPlayerPed->get_cam_x().ToInt(),
                                                                           mPlayer->mPlayerPed->mCam.y.ToInt());
        }

        while (mCam)
        {
            if (mCam->HasFollowTarget_433E90())
            {
                TrafficSpawner::MakeTrafficForCurrCamera();
            }
            mCam = gGame_0x40_67E008->IterateNextPlayerCamera();

            Ped* pPed;
            if (mCam == &mPlayer->mAuxGameCamera)
            {
                pPed = mPlayer->mAuxPed;
            }
            else
            {
                mPlayer = gGame_0x40_67E008->IterateNextPlayer();
                if (mPlayer)
                {
                    pPed = mPlayer->mPlayerPed;
                }
                else
                {
                    continue;
                }
            }
            if (pPed)
            {
                mZone = gMap_0x370_6F6268->GetZoneInfoAt(pPed->get_cam_x().ToInt(), pPed->mCam.y.ToInt());
            }
        }
    }
}

MATCH_FUNC(0x583750)
Fix16 TrafficSpawner::GetRandomTrafficSpeed(Fix16 max_speed, u8* pOutSpeedBand)
{

    Fix16 roll;
    Fix16 lo;
    Fix16 hi;

    if (max_speed >= kFpPoint4_6FF70C)
    {
        *pOutSpeedBand = k_traffic_speed_fast;
    }

    if (max_speed >= kFpPoint3)
    {
        *pOutSpeedBand = k_traffic_speed_fast;
        roll = Fix16(gRng_6F6784.get_uint8(100)) / Fix16(100);
        lo = kFpPoint3;
        hi = kFpPoint4_6FF70C;
    }
    else if (max_speed >= kFpPoint2_6FF724)
    {
        *pOutSpeedBand = k_traffic_speed_medium;
        roll = Fix16(gRng_6F6784.get_uint8(100)) / Fix16(100);
        lo = kFpPoint2_6FF724;
        hi = kFpPoint3;
    }
    else
    {
        *pOutSpeedBand = k_traffic_speed_slow;
        roll = Fix16(gRng_6F6784.get_uint8(100)) / Fix16(100);
        lo = kFpPoint03;
        hi = kFpPoint2_6FF724;
    }
    // Fix16::operator= (not the copy ctor) on the result: it loads the return slot
    // into ecx before the add and gives the trailing `mov eax, ecx`.
    Fix16 result;
    result = lo + roll * (hi - lo);
    return result;
}

// 9.6f 0x4B34E0
MATCH_FUNC(0x582480)
bool TrafficSpawner::SpawnTrafficCar(s32 spawn_type, s32 arrow_direction, s32 blocked_direction)
{
    char_type x_step = 0;
    char_type y_step = 0;
    u8 speed_kind;
    bool bRoadblock = false;
    Gang* pGang = NULL;
    s32 found_z;
    Fix16 ground_z;

    if (gCarManager_6C_677930->mRecycledCars + gCarManager_6C_677930->mProtoRecycledCars == 16 || arrow_direction == blocked_direction ||
        gCarManager_6C_677930->mSpawnBlock > 0)
    {
        return false;
    }

    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(mTrafficCarModel);
    Fix16 car_w = gPixelsToFix16_6F6850.list[pCarInfo->w];
    Fix16 car_h = gPixelsToFix16_6F6850.list[pCarInfo->h];
    SwapIf3or4_41FE40(arrow_direction, car_w, car_h);

    DAT_006FF8C4 = gRng_6F6784.get_uint8(100);
    switch (arrow_direction)
    {
        case 1:
        case 2:
            car_h *= kFpOneAndHalf_6FF680;
            break;
        case 3:
        case 4:
            car_w *= kFpOneAndHalf_6FF680;
            break;
    }

    Fix16 xpos;
    Fix16 ypos;
    // Case order 1, 2, 4, 3 follows the 10.5 block layout (case 4 jumps into case 3's tail). The empty
    // default is needed: without it case 1's x_step = -1 block merges into case 2's instead of the reverse.
    switch (spawn_type)
    {
        case 1:
            ypos = mCam->mViewBounds.mTop - kFpFive_6FF6D4;
            if (!mSpawnSide)
            {
                xpos = mCam->mViewBounds.mLeft - kFpOne_6FF778;
                x_step = 1;
            }
            else
            {
                xpos = mCam->mViewBounds.mRight + kFpOne_6FF778;
                x_step = -1;
            }
            break;
        case 2:
            ypos = mCam->mViewBounds.mBottom + kFpFive_6FF6D4;
            if (!mSpawnSide)
            {
                xpos = mCam->mViewBounds.mLeft - kFpOne_6FF778;
                x_step = 1;
            }
            else
            {
                xpos = mCam->mViewBounds.mRight + kFpOne_6FF778;
                x_step = -1;
            }
            break;
        case 4:
            xpos = mCam->mViewBounds.mLeft - kFpFive_6FF6D4;
            if (!mSpawnSide)
            {
                ypos = mCam->mViewBounds.mTop - kFpOne_6FF778;
                y_step = 1;
            }
            else
            {
                ypos = mCam->mViewBounds.mBottom + kFpOne_6FF778;
                y_step = -1;
            }
            break;
        case 3:
            xpos = mCam->mViewBounds.mRight + kFpFive_6FF6D4;
            if (!mSpawnSide)
            {
                ypos = mCam->mViewBounds.mTop - kFpOne_6FF778;
                y_step = 1;
            }
            else
            {
                ypos = mCam->mViewBounds.mBottom + kFpOne_6FF778;
                y_step = -1;
            }
            break;
        default:
            break;
    }

    // One and two blocks along the road direction
    Fix16 x_one = kFpZero_6FF774;
    Fix16 y_one = kFpZero_6FF774;
    Fix16 x_two = kFpZero_6FF774;
    Fix16 y_two = kFpZero_6FF774;
    switch (arrow_direction)
    {
        case 1:
            x_one = kFpOne_6FF778;
            x_two = kFpTwo_6FF77C;
            break;
        case 2:
            x_one = -kFpOne_6FF778;
            x_two = -kFpTwo_6FF77C;
            break;
        case 3:
            y_one = kFpOne_6FF778;
            y_two = kFpTwo_6FF77C;
            break;
        case 4:
            y_one = -kFpOne_6FF778;
            y_two = -kFpTwo_6FF77C;
            break;
    }

    while (xpos > kFpZero_6FF774 && ypos > kFpZero_6FF774 && xpos < kFp255_6FF558 && ypos < kFp255_6FF558)
    {
        gmp_block_info* pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(xpos.ToInt(), ypos.ToInt(), &found_z);
        ground_z = gMap_0x370_6F6268->FindGroundZForCoord(xpos, ypos);
        if (pBlock)
        {
            u8 slope_type = pBlock->mSlopeType;
            if (!(IS_GRADIENT_SLOPE(slope_type) && GET_BLOCK_TYPE(slope_type) != 0) && GET_BLOCK_TYPE(slope_type) == 1)
            {
                Fix16 nav_x = xpos.GetFloorValue() + kFpHalf_6FF674;
                Fix16 nav_y = ypos.GetFloorValue() + kFpHalf_6FF674;
                sTrafficCarSpeed = kFpPoint2_6FF5CC;
                mRect->SetRect_41E350(nav_x - car_w, nav_x + car_w, nav_y - car_h, nav_y + car_h);
                mRect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                if (mRect->EveryTileMatchesArrowType(arrow_direction))
                {
                    gmp_zone_info* pZone = gMap_0x370_6F6268->GetZoneInfoAt(nav_x.ToInt(), nav_y.ToInt());
                    if (pZone && !pZone->mCarDensity)
                    {
                        return false;
                    }

                    s32 car_model;
                    u16 traffic_category;
                    u8 rng_val;
                    if (gPolice_7B8_6FEE40->mMaxWantedLevel < cop_level_enum::police_1 || (rng_val = static_cast<u8>(gRng_6F6784.get_int(40)), rng_val <= 20u) ||
                        rng_val >= 30u)
                    {
                        car_model = gCarManager_6C_677930->SelectTrafficCarModel(mPlayer, mZone, sTrafficDensityScale, &traffic_category);
                        if (traffic_category == k_traffic_none)
                        {
                            return false;
                        }
                    }
                    else
                    {
                        car_model = car_model_enum::COPCAR;
                        traffic_category = k_traffic_police;
                    }

                    if (gPolice_7B8_6FEE40->TryBeginRoadblock())
                    {
                        bRoadblock = true;
                        car_model = car_model_enum::bank_van;
                    }
                    else
                    {
                        if (car_model == car_model_enum::COPCAR)
                        {
                            if (gPolice_7B8_6FEE40->mPoliceCarCount >= static_cast<u8>(gPolice_7B8_6FEE40->mMaxPoliceCars) || bSkip_police_67D4F9)
                            {
                                car_model =
                                    gCarManager_6C_677930->SelectTrafficCarModel(mPlayer, mZone, sTrafficDensityScale, &traffic_category);
                                if (car_model == car_model_enum::COPCAR || traffic_category == k_traffic_none)
                                {
                                    return false;
                                }
                            }
                            else
                            {
                                switch (gPolice_7B8_6FEE40->mHighestCrewTypeInPursuit)
                                {
                                    case crew_type::police_3:
                                        car_model = car_model_enum::COPCAR;
                                        break;
                                    case crew_type::fbi_4:
                                        car_model = car_model_enum::EDSELFBI;
                                        break;
                                    case crew_type::army_6:
                                        car_model = car_model_enum::GUNJEEP;
                                        break;
                                }
                            }
                        }
                        else if (gPolice_7B8_6FEE40->mHighestCrewTypeInPursuit == crew_type::army_6)
                        {
                            switch (gRng_6F6784.get_int(10))
                            {
                                case 0:
                                case 1:
                                case 2:
                                case 3:
                                    car_model = car_model_enum::apc;
                                    break;
                                case 4:
                                case 5:
                                case 6:
                                    car_model = car_model_enum::TANK;
                                    break;
                                default:
                                    car_model = car_model_enum::JEEP;
                                    break;
                            }
                        }

                        if (traffic_category == k_traffic_gang)
                        {
                            pGang = mPlayer->get_gang_curr_location_4766D0();
                            if (!pGang || !pGang->HasKillRespectChange_45DD50())
                            {
                                return false;
                            }
                            car_model = pGang->mGangCarModel;
                        }
                    }

                    Fix16 max_speed = gCarInfo_808_678098->GetModelPhysicsFromIdx(car_model)->mMaxSpeed;
                    Fix16 speed;
                    speed = GetRandomTrafficSpeed(max_speed, &speed_kind);
                    sTrafficCarSpeed = speed;

                    // Number of matching road tiles in the 5 tile column and the indices of the first and last one
                    u8 road_len = 1;
                    u8 first_idx = 2;
                    u8 last_idx = 2;

                    if (IsFlatRoadAt(xpos - x_one, ypos - y_one))
                    {
                        Fix16 tx = (xpos - x_one).GetFloorValue() + kFpHalf_6FF674;
                        Fix16 ty = (ypos - y_one).GetFloorValue() + kFpHalf_6FF674;
                        mRect->SetRect_41E350(tx - car_w, tx + car_w, ty - car_h, ty + car_h);
                        mRect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                        if (mRect->EveryTileMatchesArrowType(arrow_direction))
                        {
                            road_len = 2;
                            first_idx = 1;
                        }
                    }

                    if (IsFlatRoadAt(xpos - x_two, ypos - y_two))
                    {
                        Fix16 tx = (xpos - x_two).GetFloorValue() + kFpHalf_6FF674;
                        Fix16 ty = (ypos - y_two).GetFloorValue() + kFpHalf_6FF674;
                        mRect->SetRect_41E350(tx - car_w, tx + car_w, ty - car_h, ty + car_h);
                        mRect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                        if (mRect->EveryTileMatchesArrowType(arrow_direction))
                        {
                            road_len++;
                            first_idx = 0;
                        }
                    }

                    if (IsFlatRoadAt(xpos + x_one, ypos + y_one))
                    {
                        Fix16 tx = (xpos + x_one).GetFloorValue() + kFpHalf_6FF674;
                        Fix16 ty = (ypos + y_one).GetFloorValue() + kFpHalf_6FF674;
                        mRect->SetRect_41E350(tx - car_w, tx + car_w, ty - car_h, ty + car_h);
                        mRect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                        if (mRect->EveryTileMatchesArrowType(arrow_direction))
                        {
                            last_idx = 3;
                            road_len++;
                        }
                    }

                    if (IsFlatRoadAt(xpos + x_two, ypos + y_two))
                    {
                        Fix16 tx = (xpos + x_two).GetFloorValue() + kFpHalf_6FF674;
                        Fix16 ty = (ypos + y_two).GetFloorValue() + kFpHalf_6FF674;
                        mRect->SetRect_41E350(tx - car_w, tx + car_w, ty - car_h, ty + car_h);
                        mRect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                        if (mRect->EveryTileMatchesArrowType(arrow_direction))
                        {
                            last_idx = 4;
                            road_len++;
                        }
                    }

                    Fix16 front_x;
                    Fix16 front_y;
                    Fix16 back_x;
                    Fix16 back_y;
                    Fix16 mid_x;
                    Fix16 mid_y;
                    if (road_len == 1)
                    {
                        front_x = xpos + x_one * GetLaneOffset(last_idx);
                        front_y = ypos + y_one * GetLaneOffset(last_idx);
                        mid_x = front_x;
                        mid_y = front_y;
                        back_x = front_x;
                        back_y = front_y;
                        sTrafficCarSpeed = GetRandomTrafficSpeed(kFpPoint03, &speed_kind);
                    }
                    else if (road_len == 2)
                    {
                        front_x = xpos + x_one * GetLaneOffset(last_idx);
                        front_y = ypos + y_one * GetLaneOffset(last_idx);
                        back_x = xpos + x_one * GetLaneOffset(first_idx);
                        back_y = ypos + y_one * GetLaneOffset(first_idx);
                        mid_x = back_x;
                        mid_y = back_y;
                    }
                    else if (road_len == 3)
                    {
                        front_x = xpos + x_one * GetLaneOffset(last_idx);
                        front_y = ypos + y_one * GetLaneOffset(last_idx);
                        back_x = xpos + x_one * GetLaneOffset(first_idx);
                        back_y = ypos + y_one * GetLaneOffset(first_idx);
                        mid_x = (back_x + front_x) / kFpTwo_6FF77C;
                        mid_y = (back_y + front_y) / kFpTwo_6FF77C;
                    }

                    Fix16 spawn_x;
                    Fix16 spawn_y;
                    if (speed_kind == k_traffic_speed_fast)
                    {
                        if (static_cast<u8>(DAT_006FF8C4) > 2u)
                        {
                            spawn_x = back_x;
                            spawn_y = back_y;
                            ResolveSpawnDirection(2, road_len);
                        }
                        else if (static_cast<u8>(DAT_006FF8C4) > 0u)
                        {
                            spawn_x = mid_x;
                            spawn_y = mid_y;
                            ResolveSpawnDirection(1, road_len);
                        }
                        else
                        {
                            spawn_x = front_x;
                            spawn_y = front_y;
                            ResolveSpawnDirection(0, road_len);
                        }
                    }
                    else if (speed_kind == k_traffic_speed_medium)
                    {
                        if (static_cast<u8>(DAT_006FF8C4) > 1u)
                        {
                            spawn_x = mid_x;
                            spawn_y = mid_y;
                            ResolveSpawnDirection(1, road_len);
                        }
                        else
                        {
                            spawn_x = front_x;
                            spawn_y = front_y;
                            ResolveSpawnDirection(0, road_len);
                        }
                    }
                    else
                    {
                        spawn_x = front_x;
                        spawn_y = front_y;
                        ResolveSpawnDirection(0, road_len);
                    }

                    Fix16 sx = spawn_x.GetFloorValue() + kFpHalf_6FF674;
                    Fix16 sy = spawn_y.GetFloorValue() + kFpHalf_6FF674;
                    mRect->SetRect_41E350(sx - car_w, sx + car_w, sy - car_h, sy + car_h);
                    mRect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);

                    u8 spawn_chance;
                    if (bStartNetworkGame_7081F0)
                    {
                        spawn_chance = 0;
                    }
                    else
                    {
                        spawn_chance = gRng_6F6784.get_uint8(100);
                    }

                    if ((speed_kind == k_traffic_speed_fast && spawn_chance < 33u) || (speed_kind == k_traffic_speed_medium && spawn_chance < 67u) || speed_kind == k_traffic_speed_slow)
                    {
                        if (!gSpriteGrid_1_679208->CheckRectForCollisions(mRect, false, sprite_types_enum::none_0, NULL) &&
                            !mRect->IsRectBlockedByMap() && !mRect->RectOverlapsZone(2))
                        {
                            Fix16 car_ypos = sy.GetFloorValue() + kFpHalf_6FF674;
                            Fix16 car_x = sx.GetFloorValue() + kFpHalf_6FF674;
                            Fix16 car_y = car_ypos;
                            if (!gGame_0x40_67E008->IsRectVisibleToAnyPlayer(mRect))
                            {
                                if (bRoadblock)
                                {
                                    gPolice_7B8_6FEE40->TryCreateRoadblockAt(car_x.ToInt(), car_y.ToInt(), arrow_direction);
                                }
                                else
                                {
                                    Car* pNewCar = gCarManager_6C_677930->SpawnCarAtCorrectZ_WithScale(car_x,
                                                                                                car_y,
                                                                                                ReturnAngleFromRoadDirection(arrow_direction),
                                                                                                car_model,
                                                                                                kFpOne_6FF778);
                                    if (car_model != car_model_enum::SWATVAN && car_model != car_model_enum::COPCAR &&
                                        car_model != car_model_enum::JEEP && car_model != car_model_enum::EDSELFBI)
                                    {
                                        pNewCar->IncrementCarStats(car_kind::recycled_1);
                                        if (traffic_category == k_traffic_gang)
                                        {
                                            u8 gang_car_remap = pGang->mGangCarRemap;
                                            if (gang_car_remap == 0xFF)
                                            {
                                                pNewCar->mCarSprite->SetPaletteSprites_420700();
                                            }
                                            else
                                            {
                                                pNewCar->mCarSprite->SetRemap(gang_car_remap);
                                            }
                                            pNewCar->AddGangDriver(pGang);
                                            pNewCar->AttachGangIcon(pGang->mArrowColour);
                                        }
                                        else
                                        {
                                            pNewCar->AssignRandomRemap();
                                            pNewCar->SpawnDriverPed();
                                        }
                                        pNewCar->InitCarAIControl();
                                        pNewCar->StartEngine_426E00();
                                        pNewCar->mAI->mMaxSpeed = sTrafficCarSpeed;
                                    }
                                    else if (gCarManager_6C_677930->CanAllocateCarOfKind(car_kind::police_6) &&
                                             gPolice_7B8_6FEE40->SpawnCrewInCar(pNewCar))
                                    {
                                        pNewCar->IncrementCarStats(car_kind::police_6);
                                    }
                                    else
                                    {
                                        pNewCar->IncrementCarStats(car_kind::recycled_1);
                                        pNewCar->DespawnSoon_421490();
                                    }

                                    gGame_0x40_67E008->ReduceTrafficSpawnCounterOfOtherViewers(pNewCar->mCarSprite,
                                                                                                      mPlayer);

                                    Player* pPlayer = mPlayer;
                                    if (pPlayer->mTrafficSpawnCounter > pPlayer->mTrafficSpawnThreshold)
                                    {
                                        pPlayer->mTrafficSpawnCounter -= pPlayer->mTrafficSpawnThreshold;
                                        return false;
                                    }
                                    pPlayer->mTrafficSpawnCounter = 0;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        xpos += Fix16(x_step);
        ypos += Fix16(y_step);
        if (!IsInsideCameraEdge(spawn_type, xpos, ypos))
        {
            return false;
        }
    }
    return false;
}

MATCH_FUNC(0x583870)
bool TrafficSpawner::IsFlatRoadAt(Fix16 xpos, Fix16 ypos)
{
    if (xpos > kFpZero_6FF774 && ypos > kFpZero_6FF774 && xpos < kFp255_6FF558 && ypos < kFp255_6FF558)
    {
        s32 zpos;
        gmp_block_info* pHighBlock = gMap_0x370_6F6268->FindHighestBlockForCoord(xpos.ToInt(), ypos.ToInt(), &zpos);
        if (!pHighBlock)
        {
            return false;
        }

        u8 slope_type = pHighBlock->mSlopeType;
        if (IS_GRADIENT_SLOPE(slope_type) && GET_BLOCK_TYPE(slope_type) != 0)
        {
            return false;
        }

        if (GET_BLOCK_TYPE(slope_type) == 1)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5838E0)
s32 TrafficSpawner::GetLaneOffset(u8 lane_idx)
{
    switch (lane_idx)
    {
        case 0:
            return -2;
        case 1:
            return -1;
        case 3:
            return 1;
        case 4:
            return 2;
    }
    return 0;
}

MATCH_FUNC(0x583930)
char_type TrafficSpawner::ResolveSpawnDirection(char_type spawn_pos_idx, char_type road_len)
{
    if (road_len == 3)
    {
        return spawn_pos_idx;
    }
    else if (road_len != 2)
    {
        return 2;
    }
    else if (spawn_pos_idx == 1)
    {
        return 2;
    }
    else
    {
        return spawn_pos_idx;
    }
}

// Defined after HandleCarHitByObject, so that VC6 hasn't compiled their out-of-line copies (and
// learnt that they can't throw) when it compiles that function.
inline Fix16_Point Sprite::get_x_y_late_443580()
{
    return Fix16_Point(mXy.x, mXy.y);
}

inline Fix16_Point Fix16_Point::AddLate_40AC50(const Fix16_Point& in)
{
    return Fix16_Point(x + in.x, y + in.y);
}

inline Fix16_Point Fix16_Point::SubLate_40AC80(const Fix16_Point& rhs)
{
    return Fix16_Point(x - rhs.x, y - rhs.y);
}
