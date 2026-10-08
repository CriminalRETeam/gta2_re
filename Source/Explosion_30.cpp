#include "Explosion_30.hpp"
#include "ped_death_cause.hpp"
#include "explosion_type.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Particle_4C.hpp"
#include "Particle_8.hpp"
#include "Player.hpp"
#include "PurpleDoom.hpp"
#include "Varrok_7F8.hpp"
#include "debug.hpp"
#include "rng.hpp"

DEFINE_GLOBAL(ExplosionPool_7A8*, gExplosionPool_7A8_6FD5F0, 0x6FD5F0);
DEFINE_GLOBAL(ExplosionPool_3D4*, gExplosionPool_3D4_6FD5EC, 0x6FD5EC);

EXTERN_GLOBAL(u16, gParticleInstCount_6FD5F4);

DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_6FD49C, Fix16(0), 0x6FD49C);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_6FD5D4, Ang16(0), 0x6FD5D4);

// Defined in Particle_4C.cpp: with the definition (and its dynamic initialiser) in this TU, VC6
// loads it with a 32-bit mov and adds with lea in the state_3/4/5/13 functions instead of the
// original's 16-bit mov/add (540D30 181 -> 32 diff lines).
EXTERN_GLOBAL(Ang16, kAng180_6FD3EE);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD448, Fix16(0x100, 0), 0x6FD448);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD328, dword_6FD448, 0x6FD328);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD330, dword_6FD328 * 2, 0x6FD330);

DEFINE_GLOBAL_INIT(s16, gExplosionId_623F18, 1, 0x623F18);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD2F0, Fix16(0xCCC, 0), 0x6FD2F0);

EXTERN_GLOBAL(Fix16, kFP16Half_6FD39C);
EXTERN_GLOBAL(Fix16, kFP16One_6FD4A0);
EXTERN_GLOBAL(Fix16, kFP16Two_6FD4A4);

EXTERN_GLOBAL(Fix16, stru_6FD388);
EXTERN_GLOBAL(Fix16, stru_6FD38C);

DEFINE_GLOBAL_INIT(Fix16, kFP16ThreeQuarters_6FD370, Fix16(0x3000, 0), 0x6FD370);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FD2EC, Fix16(0x1000, 0), 0x6FD2EC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD540, Fix16(0x10, 0), 0x6FD540);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD484, Fix16(0x5C2, 0), 0x6FD484);
DEFINE_GLOBAL(u8, unk_6FD5F6, 0x6FD5F6);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD548, Fix16(0x20, 0), 0x6FD548);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eight_6FD4C0, Fix16(8), 0x6FD4C0);
DEFINE_GLOBAL_INIT(Ang16, dword_6FD350, Ang16(96), 0x6FD350);
DEFINE_GLOBAL_INIT(Ang16, kAng135_6FD40C, Ang16(540), 0x6FD40C);
DEFINE_GLOBAL_INIT(Ang16, kAng315_6FD418, Ang16(1260), 0x6FD418);
DEFINE_GLOBAL_INIT(Ang16, kAng225_6FD3E0, Ang16(900), 0x6FD3E0);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6FD35C, Ang16(180), 0x6FD35C);

DEFINE_GLOBAL_INIT(Fix16_Point, kZeroPoint_6FD570, Fix16_Point(Fix16(0), Fix16(0)), 0x6FD570);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD2F4, Fix16(0x1333, 0), 0x6FD2F4);

EXTERN_GLOBAL(Fix16, dword_6FD2E8);
EXTERN_GLOBAL(Fix16, dword_6FD46C);

MATCH_FUNC(0x5408f0)
Explosion_30::Explosion_30()
{
    this->field_C_angle = 0;
    this->field_20_unused = 0;
    this->field_22_spawn_angle = 0;
    this->field_4_idx = 0;
    this->field_10_type = 0;
    this->field_14_pObj2C = 0;
    this->field_18_particle_cooldown = 0;
    this->field_1A_timer = 0;
    this->field_8_speed = kFP16Zero_6FD49C;
    this->field_20_unused = kAngZero_6FD5D4;
    this->field_22_spawn_angle = kAngZero_6FD5D4;
    this->field_24_particle_spread = kFP16Zero_6FD49C;
    this->field_28_blast_radius = kFP16Zero_6FD49C;
    this->field_C_angle = kAngZero_6FD5D4;
    this->field_1C_pAttachedSprite = 0;
    this->field_6_id = 0;
    this->field_2C_owner_ped_id = 0;
}

MATCH_FUNC(0x540a10)
Explosion_30::~Explosion_30()
{
    field_14_pObj2C = 0;
    field_1C_pAttachedSprite = 0;
}

