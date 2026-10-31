#include "AmbulanceService.hpp"
#include "ped_jump_over_mode.hpp"
#include "Char_Pool.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "EmergencyCrewPool.hpp"
#include "Car.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "PathFinder.hpp"
#include "CarAI.hpp"
#include "CarChaseTaskTable.hpp"
#include "RouteFinder.hpp"
#include <stdio.h>
#include "ped_graphic_type.hpp"

DEFINE_GLOBAL(AmbulanceService*, gAmbulance_110, 0x6F70A8);

DEFINE_STATIC_MEMBER(class Ped*, AmbulanceTask, sParamedicCrewPed, 0x6F6D60);
DEFINE_STATIC_GLOBAL_INIT(Fix16, sParamedicRunSpeed, Fix16(0x1999, 0), 0x6F6DD4);

MATCH_FUNC(0x4beab0)
AmbulanceTask::AmbulanceTask()
{
    mPatients.ClearList_420E90();
    ClearTask();
}

MATCH_FUNC(0x4bead0)
AmbulanceTask::~AmbulanceTask()
{
}

MATCH_FUNC(0x4beae0)
AmbulanceService::~AmbulanceService()
{
}

MATCH_FUNC(0x4fa310)
void AmbulanceService::init()
{
    mUnused0 = 1;
    mCorpseCount = 0;
    mPatientQueue.ClearList_420E90();

    for (s32 i = 0; i < 25; i++)
    {
        mUnusedSlots[i].mUnused0 = 0;
        mUnusedSlots[i].mUnused4 = 0;
    }
}

