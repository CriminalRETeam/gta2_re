// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO dword_706EB8
// This TU's copy of the Fix16_Rect::ComputeCollisionPrism_4204D0 half height (see Fix16_Rect.hpp)
#define FIX16_RECT_HALF_HEIGHT dword_706CC8
#include "Function.hpp"
#include "fix16.hpp"
EXTERN_GLOBAL(Fix16, dword_706CC8);

#include "Weapon_30.hpp"
#include "CarPhysics_B0.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "Object_8.hpp"
#include "Particle_8.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Shooey_CC.hpp"
#include "Weapon_8.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "map_0x370.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "Rozza_C88.hpp"
#include "Police_7B8.hpp"
#include "Car_BC.hpp"
#include "eager_benz.hpp"
#include "Fix16_Rect.hpp"
#include "rng.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "PurpleDoom.hpp"

DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_706CF4, Fix16(0x1000, 0), 0x706CF4);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_706EC0, Fix16(0x8000, 0), 0x706EC0);
DEFINE_GLOBAL(bool, bAllowFlameSegment_706D60, 0x706D60);

// TODO: Check these for inits
DEFINE_GLOBAL_INIT(Fix16, dword_706FF4, Fix16(0x100, 0), 0x706FF4);
DEFINE_GLOBAL_INIT(Fix16, dword_706FEC, Fix16(0x1200, 0), 0x706FEC);
DEFINE_GLOBAL(Fix16, dword_706EB8, 0x706EB8);
DEFINE_GLOBAL_INIT(Fix16, dword_706EBC, Fix16(1), 0x706EBC);
DEFINE_GLOBAL_INIT(Fix16, dword_706EC4, Fix16(3), 0x706EC4);
DEFINE_GLOBAL_INIT(Ang16, word_706D6C, Ang16(180), 0x706D6C);
DEFINE_GLOBAL_INIT(Ang16, word_706E28, Ang16(1260), 0x706E28);
DEFINE_GLOBAL(u8, byte_706C94, 0x706C94);
DEFINE_GLOBAL(Ang16, word_707004, 0x707004);
DEFINE_GLOBAL(Fix16_Point_POD, stru_706E58, 0x706E58);
DEFINE_GLOBAL(Fix16_Point, stru_706F90, 0x706F90);
DEFINE_GLOBAL_INIT(Fix16, dword_706CC8, Fix16(0x800, 0), 0x706CC8);
DEFINE_GLOBAL_INIT(Fix16, k_dword_706EDC, Fix16(0x20000, 0), 0x706EDC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_706F70, Fix16(0x100, 0), 0x706F70);
DEFINE_GLOBAL_INIT(Fix16, dword_706DCC, Fix16(0xFFFFFD00, 0), 0x706DCC);
DEFINE_GLOBAL_INIT(Fix16, dword_706FD0, Fix16(0x100, 0), 0x706FD0);
DEFINE_GLOBAL_INIT(Fix16, dword_706EA4, Fix16(0x600, 0), 0x706EA4);
DEFINE_GLOBAL_INIT(Fix16, dword_706EE8, Fix16(0xFFFFEE00, 0), 0x706EE8);
DEFINE_GLOBAL_INIT(Fix16, dword_706E7C, Fix16(0x1EB, 0), 0x706E7C);
DEFINE_GLOBAL_INIT(Fix16, dword_706CF0, Fix16(0x666, 0), 0x706CF0);
DEFINE_GLOBAL_INIT(Fix16, dword_706E80, Fix16(0x147, 0), 0x706E80);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_706DA8, Fix16(0.5), 0x706DA8);
DEFINE_GLOBAL_INIT(Fix16, dword_706E74, Fix16(0xA3, 0), 0x706E74);
DEFINE_GLOBAL_INIT(Fix16, dword_706F64, Fix16(0x20, 0), 0x706F64);
DEFINE_GLOBAL_INIT(Fix16, dword_706C8C, Fix16(0x340, 0), 0x706C8C);

DEFINE_GLOBAL_INIT(Fix16, k_dword_706EB4, k_dword_706F70 * 24, 0x706EB4);
DEFINE_GLOBAL_INIT(Fix16, k_dword_706E6C, k_dword_706F70 * 10, 0x706E6C);
DEFINE_GLOBAL_INIT(Fix16, gTankCannonLength_706E20, k_dword_706F70 * 30, 0x706E20);
DEFINE_GLOBAL_INIT(Fix16, dword_706D88, k_dword_706F70 * 8, 0x706D88);

// Defined in Weapon_8.cpp: with the definition (and its dynamic initialiser) in this TU, VC6
// loads it with a 32-bit mov in dual_pistol_5DDA70 instead of the original's 16-bit mov/sub.
EXTERN_GLOBAL(Ang16, word_706D5E);
DEFINE_GLOBAL_INIT(Ang16, word_707002, Ang16(24), 0x707002);
DEFINE_GLOBAL_INIT(Ang16, word_706D5C, Ang16(96), 0x706D5C);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_707006, Ang16(0), 0x707006);

// TODO: move
EXTERN_GLOBAL(Shooey_CC*, gShooey_CC_67A4B8);

MATCH_FUNC(0x5DCD10)
Weapon_30::Weapon_30()
{
    field_0_ammo = 0;
    field_24_pPed = 0;
    field_14_car = 0;
    field_2_reload_speed = 0;
    field_4 = 0;
    mpNext = 0;
    field_1C_idx = 0;
    field_10 = 0;
    field_8 = 0;
    field_C = -1;
    field_20 = 0;
    field_21 = 0;
    field_2C_shot_fired = 0;
    field_28_pSound = 0;
}

MATCH_FUNC(0x5DCD50)
Weapon_30::~Weapon_30()
{
    field_24_pPed = 0;
    mpNext = 0;
    field_14_car = 0;
    field_8 = 0;
    if (field_28_pSound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_28_pSound);
        field_28_pSound = 0;
    }
}

MATCH_FUNC(0x5DCD90)
void Weapon_30::init_5DCD90()
{
    field_24_pPed = 0;
    field_14_car = 0;
    field_1C_idx = 0;
    field_0_ammo = 0;
    field_2_reload_speed = 0;
    field_4 = 0;
    field_21 = 0;
    field_8 = 0;
    field_C = -1;
    field_20 = 0;
    field_2C_shot_fired = 0;
    if (!field_28_pSound && !bSkip_audio_67D6BE)
    {
        field_28_pSound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::Weapon_30_7);
    }
}

MATCH_FUNC(0x5DCDE0)
void Weapon_30::PoolDeallocate()
{
    init_5DCD90();

    field_8 = 0;

    if (field_28_pSound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_28_pSound);
        field_28_pSound = 0;
    }
}

MATCH_FUNC(0x5dce20)
void Weapon_30::add_ammo_5DCE20(u8 a2)
{
    field_0_ammo = a2 * 10;
}

MATCH_FUNC(0x5dce40)
char_type Weapon_30::add_ammo_capped_5DCE40(u8 to_add)
{
    s32 cap_total = max_ammo_capacity_5FF75C[field_1C_idx] * 10;
    if (is_infinite_ammo_4A4FA0())
    {
        return 0;
    }

    u16 cur_amount = field_0_ammo;
    if (cur_amount == cap_total)
    {
        return 0;
    }

    s32 new_amount = cur_amount + (to_add * 10);
    if (new_amount > cap_total)
    {
        field_0_ammo = cap_total;
    }
    else
    {
        field_0_ammo = new_amount;
    }
    return 1;
}

MATCH_FUNC(0x5dcea0)
bool Weapon_30::is_max_capacity_5DCEA0()
{
    return this->field_0_ammo == 10 * max_ammo_capacity_5FF75C[this->field_1C_idx];
}

MATCH_FUNC(0x5dcef0)
bool Weapon_30::sub_5DCEF0()
{
    bool result;
    switch (field_1C_idx)
    {
        case weapon_type::car_bomb:
        case weapon_type::oil_stain:
        case weapon_type::car_mines:
        case weapon_type::fire_truck_gun:
        case weapon_type::weapon_0x17:
            result = 0;
            break;
        case weapon_type::car_smg:
        case weapon_type::tank_main_gun:
        case weapon_type::fire_truck_flamethrower:
        case weapon_type::army_gun_jeep:
            result = 1;
            break;
        default:
            result = field_24_pPed->field_16C_car == 0;
            break;
    }
    return result;
}

MATCH_FUNC(0x5dcf40)
void Weapon_30::TickReloadSpeed_5DCF40()
{
    Player* pPlayer = field_24_pPed->field_15C_player;
    if (pPlayer)
    {
        if (pPlayer->field_6F4_power_up_timers[power_up_indices::FastReload_8])
        {
            field_2_reload_speed /= 2;
        }
    }
}

