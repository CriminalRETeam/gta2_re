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
EXTERN_GLOBAL(s16, gPedOffscreenDespawnTime);
EXTERN_GLOBAL(Ang16, gSearchConeHalfAngle);
EXTERN_GLOBAL(u8, gRunPedObjectiveAI);
EXTERN_GLOBAL(s8, gObjectiveMayAct);
EXTERN_GLOBAL(u8, gRunPedThink);
EXTERN_GLOBAL(Fix16, kFpZero_678660);
EXTERN_GLOBAL(Fix16, gDistanceToTarget);
EXTERN_GLOBAL(Ped*, gLastProcessedPed);
EXTERN_GLOBAL(u8, gNumberArmedGangMembers);
EXTERN_GLOBAL(s16, gNumPedsCrossingRoad);
EXTERN_GLOBAL(u8, gNewTaxiCustomersThisTick);
EXTERN_GLOBAL(char_type, gNumberBusCustomers);
EXTERN_GLOBAL(u8, gCloseRangeHit);
DEFINE_GLOBAL(u8, gTargetSearchMode, 0x6787D7);
EXTERN_GLOBAL(u8, gPathStepRoundDone);
EXTERN_GLOBAL(u8, gLosCheckRoundDone);
EXTERN_GLOBAL(Ped*, gSearchingPed);
EXTERN_GLOBAL(u16, gNumPedsUpdated);
EXTERN_GLOBAL(u8, gNumDummyChars);
EXTERN_GLOBAL(u8, gNumScriptCreatedPeds);

// Initial value not verified (assumed to be gPedOffscreenDespawnTime's default, which PedManager resets from it)
DEFINE_GLOBAL_INIT(s16, kDefaultPedOffscreenDespawnTime, 40, 0x61A898);
DEFINE_GLOBAL(PedManager*, gPedManager, 0x6787BC);
DEFINE_GLOBAL(PedPool*, gPedPool, 0x6787B8);
DEFINE_GLOBAL(CharPool*, gChar_B4_Pool, 0x6FDB44);
DEFINE_GLOBAL(PedListNodePool*, gChar_8_Pool, 0x678b50);

DEFINE_STATIC_MEMBER(u16, PedManager, sPedsServiceTickCount, 0x6787F0);
DEFINE_GLOBAL_INIT(u8, gPathStepSlotFree, 1, 0x61A8A1);
DEFINE_GLOBAL_INIT(u8, gLosCheckSlotFree, 1, 0x61A8A2);
DEFINE_GLOBAL(u8, gNumPolicePedsInRangeScreen, 0x6787EE);
DEFINE_GLOBAL(u8, bThreateningPedAdded, 0x6787EF);
DEFINE_GLOBAL(u8, bHaveThreateningPeds, 0x6787DA);

DEFINE_STATIC_MEMBER(u8, PedManager, sSpawnSideLocked, 0x6787D5);
DEFINE_STATIC_MEMBER(u8, PedManager, sSpawnCountLimit, 0x6787D6);
DEFINE_STATIC_GLOBAL_INIT(Ang16, sCameraFacingAng, Ang16(0), 0x678760);
DEFINE_STATIC_MEMBER(s8, PedManager, sSpawnSide, 0x6787C8);
DEFINE_STATIC_MEMBER(s8, PedManager, sSpawnIndex, 0x6787C9);

EXTERN_GLOBAL(Fix16, gDummyW);
EXTERN_GLOBAL(Fix16, gDummyZ);

DEFINE_STATIC_GLOBAL_INIT(Fix16, sDummyH, Fix16(0xCCC, 0), 0x678584); // same as gDummyW, which Ped.cpp initializes too late
DEFINE_GLOBAL_INIT(Fix16, kFp255_678414, Fix16(255), 0x678414);

EXTERN_GLOBAL(u8, gNumEmergencyPeds);

EXTERN_GLOBAL(Fix16, kFpZero_678438);
EXTERN_GLOBAL(Fix16, kFpHalf_67853C);
EXTERN_GLOBAL(Ang16, gDummyPedAng);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff, 640);

EXTERN_GLOBAL(Fix16, gSpawnJitterScale);
EXTERN_GLOBAL(Fix16, kFpOne_678664);

EXTERN_GLOBAL(Fix16, kFpPoint8_6784A0);
EXTERN_GLOBAL(Fix16, kFpPoint1_678480);

