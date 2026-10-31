#include "ParticleManager.hpp"
#include "Car.hpp"
#include "CarPhysics.hpp"
#include "Globals.hpp"
#include "Particle_4C.hpp"
#include "ObjectDefinitions.hpp"
#include "Pool.hpp"
#include "SpriteGrid.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "Object_5C.hpp"
#include "sprite.hpp"
#include "rng.hpp"

// Forward declarations: the functions below are in address order
static inline Ang16 AddAng16_ool(const Ang16& a, const Ang16& b);


EXTERN_GLOBAL(Fix16, kFpPoint05_6FD46C);
EXTERN_GLOBAL(Fix16, gParticlePosX);
EXTERN_GLOBAL(Fix16, gParticlePosY);
EXTERN_GLOBAL(Fix16, kFpOne512th_6FD548);
EXTERN_GLOBAL(Fix16, kFpOneThirtySecond_6FD330);
EXTERN_GLOBAL(Fix16, kFpOne64th_6FD448);
EXTERN_GLOBAL(Fix16, kFpPoint1_6FD2E8);
EXTERN_GLOBAL(Fix16, kFP16Quarter_6FD2EC);
EXTERN_GLOBAL(Fix16, kFpOne64th_6FD554);
EXTERN_GLOBAL(Fix16, kFpMinusOneThirtySecond);
EXTERN_GLOBAL(Fix16, kFpPoint2PlusOne64th);

EXTERN_GLOBAL(Ang16, kAngZero_6FD5D4);
EXTERN_GLOBAL(Ang16, kAng180_6FD3EE);

DEFINE_GLOBAL(T_Particle_4C_Pool*, gParticle_4C_Pool, 0x6FD5E4);
DEFINE_GLOBAL(ParticleManager*, gParticle_8, 0x6FD5E8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint07, Fix16(0x47A, 0), 0x6FD474);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint02, Fix16(0x147, 0), 0x6FD468);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD314, Ang16(360), 0x6FD314);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint0007, Fix16(0xB, 0), 0x6FD4EC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFifty, Fix16(50), 0x6FD558);
DEFINE_GLOBAL_INIT(Ang16, kAng1_6FD5CC, Ang16(4), 0x6FD5CC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpTen, Fix16(10), 0x6FD508);

DEFINE_GLOBAL_INIT(Fix16, kFpSevenThirtySeconds_6FD2D4, Fix16(0xE00, 0), 0x6FD2D4);
DEFINE_GLOBAL_INIT(Fix16, kFpMinusNineThirtySeconds_6FD4CC, Fix16(0xFFFFEE00, 0), 0x6FD4CC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eighth_6FD2D0, Fix16(0x800, 0), 0x6FD2D0);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeThirtySeconds_6FD48C, Fix16(0x600, 0), 0x6FD48C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint03_6FD464, Fix16(0x1EB, 0), 0x6FD464);

// NOTE: Will not match in marked extern !!
DEFINE_GLOBAL(u16, gParticleInstCount, 0x6FD5F4);

MATCH_FUNC(0x53e320)
void ParticleManager::ParticlesService()
{
    gParticle_4C_Pool->UpdatePool();
}

MATCH_FUNC(0x53E3C0)
Particle_4C* ParticleManager::New(Fix16 speed_x, Fix16 speed_y, Fix16 speed_z, Fix16 additional_speed_x, Fix16 additional_speed_y, Fix16 additional_speed_z)
{
    Particle_4C* pNewParticle = NULL;
    if (gParticle_4C_Pool->has_pStart() && gSpritePool->has_free())
    {
        pNewParticle = gParticle_4C_Pool->Allocate();
        pNewParticle->mSpeed8.x = speed_x;
        pNewParticle->mSpeed8.y = speed_y;
        pNewParticle->mSpeed8.z = speed_z;
        pNewParticle->mAdditionalSpeed.x = additional_speed_x;
        pNewParticle->mAdditionalSpeed.y = additional_speed_y;
        pNewParticle->mAdditionalSpeed.z = additional_speed_z;
        pNewParticle->mId = gParticleInstCount;
        pNewParticle->mpOwnSprite = gSpritePool->get_new_sprite();
        ++gParticleInstCount;
    }
    return pNewParticle;
}

