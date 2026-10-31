#pragma once

#include "Function.hpp"
#include "Ped.hpp"
#include "Pool.hpp"
#include "char.hpp"

class Camera;

class CharPool
{
  public:
    __forceinline CharPool()
    {
    }

    EXPORT ~CharPool();

    // inline 0x4355C0
    void DeAllocate(Char* pChar)
    {
        mPool.DeAllocate(pChar);
    }

    PoolBasic<Char, 400> mPool; // 0x0
};

class PedListNode
{
  public:
    // 9.6f 0x420EA0
    inline bool no_char_ped_420EA0() const
    {
        return mCharPed == NULL;
    }

    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    Ped* mCharPed; // 0x0
    PedListNode* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(PedListNode, 0x8);

class PedListNodePool
{
  public:
    PoolBasic<PedListNode, 100> mPool; // 0x0

    ~PedListNodePool()
    {
        mPool.mpHead = NULL;
    }

    // 9.6f 0x445EF0
    inline PedListNode* Allocate()
    {
        return mPool.Allocate();
    }

    // 9.6f 0x445F00
    inline void DeAllocate(PedListNode* pItem)
    {
        mPool.DeAllocate(pItem);
    }

    __forceinline PedListNodePool()
    {
    }
};

class PedManager
{
  private:
    EXPORT void SpawnDummies(Camera* pCam);

  public:
    STATIC_MEMBER(u8, sSpawnSideLocked);
    STATIC_MEMBER(u8, sSpawnCountLimit);
    STATIC_MEMBER(s8, sSpawnSide);
    STATIC_MEMBER(s8, sSpawnIndex);
    STATIC_MEMBER(u16, sPedsServiceTickCount);
    EXPORT void PedsService();
    EXPORT PedManager();
    EXPORT ~PedManager();
    EXPORT Ped* SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation);
    EXPORT Ped* SpawnDriver(Car* pCar);
    EXPORT Ped* SpawnGangDriver(Car* pCar, Gang* pGang);
    EXPORT Ped* CreateDummyDriver(Car* pCar);
    EXPORT Ped* SpawnRunAwayGuy();
    EXPORT Ped* SpawnTrainLeaver();
    EXPORT Ped* AllocatePed();
    EXPORT Ped* ClonePed(Ped* pSrc);
    EXPORT void DoIanTest(u16 key);
    EXPORT Ped* PedById(s32 pedId);

  private:
    EXPORT void Dummies();

  public:
    s16 mMaxDummyChars; // 0x0
    char_type mNumDummyChars; // 0x2
    char_type mNumPedsUpdated; // 0x3
    char_type mNumScriptCreatedPeds; // 0x4
    u8 mFbiArmyCount; // 0x5
    char_type mNumPedsOnScreen; // 0x6
    char_type mMakeAllMuggers; // 0x7
    Sprite* mSpawnProbeSprite; // 0x8
};

class PedPool
{
  public:
    // __forceinline on the pool ctors: PedManager::PedManager (0x470650) runs out of inline
    // expansions on the Fix16 argument conversions but still expands these
    __forceinline PedPool()
    {
    }

    EXPORT ~PedPool();

    // 9.6f 0x403890
    Ped* Allocate()
    {
        return mPool.Allocate();
    }

    // 9.6f 0x445960
    // Pool::UpdatePool with the next item stored in both branches
    void UpdatePool()
    {
        mPool.field_X_count = 0;

        Ped* pPreviousItem = NULL;
        Ped* pCurrItem = mPool.mpPrev;

        while (pCurrItem)
        {
            ++mPool.field_X_count;

            Ped* pNext = pCurrItem->mpNext;

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
                    if (mPool.mpPrev == pCurrItem)
                    {
                        mPool.mpPrev = pCurrItem->mpNext;
                    }
                    else
                    {
                        pPreviousItem = mPool.mpPrev;
                        while (pPreviousItem->mpNext != pCurrItem)
                        {
                            pPreviousItem = pPreviousItem->mpNext;
                        }
                        pPreviousItem->mpNext = pCurrItem->mpNext;
                    }
                }

                pCurrItem->mpNext = mPool.mpStart;
                mPool.mpStart = pCurrItem;
                pCurrItem = pNext;
            }
            else
            {
                pPreviousItem = pCurrItem;
                pCurrItem = pNext;
            }
        }
    }

    // 9.6f 0x435530
    inline Ped* GetFirstPed() const
    {
        return mPool.mpPrev;
    }

    Pool<Ped, 200> mPool; // 0x0
};

EXTERN_GLOBAL(PedManager*, gPedManager);

EXTERN_GLOBAL(PedPool*, gPedPool);

EXTERN_GLOBAL(CharPool*, gChar_B4_Pool);

EXTERN_GLOBAL(PedListNodePool*, gChar_8_Pool);

EXTERN_GLOBAL(u8, gNumPolicePedsInRangeScreen);

EXTERN_GLOBAL(u8, gLosCheckSlotFree);
