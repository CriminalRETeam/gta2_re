#include "Explosion.hpp"
#include "ped_death_cause.hpp"
#include "explosion_type.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Particle_4C.hpp"
#include "ParticleManager.hpp"
#include "Player.hpp"
#include "SpriteGrid.hpp"
#include "PedRefTable.hpp"
#include "debug.hpp"
#include "rng.hpp"

DEFINE_GLOBAL(ExplosionPool*, gExplosionPool_7A8, 0x6FD5F0);
DEFINE_GLOBAL(ExplosionPool_3D4*, gExplosionPool_3D4, 0x6FD5EC);

EXTERN_GLOBAL(u16, gParticleInstCount);

DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_6FD49C, Fix16(0), 0x6FD49C);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_6FD5D4, Ang16(0), 0x6FD5D4);

// Defined in Particle_4C.cpp: with the definition (and its dynamic initialiser) in this TU, VC6
// loads it with a 32-bit mov and adds with lea in the state_3/4/5/13 functions instead of the
// original's 16-bit mov/add (540D30 181 -> 32 diff lines).
EXTERN_GLOBAL(Ang16, kAng180_6FD3EE);

DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD448, Fix16(0x100, 0), 0x6FD448);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD328, kFpOne64th_6FD448, 0x6FD328);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_6FD330, kFpOne64th_6FD328 * 2, 0x6FD330);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne6400th, kFpOne64th_6FD328 / 100, 0x6FD450);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne3200th, kFpOne64th_6FD328 / 50, 0x6FD44C);

DEFINE_STATIC_GLOBAL_INIT(s16, sExplosionId20, 1, 0x623F1A); // ids for ExplosionPool_3D4

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FD2F0, Fix16(0xCCC, 0), 0x6FD2F0);

EXTERN_GLOBAL(Fix16, kFP16Half_6FD39C);
EXTERN_GLOBAL(Fix16, kFP16One_6FD4A0);
EXTERN_GLOBAL(Fix16, kFP16Two_6FD4A4);

EXTERN_GLOBAL(Fix16, gParticlePosX);
EXTERN_GLOBAL(Fix16, gParticlePosY);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFP16ThreeQuarters, Fix16(0x3000, 0), 0x6FD370);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FD2EC, Fix16(0x1000, 0), 0x6FD2EC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne1024th, Fix16(0x10, 0), 0x6FD540);
DEFINE_STATIC_MEMBER_INIT(Fix16, Explosion, kFpPoint09, Fix16(0x5C2, 0), 0x6FD484);
DEFINE_STATIC_MEMBER(u8, Explosion, sExplosionWantsFlash, 0x6FD5F6);

DEFINE_GLOBAL_INIT(Fix16, kFpOne512th_6FD548, Fix16(0x20, 0), 0x6FD548);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eight_6FD4C0, Fix16(8), 0x6FD4C0);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng24, Ang16(96), 0x6FD350);
DEFINE_GLOBAL_INIT(Ang16, kAng135_6FD40C, Ang16(540), 0x6FD40C);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng315, Ang16(1260), 0x6FD418);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng225, Ang16(900), 0x6FD3E0);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6FD35C, Ang16(180), 0x6FD35C);

DEFINE_STATIC_GLOBAL_INIT(Fix16_Point, kZeroPoint, Fix16_Point(Fix16(0), Fix16(0)), 0x6FD570);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint3_6FD2F4, Fix16(0x1333, 0), 0x6FD2F4);

EXTERN_GLOBAL(Fix16, kFpPoint1_6FD2E8);
EXTERN_GLOBAL(Fix16, kFpPoint05_6FD46C);

MATCH_FUNC(0x5408f0)
Explosion::Explosion()
{
    this->mAngle = 0;
    this->mUnused20 = 0;
    this->mSpawnAngle = 0;
    this->mPoolSlotIdx = 0;
    this->mType = explosion_type::none_0;
    this->mpObj2C = NULL;
    this->mParticleCooldown = 0;
    this->mTimer = 0;
    this->mSpeed8 = kFP16Zero_6FD49C;
    this->mUnused20 = kAngZero_6FD5D4;
    this->mSpawnAngle = kAngZero_6FD5D4;
    this->mParticleSpread = kFP16Zero_6FD49C;
    this->mBlastRadius = kFP16Zero_6FD49C;
    this->mAngle = kAngZero_6FD5D4;
    this->mpAttachedSprite = NULL;
    this->mId = 0;
    this->mOwnerPedId = 0;
}

MATCH_FUNC(0x540a10)
Explosion::~Explosion()
{
    mpObj2C = NULL;
    mpAttachedSprite = NULL;
}

