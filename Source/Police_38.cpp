#include "Police_38.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Fix16_Rect.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "CarChaseTaskTable.hpp"
#include "EmergencyCrewPool.hpp"
#include "Object_5C.hpp"
#include "PathFinder.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "SpriteGrid.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "RouteFinder.hpp"
#include "CarAI.hpp"
#include "ped_graphic_type.hpp"

DEFINE_GLOBAL(Fix16, kFpZero_6FECE8, 0x6FECE8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpTen, Fix16(10), 0x6FED54);
DEFINE_STATIC_MEMBER(Ped*, PoliceCrew, sCurrentCrewPed, 0x6FEDDC);
DEFINE_GLOBAL(u8, gPoliceCrew_Unused, 0x6FEB48);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpEight, Fix16(8), 0x6FED48);

MATCH_FUNC(0x4beb30)
PoliceCrew::PoliceCrew()
{
    Init();
}

MATCH_FUNC(0x4beb40)
PoliceCrew::~PoliceCrew()
{
}

MATCH_FUNC(0x5709c0)
void PoliceCrew::Init()
{
    mTargX = 0;
    mTargY = 0;
    mTargZ = 0;
    mUnused18 = 0;
    mUsed = 0;
    mUnused1A = 0;
    mSubObj = NULL;
    mState = police_crew_state::none_0;
    mUseCar = false;
    mCriminalDistance = kFpZero_6FECE8;
    mUnusedC = kFpZero_6FECE8;
    mpPursuitTarget = NULL;
    mUnused1A = 0;
    mbCountedInPoliceCount = false;
    mUnused2A = 0;
    mUnused2C = 0;
    mPrevTargetPed = NULL;
    mIsDismissed = false;
}

