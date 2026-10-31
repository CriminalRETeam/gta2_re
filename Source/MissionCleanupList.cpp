#include "MissionCleanupList.hpp"
#include "ScriptThread.hpp"
#include "Car.hpp"
#include "ScriptManager.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"

DEFINE_GLOBAL(MissionCleanupList*, gMiss2_25C_6F805C, 0x6F805C);

MATCH_FUNC(0x502d90)
MissionCleanupList::MissionCleanupList() throw()
{
    mCount = 0;
    for (s32 i = 0; i < 50; i++)
    {
        mEntries[i].mObj = NULL;
        mEntries[i].mEntityId = 0;
        mEntries[i].mType = cleanup_entry_type::empty_0;
    }
}

MATCH_FUNC(0x502f60)
MissionCleanupEntry* MissionCleanupList::allocate_next()
{
    MissionCleanupEntry* pIter = mEntries;
    for (u16 idx = 0; idx < 50; idx++)
    {
        if (!pIter->mObj)
        {
            return pIter;
        }
        pIter++;
    }
    return NULL;
}

MATCH_FUNC(0x502f80)
void MissionCleanupList::push_type_1_car(Car* pCar)
{
    MissionCleanupEntry* pFree = allocate_next();
    if (pFree)
    {
        pFree->mCar = pCar;
        const s32 id = pCar->mCarId;
        pFree->mType = cleanup_entry_type::car_1;
        pFree->mEntityId = id;
        mCount++;
    }
}

MATCH_FUNC(0x502fb0)
void MissionCleanupList::push_type_3_ped(Ped* pPed)
{
    MissionCleanupEntry* pFree = allocate_next();
    if (pFree)
    {
        pFree->mPed = pPed;
        const s32 entity_id = pPed->mId;
        pFree->mType = cleanup_entry_type::ped_3;
        pFree->mEntityId = entity_id;
        mCount++;
    }
}

MATCH_FUNC(0x502ff0)
void MissionCleanupList::push_type_2_non_pickup_object(Object* pObj, bool bForce)
{
    if (bForce || pObj && ((pObj->GetModel_40FEF0() < 64) || pObj->GetModel_40FEF0() > 108) && (pObj->GetModel_40FEF0() < 200 || pObj->GetModel_40FEF0() > 244))
    {
        MissionCleanupEntry* pEntry = allocate_next();
        if (pEntry)
        {
            pEntry->mObj = pObj;
            pEntry->mEntityId = pObj->mId;
            pEntry->mType = cleanup_entry_type::object_2;
            mCount++;
        }
    }
}

MATCH_FUNC(0x503050)
void MissionCleanupList::push_type_2_object(Object* pObj)
{
    MissionCleanupEntry* pEntry = allocate_next();
    if (pEntry)
    {
        pEntry->mObj = pObj;
        const s32 entity_id = pObj->mId;
        pEntry->mType = cleanup_entry_type::object_2;
        pEntry->mEntityId = entity_id;
        mCount++;
    }
}