MATCH_FUNC(0x540d30)
void Explosion_30::EmitFireTrail_3_12_540D30(Ang16 ang, Fix16 speed)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the speed stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = speed;
    point.y = speed;
    point.RotateByAngle_40F6B0(ang + kAng180_6FD3EE);

    this->field_8_speed = speed;
    this->field_C_angle = ang;

    if (this->field_18_particle_cooldown == 0)
    {
        //speed = (int)&v27; // TODO: Field_20 wrong val ??
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(point.x, point.y, dword_6FD330, point.x, point.y, 0);
        if (pParticle)
        {
            pParticle->field_40_pExplosion = this;
            pParticle->field_44 = this->field_6_id;
            pParticle->field_20_speed = speed;
            pParticle->field_24_angle = ang;
            pParticle->field_34 = 0;
            pParticle->field_46_sub_state = 0;
            pParticle->field_2C_counter = 32;
            pParticle->field_2E = 32;
            pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_38_state = 3;
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 96);
            pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pParticle->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            this->field_18_particle_cooldown = gRng_6F6784.get_int_4F7AE0(2);
        }
    }
    else
    {
        this->field_18_particle_cooldown--;
    }
}

// 9.6f 0x48E5F0
MATCH_FUNC(0x540f90)
void Explosion_30::EmitFireTrail_4_540F90(Ang16 ang, Fix16 speed)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the speed stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = speed;
    point.y = speed;
    point.RotateByAngle_40F6B0(ang + kAng180_6FD3EE);

    this->field_8_speed = speed;
    this->field_C_angle = ang;

    if (this->field_18_particle_cooldown == 0)
    {
        //speed = (int)&v27; // TODO: Field_20 wrong val ??
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(point.x, point.y, dword_6FD330, point.x, point.y, 0);
        if (pParticle)
        {
            pParticle->field_40_pExplosion = this;
            pParticle->field_44 = this->field_6_id;
            pParticle->field_20_speed = speed;
            pParticle->field_24_angle = ang;
            pParticle->field_34 = 0;
            pParticle->field_46_sub_state = 0;
            pParticle->field_2C_counter = 32;
            pParticle->field_2E = 32;
            pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_38_state = 4;
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette);
            pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pParticle->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            this->field_18_particle_cooldown = gRng_6F6784.get_int_4F7AE0(2);
        }
    }
    else
    {
        this->field_18_particle_cooldown--;
    }
}

MATCH_FUNC(0x5411e0)
void Explosion_30::EmitFireTrail_13_14_5411E0(Ang16 ang, Fix16 speed)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the speed stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = speed;
    point.y = speed;
    point.RotateByAngle_40F6B0(ang + kAng180_6FD3EE);

    this->field_8_speed = speed;
    this->field_C_angle = ang;

    if (this->field_18_particle_cooldown == 0)
    {
        //speed = (int)&v27; // TODO: Field_20 wrong val ??
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(point.x, point.y, dword_6FD330, point.x, point.y, 0);
        if (pParticle)
        {
            pParticle->field_40_pExplosion = this;
            pParticle->field_44 = this->field_6_id;
            pParticle->field_20_speed = speed;
            pParticle->field_24_angle = ang;
            pParticle->field_34 = 0;
            pParticle->field_46_sub_state = 0;
            pParticle->field_2C_counter = 32;
            pParticle->field_2E = 32;
            pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_38_state = 36;
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette);
            pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pParticle->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            this->field_18_particle_cooldown = gRng_6F6784.get_int_4F7AE0(2);
        }
    }
    else
    {
        this->field_18_particle_cooldown--;
    }
}

MATCH_FUNC(0x541430)
void Explosion_30::EmitFireTrail_5_541430(Ang16 ang, Fix16 speed)
{

    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = speed;
    point.y = speed;
    point.RotateByAngle_40F6B0(ang + kAng180_6FD3EE);

    this->field_8_speed = speed;
    this->field_C_angle = ang;

    if (field_14_pObj2C->sub_5290F0() == kFP16Zero_6FD49C && this->field_1A_timer == explosion_timer::forever_9999)
    {
        this->field_1A_timer = 20;
    }

    if (!this->field_18_particle_cooldown)
    {
        Particle_4C* pParticle = gParticle_8_6FD5E8->New_53E3C0(point.x, point.y, dword_6FD330, point.x, point.y, 0);
        if (pParticle)
        {
            pParticle->field_40_pExplosion = this;
            pParticle->field_44 = field_6_id;
            pParticle->field_20_speed = speed;
            pParticle->field_24_angle = ang;
            pParticle->field_34 = 0;
            pParticle->field_46_sub_state = 0;
            pParticle->field_2C_counter = 32;
            pParticle->field_2E = 32;
            pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_38_state = 5;
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 96);
            pParticle->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x,
                                                      field_14_pObj2C->field_4->field_14_xy.y,
                                                      field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            this->field_18_particle_cooldown = gRng_6F6784.get_int_4F7AE0(2);
            pParticle->field_30_pNext->SetFlags_4337D0(2, 20);
            pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
        }
    }
    else
    {
        this->field_18_particle_cooldown--;
    }
}

