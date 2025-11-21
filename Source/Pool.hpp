#pragma once
#include "types.hpp"

template<typename PoolType, s32 PoolSize>
class Pool
{
  public:
    Pool()
    {
        PoolType* pIter = field_8_pool;
        for (s32 i = 0; i < PoolSize - 1; i++)
        {
            pIter->mpNext = pIter + 1;
            pIter++;
        }

        field_8_pool[PoolSize - 1].mpNext = 0;
        field_0_pStart = field_8_pool;

        field_4_pPrev = 0;
        field_X_count = 0;
    }

    ~Pool()
    {
        field_0_pStart = 0;
        field_4_pPrev = 0;
    }

    PoolType* New()
    {
        PoolType* tmp = field_0_pStart;
        field_0_pStart = tmp->mpNext;
        tmp->mpNext = field_4_pPrev;
        field_4_pPrev = tmp;
        tmp->PoolInit();
        return tmp;
    }

    PoolType* field_0_pStart;
    PoolType* field_4_pPrev;
    PoolType field_8_pool[PoolSize];

    // Unknown offsets due to sizeof(PoolType) varying per template
    s16 field_X_count;
    s16 field_X_pad;
};
