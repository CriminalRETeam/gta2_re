#include "RouteFinder.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "file.hpp"
#include "map_0x370.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Hud.hpp"
#include "Frontend.hpp"
#include <cstdio>

// Forward declarations: the functions below are in address order
static inline void WorldToScreen_RouteFinder_40CFC0(Camera* pCam, Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY);

DEFINE_GLOBAL(RouteFinder*, gRouteFinder_6FFDC8, 0x6FFDC8);
DEFINE_GLOBAL(u16, gLastRouteLength_6FFDCC, 0x6ffdcc);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FFC7C, Fix16(1), 0x6FFC7C);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_6FFC9C, Fix16(8), 0x6FFC9C);
EXTERN_GLOBAL(s16, word_703BAA);

MATCH_FUNC(0x588580)
bool Junction::IsUsableWithArrowType_588580(s32 arrow_type) const
{
    if (arrow_type == 2)
    {
        if (this->field_8_type != 1)
        {
            return true;
        }
    }
    else if (arrow_type == 1)
    {
        if (this->field_8_type != 2)
        {
            return true;
        }
    }
    else if (arrow_type == 3)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x5885c0)
u16 Junction::GetDirectionToJunction_5885C0(u16 junction_idx) const
{
    if (junction_idx != 0)
    {
        if (field_0_n.GetIndex_0040CE90() == junction_idx)
        {
            return 1;
        }
        if (field_2_s.GetIndex_0040CE90() == junction_idx)
        {
            return 2;
        }
        if (field_4_e.GetIndex_0040CE90() != junction_idx)
        {
            return 3;
        }
        else
        {
            return 4;
        }
    }
    else
    {
        return gRouteFinder_6FFDC8->field_2;
    }
}

MATCH_FUNC(0x588620)
void RouteFinder::ShowJunctionIds_588620()
{
    Junction* pJunction = &field_8_junctions[1];
    for (u16 i = 1; i < GTA2_COUNTOF(field_8_junctions); i++, pJunction++)
    {
        if (!pJunction->field_C_min_x)
        {
            break;
        }

        if (gGame_0x40_67E008->field_38_local_player->field_14C_view_camera.IsPointInBoundaries_58CF10(pJunction->field_C_min_x,
                                                                                              static_cast<s32>(pJunction->field_D_min_y)))
        {
            Fix16 screen_x;
            Fix16 screen_y;
            WorldToScreen_RouteFinder_40CFC0(&gGame_0x40_67E008->field_38_local_player->field_14C_view_camera,
                                             Fix16(pJunction->field_C_min_x),
                                             Fix16(pJunction->field_D_min_y),
                                             gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(Fix16(pJunction->field_C_min_x),
                                                                                           Fix16(pJunction->field_D_min_y)),
                                             &screen_x,
                                             &screen_y);

            swprintf(tmpBuff_67BD9C, L"%d", i);
            gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, screen_x.ToInt(), screen_y.ToInt(), word_703BAA, 1);
        }
    }
}

// 9.6f 0x40CFC0: Camera::WorldToScreen_40CFC0 (x/y/z by value, writes through two out pointers), but
// the original reads this file's copies of the constants (0x6FFC7C, 0x6FFC9C), so the Camera.hpp inline can't
// be used here
static inline void WorldToScreen_RouteFinder_40CFC0(Camera* pCam, Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY)
{
    Fix16 u = pCam->field_98_cur_pos.field_8_z - z;
    Fix16 t(kFpOne_6FFC7C / Fix16(u.mValue + kFpEight_6FFC9C.mValue, 0));
    *pOutX = (((x - pCam->field_98_cur_pos.field_0_x) * pCam->field_60_screen_scale.y) * t) + Fix16(320);
    *pOutY = (((y - pCam->field_98_cur_pos.field_4_y) * pCam->field_60_screen_scale.y) * t) + Fix16(240);
}

MATCH_FUNC(0x588810)
void RouteFinder::RoadOff_588810(u8 x, u8 y, u8 z)
{
    const u16 r1 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, 0, green_or_red_3);
    const u16 r2 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, 1, green_or_red_3);
    if (r1 && r2)
    {
        Junction* j1 = &field_8_junctions[r1];
        Junction* j2 = &field_8_junctions[r2];
        if ((j1->field_4_e.GetIndex_0040CE90()) == r2)
        {
            j1->field_4_e.Disable_40CEC0();
            j2->field_6_w.Disable_40CEC0();
        }
        else if ((j1->field_6_w.GetIndex_0040CE90()) == r2)
        {
            j1->field_6_w.Disable_40CEC0();
            j2->field_4_e.Disable_40CEC0();
        }
    }
    else
    {
        const u16 r3 = RouteFinder::FindVertSegmentJunction_588F30(x, y, z, 0, green_or_red_3);
        const u16 r4 = RouteFinder::FindVertSegmentJunction_588F30(x, y, z, 1, green_or_red_3);
        if (r3 && r4)
        {
            Junction* j1 = &field_8_junctions[r3];
            Junction* j2 = &field_8_junctions[r4];

            if ((j1->field_2_s.GetIndex_0040CE90()) == r4)
            {
                j1->field_2_s.Disable_40CEC0();
                j2->field_0_n.Disable_40CEC0();
            }
            else if ((j1->field_0_n.GetIndex_0040CE90()) == r4)
            {
                j1->field_0_n.Disable_40CEC0();
                j2->field_2_s.Disable_40CEC0();
            }
        }
    }
}

