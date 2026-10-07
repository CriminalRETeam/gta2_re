#define FIX16_POINT_ZERO kFP16Zero_6FD9E4
#include "char.hpp"
#include "CarAI_78.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Door_4D4.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Garage_48.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "Particle_8.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "Varrok_7F8.hpp"
#include "Weapon_30.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "winmain.hpp"

// Ped.cpp
EXTERN_GLOBAL(Fix16, kFpPoint02_6FD9AC);
EXTERN_GLOBAL(Fix16, kFpPoint3_6FD830);

DEFINE_GLOBAL(s8, gCharB4_UpdateCounter_6FDB48, 0x6FDB48);
DEFINE_GLOBAL(s8, byte_6FDB49, 0x6FDB49);
DEFINE_GLOBAL(u32, gB4_id_6FDB4C, 0x6FDB4C);

DEFINE_GLOBAL_INIT(s32, gCharB4_PathDirection_623F44, path_direction::up_1, 0x623F44);
DEFINE_GLOBAL(Fix16, gCharB4_DistanceToTarget_6FD80C, 0x6FD80C);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD936, Ang16(720), 0x6FD936);
DEFINE_GLOBAL_INIT(Ang16, kAng315_6FD938, Ang16(1260), 0x6FD938);

DEFINE_GLOBAL(u8, byte_6FDB55, 0x6FDB55);
DEFINE_GLOBAL(Ang16, word_6FDB2E, 0x6FDB2E);

DEFINE_GLOBAL(u8, byte_6FDB58, 0x6FDB58);
DEFINE_GLOBAL(u8, gCharB4_HitByMine_6FDB59, 0x6FDB59);

DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_6FD9E4, Fix16(0), 0x6FD9E4);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Saved_Zpos_6FD7FC, kFP16Zero_6FD9E4, 0x6FD7FC);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Saved_Ypos_6FD800, kFP16Zero_6FD9E4, 0x6FD800);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Saved_Xpos_6FD7F8, kFP16Zero_6FD9E4, 0x6FD7F8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Four_6FD9F4, Fix16(4), 0x6FD9F4);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FD7A4, Fix16(0x1000, 0), 0x6FD7A4);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_Saved_SlopeGradDir_6FD7B0, kFP16Zero_6FD9E4, 0x6FD7B0);
DEFINE_GLOBAL_INIT(Fix16, kZeroVelocity_6FD7C0, kFP16Zero_6FD9E4, 0x6FD7C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7DC, kFP16Zero_6FD9E4, 0x6FD7DC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD868, Fix16(256, 0), 0x6FD868);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FD9B4, Fix16(0x2000, 0), 0x6FD9B4);
DEFINE_GLOBAL_INIT(Fix16, gRunOrJumpSpeed_6FD7D0, kFP16Four_6FD9F4* k_dword_6FD868, 0x6FD7D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8B4, kFP16Zero_6FD9E4, 0x6FD8B4);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_StepXpos_6FD8B8, kFP16Zero_6FD9E4, 0x6FD8B8);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_StepYpos_6FD8BC, kFP16Zero_6FD9E4, 0x6FD8BC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD82C, Fix16(0xCCC, 0), 0x6FD82C);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_WorldCollisionOffset_6FD8D8, dword_6FD82C, 0x6FD8D8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD870, k_dword_6FD868 * 2, 0x6FD870);
DEFINE_GLOBAL_INIT(Fix16, k_CollisionRepulsionSpeed_6FD7BC, dword_6FD870, 0x6FD7BC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9B0, Fix16(0x333, 0), 0x6FD9B0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDAE4, dword_6FD9B0, 0x6FDAE4);
DEFINE_GLOBAL_INIT(Fix16, gFix16_Two_6FD9EC, Fix16(2), 0x6FD9EC);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FD9E8, Fix16(1), 0x6FD9E8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9A8, Fix16(0x1EB, 0), 0x6FD9A8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Three_6FD9F0, Fix16(3), 0x6FD9F0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Five_6FD9F8, Fix16(5), 0x6FD9F8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Six_6FD9FC, Fix16(6), 0x6FD9FC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Seven_6FDA00, Fix16(7), 0x6FDA00);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDAC8, k_dword_6FD868 * 6, 0x6FDAC8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD99C, k_dword_6FD868 / kFP16Four_6FD9F4, 0x6FD99C);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD7B8, k_dword_6FD868, 0x6FD7B8);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD7CC, kFP16Three_6FD9F0* k_dword_6FD868, 0x6FD7CC);

DEFINE_GLOBAL_INIT(Fix16, dword_6FDB20, k_dword_6FD868 * 64, 0x6FDB20);

DEFINE_GLOBAL_INIT(Ang16, k_dword_6FD892, Ang16(48), 0x6FD892);

DEFINE_GLOBAL_INIT(Fix16, k_dword_6FD8DC, Fix16(0x666, 0), 0x6FD8DC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FD8E4, Fix16(0x2000, 0), 0x6FD8E4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDB28, kFP16Half_6FD8E4, 0x6FDB28);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD87C, k_dword_6FD868 * 4, 0x6FD87C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD88C, k_dword_6FD868 * 8, 0x6FD88C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FD828, Fix16(0x1000, 0), 0x6FD828);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eighth_6FDB04, Fix16(0x800, 0), 0x6FDB04);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9A0, Fix16(0xA3, 0), 0x6FD9A0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDB24, Fix16(0x200, 0), 0x6FDB24);

DEFINE_GLOBAL_INIT(Fix16, k_dword_6FDA9C, Fix16(0x100, 0), 0x6FDA9C);

DEFINE_GLOBAL(u16, gNumPedsOnScreen_6787EC, 0x6787EC);

DEFINE_GLOBAL(u8, byte_6FDB51, 0x6FDB51);
DEFINE_GLOBAL(u8, byte_6FDB52, 0x6FDB52);
DEFINE_GLOBAL(u8, byte_6FDB53, 0x6FDB53);
DEFINE_GLOBAL(u8, byte_6FDB54, 0x6FDB54);

DEFINE_GLOBAL(u8, byte_6FDB56, 0x6FDB56);
DEFINE_GLOBAL_INIT(u8, byte_623F48, 1, 0x623F48);
DEFINE_GLOBAL(u8, gCharB4_Saved_TileX_6FDAD8, 0x6FDAD8);
DEFINE_GLOBAL(u8, gCharB4_Saved_TileY_6FDAD9, 0x6FDAD9);
DEFINE_GLOBAL(u8, byte_6FDB57, 0x6FDB57);

DEFINE_GLOBAL_INIT(Ang16, gAng16_AngleOfCollision_6FD808, Ang16(0), 0x6FD808);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD8A2, Ang16(360), 0x6FD8A2);
DEFINE_GLOBAL_INIT(Ang16, word_6FD940, Ang16(64), 0x6FD940);
DEFINE_GLOBAL_INIT(Ang16, word_6FD8F8, Ang16(1376), 0x6FD8F8);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD94C, Ang16(1080), 0x6FD94C);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD95C, Ang16(1080), 0x6FD95C);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD854, Ang16(360), 0x6FD854);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD8E8, Ang16(0x2D0), 0x6FD8E8);
DEFINE_GLOBAL(Ang16, word_6FDB3C, 0x6FDB3C);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FDA64, Ang16(0x168), 0x6FDA64);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD904, Ang16(0x438), 0x6FD904);

DEFINE_GLOBAL_INIT(Ang16, word_6FDA54, Ang16(0x18), 0x6FDA54); // TODO: Init via func 0x54A300
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD920, kAng180_6FD936, 0x6FD920);
DEFINE_GLOBAL_INIT(Ang16, dword_6FD9D8, Ang16(0x588), 0x6FD9D8); // TODO: Init via func 0x54A270
DEFINE_GLOBAL_INIT(Ang16, word_6FD890, Ang16(96), 0x6FD890);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6FD89C, Ang16(180), 0x6FD89C);

DEFINE_GLOBAL_INIT(Fix16, dword_6FDAB0, kFP16Half_6FD8E4, 0x6FDAB0);

EXTERN_GLOBAL(Ang16, kAng0_6FDB34);
EXTERN_GLOBAL(Ped_List_4, gThreateningPedsList_678468);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F67B0, Fix16(0x2000, 0), 0x6F67B0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDB18, k_dword_6FD868 * 32, 0x6FDB18);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDB08, k_dword_6FD868 * 12, 0x6FDB08);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD91C, Fix16(0x1333, 0), 0x6FD91C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eight_6FDA08, Fix16(8), 0x6FDA08);
DEFINE_GLOBAL_INIT(Fix16, gFP16_CollisionCheckRadius_6FDACC, Fix16(0x800, 0), 0x6FDACC);
DEFINE_GLOBAL_INIT(Fix16, kFP16MinusOne_6FD790, Fix16(-1), 0x6FD790);
DEFINE_GLOBAL_INIT(Fix16, dword_6FDA04, k_dword_6FD868 * 256, 0x6FDA04);


MATCH_FUNC(0x4056C0)
EXPORT Ang16 __stdcall ComputeShortestAngleDelta_4056C0(Ang16& a2, Ang16& a3)
{
    Ang16 delta = a2 - a3;
    if (delta > kAng180_669156)
    {
        delta = -delta;
    }
    return delta;
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

MATCH_FUNC(0x4F79B0)
EXPORT Fix16 __stdcall SnapZTo16_4F79B0(Fix16 a2)
{
    return ((kFP16Half_6F67B0 + (a2 * 1000)).GetRoundValue()) / 1000;
}

MATCH_FUNC(0x529050)
EXPORT void __stdcall UnpackSignedNibbles_529050(u8 a1, s8* a2, s8* a3)
{
    *a2 = (a1 >> 4) - 7;
    *a3 = (a1 & 0xF) - 7;
}

//https://decomp.me/scratch/iQH9l
MATCH_FUNC(0x544F70)
void __stdcall ResetCharUpdateGlobals_544F70()
{
    gCharB4_Saved_Xpos_6FD7F8 = kFP16Zero_6FD9E4;
    gCharB4_Saved_Ypos_6FD800 = kFP16Zero_6FD9E4;
    gCharB4_Saved_Zpos_6FD7FC = kFP16Zero_6FD9E4;
    dword_6FD7DC = kFP16Zero_6FD9E4;
    gCharB4_Saved_SlopeGradDir_6FD7B0 = kFP16Zero_6FD9E4;
    gCharB4_StepXpos_6FD8B8 = kFP16Zero_6FD9E4;
    gCharB4_StepYpos_6FD8BC = kFP16Zero_6FD9E4;
    dword_6FD8B4 = kFP16Zero_6FD9E4;
    byte_6FDB51 = 0;
    byte_6FDB52 = 0;
    byte_6FDB53 = 0;
    byte_6FDB54 = 0;
    byte_6FDB55 = 0;
    gCharB4_PathDirection_623F44 = path_direction::up_1;
    byte_6FDB56 = 0;
    byte_623F48 = 1;
    gCharB4_Saved_TileX_6FDAD8 = kFP16Zero_6FD9E4.ToUInt8();
    gCharB4_Saved_TileY_6FDAD9 = kFP16Zero_6FD9E4.ToUInt8();
    byte_6FDB57 = 0;
    byte_6FDB58 = 0;
}

// https://decomp.me/scratch/ZsDjc
MATCH_FUNC(0x544ff0)
Char_B4::Char_B4()
{
    field_0_id = 0;
    field_4 = 0;
    field_5_remap = -1;
    field_6 = 0;
    field_8_ped_state_1 = 11;
    field_C_ped_state_2 = 28;
    field_10_char_state = 36;
    field_14_target_rotation = kAng0_6FDB34;
    field_16_state_init_pending = 0;
    field_18_collided_entity = 0;
    field_1C_prev_collided_entity = 0;
    field_20 = 0;
    field_24 = 3;
    field_28 = kAng0_6FDB34;
    field_2A = kAng0_6FDB34;
    field_2C_ang = kAng0_6FDB34;
    field_30 = 4;
    field_34 = 0;
    field_38_velocity = kZeroVelocity_6FD7C0;
    field_3C_run_or_jump_speed = gRunOrJumpSpeed_6FD7D0;
    field_40_rotation = kAng0_6FDB34;
    field_42_rotation_jitter = kAng0_6FDB34;
    field_44_block_type = 0;
    field_45_slope_gradient_direction = 0;
    field_5C = 0;
    field_46_timer = 0; // maybe field_46_shock_counter
    field_48_lying_on_floor_timer = 0;
    mpNext = 0;
    field_7C_pPed = 0;
    field_80_sprite_ptr = 0;
    field_68_animation_frame = 0;
    field_69_is_colliding_with_sprite = 0;
    field_74 = kAng0_6FDB34;
    field_6A = 0;
    field_84_target_car = 0;
    field_58_flags_bf.b0 = 0;
    field_88_obj_2c.DestroyAllSprites_5A7010();
    field_8C_jump_base_z = kFP16Zero_6FD9E4;
    field_58_flags_bf.b2 = 0;
    field_6C_animation_state = 18;
    field_70_frame_timer = 0;
    field_71_frame_delay = 0;
    field_90_fall_speed = kFP16Zero_6FD9E4;
    field_94_fall_z_speed = kFP16Zero_6FD9E4;
    field_58_flags_bf.b1 = 0;
    field_98_velocity_vector.x = kFP16Zero_6FD9E4;
    field_98_velocity_vector.y = kFP16Zero_6FD9E4;
    field_58_flags_bf.b3 = 0;
    field_58_flags_bf.b5 = 0;
    field_A4_xpos = kFP16Zero_6FD9E4;
    field_A8_ypos = kFP16Zero_6FD9E4;
    field_AC_zpos = kFP16Zero_6FD9E4;
    field_4A = 0;
}

MATCH_FUNC(0x5451A0)
Char_B4::~Char_B4()
{
    field_18_collided_entity = 0;
    field_1C_prev_collided_entity = 0;
    mpNext = 0;
    field_7C_pPed = 0;
    field_80_sprite_ptr = 0;
    field_84_target_car = 0;
}

MATCH_FUNC(0x5451C0)
bool Char_B4::HasShadows_5451C0()
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

MATCH_FUNC(0x545200)
void Char_B4::PoolAllocate()
{
    field_0_id = gB4_id_6FDB4C++;
    Char_B4::InitSprite_5456A0();
    field_4 = 1;
    field_5_remap = -1;
    field_6 = 0;
    field_8_ped_state_1 = 11;
    field_C_ped_state_2 = 28;
    field_10_char_state = 36;
    field_14_target_rotation = kAng0_6FDB34;
    field_16_state_init_pending = 0;
    field_18_collided_entity = 0;
    field_1C_prev_collided_entity = 0;
    field_20 = 0;
    field_24 = 3;
    field_28 = kAng0_6FDB34;
    field_2A = kAng0_6FDB34;
    field_2C_ang = kAng0_6FDB34;
    field_30 = 4;
    field_34 = 0;
    field_38_velocity = kZeroVelocity_6FD7C0;
    field_3C_run_or_jump_speed = gRunOrJumpSpeed_6FD7D0;
    field_40_rotation = kAng0_6FDB34;
    field_42_rotation_jitter = kAng0_6FDB34;
    field_44_block_type = 0;
    field_45_slope_gradient_direction = 0;
    field_5C = 0;
    field_46_timer = 0;
    field_48_lying_on_floor_timer = 0;
    field_4A = 500;
    mpNext = 0;
    field_7C_pPed = 0;
    field_68_animation_frame = 0;
    field_69_is_colliding_with_sprite = 0;
    field_74 = kAng0_6FDB34;
    field_6A = 0;
    field_84_target_car = 0;
    field_58_flags_bf.b0 = 0;
    field_88_obj_2c.DestroyAllSprites_5A7010();
    field_8C_jump_base_z = kFP16Zero_6FD9E4;
    field_58_flags_bf.b2 = 0;
    field_6C_animation_state = 18;
    field_70_frame_timer = 0;
    field_71_frame_delay = 0;
    field_90_fall_speed = kFP16Zero_6FD9E4;
    field_94_fall_z_speed = kFP16Zero_6FD9E4;
    field_58_flags_bf.b1 = 0;
    field_98_velocity_vector.x = kFP16Zero_6FD9E4;
    field_98_velocity_vector.y = kFP16Zero_6FD9E4;
    field_58_flags_bf.b5 = 0;
    field_58_flags_bf.b3 = 0;
    field_A4_xpos = kFP16Zero_6FD9E4;
    field_A8_ypos = kFP16Zero_6FD9E4;
    field_AC_zpos = kFP16Zero_6FD9E4;
    field_58_flags_bf.b4 = 0;
    field_4C_conveyor_dx = kFP16Zero_6FD9E4;
    field_50_conveyor_dy = kFP16Zero_6FD9E4;
    field_72_next_tile_x = kFP16Zero_6FD9E4.ToInt();
    field_73_next_tile_y = kFP16Zero_6FD9E4.ToInt();
    field_58_flags_bf.b6 = 0;
    field_58_flags_bf.b7 = 0;
    field_60 = 0;
    field_64 = 0;
    field_55 = 0;
    field_A0 = 0;
    field_B0_scream_timer = -1;
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
    field_88_obj_2c.DestroyAllSprites_5A7010();
    field_B0_scream_timer = -1;
}

MATCH_FUNC(0x545430)
void Char_B4::DrawFlamesAndStartScreamTimer_545430()
{
    // Spawn fire
    Object_2C* p2C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(197, 0, 0, 0, kAng0_6FDB34); // ped_like_fire_197 ?? but its actually fire
    field_80_sprite_ptr->DispatchCollisionEvent_5A3100(p2C->field_4, 0, 0, kAng0_6FDB34);
    field_B0_scream_timer = 10; // Start screaming timer
}

MATCH_FUNC(0x5454B0)
void Char_B4::RemoveFireSprites_5454B0()
{
    field_B0_scream_timer = -1;
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
                field_71_frame_delay = 2;
                field_70_frame_timer = 0;
            }
        }
        else
        {
            field_10_char_state = Char_B4_state::Jumping_15;
            field_6C_animation_state = Char_Anim_state::Jumping_5;
            field_68_animation_frame = 0;
            field_38_velocity = gRunOrJumpSpeed_6FD7D0;
            field_8C_jump_base_z = Fix16(field_80_sprite_ptr->field_1C_zpos.ToUInt8());
        }
    }
}

MATCH_FUNC(0x545530)
void Char_B4::Teleport_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos)
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

// 9.6f 0x493780
MATCH_FUNC(0x545580)
Fix16_Point Char_B4::sub_545580()
{
    Fix16_Point p;
    p.FromPolar_41E210(-gRunOrJumpSpeed_6FD7D0, field_80_sprite_ptr->field_0);
    return p;
}

MATCH_FUNC(0x5455f0)
void Char_B4::KillPed_5455F0()
{
    field_7C_pPed->Kill_46F9D0();
}

MATCH_FUNC(0x545600)
void Char_B4::ClearCollisionState_545600()
{
    field_18_collided_entity = 0;
    field_1C_prev_collided_entity = 0;
    field_20 = 0;
    field_2C_ang = kAng0_6FDB34;
    field_69_is_colliding_with_sprite = 0;
    field_24 = 0;
    field_28 = kAng0_6FDB34;
    field_2A = kAng0_6FDB34;
    field_2C_ang = kAng0_6FDB34;
}

MATCH_FUNC(0x545640)
void Char_B4::GetTileFracX64_545640(Fix16 a1, s16* output)
{
    *output = (a1.GetFracValue() * Fix16(64)).ToInt();
}

MATCH_FUNC(0x545670)
void Char_B4::GetTileFracY64_545670(Fix16 a1, s16* output)
{
    *output = (a1.GetFracValue() * Fix16(64)).ToInt();
}

MATCH_FUNC(0x5456a0)
void Char_B4::InitSprite_5456A0()
{
    // TODO: maybe an inline here: temp var not needed
    Sprite* pFirst = gSprite_Pool_703818->get_new_sprite();
    field_80_sprite_ptr = pFirst;
    pFirst->SetType_4206F0(sprite_types_enum::ped_3);
    field_80_sprite_ptr->AllocInternal_59F950(gCharB4_WorldCollisionOffset_6FD8D8, gCharB4_WorldCollisionOffset_6FD8D8, kFP16Quarter_6FD7A4);
    field_80_sprite_ptr->field_8_char_b4_ptr = this;
    field_80_sprite_ptr->CreateSoundObj_5A29D0();
}

MATCH_FUNC(0x545700)
bool Char_B4::IsOnScreen_545700()
{
    return gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(this->field_80_sprite_ptr, kFP16Zero_6FD9E4) == 1;
}

MATCH_FUNC(0x545720)
void Char_B4::Update_545720(Fix16 a2)
{
    if (++gCharB4_UpdateCounter_6FDB48 > 20)
    {
        gCharB4_UpdateCounter_6FDB48 = 0;
    }
    if (field_4A > 0)
    {
        field_4A--;
    }
    gCharB4_DistanceToTarget_6FD80C = a2;

    if (field_5C > 0)
    {
        field_5C--;
    }
    ResetCharUpdateGlobals_544F70();
    gCharB4_Saved_Xpos_6FD7F8 = field_80_sprite_ptr->field_14_xy.x;
    gCharB4_Saved_Ypos_6FD800 = field_80_sprite_ptr->field_14_xy.y;
    gCharB4_Saved_Zpos_6FD7FC = field_80_sprite_ptr->field_1C_zpos;
    gCharB4_Saved_SlopeGradDir_6FD7B0 = Fix16(field_45_slope_gradient_direction);
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
                field_80_sprite_ptr->set_num_40F7B0(34);
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
            field_98_velocity_vector.SetFromPolar_41E210(-field_38_velocity, field_40_rotation);
        }
        else
        {
            // anti-clockwise?
            field_98_velocity_vector.SetFromPolar_41E210(field_38_velocity, field_40_rotation);
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
void Char_B4::CheckAndHandleCollisions_5459C0()
{
    gCharB4_HitByMine_6FDB59 = 0;
    gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);
}

MATCH_FUNC(0x5459e0)
void Char_B4::DrownPed_5459E0()
{
    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
    field_7C_pPed->ChangeNextPedState2_45C540(20);
    field_16_state_init_pending = 1;

    gParticle_8_6FD5E8->EmitWaterSplash_53F060(field_80_sprite_ptr->field_14_xy.x,
                                               field_80_sprite_ptr->field_14_xy.y,
                                               field_80_sprite_ptr->field_1C_zpos,
                                               kAng180_6FD936 + field_80_sprite_ptr->field_0,
                                               1);
    field_7C_pPed->Set_F250_IfBit_433DD0(28);
    s32 ped_killer_id = field_7C_pPed->field_204_killer_id;
    if (ped_killer_id)
    {
        if (gPedManager_6787BC->PedById(ped_killer_id))
        {
            field_7C_pPed->field_290 = 5;
            field_7C_pPed->field_264_killer_id_timer = 50;
        }
    }
}

