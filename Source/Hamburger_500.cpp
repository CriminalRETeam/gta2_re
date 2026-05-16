#include "Hamburger_500.hpp"
#include "Globals.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(Hamburger_500*, gHamburger_500_678E30, 0x678E30);

DEFINE_GLOBAL_INIT(Fix16, dword_678D0C, Fix16(0), 0x678D0C);

MATCH_FUNC(0x4747b0)
void Hamburger_40::ResetEntry_4747B0()
{
    field_0 = 0;
    field_10 = 0;
    field_8_maybe_path_type = 0;
    field_14_target_x = dword_678D0C;
    field_18_target_y = dword_678D0C;
    field_1C_target_z = dword_678D0C;
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
    field_C = PedRelationship::Code0;
    field_34 = 0;
    field_4_ped_owner = 0;
    field_38 = 0;
    field_3C = 0;
}

MATCH_FUNC(0x474ca0)
Hamburger_40::Hamburger_40()
{
    ResetEntry_4747B0();
}

MATCH_FUNC(0x474cb0)
Hamburger_40::~Hamburger_40()
{
}

MATCH_FUNC(0x474810)
Hamburger_40* Hamburger_500::AllocateEntry_474810()
{
    for (u8 i = 0; i < 20; i++)
    {
        if (!field_0[i].field_0)
        {
            field_0[i].field_0 = 1;
            return &field_0[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x474850)
char_type Hamburger_500::ArePedsCompatible_474850(Ped* pPed1, Ped* pPed2)
{
    if (pPed1->field_240_occupation < 24 || pPed1->field_240_occupation > 27) // ped 1 is not police
    {
        if (pPed2->field_17C_pGang == pPed1->field_17C_pGang) // they are from same gang (or both dont have any)
        {
            return 1;
        }
    }
    else
    {
        if (pPed2->field_240_occupation >= 24 && pPed2->field_240_occupation <= 27) // both ped 1 and ped 2 are police feds
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4748a0)
Ped* Hamburger_500::FindOwnerForFollowCode_4748A0(s32 a2, Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1 && field_0[i].field_30_ped_to_follow == pPed && ArePedsCompatible_474850(pPed, field_0[i].field_4_ped_owner) &&
            a2 == field_0[i].field_C)
        {
            return field_0[i].field_4_ped_owner;
        }
    }
    return 0;
}

MATCH_FUNC(0x474920)
char_type Hamburger_500::CountFollowers_474920(Ped* a2, Ped* a3)
{
    u8 total = 0;
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1 && field_0[i].field_30_ped_to_follow == a2)
        {
            if (ArePedsCompatible_474850(a3, field_0[i].field_4_ped_owner))
            {
                total++;
            }
        }
    }
    return total;
}

MATCH_FUNC(0x474970)
char_type Hamburger_500::HasAnyFollower_474970(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1 && field_0[i].field_30_ped_to_follow == pPed)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4749b0)
char_type Hamburger_500::HasRelationshipCode_13_15_4749B0(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0[i].field_4_ped_owner))
            {
                switch (field_0[i].field_C)
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
char_type Hamburger_500::HasRelationshipCode_8_474A20(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0[i].field_4_ped_owner) && field_0[i].field_C == 8)
            {
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474a80)
char_type Hamburger_500::HasRelationshipCode_6_8_10_474A80(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0[i].field_4_ped_owner))
            {
                switch (field_0[i].field_C)
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
char_type Hamburger_500::HasRelationshipCode_9_474AF0(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0[i].field_4_ped_owner) && field_0[i].field_C == 9)
            {
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474b50)
char_type Hamburger_500::HasRelationshipCode_7_9_11_474B50(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0[i].field_4_ped_owner))
            {
                switch (field_0[i].field_C)
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
char_type Hamburger_500::HasRelationshipCode_6_7_8_9_13_474BC0(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0[i].field_4_ped_owner))
            {
                if (field_0[i].field_C >= PedRelationship::Code6 && (field_0[i].field_C <= PedRelationship::Code9 || field_0[i].field_C == PedRelationship::Code13))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474c30)
char_type Hamburger_500::HasRelationshipCode_4_5_474C30(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (ArePedsCompatible_474850(pPed, field_0[i].field_4_ped_owner))
            {
                if (field_0[i].field_C < PedRelationship::Code4)
                {
                    continue;
                }

                if (field_0[i].field_C <= PedRelationship::Code5)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x474cc0)
void Hamburger_500::FreeEntry_474CC0(Hamburger_40* toFind)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (&field_0[i] == toFind)
        {
            field_0[i].field_0 = 0;
            field_0[i].ResetEntry_4747B0();
            return;
        }
    }
}

MATCH_FUNC(0x474d10)
Hamburger_500::Hamburger_500()
{
}

MATCH_FUNC(0x474d30)
Hamburger_500::~Hamburger_500()
{
}