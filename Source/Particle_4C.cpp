// GetLength_41E260 compares against this TU's zero (see Fix16_Point.hpp)
#include "Function.hpp"
#include "fix16.hpp"
EXTERN_GLOBAL(Fix16, kFP16Zero_6FD49C);
#define FIX16_POINT_ZERO kFP16Zero_6FD49C
#include "Particle_4C.hpp"
#include "map_0x370.hpp"
#include "Object_5C.hpp"
#include "ObjectDefinitions.hpp"
#include "SpriteGrid.hpp"
#include "rng.hpp"
#include "sprite.hpp"
#include "Explosion.hpp"
#include "char.hpp"
#include "Ped.hpp"
#include "ParticleManager.hpp"
#include "Car.hpp"
#include "ScriptManager.hpp"
#include "Object_3C.hpp"
#include "CarPhysics.hpp"

EXTERN_GLOBAL(Fix16, kFpPoint2_6FD2F0);
EXTERN_GLOBAL(Fix16, kFpOne64th_6FD448);
EXTERN_GLOBAL(Fix16, kFP16Eight_6FD4C0);
EXTERN_GLOBAL(Fix16, kFpOne1024th);
EXTERN_GLOBAL(Fix16, kFpPoint03_6FD464);
EXTERN_GLOBAL(Fix16, kFpPoint3_6FD2F4);
EXTERN_GLOBAL(Ang16, kAng1_6FD5CC);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD3EE, Ang16(720), 0x6FD3EE);
EXTERN_GLOBAL(Fix16, kFpSevenThirtySeconds_6FD2D4);
EXTERN_GLOBAL(Fix16, kFP16Eighth_6FD2D0);
EXTERN_GLOBAL(Fix16, kFpThreeThirtySeconds_6FD48C);
EXTERN_GLOBAL(Fix16, kFpMinusNineThirtySeconds_6FD4CC);
EXTERN_GLOBAL(Fix16, kFpOne64th_6FD328);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_6FD46C, Fix16(0x333, 0), 0x6FD46C);
// Literal values: the globals they come from are in Explosion.cpp, whose initializers run after this TU's
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD554, Fix16(0x100, 0), 0x6FD554);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kMaxZ, Fix16(0x1FF00, 0), 0x6FD28C); // 8 - 1/64

DEFINE_GLOBAL(Fix16, gParticlePosX, 0x6FD388);
DEFINE_GLOBAL(Fix16, gParticlePosY, 0x6FD38C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6FD2E8, Fix16(0x666, 0), 0x6FD2E8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FD4A0, Fix16(1), 0x6FD4A0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FD39C, Fix16(0.5f), 0x6FD39C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_6FD4A4, Fix16(2), 0x6FD4A4);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_6FD4A8, Fix16(3), 0x6FD4A8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint04, Fix16(0x28F, 0), 0x6FD470);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_6FD45C, Fix16(0xA3, 0), 0x6FD45C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint005, Fix16(0x51, 0), 0x6FD564);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint003, Fix16(0x31, 0), 0x6FD538);

DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_6FD55C, Fix16(0x3000, 0), 0x6FD55C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint9, Fix16(0x3999, 0), 0x6FD30C);
DEFINE_GLOBAL_INIT(Fix16, kFp255_6FD280, Fix16(255), 0x6FD280);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint6, Fix16(0x2666, 0), 0x6FD300);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint7, Fix16(0x2CCC, 0), 0x6FD304);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint8, Fix16(0x3333, 0), 0x6FD308);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneSixth, Fix16(0xAAA, 0), 0x6FD364);
DEFINE_GLOBAL_INIT(Fix16, kFpMinusOneThirtySecond, Fix16(0xFFFFFE00, 0), 0x6FD3C0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2PlusOne64th, Fix16(0xDCC, 0), 0x6FD5A8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint4, Fix16(0x1999, 0), 0x6FD2F8);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_6FD4AC, Fix16(4), 0x6FD4AC);

// https://decomp.me/scratch/nKSYL
MATCH_FUNC(0x538060)
bool Particle_4C::UpdateFloatingParticle_state_6_15_16_17()
{
    Fix16 rng_1;
    Fix16 rng_2;
    Fix16 off_y;
    Fix16_Point vector(Fix16(0), Fix16(0));
    Fix16 new_z = kFpPoint01_6FD45C + mpOwnSprite->mZpos;
    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mCounter == 0)
    {
        return true;
    }
    if (new_z > kMaxZ)
    {
        return true;
    }

    if (new_z.GetFracValue() > kFpThreeQuarters_6FD55C &&
        gMap_0x370->IsBlockNonAirType(mpOwnSprite->mXy.x.ToInt(),
                                      mpOwnSprite->mXy.y.ToInt(),
                                      new_z.ToInt()))
    {
        new_z = mpOwnSprite->mZpos; // blocked above: stay at the current height
        Fix16 r1;
        r1 = Fix16(gRng.get_int(61) - 30);
        rng_1 = r1 / 100;
        Fix16 r2;
        r2 = Fix16(gRng.get_int(10) - 5);
        rng_2 = r2 / 100;
        ++mCounter;
    }
    else
    {
        Fix16 r1;
        r1 = Fix16(gRng.get_int(3) - 1);
        rng_1 = r1 / 100;
        Fix16 r2;
        r2 = Fix16(gRng.get_int(3) - 1);
        rng_2 = r2 / 100;
    }

    if (mpExplosion)
    {
        if (mCounter > 60)
        {
            if (mpExplosion->mpObj2C->mSprite)
            {
                mSpeed20 = mpExplosion->mpObj2C->mSprite->mObject2CPtr->GetEffectiveSpeed();
                mAngle = mpExplosion->mpObj2C->mSprite->mObject2CPtr->mObj3c->mAngle;
            }
            if (mpExplosion->mTimer == 1)
            {
                mpExplosion = NULL;
            }
        }
    }
    else
    {
        mSpeed20 = kFP16Zero_6FD49C;
    }

    if (mSpeed20 == kFP16Zero_6FD49C)
    {
        // Assigned (not initialised) here so VC6 keeps the neg; add instead of folding it into a sub
        off_y = -kFpPoint01_6FD45C;
        gParticlePosX = mpOwnSprite->mXy.x + rng_1;
        gParticlePosY = mpOwnSprite->mXy.y + off_y + rng_2;
        if (gParticlePosX > kFP16One_6FD4A0 && gParticlePosX < kFp255_6FD280 - kFP16One_6FD4A0 && gParticlePosY > kFP16One_6FD4A0 &&
            gParticlePosY < kFp255_6FD280 - kFP16One_6FD4A0)
        {
            mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, new_z);
        }
        else
        {
            mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x, mpOwnSprite->mXy.y, new_z);
        }
    }
    else
    {
        mSpeed20 = mSpeed20 * kFpPoint9;
        if (mSpeed20 < kFP16Zero_6FD49C)
        {
            mSpeed20 = kFP16Zero_6FD49C;
        }
        // A named copy of the zero constant: gives it ebx and pos_y ebp, as in the original
        Fix16 zero = kFP16Zero_6FD49C;
        Fix16 pos_x = mpOwnSprite->mXy.x;
        Fix16 pos_y = mpOwnSprite->mXy.y;
        vector.y = mSpeed20;
        vector.x = zero;
        {
            Fix16 sin = Ang16::sine(mAngle);
            Fix16 cos = Ang16::cosine(mAngle);
            Fix16 old_x = vector.x;
            vector.x = (vector.x * cos).Add(vector.y * sin);
            vector.y = old_x.Negated().Multiply(sin).Add(vector.y.Multiply(cos));
        }

        mAdditionalSpeed.x = vector.x;
        mAdditionalSpeed.y = vector.y;

        mSpeed8.x = mAdditionalSpeed.x + rng_1;
        mSpeed8.y = mAdditionalSpeed.y + rng_2;

        pos_x = pos_x + mSpeed8.x;
        pos_y = pos_y + mSpeed8.y;
        gParticlePosX = pos_x;
        gParticlePosY = pos_y;
        if (gParticlePosX > kFP16One_6FD4A0 && gParticlePosX < kFp255_6FD280 - kFP16One_6FD4A0 && gParticlePosY > kFP16One_6FD4A0 &&
            gParticlePosY < kFp255_6FD280 - kFP16One_6FD4A0)
        {
            mpOwnSprite->set_xyz_lazy(pos_x, pos_y, new_z);
        }
        else
        {
            mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x, mpOwnSprite->mXy.y, new_z);
        }
    }

    gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
    return false;
}

