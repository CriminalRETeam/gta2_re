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
EXTERN_GLOBAL(Fix16, kFpOne1024th_6FD540);
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
DEFINE_GLOBAL_INIT(Fix16, kMaxZ_6FD28C, Fix16(0x1FF00, 0), 0x6FD28C); // 8 - 1/64

DEFINE_GLOBAL(Fix16, gParticlePosX_6FD388, 0x6FD388);
DEFINE_GLOBAL(Fix16, gParticlePosY_6FD38C, 0x6FD38C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6FD2E8, Fix16(0x666, 0), 0x6FD2E8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FD4A0, Fix16(1), 0x6FD4A0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FD39C, Fix16(0.5f), 0x6FD39C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_6FD4A4, Fix16(2), 0x6FD4A4);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_6FD4A8, Fix16(3), 0x6FD4A8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint04_6FD470, Fix16(0x28F, 0), 0x6FD470);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_6FD45C, Fix16(0xA3, 0), 0x6FD45C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint005_6FD564, Fix16(0x51, 0), 0x6FD564);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint003_6FD538, Fix16(0x31, 0), 0x6FD538);

DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_6FD55C, Fix16(0x3000, 0), 0x6FD55C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint9_6FD30C, Fix16(0x3999, 0), 0x6FD30C);
DEFINE_GLOBAL_INIT(Fix16, kFp255_6FD280, Fix16(255), 0x6FD280);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint6_6FD300, Fix16(0x2666, 0), 0x6FD300);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint7_6FD304, Fix16(0x2CCC, 0), 0x6FD304);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6FD308, Fix16(0x3333, 0), 0x6FD308);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixth_6FD364, Fix16(0xAAA, 0), 0x6FD364);
DEFINE_GLOBAL_INIT(Fix16, kFpMinusOneThirtySecond_6FD3C0, Fix16(0xFFFFFE00, 0), 0x6FD3C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD5A8, Fix16(0xDCC, 0), 0x6FD5A8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint4_6FD2F8, Fix16(0x1999, 0), 0x6FD2F8);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_6FD4AC, Fix16(4), 0x6FD4AC);

// https://decomp.me/scratch/nKSYL
MATCH_FUNC(0x538060)
bool Particle_4C::UpdateFloatingParticle_state_6_15_16_17_538060()
{
    Fix16 rng_1;
    Fix16 rng_2;
    Fix16 off_y;
    Fix16_Point vector(Fix16(0), Fix16(0));
    Fix16 new_z = kFpPoint01_6FD45C + field_30_pOwnSprite->field_1C_zpos;
    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_2C_counter == 0)
    {
        return true;
    }
    if (new_z > kMaxZ_6FD28C)
    {
        return true;
    }

    if (new_z.GetFracValue() > kFpThreeQuarters_6FD55C &&
        gMap_0x370_6F6268->IsBlockNonAirType_48A350(field_30_pOwnSprite->field_14_xy.x.ToInt(),
                                                    field_30_pOwnSprite->field_14_xy.y.ToInt(),
                                                    new_z.ToInt()))
    {
        new_z = field_30_pOwnSprite->field_1C_zpos; // blocked above: stay at the current height
        Fix16 r1;
        r1 = Fix16(gRng_6F6784.get_int_4F7AE0(61) - 30);
        rng_1 = r1 / 100;
        Fix16 r2;
        r2 = Fix16(gRng_6F6784.get_int_4F7AE0(10) - 5);
        rng_2 = r2 / 100;
        ++field_2C_counter;
    }
    else
    {
        Fix16 r1;
        r1 = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
        rng_1 = r1 / 100;
        Fix16 r2;
        r2 = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
        rng_2 = r2 / 100;
    }

    if (field_40_pExplosion)
    {
        if (field_2C_counter > 60)
        {
            if (field_40_pExplosion->field_14_pObj2C->field_4_sprite)
            {
                field_20_speed = field_40_pExplosion->field_14_pObj2C->field_4_sprite->field_8_object_2C_ptr->GetEffectiveSpeed_5290F0();
                field_24_angle = field_40_pExplosion->field_14_pObj2C->field_4_sprite->field_8_object_2C_ptr->field_10_obj_3c->field_4_angle;
            }
            if (field_40_pExplosion->field_1A_timer == 1)
            {
                field_40_pExplosion = NULL;
            }
        }
    }
    else
    {
        field_20_speed = kFP16Zero_6FD49C;
    }

    if (field_20_speed == kFP16Zero_6FD49C)
    {
        // Assigned (not initialised) here so VC6 keeps the neg; add instead of folding it into a sub
        off_y = -kFpPoint01_6FD45C;
        gParticlePosX_6FD388 = field_30_pOwnSprite->field_14_xy.x + rng_1;
        gParticlePosY_6FD38C = field_30_pOwnSprite->field_14_xy.y + off_y + rng_2;
        if (gParticlePosX_6FD388 > kFP16One_6FD4A0 && gParticlePosX_6FD388 < kFp255_6FD280 - kFP16One_6FD4A0 && gParticlePosY_6FD38C > kFP16One_6FD4A0 &&
            gParticlePosY_6FD38C < kFp255_6FD280 - kFP16One_6FD4A0)
        {
            field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, new_z);
        }
        else
        {
            field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x, field_30_pOwnSprite->field_14_xy.y, new_z);
        }
    }
    else
    {
        field_20_speed = field_20_speed * kFpPoint9_6FD30C;
        if (field_20_speed < kFP16Zero_6FD49C)
        {
            field_20_speed = kFP16Zero_6FD49C;
        }
        // A named copy of the zero constant: gives it ebx and pos_y ebp, as in the original
        Fix16 zero = kFP16Zero_6FD49C;
        Fix16 pos_x = field_30_pOwnSprite->field_14_xy.x;
        Fix16 pos_y = field_30_pOwnSprite->field_14_xy.y;
        vector.y = field_20_speed;
        vector.x = zero;
        {
            Fix16 sin = Ang16::sine_40F500(field_24_angle);
            Fix16 cos = Ang16::cosine_40F520(field_24_angle);
            Fix16 old_x = vector.x;
            vector.x = (vector.x * cos).Add_408660(vector.y * sin);
            vector.y = old_x.Negate_4086A0().Multiply_408680(sin).Add_408660(vector.y.Multiply_408680(cos));
        }

        field_14_additional_speed.x = vector.x;
        field_14_additional_speed.y = vector.y;

        field_8_speed.x = field_14_additional_speed.x + rng_1;
        field_8_speed.y = field_14_additional_speed.y + rng_2;

        pos_x = pos_x + field_8_speed.x;
        pos_y = pos_y + field_8_speed.y;
        gParticlePosX_6FD388 = pos_x;
        gParticlePosY_6FD38C = pos_y;
        if (gParticlePosX_6FD388 > kFP16One_6FD4A0 && gParticlePosX_6FD388 < kFp255_6FD280 - kFP16One_6FD4A0 && gParticlePosY_6FD38C > kFP16One_6FD4A0 &&
            gParticlePosY_6FD38C < kFp255_6FD280 - kFP16One_6FD4A0)
        {
            field_30_pOwnSprite->set_xyz_lazy_420600(pos_x, pos_y, new_z);
        }
        else
        {
            field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x, field_30_pOwnSprite->field_14_xy.y, new_z);
        }
    }

    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
    return false;
}

