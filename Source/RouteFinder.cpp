#include "RouteFinder.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "file.hpp"
#include "map_0x370.hpp"
#include <cstdio>

DEFINE_GLOBAL(RouteFinder*, gRouteFinder_6FFDC8, 0x6FFDC8);
DEFINE_GLOBAL(u8, DAT_6ffdcc, 0x6ffdcc);

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
u16 Junction_10::sub_5885C0(u16 a2)
{
    if (a2 != 0)
    {
        if (field_0_n.FUN_0040ce90() == a2)
        {
            return 1;
        }
        if (field_2_s.FUN_0040ce90() == a2)
        {
            return 2;
        }
        if (field_4_e.FUN_0040ce90() != a2)
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
    field_2 = -1;
    field_4 = 0;
    field_8 = 0;
    field_C_pNext = 0;
}

STUB_FUNC(0x588620)
void RouteFinder::ShowJunctionIds_588620()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x588810)
u16 RouteFinder::RoadOff_588810(u8 a2, u8 a3, u8 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x588950)
u16 RouteFinder::RoadOn_588950(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x588aa0)
u16 RouteFinder::IsPointInJunctionBounds_588AA0(u8 a2, u8 a3, u16 a4, u16 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x588b30)
void RouteFinder::Load_RGEN_588B30()
{
    File::Global_Read_4A71C0(field_8, 0x2210);
    File::Global_Read_4A71C0(this->field_A830, 0x1108);
    File::Global_Read_4A71C0(this->field_B938, 0x1108);
    File::Global_Read_4A71C0(&this->field_4, 2);
    File::Global_Read_4A71C0(&this->field_CC62, 2);
    File::Global_Read_4A71C0(&this->field_CC64, 2);
    this->field_0 = 0;

    if (bLog_routefinder_67D6D1)
    {
        int iVar2 = 0;
        do
        {
            sprintf(gTmpBuffer_67C598,
                    "Junc: %d (%d, %d) n %d s %d w %d e %d",
                    iVar2,
                    field_8[iVar2].field_C_min_x,
                    field_8[iVar2].field_D_min_y,
                    field_8[iVar2].field_0_n.FUN_0040ce90(),
                    field_8[iVar2].field_2_s.FUN_0040ce90(),
                    field_8[iVar2].field_6_w.FUN_0040ce90(),
                    field_8[iVar2].field_4_e.FUN_0040ce90());
            gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);

            if (iVar2 > 0 && field_8[iVar2].field_C_min_x == 0 && field_8[iVar2].field_D_min_y == 0)
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
    memset(this->field_CA40, 0, sizeof(this->field_CA40));
    memset(this->field_861C, 0, sizeof(this->field_861C));
    memset(this->field_2218, 0, sizeof(this->field_2218));
}

MATCH_FUNC(0x588ca0)
bool RouteFinder::sub_588CA0(gmp_block_info* block, s32 a2, u8 a3)
{
    switch (a3)
    {
        case 1:

            switch (a2)
            {
                case 1:
                    if ((block->field_A_arrows & 4) != 0)
                    {
                        return true;
                    }
                    break;
                case 2:
                    if ((block->field_A_arrows & 0x40) != 0)
                    {
                        return true;
                    }
                    break;
                case 3:
                    if ((block->field_A_arrows & 0x44) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case 2:
            switch (a2)
            {
                case 1:
                    if ((block->field_A_arrows & 8) != 0)
                    {
                        return true;
                    }
                    break;
                case 2:
                    if ((block->field_A_arrows & 0x80) != 0)
                    {
                        return true;
                    }
                    break;
                case 3:
                    if ((block->field_A_arrows & 0x88) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case 3:
            switch (a2)
            {
                case 1:
                    if ((block->field_A_arrows & 1) != 0)
                    {
                        return true;
                    }
                    break;
                case 2:
                    if ((block->field_A_arrows & 0x10) != 0)
                    {
                        return true;
                    }
                    break;
                case 3:
                    if ((block->field_A_arrows & 0x11) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case 4:
            switch (a2)
            {
                case 1:
                    if ((block->field_A_arrows & 2) != 0)
                    {
                        return true;
                    }
                    break;
                case 2:
                    if ((block->field_A_arrows & 0x20) != 0)
                    {
                        return true;
                    }
                    break;
                case 3:
                    if ((block->field_A_arrows & 0x22) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;
    }
    return false;
}

STUB_FUNC(0x588de0)
char_type RouteFinder::sub_588DE0(gmp_block_info* a1, s32 a2, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x588e60)
u16 RouteFinder::sub_588E60(u8 x, u8 y, u8 z, char_type a5, s32 a6)
{
    JunctionSegment_0x8* pSegment = field_A830;
    gmp_block_info* block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(x, y, z);

    if (block_4DFE10 != NULL)
    {
        s8 v9 = RouteFinder::sub_588CA0(block_4DFE10, a6, 3);
        for (s16 junc_idx = 0; junc_idx < field_CC62; pSegment++, junc_idx++)
        {
            if (pSegment->sub_40CF20(x, y))
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
u16 RouteFinder::sub_588F30(u8 x, u8 y, u8 z, char_type a5, s32 a6)
{
    JunctionSegment_0x8* pSegment = field_B938;
    gmp_block_info* block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(x, y, z);

    if (block_4DFE10 != NULL)
    {
        s8 v9 = RouteFinder::sub_588CA0(block_4DFE10, a6, 1);
        for (s16 junc_idx = 0; junc_idx < field_CC64; pSegment++, junc_idx++)
        {
            if (pSegment->sub_40CF20(x, y))
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
u16 RouteFinder::sub_589000(u8 x_coord, u8 y_coord, u8 z_coord, char_type a5, s32 a6)
{
    gmp_block_info* pBlock = gMap_0x370_6F6268->get_block_4DFE10(x_coord, y_coord, z_coord);

    if (pBlock)
    {
        if (RouteFinder::sub_588CA0(pBlock, a6, 1) || RouteFinder::sub_588CA0(pBlock, a6, 2))
        {
            return RouteFinder::sub_588F30(x_coord, y_coord, z_coord, a5, a6);
        }
        if (RouteFinder::sub_588CA0(pBlock, a6, 3) || RouteFinder::sub_588CA0(pBlock, a6, 4))
        {
            return RouteFinder::sub_588E60(x_coord, y_coord, z_coord, a5, a6);
        }
    }
    return 0;
}

MATCH_FUNC(0x5890d0)
void RouteFinder::sub_5890D0(u16 junction_idx, s32 direction, u8* xpos, u8* ypos)
{
    for (u8 y = field_8[junction_idx].field_D_min_y; y <= field_8[junction_idx].field_F_max_y; y++)
    {
        for (u8 x = field_8[junction_idx].field_C_min_x; x <= field_8[junction_idx].field_E_max_x; x++)
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

    for (u8 y_pos = field_8[junction_idx].field_D_min_y; y_pos <= field_8[junction_idx].field_F_max_y; y_pos++)
    {
        for (u8 x_pos = field_8[junction_idx].field_C_min_x; x_pos <= field_8[junction_idx].field_E_max_x; x_pos++)
        {
            s32 z;
            gmp_block_info* block = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x_pos, y_pos, &z);
            if (gMap_0x370_6F6268->sub_4E5FC0(block, 0))
            {
                *xpos = x_pos;
                *ypos = y_pos;
                return;
            }
        }
    }
}

STUB_FUNC(0x589210)
s32 RouteFinder::sub_589210(char_type a2, char_type a3, s32 a4, char_type a5, s32 a6, u16 a7)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5892f0)
RouteFinder_10* RouteFinder::sub_5892F0(RouteFinder_10* a2, u16 a3, s16 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x589390)
RouteFinder_10* RouteFinder::sub_589390(u16 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x589420)
void RouteFinder::sub_589420(RouteFinder_10* p10)
{
    WIP_IMPLEMENTED;

    this->field_CA40[p10->field_0_idx] = 1;
    RouteFinder_10* pItem = this->field_A82C;
    u16 toFind = p10->field_2;
    if (toFind >= (s32)pItem->field_2)
    {
        for (RouteFinder_10* pIter = pItem->field_C_pNext; pIter; pIter = pIter->field_C_pNext)
        {
            if ((s32)pIter->field_2 >= toFind)
            {
                break;
            }
            pItem = pIter;
        }
        p10->field_C_pNext = pItem->field_C_pNext;
        pItem->field_C_pNext = p10;
    }
    else
    {
        p10->field_C_pNext = pItem;
        this->field_A82C = p10;
    }
}

STUB_FUNC(0x589480)
char_type RouteFinder::sub_589480(u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, s32 a8)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5895c0)
char_type RouteFinder::sub_5895C0(u8 a2, s16 a3, u8 a4, s32 a5, s32 a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x589930)
void RouteFinder::CancelRoute_589930(s16 junc_idx)
{
    this->field_2218[junc_idx].field_0[0] = 0;
    if (this->field_0 > 0)
    {
        --this->field_0;
    }
}

MATCH_FUNC(0x589960)
s16 RouteFinder::sub_589960()
{
    s16 sVar1 = 1;
    if (this->field_0 < 50)
    {
        while (sVar1 < 0x32)
        {
            if (field_2218[sVar1++].field_0[0] == 0)
            {
                return sVar1 - 1;
            }
        }
    }
    return -1;
}

MATCH_FUNC(0x589990)
u16 RouteFinder::sub_589990(RouteFinder_10* a2, u16 a3, s16 a4)
{
    RouteFinder_10* puVar1 = sub_5892F0(a2, a3, a4);
    sub_589420(puVar1);
    return puVar1->field_0_idx;
}

STUB_FUNC(0x5899c0)
bool RouteFinder::sub_5899C0(RouteFinder_10* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x589bb0)
char_type RouteFinder::sub_589BB0(RouteFinder_10* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x589e00)
RouteFinder_10* RouteFinder::sub_589E00()
{
    RouteFinder_10* pjVar1;

    for (pjVar1 = field_A82C; pjVar1 != NULL && pjVar1->field_4 != 0; pjVar1 = pjVar1->field_C_pNext)
        ;
    return pjVar1;
}

STUB_FUNC(0x589e20)
char_type RouteFinder::sub_589E20(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x589e70)
char_type RouteFinder::sub_589E70(s32 a2)
{
    char cVar1;
    RouteFinder_10* iVar2;

    cVar1 = '\0';
    if (this->field_A82C != 0)
    {
        while (true)
        {
            if (cVar1 != '\0')
            {
                break;
            }

            iVar2 = sub_589E00();
            if (iVar2 == NULL)
            {
                break;
            }
            cVar1 = sub_589BB0(iVar2, a2);
        }
    }
    return cVar1;
}

STUB_FUNC(0x589eb0)
s16 RouteFinder::sub_589EB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x589f70)
s16 RouteFinder::sub_589F70()
{
    NOT_IMPLEMENTED;
    s16 sVar1 = 0;
    u8 uVar3 = 0;
    if (field_CC66_545_count == 0)
    {
        return -1;
    }

    RouteFinder_10* pjVar4 = &field_861C[field_CC66_545_count - 1];
    s16 sVar2 = sub_589960();
    if (sVar2 == -1)
    {
        return -1;
    }

    while (pjVar4 != NULL)
    {
        pjVar4 = pjVar4->field_8;
        sVar1++;
    }
    DAT_6ffdcc = sVar1;

    pjVar4 = &field_861C[field_CC66_545_count - 1];

    if (pjVar4 != NULL)
    {
        do
        {
            field_2218[sVar2].field_0[uVar3] = pjVar4->field_0_idx;
            pjVar4 = pjVar4->field_8;
            uVar3++;

        } while (pjVar4 != NULL);
    }
    field_0++;
    return sVar2;
}

STUB_FUNC(0x58a020)
void RouteFinder::sub_58A020(char_type a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x58a0b0)
Junction_10* RouteFinder::GetJunction_58A0B0(u16 jIdx)
{
    return &field_8[jIdx];
}

MATCH_FUNC(0x58a0d0)
s16 RouteFinder::DoStartRoute_58A0D0(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 a8)
{
    if (sub_589480(x1, y1, z1, x2, y2, z2, a8) && sub_589E20(a8))
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
            *a4 = DAT_6ffdcc;
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
    field_0 = 0;
    field_2 = 0;
    field_4 = 0;
    memset(field_8, 0, sizeof(field_8));
    memset(field_2218, 0, sizeof(field_2218));
    field_8618_idx = 0;
    field_861A = 0;
    memset(field_861C, 0, sizeof(field_861C));
    field_A82C = 0;
    memset(field_A830, 0, sizeof(field_A830));
    memset(field_B938, 0, sizeof(field_B938));
    memset(field_CA40, 0, sizeof(field_CA40));
    field_CC62 = 0;
    field_CC64 = 0;
    field_CC66_545_count = 0;
}