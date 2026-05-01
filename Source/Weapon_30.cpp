#include "Weapon_30.hpp"
#include "CarPhysics_B0.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
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

DEFINE_GLOBAL_INIT(Fix16, dword_706CF4, Fix16(0x1000, 0), 0x706CF4);
DEFINE_GLOBAL_INIT(Fix16, k_dword_706EC0, Fix16(0x8000, 0), 0x706EC0);
DEFINE_GLOBAL(bool, bAllowFlameSegment_706D60, 0x706D60);

// TODO: Check these for inits
DEFINE_GLOBAL_INIT(Fix16, dword_706FF4, Fix16(0x100, 0), 0x706FF4);
DEFINE_GLOBAL_INIT(Fix16, dword_706FEC, Fix16(0x1200, 0), 0x706FEC);
DEFINE_GLOBAL(Fix16, dword_706EB8, 0x706EB8);
DEFINE_GLOBAL_INIT(Fix16, k_dword_706EDC, Fix16(0x20000, 0), 0x706EDC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_706F70, Fix16(0x100, 0), 0x706F70);
DEFINE_GLOBAL_INIT(Fix16, dword_706DCC, Fix16(0xFFFFFD00, 0), 0x706DCC);
DEFINE_GLOBAL_INIT(Fix16, dword_706FD0, Fix16(0x100, 0), 0x706FD0);
DEFINE_GLOBAL_INIT(Fix16, dword_706EA4, Fix16(0x600, 0), 0x706EA4);
DEFINE_GLOBAL_INIT(Fix16, dword_706EE8, Fix16(0xFFFFEE00, 0), 0x706EE8);
DEFINE_GLOBAL_INIT(Fix16, dword_706E7C, Fix16(0x1EB, 0), 0x706E7C);
DEFINE_GLOBAL_INIT(Fix16, dword_706CF0, Fix16(0x666, 0), 0x706CF0);
DEFINE_GLOBAL_INIT(Fix16, dword_706E80, Fix16(0x147, 0), 0x706E80);

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
    field_2C = 0;
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
    field_2C = 0;
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
    if (field_0_ammo == 0xFFFF)
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
WIP_FUNC(0x5dcf60)
Object_2C* Weapon_30::spawn_bullet_5DCF60(s32 bullet_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rot, Fix16_Point& rPoint)
{
    WIP_IMPLEMENTED;

    Sprite* p5CSprite = gObject_5C_6F8F84->field_58;
    Object_2C* pNewBullet = gObject_5C_6F8F84->NewPhysicsObj_5299B0(bullet_type, xpos, ypos, zpos, rot);

    p5CSprite->set_xyz_lazy_420600(field_24_pPed->get_cam_x() + (xpos - field_24_pPed->get_cam_x()) / k_dword_706EC0,
                                   field_24_pPed->get_cam_y() + (ypos - field_24_pPed->get_cam_y()) / k_dword_706EC0,
                                   zpos);

    p5CSprite->set_ang_lazy_420690(pNewBullet->field_4->field_0);

    p5CSprite->AllocInternal_59F950(pNewBullet->field_8->field_0, dword_706CF4, pNewBullet->field_8->field_8);
    p5CSprite->SetType_4206F0(pNewBullet->field_4->get_type_416B40());
    p5CSprite->SetObj2C_482A30(pNewBullet->field_4->field_8_object_2C_ptr);

    pNewBullet->SetDamageOwner_529080(field_24_pPed->get_varrok_idx_420B50());

    if (bullet_type == 254 || bullet_type == 265)
    {
        pNewBullet->sub_5290C0(field_24_pPed->sub_45BE30());
    }

    if (p5CSprite->CheckSpriteMovementRegion_5A2500())
    {
        pNewBullet->sub_5290A0();
        bAllowFlameSegment_706D60 = 0;
        return NULL;
    }
    else
    {
        bAllowFlameSegment_706D60 = 1;
        pNewBullet->SetMovementVector_5224E0(&rPoint);
        return pNewBullet;
    }
}