MATCH_FUNC(0x4fa330)
bool AmbulanceService::HandlePedDeath(Ped* pDeadPed)
{
    for (u8 i = 0; i < 2; i++)
    {
        AmbulanceTask* pTask = &mTasks[i];
        if (pTask->mInUse)
        {
            if (pTask->mParamedicsCrew->mPed == pDeadPed) // the crew leader died
            {
                bool bLeaderReplaced = pTask->mParamedicsCrew->ReplaceLeaderIfNeeded();
                if (pTask->mParamedicsCrew->mGroup)
                {
                    pTask->mParamedicsCrew->mPed = pTask->mParamedicsCrew->mGroup->mPedLeader;
                }

                if (pTask->mLeaderPatient && pTask->mLeaderPatient->GetPedState_403990() == ped_state_1::dead_9)
                {
                    TryAddPatient(pTask->mLeaderPatient);
                    pTask->mLeaderPatient = NULL;
                }

                if (pTask->mMemberPatient && pTask->mMemberPatient->GetPedState_403990() == ped_state_1::dead_9)
                {
                    TryAddPatient(pTask->mMemberPatient);
                    pTask->mMemberPatient = NULL;
                }

                if (bLeaderReplaced)
                {
                    return true;
                }

                if (pDeadPed->mGameObject)
                {
                    pDeadPed->SetObjective(objectives_enum::dead_patient_28);
                    return false;
                }
                return false;
            }

            if (pTask->mParamedicsCrew->mGroup && pTask->mParamedicsCrew->mGroup->mPedList[0] == pDeadPed)
            {
                if (pDeadPed->mCar)
                {
                    pTask->mParamedicsCrew->RemovePed(pDeadPed);
                }

                if (pTask->mMemberPatient && pTask->mMemberPatient->GetPedState_403990() == ped_state_1::dead_9)
                {
                    TryAddPatient(pTask->mMemberPatient);
                }

                if (pDeadPed->mGameObject)
                {
                    pDeadPed->SetObjective(objectives_enum::dead_patient_28);
                }

                return false;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4fa470)
bool AmbulanceService::TryAddPatient(Ped* pPed)
{
    if (pPed->PedTypeIs(ped_type::player_2) || mCorpseCount >= 25)
    {
        return false;
    }

    mPatientQueue.AddPedToFront(pPed);
    mCorpseCount++;

    return true;
}

EXTERN_GLOBAL(Fix16, kFpEight_6F6FC0);

MATCH_FUNC(0x4fa4b0)
AmbulanceTask* AmbulanceService::AllocateTaskSlot()
{
    for (u8 i = 0; i < 2; i++)
    {
        if (!mTasks[i].mInUse)
        {
            return &mTasks[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4fa500)
void AmbulanceService::ProcessPatientQueue()
{
    u8 x, y, z;
    mCorpseCount -= mPatientQueue.RemoveNonCorpses();
    if (mCorpseCount == 0)
    {
        return;
    }

    Ped* pPed = mPatientQueue.RemoveFirstPed();
    if (pPed->IsNearestSpriteACar())
    {
        mCorpseCount--;
        gAmbulance_110->TryAddPatient(pPed);
        return;
    }

    {
        x = pPed->get_cam_x().ToInt();
        y = pPed->get_cam_y().ToInt();
        z = pPed->get_cam_z().ToInt();
        if (!gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &x, &y, &z, false))
        {
            mCorpseCount--;
            pPed->SetObjective(objectives_enum::dead_no_ambulance_50);
            return;
        }

        for (u8 i = 0; i < 2; i++)
        {
            AmbulanceTask* pAmbulance = &mTasks[i];
            if (1 == pAmbulance->mInUse && pAmbulance->mParamedicsCrew->IsLeaderAlive())
            {
                if (Fix16::MaxAbsDistance_42A6B0(pPed->get_cam_x(),
                                                 pPed->get_cam_y(),
                                                 Fix16(pAmbulance->mTargetX),
                                                 Fix16(pAmbulance->mTargetY)) < kFpEight_6F6FC0 &&
                    pAmbulance->mPatientCount < 10)
                {
                    pAmbulance->AddPatient(pPed);
                    mCorpseCount--;
                    EmergencyCrew* pCrew = pAmbulance->mParamedicsCrew;
                    if (pCrew->mState != crew_state::update_6)
                    {
                        if (pCrew->mState == crew_state::clean_up_5)
                        {
                            pAmbulance->mTargetX = x;
                            pAmbulance->mTargetY = y;
                            pAmbulance->mTargetZ = z;
                            pCrew->mState = crew_state::update_6;
                        }
                    }
                    return;
                }
            }
        }

        AmbulanceTask* pNew = AllocateTaskSlot();
        if (pNew)
        {
            pNew->mInUse = 1;
            pNew->mTargetX = x;
            pNew->mTargetY = y;
            pNew->mTargetZ = z;
            pNew->mParamedicsCrew = gEmergencyCrewPool->AllocateSlot();
            if (!pNew->mParamedicsCrew)
            {
                mCorpseCount--;
                pPed->SetObjective(objectives_enum::dead_no_ambulance_50);
                pNew->ClearTask();
                return;
            }

            EmergencyCrew* pCrew = pNew->mParamedicsCrew;
            pCrew->mIsUsed = true;
            pCrew->mCrewType = crew_type::paramedic_1;
            pCrew->mPhase = crew_phase::in_car_1;
            pCrew->mState = crew_state::spawn_car_3;
            pCrew->mSpawnDelay = 300;
            pCrew->mSpawnWaitTicks = 0;
            pCrew->mSpawnX = Fix16(x);
            pCrew->mSpawnY = Fix16(y);
            pCrew->mSpawnZ = Fix16(z);
            pNew->AddPatient(pPed);
        }
    }
    mCorpseCount--;
}

MATCH_FUNC(0x4fa790)
void AmbulanceService::AmbulancesService()
{
    if (mCorpseCount > 0)
    {
        ProcessPatientQueue();
    }

    for (s32 i = 0; i < 2; i++)
    {
        if (mTasks[i].mInUse == 1)
        {
            mTasks[i].UpdateState();
        }
    }
}

MATCH_FUNC(0x4fa7d0)
void AmbulanceTask::ClearTask()
{
    mPatients.ClearList_420E90();
    mTargetX = 0;
    mTargetY = 0;
    mTargetZ = 0;
    mPatientCount = 0;
    mUnused16 = 0;
    mInUse = 0;
    mDone = false;
    mParamedicsCrew = NULL;
    mLeaderPatient = NULL;
    mMemberPatient = NULL;
    mReviveTicks = 0;
}

MATCH_FUNC(0x4fa800)
void AmbulanceTask::AddPatient(Ped* pPed)
{
    mPatients.AddPedToFront(pPed);
    mPatientCount++;
}

MATCH_FUNC(0x4fa820)
bool AmbulanceTask::SpawnParamedicCrew()
{
    PedGroup* pGroup = PedGroup::New();
    if (!pGroup)
    {
        return false;
    }

    if (gPedManager->mFbiArmyCount >= 30u)
    {
        return false;
    }

    Ped* pPed1 = gPedManager->AllocatePed();
    if (!pPed1)
    {
        return false;
    }
    pPed1->SetPedType_403920(ped_type::special_ped_4);
    pPed1->set_occupation_403970(ped_ocupation_enum::paramedic_23);
    pPed1->SetJumpOverMode_433BB0(ped_jump_over_mode::can_jump_2);
    pPed1->SpawnPedInCar(mParamedicsCrew->mCar);
    pPed1->SetObjective(objectives_enum::goto_area_in_car_14, 0);
    pPed1->mObjectiveTargetX = this->mTargetX << 14;
    pPed1->mObjectiveTargetY = this->mTargetY << 14;
    pPed1->mObjectiveTargetZ = this->mTargetZ << 14;
    pPed1->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
    pPed1->mThreatSearch = threat_search_enum::no_threats_0;
    pPed1->set_remap_433B90(ped_remap_enum::ped_remap_medic);
    pPed1->mGraphicType = ped_graphic_type::civilian_0;
    pPed1->mRunSpeed = sParamedicRunSpeed;

    Ped* pPed2 = gPedManager->AllocatePed();
    if (!pPed2)
    {
        return false;
    }

    pPed2->EnterCarAsPassenger(mParamedicsCrew->mCar);
    pPed2->SetPedType_403920(ped_type::special_ped_4);
    pPed2->set_occupation_403970(ped_ocupation_enum::paramedic_23);
    pPed2->SetJumpOverMode_433BB0(ped_jump_over_mode::can_jump_2);
    pPed2->SetObjective(objectives_enum::no_obj_0);
    pPed2->set_remap_433B90(ped_remap_enum::ped_remap_medic);
    pPed2->mGraphicType = ped_graphic_type::civilian_0;
    pPed2->mThreatReaction = threat_reaction_enum::react_as_emergency_1;
    pPed2->mThreatSearch = threat_search_enum::no_threats_0;
    pGroup->add_ped_leader(pPed1);
    pGroup->SetCounts_433360(1);
    pGroup->add_ped_to_list(pPed2, 0);
    pGroup->mRemoveDeadMembers = false;

    mParamedicsCrew->mPed = pPed1;
    mParamedicsCrew->mState = crew_state::update_6;
    mParamedicsCrew->mCar->SetDriverPedType_421560(ped_type::special_ped_4);
    mParamedicsCrew->mCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
    mParamedicsCrew->mCar->InitCarAIControl();
    mParamedicsCrew->mCar->ResumeAIDriving();
    mParamedicsCrew->mGroup = pGroup;
    return true;
}

MATCH_FUNC(0x4fa9d0)
void AmbulanceTask::EvaluatePickupState()
{
    bool bPhaseFinished = false;
    if (mParamedicsCrew->mPhase == crew_phase::finished_2)
    {
        bPhaseFinished = true;
        if (!mParamedicsCrew->mPed || mParamedicsCrew->mPed->isDead_403B60())
        {
            mParamedicsCrew->mState = crew_state::clean_up_5;
            mParamedicsCrew->mReady = false;
            sParamedicCrewPed = NULL;
            return;
        }
    }
    if (bPhaseFinished)
    {
        if (mParamedicsCrew->mCar)
        {
            if (mParamedicsCrew->mCar->IsMaxDamage_40F890())
            {
                mParamedicsCrew->mCar = NULL;
                mParamedicsCrew->mPhase = crew_phase::on_foot_0;
            }
        }
        else
        {
            mParamedicsCrew->mPhase = crew_phase::on_foot_0;
        }
    }

    sParamedicCrewPed = mParamedicsCrew->mPed;
    if (!sParamedicCrewPed)
    {
        mParamedicsCrew->mPhase = crew_phase::finished_2;
    }
    else if (mDone)
    {
        if (mParamedicsCrew->mPhase == crew_phase::on_foot_0)
        {
            mParamedicsCrew->mState = crew_state::clean_up_5;
            mParamedicsCrew->mReady = false;
        }
        else if (sParamedicCrewPed->mCar && sParamedicCrewPed == sParamedicCrewPed->mCar->mDriver)
        {
            if (mParamedicsCrew->mGroup)
            {
                if (mParamedicsCrew->mGroup->IsAllMembersInSomeCar())
                {
                    mParamedicsCrew->mCar->ResumeAIDriving();
                    mParamedicsCrew->mState = crew_state::clean_up_5;
                    mParamedicsCrew->mReady = false;
                    sParamedicCrewPed = NULL;
                }
                else
                {
                    mParamedicsCrew->mCar->HaltAIDriving();
                }
            }
        }
    }
}

DEFINE_GLOBAL_INIT(Fix16, kFpEight_6F6FC0, Fix16(8), 0x6F6FC0);

// Runs the paramedic crew (the leader, then each group member, in sParamedicCrewPed): pick up the
// patients in mPatients one by one, revive them, then drive off. Sets mDone when nobody had
// anything left to do.
MATCH_FUNC(0x4faac0)
void AmbulanceTask::HandleObjectiveState()
{
    bool bBusy = false;
    bool bParamedicVictim = false;
    u8 i = 0;
    sParamedicCrewPed = mParamedicsCrew->mPed;
    bool bNoCar = mParamedicsCrew->mPhase == crew_phase::on_foot_0;
    Ped* pTarget = mLeaderPatient;

    while (sParamedicCrewPed)
    {
        if (pTarget)
        {
            if (!pTarget->IsActive_433B40())
            {
                if (sParamedicCrewPed->GetPedState_403990() != ped_state_1::dead_9)
                {
                    sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                }
                if (pTarget == mLeaderPatient)
                {
                    mLeaderPatient = NULL;
                }
                else
                {
                    mMemberPatient = NULL;
                }
            }
            else if (sParamedicCrewPed->mGameObject && pTarget->IsNearestSpriteACar())
            {
                gAmbulance_110->TryAddPatient(pTarget);
                if (sParamedicCrewPed->GetPedState_403990() != ped_state_1::dead_9)
                {
                    sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                }
                if (pTarget == mLeaderPatient)
                {
                    mLeaderPatient = NULL;
                }
                else
                {
                    mMemberPatient = NULL;
                }
            }
        }

        Ped* pPatient;
        switch (sParamedicCrewPed->get_objective_403A80())
        {
            case objectives_enum::goto_area_in_car_14:
                bBusy = true;
                if (sParamedicCrewPed->mCar)
                {
                    Car* pCar = mParamedicsCrew->mCar;
                    if (pCar->mLastSeenTimer > 1000)
                    {
                        bBusy = false;
                    }
                    if (sParamedicCrewPed->GetObjectiveStatus_450CB0() == objective_status::passed_1)
                    {
                        pCar->HaltAIDriving();
                        mParamedicsCrew->mState = crew_state::update_6;
                        sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                    }
                    else
                    {
                        // 9.6f calls MaxAbsDistance_42A6B0(get_cam_x(), get_cam_y(), ...)
                        if (Fix16::MaxAbsDistance_42A6B0(sParamedicCrewPed->get_cam_x(),
                                                            sParamedicCrewPed->get_cam_y(),
                                                            Fix16(mTargetX),
                                                            Fix16(mTargetY)) >= kFpEight_6F6FC0)
                        {
                            break;
                        }
                        if (!mPatients.GetPedsCount())
                        {
                            bBusy = false;
                            break;
                        }
                        if (sParamedicCrewPed->get_objective_timer_403B30() <= 50)
                        {
                            break;
                        }
                        mParamedicsCrew->mCar->HaltAIDriving();
                        mParamedicsCrew->mState = crew_state::update_6;
                        sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                    }
                }
                else
                {
                    mParamedicsCrew->mState = crew_state::update_6;
                    sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                }
                break;

            case objectives_enum::no_obj_0:
                if (sParamedicCrewPed->IsGroupLeader_472FD0())
                {
                    pPatient = mPatients.RemoveFirstPed();
                    if (!pPatient)
                    {
                        if (sParamedicCrewPed->mCar)
                        {
                            sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                            sParamedicCrewPed->ChangeNextPedState1(ped_state_1::in_car_10);
                            sParamedicCrewPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
                            if (mParamedicsCrew->mCar)
                            {
                                CarAI* pAI = mParamedicsCrew->mCar->mAI;
                                if (pAI && pAI->mRouteIdx > 0)
                                {
                                    gRouteFinder->CancelRoute(pAI->mRouteIdx);
                                    mParamedicsCrew->mCar->mAI->mRouteIdx = -1;
                                }
                                if (mParamedicsCrew->mCar->mpChaseTask)
                                {
                                    gCarChaseTaskTable->FreeEntry(mParamedicsCrew->mCar->mpChaseTask);
                                    mParamedicsCrew->mCar->mpChaseTask = NULL;
                                }
                                if (mParamedicsCrew->mGroup->IsAllMembersInSomeCar())
                                {
                                    mParamedicsCrew->mCar->ResumeAIDriving();
                                    bBusy = false;
                                }
                                else
                                {
                                    mParamedicsCrew->mCar->HaltAIDriving();
                                }
                            }
                        }
                        else if (bNoCar || (sParamedicCrewPed->mBf21C.bLeftVehicle))
                        {
                            bBusy = false;
                        }
                        else
                        {
                            sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                            sParamedicCrewPed->SetObjective(objectives_enum::enter_car_as_driver_35);
                            sParamedicCrewPed->SetTargetObjectiveCar(mParamedicsCrew->mCar);
                            sParamedicCrewPed->set_enter_car_as_passenger_4039B0(0);
                            sParamedicCrewPed->set_target_car_door_403A70(0);
                        }
                    }
                    else if (pPatient->IsNearestSpriteACar())
                    {
                        if (sParamedicCrewPed->mCar)
                        {
                            sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                            sParamedicCrewPed->ChangeNextPedState1(ped_state_1::in_car_10);
                            sParamedicCrewPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
                        }
                        else if (!bNoCar && !sParamedicCrewPed->mBf21C.bLeftVehicle)
                        {
                            sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                            sParamedicCrewPed->SetObjective(objectives_enum::enter_car_as_driver_35);
                            sParamedicCrewPed->SetTargetObjectiveCar(mParamedicsCrew->mCar);
                            sParamedicCrewPed->set_enter_car_as_passenger_4039B0(0);
                            sParamedicCrewPed->set_target_car_door_403A70(0);
                        }
                        bBusy = true;
                        mPatientCount--;
                    }
                    else if (sParamedicCrewPed->mCar && !sParamedicCrewPed->mBf21C.bLeftVehicle)
                    {
                        sParamedicCrewPed->SetObjective(objectives_enum::leave_car_36);
                        bBusy = true;
                        sParamedicCrewPed->SetTargetObjectiveCar(mParamedicsCrew->mCar);
                        sParamedicCrewPed->SetPedType_403920(ped_type::special_ped_4);
                    }
                    else
                    {
                        sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                        sParamedicCrewPed->SetObjective(objectives_enum::goto_char_on_foot_16);
                        bBusy = true;
                        sParamedicCrewPed->set_objective_target_ped_403AC0(pPatient);
                    }
                    mLeaderPatient = pPatient;
                }
                else if (sParamedicCrewPed->GetInternalObjective_403A90() == objectives_enum::follow_char_9)
                {
                    pPatient = mPatients.RemoveFirstPed();
                    if (pPatient)
                    {
                        sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                        sParamedicCrewPed->SetObjective(objectives_enum::goto_char_on_foot_16);
                        sParamedicCrewPed->set_objective_target_ped_403AC0(pPatient);
                        mPatientCount--;
                        mParamedicsCrew->mGroup->mPanicking = 1;
                    }
                    mMemberPatient = pPatient;
                }
                break;

            case objectives_enum::dead_patient_28:
                if (sParamedicCrewPed->GetObjectiveStatus_450CB0())
                {
                    sParamedicCrewPed->mPedState1 = ped_state_1::walking_0;
                    sParamedicCrewPed->mPedState2 = ped_state_2::ped2_walking_0;
                    sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                    sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                    Ped* pLeader = mParamedicsCrew->mPed;
                    if (sParamedicCrewPed != pLeader && pLeader->isDead_403B60())
                    {
                        mParamedicsCrew->mGroup->PromoteMemberToLeader(0);
                        mParamedicsCrew->mPed = mParamedicsCrew->mGroup->mPedLeader;
                        mParamedicsCrew->mPed->SetObjective(objectives_enum::no_obj_0);
                        mParamedicsCrew->mPed->SetObjective2(objectives_enum::no_obj_0);
                    }
                    mParamedicsCrew->mState = crew_state::update_6;
                }
                break;

            case objectives_enum::leave_car_36:
                if (sParamedicCrewPed->GetObjectiveStatus_450CB0())
                {
                    sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                    sParamedicCrewPed->SetObjective(objectives_enum::goto_char_on_foot_16);
                    sParamedicCrewPed->set_objective_target_ped_403AC0(mLeaderPatient);
                }
                bBusy = true;
                break;

            case objectives_enum::goto_char_on_foot_16:
            {
                Ped* pVictim = sParamedicCrewPed->get_objective_target_ped_403AD0();
                if (sParamedicCrewPed->GetObjectiveStatus_450CB0())
                {
                    if (++mReviveTicks == 50)
                    {
                        if (pVictim->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
                        {
                            bParamedicVictim = pVictim->get_occupation_403980() == ped_ocupation_enum::paramedic_23;
                        }
                        pVictim->SetObjective2(objectives_enum::no_obj_0);
                        if (!bParamedicVictim)
                        {
                            pVictim->SetObjective2(objectives_enum::no_obj_0);
                            pVictim->mPedState1 = ped_state_1::walking_0;
                            pVictim->mPedState2 = ped_state_2::ped2_walking_0;
                            pVictim->SetObjective(objectives_enum::flee_on_foot_till_safe_1);
                            pVictim->mObjectiveTargetX = pVictim->get_cam_x();
                            pVictim->mObjectiveTargetY = pVictim->get_cam_y();
                            pVictim->mObjectiveTargetZ = pVictim->get_cam_z();
                            pVictim->SetPedType_403920(ped_type::dummy_3);
                            pVictim->set_occupation_403970(ped_ocupation_enum::dummy);
                            pVictim->mThreatReaction = threat_reaction_enum::run_away_3;
                        }
                        else
                        {
                            pVictim->set_objective_status_403B40(objective_status::passed_1);
                        }
                        pVictim->set_health_4039A0(100);
                        pVictim->ClearPanicking_403960();
                        if (pVictim == mLeaderPatient)
                        {
                            mLeaderPatient = NULL;
                        }
                        else
                        {
                            mMemberPatient = NULL;
                        }
                        sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                        mReviveTicks = 0;
                    }
                }
                bBusy = true;
                if (mParamedicsCrew->mGroup)
                {
                    mParamedicsCrew->mGroup->mWaitForMembers = false;
                }
                break;
            }

            case objectives_enum::enter_car_as_driver_35:
                if (!sParamedicCrewPed->IsGroupLeader_472FD0())
                {
                    sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                    sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                    break;
                }
                if (sParamedicCrewPed->mCar)
                {
                    sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                    sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                    sParamedicCrewPed->ChangeNextPedState1(ped_state_1::in_car_10);
                    sParamedicCrewPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
                }
                else
                {
                    pPatient = mPatients.RemoveFirstPed();
                    if (!pPatient)
                    {
                        if (mParamedicsCrew->mPhase == crew_phase::in_car_1)
                        {
                            if (mParamedicsCrew->mGroup)
                            {
                                mParamedicsCrew->mGroup->mWaitForMembers = true;
                            }
                        }
                        else
                        {
                            sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                            sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                        }
                    }
                    else if (pPatient->IsNearestSpriteACar())
                    {
                        gAmbulance_110->TryAddPatient(pTarget);
                        if (mParamedicsCrew->mPhase == crew_phase::in_car_1)
                        {
                            if (mParamedicsCrew->mGroup)
                            {
                                mParamedicsCrew->mGroup->mWaitForMembers = true;
                            }
                        }
                        else
                        {
                            sParamedicCrewPed->SetObjective(objectives_enum::no_obj_0);
                            sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                        }
                    }
                    else
                    {
                        sParamedicCrewPed->SetObjective2(objectives_enum::no_obj_0);
                        sParamedicCrewPed->SetObjective(objectives_enum::goto_char_on_foot_16);
                        sParamedicCrewPed->set_objective_target_ped_403AC0(pPatient);
                        mPatientCount--;
                        if (sParamedicCrewPed->IsGroupLeader_472FD0())
                        {
                            mLeaderPatient = pPatient;
                        }
                        else
                        {
                            mMemberPatient = pPatient;
                        }
                    }
                }
                bBusy = true;
                break;
        }

        if (mParamedicsCrew->mGroup)
        {
            sParamedicCrewPed = mParamedicsCrew->mGroup->mPedList[i];
            pTarget = mMemberPatient;
        }
        else
        {
            sParamedicCrewPed = NULL;
        }
        i++;
    }

    if (!bBusy)
    {
        mDone = true;
    }
}

// https://decomp.me/scratch/cxgie
MATCH_FUNC(0x4fb330)
void AmbulanceTask::UpdateState()
{
    bool bHandle = true;
    mPatients.RemoveNonCorpses();
    switch (mParamedicsCrew->mState)
    {
        case crew_state::spawn_car_3:
        {
            if (mParamedicsCrew->mReady)
            {
                if (!SpawnParamedicCrew())
                {
                    if (mParamedicsCrew->mCar)
                    {
                        mParamedicsCrew->mCar->MarkForDespawn_421470();
                    }
                    mParamedicsCrew->ReInit();
                    ClearTask();
                    bHandle = false;
                }
                else
                {
                    sParamedicCrewPed = mParamedicsCrew->mPed;
                    this->mDone = false;
                    if (mParamedicsCrew->mCar)
                    {
                        mParamedicsCrew->mCar->ActivateEmergencyLights();
                    }
                }
            }
            else
            {
                ++mParamedicsCrew->mSpawnWaitTicks;
                if (mParamedicsCrew->mSpawnWaitTicks > 500)
                {
                    mParamedicsCrew->mState = crew_state::clean_up_5;
                }
                bHandle = false;
            }
            break;
        }
        case crew_state::clean_up_5:
        {
            while (!mPatients.IsEmpty_420EA0())
            {
                Ped* pPed = mPatients.RemoveFirstPed();
                gAmbulance_110->TryAddPatient(pPed);
            }

            if (mParamedicsCrew->mReady)
            {
                mParamedicsCrew->mState = crew_state::idle_0;
                mParamedicsCrew->ReInit();
                ClearTask();
                bHandle = false;
            }
            break;
        }
        case crew_state::update_6:
        {
            EvaluatePickupState();
            break;
        }
        default:
            FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\medical.cpp", 1087);
            break;
    }
    if (bHandle)
    {
        HandleObjectiveState();
    }
}