MATCH_FUNC(0x5384c0)
bool Particle_4C::UpdateDirectedProjectile_state_3_12_5384C0()
{
    Fix16 jitter_x;
    Fix16 off_x = kFP16Zero_6FD49C;
    Fix16 jitter_y;
    Fix16 off_y = kFP16Zero_6FD49C;
    bool bJitter = true;
    Fix16_Point dir(Fix16(0), Fix16(0));

    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_2C_counter == 0)
    {
        return true;
    }

    if (field_40_pExplosion)
    {
        if (field_40_pExplosion->field_6_id != field_44_explosion_id)
        {
            field_40_pExplosion = NULL;
            field_20_speed = kFP16Zero_6FD49C;
        }
        else if (field_40_pExplosion->field_14_pObj2C)
        {
            if (field_40_pExplosion->field_14_pObj2C->field_4_sprite)
            {
                field_20_speed = field_40_pExplosion->field_14_pObj2C->field_4_sprite->field_8_object_2C_ptr->GetEffectiveSpeed_5290F0();
                field_24_angle = field_40_pExplosion->field_14_pObj2C->field_10_obj_3c->field_4_angle;
            }
            if (field_40_pExplosion->field_1A_timer == 1)
            {
                field_40_pExplosion = NULL;
            }
        }
        else
        {
            field_40_pExplosion = NULL;
            field_20_speed = kFP16Zero_6FD49C;
        }
    }
    else
    {
        field_20_speed = kFP16Zero_6FD49C;
    }

    Fix16 zpos = field_30_pOwnSprite->field_1C_zpos + kFpOne1024th_6FD540;
    if (zpos > kMaxZ_6FD28C)
    {
        return true;
    }

    Fix16 xpos = field_30_pOwnSprite->field_14_xy.x;
    Fix16 ypos = field_30_pOwnSprite->field_14_xy.y;

    if (field_20_speed == kFP16Zero_6FD49C)
    {
        off_x = 0;
        off_y = -kFpPoint01_6FD45C;
    }

    switch (static_cast<u8>(field_2C_counter >> 2))
    {
        case 2:
            if (gRng_6F6784.get_int_4F7AE0(2))
            {
                return true;
            }
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 102);
            dir.x = 0;
            dir.y = field_20_speed * kFpPoint6_6FD300;
            bJitter = false;
            break;
        case 3:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 101);
            dir.x = 0;
            dir.y = field_20_speed * kFpPoint7_6FD304;
            break;
        case 4:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 100);
            dir.x = 0;
            dir.y = field_20_speed * kFpPoint8_6FD308;
            break;
        case 5:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 99);
            dir.x = 0;
            dir.y = field_20_speed * kFpPoint8_6FD308;
            break;
        case 6:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 98);
            dir.x = 0;
            dir.y = field_20_speed * kFpPoint8_6FD308;
            break;
        case 7:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 97);
            dir.x = 0;
            dir.y = field_20_speed * kFpPoint8_6FD308;
            break;
        default:
            if (gRng_6F6784.get_int_4F7AE0(2))
            {
                return true;
            }
            break;
    }

    dir.RotateByAngle_40F6B0(field_24_angle);
    field_14_additional_speed.x = dir.x;
    field_14_additional_speed.y = dir.y;

    jitter_x = 0;
    jitter_y = 0;
    if (bJitter)
    {
        Fix16 rx;
        rx = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
        jitter_x = rx / 100;
        Fix16 ry;
        ry = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
        jitter_y = ry / 100;
    }

    field_8_speed.x = field_14_additional_speed.x + off_x + jitter_x;
    field_8_speed.y = field_14_additional_speed.y + off_y + jitter_y;
    xpos += field_8_speed.x;
    ypos += field_8_speed.y;
    gParticlePosX_6FD388 = xpos;
    gParticlePosY_6FD38C = ypos;

    if (xpos > kFP16One_6FD4A0 && xpos < kFp255_6FD280 - kFP16One_6FD4A0 && ypos > kFP16One_6FD4A0 &&
        ypos < kFp255_6FD280 - kFP16One_6FD4A0)
    {
        field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
    }
    else
    {
        field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x, field_30_pOwnSprite->field_14_xy.y, zpos);
    }

    switch (field_46_sub_state)
    {
        case 0:
        case 1:
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(1, 10);
            break;
        case 2:
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(1, 10);
            break;
        case 3:
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 10);
            break;
        case 4:
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 15);
            break;
        case 5:
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 20);
            break;
        case 6:
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 25);
            break;
        case 7:
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 30);
            break;
    }

    field_30_pOwnSprite->SetNoLighting_4337F0();
    if (field_40_pExplosion)
    {
        field_30_pOwnSprite->ResolveZOrder_5A1B30(field_40_pExplosion->field_14_pObj2C->field_4_sprite);
    }
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);

    if (++field_46_sub_state > 8)
    {
        field_46_sub_state = 0;
    }
    return false;
}

MATCH_FUNC(0x538a40)
bool Particle_4C::UpdateBeamSegment_state_43_538A40()
{
    gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);

    this->field_46_sub_state++;
    if (field_46_sub_state > 7u)
    {
        return true;
    }

    field_30_pOwnSprite->set_id_lazy_4206C0(field_46_sub_state + gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 103);

    this->field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 20);
    this->field_30_pOwnSprite->SetNoLighting_4337F0();
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);
    return false;
}

// The normalizing Ang16 ctor (out of line, AssignNormalized_409300) on a by-value parameter: the
// original stores the 16-bit sum as a dword before the call (UpdateObjectBeamLink_state_38_538AC0)
static inline Ang16 NormalizedAng(s16 value)
{
    return Ang16(&value, 0);
}

// 9.6f 0x41E210 (Fix16_Point::SetFromPolar(Fix16& radius, Ang16& angle)) with the multiplies out of line.
// The angle is a temporary passed by reference, so the table index is reloaded after each multiply.
static inline void SetFromPolar_OOL_41E210(Fix16_Point& p, const Fix16& radius, const Ang16& angle)
{
    p.x = radius.Multiply_408680(Ang16::sine_40F500(angle));
    p.y = radius.Multiply_408680(Ang16::cosine_40F520(angle));
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

// 9.6f 0x48F230: SetXY_432860 for src/dst, delta = dst - src, delta.atan2_40F790() (result discarded), segments
// from delta.MaxAbs_48A270() / kFpOneSixth_6FD364, delta.DivideAssign_48A250(segments), cur += delta, mid = cur - prev,
// mid.DivideAssign_48A250(2), mid += prev, SetBlendAndAlpha_4337D0(2, 20); the function-scope Ang16 is the jitter;
// each case is one SetFromPolar(radius, angle sum) call with both arguments as temporaries;
// dst.SetXY(target) then dst += src (target declared ahead of src, else the x add's operands swap).
MATCH_FUNC(0x538ac0)
bool Particle_4C::UpdateObjectBeamLink_state_38_538AC0()
{
    ++field_46_sub_state;
    Ang16 jitter;
    // Declared before src: the symbol order decides the operand order of `dst += src` (x add)
    Fix16_Point_POD target;
    Fix16_Point src;
    Fix16_Point dst;
    Fix16_Point delta;
    Fix16_Point cur;
    Fix16_Point prev;
    Fix16_Point mid;
    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_46_sub_state != 6 && field_28_pSprite->GetType_416B40() == sprite_types_enum::code_obj1_4)
    {
        src.SetXY_432860(field_30_pOwnSprite->field_14_xy.x, field_30_pOwnSprite->field_14_xy.y);
        dst.SetXY_432860(field_28_pSprite->field_14_xy.x, field_28_pSprite->field_14_xy.y);
        delta = dst - src;
        delta.atan2_40F790();
        Fix16 segments = delta.MaxAbs_48A270() / kFpOneSixth_6FD364;

        if (segments != kFP16Zero_6FD49C)
        {
            delta.DivideAssign_48A250(segments);
            prev = src;
            cur = src;
            for (s32 i = 1; i <= segments.ToInt(); i++)
            {
                cur += delta;
                mid = cur - prev;
                mid.DivideAssign_48A250(kFP16Two_6FD4A4);
                mid += prev;

                Particle_4C* pSegment = gParticle_8_6FD5E8->New_53E3C0(0, 0, 0, 0, 0, 0);
                if (pSegment)
                {
                    pSegment->field_34_unused = 0;
                    pSegment->field_38_state = 43;
                    pSegment->field_2C_counter = 50;
                    pSegment->field_46_sub_state = 0;
                    pSegment->field_2E_initial_counter = 50;
                    pSegment->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
                    pSegment->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 104);
                    pSegment->field_30_pOwnSprite->set_xyz_lazy_420600(mid.x, mid.y, field_30_pOwnSprite->field_1C_zpos);
                    pSegment->field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 20);
                    pSegment->field_30_pOwnSprite->SetNoLighting_4337F0();
                    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pSegment->field_30_pOwnSprite);
                }
                prev = cur;
            }
        }

        target.SetXY_432860(field_28_pSprite->field_14_xy.x, field_28_pSprite->field_14_xy.y);
        jitter = kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(gRng_6F6784.get_int_4F7AE0(16) - 8));

        switch (field_46_sub_state)
        {
            case 1:
            case 2:
            case 3:
            {
                SetFromPolar_OOL_41E210(src,
                                        Fix16(field_46_sub_state) * kFpPoint05_6FD46C,
                                        NormalizedAng(field_28_pSprite->field_0_rotation.rValue + kAng180_6FD3EE.rValue));
                break;
            }
            case 4:
            case 5:
            {
                SetFromPolar_OOL_41E210(src,
                                        Fix16(field_46_sub_state) * kFpPoint05_6FD46C + kFpPoint01_6FD45C,
                                        NormalizedAng(NormalizedAng(field_28_pSprite->field_0_rotation.rValue + kAng180_6FD3EE.rValue).rValue + jitter.rValue));
                break;
            }
        }

        dst.SetXY_432860(target.x, target.y);
        dst += src;

        if (field_28_pSprite->GetType_416B40() == sprite_types_enum::code_obj1_4)
        {
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 163);
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 20);
            field_30_pOwnSprite->SetNoLighting_4337F0();
            field_30_pOwnSprite->set_xyz_lazy_420600(dst.x, dst.y, field_30_pOwnSprite->field_1C_zpos);
            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
            return 0;
        }
    }
    return true;
}