// No callers. Every few frames, a particle thrown in a random diagonal direction from the attached object
MATCH_FUNC(0x540A40)
void Explosion::EmitRandomDiagonalParticle()
{
    LOCAL_STATIC(s16, sDebrisDirection, 0x6FD5F8);
    if (this->mParticleCooldown == 0)
    {
        Particle_4C* pParticle;
        sDebrisDirection = gRng.get_int(4);
        switch (sDebrisDirection)
        {
            case 0:
                pParticle = gParticle_8->New(kFpOne3200th, kFpOne3200th, kFpOneThirtySecond_6FD330, kFpOne6400th, kFpOne6400th, 0);
                break;
            case 1:
                pParticle = gParticle_8->New(-kFpOne3200th,
                                             kFpOne3200th,
                                             kFpOneThirtySecond_6FD330,
                                             -kFpOne6400th,
                                             kFpOne6400th,
                                             0);
                break;
            case 2:
                pParticle = gParticle_8->New(kFpOne3200th,
                                             -kFpOne3200th,
                                             kFpOneThirtySecond_6FD330,
                                             kFpOne6400th,
                                             -kFpOne6400th,
                                             0);
                break;
            case 3:
                pParticle = gParticle_8->New(-kFpOne3200th,
                                             -kFpOne3200th,
                                             kFpOneThirtySecond_6FD330,
                                             -kFpOne6400th,
                                             -kFpOne6400th,
                                             0);
                break;
            default:
                pParticle = NULL;
                break;
        }

        if (pParticle)
        {
            pParticle->mpExplosion = this;
            pParticle->mExplosionId = this->mId;
            if (mpObj2C->mSprite)
            {
                pParticle->mSpeed20 = mpObj2C->mSprite->mObject2CPtr->GetEffectiveSpeed();
                pParticle->mAngle = mpObj2C->mSprite->mObject2CPtr->mObj3c->mAngle;
            }
            pParticle->mUnused34 = 1;
            pParticle->mState = 6;
            pParticle->mCounter = 100;
            pParticle->mInitialCounter = 100;
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx);
            pParticle->mpOwnSprite->set_xyz_lazy(mpObj2C->mSprite->mXy.x, mpObj2C->mSprite->mXy.y, mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            this->mParticleCooldown = gRng.get_int(5);
        }
    }
    else
    {
        this->mParticleCooldown--;
    }
}

MATCH_FUNC(0x540d30)
void Explosion::EmitFireTrail_3_12(Ang16 ang, Fix16 speed)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the speed stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = speed;
    point.y = speed;
    point.RotateByAngle(ang + kAng180_6FD3EE);

    this->mSpeed8 = speed;
    this->mAngle = ang;

    if (this->mParticleCooldown == 0)
    {
        Particle_4C* pParticle = gParticle_8->New(point.x, point.y, kFpOneThirtySecond_6FD330, point.x, point.y, 0);
        if (pParticle)
        {
            pParticle->mpExplosion = this;
            pParticle->mExplosionId = this->mId;
            pParticle->mSpeed20 = speed;
            pParticle->mAngle = ang;
            pParticle->mUnused34 = 0;
            pParticle->mSubState = 0;
            pParticle->mCounter = 32;
            pParticle->mInitialCounter = 32;
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mState = 3;
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 96);
            pParticle->mpOwnSprite->SetNoLighting();
            pParticle->mpOwnSprite->set_xyz_lazy(mpObj2C->mSprite->mXy.x, mpObj2C->mSprite->mXy.y, mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            this->mParticleCooldown = gRng.get_int(2);
        }
    }
    else
    {
        this->mParticleCooldown--;
    }
}

// 9.6f 0x48E5F0
MATCH_FUNC(0x540f90)
void Explosion::EmitFireTrail_4(Ang16 ang, Fix16 speed)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the speed stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = speed;
    point.y = speed;
    point.RotateByAngle(ang + kAng180_6FD3EE);

    this->mSpeed8 = speed;
    this->mAngle = ang;

    if (this->mParticleCooldown == 0)
    {
        Particle_4C* pParticle = gParticle_8->New(point.x, point.y, kFpOneThirtySecond_6FD330, point.x, point.y, 0);
        if (pParticle)
        {
            pParticle->mpExplosion = this;
            pParticle->mExplosionId = this->mId;
            pParticle->mSpeed20 = speed;
            pParticle->mAngle = ang;
            pParticle->mUnused34 = 0;
            pParticle->mSubState = 0;
            pParticle->mCounter = 32;
            pParticle->mInitialCounter = 32;
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mState = 4;
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx);
            pParticle->mpOwnSprite->SetNoLighting();
            pParticle->mpOwnSprite->set_xyz_lazy(mpObj2C->mSprite->mXy.x, mpObj2C->mSprite->mXy.y, mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            this->mParticleCooldown = gRng.get_int(2);
        }
    }
    else
    {
        this->mParticleCooldown--;
    }
}

MATCH_FUNC(0x5411e0)
void Explosion::EmitFireTrail_13_14(Ang16 ang, Fix16 speed)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the speed stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = speed;
    point.y = speed;
    point.RotateByAngle(ang + kAng180_6FD3EE);

    this->mSpeed8 = speed;
    this->mAngle = ang;

    if (this->mParticleCooldown == 0)
    {
        Particle_4C* pParticle = gParticle_8->New(point.x, point.y, kFpOneThirtySecond_6FD330, point.x, point.y, 0);
        if (pParticle)
        {
            pParticle->mpExplosion = this;
            pParticle->mExplosionId = this->mId;
            pParticle->mSpeed20 = speed;
            pParticle->mAngle = ang;
            pParticle->mUnused34 = 0;
            pParticle->mSubState = 0;
            pParticle->mCounter = 32;
            pParticle->mInitialCounter = 32;
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mState = 36;
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx);
            pParticle->mpOwnSprite->SetNoLighting();
            pParticle->mpOwnSprite->set_xyz_lazy(mpObj2C->mSprite->mXy.x, mpObj2C->mSprite->mXy.y, mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            this->mParticleCooldown = gRng.get_int(2);
        }
    }
    else
    {
        this->mParticleCooldown--;
    }
}

