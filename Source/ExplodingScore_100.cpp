#include "ExplodingScore_100.hpp"
#include "Camera.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include <memory.h>

DEFINE_GLOBAL(ExplodingScorePool*, gExplodingScorePool, 0x702F34);

DEFINE_GLOBAL(Fix16, dword_702C6C, 0x702C6C); // 0x2800000000LL / dword_702DF0
DEFINE_GLOBAL(Fix16, dword_702BC4, 0x702BC4); // 0x3FC000
DEFINE_GLOBAL(Fix16, dword_702F10, 0x702F10); // 0xA00000 - dword_702C6C
DEFINE_GLOBAL(Fix16, dword_702C74, 0x702C74); // 0x1E00000000LL / dword_702DF0
DEFINE_GLOBAL(Fix16, dword_702C08, 0x702C08); // 0x780000 - dword_702C74

DEFINE_GLOBAL(Fix16, dword_702DE0, 0x702DE0);
DEFINE_GLOBAL(Ang16, word_702F24, 0x702F24);

MATCH_FUNC(0x596a00)
ExplodingScore_50::ExplodingScore_50()
{
    field_0_numbers_count = 0;
    for (s32 i = 0; i < 9; i++)
    {
        field_4[i] = 0;
    }
    field_36 = 0;
    field_38 = 0;
    field_3C = 0;
    field_40 = 0;
    field_4C_score = 0;
    field_48 = 0;
    mpNext = 0;
    field_34 = 9;
}

MATCH_FUNC(0x596a40)
ExplodingScore_50::~ExplodingScore_50()
{
    mpNext = 0;
}

MATCH_FUNC(0x596a50)
void ExplodingScore_50::PoolAllocate()
{
}

MATCH_FUNC(0x596a60)
void ExplodingScore_50::PoolDeallocate()
{
}

MATCH_FUNC(0x596a70)
void ExplodingScore_50::ClearNumbersArrayFrom_596A70(s32 a2)
{
    for (u32 i = a2; i > 0; i--)
    {
        field_4[i] = 0;
    }
}

MATCH_FUNC(0x596a90)
void ExplodingScore_50::InitScore_596A90(Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 score)
{
    u32 tmpScore = score;

    this->field_4C_score = score;
    this->field_0_numbers_count = 0;

    if (score >= 100000000)
    {
        this->field_0_numbers_count = 8;
        this->field_4[8] = score / 100000000;
        ClearNumbersArrayFrom_596A70(7);
        tmpScore = score % 100000000;
    }

    if (tmpScore >= 10000000)
    {
        if (!this->field_0_numbers_count)
        {
            this->field_0_numbers_count = 7;
        }
        this->field_4[7] = tmpScore / 10000000;
        ClearNumbersArrayFrom_596A70(6);
        tmpScore %= 10000000u;
    }

    if (tmpScore >= 1000000)
    {
        if (!this->field_0_numbers_count)
        {
            this->field_0_numbers_count = 6;
        }
        this->field_4[6] = tmpScore / 1000000;
        ClearNumbersArrayFrom_596A70(5);
        tmpScore %= 1000000u;
    }

    if (tmpScore >= 100000)
    {
        if (!this->field_0_numbers_count)
        {
            this->field_0_numbers_count = 5;
        }
        this->field_4[5] = tmpScore / 100000;
        ClearNumbersArrayFrom_596A70(4);
        tmpScore %= 100000u;
    }

    if (tmpScore >= 10000)
    {
        if (!this->field_0_numbers_count)
        {
            this->field_0_numbers_count = 4;
        }
        this->field_4[4] = tmpScore / 10000;
        ClearNumbersArrayFrom_596A70(3);
        tmpScore %= 10000u;
    }

    if (tmpScore >= 1000)
    {
        if (!this->field_0_numbers_count)
        {
            this->field_0_numbers_count = 3;
        }
        this->field_4[3] = tmpScore / 1000;
        ClearNumbersArrayFrom_596A70(2);
        tmpScore %= 1000u;
    }

    if (tmpScore >= 100)
    {
        if (!this->field_0_numbers_count)
        {
            this->field_0_numbers_count = 2;
        }
        this->field_4[2] = tmpScore / 100;
        ClearNumbersArrayFrom_596A70(1);
        tmpScore %= 100u;
    }

    if (tmpScore >= 10)
    {
        if (!this->field_0_numbers_count)
        {
            this->field_0_numbers_count = 1;
        }
        this->field_4[1] = tmpScore / 10;
        this->field_4[0] = tmpScore % 10;
    }
    else
    {
        this->field_4[0] = tmpScore;
    }

    this->field_28_x = xpos;
    this->field_2C_y = ypos;
    this->field_30_z = zpos;

    this->field_34 = gExplodingScorePool->sub_596860();

    this->field_36 = 25;
    this->field_38 = 2;
    this->field_3C = 1;
    this->field_40 = 1;
}

