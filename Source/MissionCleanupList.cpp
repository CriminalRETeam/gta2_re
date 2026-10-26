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
    field_258_count = 0;
    for (s32 i = 0; i < 50; i++)
    {
        field_0_entries[i].field_0_obj = NULL;
        field_0_entries[i].field_8_entity_id = 0;
        field_0_entries[i].field_4_type = 0;
    }
}

MATCH_FUNC(0x502f60)
MissionCleanupEntry* MissionCleanupList::allocate_next_502F60()
{
    MissionCleanupEntry* pIter = field_0_entries;
    for (u16 idx = 0; idx < 50; idx++)
    {
        if (!pIter->field_0_obj)
        {
            return pIter;
        }
        pIter++;
    }
    return NULL;
}

MATCH_FUNC(0x502f80)
void MissionCleanupList::push_type_1_car_502F80(Car* pCar)
{
    MissionCleanupEntry* pFree = allocate_next_502F60();
    if (pFree)
    {
        pFree->field_0_car = pCar;
        const s32 id = pCar->field_6C_car_id;
        pFree->field_4_type = 1;
        pFree->field_8_entity_id = id;
        field_258_count++;
    }
}

MATCH_FUNC(0x502fb0)
void MissionCleanupList::push_type_3_ped_502FB0(Ped* pPed)
{
    MissionCleanupEntry* pFree = allocate_next_502F60();
    if (pFree)
    {
        pFree->field_0_ped = pPed;
        const s32 entity_id = pPed->field_200_id;
        pFree->field_4_type = 3;
        pFree->field_8_entity_id = entity_id;
        field_258_count++;
    }
}

MATCH_FUNC(0x502ff0)
void MissionCleanupList::push_type_2_502FF0(Object* pObj, char_type bForce)
{
    if (bForce || pObj && ((pObj->GetModel_40FEF0() < 64) || pObj->GetModel_40FEF0() > 108) && (pObj->GetModel_40FEF0() < 200 || pObj->GetModel_40FEF0() > 244))
    {
        MissionCleanupEntry* pEntry = allocate_next_502F60();
        if (pEntry)
        {
            pEntry->field_0_obj = pObj;
            pEntry->field_8_entity_id = pObj->field_14_id;
            pEntry->field_4_type = 2;
            field_258_count++;
        }
    }
}

MATCH_FUNC(0x503050)
void MissionCleanupList::push_type_2_503050(Object* pObj)
{
    MissionCleanupEntry* pEntry = allocate_next_502F60();
    if (pEntry)
    {
        pEntry->field_0_obj = pObj;
        const s32 entity_id = pObj->field_14_id;
        pEntry->field_4_type = 2;
        pEntry->field_8_entity_id = entity_id;
        field_258_count++;
    }
}