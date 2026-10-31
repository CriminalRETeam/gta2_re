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

DEFINE_GLOBAL_INIT(Fix16, kFp255_6F8BF0, Fix16(255), 0x6F8BF0);
DEFINE_GLOBAL(Ang16, kZeroAng_6F8F68, 0x6F8F68);
DEFINE_GLOBAL(SpriteList, gTrailerHitchCandidates_67727C, 0x67727C);

EXTERN_GLOBAL(Fix16, kFpOne64th_6F8ECC);

// 10.5 https://decomp.me/scratch/kj3y3
// 9.6f 0x482D90
MATCH_FUNC(0x521FD0)
void Object_3C::GetMovementSpeedAndAngle(Fix16& Speed, Ang16& Angle)
{
    s8 conveyor_x;
    s8 conveyor_y;
    Fix16_Point point;
    Fix16_Point conveyor_vel;
    if (mConveyorSpeed)
    {
        UnpackSignedNibbles(mConveyorSpeed, &conveyor_x, &conveyor_y);
        conveyor_vel.x = kFpOne64th_6F8ECC * conveyor_x;
        conveyor_vel.y = kFpOne64th_6F8ECC * conveyor_y;
        point = conveyor_vel + GetSpeedVector_482BA0();

        Speed = point.GetLength_41E260();
        Angle = point.atan2_40F790();
        ClearConveyorSpeed_482BD0();
    }
    else
    {
        Speed = mSpeed;
        Angle = mAngle;
    }
}

MATCH_FUNC(0x52ad80)
Object_3C::Object_3C()
{
    this->mAngle = 0;
    this->mpNext = NULL;
    this->mZAccel = kFp255_6F8BF0;
    this->mFriction = kFp255_6F8BF0;
    this->mZSpeed = kFp255_6F8BF0;
    this->mSpeed = kFp255_6F8BF0;
    this->mAngle = kZeroAng_6F8F68;
    this->mNextDefinitionTimer = 0;
    this->mObj2cId = 0;
    this->mUnused2C = 0;
    this->mbAirborne = 0;
    this->mConveyorSpeed = 0;
    this->mSpriteCollideMode = 2;
    this->mUnused2E = 0;
    this->mbOnSlope = false;
}

MATCH_FUNC(0x52ade0)
Object_3C::~Object_3C()
{
    mpNext = NULL;
}

// 9.6f 0x482BA0
MATCH_FUNC(0x52adf0)
Fix16_Point Object_3C::GetSpeedVector_52ADF0() const
{
    Fix16_Point p;
    p.FromPolar_41E210(mSpeed, mAngle);
    return p;
}

MATCH_FUNC(0x5a6a50)
AttachedSprite* SpriteList::GetSpriteForModel(s32 obj_type) const
{
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        Object* pObject = pIter->mpSprite->AsObject2C_40FEC0();
        if (pObject)
        {
            if (pObject->mModel == obj_type)
            {
                return pIter;
            }
        }
        pIter = pIter->mpNext;
    }
    return NULL;
}