// Ang16::operator- (9.6f 0x40E5D0) with the normalizing ctor called out of line (AssignNormalized)
static inline Ang16 SubAng16_ool(const Ang16& a, const Ang16& b)
{
    s16 value = a.rValue - b.rValue;
    return Ang16(&value, 0);
}

// RotateByAngle with the x_old negate as the named out-of-line Negated: the rotations and the
// -(v / 15) New arguments have to reach the same function (the COMDAT copy of operator- and
// Negated are not folded in our build). The other operators stay the plain inlines.
static inline void RotateNegExport_(Fix16_Point& v, const Ang16& angle)
{
    Fix16 sin = Ang16::sine(angle);
    Fix16 cos = Ang16::cosine(angle);
    Fix16 x_old = v.x;
    v.x = (v.x * cos) + (v.y * sin);
    v.y = (x_old.Negated() * sin) + (v.y * cos);
}

// 9.6f 0x48C9C0: plain members and operators. The do-while count-down loop (smaller than the for form) and
// the unary minus on the divides (never inlined: it builds a by-value argument, so it adds two sites) set
// the inline budget: + out of line and the negate inline in the first rotation, the multiplies out of line
// in MultiplyByFix16, the ctor out of line in Ang16::operator-.
MATCH_FUNC(0x53E450)
void ParticleManager::EmitBloodBurst(Fix16 x, Fix16 y, Fix16 z, Ang16 ang)
{
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));

    if (!bSkip_particles)
    {
        vector.x = Fix16(0);
        vector.y = Fix16(gRng.get_int(50) + 25) * kFpOne512th_6FD548;
        vector.RotateByAngle(ang);

        s32 count = 6;
        do
        {
            vector.x = Fix16(0);
            vector.y = (Fix16(gRng.get_int(100)) + kFpFifty) * kFpPoint0007;

            angle = kAng1_6FD5CC.MultiplyByFix16(Fix16(gRng.get_int(16)));
            vector.RotateByAngle((angle + ang) - kAng1_6FD5CC.MultiplyByFix16(Fix16(8)));

            Particle_4C* pBloodParticle =
                gParticle_8->New(vector.x, vector.y, kFpOneThirtySecond_6FD330, -vector.x.DivideInt(15), -vector.y.DivideInt(15), 0);

            if (pBloodParticle)
            {
                pBloodParticle->mUnused34 = 1;
                pBloodParticle->mState = 1;
                pBloodParticle->mCounter = 15;
                pBloodParticle->mInitialCounter = 15;

                pBloodParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pBloodParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 16);
                pBloodParticle->mpOwnSprite->set_xyz_lazy(x, y, z);
                gSpriteGrid_3->AddToSingleBucket(pBloodParticle->mpOwnSprite);
            }
        } while (--count);
    }
}

// 9.6f 0x48CC50
MATCH_FUNC(0x53e880)
void ParticleManager::SpawnBlood(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Particle_4C* pNewParticle = gParticle_8->New(xpos, ypos, 0, 0, 0, 0);
    if (pNewParticle)
    {
        pNewParticle->mUnused34 = 1;
        pNewParticle->mState = 39;
        pNewParticle->mCounter = 800;
        pNewParticle->mSubState = 0;
        pNewParticle->mTimer = 3;
        pNewParticle->mInitialCounter = 800;
        pNewParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
        pNewParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 191);
        pNewParticle->mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos);
        pNewParticle->mpOwnSprite->SetDrawLayer(2);
        gSpriteGrid_3->AddToSingleBucket(pNewParticle->mpOwnSprite);
    }
}