MATCH_FUNC(0x541680)
Fix16 Explosion_30::GetBlastRadius_541680()
{
    Fix16 radius;
    switch (this->field_10_type)
    {
        case explosion_type::small_18:
        case explosion_type::building_45_22:
        case explosion_type::building_225_23:
        case explosion_type::building_135_24:
        case explosion_type::building_315_25:
            radius = kFP16Half_6FD39C;
            break;
        case explosion_type::small_33:
            radius = kFP16ThreeQuarters_6FD370;
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
Fix16 Explosion_30::GetBlastHeight_541710()
{
    Fix16 height;
    switch (this->field_10_type)
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
            height = dword_6FD2F0;
            break;
        default:
            break;
    }
    return height;
}

MATCH_FUNC(0x541760)
void Explosion_30::SpawnFlashParticle_541760()
{
    if (field_10_type != explosion_type::small_18 && field_10_type != explosion_type::no_ring_32)
    {
        if (gParticle_4C_Pool_6FD5E4->field_0_pStart)
        {
            Particle_4C* pNew = gParticle_4C_Pool_6FD5E4->Allocate();
            pNew->field_46_sub_state = 0;
            pNew->field_38_state = 29;
            pNew->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pNew->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pNew->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 37);
            pNew->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x,
                                                      field_14_pObj2C->field_4->field_14_xy.y,
                                                      field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew->field_30_pNext);
            pNew->field_48_timer = 10;
        }
    }
}

