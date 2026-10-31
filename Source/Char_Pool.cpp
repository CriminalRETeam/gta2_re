#include "Char_Pool.hpp"
#include "ped_group_type.hpp"
#include "ped_class.hpp"
#include "ped_fire_mode.hpp"
#include "Car.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Hud.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Ped_List.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "SpriteGrid.hpp"
#include "Weapon.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "rng.hpp"

#include "Frontend.hpp" // For the cheat bools

#include <DINPUT.H>
#include "ped_graphic_type.hpp"

// Defined in Ped.cpp
EXTERN_GLOBAL(s16, gPedOffscreenDespawnTime_678656);
EXTERN_GLOBAL(Ang16, gSearchConeHalfAngle_6784F0);
EXTERN_GLOBAL(u8, gRunPedObjectiveAI_61A8A0);
EXTERN_GLOBAL(s8, gObjectiveMayAct_61A8A3);
EXTERN_GLOBAL(u8, gRunPedThink_61A8A4);
EXTERN_GLOBAL(Fix16, kFpZero_678660);
EXTERN_GLOBAL(Fix16, gDistanceToTarget_678750);
EXTERN_GLOBAL(Ped*, gLastProcessedPed_6787C0);
EXTERN_GLOBAL(u8, gNumberArmedGangMembers_6787CE);
EXTERN_GLOBAL(s16, gNumPedsCrossingRoad_6787D0);
EXTERN_GLOBAL(u8, gNewTaxiCustomersThisTick_6787D2);
EXTERN_GLOBAL(char_type, gNumberBusCustomers_6787D3);
EXTERN_GLOBAL(u8, gCloseRangeHit_6787D4);
DEFINE_GLOBAL(u8, gTargetSearchMode_6787D7, 0x6787D7);
EXTERN_GLOBAL(u8, gPathStepRoundDone_6787D8);
EXTERN_GLOBAL(u8, gLosCheckRoundDone_6787D9);
EXTERN_GLOBAL(Ped*, gSearchingPed_6787DC);
EXTERN_GLOBAL(u16, gNumPedsUpdated_6787E0);
EXTERN_GLOBAL(u8, gNumDummyChars_6787E2);
EXTERN_GLOBAL(u8, gNumScriptCreatedPeds_6787E3);

// Initial value not verified (assumed to be gPedOffscreenDespawnTime_678656's default, which PedManager resets from it)
DEFINE_GLOBAL_INIT(s16, kDefaultPedOffscreenDespawnTime_61A898, 40, 0x61A898);
DEFINE_GLOBAL(PedManager*, gPedManager_6787BC, 0x6787BC);
DEFINE_GLOBAL(PedPool*, gPedPool_6787B8, 0x6787B8);
DEFINE_GLOBAL(CharPool*, gChar_B4_Pool_6FDB44, 0x6FDB44);
DEFINE_GLOBAL(PedListNodePool*, gChar_8_Pool_678b50, 0x678b50);

DEFINE_STATIC_MEMBER(u16, PedManager, sPedsServiceTickCount, 0x6787F0);
DEFINE_GLOBAL_INIT(u8, gPathStepSlotFree_61A8A1, 1, 0x61A8A1);
DEFINE_GLOBAL_INIT(u8, gLosCheckSlotFree_61A8A2, 1, 0x61A8A2);
DEFINE_GLOBAL(u8, gNumPolicePedsInRangeScreen_6787EE, 0x6787EE);
DEFINE_GLOBAL(u8, bThreateningPedAdded_6787EF, 0x6787EF);
DEFINE_GLOBAL(u8, bHaveThreateningPeds_6787DA, 0x6787DA);

DEFINE_STATIC_MEMBER(u8, PedManager, sSpawnSideLocked, 0x6787D5);
DEFINE_STATIC_MEMBER(u8, PedManager, sSpawnCountLimit, 0x6787D6);
DEFINE_STATIC_GLOBAL_INIT(Ang16, sCameraFacingAng, Ang16(0), 0x678760);
DEFINE_STATIC_MEMBER(s8, PedManager, sSpawnSide, 0x6787C8);
DEFINE_STATIC_MEMBER(s8, PedManager, sSpawnIndex, 0x6787C9);

EXTERN_GLOBAL(Fix16, gDummyW_678530);
EXTERN_GLOBAL(Fix16, gDummyZ_67841C);

DEFINE_STATIC_GLOBAL_INIT(Fix16, sDummyH, Fix16(0xCCC, 0), 0x678584); // same as gDummyW_678530, which Ped.cpp initializes too late
DEFINE_GLOBAL_INIT(Fix16, kFp255_678414, Fix16(255), 0x678414);

EXTERN_GLOBAL(u8, gNumEmergencyPeds_6787E4);

EXTERN_GLOBAL(Fix16, kFpZero_678438);
EXTERN_GLOBAL(Fix16, kFpHalf_67853C);
EXTERN_GLOBAL(Ang16, gDummyPedAng_6787A8);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

EXTERN_GLOBAL(Fix16, gSpawnJitterScale_678618);
EXTERN_GLOBAL(Fix16, kFpOne_678664);

EXTERN_GLOBAL(Fix16, kFpPoint8_6784A0);
EXTERN_GLOBAL(Fix16, kFpPoint1_678480);