MATCH_FUNC(0x5384c0)
bool Particle_4C::UpdateDirectedProjectile_state_3_12()
{
    Fix16 jitter_x;
    Fix16 off_x = kFP16Zero_6FD49C;
    Fix16 jitter_y;
    Fix16 off_y = kFP16Zero_6FD49C;
    bool bJitter = true;
    Fix16_Point dir(Fix16(0), Fix16(0));

    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mCounter == 0)
    {
        return true;
    }

    if (mpExplosion)
    {
        if (mpExplosion->mId != mExplosionId)
        {
            mpExplosion = NULL;
            mSpeed20 = kFP16Zero_6FD49C;
        }
        else if (mpExplosion->mpObj2C)
        {
            if (mpExplosion->mpObj2C->mSprite)
            {
                mSpeed20 = mpExplosion->mpObj2C->mSprite->mObject2CPtr->GetEffectiveSpeed();
                mAngle = mpExplosion->mpObj2C->mObj3c->mAngle;
            }
            if (mpExplosion->mTimer == 1)
            {
                mpExplosion = NULL;
            }
        }
        else
        {
            mpExplosion = NULL;
            mSpeed20 = kFP16Zero_6FD49C;
        }
    }
    else
    {
        mSpeed20 = kFP16Zero_6FD49C;
    }

    Fix16 zpos = mpOwnSprite->mZpos + kFpOne1024th;
    if (zpos > kMaxZ)
    {
        return true;
    }

    Fix16 xpos = mpOwnSprite->mXy.x;
    Fix16 ypos = mpOwnSprite->mXy.y;

    if (mSpeed20 == kFP16Zero_6FD49C)
    {
        off_x = 0;
        off_y = -kFpPoint01_6FD45C;
    }

    switch (static_cast<u8>(mCounter >> 2))
    {
        case 2:
            if (gRng.get_int(2))
            {
                return true;
            }
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 102);
            dir.x = 0;
            dir.y = mSpeed20 * kFpPoint6;
            bJitter = false;
            break;
        case 3:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 101);
            dir.x = 0;
            dir.y = mSpeed20 * kFpPoint7;
            break;
        case 4:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 100);
            dir.x = 0;
            dir.y = mSpeed20 * kFpPoint8;
            break;
        case 5:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 99);
            dir.x = 0;
            dir.y = mSpeed20 * kFpPoint8;
            break;
        case 6:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 98);
            dir.x = 0;
            dir.y = mSpeed20 * kFpPoint8;
            break;
        case 7:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 97);
            dir.x = 0;
            dir.y = mSpeed20 * kFpPoint8;
            break;
        default:
            if (gRng.get_int(2))
            {
                return true;
            }
            break;
    }

    dir.RotateByAngle(mAngle);
    mAdditionalSpeed.x = dir.x;
    mAdditionalSpeed.y = dir.y;

    jitter_x = 0;
    jitter_y = 0;
    if (bJitter)
    {
        Fix16 rx;
        rx = Fix16(gRng.get_int(3) - 1);
        jitter_x = rx / 100;
        Fix16 ry;
        ry = Fix16(gRng.get_int(3) - 1);
        jitter_y = ry / 100;
    }

    mSpeed8.x = mAdditionalSpeed.x + off_x + jitter_x;
    mSpeed8.y = mAdditionalSpeed.y + off_y + jitter_y;
    xpos += mSpeed8.x;
    ypos += mSpeed8.y;
    gParticlePosX = xpos;
    gParticlePosY = ypos;

    if (xpos > kFP16One_6FD4A0 && xpos < kFp255_6FD280 - kFP16One_6FD4A0 && ypos > kFP16One_6FD4A0 &&
        ypos < kFp255_6FD280 - kFP16One_6FD4A0)
    {
        mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos);
    }
    else
    {
        mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x, mpOwnSprite->mXy.y, zpos);
    }

    switch (mSubState)
    {
        case 0:
        case 1:
            mpOwnSprite->SetBlendAndAlpha(1, 10);
            break;
        case 2:
            mpOwnSprite->SetBlendAndAlpha(1, 10);
            break;
        case 3:
            mpOwnSprite->SetBlendAndAlpha(2, 10);
            break;
        case 4:
            mpOwnSprite->SetBlendAndAlpha(2, 15);
            break;
        case 5:
            mpOwnSprite->SetBlendAndAlpha(2, 20);
            break;
        case 6:
            mpOwnSprite->SetBlendAndAlpha(2, 25);
            break;
        case 7:
            mpOwnSprite->SetBlendAndAlpha(2, 30);
            break;
    }

    mpOwnSprite->SetNoLighting();
    if (mpExplosion)
    {
        mpOwnSprite->ResolveZOrder(mpExplosion->mpObj2C->mSprite);
    }
    gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);

    if (++mSubState > 8)
    {
        mSubState = 0;
    }
    return false;
}

MATCH_FUNC(0x538a40)
bool Particle_4C::UpdateBeamSegment_state_43()
{
    gSpriteGrid_3->Remove(this->mpOwnSprite);

    this->mSubState++;
    if (mSubState > 7u)
    {
        return true;
    }

    mpOwnSprite->set_id_lazy(mSubState + gObjectDefinitions->mDef112BaseSpriteIdx + 103);

    this->mpOwnSprite->SetBlendAndAlpha(2, 20);
    this->mpOwnSprite->SetNoLighting();
    gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);
    return false;
}

// The normalizing Ang16 ctor (out of line, AssignNormalized) on a by-value parameter: the
// original stores the 16-bit sum as a dword before the call (UpdateObjectBeamLink_state_38)
static inline Ang16 NormalizedAng(s16 value)
{
    return Ang16(&value, 0);
}

// 9.6f 0x41E210 (Fix16_Point::SetFromPolar(Fix16& radius, Ang16& angle)) with the multiplies out of line.
// The angle is a temporary passed by reference, so the table index is reloaded after each multiply.
static inline void SetFromPolar_OOL_41E210(Fix16_Point& p, const Fix16& radius, const Ang16& angle)
{
    p.x = radius.Multiply(Ang16::sine(angle));
    p.y = radius.Multiply(Ang16::cosine(angle));
}