DEFINE_GLOBAL_INIT(Ang16, gSpawnRotationLeft, Ang16(360), 0x6786E0);
DEFINE_GLOBAL_INIT(Ang16, gSpawnRotationTop, Ang16(0), 0x6787B0);
DEFINE_GLOBAL_INIT(Ang16, gSpawnRotationRight, Ang16(1080), 0x678578);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6785A6, Ang16(0x2D0), 0x6785A6);
DEFINE_GLOBAL_INIT(Ang16, gSpawnRotationBottom, Ang16(720), 0x678540);
DEFINE_STATIC_GLOBAL_INIT(s16, sSpawnCounter, 0, 0x6787C6);

EXPORT Ped* __stdcall SpawnPedChainGroupAt(char_type remap, u8 number_followers, Fix16 xpos, Fix16 ypos, Fix16 zpos);

MATCH_FUNC(0x46DB90)
EXPORT Ped* __stdcall SpawnPedChainGroupAt(char_type remap, u8 number_followers, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Fix16 xpos_adjusted = kFpPoint8_6784A0 + Fix16(static_cast<u8>(xpos.ToInt()));
    Fix16 ypos_adjusted = kFpPoint8_6784A0 + Fix16(static_cast<u8>(ypos.ToInt()));

    Ped* pLeader = gPedPool->Allocate();
    pLeader->mOccupation = ped_ocupation_enum::elvis_leader;
    pLeader->mRemap = remap;
    pLeader->mGraphicType = ped_graphic_type::character_1;
    pLeader->mPedType = ped_type::special_ped_4;
    pLeader->AllocCharB4(xpos_adjusted, ypos_adjusted, zpos);
    pLeader->mGameObject->SetRemap(pLeader->mRemap);
    pLeader->mHealth = 100;

    PedGroup* pGroup = PedGroup::New();
    pGroup->add_ped_leader(pLeader);
    pGroup->mGroupType = ped_group_type::chain_1;
    pGroup->mCount36 = number_followers;
    pGroup->mCount34 = number_followers;

    for (u8 ped_idx = 0; ped_idx < number_followers; ped_idx++)
    {
        Fix16 xy_off = Fix16(0x4000 * (ped_idx + 1), 0);

        Ped* pNewPed = gPedPool->Allocate();

        pNewPed->mOccupation = ped_ocupation_enum::elvis;
        pNewPed->mRemap = remap;
        pNewPed->mPedType = ped_type::special_ped_4;
        pNewPed->AllocCharB4(xpos_adjusted - ((kFpPoint1_678480 * xy_off)), ypos_adjusted - ((kFpPoint1_678480 * xy_off)), zpos);

        pNewPed->SetRemap(pNewPed->get_remap());

        pNewPed->mHealth = 100;
        pNewPed->mGraphicType = ped_graphic_type::character_1;
        pGroup->add_ped_to_list(pNewPed, ped_idx);
    }
    return pLeader;
}

// Not in 9.6f (0x440CC0 calls Fix16::multiply_401BD0 directly), but written inline the product
// lands in ebx directly instead of through eax like 10.5.
static inline Fix16 GetSpawnJitter()
{
    return gSpawnJitterScale * (gRng.get_int(32) + 8);
}