MATCH_FUNC(0x588950)
void RouteFinder::RoadOn_588950(u8 x, u8 y, u8 z)
{
    // Strangely not the exact inverse logic of RoadOff
    const u16 r1 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, 0, green_or_red_3);
    const u16 r2 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, 1, green_or_red_3);
    if (r1 && r2)
    {
        Junction* j1 = &field_8_junctions[r1];
        Junction* j2 = &field_8_junctions[r2];
        if ((j1->field_4_e.GetIndex_0040CE90()) == r2)
        {
            j1->field_4_e.Enable_40CEB0();
            j2->field_6_w.Enable_40CEB0();
        }
        else if ((j1->field_6_w.GetIndex_0040CE90()) == r2)
        {
            j1->field_6_w.Enable_40CEB0();
            j2->field_4_e.Enable_40CEB0();
        }
    }

    const u16 r3 = RouteFinder::FindVertSegmentJunction_588F30(x, y, z, 0, green_or_red_3);
    const u16 r4 = RouteFinder::FindVertSegmentJunction_588F30(x, y, z, 1, green_or_red_3);
    if (r3 && r4)
    {
        Junction* j1 = &field_8_junctions[r3];
        Junction* j2 = &field_8_junctions[r4];

        if ((j1->field_2_s.GetIndex_0040CE90()) == r4)
        {
            j1->field_2_s.Enable_40CEB0();
        }
        else if ((j1->field_0_n.GetIndex_0040CE90()) == r4)
        {
            j1->field_0_n.Enable_40CEB0();
        }

        if ((j2->field_2_s.GetIndex_0040CE90()) == r3)
        {
            j2->field_2_s.Enable_40CEB0();
        }
        else if ((j2->field_0_n.GetIndex_0040CE90()) == r3)
        {
            j2->field_0_n.Enable_40CEB0();
        }
    }
}

MATCH_FUNC(0x588aa0)
u16 RouteFinder::IsPointInJunctionBounds_588AA0(u8 x, u8 y, u16 junc_idx1, u16 junc_idx2)
{
    u8 x1;
    u8 y1;
    u8 y2;
    u8 x2;

    Junction* pJ1 = &this->field_8_junctions[junc_idx1];
    Junction* pJ2 = &this->field_8_junctions[junc_idx2];

    if (pJ1->field_C_min_x <= pJ2->field_C_min_x)
    {
        x1 = pJ1->field_C_min_x;
    }
    else
    {
        x1 = pJ2->field_C_min_x;
    }

    if (pJ1->field_E_max_x > pJ2->field_E_max_x)
    {
        x2 = pJ1->field_E_max_x;
    }
    else
    {
        x2 = pJ2->field_E_max_x;
    }

    if (pJ1->field_D_min_y <= pJ2->field_D_min_y)
    {
        y1 = pJ1->field_D_min_y;
    }
    else
    {
        y1 = pJ2->field_D_min_y;
    }

    if (pJ1->field_F_max_y > pJ2->field_F_max_y)
    {
        y2 = pJ1->field_F_max_y;
    }
    else
    {
        y2 = pJ2->field_F_max_y;
    }

    if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x588b30)
void RouteFinder::Load_RGEN_588B30()
{
    File::Global_Read_4A71C0(field_8_junctions, 0x2210);
    File::Global_Read_4A71C0(this->field_A830_horz_segments, 0x1108);
    File::Global_Read_4A71C0(this->field_B938_vert_segments, 0x1108);
    File::Global_Read_4A71C0(&this->field_4_unused, 2);
    File::Global_Read_4A71C0(&this->field_CC62_horz_segment_count, 2);
    File::Global_Read_4A71C0(&this->field_CC64_vert_segment_count, 2);
    this->field_0_route_count = 0;

    if (bLog_routefinder_67D6D1)
    {
        s32 junction_idx = 0;
        do
        {
            sprintf(gTmpBuffer_67C598,
                    "Junc: %d (%d, %d) n %d s %d w %d e %d",
                    junction_idx,
                    field_8_junctions[junction_idx].field_C_min_x,
                    field_8_junctions[junction_idx].field_D_min_y,
                    field_8_junctions[junction_idx].field_0_n.GetIndex_0040CE90(),
                    field_8_junctions[junction_idx].field_2_s.GetIndex_0040CE90(),
                    field_8_junctions[junction_idx].field_6_w.GetIndex_0040CE90(),
                    field_8_junctions[junction_idx].field_4_e.GetIndex_0040CE90());
            gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);

            if (junction_idx > 0 && field_8_junctions[junction_idx].field_C_min_x == 0 && field_8_junctions[junction_idx].field_D_min_y == 0)
            {
                break;
            }
            junction_idx++;
        } while (junction_idx < 0x221);

        gErrorLog_67C530.Write_4D9620("     ");
    }
}

