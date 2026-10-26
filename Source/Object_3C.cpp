// GetLength_41E260 compares against this TU's zero
#define FIX16_POINT_ZERO kFpZero_6F8E10
#include "Object_3C.hpp"
#include "Car.hpp"
#include "char.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Explosion.hpp"
#include "enums.hpp"
#include "rng.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL_INIT(Fix16, dword_6F8BF0, Fix16(255), 0x6F8BF0);
DEFINE_GLOBAL(Ang16, kZeroAng_6F8F68, 0x6F8F68);
DEFINE_GLOBAL(SpriteList, stru_67727C, 0x67727C);

EXTERN_GLOBAL(Fix16, kFpOne64th_6F8ECC);

// 10.5 https://decomp.me/scratch/kj3y3
// 9.6f 0x482D90
MATCH_FUNC(0x521FD0)
void Object_3C::GetMovementSpeedAndAngle_521FD0(Fix16& Speed, Ang16& Angle)
{
    s8 conveyor_x;
    s8 conveyor_y;
    Fix16_Point point;
    Fix16_Point conveyor_vel;
    if (field_38_conveyor_speed)
    {
        UnpackSignedNibbles_529050(field_38_conveyor_speed, &conveyor_x, &conveyor_y);
        conveyor_vel.x = kFpOne64th_6F8ECC * conveyor_x;
        conveyor_vel.y = kFpOne64th_6F8ECC * conveyor_y;
        point = conveyor_vel + GetSpeedVector_482BA0();

        Speed = point.GetLength_41E260();
        Angle = point.atan2_40F790();
        ClearConveyorSpeed_482BD0();
    }
    else
    {
        Speed = field_C_speed;
        Angle = field_4_angle;
    }
}

MATCH_FUNC(0x52ad80)
Object_3C::Object_3C()
{
    this->field_4_angle = 0;
    this->mpNext = NULL;
    this->field_1C_z_accel = dword_6F8BF0;
    this->field_18_friction = dword_6F8BF0;
    this->field_10_z_speed = dword_6F8BF0;
    this->field_C_speed = dword_6F8BF0;
    this->field_4_angle = kZeroAng_6F8F68;
    this->field_28_next_definition_timer = 0;
    this->field_20_obj2c_id = 0;
    this->field_2C_unused = 0;
    this->field_2A_bAirborne = 0;
    this->field_38_conveyor_speed = 0;
    this->field_34 = 2;
    this->field_2E_unused = 0;
    this->field_2F_bOnSlope = false;
}

MATCH_FUNC(0x52ade0)
Object_3C::~Object_3C()
{
    mpNext = NULL;
}

// 9.6f 0x482BA0
MATCH_FUNC(0x52adf0)
Fix16_Point Object_3C::GetSpeedVector_52ADF0()
{
    Fix16_Point p;
    p.FromPolar_41E210(field_C_speed, field_4_angle);
    return p;
}

MATCH_FUNC(0x5a6a50)
AttachedSprite* SpriteList::GetSpriteForModel_5A6A50(s32 obj_type)
{
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        Object* pObject = pIter->field_0_pSprite->AsObject2C_40FEC0();
        if (pObject)
        {
            if (pObject->field_18_model == obj_type)
            {
                return pIter;
            }
        }
        pIter = pIter->mpNext;
    }
    return NULL;
}

MATCH_FUNC(0x5a6a90)
Object* SpriteList::FindObject2CByModel_5A6A90(s32 obj_type)
{
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        const s32 sprite_type_enum = pIter->field_0_pSprite->field_30_sprite_type_enum;
        if (sprite_type_enum == sprite_types_enum::code_obj1_4 || sprite_type_enum == sprite_types_enum::map_obj_5 ||
            sprite_type_enum == sprite_types_enum::object_1)
        {
            Object* pObject = pIter->field_0_pSprite->field_8_object_2C_ptr;
            if (pObject)
            {
                if (pObject->field_18_model == obj_type)
                {
                    return pObject;
                }
            }
        }
        pIter = pIter->mpNext;
    }
    return NULL;
}