// 9.6f 0x4CDA90
MATCH_FUNC(0x5dcf60)
Object_2C* Weapon_30::spawn_bullet_5DCF60(s32 bullet_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rot, Fix16_Point& speed)
{
    // probe_x/probe_y are assigned, not initialised: initialised, VC6 folds them into the call
    // arguments and computes y first
    Fix16 probe_x;
    Fix16 probe_y;

    Sprite* p5CSprite = gObject_5C_6F8F84->field_58_collision_probe_sprite;
    Object_2C* pNewBullet = gObject_5C_6F8F84->NewPhysicsObj_5299B0(bullet_type, xpos, ypos, zpos, rot);

    probe_x = field_24_pPed->get_cam_x() + (xpos - field_24_pPed->get_cam_x()) / kFP16Two_706EC0;
    probe_y = field_24_pPed->get_cam_y() + (ypos - field_24_pPed->get_cam_y()) / kFP16Two_706EC0;
    p5CSprite->set_xyz_lazy_420600(probe_x, probe_y, zpos);

    p5CSprite->set_ang_lazy_420690(pNewBullet->field_4->field_0);

    p5CSprite->AllocInternal_59F950(pNewBullet->field_8->field_0_width, kFP16Quarter_706CF4, pNewBullet->field_8->field_8_depth);
    p5CSprite->SetType_4206F0(pNewBullet->field_4->get_type_416B40());
    p5CSprite->SetObj2C_482A30(pNewBullet->field_4->field_8_object_2C_ptr);

    pNewBullet->SetDamageOwner_529080(field_24_pPed->get_varrok_idx_420B50());

    if (bullet_type == objects::machine_gun_bullet_254 || bullet_type == objects::pistol_bullet_265)
    {
        pNewBullet->SetSpriteIdOffset_5290C0(field_24_pPed->GetBulletSpriteOffset_45BE30());
    }

    if (p5CSprite->CheckSpriteMovementRegion_5A2500())
    {
        pNewBullet->RequestRemoval_5290A0();
        pNewBullet = NULL;
        bAllowFlameSegment_706D60 = 0;
    }
    else
    {
        bAllowFlameSegment_706D60 = 1;
        pNewBullet->SetMovementVector_5224E0(speed);
    }
    return pNewBullet;
}

// https://decomp.me/scratch/73olU
MATCH_FUNC(0x5dd0f0)
void Weapon_30::flamethrower_5DD0F0()
{
    Ang16 ped_rot;
    Fix16_Point cartesian_offset;
    Fix16_Point ped_pos_maybe;

    Fix16 cam_x = field_24_pPed->get_cam_x();
    Fix16 cam_y = field_24_pPed->get_cam_y();
    Fix16 cam_z = field_24_pPed->get_cam_z();

    ped_rot = field_24_pPed->GetRotation();

    ped_pos_maybe = field_24_pPed->GetVelocityVector_45B520();

    cartesian_offset.FromPolar_41E210(kFP16Quarter_706CF4, ped_rot);

    Fix16 xpos = cam_x + cartesian_offset.x;
    Fix16 ypos = cam_y + cartesian_offset.y;

    if (!field_4)
    {
        set_field_2C_4CCA80(1);
        bAllowFlameSegment_706D60 = 0;
        Weapon_30::spawn_bullet_5DCF60(154, xpos, ypos, cam_z, ped_rot, ped_pos_maybe);
        if (bAllowFlameSegment_706D60)
        {
            gParticle_8_6FD5E8->EmitFlameStreamSegment_53F4C0(field_24_pPed->field_168_game_object->field_80_sprite_ptr);

            if (field_24_pPed->IsField238_45EDE0(2))
            {
                DecreaseAmmo_4CCA60();
            }
            field_24_pPed->AddThreateningPedToList_46FC70();
            if (field_24_pPed->is_player_41B0A0())
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
            }
        }
    }
    else
    {
        Weapon_30::spawn_bullet_5DCF60(195, cam_x, cam_y, cam_z, ped_rot, ped_pos_maybe);
    }
}

// https://decomp.me/scratch/3qEdg
// The spread angles are built three ways: Ang16(s16, u8) gives the 16-bit add/sub + jns, operator+
// (the const s16& ctor) gives the 32-bit lea + test/jge of the first tank shot, and the last tank shot
// normalizes out of line through Normalize_406C20.
MATCH_FUNC(0x5dd290)
void Weapon_30::shotgun_5DD290()
{
    Ang16 ped_rotation;
    Fix16_Point vector;
    if (field_2_reload_speed == 0)
    {
        Fix16 x = field_24_pPed->get_cam_x();
        Fix16 y = field_24_pPed->get_cam_y();
        Fix16 z = field_24_pPed->get_cam_z();
        ped_rotation = field_24_pPed->GetRotation();
        vector = field_24_pPed->GetVelocityVector_45B520();
        set_field_2C_4CCA80(1);
        if (!field_4)
        {
            Object_2C* pBullet_1 = Weapon_30::spawn_bullet_5DCF60(objects::shotgun_bullet_192, x, y, z, Ang16(word_706D5E.rValue + ped_rotation.rValue, (u8)0), vector);
            Object_2C* pBullet_2 = Weapon_30::spawn_bullet_5DCF60(objects::shotgun_bullet_192, x, y, z, Ang16(word_707002.rValue + ped_rotation.rValue, (u8)0), vector);
            Object_2C* pBullet_3 = Weapon_30::spawn_bullet_5DCF60(objects::shotgun_bullet_192, x, y, z, ped_rotation, vector);
            Object_2C* pBullet_4 = Weapon_30::spawn_bullet_5DCF60(objects::shotgun_bullet_192, x, y, z, Ang16(ped_rotation.rValue - word_707002.rValue, (u8)0), vector);
            Object_2C* pBullet_5 = Weapon_30::spawn_bullet_5DCF60(objects::shotgun_bullet_192, x, y, z, Ang16(ped_rotation.rValue - word_706D5E.rValue, (u8)0), vector);
            if ((pBullet_1 || pBullet_2 || pBullet_3 || pBullet_4 || pBullet_5) && field_24_pPed->IsField238_45EDE0(2))
            {
                decrement_ammo_4CCA30();
            }
            field_2_reload_speed = 40;
            field_24_pPed->AddThreateningPedToList_46FC70();
            gParticle_8_6FD5E8->GunMuzzelFlash_53E970(field_24_pPed->field_168_game_object->field_80_sprite_ptr);

            if (field_24_pPed->is_player_41B0A0())
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
            }
        }
        else
        {
            Weapon_30::spawn_bullet_5DCF60(objects::tanktop_193, x, y, z, word_706D5C + ped_rotation, vector);
            Weapon_30::spawn_bullet_5DCF60(objects::tanktop_193, x, y, z, Ang16(word_706D5E.rValue + ped_rotation.rValue, (u8)0), vector);
            Weapon_30::spawn_bullet_5DCF60(objects::tanktop_193, x, y, z, ped_rotation, vector);
            Weapon_30::spawn_bullet_5DCF60(objects::tanktop_193, x, y, z, Ang16(ped_rotation.rValue - word_706D5E.rValue, (u8)0), vector);
            Weapon_30::spawn_bullet_5DCF60(objects::tanktop_193, x, y, z, Ang16(ped_rotation.rValue - word_706D5C.rValue).Normalized_406C20(), vector);
            field_2_reload_speed = 5;
        }
        Weapon_30::TickReloadSpeed_5DCF40();
    }
    else
    {
        field_2_reload_speed--;
    }
}

// Fix16_Point::FromPolar_41E210 (9.6f 0x41E210) as it comes out in pistol_5DD860: the x line
// reads the sine table directly (through Ang16::sine_40F500 VC6 loads the radius first), the
// y line is the out of line radius * cos
static inline void FromPolar_41E210_sin_table(Fix16_Point& p, const Fix16& radius, const Ang16& angle)
{
    p.x = radius * gSin_table_667A80[angle.rValue];
    p.y = radius * Ang16::cosine_40F520(angle);
}

// 9.6f 0x4CE070
MATCH_FUNC(0x5dd860)
void Weapon_30::pistol_5DD860()
{
    Ang16 pedRot;
    Fix16_Point offset;
    Fix16_Point velocity;

    if (field_2_reload_speed == 0)
    {
        set_field_2C_4CCA80(1);
        if (!field_4) // first shot ??
        {
            const s32 bullet_type = field_24_pPed->IsField238_45EDE0(2) ? 265 : 254;

            Fix16 x = field_24_pPed->get_cam_x();
            Fix16 y = field_24_pPed->get_cam_y();
            Fix16 z = field_24_pPed->get_cam_z();
            pedRot = field_24_pPed->GetRotation();
            velocity = field_24_pPed->GetVelocityVector_45B520();
            FromPolar_41E210_sin_table(offset, kFP16Quarter_706CF4, pedRot);
            Fix16 xx = x + offset.x;
            Fix16 yy = y + offset.y;
            if (spawn_bullet_5DCF60(bullet_type, xx, yy, z, pedRot, velocity))
            {
                if (field_24_pPed->IsField238_45EDE0(2))
                {
                    decrement_ammo_4CCA30();
                }
            }

            field_2_reload_speed = 20;

            gParticle_8_6FD5E8->GunMuzzelFlash_53E970(field_24_pPed->field_168_game_object->field_80_sprite_ptr);
            field_24_pPed->AddThreateningPedToList_46FC70();

            if (field_24_pPed->is_player_41B0A0())
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2u, this->field_24_pPed);
            }
        }
        else
        {
            spawn_bullet_5DCF60(154,
                                field_24_pPed->get_cam_x(),
                                field_24_pPed->get_cam_y(),
                                field_24_pPed->get_cam_z(),
                                field_24_pPed->Get_F12E_4CCA90(),
                                field_24_pPed->GetVelocityVector_45B520());
            field_2_reload_speed = 5;
        }
        TickReloadSpeed_5DCF40();
    }
    else
    {
        field_2_reload_speed--;
    }
}

