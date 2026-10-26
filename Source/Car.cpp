#include "Car.hpp"
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

// field_A4_light_flags bits of CarLightFlags8: bEmergencyLights, bLightFlashing and bHeadlightFlash
enum
{
    kEmergencyOrFlashingLightsMask = 0x1C
};

// TODO: Its in FrontEnd by seems crazy to include the whole file just for this
EXTERN_GLOBAL(bool, gCheatMiniCars_67D6C8);

DEFINE_GLOBAL(CarThreadTriggers*, gCarThreadTriggers_214_705F20, 0x705F20);
DEFINE_GLOBAL(CarManager*, gCarManager_6C_677930, 0x677930);
DEFINE_GLOBAL(CarPool*, gCar_BC_Pool_67792C, 0x67792C);
DEFINE_GLOBAL(TrailerPool*, gTrailerPool_66AC80, 0x66AC80);
DEFINE_GLOBAL_INIT(Fix16_Point, gTrailerHitchOffset_66AAC8, Fix16_Point(Fix16(0, 0), Fix16(-0x1E00, 0)), 0x66AAC8); // (0, -30/64)
DEFINE_GLOBAL_INIT(Fix16_Point, gTrailerCabOffset_66AAE0, Fix16_Point(Fix16(0, 0), Fix16(0x2F00, 0)), 0x66AAE0); // (0, 47/64)
DEFINE_GLOBAL_INIT(Ang16, kAng90_66A9C8, Ang16(360), 0x66A9C8);
DEFINE_GLOBAL_INIT(Ang16, kAng270_66AABC, Ang16(1080), 0x66AABC);
DEFINE_GLOBAL(TrafficSpawner*, gTrafficSpawner_14_677934, 0x677934);
DEFINE_GLOBAL_INIT(Fix16, kFpOneAndHalf_6772AC, Fix16(0x6000, 0), 0x6772AC);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint4_6FF70C, Fix16(0x1999, 0), 0x6FF70C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint3_6FF85C, Fix16(0x1333, 0), 0x6FF85C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FF724, Fix16(0xCCC, 0), 0x6FF724);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint03_6FF6A4, Fix16(0x1EB, 0), 0x6FF6A4);

// This is not used outside this file.
// In fact, it's only allocated and deallocated, it's never used.
DEFINE_GLOBAL(Sprite*, gSprite_Unused_677938, 0x677938);

DEFINE_GLOBAL_INIT(Fix16, gFix16_6777CC, Fix16(0), 0x6777CC);
DEFINE_GLOBAL_INIT(Fix16, kTvAntennaOffsetY_6778A0, Fix16(0x100, 0), 0x6778A0);
DEFINE_GLOBAL_INIT(Fix16, kMachineGunOffsetY_6777FC, Fix16(0xFFFFEE00, 0), 0x6777FC);

EXTERN_GLOBAL(CarInfo_2C*, gCarInfo_2C_6FE0E4);
EXTERN_GLOBAL(ModelPhysics*, gCarInfo_48_6FE258);

DEFINE_GLOBAL(u16, gBlockedRoadBrakeTimer_677CFC, 0x677CFC);
DEFINE_GLOBAL(SpriteList, stru_67737C, 0x67737c);

DEFINE_GLOBAL_INIT(Fix16, kWaterCannonOffsetY_6771F0, Fix16(0x800, 0), 0x6771F0);
DEFINE_GLOBAL_INIT(Ang16, kAng180_677326, Ang16(0x2D0), 0x677326);
DEFINE_GLOBAL_INIT(Fix16, kTankCannonOffsetY_6772A4, Fix16(0x800, 0), 0x6772A4);

DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_677208, Fix16(0x1000, 0), 0x677208);

// Indicates if CarRemapRng is initialised
// It can probably turned into a static variable inside CarRemapRng
DEFINE_GLOBAL(char_type, gbRngRemapTableDone_679C0A, 0x679C0A);

// Array of values used by CarRemapRng.
// It can probably turned into a static variable inside CarRemapRng
DEFINE_GLOBAL_ARRAY(u16, gRngRemapTable_679320, 1000, 0x679320);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6777D0, Fix16(1), 0x6777D0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6772D0, Fix16(0x2000, 0), 0x6772D0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6771FC, Fix16(0x2000, 0), 0x6771FC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_677888, Fix16(0x100, 0), 0x677888);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_6778D0, Fix16(5), 0x6778D0);
DEFINE_GLOBAL_INIT(Fix16, kDefaultTrafficCarSpeed_6FF744, Fix16(0x147, 0), 0x6FF744);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6FF774, Fix16(0), 0x006FF774);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6FF580, kFpZero_6FF774, 0x6FF580);
DEFINE_GLOBAL_INIT(Fix16, kFp255_6FF558, Fix16(255), 0x6FF558);
DEFINE_GLOBAL_INIT(Fix16, gTrafficCarSpeed_6FF570, Fix16(0x147, 0), 0x6FF570);
DEFINE_GLOBAL_INIT(Fix16, gTrafficDensityScale_6FF7E8, kFpZero_6FF774, 0x6FF7E8);
DEFINE_GLOBAL(s8, DAT_006FF8C4, 0x6FF8C4);
DEFINE_GLOBAL(s8, DAT_006FF8C5, 0x6FF8C5);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FF778, Fix16(1), 0x6ff778);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6FF5E4, Fix16(0x3333, 0), 0x6FF5E4);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint6_6FF5DC, Fix16(0x2666, 0), 0x6FF5DC);
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

DEFINE_GLOBAL_INIT(Fix16_Point, kZeroPoint_6778A8, Fix16_Point(0, 0), 0x6778A8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_677908, Fix16(1), 0x677908);

DEFINE_GLOBAL_INIT(Fix16, kZero_705DD8, Fix16(0), 0x705DD8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_705DDC, Fix16(1), 0x705DDC);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_705F10, Ang16(0), 0x705F10);

DEFINE_GLOBAL_INIT(Fix16, kFpHalf_677218, Fix16(0x2000, 0), 0x677218);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint13_676984, Fix16(0x852, 0), 0x676984);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint26_6778B4, Fix16(0x10A4, 0), 0x6778B4);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6778E0, Fix16(0x666, 0), 0x6778E0);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6777D4, Fix16(2), 0x6777D4);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6772CC, Fix16(0xCCC, 0), 0x6772CC);
EXTERN_GLOBAL(u8, gDoorAccessCheckActive_6F8EDC);

DEFINE_GLOBAL_INIT(Fix16, kFpZero_66AB38, Fix16(0), 0x66AB38);

DEFINE_GLOBAL_INIT(Fix16_Point, kTrailerHitchOffset_677370, Fix16_Point(0, kFpOne64th_677888 * 47), 0x677370);
DEFINE_GLOBAL_INIT(Fix16_Point, kCabHitchOffset_677358, Fix16_Point(0, kFpOne64th_677888 * -30), 0x677358);
DEFINE_GLOBAL_INIT(Ang16, kAng90_677234, Ang16(0x168), 0x677234);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6778FC, Fix16(0x2000, 0), 0x6778FC);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_677918, Fix16(8), 0x677918);
DEFINE_GLOBAL_INIT(Fix16, kAngFix16OneDegree_677920, Fix16(0x11C, 0), 0x677920);

DEFINE_GLOBAL_INIT(Fix16, kFpFiveEighths_6778C8, Fix16(0x2800, 0), 0x6778C8);
DEFINE_GLOBAL_INIT(Ang16, kRoofTurretAutoRotSpeed_677910, Ang16(4), 0x677910);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6F67EA, Ang16(720), 0x6F67EA);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6F6754, Ang16(360), 0x6F6754);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6F6808, Ang16(1080), 0x6F6808);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_6F6D3C, Ang16(0), 0x6F6D3C);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6772BC, Fix16(0xCCC, 0), 0x6772BC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint4_677214, Fix16(0x1999, 0), 0x677214);

DEFINE_GLOBAL_INIT(Ang16, kFireTruckCannonRotSpeed_6771C0, Ang16(0x14), 0x6771C0);
DEFINE_GLOBAL_INIT(Ang16, kTankTurretRotSpeed_677352, Ang16(0x14), 0x677352);
DEFINE_GLOBAL_INIT(Ang16, kGunJeepTurretRotSpeed_677810, Ang16(0x14), 0x677810);

DEFINE_GLOBAL_INIT(Fix16, kFp255_676D98, Fix16(255), 0x676D98);
DEFINE_GLOBAL_INIT(Fix16, kDrowningMaxSpeed_677794, kFpPoint05_6777A0, 0x677794);

DEFINE_GLOBAL(s32, gAverageCarModelsCount_6772DC, 0x6772DC);
DEFINE_GLOBAL(s32, gBadCarModelsCount_6772EC, 0x6772EC);
DEFINE_GLOBAL(s32, gGoodCarModelsCount_677294, 0x677294);

DEFINE_GLOBAL_INIT(car_rng_list, gGoodCarModels_677384, car_rng_list(Fix16(256, 0)), 0x677384);
DEFINE_GLOBAL_INIT(car_rng_list, gAverageCarModels_676DB4, car_rng_list(Fix16(512, 0)), 0x676DB4);
DEFINE_GLOBAL_INIT(car_rng_list, gBadCarModels_676988, car_rng_list(Fix16(5)), 0x676988);

//DEFINE_GLOBAL_ARRAY(s32, dword_676DB8, 256, 0x676DB8); // TODO: may be part of a struct
//DEFINE_GLOBAL_ARRAY(s32, dword_67698C, 256, 0x67698C); // TODO: may be part of a struct

DEFINE_GLOBAL_INIT(Fix16, kHitchPullDist_6772C0, kFpOne64th_677888 * 8, 0x6772C0);
DEFINE_GLOBAL_INIT(Fix16, kHitchAttachDist_677900, kFpOne64th_677888 * 3, 0x677900);

// TODO: Move
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
void ClearCarInfoGlobals_5639C0()
{
    gCarInfo_2C_6FE0E4 = NULL;
    gCarInfo_48_6FE258 = NULL;
}

MATCH_FUNC(0x447640)
void ResetBlockedRoadBrakeTimer_447640()
{
    gBlockedRoadBrakeTimer_677CFC = 0;
}

MATCH_FUNC(0x5c8680)
void CarThreadTriggers::FreeTrigger_5C8680(u8 idx)
{
    field_0_triggers[idx].field_8_type = 0;
    field_0_triggers[idx].field_C_action = 0;
    field_0_triggers[idx].field_0_pScriptCmd = 0;
    field_0_triggers[idx].field_14_enable_state = 2;
    field_0_triggers[idx].field_4_O2C->Dealloc_5291B0();
    field_0_triggers[idx].field_4_O2C = 0;
}

MATCH_FUNC(0x5c86c0)
char_type CarThreadTriggers::AddThreadTrigger_5C86C0(const s32& type, const s32& param, SCR_THREAD* pScriptThread, Fix16 xpos, Fix16 ypos, Fix16 zpos, Fix16 width, Fix16 height)
{
    CarThreadTrigger* pIter = field_0_triggers;
    for (u8 idx = 0; idx < GTA2_COUNTOF(field_0_triggers); idx++)
    {
        if (pIter->field_8_type == 0)
        {
            pIter->field_8_type = type;
            pIter->field_C_action = param;
            pIter->field_0_pScriptCmd = reinterpret_cast<s32>(pScriptThread);
            pIter->field_14_enable_state = 1;
            pIter->field_4_O2C = gObject_5C_6F8F84->NewTouchPoint_529950(objects::savepoint_161,
                                                                         xpos,
                                                                         ypos,
                                                                         zpos,
                                                                         kAngZero_705F10,
                                                                         width,
                                                                         height,
                                                                         kFpOne_705DDC);
            pIter->field_4_O2C->SetThreadTriggerIdx_4C4F10(idx);
            field_210_count++;
            return idx;
        }
        pIter++;
    }
    return -1;
}

MATCH_FUNC(0x5c8750)
void CarThreadTriggers::Reset_5C8750()
{
    field_210_count = 0;
    CarThreadTrigger* pOff = &field_0_triggers[0];
    for (u8 i = 0; i < GTA2_COUNTOF(field_0_triggers); i++)
    {
        pOff->field_10_remap_rng = i;
        pOff->field_8_type = 0;
        pOff->field_C_action = 0;
        pOff->field_0_pScriptCmd = 0;
        pOff->field_14_enable_state = 1;
        pOff++;
    }
}

// Script command that a CarThreadTrigger touch point waits on (several command types share this layout)
struct CarThreadTriggerCmd : SCR_CMD_HEADER
{
    union
    {
        u16 field_8_idx;
        ScriptThread* field_8_thread;
    };
    u16 field_C_thread_id;
    s16 field_E_thread_start_cmd;
    u16 field_10_char_idx;
    u8 field_12_pad[0x12];
    u16 field_24_char_idx;
};

MATCH_FUNC(0x5c8780)
void CarThreadTriggers::CheckThreadTrigger_5C8780(u8 idx, Sprite* pSprite)
{
    CarThreadTrigger* pEntry = &field_0_triggers[idx];
    if (pEntry->field_14_enable_state != 1 || !pEntry->field_4_O2C)
    {
        return;
    }

    CarThreadTriggerCmd* pCmd;
    Car* pCar;
    Ped* pPed;
    SCR_POINTER* pPointer;

    switch (pEntry->field_8_type)
    {
        case 1:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pEntry->field_14_enable_state = 0;
                pCar = pSprite->AsCar_40FEB0();
            }
            break;

        case 2:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->field_8_car_bc_ptr;
                pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd)->field_8_idx));
                if (pCar->field_6C_car_id == pPointer->field_8_car->field_6C_car_id)
                {
                    pEntry->field_14_enable_state = 0;
                }
            }
            break;

        case 4:
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                pPed = pSprite->AsCharB4_40FEA0()->get_ped_433A20();
                pCmd = reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd);
                if (pCmd->field_2_type == SCRCMD_THREAD_DECLARE3)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_10_char_idx));
                }
                else if (pCmd->field_2_type == SCRCMD_THREAD_DECLARE5)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_24_char_idx));
                }
                else
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_idx));
                }
                if (pPed->get_id() == pPointer->field_8_char->get_id())
                {
                    pEntry->field_14_enable_state = 0;
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

        case 3:
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                pPed = pSprite->AsCharB4_40FEA0()->get_ped_433A20();
                if (pPed)
                {
                    pCmd = reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd);
                    if (pCmd->field_2_type == SCRCMD_CHAR_AREA_ANY_MEANS)
                    {
                        pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_24_char_idx));
                    }
                    else
                    {
                        pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_idx));
                    }
                    if (pPed->get_id() == pPointer->field_8_char->get_id())
                    {
                        pEntry->field_14_enable_state = 0;
                    }
                }
            }
            else if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->field_8_car_bc_ptr;
                pPed = pCar->GetDriver_4118B0();
                if (pPed)
                {
                    pCmd = reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd);
                    if (pCmd->field_2_type == SCRCMD_CHAR_AREA_ANY_MEANS)
                    {
                        pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_24_char_idx));
                    }
                    else
                    {
                        pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_8_idx));
                    }
                    if (pPed->get_id() == pPointer->field_8_char->get_id())
                    {
                        pEntry->field_14_enable_state = 0;
                    }
                }
            }
            break;

        case 5:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->field_8_car_bc_ptr;
                pPed = pCar->GetDriver_4118B0();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd)->field_8_idx));
                    if (pPed->get_id() == pPointer->field_8_char->get_id())
                    {
                        pEntry->field_14_enable_state = 0;
                    }
                }
            }
            break;

        case 7:
            // Same ped check as case 6 (written out in full as in 9.6f; VC6 cross-jumps it into case 6)
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                pPed = pSprite->AsCharB4_40FEA0()->get_ped_433A20();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd)->field_8_idx));
                    if (pPed->GetGameObjectVelocity_433C20() == kZero_705DD8 &&
                        pPed->get_id() == pPointer->field_8_char->get_id())
                    {
                        pEntry->field_14_enable_state = 0;
                    }
                }
            }
            break;

        case 6:
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                pPed = pSprite->AsCharB4_40FEA0()->get_ped_433A20();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd)->field_8_idx));
                    if (pPed->GetGameObjectVelocity_433C20() == kZero_705DD8 &&
                        pPed->get_id() == pPointer->field_8_char->get_id())
                    {
                        pEntry->field_14_enable_state = 0;
                    }
                }
            }
            else if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->field_8_car_bc_ptr;
                pPed = pCar->GetDriver_4118B0();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd)->field_8_idx));
                    if (pPed->GetPedVelocity_45C920() == kZero_705DD8 &&
                        pPed->get_id() == pPointer->field_8_char->get_id())
                    {
                        pEntry->field_14_enable_state = 0;
                    }
                }
            }
            break;

        case 8:
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                pCar = pSprite->field_8_car_bc_ptr;
                pPed = pCar->GetDriver_4118B0();
                if (pPed)
                {
                    pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd)->field_8_idx));
                    if (pPed->GetPedVelocity_45C920() == kZero_705DD8 &&
                        pPed->get_id() == pPointer->field_8_char->get_id())
                    {
                        pEntry->field_14_enable_state = 0;
                    }
                }
            }
            break;
    }

    if (pEntry->field_14_enable_state == 0)
    {
        switch (pEntry->field_C_action)
        {
            case 2:
                pCmd = reinterpret_cast<CarThreadTriggerCmd*>(pEntry->field_0_pScriptCmd);
                pCmd->field_8_thread = gScriptManager_6F8060->SpawnThread_5120C0(pCmd->field_E_thread_start_cmd, 0);
                break;
            case 3:
                gScriptManager_6F8060->SetGeneratorTypeForCar_511A70(pCar->field_84_car_info_idx, reinterpret_cast<Generator*>(pEntry->field_0_pScriptCmd));
                FreeTrigger_5C8680(pEntry->field_10_remap_rng);
                break;
        }
    }
}

MATCH_FUNC(0x47bd00)
CarRemapRng::CarRemapRng()
{
    if (gbRngRemapTableDone_679C0A == false)
    {
        gbRngRemapTableDone_679C0A = true;
        for (u16 i = 0; i < 1000; i++)
        {
            gRngRemapTable_679320[i] = i;
        }

        for (u16 j = 0; j < 1000; j++)
        {
            u16 idx = gRng_6F6784.get_int_4F7AE0(1000);
            s16 next = gRngRemapTable_679320[j];
            gRngRemapTable_679320[j] = gRngRemapTable_679320[idx];
            gRngRemapTable_679320[idx] = next;
        }
    }
    field_0_table_idx = 0;
}

MATCH_FUNC(0x47bd90)
void CarRemapRng::IncNextRngRemapIdx_47BD90()
{
    field_0_table_idx++;
    if (field_0_table_idx == 1000)
    {
        field_0_table_idx = 0;
    }
}

