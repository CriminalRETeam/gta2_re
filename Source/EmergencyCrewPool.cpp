#include "EmergencyCrewPool.hpp"
#include "Car.hpp"
#include "CarChaseTaskTable.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"

DEFINE_GLOBAL(EmergencyCrewPool*, gEmergencyCrewPool_706280, 0x706280);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_706148, Fix16(4), 0x706148);

MATCH_FUNC(0x4beb00)
EmergencyCrew::EmergencyCrew()
{
    Init_5CBC00();
}

MATCH_FUNC(0x4beb10)
EmergencyCrew::~EmergencyCrew()
{
}

MATCH_FUNC(0x4beb20)
EmergencyCrewPool::~EmergencyCrewPool()
{
}

MATCH_FUNC(0x5cbb70)
void EmergencyCrewPool::init_5CBB70()
{
}

MATCH_FUNC(0x5cbb80)
EmergencyCrew* EmergencyCrewPool::AllocateSlot_5CBB80()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_0_slots); i++)
    {
        if (!field_0_slots[i].field_1E_is_used)
        {
            return &field_0_slots[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x5cbbd0)
void EmergencyCrewPool::ServiceAll_5CBBD0()
{
    for (s32 i = 0; i < 10; i++)
    {
        if (field_0_slots[i].field_1E_is_used)
        {
            if (field_0_slots[i].Service_5CC480())
            {
                field_0_slots[i].field_1E_is_used = false;
            }
        }
    }
}

MATCH_FUNC(0x5cbc00)
void EmergencyCrew::Init_5CBC00()
{
    field_1A_idle_limit = 150;
    field_1E_is_used = false;
    field_20_crew_type = 0;
    field_24_phase = crew_phase::on_foot_0;
    field_0_car = NULL;
    field_4_ped = NULL;
    field_28_state = crew_state::idle_0;
    field_8_group = NULL;
    field_2C_ready = false;
    field_1C_spawn_wait_ticks = 0;
}

MATCH_FUNC(0x5cbc30)
void EmergencyCrew::ReInit_5CBC30()
{
    Init_5CBC00();
}

MATCH_FUNC(0x5cbc40)
void EmergencyCrew::RemovePed_5CBC40(Ped* pPed)
{
    field_8_group->RemovePed_4C9970(pPed);
    field_4_ped = field_8_group->field_2C_ped_leader;
}

// https://decomp.me/scratch/HmQPr
MATCH_FUNC(0x5cbc60)
bool EmergencyCrew::IsLeaderAlive_5CBC60() const
{
    if (field_4_ped && field_4_ped->isDead_403B60())
    {
        return false;
    }
    // A switch whose cases all return true: VC6 drops the compares but keeps the
    // load of field_28_state (the stray mov 0x28(%ecx),%ecx). The case values are a guess.
    switch (field_28_state)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return true;
    }
    return true;
}

MATCH_FUNC(0x5cbc90)
bool EmergencyCrew::ReplaceLeaderIfNeeded_5CBC90()
{
    PedGroup* pGroup = this->field_8_group;
    if (!pGroup)
    {
        return false;
    }

    u8 idx = 0;
    Ped* pPedAtIdx = pGroup->field_4_ped_list[idx];
    while (pPedAtIdx)
    {
        if (pPedAtIdx->GetPedState_403990() != ped_state_1::dead_9 && !pPedAtIdx->field_16C_car)
        {
            Ped* pLeaderPed = this->field_4_ped;
            if (pLeaderPed->field_16C_car)
            {
                RemovePed_5CBC40(pLeaderPed);
            }
            else
            {
                pGroup->PromoteMemberToLeader_4C9680(idx);
                Ped* pLeader = this->field_8_group->field_2C_ped_leader;
                this->field_4_ped = pLeader;
                pLeader->SetObjective(objectives_enum::no_obj_0);
                const s32 occupation = pPedAtIdx->get_occupation_403980();
                pPedAtIdx->set_occupation_403970(ped_ocupation_enum::dummy);
                pPedAtIdx->Kill_46F9D0();
                pPedAtIdx->set_occupation_403970(occupation);
                pPedAtIdx->SetObjective(objectives_enum::dead_patient_28);
            }
            return true;
        }
        idx++;
        pPedAtIdx = pGroup->field_4_ped_list[idx];
    }
    return false;
}

// https://decomp.me/scratch/sUPg8
MATCH_FUNC(0x5cbd50)
void EmergencyCrew::UpdateStateMachine_5CBD50()
{
    bool bCarGone = false;
    bool bNoneOnFoot = true;
    bool bAllOnFoot = true;
    bool bDriverIsOther = false;
    u8 idx;
    if (field_8_group)
    {
        if (!field_8_group->field_34_count)
        {
            field_8_group->ClearGroupData_4C8E90();
            field_8_group = NULL;
        }
    }

    if (field_24_phase == crew_phase::in_car_1)
    {
        if (field_0_car)
        {
            if (field_0_car->IsDespawning_4215B0())
            {
                bCarGone = true;
            }
            if (field_0_car->IsMaxDamage_40F890())
            {
                bCarGone = true;
            }
            if (field_0_car->HasSpriteZoom_43A230())
            {
                bCarGone = true;
            }

            if (field_0_car->field_54_driver && field_0_car->field_54_driver != field_4_ped)
            {
                bDriverIsOther = true;
                bCarGone = true;
            }
        }
        else
        {
            bCarGone = true;
        }

        if (bCarGone)
        {
            if (field_8_group)
            {
                idx = 0;
                for (Ped* pMember = field_8_group->field_4_ped_list[0]; pMember; pMember = field_8_group->field_4_ped_list[++idx])
                {
                    if (pMember->field_168_game_object)
                    {
                        bNoneOnFoot = false;
                    }
                    else
                    {
                        bAllOnFoot = false;
                    }
                }
            }

            if (field_4_ped)
            {
                if (field_4_ped->field_168_game_object)
                {
                    bNoneOnFoot = false;
                }
            }
            if (bDriverIsOther)
            {
                if (!bAllOnFoot)
                {
                    return;
                }
                if (field_4_ped)
                {
                    if (!field_4_ped->field_168_game_object)
                    {
                        return;
                    }

                    if (Fix16::MaxAbsDistance_42A6B0(field_4_ped->get_cam_x(),
                                                     field_4_ped->get_cam_y(),
                                                     field_0_car->field_50_car_sprite->field_14_xy.x,
                                                     field_0_car->field_50_car_sprite->field_14_xy.y) > kFpFour_706148)
                    {
                        bCarGone = true;
                    }
                }
                else
                {
                    bCarGone = true;
                }
            }
            if (!bCarGone)
            {
                return;
            }
            if (field_0_car)
            {
                field_0_car->field_76_last_seen_timer = 0;
                if (field_0_car->field_7C_ped_type != ped_type::player_2)
                {
                    field_0_car->field_7C_ped_type = ped_type::dummy_3;
                }
                if (field_0_car->field_60_pChaseTask)
                {
                    gCarChaseTaskTable_678E30->FreeEntry_474CC0(field_0_car->field_60_pChaseTask);
                    field_0_car->field_60_pChaseTask = NULL;
                }
                field_0_car = NULL;
            }
            if (bNoneOnFoot)
            {
                if (field_4_ped)
                {
                    field_4_ped->set_occupation_403970(ped_ocupation_enum::dummy);
                    field_4_ped->ClearGroupAndGroupIdx_403A30();
                    field_4_ped->Deallocate_45EB60();
                }
                idx = 0;
                if (field_8_group)
                {
                    for (Ped* pMember = field_8_group->field_4_ped_list[0]; pMember; pMember = field_8_group->field_4_ped_list[idx])
                    {
                        pMember->set_occupation_403970(ped_ocupation_enum::dummy);
                        pMember->ClearGroupAndGroupIdx_403A30();
                        pMember->Deallocate_45EB60();
                        ++idx;
                    }
                    field_8_group->ClearGroupData_4C8E90();
                }
                field_8_group = NULL;
                field_4_ped = NULL;
                field_24_phase = crew_phase::finished_2;
                return;
            }
            if (bAllOnFoot)
            {
                if (field_4_ped)
                {
                    if (field_4_ped->field_168_game_object)
                    {
                        field_24_phase = crew_phase::on_foot_0;
                        if (field_8_group)
                        {
                            field_8_group->ResetGroupObjectives_4C8F20();
                        }
                        return;
                    }
                }
            }

            if (field_8_group)
            {
                if (field_8_group->PurgeMembersInCars_4C9040())
                {
                    if (field_8_group->field_2C_ped_leader)
                    {
                        field_24_phase = crew_phase::on_foot_0;
                        field_8_group->ResetGroupObjectives_4C8F20();
                    }
                    else if (field_8_group->field_34_count == 1)
                    {
                        field_4_ped = field_8_group->field_4_ped_list[0];
                        field_8_group->ClearGroupData_4C8E90();
                        field_8_group = NULL;
                        field_4_ped->SetObjective(objectives_enum::no_obj_0);
                        field_4_ped->SetObjective2_463830(objectives_enum::no_obj_0);
                        field_24_phase = crew_phase::on_foot_0;
                    }
                    else
                    {
                        field_8_group->field_2C_ped_leader = field_8_group->field_4_ped_list[field_8_group->field_34_count];
                        field_8_group->field_4_ped_list[field_8_group->field_34_count] = NULL;
                        --field_8_group->field_36_count;
                        --field_8_group->field_34_count;
                        field_4_ped = field_8_group->field_2C_ped_leader;
                        field_8_group->ResetGroupObjectives_4C8F20();
                        field_24_phase = crew_phase::on_foot_0;
                    }
                }
                else
                {
                    field_4_ped = field_8_group->field_2C_ped_leader;
                    field_8_group->DestroyGroup_4C93A0();
                    field_8_group = NULL;
                    field_4_ped->SetObjective(objectives_enum::no_obj_0);
                    field_4_ped->SetObjective2_463830(objectives_enum::no_obj_0);
                    field_24_phase = crew_phase::on_foot_0;
                }
            }
            else if (field_4_ped)
            {
                if (field_4_ped->field_168_game_object)
                {
                    field_4_ped->SetObjective(objectives_enum::no_obj_0);
                    field_4_ped->SetObjective2_463830(objectives_enum::no_obj_0);
                    field_24_phase = crew_phase::on_foot_0;
                }
            }
            else if (!field_0_car)
            {
                field_24_phase = crew_phase::finished_2;
            }
        }
        else
        {
            bCarGone = true;

            if (field_8_group)
            {
                idx = 0;
                for (Ped* pMember = field_8_group->field_4_ped_list[0]; pMember; pMember = field_8_group->field_4_ped_list[++idx])
                {
                    if (!pMember->isDead_403B60())
                    {
                        bCarGone = false;
                    }
                }
            }

            if (bCarGone)
            {
                if (!field_4_ped || field_4_ped->isDead_403B60())
                {
                    field_24_phase = crew_phase::finished_2;
                }
            }
            if (field_24_phase == crew_phase::finished_2)
            {
                if (field_0_car)
                {
                    field_0_car->SetDriverPedType_421560(ped_type::dummy_3);
                    field_0_car->field_76_last_seen_timer = -200;
                }
            }
        }
    }
    else if (!field_24_phase)
    {
        bCarGone = true;
        if (field_8_group)
        {
            idx = 0;
            for (Ped* pMember = field_8_group->field_4_ped_list[0]; pMember; pMember = field_8_group->field_4_ped_list[++idx])
            {
                if (!pMember->isDead_403B60())
                {
                    bCarGone = false;
                }
            }
        }

        if (bCarGone)
        {
            if (!field_4_ped || field_4_ped->isDead_403B60())
            {
                field_24_phase = crew_phase::finished_2;
            }
        }
    }
    else
    {
        if (field_0_car)
        {
            if (field_0_car->field_60_pChaseTask)
            {
                gCarChaseTaskTable_678E30->FreeEntry_474CC0(field_0_car->field_60_pChaseTask);
                field_0_car->field_60_pChaseTask = NULL;
            }
        }
        field_0_car = NULL;
    }
}

// 9.6f 0x4C5A00
MATCH_FUNC(0x5cc1c0)
void EmergencyCrew::CleanupExpiredEntities_5CC1C0()
{
    bool bReleaseCar = false;
    bool bClearPedAndGroup = true;
    bool bAllOnFoot = true;

    if (field_8_group)
    {
        if (field_8_group->field_34_count == 0)
        {
            field_8_group->ClearGroupData_4C8E90();
            this->field_8_group = NULL;
        }
    }

    if (this->field_24_phase == crew_phase::in_car_1)
    {
        if (this->field_0_car && !field_0_car->IsDespawning_4215B0() && !field_0_car->IsMaxDamage_40F890())
        {
            if (field_0_car->GetLastSeenTimer_4A9AD0() > this->field_1A_idle_limit)
            {
                field_0_car->MarkForDespawn_421470();
                bReleaseCar = true;
            }
        }
        else
        {
            bReleaseCar = true;
        }

        if (field_4_ped)
        {
            if (field_4_ped->field_168_game_object)
            {
                if (field_4_ped->GetOffscreenCounter_4039F0() < this->field_1A_idle_limit)
                {
                    bClearPedAndGroup = false;
                }
            }
            else if (field_4_ped->isDead_403B60())
            {
                this->field_4_ped = NULL;
            }
        }

        if (field_4_ped)
        {
            if (field_4_ped->field_168_game_object)
            {
                bClearPedAndGroup = false;
            }
        }

        if (field_8_group)
        {
            u8 i = 0;
            for (Ped* pPedListIter = field_8_group->field_4_ped_list[0]; pPedListIter;)
            {
                if (pPedListIter->field_168_game_object)
                {
                    if (pPedListIter->GetOffscreenCounter_4039F0() < this->field_1A_idle_limit)
                    {
                        bClearPedAndGroup = false;
                    }
                }
                else
                {
                    bAllOnFoot = false;
                }
                i++;
                pPedListIter = field_8_group->field_4_ped_list[i];
            }
        }

        if (bReleaseCar)
        {
            if (field_0_car)
            {
                if (field_0_car->field_60_pChaseTask)
                {
                    gCarChaseTaskTable_678E30->FreeEntry_474CC0(field_0_car->field_60_pChaseTask);
                    field_0_car->field_60_pChaseTask = NULL;
                }
            }
            field_0_car = NULL;

            if (bClearPedAndGroup)
            {
                if (field_4_ped)
                {
                    field_4_ped->set_occupation_403970(ped_ocupation_enum::dummy);
                    field_4_ped->ClearGroupAndGroupIdx_403A30();
                    field_4_ped->Deallocate_45EB60();
                }

                u8 i = 0;
                if (field_8_group)
                {
                    for (Ped* pPedListIter = field_8_group->field_4_ped_list[0]; pPedListIter;)
                    {
                        pPedListIter->set_occupation_403970(ped_ocupation_enum::dummy);
                        pPedListIter->ClearGroupAndGroupIdx_403A30();
                        pPedListIter->Deallocate_45EB60();
                        i++;
                        pPedListIter = field_8_group->field_4_ped_list[i];
                    }
                    field_8_group->ClearGroupData_4C8E90();
                }
                this->field_8_group = NULL;
                this->field_2C_ready = true;
            }
            else if (bAllOnFoot)
            {
                if (this->field_4_ped->field_168_game_object)
                {
                    this->field_24_phase = crew_phase::on_foot_0;
                }
            }
        }
        return;
    }

    if (field_4_ped)
    {
        if (field_4_ped->GetOffscreenCounter_4039F0() < this->field_1A_idle_limit)
        {
            bClearPedAndGroup = false;
        }
    }

    if (field_8_group)
    {
        u8 i = 0;
        for (Ped* pPedListIter = field_8_group->field_4_ped_list[0]; pPedListIter;)
        {
            if (pPedListIter->GetOffscreenCounter_4039F0() < this->field_1A_idle_limit)
            {
                bClearPedAndGroup = false;
            }
            i++;
            pPedListIter = field_8_group->field_4_ped_list[i];
        }
    }

    if (bClearPedAndGroup)
    {
        if (field_4_ped)
        {
            field_4_ped->ClearGroupAndGroupIdx_403A30();
            field_4_ped->Deallocate_45EB60();
        }

        u8 i = 0;
        if (field_8_group)
        {
            Ped* pPedListIter = field_8_group->field_4_ped_list[0];
            while(pPedListIter)
            {
                pPedListIter->ClearGroupAndGroupIdx_403A30();
                pPedListIter->Deallocate_45EB60();
                i++;
                pPedListIter = field_8_group->field_4_ped_list[i];
            }
            field_8_group->ClearGroupData_4C8E90();
        }
        field_2C_ready = true;
    }
}

MATCH_FUNC(0x5cc480)
bool EmergencyCrew::Service_5CC480()
{
    if (field_18_spawn_delay > 0)
    {
        this->field_18_spawn_delay--;
        return false;
    }

    if (field_18_spawn_delay > -80)
    {
        this->field_18_spawn_delay--;
    }

    switch (this->field_28_state)
    {
        case crew_state::clean_up_5:
            CleanupExpiredEntities_5CC1C0();
            return false;

        case crew_state::update_6:
            UpdateStateMachine_5CBD50();
            return false;

        case crew_state::spawn_car_3:
            break;

        default:
            return false;
    }

    switch (field_20_crew_type)
    {
        case crew_type::paramedic_1:
            if (gCarManager_6C_677930->CanAllocateCarOfKind_446930(car_kind::paramedic_car_4))
            {
                this->field_0_car = gCarManager_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::MEDICAR,
                                                                                   this->field_C_spawn_x,
                                                                                   this->field_10_spawn_y,
                                                                                   this->field_14_spawn_z);
            }
            else
            {
                this->field_0_car = NULL;
            }

            if (this->field_0_car)
            {
                this->field_2C_ready = true;
                field_0_car->IncrementCarStats_443D70(car_kind::paramedic_car_4);
            }
            break;

        case crew_type::police_3:
            if (gCarManager_6C_677930->CanAllocateCarOfKind_446930(car_kind::police_6))
            {
                this->field_0_car = gCarManager_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::COPCAR,
                                                                                   this->field_C_spawn_x,
                                                                                   this->field_10_spawn_y,
                                                                                   this->field_14_spawn_z);
            }
            else
            {
                this->field_0_car = NULL;
            }
            if (this->field_0_car)
            {
                this->field_2C_ready = true;
                field_0_car->IncrementCarStats_443D70(car_kind::police_6);
            }
            break;

        case crew_type::swat_5:
            if (gCarManager_6C_677930->CanAllocateCarOfKind_446930(car_kind::police_6))
            {
                this->field_0_car = gCarManager_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::SWATVAN,
                                                                                   this->field_C_spawn_x,
                                                                                   this->field_10_spawn_y,
                                                                                   this->field_14_spawn_z);
            }
            else
            {
                this->field_0_car = NULL;
            }

            if (this->field_0_car)
            {
                this->field_2C_ready = true;
                field_0_car->IncrementCarStats_443D70(car_kind::police_6);
            }
            break;

        default:
            field_0_car = NULL;
            break;
    }

    return false;
}