MATCH_FUNC(0x539040)
bool Particle_4C::UpdateDirectedBurstSweep_state_4_539040()
{
    Fix16 jitter_x;
    Fix16 jitter_y;
    Fix16 off_x = kFP16Zero_6FD49C;
    Fix16 off_y = kFP16Zero_6FD49C;
    Fix16_Point dir(Fix16(0), Fix16(0));

    ++field_46_sub_state;
    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_46_sub_state != 16 && field_2C_counter != 0)
    {
        if (field_40_pExplosion)
        {
            if (field_40_pExplosion->field_6_id != field_44_explosion_id)
            {
                field_40_pExplosion = NULL;
                field_20_speed = kFP16Zero_6FD49C;
            }
            else if (field_40_pExplosion->field_14_pObj2C)
            {
                if (field_40_pExplosion->field_14_pObj2C->field_4_sprite)
                {
                    field_20_speed = field_40_pExplosion->field_14_pObj2C->field_4_sprite->field_8_object_2C_ptr->GetEffectiveSpeed_5290F0();
                    field_24_angle = field_40_pExplosion->field_14_pObj2C->field_10_obj_3c->field_4_angle;
                }
                if (field_40_pExplosion->field_1A_timer == 1)
                {
                    field_40_pExplosion = NULL;
                }
            }
            else
            {
                field_40_pExplosion = NULL;
                field_20_speed = kFP16Zero_6FD49C;
            }
        }
        else
        {
            field_20_speed = kFP16Zero_6FD49C;
        }

        Fix16 zpos = field_30_pOwnSprite->field_1C_zpos + kFpPoint01_6FD45C;
        if (zpos < kMaxZ_6FD28C)
        {

            Fix16 xpos = field_30_pOwnSprite->field_14_xy.x;
            Fix16 ypos = field_30_pOwnSprite->field_14_xy.y;

            if (field_20_speed < kFpPoint03_6FD464)
            {
                off_x = 0;
                off_y = -kFpPoint05_6FD46C;
            }

            if (field_46_sub_state > 0 && field_46_sub_state <= 4)
            {
                field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 147);
                dir.x = 0;
                dir.y = (kFP16One_6FD4A0 - Fix16(field_46_sub_state) * kFpPoint1_6FD2E8) * field_20_speed;
            }
            else
            {
                field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 147);
                dir.x = 0;
                dir.y = (field_20_speed * kFpPoint3_6FD2F4) * Fix16(field_46_sub_state / 10);
            }

            dir.RotateByAngle_40F6B0(field_24_angle);
            field_14_additional_speed.x = dir.x;
            field_14_additional_speed.y = dir.y;

            jitter_x = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
            jitter_x = jitter_x / 30;
            jitter_y = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
            jitter_y = jitter_y / 30;

            field_8_speed.x = field_14_additional_speed.x + off_x + jitter_x;
            field_8_speed.y = field_14_additional_speed.y + off_y + jitter_y;
            xpos = xpos + field_8_speed.x;
            ypos = ypos + field_8_speed.y;
            gParticlePosX_6FD388 = xpos;
            gParticlePosY_6FD38C = ypos;

            if (gParticlePosX_6FD388 > kFP16One_6FD4A0 && gParticlePosX_6FD388 < kFp255_6FD280 - kFP16One_6FD4A0 && gParticlePosY_6FD38C > kFP16One_6FD4A0 &&
                gParticlePosY_6FD38C < kFp255_6FD280 - kFP16One_6FD4A0)
            {
                field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
            }
            else
            {
                field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x, field_30_pOwnSprite->field_14_xy.y, zpos);
            }

            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 15);
            field_30_pOwnSprite->SetNoLighting_4337F0();
            field_30_pOwnSprite->ApplyScaleToDimensions_59E4C0(kFP16One_6FD4A0 + Fix16(field_46_sub_state) * kFpPoint1_6FD2E8, 0);
            if (field_40_pExplosion)
            {
                field_30_pOwnSprite->ResolveZOrder_5A1B30(field_40_pExplosion->field_14_pObj2C->field_4_sprite);
            }
            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x539480)
bool Particle_4C::UpdateDirectedBurst_state_13_14_36_539480()
{
    Fix16 jitter_x;
    Fix16 jitter_y;
    Fix16 off_x = kFP16Zero_6FD49C;
    Fix16 off_y = kFP16Zero_6FD49C;
    Fix16_Point dir(Fix16(0), Fix16(0));

    ++field_46_sub_state;
    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_46_sub_state != 16 && field_2C_counter != 0)
    {
        if (field_40_pExplosion)
        {
            if (field_40_pExplosion->field_6_id != field_44_explosion_id)
            {
                field_40_pExplosion = NULL;
                field_20_speed = kFP16Zero_6FD49C;
            }
            else if (field_40_pExplosion->field_14_pObj2C)
            {
                if (field_40_pExplosion->field_14_pObj2C->field_4_sprite)
                {
                    field_20_speed = field_40_pExplosion->field_14_pObj2C->field_4_sprite->field_8_object_2C_ptr->GetEffectiveSpeed_5290F0();
                    field_24_angle = field_40_pExplosion->field_14_pObj2C->field_10_obj_3c->field_4_angle;
                }
                if (field_40_pExplosion->field_1A_timer == 1)
                {
                    field_40_pExplosion = NULL;
                }
            }
            else
            {
                field_40_pExplosion = NULL;
                field_20_speed = kFP16Zero_6FD49C;
            }
        }
        else
        {
            field_20_speed = kFP16Zero_6FD49C;
        }

        Fix16 zpos = field_30_pOwnSprite->field_1C_zpos + kFpOne1024th_6FD540;
        if (zpos < kMaxZ_6FD28C)
        {

            Fix16 xpos = field_30_pOwnSprite->field_14_xy.x;
            Fix16 ypos = field_30_pOwnSprite->field_14_xy.y;

            if (field_20_speed < kFpPoint03_6FD464)
            {
                off_x = 0;
                off_y = -kFpPoint05_6FD46C;
            }

            if (field_46_sub_state > 0 && field_46_sub_state <= 4)
            {
                field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 147);
                dir.x = 0;
                dir.y = (kFP16One_6FD4A0 - Fix16(field_46_sub_state) * kFpPoint1_6FD2E8) * field_20_speed;
            }
            else
            {
                field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 147);
                dir.x = 0;
                dir.y = (field_20_speed * kFpPoint6_6FD300) * Fix16(field_46_sub_state / 10);
            }

            dir.RotateByAngle_40F6B0(field_24_angle);
            field_14_additional_speed.x = dir.x;
            field_14_additional_speed.y = dir.y;

            jitter_x = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
            jitter_x = jitter_x / 50;
            jitter_y = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
            jitter_y = jitter_y / 50;

            field_8_speed.x = field_14_additional_speed.x + off_x + jitter_x;
            field_8_speed.y = field_14_additional_speed.y + off_y + jitter_y;
            xpos = xpos + field_8_speed.x;
            ypos = ypos + field_8_speed.y;
            gParticlePosX_6FD388 = xpos;
            gParticlePosY_6FD38C = ypos;

            if (gParticlePosX_6FD388 > kFP16One_6FD4A0 && gParticlePosX_6FD388 < kFp255_6FD280 - kFP16One_6FD4A0 && gParticlePosY_6FD38C > kFP16One_6FD4A0 &&
                gParticlePosY_6FD38C < kFp255_6FD280 - kFP16One_6FD4A0)
            {
                field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
            }
            else
            {
                field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x, field_30_pOwnSprite->field_14_xy.y, zpos);
            }

            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 15);
            field_30_pOwnSprite->SetNoLighting_4337F0();
            if (field_40_pExplosion)
            {
                field_30_pOwnSprite->ResolveZOrder_5A1B30(field_40_pExplosion->field_14_pObj2C->field_4_sprite);
            }
            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
            return false;
        }
    }
    return true;
}

