#include "Wolfy_3D4.hpp"
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

DEFINE_GLOBAL(Wolfy_7A8*, gWolfy_7A8_6FD5F0, 0x6FD5F0);
DEFINE_GLOBAL(Wolfy_3D4*, gWolfy_3D4_6FD5EC, 0x6FD5EC);

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

DEFINE_GLOBAL_INIT(s16, gWolfyId_40_pool_623F18, 1, 0x623F18);

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
Wolfy_30::Wolfy_30()
{
    this->field_C_angle = 0;
    this->field_20 = 0;
    this->field_22 = 0;
    this->field_4_idx = 0;
    this->field_10_type_or_state = 0;
    this->field_14_pObj2C = 0;
    this->field_18_particle_cooldown = 0;
    this->field_1A_timer = 0;
    this->field_8_speed = kFP16Zero_6FD49C;
    this->field_20 = kAngZero_6FD5D4;
    this->field_22 = kAngZero_6FD5D4;
    this->field_24 = kFP16Zero_6FD49C;
    this->field_28 = kFP16Zero_6FD49C;
    this->field_C_angle = kAngZero_6FD5D4;
    this->field_1C = 0;
    this->field_6_id = 0;
    this->field_2C_ped_id = 0;
}

MATCH_FUNC(0x540a10)
Wolfy_30::~Wolfy_30()
{
    field_14_pObj2C = 0;
    field_1C = 0;
}

MATCH_FUNC(0x540d30)
void Wolfy_30::state_3_12_540D30(Ang16 ang, Fix16 pos)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the pos stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = pos;
    point.y = pos;
    point.RotateByAngle_40F6B0(ang + kAng180_6FD3EE);

    this->field_8_speed = pos;
    this->field_C_angle = ang;

    if (this->field_18_particle_cooldown == 0)
    {
        //pos = (int)&v27; // TODO: Field_20 wrong val ??
        Particle_4C* pNew = gParticle_8_6FD5E8->New_53E3C0(point.x, point.y, dword_6FD330, point.x, point.y, 0);
        if (pNew)
        {
            pNew->field_40_pUnknown = this;
            pNew->field_44 = this->field_6_id;
            pNew->field_20_speed = pos;
            pNew->field_24_angle = ang;
            pNew->field_34 = 0;
            pNew->field_46_sub_state = 0;
            pNew->field_2C_counter = 32;
            pNew->field_2E = 32;
            pNew->field_30_pNext->SetType_4206F0(8);
            pNew->field_38_state = 3;
            pNew->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 96);
            pNew->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew->field_30_pNext);
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
void Wolfy_30::state_4_540F90(Ang16 ang, Fix16 pos)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the pos stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = pos;
    point.y = pos;
    point.RotateByAngle_40F6B0(ang + kAng180_6FD3EE);

    this->field_8_speed = pos;
    this->field_C_angle = ang;

    if (this->field_18_particle_cooldown == 0)
    {
        //pos = (int)&v27; // TODO: Field_20 wrong val ??
        Particle_4C* pNew = gParticle_8_6FD5E8->New_53E3C0(point.x, point.y, dword_6FD330, point.x, point.y, 0);
        if (pNew)
        {
            pNew->field_40_pUnknown = this;
            pNew->field_44 = this->field_6_id;
            pNew->field_20_speed = pos;
            pNew->field_24_angle = ang;
            pNew->field_34 = 0;
            pNew->field_46_sub_state = 0;
            pNew->field_2C_counter = 32;
            pNew->field_2E = 32;
            pNew->field_30_pNext->SetType_4206F0(8);
            pNew->field_38_state = 4;
            pNew->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette);
            pNew->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew->field_30_pNext);
            this->field_18_particle_cooldown = gRng_6F6784.get_int_4F7AE0(2);
        }
    }
    else
    {
        this->field_18_particle_cooldown--;
    }
}