// It matches on decompme: https://decomp.me/scratch/dAQ5C
MATCH_FUNC(0x5dda70)
void Weapon_30::dual_pistol_5DDA70()
{
    Ang16 ped_rotation;
    Fix16_Point vector;
    Fix16_Point vector2;
    Fix16_Point vector3;
    Fix16_Point vector_esp1; // not used, but required to match
    Fix16_Point vector_esp2; // not used, but required to match

    if (field_2_reload_speed == 0)
    {
        vector = field_24_pPed->GetVelocityVector_45B520();

        Fix16 x = field_24_pPed->get_cam_x();
        Fix16 y = field_24_pPed->get_cam_y();
        Fix16 z = field_24_pPed->get_cam_z();
        ped_rotation = field_24_pPed->GetRotation();
        vector3 = field_24_pPed->GetVelocityVector_45B520(); // vector isnt used, but required to match

        vector2.FromPolar_41E210(kFP16Quarter_706CF4, ped_rotation);
        Fix16 point_x = x + vector2.x;
        Fix16 point_y = y + vector2.y;

        set_field_2C_4CCA80(1);
        if (!field_4)
        {
            s32 weapon_bullet_model;
            if (field_24_pPed->IsField238_45EDE0(2))
            {
                weapon_bullet_model = objects::pistol_bullet_265;
            }
            else
            {
                weapon_bullet_model = objects::machine_gun_bullet_254;
            }

            Object_2C* pBullet_1 =
                Weapon_30::spawn_bullet_5DCF60(weapon_bullet_model, point_x, point_y, z, ped_rotation - word_706D5E, vector);
            Object_2C* pBullet_2 =
                Weapon_30::spawn_bullet_5DCF60(weapon_bullet_model, point_x, point_y, z, ped_rotation + word_706D5E, vector);
            if ((pBullet_1 || pBullet_2) && field_24_pPed->IsField238_45EDE0(2))
            {
                decrement_ammo_4CCA30();
            }
            field_2_reload_speed = 10;
            field_24_pPed->AddThreateningPedToList_46FC70();
            gParticle_8_6FD5E8->GunMuzzelFlash_53E970(field_24_pPed->field_168_game_object->field_80_sprite_ptr);

            if (field_24_pPed->is_player_41B0A0())
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
            }
        }
        else
        {
            Weapon_30::spawn_bullet_5DCF60(objects::flamethrower_fire_154, point_x, point_y, z, ped_rotation - word_706D5E, vector);
            Weapon_30::spawn_bullet_5DCF60(objects::flamethrower_fire_154, point_y, point_y, z, ped_rotation + word_706D5E, vector);
            field_2_reload_speed = 5;
        }
        Weapon_30::TickReloadSpeed_5DCF40();
    }
    else
    {
        field_2_reload_speed--;
    }
}

// https://decomp.me/scratch/lAo1H
MATCH_FUNC(0x5ddd20)
void Weapon_30::smg_5DDD20()
{
    Fix16_Point point;
    if (field_2_reload_speed == 0)
    {
        set_field_2C_4CCA80(1);
        if (!field_4)
        {
            Ang16 AimAngle;
            AimAngle = field_24_pPed->ComputeAimAngle_45C9D0();

            point.x = -dword_706E7C;
            point.y = dword_706CF0 + dword_706E80;

            point.RotateByAngle_40F6B0(field_24_pPed->field_168_game_object->field_80_sprite_ptr->field_0);

            point = point.Add_40AC50(field_24_pPed->GetVelocityVector_45B520());

            if (Weapon_30::spawn_bullet_5DCF60(254,
                                               field_24_pPed->get_cam_x() + point.x,
                                               field_24_pPed->get_cam_y() + point.y,
                                               field_24_pPed->get_cam_z(),
                                               AimAngle,
                                               field_24_pPed->GetVelocityVector_45B520()))
            {
                if (field_24_pPed->IsField238_45EDE0(2))
                {
                    DecreaseAmmo_4CCA60();
                }
            }

            field_2_reload_speed = 2;
            gParticle_8_6FD5E8->GunMuzzelFlash_53E970(field_24_pPed->field_168_game_object->field_80_sprite_ptr);

            if (field_1C_idx != weapon_type::silence_smg)
            {
                field_24_pPed->AddThreateningPedToList_46FC70();
                if (field_24_pPed->is_player_41B0A0())
                {
                    gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
                }
            }
        }
        else
        {
            Weapon_30::spawn_bullet_5DCF60(154,
                                           field_24_pPed->get_cam_x(),
                                           field_24_pPed->get_cam_y(),
                                           field_24_pPed->get_cam_z(),
                                           field_24_pPed->Get_F12E_4CCA90(),
                                           field_24_pPed->GetVelocityVector_45B520());
            field_2_reload_speed = 1;
        }
        Weapon_30::TickReloadSpeed_5DCF40();
    }
    else
    {
        field_2_reload_speed--;
    }
}

// https://decomp.me/scratch/OrmRn
MATCH_FUNC(0x5ddfc0)
void Weapon_30::throwable_5DDFC0(s32 obj_idx, s32 a3, s32 a4)
{
    Fix16_Point vector;
    Object_2C* pProjectile;

    if (a3)
    {
        field_21 = 0;
        if (field_2_reload_speed == 0)
        {
            set_field_2C_4CCA80(1);
            if (!field_4)
            {
                if (!field_24_pPed->IsField238_45EDE0(2) && !field_20)
                {
                    spawn_bullet_5DCF60(objects::object_159,
                                        field_24_pPed->get_cam_x(),
                                        field_24_pPed->get_cam_y(),
                                        field_24_pPed->get_cam_z(),
                                        field_24_pPed->Get_F12E_4CCA90(),
                                        field_24_pPed->GetVelocityVector_45B520());
                    field_2_reload_speed = 5;
                    field_20 = 1;
                    if (field_24_pPed->is_player_41B0A0())
                    {
                        gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
                    }
                }
                else
                {
                    if (obj_idx == objects::grenade_obj_183 && a4 == 96)
                    {
                        // maybe holding the grenade for too long
                        gObject_5C_6F8F84->CreateExplosion_52A3D0(field_24_pPed->get_cam_x(),
                                                                  field_24_pPed->get_cam_y(),
                                                                  field_24_pPed->get_cam_z(),
                                                                  field_24_pPed->Get_F12E_4CCA90(),
                                                                  18,
                                                                  field_24_pPed->field_200_id);
                        if (field_24_pPed->IsField238_45EDE0(2))
                        {
                            decrement_ammo_4CCA30();
                        }
                        if (field_24_pPed->is_player_41B0A0())
                        {
                            gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
                        }
                    }
                    else
                    {
                        Fix16 speed;
                        if (obj_idx == objects::grenade_obj_183)
                        {
                            speed = dword_706E74 + (Fix16(a3) / Fix16(60)) * (dword_706CF0 + dword_706E80);
                        }
                        else
                        {
                            speed = dword_706E80 + (Fix16(a3) / Fix16(60)) * dword_706CF0;
                        }
                        gObject_5C_6F8F84->SetPendingDamageOwner_52A210(field_24_pPed->get_varrok_idx_420B50());

                        // field_24_pPed->Get_F12E_4CCA90()
                        pProjectile = gObject_5C_6F8F84->sub_52A280(obj_idx,
                                                                               field_24_pPed->get_cam_x(),
                                                                               field_24_pPed->get_cam_y(),
                                                                               field_24_pPed->get_cam_z() + kFP16Half_706DA8,
                                                                               field_24_pPed->Get_F12E_4CCA90(),
                                                                               field_24_pPed->Get_F12E_4CCA90(),
                                                                               speed,
                                                                               -dword_706F64,
                                                                               dword_706CF0);
                        if (pProjectile)
                        {
                            if ((field_24_pPed->field_168_game_object->field_58_flags & 8) == 0)
                            {
                                vector = field_24_pPed->GetVelocityVector_45B520();
                                pProjectile->SetMovementVector_5224E0(vector);
                                if (!vector.IsNull_420360())
                                {
                                    pProjectile->field_10_obj_3c->field_C_speed += dword_706C8C;
                                }
                            }
                            if (obj_idx == objects::moving_molotov_138)
                            {
                                Object_2C* pLightObj = gObject_5C_6F8F84->NewLight_529A40(94, 138, 2, 0xFF8000, 3, 255);
                                pProjectile->field_4->DispatchCollisionEvent_5A3100(pLightObj->field_4, 0, 0, kAngZero_707006);
                                Object_2C* pMaybeExplosionObj =
                                    gObject_5C_6F8F84->CreateExplosion_52A3D0(Fix16(113), Fix16(145), 2, kAngZero_707006, 5, field_24_pPed->field_200_id);
                                if (pMaybeExplosionObj)
                                {
                                    pProjectile->field_4->DispatchCollisionEvent_5A3100(pMaybeExplosionObj->field_4, 0, 0, kAngZero_707006);
                                }
                            }
                            else
                            {
                                // inline here: sub_434130
                                pProjectile->SetO8Timer_434130((96 - a4) / 8);
                            }

                            if (field_24_pPed->IsField238_45EDE0(2))
                            {
                                decrement_ammo_4CCA30();
                            }
                            field_21 = 1;
                            if (field_24_pPed->is_player_41B0A0())
                            {
                                gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
                            }
                            field_24_pPed->AddThreateningPedToList_46FC70();
                        }
                    }
                    if (field_24_pPed->field_15C_player)
                    {
                        field_2_reload_speed = 4;
                        field_24_pPed->field_21C_bf.b22 = true;
                    }
                    else
                    {
                        field_2_reload_speed = 50;
                        field_24_pPed->field_21C_bf.b22 = true;
                    }
                    field_21 = 1;
                    Weapon_30::TickReloadSpeed_5DCF40();
                }
            }
            else
            {
                spawn_bullet_5DCF60(objects::object_159,
                                    field_24_pPed->get_cam_x(),
                                    field_24_pPed->get_cam_y(),
                                    field_24_pPed->get_cam_z(),
                                    field_24_pPed->Get_F12E_4CCA90(),
                                    field_24_pPed->GetVelocityVector_45B520());
                field_2_reload_speed = 5;
                field_20 = 0;
                Weapon_30::TickReloadSpeed_5DCF40();
            }
        }
        else
        {
            if (field_20 == 0)
            {
                field_24_pPed->field_21C_bf.b22 = false;
            }
            --field_2_reload_speed;
            if (field_2_reload_speed < 30 && field_21)
            {
                field_21 = 0;
                field_24_pPed->field_21C_bf.b22 = false;
            }
        }
    }
}

EXPORT void __stdcall sub_5DE910(Fix16_Point_POD a1, Fix16_Point& a2, Fix16 a3);

