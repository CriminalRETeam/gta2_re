#include "Miss2_25C.hpp"
#include "miss2_0x11C.hpp"
#include "Car_BC.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"

DEFINE_GLOBAL(Miss2_25C*, gMiss2_25C_6F805C, 0x6F805C);

MATCH_FUNC(0x502d90)
Miss2_25C::Miss2_25C()
{
    field_258_count = 0;
    for (s32 i = 0; i < 50; i++)
    {
        field_0[i].field_0_obj = 0;
        field_0[i].field_8_uni2 = 0;
        field_0[i].field_4_type = 0;
    }
}

MATCH_FUNC(0x502dc0)
void Miss2_25C::MissionCleanUp_502DC0()
{
    Miss2_C* pIter = &field_0[0];
    PedGroup* pGroup = gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->field_164_ped_group;
    if (pGroup)
    {
        pGroup->DestroyGroup_4C93A0();
    }

    if (gStoredCar_6F7560)
    {
        if (gStoredCar_6F7560->field_6C_maybe_id == gStoredCarId_6F78B4)
        {
            gGame_0x40_67E008->field_38_orf1->PushCarInfo_564680(gStoredCar_6F7560);
            gStoredCar_6F7560->sub_421560(3);
            if (!gStoredCar_6F7560->sub_4214D0())
            {
                gStoredCar_6F7560->sub_421470();
            }
            gStoredCar_6F7560 = 0;
            gStoredCarId_6F78B4 = 0;
        }
    }

    for (s32 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        if ((u32*)pIter->field_0_car != NULL)
        {
            switch (pIter->field_4_type)
            {
                case 2:
                    if (pIter->field_0_obj->field_14_id == pIter->field_8_uni2)
                    {
                        pIter->field_0_obj->Dealloc_5291B0();
                        pIter->field_0_obj->sub_5290B0();
                    }
                    break;
                case 3:
                    if (pIter->field_0_ped->field_200_id == pIter->field_8_uni2)
                    {
                        PedGroup* pPedGroup = pIter->field_0_ped->field_164_ped_group;
                        if (pPedGroup)
                        {
                            pPedGroup->sub_4C92A0();
                        }
                        pIter->field_0_ped->ForceDoNothing_462590();
                        pIter->field_0_ped->field_21C_bf.b10 = true;
                    }
                    break;
                case 1:
                    if (pIter->field_0_car->field_6C_maybe_id == pIter->field_8_uni2)
                    {
                        gGame_0x40_67E008->field_38_orf1->PushCarInfo_564680(pIter->field_0_car);
                        pIter->field_0_car->sub_421560(3);
                        if (!pIter->field_0_car->sub_4214D0())
                        {
                            pIter->field_0_car->sub_421470();
                        }
                    }
                    break;
                default:
                    break;
            }
            pIter->field_0_car = NULL;
            pIter->field_8_uni2 = 0;
            pIter->field_4_type = 0;
            --field_258_count;
        }
        pIter++;
    }
    gfrosty_pasteur_6F8060->field_C1E70 = 87;
}

MATCH_FUNC(0x502f60)
Miss2_C* Miss2_25C::allocate_next_502F60()
{
    Miss2_C* pIter = field_0;
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
void Miss2_25C::push_type_1_car_502F80(Car_BC* pCar)
{
    Miss2_C* pFree = allocate_next_502F60();
    if (pFree)
    {
        pFree->field_0_car = pCar;
        const s32 id = pCar->field_6C_maybe_id;
        pFree->field_4_type = 1;
        pFree->field_8_uni2 = id;
        field_258_count++;
    }
}

MATCH_FUNC(0x502fb0)
void Miss2_25C::push_type_3_ped_502FB0(Ped* pPed)
{
    Miss2_C* pFree = allocate_next_502F60();
    if (pFree)
    {
        pFree->field_0_ped = pPed;
        const s32 v4 = pPed->field_200_id;
        pFree->field_4_type = 3;
        pFree->field_8_uni2 = v4;
        field_258_count++;
    }
}

MATCH_FUNC(0x502ff0)
void Miss2_25C::push_type_2_502FF0(Object_2C* pObj, char_type bUnknown)
{
    if (bUnknown || pObj && ((pObj->field_18_model < 64) || pObj->field_18_model > 108) && (pObj->field_18_model < 200 || pObj->field_18_model > 244))
    {
        Miss2_C* pC = allocate_next_502F60();
        if (pC)
        {
            pC->field_0_obj = pObj;
            pC->field_8_uni2 = pObj->field_14_id;
            pC->field_4_type = 2;
            field_258_count++;
        }
    }
}

MATCH_FUNC(0x503050)
void Miss2_25C::push_type_2_503050(Object_2C* pObj)
{
    Miss2_C* pC = allocate_next_502F60();
    if (pC)
    {
        pC->field_0_obj = pObj;
        const s32 v4 = pObj->field_14_id;
        pC->field_4_type = 2;
        pC->field_8_uni2 = v4;
        field_258_count++;
    }
}