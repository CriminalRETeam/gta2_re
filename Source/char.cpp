#include "char.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "Particle_8.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "Varrok_7F8.hpp"
#include "Weapon_30.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "rng.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL(s8, byte_6FDB48, 0x6FDB48);
DEFINE_GLOBAL(s8, byte_6FDB49, 0x6FDB49);
DEFINE_GLOBAL(u32, gB4_id_6FDB4C, 0x6FDB4C);

DEFINE_GLOBAL(s32, dword_623F44, 0x623F44);
DEFINE_GLOBAL(Fix16, dword_6FD80C, 0x6FD80C);

DEFINE_GLOBAL_INIT(Ang16, word_6FD936, Ang16(720), 0x6FD936);

DEFINE_GLOBAL(u8, byte_6FDB55, 0x6FDB55);

DEFINE_GLOBAL(u8, byte_6FDB58, 0x6FDB58);
DEFINE_GLOBAL(u8, byte_6FDB59, 0x6FDB59);

DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD9E4, Fix16(0), 0x6FD9E4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7FC, k_dword_6FD9E4, 0x6FD7FC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD800, k_dword_6FD9E4, 0x6FD800);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7F8, k_dword_6FD9E4, 0x6FD7F8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9F4, Fix16(4), 0x6FD9F4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7A4, Fix16(0x1000, 0), 0x6FD7A4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7B0, k_dword_6FD9E4, 0x6FD7B0);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD7C0, k_dword_6FD9E4, 0x6FD7C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7DC, k_dword_6FD9E4, 0x6FD7DC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD868, Fix16(256, 0), 0x6FD868);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9B4, Fix16(0x2000, 0), 0x6FD9B4);
DEFINE_GLOBAL_INIT(Fix16, gRunOrJumpSpeed_6FD7D0, dword_6FD9F4* dword_6FD868, 0x6FD7D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8B4, k_dword_6FD9E4, 0x6FD8B4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8B8, k_dword_6FD9E4, 0x6FD8B8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8BC, k_dword_6FD9E4, 0x6FD8BC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8D8, Fix16(0xCCC, 0), 0x6FD8D8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD870, dword_6FD868 * 2, 0x6FD870);
DEFINE_GLOBAL_INIT(Fix16, gCollisionRepulsionSpeed_6FD7BC, dword_6FD870, 0x6FD7BC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9B0, Fix16(0x333, 0), 0x6FD9B0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDAE4, dword_6FD9B0, 0x6FDAE4);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD9EC, Fix16(2), 0x6FD9EC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD9E8, Fix16(1), 0x6FD9E8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9A8, Fix16(0x1EB, 0), 0x6FD9A8);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD9F0, Fix16(3), 0x6FD9F0);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD9F8, Fix16(5), 0x6FD9F8);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD9FC, Fix16(6), 0x6FD9FC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDA00, Fix16(7), 0x6FDA00);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDAC8, dword_6FD868 * 6, 0x6FDAC8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD99C, dword_6FD868 / dword_6FD9F4, 0x6FD99C);

DEFINE_GLOBAL(u16, gNumPedsOnScreen_6787EC, 0x6787EC);

DEFINE_GLOBAL(u8, byte_6FDB51, 0x6FDB51);
DEFINE_GLOBAL(u8, byte_6FDB52, 0x6FDB52);
DEFINE_GLOBAL(u8, byte_6FDB53, 0x6FDB53);
DEFINE_GLOBAL(u8, byte_6FDB54, 0x6FDB54);

DEFINE_GLOBAL(u8, byte_6FDB56, 0x6FDB56);
DEFINE_GLOBAL(u8, byte_623F48, 0x623F48);
DEFINE_GLOBAL(u8, byte_6FDAD8, 0x6FDAD8);
DEFINE_GLOBAL(u8, byte_6FDAD9, 0x6FDAD9);
DEFINE_GLOBAL(u8, byte_6FDB57, 0x6FDB57);

DEFINE_GLOBAL_INIT(Ang16, word_6FD808, Ang16(0), 0x6FD808);
DEFINE_GLOBAL_INIT(Ang16, word_6FD8A2, Ang16(360), 0x6FD8A2);
DEFINE_GLOBAL_INIT(Ang16, word_6FD940, Ang16(64), 0x6FD940);
DEFINE_GLOBAL_INIT(Ang16, word_6FD8F8, Ang16(1376), 0x6FD8F8);
DEFINE_GLOBAL_INIT(Ang16, word_6FD94C, Ang16(1080), 0x6FD94C);

EXTERN_GLOBAL(Ang16, word_6FDB34);
EXTERN_GLOBAL(Ped_List_4, gThreateningPedsList_678468);

//https://decomp.me/scratch/iQH9l
MATCH_FUNC(0x544F70)
void __stdcall sub_544F70()
{
    dword_6FD7F8 = k_dword_6FD9E4;
    dword_6FD800 = k_dword_6FD9E4;
    dword_6FD7FC = k_dword_6FD9E4;
    dword_6FD7DC = k_dword_6FD9E4;
    dword_6FD7B0 = k_dword_6FD9E4;
    dword_6FD8B8 = k_dword_6FD9E4;
    dword_6FD8BC = k_dword_6FD9E4;
    dword_6FD8B4 = k_dword_6FD9E4;
    byte_6FDB51 = 0;
    byte_6FDB52 = 0;
    byte_6FDB53 = 0;
    byte_6FDB54 = 0;
    byte_6FDB55 = 0;
    dword_623F44 = 1;
    byte_6FDB56 = 0;
    byte_623F48 = 1;
    byte_6FDAD8 = k_dword_6FD9E4.ToUInt8();
    byte_6FDAD9 = k_dword_6FD9E4.ToUInt8();
    byte_6FDB57 = 0;
    byte_6FDB58 = 0;
}

// https://decomp.me/scratch/ZsDjc
WIP_FUNC(0x544ff0)
Char_B4::Char_B4()
{
    field_0_id = 0;
    field_4 = 0;
    field_5_remap = -1;
    field_6 = 0;
    field_8_ped_state_1 = 11;
    field_C_ped_state_2 = 28;
    field_10_char_state = 36;
    field_14 = word_6FDB34;
    field_16 = 0;
    field_18 = 0;
    field_1C = 0;
    field_20 = 0;
    field_24 = 3;
    field_28 = word_6FDB34;
    field_2A = word_6FDB34;
    field_2C_ang = word_6FDB34;
    field_30 = 4;
    field_34 = 0;
    field_38_velocity = k_dword_6FD7C0;
    field_3C_run_or_jump_speed = gRunOrJumpSpeed_6FD7D0;
    field_40_rotation = word_6FDB34;
    field_42 = word_6FDB34;
    field_44 = 0;
    field_45 = 0;
    field_5C = 0;
    field_46 = 0; // maybe field_46_shock_counter
    field_48 = 0;
    mpNext = 0;
    field_7C_pPed = 0;
    field_80_sprite_ptr = 0;
    field_68_animation_frame = 0;
    field_69 = 0;
    field_58_flags_bf.b0 = 0;
    field_74 = word_6FDB34;
    field_6A = 0;
    field_84 = 0;
    field_88_obj_2c.sub_5A7010();
    field_8C = k_dword_6FD9E4;
    field_58_flags_bf.b2 = 0;
    field_6C_animation_state = 18;
    field_70_frame_timer = 0;
    field_71 = 0;
    field_90 = k_dword_6FD9E4;
    field_94 = k_dword_6FD9E4;
    field_58_flags_bf.b1 = 0;
    field_98.x = k_dword_6FD9E4;
    field_98.y = k_dword_6FD9E4;
    field_58_flags_bf.b3 = 0;
    field_58_flags_bf.b5 = 0;
    field_A4_xpos = k_dword_6FD9E4;
    field_A8_ypos = k_dword_6FD9E4;
    field_AC_zpos = k_dword_6FD9E4;
    field_4A = 0;
}

MATCH_FUNC(0x5451A0)
Char_B4::~Char_B4()
{
    field_18 = 0;
    field_1C = 0;
    mpNext = 0;
    field_7C_pPed = 0;
    field_80_sprite_ptr = 0;
    field_84 = 0;
}

MATCH_FUNC(0x545200)
void Char_B4::PoolAllocate()
{
    field_0_id = gB4_id_6FDB4C++;
    Char_B4::sub_5456A0();
    field_4 = 1;
    field_5_remap = -1;
    field_6 = 0;
    field_8_ped_state_1 = 11;
    field_C_ped_state_2 = 28;
    field_10_char_state = 36;
    field_14 = word_6FDB34;
    field_16 = 0;
    field_18 = 0;
    field_1C = 0;
    field_20 = 0;
    field_24 = 3;
    field_28 = word_6FDB34;
    field_2A = word_6FDB34;
    field_2C_ang = word_6FDB34;
    field_30 = 4;
    field_34 = 0;
    field_38_velocity = k_dword_6FD7C0;
    field_3C_run_or_jump_speed = gRunOrJumpSpeed_6FD7D0;
    field_40_rotation = word_6FDB34;
    field_42 = word_6FDB34;
    field_44 = 0;
    field_45 = 0;
    field_5C = 0;
    field_46 = 0;
    field_48 = 0;
    field_4A = 500;
    mpNext = 0;
    field_7C_pPed = 0;
    field_68_animation_frame = 0;
    field_69 = 0;
    field_74 = word_6FDB34;
    field_6A = 0;
    field_84 = 0;
    field_58_flags_bf.b0 = 0;
    field_88_obj_2c.sub_5A7010();
    field_8C = k_dword_6FD9E4;
    field_58_flags_bf.b2 = 0;
    field_6C_animation_state = 18;
    field_70_frame_timer = 0;
    field_71 = 0;
    field_90 = k_dword_6FD9E4;
    field_94 = k_dword_6FD9E4;
    field_58_flags_bf.b1 = 0;
    field_98.x = k_dword_6FD9E4;
    field_98.y = k_dword_6FD9E4;
    field_58_flags_bf.b5 = 0;
    field_58_flags_bf.b3 = 0;
    field_A4_xpos = k_dword_6FD9E4;
    field_A8_ypos = k_dword_6FD9E4;
    field_AC_zpos = k_dword_6FD9E4;
    field_58_flags_bf.b4 = 0;
    field_4C = k_dword_6FD9E4;
    field_50 = k_dword_6FD9E4;
    field_72 = k_dword_6FD9E4.ToInt();
    field_73 = k_dword_6FD9E4.ToInt();
    field_58_flags_bf.b6 = 0;
    field_58_flags_bf.b7 = 0;
    field_60 = 0;
    field_64 = 0;
    field_55 = 0;
    field_A0 = 0;
    field_B0 = -1;
}