// https://decomp.me/scratch/2UJLM
MATCH_FUNC(0x539890)
bool Particle_4C::UpdateCircularBurst_state_5_539890()
{
    bool bJitter = true;
    Fix16 jitter_y;
    Fix16_Point dir(Fix16(0), Fix16(0));

    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_2C_counter == 0)
    {
        return true;
    }

    if (field_40_pExplosion)
    {
        if (field_40_pExplosion->field_6_id != field_44_explosion_id)
        {
            field_40_pExplosion = NULL;
        }
        else if (field_40_pExplosion->field_14_pObj2C)
        {
            if (field_40_pExplosion->field_14_pObj2C->field_4_sprite)
            {
                field_20_speed = field_40_pExplosion->field_14_pObj2C->field_4_sprite->field_8_object_2C_ptr->GetEffectiveSpeed_5290F0();
                field_24_angle = field_40_pExplosion->field_14_pObj2C->field_4_sprite->field_8_object_2C_ptr->field_10_obj_3c->field_4_angle;
            }
            if (field_40_pExplosion->field_1A_timer == 1)
            {
                field_40_pExplosion = NULL;
            }
        }
        else
        {
            field_40_pExplosion = NULL;
            field_20_speed = kFP16Zero_6FD49C;
        }
    }
    else
    {
        field_20_speed = kFP16Zero_6FD49C;
    }
    field_20_speed = kFP16Zero_6FD49C;

    Fix16 zpos = field_30_pOwnSprite->field_1C_zpos + kFpOne1024th_6FD540;
    if (zpos >= kMaxZ_6FD28C)
    {
        return true;
    }

    Fix16 xpos = field_30_pOwnSprite->field_14_xy.x;
    Fix16 ypos = field_30_pOwnSprite->field_14_xy.y;
    Fix16 off_y = -kFpPoint01_6FD45C;

    switch (static_cast<u8>(field_2C_counter >> 2))
    {
        case 2:
            if (gRng_6F6784.get_int_4F7AE0(2))
            {
                return true;
            }
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 102);
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(1, 25);
            field_30_pOwnSprite->ClearNoLighting_433800();
            dir.x = 0;
            dir.y = kFpPoint6_6FD300 * field_20_speed;
            bJitter = false;
            break;
        case 3:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 101);
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(1, 25);
            field_30_pOwnSprite->ClearNoLighting_433800();
            dir.x = 0;
            dir.y = kFpPoint7_6FD304 * field_20_speed;
            break;
        case 4:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 100);
            dir.x = 0;
            dir.y = kFpPoint8_6FD308 * field_20_speed;
            break;
        case 5:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 99);
            dir.x = 0;
            dir.y = kFpPoint8_6FD308 * field_20_speed;
            break;
        case 6:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 98);
            dir.x = 0;
            dir.y = 0;
            if (field_40_pExplosion && field_40_pExplosion->field_14_pObj2C->field_4_sprite)
            {
                Sprite* pTarget = field_40_pExplosion->field_14_pObj2C->field_4_sprite;
                xpos = pTarget->field_14_xy.x;
                ypos = pTarget->field_14_xy.y;
                zpos = pTarget->field_1C_zpos;
            }
            bJitter = false;
            break;
        case 7:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 97);
            dir.x = 0;
            dir.y = 0;
            if (field_40_pExplosion && field_40_pExplosion->field_14_pObj2C->field_4_sprite)
            {
                Sprite* pTarget = field_40_pExplosion->field_14_pObj2C->field_4_sprite;
                xpos = pTarget->field_14_xy.x;
                ypos = pTarget->field_14_xy.y;
                zpos = pTarget->field_1C_zpos;
            }
            bJitter = false;
            break;
        default:
            if (gRng_6F6784.get_int_4F7AE0(2))
            {
                return true;
            }
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 103);
            dir.x = 0;
            dir.y = field_20_speed / kFP16Two_6FD4A4;
            bJitter = false;
            if (gRng_6F6784.get_int_4F7AE0(7) == 4 && field_30_pOwnSprite->field_14_xy.x > kFP16One_6FD4A0 &&
                field_30_pOwnSprite->field_14_xy.x < kFp255_6FD280 - kFP16One_6FD4A0 && field_30_pOwnSprite->field_14_xy.y > kFP16One_6FD4A0 &&
                field_30_pOwnSprite->field_14_xy.y < kFp255_6FD280 - kFP16One_6FD4A0)
            {
                Particle_4C* pSmoke = gParticle_8_6FD5E8->New_53E3C0(0, 0, 0, 0, 0, 0);
                if (pSmoke)
                {
                    pSmoke->field_34_unused = 1;
                    pSmoke->field_38_state = 6;
                    pSmoke->field_2C_counter = 100;
                    pSmoke->field_2E_initial_counter = 100;
                    pSmoke->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
                    pSmoke->field_38_state = 6;
                    pSmoke->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx);
                    pSmoke->field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x,
                                                                field_30_pOwnSprite->field_14_xy.y,
                                                                field_30_pOwnSprite->field_1C_zpos);
                    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pSmoke->field_30_pOwnSprite);
                }
            }
            break;
    }

    // 9.6f: Fix16_Point::RotateByAngle_40F6B0 (inlined). The original calls the out-of-line
    // Add_408660/Multiply_408680/Negate_4086A0 copies for all but x * cos and y * sin
    {
        Fix16 sin = Ang16::sine_40F500(field_24_angle);
        Fix16 cos = Ang16::cosine_40F520(field_24_angle);
        Fix16 old_x = dir.x;
        dir.x = (dir.x * cos).Add_408660(dir.y * sin);
        dir.y = old_x.Negate_4086A0().Multiply_408680(sin).Add_408660(dir.y.Multiply_408680(cos));
    }
    field_14_additional_speed.x = dir.x;
    field_14_additional_speed.y = dir.y;

    Fix16 jitter_x = 0;
    jitter_y = 0;
    if (bJitter)
    {
        Fix16 rx;
        rx = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
        jitter_x = rx / 100;
        Fix16 ry;
        ry = Fix16(gRng_6F6784.get_int_4F7AE0(3) - 1);
        jitter_y = ry / 100;
    }

    field_8_speed.x = field_14_additional_speed.x + jitter_x;
    field_8_speed.y = field_14_additional_speed.y + off_y + jitter_y;
    xpos += field_8_speed.x;
    ypos += field_8_speed.y;
    gParticlePosX_6FD388 = xpos;
    gParticlePosY_6FD38C = ypos;

    if (xpos > kFP16One_6FD4A0 && xpos < kFp255_6FD280 - kFP16One_6FD4A0 && ypos > kFP16One_6FD4A0 &&
        ypos < kFp255_6FD280 - kFP16One_6FD4A0)
    {
        field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
    }
    else
    {
        field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x, field_30_pOwnSprite->field_14_xy.y, zpos);
    }

    field_30_pOwnSprite->field_2C_flags = 0xA2;
    field_30_pOwnSprite->SetNoLighting_4337F0();
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);

    if (++field_46_sub_state > 8)
    {
        field_46_sub_state = 0;
    }
    return false;
}