MATCH_FUNC(0x5a6ad0)
AttachedSprite* SpriteList::FindFirstFireExplosion_5A6AD0()
{
    AttachedSprite* pObjIter = this->field_0_p18;
    while (pObjIter)
    {
        const s32 sprite_type = pObjIter->field_0_pSprite->field_30_sprite_type_enum;
        if (sprite_type == sprite_types_enum::code_obj1_4 || sprite_type == sprite_types_enum::map_obj_5 || sprite_type == sprite_types_enum::object_1)
        {
            Object* pObject = pObjIter->field_0_pSprite->field_8_object_2C_ptr;
            if (pObject)
            {
                if (pObject->IsFireExplosion_525AC0())
                {
                    return pObjIter;
                }
            }
        }
        pObjIter = pObjIter->mpNext;
    }
    return NULL;
}

MATCH_FUNC(0x5a6b10)
void SpriteList::RemoveSprite_5A6B10(Sprite* toFind)
{
    AttachedSprite* pLast = NULL;
    AttachedSprite* pIter = this->field_0_p18;
    for (;;)
    {
        if (pIter->field_0_pSprite == toFind)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
            }
            else
            {
                this->field_0_p18 = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
            }
            return;
        }
        pLast = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x5a6b60)
void SpriteList::RemoveSpriteSafe_5A6B60(Sprite* toFind)
{
    AttachedSprite* pLast = NULL;
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        if (pIter->field_0_pSprite == toFind)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
            }
            else
            {
                this->field_0_p18 = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
            }
            return;
        }
        pLast = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x5a6bb0)
void SpriteList::DestroyAllAttached_5A6BB0()
{
    for (AttachedSprite* pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        pIter->DestroyAttached_5A69E0();
    }
}

MATCH_FUNC(0x5a6bd0)
void SpriteList::SinkAllAttachedCars_5A6BD0()
{
    for (AttachedSprite* pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        pIter->SinkAttachedCar_5A6A20();
    }
}

MATCH_FUNC(0x5a6bf0)
void SpriteList::DispatchCarImpactEvents_5A6BF0(Sprite* pSprite)
{
    for (AttachedSprite* pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        pIter->field_0_pSprite->ProcessCarToCarImpactIfCar_59E910(pSprite);
    }
}