WIP_FUNC(0x596c90)
void ExplodingScore_50::DrawNumbers_596C90()
{
    WIP_IMPLEMENTED;

    Player* field_38_orf1 = gGame_0x40_67E008->field_38_orf1;

    Fix16 a3;
    Fix16 v12;
    Fix16 v6;
    Fix16 v7;
    field_38_orf1->field_14C_view_camera.ProjectWorldToScreen_4B90E0(field_28_x, field_2C_y, field_30_z, &a3, &v12);
    if (a3 < dword_702C6C)
    {
        a3 = dword_702C6C - a3;
        v6 = a3 / dword_702BC4;
    }
    else
    {
        if (!(a3 > dword_702F10))
        {
            goto LABEL_6;
        }
        a3 = dword_702F10 - a3;
        v6 = a3 / dword_702BC4;
    }
    field_28_x += v6;

LABEL_6:
    if (v12 < dword_702C74)
    {
        v12 = dword_702C74 - v12;
        v7 = v12 / dword_702BC4;
    }
    else
    {
        if (!(v12 > dword_702C08))
        {
            goto LABEL_11;
        }
        else
        {
            v12 = dword_702C08 - v12;
            v7 = v12 / dword_702BC4;
        }
    }
    field_2C_y += v7;

LABEL_11:

    switch (this->field_0_numbers_count)
    {
        case 0:
            DrawSingleNumber_597100(this->field_4[0], 10);
            break;

        case 1:
            DrawSingleNumber_597100(this->field_4[1], 21);
            DrawSingleNumber_597100(this->field_4[0], 0);
            break;

        case 2:
            DrawSingleNumber_597100(this->field_4[2], 31);
            DrawSingleNumber_597100(this->field_4[1], 10);
            DrawSingleNumber_597100(this->field_4[0], -11);
            break;

        case 3:
            DrawSingleNumber_597100(this->field_4[3], 42);
            DrawSingleNumber_597100(this->field_4[2], 21);
            DrawSingleNumber_597100(this->field_4[1], 0);
            DrawSingleNumber_597100(this->field_4[0], -21);
            break;

        case 4:
            DrawSingleNumber_597100(this->field_4[4], 52);
            DrawSingleNumber_597100(this->field_4[3], 31);
            DrawSingleNumber_597100(this->field_4[2], 10);
            DrawSingleNumber_597100(this->field_4[1], -11);
            DrawSingleNumber_597100(this->field_4[0], -32);
            break;

        case 5:
            DrawSingleNumber_597100(this->field_4[5], 63);
            DrawSingleNumber_597100(this->field_4[4], 42);
            DrawSingleNumber_597100(this->field_4[3], 21);
            DrawSingleNumber_597100(this->field_4[2], 0);
            DrawSingleNumber_597100(this->field_4[1], -21);
            DrawSingleNumber_597100(this->field_4[0], -42);
            break;

        case 6:
            DrawSingleNumber_597100(this->field_4[6], 73);
            DrawSingleNumber_597100(this->field_4[5], 52);
            DrawSingleNumber_597100(this->field_4[4], 31);
            DrawSingleNumber_597100(this->field_4[3], 10);
            DrawSingleNumber_597100(this->field_4[2], -11);
            DrawSingleNumber_597100(this->field_4[1], -32);
            DrawSingleNumber_597100(this->field_4[0], -53);
            break;

        case 7:
            DrawSingleNumber_597100(this->field_4[7], 84);
            DrawSingleNumber_597100(this->field_4[6], 63);
            DrawSingleNumber_597100(this->field_4[5], 42);
            DrawSingleNumber_597100(this->field_4[4], 21);
            DrawSingleNumber_597100(this->field_4[3], 0);
            DrawSingleNumber_597100(this->field_4[2], -21);
            DrawSingleNumber_597100(this->field_4[1], -42);
            DrawSingleNumber_597100(this->field_4[0], -63);
            break;

        case 8:
            DrawSingleNumber_597100(this->field_4[8], 94);
            DrawSingleNumber_597100(this->field_4[7], 73);
            DrawSingleNumber_597100(this->field_4[6], 52);
            DrawSingleNumber_597100(this->field_4[5], 31);
            DrawSingleNumber_597100(this->field_4[4], 10);
            DrawSingleNumber_597100(this->field_4[3], -11);
            DrawSingleNumber_597100(this->field_4[2], -32);
            DrawSingleNumber_597100(this->field_4[1], -53);
            DrawSingleNumber_597100(this->field_4[0], -74);
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5970c0)
char_type ExplodingScore_50::PoolUpdate()
{
    if (field_38 < 0)
    {
        field_3C++;
        field_40++;

        if (field_36 > 2u)
        {
            field_36 -= 2;
            field_38--;
            return 0;
        }
        else
        {
            field_36 = 0;
            gExplodingScorePool->sub_596880();
            return 1;
        }
    }
    else
    {
        field_38--;
        return 0;
    }
}

// 9.6f 0x4B92B0
WIP_FUNC(0x597100)
void ExplodingScore_50::DrawSingleNumber_597100(s32 number_to_draw, s32 xpos_sub)
{
    WIP_IMPLEMENTED;

    Player* pPlayer = gGame_0x40_67E008->field_38_orf1;

    Fix16 proj_x;
    Fix16 proj_y;

    s32 x_base;
    if (number_to_draw == 1)
    {
        x_base = 4;
    }
    else
    {
        x_base = 0;
    }

    pPlayer->field_14C_view_camera.ProjectWorldToScreen_4B90E0(field_28_x, field_2C_y, field_30_z, &proj_x, &proj_y);

    Fix16 base_scale;
    s32 x_off;
    s32 y_off;
    if (field_38 < 0) // scale factor?
    {
        x_off = this->field_3C * (x_base - xpos_sub);
        y_off = -7 * this->field_40;
        base_scale = Fix16(-field_38);
    }
    else
    {
        x_off = x_base - xpos_sub;
        y_off = -7;
        base_scale = dword_702DE4;
    }

    Fix16 x_to_use = proj_x + Fix16(x_off);
    Fix16 y_to_use = proj_y + Fix16(y_off);

    if (x_to_use >= dword_702DE0)
    {
        if (x_to_use <= Fix16(640))
        {
            if (y_to_use >= dword_702DE0)
            {
                if (y_to_use <= Fix16(480))
                {
                    s32 drawKind = 7;
                    Fix16 finalScale = pPlayer->field_14C_view_camera.field_A8_ui_scale * base_scale;
                    DrawFigure_5D7EC0(6, // type
                                      number_to_draw + 163, // pal
                                      x_to_use, // x
                                      y_to_use, // y
                                      word_702F24, // rot
                                      finalScale, // scale
                                      drawKind, // drawkind
                                      this->field_34,
                                      1,
                                      this->field_36,
                                      1);
                }
            }
        }
    }
}

MATCH_FUNC(0x5967e0)
ExplodingScorePool::ExplodingScorePool()
{
    field_0 = 9;
    field_2_free_scores = 3;
}

MATCH_FUNC(0x596840)
ExplodingScorePool::~ExplodingScorePool()
{
}

MATCH_FUNC(0x596860)
s16 ExplodingScorePool::sub_596860()
{
    field_0++;
    if (field_0 > 13u)
    {
        field_0 = 9;
    }
    return field_0;
}

MATCH_FUNC(0x596880)
void ExplodingScorePool::sub_596880()
{
    field_2_free_scores++;
}

MATCH_FUNC(0x596890)
void ExplodingScorePool::PushScore_596890(Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 score)
{
    if (this->field_2_free_scores == 0)
    {
        ExplodingScore_50* pIter = field_4_pool.field_4_pPrev; // TODO: Inline here 0x4B8FD0 pool template 9.6f
        while (pIter)
        {
            if (pIter->field_4C_score >= score)
            {
                pIter = pIter->mpNext;
            }
            else
            {
                field_4_pool.unknown_func(pIter);
                ++this->field_2_free_scores;
                break;
            }
        }
    }

    if (this->field_2_free_scores > 0)
    {
        ExplodingScore_50* pNew = field_4_pool.Allocate();
        if (pNew)
        {
            --this->field_2_free_scores;
            pNew->InitScore_596A90(xpos, ypos, zpos, score);
        }
    }
}

MATCH_FUNC(0x596940)
void ExplodingScorePool::sub_596940()
{
    field_4_pool.UpdatePool();
}

MATCH_FUNC(0x5969e0)
void ExplodingScorePool::DrawExplodingScores_5969E0()
{
    // TODO: Would there have been an iterator object ??
    ExplodingScore_50* pIter = field_4_pool.field_4_pPrev;
    while (pIter)
    {
        pIter->DrawNumbers_596C90();
        pIter = pIter->mpNext;
    }
}