MATCH_FUNC(0x53e970)
void ParticleManager::GunMuzzelFlash(Sprite* pSprite)
{
    // pos, offset, point_velocity and corner_3 are unused but give the original's EH states
    Fix16_Point vel(0, 0);
    Fix16_Point pos;
    Fix16_Point offset;
    Fix16_Point ped_offset;
    Fix16_Point point_velocity;
    Fix16 dx;
    Fix16 dy;
    if (!bSkip_particles)
    {
        if (pSprite->mSpriteTypeEnum == sprite_types_enum::car_2)
        {
            Ang16 angle;
            Fix16_Point corner_1;
            Fix16_Point corner_2;
            Fix16_Point corner_3;
            Car* pCar = pSprite->mCarBcPtr;
            Fix16 half_w;
            Fix16 half_h;
            Particle_4C* pParticle = gParticle_8->New(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
            if (pParticle)
            {
                pParticle->mBf4.bDriverAttacking = true;
                pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 197);
                pParticle->mUnused34 = 0;
                pParticle->mState = 40;
                Ang16::PolarToCartesian_41FC20(pSprite->mRotation, kFpPoint1_6FD2E8, dx, dy);
                pParticle->mSubState = 0;
                pParticle->mTimer = 0;

                half_w = pCar->GetCarWidth() / 2 + kFpMinusOneThirtySecond;
                half_h = pCar->GetCarHeight() / 2 + kFpPoint2PlusOne64th;
                corner_1.x = half_w;
                corner_1.y = half_h;
                angle = pSprite->mRotation;
                corner_1.RotateByAngle(angle);
                corner_1 += pSprite->get_x_y_443580();

                pParticle->mpSprite = pSprite;
                pParticle->mpOwnSprite->set_ang_lazy(pSprite->mRotation);
                pParticle->mpOwnSprite->set_xyz_lazy(corner_1.x, corner_1.y, pSprite->mZpos);
                gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
                pParticle->mpOwnSprite->mBf2C.bNoLighting = true;
            }

            pParticle = gParticle_8->New(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
            if (pParticle)
            {
                pParticle->mBf4.bDriverAttacking = true;
                pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 197);
                pParticle->mUnused34 = 0;
                pParticle->mState = 41;
                Ang16::PolarToCartesian_41FC20(pSprite->mRotation, kFpPoint1_6FD2E8, dx, dy);
                pParticle->mSubState = 0;
                pParticle->mTimer = 0;

                angle = pSprite->mRotation;
                half_w = pCar->GetCarWidth() / 2 + kFpMinusOneThirtySecond;
                half_h = pCar->GetCarHeight() / 2 + kFpPoint2PlusOne64th;
                corner_2.SetXY(-half_w, half_h);
                corner_2.RotateByAngle(angle);
                corner_2 += pSprite->get_x_y_443580();

                pParticle->mpOwnSprite->set_ang_lazy(pSprite->mRotation);
                pParticle->mpOwnSprite->set_xyz_lazy(corner_2.x, corner_2.y, pSprite->mZpos);
                pParticle->mpSprite = pSprite;
                gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
                pParticle->mpOwnSprite->mBf2C.bNoLighting = true;
            }
        }
        else
        {
            Particle_4C* pParticle = gParticle_8->New(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
            if (pParticle)
            {
                pParticle->mBf4.bDriverAttacking = true;
                pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 197);
                pParticle->mUnused34 = 0;
                pParticle->mState = 40;
                Ang16::PolarToCartesian_41FC20(pSprite->mRotation, kFpPoint1_6FD2E8, dx, dy);
                pParticle->mSubState = 0;
                pParticle->mTimer = 0;
                gParticlePosX = pSprite->mXy.x + dx;
                gParticlePosY = pSprite->mXy.y + dy;
                Fix16 zpos = pSprite->mZpos;

                Char* pChar = pSprite->AsCharB4();
                ped_offset.x = -kFpPoint03_6FD464;
                ped_offset.y = kFpPoint02 + kFpPoint1_6FD2E8;
                ped_offset.RotateByAngle(pSprite->mRotation);
                ped_offset = ped_offset + *reinterpret_cast<Fix16_Point*>(&pChar->mVelocityVector);

                pParticle->mpOwnSprite->set_ang_lazy(pSprite->mRotation);
                pParticle->mpOwnSprite->set_xyz_lazy(pSprite->mXy.x + ped_offset.x, pSprite->mXy.y + ped_offset.y, zpos);
                pParticle->mpSprite = pSprite;
                gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
                pParticle->mpOwnSprite->mBf2C.bNoLighting = true;
            }
        }
    }
}