// 9.6f 0x497DF0
WIP_FUNC(0x546360)
void Char_B4::UpdateAnimState_546360()
{
    WIP_IMPLEMENTED;

    s16 newId = 0;
    u8 frame_limit = 0;

    field_70_frame_timer++;

    Fix16 groundZ = kFP16Zero_6FD9E4;
    Fix16 newx = kFP16Zero_6FD9E4;
    Fix16 newy = kFP16Zero_6FD9E4;

    // Declared here, before the switch, to get the original stack layout
    u8 weapon_ofs = 0;
    u8 bThrowWeapon = 0;
    s32 baseId;

    // Separate function scope slots for the sub_4E4E50 results, the original doesn't share them
    Fix16 driverZ;
    Fix16 carZ69;
    Fix16 carZ7;

    switch (field_7C_pPed->field_26C_graphic_type)
    {
        case 0:
            baseId = 0;
            break;
        case 1:
            baseId = 158;
            break;
        case 2:
            baseId = 316;
            break;
        default:
            baseId = 158;
            break;
    }

    if (field_7C_pPed->field_244_remap > -1)
    {
        field_80_sprite_ptr->SetRemap((u8)field_7C_pPed->field_244_remap);
    }
    else
    {
        field_80_sprite_ptr->SetPaletteSprites_420700();
    }

    if (field_7C_pPed->field_15C_player && field_C_ped_state_2 != 19)
    {
        if (field_7C_pPed->field_170_selected_weapon &&
            (field_7C_pPed->field_170_selected_weapon->field_1C_idx == weapon_type::molotov ||
             field_7C_pPed->field_170_selected_weapon->field_1C_idx == weapon_type::grenade))
        {
            bThrowWeapon = 1;
        }

        if (field_7C_pPed->field_170_selected_weapon && (field_7C_pPed->field_21C & 0x800) == 0)
        {
            if ((field_7C_pPed->field_170_selected_weapon->field_1C_idx == weapon_type::molotov ||
                 field_7C_pPed->field_170_selected_weapon->field_1C_idx == weapon_type::grenade) &&
                field_7C_pPed->field_15C_player->field_8D_bWasAttackPressed)
            {
                field_6C_animation_state = 4;
                field_68_animation_frame = 0;
            }
        }
        else if (field_7C_pPed->field_170_selected_weapon && (field_7C_pPed->field_21C & 0x800) != 0)
        {
            if (field_7C_pPed->field_170_selected_weapon->field_1C_idx == weapon_type::molotov ||
                field_7C_pPed->field_170_selected_weapon->field_1C_idx == weapon_type::grenade)
            {
                field_7C_pPed->field_21C &= ~0x800;
            }
        }
    }
    else
    {
        if (field_7C_pPed->field_170_selected_weapon && (field_7C_pPed->field_21C & 0x800) != 0 &&
            (field_7C_pPed->field_170_selected_weapon->field_1C_idx == weapon_type::molotov ||
             field_7C_pPed->field_170_selected_weapon->field_1C_idx == weapon_type::grenade))
        {
            field_6C_animation_state = 4;
            bThrowWeapon = 1;
        }
    }

    switch (field_6C_animation_state)
    {
        case 0:
        {
            if ((u8)field_70_frame_timer > 2)
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7)
                {
                    field_68_animation_frame = 0;
                }
                field_70_frame_timer = 0;
            }

            if (field_7C_pPed->field_170_selected_weapon && !bThrowWeapon)
            {
                weapon_ofs = 37;
            }
            if (field_10_char_state == 0)
            {
                if (field_68_animation_frame > 5)
                {
                    field_68_animation_frame = 0;
                }
                newId = field_68_animation_frame + baseId + 143;
            }
            else
            {
                newId = baseId + weapon_ofs + field_68_animation_frame;
            }
            break;
        }

        case 1:
        {
            if ((u8)field_70_frame_timer > 1)
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7)
                {
                    field_68_animation_frame = 0;
                }
                field_70_frame_timer = 0;
            }

            if (field_7C_pPed->field_170_selected_weapon && !bThrowWeapon)
            {
                weapon_ofs = 37;
            }
            if (field_10_char_state == 0)
            {
                if (field_68_animation_frame > 5)
                {
                    field_68_animation_frame = 0;
                }
                newId = field_68_animation_frame + baseId + 135;
            }
            else
            {
                newId = baseId + weapon_ofs + field_68_animation_frame + 8;
            }
            break;
        }

        case 2:
            if ((field_7C_pPed->field_21C & 0x800) == 0x800)
            {
                newId = baseId + 139;
            }
            else
            {
                u8 smoke_ofs;
                if (field_10_char_state != Char_B4_state::Smoking_35)
                {
                    if ((u8)field_70_frame_timer > 8)
                    {
                        field_68_animation_frame++;
                        if (field_68_animation_frame > 3)
                        {
                            field_68_animation_frame = 0;
                        }
                        field_70_frame_timer = 0;
                    }
                    smoke_ofs = 0;
                }
                else
                {
                    u8 limit;
                    if (field_68_animation_frame == 5)
                    {
                        limit = 40;
                        if (field_70_frame_timer == 1)
                        {
                            gParticle_8_6FD5E8->SpawnCigaretteSmokePuff_5406B0(field_80_sprite_ptr, 1);
                        }
                    }
                    else
                    {
                        limit = 3;
                    }

                    smoke_ofs = 4;
                    if ((u8)field_70_frame_timer > limit)
                    {
                        field_68_animation_frame++;
                        if (field_68_animation_frame > 7)
                        {
                            field_68_animation_frame = 0;
                            field_10_char_state = 7;
                        }
                        field_70_frame_timer = 0;
                    }
                }
                newId = field_68_animation_frame + (smoke_ofs + baseId) + 53;
            }
            break;

        case 3:
            newId = baseId + 139;
            break;

        case 4:
            if (field_38_velocity > kZeroVelocity_6FD7C0)
            {
                if ((u8)field_70_frame_timer > 2)
                {
                    field_68_animation_frame++;
                    if (field_68_animation_frame > 7)
                    {
                        field_68_animation_frame = 0;
                    }
                    field_70_frame_timer = 0;
                }
                newId = field_68_animation_frame + baseId + 123;
            }
            else
            {
                if ((u8)field_70_frame_timer > 2)
                {
                    field_68_animation_frame++;
                    if (field_68_animation_frame > 7)
                    {
                        field_68_animation_frame = 0;
                    }
                    field_70_frame_timer = 0;
                }
                newId = field_68_animation_frame + baseId + 115;
            }
            break;

        case Char_Anim_state::Entering_Car_6:
        case 9:
        {
            Car_BC* pCar = field_84_target_car;
            pCar->field_76_last_seen_timer = 0;
            switch (field_68_animation_frame)
            {
                case 0:
                {
                    Car_Door_10* pDoor = field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60());
                    if (field_58_flags & 0x10)
                    {
                        frame_limit = 1;
                        pDoor->Open_439E60();
                        newId = baseId + field_68_animation_frame;
                        CarDoorAlignmentSolver_545AF0(7, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        if (field_70_frame_timer == 1 && pDoor->IsStateActive_421360())
                        {
                            field_68_animation_frame = 4;
                            field_71_frame_delay = 0;
                        }
                        CarDoorAlignmentSolver_545AF0(field_68_animation_frame,
                                                      pCar,
                                                      field_7C_pPed->get_target_car_door_403A60(),
                                                      newx,
                                                      newy,
                                                      field_40_rotation);
                        newId = field_68_animation_frame + baseId + 24;
                        frame_limit = 5;
                    }
                    break;
                }

                case 1:
                case 2:
                case 3:
                    if (field_58_flags & 0x10)
                    {
                        frame_limit = 1;
                        CarDoorAlignmentSolver_545AF0(7 - field_68_animation_frame,
                                                      pCar,
                                                      field_7C_pPed->get_target_car_door_403A60(),
                                                      newx,
                                                      newy,
                                                      field_40_rotation);
                        field_80_sprite_ptr->set_num_40F7B0(9);
                        newId = baseId + field_68_animation_frame;
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(field_68_animation_frame,
                                                      pCar,
                                                      field_7C_pPed->get_target_car_door_403A60(),
                                                      newx,
                                                      newy,
                                                      field_40_rotation);
                        frame_limit = 3;
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 4:
                case 5:
                case 6:
                case 7:
                    if (field_58_flags & 0x10)
                    {
                        frame_limit = 1;
                        CarDoorAlignmentSolver_545AF0(7 - field_68_animation_frame,
                                                      pCar,
                                                      field_7C_pPed->get_target_car_door_403A60(),
                                                      newx,
                                                      newy,
                                                      field_40_rotation);
                        newId = baseId + field_68_animation_frame;
                        field_80_sprite_ptr->set_num_40F7B0(9);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(field_68_animation_frame,
                                                      pCar,
                                                      field_7C_pPed->get_target_car_door_403A60(),
                                                      newx,
                                                      newy,
                                                      field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->set_num_40F7B0(9);
                        newId = field_68_animation_frame + baseId + 28;
                    }
                    break;

                case 8:
                    CarDoorAlignmentSolver_545AF0(field_68_animation_frame,
                                                  pCar,
                                                  field_7C_pPed->get_target_car_door_403A60(),
                                                  newx,
                                                  newy,
                                                  field_40_rotation);
                    field_80_sprite_ptr->set_num_40F7B0(9);
                    field_7C_pPed->ChangeNextPedState2_45C540(10);
                    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                    field_C_ped_state_2 = 10;
                    field_8_ped_state_1 = 10;
                    return;

                case 9:
                {
                    frame_limit = 2;
                    newId = field_68_animation_frame + baseId + 19;
                    Ped* pDriver = pCar->field_54_driver;
                    if (pDriver)
                    {
                        switch (pDriver->get_occupation_403980())
                        {
                            case ped_ocupation_enum::unknown_2:
                                pDriver = gPedManager_6787BC->SpawnRunAwayGuy_470D60();
                                if (pCar->field_84_car_info_idx == car_model_enum::apc ||
                                    pCar->field_84_car_info_idx == car_model_enum::JEEP ||
                                    pCar->field_84_car_info_idx == car_model_enum::TANK)
                                {
                                    pDriver->field_26C_graphic_type = 2;
                                    pDriver->set_remap_433B90(4);
                                }
                                pDriver->set_field_140_492CB0(pCar);
                                pDriver->field_180_car_thief = field_7C_pPed;
                                break;

                            case ped_ocupation_enum::driver:
                                pDriver->set_occupation_403970(ped_ocupation_enum::robbed_driver_10);
                                pDriver->set_field_140_492CB0(pCar);
                                pDriver->field_180_car_thief = field_7C_pPed;
                                break;

                            case ped_ocupation_enum::police:
                                if (field_7C_pPed->field_20A_wanted_points < 600)
                                {
                                    gPolice_7B8_6FEE40->RegisterCriminal_56F940(field_7C_pPed);
                                    field_7C_pPed->field_20A_wanted_points = 600;
                                }
                                gPolice_7B8_6FEE40->UpdateLastSeenCoordsForCriminal_5708C0(field_7C_pPed);
                                if (pDriver->get_objective_403A80() == objectives_enum::objective_43)
                                {
                                    gPolice_7B8_6FEE40->TryAssignCarCrewToCriminal_5707B0(pDriver->field_16C_car, field_7C_pPed);
                                }
                                pDriver->SetObjective(objectives_enum::no_obj_0, 9999);
                                break;

                            default:
                                if (pDriver->field_17C_pGang)
                                {
                                    pDriver->SetObjective2_463830(20, 9999);
                                    pDriver->set_field_14C_403AE0(field_7C_pPed);
                                }
                                pDriver->set_field_140_492CB0(pCar);
                                pDriver->field_180_car_thief = field_7C_pPed;
                                break;
                        }

                        if (field_7C_pPed->GetInternalObjective_403A90() == 35 && field_7C_pPed->get_field_226_4039D0() == 1)
                        {
                            field_7C_pPed->set_field_226_403B50(0);
                        }
                        pDriver->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                        pDriver->ChangeNextPedState2_45C540(17);
                        pCar->ClearDriver_4407F0();
                        CarDoorAlignmentSolver_545AF0(8, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        pDriver->AllocCharB4_45C830(
                            newx,
                            newy,
                            *gMap_0x370_6F6268->sub_4E4E50(&driverZ, newx, newy, pCar->field_50_car_sprite->field_1C_zpos));
                        pDriver->field_168_game_object->set_rotation_433A30(pCar->field_50_car_sprite->field_0);
                        pDriver->field_168_game_object->Set_F8_ped_state_1_433910(8);
                        pDriver->set_target_car_door_403A70(field_7C_pPed->get_target_car_door_403A60());
                        pDriver->field_168_game_object->SetPedState2_433A50(17);
                        pDriver->field_168_game_object->field_84_target_car = pCar;
                        pDriver->field_168_game_object->field_80_sprite_ptr->set_num_40F7B0(6);
                        {
                            pDriver->SetRemap_433C10(pDriver->get_remap_433BA0());
                        }
                        pDriver->field_16C_car = 0;
                        pDriver->Set_B4_F16_To_1_433B50();
                        if (!pDriver->is_player_41B0A0())
                        {
                            pDriver->Set_F250_IfBit_433DD0(12);
                        }
                    }
                    CarDoorAlignmentSolver_545AF0(field_68_animation_frame,
                                                  pCar,
                                                  field_7C_pPed->get_target_car_door_403A60(),
                                                  newx,
                                                  newy,
                                                  field_40_rotation);
                    break;
                }

                case 10:
                case 11:
                case 12:
                    CarDoorAlignmentSolver_545AF0(field_68_animation_frame,
                                                  pCar,
                                                  field_7C_pPed->get_target_car_door_403A60(),
                                                  newx,
                                                  newy,
                                                  field_40_rotation);
                    newId = field_68_animation_frame + baseId + 19;
                    frame_limit = 2;
                    break;
            }

            field_80_sprite_ptr->set_xyz_lazy_420600(
                newx,
                newy,
                *gMap_0x370_6F6268->sub_4E4E50(&carZ69, newx, newy, pCar->field_50_car_sprite->field_1C_zpos));

            if (field_58_flags & 0x10)
            {
                field_40_rotation = field_40_rotation + kAng180_6FD936;
            }

            if ((u8)field_70_frame_timer > frame_limit)
            {
                field_70_frame_timer = 0;
                field_68_animation_frame++;
                if (field_68_animation_frame == 13)
                {
                    field_68_animation_frame = 4;
                }
            }
            else if (field_70_frame_timer == 5 && field_68_animation_frame == 0)
            {
                Car_Door_10* pDoor = field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60());
                if (!(field_58_flags & 0x10))
                {
                    pDoor->Open_439E60();
                    pDoor->set_ped_421380(field_7C_pPed);
                }
            }
            break;
        }

        case 8:
        {
            Car_BC* pCar = field_84_target_car;
            pCar->field_76_last_seen_timer = 0;
            switch (field_68_animation_frame)
            {
                case 0:
                    CarDoorAlignmentSolver_545AF0(4, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    break;
                case 1:
                    CarDoorAlignmentSolver_545AF0(2, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    break;
                case 2:
                    CarDoorAlignmentSolver_545AF0(99, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    break;
            }

            field_80_sprite_ptr->set_xyz_lazy_420600(newx, newy, pCar->field_50_car_sprite->field_1C_zpos);
            field_80_sprite_ptr->set_num_40F7B0(6);
            byte_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                      field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                      field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);
            ManageZCoordAndSlopes_548590();
            if ((u8)field_70_frame_timer > 2)
            {
                field_70_frame_timer = 0;
                field_68_animation_frame++;
            }
            newId = baseId + 36;
            break;
        }

        case Char_Anim_state::Exiting_Car_7:
        {
            Car_BC* pCar = field_84_target_car;
            pCar->field_76_last_seen_timer = 0;
            switch (field_68_animation_frame)
            {
                case 0:
                    if (field_58_flags & 0x10)
                    {
                        field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60())->Open_439E60();
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(0, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(7, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 3;
                        field_80_sprite_ptr->set_num_40F7B0(9);
                        newId = baseId + 36;
                    }
                    break;

                case 1:
                    if (field_58_flags & 0x10)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(1, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(6, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 3;
                        field_80_sprite_ptr->set_num_40F7B0(9);
                        newId = baseId + 35;
                    }
                    break;

                case 2:
                    if (field_58_flags & 0x10)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(2, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(5, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 3;
                        field_80_sprite_ptr->set_num_40F7B0(9);
                        newId = baseId + 34;
                    }
                    break;

                case 3:
                    if (field_58_flags & 0x10)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(3, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(4, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 3;
                        field_80_sprite_ptr->set_num_40F7B0(9);
                        newId = baseId + 33;
                    }
                    break;

                case 4:
                    if (field_58_flags & 0x10)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(4, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(0, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->set_num_40F7B0(23);
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 5:
                    if (field_58_flags & 0x10)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(5, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(1, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->set_num_40F7B0(23);
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 6:
                    if (field_58_flags & 0x10)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(6, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(2, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->set_num_40F7B0(23);
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 7:
                    if (field_58_flags & 0x10)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(7, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(3, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->set_num_40F7B0(23);
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 8:
                {
                    field_80_sprite_ptr->set_num_40F7B0(23);
                    field_7C_pPed->ChangeNextPedState2_45C540(0);
                    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::walking_0);
                    field_C_ped_state_2 = 0;
                    field_8_ped_state_1 = 0;
                    Car_Door_10* pDoor = field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60());
                    if (!(field_58_flags & 0x10))
                    {
                        pDoor->Close_439EA0();
                    }
                    pDoor->set_ped_421380(0);

                    if ((u8)IsOnWater_545570())
                    {
                        field_7C_pPed->PutOutFire();
                        DrownPed_5459E0();
                    }
                    else
                    {
                        byte_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                                  field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                                  field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);
                        ManageZCoordAndSlopes_548590();
                    }
                    return;
                }
            }

            field_80_sprite_ptr->set_xyz_lazy_420600(
                newx,
                newy,
                *gMap_0x370_6F6268->sub_4E4E50(&carZ7, newx, newy, pCar->field_50_car_sprite->field_1C_zpos));

            if ((u8)field_70_frame_timer > frame_limit)
            {
                field_70_frame_timer = 0;
                field_68_animation_frame++;
                if (field_68_animation_frame == 13)
                {
                    field_68_animation_frame = 4;
                }
            }
            else if (field_70_frame_timer == 1 && field_68_animation_frame == 0)
            {
                Car_Door_10* pDoor = field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60());
                if (!(field_58_flags & 0x10))
                {
                    pDoor->Open_439E60();
                    pDoor->set_ped_421380(field_7C_pPed);
                }
            }
            break;
        }

        case Char_Anim_state::Jumping_5:
        {
            if (field_7C_pPed->field_15C_player)
            {
                field_71_frame_delay = 2;
            }
            else
            {
                field_71_frame_delay = 1;
            }

            switch (field_68_animation_frame)
            {
                case 8:
                {
                    field_10_char_state = 1;
                    field_6C_animation_state = 2;
                    field_68_animation_frame = 0;
                    s16 landId = baseId + 23;
                    groundZ = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x,
                                                                            field_80_sprite_ptr->field_14_xy.y);
                    gMap_0x370_6F6268->UpdateZFromSlopeAtCoord_4E5BF0(field_80_sprite_ptr->field_14_xy.x,
                                                                      field_80_sprite_ptr->field_14_xy.y,
                                                                      groundZ);
                    Fix16 fall = field_80_sprite_ptr->field_1C_zpos - groundZ;
                    if (fall > kFP16Zero_6FD9E4)
                    {
                        gCharB4_Saved_Zpos_6FD7FC -= fall;
                        gCharB4_Saved_Zpos_6FD7FC = SnapZTo16_4F79B0(gCharB4_Saved_Zpos_6FD7FC);
                        ManageZCoordAndSlopes_548590();
                    }
                    else
                    {
                        gCharB4_Saved_Zpos_6FD7FC = groundZ;
                    }

                    if (field_80_sprite_ptr->field_1C_zpos < field_8C_jump_base_z)
                    {
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x,
                                                                 field_80_sprite_ptr->field_14_xy.y,
                                                                 field_8C_jump_base_z);
                    }
                    field_80_sprite_ptr->set_id_lazy_4206C0(landId);
                    return;
                }

                case 6:
                case 7:
                {
                    groundZ = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x,
                                                                            field_80_sprite_ptr->field_14_xy.y);
                    gMap_0x370_6F6268->UpdateZFromSlopeAtCoord_4E5BF0(field_80_sprite_ptr->field_14_xy.x,
                                                                      field_80_sprite_ptr->field_14_xy.y,
                                                                      groundZ);
                    Fix16 fall = (field_80_sprite_ptr->field_1C_zpos - groundZ) / gFix16_Two_6FD9EC;
                    if (fall > kFP16Zero_6FD9E4)
                    {
                        gCharB4_Saved_Zpos_6FD7FC -= fall;
                        gCharB4_Saved_Zpos_6FD7FC = SnapZTo16_4F79B0(gCharB4_Saved_Zpos_6FD7FC);
                        ManageZCoordAndSlopes_548590();
                    }
                    else
                    {
                        gCharB4_Saved_Zpos_6FD7FC = groundZ;
                    }
                    break;
                }

                case 5:
                    field_71_frame_delay = 3;
                    field_54_jump_scale_counter = 12;
                    break;

                case 1:
                case 2:
                case 3:
                case 4:
                    gCharB4_Saved_Zpos_6FD7FC += dword_6FDAE4;
                    gCharB4_Saved_Zpos_6FD7FC = SnapZTo16_4F79B0(gCharB4_Saved_Zpos_6FD7FC);
                    if (field_58_flags & 1)
                    {
                        byte_6FDB54 = 0;
                    }
                    ManageZCoordAndSlopes_548590();
                    break;

                case 0:
                    field_54_jump_scale_counter = 0;
                    gCharB4_Saved_Zpos_6FD7FC += dword_6FDAE4;
                    gCharB4_Saved_Zpos_6FD7FC = SnapZTo16_4F79B0(gCharB4_Saved_Zpos_6FD7FC);
                    if (field_58_flags & 1)
                    {
                        byte_6FDB54 = 0;
                    }
                    ManageZCoordAndSlopes_548590();
                    break;
            }

            field_54_jump_scale_counter++;
            s16 jumpId = field_68_animation_frame + baseId + 16;
            if ((u8)field_70_frame_timer > (u8)field_71_frame_delay)
            {
                field_70_frame_timer = 0;
                field_68_animation_frame++;
            }
            field_80_sprite_ptr->set_id_lazy_4206C0(197);
            field_80_sprite_ptr->set_id_lazy_4206C0(jumpId);

            if ((u8)field_54_jump_scale_counter < 12)
            {
                field_80_sprite_ptr->ApplyScaleToDimensions_59E4C0(kFP16One_6FD9E8 + Fix16((u8)field_54_jump_scale_counter) * dword_6FD9A8,
                                                                   0);
            }
            else if ((u8)field_54_jump_scale_counter < 24)
            {
                field_80_sprite_ptr->ApplyScaleToDimensions_59E4C0(kFP16One_6FD9E8 +
                                                                       Fix16(25 - (u8)field_54_jump_scale_counter) * dword_6FD9A8,
                                                                   0);
            }
            return;
        }

        case Char_Anim_state::Normal_Fall_11:
        case Char_Anim_state::Lethal_Fall_12:
            field_71_frame_delay = 1;
            switch (field_68_animation_frame)
            {
                case 10:
                case 11:
                case 12:
                case 13:
                    field_7C_pPed->TakeDamage(4);
                    // fall through
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    newId = field_68_animation_frame + baseId + 81;
                    break;
                case 14:
                case 15:
                case 16:
                    newId = field_68_animation_frame + baseId + 81;
                    field_6C_animation_state = Char_Anim_state::Lethal_Fall_12;
                    break;
            }

            if ((u8)field_70_frame_timer > (u8)field_71_frame_delay)
            {
                if (field_68_animation_frame < 16)
                {
                    field_68_animation_frame++;
                }
                field_70_frame_timer = 0;
            }
            break;

        case 10:
            newId = baseId + 72;
            break;

        case 14:
            newId = baseId + 80;
            break;

        case 13:
            newId = baseId + 72;
            break;

        case 19:
            newId = baseId + 156;
            break;

        case 20:
            newId = baseId + 157;
            break;

        case 21:
            newId = baseId + 155;
            break;

        case 15:
            if ((u8)field_70_frame_timer > (field_68_animation_frame > 5 ? 3 : 4))
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7)
                {
                    switch (gRng_6F6784.get_int_4F7AE0(2))
                    {
                        case 0:
                            field_6C_animation_state = 19;
                            break;
                        case 1:
                            field_6C_animation_state = Char_Anim_state::Unknown_13;
                            break;
                    }
                    field_68_animation_frame = 7;
                }
            }
            newId = field_68_animation_frame + baseId + 65;
            break;

        case 16:
            if ((u8)field_70_frame_timer > (field_68_animation_frame > 5 ? 3 : 4))
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7)
                {
                    switch (gRng_6F6784.get_int_4F7AE0(3))
                    {
                        case 0:
                            field_6C_animation_state = Char_Anim_state::Unknown_14;
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
            newId = field_68_animation_frame + baseId + 73;
            break;

        case 17:
            if ((u8)field_70_frame_timer > 3)
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 4)
                {
                    field_68_animation_frame = 0;
                }
                field_70_frame_timer = 0;
            }

            if (field_68_animation_frame)
            {
                field_80_sprite_ptr->SetPaletteSprites_420700();
            }
            field_80_sprite_ptr->set_num_40F7B0(6);
            newId = field_68_animation_frame + baseId + 151;
            break;
    }

    field_80_sprite_ptr->set_id_lazy_4206C0(newId);
}

MATCH_FUNC(0x548590)
void Char_B4::ManageZCoordAndSlopes_548590()
{
    Fix16 zpos = kFP16Zero_6FD9E4;
    zpos = field_80_sprite_ptr->field_1C_zpos;

    if (byte_6FDB54 == 1)
    {
        zpos--;
    }
    u8 gradient_direction =
        gMap_0x370_6F6268->UpdateZFromSlopeAtCoord_4E5BF0(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y, zpos);

    if (gradient_direction == NO_GRADIENT_SLOPE_0)
    {
        if (field_45_slope_gradient_direction)
        {
            Fix16 frac = zpos.GetFracValue();
            zpos = zpos.GetRoundValue();

            if (frac > dword_6FDB28)
            {
                zpos++;
            }
        }
        else
        {
            field_58_flags_bf.b0 = false;
            zpos = Fix16(zpos.ToUInt8());
        }
        field_45_slope_gradient_direction = 0;
    }
    else
    {
        field_45_slope_gradient_direction = gradient_direction;
        field_58_flags_bf.b0 = true;
    }
    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y, zpos);
}

DEFINE_GLOBAL_INIT(Ang16, word_6FD888, Ang16(64), 0x6FD888);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD860, Fix16(0x80, 0), 0x6FD860);

// The angle is a by-value parameter: the original keeps it in memory and adds it as a dword
static inline Ang16 TurnBy16Deg_548BD0(Ang16 angle)
{
    return word_6FD888 + angle;
}

// 9.6f 0x499F00
MATCH_FUNC(0x548670)
void Char_B4::DispatchCollision_548670(char_type a2)
{
    u8 bUnknown; // bl
    if (this->field_69_is_colliding_with_sprite == 1 && this->field_20 && this->field_24 != 3)
    {
        bUnknown = 1;
    }
    else
    {
        bUnknown = a2;
    }

    Sprite* pNearSprite = field_80_sprite_ptr->QuerySpriteCollision_59E7D0(2);
    if (pNearSprite)
    {
        s32 sprite_type_enum = pNearSprite->field_30_sprite_type_enum;
        switch (pNearSprite->get_type_416B40())
        {

            case 3: // char_b4
                if (bUnknown == byte_623F48)
                {
                    this->field_18_collided_entity = 0;
                }
                else
                {
                    if (field_7C_pPed->GetPedType_420B70() >= 2 && field_7C_pPed->field_238_ped_type <= 6)
                    {
                        Char_B4::HandlePedCollision_548BD0(pNearSprite->AsCharB4_40FEA0());
                        this->field_18_collided_entity = 0;
                    }
                }
                break;

            case 2: // car
            {
                Sprite* pNearSprite2 = field_80_sprite_ptr->QuerySpriteCollision_59E7D0(1);
                if (pNearSprite2->get_type_416B40() == 1) // object
                {
                    if (pNearSprite2->As2C_40FEC0())
                    {
                        Char_B4::HandleObjectCollision_548840(pNearSprite2->As2C_40FEC0());
                        this->field_20 = 3;
                    }
                }
                else
                {
                    Char_B4::HandleGenericCollision_54A530(pNearSprite->AsCar_40FEB0(), 0, 0);
                    this->field_20 = 1;
                }
                break;
            }
            case 1:
            case 4:
            case 5: // object
                Char_B4::HandleObjectCollision_548840(pNearSprite->As2C_40FEC0());
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
                this->field_1C_prev_collided_entity = 0;
                this->field_20 = 0;
                this->field_18_collided_entity = 0;
            }
            else
            {
                this->field_18_collided_entity = 0;
            }
        }
        else
        {
            this->field_10_char_state = 1;
            this->field_18_collided_entity = 0;
        }

        if (field_38_velocity == kZeroVelocity_6FD7C0 || field_7C_pPed->IsField238_45EDE0(2) && !this->field_8_ped_state_1)
        {
            this->field_1C_prev_collided_entity = 0;
            this->field_20 = 0;
            this->field_69_is_colliding_with_sprite = 0;
            this->field_2A = kAng0_6FDB34;
        }
    }
}

// PolarToCartesian_41FC20 as ApplyMovement_54CC40 expands it: the big function is past the
// inline budget, so the original calls Multiply_408680 instead of inlining operator*
static inline void PolarToCartesian_OutOfLineMul(Ang16& angle, const Fix16& radius, Fix16& ret1, Fix16& ret2)
{
    ret1 = Ang16::sine_40F500(angle).Multiply_408680(radius);
    ret2 = Ang16::cosine_40F520(angle).Multiply_408680(radius);
}

// Ang16 operator+ with the normalizing ctor called out of line (AssignNormalized_409300)
static inline Ang16 AddAngles_ool_54B8F0(const Ang16& a, const Ang16& b)
{
    s16 sum = a.rValue + b.rValue;
    return Ang16((Ang16&)sum, 0);
}

static inline bool CanJumpOver_54A530(Char_B4* pThis, Char_B4* pChar)
{
    if (!pThis->field_7C_pPed->IsField238_45EDE0(2))
    {
        if (!pChar)
        {
            return true;
        }
        return false;
    }
    if (pThis->field_8_ped_state_1 == ped_state_1::entering_car_3)
    {
        return true;
    }
    return false;
}

// RotateAndTranslatePoint_42A720 as HandleGenericCollision_54A530 expands it: past the inline
// budget; the multiplies, the negate and the adds are the named out-of-line copies, the
// subtractions follow the budget
static inline void RotateAndTranslatePoint_OOL_42A720(Fix16& pInX,
                                                      Fix16& pInY,
                                                      Ang16& pRotAng,
                                                      Fix16& pTransX,
                                                      Fix16& pTransY,
                                                      Fix16& pRotTransX,
                                                      Fix16& pRotTransY)
{
    pRotTransX = (pInX - pTransX)
                     .Multiply_408680(Ang16::cosine_40F520(pRotAng))
                     .Add_408660((pInY - pTransY).Multiply_408680(Ang16::sine_40F500(pRotAng)));
    pRotTransY = (pInX - pTransX)
                     .Negate_4086A0()
                     .Multiply_408680(Ang16::sine_40F500(pRotAng))
                     .Add_408660((pInY - pTransY).Multiply_408680(Ang16::cosine_40F520(pRotAng)));
}

MATCH_FUNC(0x548840)
void Char_B4::HandleObjectCollision_548840(Object_2C* pObj)
{
    //pObj_ = pObj;
    //out3 = 0;
    //v19 = 4;
    //pPhi = pObj->field_8;
    //phi_type = pPhi->field_34_type;
    Fix16_Point a4;
    Fix16_Point point;
    // Unused: the original enters with EH state 4, so three more Fix16_Point locals were constructed up front
    Fix16_Point unused_1, unused_2, unused_3;
    u8 a6;
    u8 out2;
    u8 out3 = 0;
    bool bUnknown = 0;

    s32 phi_type = pObj->field_8->field_34_behavior_type;

    if (!pObj->is_not_type6_to_12_421080() || (pObj->get_field_26_420FF0()) == 0 ||
        pObj->get_field_26_420FF0() != this->field_7C_pPed->field_267_varrok_idx)
    {
        if (!pObj->sub_482C90())
        {
            goto LABEL_28;
        }

        if (field_7C_pPed->IsField238_45EDE0(2))
        {
            if (this->field_10_char_state == 15)
            {
                goto LABEL_28;
            }
        }
        else
        {
            if (pObj->field_8->field_4C == 3)
            {
                bUnknown = 1;
            }

            if (pObj->field_8->field_4C && !bUnknown)
            {
                goto LABEL_28;
            }
        }

        if (pObj->GetMass_482C80() < dword_6FDAB0)
        {

            field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);

            point = field_80_sprite_ptr
                        ->FindCollisionIntersectionPoint_5A2710(pObj->field_4, a4, field_80_sprite_ptr->field_0, a6, out2, out3);
            pObj->ResolveCollisionWithPed_5229B0(this, &point, 1);
            return;
        }
        this->field_5C = 10;

    LABEL_28:
        HandleGenericCollision_54A530(0, pObj, 0);
    }
}

