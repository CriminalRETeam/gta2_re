#include "Police.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "EmergencyCrewPool.hpp"
#include "Object_5C.hpp"
#include "PathFinder.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "winmain.hpp"
#include "ped_graphic_type.hpp"

DEFINE_GLOBAL(Police*, gPolice_7B8, 0x6FEE40);
DEFINE_GLOBAL(s32, gCrewKind, 0x6FEDB8);
DEFINE_STATIC_MEMBER(u8, Police, sbHaveCriminals, 0x6FEE44);
DEFINE_STATIC_MEMBER(u16, Police, sIdCounter, 0x6FEE46);
DEFINE_STATIC_MEMBER(s32, Police, sDispatchCrewState, 0x6FEDCC);
DEFINE_STATIC_MEMBER(u32, Police, sDispatchCrewType, 0x6FEE18);
DEFINE_STATIC_MEMBER(s16, Police, sDispatchCrewSpawnDelay, 0x6FEAC8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6FEB68, Fix16(13107, 0), 0x6FEB68);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FECA0, Fix16(256, 0), 0x6FECA0);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FEB88, kFpOne64th_6FECA0, 0x6FEB88);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_6FECF8, Fix16(4), 0x6FECF8);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixteenth_6FEB0C, kFpFour_6FECF8* kFpOne64th_6FEB88, 0x6FEB0C);

EXTERN_GLOBAL(Fix16, kFpZero_6FECE8);

MATCH_FUNC(0x4BEB50)
Police::~Police()
{
}

MATCH_FUNC(0x56f400)
void Police::Init()
{
    mUnused0 = 1;
    for (s32 i = 0; i < MAX_PURSUIT_TARGETS; i++)
    {
        mPursuitTargets[i].mCriminalPed = NULL;
        mPursuitTargets[i].mTimer = 0;
        mPursuitTargets[i].mCrewGaveUp = false;

        mPursuitTargets[i].mNumPoliceCrews = 0;
        mPursuitTargets[i].mTargetPoliceCrews = 0;
        mPursuitTargets[i].mNumSwatCrews = 0;
        mPursuitTargets[i].mNumFbiCrews = 0;
        mPursuitTargets[i].mNumArmyCrews = 0;
        mPursuitTargets[i].mUnused76 = 0;

        mPursuitTargets[i].mWantedLevel = cop_level_enum::none_0;
        mPursuitTargets[i].mState = pursuit_state::idle_0;

        mPursuitTargets[i].mX = kFpZero_6FECE8;
        mPursuitTargets[i].mY = kFpZero_6FECE8;
        mPursuitTargets[i].mZ = kFpZero_6FECE8;

        mPursuitTargets[i].mCrewGaveUp = false;
        mPursuitTargets[i].mBlockCounter = 0;

        mPursuitTargets[i].mNumCrews = 0;
        mPursuitTargets[i].mIsActivelyChased = false;
        mPursuitTargets[i].mWantedTimer = 0;

        memset(mPursuitTargets[i].mCrews, 0, 0x18);
    }
    mMaxWantedLevel = cop_level_enum::none_0;
    mPoliceCarCount = 0;
    mMaxPoliceCars = 1;
    mHighestCrewTypeInPursuit = crew_type::police_3;
    sbHaveCriminals = 0;
    if (bStartNetworkGame)
    {
        mMaxWantedStars = 1;
    }
    else
    {
        mMaxWantedStars = 6;
    }
    mRoadblockCooldown = 100;
    mPolicePedsInRangeScreen = 0;
    mLastFiringEmergencyPed = NULL;
    mCrewPedOnscreen = false;
}