// The kind of ped SpawnPedestrianAt picks from the zone's ratios
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
EXPORT void __stdcall SpawnPedestrianAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    u8 kind = k_spawn_dummy;
    s16 rng_val = gRng.get_int(1000);

    Ped* pPed = gPedPool->Allocate();

    ++sSpawnCounter;

    gmp_zone_info* pZone = gMap_0x370->GetZoneInfoAt(xpos.ToInt(), ypos.ToInt());

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
        kind = gRng.get_int(50) == 25 ? k_spawn_elvis : k_spawn_dummy;
    }
    else if (rng_val < static_cast<u16>(pZone->mMuggerRatio) + static_cast<u16>(pZone->mCarthiefRatio) + static_cast<u16>(pZone->mElvisRatio) +
                 static_cast<u16>(pZone->mGangcharRatio))
    {
        // Gang member, limited to 8
        kind = static_cast<u8>(gNumberArmedGangMembers) < 8u ? k_spawn_gang_member : k_spawn_dummy;
    }
    else if (rng_val < static_cast<u16>(pZone->mMuggerRatio) + static_cast<u16>(pZone->mCarthiefRatio) + static_cast<u16>(pZone->mElvisRatio) +
                 static_cast<u16>(pZone->mGangcharRatio) + static_cast<u16>(pZone->mPolicepedRatio))
    {
        kind = k_spawn_cop;
    }

    if (gCheatOnlyElvisPeds)
    {
        kind = k_spawn_elvis;
    }

    if (bNo_annoying_chars)
    {
        if (kind != k_spawn_cop)
        {
            kind = k_spawn_dummy;
        }
    }

    if (pPed)
    {
        pPed->ChangeNextPedState1(ped_state_1::walking_0);
        pPed->ChangeNextPedState2(ped_state_2::ped2_walking_0);
        pPed->SetPedType(ped_type::dummy_3);
        pPed->SetOffscreenCounter(1);
        pPed->set_remap(ped_remap_enum::ped_remap_yellow_police);

        switch (kind)
        {
            case k_spawn_mugger:
                if (gNumberMuggersSpawned)
                {
                    goto delloc_ret_2;
                }
                gNumberMuggersSpawned = 1;
                pPed->set_occupation(ped_ocupation_enum::mugger);
                pPed->SetPedClass(ped_class::mugger_or_car_thief_2);
                pPed->set_objective_timer(40);
                pPed->SetPedType(ped_type::special_ped_4);
                pPed->set_remap(ped_remap_enum::ped_remap_mugger);
                pPed->mThreatSearch = threat_search_enum::area_2;
                pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                pPed->mGraphicType = ped_graphic_type::civilian_0;
                break;

            case k_spawn_car_thief:
                if (gNumberCarThiefsSpawned)
                {
                    goto delloc_ret_2;
                }
                gNumberCarThiefsSpawned = 1;
                pPed->SetPedClass(ped_class::mugger_or_car_thief_2);
                pPed->mThreatSearch = threat_search_enum::area_2;
                pPed->set_objective_timer(40);
                pPed->set_occupation(ped_ocupation_enum::car_thief);
                pPed->SetPedType(ped_type::dummy_with_occupation_6);
                pPed->set_remap(ped_remap_enum::ped_remap_green_shorts);
                pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                pPed->mGraphicType = ped_graphic_type::civilian_0;
                pPed->mBf21C.bDriveAggressively = true;
                pPed->mRunSpeed = kFpFour_678670;
                break;

            case k_spawn_elvis:
                if (gNumberElvisLeadersSpawned)
                {
                    goto delloc_ret_2;
                }
                SpawnPedChainGroupAt(12, 5u, xpos, ypos, zpos);
                pPed->Deallocate();
                ++gNumberElvisLeadersSpawned;
                break;

            case k_spawn_gang_member:
            {
                gmp_map_zone* pMapZone = gMap_0x370->GetZoneAtPosOfType(xpos.ToInt(), ypos.ToInt(), 14u);
                if (pMapZone)
                {
                    Gang* pGang = gGangPool_CA8->FindGangByName(pMapZone->mName);
                    if (static_cast<u8>(gNumberArmedGangMembers) < 4u)
                    {
                        ++gNumberArmedGangMembers;
                        pPed->SetPedType(ped_type::special_ped_4);
                        pPed->set_occupation(ped_ocupation_enum::armed_gang_member_19);
                        pPed->mpGang = pGang;
                        pPed->set_remap(pGang->mRemap);
                        if (pPed->get_remap() == ped_remap_enum::ped_remap_red_head_redneck)
                        {
                            if (!gRng.get_int(2))
                            {
                                pPed->set_remap(ped_remap_enum::ped_remap_blond_head_redneck);
                            }
                        }
                        pPed->mGraphicType = ped_graphic_type::character_1;
                        pPed->SetPedClass(ped_class::gang_member_1);
                        pPed->ForceWeapon(pPed->mpGang->GetCurrentWeapon());
                        if (pPed->mSelectedWeapon && pPed->mSelectedWeapon->mWeaponType)
                        {
                            pPed->mBf21C.bArmedGangMember = true;
                            pGang->mAlwaysBasicWeapon = true;
                        }
                        pPed->GiveWeapon(weapon_type::pistol);
                        pPed->mFireMode = ped_fire_mode::random_shots_0;
                        pPed->mThreatSearch = threat_search_enum::line_of_sight_1;
                        pPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;
                    }
                    else
                    {
                        pPed->mDummyGang = pGang;
                        pPed->set_occupation(ped_ocupation_enum::dummy);
                        pPed->SetPedClass(ped_class::gang_dummy_0);
                        pPed->SetPedType(ped_type::dummy_3);
                        pPed->mThreatSearch = threat_search_enum::area_2;
                        pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                        pPed->set_remap(pGang->mRemap);
                        pPed->mGraphicType = ped_graphic_type::character_1;
                        if (pPed->get_remap() == ped_remap_enum::ped_remap_red_head_redneck)
                        {
                            if (!gRng.get_int(2))
                            {
                                pPed->set_remap(ped_remap_enum::ped_remap_blond_head_redneck);
                            }
                        }
                    }
                }
                else
                {
                    pPed->set_occupation(ped_ocupation_enum::dummy);
                    pPed->SetPedClass(ped_class::gang_dummy_0);
                    pPed->SetPedType(ped_type::dummy_3);
                    pPed->mThreatSearch = threat_search_enum::area_2;
                    pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                    pPed->mGraphicType = ped_graphic_type::civilian_0;
                    switch (gRng.get_int(4))
                    {
                        case 0:
                            pPed->set_remap(ped_remap_enum::ped_remap_blue_dummy);
                            break;

                        case 1:
                            pPed->set_remap(ped_remap_enum::ped_remap_light_blue_dummy);
                            break;

                        case 2:
                            pPed->set_remap(ped_remap_enum::ped_remap_tshirt_and_shorts_dummy);
                            break;

                        default:
                            pPed->set_remap(ped_remap_enum::ped_remap_short_sleeve);
                            break;
                    }
                }
                break;
            }
            case k_spawn_cop:
                if (gNumberWalkingCopsSpawned || bSkip_police || gPolice_7B8->mHighestCrewTypeInPursuit == crew_type::army_6)
                {
                // shared exit for the four spawn limits: goto-free forms give each its own epilogue
                delloc_ret_2:
                    pPed->Deallocate();
                    return;
                }
                gPolice_7B8->SpawnWalkingGuard(pPed, xpos, ypos, zpos, rotation);
                gNumberWalkingCopsSpawned = 1;
                pPed->mThreatSearch = threat_search_enum::line_of_sight_1;
                pPed->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
                switch (gPolice_7B8->mMaxWantedLevel)
                {
                    case cop_level_enum::police_2:
                        pPed->GiveWeapon(weapon_type::pistol);
                        pPed->set_health(100);
                        pPed->mMaxSpeed = (kFpOneSixteenth_678448 * kFpPoint8_6784A0);
                        pPed->mGraphicType = ped_graphic_type::cop_2;
                        break;
                    case cop_level_enum::none_0:
                    case cop_level_enum::police_1:
                        pPed->mSelectedWeapon = NULL;
                        pPed->GiveWeapon(weapon_type::pistol);
                        pPed->set_health(50);
                        pPed->mMaxSpeed = (kFpOneSixteenth_678448 * kFpPoint8_6784A0);
                        pPed->mGraphicType = ped_graphic_type::cop_2;
                        break;

                    default:
                        pPed->GiveWeapon(weapon_type::pistol);
                        pPed->set_health(100);
                        pPed->mGraphicType = ped_graphic_type::cop_2;
                        break;
                } // End switch
                break;

            default:
                if (gPolice_7B8->mHighestCrewTypeInPursuit == crew_type::army_6)
                {
                    gPolice_7B8->SpawnWalkingGuard(pPed, xpos, ypos, zpos, rotation);
                    pPed->mThreatSearch = threat_search_enum::line_of_sight_1;
                    pPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;
                    pPed->ForceWeapon(weapon_type::smg);
                    pPed->mGraphicType = ped_graphic_type::cop_2;
                }
                else
                {
                    pPed->set_occupation(ped_ocupation_enum::dummy);
                    pPed->SetPedClass(ped_class::gang_dummy_0);
                    pPed->SetPedType(ped_type::dummy_3);
                    pPed->mThreatSearch = threat_search_enum::area_2;
                    pPed->mThreatReaction = threat_reaction_enum::run_away_3;
                    pPed->mGraphicType = ped_graphic_type::civilian_0;

                    if (gCheatNakedPeds)
                    {
                        pPed->set_remap(ped_remap_enum::ped_remap_naked_dummy);
                    }
                    else
                    {
                        u8 remap = static_cast<u8>(gRng.get_int(25));
                        if (remap < 4u)
                        {
                            remap += 18;
                        }
                        else
                        {
                            remap += 27;
                        }
                        pPed->set_remap(remap);
                    }
                }
                break;
        } // End switch

        if (pPed->get_occupation() != ped_ocupation_enum::walking_guard_29 &&
            pPed->get_occupation() != ped_ocupation_enum::walking_army_guard_31)
        {
            pPed->AllocCharB4(xpos, ypos, zpos);
        }

        gSpriteGrid_1->RemoveFromRegionBuckets(pPed->mGameObject->mSpritePtr);

        if (gSpriteGrid_1->FindCollidingSpritePreferType(pPed->mGameObject->mSpritePtr, sprite_types_enum::none_0))
        {
            pPed->ChangeNextPedState1(ped_state_1::dead_9);
            pPed->ChangeNextPedState2(ped_state_2::ped2_dead_15);
            pPed->Deallocate();
            gSpriteGrid_1->AddToRegionBuckets(pPed->mGameObject->mSpritePtr);
            return;
        }

        gSpriteGrid_1->AddToRegionBuckets(pPed->mGameObject->mSpritePtr);
        if (pPed->get_occupation() != ped_ocupation_enum::walking_guard_29)
        {
            pPed->SetRemap(pPed->get_remap());
            pPed->SetRotation(rotation);
            pPed->ResetAnimAndFindNearestSprite();
        }

        if (gPedManager->mMakeAllMuggers)
        {
            if (!gRng.get_int(2))
            {
                if (pPed->get_occupation() == ped_ocupation_enum::armed_gang_member_19)
                {
                    --gNumberArmedGangMembers;
                }
                pPed->SetPedClass(ped_class::mugger_or_car_thief_2);
                pPed->set_objective_timer(40);
                pPed->set_occupation(ped_ocupation_enum::mad_mugger_40);
                pPed->SetPedType(ped_type::special_ped_4);
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
void PedManager::SpawnDummies(Camera* pCam)
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
        switch (Ang16::GetAngleFace(sCameraFacingAng))
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
                rotation = gSpawnRotationLeft;
                break;

            case 1:
                ypos = Fix16(tileTop);
                xpos = Fix16(tileLeft + sSpawnIndex);
                rotation = gSpawnRotationTop;
                break;

            case 2:
                xpos = Fix16(tileRight);
                ypos = Fix16(tileTop + sSpawnIndex);
                rotation = gSpawnRotationRight;
                break;

            case 3:
                ypos = Fix16(tileBottom);
                xpos = Fix16(tileLeft + sSpawnIndex);
                rotation = gSpawnRotationBottom;
                break;
        }

        Fix16 jitterX = GetSpawnJitter();
        Fix16 jitterY = GetSpawnJitter();
        xpos += jitterX;
        ypos += jitterY;

        if (xpos > kFpOne_678664 && xpos < kFp255_678414 - kFpOne_678664 && ypos > kFpOne_678664 &&
            ypos < kFp255_678414 - kFpOne_678664)
        {
            gmp_zone_info* pZoneInfo = gMap_0x370->GetZoneInfoAt(xpos.ToInt(), ypos.ToInt());
            if (static_cast<u8>(mNumPedsOnScreen) < static_cast<u16>(pZoneInfo->mPedDensity) / 25)
            {
                if (gMap_0x370->FindPavementBlockForCoord(xpos.ToInt(), ypos.ToInt(), zpos))
                {
                    pSprite->set_xyz_lazy_451950(xpos, ypos, (zpos + 1));
                    pSprite->set_ang_lazy(gDummyPedAng);
                    pSprite->AllocCollisionShape(gDummyW, sDummyH, gDummyZ);
                    if (!gGame_0x40->IsPointOnScreen(pSprite->mXy.x, pSprite->mXy.y))
                    {
                        if (!gMap_0x370->IsSlopeB4ToD0At(static_cast<u8>(xpos.ToInt()), static_cast<u8>(ypos.ToInt()), static_cast<u8>(zpos) + 1))
                        {
                            SpawnPedestrianAt(xpos, ypos, Fix16(zpos + 1), rotation);
                        }
                    }
                }
            }
        }
        ++sSpawnIndex;
    }
}