MATCH_FUNC(0x541430)
void Explosion::EmitFireTrail_5(Ang16 ang, Fix16 speed)
{

    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = speed;
    point.y = speed;
    point.RotateByAngle(ang + kAng180_6FD3EE);

    this->mSpeed8 = speed;
    this->mAngle = ang;

    if (mpObj2C->GetEffectiveSpeed() == kFP16Zero_6FD49C && this->mTimer == explosion_timer::forever_9999)
    {
        this->mTimer = 20;
    }

    if (!this->mParticleCooldown)
    {
        Particle_4C* pParticle = gParticle_8->New(point.x, point.y, kFpOneThirtySecond_6FD330, point.x, point.y, 0);
        if (pParticle)
        {
            pParticle->mpExplosion = this;
            pParticle->mExplosionId = mId;
            pParticle->mSpeed20 = speed;
            pParticle->mAngle = ang;
            pParticle->mUnused34 = 0;
            pParticle->mSubState = 0;
            pParticle->mCounter = 32;
            pParticle->mInitialCounter = 32;
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mState = 5;
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 96);
            pParticle->mpOwnSprite->set_xyz_lazy(mpObj2C->mSprite->mXy.x,
                                                      mpObj2C->mSprite->mXy.y,
                                                      mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            this->mParticleCooldown = gRng.get_int(2);
            pParticle->mpOwnSprite->SetBlendAndAlpha(2, 20);
            pParticle->mpOwnSprite->SetNoLighting();
        }
    }
    else
    {
        this->mParticleCooldown--;
    }
}

MATCH_FUNC(0x541680)
Fix16 Explosion::GetBlastRadius() const
{
    Fix16 radius;
    switch (this->mType)
    {
        case explosion_type::small_18:
        case explosion_type::building_45_22:
        case explosion_type::building_225_23:
        case explosion_type::building_135_24:
        case explosion_type::building_315_25:
            radius = kFP16Half_6FD39C;
            break;
        case explosion_type::small_33:
            radius = kFP16ThreeQuarters;
            break;
        case explosion_type::item_19:
        case explosion_type::no_ring_32:
            radius = kFP16One_6FD4A0;
            break;
        case explosion_type::large_20:
            radius = kFP16Two_6FD4A4;
            break;
        default:
            break; // radius stays uninitialised, as in the original
    }
    return radius;
}


MATCH_FUNC(0x541710)
Fix16 Explosion::GetBlastHeight() const
{
    Fix16 height;
    switch (this->mType)
    {
        case explosion_type::small_18:
        case explosion_type::item_19:
        case explosion_type::large_20:
        case explosion_type::building_45_22:
        case explosion_type::building_225_23:
        case explosion_type::building_135_24:
        case explosion_type::building_315_25:
        case explosion_type::no_ring_32:
        case explosion_type::small_33:
            height = kFpPoint2_6FD2F0;
            break;
        default:
            break;
    }
    return height;
}

MATCH_FUNC(0x541760)
void Explosion::SpawnFlashParticle()
{
    if (mType != explosion_type::small_18 && mType != explosion_type::no_ring_32)
    {
        if (gParticle_4C_Pool->mpStart)
        {
            Particle_4C* pNew = gParticle_4C_Pool->Allocate();
            pNew->mSubState = 0;
            pNew->mState = 29;
            pNew->mpOwnSprite = gSpritePool->get_new_sprite();
            pNew->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pNew->mpOwnSprite->SetNoLighting();
            pNew->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 37);
            pNew->mpOwnSprite->set_xyz_lazy(mpObj2C->mSprite->mXy.x,
                                                      mpObj2C->mSprite->mXy.y,
                                                      mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pNew->mpOwnSprite);
            pNew->mTimer = 10;
        }
    }
}

