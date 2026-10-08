#include "PathFinder_2FD4.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "map_0x370.hpp"
#include <string.h>

DEFINE_GLOBAL(u8, gPathFinder_SlopeZDelta_6FDEEC, 0x6FDEEC);
DEFINE_GLOBAL(s16, gPathFinder_best_cost_6FDECE, 0x6FDECE);

DEFINE_GLOBAL(PathFinder_2FD4*, gPathFinder_6FDEF0, 0x6FDEF0);

DEFINE_GLOBAL_INIT(Fix16, kFpZero_6FDD98, Fix16(0), 0x6FDD98);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FDD50, Fix16(0x100, 0), 0x6FDD50);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FDC00, Fix16(0.25), 0x6FDC00);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDCA8, Fix16(0.5), 0x6FDCA8);

DEFINE_GLOBAL(u8, gPathFinder_edge2_grid_x_6FDBF8, 0x6FDBF8);
DEFINE_GLOBAL(u8, gPathFinder_edge2_grid_y_6FDBF9, 0x6FDBF9);

DEFINE_GLOBAL(s32, gPathFinder_angle_face_6FDD38, 0x6FDD38);
DEFINE_GLOBAL(u8, gPathFinder_edge1_x_6FDBE2, 0x6FDBE2);
DEFINE_GLOBAL(u8, gPathFinder_edge1_y_6FDBE3, 0x6FDBE3);
DEFINE_GLOBAL(u8, gPathFinder_edge1_z_6FDBE4, 0x6FDBE4);
DEFINE_GLOBAL(u8, gPathFinder_edge1_grid_x_6FDBE0, 0x6FDBE0);
DEFINE_GLOBAL(u8, gPathFinder_edge1_grid_y_6FDBE1, 0x6FDBE1);

DEFINE_GLOBAL(u8, gPathFinder_best_x_6FDECA, 0x6FDECA);
DEFINE_GLOBAL(u8, gPathFinder_best_y_6FDECB, 0x6FDECB);
DEFINE_GLOBAL(u8, gPathFinder_best_z_6FDECC, 0x6FDECC);
DEFINE_GLOBAL(u8, gPathFinder_best_grid_x_6FDEC8, 0x6FDEC8);
DEFINE_GLOBAL(u8, gPathFinder_best_grid_y_6FDEC9, 0x6FDEC9);

DEFINE_GLOBAL(u8, gPathFinder_edge2_x_6FDBFA, 0x6FDBFA);
DEFINE_GLOBAL(u8, gPathFinder_edge2_y_6FDBFB, 0x6FDBFB);
DEFINE_GLOBAL(u8, gPathFinder_edge2_z_6FDBFC, 0x6FDBFC);
DEFINE_GLOBAL(u8, gPathFinder_edge3_grid_x_6FDB68, 0x6FDB68);
DEFINE_GLOBAL(u8, gPathFinder_edge3_grid_y_6FDB69, 0x6FDB69);

DEFINE_GLOBAL(u8, gPathFinder_edge3_x_6FDB6A, 0x6FDB6A);
DEFINE_GLOBAL(u8, gPathFinder_edge3_y_6FDB6B, 0x6FDB6B);
DEFINE_GLOBAL(u8, gPathFinder_edge3_z_6FDB6C, 0x6FDB6C);

DEFINE_GLOBAL(u8, gPathFinder_edge4_x_6FDBF2, 0x6FDBF2);
DEFINE_GLOBAL(u8, gPathFinder_edge4_y_6FDBF3, 0x6FDBF3);
DEFINE_GLOBAL(u8, gPathFinder_edge4_z_6FDBF4, 0x6FDBF4);
DEFINE_GLOBAL(u8, gPathFinder_edge4_grid_x_6FDBF0, 0x6FDBF0);
DEFINE_GLOBAL(u8, gPathFinder_edge4_grid_y_6FDBF1, 0x6FDBF1);

MATCH_FUNC(0x554080)
bool PathFinder_2FD4::CanMoveInDirection_554080(s32 path_direction)
{
    if (field_25_xpos > 1u && field_25_xpos < 254u && field_26_ypos > 1u && field_26_ypos < 254u)
    {
        bool result =
            gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(field_25_xpos, field_26_ypos, field_27_zpos, path_direction, &gPathFinder_SlopeZDelta_6FDEEC, 1) ==
            0;
        return result;
    }
    return false;
}

MATCH_FUNC(0x5540e0)
char_type PathFinder_2FD4::TestDiagonalMove_5540E0(u8 curr_xpos, u8 curr_ypos, u8 curr_zpos, u8 desired_xpos, u8 desired_ypos)
{
    field_25_xpos = curr_xpos;
    field_26_ypos = curr_ypos;
    field_27_zpos = curr_zpos;
    return CanMoveDiagonally_554110(desired_xpos, desired_ypos);
}