MATCH_FUNC(0x444980)
void CarManager::DistributeCarsByRating_444980()
{
    u16 i = 0;
    gBadCarModelsCount_6772EC = 0;
    gAverageCarModelsCount_6772DC = 0;
    gGoodCarModelsCount_677294 = 0;

    for (; i < 256; i++)
    {
        if (gGtx_0x106C_703DD4->does_car_exist(static_cast<u8>(i)))
        {
            if (bStartNetworkGame_7081F0 || gGtx_0x106C_703DD4->IsCarModelInRecycleList_5AB380(static_cast<u8>(i)))
            {
                car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(static_cast<u8>(i));

                if (pInfo->rating != 99)
                {
                    u16 r_mod_10 = pInfo->rating % 10;
                    u16 idx_type;

                    if (pInfo->rating < 10)
                    {
                        idx_type = 1;
                    }
                    else if (pInfo->rating < 20)
                    {
                        idx_type = 2;
                    }
                    else if (pInfo->rating < 30)
                    {
                        idx_type = 3;
                    }

                    if (idx_type == 1)
                    {
                        for (; r_mod_10 > 0; r_mod_10--)
                        {
                            gBadCarModels_676988.values[gBadCarModelsCount_6772EC++] = pInfo->model;
                        }
                    }
                    else if (idx_type == 2)
                    {
                        for (; r_mod_10 > 0; r_mod_10--)
                        {
                            gAverageCarModels_676DB4.values[gAverageCarModelsCount_6772DC++] = pInfo->model;
                        }
                    }
                    else if (idx_type == 3)
                    {
                        for (; r_mod_10 > 0; r_mod_10--)
                        {
                            gGoodCarModels_677384.values[gGoodCarModelsCount_677294++] = pInfo->model;
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x444ab0)
u32 CarManager::SelectTrafficCarModel_444AB0(Player* pPlayer, gmp_zone_info* pZoneInfo, Fix16 density_scale, u16* pOut)
{
    s32 result;
    Fix16 density_calc = Fix16(pZoneInfo->field_0_car_density) / Fix16(1000);
    if (density_calc == gFix16_6777CC)
    {
        *pOut = 0;
        return car_model_enum::none;
    }
    else
    {
        u16 density = (Fix16(100) * (density_scale * density_calc)).ToInt();
        if (density > 99)
        {
            density = 99;
        }

        pPlayer->field_682_traffic_spawn_threshold = 100 - density;
        if (pPlayer->field_680_traffic_spawn_counter < static_cast<u16>(100 - density))
        {
            *pOut = 0;
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
                goodcar_ratio = pZoneInfo->field_2_goodcar_ratio;
                badcar_ratio = pZoneInfo->field_4_badcar_ratio;
                policecar_ratio = pZoneInfo->field_6_policecar_ratio;
                gangcar_ratio = pZoneInfo->field_8_gangcar_ratio;
            }
            else
            {
                goodcar_ratio = 300;
                policecar_ratio = 100;
                badcar_ratio = 300;
                gangcar_ratio = 0;
            }

            u16 value = field_0_model_rng.GetRngValue_4212D0();

            field_0_model_rng.IncNextRngRemapIdx_47BD90();

            if (value < goodcar_ratio)
            {
                do
                {
                    result = gGoodCarModels_677384.values[field_9_good_car_idx++];
                    if (field_9_good_car_idx == gGoodCarModelsCount_677294)
                    {
                        field_9_good_car_idx = 0;
                    }
                } while (result == field_C_suppressed_model);
                *pOut = 1;
            }
            else
            {
                if (value < goodcar_ratio + badcar_ratio)
                {
                    do
                    {
                        result = gBadCarModels_676988.values[field_A_bad_car_idx++];
                        if (field_A_bad_car_idx == gBadCarModelsCount_6772EC)
                        {
                            field_A_bad_car_idx = 0;
                        }
                    } while (result == field_C_suppressed_model);
                    *pOut = 2;
                }
                else
                {
                    if (value < badcar_ratio + goodcar_ratio + policecar_ratio)
                    {
                        result = 12;
                        *pOut = 4;
                    }
                    else
                    {
                        if (value < badcar_ratio + goodcar_ratio + policecar_ratio + gangcar_ratio)
                        {
                            result = car_model_enum::none;
                            *pOut = 5;
                        }
                        else
                        {
                            do
                            {
                                result = gAverageCarModels_676DB4.values[field_B_average_car_idx++];
                                if (field_B_average_car_idx == gAverageCarModelsCount_6772DC)
                                {
                                    field_B_average_car_idx = 0;
                                }
                            } while (result == field_C_suppressed_model);
                            *pOut = 3;
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
bool CarManager::NoRefs_444CE0(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return 1;
}

MATCH_FUNC(0x444cf0)
Car* CarManager::SpawnCarAtRoadDirection_444CF0(s32 car_model_type, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{

    u8 tile_x = xpos.ToInt();
    u8 tile_y = ypos.ToInt();
    u8 z = zpos.ToInt();
    Car* pCar = 0;

    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(tile_x, tile_y, z - 1);

    if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(4, pBlock))
    {
        pCar = gCarManager_6C_677930->SpawnCarOnRoadNetwork_4458B0(Fix16(tile_x), kFpHalf_677218 + Fix16(tile_y), 4, car_model_type);
    }
    else if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(2, pBlock))
    {
        pCar = gCarManager_6C_677930->SpawnCarOnRoadNetwork_4458B0(kFpHalf_677218 + Fix16(tile_x), kFP16One_6777D0 + Fix16(tile_y), 2, car_model_type);
    }
    else if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(3, pBlock))
    {
        pCar = gCarManager_6C_677930->SpawnCarOnRoadNetwork_4458B0(Fix16(tile_x), kFpHalf_677218 + Fix16(tile_y), 3, car_model_type);
    }
    else if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(1, pBlock))
    {
        pCar = gCarManager_6C_677930->SpawnCarOnRoadNetwork_4458B0(kFpHalf_677218 + Fix16(tile_x), Fix16(tile_y), 1, car_model_type);
    }

    return pCar;
}

MATCH_FUNC(0x444E40)
bool Car::SnapCarToGreenArrow_444E40(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    const u8 x_int = xpos.ToInt();
    const u8 y_int = ypos.ToInt();
    const u8 z_int = zpos.ToInt();
    gmp_block_info* pBlockInfo = gMap_0x370_6F6268->GetBlock_4DFE10(x_int, y_int, z_int - 1);

    if (pBlockInfo)
    {

        if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(4, pBlockInfo))
        {
            return Car::TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0(Fix16(x_int), (kFpHalf_677218 + (y_int)), 4);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(2, pBlockInfo))
        {
            return Car::TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0((kFpHalf_677218 + (x_int)), (kFP16One_6777D0 + (y_int)), 2);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(3, pBlockInfo))
        {
            return Car::TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0(Fix16(x_int), (kFpHalf_677218 + (y_int)), 3);
        }

        if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(1, pBlockInfo))
        {
            return Car::TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0((kFpHalf_677218 + (x_int)), Fix16(y_int), 1);
        }
    }
    return 0;
}

MATCH_FUNC(0x445EC0)
bool Car::TrySnapCarToNearestDrivableRoadAndDriveForward_445EC0(Fix16 xpos, Fix16 ypos, s32 direction)
{

    s32 time_out_counter = 0;
    s32 search_z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(xpos, ypos).ToInt();
    while (1)
    {
        gmp_block_info* pBlock = gMap_0x370_6F6268->FindNonAirBlockAtOrBelowZ_4E4CB0(xpos.ToInt(), ypos.ToInt(), search_z);
        ++search_z;
        if (pBlock)
        {
            if (GET_BLOCK_TYPE(pBlock->field_B_slope_type) == ROAD)
            {
                if (!gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, direction))
                {
                    if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::up_1))
                    {
                        direction = road_direction::up_1;
                    }
                    else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::right_3))
                    {
                        direction = road_direction::right_3;
                    }
                    else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::down_2))
                    {
                        direction = road_direction::down_2;
                    }
                    else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::left_4))
                    {
                        direction = road_direction::left_4;
                    }
                    else
                    {
                        return 0;
                    }
                }

                if (!gGame_0x40_67E008->IsPointOnScreen_4B9A80(xpos, ypos))
                {
                    Fix16 pos_z = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(xpos, ypos, search_z);
                    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
                    Fix16 w_val = gPixelsToFix16_6F6850.list[pInfo->w];
                    Fix16 h_val = gPixelsToFix16_6F6850.list[pInfo->h];
                    SwapIf3or4_41FE40(direction, w_val, h_val);
                    Fix16_Rect rect;
                    rect.SetRect_41E350(xpos - w_val, w_val + xpos, ypos - h_val, h_val + ypos);
                    rect.SetHiLowZ_41E370(pos_z - kFP16One_6777D0, kFP16One_6777D0 + pos_z);
                    if (!gSpriteGrid_1_679208->CheckRectForCollisions_477F60(&rect, 0, sprite_types_enum::none_0, 0) && !rect.IsRectBlockedByMap_59DE80())
                    {
                        field_50_car_sprite->set_xyz_lazy_420600(xpos, ypos, pos_z);

                        field_50_car_sprite->set_ang_lazy_420690(ReturnAngleFromRoadDirection_4F7940(direction));

                        if (field_58_physics)
                        {
                            field_58_physics->SnapVelocityToSpriteDirection_563590(field_50_car_sprite);
                            field_58_physics->Accelerate_421210();
                            field_58_physics->field_92_is_hand_brake_on = 0;
                            field_58_physics->field_AD_turn_direction = car_turn_direction::none_0;
                        }
                        return 1;
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
            return 0;
        }
    }
}

MATCH_FUNC(0x444f80)
Car* CarManager::GetNearestCarFromCoord_444F80(Fix16 x, Fix16 y, Fix16 z, Ped* pPed)
{
    return DoGetNearestCarFromCoord_444FC0(x, y, z, 1, pPed, 1);
}

MATCH_FUNC(0x444fa0)
Car* CarManager::GetNearestEnterableCarFromCoord_444FA0(Fix16 x, Fix16 y, Fix16 z, Ped* pPed)
{
    return DoGetNearestCarFromCoord_444FC0(x, y, z, 0, pPed, 0);
}

// 9.6f 0x424BD0
MATCH_FUNC(0x444FC0)
Car* CarManager::DoGetNearestCarFromCoord_444FC0(Fix16 xpos,
                                                Fix16 ypos,
                                                Fix16 zpos,
                                                s32 bMatchDriverless,
                                                Ped* pPed,
                                                char_type bIgnorePedRestrictions)
{
    // 9.6f calls sub_421D80 and sub_421DF0 here, which are HasSpriteZoom_43A230 and IsCarInAir_43A3C0 (not inlined in 10.5)
    Fix16 smallestDist = Fix16(0x300);
    Car* pNearestCar = 0;
    for (Car* pCarIter = gCar_BC_Pool_67792C->GetFirstCar_420E50(); pCarIter; pCarIter = pCarIter->mpNext)
    {
        if (!pCarIter->IsMaxDamage_40F890() && !pCarIter->IsTrailerInfo_421640() &&
            (bIgnorePedRestrictions || !pCarIter->IsDoorLockedForPed_43B2B0(pPed)) && !pCarIter->HasSpriteZoom_43A230() && !pCarIter->IsDeactivated_4214D0() &&
            !pCarIter->IsCarInAir_43A3C0() && pCarIter->GetCarInfoIdx_411940() != car_model_enum::TRAINFB &&
            (!bMatchDriverless || !pCarIter->field_54_driver))
        {
            Fix16 currentDistance = Fix16::Abs(pCarIter->field_50_car_sprite->field_14_xy.x - xpos) +
                Fix16::Abs(pCarIter->field_50_car_sprite->field_14_xy.y - ypos) + Fix16::Abs(pCarIter->field_50_car_sprite->field_1C_zpos - zpos);
            if (currentDistance < smallestDist)
            {
                smallestDist = currentDistance;
                pNearestCar = pCarIter;
            }

            if (pCarIter->IsTrainModel_403BA0())
            {
                u8 train_car_idx = 0;
                Car** pTrainCars = gPublicTransport_181C_6FF1D4->GetCarArrayFromLeadCar_579B40(pCarIter);
                for (Car* pTrainIter = *pTrainCars; pTrainIter; pTrainIter = pTrainCars[train_car_idx])
                {
                    if (train_car_idx >= 2)
                    {
                        break;
                    }

                    // The Abs negates and the last add go out of line by the inline budget. Add_408660 is the
                    // nothrow copy: the plain operator+ changes the register allocation of the whole loop
                    Fix16 trainDistance;
                    trainDistance = (Fix16::Abs(pTrainIter->field_50_car_sprite->field_14_xy.x - xpos) +
                                     Fix16::Abs(pTrainIter->field_50_car_sprite->field_14_xy.y - ypos))
                                        .Add_408660(Fix16::Abs(pTrainIter->field_50_car_sprite->field_1C_zpos - zpos));
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
Car* CarManager::GetNearestFrontVehicle_445210(Sprite* pSprite, u8 k3)
{
    Fix16 oldx = pSprite->field_14_xy.x;
    Fix16 oldy = pSprite->field_14_xy.y;
    Fix16 oldz = pSprite->field_1C_zpos;

    gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(pSprite);

    Fix16 new_x;
    Fix16 new_y;
    Ang16::PolarToCartesian_41FC20(pSprite->field_0_rotation, kFpHalf_6772D0, new_x, new_y);

    pSprite->set_xyz_lazy_420600(pSprite->field_14_xy.x + new_x, pSprite->field_14_xy.y + new_y, pSprite->field_1C_zpos);

    Sprite* pNearest =
        gSpriteGrid_1_679208->FindNearestSprite_SpiralSearch_477C90(sprite_types_enum::car_2, sprite_types_enum::car_2, pSprite, k3, 3, 1u);

    pSprite->set_xyz_lazy_420600(oldx, oldy, oldz);

    gSpriteGrid_1_679208->AddToRegionBuckets_477B20(pSprite);

    if (pNearest)
    {
        // Extract the car pointer first
        Car* pCar = pNearest->field_8_car_bc_ptr;

        if (pCar->IsTrain_421610())
        {
            Car* pLeadCar = gPublicTransport_181C_6FF1D4->GetLeadTrainCar_57B540(pNearest->field_8_car_bc_ptr);
            if (pLeadCar->GetCarLinearSpeed_43A240() == gFix16_6777CC)
            {
                return pCar;
            }
        }
        else
        {
            return pCar;
        }
    }
    return 0;
}

MATCH_FUNC(0x4453E0)
Car* CarManager::SpawnBusAtValidRoadPosition_4453E0(Fix16 x, Fix16 y, s32 road_direction, const s32& car_model)
{
    s32 found_z;
    s32 attempts = 0;
    gmp_block_info* pHighestBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
    do
    {
        gmp_block_info* pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
        if (pBlock)
        {
            if (GET_BLOCK_TYPE(pBlock->field_B_slope_type) == 1 && !gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction))
            {
                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::up_1))
                {
                    road_direction = road_direction::up_1;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::right_3))
                {
                    road_direction = road_direction::right_3;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::down_2))
                {
                    road_direction = road_direction::down_2;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::left_4))
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
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::left_4))
                            {
                                road_direction = road_direction::left_4;
                            }
                            else
                            {
                                x -= kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::right_3))
                                {
                                    road_direction = road_direction::right_3;
                                }
                            }
                            break;
                        case road_direction::down_2:
                            x += kFP16One_6777D0;
                            y += kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::left_4))
                            {
                                road_direction = road_direction::left_4;
                            }
                            else
                            {
                                x -= kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::right_3))
                                {
                                    road_direction = road_direction::right_3;
                                }
                            }
                            break;
                        case road_direction::right_3:
                            x += kFP16One_6777D0;
                            y -= kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::down_2))
                            {
                                road_direction = road_direction::down_2;
                            }
                            else
                            {
                                y += kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::up_1))
                                {
                                    road_direction = road_direction::up_1;
                                }
                            }
                            break;
                        case road_direction::left_4:
                            x -= kFP16One_6777D0;
                            y -= kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);

                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::down_2))
                            {
                                road_direction = road_direction::down_2;
                            }
                            else
                            {
                                y += kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::up_1))
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
            if (GET_BLOCK_TYPE(pBlock->field_B_slope_type) == 1 && !gGame_0x40_67E008->IsPointOnScreen_4B9A80(x, y))
            {
                Fix16 ground_z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(x, y);
                car_info* car_info_5AA3B0 = gGtx_0x106C_703DD4->get_car_info_5AA3B0(car_model);

                Fix16 sprite_width = gPixelsToFix16_6F6850.list[car_info_5AA3B0->w];
                Fix16 sprite_height = gPixelsToFix16_6F6850.list[car_info_5AA3B0->h];

                SwapIf3or4_41FE40(road_direction, sprite_width, sprite_height);

                Fix16_Rect rect;
                rect.SetRect_41E350(x - sprite_width, sprite_width + x, y - sprite_height, sprite_height + y);
                rect.SetHiLowZ_41E370(ground_z - kFP16One_6777D0, ground_z + kFP16One_6777D0);

                if (!gSpriteGrid_1_679208->CheckRectForCollisions_477F60(&rect, 0, sprite_types_enum::none_0, 0) && !rect.IsRectBlockedByMap_59DE80() &&
                    !gGame_0x40_67E008->IsRectVisibleToAnyPlayer_4B9B10(&rect))
                {
                    return CarManager::SpawnCar_426E10_v2(x, y, ground_z, ReturnAngleFromRoadDirection_4F7940(road_direction), car_model);
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
Car* CarManager::SpawnCarOnRoadNetwork_4458B0(Fix16 xpos, Fix16 ypos, s32 road_direction, s32 car_model_type)
{
    u8 unused;
    s32 found_z;
    s32 attempts = 0;
    u8 x8 = xpos.ToUInt8();
    u8 y8 = ypos.ToUInt8();
    s32 x_int = xpos.ToInt();
    s32 y_int = ypos.ToInt();
    gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x_int, y_int, &found_z);
    s16 route_idx = static_cast<char_type>(gRouteFinder_6FFDC8->FindOffScreenRoute_58A130(x_int, y_int, found_z, &unused, 1, road_direction));
    if (route_idx <= 0)
    {
        return NULL;
    }

    Junction* pJunction = gRouteFinder_6FFDC8->GetJunction_58A0B0(gRouteFinder_6FFDC8->field_2218_routes[route_idx].field_0_junctions[0]);
    u16 junction_idx = gRouteFinder_6FFDC8->field_2218_routes[route_idx].field_0_junctions[0];
    u16 next_idx = gRouteFinder_6FFDC8->field_2218_routes[route_idx].field_0_junctions[1];
    s32 dir;
    switch (static_cast<s16>(pJunction->GetDirectionToJunction_5885C0(next_idx)))
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
    gRouteFinder_6FFDC8->FindArrowBlockInJunction_5890D0(junction_idx, dir, &x8, &y8);
    Fix16 x = Fix16(x8) + kFpHalf_6772D0;
    Fix16 y = Fix16(y8) + kFpHalf_6772D0;
    road_direction = dir;

    do
    {
        gmp_block_info* pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
        if (pBlock)
        {
            if (!gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction) && GET_BLOCK_TYPE(pBlock->field_B_slope_type) == 1)
            {
                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::up_1))
                {
                    road_direction = road_direction::up_1;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::right_3))
                {
                    road_direction = road_direction::right_3;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::down_2))
                {
                    road_direction = road_direction::down_2;
                }
                else if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::left_4))
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
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::left_4))
                            {
                                road_direction = road_direction::left_4;
                            }
                            else
                            {
                                x = x - kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::right_3))
                                {
                                    road_direction = road_direction::right_3;
                                }
                            }
                            break;
                        case road_direction::down_2:
                            y = y + kFP16One_6777D0;
                            x = x + kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::left_4))
                            {
                                road_direction = road_direction::left_4;
                            }
                            else
                            {
                                x = x - kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::right_3))
                                {
                                    road_direction = road_direction::right_3;
                                }
                            }
                            break;
                        case road_direction::right_3:
                            x = x + kFP16One_6777D0;
                            y = y - kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::down_2))
                            {
                                road_direction = road_direction::down_2;
                            }
                            else
                            {
                                y = y + kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::up_1))
                                {
                                    road_direction = road_direction::up_1;
                                }
                            }
                            break;
                        case road_direction::left_4:
                            x = x - kFP16One_6777D0;
                            y = y - kFP16One_6777D0;
                            pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                            if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::down_2))
                            {
                                road_direction = road_direction::down_2;
                            }
                            else
                            {
                                y = y + kFpTwo_6777D4;
                                pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x.ToInt(), y.ToInt(), &found_z);
                                if (gRouteFinder_6FFDC8->HasBlockArrowInRoadDirection_588DE0(pBlock, green_1, road_direction::up_1))
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
            if (GET_BLOCK_TYPE(pBlock->field_B_slope_type) == 1 && !gGame_0x40_67E008->IsPointOnScreen_4B9A80(x, y))
            {
                Fix16 ground_z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(x, y);
                car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(car_model_type);

                Fix16 sprite_width = gPixelsToFix16_6F6850.list[pCarInfo->w];
                Fix16 sprite_height = gPixelsToFix16_6F6850.list[pCarInfo->h];

                SwapIf3or4_41FE40(road_direction, sprite_width, sprite_height);

                Fix16_Rect rect;
                rect.SetRect_41E350(x - sprite_width, sprite_width + x, y - sprite_height, sprite_height + y);
                rect.SetHiLowZ_41E370(ground_z - kFP16One_6777D0, ground_z + kFP16One_6777D0);

                if (!gSpriteGrid_1_679208->CheckRectForCollisions_477F60(&rect, 0, sprite_types_enum::none_0, 0) && !rect.IsRectBlockedByMap_59DE80() &&
                    !gGame_0x40_67E008->IsRectVisibleToAnyPlayer_4B9B10(&rect))
                {
                    Car* pCar =
                        SpawnCar_426E10_v2(x, y, ground_z, ReturnAngleFromRoadDirection_4F7940(road_direction), car_model_type);
                    pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
                    if (!pCar->field_5C_AI)
                    {
                        pCar->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
                    }
                    pCar->field_5C_AI->SetCar_453BF0(pCar);
                    pCar->field_5C_AI->field_28_route_idx = static_cast<char_type>(route_idx);
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
        gRouteFinder_6FFDC8->CancelRoute_589930(route_idx);
    }
    return NULL;
}

// https://decomp.me/scratch/HFGKH
// 9.6f 0x426AC0
MATCH_FUNC(0x446230)
Car* CarManager::SpawnCarAt_446230(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, Fix16 scale)
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
        pCar = gCar_BC_Pool_67792C->field_0_pool.AllocateAppend();
    }
    else if (pCarInfo->IsHeavy_41FF00())
    {
        pCar = gCar_BC_Pool_67792C->field_0_pool.AllocateAppend();
    }
    else
    {
        pCar = gCar_BC_Pool_67792C->field_0_pool.Allocate();
    }

    //pCar->PoolAllocate(); // PoolAllocate

    pCar->field_84_car_info_idx = car_info_idx;
    pCar->field_88_despawn_status = car_despawn_status::active_1;

    pCar->field_50_car_sprite = gSpritePool_703818->get_new_sprite();
    pCar->field_50_car_sprite->set_xyz_lazy_420600(xpos, ypos, zpos);

    // Sprite* pCarSprite = pNewSprite;
    gMap_0x370_6F6268->UpdateZFromSlopeAtCoord_4E5BF0(pCar->field_50_car_sprite->field_14_xy.x,
                                                      pCar->field_50_car_sprite->field_14_xy.y,
                                                      pCar->field_50_car_sprite->field_1C_zpos);
    pCar->field_50_car_sprite->set_ang_lazy_420690(rotation);
    pCar->field_50_car_sprite->SetType_4206F0(sprite_types_enum::car_2);
    pCar->field_50_car_sprite->set_id_lazy_4206C0(pCarInfo->sprite);
    pCar->field_50_car_sprite->SetCar_420710(pCar);
    pCar->field_50_car_sprite->AllocCollisionShape_59F950(gPixelsToFix16_6F6850.list[pCarInfo->w], gPixelsToFix16_6F6850.list[pCarInfo->h], kFpHalf_6771FC);

    pCar->field_68_scale = scale;

    if (scale != kFP16One_6777D0)
    {
        pCar->field_50_car_sprite->ApplyScaleToDimensions_59E4C0(pCar->field_68_scale, 1);
    }

    gSpriteGrid_1_679208->AddToRegionBuckets_477B20(pCar->field_50_car_sprite);

    pCar->InitDoors_4435A0();

    pCar->field_5C_AI = 0;
    pCar->field_64_pTrailer = 0;

    if (pCar->HasBottomLeftRoofLight_421680() && !pCar->IsGt24640_4217D0())
    {
        pCar->SetBottomLeftRoofLightDelta_421810();
    }

    if (pCar->IsCopCar_421790())
    {
        pCar->AddFlags_421890(car_flag::Jammed);
    }
    else if (pCar->IsFireTruck_4118F0())
    {
        pCar->PutWaterCannonOnRoof_440AC0();
        gWeapon_8_707018->allocate_5E3D50(weapon_type::fire_truck_gun, 50u, pCar);

        // shared flag set
        pCar->AddFlags_421890(car_flag::Jammed);
    }
    else if (pCar->IsTank_411900())
    {
        pCar->AddFlags_421890(car_flag::Jammed);
        pCar->PutTankCannonOnRoof_440B10();
        //        pCar->field_78_flags |= 0x0508u;
        pCar->SetBulletproof_4218C0();
        pCar->SetFlameproof_4218D0();
        pCar->SetFlag3_4218A0();
        gWeapon_8_707018->allocate_5E3D50(weapon_type::tank_main_gun, 20u, pCar);
    }
    else if (pCar->IsGunJeep_411910())
    {
        pCar->PutMachineGunOnRoof_440B60();
        gWeapon_8_707018->allocate_5E3D50(weapon_type::army_gun_jeep, 20u, pCar);
    }
    else if (pCar->IsTvVan_4217E0())
    {
        pCar->PutTV_Antenna_440BB0();
    }
    else if (pCar->IsTrainModel_403BA0())
    {
        pCar->AddFlags_421890(car_flag::NoCollide);
        pCar->SetFlag6_4218B0();
    }
    else if (pCar->IsGt24640_4217D0())
    {
        pCar->SetCarGraphic_43CDF0(field_0_model_rng.GetRngValue_4212D0() % 11);
    }
    else if (pCar->IsFbiCar_411920())
    {
        pCar->field_50_car_sprite->SetRemap(2);

        // shared flag set
        pCar->AddFlags_421890(car_flag::Jammed);
    }

    pCar->SetSirens_4441B0();
    pCar->UpdateDrawLayer_4435F0();
    pCar->field_50_car_sprite->CreateSoundObj_5A29D0();
    return pCar;
}

MATCH_FUNC(0x408370)
EXPORT void __stdcall SpawnCabAndTrailerHelper_408370(
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
    Fix16_Point hitch = gTrailerHitchOffset_66AAC8;
    Fix16_Point cab_offset = gTrailerCabOffset_66AAE0;

    hitch.RotateByAngle_40F6B0_out_of_line(rot);
    hitch_pos = pos.Add_40AC50(hitch);

    cab_offset.RotateByAngle_40F6B0_all_out_of_line(rot);
    // 9.6f 0x40F760: negate in place
    cab_offset.x = cab_offset.x.Negate_4086A0();
    cab_offset.y = cab_offset.y.Negate_4086A0();
    cab_offset.RotateByAngle_40F6B0_all_out_of_line(uknown_rot);
    trailer_pos = hitch_pos.Add_40AC50(cab_offset);

    *pOutZ = zpos;
    *pOutX = trailer_pos.x;
    *pOutY = trailer_pos.y;
    // 9.6f 0x40F640 (unary -) then 0x40F790 (angle of the vector)
    *pOutRot = cab_offset.Negate_40ACB0().atan2_40F790();
}

// 9.6f 0x428EC0
MATCH_FUNC(0x446530)
Trailer* CarManager::SpawnCabAndTrailer_446530(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_model, s32 trailer_model)
{
    Ang16 out_rot;

    Car* pCab = SpawnCarAtCorrectZ_426E40(xpos, ypos, rotation, car_model);

    // Separate output locals: VC6 puts them in the dead xpos/ypos arg slots and keeps the
    // parameters in registers across the first spawn
    Fix16 trailer_x;
    Fix16 trailer_y;
    Fix16 zpos;
    SpawnCabAndTrailerHelper_408370(xpos,
                                    ypos,
                                    pCab->field_50_car_sprite->field_1C_zpos,
                                    rotation,
                                    kAngZero_67791C,
                                    &trailer_x,
                                    &trailer_y,
                                    &zpos,
                                    &out_rot);

    Car* pTrailer = SpawnCarAtCorrectZ_426E40(trailer_x, trailer_y, out_rot, trailer_model);
    gCar_BC_Pool_67792C->field_0_pool.UnlinkFromActiveList_420F30(pTrailer);

    Trailer* pNewTrailer = gTrailerPool_66AC80->field_0_pool.Allocate();
    pNewTrailer->SetTruckCabAndTrailerCar_407BB0(pCab, pTrailer);

    return pNewTrailer;
}

MATCH_FUNC(0x446730)
void CarManager::RemoveFromPoolAndCollision_446730(Car* pCar)
{
    gSpriteGrid_3_679210->Remove_477B00(pCar->field_50_car_sprite);
    gCar_BC_Pool_67792C->Remove(pCar);
}

MATCH_FUNC(0x4466c0)
void CarManager::DecrementAllocatedCarType_4466C0(s32 car_kind)
{
    switch (car_kind)
    {
        case 1:
            --this->field_28_recycled_cars;
            break;

        case 2:
            --this->field_40_proto_recycled_cars;
            break;

        case 10:
            --this->field_48_kind10_cars;
            break;

        case 4:
            --this->field_2C_paramedic_cars;
            break;

        case 5:
            --this->field_30_firefighter_cars;
            break;

        case 6:
            --this->field_34_unit_cars;
            break;

        case 7:
            --this->field_38_roadblock_cars;
            break;

        case 8:
            --this->field_3C_mission_cars;
            break;
        case 9:
            --this->field_44_parked_cars;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x446760)
void CarManager::EnableFreeShoppingIfAllKFsPassed_446760()
{
    // Enable free shopping once all KF's are passed
    s32* pSecretsPassed = gScriptManager_6F8060->field_338_secrets_passed;
    if (pSecretsPassed)
    {
        const u32 total_secrets = gScriptManager_6F8060->field_318_total_secrets;
        if (total_secrets > 0)
        {
            if (!this->field_69_do_free_shopping && static_cast<u32>(*pSecretsPassed) >= total_secrets)
            {
                this->field_69_do_free_shopping = 1;
            }
        }
    }
}

MATCH_FUNC(0x446790)
void CarManager::CarsService_446790()
{
    EnableFreeShoppingIfAllKFsPassed_446760();

    gTaxi_4_704130->PopAll_457BC0();

    field_55_visible_cars_count = 0;

    gCar_BC_Pool_67792C->field_0_pool.UpdatePool();

    gTrafficSpawner_14_677934->GenerateTraffic_583670();

    if (field_54_spawn_block > 0)
    {
        field_54_spawn_block--;
    }
}

MATCH_FUNC(0x446870)
bool CarManager::CanAllocateCarOfKindWithSpare_446870(s32 type)
{
    switch (type)
    {
        case car_kind::firefighter_5:
            return this->field_30_firefighter_cars < 1;
        case car_kind::paramedic_car_4:
            return this->field_2C_paramedic_cars < 1;
        case car_kind::police_6:
            return this->field_34_unit_cars < 5;
        case car_kind::roadblock_car_7:
            return this->field_38_roadblock_cars < 11;
        case car_kind::Unknown_10:
            return this->field_48_kind10_cars < 11;
        case car_kind::parked_car_9:
            return this->field_44_parked_cars < 200;
        case car_kind::recycled_1:
            return gCarManager_6C_677930->field_28_recycled_cars + gCarManager_6C_677930->field_40_proto_recycled_cars < 15;
        case car_kind::mission_car_8:
            return this->field_3C_mission_cars < 23;
        default:
            return false;
    }
}

MATCH_FUNC(0x446930)
bool CarManager::CanAllocateCarOfKind_446930(s32 type)
{
    switch (type)
    {
        case car_kind::firefighter_5:
            return this->field_30_firefighter_cars < 2;
        case car_kind::paramedic_car_4:
            return this->field_2C_paramedic_cars < 2;
        case car_kind::police_6:
            return this->field_34_unit_cars < 6;
        case car_kind::roadblock_car_7:
            return this->field_38_roadblock_cars < 12;
        case car_kind::Unknown_10:
            return this->field_48_kind10_cars < 12;
        case car_kind::parked_car_9:
            return this->field_44_parked_cars < 200;
        case car_kind::recycled_1:
            return gCarManager_6C_677930->field_28_recycled_cars + gCarManager_6C_677930->field_40_proto_recycled_cars < 16;
        case car_kind::mission_car_8:
            return this->field_3C_mission_cars < 24;
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

    field_64_zpos = gFix16_6777CC;
    field_60_impact_mode = 0;
    field_4_spare_driver = 0;
    field_14_next_car_id = 1;
    field_9_good_car_idx = 0;
    field_8_unused = 0;
    field_A_bad_car_idx = 0;
    field_B_average_car_idx = 0;
    field_1A_fire_effect_cycle = 0;
    field_1C_explosion_offset_cycle = 0;
    field_28_recycled_cars = 0;
    field_40_proto_recycled_cars = 0;
    field_30_firefighter_cars = 0;
    field_2C_paramedic_cars = 0;
    field_34_unit_cars = 0;
    field_38_roadblock_cars = 0;
    field_3C_mission_cars = 0;
    field_44_parked_cars = 0;

    stru_67727C.ResetHead_4207E0();
    stru_67737C.ResetHead_4207E0();

    ClearCarInfoGlobals_5639C0();
    ResetBlockedRoadBrakeTimer_447640();

    field_20_light_state_a = 4;
    field_24_light_state_b = 1;

    if (!gSprite_Unused_677938)
    {
        gSprite_Unused_677938 = new Sprite(kAngZero_67791C, gFix16_6777CC);
        if (!gSprite_Unused_677938)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8375);
        }
    }

    // On version 9.6f this is call to a class method
    // that initialises the next two fields
    //field_4C = 0;
    //field_50_tv_van_dir = 0;
    field_4C_tv_van_dir.x = Fix16(0);
    field_4C_tv_van_dir.y = Fix16(0);
    field_54_spawn_block = 0;
    field_55_visible_cars_count = 0;
    field_58_model_to_check_destroy = car_model_enum::none;
    field_5C_model_check_destroyed = 0;
    field_C_suppressed_model = car_model_enum::none;
    field_69_do_free_shopping = bDo_free_shopping_67D6CD;
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
    if (gSprite_Unused_677938 != NULL)
    {
        Sprite* p = gSprite_Unused_677938;
        gSprite_Unused_677938->~Sprite();
        operator delete(p);
        gSprite_Unused_677938 = 0;
    }

    field_4_spare_driver = 0;
}

MATCH_FUNC(0x4451E0)
bool Car::CanBeEnteredByPed_4451E0(Ped* pPed)
{
    Ped* pDriver = this->field_54_driver;
    if (pDriver && !this->IsMediCar_421780() &&
        pDriver->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
    {
        return false;
    }
    return IsDoorLockedForPed_43B2B0(pPed) ? false : true;
}

MATCH_FUNC(0x445360)
bool Car::IsEnterable_445360()
{
    if (!this->IsMaxDamage_40F890() && !IsTrailerInfo_421640() &&
        !IsDoorLockedForPed_43B2B0(gSpriteGrid_ped_678F64) && !HasSpriteZoom_43A230() && !IsDeactivated_4214D0() && !IsCarInAir_43A3C0())
    {
        // Not TRAIN (0x3B) itself, only the carriages
        if (field_84_car_info_idx != car_model_enum::boxcar && field_84_car_info_idx != car_model_enum::TRAINFB &&
            field_84_car_info_idx != car_model_enum::TRAINCAB)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x43D5D0)
EXPORT s16 Car::ApplyImpactDamage_43D5D0(Fix16 damage)
{
    if ((field_84_car_info_idx == car_model_enum::TAXI || field_84_car_info_idx == car_model_enum::STYPECAB) && damage > kFpTwo_6777D4)
    {
        // Big taxi crash, time for the passengers to do a runner
        field_4_passengers_list.ForceTaxiPassengersToExit_471680();
    }

    if (!(this->field_78_flags & car_flag::Flag3))
    {

        return AccumulateDamage_43DA90((damage * 200).ToInt(), &kZeroPoint_6778A8);
    }

    return 0;
}

MATCH_FUNC(0x4403a0)
Ang16 Car::GetCornerAngle_4403A0()
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    Fix16 w_fp = Fix16(pCarInfo->w) / 2;
    Fix16 h_fp = Fix16(pCarInfo->h) / 2;
    return Ang16(Fix16::atan2_fixed_405320(h_fp, w_fp).rValue);
}

MATCH_FUNC(0x439ec0)
bool Car::IsPoliceCar_439EC0()
{
    bool b = field_84_car_info_idx == car_model_enum::COPCAR || field_84_car_info_idx == car_model_enum::SWATVAN ||
        field_84_car_info_idx == car_model_enum::EDSELFBI;
    return b;
}

// https://decomp.me/scratch/I0oG6
MATCH_FUNC(0x439ee0)
Fix16 Car::GetDamageFactorOnSpeed_439EE0()
{
    if (field_74_damage >= 16000)
    {
        return (Fix16((32001 - field_74_damage))) / Fix16(16000);
    }
    else
    {
        return Fix16(kFP16One_6777D0);
    }
}

MATCH_FUNC(0x439f30)
Fix16 Car::GetMaxSpeed_439F30()
{
    ModelPhysics* pModelPhysics = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_84_car_info_idx);
    // Max speed is limited by how smashed up the car is
    return GetDamageFactorOnSpeed_439EE0() * pModelPhysics->field_28_max_speed;
}

MATCH_FUNC(0x439f80)
wchar_t* Car::GetCarStr_439F80()
{
    sprintf(gTmpGxtKey_67CE50, "car%d%c%c", field_84_car_info_idx, 0, 0);
    return gText_0x14_704DFC->Find_5B5F90(gTmpGxtKey_67CE50);
}

// 9.6f 0x421C40
MATCH_FUNC(0x439fb0)
Fix16_Point Car::GetHitchPoint_439FB0()
{
    Fix16_Point point;
    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    if ((pInfo->info_flags & car_info_flag::trailer) == car_info_flag::trailer)
    {
        point = kTrailerHitchOffset_677370;
    }
    else
    {
        point = kCabHitchOffset_677358;
    }

    point.RotateByAngle_40F6B0(field_50_car_sprite->field_0_rotation);
    return point + field_50_car_sprite->get_x_y_443580();
}

MATCH_FUNC(0x43a0e0)
Fix16_Point Car::GetRearWheelOffset_43A0E0()
{
    CarInfo_2C* pInfo = gCarInfo_808_678098->GetInfoAtIdx_454840(field_84_car_info_idx);
    return Fix16_Point(Fix16(0), pInfo->field_8_rear_wheel_offset);
}

MATCH_FUNC(0x43a120)
Fix16 Car::GetMass_43A120()
{
    ModelPhysics* pCarInfo = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_84_car_info_idx);

    if (HasPlayerDriver_4215C0())
    {
        return ApplyScale_421910(pCarInfo->field_4_mass) * (kFP16One_6777D0 + kFpPoint05_6777A0);
    }
    else
    {
        return ApplyScale_421910(pCarInfo->field_4_mass);
    }
}

MATCH_FUNC(0x43a1d0)
Fix16 Car::GetAntiStrength_43A1D0()
{
    return gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(field_84_car_info_idx)->field_2C_anti_strngth;
}

MATCH_FUNC(0x43a1f0)
bool Car::IsBus_43A1F0()
{
    if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(this))
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x43a230)
bool Car::HasSpriteZoom_43A230()
{
    return field_50_car_sprite->field_38_zoom != 0;
}

// CarPhysics::GetLinearSpeed_4211A0 with the square root forced inline (see
// Fix16_Point::GetLength_SqrtForced_43A240)
static inline Fix16 GetLinearSpeed_43A240(CarPhysics* pPhysics)
{
    return pPhysics->field_40_linvel_1.GetLength_SqrtForced_43A240();
}