DEFINE_GLOBAL_INIT(Ang16, gSpawnRotationLeft_6786E0, Ang16(360), 0x6786E0);
DEFINE_GLOBAL_INIT(Ang16, gSpawnRotationTop_6787B0, Ang16(0), 0x6787B0);
DEFINE_GLOBAL_INIT(Ang16, gSpawnRotationRight_678578, Ang16(1080), 0x678578);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6785A6, Ang16(0x2D0), 0x6785A6);
DEFINE_GLOBAL_INIT(Ang16, gSpawnRotationBottom_678540, Ang16(720), 0x678540);
DEFINE_STATIC_GLOBAL_INIT(s16, sSpawnCounter, 0, 0x6787C6);

EXPORT Ped* __stdcall SpawnPedChainGroupAt_46DB90(char_type remap, u8 number_followers, Fix16 xpos, Fix16 ypos, Fix16 zpos);

MATCH_FUNC(0x46DB90)
EXPORT Ped* __stdcall SpawnPedChainGroupAt_46DB90(char_type remap, u8 number_followers, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Fix16 xpos_adjusted = kFpPoint8_6784A0 + Fix16(static_cast<u8>(xpos.ToInt()));
    Fix16 ypos_adjusted = kFpPoint8_6784A0 + Fix16(static_cast<u8>(ypos.ToInt()));

    Ped* pLeader = gPedPool_6787B8->Allocate();
    pLeader->mOccupation = ped_ocupation_enum::elvis_leader;
    pLeader->mRemap = remap;
    pLeader->mGraphicType = ped_graphic_type::character_1;
    pLeader->mPedType = ped_type::special_ped_4;
    pLeader->AllocCharB4_45C830(xpos_adjusted, ypos_adjusted, zpos);
    pLeader->mGameObject->SetRemap_46DD50(pLeader->mRemap);
    pLeader->mHealth = 100;

    PedGroup* pGroup = PedGroup::New_4CB0D0();
    pGroup->add_ped_leader_4C9B10(pLeader);
    pGroup->mGroupType = ped_group_type::chain_1;
    pGroup->mCount36 = number_followers;
    pGroup->mCount34 = number_followers;

    for (u8 ped_idx = 0; ped_idx < number_followers; ped_idx++)
    {
        Fix16 xy_off = Fix16(0x4000 * (ped_idx + 1), 0);

        Ped* pNewPed = gPedPool_6787B8->Allocate();

        pNewPed->mOccupation = ped_ocupation_enum::elvis;
        pNewPed->mRemap = remap;
        pNewPed->mPedType = ped_type::special_ped_4;
        pNewPed->AllocCharB4_45C830(xpos_adjusted - ((kFpPoint1_678480 * xy_off)), ypos_adjusted - ((kFpPoint1_678480 * xy_off)), zpos);

        pNewPed->SetRemap_433C10(pNewPed->get_remap_433BA0());

        pNewPed->mHealth = 100;
        pNewPed->mGraphicType = ped_graphic_type::character_1;
        pGroup->add_ped_to_list_4C9B30(pNewPed, ped_idx);
    }
    return pLeader;
}

// Not in 9.6f (0x440CC0 calls Fix16::multiply_401BD0 directly), but written inline the product
// lands in ebx directly instead of through eax like 10.5.
static inline Fix16 GetSpawnJitter()
{
    return gSpawnJitterScale_678618 * (gRng_6F6784.get_int_4F7AE0(32) + 8);
}

// The kind of ped SpawnPedestrianAt_46E380 picks from the zone's ratios
enum
{
    k_spawn_dummy = 0,
    k_spawn_mugger = 1,
    k_spawn_car_thief = 2,
    k_spawn_elvis = 3,
    k_spawn_gang_member = 4,
    k_spawn_cop = 5,
};