MATCH_FUNC(0x5453d0)
void Char_B4::PoolDeallocate()
{
    if (field_80_sprite_ptr)
    {
        gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_80_sprite_ptr);
        gSprite_Pool_703818->remove(field_80_sprite_ptr);
        field_80_sprite_ptr = NULL;
    }
    field_88_obj_2c.sub_5A7010();
    field_B0 = -1;
}

MATCH_FUNC(0x5451C0)
bool Char_B4::sub_5451C0()
{
    if (field_8_ped_state_1 == 9)
    {
        return false;
    }

    if (field_C_ped_state_2 != 22 && field_10_char_state != Char_B4_state::Jumping_15 && field_C_ped_state_2 != 27 &&
        !field_7C_pPed->sub_433DA0())
    {
        return true;
    }

    return false;
}

MATCH_FUNC(0x545430)
void Char_B4::sub_545430()
{
    // Spawn fire
    Object_2C* p2C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(197, 0, 0, 0, word_6FDB34); // dead_rubbish_197 ?? but its actually fire
    field_80_sprite_ptr->DispatchCollisionEvent_5A3100(p2C->field_4, 0, 0, word_6FDB34);
    field_B0 = 10; // Start screaming timer
}

MATCH_FUNC(0x46DD50)
void Char_B4::SetRemap_46DD50(u8 remap)
{
    this->field_5_remap = remap;
    if (remap != 0xFF)
    {
        field_80_sprite_ptr->SetRemap(remap);
    }
}

MATCH_FUNC(0x5454B0)
void Char_B4::sub_5454B0()
{
    field_B0 = -1;
    field_88_obj_2c.CleanupSpriteList_5A7080();
}

MATCH_FUNC(0x5454d0)
void Char_B4::DoJump_5454D0()
{
    if (field_8_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (field_10_char_state == Char_B4_state::Jumping_15 && field_6C_animation_state == Char_Anim_state::Jumping_5)
        {
            if (field_68_animation_frame >= 5u)
            {
                field_68_animation_frame = 5;
                field_71 = 2;
                field_70_frame_timer = 0;
            }
        }
        else
        {
            field_10_char_state = Char_B4_state::Jumping_15;
            field_6C_animation_state = Char_Anim_state::Jumping_5;
            field_68_animation_frame = 0;
            field_38_velocity = gRunOrJumpSpeed_6FD7D0;
            field_8C = Fix16(field_80_sprite_ptr->field_1C_zpos.ToUInt8());
        }
    }
}

MATCH_FUNC(0x545530)
void Char_B4::sub_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    field_58_flags_bf.b5 = true;
    field_A4_xpos = xpos;
    field_A8_ypos = ypos;
    field_AC_zpos = zpos;
}

MATCH_FUNC(0x545570)
s32 Char_B4::IsOnWater_545570()
{
    return field_80_sprite_ptr->IsOnWater_59E1D0();
}

MATCH_FUNC(0x5455f0)
void Char_B4::sub_5455F0()
{
    field_7C_pPed->Kill_46F9D0();
}

MATCH_FUNC(0x545600)
void Char_B4::sub_545600()
{
    field_18 = 0;
    field_1C = 0;
    field_20 = 0;
    field_2C_ang = word_6FDB34;
    field_69 = 0;
    field_24 = 0;
    field_28 = word_6FDB34;
    field_2A = word_6FDB34;
    field_2C_ang = word_6FDB34;
}

MATCH_FUNC(0x5456a0)
void Char_B4::sub_5456A0()
{
    // TODO: maybe an inline here: temp var not needed
    Sprite* pFirst = gSprite_Pool_703818->get_new_sprite();
    field_80_sprite_ptr = pFirst;
    pFirst->field_30_sprite_type_enum = 3;
    pFirst->sub_59E960();
    field_80_sprite_ptr->AllocInternal_59F950(dword_6FD8D8, dword_6FD8D8, dword_6FD7A4);
    field_80_sprite_ptr->field_8_char_b4_ptr = this;
    field_80_sprite_ptr->CreateSoundObj_5A29D0();
}

MATCH_FUNC(0x545700)
bool Char_B4::IsOnScreen_545700()
{
    return gGame_0x40_67E008->sub_4B97E0(this->field_80_sprite_ptr, k_dword_6FD9E4) == 1;
}

MATCH_FUNC(0x545720)
void Char_B4::sub_545720(Fix16 a2)
{
    if (++byte_6FDB48 > 20)
    {
        byte_6FDB48 = 0;
    }
    if (field_4A > 0)
    {
        field_4A--;
    }
    dword_6FD80C = a2;

    if (field_5C > 0)
    {
        field_5C--;
    }
    sub_544F70();
    dword_6FD7F8 = field_80_sprite_ptr->field_14_xy.x;
    dword_6FD800 = field_80_sprite_ptr->field_14_xy.y;
    dword_6FD7FC = field_80_sprite_ptr->field_1C_zpos;
    dword_6FD7B0 = Fix16(field_45);
    byte_6FDB55 = 0;
    byte_6FDB58 = 0;

    gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_80_sprite_ptr);

    if (field_58_flags_bf.b5)
    {
        field_80_sprite_ptr->set_xyz_lazy_420600(field_A4_xpos, field_A8_ypos, field_AC_zpos);
        field_58_flags_bf.b5 = 0;
    }
    else
    {
        switch (field_8_ped_state_1)
        {
            case 0:
                Char_B4::state_0_54DDF0(); // Walking
                break;
            case 1:
                Char_B4::state_1_5504F0(); // Scared/Running
                break;
            case 2:
                Char_B4::state_1_5519F0(); // Running (for cops?)
                break;
            case 3:
                Char_B4::state_3_551A00(); // EnteringCar
                break;
            case 4:
                Char_B4::state_4_551B30(); // ExitingCar
                break;
            case 5:
                Char_B4::state_5_551BB0(); // ??????
                break;
            case 6:
                nullsub_28();
                break;
            case 7:
                Char_B4::state_7_551CB0(); // Standing Still?
                break;
            case 8:
                Char_B4::state_8_5520A0(); // Immobilized?
                break;
            case 9:
                Char_B4::state_9_552E90(); // Dying?
                break;
            default:
                break;
        }
        if (field_A0)
        {
            if (field_8_ped_state_1 != ped_state_1::dead_9)
            {
                field_80_sprite_ptr->field_28_num = 34;
            }
        }
        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            if (field_6C_animation_state != Char_Anim_state::Jumping_5 && field_8_ped_state_1 != ped_state_1::dead_9)
            {
                Char_B4::DoJump_5454D0();
            }
        }
        else
        {
            field_A0 = 0;
        }
        Char_B4::UpdateAnimState_546360();

        field_80_sprite_ptr->set_ang_lazy_420690(field_40_rotation);

        if (field_58_flags_bf.b3)
        {
            // clockwise?
            field_98.sub_41E210(-field_38_velocity, field_40_rotation);
        }
        else
        {
            // anti-clockwise?
            field_98.sub_41E210(field_38_velocity, field_40_rotation);
        }
    }
    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_80_sprite_ptr);
    if (field_88_obj_2c.field_0_p18)
    {
        field_88_obj_2c.PoolUpdate_5A6F70(field_80_sprite_ptr);
    }
    if (field_6A > 0)
    {
        field_6A--;
    }
}

MATCH_FUNC(0x5459c0)
void Char_B4::sub_5459C0()
{
    byte_6FDB59 = 0;
    gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);
}

MATCH_FUNC(0x5459e0)
void Char_B4::DrownPed_5459E0()
{
    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
    field_7C_pPed->ChangeNextPedState2_45C540(20);
    field_16 = 1;

    gParticle_8_6FD5E8->EmitWaterSplash_53F060(field_80_sprite_ptr->field_14_xy.x,
                                               field_80_sprite_ptr->field_14_xy.y,
                                               field_80_sprite_ptr->field_1C_zpos,
                                               word_6FD936 + field_80_sprite_ptr->field_0,
                                               1);
    if (!field_7C_pPed->field_21C_bf.b24)
    {
        field_7C_pPed->field_250 = 28;
    }
    s32 ped_killer_id = field_7C_pPed->field_204_killer_id;
    if (ped_killer_id)
    {
        if (gPedManager_6787BC->PedById(ped_killer_id))
        {
            field_7C_pPed->field_290 = 5;
            field_7C_pPed->field_264 = 50;
        }
    }
}

