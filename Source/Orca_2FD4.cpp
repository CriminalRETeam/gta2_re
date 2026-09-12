#include "Orca_2FD4.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "map_0x370.hpp"
#include <string.h>

DEFINE_GLOBAL(u8, byte_6FDEEC, 0x6FDEEC);
DEFINE_GLOBAL(s16, word_6FDECE, 0x6FDECE);

DEFINE_GLOBAL(Orca_2FD4*, gOrca_2FD4_6FDEF0, 0x6FDEF0);

DEFINE_GLOBAL_INIT(Fix16, dword_6FDD98, Fix16(0), 0x6FDD98);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDD50, Fix16(0x100, 0), 0x6FDD50);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDC00, Fix16(0.25), 0x6FDC00);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDCA8, Fix16(0.5), 0x6FDCA8);

DEFINE_GLOBAL(u8, gOrca_Idx1_6FDBF8, 0x6FDBF8);
DEFINE_GLOBAL(u8, gOrca_idx2_6FDBF9, 0x6FDBF9);

DEFINE_GLOBAL(s32, gOrca_AngleFace_6FDD38, 0x6FDD38);
DEFINE_GLOBAL(u8, gOrca_XPos_1_6FDBE2, 0x6FDBE2);
DEFINE_GLOBAL(u8, gOrca_YPos_1_6FDBE3, 0x6FDBE3);
DEFINE_GLOBAL(u8, gOrca_ZPos1_6FDBE4, 0x6FDBE4);
DEFINE_GLOBAL(u8, gOrca_idx1_1_6FDBE0, 0x6FDBE0);
DEFINE_GLOBAL(u8, gOrca_idx2_1_6FDBE1, 0x6FDBE1);

DEFINE_GLOBAL(u8, gOrca_XPosAny_6FDECA, 0x6FDECA);
DEFINE_GLOBAL(u8, gOrca_YPosAny_6FDECB, 0x6FDECB);
DEFINE_GLOBAL(u8, gOrca_ZPosAny_6FDECC, 0x6FDECC);
DEFINE_GLOBAL(u8, gOrca_idx1_any_6FDEC8, 0x6FDEC8);
DEFINE_GLOBAL(u8, gOrca_idx2_any_6FDEC9, 0x6FDEC9);

DEFINE_GLOBAL(u8, gOrca_XPos2_6FDBFA, 0x6FDBFA);
DEFINE_GLOBAL(u8, gOrca_YPos2_6FDBFB, 0x6FDBFB);
DEFINE_GLOBAL(u8, gOrca_ZPos2_6FDBFC, 0x6FDBFC);
DEFINE_GLOBAL(u8, gOrca_idx1_2_6FDB68, 0x6FDB68);
DEFINE_GLOBAL(u8, gOrca_idx_2_2_6FDB69, 0x6FDB69);

DEFINE_GLOBAL(u8, gOrca_XPos3_6FDB6A, 0x6FDB6A);
DEFINE_GLOBAL(u8, gOrca_YPos3_6FDB6B, 0x6FDB6B);
DEFINE_GLOBAL(u8, gOrca_ZPos3_6FDB6C, 0x6FDB6C);

DEFINE_GLOBAL(u8, gOrca_XPos4_6FDBF2, 0x6FDBF2);
DEFINE_GLOBAL(u8, gOrca_YPos4_6FDBF3, 0x6FDBF3);
DEFINE_GLOBAL(u8, gOrca_ZPos4_6FDBF4, 0x6FDBF4);
DEFINE_GLOBAL(u8, gOrca_idx1_6FDBF0, 0x6FDBF0);
DEFINE_GLOBAL(u8, gOrca_idx2_6FDBF1, 0x6FDBF1);

MATCH_FUNC(0x554080)
bool Orca_2FD4::CanMoveInDirection_554080(s32 path_direction)
{
    if (field_25_xpos > 1u && field_25_xpos < 254u && field_26_ypos > 1u && field_26_ypos < 254u)
    {
        bool result =
            gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(field_25_xpos, field_26_ypos, field_27_zpos, path_direction, &byte_6FDEEC, 1) ==
            0;
        return result;
    }
    return false;
}