MATCH_FUNC(0x46E380)
EXPORT void __stdcall SpawnPedestrianAt_46E380(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    u8 kind = k_spawn_dummy;
    s16 rng_val = gRng_6F6784.get_int_4F7AE0(1000);

    Ped* pPed = gPedPool_6787B8->Allocate();

    ++sSpawnCounter;

    gmp_zone_info* pZone = gMap_0x370_6F6268->GetZoneInfoAt_4DF890(xpos.ToInt(), ypos.ToInt());

    if (rng_val < static_cast<u16>(pZone->mMuggerRatio))
    {
        kind = k_spawn_mugger;
    }
    else if (rng_val < static_cast<u16>(pZone->mMuggerRatio) + static_cast<u16>(pZone->mCarthiefRatio))
    {
        kind = k_spawn_car_thief;
    }
    else if (rng_val < static_cast<u16>(pZone->mMuggerRatio) + static_cast<u16>(pZone->mCarthiefRatio) + static_cast<u16>(pZone->mElvisRatio))
    {
        // 1 in 50 chance of elvis
        kind = gRng_6F6784.get_int_4F7AE0(50) == 25 ? k_spawn_elvis : k_spawn_dummy;
    }
    else if (rng_val < static_cast<u16>(pZone->mMuggerRatio) + static_cast<u16>(pZone->mCarthiefRatio) + static_cast<u16>(pZone->mElvisRatio) +
                 static_cast<u16>(pZone->mGangcharRatio))
    {
        // Gang member, limited to 8
        kind = static_cast<u8>(gNumberArmedGangMembers_6787CE) < 8u ? k_spawn_gang_member : k_spawn_dummy;
    }
    else if (rng_val < static_cast<u16>(pZone->mMuggerRatio) + static_cast<u16>(pZone->mCarthiefRatio) + static_cast<u16>(pZone->mElvisRatio) +
                 static_cast<u16>(pZone->mGangcharRatio) + static_cast<u16>(pZone->mPolicepedRatio))
    {
        kind = k_spawn_cop;
    }

    if (gCheatOnlyElvisPeds_67D4ED)
    {
        kind = k_spawn_elvis;
    }

    if (bNo_annoying_chars_67D586)
    {
        if (kind != k_spawn_cop)
        {
            kind = k_spawn_dummy;
        }
    }

    if (pPed)
    {
        pPed->ChangeNextPedState1_45C500(ped_state_1::walking_0);
        pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        pPed->SetPedType_403920(ped_type::dummy_3);
        pPed->SetOffscreenCounter_433B80(1);
        pPed->set_remap_433B90(ped_remap_enum::ped_remap_yellow_police);

        switch (kind)
        {
            case k_spawn_mugger:
                if (gNumberMuggersSpawned_6787CA)
                {
                    goto delloc_ret_2;
                }
                gNumberMuggersSpawned_6787CA = 1;
                pPed->set_occupation_403970(ped_ocupation_enum::mugger);
                pPed->SetPedClass_433BC0(ped_class::mugger_or_car_thief_2);
                pPed->set_objective_timer_433C80(40);
                pPed->SetPedType_403920(ped_type::special_ped_4);
                pPed->set_remap_433B90(ped_remap_enum::ped_remap_mugger);
                pPed->mThreatSearch = threat_search_enum::area_2;
                pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                pPed->mGraphicType = ped_graphic_type::civilian_0;
                break;

            case k_spawn_car_thief:
                if (gNumberCarThiefsSpawned_6787CB)
                {
                    goto delloc_ret_2;
                }
                gNumberCarThiefsSpawned_6787CB = 1;
                pPed->SetPedClass_433BC0(ped_class::mugger_or_car_thief_2);
                pPed->mThreatSearch = threat_search_enum::area_2;
                pPed->set_objective_timer_433C80(40);
                pPed->set_occupation_403970(ped_ocupation_enum::car_thief);
                pPed->SetPedType_403920(ped_type::dummy_with_occupation_6);
                pPed->set_remap_433B90(ped_remap_enum::ped_remap_green_shorts);
                pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                pPed->mGraphicType = ped_graphic_type::civilian_0;
                pPed->mBf21C.bDriveAggressively = true;
                pPed->mRunSpeed = kFpFour_678670;
                break;

            case k_spawn_elvis:
                if (gNumberElvisLeadersSpawned_6787CC)
                {
                    goto delloc_ret_2;
                }
                SpawnPedChainGroupAt_46DB90(12, 5u, xpos, ypos, zpos);
                pPed->Deallocate_45EB60();
                ++gNumberElvisLeadersSpawned_6787CC;
                break;

            case k_spawn_gang_member:
            {
                gmp_map_zone* pMapZone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(xpos.ToInt(), ypos.ToInt(), 14u);
                if (pMapZone)
                {
                    Gang* pGang = gGangPool_CA8_67E274->FindGangByName_4BF100(pMapZone->mName);
                    if (static_cast<u8>(gNumberArmedGangMembers_6787CE) < 4u)
                    {
                        ++gNumberArmedGangMembers_6787CE;
                        pPed->SetPedType_403920(ped_type::special_ped_4);
                        pPed->set_occupation_403970(ped_ocupation_enum::armed_gang_member_19);
                        pPed->mpGang = pGang;
                        pPed->set_remap_433B90(pGang->mRemap);
                        if (pPed->get_remap_433BA0() == ped_remap_enum::ped_remap_red_head_redneck)
                        {
                            if (!gRng_6F6784.get_int_4F7AE0(2))
                            {
                                pPed->set_remap_433B90(ped_remap_enum::ped_remap_blond_head_redneck);
                            }
                        }
                        pPed->mGraphicType = ped_graphic_type::character_1;
                        pPed->SetPedClass_433BC0(ped_class::gang_member_1);
                        pPed->ForceWeapon_46F600(pPed->mpGang->GetCurrentWeapon_4BF0C0());
                        if (pPed->mSelectedWeapon && pPed->mSelectedWeapon->mWeaponType)
                        {
                            pPed->mBf21C.bArmedGangMember = true;
                            pGang->mAlwaysBasicWeapon = true;
                        }
                        pPed->GiveWeapon_46F650(weapon_type::pistol);
                        pPed->mFireMode = ped_fire_mode::random_shots_0;
                        pPed->mThreatSearch = threat_search_enum::line_of_sight_1;
                        pPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;
                    }
                    else
                    {
                        pPed->mDummyGang = pGang;
                        pPed->set_occupation_403970(ped_ocupation_enum::dummy);
                        pPed->SetPedClass_433BC0(ped_class::gang_dummy_0);
                        pPed->SetPedType_403920(ped_type::dummy_3);
                        pPed->mThreatSearch = threat_search_enum::area_2;
                        pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                        pPed->set_remap_433B90(pGang->mRemap);
                        pPed->mGraphicType = ped_graphic_type::character_1;
                        if (pPed->get_remap_433BA0() == ped_remap_enum::ped_remap_red_head_redneck)
                        {
                            if (!gRng_6F6784.get_int_4F7AE0(2))
                            {
                                pPed->set_remap_433B90(ped_remap_enum::ped_remap_blond_head_redneck);
                            }
                        }
                    }
                }
                else
                {
                    pPed->set_occupation_403970(ped_ocupation_enum::dummy);
                    pPed->SetPedClass_433BC0(ped_class::gang_dummy_0);
                    pPed->SetPedType_403920(ped_type::dummy_3);
                    pPed->mThreatSearch = threat_search_enum::area_2;
                    pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                    pPed->mGraphicType = ped_graphic_type::civilian_0;
                    switch (gRng_6F6784.get_int_4F7AE0(4))
                    {
                        case 0:
                            pPed->set_remap_433B90(ped_remap_enum::ped_remap_blue_dummy);
                            break;

                        case 1:
                            pPed->set_remap_433B90(ped_remap_enum::ped_remap_light_blue_dummy);
                            break;

                        case 2:
                            pPed->set_remap_433B90(ped_remap_enum::ped_remap_tshirt_and_shorts_dummy);
                            break;

                        default:
                            pPed->set_remap_433B90(ped_remap_enum::ped_remap_short_sleeve);
                            break;
                    }
                }
                break;
            }
            case k_spawn_cop:
                if (gNumberWalkingCopsSpawned_6787CD || bSkip_police_67D4F9 || gPolice_7B8_6FEE40->mHighestCrewTypeInPursuit == crew_type::army_6)
                {
                // shared exit for the four spawn limits: goto-free forms give each its own epilogue
                delloc_ret_2:
                    pPed->Deallocate_45EB60();
                    return;
                }
                gPolice_7B8_6FEE40->SpawnWalkingGuard_570320(pPed, xpos, ypos, zpos, rotation);
                gNumberWalkingCopsSpawned_6787CD = 1;
                pPed->mThreatSearch = threat_search_enum::line_of_sight_1;
                pPed->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
                switch (gPolice_7B8_6FEE40->mMaxWantedLevel)
                {
                    case cop_level_enum::police_2:
                        pPed->GiveWeapon_46F650(weapon_type::pistol);
                        pPed->set_health_4039A0(100);
                        pPed->mMaxSpeed = (kFpOneSixteenth_678448 * kFpPoint8_6784A0);
                        pPed->mGraphicType = ped_graphic_type::cop_2;
                        break;
                    case cop_level_enum::none_0:
                    case cop_level_enum::police_1:
                        pPed->mSelectedWeapon = NULL;
                        pPed->GiveWeapon_46F650(weapon_type::pistol);
                        pPed->set_health_4039A0(50);
                        pPed->mMaxSpeed = (kFpOneSixteenth_678448 * kFpPoint8_6784A0);
                        pPed->mGraphicType = ped_graphic_type::cop_2;
                        break;

                    default:
                        pPed->GiveWeapon_46F650(weapon_type::pistol);
                        pPed->set_health_4039A0(100);
                        pPed->mGraphicType = ped_graphic_type::cop_2;
                        break;
                } // End switch
                break;

            default:
                if (gPolice_7B8_6FEE40->mHighestCrewTypeInPursuit == crew_type::army_6)
                {
                    gPolice_7B8_6FEE40->SpawnWalkingGuard_570320(pPed, xpos, ypos, zpos, rotation);
                    pPed->mThreatSearch = threat_search_enum::line_of_sight_1;
                    pPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;
                    pPed->ForceWeapon_46F600(weapon_type::smg);
                    pPed->mGraphicType = ped_graphic_type::cop_2;
                }
                else
                {
                    pPed->set_occupation_403970(ped_ocupation_enum::dummy);
                    pPed->SetPedClass_433BC0(ped_class::gang_dummy_0);
                    pPed->SetPedType_403920(ped_type::dummy_3);
                    pPed->mThreatSearch = threat_search_enum::area_2;
                    pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                    pPed->mGraphicType = ped_graphic_type::civilian_0;

                    if (gCheatNakedPeds_67D5E8)
                    {
                        pPed->set_remap_433B90(ped_remap_enum::ped_remap_naked_dummy);
                    }
                    else
                    {
                        u8 remap = static_cast<u8>(gRng_6F6784.get_int_4F7AE0(25));
                        if (remap < 4u)
                        {
                            remap += 18;
                        }
                        else
                        {
                            remap += 27;
                        }
                        pPed->set_remap_433B90(remap);
                    }
                }
                break;
        } // End switch

        if (pPed->get_occupation_403980() != ped_ocupation_enum::walking_guard_29 &&
            pPed->get_occupation_403980() != ped_ocupation_enum::walking_army_guard_31)
        {
            pPed->AllocCharB4_45C830(xpos, ypos, zpos);
        }

        gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(pPed->mGameObject->mSpritePtr);

        if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(pPed->mGameObject->mSpritePtr, sprite_types_enum::none_0))
        {
            pPed->ChangeNextPedState1_45C500(ped_state_1::dead_9);
            pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_dead_15);
            pPed->Deallocate_45EB60();
            gSpriteGrid_1_679208->AddToRegionBuckets_477B20(pPed->mGameObject->mSpritePtr);
            return;
        }

        gSpriteGrid_1_679208->AddToRegionBuckets_477B20(pPed->mGameObject->mSpritePtr);
        if (pPed->get_occupation_403980() != ped_ocupation_enum::walking_guard_29)
        {
            pPed->SetRemap_433C10(pPed->get_remap_433BA0());
            pPed->SetRotation_433C00(rotation);
            pPed->ResetAnimAndFindNearestSprite_467280();
        }

        if (gPedManager_6787BC->mMakeAllMuggers)
        {
            if (!gRng_6F6784.get_int_4F7AE0(2))
            {
                if (pPed->get_occupation_403980() == ped_ocupation_enum::armed_gang_member_19)
                {
                    --gNumberArmedGangMembers_6787CE;
                }
                pPed->SetPedClass_433BC0(ped_class::mugger_or_car_thief_2);
                pPed->set_objective_timer_433C80(40);
                pPed->set_occupation_403970(ped_ocupation_enum::mad_mugger_40);
                pPed->SetPedType_403920(ped_type::special_ped_4);
                pPed->mThreatSearch = threat_search_enum::area_2;
                pPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;
            }
        }

    } // End if

    if (static_cast<u16>(sSpawnCounter) > 200u)
    {
        sSpawnCounter = 0;
    }
}