WIP_FUNC(0x5de4f0)
void Weapon_30::sub_5DE4F0()
{
    Sprite* pBeam = gObject_5C_6F8F84->field_58_collision_probe_sprite;
    Fix16_Point delta(field_24_pPed->field_198->get_cam_x() - field_24_pPed->get_cam_x(),
                      field_24_pPed->field_198->get_cam_y() - field_24_pPed->get_cam_y());
    gRozza_679188.Reset_4637B0();

    Ang16 angle;
    angle = Fix16::atan2_fixed_405320(field_24_pPed->field_198->get_cam_y() - field_24_pPed->get_cam_y(),
                                            field_24_pPed->field_198->get_cam_x() - field_24_pPed->get_cam_x());

    Fix16 dist = delta.GetLength_41E260();

    if (dist > dword_706EC4)
    {
        field_24_pPed->field_198 = NULL;
        return;
    }

    pBeam->set_xyz_lazy_420600(field_24_pPed->field_1AC_cam.x, field_24_pPed->field_1AC_cam.y, field_24_pPed->field_1AC_cam.z);
    pBeam->set_ang_lazy_420690(angle);
    pBeam->AllocInternal_59F950(dword_706CF0, dword_706CF0, dword_706CF0);

    Fix16 steps;
    Fix16 step_len;
    if (dist != dword_706EB8)
    {
        steps = dist / dword_706CF0;
        step_len = dist / steps;
    }
    else
    {
        steps = dword_706EB8;
        step_len = dword_706EB8;
    }

    if (steps < dword_706EBC)
    {
        steps = dword_706EBC;
        step_len = dist;
    }

    Fix16 step_x;
    Fix16 step_y;
    Ang16::PolarToCartesian_41FC20(angle, step_len, step_x, step_y);
    for (u8 i = 1; i <= steps.ToInt(); i++)
    {
        gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(pBeam->field_14_xy.x, pBeam->field_14_xy.y, pBeam->field_1C_zpos);
        pBeam->set_xy_lazy_447E20(pBeam->field_14_xy.x + step_x, pBeam->field_14_xy.y + step_y);
        if (pBeam->sub_5A2440())
        {
            break;
        }

        Sprite* pHit = pBeam->QuerySpriteCollision_59E7D0(2);
        if (pHit)
        {
            switch (pHit->field_30_sprite_type_enum)
            {
                case sprite_types_enum::car_2:
                    field_24_pPed->field_170_selected_weapon->field_4 = 1;
                    field_24_pPed->field_198 = 0;
                    return;

                case sprite_types_enum::ped_3:
                {
                    if (pHit->field_8_char_b4_ptr->field_7C_pPed != field_24_pPed->field_198 &&
                        pHit->field_8_char_b4_ptr->field_7C_pPed != field_24_pPed)
                    {
                        s32 state = pHit->field_8_char_b4_ptr->field_7C_pPed->field_278_ped_state_1;
                        if (state < 8 || state > 9)
                        {
                            field_24_pPed->field_170_selected_weapon->field_4 = 1;
                        }
                    }
                    break;
                }
            }
        }
    }

    field_24_pPed->field_198->field_144_attacker = field_24_pPed;
    field_24_pPed->field_198->field_204_killer_id = field_24_pPed->field_200_id;
    field_24_pPed->field_198->field_21C_bf.b8 = 1;
    if (field_24_pPed->field_28C_threat_reaction == 1)
    {
        gPolice_7B8_6FEE40->field_7B0_last_firing_emergency_ped = field_24_pPed;
    }

    sub_5DE910(field_24_pPed->field_168_game_object->field_80_sprite_ptr->get_x_y(),
               field_24_pPed->field_198->field_168_game_object->field_80_sprite_ptr->get_x_y(),
               field_24_pPed->get_cam_z());
}

DEFINE_GLOBAL_INIT(Fix16, dword_706CF8, Fix16(0xCCC, 0), 0x706CF8);
DEFINE_GLOBAL_INIT(Fix16, dword_706D34, Fix16(0x100, 0), 0x706D34);

// Length of `d`, with dword_706EB8 as the zero (the multiplies, the add and the square root are the named
// out-of-line copies; Abs follows the inline budget).
static inline Fix16 BeamLength_5DE910(Fix16_Point& d)
{
    if (d.x == dword_706EB8)
    {
        return Fix16::Abs(d.y);
    }
    else if (d.y == dword_706EB8)
    {
        return Fix16::Abs(d.x);
    }
    else
    {
        return Fix16::SquareRoot_436A70(d.x.Multiply_408680(d.x).Add_408660(d.y.Multiply_408680(d.y)));
    }
}

// Draws the electro beam from `a1` (or the gun muzzle when byte_706C94 is clear) to `a2` at height
// `a3`: kFP16Quarter_706CF4 long segments with a random kink each, then straight segments for the rest.
// `a1` is the base type: callers pass a get_x_y() temporary, which is sliced into it
// (Fix16_Point here breaks the caller sub_5DFB60). 9.6f: sub_4CCBD0.
// The EH state is 0xA at entry and never changes: 11 Fix16_Point locals, two of them unused
// (likely the original's by-value a1 is one of them). `d` is reused for the rest of the way and
// `len` for the straight segment count, as the original's frame shows.
MATCH_FUNC(0x5de910)
void __stdcall sub_5DE910(Fix16_Point_POD a1, Fix16_Point& a2, Fix16 a3)
{
    Fix16_Point d;
    Fix16_Point start;
    Fix16_Point cur;
    Fix16_Point mid;
    Fix16_Point next;
    Fix16_Point step;
    Fix16_Point d3;
    Fix16_Point from;
    Fix16_Point to;
    Fix16_Point unused_eh_1;
    Fix16_Point unused_eh_2;

    Fix16 seg_len = kFP16Quarter_706CF4;
    if (byte_706C94 > 0)
    {
        start.x = a1.x;
        start.y = a1.y;
    }
    else
    {
        start.x = -dword_706E7C;
        start.y = dword_706CF8 + dword_706E80;
        start.RotateByAngle_40F6B0_all_out_of_line(word_707004);
        start = start.Add_40AC50(a1);
        start.x += stru_706E58.x;
        start.y += stru_706E58.y;
    }

    Fix16 len;
    Ang16 angle;
    Ang16 seg_angle;
    // The first length and angle are dead, like 9.6f's (it only kept the atan2 call)
    d = a2.Sub_40AC80(start);
    len = BeamLength_5DE910(d);
    d.atan2_40F790();

    to = a2;
    from = start;
    d = a2.Sub_40AC80(start);
    len = BeamLength_5DE910(d);
    gRng_6F6784.get_int_4F7AE0(2);

    from = start;
    to = a2;
    d3 = to.Sub_40AC80(from);
    len = BeamLength_5DE910(d3);
    u8 count = (len / seg_len).ToInt();
    angle = d3.atan2_40F790();

    cur = from;
    for (u8 i = 0; i < count; i++)
    {
        // 9.6f: Fix16(s16) 0x401AE0 minus Fix16(u16) 0x41F990, times dword_706D34. The kink stays a
        // temporary (a named local moves the frame slots) and `-=` evaluates the half first.
        u16 spread = (gRng_6F6784.get_int_4F7AE0(4) + 1) * 32;
        Ang16 jitter = Ang16::Fix16_To_Ang16_ool_40F540(
            (Fix16(gRng_6F6784.get_int_4F7AE0(spread)) -= Fix16((u16)(spread >> 1))) * dword_706D34);

        step.x = seg_len.Multiply_408680(Ang16::sine_40F500(angle));
        step.y = seg_len.Multiply_408680(Ang16::cosine_40F520(angle));
        step.RotateByAngle_40F6B0_all_out_of_line(jitter);
        seg_angle = step.atan2_40F790();

        next = cur.Add_40AC50(step);
        mid = next.Sub_40AC80(cur);
        mid.x.DivideAssign_539F90(kFP16Two_706EC0);
        mid.y.DivideAssign_539F90(kFP16Two_706EC0);
        mid.x += cur.x;
        mid.y += cur.y;
        gParticle_8_6FD5E8->EmitElectricArcParticle(mid.x, mid.y, a3, seg_angle);
        cur = next;
    }

    d = a2.Sub_40AC80(cur);
    seg_angle = d.atan2_40F790();
    len = d.MaxAbs_5E4140() / seg_len;
    if (len != dword_706EB8)
    {
        d.DivAssign_5E40E0(len);
        cur = next;
        for (s32 j = 1; j <= len.ToInt(); j++)
        {
            next.AddAssign_5E40C0(d);
            mid = next.Sub_40AC80(cur);
            mid.DivAssign_5E40E0(kFP16Two_706EC0);
            mid.AddAssign_5E40C0(cur);
            gParticle_8_6FD5E8->EmitElectricArcParticle(mid.x, mid.y, a3, seg_angle);
            cur = next;
        }
    }
}