// What happens when this ped walks into pOther, by the types of both peds (field_238)
WIP_FUNC(0x548bd0)
void Char_B4::HandlePedCollision_548BD0(Char_B4* pOther)
{
    s32 my_type = field_7C_pPed->field_238_ped_type;
    s32 other_type = pOther->field_7C_pPed->field_238_ped_type;
    if (pOther->field_8_ped_state_1 == 9 || pOther->field_8_ped_state_1 == 8 || (field_58_flags & 0x80) || field_10_char_state == 15)
    {
        return;
    }

    Ped* pMyPed = field_7C_pPed;
    Ped* pOtherPed = pOther->field_7C_pPed;
    if (pMyPed->field_15C_player && pOtherPed->field_240_occupation == ped_ocupation_enum::elvis)
    {
        Char_B4* pMyChar = pMyPed->field_168_game_object;
        if ((s32)pMyChar != pOtherPed->field_138)
        {
            pOtherPed->field_138 = (s32)pMyChar;
            pOtherPed->field_224 &= ~4;
            pMyChar->field_7C_pPed->field_138 = (s32)pOtherPed->field_168_game_object;
            if (!(pOtherPed->field_21C & 0x1000000))
            {
                pOtherPed->field_250 = ped_ocupation_enum::elvis;
            }
        }
    }
    else if (pMyPed->field_15C_player || !(pMyPed->field_200_id & 3))
    {
        if (!pOtherPed->field_15C_player && (s32)pOther != pMyPed->field_138)
        {
            pMyPed->field_224 |= 4;
            pMyPed->field_138 = (s32)pOther;
            pOther->field_7C_pPed->field_138 = (s32)pMyPed->field_168_game_object;
        }
    }

    switch (my_type)
    {
        case 2:
            switch (other_type)
            {
                case 5:
                    if (field_7C_pPed->field_164_ped_group && pOther->field_7C_pPed->field_164_ped_group == field_7C_pPed->field_164_ped_group)
                    {
                        break;
                    }
                    if (pOther->field_7C_pPed->field_240_occupation != 43)
                    {
                        pOther->field_6A = 4;
                        pOther->field_74 = Ang16(Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                                           field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x).rValue + kAng180_6FD936.rValue).Normalized_406C20();
                    }
                    else
                    {
                        field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
                    }
                    break;
                case 2:
                    field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
                    break;
                case 3:
                case 4:
                case 6:
                    field_10_char_state = 1;
                    if (field_38_velocity == kZeroVelocity_6FD7C0)
                    {
                        break;
                    }
                    if (field_7C_pPed->field_164_ped_group && pOther->field_7C_pPed->field_164_ped_group == field_7C_pPed->field_164_ped_group)
                    {
                        break;
                    }
                    pOther->field_6A = 4;
                    pOther->field_74 = Ang16(Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                                       field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x).rValue +
                                             kAng180_6FD936.rValue)
                                           .Normalized_406C20();
                    break;
            }
            break;

        case 5:
            switch (other_type)
            {
                case 5:
                    if (field_10_char_state == 10 || field_7C_pPed->field_200_id >= pOther->field_7C_pPed->field_200_id)
                    {
                        field_10_char_state = 1;
                    }
                    else if (!field_7C_pPed->field_164_ped_group || field_7C_pPed->field_164_ped_group->field_30)
                    {
                        if (field_8_ped_state_1 != 3)
                        {
                            if (field_7C_pPed->field_25C_internal_objective != 11)
                            {
                                field_40_rotation += word_6FD888;
                                field_10_char_state = 10;
                            }
                            else
                            {
                                field_10_char_state = 10;
                            }
                        }
                        else
                        {
                            field_38_velocity = kZeroVelocity_6FD7C0;
                        }
                    }
                    break;
                case 2:
                    // Nothing to do, but the original's jump table starts at 2 (an empty case is dropped)
                    return;
                case 3:
                case 4:
                case 6:
                {
                    pOther->field_6A = 4;
                    pOther->field_74 = Ang16(kAng180_6FD936.rValue + Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                                                              field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x).rValue)
                                           .Normalized_406C20();
                    break;
                }
            }
            break;

        case 4:
        case 6:
            switch (other_type)
            {
                case 4:
                case 6:
                    if (field_10_char_state == 10 || field_7C_pPed->field_200_id >= pOther->field_7C_pPed->field_200_id)
                    {
                        field_10_char_state = 1;
                        // A return, not a break: the original keeps the type 5 copy of this tail
                        return;
                    }
                    else if (!field_7C_pPed->field_164_ped_group || field_7C_pPed->field_164_ped_group->field_30)
                    {
                        if (field_8_ped_state_1 != 3)
                        {
                            if (field_7C_pPed->field_25C_internal_objective != 11)
                            {
                                field_40_rotation += word_6FD888;
                                field_10_char_state = 10;
                            }
                            else
                            {
                                field_10_char_state = 10;
                            }
                        }
                    }
                    break;
                case 2:
                    if (pOther->field_7C_pPed->field_20A_wanted_points >= 1)
                    {
                        field_7C_pPed->IsLawEnforcement_45B4E0();
                    }
                    break;
                case 3:
                {
                    pOther->field_6A = 4;
                    pOther->field_74 = Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                                 field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x) + kAng180_6FD936;
                    break;
                }
            }
            break;

        case 3:
        {
            s32 objective = field_7C_pPed->field_25C_internal_objective;
            if (objective == 48 || objective == 37 || objective == 38 || objective == 12)
            {
                field_38_velocity = k_dword_6FD7CC;
                break;
            }
            switch (other_type)
            {
                case 2:
                    if (pOther->field_7C_pPed->field_20A_wanted_points >= 1 && field_7C_pPed->IsLawEnforcement_45B4E0())
                    {
                        break;
                    }
                    if (pOther->field_38_velocity != kZeroVelocity_6FD7C0)
                    {
                        field_38_velocity = dword_6FD860;
                    }
                    else
                    {
                        field_40_rotation = Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                                      field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x);
                    }
                    break;
                case 5:
                    if (pOther->field_38_velocity != kZeroVelocity_6FD7C0)
                    {
                        field_38_velocity = dword_6FD860;
                    }
                    else
                    {
                        field_40_rotation = Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                                      field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x);
                    }
                    break;
                case 4:
                case 6:
                    field_38_velocity = dword_6FD860;
                    break;
                case 3:
                    if (!field_20)
                    {
                        pOther->field_6A = 4;
                        pOther->field_74 = TurnBy16Deg_548BD0(Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                                                         field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x) +
                                                              kAng180_6FD936);
                    }
                    break;
            }
            break;
        }
    }
}

// https://decomp.me/scratch/ph2wn
WIP_FUNC(0x54a530)
void Char_B4::HandleGenericCollision_54A530(Car_BC* pCar, Object_2C* pObj, Char_B4* pChar)
{
    WIP_IMPLEMENTED;
    Ang16 corner_ang;
    Ang16 hit_ang;
    Sprite* pSprt;
    Fix16 rot_x;
    Fix16 rot_y;
    Fix16 target_x;
    Fix16 target_y = kFP16Zero_6FD9E4;
    u8 side = 0;
    bool bNoJump = false;

    Fix16 xpos = field_80_sprite_ptr->field_14_xy.x;
    Fix16 ypos = field_80_sprite_ptr->field_14_xy.y;

    if (field_10_char_state != Char_B4_state::Jumping_15)
    {
        field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
        field_45_slope_gradient_direction = gCharB4_Saved_SlopeGradDir_6FD7B0.ToInt();
        field_10_char_state = Char_B4_state::Colliding_With_Car_27;
    }

    if (!field_69_is_colliding_with_sprite)
    {
        if (pCar)
        {
            corner_ang = pCar->GetCornerAngle_4403A0();
            pSprt = pCar->field_50_car_sprite;
            if (gGarage_48_6FD26C->IsMaybeParkingCar_493540(pCar))
            {
                bNoJump = true;
                byte_6FDB58 = 1;
            }
        }
        else if (pObj)
        {
            pSprt = pObj->field_4;
            Fix16 half_w;
            Fix16 half_h;
            half_w = pSprt->field_C_sprite_4c_ptr->field_0_width / 2;
            half_h = pSprt->field_C_sprite_4c_ptr->field_4_height / 2;
            corner_ang = Fix16::atan2_fixed_405320(half_h, half_w);
            if (pSprt->field_C_sprite_4c_ptr->GetF8_492170() > kFP16Half_6FD8E4)
            {
                bNoJump = true;
                byte_6FDB58 = 1;
            }
        }
        else
        {
            corner_ang = kAng45_6FD89C;
            pSprt = pChar->field_80_sprite_ptr;
            pSprt->field_0 = kAng0_6FDB34;
        }

        if ((field_7C_pPed->get_field_230_492C20() == 2 || field_5C > 0) && field_10_char_state != Char_B4_state::Jumping_15)
        {
            if (!bNoJump)
            {
                if (CanJumpOver_54A530(this, pChar))
                {
                    Char_B4::DoJump_5454D0();
                }
            }
        }

        if (!bNoJump)
        {
            if (field_10_char_state == Char_B4_state::Jumping_15)
            {
                if (field_68_animation_frame >= 5)
                {
                    field_68_animation_frame = 5;
                    field_71_frame_delay = 2;
                    field_70_frame_timer = 0;
                }
                field_69_is_colliding_with_sprite = 0;

                if (pCar)
                {
                    if (pCar->field_50_car_sprite->field_1C_zpos >= field_80_sprite_ptr->field_1C_zpos)
                    {
                        field_A0 = 1;
                        field_80_sprite_ptr->ResolveZOrder_5A1B30(pCar->field_50_car_sprite);
                    }
                }
                return;
            }
        }
        else
        {
            field_2A = kAng90_6FD854;
            if (field_7C_pPed->IsField238_45EDE0(2))
            {
                if (field_10_char_state != Char_B4_state::Jumping_15 && field_8_ped_state_1 != ped_state_1::entering_car_3)
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
                    field_45_slope_gradient_direction = gCharB4_Saved_SlopeGradDir_6FD7B0.ToInt();
                    field_10_char_state = Char_B4_state::Colliding_With_Car_27;
                    return;
                }
                else if (field_20 == 1)
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
                    field_40_rotation = Ang16(field_80_sprite_ptr->field_0.rValue + kAng180_6FD936.rValue).Normalized_406C20();
                    return;
                }
                else
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
                    field_40_rotation = pSprt->field_0;
                    rot_x = Ang16::sine_40F500(field_40_rotation) * field_38_velocity;
                    rot_y = Ang16::cosine_40F520(field_40_rotation).Multiply_408680(field_38_velocity);
                    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + rot_x,
                                                             field_80_sprite_ptr->field_14_xy.y + rot_y,
                                                             field_80_sprite_ptr->field_1C_zpos);
                }
            }
            else
            {
                if (field_10_char_state == Char_B4_state::Jumping_15 && field_68_animation_frame >= 5)
                {
                    field_68_animation_frame = 5;
                    field_71_frame_delay = 2;
                    field_70_frame_timer = 0;
                }
            }
        }

        // Work out which side of the obstacle we hit, in the obstacle's local space
        RotateAndTranslatePoint_OOL_42A720(field_80_sprite_ptr->field_14_xy.x,
                                       field_80_sprite_ptr->field_14_xy.y,
                                       Ang16(-pSprt->field_0.rValue).Normalized_406C20(),
                                       pSprt->field_14_xy.x,
                                       pSprt->field_14_xy.y,
                                       rot_x,
                                       rot_y);
        hit_ang = Fix16::atan2_fixed_405320(rot_y, rot_x);

        target_x = field_7C_pPed->Get_F1C4_x_492CE0();
        target_y = field_7C_pPed->Get_F1C4_y_492CF0();
        if (target_x == kFP16MinusOne_6FD790 || target_y == kFP16MinusOne_6FD790)
        {
            Ang16 rot = field_40_rotation;
            target_x = Ang16::sine_40F500(rot) * dword_6FDA04;
            target_y = Ang16::cosine_40F520(rot).Multiply_408680(dword_6FDA04);
            target_x += field_80_sprite_ptr->field_14_xy.x;
            target_y += field_80_sprite_ptr->field_14_xy.y;
        }

        RotateAndTranslatePoint_OOL_42A720(target_x, target_y, Ang16(-pSprt->field_0.rValue).Normalized_406C20(), pSprt->field_14_xy.x, pSprt->field_14_xy.y, rot_x, rot_y);
        target_x = pSprt->field_14_xy.x + rot_x;
        target_y = pSprt->field_14_xy.y + rot_y;
        RotateAndTranslatePoint_OOL_42A720(xpos, ypos, Ang16(-pSprt->field_0.rValue).Normalized_406C20(), pSprt->field_14_xy.x, pSprt->field_14_xy.y, rot_x, rot_y);
        xpos = pSprt->field_14_xy.x + rot_x;
        ypos = pSprt->field_14_xy.y + rot_y;

        if (hit_ang < Ang16(-corner_ang.rValue).Normalized_406C20())
        {
            side = 3;
            if (hit_ang < Ang16(kAng180_6FD936.rValue + corner_ang.rValue).Normalized_406C20())
            {
                side = 2;
                if (hit_ang < Ang16(kAng180_6FD936.rValue - corner_ang.rValue).Normalized_406C20())
                {
                    side = 1;
                    if (hit_ang < corner_ang)
                    {
                        side = 0;
                        if (target_x < xpos)
                        {
                            field_28 = Ang16(pSprt->field_0.rValue - kAng90_6FD854.rValue).Normalized_406C20();
                            field_2A = kAng90_6FD8A2;
                        }
                        else
                        {
                            field_28 = Ang16(pSprt->field_0.rValue + kAng90_6FD854.rValue).Normalized_406C20();
                            field_2A = kAng270_6FD94C;
                        }
                    }
                    else if (target_y < ypos)
                    {
                        field_28 = pSprt->field_0 + kAng180_6FD936;
                        field_2A = kAng270_6FD94C;
                    }
                    else
                    {
                        field_28 = pSprt->field_0;
                        field_2A = kAng90_6FD8A2;
                    }
                }
                else if (target_x < xpos)
                {
                    field_28 = pSprt->field_0 - kAng90_6FD854;
                    field_2A = kAng270_6FD94C;
                }
                else
                {
                    field_28 = pSprt->field_0 + kAng90_6FD854;
                    field_2A = kAng90_6FD8A2;
                }
            }
            else if (target_y < ypos)
            {
                if (field_2A == kAng90_6FD8A2)
                {
                    field_28 = pSprt->field_0 + kAng180_6FD936;
                    field_2A = kAng90_6FD8A2;
                }
                else
                {
                    field_28 = pSprt->field_0;
                    field_2A = kAng270_6FD94C;
                }
            }
            else
            {
                if (field_2A == kAng90_6FD8A2)
                {
                    field_28 = pSprt->field_0 + kAng180_6FD936;
                    field_2A = kAng90_6FD8A2;
                }
                else
                {
                    field_28 = pSprt->field_0;
                    field_2A = kAng270_6FD94C;
                }
            }
        }
        else if (target_x < xpos)
        {
            field_28 = pSprt->field_0 - kAng90_6FD854;
            field_2A = kAng90_6FD8A2;
        }
        else
        {
            field_28 = pSprt->field_0 + kAng90_6FD854;
            field_2A = kAng270_6FD94C;
        }

        field_69_is_colliding_with_sprite = true;
        field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);

        if (field_7C_pPed->IsField238_45EDE0(2))
        {
            if (field_7C_pPed->GetInternalObjective_403A90() != objectives_enum::enter_car_as_driver_35 ||
                gGarage_48_6FD26C->IsMaybeParkingCar_493540(field_7C_pPed->get_target_to_enter_403B10()))
            {
                return;
            }
        }

        Fix16 step_x;
        Fix16 step_y;
        Ang16::PolarToCartesian_41FC20(field_28, field_38_velocity, step_x, step_y);
        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + step_x,
                                                 field_80_sprite_ptr->field_14_xy.y + step_y,
                                                 field_80_sprite_ptr->field_1C_zpos);
        Char_B4::DispatchCollision_548670(byte_623F48);

        if (field_18_collided_entity)
        {
            switch (side)
            {
                case 1:
                    field_28 = pSprt->field_0 - kAng90_6FD854;
                    break;
                case 2:
                    field_28 = pSprt->field_0;
                    break;
                case 3:
                    field_28 = pSprt->field_0 - kAng90_6FD854;
                    break;
            }

            if (field_10_char_state != Char_B4_state::Jumping_15)
            {
                field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
            }
            else if (field_68_animation_frame >= 5)
            {
                field_68_animation_frame = 5;
                field_71_frame_delay = 2;
                field_70_frame_timer = 0;
            }
        }

        if (pCar)
        {
            field_1C_prev_collided_entity = pCar;
            field_18_collided_entity = pCar;
        }
        else if (pObj)
        {
            field_1C_prev_collided_entity = pObj;
            field_18_collided_entity = pObj;
        }
        else
        {
            field_1C_prev_collided_entity = pChar;
            field_18_collided_entity = pChar;
        }
    }
    else
    {
        void* pAny = pCar;
        if (!pCar)
        {
            pAny = pObj;
            if (pObj)
            {
                if (pObj->field_4->field_C_sprite_4c_ptr->GetF8_492170() > kFP16Half_6FD8E4)
                {
                    bNoJump = true;
                }
            }
            else
            {
                pAny = pChar;
            }
        }

        if (field_1C_prev_collided_entity == pAny || !field_1C_prev_collided_entity)
        {
            if (field_10_char_state != Char_B4_state::Jumping_15)
            {
                field_1C_prev_collided_entity = pAny;
                field_18_collided_entity = pAny;
            }
            else
            {
                field_69_is_colliding_with_sprite = false;
                field_2A = kAng0_6FDB34;
            }
        }
        else if (field_20 != 2 && !bNoJump)
        {
            if (field_10_char_state != Char_B4_state::Jumping_15)
            {
                Char_B4::DoJump_5454D0();
            }
            else if (field_68_animation_frame >= 5)
            {
                field_68_animation_frame = 5;
                field_71_frame_delay = 2;
                field_70_frame_timer = 0;
            }
        }
    }
}