MATCH_FUNC(0x43a240)
Fix16 Car::GetCarLinearSpeed_43A240()
{
    CarPhysics* pPhysics;
    if (IsTrainModel_403BA0())
    {
        pPhysics = gPublicTransport_181C_6FF1D4->GetLeadTrainCar_57B540(this)->field_58_physics;
        if (!pPhysics)
        {
            return gFix16_6777CC;
        }
        return GetLinearSpeed_43A240(pPhysics);
    }
    else
    {
        pPhysics = this->field_58_physics;
        if (!pPhysics)
        {
            return gFix16_6777CC;
        }
        return GetLinearSpeed_43A240(pPhysics);
    }
}

MATCH_FUNC(0x43a3c0)
bool Car::IsCarInAir_43A3C0()
{
    if (!field_58_physics)
    {
        return false;
    }
    return field_58_physics->field_98_surface_type == car_surface_type::air_surface_6;
}

// 9.6f 0x421E00
MATCH_FUNC(0x43a3e0)
Ang16 Car::GetOrientationAngle_43A3E0()
{
    if (!field_58_physics)
    {
        return field_50_car_sprite->field_0_rotation;
    }
    else
    {
        return field_58_physics->GetLinvelAngle_4211E0();
    }
}

MATCH_FUNC(0x43a450)
Fix16_Point Car::GetLinearVelocity_43A450()
{
    if (IsTrainModel_403BA0())
    {
        Car* carObj = gPublicTransport_181C_6FF1D4->GetLeadTrainCar_57B540(this);
        if (!carObj->field_58_physics)
        {
            return kZeroPoint_6778A8;
        }
        else
        {
            return carObj->field_58_physics->get_linvel_447010();
        }
    }
    else
    {
        if (!this->field_58_physics)
        {
            return kZeroPoint_6778A8;
        }
        else
        {
            return this->field_58_physics->get_linvel_447010();
        }
    }
}

MATCH_FUNC(0x43a4c0)
Fix16 Car::GetVelocity_43A4C0()
{
    if (!field_58_physics)
    {
        return 0;
    }
    else
    {
        return field_58_physics->field_0_vel_read_only.GetLength_41E260();
    }
}

MATCH_FUNC(0x43a590)
Fix16 Car::GetMomentOfInertia_43A590()
{
    return gCarInfo_808_678098->GetInfoAtIdx_454840(GetCarModelForPhysics_43A850())->field_0_moment_of_inertia;
}

MATCH_FUNC(0x43a5b0)
Fix16 Car::GetMinDimension_43A5B0()
{
    const car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    if (pInfo->w < pInfo->h)
    {
        Fix16 t = gPixelsToFix16_6F6850.list[pInfo->w];
        return t;
    }
    else
    {
        Fix16 tt = gPixelsToFix16_6F6850.list[pInfo->h];
        return tt;
    }
}

MATCH_FUNC(0x43a600)
void Car::RemoveAllDamage()
{
    RepairDamage_43D400();
    if (field_64_pTrailer)
    {
        field_64_pTrailer->field_C_trailer_car->RepairDamage_43D400();
    }
}

MATCH_FUNC(0x43a680)
bool Car::AllowResprayOrPlates()
{
    return (!IsTrainModel_403BA0() && (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & car_info_flag::emergency) != car_info_flag::emergency &&
            !IsFbiCar_411920() && !field_0_attachments.GetGangIdxFromSpriteIfAny_5A71A0() && !IsArmyModel(field_84_car_info_idx));
}

MATCH_FUNC(0x43a6f0)
bool Car::IsNotCurrentRemap(u8 remap)
{
    return gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->has_remaps() && field_50_car_sprite->get_remap_41C1F0() != remap;
}

MATCH_FUNC(0x43a730)
bool Car::IsNotCurrentRemapOfCarAndTrailerCar(u8 remap)
{
    if (field_64_pTrailer)
    {
        // Check trailer car
        return IsNotCurrentRemap(remap) || field_64_pTrailer->field_C_trailer_car->IsNotCurrentRemap(remap);
    }
    return IsNotCurrentRemap(remap);
}

MATCH_FUNC(0x43a780)
void Car::SetCarRemap(u8 remap)
{
    if (IsNotCurrentRemap(remap))
    {
        field_50_car_sprite->SetRemap(remap);
    }

    if (field_64_pTrailer)
    {
        if (field_64_pTrailer->field_C_trailer_car->IsNotCurrentRemap(remap))
        {
            field_64_pTrailer->field_C_trailer_car->field_50_car_sprite->SetRemap(remap);
        }
    }
}

MATCH_FUNC(0x43a7d0)
void Car::AssignRandomRemap_43A7D0()
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    if (pCarInfo->num_remaps)
    {
        const u32 remap_idx = gCarManager_6C_677930->field_10_remap_rng.GetRngValue_4212D0() % (pCarInfo->num_remaps + 1);
        if (remap_idx == pCarInfo->num_remaps)
        {
            field_50_car_sprite->SetPaletteSprites_420700();
        }
        else
        {
            field_50_car_sprite->SetRemap(pCarInfo->remap[remap_idx]);
        }

        gCarManager_6C_677930->field_10_remap_rng.IncNextRngRemapIdx_47BD90();
    }
}

// https://decomp.me/scratch/zoRIL
MATCH_FUNC(0x43a850)
char_type Car::GetCarModelForPhysics_43A850()
{
    if (HasNonPlayerDriver_4214F0())
    {
        if (!IsTrainModel_403BA0())
        {
            if (field_60_pChaseTask)
            {
                if (field_60_pChaseTask->field_22_following_route != 0)
                {
                    if (field_84_car_info_idx == car_model_enum::TANK)
                    {
                        return car_model_enum::HOTDOG_D4;
                    }
                    return car_model_enum::HOTDOG_D1;
                }
            }
            else
            {
                if (field_84_car_info_idx == car_model_enum::TANK)
                {
                    return car_model_enum::HOTDOG_D4;
                }
                return car_model_enum::HOTDOG_D1;
            }

            switch (field_84_car_info_idx)
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
        return this->field_84_car_info_idx;
    }

    if (this->field_68_scale < kFP16One_6777D0)
    {
        return car_model_enum::HOTDOG_D3;
    }

    if (field_84_car_info_idx == car_model_enum::TRUKCAB2 && !this->field_64_pTrailer)
    {
        return car_model_enum::TRUKCAB1;
    }

    if (field_84_car_info_idx == car_model_enum::TRUKCAB1 && this->field_64_pTrailer)
    {
        return car_model_enum::TRUKCAB2;
    }

    return this->field_84_car_info_idx;
}

MATCH_FUNC(0x43a950)
void Car::DoBrake_43A950()
{
    CarPhysics* pCarPhysics = field_58_physics;
    pCarPhysics->field_91_is_foot_brake_on = 1;
    pCarPhysics->field_93_is_forward_gas_on = 0;
    pCarPhysics->field_94_is_backward_gas_on = 0;
    pCarPhysics->field_95_is_coasting = 0;
}

MATCH_FUNC(0x43a970)
void Car::DoBrakeAndHandbrake_43A970()
{
    field_58_physics->field_92_is_hand_brake_on = 1;
    CarPhysics* pCarPhysics = field_58_physics;
    pCarPhysics->field_91_is_foot_brake_on = 1;
    pCarPhysics->field_93_is_forward_gas_on = 0;
    pCarPhysics->field_94_is_backward_gas_on = 0;
    pCarPhysics->field_95_is_coasting = 0;
}

MATCH_FUNC(0x43a9a0)
void Car::SetDriver(Ped* pNewDriver)
{
    char hand_brake_on;
    if (!pNewDriver)
    {
        CarPhysics* pCarPhysics = this->field_58_physics;
        if (pCarPhysics)
        {
            Ped* pOldDriver = this->field_54_driver;
            if (pOldDriver && pOldDriver->field_15C_player)
            {
                hand_brake_on = pCarPhysics->get_is_hand_brake_on_421270();
            }
            else
            {
                hand_brake_on = 1;
            }
            pCarPhysics->HandleUserInputs_55A860(0, 0, 0, 0, hand_brake_on);
        }
    }
    this->field_54_driver = pNewDriver;
}

MATCH_FUNC(0x43a9f0)
void Car::TriggerCarAlarm_43A9F0()
{
    if (!field_54_driver && ((field_78_flags & car_flag::CarAlarm)) && field_7C_ped_type != ped_type::player_2 && !field_A4_bf.bLightFlashing && field_74_damage != 32001)
    {
        StartLightFlashing_43CAC0();
    }
}

MATCH_FUNC(0x43CAC0)
void Car::StartLightFlashing_43CAC0()
{
    if ((field_A4_light_flags & kEmergencyOrFlashingLightsMask) == 0)
    {
        field_A5_flash_phase_counter = 12;
        field_A4_bf.bLightFlashing = true;

        if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2))
        {
            if (HasPopupHeadlights_421700())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            }
            else
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
            }
        }

        if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3))
        {
            if (HasPopupHeadlights_421700())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            }
            else
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
            }
        }

        if (HasRoofLight_421660())
        {
            field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
        }

        if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
        {
            field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
        }

        if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
        {
            field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
        }

        field_8E_flash_count = 50;
    }
}

// Not in IDA's function list: tail called from Ped::BecomeDummyOnPlayerDisconnect_470300
MATCH_FUNC(0x43AA20)
void Car::SetDummyControl_43AA20()
{
    const s32 info_idx = field_84_car_info_idx;
    if (info_idx != car_model_enum::TRAIN && info_idx != car_model_enum::TRAINCAB && info_idx != car_model_enum::TRAINFB &&
        info_idx != car_model_enum::boxcar)
    {
        SetDriverPedType_421560(ped_type::dummy_3);
        CarPhysics* pCarPhysics = field_58_physics;
        if (pCarPhysics)
        {
            pCarPhysics->SetField8C_to_1();
        }
    }
}

MATCH_FUNC(0x43aa60)
void Car::Deactivate_43AA60()
{
    gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(field_50_car_sprite);
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_50_car_sprite);
    gCar_BC_Pool_67792C->field_0_pool.UnlinkFromActiveList_420F30(this);
    SetDeactivatedStatus_4214E0();
}

MATCH_FUNC(0x43AAE0)
bool Car::IsThreatToSearchingPed_43AAE0()
{
    Ped* pDriver = this->field_54_driver;
    if (!pDriver)
    {
        return 0;
    }
    return pDriver->IsThreatToSearchingPed_4661F0();
}

MATCH_FUNC(0x43aaf0)
bool Car::CanCarCollideWithSprite_43AAF0(Sprite* pSprite)
{

    s32 sprite_type;
    Car* pCar;
    u16 our_flags;
    u16 their_flags;
    bool bCanCollide;
    s32 despawn_status;
    Object* pObject;
    ObjectDefinition* pDefinition;
    s32 phi_type;
    s32 field_18_model;
    Ped* pPed;
    Char* pChar;
    Ped* pCharPed;
    Trailer* pTrailer;

    if (pSprite)
    {
        if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car_2)
        {
            pCar = pSprite->field_8_car_bc_ptr;
            if (pCar)
            {
                if (gPublicTransport_181C_6FF1D4->AreTrainsOnDifferentTracks_57B740(pCar, this))
                {
                    return 0;
                }

                our_flags = this->field_78_flags;
                their_flags = pCar->field_78_flags;
                if ((static_cast<u8>(their_flags) & car_flag::Flag4) != (static_cast<u8>(our_flags) & car_flag::Flag4))
                {
                    return 0;
                }
                if ((their_flags & car_flag::Flag6) != 0 && this->field_74_damage == 32001 && (our_flags & car_flag::Flag6) == 0 ||
                    (our_flags & car_flag::Flag6) != 0 && pCar->field_74_damage == 32001 && (their_flags & car_flag::Flag6) == 0)
                {
                    return 0;
                }
                if ((their_flags & car_flag::Crushing) != 0 && (gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->info_flags_2 & car_info_flag2::heavy) != car_info_flag2::heavy ||
                    (this->field_78_flags & car_flag::Crushing) &&
                        (gGtx_0x106C_703DD4->get_car_info_5AA3B0(pCar->field_84_car_info_idx)->info_flags_2 & car_info_flag2::heavy) != car_info_flag2::heavy)
                {
                    return 0;
                }

                if (this->field_64_pTrailer)
                {
                    return 1;
                }

                if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->info_flags & car_info_flag::truck_cab) == car_info_flag::truck_cab && !pCar->field_64_pTrailer &&
                    (gGtx_0x106C_703DD4->get_car_info_5AA3B0(pCar->field_84_car_info_idx)->info_flags & car_info_flag::trailer) == car_info_flag::trailer)
                {
                    return field_50_car_sprite->CheckDirectionalSliceCollision_59E680(kFpFiveEighths_6778C8, pSprite);
                }

                if (this->field_64_pTrailer || (gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->info_flags & car_info_flag::trailer) != car_info_flag::trailer ||
                    pCar->field_64_pTrailer || (gGtx_0x106C_703DD4->get_car_info_5AA3B0(pCar->field_84_car_info_idx)->info_flags & car_info_flag::truck_cab) != car_info_flag::truck_cab)
                {
                    return 1;
                }

                bCanCollide = pSprite->CheckDirectionalSliceCollision_59E680(kFpFiveEighths_6778C8, this->field_50_car_sprite);
                if (!HasSpriteZoom_43A230() && this->field_74_damage != 32001)
                {
                    despawn_status = this->field_88_despawn_status;
                    if (despawn_status != car_despawn_status::despawn_pending_2 && despawn_status != car_despawn_status::marked_for_despawn_4 && despawn_status != car_despawn_status::despawn_soon_3 && despawn_status != car_despawn_status::despawning_5)
                    {
                        stru_67727C.PushSprite_5A6D40(this->field_50_car_sprite);
                    }
                }
                return bCanCollide;
            }
        }

        sprite_type = pSprite->field_30_sprite_type_enum;
        if ((sprite_type == sprite_types_enum::code_obj1_4 || sprite_type == sprite_types_enum::map_obj_5 || sprite_type == sprite_types_enum::object_1) && (pObject = pSprite->field_8_object_2C_ptr) != 0)
        {
            pDefinition = pObject->field_8_definition;
            phi_type = pDefinition->field_34_behavior_type;
            if (phi_type == object_behavior_type::pickup_6 || phi_type == object_behavior_type::pickup_physics_7 || phi_type == object_behavior_type::pickup_animated_8 ||
            phi_type == object_behavior_type::pickup_physics_animated_9 || phi_type == object_behavior_type::shop_10 || phi_type == object_behavior_type::static_collision_check_1 || phi_type == object_behavior_type::effect_only_12 ||
                (field_18_model = pObject->field_18_model, field_18_model == 182) || field_18_model == objects::grenade_obj_183)
            {
                if (pDefinition->field_40_collision_bucket_category == collision_bucket_category::sprite_grid_2_region_3)
                {
                    return 0;
                }
            }
            else
            {
                if (gPedRefTable_7F8_703398->field_0_entries[pObject->field_26_ped_ref_idx].field_0_ped_id)
                {
                    pPed = gPedManager_6787BC->PedById(gPedRefTable_7F8_703398->field_0_entries[pObject->field_26_ped_ref_idx].field_0_ped_id);
                    if (pPed)
                    {
                        if (pPed->field_16C_car == this)
                        {
                            return 0;
                        }
                    }
                }
            }
        }
        else if (sprite_type == sprite_types_enum::ped_3)
        {
            pChar = pSprite->field_8_char_b4_ptr;
            if (pChar)
            {
                pCharPed = pChar->field_7C_pPed;
                if (pCharPed->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6 && pCharPed->field_154_target_to_enter == this)
                {
                    return 0;
                }
            }
        }
        return 1;
    }
    else
    {
        if (gCollisionCounters_6791FC->field_8_bDoorAccessCheck == 1)
        {
            pTrailer = this->field_64_pTrailer;
            if (pTrailer)
            {
                if (pTrailer->field_C_trailer_car == this)
                {
                    return 0;
                }
            }
        }
        return 1;
    }
}

MATCH_FUNC(0x43adc0)
void Car::ProcessCarToCarImpact_43ADC0(Sprite* pSprite)
{
    s32 score_base_multi = 0;
    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        switch (gCarManager_6C_677930->field_60_impact_mode)
        {
            case 1:
            {
                if (!this->IsMaxDamage_40F890())
                {
                    if (!Car::IsAreaDamaged_43D1C0(3))
                    {
                        Car::DamageArea_43CF30(3);
                        score_base_multi = 1;
                    }
                    if (!Car::IsAreaDamaged_43D1C0(2))
                    {
                        Car::DamageArea_43CF30(2);
                        ++score_base_multi;
                    }
                    if (!Car::IsAreaDamaged_43D1C0(0))
                    {
                        Car::DamageArea_43CF30(0);
                        ++score_base_multi;
                    }
                    if (!Car::IsAreaDamaged_43D1C0(1))
                    {
                        Car::DamageArea_43CF30(1);
                        ++score_base_multi;
                    }
                }

                s32 score = 1000 * score_base_multi;

                if (pCar->IsTank_411900())
                {
                    score += Car::ApplyImpactDamage_43D5D0(kFpEight_677918);
                }

                if (score > 200)
                {
                    Ped* pDriver = pCar->GetDriver_4118B0();
                    if (pDriver && pDriver->is_player_41B0A0())
                    {
                        pDriver->field_15C_player->field_2D4_scores.AwardCarDamageScore_593030(this, score);
                    }
                }

                field_0_attachments.CleanupSpriteList_5A7080();
                if (field_58_physics)
                {
                    field_58_physics->set_linvel_421170(kZeroPoint_6778A8);
                }
                stru_67737C.AddSprite_5A6CD0(field_50_car_sprite);
                break;
            }

            case 2:
                if (field_50_car_sprite->field_1C_zpos > gCarManager_6C_677930->field_64_zpos)
                {
                    gCarManager_6C_677930->field_64_zpos = field_50_car_sprite->field_1C_zpos;
                    // NOTE: returned here originally
                }
                break;
        }
    }
    field_0_attachments.DispatchCarImpactEvents_5A6BF0(pSprite);
}

MATCH_FUNC(0x43af10)
bool Car::CanExitCar_43AF10()
{
    if (HasSpriteZoom_43A230())
    {
        return false;
    }

    if (field_58_physics)
    {
        return field_58_physics->IsNearlyStopped_5636E0();
    }

    return true;
}

MATCH_FUNC(0x43af40)
void Car::ResumeAIDriving_43AF40()
{
    if (field_5C_AI)
    {
        field_5C_AI->field_18_target_speed = kFpPoint1_6778E0;
        ClearAIHalted_421550();
    }
}

MATCH_FUNC(0x43af60)
void Car::HaltAIDriving_43AF60()
{
    if (field_5C_AI)
    {
        SetAIHalted_421540();
    }
}

MATCH_FUNC(0x43afe0)
bool Car::IsDoorAccessible_43AFE0(u8 target_door)
{
    Fix16 y;
    Fix16 x;
    Fix16_Rect fr;

    GetDoorWorldPosition_43B5A0(target_door, &x, &y);
    if (gPathFinder_6FDEF0->TestMoveToAdjacentTile_5540E0(field_50_car_sprite->field_14_xy.x.ToInt(),
                                                   field_50_car_sprite->field_14_xy.y.ToInt(),
                                                   field_50_car_sprite->field_1C_zpos.ToInt(),
                                                   x.ToInt(),
                                                   y.ToInt()))
    {
        gDoorAccessCheckActive_6F8EDC = 1;
        fr.ComputeCollisionPrism_4204D0(x, y, kFpPoint2_6772CC, field_50_car_sprite->field_1C_zpos);
        fr.ExpandToIncludePoint_59DEE0(field_50_car_sprite->field_14_xy.x, field_50_car_sprite->field_14_xy.y);
        gCollisionCounters_6791FC->field_8_bDoorAccessCheck = 1;
        if (!fr.IsRectBlockedByMap_59DE80() &&
            !gSpriteGrid_1_679208->CheckRectForCollisions_477F60(&fr, 1, sprite_types_enum::ped_3, field_50_car_sprite) && !gMap_0x370_6F6268->RectHitsDiagonalWall_4E11E0(&fr))
        {
            gCollisionCounters_6791FC->field_8_bDoorAccessCheck = 0;
            gDoorAccessCheckActive_6F8EDC = 0;
            return 1;
        }
        gDoorAccessCheckActive_6F8EDC = 0;
        gCollisionCounters_6791FC->field_8_bDoorAccessCheck = 0;
    }
    return 0;
}

// 9.6f 0x425B60
MATCH_FUNC(0x43b140)
bool Car::IsStoppedWithPavementAtDoor_43B140(u8 target_car_door)
{
    Ang16 angToUse;

    u8 remap = GetDoorCount_43B360();
    if (GetCarLinearSpeed_43A240() > gFix16_6777CC)
    {
        return 0;
    }

    Fix16 door_x;
    Fix16 door_y;
    GetDoorWorldPosition_43B5A0(target_car_door, &door_x, &door_y);

    if (remap != 3)
    {
        switch (target_car_door)
        {
            case 0:
            case 2:
                angToUse = field_50_car_sprite->field_0_rotation + kAng90_677234;
                break;
            case 1:
            case 3:
                angToUse = field_50_car_sprite->field_0_rotation - kAng90_677234;
                break;
            default:
                break;
        }
    }
    else
    {
        angToUse = field_50_car_sprite->field_0_rotation - kAng90_677234;
    }

    Fix16 t1;
    Fix16 t2;
    Ang16::PolarToCartesian_41FC20(angToUse, kFpHalf_6778FC, t1, t2);

    t1 += door_x;
    t2 += door_y;
    if (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(t1.ToInt(), t2.ToInt(), field_50_car_sprite->field_1C_zpos.ToInt() - 1) != 2)
    {
        return false;
    }
    return true;
}

MATCH_FUNC(0x43b2b0)
bool Car::IsDoorLockedForPed_43B2B0(Ped* pPed)
{
    // Returning the comparison chains (not 0/1 constants) gives the original's 'xor %eax,%eax'/'mov $1,%eax'
    if (pPed)
    {
        if (field_98_door_lock == Car_Door_Lock::lockout_player_5)
        {
            return pPed->PedTypeIs_45EDE0(ped_type::player_2);
        }
        if (!pPed->PedTypeIs_45EDE0(ped_type::player_2) &&
            !(pPed->PedTypeIs_45EDE0(ped_type::script_created_5) && pPed->GetInternalObjective_403A90() == objectives_enum::enter_car_as_driver_35 && pPed->get_target_to_enter_403B10() == this))
        {
            return field_98_door_lock == Car_Door_Lock::locked_1 || field_98_door_lock == Car_Door_Lock::locked_permanently_4 || field_98_door_lock == Car_Door_Lock::lockout_thief_2;
        }
    }
    return field_98_door_lock == Car_Door_Lock::locked_1 || field_98_door_lock == Car_Door_Lock::locked_permanently_4;
}

MATCH_FUNC(0x43b340)
Car_Door* Car::GetDoor(u8 door_idx)
{
    return &field_C_doors[door_idx];
}

MATCH_FUNC(0x43b360)
char_type Car::GetDoorCount_43B360()
{
    return *gGtx_0x106C_703DD4->get_car_door_data_5AA3D0(field_84_car_info_idx);
}

MATCH_FUNC(0x43b380)
void Car::OpenAllDoors_43B380()
{
    u8* pRemap = gGtx_0x106C_703DD4->get_car_door_data_5AA3D0(field_84_car_info_idx);
    for (u8 i = 0; i < *pRemap; i++)
    {
        Car_Door* pDoor = GetDoor(i);
        pDoor->Open_439E60();
    }
}

MATCH_FUNC(0x43b3d0)
void Car::CloseAllDoors_43B3D0()
{
    u8* pRemap = gGtx_0x106C_703DD4->get_car_door_data_5AA3D0(field_84_car_info_idx);
    for (u8 i = 0; i < *pRemap; i++)
    {
        Car_Door* pDoor = GetDoor(i);
        pDoor->Close_439EA0();
    }
}

MATCH_FUNC(0x43b420)
void Car::GetDoorWorldPos_43B420(u8 door_idx, Fix16* pOutX, Fix16* pOutY)
{
    door_info* pDoor = reinterpret_cast<door_info*>(gGtx_0x106C_703DD4->get_car_door_data_5AA3D0(field_84_car_info_idx) + 1) + door_idx;
    Fix16 door_x = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pDoor->rx);
    Fix16 door_y = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pDoor->ry);

    // Ang16::RotateVector_41FC90 written out: old_xpos copy and an Ang16& to the sprite angle as in the
    // inline, the new x kept in x_off, and the y line through the out-of-line Multiply_408680 and
    // operator+ (0x408660) like Trailer::GetHitchPosition_407BD0. An Ang16 copy of the angle swaps which of
    // sin/cos stays in a register.
    Fix16 old_xpos = door_x;
    Ang16& rotation = field_50_car_sprite->field_0_rotation;
    Fix16 x_off;
    x_off = door_x * Ang16::cosine_40F520(rotation) + door_y * Ang16::sine_40F500(rotation);
    door_y = (-old_xpos).Multiply_408680(Ang16::sine_40F500(rotation)).Add_408660(door_y.Multiply_408680(Ang16::cosine_40F520(rotation)));

    *pOutX = field_50_car_sprite->field_14_xy.x + x_off;
    *pOutY = field_50_car_sprite->field_14_xy.y + door_y;
}

MATCH_FUNC(0x43B540)
bool Car::IsDoorOffsetLarge_43B540(u8 targetDoor)
{
    s8* array = reinterpret_cast<s8*>(gGtx_0x106C_703DD4->get_car_door_data_5AA3D0(field_84_car_info_idx));
    if (Fix16::Abs(gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(array[2 * targetDoor + 1])) >= kFpOne_677908)
    {
        return true;
    }
    return false;
}

// Ang16::RotateVector_41FC90 with the new x built in a local and stored last: with the header
// version VC6 folds the new x into the caller's final add (GetDoorWorldPosition_43B5A0)
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
void Car::GetDoorWorldPosition_43B5A0(u8 target_door, Fix16* pOutX, Fix16* pOutY)
{
    Fix16 door_relative_xpos;
    Fix16 door_relative_ypos;

    u8* pRemap_0 = &gGtx_0x106C_703DD4->get_car_door_data_5AA3D0(field_84_car_info_idx)[2 * target_door + 1];

    door_relative_xpos = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pRemap_0[0]);
    door_relative_ypos = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(pRemap_0[1]);

    if (door_relative_xpos >= kFpOne_677908)
    {
        door_relative_xpos -= kFpOne_677908;
    }
    else
    {
        if (door_relative_xpos <= -kFpOne_677908)
        {
            door_relative_xpos += kFpOne_677908;
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

    RotateVector_41FC90_NewX(door_relative_xpos, door_relative_ypos, field_50_car_sprite->field_0_rotation);

    *pOutX = door_relative_xpos + field_50_car_sprite->field_14_xy.x;
    *pOutY = door_relative_ypos + field_50_car_sprite->field_14_xy.y;
}

MATCH_FUNC(0x43b730)
bool Car::IsOnScreenForAnyPlayer_43B730()
{
    return gGame_0x40_67E008->IsSpriteInAnyPlayerBounds_4B97E0(field_50_car_sprite, kFpFive_6778D0);
}

MATCH_FUNC(0x43b750)
bool Car::IsVisibleToAnyPlayer_43B750()
{
    return gGame_0x40_67E008->IsSpriteInBoundsOrFollowedByAnyPlayer_4B9830(field_50_car_sprite, kFpFive_6778D0);
}

MATCH_FUNC(0x43b770)
void Car::AssignKillerToOccupants_43B770()
{
    Ped* pDriver = this->field_54_driver;
    if (pDriver)
    {
        if (pDriver->get_occupation_403980() != ped_ocupation_enum::dummy_driver_4)
        {
            pDriver->field_204_killer_id = this->field_70_exploder_ped_id;
            this->field_54_driver->field_290_death_cause = this->field_90_death_cause;
            this->field_54_driver->field_264_killer_id_timer = 50;
        }
    }
    field_4_passengers_list.SyncPassengersWithCarState_4716D0(this);
}

// 9.6f 0x420C30 (a method of an unknown global object in 9.6f that only looks at its argument)
static inline bool IsPlayerPedId_420C30(u32 ped_id)
{
    return ped_id <= 12;
}

MATCH_FUNC(0x43b7b0)
void Car::AssignDriverBlameForExplosion_43B7B0(Car* pCar)
{
    char_type bBlameUs = 0;
    char_type bBlameThem = 0;
    u32 our_id = GetEffectiveDriverPedId_444090();
    u32 their_id = pCar->GetEffectiveDriverPedId_444090();
    bool bOurIsPlayer = IsPlayerPedId_420C30(our_id);
    bool bTheirIsPlayer = IsPlayerPedId_420C30(their_id);

    if (bOurIsPlayer)
    {
        if (bTheirIsPlayer && our_id != their_id)
        {
            bBlameUs = 1;
        }
    }
    else if (their_id > 0 && our_id != their_id)
    {
        bBlameUs = 1;
    }

    if (bTheirIsPlayer)
    {
        if (bOurIsPlayer && their_id != our_id)
        {
            bBlameThem = 1;
        }
    }
    else if (our_id > 0 && their_id != our_id)
    {
        bBlameThem = 1;
    }

    if (bBlameUs)
    {
        this->field_70_exploder_ped_id = their_id;
        this->field_90_death_cause = ped_death_cause::run_over_1;
        this->field_94_exploder_timer = 50;
    }
    if (bBlameThem)
    {
        pCar->field_70_exploder_ped_id = our_id;
        pCar->field_90_death_cause = ped_death_cause::run_over_1;
        pCar->field_94_exploder_timer = 50;
    }
}

MATCH_FUNC(0x43b850)
bool Car::IsImmuneToExplosionType_43B850(s32 type)
{
    // Two separate bit tests: VC6 loads the u16 once into cx and merges them into 'test $6,%ch'
    return ((field_78_flags & car_flag::Rocketproof) || (field_78_flags & car_flag::Flameproof)) && type != explosion_type::large_20 ? true : false;
}

// 9.6f 0x4226C0

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
MATCH_FUNC(0x43b870)
void Car::SpawnDamageFireEffect_43B870(s32 fire_size, const Fix16_Point* pPos)
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);

    Fix16 x_hit;
    Fix16 y_hit;
    Fix16_Point rot_point; // function scope: the original has its EH state 0 from entry

    if (pPos->IsNull_420360())
    {
        switch (static_cast<u8>(fire_size))
        {
            case 1u: // small fire
                y_hit = Get_rear_window_offset(pCarInfo);
                if (gCarManager_6C_677930->field_1A_fire_effect_cycle && gCarManager_6C_677930->field_1A_fire_effect_cycle != 2)
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
                if (gCarManager_6C_677930->field_1A_fire_effect_cycle && gCarManager_6C_677930->field_1A_fire_effect_cycle != 3)
                {
                    y_hit = Get_front_window_offset(pCarInfo);
                }
                else
                {
                    y_hit = Get_rear_window_offset(pCarInfo);
                }
                break;

            case 3u: // also a big fire but never used?
                y_hit = Get_front_window_offset(pCarInfo);
                if (gCarManager_6C_677930->field_1A_fire_effect_cycle && gCarManager_6C_677930->field_1A_fire_effect_cycle != 2)
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
        x_hit *= field_68_scale;
        y_hit *= field_68_scale;
    }
    else
    {
        rot_point = *pPos - field_50_car_sprite->get_x_y();
        rot_point.RotateByAngle_40F6B0(field_50_car_sprite->field_0_rotation);
        x_hit = rot_point.x;
        y_hit = rot_point.y;
    }

    Object* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(field_50_car_sprite->field_14_xy.x,
                                                                      field_50_car_sprite->field_14_xy.y,
                                                                      2, // implicit: the out-of-line Fix16(s32) ctor
                                                                      kAngZero_67791C,
                                                                      Car::GetFireExplosionType_43BB90(fire_size),
                                                                      field_70_exploder_ped_id);
    if (pExplosion)
    {
        field_50_car_sprite->AttachSprite_5A3100(pExplosion->field_4_sprite, x_hit, y_hit, kAngZero_67791C);
        if (++gCarManager_6C_677930->field_1A_fire_effect_cycle > 3u)
        {
            gCarManager_6C_677930->field_1A_fire_effect_cycle = 0;
        }
    }
}