MATCH_FUNC(0x470330)
void PedManager::Dummies()
{
    s16 max_dummies = gPedManager->mMaxDummyChars;
    if (gPolice_7B8->mMaxWantedLevel > cop_level_enum::police_3)
    {
        max_dummies = static_cast<u16>(max_dummies) >> 1;
    }
    if (gNumDummyChars < max_dummies)
    {
        for (Camera* pCam = gGame_0x40->IteratePlayerCamera(); pCam; pCam = gGame_0x40->IterateNextPlayerCamera())
        {
            sSpawnSideLocked = 0;
            if (pCam->HasFollowTarget())
            {
                if (pCam->ReturnOwnerVelocity() > kFpZero_678438)
                {
                    sSpawnSideLocked = 1;
                    sCameraFacingAng = pCam->ComputeTargetFacingAngle();
                    sSpawnCountLimit = 1;
                }
                else
                {
                    sSpawnCountLimit = 2;
                }
            }
            // Use global instance even tho we're already in an instance method :)
            gPedManager->SpawnDummies(pCam);
        }
    }
}

// https://decomp.me/scratch/dQf8H
MATCH_FUNC(0x4703f0)
void PedManager::PedsService()
{
    ++sPedsServiceTickCount;
    gNumPedsUpdated = 0;
    gNumDummyChars = 0;
    gNumEmergencyPeds = 0;
    gNumScriptCreatedPeds = 0;
    gNumPedsOnScreen = 0;
    gPathStepSlotFree = 1;
    gLosCheckSlotFree = 1;
    gNewTaxiCustomersThisTick = 0;
    gNumPolicePedsInRangeScreen = 0;

    gPedPool->UpdatePool();

    if (bThreateningPedAdded)
    {
        bHaveThreateningPeds = 1;
    }
    else
    {
        bHaveThreateningPeds = 0;
        gThreateningPedsList.ClearPeds();
    }

    gPathStepRoundDone = gPathStepSlotFree == 1;
    gLosCheckRoundDone = gLosCheckSlotFree == 1;

    if (!bSkip_dummies)
    {
        Dummies();
    }
    mNumPedsUpdated = static_cast<char_type>(gNumPedsUpdated);
    mNumDummyChars = gNumDummyChars;
    mNumScriptCreatedPeds = gNumScriptCreatedPeds;
    mFbiArmyCount = gNumEmergencyPeds;
    mNumPedsOnScreen = static_cast<char_type>(gNumPedsOnScreen);
    if (gPolice_7B8)
    {
        gPolice_7B8->mPolicePedsInRangeScreen = gNumPolicePedsInRangeScreen;
    }
    bThreateningPedAdded = 0;
    if (bDo_iain_test)
    {
        // The original reads a dword here
        u16 num_peds = *reinterpret_cast<u32*>(&gNumPedsOnScreen);
        swprintf(tmpBuff, L"num peds on screen : %d", num_peds);
        gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 64, gDebugFont, 1);

        if (gPolice_7B8)
        {
            swprintf(tmpBuff,
                     L"num police peds in range screen : %d",
                     static_cast<u8>(gPolice_7B8->mPolicePedsInRangeScreen));
            gHud_2B00->mTexts.DisplayText(tmpBuff, 0, 80, gDebugFont, 1);
        }
    }
}