MATCH_FUNC(0x5a6c10)
bool SpriteList::TagSpriteWithRng_5A6C10(Sprite* toFind)
{
    if (field_0_p18 != NULL)
    {
        for (AttachedSprite* pNext = field_0_p18; pNext != NULL; pNext = pNext->mpNext)
        {
            if (pNext->field_0_pSprite == toFind)
            {
                pNext->field_14_rng = gpRng_67AB34->get_cur_rng_41CFE0();
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x5a6c40)
void SpriteList::RemoveAllExceptRngValue_5A6C40(s32 toFind)
{
    AttachedSprite* pIter = this->field_0_p18;
    AttachedSprite* pLast = NULL;

    while (pIter)
    {
        if (pIter->field_14_rng == toFind)
        {
            pLast = pIter;
            pIter = pIter->mpNext;
        }
        else
        {
            AttachedSprite* pSavedNext = pIter->mpNext;
            if (pLast)
            {
                pLast->mpNext = pSavedNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
                pIter = pLast->mpNext;
            }
            else
            {
                gAttachedSpritePool_703B80->DeAllocate(pIter);
                pIter = pSavedNext;
                this->field_0_p18 = pSavedNext;
            }
        }
    }
}

MATCH_FUNC(0x5a6ca0)
Sprite* SpriteList::FirstSpriteOfType_5A6CA0(s32 sprite_type)
{
    if (field_0_p18 != NULL)
    {
        for (AttachedSprite* pNext = field_0_p18; pNext != NULL; pNext = pNext->mpNext)
        {
            if (pNext->field_0_pSprite->field_30_sprite_type_enum == sprite_type)
            {
                return pNext->field_0_pSprite;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x5a6cd0)
void SpriteList::AddSprite_5A6CD0(Sprite* pSprite)
{
    AttachedSprite* pAttached = gAttachedSpritePool_703B80->Allocate();
    pAttached->field_0_pSprite = pSprite;
    pAttached->mpNext = this->field_0_p18;
    pAttached->field_8_offset_x = 0;
    pAttached->field_C_offset_y = 0;
    field_0_p18 = pAttached;
}

MATCH_FUNC(0x5a6d00)
void SpriteList::PushAttachedSprite_5A6D00(Sprite* pSprite1, Fix16 x, Fix16 y, Ang16 angle)
{
    AttachedSprite* pAttached = gAttachedSpritePool_703B80->Allocate();
    pAttached->field_0_pSprite = pSprite1;
    pAttached->mpNext = field_0_p18;
    pAttached->field_8_offset_x = x;
    pAttached->field_C_offset_y = y;
    pAttached->field_10_offset_rot = angle;
    field_0_p18 = pAttached;
}

MATCH_FUNC(0x5a6d40)
void SpriteList::PushSprite_5A6D40(Sprite* pToFind)
{
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        if (pIter->field_0_pSprite == pToFind)
        {
            return;
        }
        pIter = pIter->mpNext;
    }

    AttachedSprite* pNew = gAttachedSpritePool_703B80->Allocate();
    pNew->field_0_pSprite = pToFind;

    pNew->mpNext = this->field_0_p18;
    this->field_0_p18 = pNew;
}

MATCH_FUNC(0x5a6d80)
bool SpriteList::SpriteExists_5A6D80(Sprite* pToFind)
{
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        if (pIter->field_0_pSprite == pToFind)
        {
            return true;
        }
        pIter = pIter->mpNext;
    }
    return false;
}

MATCH_FUNC(0x5a6da0)
Sprite* SpriteList::PopFrontSprite_5A6DA0()
{
    AttachedSprite* pAttached = this->field_0_p18;
    if (!this->field_0_p18)
    {
        return NULL;
    }
    Sprite* pOld = pAttached->field_0_pSprite;
    this->field_0_p18 = pAttached->mpNext;
    gAttachedSpritePool_703B80->DeAllocate(pAttached);
    return pOld;
}

MATCH_FUNC(0x5a6dc0)
Sprite* SpriteList::PopBackSprite_5A6DC0()
{
    AttachedSprite* pIter = this->field_0_p18;
    AttachedSprite* pLast = NULL;
    while (pIter->mpNext)
    {
        pLast = pIter;
        pIter = pIter->mpNext;
    }

    Sprite* result = pIter->field_0_pSprite;
    if (pLast)
    {
        pLast->mpNext = NULL;
    }
    else
    {
        this->field_0_p18 = NULL;
    }
    gAttachedSpritePool_703B80->DeAllocate(pIter);

    return result;
}

MATCH_FUNC(0x5a6e10)
AttachedSprite* SpriteList::ClearList_5A6E10()
{
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        AttachedSprite* pLast = pIter;
        pIter = pIter->mpNext;
        gAttachedSpritePool_703B80->DeAllocate(pLast);
    }
    this->field_0_p18 = NULL;
    return pIter;
}

MATCH_FUNC(0x5a6e40)
Sprite* SpriteList::FindClosestSprite_5A6E40(Fix16 xOff, Fix16 yOff)
{
    Fix16 smallest(99999);

    Sprite* new_ret = NULL;
    for (AttachedSprite* pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        Fix16 yDelta = Fix16::MaxAbsDistance_42A6B0(xOff, yOff, pIter->field_0_pSprite->field_14_xy.x, pIter->field_0_pSprite->field_14_xy.y);
        if (yDelta < smallest)
        {
            new_ret = pIter->field_0_pSprite;
            smallest = yDelta;
        }
    }
    return new_ret;
}

MATCH_FUNC(0x5a6ea0)
Sprite* SpriteList::TakeClosestSprite_5A6EA0(Fix16 xpos, Fix16 ypos)
{
    AttachedSprite* pPrev = NULL;
    AttachedSprite* pClosest = NULL;
    AttachedSprite* pBeforeClosest = NULL;
    Fix16 smallest(99999);
    Fix16 distance;

    for (AttachedSprite* pIter = field_0_p18; pIter; pIter = pIter->mpNext)
    {
        distance = Fix16::MaxAbsDistance_42A6B0(xpos, ypos, pIter->field_0_pSprite->field_14_xy.x, pIter->field_0_pSprite->field_14_xy.y);
        if (distance < smallest)
        {
            pClosest = pIter;
            pBeforeClosest = pPrev;
            smallest = distance;
        }
        pPrev = pIter;
    }

    if (!pClosest)
    {
        return NULL;
    }

    Sprite* pSprite = pClosest->field_0_pSprite;
    if (pBeforeClosest)
    {
        pBeforeClosest->mpNext = pClosest->mpNext;
    }
    else
    {
        field_0_p18 = pClosest->mpNext;
    }
    gAttachedSpritePool_703B80->DeAllocate(pClosest);
    return pSprite;
}

MATCH_FUNC(0x5a6f70)
void SpriteList::PoolUpdate_5A6F70(Sprite* pSprite)
{
    // TODO: Pretty much an exact match of UpdatePoolNoDeallocate but this one passes an extra
    // argument to PoolUpdate, probably DeAllocate was inlined - need to try to consolidate and refactor
    // all of these.
    // 9.6f 0x4BEF70
    AttachedSprite* pPreviousItem = NULL;
    AttachedSprite* pCurrItem = field_0_p18;
    while (pCurrItem)
    {
        AttachedSprite* pNext = pCurrItem->mpNext;
        if (pCurrItem->UpdateAttached_5A6910(pSprite))
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
                if (field_0_p18 == pCurrItem)
                {
                    field_0_p18 = pCurrItem->mpNext;
                }
                else
                {
                    pPreviousItem = field_0_p18;
                    while (pPreviousItem->mpNext != pCurrItem)
                    {
                        pPreviousItem = pPreviousItem->mpNext;
                    }
                    pPreviousItem->mpNext = pCurrItem->mpNext;
                }
            }
            gAttachedSpritePool_703B80->DeAllocate(pCurrItem);
        }
        else
        {
            pPreviousItem = pCurrItem;
        }

        pCurrItem = pNext;
    }
}

MATCH_FUNC(0x5a7010)
void SpriteList::DestroyAllSprites_5A7010()
{
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        Sprite* pSprite = pIter->field_0_pSprite;
        switch (pIter->field_0_pSprite->GetType_416B40())
        {
            case sprite_types_enum::car_2:
                gCarManager_6C_677930->RemoveFromPoolAndCollision_446730(pSprite->field_8_car_bc_ptr);
                break;
            case sprite_types_enum::object_1:
            case sprite_types_enum::code_obj1_4:
            case sprite_types_enum::map_obj_5:
                gObject_5C_6F8F84->RemoveAndFree_52A610(pSprite->field_8_object_2C_ptr);
                break;
            default:
                break;
        }
        pIter = pIter->mpNext;
    }
    ClearList_5A6E10();
}