// https://decomp.me/scratch/2Kz9I
// 9.6f 0x48D1F0 (unpaired before): plain members (MultiplyByFix16, +, -, RotateByAngle as
// RotateNegExport_), a count-down loop, -(v / 15) through Negated.
MATCH_FUNC(0x53f060)
void ParticleManager::EmitWaterSplash(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, bool bRandomRot)
{
    Ang16 angle_2;
    Fix16_Point velocity(Fix16(0), Fix16(0));

    if (!bSkip_particles)
    {
        velocity.x = Fix16(0);
        velocity.y = Fix16(gRng.get_int(50) + 25) * kFpOne512th_6FD548;
        RotateNegExport_(velocity, rotation);

        for (s32 count = 6; count != 0; count--)
        {
            if (bRandomRot)
            {
                angle_2 = kAng1_6FD5CC.MultiplyByFix16(Fix16(gRng.get_int(360)));
            }
            else
            {
                angle_2 = rotation;
            }

            velocity.x = Fix16(0);
            velocity.y = (Fix16(gRng.get_int(100)) + kFpFifty) * kFpPoint0007;

            Ang16 angle_1 = kAng1_6FD5CC.MultiplyByFix16(Fix16(gRng.get_int(16)));
            RotateNegExport_(velocity, (angle_1 + angle_2) - kAng1_6FD5CC.MultiplyByFix16(Fix16(8)));

            // last arg: plain 0 (the original builds this Fix16 arg in place; Fix16(0) gives a plain push and costs ebp)
            Particle_4C* pWaterSplashParticle = gParticle_8->New(velocity.x,
                                                                 velocity.y,
                                                                 kFpOneThirtySecond_6FD330,
                                                                 velocity.x.DivideInt(15).Negated(),
                                                                 velocity.y.DivideInt(15).Negated(),
                                                                 0);

            if (pWaterSplashParticle)
            {
                pWaterSplashParticle->mUnused34 = 0;
                pWaterSplashParticle->mState = 35;
                pWaterSplashParticle->mCounter = 15;
                pWaterSplashParticle->mInitialCounter = 15;

                pWaterSplashParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pWaterSplashParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 132);
                pWaterSplashParticle->mSubState = 0;
                pWaterSplashParticle->mTimer = 6;
                pWaterSplashParticle->mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos);
                gSpriteGrid_3->AddToSingleBucket(pWaterSplashParticle->mpOwnSprite);
            }
        }
    }
}

