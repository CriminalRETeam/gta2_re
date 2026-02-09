#include "ExplodingScore_100.hpp"
#include "Globals.hpp"
#include <memory.h>

DEFINE_GLOBAL(ExplodingScorePool*, gExplodingScorePool, 0x702F34);

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

STUB_FUNC(0x596c90)
void ExplodingScore_50::DrawNumbers_596C90()
{
    NOT_IMPLEMENTED;
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

STUB_FUNC(0x597100)
void ExplodingScore_50::DrawSingleNumber_597100(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
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
