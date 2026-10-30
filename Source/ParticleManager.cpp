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
EXTERN_GLOBAL(Fix16, gParticlePosX_6FD388);
EXTERN_GLOBAL(Fix16, gParticlePosY_6FD38C);
EXTERN_GLOBAL(Fix16, kFpOne512th_6FD548);
EXTERN_GLOBAL(Fix16, kFpOneThirtySecond_6FD330);
EXTERN_GLOBAL(Fix16, kFpOne64th_6FD448);
EXTERN_GLOBAL(Fix16, kFpPoint1_6FD2E8);
EXTERN_GLOBAL(Fix16, kFP16Quarter_6FD2EC);
EXTERN_GLOBAL(Fix16, kFpOne64th_6FD554);
EXTERN_GLOBAL(Fix16, kFpMinusOneThirtySecond_6FD3C0);
EXTERN_GLOBAL(Fix16, kFpPoint2PlusOne64th_6FD5A8);

EXTERN_GLOBAL(Ang16, kAngZero_6FD5D4);
EXTERN_GLOBAL(Ang16, kAng180_6FD3EE);

DEFINE_GLOBAL(T_Particle_4C_Pool*, gParticle_4C_Pool_6FD5E4, 0x6FD5E4);
DEFINE_GLOBAL(ParticleManager*, gParticle_8_6FD5E8, 0x6FD5E8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint07_6FD474, Fix16(0x47A, 0), 0x6FD474);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint02_6FD468, Fix16(0x147, 0), 0x6FD468);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD314, Ang16(360), 0x6FD314);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint0007_6FD4EC, Fix16(0xB, 0), 0x6FD4EC);
DEFINE_GLOBAL_INIT(Fix16, kFpFifty_6FD558, Fix16(50), 0x6FD558);
DEFINE_GLOBAL_INIT(Ang16, kAng1_6FD5CC, Ang16(4), 0x6FD5CC);
DEFINE_GLOBAL_INIT(Fix16, kFpTen_6FD508, Fix16(10), 0x6FD508);

DEFINE_GLOBAL_INIT(Fix16, kFpSevenThirtySeconds_6FD2D4, Fix16(0xE00, 0), 0x6FD2D4);
DEFINE_GLOBAL_INIT(Fix16, kFpMinusNineThirtySeconds_6FD4CC, Fix16(0xFFFFEE00, 0), 0x6FD4CC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eighth_6FD2D0, Fix16(0x800, 0), 0x6FD2D0);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeThirtySeconds_6FD48C, Fix16(0x600, 0), 0x6FD48C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint03_6FD464, Fix16(0x1EB, 0), 0x6FD464);

// NOTE: Will not match in marked extern !!
DEFINE_GLOBAL(u16, gParticleInstCount_6FD5F4, 0x6FD5F4);

MATCH_FUNC(0x53e320)
void ParticleManager::ParticlesService_53E320()
{
    gParticle_4C_Pool_6FD5E4->UpdatePool();
}

MATCH_FUNC(0x53E3C0)
Particle_4C* ParticleManager::New_53E3C0(Fix16 speed_x, Fix16 speed_y, Fix16 speed_z, Fix16 additional_speed_x, Fix16 additional_speed_y, Fix16 additional_speed_z)
{
    Particle_4C* pNewParticle = NULL;
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0() && gSpritePool_703818->has_free_48A8D0())
    {
        pNewParticle = gParticle_4C_Pool_6FD5E4->Allocate();
        pNewParticle->field_8_speed.x = speed_x;
        pNewParticle->field_8_speed.y = speed_y;
        pNewParticle->field_8_speed.z = speed_z;
        pNewParticle->field_14_additional_speed.x = additional_speed_x;
        pNewParticle->field_14_additional_speed.y = additional_speed_y;
        pNewParticle->field_14_additional_speed.z = additional_speed_z;
        pNewParticle->field_0_id = gParticleInstCount_6FD5F4;
        pNewParticle->field_30_pOwnSprite = gSpritePool_703818->get_new_sprite();
        ++gParticleInstCount_6FD5F4;
    }
    return pNewParticle;
}

