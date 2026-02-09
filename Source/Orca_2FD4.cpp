#include "Orca_2FD4.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "map_0x370.hpp"
#include <string.h>

DEFINE_GLOBAL(u8, byte_6FDEEC, 0x6FDEEC);

DEFINE_GLOBAL(Orca_2FD4*, gOrca_2FD4_6FDEF0, 0x6FDEF0);

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
bool Orca_2FD4::CanMoveInDirection_554080(s32 a2)
{
    if (field_25_xpos > 1u && field_25_xpos < 254u && field_26_ypos > 1u && field_26_ypos < 254u)
    {
        bool result =
            gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130((u8)field_25_xpos, (u8)field_26_ypos, (u8)field_27_zpos, a2, &byte_6FDEEC, 1) ==
            0;
        return result;
    }
    return false;
}

MATCH_FUNC(0x5540e0)
char_type Orca_2FD4::TestDiagonalMove_5540E0(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6)
{
    this->field_25_xpos = a2;
    this->field_26_ypos = a3;
    this->field_27_zpos = a4;
    return Internel_CanMoveDiagonally_554110(a5, a6);
}

WIP_FUNC(0x554110)
char_type Orca_2FD4::Internel_CanMoveDiagonally_554110(char_type xpos2, char_type ypos2)
{
    WIP_IMPLEMENTED;

    byte_6FDEEC = 0;

    char_type f25_xpos = this->field_25_xpos;
    char_type f26_ypos = this->field_26_ypos;
    char_type xd_ = xpos2 - f25_xpos;
    char_type yd_ = ypos2 - f26_ypos;
    bool xd = xpos2 == f25_xpos;
    char_type yd = ypos2 - f26_ypos;
    char_type bCanMove;

    if (xd)
    {
        if (!yd_)
        {
            return 1;
        }
        if (yd_ == -1)
        {
            return CanMoveInDirection_554080(1);
        }
        return CanMoveInDirection_554080(2);
    }

    if (yd_)
    {
        if (gMap_0x370_6F6268->sub_466CF0(f25_xpos, f26_ypos, this->field_27_zpos))
        {
            return 0;
        }

        if (xd_ == 1)
        {
            if (yd == 1)
            {
                gmp_block_info* pBlock_ =
                    gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos);
                if (pBlock_)
                {
                    if ((pBlock_->field_B_slope_type & 0xFC) != 0 && (pBlock_->field_B_slope_type & 0xFCu) < 0xB4 &&
                        (pBlock_->field_B_slope_type & 3) != 0)
                    {
                        return 0;
                    }
                }

                gmp_block_info* pBlock__ =
                    gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos);
                if (pBlock__)
                {
                    if ((pBlock__->field_B_slope_type & 0xFC) != 0 && (pBlock__->field_B_slope_type & 0xFCu) < 0xB4 &&
                        (pBlock__->field_B_slope_type & 3) != 0)
                    {
                        return 0;
                    }
                }

                if (CanMoveInDirection_554080(2) && CanMoveInDirection_554080(3))
                {
                    if (gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos + 1,
                                                                       this->field_26_ypos,
                                                                       this->field_27_zpos,
                                                                       2,
                                                                       &byte_6FDEEC,
                                                                       1))
                    {
                        return 0;
                    }
                    bCanMove = gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                              this->field_26_ypos + 1,
                                                                              this->field_27_zpos,
                                                                              3,
                                                                              &byte_6FDEEC,
                                                                              1);
                    return bCanMove == 0;
                }
                return 0;
            }

            gmp_block_info* pBlock___1 =
                gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos, this->field_26_ypos - 1, this->field_27_zpos);
            if (pBlock___1)
            {
                if ((pBlock___1->field_B_slope_type & 0xFC) != 0 && (pBlock___1->field_B_slope_type & 0xFCu) < 0xB4 &&
                    (pBlock___1->field_B_slope_type & 3) != 0)
                {
                    return 0;
                }
            }

            gmp_block_info* pBlock_1 =
                gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos);
            if (pBlock_1)
            {
                if ((pBlock_1->field_B_slope_type & 0xFC) != 0 && (pBlock_1->field_B_slope_type & 0xFCu) < 0xB4 &&
                    (pBlock_1->field_B_slope_type & 3) != 0)
                {
                    return 0;
                }
            }

            if (!CanMoveInDirection_554080(1) || !CanMoveInDirection_554080(3))
            {
                return 0;
            }

            if (gMap_0x370_6F6268
                    ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos, 1, &byte_6FDEEC, 1))
            {
                return 0;
            }
            //v29 = 3;
            //LABEL_73:
            bCanMove = gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                      this->field_26_ypos - 1,
                                                                      this->field_27_zpos,
                                                                      3,
                                                                      &byte_6FDEEC,
                                                                      1);
            return bCanMove == 0;
        }
        if (yd != 1)
        {
            gmp_block_info* pBlock_2 =
                gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos, this->field_26_ypos - 1, this->field_27_zpos);
            if (pBlock_2)
            {
                if ((pBlock_2->field_B_slope_type & 0xFC) != 0 && (pBlock_2->field_B_slope_type & 0xFCu) < 0xB4 &&
                    (pBlock_2->field_B_slope_type & 3) != 0)
                {
                    return 0;
                }
            }

            gmp_block_info* pBlock_3 =
                gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos);
            if (pBlock_3)
            {
                if ((pBlock_3->field_B_slope_type & 0xFC) != 0 && (pBlock_3->field_B_slope_type & 0xFCu) < 0xB4 &&
                    (pBlock_3->field_B_slope_type & 3) != 0)
                {
                    return 0;
                }
            }

            if (!CanMoveInDirection_554080(1) || !CanMoveInDirection_554080(4) ||
                gMap_0x370_6F6268
                    ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos, 1, &byte_6FDEEC, 1))
            {
                return 0;
            }
            //v29 = 4;
            //goto LABEL_73;
            bCanMove = gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                      this->field_26_ypos - 1,
                                                                      this->field_27_zpos,
                                                                      4,
                                                                      &byte_6FDEEC,
                                                                      1);
            return bCanMove == 0;
        }

        gmp_block_info* pBlock_5 = gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos);
        if (pBlock_5)
        {
            if ((pBlock_5->field_B_slope_type & 0xFC) != 0 && (pBlock_5->field_B_slope_type & 0xFCu) < 0xB4 &&
                (pBlock_5->field_B_slope_type & 3) != 0)
            {
                return 0;
            }
        }

        gmp_block_info* pBlock_4 = gMap_0x370_6F6268->get_block_4DFE10(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos);
        if (pBlock_4)
        {
            if ((pBlock_4->field_B_slope_type & 0xFC) != 0 && (pBlock_4->field_B_slope_type & 0xFCu) < 0xB4 &&
                (pBlock_4->field_B_slope_type & 3) != 0)
            {
                return 0;
            }
        }

        if (!CanMoveInDirection_554080(2) || !CanMoveInDirection_554080(4))
        {
            return 0;
        }

        return !gMap_0x370_6F6268
                    ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos, 2, &byte_6FDEEC, 1) &&
            gMap_0x370_6F6268
                ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos, 4, &byte_6FDEEC, 1) == 0;
    }
    else if (xd_ == -1)
    {
        return CanMoveInDirection_554080(4);
    }
    else
    {
        return CanMoveInDirection_554080(3);
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

STUB_FUNC(0x554710)
char_type Orca_2FD4::Internel_UpdateBehaviorGrid_554710()
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x5548c0)
char_type Orca_2FD4::Internal_ProcessBehaviorGrid_5548C0()
{
    WIP_IMPLEMENTED;

    char_type v2 = Orca_2FD4::Internel_EvaluateBehaviorGridCell_554640();
    if (v2 == 1)
    {
        if (Orca_2FD4::Internel_CanMoveDiagonally_554110(this->field_20_xpos, this->field_21_ypos))
        {
            Orca_2FD4::Internel_UpdateBehaviorGrid_554710();
            return 0;
        }

        u16 idx = this->field_1C_f40_idx;
        if (this->field_40[idx].field_0 != 1 || this->field_40[idx].field_3)
        {
            return 0;
        }
        this->field_40[idx].field_0 = 0;
        return 0;
    }
    else
    {
        if (v2 != 2)
        {
            return 0;
        }
        this->field_18 = 0;
        this->field_19 = 1;
        this->field_1A = 1;
        return 1;
    }
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
char_type Orca_2FD4::ComputePath_554AB0(s32 a2, s32 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8, u8 a9, s32 a10, u8* a11)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5552b0)
char_type Orca_2FD4::FindNearbyTileMatchingSlopeType_5552B0(char_type a2, u8* xpos, u8* ypos, u8* zpos, char_type a6)
{
    NOT_IMPLEMENTED;
    return 0;
}