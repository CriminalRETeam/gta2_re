#include "RouteFinder.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "file.hpp"
#include "map_0x370.hpp"
#include "Game_0x40.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Hud.hpp"
#include "Frontend.hpp"
#include <cstdio>

DEFINE_GLOBAL(RouteFinder*, gRouteFinder_6FFDC8, 0x6FFDC8);
DEFINE_GLOBAL(u16, gLastRouteLength_6FFDCC, 0x6ffdcc);
DEFINE_GLOBAL(Fix16, dword_6FFC7C, 0x6FFC7C);
DEFINE_GLOBAL(Fix16, dword_6FFC9C, 0x6FFC9C);
EXTERN_GLOBAL(s16, word_703BAA);

MATCH_FUNC(0x588580)
char_type Junction_10::sub_588580(s32 a2)
{
    if (a2 == 2)
    {
        if (this->field_8_type != 1)
        {
            return 1;
        }
    }
    else if (a2 == 1)
    {
        if (this->field_8_type != 2)
        {
            return 1;
        }
    }
    else if (a2 == 3)
    {
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x5885c0)
u16 Junction_10::GetDirectionToJunction_5885C0(u16 a2)
{
    if (a2 != 0)
    {
        if (field_0_n.GetIndex_0040CE90() == a2)
        {
            return 1;
        }
        if (field_2_s.GetIndex_0040CE90() == a2)
        {
            return 2;
        }
        if (field_4_e.GetIndex_0040CE90() != a2)
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

MATCH_FUNC(0x5892d0)
RouteFinder_10::RouteFinder_10()
{
    field_0_idx = 0;
    field_2_cost = -1;
    field_4_expanded = 0;
    field_8_pParent = 0;
    field_C_pNext = 0;
}

// 9.6f 0x40CFC0: Camera_0xBC::WorldToScreen_40CFC0, but the original reads this file's copies
// of the constants (0x6FFC7C, 0x6FFC9C), so the Camera.hpp inline can't be used here
static inline Fix16_Point_POD ProjectToScreen(Camera_0xBC* pCam, Fix16 x, Fix16 y, Fix16 z)
{
    Fix16_Point_POD tmp;
    Fix16 u = pCam->field_98_cam_pos2.field_8_z - z;
    Fix16 t(dword_6FFC7C / Fix16(u.mValue + dword_6FFC9C.mValue, 0));

    tmp.x = (((x - pCam->field_98_cam_pos2.field_0_x) * pCam->field_60.y) * t) + Fix16(320);
    tmp.y = (((y - pCam->field_98_cam_pos2.field_4_y) * pCam->field_60.y) * t) + Fix16(240);
    return tmp;
}

WIP_FUNC(0x588620)
void RouteFinder::ShowJunctionIds_588620()
{
    Junction_10* pJunction = &field_8_junctions[1];
    for (u16 i = 1; i < GTA2_COUNTOF(field_8_junctions); i++, pJunction++)
    {
        if (!pJunction->field_C_min_x)
        {
            break;
        }

        if (gGame_0x40_67E008->field_38_orf1->field_14C_view_camera.IsPointInBoundaries_58CF10(pJunction->field_C_min_x,
                                                                                              (s32)pJunction->field_D_min_y))
        {
            u8 x = pJunction->field_C_min_x;
            u8 y = pJunction->field_D_min_y;
            Fix16_Point_POD screen = ProjectToScreen(&gGame_0x40_67E008->field_38_orf1->field_14C_view_camera,
                                                     Fix16(x),
                                                     Fix16(y),
                                                     gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(Fix16(x), Fix16(y)));

            swprintf(tmpBuff_67BD9C, L"%d", i);
            gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, screen.x.ToInt(), screen.y.ToInt(), word_703BAA, 1);
        }
    }
}

MATCH_FUNC(0x588810)
void RouteFinder::RoadOff_588810(u8 x, u8 y, u8 z)
{
    const u16 r1 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, 0, green_or_red_3);
    const u16 r2 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, 1, green_or_red_3);
    if (r1 && r2)
    {
        Junction_10* j1 = &field_8_junctions[r1];
        Junction_10* j2 = &field_8_junctions[r2];
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
            Junction_10* j1 = &field_8_junctions[r3];
            Junction_10* j2 = &field_8_junctions[r4];

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
        Junction_10* j1 = &field_8_junctions[r1];
        Junction_10* j2 = &field_8_junctions[r2];
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
        Junction_10* j1 = &field_8_junctions[r3];
        Junction_10* j2 = &field_8_junctions[r4];

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

    Junction_10* pJ1 = &this->field_8_junctions[junc_idx1];
    Junction_10* pJ2 = &this->field_8_junctions[junc_idx2];

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
    File::Global_Read_4A71C0(&this->field_4, 2);
    File::Global_Read_4A71C0(&this->field_CC62_horz_segment_count, 2);
    File::Global_Read_4A71C0(&this->field_CC64_vert_segment_count, 2);
    this->field_0_route_count = 0;

    if (bLog_routefinder_67D6D1)
    {
        int iVar2 = 0;
        do
        {
            sprintf(gTmpBuffer_67C598,
                    "Junc: %d (%d, %d) n %d s %d w %d e %d",
                    iVar2,
                    field_8_junctions[iVar2].field_C_min_x,
                    field_8_junctions[iVar2].field_D_min_y,
                    field_8_junctions[iVar2].field_0_n.GetIndex_0040CE90(),
                    field_8_junctions[iVar2].field_2_s.GetIndex_0040CE90(),
                    field_8_junctions[iVar2].field_6_w.GetIndex_0040CE90(),
                    field_8_junctions[iVar2].field_4_e.GetIndex_0040CE90());
            gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);

            if (iVar2 > 0 && field_8_junctions[iVar2].field_C_min_x == 0 && field_8_junctions[iVar2].field_D_min_y == 0)
            {
                break;
            }
            iVar2++;
        } while (iVar2 < 0x221);

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
bool RouteFinder::HasBlockDesiredArrow_588CA0(gmp_block_info* block, s32 arrow_type, u8 direction)
{
    switch (direction)
    {
        case UP_1:

            switch (arrow_type)
            {
                case green_1:
                    if ((block->field_A_arrows & 4) != 0) // green up
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->field_A_arrows & 0x40) != 0) // red up
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->field_A_arrows & 0x44) != 0) // green or red up
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
                    if ((block->field_A_arrows & 8) != 0) // green down
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->field_A_arrows & 0x80) != 0) // red down
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->field_A_arrows & 0x88) != 0) // green or red down
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
                    if ((block->field_A_arrows & 1) != 0) // green left
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->field_A_arrows & 0x10) != 0) // red left
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->field_A_arrows & 0x11) != 0) // green or red left
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
                    if ((block->field_A_arrows & 2) != 0) // green right
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->field_A_arrows & 0x20) != 0) // red right
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->field_A_arrows & 0x22) != 0) // green or red right
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
char_type RouteFinder::sub_588DE0(gmp_block_info* pBlock, s32 arrow_type, s32 road_direction)
{

    char_type result = 0;
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
u16 RouteFinder::FindHorzSegmentJunction_588E60(u8 x, u8 y, u8 z, char_type a5, s32 arrow_type)
{
    JunctionSegment_0x8* pSegment = field_A830_horz_segments;
    gmp_block_info* block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(x, y, z);

    if (block_4DFE10 != NULL)
    {
        s8 v9 = RouteFinder::HasBlockDesiredArrow_588CA0(block_4DFE10, arrow_type, LEFT_3);
        for (s16 junc_idx = 0; junc_idx < field_CC62_horz_segment_count; pSegment++, junc_idx++)
        {
            if (pSegment->ContainsPoint_40CF20(x, y))
            {
                if (a5 == 1)
                {
                    if (v9 == 1)
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
                    if (v9 == 1)
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
u16 RouteFinder::FindVertSegmentJunction_588F30(u8 x, u8 y, u8 z, char_type a5, s32 arrow_type)
{
    JunctionSegment_0x8* pSegment = field_B938_vert_segments;
    gmp_block_info* block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(x, y, z);

    if (block_4DFE10 != NULL)
    {
        s8 v9 = RouteFinder::HasBlockDesiredArrow_588CA0(block_4DFE10, arrow_type, UP_1);
        for (s16 junc_idx = 0; junc_idx < field_CC64_vert_segment_count; pSegment++, junc_idx++)
        {
            if (pSegment->ContainsPoint_40CF20(x, y))
            {
                if (a5 == 1)
                {
                    if (v9 == 1)
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
                    if (v9 == 1)
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
u16 RouteFinder::FindSegmentJunction_589000(u8 x_coord, u8 y_coord, u8 z_coord, char_type a5, s32 arrow_type)
{
    gmp_block_info* pBlock = gMap_0x370_6F6268->get_block_4DFE10(x_coord, y_coord, z_coord);

    if (pBlock)
    {
        if (RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, UP_1) || RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, DOWN_2))
        {
            return RouteFinder::FindVertSegmentJunction_588F30(x_coord, y_coord, z_coord, a5, arrow_type);
        }
        if (RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, LEFT_3) || RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, RIGHT_4))
        {
            return RouteFinder::FindHorzSegmentJunction_588E60(x_coord, y_coord, z_coord, a5, arrow_type);
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
            if (gMap_0x370_6F6268->GetArrowDirectionFromBlock_4E5FC0(block, 0))
            {
                *xpos = x_pos;
                *ypos = y_pos;
                return;
            }
        }
    }
}

// dx/dy are uninitialised for a direction that isn't 1, 2, 4 or 8, as in the original
#pragma warning(push)
#pragma warning(disable : 4701)
MATCH_FUNC(0x589210)
s32 RouteFinder::NoRefs_589210(u8 x, u8 y, s32 a4, u8 direction, s32 a6, u16 junction_idx)
{
    Junction_10* pJunction = &field_8_junctions[junction_idx];
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
    if (pJunction->ContainsPoint((u8)(x + (s16)dx), (u8)(y + dy)))
    {
        result = 1;
    }
    return result;
}
#pragma warning(pop)

WIP_FUNC(0x5892f0)
RouteFinder_10* RouteFinder::NewChildNode_5892F0(RouteFinder_10* a2, u16 idx, s16 a4)
{
    WIP_IMPLEMENTED;

    RouteFinder_10* pNew10 = &this->field_861C_nodes[this->field_CC66_545_count++];

    s32 dy = abs((u8)field_8_junctions[idx].field_D_min_y - (u8)field_8_junctions[field_861A_dest_idx].field_D_min_y);
    s32 dx = abs((u8)field_8_junctions[idx].field_C_min_x - (u8)field_8_junctions[field_861A_dest_idx].field_C_min_x);

    pNew10->field_0_idx = idx;
    pNew10->field_2_cost = a2->field_2_cost + a4 + (dy + dx);
    pNew10->field_8_pParent = a2;
    pNew10->field_C_pNext = 0;

    return pNew10;
}

MATCH_FUNC(0x589390)
RouteFinder_10* RouteFinder::NewStartNode_589390(u16 a2)
{
    RouteFinder_10* pNew10 = &field_861C_nodes[field_CC66_545_count++];
    s32 distance = abs(field_8_junctions[a2].field_C_min_x - field_8_junctions[field_861A_dest_idx].field_C_min_x) +
        abs(field_8_junctions[a2].field_D_min_y - field_8_junctions[field_861A_dest_idx].field_D_min_y);

    pNew10->field_2_cost = distance;
    pNew10->field_0_idx = a2;
    pNew10->field_4_expanded = 0;
    // field_6 is preserved by the original function.
    pNew10->field_8_pParent = 0;
    pNew10->field_C_pNext = 0;
    return pNew10;
}

MATCH_FUNC(0x589420)
void RouteFinder::InsertIntoOpenList_589420(RouteFinder_10* p10)
{
    field_CA40_visited[p10->field_0_idx] = 1;

    if (p10->field_2_cost < field_A82C_open_list->field_2_cost)
    {
        p10->field_C_pNext = field_A82C_open_list;
        field_A82C_open_list = p10;
    }
    else
    {
        RouteFinder_10* v3;
        for (v3 = field_A82C_open_list; v3->field_C_pNext != NULL && v3->field_C_pNext->field_2_cost < p10->field_2_cost; v3 = v3->field_C_pNext)
        {
        }
        p10->field_C_pNext = v3->field_C_pNext;
        v3->field_C_pNext = p10;
    }
}

MATCH_FUNC(0x589480)
char_type RouteFinder::InitSearch_589480(u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, s32 a8)
{
    field_CC66_545_count = 0;
    memset(field_CA40_visited, 0, sizeof(field_CA40_visited));
    memset(field_861C_nodes, 0, sizeof(field_861C_nodes));

    field_8618_idx = FindSegmentJunction_589000(a2, a3, a4, 0, a8);
    if (field_8618_idx == 0)
    {
        field_8618_idx = FindSegmentJunction_589000(a2, a3, a4, 1, a8);
    }

    u16 initialIdx = field_8618_idx;
    Junction_10* pJunction = &field_8_junctions[initialIdx];
    if (a2 < pJunction->field_C_min_x || a2 > pJunction->field_E_max_x || a3 < pJunction->field_D_min_y || a3 > pJunction->field_F_max_y)
    {
        field_8618_idx = FindSegmentJunction_589000(a2, a3, a4, 1, a8);
    }

    field_861A_dest_idx = FindSegmentJunction_589000(a5, a6, a7, 1, a8);
    if (field_861A_dest_idx == field_8618_idx)
    {
        field_8618_idx = initialIdx;
    }

    if (field_8618_idx != 0 && field_861A_dest_idx != 0)
    {
        NewStartNode_589390(field_8618_idx);
        field_CA40_visited[0] = 1;
        field_A82C_open_list = field_861C_nodes;
        field_CA40_visited[field_8618_idx] = 1;
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x5895c0)
char_type RouteFinder::sub_5895C0(u8 x, u8 y, u8 z, s32 arrow_type, s32 direction)
{
    field_CC66_545_count = 0;
    memset(field_CA40_visited, 0, sizeof(field_CA40_visited));
    RouteFinder_10* pStart = field_861C_nodes;
    memset(pStart, 0, sizeof(field_861C_nodes));

    field_8618_idx = FindSegmentJunction_589000(x, y, z, 0, arrow_type);
    if (field_8618_idx == 0)
    {
        field_8618_idx = FindSegmentJunction_589000(x, y, z, 1, arrow_type);
    }

    if (field_8618_idx != 0)
    {
        Junction_10* pJunction = &field_8_junctions[field_8618_idx];
        if (x < pJunction->field_C_min_x || x > pJunction->field_E_max_x || y < pJunction->field_D_min_y ||
            y > pJunction->field_F_max_y)
        {
            field_8618_idx = FindSegmentJunction_589000(x, y, z, 1, arrow_type);
        }

        if (field_8618_idx != 0)
        {
            NewStartNode_589390(field_8618_idx);
            field_A82C_open_list = pStart;
            field_CA40_visited[0] = 1;
            field_CA40_visited[field_8618_idx] = 1;

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
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x589930)
void RouteFinder::CancelRoute_589930(s16 junc_idx)
{
    this->field_2218_routes[junc_idx].field_0_junctions[0] = 0;
    if (this->field_0_route_count > 0)
    {
        --this->field_0_route_count;
    }
}

MATCH_FUNC(0x589960)
s16 RouteFinder::GetFreeRouteIdx_589960()
{
    s16 sVar1 = 1;
    if (this->field_0_route_count < 50)
    {
        while (sVar1 < 50)
        {
            if (field_2218_routes[sVar1++].field_0_junctions[0] == 0)
            {
                return sVar1 - 1;
            }
        }
    }
    return -1;
}

MATCH_FUNC(0x589990)
u16 RouteFinder::AddChildNode_589990(RouteFinder_10* a2, u16 a3, s16 a4)
{
    RouteFinder_10* puVar1 = NewChildNode_5892F0(a2, a3, a4);
    InsertIntoOpenList_589420(puVar1);
    return puVar1->field_0_idx;
}

// Returns true if a neighbour of the node's junction that hasn't been visited yet completes the route
MATCH_FUNC(0x5899c0)
bool RouteFinder::sub_5899C0(RouteFinder_10* pNode, s32 a3)
{
    Junction_10* pJunction = &field_8_junctions[pNode->field_0_idx];
    u16 north = pJunction->field_0_n.GetIndex_0040CE90();
    u16 west = pJunction->field_6_w.GetIndex_0040CE90();
    u16 south = pJunction->field_2_s.GetIndex_0040CE90();
    u16 east = pJunction->field_4_e.GetIndex_0040CE90();
    pNode->field_4_expanded = 1;

    if (!field_CA40_visited[north] && pJunction->field_0_n.IsEnabled() && field_8_junctions[north].sub_588580(a3) &&
        AddChildNode_589990(pNode, north, pJunction->field_0_n.GetLength()) == field_861A_dest_idx)
    {
        return true;
    }
    if (!field_CA40_visited[south] && pJunction->field_2_s.IsEnabled() && field_8_junctions[south].sub_588580(a3) &&
        AddChildNode_589990(pNode, south, pJunction->field_2_s.GetLength()) == field_861A_dest_idx)
    {
        return true;
    }
    if (!field_CA40_visited[west] && pJunction->field_6_w.IsEnabled() && field_8_junctions[west].sub_588580(a3) &&
        AddChildNode_589990(pNode, west, pJunction->field_6_w.GetLength()) == field_861A_dest_idx)
    {
        return true;
    }
    if (!field_CA40_visited[east] && pJunction->field_4_e.IsEnabled() && field_8_junctions[east].sub_588580(a3) &&
        AddChildNode_589990(pNode, east, pJunction->field_4_e.GetLength()) == field_861A_dest_idx)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x589bb0)
char_type RouteFinder::sub_589BB0(RouteFinder_10* a2, s32 a3)
{
    Junction_10* pJunction = &field_8_junctions[a2->field_0_idx];
    if (!gGame_0x40_67E008->is_point_on_screen_4B9A80(pJunction->field_C_min_x, pJunction->field_D_min_y) &&
        !gGame_0x40_67E008->is_point_on_screen_4B9A80(pJunction->field_E_max_x, pJunction->field_D_min_y) &&
        !gGame_0x40_67E008->is_point_on_screen_4B9A80(pJunction->field_C_min_x, pJunction->field_F_max_y) &&
        !gGame_0x40_67E008->is_point_on_screen_4B9A80(pJunction->field_E_max_x, pJunction->field_F_max_y))
    {
        return 1;
    }

    u16 north = pJunction->field_0_n.GetIndex_0040CE90();
    u16 south = pJunction->field_2_s.GetIndex_0040CE90();
    u16 west = pJunction->field_6_w.GetIndex_0040CE90();
    u16 east = pJunction->field_4_e.GetIndex_0040CE90();
    a2->field_4_expanded = 1;

    if (!field_CA40_visited[north] && pJunction->field_0_n.IsEnabled() && field_8_junctions[north].sub_588580(a3))
    {
        AddChildNode_589990(a2, north, pJunction->field_0_n.GetLength());
    }
    if (!field_CA40_visited[south] && pJunction->field_2_s.IsEnabled() && field_8_junctions[south].sub_588580(a3))
    {
        AddChildNode_589990(a2, south, pJunction->field_2_s.GetLength());
    }
    if (!field_CA40_visited[west] && pJunction->field_6_w.IsEnabled() && field_8_junctions[west].sub_588580(a3))
    {
        AddChildNode_589990(a2, west, pJunction->field_6_w.GetLength());
    }
    if (!field_CA40_visited[east] && pJunction->field_4_e.IsEnabled() && field_8_junctions[east].sub_588580(a3))
    {
        AddChildNode_589990(a2, east, pJunction->field_4_e.GetLength());
    }
    return 0;
}

MATCH_FUNC(0x589e00)
RouteFinder_10* RouteFinder::GetFirstUnexpandedNode_589E00()
{
    RouteFinder_10* pjVar1;

    for (pjVar1 = field_A82C_open_list; pjVar1 != NULL && pjVar1->field_4_expanded != 0; pjVar1 = pjVar1->field_C_pNext)
        ;
    return pjVar1;
}

WIP_FUNC(0x589e20)
char_type RouteFinder::sub_589E20(s32 a2)
{
    WIP_IMPLEMENTED;

    bool bRet = 0;
    RouteFinder_10* f_A82C = this->field_A82C_open_list;
    if (f_A82C->field_0_idx == this->field_861A_dest_idx)
    {
        return 1;
    }

    if (f_A82C)
    {
        while (!bRet)
        {
            RouteFinder_10* p10 = GetFirstUnexpandedNode_589E00();
            if (!p10)
            {
                break;
            }
            bRet = sub_5899C0(p10, a2);
        }
    }
    return bRet;
}

MATCH_FUNC(0x589e70)
char_type RouteFinder::sub_589E70(s32 a2)
{
    char cVar1;
    RouteFinder_10* iVar2;

    cVar1 = '\0';
    if (this->field_A82C_open_list != 0)
    {
        while (true)
        {
            if (cVar1 != '\0')
            {
                break;
            }

            iVar2 = GetFirstUnexpandedNode_589E00();
            if (iVar2 == NULL)
            {
                break;
            }
            cVar1 = sub_589BB0(iVar2, a2);
        }
    }
    return cVar1;
}

MATCH_FUNC(0x589eb0)
s16 RouteFinder::sub_589EB0()
{
    u16 count = 0;
    if (field_CC66_545_count == 0)
    {
        return -1;
    }

    RouteFinder_10* pjVar4 = &field_861C_nodes[field_CC66_545_count - 1];
    s16 sVar2 = GetFreeRouteIdx_589960();
    if (sVar2 == -1)
    {
        return -1;
    }

    while (pjVar4 != NULL)
    {
        pjVar4 = pjVar4->field_8_pParent;
        count++;
    }
    gLastRouteLength_6FFDCC = count;

    pjVar4 = &field_861C_nodes[field_CC66_545_count - 1];

    if (pjVar4 != NULL)
    {
        do
        {
            field_2218_routes[sVar2].field_0_junctions[--count] = pjVar4->field_0_idx;
            pjVar4 = pjVar4->field_8_pParent;
        } while (pjVar4 != NULL);
    }
    field_2218_routes[sVar2].field_0_junctions[gLastRouteLength_6FFDCC] = 0;
    field_0_route_count++;
    return sVar2;
}

MATCH_FUNC(0x589f70)
s16 RouteFinder::sub_589F70()
{
    s16 sVar1 = 0;
    u8 uVar3 = 0;
    if (field_CC66_545_count == 0)
    {
        return -1;
    }

    RouteFinder_10* pjVar4 = &field_861C_nodes[field_CC66_545_count - 1];
    s16 sVar2 = GetFreeRouteIdx_589960();
    if (sVar2 == -1)
    {
        return -1;
    }

    while (pjVar4 != NULL)
    {
        pjVar4 = pjVar4->field_8_pParent;
        sVar1++;
    }
    gLastRouteLength_6FFDCC = sVar1;

    pjVar4 = &field_861C_nodes[field_CC66_545_count - 1];

    if (pjVar4 != NULL)
    {
        do
        {
            field_2218_routes[sVar2].field_0_junctions[uVar3] = pjVar4->field_0_idx;
            pjVar4 = pjVar4->field_8_pParent;
            uVar3++;

        } while (pjVar4 != NULL);
    }
    field_0_route_count++;
    return sVar2;
}

MATCH_FUNC(0x58a020)
void RouteFinder::DebugPrintRoute_58A020(char_type junc_idx)
{
    s32 v3 = 0;
    u16 i = this->field_2218_routes[junc_idx].field_0_junctions[(u16)v3];
    while (i)
    {
        Junction_10* pJunc = &this->field_8_junctions[i];
        sprintf(gTmpBuffer_67C598,
                "Junc: %d : (%d, %d)(%d, %d)",
                i,
                pJunc->field_C_min_x,
                pJunc->field_D_min_y,
                pJunc->field_E_max_x,
                pJunc->field_F_max_y);
        gFile_67C530.Write_4D9620(gTmpBuffer_67C598);
        ++v3;
        i = this->field_2218_routes[junc_idx].field_0_junctions[(u16)v3];
    }
}

MATCH_FUNC(0x58a0b0)
Junction_10* RouteFinder::GetJunction_58A0B0(u16 jIdx)
{
    return &field_8_junctions[jIdx];
}

MATCH_FUNC(0x58a0d0)
s16 RouteFinder::DoStartRoute_58A0D0(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 a8)
{
    if (InitSearch_589480(x1, y1, z1, x2, y2, z2, a8) && sub_589E20(a8))
    {
        return sub_589EB0();
    }
    else
    {
        return -1;
    }
}

MATCH_FUNC(0x58a130)
s16 RouteFinder::sub_58A130(u8 a1, s16 a2, u8 a3, u8* a4, s32 a5, s32 a6)
{
    if (sub_5895C0(a1, a2, a3, a5, a6))
    {
        if (sub_589E70(a5))
        {
            s16 ret = sub_589F70();
            *a4 = (u8)gLastRouteLength_6FFDCC;
            return ret;
        }
    }
    return -1;
}

MATCH_FUNC(0x58a190)
u16 RouteFinder::StartRoute_58A190(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 a8)
{
    return DoStartRoute_58A0D0(x1, y1, z1, x2, y2, z2, a8);
}

MATCH_FUNC(0x58a1c0)
RouteFinder::RouteFinder()
{
    field_0_route_count = 0;
    field_2 = 0;
    field_4 = 0;
    memset(field_8_junctions, 0, sizeof(field_8_junctions));
    memset(field_2218_routes, 0, sizeof(field_2218_routes));
    field_8618_idx = 0;
    field_861A_dest_idx = 0;
    memset(field_861C_nodes, 0, sizeof(field_861C_nodes));
    field_A82C_open_list = 0;
    memset(field_A830_horz_segments, 0, sizeof(field_A830_horz_segments));
    memset(field_B938_vert_segments, 0, sizeof(field_B938_vert_segments));
    memset(field_CA40_visited, 0, sizeof(field_CA40_visited));
    field_CC62_horz_segment_count = 0;
    field_CC64_vert_segment_count = 0;
    field_CC66_545_count = 0;
}