STUB_FUNC(0x4F79B0)
EXPORT Fix16 __stdcall SnapZTo16_4F79B0(Fix16 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// 9.6f 0x497DF0
WIP_FUNC(0x546360)
void Char_B4::UpdateAnimState_546360()
{
    WIP_IMPLEMENTED;

    s32 newId_ = 0;
    Ped* pPed = field_7C_pPed;
    s32 v124 = 0;

    field_70_frame_timer++;

    Fix16 v129 = k_dword_6FD9E4;
    Fix16 newx = k_dword_6FD9E4;
    Fix16 newy = k_dword_6FD9E4;

    s32 bUnknown = 0;
    s32 baseId;

    gmp_block_info* pBlock_;
    s32 v19;
    Car_Door_10* pDoor___;
    Fix16 v44;
    Fix16 pNewZ_;
    Ped* pDriver;
    Char_B4* pB4;
    u8 remap_;
    s32 v17;
    s32 v111;
    s32 v115;
    Fix16 v109;
    s32 bUnknown_1;
    s32 newId;
    Fix16 v102;
    Fix16 pNewZ;
    Car_Door_10* pDoor__;
    Car_Door_10* pDoor;

    // Note: was if/else
    switch (pPed->field_26C_graphic_type)
    {
        case 0:
            baseId = 0;
            break;

        case 2:
            baseId = 316;
            break;

        case 1:
        default:
            baseId = 158;
            break;
    }

    /*
    if (pPed->field_26C_graphic_type == 0)
    {
        baseId = 0;
    }
    else if (pPed->field_26C_graphic_type == 2)
    {
        baseId = 316;
    }
    else
    {
        baseId = 158;
    }*/

    char_type remap = pPed->field_244_remap;
    if (remap <= -1)
    {
        field_80_sprite_ptr->field_34 = 2;
    }
    else
    {
        field_80_sprite_ptr->SetRemap((u8)remap);
    }

    Player* pPlayer = pPed->field_15C_player;
    if (!pPlayer || field_C_ped_state_2 == 19)
    {
        Weapon_30* pWeapon = pPed->field_170_selected_weapon;
        if (pWeapon)
        {
            if ((pPed->field_21C & 0x800) != 0)
            {
                if (pWeapon->field_1C_idx == weapon_type::molotov || pWeapon->field_1C_idx == weapon_type::grenade)
                {
                    field_6C_animation_state = 4;
                    bUnknown = 1;
                }
            }
        }
    }
    else
    {
        Weapon_30* pWeapon = pPed->field_170_selected_weapon;
        if (pWeapon)
        {
            if (pWeapon->field_1C_idx == weapon_type::molotov || pWeapon->field_1C_idx == weapon_type::grenade)
            {
                bUnknown = 1;
            }

            if ((pPed->field_21C & 0x800) != 0)
            {
                //flags = pPed->field_21C;
                //if ((BYTE1(flags) & 8) != 0)
                if ((pPed->field_21C & 0x800) != 0)
                {
                    if (pWeapon->field_1C_idx == weapon_type::molotov || pWeapon->field_1C_idx == weapon_type::grenade)
                    {
                        pPed->field_21C &= ~0x800;
                        //BYTE1(flags) &= ~8u;
                        //pPed->field_21C = flags;
                    }
                }
            }
            else
            {
                if ((pWeapon->field_1C_idx == weapon_type::molotov || pWeapon->field_1C_idx == weapon_type::grenade) &&
                    pPlayer->field_8D_bWasAttackPressed)
                {
                    field_6C_animation_state = 4;
                    field_68_animation_frame = 0;
                }
            }
        }
    }

    switch (field_6C_animation_state)
    {
        case 0:
            if (field_70_frame_timer > 2u)
            {
                field_68_animation_frame++;
                if ((u8)field_68_animation_frame > 7u)
                {
                    field_68_animation_frame = 0;
                }
                field_70_frame_timer = 0;
            }

            v17 = 37;
            if (!field_7C_pPed->field_170_selected_weapon || bUnknown)
            {
                v17 = 0;
            }

            if (field_10_char_state)
            {
                newId_ = v17 + baseId + (u8)field_68_animation_frame;
            }
            else
            {
                if (field_68_animation_frame > 5u)
                {
                    field_68_animation_frame = 0;
                }
                newId_ = (u8)field_68_animation_frame + baseId + 143;
            }
            goto LABEL_277;

        case 1:
            if (field_70_frame_timer > 1u)
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7u)
                {
                    field_68_animation_frame = 0;
                }
                field_70_frame_timer = 0;
            }

            v19 = 37;
            if (!field_7C_pPed->field_170_selected_weapon || bUnknown)
            {
                v19 = 0;
            }

            if (field_10_char_state)
            {
                newId_ = (u8)field_68_animation_frame + v19 + baseId + 8;
            }
            else
            {
                if (field_68_animation_frame > 5u)
                {
                    field_68_animation_frame = 0;
                }
                newId_ = (u8)field_68_animation_frame + baseId + 135;
            }
            goto LABEL_277;

        case 2:
            if ((field_7C_pPed->field_21C & 0x800) == 0x800)
            {
                goto LABEL_55;
            }

            s32 v21;
            if (field_10_char_state == Char_B4_state::Smoking_35)
            {
                s32 f70_limit;
                if (field_68_animation_frame == 5)
                {
                    f70_limit = 40;
                    if (field_70_frame_timer == 1)
                    {
                        gParticle_8_6FD5E8->SpawnCigaretteSmokePuff_5406B0(field_80_sprite_ptr, 1);
                    }
                }
                else
                {
                    f70_limit = 3;
                }

                v21 = 4;
                if ((u32)field_70_frame_timer > f70_limit)
                {
                    field_68_animation_frame++;
                    if ((u8)field_68_animation_frame > 7u)
                    {
                        field_68_animation_frame = 0;
                        field_10_char_state = 7;
                    }
                    field_70_frame_timer = 0;
                }
            }
            else
            {
                if (field_70_frame_timer > 8u)
                {
                    field_68_animation_frame++;
                    if ((u8)field_68_animation_frame > 3u)
                    {
                        field_68_animation_frame = 0;
                    }
                    field_70_frame_timer = 0;
                }
                v21 = 0;
            }
            newId_ = (u8)field_68_animation_frame + v21 + baseId + 53;
            goto LABEL_277;

        case 3:
        LABEL_55:
            newId_ = baseId + 139;
            goto LABEL_277;

        case 4:
            if (field_38_velocity <= k_dword_6FD7C0)
            {
                if ((u8)field_70_frame_timer > 2u)
                {
                    field_68_animation_frame++;
                    if ((u8)field_68_animation_frame > 7u)
                    {
                        field_68_animation_frame = 0;
                    }
                    field_70_frame_timer = 0;
                }
                newId_ = (u8)field_68_animation_frame + baseId + 115;
            }
            else
            {
                if ((u8)field_70_frame_timer > 2u)
                {
                    field_68_animation_frame++;
                    if (field_68_animation_frame > 7u)
                    {
                        field_68_animation_frame = 0;
                    }
                    field_70_frame_timer = 0;
                }
                newId_ = (u8)field_68_animation_frame + baseId + 123;
            }
            goto LABEL_277;

        case Char_Anim_state::Jumping_5:
            if (field_7C_pPed->field_15C_player)
            {
                field_71 = 2;
            }
            else
            {
                field_71 = 1;
            }

            switch (field_68_animation_frame)
            {
                case 0:
                    field_54 = 0;
                    dword_6FD7FC += dword_6FDAE4;
                    dword_6FD7FC = SnapZTo16_4F79B0(dword_6FD7FC);
                    if ((this->field_58_flags & 1) == 0)
                    {
                        goto LABEL_226;
                    }
                    goto LABEL_225;
                case 1:
                case 2:
                case 3:
                case 4:
                    dword_6FD7FC += dword_6FDAE4;
                    dword_6FD7FC = SnapZTo16_4F79B0(dword_6FD7FC);
                    if ((this->field_58_flags & 1) != 0)
                    {
                    LABEL_225:
                        byte_6FDB54 = 0;
                    }
                    goto LABEL_226;
                case 5:
                    this->field_71 = 3;
                    this->field_54 = 12;
                    goto LABEL_227;
                case 6:
                case 7:
                    v129 = *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&v129,
                                                                          this->field_80_sprite_ptr->field_14_xy.x,
                                                                          this->field_80_sprite_ptr->field_14_xy.y);
                    gMap_0x370_6F6268->UpdateZFromSlopeAtCoord_4E5BF0(field_80_sprite_ptr->field_14_xy.x,
                                                                      field_80_sprite_ptr->field_14_xy.y,
                                                                      v129);
                    v109 = (this->field_80_sprite_ptr->field_1C_zpos - v129) / k_dword_6FD9EC;
                    if (v109 <= k_dword_6FD9E4)
                    {
                        dword_6FD7FC = v129;
                    }
                    else
                    {
                        dword_6FD7FC -= v109;
                        dword_6FD7FC = SnapZTo16_4F79B0(dword_6FD7FC);
                    LABEL_226:
                        Char_B4::sub_548590();
                    }

                LABEL_227:
                    field_54++;
                    v111 = field_68_animation_frame + baseId + 16;
                    if (field_70_frame_timer > (u32)field_71)
                    {
                        field_70_frame_timer = 0;
                        field_68_animation_frame++;
                    }

                    field_80_sprite_ptr->set_id_lazy_4206C0(197);
                    field_80_sprite_ptr->set_id_lazy_4206C0(v111);

                    if (field_54 < 0xCu)
                    {
                        v115 = field_54;
                    LABEL_235:
                        field_80_sprite_ptr->sub_59E4C0((k_dword_6FD9E8 + ((dword_6FD9A8 * v115))), 0);
                        return;
                    }
                    if (field_54 < 24)
                    {
                        v115 = 25 - (u8)this->field_54;
                        goto LABEL_235;
                    }
                    break;

                case 8:
                    this->field_10_char_state = 1;
                    this->field_6C_animation_state = 2;
                    this->field_68_animation_frame = 0;
                    newId = baseId + 23;
                    v129 = *gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&v129,
                                                                          this->field_80_sprite_ptr->field_14_xy.x,
                                                                          this->field_80_sprite_ptr->field_14_xy.y);
                    gMap_0x370_6F6268->UpdateZFromSlopeAtCoord_4E5BF0(field_80_sprite_ptr->field_14_xy.x,
                                                                      field_80_sprite_ptr->field_14_xy.y,
                                                                      v129);
                    v102 = this->field_80_sprite_ptr->field_1C_zpos - v129;
                    if (v102 <= k_dword_6FD9E4)
                    {
                        dword_6FD7FC = v129;
                    }
                    else
                    {
                        dword_6FD7FC -= v102;
                        dword_6FD7FC = SnapZTo16_4F79B0(dword_6FD7FC);
                        Char_B4::sub_548590();
                    }

                    if (field_80_sprite_ptr->field_1C_zpos < field_8C)
                    {
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x, newy, field_8C); // ypos ??
                    }

                    field_80_sprite_ptr->set_id_lazy_4206C0(newId);
                    return;

                default:
                    goto LABEL_227;
            }
            return;

        case Char_Anim_state::Entering_Car_6:
        case 9:
            field_84->field_76 = 0;
            if ((u8)field_68_animation_frame > 12u)
            {
                goto LABEL_125;
            }
            break;

        case Char_Anim_state::Exiting_Car_7:
            field_84->field_76 = 0;
            switch (field_68_animation_frame)
            {
                case 0u:
                    if ((this->field_58_flags & 0x10) != 0)
                    {
                        Car_Door_10* pDoor = field_84->GetDoor(field_7C_pPed->field_24C_target_car_door);
                        pDoor->sub_439E60();
                        newId_ = baseId + (u8)this->field_68_animation_frame;
                        v124 = 0;
                        CarDoorAlignmentSolver_545AF0(0, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(7, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                        v124 = 3;
                        this->field_80_sprite_ptr->field_28_num = 9;
                        newId_ = baseId + 36;
                    }
                    goto LABEL_162;

                case 1u:
                    if ((this->field_58_flags & 0x10) != 0)
                    {
                        newId_ = baseId + field_68_animation_frame;
                        v124 = 0;
                        CarDoorAlignmentSolver_545AF0(1, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(6, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                        v124 = 3;
                        this->field_80_sprite_ptr->field_28_num = 9;
                        newId_ = baseId + 35;
                    }
                    goto LABEL_162;

                case 2u:
                    if ((this->field_58_flags & 0x10) != 0)
                    {
                        newId_ = baseId + field_68_animation_frame;
                        v124 = 0;
                        CarDoorAlignmentSolver_545AF0(2, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(5, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                        v124 = 3;
                        this->field_80_sprite_ptr->field_28_num = 9;
                        newId_ = baseId + 34;
                    }
                    goto LABEL_162;

                case 3u:
                    if ((this->field_58_flags & 0x10) != 0)
                    {
                        newId_ = baseId + field_68_animation_frame;
                        v124 = 0;
                        CarDoorAlignmentSolver_545AF0(3, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(4, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                        v124 = 3;
                        this->field_80_sprite_ptr->field_28_num = 9;
                        newId_ = baseId + 33;
                    }
                    goto LABEL_162;

                case 4u:
                    if ((this->field_58_flags & 0x10) != 0)
                    {
                        newId_ = baseId + field_68_animation_frame;
                        v124 = 0;
                        CarDoorAlignmentSolver_545AF0(4, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                        goto LABEL_162;
                    }
                    CarDoorAlignmentSolver_545AF0(0, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    goto LABEL_191;

                case 5u:
                    if ((this->field_58_flags & 0x10) != 0)
                    {
                        newId_ = baseId + field_68_animation_frame;
                        v124 = 0;
                        CarDoorAlignmentSolver_545AF0(5, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(1, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                        v124 = 1;
                        this->field_80_sprite_ptr->field_28_num = 23;
                        newId_ = (u8)this->field_68_animation_frame + baseId + 24;
                    }
                    goto LABEL_162;
                case 6u:
                    if ((this->field_58_flags & 0x10) != 0)
                    {
                        newId_ = baseId + field_68_animation_frame;
                        v124 = 0;
                        CarDoorAlignmentSolver_545AF0(6, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(2, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                        v124 = 1;
                        this->field_80_sprite_ptr->field_28_num = 23;
                        newId_ = (u8)this->field_68_animation_frame + baseId + 24;
                    }
                    goto LABEL_162;
                case 7u:
                    if ((this->field_58_flags & 0x10) != 0)
                    {
                        newId_ = baseId + field_68_animation_frame;
                        v124 = 0;
                        CarDoorAlignmentSolver_545AF0(7, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(3, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    LABEL_191:
                        v124 = 1;
                        this->field_80_sprite_ptr->field_28_num = 23;
                        newId_ = (u8)this->field_68_animation_frame + baseId + 24;
                    }
                LABEL_162:
                    pNewZ = *gMap_0x370_6F6268->sub_4E4E50(&pNewZ, newx, newy, field_84->field_50_car_sprite->field_1C_zpos);

                    field_80_sprite_ptr->set_xyz_lazy_420600(newx, newy, pNewZ);

                    if (field_70_frame_timer > v124)
                    {
                        goto LABEL_167;
                    }

                    if (field_70_frame_timer == 1 && !this->field_68_animation_frame)
                    {
                        Car_Door_10* pDoor_ = field_84->GetDoor(field_7C_pPed->field_24C_target_car_door);
                        if ((this->field_58_flags & 0x10) == 0)
                        {
                            goto LABEL_139;
                        }
                    }
                    goto LABEL_277;

                case 8u:
                    field_80_sprite_ptr->field_28_num = 23;
                    field_7C_pPed->ChangeNextPedState2_45C540(0);
                    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::walking_0);
                    this->field_C_ped_state_2 = 0;
                    this->field_8_ped_state_1 = 0;
                    pDoor__ = field_84->GetDoor(field_7C_pPed->field_24C_target_car_door);
                    if ((this->field_58_flags & 0x10) == 0)
                    {
                        pDoor__->sub_439EA0();
                    }

                    pDoor__->field_8_pObj = 0;

                    if (Char_B4::IsOnWater_545570())
                    {
                        field_7C_pPed->PutOutFire();
                        Char_B4::DrownPed_5459E0();
                    }
                    else
                    {
                        gmp_block_info* pBlock =
                            gMap_0x370_6F6268->get_block_4DFE10(this->field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                this->field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                (this->field_80_sprite_ptr->field_1C_zpos.ToInt()) - 1);
                        if (pBlock && ((pBlock->field_B_slope_type & 0xFC) != 0) && (pBlock->field_B_slope_type & 0xFCu) < 0xB4 &&
                            (pBlock->field_B_slope_type & 3) != 0)
                        {
                            byte_6FDB54 = 1;
                            Char_B4::sub_548590();
                        }
                        else
                        {
                            byte_6FDB54 = 0;
                            Char_B4::sub_548590();
                        }
                    }
                    return;
                default:
                    goto LABEL_162;
            }

        case 8:
            field_84->field_76 = 0;

            // Note: was if/else
            switch (field_68_animation_frame)
            {
                case 0: // and default?
                    CarDoorAlignmentSolver_545AF0(4, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    break;

                case 1:
                    CarDoorAlignmentSolver_545AF0(2, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    break;

                case 2:
                    CarDoorAlignmentSolver_545AF0(99, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
                    break;
            }

            field_80_sprite_ptr->set_xyz_lazy_420600(newx, newy, field_84->field_50_car_sprite->field_1C_zpos);

            this->field_80_sprite_ptr->field_28_num = 6;

            pBlock_ = gMap_0x370_6F6268->get_block_4DFE10(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                          field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                          field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);
            bUnknown_1 = 0;
            if (pBlock_)
            {
                if ((pBlock_->field_B_slope_type & 0xFC) != 0 && (pBlock_->field_B_slope_type & 0xFCu) < 0xB4 &&
                    (pBlock_->field_B_slope_type & 3) != 0)
                {
                    bUnknown_1 = 1;
                }
            }
            byte_6FDB54 = bUnknown_1;
            Char_B4::sub_548590();
            if (field_70_frame_timer > 2u)
            {
                field_70_frame_timer = 0;
                field_68_animation_frame = field_68_animation_frame + 1;
            }
            newId_ = baseId + 36;
            goto LABEL_277;

        case 10:
            newId_ = baseId + 72;
            goto LABEL_277;

        case Char_Anim_state::Normal_Fall_11:
        case Char_Anim_state::Letal_Fall_12:
            field_71 = 1;
            switch (field_68_animation_frame)
            {
                case 0u:
                case 1u:
                case 2u:
                case 3u:
                case 4u:
                case 5u:
                case 6u:
                case 7u:
                case 8u:
                case 9u:
                    goto LABEL_240;
                case 0xAu:
                case 0xBu:
                case 0xCu:
                case 0xDu:
                    field_7C_pPed->TakeDamage(4);
                LABEL_240:
                    newId_ = field_68_animation_frame + baseId + 81;
                    break;
                case 0xEu:
                case 0xFu:
                case 0x10u:
                    field_6C_animation_state = Char_Anim_state::Letal_Fall_12;
                    newId_ = field_68_animation_frame + baseId + 81;
                    break;
                default:
                    break;
            }

            if (field_70_frame_timer > (u32)field_71)
            {
                if (field_68_animation_frame < 16u)
                {
                    field_68_animation_frame++;
                }
                field_70_frame_timer = 0;
            }
            goto LABEL_277;

        case 13:
            newId_ = baseId + 72;
            goto LABEL_277;

        case 14:
            newId_ = baseId + 80;
            goto LABEL_277;

        case 15:
            if ((u32)this->field_70_frame_timer > (u8)(4 - (field_68_animation_frame > 5u)))
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7u)
                {
                    s16 max_rng = 2;
                    s32 rng_val = stru_6F6784.get_int_4F7AE0((s16*)&max_rng);
                    // NOTE: was if else
                    switch (rng_val)
                    {
                        case 0:
                            field_6C_animation_state = 19;
                            break;

                        case 1:
                            field_6C_animation_state = 13;
                            break;
                    }
                    field_68_animation_frame = 7;
                }
            }
            newId_ = (u8)field_68_animation_frame + baseId + 65;
            goto LABEL_277;

        case 16:
            if ((u32)field_70_frame_timer > (u8)(4 - (field_68_animation_frame > 5u)))
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7u)
                {
                    s16 max_rng_ = 3;
                    s32 rng_val_ = stru_6F6784.get_int_4F7AE0((s16*)&max_rng_);
                    // NOTE: Was if else
                    switch (rng_val_)
                    {
                        case 0:
                            field_6C_animation_state = 14;
                            break;

                        case 1:
                            field_6C_animation_state = 19;
                            break;

                        case 2:
                            field_6C_animation_state = 20;
                            break;
                    }
                    field_68_animation_frame = 7;
                }
            }
            newId_ = (u8)field_68_animation_frame + baseId + 73;
            goto LABEL_277;

        case 17:
            if (field_70_frame_timer > 3u)
            {
                field_68_animation_frame++;
                if ((u8)field_68_animation_frame > 4u)
                {
                    field_68_animation_frame = 0;
                }
                field_70_frame_timer = 0;
            }

            if (field_68_animation_frame)
            {
                field_80_sprite_ptr->field_34 = 2;
            }
            field_80_sprite_ptr->field_28_num = 6;
            newId_ = (u8)field_68_animation_frame + baseId + 151;
            goto LABEL_277;
        case 19:
            newId_ = baseId + 156;
            goto LABEL_277;
        case 20:
            newId_ = baseId + 157;
            goto LABEL_277;
        case 21:
            newId_ = baseId + 155;
            goto LABEL_277;
        default:
            goto LABEL_277;
    }

    switch (field_68_animation_frame)
    {
        case 0:
            pDoor___ = field_84->GetDoor(field_7C_pPed->field_24C_target_car_door);
            if ((this->field_58_flags & 0x10) != 0)
            {
                v124 = 1;
                pDoor___->sub_439E60();
                newId_ = baseId + (u8)field_68_animation_frame;
                CarDoorAlignmentSolver_545AF0(7, field_84, field_7C_pPed->field_24C_target_car_door, newx, newy, field_40_rotation);
            }
            else
            {
                if (field_70_frame_timer == 1)
                {
                    if (pDoor___->field_4_state)
                    {
                        if (pDoor___->field_4_state != 6)
                        {
                            field_68_animation_frame = 4;
                            field_71 = 0;
                        }
                    }
                }
                CarDoorAlignmentSolver_545AF0(field_68_animation_frame,
                                              field_84,
                                              field_7C_pPed->field_24C_target_car_door,
                                              newx,
                                              newy,
                                              field_40_rotation);
                v124 = 5;
                newId_ = (u8)field_68_animation_frame + baseId + 24;
            }
            goto LABEL_125;

        case 1:
        case 2:
        case 3:
            if ((this->field_58_flags & 0x10) != 0)
            {
                v124 = 1;
                CarDoorAlignmentSolver_545AF0(7 - field_6C_animation_state,
                                              field_84,
                                              field_7C_pPed->field_24C_target_car_door,
                                              newx,
                                              newy,
                                              field_40_rotation);
                field_80_sprite_ptr->field_28_num = 9;
                newId_ = baseId + (u8)field_68_animation_frame;
            }
            else
            {
                CarDoorAlignmentSolver_545AF0(field_6C_animation_state,
                                              field_84,
                                              field_7C_pPed->field_24C_target_car_door,
                                              newx,
                                              newy,
                                              field_40_rotation);
                v124 = 3;
                newId_ = (u8)field_68_animation_frame + baseId + 24;
            }
            goto LABEL_125;

        case 4:
        case 5:
        case 6:
        case 7:
            if ((this->field_58_flags & 0x10) != 0)
            {
                v124 = 1;
                CarDoorAlignmentSolver_545AF0(7 - field_6C_animation_state,
                                              field_84,
                                              field_7C_pPed->field_24C_target_car_door,
                                              newx,
                                              newy,
                                              field_40_rotation);
                newId_ = baseId + (u8)field_68_animation_frame;
                field_80_sprite_ptr->field_28_num = 9;
            }
            else
            {
                CarDoorAlignmentSolver_545AF0(field_6C_animation_state,
                                              field_84,
                                              field_7C_pPed->field_24C_target_car_door,
                                              newx,
                                              newy,
                                              field_40_rotation);
                v124 = 1;
                field_80_sprite_ptr->field_28_num = 9;
                newId_ = (u8)field_68_animation_frame + baseId + 28;
            }
            goto LABEL_125;

        case 8:
            CarDoorAlignmentSolver_545AF0(field_6C_animation_state,
                                          field_84,
                                          field_7C_pPed->field_24C_target_car_door,
                                          newx,
                                          newy,
                                          field_40_rotation);
            field_80_sprite_ptr->field_28_num = 9;
            field_7C_pPed->ChangeNextPedState2_45C540(10);
            field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
            field_C_ped_state_2 = 10;
            field_8_ped_state_1 = 10;
            return;

        case 9:
            pDriver = field_84->field_54_driver;
            v124 = 2;
            newId_ = (u8)field_6C_animation_state + baseId + 19;

            if (!pDriver)
            {
                goto LABEL_123;
            }

            if (pDriver->field_240_occupation == ped_ocupation_enum::unknown_2)
            {
                pDriver = gPedManager_6787BC->sub_470D60();
                if (field_84->field_84_car_info_idx == car_model_enum::apc || field_84->field_84_car_info_idx == car_model_enum::JEEP ||
                    field_84->field_84_car_info_idx == car_model_enum::TANK)
                {
                    pDriver->field_26C_graphic_type = 2;
                    pDriver->field_244_remap = 4;
                }
                goto LABEL_114;
            }

            if (pDriver->field_240_occupation == ped_ocupation_enum::driver)
            {
                pDriver->field_240_occupation = ped_ocupation_enum::driver_2;
                pDriver->field_140 = field_84;
                pDriver->field_180 = field_7C_pPed;
                goto LABEL_115;
            }

            if (pDriver->field_240_occupation != ped_ocupation_enum::police)
            {
                if (pDriver->field_17C_pZone)
                {
                    pDriver->sub_463830(20, 9999);
                    pDriver->field_14C = field_7C_pPed;
                }
            LABEL_114:
                pDriver->field_140 = field_84;
                pDriver->field_180 = field_7C_pPed;
                goto LABEL_115;
            }

            if (field_7C_pPed->field_20A_wanted_points < 600)
            {
                gPolice_7B8_6FEE40->sub_56F940(field_7C_pPed);
                field_7C_pPed->field_20A_wanted_points = 600;
            }

            gPolice_7B8_6FEE40->sub_5708C0(field_7C_pPed);
            if (pDriver->field_258_objective == objectives_enum::objective_43)
            {
                gPolice_7B8_6FEE40->sub_5707B0(pDriver->field_16C_car, this->field_7C_pPed);
            }

            pDriver->SetObjective(objectives_enum::no_obj_0, 9999);

        LABEL_115:
            if (field_7C_pPed->field_25C_car_state == 35 && field_7C_pPed->field_226 == 1)
            {
                field_7C_pPed->field_226 = 0;
            }
            pDriver->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
            pDriver->ChangeNextPedState2_45C540(17);
            field_84->ClearDriver_4407F0();
            CarDoorAlignmentSolver_545AF0(8, field_84, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
            v44 = *gMap_0x370_6F6268->sub_4E4E50(&v44, newx, newy, field_84->field_50_car_sprite->field_1C_zpos);
            pDriver->AllocCharB4_45C830(newx, newy, v44);
            pDriver->field_168_game_object->set_rotation_433A30(field_84->field_50_car_sprite->field_0);
            pDriver->field_168_game_object->Set_F8_ped_state_1_433910(8);
            pDriver->field_24C_target_car_door = field_7C_pPed->get_target_car_door_403A60();
            pDriver->field_168_game_object->field_C_ped_state_2 = 17;
            pDriver->field_168_game_object->field_84 = field_84;
            pDriver->field_168_game_object->field_80_sprite_ptr->field_28_num = 6;

            pB4 = pDriver->field_168_game_object;
            remap_ = pDriver->get_remap_433BA0();
            pDriver->SetRemap_433C10(remap_);
            pDriver->field_16C_car = 0;

            pDriver->Set_B4_F16_To_1_433B50();
            if (!pDriver->is_player_41B0A0())
            {
                pDriver->Set_F250_IfBit_433DD0(12);
            }

        LABEL_123:
            CarDoorAlignmentSolver_545AF0(field_68_animation_frame,
                                          field_84,
                                          field_7C_pPed->get_target_car_door_403A60(),
                                          newx,
                                          newy,
                                          field_40_rotation);

        LABEL_125:
            pNewZ_ = *gMap_0x370_6F6268->sub_4E4E50(&pNewZ_, newx, newy, field_84->field_50_car_sprite->field_1C_zpos);
            field_80_sprite_ptr->set_xyz_lazy_420600(newx, newy, pNewZ_);

            if ((this->field_58_flags & 0x10) != 0)
            {
                field_40_rotation = field_40_rotation + word_6FD936;
            }

            if (field_70_frame_timer > v124)
            {
            LABEL_167:
                field_70_frame_timer = 0;
                field_68_animation_frame = field_68_animation_frame + 1;
                if (field_68_animation_frame == 12)
                {
                    field_68_animation_frame = 4;
                }
            }
            else if (field_70_frame_timer == 5 && !field_68_animation_frame)
            {
                pDoor = field_84->GetDoor(field_7C_pPed->field_24C_target_car_door);
                if ((this->field_58_flags & 0x10) == 0)
                {
                LABEL_139:
                    pDoor->sub_439E60();
                    pDoor->field_8_pObj = field_7C_pPed;
                }
            }
        LABEL_277:
            field_80_sprite_ptr->set_id_lazy_4206C0(newId_);
            return;

        case 10:
        case 11:
        case 12:
            CarDoorAlignmentSolver_545AF0(field_6C_animation_state,
                                          field_84,
                                          field_7C_pPed->field_24C_target_car_door,
                                          newx,
                                          newy,
                                          field_40_rotation);
            v124 = 2;
            newId_ = (u8)field_68_animation_frame + baseId + 19;
            goto LABEL_125;
    }
}

STUB_FUNC(0x548590)
void Char_B4::sub_548590()
{
    NOT_IMPLEMENTED;
}

// 9.6f 0x499F00
MATCH_FUNC(0x548670)
void Char_B4::sub_548670(char_type a2)
{
    u8 bUnknown; // bl
    if (this->field_69 == 1 && this->field_20 && this->field_24 != 3)
    {
        bUnknown = 1;
    }
    else
    {
        bUnknown = a2;
    }

    Sprite* pNearSprite = field_80_sprite_ptr->sub_59E7D0(2);
    if (pNearSprite)
    {
        s32 sprite_type_enum = pNearSprite->field_30_sprite_type_enum;
        switch (pNearSprite->get_type_416B40())
        {

            case 3: // char_b4
                if (bUnknown == byte_623F48)
                {
                    this->field_18 = 0;
                }
                else
                {
                    if (field_7C_pPed->field_238 >= 2 && field_7C_pPed->field_238 <= 6)
                    {
                        Char_B4::sub_548BD0(pNearSprite->AsCharB4_40FEA0());
                        this->field_18 = 0;
                    }
                }
                break;

            case 2: // car
            {
                Sprite* pNearSprite2 = field_80_sprite_ptr->sub_59E7D0(1);
                if (pNearSprite2->get_type_416B40() == 1) // object
                {
                    if (pNearSprite2->As2C_40FEC0())
                    {
                        Char_B4::sub_548840(pNearSprite2->As2C_40FEC0());
                        this->field_20 = 3;
                    }
                }
                else
                {
                    Char_B4::sub_54A530(pNearSprite->AsCar_40FEB0(), 0, 0);
                    this->field_20 = 1;
                }
                break;
            }
            case 1:
            case 4:
            case 5: // object
                Char_B4::sub_548840(pNearSprite->As2C_40FEC0());
                this->field_20 = 3;
                break;

            default:
                return;
        }
    }
    else
    {
        if (field_10_char_state != 10)
        {
            if (field_10_char_state == Char_B4_state::Colliding_With_Car_27)
            {
                this->field_10_char_state = 1;
                this->field_1C = 0;
                this->field_20 = 0;
                this->field_18 = 0;
            }
            else
            {
                this->field_18 = 0;
            }
        }
        else
        {
            this->field_10_char_state = 1;
            this->field_18 = 0;
        }

        if (field_38_velocity == k_dword_6FD7C0 || field_7C_pPed->IsField238_45EDE0(2) && !this->field_8_ped_state_1)
        {
            this->field_1C = 0;
            this->field_20 = 0;
            this->field_69 = 0;
            this->field_2A = word_6FDB34;
        }
    }
}

STUB_FUNC(0x548840)
void Char_B4::sub_548840(Object_2C* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x548bd0)
void Char_B4::sub_548BD0(Char_B4* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x54a530)
void Char_B4::sub_54A530(Car_BC* a2, Car_BC* a3, s32 a4)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x54b8f0)
char_type Char_B4::sub_54B8F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x54c090)
s32 Char_B4::sub_54C090()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x54c1a0)
char_type Char_B4::sub_54C1A0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x54c3e0)
char_type Char_B4::sub_54C3E0()
{
    NOT_IMPLEMENTED;
    return 0;
}

// 9.6f 0x495540
WIP_FUNC(0x54c500)
char_type Char_B4::sub_54C500(char_type x, char_type y)
{
    WIP_IMPLEMENTED;

    Fix16 tx = field_80_sprite_ptr->field_14_xy.x;
    char_type dx = x - tx.ToInt();
    Fix16 ty = field_80_sprite_ptr->field_14_xy.y;
    char_type dy = y - ty.ToInt();

    // No movement
    if (dx == 0 && dy == 0)
    {
        return 1;
    }

    // Diagonal movement not allowed
    if (dx != 0 && dy != 0)
    {
        return 0;
    }

    // Pure horizontal
    if (dx != 0)
    {
        return (dx == -1) ? Char_B4::sub_54C1A0(4) : Char_B4::sub_54C1A0(3);
    }

    return (dy == -1) ? Char_B4::sub_54C1A0(1) : Char_B4::sub_54C1A0(2);
}

STUB_FUNC(0x54c580)
void Char_B4::sub_54C580()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x54c6c0)
s16 Char_B4::sub_54C6C0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x54c900)
s16 Char_B4::sub_54C900()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x54cae0)
s16 Char_B4::sub_54CAE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x54cc40)
void Char_B4::sub_54CC40()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x54dd70)
void Char_B4::sub_54DD70()
{
    if (this->field_8_ped_state_1 != ped_state_1::dead_9 && this->field_10_char_state != Char_B4_state::Jumping_15)
    {
        sub_5459C0();

        if (field_7C_pPed->check_bit_11() && field_7C_pPed->field_21C_bf.b9)
        {
            if (this->field_6C_animation_state != 4)
            {
                this->field_6C_animation_state = 4;
                this->field_68_animation_frame = 0;
            }
            else
            {
                field_7C_pPed->HandleClosePedInteraction_45CAA0();
            }
        }
        else if (this->field_6C_animation_state != 4 || this->field_68_animation_frame == 6)
        {
            if (field_38_velocity != k_dword_6FD7C0)
            {
                this->field_6C_animation_state = field_38_velocity > gCollisionRepulsionSpeed_6FD7BC;
            }
            else
            {
                this->field_6C_animation_state = 2;
            }
        }
    }
}

// https://decomp.me/scratch/56qkT
WIP_FUNC(0x54ddf0)
void Char_B4::state_0_54DDF0()
{
    WIP_IMPLEMENTED;
    bool v3;
    u8 v74;
    char v88;
    char v89;
    u8 v91;

    byte_6FDB51 = 1;
    byte_6FDB52 = 1;
    byte_6FDB53 = 0;
    byte_6FDB54 = 0;
    field_58_flags = this->field_58_flags;
    this->field_4A = 500;
    v3 = (field_58_flags & 1) == 0;
    //v4 = dword_6FD7FC;
    v89 = 1;
    v88 = 0;
    u8 v87 = 1;
    Ang16 v93(0);
    Ang16 v95(0); //v95 = 0;

    Fix16 unknown_v97(0);

    u8 block_type;
    gmp_block_info* pBlock;

    if ((field_58_flags & 1) == 0 && dword_6FD7FC.GetFracValue() == k_dword_6FD9E4)
    {
        //goto LABEL_9;
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), dword_6FD7FC.ToInt() - 1);
        pBlock = gMap_0x370_6F6268->get_block_4DFE10(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), dword_6FD7FC.ToInt() - 1);
    }
    else
    {
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), dword_6FD7FC.ToInt());
        pBlock = gMap_0x370_6F6268->get_block_4DFE10(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), dword_6FD7FC.ToInt());

        if (pBlock == NULL || block_type == 0)
        {
            //LABEL_9:
            block_type =
                gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), dword_6FD7FC.ToInt() - 1);
            pBlock = gMap_0x370_6F6268->get_block_4DFE10(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), dword_6FD7FC.ToInt() - 1);
        }
    }

    if (pBlock)
    {
        u8 v10 = get_slope_bits(pBlock->field_B_slope_type) != 0 &&
            get_slope_bits(pBlock->field_B_slope_type) != 0xFC; //(pBlock->field_B_slope_type & 0xFC) != 0xFC;
        field_58_flags = field_58_flags ^ (v10 ^ (u8)this->field_58_flags) & 1;
        //v5 = this->field_58_flags ^ (v10 ^ (u8)this->field_58_flags) & 1;
        //this->field_58_flags = v5;
        if (gGtx_0x106C_703DD4->IsElectrifiedFloorType_491F80(pBlock->field_8_lid & 0x3FF) &&
            field_10_char_state != Char_B4_state::Jumping_15)
        {
            //if ((field_7C_pPed->field_21C & 0x8000000) == 0)
            if (field_7C_pPed->field_21C_bf.b27 == 0)
            {
                field_7C_pPed->field_210_shock_counter += 3;
                s32 Leader_id = field_7C_pPed->field_204_killer_id;
                if (Leader_id != 0)
                {
                    if (gPedManager_6787BC->PedById(Leader_id))
                    {
                        field_7C_pPed->field_290 = 2;
                        field_7C_pPed->field_264 = 50;
                    }
                }
            }
        }
    }
    if (Char_B4::IsOnWater_545570())
    {
        field_7C_pPed->PutOutFire();
        Char_B4::DrownPed_5459E0();
        return;
    }

    Fix16 ret1;
    Fix16 ret2;

    if (block_type == 0 && (field_58_flags & 1) == 0) // or !pBlock
    {
        if (field_7C_pPed->field_15C_player && field_7C_pPed->get_fieldC_45C9B0() < k_dword_6FD9E4 || (field_58_flags & 8) != 0)
        {
            Ang16::sub_41FC20(field_40_rotation + word_6FD936, dword_6FDAC8, ret1, ret2);
        }
        else
        {
            Ang16::sub_41FC20(field_40_rotation, dword_6FDAC8, ret1, ret2);
        }
        // ......

        ret1 += dword_6FD800;
        ret2 += dword_6FD7F8;

        //v18 = dword_6FD800 + *v16;
        //v19 = dword_6FD7F8 + v15;
        //if ((dword_6FD7FC & 0x3FFF) == dword_6FD9E4)

        s32 zpos; // v20

        if (dword_6FD7FC.GetFracValue() == k_dword_6FD9E4)
        {
            zpos = dword_6FD7FC.ToInt() - 1; //(dword_6FD7FC >> 14) - 1;
        }
        else
        {
            zpos = dword_6FD7FC.ToInt(); //dword_6FD7FC >> 14;
        }

        // OBS: are ret1.ToInt(), ret2.ToInt() swapped?
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(ret1.ToInt(), ret2.ToInt(), zpos);
        if (block_type == AIR)
        {
            if (field_10_char_state != Char_B4_state::Jumping_15)
            {
                // Ped is falling
            LABEL_44:
                field_6C_animation_state = Char_Anim_state::Normal_Fall_11;
                field_68_animation_frame = 0;
                field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::falling_19);
                field_8_ped_state_1 = ped_state_1::immobilized_8;
                field_C_ped_state_2 = ped_state_2::falling_19;
                if (field_7C_pPed->field_15C_player)
                {
                    // inline here ???
                    if (field_7C_pPed->get_fieldC_45C9B0() < k_dword_6FD9E4 || (field_58_flags & 8) != 0)
                    {
                        field_38_velocity = -field_38_velocity;
                    }
                }
                field_90 = field_38_velocity;
                field_94 = k_dword_6FD9E4;
                if (field_38_velocity == k_dword_6FD7C0)
                {
                    field_16 = 1;
                }
                Char_B4::state_8_5520A0();
                return;
            }
            if (this->field_68_animation_frame == 7 || this->field_7C_pPed->field_21C_bf.b11 != 0)
            {
                this->field_10_char_state = 1;
                field_7C_pPed->field_21C_bf.b11 = 0;
                goto LABEL_44;
            }
        }
    }

    field_44 = block_type;
    Fix16 field_1C_zpos = dword_6FDAC8;
    if (field_7C_pPed->IsField238_45EDE0(2) == true)
    {
        if (this->field_10_char_state)
        {
            this->field_46 = 9999;
        }
        field_40_rotation += field_7C_pPed->get_field8_45C900();

        if (this->field_10_char_state == Char_B4_state::Jumping_15)
        {
            field_7C_pPed->field_21C_bf.b11 = 0;
            if (this->field_6C_animation_state != 5)
            {
                this->field_6C_animation_state = 5;
                this->field_68_animation_frame = 0; // line 4b2
            }
            if (field_7C_pPed->get_fieldC_45C9B0() > k_dword_6FD9E4)
            {
                if (field_38_velocity == k_dword_6FD7C0)
                {
                    this->field_38_velocity = this->field_3C_run_or_jump_speed;
                }
                else
                {
                    if (field_38_velocity < field_3C_run_or_jump_speed)
                    {
                        this->field_38_velocity += dword_6FD99C;
                    }
                    else
                    {
                        if (field_38_velocity > field_3C_run_or_jump_speed)
                        {
                            this->field_38_velocity -= dword_6FD99C;
                        }
                    }
                }
            }
            else
            {
                this->field_38_velocity = gRunOrJumpSpeed_6FD7D0;
            }
        }
        else
        {
            if (field_7C_pPed->get_fieldC_45C9B0() > k_dword_6FD9E4) // line 53c
            {
                if (field_38_velocity == k_dword_6FD7C0)
                {
                    this->field_38_velocity = this->field_3C_run_or_jump_speed;
                    field_58_flags &= 0xF7;
                }
                else
                {
                    if (field_38_velocity < field_3C_run_or_jump_speed)
                    {
                        this->field_38_velocity += dword_6FD99C;
                        field_58_flags &= 0xF7;
                    }
                    else
                    {
                        if (field_38_velocity > field_3C_run_or_jump_speed)
                        {
                            this->field_38_velocity -= dword_6FD99C;
                        }
                        field_58_flags &= 0xF7;
                    }
                }
            }
            else
            {
                if (field_7C_pPed->get_fieldC_45C9B0() < k_dword_6FD9E4)
                {
                    v95 = this->field_40_rotation;
                    this->field_58_flags = this->field_58_flags | 8;
                    field_40_rotation += word_6FD936;

                    if (field_38_velocity == k_dword_6FD7C0)
                    {
                        v87 = 0;
                        this->field_38_velocity = this->field_3C_run_or_jump_speed;
                    }
                    else
                    {
                        if (field_38_velocity < field_3C_run_or_jump_speed)
                        {
                            v87 = 0;
                            this->field_38_velocity += dword_6FD99C;
                        }
                        else
                        {
                            if (field_38_velocity > field_3C_run_or_jump_speed)
                            {
                                this->field_38_velocity -= dword_6FD99C;
                            }
                            v87 = 0;
                        }
                    }
                }
                else
                {
                    if (this->field_6A)
                    {
                        byte_6FDB51 = 0;
                        byte_6FDB52 = 0;
                        v95 = field_40_rotation;
                        this->field_40_rotation = this->field_74;

                        v87 = 0;
                        this->field_38_velocity = gCollisionRepulsionSpeed_6FD7BC;
                    }
                    else
                    {
                        this->field_38_velocity = k_dword_6FD7C0;
                        if (this->field_10_char_state)
                        {
                            this->field_10_char_state = 7;
                        }
                    }
                }
            }
        }

        if (this->field_7C_pPed->field_21C_bf.b8 != 0) // line 69a
        {
            this->field_38_velocity = gCollisionRepulsionSpeed_6FD7BC;
        }
    }
    else
    {
        if (this->field_69 || this->field_10_char_state == Char_B4_state::Jumping_15) // line 6b1
        {
            v89 = 0;
            field_7C_pPed->field_21C_bf.b11 = 0; // TODO: Check it
            byte_6FDB51 = 0;
            byte_6FDB52 = 0;
        }
        else
        {
            if (this->field_6A)
            {
                byte_6FDB51 = 0;
                byte_6FDB52 = 0;
                v95 = field_40_rotation;
                this->field_40_rotation = this->field_74;
                v89 = 0;
                this->field_38_velocity = gCollisionRepulsionSpeed_6FD7BC;
                v87 = 0;
            }
            if (this->field_38_velocity < k_dword_6FD7C0)
            {
                // line 367 on 9.6f IDA
                field_40_rotation = field_40_rotation + word_6FD936;
                field_58_flags = field_58_flags | 0x8;
                field_38_velocity = Fix16::Abs(field_38_velocity);
                v87 = 0;
            }
        }
        if (field_10_char_state)
        {
            if (field_10_char_state != 15 && this->field_8_ped_state_1 != 9)
            {
                switch (block_type)
                {
                    case ROAD: // 1
                    case FIELD: // 3
                        if (field_7C_pPed->get_objective_403A80() == objectives_enum::no_obj_0 ||
                            field_7C_pPed->get_objective_403A80() == objectives_enum::objective_8)
                        {
                            field_7C_pPed->sub_463830(17, 9999);
                        }
                        break;
                    default:
                        if (this->field_C_ped_state_2 == 0 && v89)
                        {
                            Char_B4::sub_54C580();
                            Char_B4::sub_54C900();
                        }
                        break;
                }
            }
        }
    }

    if (v87)
    {
        v95 = field_40_rotation;
    }
    if (field_38_velocity == k_dword_6FD9E4)
    {
        field_58_flags &= 0xF7;
    }

    // line 401 on 9.6f IDA
    Ang16::sub_41FC20(field_40_rotation, dword_6FDAC8, ret1, ret2);

    ret1 += dword_6FD7F8 + this->field_4C;
    ret2 += dword_6FD800 + this->field_50;

    if (Char_B4::sub_5532C0())
    {
        goto LABEL_148;
    }
    if (field_7C_pPed->IsField238_45EDE0(2))
    {
        v88 = 1;
        word_6FD808 = field_40_rotation;
        u8 v90 = Char_B4::sub_54EF60(ret1.ToInt(), ret2.ToInt());
        if (gMap_0x370_6F6268->sub_4E0110() != 1 && !v90)
        {
            Char_B4::sub_54CC40();
            v88 = 0;
            goto LABEL_152;
        }
        goto LABEL_148;
    }
    v91 = Char_B4::sub_54C500(ret1.ToInt(), ret2.ToInt()); //(v54 >> 14, v55 >> 14);

    if (byte_6FDB51)
    {
        Char_B4::sub_54C3E0();
    }

    if (v91 == 1)
    {
        v88 = 1;
        if (byte_6FDB52)
        {
            v93 = field_40_rotation;
            v93.SnapToAng4_405640();
            v93 += word_6FD8A2;
            Fix16 xpos_3;
            Fix16 ypos_3;
            Ang16::sub_41FC20(v93, dword_6FD9B4, xpos_3, ypos_3);

            xpos_3 += dword_6FD7F8;
            ypos_3 += dword_6FD800;

            if (Char_B4::sub_54C500(xpos_3.ToInt(), ypos_3.ToInt()) == 1)
            {
                v93 = field_40_rotation;
                v93.SnapToAng4_405640();
                v93 += word_6FD94C;

                Fix16 xpos_4;
                Fix16 ypos_4;

                Ang16::sub_41FC20(v93, dword_6FD9B4, xpos_4, ypos_4);

                xpos_4 += dword_6FD7F8;
                ypos_4 += dword_6FD800;

                if (!Char_B4::sub_54C500(xpos_4.ToInt(), ypos_4.ToInt()))
                {
                    sub_4923D0();
                }
            }
            else
            {
                sub_4923A0();
            }
        }
    LABEL_148:
        field_80_sprite_ptr = this->field_80_sprite_ptr;

        Ang16::sub_41FC20(field_40_rotation, field_38_velocity, ret1, ret2);
        ret1 += field_4C;
        ret2 += field_50;
        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + ret1,
                                                 field_80_sprite_ptr->field_14_xy.y + ret2,
                                                 field_80_sprite_ptr->field_1C_zpos);
    }
    else
    {
        Char_B4::sub_54C090();
    }