MATCH_FUNC(0x5411e0)
void Wolfy_30::state_13_14_5411E0(Ang16 ang, Fix16 pos)
{
    // Fix16_Point (has a destructor): the original sets an EH state for it. Zero-constructed
    // then assigned as in 9.6f: the pos stores are scheduled after the angle add.
    Fix16_Point point(Fix16(0), Fix16(0));
    point.x = pos;
    point.y = pos;
    point.RotateByAngle_40F6B0(ang + kAng180_6FD3EE);

    this->field_8_speed = pos;
    this->field_C_angle = ang;

    if (this->field_18_particle_cooldown == 0)
    {
        //pos = (int)&v27; // TODO: Field_20 wrong val ??
        Particle_4C* pNew = gParticle_8_6FD5E8->New_53E3C0(point.x, point.y, dword_6FD330, point.x, point.y, 0);
        if (pNew)
        {
            pNew->field_40_pUnknown = this;
            pNew->field_44 = this->field_6_id;
            pNew->field_20_speed = pos;
            pNew->field_24_angle = ang;
            pNew->field_34 = 0;
            pNew->field_46_sub_state = 0;
            pNew->field_2C_counter = 32;
            pNew->field_2E = 32;
            pNew->field_30_pNext->SetType_4206F0(8);
            pNew->field_38_state = 36;
            pNew->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette);
            pNew->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew->field_30_pNext);
            this->field_18_particle_cooldown = gRng_6F6784.get_int_4F7AE0(2);
        }
    }
    else
    {
        this->field_18_particle_cooldown--;
    }
}

MATCH_FUNC(0x541430)
void Wolfy_30::state_5_541430(Ang16 ang, Fix16 pos)
{

    Fix16_Point p(Fix16(0), Fix16(0));
    p.x = pos;
    p.y = pos;
    p.RotateByAngle_40F6B0(ang + kAng180_6FD3EE);

    this->field_8_speed = pos;
    this->field_C_angle = ang;

    if (field_14_pObj2C->sub_5290F0() == kFP16Zero_6FD49C && this->field_1A_timer == 9999)
    {
        this->field_1A_timer = 20;
    }

    if (!this->field_18_particle_cooldown)
    {
        Particle_4C* pNew = gParticle_8_6FD5E8->New_53E3C0(p.x, p.y, dword_6FD330, p.x, p.y, 0);
        if (pNew)
        {
            pNew->field_40_pUnknown = this;
            pNew->field_44 = field_6_id;
            pNew->field_20_speed = pos;
            pNew->field_24_angle = ang;
            pNew->field_34 = 0;
            pNew->field_46_sub_state = 0;
            pNew->field_2C_counter = 32;
            pNew->field_2E = 32;
            pNew->field_30_pNext->SetType_4206F0(8);
            pNew->field_38_state = 5;
            pNew->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 96);
            pNew->field_30_pNext->set_xyz_lazy_420600(field_14_pObj2C->field_4->field_14_xy.x,
                                                      field_14_pObj2C->field_4->field_14_xy.y,
                                                      field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew->field_30_pNext);
            this->field_18_particle_cooldown = gRng_6F6784.get_int_4F7AE0(2);
            pNew->field_30_pNext->SetFlags_4337D0(2, 20);
            pNew->field_30_pNext->Set_2C_0x4_Flag_4337F0();
        }
    }
    else
    {
        this->field_18_particle_cooldown--;
    }
}

MATCH_FUNC(0x541680)
Fix16 Wolfy_30::sub_541680()
{
    Fix16 r;
    switch (this->field_10_type_or_state)
    {
        case 18:
        case 22:
        case 23:
        case 24:
        case 25:
            r = kFP16Half_6FD39C;
            break;
        case 33:
            r = kFP16ThreeQuarters_6FD370;
            break;
        case 19:
        case 32:
            r = kFP16One_6FD4A0;
            break;
        case 20:
            r = kFP16Two_6FD4A4;
            break;
        default:
            break; // r stays uninitialised, as in the original
    }
    return r;
}


MATCH_FUNC(0x541710)
Fix16 Wolfy_30::sub_541710()
{
    Fix16 r;
    switch (this->field_10_type_or_state)
    {
        case 18:
        case 19:
        case 20:
        case 22:
        case 23:
        case 24:
        case 25:
        case 32:
        case 33:
            r = dword_6FD2F0;
            break;
        default:
            break;
    }
    return r;
}