MATCH_FUNC(0x541850)
void Explosion_30::ApplyBlastDamage_541850(u16 timerVal)
{
    struct_4 collision_list;

    Fix16 half_height = Explosion_30::GetBlastHeight_541710();

    Fix16 blast_radius = Explosion_30::GetBlastRadius_541680();

    this->field_28_blast_radius = blast_radius;

    Fix16 left = field_14_pObj2C->field_4->field_14_xy.x - field_28_blast_radius * kFP16Two_6FD4A4;
    Fix16 right = field_14_pObj2C->field_4->field_14_xy.x + field_28_blast_radius * kFP16Two_6FD4A4;
    Fix16 top = field_14_pObj2C->field_4->field_14_xy.y - field_28_blast_radius * kFP16Two_6FD4A4;
    Fix16 bottom = field_14_pObj2C->field_4->field_14_xy.y + field_28_blast_radius * kFP16Two_6FD4A4;
    Fix16 z_low = field_14_pObj2C->field_4->field_1C_zpos - half_height;
    Fix16 z_high = field_14_pObj2C->field_4->field_1C_zpos + half_height;

    if (this->field_1A_timer == explosion_timer::first_tick_99 && unk_6FD5F6 == 1)
    {
        SpawnFlashParticle_541760();
    }

    Fix16_Rect rect;
    rect.SetRect_41E350(left, right, top, bottom);
    rect.SetHiLowZ_41E370(z_low, z_high);

    if (gPurpleDoom_1_679208->CollectRectCollisions_477F30(&rect, 0, 0, field_14_pObj2C->field_4, &collision_list))
    {
        while (collision_list.field_0_p18)
        {
            Sprite* pSprite = collision_list.PopFrontSprite_5A6DA0();
            Char_B4* pPedChar = pSprite->AsCharB4_40FEA0();
            if (pPedChar)
            {
                if (timerVal > explosion_timer::damage_phase_50 && pPedChar->get_ped_state_1_48A4C0() != ped_state_1::immobilized_8)
                {
                    s32 ped_id = gVarrok_7F8_703398->GetPedId_420F10(this->field_14_pObj2C->field_26_varrok_idx);
                    if (!ped_id)
                    {
                        pPedChar->field_7C_pPed->field_204_killer_id = this->field_2C_owner_ped_id;
                    }
                    else
                    {
                        pPedChar->field_7C_pPed->field_204_killer_id = ped_id;
                    }
                    pPedChar->field_7C_pPed->field_290_death_cause = ped_death_cause::unknown_4;
                    pPedChar->field_7C_pPed->field_264_killer_id_timer = 50;

                    Fix16 dx = pSprite->field_14_xy.x - this->field_14_pObj2C->field_4->field_14_xy.x;
                    Fix16 dy = pSprite->field_14_xy.y - this->field_14_pObj2C->field_4->field_14_xy.y;

                    Ang16 ang;
                    ang = Fix16::atan2_fixed_405320(dy, dx);

                    Fix16 distance = Fix16::MaxAbsDistance_42A6B0(pSprite->field_14_xy.x, pSprite->field_14_xy.y, field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y);
                    if (distance > this->field_28_blast_radius)
                    {
                        if (timerVal < 70u)
                        {
                            pPedChar->HandleGenericImpact_553E00(ang, dword_6FD2E8 + dword_6FD46C, kFP16Zero_6FD49C, 0);
                        }
                    }
                    else
                    {
                        char_type knockback_type;
                        Fix16 knockback_force;
                        if (distance < (kFP16Half_6FD39C * this->field_28_blast_radius))
                        {
                            knockback_force = dword_6FD2F4;
                            knockback_type = 2;
                        }
                        else
                        {
                            knockback_force = dword_6FD2E8;
                            knockback_type = 1;
                        }
                        pPedChar->HandleGenericImpact_553E00(ang, dword_6FD484, knockback_force, knockback_type);
                    }
                }
            }
            else
            {
                Car_BC* pCar = pSprite->AsCar_40FEB0();
                if (pCar)
                {
                    if ((timerVal > explosion_timer::damage_phase_50 && timerVal < 60u) || (timerVal > 80u && timerVal < 90u))
                    {
                        if (!pCar->IsMaxDamage_40F890() && !pCar->IsTrainModel_403BA0() && !pCar->sub_43B850(field_10_type))
                        {
                            if (Fix16::MaxAbsDistance_42A6B0(pSprite->field_14_xy.x, pSprite->field_14_xy.y, field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y) <= this->field_28_blast_radius)
                            {
                                // 9.6f: Varrok_7F8::GetPedId_420F10 (inlined, using it here makes the diff worse)
                                s32 exploder_ped_id = gVarrok_7F8_703398->field_0_entries[this->field_14_pObj2C->field_26_varrok_idx].field_0_ped_id;
                                if (!exploder_ped_id)
                                {
                                    pCar->field_70_exploder_ped_id = this->field_2C_owner_ped_id;
                                }
                                else
                                {
                                    pCar->field_70_exploder_ped_id = exploder_ped_id;
                                }
                                pCar->field_90 = 4;
                                pCar->field_94_exploder_timer = 50;
                                s16 damage = pCar->AccumulateDamage_43DA90(32000, &kZeroPoint_6FD570);
                                if (pCar->field_70_exploder_ped_id)
                                {
                                    if (damage > 0)
                                    {
                                        Ped* pExploderPed = gPedManager_6787BC->PedById(pCar->field_70_exploder_ped_id);
                                        if (pExploderPed)
                                        {
                                            if (pExploderPed->PedTypeIs_45EDE0(ped_type::player_2))
                                            {
                                                pExploderPed->field_15C_player->field_2D4_scores.AwardCarDamageScoreHit_593150(pCar, 1);
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                pCar->ApplyVisualDamage_43A9F0();
                            }
                        }
                    }
                    else if (timerVal == explosion_timer::first_tick_99)
                    {
                        if (Fix16::MaxAbsDistance_42A6B0(pSprite->field_14_xy.x, pSprite->field_14_xy.y, field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y) <= this->field_28_blast_radius)
                        {
                            pCar->ApplyExplosionImpulse_443710(&this->field_14_pObj2C->field_4->get_x_y_443580());
                        }
                    }
                }
                else
                {
                    Object_2C* pObject = pSprite->As2C_40FEC0();
                    if (timerVal > explosion_timer::damage_phase_50 && timerVal < 60u)
                    {
                        pObject->sub_525190(this->field_14_pObj2C->field_26_varrok_idx);
                    }
                }
            }
        }
    }
}

// 9.6f 0x48EB00
MATCH_FUNC(0x541d60)
void Explosion_30::EmitExplosion_18_33_541D60()
{
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0())
    {
        if ((u16)field_1A_timer > 0x52u)
        {
            if ((u16)field_1A_timer > 0x5Au)
            {
                Fix16 radius;
                radius = this->field_24_particle_spread * Fix16(gRng_6F6784.get_int_4F7AE0(8));

                this->field_22_spawn_angle = Ang16::Fix16_To_Ang16_inlined_40F540(dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(360)));

                // 9.6f calls Ang16::PolarToCartesian_41FC20
                Ang16::PolarToCartesian_41FC20(field_22_spawn_angle, radius, stru_6FD388, stru_6FD38C);

                // NOTE: This proves these 2 vars are not a Fix16_Point
                stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x + stru_6FD388;
                stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y + stru_6FD38C;

                Particle_4C* pParticle = gParticle_4C_Pool_6FD5E4->Allocate();
                pParticle->field_46_sub_state = 0;
                pParticle->field_38_state = 18;
                pParticle->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
                pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
                pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
                pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
                pParticle->field_48_timer = 1;
            }
            else
            {
                stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x;
                stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y;

                Particle_4C* pParticle = gParticle_4C_Pool_6FD5E4->Allocate();
                pParticle->field_46_sub_state = 0;
                pParticle->field_38_state = 18;
                pParticle->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
                pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
                pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
                pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
                pParticle->field_30_pNext->ApplyScaleToDimensions_59E4C0(kFP16Quarter_6FD2EC, 0);
                pParticle->field_48_timer = 5;
            }
        }
    }
}

MATCH_FUNC(0x542060)
void Explosion_30::EmitExplosion_19_32_542060()
{
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0())
    {
        if (this->field_1A_timer > 8u)
        {
            Fix16 radius;
            radius = this->field_24_particle_spread * Fix16(gRng_6F6784.get_int_4F7AE0(48));
            this->field_22_spawn_angle = Ang16::Fix16_To_Ang16_inlined_40F540(dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(360)));

            // 9.6f calls Ang16::PolarToCartesian_41FC20
            Ang16::PolarToCartesian_41FC20(field_22_spawn_angle, radius, stru_6FD388, stru_6FD38C);

            stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x + stru_6FD388;
            stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y + stru_6FD38C;

            Particle_4C* pParticle = gParticle_4C_Pool_6FD5E4->Allocate();
            pParticle->field_46_sub_state = 0;
            pParticle->field_38_state = 19;
            pParticle->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pParticle->field_40_pExplosion = this;
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
            pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, this->field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            pParticle->field_30_pNext->ApplyScaleToDimensions_59E4C0(kFP16Half_6FD39C + kFP16One_6FD4A0, 0);
            pParticle->field_48_timer = 1;
        }
        else
        {
            stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x;
            stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y;

            Particle_4C* pParticle = gParticle_4C_Pool_6FD5E4->Allocate();
            pParticle->field_46_sub_state = 0;
            pParticle->field_38_state = 19;
            pParticle->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
            pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            pParticle->field_48_timer = 5;
        }
    }
}

