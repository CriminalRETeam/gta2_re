#include "ImGuiDebug.hpp"
#include "car_despawn_status.hpp"
#include "PlayerCrimeStats.hpp"
#include "3rdParty/GTA2Hax/3rdParty/imgui/imgui.h"
#include "AmbulanceService.hpp"
#include "InputRecorder_67F8B0.hpp"
#include "CarInfoTable.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Firefighters.hpp"
#include "Frontend.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "CarChaseTaskTable.hpp"
#include "Hud.hpp"
#include "EmergencyCrewPool.hpp"
#include "MapRenderer.hpp"
#include "NetUi.hpp"
#include "Object_5C.hpp"
#include "ObjectPool.hpp"
#include "PathFinder.hpp"
#include "ParticleManager.hpp"
#include "PedGroup.hpp"
#include "ObjectDefinitions.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "CrimeReportQueue.hpp"
#include "WeaponManager.hpp"
#include "collide.hpp"
#include "debug.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "PlyDat.hpp"
#include "GameSession.hpp"
#include "map_0x370.hpp"
#include "registry.hpp"
#include "sprite.hpp"
#include "TextTable.hpp"
#include "sound_obj.hpp"
#include "root_sound.hpp"
#include "CollisionSoundQueue.hpp"
#include "ExplodingScore_100.hpp"
#include "CarAI.hpp"
#include <direct.h>
#include <vector>
#include <stdarg.h>

#include "HookLoader.hpp"
#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

EXTERN_GLOBAL(AmbulanceService*, gAmbulance_110);
EXTERN_GLOBAL(CollisionCounters*, gCollisionCounters);
EXTERN_GLOBAL(FirefighterPool*, gFirefighterPool_54);
EXTERN_GLOBAL(PathFinder*, gPathFinder);

EXTERN_GLOBAL(car_rng_list, gAverageCarModels);
EXTERN_GLOBAL(car_rng_list, gBadCarModels);
EXTERN_GLOBAL(car_rng_list, gGoodCarModels);

EXTERN_GLOBAL(u8, gPoliceCrew_Unused);
EXTERN_GLOBAL(Fix16_Point, kZeroOnePoint);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff, 640);

EXTERN_GLOBAL(Fix16_Point, CollisionIntersectionPoint);

Object* spawned_obj = NULL;
Car* pNewCar = NULL;
char TmpBitSetChar[50];

Hud_Arrow* gpArrow = NULL;

void wchar_to_char(const wchar_t* wchar, char* out, u8 size)
{
    u16 i = 0;
    for (; wchar[i]; i++)
    {
        out[static_cast<s8>(i)] = static_cast<char>(wchar[i]);
    }
    // clear remaining slots
    for (s8 j = static_cast<s8>(i); j < size; j++)
    {
        out[j] = 0;
    }
}

void get_car_name(Car* pCar, char* out)
{
    wchar_t* car_wname = pCar->GetCarStr();
    wchar_to_char(car_wname, out, 33);
}

namespace ImGui
{
bool SliderS16(const char* label, s16* v, s16 v_min, s16 v_max)
{
    s32 value = static_cast<s32>(*v);
    bool result = ImGui::SliderInt(label, &value, static_cast<s32>(v_min), static_cast<s32>(v_max));
    if (result)
    {
        *v = static_cast<s16>(value);
    }
    return result;
}

bool SliderU16(const char* label, u16* v, s16 v_min, s16 v_max)
{
    s32 value = static_cast<s32>(*v);
    bool result = ImGui::SliderInt(label, &value, static_cast<s32>(v_min), static_cast<s32>(v_max));
    if (result)
    {
        *v = static_cast<u16>(value);
    }
    return result;
}

bool SliderS8(const char* label, char_type* v, char_type v_min, char_type v_max)
{
    s32 value = static_cast<s32>(*v);
    bool result = ImGui::SliderInt(label, &value, static_cast<s32>(v_min), static_cast<s32>(v_max));
    if (result)
    {
        *v = static_cast<char_type>(value);
    }
    return result;
}

bool SliderU8(const char* label, u8* v, u8 v_min, u8 v_max)
{
    s32 value = static_cast<s32>(*v);
    bool result = ImGui::SliderInt(label, &value, static_cast<s32>(v_min), static_cast<s32>(v_max));
    if (result)
    {
        *v = static_cast<u8>(value);
    }
    return result;
}

bool Input_char_type(const char* label, char_type* v, s32 step, s32 step_fast, ImGuiInputTextFlags extra_flags = 0)
{
    s32 tmp = *v;
    bool ret = ImGui::InputInt(label, &tmp, step, step_fast, extra_flags);
    *v = static_cast<char_type>(tmp);
    return ret;
}

bool InputU8(const char* label, u8* v, s32 step, s32 step_fast, ImGuiInputTextFlags extra_flags = 0)
{
    s32 tmp = *v;
    bool ret = ImGui::InputInt(label, &tmp, step, step_fast, extra_flags);
    *v = static_cast<u8>(tmp);
    return ret;
}

bool InputS16(const char* label, s16* v, s32 step, s32 step_fast, ImGuiInputTextFlags extra_flags = 0)
{
    s32 tmp = *v;
    bool ret = ImGui::InputInt(label, &tmp, step, step_fast, extra_flags);
    *v = static_cast<s16>(tmp);
    return ret;
}

bool InputU16(const char* label, u16* v, s32 step, s32 step_fast, ImGuiInputTextFlags extra_flags = 0)
{
    s32 tmp = *v;
    bool ret = ImGui::InputInt(label, &tmp, step, step_fast, extra_flags);
    *v = static_cast<u16>(tmp);
    return ret;
}

} // namespace ImGui

EXTERN_GLOBAL(CrimeReportQueue*, gCrimeReportQueue);

void EnableBoot2MapDebugOptions()
{
    bSkip_traffic_lights = true;
    bSkip_trains = true;
}

static Sprite* GetPedSprite(Ped* pPed)
{
    if (!pPed->mGameObject)
    {
        return NULL;
    }

    return pPed->mGameObject->mSpritePtr;
}

static Ped* GetPlayerPed()
{
    if (!gGame_0x40)
    {
        return NULL;
    }

    Player* pPlayer = gGame_0x40->mPlayers[0];
    if (!pPlayer)
    {
        return NULL;
    }

    if (!pPlayer->mPlayerPed)
    {
        return NULL;
    }

    return pPlayer->mPlayerPed;
}

static Sprite* GetPlayerSprite()
{
    Ped* pPed = GetPlayerPed();
    if (!pPed)
    {
        return NULL;
    }

    return GetPedSprite(pPed);
}

static Sprite* GetPlayerAssociatedSprite()
{
    Ped* pPed = GetPlayerPed();
    if (!pPed)
    {
        return NULL;
    }

    if (!pPed->mGameObject)
    {
        if (pPed->mCar)
        {
            return pPed->mCar->GetSprite();
        }
        return NULL;
    }
    return pPed->mGameObject->mSpritePtr;
}

static void GetPlayerPos(Fix16& xpos, Fix16& ypos, Fix16& zpos)
{
    Ped* pPlayerPed = GetPlayerPed();
    Sprite* pPlayerSprite = GetPedSprite(pPlayerPed);

    if (pPlayerSprite)
    {
        xpos = pPlayerSprite->mXy.x;
        ypos = pPlayerSprite->mXy.y;
        zpos = pPlayerSprite->mZpos;
    }
    else
    {
        xpos = pPlayerPed->mCam.x;
        ypos = pPlayerPed->mCam.y;
        zpos = pPlayerPed->mCam.z;
    }
}

static void ShowRectAndPointsForSprite4C(SpriteShape* pShape)
{
    Fix16_Rect rect = pShape->mBoundingBox;
    if (ImGui::TreeNode("bounding box"))
    {
        ImGui::Value("Left", rect.mLeft.ToFloat(), "%.2f");
        ImGui::Value("Right", rect.mRight.ToFloat(), "%.2f");
        ImGui::Value("Top", rect.mTop.ToFloat(), "%.2f");
        ImGui::Value("Bottom", rect.mBottom.ToFloat(), "%.2f");
        ImGui::Value("Lower Z", rect.mLowZ.ToFloat(), "%.2f");
        ImGui::Value("Higher Z", rect.mHighZ.ToFloat(), "%.2f");
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("rendering rect"))
    {
        ImGui::Value("0 x:", pShape->mCorners[0].x.ToFloat(), "%.2f");
        ImGui::Value("0 y:", pShape->mCorners[0].y.ToFloat(), "%.2f");
        ImGui::Value("1 x:", pShape->mCorners[1].x.ToFloat(), "%.2f");
        ImGui::Value("1 y:", pShape->mCorners[1].y.ToFloat(), "%.2f");
        ImGui::Value("2 x:", pShape->mCorners[2].x.ToFloat(), "%.2f");
        ImGui::Value("2 y:", pShape->mCorners[2].y.ToFloat(), "%.2f");
        ImGui::Value("3 x:", pShape->mCorners[3].x.ToFloat(), "%.2f");
        ImGui::Value("3 y:", pShape->mCorners[3].y.ToFloat(), "%.2f");
        ImGui::TreePop();
    }
}

static void ShowPedBitMask(Ped* pPed)
{
    if (pPed)
    {
        if (ImGui::TreeNode("Ped BitMask Values"))
        {
            ImGui::Value("bActive", pPed->mBf21C.bActive);
            ImGui::Value("bUnused1", pPed->mBf21C.bUnused1);
            ImGui::Value("bPanicking", pPed->mBf21C.bPanicking);
            ImGui::Value("bDriveAggressively", pPed->mBf21C.bDriveAggressively);
            ImGui::Value("bStayInCar", pPed->mBf21C.bStayInCar);
            ImGui::Value("bBusted", pPed->mBf21C.bBusted);
            ImGui::Value("bSkipCarSearch", pPed->mBf21C.bSkipCarSearch);
            ImGui::Value("bUseCarWeapon", pPed->mBf21C.bUseCarWeapon);
            ImGui::Value("bHitByAttacker", pPed->mBf21C.bHitByAttacker);
            ImGui::Value("bNoWeapon", pPed->mBf21C.bNoWeapon);
            ImGui::Value("bScheduledForRemoval", pPed->mBf21C.bScheduledForRemoval);
            ImGui::Value("bAttacking", pPed->mBf21C.bAttacking);
            ImGui::Value("bUnused12", pPed->mBf21C.bUnused12);
            ImGui::Value("bWeaponChosen", pPed->mBf21C.bWeaponChosen);
            ImGui::Value("bInPathList", pPed->mBf21C.bInPathList);
            ImGui::Value("bFlag15", pPed->mBf21C.bFindingPath);
            ImGui::Value("bFlag16", pPed->mBf21C.bPathSteppedThisRound);
            ImGui::Value("bFlag17", pPed->mBf21C.bNoPathFound);
            ImGui::Value("bUnused18", pPed->mBf21C.bUnused18);
            ImGui::Value("bFlag19", pPed->mBf21C.bLosCheckedThisRound);
            ImGui::Value("bUnused20", pPed->mBf21C.bUnused20);
            ImGui::Value("bUnused21", pPed->mBf21C.bUnused21);
            ImGui::Value("bFlag22", pPed->mBf21C.bHoldPosition);
            ImGui::Value("bSpottedPlayer", pPed->mBf21C.bSpottedPlayer);
            ImGui::Value("bOnFire", pPed->mBf21C.bOnFire);
            ImGui::Value("bInvisible", pPed->mBf21C.bInvisible);
            ImGui::Value("bElectroFingers", pPed->mBf21C.bElectroFingers);
            ImGui::Value("bLeftVehicle", pPed->mBf21C.bLeftVehicle);
            ImGui::Value("bArmedGangMember", pPed->mBf21C.bArmedGangMember);
            ImGui::Value("bForcedOutOfTaxi", pPed->mBf21C.bForcedOutOfTaxi);
            ImGui::Value("bUnused30", pPed->mBf21C.bUnused30);
            ImGui::Value("bUnused31", pPed->mBf21C.bUnused31);
            ImGui::TreePop();
        }
    }
}

// Bit order of PedFlags32
static const char* const kPedFlagNames[32] = {
    "bActive",
    "bUnused1",
    "bPanicking",
    "bDriveAggressively",
    "bStayInCar",
    "bBusted",
    "bSkipCarSearch",
    "bUseCarWeapon",
    "bHitByAttacker",
    "bNoWeapon",
    "bScheduledForRemoval",
    "bAttacking",
    "bUnused12",
    "bWeaponChosen",
    "bInPathList",
    "bFlag15",
    "bFlag16",
    "bFlag17",
    "bUnused18",
    "bFlag19",
    "bUnused20",
    "bUnused21",
    "bFlag22",
    "bSpottedPlayer",
    "bOnFire",
    "bInvisible",
    "bElectroFingers",
    "bLeftVehicle",
    "bArmedGangMember",
    "bForcedOutOfTaxi",
    "bUnused30",
    "bUnused31",
};

static void ShowPedBitMaskSetting(PedFlags32* flags)
{
    if (flags && ImGui::TreeNode("Ped BitMask Setting"))
    {
        // PedFlags32 is one dword of single bit fields, so the flags can be edited as a plain mask
        u32* pMask = reinterpret_cast<u32*>(flags);
        for (u32 bit = 0; bit < 32; bit++)
        {
            ImGui::CheckboxFlags(kPedFlagNames[bit], pMask, 1u << bit);
        }
        ImGui::TreePop();
    }
}

static const char* GetOccupationStrFromPed(Ped* pPed)
{
    if (pPed)
    {
        switch (pPed->mOccupation)
        {
            case ped_ocupation_enum::car_thief:
                return "Car Thief";
            case ped_ocupation_enum::mugger:
                return "Mugger";
            case ped_ocupation_enum::driver:
                return "Driver";
            case ped_ocupation_enum::fireman:
                return "Fireman";
            case ped_ocupation_enum::elvis_leader:
                return "Elvis Leader";
            case ped_ocupation_enum::elvis:
                return "Elvis";
            case ped_ocupation_enum::dummy:
                return "Dummy";
            case ped_ocupation_enum::robbed_driver_10:
                return "Driver 2";
            case ped_ocupation_enum::scared_driver_50:
                return "Driv3r";
            case ped_ocupation_enum::no_occupation:
                return "None";
            case ped_ocupation_enum::any_gang_member:
                return "Gang member";
            case ped_ocupation_enum::police:
                return "Police";
            case ped_ocupation_enum::fbi:
                return "FBI";
            case ped_ocupation_enum::player:
                return "Player";
            case ped_ocupation_enum::swat:
                return "SWAT";
            case ped_ocupation_enum::road_block_tank_man:
                return "Roadblock Tank Man";
            case ped_ocupation_enum::army_army:
                return "Army";
            case ped_ocupation_enum::tank_driver:
                return "Tank Driver";
            default:
                return "Other";
        }
    }
    else
    {
        return "No Ped";
    }
    
}

// Test scene globals
DEFINE_GLOBAL(Car*, gTestCar1, 0x704418);
DEFINE_GLOBAL(Trailer*, gTestTrailer, 0x7041A8);
DEFINE_GLOBAL(Car*, gTestCar2, 0x704410);
DEFINE_GLOBAL(Car*, gTestCar3, 0x7043D4);
DEFINE_GLOBAL(Object*, gTestObject, 0x704274);
DEFINE_GLOBAL_INIT(Ang16, kAng90_70420C, Ang16(360), 0x70420C);
DEFINE_GLOBAL(Ang16, kAng0_704500, 0x704500);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_7042A4, Fix16(0.5), 0x7042A4);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_7043AC, Fix16(1), 0x7043AC);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_704420, Fix16(0.25), 0x704420);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_7041F0, Fix16(0.5), 0x7041F0);
EXTERN_GLOBAL(Car*, gTestArrowCar);

