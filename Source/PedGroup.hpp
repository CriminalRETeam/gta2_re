#pragma once

#include "Function.hpp"

class Ped;
class Fix16;

class PedGroup
{
  public:
    // 9.6f 0x4038E0
    inline void set_in_use_4038E0()
    {
        field_40_in_use = 1;
    }

    // 9.6f 0x4038F0
    inline char_type get_in_use_4038F0()
    {
        return field_40_in_use;
    }

    EXPORT void DestroyGroupArray_4C8E60();
    EXPORT static void sub_4C8E80();
    EXPORT void ClearGroupData_4C8E90();
    EXPORT void Reset_4C8EF0();
    EXPORT void ResetGroupObjectives_4C8F20();
    EXPORT void add_ped_to_end_of_list_4C8F90(Ped* pPed);
    EXPORT void replace_leader_4C8FE0(Ped* pPed);
    EXPORT bool PurgeMembersInCars_4C9040();
    EXPORT char_type AreAllMembersOffScreen_4C9150();
    EXPORT void ResetMembersToFollowLeader_4C91B0();
    EXPORT bool IsLeaderInCar_4C9210();
    EXPORT bool IsLeaderEnteringCarOrUnknown5_4C9220();
    EXPORT void KillEntireGroup_4C9240();
    EXPORT void DisbandGroup_4C92A0();
    EXPORT void DestroyGroup_4C93A0();
    EXPORT void DisbandGroupDueToAttack_4C94E0(Ped* pAttacker);
    EXPORT void PromoteMemberToLeader_4C9680(u8 idx);
    EXPORT void RemovePed_4C9970(Ped* pPed);
    EXPORT void add_ped_leader_4C9B10(Ped* ptr);
    EXPORT void add_ped_to_list_4C9B30(Ped* ptr, u8 idx);
    EXPORT char_type GroupAttackPed_4C9B60(Ped* pPed);
    EXPORT Ped* FindLastNonPanickingMember_4C9ED0();
    EXPORT void CoordinateGroupCarEntry_4C9F00();
    EXPORT u32 IsLeaderPanicking_4CA3E0();
    EXPORT Ped* FindFarthestMember_4CA3F0(Fix16* pFoundDistance);
    EXPORT void UpdateFormation_4CA4B0();
    EXPORT void UpdateMemberAIState_4CA5E0(u8 idx);
    EXPORT void UpdateMemberTightFollowState_4CA820(u8 idx);
    EXPORT bool IsAllMembersInSomeCar_4CAA20();
    EXPORT char_type HasNoActiveMembers_4CAAE0();
    EXPORT char_type AreAllMembersOnFoot_4CAB80();
    EXPORT bool IsMemberTooFarFromLeader_4CAC20(u8 idx);
    EXPORT bool IsLeaderCloseToTargetCar_4CAD40();
    EXPORT Ped* FindNearestOtherMember_4CAE80(u8 idx);
    EXPORT static void ResetAllGroups_4CB080();
    EXPORT static PedGroup* New_4CB0D0();
    EXPORT PedGroup();
    EXPORT ~PedGroup();

    // 9.6f 0x433360
    inline void SetCounts_433360(u8 count)
    {
        field_36_count = count;
        field_34_count = count;
    }

    inline s32 Get_F3C_433370()
    {
        return field_3C_is_leader_waiting;
    }

    char_type field_0_remove_dead_members;
    bool field_1_wait_for_members; // while set, UpdateFormation makes the leader wait (objective 9) for a member who has fallen too far behind
    char_type field_2_unused; // padding
    char_type field_3_unused; // padding
    Ped* field_4_ped_list[9];
    s32 field_28_unused;
    Ped* field_2C_ped_leader;
    s32 field_30_panicking; // UpdateFormation recomputes it as "the leader or a member is panicking"; GroupAttackPed_4C9B60, a leader in state 3/5/10 and the paramedics' revive step also set it. A dword in the original, so not a bool
    u8 field_34_count;
    char_type field_35_unused;
    char_type field_36_count;
    char_type field_37_unused;
    s32 field_38_group_type;
    s32 field_3C_is_leader_waiting;
    char_type field_40_in_use;
    char_type field_41_unused;
    char_type field_42_unused;
    char_type field_43_unused;
};