MATCH_FUNC(0x5540e0)
char_type Orca_2FD4::TestDiagonalMove_5540E0(u8 curr_xpos, u8 curr_ypos, u8 curr_zpos, u8 desired_xpos, u8 desired_ypos)
{
    field_25_xpos = curr_xpos;
    field_26_ypos = curr_ypos;
    field_27_zpos = curr_zpos;
    return Internel_CanMoveDiagonally_554110(desired_xpos, desired_ypos);
}

WIP_FUNC(0x554110)
char_type Orca_2FD4::Internel_CanMoveDiagonally_554110(u8 desired_xpos, u8 desired_ypos)
{
    WIP_IMPLEMENTED;

    byte_6FDEEC = 0;

    const char_type xd = desired_xpos - field_25_xpos;
    const char_type yd = desired_ypos - field_26_ypos;
    char_type bCanMove;

    if (xd == 0)
    {
        if (yd == 0)
        {
            return true; // there is no moving: of course, it's allowed
        }
        if (yd == -1)
        {
            return CanMoveInDirection_554080(path_direction::up_1);
        }
        return CanMoveInDirection_554080(path_direction::down_2);
    }

    if (yd)
    {
        // here both xd and yd are different from zero

        // can't move into any direction if it's currently on a slope? weird
        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_25_xpos, field_26_ypos, field_27_zpos))
        {
            return false;
        }

        if (xd == 1)
        {
            if (yd == 1)
            {
                // moving southeast
                if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos))
                {
                    return false;
                }

                if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos))
                {
                    return false;
                }

                if (CanMoveInDirection_554080(path_direction::down_2) && CanMoveInDirection_554080(path_direction::right_3))
                {
                    if (gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos + 1,
                                                                       this->field_26_ypos,
                                                                       this->field_27_zpos,
                                                                       path_direction::down_2,
                                                                       &byte_6FDEEC,
                                                                       1))
                    {
                        return false;
                    }
                    bCanMove = gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                              this->field_26_ypos + 1,
                                                                              this->field_27_zpos,
                                                                              path_direction::right_3,
                                                                              &byte_6FDEEC,
                                                                              1);
                    return bCanMove == 0;
                }
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos, this->field_26_ypos - 1, this->field_27_zpos))
            {
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos))
            {
                return false;
            }

            if (!CanMoveInDirection_554080(path_direction::up_1) || !CanMoveInDirection_554080(path_direction::right_3))
            {
                return false;
            }

            if (gMap_0x370_6F6268
                    ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos, path_direction::up_1, &byte_6FDEEC, 1))
            {
                return false;
            }
            //v29 = 3;
            //LABEL_73:
            bCanMove = gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                      this->field_26_ypos - 1,
                                                                      this->field_27_zpos,
                                                                      path_direction::right_3,
                                                                      &byte_6FDEEC,
                                                                      1);
            return bCanMove == 0;
        }

        if (yd != 1)
        {
            if (gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos, this->field_26_ypos - 1, this->field_27_zpos))
            {
                return false;
            }

            if (gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos))
            {
                return false;
            }

            if (!CanMoveInDirection_554080(path_direction::up_1) || !CanMoveInDirection_554080(path_direction::left_4) ||
                gMap_0x370_6F6268
                    ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos, path_direction::up_1, &byte_6FDEEC, 1))
            {
                return false;
            }
            //v29 = 4;
            //goto LABEL_73;
            bCanMove = gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                      this->field_26_ypos - 1,
                                                                      this->field_27_zpos,
                                                                      path_direction::left_4,
                                                                      &byte_6FDEEC,
                                                                      1);
            return bCanMove == 0;
        }

        if (gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos))
        {
            return false;
        }

        if (gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos))
        {
            return false;
        }

        if (!CanMoveInDirection_554080(path_direction::down_2) || !CanMoveInDirection_554080(path_direction::left_4))
        {
            return false;
        }

        return !gMap_0x370_6F6268
                    ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos, path_direction::down_2, &byte_6FDEEC, 1) &&
            gMap_0x370_6F6268
                ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos, path_direction::left_4, &byte_6FDEEC, 1) == 0;
    }

    else if (xd == -1)
    {
        return CanMoveInDirection_554080(path_direction::left_4);
    }
    else
    {
        return CanMoveInDirection_554080(path_direction::right_3);
    }
}