// https://decomp.me/scratch/aRlEV
WIP_FUNC(0x54b8f0)
char_type Char_B4::ContinueMovementAfterCollision_54B8F0()
{
    WIP_IMPLEMENTED;
    Fix16 x_vec;
    Fix16 y_vec = kFP16Zero_6FD9E4;
    if (field_10_char_state == Char_B4_state::Jumping_15)
    {
        return 1;
    }
    volatile char_type bMoved = false;

    if (field_18_collided_entity)
    {
        field_40_rotation = field_28;
        field_24 = 1;
        PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity, x_vec, y_vec);
        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                 field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                 field_80_sprite_ptr->field_1C_zpos);
        Char_B4::DispatchCollision_548670(byte_623F48);
        if (field_18_collided_entity)
        {
            field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
            field_28.rValue += field_2A.rValue;
            field_28.Normalize_406C20();
            PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity, x_vec, y_vec);
            field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                     field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                     field_80_sprite_ptr->field_1C_zpos);
            Char_B4::DispatchCollision_548670(byte_623F48);
            if (field_18_collided_entity)
            {
                field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                return bMoved;
            }
        }
    }
    else
    {
        PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity * gFix16_Two_6FD9EC, x_vec, y_vec);
        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                 field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                 field_80_sprite_ptr->field_1C_zpos);
        Char_B4::DispatchCollision_548670(byte_623F48);
        if (field_18_collided_entity)
        {
            field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);

            // Try stepping back away from the obstacle
            field_40_rotation = field_28;
            Ang16 back_angle = AddAngles_ool_54B8F0(kAng180_6FD936, AddAngles_ool_54B8F0(field_28, field_2A));
            PolarToCartesian_OutOfLineMul(back_angle, field_38_velocity * kFP16Four_6FD9F4, x_vec, y_vec);
            field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                     field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                     field_80_sprite_ptr->field_1C_zpos);
            Char_B4::DispatchCollision_548670(byte_623F48);
            if (!field_18_collided_entity && field_24 != 2)
            {
                // Free behind: turn around
                field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                field_28.rValue += AddAngles_ool_54B8F0(kAng180_6FD936, field_2A).rValue;
                field_28.Normalize_406C20();
                field_24 = 2;
                field_40_rotation = field_28;
                PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity * gFix16_Two_6FD9EC, x_vec, y_vec);
                field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                         field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                         field_80_sprite_ptr->field_1C_zpos);
                Char_B4::DispatchCollision_548670(byte_623F48);
                if (field_18_collided_entity)
                {
                    field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                    field_28.rValue += field_2A.rValue;
                    field_28.Normalize_406C20();
                    field_24 = 0;
                    field_40_rotation = field_28;
                    PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity, x_vec, y_vec);
                    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                             field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                             field_80_sprite_ptr->field_1C_zpos);
                    bMoved = true;
                    Char_B4::DispatchCollision_548670(byte_623F48);
                    if (field_18_collided_entity)
                    {
                        field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                    }
                    return bMoved;
                }
            }
            else
            {
                field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity, x_vec, y_vec);
                field_80_sprite_ptr->set_xyz_lazy_451950(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                         field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                         field_80_sprite_ptr->field_1C_zpos);
                Char_B4::DispatchCollision_548670(byte_623F48);
                if (field_18_collided_entity)
                {
                    field_80_sprite_ptr->setxy_lazy_54EC80(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                    return bMoved;
                }
            }
        }
        else
        {
            field_69_is_colliding_with_sprite = 0;
            field_80_sprite_ptr->setxy_lazy_54EC80(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
            Ang16::PolarToCartesian_451730(field_40_rotation, field_38_velocity, x_vec, y_vec);
            field_80_sprite_ptr->set_xyz_lazy_451950(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                     field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                     field_80_sprite_ptr->field_1C_zpos);
            Char_B4::DispatchCollision_548670(byte_623F48);
            if (field_18_collided_entity)
            {
                field_80_sprite_ptr->setxy_lazy_54EC80(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                return bMoved;
            }
        }
    }
    bMoved = true;
    return bMoved;
}

// 9.6f 0x491F10: the face (1-4) next to `face`, clockwise or not
static inline s32 __stdcall RotateFace_491F10(s32* face, bool* clockwise)
{
    switch (*face)
    {
        case 1:
            return *clockwise ? 4 : 3;
        case 3:
            return *clockwise ? 1 : 2;
        case 2:
            return *clockwise ? 3 : 4;
        case 4:
            return *clockwise ? 2 : 1;
        default:
            return 2;
    }
}

// 9.6f 0x492400
inline void Char_B4::SetTurnTarget_492400(Ang16 target_rotation)
{
    field_10_char_state = 25;
    field_14_target_rotation = target_rotation;
    field_46_timer = 255;
}

MATCH_FUNC(0x54c090)
void Char_B4::sub_54C090()
{

    s32 AngleFace_4F78F0 = Ang16::GetAngleFace_4F78F0(field_40_rotation);
    if (!CanMoveOntoSlope_54C1A0(AngleFace_4F78F0))
    {
        u8 bPastHalf;
        s32 new_dir;
        switch (AngleFace_4F78F0)
        {
            case 1:
                bPastHalf = field_40_rotation > kAng180_6FD8E8;
                break;
            case 3:
                bPastHalf = field_40_rotation > word_6FDB3C;
                break;
            case 2:
                bPastHalf = field_40_rotation > kAng90_6FDA64;
                break;
            case 4:
                bPastHalf = field_40_rotation > kAng270_6FD904;
                break;
        }

        switch (AngleFace_4F78F0)
        {
            case 1:
                new_dir = bPastHalf ? 4 : 3;
                break;
            case 3:
                new_dir = bPastHalf ? 1 : 2;
                break;
            case 2:
                new_dir = bPastHalf ? 3 : 4;
                break;
            case 4:
                new_dir = bPastHalf ? 2 : 1;
                break;
            default:
                new_dir = 2;
                break;
        }
        AngleFace_4F78F0 = new_dir;
    }

    field_40_rotation = ReturnAngleFromRoadDirection_4F7940(&AngleFace_4F78F0);

    if (field_10_char_state != 15)
    {
        this->field_10_char_state = 1;
    }
    this->field_46_timer = 100;
}

MATCH_FUNC(0x54c1a0)
char_type Char_B4::CanMoveOntoSlope_54C1A0(s32 path_direction)
{
    u8 slope_type = 0;
    char_type bUnknown = 0;
    Fix16 x_fp = field_80_sprite_ptr->field_14_xy.x;
    Fix16 y_fp = field_80_sprite_ptr->field_14_xy.y;
    Fix16 zpos = field_80_sprite_ptr->field_1C_zpos;
    s32 tmpZ = (zpos.ToInt()) - 1;
    if (field_10_char_state == 15 || field_8_ped_state_1 == ped_state_1::dead_9 || field_7C_pPed->IsField238_45EDE0(2) ||
        field_7C_pPed->get_occupation_403980() == ped_ocupation_enum::drone)
    {
        bUnknown = 1;
    }
    if ((field_58_flags & 1) == 1)
    {
        tmpZ = zpos.ToInt();
    }

    s32 x = x_fp.ToInt();
    s32 y = y_fp.ToInt();
    s32 z = zpos.ToInt();
    u8 bUnknown_2;
    if (gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(x, y, z, path_direction, &bUnknown_2, 0))
    {
        return 0;
    }


    switch (path_direction)
    {
        case path_direction::up_1:
            slope_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x_fp.ToInt(), y_fp.ToInt() - 1, tmpZ);
            break;
        case path_direction::right_3:
            slope_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x_fp.ToInt() + 1, y_fp.ToInt(), tmpZ);
            break;
        case path_direction::down_2:
            slope_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x_fp.ToInt(), y_fp.ToInt() + 1, tmpZ);
            break;
        case path_direction::left_4:
            slope_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x_fp.ToInt() - 1, y_fp.ToInt(), tmpZ);
            break;
        default:
            break;
    }

    Fix16 zpos_frac;
    char_type result;

    switch (slope_type)
    {
        case AIR:
            if ((field_58_flags & 1) != 1)
            {
                return 0;
            }
            zpos_frac = zpos.GetFracValue();

            if (zpos_frac < k_dword_6FD8DC)
            {
                field_58_flags &= ~1;
                result = CanMoveOntoSlope_54C1A0(path_direction);
                field_58_flags |= 1;
                return result;
            }

            if (zpos_frac <= kFP16Half_6FD8E4)
            {
                return 0;
            }

            field_58_flags &= ~1;
            field_80_sprite_ptr->field_1C_zpos++;
            result = CanMoveOntoSlope_54C1A0(path_direction);
            field_80_sprite_ptr->field_1C_zpos--;
            field_58_flags |= 1u;
            break;
        case ROAD:
        case FIELD:
            if (bUnknown)
            {
                return true;
            }
            return false;
        case PAVEMENT:
        case 4: // ???
            return 1;
        default:
            return 0;
    }
    return result;
}

// 9.6f 0x495470
MATCH_FUNC(0x54c3e0)
void Char_B4::sub_54C3E0()
{
    bool cw_free = false;
    s32 face = Ang16::GetAngleFace_4F78F0(field_40_rotation);
    if (!CanMoveOntoSlope_54C1A0(face))
    {
        bool clockwise = true;
        s32 cw_face = RotateFace_491F10(&face, &clockwise);
        if (CanMoveOntoSlope_54C1A0(cw_face) == 1)
        {
            cw_free = true;
        }

        clockwise = false;
        s32 ccw_face = RotateFace_491F10(&face, &clockwise);
        if (CanMoveOntoSlope_54C1A0(ccw_face) == 1)
        {
            if (cw_free == 1)
            {
                if (!(gCharB4_UpdateCounter_6FDB48 % 2))
                {
                    SetTurnTarget_492400(ReturnAngleFromRoadDirection_4F7940(&ccw_face));
                }
                else
                {
                    SetTurnTarget_492400(ReturnAngleFromRoadDirection_4F7940(&cw_face));
                }
            }
            else
            {
                SetTurnTarget_492400(ReturnAngleFromRoadDirection_4F7940(&ccw_face));
            }
        }
        else if (cw_free == 1)
        {
            SetTurnTarget_492400(ReturnAngleFromRoadDirection_4F7940(&cw_face));
        }
    }
}

// 9.6f 0x495540
MATCH_FUNC(0x54c500)
char_type Char_B4::CanMoveToTile_54C500(char_type x, char_type y)
{
    Fix16 tx = field_80_sprite_ptr->field_14_xy.x;
    Fix16 ty = field_80_sprite_ptr->field_14_xy.y;
    char_type dx = x - tx.ToInt();
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
        return (dx == -1) ? Char_B4::CanMoveOntoSlope_54C1A0(4) : Char_B4::CanMoveOntoSlope_54C1A0(3);
    }

    return (dy == -1) ? Char_B4::CanMoveOntoSlope_54C1A0(1) : Char_B4::CanMoveOntoSlope_54C1A0(2);
}

MATCH_FUNC(0x54c580)
void Char_B4::SelectRandomIdleBehavior_54C580()
{
    if (!this->field_46_timer)
    {
        switch (gCharB4_UpdateCounter_6FDB48)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            {
                this->field_10_char_state = 1;
                this->field_6C_animation_state = 0;
                this->field_46_timer = gRng_6F6784.get_int_4F7AE0(400);
                break;
            }
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            {
                this->field_10_char_state = 3;
                this->field_6C_animation_state = 0;
                this->field_46_timer = gRng_6F6784.get_int_4F7AE0(200);
                break;
            }
            case 15:
            {
                this->field_10_char_state = 4;
                this->field_6C_animation_state = 1;
                this->field_46_timer = gRng_6F6784.get_int_4F7AE0(200);
                break;
            }
            case 20:
            {
                this->field_10_char_state = 7;
                this->field_6C_animation_state = 2;
                this->field_46_timer = gRng_6F6784.get_int_4F7AE0(200);
                break;
            }
            default:
                break;
        }
    }

    if (--this->field_46_timer == 255 && this->field_10_char_state != 25)
    {
        this->field_46_timer = 0;
    }
}

MATCH_FUNC(0x54c6c0)
void Char_B4::ApplyRandomRotationJitter_54C6C0()
{
    if (gRng_6F6784.get_int_4F7AE0(32) > 22)
    {
        Ang16 old_angle = field_42_rotation_jitter;
        field_42_rotation_jitter = Ang16::Fix16_To_Ang16_inlined_40F540(k_dword_6FD868 * (Fix16(gRng_6F6784.get_int_4F7AE0(16)) - kFP16Eight_6FDA08)); // INLINED_MODE required
        if (old_angle > kAng180_6FD936 && field_42_rotation_jitter > kAng180_6FD936)
        {
            field_42_rotation_jitter = -field_42_rotation_jitter;
        }
        if (old_angle < kAng180_6FD936 && field_42_rotation_jitter < kAng180_6FD936)
        {
            field_42_rotation_jitter = -field_42_rotation_jitter;
        }
        AddAngle_4928A0(field_42_rotation_jitter);
    }
}