MATCH_FUNC(0x46eb60)
void PedManager::SpawnDummies_46EB60(Camera* pCam)
{
    Fix16 xpos;
    Fix16 ypos;
    s32 zpos;
    Ang16 rotation;

    Sprite* pSprite = this->mSpawnProbeSprite;

    s16 tileLeft = (pCam->mViewBounds.mLeft - kFpHalf_67853C).ToInt();
    s16 tileRight = (pCam->mViewBounds.mRight + kFpHalf_67853C).ToInt();
    s16 tileTop = (pCam->mViewBounds.mTop - kFpHalf_67853C).ToInt();
    s16 tileBottom = (pCam->mViewBounds.mBottom + kFpHalf_67853C).ToInt();

    s16 bound_max = 0;

    if (sSpawnSideLocked)
    {
        switch (Ang16::GetAngleFace_4F78F0(sCameraFacingAng))
        {
            case 1:
                sSpawnSide = 1;
                break;
            case 3:
                sSpawnSide = 2;
                break;
            case 2:
                sSpawnSide = 3;
                break;
            case 4:
                sSpawnSide = 0;
                break;
        }
    }

    for (u8 i = 0; i < sSpawnCountLimit; i++)
    {
        switch (sSpawnSide)
        {
            case 0:
            case 2:
                bound_max = tileBottom - tileTop;
                break;
            case 1:
            case 3:
                bound_max = tileRight - tileLeft;
                break;
        }

        if (sSpawnIndex > bound_max)
        {
            sSpawnIndex = 0;
            if (!sSpawnSideLocked)
            {
                if (++sSpawnSide > 3)
                {
                    sSpawnSide = 0;
                }
            }
        }

        switch (sSpawnSide)
        {
            case 0:
                xpos = Fix16(tileLeft);
                ypos = Fix16(tileTop + sSpawnIndex);
                rotation = gSpawnRotationLeft_6786E0;
                break;

            case 1:
                ypos = Fix16(tileTop);
                xpos = Fix16(tileLeft + sSpawnIndex);
                rotation = gSpawnRotationTop_6787B0;
                break;

            case 2:
                xpos = Fix16(tileRight);
                ypos = Fix16(tileTop + sSpawnIndex);
                rotation = gSpawnRotationRight_678578;
                break;

            case 3:
                ypos = Fix16(tileBottom);
                xpos = Fix16(tileLeft + sSpawnIndex);
                rotation = gSpawnRotationBottom_678540;
                break;
        }

        Fix16 jitterX = GetSpawnJitter();
        Fix16 jitterY = GetSpawnJitter();
        xpos += jitterX;
        ypos += jitterY;

        if (xpos > kFpOne_678664 && xpos < kFp255_678414 - kFpOne_678664 && ypos > kFpOne_678664 &&
            ypos < kFp255_678414 - kFpOne_678664)
        {
            gmp_zone_info* pZoneInfo = gMap_0x370_6F6268->GetZoneInfoAt_4DF890(xpos.ToInt(), ypos.ToInt());
            if (static_cast<u8>(mNumPedsOnScreen) < static_cast<u16>(pZoneInfo->mPedDensity) / 25)
            {
                if (gMap_0x370_6F6268->FindPavementBlockForCoord_4E4BB0(xpos.ToInt(), ypos.ToInt(), zpos))
                {
                    pSprite->set_xyz_lazy_451950(xpos, ypos, (zpos + 1));
                    pSprite->set_ang_lazy_420690(gDummyPedAng_6787A8);
                    pSprite->AllocCollisionShape_59F950(gDummyW_678530, sDummyH, gDummyZ_67841C);
                    if (!gGame_0x40_67E008->IsPointOnScreen_4B9A80(pSprite->mXy.x, pSprite->mXy.y))
                    {
                        if (!gMap_0x370_6F6268->IsSlopeB4ToD0At_433430(static_cast<u8>(xpos.ToInt()), static_cast<u8>(ypos.ToInt()), static_cast<u8>(zpos) + 1))
                        {
                            SpawnPedestrianAt_46E380(xpos, ypos, Fix16(zpos + 1), rotation);
                        }
                    }
                }
            }
        }
        ++sSpawnIndex;
    }
}

