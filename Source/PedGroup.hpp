#pragma once

#include "Function.hpp"

class Ped;
class Fix16;

class PedGroup
{
  private:
    // 9.6f 0x4038E0
    inline void set_in_use_4038E0()
    {
        mInUse = true;
    }

    // 9.6f 0x4038F0
    inline bool get_in_use_4038F0() const
    {
        return mInUse;
    }

  public:
    STATIC_MEMBER(char_type, sGroupMemberCarDoor);
    EXPORT void DestroyGroupArray();

  private:
    EXPORT static void ResetMemberCarDoor();

  public:
    EXPORT void ClearGroupData();

  private:
    EXPORT void Reset();

  public:
    EXPORT void ResetGroupObjectives();
    EXPORT void add_ped_to_end_of_list(Ped* pPed);
    EXPORT void replace_leader(Ped* pNewLeader);
    EXPORT bool PurgeMembersInCars();
    EXPORT bool AreAllMembersOffScreen() const;
    EXPORT void ResetMembersToFollowLeader();
    EXPORT bool IsLeaderInCar() const;
    EXPORT bool IsLeaderEnteringCarOrTrain() const;
    EXPORT void KillEntireGroup();
    EXPORT void DisbandGroup();
    EXPORT void DestroyGroup();
    EXPORT void DisbandGroupDueToAttack(Ped* pAttacker);
    EXPORT void PromoteMemberToLeader(u8 idx);
    EXPORT void RemovePed(Ped* pPed);
    EXPORT void add_ped_leader(Ped* pLeader);
    EXPORT void add_ped_to_list(Ped* pPed, u8 idx);
    EXPORT void GroupAttackPed(Ped* pPed);

  private:
    EXPORT Ped* FindLastNonPanickingMember() const;

  public:
    EXPORT void CoordinateGroupCarEntry();

  private:
    EXPORT u32 IsLeaderPanicking() const;
    EXPORT Ped* FindFarthestMember(Fix16* pFoundDistance) const;
    EXPORT void UpdateFormation();

  public:
    EXPORT void UpdateMemberAIState(u8 idx);
    EXPORT void UpdateMemberTightFollowState(u8 idx);
    EXPORT bool IsAllMembersInSomeCar() const;
    EXPORT bool HasNoActiveMembers() const;
    EXPORT bool AreAllMembersOnFoot() const;
    EXPORT bool IsMemberTooFarFromLeader(u8 idx) const;
    EXPORT bool IsLeaderCloseToTargetCar() const;

  private:
    EXPORT Ped* FindNearestOtherMember(u8 idx) const;

  public:
    EXPORT static void ResetAllGroups();
    EXPORT static PedGroup* New();
    EXPORT PedGroup();
    EXPORT ~PedGroup();

    // 9.6f 0x433360
    inline void SetCounts_433360(u8 count)
    {
        mCount36 = count;
        mCount34 = count;
    }

    inline s32 Get_F3C_433370() const
    {
        return mIsLeaderWaiting;
    }

    bool mRemoveDeadMembers; // 0x0
    bool mWaitForMembers; // 0x1: while set, UpdateFormation makes the leader wait (objective 9) for a member who has fallen too far behind
    char_type mUnused2; // 0x2: padding
    char_type mUnused3; // 0x3: padding
    Ped* mPedList[9]; // 0x4: the members, without gaps up to mCount34: the loops over them don't check for NULL
    s32 mUnused28; // 0x28
    Ped* mPedLeader; // 0x2C
    s32 mPanicking; // 0x30: UpdateFormation recomputes it as "the leader or a member is panicking"; GroupAttackPed, a leader in state 3/5/10 and the paramedics' revive step also set it. A dword in the original, so not a bool
    u8 mCount34; // 0x34
    char_type mUnused35; // 0x35
    char_type mCount36; // 0x36
    char_type mUnused37; // 0x37
    s32 mGroupType; // 0x38
    s32 mIsLeaderWaiting; // 0x3C
    bool mInUse; // 0x40
    char_type mUnused41; // 0x41
    char_type mUnused42; // 0x42
    char_type mUnused43; // 0x43
};