WIP_FUNC(0x5DF270)
void __stdcall sub_5DF270(Sprite* a1, Fix16 a2, char_type a3, char_type a4, Ped* a5, Sprite* a6)
{
    // 9.6f 0x4CEF40
    struct_4 hits;
    Fix16 xpos = a1->field_14_xy.x;
    Fix16 ypos = a1->field_14_xy.y;
    Fix16 zpos = a1->field_1C_zpos;
    Ang16 angle = a1->field_0;
    Ang16 diff;

    Fix16_Rect rect;
    rect.ComputeCollisionPrism_4204D0(xpos, ypos, a2, zpos);

    if (gPurpleDoom_1_679208->CollectRectCollisions_477F30(&rect, 0, 0, a1, &hits))
    {
        Sprite* pHit;
        if (a6)
        {
            // Every hit nearer than a6 must be inside the angle window
            pHit = hits.TakeClosestSprite_5A6EA0(xpos, ypos);
            while (pHit)
            {
                if (!pHit->AsCharB4_40FEA0())
                {
                    diff = Fix16::atan2_fixed_405320(pHit->field_14_xy.y - ypos, pHit->field_14_xy.x - xpos) - angle;
                    if (diff < word_706D6C || diff > word_706E28)
                    {
                        hits.ClearList_5A6E10();
                        if (a5->field_170_selected_weapon)
                        {
                            a5->field_170_selected_weapon->Set_F4_433810(1);
                        }
                        return;
                    }
                }
                else
                {
                    if (pHit == a6)
                    {
                        break;
                    }
                    diff = Fix16::atan2_fixed_405320(pHit->field_14_xy.y - ypos, pHit->field_14_xy.x - xpos) - angle;
                    if (diff < word_706D6C || diff > word_706E28)
                    {
                        hits.ClearList_5A6E10();
                        if (a5->field_170_selected_weapon)
                        {
                            a5->field_170_selected_weapon->Set_F4_433810(1);
                        }
                        return;
                    }
                }
                pHit = hits.TakeClosestSprite_5A6EA0(xpos, ypos);
            }
        }
        else
        {
            pHit = hits.TakeClosestSprite_5A6EA0(xpos, ypos);
        }

        while (pHit)
        {
            Char_B4* pB4 = pHit->AsCharB4_40FEA0();
            if (pB4)
            {
                char_type bOutside;
                if (a3)
                {
                    diff = Fix16::atan2_fixed_405320(pHit->field_14_xy.y - ypos, pHit->field_14_xy.x - xpos) - angle;
                    if (diff < word_706D6C || diff > word_706E28)
                    {
                        bOutside = 1;
                    }
                    else
                    {
                        bOutside = 0;
                    }
                }
                else
                {
                    bOutside = 1;
                }

                // The result is unused
                Fix16::MaxAbsDistance_42A6B0(a5->get_cam_x(), a5->get_cam_y(), pB4->field_80_sprite_ptr->field_14_xy.x, pB4->field_80_sprite_ptr->field_14_xy.y);

                if (bOutside)
                {
                    if (gMap_0x370_6F6268->sub_4E5640(dword_706CF0,
                                                      dword_706CF0,
                                                      dword_706CF0,
                                                      xpos,
                                                      ypos,
                                                      zpos,
                                                      pHit->field_14_xy.x,
                                                      pHit->field_14_xy.y,
                                                      pHit->field_1C_zpos))
                    {
                        pB4->field_7C_pPed->SetAttacker_433BF0(a5);
                        pB4->field_7C_pPed->field_204_killer_id = a5->field_200_id;
                        pB4->field_7C_pPed->field_290 = 18;
                        pB4->field_7C_pPed->field_264_killer_id_timer = 50;
                        if (a4)
                        {
                            Fix16 z;
                            if (pHit->field_1C_zpos > zpos)
                            {
                                z = pHit->field_1C_zpos;
                            }
                            else
                            {
                                z = zpos;
                            }
                            sub_5DE910(a1->get_x_y(), pHit->get_x_y(), z);
                            pB4->field_7C_pPed->TakeDamage(3);
                        }
                        else
                        {
                            Fix16 z;
                            if (pHit->field_1C_zpos > zpos)
                            {
                                z = pHit->field_1C_zpos;
                            }
                            else
                            {
                                z = zpos;
                            }
                            sub_5DE910(a1->get_x_y(), pHit->get_x_y(), z);
                            pB4->field_7C_pPed->field_210_shock_counter += 3;
                            if (a5->field_170_selected_weapon)
                            {
                                a5->field_170_selected_weapon->Set_F4_433810(0);
                            }
                            hits.ClearList_5A6E10();
                            return;
                        }
                    }
                }
                else if (a5->field_198)
                {
                    a5->field_198 = 0;
                }
            }
            pHit = hits.TakeClosestSprite_5A6EA0(xpos, ypos);
        }
    }

    a5->field_198 = 0;
    if (a5->field_170_selected_weapon)
    {
        a5->field_170_selected_weapon->Set_F4_433810(1);
    }
}

MATCH_FUNC(0x5dfb60)
void Weapon_30::sub_5DFB60(u8 a2, Sprite* a3, Ang16 a4)
{
    struct_4 hits;
    char_type bHit = 0;

    // Both branches compute the four bounds into registers and share the stores.
    Fix16_Rect rect;
    Fix16 left;
    Fix16 right;
    Fix16 top;
    Fix16 bottom;
    if (!a2)
    {
        left = a3->field_14_xy.x - kFP16Two_706EC0;
        right = a3->field_14_xy.x + kFP16Two_706EC0;
        top = a3->field_14_xy.y - kFP16Two_706EC0;
        bottom = a3->field_14_xy.y + kFP16Two_706EC0;
    }
    else
    {
        left = a3->field_14_xy.x - dword_706EBC;
        right = a3->field_14_xy.x + dword_706EBC;
        top = a3->field_14_xy.y - dword_706EBC;
        bottom = a3->field_14_xy.y + dword_706EBC;
    }
    rect.SetRect_41E350(left, right, top, bottom);
    rect.SetHiLowZ_41E370(a3->field_1C_zpos - dword_706EBC, a3->field_1C_zpos + dword_706EBC);

    word_707004 = field_24_pPed->field_168_game_object->field_80_sprite_ptr->field_0;
    // The original copies the returned point through the return pointer, as a struct assignment
    reinterpret_cast<Fix16_Point&>(stru_706E58) = field_24_pPed->GetVelocityVector_45B520();

    if (gPurpleDoom_1_679208->CollectRectCollisions_477F30(&rect, 0, 0, a3, &hits) && hits.field_0_p18)
    {
        do
        {
            Sprite* pHit = hits.PopFrontSprite_5A6DA0();
            // Declared here (9.6f constructs them at the top): both branches share the slots.
            // The ped branch's `angle - a4` is past the inline budget, so its Normalize is the
            // out-of-line Normalize_406C20 while the car branch's is inlined.
            Ang16 angle;
            Ang16 diff;
            switch (pHit->get_type_416B40())
            {
                case sprite_types_enum::ped_3:
                    if (a3 != pHit && !gWeapon_8_707018->field_0.SpriteExists_5A6D80(pHit))
                    {
                        angle = Fix16::atan2_fixed_405320(pHit->field_14_xy.y - a3->field_14_xy.y, pHit->field_14_xy.x - a3->field_14_xy.x);
                        Fix16::MaxAbsDistance_42A6B0(pHit->field_14_xy.x, pHit->field_14_xy.y, a3->field_14_xy.x, a3->field_14_xy.y);

                        diff = angle - a4;
                        if (diff < word_706D6C || diff > word_706E28)
                        {
                            if (gMap_0x370_6F6268->sub_4E5640(dword_706CF0,
                                                              dword_706CF0,
                                                              dword_706CF0,
                                                              a3->field_14_xy.x,
                                                              a3->field_14_xy.y,
                                                              a3->field_1C_zpos,
                                                              pHit->field_14_xy.x,
                                                              pHit->field_14_xy.y,
                                                              pHit->field_1C_zpos))
                            {
                                byte_706C94 = a2;
                                sub_5DE910(a3->get_x_y(),
                                           pHit->get_x_y(),
                                           pHit->field_1C_zpos > a3->field_1C_zpos ? pHit->field_1C_zpos : a3->field_1C_zpos);
                                gWeapon_8_707018->field_0.PushSprite_5A6D40(pHit);
                                if (a2 < 1)
                                {
                                    sub_5DFB60(a2 + 1, pHit, angle);
                                }
                                pHit->field_8_char_b4_ptr->field_7C_pPed->SetAttacker_433BF0(field_24_pPed);
                                pHit->field_8_char_b4_ptr->field_7C_pPed->field_204_killer_id = field_24_pPed->field_200_id;
                                pHit->field_8_char_b4_ptr->field_7C_pPed->field_290 = 18;
                                pHit->field_8_char_b4_ptr->field_7C_pPed->field_264_killer_id_timer = 50;
                                pHit->field_8_char_b4_ptr->field_7C_pPed->field_210_shock_counter += 5;
                                if (field_24_pPed->is_player_41B0A0())
                                {
                                    gShooey_CC_67A4B8->ReportCrimeForPed(2u, field_24_pPed);
                                }
                                field_24_pPed->AddThreateningPedToList_46FC70();
                                gfrosty_pasteur_6F8060->RecordWeaponHit_512C00(pHit->field_8_char_b4_ptr->field_7C_pPed->field_200_id, 160, 1);
                            }
                            bHit = 1;
                        }
                    }
                    break;

                case sprite_types_enum::car_2:
                    if (a3 != pHit && !gWeapon_8_707018->field_0.SpriteExists_5A6D80(pHit))
                    {
                        angle = Fix16::atan2_fixed_405320(pHit->field_14_xy.y - a3->field_14_xy.y, pHit->field_14_xy.x - a3->field_14_xy.x);
                        Fix16::MaxAbsDistance_42A6B0(pHit->field_14_xy.x, pHit->field_14_xy.y, a3->field_14_xy.x, a3->field_14_xy.y);

                        diff = angle - a4;
                        if (diff < word_706D6C || diff > word_706E28)
                        {
                            if (gMap_0x370_6F6268->sub_4E5640(dword_706CF0,
                                                              dword_706CF0,
                                                              dword_706CF0,
                                                              a3->field_14_xy.x,
                                                              a3->field_14_xy.y,
                                                              a3->field_1C_zpos,
                                                              pHit->field_14_xy.x,
                                                              pHit->field_14_xy.y,
                                                              pHit->field_1C_zpos))
                            {
                                byte_706C94 = a2;
                                sub_5DE910(a3->get_x_y(),
                                           pHit->get_x_y(),
                                           pHit->field_1C_zpos > a3->field_1C_zpos ? pHit->field_1C_zpos : a3->field_1C_zpos);
                                gWeapon_8_707018->field_0.PushSprite_5A6D40(pHit);
                                if (a2 < 1)
                                {
                                    sub_5DFB60(a2 + 1, pHit, angle);
                                }
                                if (!pHit->field_8_car_bc_ptr->is_f78_0x400_425770())
                                {
                                    pHit->field_8_car_bc_ptr->field_70_exploder_ped_id = field_24_pPed->field_200_id;
                                    pHit->field_8_car_bc_ptr->field_90 = 18;
                                    pHit->field_8_car_bc_ptr->field_94_exploder_timer = 50;
                                    s16 damage = pHit->field_8_car_bc_ptr->AccumulateDamage_43DA90(300, &stru_706F90);
                                    pHit->field_8_car_bc_ptr->ApplyVisualDamage_43A9F0();
                                    if (field_24_pPed->IsField238_45EDE0(2) && damage > 0)
                                    {
                                        field_24_pPed->field_15C_player->field_2D4_scores.sub_593150(pHit->field_8_car_bc_ptr, 1);
                                    }
                                }
                                if (field_24_pPed->is_player_41B0A0())
                                {
                                    gShooey_CC_67A4B8->ReportCrimeForPed(2u, field_24_pPed);
                                }
                                field_24_pPed->AddThreateningPedToList_46FC70();
                            }
                            bHit = 1;
                            gfrosty_pasteur_6F8060->RecordWeaponHit_512C00(pHit->field_8_car_bc_ptr->field_6C_maybe_id, 160, 0);
                        }
                    }
                    break;
            }
        } while (hits.field_0_p18);

        if (bHit && !a2)
        {
            set_field_2C_4CCA80(1);
            if (field_24_pPed->IsField238_45EDE0(2) && (gpRng_67AB34->get_cur_rng_41CFE0() & 1))
            {
                DecreaseAmmo_4CCA60();
            }
        }
    }
}