// https://decomp.me/scratch/3Ars7
// 9.6f 0x48D4E0. The original enters with EH state 4 (five points) and sets no state in the car
// branch: the `offset + get_x_y()` sum is the plain inline operator+ (charged, called out of line,
// no EH state for the temporaries) in both branches. `Ang16 angle` is at function scope as in 9.6f.
// The inline budget is tight: the first car rotation must inline exactly one operator* and
// PolarToCartesian_41FC20 one, which needs the front-end size of the function in a window of
// about 230 nodes. The implicit int -> Fix16 conversions in the vel constructions, the declared
// then assigned zpos, the `x = x | n` flag stores and the `!= 0` are code-neutral spellings that
// put it there (Scripts/inline_budget/inlsim.py --scan); explicit `Fix16(0)` / `|=` forms fall short.
MATCH_FUNC(0x53F4C0)
void ParticleManager::EmitFlameStreamSegment(Sprite* pSprt)
{
    {
        Fix16_Point vel(0, 0);
        Fix16_Point pos;
        Fix16_Point offset;
        Fix16_Point ped_offset;
        // point_velocity only receives the unused result of GetPointVelocity (9.6f passed it
        // to set_xyz_lazy instead of pos).
        Fix16_Point point_velocity;
        Ang16 angle;
        if (!bSkip_particles)
        {
            if (!mFireHitObj)
            {
                mFireHitObj = gObject_5C->NewPhysicsObj(objects::fire_hitting_194, 0, 0, 0, kAngZero_6FD5D4);
            }
            vel = Fix16_Point(0, 0);
            Particle_4C* pParticle = gParticle_8->New(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
            if (pParticle)
            {
                pParticle->mFlags = pParticle->mFlags | 1;
                pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 73);
                Particle_4C* tmp = pParticle;
                tmp->mpOwnSprite->AllocCollisionShape(kFpOne64th_6FD554 * kFpTen, kFpOne64th_6FD554 * kFpTen, kFP16Quarter_6FD2EC);
                pParticle->mState = 31;
                pParticle->mUnused34 = 0;
                Fix16 vec_x;
                Fix16 vec_y;
                Ang16::PolarToCartesian_41FC20(pSprt->mRotation, kFpPoint1_6FD2E8, vec_x, vec_y);
                pParticle->mCounter = 100;
                pParticle->mSubState = 0;
                pParticle->mTimer = 0;
                gParticlePosX = pSprt->mXy.x + vec_x;
                gParticlePosY = pSprt->mXy.y + vec_y;
                Fix16 zpos;
                zpos = pSprt->mZpos;
                // Both loaded before the type check (one load of field_8, kept for both branches)
                Car* pCar = pSprt->mCarBcPtr;
                Char* pPed = pSprt->mCharB4Ptr;
                if (pSprt->mSpriteTypeEnum == sprite_types_enum::car_2)
                {
                    AttachedSprite* pAttached = pCar->mAttachments.GetSpriteForModel(objects::water_cannon_114);
                    if (pAttached)
                    {
                        angle = pAttached->mpSprite->mRotation + kAng180_6FD3EE;
                        pos.SetXY(Fix16(0), kFpSevenThirtySeconds_6FD2D4);
                        pos.RotateByAngle(angle);
                        offset.SetXY(Fix16(0), kFP16Eighth_6FD2D0);
                    }
                    else
                    {
                        AttachedSprite* pGun = pCar->mAttachments.GetSpriteForModel(248);
                        angle = pGun->mpSprite->mRotation;
                        pos.SetXY(Fix16(0), kFpThreeThirtySeconds_6FD48C);
                        pos.RotateByAngle(angle);
                        offset.SetXY(Fix16(0), kFpMinusNineThirtySeconds_6FD4CC);
                    }
                    pParticle->mpOwnSprite->set_ang_lazy(angle);
                    offset.RotateByAngle(pSprt->mRotation);
                    pos += offset + pSprt->get_x_y_443580();
                    point_velocity = pSprt->mCarBcPtr->mPhysics->GetPointVelocity(&pos); // not used
                    pParticle->mpOwnSprite->set_xyz_lazy(pos.x, pos.y, zpos);
                }
                else
                {
                    ped_offset.x = -kFpPoint03_6FD464;
                    ped_offset.y = kFpPoint02 + kFpPoint1_6FD2E8;
                    ped_offset.RotateByAngle(pSprt->mRotation);
                    ped_offset = ped_offset + *reinterpret_cast<Fix16_Point*>(&pPed->mVelocityVector);
                    pParticle->mpOwnSprite->set_ang_lazy(pSprt->mRotation);
                    pParticle->mpOwnSprite->set_xyz_lazy(pSprt->mXy.x + ped_offset.x, pSprt->mXy.y + ped_offset.y, zpos);
                }
                pParticle->mpSprite = pSprt;
                if (pParticle->mpOwnSprite->CheckSpriteMovementRegion() != 0)
                {
                    pParticle->mCounter = 0;
                }
                gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
                pParticle->mpOwnSprite->mFlags = pParticle->mpOwnSprite->mFlags | 4;
            }
        }
    }
}

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpEpsilon, Fix16(0x1, 0), 0x6FD500);