// Inlined Fix16::Max_44E540: compares and returns the values, not references
static inline Fix16 MaxValue_44E540(const Fix16& a, const Fix16& b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

// 9.6f 0x48F230: SetXY for src/dst, delta = dst - src, delta.GetAngle() (result discarded), segments
// from delta.MaxAbs() / kFpOneSixth, delta.DivideAssign(segments), cur += delta, mid = cur - prev,
// mid.DivideAssign(2), mid += prev, SetBlendAndAlpha(2, 20); the function-scope Ang16 is the jitter;
// each case is one SetFromPolar(radius, angle sum) call with both arguments as temporaries;
// dst.SetXY(target) then dst += src (target declared ahead of src, else the x add's operands swap).
MATCH_FUNC(0x538ac0)
bool Particle_4C::UpdateObjectBeamLink_state_38()
{
    ++mSubState;
    Ang16 jitter;
    // Declared before src: the symbol order decides the operand order of `dst += src` (x add)
    Fix16_Point_POD target;
    Fix16_Point src;
    Fix16_Point dst;
    Fix16_Point delta;
    Fix16_Point cur;
    Fix16_Point prev;
    Fix16_Point mid;
    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mSubState != 6 && mpSprite->GetType() == sprite_types_enum::code_obj1_4)
    {
        src.SetXY(mpOwnSprite->mXy.x, mpOwnSprite->mXy.y);
        dst.SetXY(mpSprite->mXy.x, mpSprite->mXy.y);
        delta = dst - src;
        delta.GetAngle();
        Fix16 segments = delta.MaxAbs() / kFpOneSixth;

        if (segments != kFP16Zero_6FD49C)
        {
            delta.DivideAssign(segments);
            prev = src;
            cur = src;
            for (s32 i = 1; i <= segments.ToInt(); i++)
            {
                cur += delta;
                mid = cur - prev;
                mid.DivideAssign(kFP16Two_6FD4A4);
                mid += prev;

                Particle_4C* pSegment = gParticle_8->New(0, 0, 0, 0, 0, 0);
                if (pSegment)
                {
                    pSegment->mUnused34 = 0;
                    pSegment->mState = 43;
                    pSegment->mCounter = 50;
                    pSegment->mSubState = 0;
                    pSegment->mInitialCounter = 50;
                    pSegment->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                    pSegment->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 104);
                    pSegment->mpOwnSprite->set_xyz_lazy(mid.x, mid.y, mpOwnSprite->mZpos);
                    pSegment->mpOwnSprite->SetBlendAndAlpha(2, 20);
                    pSegment->mpOwnSprite->SetNoLighting();
                    gSpriteGrid_3->AddToSingleBucket(pSegment->mpOwnSprite);
                }
                prev = cur;
            }
        }

        target.SetXY(mpSprite->mXy.x, mpSprite->mXy.y);
        jitter = kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(gRng.get_int(16) - 8));

        switch (mSubState)
        {
            case 1:
            case 2:
            case 3:
            {
                SetFromPolar_OOL_41E210(src,
                                        Fix16(mSubState) * kFpPoint05_6FD46C,
                                        NormalizedAng(mpSprite->mRotation.rValue + kAng180_6FD3EE.rValue));
                break;
            }
            case 4:
            case 5:
            {
                SetFromPolar_OOL_41E210(src,
                                        Fix16(mSubState) * kFpPoint05_6FD46C + kFpPoint01_6FD45C,
                                        NormalizedAng(NormalizedAng(mpSprite->mRotation.rValue + kAng180_6FD3EE.rValue).rValue + jitter.rValue));
                break;
            }
        }

        dst.SetXY(target.x, target.y);
        dst += src;

        if (mpSprite->GetType() == sprite_types_enum::code_obj1_4)
        {
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 163);
            mpOwnSprite->SetBlendAndAlpha(2, 20);
            mpOwnSprite->SetNoLighting();
            mpOwnSprite->set_xyz_lazy(dst.x, dst.y, mpOwnSprite->mZpos);
            gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x539040)
bool Particle_4C::UpdateDirectedBurstSweep_state_4()
{
    Fix16 jitter_x;
    Fix16 jitter_y;
    Fix16 off_x = kFP16Zero_6FD49C;
    Fix16 off_y = kFP16Zero_6FD49C;
    Fix16_Point dir(Fix16(0), Fix16(0));

    ++mSubState;
    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mSubState != 16 && mCounter != 0)
    {
        if (mpExplosion)
        {
            if (mpExplosion->mId != mExplosionId)
            {
                mpExplosion = NULL;
                mSpeed20 = kFP16Zero_6FD49C;
            }
            else if (mpExplosion->mpObj2C)
            {
                if (mpExplosion->mpObj2C->mSprite)
                {
                    mSpeed20 = mpExplosion->mpObj2C->mSprite->mObject2CPtr->GetEffectiveSpeed();
                    mAngle = mpExplosion->mpObj2C->mObj3c->mAngle;
                }
                if (mpExplosion->mTimer == 1)
                {
                    mpExplosion = NULL;
                }
            }
            else
            {
                mpExplosion = NULL;
                mSpeed20 = kFP16Zero_6FD49C;
            }
        }
        else
        {
            mSpeed20 = kFP16Zero_6FD49C;
        }

        Fix16 zpos = mpOwnSprite->mZpos + kFpPoint01_6FD45C;
        if (zpos < kMaxZ)
        {

            Fix16 xpos = mpOwnSprite->mXy.x;
            Fix16 ypos = mpOwnSprite->mXy.y;

            if (mSpeed20 < kFpPoint03_6FD464)
            {
                off_x = 0;
                off_y = -kFpPoint05_6FD46C;
            }

            if (mSubState > 0 && mSubState <= 4)
            {
                mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 147);
                dir.x = 0;
                dir.y = (kFP16One_6FD4A0 - Fix16(mSubState) * kFpPoint1_6FD2E8) * mSpeed20;
            }
            else
            {
                mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 147);
                dir.x = 0;
                dir.y = (mSpeed20 * kFpPoint3_6FD2F4) * Fix16(mSubState / 10);
            }

            dir.RotateByAngle(mAngle);
            mAdditionalSpeed.x = dir.x;
            mAdditionalSpeed.y = dir.y;

            jitter_x = Fix16(gRng.get_int(3) - 1);
            jitter_x = jitter_x / 30;
            jitter_y = Fix16(gRng.get_int(3) - 1);
            jitter_y = jitter_y / 30;

            mSpeed8.x = mAdditionalSpeed.x + off_x + jitter_x;
            mSpeed8.y = mAdditionalSpeed.y + off_y + jitter_y;
            xpos = xpos + mSpeed8.x;
            ypos = ypos + mSpeed8.y;
            gParticlePosX = xpos;
            gParticlePosY = ypos;

            if (gParticlePosX > kFP16One_6FD4A0 && gParticlePosX < kFp255_6FD280 - kFP16One_6FD4A0 && gParticlePosY > kFP16One_6FD4A0 &&
                gParticlePosY < kFp255_6FD280 - kFP16One_6FD4A0)
            {
                mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos);
            }
            else
            {
                mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x, mpOwnSprite->mXy.y, zpos);
            }

            mpOwnSprite->SetBlendAndAlpha(2, 15);
            mpOwnSprite->SetNoLighting();
            mpOwnSprite->ApplyScaleToDimensions(kFP16One_6FD4A0 + Fix16(mSubState) * kFpPoint1_6FD2E8, 0);
            if (mpExplosion)
            {
                mpOwnSprite->ResolveZOrder(mpExplosion->mpObj2C->mSprite);
            }
            gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x539480)