MATCH_FUNC(0x542340)
void Explosion_30::EmitExplosion_20_542340()
{
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0())
    {
        if (this->field_1A_timer > 8u)
        {
            Fix16 radius;
            radius = this->field_24_particle_spread * Fix16(gRng_6F6784.get_int_4F7AE0(80));
            this->field_22_spawn_angle = Ang16::Fix16_To_Ang16_inlined_40F540(dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(360)));

            // 9.6f calls Ang16::PolarToCartesian_41FC20
            Ang16::PolarToCartesian_41FC20(field_22_spawn_angle, radius, stru_6FD388, stru_6FD38C);

            stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x + stru_6FD388;
            stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y + stru_6FD38C;

            Particle_4C* pParticle = gParticle_4C_Pool_6FD5E4->Allocate();
            pParticle->field_46_sub_state = 0;
            pParticle->field_38_state = 20;
            pParticle->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 56);
            pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            pParticle->field_48_timer = 1;
        }
        else
        {
            stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x;
            stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y;

            Particle_4C* pParticle = gParticle_4C_Pool_6FD5E4->Allocate();
            pParticle->field_46_sub_state = 0;
            pParticle->field_38_state = 20;
            pParticle->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
            pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 56);
            pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
            pParticle->field_48_timer = 5;
        }
    }
}

MATCH_FUNC(0x542790)
void Explosion_30::UpdateExplosion_18_19_20_32_33_542790()
{
    bool isOnScreen = true;
    if (this->field_1A_timer < 90u)
    {
        if (!gGame_0x40_67E008->is_point_on_screen_4B9A80(this->field_14_pObj2C->field_4->field_14_xy.x, this->field_14_pObj2C->field_4->field_14_xy.y))
        {
            isOnScreen = false;
        }
    }

    unk_6FD5F6 = 0;
    if (isOnScreen)
    {
        switch (this->field_10_type)
        {
            case explosion_type::small_18:
            case explosion_type::small_33:
                EmitExplosion_18_33_541D60();
                break;
            case explosion_type::item_19:
            case explosion_type::no_ring_32:
                EmitExplosion_19_32_542060();
                unk_6FD5F6 = 1;
                break;
            case explosion_type::large_20:
                EmitExplosion_20_542340();
                unk_6FD5F6 = 1;
                break;
            default:
                break;
        }

        switch (this->field_1A_timer)
        {
            case 59:
            case 69:
            case 79:
            case 89:
            {
                Object_2C* pExplosionObj = gObject_5C_6F8F84->CreateExplosion_52A3D0(Fix16(113), Fix16(145), 2, kAngZero_6FD5D4, explosion_type::molotov_fire_5, field_2C_owner_ped_id);
                if (pExplosionObj)
                {
                    Object_2C* pDebris = gObject_5C_6F8F84->NewUnknown_52A240(127,
                                                                             field_14_pObj2C->field_4->field_14_xy.x,
                                                                             field_14_pObj2C->field_4->field_14_xy.y,
                                                                             field_14_pObj2C->field_4->field_1C_zpos,
                                                                             this->field_22_spawn_angle,
                                                                             kAngZero_6FD5D4,
                                                                             dword_6FD484,
                                                                             -dword_6FD540,
                                                                             dword_6FD2F0);
                    pDebris->field_4->DispatchCollisionEvent_5A3100(pExplosionObj->field_4, 0, 0, kAngZero_6FD5D4);

                    Object_2C* pLight = gObject_5C_6F8F84->NewLight_529A40(94, 138, 2, 0xFF8000, 3, 255);
                    pDebris->field_4->DispatchCollisionEvent_5A3100(pLight->field_4, 0, 0, kAngZero_6FD5D4);
                }
                break;
            }

            default:
                break;
        }
    }

    if (this->field_1A_timer > 0x1Eu)
    {
        Fix16 max_spread = (dword_6FD448 * kFP16Two_6FD4A4);
        if (this->field_24_particle_spread > max_spread)
        {
            this->field_24_particle_spread = max_spread;
        }
        this->field_24_particle_spread += (dword_6FD540 / kFP16Two_6FD4A4);
    }
    else
    {
        if (this->field_24_particle_spread > dword_6FD448)
        {
            this->field_24_particle_spread = dword_6FD448;
        }
        this->field_24_particle_spread -= (dword_6FD540 / kFP16Two_6FD4A4);
    }

    if (this->field_1A_timer > explosion_timer::damage_phase_50)
    {
        ApplyBlastDamage_541850(this->field_1A_timer);
    }

    if (this->field_1A_timer == explosion_timer::first_tick_99)
    {
        // TODO: Arg order correct?
        gGame_0x40_67E008->ShakeCamerasAtPos_4B9790(8, this->field_14_pObj2C->field_4->field_14_xy.x, this->field_14_pObj2C->field_4->field_14_xy.y);
    }

    if (this->field_1A_timer != explosion_timer::forever_9999)
    {
        this->field_1A_timer--;
    }

    if (this->field_1A_timer > explosion_timer::forever_9999)
    {
        this->field_1A_timer = 1;
    }
}