MATCH_FUNC(0x541760)
void Wolfy_30::sub_541760()
{
    if (field_10_type_or_state != 18 && field_10_type_or_state != 32)
    {
        if (gParticle_4C_Pool_6FD5E4->field_0_pStart)
        {
            Particle_4C* pNew = gParticle_4C_Pool_6FD5E4->Allocate();
            pNew->field_46_sub_state = 0;
            pNew->field_38_state = 29;
            pNew->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pNew->field_30_pNext->SetType_4206F0(8);
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
void Wolfy_30::TimerAfter50Handler_541850(u16 timerVal)
{
    struct_4 collision_list;

    Fix16 zoff = Wolfy_30::sub_541710();

    Fix16 f28 = Wolfy_30::sub_541680();

    this->field_28 = f28;

    Fix16 new_left = field_14_pObj2C->field_4->field_14_xy.x - field_28 * kFP16Two_6FD4A4;
    Fix16 new_right = field_14_pObj2C->field_4->field_14_xy.x + field_28 * kFP16Two_6FD4A4;
    Fix16 new_top = field_14_pObj2C->field_4->field_14_xy.y - field_28 * kFP16Two_6FD4A4;
    Fix16 new_bottom = field_14_pObj2C->field_4->field_14_xy.y + field_28 * kFP16Two_6FD4A4;
    Fix16 zm = field_14_pObj2C->field_4->field_1C_zpos - zoff;
    Fix16 zp = field_14_pObj2C->field_4->field_1C_zpos + zoff;

    if (this->field_1A_timer == 99 && unk_6FD5F6 == 1)
    {
        sub_541760();
    }

    Fix16_Rect rect;
    rect.SetRect_41E350(new_left, new_right, new_top, new_bottom);
    rect.SetHiLowZ_41E370(zm, zp);

    if (gPurpleDoom_1_679208->CollectRectCollisions_477F30(&rect, 0, 0, field_14_pObj2C->field_4, &collision_list))
    {
        while (collision_list.field_0_p18)
        {
            Sprite* pCollisionSprite = collision_list.PopFrontSprite_5A6DA0();
            Char_B4* pB4 = pCollisionSprite->AsCharB4_40FEA0();
            if (pB4)
            {
                if (timerVal > 50u && pB4->get_ped_state_1_48A4C0() != ped_state_1::immobilized_8)
                {
                    s32 ped_id = gVarrok_7F8_703398->GetPedId_420F10(this->field_14_pObj2C->field_26_varrok_idx);
                    if (!ped_id)
                    {
                        pB4->field_7C_pPed->field_204_killer_id = this->field_2C_ped_id;
                    }
                    else
                    {
                        pB4->field_7C_pPed->field_204_killer_id = ped_id;
                    }
                    pB4->field_7C_pPed->field_290 = 4;
                    pB4->field_7C_pPed->field_264_killer_id_timer = 50;

                    Fix16 dx = pCollisionSprite->field_14_xy.x - this->field_14_pObj2C->field_4->field_14_xy.x;
                    Fix16 dy = pCollisionSprite->field_14_xy.y - this->field_14_pObj2C->field_4->field_14_xy.y;

                    Ang16 ang;
                    ang = Fix16::atan2_fixed_405320(dy, dx);

                    Fix16 cur_max = Fix16::MaxAbsDistance_42A6B0(pCollisionSprite->field_14_xy.x, pCollisionSprite->field_14_xy.y, field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y);
                    if (cur_max > this->field_28)
                    {
                        if (timerVal < 70u)
                        {
                            pB4->HandleGenericImpact_553E00(ang, dword_6FD2E8 + dword_6FD46C, kFP16Zero_6FD49C, 0);
                        }
                    }
                    else
                    {
                        char_type a5;
                        Fix16 v30;
                        if (cur_max < (kFP16Half_6FD39C * this->field_28))
                        {
                            v30 = dword_6FD2F4;
                            a5 = 2;
                        }
                        else
                        {
                            v30 = dword_6FD2E8;
                            a5 = 1;
                        }
                        pB4->HandleGenericImpact_553E00(ang, dword_6FD484, v30, a5);
                    }
                }
            }
            else
            {
                Car_BC* pCar = pCollisionSprite->AsCar_40FEB0();
                if (pCar)
                {
                    if ((timerVal > 50u && timerVal < 60u) || (timerVal > 80u && timerVal < 90u))
                    {
                        if (!pCar->IsMaxDamage_40F890() && !pCar->IsTrainModel_403BA0() && !pCar->sub_43B850(field_10_type_or_state))
                        {
                            if (Fix16::MaxAbsDistance_42A6B0(pCollisionSprite->field_14_xy.x, pCollisionSprite->field_14_xy.y, field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y) <= this->field_28)
                            {
                                // 9.6f: Varrok_7F8::GetPedId_420F10 (inlined, using it here makes the diff worse)
                                s32 ped_id_ = gVarrok_7F8_703398->field_0_entries[this->field_14_pObj2C->field_26_varrok_idx].field_0_ped_id;
                                if (!ped_id_)
                                {
                                    pCar->field_70_exploder_ped_id = this->field_2C_ped_id;
                                }
                                else
                                {
                                    pCar->field_70_exploder_ped_id = ped_id_;
                                }
                                pCar->field_90 = 4;
                                pCar->field_94_exploder_timer = 50;
                                s16 damage = pCar->AccumulateDamage_43DA90(32000, &kZeroPoint_6FD570);
                                if (pCar->field_70_exploder_ped_id)
                                {
                                    if (damage > 0)
                                    {
                                        Ped* pPed_ = gPedManager_6787BC->PedById(pCar->field_70_exploder_ped_id);
                                        if (pPed_)
                                        {
                                            if (pPed_->IsField238_45EDE0(2))
                                            {
                                                pPed_->field_15C_player->field_2D4_scores.sub_593150(pCar, 1);
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
                    else if (timerVal == 99)
                    {
                        if (Fix16::MaxAbsDistance_42A6B0(pCollisionSprite->field_14_xy.x, pCollisionSprite->field_14_xy.y, field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y) <= this->field_28)
                        {
                            pCar->ApplyExplosionImpulse_443710(&this->field_14_pObj2C->field_4->get_x_y_443580());
                        }
                    }
                }
                else
                {
                    Object_2C* o2C = pCollisionSprite->As2C_40FEC0();
                    if (timerVal > 50u && timerVal < 60u)
                    {
                        o2C->sub_525190(this->field_14_pObj2C->field_26_varrok_idx);
                    }
                }
            }
        }
    }
}

// 9.6f 0x48EB00
MATCH_FUNC(0x541d60)
void Wolfy_30::state_18_33_541D60()
{
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0())
    {
        if ((u16)field_1A_timer > 0x52u)
        {
            if ((u16)field_1A_timer > 0x5Au)
            {
                Fix16 radius;
                radius = this->field_24 * Fix16(gRng_6F6784.get_int_4F7AE0(8));

                this->field_22 = Ang16::Fix16_To_Ang16_inlined_40F540(dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(360)));

                // 9.6f calls Ang16::PolarToCartesian_41FC20
                Ang16::PolarToCartesian_41FC20(field_22, radius, stru_6FD388, stru_6FD38C);

                // NOTE: This proves these 2 vars are not a Fix16_Point
                stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x + stru_6FD388;
                stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y + stru_6FD38C;

                Particle_4C* pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
                pNew4C->field_46_sub_state = 0;
                pNew4C->field_38_state = 18;
                pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
                pNew4C->field_30_pNext->SetType_4206F0(8);
                pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
                pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
                pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
                pNew4C->field_48_timer = 1;
            }
            else
            {
                stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x;
                stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y;

                Particle_4C* pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
                pNew4C->field_46_sub_state = 0;
                pNew4C->field_38_state = 18;
                pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
                pNew4C->field_30_pNext->SetType_4206F0(8);
                pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
                pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
                pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
                pNew4C->field_30_pNext->ApplyScaleToDimensions_59E4C0(kFP16Quarter_6FD2EC, 0);
                pNew4C->field_48_timer = 5;
            }
        }
    }
}

MATCH_FUNC(0x542060)
void Wolfy_30::state_19_32_542060()
{
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0())
    {
        if (this->field_1A_timer > 8u)
        {
            Fix16 v24;
            v24 = this->field_24 * Fix16(gRng_6F6784.get_int_4F7AE0(48));
            this->field_22 = Ang16::Fix16_To_Ang16_inlined_40F540(dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(360)));

            // 9.6f calls Ang16::PolarToCartesian_41FC20
            Ang16::PolarToCartesian_41FC20(field_22, v24, stru_6FD388, stru_6FD38C);

            stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x + stru_6FD388;
            stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y + stru_6FD38C;

            Particle_4C* pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
            pNew4C->field_46_sub_state = 0;
            pNew4C->field_38_state = 19;
            pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew4C->field_40_pUnknown = this;
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
            pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, this->field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
            pNew4C->field_30_pNext->ApplyScaleToDimensions_59E4C0(kFP16Half_6FD39C + kFP16One_6FD4A0, 0);
            pNew4C->field_48_timer = 1;
        }
        else
        {
            stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x;
            stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y;

            Particle_4C* pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
            pNew4C->field_46_sub_state = 0;
            pNew4C->field_38_state = 19;
            pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 20);
            pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
            pNew4C->field_48_timer = 5;
        }
    }
}

MATCH_FUNC(0x542340)
void Wolfy_30::state_20_542340()
{
    if (gParticle_4C_Pool_6FD5E4->has_pStart_48A8F0())
    {
        if (this->field_1A_timer > 8u)
        {
            Fix16 v24;
            v24 = this->field_24 * Fix16(gRng_6F6784.get_int_4F7AE0(80));
            this->field_22 = Ang16::Fix16_To_Ang16_inlined_40F540(dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(360)));

            // 9.6f calls Ang16::PolarToCartesian_41FC20
            Ang16::PolarToCartesian_41FC20(field_22, v24, stru_6FD388, stru_6FD38C);

            stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x + stru_6FD388;
            stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y + stru_6FD38C;

            Particle_4C* pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
            pNew4C->field_46_sub_state = 0;
            pNew4C->field_38_state = 20;
            pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 56);
            pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
            pNew4C->field_48_timer = 1;
        }
        else
        {
            stru_6FD388 = this->field_14_pObj2C->field_4->field_14_xy.x;
            stru_6FD38C = this->field_14_pObj2C->field_4->field_14_xy.y;

            Particle_4C* pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
            pNew4C->field_46_sub_state = 0;
            pNew4C->field_38_state = 20;
            pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
            pNew4C->field_30_pNext->SetType_4206F0(8);
            pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 56);
            pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
            pNew4C->field_48_timer = 5;
        }
    }
}

MATCH_FUNC(0x542790)
void Wolfy_30::state_18_19_20_32_33_542790()
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
        switch (this->field_10_type_or_state)
        {
            case 18:
            case 33:
                state_18_33_541D60();
                break;
            case 19:
            case 32:
                state_19_32_542060();
                unk_6FD5F6 = 1;
                break;
            case 20:
                state_20_542340();
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
                Object_2C* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(Fix16(113), Fix16(145), 2, kAngZero_6FD5D4, 5, field_2C_ped_id);
                if (pExplosion)
                {
                    Object_2C* pBlast = gObject_5C_6F8F84->NewUnknown_52A240(127,
                                                                             field_14_pObj2C->field_4->field_14_xy.x,
                                                                             field_14_pObj2C->field_4->field_14_xy.y,
                                                                             field_14_pObj2C->field_4->field_1C_zpos,
                                                                             this->field_22,
                                                                             kAngZero_6FD5D4,
                                                                             dword_6FD484,
                                                                             -dword_6FD540,
                                                                             dword_6FD2F0);
                    pBlast->field_4->DispatchCollisionEvent_5A3100(pExplosion->field_4, 0, 0, kAngZero_6FD5D4);

                    Object_2C* pLight = gObject_5C_6F8F84->NewLight_529A40(94, 138, 2, 0xFF8000, 3, 255);
                    pBlast->field_4->DispatchCollisionEvent_5A3100(pLight->field_4, 0, 0, kAngZero_6FD5D4);
                }
                break;
            }

            default:
                break;
        }
    }

    if (this->field_1A_timer > 0x1Eu)
    {
        Fix16 v21 = (dword_6FD448 * kFP16Two_6FD4A4);
        if (this->field_24 > v21)
        {
            this->field_24 = v21;
        }
        this->field_24 += (dword_6FD540 / kFP16Two_6FD4A4);
    }
    else
    {
        if (this->field_24 > dword_6FD448)
        {
            this->field_24 = dword_6FD448;
        }
        this->field_24 -= (dword_6FD540 / kFP16Two_6FD4A4);
    }

    if (this->field_1A_timer > 50u)
    {
        TimerAfter50Handler_541850(this->field_1A_timer);
    }

    if (this->field_1A_timer == 99)
    {
        // TODO: Arg order correct?
        gGame_0x40_67E008->ShakeCamerasAtPos_4B9790(8, this->field_14_pObj2C->field_4->field_14_xy.x, this->field_14_pObj2C->field_4->field_14_xy.y);
    }

    if (this->field_1A_timer != 9999)
    {
        this->field_1A_timer--;
    }

    if (this->field_1A_timer > 9999u)
    {
        this->field_1A_timer = 1;
    }
}