MATCH_FUNC(0x43bb90)
s32 __stdcall Car::GetFireExplosionType_43BB90(u8 fire_level)
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
void Car::SpawnFire_43BBC0()
{
    if (gFirefighterPool_54_67D4C0->TryDispatchFirefightersToCar_4A8820(this))
    {
        Object* pExplosion =
            gObject_5C_6F8F84->CreateExplosion_52A3D0(field_50_car_sprite->field_14_xy.x, field_50_car_sprite->field_14_xy.y, 4, kAngZero_67791C, explosion_type::car_fire_4, field_70_exploder_ped_id);
        if (pExplosion)
        {
            field_50_car_sprite->AttachSprite_5A3100(pExplosion->field_4_sprite, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
        }
    }
}

MATCH_FUNC(0x43bc30)
void Car::SetupCarPhysicsAndSpriteBinding_43BC30()
{
    if (!field_58_physics)
    {
        AllocCarPhysics_4419E0();
        field_58_physics->SetCar_5638C0(this);
        field_58_physics->SetSprite_563560(this->field_50_car_sprite);
    }
    else
    {
        if (IsTrainModel_403BA0())
        {
            field_58_physics->SetSprite_563560(this->field_50_car_sprite);
        }
    }
}

MATCH_FUNC(0x43bca0)
void Car::SetupCarPhysicsAndSpriteBinding_43BCA0()
{
    if (field_64_pTrailer)
    {
        field_64_pTrailer->SetupCarPhysicsAndSpriteBinding_408190();
    }
    else
    {
        SetupCarPhysicsAndSpriteBinding_43BC30();
    }
}

MATCH_FUNC(0x43bd00)
void Car::DeAllocateCarPhysics_43BD00()
{
    if (field_64_pTrailer)
    {
        field_64_pTrailer->DeAllocateCarPhysics_4081B0();
    }
    else
    {
        DeAllocateCarPhysics_441A10();
    }
}

MATCH_FUNC(0x43bd40)
void Car::FlashHeadlights_43BD40()
{
    if ((field_A4_light_flags & kEmergencyOrFlashingLightsMask) == 0)
    {
        if (field_A4_bf.bHeadlights)
        {
            if (HasPopupHeadlights_421700())
            {
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);

                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
            }

            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);

            if (HasRoofLight_421660())
            {
                field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
            }
        }
        else
        {
            if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2))
            {
                if (HasPopupHeadlights_421700())
                {
                    field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
                }
                else
                {
                    field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
                }
            }

            if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3))
            {
                if (HasPopupHeadlights_421700())
                {
                    field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
                }
                else
                {
                    field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
                }
            }

            if (HasRoofLight_421660())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
            }
        }

        this->field_A5_flash_phase_counter = 6;
        this->field_A4_bf.bHeadlightFlash = true;
    }
}

MATCH_FUNC(0x43bf10)
void Car::BrakeLightsOn_43BF10()
{
    if (!this->field_A4_bf.bLightFlashing)
    {
        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
        }
        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
        }
    }
    this->field_A4_bf.bBrakeLights = true;
}

MATCH_FUNC(0x43bf70)
void Car::BrakeLightsOff_43BF70()
{
    if (!this->field_A4_bf.bLightFlashing)
    {
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
    }
    this->field_A4_bf.bBrakeLights = false;
}

MATCH_FUNC(0x43bfe0)
void Car::HeadlightsOn_43BFE0()
{
    if (!this->field_A4_bf.bLightFlashing)
    {
        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2))
        {
            if (HasPopupHeadlights_421700())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            }
            else
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
            }
        }

        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3))
        {
            if (HasPopupHeadlights_421700())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            }
            else
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
            }
        }

        if (HasRoofLight_421660())
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
        }
    }
    this->field_A4_bf.bHeadlights = true;
}

MATCH_FUNC(0x43c0c0)
void Car::HeadlightsOff_43C0C0()
{
    if (!field_A4_bf.bLightFlashing) // bit 3
    {
        if (HasPopupHeadlights_421700()) // bit 1
        {
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);

            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
        }

        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);

        if (HasRoofLight_421660()) // bit 2
        {
            field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
        }
    }

    field_A4_bf.bHeadlights = false; // bit 1
}

MATCH_FUNC(0x43c1c0)
void Car::PrepareForExplosion_43C1C0()
{
    if (this->field_A4_bf.bLightFlashing)
    {
        Car::StopLightFlashing_43CBE0();
    }

    this->field_A4_light_flags = 0;

    if (HasEmergencyLights_414F20())
    {
        Car::DeactivateEmergencyLights_43C9D0();
    }

    Car::HeadlightsOff_43C0C0();
    Car::BrakeLightsOff_43BF70();
}

MATCH_FUNC(0x43c260)
void Car::UpdateTopRightRoofLight_43C260()
{
    if (HasTopRoofLights_4216A0() && !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
    {
        this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightRoofLight_18);
        Object* pObject = field_0_attachments.FindObject2CByModel_5A6A90(172);
        pObject->UpdateLight_527A30();
    }
}

MATCH_FUNC(0x43c310)
void Car::ResetTopRightRoofLight_43C310()
{
    if (HasTopRoofLights_4216A0() && !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
    {
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightRoofLight_18);
        Object* pLight = field_0_attachments.FindObject2CByModel_5A6A90(172);
        if (pLight)
        {
            pLight->TurnOffLight_527990();
        }
    }
}

MATCH_FUNC(0x43c3c0)
void Car::UpdateTopLeftRoofLight_43C3C0()
{
    if (HasTopRoofLights_4216A0() && !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
    {
        this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftRoofLight_17);
        Object* pLight = field_0_attachments.FindObject2CByModel_5A6A90(173);
        pLight->UpdateLight_527A30();
    }
}

MATCH_FUNC(0x43c470)
void Car::ResetTopLeftRoofLight_43C470()
{
    if (HasTopRoofLights_4216A0() && !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
    {
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftRoofLight_17);
        Object* pLight = field_0_attachments.FindObject2CByModel_5A6A90(173);
        if (pLight)
        {
            pLight->TurnOffLight_527990();
        }
    }
}

MATCH_FUNC(0x43c500)
void Car::UpdateRoofLights_43C500()
{
    if (field_84_car_info_idx != car_model_enum::EDSELFBI ||
        !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11) &&
            !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12) &&
            !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13) &&
            !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
    {
        field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomRightRoofLight_16);

        Object* pObj_165 = field_0_attachments.FindObject2CByModel_5A6A90(objects::light_165);
        pObj_165->UpdateLight_527A30();
    }

    if (field_84_car_info_idx == car_model_enum::SWATVAN || field_84_car_info_idx == car_model_enum::FIRETRUK)
    {
        Object* pObj_172 = field_0_attachments.FindObject2CByModel_5A6A90(172);
        pObj_172->UpdateLight_527A30();
    }
}

MATCH_FUNC(0x43c650)
void Car::ResetRoofLights_43C650()
{
    field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomRightRoofLight_16);

    Object* pObj1 = field_0_attachments.FindObject2CByModel_5A6A90(objects::light_165);
    if (pObj1)
    {
        pObj1->TurnOffLight_527990();
    }

    if (IsSwatVan_4217A0() || IsFireTruck_4118F0())
    {
        Object* pObj2 = field_0_attachments.FindObject2CByModel_5A6A90(172);
        if (pObj2)
        {
            pObj2->TurnOffLight_527990();
        }
    }
}

MATCH_FUNC(0x43c700)
void Car::UpdateBottomLeftRoofLight_43C700()
{
    if (this->field_84_car_info_idx != car_model_enum::EDSELFBI ||
        !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11) &&
            !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12) &&
            !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13) &&
            !field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
    {
        field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }

    if (this->field_84_car_info_idx != car_model_enum::EDSELFBI)
    {
        Object* pObj_171 = field_0_attachments.FindObject2CByModel_5A6A90(171);
        pObj_171->UpdateLight_527A30();
    }

    if (field_84_car_info_idx == car_model_enum::SWATVAN || field_84_car_info_idx == car_model_enum::FIRETRUK)
    {
        Object* pObj_173 = field_0_attachments.FindObject2CByModel_5A6A90(173);
        pObj_173->UpdateLight_527A30();
    }
}

MATCH_FUNC(0x43c840)
void Car::ResetBottomLeftRoofLight_43C840()
{
    field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);

    if (field_84_car_info_idx != car_model_enum::EDSELFBI)
    {
        Object* pObject = field_0_attachments.FindObject2CByModel_5A6A90(171);
        if (pObject)
        {
            pObject->TurnOffLight_527990();
        }
    }

    if (field_84_car_info_idx == car_model_enum::SWATVAN || field_84_car_info_idx == car_model_enum::FIRETRUK)
    {
        Object* pObject = field_0_attachments.FindObject2CByModel_5A6A90(173);
        if (pObject)
        {
            pObject->TurnOffLight_527990();
        }
    }
}

MATCH_FUNC(0x43c920)
void Car::ActivateEmergencyLights_43C920()
{
    if ((field_A4_light_flags & kEmergencyOrFlashingLightsMask) == 0)
    {
        if (IsFbiCar_411920())
        {
            field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        }
        field_A4_bf.bEmergencyLights = true;
        if (IsFbiCar_411920())
        {
            field_A5_flash_phase_counter = 8;
        }
        else
        {
            field_A5_flash_phase_counter = 15;
        }
    }
}

MATCH_FUNC(0x43c9d0)
void Car::DeactivateEmergencyLights_43C9D0()
{
    Car::ResetRoofLights_43C650();
    Car::ResetBottomLeftRoofLight_43C840();
    Car::ResetTopRightRoofLight_43C310();
    Car::ResetTopLeftRoofLight_43C470();
    field_A4_bf.bEmergencyLights = false;
    if (IsFbiCar_411920() && !IsMaxDamage_40F890())
    {
        field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor4_14);
    }
}

MATCH_FUNC(0x43ca80)
void Car::ToggleEmergencyLights_43CA80()
{
    if ((field_A4_light_flags & kEmergencyOrFlashingLightsMask) != 0)
    {
        Car::DeactivateEmergencyLights_43C9D0();
    }
    else
    {
        Car::ActivateEmergencyLights_43C920();
    }
}

MATCH_FUNC(0x43cbe0)
void Car::StopLightFlashing_43CBE0()
{
    field_A4_bf.bLightFlashing = false;

    if (field_A4_bf.bHeadlights)
    {
        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2))
        {
            if (HasPopupHeadlights_421700())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            }
            else
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
            }
        }

        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3))
        {
            if (HasPopupHeadlights_421700())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            }
            else
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
            }
        }

        if (HasRoofLight_421660())
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
        }
    }
    else
    {
        if (HasPopupHeadlights_421700())
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);

            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
        }

        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
        this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);

        if (HasRoofLight_421660())
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
        }
    }

    // Braking?
    if (this->field_A4_bf.bBrakeLights)
    {
        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
        }

        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
            this->field_8E_flash_count = 0;
            return;
        }
    }
    else
    {
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
    }

    this->field_8E_flash_count = 0;
}

MATCH_FUNC(0x43cdf0)
void Car::SetCarGraphic_43CDF0(char_type graphic_idx)
{
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomRightRoofLight_16);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftRoofLight_17);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightRoofLight_18);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::Bit19_19);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::Bit20_20);

    if (graphic_idx != 10)
    {
        // Hmm... bit annoying to make this clean
        this->field_8_damaged_areas.set_bit(graphic_idx + CarDeltaBitsEnum::TopRightDoor1_11);
    }
}

MATCH_FUNC(0x43cf30)
void Car::DamageArea_43CF30(s32 damage_area)
{
    switch (damage_area)
    {
        case 0:
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
            if (HasPopupHeadlights_421700())
            {
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
            }
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomRightDamage_2);
            break;

        case 1:
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftDamage_3);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
            if (HasPopupHeadlights_421700())
            {
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
            }
            break;

        case 2:
            ResetTopRightRoofLight_43C310();
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDamage_1);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
            break;

        case 3:
            ResetTopLeftRoofLight_43C470();
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDamage_0);
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
            break;

        case 4:
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::WindshieldDamage_4);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x43d1c0)
bool Car::IsAreaDamaged_43D1C0(s32 damage_area)
{
    // damage_area is the script's damage position (miss2_0x11C), not the bit: 10.5.exe's jump table sends
    // 0 to the BottomRight test, 1 to BottomLeft, 2 to TopRight, 3 to TopLeft and 4 to the windshield
    switch (damage_area)
    {
        case 0:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2); // 4

        case 1:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3); // 8

        case 2:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1);

        case 3:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0);

        case 4:
            return field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::WindshieldDamage_4); // 10

        default:
            return false;
    }
    return false;
}

// 9.6f: the field_78_flags test is Car::sub_411930 (inlined, as a bool helper it changes the code)
MATCH_FUNC(0x43d2c0)
void Car::TryDamageArea_43D2C0(u8 damage_area, s32 damageAmount)
{
    if (!IsMaxDamage_40F890() && !(field_78_flags & car_flag::Flag3) && (GetAntiStrength_43A1D0() * Fix16(damageAmount, 0)) >= kFP16One_6777D0)
    {
        switch (damage_area)
        {
            case 0:
                DamageArea_43CF30(3);
                break;
            case 1:
                DamageArea_43CF30(2);
                break;
            case 2:
                DamageArea_43CF30(0);
                break;
            case 3:
                DamageArea_43CF30(1);
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x43d400)
void Car::RepairDamage_43D400()
{
    this->field_74_damage = 0;
    this->field_8C_damage_level = 0;

    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDamage_0);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDamage_1);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomRightDamage_2);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftDamage_3);
    this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::WindshieldDamage_4);

    if (this->field_A4_bf.bHeadlights)
    {
        if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomRightDamage_2))
        {
            if (HasPopupHeadlights_421700())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            }
            else
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
            }
        }

        if (!field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::BottomLeftDamage_3))
        {
            if (HasPopupHeadlights_421700())
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            }
            else
            {
                field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
            }
        }

        if (HasRoofLight_421660())
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
        }
    }

    if (this->field_A4_bf.bBrakeLights)
    {
        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDamage_1))
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackRightBrakeLight_5);
        }

        if (!this->field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDamage_0))
        {
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BackLeftBrakeLight_22);
        }
    }

    field_0_attachments.CleanupSpriteList_5A7080();
    ResetEngineStatus_421570();
}

MATCH_FUNC(0x43d690)
void Car::EmitExplosion_43D690(s32 type, Fix16 x, Fix16 y)
{
    Fix16_Point unused; // EH frame with state 0 before the first call: the original has a destructible local

    Object* pObject = gObject_5C_6F8F84->CreateExplosion_52A3D0(gFix16_6777CC, gFix16_6777CC, 2, kAngZero_67791C, type, field_70_exploder_ped_id);
    if (pObject)
    {
        field_50_car_sprite->AttachSprite_5A3100(pObject->field_4_sprite, x, y, kAngZero_67791C);
    }
}

MATCH_FUNC(0x43d7b0)
void Car::TriggerExplosion_43D7B0(s32 type)
{
    if (GetAntiStrength_43A1D0() != gFix16_6777CC && !this->IsMaxDamage_40F890())
    {
        HandleCarExplosion_43D840(type);
    }
    else
    {
        EmitExplosion_43D690(type, kZeroPoint_6778A8.x, kZeroPoint_6778A8.y);
    }
}

MATCH_FUNC(0x43d840)
void Car::HandleCarExplosion_43D840(s32 type)
{
    char bOcc2;
    s32 model_to_check;
    Ped* pExploder;
    Gang* pGang;

    bOcc2 = 0;
    if (field_60_pChaseTask)
    {
        gCarChaseTaskTable_678E30->FreeEntry_474CC0(field_60_pChaseTask);
        this->field_60_pChaseTask = 0;
    }

    if (!this->IsMaxDamage_40F890())
    {
        StartWreckCountdown_441380();
        EmitExplosion_43D690(type, kZeroPoint_6778A8.x, kZeroPoint_6778A8.y);
        AssignKillerToOccupants_43B770();
        field_0_attachments.ExpireAttachedFlames_5A71F0();

        if (field_54_driver)
        {
            if (field_54_driver->get_occupation_403980() == ped_ocupation_enum::dummy_driver_4)
            {
                bOcc2 = 1;
            }
        }

        KillContainedPeds_43DB80();

        ApplyExplosionImpulse_443710(&field_50_car_sprite->get_x_y());

        this->field_74_damage = 32001;

        field_0_attachments.DestroyAllAttached_5A6BB0();

        model_to_check = gCarManager_6C_677930->field_58_model_to_check_destroy;
        if (model_to_check != car_model_enum::none && model_to_check == this->field_84_car_info_idx)
        {
            gCarManager_6C_677930->field_5C_model_check_destroyed = 1;
        }

        if (this->field_70_exploder_ped_id)
        {
            pExploder = gPedManager_6787BC->PedById(this->field_70_exploder_ped_id);
            if (pExploder)
            {
                if (pExploder->PedTypeIs_45EDE0(ped_type::player_2))
                {
                    if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(this) &&
                        gPublicTransport_181C_6FF1D4->field_17C0_bus.field_56_passenger_count >= 10)
                    {
                        pExploder->field_15C_player->field_2D4_scores.AwardFullBusDestroyedScore_593410(this);
                    }
                    else
                    {
                        pExploder->field_15C_player->field_2D4_scores.AwardCarDestroyedScore_592DD0(this, pExploder);
                        if (bOcc2)
                        {
                            pExploder->field_15C_player->field_2D4_scores.AddCashWithMultiplier_593220();
                        }
                    }
                    s16 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(this->field_84_car_info_idx);
                    if (gang_idx != -1)
                    {
                        pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(static_cast<u8>(gang_idx));
                        pGang->ApplyKillRespectChange_4BEF70(pExploder->field_15C_player->get_idx_4219D0(), 1u);
                    }
                    if (pExploder->is_player_41B0A0())
                    {
                        if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime_485090(this, pExploder->field_15C_player))
                        {
                            if (pExploder->field_20A_wanted_points < cop_level_ped_enum::cop_1_stars)
                            {
                                pExploder->field_20A_wanted_points = cop_level_ped_enum::cop_1_stars;
                            }
                            else
                            {
                                pExploder->add_wanted_points_470160(200);
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
s16 Car::AccumulateDamage_43DA90(s16 damage, const Fix16_Point* pVec)
{
    if (IsMaxDamage_40F890())
    {
        return 0;
    }

    Fix16 anti_s;
    anti_s = GetAntiStrength_43A1D0();
    Fix16 t = (anti_s * Fix16(damage));
    s16 damage_1 = t.ToInt();

    if (damage_1 == 0)
    {
        if (anti_s != gFix16_6777CC)
        {
            damage_1 = 1;
        }
    }

    this->field_74_damage += damage_1;
    if (this->field_74_damage < 0 || this->field_74_damage > 32000)
    {
        this->field_74_damage = 32000;
    }

    if (this->field_74_damage >= 16000)
    {
        if (this->field_8C_damage_level < 3)
        {
            Car::SpawnDamageFireEffect_43B870(1, pVec);
            this->field_8C_damage_level = 3;
        }
        if (this->field_74_damage >= 25000)
        {
            if (this->field_8C_damage_level < 4)
            {
                field_0_attachments.ExpireAttachedFlames_5A71F0();
                Car::SpawnDamageFireEffect_43B870(2, pVec);
                this->field_8C_damage_level = 4;
            }
            if (this->field_74_damage >= 31500)
            {
                Car::PrepareForExplosion_43C1C0();
            }
            if (this->field_74_damage == 32000)
            {
                Car::HandleCarExplosion_43D840(explosion_type::item_19);
            }
        }
    }
    return damage_1;
}

MATCH_FUNC(0x43db80)
void Car::KillContainedPeds_43DB80()
{
    Ped* pDriver;
    CarPhysics* pPhysics;

    pDriver = this->field_54_driver;
    if (pDriver)
    {
        if (pDriver->get_occupation_403980() != ped_ocupation_enum::dummy_driver_4)
        {
            if (pDriver->is_player_41B0A0())
            {
                pPhysics = this->field_58_physics;
                if (pPhysics)
                {
                    pPhysics->SetField8C_to_1();
                }
            }
            field_54_driver->Kill_46F9D0();
        }
        this->field_54_driver = 0;
    }
    field_4_passengers_list.KillAndClearAllPedsResetCarRef_4715E0();
}

MATCH_FUNC(0x43dbd0)
void Car::StopMovement_43DBD0()
{
    CarPhysics* pPhysics = this->field_58_physics;
    if (pPhysics)
    {
        pPhysics->field_40_linvel_1 = kZeroPoint_6778A8;
        this->field_58_physics->field_74_ang_vel_rad = gFix16_6777CC;
    }
}

MATCH_FUNC(0x43dc00)
bool Car::IsSpriteShrunk_43DC00()
{
    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    if (field_74_damage == 32001)
    {
        SpriteShape* pShape = field_50_car_sprite->field_C_pCollisionShape;
        SpriteShape* pLen = field_50_car_sprite->field_4_pVisualShape;
        Fix16 shapeWidth = pShape->field_0_width;
        Fix16 lenWidth = pLen->field_0_width;
        return (lenWidth != shapeWidth) != 0;
    }

    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::car_2, pInfo->sprite);
    sprite_index* pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_idx);
    Fix16 lenWidth = field_50_car_sprite->field_4_pVisualShape->field_0_width;
    return lenWidth.mValue != gPixelsToFix16_6F6850.list[pSpriteIndex->field_4_width].mValue;
}

MATCH_FUNC(0x43dc80)
bool Car::CarShrinkSprite_43DC80(s32 xoff, s32 yoff)
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    Fix16 w_fp(pCarInfo->w << 14, 0);
    Fix16 h_fp(pCarInfo->h << 14, 0);
    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::car_2, pCarInfo->sprite);
    sprite_index* pSpriteIndex = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_idx);
    Fix16 sprite_w = gPixelsToFix16_6F6850.list[pSpriteIndex->field_4_width];
    Fix16 sprite_h = gPixelsToFix16_6F6850.list[pSpriteIndex->field_5_height];
    return field_50_car_sprite->ShrinkSprite_59E390((sprite_w * xoff) / w_fp, (sprite_h * yoff) / h_fp, 0);
}

MATCH_FUNC(0x43dd50)
bool Car::IsBeingCrushed_43DD50()
{
    return gCrusherPool_94_67A830->IsCarBeingCrushed_4887A0(this);
}

MATCH_FUNC(0x43dd60)
void Car::DestroyWhileSinking_43DD60()
{
    bool bOcc2;
    Ped* field_54_driver;
    s32 exploder_ped_id;
    Ped* pExploderPed;
    Ped* pPed;
    Gang* pGang;
    s32 model_to_check;

    bOcc2 = 0;

    StopMovement_43DBD0();
    field_50_car_sprite->IncreaseZoom_59E320(1);
    field_0_attachments.SinkAllAttachedCars_5A6BD0();

    if (CarShrinkSprite_43DC80(2, 2))
    {
        field_54_driver = this->field_54_driver;
        if (field_54_driver)
        {
            if (field_54_driver->get_occupation_403980() == ped_ocupation_enum::dummy_driver_4)
            {
                bOcc2 = 1;
            }
        }
        exploder_ped_id = this->field_70_exploder_ped_id;
        if (exploder_ped_id)
        {
            pExploderPed = gPedManager_6787BC->PedById(exploder_ped_id);
            pPed = pExploderPed;
            if (pExploderPed)
            {
                if (pExploderPed->PedTypeIs_45EDE0(ped_type::player_2))
                {
                    if (gPublicTransport_181C_6FF1D4->is_bus_579AA0(this) &&
                        gPublicTransport_181C_6FF1D4->field_17C0_bus.field_56_passenger_count >= 10)
                    {
                        pPed->field_15C_player->field_2D4_scores.AwardFullBusDestroyedScore_593410(this);
                    }
                    else
                    {
                        pPed->field_15C_player->field_2D4_scores.AwardCarDestroyedScore_592DD0(this, pPed);
                        if (bOcc2)
                        {
                            pPed->field_15C_player->field_2D4_scores.AddCashWithMultiplier_593220();
                        }
                    }
                    s16 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(this->field_84_car_info_idx);
                    if (gang_idx != -1)
                    {
                        pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(static_cast<u8>(gang_idx));
                        pGang->ApplyKillRespectChange_4BEF70(pPed->field_15C_player->get_idx_4219D0(), 1u);
                    }
                    if (pPed->is_player_41B0A0())
                    {
                        if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime_485090(this, pPed->field_15C_player))
                        {
                            if (pPed->field_20A_wanted_points < cop_level_ped_enum::cop_1_stars)
                            {
                                pPed->field_20A_wanted_points = cop_level_ped_enum::cop_1_stars;
                            }
                            else
                            {
                                pPed->add_wanted_points_470160(200);
                            }
                        }
                    }
                }
            }
        }

        model_to_check = gCarManager_6C_677930->field_58_model_to_check_destroy;
        if (model_to_check != car_model_enum::none && model_to_check == this->field_84_car_info_idx)
        {
            gCarManager_6C_677930->field_5C_model_check_destroyed = 1;
        }

        AssignKillerToOccupants_43B770();
        KillContainedPeds_43DB80();

        SetEngineDamaged_421590();
        PrepareForExplosion_43C1C0();
        field_0_attachments.DestroyAllSprites_5A7010();
        field_50_car_sprite->set_z_lazy_420660(gFix16_6777CC);
        MarkForDespawn_421470();
        DeAllocateCarPhysics_43BD00();
    }
}

MATCH_FUNC(0x43e560)
bool Car::ManageDrowning_43E560()
{
    bool ret = field_58_physics->IsDrowning_421100();
    if (ret)
    {
        if (this->field_94_exploder_timer > 0)
        {
            this->field_94_exploder_timer = 50;
        }

        DestroyWhileSinking_43DD60();

        gParticle_8_6FD5E8->EmitWaterSplash_53F060(field_50_car_sprite->field_14_xy.x,
                                                   field_50_car_sprite->field_14_xy.y,
                                                   field_50_car_sprite->field_1C_zpos,
                                                   field_50_car_sprite->field_0_rotation + kAng180_677326,
                                                   1);
    }
    return ret;
}

MATCH_FUNC(0x43e8d0)
Car* Car::GetCabOrSelf_43E8D0()
{
    if (field_64_pTrailer)
    {
        return field_64_pTrailer->field_8_truck_cab;
    }
    return this;
}

MATCH_FUNC(0x43e990)
Ped* Car::GetEffectiveDriver_43E990()
{
    if (IsTrailerCar_421720())
    {
        return field_64_pTrailer->GetTruckCabDriver_407B80();
    }
    else
    {
        return this->field_54_driver;
    }
}

