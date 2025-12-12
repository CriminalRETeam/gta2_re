#pragma once
#include "types.hpp"

// TODO: name these better, this "basic" pool has O(1) allocate
// and deallocate.
template<typename PoolType, s32 PoolSize>
class PoolBasic
{
  public:
    PoolBasic()
    {
        PoolType* pIter = &field_4_pool[0];
        for (s32 i = 0; i < PoolSize - 1; i++)
        {
            pIter->mpNext = pIter + 1;
            pIter++;
        }

        field_4_pool[PoolSize - 1].mpNext = NULL;
        field_0_pHead = field_4_pool;
    }

    ~PoolBasic()
    {
        field_0_pHead = 0;
    }

    PoolType* Allocate()
    {
        PoolType* pNew = field_0_pHead;
        field_0_pHead = pNew->mpNext;
        pNew->PoolAllocate();
        return pNew;
    }

    void DeAllocate(PoolType* pItem)
    {
        pItem->PoolDeallocate();
        pItem->mpNext = field_0_pHead;
        field_0_pHead = pItem;
    }

    PoolType* UnlinkAndReturnNext(PoolType* pOldItem)
    {
        PoolType* pNewItem = pOldItem->mpNext;
        pOldItem->mpNext = this->field_0_pHead;
        this->field_0_pHead = pOldItem;
        return pNewItem;
    }

    PoolType* field_0_pHead;
    PoolType field_4_pool[PoolSize];
};

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

    PoolType* Allocate()
    {
        PoolType* tmp = field_0_pStart;
        field_0_pStart = tmp->mpNext;
        tmp->mpNext = field_4_pPrev;
        field_4_pPrev = tmp;
        tmp->PoolAllocate();
        return tmp;
    }

    PoolType* AllocateUntracked()
    {
        PoolType* tmp = this->field_0_pStart;
        this->field_0_pStart = tmp->mpNext;
        tmp->mpNext = 0;
        tmp->PoolAllocate();
        return tmp;
    }

    void DeAllocate(PoolType* pItem)
    {
        pItem->PoolDeallocate();
        pItem->mpNext = field_0_pStart;
        field_0_pStart = pItem;
    }

    void UpdatePool()
    {
        field_X_count = 0;

        PoolType* pPreviousItem = NULL;
        PoolType* pCurrItem = field_4_pPrev;

        while (pCurrItem)
        {
            ++field_X_count;

            PoolType* pNext = pCurrItem->mpNext;

            if (pCurrItem->PoolUpdate())
            {
                pCurrItem->PoolDeallocate();

                if (pPreviousItem && pPreviousItem->mpNext != pCurrItem)
                {
                    pPreviousItem = NULL;
                }

                if (pPreviousItem)
                {
                    pPreviousItem->mpNext = pCurrItem->mpNext;
                }
                else
                {
                    if (field_4_pPrev == pCurrItem)
                    {
                        field_4_pPrev = pCurrItem->mpNext;
                    }
                    else
                    {
                        pPreviousItem = field_4_pPrev;
                        while (pPreviousItem->mpNext != pCurrItem)
                        {
                            pPreviousItem = pPreviousItem->mpNext;
                        }
                        pPreviousItem->mpNext = pCurrItem->mpNext;
                    }
                }

                pCurrItem->mpNext = field_0_pStart;
                field_0_pStart = pCurrItem;
            }
            else
            {
                pPreviousItem = pCurrItem;
            }

            pCurrItem = pNext;
        }
    }

    void UpdatePoolNoDeallocate()
    {
        field_X_count = 0;

        PoolType* pPreviousItem = NULL;
        PoolType* pCurrItem = field_4_pPrev;

        while (pCurrItem)
        {
            ++field_X_count;

            PoolType* pNext = pCurrItem->mpNext;

            if (pCurrItem->PoolUpdate())
            {
                if (pPreviousItem && pPreviousItem->mpNext != pCurrItem)
                {
                    pPreviousItem = NULL;
                }

                if (pPreviousItem)
                {
                    pPreviousItem->mpNext = pCurrItem->mpNext;
                }
                else
                {
                    if (field_4_pPrev == pCurrItem)
                    {
                        field_4_pPrev = pCurrItem->mpNext;
                    }
                    else
                    {
                        pPreviousItem = field_4_pPrev;
                        while (pPreviousItem->mpNext != pCurrItem)
                        {
                            pPreviousItem = pPreviousItem->mpNext;
                        }
                        pPreviousItem->mpNext = pCurrItem->mpNext;
                    }
                }

                pCurrItem->mpNext = field_0_pStart;
                field_0_pStart = pCurrItem;
            }
            else
            {
                pPreviousItem = pCurrItem;
            }

            pCurrItem = pNext;
        }
    }

    PoolType* unknown_func(PoolType* pObj)
    {
        PoolType* v18 = 0;

        for (PoolType* pFirst = field_4_pPrev; pFirst; pFirst = pFirst->mpNext)
        {
            if (pFirst == pObj)
            {
                pFirst->PoolDeallocate();
    
                if (v18)
                {
                    v18->mpNext = pFirst->mpNext;
                   
                }
                else
                {
                    field_4_pPrev = pFirst->mpNext;
                }
                pFirst->mpNext = field_0_pStart;
                field_0_pStart = pFirst;
                return pObj;
            }
            v18 = pFirst;
        }
        return pObj;
    }

    PoolType* field_0_pStart;
    PoolType* field_4_pPrev;
    PoolType field_8_pool[PoolSize];

    // Unknown offsets due to sizeof(PoolType) varying per template
    s16 field_X_count;
    s16 field_X_pad;
};