MATCH_FUNC(0x570a10)
void PoliceCrew::AddToPursuit()
{
    if (mpPursuitTarget->mNumCrews < 6)
    {
        for (u8 i = 0; i < 6; i++)
        {
            if (mpPursuitTarget->mCrews[i] == this)
            {
                return;
            }
        }
        mpPursuitTarget->mCrews[mpPursuitTarget->mNumCrews] = this;
        ++mpPursuitTarget->mNumCrews;
        switch (mSubObj->mCrewType)
        {
            case crew_type::police_3:
                ++mpPursuitTarget->mNumPoliceCrews;
                break;
            case crew_type::swat_5:
                ++mpPursuitTarget->mNumSwatCrews;
                break;
            case crew_type::fbi_4:
                ++mpPursuitTarget->mNumFbiCrews;
                break;
            case crew_type::army_6:
                ++mpPursuitTarget->mNumArmyCrews;
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x570ab0)
void PoliceCrew::RemoveFromPursuit()
{
    if (!mUsed || (mState != police_crew_state::none_0 
        && mState != police_crew_state::patrol_1 
        && mState != police_crew_state::shutdown_6))
    {
        u8 last_idx = mpPursuitTarget->mNumCrews - 1;
        PoliceCrew* pLastCrew = mpPursuitTarget->mCrews[last_idx];
        if (pLastCrew == this)
        {
            mpPursuitTarget->mCrews[last_idx] = NULL;
        }
        else
        {
            for (u8 i = 0; i < last_idx; i++)
            {
                if (mpPursuitTarget->mCrews[i] == this)
                {
                    mpPursuitTarget->mCrews[i] = pLastCrew;
                    mpPursuitTarget->mCrews[last_idx] = NULL;
                    break;
                }
            }
        }
        if (mUsed)
        {
            switch (mCrewKind)
            {
                case crew_kind::police_1:
                    --mpPursuitTarget->mNumPoliceCrews;
                    break;
                case crew_kind::swat_2:
                    --mpPursuitTarget->mNumSwatCrews;
                    break;
                case crew_kind::fbi_3:
                    --mpPursuitTarget->mNumFbiCrews;
                    break;
                case crew_kind::army_4:
                    --mpPursuitTarget->mNumArmyCrews;
                    break;
                default:
                    break;
            }
        }
        --mpPursuitTarget->mNumCrews;
        mState = police_crew_state::shutdown_6;
    }
}

MATCH_FUNC(0x570bf0)
void PoliceCrew::SpawnPoliceInCar()
{
    PedGroup* pGroup = PedGroup::New();
    Ped* pCopLeader = gPedManager->AllocatePed();
    pCopLeader->SetPedType(ped_type::special_ped_4);
    pCopLeader->set_occupation(ped_ocupation_enum::police);
    pCopLeader->SpawnPedInCar(mSubObj->mCar);
    pCopLeader->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pCopLeader->mObjectiveTargetX = Fix16(mTargX);
    pCopLeader->mObjectiveTargetY = Fix16(mTargY);
    pCopLeader->mObjectiveTargetZ = Fix16(mTargZ);
    pCopLeader->set_remap(ped_remap_enum::ped_remap_blue_police);
    pCopLeader->mGraphicType = ped_graphic_type::cop_2;

    s32 wanted_level = gPolice_7B8->mMaxWantedLevel;

    switch (wanted_level)
    {
        case cop_level_enum::none_0:
        case cop_level_enum::police_1:
            pCopLeader->mSelectedWeapon = NULL;
            pCopLeader->GiveWeapon(weapon_type::pistol);
            pCopLeader->set_health(50);
            pCopLeader->mMaxSpeed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
            break;
        case cop_level_enum::police_2:
            pCopLeader->GiveWeapon(weapon_type::pistol);
            pCopLeader->set_health(100);
            pCopLeader->mMaxSpeed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
            break;
        default:
            pCopLeader->GiveWeapon(weapon_type::pistol);
            pCopLeader->set_health(100);
            break;
    }

    pCopLeader->mThreatSearch = threat_search_enum::line_of_sight_1;
    pCopLeader->mThreatReaction = threat_reaction_enum::react_as_emergency_1;

    Ped* pCopSupporter = gPedManager->AllocatePed();
    pCopSupporter->EnterCarAsPassenger(mSubObj->mCar);
    pCopSupporter->SetPedType(ped_type::special_ped_4);
    pCopSupporter->set_occupation(ped_ocupation_enum::police);
    pCopSupporter->SetObjective(objectives_enum::no_obj_0);
    pCopSupporter->set_remap(ped_remap_enum::ped_remap_blue_police);

    switch (mpPursuitTarget->mWantedLevel)
    {
        case cop_level_enum::police_1:
            pCopSupporter->mSelectedWeapon = NULL;
            pCopSupporter->GiveWeapon(weapon_type::pistol);
            pCopSupporter->mHealth = 50;
            pCopSupporter->mMaxSpeed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
            break;
        case cop_level_enum::police_2:
            pCopSupporter->GiveWeapon(weapon_type::pistol);
            pCopSupporter->mHealth = 100;
            pCopSupporter->mMaxSpeed = kFpOneSixteenth_6FEB0C * kFpPoint8_6FEB68;
            break;
        default:
            pCopSupporter->GiveWeapon(weapon_type::pistol);
            pCopSupporter->mHealth = 100;
            break;
    }

    pCopSupporter->mThreatSearch = threat_search_enum::line_of_sight_1;
    pCopSupporter->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
    pCopSupporter->mGraphicType = ped_graphic_type::cop_2;
    pGroup->add_ped_leader(pCopLeader);
    pGroup->SetCounts(1);
    pGroup->add_ped_to_list(pCopSupporter, 0);
    pGroup->mRemoveDeadMembers = false;
    mSubObj->mPed = pCopLeader;
    mSubObj->mState = crew_state::update_6;
    mSubObj->mCar->SetDriverPedType(ped_type::script_created_5);
    mSubObj->mCar->InitCarAIControl();
    mSubObj->mCar->ResumeAIDriving();
    mSubObj->mCar->ActivateEmergencyLights();
    mSubObj->mGroup = pGroup;
}

MATCH_FUNC(0x570e30)
void PoliceCrew::SpawnSWAT()
{
    PedGroup* pSwatGroup = PedGroup::New();
    Ped* pSwatLeader = gPedManager->AllocatePed();
    pSwatLeader->SetPedType(ped_type::special_ped_4);
    pSwatLeader->set_occupation(ped_ocupation_enum::swat);
    pSwatLeader->SpawnPedInCar(mSubObj->mCar);
    pSwatLeader->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pSwatLeader->mObjectiveTargetX = Fix16(mTargX);
    pSwatLeader->mObjectiveTargetY = Fix16(mTargY);
    pSwatLeader->mObjectiveTargetZ = Fix16(mTargZ);
    pSwatLeader->set_remap(-1);
    pSwatLeader->ForceWeapon(weapon_type::pistol);
    pSwatLeader->set_health(400);
    pSwatLeader->mThreatSearch = threat_search_enum::line_of_sight_1;
    pSwatLeader->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
    pSwatLeader->mGraphicType = ped_graphic_type::cop_2;
    pSwatGroup->add_ped_leader(pSwatLeader);
    pSwatGroup->SetCounts(3);
    for (u8 i = 0; i < 3; ++i)
    {
        Ped* pSwatMember = gPedManager->AllocatePed();
        pSwatMember->EnterCarAsPassenger(mSubObj->mCar);
        pSwatMember->SetPedType(ped_type::special_ped_4);
        pSwatMember->set_occupation(ped_ocupation_enum::swat);
        pSwatMember->SetObjective(objectives_enum::no_obj_0);
        pSwatMember->set_remap(-1);
        pSwatMember->ForceWeapon(weapon_type::pistol);
        pSwatMember->set_health(400);
        pSwatMember->mThreatSearch = threat_search_enum::line_of_sight_1;
        pSwatMember->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
        pSwatMember->mGraphicType = ped_graphic_type::cop_2;

        pSwatGroup->add_ped_to_list(pSwatMember, i);
    }
    pSwatGroup->mRemoveDeadMembers = false;
    mSubObj->mPed = pSwatLeader;
    mSubObj->mState = crew_state::update_6;
    mSubObj->mCar->SetDriverPedType(ped_type::script_created_5);
    mSubObj->mCar->InitCarAIControl();
    mSubObj->mCar->ResumeAIDriving();
    mSubObj->mCar->ActivateEmergencyLights();
    mSubObj->mGroup = pSwatGroup;
}

MATCH_FUNC(0x571150)
void PoliceCrew::SpawnFBI_nonused()
{
    Ped* pFBI = gPedManager->AllocatePed();
    pFBI->SetPedType(ped_type::special_ped_4);
    pFBI->set_occupation(ped_ocupation_enum::fbi);
    pFBI->SpawnPedInCar(mSubObj->mCar);
    pFBI->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pFBI->mObjectiveTargetX = Fix16(mTargX);
    pFBI->mObjectiveTargetY = Fix16(mTargY);
    pFBI->mObjectiveTargetZ = Fix16(mTargZ);
    pFBI->set_remap(ped_remap_enum::ped_remap_blue_police);
    pFBI->ForceWeapon(weapon_type::smg);
    pFBI->set_health(200);
    pFBI->mThreatSearch = threat_search_enum::line_of_sight_1;
    pFBI->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
    pFBI->mGraphicType = ped_graphic_type::character_1;
    mSubObj->mPed = pFBI;
    mSubObj->mState = crew_state::update_6;
    mSubObj->mCar->SetDriverPedType(ped_type::script_created_5);
    mSubObj->mCar->InitCarAIControl();
    mSubObj->mCar->ResumeAIDriving();
    mSubObj->mCar->ActivateEmergencyLights();
    mSubObj->mGroup = NULL;
}

MATCH_FUNC(0x571350)
void PoliceCrew::TryDespawnOffscreenCrew()
{
    mState = police_crew_state::despawning_2;
    PedGroup* pGroup = mSubObj->mGroup;
    if (pGroup)
    {
        if (pGroup->AreAllMembersOffScreen())
        {
            u8 ped_idx = 0;
            for (Ped* pPedIter = mSubObj->mPed; pPedIter; pPedIter = mSubObj->mGroup->mPedList[ped_idx++])
            {
                pPedIter->ClearGroupAndGroupIdx();
                pPedIter->Deallocate();
                if (!mSubObj->mGroup)
                {
                    break;
                }
            }
            mSubObj->mGroup->ClearGroupData();
            mSubObj->mState = crew_state::clean_up_5;
            mSubObj->mReady = true;
        }
    }
    else
    {
        Ped* pPed = mSubObj->mPed;
        if (pPed)
        {
            if (pPed->GetOffscreenCounter() >= 0x1Eu)
            {
                pPed->Deallocate();
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
        }
        else
        {
            mSubObj->mState = crew_state::clean_up_5;
            mSubObj->mReady = true;
        }
    }
}

MATCH_FUNC(0x571540)
void PoliceCrew::TryDespawnFinishedCrew()
{
    Car* pCar = mSubObj->mCar;
    PedGroup* pGroup = mSubObj->mGroup;
    if (pCar)
    {
        if (pGroup)
        {
            if (pCar->GetLastSeenTimer() > 200 && pGroup->AreAllMembersOffScreen())
            {
                u8 ped_idx = 0;
                for (Ped* pPedIter = mSubObj->mPed; pPedIter; pPedIter = mSubObj->mGroup->mPedList[ped_idx++])
                {
                    pPedIter->ClearGroupAndGroupIdx();
                    pPedIter->Deallocate();
                    if (!mSubObj->mGroup)
                    {
                        break;
                    }
                }
                mSubObj->mGroup->ClearGroupData();
                mSubObj->mCar->MarkForDespawn();
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
        }
        else
        {
            Ped* pPed = mSubObj->mPed;
            if (pPed)
            {
                if (pPed->GetOffscreenCounter() > 30 && pCar->GetLastSeenTimer() > 200)
                {
                    pPed->Deallocate();
                    mSubObj->mCar->MarkForDespawn();
                    mSubObj->mState = crew_state::clean_up_5;
                    mSubObj->mReady = true;
                }
            }
            else if (pCar->GetLastSeenTimer() > 200)
            {
                pCar->MarkForDespawn();
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
        }
    }
    else if (pGroup)
    {
        if (pGroup->AreAllMembersOffScreen())
        {
            u8 ped_idx = 0;
            for (Ped* pPedIter = mSubObj->mPed; pPedIter; pPedIter = mSubObj->mGroup->mPedList[ped_idx++])
            {
                pPedIter->ClearGroupAndGroupIdx();
                pPedIter->Deallocate();
                if (!mSubObj->mGroup)
                {
                    break;
                }
            }
            mSubObj->mGroup->ClearGroupData();
            mSubObj->mState = crew_state::clean_up_5;
            mSubObj->mReady = true;
        }
    }
    else
    {
        Ped* pPed = mSubObj->mPed;
        if (pPed)
        {
            if (pPed->GetOffscreenCounter() >= 30)
            {
                pPed->Deallocate();
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
            else if (pPed->mCar)
            {
                pPed->Deallocate();
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
            else if (pPed->mThreatReaction != threat_reaction_enum::react_as_emergency_1)
            {
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
        }
        else
        {
            mSubObj->mState = crew_state::clean_up_5;
            mSubObj->mReady = true;
        }
    }
}

MATCH_FUNC(0x571a30)
void PoliceCrew::TryDespawnInCarCrew()
{
    Car* pCrewPedCar = sCurrentCrewPed->mCar;
    if (pCrewPedCar && sCurrentCrewPed == pCrewPedCar->mDriver)
    {
        PedGroup* pGroup = mSubObj->mGroup;
        if (pGroup)
        {
            if (mSubObj->mCar->GetLastSeenTimer() <= 200)
            {
                return;
            }

            if (pGroup->IsAllMembersInSomeCar())
            {
                u8 i = 0;
                for (Ped* pPedIter = mSubObj->mPed; pPedIter; pPedIter = mSubObj->mGroup->mPedList[i++])
                {
                    pPedIter->SetPedType(ped_type::dummy_3);
                    pPedIter->ClearGroupAndGroupIdx();
                    if (!mSubObj->mGroup)
                    {
                        break;
                    }
                }
                mSubObj->mGroup->ClearGroupData();
                mSubObj->mCar->MarkForDespawn();
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
            else
            {
                bool bAllReady = true;
                mSubObj->mCar->HaltAIDriving();

                PedGroup* pGroup2 = mSubObj->mGroup;
                u8 j = 0;
                for (Ped* pMember = pGroup2->mPedList[0]; pMember; pMember = pGroup2->mPedList[++j])
                {
                    if (pMember->mGameObject && pMember->GetOffscreenCounter() < 10)
                    {
                        bAllReady = false;
                    }
                }

                if (!bAllReady)
                {
                    return;
                }

                mSubObj->mPed->ClearGroupAndGroupIdx();
                mSubObj->mCar->MarkForDespawn();

                u8 k = 0;
                for (Ped* pPed = mSubObj->mGroup->mPedList[0]; pPed;
                     pPed = mSubObj->mGroup->mPedList[++k])
                {
                    if (pPed->mGameObject)
                    {
                        pPed->ClearGroupAndGroupIdx();
                        pPed->Deallocate();
                    }
                }
                mSubObj->mGroup->ClearGroupData();
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
        }
        else
        {
            Car* pCar = mSubObj->mCar;
            if (pCar->GetLastSeenTimer() > 80)
            {
                pCar->MarkForDespawn();
                mSubObj->mState = crew_state::clean_up_5;
                mSubObj->mReady = true;
            }
        }
    }
    else
    {
        PedGroup* pGroup = mSubObj->mGroup;
        if (pGroup)
        {
            if (!pGroup->AreAllMembersOffScreen())
            {
                return;
            }

            Car* pCar = mSubObj->mCar;
            if (pCar->GetLastSeenTimer() <= 200 && pCar->mPedType != ped_type::player_2)
            {
                return;
            }

            u8 i = 0;
            for (Ped* pPedIter = mSubObj->mPed; pPedIter; pPedIter = mSubObj->mGroup->mPedList[i++])
            {
                pPedIter->ClearGroupAndGroupIdx();
                pPedIter->Deallocate();
                if (!mSubObj->mGroup)
                {
                    break;
                }
            }
            mSubObj->mGroup->ClearGroupData();

            pCar = mSubObj->mCar;
            if (pCar->mPedType != ped_type::player_2)
            {
                pCar->MarkForDespawn();
            }
            mSubObj->mState = crew_state::clean_up_5;
            mSubObj->mReady = true;
        }
        else
        {
            Car* pCar = mSubObj->mCar;
            if (pCar->GetLastSeenTimer() <= 200 && pCar->mPedType != ped_type::player_2)
            {
                return;
            }
            if (mSubObj->mPed->GetOffscreenCounter() <= 30)
            {
                return;
            }
            if (pCar->mPedType != ped_type::player_2)
            {
                pCar->MarkForDespawn();
            }
            mSubObj->mPed->Deallocate();
            mSubObj->mState = crew_state::clean_up_5;
            mSubObj->mReady = true;
        }
    }
}

MATCH_FUNC(0x5720c0)
void PoliceCrew::HandleShutdownAndDespawn()
{
    if (mSubObj->mPhase == crew_phase::finished_2)
    {
        if (mbCountedInPoliceCount)
        {
            if (gPolice_7B8->mPoliceCarCount > 0)
            {
                gPolice_7B8->mPoliceCarCount--;
            }
            mbCountedInPoliceCount = false;
        }

        if (mState != police_crew_state::shutdown_6)
        {
            UnlinkFromPursuitTarget();
            mState = police_crew_state::shutdown_6;
            return;
        }
    }
    else if (mState != police_crew_state::shutdown_6)
    {
        return;
    }

    if (mSubObj->mPhase == crew_phase::on_foot_0)
    {
        TryDespawnOffscreenCrew();
    }
    else if (mSubObj->mPhase == crew_phase::finished_2)
    {
        TryDespawnFinishedCrew();
    }
    else
    {
        TryDespawnInCarCrew();
    }
}

// https://decomp.me/scratch/p2NiN
// MaxAbsDistance with a by-value max, which keeps the compare in registers
MATCH_FUNC(0x572210)
bool PoliceCrew::CanTrackCriminal() const
{
    if (mpPursuitTarget->mCriminalPed && !mIsDismissed && mpPursuitTarget->mTimer > 0)
    {
        if (mSubObj->mPhase == crew_phase::on_foot_0)
        {
            return Fix16::MaxAbsDistance(sCurrentCrewPed->get_cam_x(),
                                        sCurrentCrewPed->get_cam_y(),
                                        mpPursuitTarget->mCriminalPed->get_cam_x(),
                                        mpPursuitTarget->mCriminalPed->get_cam_y()) < kFpEight ? true : false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x572340)
void PoliceCrew::State3_AlertedSearch()
{
    bool bSpawned = true;
    u8 idx = 0;
    gPoliceCrew_Unused = 1;

    if (mSubObj->mState != crew_state::spawn_car_3)
    {
        if (mSubObj->mState == crew_state::update_6)
        {
            PoliceCrew::HandleShutdownAndDespawn();
        }
    }
    else if (mSubObj->mReady)
    {
        if (gPedManager->mFbiArmyCount < 0x1Au)
        {
            switch (mSubObj->mCrewType)
            {
                case crew_type::police_3:
                    PoliceCrew::SpawnPoliceInCar();
                    break;
                case crew_type::swat_5:
                    PoliceCrew::SpawnSWAT();
                default:
                    break;
            }
            sCurrentCrewPed = mSubObj->mPed;
            mUnused1A = 0;
        }
        else
        {
            Car* pCar = mSubObj->mCar;
            if (pCar)
            {
                pCar->MarkForDespawn();
                mSubObj->mCar = NULL;
                mState = police_crew_state::shutdown_6;
                PoliceCrew::UnlinkFromPursuitTarget();
            }
        }
    }
    else
    {
        bSpawned = false;
        if (mSubObj->mSpawnDelay == -80)
        {
            mState = police_crew_state::shutdown_6;
            PoliceCrew::UnlinkFromPursuitTarget();
        }
    }

    if (mState != police_crew_state::shutdown_6)
    {
        if (bSpawned)
        {
            Ped* pPed = mSubObj->mPed;
            sCurrentCrewPed = pPed;
            if (mpPursuitTarget->mTimer == 250)
            {
                mState = police_crew_state::pursue_or_chase_5;
            }
            else
            {
                for (; pPed; ++idx)
                {
                    if (pPed->mPedState1 != ped_state_1::dead_9 
                        && pPed->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        Fix16 xpos_f;
                        switch (pPed->get_objective())
                        {
                            case objectives_enum::chase_char_in_car_52:
                                pPed->SetObjective(objectives_enum::goto_area_in_car_14);
                                sCurrentCrewPed->mObjectiveTargetX = Fix16(mpPursuitTarget->mX.ToUInt8());
                                sCurrentCrewPed->mObjectiveTargetY = Fix16(mpPursuitTarget->mY.ToUInt8());
                                sCurrentCrewPed->mObjectiveTargetZ = Fix16(mpPursuitTarget->mZ.ToUInt8());
                                break;
                            case objectives_enum::goto_area_in_car_14:
                                PoliceCrew::HandleGotoAreaInCarObjective();
                                xpos_f = mpPursuitTarget->mX;
                                if (sCurrentCrewPed->mObjectiveTargetX != mpPursuitTarget->mX ||
                                    sCurrentCrewPed->mObjectiveTargetY != mpPursuitTarget->mY)
                                {
                                    u8 xpos = xpos_f.ToInt();
                                    u8 ypos = mpPursuitTarget->mY.ToInt();
                                    u8 zpos = mpPursuitTarget->mZ.ToInt();
                                    if (gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &xpos, &ypos, &zpos, false))
                                    {
                                        sCurrentCrewPed->mObjectiveTargetX = Fix16(xpos);
                                        mpPursuitTarget->mX = sCurrentCrewPed->mObjectiveTargetX;
                                        sCurrentCrewPed->mObjectiveTargetY = Fix16(ypos);
                                        mpPursuitTarget->mY = sCurrentCrewPed->mObjectiveTargetY;
                                        sCurrentCrewPed->mObjectiveTargetZ = Fix16(zpos);
                                        mpPursuitTarget->mZ = sCurrentCrewPed->mObjectiveTargetZ;
                                    }
                                }
                                mUseCar = true;
                                break;
                            case objectives_enum::no_obj_0:
                                if (pPed->IsGroupLeader())
                                {
                                    if (pPed->mCar)
                                    {
                                        pPed->SetObjective2(objectives_enum::no_obj_0);
                                        sCurrentCrewPed->SetObjective(objectives_enum::leave_car_36);
                                        sCurrentCrewPed->SetTargetObjectiveCar(mSubObj->mCar);
                                        break;
                                    }
                                    else
                                    {
                                        pPed->SetObjective2(objectives_enum::no_obj_0);
                                        sCurrentCrewPed->SetObjective(objectives_enum::goto_area_on_foot_12);
                                        sCurrentCrewPed->mObjectiveTargetX = mpPursuitTarget->mX;
                                        sCurrentCrewPed->mObjectiveTargetY = mpPursuitTarget->mY;
                                        sCurrentCrewPed->mObjectiveTargetZ = mpPursuitTarget->mZ;
                                    }
                                }
                                break;
                            case objectives_enum::leave_car_36:
                                PoliceCrew::HandleLeaveCarObjective();
                                break;
                            case objectives_enum::enter_car_as_driver_35:
                                PoliceCrew::HandleEnterCarObjective();
                                break;
                            case objectives_enum::wander_until_timeout_51:
                                if (pPed->GetObjectiveStatus() == objective_status::failed_2)
                                {
                                    mState = police_crew_state::shutdown_6;
                                    mpPursuitTarget->mCrewGaveUp = true;
                                }
                                break;
                            case objectives_enum::kill_char_on_foot_20:
                                pPed->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::goto_area_on_foot_12);
                                sCurrentCrewPed->mObjectiveTargetX = mpPursuitTarget->mX;
                                sCurrentCrewPed->mObjectiveTargetY = mpPursuitTarget->mY;
                                sCurrentCrewPed->mObjectiveTargetZ = mpPursuitTarget->mZ;
                                break;
                            case objectives_enum::goto_area_on_foot_12:
                                mUseCar = false;
                                if (sCurrentCrewPed->GetObjectiveStatus() == objective_status::passed_1)
                                {
                                    sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::wander_until_timeout_51, 200);
                                }
                                break;
                            case objectives_enum::dead_patient_28:
                                if (pPed->GetObjectiveStatus() != objective_status::not_finished_0)
                                {
                                    pPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                                }
                                break;
                            case objectives_enum::pull_driver_out_32:
                                pPed->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                                break;
                            default:
                                break;
                        }
                    }
                    PedGroup* pGroup = mSubObj->mGroup;
                    if (pGroup)
                    {
                        pPed = pGroup->mPedList[idx];
                    }
                    else
                    {
                        pPed = NULL;
                    }
                    sCurrentCrewPed = pPed;
                }
            }
        }
    }
}

EXTERN_GLOBAL(Fix16, kFpFour_6FECF8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6FEB44, Fix16(0x666, 0), 0x6FEB44);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFour, Fix16(4), 0x6FED08);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint02, Fix16(0x147, 0), 0x6FECB8);

// MaxAbsDistance as State5_PursueOrChase inlines it: past the nested inline budget, Max is the named
// out-of-line copy Max_44E540 (as in Weapon's MaxAbsDistance_5DF270)
static inline Fix16 MaxAbsDistance_572920(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2)
{
    Fix16 diff_x;
    diff_x = x2 - x1;
    Fix16 diff_y;
    diff_y = y2 - y1;
    Fix16 result;
    result = Fix16::Max_44E540(Fix16::Abs(diff_x), Fix16::Abs(diff_y));
    return result;
}

// The crew chasing the criminal: like State3_AlertedSearch, then each member follows the criminal on foot
// or in the car depending on how far away and how fast the criminal is.
// The inline cut-offs of MaxAbsDistance (one Abs negate inline, Max out of line) need its nested budget at 287..321,
// i.e. the 18 sites left after it: the enter-car test reads mPedState1 directly. `default: break;` first in
// the objective switch keeps goto_area_in_car_14's trailing `field_28 = 1; break;` as the copy patrol_in_car_43 merges into.
MATCH_FUNC(0x572920)
void PoliceCrew::State5_PursueOrChase()
{
    bool bSpawned = true;
    bool bChaseOnFoot = false;
    u8 idx = 0;

    if (!mpPursuitTarget->mCriminalPed)
    {
        mState = police_crew_state::shutdown_6;
        PoliceCrew::UnlinkFromPursuitTarget();
        return;
    }

    if (mSubObj->mState != crew_state::spawn_car_3)
    {
        if (mSubObj->mState == crew_state::update_6)
        {
            PoliceCrew::HandleShutdownAndDespawn();
            if (mSubObj->mState == crew_state::clean_up_5 || mState == police_crew_state::shutdown_6)
            {
                mState = police_crew_state::shutdown_6;
                PoliceCrew::UnlinkFromPursuitTarget();
                return;
            }
        }
    }
    else
    {
        if (mSubObj->mReady)
        {
            if (gPedManager->mFbiArmyCount < 0x1Au)
            {
                switch (mSubObj->mCrewType)
                {
                    case crew_type::police_3:
                        PoliceCrew::SpawnPoliceInCar();
                        break;
                    case crew_type::swat_5:
                        PoliceCrew::SpawnSWAT();
                    default:
                        break;
                }
                sCurrentCrewPed = mSubObj->mPed;
                mUnused1A = 0;
            }
            else
            {
                Car* pCar = mSubObj->mCar;
                if (pCar)
                {
                    pCar->MarkForDespawn();
                    mSubObj->mCar = NULL;
                    mState = police_crew_state::shutdown_6;
                    PoliceCrew::UnlinkFromPursuitTarget();
                }
            }
        }
        else
        {
            bSpawned = false;
            if (mSubObj->mSpawnDelay == -80)
            {
                mState = police_crew_state::shutdown_6;
                PoliceCrew::UnlinkFromPursuitTarget();
            }
        }
    }
    if (mState == police_crew_state::shutdown_6 || !bSpawned)
    {
        return;
    }

    sCurrentCrewPed = mSubObj->mPed;
    if (PoliceCrew::CanTrackCriminal())
    {
        mpPursuitTarget->mX = mpPursuitTarget->mCriminalPed->get_cam_x();
        mpPursuitTarget->mY = mpPursuitTarget->mCriminalPed->get_cam_y();
        mpPursuitTarget->mZ = mpPursuitTarget->mCriminalPed->get_cam_z();
    }
    else if (mpPursuitTarget->mTimer > 0)
    {
        if (mSubObj->mPhase == crew_phase::on_foot_0)
        {
            mState = police_crew_state::shutdown_6;
            PoliceCrew::UnlinkFromPursuitTarget();
            return;
        }
        if (mpPursuitTarget->mCriminalPed->mCar)
        {
            mpPursuitTarget->mX = mpPursuitTarget->mCriminalPed->get_cam_x();
            mpPursuitTarget->mY = mpPursuitTarget->mCriminalPed->get_cam_y();
            mpPursuitTarget->mZ = mpPursuitTarget->mCriminalPed->get_cam_z();
        }
    }
    else
    {
        if (mSubObj->mPhase == crew_phase::on_foot_0)
        {
            mState = police_crew_state::shutdown_6;
            PoliceCrew::UnlinkFromPursuitTarget();
            return;
        }
        mState = police_crew_state::alerted_search_3;
        return;
    }

    if (mpPursuitTarget->mTimer > 0)
    {
        while (sCurrentCrewPed)
        {
            Ped* pPed = sCurrentCrewPed;
            if (pPed->mPedState1 != ped_state_1::dead_9 && pPed->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
            {
                switch (pPed->get_objective())
                {
                    default:
                        break;
                    case objectives_enum::goto_area_in_car_14:
                        if (pPed == mSubObj->mPed)
                        {
                            if (mSubObj->mGroup)
                            {
                                if (mSubObj->mGroup->IsAllMembersInSomeCar())
                                {
                                    sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::chase_char_in_car_52);
                                    sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                                    mUseCar = true;
                                }
                                else
                                {
                                    sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                                    mUseCar = true;
                                    break;
                                }
                            }
                            else
                            {
                                sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::chase_char_in_car_52);
                                sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                                mUseCar = true;
                            }
                        }
                        mUseCar = true;
                        break;

                    case objectives_enum::chase_char_in_car_52:
                        mUseCar = true;
                        if (mSubObj->mCar &&
                            (!mSubObj->mGroup || mSubObj->mGroup->HasNoActiveMembers()))
                        {
                            PoliceCrew::HandleChaseInCarObjective();
                            mUseCar = true;
                        }
                        else
                        {
                            sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                            sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                        }
                        break;

                    case objectives_enum::no_obj_0:
                        if (pPed == mSubObj->mPed)
                        {
                            if (pPed->mCar)
                            {
                                if (mpPursuitTarget->mCriminalPed->mCar)
                                {
                                    if (mSubObj->mGroup && !mSubObj->mGroup->HasNoActiveMembers())
                                    {
                                        break;
                                    }
                                    sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::chase_char_in_car_52);
                                    sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                                    mUseCar = true;
                                }
                                else
                                {
                                    sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::leave_car_36);
                                    sCurrentCrewPed->SetTargetObjectiveCar(mSubObj->mCar);
                                    mUseCar = false;
                                }
                            }
                            else if (mUseCar && mSubObj->mPhase == crew_phase::in_car_1)
                            {
                                if (!pPed->mBf21C.bLeftVehicle)
                                {
                                    sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::enter_car_as_driver_35);
                                    sCurrentCrewPed->SetTargetObjectiveCar(mSubObj->mCar);
                                    sCurrentCrewPed->ClearPanicking();
                                }
                            }
                            else if (!pPed->mBf21C.bLeftVehicle)
                            {
                                sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::kill_char_on_foot_20);
                                sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                                sCurrentCrewPed->ClearPanicking();
                                mUseCar = false;
                                mFastTargetCounter = 0;
                            }
                        }
                        else if (pPed->GetInternalTargetPed() == mPrevTargetPed && pPed->GetInternalTargetPed())
                        {
                            pPed->SetInternalTargetPed(mpPursuitTarget->mCriminalPed);
                        }
                        break;

                    case objectives_enum::leave_car_36:
                        PoliceCrew::HandleLeaveCarObjective();
                        mUseCar = false;
                        break;

                    case objectives_enum::enter_car_as_driver_35:
                        PoliceCrew::HandleEnterCarObjective();
                        mUseCar = true;
                        break;

                    case objectives_enum::kill_char_on_foot_20:
                    case objectives_enum::pull_driver_out_32:
                    {
                        if (pPed->get_objective_target_ped() == pPed->GetInternalTargetPed())
                        {
                            mPrevTargetPed = pPed->get_objective_target_ped();
                            sCurrentCrewPed->SetInternalTargetPed(mpPursuitTarget->mCriminalPed);
                        }
                        sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                        if (sCurrentCrewPed->GetObjectiveStatus() == objective_status::passed_1)
                        {
                            gPoliceCrew_Unused = 0;
                            break;
                        }
                        // An if/else rather than an early break: the then-branch exit is retargeted late, so its
                        // copy of SetObjective2/SetObjective survives the cross-jump with goto_area_on_foot_12,
                        // wander_until_timeout_51 and dead_patient_28 (9.6f 0x4AC580 matches this function exactly with VC7)
                        if (sCurrentCrewPed->GetObjectiveStatus() == objective_status::failed_2)
                        {
                            sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                            sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                        }
                        else
                        {

                            if (mpPursuitTarget->mCriminalPed)
                            {
                                // 9.6f: MaxAbsDistance
                                mCriminalDistance = MaxAbsDistance_572920(sCurrentCrewPed->get_cam_x(),
                                                                          sCurrentCrewPed->get_cam_y(),
                                                                          mpPursuitTarget->mCriminalPed->get_cam_x(),
                                                                          mpPursuitTarget->mCriminalPed->get_cam_y());
                            }
                            else
                            {
                                mCriminalDistance = kFpEight * kFpFour_6FECF8;
                            }

                            if (mSubObj->mPhase == crew_phase::in_car_1)
                            {
                                bool bEnterCar = false;
                                if (mCriminalDistance > kFpEight)
                                {
                                    bEnterCar = true;
                                }
                                else if (mpPursuitTarget->mCriminalPed->mCar)
                                {
                                    if (mpPursuitTarget->mCriminalPed->mCar->GetCarLinearSpeed() > kFpPoint1_6FEB44)
                                    {
                                        if (++mFastTargetCounter > 30)
                                        {
                                            bEnterCar = true;
                                        }
                                    }
                                    else if (mCriminalDistance > kFpFour)
                                    {
                                        bEnterCar = true;
                                    }
                                    else
                                    {
                                        bChaseOnFoot = true;
                                    }
                                }

                                if (bEnterCar)
                                {
                                    // The criminal is too far or too fast to chase on foot
                                    if (sCurrentCrewPed->get_objective() == objectives_enum::pull_driver_out_32 && sCurrentCrewPed->mPedState1 != ped_state_1::flee_or_running_1)
                                    {
                                        break;
                                    }
                                    if (!sCurrentCrewPed->mBf21C.bLeftVehicle)
                                    {
                                        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                        sCurrentCrewPed->SetObjective(objectives_enum::enter_car_as_driver_35);
                                        sCurrentCrewPed->SetTargetObjectiveCar(mSubObj->mCar);
                                        sCurrentCrewPed->ClearPanicking();
                                        mUseCar = true;
                                    }
                                    break;
                                }
                            }

                            if (sCurrentCrewPed->get_objective() == objectives_enum::pull_driver_out_32)
                            {
                                if (mpPursuitTarget->mCriminalPed->mGameObject && sCurrentCrewPed->mBf21C.bLeftVehicle)
                                {
                                    sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::kill_char_on_foot_20);
                                    sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                                    sCurrentCrewPed->ClearPanicking();
                                    mUseCar = false;
                                }
                            }
                            else if (bChaseOnFoot)
                            {
                                sCurrentCrewPed->SetObjective(objectives_enum::pull_driver_out_32);
                                sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                                sCurrentCrewPed->ClearPanicking();
                            }
                        }
                        break;
                    }

                    case objectives_enum::goto_area_on_foot_12:
                        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                        sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                        break;
                    case objectives_enum::wander_until_timeout_51:
                        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                        sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                        break;

                    case objectives_enum::dead_patient_28:
                        if (pPed->GetObjectiveStatus())
                        {
                            sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                            sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                        }
                        break;

                    case objectives_enum::flee_char_on_foot_till_safe_2:
                        gPoliceCrew_Unused = 0;
                        PoliceCrew::RemoveFromPursuit();
                        break;

                    case objectives_enum::patrol_in_car_43:
                        pPed->SetObjective2(objectives_enum::no_obj_0);
                        if (mSubObj->mGroup)
                        {
                            if (mSubObj->mGroup->IsAllMembersInSomeCar())
                            {
                                sCurrentCrewPed->SetObjective(objectives_enum::chase_char_in_car_52);
                                sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                                mUseCar = true;
                            }
                            else
                            {
                                sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                            }
                        }
                        else
                        {
                            sCurrentCrewPed->SetObjective(objectives_enum::chase_char_in_car_52);
                            sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);
                            mUseCar = true;
                        }
                        break;

                    case objectives_enum::wait_in_car_27:
                        if (mSubObj->mCar->GetCarLinearSpeed() <= kFpPoint02)
                        {
                            sCurrentCrewPed->SetObjective(objectives_enum::leave_car_36);
                            sCurrentCrewPed->SetTargetObjectiveCar(sCurrentCrewPed->mCar);
                        }
                        break;
                }
            }

            PedGroup* pGroup = mSubObj->mGroup;
            if (pGroup)
            {
                sCurrentCrewPed = pGroup->mPedList[idx];
            }
            else
            {
                sCurrentCrewPed = NULL;
            }
            idx++;
        }
    }
    else
    {
        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
        sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
        mState = police_crew_state::shutdown_6;
        PoliceCrew::UnlinkFromPursuitTarget();
    }
}