MATCH_FUNC(0x53a180)
bool Particle_4C::UpdateStaticAnim_state_39_53A180()
{
    gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);

    if (this->field_2C_counter == 0)
    {
        return true;
    }

    if (field_48_timer > 0)
    {
        this->field_48_timer--;
    }
    else
    {
        this->field_48_timer = 3;
        this->field_46_sub_state++;
        if (field_46_sub_state > 5u)
        {
            this->field_46_sub_state = 5;
        }
    }

    field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + this->field_46_sub_state + 191);
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);
    return false;
}

// 9.6f 0x48BEE0: the car box is read through GetCarWidth_48A930/GetCarHeight_48A950 (puts this in edi)
MATCH_FUNC(0x53a280)
bool Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280()
{
    Fix16_Point corner;

    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    // The original enters EH state 2 right after Remove_477B00: two more points, one unused
    Fix16_Point pos;
    Fix16_Point offset;

    gParticlePosX_6FD388 = field_30_pOwnSprite->field_14_xy.x;
    gParticlePosY_6FD38C = field_30_pOwnSprite->field_14_xy.y;

    if (field_28_pSprite->field_30_sprite_type_enum == sprite_types_enum::car_2)
    {
        Car* pCar = field_28_pSprite->AsCar_40FEB0();
        if (!pCar || pCar->IsDespawning_4215B0())
        {
            return true;
        }

        // The copy of the incremented state (dl) outlives the stored value (bl, reused for the angle)
        u8 sub = ++field_46_sub_state;
        if (sub == 5)
        {
            return true;
        }

        Ang16 angle = field_28_pSprite->field_0_rotation;
        Fix16 half_w;
        Fix16 half_h;
        if (sub < 4)
        {
            half_w = pCar->GetCarWidth_48A930() / 2 + kFpMinusOneThirtySecond_6FD3C0;
            half_h = pCar->GetCarHeight_48A950() / 2 + dword_6FD5A8;
        }
        else
        {
            half_w = pCar->GetCarWidth_48A930() / 2 + kFpMinusOneThirtySecond_6FD3C0;
            half_h = pCar->GetCarHeight_48A950() / 2 + kFpPoint1_6FD2E8 + kFpOne64th_6FD554;
        }

        // The rotations are written out (RotateByAngle_OOL_40F6B0 shape): with the helper in both
        // branches the inline budget runs out and the Fix16_Point ctors / unary minus go out of line.
        // The original has `corner += get_x_y` in each branch (tail-merged after the lea of its return temp),
        // and state 41's sin/cos in swapped stack slots (from `x_old = -half_w` before the corner stores).
        if (field_38_state == 40)
        {
            corner.x = half_w;
            corner.y = half_h;
            Fix16 x_old = corner.x;
            Fix16 sin = Ang16::sine_40F500(angle);
            Fix16 cos = Ang16::cosine_40F520(angle);
            corner.x = corner.x.Multiply_408680(cos).Add_408660(corner.y.Multiply_408680(sin));
            corner.y = (-x_old).Multiply_408680(sin).Add_408660(corner.y.Multiply_408680(cos));
            corner += field_28_pSprite->get_x_y();
        }
        else
        {
            Fix16 x_old = -half_w;
            corner.x = x_old;
            corner.y = half_h;
            Fix16 sin = Ang16::sine_40F500(angle);
            Fix16 cos = Ang16::cosine_40F520(angle);
            corner.x = corner.x.Multiply_408680(cos).Add_408660(corner.y.Multiply_408680(sin));
            corner.y = (-x_old).Multiply_408680(sin).Add_408660(corner.y.Multiply_408680(cos));
            corner += field_28_pSprite->get_x_y();
        }

        field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 200);
        field_30_pOwnSprite->field_2C_flags = 0xA2;
        field_30_pOwnSprite->set_ang_lazy_420690(field_28_pSprite->field_0_rotation);
        field_30_pOwnSprite->set_xyz_lazy_420600(corner.x, corner.y, field_28_pSprite->field_1C_zpos);
        field_30_pOwnSprite->SetNoLighting_4337F0();
        gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
        return false;
    }
    else
    {
        Char* pChar = field_28_pSprite->AsCharB4_40FEA0();
        if (!pChar || !pChar->field_7C_pPed || !pChar->field_7C_pPed->field_21C_bf.bActive)
        {
            return true;
        }

        if (++field_46_sub_state == 4)
        {
            return true;
        }

        if (!pChar->field_58_flags_bf.bReverseVelocity)
        {
            offset.x = -kFpPoint05_6FD46C;
            offset.y = kFpPoint3_6FD2F4;
        }
        else
        {
            offset.x = -kFpPoint05_6FD46C;
            offset.y = kFpPoint4_6FD2F8;
        }
        offset.RotateByAngle_OneMulInline_40F6B0(field_28_pSprite->field_0_rotation);
        offset = offset.Add_40AC50(pChar->field_98_velocity_vector);

        field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 197);
        field_30_pOwnSprite->field_2C_flags = 0xA2;
        field_30_pOwnSprite->set_xyz_lazy_420600(field_28_pSprite->field_14_xy.x + offset.x,
                                            field_28_pSprite->field_14_xy.y + offset.y,
                                            field_28_pSprite->field_1C_zpos);
        field_30_pOwnSprite->SetNoLighting_4337F0();
        gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
        return false;
    }
}

MATCH_FUNC(0x53ab70)
bool Particle_4C::Empty_state_42_53AB70()
{
    return false;
}

// 9.6f 0x48C270
MATCH_FUNC(0x53aba0)
bool Particle_4C::UpdateSimpleBallisticMotion_state_1_53ABA0()
{

    Fix16 z = kFpPoint05_6FD46C;
    Fix16 x = kFP16Zero_6FD49C;
    Fix16 y = kFP16Zero_6FD49C;
    Fix16_Point point2(Fix16(0), Fix16(0));
    Fix16 new_z;

    gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);

    if (!field_2C_counter)
    {
        return true;
    }

    if (field_2C_counter > this->field_2E_initial_counter >> 1)
    {
        this->field_8_speed.x = this->field_14_additional_speed.x + this->field_8_speed.x;
        this->field_8_speed.y = this->field_14_additional_speed.y + field_8_speed.y;
        new_z = field_30_pOwnSprite->field_1C_zpos + z;
    }
    else
    {
        this->field_8_speed.x = this->field_14_additional_speed.x + this->field_8_speed.x;
        this->field_8_speed.y = this->field_14_additional_speed.y + field_8_speed.y;
        new_z = field_30_pOwnSprite->field_1C_zpos - z;
    }

    gParticlePosX_6FD388 = x + field_8_speed.x + field_30_pOwnSprite->field_14_xy.x;
    gParticlePosY_6FD38C = y + field_8_speed.y + field_30_pOwnSprite->field_14_xy.y;
    if (new_z > kMaxZ_6FD28C)
    {
        new_z = kMaxZ_6FD28C;
    }

    this->field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, new_z);

    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);

    return false;
}

