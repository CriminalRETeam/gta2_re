// GetLength_41E260 compares against this TU's zero
#define FIX16_POINT_ZERO kFpZero_6F8E10
#include "Object_3C.hpp"
#include "Car_BC.hpp"
#include "char.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Explosion_30.hpp"
#include "enums.hpp"
#include "rng.hpp"
#include "sprite.hpp"

// TODO: Init to correct values
DEFINE_GLOBAL_INIT(Fix16, dword_6F8BF0, Fix16(0x3FC000, 0), 0x6F8BF0);
DEFINE_GLOBAL(Ang16, kZeroAng_6F8F68, 0x6F8F68);
DEFINE_GLOBAL(SpriteList_4, stru_67727C, 0x67727C);

EXTERN_GLOBAL(Fix16, dword_6F8ECC);

// 10.5 https://decomp.me/scratch/kj3y3
// 9.6f 0x482D90
MATCH_FUNC(0x521FD0)
void Object_3C::GetMovementSpeedAndAngle_521FD0(Fix16& Speed, Ang16& Angle)
{
    s8 x_related;
    s8 y_related;
    Fix16_Point point;
    Fix16_Point unk;
    if (field_38_conveyor_speed)
    {
        UnpackSignedNibbles_529050(field_38_conveyor_speed, &x_related, &y_related);
        unk.x = dword_6F8ECC * x_related;
        unk.y = dword_6F8ECC * y_related;
        point = unk + GetSpeedVector_482BA0();

        Speed = point.GetLength_41E260();
        Angle = point.atan2_40F790();
        ClearF38_482BD0();
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
    this->mpNext = 0;
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
    this->field_2F_bOnSlope = 0;
}

MATCH_FUNC(0x52ade0)
Object_3C::~Object_3C()
{
    mpNext = 0;
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
AttachedSprite_18* SpriteList_4::GetSpriteForModel_5A6A50(s32 obj_type)
{
    AttachedSprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        Object_2C* o5c = pIter->field_0_pSprite->AsObject2C_40FEC0();
        if (o5c)
        {
            if (o5c->field_18_model == obj_type)
            {
                return pIter;
            }
        }
        pIter = pIter->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x5a6a90)
Object_2C* SpriteList_4::FindObject2CByModel_5A6A90(s32 obj_type)
{
    AttachedSprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        const s32 sprite_type_enum = pIter->field_0_pSprite->field_30_sprite_type_enum;
        if (sprite_type_enum == sprite_types_enum::code_obj1_4 || sprite_type_enum == sprite_types_enum::map_obj_5 ||
            sprite_type_enum == sprite_types_enum::object_1)
        {
            Object_2C* p2C = pIter->field_0_pSprite->field_8_object_2C_ptr;
            if (p2C)
            {
                if (p2C->field_18_model == obj_type)
                {
                    return p2C;
                }
            }
        }
        pIter = pIter->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x5a6ad0)
AttachedSprite_18* SpriteList_4::FindFirstFireExplosion_5A6AD0()
{
    AttachedSprite_18* pObjIter = this->field_0_p18;
    while (pObjIter)
    {
        const s32 sprite_type = pObjIter->field_0_pSprite->field_30_sprite_type_enum;
        if (sprite_type == sprite_types_enum::code_obj1_4 || sprite_type == sprite_types_enum::map_obj_5 || sprite_type == sprite_types_enum::object_1)
        {
            Object_2C* o2c = pObjIter->field_0_pSprite->field_8_object_2C_ptr;
            if (o2c)
            {
                if (o2c->IsFireExplosion_525AC0())
                {
                    return pObjIter;
                }
            }
        }
        pObjIter = pObjIter->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x5a6b10)
void SpriteList_4::RemoveSprite_5A6B10(Sprite* toFind)
{
    AttachedSprite_18* pLast = 0;
    AttachedSprite_18* pIter = this->field_0_p18;
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
void SpriteList_4::RemoveSpriteSafe_5A6B60(Sprite* toFind)
{
    AttachedSprite_18* pLast = 0;
    AttachedSprite_18* pIter = this->field_0_p18;
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
void SpriteList_4::sub_5A6BB0()
{
    for (AttachedSprite_18* p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        p18Iter->DestroyAttached_5A69E0();
    }
}

MATCH_FUNC(0x5a6bd0)
void SpriteList_4::sub_5A6BD0()
{
    for (AttachedSprite_18* p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        p18Iter->SinkAttachedCar_5A6A20();
    }
}

MATCH_FUNC(0x5a6bf0)
void SpriteList_4::DispatchCarImpactEvents_5A6BF0(Sprite* pSprite)
{
    for (AttachedSprite_18* p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        p18Iter->field_0_pSprite->ProcessCarToCarImpactIfCar_59E910(pSprite);
    }
}

MATCH_FUNC(0x5a6c10)
char_type SpriteList_4::TagSpriteWithRng_5A6C10(Sprite* toFind)
{
    if (field_0_p18 != NULL)
    {
        for (AttachedSprite_18* pNext = field_0_p18; pNext != NULL; pNext = pNext->mpNext)
        {
            if (pNext->field_0_pSprite == toFind)
            {
                pNext->field_14_rng = gpRng_67AB34->get_cur_rng_41CFE0();
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x5a6c40)
void SpriteList_4::RemoveAllExceptRngValue_5A6C40(s32 toFind)
{
    AttachedSprite_18* pIter = this->field_0_p18;
    AttachedSprite_18* pLast = 0;

    while (pIter)
    {
        if (pIter->field_14_rng == toFind)
        {
            pLast = pIter;
            pIter = pIter->mpNext;
        }
        else
        {
            AttachedSprite_18* pSavedNext = pIter->mpNext;
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
Sprite* SpriteList_4::FirstSpriteOfType_5A6CA0(s32 sprite_type)
{
    if (field_0_p18 != NULL)
    {
        for (AttachedSprite_18* pNext = field_0_p18; pNext != NULL; pNext = pNext->mpNext)
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
void SpriteList_4::AddSprite_5A6CD0(Sprite* pSprite)
{
    AttachedSprite_18* p18 = gAttachedSpritePool_703B80->Allocate();
    p18->field_0_pSprite = pSprite;
    p18->mpNext = this->field_0_p18;
    p18->field_8_offset_x = 0;
    p18->field_C_offset_y = 0;
    field_0_p18 = p18;
}

MATCH_FUNC(0x5a6d00)
void SpriteList_4::PushAttachedSprite_5A6D00(Sprite* pSprite1, Fix16 x, Fix16 y, Ang16 angle)
{
    AttachedSprite_18* p18 = gAttachedSpritePool_703B80->Allocate();
    p18->field_0_pSprite = pSprite1;
    p18->mpNext = field_0_p18;
    p18->field_8_offset_x = x;
    p18->field_C_offset_y = y;
    p18->field_10_offset_rot = angle;
    field_0_p18 = p18;
}

MATCH_FUNC(0x5a6d40)
void SpriteList_4::PushSprite_5A6D40(Sprite* pToFind)
{
    AttachedSprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        if (pIter->field_0_pSprite == pToFind)
        {
            return;
        }
        pIter = pIter->mpNext;
    }

    AttachedSprite_18* pNew = gAttachedSpritePool_703B80->Allocate();
    pNew->field_0_pSprite = pToFind;

    pNew->mpNext = this->field_0_p18;
    this->field_0_p18 = pNew;
}

MATCH_FUNC(0x5a6d80)
char_type SpriteList_4::SpriteExists_5A6D80(Sprite* pToFind)
{
    AttachedSprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        if (pIter->field_0_pSprite == pToFind)
        {
            return 1;
        }
        pIter = pIter->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x5a6da0)
Sprite* SpriteList_4::PopFrontSprite_5A6DA0()
{
    AttachedSprite_18* p18 = this->field_0_p18;
    if (!this->field_0_p18)
    {
        return 0;
    }
    Sprite* pOld = p18->field_0_pSprite;
    this->field_0_p18 = p18->mpNext;
    gAttachedSpritePool_703B80->DeAllocate(p18);
    return pOld;
}

MATCH_FUNC(0x5a6dc0)
Sprite* SpriteList_4::PopBackSprite_5A6DC0()
{
    AttachedSprite_18* pIter = this->field_0_p18;
    AttachedSprite_18* pLast = 0;
    while (pIter->mpNext)
    {
        pLast = pIter;
        pIter = pIter->mpNext;
    }

    Sprite* result = pIter->field_0_pSprite;
    if (pLast)
    {
        pLast->mpNext = 0;
    }
    else
    {
        this->field_0_p18 = 0;
    }
    gAttachedSpritePool_703B80->DeAllocate(pIter);

    return result;
}

MATCH_FUNC(0x5a6e10)
AttachedSprite_18* SpriteList_4::ClearList_5A6E10()
{
    AttachedSprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        AttachedSprite_18* pLast = pIter;
        pIter = pIter->mpNext;
        gAttachedSpritePool_703B80->DeAllocate(pLast);
    }
    this->field_0_p18 = 0;
    return pIter;
}

MATCH_FUNC(0x5a6e40)
Sprite* SpriteList_4::FindClosestSprite_5A6E40(Fix16 xOff, Fix16 yOff)
{
    Fix16 smallest(99999);

    Sprite* new_ret = 0;
    for (AttachedSprite_18* pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
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
Sprite* SpriteList_4::TakeClosestSprite_5A6EA0(Fix16 xpos, Fix16 ypos)
{
    AttachedSprite_18* pPrev = 0;
    AttachedSprite_18* pClosest = 0;
    AttachedSprite_18* pBeforeClosest = 0;
    Fix16 smallest(99999);
    Fix16 distance;

    for (AttachedSprite_18* pIter = field_0_p18; pIter; pIter = pIter->mpNext)
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
        return 0;
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
void SpriteList_4::PoolUpdate_5A6F70(Sprite* pSprite)
{
    // TODO: Pretty much an exact match of UpdatePoolNoDeallocate but this one passes an extra
    // argument to PoolUpdate, probably DeAllocate was inlined - need to try to consolidate and refactor
    // all of these.
    // 9.6f 0x4BEF70
    AttachedSprite_18* pPreviousItem = 0;
    AttachedSprite_18* pCurrItem = field_0_p18;
    while (pCurrItem)
    {
        AttachedSprite_18* pNext = pCurrItem->mpNext;
        if (pCurrItem->UpdateAttached_5A6910(pSprite))
        {
            if (pPreviousItem && pPreviousItem->mpNext != pCurrItem)
            {
                pPreviousItem = 0;
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
void SpriteList_4::DestroyAllSprites_5A7010()
{
    AttachedSprite_18* p18Iter = this->field_0_p18;
    while (p18Iter)
    {
        Sprite* pSprite = p18Iter->field_0_pSprite;
        switch (p18Iter->field_0_pSprite->GetType_416B40())
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
        p18Iter = p18Iter->mpNext;
    }
    ClearList_5A6E10();
}

// https://decomp.me/scratch/hQof2
MATCH_FUNC(0x5a7080)
void SpriteList_4::CleanupSpriteList_5A7080()
{
    AttachedSprite_18* pLast = 0;
    AttachedSprite_18* pIter = this->field_0_p18;
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
            AttachedSprite_18* pNext = pIter->mpNext;
            gAttachedSpritePool_703B80->DeAllocate(pIter);
            pIter = pNext;
            this->field_0_p18 = pNext;
        }
    }
}

MATCH_FUNC(0x5a7110)
void SpriteList_4::ClearGangIconSprite_5A7110()
{
    AttachedSprite_18* pIter = this->field_0_p18;
    AttachedSprite_18* pLast = 0;
    Object_2C* p5C;
    if (pIter)
    {
        while (pIter)
        {
            p5C = pIter->field_0_pSprite->AsObject2C_40FEC0();
            if (p5C)
            {
                // If it is car gang icon
                if (p5C->IsGangIcon_4BE850())
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

        gObject_5C_6F8F84->RemoveAndFree_52A610(p5C);

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
s32 SpriteList_4::GetGangIdxFromSpriteIfAny_5A71A0()
{
    AttachedSprite_18* p18Iter = this->field_0_p18;
    while (p18Iter)
    {
        const s32 type = p18Iter->field_0_pSprite->field_30_sprite_type_enum;
        if (type == sprite_types_enum::code_obj1_4 || type == sprite_types_enum::map_obj_5 || type == sprite_types_enum::object_1)
        {
            Object_2C* o5c = p18Iter->field_0_pSprite->field_8_object_2C_ptr;
            if (o5c)
            {
                // If it is car gang icon
                if (o5c->field_18_model >= objects::loonies_icon_287 && o5c->field_18_model <= objects::russian_mafia_icon_293)
                {
                    return o5c->field_18_model - 286;
                }
            }
        }
        p18Iter = p18Iter->mpNext;
        if (!p18Iter)
        {
            return 0;
        }
    }
    return 0;
}

MATCH_FUNC(0x5a71f0)
void SpriteList_4::sub_5A71F0()
{
    for (AttachedSprite_18* p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        const s32 type = p18Iter->field_0_pSprite->GetType_416B40();
        if (type == sprite_types_enum::object_1 || type > sprite_types_enum::ped_3 && type <= sprite_types_enum::map_obj_5)
        {
            Object_2C* o2c = p18Iter->field_0_pSprite->field_8_object_2C_ptr;
            if (o2c->IsFlameObject_4BE830())
            {
                p18Iter->field_0_pSprite->field_8_object_2C_ptr->field_C_pAny.pExplosion->field_1A_timer = 2;
            }
        }
    }
}

MATCH_FUNC(0x5a7240)
void SpriteList_4::PruneNonCollidingSprites_5A7240(Sprite* pSprite)
{
    AttachedSprite_18* pNext = this->field_0_p18;
    AttachedSprite_18* pLast = 0;

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
            AttachedSprite_18* pSaveNext = pNext->mpNext;
            gAttachedSpritePool_703B80->DeAllocate(pNext);
            pNext = pSaveNext;
            this->field_0_p18 = pSaveNext;
        }
    }
}

MATCH_FUNC(0x5a72b0)
void SpriteList_4::PropagateMaxZLayer_5A72B0(Sprite* pSprite, char_type bUnknown)
{
    char start_val = pSprite->ComputeZLayer_5A1BD0();
    char max_val = start_val;

    AttachedSprite_18* p18Iter;
    for (p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        if (p18Iter->field_0_pSprite->IsTypeAbove1_446950()) // object_5c type
        {
            const char_type cur_val = p18Iter->field_0_pSprite->ComputeZLayer_5A1BD0();
            if (cur_val > max_val)
            {
                max_val = cur_val;
            }
        }
    }

    if (bUnknown)
    {
        pSprite->set_z_col_4BA220(max_val);
    }

    for (p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        p18Iter->field_0_pSprite->set_z_col_4BA220(max_val);
    }
}

MATCH_FUNC(0x5a7310)
char_type SpriteList_4::sub_5A7310()
{
    AttachedSprite_18* p18Iter;
    for (p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        if (p18Iter->field_0_pSprite->sub_5A1A60())
        {
            return 1;
        }
    }
    return 0;
}