MATCH_FUNC(0x53FAE0)
void ParticleManager::EmitFireTruckSprayParticle(Sprite* pSprite)
{
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));
    if (!bSkip_particles)
    {
        if (!mWaterHitObj)
        {
            mWaterHitObj = gObject_5C->NewPhysicsObj(objects::water_hitting_198, 0, 0, 0, kAngZero_6FD5D4);
        }
        vector.x = Fix16(0);
        vector.y = Fix16(0);
        Particle_4C* pParticle = gParticle_8->New(vector.x, vector.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
        if (pParticle)
        {
            pParticle->mBf4.bDriverAttacking = true;
            pParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 111);
            pParticle->mpOwnSprite->AllocCollisionShape(kFpOne64th_6FD554 * kFpTen, kFpOne64th_6FD554 * kFpTen, kFP16Quarter_6FD2EC);
            pParticle->mUnused34 = 0;
            pParticle->mState = 34;
            Fix16 vec_x;
            Fix16 vec_y;
            Ang16::PolarToCartesian_41FC20(pSprite->mRotation, kFpPoint1_6FD2E8, vec_x, vec_y);
            pParticle->mCounter = 100;
            pParticle->mSubState = 0;
            pParticle->mTimer = 0;
            gParticlePosX = pSprite->mXy.x + vec_x;
            gParticlePosY = pSprite->mXy.y + vec_y;
            Fix16 zpos = pSprite->mZpos;
            if (pSprite->mSpriteTypeEnum == sprite_types_enum::car_2)
            {
                Car* pCar = pSprite->mCarBcPtr;
                AttachedSprite* pGun = pCar->mAttachments.GetSpriteForModel(objects::water_cannon_114);
                if (pGun)
                {
                    angle = pGun->mpSprite->mRotation + kAng180_6FD3EE;
                }
                else
                {
                    angle = pCar->mAttachments.GetSpriteForModel(248)->mpSprite->mRotation;
                }
                pParticle->mpOwnSprite->set_ang_lazy(angle);
            }
            else
            {
                pParticle->mpOwnSprite->set_ang_lazy(pSprite->mRotation);
            }
            pParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, zpos);
            pParticle->mpSprite = pSprite;
            if (pParticle->mpOwnSprite->CheckSpriteMovementRegion())
            {
                pParticle->mCounter = 0;
            }
            gSpriteGrid_3->AddToSingleBucket(pParticle->mpOwnSprite);
            pParticle->mpOwnSprite->mBf2C.bNoLighting = true;
        }
    }
}

// Ang16::PolarToCartesian_41FC20 with the cosine multiply written as the out-of-line Multiply
// the original calls (past the inline budget VC6 would call a COMDAT copy of operator* instead).
static inline void PolarToCartesian_OutOfLineCos(Ang16& angle, Fix16& radius, Fix16& ret1, Fix16& ret2)
{
    ret1 = Ang16::sine(angle) * radius;
    ret2 = Ang16::cosine(angle).Multiply(radius);
}

