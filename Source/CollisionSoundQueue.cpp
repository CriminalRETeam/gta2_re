#include "CollisionSoundQueue.hpp"
#include "Object_5C.hpp"
#include "debug.hpp"
#include "map_0x370.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "collision_event_type.hpp"

DEFINE_GLOBAL(CollisionSoundQueue*, gCollisionSoundQueue_66AFE0, 0x66AFE0);
DEFINE_GLOBAL(CollisionTarget, gCollisionTarget_679188, 0x679188);
DEFINE_GLOBAL(Fix16, kFpZero_66AE98, 0x66AE98);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_66AE9C, Fix16(1), 0x66AE9C);

MATCH_FUNC(0x40B870)
void CollisionEvent::SetPosition(Fix16 x, Fix16 y, Fix16 z)
{
    mX = x;
    mY = y;
    mZ = z;
}

MATCH_FUNC(0x40B890)
void CollisionEvent::SetupForCar(Car* pCar)
{
    mCar = pCar;

    switch (gCollisionTarget_679188.mType)
    {
        case collision_target_type::horizontal_edge_1:
            mType = collision_event_type::car_wall_5;
            mMapBlockSpec =
                gMap_0x370_6F6268->GetTopEdgeSpec(gCollisionTarget_679188.mHsegXMin, gCollisionTarget_679188.mHsegY, gCollisionTarget_679188.mZ);
            break;

        case collision_target_type::vertical_edge_2:
            mType = collision_event_type::car_wall_5;
            mMapBlockSpec =
                gMap_0x370_6F6268->GetLeftEdgeSpec(gCollisionTarget_679188.mVsegX, gCollisionTarget_679188.mVsegYMin, gCollisionTarget_679188.mZ);
            break;

        case collision_target_type::sprite_3:
        {
            Car* pCar = gCollisionTarget_679188.mpHitSprite->AsCar_40FEB0();
            if (pCar)
            {
                mOtherCar = pCar;
                mType = collision_event_type::car_car_2;
            }
            else if (gCollisionTarget_679188.mpHitSprite->AsCharB4_40FEA0())
            {
                mType = collision_event_type::car_ped_4;
            }
            else
            {
                Object* pObject = gCollisionTarget_679188.mpHitSprite->AsObject2C_40FEC0();

                mObjectModel = pObject->GetModel_40FEF0();
                if (mObjectModel == objects::diagonal_wall_collision_obj_166)
                {
                    mType = collision_event_type::car_wall_5;
                    mMapBlockSpec = pObject->GetMapBlockSoundSpec();
                }
                else
                {
                    mType = collision_event_type::car_object_3;
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x40B980)
void CollisionEvent::SetupForPed()
{
    switch (gCollisionTarget_679188.mType)
    {
        case collision_target_type::horizontal_edge_1:
            mType = collision_event_type::ped_wall_8;
            mMapBlockSpec =
                gMap_0x370_6F6268->GetTopEdgeSpec(gCollisionTarget_679188.mHsegXMin, gCollisionTarget_679188.mHsegY, gCollisionTarget_679188.mZ);
            break;

        case collision_target_type::vertical_edge_2:
            mType = collision_event_type::ped_wall_8;
            mMapBlockSpec =
                gMap_0x370_6F6268->GetLeftEdgeSpec(gCollisionTarget_679188.mVsegX, gCollisionTarget_679188.mVsegYMin, gCollisionTarget_679188.mZ);
            break;

        case collision_target_type::sprite_3:
        {
            Car* pCar = gCollisionTarget_679188.mpHitSprite->AsCar_40FEB0();
            if (pCar)
            {
                mCar = pCar;
                mType = collision_event_type::car_ped_4;
            }
            else if (gCollisionTarget_679188.mpHitSprite->AsCharB4_40FEA0())
            {
                mType = collision_event_type::ped_ped_6;
            }
            else
            {
                Object* pObject = gCollisionTarget_679188.mpHitSprite->AsObject2C_40FEC0();
                mObjectModel = pObject->GetModel_40FEF0();
                if (mObjectModel == objects::diagonal_wall_collision_obj_166)
                {
                    mType = collision_event_type::ped_wall_8;
                    mMapBlockSpec = pObject->GetMapBlockSoundSpec();
                }
                else
                {
                    mType = collision_event_type::ped_object_7;
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x40BA60)
bool CollisionEvent::SetupForObject(Object* pObj)
{
    if (!pObj->MakesCollisionSound_40FF00())
    {
        return false;
    }

    mObjectModel = pObj->mModel;

    switch (gCollisionTarget_679188.mType)
    {
        case collision_target_type::horizontal_edge_1:
            mType = collision_event_type::object_wall_10;
            mMapBlockSpec =
                gMap_0x370_6F6268->GetTopEdgeSpec(gCollisionTarget_679188.mHsegXMin, gCollisionTarget_679188.mHsegY, gCollisionTarget_679188.mZ);

            break;

        case collision_target_type::vertical_edge_2:
            mType = collision_event_type::object_wall_10;
            mMapBlockSpec =
                gMap_0x370_6F6268->GetLeftEdgeSpec(gCollisionTarget_679188.mVsegX, gCollisionTarget_679188.mVsegYMin, gCollisionTarget_679188.mZ);

            break;

        case collision_target_type::sprite_3:
        {
            Car* pCar = gCollisionTarget_679188.mpHitSprite->AsCar_40FEB0();
            if (pCar)
            {
                mCar = pCar;
                mType = collision_event_type::car_object_3;
                return true;
            }
            else if (gCollisionTarget_679188.mpHitSprite->AsCharB4_40FEA0())
            {
                mType = collision_event_type::ped_object_7;
                return true;
            }
            else
            {
                Object* pObject = gCollisionTarget_679188.mpHitSprite->AsObject2C_40FEC0();
                if (!pObject->MakesCollisionSound_40FF00())
                {
                    return false;
                }

                mOtherObjectModel = pObject->GetModel_40FEF0();
                if (mOtherObjectModel == objects::diagonal_wall_collision_obj_166)
                {
                    mType = collision_event_type::object_wall_10;
                    mMapBlockSpec = pObject->GetMapBlockSoundSpec();
                }
                else
                {
                    mType = collision_event_type::object_object_9;
                }
            }
            break;
        }
    }

    return true;
}

MATCH_FUNC(0x40bb90)
void CollisionSoundQueue::Reset()
{
    mCount = 0;
}

// ================================================================

MATCH_FUNC(0x40bba0)
void CollisionSoundQueue::AddCollision(Sprite* pSprite, Fix16 impact_strength)
{
    if (!bSkip_audio_67D6BE)
    {
        CollisionEvent* pEvent = &mEvents[mCount];
        Car* pCar = pSprite->AsCar_40FEB0();
        if (pCar)
        {
            pEvent->SetupForCar(pCar);
        }
        else if (pSprite->AsCharB4_40FEA0())
        {
            pEvent->SetupForPed();
        }
        else
        {
            Object* pObject = pSprite->AsObject2C_40FEC0();
            if (!pEvent->SetupForObject(pObject))
            {
                return;
            }
        }

        pEvent->SetPosition(pSprite->mXy.x, pSprite->mXy.y, pSprite->mZpos);
        pEvent->SetImpactStrength_40FF10(impact_strength);
        mCount++;
    }
}

MATCH_FUNC(0x40bc40)
void CollisionSoundQueue::AddFloorCollision(Sprite* pSprite)
{
    if (!bSkip_audio_67D6BE)
    {
        CollisionEvent* pEvent = &mEvents[mCount];
        pEvent->SetPosition(pSprite->mXy.x, pSprite->mXy.y, pSprite->mZpos);
        pEvent->SetImpactStrength_40FF10(kFpZero_66AE98);

        Car* pCar = pSprite->AsCar_40FEB0();
        if (pCar)
        {
            pEvent->mType = collision_event_type::car_floor_12;
            pEvent->mCar = pCar;
        }
        else if (pSprite->AsCharB4_40FEA0())
        {
            pEvent->mType = collision_event_type::ped_floor_11;
        }
        else
        {
            Object* pObject = pSprite->AsObject2C_40FEC0();
            pEvent->mType = collision_event_type::object_floor_1;
            pEvent->mObjectModel = pObject->GetModel_40FEF0();
        }
        pEvent->mMapBlockSpec = gMap_0x370_6F6268->GetBlockSpec(pSprite->mXy.x,
                                                                              pSprite->mXy.y,
                                                                              pSprite->mZpos - kFpOne_66AE9C);
        mCount++;
    }
}

MATCH_FUNC(0x40bd10)
void CollisionSoundQueue::AddBlockCollision(Sprite* pSprite)
{
    if (!bSkip_audio_67D6BE)
    {
        CollisionEvent* pEvent = &this->mEvents[this->mCount];
        pEvent->SetPosition(pSprite->mXy.x, pSprite->mXy.y, pSprite->mZpos);
        pEvent->SetImpactStrength_40FF10(kFpZero_66AE98);

        Car* pCar = pSprite->AsCar_40FEB0();
        if (pCar)
        {
            pEvent->mType = collision_event_type::car_floor_12;
            pEvent->mCar = pCar;
        }
        else if (pSprite->AsCharB4_40FEA0())
        {
            pEvent->mType = collision_event_type::ped_floor_11;
        }
        else
        {
            Object* pObject = pSprite->AsObject2C_40FEC0();
            pEvent->mType = collision_event_type::object_floor_1;
            pEvent->mObjectModel = pObject->GetModel_40FEF0();
        }
        pEvent->mMapBlockSpec =
            gMap_0x370_6F6268->GetBlockSpec(pSprite->mXy.x, pSprite->mXy.y, pSprite->mZpos);
        mCount++;
    }
}

MATCH_FUNC(0x40bdd0)
void CollisionSoundQueue::AddSpriteCollision(Sprite* pSprite, Sprite* pHitSprite)
{
    gCollisionTarget_679188.mpHitSprite = pHitSprite;
    gCollisionTarget_679188.mType = collision_target_type::sprite_3;
    AddCollision(pSprite, kFpZero_66AE98);
}

MATCH_FUNC(0x40be00)
CollisionSoundQueue::CollisionSoundQueue()
{
    if (bSkip_audio_67D6BE)
    {
        mpSoundObj = NULL;
    }
    else
    {
        mpSoundObj = gRoot_sound_66B038.CreateSoundObject(this, SoundObjectTypeEnum::CollisionSoundQueue_6);
    }
    Reset();
}

MATCH_FUNC(0x40be40)
CollisionSoundQueue::~CollisionSoundQueue()
{
    if (mpSoundObj)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(mpSoundObj);
        mpSoundObj = NULL;
    }
}

MATCH_FUNC(0x477A10)
bool CollisionTarget::IsObj2C() const
{
    if (mType == collision_target_type::sprite_3)
    {
        Object* pObject = mpHitSprite->AsObject2C_40FEC0();
        if (pObject)
        {
            if (pObject->mDefinition->mCollisionResponse == 3)
            {
                return true;
            }
        }
    }
    return false;
}