MATCH_FUNC(0x588c60)
void RouteFinder::Reset_588C60()
{
    memset(this->field_CA40_visited, 0, sizeof(this->field_CA40_visited));
    memset(this->field_861C_nodes, 0, sizeof(this->field_861C_nodes));
    memset(this->field_2218_routes, 0, sizeof(this->field_2218_routes));
}

MATCH_FUNC(0x588ca0)
bool RouteFinder::HasBlockDesiredArrow_588CA0(const gmp_block_info* block, s32 arrow_type, u8 direction) const
{
    switch (direction)
    {
        case UP_1:

            switch (arrow_type)
            {
                case green_1:
                    if ((block->field_A_arrows & block_arrow::green_up_4) != 0)
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->field_A_arrows & block_arrow::red_up_40) != 0)
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->field_A_arrows & (block_arrow::green_up_4 | block_arrow::red_up_40)) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case DOWN_2:
            switch (arrow_type)
            {
                case green_1:
                    if ((block->field_A_arrows & block_arrow::green_down_8) != 0)
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->field_A_arrows & block_arrow::red_down_80) != 0)
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->field_A_arrows & (block_arrow::green_down_8 | block_arrow::red_down_80)) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case LEFT_3:
            switch (arrow_type)
            {
                case green_1:
                    if ((block->field_A_arrows & block_arrow::green_left_1) != 0)
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->field_A_arrows & block_arrow::red_left_10) != 0)
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->field_A_arrows & block_arrow::left_mask) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case RIGHT_4:
            switch (arrow_type)
            {
                case green_1:
                    if ((block->field_A_arrows & block_arrow::green_right_2) != 0)
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->field_A_arrows & block_arrow::red_right_20) != 0)
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->field_A_arrows & block_arrow::right_mask) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;
    }
    return false;
}

MATCH_FUNC(0x588de0)
bool RouteFinder::HasBlockArrowInRoadDirection_588DE0(const gmp_block_info* pBlock, s32 arrow_type, s32 road_direction) const
{

    bool result = false;
    switch (road_direction)
    {
        case road_direction::up_1:
            result = HasBlockDesiredArrow_588CA0(pBlock, arrow_type, UP_1);
            break;
        case road_direction::down_2:
            result = HasBlockDesiredArrow_588CA0(pBlock, arrow_type, DOWN_2);
            break;
        case road_direction::left_4:
            result = HasBlockDesiredArrow_588CA0(pBlock, arrow_type, LEFT_3);
            break;
        case road_direction::right_3:
            result = HasBlockDesiredArrow_588CA0(pBlock, arrow_type, RIGHT_4);
            break;
    }
    return result;
}