// https://decomp.me/scratch/wfzEd
MATCH_FUNC(0x53FE40)
void ParticleManager::EmitImpactParticles(Fix16 x, Fix16 y, Fix16 z, Fix16_Point dir)
{
    // 9.6f declares both angles up front. dir is destroyed by the callee; the unused point gives
    // the EH state 2 that the original sets once t is built.
    Ang16 ang1;
    Ang16 tanAng;
    Fix16_Point unused;
    Fix16_Point t(Fix16(0), Fix16(0));
    tanAng = dir.GetAngle();

    for (u8 i = 0; i < 6; ++i)
    {
        t.x = Fix16(0);
        t.y = (Fix16(gRng.get_int(100)) + kFpFifty) * kFpPoint0007;
        if (i < 4)
        {
            ang1 = kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(gRng.get_int(32)));
            // 9.6f: t.RotateByAngle((ang1 + tanAng) - kAng1_6FD5CC.MultiplyByFix16(..)) in both branches
            t.RotateByAngle_40F6B0_all_out_of_line(SubAng16_ool(ang1 + tanAng, kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(16))));
        }
        else
        {
            ang1 = kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(gRng.get_int(360)));
            t.RotateByAngle_40F6B0_all_out_of_line(SubAng16_ool(AddAng16_ool(ang1, tanAng), kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(180))));
        }

        Particle_4C* pNewParticle = gParticle_8->New(t.x, t.y, kFpOneThirtySecond_6FD330, t.x.DivideInt(15).Negated(), t.y.DivideInt(15).Negated(), 0);
        if (pNewParticle)
        {
            pNewParticle->mUnused34 = 1;
            pNewParticle->mState = 7;
            pNewParticle->mCounter = 7;
            pNewParticle->mInitialCounter = 7;
            pNewParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pNewParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 127);
            pNewParticle->mpOwnSprite->set_xyz_lazy(x, y, z);
            pNewParticle->mpOwnSprite->SetNoLighting();

            gSpriteGrid_3->AddToSingleBucket(pNewParticle->mpOwnSprite);
        }
    }
}

MATCH_FUNC(0x540320)
void ParticleManager::EmitElectricArcParticle(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 ang)
{
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));

    if (!bSkip_particles)
    {
        gRng.get_int(3);
        vector.x = Fix16(0);
        vector.y = (Fix16(gRng.get_int(100)) + kFpFifty) * kFpEpsilon;

        // The rng result is bound to a const s16& temporary: stored as a short and sign extended only after the
        // word is loaded (the word ends up as the imul's eax operand)
        angle = kAng1_6FD5CC.MultiplyByFix16(Fix16(static_cast<const s16&>(gRng.get_int(360))));
        // RotateByAngle, but x * cos, the y line and both sums use the out-of-line Fix16 operators
        {
            Fix16 sin = Ang16::sine(angle);
            Fix16 cos = Ang16::cosine(angle);
            Fix16 x_old = vector.x;
            vector.x = vector.x.Multiply(cos).Add(vector.y * sin);
            vector.y = x_old.Negated().Multiply(sin).Add(vector.y.Multiply(cos));
        }

        Particle_4C* pNewParticle = gParticle_8->New(vector.x, vector.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
        if (pNewParticle)
        {
            pNewParticle->mUnused34 = 0;
            pNewParticle->mState = 37;
            pNewParticle->mSubState = 0;
            pNewParticle->mInitialCounter = pNewParticle->mCounter;
            pNewParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pNewParticle->mpOwnSprite->SetNoLighting();
            pNewParticle->mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos);
            pNewParticle->mpOwnSprite->set_ang_lazy(ang);
            pNewParticle->mpOwnSprite->set_id_lazy(gRng.get_int(4) + gObjectDefinitions->mDef112BaseSpriteIdx + 175);
            pNewParticle->mpOwnSprite->SetBlendAndAlpha(2, 20);
            pNewParticle->mpOwnSprite->SetNoLighting();
            gSpriteGrid_3->AddToSingleBucket(pNewParticle->mpOwnSprite);
        }
    }
}