MATCH_FUNC(0x54c900)
void Char_B4::TickMovementStateMachine_54C900()
{
    switch (this->field_10_char_state)
    {
        case 1:
            ApplyRandomRotationJitter_54C6C0();
            this->field_38_velocity = k_dword_6FD7B8;
            byte_6FDB51 = 1;
            byte_6FDB52 = 1;
            byte_6FDB53 = 0;
            break;

        case 3:
            if (this->field_46_timer)
            {
                this->field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                ApplyRandomRotationJitter_54C6C0();
            }
            else
            {
                this->field_38_velocity = k_dword_6FD7B8;
                this->field_10_char_state = 1;
                this->field_6C_animation_state = 0;
            }
            byte_6FDB53 = 0;
            byte_6FDB51 = 1;
            byte_6FDB52 = 1;
            break;

        case 4:
            if (this->field_46_timer)
            {
                this->field_38_velocity = k_dword_6FD7CC;
                ApplyRandomRotationJitter_54C6C0();
            }
            else
            {
                this->field_38_velocity = k_dword_6FD7B8;
                this->field_10_char_state = 1;
                this->field_6C_animation_state = 0;
            }
            byte_6FDB53 = 0;
            byte_6FDB51 = 1;
            byte_6FDB52 = 1;
            break;

        case 7:
            if (this->field_46_timer)
            {
                this->field_38_velocity = kZeroVelocity_6FD7C0;
                this->field_6C_animation_state = 2;
                this->field_68_animation_frame = 0;
            }
            else
            {
                this->field_6C_animation_state = 0;
                this->field_68_animation_frame = 0;
                this->field_10_char_state = 1;
                this->field_38_velocity = k_dword_6FD7B8;
            }
            byte_6FDB53 = 0;
            byte_6FDB51 = 1;
            byte_6FDB52 = 1;
            break;

        case 8:
        case 9:
            if (!this->field_46_timer)
            {
                this->field_46_timer = 100;
                field_40_rotation.SnapToAng4_405640();
                this->field_10_char_state = 1;
                this->field_6C_animation_state = 0;
            }
            byte_6FDB52 = 0;
            byte_6FDB53 = 0;
            byte_6FDB51 = 1;
            break;

        case 25:
            TurnTowardsAngle_54CAE0();
            if (ComputeShortestAngleDelta_4056C0(this->field_40_rotation, this->field_14_target_rotation) < k_dword_6FD892)
            {
                this->field_38_velocity = k_dword_6FD7B8;
                this->field_10_char_state = 1;
                this->field_6C_animation_state = 0;
                this->field_46_timer = 0;
            }
            byte_6FDB51 = 0;
            byte_6FDB52 = 0;
            byte_6FDB53 = 0;
            break;

        case 36:
            this->field_10_char_state = 1;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x54cae0)
void Char_B4::TurnTowardsAngle_54CAE0()
{
    // Each half jumps into the other half's add block (shared blocks in the original layout),
    // which VC6 only reproduces with these gotos.
    if (field_14_target_rotation > field_40_rotation)
    {
        if (field_14_target_rotation - field_40_rotation <= kAng180_6FD920)
        {
            goto add_6FDA54;
        }
    add_6FD9D8:
        this->field_40_rotation += dword_6FD9D8;
    }
    else
    {
        if (field_40_rotation - field_14_target_rotation <= kAng180_6FD920)
        {
            goto add_6FD9D8;
        }
    add_6FDA54:
        this->field_40_rotation += word_6FDA54;
    }
}

// https://decomp.me/scratch/RGvHW
MATCH_FUNC(0x54cc40)
void Char_B4::ApplyMovement_54CC40()
{

    Fix16 xpos;
    Fix16 ypos;

    u8 octant = field_40_rotation.GetOctant_4056A0();

    switch (gCharB4_PathDirection_623F44)
    {
        case path_direction::up_1:
            switch (octant)
            {
                case 4:
                case 5:
                case 6:
                case 7:
                    PolarToCartesian_OutOfLineMul(kAng270_6FD904, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                    if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng270_6FD904, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(word_6FDB3C, field_38_velocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos_6FD7F8;
                        ypos += gCharB4_Saved_Ypos_6FD800;
                        gAng16_AngleOfCollision_6FD808 = word_6FDB3C;
                        if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(word_6FDB3C, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                            field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                     field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                     field_80_sprite_ptr->field_1C_zpos);
                        }
                    }
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                    PolarToCartesian_OutOfLineMul(kAng90_6FDA64, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                    if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng90_6FDA64, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(word_6FDB3C, field_38_velocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos_6FD7F8;
                        ypos += gCharB4_Saved_Ypos_6FD800;
                        gAng16_AngleOfCollision_6FD808 = word_6FDB3C;
                        if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(word_6FDB3C, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                            field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                     field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                     field_80_sprite_ptr->field_1C_zpos);
                        }
                    }
                    break;
                default:
                    return;
            }
            break;

        case path_direction::right_3:
            switch (octant)
            {
                case 2:
                case 3:
                case 4:
                case 5:
                    PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                    if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    break;
                case 0:
                case 1:
                case 6:
                case 7:
                    PolarToCartesian_OutOfLineMul(word_6FDB3C, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = word_6FDB3C;
                    if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(word_6FDB3C, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    break;
                default:
                    return;
            }
            break;

        case path_direction::down_2:
            switch (octant)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                    PolarToCartesian_OutOfLineMul(kAng90_6FDA64, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                    if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng90_6FDA64, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, field_38_velocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos_6FD7F8;
                        ypos += gCharB4_Saved_Ypos_6FD800;
                        gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                        if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                            field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                     field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                     field_80_sprite_ptr->field_1C_zpos);
                        }
                    }
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                    PolarToCartesian_OutOfLineMul(kAng270_6FD904, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                    if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng270_6FD904, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, field_38_velocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos_6FD7F8;
                        ypos += gCharB4_Saved_Ypos_6FD800;
                        gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                        if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                            field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                     field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                     field_80_sprite_ptr->field_1C_zpos);
                        }
                    }
                    break;

                default:
                    return;
            }
            break;

        case path_direction::left_4:
            switch (octant)
            {
                case 2:
                case 3:
                case 4:
                case 5:
                    PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                    if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng180_6FD8E8, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_451950(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    break;
                case 0:
                case 1:
                case 6:
                case 7:
                    Ang16::PolarToCartesian_451730(word_6FDB3C, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = word_6FDB3C;
                    if (Char_B4::CanStepDiagonal_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        // Out-of-line operators (inline budget)
                        Ang16::PolarToCartesian_451730(word_6FDB3C, field_38_velocity.Divide_436A20(gFix16_Two_6FD9EC), xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_451950(field_80_sprite_ptr->field_14_xy.x.Add_408660(xpos),
                                                                 field_80_sprite_ptr->field_14_xy.y.Add_408660(ypos),
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    break;

                default:
                    return;
            }
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x54dd70)
void Char_B4::sub_54DD70()
{
    if (this->field_8_ped_state_1 != ped_state_1::dead_9 && this->field_10_char_state != Char_B4_state::Jumping_15)
    {
        CheckAndHandleCollisions_5459C0();

        if (field_7C_pPed->GetBit11_433CA0() && field_7C_pPed->field_21C_bf.b9)
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
            if (field_38_velocity != kZeroVelocity_6FD7C0)
            {
                this->field_6C_animation_state = field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC;
            }
            else
            {
                this->field_6C_animation_state = 2;
            }
        }
    }
}

// Inline helper: written out in the caller, VC6 lays the out-of-range `return false` inline and
// merges the later `return false` into it. As an inline returning true/false it keeps the original's
// inline `return true` and one shared `return false` at the end.
static inline bool IsBlockTypeInRange_1_4(u8 block_type)
{
    if (block_type > 0 && block_type <= 4)
    {
        return true;
    }
    return false;
}

// https://decomp.me/scratch/56qkT
WIP_FUNC(0x54ddf0)
void Char_B4::state_0_54DDF0()
{
    WIP_IMPLEMENTED;
    char v88;
    char v89;
    u8 v91;

    byte_6FDB51 = 1;
    byte_6FDB52 = 1;
    byte_6FDB53 = 0;
    byte_6FDB54 = 0;
    this->field_4A = 500;
    v89 = 1;
    v88 = 0;
    u8 v87 = 1;
    Ang16 v93(0);
    Ang16 v95(0); //v95 = 0;


    u8 block_type;
    gmp_block_info* pBlock;

    if ((field_58_flags & 1) == 0 && gCharB4_Saved_Zpos_6FD7FC.GetFracValue() == kFP16Zero_6FD9E4)
    {
        //goto LABEL_9;
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        pBlock = gMap_0x370_6F6268->get_block_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
    }
    else
    {
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());
        pBlock = gMap_0x370_6F6268->get_block_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());

        if (pBlock == NULL || block_type == 0)
        {
            //LABEL_9:
            block_type =
                gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
            pBlock = gMap_0x370_6F6268->get_block_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        }
    }

    if (pBlock)
    {
        // bit 0: standing on a slope
        field_58_flags_bf.b0 = get_slope_bits(pBlock->field_B_slope_type) != 0 && get_slope_bits(pBlock->field_B_slope_type) != 0xFC;
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
                        field_7C_pPed->field_264_killer_id_timer = 50;
                    }
                }
            }
        }
    }
    if ((u8)Char_B4::IsOnWater_545570())
    {
        field_7C_pPed->PutOutFire();
        Char_B4::DrownPed_5459E0();
        return;
    }

    Fix16 ret1;
    Fix16 ret2;

    if (block_type == 0 && (field_58_flags & 1) == 0) // or !pBlock
    {
        // 10.5 has a separate copy for the non-player case, so this is a nested if, not one && chain
        if (field_7C_pPed->field_15C_player)
        {
            if (field_7C_pPed->get_fieldC_45C9B0() < kFP16Zero_6FD9E4 || (field_58_flags & 8) != 0)
            {
                Ang16::PolarToCartesian_41FC20(field_40_rotation + kAng180_6FD936, dword_6FDAC8, ret1, ret2);
            }
            else
            {
                Ang16::PolarToCartesian_41FC20(field_40_rotation, dword_6FDAC8, ret1, ret2);
            }
        }
        else
        {
            Ang16::PolarToCartesian_41FC20(field_40_rotation, dword_6FDAC8, ret1, ret2);
        }
        // ......

        ret1 += gCharB4_Saved_Xpos_6FD7F8;
        ret2 += gCharB4_Saved_Ypos_6FD800;

        gmp_block_info* pAhead = gMap_0x370_6F6268->get_block_4DFE10(ret1.ToInt(),
                                                                ret2.ToInt(),
                                                                gCharB4_Saved_Zpos_6FD7FC.GetFracValue() == kFP16Zero_6FD9E4 ?
                                                                    gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1 :
                                                                    gCharB4_Saved_Zpos_6FD7FC.ToInt());
        // Written out: the GetBlockTypeAtCoord_420420 inline puts the null case after the call here
        block_type = !pAhead ? 0 : pAhead->field_B_slope_type & 3;
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
                    if (field_7C_pPed->get_fieldC_45C9B0() < kFP16Zero_6FD9E4 || (field_58_flags & 8) != 0)
                    {
                        field_38_velocity = -field_38_velocity;
                    }
                }
                field_90_fall_speed = field_38_velocity;
                field_94_fall_z_speed = kFP16Zero_6FD9E4;
                if (field_38_velocity == kZeroVelocity_6FD7C0)
                {
                    field_16_state_init_pending = 1;
                }
                Char_B4::state_8_5520A0();
                return;
            }
            // A jumping ped only starts falling once the jump is over
            if (this->field_68_animation_frame == 7 || this->field_7C_pPed->field_21C_bf.b11 != 0)
            {
                this->field_10_char_state = 1;
                field_7C_pPed->ClearBit11_403A40();
                goto LABEL_44;
            }
        }
    }

    field_44_block_type = block_type;
    Fix16 radius;
    if (field_7C_pPed->IsField238_45EDE0(2) == true)
    {
        radius = dword_6FDAC8;
        if (this->field_10_char_state)
        {
            this->field_46_timer = 9999;
        }
        field_40_rotation += field_7C_pPed->get_field8_45C900();

        if (this->field_10_char_state == Char_B4_state::Jumping_15)
        {
            field_7C_pPed->ClearBit11_403A40();
            if (this->field_6C_animation_state != 5)
            {
                this->field_6C_animation_state = 5;
                this->field_68_animation_frame = 0; // line 4b2
            }
            if (field_7C_pPed->get_fieldC_45C9B0() > kFP16Zero_6FD9E4)
            {
                if (field_38_velocity == kZeroVelocity_6FD7C0)
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
            if (field_7C_pPed->get_fieldC_45C9B0() > kFP16Zero_6FD9E4) // line 53c
            {
                if (field_38_velocity == kZeroVelocity_6FD7C0)
                {
                    this->field_38_velocity = this->field_3C_run_or_jump_speed;
                    field_58_flags &= ~8;
                }
                else
                {
                    if (field_38_velocity < field_3C_run_or_jump_speed)
                    {
                        this->field_38_velocity += dword_6FD99C;
                        field_58_flags &= ~8;
                    }
                    else
                    {
                        if (field_38_velocity > field_3C_run_or_jump_speed)
                        {
                            this->field_38_velocity -= dword_6FD99C;
                        }
                        field_58_flags &= ~8;
                    }
                }
            }
            else
            {
                if (field_7C_pPed->get_fieldC_45C9B0() < kFP16Zero_6FD9E4)
                {
                    v95 = this->field_40_rotation;
                    this->field_58_flags = this->field_58_flags | 8;
                    field_40_rotation += kAng180_6FD936;

                    if (field_38_velocity == kZeroVelocity_6FD7C0)
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
                        this->field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                    }
                    else
                    {
                        this->field_38_velocity = kZeroVelocity_6FD7C0;
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
            this->field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
        }
    }
    else
    {
        radius = dword_6FDAC8;
        if (this->field_69_is_colliding_with_sprite || this->field_10_char_state == Char_B4_state::Jumping_15) // line 6b1
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
                v89 = 0;
                byte_6FDB51 = 0;
                byte_6FDB52 = 0;
                v95 = field_40_rotation;
                this->field_40_rotation = this->field_74;
                this->field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                v87 = 0;
            }
            if (this->field_38_velocity < kZeroVelocity_6FD7C0)
            {
                // line 367 on 9.6f IDA
                Ang16 rotation = field_40_rotation;
                v95 = rotation;
                rotation += kAng180_6FD936;
                field_40_rotation = rotation;
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
                            field_7C_pPed->SetObjective2_463830(17, 9999);
                        }
                        break;
                    default:
                        if (this->field_C_ped_state_2 == 0 && v89)
                        {
                            Char_B4::SelectRandomIdleBehavior_54C580();
                            Char_B4::TickMovementStateMachine_54C900();
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
    if (field_38_velocity == kFP16Zero_6FD9E4)
    {
        field_58_flags &= ~8;
    }

    // line 401 on 9.6f IDA
    Ang16::PolarToCartesian_41FC20(field_40_rotation, radius, ret1, ret2);

    ret1 += gCharB4_Saved_Xpos_6FD7F8;
    ret2 += gCharB4_Saved_Ypos_6FD800;
    ret1 += field_4C_conveyor_dx;
    ret2 += field_50_conveyor_dy;

    if (!Char_B4::IsNearTileCentre_5532C0())
    {
        if (field_7C_pPed->IsField238_45EDE0(2) == true)
        {
            v88 = 1;
            gAng16_AngleOfCollision_6FD808 = field_40_rotation;
            u8 v90 = Char_B4::CanStepDiagonal_54EF60(ret1.ToInt(), ret2.ToInt());
            if (gMap_0x370_6F6268->sub_4E0110() != 1 && !v90)
            {
                Char_B4::ApplyMovement_54CC40();
                v88 = 0;
                goto LABEL_152;
            }
        }
        else
        {
            v91 = Char_B4::CanMoveToTile_54C500(ret1.ToInt(), ret2.ToInt());

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
                    v93 += kAng90_6FD8A2;
                    Fix16 xpos_3;
                    Fix16 ypos_3;
                    Ang16::PolarToCartesian_41FC20(v93, kFP16Half_6FD9B4, xpos_3, ypos_3);

                    xpos_3 += gCharB4_Saved_Xpos_6FD7F8;
                    ypos_3 += gCharB4_Saved_Ypos_6FD800;

                    if (Char_B4::CanMoveToTile_54C500(xpos_3.ToInt(), ypos_3.ToInt()) == 1)
                    {
                        v93 = field_40_rotation;
                        v93.SnapToAng4_405640();
                        v93 += kAng270_6FD94C;

                        Fix16 xpos_4;
                        Fix16 ypos_4;

                        Ang16::PolarToCartesian_41FC20(v93, kFP16Half_6FD9B4, xpos_4, ypos_4);

                        xpos_4 += gCharB4_Saved_Xpos_6FD7F8;
                        ypos_4 += gCharB4_Saved_Ypos_6FD800;

                        if (!Char_B4::CanMoveToTile_54C500(xpos_4.ToInt(), ypos_4.ToInt()))
                        {
                            sub_4923D0();
                        }
                    }
                    else
                    {
                        sub_4923A0();
                    }
                }
            }
            else
            {
                Char_B4::sub_54C090();
                goto LABEL_152;
            }
        }
    }

    field_80_sprite_ptr = this->field_80_sprite_ptr;

    Ang16::PolarToCartesian_41FC20(field_40_rotation, field_38_velocity, ret1, ret2);
    ret1 += field_4C_conveyor_dx;
    ret2 += field_50_conveyor_dy;
    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + ret1,
                                             field_80_sprite_ptr->field_14_xy.y + ret2,
                                             field_80_sprite_ptr->field_1C_zpos);

LABEL_152:
    field_4C_conveyor_dx = kFP16Zero_6FD9E4;
    field_50_conveyor_dy = kFP16Zero_6FD9E4;
    if (v88 == 1 || (field_58_flags & 1) == 1)
    {
        byte_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                  field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                  field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);
        Char_B4::ManageZCoordAndSlopes_548590();
    }

    Char_B4::DispatchCollision_548670(byte_6FDB56);

    if (!field_7C_pPed->IsField238_45EDE0(2))
    {
        if (field_69_is_colliding_with_sprite)
        {
            if (Char_B4::ContinueMovementAfterCollision_54B8F0() == 1)
            {
                // The sprite position before the reset: CanReachTile tests it and it is put back afterwards
                Fix16 old_xpos = field_80_sprite_ptr->field_14_xy.x;
                Fix16 old_ypos = field_80_sprite_ptr->field_14_xy.y;
                char_type tile_x = old_xpos.ToInt();
                char_type tile_y = old_ypos.ToInt();
                if ((char_type)gCharB4_Saved_Xpos_6FD7F8.ToInt() != tile_x || (char_type)gCharB4_Saved_Ypos_6FD800.ToInt() != tile_y)
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);

                    field_45_slope_gradient_direction = gCharB4_Saved_SlopeGradDir_6FD7B0.ToInt();
                    if (!Char_B4::CanReachTile_550090(tile_x, tile_y))
                    {
                        field_69_is_colliding_with_sprite = 0;
                        field_5C = 10;
                    }
                    else
                    {
                        // Line 507 on 9.6f IDA
                        field_80_sprite_ptr->set_xyz_lazy_420600(old_xpos, old_ypos, gCharB4_Saved_Zpos_6FD7FC);
                        byte_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                    field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                    (field_80_sprite_ptr->field_1C_zpos - kFP16One_6FD9E8).ToInt());
                        Char_B4::ManageZCoordAndSlopes_548590();
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

// https://decomp.me/scratch/Zk9Eh
MATCH_FUNC(0x54ecb0)
bool Char_B4::CanStepForwardWithRegionCheck_54ECB0(s32 direction)
{
    u8 u8_unk;

    Fix16 xpos = field_80_sprite_ptr->field_14_xy.x;
    Fix16 ypos = field_80_sprite_ptr->field_14_xy.y;
    Fix16 zpos = field_80_sprite_ptr->field_1C_zpos;

    s32 new_zpos = (zpos).ToInt() - 1;

    if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(xpos.ToInt(), ypos.ToInt(), (zpos - kFP16One_6FD9E8).ToInt()))
    {
        new_zpos = zpos.ToInt();
    }
    if (gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(xpos.ToInt(), ypos.ToInt(), zpos.ToInt(), direction, &u8_unk, 0))
    {
        gCharB4_PathDirection_623F44 = direction;
        return 0;
    }
    if (byte_6FDB57)
    {
        field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_StepXpos_6FD8B8, gCharB4_StepYpos_6FD8BC, zpos);
        Char_B4::ManageZCoordAndSlopes_548590();
        if (field_80_sprite_ptr->CheckSpriteMovementRegion_5A2500())
        {
            gCharB4_PathDirection_623F44 = direction;

            field_80_sprite_ptr->set_xyz_lazy_420600(xpos, ypos, zpos);
            gMap_0x370_6F6268->Clear_F36E_492130();
            return 0;
        }
        field_80_sprite_ptr->set_xyz_lazy_420600(xpos, ypos, zpos);
        Char_B4::ManageZCoordAndSlopes_548590();
    }

    u8 block_type;

    switch (direction)
    {
        case 1:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt(), ypos.ToInt() - 1, new_zpos);
            break;
        case 3:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt() + 1, ypos.ToInt(), new_zpos);
            break;
        case 2:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt(), ypos.ToInt() + 1, new_zpos);
            break;
        case 4:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt() - 1, ypos.ToInt(), new_zpos);
            break;
        default:
            block_type = AIR;
            break;
    }

    if (block_type != AIR)
    {
        return IsBlockTypeInRange_1_4(block_type);
    }

    if ((field_58_flags & 1) == 1)
    {
        if (zpos.GetFracValue() < dword_6FD91C)
        {
            field_58_flags &= ~1u;
            bool result = Char_B4::CanStepForward_54FEC0(direction);
            field_58_flags |= 1u;
            return result;
        }
        if (!field_7C_pPed->IsField238_45EDE0(2))
        {
            return false;
        }
    }
    return true;
}

// https://decomp.me/scratch/Ub1EN
WIP_FUNC(0x54ef60)
bool Char_B4::CanStepDiagonal_54EF60(char_type a2, char_type a3)
{
    WIP_IMPLEMENTED;
    bool bIsNearXposBlockBoundary = true;
    bool bIsNearYposBlockBoundary = true;

    Fix16 sprite_xpos = field_80_sprite_ptr->field_14_xy.x;
    Fix16 sprite_ypos = field_80_sprite_ptr->field_14_xy.y;
    Fix16 sprite_zpos = field_80_sprite_ptr->field_1C_zpos;

    u8 old_f45 = field_45_slope_gradient_direction;
    byte_6FDB57 = 1;
    Fix16 xpos, ypos;
    Ang16::PolarToCartesian_41FC20(gAng16_AngleOfCollision_6FD808, gFP16_CollisionCheckRadius_6FDACC, xpos, ypos);
    gCharB4_StepXpos_6FD8B8 = gCharB4_Saved_Xpos_6FD7F8 + xpos;
    gCharB4_StepYpos_6FD8BC = gCharB4_Saved_Ypos_6FD800 + ypos;

    Fix16 collision_offset = gCharB4_WorldCollisionOffset_6FD8D8 / gFix16_Two_6FD9EC;

    // Get the x diff of the neareast block on left/right side, if it's very close to them
    // Test if it is near right block
    s8 x_diff = (gCharB4_StepXpos_6FD8B8 + collision_offset).ToInt() - (sprite_xpos).ToInt();

    if (x_diff == 0)
    {
        // Test if it is near left block
        x_diff = (gCharB4_StepXpos_6FD8B8 - collision_offset).ToInt() - (sprite_xpos).ToInt();
        if (x_diff == 0)
        {
            bIsNearXposBlockBoundary = false;
        }
    }

    // Get the y diff of the neareast block on north/south side, if it's very close to them
    // Test if it is near south block
    s8 y_diff = (gCharB4_StepYpos_6FD8BC + collision_offset).ToInt() - (sprite_ypos).ToInt();

    if (y_diff == 0)
    {
        // Test if it is near north block
        y_diff = (gCharB4_StepYpos_6FD8BC - collision_offset).ToInt() - (sprite_ypos).ToInt();
        if (y_diff == 0)
        {
            bIsNearYposBlockBoundary = false;
        }
    }

    if (!bIsNearXposBlockBoundary && !bIsNearYposBlockBoundary)
    {
        x_diff = a2 - field_80_sprite_ptr->field_14_xy.x.ToInt();
        y_diff = a3 - field_80_sprite_ptr->field_14_xy.y.ToInt();
        if (x_diff == 0 && y_diff == 0)
        {
            return true;
        }
    }

    if (x_diff != 0 && y_diff != 0)
    {
        // Diagonal step: try both axis neighbours of the target block
        bool bCanStep;
        if (x_diff == -1)
        {
            bCanStep = true;
            if (y_diff == -1)
            {
                field_80_sprite_ptr->field_14_xy.x.subtract_one_491F00();
                if (field_58_flags & 1)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->sub_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::up_1;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y.subtract_one_491F00();
                if (field_58_flags & 1)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->sub_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::left_4;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y = sprite_ypos;
                field_80_sprite_ptr->field_1C_zpos = sprite_zpos;

                if (!bCanStep)
                {
                    gCharB4_StepXpos_6FD8B8 = gCharB4_Saved_Xpos_6FD7F8;
                    gCharB4_StepYpos_6FD8BC = gCharB4_Saved_Ypos_6FD800;
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::left_4;
                    }
                    else
                    {
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->sub_4E0110())
                        {
                            bCanStep = false;
                        }
                        else
                        {
                            gCharB4_PathDirection_623F44 = path_direction::left_4;
                        }
                    }
                }
                else
                {
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::left_4;
                    }
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                    }
                }
                field_45_slope_gradient_direction = old_f45;
            }
            else
            {
                field_80_sprite_ptr->field_14_xy.x.subtract_one_491F00();
                if (field_58_flags & 1)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->sub_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::down_2;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y.add_one_491EF0();
                if (field_58_flags & 1)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->sub_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::left_4;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y = sprite_ypos;
                field_80_sprite_ptr->field_1C_zpos = sprite_zpos;

                if (!bCanStep)
                {
                    gCharB4_StepXpos_6FD8B8 = gCharB4_Saved_Xpos_6FD7F8;
                    gCharB4_StepYpos_6FD8BC = gCharB4_Saved_Ypos_6FD800;
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::left_4;
                    }
                    else
                    {
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->sub_4E0110())
                        {
                            bCanStep = false;
                        }
                        else
                        {
                            gCharB4_PathDirection_623F44 = path_direction::left_4;
                        }
                    }
                }
                else
                {
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::left_4;
                    }
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                    }
                }
                field_45_slope_gradient_direction = old_f45;
            }
        }
        else
        {
            bCanStep = true;
            if (y_diff == -1)
            {
                field_80_sprite_ptr->field_14_xy.x.add_one_491EF0();
                if (field_58_flags & 1)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->sub_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::up_1;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y.subtract_one_491F00();
                if (field_58_flags & 1)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->sub_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::right_3;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y = sprite_ypos;
                field_80_sprite_ptr->field_1C_zpos = sprite_zpos;

                if (!bCanStep)
                {
                    gCharB4_StepXpos_6FD8B8 = gCharB4_Saved_Xpos_6FD7F8;
                    gCharB4_StepYpos_6FD8BC = gCharB4_Saved_Ypos_6FD800;
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    else
                    {
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->sub_4E0110())
                        {
                            bCanStep = false;
                        }
                        else
                        {
                            gCharB4_PathDirection_623F44 = path_direction::right_3;
                        }
                    }
                }
                else
                {
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                        bCanStep = false;
                    }
                }
                field_45_slope_gradient_direction = old_f45;
            }
            else
            {
                field_80_sprite_ptr->field_14_xy.x.add_one_491EF0();
                if (field_58_flags & 1)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->sub_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::down_2;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                field_80_sprite_ptr->field_14_xy.y.add_one_491EF0();
                field_80_sprite_ptr->field_1C_zpos =
                    gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->sub_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::right_3;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y = sprite_ypos;
                field_80_sprite_ptr->field_1C_zpos = sprite_zpos;

                if (!bCanStep)
                {
                    gCharB4_StepXpos_6FD8B8 = gCharB4_Saved_Xpos_6FD7F8;
                    gCharB4_StepYpos_6FD8BC = gCharB4_Saved_Ypos_6FD800;
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    else
                    {
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->sub_4E0110())
                        {
                            bCanStep = false;
                        }
                        else
                        {
                            gCharB4_PathDirection_623F44 = path_direction::right_3;
                        }
                    }
                }
                else
                {
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->sub_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                    }
                }
                field_45_slope_gradient_direction = old_f45;
            }
        }
        return bCanStep;
    }

    // Straight step along one axis
    if (x_diff != 0)
    {
        if (x_diff == -1)
        {
            return CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4);
        }
        return CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3);
    }
    if (y_diff == -1)
    {
        return CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1);
    }
    return CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2);
}