MATCH_FUNC(0x541850)
void Explosion::ApplyBlastDamage(u16 timerVal)
{
    SpriteList collision_list;

    Fix16 half_height = Explosion::GetBlastHeight();

    Fix16 blast_radius = Explosion::GetBlastRadius();

    this->mBlastRadius = blast_radius;

    Fix16 left = mpObj2C->mSprite->mXy.x - mBlastRadius * kFP16Two_6FD4A4;
    Fix16 right = mpObj2C->mSprite->mXy.x + mBlastRadius * kFP16Two_6FD4A4;
    Fix16 top = mpObj2C->mSprite->mXy.y - mBlastRadius * kFP16Two_6FD4A4;
    Fix16 bottom = mpObj2C->mSprite->mXy.y + mBlastRadius * kFP16Two_6FD4A4;
    Fix16 z_low = mpObj2C->mSprite->mZpos - half_height;
    Fix16 z_high = mpObj2C->mSprite->mZpos + half_height;

    if (this->mTimer == explosion_timer::first_tick_99 && sExplosionWantsFlash == 1)
    {
        SpawnFlashParticle();
    }

    Fix16_Rect rect;
    rect.SetBounds(left, right, top, bottom);
    rect.SetHiLowZ(z_low, z_high);

    if (gSpriteGrid_1->CollectRectCollisions(&rect, false, sprite_types_enum::none_0, mpObj2C->mSprite, &collision_list))
    {
        while (collision_list.mP18)
        {
            Sprite* pSprite = collision_list.PopFrontSprite();
            Char* pPedChar = pSprite->AsCharB4();
            if (pPedChar)
            {
                if (timerVal > explosion_timer::damage_phase_50 && pPedChar->get_ped_state_1() != ped_state_1::immobilized_8)
                {
                    s32 ped_id = gPedRefTable_7F8->GetPedId(this->mpObj2C->mPedRefIdx);
                    if (!ped_id)
                    {
                        pPedChar->mpPed->mKillerId = this->mOwnerPedId;
                    }
                    else
                    {
                        pPedChar->mpPed->mKillerId = ped_id;
                    }
                    pPedChar->mpPed->mDeathCause = ped_death_cause::explosion_4;
                    pPedChar->mpPed->mKillerIdTimer = 50;

                    Fix16 dx = pSprite->mXy.x - this->mpObj2C->mSprite->mXy.x;
                    Fix16 dy = pSprite->mXy.y - this->mpObj2C->mSprite->mXy.y;

                    Ang16 ang;
                    ang = Fix16::atan2_fixed(dy, dx);

                    Fix16 distance = Fix16::MaxAbsDistance(pSprite->mXy.x, pSprite->mXy.y, mpObj2C->mSprite->mXy.x, mpObj2C->mSprite->mXy.y);
                    if (distance > this->mBlastRadius)
                    {
                        if (timerVal < 70u)
                        {
                            pPedChar->HandleGenericImpact(ang, kFpPoint1_6FD2E8 + kFpPoint05_6FD46C, kFP16Zero_6FD49C, 0);
                        }
                    }
                    else
                    {
                        char_type knockback_type;
                        Fix16 knockback_force;
                        if (distance < (kFP16Half_6FD39C * this->mBlastRadius))
                        {
                            knockback_force = kFpPoint3_6FD2F4;
                            knockback_type = 2;
                        }
                        else
                        {
                            knockback_force = kFpPoint1_6FD2E8;
                            knockback_type = 1;
                        }
                        pPedChar->HandleGenericImpact(ang, kFpPoint09, knockback_force, knockback_type);
                    }
                }
            }
            else
            {
                Car* pCar = pSprite->AsCar();
                if (pCar)
                {
                    if ((timerVal > explosion_timer::damage_phase_50 && timerVal < 60u) || (timerVal > 80u && timerVal < 90u))
                    {
                        if (!pCar->IsMaxDamage() && !pCar->IsTrainModel() && !pCar->IsImmuneToExplosionType(mType))
                        {
                            if (Fix16::MaxAbsDistance(pSprite->mXy.x, pSprite->mXy.y, mpObj2C->mSprite->mXy.x, mpObj2C->mSprite->mXy.y) <= this->mBlastRadius)
                            {
                                // 9.6f: PedRefTable::GetPedId (inlined, using it here makes the diff worse)
                                s32 exploder_ped_id = gPedRefTable_7F8->mEntries[this->mpObj2C->mPedRefIdx].mPedId;
                                if (!exploder_ped_id)
                                {
                                    pCar->mExploderPedId = this->mOwnerPedId;
                                }
                                else
                                {
                                    pCar->mExploderPedId = exploder_ped_id;
                                }
                                pCar->mDeathCause = ped_death_cause::explosion_4;
                                pCar->mExploderTimer = 50;
                                s16 damage = pCar->AccumulateDamage(32000, &kZeroPoint);
                                if (pCar->mExploderPedId)
                                {
                                    if (damage > 0)
                                    {
                                        Ped* pExploderPed = gPedManager->PedById(pCar->mExploderPedId);
                                        if (pExploderPed)
                                        {
                                            if (pExploderPed->PedTypeIs(ped_type::player_2))
                                            {
                                                pExploderPed->mPlayer->mScores.AwardCarDamageScoreHit(pCar, 1);
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                pCar->TriggerCarAlarm();
                            }
                        }
                    }
                    else if (timerVal == explosion_timer::first_tick_99)
                    {
                        if (Fix16::MaxAbsDistance(pSprite->mXy.x, pSprite->mXy.y, mpObj2C->mSprite->mXy.x, mpObj2C->mSprite->mXy.y) <= this->mBlastRadius)
                        {
                            pCar->ApplyExplosionImpulse(&this->mpObj2C->mSprite->get_x_y_443580());
                        }
                    }
                }
                else
                {
                    Object* pObject = pSprite->AsObject2C();
                    if (timerVal > explosion_timer::damage_phase_50 && timerVal < 60u)
                    {
                        pObject->OnExplosionNearby(this->mpObj2C->mPedRefIdx);
                    }
                }
            }
        }
    }
}

// 9.6f 0x48EB00
MATCH_FUNC(0x541d60)
void Explosion::EmitExplosion_18_33()
{
    if (gParticle_4C_Pool->has_pStart())
    {
        if (mTimer > 0x52u)
        {
            if (mTimer > 0x5Au)
            {
                Fix16 radius;
                radius = this->mParticleSpread * Fix16(gRng.get_int(8));

                this->mSpawnAngle = Ang16::Fix16_To_Ang16_inlined_40F540(kFpOne64th_6FD448 * Fix16(gRng.get_int(360)));

                // 9.6f calls Ang16::PolarToCartesian_41FC20
                Ang16::PolarToCartesian_41FC20(mSpawnAngle, radius, gParticlePosX, gParticlePosY);

                // NOTE: This proves these 2 vars are not a Fix16_Point
                gParticlePosX = this->mpObj2C->mSprite->mXy.x + gParticlePosX;
                gParticlePosY = this->mpObj2C->mSprite->mXy.y + gParticlePosY;

                Particle_4C* pParticle = gParticle_4C_Pool->Allocate();
                pParticle->mSubState = 0;
                pParticle->mState = 18;
                pParticle->mpOwnSprite = gSpritePool->get_new_sprite();
                pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pParticle->mpOwnSprite->SetNoLighting();
                pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 20);
                pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpObj2C->mSprite->mZpos);
                gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
                pParticle->mTimer = 1;
            }
            else
            {
                gParticlePosX = this->mpObj2C->mSprite->mXy.x;
                gParticlePosY = this->mpObj2C->mSprite->mXy.y;

                Particle_4C* pParticle = gParticle_4C_Pool->Allocate();
                pParticle->mSubState = 0;
                pParticle->mState = 18;
                pParticle->mpOwnSprite = gSpritePool->get_new_sprite();
                pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pParticle->mpOwnSprite->SetNoLighting();
                pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 20);
                pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpObj2C->mSprite->mZpos);
                gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
                pParticle->mpOwnSprite->ApplyScaleToDimensions(kFP16Quarter_6FD2EC, 0);
                pParticle->mTimer = 5;
            }
        }
    }
}