MATCH_FUNC(0x5405D0)
void ParticleManager::SpawnParticleSprite(Sprite* pSprite)
{
    if (gParticle_4C_Pool->has_pStart())
    {
        Particle_4C* pNewParticle = gParticle_4C_Pool->Allocate();
        if (pNewParticle)
        {
            pNewParticle->mpSprite = pSprite;
            pNewParticle->mUnused34 = 0;
            pNewParticle->mSubState = 0;

            pNewParticle->mpOwnSprite = gSpritePool->get_new_sprite();
            pNewParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pNewParticle->mState = 38;
            pNewParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 164);
            pNewParticle->mpOwnSprite->SetNoLighting();
            pNewParticle->mpOwnSprite->set_xyz_lazy(pSprite->mXy.x, pSprite->mXy.y, pSprite->mZpos);

            gSpriteGrid_3->AddToSingleBucket(pNewParticle->mpOwnSprite);
        }
    }
}

// 9.6f 0x48E060. Fix16_Point vel has a dtor, which gives the EH frame; its fields are zeroed again
// before the call, and the two other zero args go through the out-of-line Fix16(s32) ctor.
MATCH_FUNC(0x5406b0)
void ParticleManager::SpawnCigaretteSmokePuff(Sprite* pSprite, char_type bSmokeSource)
{
    Fix16_Point vel(Fix16(0), Fix16(0));
    if (!bSkip_particles)
    {
        vel.x = Fix16(0);
        vel.y = Fix16(0);
        Particle_4C* pNewParticle = gParticle_8->New(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
        if (pNewParticle)
        {
            pNewParticle->mUnused34 = 1;

            if (bSmokeSource)
            {
                pNewParticle->mState = 9;
                pNewParticle->mCounter = 80;
            }
            else
            {
                pNewParticle->mState = 10;
                pNewParticle->mCounter = 70;
            }

            pNewParticle->mInitialCounter = pNewParticle->mCounter;
            pNewParticle->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
            pNewParticle->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 3);

            Fix16 x;
            Fix16 y;
            if (bSmokeSource)
            {
                PolarToCartesian_OutOfLineCos(pSprite->mRotation, kFpPoint07, x, y);
            }
            else
            {
                PolarToCartesian_OutOfLineCos(pSprite->mRotation, kFpPoint05_6FD46C, x, y);
            }
            gParticlePosX = x;
            gParticlePosY = y;

            Fix16 offset_x;
            Fix16 offset_y;
            PolarToCartesian_OutOfLineCos(pSprite->mRotation - kAng90_6FD314, kFpPoint02, offset_x, offset_y);

            gParticlePosX += offset_x + pSprite->mXy.x;
            gParticlePosY += offset_y + pSprite->mXy.y;

            pNewParticle->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, pSprite->mZpos);
            pNewParticle->mpSprite = pSprite;

            gSpriteGrid_3->AddToSingleBucket(pNewParticle->mpOwnSprite);

            if (bSmokeSource)
            {
                pNewParticle->mpOwnSprite->SetNoLighting();
            }
        }
    }
}

// Ang16::operator+ (9.6f 0x40E5A0) with the normalizing ctor called out of line (AssignNormalized)
static inline Ang16 AddAng16_ool(const Ang16& a, const Ang16& b)
{
    s16 value = a.rValue + b.rValue;
    return Ang16(&value, 0);
}

MATCH_FUNC(0x5439d0)
ParticleManager::ParticleManager()
{
    if (!gParticle_4C_Pool)
    {
        gParticle_4C_Pool = new T_Particle_4C_Pool();
        if (!gParticle_4C_Pool)
        {
            FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\particle.cpp", 4167);
        }
    }
    mFireHitObj = NULL;
    mWaterHitObj = NULL;
}

MATCH_FUNC(0x543a60)
ParticleManager::~ParticleManager()
{
    if (gParticle_4C_Pool)
    {
        GTA2_DELETE_AND_NULL(gParticle_4C_Pool);
    }
    mFireHitObj = NULL;
    mWaterHitObj = NULL;
}