MATCH_FUNC(0x554110)
char_type PathFinder_2FD4::CanMoveDiagonally_554110(u8 desired_xpos, u8 desired_ypos)
{

    gPathFinder_SlopeZDelta_6FDEEC = 0;

    const char_type xd = desired_xpos - field_25_xpos;
    const char_type yd = desired_ypos - field_26_ypos;

    if (xd == 0 && yd == 0)
    {
        return true; // there is no moving: of course, it's allowed
    }

    if (xd != 0 && yd != 0)
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
                                                                       &gPathFinder_SlopeZDelta_6FDEEC,
                                                                       1))
                    {
                        return false;
                    }
                    return gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                              this->field_26_ypos + 1,
                                                                              this->field_27_zpos,
                                                                              path_direction::right_3,
                                                                              &gPathFinder_SlopeZDelta_6FDEEC,
                                                                              1) ? false : true;
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
                    ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos, path_direction::up_1, &gPathFinder_SlopeZDelta_6FDEEC, 1))
            {
                return false;
            }
            //v29 = 3;
            //LABEL_73:
            return gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                      this->field_26_ypos - 1,
                                                                      this->field_27_zpos,
                                                                      path_direction::right_3,
                                                                      &gPathFinder_SlopeZDelta_6FDEEC,
                                                                      1) ? false : true;
        }

        if (yd == 1)
        {
            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos))
            {
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos))
            {
                return false;
            }

            if (!CanMoveInDirection_554080(path_direction::down_2) || !CanMoveInDirection_554080(path_direction::left_4))
            {
                return false;
            }

            if (gMap_0x370_6F6268
                    ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos, path_direction::down_2, &gPathFinder_SlopeZDelta_6FDEEC, 1))
            {
                return false;
            }
            return gMap_0x370_6F6268
                       ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos, path_direction::left_4, &gPathFinder_SlopeZDelta_6FDEEC, 1) ? false : true;
        }

        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos, this->field_26_ypos - 1, this->field_27_zpos))
        {
            return false;
        }

        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos))
        {
            return false;
        }

        if (!CanMoveInDirection_554080(path_direction::up_1) || !CanMoveInDirection_554080(path_direction::left_4))
        {
            return false;
        }

        if (gMap_0x370_6F6268
                ->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos, path_direction::up_1, &gPathFinder_SlopeZDelta_6FDEEC, 1))
        {
            return false;
        }
        //v29 = 4;
        //goto LABEL_73;
        return gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(this->field_25_xpos,
                                                                  this->field_26_ypos - 1,
                                                                  this->field_27_zpos,
                                                                  path_direction::left_4,
                                                                  &gPathFinder_SlopeZDelta_6FDEEC,
                                                                  1) ? false : true;
    }

    if (xd != 0)
    {
        if (xd == -1)
        {
            return CanMoveInDirection_554080(path_direction::left_4);
        }
        return CanMoveInDirection_554080(path_direction::right_3);
    }

    if (yd == -1)
    {
        return CanMoveInDirection_554080(path_direction::up_1);
    }
    return CanMoveInDirection_554080(path_direction::down_2);
}

MATCH_FUNC(0x5545c0)
void PathFinder_2FD4::ClearGrid_5545C0()
{
    memset(this->field_40_grid, 0, sizeof(this->field_40_grid));
}

MATCH_FUNC(0x5545e0)
void PathFinder_2FD4::Init_5545E0()
{
    memset(this->field_40_grid, 0, sizeof(this->field_40_grid));
    field_38_bComputePathInProgress = 0;
    field_34 = 1;
    field_36 = 0;
    field_3A = 0;
    field_2FD0_bTimedOut = 1;
    field_2FD1_time_out_counter = 0;
}

MATCH_FUNC(0x554620)
void PathFinder_2FD4::RemovePed_554620(s32 ped_id)
{
    if (ped_id == field_0_ped_id)
    {
        field_2FD0_bTimedOut = 1;
        field_2FD1_time_out_counter = 0;
    }
}