MATCH_FUNC(0x5a6a90)
Object* SpriteList::FindObject2CByModel(s32 obj_type) const
{
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        const s32 sprite_type_enum = pIter->mpSprite->mSpriteTypeEnum;
        if (sprite_type_enum == sprite_types_enum::code_obj1_4 || sprite_type_enum == sprite_types_enum::map_obj_5 ||
            sprite_type_enum == sprite_types_enum::object_1)
        {
            Object* pObject = pIter->mpSprite->mObject2CPtr;
            if (pObject)
            {
                if (pObject->mModel == obj_type)
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
AttachedSprite* SpriteList::FindFirstFireExplosion() const
{
    AttachedSprite* pObjIter = this->mP18;
    while (pObjIter)
    {
        const s32 sprite_type = pObjIter->mpSprite->mSpriteTypeEnum;
        if (sprite_type == sprite_types_enum::code_obj1_4 || sprite_type == sprite_types_enum::map_obj_5 || sprite_type == sprite_types_enum::object_1)
        {
            Object* pObject = pObjIter->mpSprite->mObject2CPtr;
            if (pObject)
            {
                if (pObject->IsFireExplosion())
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
void SpriteList::RemoveSprite(Sprite* toFind)
{
    AttachedSprite* pLast = NULL;
    AttachedSprite* pIter = this->mP18;
    for (;;)
    {
        if (pIter->mpSprite == toFind)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
            }
            else
            {
                this->mP18 = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
            }
            return;
        }
        pLast = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x5a6b60)
void SpriteList::RemoveSpriteSafe(Sprite* toFind)
{
    AttachedSprite* pLast = NULL;
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        if (pIter->mpSprite == toFind)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
            }
            else
            {
                this->mP18 = pIter->mpNext;
                gAttachedSpritePool_703B80->DeAllocate(pIter);
            }
            return;
        }
        pLast = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x5a6bb0)
void SpriteList::DestroyAllAttached()
{
    for (AttachedSprite* pIter = this->mP18; pIter; pIter = pIter->mpNext)
    {
        pIter->DestroyAttached();
    }
}

MATCH_FUNC(0x5a6bd0)
void SpriteList::SinkAllAttachedCars()
{
    for (AttachedSprite* pIter = this->mP18; pIter; pIter = pIter->mpNext)
    {
        pIter->SinkAttachedCar();
    }
}

MATCH_FUNC(0x5a6bf0)
void SpriteList::DispatchCarImpactEvents(Sprite* pSprite)
{
    for (AttachedSprite* pIter = this->mP18; pIter; pIter = pIter->mpNext)
    {
        pIter->mpSprite->ProcessCarToCarImpactIfCar(pSprite);
    }
}

MATCH_FUNC(0x5a6c10)
bool SpriteList::TagSpriteWithRng(Sprite* toFind)
{
    if (mP18 != NULL)
    {
        for (AttachedSprite* pNext = mP18; pNext != NULL; pNext = pNext->mpNext)
        {
            if (pNext->mpSprite == toFind)
            {
                pNext->mRng = gpRng_67AB34->get_cur_rng_41CFE0();
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x5a6c40)
void SpriteList::RemoveAllExceptRngValue(s32 toFind)
{
    AttachedSprite* pIter = this->mP18;
    AttachedSprite* pLast = NULL;

    while (pIter)
    {
        if (pIter->mRng == toFind)
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
                this->mP18 = pSavedNext;
            }
        }
    }
}

MATCH_FUNC(0x5a6ca0)
Sprite* SpriteList::FirstSpriteOfType(s32 sprite_type) const
{
    if (mP18 != NULL)
    {
        for (AttachedSprite* pNext = mP18; pNext != NULL; pNext = pNext->mpNext)
        {
            if (pNext->mpSprite->mSpriteTypeEnum == sprite_type)
            {
                return pNext->mpSprite;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x5a6cd0)
void SpriteList::AddSprite(Sprite* pSprite)
{
    AttachedSprite* pAttached = gAttachedSpritePool_703B80->Allocate();
    pAttached->mpSprite = pSprite;
    pAttached->mpNext = this->mP18;
    pAttached->mOffsetX = 0;
    pAttached->mOffsetY = 0;
    mP18 = pAttached;
}

MATCH_FUNC(0x5a6d00)
void SpriteList::PushAttachedSprite(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 angle)
{
    AttachedSprite* pAttached = gAttachedSpritePool_703B80->Allocate();
    pAttached->mpSprite = pSprite;
    pAttached->mpNext = mP18;
    pAttached->mOffsetX = x;
    pAttached->mOffsetY = y;
    pAttached->mOffsetRot = angle;
    mP18 = pAttached;
}

MATCH_FUNC(0x5a6d40)
void SpriteList::PushSprite(Sprite* pSprite)
{
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        if (pIter->mpSprite == pSprite)
        {
            return;
        }
        pIter = pIter->mpNext;
    }

    AttachedSprite* pNew = gAttachedSpritePool_703B80->Allocate();
    pNew->mpSprite = pSprite;

    pNew->mpNext = this->mP18;
    this->mP18 = pNew;
}

MATCH_FUNC(0x5a6d80)
bool SpriteList::SpriteExists(Sprite* pToFind) const
{
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        if (pIter->mpSprite == pToFind)
        {
            return true;
        }
        pIter = pIter->mpNext;
    }
    return false;
}

MATCH_FUNC(0x5a6da0)
Sprite* SpriteList::PopFrontSprite()
{
    AttachedSprite* pAttached = this->mP18;
    if (!this->mP18)
    {
        return NULL;
    }
    Sprite* pOld = pAttached->mpSprite;
    this->mP18 = pAttached->mpNext;
    gAttachedSpritePool_703B80->DeAllocate(pAttached);
    return pOld;
}

MATCH_FUNC(0x5a6dc0)
Sprite* SpriteList::PopBackSprite()
{
    AttachedSprite* pIter = this->mP18;
    AttachedSprite* pLast = NULL;
    while (pIter->mpNext)
    {
        pLast = pIter;
        pIter = pIter->mpNext;
    }

    Sprite* result = pIter->mpSprite;
    if (pLast)
    {
        pLast->mpNext = NULL;
    }
    else
    {
        this->mP18 = NULL;
    }
    gAttachedSpritePool_703B80->DeAllocate(pIter);

    return result;
}

MATCH_FUNC(0x5a6e10)
AttachedSprite* SpriteList::ClearList()
{
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        AttachedSprite* pLast = pIter;
        pIter = pIter->mpNext;
        gAttachedSpritePool_703B80->DeAllocate(pLast);
    }
    this->mP18 = NULL;
    return pIter;
}

MATCH_FUNC(0x5a6e40)
Sprite* SpriteList::FindClosestSprite(Fix16 xpos, Fix16 ypos) const
{
    Fix16 smallest(99999);

    Sprite* pClosest = NULL;
    for (AttachedSprite* pIter = this->mP18; pIter; pIter = pIter->mpNext)
    {
        Fix16 distance = Fix16::MaxAbsDistance_42A6B0(xpos, ypos, pIter->mpSprite->mXy.x, pIter->mpSprite->mXy.y);
        if (distance < smallest)
        {
            pClosest = pIter->mpSprite;
            smallest = distance;
        }
    }
    return pClosest;
}

MATCH_FUNC(0x5a6ea0)
Sprite* SpriteList::TakeClosestSprite(Fix16 xpos, Fix16 ypos)
{
    AttachedSprite* pPrev = NULL;
    AttachedSprite* pClosest = NULL;
    AttachedSprite* pBeforeClosest = NULL;
    Fix16 smallest(99999);
    Fix16 distance;

    for (AttachedSprite* pIter = mP18; pIter; pIter = pIter->mpNext)
    {
        distance = Fix16::MaxAbsDistance_42A6B0(xpos, ypos, pIter->mpSprite->mXy.x, pIter->mpSprite->mXy.y);
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

    Sprite* pSprite = pClosest->mpSprite;
    if (pBeforeClosest)
    {
        pBeforeClosest->mpNext = pClosest->mpNext;
    }
    else
    {
        mP18 = pClosest->mpNext;
    }
    gAttachedSpritePool_703B80->DeAllocate(pClosest);
    return pSprite;
}

MATCH_FUNC(0x5a6f70)
void SpriteList::PoolUpdate(Sprite* pSprite)
{
    // 9.6f 0x4BEF70
    AttachedSprite* pPreviousItem = NULL;
    AttachedSprite* pCurrItem = mP18;
    while (pCurrItem)
    {
        AttachedSprite* pNext = pCurrItem->mpNext;
        if (pCurrItem->UpdateAttached(pSprite))
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
                if (mP18 == pCurrItem)
                {
                    mP18 = pCurrItem->mpNext;
                }
                else
                {
                    pPreviousItem = mP18;
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
void SpriteList::DestroyAllSprites()
{
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        Sprite* pSprite = pIter->mpSprite;
        switch (pIter->mpSprite->GetType_416B40())
        {
            case sprite_types_enum::car_2:
                gCarManager_6C_677930->RemoveFromPoolAndCollision(pSprite->mCarBcPtr);
                break;
            case sprite_types_enum::object_1:
            case sprite_types_enum::code_obj1_4:
            case sprite_types_enum::map_obj_5:
                gObject_5C_6F8F84->RemoveAndFree(pSprite->mObject2CPtr);
                break;
            default:
                break;
        }
        pIter = pIter->mpNext;
    }
    ClearList();
}

// https://decomp.me/scratch/hQof2
MATCH_FUNC(0x5a7080)
void SpriteList::CleanupSpriteList()
{
    AttachedSprite* pLast = NULL;
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        bool bUnlinkHead = false;
        s32 type = pIter->mpSprite->GetType_416B40();
        if ((type == sprite_types_enum::object_1 || type > sprite_types_enum::ped_3 && type <= sprite_types_enum::map_obj_5) && pIter->mpSprite->mObject2CPtr->IsFlameObject_4BE830())
        {
            gObject_5C_6F8F84->RemoveAndFree(pIter->mpSprite->mObject2CPtr);
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
            this->mP18 = pNext;
        }
    }
}

MATCH_FUNC(0x5a7110)
void SpriteList::ClearGangIconSprite()
{
    AttachedSprite* pIter = this->mP18;
    AttachedSprite* pLast = NULL;
    Object* pObject;
    if (pIter)
    {
        while (pIter)
        {
            pObject = pIter->mpSprite->AsObject2C_40FEC0();
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

        gObject_5C_6F8F84->RemoveAndFree(pObject);

        if (pLast)
        {
            pLast->mpNext = pIter->mpNext;
        }
        else
        {
            this->mP18 = pIter->mpNext;
        }
        gAttachedSpritePool_703B80->DeAllocate(pIter);
    }
}

MATCH_FUNC(0x5a71a0)
s32 SpriteList::GetGangIdxFromSpriteIfAny() const
{
    AttachedSprite* pIter = this->mP18;
    while (pIter)
    {
        const s32 type = pIter->mpSprite->mSpriteTypeEnum;
        if (type == sprite_types_enum::code_obj1_4 || type == sprite_types_enum::map_obj_5 || type == sprite_types_enum::object_1)
        {
            Object* pObject = pIter->mpSprite->mObject2CPtr;
            if (pObject)
            {
                // If it is car gang icon
                if (pObject->mModel >= objects::loonies_icon_287 && pObject->mModel <= objects::russian_mafia_icon_293)
                {
                    return pObject->mModel - 286;
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
void SpriteList::ExpireAttachedFlames()
{
    for (AttachedSprite* pIter = this->mP18; pIter; pIter = pIter->mpNext)
    {
        const s32 type = pIter->mpSprite->GetType_416B40();
        if (type == sprite_types_enum::object_1 || type > sprite_types_enum::ped_3 && type <= sprite_types_enum::map_obj_5)
        {
            Object* pObject = pIter->mpSprite->mObject2CPtr;
            if (pObject->IsFlameObject_4BE830())
            {
                pIter->mpSprite->mObject2CPtr->mpAny.pExplosion->mTimer = 2;
            }
        }
    }
}

MATCH_FUNC(0x5a7240)
void SpriteList::PruneNonCollidingSprites(Sprite* pSprite)
{
    AttachedSprite* pNext = this->mP18;
    AttachedSprite* pLast = NULL;

    pSprite->UpdateCollisionBoundsIfNeeded();

    while (pNext)
    {
        if (pSprite->CollisionCheck_59E590(pNext->mpSprite))
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
            this->mP18 = pSaveNext;
        }
    }
}

MATCH_FUNC(0x5a72b0)
void SpriteList::PropagateMaxZLayer(Sprite* pSprite, bool bAlsoSetSprite)
{
    char_type max_layer = pSprite->ComputeZLayer();

    AttachedSprite* pIter;
    for (pIter = this->mP18; pIter; pIter = pIter->mpNext)
    {
        if (pIter->mpSprite->IsTypeAbove1_446950()) // object_5c type
        {
            const char_type layer = pIter->mpSprite->ComputeZLayer();
            if (layer > max_layer)
            {
                max_layer = layer;
            }
        }
    }

    if (bAlsoSetSprite)
    {
        pSprite->set_z_col_4BA220(max_layer);
    }

    for (pIter = this->mP18; pIter; pIter = pIter->mpNext)
    {
        pIter->mpSprite->set_z_col_4BA220(max_layer);
    }
}

MATCH_FUNC(0x5a7310)
bool SpriteList::AnySpriteHasSlopeTopInFootprint() const
{
    for (AttachedSprite* pIter = this->mP18; pIter; pIter = pIter->mpNext)
    {
        if (pIter->mpSprite->CheckSlopeTopInFootprint())
        {
            return true;
        }
    }
    return false;
}