// Ang16::operator- (9.6f 0x40E5D0) with the normalizing ctor called out of line (AssignNormalized_409300)
static inline Ang16 SubAng16_ool(const Ang16& a, const Ang16& b)
{
    s16 value = a.rValue - b.rValue;
    return Ang16(&value, 0);
}

// RotateByAngle_40F6B0 with the x_old negate as the named out-of-line Negate_4086A0: the rotations and the
// -(v / 15) New_53E3C0 arguments have to reach the same function (the COMDAT copy of operator- and
// Negate_4086A0 are not folded in our build). The other operators stay the plain inlines.
static inline void RotateNegExport_(Fix16_Point& v, const Ang16& angle)
{
    Fix16 sin = Ang16::sine_40F500(angle);
    Fix16 cos = Ang16::cosine_40F520(angle);
    Fix16 x_old = v.x;
    v.x = (v.x * cos) + (v.y * sin);
    v.y = (x_old.Negate_4086A0() * sin) + (v.y * cos);
}

// 9.6f 0x48C9C0: plain members and operators. The do-while count-down loop (smaller than the for form) and
// the unary minus on the divides (never inlined: it builds a by-value argument, so it adds two sites) set
// the inline budget: + out of line and the negate inline in the first rotation, the multiplies out of line
// in MultiplyByFix16_401CB0, the ctor out of line in Ang16::operator-.
MATCH_FUNC(0x53E450)
void ParticleManager::EmitBloodBurst_53E450(Fix16 x, Fix16 y, Fix16 z, Ang16 ang)
{
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));

    if (!bSkip_particles_67D64D)
    {
        vector.x = Fix16(0);
        vector.y = Fix16(gRng_6F6784.get_int_4F7AE0(50) + 25) * kFpOne512th_6FD548;
        vector.RotateByAngle_40F6B0(ang);

        s32 count = 6;
        do
        {
            vector.x = Fix16(0);
            vector.y = (Fix16(gRng_6F6784.get_int_4F7AE0(100)) + kFpFifty_6FD558) * kFpPoint0007_6FD4EC;

            angle = kAng1_6FD5CC.MultiplyByFix16_401CB0(Fix16(gRng_6F6784.get_int_4F7AE0(16)));
            vector.RotateByAngle_40F6B0((angle + ang) - kAng1_6FD5CC.MultiplyByFix16_401CB0(Fix16(8)));

            Particle_4C* pBloodParticle =
                gParticle_8_6FD5E8->New_53E3C0(vector.x, vector.y, kFpOneThirtySecond_6FD330, -vector.x.DivideInt_53E860(15), -vector.y.DivideInt_53E860(15), 0);

            if (pBloodParticle)
            {
                pBloodParticle->field_34_unused = 1;
                pBloodParticle->field_38_state = 1;
                pBloodParticle->field_2C_counter = 15;
                pBloodParticle->field_2E_initial_counter = 15;

                pBloodParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pBloodParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 16);
                pBloodParticle->field_30_pOwnSprite->set_xyz_lazy_420600(x, y, z);
                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pBloodParticle->field_30_pOwnSprite);
            }
        } while (--count);
    }
}

// 9.6f 0x48CC50
MATCH_FUNC(0x53e880)
void ParticleManager::SpawnBlood_53E880(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Particle_4C* pNewParticle = gParticle_8_6FD5E8->New_53E3C0(xpos, ypos, 0, 0, 0, 0);
    if (pNewParticle)
    {
        pNewParticle->field_34_unused = 1;
        pNewParticle->field_38_state = 39;
        pNewParticle->field_2C_counter = 800;
        pNewParticle->field_46_sub_state = 0;
        pNewParticle->field_48_timer = 3;
        pNewParticle->field_2E_initial_counter = 800;
        pNewParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
        pNewParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 191);
        pNewParticle->field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
        pNewParticle->field_30_pOwnSprite->SetDrawLayer_40F7B0(2);
        gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pNewParticle->field_30_pOwnSprite);
    }
}