MATCH_FUNC(0x554640)
char_type PathFinder_2FD4::EvaluateGridCell_554640()
{
    if (field_23_grid_x <= 32)
    {
        if (field_24_grid_y <= 32)
        {
            if (field_22_zpos <= 8)
            {
                field_1C_grid_idx = field_23_grid_x + 34 * field_24_grid_y;
                PathNode_8* p8 = &field_40_grid[field_1C_grid_idx]; // 1122 len
                if (p8->field_1_idx2 == 0)
                {
                    return 1;
                }
                if (p8->field_0_idx1 != 1 && p8->field_2_xpos != field_22_zpos)
                {
                    if (abs(p8->field_2_xpos - field_22_zpos) >= 1)
                    {
                        p8->field_0_idx1 = 1;
                        return 1;
                    }
                }
                return 0;
            }
        }
    }
    if (field_10_zStart != field_13_zEnd)
    {
        return 0;
    }
    switch (gPathFinder_angle_face_6FDD38)
    {
        case path_direction::up_1:
            if (field_24_grid_y < 1)
            {
                return 2;
            }
            break;
        case path_direction::down_2:
            if (field_24_grid_y > 32)
            {
                return 2;
            }
            break;
        case path_direction::right_3:
            if (field_23_grid_x > 32)
            {
                return 2;
            }
            break;
        case path_direction::left_4:
            if (field_23_grid_x < 1)
            {
                return 2;
            }
            break;
    }
    return 0;
}

// https://decomp.me/scratch/f8WDL
WIP_FUNC(0x554710)
void PathFinder_2FD4::AddGridCell_554710()
{
    WIP_IMPLEMENTED;
    u16 v12;
    u8 zpos = field_22_zpos;
    if (gPathFinder_SlopeZDelta_6FDEEC)
    {
        zpos += gPathFinder_SlopeZDelta_6FDEEC;
    }

    if (zpos != field_13_zEnd)
    {
        v12 = 2;
    }
    else
    {
        v12 = 1;
    }

    field_1C_grid_idx = field_23_grid_x + 34 * field_24_grid_y;
    PathNode_8* p8 = &field_40_grid[field_1C_grid_idx];
    if (field_40_grid[field_1C_grid_idx].field_0_idx1 == 1 && zpos == p8->field_2_xpos)
    {
        p8->field_0_idx1 = 0;
    }
    else
    {
        u16 v7;
        if (field_4_bFindTileMode == 0)
        {
            // The sum assigned first and `*= v12` after (one expression keeps v12's multiply in the sum's
            // register and pushes ebp at the top instead of inside this branch).
            v7 = (field_21_ypos - field_12_yEnd) * (field_21_ypos - field_12_yEnd) + (field_20_xpos - field_11_xEnd) * (field_20_xpos - field_11_xEnd);
            v7 *= v12;
        }
        else
        {
            v7 = field_16;
        }
        field_8_pNode->field_0_idx1 = field_23_grid_x;
        field_8_pNode->field_1_idx2 = field_24_grid_y;
        field_8_pNode->field_2_xpos = field_20_xpos;
        field_8_pNode->field_3_ypos = field_21_ypos;
        field_8_pNode->field_4_zpos = zpos;
        field_8_pNode->field_6_cost = v7;
        ++field_8_pNode;
        ++field_C_node_count;

        if (p8->field_0_idx1 == 1)
        {
            p8->field_3_ypos = field_1B_direction;
            p8->field_4_zpos = zpos;
        }
        else
        {
            p8->field_1_idx2 = field_1B_direction;
            p8->field_2_xpos = zpos;
        }
        p8->field_6_cost = field_1E_current_cost + 1;

        if (field_24_grid_y)
        {
            if (field_24_grid_y == 31)
            {
                gPathFinder_edge2_grid_x_6FDBF8 = field_23_grid_x;
                gPathFinder_edge2_grid_y_6FDBF9 = field_24_grid_y;
                gPathFinder_edge2_x_6FDBFA = field_20_xpos;
                gPathFinder_edge2_y_6FDBFB = field_21_ypos;
                gPathFinder_edge2_z_6FDBFC = field_22_zpos;
            }
        }
        else
        {
            gPathFinder_edge1_grid_x_6FDBE0 = field_23_grid_x;
            gPathFinder_edge1_grid_y_6FDBE1 = field_24_grid_y;
            gPathFinder_edge1_x_6FDBE2 = field_20_xpos;
            gPathFinder_edge1_y_6FDBE3 = field_21_ypos;
            gPathFinder_edge1_z_6FDBE4 = field_22_zpos;
        }

        if (field_23_grid_x)
        {
            if (field_23_grid_x == 31)
            {
                gPathFinder_edge3_grid_x_6FDB68 = 31;
                gPathFinder_edge3_grid_y_6FDB69 = field_24_grid_y;
                gPathFinder_edge3_x_6FDB6A = field_20_xpos;
                gPathFinder_edge3_y_6FDB6B = field_21_ypos;
                gPathFinder_edge3_z_6FDB6C = field_22_zpos;
            }
        }
        else
        {
            gPathFinder_edge4_grid_x_6FDBF0 = 0;
            gPathFinder_edge4_grid_y_6FDBF1 = field_24_grid_y;
            gPathFinder_edge4_x_6FDBF2 = field_20_xpos;
            gPathFinder_edge4_y_6FDBF3 = field_21_ypos;
            gPathFinder_edge4_z_6FDBF4 = field_22_zpos;
        }
    }
}