// https://decomp.me/scratch/xc0PO
MATCH_FUNC(0x54fec0)
bool Char_B4::CanStepForward_54FEC0(s32 direction)
{
    bool result;

    Fix16 v16;
    u8 block_type;

    Fix16 xpos = field_80_sprite_ptr->field_14_xy.x;
    Fix16 ypos = field_80_sprite_ptr->field_14_xy.y;
    Fix16 field_1C_zpos = field_80_sprite_ptr->field_1C_zpos;
    s8 v18 = 0;
    s32 zpos_int = field_1C_zpos.ToInt();

    s32 v9 = zpos_int - 1;

    if ((field_58_flags & 1) == 1)
    {
        v9 = zpos_int;
    }

    if (gMap_0x370_6F6268->CanMoveOntoSlopeTile_4E0130(xpos.ToInt(), ypos.ToInt(), zpos_int, direction, (u8*)&v18, 0))
    {
        gCharB4_PathDirection_623F44 = direction;
        return false;
    }

    v9 += v18;

    if (v9 < 0)
    {
        return false;
    }

    switch (direction)
    {
        case 1:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt(), ypos.ToInt() - 1, v9);
            break;
        case 3:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt() + 1, ypos.ToInt(), v9);
            break;
        case 2:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt(), ypos.ToInt() + 1, v9);
            break;
        case 4:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt() - 1, ypos.ToInt(), v9);
            break;
        default:
            block_type = AIR;
            break;
    }

    if (block_type != AIR)
    {
        return IsBlockTypeInRange_1_4(block_type);
    }

    if ((field_58_flags & 1) == 1)
    {
        v16 = field_1C_zpos.GetFracValue();
        if (v16 < kFP16Half_6FD8E4)
        {
            field_58_flags &= ~1u;
            result = Char_B4::CanStepForward_54FEC0(direction);
            field_58_flags |= 1u;
            return result;
        }
        if (field_7C_pPed->IsField238_45EDE0(2))
        {
            return true;
        }
        if (v16 > kFP16Half_6FD8E4)
        {
            field_58_flags &= ~1u;
            field_80_sprite_ptr->field_1C_zpos += Fix16(1);
            result = Char_B4::CanStepForward_54FEC0(direction);
            field_80_sprite_ptr->field_1C_zpos -= Fix16(1);
            field_58_flags |= 1u;
            return result;
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x550090)
bool Char_B4::CanReachTile_550090(u8 xpos, u8 ypos)
{
    bool bRes_1;
    bool bRes_2;

    Fix16 original_x = field_80_sprite_ptr->field_14_xy.x;
    Fix16 original_y = field_80_sprite_ptr->field_14_xy.y;

    char_type diff_x = xpos - original_x.ToInt();
    char_type diff_y = ypos - original_y.ToInt();

    if (diff_x == 0)
    {
        if (diff_y == 0)
        {
            return true;
        }
    }

    if (diff_x)
    {
        if (diff_y)
        {
            if (diff_x == -1)
            {
                bRes_1 = true;
                if (diff_y == -1)
                {
                    field_80_sprite_ptr->field_14_xy.x.subtract_one_491F00();

                    if (!Char_B4::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y.subtract_one_491F00();
                    if (!Char_B4::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y = original_y;
                    if (!Char_B4::CanReachTile_550090((gCharB4_Saved_Xpos_6FD7F8 - kFP16One_6FD9E8).ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt()))
                    {
                        bRes_1 = false;
                    }
                    if (!Char_B4::CanReachTile_550090(gCharB4_Saved_Xpos_6FD7F8.ToInt(), (gCharB4_Saved_Ypos_6FD800 - kFP16One_6FD9E8).ToInt()))
                    {
                        bRes_1 = false;
                    }
                }
                else
                {
                    field_80_sprite_ptr->field_14_xy.x.subtract_one_491F00();

                    if (!Char_B4::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }

                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y.add_one_491EF0();
                    if (!Char_B4::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y = original_y;
                    if (!Char_B4::CanReachTile_550090((field_80_sprite_ptr->field_14_xy.x - kFP16One_6FD9E8).ToInt(),
                                                      field_80_sprite_ptr->field_14_xy.y.ToInt()))
                    {
                        bRes_1 = false;
                    }

                    if (!Char_B4::CanReachTile_550090(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                      (kFP16One_6FD9E8 + field_80_sprite_ptr->field_14_xy.y).ToInt()))
                    {
                        bRes_1 = false;
                    }
                }
            }
            else
            {
                bRes_2 = true;
                if (diff_y == -1)
                {
                    field_80_sprite_ptr->field_14_xy.x.add_one_491EF0();
                    if (!Char_B4::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_2 = false;
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y.subtract_one_491F00();
                    if (!Char_B4::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_2 = false;
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y = original_y;
                    if (!Char_B4::CanReachTile_550090((kFP16One_6FD9E8 + field_80_sprite_ptr->field_14_xy.x).ToInt(),
                                                      field_80_sprite_ptr->field_14_xy.y.ToInt()))
                    {
                        bRes_2 = false;
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                    }
                    if (!Char_B4::CanReachTile_550090(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                      (field_80_sprite_ptr->field_14_xy.y - kFP16One_6FD9E8).ToInt()))
                    {
                        bRes_2 = false;
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    return bRes_2;
                }
                else
                {
                    field_80_sprite_ptr->field_14_xy.x.add_one_491EF0();
                    if (!Char_B4::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }

                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y.add_one_491EF0();
                    if (!Char_B4::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y = original_y;
                    if (!Char_B4::CanReachTile_550090((kFP16One_6FD9E8 + field_80_sprite_ptr->field_14_xy.x).ToInt(),
                                                      field_80_sprite_ptr->field_14_xy.y.ToInt()))
                    {
                        bRes_1 = false;
                    }
                    if (!Char_B4::CanReachTile_550090(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                      (kFP16One_6FD9E8 + field_80_sprite_ptr->field_14_xy.y).ToInt()))
                    {
                        bRes_1 = false;
                    }
                }
            }
            return bRes_1;
        }
        else // diff_y = 0
        {
            if (diff_x == -1)
            {
                if (Char_B4::CanStepForward_54FEC0(path_direction::left_4))
                {
                    return true;
                }
                else
                {
                    gCharB4_PathDirection_623F44 = path_direction::left_4;
                    return false;
                }
            }
            else if (Char_B4::CanStepForward_54FEC0(path_direction::right_3))
            {
                return true;
            }
            else
            {
                gCharB4_PathDirection_623F44 = path_direction::right_3;
                return false;
            }
        }
    }
    else // diff_x = 0
    {
        if (diff_y == 0)
        {
            return true;
        }
        if (diff_y == -1)
        {
            if (Char_B4::CanStepForward_54FEC0(path_direction::up_1))
            {
                return true;
            }
            else
            {
                gCharB4_PathDirection_623F44 = path_direction::up_1;
                return false;
            }
        }
        else if (Char_B4::CanStepForward_54FEC0(path_direction::down_2))
        {
            return true;
        }
        else
        {
            gCharB4_PathDirection_623F44 = path_direction::down_2;
            return false;
        }
    }
    return false;
}

// https://decomp.me/scratch/chZqY
WIP_FUNC(0x5504f0)
void Char_B4::state_1_5504F0()
{
    WIP_IMPLEMENTED;
    Ang16 v77;
    s32 zpos;
    Ang16 v29;
    Ang16 v30;
    Ang16 v39;
    char v70;
    char v71;
    Fix16 pMaybeX_FP16;
    Fix16 pMaybeY_FP16;

    Ang16 v20;
    Ang16 v73;

    field_4A = 500;
    Ang16 angle = 0;
    field_58_flags_bf.b3 = false;
    field_58_flags_bf.b6 = false;
    byte_6FDB54 = 0;
    gCharB4_Saved_TileX_6FDAD8 = gCharB4_Saved_Xpos_6FD7F8.ToUInt8();
    gCharB4_Saved_TileY_6FDAD9 = gCharB4_Saved_Ypos_6FD800.ToUInt8();
    v71 = 0;
    v77 = 0;
    v70 = 0;
    u8 unk_xpos = gCharB4_Saved_Xpos_6FD7F8.ToUInt8();
    u8 unk_ypos = gCharB4_Saved_Ypos_6FD800.ToUInt8();
    u8 block_type;
    if (field_58_flags_bf.b0 == false && gCharB4_Saved_Zpos_6FD7FC.GetFracValue() == kFP16Zero_6FD9E4 &&
        gCharB4_Saved_Zpos_6FD7FC > kFP16Zero_6FD9E4)
    {
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                                   gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                                   gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        zpos = gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1;
    }
    else
    {
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                                   gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                                   gCharB4_Saved_Zpos_6FD7FC.ToInt());
        zpos = gCharB4_Saved_Zpos_6FD7FC.ToInt();
    }

    gmp_block_info* pBlock =
        gMap_0x370_6F6268->get_block_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), zpos);
    if (pBlock)
    {
        u8 v9 = (pBlock->field_B_slope_type & 0xFC) != 0 && (pBlock->field_B_slope_type & 0xFC) != 0xFC;
        field_58_flags ^= (field_58_flags ^ v9) & 1;
        if (gGtx_0x106C_703DD4->IsElectrifiedFloorType_491F80(pBlock->field_8_lid & 0x3FF) && field_10_char_state != 15)
        {
            if (field_7C_pPed->field_21C_bf.b27 == false)
            {
                field_7C_pPed->field_210_shock_counter += 3;
                if (field_7C_pPed->field_204_killer_id)
                {
                    if (gPedManager_6787BC->PedById(field_7C_pPed->field_204_killer_id))
                    {
                        field_7C_pPed->field_290 = 2;
                        field_7C_pPed->field_264_killer_id_timer = 50;
                    }
                }
            }
        }
    }

    if ((u8)Char_B4::IsOnWater_545570())
    {
        field_7C_pPed->PutOutFire();
        Char_B4::DrownPed_5459E0();
        return;
    }

    if (!block_type && field_58_flags_bf.b0 == false)
    {
        if (field_10_char_state != 15)
        {
            if (gMap_0x370_6F6268->HasBlockAnyArrows_492140(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                            gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                            (gCharB4_Saved_Zpos_6FD7FC - kFP16One_6FD9E8).ToInt()))
            {
                Char_B4::DoJump_5454D0();
                field_38_velocity = kZeroVelocity_6FD7C0;
                field_58_flags_bf.b6 = true;
                field_40_rotation.SnapToAng4_405640();
            }
            else
            {
                field_7C_pPed->ChangeNextPedState1_45C500(8);
                field_7C_pPed->ChangeNextPedState2_45C540(19);
                field_16_state_init_pending = 1;
                return;
            }
        }
        else
        {
            if (gMap_0x370_6F6268->HasBlockAnyArrows_492140(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                            gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                            (gCharB4_Saved_Zpos_6FD7FC - kFP16One_6FD9E8).ToInt()))
            {
                Char_B4::DoJump_5454D0();
                field_58_flags_bf.b6 = true;
                field_40_rotation.SnapToAng4_405640();
            }
        }
    }

    field_58_flags_bf.b3 = false;
    if (field_38_velocity < kFP16Zero_6FD9E4)
    {
        field_58_flags_bf.b3 = true;
        field_40_rotation = Ang16(field_40_rotation.rValue + kAng180_6FD936.rValue, (u8)0);
        field_38_velocity = -field_38_velocity;
    }
    field_44_block_type = block_type;

    if (field_58_flags_bf.b6 != 0)
    {
        v20 = field_40_rotation;
    }
    else
    {
        v20 = field_7C_pPed->get_field_130_492CC0();
    }
    v73 = v20;
    if (field_10_char_state != 10)
    {
        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            if (field_6C_animation_state != Char_Anim_state::Jumping_5)
            {
                field_6C_animation_state = Char_Anim_state::Jumping_5;
                field_68_animation_frame = 0;
                field_71_frame_delay = 4;
            }
            else
            {
                if (field_58_flags_bf.b6)
                {
                    goto LABEL_65;
                }
                if ((field_7C_pPed->field_224 & 0x10) == 0)
                {
                    v20 = field_7C_pPed->get_field_130_492CC0();
                }
                else
                {
                    v20 = field_40_rotation;
                }
                v73 = v20;
            }

            if (field_58_flags_bf.b6 == false)
            {
                if ((field_7C_pPed->field_224 & 0x10) != 0)
                {
                    if (gCharB4_DistanceToTarget_6FD80C < dword_6FD87C)
                    {
                        field_38_velocity = k_dword_6FD7B8;
                    }
                    else
                    {
                        if (gCharB4_DistanceToTarget_6FD80C < dword_6FD88C)
                        {
                            field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                        }
                        else
                        {
                            field_38_velocity = gRunOrJumpSpeed_6FD7D0;
                        }
                    }
                }
                else
                {
                    field_38_velocity = gRunOrJumpSpeed_6FD7D0;
                }
            }
        }
    }
    else
    {
        if (field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC)
        {
            field_38_velocity = k_dword_6FD7CC;
        }
        v70 = 1;
    }
LABEL_65:
    if (field_55 > 0)
    {
        field_55--;
    }

    if (field_38_velocity != kZeroVelocity_6FD7C0)
    {
        if (field_69_is_colliding_with_sprite)
        {
            v70 = 1;
            field_38_velocity = gRunOrJumpSpeed_6FD7D0;
        }

        if (field_58_flags_bf.b6 || field_7C_pPed->IsPedGoingToEnterCar_492FD0())
        {
            v70 = 1;
        }
        angle = v20;
        if (field_58_flags_bf.b7) // line 434
        {
            if (Fix16::MaxAbsDistance_42A6B0(gCharB4_Saved_Xpos_6FD7F8,
                                             gCharB4_Saved_Ypos_6FD800,
                                             kFP16Half_6FD8E4 + Fix16(field_72_next_tile_x),
                                             kFP16Half_6FD8E4 + Fix16(field_73_next_tile_y)) < kFP16Quarter_6FD828)
            {
                field_58_flags_bf.b7 = 0;
            }
            else
            {
                v73 = Fix16::atan2_fixed_405320(kFP16Half_6FD8E4 + Fix16(field_73_next_tile_y) - gCharB4_Saved_Ypos_6FD800,
                                                kFP16Half_6FD8E4 + Fix16(field_72_next_tile_x) - gCharB4_Saved_Xpos_6FD7F8);
                angle = v73;
            }
        }
        else
        {
            if (field_6A)
            {
                if (field_C_ped_state_2 == ped_state_2::Unknown_3)
                {
                    goto LABEL_82;
                }
                v29 = field_74;
                v30 = field_40_rotation;
                field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                v73 = v29;
                v77 = v30;
                v70 = 1;
            }
        }
        if (field_C_ped_state_2 == ped_state_2::Unknown_3)
        {
        LABEL_82:
            if (field_55 > 0)
            {
                if (field_58_flags_bf.b7 == false)
                {
                    v73 = field_80_sprite_ptr->field_0;
                    angle = v73;
                }
            }
            Ang16::PolarToCartesian_41FC20(v73, kFP16Half_6FD8E4, pMaybeX_FP16, pMaybeY_FP16);
        }
        else
        {
            Ang16::PolarToCartesian_41FC20(angle, kFP16Quarter_6FD828, pMaybeX_FP16, pMaybeY_FP16);
        }
        pMaybeX_FP16 += gCharB4_Saved_Xpos_6FD7F8;
        pMaybeY_FP16 += gCharB4_Saved_Ypos_6FD800;
        u8 x_u8 = pMaybeX_FP16.ToUInt8();
        u8 y_u8 = pMaybeY_FP16.ToUInt8();
        if (unk_xpos != x_u8 || unk_ypos != y_u8)
        {
            Char_B4::ManageZCoordAndSlopes_548590();

            if (!Char_B4::CanReachTile_550090(x_u8, y_u8))
            {
                field_58_flags_bf.b7 = true;
                if (field_C_ped_state_2 == ped_state_2::Unknown_3)
                {
                    Char_B4::ChooseNextMovementTile_551400();
                }
                else
                {
                    Char_B4::SelectNextTileFast_5516F0();
                }
                v73 = gAng16_AngleOfCollision_6FD808;
                angle = gAng16_AngleOfCollision_6FD808;
            }
        }
    }

    if (field_10_char_state == 28 || field_10_char_state == 29 || v70)
    {
        field_40_rotation = v73;
    }
    else
    {
        v39 = Char_B4::GetNextRotationToward_550F60(v73);
        if (Char_B4::CanStepInDirection_551350(v39) == 1)
        {
            field_40_rotation = v39;
            v71 = 1;
        }
        else if (angle != v73)
        {
            field_40_rotation = angle;
        }
        else
        {
            field_40_rotation = v73;
        }
    }

    if (field_58_flags_bf.b6)
    {
        field_38_velocity = gRunOrJumpSpeed_6FD7D0;
    }

    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + Ang16::sine_40F500(field_40_rotation) * field_38_velocity,
                                             field_80_sprite_ptr->field_14_xy.y + Ang16::cosine_40F520(field_40_rotation) * field_38_velocity,
                                             field_80_sprite_ptr->field_1C_zpos);

    field_80_sprite_ptr->set_ang_lazy_420690(field_40_rotation);
    if (field_69_is_colliding_with_sprite)
    {
        v71 = 1;
    }

    if (field_7C_pPed->IsPedGoingToEnterCar_492FD0())
    {
        v71 = 1;
    }
    if (v71 == 1 || field_58_flags_bf.b0 == true)
    {
        byte_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                  field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                  field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);

        Char_B4::ManageZCoordAndSlopes_548590();
    }
    Char_B4::DispatchCollision_548670(byte_6FDB56);

    if (field_69_is_colliding_with_sprite)
    {
        if (Char_B4::ContinueMovementAfterCollision_54B8F0() == 1)
        {
            Fix16 x = field_80_sprite_ptr->field_14_xy.x;
            Fix16 y = field_80_sprite_ptr->field_14_xy.y;

            u8 x_tile = x.ToUInt8();
            u8 y_tile = y.ToUInt8();
            if (gCharB4_Saved_Xpos_6FD7F8.ToUInt8() != x_tile || gCharB4_Saved_Ypos_6FD800.ToUInt8() != y_tile)
            {
                field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);

                field_45_slope_gradient_direction = gCharB4_Saved_SlopeGradDir_6FD7B0.ToUInt8();
                if (!Char_B4::CanReachTile_550090(x_tile, y_tile))
                {
                    field_69_is_colliding_with_sprite = 0;
                    field_5C = 10;
                    if (byte_6FDB58)
                    {
                        unk_ypos = 4;
                        s16 int_4F7AE0 = gRng_6F6784.get_int_4F7AE0(4);
                        switch (int_4F7AE0)
                        {
                            case 0:
                                field_40_rotation = kAng0_6FDB34;
                                break;
                            case 1:
                                field_40_rotation = kAng180_6FD936;
                                break;
                            case 2:
                                field_40_rotation = kAng270_6FD95C;
                                break;
                            default:
                                field_40_rotation = kAng90_6FD854;
                                break;
                        }
                    }
                }
                else
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(x, y, gCharB4_Saved_Zpos_6FD7FC);

                    if (v71 == 1 || field_58_flags_bf.b0 == true)
                    {

                        byte_6FDB54 =
                            gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                        field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                        (field_80_sprite_ptr->field_1C_zpos - kFP16One_6FD9E8).ToInt());
                        Char_B4::ManageZCoordAndSlopes_548590();
                    }
                }
            }
        }
        else
        {
            field_69_is_colliding_with_sprite = 0;
            field_2A = kAng0_6FDB34;
            field_5C = 10;
        }
    }

    if (field_6A)
    {
        field_40_rotation = v77;
    }
    if (field_58_flags_bf.b3)
    {
        field_40_rotation = Ang16(field_40_rotation.rValue + kAng180_6FD936.rValue, (u8)0);
        field_38_velocity = -field_38_velocity;
    }
    Char_B4::sub_54DD70();
}

// https://decomp.me/scratch/4eLOQ
WIP_FUNC(0x550f60)
Ang16 Char_B4::GetNextRotationToward_550F60(Ang16 inputAng)
{
    WIP_IMPLEMENTED;

    u8 side_curr = field_40_rotation.ToAng4_405680();
    u8 side_input_ang = inputAng.ToAng4_405680();

    // 9.6f has no trace of this; 10.5 constructs the step angle from it and then overwrites it.
    // The product is a temporary inside the inline: the step angle sits in the temporaries area.
    Ang16 v12 = word_6FDB2E.MultiplyByFix16_401CB0(field_38_velocity);

    if (field_10_char_state == 10)
    {
        return inputAng;
    }

    if (field_38_velocity > kZeroVelocity_6FD7C0)
    {
        if (field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC)
        {
            v12 = k_dword_6FD892;
        }
        else
        {
            v12 = word_6FD890;
        }
    }
    else
    {
        v12 = kAng45_6FD89C;
    }

    switch (side_input_ang)
    {
        case 3: // west
            if (side_curr <= 1)
            {
                if (ComputeShortestAngleDelta_4056C0(inputAng, field_40_rotation) > kAng180_6FD936)
                {
                    return Ang16(field_40_rotation + v12);
                }
                else
                {
                    return Ang16(field_40_rotation - v12);
                }
            }
            break;
        case 2: // north
            if (side_curr == 0)
            {
                if (ComputeShortestAngleDelta_4056C0(inputAng, field_40_rotation) > kAng180_6FD936)
                {
                    return Ang16(field_40_rotation + v12);
                }
                else
                {
                    return Ang16(field_40_rotation - v12);
                }
            }
            break;
        case 1: // east
            if (side_curr == 3)
            {
                if (ComputeShortestAngleDelta_4056C0(field_40_rotation, inputAng) < kAng180_6FD936)
                {
                    return Ang16(field_40_rotation - v12);
                }
                else
                {
                    return Ang16(field_40_rotation + v12);
                }
            }
            break;
        case 0: // south
            switch (side_curr)
            {
                case 2:
                    if (ComputeShortestAngleDelta_4056C0(field_40_rotation, inputAng) < kAng180_6FD936)
                    {
                        return Ang16(field_40_rotation - v12);
                    }
                    else
                    {
                        return Ang16(field_40_rotation + v12);
                    }
                    break;
                case 3:
                    if (ComputeShortestAngleDelta_4056C0(inputAng, field_40_rotation) < v12)
                    {
                        return inputAng;
                    }
                    else
                    {
                        return field_40_rotation + v12;
                    }
                    break;
            }
            break;
    }

    // Close enough to the target: snap to it (the original returns inputAng here, not field_40_rotation)
    Ang16 result;
    if (inputAng > field_40_rotation)
    {
        if (ComputeShortestAngleDelta_4056C0(inputAng, field_40_rotation) > v12)
        {
            result = field_40_rotation + v12;
        }
        else
        {
            result = inputAng;
        }
    }
    else
    {
        if (ComputeShortestAngleDelta_4056C0(field_40_rotation, inputAng) > v12)
        {
            result = field_40_rotation - v12;
        }
        else
        {
            result = inputAng;
        }
    }
    return result;
}

MATCH_FUNC(0x551350)
bool Char_B4::CanStepInDirection_551350(Ang16 ang)
{
    Fix16 old_x = field_80_sprite_ptr->field_14_xy.x;
    Fix16 old_y = field_80_sprite_ptr->field_14_xy.y;
    Fix16 x_pos;
    Fix16 y_pos;

    if (field_C_ped_state_2 == ped_state_2::Unknown_3)
    {
        Ang16::PolarToCartesian_41FC20(ang, dword_6FDB18, x_pos, y_pos);
    }
    else
    {
        Ang16::PolarToCartesian_41FC20(ang, dword_6FDB08, x_pos, y_pos);
    }
    x_pos += old_x;
    y_pos += old_y;
    return Char_B4::CanReachTile_550090(x_pos.ToInt(), y_pos.ToInt());
}