MATCH_FUNC(0x53e970)
void ParticleManager::GunMuzzelFlash_53E970(Sprite* pSprite)
{
    // pos, offset, point_velocity and corner_3 are unused but give the original's EH states
    Fix16_Point vel(0, 0);
    Fix16_Point pos;
    Fix16_Point offset;
    Fix16_Point ped_offset;
    Fix16_Point point_velocity;
    Fix16 dx;
    Fix16 dy;
    if (!bSkip_particles_67D64D)
    {
        if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car_2)
        {
            Ang16 angle;
            Fix16_Point corner_1;
            Fix16_Point corner_2;
            Fix16_Point corner_3;
            Car* pCar = pSprite->field_8_car_bc_ptr;
            Fix16 half_w;
            Fix16 half_h;
            Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
            if (pParticle)
            {
                pParticle->field_4_bf.bDriverAttacking = true;
                pParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 197);
                pParticle->field_34_unused = 0;
                pParticle->field_38_state = 40;
                Ang16::PolarToCartesian_41FC20(pSprite->field_0_rotation, kFpPoint1_6FD2E8, dx, dy);
                pParticle->field_46_sub_state = 0;
                pParticle->field_48_timer = 0;

                half_w = pCar->GetCarWidth_48A930() / 2 + kFpMinusOneThirtySecond_6FD3C0;
                half_h = pCar->GetCarHeight_48A950() / 2 + kFpPoint2PlusOne64th_6FD5A8;
                corner_1.x = half_w;
                corner_1.y = half_h;
                angle = pSprite->field_0_rotation;
                corner_1.RotateByAngle_40F6B0(angle);
                corner_1 += pSprite->get_x_y_443580();

                pParticle->field_28_pSprite = pSprite;
                pParticle->field_30_pOwnSprite->set_ang_lazy_420690(pSprite->field_0_rotation);
                pParticle->field_30_pOwnSprite->set_xyz_lazy_420600(corner_1.x, corner_1.y, pSprite->field_1C_zpos);
                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pOwnSprite);
                pParticle->field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
            }

            pParticle = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
            if (pParticle)
            {
                pParticle->field_4_bf.bDriverAttacking = true;
                pParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 197);
                pParticle->field_34_unused = 0;
                pParticle->field_38_state = 41;
                Ang16::PolarToCartesian_41FC20(pSprite->field_0_rotation, kFpPoint1_6FD2E8, dx, dy);
                pParticle->field_46_sub_state = 0;
                pParticle->field_48_timer = 0;

                angle = pSprite->field_0_rotation;
                half_w = pCar->GetCarWidth_48A930() / 2 + kFpMinusOneThirtySecond_6FD3C0;
                half_h = pCar->GetCarHeight_48A950() / 2 + kFpPoint2PlusOne64th_6FD5A8;
                corner_2.SetXY_432860(-half_w, half_h);
                corner_2.RotateByAngle_40F6B0(angle);
                corner_2 += pSprite->get_x_y_443580();

                pParticle->field_30_pOwnSprite->set_ang_lazy_420690(pSprite->field_0_rotation);
                pParticle->field_30_pOwnSprite->set_xyz_lazy_420600(corner_2.x, corner_2.y, pSprite->field_1C_zpos);
                pParticle->field_28_pSprite = pSprite;
                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pOwnSprite);
                pParticle->field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
            }
        }
        else
        {
            Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
            if (pParticle)
            {
                pParticle->field_4_bf.bDriverAttacking = true;
                pParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 197);
                pParticle->field_34_unused = 0;
                pParticle->field_38_state = 40;
                Ang16::PolarToCartesian_41FC20(pSprite->field_0_rotation, kFpPoint1_6FD2E8, dx, dy);
                pParticle->field_46_sub_state = 0;
                pParticle->field_48_timer = 0;
                gParticlePosX_6FD388 = pSprite->field_14_xy.x + dx;
                gParticlePosY_6FD38C = pSprite->field_14_xy.y + dy;
                Fix16 zpos = pSprite->field_1C_zpos;

                Char* pChar = pSprite->AsCharB4_40FEA0();
                ped_offset.x = -kFpPoint03_6FD464;
                ped_offset.y = kFpPoint02_6FD468 + kFpPoint1_6FD2E8;
                ped_offset.RotateByAngle_40F6B0(pSprite->field_0_rotation);
                ped_offset = ped_offset + *reinterpret_cast<Fix16_Point*>(&pChar->field_98_velocity_vector);

                pParticle->field_30_pOwnSprite->set_ang_lazy_420690(pSprite->field_0_rotation);
                pParticle->field_30_pOwnSprite->set_xyz_lazy_420600(pSprite->field_14_xy.x + ped_offset.x, pSprite->field_14_xy.y + ped_offset.y, zpos);
                pParticle->field_28_pSprite = pSprite;
                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pOwnSprite);
                pParticle->field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
            }
        }
    }
}