MATCH_FUNC(0x5e06b0)
void Weapon_30::shocker_5E06B0()
{
    gWeapon_8_707018->field_0.ClearList_5A6E10();

    sub_5DFB60(0,
               this->field_24_pPed->field_168_game_object->field_80_sprite_ptr,
               this->field_24_pPed->field_168_game_object->field_80_sprite_ptr->field_0);
    Ped* pPed = this->field_24_pPed;
    if (pPed->field_15C_player)
    {
        gShooey_CC_67A4B8->ReportCrimeForPed(2u, pPed);
    }
}

MATCH_FUNC(0x5e0740)
void Weapon_30::electro_batton_5E0740()
{
    if (!field_24_pPed->field_198)
    {
        if (!field_2_reload_speed)
        {
            field_2C_shot_fired = 1;
            if (!field_4)
            {
                Object_2C* pBullet = spawn_bullet_5DCF60(277,
                                                         field_24_pPed->get_cam_x(),
                                                         field_24_pPed->get_cam_y(),
                                                         field_24_pPed->get_cam_z(),
                                                         field_24_pPed->Get_F12E_4CCA90(),
                                                         field_24_pPed->GetVelocityVector_45B520());
                if (pBullet && field_24_pPed->IsField238_45EDE0(2))
                {
                    decrement_ammo_4CCA30();
                }
                field_2_reload_speed = 20;
            }
            else
            {
                spawn_bullet_5DCF60(154,
                                    field_24_pPed->get_cam_x(),
                                    field_24_pPed->get_cam_y(),
                                    field_24_pPed->get_cam_z(),
                                    field_24_pPed->Get_F12E_4CCA90(),
                                    field_24_pPed->GetVelocityVector_45B520());
                field_2_reload_speed = 5;
            }
            TickReloadSpeed_5DCF40();
        }
        else
        {
            field_2_reload_speed--;
        }
    }
    else if (field_24_pPed->field_198->field_168_game_object)
    {
        if (field_24_pPed->IsField238_45EDE0(2))
        {
            s32 target_state = field_24_pPed->field_198->field_278_ped_state_1;
            if (target_state >= 8 && target_state <= 9)
            {
                field_24_pPed->field_198 = 0;
                return;
            }
        }
        sub_5DE4F0();
    }
    else
    {
        field_24_pPed->field_198 = 0;
    }
}

MATCH_FUNC(0x5e0ab0)
void Weapon_30::car_bomb_5E0AB0(char_type instant_bomb)
{
    field_24_pPed = field_14_car->get_driver_4118B0();

    set_field_2C_4CCA80(1);

    decrement_ammo_4CCA30();

    if (field_14_car->is_trailer_cab_41E460())
    {
        field_14_car->field_64_pTrailer->field_C_pCarOnTrailer->FireCarBomb_440F90(instant_bomb);
        field_14_car->DetachTrailerAndUpdateDamage_4418B0();
    }
    else
    {
        field_14_car->FireCarBomb_440F90(instant_bomb);
    }
}

DEFINE_GLOBAL_INIT(Ang16, word_706DFA, Ang16(720), 0x706DFA);
DEFINE_GLOBAL_INIT(Fix16, dword_706CDC, Fix16(0xE00, 0), 0x706CDC);
DEFINE_GLOBAL_INIT(Fix16, dword_706CD8, Fix16(0x800, 0), 0x706CD8);

MATCH_FUNC(0x5e0b10)
void Weapon_30::fire_truck_flamethrower_5E0B10()
{
    Ang16 gun_ang;
    Fix16_Point bullet_pos;
    Fix16_Point offset;
    Fix16_Point velocity;

    Sprite_18* pTurret;
    Ped* pDriver = field_14_car->field_54_driver;
    field_24_pPed = pDriver;

    pTurret = field_14_car->field_0_qq.GetSpriteForModel_5A6A50(114);
    if (pTurret)
    {
        gun_ang = pTurret->field_0->field_0 + word_706DFA;

        bullet_pos.SetXY_432860(Fix16(0), dword_706CDC);
        bullet_pos.RotateByAngle_40F6B0(gun_ang);
        offset.SetXY_432860(Fix16(0), dword_706CD8);
    }
    else
    {
        gun_ang = field_14_car->field_0_qq.GetSpriteForModel_5A6A50(248)->field_0->field_0;

        bullet_pos.SetXY_432860(Fix16(0), dword_706EA4);
        bullet_pos.RotateByAngle_40F6B0(gun_ang);
        offset.SetXY_432860(Fix16(0), dword_706EE8);
    }

    offset.RotateByAngle_40F6B0(field_14_car->field_50_car_sprite->field_0);

    bullet_pos += offset.Add_40AC50(field_14_car->field_50_car_sprite->get_x_y_443580());

    velocity = field_14_car->field_58_physics->GetPointVelocity_561350(&bullet_pos);

    set_field_2C_4CCA80(1);

    if (!Get_F4_41CC70())
    {
        gParticle_8_6FD5E8->EmitFlameStreamSegment_53F4C0(field_14_car->field_50_car_sprite);
    }
    else
    {
        spawn_bullet_5DCF60(195, bullet_pos.x, bullet_pos.y, field_14_car->field_50_car_sprite->field_1C_zpos, gun_ang, velocity);
    }
}

// Both fire truck guns keep the EH state stores around their Fix16_Point add: once VC6 has compiled the
// out-of-line copy of the inline operator+ it knows the call can't throw and drops them. So only this
// function uses operator+, and fire_truck_flamethrower_5E0B10 above calls Add_40AC50. The inline getters
// (get_driver_4118B0, Get_F4_41CC70) are free inline sites that set the inline budget split, so the
// second rotation calls Negate_4086A0 out of line as in 10.5.
MATCH_FUNC(0x5e0e70)
void Weapon_30::fire_truck_gun_5E0E70()
{
    Ang16 gun_ang;
    Fix16_Point bullet_pos;
    Fix16_Point offset;
    Fix16_Point velocity;

    field_24_pPed = field_14_car->get_driver_4118B0();

    Sprite_18* pTurret = field_14_car->field_0_qq.GetSpriteForModel_5A6A50(114);
    gun_ang = pTurret->field_0->field_0 + word_706DFA;

    bullet_pos.SetXY_432860(Fix16(0), dword_706CDC);
    bullet_pos.RotateByAngle_40F6B0(gun_ang);
    offset.SetXY_432860(Fix16(0), dword_706CD8);

    offset.RotateByAngle_40F6B0(field_14_car->field_50_car_sprite->field_0);

    bullet_pos += offset + field_14_car->field_50_car_sprite->get_x_y();

    velocity = field_14_car->field_58_physics->GetPointVelocity_561350(&bullet_pos);

    set_field_2C_4CCA80(1);

    if (!Get_F4_41CC70())
    {
        gParticle_8_6FD5E8->EmitFireTruckSprayParticle_53FAE0(field_14_car->field_50_car_sprite);
    }
    else
    {
        spawn_bullet_5DCF60(199, bullet_pos.x, bullet_pos.y, field_14_car->field_50_car_sprite->field_1C_zpos, gun_ang, velocity);
    }
}

// Fix16 operator* with the __int64 cast on the left operand: VC6 then keeps the left value (the point's y)
// in eax for the imul as the original does; with Fix16::operator* sin goes to eax
// (tank_main_gun_5E10E0, army_gun_jeep_5E13E0)
static inline Fix16 MultiplyLeftWide(const Fix16& a, const Fix16& b)
{
    return Fix16((s32)(((__int64)a.mValue * b.mValue) >> 14), 0);
}

// Fix16 sum as a free function: the sum lands in ecx (operator+ gives eax)
static inline Fix16 AddFree(const Fix16& a, const Fix16& b)
{
    s32 v = a.mValue + b.mValue;
    return Fix16(v, 0);
}