MATCH_FUNC(0x542e30)
void Explosion_30::EmitBuildingDebris_22_23_24_25_542E30(char_type direction_idx)
{
    Sprite* pSprite = this->field_14_pObj2C->field_4;
    if (pSprite->field_14_xy.x < Fix16(0x3F8000, 0) && pSprite->field_14_xy.x > kFP16One_6FD4A0 &&
        pSprite->field_14_xy.y < Fix16(0x3F8000, 0) && pSprite->field_14_xy.y > kFP16One_6FD4A0)
    {
        unk_6FD5F6 = 0;
        for (u8 i = 0; i < 2u; i++)
        {
            if (gParticle_4C_Pool_6FD5E4->field_0_pStart)
            {
                Particle_4C* pParticle = gParticle_4C_Pool_6FD5E4->Allocate();
                pParticle->field_46_sub_state = 0;

                switch ((u8)direction_idx)
                {
                    case 0:
                    {
                        pParticle->field_38_state = 24;
                        this->field_22_spawn_angle = (kAng135_6FD40C + dword_6FD350) + Ang16::Fix16_To_Ang16_40F540((dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(45))));

                        Ang16::PolarToCartesian_41FC20(field_22_spawn_angle, dword_6FD540, stru_6FD388, stru_6FD38C);

                        stru_6FD388 += this->field_14_pObj2C->field_4->field_14_xy.x;
                        stru_6FD38C += this->field_14_pObj2C->field_4->field_14_xy.y;
                        break;
                    }

                    case 1:
                    {
                        pParticle->field_38_state = 25;
                        this->field_22_spawn_angle = (kAng315_6FD418 + dword_6FD350) + Ang16::Fix16_To_Ang16_40F540((dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(90))));


                        Ang16::PolarToCartesian_41FC20(field_22_spawn_angle, dword_6FD540, stru_6FD388, stru_6FD38C);

                        stru_6FD388 += this->field_14_pObj2C->field_4->field_14_xy.x;
                        stru_6FD38C += this->field_14_pObj2C->field_4->field_14_xy.y;
                        break;
                    }

                    case 2:
                    {
                        pParticle->field_38_state = 23;
                        this->field_22_spawn_angle = (kAng225_6FD3E0 + dword_6FD350).Add_ool(Ang16::Fix16_To_Ang16_40F540((dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(90)))));


                        Ang16::PolarToCartesian_41FC20(field_22_spawn_angle, dword_6FD540, stru_6FD388, stru_6FD38C);

                        stru_6FD388 += this->field_14_pObj2C->field_4->field_14_xy.x;
                        stru_6FD38C += this->field_14_pObj2C->field_4->field_14_xy.y;
                        break;
                    }

                    case 3:
                    {
                        pParticle->field_38_state = 22;
                        this->field_22_spawn_angle = kAng45_6FD35C.Add_ool(dword_6FD350).Add_ool(Ang16::Fix16_To_Ang16_ool_40F540((dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(90)))));


                        Ang16::PolarToCartesian_41FC20(field_22_spawn_angle, dword_6FD540, stru_6FD388, stru_6FD38C);

                        stru_6FD388 += this->field_14_pObj2C->field_4->field_14_xy.x;
                        stru_6FD38C += this->field_14_pObj2C->field_4->field_14_xy.y;
                        break;
                    }

                    default:
                        break;
                }

                pParticle->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
                pParticle->field_30_pNext->SetType_4206F0(sprite_types_enum::code_obj2_8);
                pParticle->field_48_timer = 0;
                pParticle->field_46_sub_state = 0;
                pParticle->field_24_angle = this->field_22_spawn_angle;

                if (this->field_1A_timer < 60u && this->field_1A_timer < 30u)
                {
                    i = 4;
                }

                pParticle->field_20_speed = (dword_6FD548 * Fix16(gRng_6F6784.get_int_4F7AE0(field_1A_timer)));
                pParticle->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 40);

                if (field_14_pObj2C->field_4->field_1C_zpos + kFP16One_6FD4A0 >= kFP16Eight_6FD4C0)
                {
                    pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
                }
                else
                {
                    pParticle->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos + kFP16One_6FD4A0);
                }
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pParticle->field_30_pNext);
                pParticle->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            }
        }

        if (this->field_1A_timer == explosion_timer::first_tick_99)
        {
            gGame_0x40_67E008->ShakeCamerasAtPos_4B9790(8, field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y);
        }

        if (this->field_1A_timer > explosion_timer::damage_phase_50)
        {
            ApplyBlastDamage_541850(this->field_1A_timer);
        }

        if (this->field_1A_timer != explosion_timer::forever_9999)
        {
            if (this->field_1A_timer > 60u)
            {
                this->field_1A_timer--;
            }

            if (this->field_1A_timer > explosion_timer::forever_9999)
            {
                this->field_1A_timer = 1;
            }
        }
    }
}