MATCH_FUNC(0x53ae60)
bool Particle_4C::UpdateLargeBallisticDebris_state_35_53AE60()
{
    Fix16 z_offset = kFpPoint1_6FD2E8;
    Fix16 x = kFP16Zero_6FD49C;
    Fix16 y = kFP16Zero_6FD49C;
    Fix16_Point point2(Fix16(0), Fix16(0));

    ++field_46_sub_state;
    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_46_sub_state == 15)
    {
        return true;
    }

    Fix16 zpos;
    if (field_2C_counter > (field_2E_initial_counter >> 1))
    {
        field_8_speed.x = field_8_speed.x + field_14_additional_speed.x;
        field_8_speed.y = field_8_speed.y + field_14_additional_speed.y;
        zpos = field_30_pOwnSprite->field_1C_zpos + z_offset;
    }
    else
    {
        field_8_speed.x = field_8_speed.x + field_14_additional_speed.x;
        field_8_speed.y = field_8_speed.y + field_14_additional_speed.y;
        zpos = field_30_pOwnSprite->field_1C_zpos - z_offset;
    }
    gParticlePosX_6FD388 = field_30_pOwnSprite->field_14_xy.x + field_8_speed.x + x;
    gParticlePosY_6FD38C = field_30_pOwnSprite->field_14_xy.y + field_8_speed.y + y;

    if (zpos > kMaxZ_6FD28C)
    {
        zpos = kMaxZ_6FD28C;
    }
    field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 131);
    field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, zpos);
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
    field_30_pOwnSprite->field_2C_flags = 0xA2;
    return false;
}

MATCH_FUNC(0x53b170)
bool Particle_4C::Empty_state_44_53B170()
{
    return false;
}

// https://decomp.me/scratch/fW2BZ
MATCH_FUNC(0x53b1a0)
bool Particle_4C::UpdateDebrisArc_state_7_53B1A0()
{
    Fix16 z_offset = kFpPoint05_6FD46C;
    Fix16 x = kFP16Zero_6FD49C;
    Fix16 y = kFP16Zero_6FD49C;
    Fix16_Point point2(Fix16(0), Fix16(0));

    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (!field_2C_counter)
    {
        return true;
    }

    Fix16 zpos;
    if (field_2C_counter > (field_2E_initial_counter >> 1))
    {
        field_8_speed.x = field_8_speed.x + field_14_additional_speed.x;
        field_8_speed.y = field_8_speed.y + field_14_additional_speed.y;
        zpos = field_30_pOwnSprite->field_1C_zpos + z_offset;
    }
    else
    {
        field_8_speed.x = field_8_speed.x + field_14_additional_speed.x;
        field_8_speed.y = field_8_speed.y + field_14_additional_speed.y;
        zpos = field_30_pOwnSprite->field_1C_zpos - z_offset;
    }
    gParticlePosX_6FD388 = x + field_8_speed.x + field_30_pOwnSprite->field_14_xy.x;
    gParticlePosY_6FD38C = y + field_8_speed.y + field_30_pOwnSprite->field_14_xy.y;

    switch (field_2C_counter)
    {
        case 5:
        case 6:
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 127);
            break;
        case 4:
            field_30_pOwnSprite->set_id_4206E0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 128);
            break;
        case 2:
        case 3:
            field_30_pOwnSprite->set_id_4206E0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 129);
            break;
        case 0:
        case 1:
            field_30_pOwnSprite->set_id_4206E0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 130);
            break;
        default:
            break;
    }
    if (zpos > kMaxZ_6FD28C)
    {
        zpos = kMaxZ_6FD28C;
    }
    field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, zpos);
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
    return false;
}

// 9.6f 0x48C790
MATCH_FUNC(0x53b580)
bool Particle_4C::UpdateShortAnim_state_37_53B580()
{
    gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);

    field_46_sub_state++;

    if (field_46_sub_state == 5)
    {
        return true;
    }

    switch (field_46_sub_state)
    {
        case 1:
            this->field_30_pOwnSprite->set_id_lazy_4206C0(field_30_pOwnSprite->field_22_type_sprite_idx);
            break;

        default:
            this->field_30_pOwnSprite->set_id_4206E0(field_30_pOwnSprite->field_22_type_sprite_idx + 4);
            break;
    }

    this->field_30_pOwnSprite->SetNoLighting_4337F0();
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);
    return false;
}

MATCH_FUNC(0x53b670)
bool Particle_4C::UpdateAttachedEmitter_state_9_10_53B670()
{
    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_2C_counter == 0)
    {
        return true;
    }

    gParticlePosX_6FD388 = field_30_pOwnSprite->field_14_xy.x;
    gParticlePosY_6FD38C = field_30_pOwnSprite->field_14_xy.y;

    Sprite* pSprite = field_28_pSprite;
    Fix16 zpos = field_30_pOwnSprite->field_1C_zpos;
    Fix16_Point_POD offset;
    if (pSprite->GetType_416B40() != sprite_types_enum::ped_3)
    {
        return true;
    }

    Char* pChar = pSprite->field_8_char_b4_ptr;

    Ped* pPed = pChar->get_ped_433A20();
    if (!pPed || !pPed->IsActive_433B40())
    {
        return true;
    }

    if (field_38_state == 9)
    {
        if (pChar->field_6C_animation_state != 2)
        {
            return true;
        }
        field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 3);
        if (pChar->field_68_animation_frame != 5)
        {
            gParticle_8_6FD5E8->SpawnCigaretteSmokePuff_5406B0(field_28_pSprite, 0);
        }
    }
    else
    {
        if (field_2C_counter < 60)
        {
            if (field_2C_counter > 40)
            {
                // As in 9.6f (0x48FE90): the sum is an operator+ temporary passed straight to PolarToCartesian,
                // which puts it at the original's slot (0x12). `radius` assigned, not initialised: the extra
                // size gives PolarToCartesian's first operator* enough nested inline budget.
                Ang16 jitter;
                jitter = Ang16::Fix16_To_Ang16_40F540(Fix16((gRng_6F6784.get_int_4F7AE0(8) - 4) / 2));
                {
                    Fix16 radius;
                    radius = kFpOne1024th_6FD540 * kFpThree_6FD4A8;
                    Ang16::PolarToCartesian_41FC20(field_28_pSprite->field_0_rotation + jitter, radius, offset.x, offset.y);
                }
                field_30_pOwnSprite->SetBlendAndAlpha_4337D0(1, 10);
                field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 17);
                zpos += kFpPoint04_6FD470;
            }
            else
            {
                Fix16 radius = kFP16One_6FD4A0 * kFpOne1024th_6FD540;
                Ang16::PolarToCartesian_41FC20(field_30_pOwnSprite->field_0_rotation, radius, offset.x, offset.y);
                field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 17);
                field_30_pOwnSprite->SetBlendAndAlpha_4337D0(1, 5);
                zpos += kFpPoint04_6FD470;
            }
        }
        else
        {
            Fix16 radius = kFpOne1024th_6FD540 * kFpThree_6FD4A8;
            Ang16::PolarToCartesian_41FC20(field_28_pSprite->field_0_rotation, radius, offset.x, offset.y);
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(1, 20);
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 17);
        }
        gParticlePosX_6FD388 += offset.x;
        gParticlePosY_6FD38C += offset.y;
    }

    if (zpos > kMaxZ_6FD28C)
    {
        zpos = kMaxZ_6FD28C;
    }
    field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, zpos);
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
    return false;
}

MATCH_FUNC(0x53b9f0)
bool Particle_4C::UpdateBurstAnimation_state_29_30_53B9F0()
{
    Fix16 scale_related;
    u8 idx;

    ++field_46_sub_state;
    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_46_sub_state > 25)
    {
        return true;
    }

    if (field_46_sub_state < 20)
    {
        if (field_46_sub_state < 17)
        {
            if (field_46_sub_state < 12)
            {
                scale_related = kFpPoint01_6FD45C;
                idx = 0;
            }
            else
            {
                scale_related = kFpPoint005_6FD564;
                idx = 1;
            }
        }
        else
        {
            scale_related = kFpPoint005_6FD564;
            idx = 2;
        }
    }
    else
    {
        scale_related = kFpPoint003_6FD538;
        idx = 3;
    }

    field_30_pOwnSprite->set_id_4206E0(idx + gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 37);
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
    field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 10);
    field_30_pOwnSprite->SetNoLighting_4337F0();
    field_30_pOwnSprite->ApplyScaleToDimensions_59E4C0(kFP16One_6FD4A0 + scale_related * Fix16(field_46_sub_state), 0);
    return false;
}