MATCH_FUNC(0x542e30)
void Wolfy_30::state_22_23_24_25_542E30(char_type a2)
{
    Sprite* p2CSprite = this->field_14_pObj2C->field_4;
    if (p2CSprite->field_14_xy.x < Fix16(0x3F8000, 0) && p2CSprite->field_14_xy.x > kFP16One_6FD4A0 &&
        p2CSprite->field_14_xy.y < Fix16(0x3F8000, 0) && p2CSprite->field_14_xy.y > kFP16One_6FD4A0)
    {
        unk_6FD5F6 = 0;
        for (u8 i = 0; i < 2u; i++)
        {
            if (gParticle_4C_Pool_6FD5E4->field_0_pStart)
            {
                Particle_4C* pNew4C = gParticle_4C_Pool_6FD5E4->Allocate();
                pNew4C->field_46_sub_state = 0;

                switch ((u8)a2)
                {
                    case 0:
                    {
                        pNew4C->field_38_state = 24;
                        this->field_22 = (kAng135_6FD40C + dword_6FD350) + Ang16::Fix16_To_Ang16_40F540((dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(45))));

                        Ang16::PolarToCartesian_41FC20(field_22, dword_6FD540, stru_6FD388, stru_6FD38C);

                        stru_6FD388 += this->field_14_pObj2C->field_4->field_14_xy.x;
                        stru_6FD38C += this->field_14_pObj2C->field_4->field_14_xy.y;
                        break;
                    }

                    case 1:
                    {
                        pNew4C->field_38_state = 25;
                        this->field_22 = (kAng315_6FD418 + dword_6FD350) + Ang16::Fix16_To_Ang16_40F540((dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(90))));


                        Ang16::PolarToCartesian_41FC20(field_22, dword_6FD540, stru_6FD388, stru_6FD38C);

                        stru_6FD388 += this->field_14_pObj2C->field_4->field_14_xy.x;
                        stru_6FD38C += this->field_14_pObj2C->field_4->field_14_xy.y;
                        break;
                    }

                    case 2:
                    {
                        pNew4C->field_38_state = 23;
                        this->field_22 = (kAng225_6FD3E0 + dword_6FD350).Add_ool(Ang16::Fix16_To_Ang16_40F540((dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(90)))));


                        Ang16::PolarToCartesian_41FC20(field_22, dword_6FD540, stru_6FD388, stru_6FD38C);

                        stru_6FD388 += this->field_14_pObj2C->field_4->field_14_xy.x;
                        stru_6FD38C += this->field_14_pObj2C->field_4->field_14_xy.y;
                        break;
                    }

                    case 3:
                    {
                        pNew4C->field_38_state = 22;
                        this->field_22 = kAng45_6FD35C.Add_ool(dword_6FD350).Add_ool(Ang16::Fix16_To_Ang16_ool_40F540((dword_6FD448 * Fix16(gRng_6F6784.get_int_4F7AE0(90)))));


                        Ang16::PolarToCartesian_41FC20(field_22, dword_6FD540, stru_6FD388, stru_6FD38C);

                        stru_6FD388 += this->field_14_pObj2C->field_4->field_14_xy.x;
                        stru_6FD38C += this->field_14_pObj2C->field_4->field_14_xy.y;
                        break;
                    }

                    default:
                        break;
                }

                pNew4C->field_30_pNext = gSprite_Pool_703818->get_new_sprite();
                pNew4C->field_30_pNext->SetType_4206F0(8);
                pNew4C->field_48_timer = 0;
                pNew4C->field_46_sub_state = 0;
                pNew4C->field_24_angle = this->field_22;

                if (this->field_1A_timer < 60u && this->field_1A_timer < 30u)
                {
                    i = 4;
                }

                pNew4C->field_20_speed = (dword_6FD548 * Fix16(gRng_6F6784.get_int_4F7AE0(field_1A_timer)));
                pNew4C->field_30_pNext->set_id_lazy_4206C0(gPhi_8CA8_6FCF00->field_8CA4_def112_sprite_palette + 40);

                if (field_14_pObj2C->field_4->field_1C_zpos + kFP16One_6FD4A0 >= kFP16Eight_6FD4C0)
                {
                    pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos);
                }
                else
                {
                    pNew4C->field_30_pNext->set_xyz_lazy_420600(stru_6FD388, stru_6FD38C, field_14_pObj2C->field_4->field_1C_zpos + kFP16One_6FD4A0);
                }
                gPurpleDoom_3_679210->AddToSingleBucket_477AE0(pNew4C->field_30_pNext);
                pNew4C->field_30_pNext->Set_2C_0x4_Flag_4337F0();
            }
        }

        if (this->field_1A_timer == 99)
        {
            gGame_0x40_67E008->ShakeCamerasAtPos_4B9790(8, field_14_pObj2C->field_4->field_14_xy.x, field_14_pObj2C->field_4->field_14_xy.y);
        }

        if (this->field_1A_timer > 50u)
        {
            TimerAfter50Handler_541850(this->field_1A_timer);
        }

        if (this->field_1A_timer != 9999)
        {
            if (this->field_1A_timer > 60u)
            {
                this->field_1A_timer--;
            }

            if (this->field_1A_timer > 9999u)
            {
                this->field_1A_timer = 1;
            }
        }
    }
}