bool Particle_4C::UpdateDirectedBurst_state_13_14_36()
{
    Fix16 jitter_x;
    Fix16 jitter_y;
    Fix16 off_x = kFP16Zero_6FD49C;
    Fix16 off_y = kFP16Zero_6FD49C;
    Fix16_Point dir(Fix16(0), Fix16(0));

    ++mSubState;
    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mSubState != 16 && mCounter != 0)
    {
        if (mpExplosion)
        {
            if (mpExplosion->mId != mExplosionId)
            {
                mpExplosion = NULL;
                mSpeed20 = kFP16Zero_6FD49C;
            }
            else if (mpExplosion->mpObj2C)
            {
                if (mpExplosion->mpObj2C->mSprite)
                {
                    mSpeed20 = mpExplosion->mpObj2C->mSprite->mObject2CPtr->GetEffectiveSpeed();
                    mAngle = mpExplosion->mpObj2C->mObj3c->mAngle;
                }
                if (mpExplosion->mTimer == 1)
                {
                    mpExplosion = NULL;
                }
            }
            else
            {
                mpExplosion = NULL;
                mSpeed20 = kFP16Zero_6FD49C;
            }
        }
        else
        {
            mSpeed20 = kFP16Zero_6FD49C;
        }

        Fix16 zpos = mpOwnSprite->mZpos + kFpOne1024th;
        if (zpos < kMaxZ)
        {

            Fix16 xpos = mpOwnSprite->mXy.x;
            Fix16 ypos = mpOwnSprite->mXy.y;

            if (mSpeed20 < kFpPoint03_6FD464)
            {
                off_x = 0;
                off_y = -kFpPoint05_6FD46C;
            }

            if (mSubState > 0 && mSubState <= 4)
            {
                mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 147);
                dir.x = 0;
                dir.y = (kFP16One_6FD4A0 - Fix16(mSubState) * kFpPoint1_6FD2E8) * mSpeed20;
            }
            else
            {
                mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 147);
                dir.x = 0;
                dir.y = (mSpeed20 * kFpPoint6) * Fix16(mSubState / 10);
            }

            dir.RotateByAngle(mAngle);
            mAdditionalSpeed.x = dir.x;
            mAdditionalSpeed.y = dir.y;

            jitter_x = Fix16(gRng.get_int(3) - 1);
            jitter_x = jitter_x / 50;
            jitter_y = Fix16(gRng.get_int(3) - 1);
            jitter_y = jitter_y / 50;

            mSpeed8.x = mAdditionalSpeed.x + off_x + jitter_x;
            mSpeed8.y = mAdditionalSpeed.y + off_y + jitter_y;
            xpos = xpos + mSpeed8.x;
            ypos = ypos + mSpeed8.y;
            gParticlePosX = xpos;
            gParticlePosY = ypos;

            if (gParticlePosX > kFP16One_6FD4A0 && gParticlePosX < kFp255_6FD280 - kFP16One_6FD4A0 && gParticlePosY > kFP16One_6FD4A0 &&
                gParticlePosY < kFp255_6FD280 - kFP16One_6FD4A0)
            {
                mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos);
            }
            else
            {
                mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x, mpOwnSprite->mXy.y, zpos);
            }

            mpOwnSprite->SetBlendAndAlpha(2, 15);
            mpOwnSprite->SetNoLighting();
            if (mpExplosion)
            {
                mpOwnSprite->ResolveZOrder(mpExplosion->mpObj2C->mSprite);
            }
            gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
            return false;
        }
    }
    return true;
}

// https://decomp.me/scratch/2UJLM
MATCH_FUNC(0x539890)
bool Particle_4C::UpdateCircularBurst_state_5()
{
    bool bJitter = true;
    Fix16 jitter_y;
    Fix16_Point dir(Fix16(0), Fix16(0));

    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mCounter == 0)
    {
        return true;
    }

    if (mpExplosion)
    {
        if (mpExplosion->mId != mExplosionId)
        {
            mpExplosion = NULL;
        }
        else if (mpExplosion->mpObj2C)
        {
            if (mpExplosion->mpObj2C->mSprite)
            {
                mSpeed20 = mpExplosion->mpObj2C->mSprite->mObject2CPtr->GetEffectiveSpeed();
                mAngle = mpExplosion->mpObj2C->mSprite->mObject2CPtr->mObj3c->mAngle;
            }
            if (mpExplosion->mTimer == 1)
            {
                mpExplosion = NULL;
            }
        }
        else
        {
            mpExplosion = NULL;
            mSpeed20 = kFP16Zero_6FD49C;
        }
    }
    else
    {
        mSpeed20 = kFP16Zero_6FD49C;
    }
    mSpeed20 = kFP16Zero_6FD49C;

    Fix16 zpos = mpOwnSprite->mZpos + kFpOne1024th;
    if (zpos >= kMaxZ)
    {
        return true;
    }

    Fix16 xpos = mpOwnSprite->mXy.x;
    Fix16 ypos = mpOwnSprite->mXy.y;
    Fix16 off_y = -kFpPoint01_6FD45C;

    switch (static_cast<u8>(mCounter >> 2))
    {
        case 2:
            if (gRng.get_int(2))
            {
                return true;
            }
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 102);
            mpOwnSprite->SetBlendAndAlpha(1, 25);
            mpOwnSprite->ClearNoLighting();
            dir.x = 0;
            dir.y = kFpPoint6 * mSpeed20;
            bJitter = false;
            break;
        case 3:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 101);
            mpOwnSprite->SetBlendAndAlpha(1, 25);
            mpOwnSprite->ClearNoLighting();
            dir.x = 0;
            dir.y = kFpPoint7 * mSpeed20;
            break;
        case 4:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 100);
            dir.x = 0;
            dir.y = kFpPoint8 * mSpeed20;
            break;
        case 5:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 99);
            dir.x = 0;
            dir.y = kFpPoint8 * mSpeed20;
            break;
        case 6:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 98);
            dir.x = 0;
            dir.y = 0;
            if (mpExplosion && mpExplosion->mpObj2C->mSprite)
            {
                Sprite* pTarget = mpExplosion->mpObj2C->mSprite;
                xpos = pTarget->mXy.x;
                ypos = pTarget->mXy.y;
                zpos = pTarget->mZpos;
            }
            bJitter = false;
            break;
        case 7:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 97);
            dir.x = 0;
            dir.y = 0;
            if (mpExplosion && mpExplosion->mpObj2C->mSprite)
            {
                Sprite* pTarget = mpExplosion->mpObj2C->mSprite;
                xpos = pTarget->mXy.x;
                ypos = pTarget->mXy.y;
                zpos = pTarget->mZpos;
            }
            bJitter = false;
            break;
        default:
            if (gRng.get_int(2))
            {
                return true;
            }
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 103);
            dir.x = 0;
            dir.y = mSpeed20 / kFP16Two_6FD4A4;
            bJitter = false;
            if (gRng.get_int(7) == 4 && mpOwnSprite->mXy.x > kFP16One_6FD4A0 &&
                mpOwnSprite->mXy.x < kFp255_6FD280 - kFP16One_6FD4A0 && mpOwnSprite->mXy.y > kFP16One_6FD4A0 &&
                mpOwnSprite->mXy.y < kFp255_6FD280 - kFP16One_6FD4A0)
            {
                Particle_4C* pSmoke = gParticle_8->New(0, 0, 0, 0, 0, 0);
                if (pSmoke)
                {
                    pSmoke->mUnused34 = 1;
                    pSmoke->mState = 6;
                    pSmoke->mCounter = 100;
                    pSmoke->mInitialCounter = 100;
                    pSmoke->mpOwnSprite->SetType(sprite_types_enum::code_obj2_8);
                    pSmoke->mState = 6;
                    pSmoke->mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx);
                    pSmoke->mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x,
                                                                mpOwnSprite->mXy.y,
                                                                mpOwnSprite->mZpos);
                    gSpriteGrid_3->AddToSingleBucket(pSmoke->mpOwnSprite);
                }
            }
            break;
    }

    // 9.6f: Fix16_Point::RotateByAngle (inlined). The original calls the out-of-line
    // Add/Multiply/Negated copies for all but x * cos and y * sin
    {
        Fix16 sin = Ang16::sine(mAngle);
        Fix16 cos = Ang16::cosine(mAngle);
        Fix16 old_x = dir.x;
        dir.x = (dir.x * cos).Add(dir.y * sin);
        dir.y = old_x.Negated().Multiply(sin).Add(dir.y.Multiply(cos));
    }
    mAdditionalSpeed.x = dir.x;
    mAdditionalSpeed.y = dir.y;

    Fix16 jitter_x = 0;
    jitter_y = 0;
    if (bJitter)
    {
        Fix16 rx;
        rx = Fix16(gRng.get_int(3) - 1);
        jitter_x = rx / 100;
        Fix16 ry;
        ry = Fix16(gRng.get_int(3) - 1);
        jitter_y = ry / 100;
    }

    mSpeed8.x = mAdditionalSpeed.x + jitter_x;
    mSpeed8.y = mAdditionalSpeed.y + off_y + jitter_y;
    xpos += mSpeed8.x;
    ypos += mSpeed8.y;
    gParticlePosX = xpos;
    gParticlePosY = ypos;

    if (xpos > kFP16One_6FD4A0 && xpos < kFp255_6FD280 - kFP16One_6FD4A0 && ypos > kFP16One_6FD4A0 &&
        ypos < kFp255_6FD280 - kFP16One_6FD4A0)
    {
        mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos);
    }
    else
    {
        mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x, mpOwnSprite->mXy.y, zpos);
    }

    mpOwnSprite->mFlags = 0xA2;
    mpOwnSprite->SetNoLighting();
    gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);

    if (++mSubState > 8)
    {
        mSubState = 0;
    }
    return false;
}