LABEL_152:
    field_4C = k_dword_6FD9E4;
    field_50 = k_dword_6FD9E4;
    if (v88 == 1 || (field_58_flags & 1) == 1)
    {
        gmp_block_info* pBlock2 = gMap_0x370_6F6268->get_block_4DFE10(field_80_sprite_ptr->field_14_xy.x.ToInt(), // GetXPos
                                                                      field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                      field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);
        v74 = 0;
        if (pBlock2)
        {
            //field_B_slope_type = v72->field_B_slope_type;
            //if ((field_B_slope_type & 0xFC) != 0 && (field_B_slope_type & 0xFCu) < 0xB4 && (field_B_slope_type & 3) != 0)
            if (is_gradient_slope(pBlock2->field_B_slope_type) && !is_air_type(pBlock2->field_B_slope_type))
            {
                v74 = 1;
            }
        }
        byte_6FDB54 = v74;
        Char_B4::sub_548590();
    }

    Char_B4::sub_548670(byte_6FDB56);

    if (!field_7C_pPed->IsField238_45EDE0(2))
    {
        if (field_69)
        {
            if (Char_B4::sub_54B8F0() == 1)
            {
                if (dword_6FD7F8.ToInt() != field_80_sprite_ptr->field_14_xy.x.ToInt() ||
                    dword_6FD800.ToInt() != field_80_sprite_ptr->field_14_xy.y.ToInt())
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(dword_6FD7F8, dword_6FD800, dword_6FD7FC);

                    field_45 = dword_6FD7B0.ToInt(); //dword_6FD7B0 >> 14;
                    //////if (Char_B4::sub_550090(v85, v86))
                    if (Char_B4::sub_550090(field_80_sprite_ptr->field_14_xy.x.ToInt(), field_80_sprite_ptr->field_14_xy.y.ToInt()))
                    {
                        // Line 507 on 9.6f IDA
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x,
                                                                 field_80_sprite_ptr->field_14_xy.y,
                                                                 dword_6FD7FC);
                        byte_6FDB54 = gMap_0x370_6F6268->sub_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                    field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                    (field_80_sprite_ptr->field_1C_zpos - k_dword_6FD9E8).ToInt());
                        Char_B4::sub_548590();
                    }
                    else
                    {
                        field_69 = 0;
                        field_5C = 10;
                    }
                }
            }
        }
    }

    if (!v87)
    {
        field_40_rotation = v95; //*p_field_40_rotation = v95;
    }

    Char_B4::sub_54DD70();
}

