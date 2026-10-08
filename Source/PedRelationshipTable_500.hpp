#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Ped;

namespace PedRelationship
{
enum
{
    Code0 = 0, // None?

    // Low‑priority / passive relationships
    Code4 = 4,
    Code5 = 5,

    // Medium‑priority follow / pursuit
    Code6 = 6,
    Code8 = 8,
    Code10 = 10,

    // High‑priority engagement / aggressive pursuit
    Code7 = 7,
    Code9 = 9,
    Code11 = 11,

    // Critical / special / emergency relationships
    Code13 = 13,
    Code15 = 15,
};
} // namespace PedRelationship

// An entry of PedRelationshipTable_500: a ped (usually a driver) that is following / chasing another ped, the
// relationship code says how. Car_BC::field_60 points at the entry of the car's driver.
class PedRelationship_40
{
  public:
    EXPORT void ResetEntry_4747B0();
    EXPORT PedRelationship_40();
    EXPORT ~PedRelationship_40();

    char_type field_0_bInUse;
    Ped* field_4_ped_owner; // owner/thing going to the target
    s32 field_8_maybe_path_type;
    s32 field_C_relationship_code; // One of PedRelationship
    char_type field_10;
    Fix16 field_14_target_x;
    Fix16 field_18_target_y;
    Fix16 field_1C_target_z;
    char_type field_20;
    char_type field_21;
    char_type field_22;
    char_type field_23;
    char_type field_24;
    char_type field_25;
    char_type field_26;
    char_type field_27;
    char_type field_28;
    char_type field_29;
    s16 field_2A;
    u16 field_2C;
    s16 field_2E;
    Ped* field_30_ped_to_follow;
    s16 field_34;
    s32 field_38;
    char_type field_3C;
};

// Fixed table of 20 PedRelationship_40 entries
class PedRelationshipTable_500
{
  public:
    enum
    {
        k_max_entries = 20
    };

    EXPORT PedRelationship_40* AllocateEntry_474810();
    EXPORT char_type ArePedsCompatible_474850(Ped* pPed1, Ped* pPed2);
    EXPORT Ped* FindOwnerForFollowCode_4748A0(s32 relationship_code, Ped* pPed);
    EXPORT char_type CountFollowers_474920(Ped* pPedToFollow, Ped* pPed);
    EXPORT char_type HasAnyFollower_474970(Ped* pPed);
    EXPORT char_type HasRelationshipCode_13_15_4749B0(Ped* pPed);
    EXPORT char_type HasRelationshipCode_8_474A20(Ped* pPed);
    EXPORT char_type HasRelationshipCode_6_8_10_474A80(Ped* pPed);
    EXPORT char_type HasRelationshipCode_9_474AF0(Ped* pPed);
    EXPORT char_type HasRelationshipCode_7_9_11_474B50(Ped* pPed);
    EXPORT char_type HasRelationshipCode_6_7_8_9_13_474BC0(Ped* pPed);
    EXPORT char_type HasRelationshipCode_4_5_474C30(Ped* pPed);
    EXPORT void FreeEntry_474CC0(PedRelationship_40* pEntry);
    EXPORT PedRelationshipTable_500();
    EXPORT ~PedRelationshipTable_500();

    PedRelationship_40 field_0_entries[k_max_entries];
};

EXTERN_GLOBAL(PedRelationshipTable_500*, gPedRelationshipTable_678E30);