// https://decomp.me/scratch/73olU
WIP_FUNC(0x5dd0f0)
void Weapon_30::flamethrower_5DD0F0()
{
    WIP_IMPLEMENTED;

    Ang16 ped_rot;
    Fix16_Point cartesian_offset;
    Fix16_Point ped_pos_maybe;

    Fix16 cam_x = field_24_pPed->get_cam_x();
    Fix16 cam_y = field_24_pPed->get_cam_y();
    Fix16 cam_z = field_24_pPed->get_cam_z();

    ped_rot = field_24_pPed->GetRotation();

    ped_pos_maybe = field_24_pPed->sub_45B520();

    cartesian_offset.FromPolar_41E210(dword_706CF4, ped_rot);

    Fix16 xpos = cam_x + cartesian_offset.x;
    Fix16 ypos = cam_y + cartesian_offset.y;

    if (!field_4)
    {
        field_2C = 1;
        bAllowFlameSegment_706D60 = 0;
        Weapon_30::spawn_bullet_5DCF60(154, xpos, ypos, cam_z, ped_rot, ped_pos_maybe);
        if (bAllowFlameSegment_706D60)
        {
            gParticle_8_6FD5E8->EmitFlameStreamSegment_53F4C0(field_24_pPed->field_168_game_object->field_80_sprite_ptr);

            if (field_24_pPed->IsField238_45EDE0(2) && field_0_ammo != 0xFFFF)
            {
                --field_0_ammo;
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

STUB_FUNC(0x5dd290)
void Weapon_30::shotgun_5DD290()
{
    NOT_IMPLEMENTED;
}

// 9.6f 0x4CE070
WIP_FUNC(0x5dd860)
void Weapon_30::pistol_5DD860()
{
    WIP_IMPLEMENTED;

    Ang16 pedRot;
    Fix16_Point charPos;

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
            charPos = field_24_pPed->sub_45B520();
            charPos.FromPolar_41E210(dword_706CF4, pedRot);
            Fix16 xx = charPos.x + x;
            Fix16 yy = charPos.y + y;
            if (spawn_bullet_5DCF60(bullet_type, xx, yy, z, pedRot, charPos))
            {
                if (field_24_pPed->IsField238_45EDE0(2))
                {
                    decrement_ammo_4CCA30();
                }
            }

            field_2_reload_speed = 20;

            gParticle_8_6FD5E8->GunMuzzelFlash_53E970(field_24_pPed->field_168_game_object->field_80_sprite_ptr);
            field_24_pPed->AddThreateningPedToList_46FC70();

            if (field_24_pPed->field_15C_player)
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
                                field_24_pPed->sub_45B520());
            field_2_reload_speed = 5;
        }
        TickReloadSpeed_5DCF40();
    }
    else
    {
        field_2_reload_speed--;
    }
}

STUB_FUNC(0x5dda70)
void Weapon_30::dual_pistol_5DDA70()
{
    NOT_IMPLEMENTED;
}

// https://decomp.me/scratch/lAo1H
WIP_FUNC(0x5ddd20)
void Weapon_30::smg_5DDD20()
{
    WIP_IMPLEMENTED;

    Fix16_Point point;
    if (field_2_reload_speed == 0)
    {
        set_field_2C_4CCA80(1);
        if (!field_4)
        {
            Ang16 AimAngle = field_24_pPed->ComputeAimAngle_45C9D0();

            point = Fix16_Point(-dword_706E7C, dword_706CF0 + dword_706E80);
            point.RotateByAngle_40F6B0(field_24_pPed->field_168_game_object->field_80_sprite_ptr->field_0);

            point = point + field_24_pPed->sub_45B520();

            if (Weapon_30::spawn_bullet_5DCF60(254,
                                               field_24_pPed->get_cam_x() + point.x,
                                               field_24_pPed->get_cam_y() + point.y,
                                               field_24_pPed->get_cam_z(),
                                               AimAngle,
                                               field_24_pPed->sub_45B520()))
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
                                           field_24_pPed->field_12E,
                                           field_24_pPed->sub_45B520());
            field_2_reload_speed = 1;
        }
        Weapon_30::TickReloadSpeed_5DCF40();
    }
    else
    {
        field_2_reload_speed--;
    }
}

