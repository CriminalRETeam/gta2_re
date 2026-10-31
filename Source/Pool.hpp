#pragma once
#include "types.hpp"

// A free list only, with O(1) allocate and deallocate: unlike Pool, no list of the used items
template<typename PoolType, s32 PoolSize>
class PoolBasic
{
  public:
    __forceinline PoolBasic()
    {
        PoolType* pIter = &mPool[0];
        for (s32 i = 0; i < PoolSize - 1; i++)
        {
            pIter->mpNext = pIter + 1;
            pIter++;
        }

        mPool[PoolSize - 1].mpNext = NULL;
        mpHead = mPool;
    }

    /*~PoolBasic()
    {
        mpHead = 0;
    }*/

    PoolType* Allocate()
    {
        PoolType* pNew = mpHead;
        mpHead = pNew->mpNext;
        pNew->PoolAllocate();
        return pNew;
    }

    void DeAllocate(PoolType* pItem)
    {
        pItem->PoolDeallocate();
        pItem->mpNext = mpHead;
        mpHead = pItem;
    }

    PoolType* UnlinkAndReturnNext(PoolType* pOldItem)
    {
        PoolType* pNewItem = pOldItem->mpNext;
        pOldItem->mpNext = this->mpHead;
        this->mpHead = pOldItem;
        return pNewItem;
    }

    PoolType* mpHead; // 0x0
    PoolType mPool[PoolSize]; // 0x4
};

template<typename PoolType, s32 PoolSize>
class Pool
{
  public:
    // 9.6f 0x48A8F0
    inline bool has_pStart_48A8F0()
    {
        return mpStart != NULL;
    }

    // 9.6f 0x4B8FD0 (CokeZero_FC, 10.5 copy 0x5935C0): first used item
    inline PoolType* GetFirstUsed_4B8FD0()
    {
        return mpPrev;
    }

    __forceinline Pool()
    {
        PoolType* pIter = mPool;
        for (s32 i = 0; i < PoolSize - 1; i++)
        {
            pIter->mpNext = pIter + 1;
            pIter++;
        }

        mPool[PoolSize - 1].mpNext = NULL;
        mpStart = mPool;

        mpPrev = NULL;
        field_X_count = 0;
    }

    ~Pool()
    {
        mpStart = NULL;
        mpPrev = NULL;
    }

    PoolType* Allocate()
    {
        PoolType* pNew = mpStart;
        mpStart = pNew->mpNext;
        pNew->mpNext = mpPrev;
        mpPrev = pNew;
        pNew->PoolAllocate();
        return pNew;
    }

    // alloc_append_4254A0
    PoolType* AllocateAppend()
    {
        // Pop from free list
        PoolType* obj = mpStart;
        mpStart = obj->mpNext;
        obj->mpNext = NULL;

        // Append to active list (FIFO style)
        if (!mpPrev)
        {
            mpPrev = obj; // active list was empty
        }
        else
        {
            PoolType* tail = mpPrev;
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
        PoolType* pNew = this->mpStart;
        this->mpStart = pNew->mpNext;
        pNew->mpNext = 0;
        pNew->PoolAllocate();
        return pNew;
    }

    void DeAllocate(PoolType* pItem)
    {
        pItem->PoolDeallocate();
        pItem->mpNext = mpStart;
        mpStart = pItem;
    }
    
    void Remove_NoDeAllocate(PoolType* pItem)
    {
        pItem->mpNext = mpStart;
        mpStart = pItem;
    }

    void UpdateNextPrev(PoolType* pItem)
    {
        pItem->mpNext = mpPrev;
        mpPrev = pItem;
    }

    void UpdatePool()
    {
        field_X_count = 0;

        PoolType* pPreviousItem = NULL;
        PoolType* pCurrItem = mpPrev;

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
                    if (mpPrev == pCurrItem)
                    {
                        mpPrev = pCurrItem->mpNext;
                    }
                    else
                    {
                        pPreviousItem = mpPrev;
                        while (pPreviousItem->mpNext != pCurrItem)
                        {
                            pPreviousItem = pPreviousItem->mpNext;
                        }
                        pPreviousItem->mpNext = pCurrItem->mpNext;
                    }
                }

                pCurrItem->mpNext = mpStart;
                mpStart = pCurrItem;
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
        PoolType* pCurrItem = mpPrev;

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
                    if (mpPrev == pCurrItem)
                    {
                        mpPrev = pCurrItem->mpNext;
                    }
                    else
                    {
                        pPreviousItem = mpPrev;
                        while (pPreviousItem->mpNext != pCurrItem)
                        {
                            pPreviousItem = pPreviousItem->mpNext;
                        }
                        pPreviousItem->mpNext = pCurrItem->mpNext;
                    }
                }

                pCurrItem->mpNext = mpStart;
                mpStart = pCurrItem;
            }
            else
            {
                pPreviousItem = pCurrItem;
            }

            pCurrItem = pNext;
        }
    }

    PoolType* FindAndDeAllocate(PoolType* pToFind)
    {
        PoolType* pLast = NULL;

        for (PoolType* pIter = mpPrev; pIter; pIter = pIter->mpNext)
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
                    mpPrev = pIter->mpNext;
                }
                pIter->mpNext = mpStart;
                mpStart = pIter;
                return pToFind;
            }
            pLast = pIter;
        }
        return pToFind;
    }

    void UnlinkFromActiveList_420F30(PoolType* toFind)
    {
        PoolType* pLast = 0;
        for (PoolType* pIter = this->mpPrev; pIter; pIter = pIter->mpNext)
        {
            if (pIter == toFind)
            {
                if (pLast)
                {
                    pLast->mpNext = pIter->mpNext;
                }
                else
                {
                    this->mpPrev = pIter->mpNext;
                }
                pIter->mpNext = 0;
                return;
            }
            pLast = pIter;
        }
    }

    PoolType* mpStart; // 0x0
    PoolType* mpPrev; // 0x4
    PoolType mPool[PoolSize]; // 0x8

    // Unknown offsets due to sizeof(PoolType) varying per template
    s16 field_X_count;
    s16 field_X_pad;
};