// https://decomp.me/scratch/2Kz9I
// 9.6f 0x48D1F0 (unpaired before): plain members (MultiplyByFix16_401CB0, +, -, RotateByAngle_40F6B0 as
// RotateNegExport_), a count-down loop, -(v / 15) through Negate_4086A0.
MATCH_FUNC(0x53f060)
void ParticleManager::EmitWaterSplash_53F060(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, bool bRandomRot)
{
    Ang16 angle_2;
    Fix16_Point velocity(Fix16(0), Fix16(0));

    if (!bSkip_particles_67D64D)
    {
        velocity.x = Fix16(0);
        velocity.y = Fix16(gRng_6F6784.get_int_4F7AE0(50) + 25) * kFpOne512th_6FD548;
        RotateNegExport_(velocity, rotation);

        for (s32 count = 6; count != 0; count--)
        {
            if (bRandomRot)
            {
                angle_2 = kAng1_6FD5CC.MultiplyByFix16_401CB0(Fix16(gRng_6F6784.get_int_4F7AE0(360)));
            }
            else
            {
                angle_2 = rotation;
            }

            velocity.x = Fix16(0);
            velocity.y = (Fix16(gRng_6F6784.get_int_4F7AE0(100)) + kFpFifty_6FD558) * kFpPoint0007_6FD4EC;

            Ang16 angle_1 = kAng1_6FD5CC.MultiplyByFix16_401CB0(Fix16(gRng_6F6784.get_int_4F7AE0(16)));
            RotateNegExport_(velocity, (angle_1 + angle_2) - kAng1_6FD5CC.MultiplyByFix16_401CB0(Fix16(8)));

            // last arg: plain 0 (the original builds this Fix16 arg in place; Fix16(0) gives a plain push and costs ebp)
            Particle_4C* pWaterSplashParticle = gParticle_8_6FD5E8->New_53E3C0(velocity.x,
                                                                               velocity.y,
                                                                               kFpOneThirtySecond_6FD330,
                                                                               velocity.x.DivideInt_53E860(15).Negate_4086A0(),
                                                                               velocity.y.DivideInt_53E860(15).Negate_4086A0(),
                                                                               0);

            if (pWaterSplashParticle)
            {
                pWaterSplashParticle->field_34_unused = 0;
                pWaterSplashParticle->field_38_state = 35;
                pWaterSplashParticle->field_2C_counter = 15;
                pWaterSplashParticle->field_2E_initial_counter = 15;

                pWaterSplashParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pWaterSplashParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 132);
                pWaterSplashParticle->field_46_sub_state = 0;
                pWaterSplashParticle->field_48_timer = 6;
                pWaterSplashParticle->field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pWaterSplashParticle->field_30_pOwnSprite);
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
void ParticleManager::EmitFlameStreamSegment_53F4C0(Sprite* pSprt)
{
    {
        Fix16_Point vel(0, 0);
        Fix16_Point pos;
        Fix16_Point offset;
        Fix16_Point ped_offset;
        // point_velocity only receives the unused result of GetPointVelocity_561350 (9.6f passed it
        // to set_xyz_lazy instead of pos).
        Fix16_Point point_velocity;
        Ang16 angle;
        if (!bSkip_particles_67D64D)
        {
            if (!field_0_fire_hit_obj)
            {
                field_0_fire_hit_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::fire_hitting_194, 0, 0, 0, kAngZero_6FD5D4);
            }
            vel = Fix16_Point(0, 0);
            Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
            if (pParticle)
            {
                pParticle->field_4_flags = pParticle->field_4_flags | 1;
                pParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 73);
                Particle_4C* tmp = pParticle;
                tmp->field_30_pOwnSprite->AllocCollisionShape_59F950(kFpOne64th_6FD554 * kFpTen_6FD508, kFpOne64th_6FD554 * kFpTen_6FD508, kFP16Quarter_6FD2EC);
                pParticle->field_38_state = 31;
                pParticle->field_34_unused = 0;
                Fix16 vec_x;
                Fix16 vec_y;
                Ang16::PolarToCartesian_41FC20(pSprt->field_0_rotation, kFpPoint1_6FD2E8, vec_x, vec_y);
                pParticle->field_2C_counter = 100;
                pParticle->field_46_sub_state = 0;
                pParticle->field_48_timer = 0;
                gParticlePosX_6FD388 = pSprt->field_14_xy.x + vec_x;
                gParticlePosY_6FD38C = pSprt->field_14_xy.y + vec_y;
                Fix16 zpos;
                zpos = pSprt->field_1C_zpos;
                // Both loaded before the type check (one load of field_8, kept for both branches)
                Car* pCar = pSprt->field_8_car_bc_ptr;
                Char* pPed = pSprt->field_8_char_b4_ptr;
                if (pSprt->field_30_sprite_type_enum == sprite_types_enum::car_2)
                {
                    AttachedSprite* pAttached = pCar->field_0_attachments.GetSpriteForModel_5A6A50(objects::water_cannon_114);
                    if (pAttached)
                    {
                        angle = pAttached->field_0_pSprite->field_0_rotation + kAng180_6FD3EE;
                        pos.SetXY_432860(Fix16(0), kFpSevenThirtySeconds_6FD2D4);
                        pos.RotateByAngle_40F6B0(angle);
                        offset.SetXY_432860(Fix16(0), kFP16Eighth_6FD2D0);
                    }
                    else
                    {
                        AttachedSprite* pGun = pCar->field_0_attachments.GetSpriteForModel_5A6A50(248);
                        angle = pGun->field_0_pSprite->field_0_rotation;
                        pos.SetXY_432860(Fix16(0), kFpThreeThirtySeconds_6FD48C);
                        pos.RotateByAngle_40F6B0(angle);
                        offset.SetXY_432860(Fix16(0), kFpMinusNineThirtySeconds_6FD4CC);
                    }
                    pParticle->field_30_pOwnSprite->set_ang_lazy_420690(angle);
                    offset.RotateByAngle_40F6B0(pSprt->field_0_rotation);
                    pos += offset + pSprt->get_x_y_443580();
                    point_velocity = pSprt->field_8_car_bc_ptr->field_58_physics->GetPointVelocity_561350(&pos); // not used
                    pParticle->field_30_pOwnSprite->set_xyz_lazy_420600(pos.x, pos.y, zpos);
                }
                else
                {
                    ped_offset.x = -kFpPoint03_6FD464;
                    ped_offset.y = kFpPoint02_6FD468 + kFpPoint1_6FD2E8;
                    ped_offset.RotateByAngle_40F6B0(pSprt->field_0_rotation);
                    ped_offset = ped_offset + *reinterpret_cast<Fix16_Point*>(&pPed->field_98_velocity_vector);
                    pParticle->field_30_pOwnSprite->set_ang_lazy_420690(pSprt->field_0_rotation);
                    pParticle->field_30_pOwnSprite->set_xyz_lazy_420600(pSprt->field_14_xy.x + ped_offset.x, pSprt->field_14_xy.y + ped_offset.y, zpos);
                }
                pParticle->field_28_pSprite = pSprt;
                if (pParticle->field_30_pOwnSprite->CheckSpriteMovementRegion_5A2500() != 0)
                {
                    pParticle->field_2C_counter = 0;
                }
                gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pOwnSprite);
                pParticle->field_30_pOwnSprite->field_2C_flags = pParticle->field_30_pOwnSprite->field_2C_flags | 4;
            }
        }
    }
}