MATCH_FUNC(0x470330)
void PedManager::Dummies_470330()
{
    s16 max_dummies = gPedManager_6787BC->mMaxDummyChars;
    if (gPolice_7B8_6FEE40->mMaxWantedLevel > cop_level_enum::police_3)
    {
        max_dummies = static_cast<u16>(max_dummies) >> 1;
    }
    if (gNumDummyChars_6787E2 < max_dummies)
    {
        for (Camera* pCam = gGame_0x40_67E008->IteratePlayerCamera_4B9BC0(); pCam; pCam = gGame_0x40_67E008->IterateNextPlayerCamera_4B9C50())
        {
            sSpawnSideLocked = 0;
            if (pCam->HasFollowTarget_433E90())
            {
                if (pCam->ReturnOwnerVelocity_435A20() > kFpZero_678438)
                {
                    sSpawnSideLocked = 1;
                    sCameraFacingAng = pCam->ComputeTargetFacingAngle_4358D0();
                    sSpawnCountLimit = 1;
                }
                else
                {
                    sSpawnCountLimit = 2;
                }
            }
            // Use global instance even tho we're already in an instance method :)
            gPedManager_6787BC->SpawnDummies_46EB60(pCam);
        }
    }
}

// https://decomp.me/scratch/dQf8H
MATCH_FUNC(0x4703f0)
void PedManager::PedsService_4703F0()
{
    ++sPedsServiceTickCount;
    gNumPedsUpdated_6787E0 = 0;
    gNumDummyChars_6787E2 = 0;
    gNumEmergencyPeds_6787E4 = 0;
    gNumScriptCreatedPeds_6787E3 = 0;
    gNumPedsOnScreen_6787EC = 0;
    gPathStepSlotFree_61A8A1 = 1;
    gLosCheckSlotFree_61A8A2 = 1;
    gNewTaxiCustomersThisTick_6787D2 = 0;
    gNumPolicePedsInRangeScreen_6787EE = 0;

    gPedPool_6787B8->UpdatePool_445960();

    if (bThreateningPedAdded_6787EF)
    {
        bHaveThreateningPeds_6787DA = 1;
    }
    else
    {
        bHaveThreateningPeds_6787DA = 0;
        gThreateningPedsList_678468.ClearPeds_4712F0();
    }

    gPathStepRoundDone_6787D8 = gPathStepSlotFree_61A8A1 == 1;
    gLosCheckRoundDone_6787D9 = gLosCheckSlotFree_61A8A2 == 1;

    if (!bSkip_dummies_67D4EF)
    {
        Dummies_470330();
    }
    mNumPedsUpdated = static_cast<char_type>(gNumPedsUpdated_6787E0);
    mNumDummyChars = gNumDummyChars_6787E2;
    mNumScriptCreatedPeds = gNumScriptCreatedPeds_6787E3;
    mFbiArmyCount = gNumEmergencyPeds_6787E4;
    mNumPedsOnScreen = static_cast<char_type>(gNumPedsOnScreen_6787EC);
    if (gPolice_7B8_6FEE40)
    {
        gPolice_7B8_6FEE40->mPolicePedsInRangeScreen = gNumPolicePedsInRangeScreen_6787EE;
    }
    bThreateningPedAdded_6787EF = 0;
    if (bDo_iain_test_67D4E9)
    {
        // The original reads a dword here
        u16 num_peds = *reinterpret_cast<u32*>(&gNumPedsOnScreen_6787EC);
        swprintf(tmpBuff_67BD9C, L"num peds on screen : %d", num_peds);
        gHud_2B00_706620->mTexts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 64, gDebugFont_706600, 1);

        if (gPolice_7B8_6FEE40)
        {
            swprintf(tmpBuff_67BD9C,
                     L"num police peds in range screen : %d",
                     static_cast<u8>(gPolice_7B8_6FEE40->mPolicePedsInRangeScreen));
            gHud_2B00_706620->mTexts.DisplayText_5D1F50(tmpBuff_67BD9C, 0, 80, gDebugFont_706600, 1);
        }
    }
}