MATCH_FUNC(0x542060)
void Explosion::EmitExplosion_19_32()
{
    if (gParticle_4C_Pool->has_pStart())
    {
        if (this->mTimer > 8u)
        {
            Fix16 radius;
            radius = this->mParticleSpread * Fix16(gRng.get_int(48));
            this->mSpawnAngle = Ang16::Fix16_To_Ang16_inlined_40F540(kFpOne64th_6FD448 * Fix16(gRng.get_int(360)));

            // 9.6f calls Ang16::PolarToCartesian_41FC20
            Ang16::PolarToCartesian_41FC20(mSpawnAngle, radius, gParticlePosX, gParticlePosY);

            gParticlePosX = this->mpObj2C->mSprite->mXy.x + gParticlePosX;
            gParticlePosY = this->mpObj2C->mSprite->mXy.y + gParticlePosY;

            Particle_4C* pParticle = gParticle_4C_Pool->Allocate();
            pParticle->mSubState = 0;
            pParticle->mState = 19;
            pParticle->mpOwnSprite = gSpritePool->get_new_sprite();
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mpOwnSprite->SetNoLighting();
            pParticle->mpExplosion = this;
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 20);
            pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, this->mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            pParticle->mpOwnSprite->ApplyScaleToDimensions(kFP16Half_6FD39C + kFP16One_6FD4A0, 0);
            pParticle->mTimer = 1;
        }
        else
        {
            gParticlePosX = this->mpObj2C->mSprite->mXy.x;
            gParticlePosY = this->mpObj2C->mSprite->mXy.y;

            Particle_4C* pParticle = gParticle_4C_Pool->Allocate();
            pParticle->mSubState = 0;
            pParticle->mState = 19;
            pParticle->mpOwnSprite = gSpritePool->get_new_sprite();
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mpOwnSprite->SetNoLighting();
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 20);
            pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            pParticle->mTimer = 5;
        }
    }
}

MATCH_FUNC(0x542340)
void Explosion::EmitExplosion_20()
{
    if (gParticle_4C_Pool->has_pStart())
    {
        if (this->mTimer > 8u)
        {
            Fix16 radius;
            radius = this->mParticleSpread * Fix16(gRng.get_int(80));
            this->mSpawnAngle = Ang16::Fix16_To_Ang16_inlined_40F540(kFpOne64th_6FD448 * Fix16(gRng.get_int(360)));

            // 9.6f calls Ang16::PolarToCartesian_41FC20
            Ang16::PolarToCartesian_41FC20(mSpawnAngle, radius, gParticlePosX, gParticlePosY);

            gParticlePosX = this->mpObj2C->mSprite->mXy.x + gParticlePosX;
            gParticlePosY = this->mpObj2C->mSprite->mXy.y + gParticlePosY;

            Particle_4C* pParticle = gParticle_4C_Pool->Allocate();
            pParticle->mSubState = 0;
            pParticle->mState = 20;
            pParticle->mpOwnSprite = gSpritePool->get_new_sprite();
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mpOwnSprite->SetNoLighting();
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 56);
            pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            pParticle->mTimer = 1;
        }
        else
        {
            gParticlePosX = this->mpObj2C->mSprite->mXy.x;
            gParticlePosY = this->mpObj2C->mSprite->mXy.y;

            Particle_4C* pParticle = gParticle_4C_Pool->Allocate();
            pParticle->mSubState = 0;
            pParticle->mState = 20;
            pParticle->mpOwnSprite = gSpritePool->get_new_sprite();
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mpOwnSprite->SetNoLighting();
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 56);
            pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpObj2C->mSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            pParticle->mTimer = 5;
        }
    }
}