MATCH_FUNC(0x5434a0)
char_type Wolfy_30::Update_5434A0(Fix16 speed, Ang16 ang)
{
    u16 timer = this->field_1A_timer;
    if (timer != 9999)
    {
        if (timer > 0)
        {
            this->field_1A_timer = timer - 1;
        }
    }

    if (!this->field_1A_timer)
    {
        Wolfy_30::DeInit_543610();
        return 1;
    }

    if (bSkip_particles_67D64D)
    {
        return 1;
    }

    switch (this->field_10_type_or_state)
    {
        case 3:
        case 12:
            Wolfy_30::state_3_12_540D30(ang, speed);
            return 0;
        case 13:
        case 14:
            Wolfy_30::state_13_14_5411E0(ang, speed);
            return 0;
        case 4:
            Wolfy_30::state_4_540F90(ang, speed);
            return 0;
        case 5:
            Wolfy_30::state_5_541430(ang, speed);
            return 0;
        case 18:
        case 19:
        case 20:
        case 32:
        case 33:
            Wolfy_30::state_18_19_20_32_33_542790();
            return 0;
        case 24:
            Wolfy_30::state_22_23_24_25_542E30(0);
            return 0;
        case 25:
            Wolfy_30::state_22_23_24_25_542E30(1);
            return 0;
        case 23:
            Wolfy_30::state_22_23_24_25_542E30(2);
            return 0;
        case 22:
            Wolfy_30::state_22_23_24_25_542E30(3);
            return 0;
    }
    return 0;
}