MATCH_FUNC(0x53a180)
bool Particle_4C::UpdateStaticAnim_state_39()
{
    gSpriteGrid_3->Remove(this->mpOwnSprite);

    if (this->mCounter == 0)
    {
        return true;
    }

    if (mTimer > 0)
    {
        this->mTimer--;
    }
    else
    {
        this->mTimer = 3;
        this->mSubState++;
        if (mSubState > 5u)
        {
            this->mSubState = 5;
        }
    }

    mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + this->mSubState + 191);
    gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);
    return false;
}

// 9.6f 0x48BEE0: the car box is read through GetCarWidth/GetCarHeight (puts this in edi)
MATCH_FUNC(0x53a280)
bool Particle_4C::UpdateSkidOrScrapeSpark_state_40_41()
{
    Fix16_Point corner;

    gSpriteGrid_3->Remove(mpOwnSprite);
    // The original enters EH state 2 right after Remove: two more points, one unused
    Fix16_Point pos;
    Fix16_Point offset;

    gParticlePosX = mpOwnSprite->mXy.x;
    gParticlePosY = mpOwnSprite->mXy.y;

    if (mpSprite->mSpriteTypeEnum == sprite_types_enum::car_2)
    {
        Car* pCar = mpSprite->AsCar();
        if (!pCar || pCar->IsDespawning())
        {
            return true;
        }

        // The copy of the incremented state (dl) outlives the stored value (bl, reused for the angle)
        u8 sub = ++mSubState;
        if (sub == 5)
        {
            return true;
        }

        Ang16 angle = mpSprite->mRotation;
        Fix16 half_w;
        Fix16 half_h;
        if (sub < 4)
        {
            half_w = pCar->GetCarWidth() / 2 + kFpMinusOneThirtySecond;
            half_h = pCar->GetCarHeight() / 2 + kFpPoint2PlusOne64th;
        }
        else
        {
            half_w = pCar->GetCarWidth() / 2 + kFpMinusOneThirtySecond;
            half_h = pCar->GetCarHeight() / 2 + kFpPoint1_6FD2E8 + kFpOne64th_6FD554;
        }

        // The rotations are written out (RotateByAngle_OOL_40F6B0 shape): with the helper in both
        // branches the inline budget runs out and the Fix16_Point ctors / unary minus go out of line.
        // The original has `corner += get_x_y` in each branch (tail-merged after the lea of its return temp),
        // and state 41's sin/cos in swapped stack slots (from `x_old = -half_w` before the corner stores).
        if (mState == 40)
        {
            corner.x = half_w;
            corner.y = half_h;
            Fix16 x_old = corner.x;
            Fix16 sin = Ang16::sine(angle);
            Fix16 cos = Ang16::cosine(angle);
            corner.x = corner.x.Multiply(cos).Add(corner.y.Multiply(sin));
            corner.y = (-x_old).Multiply(sin).Add(corner.y.Multiply(cos));
            corner += mpSprite->get_x_y();
        }
        else
        {
            Fix16 x_old = -half_w;
            corner.x = x_old;
            corner.y = half_h;
            Fix16 sin = Ang16::sine(angle);
            Fix16 cos = Ang16::cosine(angle);
            corner.x = corner.x.Multiply(cos).Add(corner.y.Multiply(sin));
            corner.y = (-x_old).Multiply(sin).Add(corner.y.Multiply(cos));
            corner += mpSprite->get_x_y();
        }

        mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 200);
        mpOwnSprite->mFlags = 0xA2;
        mpOwnSprite->set_ang_lazy(mpSprite->mRotation);
        mpOwnSprite->set_xyz_lazy(corner.x, corner.y, mpSprite->mZpos);
        mpOwnSprite->SetNoLighting();
        gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
        return false;
    }
    else
    {
        Char* pChar = mpSprite->AsCharB4();
        if (!pChar || !pChar->mpPed || !pChar->mpPed->mBf21C.bActive)
        {
            return true;
        }

        if (++mSubState == 4)
        {
            return true;
        }

        if (!pChar->mFlagsBf.bReverseVelocity)
        {
            offset.x = -kFpPoint05_6FD46C;
            offset.y = kFpPoint3_6FD2F4;
        }
        else
        {
            offset.x = -kFpPoint05_6FD46C;
            offset.y = kFpPoint4;
        }
        offset.RotateByAngle_OneMulInline_40F6B0(mpSprite->mRotation);
        offset = offset.Add(pChar->mVelocityVector);

        mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 197);
        mpOwnSprite->mFlags = 0xA2;
        mpOwnSprite->set_xyz_lazy(mpSprite->mXy.x + offset.x,
                                            mpSprite->mXy.y + offset.y,
                                            mpSprite->mZpos);
        mpOwnSprite->SetNoLighting();
        gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
        return false;
    }
}

MATCH_FUNC(0x53ab70)
bool Particle_4C::Empty_state_42()
{
    return false;
}

// 9.6f 0x48C270
MATCH_FUNC(0x53aba0)
bool Particle_4C::UpdateSimpleBallisticMotion_state_1()
{

    Fix16 z = kFpPoint05_6FD46C;
    Fix16 x = kFP16Zero_6FD49C;
    Fix16 y = kFP16Zero_6FD49C;
    Fix16_Point point2(Fix16(0), Fix16(0));
    Fix16 new_z;

    gSpriteGrid_3->Remove(this->mpOwnSprite);

    if (!mCounter)
    {
        return true;
    }

    if (mCounter > this->mInitialCounter >> 1)
    {
        this->mSpeed8.x = this->mAdditionalSpeed.x + this->mSpeed8.x;
        this->mSpeed8.y = this->mAdditionalSpeed.y + mSpeed8.y;
        new_z = mpOwnSprite->mZpos + z;
    }
    else
    {
        this->mSpeed8.x = this->mAdditionalSpeed.x + this->mSpeed8.x;
        this->mSpeed8.y = this->mAdditionalSpeed.y + mSpeed8.y;
        new_z = mpOwnSprite->mZpos - z;
    }

    gParticlePosX = x + mSpeed8.x + mpOwnSprite->mXy.x;
    gParticlePosY = y + mSpeed8.y + mpOwnSprite->mXy.y;
    if (new_z > kMaxZ)
    {
        new_z = kMaxZ;
    }

    this->mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, new_z);

    gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);

    return false;
}