MATCH_FUNC(0x542790)
void Explosion::UpdateExplosion_18_19_20_32_33()
{
    bool isOnScreen = true;
    if (this->mTimer < 90u)
    {
        if (!gGame_0x40->IsPointOnScreen(this->mpObj2C->mSprite->mXy.x, this->mpObj2C->mSprite->mXy.y))
        {
            isOnScreen = false;
        }
    }

    sExplosionWantsFlash = 0;
    if (isOnScreen)
    {
        switch (this->mType)
        {
            case explosion_type::small_18:
            case explosion_type::small_33:
                EmitExplosion_18_33();
                break;
            case explosion_type::item_19:
            case explosion_type::no_ring_32:
                EmitExplosion_19_32();
                sExplosionWantsFlash = 1;
                break;
            case explosion_type::large_20:
                EmitExplosion_20();
                sExplosionWantsFlash = 1;
                break;
            default:
                break;
        }

        switch (this->mTimer)
        {
            case 59:
            case 69:
            case 79:
            case 89:
            {
                Object* pExplosionObj = gObject_5C->CreateExplosion(Fix16(113), Fix16(145), 2, kAngZero_6FD5D4, explosion_type::molotov_fire_5, mOwnerPedId);
                if (pExplosionObj)
                {
                    Object* pDebris = gObject_5C->NewMovingObj(objects::explosion_fragments_127,
                                                               mpObj2C->mSprite->mXy.x,
                                                               mpObj2C->mSprite->mXy.y,
                                                               mpObj2C->mSprite->mZpos,
                                                               this->mSpawnAngle,
                                                               kAngZero_6FD5D4,
                                                               kFpPoint09,
                                                               -kFpOne1024th,
                                                               kFpPoint2_6FD2F0);
                    pDebris->mSprite->AttachSprite(pExplosionObj->mSprite, 0, 0, kAngZero_6FD5D4);

                    Object* pLight = gObject_5C->NewLight(94, 138, 2, 0xFF8000, 3, 255);
                    pDebris->mSprite->AttachSprite(pLight->mSprite, 0, 0, kAngZero_6FD5D4);
                }
                break;
            }

            default:
                break;
        }
    }

    if (this->mTimer > 0x1Eu)
    {
        Fix16 max_spread = (kFpOne64th_6FD448 * kFP16Two_6FD4A4);
        if (this->mParticleSpread > max_spread)
        {
            this->mParticleSpread = max_spread;
        }
        this->mParticleSpread += (kFpOne1024th / kFP16Two_6FD4A4);
    }
    else
    {
        if (this->mParticleSpread > kFpOne64th_6FD448)
        {
            this->mParticleSpread = kFpOne64th_6FD448;
        }
        this->mParticleSpread -= (kFpOne1024th / kFP16Two_6FD4A4);
    }

    if (this->mTimer > explosion_timer::damage_phase_50)
    {
        ApplyBlastDamage(this->mTimer);
    }

    if (this->mTimer == explosion_timer::first_tick_99)
    {
        gGame_0x40->ShakeCamerasAtPos(8, this->mpObj2C->mSprite->mXy.x, this->mpObj2C->mSprite->mXy.y);
    }

    if (this->mTimer != explosion_timer::forever_9999)
    {
        this->mTimer--;
    }

    if (this->mTimer > explosion_timer::forever_9999)
    {
        this->mTimer = 1;
    }
}

MATCH_FUNC(0x542e30)
void Explosion::EmitBuildingDebris_22_23_24_25(u8 direction_idx)
{
    Sprite* pSprite = this->mpObj2C->mSprite;
    if (pSprite->mXy.x < Fix16(254) && pSprite->mXy.x > kFP16One_6FD4A0 &&
        pSprite->mXy.y < Fix16(254) && pSprite->mXy.y > kFP16One_6FD4A0)
    {
        sExplosionWantsFlash = 0;
        for (u8 i = 0; i < 2u; i++)
        {
            if (gParticle_4C_Pool->mpStart)
            {
                Particle_4C* pParticle = gParticle_4C_Pool->Allocate();
                pParticle->mSubState = 0;

                switch (direction_idx)
                {
                    case building_debris_dir::towards_135_0:
                    {
                        pParticle->mState = 24;
                        this->mSpawnAngle = (kAng135_6FD40C + kAng24) + Ang16::Fix16_To_Ang16((kFpOne64th_6FD448 * Fix16(gRng.get_int(45))));

                        Ang16::PolarToCartesian_41FC20(mSpawnAngle, kFpOne1024th, gParticlePosX, gParticlePosY);

                        gParticlePosX += this->mpObj2C->mSprite->mXy.x;
                        gParticlePosY += this->mpObj2C->mSprite->mXy.y;
                        break;
                    }

                    case building_debris_dir::towards_315_1:
                    {
                        pParticle->mState = 25;
                        this->mSpawnAngle = (kAng315 + kAng24) + Ang16::Fix16_To_Ang16((kFpOne64th_6FD448 * Fix16(gRng.get_int(90))));


                        Ang16::PolarToCartesian_41FC20(mSpawnAngle, kFpOne1024th, gParticlePosX, gParticlePosY);

                        gParticlePosX += this->mpObj2C->mSprite->mXy.x;
                        gParticlePosY += this->mpObj2C->mSprite->mXy.y;
                        break;
                    }

                    case building_debris_dir::towards_225_2:
                    {
                        pParticle->mState = 23;
                        this->mSpawnAngle = (kAng225 + kAng24).Add_ool(Ang16::Fix16_To_Ang16((kFpOne64th_6FD448 * Fix16(gRng.get_int(90)))));


                        Ang16::PolarToCartesian_41FC20(mSpawnAngle, kFpOne1024th, gParticlePosX, gParticlePosY);

                        gParticlePosX += this->mpObj2C->mSprite->mXy.x;
                        gParticlePosY += this->mpObj2C->mSprite->mXy.y;
                        break;
                    }

                    case building_debris_dir::towards_45_3:
                    {
                        pParticle->mState = 22;
                        this->mSpawnAngle = kAng45_6FD35C.Add_ool(kAng24).Add_ool(Ang16::Fix16_To_Ang16_ool_40F540((kFpOne64th_6FD448 * Fix16(gRng.get_int(90)))));


                        Ang16::PolarToCartesian_41FC20(mSpawnAngle, kFpOne1024th, gParticlePosX, gParticlePosY);

                        gParticlePosX += this->mpObj2C->mSprite->mXy.x;
                        gParticlePosY += this->mpObj2C->mSprite->mXy.y;
                        break;
                    }

                    default:
                        break;
                }

                pParticle->mpOwnSprite = gSpritePool->get_new_sprite();
                pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pParticle->mTimer = 0;
                pParticle->mSubState = 0;
                pParticle->mAngle = this->mSpawnAngle;

                if (this->mTimer < 60u && this->mTimer < 30u)
                {
                    i = 4;
                }

                pParticle->mSpeed20 = (kFpOne512th_6FD548 * Fix16(gRng.get_int(mTimer)));
                pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 40);

                if (mpObj2C->mSprite->mZpos + kFP16One_6FD4A0 >= kFP16Eight_6FD4C0)
                {
                    pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpObj2C->mSprite->mZpos);
                }
                else
                {
                    pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpObj2C->mSprite->mZpos + kFP16One_6FD4A0);
                }
                gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
                pParticle->mpOwnSprite->SetNoLighting();
            }
        }

        if (this->mTimer == explosion_timer::first_tick_99)
        {
            gGame_0x40->ShakeCamerasAtPos(8, mpObj2C->mSprite->mXy.x, mpObj2C->mSprite->mXy.y);
        }

        if (this->mTimer > explosion_timer::damage_phase_50)
        {
            ApplyBlastDamage(this->mTimer);
        }

        if (this->mTimer != explosion_timer::forever_9999)
        {
            if (this->mTimer > 60u)
            {
                this->mTimer--;
            }

            if (this->mTimer > explosion_timer::forever_9999)
            {
                this->mTimer = 1;
            }
        }
    }
}