// Builds a test scene: a few blocks, cars with weapons and gang icons, and a row of each object
MATCH_FUNC(0x5B1170)
EXPORT void __stdcall NoRefs_sub()
{
    gmp_block_info block;
    gGtx_0x106C->get_sprite_index(
        gGtx_0x106C->GetSpriteTrueIndex(sprite_types_enum::font_7,
                                                      gGtx_0x106C->GetSpriteIdxFromFont(gKanjiSmallFont, 0)));
    block.init();
    block.mLeft = 7206;
    block.mRight = 7206;
    block.mTop = 7206;
    block.mBottom = 7206;
    block.mLid = 0;
    block.mSlopeType = 215;
    gMap_0x370->AddNewBlock(112, 147, 2, &block);
    block.mSlopeType = 219;
    gMap_0x370->AddNewBlock(110, 147, 2, &block);
    block.mSlopeType = 223;
    gMap_0x370->AddNewBlock(112, 149, 2, &block);
    block.mSlopeType = 227;
    gMap_0x370->AddNewBlock(110, 149, 2, &block);
    block.mSlopeType = 231;
    gMap_0x370->AddNewBlock(112, 143, 2, &block);
    block.mSlopeType = 235;
    gMap_0x370->AddNewBlock(110, 143, 2, &block);
    block.mSlopeType = 239;
    gMap_0x370->AddNewBlock(112, 145, 2, &block);
    block.mSlopeType = 243;
    gMap_0x370->AddNewBlock(110, 145, 2, &block);
    block.mSlopeType = 247;
    gMap_0x370->AddNewBlock(111, 148, 2, &block);
    block.mSlopeType = 171;
    gMap_0x370->AddNewBlock(111, 143, 2, &block);
    block.mSlopeType = 3;
    gMap_0x370->AddNewBlock(111, 141, 2, &block);
    block.mSlopeType = 199;
    gMap_0x370->AddNewBlock(112, 152, 2, &block);
    gTestCar1 =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(98), Fix16(180), kAng90_70420C, car_model_enum::GT24640, kFpHalf_7042A4);
    gTestCar1->LockPermanently();
    gTestCar1->IncrementCarStats(car_kind::mission_car_8);
    gTestCar1->SetMiniCar();
    gTestCar1->UpdateDrawLayer();
    gWeapon_8->add_car_weapon(23, 1, gTestCar1);
    gTestCar1->IsBeingCrushed();
    gTestTrailer =
        gCarManager_6C->SpawnCabAndTrailer(98, 179, kAng90_70420C, car_model_enum::TRUKCAB2, car_model_enum::TRUKTRNS);
    gTestTrailer->mTruckCab->IncrementCarStats(car_kind::mission_car_8);
    gWeapon_8->add_car_weapon(15, 1, gTestTrailer->mTruckCab);
    gTestCar2 = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(92) + kFpQuarter_704420,
                                                                        Fix16(175),
                                                                        kAng90_70420C,
                                                                        car_model_enum::MONSTER,
                                                                        kFpOne_7043AC);
    gTestCar2->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(97), Fix16(178), kAng90_70420C, car_model_enum::EDSELFBI, kFpOne_7043AC);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gWeapon_8->add_car_weapon(18, 99, gTestCar3);
    gWeapon_8->add_car_weapon(15, 1, gTestCar3);
    gWeapon_8->add_car_weapon(16, 99, gTestCar3);
    gWeapon_8->add_car_weapon(17, 99, gTestCar3);
    gTestArrowCar =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(95), Fix16(178), kAng90_70420C, car_model_enum::TANK, kFpOne_7043AC);
    gTestArrowCar->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(95), Fix16(180), kAng90_70420C, car_model_enum::MIURA, kFpOne_7043AC);
    gWeapon_8->add_car_weapon(17, 99, gTestCar3);
    gTestCar3->mCarSprite->SetRemap(14);
    gTestCar3->AttachGangIcon(2);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(95), Fix16(179), kAng90_70420C, car_model_enum::STINGRAY, kFpOne_7043AC);
    gWeapon_8->add_car_weapon(16, 99, gTestCar3);
    gTestCar3->mCarSprite->SetRemap(2);
    gTestCar3->AttachGangIcon(3);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(93), Fix16(179), kAng90_70420C, car_model_enum::ISETTA, kFpOne_7043AC);
    gTestCar3->mCarSprite->SetRemap(5);
    gTestCar3->AttachGangIcon(1);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(93), Fix16(178), kAng90_70420C, car_model_enum::KRSNABUS, kFpOne_7043AC);
    gTestCar3->AttachGangIcon(6);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(98) + kFpQuarter_704420,
                                                                        Fix16(178),
                                                                        kAng90_70420C,
                                                                        car_model_enum::STRATOSB,
                                                                        kFpOne_7043AC);
    gTestCar3->mCarSprite->SetRemap(10);
    gTestCar3->AttachGangIcon(5);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(100), Fix16(180), kAng90_70420C, car_model_enum::ZCX5, kFpOne_7043AC);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 =
        gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(101), Fix16(178), kAng90_70420C, car_model_enum::BUICK, kFpOne_7043AC);
    gTestCar3->mCarSprite->SetRemap(28);
    gTestCar3->AttachGangIcon(7);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(102) + kFpHalf_7041F0,
                                                                        Fix16(178),
                                                                        kAng90_70420C,
                                                                        car_model_enum::boxtruck,
                                                                        kFpOne_7043AC);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(104) + kFpHalf_7041F0,
                                                                        Fix16(178),
                                                                        kAng90_70420C,
                                                                        car_model_enum::TRUKCAB1,
                                                                        kFpOne_7043AC);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(106) + kFpHalf_7041F0,
                                                                        Fix16(178),
                                                                        kAng90_70420C,
                                                                        car_model_enum::TRUKCAB2,
                                                                        kFpOne_7043AC);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gTestCar3 = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(108) + kFpHalf_7041F0,
                                                                        Fix16(178),
                                                                        kAng90_70420C,
                                                                        car_model_enum::bank_van,
                                                                        kFpOne_7043AC);
    gTestCar3->IncrementCarStats(car_kind::mission_car_8);
    gObject_5C->NewPhysicsObj(objects::bollard_2, 97, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::power_generator_281, 98, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::secret_token_266, 99, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::secret_token_266, 100, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(228, 101, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(229, 102, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::health_powerup_230, 103, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::armor_powerup_231, 104, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::get_out_jail_powerup_232, 105, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(233, 106, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::invulnerability_powerup_234, 107, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(235, 108, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(236, 109, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(237, 110, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(238, 111, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::invisibility_powerup_239, 112, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(240, 113, 181, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(228, 101, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(229, 102, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::health_powerup_230, 103, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::armor_powerup_231, 104, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::get_out_jail_powerup_232, 105, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(233, 106, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::invulnerability_powerup_234, 107, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(235, 108, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(236, 109, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(237, 110, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(238, 111, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::invisibility_powerup_239, 112, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(240, 113, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(247, 107, 180, 2, kAng0_704500);
    gTestObject = gObject_5C->NewPhysicsObj(objects::car_shop_130, 107, 180, 2, kAng0_704500);
    gTestObject->SetParam(1);
    gObject_5C->NewPhysicsObj(247, 105, 180, 2, kAng0_704500);
    gTestObject = gObject_5C->NewPhysicsObj(objects::car_shop_130, 105, 180, 2, kAng0_704500);
    gTestObject->SetParam(254);
    gObject_5C->NewPhysicsObj(247, 103, 180, 2, kAng0_704500);
    gTestObject = gObject_5C->NewPhysicsObj(objects::car_shop_130, 103, 180, 2, kAng0_704500);
    gTestObject->SetParam(250);
    gObject_5C->NewPhysicsObj(175, 101, 180, 2, kAng0_704500);
    gTestObject = gObject_5C->NewPhysicsObj(objects::car_shop_130, 101, 180, 2, kAng0_704500);
    gTestObject->SetParam(252);
    gObject_5C->NewPhysicsObj(objects::cone_3, 106, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::footy_13, 107, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::invisible_powergen_283, 108, 180, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::pistol_weapon_200, 105, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::machine_gun_weapon_201, 106, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(202, 107, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(203, 108, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::molotov_weapon_204, 109, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(205, 110, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::shotgun_weapon_206, 111, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(207, 112, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::flamethrower_weapon_208, 121, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(209, 122, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(210, 123, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(215, 124, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(216, 125, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(217, 126, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(objects::armor_powerup_231, 127, 179, 2, kAng0_704500);
    gObject_5C->NewPhysicsObj(218, 128, 179, 2, kAng0_704500);
}

static const char* ObjectIdToString(s32 id)
{
    switch (id)
    {
        case objects::object_0: return "object_0";
        case objects::bin_lid_1: return "bin_lid_1";
        case objects::bollard_2: return "bollard_2";
        case objects::cone_3: return "cone_3";
        case objects::boxes_4: return "boxes_4";
        case objects::blaster_5: return "blaster_5";
        case objects::rubbish_6: return "rubbish_6";
        case objects::bin_7: return "bin_7";
        case objects::animating_oil_8: return "animating_oil_8";
        case objects::oil_9: return "oil_9";
        case objects::mine_10: return "mine_10";
        case objects::bush_11: return "bush_11";
        case objects::crate_12: return "crate_12";
        case objects::footy_13: return "footy_13";
        case objects::hardbox_14: return "hardbox_14";
        case objects::newsdis_15: return "newsdis_15";
        case objects::oildrum_16: return "oildrum_16";
        case objects::tyre_17: return "tyre_17";
        case objects::hydrant_lid_18: return "hydrant_lid_18";
        case objects::hydrant_19: return "hydrant_19";
        case objects::hydrant_unlid_20: return "hydrant_unlid_20";
        case objects::roadblock_21: return "roadblock_21";
        case objects::bench_22: return "bench_22";
        case objects::package_23: return "package_23";
        case objects::object_24: return "object_24";
        case objects::tower_25: return "tower_25";
        case objects::object_26: return "object_26";
        case objects::object_27: return "object_27";
        case objects::object_28: return "object_28";
        case objects::object_29: return "object_29";
        case objects::object_30: return "object_30";
        case objects::object_31: return "object_31";
        case objects::object_32: return "object_32";
        case objects::object_33: return "object_33";
        case objects::object_34: return "object_34";
        case objects::object_35: return "object_35";
        case objects::object_36: return "object_36";
        case objects::object_37: return "object_37";
        case objects::object_38: return "object_38";
        case objects::object_39: return "object_39";
        case objects::object_40: return "object_40";
        case objects::explode_medium_41: return "explode_medium_41";
        case objects::object_42: return "object_42";
        case objects::object_43: return "object_43";
        case objects::object_44: return "object_44";
        case objects::object_45: return "object_45";
        case objects::object_46: return "object_46";

        case objects::moving_collect_32_96: return "moving_collect_32_96";
        case objects::moving_collect_33_97: return "moving_collect_33_97";
        case objects::moving_collect_34_98: return "moving_collect_34_98";
        case objects::moving_collect_35_99: return "moving_collect_35_99";
        case objects::moving_collect_36_100: return "moving_collect_36_100";
        case objects::moving_collect_37_101: return "moving_collect_37_101";
        case objects::moving_collect_38_102: return "moving_collect_38_102";
        case objects::moving_collect_39_103: return "moving_collect_39_103";
        case objects::moving_collect_40_104: return "moving_collect_40_104";
        case objects::moving_collect_41_105: return "moving_collect_41_105";
        case objects::moving_collect_42_106: return "moving_collect_42_106";
        case objects::moving_collect_43_107: return "moving_collect_43_107";
        case objects::moving_collect_44_108: return "moving_collect_44_108";
        case objects::object_109: return "object_109";
        case objects::ped_pushed_110: return "ped_pushed_110";
        case objects::object_111: return "object_111";
        case objects::object_112: return "object_112";
        case objects::explosion_113: return "explosion_113";
        case objects::water_cannon_114: return "water_cannon_114";
        case objects::object_115: return "object_115";
        case objects::small_brown_skid_116: return "small_brown_skid_116";
        case objects::small_grey_skid_117: return "small_grey_skid_117";
        case objects::small_red_skid_118: return "small_red_skid_118";
        case objects::medium_brown_skid_119: return "medium_brown_skid_119";
        case objects::medium_grey_skid_120: return "medium_grey_skid_120";
        case objects::medium_red_skid_121: return "medium_red_skid_121";
        case objects::pedestrian_crossing_marker_122: return "pedestrian_crossing_marker_122";
        case objects::moving_bush_123: return "moving_bush_123";
        case objects::big_white_skid_124: return "big_white_skid_124";
        case objects::medium_white_skid_125: return "medium_white_skid_125";
        case objects::small_white_skid_126: return "small_white_skid_126";
        case objects::explosion_fragments_127: return "explosion_fragments_127";
        case objects::rocket_bullet_128: return "rocket_bullet_128";
        case objects::bus_stop_marker_129: return "bus_stop_marker_129";
        case objects::car_shop_130: return "car_shop_130";
        case objects::car_shop_busy_131: return "car_shop_busy_131";
        case objects::car_bomb_132: return "car_bomb_132";
        case objects::object_133: return "object_133";
        case objects::crane_cables_134: return "crane_unknown_134";
        case objects::crane_rotor_135: return "crane_rotor_135";
        case objects::crane_hook_136: return "crane_hook_136";
        case objects::crane_base_137: return "crane_base_137";
        case objects::moving_molotov_138: return "moving_molotov_138";
        case objects::conveyor_139: return "conveyor_139";
        case objects::crane_counterweight_140: return "crane_counterweight_140";

        case objects::destructor_141: return "destructor_141";
        case objects::crusher_piston_142: return "crusher_piston_142";
        case objects::crusher_central_spot_143: return "crusher_central_spot_143";
        case objects::huge_red_skid_144: return "huge_red_skid_144";
        case objects::huge_white_skid_145: return "huge_white_skid_145";
        case objects::huge_brown_skid_146: return "huge_brown_skid_146";
        case objects::huge_grey_skid_147: return "huge_grey_skid_147";
        case objects::tank_cannon_148: return "tank_cannon_148";
        case objects::tv_antenna_149: return "tv_antenna_149";
        case objects::crusher_piston_base_150: return "crusher_piston_base_150";
        case objects::object_151: return "object_151";
        case objects::object_152: return "object_152";
        case objects::object_153: return "object_153";
        case objects::invisible_bullet_154: return "invisible_bullet_154";
        case objects::moving_cone_155: return "moving_cone_155";
        case objects::moving_blaster_156: return "moving_blaster_156";
        case objects::moving_bin_157: return "moving_bin_157";
        case objects::moving_bin_lid_158: return "moving_bin_lid_158";
        case objects::invisible_rocket_159: return "invisible_rocket_159";
        case objects::rocket_160: return "rocket_160";
        case objects::savepoint_161: return "savepoint_161";
        case objects::car_shop_162: return "car_shop_162";
        case objects::blue_phone_not_ringing_163: return "blue_phone_not_ringing_163";
        case objects::blue_phone_164: return "blue_phone_164";
        case objects::light_165: return "light_165";
        case objects::diagonal_wall_collision_obj_166: return "diagonal_wall_collision_obj_166";
        case objects::door_trigger_167: return "door_trigger_167";

        case objects::yellow_phone_answered_189: return "yellow_phone_answered_189";
        case objects::green_phone_after_answer_190: return "green_phone_after_answer_190";
        case objects::green_phone_answered_191: return "green_phone_answered_191";
        case objects::shotgun_bullet_192: return "shotgun_bullet_192";
        case objects::invisible_shotgun_bullet_193: return "invisible_shotgun_bullet_193";
        case objects::fire_hitting_194: return "fire_hitting_194";
        case objects::flamethrower_bullet_195: return "flamethrower_bullet_195";
        case objects::animating_rubbish_196: return "animating_rubbish_196";
        case objects::fire_197: return "fire_197";
        case objects::water_hitting_198: return "water_hitting_198";
        case objects::fire_truck_gun_bullet_199: return "fire_truck_gun_bullet_199";
        case objects::pistol_weapon_200: return "pistol_weapon_200";

        case objects::secret_token_266: return "secret_token_266";

        default:
            return "unknown_object";
    }
}

static Camera* GetPlayerCam()
{
    if (gGame_0x40)
    {
        Player* pPlayer = gGame_0x40->mLocalPlayer;
        if (pPlayer)
        {
            if (pPlayer->mInDebugCamMode)
            {
                return &pPlayer->mViewCamera;
            }
            if (pPlayer->mCameraMode == 2 || pPlayer->mCameraMode == 3)
            {
                return &pPlayer->mAuxGameCamera;
            }
            else
            {
                return &pPlayer->mGameCamera;
            }
        }
    }
    return NULL;
}

EXTERN_GLOBAL(Fix16, kFpOne_7064C4);
EXTERN_GLOBAL(Fix16, kFpEight_7064E8);
EXTERN_GLOBAL(s16, gDebugFont);

static void ProjectXYZ_intoScreen(Fix16 xpos, Fix16 ypos, Fix16 zpos, Fix16& screen_x, Fix16& screen_y, Camera* pCam)
{
    Fix16 zCalc = (kFpOne_7064C4) /
        (kFpEight_7064E8 + (pCam->mCurPos.mZ - zpos));

    Fix16 scaled_x = pCam->mScreenScale.x * (xpos - pCam->mCurPos.mX);
    screen_x = ((zCalc * scaled_x)) + Fix16(320);

    Fix16 scaled_y = (pCam->mScreenScale.y * (ypos - pCam->mCurPos.mY));
    screen_y = ((zCalc * scaled_y) + Fix16(240));
}

static void DisplayWideTextAtSprite(wchar_t* pStr, Sprite* pSprt, s16 x_offset, s16 y_offset)
{
    if (gHud_2B00)
    {
        Camera* pPlayerCam = GetPlayerCam();
        if (pPlayerCam && pSprt)
        {
            Fix16 sprt_xpos = pSprt->mXy.x;
            Fix16 sprt_ypos = pSprt->mXy.y;
            Fix16 sprt_zpos = pSprt->mZpos;
            if (pPlayerCam->IsCoordsPosVisible(sprt_xpos, sprt_ypos, sprt_zpos))
            {
                Fix16 screen_xpos_f16;
                Fix16 screen_ypos_f16;
                ProjectXYZ_intoScreen(sprt_xpos, sprt_ypos, sprt_zpos, screen_xpos_f16, screen_ypos_f16, pPlayerCam);

                s16 screen_xpos = screen_xpos_f16.ToInt() + x_offset;
                s16 screen_ypos = screen_ypos_f16.ToInt() + y_offset;

                if (gHud_2B00->mTexts.mpFreeList) // avoid annoying crash when pausing the game
                {
                    gHud_2B00->mTexts.DisplayText(pStr, screen_xpos, screen_ypos, gDebugFont, 1);
                }
            }
        }
    }
}

static void DisplayTextAtSprite(char* pStr, Sprite* pSprt, s16 x_offset, s16 y_offset)
{
    DisplayWideTextAtSprite(TextTable::Ascii2Wide(pStr), pSprt, x_offset, y_offset);
}

// Draw Text at fixed position in screen (like OG debug stuff)
static void DisplayWideTextAtScreenCoords(wchar_t* pStr, s16 screen_xpos, s16 screen_ypos)
{
    if (gHud_2B00 && gHud_2B00->mTexts.mpFreeList) // avoid annoying crash when pausing the game
    {
        gHud_2B00->mTexts.DisplayText(pStr, screen_xpos, screen_ypos, gDebugFont, 1);
    }
}

static void DisplayTextAtScreenCoords(char* pStr, s16 screen_xpos, s16 screen_ypos)
{
    DisplayWideTextAtScreenCoords(TextTable::Ascii2Wide(pStr), screen_xpos, screen_ypos);
}

static void DisplayWideTextAtXYZ(wchar_t* pStr, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (gHud_2B00)
    {
        Camera* pPlayerCam = GetPlayerCam();
        if (pPlayerCam)
        {
            if (pPlayerCam->IsCoordsPosVisible(xpos, ypos, zpos))
            {
                Fix16 screen_xpos_f16;
                Fix16 screen_ypos_f16;
                ProjectXYZ_intoScreen(xpos, ypos, zpos, screen_xpos_f16, screen_ypos_f16, pPlayerCam);

                s16 screen_xpos = screen_xpos_f16.ToInt();
                s16 screen_ypos = screen_ypos_f16.ToInt();

                if (gHud_2B00->mTexts.mpFreeList) // avoid annoying crash when pausing the game
                {
                    gHud_2B00->mTexts.DisplayText(pStr, screen_xpos, screen_ypos, gDebugFont, 1);
                }
            }
        }
    }
}

void AddMenuOption(u32 page_idx, s16 xpos, s16 ypos, const wchar_t* pLabelStr, s16 target_page)
{
    u16 num_options = gFrontend->mMenuPagesArray[page_idx].mNumberOfOptions;
    gFrontend->mMenuPagesArray[page_idx].mOptionsArray[num_options].mOptionType = STRING_TEXT_1;
    gFrontend->mMenuPagesArray[page_idx].mOptionsArray[num_options].mXPos = xpos;
    gFrontend->mMenuPagesArray[page_idx].mOptionsArray[num_options].mYPos = ypos;
    wcsncpy(gFrontend->mMenuPagesArray[page_idx].mOptionsArray[num_options].mOptionNameStr,
            pLabelStr,
            0x32u);
    gFrontend->mMenuPagesArray[page_idx].mOptionsArray[num_options].mMenuPageTarget = target_page;
    gFrontend->mMenuPagesArray[page_idx].mNumberOfOptions++;
}

Network_Enumerated_Map multiplayer_maps[100];
char mmp_maps_description[100][256];
bool bExtMapsLoaded = false;
u16 total_num_maps_loaded = 0;

void LoadMapsFromData(Network_Enumerated_Map* pOut, u16& num_maps_loaded)
{
    char FileName[MAX_PATH];
    Network_Enumerated_Map enumerated_mmp_name[99];
    WIN32_FIND_DATAA findFileData;
    
    memset(&findFileData, 0, sizeof(findFileData));
    strcpy(FileName, "data\\");
    strcat(FileName, "*.mmp");

    u32 map_count = 0;
    HANDLE hFindFile = FindFirstFileA(FileName, &findFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        map_count = 1;
        strcpy(enumerated_mmp_name[0].mMapName, findFileData.cFileName);
        for (; FindNextFileA(hFindFile, &findFileData); map_count += 1)
        {
            if (map_count >= GTA2_COUNTOF(enumerated_mmp_name))
            {
                break;
            }
            strcpy(enumerated_mmp_name[map_count].mMapName, findFileData.cFileName);
        }
        FindClose(hFindFile);
    }

    u32 total_map_count = 0;
    if (map_count > 0)
    {
        for (u32 i = 0; i < map_count; i++)
        {
            strcpy(FileName, "data\\");
            strcat(FileName, static_cast<const char *>(enumerated_mmp_name[i].mMapName));
            GetPrivateProfileStringA(
                "MapFiles",
                "GMPFile",
                "",
                static_cast<LPSTR>(enumerated_mmp_name[i].mMapName),
                0x103u,
                FileName
            );
            GetPrivateProfileStringA(
                "MapFiles",
                "STYFile",
                "",
                static_cast<LPSTR>(enumerated_mmp_name[i].mStyleName),
                0x103u,
                FileName
            );
            GetPrivateProfileStringA(
                "MapFiles",
                "SCRFile",
                "",
                static_cast<LPSTR>(enumerated_mmp_name[i].mScriptName),
                0x103u,
                FileName
            );
            GetPrivateProfileStringA(
                "MapFiles",
                "Description",
                "",
                static_cast<LPSTR>(enumerated_mmp_name[i].mDescription),
                0x103u,
                FileName
            );
            // Now check if the map exists
            _chdir("data");
            if (GetFileAttributesA(static_cast<LPCSTR>(enumerated_mmp_name[i].mMapName)) != -1
                && GetFileAttributesA(static_cast<LPCSTR>(enumerated_mmp_name[i].mStyleName)) != -1
                && GetFileAttributesA(static_cast<LPCSTR>(enumerated_mmp_name[i].mScriptName)) != -1)
            {
                memcpy(&pOut[total_map_count], &enumerated_mmp_name[i], sizeof(Network_Enumerated_Map));
                strcpy(&mmp_maps_description[total_map_count][0], reinterpret_cast<const char*>(&enumerated_mmp_name[i].mDescription));
                total_map_count++;
            }
            _chdir("..");
        }
    }
    num_maps_loaded = total_map_count;
}

// Packs the NUL-padded rows of a char[][] into the NUL-separated item list ImGui::Combo takes
char* flat_char_array(const char* pArray, u16 num_items)
{
    static char flattened_arr[25600];
    u16 flat_idx = 0;
    for (u16 i = 0; i < num_items; i++)
    {
        while (*pArray)
        {
            flattened_arr[flat_idx] = *pArray;
            ++pArray;
            ++flat_idx;
        }
        flattened_arr[flat_idx] = 0;
        flat_idx++;

        while (!*pArray)
        {
            pArray++;
        }
    }
    return flattened_arr;
}

bool PointArrowToEntity(Ped* pPed, Car* pCar, Object* pObj)
{
    if (gHud_2B00)
    {
        if (!gpArrow)
        {
            gpArrow = gHud_2B00->mArrows.AllocArrow();
        }
        if (gpArrow)
        {
            ArrowTrace* pTarget = &gpArrow->mTargets.mPrimaryTarget;
            if (pPed)
            {
                pTarget->mPed = pPed;
                pTarget->mTargetType = ArrowTargetType::Ped_2;
            }
            else if (pCar)
            {
                pTarget->mCar = pCar;
                pTarget->mTargetType = ArrowTargetType::Car_3;
            }
            else if (pObj)
            {
                pTarget->mObj = pObj;
                pTarget->mTargetType = ArrowTargetType::Object_4;
            }

            if (pTarget->mTargetType != ArrowTargetType::Nothing_0)
            {
                return true;
            }
        }
    }
    return false;
}

void ClearGlobalArrow()
{
    if (gpArrow)
    {
        gpArrow->ClearTargets();
    }
}

namespace HookManagement
{
static TEnumerateFuncsFn GetEnumerateFuncsFn()
{
    HMODULE hMod = ::GetModuleHandle("HookLoader.dll");
    if (hMod)
    {
        return reinterpret_cast<TEnumerateFuncsFn>(::GetProcAddress(hMod, "EnumerateFuncs"));
    }
    return NULL;
}

static TChangeHookFn GetChangeHookFn()
{
    HMODULE hMod = ::GetModuleHandle("HookLoader.dll");
    if (hMod)
    {
        return reinterpret_cast<TChangeHookFn>(::GetProcAddress(hMod, "ChangeHook"));
    }
    return NULL;
}

struct FuncData
{
    const char* funcName;
    u32 ogAddr;
    bool hooked;
};
static std::vector<FuncData> gStubFuncs;
static std::vector<FuncData> gWipFuncs;

extern "C"
{
    void OnEntryFn(const char* funcName, u32 ogAddr, u32 status)
    {
        FuncData d;
        d.funcName = funcName;
        d.ogAddr = ogAddr;
        d.hooked = false;
        if (status == 0)
        {
            gStubFuncs.push_back(d);
        }
        else if (status == 2)
        {
            gWipFuncs.push_back(d);
        }
    }
}

static void DrawHookList(char* filter, std::vector<FuncData>& funcs, TChangeHookFn pChangeHookFn)
{
    u32 i;

    ImGui::InputText("Filter", filter, 256);

    if (ImGui::Button("Toggle all listed"))
    {
        for (i = 0; i < funcs.size(); i++)
        {
            FuncData& d = funcs[i];
            if (filter[0] == 0 || StrStrIA(d.funcName, filter))
            {
                d.hooked = !d.hooked;
                pChangeHookFn(d.funcName, d.ogAddr, d.hooked);
            }
        }
    }

    static bool enableAllSlowly = false;
    if (ImGui::Button("Slowly enable all"))
    {
        enableAllSlowly = true;
    }

    if (enableAllSlowly)
    {
        static s32 delay = 0;
        delay++;
        if (delay > 10)
        {
            delay = 0;
            for (i = 0; i < funcs.size(); i++)
            {
                FuncData& d = funcs[i];
                if (!d.hooked)
                {
                    d.hooked = !d.hooked;
                    printf("Enable %s\n", d.funcName);
                    pChangeHookFn(d.funcName, d.ogAddr, d.hooked);
                    break;
                }
            }
        }
    }

    for (i = 0; i < funcs.size(); i++)
    {
        FuncData& d = funcs[i];
        if (filter[0] == 0 || StrStrIA(d.funcName, filter))
        {
            if (ImGui::Checkbox(d.funcName, &d.hooked))
            {
                pChangeHookFn(d.funcName, d.ogAddr, d.hooked);
            }
        }
    }    
}

static void HooksDebug()
{
    static TEnumerateFuncsFn pGetEnumerateFuncs = GetEnumerateFuncsFn();
    static TChangeHookFn pChangeHookFn = GetChangeHookFn();
    if (pGetEnumerateFuncs && pChangeHookFn)
    {
        static bool collectedFuncs = false;
        if (!collectedFuncs)
        {
            pGetEnumerateFuncs(OnEntryFn);
            collectedFuncs = true;
        }

        if (!gStubFuncs.empty() || !gWipFuncs.empty())
        {
            ImGui::Begin("Hooks debug");

            if (!gStubFuncs.empty() && ImGui::TreeNode("Stubs"))
            {
                static char filter[256];
                DrawHookList(filter, gStubFuncs, pChangeHookFn);
                ImGui::TreePop();
            }

            if (!gWipFuncs.empty() && ImGui::TreeNode("Wips"))
            {
                static char filter[256];
                DrawHookList(filter, gWipFuncs, pChangeHookFn);
                ImGui::TreePop();
            }

            ImGui::End();
        } 
    }
}
}

void BootMap(const char* mapName, const char* styName, const char* scrName)
{
    if (gFrontend && gFrontend->mState == FrontendState::Menu_Navigation_1)
    {
        char fullPath[256];

        gGameSession.SetSaveFileName("");
        strcpy(fullPath, "data\\");
        strcat(fullPath, mapName);
        gGameSession.SetMapName(fullPath);
        strcpy(fullPath, "data\\");
        strcat(fullPath, styName);
        gGameSession.SetStyleName(fullPath);
        strcpy(fullPath, "data\\");
        strcat(fullPath, scrName);
        gGameSession.SetScriptName(fullPath);

        gGameSession.SetStartedFromPlayBonusMenu(false);

        if (!HookManagement::GetEnumerateFuncsFn()) // if standalone version
        {
            EnableBoot2MapDebugOptions();
        }

        gFrontend->mMenuScreen = RedBar_16;
        gFrontend->mState = FrontendState::Booting_Map_2;
    }
}

void ConvertBitSetIntoString(char* pOut, u8 max_width, s32 bitset, u8 max_num_bits)
{
    for (u8 i = 0; i < max_num_bits; i++, pOut++)
    {
        *pOut = ( (bitset >> i) & 1 ) ? '1' : '0';
        if (i > 0 && ((i+1) % max_width) == 0)
        {
            pOut++;
            *pOut = '\n';
        }
    }
    *pOut = 0;
}

void CC ImGuiDebugDraw()
{
    HookManagement::HooksDebug();

    ImGui::Begin("Debugger");

    if (ImGui::Button("set boot2map debug opts"))
    {
        EnableBoot2MapDebugOptions();
    }

    ImGui::InputFloat("UI scale", &ImGui::GetIO().FontGlobalScale, 0.1f, 0.5f);
    if (ImGui::GetIO().FontGlobalScale < 1.0f)
    {
        ImGui::GetIO().FontGlobalScale = 1.0f;
    }
    else if (ImGui::GetIO().FontGlobalScale > 4.0f)
    {
        ImGui::GetIO().FontGlobalScale = 4.0f;
    }


    if (ImGui::TreeNode("Exploding score"))
    {
        static ExplodingScore score;
        Sprite* pPlayerSprite = GetPlayerSprite();
        if (ImGui::Button("Single number"))
        {
            if (pPlayerSprite)
            {
                static s32 number = 3;
                score.mX = pPlayerSprite->GetXPos();
                score.mY = pPlayerSprite->GetYPos();
                score.mZ = pPlayerSprite->GetZPos();
                score.mPalette = 9; // 9-13
                score.mAlpha = 25;
                score.mExplodeCounter = 2;
                score.mXOffsetMult = 1;
                score.mYOffsetMult = 1;
                score.DrawSingleNumber(number, 10);
            }
        }

        if (ImGui::Button("Push score"))
        {
            if (pPlayerSprite && gExplodingScorePool)
            {
                gExplodingScorePool->PushScore(pPlayerSprite->GetXPos(), pPlayerSprite->GetYPos(), pPlayerSprite->GetZPos(), 999999999);
            }
        }
    }

    if (ImGui::TreeNode("Sound"))
    {
        if (ImGui::Button("CycleRadioStation_40F070 (next)"))
        {
            gRoot_sound.CycleRadioStation(0);
        }

        if (ImGui::Button("CycleRadioStation_40F070 (prev)"))
        {
            gRoot_sound.CycleRadioStation(1);
        }

        if (ImGui::Button("AppendRadioMessageSuffix_4273B0"))
        {
            gSound_obj.AppendRadioMessageSuffix();
        }

        if (ImGui::Button("EnqueueRadioWord_4271B0"))
        {
            gSound_obj.EnqueueRadioWord(0x78u);
        }

    }

    if (ImGui::TreeNode("gText_0x14_704DFC"))
    {
        static s32 model_num = 0;
        ImGui::InputInt("model_num", &model_num, 1, 1);

        static char tmpBuffer[512];
        sprintf(tmpBuffer, "c%02d", model_num);
        ImGui::Text(gText_0x14->Wide2PesudoAscii(gText_0x14->Find(tmpBuffer)));

        static s32 currentCharIndex = 0;
        const char* char_lang_codes[] = {"english", "french", "italian", "german", "spanish", "russian", "japanese"};
        const char char_lang[] = {'e', 'f', 'i', 'g', 's', 'r', 'j'};

        const s32 charCount = sizeof(char_lang_codes) / sizeof(char_lang_codes[0]);
        if (ImGui::Combo("Lang Code", &currentCharIndex, char_lang_codes, charCount))
        {
            // Lang Code selection changed
        }

        if (ImGui::Button("Change language"))
        {
            gText_0x14->mLangCode = char_lang[currentCharIndex];
            gText_0x14->Load();
            if (gFrontend)
            {
                gFrontend->SetupMenuStringsOptionsElements();
            }
        }
        ImGui::Text("Curr Lang Code: %c", gText_0x14->mLangCode);
    }

    if (gGame_0x40)
    {
        // Put in-game debug stuff here

        Sprite* pPlayerSprite = GetPlayerSprite();
        if (ImGui::Button("Water cannon peds"))
        {
            if (gPedPool)
            {
                Ped* pIter = gPedPool->mPool.mpPrev;
                while (pIter)
                {
                    Sprite* pPedSprite = GetPedSprite(pIter);
                    if (pPedSprite && pPedSprite != pPlayerSprite)
                    {
                        gParticle_8->EmitFireTruckSprayParticle(pPedSprite);
                    }
                    pIter = pIter->mpNext;
                }
            }
        }

        if (ImGui::Button("World burn"))
        {
            if (gPedPool)
            {
                Ped* pIter = gPedPool->mPool.mpPrev;
                while (pIter)
                {
                    if (pIter != GetPlayerPed())
                    {
                        pIter->SetOnFire();
                    }
                    pIter = pIter->mpNext;
                }
            }
        }

        if (pPlayerSprite)
        {

            if (ImGui::Button("Testing1"))
            {
                gGame_0x40->mPlayers[0]->mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 6);
            }

            if (ImGui::Button("Testing2"))
            {
                gGame_0x40->mPlayers[0]->mScores.GetScoreDigits()->ColorDigits(palette_types_enum::sprites_2, 0);
            }
        }
        else
        {
            ImGui::Text("Particle test (disabled - no player)");
        }

        if (gCar_BC_Pool && ImGui::TreeNode("Car Pool"))
        {
            Car* pCarIter = gCar_BC_Pool->mPool.mpPrev;

            if (ImGui::TreeNode("All cars field_88_despawn_status"))
            {
                while (pCarIter)
                {
                    swprintf(tmpBuff, L"%d", pCarIter->mDespawnStatus);
                    DisplayWideTextAtSprite(tmpBuff, pCarIter->GetSprite(), 0, 0);
                    pCarIter = pCarIter->mpNext;
                }
                ImGui::TreePop();
            }

            pCarIter = gCar_BC_Pool->mPool.mpPrev;

            while (pCarIter)
            {
                char buffer[128];
                sprintf(buffer, "Car %S %X", pCarIter->GetCarStr(), reinterpret_cast<u32>(pCarIter));
                if (ImGui::TreeNode(buffer))
                {

                    ImGui::Begin(buffer);

                    ImGui::SliderInt("field_A0_car_kind", &pCarIter->mCarKind, 0, 120);

                    ImGui::Text("ang %d", pCarIter->mCarSprite->mRotation.rValue);
                    ImGui::Text("field_7C_ped_type %d", pCarIter->mPedType);
                    ImGui::Text("field_74_damage %d", pCarIter->mDamage);

                    if (ImGui::Button("TurnToWreck_4436A0"))
                    {
                        pCarIter->mDespawnStatus = car_despawn_status::marked_for_despawn_4;
                        pCarIter->TurnToWreck();
                    }

                    if (ImGui::Button("SpawnDamageFireEffect_43B870 1"))
                    {
                        Fix16_Point p = pCarIter->mCarSprite->get_x_y_443580();
                        pCarIter->SpawnDamageFireEffect(1, &p);
                    }

                    if (ImGui::Button("SpawnDamageFireEffect_43B870 2"))
                    {
                        Fix16_Point p;
                        p.x = 0;
                        p.y = 0;
                        pCarIter->SpawnDamageFireEffect(3, &p);
                    }
                    
                    if (ImGui::Button("AssignRandomRemap_43A7D0"))
                    {
                        pCarIter->AssignRandomRemap();
                    }

                    if (pCarIter->mDriver)
                    {
                        ImGui::Text("Driver occupation: %s", GetOccupationStrFromPed(pCarIter->mDriver));
                        ShowPedBitMask(pCarIter->mDriver);
                        ShowPedBitMaskSetting(&pCarIter->mDriver->mBf21C);
                    }

                    static bool bits[32];
                    ImGui::Checkbox("top left damage", &bits[0]);
                    ImGui::Checkbox("top right damage", &bits[1]);
                    ImGui::Checkbox("bottom right damage", &bits[2]);
                    ImGui::Checkbox("bottom left damage", &bits[3]);
                    ImGui::Checkbox("windsheild damage", &bits[4]);

                    ImGui::Checkbox("back right brakelight", &bits[5]);
                    ImGui::Checkbox("front right headlight", &bits[6]);

                    ImGui::Checkbox("bottom right door frame 1", &bits[7]);
                    ImGui::Checkbox("bottom right door frame 2", &bits[8]);
                    ImGui::Checkbox("bottom right door frame 3", &bits[9]);
                    ImGui::Checkbox("bottom right door frame 4", &bits[10]);

                    ImGui::Checkbox("top right door frame 1", &bits[11]);
                    ImGui::Checkbox("top right door frame 2", &bits[12]);
                    ImGui::Checkbox("top right door frame 3", &bits[13]);
                    ImGui::Checkbox("top right door frame 4", &bits[14]);

                    // Star on tv vans, prob other roof stuff
                    ImGui::Checkbox("bottom left roof light", &bits[15]);
                    ImGui::Checkbox("bottom right roof light", &bits[16]);
                    ImGui::Checkbox("top left roof light", &bits[17]);
                    ImGui::Checkbox("top right roof light", &bits[18]);

                    ImGui::Checkbox("bFlag19", &bits[19]);
                    ImGui::Checkbox("bUnused20", &bits[20]);
                    ImGui::Checkbox("bUnused21", &bits[21]);

                    ImGui::Checkbox("back left brakelight", &bits[22]);
                    ImGui::Checkbox("front left headlight", &bits[23]);

                    ImGui::Checkbox("bottom left door frame 1", &bits[24]);
                    ImGui::Checkbox("bottom left door frame 2", &bits[25]);
                    ImGui::Checkbox("bottom left door frame 3", &bits[26]);
                    ImGui::Checkbox("bottom left door frame 4", &bits[27]);

                    ImGui::Checkbox("top left door frame 1", &bits[28]);
                    ImGui::Checkbox("top left door frame 2", &bits[29]);
                    ImGui::Checkbox("top left door frame 3", &bits[30]);
                    ImGui::Checkbox("top left door frame 4", &bits[31]);

                    for (u8 i = 0; i < 32; i++)
                    {
                        if (bits[i])
                        {
                            pCarIter->mDamagedAreas.set_bit(i);
                        }
                        else
                        {
                            pCarIter->mDamagedAreas.clear_bit(i);
                        }
                    }

                    ImGui::End();
                    ImGui::TreePop();
                }
                pCarIter = pCarIter->mpNext;
            }
            ImGui::TreePop();
        }

        if (gpMapRenderer)
        {
            if (ImGui::TreeNode("MapRenderer & Lights"))
            {
                ImGui::SliderInt("Ambient Level", &gpMapRenderer->mAmbient.mValue, 0, 16383);

                static u8 shading_level = 15;
                ImGui::SliderU8("Shading Level", &shading_level, 0, 255);

                if (ImGui::Button("Apply Shading Level"))
                {
                    gpMapRenderer->set_shading_lev(shading_level);
                }
                if (ImGui::TreeNode("Colours"))
                {
                    ImGui::Value("field C", gpMapRenderer->mColourT1);
                    ImGui::Value("field D", gpMapRenderer->mRightColour);
                    ImGui::Value("field E", gpMapRenderer->mColourT2);
                    ImGui::Value("field F", gpMapRenderer->mColourT3);
                    ImGui::Value("field 10", gpMapRenderer->mDiagUpLeftColour);
                    ImGui::Value("field 11", gpMapRenderer->mDiagUpRightColour);
                    ImGui::Value("field 12", gpMapRenderer->mDiagDownLeftColour);
                    ImGui::Value("field 13", gpMapRenderer->mDiagDownRightColour);
                    ImGui::Value("field 14", gpMapRenderer->mDcolour);
                    ImGui::Value("field 15", gpMapRenderer->mSlopeSouthColour);
                    ImGui::Value("field 16", gpMapRenderer->mSlopeWestColour);
                    ImGui::Value("field 17", gpMapRenderer->mSlopeEastColour);
                    ImGui::Value("field 18", gpMapRenderer->mColor);
                    ImGui::Value("field 19", gpMapRenderer->mTriDiagUpRightColour);
                    ImGui::Value("field 1A", gpMapRenderer->mTriDiagDownLeftColour);
                    ImGui::Value("field 1B", gpMapRenderer->mTriDiagDownRightColour);
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Globals"))
                {
                    ImGui::SliderInt("kZeroOnePoint_6F6484.x", &kZeroOnePoint.x.mValue, 0, 32768);
                    ImGui::SliderInt("kZeroOnePoint_6F6484.y", &kZeroOnePoint.y.mValue, 16, 32768);

                    if (ImGui::Button("Return to default values"))
                    {
                        kZeroOnePoint.x = Fix16(0);
                        kZeroOnePoint.y = Fix16(1);
                    }

                    if (ImGui::Button("Test 1,1"))
                    {
                        kZeroOnePoint.x = Fix16(1);
                        kZeroOnePoint.y = Fix16(1);
                    }

                    if (ImGui::Button("Test 0,2"))
                    {
                        kZeroOnePoint.x = Fix16(0);
                        kZeroOnePoint.y = Fix16(2);
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }

        if (ImGui::TreeNode("Camera"))
        {
            if (ImGui::TreeNode("gViewCamera_676978"))
            {
                if (gViewCamera)
                {
                    ImGui::InputInt("field_60_screen_scale", &gViewCamera->mScreenScale.x.mValue, 1, 1);
                    ImGui::InputInt("field_60_screen_scale", &gViewCamera->mScreenScale.y.mValue, 1, 1);
                    ImGui::Text("field_78_view_bounds.field_0_left %f", gViewCamera->mViewBounds.mLeft.ToFloat());
                    ImGui::Text("field_78_view_bounds.field_4_right %f", gViewCamera->mViewBounds.mRight.ToFloat());
                    ImGui::Text("field_78_view_bounds.field_8_top %f", gViewCamera->mViewBounds.mTop.ToFloat());
                    ImGui::Text("field_78_view_bounds.field_C_bottom %f", gViewCamera->mViewBounds.mBottom.ToFloat());
                    ImGui::SliderInt("field_98_cur_pos.field_C_zoom", &gViewCamera->mCurPos.mZoom.mValue, 0, 30000);
                    ImGui::SliderInt("screen_px_center_x", &gViewCamera->mScreenPxCenterX, 0, 2000);
                    ImGui::SliderInt("screen_px_center_y", &gViewCamera->mScreenPxCenterY, 0, 1200);

                    ImGui::SliderInt("gXCoord_6F63AC", &gXCoord.mValue, 0, 2000);
                    ImGui::SliderInt("gYCoord_6F63B8", &gYCoord.mValue, 0, 2000);
                    ImGui::SliderInt("gZCoord_6F63E0", reinterpret_cast<s32*>(&gZCoord), 0, 2000);
                }

                if (ImGui::Button("PathFinder::FindNearbyTileMatchingSlopeType_5552B0"))
                {
                    u8 xpos = gViewCamera->mViewBounds.mLeft.ToInt() + 5;
                    u8 ypos = gViewCamera->mViewBounds.mTop.ToInt() + 5;
                    u8 zpos = 2;
                    if (gPathFinder->FindNearbyTileMatchingSlopeType(0, &xpos, &ypos, &zpos, true))
                    {
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Game Camera"))
            {
                if (gGame_0x40)
                {
                    Player* pPlayer = gGame_0x40->mPlayers[0];
                    if (pPlayer)
                    {
                        Camera* game_camera = &pPlayer->mGameCamera;
                        if (game_camera)
                        {
                            ImGui::SliderInt("field_98_cur_pos.field_C_zoom", &game_camera->mCurPos.mZoom.mValue, 0, 25000);
                        }
                    }
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (gFirefighterPool_54 && ImGui::TreeNode("gFirefighterPool_54_67D4C0"))
        {
            if (ImGui::Button("TryDispatchFirefightersToCar_4A8820"))
            {
                gFirefighterPool_54->TryDispatchFirefightersToCar(NULL);
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gCollisionCounters_6791FC"))
        {
            if (gCollisionCounters)
            {
                ImGui::Text("field_0_test_count %d", gCollisionCounters->mTestCount);
                ImGui::Text("field_4_query_id %d", gCollisionCounters->mQueryId);
                ImGui::Text("field_8_bDoorAccessCheck %d", gCollisionCounters->mbDoorAccessCheck);
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gCarManager_6C_677930"))
        {
            Player* pPlayer = gGame_0x40->mPlayers[0];
            if (gCarManager_6C && pPlayer && pPlayer->mPlayerPed && pPlayer->mPlayerPed->mGameObject)
            {
                Ped* pPlayerPed = pPlayer->mPlayerPed;

                Char* pPlayerChar = pPlayerPed->mGameObject;

                Fix16 xpos, ypos, zpos;
                GetPlayerPos(xpos, ypos, zpos);

                Fix16 scale;
                scale.mValue = 0x4000;

                static s32 currentCarModelIndex = 0;
                const char* carModelNames[] = {
                    "Alfa",     "Allard",   "Amdb4",    "Apc",      "Bank Van",  "BMW",      "Boxcar",    "Boxtruck",  "Bug",
                    "CAR9",     "BUICK",    "BUS",      "COPCAR",   "DART",      "EDSEL",    "CAR15",     "FIAT",      "FIRETRUK",
                    "GRAHAM",   "GT24640",  "CAR20",    "GTRUCK",   "GUNJEEP",   "HOTDOG",   "HOTDOG_D1", "HOTDOG_D2", "HOTDOG_D3",
                    "ICECREAM", "ISETLIMO", "ISETTA",   "JEEP",     "JEFFREY",   "LIMO",     "LIMO2",     "MEDICAR",   "MERC",
                    "MESSER",   "MIURA",    "MONSTER",  "MORGAN",   "MORRIS",    "PICKUP",   "RTYPE",     "CAR43",     "SPIDER",
                    "SPRITE",   "STINGRAY", "STRATOS",  "STRATOSB", "STRIPETB",  "STYPE",    "STYPECAB",  "SWATVAN",   "T2000GT",
                    "TANK",     "TANKER",   "TAXI",     "TBIRD",    "TOWTRUCK",  "TRAIN",    "TRAINCAB",  "TRAINFB",   "TRANCEAM",
                    "TRUKCAB1", "TRUKCAB2", "TRUKCONT", "TRUKTRNS", "TVVAN",     "VAN",      "VESPA",     "VTYPE",     "WBTWIN",
                    "WRECK0",   "WRECK1",   "WRECK2",   "WRECK3",   "WRECK4",    "WRECK5",   "WRECK6",    "WRECK7",    "WRECK8",
                    "WRECK9",   "XK120",    "ZCX5",     "EDSELFBI", "HOTDOG_D4", "KRSNABUS", "None"};

                const s32 carModelCount = sizeof(carModelNames) / sizeof(carModelNames[0]);

                Fix16 xOff;
                xOff.FromInt(1);

                if (ImGui::Combo("Car Model", &currentCarModelIndex, carModelNames, carModelCount))
                {
                    // Car model selection changed
                }

                if (ImGui::Button("Spawn car"))
                {

                    pNewCar = gCarManager_6C->SpawnCarAt(xpos + xOff, ypos, zpos, 0, currentCarModelIndex, scale);

                    pNewCar->IncrementCarStats(car_kind::recycled_1); // avoid crashes when entering the car

                    s8 zone_idx = gGangPool_CA8->FindGangByCarModel(pNewCar->mCarInfoIdx);
                    if (zone_idx > -1)
                    {
                        Gang* pGang = gGangPool_CA8->GangByIdx(zone_idx);
                        pNewCar->AttachGangIcon(pGang->mArrowColour);
                        pNewCar->SetCarRemap(pGang->mGangCarRemap);
                    }
                }
                if (ImGui::Button("Spawn car with trailer"))
                {
                    pNewCar = gCarManager_6C->SpawnCarAt(xpos + xOff, ypos, zpos, 0, currentCarModelIndex, scale);

                    pNewCar->IncrementCarStats(car_kind::recycled_1); // avoid crashes when entering the car

                    // Spawns a cab and connected trailer, can spawn trains too apparently
                    gCarManager_6C->SpawnCabAndTrailer(xpos + xOff, ypos, 0, car_model_enum::TRUKCAB1, car_model_enum::TRUKTRNS);

                    pNewCar->SetupCarPhysicsAndSpriteBindingWithTrailer();
                    if (!pNewCar->mAI)
                    {
                        printf("Alloc AI (?)\n");
                        pNewCar->mAI = gCarAI_78_Pool->Allocate();
                    }
                    pNewCar->mAI->SetCar(pNewCar);
                    pNewCar->SpawnDriverPed();

                    pNewCar->mPedType = ped_type::script_created_5;
                    pNewCar->mLastSeenTimer = 0;
                    if (pNewCar->mDoorLock != Car_Door_Lock::locked_permanently_4)
                    {
                        pNewCar->mDoorLock = Car_Door_Lock::lockout_thief_2;
                    }

                    pNewCar->mFlags |= 0x10u;

                    pNewCar->UpdateDrawLayer();

                    pPlayer->AddCarToHistory(pNewCar);
                }

            }

            if (pNewCar)
            {
                // test things on spawned car
                static s32 x_explosion_offset = 0;
                static s32 y_explosion_offset = 0;
                ImGui::InputInt("Explosion x offset", &x_explosion_offset, 1, 1);
                ImGui::InputInt("Explosion y offset", &y_explosion_offset, 1, 1);

                static s32 explosion_type = 19;
                ImGui::InputInt("Explosion type", &explosion_type, 1, 1);

                if (ImGui::Button("EmitExplosion_43D690"))
                {
                    pNewCar->EmitExplosion(explosion_type, x_explosion_offset, y_explosion_offset);
                }
                if (ImGui::Button("HandleCarExplosion_43D840"))
                {
                    pNewCar->HandleCarExplosion(explosion_type);
                }

                ImGui::InputInt("field_88_despawn_status", &pNewCar->mDespawnStatus, 1, 1);

                if (ImGui::TreeNode("Draw Testing") && pNewCar->mCarSprite)
                {
                    swprintf(tmpBuff, L"Testing Draw\nCoords (%.1f, %.1f, %.1f)", 
                        pNewCar->mCarSprite->mXy.x.ToFloat(),
                        pNewCar->mCarSprite->mXy.y.ToFloat(),
                        pNewCar->mCarSprite->mZpos.ToFloat());
                    DisplayWideTextAtSprite(tmpBuff, pNewCar->mCarSprite, 0, 0);
                    ImGui::TreePop();
                }
            }

            if (gCarManager_6C)
            {
                ImGui::SliderInt("field_1C_explosion_offset_cycle", &gCarManager_6C->mExplosionOffsetCycle, -30, 30);

                if (ImGui::TreeNode("Draw debug stuff"))
                {
                    if (gCar_BC_Pool)
                    {
                        for (u32 i = 0; i < GTA2_COUNTOF(gCar_BC_Pool->mPool.mPool); i++)
                        {
                            Car* pCarIter = &gCar_BC_Pool->mPool.mPool[i];
                            if (pCarIter && pCarIter->mCarSprite)
                            {
                                swprintf(tmpBuff, L"%d", pCarIter->mEngineStatus);
                                DisplayWideTextAtSprite(tmpBuff, pCarIter->mCarSprite, 0, 0);
                            }
                        }
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Car AI"))
        {
            if (ImGui::TreeNode("Show Car AI info"))
            {
                s32 num_AI_count = 0;
                Car* pCarIter = gCar_BC_Pool->mPool.mpPrev;
                while (pCarIter)
                {
                    CarAI* pAI_Iter = pCarIter->mAI;
                    if (pAI_Iter && pCarIter->mCarSprite)
                    {
                        if (pAI_Iter)
                        {
                            swprintf(tmpBuff, L"%d", pAI_Iter->mProbeDirection);
                            DisplayWideTextAtSprite(tmpBuff, pCarIter->mCarSprite, 0, 0);
                        }
                        num_AI_count++;
                    }
                    pCarIter = pCarIter->mpNext;
                }
                ImGui::Value("Count", num_AI_count);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Show Nearest AI info"))
            {
                Sprite* pPlayerSprt = GetPlayerAssociatedSprite();
                if (pPlayerSprt)
                {
                    Car* pCarIter = gCar_BC_Pool->mPool.mpPrev;
                    CarAI* pNearestAI = NULL;
                    Fix16 smallest = Fix16(99999);
                    while (pCarIter)
                    {
                        CarAI* pAI_Iter = pCarIter->mAI;
                        if (pAI_Iter && pCarIter->mCarSprite)
                        {
                            Fix16 curr_dist = Fix16::MaxAbsDistance(pPlayerSprt->mXy.x, pPlayerSprt->mXy.y,
                                                        pCarIter->mCarSprite->mXy.x, pCarIter->mCarSprite->mXy.y);
                            if (curr_dist < smallest)
                            {
                                smallest = curr_dist;
                                pNearestAI = pAI_Iter;
                            }
                        }
                        pCarIter = pCarIter->mpNext;
                    }
                    
                    if (pNearestAI)
                    {
                        char buffer[50];
                        get_car_name(pNearestAI->mCar, buffer);
                        ImGui::Text("AI Found! Car: %s", buffer);
                        ImGui::Text("field_6C_yield_to_car: %d", pNearestAI->mYieldToCar != NULL);
                        if (!PointArrowToEntity(NULL, pNearestAI->mYieldToCar, NULL))
                        {
                            //ClearGlobalArrow();
                        }
                    }
                    else
                    {
                        //ClearGlobalArrow();
                    }
                }
                
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gObject_5C_6F8F84"))
        {
            Player* pPlayer = gGame_0x40->mPlayers[0];
            if (gObject_5C && pPlayer)
            {
                Ped* pPlayerPed = pPlayer->mPlayerPed;

                if (ImGui::Button("gCrimeReportQueue_67A4B8->ReportCrimeForPed"))
                {
                    // 0 = ?
                    // 1 = 10 24
                    // 2 = 10 34
                    // 3 = 10 90
                    // 7 = 10 71 crime - shooting?
                    gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::none_0, pPlayer->mPlayerPed);
                }


                static s32 spawnObjectType = 0;
                ImGui::InputInt("Object type", &spawnObjectType, 1, 1);
                ImGui::Text("Object name %s", ObjectIdToString(spawnObjectType));
                ObjectDefinition* pObjectDef = gObjectDefinitions->GetObjectDefinition(spawnObjectType);
                if (pObjectDef)
                {
                    ImGui::Value("Behaviour", pObjectDef->mBehaviorType);
                }

                if (ImGui::Button("Obj spawn"))
                {
                    Char* pPlayerChar = pPlayerPed->mGameObject;
                    Sprite* pSprite = pPlayerChar->mSpritePtr;
                    spawned_obj = gObject_5C->NewPhysicsObj(spawnObjectType,
                                                                          pSprite->mXy.x + Fix16(1),
                                                                          pSprite->mXy.y,
                                                                          pSprite->mZpos,
                                                                          0);
                }
            }
            if (spawned_obj != NULL && spawned_obj->mSprite)
            {
                ImGui::Text("Object spawned attributes:");
                static s32 sprt_lazy_idx = spawned_obj->mSprite->mTypeSpriteIdx;
                
                if (ImGui::InputInt("Sprite Lazy Idx", &sprt_lazy_idx, 1, 1))
                {
                    if (spawned_obj->mSprite)
                    {
                        spawned_obj->mSprite->set_id_lazy(sprt_lazy_idx);
                    }
                }
                Object_3C* obj_3c = spawned_obj->mObj3c;
                if (ImGui::TreeNode("Object_3C"))
                {
                    if (obj_3c)
                    {
                        ImGui::SliderS16("Obj3C angle", &obj_3c->mAngle.rValue, 0, 1439);
                        ImGui::InputS16("Obj3C field_6_unused", &obj_3c->mUnused6, 0, 1439);
                        ImGui::Value("Obj3C field_C_speed", obj_3c->mSpeed.mValue);
                        ImGui::Value("Obj3C field_10_z_speed", obj_3c->mZSpeed.mValue);
                        ImGui::Value("Obj3C field_14_saved_speed", obj_3c->mSavedSpeed.mValue);
                        ImGui::Value("Obj3C field_18_friction", obj_3c->mFriction.mValue);
                        ImGui::Value("Obj3C field_1C_z_accel", obj_3c->mZAccel.mValue);

                        ImGui::Value("Obj3C field_2E_unused", obj_3c->mUnused2E);
                        ImGui::Value("Obj3C field_2F_bOnSlope", obj_3c->mbOnSlope);

                        ImGui::Value("Obj3C field_38_conveyor_speed", obj_3c->mConveyorSpeed);
                        ImGui::Value("Obj3C field_39_unused", obj_3c->mUnused39);
                        ImGui::Value("Obj3C field_3A_unused", obj_3c->mUnused3A);
                        ImGui::Value("Obj3C field_3B_unused", obj_3c->mUnused3B);
                    }
                    ImGui::TreePop();
                }
            }
            static bool bFound = false;
            static s32 id = -1;
            if (ImGui::Button("Search for Gore sprite"))
            {
                id++;
                bFound = false;
                for (; id < 8000; id++)
                {
                    u16 true_idx = gGtx_0x106C->GetSpriteTrueIndex(sprite_types_enum::code_obj1_4, id);
                    sprite_index* pSpriteIndex = gGtx_0x106C->get_sprite_index(true_idx);
                    if (pSpriteIndex)
                    {
                        if (pSpriteIndex->mWidth == 20
                            && pSpriteIndex->mHeight == 16)
                        {
                            bFound = true;
                            if (spawned_obj != NULL && spawned_obj->mSprite)
                            {
                                spawned_obj->mSprite->mSpriteTypeEnum = sprite_types_enum::code_obj1_4;
                                spawned_obj->mSprite->set_id_lazy(id);
                            }
                            break;
                        }
                    }
                }
            }

            if (bFound)
            {
                ImGui::Value("ID found", id);
            }

            if (gObject_2C_Pool && ImGui::TreeNode("Show Object model IDs"))
            {
                for (u32 i = 0;i < 3824; i++)
                {
                    Object* pObjIter = &gObject_2C_Pool->mPool[i];
                    if (pObjIter && pObjIter->mModel != 0)
                    {
                        swprintf(tmpBuff, L"%d", pObjIter->mModel);
                        if (pObjIter->mSprite != NULL)
                        {
                            DisplayWideTextAtSprite(tmpBuff, pObjIter->mSprite, 0, 0);
                        }
                    }
                }
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Player"))
        {
            if (gGame_0x40)
            {
                Player* pPlayer = gGame_0x40->mPlayers[0];

                if (pPlayer)
                {
                    ImGui::SliderInt("Lives", &pPlayer->mLives.mValue, 0, 99);
                    ImGui::SliderInt("Multiplier", &pPlayer->mMultipliers.mValue, 0, 99);
                    ImGui::SliderInt("Cash", &pPlayer->mScores.mMoney.mValue, 0, 999999999 - 50);
                    ImGui::SliderInt("Multiplayer frags", &pPlayer->mScores.mMultiplayerFrags.mValue, 0, 99);

                    Ped* pPlayerPed = pPlayer->mPlayerPed;
                    if (ImGui::Button("Instant gang"))
                    {
                        pPlayerPed->RecruitNearbyPeds(9, Fix16(8));
                    }

                    ImGui::SliderS16("wanted points", &pPlayerPed->mWantedPoints, 0, 12000);
                    ImGui::SliderS16("health", &pPlayerPed->mHealth, 0, 32767);

                    ImGui::SliderU8("accuracy_count", &pPlayer->mScores.mAccuracyCount, 0, 255);

                    static u16 powerup_idx = 0;
                    ImGui::SliderU16("Powerup idx", &powerup_idx, 0, 16);
                    ImGui::InputU16("Powerup timer", &pPlayer->mPowerUpTimers[powerup_idx], 1, 1);

                    if (ImGui::Button("Scare people (wielding weapon or in car)"))
                    {
                        pPlayer->mPlayerPed->AddThreateningPedToList();
                    }

                    ImGui::SliderU16("F-210", &pPlayer->mPlayerPed->mShockCounter, 0, 500);
                    ImGui::SliderU16("F-212", &pPlayer->mPlayerPed->mElectrocutionThreshold, 0, 500);

                    Car* pPlayerCar = pPlayerPed->mCar;
                    ImGui::Text("Car 0x%X", pPlayerCar);
                    if (pPlayerCar)
                    {

                        if (ImGui::TreeNode("Player Car"))
                        {
                            ImGui::Begin("Player Car");

                            char buffer[128];
                            get_car_name(pPlayerCar, buffer);
                            ImGui::Text("Car name: %s", buffer);

                            ImGui::SliderInt("field_9C_engine_status", &pPlayerCar->mEngineStatus, 1, 7);

                            if (ImGui::TreeNode("Trailer"))
                            {
                                ImGui::Text("field_64_pTrailer 0x%X", pPlayerCar->mpTrailer);
                                ImGui::TreePop();
                            }

                            if (ImGui::Button("ResprayOrCleanPlates"))
                            {
                                pPlayerCar->ResprayOrCleanPlates(2); // 0xFD - clean plates
                            }

                            CarPhysics* pPhysics = pPlayerCar->mPhysics;

                            if (pPhysics)
                            {
                                ImGui::SliderS16("Car Angle", &pPhysics->mTheta.rValue, 0, 1439);
                                ImGui::SliderInt("Car Angle Velocity", &pPhysics->mAngVelRad.mValue, -10 * 16384, 10 * 16384);
                                if (ImGui::TreeNode("Car Physics"))
                                {
                                    ImGui::Value("field_A0_oil_spin_dir", pPhysics->mOilSpinDir);
                                    ImGui::SliderInt("field_A0_oil_spin_dir", &pPhysics->mOilSpinDir, 0, 3);
                                    ImGui::Value("field_AD_turn_direction", pPhysics->mTurnDirection);
                                    ImGui::Value("Surface type", pPhysics->mSurfaceType);
                                    ImGui::TreePop();
                                }

                                if (ImGui::TreeNode("Model Physics"))
                                {
                                    ModelPhysics* pModelPhy =
                                        gCarInfo_808->GetModelPhysicsFromIdx(pPlayerCar->mCarInfoIdx);
                                    if (pModelPhy)
                                    {
                                        ImGui::SliderInt("Turn In", &pModelPhy->mTurnIn.mValue, 0, 4 * 16384);
                                        ImGui::SliderInt("Turn Ratio", &pModelPhy->mTurnRatio.mValue, 0, 4 * 16384);
                                        ImGui::SliderInt("Mass", &pModelPhy->mMass.mValue, 0, 30 * 16384);
                                        ImGui::SliderInt("Anti Strength", &pModelPhy->mAntiStrngth.mValue, 0, 10 * 16384);
                                        ImGui::SliderInt("Brake Friction", &pModelPhy->mBrakeFriction.mValue, 0, 10 * 16384);
                                        ImGui::SliderInt("Rear End Stability",
                                                         &pModelPhy->mRearEndStability.mValue,
                                                         0,
                                                         10 * 16384);
                                        ImGui::SliderInt("Thrust", &pModelPhy->mThrust.mValue, 0, 10 * 16384);
                                        ImGui::SliderInt("Skid Threshold", &pModelPhy->mSkedThreshold.mValue, 0, 10 * 16384);
                                        ImGui::SliderInt("Front Drive Bias", &pModelPhy->mFrontDriveBias.mValue, 0, 10 * 16384);
                                        ImGui::SliderInt("Front Mass Bias", &pModelPhy->mFrontMassBias.mValue, 0, 10 * 16384);
                                        ImGui::SliderS8("Turbo", reinterpret_cast<char_type*>(&pModelPhy->mTurbo), 0, 10);
                                    }
                                    ImGui::TreePop();
                                }
                            }

                            if (gGangPool_CA8)
                            {
                                if (ImGui::TreeNode("Gang Icon"))
                                {
                                    static char_type gang_idx = 0;
                                    ImGui::SliderS8("Gang idx", &gang_idx, 0, 2);

                                    Gang* pGang = &gGangPool_CA8->mGangList[gang_idx];
                                    if (pGang)
                                    {
                                        if (ImGui::Button("Attach gang icon"))
                                        {
                                            pPlayerCar->AttachGangIcon(pGang->mArrowColour);
                                        }
                                    }
                                    ImGui::TreePop();
                                }
                            }
                            
                            if (pPlayerCar->mCarSprite)
                            {
                                Sprite* pSprt = pPlayerCar->mCarSprite;
                                if (ImGui::TreeNode("Sprite"))
                                {
                                    ImGui::Input_char_type("field_38_zoom", &pSprt->mZoom, 1, 1);
                                    SpriteShape* pShape = pSprt->mpVisualShape;
                                    if (pShape)
                                    {
                                        ShowRectAndPointsForSprite4C(pShape);
                                    }
                                    ImGui::TreePop();
                                }
                            }
                            

                            ImGui::End();
                            ImGui::TreePop();
                        }
                    }
                    
                    if (pPlayerPed)
                    {

                        ImGui::InputInt("258_objective", &pPlayerPed->mObjective, 1, 1);

                        ImGui::InputInt("F 238", &pPlayerPed->mPedType, 1, 1);
                        ImGui::InputInt("Ped State 1", &pPlayerPed->mPedState1, 1, 1);
                        ImGui::InputInt("Ped State 2", &pPlayerPed->mPedState2, 1, 1);
                        ImGui::InputInt("Car State", &pPlayerPed->mInternalObjective, 1, 1);
                        ShowPedBitMask(pPlayerPed);
                    }

                    if (pPlayerPed->mGameObject)
                    {
                        ImGui::InputInt("field_10_char_state", &pPlayerPed->mGameObject->mCharState, 1, 1);
                        ImGui::InputU8("field_68_animation_frame", &pPlayerPed->mGameObject->mAnimationFrame, 1, 1);
                        ImGui::InputInt("field_6C_animation_state", &pPlayerPed->mGameObject->mAnimationState, 1, 1);
                        ImGui::Input_char_type("field_70_frame_timer", reinterpret_cast<char_type*>(&pPlayerPed->mGameObject->mFrameTimer), 1, 1);
                        ImGui::Input_char_type("field_71_frame_delay", reinterpret_cast<char_type*>(&pPlayerPed->mGameObject->mFrameDelay), 1, 1);

                        if (ImGui::TreeNode("Display CharB4 flags"))
                        {
                            ConvertBitSetIntoString(TmpBitSetChar, 8, pPlayerPed->mGameObject->mFlags, 8 * sizeof(pPlayerPed->mGameObject->mFlags));
                            DisplayTextAtSprite(TmpBitSetChar, pPlayerPed->GetSprite(), 0, 0);
                            ImGui::TreePop();
                        }
                    }

                    static s32 currentWeaponIndex = 0;
                    const char* weaponNames[] = {"Pistol",         "SMG",
                                                 "Rocket",         "Shocker",
                                                 "Molotov",        "Grenade",
                                                 "Shotgun",        "Electro Batton",
                                                 "Flamethrower",   "Silence SMG",
                                                 "Dual Pistol",    "Weapon 11",
                                                 "Weapon 12",      "Weapon 13",
                                                 "Weapon 14",      "Car Bomb",
                                                 "Oil Stain",      "Car Mines",
                                                 "Car SMG",        "Tank Main Gun",
                                                 "Fire Truck Gun", "Fire Truck Flamethrower",
                                                 "Army Gun Jeep",  "Weapon 0x17"};
                    const s32 weaponCount = sizeof(weaponNames) / sizeof(weaponNames[0]);
                    if (ImGui::Combo("Weapon Type", &currentWeaponIndex, weaponNames, weaponCount))
                    {
                        // Weapon selection changed
                    }
                    if (ImGui::Button("Add car weapon"))
                    {
                        if (pPlayerPed->mCar)
                        {
                            gWeapon_8->add_car_weapon(currentWeaponIndex, 20, pPlayerPed->mCar);
                        }
                    }
                    if (ImGui::Button("Add player weapon"))
                    {
                        pPlayerPed->AddWeaponWithAmmo(currentWeaponIndex, 99);
                    }

                    if (ImGui::Button("Shoot bullet 198"))
                    {
                        if (pPlayerPed->mSelectedWeapon)
                        {
                            pPlayerPed->mSelectedWeapon->spawn_bullet(198,
                                pPlayerPed->get_cam_x(),
                                pPlayerPed->get_cam_y(),
                                pPlayerPed->get_cam_z(),
                                pPlayerPed->GetAimAngle(),
                                pPlayerPed->GetVelocityVector());
                        }
                    }

                    Car** pCarHistory = pPlayer->mCarHistory;
                    if (pCarHistory)
                    {
                        for (u32 car_idx = 0; car_idx < 3; car_idx++)
                        {
                            Car* pCar = pCarHistory[car_idx];
                            if (pCar)
                            {
                                static char car_name[33];
                                get_car_name(pCar, car_name);
                                ImGui::Text("field_54_car_history car %d name: %s", car_idx, car_name);
                            }
                        }
                    }

                    if (ImGui::Button("Drown Player ped"))
                    {
                        if (pPlayer->mPlayerPed->mGameObject)
                        {
                            pPlayer->mPlayerPed->mGameObject->DrownPed();
                        }
                    }

                    pPlayerSprite = GetPlayerSprite();

                    if (pPlayerSprite)
                    {
                        ImGui::Input_char_type("field_38_zoom", &pPlayerSprite->mZoom, 1, 1);
                        if (pPlayerSprite->mpVisualShape)
                        {
                            ShowRectAndPointsForSprite4C(pPlayerSprite->mpVisualShape);
                        }
                    }

                    if (ImGui::TreeNode("Money counter"))
                    {
                        ImGui::Value("field_0_value", pPlayer->mScores.mMoney.mValue);
                        ImGui::Value("field_4_target_value", pPlayer->mScores.mMoney.mTargetValue);
                        ImGui::Value("field_8_bRollingUp", pPlayer->mScores.mMoney.mbRollingUp);

                        ImGui::Value("field_2A_max_num_of_digits", pPlayer->mScores.mMoney.mMaxNumOfDigits);
                        ImGui::InputS16("field_2C_digit_transition_speed", &pPlayer->mScores.mMoney.mDigitTransitionSpeed, 1, 1);
                        ImGui::Value("field_2E_non_used_digits", pPlayer->mScores.mMoney.mNonUsedDigits);
                        ImGui::Value("field_30_max_value", pPlayer->mScores.mMoney.mMaxValue);
                        ImGui::Value("field_34_first_digit_texture_idx", pPlayer->mScores.mMoney.mFirstDigitTextureIdx);
                        ImGui::Value("field_36_sprite_idx", pPlayer->mScores.mMoney.mSpriteIdx);

                        static char_type num_idx = 0;
                        ImGui::SliderS8("Array Idx", &num_idx, 0, 9);

                        ImGui::Value("field_9_shown_digits", pPlayer->mScores.mMoney.mShownDigits[num_idx]);
                        ImGui::Value("field_13_scroll_offsets", pPlayer->mScores.mMoney.mScrollOffsets[num_idx]);
                        ImGui::Value("field_1D_target_digits", pPlayer->mScores.mMoney.mTargetDigits[num_idx]);

                        ImGui::TreePop();
                    }
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gGame_0x40_67E008"))
        {
            if (ImGui::Button("DebugToggleRemoteControl_569E70"))
            {
                gGame_0x40->mLocalPlayer->DebugToggleRemoteControl();
            }
            ImGui::InputInt("Game State", &gGame_0x40->mGameState, 1, 1);

            static s32 exit_state = 0;

            ImGui::InputInt("Exit State", &exit_state, 1, 1);

            if (ImGui::Button("Apply exit state"))
            {
                gGame_0x40->ExitGameNoBonus(0, exit_state);
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("HUD"))
        {
            if (gHud_2B00)
            {
                if (ImGui::TreeNode("Arrows"))
                {
                    static s32 arrow_idx = 0;
                    ImGui::SliderInt("Arrow idx", &arrow_idx, 0, GTA2_COUNTOF(gHud_2B00->mArrows.mArray) - 1);
                    Hud_Arrow* pArrow = &gHud_2B00->mArrows.mArray[arrow_idx];

                    if (pArrow)
                    {
                        ImGui::SliderS16("Rotation", &pArrow->mRotation.rValue, 0, 1439);

                        Hud_ArrowTargets* pTargets = &pArrow->mTargets;
                        Hud_ArrowGangInfo* pGangInfo = &pTargets->mGangInfo;
                        ImGui::Input_char_type("field_5_is_visible", &pGangInfo->mIsVisible, 1, 1);
                        ImGui::Input_char_type("field_6_in_use", &pGangInfo->mInUse, 1, 1);

                        ArrowTrace* pPrimaryTrace = &pTargets->mPrimaryTarget;
                        if (pPrimaryTrace)
                        {

                            ImGui::InputInt("Primary Arrow type", &pPrimaryTrace->mTargetType, 1, 1);
                            ImGui::Input_char_type("field_20_bIsTargetVisible", &pPrimaryTrace->mbIsTargetVisible, 1, 1);
                        }
                    }

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Timer"))
                {
                    if (ImGui::Button("CreateTimer_5D31F0"))
                    {
                        gHud_2B00->mPagers.CreateTimer(40);
                    }

                    static s32 v = 0;
                    ImGui::InputInt("Timer num", static_cast<s32*>(&v), 1, 1);
                    if (ImGui::Button("Stop timer") && v <= 3)
                    {
                        gHud_2B00->mPagers.ClearClockOnly(v);
                    }

                    if (ImGui::Button("ClearPager_5D3280") && v <= 3)
                    {
                        gHud_2B00->mPagers.ClearPager(v);
                    }

                    if (ImGui::Button("AddOnScreenCounter_5D3220") && v <= 3)
                    {
                        gHud_2B00->mPagers.AddOnScreenCounter(&v);
                    }

                    if (gHud_2B00->mPagers.mPagersArray[1].mPtrCounter)
                    {
                        ImGui::InputInt("field_4_ptr_counter", gHud_2B00->mPagers.mPagersArray[1].mPtrCounter, 1, 100);
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Hud_Brief"))
                {
                    Hud_Brief* pBrief = &gHud_2B00->mBrief;

                    if (pBrief)
                    {
                        ImGui::Value("field_500_unused", pBrief->mUnused500);
                        ImGui::InputU8("field_502_face_idx", &pBrief->mFaceIdx, 1, 1);
                        ImGui::Value("field_504_tick_timer", pBrief->mTickTimer);
                        ImGui::Value("field_508_num_lines", pBrief->mNumLines);
                        ImGui::Value("field_50C_face_variant", pBrief->mFaceVariant);
                        ImGui::Value("field_510_num_chars", pBrief->mNumChars);
                        ImGui::Value("field_514_face_anim_char_idx", pBrief->mFaceAnimCharIdx);

                        if (ImGui::TreeNode("Try field_6F8_curr_briefs as text"))
                        {
                            ImGui::Text("Text: %s", pBrief->mCurrBriefs->mBriefIdStr);
                            ImGui::TreePop();
                        }

                        if (ImGui::TreeNode("Try field_6FC_free_briefs as text"))
                        {
                            ImGui::Text("Text: %s", reinterpret_cast<const char*>(pBrief->mFreeBriefs));
                            ImGui::TreePop();
                        }

                        if (ImGui::TreeNode("Try field_700_prev_briefs as text"))
                        {
                            ImGui::Text("Text: %s", reinterpret_cast<const char*>(pBrief->mPrevBriefs));
                            ImGui::TreePop();
                        }


                        Hud_BriefEntry* curr_brief = pBrief->mCurrBriefs;
                        if (curr_brief)
                        {
                            ImGui::Value("curr brief field_10_was_displayed", curr_brief->mWasDisplayed);
                            if (ImGui::TreeNode("Try field_C_pNext as text"))
                            {
                                ImGui::Text("Text: %s", reinterpret_cast<const char*>(curr_brief->mpNext));
                                ImGui::TreePop();
                            }
                        }

                        static u16 brief_idx = 0;
                        ImGui::SliderU16("Brief idx", &brief_idx, 0, 19);
                        Hud_BriefEntry* brief = &pBrief->mBriefs[brief_idx];
                        if (brief)
                        {
                            ImGui::Text("Brief id: %.8s", brief->mBriefIdStr);
                            ImGui::Value("field_8_brief_priority", brief->mBriefPriority);
                            ImGui::Value("field_10_was_displayed", brief->mWasDisplayed);

                            if (ImGui::TreeNode("Try field_C_pNext as text"))
                            {
                                if (brief->mpNext)
                                {
                                    ImGui::Text("Text: %s", static_cast<const char*>(brief->mpNext->mBriefIdStr));
                                }
                                else
                                {
                                    ImGui::Text("Null field C :(");
                                }
                                ImGui::TreePop();
                            }
                        }
                    }

                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gPedManager_6787BC"))
        {
            if (gPedManager)
            {
                if (ImGui::Button("Player invisible Ian Test 8"))
                {
                    gPedManager->DoIanTest(8);
                }
                if (ImGui::Button("Player visible Ian Test 9"))
                {
                    gPedManager->DoIanTest(9);
                }
                if (ImGui::Button("Set player on fire PIan Test 10"))
                {
                    gPedManager->DoIanTest(10);
                }

                ImGui::Text("field_0_max_dummy_chars %d", gPedManager->mMaxDummyChars);
                ImGui::Input_char_type("field_2_num_dummy_chars", &gPedManager->mNumDummyChars, 1, 1);
                ImGui::Input_char_type("field_3_num_peds_updated", &gPedManager->mNumPedsUpdated, 1, 1);
                ImGui::Input_char_type("field_4_num_script_created_peds", &gPedManager->mNumScriptCreatedPeds, 1, 1);
                ImGui::InputU8("field_5_fbi_army_count", &gPedManager->mFbiArmyCount, 1, 1);
                ImGui::Text("Num peds on screen %d", gPedManager->mNumPedsOnScreen);
                ImGui::Input_char_type("field_7_make_all_muggers", &gPedManager->mMakeAllMuggers, 1, 1);
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gPedPool_6787B8"))
        {
            if (gPedPool)
            {
                static s32 ped_state = 0;
                static s32 move_state = 0;
                ImGui::InputInt("Ped state", static_cast<s32*>(&ped_state), 1, 1);
                ImGui::InputInt("Move state", static_cast<s32*>(&move_state), 1, 1);
                
                if (ImGui::Button("Apply states globally"))
                {
                    Ped* pIter = gPedPool->mPool.mpPrev;
                    while (pIter)
                    {
                        if (pIter != GetPlayerPed())
                        {
                            pIter->ChangeNextPedState1(ped_state);
                            pIter->ChangeNextPedState2(move_state);
                        }
                        pIter = pIter->mpNext;
                    }
                }
                
                if (ImGui::TreeNode("Show ped type"))
                {
                    Ped* pPedIter = gPedPool->mPool.mpPrev;
                    while (pPedIter)
                    {
                        if (1) // pPedIter->mGameObject
                        {
                            swprintf(tmpBuff, L"%d", pPedIter->mPedType);
                            DisplayWideTextAtSprite(tmpBuff, pPedIter->GetSprite(), 0, 0);
                        }
                        pPedIter = pPedIter->mpNext;
                    }
                    ImGui::TreePop();
                }

                Player* pPlayer = gGame_0x40->mPlayers[0];
                Ped* pPlayerPed = pPlayer->mPlayerPed;

                if (ImGui::Button("Spawn 100"))
                {
                    for (s32 c = 0; c < 100; c++)
                    {
                        Char* pPlayerChar = pPlayerPed->mGameObject;
                        Sprite* pPlayerSprite = pPlayerChar->mSpritePtr;
                        gPedManager->SpawnPedAt(pPlayerSprite->mXy.x,
                                                       pPlayerSprite->mXy.y,
                                                       pPlayerSprite->mZpos,
                                                       pPlayerChar->mRemap,
                                                       pPlayerPed->mRotation);
                    }
                }

                Ped* pPedIter = gPedPool->mPool.mpPrev;

                if (ImGui::TreeNode("Show Objectives"))
                {
                    while (pPedIter)
                    {
                        char buffer[128];
                        sprintf(buffer, "Ped %d %s", pPedIter->mId, GetOccupationStrFromPed(pPedIter));
                        if (ImGui::TreeNode(buffer))
                        {
                            ImGui::Begin(buffer);

                            ImGui::SliderS16("wanted points", &pPedIter->mWantedPoints, 0, 12000);
                            ImGui::SliderS16("health", &pPedIter->mHealth, 0, 32767);
                            ImGui::SliderInt("occupation", &pPedIter->mOccupation, 0, 500);
                            ShowPedBitMask(pPedIter);
                            ShowPedBitMaskSetting(&pPedIter->mBf21C);

                            ImGui::End();
                            ImGui::TreePop();
                        }

                        if (pPedIter->mOccupation == ped_ocupation_enum::mugger
                            || pPedIter->mOccupation == ped_ocupation_enum::car_thief)
                        {
                            swprintf(tmpBuff, L"(%d, %d)", 
                            pPedIter->mBf21C.bPanicking,
                            pPedIter->mBf21C.bAttacking);
                            DisplayWideTextAtSprite(tmpBuff, pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::run_to_char_7)
                        {
                            DisplayTextAtSprite("Obj 7", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::left_behind_by_leader_8)
                        {
                            DisplayTextAtSprite("Obj 8", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::follow_char_9)
                        {
                            DisplayTextAtSprite("Obj 9", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::follow_char_in_car_10) 
                        {
                            DisplayTextAtSprite("Obj 10", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::stay_close_to_char_11) 
                        {
                            DisplayTextAtSprite("Obj 11", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::follow_path_points_15) 
                        {
                            DisplayTextAtSprite("Obj 15", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::walk_to_point_17) 
                        {
                            DisplayTextAtSprite("Obj 17", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::walk_to_car_door_18) 
                        {
                            DisplayTextAtSprite("Obj 18", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::dead_patient_28) 
                        {
                            DisplayTextAtSprite("Obj 28", pPedIter->GetSprite(), 0, 0);
                        }
                        else if (pPedIter->mObjective == objectives_enum::patrol_in_car_43) 
                        {
                            DisplayTextAtSprite("Obj 43", pPedIter->GetSprite(), 0, 0);
                        }
                        pPedIter = pPedIter->mpNext;
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Show Fields"))
                {
                    while (pPedIter)
                    {
                        if (pPedIter->mGameObject)
                        {
                            swprintf(tmpBuff, L"%d", pPedIter->mGameObject->mIsCollidingWithSprite);
                            DisplayWideTextAtSprite(tmpBuff, pPedIter->GetSprite(), 0, 0);
                        }
                        pPedIter = pPedIter->mpNext;
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gAmbulance_110_6F70A8"))
        {
            if (gAmbulance_110)
            {
                s32 i;

                for (i = 0; i < 25; i++)
                {
                    PatientSlot& slot = gAmbulance_110->mUnusedSlots[i];
                    ImGui::Value("field_0_unused", slot.mUnused0);
                    ImGui::Value("field_4_unused", slot.mUnused4);
                }

                for (i = 0; i < 2; i++)
                {
                    AmbulanceTask& task = gAmbulance_110->mTasks[i];
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gCarChaseTaskTable_678E30"))
        {
            if (gCarChaseTaskTable)
            {
                for (s32 i = 0; i < 20; i++)
                {
                    CarChaseTask& task = gCarChaseTaskTable->mEntries[i];
                    ImGui::Value("field_0_in_use", task.mInUse);
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gObjectDefinitions_6FCF00"))
        {
            if (gObjectDefinitions)
            {
                ImGui::Value("field_0_next_idx", gObjectDefinitions->mNextIdx);
                ImGui::Value("field_2_unused", gObjectDefinitions->mUnused2);
                ImGui::Value("field_8CA4_def112_base_sprite_idx", gObjectDefinitions->mDef112BaseSpriteIdx);
                ImGui::Value("field_8CA6_unused", gObjectDefinitions->mUnused8CA6);

                if (ImGui::TreeNode("Spawned object"))
                {
                    if (spawned_obj)
                    {
                        ObjectDefinition* pDef = spawned_obj->mDefinition;

                        ImGui::InputInt("field_0_width", &pDef->mWidth.mValue, 1, 1);
                        ImGui::InputInt("field_4_height", &pDef->mHeight.mValue, 1, 1);
                        ImGui::InputInt("field_8_depth", &pDef->mDepth.mValue, 1, 1);
                        ImGui::InputInt("field_C_min_size", &pDef->mMinSize.mValue, 1, 1);
                        ImGui::InputInt("field_10_speed", &pDef->mSpeed.mValue, 1, 1);
                        ImGui::InputInt("field_14_friction", &pDef->mFriction.mValue, 1, 1);
                        ImGui::InputInt("field_18_mass", &pDef->mMass.mValue, 1, 1);
                        ImGui::SliderS16("field_1C_remap", &pDef->mRemap, 0, 50);
                        ImGui::Input_char_type("field_20_sprite_flags", &pDef->mSpriteFlags, 1, 1);
                        ImGui::InputInt("field_24_object_idx", &pDef->mObjectIdx, 1, 1);
                        ImGui::InputInt("field_28_sprite_type", &pDef->mSpriteType, 1, 1);

                        if (ImGui::TreeNode("another vars"))
                        {
                            ImGui::InputInt("field_2C_draw_layer", &pDef->mDrawLayer, 1, 1);
                            ImGui::InputInt("field_30_palette_type", &pDef->mPaletteType, 1, 1);
                            ImGui::InputInt("field_34_behavior_type", &pDef->mBehaviorType, 1, 1);
                            ImGui::InputInt("field_38_impact_definition_idx", &pDef->mImpactDefinitionIdx, 1, 1);
                            ImGui::InputInt("field_3C_next_definition_idx", &pDef->mNextDefinitionIdx, 1, 1);
                            ImGui::InputInt("field_40_collision_bucket_category", &pDef->mCollisionBucketCategory, 1, 1);
                            ImGui::InputInt("field_44_touch_behavior", &pDef->mTouchBehavior, 1, 1);
                            ImGui::InputInt("field_48_projectile_hit_type", &pDef->mProjectileHitType, 1, 1);
                            ImGui::InputInt("field_4C", &pDef->mCollisionResponse, 1, 1);
                            ImGui::InputInt("field_50", &pDef->mZCollisionResponse, 1, 1);
                            ImGui::InputInt("field_54_react_to_collisions_with", &pDef->mReactToCollisionsWith, 1, 1);
                            ImGui::InputInt("field_58_uses_3d_movement", &pDef->mUses3dMovement, 1, 1);
                            ImGui::InputInt("field_5C_instance_limit_kind", &pDef->mInstanceLimitKind, 1, 1);

                            ImGui::Input_char_type("field_60_destroyed_by_destructor", &pDef->mDestroyedByDestructor, 1, 1);
                            ImGui::Input_char_type("field_61_in_pool_list", &pDef->mInPoolList, 1, 1);
                            ImGui::Input_char_type("field_62_has_shadows", &pDef->mHasShadows, 1, 1);
                            ImGui::Input_char_type("field_63_makes_collision_sound", &pDef->mMakesCollisionSound, 1, 1);
                            ImGui::Input_char_type("field_64_next_frame_max", &pDef->mNextFrameMax, 1, 1);
                            ImGui::Input_char_type("field_65_next_definition_timer", &pDef->mNextDefinitionTimer, 1, 1);

                            ImGui::InputInt("field_68_uses_bullet_walls", &pDef->mUsesBulletWalls, 1, 1);

                            ImGui::TreePop();
                        }

                        ImGui::SliderU8("field_6C_num_frames", &pDef->mNumFrames, 0, 32);
                        ImGui::InputInt("field_70_has_sound", &pDef->mHasSound, 1, 1);
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gGtx_0x106C_703DD4"))
        {
            if (gGtx_0x106C)
            {
                static s32 tile_idx = 0;
                ImGui::InputInt("Tile idx", &tile_idx, 1, 1);
                s32 spec = gGtx_0x106C->mSpec[tile_idx];
                ImGui::Value("Spec", spec);
            }
            ImGui::TreePop();
        }

        if (gPolice_7B8)
        {
            if (ImGui::TreeNode("gPolice_7B8_6FEE40"))
            {
                if (ImGui::TreeNode("Show debug stuff"))
                {
                    s16 ypos = 64;
                    for (u32 idx = 0; idx < GTA2_COUNTOF(gPolice_7B8->mCopCrew); idx++)
                    {
                        PoliceCrew* pCrew = &gPolice_7B8->mCopCrew[idx];
                        if (pCrew && pCrew->mUsed)
                        {
                            swprintf(tmpBuff, L"Crew %d state: %d", idx, pCrew->mState);
                            DisplayWideTextAtScreenCoords(tmpBuff, 0, ypos);
                            
                            // display on cop sprite
                            if (pCrew->mSubObj && pCrew->mSubObj->mGroup && pCrew->mSubObj->mGroup->mPedLeader)
                            {
                                swprintf(tmpBuff, L"State %d", pCrew->mState);
                                DisplayWideTextAtSprite(tmpBuff, pCrew->mSubObj->mGroup->mPedLeader->GetSprite(), 0, 0);
                            }
                            ypos += 16;
                        }
                    }

                    ypos += 16;
                    
                    for (u32 idx2 = 0; idx2 < GTA2_COUNTOF(gPolice_7B8->mPursuitTargets); idx2++)
                    {
                        PolicePursuitTarget* pTarget = &gPolice_7B8->mPursuitTargets[idx2];
                        if (pTarget && pTarget->mZ != kFP16Zero_6FE20C)
                        {
                            swprintf(tmpBuff, L"P7C target coords (%.1f, %.1f, %.1f)", pTarget->mX.ToFloat(), pTarget->mY.ToFloat(), pTarget->mZ.ToFloat());
                            DisplayWideTextAtScreenCoords(tmpBuff, 0, ypos);
                            ypos += 16;
                        }
                    }

                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }

        if (gMap_0x370 && ImGui::TreeNode("gMap_0x370_6F6268"))
        {
            Fix16 player_xpos;
            Fix16 player_ypos;
            Fix16 player_zpos;
            GetPlayerPos(player_xpos, player_ypos, player_zpos);
            gmp_block_info* pPlayerBlock = gMap_0x370->GetBlock(player_xpos.ToInt(), player_ypos.ToInt(), player_zpos.ToInt() - 1);
            if (pPlayerBlock)
            {
                s32 value_1 = gMap_0x370->GetArrowDirectionFromBlock(pPlayerBlock, 0);
                s32 value_2 = gMap_0x370->GetArrowDirectionFromBlock(pPlayerBlock, 1);
                ImGui::Value("GetArrowDirectionFromBlock_4E5FC0 type 0", value_1);
                ImGui::Value("GetArrowDirectionFromBlock_4E5FC0 type 1", value_2);
            }

            u8 xpos = player_xpos.ToUInt8();
            u8 ypos = player_ypos.ToUInt8();
            u8 zpos = player_zpos.ToUInt8();
            
            if (gPathFinder)
            {
                gPathFinder->mXpos25 = xpos;
                gPathFinder->mYpos26 = ypos;
                gPathFinder->mZpos27 = zpos;
                ImGui::Value("PathFinder Direction 1", gPathFinder->CanMoveInDirection(path_direction::up_1));
                ImGui::Value("PathFinder Direction 2", gPathFinder->CanMoveInDirection(path_direction::down_2));
                ImGui::Value("PathFinder Direction 3", gPathFinder->CanMoveInDirection(path_direction::right_3));
                ImGui::Value("PathFinder Direction 4", gPathFinder->CanMoveInDirection(path_direction::left_4));
            }
            
            u8 slope_z_delta;
            static bool bReportStepUp;
            ImGui::Value("Can Move Dir 1", gMap_0x370->IsMoveOntoSlopeTileBlocked(xpos, ypos, zpos, path_direction::up_1, &slope_z_delta, bReportStepUp));
            ImGui::Value("Can Move Dir 2", gMap_0x370->IsMoveOntoSlopeTileBlocked(xpos, ypos, zpos, path_direction::down_2, &slope_z_delta, bReportStepUp));
            ImGui::Value("Can Move Dir 3", gMap_0x370->IsMoveOntoSlopeTileBlocked(xpos, ypos, zpos, path_direction::right_3, &slope_z_delta, bReportStepUp));
            ImGui::Value("Can Move Dir 4", gMap_0x370->IsMoveOntoSlopeTileBlocked(xpos, ypos, zpos, path_direction::left_4, &slope_z_delta, bReportStepUp));
            ImGui::TreePop();
        }

        if (gParticle_4C_Pool && ImGui::TreeNode("gParticle_4C_Pool_6FD5E4"))
        {
            Particle_4C* pIter = gParticle_4C_Pool->mpPrev;
            for (; pIter; pIter = pIter->mpNext)
            {
                swprintf(tmpBuff, L"%d", pIter->mState);
                if (pIter->mpSprite)
                {
                    DisplayWideTextAtSprite(tmpBuff, pIter->mpSprite, 0, 0);
                }
                else if (pIter->mpOwnSprite)
                {
                    DisplayWideTextAtSprite(tmpBuff, pIter->mpOwnSprite, 0, 0);
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Car Physics Debug"))
        {
            if (ImGui::TreeNode("Show collision point"))
            {
                if (CollisionIntersectionPoint.x > Fix16(0)
                    && CollisionIntersectionPoint.y > Fix16(0))
                {
                    DisplayWideTextAtXYZ(L"E", 
                        CollisionIntersectionPoint.x,
                        CollisionIntersectionPoint.y,
                        Fix16(3));
                }
                ImGui::TreePop();
            }
            
            if (&gCollisionTarget && ImGui::TreeNode("Show collision target"))
            {
                ImGui::Value("Type", gCollisionTarget.mType);
                if (gCollisionTarget.mpHitSprite)
                {
                    if (gCollisionTarget.mpHitSprite->AsCharB4())
                    {
                        PointArrowToEntity(gCollisionTarget.mpHitSprite->mCharB4Ptr->mpPed, NULL, NULL);
                    }
                    else if (gCollisionTarget.mpHitSprite->AsCar())
                    {
                        PointArrowToEntity(NULL, gCollisionTarget.mpHitSprite->mCarBcPtr, NULL);
                    }
                    else if (gCollisionTarget.mpHitSprite->AsObject2C())
                    {
                        PointArrowToEntity(NULL, NULL, gCollisionTarget.mpHitSprite->mObject2CPtr);
                    }
                }
            }
            else
            {
                ClearGlobalArrow();
            }
            ImGui::TreePop();
        }
        
        if (gParticle_8 && ImGui::TreeNode("ParticleManager"))
        {
            pPlayerSprite = GetPlayerSprite();

            static Fix16_Point particle_velocity = Fix16_Point(Fix16(0), Fix16(0));
            static Fix16 speed = Fix16(0);
            static Fix16 speed_z = Fix16(0);
            static s32 mVar_speed = 0;
            static s32 mVar_speed_z = 0;
            static s32 particle_type = 1;

            static s32 counter = 100;
            static s32 timer = 0;
            static s32 unused_34 = 0;
            static s32 initial_counter = 0;
            
            ImGui::InputInt("Type (1 - 44)", &particle_type, 1, 1);

            ImGui::SliderInt("Speed", &mVar_speed, 0, 0x4000 * 5);
            speed = Fix16(mVar_speed, 0);

            ImGui::SliderInt("Speed z", &mVar_speed_z, 0, 0x4000 * 5);
            speed_z = Fix16(mVar_speed_z, 0);

            ImGui::SliderInt("Counter", &counter, 0, 600);
            ImGui::SliderInt("Timer", &timer, 0, 10);
            ImGui::SliderInt("field_34_unused", &unused_34, 0, 1);
            ImGui::SliderInt("field_2E_initial_counter", &initial_counter, 0, 1000);

            if (ImGui::Button("Create Particle"))
            {
                particle_velocity.FromPolar_41E210(speed, pPlayerSprite->mRotation);
                Particle_4C* pParticle = gParticle_8->New(particle_velocity.x, particle_velocity.y, speed_z, 0, 0, 0);

                if (pParticle)
                {
                    pParticle->mUnused34 = unused_34;
                    pParticle->mState = particle_type;
                    pParticle->mCounter = counter;
                    pParticle->mInitialCounter = initial_counter;

                    pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                    pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 132);
                    pParticle->mSubState = 0;
                    pParticle->mTimer = timer;
                    pParticle->mpOwnSprite->set_xyz_lazy_420600(pPlayerSprite->mXy.x, pPlayerSprite->mXy.y, pPlayerSprite->mZpos);
                    gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
                }
            }
            

            if (ImGui::Button("Many Particles test"))
            {
                Fix16 xOff;
                xOff.FromInt(1);

                // From player when stood idle
                gParticle_8->SpawnCigaretteSmokePuff(pPlayerSprite, 1);
                gParticle_8->SpawnCigaretteSmokePuff(pPlayerSprite, 0);

                // Drowning peds/cars in water etc
                gParticle_8->EmitWaterSplash(pPlayerSprite->mXy.x,
                                                           pPlayerSprite->mXy.y,
                                                           pPlayerSprite->mZpos,
                                                           0,
                                                           false);

                gParticle_8->GunMuzzelFlash(pPlayerSprite);

                // The electro gun's arc segments (DrawElectricArc)
                gParticle_8->EmitElectricArcParticle(pPlayerSprite->mXy.x,
                                                            pPlayerSprite->mXy.y,
                                                            pPlayerSprite->mZpos,
                                                            0);

                gParticle_8->EmitFireTruckSprayParticle(pPlayerSprite);
                gParticle_8->EmitFlameStreamSegment(pPlayerSprite);

                // When being shot etc
                gParticle_8->EmitBloodBurst(pPlayerSprite->mXy.x,
                                                          pPlayerSprite->mXy.y,
                                                          pPlayerSprite->mZpos,
                                                          0);

                // Like when a car crashes
                gParticle_8->EmitImpactParticles(pPlayerSprite->mXy.x,
                                                               pPlayerSprite->mXy.y,
                                                               pPlayerSprite->mZpos,
                                                               Fix16_Point(0, 0));
            }
            ImGui::TreePop();
        }
    } // end gGame_0x40 != NULL

    if (ImGui::TreeNode("Debug options"))
    {
        ImGui::Checkbox("Debug Keys", &bDo_debug_keys);
        ImGui::Checkbox("Iain Test", &bDo_iain_test);
        ImGui::Checkbox("Brian Test", &bDo_brian_test);
        ImGui::Checkbox("Do Test", &bDo_test);
        ImGui::Checkbox("Do Mike", &bDo_mike);
        ImGui::Checkbox("Do Text ID Test", &gDo_text_id_test); // press '<' or '>' to navigate through gxt text id's  (Hud_BriefSelector stuff)
        ImGui::Checkbox("3D Sound", &bDo_3d_sound); // seems to not work

        if (ImGui::TreeNode("Skip stuff"))
        {
            ImGui::Checkbox("Skip Missions", &bSkip_mission);
            ImGui::Checkbox("Skip Particles", &bSkip_particles);
            ImGui::Checkbox("Skip Ambulance", &bSkip_ambulance);
            ImGui::Checkbox("Skip Explosions", &bExplodingScoresOff);
            ImGui::Checkbox("Skip Police", &bSkip_police);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Show things"))
        {
            ImGui::Checkbox("Show Ped IDs", &bDo_show_ids);
            ImGui::Checkbox("Show Objects IDs", &bDo_show_object_ids);
            ImGui::Checkbox("Show Junctions IDs", &bDo_show_junc_ids);
            ImGui::Checkbox("Show Traffic lights info", &bDo_show_traffic_lights_info);
            ImGui::Checkbox("Show Imaginary", &bDo_show_imaginary); // some cones appears when the player is hit by rocket launcher
            ImGui::Checkbox("Show Camera", &bDo_show_camera);
            ImGui::Checkbox("Show Physics", &bDo_show_physics);
            ImGui::Checkbox("Show Instruments", &bDo_show_instruments);
            ImGui::Checkbox("Show Cycle", &gShow_cycle);
            ImGui::Checkbox("Show Timing", &bDo_show_timing);
            ImGui::Checkbox("Show Counter", &gDo_show_counter);
            ImGui::Checkbox("Show Brief number", &bShow_brief_number);
            ImGui::Checkbox("Show Horn", &bDo_show_horn);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Cheats"))
        {
            ImGui::Checkbox("Invulnerable", &bDo_invulnerable);
            ImGui::Checkbox("Get all weapons", &bGet_all_weapons);
            ImGui::Checkbox("Keep weapons after death", &bKeep_weapons_after_death);
            ImGui::Checkbox("Free Shopping", &bDo_free_shopping);
            ImGui::Checkbox("Do Blood", &bDo_blood);
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    if (gFrontend && ImGui::TreeNode("Frontend"))
    {
        if (ImGui::TreeNode("MenuPage"))
        {
            static s32 page_idx = 0;
            ImGui::SliderInt("Menu page", &page_idx, 0, 16);

            MenuPage* pMenuPage = &gFrontend->mMenuPagesArray[page_idx];
            ImGui::Value("field_0_number_of_options", pMenuPage->mNumberOfOptions);
            ImGui::Value("field_2_number_of_elements", pMenuPage->mNumberOfElements);
            ImGui::Value("field_BC6_current_option_idx", pMenuPage->mCurrentOptionIdx);
            ImGui::Value("field_BC8_default_option_idx", pMenuPage->mDefaultOptionIdx);

            if (ImGui::TreeNode("MenuItem"))
            {
                static s32 option_entry_idx = 0;
                ImGui::SliderInt("MenuItem idx", &option_entry_idx, 0, 9);

                MenuItem* pOptionEntry = &pMenuPage->mOptionsArray[option_entry_idx];

                ImGui::Value("field_0_option_type", pOptionEntry->mOptionType);
                ImGui::Value("field_1_is_unlocked", pOptionEntry->mIsUnlocked);

                ImGui::SliderS16("field_2_x_pos", &pOptionEntry->mXPos, 0, 1000);
                ImGui::SliderS16("field_4_y_pos", &pOptionEntry->mYPos, 0, 700);

                static char str_buf[50];
                wchar_to_char(pOptionEntry->mOptionNameStr, str_buf, 50);
                ImGui::Text(str_buf);

                ImGui::Value("field_6A_font_type", pOptionEntry->mFontType);
                ImGui::Value("field_6C_palette", pOptionEntry->mPalette);
                ImGui::Value("field_6E_horizontal_selected_idx", pOptionEntry->mHorizontalSelectedIdx);
                ImGui::Value("field_70_unused", pOptionEntry->mUnused70);

                ImGui::Text(pOptionEntry->mHorizontalIdxEnabled);

                ImGui::Value("field_7E_horizontal_max_idx", pOptionEntry->mHorizontalMaxIdx);
                ImGui::Value("field_80_menu_page_target", pOptionEntry->mMenuPageTarget);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("MenuElement"))
            {
                static s32 element_idx = 0;
                ImGui::SliderInt("Competent Noyce ID", &element_idx, 0, 14);

                MenuElement* pElement = &pMenuPage->mElementsArray[element_idx];

                ImGui::Value("field_0_element_type", pElement->mElementType);
                ImGui::Value("field_1_is_it_displayed", pElement->mIsItDisplayed);

                ImGui::SliderS16("field_2_xpos", &pElement->mXpos, 0, 1000);
                ImGui::SliderS16("field_4_ypos", &pElement->mYpos, 0, 700);

                static char str_buf_2[50];
                wchar_to_char(pElement->mElementNameStr, str_buf_2, 50);
                ImGui::Text(str_buf_2);

                ImGui::InputU16("field_6A_font_type", &pElement->mFontType, 1, 1);
                ImGui::InputU16("field_6C_font_palette", &pElement->mFontPalette, 1, 1);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("MenuOption"))
            {
                static s32 option_idx = 0;
                ImGui::SliderInt("Kind Beaver ID", &option_idx, 0, 9);

                MenuOption* menu_option = &pMenuPage->mOptionStates[option_idx];

                ImGui::SliderS16("field_0_unused", &menu_option->mUnused0, 0, 1000);
                ImGui::SliderS16("field_2_unused", &menu_option->mUnused2, 0, 700);

                ImGui::Input_char_type("field_4_is_option_unlocked", &menu_option->mIsOptionUnlocked, 1, 1);
                ImGui::Input_char_type("field_5_unused", &menu_option->mUnused5, 1, 1);

                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("SaveSlotSummary"))
        {
            static s32 slot_idx = 0;
            ImGui::SliderInt("Save slot", &slot_idx, 0, 7);

            SaveSlotSummary* pSlot = &gFrontend->mPlySlots[slot_idx];

            ImGui::Value("field_0_save_exists", pSlot->mSaveExists);
            ImGui::Value("field_1_last_saved_stage", pSlot->mLastSavedStage);
            ImGui::Value("field_2_last_saved_bonus_stage_code", pSlot->mLastSavedBonusStageCode);
            ImGui::Value("field_3_last_saved_is_bonus", pSlot->mLastSavedIsBonus);
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNode("Special settings"))
        {
            if (ImGui::Button("Load multiplayer maps"))
            {
                LoadMapsFromData(multiplayer_maps, total_num_maps_loaded);
                bExtMapsLoaded = true;
            }

            if (bExtMapsLoaded)
            {
                ImGui::Value("Total loaded maps", total_num_maps_loaded);
                static s32 currentMapIndex = 0;
                if (ImGui::Combo("Map", &currentMapIndex, static_cast<const char*>(flat_char_array(reinterpret_cast<char*>(mmp_maps_description), total_num_maps_loaded)), total_num_maps_loaded))
                {
                    // Map selection changed
                }
                if (ImGui::Button("Load external map"))
                {
                    BootMap(multiplayer_maps[currentMapIndex].mMapName,
                        multiplayer_maps[currentMapIndex].mStyleName,
                        multiplayer_maps[currentMapIndex].mScriptName
                    );
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Debug Frontend"))
        {
            
            if (ImGui::Button("Add new menu option (WIP)"))
            {
                AddMenuOption(MENUPAGE_PLAY, 300, 415, L"TEST OPTION", 264);
            }

            if (ImGui::TreeNode("Player settings"))
            {
                static s32 player_idx;
                ImGui::SliderInt("Player idx", &player_idx, 0, 7);

                // fix softlock crashes after trying to change player name in standalone
                if (ImGui::Button("Set Player"))
                {
                    gGameSession.SetPlySlotIdx(player_idx);
                    gFrontend->UpdateMenuForCurrPlayer();
                    gRegistry.Set_Player_Setting("plyrslot", player_idx);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Fields"))
            {
                ImGui::SliderInt("Frontend State", &gFrontend->mState, 1, 5);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Credits"))
            {

                CreditsText* pCreditsStruct = &gFrontend->mCredits;
                if (pCreditsStruct)
                {
                    ImGui::InputS16("Credits line count", &pCreditsStruct->mLineCount, 1, 1);

                    static s32 credit_line;
                    ImGui::InputInt("Credit line", &credit_line, 1, 1);

                    CreditsLine* pCreditLine = &pCreditsStruct->mLines[credit_line];
                    if (pCreditLine)
                    {
                        ImGui::Checkbox("Credit line loaded", &pCreditLine->mbLoaded);
                        ImGui::InputS16("Credit line field_2_unused", &pCreditLine->mUnused2, 1, 1);
                        ImGui::InputU16("Credit line y gap", &pCreditLine->mYGap, 1, 1);
                        ImGui::InputU16("Credit line category", &pCreditLine->mStringCategory, 1, 1);
                    }
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    if (gPlyDat && ImGui::TreeNode("gPlyDat_6FEAC0"))
    {
        if (ImGui::TreeNode("BestStageStats"))
        {
            static s32 best_stats_idx = 0;
            ImGui::SliderInt("BestStageStats ID", &best_stats_idx, 0, 2);
            BestStageStats* pBestStats = &gPlyDat->mBestStats[best_stats_idx];

            static s32 stat_idx = 0;
            ImGui::SliderInt("Byte ID", &stat_idx, 0, 39);
            ImGui::Value("field_0_best_statistics at id", pBestStats->mBestStatistics[stat_idx]);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("HighScoreTable"))
        {
            static s32 table_type = 0;

            const char* table_names[] = {"field_1890_stage_scores", "field_23D0_total_scores", "field_24C0_alt_scores", "field_25B0_merged_scores"};
            ImGui::Combo("table_field", &table_type, table_names, 4);

            static s32 main_stage_idx = 0;
            static s32 stage_idx = 0;
            ImGui::SliderInt("field_1890_stage_scores ID 1", &main_stage_idx, 0, 2);
            ImGui::SliderInt("field_1890_stage_scores ID 2", &stage_idx, 0, 3);

            HighScoreTable* pTable;

            switch (table_type)
            {
                case 0:
                    pTable = &gPlyDat->mStageScores[main_stage_idx][stage_idx];
                    break;
                case 1:
                    pTable = &gPlyDat->mTotalScores;
                    break;
                case 2:
                    pTable = &gPlyDat->mAltScores;
                    break;
                default:
                    pTable = &gPlyDat->mMergedScores;
                    break;
            }

            static s32 string_id = 0;
            ImGui::SliderInt("string_id", &string_id, 0, 9);
            ScoreTableLine* pLine = &pTable->mScoreTableLine[string_id];

            static char str_buf_3[10];
            wchar_to_char(pLine->mPlayerName, str_buf_3, 10);
            ImGui::Text(str_buf_3);
            ImGui::Value("field_14_score", pLine->mScore);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("PlySlot"))
        {
            static s32 table_idx = 0;
            ImGui::SliderInt("table_idx", &table_idx, 0, 7);
            PlySlot* pPlySlot = &gPlyDat->mPlyrStats[table_idx];

            static char str_buf_4[9];
            wchar_to_char(pPlySlot->mstrPlayerName, str_buf_4, 9);
            ImGui::Text(str_buf_4);

            static s32 main_stage_idx = 0;
            static s32 stage_idx = 0;
            ImGui::SliderInt("StageStats ID 1", &main_stage_idx, 0, 2);
            ImGui::SliderInt("StageStats ID 2", &stage_idx, 0, 3);
            StageStats* pStageStats = &pPlySlot->mPlyrStageStats[main_stage_idx][stage_idx];

            ImGui::Value("field_0_is_stage_unlocked", pStageStats->mIsStageUnlocked);
            ImGui::Value("field_4_stage_best_score", pStageStats->mStageBestScore);
            ImGui::Value("field_8_stage_latest_score", pStageStats->mStageLatestScore);

            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Global structs & vars"))
    {
        if (ImGui::TreeNode("Slope struct 0x6F5BA8"))
        {
            static u8 slope_idx = 0;
            ImGui::SliderU8("slope_idx", &slope_idx, 0, 63);
            gmp_map_slope* slope_struct = &gGmpSlopes[slope_idx];
            if (slope_struct)
            {
                ImGui::Value("field_0_gradient_direction", slope_struct->mGradientDirection);
                ImGui::Value("field_1_gradient_size", slope_struct->mGradientSize);
                ImGui::Value("field_2_gradient_level", slope_struct->mGradientLevel);
                ImGui::Value("field_3_padding", slope_struct->mPadding3);
                ImGui::Value("field_4_zpos_lower", slope_struct->mZposLower.ToFloat());
                ImGui::Value("field_8_zpos_higher", slope_struct->mZposHigher.ToFloat());
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Car RNG arrays"))
        {
            static u8 idx = 0;
            ImGui::SliderU8("idx", &idx, 0, 255);

            ImGui::Value("dword_676DB8", gAverageCarModels.values[idx]);
            ImGui::Value("dword_67698C", gBadCarModels.values[idx]);
            ImGui::Value("dword_677388", gGoodCarModels.values[idx]);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("gMapObjectOverrides_6FC5F8"))
        {
            static s32 override_idx = 0;
            ImGui::SliderInt("MapObjectOverride idx", &override_idx, 0, 23);

            if (gMapObjectOverrides)
            {
                MapObjectOverride* pOverride = &gMapObjectOverrides[override_idx];
                if (pOverride)
                {
                    ImGui::Value("field_0_definition_idx", pOverride->mDefinitionIdx);
                    ImGui::Value("field_4_behavior_type", pOverride->mBehaviorType);
                    ImGui::Value("field_8_next_definition_idx", pOverride->mNextDefinitionIdx);
                    ImGui::Value("field_C_in_pool_list", pOverride->mInPoolList);
                    ImGui::Value("field_10_touch_behavior", pOverride->mTouchBehavior);
                    ImGui::Value("field_14_projectile_hit_type", pOverride->mProjectileHitType);
                    ImGui::Value("field_18_collision_bucket_category", pOverride->mCollisionBucketCategory);
                    ImGui::Value("field_1C_speed", pOverride->mSpeed.mValue);
                    ImGui::Value("field_20_friction", pOverride->mFriction.mValue);
                    ImGui::Value("field_24_next_definition_timer", pOverride->mNextDefinitionTimer);
                    ImGui::Value("field_28_collision_response", pOverride->mCollisionResponse);
                    ImGui::Value("field_2C_z_collision_response", pOverride->mZCollisionResponse);
                    ImGui::Value("field_30_next_frame_max", pOverride->mNextFrameMax);
                    ImGui::Value("field_34_uses_3d_movement", pOverride->mUses3dMovement);
                    ImGui::Value("field_38_mass", pOverride->mMass.mValue);
                    ImGui::Value("field_3C_draw_layer", pOverride->mDrawLayer);
                    ImGui::Value("field_40_sprite_flags", pOverride->mSpriteFlags);
                    ImGui::Value("field_44_has_sound", pOverride->mHasSound);
                    ImGui::Value("field_48_has_shadows", pOverride->mHasShadows);
                    ImGui::Value("field_4C", pOverride->mInstanceLimitKind);
                    ImGui::Value("field_50", pOverride->mMakesCollisionSound);
                }
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    ImGui::End(); // ImGui::Begin("Debugger");
}