// https://decomp.me/scratch/hQof2
MATCH_FUNC(0x5a7080)
void SpriteList::CleanupSpriteList_5A7080()
{
    AttachedSprite* pLast = NULL;
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        bool bUnlinkHead = false;
        s32 type = pIter->field_0_pSprite->GetType_416B40();
        if ((type == sprite_types_enum::object_1 || type > sprite_types_enum::ped_3 && type <= sprite_types_enum::map_obj_5) && pIter->field_0_pSprite->field_8_object_2C_ptr->IsFlameObject_4BE830())
        {
            gObject_5C_6F8F84->RemoveAndFree_52A610(pIter->field_0_pSprite->field_8_object_2C_ptr);
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
                pIter = pLast->mpNext;
            }
            else
            {
                bUnlinkHead = true;
            }
        }
        else
        {
            pLast = pIter;
            pIter = pIter->mpNext;
        }

        if (bUnlinkHead)
        {
            AttachedSprite* pNext = pIter->mpNext;
            gAttachedSpritePool_703B80->DeAllocate(pIter);
            pIter = pNext;
            this->field_0_p18 = pNext;
        }
    }
}

MATCH_FUNC(0x5a7110)
void SpriteList::ClearGangIconSprite_5A7110()
{
    AttachedSprite* pIter = this->field_0_p18;
    AttachedSprite* pLast = NULL;
    Object* pObject;
    if (pIter)
    {
        while (pIter)
        {
            pObject = pIter->field_0_pSprite->AsObject2C_40FEC0();
            if (pObject)
            {
                // If it is car gang icon
                if (pObject->IsGangIcon_4BE850())
                {
                    break;
                }
            }
            pLast = pIter;
            pIter = pIter->mpNext;
            if (!pIter)
            {
                return;
            }
        }

        gObject_5C_6F8F84->RemoveAndFree_52A610(pObject);

        if (pLast)
        {
            pLast->mpNext = pIter->mpNext;
        }
        else
        {
            this->field_0_p18 = pIter->mpNext;
        }
        gAttachedSpritePool_703B80->DeAllocate(pIter);
    }
}