MATCH_FUNC(0x5434a0)
bool Explosion::Update(Fix16 speed, Ang16 ang)
{
    u16 timer = this->mTimer;
    if (timer != explosion_timer::forever_9999)
    {
        if (timer > 0)
        {
            this->mTimer = timer - 1;
        }
    }

    if (!this->mTimer)
    {
        Explosion::Release();
        return true;
    }

    if (bSkip_particles)
    {
        return true;
    }

    switch (this->mType)
    {
        case explosion_type::fire_3:
        case explosion_type::car_fire_level1_12:
            Explosion::EmitFireTrail_3_12(ang, speed);
            return false;
        case explosion_type::car_fire_level2_13:
        case explosion_type::car_fire_level3_14:
            Explosion::EmitFireTrail_13_14(ang, speed);
            return false;
        case explosion_type::car_fire_4:
            Explosion::EmitFireTrail_4(ang, speed);
            return false;
        case explosion_type::molotov_fire_5:
            Explosion::EmitFireTrail_5(ang, speed);
            return false;
        case explosion_type::small_18:
        case explosion_type::item_19:
        case explosion_type::large_20:
        case explosion_type::no_ring_32:
        case explosion_type::small_33:
            Explosion::UpdateExplosion_18_19_20_32_33();
            return false;
        case explosion_type::building_135_24:
            Explosion::EmitBuildingDebris_22_23_24_25(building_debris_dir::towards_135_0);
            return false;
        case explosion_type::building_315_25:
            Explosion::EmitBuildingDebris_22_23_24_25(building_debris_dir::towards_315_1);
            return false;
        case explosion_type::building_225_23:
            Explosion::EmitBuildingDebris_22_23_24_25(building_debris_dir::towards_225_2);
            return false;
        case explosion_type::building_45_22:
            Explosion::EmitBuildingDebris_22_23_24_25(building_debris_dir::towards_45_3);
            return false;
    }
    return false;
}

MATCH_FUNC(0x5435d0)
bool Explosion::IsFireType() const
{
    switch (mType)
    {
        case explosion_type::fire_3:
        case explosion_type::car_fire_4:
        case explosion_type::molotov_fire_5:
        case explosion_type::car_fire_level1_12:
        case explosion_type::car_fire_level2_13:
        case explosion_type::car_fire_level3_14:
            return true;
        default:
            return false;
    }
}

MATCH_FUNC(0x543610)
void Explosion::Release()
{
    this->mId = 0;
    if (mbIn20Pool == 0)
    {
        gExplosionPool_7A8->mbUsed[this->mPoolSlotIdx] = false;
    }
    else
    {
        gExplosionPool_3D4->mbUsed[this->mPoolSlotIdx] = false;
    }
}

MATCH_FUNC(0x543650)
void Explosion::Init()
{
    this->mType = explosion_type::none_0;
    this->mParticleCooldown = 0;
    this->mParticleSpread = 0;
    this->mSpawnAngle = kAngZero_6FD5D4;
    this->mTimer = 200;
    this->mpObj2C = NULL;
    this->mOwnerPedId = 0;
    this->mbIn20Pool = 0;
}

MATCH_FUNC(0x543680)
void Explosion::SetObject(Object* pObj)
{
    this->mpObj2C = pObj;
}