// https://decomp.me/scratch/mAN9o
MATCH_FUNC(0x574720)
void PoliceCrew::State6_ShutDown()
{
    u8 i = 0;
    gPoliceCrew_Unused = 1;
    sCurrentCrewPed = mSubObj->mPed;
    if (mSubObj->mCar)
    {
        if (mSubObj->mCar->mpChaseTask)
        {
            gCarChaseTaskTable->FreeEntry(mSubObj->mCar->mpChaseTask);
            mSubObj->mCar->mpChaseTask = NULL;
        }
        if (mSubObj->mCar->HasEmergencyLights())
        {
            mSubObj->mCar->DeactivateEmergencyLights();
        }
    }

    if (mSubObj->mState != crew_state::spawn_car_3)
    {
        if (mSubObj->mState == crew_state::update_6)
        {
            PoliceCrew::HandleShutdownAndDespawn();
        }
        if (mSubObj->mState == crew_state::clean_up_5)
        {
            if (mSubObj->mCar)
            {
                if (mSubObj->mCar->mAI)
                {
                    if (mSubObj->mCar->mAI->mRouteIdx > 0)
                    {
                        gRouteFinder->CancelRoute(mSubObj->mCar->mAI->mRouteIdx);
                        mSubObj->mCar->mAI->mRouteIdx = -1;
                    }
                }
            }
            PoliceCrew::UnlinkFromPursuitTarget();
            if (mbCountedInPoliceCount)
            {
                if (gPolice_7B8->mPoliceCarCount > 0)
                {
                    --gPolice_7B8->mPoliceCarCount;
                }
                mbCountedInPoliceCount = false;
            }

            if (mSubObj->mPed)
            {
                mSubObj->mPed->ForceDoNothing();
            }
            mSubObj->mState = crew_state::idle_0;
            mSubObj->ReInit();
            PoliceCrew::Init();
            return;
        }

        if (mpPursuitTarget)
        {
            if (mSubObj->mPhase != crew_phase::finished_2)
            {
                if (mpPursuitTarget->mCriminalPed)
                {
                    if (PoliceCrew::CanTrackCriminal())
                    {
                        if (mSubObj->mCrewType == crew_type::army_6 ||
                            (mpPursuitTarget->mWantedLevel != cop_level_enum::army_6 && mpPursuitTarget->mWantedLevel))
                        {
                            if (sCurrentCrewPed->get_objective() == objectives_enum::enter_car_as_driver_35 &&
                                !sCurrentCrewPed->mBf21C.bLeftVehicle)
                            {
                                sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                            }
                            mpPursuitTarget->mX = mpPursuitTarget->mCriminalPed->get_cam_x();
                            mpPursuitTarget->mY = mpPursuitTarget->mCriminalPed->get_cam_y();
                            mpPursuitTarget->mZ = mpPursuitTarget->mCriminalPed->get_cam_z();

                            gPolice_7B8->AssignCrewToPursuit(this, mpPursuitTarget);
                            return;
                        }
                    }
                }
            }
        }
        if (mSubObj->mPhase != crew_phase::finished_2)
        {
            if (sCurrentCrewPed)
            {
                sCurrentCrewPed->ClearAttacking_403A40();

                for (; sCurrentCrewPed; ++i)
                {
                    if (sCurrentCrewPed->mPedState1 != ped_state_1::dead_9 &&
                        sCurrentCrewPed->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        switch (sCurrentCrewPed->get_objective())
                        {
                            // Cases 14 and 52 are written out separately (VC6 tail merges them). With one shared
                            // body the zero constant has one store fewer, its priority drops below the ~4 of
                            // ClearPanicking and VC6 keeps 0 in ebp for the whole function.
                            case objectives_enum::goto_area_in_car_14:
                                sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::patrol_in_car_43);
                                gPoliceCrew_Unused = 0;
                                break;
                            case objectives_enum::chase_char_in_car_52:
                                sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::patrol_in_car_43);
                                gPoliceCrew_Unused = 0;
                                break;
                            case objectives_enum::no_obj_0:
                                if (sCurrentCrewPed != mSubObj->mPed)
                                {
                                    break;
                                }
                                if (sCurrentCrewPed->mCar)
                                {
                                    if (!mSubObj->mGroup || mSubObj->mGroup->IsAllMembersInSomeCar())
                                    {
                                        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                        sCurrentCrewPed->SetObjective(objectives_enum::patrol_in_car_43);
                                        sCurrentCrewPed->mCar->InitCarAIControl();
                                        sCurrentCrewPed->mCar->ResumeAIDriving();
                                        gPoliceCrew_Unused = 0;
                                    }
                                }
                                else
                                {
                                    if (mSubObj->mPhase == crew_phase::in_car_1 && !sCurrentCrewPed->mBf21C.bLeftVehicle)
                                    {
                                        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                        sCurrentCrewPed->SetObjective(objectives_enum::enter_car_as_driver_35);
                                        sCurrentCrewPed->SetTargetObjectiveCar(mSubObj->mCar);
                                        sCurrentCrewPed->ClearPanicking();
                                    }
                                }
                                break;
                            case objectives_enum::left_behind_by_leader_8:
                            case objectives_enum::wait_in_car_27:
                            case objectives_enum::leave_car_and_flee_34:
                            case objectives_enum::patrol_in_car_43:
                            case objectives_enum::dead_no_ambulance_50:
                                break;
                            case objectives_enum::enter_car_as_driver_35:
                                PoliceCrew::HandleEnterCarObjective();
                                break;
                            case objectives_enum::leave_car_36:
                                PoliceCrew::HandleLeaveCarObjective();
                                break;
                            case objectives_enum::dead_patient_28:
                                if (sCurrentCrewPed->GetObjectiveStatus())
                                {
                                    sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                                }
                                break;
                            default:
                                sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                                break;
                        }
                    }

                    if (mSubObj->mGroup)
                    {
                        sCurrentCrewPed = mSubObj->mGroup->mPedList[i];
                    }
                    else
                    {
                        sCurrentCrewPed = NULL;
                    }
                }
            }
        }
    }
    else
    {
        if (mSubObj->mCar)
        {
            if (mSubObj->mCar->mAI)
            {
                if (mSubObj->mCar->mAI->mRouteIdx > 0)
                {
                    gRouteFinder->CancelRoute(mSubObj->mCar->mAI->mRouteIdx);
                    mSubObj->mCar->mAI->mRouteIdx = -1;
                }
            }
        }
        mSubObj->mState = crew_state::idle_0;
        mSubObj->ReInit();
        if (mbCountedInPoliceCount)
        {
            if (gPolice_7B8->mPoliceCarCount > 0)
            {
                --gPolice_7B8->mPoliceCarCount;
            }
        }
        PoliceCrew::Init();
        PoliceCrew::UnlinkFromPursuitTarget();
    }
}