MATCH_FUNC(0x588e60)
u16 RouteFinder::FindHorzSegmentJunction_588E60(u8 x, u8 y, u8 z, char_type bIsDest, s32 arrow_type)
{
    JunctionSegment* pSegment = field_A830_horz_segments;
    gmp_block_info* pBlockAtPos = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z);

    if (pBlockAtPos != NULL)
    {
        s8 bHasArrow = RouteFinder::HasBlockDesiredArrow_588CA0(pBlockAtPos, arrow_type, LEFT_3);
        for (s16 junc_idx = 0; junc_idx < field_CC62_horz_segment_count; pSegment++, junc_idx++)
        {
            if (pSegment->ContainsPoint_40CF20(x, y))
            {
                if (bIsDest == 1)
                {
                    if (bHasArrow == 1)
                    {
                        return pSegment->field_0_junction_num1;
                    }
                    else
                    {
                        return pSegment->field_2_junction_num2;
                    }
                }
                else
                {
                    if (bHasArrow == 1)
                    {
                        return pSegment->field_2_junction_num2;
                    }
                    else
                    {
                        return pSegment->field_0_junction_num1;
                    }
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x588f30)
u16 RouteFinder::FindVertSegmentJunction_588F30(u8 x, u8 y, u8 z, char_type bIsDest, s32 arrow_type)
{
    JunctionSegment* pSegment = field_B938_vert_segments;
    gmp_block_info* pBlockAtPos = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z);

    if (pBlockAtPos != NULL)
    {
        s8 bHasArrow = RouteFinder::HasBlockDesiredArrow_588CA0(pBlockAtPos, arrow_type, UP_1);
        for (s16 junc_idx = 0; junc_idx < field_CC64_vert_segment_count; pSegment++, junc_idx++)
        {
            if (pSegment->ContainsPoint_40CF20(x, y))
            {
                if (bIsDest == 1)
                {
                    if (bHasArrow == 1)
                    {
                        return pSegment->field_0_junction_num1;
                    }
                    else
                    {
                        return pSegment->field_2_junction_num2;
                    }
                }
                else
                {
                    if (bHasArrow == 1)
                    {
                        return pSegment->field_2_junction_num2;
                    }
                    else
                    {
                        return pSegment->field_0_junction_num1;
                    }
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x589000)
u16 RouteFinder::FindSegmentJunction_589000(u8 x_coord, u8 y_coord, u8 z_coord, char_type bIsDest, s32 arrow_type)
{
    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(x_coord, y_coord, z_coord);

    if (pBlock)
    {
        if (RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, UP_1) || RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, DOWN_2))
        {
            return RouteFinder::FindVertSegmentJunction_588F30(x_coord, y_coord, z_coord, bIsDest, arrow_type);
        }
        if (RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, LEFT_3) || RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, RIGHT_4))
        {
            return RouteFinder::FindHorzSegmentJunction_588E60(x_coord, y_coord, z_coord, bIsDest, arrow_type);
        }
    }
    return 0;
}

MATCH_FUNC(0x5890d0)
void RouteFinder::FindArrowBlockInJunction_5890D0(u16 junction_idx, s32 direction, u8* xpos, u8* ypos)
{
    for (u8 y = field_8_junctions[junction_idx].field_D_min_y; y <= field_8_junctions[junction_idx].field_F_max_y; y++)
    {
        for (u8 x = field_8_junctions[junction_idx].field_C_min_x; x <= field_8_junctions[junction_idx].field_E_max_x; x++)
        {
            s32 z;
            gmp_block_info* block = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x, y, &z);
            if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(direction, block))
            {
                *xpos = x;
                *ypos = y;
                return;
            }
        }
    }

    for (u8 y_pos = field_8_junctions[junction_idx].field_D_min_y; y_pos <= field_8_junctions[junction_idx].field_F_max_y; y_pos++)
    {
        for (u8 x_pos = field_8_junctions[junction_idx].field_C_min_x; x_pos <= field_8_junctions[junction_idx].field_E_max_x; x_pos++)
        {
            s32 z;
            gmp_block_info* block = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x_pos, y_pos, &z);
            if (gMap_0x370_6F6268->GetArrowDirectionFromBlock_4E5FC0(block, false))
            {
                *xpos = x_pos;
                *ypos = y_pos;
                return;
            }
        }
    }
}

MATCH_FUNC(0x589210)
s32 RouteFinder::NoRefs_589210(u8 x, u8 y, s32 a4, u8 direction, s32 a6, u16 junction_idx)
{
    Junction* pJunction = &field_8_junctions[junction_idx];
    s32 dy;
    s16 dx;
    switch (direction)
    {
        case 1:
            dx = 0;
            dy = -1;
            break;
        case 2:
            dx = 0;
            dy = 1;
            break;
        case 8:
            dx = 1;
            dy = 0;
            break;
        case 4:
            dx = -1;
            dy = 0;
            break;
    }

    s32 result = 0;
    if (pJunction->ContainsPoint(static_cast<u8>(x + static_cast<s16>(dx)), static_cast<u8>(y + dy)))
    {
        result = 1;
    }
    return result;
}

// dx/dy are uninitialised for a direction that isn't 1, 2, 4 or 8, as in the original
#pragma warning(push)
#pragma warning(disable : 4701)
MATCH_FUNC(0x5892d0)
RouteSearchNode::RouteSearchNode()
{
    field_0_idx = 0;
    field_2_cost = -1;
    field_4_expanded = 0;
    field_8_pParent = NULL;
    field_C_pNext = NULL;
}
#pragma warning(pop)

MATCH_FUNC(0x5892f0)
RouteSearchNode* RouteFinder::NewChildNode_5892F0(RouteSearchNode* pParent, u16 idx, s16 step_cost)
{
    RouteSearchNode* pNode = &this->field_861C_nodes[this->field_CC66_545_count++];

    s32 dy = abs(static_cast<u8>(field_8_junctions[idx].field_D_min_y) - static_cast<u8>(field_8_junctions[field_861A_dest_idx].field_D_min_y));
    s32 dx = abs(static_cast<u8>(field_8_junctions[idx].field_C_min_x) - static_cast<u8>(field_8_junctions[field_861A_dest_idx].field_C_min_x));
    s16 dist = dy + dx;
    s16 cost = pParent->field_2_cost + step_cost;

    pNode->field_0_idx = idx;
    pNode->field_2_cost = cost + dist;
    pNode->field_8_pParent = pParent;
    pNode->field_C_pNext = NULL;

    return pNode;
}

