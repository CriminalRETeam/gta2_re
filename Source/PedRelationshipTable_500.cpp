#include "PedRelationshipTable_500.hpp"
#include "Globals.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(PedRelationshipTable_500*, gPedRelationshipTable_678E30, 0x678E30);

DEFINE_GLOBAL_INIT(Fix16, kFpZero_678D0C, Fix16(0), 0x678D0C);

MATCH_FUNC(0x4747b0)
void PedRelationship_40::ResetEntry_4747B0()
{
    field_0_bInUse = 0;
    field_10 = 0;
    field_8_maybe_path_type = 0;
    field_14_target_x = kFpZero_678D0C;
    field_18_target_y = kFpZero_678D0C;
    field_1C_target_z = kFpZero_678D0C;
    field_20 = 0;
    field_21 = 0;
    field_22 = 1;
    field_23 = 1;
    field_24 = 0;
    field_25 = 0;
    field_30_ped_to_follow = 0;
    field_26 = 0;
    field_2A = 0;
    field_2C = 0;
    field_2E = 0;
    field_C_relationship_code = PedRelationship::Code0;
    field_34 = 0;
    field_4_ped_owner = 0;
    field_38 = 0;
    field_3C = 0;
}

MATCH_FUNC(0x474810)
PedRelationship_40* PedRelationshipTable_500::AllocateEntry_474810()
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (!field_0_entries[i].field_0_bInUse)
        {
            field_0_entries[i].field_0_bInUse = 1;
            return &field_0_entries[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x474850)
char_type PedRelationshipTable_500::ArePedsCompatible_474850(Ped* pPed1, Ped* pPed2)
{
    if (pPed1->get_occupation_403980() < 24 || pPed1->get_occupation_403980() > 27) // ped 1 is not police
    {
        if (pPed2->field_17C_pGang == pPed1->field_17C_pGang) // they are from same gang (or both dont have any)
        {
            return 1;
        }
    }
    else
    {
        if (pPed2->get_occupation_403980() >= 24 && pPed2->field_240_occupation <= 27) // both ped 1 and ped 2 are police feds
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4748a0)
Ped* PedRelationshipTable_500::FindOwnerForFollowCode_4748A0(s32 relationship_code, Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1 && field_0_entries[i].field_30_ped_to_follow == pPed && ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner) &&
            relationship_code == field_0_entries[i].field_C_relationship_code)
        {
            return field_0_entries[i].field_4_ped_owner;
        }
    }
    return 0;
}

MATCH_FUNC(0x474920)
char_type PedRelationshipTable_500::CountFollowers_474920(Ped* pPedToFollow, Ped* pPed)
{
    u8 total = 0;
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1 && field_0_entries[i].field_30_ped_to_follow == pPedToFollow)
        {
            if (ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner))
            {
                total++;
            }
        }
    }
    return total;
}

MATCH_FUNC(0x474970)
char_type PedRelationshipTable_500::HasAnyFollower_474970(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1 && field_0_entries[i].field_30_ped_to_follow == pPed)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4749b0)
char_type PedRelationshipTable_500::HasRelationshipCode_13_15_4749B0(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner))
            {
                switch (field_0_entries[i].field_C_relationship_code)
                {
                    case PedRelationship::Code13:
                    case PedRelationship::Code15:
                        return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474a20)
char_type PedRelationshipTable_500::HasRelationshipCode_8_474A20(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner) && field_0_entries[i].field_C_relationship_code == 8)
            {
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474a80)
char_type PedRelationshipTable_500::HasRelationshipCode_6_8_10_474A80(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner))
            {
                switch (field_0_entries[i].field_C_relationship_code)
                {
                    case PedRelationship::Code6:
                    case PedRelationship::Code8:
                    case PedRelationship::Code10:
                        return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474af0)
char_type PedRelationshipTable_500::HasRelationshipCode_9_474AF0(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner) && field_0_entries[i].field_C_relationship_code == 9)
            {
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474b50)
char_type PedRelationshipTable_500::HasRelationshipCode_7_9_11_474B50(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner))
            {
                switch (field_0_entries[i].field_C_relationship_code)
                {
                    case PedRelationship::Code7:
                        return 1;
                    case PedRelationship::Code9:
                    case PedRelationship::Code11:
                        return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474bc0)
char_type PedRelationshipTable_500::HasRelationshipCode_6_7_8_9_13_474BC0(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner))
            {
                if (field_0_entries[i].field_C_relationship_code >= PedRelationship::Code6 && (field_0_entries[i].field_C_relationship_code <= PedRelationship::Code9 || field_0_entries[i].field_C_relationship_code == PedRelationship::Code13))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474c30)
char_type PedRelationshipTable_500::HasRelationshipCode_4_5_474C30(Ped* pPed)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (field_0_entries[i].field_0_bInUse == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0_entries[i].field_4_ped_owner))
            {
                if (field_0_entries[i].field_C_relationship_code < PedRelationship::Code4)
                {
                    continue;
                }

                if (field_0_entries[i].field_C_relationship_code <= PedRelationship::Code5)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474ca0)
PedRelationship_40::PedRelationship_40()
{
    ResetEntry_4747B0();
}

MATCH_FUNC(0x474cb0)
PedRelationship_40::~PedRelationship_40()
{
}

MATCH_FUNC(0x474cc0)
void PedRelationshipTable_500::FreeEntry_474CC0(PedRelationship_40* toFind)
{
    for (u8 i = 0; i < k_max_entries; i++)
    {
        if (&field_0_entries[i] == toFind)
        {
            field_0_entries[i].field_0_bInUse = 0;
            field_0_entries[i].ResetEntry_4747B0();
            return;
        }
    }
}

MATCH_FUNC(0x474d10)
PedRelationshipTable_500::PedRelationshipTable_500()
{
}

MATCH_FUNC(0x474d30)
PedRelationshipTable_500::~PedRelationshipTable_500()
{
}