// https://decomp.me/scratch/P1OvR
MATCH_FUNC(0x470650)
PedManager::PedManager()
{
    mSpawnProbeSprite = NULL;
    if (!gPedPool_6787B8)
    {
        gPedPool_6787B8 = new PedPool();
        if (!gPedPool_6787B8)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15827);
        }
    }

    if (!gChar_B4_Pool_6FDB44)
    {
        gChar_B4_Pool_6FDB44 = new CharPool();
        if (!gChar_B4_Pool_6FDB44)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15834);
        }
    }

    if (!gChar_8_Pool_678b50)
    {
        gChar_8_Pool_678b50 = new PedListNodePool();
        if (!gChar_8_Pool_678b50)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15841);
        }
    }

    mSpawnProbeSprite = gSpritePool_703818->get_new_sprite();
    // Plain 0s: implicit Fix16 conversions, out of line (Fix16::FromInt_4369F0) once the inline budget runs out
    mSpawnProbeSprite->set_xyz_lazy_451950(0, 0, 0); // 9.6f inlined set_xyz_lazy_420600
    mSpawnProbeSprite->set_ang_lazy_420690(gDummyPedAng_6787A8);
    mSpawnProbeSprite->AllocCollisionShape_59F950(0, 0, 0);

    mNumDummyChars = 0;
    mNumPedsUpdated = 0;
    mNumScriptCreatedPeds = 0;
    mNumPedsOnScreen = 0;
    mMaxDummyChars = 50;
    mMakeAllMuggers = false;

    gPedId_61A89C = 7;
    gLastProcessedPed_6787C0 = NULL;
    sSpawnCounter = 0;
    sSpawnSide = 0;
    sSpawnIndex = 0;
    gNumberMuggersSpawned_6787CA = 0;
    gNumberCarThiefsSpawned_6787CB = 0;
    gNumberElvisLeadersSpawned_6787CC = 0;
    gNumberWalkingCopsSpawned_6787CD = 0;
    gNumberArmedGangMembers_6787CE = 0;
    gNumPedsCrossingRoad_6787D0 = 0;
    mFbiArmyCount = 0;
    gPedOffscreenDespawnTime_678656 = kDefaultPedOffscreenDespawnTime_61A898;
    sPedsServiceTickCount = 0;
    gNewTaxiCustomersThisTick_6787D2 = 0;
    gNumberBusCustomers_6787D3 = 0;
    gSearchConeHalfAngle_6784F0 = gDummyPedAng_6787A8;
    gCloseRangeHit_6787D4 = 0;
    sSpawnSideLocked = 0;
    sSpawnCountLimit = 0;
    gTargetSearchMode_6787D7 = threat_search_enum::no_threats_0;
    gRunPedObjectiveAI_61A8A0 = 1;
    gPathStepSlotFree_61A8A1 = 1;
    gPathStepRoundDone_6787D8 = 0;
    gLosCheckSlotFree_61A8A2 = 1;
    gLosCheckRoundDone_6787D9 = 0;
    bHaveThreateningPeds_6787DA = 0;
    gDistanceToTarget_678750 = kFpZero_678660;
    gSearchingPed_6787DC = NULL;
    sCameraFacingAng = gDummyPedAng_6787A8;
    gObjectiveMayAct_61A8A3 = 1;
    gRunPedThink_61A8A4 = 1;
    gNumPedsUpdated_6787E0 = 0;
    gNumDummyChars_6787E2 = 0;
    gNumScriptCreatedPeds_6787E3 = 0;
    gNumPedsOnScreen_6787EC = 0;
    bThreateningPedAdded_6787EF = 0;
    ResetCharStatics_553F90();
    gThreateningPedsList_678468.ClearList_420E90();
}