MATCH_FUNC(0x43ea60)
bool Car::OnObjectTouched_43EA60(Object* pObj)
{
    if (pObj->IsPickup_421060() || pObj->field_18_model == objects::secret_token_266)
    {
        Ped* pPed = Car::GetEffectiveDriver_43E990();
        if (pPed)
        {
            return (pPed->HandlePickupCollision_45DE80(pObj));
        }

        return 0;
    }

    switch (pObj->field_18_model)
    {

        case objects::bus_stop_marker_129:
            gPublicTransport_181C_6FF1D4->OnBusStopMarkerHit_579A30(this);
            break;

        case objects::car_shop_130:
            GetCabOrSelf_43E8D0()->HandleShops_443C40(pObj);
            return 1;

        case objects::crane_base_137:
            gCranePool_D9C_679FD4->PickUpCar_480E00(this, pObj->get_field_26_420FF0());
            break;

        case objects::crusher_central_spot_143:
            gCrusherPool_94_67A830->CrushCarWithCrusher_4887D0(this, pObj->get_field_26_420FF0());
            break;

        case objects::animating_oil_8:
        case objects::oil_9:
            field_58_physics->SpinOutOnOil_559BA0();
            break;
        case objects::conveyor_139:
            field_58_physics->ApplyConveyorPush_559E20(pObj); // push car into conveyor direction
            break;
        case objects::savepoint_161:
            gCarThreadTriggers_214_705F20->CheckThreadTrigger_5C8780(pObj->field_27_thread_trigger_idx, this->field_50_car_sprite);
            break;

        case objects::destructor_141:
            if (!field_54_driver)
            {
                DespawnSoon_421490();
            }
            break;

        case objects::door_trigger_167:
            gDoor_4D4_67BD2C->TryOpenDoorForCar_49D340(this, pObj->get_field_26_420FF0());
            break;

        default:
            break;
    }
    return 0;
}

// TODO: move
MATCH_FUNC(0x48E720)
bool __stdcall IsNoKillCreditObject_48E720(s32 model)
{
    bool result;
    switch (model)
    {
        case objects::invisible_bullet_154:
        case objects::invisible_rocket_159:
        case objects::tanktop_193:
        case objects::object_195:
        case objects::object_199:
            result = 1;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

// TODO: move
MATCH_FUNC(0x48E780);
s32 __stdcall GetDeathCauseForObjectModel_48E780(s32 model)
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
            case 251:
                return ped_death_cause::burning_20;
        }
    }
    return ped_death_cause::projectile_default_9;
}

// TODO: move
MATCH_FUNC(0x45CF90)
s32 __stdcall GetDamageMultiplier_45CF90(Ped* pPed)
{
    if (pPed && pPed->field_15C_player && pPed->field_15C_player->field_6F4_power_up_timers[power_up_indices::DoubleDamage_7] != 0)
    {
        return 2;
    }
    return 1;
}

// impact_vec is read uninitialised, as in the original
#pragma warning(push)
#pragma warning(disable : 4700)
// 9.6f 0x4274C0. The Fix16 and Fix16_Point operators are the out-of-line copies (Add_40AC50,
// Multiply_438FE0, operator- 0x40AC80, Multiply_408680, operator+ 0x408660, Normalize_406C20).
// The 9.6f getters (IsMaxDamage_40F890, the flag tests, get_x_y 0x4207B0) set the inline budget: every
// RotateByAngle_40F6B0 keeps only the x_old negate inline. The original keeps EH state stores around the
// get_x_y/operator+/operator- calls, so their bodies weren't compiled yet: the *Late* aliases are defined at
// the end of this file (see "An inline defined after its callers" in docs/matching_quirks.md).
MATCH_FUNC(0x43f130)
bool Car::HandleCarHitByObject_43F130(Object* pObj)
{
    s16 damage = 0;
    Ped* pFoundPed = 0;
    Fix16_Point offset;
    Fix16_Point impact_vec; // never set: 265 and default pass it uninitialised

    if (pObj->get_field_26_420FF0())
    {
        s32 pedId = gPedRefTable_7F8_703398->GetPedId_420F10(pObj->get_field_26_420FF0());
        pFoundPed = pedId ? gPedManager_6787BC->PedById(pedId) : 0;
        if (pFoundPed)
        {
            pFoundPed->HandleShootingAtCar_46FC90(this, pObj->field_18_model);
        }

        if (pObj->field_18_model == objects::invisible_bullet_154 || pObj->field_18_model == objects::tanktop_193 ||
            pObj->field_18_model == objects::object_195 || pObj->field_18_model == objects::invisible_rocket_159 ||
            pObj->field_18_model == objects::object_199)
        {
            return 1;
        }
    }

    if (field_54_driver && pObj->field_18_model != objects::mine_10 && pObj->get_field_26_420FF0() == field_54_driver->get_ped_ref_idx_420B50())
    {
        return 0;
    }

    if (pObj->field_18_model != objects::water_hitting_198 && !IsNoKillCreditObject_48E720(pObj->field_18_model))
    {
        if (pFoundPed)
        {
            this->field_70_exploder_ped_id = pFoundPed->field_200_id;
            this->field_90_death_cause = GetDeathCauseForObjectModel_48E780(pObj->field_18_model);
            this->field_94_exploder_timer = 50;
        }
    }

    gScriptManager_6F8060->RecordWeaponHit_512C00(field_6C_car_id, pObj->field_18_model, 0);

    switch (pObj->field_18_model)
    {
        case objects::rocket_bullet_128:
        case objects::moving_molotov_138:
            if (!IsFlagSet_411930(car_flag::Rocketproof) && !IsMaxDamage_40F890())
            {
                damage = AccumulateDamage_43DA90(
                    32000,
                    &((pObj->field_4_sprite->get_x_y_late_443580().AddLate_40AC50(field_50_car_sprite->get_x_y_late_443580())) * kFpHalf_677218));
                if (!IsMaxDamage_40F890())
                {
                    offset = pObj->field_4_sprite->get_x_y_late_443580().SubLate_40AC80(field_50_car_sprite->get_x_y_late_443580());
                    offset.RotateByAngle_40F6B0(-field_50_car_sprite->field_0_rotation);
                    EmitExplosion_43D690(explosion_type::small_18, offset.x, offset.y);
                }
            }
            else
            {
                offset = pObj->field_4_sprite->get_x_y_late_443580().SubLate_40AC80(field_50_car_sprite->get_x_y_late_443580());
                offset.RotateByAngle_40F6B0(-field_50_car_sprite->field_0_rotation);
                EmitExplosion_43D690(explosion_type::small_18, offset.x, offset.y);
            }
            break;

        case objects::mine_10:
            if (!gCarManager_6C_677930->field_68_object_hit_active)
            {
                gCarManager_6C_677930->field_68_object_hit_active = 1;

                if (!IsMaxDamage_40F890())
                {
                    if (!IsFlagSet_411930(car_flag::Rocketproof))
                    {
                        damage = AccumulateDamage_43DA90(
                            32000,
                            &((pObj->field_4_sprite->get_x_y_late_443580().AddLate_40AC50(field_50_car_sprite->get_x_y_late_443580())) * kFpHalf_677218));
                    }

                    if (!IsMaxDamage_40F890())
                    {
                        offset = pObj->field_4_sprite->get_x_y_late_443580().SubLate_40AC80(field_50_car_sprite->get_x_y_late_443580());
                        offset.RotateByAngle_40F6B0(-field_50_car_sprite->field_0_rotation);
                        EmitExplosion_43D690(explosion_type::small_18, offset.x, offset.y);
                    }
                }
            }
            break;

        case objects::fire_hitting_194:
            if (!IsFlameproof_425770())
            {
                damage = AccumulateDamage_43DA90(
                    100 * GetDamageMultiplier_45CF90(pFoundPed),
                    &((pObj->field_4_sprite->get_x_y_late_443580().AddLate_40AC50(field_50_car_sprite->get_x_y_late_443580())) * kFpHalf_677218));
            }
            break;

        case objects::pistol_bullet_265:
            if (!IsFlagSet_411930(car_flag::Bulletproof))
            {
                damage = AccumulateDamage_43DA90(
                    1600 * GetDamageMultiplier_45CF90(pFoundPed),
                    &((pObj->field_4_sprite->get_x_y_late_443580().AddLate_40AC50(field_50_car_sprite->get_x_y_late_443580())) * kFpHalf_677218));
            }
            // Like 9.6f, 265 and default pass `offset`, which only the explosion cases set
            gParticle_8_6FD5E8->EmitImpactParticles_53FE40(pObj->field_4_sprite->field_14_xy.x,
                                                           pObj->field_4_sprite->field_14_xy.y,
                                                           pObj->field_4_sprite->field_1C_zpos,
                                                           impact_vec);
            break;

        default:
            if (!IsFlagSet_411930(car_flag::Bulletproof))
            {
                damage = AccumulateDamage_43DA90(
                    800 * GetDamageMultiplier_45CF90(pFoundPed),
                    &((pObj->field_4_sprite->get_x_y_late_443580().AddLate_40AC50(field_50_car_sprite->get_x_y_late_443580())) * kFpHalf_677218));
            }
            gParticle_8_6FD5E8->EmitImpactParticles_53FE40(pObj->field_4_sprite->field_14_xy.x,
                                                           pObj->field_4_sprite->field_14_xy.y,
                                                           pObj->field_4_sprite->field_1C_zpos,
                                                           impact_vec);
            break;

        case objects::water_hitting_198:
            field_0_attachments.CleanupSpriteList_5A7080();
            break;

        case objects::electrobaton_bullet_277:
            break;
    }

    switch (pObj->field_18_model)
    {
        case objects::shotgun_bullet_192:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
            this->field_AC_sound_event = 3;
            break;
    }

    if (pObj->field_18_model != objects::water_hitting_198 && damage > 0)
    {
        if (pFoundPed && pFoundPed->PedTypeIs_45EDE0(ped_type::player_2))
        {
            pFoundPed->field_15C_player->field_2D4_scores.AwardCarDamageScoreHit_593150(this, 1);
        }
    }

    return 1;
}

#pragma warning(pop)

MATCH_FUNC(0x440510)
Fix16 Car::GetMaxTurnRate_440510()
{
    if (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->h <= 64u)
    {
        return kFpPoint26_6778B4 * GetCarLinearSpeed_43A240();
    }
    else
    {
        return kFpPoint13_676984 * GetCarLinearSpeed_43A240();
    }
}

MATCH_FUNC(0x440570)
char_type Car::GetMaxPassengers_440570()
{
    if ((field_78_flags & car_flag::NoPassengers))
    {
        return 0;
    }
    else
    {
        return gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->passengers;
    }
}

MATCH_FUNC(0x440590)
void Car::InitCarAIControl_440590()
{
    SetupCarPhysicsAndSpriteBinding_43BCA0();

    field_58_physics->SetHandBrakeOff_421260();

    Ped* pDriver = this->field_54_driver;
    if (pDriver)
    {
        if (pDriver->GetPedType_420B70() != ped_type::player_2)
        {
            if (this->field_5C_AI == 0)
            {
                this->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
            }
            this->field_5C_AI->SetCar_453BF0(this);
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
        pNextPed = gPedManager_6787BC->CreateDummyDriver_470CC0(this);
        gCarManager_6C_677930->SetSpareDriver_421970(pNextPed);
    }
    SetDriver(pNextPed);
    this->field_7C_ped_type = ped_type::dummy_3;
}

MATCH_FUNC(0x440630)
void Car::AddGangDriver_440630(Gang* pGang)
{
    Ped* pNewPed = gPedManager_6787BC->SpawnGangDriver_470BA0(this, pGang);
    SetDriver(pNewPed);
    field_7C_ped_type = ped_type::dummy_3;
}

MATCH_FUNC(0x440660)
void Car::AttachGangIcon_440660(u8 arrow_colour)
{
    Object* pIcon =
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(arrow_colour + 286, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    field_50_car_sprite->AttachSprite_5A3100(pIcon->field_4_sprite, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
}

MATCH_FUNC(0x4406b0)
void Car::ShowCarName_4406B0(Ped* pPed)
{
    Player* pPlayer = pPed->field_15C_player;
    if (pPlayer)
    {
        if (pPlayer->field_0_is_user)
        {
            gHud_2B00_706620->ShowCarName_5D5240(GetCarStr_439F80());
        }
    }
}

MATCH_FUNC(0x4406e0)
void Car::AssignDriver_4406E0(Ped* pPed)
{
    Player* pPlayer = pPed->field_15C_player;
    SetDriver(pPed);
    InitCarAIControl_440590();
    SetDriverPedType_421560(pPed->GetPedType_420B70());
    if (pPed->PedTypeIs_45EDE0(ped_type::player_2))
    {
        DeAllocateAI_4446E0();
    }

    CancelDespawn_4215F0();

    if (IsCarThreadPending_4218F0())
    {
        if (gScriptManager_6F8060->TriggerCarThread_512910(pPed->get_id(), field_6C_car_id))
        {
            ClearCarThreadPending_4218E0();
        }
    }
    if (pPlayer)
    {
        if (pPlayer->IsUser_41DC70())
        {
            if (field_54_driver->get_occupation_403980() != ped_ocupation_enum::empty)
            {
                gHud_2B00_706620->ShowCarName_5D5240(GetCarStr_439F80());
            }
        }
        MarkProtoRecycled_443E50();
        pPlayer->AddCarToHistory_5645B0(this);
        if (field_54_driver->get_occupation_403980() != ped_ocupation_enum::empty)
        {
            pPlayer->LoadCarWeapons_564AD0(this);
        }

        if (field_58_physics)
        {
            field_58_physics->SetField8C_to_2();
        }
        field_95_player_ped_id = pPed->field_200_id;
    }
    else
    {
        field_95_player_ped_id = 0;
    }
}

MATCH_FUNC(0x4407f0)
void Car::ClearDriver_4407F0()
{
    Player* pPlayer = field_54_driver->field_15C_player;
    if (pPlayer)
    {
        if (field_54_driver->get_occupation_403980() != ped_ocupation_enum::empty)
        {
            if (pPlayer->IsUser_41DC70())
            {
                gHud_2B00_706620->field_0_car_name.field_0_display_time = 0;
            }
            pPlayer->UnloadCarWeapons_564C00();
        }

        if (field_58_physics)
        {
            field_58_physics->SetField8C_to_1();
        }
    }
    SetDriver(NULL);
    ClearHorn_421460();
}

MATCH_FUNC(0x440840)
Sprite* Car::GetSprite_440840()
{
    Trailer* pTrailer = this->field_64_pTrailer;
    if (pTrailer)
    {
        return pTrailer->field_C_trailer_car->field_50_car_sprite;
    }
    else
    {
        return this->field_50_car_sprite;
    }
}

MATCH_FUNC(0x440ac0)
void Car::PutWaterCannonOnRoof_440AC0()
{
    Object* pObject =
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::water_cannon_114, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    field_50_car_sprite->AttachSprite_5A3100(pObject->field_4_sprite, gFix16_6777CC, kWaterCannonOffsetY_6771F0, kAng180_677326);
}

MATCH_FUNC(0x440b10)
void Car::PutTankCannonOnRoof_440B10()
{
    Object* pObject =
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::tank_cannon_148, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    field_50_car_sprite->AttachSprite_5A3100(pObject->field_4_sprite, gFix16_6777CC, kTankCannonOffsetY_6772A4, kAngZero_67791C);
}

MATCH_FUNC(0x440b60)
void Car::PutMachineGunOnRoof_440B60()
{
    Object* pObj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(248, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    field_50_car_sprite->AttachSprite_5A3100(pObj->field_4_sprite, gFix16_6777CC, kMachineGunOffsetY_6777FC, kAngZero_67791C);
}

MATCH_FUNC(0x440bb0)
void Car::PutTV_Antenna_440BB0()
{
    Object* pNewObj =
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::tv_antenna_149, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    field_50_car_sprite->AttachSprite_5A3100(pNewObj->field_4_sprite, gFix16_6777CC, kTvAntennaOffsetY_6778A0, GetRadioTowerAngle_442520());
}

MATCH_FUNC(0x440c10)
bool Car::RotateRoofObjectTowardTarget_440C10(Ang16 targetAngle)
{
    AttachedSprite* pFound = 0;
    Ang16 ang = 0;

    if (IsFireTruck_4118F0())
    {
        pFound = field_0_attachments.GetSpriteForModel_5A6A50(objects::water_cannon_114);
        ang = pFound->field_0_pSprite->field_0_rotation + kAng180_677326;
    }
    else if (IsTank_411900())
    {
        pFound = field_0_attachments.GetSpriteForModel_5A6A50(objects::tank_cannon_148);
        ang = pFound->field_0_pSprite->field_0_rotation;
    }
    else if (IsGunJeep_411910())
    {
        pFound = field_0_attachments.GetSpriteForModel_5A6A50(248);
        ang = pFound->field_0_pSprite->field_0_rotation;
    }

    if (ComputeShortestAngleDelta_4056C0(ang, targetAngle) <= kRoofTurretAutoRotSpeed_677910)
    {
        return 1;
    }

    if (Ang16::IsAngleAhead_405C60(&ang, &targetAngle))
    {
        pFound->field_10_offset_rot += kRoofTurretAutoRotSpeed_677910;
    }
    else
    {
        pFound->field_10_offset_rot -= kRoofTurretAutoRotSpeed_677910;
    }
    return 0;
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
bool Car::HandleRoofTurretRotation_440D90(char_type left_on)
{
    // 9.6f joins the three models into one +=/-= pair. Here field_B8 is set in both branches (a
    // single store after the if/else keeps the angle in %di across the second Normalize loop)
    if (field_84_car_info_idx == car_model_enum::FIRETRUK)
    {
        AttachedSprite* pSprite = field_0_attachments.GetSpriteForModel_5A6A50(objects::water_cannon_114);
        if (left_on)
        {
            compound_add_41FA70(pSprite->field_10_offset_rot, kFireTruckCannonRotSpeed_6771C0);
            this->field_B8_turret_rotated = 1;
        }
        else
        {
            compound_subtract_41FA90(pSprite->field_10_offset_rot, kFireTruckCannonRotSpeed_6771C0);
            this->field_B8_turret_rotated = 1;
        }
        return 1;
    }

    if (field_84_car_info_idx == car_model_enum::TANK)
    {
        AttachedSprite* pSprite = field_0_attachments.GetSpriteForModel_5A6A50(objects::tank_cannon_148);
        if (left_on)
        {
            compound_add_41FA70(pSprite->field_10_offset_rot, kTankTurretRotSpeed_677352);
            this->field_B8_turret_rotated = 1;
        }
        else
        {
            compound_subtract_41FA90(pSprite->field_10_offset_rot, kTankTurretRotSpeed_677352);
            this->field_B8_turret_rotated = 1;
        }
        return 1;
    }

    if (field_84_car_info_idx == car_model_enum::GUNJEEP)
    {
        AttachedSprite* pSprite = field_0_attachments.GetSpriteForModel_5A6A50(248);
        if (left_on)
        {
            compound_add_41FA70(pSprite->field_10_offset_rot, kGunJeepTurretRotSpeed_677810);
            this->field_B8_turret_rotated = 1;
        }
        else
        {
            compound_subtract_41FA90(pSprite->field_10_offset_rot, kGunJeepTurretRotSpeed_677810);
            this->field_B8_turret_rotated = 1;
        }
        return 1;
    }

    return 0;
}

MATCH_FUNC(0x440f90)
void Car::FireCarBomb_440F90(char_type instant_bomb)
{
    if (instant_bomb)
    {
        s32 ped_id = gPedRefTable_7F8_703398->GetPedId_420F10(field_54_driver->get_ped_ref_idx_420B50());
        if (ped_id)
        {
            this->field_70_exploder_ped_id = ped_id;
            this->field_90_death_cause = ped_death_cause::bomb_12;
            this->field_94_exploder_timer = 50;
        }
        Car::TriggerExplosion_43D7B0(explosion_type::large_20);
    }
    else
    {
        Object* pNewObject = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::car_bomb_132,
                                                                    gFix16_6777CC,
                                                                    gFix16_6777CC,
                                                                    gFix16_6777CC,
                                                                    kAngZero_67791C);
        Ped* pDriver = this->field_54_driver;
        if (pDriver)
        {
            pNewObject->SetDamageOwner_529080(pDriver->get_ped_ref_idx_420B50());
        }
        field_50_car_sprite->AttachSprite_5A3100(pNewObject->field_4_sprite, gFix16_6777CC, gFix16_6777CC, kAngZero_67791C);
    }
}

MATCH_FUNC(0x441030)
void Car::GoToBlockTest_441030(u8 x, u8 y, u8 z, s32 direction)
{
    if (!field_5C_AI)
    {
        field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
    }
    field_5C_AI->SetCar_453BF0(this);
    field_5C_AI->GoToBlock_447CA0(x, y, z, direction);
}

MATCH_FUNC(0x441080)
void Car::GotoBlock_441080(u8 x, u8 y, u8 z, s32 direction)
{
    if (!field_5C_AI)
    {
        field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
    }
    field_5C_AI->SetCar_453BF0(this);
    field_5C_AI->GoToBlock_447CA0(x, y, z, direction);
}

// 9.6f 0x41FF90
MATCH_FUNC(0x4410d0)
char_type Car::CountConsecutiveArrowBlocks_4410D0(Ang16 ang, s8* pOutCount, Fix16 sprite_x, Fix16 sprite_y)
{
    u8 x_add = 0;
    u8 y_add = 0;
    u8 mask1 = 0;
    u8 mask2 = 0;
    s32 angleFace = Ang16::GetAngleFace_4F78F0(ang);
    u8 x = sprite_x.ToInt();
    u8 y = sprite_y.ToInt();
    u8 z = field_50_car_sprite->field_1C_zpos.ToInt() - 1;

    switch (angleFace)
    {
        case 1:
            x_add = -1;
            mask1 = 4;
            mask2 = 0x40;
            break;
        case 2:
            x_add = 1;
            mask1 = 8;
            mask2 = 0x80;
            break;
        case 4:
            y_add = 1;
            mask2 = 0x10;
            mask1 = 1;
            break;
        case 3:
            y_add = -1;
            mask1 = 2;
            mask2 = 0x20;
            break;
        default:
            break;
    }

    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z);
    if (pBlock && ((pBlock->field_A_arrows & mask1) || (pBlock->field_A_arrows & mask2)))
    {
        x += x_add;
        y += y_add;

        u8 count = 0;
        for (pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z); pBlock; pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z))
        {
            if (!(pBlock->field_A_arrows & mask1) && !(pBlock->field_A_arrows & mask2))
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

        for (pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z); pBlock; pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z))
        {
            if (!(pBlock->field_A_arrows & mask1) && !(pBlock->field_A_arrows & mask2))
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
Fix16 Car::GetZPos_441330()
{
    if (field_58_physics)
    {
        return field_58_physics->ComputeZPosition_559E90();
    }
    return field_50_car_sprite->field_1C_zpos;
}

MATCH_FUNC(0x441360)
void Car::CountDownToWreck_441360()
{
    if (field_A9_wreck_countdown)
    {
        field_A9_wreck_countdown--;
        if (field_A9_wreck_countdown == 0)
        {
            TurnToWreck_4436A0(); // jmp to function chunk
        }
    }
}

MATCH_FUNC(0x4436A0)
void Car::TurnToWreck_4436A0()
{
    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    s32 wreck_idx = pCarInfo->wreck + car_model_enum::WRECK0;
    field_50_car_sprite->field_22_type_sprite_idx = gGtx_0x106C_703DD4->get_car_info_5AA3B0(wreck_idx)->sprite;
    field_50_car_sprite->CopyCollisionShapeToVisual_59E2E0();
    field_50_car_sprite->field_34_palette_type = palette_types_enum::sprites_2;
    field_8_damaged_areas.m_var = 0;
    PrepareForExplosion_43C1C0();
    field_A7_horn = 0;
    SpawnFire_43BBC0();
}

MATCH_FUNC(0x441380)
void Car::StartWreckCountdown_441380()
{
    if (this->field_A9_wreck_countdown == 0)
    {
        if (this->field_9C_engine_status != car_engine_status::destroyed_6)
        {
            this->field_9C_engine_status = car_engine_status::destroying_5;
        }
        this->field_A9_wreck_countdown = 50;
    }
}

MATCH_FUNC(0x4F7940)
EXPORT Ang16 __stdcall ReturnAngleFromRoadDirection_4F7940(s32& road_direction)
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
void Car::UpdateTrainCarriagesOnTrack_4413B0(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{

    u8 idx = 0;
    Car** pTrainCars = gPublicTransport_181C_6FF1D4->GetCarArrayFromLeadCar_579B40(this);
    Train* pTrain = gPublicTransport_181C_6FF1D4->GetTrainFromCar_57B5C0(this);
    if (pTrain)
    {
        pTrainCars = &pTrain->field_C_carriages[1];
    }

    Fix16 newx;
    Fix16 newy;
    Fix16 newz;

    Fix16 x = xpos;
    Fix16 y = ypos;
    Fix16 z = zpos;

    Fix16 train_speed;
    bool bUnknown = gPublicTransport_181C_6FF1D4->GetTrainSpeed_579B90(this, &train_speed);

    for (Car* pTrainCarIter = *pTrainCars; pTrainCarIter; pTrainCarIter = pTrainCars[idx])
    {

        newx = x;
        newy = y;
        newz = z;

        Ang16 carriage_angle;
        if (bUnknown)
        {
            s32 track_dir_a = gMap_0x370_6F6268->AdvanceAlongRailwayChecked_4E7190(&newx, &newy, &newz, kFpTwo_6777D4);
            carriage_angle = ReturnAngleFromRoadDirection_4F7940(track_dir_a);
        }
        else
        {
            s32 track_dir_b = gMap_0x370_6F6268->AdvanceAlongRailway_4E6660(&newx, &newy, &newz, kFpTwo_6777D4);
            carriage_angle = ReturnAngleFromRoadDirection_4F7940(track_dir_b);
        }

        pTrainCarIter->field_50_car_sprite->set_xyz_lazy_420600(newx, newy, newz);
        pTrainCarIter->field_50_car_sprite->set_ang_lazy_420690(carriage_angle);

        // The next carriage follows on from this one
        x = newx;
        y = newy;
        z = newz;

        if (pTrainCarIter->field_0_attachments.field_0_p18)
        {
            pTrainCarIter->field_0_attachments.PoolUpdate_5A6F70(pTrainCarIter->field_50_car_sprite);
            pTrainCarIter->field_0_attachments.PropagateMaxZLayer_5A72B0(pTrainCarIter->field_50_car_sprite, 0);
        }

        pTrainCarIter->ServiceDoorsLightsAndHorn_4426D0();
        pTrainCarIter->CountDownToWreck_441360();

        idx++;
    }
}

MATCH_FUNC(0x441520)
void Car::UpdateEngineStatus_441520()
{
    CarPhysics* pCarPhysics;
    switch (field_9C_engine_status)
    {
        case car_engine_status::off_1:
            pCarPhysics = field_58_physics;
            if (pCarPhysics)
            {
                if (pCarPhysics->IsAccelerationOrReverseOn_55A180())
                {
                    field_9C_engine_status = car_engine_status::starting_4;
                }
            }
            break;
        case car_engine_status::starting_4:
            HeadlightsOn_43BFE0();
            field_9C_engine_status = car_engine_status::on_3;
            break;
        case car_engine_status::stopping_2:
            HeadlightsOff_43C0C0();
            field_9C_engine_status = car_engine_status::off_1;
            break;
        case car_engine_status::destroying_5:
            HeadlightsOff_43C0C0();
            field_9C_engine_status = car_engine_status::destroyed_6;
            break;
        case car_engine_status::crushed_7:
            field_50_car_sprite->CopyVisualShapeToCollision_59E300();
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4415c0)
void Car::UpdateBrakeLights_4415C0()
{
    CarPhysics* pCarPhysics = this->field_58_physics;
    if (pCarPhysics && pCarPhysics->IsFootBrakeOn_55A150())
    {
        BrakeLightsOn_43BF10();
    }
    else
    {
        BrakeLightsOff_43BF70();
    }
}

MATCH_FUNC(0x441600)
Fix16_Rect Car::GetOrientedHalfExtentsRect_441600()
{
    car_info* pInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx);
    Fix16 half_w = gPixelsToFix16_6F6850.list[pInfo->w] / kFpTwo_6777D4;
    Fix16 half_h = gPixelsToFix16_6F6850.list[pInfo->h] / kFpTwo_6777D4;

    s32 face = Ang16::GetAngleFace_4F78F0(field_50_car_sprite->field_0_rotation);
    if (face >= 3 && face <= 4)
    {
        Fix16 tmp = half_w;
        half_w = half_h;
        half_h = tmp;
    }

    Fix16 z = field_50_car_sprite->field_1C_zpos;
    return Fix16_Rect(-half_w, half_w, -half_h, half_h, z);
}

MATCH_FUNC(0x4416d0)
void Car::TryHonkHorn_4416D0(s32 horn_mode)
{
    char bHornStarted = 0;
    if (field_A8_horn_cooldown > 0)
    {
        field_A8_horn_cooldown--;
    }

    if (horn_mode != 0)
    {
        if (!this->field_A8_horn_cooldown)
        {
            if (!IsTrainModel_403BA0() && field_84_car_info_idx != car_model_enum::TANK && (this->field_6C_car_id & 7) == 0)
            {
                if (horn_mode == 2)
                {
                    this->field_AC_sound_event = 1;
                }
                else if (horn_mode == 1)
                {
                    this->field_AC_sound_event = 2;
                }
                bHornStarted = 1;
                this->field_A8_horn_cooldown = -1;
            }
        }
    }

    if (!this->field_A7_horn && !IsMaxDamage_40F890())
    {
        if (this->field_54_driver->field_15C_player)
        {
            this->field_A7_horn = 45;
        }
        else if (!this->field_A8_horn_cooldown || bHornStarted)
        {
            if (field_84_car_info_idx == car_model_enum::FIRETRUK || field_84_car_info_idx == car_model_enum::COPCAR ||
                field_84_car_info_idx == car_model_enum::MEDICAR || field_84_car_info_idx == car_model_enum::SWATVAN ||
                field_84_car_info_idx == car_model_enum::TANK || IsFbiCar_411920() || IsTrainModel_403BA0() ||
                (this->field_6C_car_id & 3) == 0)
            {
                this->field_A7_horn = 45;
                this->field_A8_horn_cooldown = -1;
            }
        }
    }
}

MATCH_FUNC(0x4417d0)
void Car::UpdateHorn_4417D0()
{
    if (field_A7_horn <= 0)
    {
        return;
    }

    if (field_A7_horn != 248)
    {
        field_A7_horn--;
    }
}

MATCH_FUNC(0x4417f0)
void Car::HonkHorn_4417F0()
{
    field_A7_horn = 45;
}

MATCH_FUNC(0x441800)
void Car::HandleSpecialInput_441800(char_type now_special_pressed)
{
    if (HasEmergencyLights_414F20() && !now_special_pressed && field_A7_horn > 248u)
    {
        ToggleEmergencyLights_43CA80(); // on/off depending on field_A4_light_flags flags
    }

    if (!now_special_pressed || IsMaxDamage_40F890())
    {
        field_A7_horn = 0;
        return;
    }

    if (HasEmergencyLights_414F20())
    {
        field_A7_horn = -1;
    }
    else
    {
        field_A7_horn = -8;
    }
}

MATCH_FUNC(0x4418a0)
void Car::DoDetachTrailer_4418A0()
{
    if (field_64_pTrailer)
    {
        FlashAndDetachTrailer_4418B0();
    }
}

MATCH_FUNC(0x4418b0)
void Car::FlashAndDetachTrailer_4418B0()
{
    FlashHeadlights_43BD40();
    DetachTrailer_442760();
    AddFlags_421890(car_flag::TrailerDetached);
}

// https://decomp.me/scratch/KU02C
MATCH_FUNC(0x4418d0)
void Car::HandleUserInput_4418D0(char_type forward_gas_on,
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
    this->field_B8_turret_rotated = 0;

    if (now_special_pressed && (left_on || right_on))
    {
        // NB: If left is off then right is on
        bRotated = Car::HandleRoofTurretRotation_440D90(left_on);
    }

    if (bRotated)
    {
        left_on = 0;
        right_on = 0;
        now_special_pressed = 0;
    }

    if (was_special_pressed)
    {
        Car::HandleSpecialInput_441800(now_special_pressed);
    }

    if (attack)
    {
        Car::DoDetachTrailer_4418A0();
    }

    // 9.6f: Car::sub_425780 (field_78_flags & 0x1000 through sub_411930), as a bool helper it changes the code
    if ((this->field_78_flags & car_flag::Flag12))
    {
        forward_gas_on = 1;
    }

    field_58_physics->HandleUserInputs_55A860(forward_gas_on, foot_brake_on, left_on, right_on, hand_brake_on);
}

MATCH_FUNC(0x4419e0)
void Car::AllocCarPhysics_4419E0()
{
    if (!field_58_physics)
    {
        field_58_physics = gCarPhysicsPool_6FE3E0->Allocate();
    }
}

MATCH_FUNC(0x441a10)
void Car::DeAllocateCarPhysics_441A10()
{
    if (field_58_physics)
    {
        gCarPhysicsPool_6FE3E0->DeAllocate(field_58_physics);
    }
    field_58_physics = NULL;
}

MATCH_FUNC(0x441a40)
bool Car::AreAllDoorsClosed_441A40()
{
    u32 i = 0;
    do
    {
        if (field_C_doors[i].field_4_state != car_door_state::inactive_0 && field_C_doors[i].field_4_state != car_door_state::closed_6)
        {
            return 0;
        }
        i++;
    } while (i < GTA2_COUNTOF(field_C_doors));
    return 1;
}

MATCH_FUNC(0x441a70)
void Car::InitDoors_441A70()
{
    const u8* pRemapCount = gGtx_0x106C_703DD4->get_car_door_data_5AA3D0(field_84_car_info_idx);
    u8 i;
    for (i = 0; i < *pRemapCount; i++)
    {
        field_C_doors[i].Init_439E40(i);
        field_C_doors[i].set_ped_421380(0);
    }

    for (i = *pRemapCount; i < 4; i++)
    {
        field_C_doors[i].ResetState_421340();
    }
}

MATCH_FUNC(0x441b00)
void Car::ServiceDoors_441B00()
{
    Car_Door* p = field_C_doors;
    for (s32 i = 0; i < 4; i++)
    {
        p->Service_439DA0(&field_8_damaged_areas.m_var);
        p++;
    }
}

MATCH_FUNC(0x441b20)
void Car::UpdateTaxiLight_441B20()
{
    if (field_9C_engine_status != car_engine_status::on_3 || field_4_passengers_list.field_0_pFirstPed)
    {
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }
    else
    {
        field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }
}

MATCH_FUNC(0x441b50)
void Car::UpdateRoofLightFlasher_441B50()
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
    if (field_A5_flash_phase_counter > 0)
    {
        field_A5_flash_phase_counter--;
        if (field_A5_flash_phase_counter == flashOffThreshold)
        {
            ResetBottomLeftRoofLight_43C840();
            ResetTopLeftRoofLight_43C470();
        }

        if (!field_A5_flash_phase_counter)
        {
            field_A5_flash_phase_counter = -flashCycleAmplitude;
            UpdateRoofLights_43C500();
            UpdateTopRightRoofLight_43C260();
        }
    }
    // negative side of flash cycle
    else
    {
        field_A5_flash_phase_counter++;
        if (field_A5_flash_phase_counter == -flashOffThreshold)
        {
            ResetRoofLights_43C650();
            ResetTopRightRoofLight_43C310();
        }

        if (!field_A5_flash_phase_counter)
        {
            field_A5_flash_phase_counter = flashCycleAmplitude;
            UpdateBottomLeftRoofLight_43C700();
            UpdateTopLeftRoofLight_43C3C0();
        }
    }
}

MATCH_FUNC(0x441c00)
void Car::UpdatePopupHeadlights_441C00()
{
    if (!(gpRng_67AB34->get_cur_rng_41CFE0() % 3u))
    {
        if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor2_12);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor3_13);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor4_14);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
        }

        if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDoor1_28))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDoor2_29))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDoor3_30))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopLeftDoor4_31))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
        }
    }
}