// Fix16_Point::RotateByAngle_40F6B0 as VC6 emits it in a caller that has run out of inline
// expansions: the multiplies, the negate and the y line's add are the out of line operator
// copies, and the x line's sum is AddFree (tank_main_gun_5E10E0, army_gun_jeep_5E13E0)
static inline void RotateByAngle_40F6B0_no_budget3(Fix16_Point& p, const Ang16& angle)
{
    Fix16 sin = Ang16::sine_40F500(angle);
    Fix16 cos = Ang16::cosine_40F520(angle);
    Fix16 x_old = p.x;
    p.x = AddFree(p.x.Multiply_408680(cos), p.y.Multiply_408680(sin));
    p.y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(p.y.Multiply_408680(cos));
}

// https://decomp.me/scratch/QliaE
MATCH_FUNC(0x5e10e0)
void Weapon_30::tank_main_gun_5E10E0()
{
    Ang16 cannon_angle;
    Fix16_Point cannon_pos;
    Fix16_Point offset;
    Fix16_Point velocity;

    if (field_2_reload_speed == 0)
    {
        field_24_pPed = field_14_car->get_driver_4118B0();
        cannon_angle = field_14_car->field_0_qq.GetSpriteForModel_5A6A50(148)
                           ->field_0->field_0;
        cannon_pos.SetXY_432860(Fix16(0), gTankCannonLength_706E20);
        {
            // RotateByAngle_MixOOL_40F6B0 written out, y * sin with the left-wide multiply
            Fix16 x_old = cannon_pos.x;
            Fix16 sin = Ang16::sine_40F500(cannon_angle);
            Fix16 cos = Ang16::cosine_40F520(cannon_angle);
            cannon_pos.x = cannon_pos.x.Multiply_408680(cos).Add_408660(MultiplyLeftWide(cannon_pos.y, sin));
            cannon_pos.y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(cannon_pos.y.Multiply_408680(cos));
        }

        offset.SetXY_432860(Fix16(0), dword_706D88);
        RotateByAngle_40F6B0_no_budget3(offset, field_14_car->field_50_car_sprite->field_0);

        cannon_pos += offset.Add_40AC50(field_14_car->field_50_car_sprite->get_x_y());

        if (field_14_car->field_58_physics)
        {
            velocity = field_14_car->field_58_physics->GetPointVelocity_561350(&cannon_pos);
        }
        else
        {
            velocity.reset();
        }
        set_field_2C_4CCA80(1);
        if (!field_4)
        {
            if (Weapon_30::spawn_bullet_5DCF60(objects::rocket_bullet_128,
                                               cannon_pos.x,
                                               cannon_pos.y,
                                               field_14_car->field_50_car_sprite->field_1C_zpos,
                                               cannon_angle,
                                               velocity))
            {
                if (field_24_pPed->IsField238_45EDE0(2))
                {
                    decrement_ammo_4CCA30();
                }
            }
            field_2_reload_speed = 50;
            field_24_pPed->AddThreateningPedToList_46FC70();
            if (field_24_pPed->is_player_41B0A0())
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
            }
        }
        else
        {
            Weapon_30::spawn_bullet_5DCF60(objects::object_159,
                                           cannon_pos.x,
                                           cannon_pos.y,
                                           field_14_car->field_50_car_sprite->field_1C_zpos,
                                           cannon_angle,
                                           velocity);
            field_2_reload_speed = 5;
        }
        Weapon_30::TickReloadSpeed_5DCF40();
    }
    else
    {
        --field_2_reload_speed;
    }
}

MATCH_FUNC(0x5e13e0)
void Weapon_30::army_gun_jeep_5E13E0()
{
    Ang16 gun_ang;
    Fix16_Point bullet_pos;
    Fix16_Point v41;
    Fix16_Point v42;
    if (field_2_reload_speed == 0)
    {
        field_24_pPed = field_14_car->get_driver_4118B0();

        gun_ang = field_14_car->field_0_qq.GetSpriteForModel_5A6A50(248)->field_0->field_0;

        bullet_pos.SetXY_432860(Fix16(0), dword_706EA4);
        {
            // RotateByAngle_MixOOL_40F6B0 written out, y * sin with the left-wide multiply
            Fix16 x_old = bullet_pos.x;
            Fix16 sin = Ang16::sine_40F500(gun_ang);
            Fix16 cos = Ang16::cosine_40F520(gun_ang);
            bullet_pos.x = bullet_pos.x.Multiply_408680(cos).Add_408660(MultiplyLeftWide(bullet_pos.y, sin));
            bullet_pos.y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(bullet_pos.y.Multiply_408680(cos));
        }

        v41.SetXY_432860(Fix16(0), dword_706EE8);
        RotateByAngle_40F6B0_no_budget3(v41, field_14_car->field_50_car_sprite->field_0);

        bullet_pos += v41.Add_40AC50(field_14_car->field_50_car_sprite->get_x_y());

        v42 = field_14_car->field_58_physics->GetPointVelocity_561350(&bullet_pos);

        set_field_2C_4CCA80(1);

        if (!field_4)
        {
            if (spawn_bullet_5DCF60(254, bullet_pos.x, bullet_pos.y, field_14_car->field_50_car_sprite->field_1C_zpos, gun_ang, v42))
            {
                if (field_24_pPed->IsField238_45EDE0(2) && !is_infinite_ammo_4A4FA0())
                {
                    field_0_ammo--;
                }
            }
            field_2_reload_speed = 2;

            field_24_pPed->AddThreateningPedToList_46FC70();
            if (field_24_pPed->field_15C_player)
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2u, field_24_pPed);
            }
        }
        else
        {
            spawn_bullet_5DCF60(154, bullet_pos.x, bullet_pos.y, field_14_car->field_50_car_sprite->field_1C_zpos, gun_ang, v42);
            field_2_reload_speed = 1;
        }
        TickReloadSpeed_5DCF40();
    }
    else
    {
        field_2_reload_speed--;
    }
}

// https://decomp.me/scratch/75934
MATCH_FUNC(0x5e1dc0)
void Weapon_30::oil_stain_5E1DC0()
{
    Fix16_Point vector;
    Sprite* pSprt = field_14_car->GetSprite_440840();
    vector.y = (-(pSprt->field_C_sprite_4c_ptr->GetH_447E10() + k_dword_706EB4)) / kFP16Two_706EC0;

    if (get_ammo_4A4FB0() % 2 != 0)
    {
        vector.x = -k_dword_706E6C;
    }
    else
    {
        vector.x = k_dword_706E6C;
    }

    vector.RotateByAngle_40F6B0(pSprt->field_0);
    vector += pSprt->get_x_y_443580();

    Fix16 half_depth = pSprt->field_C_sprite_4c_ptr->GetF8_492170() / 2;
    Fix16 zpos_lower = pSprt->field_1C_zpos - half_depth;
    Fix16 zpos_upper = pSprt->field_1C_zpos + half_depth;

    if (zpos_upper >= k_dword_706EDC)
    {
        zpos_upper = k_dword_706EDC - k_dword_706F70;
    }
    Fix16 found_z;
    if (gMap_0x370_6F6268->CanPlaceOilOrMine_4E5480(vector.x, vector.y, zpos_lower, zpos_upper, &found_z))
    {
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(8, vector.x, vector.y, found_z, pSprt->field_0);
        decrement_ammo_4CCA30();
        set_field_2C_4CCA80(1);
    }
}

MATCH_FUNC(0x5e2550)
void Weapon_30::car_mine_5E2550()
{
    Fix16_Point p;

    field_24_pPed = field_14_car->get_driver_4118B0();

    Sprite* Sprite_440840 = field_14_car->GetSprite_440840();

    p.y = -(dword_706FF4 + (dword_706FEC + Sprite_440840->field_C_sprite_4c_ptr->GetH_447E10()) / kFP16Two_706EC0);
    p.x = dword_706EB8;

    p.RotateByAngle_40F6B0(Sprite_440840->field_0);

    p += Sprite_440840->get_x_y();

    Fix16 half_depth = Sprite_440840->field_C_sprite_4c_ptr->field_8_depth / 2;
    Fix16 z_low = Sprite_440840->field_1C_zpos - half_depth;
    Fix16 z_high = Sprite_440840->field_1C_zpos + half_depth;
    if (z_high >= k_dword_706EDC)
    {
        z_high = k_dword_706EDC - k_dword_706F70;
    }

    Fix16 newZ;
    if (gMap_0x370_6F6268->CanPlaceOilOrMine_4E5480(p.x, p.y, z_low, z_high, &newZ))
    {
        Object_2C* pMine = gObject_5C_6F8F84->NewPhysicsObj_5299B0(10, p.x, p.y, newZ, Sprite_440840->field_0);
        pMine->SetDamageOwner_529080(field_24_pPed->field_267_varrok_idx);

        decrement_ammo_4CCA30(); // NOTE: Didn't get inlined without __forceinline here, wtf??
        set_field_2C_4CCA80(1);
    }
}