MATCH_FUNC(0x56f4d0)
bool Police::TryReplaceCrewLeaderOnDeath(Ped* pPed)
{
    for (u8 idx = 0; idx < MAX_POLICE_CREWS; idx++)
    {
        PoliceCrew* pCrew = &this->mCopCrew[idx];
        if (pCrew->mUsed)
        {
            if (pCrew->mSubObj->mPed == pPed)
            {
                bool bLeaderReplaced = pCrew->mSubObj->ReplaceLeaderIfNeeded();
                EmergencyCrew* pEmergencyCrew = pCrew->mSubObj;
                if (pCrew->mSubObj->mGroup)
                {
                    pEmergencyCrew->mPed = pEmergencyCrew->mGroup->mPedLeader;
                }
                if (bLeaderReplaced)
                {
                    return true;
                }
                else
                {
                    pCrew->mSubObj->mPed = NULL;
                    return false;
                }
            }
            else
            {
                if (pPed->mPedGroup)
                {
                    pPed->mPedGroup->RemovePed(pPed);
                }
                return false;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x56f560)
PoliceCrew* Police::NewCrew()
{
    for (u8 i = 0; i < MAX_POLICE_CREWS; i++)
    {
        if (!mCopCrew[i].mUsed)
        {
            PoliceCrew* pNew = &mCopCrew[i];
            pNew->Init();
            return pNew;
        }
    }
    return NULL;
}

MATCH_FUNC(0x56f5c0)
Ped* Police::SpawnRoadblockGuard(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Ped* pCop = NULL;

    if (gPedManager->mFbiArmyCount >= 30)
    {
        return NULL;
    }

    switch (gCrewKind)
    {
        case crew_kind::fbi_3:
            pCop = gPedManager->SpawnPedAt(xpos, ypos, zpos, 0, rotation);
            pCop->SetPedType_403920(ped_type::special_ped_4);
            pCop->set_occupation_403970(ped_ocupation_enum::roadblock_cop_37);
            pCop->SetObjective(objectives_enum::guard_spot_24, 0);
            pCop->set_remap_433B90(ped_remap_enum::ped_remap_zaibatsu);
            pCop->mGraphicType = ped_graphic_type::character_1;
            pCop->ForceWeapon(weapon_type::silence_smg);
            pCop->set_health_4039A0(200);
            pCop->mThreatSearch = threat_search_enum::area_2;
            pCop->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
            break;
        case crew_kind::police_1:
            pCop = gPedManager->SpawnPedAt(xpos, ypos, zpos, 0, rotation);
            pCop->SetPedType_403920(ped_type::special_ped_4);
            pCop->set_occupation_403970(ped_ocupation_enum::roadblock_cop_37);
            pCop->SetObjective(objectives_enum::guard_spot_24, 0);
            pCop->set_remap_433B90(ped_remap_enum::ped_remap_blue_police);
            pCop->mGraphicType = ped_graphic_type::cop_2;
            pCop->mSelectedWeapon = NULL;
            pCop->GiveWeapon(weapon_type::pistol);
            pCop->set_health_4039A0(200);
            pCop->mThreatSearch = threat_search_enum::area_2;
            pCop->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
            break;
    }
    return pCop;
}

MATCH_FUNC(0x56f6d0)
void Police::DespawnCrewInCar(Car* pCar)
{
    bool bOutranked = false;

    for (u8 idx = 0; idx < MAX_POLICE_CREWS; idx++)
    {
        if (mCopCrew[idx].mUsed)
        {
            PoliceCrew* pCrew = &mCopCrew[idx];
            if (mCopCrew[idx].mSubObj && mCopCrew[idx].mSubObj->mCar == pCar)
            {
                PedGroup* pPedGroup = pCrew->mSubObj->mGroup;

                if (!pPedGroup || pPedGroup->IsAllMembersInSomeCar())
                {
                    switch (pCrew->mpPursuitTarget->mWantedLevel)
                    {
                        case cop_level_enum::army_6:

                            if (pCrew->mCrewKind == crew_kind::police_1)
                            {
                                bOutranked = true;
                            }
                            if (pCrew->mCrewKind == crew_kind::swat_2)
                            {
                                bOutranked = true;
                            }
                            if (pCrew->mCrewKind == crew_kind::fbi_3)
                            {
                                bOutranked = true;
                            }

                            break;

                        case cop_level_enum::fbi_5:

                            if (pCrew->mCrewKind == crew_kind::police_1)
                            {
                                bOutranked = true;
                            }
                            if (pCrew->mCrewKind == crew_kind::swat_2)
                            {
                                bOutranked = true;
                            }
                            break;

                        default:
                            return;
                    }
                    if (!bOutranked)
                    {
                        return;
                    }
                }

                idx = 0;
                if (pCrew->mSubObj->mGroup)
                {
                    for (Ped* pPedIter = pCrew->mSubObj->mGroup->mPedList[idx]; pPedIter;
                         pPedIter = pCrew->mSubObj->mGroup->mPedList[++idx])
                    {
                        if (pPedIter->mGameObject)
                        {
                            pPedIter->Deallocate();
                        }
                    }
                }

                pCrew->RemoveFromPursuit();
                Car* pCrewCar = pCrew->mSubObj->mCar;

                pCrewCar->MarkForDespawn_421470();
                pCrew->mSubObj->mState = crew_state::clean_up_5;
                pCrew->mSubObj->mReady = true;
                pCrew->mState = police_crew_state::shutdown_6;
                return;
            }
        }
    }
}

MATCH_FUNC(0x56f800)
bool Police::IsBeingPursued(Ped* pCriminal) const
{
    for (u8 i = 0; i < MAX_PURSUIT_TARGETS; i++)
    {
        if (mPursuitTargets[i].mCriminalPed == pCriminal)
        {
            if (mPursuitTargets[i].mNumCrews > 0 && (mPursuitTargets[i].mState == pursuit_state::active_3 || mPursuitTargets[i].mTimer != 0))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x56f880)
bool Police::IsActivelyChased(Ped* pCriminal) const
{
    for (u8 i = 0; i < MAX_PURSUIT_TARGETS; i++)
    {
        if (mPursuitTargets[i].mCriminalPed == pCriminal)
        {
            if (mPursuitTargets[i].mIsActivelyChased)
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

MATCH_FUNC(0x56f8e0)
void Police::SetArrestedPed(Ped* pCriminal, Ped* pAuxPed)
{
    for (u8 i = 0; i < MAX_PURSUIT_TARGETS; i++)
    {
        if (mPursuitTargets[i].mCriminalPed == pCriminal)
        {
            mPursuitTargets[i].mCriminalPed = pCriminal;
            return;
        }
    }
}

MATCH_FUNC(0x56f940)
void Police::RegisterCriminal(Ped* pPed)
{
    sbHaveCriminals = 0;
    if (pPed->is_player_41B0A0())
    {
        bool bFound = false;
        for (u8 idx = 0; idx < GTA2_COUNTOF(mPursuitTargets); idx++)
        {
            if (mPursuitTargets[idx].mCriminalPed == pPed)
            {
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            for (u8 i = 0; i < GTA2_COUNTOF(mPursuitTargets); i++)
            {
                if (mPursuitTargets[i].mCriminalPed == NULL)
                {
                    mPursuitTargets[i].mCriminalPed = pPed;
                    mPursuitTargets[i].mState = pursuit_state::idle_0;
                    mPursuitTargets[i].mX = pPed->get_cam_x();
                    mPursuitTargets[i].mY = pPed->get_cam_y();
                    mPursuitTargets[i].mZ = pPed->get_cam_z();
                    break;
                }
            }
        }

        for (u8 j = 0; j < GTA2_COUNTOF(mPursuitTargets); j++)
        {
            if (mPursuitTargets[j].mCriminalPed != NULL)
            {
                sbHaveCriminals = 1;
                return;
            }
        }
    }
}

MATCH_FUNC(0x56fa40)
void Police::UpdateFirstPursuitTimer()
{
    if (mPursuitTargets[0].mCriminalPed)
    {
        if (!mPursuitTargets[0].mCriminalPed->IsActive_433B40() 
            || mPursuitTargets[0].mCriminalPed->isDead_403B60())
        {
            mPursuitTargets[0].mState = pursuit_state::ended_4;
        }
        else
        {
            if (mPursuitTargets[0].mTimer > 0)
            {
                mPursuitTargets[0].mTimer--;
            }

            if (mPursuitTargets[0].mState == pursuit_state::active_3 && mPursuitTargets[0].mTimer == 0)
            {
                mPursuitTargets[0].mState = pursuit_state::searching_5;
            }
        }
    }
}

MATCH_FUNC(0x56faa0)
bool Police::DispatchNewCrewToPursuit(PolicePursuitTarget* pPursuitTarget)
{
    u8 tileX = pPursuitTarget->mX.ToInt();
    u8 tileY = pPursuitTarget->mY.ToInt();
    u8 tileZ = pPursuitTarget->mZ.ToInt();

    if (gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &tileX, &tileY, &tileZ, false))
    {
        PoliceCrew* pNewPoliceCrew = Police::NewCrew();
        pNewPoliceCrew->mUsed = 1;
        pNewPoliceCrew->mTargX = tileX;
        pNewPoliceCrew->mTargY = tileY;
        pNewPoliceCrew->mTargZ = tileZ;
        EmergencyCrew* pNewEmergencyCrew = gEmergencyCrewPool->AllocateSlot();
        pNewPoliceCrew->mSubObj = pNewEmergencyCrew;
        if (!pNewPoliceCrew->mSubObj)
        {
            pNewPoliceCrew->Init();
            return false;
        }
        pNewPoliceCrew->mId = sIdCounter++;
        EmergencyCrew* pEmergencyCrew = pNewPoliceCrew->mSubObj;
        pNewPoliceCrew->mpPursuitTarget = pPursuitTarget;
        pNewPoliceCrew->mState = sDispatchCrewState;
        pNewPoliceCrew->mCrewKind = gCrewKind;
        pEmergencyCrew->mIsUsed = true;
        pEmergencyCrew->mCrewType = sDispatchCrewType;
        pEmergencyCrew->mPhase = crew_phase::in_car_1;
        pEmergencyCrew->mState = crew_state::spawn_car_3;
        pEmergencyCrew->mSpawnDelay = sDispatchCrewSpawnDelay;
        pEmergencyCrew->mSpawnX = Fix16(tileX);
        pEmergencyCrew->mSpawnY = Fix16(tileY);
        pEmergencyCrew->mSpawnZ = Fix16(tileZ);
        pNewPoliceCrew->AddToPursuit();
        return true;
    }
    return false;
}

DEFINE_GLOBAL_INIT(Fix16, kFpFive_6FECFC, Fix16(5), 0x6FECFC);

// Updates every pursuit target: its wanted level from the criminal's stars, then its state
// (send crews, escalate, give up, clean up when the criminal is gone).
MATCH_FUNC(0x56fbd0)
void Police::UpdatePursuitTargets()
{
    u8 numCrews;
    u8 crewIdx;
    u8 targetIdx = 0;
    PolicePursuitTarget* pPursuitTarget = &mPursuitTargets[0];
    while (pPursuitTarget->mCriminalPed && targetIdx < MAX_PURSUIT_TARGETS)
    {
        pPursuitTarget->mIsActivelyChased = false;
        Ped* pCriminal = pPursuitTarget->mCriminalPed;
        if (pCriminal->GetPedType_420B70() == ped_type::player_2 && pCriminal->mBf21C.bBusted == 1)
        {
            pCriminal->mPlayer->mBusted = true;
        }

        switch (pPursuitTarget->mCriminalPed->get_wanted_star_count())
        {
            case cop_level_enum::none_0:
                if (pPursuitTarget->mState != pursuit_state::idle_0)
                {
                    pPursuitTarget->mState = pursuit_state::ended_4;
                    mMaxPoliceCars = 2;
                    pPursuitTarget->mWantedLevel = cop_level_enum::none_0;
                    pPursuitTarget->mTargetPoliceCrews = 0;
                }
                break;
            case cop_level_enum::police_1:
                pPursuitTarget->mTargetPoliceCrews = 1;
                pPursuitTarget->mWantedLevel = cop_level_enum::police_1;
                if (pPursuitTarget->mNumPoliceCrews < 1)
                {
                    mMaxPoliceCars = 1;
                }
                else
                {
                    mMaxPoliceCars = 0;
                }
                break;
            case cop_level_enum::police_2:
                if (pPursuitTarget->mTargetPoliceCrews == 1)
                {
                    pPursuitTarget->mBlockCounter = 1;
                }
                pPursuitTarget->mTargetPoliceCrews = 2;
                pPursuitTarget->mWantedLevel = cop_level_enum::police_2;
                if (pPursuitTarget->mNumPoliceCrews <= 1)
                {
                    mMaxPoliceCars = 2;
                }
                else
                {
                    mMaxPoliceCars = 0;
                }
                break;
            case cop_level_enum::police_3:
                pPursuitTarget->mTargetPoliceCrews = 2;
                pPursuitTarget->mWantedLevel = cop_level_enum::police_3;
                if (pPursuitTarget->mNumPoliceCrews <= 1)
                {
                    mMaxPoliceCars = 2;
                }
                else
                {
                    mMaxPoliceCars = 0;
                }
                break;
            case cop_level_enum::swat_4:
                pPursuitTarget->mTargetPoliceCrews = 2;
                pPursuitTarget->mWantedLevel = cop_level_enum::swat_4;
                if (pPursuitTarget->mNumPoliceCrews <= 1)
                {
                    mMaxPoliceCars = 2;
                }
                else
                {
                    mMaxPoliceCars = 0;
                }
                break;
            case cop_level_enum::fbi_5:
                pPursuitTarget->mWantedLevel = cop_level_enum::fbi_5;
                gPolice_7B8->mHighestCrewTypeInPursuit = crew_type::fbi_4;
                if (!pPursuitTarget->mNumPoliceCrews && !pPursuitTarget->mNumSwatCrews && pPursuitTarget->mNumFbiCrews <= 1)
                {
                    mMaxPoliceCars = 2;
                }
                else
                {
                    mMaxPoliceCars = 0;
                }
                break;
            case cop_level_enum::army_6:
                pPursuitTarget->mTargetPoliceCrews = 0;
                pPursuitTarget->mWantedLevel = cop_level_enum::army_6;
                gPolice_7B8->mHighestCrewTypeInPursuit = crew_type::army_6;
                break;
        }

        if (pPursuitTarget->mWantedLevel > mMaxWantedLevel)
        {
            mMaxWantedLevel = pPursuitTarget->mWantedLevel;
        }

        pCriminal = pPursuitTarget->mCriminalPed;
        if (!pCriminal->IsActive_433B40() || pCriminal->isDead_403B60())
        {
            pPursuitTarget->mState = pursuit_state::ended_4;
        }

        if (pPursuitTarget->mWantedLevel == cop_level_enum::police_1)
        {
            if (!IsBeingPursued(pPursuitTarget->mCriminalPed))
            {
                if (static_cast<u16>(pPursuitTarget->mWantedTimer) >= 900)
                {
                    pPursuitTarget->mCriminalPed->ClearWantedPoints_420B80();
                    pPursuitTarget->mCriminalPed->mWantedPoints = cop_level_ped_enum::cop_0_stars;
                    pPursuitTarget->mWantedTimer = 0;
                    return;
                }
                pPursuitTarget->mWantedTimer++;
            }
            else
            {
                pPursuitTarget->mWantedTimer = 0;
            }
        }

        switch (pPursuitTarget->mState)
        {
            case pursuit_state::idle_0:
                if (pPursuitTarget->mWantedLevel > 0)
                {
                    if (!pPursuitTarget->mNumPoliceCrews)
                    {
                        if (!mPoliceCarCount)
                        {
                            sDispatchCrewSpawnDelay = 200;
                            sDispatchCrewType = crew_type::police_3;
                            sDispatchCrewState = police_crew_state::alerted_search_3;
                            gCrewKind = crew_kind::police_1;
                            if (gPolice_7B8->DispatchNewCrewToPursuit(pPursuitTarget))
                            {
                                pPursuitTarget->mState = pursuit_state::responding_1;
                                mMaxPoliceCars = 0;
                            }
                        }
                    }
                    else
                    {
                        pPursuitTarget->mState = pursuit_state::active_3;
                    }
                }
                break;

            case pursuit_state::responding_1:
                mMaxPoliceCars = 0;
                if (pPursuitTarget->mTimer == 250)
                {
                    pPursuitTarget->mState = pursuit_state::active_3;
                    for (crewIdx = 0; crewIdx < pPursuitTarget->mNumCrews; crewIdx++)
                    {
                        pPursuitTarget->mCrews[crewIdx]->mState = police_crew_state::pursue_or_chase_5;
                    }
                }
                break;

            case pursuit_state::active_3:
                pPursuitTarget->mCrewGaveUp = false;
                switch (pPursuitTarget->mWantedLevel)
                {
                    case cop_level_enum::police_3:
                        mMaxPoliceCars = 0;
                        if (pPursuitTarget->mNumPoliceCrews < pPursuitTarget->mTargetPoliceCrews)
                        {
                            sDispatchCrewType = crew_type::police_3;
                            sDispatchCrewSpawnDelay = 50;
                            sDispatchCrewState = police_crew_state::pursue_or_chase_5;
                            gCrewKind = crew_kind::police_1;
                            gPolice_7B8->DispatchNewCrewToPursuit(pPursuitTarget);
                        }
                        break;
                    case cop_level_enum::swat_4:
                        if (pPursuitTarget->mNumPoliceCrews < pPursuitTarget->mTargetPoliceCrews)
                        {
                            sDispatchCrewType = crew_type::police_3;
                            sDispatchCrewSpawnDelay = 50;
                            sDispatchCrewState = police_crew_state::pursue_or_chase_5;
                            gCrewKind = crew_kind::police_1;
                            gPolice_7B8->DispatchNewCrewToPursuit(pPursuitTarget);
                        }
                        if (!pPursuitTarget->mNumSwatCrews)
                        {
                            sDispatchCrewSpawnDelay = 50;
                            sDispatchCrewType = crew_type::swat_5;
                            sDispatchCrewState = police_crew_state::pursue_or_chase_5;
                            gCrewKind = crew_kind::swat_2;
                            if (gPolice_7B8->DispatchNewCrewToPursuit(pPursuitTarget))
                            {
                                pPursuitTarget->mNumSwatCrews = 1;
                            }
                        }
                        break;
                    case cop_level_enum::fbi_5:
                        if (pPursuitTarget->mNumPoliceCrews > 0)
                        {
                            numCrews = pPursuitTarget->mNumCrews;
                            for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                            {
                                pPursuitTarget->mCrews[0]->mIsDismissed = true;
                                pPursuitTarget->mCrews[0]->RemoveFromPursuit();
                            }
                            pPursuitTarget->mNumPoliceCrews = 0;
                            pPursuitTarget->mTargetPoliceCrews = 0;
                            pPursuitTarget->mNumSwatCrews = 0;
                            gPolice_7B8->mHighestCrewTypeInPursuit = crew_type::fbi_4;
                        }
                        break;
                    case cop_level_enum::army_6:
                        if (pPursuitTarget->mNumPoliceCrews > 0 || pPursuitTarget->mNumSwatCrews || pPursuitTarget->mNumFbiCrews)
                        {
                            numCrews = pPursuitTarget->mNumCrews;
                            for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                            {
                                pPursuitTarget->mCrews[0]->RemoveFromPursuit();
                            }
                            pPursuitTarget->mNumPoliceCrews = 0;
                            pPursuitTarget->mTargetPoliceCrews = 0;
                            sbHaveCriminals--;
                            gPolice_7B8->mHighestCrewTypeInPursuit = crew_type::army_6;
                        }
                        break;
                }

                if (pPursuitTarget->mNumPoliceCrews > pPursuitTarget->mTargetPoliceCrews)
                {
                    numCrews = pPursuitTarget->mNumCrews;
                    for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                    {
                        PoliceCrew* pCrew = pPursuitTarget->mCrews[crewIdx];
                        if (pCrew && pCrew->mUsed == 1 && pCrew->mCrewKind == crew_kind::police_1 && pCrew->mSubObj->mCar)
                        {
                            pPursuitTarget->mCrews[crewIdx]->mIsDismissed = true;
                            pPursuitTarget->mCrews[crewIdx]->RemoveFromPursuit();
                            break;
                        }
                    }
                }
                break;

            case pursuit_state::searching_5:
                if (pPursuitTarget->mTimer > 0)
                {
                    pPursuitTarget->mState = pursuit_state::active_3;
                }
                else if (pPursuitTarget->mWantedLevel == cop_level_enum::fbi_5)
                {
                    pPursuitTarget->mState = pursuit_state::active_3;
                }
                else if (!pPursuitTarget->mCrewGaveUp)
                {
                    bool bNoneSearching = true;
                    numCrews = pPursuitTarget->mNumCrews;
                    for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                    {
                        PoliceCrew* pCrew = pPursuitTarget->mCrews[crewIdx];
                        if (pCrew && pCrew->mState == police_crew_state::alerted_search_3)
                        {
                            pCriminal = pPursuitTarget->mCriminalPed;
                            if (pCriminal)
                            {
                                if (Fix16::MaxAbsDistance_42A6B0(pPursuitTarget->mX,
                                                                 pPursuitTarget->mY,
                                                                 pCriminal->get_cam_x(),
                                                                 pCriminal->get_cam_y()) > kFpFive_6FECFC)
                                {
                                    pCrew->RemoveFromPursuit();
                                    pPursuitTarget->mCrewGaveUp = true;
                                }
                            }
                            bNoneSearching = false;
                        }
                    }
                    if (bNoneSearching && !mPoliceCarCount)
                    {
                        sDispatchCrewSpawnDelay = 200;
                        sDispatchCrewType = crew_type::police_3;
                        sDispatchCrewState = police_crew_state::alerted_search_3;
                        gCrewKind = crew_kind::police_1;
                        gPolice_7B8->DispatchNewCrewToPursuit(pPursuitTarget);
                    }
                }
                break;

            case pursuit_state::ended_4:
            {
                numCrews = pPursuitTarget->mNumCrews;
                for (crewIdx = 0; crewIdx < numCrews; crewIdx++)
                {
                    PoliceCrew* pCrew = pPursuitTarget->mCrews[0];
                    if (!pCrew->mUsed)
                    {
                        if (pPursuitTarget->mNumCrews > 0)
                        {
                            pPursuitTarget->mCrews[0] = pPursuitTarget->mCrews[pPursuitTarget->mNumCrews - 1];
                            pPursuitTarget->mCrews[pPursuitTarget->mNumCrews - 1] = NULL;
                            pPursuitTarget->mNumCrews--;
                        }
                        else
                        {
                            pPursuitTarget->mCrews[0] = NULL;
                        }
                    }
                    else
                    {
                        pCrew->RemoveFromPursuit();
                    }
                }
                pPursuitTarget->mCriminalPed = NULL;
                pPursuitTarget->mNumPoliceCrews = 0;
                pPursuitTarget->mTargetPoliceCrews = 0;
                pPursuitTarget->mNumCrews = 0;
                pPursuitTarget->mUnused76 = 0;
                pPursuitTarget->mBlockCounter = 0;
                sbHaveCriminals--;
                gPolice_7B8->mHighestCrewTypeInPursuit = crew_type::police_3;
                break;
            }
        }

        if (++targetIdx < MAX_PURSUIT_TARGETS)
        {
            pPursuitTarget = &mPursuitTargets[targetIdx];
        }
    }
}

MATCH_FUNC(0x570270)
void Police::Service()
{
    mCrewPedOnscreen = false;
    mMaxWantedLevel = cop_level_enum::none_0;

    if (sbHaveCriminals == 1)
    {
        Police::UpdatePursuitTargets();
    }

    for (s32 i = 0; i < GTA2_COUNTOF(mCopCrew); i++)
    {
        if (mCopCrew[i].mUsed == 1)
        {
            mCopCrew[i].Service();
        }
    }

    if (sbHaveCriminals == 1)
    {
        Police::UpdateFirstPursuitTimer();
    }

    mRoadblock1.Update();
    mRoadblock2.Update();

    if (mRoadblockCooldown > 0)
    {
        mRoadblockCooldown--;
    }

    if (mLastFiringEmergencyPed != NULL)
    {
        if (mLastFiringEmergencyPed->GetPedState_403990() == ped_state_1::dead_9)
        {
            mLastFiringEmergencyPed = NULL;
        }
        else if (!mLastFiringEmergencyPed->IsActive_433B40())
        {
            mLastFiringEmergencyPed = NULL;
        }
        else if (mLastFiringEmergencyPed->mBf21C.bAttacking == 0)
        {
            mLastFiringEmergencyPed = NULL;
        }
    }
}

MATCH_FUNC(0x570320)
void Police::SpawnWalkingGuard(Ped* pPed, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    if (mHighestCrewTypeInPursuit == crew_type::army_6)
    {
        pPed->set_occupation_403970(ped_ocupation_enum::walking_army_guard_31);
        pPed->SetPedType_403920(ped_type::dummy_3);
        pPed->set_remap_433B90(ped_remap_enum::ped_remap_army);
    }
    else
    {
        pPed->set_occupation_403970(ped_ocupation_enum::walking_guard_29);
        pPed->SetPedType_403920(ped_type::dummy_3);
        pPed->set_remap_433B90(ped_remap_enum::ped_remap_blue_police);
    }
    pPed->mGraphicType = ped_graphic_type::cop_2;
    pPed->mThreatSearch = threat_search_enum::line_of_sight_1;
    pPed->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
    pPed->AllocCharB4(xpos, ypos, zpos);

    pPed->SetRemap_433C10(pPed->get_remap_433BA0());
    pPed->SetRotation_433C00(rotation);
    pPed->ResetAnimAndFindNearestSprite();
}

MATCH_FUNC(0x5703e0)
bool Police::SpawnCrewInCar(Car* pCar)
{
    if (gPedManager->mFbiArmyCount >= 30)
    {
        return false;
    }
    if (mPoliceCarCount > 2)
    {
        return false;
    }
    PoliceCrew* pNewCrew = Police::NewCrew();
    pNewCrew->mUsed = 1;
    EmergencyCrew* pNewEmergencyCrew = gEmergencyCrewPool->AllocateSlot();
    pNewCrew->mSubObj = pNewEmergencyCrew;
    if (!pNewEmergencyCrew)
    {
        pNewCrew->Init();
        return false;
    }
    pNewCrew->mId = sIdCounter++;
    EmergencyCrew* pEmergencyCrew = pNewCrew->mSubObj;
    pNewCrew->mState = police_crew_state::patrol_1;
    pNewCrew->mbCountedInPoliceCount = true;
    pEmergencyCrew->mIsUsed = true;
    pEmergencyCrew->mCrewType = gPolice_7B8->mHighestCrewTypeInPursuit;
    pEmergencyCrew->mPhase = crew_phase::in_car_1;
    pEmergencyCrew->mCar = pCar;
    PedGroup* pNewPedGroup = PedGroup::New();
    Ped* pNewPed1 = gPedManager->AllocatePed();
    pNewPed1->SetPedType_403920(ped_type::special_ped_4);
    pNewPed1->set_occupation_403970(ped_ocupation_enum::police);
    pNewPed1->SpawnPedInCar(pEmergencyCrew->mCar);
    pNewPed1->SetObjective(objectives_enum::patrol_in_car_43);
    pNewPed1->mThreatSearch = threat_search_enum::line_of_sight_1;
    pNewPed1->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
    Ped* pNewPed2 = gPedManager->AllocatePed();
    pNewPed2->SetObjective(objectives_enum::no_obj_0);
    pNewPed2->EnterCarAsPassenger(pEmergencyCrew->mCar);
    pNewPed2->SetPedType_403920(ped_type::special_ped_4);
    pNewPed2->set_occupation_403970(ped_ocupation_enum::police);
    pNewPed2->mThreatSearch = threat_search_enum::line_of_sight_1;
    pNewPed2->mThreatReaction = threat_reaction_enum::react_as_emergency_1;

    switch (gPolice_7B8->mHighestCrewTypeInPursuit)
    {
        case crew_type::fbi_4:
            pNewPed1->set_health_4039A0(250);
            pNewPed1->ForceWeapon(weapon_type::shotgun);
            pNewPed1->GiveWeapon(weapon_type::silence_smg);
            pNewPed1->set_occupation_403970(ped_ocupation_enum::fbi);
            pNewPed1->set_remap_433B90(ped_remap_enum::ped_remap_zaibatsu);
            pNewPed1->mGraphicType = ped_graphic_type::character_1;
            pNewPed2->set_health_4039A0(250);
            pNewPed2->set_remap_433B90(ped_remap_enum::ped_remap_zaibatsu);
            pNewPed2->ForceWeapon(weapon_type::silence_smg);
            pNewPed2->mGraphicType = ped_graphic_type::character_1;
            pNewPed2->set_occupation_403970(ped_ocupation_enum::fbi);
            pNewCrew->mCrewKind = crew_kind::fbi_3;
            break;

        case crew_type::police_3:

            switch (mMaxWantedLevel)
            {
                case cop_level_enum::none_0:
                case cop_level_enum::police_1:
                    pNewPed1->mSelectedWeapon = NULL;
                    pNewPed1->GiveWeapon(weapon_type::pistol);
                    pNewPed1->set_health_4039A0(50);
                    pNewPed1->mMaxSpeed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
                    pNewPed2->mSelectedWeapon = NULL;
                    pNewPed2->GiveWeapon(weapon_type::pistol);
                    pNewPed2->set_health_4039A0(50);
                    pNewPed2->mMaxSpeed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;

                    break;

                case cop_level_enum::police_2:
                    pNewPed1->GiveWeapon(weapon_type::pistol);
                    pNewPed1->set_health_4039A0(100);
                    pNewPed1->mMaxSpeed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
                    pNewPed2->GiveWeapon(weapon_type::pistol);
                    pNewPed2->set_health_4039A0(100);

                    pNewPed2->mMaxSpeed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;

                    break;

                default:
                            pNewPed1->GiveWeapon(weapon_type::pistol);
                    pNewPed1->set_health_4039A0(100);
                    pNewPed2->GiveWeapon(weapon_type::pistol);
                    pNewPed2->set_health_4039A0(100);

                    break;
            }

            pNewPed1->set_occupation_403970(ped_ocupation_enum::police);
            pNewPed1->set_remap_433B90(ped_remap_enum::ped_remap_blue_police);
            pNewPed1->mGraphicType = ped_graphic_type::cop_2;
            pNewPed2->set_remap_433B90(ped_remap_enum::ped_remap_blue_police);
            pNewPed2->mGraphicType = ped_graphic_type::cop_2;
            pNewCrew->mCrewKind = crew_kind::police_1;

            break;

        default:
            pNewPed1->set_health_4039A0(250);
            pNewPed1->ForceWeapon(weapon_type::smg);
            pNewPed1->set_remap_433B90(ped_remap_enum::ped_remap_army);
            pNewPed1->set_occupation_403970(ped_ocupation_enum::army_army);
            pNewPed1->mGraphicType = ped_graphic_type::cop_2;
            pNewPed2->set_health_4039A0(250);
            pNewPed2->ForceWeapon(weapon_type::smg);
            pNewPed2->set_remap_433B90(ped_remap_enum::ped_remap_army);
            pNewPed2->set_occupation_403970(ped_ocupation_enum::army_army);
            pNewPed2->mGraphicType = ped_graphic_type::cop_2;
            pNewCrew->mCrewKind = crew_kind::army_4;
            break;
    }

    pNewPedGroup->add_ped_leader(pNewPed1);
    pNewPedGroup->SetCounts_433360(1);
    pNewPedGroup->add_ped_to_list(pNewPed2, 0);
    pNewPedGroup->mRemoveDeadMembers = false;
    pEmergencyCrew->mPed = pNewPed1;
    pEmergencyCrew->mSpawnDelay = 0;
    pEmergencyCrew->mState = crew_state::update_6;
    pEmergencyCrew->mCar->SetDriverPedType_421560(ped_type::script_created_5);
    pEmergencyCrew->mCar->InitCarAIControl();
    pEmergencyCrew->mCar->ResumeAIDriving();
    ++mPoliceCarCount;
    return true;
}

MATCH_FUNC(0x570790)
bool Police::AssignCrewToPursuit(PoliceCrew* pCrew, PolicePursuitTarget* pPursuitTarget)
{
    pCrew->mpPursuitTarget = pPursuitTarget;
    pCrew->mState = police_crew_state::pursue_or_chase_5;
    pCrew->AddToPursuit();
    return true;
}

// https://decomp.me/scratch/pfRaI
// Inlined search helpers: their NULL results are tested by the caller
static inline PolicePursuitTarget* FindPursuitTargetForCriminal_5707B0(Police* pThis, Ped* pCriminal)
{
    for (u8 i = 0; i < GTA2_COUNTOF(pThis->mPursuitTargets); i++)
    {
        if (pThis->mPursuitTargets[i].mCriminalPed == pCriminal)
        {
            return &pThis->mPursuitTargets[i];
        }
    }
    return NULL;
}

static inline PoliceCrew* FindCrewInCar_5707B0(Police* pThis, Car* pCar)
{
    for (u8 j = 0; j < GTA2_COUNTOF(pThis->mCopCrew); j++)
    {
        PoliceCrew* pCrew = &pThis->mCopCrew[j];
        if (pCrew->mUsed && pCrew->mSubObj->mCar == pCar)
        {
            return pCrew;
        }
    }
    return NULL;
}

MATCH_FUNC(0x5707b0)
bool Police::TryAssignCarCrewToCriminal(Car* pCar, Ped* pCriminal)
{
    if (!pCriminal->is_player_41B0A0())
    {
        return false;
    }

    PolicePursuitTarget* pPursuitTarget = FindPursuitTargetForCriminal_5707B0(this, pCriminal);
    if (pPursuitTarget == NULL)
    {
        return false;
    }

    PoliceCrew* pCrew = FindCrewInCar_5707B0(this, pCar);
    if (pCrew == NULL)
    {
        return false;
    }

    if (pCrew->mSubObj->mCrewType != crew_type::army_6 && pPursuitTarget->mWantedLevel == cop_level_enum::army_6)
    {
        return false;
    }

    pPursuitTarget->mState = pursuit_state::active_3;
    pCrew->mpPursuitTarget = pPursuitTarget;
    pCrew->mState = police_crew_state::pursue_or_chase_5;
    pCrew->AddToPursuit();

    if (pCrew->mSubObj->mCrewType != crew_type::army_6)
    {
        pCrew->mSubObj->mCar->ActivateEmergencyLights();
    }

    return true;
}

MATCH_FUNC(0x5708c0)
void Police::UpdateLastSeenCoordsForCriminal(Ped* pPed)
{
    for (u8 i = 0; i < MAX_PURSUIT_TARGETS; i++)
    {
        if (mPursuitTargets[i].mCriminalPed == pPed)
        {
            mPursuitTargets[i].mX = pPed->get_cam_x();
            mPursuitTargets[i].mY = pPed->get_cam_y();
            mPursuitTargets[i].mZ = pPed->get_cam_z();
            mPursuitTargets[i].mTimer = 250;
            return;
        }
    }
}

MATCH_FUNC(0x570940)
void Police::UpdateCriminalLatestPosition(Ped* pPed)
{
    for (u8 i = 0; i < MAX_PURSUIT_TARGETS; i++)
    {
        if (mPursuitTargets[i].mCriminalPed == pPed)
        {
            mPursuitTargets[i].mX = pPed->get_cam_x();
            mPursuitTargets[i].mY = pPed->get_cam_y();
            mPursuitTargets[i].mZ = pPed->get_cam_z();
            return;
        }
    }
}

MATCH_FUNC(0x577320)
bool Police::TryBeginRoadblock()
{
    if (this->mMaxWantedLevel < cop_level_enum::police_3 || this->mRoadblock1.mbActive || this->mRoadblockCooldown)
    {
        return false;
    }
    this->mRoadblockCooldown = 40;
    return true;
}

MATCH_FUNC(0x577370)
void Police::TryCreateRoadblockAt(u8 tileX, u8 tileY, s32 roadblock_type)
{
    bool bBothSides = false;
    switch (mMaxWantedLevel)
    {
        case cop_level_enum::police_3:
            gCrewKind = crew_kind::police_1;
            break;
        case cop_level_enum::swat_4:
            gCrewKind = crew_kind::police_1;
            break;
        case cop_level_enum::fbi_5:
            gCrewKind = crew_kind::fbi_3;
            break;
        case cop_level_enum::army_6:
            gCrewKind = crew_kind::army_4;
            break;
    }

    if (roadblock_type > 0 && roadblock_type <= 2)
    {
        bBothSides = true;
    }

    u8 tileZ = gMap_0x370->FindGroundZForCoord(tileX, static_cast<s32>(tileY)).ToUInt8();

    if (bBothSides)
    {
        if (!mRoadblock1.mbActive)
        {
            mRoadblock1.CreateRoadblock(tileX, tileY, tileZ, 3);
        }
        else if (!mRoadblock2.mbActive)
        {
            mRoadblock2.CreateRoadblock(tileX, tileY, tileZ, 3);
        }
    }
    else
    {
        if (!mRoadblock1.mbActive)
        {
            mRoadblock1.CreateRoadblock(tileX, tileY, tileZ, 2);
        }
        else if (!mRoadblock2.mbActive)
        {
            mRoadblock2.CreateRoadblock(tileX, tileY, tileZ, 3);
        }
    }
}