MATCH_FUNC(0x551400)
void Char_B4::ChooseNextMovementTile_551400()
{
    s32 saved_direction = gCharB4_PathDirection_623F44;
    switch (gCharB4_PathDirection_623F44)
    {
        case 1:
        case 2:
            if (field_7C_pPed->Get_F1C4_x_492CE0() <= gCharB4_Saved_Xpos_6FD7F8)
            {
                gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 + 1, gCharB4_Saved_TileY_6FDAD9))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                    field_60 = 3;
                    field_55 = 40;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                    if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 - 1, gCharB4_Saved_TileY_6FDAD9))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                        field_60 = 4;
                        field_55 = 40;
                        return;
                    }
                }
            }
            else
            {
                gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 - 1, gCharB4_Saved_TileY_6FDAD9))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                    field_60 = 4;
                    field_55 = 40;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                    if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 + 1, gCharB4_Saved_TileY_6FDAD9))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                        field_60 = 3;
                        field_55 = 40;
                        return;
                    }
                }
            }

            if (saved_direction == 1)
            {
                field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
            }
            else
            {
                field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
            }
            break;

        case 3:
        case 4:

            if (field_7C_pPed->Get_F1C4_y_492CF0() <= gCharB4_Saved_Ypos_6FD800)
            {
                gAng16_AngleOfCollision_6FD808 = word_6FDB3C;
                if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 + 1))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
                    field_55 = 40;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                    if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 - 1))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
                        field_55 = 40;
                        return;
                    }
                }
            }
            else
            {
                gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 - 1))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
                    field_55 = 40;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = word_6FDB3C;
                    if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 + 1))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
                        field_55 = 40;
                        return;
                    }
                }
            }

            if (saved_direction == 3)
            {
                field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
            }
            else
            {
                field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
            }
            break;
        default:
            break;
    }
    field_55 = 40;
}

MATCH_FUNC(0x5516f0)
void Char_B4::SelectNextTileFast_5516F0()
{
    s32 saved_direction = gCharB4_PathDirection_623F44;
    switch (gCharB4_PathDirection_623F44)
    {
        case 1:
        case 2:
            if (field_7C_pPed->Get_F1C4_x_492CE0() >= gCharB4_Saved_Xpos_6FD7F8)
            {
                gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 + 1, gCharB4_Saved_TileY_6FDAD9))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                    if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 - 1, gCharB4_Saved_TileY_6FDAD9))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                        return;
                    }
                }
            }
            else
            {
                gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 - 1, gCharB4_Saved_TileY_6FDAD9))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                    if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 + 1, gCharB4_Saved_TileY_6FDAD9))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                        return;
                    }
                }
            }

            if (saved_direction == 1)
            {
                field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
            }
            else
            {
                field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
            }
            break;

        case 3:
        case 4:

            if (field_7C_pPed->Get_F1C4_y_492CF0() >= gCharB4_Saved_Ypos_6FD800)
            {
                gAng16_AngleOfCollision_6FD808 = word_6FDB3C;
                if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 + 1))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                    if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 - 1))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
                        return;
                    }
                }
            }
            else
            {
                gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 - 1))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = word_6FDB3C;
                    if (Char_B4::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 + 1))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
                        return;
                    }
                }
            }

            if (saved_direction == 3)
            {
                field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
            }
            else
            {
                field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
            }
            break;
        default:
            break;
    }
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
            if (field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC)
            {
                field_6C_animation_state = 1;
            }
            else
            {
                if (field_38_velocity != kZeroVelocity_6FD7C0)
                {
                    field_6C_animation_state = 0;
                }
                else
                {
                    field_6C_animation_state = 2;
                    if (field_84_target_car)
                    {
                        field_40_rotation = field_84_target_car->field_50_car_sprite->field_0;
                    }
                }
            }
        }
    }
    else if (field_C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
    {
        field_10_char_state = Char_B4_state::Interacting_Car_Door_36;
        Sprite* nearestSprt = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(field_80_sprite_ptr, 0);
        if (!nearestSprt || nearestSprt->get_type_416B40() != sprite_types_enum::car_2 ||
            (nearestSprt->field_8_car_bc_ptr == field_7C_pPed->get_target_to_enter_403B10()) ||
            nearestSprt->field_8_car_bc_ptr->is_on_trailer_421720() || field_7C_pPed->sub_45BD20(nearestSprt->field_8_car_bc_ptr))
        {
            if (field_6C_animation_state != Char_Anim_state::Entering_Car_6)
            {
                // TODO: remove Ang16 operator=(const Ang16& other) without breaking Player::DoPedControlInputs_566C80
                field_40_rotation.rValue = field_84_target_car->field_50_car_sprite->field_0.rValue;
                field_6C_animation_state = Char_Anim_state::Entering_Car_6;
                field_68_animation_frame = 0;
                if (field_84_target_car->sub_43B540(field_7C_pPed->get_target_car_door_403A60()))
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
        s8 target_door = field_7C_pPed->get_target_car_door_403A60();
        if (field_84_target_car->sub_43B540(target_door))
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

MATCH_FUNC(0x551BB0)
void Char_B4::state_5_551BB0()
{
    if (field_C_ped_state_2 == ped_state_2::ped2_following_a_car_4)
    {
        this->field_58_flags_bf.b7 = 0;

        state_1_5504F0();

        if ((this->field_7C_pPed->GetBit11_433CA0()) == 0 && this->field_10_char_state != 15)
        {
            if (field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC)
            {

                this->field_6C_animation_state = 1;
            }
            else if (field_38_velocity != kZeroVelocity_6FD7C0)
            {
                this->field_6C_animation_state = 0;
            }
            else
            {
                this->field_6C_animation_state = 2;
                this->field_40_rotation = field_84_target_car->field_50_car_sprite->field_0;
            }
        }
    }
    else if (field_C_ped_state_2 == ped_state_2::ped2_entering_a_car_6 && this->field_6C_animation_state != 6)
    {
        this->field_40_rotation = field_84_target_car->field_50_car_sprite->field_0;
        field_84_target_car->GetDoorWorldPosition_43B5A0(field_7C_pPed->get_target_car_door_403A60(),
                                              &field_80_sprite_ptr->field_14_xy.x,
                                              &field_80_sprite_ptr->field_14_xy.y);
        this->field_6C_animation_state = 6;
        this->field_68_animation_frame = 0;
        if (field_84_target_car->sub_43B540(field_7C_pPed->get_target_car_door_403A60()))
        {
            this->field_70_frame_timer = 3;
        }
        else
        {
            this->field_70_frame_timer = 0;
        }
    }
}

// https://decomp.me/scratch/qNjdM
MATCH_FUNC(0x551CB0)
void Char_B4::state_7_551CB0()
{
    u8 block_type;
    gmp_block_info* block_4DFE10;
    field_38_velocity = kZeroVelocity_6FD7C0;
    if (field_10_char_state != Char_B4_state::Jumping_15)
    {
        Char_B4::CheckAndHandleCollisions_5459C0();
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
        if (field_58_flags_bf.b0 || gCharB4_Saved_Zpos_6FD7FC.GetFracValue() != kFP16Zero_6FD9E4)
        {
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());
            block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());
        }
        else
        {
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
            block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        }
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
                            field_7C_pPed->field_264_killer_id_timer = 50;
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
        field_40_rotation = field_7C_pPed->get_field_130_492CC0();
    }
    if (field_10_char_state == Char_B4_state::Jumping_15)
    {
        field_38_velocity = gRunOrJumpSpeed_6FD7D0; // line 1be
    }
    if (field_6A > 0 || field_10_char_state == Char_B4_state::Jumping_15 || field_4C_conveyor_dx != kFP16Zero_6FD9E4 || field_50_conveyor_dy != kFP16Zero_6FD9E4)
    {
        Set_F8_ped_state_1_433910(0);
        field_C_ped_state_2 = 0;
        Char_B4::state_0_54DDF0();
        Set_F8_ped_state_1_433910(7);
        field_C_ped_state_2 = 14;
        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            return;
        }
    }
    else
    {
        // Raw compares: the Fix16 operator== inlines push this function over VC6's inline budget
        if (field_58_flags_bf.b0 == false &&
            (gCharB4_Saved_Zpos_6FD7FC.mValue == kFP16One_6FD9E8.mValue || gCharB4_Saved_Zpos_6FD7FC.mValue == gFix16_Two_6FD9EC.mValue ||
             gCharB4_Saved_Zpos_6FD7FC.mValue == kFP16Three_6FD9F0.mValue || gCharB4_Saved_Zpos_6FD7FC.mValue == kFP16Four_6FD9F4.mValue ||
             gCharB4_Saved_Zpos_6FD7FC.mValue == kFP16Five_6FD9F8.mValue || gCharB4_Saved_Zpos_6FD7FC.mValue == kFP16Six_6FD9FC.mValue ||
             gCharB4_Saved_Zpos_6FD7FC.mValue == kFP16Seven_6FDA00.mValue))
        {
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        }
        else
        {
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());
        }

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
            field_16_state_init_pending = 1;
            return;
        }
    }

    switch (field_C_ped_state_2)
    {
        case ped_state_2::ped2_staying_14:
            if (field_7C_pPed->GetBit11_433CA0() == 1) // line 344
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
                        if (gRng_6F6784.get_int_4F7AE0(600) < 4u && field_10_char_state != Char_B4_state::Smoking_35)
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
        case 8:
            field_6C_animation_state = 9;
            break;
        case 9:
            field_6C_animation_state = 9;
            break;
        default:
            field_6C_animation_state = 2;
            break;
    }
}

// https://decomp.me/scratch/x9UcS
WIP_FUNC(0x5520A0)
void Char_B4::state_8_5520A0()
{
    WIP_IMPLEMENTED;
    Fix16 v8;
    Fix16 v9;
    Object_2C* field_184_pObj2C;
    Fix16 v36;

    Sprite* NearestSpriteOfType_477E60;
    Fix16 v44;
    Fix16 v45;
    Sprite* v54;
    Sprite* v60;

    Fix16 temp;

    Ang16 rotation;

    s16 v76 = 0;
    s16 v77 = 0;

    field_7C_pPed->ClearBit11_403A40();
    if (field_16_state_init_pending == 1)
    {
        field_16_state_init_pending = 0;
        switch (field_C_ped_state_2)
        {
            case ped_state_2::lying_on_floor_22:
                if (field_7C_pPed->IsField238_45EDE0(2))
                {
                    field_48_lying_on_floor_timer = 20;
                }
                else
                {
                    field_48_lying_on_floor_timer = 60;
                }

                switch (field_10_char_state)
                {
                    case 33:
                        if (field_6C_animation_state != 15)
                        {
                            field_6C_animation_state = 15;
                            field_68_animation_frame = 0;
                        }
                        break;
                    case 34:
                        if (field_6C_animation_state != 16)
                        {
                            field_40_rotation = field_40_rotation + kAng180_6FD936;
                            field_6C_animation_state = 16;
                            field_68_animation_frame = 0;
                        }
                        break;
                    default:
                        field_6C_animation_state = 10;
                        break;
                }
                field_80_sprite_ptr->set_num_40F7B0(6);
                break;

            case ped_state_2::falling_19:
                field_6C_animation_state = 11;
                field_90_fall_speed = field_38_velocity;
                field_94_fall_z_speed = kFP16Zero_6FD9E4;
                field_68_animation_frame = 0;
                GetTileFracX64_545640(gCharB4_Saved_Xpos_6FD7F8, &v76);
                GetTileFracY64_545670(gCharB4_Saved_Ypos_6FD800, &v77);
                v8 = kFP16Zero_6FD9E4;
                v9 = kFP16Zero_6FD9E4;
                if (v76 < 10)
                {
                    v8 = dword_6FD87C;
                }
                if (v76 > 54)
                {
                    v8 = -dword_6FD87C;
                }
                if (v77 < 10)
                {
                    v9 = dword_6FD87C;
                }
                if (v77 > 54)
                {
                    v9 = -dword_6FD87C;
                }
                field_80_sprite_ptr->set_xyz_lazy_420600(v8 + field_80_sprite_ptr->field_14_xy.x,
                                                         v9 + field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_1C_zpos);
                break;

            case ped_state_2::sinking_20:
                field_6C_animation_state = 2;
                field_46_timer = 10;
                break;

            case ped_state_2::Unknown_24:
            case ped_state_2::Unknown_25:
            case ped_state_2::Unknown_26:
                field_184_pObj2C = field_7C_pPed->field_184_pObj2C;
                if (!field_184_pObj2C || !field_184_pObj2C->field_10_obj_3c || !field_184_pObj2C->field_4)
                {
                    field_7C_pPed->Kill_46F9D0();
                    Set_F8_ped_state_1_433910(9);
                    field_C_ped_state_2 = 15;
                    field_7C_pPed->field_278_ped_state_1 = 9;
                    field_7C_pPed->field_27C_ped_state_2 = 15;
                    return;
                }
                field_6C_animation_state = 12;
                field_184_pObj2C = field_7C_pPed->field_184_pObj2C;
                {
                    Sprite* pMySprite = field_80_sprite_ptr;
                    pMySprite->set_xyz_lazy_420600(field_184_pObj2C->field_4->field_14_xy.x,
                                                   field_184_pObj2C->field_4->field_14_xy.y,
                                                   field_184_pObj2C->field_4->field_1C_zpos);
                }

                if (field_C_ped_state_2 != 24)
                {
                    Ang16 rot = field_7C_pPed->field_184_pObj2C->field_4->field_0;
                    field_40_rotation = rot;
                }
                if (!field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_34)
                {
                    field_80_sprite_ptr->set_num_40F7B0(6);
                }
                field_4A = 300;
                break;

            case ped_state_2::Unknown_17:
                field_6C_animation_state = 8;
                field_68_animation_frame = 0;
                break;

            case ped_state_2::electrocuted_27:
                field_6C_animation_state = 17;
                field_68_animation_frame = 0;
                field_46_timer = 0;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (field_C_ped_state_2)
        {
            case ped_state_2::sinking_20:
                --field_46_timer;
                rotation = kAng180_6FD936 + field_80_sprite_ptr->field_0;
                gParticle_8_6FD5E8->EmitWaterSplash_53F060(field_80_sprite_ptr->field_14_xy.x,
                                                           field_80_sprite_ptr->field_14_xy.y,
                                                           field_80_sprite_ptr->field_1C_zpos,
                                                           rotation,
                                                           1);
                if ((field_46_timer % 2) == 0)
                {
                    field_80_sprite_ptr->ShrinkSprite_59E390(dword_6FDB24, dword_6FDB24, 0);
                }
                if (field_46_timer == 0)
                {
                    // Ped sunk
                    field_7C_pPed->Kill_46F9D0();
                    field_7C_pPed->ChangeNextPedState1_45C500(9);
                    field_7C_pPed->ChangeNextPedState2_45C540(15);
                    field_7C_pPed->RestorePreviousPedState_45C5A0();
                    field_80_sprite_ptr->set_z_lazy_420660(kFP16Zero_6FD9E4);
                    field_7C_pPed->field_224 &= ~0x20u;
                }
                break;

            case ped_state_2::lying_on_floor_22:

                if (field_48_lying_on_floor_timer == 0)
                {
                    if ((field_7C_pPed->field_21C & 0x20) == 0)
                    {
                        field_7C_pPed->field_210_shock_counter = 0;
                        if (field_7C_pPed->field_280_stored_ped_state_1 == 9)
                        {
                            field_7C_pPed->RestorePreviousPedState_45C5A0();
                            if (!field_7C_pPed->IsField238_45EDE0(2) && !field_7C_pPed->IsField238_45EDE0(5))
                            {
                                field_7C_pPed->SetObjective(objectives_enum::objective_28, 9999);
                            }
                        }
                        else
                        {
                            if (field_7C_pPed->get_health_433B70() == 0)
                            {
                                field_7C_pPed->Kill_46F9D0();
                            }
                            else
                            {
                                field_7C_pPed->ChangeNextPedState1_45C500(0);
                                field_7C_pPed->ChangeNextPedState2_45C540(0);
                                field_7C_pPed->RestorePreviousPedState_45C5A0();
                            }

                            field_6C_animation_state = 0;
                        }
                        gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);
                        if (field_4C_conveyor_dx != kFP16Zero_6FD9E4 || field_50_conveyor_dy != kFP16Zero_6FD9E4)
                        {
                            field_80_sprite_ptr->set_xyz_lazy_420600(field_4C_conveyor_dx + field_80_sprite_ptr->field_14_xy.x,
                                                                     field_50_conveyor_dy + field_80_sprite_ptr->field_14_xy.y,
                                                                     field_80_sprite_ptr->field_1C_zpos);
                        }
                    }
                }

                break;

            case ped_state_2::Unknown_17:
                if (!field_84_target_car->IsDoorAccessible_43AFE0(field_7C_pPed->field_24C_target_car_door))
                {
                    field_68_animation_frame = 3;
                }
                if (field_68_animation_frame == 3)
                {

                    if (field_84_target_car->field_58_physics)
                    {
                        field_7C_pPed->field_184_pObj2C = gObject_5C_6F8F84->NewUnknown_52A240(110,
                                                                                               field_80_sprite_ptr->field_14_xy.x,
                                                                                               field_80_sprite_ptr->field_14_xy.y,
                                                                                               field_80_sprite_ptr->field_1C_zpos,
                                                                                               field_84_target_car->field_50_car_sprite->field_0,
                                                                                               field_80_sprite_ptr->field_0,
                                                                                               field_84_target_car->field_58_physics->vec_len_552DE0(),
                                                                                               -k_dword_6FD868,
                                                                                               0);
                    }
                    else
                    {
                        field_7C_pPed->field_184_pObj2C = gObject_5C_6F8F84->NewUnknown_52A240(110,
                                                                                               field_80_sprite_ptr->field_14_xy.x,
                                                                                               field_80_sprite_ptr->field_14_xy.y,
                                                                                               field_80_sprite_ptr->field_1C_zpos,
                                                                                               field_84_target_car->field_50_car_sprite->field_0,
                                                                                               field_80_sprite_ptr->field_0,
                                                                                               dword_6FD87C,
                                                                                               -k_dword_6FD868,
                                                                                               0);
                    }

                    field_7C_pPed->field_280_stored_ped_state_1 = 0;
                    field_7C_pPed->field_284_stored_ped_state_2 = 0;
                    field_7C_pPed->ChangeNextPedState1_45C500(8);
                    gParticle_8_6FD5E8->EmitBloodBurst_53E450(field_80_sprite_ptr->field_14_xy.x,
                                                              field_80_sprite_ptr->field_14_xy.y,
                                                              field_80_sprite_ptr->field_1C_zpos,
                                                              field_80_sprite_ptr->field_0);
                    field_7C_pPed->ChangeNextPedState2_45C540(25);
                    field_C_ped_state_2 = 24;
                    field_6C_animation_state = 10;
                }
                break;
            case ped_state_2::falling_19:
                v36 = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC); // TODO: fix Fix16 return
                if (gCharB4_Saved_Zpos_6FD7FC < v36 || gCharB4_Saved_Zpos_6FD7FC >= v36 + kFP16Eighth_6FDB04)
                {
                    Ang16::PolarToCartesian_41FC20(field_40_rotation, field_90_fall_speed, v44, v45);
                    v44 += gCharB4_Saved_Xpos_6FD7F8;
                    v45 += gCharB4_Saved_Ypos_6FD800;
                    if (Char_B4::CanMoveToTile_54C500(v44.ToInt(), v45.ToInt())) // ToUInt8 ?????
                    {
                        field_80_sprite_ptr->set_xyz_lazy_420600(v44, v45, field_80_sprite_ptr->field_1C_zpos);
                    }
                    if (field_94_fall_z_speed < kFpPoint1_6FD824)
                    {
                        field_94_fall_z_speed += dword_6FD9A0;
                    }
                    field_80_sprite_ptr->field_1C_zpos -= field_94_fall_z_speed;
                    if (field_90_fall_speed > kFP16One_6FD9E8)
                    {
                        field_90_fall_speed -= dword_6FD82C;
                    }
                }
                else
                {
                    field_80_sprite_ptr->field_1C_zpos = v36;
                    field_7C_pPed->RestorePreviousPedState_45C5A0();
                    Set_F8_ped_state_1_433910(field_7C_pPed->GetPedState_403990());
                    field_C_ped_state_2 = field_7C_pPed->GetPedState2_433B60();
                    NearestSpriteOfType_477E60 = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(field_80_sprite_ptr, 0);
                    if (field_6C_animation_state == Char_Anim_state::Lethal_Fall_12)
                    {
                        if (field_7C_pPed->field_21C_bf.b24 == false) //if ((v41 & 0x1000000) == 0)
                        {
                            field_7C_pPed->field_250 = 27;
                        }
                        if (NearestSpriteOfType_477E60)
                        {
                            field_7C_pPed->field_184_pObj2C = gObject_5C_6F8F84->NewUnknown_52A240(110,
                                                                                                   field_80_sprite_ptr->field_14_xy.x,
                                                                                                   field_80_sprite_ptr->field_14_xy.y,
                                                                                                   field_80_sprite_ptr->field_1C_zpos,
                                                                                                   field_80_sprite_ptr->field_0,
                                                                                                   field_80_sprite_ptr->field_0,
                                                                                                   kFpPoint1_6FD824,
                                                                                                   -dword_6FD9A0,
                                                                                                   kFP16Zero_6FD9E4);

                            field_7C_pPed->ChangeNextPedState1_45C500(8);
                            field_7C_pPed->ChangeNextPedState2_45C540(26);
                            // 9.6f: Ped::Set_B4_F16_To_1_433B50 here and below (inlined, using it makes the diff worse)
                            field_7C_pPed->field_168_game_object->field_16_state_init_pending = 1;
                            return;
                        }

                        if (field_7C_pPed->field_208_invulnerability > 0)
                        {
                            SetPedState1_433910(0);
                            SetPedState2_433A50(0);
                        }
                        else
                        {
                            field_7C_pPed->Kill_46F9D0();
                        }
                    }
                    else
                    {
                        field_7C_pPed->Set_F250_IfBit_433DD0(26);
                        if (NearestSpriteOfType_477E60)
                        {
                            Char_B4::DoJump_5454D0();
                        }
                    }
                }
                break;

            case ped_state_2::Unknown_24:
            case ped_state_2::Unknown_25:
            case ped_state_2::Unknown_26:

                if (field_4A > 0)
                {
                    field_4A--;
                }
                if (field_4A == 0)
                {
                    field_C_ped_state_2 = 26;
                }
                if (!field_7C_pPed->field_184_pObj2C)
                {
                    field_7C_pPed->Kill_46F9D0();
                    Set_F8_ped_state_1_433910(9);
                    field_C_ped_state_2 = 15;
                    field_7C_pPed->field_278_ped_state_1 = 9;
                    field_7C_pPed->field_27C_ped_state_2 = 15;
                    return;
                }
                if (field_7C_pPed->field_184_pObj2C->sub_5290F0() == kFP16Zero_6FD9E4 || !field_4A)
                {
                    switch (field_C_ped_state_2)
                    {
                        case 24:
                            field_7C_pPed->RestorePreviousPedState_45C5A0();
                            Set_F8_ped_state_1_433910(field_7C_pPed->field_278_ped_state_1);
                            field_C_ped_state_2 = field_7C_pPed->field_27C_ped_state_2;
                            break;
                        case 25:
                            field_7C_pPed->ChangeNextPedState1_45C500(8);
                            field_7C_pPed->field_27C_ped_state_2 = 22;
                            field_7C_pPed->field_168_game_object->field_16_state_init_pending = 1;
                            Set_F8_ped_state_1_433910(8);
                            field_C_ped_state_2 = 22;
                            break;
                        case 26:
                            field_7C_pPed->RestorePreviousPedState_45C5A0();
                            field_7C_pPed->Kill_46F9D0();
                            field_7C_pPed->Set_F250_IfBit_433DD0(27);
                            break;
                    }
                    v60 = field_7C_pPed->field_184_pObj2C->field_4;

                    field_80_sprite_ptr->set_xyz_lazy_420600(v60->field_14_xy.x, v60->field_14_xy.y, v60->field_1C_zpos);

                    field_80_sprite_ptr->set_ang_lazy_420690(field_7C_pPed->field_184_pObj2C->field_4->field_0);

                    if (!field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_34)
                    {
                        field_80_sprite_ptr->set_num_40F7B0(6);
                    }
                    field_7C_pPed->field_184_pObj2C->RequestRemoval_5290A0();
                    field_7C_pPed->field_184_pObj2C = 0;
                    Char_B4::ManageZCoordAndSlopes_548590();
                    if ((u8)Char_B4::IsOnWater_545570())
                    {
                        field_7C_pPed->PutOutFire();
                        Char_B4::DrownPed_5459E0();
                        return;
                    }

                    gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);

                    if (field_4C_conveyor_dx != kFP16Zero_6FD9E4 || field_50_conveyor_dy != kFP16Zero_6FD9E4)
                    {

                        field_80_sprite_ptr->set_xyz_lazy_420600(field_4C_conveyor_dx + field_80_sprite_ptr->field_14_xy.x,
                                                                 field_50_conveyor_dy + field_80_sprite_ptr->field_14_xy.y,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                }
                else
                {
                    v54 = field_7C_pPed->field_184_pObj2C->field_4;
                    field_80_sprite_ptr->set_xyz_lazy_420600(v54->field_14_xy.x, v54->field_14_xy.y, v54->field_1C_zpos);

                    if (field_C_ped_state_2 != 24)
                    {
                        field_40_rotation = field_7C_pPed->field_184_pObj2C->field_4->field_0;
                    }
                    if (!field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_34)
                    {
                        field_80_sprite_ptr->set_num_40F7B0(6);
                    }
                }
                break;

            case ped_state_2::electrocuted_27:
                ++field_46_timer;
                if (!bStartNetworkGame_7081F0)
                {
                    if (field_46_timer == 5)
                    {
                        field_7C_pPed->UpdateStatsForKiller_46F720();
                    }
                }
                else
                {
                    if (field_7C_pPed->field_204_killer_id)
                    {
                        field_7C_pPed->field_264_killer_id_timer = 99;
                    }
                }
                if (field_46_timer > 100u)
                {
                    if (!bStartNetworkGame_7081F0)
                    {
                        field_7C_pPed->field_264_killer_id_timer = 0;
                        field_7C_pPed->field_204_killer_id = 0;
                    }
                    Set_F8_ped_state_1_433910(0);
                    field_C_ped_state_2 = 0;
                    field_7C_pPed->field_27C_ped_state_2 = 0;
                    field_7C_pPed->field_278_ped_state_1 = 0;
                    field_7C_pPed->field_224 &= ~0x20u;
                    if (field_7C_pPed->field_15C_player)
                    {
                        gRoot_sound_66B038.PlayVoice_40F090(25);
                        field_7C_pPed->field_15C_player->SetDeathType_434950(4);
                    }
                    field_7C_pPed->Kill_46F9D0();
                    Set_F8_ped_state_1_433910(9);
                    field_C_ped_state_2 = 15;
                    field_6C_animation_state = 21;
                    field_10_char_state = 1;
                }

                break;
            default:
                break;
        }
    }
    if (field_7C_pPed->field_210_shock_counter < (u32)field_7C_pPed->field_212_electrocution_threshold)
    {
        --field_48_lying_on_floor_timer;
    }
}