MATCH_FUNC(0x53ae60)
bool Particle_4C::UpdateLargeBallisticDebris_state_35()
{
    Fix16 z_offset = kFpPoint1_6FD2E8;
    Fix16 x = kFP16Zero_6FD49C;
    Fix16 y = kFP16Zero_6FD49C;
    Fix16_Point point2(Fix16(0), Fix16(0));

    ++mSubState;
    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mSubState == 15)
    {
        return true;
    }

    Fix16 zpos;
    if (mCounter > (mInitialCounter >> 1))
    {
        mSpeed8.x = mSpeed8.x + mAdditionalSpeed.x;
        mSpeed8.y = mSpeed8.y + mAdditionalSpeed.y;
        zpos = mpOwnSprite->mZpos + z_offset;
    }
    else
    {
        mSpeed8.x = mSpeed8.x + mAdditionalSpeed.x;
        mSpeed8.y = mSpeed8.y + mAdditionalSpeed.y;
        zpos = mpOwnSprite->mZpos - z_offset;
    }
    gParticlePosX = mpOwnSprite->mXy.x + mSpeed8.x + x;
    gParticlePosY = mpOwnSprite->mXy.y + mSpeed8.y + y;

    if (zpos > kMaxZ)
    {
        zpos = kMaxZ;
    }
    mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 131);
    mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, zpos);
    gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
    mpOwnSprite->mFlags = 0xA2;
    return false;
}

MATCH_FUNC(0x53b170)
bool Particle_4C::Empty_state_44()
{
    return false;
}

// https://decomp.me/scratch/fW2BZ
MATCH_FUNC(0x53b1a0)
bool Particle_4C::UpdateDebrisArc_state_7()
{
    Fix16 z_offset = kFpPoint05_6FD46C;
    Fix16 x = kFP16Zero_6FD49C;
    Fix16 y = kFP16Zero_6FD49C;
    Fix16_Point point2(Fix16(0), Fix16(0));

    gSpriteGrid_3->Remove(mpOwnSprite);
    if (!mCounter)
    {
        return true;
    }

    Fix16 zpos;
    if (mCounter > (mInitialCounter >> 1))
    {
        mSpeed8.x = mSpeed8.x + mAdditionalSpeed.x;
        mSpeed8.y = mSpeed8.y + mAdditionalSpeed.y;
        zpos = mpOwnSprite->mZpos + z_offset;
    }
    else
    {
        mSpeed8.x = mSpeed8.x + mAdditionalSpeed.x;
        mSpeed8.y = mSpeed8.y + mAdditionalSpeed.y;
        zpos = mpOwnSprite->mZpos - z_offset;
    }
    gParticlePosX = x + mSpeed8.x + mpOwnSprite->mXy.x;
    gParticlePosY = y + mSpeed8.y + mpOwnSprite->mXy.y;

    switch (mCounter)
    {
        case 5:
        case 6:
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 127);
            break;
        case 4:
            mpOwnSprite->set_id(gObjectDefinitions->mDef112BaseSpriteIdx + 128);
            break;
        case 2:
        case 3:
            mpOwnSprite->set_id(gObjectDefinitions->mDef112BaseSpriteIdx + 129);
            break;
        case 0:
        case 1:
            mpOwnSprite->set_id(gObjectDefinitions->mDef112BaseSpriteIdx + 130);
            break;
        default:
            break;
    }
    if (zpos > kMaxZ)
    {
        zpos = kMaxZ;
    }
    mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, zpos);
    gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
    return false;
}

// 9.6f 0x48C790
MATCH_FUNC(0x53b580)
bool Particle_4C::UpdateShortAnim_state_37()
{
    gSpriteGrid_3->Remove(this->mpOwnSprite);

    mSubState++;

    if (mSubState == 5)
    {
        return true;
    }

    switch (mSubState)
    {
        case 1:
            this->mpOwnSprite->set_id_lazy(mpOwnSprite->mTypeSpriteIdx);
            break;

        default:
            this->mpOwnSprite->set_id(mpOwnSprite->mTypeSpriteIdx + 4);
            break;
    }

    this->mpOwnSprite->SetNoLighting();
    gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);
    return false;
}

MATCH_FUNC(0x53b670)
bool Particle_4C::UpdateAttachedEmitter_state_9_10()
{
    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mCounter == 0)
    {
        return true;
    }

    gParticlePosX = mpOwnSprite->mXy.x;
    gParticlePosY = mpOwnSprite->mXy.y;

    Sprite* pSprite = mpSprite;
    Fix16 zpos = mpOwnSprite->mZpos;
    Fix16_Point_POD offset;
    if (pSprite->GetType() != sprite_types_enum::ped_3)
    {
        return true;
    }

    Char* pChar = pSprite->mCharB4Ptr;

    Ped* pPed = pChar->get_ped();
    if (!pPed || !pPed->IsActive())
    {
        return true;
    }

    if (mState == 9)
    {
        if (pChar->mAnimationState != Char_Anim_state::Standing_2)
        {
            return true;
        }
        mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 3);
        if (pChar->mAnimationFrame != 5)
        {
            gParticle_8->SpawnCigaretteSmokePuff(mpSprite, 0);
        }
    }
    else
    {
        if (mCounter < 60)
        {
            if (mCounter > 40)
            {
                // As in 9.6f (0x48FE90): the sum is an operator+ temporary passed straight to PolarToCartesian,
                // which puts it at the original's slot (0x12). `radius` assigned, not initialised: the extra
                // size gives PolarToCartesian's first operator* enough nested inline budget.
                Ang16 jitter;
                jitter = Ang16::Fix16_To_Ang16(Fix16((gRng.get_int(8) - 4) / 2));
                {
                    Fix16 radius;
                    radius = kFpOne1024th * kFpThree_6FD4A8;
                    Ang16::PolarToCartesian_41FC20(mpSprite->mRotation + jitter, radius, offset.x, offset.y);
                }
                mpOwnSprite->SetBlendAndAlpha(1, 10);
                mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 17);
                zpos += kFpPoint04;
            }
            else
            {
                Fix16 radius = kFP16One_6FD4A0 * kFpOne1024th;
                Ang16::PolarToCartesian_41FC20(mpOwnSprite->mRotation, radius, offset.x, offset.y);
                mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 17);
                mpOwnSprite->SetBlendAndAlpha(1, 5);
                zpos += kFpPoint04;
            }
        }
        else
        {
            Fix16 radius = kFpOne1024th * kFpThree_6FD4A8;
            Ang16::PolarToCartesian_41FC20(mpSprite->mRotation, radius, offset.x, offset.y);
            mpOwnSprite->SetBlendAndAlpha(1, 20);
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + 17);
        }
        gParticlePosX += offset.x;
        gParticlePosY += offset.y;
    }

    if (zpos > kMaxZ)
    {
        zpos = kMaxZ;
    }
    mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, zpos);
    gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
    return false;
}

MATCH_FUNC(0x53b9f0)
bool Particle_4C::UpdateBurstAnimation_state_29_30()
{
    Fix16 scale_related;
    u8 idx;

    ++mSubState;
    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mSubState > 25)
    {
        return true;
    }

    if (mSubState < 20)
    {
        if (mSubState < 17)
        {
            if (mSubState < 12)
            {
                scale_related = kFpPoint01_6FD45C;
                idx = 0;
            }
            else
            {
                scale_related = kFpPoint005;
                idx = 1;
            }
        }
        else
        {
            scale_related = kFpPoint005;
            idx = 2;
        }
    }
    else
    {
        scale_related = kFpPoint003;
        idx = 3;
    }

    mpOwnSprite->set_id(idx + gObjectDefinitions->mDef112BaseSpriteIdx + 37);
    gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
    mpOwnSprite->SetBlendAndAlpha(2, 10);
    mpOwnSprite->SetNoLighting();
    mpOwnSprite->ApplyScaleToDimensions(kFP16One_6FD4A0 + scale_related * Fix16(mSubState), 0);
    return false;
}