// https://decomp.me/scratch/P1OvR
MATCH_FUNC(0x470650)
PedManager::PedManager()
{
    mSpawnProbeSprite = NULL;
    if (!gPedPool)
    {
        gPedPool = new PedPool();
        if (!gPedPool)
        {
            FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15827);
        }
    }

    if (!gChar_B4_Pool)
    {
        gChar_B4_Pool = new CharPool();
        if (!gChar_B4_Pool)
        {
            FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15834);
        }
    }

    if (!gChar_8_Pool)
    {
        gChar_8_Pool = new PedListNodePool();
        if (!gChar_8_Pool)
        {
            FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15841);
        }
    }

    mSpawnProbeSprite = gSpritePool->get_new_sprite();
    // Plain 0s: implicit Fix16 conversions, out of line (Fix16::FromInt_4369F0) once the inline budget runs out
    mSpawnProbeSprite->set_xyz_lazy_451950(0, 0, 0); // 9.6f inlined set_xyz_lazy
    mSpawnProbeSprite->set_ang_lazy(gDummyPedAng);
    mSpawnProbeSprite->AllocCollisionShape(0, 0, 0);

    mNumDummyChars = 0;
    mNumPedsUpdated = 0;
    mNumScriptCreatedPeds = 0;
    mNumPedsOnScreen = 0;
    mMaxDummyChars = 50;
    mMakeAllMuggers = false;

    gPedId = 7;
    gLastProcessedPed = NULL;
    sSpawnCounter = 0;
    sSpawnSide = 0;
    sSpawnIndex = 0;
    gNumberMuggersSpawned = 0;
    gNumberCarThiefsSpawned = 0;
    gNumberElvisLeadersSpawned = 0;
    gNumberWalkingCopsSpawned = 0;
    gNumberArmedGangMembers = 0;
    gNumPedsCrossingRoad = 0;
    mFbiArmyCount = 0;
    gPedOffscreenDespawnTime = kDefaultPedOffscreenDespawnTime;
    sPedsServiceTickCount = 0;
    gNewTaxiCustomersThisTick = 0;
    gNumberBusCustomers = 0;
    gSearchConeHalfAngle = gDummyPedAng;
    gCloseRangeHit = 0;
    sSpawnSideLocked = 0;
    sSpawnCountLimit = 0;
    gTargetSearchMode = threat_search_enum::no_threats_0;
    gRunPedObjectiveAI = 1;
    gPathStepSlotFree = 1;
    gPathStepRoundDone = 0;
    gLosCheckSlotFree = 1;
    gLosCheckRoundDone = 0;
    bHaveThreateningPeds = 0;
    gDistanceToTarget = kFpZero_678660;
    gSearchingPed = NULL;
    sCameraFacingAng = gDummyPedAng;
    gObjectiveMayAct = 1;
    gRunPedThink = 1;
    gNumPedsUpdated = 0;
    gNumDummyChars = 0;
    gNumScriptCreatedPeds = 0;
    gNumPedsOnScreen = 0;
    bThreateningPedAdded = 0;
    ResetCharStatics();
    gThreateningPedsList.ClearList();
}