DEFINE_GLOBAL_INIT(Fix16, kFpEpsilon_6FD500, Fix16(0x1, 0), 0x6FD500);

MATCH_FUNC(0x53FAE0)
void ParticleManager::EmitFireTruckSprayParticle_53FAE0(Sprite* pSprite)
{
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));
    if (!bSkip_particles_67D64D)
    {
        if (!field_4_water_hit_obj)
        {
            field_4_water_hit_obj = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::water_hitting_198, 0, 0, 0, kAngZero_6FD5D4);
        }
        vector.x = Fix16(0);
        vector.y = Fix16(0);
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(vector.x, vector.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
        if (pParticle)
        {
            pParticle->field_4_bf.bDriverAttacking = true;
            pParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 111);
            pParticle->field_30_pOwnSprite->AllocCollisionShape_59F950(kFpOne64th_6FD554 * kFpTen_6FD508, kFpOne64th_6FD554 * kFpTen_6FD508, kFP16Quarter_6FD2EC);
            pParticle->field_34_unused = 0;
            pParticle->field_38_state = 34;
            Fix16 vec_x;
            Fix16 vec_y;
            Ang16::PolarToCartesian_41FC20(pSprite->field_0_rotation, kFpPoint1_6FD2E8, vec_x, vec_y);
            pParticle->field_2C_counter = 100;
            pParticle->field_46_sub_state = 0;
            pParticle->field_48_timer = 0;
            gParticlePosX_6FD388 = pSprite->field_14_xy.x + vec_x;
            gParticlePosY_6FD38C = pSprite->field_14_xy.y + vec_y;
            Fix16 zpos = pSprite->field_1C_zpos;
            if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car_2)
            {
                Car* pCar = pSprite->field_8_car_bc_ptr;
                AttachedSprite* pGun = pCar->field_0_attachments.GetSpriteForModel_5A6A50(objects::water_cannon_114);
                if (pGun)
                {
                    angle = pGun->field_0_pSprite->field_0_rotation + kAng180_6FD3EE;
                }
                else
                {
                    angle = pCar->field_0_attachments.GetSpriteForModel_5A6A50(248)->field_0_pSprite->field_0_rotation;
                }
                pParticle->field_30_pOwnSprite->set_ang_lazy_420690(angle);
            }
            else
            {
                pParticle->field_30_pOwnSprite->set_ang_lazy_420690(pSprite->field_0_rotation);
            }
            pParticle->field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, zpos);
            pParticle->field_28_pSprite = pSprite;
            if (pParticle->field_30_pOwnSprite->CheckSpriteMovementRegion_5A2500())
            {
                pParticle->field_2C_counter = 0;
            }
            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pOwnSprite);
            pParticle->field_30_pOwnSprite->field_2C_bf.bNoLighting = true;
        }
    }
}