MATCH_FUNC(0x53bac0)
bool Particle_4C::UpdateCollisionBurst_state_31_34()
{
    Char* pChar = NULL;
    Car* pCar = NULL;
    Ang16 spread;
    // Declaration order sets C2's symbol numbers: the vel + d operand order and the ypos/xpos reloads depend on it
    Fix16 dx;
    Fix16 dy;
    Fix16_Point vel;
    Ang16 angle;
    Fix16 zpos;
    Fix16 ypos;
    Fix16 xpos;
    gSpriteGrid_3->Remove(mpOwnSprite);
    if (mCounter == 0)
    {
        return true;
    }

    xpos = gParticlePosX = mpOwnSprite->mXy.x;
    ypos = gParticlePosY = mpOwnSprite->mXy.y;
    zpos = mpOwnSprite->mZpos;

    Sprite* pTarget = mpSprite;
    switch (pTarget->mSpriteTypeEnum)
    {
        case sprite_types_enum::car_2:
            pCar = pTarget->mCarBcPtr;
            if (!pCar || !pCar->mDriver)
            {
                return true;
            }
            if (!pCar->mDriver->mBf21C.bAttacking)
            {
                mBf4.bDriverAttacking = false;
            }
            if (!pCar->mPhysics)
            {
                return true;
            }
            break;

        default:
            return true;

        case sprite_types_enum::ped_3:
            pChar = pTarget->mCharB4Ptr;
            if (!pChar || !pChar->mpPed || !pChar->mpPed->IsActive())
            {
                return true;
            }
            if (!pChar->mpPed->mBf21C.bAttacking)
            {
                mBf4.bDriverAttacking = false;
            }
            angle = pTarget->mRotation;
            break;
    }

    if (mTimer > 0)
    {
        --mTimer;
    }
    else
    {
        if (++mSubState > 15)
        {
            return true;
        }
        if (mSubState > 3)
        {
            mTimer = 1;
        }
    }

    spread = Ang16::Fix16_To_Ang16(kFpOne64th_6FD328 * (Fix16(mSubState) + Fix16(gRng.get_int(static_cast<s16>(mSubState))) - kFpFour_6FD4AC));

    if (pChar)
    {
        vel = pChar->mpPed->GetVelocityVector();
    }
    else
    {
        Ang16 car_angle;
        Fix16_Point offset;
        Fix16_Point attach;
        AttachedSprite* pGun = pCar->mAttachments.GetSpriteForModel(objects::water_cannon_114);
        if (pGun)
        {
            car_angle = pGun->mpSprite->mRotation + kAng180_6FD3EE;
            attach.x = 0;
            attach.y = kFpSevenThirtySeconds_6FD2D4;
            attach.RotateByAngle(car_angle);
            offset.SetXY(Fix16(0), kFP16Eighth_6FD2D0);
        }
        else
        {
            car_angle = pCar->mAttachments.GetSpriteForModel(248)->mpSprite->mRotation;
            attach.x = 0;
            attach.y = kFpThreeThirtySeconds_6FD48C;
            attach.RotateByAngle(car_angle);
            offset.SetXY(Fix16(0), kFpMinusNineThirtySeconds_6FD4CC);
        }
        offset.RotateByAngle(pCar->mCarSprite->mRotation);
        attach += offset.Add(pCar->mCarSprite->get_x_y());
        vel = pCar->mPhysics->GetPointVelocity(&attach);
        angle = car_angle;
    }

    vel.GetLength_41E260();

    if ((mBf4.bDriverAttacking) && mSubState <= 12)
    {
        Ang16::PolarToCartesian_41FC20(spread + angle, kFpPoint1_6FD2E8, dx, dy);
    }
    else
    {
        Ang16::PolarToCartesian_41FC20(mpOwnSprite->mRotation, kFpPoint1_6FD2E8, dx, dy);
    }
    gParticlePosX += vel.x + dx;
    gParticlePosY += vel.y + dy;

    u8 max_sub_state;
    if (mState == 31)
    {
        mpOwnSprite->set_id_lazy(mSubState + gObjectDefinitions->mDef112BaseSpriteIdx + 73);
        max_sub_state = 14;
    }
    else
    {
        mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx + mSubState + 111);
        max_sub_state = 12;
    }
    mpOwnSprite->mFlags = 0xA2;
    mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, zpos);

    if (mSubState > 1)
    {
        if (mSubState < max_sub_state)
        {
            Sprite* pHit = mpOwnSprite->QuerySpriteCollision(sprite_types_enum::car_2);
            if (pHit && pHit != mpSprite)
            {
                if (mState == 31)
                {
                    mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos + kFpPoint01_6FD45C);
                    if (mSubState < 10)
                    {
                        mSubState = 10;
                    }
                    gParticle_8->mFireHitObj->mSprite->set_xyz_lazy(xpos, ypos, zpos);
                    if (pChar)
                    {
                        gParticle_8->mFireHitObj->SetDamageOwner(pChar->mpPed->get_ped_ref_idx());
                    }
                    else if (pCar)
                    {
                        gParticle_8->mFireHitObj->SetDamageOwner(pCar->mDriver->get_ped_ref_idx());
                    }
                    if (!pHit->IsObject2CType())
                    {
                        gParticle_8->mFireHitObj->HandleObjectHit(pHit);
                    }
                }
                else
                {
                    mSubState = max_sub_state;
                    mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos + kFpPoint01_6FD45C);
                    if (!(gpRng->get_cur_rng() & 1))
                    {
                        gParticle_8->EmitWaterSplash(xpos, ypos, zpos, angle, false);
                    }
                    gParticle_8->mWaterHitObj->mSprite->set_xyz_lazy(xpos, ypos, zpos);
                    if (pChar)
                    {
                        gParticle_8->mWaterHitObj->SetDamageOwner(pChar->mpPed->get_ped_ref_idx());
                    }
                    else if (pCar)
                    {
                        gParticle_8->mWaterHitObj->SetDamageOwner(pCar->mDriver->get_ped_ref_idx());
                    }
                    if (!pHit->IsObject2CType())
                    {
                        gParticle_8->mWaterHitObj->HandleObjectHit(pHit);
                    }
                    if (pHit->GetType() == sprite_types_enum::ped_3)
                    {
                        pHit->mCharB4Ptr->HandleGenericImpact(angle, kFpPoint1_6FD2E8 + kFpPoint05_6FD46C, kFP16Zero_6FD49C, 0);
                        Ped* pHitPed = pHit->mCharB4Ptr->mpPed;
                        if (pHitPed)
                        {
                            if (mState == 31)
                            {
                                gScriptManager->RecordWeaponHit(pHitPed->mId, 194, 1);
                            }
                            else
                            {
                                gScriptManager->RecordWeaponHit(pHitPed->mId, 198, 1);
                            }
                        }
                    }
                }
            }
        }
    }

    if (mpOwnSprite->CheckSpriteMovementRegion())
    {
        mpOwnSprite->set_xyz_lazy(xpos, ypos, zpos + kFpPoint01_6FD45C);
        if (mSubState < 10)
        {
            mSubState = 10;
        }
    }
    gSpriteGrid_3->AddToSingleBucket(mpOwnSprite);
    return false;
}

