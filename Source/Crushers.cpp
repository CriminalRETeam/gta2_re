#include "Crushers.hpp"
#include "explosion_type.hpp"
#include "ped_death_cause.hpp"
#include "car_despawn_status.hpp"
#include "Camera.hpp"
#include "Car.hpp"
#include "Globals.hpp"
#include "SoundObject.hpp"
#include "root_sound.hpp"
#include "Object_5C.hpp"
#include "debug.hpp"
#include "map_0x370.hpp"
#include "winmain.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include <stdio.h>

DEFINE_GLOBAL(CrusherPool*, gCrusherPool_94_67A830, 0x67A830);

DEFINE_STATIC_MEMBER_INIT(Fix16, Crusher, kCrusherOpenDist, Fix16(0x3C00, 0), 0x67A7D0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kCrusherClosedDist, Fix16(0x2400, 0), 0x67A4E4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kCrusherPistonBaseDist, Fix16(0x4800, 0), 0x67A664);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kCrusherSpeed, Fix16(0x100, 0), 0x67A5F4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kCrusherPistonHalfDepth, Fix16(0x1C00, 0), 0x67A810);

DEFINE_GLOBAL(Ang16, kAngZero_67A820, 0x67A820);
DEFINE_GLOBAL_INIT(Ang16, kAng270_67A660, Ang16(0x438), 0x67A660);
DEFINE_GLOBAL_INIT(Ang16, kAng180_67A63A, Ang16(0x2D0), 0x67A63A);
DEFINE_GLOBAL_INIT(Ang16, kAng90_67A568, Ang16(0x168), 0x67A568);

MATCH_FUNC(0x4882d0)
Crusher::Crusher()
{
    mSound = NULL;
}

MATCH_FUNC(0x4882e0)
Crusher::~Crusher()
{
    if (mSound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(mSound);
        mSound = NULL;
    }
}

MATCH_FUNC(0x488310)
void Crusher::CrushCar_488310(Car* pCar)
{
    if (mState == CrusherStates::Idle_0 && !pCar->IsDespawning_4215B0() && !pCar->IsSpriteShrunk_43DC00() && !pCar->IsEngineCrushed_447EB0())
    {
        mpCarBeingCrushed = pCar;
        mState = CrusherStates::CrushW_1;
        pCar->SetNoCollide_44A3E0();
    }
}

MATCH_FUNC(0x488350)
void Crusher::Service_488350()
{
    if (mState != CrusherStates::Idle_0)
    {
        if (mpCarBeingCrushed->mDespawnStatus == car_despawn_status::despawn_pending_2 || mpCarBeingCrushed->mDespawnStatus == car_despawn_status::marked_for_despawn_4 || mpCarBeingCrushed->mDespawnStatus == car_despawn_status::despawn_soon_3)
        {
            mpCarBeingCrushed->mDespawnStatus = car_despawn_status::active_1;
        }

        switch (mState)
        {
            case CrusherStates::CrushW_1:
                mW -= kCrusherSpeed;
                if (mW == kCrusherClosedDist)
                {
                    mState = CrusherStates::UnCrushW_2;
                    if (!bStartNetworkGame_7081F0)
                    {
                        mpCarBeingCrushed->mExploderPedId = gGame_0x40_67E008->mLocalPlayer->mPlayerPed->mId;
                        mpCarBeingCrushed->mDeathCause = ped_death_cause::explosion_4;
                        mpCarBeingCrushed->mExploderTimer = 50;
                    }
                    mpCarBeingCrushed->HandleCarExplosion_43D840(explosion_type::item_19);
                    mpCarBeingCrushed->mAttachments.CleanupSpriteList_5A7080();
                }
                if (mW - kCrusherPistonHalfDepth <= mpCarBeingCrushed->GetCarWidth_48A930() / 2)
                {
                    mpCarBeingCrushed->CarShrinkSprite_43DC80(2, 0);
                }
                break;

            case CrusherStates::UnCrushW_2:
                mW += kCrusherSpeed;
                if (mW == kCrusherOpenDist)
                {
                    mState = CrusherStates::CrushH_3;
                }
                break;

            case CrusherStates::CrushH_3:
            {
                mH -= kCrusherSpeed;
                if (mH == kCrusherClosedDist)
                {
                    mpCarBeingCrushed->mEngineStatus = car_engine_status::crushed_7;
                    mState = CrusherStates::UnCrushH_4;
                }

                if (mH - kCrusherPistonHalfDepth <= (mpCarBeingCrushed->GetCarHeight_48A950() / 2))
                {
                    mpCarBeingCrushed->CarShrinkSprite_43DC80(0, 2);
                }
            }
            break;

            case CrusherStates::UnCrushH_4:
                mH += kCrusherSpeed;
                if (mH == kCrusherOpenDist)
                {
                    mpCarBeingCrushed = NULL;
                    mState = CrusherStates::Idle_0;
                }
                break;

            default:
                break;
        }

        Object* Obj_0 = mPistonTop;
        Obj_0->RemoveFromCollisionBuckets_527D00();
        Obj_0->mSprite->set_xy_lazy_447E20(mXpos, mYpos - mH);
        Obj_0->AssignToBucket_527AE0();

        Object* Obj_4 = mPistonRight;
        Obj_4->RemoveFromCollisionBuckets_527D00();
        Obj_4->mSprite->set_xy_lazy_447E20(mXpos + mW, mYpos);
        Obj_4->AssignToBucket_527AE0();

        Object* Obj_8 = mPistonBottom;
        Obj_8->RemoveFromCollisionBuckets_527D00();
        Obj_8->mSprite->set_xy_lazy_447E20(mXpos, mH + mYpos);
        Obj_8->AssignToBucket_527AE0();

        Object* Obj_C = mPistonLeft;
        Obj_C->RemoveFromCollisionBuckets_527D00();
        Obj_C->mSprite->set_xy_lazy_447E20(mXpos - mW, mYpos);
        Obj_C->AssignToBucket_527AE0();
    }
}