MATCH_FUNC(0x53bac0)
bool Particle_4C::UpdateCollisionBurst_state_31_34_53BAC0()
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
    gSpriteGrid_3_679210->Remove_477B00(field_30_pOwnSprite);
    if (field_2C_counter == 0)
    {
        return true;
    }

    xpos = gParticlePosX_6FD388 = field_30_pOwnSprite->field_14_xy.x;
    ypos = gParticlePosY_6FD38C = field_30_pOwnSprite->field_14_xy.y;
    zpos = field_30_pOwnSprite->field_1C_zpos;

    Sprite* pTarget = field_28_pSprite;
    switch (pTarget->field_30_sprite_type_enum)
    {
        case sprite_types_enum::car_2:
            pCar = pTarget->field_8_car_bc_ptr;
            if (!pCar || !pCar->field_54_driver)
            {
                return true;
            }
            if (!pCar->field_54_driver->field_21C_bf.bAttacking)
            {
                field_4_bf.bDriverAttacking = false;
            }
            if (!pCar->field_58_physics)
            {
                return true;
            }
            break;

        default:
            return true;

        case sprite_types_enum::ped_3:
            pChar = pTarget->field_8_char_b4_ptr;
            if (!pChar || !pChar->field_7C_pPed || !pChar->field_7C_pPed->IsActive_433B40())
            {
                return true;
            }
            if (!pChar->field_7C_pPed->field_21C_bf.bAttacking)
            {
                field_4_bf.bDriverAttacking = false;
            }
            angle = pTarget->field_0_rotation;
            break;
    }

    if (field_48_timer > 0)
    {
        --field_48_timer;
    }
    else
    {
        if (++field_46_sub_state > 15)
        {
            return true;
        }
        if (field_46_sub_state > 3)
        {
            field_48_timer = 1;
        }
    }

    spread = Ang16::Fix16_To_Ang16_40F540(kFpOne64th_6FD328 * (Fix16(field_46_sub_state) + Fix16(gRng_6F6784.get_int_4F7AE0(static_cast<s16>(field_46_sub_state))) - kFpFour_6FD4AC));

    if (pChar)
    {
        vel = pChar->field_7C_pPed->GetVelocityVector_45B520();
    }
    else
    {
        Ang16 car_angle;
        Fix16_Point offset;
        Fix16_Point attach;
        AttachedSprite* pGun = pCar->field_0_attachments.GetSpriteForModel_5A6A50(objects::water_cannon_114);
        if (pGun)
        {
            car_angle = pGun->field_0_pSprite->field_0_rotation + kAng180_6FD3EE;
            attach.x = 0;
            attach.y = kFpSevenThirtySeconds_6FD2D4;
            attach.RotateByAngle_40F6B0(car_angle);
            offset.SetXY_432860(Fix16(0), kFP16Eighth_6FD2D0);
        }
        else
        {
            car_angle = pCar->field_0_attachments.GetSpriteForModel_5A6A50(248)->field_0_pSprite->field_0_rotation;
            attach.x = 0;
            attach.y = kFpThreeThirtySeconds_6FD48C;
            attach.RotateByAngle_40F6B0(car_angle);
            offset.SetXY_432860(Fix16(0), kFpMinusNineThirtySeconds_6FD4CC);
        }
        offset.RotateByAngle_40F6B0(pCar->field_50_car_sprite->field_0_rotation);
        attach += offset.Add_40AC50(pCar->field_50_car_sprite->get_x_y());
        vel = pCar->field_58_physics->GetPointVelocity_561350(&attach);
        angle = car_angle;
    }

    vel.GetLength_41E260();

    if ((field_4_bf.bDriverAttacking) && field_46_sub_state <= 12)
    {
        Ang16::PolarToCartesian_41FC20(spread + angle, kFpPoint1_6FD2E8, dx, dy);
    }
    else
    {
        Ang16::PolarToCartesian_41FC20(field_30_pOwnSprite->field_0_rotation, kFpPoint1_6FD2E8, dx, dy);
    }
    gParticlePosX_6FD388 += vel.x + dx;
    gParticlePosY_6FD38C += vel.y + dy;

    u8 max_sub_state;
    if (field_38_state == 31)
    {
        field_30_pOwnSprite->set_id_lazy_4206C0(field_46_sub_state + gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 73);
        max_sub_state = 14;
    }
    else
    {
        field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + field_46_sub_state + 111);
        max_sub_state = 12;
    }
    field_30_pOwnSprite->field_2C_flags = 0xA2;
    field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, zpos);

    if (field_46_sub_state > 1)
    {
        if (field_46_sub_state < max_sub_state)
        {
            Sprite* pHit = field_30_pOwnSprite->QuerySpriteCollision_59E7D0(2);
            if (pHit && pHit != field_28_pSprite)
            {
                if (field_38_state == 31)
                {
                    field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos + kFpPoint01_6FD45C);
                    if (field_46_sub_state < 10)
                    {
                        field_46_sub_state = 10;
                    }
                    gParticle_8_6FD5E8->field_0_fire_hit_obj->field_4_sprite->set_xyz_lazy_420600(xpos, ypos, zpos);
                    if (pChar)
                    {
                        gParticle_8_6FD5E8->field_0_fire_hit_obj->SetDamageOwner_529080(pChar->field_7C_pPed->get_ped_ref_idx_420B50());
                    }
                    else if (pCar)
                    {
                        gParticle_8_6FD5E8->field_0_fire_hit_obj->SetDamageOwner_529080(pCar->field_54_driver->get_ped_ref_idx_420B50());
                    }
                    if (!pHit->IsObject2CType_40FE80())
                    {
                        gParticle_8_6FD5E8->field_0_fire_hit_obj->HandleObjectHit_528990(pHit);
                    }
                }
                else
                {
                    field_46_sub_state = max_sub_state;
                    field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos + kFpPoint01_6FD45C);
                    if (!(gpRng_67AB34->get_cur_rng_41CFE0() & 1))
                    {
                        gParticle_8_6FD5E8->EmitWaterSplash_53F060(xpos, ypos, zpos, angle, false);
                    }
                    gParticle_8_6FD5E8->field_4_water_hit_obj->field_4_sprite->set_xyz_lazy_420600(xpos, ypos, zpos);
                    if (pChar)
                    {
                        gParticle_8_6FD5E8->field_4_water_hit_obj->SetDamageOwner_529080(pChar->field_7C_pPed->get_ped_ref_idx_420B50());
                    }
                    else if (pCar)
                    {
                        gParticle_8_6FD5E8->field_4_water_hit_obj->SetDamageOwner_529080(pCar->field_54_driver->get_ped_ref_idx_420B50());
                    }
                    if (!pHit->IsObject2CType_40FE80())
                    {
                        gParticle_8_6FD5E8->field_4_water_hit_obj->HandleObjectHit_528990(pHit);
                    }
                    if (pHit->GetType_416B40() == sprite_types_enum::ped_3)
                    {
                        pHit->field_8_char_b4_ptr->HandleGenericImpact_553E00(angle, kFpPoint1_6FD2E8 + kFpPoint05_6FD46C, kFP16Zero_6FD49C, 0);
                        Ped* pHitPed = pHit->field_8_char_b4_ptr->field_7C_pPed;
                        if (pHitPed)
                        {
                            if (field_38_state == 31)
                            {
                                gScriptManager_6F8060->RecordWeaponHit_512C00(pHitPed->field_200_id, 194, 1);
                            }
                            else
                            {
                                gScriptManager_6F8060->RecordWeaponHit_512C00(pHitPed->field_200_id, 198, 1);
                            }
                        }
                    }
                }
            }
        }
    }

    if (field_30_pOwnSprite->CheckSpriteMovementRegion_5A2500())
    {
        field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos + kFpPoint01_6FD45C);
        if (field_46_sub_state < 10)
        {
            field_46_sub_state = 10;
        }
    }
    gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_30_pOwnSprite);
    return false;
}