MATCH_FUNC(0x441d40)
void Car::UpdateFbiPopupSiren_441D40()
{
    if (!(gpRng_67AB34->get_cur_rng_41CFE0() % 3u))
    {
        if (this->field_A4_bf.bEmergencyLights)
        {
            if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11))
            {
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
                this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            }
            else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12))
            {
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
                this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            }
            else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13))
            {
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
                this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor4_14);
            }
            else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
            {
                this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
                this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
            }
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor4_14))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor3_13);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor3_13))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor2_12);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor2_12))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
            this->field_8_damaged_areas.set_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        }
        else if (field_8_damaged_areas.mask_bit(CarDeltaBitsEnum::TopRightDoor1_11))
        {
            this->field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        }
    }
}

MATCH_FUNC(0x441e70)
void Car::UpdateLights_441E70()
{
    if (field_A4_bf.bEmergencyLights)
    {
        Car::UpdateRoofLightFlasher_441B50();
    }
    else if (field_A4_bf.bLightFlashing)
    {
        field_A5_flash_phase_counter--;
        if (field_A5_flash_phase_counter == 0)
        {
            field_8E_flash_count--;
            if (field_8E_flash_count == 0)
            {
                Car::StopLightFlashing_43CBE0();
            }
            else
            {
                SetHeadlightDeltas_425590();
                SetBrakeLightDeltas_4213D0();
                field_A5_flash_phase_counter = 12;
            }
        }
        else
        {
            if (field_A5_flash_phase_counter == 6)
            {
                ClearHeadlightDeltas_425650();
                ClearBrakeLightDeltas_421430();
            }
        }
    }
    else if (field_A4_bf.bHeadlightFlash)
    {
        field_A5_flash_phase_counter--;
        if (field_A5_flash_phase_counter == 0)
        {
            if (field_A4_bf.bHeadlights)
            {
                SetHeadlightDeltas_425590();
            }
            else
            {
                Car::ClearHeadlightDeltas_447360();
            }
            field_A4_bf.bHeadlightFlash = false;
        }
    }

    if (HasPopupHeadlights_421700())
    {
        Car::UpdatePopupHeadlights_441C00();
    }
    if (HasTaxiLight_4216C0())
    {
        Car::UpdateTaxiLight_441B20();
    }
    if (IsFbiCar_411920())
    {
        Car::UpdateFbiPopupSiren_441D40();
    }
}

MATCH_FUNC(0x442170)
bool Car::IsCarInConditionsToDespawn_442170()
{
    Ped* pDriver = field_54_driver;
    if ((!pDriver || !pDriver->field_15C_player) && !IsVisibleToAnyPlayer_43B750())
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x442190)
void Car::ServicePhysics_442190()
{
    if (field_58_physics->ProcessCarPhysicsStateMachine_562FE0())
    {
        if (!this->field_54_driver)
        {
            DeAllocateCarPhysics_43BD00();
        }
    }
}

MATCH_FUNC(0x4421b0)
bool Car::IsMissionRelated_4421B0()
{
    if (field_A0_car_kind != car_kind::mission_car_8 && field_7C_ped_type != ped_type::script_created_5 && !field_4_passengers_list.HasScriptCreatedPed_471710())
    {
        return 0;
    }

    return 1;
}

MATCH_FUNC(0x442200)
bool Car::CanDespawn_442200()
{
    if (field_A0_car_kind == car_kind::parked_car_9)
    {
        return IsMaxDamage_40F890() ? true : false;
    }

    if (!IsTrainModel_403BA0() && !gGame_0x40_67E008->IsCarInAnyPlayerHistory_4B9C10(this) && !Car::IsMissionRelated_4421B0())
    {
        if (field_7C_ped_type != ped_type::special_ped_4 && (field_7C_ped_type != ped_type::dummy_with_occupation_6 || !field_54_driver))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x442310)
void Car::ManageDespawning_442310()
{
    bool bOnScreenForAnyPlayer = false;
    bool bIsNotSeen = true;

    if (bSkip_recycling_67D575 && field_84_car_info_idx != car_model_enum::MEDICAR && field_84_car_info_idx != car_model_enum::COPCAR &&
        field_84_car_info_idx != car_model_enum::EDSELFBI)
    {
        return;
    }

    if (IsTrainModel_403BA0())
    {
        if (IsOnScreenForAnyPlayer_43B730())
        {
            field_76_last_seen_timer = 0;
            bIsNotSeen = false;
        }
    }
    else
    {
        for (u8 i = 0; i < 6; i++)
        {
            if (gGame_0x40_67E008->GetPlayer_4219E0(i))
            {
                if (gGame_0x40_67E008->IsSpriteInPlayerBounds_4B9950(field_50_car_sprite, i, kFpFive_6778D0))
                {
                    bOnScreenForAnyPlayer = true;
                }
            }
        }

        if (bOnScreenForAnyPlayer)
        {
            field_76_last_seen_timer = 0;
            bIsNotSeen = false;
        }
    }

    if (bIsNotSeen)
    {
        field_76_last_seen_timer++;
        if (field_76_last_seen_timer > 9999)
        {
            field_76_last_seen_timer = 9999;
        }
    }

    if (((IsCopCar_421790() || IsFireTruck_4118F0() || IsSwatVan_4217A0() || IsTank_411900() || IsGunJeep_411910() ||
          IsFbiCar_411920()) &&
         field_76_last_seen_timer == 300) ||
        field_76_last_seen_timer >= 130)
    {
        if (CanDespawn_442200() && !IsMarkedForDespawn_4214B0() && field_88_despawn_status != car_despawn_status::despawning_5)
        {
            MarkForDespawn_421470();
        }
    }
}

MATCH_FUNC(0x4424c0)
bool Car::UpdateCarDespawnStatus_4424C0()
{
    switch (field_88_despawn_status)
    {
        case car_despawn_status::despawn_soon_3: // Destructor?
            field_88_despawn_status = car_despawn_status::despawning_5; // Despawn now
            return 0;

        case car_despawn_status::marked_for_despawn_4:
            field_88_despawn_status = car_despawn_status::despawn_pending_2;
            return 0;

        case car_despawn_status::despawn_pending_2: // Mostly NPC cars
            if (!IsCarInConditionsToDespawn_442170())
            {
                return 0;
            }
            field_88_despawn_status = car_despawn_status::despawning_5; // Despawn now
            return 0;

        case car_despawn_status::despawning_5: // Despawn now
            field_88_despawn_status = car_despawn_status::despawned_6; // Despawned
            return 1;

        default:
            return 0;
    }
}

// 9.6f 0x424220
// https://decomp.me/scratch/vhWKK
MATCH_FUNC(0x442520)
Ang16 Car::GetRadioTowerAngle_442520()
{
    Fix16_Point xy;
    xy = gCarManager_6C_677930->field_4C_tv_van_dir - field_50_car_sprite->get_x_y();
    return xy.atan2_40F790() - field_50_car_sprite->field_0_rotation;
}


// TODO: Move all of these together with SmoothApproachAngle_405CE0
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
// TODO: Move
// https://decomp.me/scratch/GnD4O
MATCH_FUNC(0x405CE0)
EXPORT void __stdcall SmoothApproachAngle_405CE0(Fix16& target_angle, Fix16& angle_speed, Fix16& angle, Fix16& accel, Fix16& max_speed)
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

    Fix16 a1_m_a3 = target_angle - angle;

    if (a1_m_a3 > kFPZero_6691B0)
    {
        if (angle_speed >= kFPZero_6691B0)
        {
            if (angle_speed + accel <= a1_m_a3)
            {
                angle_speed += accel;
                if (angle_speed > max_speed)
                {
                    angle_speed = max_speed;
                }
            }
            else
            {
                angle_speed = a1_m_a3;
            }
        }
        else
        {
            angle_speed = kFPZero_6691B0;
        }
    }
    else
    {
        if (a1_m_a3 >= kFPZero_6691B0 || angle_speed > kFPZero_6691B0)
        {
            angle_speed = kFPZero_6691B0;
        }
        else
        {
            if (angle_speed - accel >= a1_m_a3)
            {
                angle_speed -= accel;
                if (angle_speed < -max_speed)
                {
                    angle_speed = -max_speed;
                }
            }
            else
            {
                angle_speed = a1_m_a3;
            }
        }
    }

    angle = WrapAngle_40E790(angle + angle_speed);
}

// 9.6f 0x424280
MATCH_FUNC(0x4425d0)
void Car::ManageTVAntenna_4425D0()
{
    // Fix16 locals declared at the top (not in the inner block) give the original stack slot layout
    Ang16 towerAng;
    Fix16 zero;
    Fix16 spriteAngFp;
    AttachedSprite* pSprite = field_0_attachments.GetSpriteForModel_5A6A50(objects::tv_antenna_149);
    if (pSprite)
    {
        towerAng = GetRadioTowerAngle_442520();
        if (pSprite->field_10_offset_rot != towerAng)
        {
            zero = gFix16_6777CC;
            // A reference makes VC6 reload the rotation instead of reusing the compared value
            Ang16& rot = pSprite->field_10_offset_rot;
            spriteAngFp = Ang16::Ang16_to_Fix16(rot);
            SmoothApproachAngle_405CE0(Ang16::Ang16_to_Fix16(towerAng), zero, spriteAngFp, kAngFix16OneDegree_677920, kAngFix16OneDegree_677920);
            rot = Ang16::Fix16_To_Ang16_40F540(spriteAngFp);
        }
    }
}

MATCH_FUNC(0x4426d0)
void Car::ServiceDoorsLightsAndHorn_4426D0()
{
    ServiceDoors_441B00();
    UpdateEngineStatus_441520();

    if (!IsMaxDamage_40F890())
    {
        UpdateBrakeLights_4415C0();
        UpdateLights_441E70();
    }

    UpdateHorn_4417D0();

    if (IsTvVan_4217E0() && IsEngineOn_4118C0())
    {
        ManageTVAntenna_4425D0();
    }
}

MATCH_FUNC(0x442760)
void Car::DetachTrailer_442760()
{
    Trailer* p = field_64_pTrailer;
    gCar_BC_Pool_67792C->UpdateNextPrev(field_64_pTrailer->field_C_trailer_car);
    field_64_pTrailer->field_C_trailer_car->field_64_pTrailer = 0;
    field_64_pTrailer->field_8_truck_cab->field_64_pTrailer = 0;
    gTrailerPool_66AC80->Remove_425580(p);
}

MATCH_FUNC(0x4427a0)
void Car::AttachTrailer_4427A0(Car* pToFind)
{
    field_64_pTrailer = gTrailerPool_66AC80->field_0_pool.Allocate();
    field_64_pTrailer->SetTruckCabAndTrailerCar_407BB0(this, pToFind);

    Car* pLast = 0;
    for (Car* pIter = gCar_BC_Pool_67792C->field_0_pool.field_4_pPrev; pIter; pIter = pIter->mpNext)
    {
        if (pIter == pToFind)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
            }
            else
            {
                gCar_BC_Pool_67792C->field_0_pool.field_4_pPrev = pIter->mpNext;
            }
            pIter->mpNext = 0;
            return;
        }
        pLast = pIter;
    }
}

// 9.6f 0x4262E0
MATCH_FUNC(0x442810)
void Car::TryHitchTrailer_442810()
{

    Fix16_Point hitch_this;
    Fix16_Point hitch_car;
    Fix16_Point offset;
    Fix16_Point hitch_point; // the original sets EH state 3 up front: 4 points constructed

    field_50_car_sprite->SetDrawLayer_40F7B0(15);

    if (!HasSpriteZoom_43A230() && !IsMaxDamage_40F890() && !IsMarkedForDespawn_4214B0() && !IsDespawning_4215B0())
    {
        stru_67727C.PruneNonCollidingSprites_5A7240(field_50_car_sprite);
        Sprite* pHitchSprite =
            stru_67727C.FindClosestSprite_5A6E40(this->field_50_car_sprite->field_14_xy.x, this->field_50_car_sprite->field_14_xy.y);
        if (!pHitchSprite)
        {
            //goto LABEL_28;
            this->field_78_flags &= static_cast<u16>(~car_flag::TrailerDetached);
        }
        else
        {
            if (field_50_car_sprite->CheckSlopeTopInFootprint_5A1A60() || stru_67727C.AnySpriteHasSlopeTopInFootprint_5A7310())
            {
                this->field_50_car_sprite->SetDrawLayer_40F7B0(8);
                stru_67727C.PropagateMaxZLayer_5A72B0(this->field_50_car_sprite, 1);
            }

            Car* pCar = pHitchSprite->AsCar_40FEB0();
            hitch_car = pCar->GetHitchPoint_439FB0();
            hitch_this = GetHitchPoint_439FB0();
            offset = hitch_this - hitch_car;
            // Declared, then assigned: keeps the length in eax (Abs results reloaded from their temp)
            Fix16 v6_len;
            v6_len = offset.GetLength_41E260();

            Fix16 z_delta = Fix16::Abs(pHitchSprite->field_1C_zpos - field_50_car_sprite->field_1C_zpos);

            if (v6_len < kHitchPullDist_6772C0 && z_delta < kHitchPullDist_6772C0)
            {
                if ((this->field_78_flags & 1) == 0)
                {
                    if (v6_len < kHitchAttachDist_677900)
                    {
                        AttachTrailer_4427A0(pCar);
                        FlashHeadlights_43BD40();
                    }
                    else
                    {
                        pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
                        if (!pCar->field_58_physics->IsDamageWindowOpen_421130())
                        {
                            hitch_point = pCar->field_50_car_sprite->get_x_y_443580().AddInl_40AC50(offset.NormalizeSafe_442AD0().MultiplyInl_438FE0(kFpOne64th_677888));
                            {
                                s32 rotation_mode = 1;
                                pCar->field_58_physics->SetVelocityTowardTarget_55A1D0(
                                    hitch_point.x,
                                    hitch_point.y,
                                    Ang16::Ang16_to_Fix16(pCar->field_58_physics->get_theta_40F820()),
                                    &rotation_mode);
                            }
                        }
                    }
                }
            }
            else
            {
                //LABEL_28:
                this->field_78_flags &= ~1u;
            }
        }
    }
    stru_67727C.ClearList_5A6E10();
}

MATCH_FUNC(0x442d10)
void Car::LightUpdate_442D10()
{
    AttachedSprite* pAttached = field_0_attachments.GetSpriteForModel_5A6A50(objects::light_165);
    if (pAttached)
    {
        Sprite* pSprite = pAttached->field_0_pSprite;
        Object* pObject = pSprite->AsObject2C_40FEC0();
        if (gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos))
        {
            pObject->TurnOffLight_527990();
        }
        else
        {
            pObject->UpdateLight_527A30();
        }
    }
}

// Not in 9.6f (no pair for TrainUpdate_442D70). high_z calls the out-of-line
// Fix16::Add_408660 (operator+) as the original does; all the arguments are
// evaluated before any store, which gives the original's schedule.
static inline void SetPrism(Fix16_Rect& rect, Fix16 left, Fix16 right, Fix16 top, Fix16 bottom, const Fix16 z)
{
    rect.field_0_left = left;
    rect.field_4_right = right;
    rect.field_8_top = top;
    rect.field_C_bottom = bottom;
    rect.field_10_low_z = z - kCollisionPrismHalfHeight_6771E4;
    rect.field_14_high_z = z.Add_408660(kCollisionPrismHalfHeight_6771E4);
}

MATCH_FUNC(0x442d70)
bool Car::TrainUpdate_442D70()
{
    u8 train_car_idx_ = 0;
    Fix16 player_x;
    Fix16 player_y;
    Fix16 player_z;
    if (GetCarLinearSpeed_43A240() > gFix16_6777CC)
    {
        gGame_0x40_67E008->field_38_local_player->get_pos_569920(&player_x, &player_y, &player_z);

        if (Fix16::Abs_negate_out_of_line(field_50_car_sprite->field_14_xy.x - player_x) >= Fix16(10) ||
            Fix16::Abs(field_50_car_sprite->field_14_xy.y - player_y) >= Fix16(10))
        {
            field_A8_horn_cooldown = 0;
        }
        else
        {
            Fix16_Rect rect;
            switch (Ang16::GetAngleFace_4F78F0(field_50_car_sprite->field_0_rotation))
            {
                case 2:
                {
                    Fix16 y = field_50_car_sprite->field_14_xy.y;
                    Fix16 z = field_50_car_sprite->field_1C_zpos;
                    Fix16 x = field_50_car_sprite->field_14_xy.x;
                    SetPrism(rect, x - Fix16(1), x + Fix16(1), y, y + Fix16(10), z);
                    break;
                }

                case 1:
                {
                    Fix16 z = field_50_car_sprite->field_1C_zpos;
                    Fix16 y = field_50_car_sprite->field_14_xy.y;
                    Fix16 x = field_50_car_sprite->field_14_xy.x;
                    SetPrism(rect, x - Fix16(1), x + Fix16(1), y - Fix16(10), y, z);
                    break;
                }

                case 3:
                {
                    Fix16 y = field_50_car_sprite->field_14_xy.y;
                    Fix16 z = field_50_car_sprite->field_1C_zpos;
                    Fix16 x = field_50_car_sprite->field_14_xy.x;
                    SetPrism(rect, x, x + Fix16(10), y - Fix16(1), y + Fix16(1), z);
                    break;
                }

                case 4:
                {
                    // Written out: here the original inlines operator+ for high_z
                    Fix16 z = field_50_car_sprite->field_1C_zpos;
                    rect.SetRect_41E350(field_50_car_sprite->field_14_xy.x - Fix16(10),
                                        field_50_car_sprite->field_14_xy.x,
                                        field_50_car_sprite->field_14_xy.y - Fix16(1),
                                        field_50_car_sprite->field_14_xy.y + Fix16(1));
                    rect.field_10_low_z = z - kCollisionPrismHalfHeight_6771E4;
                    rect.field_14_high_z = z + kCollisionPrismHalfHeight_6771E4;
                    break;
                }

                default:
                    FatalError_4A38C0(0x431, "C:\\Splitting\\Gta2\\Source\\car.cpp", 5458, 0);
            }

            if (gSpriteGrid_1_679208->CheckRectForCollisions_477F60(&rect, 0, sprite_types_enum::none_0, this->field_50_car_sprite))
            {
                TryHonkHorn_4416D0(0);
            }
        }
    }

    Car** pTrainCarsArray = gPublicTransport_181C_6FF1D4->GetCarArrayFromLeadCar_579B40(this);

    if (field_88_despawn_status != car_despawn_status::despawning_5)
    {
        gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(field_50_car_sprite);
    }

    for (Car* pTrainCarIter_ = *pTrainCarsArray; pTrainCarIter_; pTrainCarIter_ = pTrainCarsArray[train_car_idx_])
    {
        if (pTrainCarIter_->field_88_despawn_status != car_despawn_status::despawning_5)
        {
            gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(pTrainCarIter_->field_50_car_sprite);
        }
        ++train_car_idx_;
    }

    if (UpdateCarDespawnStatus_4424C0())
    {
        return 1;
    }

    if (field_58_physics)
    {
        if (!field_5C_AI)
        {
            field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
            field_5C_AI->SetCar_453BF0(this);
        }

        if (field_5C_AI)
        {
            field_5C_AI->UpdateTrainMovement_453A40();
        }
        ServicePhysics_442190();
    }

    UpdateTrainCarriagesOnTrack_4413B0(this->field_50_car_sprite->field_14_xy.x,
                                       this->field_50_car_sprite->field_14_xy.y,
                                       this->field_50_car_sprite->field_1C_zpos);
    ServiceDoorsLightsAndHorn_4426D0();
    ManageDespawning_442310();
    CountDownToWreck_441360();

    if (this->field_0_attachments.field_0_p18)
    {
        field_0_attachments.PoolUpdate_5A6F70(field_50_car_sprite);
        field_0_attachments.PropagateMaxZLayer_5A72B0(field_50_car_sprite, 0);
    }

    if (this->field_88_despawn_status != car_despawn_status::despawning_5)
    {
        gSpriteGrid_1_679208->AddToRegionBuckets_477B20(field_50_car_sprite);
    }

    train_car_idx_ = 0;
    for (Car* pTrainCarIter = *pTrainCarsArray; pTrainCarIter; pTrainCarIter = pTrainCarsArray[train_car_idx_])
    {
        if (pTrainCarIter->field_88_despawn_status != car_despawn_status::despawning_5)
        {
            gSpriteGrid_1_679208->AddToRegionBuckets_477B20(pTrainCarIter->field_50_car_sprite);
        }
        ++train_car_idx_;
    }

    LightUpdate_442D10();
    return 0;
}

MATCH_FUNC(0x443130)
bool Car::TrailerUpdate_443130()
{
    const s32 state = field_64_pTrailer->Update_408220();
    switch (state)
    {
        case 0:
            return 0;
        case 1:
            gTrailerPool_66AC80->field_0_pool.DeAllocate(field_64_pTrailer);
            return 1;
        case 2:
            Car::DetachTrailer_442760();
            break;
    }
    return 0;
}

MATCH_FUNC(0x443170)
char_type Car::PoolUpdate()
{
    if (!this->field_76_last_seen_timer)
    {
        gCarManager_6C_677930->field_55_visible_cars_count++;
    }

    UpdateExploderTimer_444020();

    if (this->field_64_pTrailer)
    {
        return TrailerUpdate_443130();
    }

    if (IsTrainModel_403BA0())
    {
        return TrainUpdate_442D70();
    }

    if (this->field_88_despawn_status != car_despawn_status::despawning_5)
    {
        gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(this->field_50_car_sprite);
    }

    if (UpdateCarDespawnStatus_4424C0())
    {
        return 1;
    }

    if (this->field_58_physics)
    {
        CarAI* pAi = this->field_5C_AI;
        if (pAi)
        {
            Ped* pDriver = this->field_54_driver;
            if (pDriver)
            {
                if (pDriver->GetPedType_420B70() == ped_type::player_2 && this->field_84_car_info_idx == car_model_enum::TRAINCAB || pDriver->GetPedType_420B70() != ped_type::player_2)
                {
                    pAi->AI_Service_453BB0();
                }
            }
            field_5C_AI->field_68_car_in_collision = 0;
            field_5C_AI->field_24_bf.bFlag12 = false;
        }
        ServicePhysics_442190();
    }

    if ((this->field_78_flags & car_flag::Crushing))
    {
        stru_67737C.PropagateMaxZLayer_5A72B0(this->field_50_car_sprite, 1);
        stru_67737C.ClearList_5A6E10();
    }

    if (this->field_0_attachments.field_0_p18)
    {
        this->field_0_attachments.PoolUpdate_5A6F70(this->field_50_car_sprite);
        this->field_0_attachments.PropagateMaxZLayer_5A72B0(this->field_50_car_sprite, 0);
    }

    if (this->field_58_physics)
    {
        ManageDrowning_43E560();
    }

    if (!this->field_64_pTrailer && (gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->info_flags & car_info_flag::truck_cab) == car_info_flag::truck_cab)
    {
        TryHitchTrailer_442810();
    }

    ServiceDoorsLightsAndHorn_4426D0();
    ManageDespawning_442310();
    CountDownToWreck_441360();

    if (field_88_despawn_status != car_despawn_status::despawning_5)
    {
        if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & car_info_flag::taxi_light) == car_info_flag::taxi_light)
        {
            if (field_54_driver)
            {
                if (field_9C_engine_status == car_engine_status::on_3 && !field_4_passengers_list.field_0_pFirstPed && field_8C_damage_level < 3u)
                {
                    gTaxi_4_704130->PushTaxi_457BA0(this);
                }
            }
        }
        gSpriteGrid_1_679208->AddToRegionBuckets_477B20(field_50_car_sprite);
    }
    return 0;
}