// https://decomp.me/scratch/4c8aU
MATCH_FUNC(0x552DE0)
Fix16 CarPhysics_B0::vec_len_552DE0() // Weird location, I'm putting this here to preserve ordering
{
    return field_40_linvel_1.GetLength_41E260();
}

MATCH_FUNC(0x552E90)
void Char_B4::state_9_552E90()
{
    s32 rng;

    field_7C_pPed->ClearBit11_403A40();
    if (field_16_state_init_pending == 1)
    {
        switch (field_10_char_state)
        {
            case 33:
                if (field_6C_animation_state != 15)
                {
                    field_6C_animation_state = 15;
                    field_68_animation_frame = 0;
                }
                break;
            case 34:
                if (field_6C_animation_state != 16)
                {
                    field_40_rotation = field_40_rotation + kAng180_6FD936;
                    field_6C_animation_state = 16;
                    field_68_animation_frame = 0;
                }
                break;
            default:
                if (field_6C_animation_state != 21)
                {
                    rng = gRng_6F6784.get_int_4F7AE0(3);

                    switch (rng)
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
                }
                break;
        }

        field_16_state_init_pending = 0;
        if (field_7C_pPed->field_164_ped_group)
        {
            if (field_7C_pPed->field_164_ped_group->field_0)
            {
                field_7C_pPed->field_164_ped_group->RemovePed_4C9970(field_7C_pPed);
                field_7C_pPed->field_164_ped_group = 0;
            }
        }

        field_34 = 0;
        gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);
        Char_B4::ManageZCoordAndSlopes_548590();
        field_80_sprite_ptr->set_xyz_lazy_420600(field_4C_conveyor_dx + field_80_sprite_ptr->field_14_xy.x,
                                                 field_50_conveyor_dy + field_80_sprite_ptr->field_14_xy.y,
                                                 field_80_sprite_ptr->field_1C_zpos);
    }
    else
    {
        switch (field_6C_animation_state)
        {
            case 15:
                if (field_80_sprite_ptr->field_1C_zpos.ToUInt8() != 0 && field_80_sprite_ptr->field_14_xy.x.ToUInt8() > 1 &&
                    field_80_sprite_ptr->field_14_xy.y.ToUInt8() > 1 && field_80_sprite_ptr->field_14_xy.x.ToUInt8() < 254 &&
                    field_80_sprite_ptr->field_14_xy.y.ToUInt8() < 254)
                {
                    field_38_velocity = -k_CollisionRepulsionSpeed_6FD7BC;
                    Char_B4::state_0_54DDF0();
                }
                else
                {
                    field_6C_animation_state = 14;
                }
                break;
            case 16:
                if (field_80_sprite_ptr->field_1C_zpos.ToUInt8() != 0 && field_80_sprite_ptr->field_14_xy.x.ToUInt8() > 1 &&
                    field_80_sprite_ptr->field_14_xy.y.ToUInt8() > 1 && field_80_sprite_ptr->field_14_xy.x.ToUInt8() < 254 &&
                    field_80_sprite_ptr->field_14_xy.y.ToUInt8() < 254)
                {
                    field_38_velocity = -k_CollisionRepulsionSpeed_6FD7BC;
                    Char_B4::state_0_54DDF0();
                }
                else
                {
                    field_6C_animation_state = 14;
                }
                break;
            case 13:
            case 14:
            case 19:
            case 20:
                if (field_4C_conveyor_dx != kFP16Zero_6FD9E4 || field_50_conveyor_dy != kFP16Zero_6FD9E4)
                {
                    field_4C_conveyor_dx = kFP16Zero_6FD9E4;
                    field_50_conveyor_dy = kFP16Zero_6FD9E4;
                    gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);

                    field_80_sprite_ptr->set_xyz_lazy_420600(field_4C_conveyor_dx + field_80_sprite_ptr->field_14_xy.x,
                                                             field_50_conveyor_dy + field_80_sprite_ptr->field_14_xy.y,
                                                             field_80_sprite_ptr->field_1C_zpos);
                }

                field_80_sprite_ptr->field_28_num = 6;
                break;
            default:
                if (field_6C_animation_state != 21)
                {
                    rng = gRng_6F6784.get_int_4F7AE0(3);
                    switch (rng)
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
                }
                field_68_animation_frame = 0;
                field_80_sprite_ptr->set_num_40F7B0(6);
                break;
        }
    }
}

MATCH_FUNC(0x5532C0)
bool Char_B4::IsNearTileCentre_5532C0()
{
    s16 ret1 = 0;
    s16 ret2 = 0;
    Char_B4::GetTileFracX64_545640(gCharB4_Saved_Xpos_6FD7F8, &ret1);
    Char_B4::GetTileFracY64_545670(gCharB4_Saved_Ypos_6FD800, &ret2);
    if (ret1 > 20 && ret1 < 44 && ret2 > 20 && ret2 < 44)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x553330)
char_type Char_B4::IsThreatToSearchingPed_553330()
{
    return field_7C_pPed->IsThreatToSearchingPed_4661F0();
}

// 9.6f 0x497480
MATCH_FUNC(0x553340)
bool Char_B4::ShouldCollideWithSprite_553340(Sprite* pSprite)
{
    Ped* pPed; // eax

    if (pSprite)
    {
        switch (pSprite->get_type_416B40())
        {
            case sprite_types_enum::car_2:
                break;

            case sprite_types_enum::ped_3:
                break;

            default:
            {
                Object_2C* pObj = pSprite->As2C_40FEC0();
                if (pObj)
                {
                    u8 idx = field_7C_pPed->get_varrok_idx_420B50();
                    if (pObj->is_not_type6_to_12_and_idx_matches_4973E0(idx))
                    {
                        return 0;
                    }

                    if (pObj->is_region_bucket_3_4210B0())
                    {
                        return 0;
                    }
                }
            }
            break;
        }
    }

    switch (this->field_C_ped_state_2)
    {
        case 6:
        case 7:
        case 9:
        case 10:
        case 17:
        case 19:
        case 24:
        case 25:
        case 26:
        case 27:
            return 0;

        case 8:
            if (pSprite == 0)
            {
                return 1;
            }
            return 0;

        case 22:
            break;

        default:
            pPed = this->field_7C_pPed;
            if (pPed->GetPedState_403990() == ped_state_1::dead_9 &&
                ((pPed->GetBit24_475B50()) == 0 || !pSprite || !pSprite->IsObjectModelEqual_59E930(198)))
            {
                return 0;
            }

            if (this->field_10_char_state == 15 && pSprite)
            {
                switch (pSprite->get_type_416B40())
                {
                    case 2:
                        return 0;
                    case 4:
                        break;
                    default:
                        return 0;
                }
            }
            break;
    }
    return 1;
}

MATCH_FUNC(0x5535B0)
bool Char_B4::PhoneTouched_5535B0(Object_2C* p2c)
{
    Ped* pPed = field_7C_pPed;
    if (pPed->field_15C_player)
    {
        return gfrosty_pasteur_6F8060->AnswerPhone_5129F0(pPed->get_id(), p2c->field_14_id);
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x553640)
char_type Char_B4::OnObjectTouched_553640(Object_2C* p2c)
{
    s8 v6;
    s8 v7;

    if (p2c->check_is_shop_421060())
    {
        return get_ped_433A20()->HandlePickupCollision_45DE80(p2c);
    }

    switch (p2c->field_18_model)
    {
        case objects::secret_token_266: 
            return get_ped_433A20()->HandlePickupCollision_45DE80(p2c);

        case 257:
        case objects::ped_crossing_trigger_258:
            if (field_8_ped_state_1 != ped_state_1::dead_9 && field_8_ped_state_1 != ped_state_1::immobilized_8)
            {
                get_ped_433A20()->HandlePedCrossingTrigger_45CF20(p2c);
            }
            break;

        case objects::savepoint_161:
            gCar_214_705F20->sub_5C8780(p2c->field_27, this->field_80_sprite_ptr);
            break;

        case objects::blue_phone_164:
        case objects::red_phone_177:
        case objects::yellow_phone_179:
        case objects::green_phone_181:
            return PhoneTouched_5535B0(p2c);

        case objects::maybe_door_trigger_167:
            gDoor_4D4_67BD2C->TryOpenDoorForPed_49D370(get_ped_433A20(), p2c->get_field_26_420FF0());
            break;

        case objects::destructor_141:
            get_ped_433A20()->Kill_46F9D0();
            break;

        case objects::conveyor_139:
            p2c->GetConveyorDirection_493090(&v6, &v7); // TODO: Ang8 or something ???
            this->field_4C_conveyor_dx = k_dword_6FDA9C * v6;
            this->field_50_conveyor_dy = k_dword_6FDA9C * v7;
            break;

        default:
            break;
    }
    return 0;
}

MATCH_FUNC(0x5537F0)
char_type Char_B4::HandlePedObjectHit_5537F0(Object_2C* p2c)
{
    const u8 idx = p2c->get_field_26_420FF0();
    const s32 pedId = gVarrok_7F8_703398->GetPedId_420F10(idx);
    if (p2c->field_18_model == objects::rocket_bullet_128 || p2c->field_18_model == objects::moving_molotov_138 || p2c->field_18_model == objects::mine_10 && !gCharB4_HitByMine_6FDB59)
    {
        gObject_5C_6F8F84->CreateExplosion_52A3D0(field_80_sprite_ptr->field_14_xy.x,
                                                  field_80_sprite_ptr->field_14_xy.y,
                                                  field_80_sprite_ptr->field_1C_zpos,
                                                  kAng0_6FDB34,
                                                  18,
                                                  pedId);
        if (p2c->field_18_model == objects::mine_10)
        {
            gCharB4_HitByMine_6FDB59 = 1;
        }
    }

    if (p2c->get_field_26_420FF0() != field_7C_pPed->get_varrok_idx_420B50())
    {
        return field_7C_pPed->HandlePedHitByObject_45D000(p2c);
    }
    else
    {
        return 0;
    }
}

// GetLength_41E260 as inlined into HandleCarImpact_5538A0: kFP16Zero_6FD9E4 as the zero, out of line helpers
static inline Fix16 GetLength_inline_5538A0(Fix16_Point& p)
{
    if (p.x == kFP16Zero_6FD9E4)
    {
        return Fix16::Abs_negate_out_of_line(p.y);
    }
    else if (p.y == kFP16Zero_6FD9E4)
    {
        return Fix16::Abs_negate_out_of_line(p.x);
    }
    else
    {
        return Fix16::SquareRoot_436A70(p.x.Multiply_408680(p.x).Add_408660(p.y.Multiply_408680(p.y)));
    }
}

WIP_FUNC(0x5538A0)
#define SPAWN_KNOCKDOWN_OBJ(speed) \
    gObject_5C_6F8F84->NewUnknown_52A240(110, \
                                         this->field_80_sprite_ptr->field_14_xy.x, \
                                         this->field_80_sprite_ptr->field_14_xy.y, \
                                         this->field_80_sprite_ptr->field_1C_zpos, \
                                         tanVec, \
                                         this->field_80_sprite_ptr->field_0, \
                                         speed, \
                                         -kFpPoint1_6FD824, \
                                         kFP16Zero_6FD9E4)
void Char_B4::HandleCarImpact_5538A0(Car_BC* pCar, u8 bUnknown, Fix16 x, Fix16 y)
{
    WIP_IMPLEMENTED;

    Fix16_Point p(x, y);
    Fix16 vecLen = GetLength_inline_5538A0(p);
    Ang16 tanVec;
    tanVec = p.atan2_40F790();

    if (this->field_7C_pPed->field_208_invulnerability > 0)
    {
        DoJump_5454D0();
    }
    else
    {
        if (vecLen < gRunOrJumpSpeed_6FD7D0)
        {
            vecLen = gRunOrJumpSpeed_6FD7D0;
        }

        if (!this->field_7C_pPed->field_267_varrok_idx)
        {
            this->field_7C_pPed->field_267_varrok_idx = gVarrok_7F8_703398->AllocForPed_59B060(this->field_7C_pPed->field_200_id);
        }

        this->field_6C_animation_state = 12;
        this->field_68_animation_frame = 0;

        if (pCar->field_54_driver)
        {
            if ((pCar->field_54_driver->field_21C & 8) == 0 && pCar->field_54_driver->IsField238_45EDE0(3))
            {
                if (pCar->field_5C_AI)
                {
                    pCar->field_5C_AI->field_30_forced_stop_timer = 100;
                }
            }
        }

        if (vecLen < kFpPoint1_6FD824)
        {
            if (this->field_C_ped_state_2 == ped_state_2::lying_on_floor_22)
            {
                this->field_7C_pPed->Kill_46F9D0();
            }
            else
            {
                this->field_7C_pPed->field_184_pObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen);
                this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = kFP16Zero_6FD9E4;
                this->field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(this->field_7C_pPed->field_267_varrok_idx);
                this->field_7C_pPed->ChangeNextPedState1_45C500(8);
                this->field_7C_pPed->ChangeNextPedState2_45C540(24);
                this->field_C_ped_state_2 = ped_state_2::Unknown_24;
            }
        }
        else if (this->field_7C_pPed->IsField238_45EDE0(3))
        {
            this->field_7C_pPed->Kill_46F9D0();
            gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                      this->field_80_sprite_ptr->field_14_xy.y,
                                                      this->field_80_sprite_ptr->field_1C_zpos,
                                                      tanVec);
        }
        else if (vecLen < kFpPoint3_6FD830)
        {
            if (vecLen < dword_6FD82C)
            {
                if (this->field_C_ped_state_2 == ped_state_2::lying_on_floor_22)
                {
                    this->field_7C_pPed->Kill_46F9D0();
                }
                else
                {
                    this->field_7C_pPed->field_184_pObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two_6FD9EC);
                    this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = dword_6FD9B0;
                    this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_2A_bAirborne = 1;
                    this->field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(this->field_7C_pPed->field_267_varrok_idx);
                    this->field_7C_pPed->ChangeNextPedState1_45C500(8);

                    if (this->field_7C_pPed->field_208_invulnerability > 0)
                    {
                        this->field_7C_pPed->ChangeNextPedState2_45C540(24);
                    }
                    else if (pCar->field_7C_uni_num == 2)
                    {
                        this->field_7C_pPed->ChangeNextPedState2_45C540(26);
                    }
                    else
                    {
                        this->field_7C_pPed->ChangeNextPedState2_45C540(25);
                    }

                    this->field_C_ped_state_2 = ped_state_2::Unknown_24;

                    gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                              this->field_80_sprite_ptr->field_14_xy.y,
                                                              this->field_80_sprite_ptr->field_1C_zpos,
                                                              tanVec);
                }
            }
            else if (this->field_7C_pPed->IsField238_45EDE0(2) && pCar->field_7C_uni_num != 2)
            {
                if (this->field_C_ped_state_2 == ped_state_2::lying_on_floor_22)
                {
                    this->field_7C_pPed->Kill_46F9D0();
                }
                else
                {
                    this->field_7C_pPed->ChangeNextPedState1_45C500(8);
                    this->field_7C_pPed->field_184_pObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two_6FD9EC);
                    this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = kFpPoint1_6FD824;
                    this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_2A_bAirborne = 1;
                    this->field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(this->field_7C_pPed->field_267_varrok_idx);
                    if (field_7C_pPed->field_208_invulnerability > 0)
                    {
                        field_7C_pPed->ChangeNextPedState2_45C540(24);
                    }
                    else
                    {
                        field_7C_pPed->ChangeNextPedState2_45C540(25);
                    }

                    this->field_C_ped_state_2 = ped_state_2::Unknown_24;

                    gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                              this->field_80_sprite_ptr->field_14_xy.y,
                                                              this->field_80_sprite_ptr->field_1C_zpos,
                                                              tanVec);
                }
            }
            else
            {
                this->field_7C_pPed->Kill_46F9D0();
                gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                          this->field_80_sprite_ptr->field_14_xy.y,
                                                          this->field_80_sprite_ptr->field_1C_zpos,
                                                          tanVec);
            }
        }
        else
        {
            if (this->field_7C_pPed->field_208_invulnerability > 0)
            {
                this->field_7C_pPed->field_184_pObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two_6FD9EC);
                this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = kFP16Zero_6FD9E4;
                this->field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(this->field_7C_pPed->field_267_varrok_idx);
                this->field_7C_pPed->ChangeNextPedState1_45C500(8);
                this->field_7C_pPed->ChangeNextPedState2_45C540(24);
                this->field_C_ped_state_2 = ped_state_2::Unknown_24;
            }
            else
            {
                this->field_7C_pPed->Kill_46F9D0();
            }
            gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                      this->field_80_sprite_ptr->field_14_xy.y,
                                                      this->field_80_sprite_ptr->field_1C_zpos,
                                                      tanVec);
        }
    }
}
#undef SPAWN_KNOCKDOWN_OBJ

MATCH_FUNC(0x553E00)
void Char_B4::HandleGenericImpact_553E00(Ang16 ang, Fix16 a3, Fix16 a4, char_type a5)
{

    if (field_7C_pPed->field_208_invulnerability > 0)
    {
        a5 = 0;
    }

    if (field_7C_pPed->GetPedState_403990() != ped_state_1::dead_9)
    {
        this->field_16_state_init_pending = 1;
        this->field_7C_pPed->field_184_pObj2C = gObject_5C_6F8F84->NewUnknown_52A240(110,
                                                                                     field_80_sprite_ptr->field_14_xy.x,
                                                                                     field_80_sprite_ptr->field_14_xy.y,
                                                                                     field_80_sprite_ptr->field_1C_zpos,
                                                                                     ang,
                                                                                     field_80_sprite_ptr->field_0,
                                                                                     a3,
                                                                                     -kFpPoint02_6FD9AC,
                                                                                     a4);
        if (!field_7C_pPed->field_267_varrok_idx)
        {
            field_7C_pPed->field_267_varrok_idx = gVarrok_7F8_703398->AllocForPed_59B060(field_7C_pPed->field_200_id);
        }
        field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(field_7C_pPed->field_267_varrok_idx);
        field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);

        switch (a5)
        {
            case 0:
                field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = kFP16Zero_6FD9E4;
                field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::Unknown_24);
                field_C_ped_state_2 = ped_state_2::Unknown_24;
                break;

            case 1:
                field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = a4;
                if (field_7C_pPed->field_208_invulnerability > 0)
                {

                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::Unknown_24);
                }
                else
                {
                    if (field_7C_pPed->IsField238_45EDE0(2))
                    {
                        field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::Unknown_25);
                    }
                    else
                    {
                        field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::Unknown_26);
                    }
                }

                this->field_C_ped_state_2 = ped_state_2::Unknown_24;
                gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                          this->field_80_sprite_ptr->field_14_xy.y,
                                                          this->field_80_sprite_ptr->field_1C_zpos,
                                                          ang);
                break;

            case 2:
                field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = a4;
                if (field_7C_pPed->field_208_invulnerability > 0)
                {
                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::Unknown_24);
                }
                else
                {
                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::Unknown_26);
                }

                this->field_C_ped_state_2 = ped_state_2::Unknown_24;
                gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                          this->field_80_sprite_ptr->field_14_xy.y,
                                                          this->field_80_sprite_ptr->field_1C_zpos,
                                                          ang);
                break;
        }
    }
}

MATCH_FUNC(0x553F90)
void __stdcall ResetCharStatics_553F90()
{
    gCharB4_UpdateCounter_6FDB48 = 0;
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