STUB_FUNC(0x5ddfc0)
void Weapon_30::throwable_5DDFC0(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5de4f0)
s32 Weapon_30::sub_5DE4F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5dfb60)
char_type Weapon_30::sub_5DFB60(char_type a2, Sprite* a3, Ang16 a4)
{
    NOT_IMPLEMENTED;
    return 0;
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

STUB_FUNC(0x5e0740)
void Weapon_30::electro_batton_5E0740()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5e0ab0)
void Weapon_30::car_bomb_5E0AB0(char_type instant_bomb)
{
    field_24_pPed = field_14_car->get_driver_4118B0();

    set_field_2C_4CCA80(1);

    decrement_ammo_4CCA30();

    if (field_14_car->is_trailer_cab_41E460())
    {
        field_14_car->field_64_pTrailer->field_C_pCarOnTrailer->sub_440F90(instant_bomb);
        field_14_car->DetachTrailerAndUpdateDamage_4418B0();
    }
    else
    {
        field_14_car->sub_440F90(instant_bomb);
    }
}

STUB_FUNC(0x5e0b10)
void Weapon_30::fire_truck_flamethrower_5E0B10()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5e0e70)
void Weapon_30::fire_truck_gun_5E0E70()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5e10e0)
void Weapon_30::tank_main_gun_5E10E0()
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x5e13e0)
void Weapon_30::army_gun_jeep_5E13E0()
{
    WIP_IMPLEMENTED;

    Ang16 gun_ang;
    if (field_2_reload_speed == 0)
    {
        field_24_pPed = field_14_car->field_54_driver;

        Sprite* pGunSprite = field_14_car->field_0_qq.GetSpriteForModel_5A6A50(248)->field_0;
        gun_ang = pGunSprite->field_0;

        Fix16_Point bullet_pos;
        bullet_pos.SetXY_432860(Fix16(0), dword_706EA4);
        bullet_pos.RotateByAngle_40F6B0(gun_ang);

        Fix16_Point v41;
        v41.SetXY_432860(Fix16(0), dword_706EE8);
        v41.RotateByAngle_40F6B0(field_14_car->field_50_car_sprite->field_0);

        bullet_pos += (v41 + field_14_car->field_50_car_sprite->get_x_y_443580());

        Fix16_Point v42 = field_14_car->field_58_physics->GetPointVelocity_561350(&v41);

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

STUB_FUNC(0x5e1dc0)
void Weapon_30::oil_stain_5E1DC0()
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x5e2550)
void Weapon_30::car_mine_5E2550()
{
    WIP_IMPLEMENTED;

    field_24_pPed = field_14_car->get_driver_4118B0();

    Sprite* Sprite_440840 = field_14_car->GetSprite_440840();

    Fix16_Point p;
    p.y = -(dword_706FF4 + ((dword_706FEC + Sprite_440840->field_C_sprite_4c_ptr->field_4_height)) / k_dword_706EC0);
    p.x = dword_706EB8;

    p.RotateByAngle_40F6B0(Sprite_440840->field_0);

    Fix16_Point x_y_443580 = Sprite_440840->get_x_y_443580() + p;

    Fix16 v13 = Sprite_440840->field_C_sprite_4c_ptr->field_8;
    Fix16 v14 = Sprite_440840->field_1C_zpos + v13 / 2;
    if (v14 >= k_dword_706EDC)
    {
        v14 = k_dword_706EDC - k_dword_706F70;
    }

    Fix16 newZ;
    if (gMap_0x370_6F6268->CanPlaceOilOrMine_4E5480(x_y_443580.x, x_y_443580.y, Sprite_440840->field_1C_zpos - v13 / 2, v14, &newZ))
    {
        Object_2C* pMine = gObject_5C_6F8F84->NewPhysicsObj_5299B0(10, x_y_443580.x, x_y_443580.y, newZ, Sprite_440840->field_0);
        pMine->SetDamageOwner_529080(field_24_pPed->field_267_varrok_idx);

        decrement_ammo_4CCA30(); // NOTE: Didn't get inlined without __forceinline here, wtf??
        set_field_2C_4CCA80(1);
    }
}

// 9.6f 0x4D0230
WIP_FUNC(0x5e2940)
void Weapon_30::car_smg_5E2940()
{
    WIP_IMPLEMENTED;

    Ang16 sprite_ang;
    if (field_2_reload_speed == 0)
    {
        field_24_pPed = field_14_car->field_54_driver;

        Sprite* pCarSprite = field_14_car->field_50_car_sprite;
        sprite_ang = field_14_car->field_50_car_sprite->field_0;

        Fix16 tmpx = dword_706DCC + field_14_car->get_car_width() / 2;
        Fix16 tmpy = dword_706FD0 + field_14_car->get_car_height() / 2;

        Fix16_Point left;
        left.SetXY_432860(tmpx, tmpy);
        left.RotateByAngle_40F6B0(sprite_ang);
        left += pCarSprite->get_x_y_443580();

        Fix16_Point right;
        right.SetXY_432860(-left.x, left.y);
        right.RotateByAngle_40F6B0(sprite_ang);
        right += pCarSprite->get_x_y_443580();

        Fix16_Point left_point = field_14_car->field_58_physics->GetPointVelocity_561350(&left);
        Fix16_Point right_point = field_14_car->field_58_physics->GetPointVelocity_561350(&right);

        if (!field_4)
        {
            Object_2C* pLeft = spawn_bullet_5DCF60(254, left.x, left.y, pCarSprite->field_1C_zpos, sprite_ang, left_point);
            Object_2C* pRight = spawn_bullet_5DCF60(254, right.x, right.y, pCarSprite->field_1C_zpos, sprite_ang, right_point);
            if ((pLeft || pRight) && field_24_pPed->IsField238_45EDE0(2) && !is_infinite_ammo_4A4FA0())
            {
                field_0_ammo--;
            }

            gParticle_8_6FD5E8->GunMuzzelFlash_53E970(field_14_car->field_50_car_sprite);

            field_2_reload_speed = 2;

            field_24_pPed->AddThreateningPedToList_46FC70();
            if (field_24_pPed->is_player_41B0A0())
            {
                gShooey_CC_67A4B8->ReportCrimeForPed(2u, field_24_pPed);
            }
        }
        else
        {
            spawn_bullet_5DCF60(154, left.x, left.y, pCarSprite->field_1C_zpos, sprite_ang, left_point);
            spawn_bullet_5DCF60(154, right.x, right.y, pCarSprite->field_1C_zpos, sprite_ang, right_point);
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
                    if (field_24_pPed->field_15C_player->sub_4CCB00())
                    {
                        s32 v1 = field_24_pPed->field_15C_player->sub_4CCAD0();
                        s32 v2 = field_24_pPed->field_15C_player->Get_Field_50();
                        throwable_5DDFC0(obj_type, v1, v2);
                    }
                    field_24_pPed->field_15C_player->sub_4A5180();
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
                field_24_pPed->field_15C_player->sub_4CCAB0();
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
                p->sub_4CCAB0();

                // This is really whacky, using p results in most of these inlines being optimized out
                Player* pp = field_24_pPed->field_15C_player;

                if (pp->Get_Field_50() == 0x60)
                {
                    throwable_5DDFC0(183, field_24_pPed->field_15C_player->sub_4CCAD0(), pp->Get_Field_50());
                    this->field_24_pPed->field_15C_player->field_50 = -1;
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
                                              field_24_pPed->sub_45B520());
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
                                        field_24_pPed->sub_45B520());
                    field_2_reload_speed = 5;
                    field_20 = 1;
                    return;
                }

                pBullet = spawn_bullet_5DCF60(128,
                                              field_24_pPed->get_cam_x(),
                                              field_24_pPed->get_cam_y(),
                                              field_24_pPed->get_cam_z(),
                                              field_24_pPed->Get_F12E_4CCA90(),
                                              field_24_pPed->sub_45B520());
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
                                field_24_pPed->sub_45B520());
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