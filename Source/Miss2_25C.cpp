#include "Miss2_25C.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(Miss2_25C*, gMiss2_25C_6F805C, 0x6F805C);

// FUNCTION: 105 0x502d90
Miss2_25C::Miss2_25C()
{
    field_258_count = 0;
    for (s32 i = 0; i < 50; i++)
    {
        field_0[i].field_0_uni1 = 0;
        field_0[i].field_8_uni2 = 0;
        field_0[i].field_4_type = 0;
    }
}

// STUB: 105 0x502dc0
void Miss2_25C::sub_502DC0()
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x502f60
Miss2_C* Miss2_25C::allocate_next_502F60()
{
    Miss2_C* pIter = field_0;
    for (u16 idx = 0; idx < 50; idx++)
    {
        if (!pIter->field_0_uni1)
        {
            return pIter;
        }
        pIter++;
    }
    return NULL;
}

// FUNCTION: 105 0x502f80
void Miss2_25C::push_type_1_car_502F80(Car_BC* pCar)
{
    Miss2_C* pFree = allocate_next_502F60();
    if (pFree)
    {
        pFree->field_0_uni1 = (int)pCar;
        const s32 id = pCar->field_6C_maybe_id;
        pFree->field_4_type = 1;
        pFree->field_8_uni2 = id;
        field_258_count++;
    }
}

// FUNCTION: 105 0x502fb0
void Miss2_25C::push_type_3_ped_502FB0(Ped* pPed)
{
    Miss2_C* pFree = allocate_next_502F60();
    if (pFree)
    {
        pFree->field_0_uni1 = (int)pPed;
        const s32 v4 = pPed->field_200_id;
        pFree->field_4_type = 3;
        pFree->field_8_uni2 = v4;
        field_258_count++;
    }
}

// FUNCTION: 105 0x502ff0
void Miss2_25C::push_type_2_502FF0(Object_2C* pObj, char_type bUnknown)
{
    if (bUnknown || pObj && ((pObj->field_18_model < 64) || pObj->field_18_model > 108) && (pObj->field_18_model < 200 || pObj->field_18_model > 244))
    {
        Miss2_C* pC = allocate_next_502F60();
        if (pC)
        {
            pC->field_0_uni1 = (int)pObj;
            pC->field_8_uni2 = pObj->field_14;
            pC->field_4_type = 2;
            field_258_count++;
        }
    }
}

// FUNCTION: 105 0x503050
void Miss2_25C::push_type_2_503050(Object_2C* pObj)
{
    Miss2_C* pC = allocate_next_502F60();
    if (pC)
    {
        pC->field_0_uni1 = (int)pObj;
        const s32 v4 = pObj->field_14;
        pC->field_4_type = 2;
        pC->field_8_uni2 = v4;
        field_258_count++;
    }
}