MATCH_FUNC(0x5548c0)
bool PathFinder_2FD4::ProcessGridCell_5548C0()
{
    char_type v2 = PathFinder_2FD4::EvaluateGridCell_554640();
    if (v2 == 1)
    {
        if (PathFinder_2FD4::CanMoveDiagonally_554110(field_20_xpos, field_21_ypos))
        {
            PathFinder_2FD4::AddGridCell_554710();
        }
        else
        {
            PathNode_8* pCell = &field_40_grid[field_1C_grid_idx];
            if (pCell->field_0_idx1 == 1 && pCell->field_3_ypos == 0)
            {
                pCell->field_0_idx1 = 0;
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
void PathFinder_2FD4::RestoreSavedPosition_554920()
{
    switch (gPathFinder_angle_face_6FDD38)
    {
        case path_direction::up_1:
            this->field_25_xpos = gPathFinder_edge1_x_6FDBE2;
            gPathFinder_best_x_6FDECA = gPathFinder_edge1_x_6FDBE2;
            this->field_26_ypos = gPathFinder_edge1_y_6FDBE3;
            gPathFinder_best_y_6FDECB = gPathFinder_edge1_y_6FDBE3;
            this->field_27_zpos = gPathFinder_edge1_z_6FDBE4;
            gPathFinder_best_z_6FDECC = gPathFinder_edge1_z_6FDBE4;
            gPathFinder_best_grid_x_6FDEC8 = gPathFinder_edge1_grid_x_6FDBE0;
            gPathFinder_best_grid_y_6FDEC9 = gPathFinder_edge1_grid_y_6FDBE1;
            break;
        case path_direction::down_2:
            this->field_25_xpos = gPathFinder_edge2_x_6FDBFA;
            gPathFinder_best_x_6FDECA = gPathFinder_edge2_x_6FDBFA;
            this->field_26_ypos = gPathFinder_edge2_y_6FDBFB;
            gPathFinder_best_y_6FDECB = gPathFinder_edge2_y_6FDBFB;
            this->field_27_zpos = gPathFinder_edge2_z_6FDBFC;
            gPathFinder_best_z_6FDECC = gPathFinder_edge2_z_6FDBFC;
            gPathFinder_best_grid_x_6FDEC8 = gPathFinder_edge3_grid_x_6FDB68;
            gPathFinder_best_grid_y_6FDEC9 = gPathFinder_edge3_grid_y_6FDB69;
            break;
        case path_direction::right_3:
            this->field_25_xpos = gPathFinder_edge3_x_6FDB6A;
            gPathFinder_best_x_6FDECA = gPathFinder_edge3_x_6FDB6A;
            this->field_26_ypos = gPathFinder_edge3_y_6FDB6B;
            gPathFinder_best_y_6FDECB = gPathFinder_edge3_y_6FDB6B;
            this->field_27_zpos = gPathFinder_edge3_z_6FDB6C;
            gPathFinder_best_z_6FDECC = gPathFinder_edge3_z_6FDB6C;
            gPathFinder_best_grid_x_6FDEC8 = gPathFinder_edge3_grid_x_6FDB68;
            gPathFinder_best_grid_y_6FDEC9 = gPathFinder_edge3_grid_y_6FDB69;
            break;
        case path_direction::left_4:
            this->field_25_xpos = gPathFinder_edge4_x_6FDBF2;
            gPathFinder_best_x_6FDECA = gPathFinder_edge4_x_6FDBF2;
            this->field_26_ypos = gPathFinder_edge4_y_6FDBF3;
            gPathFinder_best_y_6FDECB = gPathFinder_edge4_y_6FDBF3;
            this->field_27_zpos = gPathFinder_edge4_z_6FDBF4;
            gPathFinder_best_z_6FDECC = gPathFinder_edge4_z_6FDBF4;
            gPathFinder_best_grid_x_6FDEC8 = gPathFinder_edge4_grid_x_6FDBF0;
            gPathFinder_best_grid_y_6FDEC9 = gPathFinder_edge4_grid_y_6FDBF1;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x554a90)
s32 PathFinder_2FD4::IsFirstPassenger_554A90(Ped* pPed)
{
    return field_3C_ped_list.field_0_pFirstPed->field_0_char_ped == pPed;
}

// https://decomp.me/scratch/Fr0bT
WIP_FUNC(0x554ab0)
char_type PathFinder_2FD4::ComputePath_554AB0(s32 ped_id,
                                        Ped* pPed,
                                        u8 x_start,
                                        u8 y_start,
                                        u8 z_start,
                                        u8 x_end,
                                        u8 y_end,
                                        u8 z_end,
                                        s32 angle_face,
                                        u8* pOutPathFailCount)
{
    WIP_IMPLEMENTED;

    PathNode_8* v23; // eax
    PathNode_8* v40; // ecx
    u8 yCoord;
    u8 idx1;
    u8 idx2;
    u8 cur_x;
    u8 cur_z;
    u8 new_z; // only stored to cur_z on the paths that add a node
    u8 i;

    PatrolPoint_3* pPatrolPoint_2;
    u16 j;

    field_2E_iteration_budget = 100;
    field_0_ped_id = ped_id;
    gPathFinder_edge1_grid_x_6FDBE0 = 0;
    gPathFinder_edge1_grid_y_6FDBE1 = 0;
    gPathFinder_edge1_x_6FDBE2 = 0;
    gPathFinder_edge1_y_6FDBE3 = 0;
    gPathFinder_edge1_z_6FDBE4 = 0;
    gPathFinder_edge2_grid_x_6FDBF8 = 0;
    gPathFinder_edge2_grid_y_6FDBF9 = 0;
    gPathFinder_edge2_x_6FDBFA = 0;
    gPathFinder_edge2_y_6FDBFB = 0;
    gPathFinder_edge2_z_6FDBFC = 0;
    gPathFinder_edge3_grid_x_6FDB68 = 0;
    gPathFinder_edge3_grid_y_6FDB69 = 0;
    gPathFinder_edge3_x_6FDB6A = 0;
    gPathFinder_edge3_y_6FDB6B = 0;
    gPathFinder_edge3_z_6FDB6C = 0;
    gPathFinder_edge4_grid_x_6FDBF0 = 0;
    gPathFinder_edge4_grid_y_6FDBF1 = 0;
    gPathFinder_edge4_x_6FDBF2 = 0;
    gPathFinder_edge4_y_6FDBF3 = 0;
    gPathFinder_edge4_z_6FDBF4 = 0;
    if (field_2FD0_bTimedOut)
    {
        field_E_xStart = x_start;
        field_F_yStart = y_start;
        field_10_zStart = z_start;

        field_11_xEnd = x_end;
        field_12_yEnd = y_end;
        field_13_zEnd = z_end;

        field_2FD1_time_out_counter = 0;
        gPathFinder_angle_face_6FDD38 = angle_face;
        field_4_bFindTileMode = 0;
        PathFinder_2FD4::ClearGrid_5545C0();

        if (!gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(x_start, y_start, z_start))
        {
            field_10_zStart = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(Fix16(x_start), Fix16(y_start), Fix16(z_start)).ToUInt8();
        }
        gPathFinder_SlopeZDelta_6FDEEC = 0;
        field_20_xpos = x_start;
        field_C_node_count = 0;
        field_8_pNode = field_2350_nodes;
        field_21_ypos = y_start;
        field_22_zpos = z_start;
        field_23_grid_x = 16;
        field_24_grid_y = 16;
        field_1B_direction = 66;
        PathFinder_2FD4::AddGridCell_554710();
        field_8_pNode = field_2350_nodes;
        field_1C_grid_idx = 560;
        field_40_grid[field_1C_grid_idx].field_6_cost = 0;
        field_2FD0_bTimedOut = 0;
        field_1E_current_cost = 0;
        field_18 = 1;
    }
    ++field_2FD1_time_out_counter;
    if (field_2FD1_time_out_counter > 200)
    {
        field_2FD0_bTimedOut = 1;
        field_38_bComputePathInProgress = 0;
        return 1;
    }
    if (field_C_node_count)
    {
        while (1)
        {
            j = 0;
            field_38_bComputePathInProgress = 1;
            field_8_pNode = field_2350_nodes;
            v23 = field_2350_nodes;

            for (j = 0; j < field_C_node_count - 1; j++)
            {
                ++field_8_pNode;
                if (field_8_pNode->field_6_cost < v23->field_6_cost)
                {
                    v23 = field_8_pNode;
                }
            }

            gPathFinder_best_grid_x_6FDEC8 = v23->field_0_idx1;
            gPathFinder_best_grid_y_6FDEC9 = v23->field_1_idx2;
            gPathFinder_best_x_6FDECA = v23->field_2_xpos;
            gPathFinder_best_y_6FDECB = v23->field_3_ypos;
            gPathFinder_best_z_6FDECC = v23->field_4_zpos;
            gPathFinder_best_cost_6FDECE = v23->field_6_cost;
            field_1C_grid_idx = gPathFinder_best_grid_x_6FDEC8 + 34 * gPathFinder_best_grid_y_6FDEC9;
            field_1E_current_cost = field_40_grid[field_1C_grid_idx].field_6_cost;
            v23->field_0_idx1 = field_8_pNode->field_0_idx1;
            v23->field_1_idx2 = field_8_pNode->field_1_idx2;
            v23->field_2_xpos = field_8_pNode->field_2_xpos;
            v23->field_3_ypos = field_8_pNode->field_3_ypos;
            v23->field_4_zpos = field_8_pNode->field_4_zpos;
            v23->field_6_cost = field_8_pNode->field_6_cost;
            --field_C_node_count;
            field_25_xpos = gPathFinder_best_x_6FDECA;
            field_26_ypos = gPathFinder_best_y_6FDECB;
            field_27_zpos = gPathFinder_best_z_6FDECC;
            if (field_25_xpos == field_11_xEnd && field_26_ypos == field_12_yEnd && field_27_zpos == field_13_zEnd)
            {
                field_18 = 0;
                field_19 = 1;
                field_1A = 1;
                goto LABEL_35;
            }
            field_20_xpos = gPathFinder_best_x_6FDECA;
            field_21_ypos = gPathFinder_best_y_6FDECB - 1;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9 - 1;
            field_1B_direction = 1;

            if (PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                goto LABEL_35;
            }
            field_20_xpos = gPathFinder_best_x_6FDECA + 1;
            field_21_ypos = gPathFinder_best_y_6FDECB - 1;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8 + 1;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9 - 1;
            field_1B_direction = 5;

            if (PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                goto LABEL_35;
            }
            field_20_xpos = gPathFinder_best_x_6FDECA + 1;
            field_21_ypos = gPathFinder_best_y_6FDECB;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8 + 1;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9;
            field_1B_direction = 2;

            if (PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                goto LABEL_35;
            }
            field_20_xpos = gPathFinder_best_x_6FDECA + 1;
            field_21_ypos = gPathFinder_best_y_6FDECB + 1;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8 + 1;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9 + 1;
            field_1B_direction = 6;

            if (PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                goto LABEL_35;
            }
            field_20_xpos = gPathFinder_best_x_6FDECA;
            field_21_ypos = gPathFinder_best_y_6FDECB + 1;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9 + 1;
            field_1B_direction = 3;
            if (PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                goto LABEL_35;
            }
            field_20_xpos = gPathFinder_best_x_6FDECA - 1;
            field_21_ypos = gPathFinder_best_y_6FDECB + 1;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8 - 1;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9 + 1;
            field_1B_direction = 7;
            if (PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                goto LABEL_35;
            }
            field_20_xpos = gPathFinder_best_x_6FDECA - 1;
            field_21_ypos = gPathFinder_best_y_6FDECB;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8 - 1;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9;
            field_1B_direction = 4;
            if (PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                goto LABEL_35;
            }
            field_20_xpos = gPathFinder_best_x_6FDECA - 1;
            field_21_ypos = gPathFinder_best_y_6FDECB - 1;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8 - 1;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9 - 1;
            field_1B_direction = 8;

            if (PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                goto LABEL_35;
            }
            if (!field_C_node_count)
            {
                field_18 = 0;
                field_19 = 1;
                PathFinder_2FD4::RestoreSavedPosition_554920();
                if (gPathFinder_best_x_6FDECA)
                {
                    goto LABEL_30;
                }
                goto LABEL_61;
            }
            field_14 = 1;
            if (field_2E_iteration_budget == 0)
            {
                return 0;
            }
        LABEL_30:
            if (field_2E_iteration_budget > 0)
            {
                --field_2E_iteration_budget;
            }
            if (!field_18)
            {
                goto LABEL_35;
            }
        } // end while
    }

LABEL_35:
    field_14 = 0;
    cur_x = field_25_xpos;
    yCoord = field_26_ypos;
    cur_z = field_27_zpos;
    idx1 = gPathFinder_best_grid_x_6FDEC8;
    idx2 = gPathFinder_best_grid_y_6FDEC9;
    field_8_pNode = field_2350_nodes;
    field_8_pNode->field_2_xpos = cur_x;
    field_8_pNode->field_3_ypos = yCoord;
    field_8_pNode->field_4_zpos = cur_z;
    field_C_node_count = 1;
    v40 = &field_40_grid[(s16)(idx1 + 34 * idx2)];
    while (2)
    {
        if (v40->field_0_idx1 == 1)
        {
            if (abs(cur_z - (u8)v40->field_4_zpos) < 1)
            {
                // new_z first, so the branches don't start with the same ypos load (VC6 hoists it above
                // the jge). Left: the original stores field_1B before loading new_z / t
                new_z = v40->field_4_zpos;
                field_1B_direction = v40->field_3_ypos;
                v40->field_0_idx1 = 0;
            }
            else
            {
                // ypos read first: with idx2 first VC6 swaps al/dl for new_z and the switch index
                u8 t = v40->field_3_ypos;
                field_1B_direction = v40->field_1_idx2;
                new_z = v40->field_2_xpos;
                v40->field_1_idx2 = t;
                v40->field_0_idx1 = 0;
                v40->field_2_xpos = v40->field_4_zpos;
            }
        }
        else
        {
            field_1B_direction = v40->field_1_idx2;
            new_z = v40->field_2_xpos;
            v40->field_1_idx2 = 0;
        }
        switch (field_1B_direction)
        {
            case 1:
                v40 += 34; // 34
                ++yCoord;
                goto LABEL_52;
            case 3:
                v40 -= 34;
                --yCoord;
                goto LABEL_52;
            case 2:
                --v40;
                cur_x--;
                goto LABEL_52;
            case 4:
                ++v40;
                goto LABEL_51;
            case 5:
                ++yCoord;
                v40 += 33;
                cur_x--;
                goto LABEL_52;
            case 6:
                --yCoord;
                v40 -= 35;
                cur_x--;
                goto LABEL_52;
            case 7:
                --yCoord;
                v40 -= 33;
                goto LABEL_51;
            case 8:
                ++yCoord;
                v40 += 35;
            LABEL_51:
                ++cur_x;
            LABEL_52:
                field_8_pNode++;
                cur_z = new_z;
                field_8_pNode->field_2_xpos = cur_x;
                field_8_pNode->field_3_ypos = yCoord;
                field_8_pNode->field_4_zpos = cur_z;
                ++field_C_node_count;
                continue;
            case 66:
                if (field_C_node_count > 1)
                {
                    --field_8_pNode;
                }
                if (field_C_node_count > 100)
                {
                    field_C_node_count = 100;
                }

                for (i = 0; i < field_C_node_count; i++)
                {
                    PatrolPoint_3* pPatrolPoint = &pPed->field_0_patrol_points[i];
                    pPatrolPoint->field_0_x = field_8_pNode->field_2_xpos;
                    pPatrolPoint->field_1_y = field_8_pNode->field_3_ypos;
                    pPatrolPoint->field_2_z = field_8_pNode->field_4_zpos;
                    --field_8_pNode;
                }
                pPatrolPoint_2 = &pPed->field_0_patrol_points[i];
                pPatrolPoint_2->field_0_x = 0;
                pPatrolPoint_2->field_1_y = 0;
                pPatrolPoint_2->field_2_z = 0;
                *pOutPathFailCount = field_C_node_count;
                field_2FD0_bTimedOut = 1;
                field_38_bComputePathInProgress = 0;
                field_2FD1_time_out_counter = 0;
                return 1;
            default:
                // Shared failure block: the default case and the "no more nodes" path both end here.
                goto LABEL_61;
        }
    }
LABEL_61:
    PathFinder_2FD4::RemovePed_554620(field_0_ped_id);
    return 1;
}

// https://decomp.me/scratch/19LIh
MATCH_FUNC(0x5552b0)
bool PathFinder_2FD4::FindNearbyTileMatchingSlopeType_5552B0(u8 block_type, u8* xpos, u8* ypos, u8* zpos, char_type maybe_timer)
{
    u8 j = 0;
    if (field_38_bComputePathInProgress && !maybe_timer)
    {
        return 0;
    }
    if (*zpos > 6u || *zpos == 0)
    {
        return 0;
    }
    u8 default_pos = kFpZero_6FDD98.ToUInt8();
    field_27_zpos = default_pos;
    field_26_ypos = default_pos;
    field_25_xpos = default_pos;
    field_28 = *xpos - 16;
    field_29 = *ypos - 16;
    PathFinder_2FD4::ClearGrid_5545C0();
    field_E_xStart = *xpos;
    field_F_yStart = *ypos;
    field_10_zStart = *zpos;
    field_4_bFindTileMode = 1;

    if (!gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_E_xStart, field_F_yStart, field_10_zStart))
    {
        field_10_zStart = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(Fix16(field_E_xStart), Fix16(field_F_yStart), Fix16(field_10_zStart)).ToUInt8();
    }
    field_16 = 0;
    field_11_xEnd = 0;
    field_12_yEnd = 0;
    field_13_zEnd = 0;
    gPathFinder_SlopeZDelta_6FDEEC = 0;
    field_C_node_count = 0;
    field_8_pNode = field_2350_nodes;
    field_22_zpos = field_10_zStart;
    field_24_grid_y = 16;
    field_23_grid_x = 16;
    field_20_xpos = field_E_xStart;
    field_21_ypos = field_F_yStart;
    field_1B_direction = 66;
    PathFinder_2FD4::AddGridCell_554710();
    field_8_pNode = field_2350_nodes;
    field_1C_grid_idx = 560;
    field_40_grid[field_1C_grid_idx].field_6_cost = field_16;
    field_1E_current_cost = 0;
    field_18 = 1;
    bool bListWasEmpty = true;
    while (field_C_node_count)
    {
        bListWasEmpty = false;
        PathNode_8* pIter = field_2350_nodes;
        field_8_pNode = field_2350_nodes;

        for (u8 i = 0; i < field_C_node_count - 1; i++)
        {
            ++field_8_pNode;
            if (field_8_pNode->field_6_cost < pIter->field_6_cost)
            {
                pIter = field_8_pNode;
            }
        }
        gPathFinder_best_grid_x_6FDEC8 = pIter->field_0_idx1;
        gPathFinder_best_grid_y_6FDEC9 = pIter->field_1_idx2;
        gPathFinder_best_x_6FDECA = pIter->field_2_xpos;
        gPathFinder_best_y_6FDECB = pIter->field_3_ypos;
        gPathFinder_best_z_6FDECC = pIter->field_4_zpos;
        gPathFinder_best_cost_6FDECE = pIter->field_6_cost;
        field_1C_grid_idx = gPathFinder_best_grid_x_6FDEC8 + 34 * gPathFinder_best_grid_y_6FDEC9;
        field_1E_current_cost = field_40_grid[field_1C_grid_idx].field_6_cost;
        pIter->field_0_idx1 = field_8_pNode->field_0_idx1;
        pIter->field_1_idx2 = field_8_pNode->field_1_idx2;
        pIter->field_2_xpos = field_8_pNode->field_2_xpos;
        pIter->field_3_ypos = field_8_pNode->field_3_ypos;
        pIter->field_4_zpos = field_8_pNode->field_4_zpos;
        pIter->field_6_cost = field_8_pNode->field_6_cost;
        --field_C_node_count;
        field_25_xpos = gPathFinder_best_x_6FDECA;
        field_26_ypos = gPathFinder_best_y_6FDECB;
        field_27_zpos = gPathFinder_best_z_6FDECC;

        u8 block_below_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(field_25_xpos, field_26_ypos, field_27_zpos - 1);

        if (block_type == 5)
        {
            if (!gMap_0x370_6F6268->sub_4E5640(kFpOne64th_6FDD50,
                                               kFpQuarter_6FDC00,
                                               kFpOne64th_6FDD50,
                                               Fix16(*xpos) + kFpHalf_6FDCA8,
                                               Fix16(*ypos) + kFpHalf_6FDCA8,
                                               Fix16(*zpos),
                                               Fix16(field_25_xpos) + kFpHalf_6FDCA8,
                                               Fix16(field_26_ypos) + kFpHalf_6FDCA8,
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
        field_20_xpos = gPathFinder_best_x_6FDECA;
        field_21_ypos = gPathFinder_best_y_6FDECB - 1;
        field_22_zpos = gPathFinder_best_z_6FDECC;
        field_23_grid_x = gPathFinder_best_grid_x_6FDEC8;
        field_24_grid_y = gPathFinder_best_grid_y_6FDEC9 - 1;
        field_1B_direction = 1;
        if (!PathFinder_2FD4::ProcessGridCell_5548C0())
        {
            field_20_xpos = gPathFinder_best_x_6FDECA + 1;
            field_21_ypos = gPathFinder_best_y_6FDECB;
            field_22_zpos = gPathFinder_best_z_6FDECC;
            field_23_grid_x = gPathFinder_best_grid_x_6FDEC8 + 1;
            field_24_grid_y = gPathFinder_best_grid_y_6FDEC9;
            field_1B_direction = 2;
            if (!PathFinder_2FD4::ProcessGridCell_5548C0())
            {
                field_20_xpos = gPathFinder_best_x_6FDECA;
                field_21_ypos = gPathFinder_best_y_6FDECB + 1;
                field_22_zpos = gPathFinder_best_z_6FDECC;
                field_23_grid_x = gPathFinder_best_grid_x_6FDEC8;
                field_24_grid_y = gPathFinder_best_grid_y_6FDEC9 + 1;
                field_1B_direction = 3;
                if (!PathFinder_2FD4::ProcessGridCell_5548C0())
                {
                    field_20_xpos = gPathFinder_best_x_6FDECA - 1;
                    field_21_ypos = gPathFinder_best_y_6FDECB;
                    field_22_zpos = gPathFinder_best_z_6FDECC;
                    field_23_grid_x = gPathFinder_best_grid_x_6FDEC8 - 1;
                    field_24_grid_y = gPathFinder_best_grid_y_6FDEC9;
                    field_1B_direction = 4;
                    if (!PathFinder_2FD4::ProcessGridCell_5548C0())
                    {
                        ++field_16;
                        if (++j <= 6 || maybe_timer != 0)
                        {
                            if (!field_18)
                            {
                                return 1;
                            }
                            continue;
                        }
                    }
                }
            }
        }
        return 0;
    }
    if (bListWasEmpty)
    {
        return 1;
    }
    return 0;
}