MATCH_FUNC(0x4885a0)
void Crusher::InitCrusher_4885A0(Fix16 xpos, Fix16 ypos, char_type crusher_idx)
{
    Fix16 zpos = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(xpos, ypos);

    mXpos = xpos;
    mYpos = ypos;

    mCentralSpot = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_central_spot_143, xpos, ypos, zpos, kAngZero_67A820);
    mCentralSpot->mPedRefIdx = crusher_idx;

    mPistonTop = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_142,
                                                                 mXpos,
                                                                 mYpos - kCrusherOpenDist,
                                                                 zpos,
                                                                 kAngZero_67A820);
    mPistonRight = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_142,
                                                                   mXpos + kCrusherOpenDist,
                                                                   mYpos,
                                                                   zpos,
                                                                   kAng270_67A660);
    mPistonBottom = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_142,
                                                                    mXpos,
                                                                    kCrusherOpenDist + mYpos,
                                                                    zpos,
                                                                    kAng180_67A63A);
    mPistonLeft = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_142,
                                                                  mXpos - kCrusherOpenDist,
                                                                  mYpos,
                                                                  zpos,
                                                                  kAng90_67A568);

    gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_base_150,
                                            mXpos,
                                            mYpos - kCrusherPistonBaseDist,
                                            zpos,
                                            kAngZero_67A820);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_base_150,
                                            mXpos + kCrusherPistonBaseDist,
                                            mYpos,
                                            zpos,
                                            kAng270_67A660);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_base_150,
                                            mXpos,
                                            kCrusherPistonBaseDist + mYpos,
                                            zpos,
                                            kAng180_67A63A);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_base_150,
                                            mXpos - kCrusherPistonBaseDist,
                                            mYpos,
                                            zpos,
                                            kAng90_67A568);

    mW = kCrusherOpenDist;
    mH = kCrusherOpenDist;

    mState = CrusherStates::Idle_0;
    mpCarBeingCrushed = NULL;

    if (!mSound && !bSkip_audio_67D6BE)
    {
        mSound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::Crusher_30_9);
    }
}

MATCH_FUNC(0x4887a0)
bool CrusherPool::IsCarBeingCrushed_4887A0(Car* pCar)
{
    for (s32 i = 0; i < mCount; i++)
    {
        if (mCrushers[i].GetCarBeingCrushed() == pCar)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4887d0)
void CrusherPool::CrushCarWithCrusher_4887D0(Car* pCar, u8 crusher_idx)
{
    mCrushers[crusher_idx].CrushCar_488310(pCar);
}

MATCH_FUNC(0x4887f0)
void CrusherPool::CrushersService_4887F0()
{
    s32 idx = 0;
    Crusher* pIter = &mCrushers[0];
    while (idx < mCount)
    {
        pIter->Service_488350();
        idx++;
        pIter++;
    }
}

MATCH_FUNC(0x488820)
Crusher* CrusherPool::CreateCrusher_488820(Fix16 xpos, Fix16 ypos)
{
    Crusher* pNew = &mCrushers[mCount];
    pNew->InitCrusher_4885A0(xpos, ypos, mCount);
    mCount++;
    return pNew;
}

MATCH_FUNC(0x488860)
CrusherPool::CrusherPool()
{
    mCount = 0;
}

MATCH_FUNC(0x4bbc30)
CrusherPool::~CrusherPool()
{
}