MATCH_FUNC(0x5545c0)
void Orca_2FD4::Internel_ClearGrid_5545C0()
{
    memset(this->field_40, 0, sizeof(this->field_40));
}

MATCH_FUNC(0x5545e0)
void Orca_2FD4::init_5545E0()
{
    memset(this->field_40, 0, sizeof(this->field_40));
    field_38 = 0;
    field_34 = 1;
    field_36 = 0;
    field_3A = 0;
    field_2FD0_bTimedOut = 1;
    field_2FD1_time_out_counter = 0;
}

MATCH_FUNC(0x554620)
void Orca_2FD4::remove_ped_554620(s32 a2)
{
    if (a2 == field_0_ped_id)
    {
        field_2FD0_bTimedOut = 1;
        field_2FD1_time_out_counter = 0;
    }
}

STUB_FUNC(0x554640)
char_type Orca_2FD4::Internel_EvaluateBehaviorGridCell_554640()
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/f8WDL
WIP_FUNC(0x554710)
void Orca_2FD4::Internel_UpdateBehaviorGrid_554710()
{
    WIP_IMPLEMENTED;
    u16 v12;
    u8 zpos = field_22_zpos;
    if (byte_6FDEEC)
    {
        zpos += byte_6FDEEC;
    }

    if (zpos != field_13_xEnd)
    {
        v12 = 2;
    }
    else
    {
        v12 = 1;
    }

    field_1C_f40_idx = field_23_f40_idx1 + 34 * field_24_f40_idx2;
    Orca_8* p8 = &field_40[field_1C_f40_idx];
    if (field_40[field_1C_f40_idx].field_0_idx1 == 1 && zpos == p8->field_2_xpos)
    {
        p8->field_0_idx1 = 0;
    }
    else
    {
        u16 v7;
        if (field_4 == 0)
        {
            v7 = v12 *
                ((field_20_xpos - field_11_yStart) * (field_20_xpos - field_11_yStart) +
                 (field_21_ypos - field_12_xEnd) * (field_21_ypos - field_12_xEnd));
        }
        else
        {
            v7 = field_16;
        }
        field_8->field_0_idx1 = field_23_f40_idx1;
        field_8->field_1_idx2 = field_24_f40_idx2;
        field_8->field_2_xpos = field_20_xpos;
        field_8->field_3_ypos = field_21_ypos;
        field_8->field_4_zpos = zpos;
        field_8->field_6 = v7;
        ++field_C;
        ++field_8;

        if (p8->field_0_idx1 == 1)
        {
            p8->field_3_ypos = field_1B;
            p8->field_4_zpos = zpos;
        }
        else
        {
            p8->field_1_idx2 = field_1B;
            p8->field_2_xpos = zpos;
        }
        p8->field_6 = field_1E + 1;

        if (field_24_f40_idx2)
        {
            if (field_24_f40_idx2 == 31)
            {
                gOrca_Idx1_6FDBF8 = field_23_f40_idx1;
                gOrca_idx2_6FDBF9 = field_24_f40_idx2;
                gOrca_XPos2_6FDBFA = field_20_xpos;
                gOrca_YPos2_6FDBFB = field_21_ypos;
                gOrca_ZPos2_6FDBFC = field_22_zpos;
            }
        }
        else
        {
            gOrca_idx1_1_6FDBE0 = field_23_f40_idx1;
            gOrca_idx2_1_6FDBE1 = field_24_f40_idx2;
            gOrca_XPos_1_6FDBE2 = field_20_xpos;
            gOrca_YPos_1_6FDBE3 = field_21_ypos;
            gOrca_ZPos1_6FDBE4 = field_22_zpos;
        }

        if (field_23_f40_idx1)
        {
            if (field_23_f40_idx1 == 31)
            {
                gOrca_idx1_2_6FDB68 = 31;
                gOrca_idx_2_2_6FDB69 = field_24_f40_idx2;
                gOrca_XPos3_6FDB6A = field_20_xpos;
                gOrca_YPos3_6FDB6B = field_21_ypos;
                gOrca_ZPos3_6FDB6C = field_22_zpos;
            }
        }
        else
        {
            gOrca_idx1_6FDBF0 = 0;
            gOrca_idx2_6FDBF1 = field_24_f40_idx2;
            gOrca_XPos4_6FDBF2 = field_20_xpos;
            gOrca_YPos4_6FDBF3 = field_21_ypos;
            gOrca_ZPos4_6FDBF4 = field_22_zpos;
        }
    }
}