// Ang16::PolarToCartesian_41FC20 with the cosine multiply written as the out-of-line Multiply_408680
// the original calls (past the inline budget VC6 would call a COMDAT copy of operator* instead).
static inline void PolarToCartesian_OutOfLineCos(Ang16& angle, Fix16& radius, Fix16& ret1, Fix16& ret2)
{
    ret1 = Ang16::sine_40F500(angle) * radius;
    ret2 = Ang16::cosine_40F520(angle).Multiply_408680(radius);
}

// https://decomp.me/scratch/wfzEd
MATCH_FUNC(0x53FE40)
void ParticleManager::EmitImpactParticles_53FE40(Fix16 x, Fix16 y, Fix16 z, Fix16_Point dir)
{
    // 9.6f declares both angles up front. dir is destroyed by the callee; the unused point gives
    // the EH state 2 that the original sets once t is built.
    Ang16 ang1;
    Ang16 tanAng;
    Fix16_Point unused;
    Fix16_Point t(Fix16(0), Fix16(0));
    tanAng = dir.atan2_40F790();

    for (u8 i = 0; i < 6; ++i)
    {
        t.x = Fix16(0);
        t.y = (Fix16(gRng_6F6784.get_int_4F7AE0(100)) + kFpFifty_6FD558) * kFpPoint0007_6FD4EC;
        if (i < 4)
        {
            ang1 = kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(gRng_6F6784.get_int_4F7AE0(32)));
            // 9.6f: t.RotateByAngle_40F6B0((ang1 + tanAng) - kAng1_6FD5CC.MultiplyByFix16_401CB0(..)) in both branches
            t.RotateByAngle_40F6B0_all_out_of_line(SubAng16_ool(ang1 + tanAng, kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(16))));
        }
        else
        {
            ang1 = kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(gRng_6F6784.get_int_4F7AE0(360)));
            t.RotateByAngle_40F6B0_all_out_of_line(SubAng16_ool(AddAng16_ool(ang1, tanAng), kAng1_6FD5CC.MultiplyByFix16_401CB0_out_of_line(Fix16(180))));
        }

        Particle_4C* pNewParticle = gParticle_8_6FD5E8->New_53E3C0(t.x, t.y, kFpOneThirtySecond_6FD330, t.x.DivideInt_53E860(15).Negate_4086A0(), t.y.DivideInt_53E860(15).Negate_4086A0(), 0);
        if (pNewParticle)
        {
            pNewParticle->field_34_unused = 1;
            pNewParticle->field_38_state = 7;
            pNewParticle->field_2C_counter = 7;
            pNewParticle->field_2E_initial_counter = 7;
            pNewParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pNewParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 127);
            pNewParticle->field_30_pOwnSprite->set_xyz_lazy_420600(x, y, z);
            pNewParticle->field_30_pOwnSprite->SetNoLighting_4337F0();

            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pNewParticle->field_30_pOwnSprite);
        }
    }
}