MATCH_FUNC(0x589390)
RouteSearchNode* RouteFinder::NewStartNode_589390(u16 junction_idx)
{
    RouteSearchNode* pNode = &field_861C_nodes[field_CC66_545_count++];
    s32 distance = abs(field_8_junctions[junction_idx].field_C_min_x - field_8_junctions[field_861A_dest_idx].field_C_min_x) +
        abs(field_8_junctions[junction_idx].field_D_min_y - field_8_junctions[field_861A_dest_idx].field_D_min_y);

    pNode->field_2_cost = distance;
    pNode->field_0_idx = junction_idx;
    pNode->field_4_expanded = 0;
    // field_6 is preserved by the original function.
    pNode->field_8_pParent = NULL;
    pNode->field_C_pNext = NULL;
    return pNode;
}

MATCH_FUNC(0x589420)
void RouteFinder::InsertIntoOpenList_589420(RouteSearchNode* pNode)
{
    field_CA40_visited[pNode->field_0_idx] = true;

    if (pNode->field_2_cost < field_A82C_open_list->field_2_cost)
    {
        pNode->field_C_pNext = field_A82C_open_list;
        field_A82C_open_list = pNode;
    }
    else
    {
        RouteSearchNode* pInsertAfter;
        for (pInsertAfter = field_A82C_open_list; pInsertAfter->field_C_pNext != NULL && pInsertAfter->field_C_pNext->field_2_cost < pNode->field_2_cost; pInsertAfter = pInsertAfter->field_C_pNext)
        {
        }
        pNode->field_C_pNext = pInsertAfter->field_C_pNext;
        pInsertAfter->field_C_pNext = pNode;
    }
}

MATCH_FUNC(0x589480)
bool RouteFinder::InitSearch_589480(u8 start_x, u8 start_y, u8 start_z, u8 dest_x, u8 dest_y, u8 dest_z, s32 arrow_type)
{
    field_CC66_545_count = 0;
    memset(field_CA40_visited, 0, sizeof(field_CA40_visited));
    memset(field_861C_nodes, 0, sizeof(field_861C_nodes));

    field_8618_idx = FindSegmentJunction_589000(start_x, start_y, start_z, 0, arrow_type);
    if (field_8618_idx == 0)
    {
        field_8618_idx = FindSegmentJunction_589000(start_x, start_y, start_z, 1, arrow_type);
    }

    u16 initialIdx = field_8618_idx;
    Junction* pJunction = &field_8_junctions[initialIdx];
    if (start_x < pJunction->field_C_min_x || start_x > pJunction->field_E_max_x || start_y < pJunction->field_D_min_y || start_y > pJunction->field_F_max_y)
    {
        field_8618_idx = FindSegmentJunction_589000(start_x, start_y, start_z, 1, arrow_type);
    }

    field_861A_dest_idx = FindSegmentJunction_589000(dest_x, dest_y, dest_z, 1, arrow_type);
    if (field_861A_dest_idx == field_8618_idx)
    {
        field_8618_idx = initialIdx;
    }

    if (field_8618_idx != 0 && field_861A_dest_idx != 0)
    {
        NewStartNode_589390(field_8618_idx);
        field_CA40_visited[0] = true;
        field_A82C_open_list = field_861C_nodes;
        field_CA40_visited[field_8618_idx] = true;
        return true;
    }
    return false;
}