// 9.6f 0x4D0230
// 10.5 https://decomp.me/scratch/odtu0
MATCH_FUNC(0x5e2940)
void Weapon_30::car_smg_5E2940()
{
    // The original has a fifth destructible local up front (EH entry state 4), hence unused_5.
    // Declaration order and the Ang16 copy below are needed to match. tmpx/tmpy are initialised where
    // they are computed: declared up front and assigned, the caller is too big for the rotations'
    // inline budget (the first + of each RotateByAngle_40F6B0 is inlined, its other operators aren't).
    Ang16 sprite_ang;
    Fix16_Point unused_5;
    Fix16_Point left;
    Fix16_Point right;
    Fix16_Point left_point_velocity;
    Fix16_Point right_point_velocity;
    if (field_2_reload_speed == 0)
    {
        field_24_pPed = field_14_car->get_driver_4118B0();

        Sprite* pCarSprite = field_14_car->field_50_car_sprite;
        sprite_ang = field_14_car->field_50_car_sprite->field_0;

        Fix16 tmpx = dword_706DCC + field_14_car->get_car_width() / 2;
        Fix16 tmpy = dword_706FD0 + field_14_car->get_car_height() / 2;

        left.SetXY_432860(tmpx, tmpy);
        left.RotateByAngle_40F6B0(sprite_ang);
        left += pCarSprite->get_x_y_443580();

        right.SetXY_432860(-tmpx, tmpy);
        right.RotateByAngle_40F6B0(sprite_ang);
        right += pCarSprite->get_x_y_443580();

        left_point_velocity = field_14_car->field_58_physics->GetPointVelocity_561350(&left);
        right_point_velocity = field_14_car->field_58_physics->GetPointVelocity_561350(&right);

        if (!field_4)
        {
            Object_2C* pLeft = spawn_bullet_5DCF60(objects::machine_gun_bullet_254,
                                                   left.x,
                                                   left.y,
                                                   pCarSprite->field_1C_zpos,
                                                   sprite_ang,
                                                   left_point_velocity);
            Object_2C* pRight = spawn_bullet_5DCF60(objects::machine_gun_bullet_254,
                                                    right.x,
                                                    right.y,
                                                    pCarSprite->field_1C_zpos,
                                                    sprite_ang,
                                                    right_point_velocity);
            if ((pLeft || pRight) && field_24_pPed->IsField238_45EDE0(2))
            {
                DecreaseAmmo_4CCA60();
            }

            gParticle_8_6FD5E8->GunMuzzelFlash_53E970(field_14_car->field_50_car_sprite);

            field_2_reload_speed = 2;

            field_24_pPed->AddThreateningPedToList_46FC70();
            if (field_24_pPed->is_player_41B0A0())
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2, field_24_pPed);
            }
        }
        else
        {
            Ang16 tmp = sprite_ang;
            spawn_bullet_5DCF60(objects::flamethrower_fire_154, left.x, left.y, pCarSprite->field_1C_zpos, tmp, left_point_velocity);
            spawn_bullet_5DCF60(objects::flamethrower_fire_154,
                                right.x,
                                right.y,
                                pCarSprite->field_1C_zpos,
                                sprite_ang,
                                right_point_velocity);
            field_2_reload_speed = 1;
        }

        TickReloadSpeed_5DCF40();
        set_field_2C_4CCA80(1);
    }
    else
    {
        field_2_reload_speed--;
    }
}

MATCH_FUNC(0x5e33c0)
char_type Weapon_30::sub_5E33C0()
{
    char result;
    switch (this->field_1C_idx)
    {
        case weapon_type::car_bomb:
        case weapon_type::oil_stain:
        case weapon_type::car_mines:
        case weapon_type::tank_main_gun:
        case weapon_type::weapon_0x17:
            result = 0;
            break;
        default:
            result = 1;
            break;
    }
    return result;
}

MATCH_FUNC(0x5e34b0)
void Weapon_30::ChuckThrowable_5E34B0()
{
    if (field_2_reload_speed > 0)
    {
        field_2_reload_speed--;
    }
    else
    {
        if (field_24_pPed)
        {
            if (field_24_pPed->field_15C_player)
            {
                if (field_1C_idx == weapon_type::molotov || field_1C_idx == weapon_type::grenade)
                {
                    s32 obj_type = (field_1C_idx != weapon_type::molotov ? 183 : 138);
                    if (field_24_pPed->field_15C_player->IsThrowCharging_4CCB00())
                    {
                        s32 v1 = field_24_pPed->field_15C_player->GetThrowStrength_4CCAD0();
                        s32 v2 = field_24_pPed->field_15C_player->Get_Field_50();
                        throwable_5DDFC0(obj_type, v1, v2);
                    }
                    field_24_pPed->field_15C_player->ResetThrowCharge_4A5180();
                }
            }
        }
    }
}

MATCH_FUNC(0x5e3670)
void Weapon_30::pull_trigger_5E3670()
{
    switch (field_1C_idx)
    {
        case weapon_type::pistol:
            pistol_5DD860();
            break;

        case weapon_type::smg:
        case weapon_type::silence_smg:
            smg_5DDD20();
            break;

        case weapon_type::rocket:
            rocket_5E3850();
            break;

        case weapon_type::car_bomb:
            car_bomb_5E0AB0(0);
            break;

        case weapon_type::oil_stain:
            oil_stain_5E1DC0();
            break;

        case weapon_type::car_mines:
            car_mine_5E2550();
            break;

        case weapon_type::tank_main_gun:
            tank_main_gun_5E10E0();
            break;

        case weapon_type::army_gun_jeep:
            army_gun_jeep_5E13E0();
            break;

        case weapon_type::electro_batton:
            electro_batton_5E0740();
            break;

        case weapon_type::shocker:
            shocker_5E06B0();
            break;

        case weapon_type::molotov:
            if (field_24_pPed && (field_24_pPed->field_15C_player) != 0)
            {
                field_24_pPed->field_15C_player->IncrementThrowCharge_4CCAB0();
            }
            else
            {
                throwable_5DDFC0(138, 0x1E, 45);
            }
            break;

        case weapon_type::grenade:
            if (field_24_pPed && (field_24_pPed->field_15C_player) != 0)
            {
                Player* p = field_24_pPed->field_15C_player;
                p->IncrementThrowCharge_4CCAB0();

                // This is really whacky, using p results in most of these inlines being optimized out
                Player* pp = field_24_pPed->field_15C_player;

                if (pp->Get_Field_50() == 0x60)
                {
                    throwable_5DDFC0(183, field_24_pPed->field_15C_player->GetThrowStrength_4CCAD0(), pp->Get_Field_50());
                    this->field_24_pPed->field_15C_player->field_50_throw_charge = -1;
                }
            }
            else
            {
                throwable_5DDFC0(183, 0x1E, 45);
            }
            break;

        case weapon_type::dual_pistol:
            dual_pistol_5DDA70();
            break;

        case weapon_type::shotgun:
            shotgun_5DD290();
            break;

        case weapon_type::car_smg:
            car_smg_5E2940();
            break;

        case weapon_type::flamethrower:
            flamethrower_5DD0F0();
            break;

        case weapon_type::fire_truck_gun:
            fire_truck_gun_5E0E70();
            break;

        case weapon_type::fire_truck_flamethrower:
            fire_truck_flamethrower_5E0B10();
            break;

        case weapon_type::weapon_0x17:
            car_bomb_5E0AB0(1);
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5e3850)
void Weapon_30::rocket_5E3850()
{
    if (field_2_reload_speed == 0)
    {
        set_field_2C_4CCA80(1);

        if (!field_4)
        {
            Object_2C* pBullet;
            if (field_24_pPed->IsField238_45EDE0(2))
            {
                pBullet = spawn_bullet_5DCF60(128,
                                              field_24_pPed->get_cam_x(),
                                              field_24_pPed->get_cam_y(),
                                              field_24_pPed->get_cam_z(),
                                              field_24_pPed->Get_F12E_4CCA90(),
                                              field_24_pPed->GetVelocityVector_45B520());
            }
            else
            {
                if (!field_20)
                {
                    spawn_bullet_5DCF60(159,
                                        field_24_pPed->get_cam_x(),
                                        field_24_pPed->get_cam_y(),
                                        field_24_pPed->get_cam_z(),
                                        field_24_pPed->Get_F12E_4CCA90(),
                                        field_24_pPed->GetVelocityVector_45B520());
                    field_2_reload_speed = 5;
                    field_20 = 1;
                    return;
                }

                pBullet = spawn_bullet_5DCF60(128,
                                              field_24_pPed->get_cam_x(),
                                              field_24_pPed->get_cam_y(),
                                              field_24_pPed->get_cam_z(),
                                              field_24_pPed->Get_F12E_4CCA90(),
                                              field_24_pPed->GetVelocityVector_45B520());
            }

            // People get scared when someone starts firing off rockets
            field_24_pPed->AddThreateningPedToList_46FC70();

            if (pBullet)
            {
                if (field_24_pPed->IsField238_45EDE0(2))
                {
                    decrement_ammo_4CCA30();
                }
            }

            field_2_reload_speed = 50;
            gParticle_8_6FD5E8->GunMuzzelFlash_53E970(field_24_pPed->field_168_game_object->field_80_sprite_ptr);
            field_20 = 0;

            if (field_24_pPed->is_player_41B0A0())
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2u, field_24_pPed);
            }
        }
        else
        {
            spawn_bullet_5DCF60(159,
                                field_24_pPed->get_cam_x(),
                                field_24_pPed->get_cam_y(),
                                field_24_pPed->get_cam_z(),
                                field_24_pPed->Get_F12E_4CCA90(),
                                field_24_pPed->GetVelocityVector_45B520());
            field_2_reload_speed = 5;
            field_20 = 0;
        }
        TickReloadSpeed_5DCF40();
    }
    else
    {
        if (!field_20)
        {
            field_24_pPed->field_21C &= ~0x400000u;
        }
        field_2_reload_speed--;
    }
}

MATCH_FUNC(0x5e3bd0)
char_type Weapon_30::IsExplosiveWeapon_5E3BD0()
{
    char result;
    switch (this->field_1C_idx)
    {
        case weapon_type::rocket:
        case weapon_type::molotov:
        case weapon_type::car_bomb:
        case weapon_type::car_mines:
        case weapon_type::tank_main_gun:
            result = 1;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

// 9.6f 0x4CD8C0
MATCH_FUNC(0x5E3F90)
void Weapon_30::GetSoundPos_5E3F90(Fix16* pX, Fix16* pY, Fix16* pZ)
{
    if (field_24_pPed)
    {
        *pX = field_24_pPed->get_cam_x();
        *pY = field_24_pPed->get_cam_y();
        *pZ = field_24_pPed->get_cam_z();
    }
    else if (field_14_car)
    {
        *pX = field_14_car->field_50_car_sprite->GetXPos();
        *pY = field_14_car->field_50_car_sprite->GetYPos();
        *pZ = field_14_car->field_50_car_sprite->GetZPos();
    }
}