MATCH_FUNC(0x540320)
void ParticleManager::EmitElectricArcParticle(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 ang)
{
    Ang16 angle;
    Fix16_Point vector(Fix16(0), Fix16(0));

    if (!bSkip_particles_67D64D)
    {
        gRng_6F6784.get_int_4F7AE0(3);
        vector.x = Fix16(0);
        vector.y = (Fix16(gRng_6F6784.get_int_4F7AE0(100)) + kFpFifty_6FD558) * kFpEpsilon_6FD500;

        // The rng result is bound to a const s16& temporary: stored as a short and sign extended only after the
        // word is loaded (the word ends up as the imul's eax operand)
        angle = kAng1_6FD5CC.MultiplyByFix16_401CB0(Fix16(static_cast<const s16&>(gRng_6F6784.get_int_4F7AE0(360))));
        // RotateByAngle_40F6B0, but x * cos, the y line and both sums use the out-of-line Fix16 operators
        {
            Fix16 sin = Ang16::sine_40F500(angle);
            Fix16 cos = Ang16::cosine_40F520(angle);
            Fix16 x_old = vector.x;
            vector.x = vector.x.Multiply_408680(cos).Add_408660(vector.y * sin);
            vector.y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(vector.y.Multiply_408680(cos));
        }

        Particle_4C* pNewParticle = gParticle_8_6FD5E8->New_53E3C0(vector.x, vector.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
        if (pNewParticle)
        {
            pNewParticle->field_34_unused = 0;
            pNewParticle->field_38_state = 37;
            pNewParticle->field_46_sub_state = 0;
            pNewParticle->field_2E_initial_counter = pNewParticle->field_2C_counter;
            pNewParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pNewParticle->field_30_pOwnSprite->SetNoLighting_4337F0();
            pNewParticle->field_30_pOwnSprite->set_xyz_lazy_420600(xpos, ypos, zpos);
            pNewParticle->field_30_pOwnSprite->set_ang_lazy_420690(ang);
            pNewParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gRng_6F6784.get_int_4F7AE0(4) + gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 175);
            pNewParticle->field_30_pOwnSprite->SetBlendAndAlpha_4337D0(2, 20);
            pNewParticle->field_30_pOwnSprite->SetNoLighting_4337F0();
            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pNewParticle->field_30_pOwnSprite);
        }
    }
}

MATCH_FUNC(0x5405D0)
void ParticleManager::SpawnParticleSprite_5405D0(Sprite* pSprite)
{
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0())
    {
        Particle_4C* pNewParticle = gParticle_4C_Pool_6FD5E4->Allocate();
        if (pNewParticle)
        {
            pNewParticle->field_28_pSprite = pSprite;
            pNewParticle->field_34_unused = 0;
            pNewParticle->field_46_sub_state = 0;

            pNewParticle->field_30_pOwnSprite = gSpritePool_703818->get_new_sprite();
            pNewParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pNewParticle->field_38_state = 38;
            pNewParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 164);
            pNewParticle->field_30_pOwnSprite->SetNoLighting_4337F0();
            pNewParticle->field_30_pOwnSprite->set_xyz_lazy_420600(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos);

            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pNewParticle->field_30_pOwnSprite);
        }
    }
}