MATCH_FUNC(0x5435d0)
char_type Wolfy_30::IsState_5435D0()
{
    switch (field_10_type_or_state)
    {
        case 3:
        case 4:
        case 5:
        case 12:
        case 13:
        case 14:
            return 1;
        default:
            return 0;
    }
}

MATCH_FUNC(0x543610)
void Wolfy_30::DeInit_543610()
{
    this->field_6_id = 0;
    if (field_0_bIn20Pool == 0)
    {
        gWolfy_7A8_6FD5F0->field_780_bUsed[this->field_4_idx] = 0;
    }
    else
    {
        gWolfy_3D4_6FD5EC->field_3C0_bUsed[this->field_4_idx] = 0;
    }
}

MATCH_FUNC(0x543650)
void Wolfy_30::Init_543650()
{
    this->field_10_type_or_state = 0;
    this->field_18_particle_cooldown = 0;
    this->field_24 = 0;
    this->field_22 = kAngZero_6FD5D4;
    this->field_1A_timer = 200;
    this->field_14_pObj2C = 0;
    this->field_2C_ped_id = 0;
    this->field_0_bIn20Pool = 0;
}

MATCH_FUNC(0x543680)
void Wolfy_30::Set_Obj2C_543680(Object_2C* a2)
{
    this->field_14_pObj2C = a2;
}