MATCH_FUNC(0x574f10)
void PoliceCrew::State1_Patrol()
{
    Car* pCrewCar;

    gPoliceCrew_Unused = 1;
    u8 idx = 0;

    if (mSubObj->mPhase == crew_phase::finished_2 || mSubObj->mPhase == crew_phase::on_foot_0 || (pCrewCar = mSubObj->mCar, pCrewCar->mLastSeenTimer > 80))
    {
        mState = police_crew_state::shutdown_6;
    }
    else
    {
        pCrewCar->ClearAIHalted();
        if (mSubObj->mState == crew_state::update_6)
        {
            PoliceCrew::HandleShutdownAndDespawn();
        }

        if (mSubObj->mState == crew_state::clean_up_5)
        {
            Car* pCar = mSubObj->mCar;
            if (pCar)
            {
                CarAI* pAI = pCar->mAI;
                if (pAI)
                {
                    char_type route_idx = pAI->mRouteIdx;
                    if (route_idx > 0)
                    {
                        gRouteFinder->CancelRoute(route_idx);
                        mSubObj->mCar->mAI->mRouteIdx = -1;
                    }
                }
            }
            PoliceCrew::UnlinkFromPursuitTarget();
            if (mbCountedInPoliceCount)
            {
                if (gPolice_7B8->mPoliceCarCount > 0)
                {
                    gPolice_7B8->mPoliceCarCount--;
                }
            }
            mSubObj->mState = crew_state::idle_0;
            mSubObj->ReInit();
            PoliceCrew::Init();
        }
        else
        {
            if (mpPursuitTarget && mpPursuitTarget->mCriminalPed && PoliceCrew::CanTrackCriminal() &&
                (mSubObj->mCrewType == crew_type::army_6 || mpPursuitTarget->mWantedLevel != cop_level_enum::army_6))
            {
                mpPursuitTarget->mX = mpPursuitTarget->mCriminalPed->get_cam_x();
                mpPursuitTarget->mY = mpPursuitTarget->mCriminalPed->get_cam_y();
                mpPursuitTarget->mZ = mpPursuitTarget->mCriminalPed->get_cam_z();
                gPolice_7B8->AssignCrewToPursuit(this, mpPursuitTarget);
            }
            else
            {
                sCurrentCrewPed = mSubObj->mPed;
                sCurrentCrewPed->ClearAttacking_403A40();
                CarChaseTask* pChaseTask;
                for (Ped* pPedIter = sCurrentCrewPed; pPedIter; ++idx)
                {
                    switch (pPedIter->get_objective())
                    {
                        case objectives_enum::goto_area_in_car_14:
                        case objectives_enum::chase_char_in_car_52:
                            gPoliceCrew_Unused = 0;
                            pChaseTask = mSubObj->mCar->mpChaseTask;
                            if (pChaseTask)
                            {
                                gCarChaseTaskTable->FreeEntry(pChaseTask);
                                mSubObj->mCar->mpChaseTask = NULL;
                            }
                            break;
                        case objectives_enum::no_obj_0:
                            if (pPedIter == mSubObj->mPed)
                            {
                                if (pPedIter->mCar)
                                {
                                    gPoliceCrew_Unused = 0;
                                }
                                else if (!pPedIter->mBf21C.bLeftVehicle)
                                {
                                    pPedIter->SetObjective2(objectives_enum::no_obj_0);
                                    sCurrentCrewPed->SetObjective(objectives_enum::enter_car_as_driver_35);
                                    sCurrentCrewPed->SetTargetObjectiveCar(mSubObj->mCar);
                                    sCurrentCrewPed->ClearPanicking();
                                }
                            }
                            break;
                        case objectives_enum::leave_car_36:
                            PoliceCrew::HandleLeaveCarObjective();
                            break;
                        case objectives_enum::enter_car_as_driver_35:
                            PoliceCrew::HandleEnterCarObjective();
                            break;
                        case objectives_enum::flee_char_on_foot_till_safe_2:
                        case objectives_enum::goto_area_on_foot_12:
                        case objectives_enum::kill_char_on_foot_20:
                            pPedIter->SetObjective2(objectives_enum::no_obj_0);
                            sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                            break;
                        case objectives_enum::dead_patient_28:
                            if (pPedIter->GetObjectiveStatus() != objective_status::not_finished_0)
                            {
                                pPedIter->SetObjective2(objectives_enum::no_obj_0);
                                sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
                            }
                            break;
                        default:
                            break;
                    }
                    PedGroup* pGroup = mSubObj->mGroup;
                    if (pGroup)
                    {
                        pPedIter = pGroup->mPedList[idx];
                    }
                    else
                    {
                        pPedIter = NULL;
                    }
                    sCurrentCrewPed = pPedIter;
                }
            }
        }
    }
}