MATCH_FUNC(0x5434a0)
char_type Explosion_30::Update_5434A0(Fix16 speed, Ang16 ang)
{
    u16 timer = this->field_1A_timer;
    if (timer != explosion_timer::forever_9999)
    {
        if (timer > 0)
        {
            this->field_1A_timer = timer - 1;
        }
    }

    if (!this->field_1A_timer)
    {
        Explosion_30::Release_543610();
        return 1;
    }

    if (bSkip_particles_67D64D)
    {
        return 1;
    }

    switch (this->field_10_type)
    {
        case explosion_type::fire_3:
        case explosion_type::car_fire_level1_12:
            Explosion_30::EmitFireTrail_3_12_540D30(ang, speed);
            return 0;
        case explosion_type::car_fire_level2_13:
        case explosion_type::car_fire_level3_14:
            Explosion_30::EmitFireTrail_13_14_5411E0(ang, speed);
            return 0;
        case explosion_type::car_fire_4:
            Explosion_30::EmitFireTrail_4_540F90(ang, speed);
            return 0;
        case explosion_type::molotov_fire_5:
            Explosion_30::EmitFireTrail_5_541430(ang, speed);
            return 0;
        case explosion_type::small_18:
        case explosion_type::item_19:
        case explosion_type::large_20:
        case explosion_type::no_ring_32:
        case explosion_type::small_33:
            Explosion_30::UpdateExplosion_18_19_20_32_33_542790();
            return 0;
        case explosion_type::building_135_24:
            Explosion_30::EmitBuildingDebris_22_23_24_25_542E30(0);
            return 0;
        case explosion_type::building_315_25:
            Explosion_30::EmitBuildingDebris_22_23_24_25_542E30(1);
            return 0;
        case explosion_type::building_225_23:
            Explosion_30::EmitBuildingDebris_22_23_24_25_542E30(2);
            return 0;
        case explosion_type::building_45_22:
            Explosion_30::EmitBuildingDebris_22_23_24_25_542E30(3);
            return 0;
    }
    return 0;
}