// 9.6f 0x48E060. Fix16_Point vel has a dtor, which gives the EH frame; its fields are zeroed again
// before the call, and the two other zero args go through the out-of-line Fix16(s32) ctor.
MATCH_FUNC(0x5406b0)
void ParticleManager::SpawnCigaretteSmokePuff_5406B0(Sprite* pSprite, char_type bSmokeSource)
{
    Fix16_Point vel(Fix16(0), Fix16(0));
    if (!bSkip_particles_67D64D)
    {
        vel.x = Fix16(0);
        vel.y = Fix16(0);
        Particle_4C* pNewParticle = gParticle_8_6FD5E8->New_53E3C0(vel.x, vel.y, kFpOneThirtySecond_6FD330, 0, 0, 0);
        if (pNewParticle)
        {
            pNewParticle->field_34_unused = 1;

            if (bSmokeSource)
            {
                pNewParticle->field_38_state = 9;
                pNewParticle->field_2C_counter = 80;
            }
            else
            {
                pNewParticle->field_38_state = 10;
                pNewParticle->field_2C_counter = 70;
            }

            pNewParticle->field_2E_initial_counter = pNewParticle->field_2C_counter;
            pNewParticle->field_30_pOwnSprite->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pNewParticle->field_30_pOwnSprite->set_id_lazy_4206C0(gObjectDefinitions_6FCF00->field_8CA4_def112_base_sprite_idx + 3);

            Fix16 x;
            Fix16 y;
            if (bSmokeSource)
            {
                PolarToCartesian_OutOfLineCos(pSprite->field_0_rotation, kFpPoint07_6FD474, x, y);
            }
            else
            {
                PolarToCartesian_OutOfLineCos(pSprite->field_0_rotation, kFpPoint05_6FD46C, x, y);
            }
            gParticlePosX_6FD388 = x;
            gParticlePosY_6FD38C = y;

            Fix16 offset_x;
            Fix16 offset_y;
            PolarToCartesian_OutOfLineCos(pSprite->field_0_rotation - kAng90_6FD314, kFpPoint02_6FD468, offset_x, offset_y);

            gParticlePosX_6FD388 += offset_x + pSprite->field_14_xy.x;
            gParticlePosY_6FD38C += offset_y + pSprite->field_14_xy.y;

            pNewParticle->field_30_pOwnSprite->set_xyz_lazy_420600(gParticlePosX_6FD388, gParticlePosY_6FD38C, pSprite->field_1C_zpos);
            pNewParticle->field_28_pSprite = pSprite;

            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(pNewParticle->field_30_pOwnSprite);

            if (bSmokeSource)
            {
                pNewParticle->field_30_pOwnSprite->SetNoLighting_4337F0();
            }
        }
    }
}

// Ang16::operator+ (9.6f 0x40E5A0) with the normalizing ctor called out of line (AssignNormalized_409300)
static inline Ang16 AddAng16_ool(const Ang16& a, const Ang16& b)
{
    s16 value = a.rValue + b.rValue;
    return Ang16(&value, 0);
}

MATCH_FUNC(0x5439d0)
ParticleManager::ParticleManager()
{
    if (!gParticle_4C_Pool_6FD5E4)
    {
        gParticle_4C_Pool_6FD5E4 = new T_Particle_4C_Pool();
        if (!gParticle_4C_Pool_6FD5E4)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\particle.cpp", 4167);
        }
    }
    field_0_fire_hit_obj = NULL;
    field_4_water_hit_obj = NULL;
}

MATCH_FUNC(0x543a60)
ParticleManager::~ParticleManager()
{
    if (gParticle_4C_Pool_6FD5E4)
    {
        GTA2_DELETE_AND_NULL(gParticle_4C_Pool_6FD5E4);
    }
    field_0_fire_hit_obj = NULL;
    field_4_water_hit_obj = NULL;
}