MATCH_FUNC(0x4709b0)
PedManager::~PedManager()
{
    if (gThreateningPedsList_678468.mpFirstPed)
    {
        gThreateningPedsList_678468.ClearPeds_4712F0();
    }

    if (gPedPool_6787B8)
    {
        GTA2_DELETE_AND_NULL(gPedPool_6787B8);
    }

    if (gChar_B4_Pool_6FDB44)
    {
        GTA2_DELETE_AND_NULL(gChar_B4_Pool_6FDB44);
    }

    if (gChar_8_Pool_678b50)
    {
        GTA2_DELETE_AND_NULL(gChar_8_Pool_678b50);
    }

    if (mSpawnProbeSprite)
    {
        gSpritePool_703818->remove(mSpawnProbeSprite);
        mSpawnProbeSprite = NULL;
    }
}

MATCH_FUNC(0x470a50)
Ped* PedManager::SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation)
{
    Ped* pPed = gPedPool_6787B8->Allocate();

    if (!pPed->AllocCharB4_45C830(xpos, ypos, zpos))
    {
        return NULL;
    }
    pPed->SetRotation_433C00(rotation);
    pPed->set_remap_433B90(remap);

    pPed->SetRemap_433C10(pPed->get_remap_433BA0());
    pPed->mRotation = rotation;
    pPed->mThreatSearch = threat_search_enum::area_2;
    pPed->mThreatReaction = threat_reaction_enum::run_away_3;
    pPed->set_health_4039A0(100);
    pPed->mGraphicType = ped_graphic_type::civilian_0;
    return pPed;
}

MATCH_FUNC(0x470b00)
Ped* PedManager::SpawnDriver_470B00(Car* pCar)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->SetPedType_403920(ped_type::dummy_3);
    pNewPed->set_occupation_403970(ped_ocupation_enum::dummy_driver_4);
    pNewPed->mCar = pCar;
    pNewPed->mGameObject = NULL;
    pNewPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
    pNewPed->set_enter_car_as_passenger_4039B0(0);
    pNewPed->set_target_car_door_403A70(0);
    pNewPed->mThreatSearch = threat_search_enum::area_2;
    pNewPed->mThreatReaction = threat_reaction_enum::run_away_3;
    pNewPed->set_health_4039A0(100);
    pNewPed->mGraphicType = ped_graphic_type::civilian_0;
    pCar->SetDriver(pNewPed);
    return pNewPed;
}

// https://decomp.me/scratch/6AW5o
MATCH_FUNC(0x470ba0)
Ped* PedManager::SpawnGangDriver_470BA0(Car* pCar, Gang* pGang)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();

    pNewPed->SetPedType_403920(ped_type::dummy_with_occupation_6);
    pNewPed->set_occupation_403970(ped_ocupation_enum::gang_driver_42);
    pNewPed->mCar = pCar;
    pNewPed->mGameObject = NULL;
    pNewPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
    pNewPed->set_enter_car_as_passenger_4039B0(0);
    pNewPed->set_target_car_door_403A70(0);
    pNewPed->mThreatSearch = threat_search_enum::area_2;
    pNewPed->mThreatReaction = threat_reaction_enum::run_away_3;
    pNewPed->set_health_4039A0(100);
    pNewPed->mGraphicType = ped_graphic_type::character_1;

    pCar->SetDriver(pNewPed);

    pNewPed->mpGang = pGang;
    pNewPed->set_remap_433B90(pGang->mRemap);

    if (pNewPed->get_remap_433BA0() == ped_remap_enum::ped_remap_red_head_redneck)
    {
        if (!gRng_6F6784.get_int_4F7AE0(2))
        {
            pNewPed->set_remap_433B90(ped_remap_enum::ped_remap_blond_head_redneck);
        }
    }
    pNewPed->mGraphicType = ped_graphic_type::character_1;
    pNewPed->SetPedClass_433BC0(ped_class::gang_member_1);

    pNewPed->ForceWeapon_46F600(pNewPed->mpGang->GetCurrentWeapon_4BF0C0());
    pNewPed->GiveWeapon_46F650(weapon_type::pistol);

    pNewPed->mFireMode = ped_fire_mode::random_shots_0;
    pNewPed->mThreatSearch = threat_search_enum::line_of_sight_1;
    pNewPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;

    return pNewPed;
}

