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

    // alloc_append_4254A0
    PoolType* AllocateAppend()
    {
        // Pop from free list
        PoolType* obj = field_0_pStart;
        field_0_pStart = obj->mpNext;
        obj->mpNext = NULL;

        // Append to active list (FIFO style)
        if (!field_4_pPrev)
        {
            field_4_pPrev = obj; // active list was empty
        }
        else
        {
            PoolType* tail = field_4_pPrev;
            while (tail->mpNext)
            {
                tail = tail->mpNext;
            }
            tail->mpNext = obj;
        }

        obj->PoolAllocate(); // per-object init
        return obj;
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
    
    void Remove_NoDeAllocate(PoolType* pItem)
    {
        pItem->mpNext = field_0_pStart;
        field_0_pStart = pItem;
    }

    void UpdateNextPrev(PoolType* pItem)
    {
        pItem->mpNext = field_4_pPrev;
        field_4_pPrev = pItem;
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

    PoolType* unknown_func(PoolType* pToFind)
    {
        PoolType* pLast = 0;

        for (PoolType* pIter = field_4_pPrev; pIter; pIter = pIter->mpNext)
        {
            if (pIter == pToFind)
            {
                pIter->PoolDeallocate();

                if (pLast)
                {
                    pLast->mpNext = pIter->mpNext;
                }
                else
                {
                    field_4_pPrev = pIter->mpNext;
                }
                pIter->mpNext = field_0_pStart;
                field_0_pStart = pIter;
                return pToFind;
            }
            pLast = pIter;
        }
        return pToFind;
    }

    void sub_420F30(PoolType* toFind)
    {
        PoolType* pIter = this->field_4_pPrev;
        PoolType* pLast = 0;
        while (pIter)
        {
            if (pIter == toFind)
            {
                if (pLast)
                {
                    pLast->mpNext = pIter->mpNext;
                    pIter->mpNext = 0;
                }
                else
                {
                    this->field_4_pPrev = pIter->mpNext;
                    pIter->mpNext = 0;
                }
                return;
            }

            pLast = pIter;
            pIter = pIter->mpNext;
        }
    }

    PoolType* field_0_pStart;
    PoolType* field_4_pPrev;
    PoolType field_8_pool[PoolSize];

    // Unknown offsets due to sizeof(PoolType) varying per template
    s16 field_X_count;
    s16 field_X_pad;
};