// The frame-delay store is written twice (two if branches): VC6 counts the uses of the constant 1 before
// merging the identical branches, and keeps it in bl for the stores and the return 1.
MATCH_FUNC(0x53d260)
bool Particle_4C::PoolUpdate()
{
    s32 state = this->mState;
    --this->mCounter;
    switch (state - 1)
    {
        case 17:
        case 32:
            if (mTimer > 0)
            {
                this->mTimer--;
                return false;
            }
            else
            {
                this->mSubState++;
                if (mSubState < 6u)
                {
                    this->mTimer = 1;
                }
                else if (mSubState > 10u)
                {
                    this->mTimer = 1;
                }

                gSpriteGrid_3->Remove(this->mpOwnSprite);
                if (mSubState > 15u)
                {
                    return true;
                }

                mpOwnSprite->set_id_lazy(mSubState + gObjectDefinitions->mDef112BaseSpriteIdx + 20);
                if (mpOwnSprite->mZpos + kFpPoint2_6FD2F0 < kMaxZ)
                {
                    mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x,
                                                        mpOwnSprite->mXy.y,
                                                        mpOwnSprite->mZpos + kFpPoint1_6FD2E8);
                }

                gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);
                if (this->mSubState == 2)
                {
                    this->mpOwnSprite->SetBlendAndAlpha(2, 20);
                }
                this->mpOwnSprite->mBf2C.bNoLighting = true;
                return false;
            }
            break;

        case 18:
        case 31:
            if (mTimer > 0)
            {
                this->mTimer--;
                return false;
            }
            else
            {
                this->mSubState++;
                if (mSubState < 6u)
                {
                    this->mTimer = 1;
                }
                else if (mSubState > 10u)
                {
                    this->mTimer = 1;
                }

                gSpriteGrid_3->Remove(this->mpOwnSprite);
                if (mSubState > 15u)
                {
                    return true;
                }

                mpOwnSprite->set_id_lazy(mSubState + gObjectDefinitions->mDef112BaseSpriteIdx + 20);

                if (mpOwnSprite->mZpos + kFpPoint2_6FD2F0 < kMaxZ)
                {
                    mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x, mpOwnSprite->mXy.y, mpOwnSprite->mZpos + kFpPoint1_6FD2E8);
                }

                gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);
                if (this->mSubState == 2)
                {
                    this->mpOwnSprite->mFlags = 0xA2;
                }
                mpOwnSprite->mBf2C.bNoLighting = true;
                mpOwnSprite->ApplyScaleToDimensions(kFP16One_6FD4A0 + kFP16Half_6FD39C, 0);
                return false;
            }
            break;

        case 19:
            if (mTimer > 0)
            {
                this->mTimer--;
                return false;
            }
            else
            {
                this->mSubState++;
                if (mSubState < 6u)
                {
                    this->mTimer = 1;
                }
                else if (mSubState > 10u)
                {
                    this->mTimer = 1;
                }

                gSpriteGrid_3->Remove(this->mpOwnSprite);
                if (mSubState > 15u)
                {
                    return true;
                }

                mpOwnSprite->set_id_lazy(mSubState + gObjectDefinitions->mDef112BaseSpriteIdx + 56);

                if (mpOwnSprite->mZpos + kFpPoint2_6FD2F0 < kMaxZ)
                {
                    mpOwnSprite->set_xyz_lazy(mpOwnSprite->mXy.x,
                                                        mpOwnSprite->mXy.y,
                                                        mpOwnSprite->mZpos + kFpPoint1_6FD2E8);
                }

                gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);
                if (this->mSubState == 2)
                {
                    this->mpOwnSprite->SetBlendAndAlpha(2, 20);
                }

                mpOwnSprite->mBf2C.bNoLighting = true;
                mpOwnSprite->ApplyScaleToDimensions(kFP16Two_6FD4A4, 0);
                return false;
            }
            break;

        case 21:
        case 22:
        case 23:
        case 24:
            if (mTimer > 0)
            {
                this->mTimer--;
                return false;
            }

            gSpriteGrid_3->Remove(this->mpOwnSprite);
            this->mTimer = 1;
            this->mSubState++;
            if (mSubState > 0xCu)
            {
                return true;
            }

            mpOwnSprite->set_id_lazy(mSubState + gObjectDefinitions->mDef112BaseSpriteIdx + 147);

            gParticlePosX = this->mSpeed20 * Ang16::sine(this->mAngle);
            gParticlePosY = this->mSpeed20 * Ang16::cosine(this->mAngle);

            gParticlePosX = this->mpOwnSprite->mXy.x + gParticlePosX;
            gParticlePosY = this->mpOwnSprite->mXy.y + gParticlePosY;


            if (this->mSubState < 9u)
            {
                mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpOwnSprite->mZpos + kFpPoint1_6FD2E8);
            }
            else
            {
                mpOwnSprite->set_xyz_lazy(gParticlePosX, gParticlePosY, mpOwnSprite->mZpos + kFpPoint05_6FD46C);
            }

            if (mpOwnSprite->mZpos > kMaxZ)
            {
                mpOwnSprite->set_z_lazy(kMaxZ);
            }

            gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);
            mpOwnSprite->SetBlendAndAlpha(2, 10);
            mpOwnSprite->mBf2C.bNoLighting = true;
            return false;

        case 5:
        case 14:
        case 15:
        case 16:
            return UpdateFloatingParticle_state_6_15_16_17();

        case 2:
        case 11:
            return UpdateDirectedProjectile_state_3_12();

        case 12:
        case 13:
        case 35:
            return UpdateDirectedBurst_state_13_14_36();

        case 3:
            return UpdateDirectedBurstSweep_state_4();

        case 4:
            return UpdateCircularBurst_state_5();

        case 6:
            return UpdateDebrisArc_state_7();

        case 8:
        case 9:
            return UpdateAttachedEmitter_state_9_10();

        case 0:
            return UpdateSimpleBallisticMotion_state_1();

        case 36:
            return UpdateShortAnim_state_37();

        case 34:
            return UpdateLargeBallisticDebris_state_35();

        case 7:
            return true;

        case 25:
            gSpriteGrid_3->Remove(this->mpOwnSprite);
            if (!this->mCounter)
            {
                return true;
            }
            mpOwnSprite->set_id_lazy(gObjectDefinitions->mDef112BaseSpriteIdx);
            gSpriteGrid_3->AddToSingleBucket(this->mpOwnSprite);
            mpOwnSprite->mBf2C.bNoLighting = true;
            return false;

        case 28:
        case 29:
            return UpdateBurstAnimation_state_29_30();

        case 30:
        case 33:
            return UpdateCollisionBurst_state_31_34();

        case 37:
            return UpdateObjectBeamLink_state_38();

        case 38:
            return UpdateStaticAnim_state_39();

        case 39:
        case 40:
            return UpdateSkidOrScrapeSpark_state_40_41();

        case 41:
            return Empty_state_42();

        case 42:
            return UpdateBeamSegment_state_43();

        case 43:
            return Empty_state_44();

        default:
            return false;
    }
}

MATCH_FUNC(0x53e2c0)
void Particle_4C::PoolAllocate()
{
    mSpeed8.x = 0;
    mSpeed8.y = 0;
    mSpeed8.z = 0;
    mpExplosion = NULL;
    mBf4.bDriverAttacking = false;
}

MATCH_FUNC(0x53e2e0)
void Particle_4C::PoolDeallocate()
{
    if (mpOwnSprite)
    {
        mpOwnSprite->ClearNoLighting();
        mpOwnSprite->SetBlendAndAlpha(0, 0);
        gSpritePool->remove(mpOwnSprite);
        mpOwnSprite = NULL;
    }
}