STUB_FUNC(0x54ecb0)
char_type Char_B4::sub_54ECB0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x54ef60)
char_type Char_B4::sub_54EF60(char_type a2, char_type a3)
{
    NOT_IMPLEMENTED;
    return true;
}

STUB_FUNC(0x54fec0)
char_type Char_B4::sub_54FEC0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x550090)
char_type Char_B4::sub_550090(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5504f0)
void Char_B4::state_1_5504F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x550f60)
s16* Char_B4::sub_550F60(s16* a2, s16 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x551350)
char_type Char_B4::sub_551350(s16 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x551400)
char_type Char_B4::sub_551400()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5516f0)
char_type Char_B4::sub_5516F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5519F0)
void Char_B4::state_1_5519F0()
{
    Char_B4::state_1_5504F0();
}

MATCH_FUNC(0x551A00)
void Char_B4::state_3_551A00()
{
    if (field_C_ped_state_2 == ped_state_2::ped2_following_a_car_4)
    {
        field_58_flags_bf.b7 = false;
        Char_B4::state_1_5504F0();
        if (!field_7C_pPed->GetBit11_433CA0() && field_10_char_state != 15)
        {
            if (field_38_velocity > gCollisionRepulsionSpeed_6FD7BC)
            {
                field_6C_animation_state = 1;
            }
            else
            {
                if (field_38_velocity != k_dword_6FD7C0)
                {
                    field_6C_animation_state = 0;
                }
                else
                {
                    field_6C_animation_state = 2;
                    if (field_84)
                    {
                        field_40_rotation = field_84->field_50_car_sprite->field_0;
                    }
                }
            }
        }
    }
    else if (field_C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
    {
        field_10_char_state = Char_B4_state::Interacting_Car_Door_36;
        Sprite* nearestSprt = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(field_80_sprite_ptr, 0);
        if (!nearestSprt || nearestSprt->field_30_sprite_type_enum != sprite_types_enum::car ||
            (nearestSprt->field_8_car_bc_ptr == field_7C_pPed->get_target_to_enter_403B10()) ||
            nearestSprt->field_8_car_bc_ptr->is_on_trailer_421720() || field_7C_pPed->sub_45BD20(nearestSprt->field_8_car_bc_ptr))
        {
            if (field_6C_animation_state != Char_Anim_state::Entering_Car_6)
            {
                // TODO: remove Ang16 operator=(const Ang16& other) without breaking Player::DoPedControlInputs_566C80
                field_40_rotation.rValue = field_84->field_50_car_sprite->field_0.rValue;
                field_6C_animation_state = Char_Anim_state::Entering_Car_6;
                field_68_animation_frame = 0;
                if (field_84->sub_43B540(field_7C_pPed->get_target_car_door_403A60()))
                {
                    field_58_flags_bf.b4 = true;
                }
                else
                {
                    field_58_flags_bf.b4 = false;
                }
                field_70_frame_timer = 0;
            }
        }
    }
}

MATCH_FUNC(0x551B30)
void Char_B4::state_4_551B30()
{
    if (field_6C_animation_state != Char_Anim_state::Exiting_Car_7)
    {
        field_6C_animation_state = Char_Anim_state::Exiting_Car_7;
        field_68_animation_frame = 0;
        s8 target_door = field_7C_pPed->field_24C_target_car_door;
        if (field_84->sub_43B540(target_door))
        {
            field_58_flags_bf.b4 = true;
        }
        else
        {
            field_58_flags_bf.b4 = false;
        }
        field_70_frame_timer = 0;
    }
    if (field_10_char_state == Char_B4_state::Jumping_15)
    {
        field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::walking_0);
        field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
    }
    if ((u8)Char_B4::IsOnWater_545570())
    {
        field_7C_pPed->PutOutFire();
        Char_B4::DrownPed_5459E0();
    }
}