// Every explosion gets a priority from its type: 1 (unknown types, evicted straight away) up to 6, and a late fire or
// explosion counts as 3. The one with the lowest priority has its timer set to 0 so that it ends on its next update.
// The early exit returns the freed index (eax), the fall-through path returns nothing like the original.
#pragma warning(push)
#pragma warning(disable : 4715) // not all control paths return a value
MATCH_FUNC(0x543690)
s32 ExplosionPool::FreeLowestPriority()
{
    u8 lowest_priority = 99;
    u8 lowest_idx = 99;
    u8 priority = 0;
    u8 idx = 0;
    u8 cur_idx = 0;
    do
    {
        if (this->mbUsed[cur_idx] == 1)
        {
            Explosion* pExplosion = &this->mExplosions[cur_idx];
            // Each case written out on its own: merged labels give a byte index table, the
            // original has one dword entry per case. Cases 1 and 39 keep the range.
            switch (pExplosion->mType)
            {
                case explosion_type::unknown_2:
                    break;
                case explosion_type::fire_3:
                    break;
                case explosion_type::car_fire_4:
                    break;
                case explosion_type::unknown_21:
                    break;
                case explosion_type::flame_stream_31:
                    break;
                case explosion_type::fire_truck_spray_34:
                    break;
                case explosion_type::molotov_fire_5:
                    priority = 2;
                    break;
                case explosion_type::unknown_28:
                    priority = 2;
                    break;
                case explosion_type::flash_29:
                    priority = 2;
                    break;
                case explosion_type::unknown_30:
                    priority = 2;
                    break;
                case explosion_type::car_fire_level2_13:
                    priority = 4;
                    break;
                case explosion_type::car_fire_level1_12:
                    priority = 5;
                    break;
                case explosion_type::car_fire_level3_14:
                    priority = 5;
                    break;
                case explosion_type::unknown_15:
                    priority = 5;
                    break;
                case explosion_type::unknown_16:
                    priority = 6;
                    break;
                case explosion_type::unknown_17:
                    priority = 6;
                    break;
                case explosion_type::small_18:
                    if (pExplosion->mTimer < 82u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::small_33:
                    if (pExplosion->mTimer < 82u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::item_19:
                    if (pExplosion->mTimer < 50u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::large_20:
                    if (pExplosion->mTimer < 50u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::no_ring_32:
                    if (pExplosion->mTimer < 50u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::building_45_22:
                    priority = 3;
                    break;
                case explosion_type::building_225_23:
                    priority = 3;
                    break;
                case explosion_type::building_135_24:
                    priority = 3;
                    break;
                case explosion_type::building_315_25:
                    priority = 3;
                    break;
                case explosion_type::blood_burst_1:
                    priority = 1;
                    break;
                case explosion_type::blood_39:
                    priority = 1;
                    break;
                default:
                    priority = 1;
                    break;
            }

            if (priority == 1)
            {
                s32 i = cur_idx;
                this->mExplosions[i].mTimer = 0;
                return i;
            }

            if (priority < lowest_priority)
            {
                lowest_priority = priority;
                lowest_idx = idx;
            }
        }
        cur_idx = ++idx;
    } while (idx < GTA2_COUNTOF(mExplosions));
    this->mExplosions[lowest_idx].mTimer = 0;
}
#pragma warning(pop)

MATCH_FUNC(0x543800)
Explosion* ExplosionPool::Allocate()
{
    LOCAL_STATIC_INIT(s16, sExplosionId, 1, 0x623F18);
    // 9.6f has the init block twice, 10.5 merges both into one block. Indexing mExplosions at each
    // use (no pNew local) gives both copies the same registers, so they merge completely.
    u8 idx;
    // the first 20 slots are tried first; only when they are all used the lowest priority explosion is evicted
    for (idx = 0; idx < 20; idx++)
    {
        if (!this->mbUsed[idx])
        {
            this->mExplosions[idx].Init();
            this->mExplosions[idx].mPoolSlotIdx = idx;
            this->mExplosions[idx].mId = sExplosionId;
            this->mExplosions[idx].mbIn20Pool = 0;
            sExplosionId++;
            this->mbUsed[idx] = true;
            return &this->mExplosions[idx];
        }
    }

    FreeLowestPriority();

    for (idx = 0; idx < GTA2_COUNTOF(mExplosions); idx++)
    {
        if (!this->mbUsed[idx])
        {
            this->mExplosions[idx].Init();
            this->mExplosions[idx].mPoolSlotIdx = idx;
            this->mExplosions[idx].mId = sExplosionId;
            this->mExplosions[idx].mbIn20Pool = 0;
            sExplosionId++;
            this->mbUsed[idx] = true;
            return &this->mExplosions[idx];
        }
    }
    return NULL;
}

MATCH_FUNC(0x5438b0)
ExplosionPool::ExplosionPool()
{
    for (u8 i = 0; i < GTA2_COUNTOF(mExplosions); i++)
    {
        mExplosions[i].mPoolSlotIdx = i;
        mbUsed[i] = false;
    }

    gParticleInstCount = 0;
}

MATCH_FUNC(0x5438f0)
ExplosionPool::~ExplosionPool()
{
}

MATCH_FUNC(0x543900)
Explosion* ExplosionPool_3D4::Allocate()
{
    for (u8 idx = 0; idx < GTA2_COUNTOF(mExplosions); idx++)
    {
        if (!this->mbUsed[idx])
        {
            this->mExplosions[idx].Init();
            this->mExplosions[idx].mPoolSlotIdx = idx;
            this->mExplosions[idx].mId = sExplosionId20;
            this->mExplosions[idx].mbIn20Pool = 1;
            sExplosionId20++;
            this->mbUsed[idx] = true;
            return &this->mExplosions[idx];
        }
    }
    return NULL;
}

MATCH_FUNC(0x543980)
ExplosionPool_3D4::ExplosionPool_3D4()
{
    for (u8 i = 0; i < GTA2_COUNTOF(mbUsed); i++)
    {
        mExplosions[i].mPoolSlotIdx = i;
        mbUsed[i] = false;
    }
    gParticleInstCount = 0;
}

MATCH_FUNC(0x5439c0)
ExplosionPool_3D4::~ExplosionPool_3D4()
{
}