WIP_FUNC(0x543690)
void Wolfy_7A8::sub_543690()
{
    WIP_IMPLEMENTED;

    u8 smallestVal = 99;
    u8 smallestVal_idx = 99;
    u8 currentVal1 = 0;
    u8 next_idx = 0;
    u8 last_idx = 0;
    do
    {
        if (this->field_780_bUsed[last_idx] == 1)
        {
            Wolfy_30* pObj = &this->field_0[last_idx];
            // Each case written out on its own: merged labels give a byte index table, the
            // original has one dword entry per case. Cases 1 and 39 keep the range.
            switch (pObj->field_10_type_or_state)
            {
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 21:
                    break;
                case 31:
                    break;
                case 34:
                    break;
                case 5:
                    currentVal1 = 2;
                    break;
                case 28:
                    currentVal1 = 2;
                    break;
                case 29:
                    currentVal1 = 2;
                    break;
                case 30:
                    currentVal1 = 2;
                    break;
                case 13:
                    currentVal1 = 4;
                    break;
                case 12:
                    currentVal1 = 5;
                    break;
                case 14:
                    currentVal1 = 5;
                    break;
                case 15:
                    currentVal1 = 5;
                    break;
                case 16:
                    currentVal1 = 6;
                    break;
                case 17:
                    currentVal1 = 6;
                    break;
                case 18:
                    if (pObj->field_1A_timer < 82u)
                    {
                        currentVal1 = 3;
                    }
                    break;
                case 33:
                    if (pObj->field_1A_timer < 82u)
                    {
                        currentVal1 = 3;
                    }
                    break;
                case 19:
                    if (pObj->field_1A_timer < 50u)
                    {
                        currentVal1 = 3;
                    }
                    break;
                case 20:
                    if (pObj->field_1A_timer < 50u)
                    {
                        currentVal1 = 3;
                    }
                    break;
                case 32:
                    if (pObj->field_1A_timer < 50u)
                    {
                        currentVal1 = 3;
                    }
                    break;
                case 22:
                    currentVal1 = 3;
                    break;
                case 23:
                    currentVal1 = 3;
                    break;
                case 24:
                    currentVal1 = 3;
                    break;
                case 25:
                    currentVal1 = 3;
                    break;
                case 1:
                    currentVal1 = 1;
                    break;
                case 39:
                    currentVal1 = 1;
                    break;
                default:
                    currentVal1 = 1;
                    break;
            }

            if (currentVal1 == 1)
            {
                this->field_0[last_idx].field_1A_timer = 0;
                return;
            }

            if (currentVal1 < smallestVal)
            {
                smallestVal = currentVal1;
                smallestVal_idx = next_idx;
            }
        }
        last_idx = ++next_idx;
    } while (next_idx < 40u);
    this->field_0[smallestVal_idx].field_1A_timer = 0;
}