STUB_FUNC(0x551BB0)
void Char_B4::state_5_551BB0()
{
    NOT_IMPLEMENTED;
}

// https://decomp.me/scratch/qNjdM
WIP_FUNC(0x551CB0)
void Char_B4::state_7_551CB0()
{
    WIP_IMPLEMENTED;
    field_38_velocity = k_dword_6FD7C0;
    if (field_10_char_state != Char_B4_state::Jumping_15)
    {
        Char_B4::sub_5459C0();
    }
    if (field_7C_pPed->IsField238_45EDE0(2) == true)
    {
        field_40_rotation += field_7C_pPed->get_field8_45C900();

        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            Char_B4::state_0_54DDF0();
            SetPedState1_433910(0);
            SetPedState2_433A50(0);
            return;
        }
        s32 unk_zpos;
        if (field_58_flags_bf.b0 || dword_6FD7FC.GetFracValue() != k_dword_6FD9E4)
        {

            gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), dword_6FD7FC.ToInt());
            unk_zpos = dword_6FD7FC.ToInt();
        }
        else
        {
            gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), dword_6FD7FC.ToInt() - 1);
            unk_zpos = dword_6FD7FC.ToInt() - 1;
        }
        gmp_block_info* block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), unk_zpos);
        if (block_4DFE10)
        {
            if (gGtx_0x106C_703DD4->IsElectrifiedFloorType_491F80(block_4DFE10->field_8_lid & 0x3FF))
            {
                if (field_7C_pPed->field_21C_bf.b27 == false)
                {
                    field_7C_pPed->field_210_shock_counter += 3;
                    if (field_7C_pPed->field_204_killer_id != 0)
                    {
                        if (gPedManager_6787BC->PedById(field_7C_pPed->field_204_killer_id))
                        {
                            // forget the last ped who harmed this ped after some time?
                            // so in this case the death reason must be shocking if it happens
                            field_7C_pPed->field_290 = 2;
                            field_7C_pPed->field_264 = 50;
                        }
                    }
                }
            }
        }
        if (field_6C_animation_state != 4)
        {
            field_6C_animation_state = 2;
        }
    }
    else
    {
        field_40_rotation = field_7C_pPed->field_130;
    }
    if (field_10_char_state == Char_B4_state::Jumping_15)
    {
        field_38_velocity = gRunOrJumpSpeed_6FD7D0; // line 1be
    }
    if (field_6A > 0 || field_10_char_state == Char_B4_state::Jumping_15 || field_4C != k_dword_6FD9E4 || field_50 != k_dword_6FD9E4)
    {
        field_8_ped_state_1 = 0;
        field_C_ped_state_2 = 0;
        Char_B4::state_0_54DDF0();
        field_8_ped_state_1 = 7;
        field_C_ped_state_2 = 14;
        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            return;
        }
    }
    else
    {
        s32 unk2_zpos;
        if (field_58_flags_bf.b0 == false &&
            (dword_6FD7FC == k_dword_6FD9E8 || dword_6FD7FC == k_dword_6FD9EC || dword_6FD7FC == k_dword_6FD9F0 ||
             dword_6FD7FC == dword_6FD9F4 || dword_6FD7FC == k_dword_6FD9F8 || dword_6FD7FC == k_dword_6FD9FC ||
             dword_6FD7FC == k_dword_6FDA00))
        {
            unk2_zpos = dword_6FD7FC.ToInt() - 1;
        }
        else
        {
            unk2_zpos = dword_6FD7FC.ToInt();
        }

        u8 block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(dword_6FD7F8.ToInt(), dword_6FD800.ToInt(), unk2_zpos);
        if ((u8)Char_B4::IsOnWater_545570())
        {
            field_7C_pPed->PutOutFire();
            Char_B4::DrownPed_5459E0();
            return;
        }
        if (block_type == AIR && field_58_flags_bf.b0 == false && field_10_char_state != Char_B4_state::Jumping_15)
        {
            field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
            field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::falling_19);
            field_16 = 1;
            return;
        }
    }

    switch (field_C_ped_state_2)
    {
        case 8:
        case 9:
            field_6C_animation_state = 9;
            break;
        case ped_state_2::ped2_staying_14:
            if (field_7C_pPed->GetBit11_433CA0() == true) // line 344
            {
                if (field_7C_pPed->field_21C_bf.b9)
                {
                    if (field_6C_animation_state != 4)
                    {
                        field_6C_animation_state = 4;
                        field_68_animation_frame = 0;
                    }
                }
                else
                {
                    if (field_6C_animation_state == 4)
                    {
                        if (field_68_animation_frame == 0)
                        {
                            field_6C_animation_state = 2;
                        }
                    }
                    else
                    {
                        field_6C_animation_state = 2;
                    }
                }
            }
            else
            {
                if (field_6C_animation_state == 4)
                {
                    if (field_68_animation_frame == 0)
                    {
                        field_6C_animation_state = 2;
                    }
                }
                else
                {
                    if ((field_7C_pPed->IsField238_45EDE0(5) || field_7C_pPed->IsField238_45EDE0(2)) && !field_4A)
                    {
                        s16 v15 = 600;
                        if (stru_6F6784.get_int_4F7AE0(&v15) < 4u && field_10_char_state != Char_B4_state::Smoking_35)
                        {
                            field_10_char_state = Char_B4_state::Smoking_35;
                            field_68_animation_frame = 0;
                            field_70_frame_timer = 0;
                        }
                    }
                    field_6C_animation_state = 2;
                }
            }
            break;
        default:
            field_6C_animation_state = 2;
            break;
    }
}