MATCH_FUNC(0x443330)
void Car::UpdateOnCraneHook_443330()
{
    Car::ServiceDoorsLightsAndHorn_4426D0();
    if (field_0_attachments.field_0_p18)
    {
        field_0_attachments.PoolUpdate_5A6F70(field_50_car_sprite);
        field_0_attachments.PropagateMaxZLayer_5A72B0(field_50_car_sprite, 0);
    }
    Car::CountDownToWreck_441360();
}

MATCH_FUNC(0x443360)
bool Car::UpdateAttachedToSprite_443360(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot)
{
    gSpriteGrid_3_679210->Remove_477B00(field_50_car_sprite);

    Fix16 sprite_x;
    Fix16 sprite_y;

    if (x != gFix16_6777CC || y != gFix16_6777CC)
    {
        sprite_x = x;
        sprite_y = y;
        Ang16::RotateVector_41FC90(sprite_x, sprite_y, pSprite->field_0_rotation);

        sprite_x = pSprite->field_14_xy.x + sprite_x;
        sprite_y = pSprite->field_14_xy.y + sprite_y;
    }
    else
    {
        sprite_x = pSprite->field_14_xy.x;
        sprite_y = pSprite->field_14_xy.y;
    }

    field_50_car_sprite->set_xyz_lazy_420600(sprite_x, sprite_y, pSprite->field_1C_zpos);
    Ang16 new_rotation = pSprite->field_0_rotation;
    new_rotation += rot;
    field_50_car_sprite->set_ang_lazy_420690(new_rotation);

    switch (pSprite->field_30_sprite_type_enum)
    {
        case sprite_types_enum::car_2:
            field_50_car_sprite->SetDrawLayer_40F7B0(18);
            break;

        case sprite_types_enum::object_1:
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::map_obj_5:
            field_50_car_sprite->SetDrawLayer_40F7B0(17);
            break;

        default:
            break;
    }

    ServiceDoorsLightsAndHorn_4426D0();
    CountDownToWreck_441360();

    if (field_0_attachments.field_0_p18)
    {
        field_0_attachments.PoolUpdate_5A6F70(field_50_car_sprite);
        field_0_attachments.PropagateMaxZLayer_5A72B0(field_50_car_sprite, 0);
    }

    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_50_car_sprite);
    return 0;
}

MATCH_FUNC(0x4435a0)
void Car::InitDoors_4435A0()
{
    InitDoors_441A70();
}

MATCH_FUNC(0x4435b0)
s32 Car::GetAttachedDrawLayer_4435B0()
{
    if ((field_78_flags & car_flag::Flag4))
    {
        return 14;
    }

    if ((field_78_flags & car_flag::Flag6))
    {
        return 20;
    }

    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & car_info_flag2::heavy) != car_info_flag2::heavy)
    {
        return 17;
    }
    return 22;
}

MATCH_FUNC(0x4435f0)
void Car::UpdateDrawLayer_4435F0()
{
    if (IsTrailerInfo_421640())
    {
        this->field_50_car_sprite->SetDrawLayer_40F7B0(16);
    }
    else
    {
        if ((field_78_flags & car_flag::Flag4))
        {
            this->field_50_car_sprite->SetDrawLayer_40F7B0(13);
        }
        else if ((field_78_flags & car_flag::Flag6))
        {
            this->field_50_car_sprite->SetDrawLayer_40F7B0(19);
        }
        else if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & car_info_flag2::heavy) == car_info_flag2::heavy)
        {
            this->field_50_car_sprite->SetDrawLayer_40F7B0(21);
        }
        else
        {
            this->field_50_car_sprite->SetDrawLayer_40F7B0(15);
        }
    }
}

// 9.6f 0x426580
MATCH_FUNC(0x443710)
void Car::ApplyExplosionImpulse_443710(Fix16_Point* xy)
{
    Fix16_Point hitch_point;
    Fix16_Point impulse_dir;
    Fix16_Point impulse_pos;

    if (!IsDeactivated_4214D0())
    {
        SetupCarPhysicsAndSpriteBinding_43BCA0();

        hitch_point = field_50_car_sprite->get_x_y();
        hitch_point.x += kFpQuarter_677208 * gCarManager_6C_677930->field_1C_explosion_offset_cycle - kFpHalf_6772D0;
        hitch_point.y += kFpQuarter_677208 * gCarManager_6C_677930->field_1C_explosion_offset_cycle - kFpHalf_6772D0;

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
            impulse_pos = impulse_dir.NormalizeSafe_442AD0().DivideInl_442CB0(vecLen);
        }
        field_58_physics->SetCurrentCarInfoAndModelPhysics_562EF0();
        field_58_physics->ApplyImpactForcesAndDamage_55FA60(hitch_point, impulse_pos, 10);
    }

done:
    gCarManager_6C_677930->field_1C_explosion_offset_cycle++;
    if (gCarManager_6C_677930->field_1C_explosion_offset_cycle > 4)
    {
        gCarManager_6C_677930->field_1C_explosion_offset_cycle = 0;
    }
}

MATCH_FUNC(0x443A50)
s32 __stdcall Car::GetCarWeaponCost_443A50(s32 weapon_kind)
{
    if (gCarManager_6C_677930->field_69_do_free_shopping)
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
void Car::BuyCarWeapon_4438C0(s32 weapon_kind)
{
    u8 ammo_capacity;
    Player* pPlayer = GetDriverPlayer_421870();
    Weapon* pWeapon = pPlayer->field_718_weapons[weapon_kind];
    if (pWeapon && pWeapon->is_max_capacity_5DCEA0())
    {
        // Ammo full
        if (pPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(1, "arig");
        }
    }
    else
    {
        s32 car_weapon_cost = Car::GetCarWeaponCost_443A50(weapon_kind);
        if (bStartNetworkGame_7081F0)
        {
            // Cheaper in multiplayer
            car_weapon_cost /= 10;
        }

        if (car_weapon_cost <= pPlayer->field_2D4_scores.GetScore_592370())
        {
            if (pPlayer->IsUser_41DC70())
            {
                gHud_2B00_706620->field_DC_brief.SetHudBrief_5D3F10(1, "bdone", car_weapon_cost);
            }

            pPlayer->field_2D4_scores.AddCash_592620(-car_weapon_cost);
            if (gWeapon_8_707018->get_max_ammo_capacity_5E3E70(weapon_kind) < 10u)
            {
                ammo_capacity = gWeapon_8_707018->get_max_ammo_capacity_5E3E70(weapon_kind);
            }
            else
            {
                ammo_capacity = 10;
            }
            gWeapon_8_707018->allocate_5E3D50(weapon_kind, ammo_capacity, this);
            switch (weapon_kind)
            {
                case weapon_type::car_bomb:
                    this->SetWeaponKind_421950(4);
                    break;
                case weapon_type::car_mines:
                    this->SetWeaponKind_421950(5);
                    break;
                case weapon_type::oil_stain:
                    this->SetWeaponKind_421950(6);
                    break;
                case weapon_type::car_smg:
                    this->SetWeaponKind_421950(7);
                    break;
                default:
                    FatalError_4A38C0(0x431, "C:\\Splitting\\Gta2\\Source\\car.cpp", 6107, 0);
            }
        }
        else
        {
            // Can't afford weapon
            Car::ShowCantAffordMessage_443AB0(pPlayer, car_weapon_cost);
            this->SetWeaponKind_421950(8);
        }
    }
}

MATCH_FUNC(0x443AB0)
void __stdcall Car::ShowCantAffordMessage_443AB0(Player* pPlayer, s32 weapon_cost)
{
    if (pPlayer->IsUser_41DC70())
    {
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D3F10(1, "nspraya", weapon_cost);
    }
}

MATCH_FUNC(0x443ae0)
void Car::ResprayOrChangePlates(u8 remap)
{
    Player* pPlayer = GetDriverPlayer_421870();
    const s32 cost = gCarManager_6C_677930->field_69_do_free_shopping != 0 ? 0 : 5000;
    if (cost <= pPlayer->field_2D4_scores.GetScore_592370())
    {
        if (pPlayer->IsUser_41DC70())
        {
            if (remap == car_shop_type::change_plates_253) // clean plates only
            {
                gHud_2B00_706620->field_DC_brief.SetHudBrief_5D3F10(1, "cdone", cost);
            }
            else
            {
                gHud_2B00_706620->field_DC_brief.SetHudBrief_5D3F10(1, "sdone", cost);
            }
        }

        if (remap != car_shop_type::change_plates_253) // respray
        {
            SetCarRemap(remap);
            field_0_attachments.ClearGangIconSprite_5A7110();
            this->SetWeaponKind_421950(1);
        }
        else
        {
            this->SetWeaponKind_421950(2);
        }

        pPlayer->field_2D4_scores.AddCash_592620(-cost);
        this->field_54_driver->field_20A_wanted_points = cop_level_ped_enum::cop_0_stars;
        RemoveAllDamage();
    }
    else
    {
        ShowCantAffordMessage_443AB0(pPlayer, cost);
        this->SetWeaponKind_421950(8);
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
        // I ain't touching that get outta here!
        gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(1, "nespray");
    }
}

MATCH_FUNC(0x443c40)
void Car::HandleShops_443C40(Object* pObj)
{
    if (HasPlayerDriver_4215C0() && pObj->field_18_model == objects::car_shop_130)
    {
        const u8 idx = pObj->get_field_26_420FF0();
        if (idx >= car_shop_type::car_smg_250 && (idx <= car_shop_type::mines_252 || idx == car_shop_type::car_bomb_254))
        {
            Car::BuyCarWeapon_4438C0(GetDriverPlayer_421870()->ObjectTypeToWeaponType_443CB0(idx));
        }
        else
        {
            Car::ResprayOrCleanPlates(idx);
        }
    }
}

MATCH_FUNC(0x443d00)
void Car::SetPosition_443D00(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(field_50_car_sprite);
    field_50_car_sprite->set_xyz_lazy_420600(xpos, ypos, zpos);
    if (field_58_physics)
    {
        field_58_physics->SetSprite_563560(field_50_car_sprite);
    }
    gSpriteGrid_1_679208->AddToRegionBuckets_477B20(field_50_car_sprite);
}

MATCH_FUNC(0x443d70)
void Car::IncrementCarStats_443D70(s32 car_kind)
{
    IncrementAllocatedCarType_443DA0(car_kind);

    if (IsTrailerCab_41E460())
    {
        field_64_pTrailer->field_C_trailer_car->IncrementAllocatedCarType_443DA0(car_kind);
    }
}

MATCH_FUNC(0x443da0)
void Car::IncrementAllocatedCarType_443DA0(s32 car_kind)
{
    this->field_A0_car_kind = car_kind;
    switch (car_kind)
    {
        case car_kind::recycled_1:
            ++gCarManager_6C_677930->field_28_recycled_cars;
            break;
        case car_kind::proto_recycled_2:
            ++gCarManager_6C_677930->field_40_proto_recycled_cars;
            break;
        case car_kind::firefighter_5:
            ++gCarManager_6C_677930->field_30_firefighter_cars;
            break;
        case car_kind::Unknown_10:
            ++gCarManager_6C_677930->field_48_kind10_cars;
            break;
        case car_kind::paramedic_car_4:
            ++gCarManager_6C_677930->field_2C_paramedic_cars;
            break;
        case car_kind::police_6:
            ++gCarManager_6C_677930->field_34_unit_cars;
            break;
        case car_kind::roadblock_car_7:
            ++gCarManager_6C_677930->field_38_roadblock_cars;
            break;
        case car_kind::mission_car_8:
            ++gCarManager_6C_677930->field_3C_mission_cars;
            break;
        case car_kind::parked_car_9:
            ++gCarManager_6C_677930->field_44_parked_cars;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x443e50)
void Car::MarkProtoRecycled_443E50()
{
    if (field_A0_car_kind == car_kind::recycled_1)
    {
        gCarManager_6C_677930->field_28_recycled_cars--;
        gCarManager_6C_677930->field_40_proto_recycled_cars++;
        field_A0_car_kind = car_kind::proto_recycled_2;
    }
}

MATCH_FUNC(0x443e80)
void Car::MarkRecycled_443E80()
{
    if (field_A0_car_kind == car_kind::proto_recycled_2)
    {
        gCarManager_6C_677930->field_28_recycled_cars++;
        gCarManager_6C_677930->field_40_proto_recycled_cars--;
        field_A0_car_kind = car_kind::recycled_1;
    }
}

MATCH_FUNC(0x443eb0)
void Car::ReassignCarStats_443EB0(s32 car_kind)
{
    gCarManager_6C_677930->DecrementAllocatedCarType_4466C0(field_A0_car_kind);
    IncrementCarStats_443D70(car_kind);
}

MATCH_FUNC(0x443ee0)
void Car::ReassignAllocatedCarType_443EE0(s32 car_kind)
{
    gCarManager_6C_677930->DecrementAllocatedCarType_4466C0(field_A0_car_kind);
    IncrementAllocatedCarType_443DA0(car_kind);
}

MATCH_FUNC(0x443f30)
void Car::AttachLight_443F30(s32 object_type, s32 argb, s32 x_offset, s32 y_offset)
{
    Object* pObj = gObject_5C_6F8F84->NewLight_529AB0(object_type, 0, 0, 0, argb, kFpOneAndHalf_6772AC, 200);
    pObj->TurnOffLight_527990();
    field_50_car_sprite->AttachSprite_5A3100(pObj->field_4_sprite, (kFpOne64th_677888 * x_offset), (kFpOne64th_677888 * y_offset), kAngZero_67791C);
}

MATCH_FUNC(0x444020)
void Car::UpdateExploderTimer_444020()
{
    if (field_94_exploder_timer > 0)
    {
        field_94_exploder_timer--;
        if (field_94_exploder_timer == 0)
        {
            field_70_exploder_ped_id = 0;
            field_90_death_cause = ped_death_cause::none_0;
        }
    }
}

MATCH_FUNC(0x444090)
u32 Car::GetEffectiveDriverPedId_444090()
{
    Car* pCar = this;
    while (!pCar->field_54_driver)
    {
        if (!pCar->field_64_pTrailer || pCar->field_64_pTrailer->field_C_trailer_car != pCar)
        {
            return pCar->field_70_exploder_ped_id;
        }
        pCar = pCar->field_64_pTrailer->field_8_truck_cab;
    }

    Ped* pDriver = pCar->field_54_driver;
    if (pDriver->get_occupation_403980() == ped_ocupation_enum::dummy_driver_4)
    {
        return pCar->field_70_exploder_ped_id;
    }

    if (!IsPlayerPedId_420C30(pDriver->field_200_id))
    {
        if (pCar->field_70_exploder_ped_id)
        {
            return pCar->field_70_exploder_ped_id;
        }
    }
    return pDriver->field_200_id;
}

/*
OBS: it is not a switch case :)
*/
MATCH_FUNC(0x4441b0)
void Car::SetSirens_4441B0()
{
    // car sirens?
    if (IsMediCar_421780())
    {
        Car::AttachLight_443F30(165, 0xFF2010, 10, 16);
        Car::AttachLight_443F30(171, 0xFF2010, -10, 16);
        Car::AttachLight_443F30(172, 0xFF2010, 10, -32);
        Car::AttachLight_443F30(173, 0xFF2010, -10, -32);
    }
    else if (IsCopCar_421790())
    {
        Car::AttachLight_443F30(165, 0xFF2010, 10, -16);
        Car::AttachLight_443F30(171, 255, -10, -16);
    }
    else if (IsSwatVan_4217A0())
    {
        Car::AttachLight_443F30(165, 0xFF2010, 10, 32);
        Car::AttachLight_443F30(171, 255, -10, 32);
        Car::AttachLight_443F30(172, 0xFF2010, 10, -32);
        Car::AttachLight_443F30(173, 255, -10, -32);
    }
    else if (IsFireTruck_4118F0())
    {
        Car::AttachLight_443F30(165, 0xFF2010, 16, 48);
        Car::AttachLight_443F30(171, 0xFF2010, -16, 48);
        Car::AttachLight_443F30(172, 0xFF2010, 16, -16);
        Car::AttachLight_443F30(173, 0xFF2010, -16, -26);
    }
    else if (IsFbiCar_411920())
    {
        Car::AttachLight_443F30(165, 0xFF2010, 0, 16);
    }
}

MATCH_FUNC(0x444490)
void Car::PoolAllocate()
{
    this->field_6C_car_id = gCarManager_6C_677930->field_14_next_car_id++;
    this->field_74_damage = 0;
    this->field_8C_damage_level = 0;
    this->field_8_damaged_areas.ClearAllBits_420D90();
    this->field_4_passengers_list.ClearList_420E90();
    this->field_54_driver = 0;
    this->field_98_door_lock = Car_Door_Lock::unlocked_3;
    this->field_58_physics = 0;
    this->field_A4_light_flags = 0;
    this->field_A5_flash_phase_counter = 0;
    this->field_76_last_seen_timer = 0;
    this->field_7C_ped_type = ped_type::dummy_3;
    this->field_50_car_sprite = 0;
    this->field_9C_engine_status = car_engine_status::off_1;
    this->field_A6_turn_flags = 0;
    this->field_80_abandoned = 0;
    this->field_78_flags = 0;
    //clear();

    this->field_0_attachments.DestroyAllSprites_5A7010();

    ClearHorn_421460();
    IncrementCarStats_443D70(car_kind::none_0);
    this->field_8D_car_thread_flags = 0;
    this->field_60_pChaseTask = 0;
    this->field_70_exploder_ped_id = 0;
    this->field_90_death_cause = ped_death_cause::none_0;
    this->field_94_exploder_timer = 0;
    this->field_95_player_ped_id = 0;
    //v3 = kFP16One_6777D0;
    this->field_68_scale = kFP16One_6777D0;

    this->field_8E_flash_count = 0;
    this->field_A8_horn_cooldown = 0;
    this->field_A9_wreck_countdown = 0;
    this->field_B4_weapon_kind = 0;
    this->field_B8_turret_rotated = 0;
    this->field_B0_emitter_status = 0;
}

MATCH_FUNC(0x4446e0)
void Car::DeAllocateAI_4446E0()
{
    if (field_5C_AI)
    {
        if (field_5C_AI->field_28_route_idx > 0)
        {
            gRouteFinder_6FFDC8->CancelRoute_589930(field_5C_AI->field_28_route_idx);
        }

        gCarAI_78_Pool_677CF8->DeAllocate(field_5C_AI);
        field_5C_AI = 0;
    }
}

MATCH_FUNC(0x4447d0)
void Car::PoolDeallocate()
{
    this->field_0_attachments.DestroyAllSprites_5A7010();

    DeAllocateCarPhysics_441A10();

    if (field_50_car_sprite)
    {
        gSpritePool_703818->remove(field_50_car_sprite);
        field_50_car_sprite = 0;
    }

    DeAllocateAI_4446E0();

    if (field_60_pChaseTask)
    {
        gCarChaseTaskTable_678E30->FreeEntry_474CC0(field_60_pChaseTask);
        field_60_pChaseTask = 0;
    }

    KillContainedPeds_43DB80();

    gWeapon_8_707018->dealloc_car_weapon_5E3DF0(this);
    gCarManager_6C_677930->DecrementAllocatedCarType_4466C0(field_A0_car_kind);

    field_6C_car_id = 0;
}

MATCH_FUNC(0x444860)
Car::Car()
{
    field_54_driver = 0;
    field_74_damage = 0;
    field_8C_damage_level = 0;
    field_98_door_lock = Car_Door_Lock::no_lock_0;
    field_9C_engine_status = 0;
    field_7C_ped_type = ped_type::unknown_0;
    field_76_last_seen_timer = 0;
    field_A4_light_flags = 0;
    field_A5_flash_phase_counter = 0;
    field_A6_turn_flags = 0;
    mpNext = 0;
    field_84_car_info_idx = car_model_enum::none;
    field_50_car_sprite = 0;
    field_58_physics = 0;
    field_5C_AI = 0;
    field_88_despawn_status = car_despawn_status::none_0;
    field_6C_car_id = 0xFFFF;
    field_64_pTrailer = 0;
    field_78_flags = 0;
    this->field_0_attachments.DestroyAllSprites_5A7010();
    ClearHorn_421460();
    field_80_abandoned = 0;
    field_A0_car_kind = car_kind::none_0;
    field_8D_car_thread_flags = 0;
    field_70_exploder_ped_id = 0;
    field_90_death_cause = ped_death_cause::none_0;
    field_94_exploder_timer = 0;
    field_95_player_ped_id = 0;
    field_68_scale.mValue = 0x4000;
    field_8E_flash_count = 0;
}

MATCH_FUNC(0x444960)
Car::~Car()
{
    mpNext = 0;
    field_50_car_sprite = 0;
    field_5C_AI = 0;
}

MATCH_FUNC(0x447360)
void Car::ClearHeadlightDeltas_447360()
{
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & car_info_flag2::popup_headlights) == car_info_flag2::popup_headlights)
    {
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor1_11);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor2_12);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor3_13);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopRightDoor4_14);

        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor1_28);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor2_29);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor3_30);
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::TopLeftDoor4_31);
    }
    field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontRightHeadlight_6);
    field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::FrontLeftHeadlight_23);
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & car_info_flag::roof_light) == car_info_flag::roof_light)
    {
        field_8_damaged_areas.clear_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }
}

MATCH_FUNC(0x564300)
bool Car::IsDrivenByNonPlayer_564300()
{
    if (field_54_driver)
    {
        bool result = field_54_driver->PedTypeIs_45EDE0(ped_type::player_2) == 0;
        return result;
    }
    return false;
}

MATCH_FUNC(0x407B80)
Ped* Trailer::GetTruckCabDriver_407B80()
{
    return field_8_truck_cab->field_54_driver;
}

MATCH_FUNC(0x407b90)
Car* Trailer::GetCabOrTrailerCar_407B90(Car* pCar)
{
    if (pCar == field_8_truck_cab)
    {
        return field_C_trailer_car;
    }
    return field_8_truck_cab;
}

MATCH_FUNC(0x407bb0)
void Trailer::SetTruckCabAndTrailerCar_407BB0(Car* pTruckCab, Car* pTrailerCar)
{
    this->field_8_truck_cab = pTruckCab;
    this->field_C_trailer_car = pTrailerCar;

    pTruckCab->field_64_pTrailer = this;
    this->field_C_trailer_car->field_64_pTrailer = this;
    this->field_0_hitch_angle_clamped = 0;
}

MATCH_FUNC(0x407bd0)
Fix16_Point Trailer::GetHitchPosition_407BD0()
{
    // The y line calls the out-of-line Negate/Multiply/operator+ in the original, as if it
    // were written with them; with the inline RotateByAngle_40F6B0 the tail stops inlining.
    // Assigning a constructed point (not a copy) loads y before x as in the original.
    // The rotation's locals live in their own block (as if from an inline rotate): without it the
    // x_old store is scheduled after the angle load.
    Fix16_Point offset;
    offset = Fix16_Point(gTrailerHitchOffset_66AAC8.x, gTrailerHitchOffset_66AAC8.y);
    Ang16 angle = field_8_truck_cab->field_58_physics->field_58_theta;
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);
        Fix16 x_old = offset.x;
        offset.x = (offset.x * cos) + (offset.y * sin);
        offset.y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(offset.y.Multiply_408680(cos));
    }
    offset += field_8_truck_cab->field_58_physics->get_cp1_40B560();
    return offset;
}

// 9.6f 0x40F840 CarPhysics::IsVelocityAlignedWithHeading_40F840, inlined here with car.cpp's own
// static copies of the 90/270 degree constants
static inline bool IsVelocityAlignedWithHeading_40F840(CarPhysics* pPhys)
{
    Ang16 drift = pPhys->field_40_linvel_1.atan2_40F790().SubtractNormalized_409340(pPhys->field_58_theta);
    return drift <= kAng90_66A9C8 || drift >= kAng270_66AABC;
}

MATCH_FUNC(0x407ce0)
void Trailer::UpdateTrailerAlignment_407CE0()
{
    // The four points are declared up front: the original enters with EH state 3. The 9.6f getters and
    // setters (get/set_theta_40F820/40F830, set_cp1_40F7E0, get/set_cp3) are written as field accesses
    // and the Fix16 compares on mValue, otherwise the inline budget pushes these ctors out of line.
    Fix16_Point rear;
    Fix16_Point hitch;
    Fix16_Point delta;
    Fix16_Point offset;

    CarPhysics* pTrailerPhys = field_C_trailer_car->field_58_physics;
    CarPhysics* pCabPhys = field_8_truck_cab->field_58_physics;
    Fix16 trailer_theta = Ang16::Ang16_to_Fix16(pTrailerPhys->field_58_theta);

    rear = field_C_trailer_car->GetRearWheelOffset_43A0E0();
    rear.RotateByAngle_40F6B0(pTrailerPhys->field_58_theta);
    rear += pTrailerPhys->get_cp1_40B560();

    hitch = gTrailerHitchOffset_66AAC8;
    Fix16 cab_theta = Ang16::Ang16_to_Fix16(pCabPhys->field_58_theta);
    hitch.RotateByAngle_40F6B0(Ang16::Fix16_To_Ang16_40F540(cab_theta));
    hitch += pCabPhys->get_cp1_40B560();

    // The trailer points along hitch - rear, kept within a window around the cab's angle
    delta = hitch.Sub_40AC80(rear);
    Fix16 new_theta = Ang16::Ang16_to_Fix16(delta.atan2_40F790());
    field_0_hitch_angle_clamped = ClampAngleAroundTarget_405E80(&cab_theta, &new_theta);

    if (new_theta.mValue != cab_theta.mValue && new_theta.mValue == trailer_theta.mValue)
    {
        if (IsVelocityAlignedWithHeading_40F840(pCabPhys) && pCabPhys->IsGasPedalPressedEnough_5626A0())
        {
            new_theta = TurnAngleToward_405DA0(new_theta, &cab_theta, &field_8_truck_cab->GetMaxTurnRate_440510());
        }
    }

    pTrailerPhys->field_58_theta = Ang16::Fix16_To_Ang16_40F540(new_theta);

    offset = gTrailerCabOffset_66AAE0;
    offset.RotateByAngle_40F6B0(Ang16::Fix16_To_Ang16_40F540(new_theta));
    field_C_trailer_car->field_58_physics->field_38_cp1 = hitch.Sub_40AC80(offset);
    pTrailerPhys->field_6C_cp3 = field_8_truck_cab->field_58_physics->field_6C_cp3;
    pTrailerPhys->UpdateCenterOfMassPoint_563350();
}

MATCH_FUNC(0x408140)
bool Trailer::ServicePhysics_408140()
{
    if (!field_8_truck_cab->field_58_physics && !field_C_trailer_car->field_58_physics)
    {
        return 0;
    }

    SetupCarPhysicsAndSpriteBinding_408190();

    if (!field_8_truck_cab->field_58_physics->ProcessCarPhysicsStateMachine_562FE0() || field_8_truck_cab->field_54_driver)
    {
        return field_8_truck_cab->ManageDrowning_43E560();
    }
    DeAllocateCarPhysics_4081B0();
    return 0;
}

MATCH_FUNC(0x408190)
void Trailer::SetupCarPhysicsAndSpriteBinding_408190()
{
    field_8_truck_cab->SetupCarPhysicsAndSpriteBinding_43BC30();
    field_C_trailer_car->SetupCarPhysicsAndSpriteBinding_43BC30();
}

MATCH_FUNC(0x4081b0)
void Trailer::DeAllocateCarPhysics_4081B0()
{
    field_8_truck_cab->DeAllocateCarPhysics_441A10();
    field_C_trailer_car->DeAllocateCarPhysics_441A10();
}