MATCH_FUNC(0x575200)
void PoliceCrew::State2_SwitchToShutDown()
{
    gPoliceCrew_Unused = 1;
    mState = police_crew_state::shutdown_6;
}

MATCH_FUNC(0x575210)
void PoliceCrew::HandleEnterCarObjective()
{
    if (mSubObj->mPhase)
    {
        if (sCurrentCrewPed->GetObjectiveStatus())
        {
            sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
            sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
        }
        else
        {
            mSubObj->mCar->SetAIHalted();
        }
    }
    else
    {
        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
        sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
    }
}

MATCH_FUNC(0x575270)
void PoliceCrew::HandleLeaveCarObjective()
{
    if (mSubObj->mPhase)
    {
        if (sCurrentCrewPed->GetObjectiveStatus())
        {
            sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
            sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
        }
    }
    else
    {
        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
        sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
    }
}

MATCH_FUNC(0x5752c0)
void PoliceCrew::HandleGotoAreaInCarObjective()
{
    if (sCurrentCrewPed->GetObjectiveStatus() != objective_status::not_finished_0)
    {
        mSubObj->mState = crew_state::update_6;
        sCurrentCrewPed->SetObjective2(objectives_enum::no_obj_0);
        sCurrentCrewPed->SetObjective(objectives_enum::no_obj_0);
    }
    gPoliceCrew_Unused = 1;
}

DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6FECF0, Fix16(2), 0x6FECF0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FEBF4, Fix16(0.5), 0x6FEBF4);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_6FECF4, Fix16(3), 0x6FECF4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint03, Fix16(0x1EB, 0), 0x6FEDE0);

MATCH_FUNC(0x575310)
void PoliceCrew::HandleChaseInCarObjective()
{
    gPoliceCrew_Unused = 1;
    sCurrentCrewPed->set_objective_target_ped(mpPursuitTarget->mCriminalPed);

    if (mpPursuitTarget->mCriminalPed->mGameObject)
    {
        Fix16 dist;
        dist = Fix16::MaxAbsDistance(sCurrentCrewPed->get_cam_x(),
                                            sCurrentCrewPed->get_cam_y(),
                                            mpPursuitTarget->mCriminalPed->get_cam_x(),
                                            mpPursuitTarget->mCriminalPed->get_cam_y());
        if (dist < kFpTwo_6FECF0 + kFpHalf_6FEBF4)
        {
            sCurrentCrewPed->SetObjective(objectives_enum::wait_in_car_27);
            if (mSubObj && mSubObj->mCar && mSubObj->mCar->mpChaseTask)
            {
                gCarChaseTaskTable->FreeEntry(mSubObj->mCar->mpChaseTask);
                mSubObj->mCar->mpChaseTask = NULL;
            }
        }
        else
        {
            mState = police_crew_state::alerted_search_3;
        }
    }
    else
    {
        Fix16 dist;
        dist = Fix16::MaxAbsDistance(sCurrentCrewPed->get_cam_x(),
                                            sCurrentCrewPed->get_cam_y(),
                                            mpPursuitTarget->mCriminalPed->get_cam_x(),
                                            mpPursuitTarget->mCriminalPed->get_cam_y());

        Car* pCar = mSubObj->mCar;
        CarChaseTask* pChase = pCar->mpChaseTask;
        if (!pChase)
        {
            return;
        }

        if (pChase->mChaseState == car_chase_state::target_stopped_15)
        {
                pCar->mAI->mBf24.bAIHalted = true;
                sCurrentCrewPed->SetObjective(objectives_enum::wait_in_car_27);
                mpPursuitTarget->mBlockCounter += mSubObj->mCar->mpChaseTask->mBlockCounter;
                if (mSubObj->mCar->mpChaseTask)
                {
                    gCarChaseTaskTable->FreeEntry(mSubObj->mCar->mpChaseTask);
                    mSubObj->mCar->mpChaseTask = NULL;
                }
                return;
        }

        switch (pChase->mChaseState)
        {
            case car_chase_state::follow_behind_0:
            case car_chase_state::unused_1:
            case car_chase_state::follow_behind_2:
            case car_chase_state::unused_14:
            case car_chase_state::target_stopped_15:
                break;
            default:
                mpPursuitTarget->mIsActivelyChased = true;
                break;
        }

        if (static_cast<u8>(mpPursuitTarget->mBlockCounter) > 0)
        {
            mSubObj->mCar->mpChaseTask->mBlockCounter = mpPursuitTarget->mBlockCounter;
        }

        if (dist < kFpThree_6FECF4 && mSubObj->mCar->GetVelocity() < kFpPoint03 &&
            mpPursuitTarget->mCriminalPed->mCar->GetVelocity() < kFpPoint03)
        {
            sCurrentCrewPed->SetObjective(objectives_enum::wait_in_car_27);
            if (mSubObj->mCar->mpChaseTask)
            {
                gCarChaseTaskTable->FreeEntry(mSubObj->mCar->mpChaseTask);
                mSubObj->mCar->mpChaseTask = NULL;
            }
        }
    }
}