MATCH_FUNC(0x4709b0)
PedManager::~PedManager()
{
    if (gThreateningPedsList.mpFirstPed)
    {
        gThreateningPedsList.ClearPeds();
    }

    if (gPedPool)
    {
        GTA2_DELETE_AND_NULL(gPedPool);
    }

    if (gChar_B4_Pool)
    {
        GTA2_DELETE_AND_NULL(gChar_B4_Pool);
    }

    if (gChar_8_Pool)
    {
        GTA2_DELETE_AND_NULL(gChar_8_Pool);
    }

    if (mSpawnProbeSprite)
    {
        gSpritePool->remove(mSpawnProbeSprite);
        mSpawnProbeSprite = NULL;
    }
}

MATCH_FUNC(0x470a50)
Ped* PedManager::SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation)
{
    Ped* pPed = gPedPool->Allocate();

    if (!pPed->AllocCharB4(xpos, ypos, zpos))
    {
        return NULL;
    }
    pPed->SetRotation(rotation);
    pPed->set_remap(remap);

    pPed->SetRemap(pPed->get_remap());
    pPed->mRotation = rotation;
    pPed->mThreatSearch = threat_search_enum::area_2;
    pPed->mThreatReaction = threat_reaction_enum::run_away_3;
    pPed->set_health(100);
    pPed->mGraphicType = ped_graphic_type::civilian_0;
    return pPed;
}