MATCH_FUNC(0x5a71a0)
s32 SpriteList::GetGangIdxFromSpriteIfAny_5A71A0()
{
    AttachedSprite* pIter = this->field_0_p18;
    while (pIter)
    {
        const s32 type = pIter->field_0_pSprite->field_30_sprite_type_enum;
        if (type == sprite_types_enum::code_obj1_4 || type == sprite_types_enum::map_obj_5 || type == sprite_types_enum::object_1)
        {
            Object* pObject = pIter->field_0_pSprite->field_8_object_2C_ptr;
            if (pObject)
            {
                // If it is car gang icon
                if (pObject->field_18_model >= objects::loonies_icon_287 && pObject->field_18_model <= objects::russian_mafia_icon_293)
                {
                    return pObject->field_18_model - 286;
                }
            }
        }
        pIter = pIter->mpNext;
        if (!pIter)
        {
            return 0;
        }
    }
    return 0;
}

MATCH_FUNC(0x5a71f0)
void SpriteList::ExpireAttachedFlames_5A71F0()
{
    for (AttachedSprite* pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        const s32 type = pIter->field_0_pSprite->GetType_416B40();
        if (type == sprite_types_enum::object_1 || type > sprite_types_enum::ped_3 && type <= sprite_types_enum::map_obj_5)
        {
            Object* pObject = pIter->field_0_pSprite->field_8_object_2C_ptr;
            if (pObject->IsFlameObject_4BE830())
            {
                pIter->field_0_pSprite->field_8_object_2C_ptr->field_C_pAny.pExplosion->field_1A_timer = 2;
            }
        }
    }
}

MATCH_FUNC(0x5a7240)
void SpriteList::PruneNonCollidingSprites_5A7240(Sprite* pSprite)
{
    AttachedSprite* pNext = this->field_0_p18;
    AttachedSprite* pLast = NULL;

    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();

    while (pNext)
    {
        if (pSprite->CollisionCheck_59E590(pNext->field_0_pSprite))
        {
            pLast = pNext;
            pNext = pNext->mpNext;
        }
        else if (pLast)
        {
            pLast->mpNext = pNext->mpNext;
            gAttachedSpritePool_703B80->DeAllocate(pNext);
            pNext = pLast->mpNext;
        }
        else
        {
            AttachedSprite* pSaveNext = pNext->mpNext;
            gAttachedSpritePool_703B80->DeAllocate(pNext);
            pNext = pSaveNext;
            this->field_0_p18 = pSaveNext;
        }
    }
}

MATCH_FUNC(0x5a72b0)
void SpriteList::PropagateMaxZLayer_5A72B0(Sprite* pSprite, bool bAlsoSetSprite)
{
    char_type start_val = pSprite->ComputeZLayer_5A1BD0();
    char_type max_val = start_val;

    AttachedSprite* pIter;
    for (pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_pSprite->IsTypeAbove1_446950()) // object_5c type
        {
            const char_type cur_val = pIter->field_0_pSprite->ComputeZLayer_5A1BD0();
            if (cur_val > max_val)
            {
                max_val = cur_val;
            }
        }
    }

    if (bAlsoSetSprite)
    {
        pSprite->set_z_col_4BA220(max_val);
    }

    for (pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        pIter->field_0_pSprite->set_z_col_4BA220(max_val);
    }
}

MATCH_FUNC(0x5a7310)
bool SpriteList::AnySpriteHasSlopeTopInFootprint_5A7310()
{
    for (AttachedSprite* pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        if (pIter->field_0_pSprite->CheckSlopeTopInFootprint_5A1A60())
        {
            return true;
        }
    }
    return false;
}