MATCH_FUNC(0x575590)
void PoliceCrew::Service()
{
    if (mSubObj)
    {
        Ped* pPed = mSubObj->mPed;
        if (pPed)
        {
            if (!pPed->mOffscreenCounter && pPed->GetPedState() != ped_state_1::dead_9 
                && pPed->IsActive())
            {
                gPolice_7B8->mCrewPedOnscreen = true;
            }
        }
        else
        {
            PedGroup* pGroup = mSubObj->mGroup;
            if (pGroup)
            {
                Ped* pNewLeader = pGroup->mPedList[0];
                if (pNewLeader)
                {
                    pGroup->RemovePed(pNewLeader);
                    mSubObj->mGroup->add_ped_leader(pNewLeader);
                    mSubObj->mPed = pNewLeader;
                }
            }
        }
    }
    switch (mState)
    {
        case police_crew_state::alerted_search_3:
            PoliceCrew::State3_AlertedSearch();
            break;
        case police_crew_state::pursue_or_chase_5:
            PoliceCrew::State5_PursueOrChase();
            break;
        case police_crew_state::shutdown_6:
            PoliceCrew::State6_ShutDown();
            break;
        case police_crew_state::patrol_1:
            PoliceCrew::State1_Patrol();
            break;
        case police_crew_state::despawning_2:
            PoliceCrew::State2_SwitchToShutDown();
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x575650)
void PoliceCrew::UnlinkFromPursuitTarget()
{
    PolicePursuitTarget* pPursuitTarget = mpPursuitTarget;
    if (pPursuitTarget)
    {
        for (u8 i = 0; i < pPursuitTarget->mNumCrews; i++)
        {
            if (this == pPursuitTarget->mCrews[i])
            {
                if (i == pPursuitTarget->mNumCrews - 1)
                {
                    pPursuitTarget->mCrews[i] = NULL;
                    pPursuitTarget->mNumCrews--;
                }
                else
                {
                    pPursuitTarget->mCrews[i] = pPursuitTarget->mCrews[pPursuitTarget->mNumCrews - 1];
                    pPursuitTarget->mCrews[pPursuitTarget->mNumCrews - 1] = NULL;
                    pPursuitTarget->mNumCrews--;
                }

                switch (mSubObj->mCrewType)
                {
                    case crew_type::police_3:
                        --mpPursuitTarget->mNumPoliceCrews;
                        break;
                    case crew_type::swat_5:
                        --mpPursuitTarget->mNumSwatCrews;
                        break;
                    case crew_type::fbi_4:
                        --mpPursuitTarget->mNumFbiCrews;
                        break;
                    case crew_type::army_6:
                        --mpPursuitTarget->mNumArmyCrews;
                        break;
                }
            }
        }
    }
}

MATCH_FUNC(0x575710)
void PoliceRoadblock::Reset()
{
    mbActive = false;
    mUnused4 = 0;
    mX = 0;
    mY = 0;
    mUnusedA = 0;
    mStartupTimer = 0;
    mCar1 = NULL;
    mCar2 = NULL;
    mCar3 = NULL;
    mCar4 = NULL;
    mCar5 = NULL;
    mCar6 = NULL;
    mBarrier1 = NULL;
    mBarrier2 = NULL;
    mBarrier3 = NULL;
    mBarrier4 = NULL;
    mBarrier5 = NULL;
    mBarrier6 = NULL;
    mBarrier7 = NULL;
    mBarrier8 = NULL;
    mBarrier9 = NULL;
    mBarrier10 = NULL;
    mBarrier11 = NULL;
    mBarrier12 = NULL;
    mBarrier1Id = 0;
    mBarrier2Id = 0;
    mBarrier3Id = 0;
    mBarrier4Id = 0;
    mBarrier5Id = 0;
    mBarrier6Id = 0;
    mBarrier7Id = 0;
    mBarrier8Id = 0;
    mBarrier9Id = 0;
    mBarrier10Id = 0;
    mBarrier11Id = 0;
    mBarrier12Id = 0;
    mGuard1 = NULL;
    mGuard2 = NULL;
    mGuard3 = NULL;
    mGuard4 = NULL;
    mGuard5 = NULL;
    mGuard6 = NULL;
}

MATCH_FUNC(0x5757b0)
void PoliceRoadblock::Update()
{
    bool bAllClear = true;

    if (mbActive)
    {
        if (mStartupTimer > 0)
        {
            mStartupTimer--;
        }
        if (!mStartupTimer)
        {
            if (mCar1)
            {
                if (mCar1->mPedType != ped_type::special_ped_4)
                {
                    mCar1 = NULL;
                }
                else
                {
                    if (!mCar1->mLastSeenTimer)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mCar2)
            {
                if (mCar2->mPedType != ped_type::special_ped_4)
                {
                    mCar2 = NULL;
                }
                else
                {
                    if (!mCar2->mLastSeenTimer)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mCar3)
            {
                if (mCar3->mPedType != ped_type::special_ped_4)
                {
                    mCar3 = NULL;
                }
                else
                {
                    if (!mCar3->mLastSeenTimer)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mCar4)
            {
                if (mCar4->mPedType != ped_type::special_ped_4)
                {
                    mCar4 = NULL;
                }
                else
                {
                    if (!mCar4->mLastSeenTimer)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mCar5)
            {
                if (mCar5->mPedType != ped_type::special_ped_4)
                {
                    mCar5 = NULL;
                }
                else
                {
                    if (!mCar5->mLastSeenTimer)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mCar6)
            {
                if (mCar6->mPedType != ped_type::special_ped_4)
                {
                    mCar6 = NULL;
                }
                else
                {
                    if (!mCar6->mLastSeenTimer)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier1)
            {
                if (mBarrier1->mId == mBarrier1Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier1->mSprite->mXy.x,
                                                                     mBarrier1->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier2)
            {
                if (mBarrier2->mId == mBarrier2Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier2->mSprite->mXy.x,
                                                                     mBarrier2->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier3)
            {
                if (mBarrier3->mId == mBarrier3Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier3->mSprite->mXy.x,
                                                                     mBarrier3->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier4)
            {
                if (mBarrier4->mId == mBarrier4Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier4->mSprite->mXy.x,
                                                                     mBarrier4->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier5)
            {
                if (mBarrier5->mId == mBarrier5Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier5->mSprite->mXy.x,
                                                                     mBarrier5->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier6)
            {
                if (mBarrier6->mId == mBarrier6Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier6->mSprite->mXy.x,
                                                                     mBarrier6->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier7)
            {
                if (mBarrier7->mId == mBarrier7Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier7->mSprite->mXy.x,
                                                                     mBarrier7->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier8)
            {
                if (mBarrier8->mId == mBarrier8Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier8->mSprite->mXy.x,
                                                                     mBarrier8->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier9)
            {
                if (mBarrier9->mId == mBarrier9Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier9->mSprite->mXy.x,
                                                                     mBarrier9->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier10)
            {
                if (mBarrier10->mId == mBarrier10Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier10->mSprite->mXy.x,
                                                                     mBarrier10->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier11)
            {
                if (mBarrier11->mId == mBarrier11Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier11->mSprite->mXy.x,
                                                                     mBarrier11->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mBarrier12)
            {
                if (mBarrier12->mId == mBarrier12Id)
                {
                    if (gGame_0x40->IsPointOnScreen(mBarrier12->mSprite->mXy.x,
                                                                     mBarrier12->mSprite->mXy.y))
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mGuard1)
            {
                if (!mGuard1->IsActive())
                {
                    mGuard1 = NULL;
                }
                else
                {
                    if (mGuard1->mOffscreenCounter < 0x50u)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mGuard2)
            {
                if (!mGuard2->IsActive())
                {
                    mGuard2 = NULL;
                }
                else
                {
                    if (mGuard2->mOffscreenCounter < 0x50u)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mGuard3)
            {
                if (!mGuard3->IsActive())
                {
                    mGuard3 = NULL;
                }
                else
                {
                    if (mGuard3->mOffscreenCounter < 0x50u)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mGuard4)
            {
                if (!mGuard4->IsActive())
                {
                    mGuard4 = NULL;
                }
                else
                {
                    if (mGuard4->mOffscreenCounter < 0x50u)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mGuard5)
            {
                if (!mGuard5->IsActive())
                {
                    mGuard5 = NULL;
                }
                else
                {
                    if (mGuard5->mOffscreenCounter < 0x50u)
                    {
                        bAllClear = false;
                    }
                }
            }

            if (mGuard6)
            {
                if (!mGuard6->IsActive())
                {
                    mGuard6 = NULL;
                }
                else
                {
                    if (mGuard6->mOffscreenCounter < 0x50u)
                    {
                        mUnseenTimer = 0;
                        return;
                    }
                }
            }
            if (bAllClear)
            {
                if (++mUnseenTimer <= 200u)
                {
                    return;
                }
                Ped* pPlayerPed = gGame_0x40->mLocalPlayer->mPlayerPed;
                if (Fix16::MaxAbsDistance(pPlayerPed->get_cam_x(),
                                                 pPlayerPed->get_cam_y(),
                                                 Fix16(mX),
                                                 Fix16(mY)) > kFpTen)
                {
                    PoliceRoadblock::RemoveRoadblock();
                    return;
                }
            }
            mUnseenTimer = 0;
        }
    }
}

MATCH_FUNC(0x575ca0)
void PoliceRoadblock::RemoveRoadblock()
{
    if (mCar1)
    {
        mCar1->MarkForDespawn();
        mCar1->SetDriverPedType(ped_type::dummy_3);
        mCar1 = NULL;
    }

    if (mCar2)
    {
        mCar2->MarkForDespawn();
        mCar2->SetDriverPedType(ped_type::dummy_3);
        mCar2 = NULL;
    }
    if (mCar3)
    {
        mCar3->MarkForDespawn();
        mCar3->SetDriverPedType(ped_type::dummy_3);
        mCar3 = NULL;
    }

    if (mCar4)
    {
        mCar4->MarkForDespawn();
        mCar4->SetDriverPedType(ped_type::dummy_3);
        mCar4 = NULL;
    }

    if (mCar5)
    {
        mCar5->MarkForDespawn();
        mCar5->SetDriverPedType(ped_type::dummy_3);
        mCar5 = NULL;
    }

    if (mCar6)
    {
        mCar6->MarkForDespawn();
        mCar6->SetDriverPedType(ped_type::dummy_3);
        mCar6 = NULL;
    }

    if (mBarrier1)
    {
        if (mBarrier1->mId == mBarrier1Id)
        {
            mBarrier1->Dealloc();
        }
        mBarrier1 = NULL;
    }

    if (mBarrier2)
    {
        if (mBarrier2->mId == mBarrier2Id)
        {
            mBarrier2->Dealloc();
        }
        mBarrier2 = NULL;
    }

    if (mBarrier3)
    {
        if (mBarrier3->mId == mBarrier3Id)
        {
            mBarrier3->Dealloc();
        }
        mBarrier3 = NULL;
    }

    if (mBarrier4)
    {
        if (mBarrier4->mId == mBarrier4Id)
        {
            mBarrier4->Dealloc();
        }
        mBarrier4 = NULL;
    }

    if (mBarrier5)
    {
        if (mBarrier5->mId == mBarrier5Id)
        {
            mBarrier5->Dealloc();
        }
        mBarrier5 = NULL;
    }

    if (mBarrier6)
    {
        if (mBarrier6->mId == mBarrier6Id)
        {
            mBarrier6->Dealloc();
        }
        mBarrier6 = NULL;
    }

    if (mBarrier7)
    {
        if (mBarrier7->mId == mBarrier7Id)
        {
            mBarrier7->Dealloc();
        }
        mBarrier7 = NULL;
    }

    if (mBarrier8)
    {
        if (mBarrier8->mId == mBarrier8Id)
        {
            mBarrier8->Dealloc();
        }
        mBarrier8 = NULL;
    }

    if (mBarrier9)
    {
        if (mBarrier9->mId == mBarrier9Id)
        {
            mBarrier9->Dealloc();
        }
        mBarrier9 = NULL;
    }

    if (mBarrier10)
    {
        if (mBarrier10->mId == mBarrier10Id)
        {
            mBarrier10->Dealloc();
        }
        mBarrier10 = NULL;
    }

    if (mBarrier11)
    {
        if (mBarrier11->mId == mBarrier11Id)
        {
            mBarrier11->Dealloc();
        }
        mBarrier11 = NULL;
    }

    if (mBarrier12)
    {
        if (mBarrier12->mId == mBarrier12Id)
        {
            mBarrier12->Dealloc();
        }
        mBarrier12 = NULL;
    }

    if (mGuard1)
    {
        if (mGuard1->mOffscreenCounter)
        {
            mGuard1->Deallocate();
        }
        else
        {
            mGuard1->SetPedType(ped_type::dummy_3);
        }
        mGuard1 = NULL;
    }

    if (mGuard2)
    {
        if (mGuard2->mOffscreenCounter)
        {
            mGuard2->Deallocate();
        }
        else
        {
            mGuard2->SetPedType(ped_type::dummy_3);
        }
        mGuard2 = NULL;
    }

    if (mGuard3)
    {
        if (mGuard3->mOffscreenCounter)
        {
            mGuard3->Deallocate();
        }
        else
        {
            mGuard3->SetPedType(ped_type::dummy_3);
        }
        mGuard3 = NULL;
    }

    if (mGuard4)
    {
        if (mGuard4->mOffscreenCounter)
        {
            mGuard4->Deallocate();
        }
        else
        {
            mGuard4->SetPedType(ped_type::dummy_3);
        }
        mGuard4 = NULL;
    }

    if (mGuard5)
    {
        if (mGuard5->mOffscreenCounter)
        {
            mGuard5->Deallocate();
        }
        else
        {
            mGuard5->SetPedType(ped_type::dummy_3);
        }
        mGuard5 = NULL;
    }

    if (mGuard6)
    {
        if (mGuard6->mOffscreenCounter)
        {
            mGuard6->Deallocate();
            mGuard6 = NULL;
            mbActive = false;
            return;
        }
        mGuard6->SetPedType(ped_type::dummy_3);
        mGuard6 = NULL;
    }
    mbActive = false;
}

DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FECEC, Fix16(1), 0x6FECEC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FEDA0, Fix16(0x100, 0), 0x6FEDA0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpSixteen, Fix16(16), 0x6FED80);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_6FED0C, Fix16(8), 0x6FED0C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeQuarters, Fix16(0.75), 0x6FEBD0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FEB50, Fix16(0xCCC, 0), 0x6FEB50);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FEB5C, Fix16(0.5), 0x6FEB5C);
DEFINE_GLOBAL(Ang16, kAng0_6FEE30, 0x6FEE30);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FEB74, Ang16(360), 0x6FEB74);
DEFINE_STATIC_GLOBAL_INIT(u8, sRoadblockLaneHasBarriers, 1, 0x624FBC); // the next roadblock lane gets barriers
DEFINE_STATIC_GLOBAL_INIT(u8, sRoadblockLaneHasGuard, 1, 0x624FBD); // the next roadblock lane gets a guard

// The "first free slot" fills for the cars, barriers and guards are written out in the body, as in
// 9.6f (built with /Ob0, so they were never inline helpers). As inline helpers they used up VC6's
// inline budget and the x branch's Ang16 Normalize went out of line.
MATCH_FUNC(0x575ff0)
bool PoliceRoadblock::CreateRoadblock(u8 x, u8 y, u8 z, s32 orientation)
{
    Car* pCar = NULL;
    bool bEdge; // tested with == 1 below: a plain test changes the codegen
    u8 tries;
    u8 width;
    u8 x_start;
    u8 y_start;
    u8 lane;
    Ang16 angle;
    Fix16 zpos;
    Fix16 x_left;
    Fix16 y_top;
    Fix16 x_right;
    Fix16 y_bottom;

    if (orientation == 2)
    {
        // Find the road's edges along y
        x_start = x;
        bEdge = false;
        tries = 0;
        s32 z_below = z - 1;
        bool bFound;
        do
        {
            bFound = false;
            y--;
            switch (gMap_0x370->GetBlockTypeAtCoord(x, y, z_below))
            {
                case AIR:
                    bFound = true;
                    y++;
                    break;
                case PAVEMENT:
                case FIELD:
                    if (!bEdge)
                    {
                        bEdge = true;
                    }
                    else
                    {
                        bFound = true;
                    }
                    break;
                case ROAD:
                    break;
                case 4:
                    return false;
                default:
                    return false;
            }
            if (++tries > 12)
            {
                return false;
            }
        } while (!bFound);

        width = 0;
        if (bEdge == 1)
        {
            width = 1;
            y++;
        }
        y_start = y;
        bEdge = false;
        tries = 0;
        do
        {
            bFound = false;
            y++;
            switch (gMap_0x370->GetBlockTypeAtCoord(x, y, z_below))
            {
                case ROAD:
                    width++;
                    break;
                case PAVEMENT:
                    if (!bEdge)
                    {
                        width++;
                        bEdge = true;
                    }
                    else
                    {
                        bFound = true;
                    }
                    break;
                case FIELD:
                    if (!bEdge)
                    {
                        width++;
                        bEdge = true;
                    }
                    else
                    {
                        bFound = true;
                    }
                    break;
                case AIR:
                    bFound = true;
                    break;
                case 4:
                    return false;
                default:
                    return false;
            }
            if (++tries > 12)
            {
                return false;
            }
        } while (!bFound);

        if (width > 12)
        {
            return false;
        }

        x_left = Fix16(x_start) + kFpHalf_6FEBF4;
        y_top = Fix16(y_start);
        x_right = Fix16(x_start) + kFpHalf_6FEBF4;
        y_bottom = Fix16(y_start + width + 1);
        zpos = Fix16(z);
        mRect->SetBounds(x_left - kFpHalf_6FEBF4, x_right + kFpHalf_6FEBF4, y_top, y_bottom);
        mRect->SetHiLowZ(zpos - kFpOne_6FECEC, zpos + kFpOne_6FECEC);
        if (gSpriteGrid_1->CheckRectForCollisions(mRect, false, sprite_types_enum::none_0, NULL))
        {
            return false;
        }
        if (gMap_0x370->HasWallInArea((x_left - kFpHalf_6FEBF4).ToInt(),
                                                    (x_right + kFpHalf_6FEBF4 - kFpOne64th_6FEDA0).ToInt(),
                                                    y_top.ToInt(),
                                                    (y_bottom - kFpOne64th_6FEDA0).ToInt(),
                                                    zpos.ToInt()))
        {
            return false;
        }

        Reset();
        for (lane = 0; lane < width; lane++)
        {
            if (lane % 2)
            {
                if (lane > 0 && lane < width - 1)
                {
                    s16 max = 32;
                    angle = Ang16::Fix16_To_Ang16_40F540((Fix16(gRng.get_int(max)) - kFpSixteen) * kFpOne64th_6FEB88) +
                        kAng0_6FEE30;
                }
                else
                {
                    angle = kAng0_6FEE30;
                }

                if (gCarManager_6C->CanAllocateCarOfKind(car_kind::roadblock_car_7))
                {
                    switch (gCrewKind)
                    {
                        default:
                            break;
                        case crew_kind::police_1:
                            pCar = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(x_start) + kFpHalf_6FEBF4,
                                                                             Fix16(y_start + lane) + kFpHalf_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::COPCAR,
                                                                             kFpOne_6FECEC);
                            sRoadblockLaneHasBarriers = 1;
                            sRoadblockLaneHasGuard = 1;
                            break;
                        case crew_kind::swat_2:
                            pCar = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(x_start) + kFpHalf_6FEBF4,
                                                                             Fix16(y_start + lane) + kFpHalf_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::COPCAR,
                                                                             kFpOne_6FECEC);
                            sRoadblockLaneHasBarriers = 1;
                            sRoadblockLaneHasGuard = 1;
                            break;
                        case crew_kind::fbi_3:
                            pCar = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(x_start) + kFpHalf_6FEBF4,
                                                                             Fix16(y_start + lane) + kFpHalf_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::EDSELFBI,
                                                                             kFpOne_6FECEC);
                            sRoadblockLaneHasBarriers = 1;
                            sRoadblockLaneHasGuard = 1;
                            break;
                        case crew_kind::army_4:
                            if (lane != width - 1)
                            {
                                pCar = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(x_start) + kFpHalf_6FEBF4,
                                                                                 Fix16(y_start + lane) + kFpThreeQuarters,
                                                                                 angle,
                                                                                 car_model_enum::TANK,
                                                                                 kFpOne_6FECEC);
                                sRoadblockLaneHasBarriers = 0;
                                sRoadblockLaneHasGuard = 0;
                                Ped* pDriver = gPedManager->SpawnDriver(pCar);
                                pDriver->SetPedType(ped_type::script_created_5);
                                pDriver->set_occupation(ped_ocupation_enum::road_block_tank_man);
                                pDriver->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
                                pCar->mAttachments.GetSpriteForModel(objects::tank_cannon_148)->mOffsetRot =
                                    kAng90_6FEB74;
                            }
                            break;
                    }
                    if (pCar)
                    {
                        pCar->SetDriverPedType(ped_type::special_ped_4);
                        pCar->IncrementCarStats(car_kind::roadblock_car_7);
                        if (pCar->HasEmergencyLights())
                        {
                            pCar->ActivateEmergencyLights();
                        }
                        if (!mCar1)
                        {
                            mCar1 = pCar;
                        }
                        else if (!mCar2)
                        {
                            mCar2 = pCar;
                        }
                        else if (!mCar3)
                        {
                            mCar3 = pCar;
                        }
                        else if (!mCar4)
                        {
                            mCar4 = pCar;
                        }
                        else if (!mCar5)
                        {
                            mCar5 = pCar;
                        }
                        else
                        {
                            mCar6 = pCar;
                        }
                    }
                }
            }
            else
            {
                if (sRoadblockLaneHasBarriers)
                {
                    Object* pBarrier1 = gObject_5C->NewPhysicsObj(objects::roadblock_21,
                                                                                Fix16(x_start) + kFpPoint2_6FEB50,
                                                                                Fix16(y_start + lane) + kFpHalf_6FEBF4,
                                                                                z,
                                                                                kAng90_6FEB74);
                    Object* pBarrier2 = gObject_5C->NewPhysicsObj(objects::roadblock_21,
                                                                                Fix16(x_start) + kFpPoint8_6FEB68,
                                                                                Fix16(y_start + lane) + kFpHalf_6FEBF4,
                                                                                z,
                                                                                kAng90_6FEB74);
                    if (!mBarrier1)
                    {
                        mBarrier1 = pBarrier1;
                        mBarrier2 = pBarrier2;
                        mBarrier1Id = pBarrier1->mId;
                        mBarrier2Id = pBarrier2->mId;
                    }
                    else if (!mBarrier3)
                    {
                        mBarrier3 = pBarrier1;
                        mBarrier4 = pBarrier2;
                        mBarrier3Id = pBarrier1->mId;
                        mBarrier4Id = pBarrier2->mId;
                    }
                    else if (!mBarrier5)
                    {
                        mBarrier5 = pBarrier1;
                        mBarrier6 = pBarrier2;
                        mBarrier5Id = pBarrier1->mId;
                        mBarrier6Id = pBarrier2->mId;
                    }
                    else if (!mBarrier7)
                    {
                        mBarrier7 = pBarrier1;
                        mBarrier8 = pBarrier2;
                        mBarrier7Id = pBarrier1->mId;
                        mBarrier8Id = pBarrier2->mId;
                    }
                    else if (!mBarrier9)
                    {
                        mBarrier9 = pBarrier1;
                        mBarrier10 = pBarrier2;
                        mBarrier9Id = pBarrier1->mId;
                        mBarrier10Id = pBarrier2->mId;
                    }
                    else if (!mBarrier11)
                    {
                        mBarrier11 = pBarrier1;
                        mBarrier12 = pBarrier2;
                        mBarrier11Id = pBarrier1->mId;
                        mBarrier12Id = pBarrier2->mId;
                    }
                }
                if (sRoadblockLaneHasGuard)
                {
                    Ped* pGuard = gPolice_7B8->SpawnRoadblockGuard(Fix16(x_start) + kFpHalf_6FEB5C,
                                                                            Fix16(y_start + lane) + kFpHalf_6FEBF4,
                                                                            z,
                                                                            kAng90_6FEB74);
                    if (!mGuard1)
                    {
                        mGuard1 = pGuard;
                    }
                    else if (!mGuard2)
                    {
                        mGuard2 = pGuard;
                    }
                    else if (!mGuard3)
                    {
                        mGuard3 = pGuard;
                    }
                    else if (!mGuard4)
                    {
                        mGuard4 = pGuard;
                    }
                    else if (!mGuard5)
                    {
                        mGuard5 = pGuard;
                    }
                    else if (!mGuard6)
                    {
                        mGuard6 = pGuard;
                    }
                }
            }
        }
    }
    else
    {
        // Find the road's edges along x
        y_start = y;
        bEdge = false;
        tries = 0;
        s32 z_below = z - 1;
        bool bFound;
        do
        {
            bFound = false;
            x--;
            switch (gMap_0x370->GetBlockTypeAtCoord(x, y, z_below))
            {
                case AIR:
                    bFound = true;
                    x++;
                    break;
                case PAVEMENT:
                case FIELD:
                    if (!bEdge)
                    {
                        bEdge = true;
                    }
                    else
                    {
                        bFound = true;
                    }
                    break;
                case ROAD:
                    break;
                case 4:
                    return false;
                default:
                    return false;
            }
            if (++tries > 12)
            {
                return false;
            }
        } while (!bFound);

        width = 0;
        if (bEdge == 1)
        {
            width = 1;
            x++;
        }
        x_start = x;
        bEdge = false;
        tries = 0;
        do
        {
            bFound = false;
            x++;
            switch (gMap_0x370->GetBlockTypeAtCoord(x, y, z_below))
            {
                case ROAD:
                    width++;
                    break;
                case PAVEMENT:
                    if (!bEdge)
                    {
                        width++;
                        bEdge = true;
                    }
                    else
                    {
                        bFound = true;
                    }
                    break;
                case FIELD:
                    if (!bEdge)
                    {
                        width++;
                        bEdge = true;
                    }
                    else
                    {
                        bFound = true;
                    }
                    break;
                case AIR:
                    bFound = true;
                    break;
                case 4:
                    return false;
                default:
                    return false;
            }
            if (++tries > 12)
            {
                return false;
            }
        } while (!bFound);

        if (width > 12)
        {
            return false;
        }

        x_left = Fix16(x_start);
        y_top = Fix16(y_start) + kFpHalf_6FEBF4;
        x_right = Fix16(x_start + width + 1);
        y_bottom = Fix16(y_start) + kFpHalf_6FEBF4;
        zpos = Fix16(z);
        mRect->SetBounds(x_left, x_right, y_top - kFpHalf_6FEBF4, y_bottom + kFpHalf_6FEBF4);
        mRect->SetHiLowZ(zpos - kFpOne_6FECEC, zpos + kFpOne_6FECEC);
        if (gSpriteGrid_1->CheckRectForCollisions(mRect, false, sprite_types_enum::none_0, NULL))
        {
            return false;
        }
        if (gMap_0x370->HasWallInArea(x_left.ToInt(),
                                                    (x_right - kFpOne64th_6FEDA0).ToInt(),
                                                    (y_top - kFpHalf_6FEBF4).ToInt(),
                                                    (y_bottom + kFpHalf_6FEBF4 - kFpOne64th_6FEDA0).ToInt(),
                                                    zpos.ToInt()))
        {
            return false;
        }

        Reset();
        for (lane = 0; lane < width; lane++)
        {
            if (lane % 2)
            {
                if (lane > 0 && lane < width - 1)
                {
                    s16 max = 16;
                    angle =
                        Ang16::Fix16_To_Ang16_40F540((Fix16(gRng.get_int(max)) - kFpEight_6FED0C) * kFpOne64th_6FEB88) + kAng90_6FEB74;
                }
                else
                {
                    angle = kAng90_6FEB74;
                }

                if (gCarManager_6C->CanAllocateCarOfKind(car_kind::roadblock_car_7))
                {
                    switch (gCrewKind)
                    {
                        default:
                            break;
                        case crew_kind::police_1:
                            pCar = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(x_start + lane) + kFpHalf_6FEBF4,
                                                                             Fix16(y_start) + kFpHalf_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::COPCAR,
                                                                             kFpOne_6FECEC);
                            sRoadblockLaneHasBarriers = 1;
                            break;
                        case crew_kind::swat_2:
                            pCar = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(x_start + lane) + kFpHalf_6FEBF4,
                                                                             Fix16(y_start) + kFpHalf_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::COPCAR,
                                                                             kFpOne_6FECEC);
                            sRoadblockLaneHasBarriers = 1;
                            break;
                        case crew_kind::fbi_3:
                            pCar = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(x_start + lane) + kFpHalf_6FEBF4,
                                                                             Fix16(y_start) + kFpHalf_6FEBF4,
                                                                             angle,
                                                                             car_model_enum::EDSELFBI,
                                                                             kFpOne_6FECEC);
                            sRoadblockLaneHasBarriers = 1;
                            break;
                        case crew_kind::army_4:
                            if (lane != width - 1)
                            {
                                pCar = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(x_start + lane) + kFpThreeQuarters,
                                                                                 Fix16(y_start) + kFpHalf_6FEBF4,
                                                                                 angle,
                                                                                 car_model_enum::TANK,
                                                                                 kFpOne_6FECEC);
                                sRoadblockLaneHasBarriers = 0;
                                sRoadblockLaneHasGuard = 0;
                                Ped* pDriver = gPedManager->SpawnDriver(pCar);
                                pDriver->SetPedType(ped_type::script_created_5);
                                pDriver->set_occupation(ped_ocupation_enum::road_block_tank_man);
                                pDriver->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
                            }
                            break;
                    }
                    if (pCar)
                    {
                        pCar->SetDriverPedType(ped_type::special_ped_4);
                        pCar->IncrementCarStats(car_kind::roadblock_car_7);
                        if (pCar->HasEmergencyLights())
                        {
                            pCar->ActivateEmergencyLights();
                        }
                        if (!mCar1)
                        {
                            mCar1 = pCar;
                        }
                        else if (!mCar2)
                        {
                            mCar2 = pCar;
                        }
                        else if (!mCar3)
                        {
                            mCar3 = pCar;
                        }
                        else if (!mCar4)
                        {
                            mCar4 = pCar;
                        }
                        else if (!mCar5)
                        {
                            mCar5 = pCar;
                        }
                        else
                        {
                            mCar6 = pCar;
                        }
                    }
                }
            }
            else
            {
                if (sRoadblockLaneHasBarriers)
                {
                    Object* pBarrier1 = gObject_5C->NewPhysicsObj(objects::roadblock_21,
                                                                                Fix16(x_start + lane) + kFpHalf_6FEBF4,
                                                                                Fix16(y_start) + kFpPoint2_6FEB50,
                                                                                z,
                                                                                kAng0_6FEE30);
                    Object* pBarrier2 = gObject_5C->NewPhysicsObj(objects::roadblock_21,
                                                                                Fix16(x_start + lane) + kFpHalf_6FEBF4,
                                                                                Fix16(y_start) + kFpPoint8_6FEB68,
                                                                                z,
                                                                                kAng0_6FEE30);
                    if (!mBarrier1)
                    {
                        mBarrier1 = pBarrier1;
                        mBarrier2 = pBarrier2;
                        mBarrier1Id = pBarrier1->mId;
                        mBarrier2Id = pBarrier2->mId;
                    }
                    else if (!mBarrier3)
                    {
                        mBarrier3 = pBarrier1;
                        mBarrier4 = pBarrier2;
                        mBarrier3Id = pBarrier1->mId;
                        mBarrier4Id = pBarrier2->mId;
                    }
                    else if (!mBarrier5)
                    {
                        mBarrier5 = pBarrier1;
                        mBarrier6 = pBarrier2;
                        mBarrier5Id = pBarrier1->mId;
                        mBarrier6Id = pBarrier2->mId;
                    }
                    else if (!mBarrier7)
                    {
                        mBarrier7 = pBarrier1;
                        mBarrier8 = pBarrier2;
                        mBarrier7Id = pBarrier1->mId;
                        mBarrier8Id = pBarrier2->mId;
                    }
                    else if (!mBarrier9)
                    {
                        mBarrier9 = pBarrier1;
                        mBarrier10 = pBarrier2;
                        mBarrier9Id = pBarrier1->mId;
                        mBarrier10Id = pBarrier2->mId;
                    }
                    else if (!mBarrier11)
                    {
                        mBarrier11 = pBarrier1;
                        mBarrier12 = pBarrier2;
                        mBarrier11Id = pBarrier1->mId;
                        mBarrier12Id = pBarrier2->mId;
                    }
                }
                if (sRoadblockLaneHasGuard)
                {
                    Ped* pGuard = gPolice_7B8->SpawnRoadblockGuard(Fix16(x_start + lane) + kFpHalf_6FEBF4,
                                                                            Fix16(y_start) + kFpHalf_6FEB5C,
                                                                            z,
                                                                            kAng0_6FEE30);
                    if (!mGuard1)
                    {
                        mGuard1 = pGuard;
                    }
                    else if (!mGuard2)
                    {
                        mGuard2 = pGuard;
                    }
                    else if (!mGuard3)
                    {
                        mGuard3 = pGuard;
                    }
                    else if (!mGuard4)
                    {
                        mGuard4 = pGuard;
                    }
                    else if (!mGuard5)
                    {
                        mGuard5 = pGuard;
                    }
                    else if (!mGuard6)
                    {
                        mGuard6 = pGuard;
                    }
                }
            }
        }
    }

    mbActive = true;
    mStartupTimer = 100;
    return true;
}

MATCH_FUNC(0x577480)
PoliceRoadblock::PoliceRoadblock()
{
    Reset();
    mRect = new Fix16_Rect();
}

MATCH_FUNC(0x5774a0)
PoliceRoadblock::~PoliceRoadblock()
{
    delete[] mRect;
}