MATCH_FUNC(0x470b00)
Ped* PedManager::SpawnDriver(Car* pCar)
{
    Ped* pNewPed = gPedPool->Allocate();
    pNewPed->SetPedType(ped_type::dummy_3);
    pNewPed->set_occupation(ped_ocupation_enum::dummy_driver_4);
    pNewPed->mCar = pCar;
    pNewPed->mGameObject = NULL;
    pNewPed->ChangeNextPedState1(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
    pNewPed->set_enter_car_as_passenger(0);
    pNewPed->set_target_car_door(0);
    pNewPed->mThreatSearch = threat_search_enum::area_2;
    pNewPed->mThreatReaction = threat_reaction_enum::run_away_3;
    pNewPed->set_health(100);
    pNewPed->mGraphicType = ped_graphic_type::civilian_0;
    pCar->SetDriver(pNewPed);
    return pNewPed;
}

// https://decomp.me/scratch/6AW5o
MATCH_FUNC(0x470ba0)
Ped* PedManager::SpawnGangDriver(Car* pCar, Gang* pGang)
{
    Ped* pNewPed = gPedPool->Allocate();

    pNewPed->SetPedType(ped_type::dummy_with_occupation_6);
    pNewPed->set_occupation(ped_ocupation_enum::gang_driver_42);
    pNewPed->mCar = pCar;
    pNewPed->mGameObject = NULL;
    pNewPed->ChangeNextPedState1(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
    pNewPed->set_enter_car_as_passenger(0);
    pNewPed->set_target_car_door(0);
    pNewPed->mThreatSearch = threat_search_enum::area_2;
    pNewPed->mThreatReaction = threat_reaction_enum::run_away_3;
    pNewPed->set_health(100);
    pNewPed->mGraphicType = ped_graphic_type::character_1;

    pCar->SetDriver(pNewPed);

    pNewPed->mpGang = pGang;
    pNewPed->set_remap(pGang->mRemap);

    if (pNewPed->get_remap() == ped_remap_enum::ped_remap_red_head_redneck)
    {
        if (!gRng.get_int(2))
        {
            pNewPed->set_remap(ped_remap_enum::ped_remap_blond_head_redneck);
        }
    }
    pNewPed->mGraphicType = ped_graphic_type::character_1;
    pNewPed->SetPedClass(ped_class::gang_member_1);

    pNewPed->ForceWeapon(pNewPed->mpGang->GetCurrentWeapon());
    pNewPed->GiveWeapon(weapon_type::pistol);

    pNewPed->mFireMode = ped_fire_mode::random_shots_0;
    pNewPed->mThreatSearch = threat_search_enum::line_of_sight_1;
    pNewPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;

    return pNewPed;
}

MATCH_FUNC(0x470cc0)
Ped* PedManager::CreateDummyDriver(Car* pCar)
{
    Ped* pNewPed = gPedPool->Allocate();
    pNewPed->set_remap(-1);
    pNewPed->mGraphicType = ped_graphic_type::civilian_0;
    pNewPed->SetPedType(ped_type::dummy_3);
    pNewPed->set_occupation(ped_ocupation_enum::dummy_driver_4);
    pNewPed->mCar = pCar;
    pNewPed->mGameObject = NULL;
    pNewPed->ChangeNextPedState1(ped_state_1::in_car_10);
    pNewPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
    pNewPed->set_target_car_door(0);
    pNewPed->mThreatSearch = threat_search_enum::area_2;
    pNewPed->mThreatReaction = threat_reaction_enum::run_away_3;
    pNewPed->set_health(100);
    pNewPed->mGraphicType = ped_graphic_type::civilian_0;
    return pNewPed;
}

MATCH_FUNC(0x470d60)
Ped* PedManager::SpawnRunAwayGuy()
{
    Ped* pPed = gPedPool->Allocate();

    s32 rng_val = gRng.get_int(4);
    switch (rng_val)
    {
        default:
            pPed->set_remap(ped_remap_enum::ped_remap_short_sleeve);
            break;
        case 1:
            pPed->set_remap(ped_remap_enum::ped_remap_light_blue_dummy);
            break;
        case 0:
            pPed->set_remap(ped_remap_enum::ped_remap_blue_dummy);
            break;

        case 2:
            pPed->set_remap(ped_remap_enum::ped_remap_tshirt_and_shorts_dummy);
            break;
    }

    pPed->SetPedType(ped_type::dummy_with_occupation_6);
    pPed->set_occupation(ped_ocupation_enum::robbed_driver_10);
    pPed->mGameObject = NULL;
    pPed->ChangeNextPedState1(ped_state_1::in_car_10);
    pPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
    pPed->set_health(100);
    pPed->mThreatSearch = threat_search_enum::area_2;
    pPed->mThreatReaction = threat_reaction_enum::react_as_normal_2;
    pPed->mGraphicType = ped_graphic_type::civilian_0;
    return pPed;
}

// 9.6f 0x43DEB0
MATCH_FUNC(0x470e30)
Ped* PedManager::SpawnTrainLeaver()
{
    Ped* pPed = gPedPool->Allocate();

    pPed->mGraphicType = ped_graphic_type::civilian_0;

    switch (gRng.get_int(4))
    {
        case 0:
            pPed->set_remap(ped_remap_enum::ped_remap_blue_dummy);
            break;
        case 1:
            pPed->set_remap(ped_remap_enum::ped_remap_light_blue_dummy);
            break;
        case 2:
            pPed->set_remap(ped_remap_enum::ped_remap_tshirt_and_shorts_dummy);
            break;
        default:
            pPed->set_remap(ped_remap_enum::ped_remap_short_sleeve);
            break;
    }

    pPed->SetPedType(ped_type::special_ped_4);
    pPed->set_occupation(ped_ocupation_enum::train_customer_9);
    pPed->mGameObject = NULL;
    pPed->ChangeNextPedState1(ped_state_1::in_car_10);
    pPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
    pPed->set_health(50);
    pPed->mThreatSearch = threat_search_enum::area_2;
    pPed->mThreatReaction = threat_reaction_enum::run_away_3;
    return pPed;
}

MATCH_FUNC(0x470f30)
Ped* PedManager::AllocatePed()
{
    Ped* pNewPed = gPedPool->Allocate();
    pNewPed->mHealth = 100;
    return pNewPed;
}

MATCH_FUNC(0x470f90)
Ped* PedManager::ClonePed(Ped* pSrc)
{
    Ped* pDst = gPedPool->Allocate();
    Ped* pNext = pDst->mpNext;
    memcpy(pDst, pSrc, sizeof(Ped));
    pDst->mpNext = pNext;

    if (pSrc->mGameObject)
    {
        pDst->AllocCharB4(pSrc->get_cam_x(), pSrc->get_cam_y(), pSrc->get_cam_z());
        pDst->SetRemap(pSrc->get_remap());
        pDst->SetRotation(pSrc->GetRotation());
        pDst->SetStateInitPending();
        pDst->mGameObject->mTargetCar = pSrc->mGameObject->mTargetCar;
    }
    return pDst;
}

MATCH_FUNC(0x471060)
void PedManager::DoIanTest(u16 key)
{
    switch (key)
    {
        case DIK_9:
            gGame_0x40->mLocalPlayer->mPlayerPed->SetOnFire();
            break;
        case DIK_8:
            gGame_0x40->mLocalPlayer->mPlayerPed->SetVisible();
            break;

        case DIK_7:
            gGame_0x40->mLocalPlayer->mPlayerPed->SetInvisible();
            break;
    }
}

MATCH_FUNC(0x4710c0)
Ped* PedManager::PedById(s32 pedId)
{
    for (Ped* pPedIter = gPedPool->GetFirstPed(); pPedIter; pPedIter = pPedIter->mpNext)
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