STUB_FUNC(0x5520A0)
void Char_B4::state_8_5520A0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x552E90)
void Char_B4::state_9_552E90()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5532C0)
bool Char_B4::sub_5532C0()
{
    NOT_IMPLEMENTED;
    return false;
}

STUB_FUNC(0x553330)
char_type Char_B4::IsThreatToSearchingPed_553330()
{
    NOT_IMPLEMENTED;
    return field_7C_pPed->IsThreatToSearchingPed_4661F0();
}

STUB_FUNC(0x553340)
bool Char_B4::sub_553340(Sprite* pSprite)
{
    NOT_IMPLEMENTED;
    return false;
}

MATCH_FUNC(0x5535B0)
char_type Char_B4::sub_5535B0(Object_2C* p2c)
{
    Ped* pPed = field_7C_pPed;
    if (pPed->field_15C_player)
    {
        return gfrosty_pasteur_6F8060->sub_5129F0(pPed->field_200_id, p2c->field_14_id);
    }
    else
    {
        return 0;
    }
}

STUB_FUNC(0x553640)
bool Char_B4::OnObjectTouched_553640(Object_2C* p2c)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/UYcej
STUB_FUNC(0x5537F0)
char_type Char_B4::HandlePedObjectHit_5537F0(Object_2C* p2c)
{
    NOT_IMPLEMENTED;

    const s32 l_18 = p2c->field_18_model;
    if (l_18 == 128 || l_18 == 138 || l_18 == 10 && !byte_6FDB59)
    {
        gObject_5C_6F8F84->CreateExplosion_52A3D0(field_80_sprite_ptr->GetXPos(),
                                                  field_80_sprite_ptr->GetYPos(),
                                                  field_80_sprite_ptr->GetZPos(),
                                                  word_6FDB34,
                                                  18,
                                                  gVarrok_7F8_703398->field_0[p2c->field_26_varrok_idx].field_0_ped_id);
        if (p2c->field_18_model == 10)
        {
            byte_6FDB59 = 1;
        }
    }

    Ped* pPed = field_7C_pPed;
    if (p2c->field_26_varrok_idx == pPed->field_267_varrok_idx)
    {
        return 0;
    }
    else
    {
        return pPed->HandlePedHitByObject_45D000(p2c);
    }
}

STUB_FUNC(0x5538A0)
void Char_B4::sub_5538A0(Car_BC* pCar, s32 a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x553E00)
void Char_B4::sub_553E00(Ang16 ang, Fix16 a3, Fix16 a4, char_type a5)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x553F90)
void __stdcall sub_553F90()
{
    byte_6FDB48 = 0;
    byte_6FDB49 = 0;
    gB4_id_6FDB4C = 0;
}

EXPORT void Char_B4::nullsub_28()
{
    NOT_IMPLEMENTED;
}

//STUB_FUNC(0x5519f0)
//void j_Char_B4::state_1_5504F0()
//{
//    NOT_IMPLEMENTED;
//}