MATCH_FUNC(0x543800)
Wolfy_30* Wolfy_7A8::New_40_543800()
{
    // 9.6f has the init block twice, 10.5 merges both into one block. Indexing field_0 at each
    // use (no pNew local) gives both copies the same registers, so they merge completely.
    u8 idx;
    for (idx = 0; idx < 20; idx++)
    {
        if (!this->field_780_bUsed[idx])
        {
            this->field_0[idx].Init_543650();
            this->field_0[idx].field_4_idx = idx;
            this->field_0[idx].field_6_id = gWolfyId_40_pool_623F18;
            this->field_0[idx].field_0_bIn20Pool = 0;
            gWolfyId_40_pool_623F18++;
            this->field_780_bUsed[idx] = 1;
            return &this->field_0[idx];
        }
    }

    sub_543690();

    for (idx = 0; idx < 40; idx++)
    {
        if (!this->field_780_bUsed[idx])
        {
            this->field_0[idx].Init_543650();
            this->field_0[idx].field_4_idx = idx;
            this->field_0[idx].field_6_id = gWolfyId_40_pool_623F18;
            this->field_0[idx].field_0_bIn20Pool = 0;
            gWolfyId_40_pool_623F18++;
            this->field_780_bUsed[idx] = 1;
            return &this->field_0[idx];
        }
    }
    return 0;
}

MATCH_FUNC(0x5438b0)
Wolfy_7A8::Wolfy_7A8()
{
    for (u8 i = 0; i < 40; i++)
    {
        field_0[i].field_4_idx = i;
        field_780_bUsed[i] = 0;
    }

    gParticleInstCount_6FD5F4 = 0;
}

MATCH_FUNC(0x5438f0)
Wolfy_7A8::~Wolfy_7A8()
{
}

MATCH_FUNC(0x543980)
Wolfy_3D4::Wolfy_3D4()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_3C0_bUsed); i++)
    {
        field_0[i].field_4_idx = i;
        field_3C0_bUsed[i] = 0;
    }
    gParticleInstCount_6FD5F4 = 0;
}

MATCH_FUNC(0x5439c0)
Wolfy_3D4::~Wolfy_3D4()
{
}