MATCH_FUNC(0x5895c0)
bool RouteFinder::InitSearchWithDirection_5895C0(u8 x, u8 y, u8 z, s32 arrow_type, s32 direction)
{
    field_CC66_545_count = 0;
    memset(field_CA40_visited, 0, sizeof(field_CA40_visited));
    RouteSearchNode* pStart = field_861C_nodes;
    memset(pStart, 0, sizeof(field_861C_nodes));

    field_8618_idx = FindSegmentJunction_589000(x, y, z, 0, arrow_type);
    if (field_8618_idx == 0)
    {
        field_8618_idx = FindSegmentJunction_589000(x, y, z, 1, arrow_type);
    }

    if (field_8618_idx != 0)
    {
        Junction* pJunction = &field_8_junctions[field_8618_idx];
        if (x < pJunction->field_C_min_x || x > pJunction->field_E_max_x || y < pJunction->field_D_min_y ||
            y > pJunction->field_F_max_y)
        {
            field_8618_idx = FindSegmentJunction_589000(x, y, z, 1, arrow_type);
        }

        if (field_8618_idx != 0)
        {
            NewStartNode_589390(field_8618_idx);
            field_A82C_open_list = pStart;
            field_CA40_visited[0] = true;
            field_CA40_visited[field_8618_idx] = true;

            pJunction = &field_8_junctions[field_8618_idx];
            u16 north = pJunction->field_0_n.GetIndex_0040CE90();
            u16 south = pJunction->field_2_s.GetIndex_0040CE90();
            u16 east = pJunction->field_4_e.GetIndex_0040CE90();
            u16 west = pJunction->field_6_w.GetIndex_0040CE90();
            pStart->field_4_expanded = 1;

            switch (direction)
            {
                case 1:
                    if (!pJunction->field_2_s.GetIndex_0040CE90())
                    {
                        if (east)
                        {
                            AddChildNode_589990(pStart, east, pJunction->field_4_e.GetLength());
                        }
                        else if (west)
                        {
                            AddChildNode_589990(pStart, west, pJunction->field_6_w.GetLength());
                        }
                        else
                        {
                            AddChildNode_589990(pStart, north, pJunction->field_0_n.GetLength());
                        }
                    }
                    else
                    {
                        AddChildNode_589990(pStart, pJunction->field_2_s.GetIndex_0040CE90(), pJunction->field_2_s.GetLength());
                    }
                    break;
                case 2:
                    if (!pJunction->field_0_n.GetIndex_0040CE90())
                    {
                        if (east)
                        {
                            AddChildNode_589990(pStart, east, pJunction->field_4_e.GetLength());
                        }
                        else if (west)
                        {
                            AddChildNode_589990(pStart, west, pJunction->field_6_w.GetLength());
                        }
                        else
                        {
                            AddChildNode_589990(pStart, south, pJunction->field_2_s.GetLength());
                        }
                    }
                    else
                    {
                        AddChildNode_589990(pStart, pJunction->field_0_n.GetIndex_0040CE90(), pJunction->field_0_n.GetLength());
                    }
                    break;
                case 3:
                    if (!pJunction->field_6_w.GetIndex_0040CE90())
                    {
                        if (south)
                        {
                            AddChildNode_589990(pStart, south, pJunction->field_2_s.GetLength());
                        }
                        else if (north)
                        {
                            AddChildNode_589990(pStart, north, pJunction->field_0_n.GetLength());
                        }
                        else
                        {
                            AddChildNode_589990(pStart, east, pJunction->field_4_e.GetLength());
                        }
                    }
                    else
                    {
                        AddChildNode_589990(pStart, pJunction->field_6_w.GetIndex_0040CE90(), pJunction->field_6_w.GetLength());
                    }
                    break;
                case 4:
                    if (!pJunction->field_4_e.GetIndex_0040CE90())
                    {
                        if (south)
                        {
                            AddChildNode_589990(pStart, south, pJunction->field_2_s.GetLength());
                        }
                        else if (north)
                        {
                            AddChildNode_589990(pStart, north, pJunction->field_0_n.GetLength());
                        }
                        else
                        {
                            AddChildNode_589990(pStart, west, pJunction->field_6_w.GetLength());
                        }
                    }
                    else
                    {
                        AddChildNode_589990(pStart, pJunction->field_4_e.GetIndex_0040CE90(), pJunction->field_4_e.GetLength());
                    }
                    break;
            }
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x589930)
void RouteFinder::CancelRoute_589930(s16 route_idx)
{
    this->field_2218_routes[route_idx].field_0_junctions[0] = 0;
    if (this->field_0_route_count > 0)
    {
        --this->field_0_route_count;
    }
}

MATCH_FUNC(0x589960)
s16 RouteFinder::GetFreeRouteIdx_589960()
{
    s16 route_idx = 1;
    if (this->field_0_route_count < 50)
    {
        while (route_idx < 50)
        {
            if (field_2218_routes[route_idx++].field_0_junctions[0] == 0)
            {
                return route_idx - 1;
            }
        }
    }
    return -1;
}

MATCH_FUNC(0x589990)
u16 RouteFinder::AddChildNode_589990(RouteSearchNode* pParent, u16 junction_idx, s16 step_cost)
{
    RouteSearchNode* pChild = NewChildNode_5892F0(pParent, junction_idx, step_cost);
    InsertIntoOpenList_589420(pChild);
    return pChild->field_0_idx;
}

// Returns true if a neighbour of the node's junction that hasn't been visited yet completes the route
MATCH_FUNC(0x5899c0)
bool RouteFinder::ExpandNode_5899C0(RouteSearchNode* pNode, s32 arrow_type)
{
    Junction* pJunction = &field_8_junctions[pNode->field_0_idx];
    u16 north = pJunction->field_0_n.GetIndex_0040CE90();
    u16 west = pJunction->field_6_w.GetIndex_0040CE90();
    u16 south = pJunction->field_2_s.GetIndex_0040CE90();
    u16 east = pJunction->field_4_e.GetIndex_0040CE90();
    pNode->field_4_expanded = 1;

    if (!field_CA40_visited[north] && pJunction->field_0_n.IsEnabled() && field_8_junctions[north].IsUsableWithArrowType_588580(arrow_type) &&
        AddChildNode_589990(pNode, north, pJunction->field_0_n.GetLength()) == field_861A_dest_idx)
    {
        return true;
    }
    if (!field_CA40_visited[south] && pJunction->field_2_s.IsEnabled() && field_8_junctions[south].IsUsableWithArrowType_588580(arrow_type) &&
        AddChildNode_589990(pNode, south, pJunction->field_2_s.GetLength()) == field_861A_dest_idx)
    {
        return true;
    }
    if (!field_CA40_visited[west] && pJunction->field_6_w.IsEnabled() && field_8_junctions[west].IsUsableWithArrowType_588580(arrow_type) &&
        AddChildNode_589990(pNode, west, pJunction->field_6_w.GetLength()) == field_861A_dest_idx)
    {
        return true;
    }
    if (!field_CA40_visited[east] && pJunction->field_4_e.IsEnabled() && field_8_junctions[east].IsUsableWithArrowType_588580(arrow_type) &&
        AddChildNode_589990(pNode, east, pJunction->field_4_e.GetLength()) == field_861A_dest_idx)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x589bb0)
bool RouteFinder::ExpandNodeUnlessOffScreen_589BB0(RouteSearchNode* pNode, s32 arrow_type)
{
    Junction* pJunction = &field_8_junctions[pNode->field_0_idx];
    if (!gGame_0x40_67E008->IsPointOnScreen_4B9A80(pJunction->field_C_min_x, pJunction->field_D_min_y) &&
        !gGame_0x40_67E008->IsPointOnScreen_4B9A80(pJunction->field_E_max_x, pJunction->field_D_min_y) &&
        !gGame_0x40_67E008->IsPointOnScreen_4B9A80(pJunction->field_C_min_x, pJunction->field_F_max_y) &&
        !gGame_0x40_67E008->IsPointOnScreen_4B9A80(pJunction->field_E_max_x, pJunction->field_F_max_y))
    {
        return true;
    }

    u16 north = pJunction->field_0_n.GetIndex_0040CE90();
    u16 south = pJunction->field_2_s.GetIndex_0040CE90();
    u16 west = pJunction->field_6_w.GetIndex_0040CE90();
    u16 east = pJunction->field_4_e.GetIndex_0040CE90();
    pNode->field_4_expanded = 1;

    if (!field_CA40_visited[north] && pJunction->field_0_n.IsEnabled() && field_8_junctions[north].IsUsableWithArrowType_588580(arrow_type))
    {
        AddChildNode_589990(pNode, north, pJunction->field_0_n.GetLength());
    }
    if (!field_CA40_visited[south] && pJunction->field_2_s.IsEnabled() && field_8_junctions[south].IsUsableWithArrowType_588580(arrow_type))
    {
        AddChildNode_589990(pNode, south, pJunction->field_2_s.GetLength());
    }
    if (!field_CA40_visited[west] && pJunction->field_6_w.IsEnabled() && field_8_junctions[west].IsUsableWithArrowType_588580(arrow_type))
    {
        AddChildNode_589990(pNode, west, pJunction->field_6_w.GetLength());
    }
    if (!field_CA40_visited[east] && pJunction->field_4_e.IsEnabled() && field_8_junctions[east].IsUsableWithArrowType_588580(arrow_type))
    {
        AddChildNode_589990(pNode, east, pJunction->field_4_e.GetLength());
    }
    return false;
}

MATCH_FUNC(0x589e00)
RouteSearchNode* RouteFinder::GetFirstUnexpandedNode_589E00()
{
    RouteSearchNode* pNode;

    for (pNode = field_A82C_open_list; pNode != NULL && pNode->field_4_expanded != 0; pNode = pNode->field_C_pNext)
        ;
    return pNode;
}

MATCH_FUNC(0x589e20)
bool RouteFinder::SearchToDest_589E20(s32 arrow_type)
{
    bool bRet = false;
    RouteSearchNode* pOpenList = this->field_A82C_open_list;
    if (pOpenList->field_0_idx == this->field_861A_dest_idx)
    {
        return true;
    }

    if (pOpenList)
    {
        // while (true) + break keeps the original's dead test of bRet on loop entry, while (!bRet) drops it
        while (true)
        {
            if (bRet)
            {
                break;
            }

            RouteSearchNode* pNode = GetFirstUnexpandedNode_589E00();
            if (!pNode)
            {
                break;
            }
            bRet = ExpandNode_5899C0(pNode, arrow_type);
        }
    }
    return bRet;
}

MATCH_FUNC(0x589e70)
bool RouteFinder::SearchUntilOffScreen_589E70(s32 arrow_type)
{
    bool result = false;
    if (this->field_A82C_open_list != NULL)
    {
        while (true)
        {
            if (result)
            {
                break;
            }

            RouteSearchNode* pNode = GetFirstUnexpandedNode_589E00();
            if (pNode == NULL)
            {
                break;
            }
            result = ExpandNodeUnlessOffScreen_589BB0(pNode, arrow_type);
        }
    }
    return result;
}

MATCH_FUNC(0x589eb0)
s16 RouteFinder::StoreRoute_589EB0()
{
    u16 count = 0;
    if (field_CC66_545_count == 0)
    {
        return -1;
    }

    RouteSearchNode* pNode = &field_861C_nodes[field_CC66_545_count - 1];
    s16 route_idx = GetFreeRouteIdx_589960();
    if (route_idx == -1)
    {
        return -1;
    }

    while (pNode != NULL)
    {
        pNode = pNode->field_8_pParent;
        count++;
    }
    gLastRouteLength_6FFDCC = count;

    pNode = &field_861C_nodes[field_CC66_545_count - 1];

    if (pNode != NULL)
    {
        do
        {
            field_2218_routes[route_idx].field_0_junctions[--count] = pNode->field_0_idx;
            pNode = pNode->field_8_pParent;
        } while (pNode != NULL);
    }
    field_2218_routes[route_idx].field_0_junctions[gLastRouteLength_6FFDCC] = 0;
    field_0_route_count++;
    return route_idx;
}

MATCH_FUNC(0x589f70)
s16 RouteFinder::StoreRouteReversed_589F70()
{
    s16 length = 0;
    u8 out_idx = 0;
    if (field_CC66_545_count == 0)
    {
        return -1;
    }

    RouteSearchNode* pNode = &field_861C_nodes[field_CC66_545_count - 1];
    s16 route_idx = GetFreeRouteIdx_589960();
    if (route_idx == -1)
    {
        return -1;
    }

    while (pNode != NULL)
    {
        pNode = pNode->field_8_pParent;
        length++;
    }
    gLastRouteLength_6FFDCC = length;

    pNode = &field_861C_nodes[field_CC66_545_count - 1];

    if (pNode != NULL)
    {
        do
        {
            field_2218_routes[route_idx].field_0_junctions[out_idx] = pNode->field_0_idx;
            pNode = pNode->field_8_pParent;
            out_idx++;

        } while (pNode != NULL);
    }
    field_0_route_count++;
    return route_idx;
}

MATCH_FUNC(0x58a020)
void RouteFinder::DebugPrintRoute_58A020(char_type route_idx)
{
    s32 route_pos = 0;
    u16 i = this->field_2218_routes[route_idx].field_0_junctions[static_cast<u16>(route_pos)];
    while (i)
    {
        Junction* pJunc = &this->field_8_junctions[i];
        sprintf(gTmpBuffer_67C598,
                "Junc: %d : (%d, %d)(%d, %d)",
                i,
                pJunc->field_C_min_x,
                pJunc->field_D_min_y,
                pJunc->field_E_max_x,
                pJunc->field_F_max_y);
        gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);
        ++route_pos;
        i = this->field_2218_routes[route_idx].field_0_junctions[static_cast<u16>(route_pos)];
    }
}

MATCH_FUNC(0x58a0b0)
Junction* RouteFinder::GetJunction_58A0B0(u16 junction_idx)
{
    return &field_8_junctions[junction_idx];
}

MATCH_FUNC(0x58a0d0)
s16 RouteFinder::DoStartRoute_58A0D0(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type)
{
    if (InitSearch_589480(x1, y1, z1, x2, y2, z2, arrow_type) && SearchToDest_589E20(arrow_type))
    {
        return StoreRoute_589EB0();
    }
    else
    {
        return -1;
    }
}

MATCH_FUNC(0x58a130)
s16 RouteFinder::FindOffScreenRoute_58A130(u8 x, s16 y, u8 z, u8* pRouteLength, s32 arrow_type, s32 direction)
{
    if (InitSearchWithDirection_5895C0(x, static_cast<u8>(y), z, arrow_type, direction))
    {
        if (SearchUntilOffScreen_589E70(arrow_type))
        {
            s16 ret = StoreRouteReversed_589F70();
            *pRouteLength = static_cast<u8>(gLastRouteLength_6FFDCC);
            return ret;
        }
    }
    return -1;
}

MATCH_FUNC(0x58a190)
u16 RouteFinder::StartRoute_58A190(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type)
{
    return DoStartRoute_58A0D0(x1, y1, z1, x2, y2, z2, arrow_type);
}

MATCH_FUNC(0x58a1c0)
RouteFinder::RouteFinder()
{
    field_0_route_count = 0;
    field_2 = 0;
    field_4_unused = 0;
    memset(field_8_junctions, 0, sizeof(field_8_junctions));
    memset(field_2218_routes, 0, sizeof(field_2218_routes));
    field_8618_idx = 0;
    field_861A_dest_idx = 0;
    memset(field_861C_nodes, 0, sizeof(field_861C_nodes));
    field_A82C_open_list = NULL;
    memset(field_A830_horz_segments, 0, sizeof(field_A830_horz_segments));
    memset(field_B938_vert_segments, 0, sizeof(field_B938_vert_segments));
    memset(field_CA40_visited, 0, sizeof(field_CA40_visited));
    field_CC62_horz_segment_count = 0;
    field_CC64_vert_segment_count = 0;
    field_CC66_545_count = 0;
}