MATCH_FUNC(0x470cc0)
Ped* PedManager::CreateDummyDriver_470CC0(Car* pCar)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->set_remap_433B90(-1);
    pNewPed->mGraphicType = ped_graphic_type::civilian_0;
    pNewPed->SetPedType_403920(ped_type::dummy_3);
    pNewPed->set_occupation_403970(ped_ocupation_enum::dummy_driver_4);
    pNewPed->mCar = pCar;
    pNewPed->mGameObject = NULL;
    pNewPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
    pNewPed->set_target_car_door_403A70(0);
    pNewPed->mThreatSearch = threat_search_enum::area_2;
    pNewPed->mThreatReaction = threat_reaction_enum::run_away_3;
    pNewPed->set_health_4039A0(100);
    pNewPed->mGraphicType = ped_graphic_type::civilian_0;
    return pNewPed;
}

MATCH_FUNC(0x470d60)
Ped* PedManager::SpawnRunAwayGuy_470D60()
{
    Ped* pPed = gPedPool_6787B8->Allocate();

    s32 rng_val = gRng_6F6784.get_int_4F7AE0(4);
    switch (rng_val)
    {
        default:
            pPed->set_remap_433B90(ped_remap_enum::ped_remap_short_sleeve);
            break;
        case 1:
            pPed->set_remap_433B90(ped_remap_enum::ped_remap_light_blue_dummy);
            break;
        case 0:
            pPed->set_remap_433B90(ped_remap_enum::ped_remap_blue_dummy);
            break;

        case 2:
            pPed->set_remap_433B90(ped_remap_enum::ped_remap_tshirt_and_shorts_dummy);
            break;
    }

    pPed->SetPedType_403920(ped_type::dummy_with_occupation_6);
    pPed->set_occupation_403970(ped_ocupation_enum::robbed_driver_10);
    pPed->mGameObject = NULL;
    pPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
    pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
    pPed->set_health_4039A0(100);
    pPed->mThreatSearch = threat_search_enum::area_2;
    pPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;
    pPed->mGraphicType = ped_graphic_type::civilian_0;
    return pPed;
}

// 9.6f 0x43DEB0
MATCH_FUNC(0x470e30)
Ped* PedManager::SpawnTrainLeaver_470E30()
{
    Ped* pPed = gPedPool_6787B8->Allocate();

    pPed->mGraphicType = ped_graphic_type::civilian_0;

    switch (gRng_6F6784.get_int_4F7AE0(4))
    {
        case 0:
            pPed->set_remap_433B90(ped_remap_enum::ped_remap_blue_dummy);
            break;
        case 1:
            pPed->set_remap_433B90(ped_remap_enum::ped_remap_light_blue_dummy);
            break;
        case 2:
            pPed->set_remap_433B90(ped_remap_enum::ped_remap_tshirt_and_shorts_dummy);
            break;
        default:
            pPed->set_remap_433B90(ped_remap_enum::ped_remap_short_sleeve);
            break;
    }

    pPed->SetPedType_403920(ped_type::special_ped_4);
    pPed->set_occupation_403970(ped_ocupation_enum::train_customer_9);
    pPed->mGameObject = NULL;
    pPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
    pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
    pPed->set_health_4039A0(50);
    pPed->mThreatSearch = threat_search_enum::area_2;
    pPed->mThreatReaction = threat_reaction_enum::run_away_3;
    return pPed;
}

MATCH_FUNC(0x470f30)
Ped* PedManager::AllocatePed_470F30()
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->mHealth = 100;
    return pNewPed;
}

MATCH_FUNC(0x470f90)
Ped* PedManager::ClonePed_470F90(Ped* pSrc)
{
    Ped* pDst = gPedPool_6787B8->Allocate();
    Ped* pNext = pDst->mpNext;
    memcpy(pDst, pSrc, sizeof(Ped));
    pDst->mpNext = pNext;

    if (pSrc->mGameObject)
    {
        pDst->AllocCharB4_45C830(pSrc->get_cam_x(), pSrc->get_cam_y(), pSrc->get_cam_z());
        pDst->SetRemap_433C10(pSrc->get_remap_433BA0());
        pDst->SetRotation_433C00(pSrc->GetRotation());
        pDst->SetStateInitPending_433B50();
        pDst->mGameObject->mTargetCar = pSrc->mGameObject->mTargetCar;
    }
    return pDst;
}

MATCH_FUNC(0x471060)
void PedManager::DoIanTest_471060(u16 key)
{
    switch (key)
    {
        case DIK_9:
            gGame_0x40_67E008->mLocalPlayer->mPlayerPed->SetOnFire();
            break;
        case DIK_8:
            gGame_0x40_67E008->mLocalPlayer->mPlayerPed->SetVisible();
            break;

        case DIK_7:
            gGame_0x40_67E008->mLocalPlayer->mPlayerPed->SetInvisible();
            break;
    }
}

MATCH_FUNC(0x4710c0)
Ped* PedManager::PedById(s32 pedId)
{
    for (Ped* pPedIter = gPedPool_6787B8->GetFirstPed_435530(); pPedIter; pPedIter = pPedIter->mpNext)
    {
        if (pPedIter->mId == static_cast<u32>(pedId))
        {
            return pPedIter;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4710F0)
CharPool::~CharPool()
{
    mPool.mpHead = NULL;
}

MATCH_FUNC(0x471110)
PedPool::~PedPool()
{
}