MATCH_FUNC(0x5548c0)
bool Orca_2FD4::Internal_ProcessBehaviorGrid_5548C0()
{
    char_type v2 = Orca_2FD4::Internel_EvaluateBehaviorGridCell_554640();
    if (v2 == 1)
    {
        if (Orca_2FD4::Internel_CanMoveDiagonally_554110(field_20_xpos, field_21_ypos))
        {
            Orca_2FD4::Internel_UpdateBehaviorGrid_554710();
        }
        else
        {
            Orca_8* pOrca = &field_40[field_1C_f40_idx];
            if (pOrca->field_0_idx1 == 1 && pOrca->field_3_ypos == 0)
            {
                pOrca->field_0_idx1 = 0;
            }
        }
    }
    else if (v2 == 2)
    {
        field_18 = 0;
        field_19 = 1;
        field_1A = 1;
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x554920)
void Orca_2FD4::RestoreSavedPosition_554920()
{
    switch (gOrca_AngleFace_6FDD38)
    {
        case 1:
            this->field_25_xpos = gOrca_XPos_1_6FDBE2;
            gOrca_XPosAny_6FDECA = gOrca_XPos_1_6FDBE2;
            this->field_26_ypos = gOrca_YPos_1_6FDBE3;
            gOrca_YPosAny_6FDECB = gOrca_YPos_1_6FDBE3;
            this->field_27_zpos = gOrca_ZPos1_6FDBE4;
            gOrca_ZPosAny_6FDECC = gOrca_ZPos1_6FDBE4;
            gOrca_idx1_any_6FDEC8 = gOrca_idx1_1_6FDBE0;
            gOrca_idx2_any_6FDEC9 = gOrca_idx2_1_6FDBE1;
            break;
        case 2:
            this->field_25_xpos = gOrca_XPos2_6FDBFA;
            gOrca_XPosAny_6FDECA = gOrca_XPos2_6FDBFA;
            this->field_26_ypos = gOrca_YPos2_6FDBFB;
            gOrca_YPosAny_6FDECB = gOrca_YPos2_6FDBFB;
            this->field_27_zpos = gOrca_ZPos2_6FDBFC;
            gOrca_ZPosAny_6FDECC = gOrca_ZPos2_6FDBFC;
            gOrca_idx1_any_6FDEC8 = gOrca_idx1_2_6FDB68;
            gOrca_idx2_any_6FDEC9 = gOrca_idx_2_2_6FDB69;
            break;
        case 3:
            this->field_25_xpos = gOrca_XPos3_6FDB6A;
            gOrca_XPosAny_6FDECA = gOrca_XPos3_6FDB6A;
            this->field_26_ypos = gOrca_YPos3_6FDB6B;
            gOrca_YPosAny_6FDECB = gOrca_YPos3_6FDB6B;
            this->field_27_zpos = gOrca_ZPos3_6FDB6C;
            gOrca_ZPosAny_6FDECC = gOrca_ZPos3_6FDB6C;
            gOrca_idx1_any_6FDEC8 = gOrca_idx1_2_6FDB68;
            gOrca_idx2_any_6FDEC9 = gOrca_idx_2_2_6FDB69;
            break;
        case 4:
            this->field_25_xpos = gOrca_XPos4_6FDBF2;
            gOrca_XPosAny_6FDECA = gOrca_XPos4_6FDBF2;
            this->field_26_ypos = gOrca_YPos4_6FDBF3;
            gOrca_YPosAny_6FDECB = gOrca_YPos4_6FDBF3;
            this->field_27_zpos = gOrca_ZPos4_6FDBF4;
            gOrca_ZPosAny_6FDECC = gOrca_ZPos4_6FDBF4;
            gOrca_idx1_any_6FDEC8 = gOrca_idx1_6FDBF0;
            gOrca_idx2_any_6FDEC9 = gOrca_idx2_6FDBF1;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x554a90)
s32 Orca_2FD4::IsFirstPassenger_554A90(Ped* a2)
{
    return field_3C_ped_list.field_0_pFirstPed->field_0_char_ped == a2;
}

STUB_FUNC(0x554ab0)
char_type Orca_2FD4::ComputePath_554AB0(s32 a2, Ped* a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8, u8 a9, s32 a10, u8* a11)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/19LIh
WIP_FUNC(0x5552b0)
bool Orca_2FD4::FindNearbyTileMatchingSlopeType_5552B0(u8 block_type, u8* xpos, u8* ypos, u8* zpos, char_type maybe_timer)
{
    WIP_IMPLEMENTED;
    u8 j = 0;
    if (field_38 && !maybe_timer)
    {
        return 0;
    }
    if (*zpos > 6u || *zpos == 0)
    {
        return 0;
    }
    u8 default_pos = dword_6FDD98.ToUInt8();
    field_27_zpos = default_pos;
    field_26_ypos = default_pos;
    field_25_xpos = default_pos;
    field_28 = *xpos - 16;
    field_29 = *ypos - 16;
    Orca_2FD4::Internel_ClearGrid_5545C0();
    field_E_xStart = *xpos;
    field_F_xStart = *ypos;
    field_10_yStart = *zpos;
    field_4 = 1;

    if (!gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_E_xStart, field_F_xStart, field_10_yStart))
    {
        field_10_yStart = gMap_0x370_6F6268->sub_4E4D40(Fix16(field_E_xStart), Fix16(field_F_xStart), Fix16(field_10_yStart)).ToUInt8();
    }
    field_16 = 0;
    field_11_yStart = 0;
    field_12_xEnd = 0;
    field_13_xEnd = 0;
    byte_6FDEEC = 0;
    field_C = 0;
    field_8 = field_2350;
    field_22_zpos = field_10_yStart;
    field_24_f40_idx2 = 16;
    field_23_f40_idx1 = 16;
    field_20_xpos = field_E_xStart;
    field_21_ypos = field_F_xStart;
    field_1B = 66;
    Orca_2FD4::Internel_UpdateBehaviorGrid_554710();
    field_8 = field_2350;
    field_1C_f40_idx = 560;
    field_40[field_1C_f40_idx].field_6 = field_16;
    field_1E = 0;
    field_18 = 1;
    while (field_C)
    {
        Orca_8* pIter = field_2350;
        field_8 = field_2350;

        for (u8 i = 0; i < field_C - 1; i++)
        {
            ++field_8;
            if (field_8->field_6 < pIter->field_6)
            {
                pIter = field_8;
            }
        }
        gOrca_idx1_any_6FDEC8 = pIter->field_0_idx1;
        gOrca_idx2_any_6FDEC9 = pIter->field_1_idx2;
        gOrca_XPosAny_6FDECA = pIter->field_2_xpos;
        gOrca_YPosAny_6FDECB = pIter->field_3_ypos;
        gOrca_ZPosAny_6FDECC = pIter->field_4_zpos;
        word_6FDECE = pIter->field_6;
        field_1C_f40_idx = gOrca_idx1_any_6FDEC8 + 34 * gOrca_idx2_any_6FDEC9;
        field_1E = field_40[field_1C_f40_idx].field_6;
        pIter->field_0_idx1 = field_8->field_0_idx1;
        pIter->field_1_idx2 = field_8->field_1_idx2;
        pIter->field_2_xpos = field_8->field_2_xpos;
        pIter->field_3_ypos = field_8->field_3_ypos;
        pIter->field_4_zpos = field_8->field_4_zpos;
        pIter->field_6 = field_8->field_6;
        --field_C;
        field_25_xpos = gOrca_XPosAny_6FDECA;
        field_26_ypos = gOrca_YPosAny_6FDECB;
        field_27_zpos = gOrca_ZPosAny_6FDECC;

        u8 block_below_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(field_25_xpos, field_26_ypos, field_27_zpos - 1);

        if (block_type == 5)
        {
            if (!gMap_0x370_6F6268->sub_4E5640(dword_6FDD50,
                                               dword_6FDC00,
                                               dword_6FDD50,
                                               Fix16(*xpos) + dword_6FDCA8,
                                               Fix16(*ypos) + dword_6FDCA8,
                                               Fix16(*zpos),
                                               Fix16(field_25_xpos) + dword_6FDCA8,
                                               Fix16(field_26_ypos) + dword_6FDCA8,
                                               Fix16(field_27_zpos)))
            {
                field_18 = 0;
                *xpos = field_25_xpos;
                *ypos = field_26_ypos;
                *zpos = field_27_zpos;
            }
        }
        else if (block_below_type == block_type)
        {
            if (block_type == 1)
            {
                if ((gMap_0x370_6F6268->get_block_4DFE10(field_25_xpos, field_26_ypos, field_27_zpos - 1)->field_A_arrows & 0xF) != 0)
                {
                    field_18 = 0;
                    *xpos = field_25_xpos;
                    *ypos = field_26_ypos;
                    *zpos = field_27_zpos;
                    return 1;
                }
            }
            else
            {
                field_18 = 0;
                *xpos = field_25_xpos;
                *ypos = field_26_ypos;
                *zpos = field_27_zpos;
            }
        }
        field_20_xpos = gOrca_XPosAny_6FDECA;
        field_21_ypos = gOrca_YPosAny_6FDECB - 1;
        field_22_zpos = gOrca_ZPosAny_6FDECC;
        field_23_f40_idx1 = gOrca_idx1_any_6FDEC8;
        field_24_f40_idx2 = gOrca_idx2_any_6FDEC9 - 1;
        field_1B = 1;
        if (!Orca_2FD4::Internal_ProcessBehaviorGrid_5548C0())
        {
            field_20_xpos = gOrca_XPosAny_6FDECA + 1;
            field_21_ypos = gOrca_YPosAny_6FDECB;
            field_22_zpos = gOrca_ZPosAny_6FDECC;
            field_23_f40_idx1 = gOrca_idx1_any_6FDEC8 + 1;
            field_24_f40_idx2 = gOrca_idx2_any_6FDEC9;
            field_1B = 2;
            if (!Orca_2FD4::Internal_ProcessBehaviorGrid_5548C0())
            {
                field_20_xpos = gOrca_XPosAny_6FDECA;
                field_21_ypos = gOrca_YPosAny_6FDECB + 1;
                field_22_zpos = gOrca_ZPosAny_6FDECC;
                field_23_f40_idx1 = gOrca_idx1_any_6FDEC8;
                field_24_f40_idx2 = gOrca_idx2_any_6FDEC9 + 1;
                field_1B = 3;
                if (!Orca_2FD4::Internal_ProcessBehaviorGrid_5548C0())
                {
                    field_20_xpos = gOrca_XPosAny_6FDECA - 1;
                    field_21_ypos = gOrca_YPosAny_6FDECB;
                    field_22_zpos = gOrca_ZPosAny_6FDECC;
                    field_23_f40_idx1 = gOrca_idx1_any_6FDEC8 - 1;
                    field_24_f40_idx2 = gOrca_idx2_any_6FDEC9;
                    field_1B = 4;
                    if (!Orca_2FD4::Internal_ProcessBehaviorGrid_5548C0())
                    {
                        ++field_16;
                        if (++j <= 6 || maybe_timer != 0)
                        {
                            if (field_18) // line 486
                            {
                                if (field_C) // line 48f
                                {
                                    continue;
                                }
                            }
                            else
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        return 0;
    } // end while
    return 1;
}