// The frame-delay store is written twice (two if branches): VC6 counts the uses of the constant 1 before
// merging the identical branches, and keeps it in bl for the stores and the return 1.
MATCH_FUNC(0x53d260)
bool Particle_4C::PoolUpdate()
{
    s32 state = this->field_38_state;
    --this->field_2C_counter;
    switch (state - 1)
    {
        case 17:
        case 32:
            if (field_48_timer > 0)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u)
                {
                    this->field_48_timer = 1;
                }
                else if (field_46_sub_state > 10u)
                {
                    this->field_48_timer = 1;
                }

                gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);
                if (field_46_sub_state > 15u)
                {
                    return true;
                }

                field_30_pOwnSprite->set_id_lazy_4206C0(field_46_sub_state + gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 20);
                if (field_30_pOwnSprite->field_1C_zpos + kFpPoint2_6FD2F0 < kMaxZ_6FD28C)
                {
                    field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x,
                                                        field_30_pOwnSprite->field_14_xy.y,
                                                        field_30_pOwnSprite->field_1C_zpos + kFpPoint1_6FD2E8);
                }

                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);
                if (this->field_46_sub_state == 2)
                {
                    this->field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 20);
                }
                this->field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
                return false;
            }
            break;

        case 18:
        case 31:
            if (field_48_timer > 0)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u)
                {
                    this->field_48_timer = 1;
                }
                else if (field_46_sub_state > 10u)
                {
                    this->field_48_timer = 1;
                }

                gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);
                if (field_46_sub_state > 15u)
                {
                    return true;
                }

                field_30_pOwnSprite->set_id_lazy_4206C0(field_46_sub_state + gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 20);

                if (field_30_pOwnSprite->field_1C_zpos + kFpPoint2_6FD2F0 < kMaxZ_6FD28C)
                {
                    field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x, field_30_pOwnSprite->field_14_xy.y, field_30_pOwnSprite->field_1C_zpos + kFpPoint1_6FD2E8);
                }

                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);
                if (this->field_46_sub_state == 2)
                {
                    this->field_30_pOwnSprite->field_2C_flags = 0xA2;
                }
                field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
                field_30_pOwnSprite->ApplyScaleToDimensions_59E4C0(kFP16One_6FD4A0 + kFP16Half_6FD39C, 0);
                return false;
            }
            break;

        case 19:
            if (field_48_timer > 0)
            {
                this->field_48_timer--;
                return false;
            }
            else
            {
                this->field_46_sub_state++;
                if (field_46_sub_state < 6u)
                {
                    this->field_48_timer = 1;
                }
                else if (field_46_sub_state > 10u)
                {
                    this->field_48_timer = 1;
                }

                gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);
                if (field_46_sub_state > 15u)
                {
                    return true;
                }

                field_30_pOwnSprite->set_id_lazy_4206C0(field_46_sub_state + gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 56);

                if (field_30_pOwnSprite->field_1C_zpos + kFpPoint2_6FD2F0 < kMaxZ_6FD28C)
                {
                    field_30_pOwnSprite->set_xyz_lazy_420600(field_30_pOwnSprite->field_14_xy.x,
                                                        field_30_pOwnSprite->field_14_xy.y,
                                                        field_30_pOwnSprite->field_1C_zpos + kFpPoint1_6FD2E8);
                }

                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);
                if (this->field_46_sub_state == 2)
                {
                    this->field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 20);
                }

                field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
                field_30_pOwnSprite->ApplyScaleToDimensions_59E4C0(kFP16Two_6FD4A4, 0);
                return false;
            }
            break;

        case 21:
        case 22:
        case 23:
        case 24:
            if (field_48_timer > 0)
            {
                this->field_48_timer--;
                return false;
            }

            gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);
            this->field_48_timer = 1;
            this->field_46_sub_state++;
            if (field_46_sub_state > 0xCu)
            {
                return true;
            }

            field_30_pOwnSprite->set_id_lazy_4206C0(field_46_sub_state + gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 147);

            gParticlePosX_6FD388 = this->field_20_speed * Ang16::sine_40F500(this->field_24_angle);
            gParticlePosY_6FD38C = this->field_20_speed * Ang16::cosine_40F520(this->field_24_angle);

            gParticlePosX_6FD388 = this->field_30_pOwnSprite->field_14_xy.x + gParticlePosX_6FD388;
            gParticlePosY_6FD38C = this->field_30_pOwnSprite->field_14_xy.y + gParticlePosY_6FD38C;


            if (this->field_46_sub_state < 9u)
            {
                field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, field_30_pOwnSprite->field_1C_zpos + kFpPoint1_6FD2E8);
            }
            else
            {
                field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, field_30_pOwnSprite->field_1C_zpos + kFpPoint05_6FD46C);
            }

            if (field_30_pOwnSprite->field_1C_zpos > kMaxZ_6FD28C)
            {
                field_30_pOwnSprite->set_z_lazy_420660(kMaxZ_6FD28C);
            }

            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);
            field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 10);
            field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
            return false;

        case 5:
        case 14:
        case 15:
        case 16:
            return UpdateFloatingParticle_state_6_15_16_17_538060();

        case 2:
        case 11:
            return UpdateDirectedProjectile_state_3_12_5384C0();

        case 12:
        case 13:
        case 35:
            return UpdateDirectedBurst_state_13_14_36_539480();

        case 3:
            return UpdateDirectedBurstSweep_state_4_539040();

        case 4:
            return UpdateCircularBurst_state_5_539890();

        case 6:
            return UpdateDebrisArc_state_7_53B1A0();

        case 8:
        case 9:
            return UpdateAttachedEmitter_state_9_10_53B670();

        case 0:
            return UpdateSimpleBallisticMotion_state_1_53ABA0();

        case 36:
            return UpdateShortAnim_state_37_53B580();

        case 34:
            return UpdateLargeBallisticDebris_state_35_53AE60();

        case 7:
            return true;

        case 25:
            gSpriteGrid_3_679210->Remove_477B00(this->field_30_pOwnSprite);
            if (!this->field_2C_counter)
            {
                return true;
            }
            field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx);
            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(this->field_30_pOwnSprite);
            field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
            return false;

        case 28:
        case 29:
            return UpdateBurstAnimation_state_29_30_53B9F0();

        case 30:
        case 33:
            return UpdateCollisionBurst_state_31_34_53BAC0();

        case 37:
            return UpdateObjectBeamLink_state_38_538AC0();

        case 38:
            return UpdateStaticAnim_state_39_53A180();

        case 39:
        case 40:
            return UpdateSkidOrScrapeSpark_state_40_41_53A280();

        case 41:
            return Empty_state_42_53AB70();

        case 42:
            return UpdateBeamSegment_state_43_538A40();

        case 43:
            return Empty_state_44_53B170();

        default:
            return false;
    }
}

MATCH_FUNC(0x53e2c0)
void Particle_4C::PoolAllocate()
{
    field_8_speed.x = 0;
    field_8_speed.y = 0;
    field_8_speed.z = 0;
    field_40_pExplosion = NULL;
    field_4_bf.bDriverAttacking = false;
}

MATCH_FUNC(0x53e2e0)
void Particle_4C::PoolDeallocate()
{
    if (field_30_pOwnSprite)
    {
        field_30_pOwnSprite->ClearNoLighting_433800();
        field_30_pOwnSprite->SetBlendAndAlpha_4337D0(0, 0);
        gSpritePool_703818->remove(field_30_pOwnSprite);
        field_30_pOwnSprite = NULL;
    }
}