MATCH_FUNC(0x5435d0)
bool Explosion_30::IsFireType_5435D0()
{
    switch (field_10_type)
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
void Explosion_30::Release_543610()
{
    this->field_6_id = 0;
    if (field_0_bIn20Pool == 0)
    {
        gExplosionPool_7A8_6FD5F0->field_780_bUsed[this->field_4_idx] = 0;
    }
    else
    {
        gExplosionPool_3D4_6FD5EC->field_3C0_bUsed[this->field_4_idx] = 0;
    }
}

MATCH_FUNC(0x543650)
void Explosion_30::Init_543650()
{
    this->field_10_type = 0;
    this->field_18_particle_cooldown = 0;
    this->field_24_particle_spread = 0;
    this->field_22_spawn_angle = kAngZero_6FD5D4;
    this->field_1A_timer = 200;
    this->field_14_pObj2C = 0;
    this->field_2C_owner_ped_id = 0;
    this->field_0_bIn20Pool = 0;
}

MATCH_FUNC(0x543680)
void Explosion_30::SetObject_543680(Object_2C* pObj)
{
    this->field_14_pObj2C = pObj;
}

// Every explosion gets a priority from its type: 1 (unknown types, evicted straight away) up to 6, and a late fire or
// explosion counts as 3. The one with the lowest priority has its timer set to 0 so that it ends on its next update.
WIP_FUNC(0x543690)
void ExplosionPool_7A8::FreeLowestPriority_543690()
{
    WIP_IMPLEMENTED;

    u8 lowest_priority = 99;
    u8 lowest_idx = 99;
    u8 priority = 0;
    u8 idx = 0;
    u8 cur_idx = 0;
    do
    {
        if (this->field_780_bUsed[cur_idx] == 1)
        {
            Explosion_30* pExplosion = &this->field_0_explosions[cur_idx];
            // Each case written out on its own: merged labels give a byte index table, the
            // original has one dword entry per case. Cases 1 and 39 keep the range.
            switch (pExplosion->field_10_type)
            {
                case explosion_type::unknown_2:
                    break;
                case explosion_type::fire_3:
                    break;
                case explosion_type::car_fire_4:
                    break;
                case explosion_type::unknown_21:
                    break;
                case explosion_type::unknown_31:
                    break;
                case explosion_type::unknown_34:
                    break;
                case explosion_type::molotov_fire_5:
                    priority = 2;
                    break;
                case explosion_type::unknown_28:
                    priority = 2;
                    break;
                case explosion_type::unknown_29:
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
                    if (pExplosion->field_1A_timer < 82u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::small_33:
                    if (pExplosion->field_1A_timer < 82u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::item_19:
                    if (pExplosion->field_1A_timer < 50u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::large_20:
                    if (pExplosion->field_1A_timer < 50u)
                    {
                        priority = 3;
                    }
                    break;
                case explosion_type::no_ring_32:
                    if (pExplosion->field_1A_timer < 50u)
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
                case explosion_type::unknown_1:
                    priority = 1;
                    break;
                case explosion_type::unknown_39:
                    priority = 1;
                    break;
                default:
                    priority = 1;
                    break;
            }

            if (priority == 1)
            {
                this->field_0_explosions[cur_idx].field_1A_timer = 0;
                return;
            }

            if (priority < lowest_priority)
            {
                lowest_priority = priority;
                lowest_idx = idx;
            }
        }
        cur_idx = ++idx;
    } while (idx < GTA2_COUNTOF(field_0_explosions));
    this->field_0_explosions[lowest_idx].field_1A_timer = 0;
}

MATCH_FUNC(0x543800)
Explosion_30* ExplosionPool_7A8::Allocate_543800()
{
    // 9.6f has the init block twice, 10.5 merges both into one block. Indexing field_0_explosions at each
    // use (no pNew local) gives both copies the same registers, so they merge completely.
    u8 idx;
    // the first 20 slots are tried first; only when they are all used the lowest priority explosion is evicted
    for (idx = 0; idx < 20; idx++)
    {
        if (!this->field_780_bUsed[idx])
        {
            this->field_0_explosions[idx].Init_543650();
            this->field_0_explosions[idx].field_4_idx = idx;
            this->field_0_explosions[idx].field_6_id = gExplosionId_623F18;
            this->field_0_explosions[idx].field_0_bIn20Pool = 0;
            gExplosionId_623F18++;
            this->field_780_bUsed[idx] = 1;
            return &this->field_0_explosions[idx];
        }
    }

    FreeLowestPriority_543690();

    for (idx = 0; idx < GTA2_COUNTOF(field_0_explosions); idx++)
    {
        if (!this->field_780_bUsed[idx])
        {
            this->field_0_explosions[idx].Init_543650();
            this->field_0_explosions[idx].field_4_idx = idx;
            this->field_0_explosions[idx].field_6_id = gExplosionId_623F18;
            this->field_0_explosions[idx].field_0_bIn20Pool = 0;
            gExplosionId_623F18++;
            this->field_780_bUsed[idx] = 1;
            return &this->field_0_explosions[idx];
        }
    }
    return 0;
}

MATCH_FUNC(0x5438b0)
ExplosionPool_7A8::ExplosionPool_7A8()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_0_explosions); i++)
    {
        field_0_explosions[i].field_4_idx = i;
        field_780_bUsed[i] = 0;
    }

    gParticleInstCount_6FD5F4 = 0;
}

MATCH_FUNC(0x5438f0)
ExplosionPool_7A8::~ExplosionPool_7A8()
{
}

MATCH_FUNC(0x543980)
ExplosionPool_3D4::ExplosionPool_3D4()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_3C0_bUsed); i++)
    {
        field_0_explosions[i].field_4_idx = i;
        field_3C0_bUsed[i] = 0;
    }
    gParticleInstCount_6FD5F4 = 0;
}

MATCH_FUNC(0x5439c0)
ExplosionPool_3D4::~ExplosionPool_3D4()
{
}