MATCH_FUNC(0x4081d0)
bool Trailer::ExplodeBothIfOneDestroyed_4081D0()
{
    if (field_8_truck_cab->field_74_damage == 32001)
    {
        if (field_C_trailer_car->field_74_damage != 32001)
        {
            field_C_trailer_car->field_74_damage = 32000;
            field_C_trailer_car->HandleCarExplosion_43D840(explosion_type::small_18);
        }
        return 1;
    }
    else if (field_C_trailer_car->field_74_damage == 32001)
    {
        field_8_truck_cab->field_74_damage = 32000;
        field_8_truck_cab->HandleCarExplosion_43D840(explosion_type::small_18);
        return 1;
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x408220)
s32 Trailer::Update_408220()
{
    if (field_8_truck_cab->field_88_despawn_status != car_despawn_status::despawning_5)
    {
        gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(field_8_truck_cab->field_50_car_sprite);
        gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(field_C_trailer_car->field_50_car_sprite);
    }

    const bool despawn_status = field_8_truck_cab->UpdateCarDespawnStatus_4424C0();
    field_C_trailer_car->field_88_despawn_status = field_8_truck_cab->field_88_despawn_status;
    if (despawn_status)
    {
        gCar_BC_Pool_67792C->Remove(field_C_trailer_car);
        return 1;
    }
    else
    {
        const bool bCabDrowning = ServicePhysics_408140();
        if (field_8_truck_cab->field_50_car_sprite->field_1C_zpos != kFpZero_66AB38)
        {
            if (field_C_trailer_car->field_50_car_sprite->field_1C_zpos != kFpZero_66AB38)
            {
                field_8_truck_cab->field_50_car_sprite->ResolveZOrder_5A1B30(field_C_trailer_car->field_50_car_sprite);
            }
        }

        field_8_truck_cab->field_50_car_sprite->SetDrawLayer_40F7B0(15);

        field_8_truck_cab->ServiceDoorsLightsAndHorn_4426D0();
        field_C_trailer_car->ServiceDoorsLightsAndHorn_4426D0();

        if (field_8_truck_cab->field_88_despawn_status != car_despawn_status::despawning_5)
        {
            gSpriteGrid_1_679208->AddToRegionBuckets_477B20(field_8_truck_cab->field_50_car_sprite);
            gSpriteGrid_1_679208->AddToRegionBuckets_477B20(field_C_trailer_car->field_50_car_sprite);
        }

        if (field_8_truck_cab->field_0_attachments.field_0_p18)
        {
            field_8_truck_cab->field_0_attachments.PoolUpdate_5A6F70(field_8_truck_cab->field_50_car_sprite);
            field_8_truck_cab->field_0_attachments.PropagateMaxZLayer_5A72B0(field_8_truck_cab->field_50_car_sprite, 0);
        }

        if (field_C_trailer_car->field_0_attachments.field_0_p18)
        {
            field_C_trailer_car->field_0_attachments.PoolUpdate_5A6F70(field_C_trailer_car->field_50_car_sprite);
            field_C_trailer_car->field_0_attachments.PropagateMaxZLayer_5A72B0(field_C_trailer_car->field_50_car_sprite, 0);
        }

        if (ExplodeBothIfOneDestroyed_4081D0() || bCabDrowning)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }
}

MATCH_FUNC(0x5822E0)
void ResetTrafficSpawnGlobals_5822E0()
{
    gTrafficDensityScale_6FF7E8 = kFpZero_6FF774;
    gTrafficCarSpeed_6FF570 = kDefaultTrafficCarSpeed_6FF744;
    DAT_006FF8C4 = 0;
    DAT_006FF8C5 = 0;
}

MATCH_FUNC(0x582310)
TrafficSpawner::TrafficSpawner()
{
    field_9_traffic_car_model = 0;
    field_A_unused = 0;
    field_8_spawn_side = 0;
    field_0_cam = NULL;
    field_C_player = NULL;
    field_10_zone = NULL;
    field_4_rect = new Fix16_Rect();
    ResetTrafficSpawnGlobals_5822E0();
}

MATCH_FUNC(0x582340)
TrafficSpawner::~TrafficSpawner()
{
    delete field_4_rect;
    field_4_rect = NULL;
    field_0_cam = NULL;
    field_C_player = NULL;
    field_10_zone = NULL;
}

MATCH_FUNC(0x582360)
bool TrafficSpawner::IsInsideCameraEdge_582360(int direction, Fix16 x, Fix16 y)
{
    switch (direction)
    {
        case 1:
        case 2:
            if (field_8_spawn_side == 0)
            {
                if (x < (field_0_cam->field_78_view_bounds.field_4_right - kFpOne_6FF778))
                {
                    return 1;
                }
            }
            else
            {
                if (x > (field_0_cam->field_78_view_bounds.field_0_left + kFpOne_6FF778))
                {
                    return 1;
                }
            }
            break;
        case 3:
        case 4:
            if (field_8_spawn_side == 0)
            {
                if (y < (field_0_cam->field_78_view_bounds.field_C_bottom - kFpOne_6FF778))
                {
                    return 1;
                }
            }
            else
            {
                if (y > (field_0_cam->field_78_view_bounds.field_8_top + kFpOne_6FF778))
                {
                    return 1;
                }
            }
            break;
    }
    return 0;
}

// 9.6f 0x4B4A60
MATCH_FUNC(0x5832C0)
void TrafficSpawner::MakeTrafficForCurrCamera_5832C0()
{
    Fix16 w;

    Fix16 wanted_related;
    if ((!bLimit_recycling_67D4CA || gCarManager_6C_677930->field_28_recycled_cars < 2) &&
        gCarManager_6C_677930->field_28_recycled_cars + gCarManager_6C_677930->field_40_proto_recycled_cars != 16)
    {
        s32 angleFace = Ang16::GetAngleFace_4F78F0(field_0_cam->ComputeTargetFacingAngle_4358D0());

        switch (gPolice_7B8_6FEE40->field_654_max_wanted_level)
        {
            case cop_level_enum::none_0:
            case cop_level_enum::police_1:
                wanted_related = kFpOne_6FF778;
                break;
            case cop_level_enum::police_3:
                wanted_related = kFpPoint6_6FF5DC;
                break;
            case cop_level_enum::police_2:
            case cop_level_enum::fbi_5:
                wanted_related = kFpPoint8_6FF5E4;
                break;
            case cop_level_enum::swat_4:
                wanted_related = kFpPoint4_6FF5D4;
                break;
            case cop_level_enum::army_6:
                wanted_related = kFpPoint4_6FF5D4;
                break;
            default:
                //wanted_related = rng_; // random value ??
                break;
        }

        // 9.6f: width and height into locals first, then multiplied
        w = field_0_cam->GetPaddedViewBoundsWidth_4B3110();
        Fix16 h = field_0_cam->GetPaddedViewBoundsHeight_4B3130();
        Fix16 t = h * w;
        t = t / Fix16(86);
        // w is declared at the top of the function: with it declared here, the field_9_traffic_car_model store after
        // gTrafficDensityScale_6FF7E8 (as in 9.6f) made VC6 compute the height first
        gTrafficDensityScale_6FF7E8 = (t)*wanted_related;
        this->field_9_traffic_car_model = 1;
        this->field_A_unused = 1;

        u8 rng_int = gRng_6F6784.get_uint8_4F7B70(5);
        bool bOwnerMoving = !!(field_0_cam->ReturnOwnerVelocity_435A20() > kFpZero_6FF580);

        switch (rng_int)
        {
            case 0:
                field_8_spawn_side = 1;
                if (!SpawnTrafficCar_582480(1, 2, 0) && !SpawnTrafficCar_582480(2, 1, 0) && !SpawnTrafficCar_582480(4, 3, 0) &&
                    !SpawnTrafficCar_582480(3, 4, 0) && gPolice_7B8_6FEE40->field_654_max_wanted_level < cop_level_enum::police_3)
                {
                    if (static_cast<u8>(bOwnerMoving))
                    {
                        SpawnTrafficCarFacing_583260(angleFace);
                    }
                }
                break;

            case 1:
                this->field_8_spawn_side = 0;
                if (!SpawnTrafficCar_582480(2, 1, 0) && !SpawnTrafficCar_582480(4, 3, 0) && !SpawnTrafficCar_582480(3, 4, 0) &&
                    (gPolice_7B8_6FEE40->field_654_max_wanted_level >= cop_level_enum::police_3 || !static_cast<u8>(bOwnerMoving) || !SpawnTrafficCarFacing_583260(angleFace)))
                {
                    SpawnTrafficCar_582480(1, 2, 0);
                }
                break;

            case 2:
                this->field_8_spawn_side = 1;
                if (!SpawnTrafficCar_582480(4, 3, 0) && !SpawnTrafficCar_582480(3, 4, 0) &&
                    (gPolice_7B8_6FEE40->field_654_max_wanted_level >= cop_level_enum::police_3 || !static_cast<u8>(bOwnerMoving) || !SpawnTrafficCarFacing_583260(angleFace)) &&
                    !SpawnTrafficCar_582480(1, 2, 0))
                {
                    SpawnTrafficCar_582480(2, 1, 0);
                }
                break;

            case 3:
                this->field_8_spawn_side = 0;
                if (!SpawnTrafficCar_582480(3, 4, 0) &&
                    (gPolice_7B8_6FEE40->field_654_max_wanted_level >= cop_level_enum::police_3 || !static_cast<u8>(bOwnerMoving) || !SpawnTrafficCarFacing_583260(angleFace)) &&
                    !SpawnTrafficCar_582480(1, 2, 0) && !TrafficSpawner::SpawnTrafficCar_582480(2, 1, 0))
                {
                    SpawnTrafficCar_582480(4, 3, 0);
                }
                break;

            case 4:
                field_8_spawn_side = 1;
                if ((gPolice_7B8_6FEE40->field_654_max_wanted_level >= cop_level_enum::police_3 || !static_cast<u8>(bOwnerMoving) || !SpawnTrafficCarFacing_583260(angleFace)) &&
                    !SpawnTrafficCar_582480(1, 2, 0) && !SpawnTrafficCar_582480(2, 1, 0) && !SpawnTrafficCar_582480(4, 3, 0))
                {
                    SpawnTrafficCar_582480(3, 4, 0);
                }
                break;

            default:
                return;
        }
    }
}

MATCH_FUNC(0x583260)
bool TrafficSpawner::SpawnTrafficCarFacing_583260(s32 angle_face)
{
    switch (angle_face)
    {
        case 1:
            return SpawnTrafficCar_582480(1, 1, 0);
        case 2:
            return SpawnTrafficCar_582480(2, 2, 0);
        case 3:
            return SpawnTrafficCar_582480(3, 3, 0);
        case 4:
            return SpawnTrafficCar_582480(4, 4, 0);
    }
    return 1;
}

// 9.6f: the y read is Ped::get_cam_y (0x403A10, inlined, using it changes the code)
MATCH_FUNC(0x583670)
void TrafficSpawner::GenerateTraffic_583670()
{
    if (!bSkip_recycling_67D575)
    {
        field_0_cam = gGame_0x40_67E008->IteratePlayerCamera_4B9BC0();
        field_C_player = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0();
        if (field_C_player)
        {
            field_10_zone = gMap_0x370_6F6268->GetZoneInfoAt_4DF890(field_C_player->field_2C4_player_ped->get_cam_x().ToInt(),
                                                                           field_C_player->field_2C4_player_ped->field_1AC_cam.y.ToInt());
        }

        while (field_0_cam)
        {
            if (field_0_cam->HasFollowTarget_433E90())
            {
                TrafficSpawner::MakeTrafficForCurrCamera_5832C0();
            }
            field_0_cam = gGame_0x40_67E008->IterateNextPlayerCamera_4B9C50();

            Ped* pPed;
            if (field_0_cam == &field_C_player->field_208_aux_game_camera)
            {
                pPed = field_C_player->field_2C8_aux_ped;
            }
            else
            {
                field_C_player = gGame_0x40_67E008->IterateNextPlayer_4B9D10();
                if (field_C_player)
                {
                    pPed = field_C_player->field_2C4_player_ped;
                }
                else
                {
                    continue;
                }
            }
            if (pPed)
            {
                field_10_zone = gMap_0x370_6F6268->GetZoneInfoAt_4DF890(pPed->get_cam_x().ToInt(), pPed->field_1AC_cam.y.ToInt());
            }
        }
    }
}

MATCH_FUNC(0x583750)
Fix16 TrafficSpawner::GetRandomTrafficSpeed_583750(Fix16 max_speed, u8* pOut)
{

    Fix16 roll;
    Fix16 lo;
    Fix16 hi;

    if (max_speed >= kFpPoint4_6FF70C)
    {
        *pOut = 2;
    }

    if (max_speed >= kFpPoint3_6FF85C)
    {
        *pOut = 2;
        roll = Fix16(gRng_6F6784.get_uint8_4F7B70(100)) / Fix16(100);
        lo = kFpPoint3_6FF85C;
        hi = kFpPoint4_6FF70C;
    }
    else if (max_speed >= kFpPoint2_6FF724)
    {
        *pOut = 1;
        roll = Fix16(gRng_6F6784.get_uint8_4F7B70(100)) / Fix16(100);
        lo = kFpPoint2_6FF724;
        hi = kFpPoint3_6FF85C;
    }
    else
    {
        *pOut = 0;
        roll = Fix16(gRng_6F6784.get_uint8_4F7B70(100)) / Fix16(100);
        lo = kFpPoint03_6FF6A4;
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
bool TrafficSpawner::SpawnTrafficCar_582480(s32 spawn_type, s32 arrow_direction, s32 blocked_direction)
{
    char_type x_step = 0;
    char_type y_step = 0;
    u8 speed_kind;
    char_type bRoadblock = 0;
    Gang* pGang = 0;
    s32 found_z;
    Fix16 ground_z;

    if (gCarManager_6C_677930->field_28_recycled_cars + gCarManager_6C_677930->field_40_proto_recycled_cars == 16 || arrow_direction == blocked_direction ||
        gCarManager_6C_677930->field_54_spawn_block > 0)
    {
        return 0;
    }

    car_info* pCarInfo = gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_9_traffic_car_model);
    Fix16 car_w = gPixelsToFix16_6F6850.list[pCarInfo->w];
    Fix16 car_h = gPixelsToFix16_6F6850.list[pCarInfo->h];
    SwapIf3or4_41FE40(arrow_direction, car_w, car_h);

    DAT_006FF8C4 = gRng_6F6784.get_uint8_4F7B70(100);
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
            ypos = field_0_cam->field_78_view_bounds.field_8_top - kFpFive_6FF6D4;
            if (!field_8_spawn_side)
            {
                xpos = field_0_cam->field_78_view_bounds.field_0_left - kFpOne_6FF778;
                x_step = 1;
            }
            else
            {
                xpos = field_0_cam->field_78_view_bounds.field_4_right + kFpOne_6FF778;
                x_step = -1;
            }
            break;
        case 2:
            ypos = field_0_cam->field_78_view_bounds.field_C_bottom + kFpFive_6FF6D4;
            if (!field_8_spawn_side)
            {
                xpos = field_0_cam->field_78_view_bounds.field_0_left - kFpOne_6FF778;
                x_step = 1;
            }
            else
            {
                xpos = field_0_cam->field_78_view_bounds.field_4_right + kFpOne_6FF778;
                x_step = -1;
            }
            break;
        case 4:
            xpos = field_0_cam->field_78_view_bounds.field_0_left - kFpFive_6FF6D4;
            if (!field_8_spawn_side)
            {
                ypos = field_0_cam->field_78_view_bounds.field_8_top - kFpOne_6FF778;
                y_step = 1;
            }
            else
            {
                ypos = field_0_cam->field_78_view_bounds.field_C_bottom + kFpOne_6FF778;
                y_step = -1;
            }
            break;
        case 3:
            xpos = field_0_cam->field_78_view_bounds.field_4_right + kFpFive_6FF6D4;
            if (!field_8_spawn_side)
            {
                ypos = field_0_cam->field_78_view_bounds.field_8_top - kFpOne_6FF778;
                y_step = 1;
            }
            else
            {
                ypos = field_0_cam->field_78_view_bounds.field_C_bottom + kFpOne_6FF778;
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
        gmp_block_info* pBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &found_z);
        ground_z = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(xpos, ypos);
        if (pBlock)
        {
            u8 slope_type = pBlock->field_B_slope_type;
            if (!(IS_GRADIENT_SLOPE(slope_type) && GET_BLOCK_TYPE(slope_type) != 0) && GET_BLOCK_TYPE(slope_type) == 1)
            {
                Fix16 nav_x = xpos.GetFloorValue() + kFpHalf_6FF674;
                Fix16 nav_y = ypos.GetFloorValue() + kFpHalf_6FF674;
                gTrafficCarSpeed_6FF570 = kFpPoint2_6FF5CC;
                field_4_rect->SetRect_41E350(nav_x - car_w, nav_x + car_w, nav_y - car_h, nav_y + car_h);
                field_4_rect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                if (field_4_rect->EveryTileMatchesArrowType_59DFB0(arrow_direction))
                {
                    gmp_zone_info* pZone = gMap_0x370_6F6268->GetZoneInfoAt_4DF890(nav_x.ToInt(), nav_y.ToInt());
                    if (pZone && !pZone->field_0_car_density)
                    {
                        return 0;
                    }

                    s32 car_model;
                    u16 car_kind;
                    u8 rng_val;
                    if (gPolice_7B8_6FEE40->field_654_max_wanted_level < cop_level_enum::police_1 || (rng_val = static_cast<u8>(gRng_6F6784.get_int_4F7AE0(40)), rng_val <= 20u) ||
                        rng_val >= 30u)
                    {
                        car_model = gCarManager_6C_677930->SelectTrafficCarModel_444AB0(field_C_player, field_10_zone, gTrafficDensityScale_6FF7E8, &car_kind);
                        if (car_kind == 0)
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        car_model = car_model_enum::COPCAR;
                        car_kind = 4;
                    }

                    if (gPolice_7B8_6FEE40->TryBeginRoadblock_577320())
                    {
                        bRoadblock = 1;
                        car_model = car_model_enum::bank_van;
                    }
                    else
                    {
                        if (car_model == car_model_enum::COPCAR)
                        {
                            if (gPolice_7B8_6FEE40->field_658_police_car_count >= static_cast<u8>(gPolice_7B8_6FEE40->field_659_max_police_cars) || bSkip_police_67D4F9)
                            {
                                car_model =
                                    gCarManager_6C_677930->SelectTrafficCarModel_444AB0(field_C_player, field_10_zone, gTrafficDensityScale_6FF7E8, &car_kind);
                                if (car_model == car_model_enum::COPCAR || car_kind == 0)
                                {
                                    return 0;
                                }
                            }
                            else
                            {
                                switch (gPolice_7B8_6FEE40->field_65C_highest_crew_type_in_pursuit)
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
                        else if (gPolice_7B8_6FEE40->field_65C_highest_crew_type_in_pursuit == crew_type::army_6)
                        {
                            switch (gRng_6F6784.get_int_4F7AE0(10))
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

                        if (car_kind == 5)
                        {
                            pGang = field_C_player->get_gang_curr_location_4766D0();
                            if (!pGang || !pGang->HasKillRespectChange_45DD50())
                            {
                                return 0;
                            }
                            car_model = pGang->field_13C_gang_car_model;
                        }
                    }

                    Fix16 max_speed = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(car_model)->field_28_max_speed;
                    Fix16 speed;
                    speed = GetRandomTrafficSpeed_583750(max_speed, &speed_kind);
                    gTrafficCarSpeed_6FF570 = speed;

                    // Number of matching road tiles in the 5 tile column and the indices of the first and last one
                    u8 road_len = 1;
                    u8 first_idx = 2;
                    u8 last_idx = 2;

                    if (IsFlatRoadAt_583870(xpos - x_one, ypos - y_one))
                    {
                        Fix16 tx = (xpos - x_one).GetFloorValue() + kFpHalf_6FF674;
                        Fix16 ty = (ypos - y_one).GetFloorValue() + kFpHalf_6FF674;
                        field_4_rect->SetRect_41E350(tx - car_w, tx + car_w, ty - car_h, ty + car_h);
                        field_4_rect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                        if (field_4_rect->EveryTileMatchesArrowType_59DFB0(arrow_direction))
                        {
                            road_len = 2;
                            first_idx = 1;
                        }
                    }

                    if (IsFlatRoadAt_583870(xpos - x_two, ypos - y_two))
                    {
                        Fix16 tx = (xpos - x_two).GetFloorValue() + kFpHalf_6FF674;
                        Fix16 ty = (ypos - y_two).GetFloorValue() + kFpHalf_6FF674;
                        field_4_rect->SetRect_41E350(tx - car_w, tx + car_w, ty - car_h, ty + car_h);
                        field_4_rect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                        if (field_4_rect->EveryTileMatchesArrowType_59DFB0(arrow_direction))
                        {
                            road_len++;
                            first_idx = 0;
                        }
                    }

                    if (IsFlatRoadAt_583870(xpos + x_one, ypos + y_one))
                    {
                        Fix16 tx = (xpos + x_one).GetFloorValue() + kFpHalf_6FF674;
                        Fix16 ty = (ypos + y_one).GetFloorValue() + kFpHalf_6FF674;
                        field_4_rect->SetRect_41E350(tx - car_w, tx + car_w, ty - car_h, ty + car_h);
                        field_4_rect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                        if (field_4_rect->EveryTileMatchesArrowType_59DFB0(arrow_direction))
                        {
                            last_idx = 3;
                            road_len++;
                        }
                    }

                    if (IsFlatRoadAt_583870(xpos + x_two, ypos + y_two))
                    {
                        Fix16 tx = (xpos + x_two).GetFloorValue() + kFpHalf_6FF674;
                        Fix16 ty = (ypos + y_two).GetFloorValue() + kFpHalf_6FF674;
                        field_4_rect->SetRect_41E350(tx - car_w, tx + car_w, ty - car_h, ty + car_h);
                        field_4_rect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);
                        if (field_4_rect->EveryTileMatchesArrowType_59DFB0(arrow_direction))
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
                        front_x = xpos + x_one * GetLaneOffset_5838E0(last_idx);
                        front_y = ypos + y_one * GetLaneOffset_5838E0(last_idx);
                        mid_x = front_x;
                        mid_y = front_y;
                        back_x = front_x;
                        back_y = front_y;
                        gTrafficCarSpeed_6FF570 = GetRandomTrafficSpeed_583750(kFpPoint03_6FF6A4, &speed_kind);
                    }
                    else if (road_len == 2)
                    {
                        front_x = xpos + x_one * GetLaneOffset_5838E0(last_idx);
                        front_y = ypos + y_one * GetLaneOffset_5838E0(last_idx);
                        back_x = xpos + x_one * GetLaneOffset_5838E0(first_idx);
                        back_y = ypos + y_one * GetLaneOffset_5838E0(first_idx);
                        mid_x = back_x;
                        mid_y = back_y;
                    }
                    else if (road_len == 3)
                    {
                        front_x = xpos + x_one * GetLaneOffset_5838E0(last_idx);
                        front_y = ypos + y_one * GetLaneOffset_5838E0(last_idx);
                        back_x = xpos + x_one * GetLaneOffset_5838E0(first_idx);
                        back_y = ypos + y_one * GetLaneOffset_5838E0(first_idx);
                        mid_x = (back_x + front_x) / kFpTwo_6FF77C;
                        mid_y = (back_y + front_y) / kFpTwo_6FF77C;
                    }

                    Fix16 spawn_x;
                    Fix16 spawn_y;
                    if (speed_kind == 2)
                    {
                        if (static_cast<u8>(DAT_006FF8C4) > 2u)
                        {
                            spawn_x = back_x;
                            spawn_y = back_y;
                            ResolveSpawnDirection_583930(2, road_len);
                        }
                        else if (static_cast<u8>(DAT_006FF8C4) > 0u)
                        {
                            spawn_x = mid_x;
                            spawn_y = mid_y;
                            ResolveSpawnDirection_583930(1, road_len);
                        }
                        else
                        {
                            spawn_x = front_x;
                            spawn_y = front_y;
                            ResolveSpawnDirection_583930(0, road_len);
                        }
                    }
                    else if (speed_kind == 1)
                    {
                        if (static_cast<u8>(DAT_006FF8C4) > 1u)
                        {
                            spawn_x = mid_x;
                            spawn_y = mid_y;
                            ResolveSpawnDirection_583930(1, road_len);
                        }
                        else
                        {
                            spawn_x = front_x;
                            spawn_y = front_y;
                            ResolveSpawnDirection_583930(0, road_len);
                        }
                    }
                    else
                    {
                        spawn_x = front_x;
                        spawn_y = front_y;
                        ResolveSpawnDirection_583930(0, road_len);
                    }

                    Fix16 sx = spawn_x.GetFloorValue() + kFpHalf_6FF674;
                    Fix16 sy = spawn_y.GetFloorValue() + kFpHalf_6FF674;
                    field_4_rect->SetRect_41E350(sx - car_w, sx + car_w, sy - car_h, sy + car_h);
                    field_4_rect->SetHiLowZ_41E370(ground_z - kFpOne_6FF778, ground_z + kFpOne_6FF778);

                    u8 spawn_chance;
                    if (bStartNetworkGame_7081F0)
                    {
                        spawn_chance = 0;
                    }
                    else
                    {
                        spawn_chance = gRng_6F6784.get_uint8_4F7B70(100);
                    }

                    if ((speed_kind == 2 && spawn_chance < 33u) || (speed_kind == 1 && spawn_chance < 67u) || speed_kind == 0)
                    {
                        if (!gSpriteGrid_1_679208->CheckRectForCollisions_477F60(field_4_rect, 0, sprite_types_enum::none_0, 0) &&
                            !field_4_rect->IsRectBlockedByMap_59DE80() && !field_4_rect->RectOverlapsZone_59DF20(2))
                        {
                            Fix16 car_ypos = sy.GetFloorValue() + kFpHalf_6FF674;
                            Fix16 car_x = sx.GetFloorValue() + kFpHalf_6FF674;
                            Fix16 car_y = car_ypos;
                            if (!gGame_0x40_67E008->IsRectVisibleToAnyPlayer_4B9B10(field_4_rect))
                            {
                                if (bRoadblock)
                                {
                                    gPolice_7B8_6FEE40->TryCreateRoadblockAt_577370(car_x.ToInt(), car_y.ToInt(), arrow_direction);
                                }
                                else
                                {
                                    Car* pNewCar = gCarManager_6C_677930->SpawnCarAtCorrectZ_WithScale(car_x,
                                                                                                car_y,
                                                                                                ReturnAngleFromRoadDirection_4F7940(arrow_direction),
                                                                                                car_model,
                                                                                                kFpOne_6FF778);
                                    if (car_model != car_model_enum::SWATVAN && car_model != car_model_enum::COPCAR &&
                                        car_model != car_model_enum::JEEP && car_model != car_model_enum::EDSELFBI)
                                    {
                                        pNewCar->IncrementCarStats_443D70(car_kind::recycled_1);
                                        if (car_kind == 5)
                                        {
                                            u8 gang_car_remap = pGang->field_140_gang_car_remap;
                                            if (gang_car_remap == 0xFF)
                                            {
                                                pNewCar->field_50_car_sprite->SetPaletteSprites_420700();
                                            }
                                            else
                                            {
                                                pNewCar->field_50_car_sprite->SetRemap(gang_car_remap);
                                            }
                                            pNewCar->AddGangDriver_440630(pGang);
                                            pNewCar->AttachGangIcon_440660(pGang->field_138_arrow_colour);
                                        }
                                        else
                                        {
                                            pNewCar->AssignRandomRemap_43A7D0();
                                            pNewCar->SpawnDriverPed();
                                        }
                                        pNewCar->InitCarAIControl_440590();
                                        pNewCar->StartEngine_426E00();
                                        pNewCar->field_5C_AI->field_74_max_speed = gTrafficCarSpeed_6FF570;
                                    }
                                    else if (gCarManager_6C_677930->CanAllocateCarOfKind_446930(car_kind::police_6) &&
                                             gPolice_7B8_6FEE40->SpawnCrewInCar_5703E0(pNewCar))
                                    {
                                        pNewCar->IncrementCarStats_443D70(car_kind::police_6);
                                    }
                                    else
                                    {
                                        pNewCar->IncrementCarStats_443D70(car_kind::recycled_1);
                                        pNewCar->DespawnSoon_421490();
                                    }

                                    gGame_0x40_67E008->ReduceTrafficSpawnCounterOfOtherViewers_4B9D60(pNewCar->field_50_car_sprite,
                                                                                                      field_C_player);

                                    Player* pPlayer = field_C_player;
                                    if (pPlayer->field_680_traffic_spawn_counter > pPlayer->field_682_traffic_spawn_threshold)
                                    {
                                        pPlayer->field_680_traffic_spawn_counter -= pPlayer->field_682_traffic_spawn_threshold;
                                        return 0;
                                    }
                                    pPlayer->field_680_traffic_spawn_counter = 0;
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        xpos += Fix16(x_step);
        ypos += Fix16(y_step);
        if (!IsInsideCameraEdge_582360(spawn_type, xpos, ypos))
        {
            return 0;
        }
    }
    return 0;
}

MATCH_FUNC(0x583870)
bool TrafficSpawner::IsFlatRoadAt_583870(Fix16 xpos, Fix16 ypos)
{
    if (xpos > kFpZero_6FF774 && ypos > kFpZero_6FF774 && xpos < kFp255_6FF558 && ypos < kFp255_6FF558)
    {
        s32 zpos;
        gmp_block_info* pHighBlock = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &zpos);
        if (!pHighBlock)
        {
            return 0;
        }

        u8 slope_type = pHighBlock->field_B_slope_type;
        if (IS_GRADIENT_SLOPE(slope_type) && GET_BLOCK_TYPE(slope_type) != 0)
        {
            return 0;
        }

        if (GET_BLOCK_TYPE(slope_type) == 1)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x5838E0)
s32 TrafficSpawner::GetLaneOffset_5838E0(u8 lane_idx)
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
char_type TrafficSpawner::ResolveSpawnDirection_583930(char_type spawn_pos_idx, char_type road_len)
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

// Defined after HandleCarHitByObject_43F130, so that VC6 hasn't compiled their out-of-line copies (and
// learnt that they can't throw) when it compiles that function.
inline Fix16_Point Sprite::get_x_y_late_443580()
{
    return Fix16_Point(field_14_xy.x, field_14_xy.y);
}

inline Fix16_Point Fix16_Point::AddLate_40AC50(const Fix16_Point& in)
{
    return Fix16_Point(x + in.x, y + in.y);
}

inline Fix16_Point Fix16_Point::SubLate_40AC80(const Fix16_Point& rhs)
{
    return Fix16_Point(x - rhs.x, y - rhs.y);
}
