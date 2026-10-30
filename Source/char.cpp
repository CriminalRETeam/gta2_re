#define FIX16_POINT_ZERO kFP16Zero_6FD9E4
#include "ped_death_cause.hpp"
#include "ped_jump_over_mode.hpp"
#include "char_b4_state_more.hpp"
#include "char_anim_state_more.hpp"
#include "ped_voice_event.hpp"
#include "explosion_type.hpp"
#include "voice_line.hpp"
#include "char.hpp"
#include "CarAI.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "DoorPool.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Garage.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "ParticleManager.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "SpriteGrid.hpp"
#include "PedRefTable.hpp"
#include "Weapon.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "ScriptManager.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "winmain.hpp"
#include "ped_graphic_type.hpp"
#include "block_face.hpp"

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
DEFINE_GLOBAL_INIT(Ang16, kAng4_6FDB2E, Ang16(16), 0x6FDB2E);

DEFINE_GLOBAL(u8, gCharCollisionNoJump_6FDB58, 0x6FDB58);
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
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD868, Fix16(256, 0), 0x6FD868);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FD9B4, Fix16(0x2000, 0), 0x6FD9B4);
DEFINE_GLOBAL_INIT(Fix16, gRunOrJumpSpeed_6FD7D0, kFP16Four_6FD9F4* kFpOne64th_6FD868, 0x6FD7D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8B4, kFP16Zero_6FD9E4, 0x6FD8B4);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_StepXpos_6FD8B8, kFP16Zero_6FD9E4, 0x6FD8B8);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_StepYpos_6FD8BC, kFP16Zero_6FD9E4, 0x6FD8BC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FD82C, Fix16(0xCCC, 0), 0x6FD82C);
DEFINE_GLOBAL_INIT(Fix16, gCharB4_WorldCollisionOffset_6FD8D8, kFpPoint2_6FD82C, 0x6FD8D8);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_6FD870, kFpOne64th_6FD868 * 2, 0x6FD870);
DEFINE_GLOBAL_INIT(Fix16, k_CollisionRepulsionSpeed_6FD7BC, kFpOneThirtySecond_6FD870, 0x6FD7BC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_6FD9B0, Fix16(0x333, 0), 0x6FD9B0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_6FDAE4, kFpPoint05_6FD9B0, 0x6FDAE4);
DEFINE_GLOBAL_INIT(Fix16, gFix16_Two_6FD9EC, Fix16(2), 0x6FD9EC);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6FD9E8, Fix16(1), 0x6FD9E8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint03_6FD9A8, Fix16(0x1EB, 0), 0x6FD9A8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Three_6FD9F0, Fix16(3), 0x6FD9F0);
DEFINE_GLOBAL_INIT(Fix16, kFP16Five_6FD9F8, Fix16(5), 0x6FD9F8);
DEFINE_GLOBAL_INIT(Fix16, kFP16Six_6FD9FC, Fix16(6), 0x6FD9FC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Seven_6FDA00, Fix16(7), 0x6FDA00);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeThirtySeconds_6FDAC8, kFpOne64th_6FD868 * 6, 0x6FDAC8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne256th_6FD99C, kFpOne64th_6FD868 / kFP16Four_6FD9F4, 0x6FD99C);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD7B8, kFpOne64th_6FD868, 0x6FD7B8);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeSixtyFourths_6FD7CC, kFP16Three_6FD9F0* kFpOne64th_6FD868, 0x6FD7CC);

DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FDB20, kFpOne64th_6FD868 * 64, 0x6FDB20);

DEFINE_GLOBAL_INIT(Ang16, kAng12_6FD892, Ang16(48), 0x6FD892);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6FD8DC, Fix16(0x666, 0), 0x6FD8DC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6FD8E4, Fix16(0x2000, 0), 0x6FD8E4);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDB28, kFP16Half_6FD8E4, 0x6FDB28);

DEFINE_GLOBAL_INIT(Fix16, kFpOneSixteenth_6FD87C, kFpOne64th_6FD868 * 4, 0x6FD87C);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_6FD88C, kFpOne64th_6FD868 * 8, 0x6FD88C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_6FD828, Fix16(0x1000, 0), 0x6FD828);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eighth_6FDB04, Fix16(0x800, 0), 0x6FDB04);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_6FD9A0, Fix16(0xA3, 0), 0x6FD9A0);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_6FDB24, Fix16(0x200, 0), 0x6FDB24);

DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FDA9C, Fix16(0x100, 0), 0x6FDA9C);

DEFINE_GLOBAL(u16, gNumPedsOnScreen_6787EC, 0x6787EC);

DEFINE_GLOBAL(u8, gCharB4_TurnToFreeFace_6FDB51, 0x6FDB51);
DEFINE_GLOBAL(u8, gCharB4_VeerFromWalls_6FDB52, 0x6FDB52);
DEFINE_GLOBAL(u8, byte_6FDB53, 0x6FDB53);
DEFINE_GLOBAL(u8, gCharB4_GradientSlopeBelow_6FDB54, 0x6FDB54);

DEFINE_GLOBAL(u8, byte_6FDB56, 0x6FDB56);
DEFINE_GLOBAL_INIT(u8, byte_623F48, 1, 0x623F48);
DEFINE_GLOBAL(u8, gCharB4_Saved_TileX_6FDAD8, 0x6FDAD8);
DEFINE_GLOBAL(u8, gCharB4_Saved_TileY_6FDAD9, 0x6FDAD9);
DEFINE_GLOBAL(u8, gCharB4_StepPosValid_6FDB57, 0x6FDB57);

DEFINE_GLOBAL_INIT(Ang16, gAng16_AngleOfCollision_6FD808, Ang16(0), 0x6FD808);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD8A2, Ang16(360), 0x6FD8A2);
DEFINE_GLOBAL_INIT(Ang16, kAng16_6FD940, Ang16(64), 0x6FD940);
DEFINE_GLOBAL_INIT(Ang16, kAng344_6FD8F8, Ang16(1376), 0x6FD8F8);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD94C, Ang16(1080), 0x6FD94C);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD95C, Ang16(1080), 0x6FD95C);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FD854, Ang16(360), 0x6FD854);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD8E8, Ang16(0x2D0), 0x6FD8E8);
DEFINE_GLOBAL(Ang16, kAng0_6FDB3C, 0x6FDB3C);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FDA64, Ang16(0x168), 0x6FDA64);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD904, Ang16(0x438), 0x6FD904);

DEFINE_GLOBAL_INIT(Ang16, kAng6_6FDA54, Ang16(0x18), 0x6FDA54); // TODO: Init via func 0x54A300
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD920, kAng180_6FD936, 0x6FD920);
DEFINE_GLOBAL_INIT(Ang16, kAng354_6FD9D8, Ang16(0x588), 0x6FD9D8); // TODO: Init via func 0x54A270
DEFINE_GLOBAL_INIT(Ang16, kAng24_6FD890, Ang16(96), 0x6FD890);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6FD89C, Ang16(180), 0x6FD89C);

DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDAB0, kFP16Half_6FD8E4, 0x6FDAB0);

EXTERN_GLOBAL(Ang16, kAng0_6FDB34);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F67B0, Fix16(0x2000, 0), 0x6F67B0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDB18, kFpOne64th_6FD868 * 32, 0x6FDB18);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeSixteenths_6FDB08, kFpOne64th_6FD868 * 12, 0x6FDB08);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint3_6FD91C, Fix16(0x1333, 0), 0x6FD91C);
DEFINE_GLOBAL_INIT(Fix16, kFP16Eight_6FDA08, Fix16(8), 0x6FDA08);
DEFINE_GLOBAL_INIT(Fix16, gFP16_CollisionCheckRadius_6FDACC, Fix16(0x800, 0), 0x6FDACC);
DEFINE_GLOBAL_INIT(Fix16, kFP16MinusOne_6FD790, Fix16(-1), 0x6FD790);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_6FDA04, kFpOne64th_6FD868 * 256, 0x6FDA04);


MATCH_FUNC(0x4056C0)
EXPORT Ang16 __stdcall ComputeShortestAngleDelta_4056C0(Ang16& angle1, Ang16& angle2)
{
    Ang16 delta = angle1 - angle2;
    if (delta > kAng180_669156)
    {
        delta = -delta;
    }
    return delta;
}

// No callers. The angle halfway between a and b, going the short way round. The normalizations of delta
// are kept at the top level (in a register, where VC6 turns the loops into its division form): through the
// unary operator- the nested Normalize runs out of inline budget and is called out of line.
MATCH_FUNC(0x4057B0)
EXPORT Ang16 __stdcall MidAngle_4057B0(Ang16& a, Ang16& b)
{
    Ang16 half;
    Ang16 delta(b.rValue - a.rValue);
    delta.Normalize();
    if (delta > kAng180_669156)
    {
        delta.rValue = -delta.rValue;
        delta.Normalize();
        half = Ang16(Fix16(delta.rValue) / Fix16(2));
        half.Normalize_406C20();
        return a - half;
    }
    half = Ang16(Fix16(delta.rValue) / Fix16(2));
    half.Normalize_406C20();
    return a + half;
}

MATCH_FUNC(0x46DD50)
void Char::SetRemap_46DD50(u8 remap)
{
    this->field_5_remap = remap;
    if (remap != 0xFF)
    {
        field_80_sprite_ptr->SetRemap(remap);
    }
}

MATCH_FUNC(0x4F79B0)
EXPORT Fix16 __stdcall SnapZTo16_4F79B0(Fix16 zpos)
{
    return ((kFP16Half_6F67B0 + (zpos * 1000)).GetFloorValue()) / 1000;
}

MATCH_FUNC(0x529050)
EXPORT void __stdcall UnpackSignedNibbles_529050(u8 packed, s8* pX, s8* pY)
{
    *pX = (packed >> 4) - 7;
    *pY = (packed & 0xF) - 7;
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
    gCharB4_TurnToFreeFace_6FDB51 = 0;
    gCharB4_VeerFromWalls_6FDB52 = 0;
    byte_6FDB53 = 0;
    gCharB4_GradientSlopeBelow_6FDB54 = 0;
    byte_6FDB55 = 0;
    gCharB4_PathDirection_623F44 = path_direction::up_1;
    byte_6FDB56 = 0;
    byte_623F48 = 1;
    gCharB4_Saved_TileX_6FDAD8 = kFP16Zero_6FD9E4.ToUInt8();
    gCharB4_Saved_TileY_6FDAD9 = kFP16Zero_6FD9E4.ToUInt8();
    gCharB4_StepPosValid_6FDB57 = 0;
    gCharCollisionNoJump_6FDB58 = 0;
}

// https://decomp.me/scratch/ZsDjc
MATCH_FUNC(0x544ff0)
Char::Char()
{
    field_0_id = 0;
    field_4_unused = 0;
    field_5_remap = -1;
    field_6_unused = 0;
    field_8_ped_state_1 = 11;
    field_C_ped_state_2 = ped_state_2::initial_28;
    field_10_char_state = Char_B4_state::Interacting_Car_Door_36;
    field_14_target_rotation = kAng0_6FDB34;
    field_16_state_init_pending = false;
    field_18_collided_entity = NULL;
    field_1C_prev_collided_entity = NULL;
    field_20_collided_entity_type = 0;
    field_24_slide_state = 3;
    field_28_slide_rotation = kAng0_6FDB34;
    field_2A_slide_turn = kAng0_6FDB34;
    field_2C_ang = kAng0_6FDB34;
    field_30_unused = 4;
    field_34_unused = 0;
    field_38_velocity = kZeroVelocity_6FD7C0;
    field_3C_run_or_jump_speed = gRunOrJumpSpeed_6FD7D0;
    field_40_rotation = kAng0_6FDB34;
    field_42_rotation_jitter = kAng0_6FDB34;
    field_44_block_type = 0;
    field_45_slope_gradient_direction = 0;
    field_5C_jump_over_timer = 0;
    field_46_timer = 0;
    field_48_lying_on_floor_timer = 0;
    mpNext = NULL;
    field_7C_pPed = NULL;
    field_80_sprite_ptr = NULL;
    field_68_animation_frame = 0;
    field_69_is_colliding_with_sprite = false;
    field_74_repulsion_rotation = kAng0_6FDB34;
    field_6A_repulsion_timer = 0;
    field_84_target_car = NULL;
    field_58_flags_bf.bOnSlope = false;
    field_88_obj_2c.DestroyAllSprites_5A7010();
    field_8C_jump_base_z = kFP16Zero_6FD9E4;
    field_58_flags_bf.bFlag2 = false;
    field_6C_animation_state = Char_Anim_state::Initial_18;
    field_70_frame_timer = 0;
    field_71_frame_delay = 0;
    field_90_fall_speed = kFP16Zero_6FD9E4;
    field_94_fall_z_speed = kFP16Zero_6FD9E4;
    field_58_flags_bf.bFlag1 = false;
    field_98_velocity_vector.x = kFP16Zero_6FD9E4;
    field_98_velocity_vector.y = kFP16Zero_6FD9E4;
    field_58_flags_bf.bReverseVelocity = false;
    field_58_flags_bf.bTeleported = false;
    field_A4_xpos = kFP16Zero_6FD9E4;
    field_A8_ypos = kFP16Zero_6FD9E4;
    field_AC_zpos = kFP16Zero_6FD9E4;
    field_4A_timeout = 0;
}

MATCH_FUNC(0x5451A0)
Char::~Char()
{
    field_18_collided_entity = NULL;
    field_1C_prev_collided_entity = NULL;
    mpNext = NULL;
    field_7C_pPed = NULL;
    field_80_sprite_ptr = NULL;
    field_84_target_car = NULL;
}

MATCH_FUNC(0x5451C0)
bool Char::HasShadows_5451C0() const
{
    if (field_8_ped_state_1 == ped_state_1::dead_9)
    {
        return false;
    }

    if (field_C_ped_state_2 != ped_state_2::lying_on_floor_22 && field_10_char_state != Char_B4_state::Jumping_15 && field_C_ped_state_2 != ped_state_2::electrocuted_27 &&
        !field_7C_pPed->IsInvisibleOnFoot_433DA0())
    {
        return true;
    }

    return false;
}

MATCH_FUNC(0x545200)
void Char::PoolAllocate()
{
    field_0_id = gB4_id_6FDB4C++;
    Char::InitSprite_5456A0();
    field_4_unused = 1;
    field_5_remap = -1;
    field_6_unused = 0;
    field_8_ped_state_1 = 11;
    field_C_ped_state_2 = ped_state_2::initial_28;
    field_10_char_state = Char_B4_state::Interacting_Car_Door_36;
    field_14_target_rotation = kAng0_6FDB34;
    field_16_state_init_pending = false;
    field_18_collided_entity = NULL;
    field_1C_prev_collided_entity = NULL;
    field_20_collided_entity_type = 0;
    field_24_slide_state = 3;
    field_28_slide_rotation = kAng0_6FDB34;
    field_2A_slide_turn = kAng0_6FDB34;
    field_2C_ang = kAng0_6FDB34;
    field_30_unused = 4;
    field_34_unused = 0;
    field_38_velocity = kZeroVelocity_6FD7C0;
    field_3C_run_or_jump_speed = gRunOrJumpSpeed_6FD7D0;
    field_40_rotation = kAng0_6FDB34;
    field_42_rotation_jitter = kAng0_6FDB34;
    field_44_block_type = 0;
    field_45_slope_gradient_direction = 0;
    field_5C_jump_over_timer = 0;
    field_46_timer = 0;
    field_48_lying_on_floor_timer = 0;
    field_4A_timeout = 500;
    mpNext = NULL;
    field_7C_pPed = NULL;
    field_68_animation_frame = 0;
    field_69_is_colliding_with_sprite = false;
    field_74_repulsion_rotation = kAng0_6FDB34;
    field_6A_repulsion_timer = 0;
    field_84_target_car = NULL;
    field_58_flags_bf.bOnSlope = false;
    field_88_obj_2c.DestroyAllSprites_5A7010();
    field_8C_jump_base_z = kFP16Zero_6FD9E4;
    field_58_flags_bf.bFlag2 = false;
    field_6C_animation_state = Char_Anim_state::Initial_18;
    field_70_frame_timer = 0;
    field_71_frame_delay = 0;
    field_90_fall_speed = kFP16Zero_6FD9E4;
    field_94_fall_z_speed = kFP16Zero_6FD9E4;
    field_58_flags_bf.bFlag1 = false;
    field_98_velocity_vector.x = kFP16Zero_6FD9E4;
    field_98_velocity_vector.y = kFP16Zero_6FD9E4;
    field_58_flags_bf.bTeleported = false;
    field_58_flags_bf.bReverseVelocity = false;
    field_A4_xpos = kFP16Zero_6FD9E4;
    field_A8_ypos = kFP16Zero_6FD9E4;
    field_AC_zpos = kFP16Zero_6FD9E4;
    field_58_flags_bf.bFlag4 = false;
    field_4C_conveyor_dx = kFP16Zero_6FD9E4;
    field_50_conveyor_dy = kFP16Zero_6FD9E4;
    field_72_next_tile_x = kFP16Zero_6FD9E4.ToInt();
    field_73_next_tile_y = kFP16Zero_6FD9E4.ToInt();
    field_58_flags_bf.bFlag6 = false;
    field_58_flags_bf.bFlag7 = false;
    field_60_unused = 0;
    field_64_unused = 0;
    field_55_next_tile_timer = 0;
    field_A0_force_top_layer = false;
    field_B0_scream_timer = -1;
}

MATCH_FUNC(0x5453d0)
void Char::PoolDeallocate()
{
    if (field_80_sprite_ptr)
    {
        gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(field_80_sprite_ptr);
        gSpritePool_703818->remove(field_80_sprite_ptr);
        field_80_sprite_ptr = NULL;
    }
    field_88_obj_2c.DestroyAllSprites_5A7010();
    field_B0_scream_timer = -1;
}

MATCH_FUNC(0x545430)
void Char::DrawFlamesAndStartScreamTimer_545430()
{
    // Spawn fire
    Object* pObject = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::fire_197, 0, 0, 0, kAng0_6FDB34);
    field_80_sprite_ptr->AttachSprite_5A3100(pObject->field_4_sprite, 0, 0, kAng0_6FDB34);
    field_B0_scream_timer = 10; // Start screaming timer
}

MATCH_FUNC(0x5454B0)
void Char::RemoveFireSprites_5454B0()
{
    field_B0_scream_timer = -1;
    field_88_obj_2c.CleanupSpriteList_5A7080();
}

MATCH_FUNC(0x5454d0)
void Char::DoJump_5454D0()
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
void Char::Teleport_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    field_58_flags_bf.bTeleported = true;
    field_A4_xpos = xpos;
    field_A8_ypos = ypos;
    field_AC_zpos = zpos;
}

MATCH_FUNC(0x545570)
s32 Char::IsOnWater_545570() const
{
    return field_80_sprite_ptr->IsOnWater_59E1D0();
}

// 9.6f 0x493780
MATCH_FUNC(0x545580)
Fix16_Point Char::GetRunVelocity_545580() const
{
    Fix16_Point p;
    p.FromPolar_41E210(-gRunOrJumpSpeed_6FD7D0, field_80_sprite_ptr->field_0_rotation);
    return p;
}

MATCH_FUNC(0x5455f0)
void Char::KillPed_5455F0()
{
    field_7C_pPed->Kill_46F9D0();
}

MATCH_FUNC(0x545600)
void Char::ClearCollisionState_545600()
{
    field_18_collided_entity = NULL;
    field_1C_prev_collided_entity = NULL;
    field_20_collided_entity_type = 0;
    field_2C_ang = kAng0_6FDB34;
    field_69_is_colliding_with_sprite = false;
    field_24_slide_state = 0;
    field_28_slide_rotation = kAng0_6FDB34;
    field_2A_slide_turn = kAng0_6FDB34;
    field_2C_ang = kAng0_6FDB34;
}

MATCH_FUNC(0x545640)
void Char::GetTileFracX64_545640(Fix16 coord, s16* output) const
{
    *output = (coord.GetFracValue() * Fix16(64)).ToInt();
}

MATCH_FUNC(0x545670)
void Char::GetTileFracY64_545670(Fix16 coord, s16* output) const
{
    *output = (coord.GetFracValue() * Fix16(64)).ToInt();
}

MATCH_FUNC(0x5456a0)
void Char::InitSprite_5456A0()
{
    Sprite* pFirst = gSpritePool_703818->get_new_sprite();
    field_80_sprite_ptr = pFirst;
    pFirst->SetType_4206F0(sprite_types_enum::ped_3);
    field_80_sprite_ptr->AllocCollisionShape_59F950(gCharB4_WorldCollisionOffset_6FD8D8, gCharB4_WorldCollisionOffset_6FD8D8, kFP16Quarter_6FD7A4);
    field_80_sprite_ptr->field_8_char_b4_ptr = this;
    field_80_sprite_ptr->CreateSoundObj_5A29D0();
}

MATCH_FUNC(0x545700)
bool Char::IsOnScreen_545700() const
{
    return gGame_0x40_67E008->IsSpriteInAnyPlayerBounds_4B97E0(this->field_80_sprite_ptr, kFP16Zero_6FD9E4) == 1;
}

MATCH_FUNC(0x545720)
void Char::Update_545720(Fix16 distance_to_target)
{
    if (++gCharB4_UpdateCounter_6FDB48 > 20)
    {
        gCharB4_UpdateCounter_6FDB48 = 0;
    }
    if (field_4A_timeout > 0)
    {
        field_4A_timeout--;
    }
    gCharB4_DistanceToTarget_6FD80C = distance_to_target;

    if (field_5C_jump_over_timer > 0)
    {
        field_5C_jump_over_timer--;
    }
    ResetCharUpdateGlobals_544F70();
    gCharB4_Saved_Xpos_6FD7F8 = field_80_sprite_ptr->field_14_xy.x;
    gCharB4_Saved_Ypos_6FD800 = field_80_sprite_ptr->field_14_xy.y;
    gCharB4_Saved_Zpos_6FD7FC = field_80_sprite_ptr->field_1C_zpos;
    gCharB4_Saved_SlopeGradDir_6FD7B0 = Fix16(field_45_slope_gradient_direction);
    byte_6FDB55 = 0;
    gCharCollisionNoJump_6FDB58 = 0;

    gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(field_80_sprite_ptr);

    if (field_58_flags_bf.bTeleported)
    {
        field_80_sprite_ptr->set_xyz_lazy_420600(field_A4_xpos, field_A8_ypos, field_AC_zpos);
        field_58_flags_bf.bTeleported = false;
    }
    else
    {
        switch (field_8_ped_state_1)
        {
            case ped_state_1::walking_0:
                Char::state_0_54DDF0();
                break;
            case ped_state_1::flee_or_running_1:
                Char::state_1_5504F0();
                break;
            case ped_state_1::findind_path_2:
                Char::state_2_5519F0();
                break;
            case ped_state_1::entering_car_3:
                Char::state_3_551A00();
                break;
            case ped_state_1::exiting_car_4:
                Char::state_4_551B30();
                break;
            case ped_state_1::entering_train_5:
                Char::state_5_551BB0();
                break;
            case ped_state_1::leaving_train_6:
                nullsub_28();
                break;
            case ped_state_1::standing_still_7:
                Char::state_7_551CB0();
                break;
            case ped_state_1::immobilized_8:
                Char::state_8_5520A0();
                break;
            case ped_state_1::dead_9:
                Char::state_9_552E90();
                break;
            default:
                break;
        }
        if (field_A0_force_top_layer)
        {
            if (field_8_ped_state_1 != ped_state_1::dead_9)
            {
                field_80_sprite_ptr->SetDrawLayer_40F7B0(34);
            }
        }
        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            if (field_6C_animation_state != Char_Anim_state::Jumping_5 && field_8_ped_state_1 != ped_state_1::dead_9)
            {
                Char::DoJump_5454D0();
            }
        }
        else
        {
            field_A0_force_top_layer = false;
        }
        Char::UpdateAnimState_546360();

        field_80_sprite_ptr->set_ang_lazy_420690(field_40_rotation);

        if (field_58_flags_bf.bReverseVelocity)
        {
            field_98_velocity_vector.SetFromPolar_41E210(-field_38_velocity, field_40_rotation);
        }
        else
        {
            field_98_velocity_vector.SetFromPolar_41E210(field_38_velocity, field_40_rotation);
        }
    }
    gSpriteGrid_1_679208->AddToRegionBuckets_477B20(field_80_sprite_ptr);
    if (field_88_obj_2c.field_0_p18)
    {
        field_88_obj_2c.PoolUpdate_5A6F70(field_80_sprite_ptr);
    }
    if (field_6A_repulsion_timer > 0)
    {
        field_6A_repulsion_timer--;
    }
}

MATCH_FUNC(0x5459c0)
void Char::CheckAndHandleCollisions_5459C0()
{
    gCharB4_HitByMine_6FDB59 = 0;
    gSpriteGrid_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);
}

MATCH_FUNC(0x5459e0)
void Char::DrownPed_5459E0()
{
    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::sinking_20);
    field_16_state_init_pending = true;

    gParticle_8_6FD5E8->EmitWaterSplash_53F060(field_80_sprite_ptr->field_14_xy.x,
                                               field_80_sprite_ptr->field_14_xy.y,
                                               field_80_sprite_ptr->field_1C_zpos,
                                               kAng180_6FD936 + field_80_sprite_ptr->field_0_rotation,
                                               1);
    field_7C_pPed->SetVoiceEventIfNotOnFire_433DD0(28);
    s32 ped_killer_id = field_7C_pPed->field_204_killer_id;
    if (ped_killer_id)
    {
        if (gPedManager_6787BC->PedById(ped_killer_id))
        {
            field_7C_pPed->field_290_death_cause = ped_death_cause::drowned_5;
            field_7C_pPed->field_264_killer_id_timer = 50;
        }
    }
}

// 9.6f 0x497DF0
// By-value angle, like TurnBy16Deg_548BD0: the copy makes VC6 load field_40 first and add kAng180
static inline Ang16 TurnBy180Deg_546360(Ang16 a)
{
    return a + kAng180_6FD936;
}

// Frame delay of the lying-on-floor animations (cases 15/16): an if/return inline returning u8
// gives the original's byte compare with the delay in al and field_68 kept in cl
static inline u8 FrameDelay_546360(Char* pThis)
{
    if (pThis->field_68_animation_frame > 5)
    {
        return 3;
    }
    return 4;
}

MATCH_FUNC(0x546360)
void Char::UpdateAnimState_546360()
{
    s16 newId = 0;
    u8 frame_limit = 0;

    field_70_frame_timer++;

    Fix16 groundZ = kFP16Zero_6FD9E4;
    Fix16 newx = kFP16Zero_6FD9E4;
    Fix16 newy = kFP16Zero_6FD9E4;

    // Declared here, before the switch, to get the original stack layout
    u8 weapon_ofs = 0;
    bool bThrowWeapon = false;
    s32 baseId;

    switch (field_7C_pPed->field_26C_graphic_type)
    {
        case ped_graphic_type::civilian_0:
            baseId = 0;
            break;
        case ped_graphic_type::character_1:
            baseId = 158;
            break;
        case ped_graphic_type::cop_2:
            baseId = 316;
            break;
        default:
            baseId = 158;
            break;
    }

    if (field_7C_pPed->get_remap_433BA0() > -1)
    {
        field_80_sprite_ptr->SetRemap(static_cast<u8>(field_7C_pPed->get_remap_433BA0()));
    }
    else
    {
        field_80_sprite_ptr->SetPaletteSprites_420700();
    }

    if (field_7C_pPed->field_15C_player && field_C_ped_state_2 != ped_state_2::falling_19)
    {
        if (field_7C_pPed->field_170_selected_weapon &&
            (field_7C_pPed->field_170_selected_weapon->field_1C_weapon_type == weapon_type::molotov ||
             field_7C_pPed->field_170_selected_weapon->field_1C_weapon_type == weapon_type::grenade))
        {
            bThrowWeapon = true;
        }

        if (field_7C_pPed->field_170_selected_weapon && !field_7C_pPed->field_21C_bf.bAttacking)
        {
            if ((field_7C_pPed->field_170_selected_weapon->field_1C_weapon_type == weapon_type::molotov ||
                 field_7C_pPed->field_170_selected_weapon->field_1C_weapon_type == weapon_type::grenade) &&
                field_7C_pPed->field_15C_player->field_8D_attack_changed)
            {
                field_6C_animation_state = Char_Anim_state::Punching_4;
                field_68_animation_frame = 0;
            }
        }
        else if (field_7C_pPed->field_170_selected_weapon && field_7C_pPed->field_21C_bf.bAttacking)
        {
            if (field_7C_pPed->field_170_selected_weapon->field_1C_weapon_type == weapon_type::molotov ||
                field_7C_pPed->field_170_selected_weapon->field_1C_weapon_type == weapon_type::grenade)
            {
                field_7C_pPed->field_21C_bf.bAttacking = false;
            }
        }
    }
    else
    {
        if (field_7C_pPed->field_170_selected_weapon && field_7C_pPed->field_21C_bf.bAttacking &&
            (field_7C_pPed->field_170_selected_weapon->field_1C_weapon_type == weapon_type::molotov ||
             field_7C_pPed->field_170_selected_weapon->field_1C_weapon_type == weapon_type::grenade))
        {
            field_6C_animation_state = Char_Anim_state::Punching_4;
            bThrowWeapon = true;
        }
    }

    switch (field_6C_animation_state)
    {
        case Char_Anim_state::Walking_0:
        {
            if (static_cast<u8>(field_70_frame_timer) > 2)
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

        case Char_Anim_state::Running_1:
        {
            if (static_cast<u8>(field_70_frame_timer) > 1)
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

        case Char_Anim_state::Standing_2:
            if (field_7C_pPed->field_21C_bf.bAttacking == 1)
            {
                newId = baseId + 139;
            }
            else
            {
                u8 smoke_ofs;
                if (field_10_char_state != Char_B4_state::Smoking_35)
                {
                    if (static_cast<u8>(field_70_frame_timer) > 8)
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
                    if (static_cast<u8>(field_70_frame_timer) > limit)
                    {
                        field_68_animation_frame++;
                        if (field_68_animation_frame > 7)
                        {
                            field_68_animation_frame = 0;
                            field_10_char_state = Char_B4_state::Standing_7;
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

        case Char_Anim_state::Punching_4:
            if (field_38_velocity > kZeroVelocity_6FD7C0)
            {
                if (static_cast<u8>(field_70_frame_timer) > 2)
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
                if (static_cast<u8>(field_70_frame_timer) > 2)
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
        case Char_Anim_state::Carjacking_9:
        {
            Car* pCar = field_84_target_car;
            pCar->field_76_last_seen_timer = 0;
            switch (field_68_animation_frame)
            {
                case 0:
                {
                    Car_Door* pDoor = field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60());
                    if (field_58_flags_bf.bFlag4)
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
                    if (field_58_flags_bf.bFlag4)
                    {
                        frame_limit = 1;
                        CarDoorAlignmentSolver_545AF0(7 - field_68_animation_frame,
                                                      pCar,
                                                      field_7C_pPed->get_target_car_door_403A60(),
                                                      newx,
                                                      newy,
                                                      field_40_rotation);
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(9);
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
                    if (field_58_flags_bf.bFlag4)
                    {
                        frame_limit = 1;
                        CarDoorAlignmentSolver_545AF0(7 - field_68_animation_frame,
                                                      pCar,
                                                      field_7C_pPed->get_target_car_door_403A60(),
                                                      newx,
                                                      newy,
                                                      field_40_rotation);
                        newId = baseId + field_68_animation_frame;
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(9);
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
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(9);
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
                    field_80_sprite_ptr->SetDrawLayer_40F7B0(9);
                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
                    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                    field_C_ped_state_2 = ped_state_2::ped2_driving_10;
                    field_8_ped_state_1 = ped_state_1::in_car_10;
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
                            case ped_ocupation_enum::dummy_driver_4:
                                pDriver = gPedManager_6787BC->SpawnRunAwayGuy_470D60();
                                if (pCar->field_84_car_info_idx == car_model_enum::apc ||
                                    pCar->field_84_car_info_idx == car_model_enum::JEEP ||
                                    pCar->field_84_car_info_idx == car_model_enum::TANK)
                                {
                                    pDriver->field_26C_graphic_type = ped_graphic_type::cop_2;
                                    pDriver->set_remap_433B90(ped_remap_enum::ped_remap_army);
                                }
                                pDriver->SetStolenCar_492CB0(pCar);
                                pDriver->field_180_car_thief = field_7C_pPed;
                                break;

                            case ped_ocupation_enum::driver:
                                pDriver->set_occupation_403970(ped_ocupation_enum::robbed_driver_10);
                                pDriver->SetStolenCar_492CB0(pCar);
                                pDriver->field_180_car_thief = field_7C_pPed;
                                break;

                            case ped_ocupation_enum::police:
                                if (field_7C_pPed->field_20A_wanted_points < cop_level_ped_enum::cop_1_stars)
                                {
                                    gPolice_7B8_6FEE40->RegisterCriminal_56F940(field_7C_pPed);
                                    field_7C_pPed->field_20A_wanted_points = cop_level_ped_enum::cop_1_stars;
                                }
                                gPolice_7B8_6FEE40->UpdateLastSeenCoordsForCriminal_5708C0(field_7C_pPed);
                                if (pDriver->get_objective_403A80() == objectives_enum::patrol_in_car_43)
                                {
                                    gPolice_7B8_6FEE40->TryAssignCarCrewToCriminal_5707B0(pDriver->field_16C_car, field_7C_pPed);
                                }
                                pDriver->SetObjective(objectives_enum::no_obj_0);
                                break;

                            default:
                                if (pDriver->field_17C_pGang)
                                {
                                    pDriver->SetObjective2_463830(objectives_enum::kill_char_on_foot_20);
                                    pDriver->SetInternalTargetPed_403AE0(field_7C_pPed);
                                }
                                pDriver->SetStolenCar_492CB0(pCar);
                                pDriver->field_180_car_thief = field_7C_pPed;
                                break;
                        }

                        if (field_7C_pPed->GetInternalObjective_403A90() == objectives_enum::enter_car_as_driver_35 && field_7C_pPed->GetInternalObjectiveStatus_4039D0() == objective_status::passed_1)
                        {
                            field_7C_pPed->SetInternalObjectiveStatus_403B50(objective_status::not_finished_0);
                        }
                        pDriver->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                        pDriver->ChangeNextPedState2_45C540(ped_state_2::ped2_at_car_door_17);
                        pCar->ClearDriver_4407F0();
                        CarDoorAlignmentSolver_545AF0(8, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        pDriver->AllocCharB4_45C830(
                            newx,
                            newy,
                            gMap_0x370_6F6268->FindGroundZAboveCoord_4E4E50(newx, newy, pCar->field_50_car_sprite->field_1C_zpos));
                        pDriver->field_168_game_object->set_rotation_433A30(pCar->field_50_car_sprite->field_0_rotation);
                        pDriver->field_168_game_object->Set_F8_ped_state_1_433910(ped_state_1::immobilized_8);
                        pDriver->set_target_car_door_403A70(field_7C_pPed->get_target_car_door_403A60());
                        pDriver->field_168_game_object->SetPedState2_433A50(ped_state_2::ped2_at_car_door_17);
                        pDriver->field_168_game_object->field_84_target_car = pCar;
                        pDriver->field_168_game_object->field_80_sprite_ptr->SetDrawLayer_40F7B0(6);
                        pDriver->SetRemap_433C10(pDriver->get_remap_433BA0());
                        pDriver->field_16C_car = NULL;
                        pDriver->SetStateInitPending_433B50();
                        if (!pDriver->is_player_41B0A0())
                        {
                            pDriver->SetVoiceEventIfNotOnFire_433DD0(12);
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
                gMap_0x370_6F6268->FindGroundZAboveCoord_4E4E50(newx, newy, pCar->field_50_car_sprite->field_1C_zpos));

            if (field_58_flags_bf.bFlag4)
            {
                field_40_rotation = TurnBy180Deg_546360(field_40_rotation);
            }

            if (static_cast<u8>(field_70_frame_timer) > frame_limit)
            {
                field_68_animation_frame++;
                field_70_frame_timer = 0;
                if (field_68_animation_frame == 13)
                {
                    field_68_animation_frame = 4;
                }
            }
            else if (field_70_frame_timer == 5 && field_68_animation_frame == 0)
            {
                Car_Door* pDoor = field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60());
                if (!field_58_flags_bf.bFlag4)
                {
                    pDoor->Open_439E60();
                    pDoor->set_ped_421380(field_7C_pPed);
                }
            }
            break;
        }

        case Char_Anim_state::Getting_pulled_out_of_car_8:
        {
            Car* pCar = field_84_target_car;
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
            field_80_sprite_ptr->SetDrawLayer_40F7B0(6);
            gCharB4_GradientSlopeBelow_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                      field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                      field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);
            ManageZCoordAndSlopes_548590();
            if (static_cast<u8>(field_70_frame_timer) > 2)
            {
                field_70_frame_timer = 0;
                field_68_animation_frame++;
            }
            newId = baseId + 36;
            break;
        }

        case Char_Anim_state::Exiting_Car_7:
        {
            Car* pCar = field_84_target_car;
            pCar->field_76_last_seen_timer = 0;
            switch (field_68_animation_frame)
            {
                case 0:
                    if (field_58_flags_bf.bFlag4)
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
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(9);
                        newId = baseId + 36;
                    }
                    break;

                case 1:
                    if (field_58_flags_bf.bFlag4)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(1, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(6, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 3;
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(9);
                        newId = baseId + 35;
                    }
                    break;

                case 2:
                    if (field_58_flags_bf.bFlag4)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(2, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(5, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 3;
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(9);
                        newId = baseId + 34;
                    }
                    break;

                case 3:
                    if (field_58_flags_bf.bFlag4)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(3, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(4, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 3;
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(9);
                        newId = baseId + 33;
                    }
                    break;

                case 4:
                    if (field_58_flags_bf.bFlag4)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(4, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(0, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(23);
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 5:
                    if (field_58_flags_bf.bFlag4)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(5, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(1, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(23);
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 6:
                    if (field_58_flags_bf.bFlag4)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(6, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(2, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(23);
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 7:
                    if (field_58_flags_bf.bFlag4)
                    {
                        newId = baseId + field_68_animation_frame;
                        frame_limit = 0;
                        CarDoorAlignmentSolver_545AF0(7, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                    }
                    else
                    {
                        CarDoorAlignmentSolver_545AF0(3, pCar, field_7C_pPed->get_target_car_door_403A60(), newx, newy, field_40_rotation);
                        frame_limit = 1;
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(23);
                        newId = field_68_animation_frame + baseId + 24;
                    }
                    break;

                case 8:
                {
                    field_80_sprite_ptr->SetDrawLayer_40F7B0(23);
                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::walking_0);
                    field_C_ped_state_2 = ped_state_2::ped2_walking_0;
                    field_8_ped_state_1 = ped_state_1::walking_0;
                    Car_Door* pDoor = field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60());
                    if (!field_58_flags_bf.bFlag4)
                    {
                        pDoor->Close_439EA0();
                    }
                    pDoor->set_ped_421380(NULL);

                    if (static_cast<u8>(IsOnWater_545570()))
                    {
                        field_7C_pPed->PutOutFire();
                        DrownPed_5459E0();
                    }
                    else
                    {
                        gCharB4_GradientSlopeBelow_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
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
                gMap_0x370_6F6268->FindGroundZAboveCoord_4E4E50(newx, newy, pCar->field_50_car_sprite->field_1C_zpos));

            if (static_cast<u8>(field_70_frame_timer) > frame_limit)
            {
                field_68_animation_frame++;
                field_70_frame_timer = 0;
                if (field_68_animation_frame == 13)
                {
                    field_68_animation_frame = 4;
                }
            }
            else if (field_70_frame_timer == 1 && field_68_animation_frame == 0)
            {
                Car_Door* pDoor = field_84_target_car->GetDoor(field_7C_pPed->get_target_car_door_403A60());
                if (!field_58_flags_bf.bFlag4)
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
                    field_10_char_state = Char_B4_state::Walking_1;
                    field_6C_animation_state = Char_Anim_state::Standing_2;
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
                    gCharB4_Saved_Zpos_6FD7FC += kFpPoint05_6FDAE4;
                    gCharB4_Saved_Zpos_6FD7FC = SnapZTo16_4F79B0(gCharB4_Saved_Zpos_6FD7FC);
                    if (field_58_flags_bf.bOnSlope)
                    {
                        gCharB4_GradientSlopeBelow_6FDB54 = 0;
                    }
                    ManageZCoordAndSlopes_548590();
                    break;

                case 0:
                    field_54_jump_scale_counter = 0;
                    gCharB4_Saved_Zpos_6FD7FC += kFpPoint05_6FDAE4;
                    gCharB4_Saved_Zpos_6FD7FC = SnapZTo16_4F79B0(gCharB4_Saved_Zpos_6FD7FC);
                    if (field_58_flags_bf.bOnSlope)
                    {
                        gCharB4_GradientSlopeBelow_6FDB54 = 0;
                    }
                    ManageZCoordAndSlopes_548590();
                    break;
            }

            field_54_jump_scale_counter++;
            s16 jumpId = field_68_animation_frame + baseId + 16;
            if (static_cast<u8>(field_70_frame_timer) > static_cast<u8>(field_71_frame_delay))
            {
                field_70_frame_timer = 0;
                field_68_animation_frame++;
            }
            field_80_sprite_ptr->set_id_lazy_4206C0(197);
            field_80_sprite_ptr->set_id_lazy_4206C0(jumpId);

            if (static_cast<u8>(field_54_jump_scale_counter) < 12)
            {
                field_80_sprite_ptr->ApplyScaleToDimensions_59E4C0(kFP16One_6FD9E8 + Fix16(static_cast<u8>(field_54_jump_scale_counter)) * kFpPoint03_6FD9A8,
                                                                   0);
            }
            else if (static_cast<u8>(field_54_jump_scale_counter) < 24)
            {
                field_80_sprite_ptr->ApplyScaleToDimensions_59E4C0(kFP16One_6FD9E8 +
                                                                       Fix16(25 - static_cast<u8>(field_54_jump_scale_counter)) * kFpPoint03_6FD9A8,
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

            if (static_cast<u8>(field_70_frame_timer) > static_cast<u8>(field_71_frame_delay))
            {
                if (field_68_animation_frame < 16)
                {
                    field_68_animation_frame++;
                }
                field_70_frame_timer = 0;
            }
            break;

        case Char_Anim_state::Lying_On_Floor_10:
            newId = baseId + 72;
            break;

        case Char_Anim_state::Lying_Pose_14:
            newId = baseId + 80;
            break;

        case Char_Anim_state::Lying_Pose_13:
            newId = baseId + 72;
            break;

        case Char_Anim_state::Lying_Pose_19:
            newId = baseId + 156;
            break;

        case Char_Anim_state::Lying_Pose_20:
            newId = baseId + 157;
            break;

        case Char_Anim_state::Charred_Corpse_21:
            newId = baseId + 155;
            break;

        case Char_Anim_state::Knocked_Down_Hit_Front_15:
            if (static_cast<u8>(field_70_frame_timer) > FrameDelay_546360(this))
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7)
                {
                    switch (gRng_6F6784.get_int_4F7AE0(2))
                    {
                        case 0:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_19;
                            break;
                        case 1:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_13;
                            break;
                    }
                    field_68_animation_frame = 7;
                }
            }
            newId = field_68_animation_frame + baseId + 65;
            break;

        case Char_Anim_state::Knocked_Down_Hit_Behind_16:
            if (static_cast<u8>(field_70_frame_timer) > FrameDelay_546360(this))
            {
                field_68_animation_frame++;
                if (field_68_animation_frame > 7)
                {
                    switch (gRng_6F6784.get_int_4F7AE0(3))
                    {
                        case 0:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_14;
                            break;
                        case 1:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_19;
                            break;
                        case 2:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_20;
                            break;
                    }
                    field_68_animation_frame = 7;
                }
            }
            newId = field_68_animation_frame + baseId + 73;
            break;

        case Char_Anim_state::Electrocuted_17:
            if (static_cast<u8>(field_70_frame_timer) > 3)
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
            field_80_sprite_ptr->SetDrawLayer_40F7B0(6);
            newId = field_68_animation_frame + baseId + 151;
            break;
    }

    field_80_sprite_ptr->set_id_lazy_4206C0(newId);
}

MATCH_FUNC(0x548590)
void Char::ManageZCoordAndSlopes_548590()
{
    Fix16 zpos = kFP16Zero_6FD9E4;
    zpos = field_80_sprite_ptr->field_1C_zpos;

    if (gCharB4_GradientSlopeBelow_6FDB54 == 1)
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
            zpos = zpos.GetFloorValue();

            if (frac > kFpHalf_6FDB28)
            {
                zpos++;
            }
        }
        else
        {
            field_58_flags_bf.bOnSlope = false;
            zpos = Fix16(zpos.ToUInt8());
        }
        field_45_slope_gradient_direction = 0;
    }
    else
    {
        field_45_slope_gradient_direction = gradient_direction;
        field_58_flags_bf.bOnSlope = true;
    }
    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y, zpos);
}

DEFINE_GLOBAL_INIT(Ang16, kAng16_6FD888, Ang16(64), 0x6FD888);
DEFINE_GLOBAL_INIT(Fix16, kFpOne128th_6FD860, Fix16(0x80, 0), 0x6FD860);

// The angle is a by-value parameter: the original keeps it in memory and adds it as a dword
static inline Ang16 TurnBy16Deg_548BD0(Ang16 angle)
{
    return kAng16_6FD888 + angle;
}

// 9.6f 0x499F00
MATCH_FUNC(0x548670)
void Char::DispatchCollision_548670(char_type bIgnorePeds)
{
    u8 bIgnorePedHit;
    if (this->field_69_is_colliding_with_sprite == 1 && this->field_20_collided_entity_type && this->field_24_slide_state != 3)
    {
        bIgnorePedHit = 1;
    }
    else
    {
        bIgnorePedHit = bIgnorePeds;
    }

    Sprite* pNearSprite = field_80_sprite_ptr->QuerySpriteCollision_59E7D0(sprite_types_enum::car_2);
    if (pNearSprite)
    {
        s32 sprite_type_enum = pNearSprite->field_30_sprite_type_enum;
        switch (pNearSprite->GetType_416B40())
        {

            case sprite_types_enum::ped_3:
                if (bIgnorePedHit == byte_623F48)
                {
                    this->field_18_collided_entity = NULL;
                }
                else
                {
                    if (field_7C_pPed->GetPedType_420B70() >= ped_type::player_2 && field_7C_pPed->field_238_ped_type <= ped_type::dummy_with_occupation_6)
                    {
                        Char::HandlePedCollision_548BD0(pNearSprite->AsCharB4_40FEA0());
                        this->field_18_collided_entity = NULL;
                    }
                }
                break;

            case sprite_types_enum::car_2:
            {
                Sprite* pNearSprite2 = field_80_sprite_ptr->QuerySpriteCollision_59E7D0(sprite_types_enum::object_1);
                if (pNearSprite2->GetType_416B40() == sprite_types_enum::object_1)
                {
                    if (pNearSprite2->AsObject2C_40FEC0())
                    {
                        Char::HandleObjectCollision_548840(pNearSprite2->AsObject2C_40FEC0());
                        this->field_20_collided_entity_type = 3;
                    }
                }
                else
                {
                    Char::HandleGenericCollision_54A530(pNearSprite->AsCar_40FEB0(), NULL, NULL);
                    this->field_20_collided_entity_type = 1;
                }
                break;
            }
            case sprite_types_enum::object_1:
            case sprite_types_enum::code_obj1_4:
            case sprite_types_enum::map_obj_5:
                Char::HandleObjectCollision_548840(pNearSprite->AsObject2C_40FEC0());
                this->field_20_collided_entity_type = 3;
                break;

            default:
                return;
        }
    }
    else
    {
        if (field_10_char_state != Char_B4_state::Avoiding_Ped_10)
        {
            if (field_10_char_state == Char_B4_state::Colliding_With_Car_27)
            {
                this->field_10_char_state = Char_B4_state::Walking_1;
                this->field_1C_prev_collided_entity = NULL;
                this->field_20_collided_entity_type = 0;
                this->field_18_collided_entity = NULL;
            }
            else
            {
                this->field_18_collided_entity = NULL;
            }
        }
        else
        {
            this->field_10_char_state = Char_B4_state::Walking_1;
            this->field_18_collided_entity = NULL;
        }

        if (field_38_velocity == kZeroVelocity_6FD7C0 || field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) && !this->field_8_ped_state_1)
        {
            this->field_1C_prev_collided_entity = NULL;
            this->field_20_collided_entity_type = 0;
            this->field_69_is_colliding_with_sprite = false;
            this->field_2A_slide_turn = kAng0_6FDB34;
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
    return Ang16(reinterpret_cast<Ang16&>(sum), 0);
}

static inline bool CanJumpOver_54A530(Char* pThis, Char* pChar)
{
    if (!pThis->field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2))
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

MATCH_FUNC(0x548840)
void Char::HandleObjectCollision_548840(Object* pObj)
{
    Fix16_Point new_pos;
    Fix16_Point point;
    // Unused: the original enters with EH state 4, so three more Fix16_Point locals were constructed up front
    Fix16_Point unused_1, unused_2, unused_3;
    u8 side_self;
    u8 out2;
    u8 out3 = 0;
    bool bUnknown = false;

    s32 phi_type = pObj->field_8_definition->field_34_behavior_type;

    if (!pObj->UsesParamAsPedRef_421080() || (pObj->get_field_26_420FF0()) == 0 ||
        pObj->get_field_26_420FF0() != this->field_7C_pPed->field_267_ped_ref_idx)
    {
        if (!pObj->CanBePushed_482C90())
        {
            goto generic_collision;
        }

        if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2))
        {
            if (this->field_10_char_state == Char_B4_state::Jumping_15)
            {
                goto generic_collision;
            }
        }
        else
        {
            if (pObj->field_8_definition->field_4C_collision_response == 3)
            {
                bUnknown = true;
            }

            if (pObj->field_8_definition->field_4C_collision_response && !bUnknown)
            {
                goto generic_collision;
            }
        }

        if (pObj->GetMass_482C80() < kFpHalf_6FDAB0)
        {

            field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);

            point = field_80_sprite_ptr
                        ->FindCollisionIntersectionPoint_5A2710(pObj->field_4_sprite, new_pos, field_80_sprite_ptr->field_0_rotation, side_self, out2, out3);
            pObj->ResolveCollisionWithPed_5229B0(this, &point, 1);
            return;
        }
        this->field_5C_jump_over_timer = 10;

    // shared exit for the three not-pushable checks: goto-free forms change the code
    generic_collision:
        HandleGenericCollision_54A530(NULL, pObj, NULL);
    }
}

// 9.6f 0x41FA70
static inline Ang16& compound_add_41FA70(Ang16& a, const Ang16& b)
{
    a.rValue += b.rValue;
    a.Normalize();
    return a;
}

// 9.6f 0x493040 (a Ped method there)
static inline void CollideWithElvis_493040(Ped* pPed, Char* pChar)
{
    if (reinterpret_cast<s32>(pChar) != pPed->field_138_collided_char_b4)
    {
        pPed->field_138_collided_char_b4 = reinterpret_cast<s32>(pChar);
        pPed->field_224_bf.bCollidedWithPed = false;
        pChar->field_7C_pPed->field_138_collided_char_b4 = reinterpret_cast<s32>(pPed->field_168_game_object);
        pPed->SetVoiceEventIfNotOnFire_433DD0(ped_voice_event::elvis_bumped_22);
    }
}

// 9.6f 0x493000 (a Ped method there)
static inline void CollideWithPed_493000(Ped* pPed, Char* pOther)
{
    if (reinterpret_cast<s32>(pOther) != pPed->field_138_collided_char_b4)
    {
        pPed->field_138_collided_char_b4 = reinterpret_cast<s32>(pOther);
        pPed->field_224_bf.bCollidedWithPed = true;
        pOther->field_7C_pPed->field_138_collided_char_b4 = reinterpret_cast<s32>(pPed->field_168_game_object);
    }
}

// What happens when this ped walks into pOther, by the types of both peds (field_238)
MATCH_FUNC(0x548bd0)
void Char::HandlePedCollision_548BD0(Char* pOther)
{
    s32 my_type = field_7C_pPed->GetPedType_420B70();
    s32 other_type = pOther->field_7C_pPed->GetPedType_420B70();
    if (pOther->get_ped_state_1_48A4C0() == ped_state_1::dead_9 || pOther->get_ped_state_1_48A4C0() == ped_state_1::immobilized_8 || field_58_flags_bf.bFlag7 || field_10_char_state == Char_B4_state::Jumping_15)
    {
        return;
    }

    if (field_7C_pPed->is_player_41B0A0() && pOther->field_7C_pPed->Is_occupation_elvis_433C90())
    {
        CollideWithElvis_493040(pOther->field_7C_pPed, field_7C_pPed->field_168_game_object);
    }
    else if (field_7C_pPed->is_player_41B0A0() || !(field_7C_pPed->field_200_id & 3))
    {
        if (!pOther->field_7C_pPed->is_player_41B0A0())
        {
            CollideWithPed_493000(field_7C_pPed, pOther);
        }
    }

    switch (my_type)
    {
        case ped_type::player_2:
            switch (other_type)
            {
                case ped_type::script_created_5:
                    if (field_7C_pPed->field_164_ped_group && pOther->field_7C_pPed->field_164_ped_group == field_7C_pPed->field_164_ped_group)
                    {
                        break;
                    }
                    if (pOther->field_7C_pPed->get_occupation_403980() != ped_ocupation_enum::stand_still_bloke)
                    {
                        pOther->field_6A_repulsion_timer = 4;
                        pOther->field_74_repulsion_rotation = Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x) + kAng180_6FD936;
                    }
                    else
                    {
                        field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
                    }
                    break;
                case ped_type::player_2:
                    field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
                    break;
                case ped_type::dummy_3:
                case ped_type::special_ped_4:
                case ped_type::dummy_with_occupation_6:
                    field_10_char_state = Char_B4_state::Walking_1;
                    if (field_38_velocity == kZeroVelocity_6FD7C0)
                    {
                        break;
                    }
                    if (field_7C_pPed->field_164_ped_group && pOther->field_7C_pPed->field_164_ped_group == field_7C_pPed->field_164_ped_group)
                    {
                        break;
                    }
                    pOther->field_6A_repulsion_timer = 4;
                    pOther->field_74_repulsion_rotation = Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x) + kAng180_6FD936;
                    break;
            }
            break;

        case ped_type::script_created_5:
            switch (other_type)
            {
                case ped_type::script_created_5:
                    if (field_10_char_state == Char_B4_state::Avoiding_Ped_10 || field_7C_pPed->get_id() >= pOther->field_7C_pPed->get_id())
                    {
                        field_10_char_state = Char_B4_state::Walking_1;
                    }
                    else if (!field_7C_pPed->field_164_ped_group || field_7C_pPed->field_164_ped_group->field_30_panicking)
                    {
                        if (field_8_ped_state_1 != ped_state_1::entering_car_3)
                        {
                            if (field_7C_pPed->GetInternalObjective_403A90() != objectives_enum::objective_11)
                            {
                                compound_add_41FA70(field_40_rotation, kAng16_6FD888);
                                field_10_char_state = Char_B4_state::Avoiding_Ped_10;
                            }
                            else
                            {
                                field_10_char_state = Char_B4_state::Avoiding_Ped_10;
                            }
                        }
                        else
                        {
                            field_38_velocity = kZeroVelocity_6FD7C0;
                        }
                    }
                    break;
                case ped_type::player_2:
                    // Nothing to do, but the original's jump table starts at 2 (an empty case is dropped)
                    return;
                case ped_type::dummy_3:
                case ped_type::special_ped_4:
                case ped_type::dummy_with_occupation_6:
                    pOther->field_6A_repulsion_timer = 4;
                    pOther->field_74_repulsion_rotation = Fix16::atan2_fixed_405320(get_sprite_ptr_4338D0()->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x) + kAng180_6FD936;
                    break;
            }
            break;

        case ped_type::special_ped_4:
        case ped_type::dummy_with_occupation_6:
            switch (other_type)
            {
                case ped_type::special_ped_4:
                case ped_type::dummy_with_occupation_6:
                    if (field_10_char_state == Char_B4_state::Avoiding_Ped_10 || field_7C_pPed->get_id() >= pOther->field_7C_pPed->get_id())
                    {
                        field_10_char_state = Char_B4_state::Walking_1;
                        // A return, not a break: the original keeps the type 5 copy of this tail
                        return;
                    }
                    else if (!field_7C_pPed->field_164_ped_group || field_7C_pPed->field_164_ped_group->field_30_panicking)
                    {
                        if (field_8_ped_state_1 != ped_state_1::entering_car_3)
                        {
                            if (field_7C_pPed->GetInternalObjective_403A90() != objectives_enum::objective_11)
                            {
                                compound_add_41FA70(field_40_rotation, kAng16_6FD888);
                                field_10_char_state = Char_B4_state::Avoiding_Ped_10;
                            }
                            else
                            {
                                field_10_char_state = Char_B4_state::Avoiding_Ped_10;
                            }
                        }
                    }
                    break;
                case ped_type::player_2:
                    if (pOther->field_7C_pPed->field_20A_wanted_points >= 1)
                    {
                        field_7C_pPed->IsLawEnforcement_45B4E0();
                    }
                    break;
                case ped_type::dummy_3:
                    pOther->field_6A_repulsion_timer = 4;
                    pOther->field_74_repulsion_rotation = Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x) + kAng180_6FD936;
                    break;
            }
            break;

        case ped_type::dummy_3:
            if (field_7C_pPed->GetInternalObjective_403A90() == objectives_enum::crossing_road_48 || field_7C_pPed->GetInternalObjective_403A90() == objectives_enum::enter_train_37 ||
                field_7C_pPed->GetInternalObjective_403A90() == objectives_enum::leave_train_38 || field_7C_pPed->GetInternalObjective_403A90() == objectives_enum::goto_area_on_foot_12)
            {
                field_38_velocity = kFpThreeSixtyFourths_6FD7CC;
                break;
            }
            switch (other_type)
            {
                case ped_type::player_2:
                    if (pOther->field_7C_pPed->field_20A_wanted_points >= 1 && field_7C_pPed->IsLawEnforcement_45B4E0())
                    {
                        break;
                    }
                    if (pOther->field_38_velocity != kZeroVelocity_6FD7C0)
                    {
                        field_38_velocity = kFpOne128th_6FD860;
                    }
                    else
                    {
                        field_40_rotation = Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x);
                    }
                    break;
                case ped_type::script_created_5:
                    if (pOther->field_38_velocity != kZeroVelocity_6FD7C0)
                    {
                        field_38_velocity = kFpOne128th_6FD860;
                    }
                    else
                    {
                        field_40_rotation = Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x);
                    }
                    break;
                case ped_type::special_ped_4:
                case ped_type::dummy_with_occupation_6:
                    field_38_velocity = kFpOne128th_6FD860;
                    break;
                case ped_type::dummy_3:
                    if (!field_20_collided_entity_type)
                    {
                        pOther->field_6A_repulsion_timer = 4;
                        pOther->field_74_repulsion_rotation = TurnBy16Deg_548BD0(Fix16::atan2_fixed_405320(field_80_sprite_ptr->field_14_xy.y - pOther->field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_14_xy.x - pOther->field_80_sprite_ptr->field_14_xy.x) + kAng180_6FD936);
                    }
                    break;
            }
            break;
    }
}

// https://decomp.me/scratch/ph2wn
// 9.6f 0x4948C0. The 18 Ang16 normalisations are the plain operators (`-pSprt->field_0`, `kAng180 +
// corner_ang`, `pSprt->field_0 - kAng90`): the first 9 go out of line (one Normalize target), the rest
// inline, and the inline budget only cuts there (side 0 out of line, side 1 inline) when the function's
// front-end size is in a window of about 60 nodes (Scripts/inline_budget/inlsim.py --scan). The
// `!= 0` / `== 0` tests, the declared-then-assigned xpos/ypos/target_y, the `default: break;` and the
// explicit `Fix16(...)` copies in the saved-position restores are code-neutral spellings that put it
// there. step_x/step_y are the one pair of function-scope locals for both PolarToCartesian calls (the
// player arm's too): separate block locals flip the player arm's imul operand order. The width/height
// getters (9.6f GetW/GetH) do the same, so those stay field accesses.
MATCH_FUNC(0x54a530)
void Char::HandleGenericCollision_54A530(Car* pCar, Object* pObj, Char* pChar)
{
    Ang16 corner_ang;
    Ang16 hit_ang;
    Sprite* pSprt;
    Fix16 rot_x;
    Fix16 rot_y;
    Fix16 target_x;
    Fix16 target_y;
    Fix16 step_x;
    Fix16 step_y;
    target_y = kFP16Zero_6FD9E4;
    u8 side = 0;
    bool bNoJump = false;

    Fix16 xpos;
    Fix16 ypos;
    xpos = field_80_sprite_ptr->field_14_xy.x;
    ypos = field_80_sprite_ptr->field_14_xy.y;

    if (field_10_char_state != Char_B4_state::Jumping_15)
    {
        field_80_sprite_ptr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos_6FD7F8), Fix16(gCharB4_Saved_Ypos_6FD800), Fix16(gCharB4_Saved_Zpos_6FD7FC));
        field_45_slope_gradient_direction = gCharB4_Saved_SlopeGradDir_6FD7B0.ToInt();
        field_10_char_state = Char_B4_state::Colliding_With_Car_27;
    }

    if (field_69_is_colliding_with_sprite == 0)
    {
        if (pCar != NULL)
        {
            corner_ang = pCar->GetCornerAngle_4403A0();
            pSprt = pCar->field_50_car_sprite;
            if (gGarage_48_6FD26C->IsParkingCar_493540(pCar) != 0)
            {
                bNoJump = true;
                gCharCollisionNoJump_6FDB58 = 1;
            }
        }
        else if (pObj != NULL)
        {
            pSprt = pObj->field_4_sprite;
            Fix16 half_w;
            Fix16 half_h;
            half_w = pSprt->field_C_pCollisionShape->field_0_width / 2;
            half_h = pSprt->field_C_pCollisionShape->field_4_height / 2;
            corner_ang = Fix16::atan2_fixed_405320(half_h, half_w);
            if (pSprt->field_C_pCollisionShape->GetDepth_492170() > kFP16Half_6FD8E4)
            {
                bNoJump = true;
                gCharCollisionNoJump_6FDB58 = 1;
            }
        }
        else
        {
            pSprt = pChar->field_80_sprite_ptr;
            corner_ang = kAng45_6FD89C;
            pSprt->field_0_rotation = kAng0_6FDB34;
        }

        if ((field_7C_pPed->GetJumpOverMode_492C20() == ped_jump_over_mode::can_jump_2 || field_5C_jump_over_timer > 0) && field_10_char_state != Char_B4_state::Jumping_15)
        {
            if (bNoJump == 0)
            {
                if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) == 0)
                {
                    if (pChar == NULL)
                    {
                        Char::DoJump_5454D0();
                    }
                }
                else if (field_8_ped_state_1 == ped_state_1::entering_car_3)
                {
                    Char::DoJump_5454D0();
                }
            }
        }

        if (bNoJump != 0)
        {
            field_2A_slide_turn = kAng90_6FD854;
            if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) != 0)
            {
                if (field_10_char_state != Char_B4_state::Jumping_15 && field_8_ped_state_1 != ped_state_1::entering_car_3)
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos_6FD7F8), Fix16(gCharB4_Saved_Ypos_6FD800), Fix16(gCharB4_Saved_Zpos_6FD7FC));
                    field_45_slope_gradient_direction = gCharB4_Saved_SlopeGradDir_6FD7B0.ToInt();
                    field_10_char_state = Char_B4_state::Colliding_With_Car_27;
                    return;
                }
                else if (field_20_collided_entity_type == 1)
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos_6FD7F8), Fix16(gCharB4_Saved_Ypos_6FD800), Fix16(gCharB4_Saved_Zpos_6FD7FC));
                    field_40_rotation = field_80_sprite_ptr->field_0_rotation + kAng180_6FD936;
                    return;
                }
                else
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos_6FD7F8), Fix16(gCharB4_Saved_Ypos_6FD800), Fix16(gCharB4_Saved_Zpos_6FD7FC));
                    field_40_rotation = pSprt->field_0_rotation;
                    Ang16::PolarToCartesian_41FC20(field_40_rotation, field_38_velocity, step_x, step_y);
                    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + step_x,
                                                             field_80_sprite_ptr->field_14_xy.y + step_y,
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
        else
        {
            if (field_10_char_state == Char_B4_state::Jumping_15)
            {
                if (field_68_animation_frame >= 5)
                {
                    field_68_animation_frame = 5;
                    field_71_frame_delay = 2;
                    field_70_frame_timer = 0;
                }
                field_69_is_colliding_with_sprite = false;

                if (pCar != NULL)
                {
                    if (pCar->field_50_car_sprite->field_1C_zpos >= field_80_sprite_ptr->field_1C_zpos)
                    {
                        field_A0_force_top_layer = true;
                        field_80_sprite_ptr->ResolveZOrder_5A1B30(pCar->field_50_car_sprite);
                    }
                }
                return;
            }
        }

        // Work out which side of the obstacle we hit, in the obstacle's local space
        RotateAndTranslatePoint_42A720(field_80_sprite_ptr->field_14_xy.x,
                                       field_80_sprite_ptr->field_14_xy.y,
                                       -pSprt->field_0_rotation,
                                       pSprt->field_14_xy.x,
                                       pSprt->field_14_xy.y,
                                       rot_x,
                                       rot_y);
        hit_ang = Fix16::atan2_fixed_405320(rot_y, rot_x);

        target_x = field_7C_pPed->GetMoveTargetX_492CE0();
        target_y = field_7C_pPed->GetMoveTargetY_492CF0();
        if (target_x == kFP16MinusOne_6FD790 || target_y == kFP16MinusOne_6FD790)
        {
            Ang16::PolarToCartesian_41FC20(field_40_rotation, kFpFour_6FDA04, target_x, target_y);
            target_x += field_80_sprite_ptr->field_14_xy.x;
            target_y += field_80_sprite_ptr->field_14_xy.y;
        }

        RotateAndTranslatePoint_42A720(target_x, target_y, -pSprt->field_0_rotation, pSprt->field_14_xy.x, pSprt->field_14_xy.y, rot_x, rot_y);
        target_x = pSprt->field_14_xy.x + rot_x;
        target_y = pSprt->field_14_xy.y + rot_y;
        RotateAndTranslatePoint_42A720(xpos, ypos, -pSprt->field_0_rotation, pSprt->field_14_xy.x, pSprt->field_14_xy.y, rot_x, rot_y);
        xpos = pSprt->field_14_xy.x + rot_x;
        ypos = pSprt->field_14_xy.y + rot_y;

        if (hit_ang < -corner_ang)
        {
            side = 3;
            if (hit_ang < kAng180_6FD936 + corner_ang)
            {
                side = 2;
                if (hit_ang < kAng180_6FD936 - corner_ang)
                {
                    side = 1;
                    if (hit_ang < corner_ang)
                    {
                        side = 0;
                        if (target_x < xpos)
                        {
                            field_28_slide_rotation = pSprt->field_0_rotation - kAng90_6FD854;
                            field_2A_slide_turn = kAng90_6FD8A2;
                        }
                        else
                        {
                            field_28_slide_rotation = pSprt->field_0_rotation + kAng90_6FD854;
                            field_2A_slide_turn = kAng270_6FD94C;
                        }
                    }
                    else if (target_y < ypos)
                    {
                        field_28_slide_rotation = pSprt->field_0_rotation + kAng180_6FD936;
                        field_2A_slide_turn = kAng270_6FD94C;
                    }
                    else
                    {
                        field_28_slide_rotation = pSprt->field_0_rotation;
                        field_2A_slide_turn = kAng90_6FD8A2;
                    }
                }
                else if (target_x < xpos)
                {
                    field_28_slide_rotation = pSprt->field_0_rotation - kAng90_6FD854;
                    field_2A_slide_turn = kAng270_6FD94C;
                }
                else
                {
                    field_28_slide_rotation = pSprt->field_0_rotation + kAng90_6FD854;
                    field_2A_slide_turn = kAng90_6FD8A2;
                }
            }
            else if (target_y < ypos)
            {
                if ((field_2A_slide_turn == kAng90_6FD8A2) != 0)
                {
                    field_28_slide_rotation = pSprt->field_0_rotation + kAng180_6FD936;
                    field_2A_slide_turn = kAng90_6FD8A2;
                }
                else
                {
                    field_28_slide_rotation = pSprt->field_0_rotation;
                    field_2A_slide_turn = kAng270_6FD94C;
                }
            }
            else
            {
                if ((field_2A_slide_turn == kAng90_6FD8A2) != 0)
                {
                    field_28_slide_rotation = pSprt->field_0_rotation + kAng180_6FD936;
                    field_2A_slide_turn = kAng90_6FD8A2;
                }
                else
                {
                    field_28_slide_rotation = pSprt->field_0_rotation;
                    field_2A_slide_turn = kAng270_6FD94C;
                }
            }
        }
        else if (target_x < xpos)
        {
            field_28_slide_rotation = pSprt->field_0_rotation - kAng90_6FD854;
            field_2A_slide_turn = kAng90_6FD8A2;
        }
        else
        {
            field_28_slide_rotation = pSprt->field_0_rotation + kAng90_6FD854;
            field_2A_slide_turn = kAng270_6FD94C;
        }

        field_69_is_colliding_with_sprite = true;
        field_80_sprite_ptr->set_xyz_lazy_420600(Fix16(gCharB4_Saved_Xpos_6FD7F8), Fix16(gCharB4_Saved_Ypos_6FD800), Fix16(gCharB4_Saved_Zpos_6FD7FC));

        if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) != 0)
        {
            if (field_7C_pPed->GetInternalObjective_403A90() != objectives_enum::enter_car_as_driver_35 ||
                gGarage_48_6FD26C->IsParkingCar_493540(field_7C_pPed->get_target_to_enter_403B10()) != 0)
            {
                return;
            }
        }

        Ang16::PolarToCartesian_41FC20(field_28_slide_rotation, field_38_velocity, step_x, step_y);
        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + step_x,
                                                 field_80_sprite_ptr->field_14_xy.y + step_y,
                                                 field_80_sprite_ptr->field_1C_zpos);
        Char::DispatchCollision_548670(byte_623F48);

        if (field_18_collided_entity != NULL)
        {
            switch (side)
            {
                case 1:
                    field_28_slide_rotation = pSprt->field_0_rotation - kAng90_6FD854;
                    break;
                case 2:
                    field_28_slide_rotation = pSprt->field_0_rotation;
                    break;
                case 3:
                    field_28_slide_rotation = pSprt->field_0_rotation - kAng90_6FD854;
                    break;
                default:
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

        if (pCar != NULL)
        {
            field_1C_prev_collided_entity = pCar;
            field_18_collided_entity = pCar;
        }
        else if (pObj != NULL)
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
        if (pCar == NULL)
        {
            pAny = pObj;
            if (pObj != NULL)
            {
                if (pObj->field_4_sprite->field_C_pCollisionShape->GetDepth_492170() > kFP16Half_6FD8E4)
                {
                    bNoJump = true;
                }
            }
            else
            {
                pAny = pChar;
            }
        }

        if (field_1C_prev_collided_entity == pAny || field_1C_prev_collided_entity == NULL)
        {
            if (field_10_char_state != Char_B4_state::Jumping_15)
            {
                field_1C_prev_collided_entity = pAny;
                field_18_collided_entity = pAny;
            }
            else
            {
                field_69_is_colliding_with_sprite = false;
                field_2A_slide_turn = kAng0_6FDB34;
            }
        }
        else if (field_20_collided_entity_type != 2 && bNoJump == 0)
        {
            if (field_10_char_state != Char_B4_state::Jumping_15)
            {
                Char::DoJump_5454D0();
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
MATCH_FUNC(0x54b8f0)
bool Char::ContinueMovementAfterCollision_54B8F0()
{
    Fix16 x_vec;
    Fix16 y_vec = kFP16Zero_6FD9E4;
    // Every path past the early return leaves through `done`: the original pushes ebx/ebp/edi only
    // after the Jumping_15 check, which needs a single exit block (separate returns keep the pushes
    // at entry). bMoved stays in memory, as in the original.
    bool bMoved = false;
    if (field_10_char_state == Char_B4_state::Jumping_15)
    {
        return true;
    }

    if (field_18_collided_entity)
    {
        field_40_rotation = field_28_slide_rotation;
        field_24_slide_state = 1;
        PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity, x_vec, y_vec);
        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                 field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                 field_80_sprite_ptr->field_1C_zpos);
        Char::DispatchCollision_548670(byte_623F48);
        if (field_18_collided_entity)
        {
            field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
            field_28_slide_rotation.rValue += field_2A_slide_turn.rValue;
            field_28_slide_rotation.Normalize_406C20();
            PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity, x_vec, y_vec);
            field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                     field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                     field_80_sprite_ptr->field_1C_zpos);
            Char::DispatchCollision_548670(byte_623F48);
            if (field_18_collided_entity)
            {
                field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                goto done;
            }
        }
        bMoved = true;
    }
    else
    {
        PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity * gFix16_Two_6FD9EC, x_vec, y_vec);
        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                 field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                 field_80_sprite_ptr->field_1C_zpos);
        Char::DispatchCollision_548670(byte_623F48);
        if (field_18_collided_entity)
        {
            field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);

            // Try stepping back away from the obstacle
            field_40_rotation = field_28_slide_rotation;
            Ang16 back_angle = AddAngles_ool_54B8F0(kAng180_6FD936, AddAngles_ool_54B8F0(field_28_slide_rotation, field_2A_slide_turn));
            PolarToCartesian_OutOfLineMul(back_angle, field_38_velocity * kFP16Four_6FD9F4, x_vec, y_vec);
            field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                     field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                     field_80_sprite_ptr->field_1C_zpos);
            Char::DispatchCollision_548670(byte_623F48);
            if (!field_18_collided_entity && field_24_slide_state != 2)
            {
                // Free behind: turn around
                field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                field_28_slide_rotation.rValue += AddAngles_ool_54B8F0(kAng180_6FD936, field_2A_slide_turn).rValue;
                field_28_slide_rotation.Normalize_406C20();
                field_24_slide_state = 2;
                field_40_rotation = field_28_slide_rotation;
                PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity * gFix16_Two_6FD9EC, x_vec, y_vec);
                field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                         field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                         field_80_sprite_ptr->field_1C_zpos);
                Char::DispatchCollision_548670(byte_623F48);
                if (field_18_collided_entity)
                {
                    field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                    field_28_slide_rotation.rValue += field_2A_slide_turn.rValue;
                    field_28_slide_rotation.Normalize_406C20();
                    field_24_slide_state = 0;
                    field_40_rotation = field_28_slide_rotation;
                    PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity, x_vec, y_vec);
                    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                             field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                             field_80_sprite_ptr->field_1C_zpos);
                    bMoved = true;
                    Char::DispatchCollision_548670(byte_623F48);
                    if (field_18_collided_entity)
                    {
                        field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                    }
                    goto done;
                }
            }
            else
            {
                field_80_sprite_ptr->set_xy_lazy_447E20(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                PolarToCartesian_OutOfLineMul(field_40_rotation, field_38_velocity, x_vec, y_vec);
                field_80_sprite_ptr->set_xyz_lazy_451950(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                         field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                         field_80_sprite_ptr->field_1C_zpos);
                Char::DispatchCollision_548670(byte_623F48);
                if (field_18_collided_entity)
                {
                    field_80_sprite_ptr->setxy_lazy_54EC80(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                    goto done;
                }
            }
        }
        else
        {
            field_69_is_colliding_with_sprite = false;
            field_80_sprite_ptr->setxy_lazy_54EC80(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
            Ang16::PolarToCartesian_451730(field_40_rotation, field_38_velocity, x_vec, y_vec);
            field_80_sprite_ptr->set_xyz_lazy_451950(field_80_sprite_ptr->field_14_xy.x + x_vec,
                                                     field_80_sprite_ptr->field_14_xy.y + y_vec,
                                                     field_80_sprite_ptr->field_1C_zpos);
            Char::DispatchCollision_548670(byte_623F48);
            if (field_18_collided_entity)
            {
                field_80_sprite_ptr->setxy_lazy_54EC80(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800);
                goto done;
            }
            bMoved = true;
            goto done;
        }
        bMoved = true;
    }
done:
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
inline void Char::SetTurnTarget_492400(Ang16 target_rotation)
{
    field_10_char_state = Char_B4_state::Turning_To_Target_25;
    field_14_target_rotation = target_rotation;
    field_46_timer = 255;
}

MATCH_FUNC(0x54c090)
void Char::SnapRotationToFace_54C090()
{

    s32 face = Ang16::GetAngleFace_4F78F0(field_40_rotation);
    if (!CanMoveOntoSlope_54C1A0(face))
    {
        u8 bPastHalf;
        s32 new_dir;
        switch (face)
        {
            case path_direction::up_1:
                bPastHalf = field_40_rotation > kAng180_6FD8E8;
                break;
            case path_direction::right_3: // 10.5.exe's jump table: face 3 compares with 90 degrees, face 2 with kAng0_6FDB3C
                bPastHalf = field_40_rotation > kAng90_6FDA64;
                break;
            case path_direction::down_2:
                bPastHalf = field_40_rotation > kAng0_6FDB3C;
                break;
            case path_direction::left_4:
                bPastHalf = field_40_rotation > kAng270_6FD904;
                break;
        }

        switch (face)
        {
            case path_direction::up_1:
                new_dir = bPastHalf ? path_direction::left_4 : path_direction::right_3;
                break;
            case path_direction::right_3:
                new_dir = bPastHalf ? path_direction::up_1 : path_direction::down_2;
                break;
            case path_direction::down_2:
                new_dir = bPastHalf ? path_direction::right_3 : path_direction::left_4;
                break;
            case path_direction::left_4:
                new_dir = bPastHalf ? path_direction::down_2 : path_direction::up_1;
                break;
            default:
                new_dir = path_direction::down_2;
                break;
        }
        face = new_dir;
    }

    field_40_rotation = ReturnAngleFromRoadDirection_4F7940(face);

    if (field_10_char_state != Char_B4_state::Jumping_15)
    {
        this->field_10_char_state = Char_B4_state::Walking_1;
    }
    this->field_46_timer = 100;
}

MATCH_FUNC(0x54c1a0)
bool Char::CanMoveOntoSlope_54C1A0(s32 path_direction)
{
    u8 slope_type = 0;
    bool bMayLeavePavement = false;
    Fix16 x_fp = field_80_sprite_ptr->field_14_xy.x;
    Fix16 y_fp = field_80_sprite_ptr->field_14_xy.y;
    Fix16 zpos = field_80_sprite_ptr->field_1C_zpos;
    s32 check_z = (zpos.ToInt()) - 1;
    if (field_10_char_state == Char_B4_state::Jumping_15 || field_8_ped_state_1 == ped_state_1::dead_9 || field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) ||
        field_7C_pPed->get_occupation_403980() == ped_ocupation_enum::drone)
    {
        bMayLeavePavement = true;
    }
    if ((field_58_flags & 1) == 1)
    {
        check_z = zpos.ToInt();
    }

    s32 x = x_fp.ToInt();
    s32 y = y_fp.ToInt();
    s32 z = zpos.ToInt();
    u8 slope_z_delta;
    if (gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(x, y, z, path_direction, &slope_z_delta, false))
    {
        return false;
    }


    switch (path_direction)
    {
        case path_direction::up_1:
            slope_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x_fp.ToInt(), y_fp.ToInt() - 1, check_z);
            break;
        case path_direction::right_3:
            slope_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x_fp.ToInt() + 1, y_fp.ToInt(), check_z);
            break;
        case path_direction::down_2:
            slope_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x_fp.ToInt(), y_fp.ToInt() + 1, check_z);
            break;
        case path_direction::left_4:
            slope_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(x_fp.ToInt() - 1, y_fp.ToInt(), check_z);
            break;
        default:
            break;
    }

    Fix16 zpos_frac;
    bool result;

    switch (slope_type)
    {
        case AIR:
            if ((field_58_flags & 1) != 1)
            {
                return false;
            }
            zpos_frac = zpos.GetFracValue();

            if (zpos_frac < kFpPoint1_6FD8DC)
            {
                field_58_flags &= ~1;
                result = CanMoveOntoSlope_54C1A0(path_direction);
                field_58_flags |= 1;
                return result;
            }

            if (zpos_frac <= kFP16Half_6FD8E4)
            {
                return false;
            }

            field_58_flags &= ~1;
            field_80_sprite_ptr->field_1C_zpos++;
            result = CanMoveOntoSlope_54C1A0(path_direction);
            field_80_sprite_ptr->field_1C_zpos--;
            field_58_flags |= 1u;
            break;
        case ROAD:
        case FIELD:
            if (bMayLeavePavement)
            {
                return true;
            }
            return false;
        case PAVEMENT:
        case 4:
            return true;
        default:
            return false;
    }
    return result;
}

// 9.6f 0x495470
MATCH_FUNC(0x54c3e0)
void Char::TurnToFreeAdjacentFace_54C3E0()
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
                    SetTurnTarget_492400(ReturnAngleFromRoadDirection_4F7940(ccw_face));
                }
                else
                {
                    SetTurnTarget_492400(ReturnAngleFromRoadDirection_4F7940(cw_face));
                }
            }
            else
            {
                SetTurnTarget_492400(ReturnAngleFromRoadDirection_4F7940(ccw_face));
            }
        }
        else if (cw_free == 1)
        {
            SetTurnTarget_492400(ReturnAngleFromRoadDirection_4F7940(cw_face));
        }
    }
}

// 9.6f 0x495540
MATCH_FUNC(0x54c500)
bool Char::CanMoveToTile_54C500(char_type x, char_type y)
{
    Fix16 tx = field_80_sprite_ptr->field_14_xy.x;
    Fix16 ty = field_80_sprite_ptr->field_14_xy.y;
    char_type dx = x - tx.ToInt();
    char_type dy = y - ty.ToInt();

    // No movement
    if (dx == 0 && dy == 0)
    {
        return true;
    }

    // Diagonal movement not allowed
    if (dx != 0 && dy != 0)
    {
        return false;
    }

    // Pure horizontal
    if (dx != 0)
    {
        return (dx == -1) ? Char::CanMoveOntoSlope_54C1A0(path_direction::left_4) : Char::CanMoveOntoSlope_54C1A0(path_direction::right_3);
    }

    return (dy == -1) ? Char::CanMoveOntoSlope_54C1A0(path_direction::up_1) : Char::CanMoveOntoSlope_54C1A0(path_direction::down_2);
}

MATCH_FUNC(0x54c580)
void Char::SelectRandomIdleBehavior_54C580()
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
                this->field_10_char_state = Char_B4_state::Walking_1;
                this->field_6C_animation_state = Char_Anim_state::Walking_0;
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
                this->field_10_char_state = Char_B4_state::Walking_Fast_3;
                this->field_6C_animation_state = Char_Anim_state::Walking_0;
                this->field_46_timer = gRng_6F6784.get_int_4F7AE0(200);
                break;
            }
            case 15:
            {
                this->field_10_char_state = Char_B4_state::Running_4;
                this->field_6C_animation_state = Char_Anim_state::Running_1;
                this->field_46_timer = gRng_6F6784.get_int_4F7AE0(200);
                break;
            }
            case 20:
            {
                this->field_10_char_state = Char_B4_state::Standing_7;
                this->field_6C_animation_state = Char_Anim_state::Standing_2;
                this->field_46_timer = gRng_6F6784.get_int_4F7AE0(200);
                break;
            }
            default:
                break;
        }
    }

    if (--this->field_46_timer == 255 && this->field_10_char_state != Char_B4_state::Turning_To_Target_25)
    {
        this->field_46_timer = 0;
    }
}

MATCH_FUNC(0x54c6c0)
void Char::ApplyRandomRotationJitter_54C6C0()
{
    if (gRng_6F6784.get_int_4F7AE0(32) > 22)
    {
        Ang16 old_angle = field_42_rotation_jitter;
        field_42_rotation_jitter = Ang16::Fix16_To_Ang16_inlined_40F540(kFpOne64th_6FD868 * (Fix16(gRng_6F6784.get_int_4F7AE0(16)) - kFP16Eight_6FDA08));
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
void Char::TickMovementStateMachine_54C900()
{
    switch (this->field_10_char_state)
    {
        case Char_B4_state::Walking_1:
            ApplyRandomRotationJitter_54C6C0();
            this->field_38_velocity = kFpOne64th_6FD7B8;
            gCharB4_TurnToFreeFace_6FDB51 = 1;
            gCharB4_VeerFromWalls_6FDB52 = 1;
            byte_6FDB53 = 0;
            break;

        case Char_B4_state::Walking_Fast_3:
            if (this->field_46_timer)
            {
                this->field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                ApplyRandomRotationJitter_54C6C0();
            }
            else
            {
                this->field_38_velocity = kFpOne64th_6FD7B8;
                this->field_10_char_state = Char_B4_state::Walking_1;
                this->field_6C_animation_state = Char_Anim_state::Walking_0;
            }
            byte_6FDB53 = 0;
            gCharB4_TurnToFreeFace_6FDB51 = 1;
            gCharB4_VeerFromWalls_6FDB52 = 1;
            break;

        case Char_B4_state::Running_4:
            if (this->field_46_timer)
            {
                this->field_38_velocity = kFpThreeSixtyFourths_6FD7CC;
                ApplyRandomRotationJitter_54C6C0();
            }
            else
            {
                this->field_38_velocity = kFpOne64th_6FD7B8;
                this->field_10_char_state = Char_B4_state::Walking_1;
                this->field_6C_animation_state = Char_Anim_state::Walking_0;
            }
            byte_6FDB53 = 0;
            gCharB4_TurnToFreeFace_6FDB51 = 1;
            gCharB4_VeerFromWalls_6FDB52 = 1;
            break;

        case Char_B4_state::Standing_7:
            if (this->field_46_timer)
            {
                this->field_38_velocity = kZeroVelocity_6FD7C0;
                this->field_6C_animation_state = Char_Anim_state::Standing_2;
                this->field_68_animation_frame = 0;
            }
            else
            {
                this->field_6C_animation_state = Char_Anim_state::Walking_0;
                this->field_68_animation_frame = 0;
                this->field_10_char_state = Char_B4_state::Walking_1;
                this->field_38_velocity = kFpOne64th_6FD7B8;
            }
            byte_6FDB53 = 0;
            gCharB4_TurnToFreeFace_6FDB51 = 1;
            gCharB4_VeerFromWalls_6FDB52 = 1;
            break;

        case Char_B4_state::Veer_Right_From_Wall_8:
        case Char_B4_state::Veer_Left_From_Wall_9:
            if (!this->field_46_timer)
            {
                this->field_46_timer = 100;
                field_40_rotation.SnapToAng4_405640();
                this->field_10_char_state = Char_B4_state::Walking_1;
                this->field_6C_animation_state = Char_Anim_state::Walking_0;
            }
            gCharB4_VeerFromWalls_6FDB52 = 0;
            byte_6FDB53 = 0;
            gCharB4_TurnToFreeFace_6FDB51 = 1;
            break;

        case Char_B4_state::Turning_To_Target_25:
            TurnTowardsAngle_54CAE0();
            if (ComputeShortestAngleDelta_4056C0(this->field_40_rotation, this->field_14_target_rotation) < kAng12_6FD892)
            {
                this->field_38_velocity = kFpOne64th_6FD7B8;
                this->field_10_char_state = Char_B4_state::Walking_1;
                this->field_6C_animation_state = Char_Anim_state::Walking_0;
                this->field_46_timer = 0;
            }
            gCharB4_TurnToFreeFace_6FDB51 = 0;
            gCharB4_VeerFromWalls_6FDB52 = 0;
            byte_6FDB53 = 0;
            break;

        case Char_B4_state::Interacting_Car_Door_36:
            this->field_10_char_state = Char_B4_state::Walking_1;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x54cae0)
void Char::TurnTowardsAngle_54CAE0()
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
        this->field_40_rotation += kAng354_6FD9D8;
    }
    else
    {
        if (field_40_rotation - field_14_target_rotation <= kAng180_6FD920)
        {
            goto add_6FD9D8;
        }
    add_6FDA54:
        this->field_40_rotation += kAng6_6FDA54;
    }
}

// https://decomp.me/scratch/RGvHW
MATCH_FUNC(0x54cc40)
void Char::ApplyMovement_54CC40()
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
                    if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng270_6FD904, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(kAng0_6FDB3C, field_38_velocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos_6FD7F8;
                        ypos += gCharB4_Saved_Ypos_6FD800;
                        gAng16_AngleOfCollision_6FD808 = kAng0_6FDB3C;
                        if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(kAng0_6FDB3C, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
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
                    if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng90_6FDA64, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
                        field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + xpos,
                                                                 field_80_sprite_ptr->field_14_xy.y + ypos,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                    else
                    {
                        PolarToCartesian_OutOfLineMul(kAng0_6FDB3C, field_38_velocity, xpos, ypos);
                        xpos += gCharB4_Saved_Xpos_6FD7F8;
                        ypos += gCharB4_Saved_Ypos_6FD800;
                        gAng16_AngleOfCollision_6FD808 = kAng0_6FDB3C;
                        if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
                        {
                            PolarToCartesian_OutOfLineMul(kAng0_6FDB3C, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
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
                    if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
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
                    PolarToCartesian_OutOfLineMul(kAng0_6FDB3C, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = kAng0_6FDB3C;
                    if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        PolarToCartesian_OutOfLineMul(kAng0_6FDB3C, field_38_velocity / gFix16_Two_6FD9EC, xpos, ypos);
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
                    if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
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
                        if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
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
                    if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
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
                        if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
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
                    if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
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
                    Ang16::PolarToCartesian_451730(kAng0_6FDB3C, field_38_velocity, xpos, ypos);
                    xpos += gCharB4_Saved_Xpos_6FD7F8;
                    ypos += gCharB4_Saved_Ypos_6FD800;
                    gAng16_AngleOfCollision_6FD808 = kAng0_6FDB3C;
                    if (Char::CanStepToTile_54EF60(xpos.ToInt(), ypos.ToInt()))
                    {
                        // Out-of-line operators (inline budget)
                        Ang16::PolarToCartesian_451730(kAng0_6FDB3C, field_38_velocity.Divide_436A20(gFix16_Two_6FD9EC), xpos, ypos);
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
void Char::CheckCollisionsAndUpdateAnim_54DD70()
{
    if (this->field_8_ped_state_1 != ped_state_1::dead_9 && this->field_10_char_state != Char_B4_state::Jumping_15)
    {
        CheckAndHandleCollisions_5459C0();

        if (field_7C_pPed->GetAttacking_433CA0() && field_7C_pPed->field_21C_bf.bNoWeapon)
        {
            if (this->field_6C_animation_state != Char_Anim_state::Punching_4)
            {
                this->field_6C_animation_state = Char_Anim_state::Punching_4;
                this->field_68_animation_frame = 0;
            }
            else
            {
                field_7C_pPed->HandleClosePedInteraction_45CAA0();
            }
        }
        else if (this->field_6C_animation_state != Char_Anim_state::Punching_4 || this->field_68_animation_frame == 6)
        {
            if (field_38_velocity != kZeroVelocity_6FD7C0)
            {
                this->field_6C_animation_state = field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC;
            }
            else
            {
                this->field_6C_animation_state = Char_Anim_state::Standing_2;
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
MATCH_FUNC(0x54ddf0)
void Char::state_0_54DDF0()
{
    char_type bCanMove;
    char_type bAllowStateChange;
    u8 bTileOk;

    gCharB4_TurnToFreeFace_6FDB51 = 1;
    gCharB4_VeerFromWalls_6FDB52 = 1;
    byte_6FDB53 = 0;
    gCharB4_GradientSlopeBelow_6FDB54 = 0;
    this->field_4A_timeout = 500;
    bAllowStateChange = 1;
    bCanMove = 0;
    u8 bNoNewRotation = 1;
    Ang16 side_angle(0);
    Ang16 new_rotation(0);


    u8 block_type;
    gmp_block_info* pBlock;

    if ((field_58_flags & 1) == 0 && gCharB4_Saved_Zpos_6FD7FC.GetFracValue() == kFP16Zero_6FD9E4)
    {
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
    }
    else
    {
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());
        pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());

        if (pBlock == NULL || block_type == 0)
        {
            block_type =
                gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
            pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        }
    }

    if (pBlock)
    {
        field_58_flags_bf.bOnSlope = GET_SLOPE_BITS(pBlock->field_B_slope_type) != 0 && GET_SLOPE_BITS(pBlock->field_B_slope_type) != 0xFC;
        if (gGtx_0x106C_703DD4->IsElectrifiedFloorType_491F80(pBlock->field_8_lid & block_face::tile_idx_mask) &&
            field_10_char_state != Char_B4_state::Jumping_15)
        {
            if (!field_7C_pPed->field_21C_bf.bLeftVehicle)
            {
                field_7C_pPed->field_210_shock_counter += 3;
                s32 killer_id = field_7C_pPed->field_204_killer_id;
                if (killer_id != 0)
                {
                    if (gPedManager_6787BC->PedById(killer_id))
                    {
                        field_7C_pPed->field_290_death_cause = ped_death_cause::electrocuted_2;
                        field_7C_pPed->field_264_killer_id_timer = 50;
                    }
                }
            }
        }
    }
    if (static_cast<u8>(Char::IsOnWater_545570()))
    {
        field_7C_pPed->PutOutFire();
        Char::DrownPed_5459E0();
        return;
    }

    Fix16 ret1;
    Fix16 ret2;

    if (block_type == 0 && (field_58_flags & 1) == 0)
    {
        // 10.5 has a separate copy for the non-player case, so this is a nested if, not one && chain
        if (field_7C_pPed->field_15C_player)
        {
            if (field_7C_pPed->GetMoveDirection_45C9B0() < kFP16Zero_6FD9E4 || (field_58_flags & 8) != 0)
            {
                Ang16::PolarToCartesian_41FC20(field_40_rotation + kAng180_6FD936, kFpThreeThirtySeconds_6FDAC8, ret1, ret2);
            }
            else
            {
                Ang16::PolarToCartesian_41FC20(field_40_rotation, kFpThreeThirtySeconds_6FDAC8, ret1, ret2);
            }
        }
        else
        {
            Ang16::PolarToCartesian_41FC20(field_40_rotation, kFpThreeThirtySeconds_6FDAC8, ret1, ret2);
        }

        ret1 += gCharB4_Saved_Xpos_6FD7F8;
        ret2 += gCharB4_Saved_Ypos_6FD800;

        // One GetBlockTypeAtCoord_420420 call per z case: VC6 tail-merges the two copies (gMap in edx, the
        // null arm first and the unfolded test after the merge)
        if (gCharB4_Saved_Zpos_6FD7FC.GetFracValue() == kFP16Zero_6FD9E4)
        {
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(ret1.ToInt(), ret2.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        }
        else
        {
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(ret1.ToInt(), ret2.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());
        }
        if (block_type == AIR)
        {
            if (field_10_char_state != Char_B4_state::Jumping_15)
            {
                // Ped is falling; the end-of-jump path below jumps here (goto-free forms change the code)
            start_falling:
                field_6C_animation_state = Char_Anim_state::Normal_Fall_11;
                field_68_animation_frame = 0;
                field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::falling_19);
                field_8_ped_state_1 = ped_state_1::immobilized_8;
                field_C_ped_state_2 = ped_state_2::falling_19;
                if (field_7C_pPed->field_15C_player)
                {
                    if (field_7C_pPed->GetMoveDirection_45C9B0() < kFP16Zero_6FD9E4 || (field_58_flags & 8) != 0)
                    {
                        field_38_velocity = -field_38_velocity;
                    }
                }
                field_90_fall_speed = field_38_velocity;
                field_94_fall_z_speed = kFP16Zero_6FD9E4;
                if (field_38_velocity == kZeroVelocity_6FD7C0)
                {
                    field_16_state_init_pending = true;
                }
                Char::state_8_5520A0();
                return;
            }
            // A jumping ped only starts falling once the jump is over
            if (this->field_68_animation_frame == 7 || this->field_7C_pPed->field_21C_bf.bAttacking)
            {
                this->field_10_char_state = Char_B4_state::Walking_1;
                field_7C_pPed->ClearAttacking_403A40();
                goto start_falling;
            }
        }
    }

    field_44_block_type = block_type;
    Fix16 radius;
    if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) == true)
    {
        radius = kFpThreeThirtySeconds_6FDAC8;
        if (this->field_10_char_state)
        {
            this->field_46_timer = 9999;
        }
        field_40_rotation += field_7C_pPed->GetTurnSpeed_45C900();

        if (this->field_10_char_state == Char_B4_state::Jumping_15)
        {
            field_7C_pPed->ClearAttacking_403A40();
            if (this->field_6C_animation_state != Char_Anim_state::Jumping_5)
            {
                this->field_6C_animation_state = Char_Anim_state::Jumping_5;
                this->field_68_animation_frame = 0;
            }
            if (field_7C_pPed->GetMoveDirection_45C9B0() > kFP16Zero_6FD9E4)
            {
                if (field_38_velocity == kZeroVelocity_6FD7C0)
                {
                    this->field_38_velocity = this->field_3C_run_or_jump_speed;
                }
                else
                {
                    if (field_3C_run_or_jump_speed > field_38_velocity)
                    {
                        this->field_38_velocity += kFpOne256th_6FD99C;
                    }
                    else
                    {
                        if (field_38_velocity > field_3C_run_or_jump_speed)
                        {
                            this->field_38_velocity -= kFpOne256th_6FD99C;
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
            if (field_7C_pPed->GetMoveDirection_45C9B0() > kFP16Zero_6FD9E4)
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
                        this->field_38_velocity += kFpOne256th_6FD99C;
                        field_58_flags &= ~8;
                    }
                    else
                    {
                        if (field_38_velocity > field_3C_run_or_jump_speed)
                        {
                            this->field_38_velocity -= kFpOne256th_6FD99C;
                        }
                        field_58_flags &= ~8;
                    }
                }
            }
            else
            {
                if (field_7C_pPed->GetMoveDirection_45C9B0() < kFP16Zero_6FD9E4)
                {
                    new_rotation = this->field_40_rotation;
                    this->field_58_flags = this->field_58_flags | 8;
                    field_40_rotation += kAng180_6FD936;

                    if (field_38_velocity == kZeroVelocity_6FD7C0)
                    {
                        bNoNewRotation = 0;
                        this->field_38_velocity = this->field_3C_run_or_jump_speed;
                    }
                    else
                    {
                        if (field_38_velocity < field_3C_run_or_jump_speed)
                        {
                            bNoNewRotation = 0;
                            this->field_38_velocity += kFpOne256th_6FD99C;
                        }
                        else
                        {
                            if (field_38_velocity > field_3C_run_or_jump_speed)
                            {
                                this->field_38_velocity -= kFpOne256th_6FD99C;
                            }
                            bNoNewRotation = 0;
                        }
                    }
                }
                else
                {
                    if (this->field_6A_repulsion_timer)
                    {
                        gCharB4_TurnToFreeFace_6FDB51 = 0;
                        gCharB4_VeerFromWalls_6FDB52 = 0;
                        new_rotation = field_40_rotation;
                        this->field_40_rotation = this->field_74_repulsion_rotation;

                        bNoNewRotation = 0;
                        this->field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                    }
                    else
                    {
                        this->field_38_velocity = kZeroVelocity_6FD7C0;
                        if (this->field_10_char_state)
                        {
                            this->field_10_char_state = Char_B4_state::Standing_7;
                        }
                    }
                }
            }
        }

        if (this->field_7C_pPed->field_21C_bf.bHitByAttacker)
        {
            this->field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
        }
    }
    else
    {
        radius = kFpThreeThirtySeconds_6FDAC8;
        if (this->field_69_is_colliding_with_sprite || this->field_10_char_state == Char_B4_state::Jumping_15)
        {
            bAllowStateChange = 0;
            field_7C_pPed->field_21C_bf.bAttacking = false;
            gCharB4_TurnToFreeFace_6FDB51 = 0;
            gCharB4_VeerFromWalls_6FDB52 = 0;
        }
        else
        {
            if (this->field_6A_repulsion_timer)
            {
                bAllowStateChange = 0;
                gCharB4_TurnToFreeFace_6FDB51 = 0;
                gCharB4_VeerFromWalls_6FDB52 = 0;
                new_rotation = field_40_rotation;
                this->field_40_rotation = this->field_74_repulsion_rotation;
                this->field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                bNoNewRotation = 0;
            }
            if (this->field_38_velocity < kZeroVelocity_6FD7C0)
            {
                Ang16 rotation = field_40_rotation;
                new_rotation = rotation;
                rotation += kAng180_6FD936;
                field_40_rotation = rotation;
                field_58_flags = field_58_flags | 0x8;
                field_38_velocity = Fix16::Abs(field_38_velocity);
                bNoNewRotation = 0;
            }
        }
        if (field_10_char_state)
        {
            if (field_10_char_state != Char_B4_state::Jumping_15 && this->field_8_ped_state_1 != ped_state_1::dead_9)
            {
                switch (block_type)
                {
                    case ROAD:
                    case FIELD:
                        if (field_7C_pPed->get_objective_403A80() == objectives_enum::no_obj_0 ||
                            field_7C_pPed->get_objective_403A80() == objectives_enum::left_behind_by_leader_8)
                        {
                            field_7C_pPed->SetObjective2_463830(objectives_enum::walk_to_point_17);
                        }
                        break;
                    default:
                        if (this->field_C_ped_state_2 == ped_state_2::ped2_walking_0 && bAllowStateChange)
                        {
                            Char::SelectRandomIdleBehavior_54C580();
                            Char::TickMovementStateMachine_54C900();
                        }
                        break;
                }
            }
        }
    }

    if (bNoNewRotation)
    {
        new_rotation = field_40_rotation;
    }
    if (field_38_velocity == kFP16Zero_6FD9E4)
    {
        field_58_flags &= ~8;
    }

    Ang16::PolarToCartesian_41FC20(field_40_rotation, radius, ret1, ret2);

    ret1 += gCharB4_Saved_Xpos_6FD7F8;
    ret2 += gCharB4_Saved_Ypos_6FD800;
    ret1 += field_4C_conveyor_dx;
    ret2 += field_50_conveyor_dy;

    if (!Char::IsNearTileCentre_5532C0())
    {
        if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) == true)
        {
            bCanMove = 1;
            gAng16_AngleOfCollision_6FD808 = field_40_rotation;
            u8 bDiagonalOk = Char::CanStepToTile_54EF60(ret1.ToInt(), ret2.ToInt());
            if (gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110() != 1 && !bDiagonalOk)
            {
                Char::ApplyMovement_54CC40();
                bCanMove = 0;
                goto after_move;
            }
        }
        else
        {
            bTileOk = Char::CanMoveToTile_54C500(ret1.ToInt(), ret2.ToInt());

            if (gCharB4_TurnToFreeFace_6FDB51)
            {
                Char::TurnToFreeAdjacentFace_54C3E0();
            }

            if (bTileOk == 1)
            {
                bCanMove = 1;
                if (gCharB4_VeerFromWalls_6FDB52)
                {
                    side_angle = field_40_rotation;
                    side_angle.SnapToAng4_405640();
                    side_angle += kAng90_6FD8A2;
                    Fix16 xpos_3;
                    Fix16 ypos_3;
                    Ang16::PolarToCartesian_41FC20(side_angle, kFP16Half_6FD9B4, xpos_3, ypos_3);

                    xpos_3 += gCharB4_Saved_Xpos_6FD7F8;
                    ypos_3 += gCharB4_Saved_Ypos_6FD800;

                    if (Char::CanMoveToTile_54C500(xpos_3.ToInt(), ypos_3.ToInt()) == 1)
                    {
                        side_angle = field_40_rotation;
                        side_angle.SnapToAng4_405640();
                        side_angle += kAng270_6FD94C;

                        Fix16 xpos_4;
                        Fix16 ypos_4;

                        Ang16::PolarToCartesian_41FC20(side_angle, kFP16Half_6FD9B4, xpos_4, ypos_4);

                        xpos_4 += gCharB4_Saved_Xpos_6FD7F8;
                        ypos_4 += gCharB4_Saved_Ypos_6FD800;

                        if (!Char::CanMoveToTile_54C500(xpos_4.ToInt(), ypos_4.ToInt()))
                        {
                            VeerLeftFromWall_4923D0();
                        }
                    }
                    else
                    {
                        VeerRightFromWall_4923A0();
                    }
                }
            }
            else
            {
                Char::SnapRotationToFace_54C090();
                goto after_move;
            }
        }
    }

    Ang16::PolarToCartesian_41FC20(field_40_rotation, field_38_velocity, ret1, ret2);
    ret1 += field_4C_conveyor_dx;
    ret2 += field_50_conveyor_dy;
    field_80_sprite_ptr->set_xyz_lazy_420600(field_80_sprite_ptr->field_14_xy.x + ret1,
                                             field_80_sprite_ptr->field_14_xy.y + ret2,
                                             field_80_sprite_ptr->field_1C_zpos);

// shared exit for the two blocked-move paths: goto-free forms change the code
after_move:
    field_4C_conveyor_dx = kFP16Zero_6FD9E4;
    field_50_conveyor_dy = kFP16Zero_6FD9E4;
    if (bCanMove == 1 || (field_58_flags & 1) == 1)
    {
        gCharB4_GradientSlopeBelow_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                  field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                  field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);
        Char::ManageZCoordAndSlopes_548590();
    }

    Char::DispatchCollision_548670(byte_6FDB56);

    if (!field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2))
    {
        if (field_69_is_colliding_with_sprite)
        {
            if (Char::ContinueMovementAfterCollision_54B8F0() == 1)
            {
                // The sprite position before the reset: CanReachTile tests it and it is put back afterwards
                Fix16 old_xpos = field_80_sprite_ptr->field_14_xy.x;
                Fix16 old_ypos = field_80_sprite_ptr->field_14_xy.y;
                char_type tile_x = old_xpos.ToInt();
                char_type tile_y = old_ypos.ToInt();
                if (static_cast<char_type>(gCharB4_Saved_Xpos_6FD7F8.ToInt()) != tile_x || static_cast<char_type>(gCharB4_Saved_Ypos_6FD800.ToInt()) != tile_y)
                {
                    field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);

                    field_45_slope_gradient_direction = gCharB4_Saved_SlopeGradDir_6FD7B0.ToInt();
                    if (!Char::CanReachTile_550090(tile_x, tile_y))
                    {
                        field_69_is_colliding_with_sprite = false;
                        field_5C_jump_over_timer = 10;
                    }
                    else
                    {
                        field_80_sprite_ptr->set_xyz_lazy_420600(old_xpos, old_ypos, gCharB4_Saved_Zpos_6FD7FC);
                        gCharB4_GradientSlopeBelow_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                    field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                    (field_80_sprite_ptr->field_1C_zpos - kFP16One_6FD9E8).ToInt());
                        Char::ManageZCoordAndSlopes_548590();
                    }
                }
            }
        }
    }

    if (!bNoNewRotation)
    {
        field_40_rotation = new_rotation;
    }

    Char::CheckCollisionsAndUpdateAnim_54DD70();
}

// https://decomp.me/scratch/Zk9Eh
MATCH_FUNC(0x54ecb0)
bool Char::CanStepForwardWithRegionCheck_54ECB0(s32 direction)
{
    u8 slope_z_delta;

    Fix16 xpos = field_80_sprite_ptr->field_14_xy.x;
    Fix16 ypos = field_80_sprite_ptr->field_14_xy.y;
    Fix16 zpos = field_80_sprite_ptr->field_1C_zpos;

    s32 new_zpos = (zpos).ToInt() - 1;
    s32 below_z = (zpos - kFP16One_6FD9E8).ToInt();

    if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(xpos.ToInt(), ypos.ToInt(), below_z))
    {
        new_zpos = zpos.ToInt();
    }
    if (gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(xpos.ToInt(), ypos.ToInt(), zpos.ToInt(), direction, &slope_z_delta, false))
    {
        gCharB4_PathDirection_623F44 = direction;
        return false;
    }
    if (gCharB4_StepPosValid_6FDB57)
    {
        field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_StepXpos_6FD8B8, gCharB4_StepYpos_6FD8BC, zpos);
        Char::ManageZCoordAndSlopes_548590();
        if (field_80_sprite_ptr->CheckSpriteMovementRegion_5A2500())
        {
            gCharB4_PathDirection_623F44 = direction;

            field_80_sprite_ptr->set_xyz_lazy_420600(xpos, ypos, zpos);
            gMap_0x370_6F6268->ClearBlockedByTerrain_492130();
            return false;
        }
        field_80_sprite_ptr->set_xyz_lazy_420600(xpos, ypos, zpos);
        Char::ManageZCoordAndSlopes_548590();
    }

    u8 block_type;

    switch (direction)
    {
        case path_direction::up_1:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt(), ypos.ToInt() - 1, new_zpos);
            break;
        case path_direction::right_3:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt() + 1, ypos.ToInt(), new_zpos);
            break;
        case path_direction::down_2:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt(), ypos.ToInt() + 1, new_zpos);
            break;
        case path_direction::left_4:
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
        if (zpos.GetFracValue() < kFpPoint3_6FD91C)
        {
            field_58_flags &= ~1u;
            bool result = Char::CanStepForward_54FEC0(direction);
            field_58_flags |= 1u;
            return result;
        }
        if (!field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2))
        {
            return false;
        }
    }
    return true;
}

// https://decomp.me/scratch/Ub1EN
// 9.6f 0x495BF0: the collision offset division is written at each use (VC6 CSEs it), bCanStep is set
// in each leaf and leaves 2-4 return explicitly (leaf 1's tail is the cross-jump keeper).
MATCH_FUNC(0x54EF60)
bool Char::CanStepToTile_54EF60(char_type target_x, char_type target_y)
{
    bool bIsNearXposBlockBoundary = true;
    bool bIsNearYposBlockBoundary = true;

    Fix16 sprite_xpos = field_80_sprite_ptr->field_14_xy.x;
    Fix16 sprite_ypos = field_80_sprite_ptr->field_14_xy.y;
    Fix16 sprite_zpos = field_80_sprite_ptr->field_1C_zpos;

    u8 old_slope_dir = field_45_slope_gradient_direction;
    gCharB4_StepPosValid_6FDB57 = 1;
    Fix16 xpos, ypos;
    Ang16::PolarToCartesian_41FC20(gAng16_AngleOfCollision_6FD808, gFP16_CollisionCheckRadius_6FDACC, xpos, ypos);
    gCharB4_StepXpos_6FD8B8 = gCharB4_Saved_Xpos_6FD7F8 + xpos;
    gCharB4_StepYpos_6FD8BC = gCharB4_Saved_Ypos_6FD800 + ypos;


    // Get the x diff of the neareast block on left/right side, if it's very close to them
    // Test if it is near right block
    s8 x_diff = (gCharB4_StepXpos_6FD8B8 + gCharB4_WorldCollisionOffset_6FD8D8 / gFix16_Two_6FD9EC).ToInt() - (sprite_xpos).ToInt();

    if (x_diff == 0)
    {
        // Test if it is near left block
        x_diff = (gCharB4_StepXpos_6FD8B8 - gCharB4_WorldCollisionOffset_6FD8D8 / gFix16_Two_6FD9EC).ToInt() - (sprite_xpos).ToInt();
        if (x_diff == 0)
        {
            bIsNearXposBlockBoundary = false;
        }
    }

    // Get the y diff of the neareast block on north/south side, if it's very close to them
    // Test if it is near south block
    s8 y_diff = (gCharB4_StepYpos_6FD8BC + gCharB4_WorldCollisionOffset_6FD8D8 / gFix16_Two_6FD9EC).ToInt() - (sprite_ypos).ToInt();

    if (y_diff == 0)
    {
        // Test if it is near north block
        y_diff = (gCharB4_StepYpos_6FD8BC - gCharB4_WorldCollisionOffset_6FD8D8 / gFix16_Two_6FD9EC).ToInt() - (sprite_ypos).ToInt();
        if (y_diff == 0)
        {
            bIsNearYposBlockBoundary = false;
        }
    }

    if (!bIsNearXposBlockBoundary && !bIsNearYposBlockBoundary)
    {
        x_diff = target_x - field_80_sprite_ptr->field_14_xy.x.ToInt();
        y_diff = target_y - field_80_sprite_ptr->field_14_xy.y.ToInt();
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
            if (y_diff == -1)
            {
                bCanStep = true;
                field_80_sprite_ptr->field_14_xy.x.subtract_one_491F00();
                if (field_58_flags_bf.bOnSlope)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::up_1;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y.subtract_one_491F00();
                if (field_58_flags_bf.bOnSlope)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::left_4;
                    }
                    else
                    {
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::left_4;
                    }
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                    }
                }
                field_45_slope_gradient_direction = old_slope_dir;
            }
            else
            {
                bCanStep = true;
                field_80_sprite_ptr->field_14_xy.x.subtract_one_491F00();
                if (field_58_flags_bf.bOnSlope)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::down_2;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y.add_one_491EF0();
                if (field_58_flags_bf.bOnSlope)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::left_4;
                    }
                    else
                    {
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::left_4) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::left_4;
                    }
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                    }
                }
                field_45_slope_gradient_direction = old_slope_dir;
                return bCanStep;
            }
        }
        else
        {
            if (y_diff == -1)
            {
                bCanStep = true;
                field_80_sprite_ptr->field_14_xy.x.add_one_491EF0();
                if (field_58_flags_bf.bOnSlope)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                {
                    gCharB4_PathDirection_623F44 = path_direction::up_1;
                    bCanStep = false;
                }
                field_80_sprite_ptr->field_14_xy.x = sprite_xpos;
                field_80_sprite_ptr->field_14_xy.y.subtract_one_491F00();
                if (field_58_flags_bf.bOnSlope)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    else
                    {
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::up_1) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                    }
                }
                field_45_slope_gradient_direction = old_slope_dir;
                return bCanStep;
            }
            else
            {
                bCanStep = true;
                field_80_sprite_ptr->field_14_xy.x.add_one_491EF0();
                if (field_58_flags_bf.bOnSlope)
                {
                    field_80_sprite_ptr->field_1C_zpos =
                        gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(field_80_sprite_ptr->field_14_xy.x, field_80_sprite_ptr->field_14_xy.y);
                }
                if (field_80_sprite_ptr->field_1C_zpos == kFP16Zero_6FD9E4)
                {
                    field_80_sprite_ptr->field_1C_zpos = sprite_zpos;
                }
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    else
                    {
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                        bCanStep = true;
                        if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
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
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::right_3) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    if (!CanStepForwardWithRegionCheck_54ECB0(path_direction::down_2) && !gMap_0x370_6F6268->TakeBlockedByTerrain_4E0110())
                    {
                        bCanStep = false;
                        gCharB4_PathDirection_623F44 = path_direction::down_2;
                    }
                }
                field_45_slope_gradient_direction = old_slope_dir;
                return bCanStep;
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
bool Char::CanStepForward_54FEC0(s32 direction)
{
    bool result;

    Fix16 z_frac;
    u8 block_type;

    Fix16 xpos = field_80_sprite_ptr->field_14_xy.x;
    Fix16 ypos = field_80_sprite_ptr->field_14_xy.y;
    Fix16 zpos = field_80_sprite_ptr->field_1C_zpos;
    s8 slope_z_adjust = 0;
    s32 zpos_int = zpos.ToInt();

    s32 check_z = zpos_int - 1;

    if ((field_58_flags & 1) == 1)
    {
        check_z = zpos_int;
    }

    if (gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(xpos.ToInt(), ypos.ToInt(), zpos_int, direction, reinterpret_cast<u8*>(&slope_z_adjust), false))
    {
        gCharB4_PathDirection_623F44 = direction;
        return false;
    }

    check_z += slope_z_adjust;

    if (check_z < 0)
    {
        return false;
    }

    switch (direction)
    {
        case path_direction::up_1:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt(), ypos.ToInt() - 1, check_z);
            break;
        case path_direction::right_3:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt() + 1, ypos.ToInt(), check_z);
            break;
        case path_direction::down_2:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt(), ypos.ToInt() + 1, check_z);
            break;
        case path_direction::left_4:
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(xpos.ToInt() - 1, ypos.ToInt(), check_z);
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
        z_frac = zpos.GetFracValue();
        if (z_frac < kFP16Half_6FD8E4)
        {
            field_58_flags &= ~1u;
            result = Char::CanStepForward_54FEC0(direction);
            field_58_flags |= 1u;
            return result;
        }
        if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2))
        {
            return true;
        }
        if (z_frac > kFP16Half_6FD8E4)
        {
            field_58_flags &= ~1u;
            field_80_sprite_ptr->field_1C_zpos += Fix16(1);
            result = Char::CanStepForward_54FEC0(direction);
            field_80_sprite_ptr->field_1C_zpos -= Fix16(1);
            field_58_flags |= 1u;
            return result;
        }
        return false;
    }
    return true;
}

MATCH_FUNC(0x550090)
bool Char::CanReachTile_550090(u8 xpos, u8 ypos)
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

                    if (!Char::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y.subtract_one_491F00();
                    if (!Char::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y = original_y;
                    if (!Char::CanReachTile_550090((gCharB4_Saved_Xpos_6FD7F8 - kFP16One_6FD9E8).ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt()))
                    {
                        bRes_1 = false;
                    }
                    if (!Char::CanReachTile_550090(gCharB4_Saved_Xpos_6FD7F8.ToInt(), (gCharB4_Saved_Ypos_6FD800 - kFP16One_6FD9E8).ToInt()))
                    {
                        bRes_1 = false;
                    }
                }
                else
                {
                    field_80_sprite_ptr->field_14_xy.x.subtract_one_491F00();

                    if (!Char::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }

                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y.add_one_491EF0();
                    if (!Char::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y = original_y;
                    if (!Char::CanReachTile_550090((field_80_sprite_ptr->field_14_xy.x - kFP16One_6FD9E8).ToInt(),
                                                      field_80_sprite_ptr->field_14_xy.y.ToInt()))
                    {
                        bRes_1 = false;
                    }

                    if (!Char::CanReachTile_550090(field_80_sprite_ptr->field_14_xy.x.ToInt(),
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
                    if (!Char::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_2 = false;
                        gCharB4_PathDirection_623F44 = path_direction::right_3;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y.subtract_one_491F00();
                    if (!Char::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_2 = false;
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y = original_y;
                    if (!Char::CanReachTile_550090((kFP16One_6FD9E8 + field_80_sprite_ptr->field_14_xy.x).ToInt(),
                                                      field_80_sprite_ptr->field_14_xy.y.ToInt()))
                    {
                        bRes_2 = false;
                        gCharB4_PathDirection_623F44 = path_direction::up_1;
                    }
                    if (!Char::CanReachTile_550090(field_80_sprite_ptr->field_14_xy.x.ToInt(),
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
                    if (!Char::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }

                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y.add_one_491EF0();
                    if (!Char::CanReachTile_550090(xpos, ypos))
                    {
                        bRes_1 = false;
                    }
                    field_80_sprite_ptr->field_14_xy.x = original_x;
                    field_80_sprite_ptr->field_14_xy.y = original_y;
                    if (!Char::CanReachTile_550090((kFP16One_6FD9E8 + field_80_sprite_ptr->field_14_xy.x).ToInt(),
                                                      field_80_sprite_ptr->field_14_xy.y.ToInt()))
                    {
                        bRes_1 = false;
                    }
                    if (!Char::CanReachTile_550090(field_80_sprite_ptr->field_14_xy.x.ToInt(),
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
                if (Char::CanStepForward_54FEC0(path_direction::left_4))
                {
                    return true;
                }
                else
                {
                    gCharB4_PathDirection_623F44 = path_direction::left_4;
                    return false;
                }
            }
            else if (Char::CanStepForward_54FEC0(path_direction::right_3))
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
            if (Char::CanStepForward_54FEC0(path_direction::up_1))
            {
                return true;
            }
            else
            {
                gCharB4_PathDirection_623F44 = path_direction::up_1;
                return false;
            }
        }
        else if (Char::CanStepForward_54FEC0(path_direction::down_2))
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

static inline s32 IsOnSlopeBlock(gmp_block_info* pBlock)
{
    if (GET_SLOPE_BITS(pBlock->field_B_slope_type) != 0 && GET_SLOPE_BITS(pBlock->field_B_slope_type) != 0xFC)
    {
        return 1;
    }
    return 0;
}

// MaxAbsDistance_42A6B0 as state_1_5504F0 inlines it: past the nested inline budget, Max is the named
// out-of-line copy Max_44E540 (as in Weapon's MaxAbsDistance_5DF270)
static inline Fix16 MaxAbsDistance_5504F0(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2)
{
    Fix16 diff_x;
    diff_x = x2 - x1;
    Fix16 diff_y;
    diff_y = y2 - y1;
    Fix16 result;
    result = Fix16::Max_44E540(Fix16::Abs(diff_x), Fix16::Abs(diff_y));
    return result;
}

// https://decomp.me/scratch/chZqY
MATCH_FUNC(0x5504f0)
void Char::state_1_5504F0()
{
    Ang16 saved_rotation;
    Ang16 prev_angle;
    Ang16 cur_rotation;
    Ang16 next_rotation;
    char_type bSnapRotation;
    char_type bMoved;
    Fix16 probe_x;
    Fix16 probe_y;

    Ang16 desired_angle;
    Ang16 target_rotation;
    u8 x_u8;
    u8 y_u8;

    field_4A_timeout = 500;
    Ang16 angle = 0;
    field_58_flags_bf.bReverseVelocity = false;
    field_58_flags_bf.bFlag6 = false;
    bMoved = 0;
    saved_rotation = 0;
    bSnapRotation = 0;
    gCharB4_GradientSlopeBelow_6FDB54 = 0;
    gCharB4_Saved_TileX_6FDAD8 = gCharB4_Saved_Xpos_6FD7F8.ToUInt8();
    gCharB4_Saved_TileY_6FDAD9 = gCharB4_Saved_Ypos_6FD800.ToUInt8();
    u8 start_tile_x = gCharB4_Saved_Xpos_6FD7F8.ToUInt8();
    u8 start_tile_y = gCharB4_Saved_Ypos_6FD800.ToUInt8();
    u8 block_type;
    gmp_block_info* pBlock;
    if (!field_58_flags_bf.bOnSlope && gCharB4_Saved_Zpos_6FD7FC.GetFracValue() == kFP16Zero_6FD9E4 &&
        gCharB4_Saved_Zpos_6FD7FC > kFP16Zero_6FD9E4)
    {
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                                   gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                                   gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                     gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                     gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
    }
    else
    {
        block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                                   gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                                   gCharB4_Saved_Zpos_6FD7FC.ToInt());
        pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                     gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                     gCharB4_Saved_Zpos_6FD7FC.ToInt());
    }

    if (pBlock)
    {
        field_58_flags_bf.bOnSlope = IsOnSlopeBlock(pBlock);
        if (gGtx_0x106C_703DD4->IsElectrifiedFloorType_491F80(pBlock->field_8_lid & block_face::tile_idx_mask) && field_10_char_state != Char_B4_state::Jumping_15)
        {
            if (!field_7C_pPed->field_21C_bf.bLeftVehicle)
            {
                field_7C_pPed->field_210_shock_counter += 3;
                if (field_7C_pPed->field_204_killer_id)
                {
                    if (gPedManager_6787BC->PedById(field_7C_pPed->field_204_killer_id))
                    {
                        field_7C_pPed->field_290_death_cause = ped_death_cause::electrocuted_2;
                        field_7C_pPed->field_264_killer_id_timer = 50;
                    }
                }
            }
        }
    }

    if (static_cast<u8>(Char::IsOnWater_545570()))
    {
        field_7C_pPed->PutOutFire();
        Char::DrownPed_5459E0();
        return;
    }

    if (!block_type && !field_58_flags_bf.bOnSlope)
    {
        if (field_10_char_state != Char_B4_state::Jumping_15)
        {
            // The z argument is the raw subtraction: Fix16::operator- here gives other registers
            if (gMap_0x370_6F6268->HasBlockAnyArrows_492140(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                            gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                            (gCharB4_Saved_Zpos_6FD7FC.mValue - kFP16One_6FD9E8.mValue) >> 14))
            {
                Char::DoJump_5454D0();
                field_38_velocity = kZeroVelocity_6FD7C0;
                field_58_flags_bf.bFlag6 = true;
                field_40_rotation.SnapToAng4_405640();
            }
            else
            {
                field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::falling_19);
                field_16_state_init_pending = true;
                return;
            }
        }
        else
        {
            if (gMap_0x370_6F6268->HasBlockAnyArrows_492140(gCharB4_Saved_Xpos_6FD7F8.ToInt(),
                                                            gCharB4_Saved_Ypos_6FD800.ToInt(),
                                                            (gCharB4_Saved_Zpos_6FD7FC.mValue - kFP16One_6FD9E8.mValue) >> 14))
            {
                Char::DoJump_5454D0();
                field_58_flags_bf.bFlag6 = true;
                field_40_rotation.SnapToAng4_405640();
            }
        }
    }

    field_58_flags_bf.bReverseVelocity = false;
    if (field_38_velocity < kFP16Zero_6FD9E4)
    {
        field_58_flags_bf.bReverseVelocity = true;
        field_40_rotation = Ang16(field_40_rotation.rValue + kAng180_6FD936.rValue, static_cast<u8>(0));
        field_38_velocity = -field_38_velocity;
    }
    field_44_block_type = block_type;

    if (field_58_flags_bf.bFlag6)
    {
        desired_angle = field_40_rotation;
    }
    else
    {
        desired_angle = field_7C_pPed->GetTargetFacingAngle_492CC0();
    }
    target_rotation = desired_angle;
    if (field_10_char_state != Char_B4_state::Avoiding_Ped_10)
    {
        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            if (field_6C_animation_state != Char_Anim_state::Jumping_5)
            {
                field_6C_animation_state = Char_Anim_state::Jumping_5;
                field_68_animation_frame = 0;
                field_71_frame_delay = 4;
            }
            else if (!field_58_flags_bf.bFlag6)
            {
                if (!field_7C_pPed->field_224_bf.bWaitingOnTarget)
                {
                    desired_angle = field_7C_pPed->GetTargetFacingAngle_492CC0();
                }
                else
                {
                    desired_angle = field_40_rotation;
                }
                target_rotation = desired_angle;
            }

            if (!field_58_flags_bf.bFlag6)
            {
                if (field_7C_pPed->field_224_bf.bWaitingOnTarget)
                {
                    if (gCharB4_DistanceToTarget_6FD80C < kFpOneSixteenth_6FD87C)
                    {
                        field_38_velocity = kFpOne64th_6FD7B8;
                    }
                    else
                    {
                        if (gCharB4_DistanceToTarget_6FD80C < kFpOneEighth_6FD88C)
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
            field_38_velocity = kFpThreeSixtyFourths_6FD7CC;
        }
        bSnapRotation = 1;
    }
    if (field_55_next_tile_timer > 0)
    {
        field_55_next_tile_timer--;
    }

    if (field_38_velocity != kZeroVelocity_6FD7C0)
    {
        if (field_69_is_colliding_with_sprite)
        {
            bSnapRotation = 1;
            field_38_velocity = gRunOrJumpSpeed_6FD7D0;
        }

        if (field_58_flags_bf.bFlag6 || field_7C_pPed->IsPedGoingToEnterCar_492FD0())
        {
            bSnapRotation = 1;
        }
        angle = desired_angle;
        if (field_58_flags_bf.bFlag7)
        {
            if (MaxAbsDistance_5504F0(gCharB4_Saved_Xpos_6FD7F8,
                                      gCharB4_Saved_Ypos_6FD800,
                                      kFP16Half_6FD8E4 + Fix16(field_72_next_tile_x),
                                      kFP16Half_6FD8E4 + Fix16(field_73_next_tile_y)) < kFP16Quarter_6FD828)
            {
                field_58_flags_bf.bFlag7 = false;
            }
            else
            {
                target_rotation = Fix16::atan2_fixed_405320(kFP16Half_6FD8E4 + Fix16(field_73_next_tile_y) - gCharB4_Saved_Ypos_6FD800,
                                                kFP16Half_6FD8E4 + Fix16(field_72_next_tile_x) - gCharB4_Saved_Xpos_6FD7F8);
                angle = target_rotation;
            }
        }
        else
        {
            if (field_6A_repulsion_timer && field_C_ped_state_2 != ped_state_2::running_away_3)
            {
                prev_angle = field_74_repulsion_rotation;
                cur_rotation = field_40_rotation;
                field_38_velocity = k_CollisionRepulsionSpeed_6FD7BC;
                target_rotation = prev_angle;
                saved_rotation = cur_rotation;
                bSnapRotation = 1;
            }
        }
        if (field_C_ped_state_2 == ped_state_2::running_away_3)
        {
            if (field_55_next_tile_timer > 0)
            {
                if (!field_58_flags_bf.bFlag7)
                {
                    target_rotation = field_80_sprite_ptr->field_0_rotation;
                    angle = target_rotation;
                }
            }
            Ang16::PolarToCartesian_41FC20(target_rotation, kFP16Half_6FD8E4, probe_x, probe_y);
        }
        else
        {
            Ang16::PolarToCartesian_41FC20(target_rotation, kFP16Quarter_6FD828, probe_x, probe_y);
        }
        probe_x += gCharB4_Saved_Xpos_6FD7F8;
        probe_y += gCharB4_Saved_Ypos_6FD800;
        x_u8 = probe_x.ToUInt8();
        y_u8 = probe_y.ToUInt8();
        if (start_tile_x != x_u8 || start_tile_y != y_u8)
        {
            Char::ManageZCoordAndSlopes_548590();

            if (!Char::CanReachTile_550090(x_u8, y_u8))
            {
                field_58_flags_bf.bFlag7 = true;
                if (field_C_ped_state_2 == ped_state_2::running_away_3)
                {
                    Char::ChooseNextMovementTile_551400();
                }
                else
                {
                    Char::SelectNextTileFast_5516F0();
                }
                target_rotation = gAng16_AngleOfCollision_6FD808;
                angle = gAng16_AngleOfCollision_6FD808;
            }
        }
    }

    if (field_10_char_state == 28 || field_10_char_state == 29 || bSnapRotation)
    {
        field_40_rotation = target_rotation;
    }
    else
    {
        next_rotation = Char::GetNextRotationToward_550F60(target_rotation);
        if (Char::CanStepInDirection_551350(next_rotation) == 1)
        {
            field_40_rotation = next_rotation;
            bMoved = 1;
        }
        else if (angle != target_rotation)
        {
            field_40_rotation = angle;
        }
        else
        {
            field_40_rotation = target_rotation;
        }
    }

    if (field_58_flags_bf.bFlag6)
    {
        field_38_velocity = gRunOrJumpSpeed_6FD7D0;
    }

    Fix16 nx = field_80_sprite_ptr->field_14_xy.x + Ang16::sine_40F500(field_40_rotation) * field_38_velocity;
    Fix16 ny = field_80_sprite_ptr->field_14_xy.y + Ang16::cosine_40F520(field_40_rotation) * field_38_velocity;
    field_80_sprite_ptr->set_xyz_lazy_420600(nx, ny, field_80_sprite_ptr->field_1C_zpos);

    field_80_sprite_ptr->set_ang_lazy_420690(field_40_rotation);
    if (field_69_is_colliding_with_sprite)
    {
        bMoved = 1;
    }

    if (field_7C_pPed->IsPedGoingToEnterCar_492FD0())
    {
        bMoved = 1;
    }
    if (bMoved == 1 || field_58_flags_bf.bOnSlope == true)
    {
        gCharB4_GradientSlopeBelow_6FDB54 = gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                  field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                  field_80_sprite_ptr->field_1C_zpos.ToInt() - 1);

        Char::ManageZCoordAndSlopes_548590();
    }
    Char::DispatchCollision_548670(byte_6FDB56);

    if (field_69_is_colliding_with_sprite)
    {
        if (Char::ContinueMovementAfterCollision_54B8F0() == 1)
        {
            Fix16 x = field_80_sprite_ptr->field_14_xy.x;
            Fix16 y = field_80_sprite_ptr->field_14_xy.y;

            x_u8 = x.ToUInt8();
            y_u8 = y.ToUInt8();
            if (gCharB4_Saved_Xpos_6FD7F8.ToUInt8() != x_u8 || gCharB4_Saved_Ypos_6FD800.ToUInt8() != y_u8)
            {
                field_80_sprite_ptr->set_xyz_lazy_420600(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);

                field_45_slope_gradient_direction = gCharB4_Saved_SlopeGradDir_6FD7B0.ToUInt8();
                if (!Char::CanReachTile_550090(x_u8, y_u8))
                {
                    field_69_is_colliding_with_sprite = false;
                    field_5C_jump_over_timer = 10;
                    if (gCharCollisionNoJump_6FDB58)
                    {
                        s16 rnd = gRng_6F6784.get_int_4F7AE0(4);
                        switch (rnd)
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

                    if (bMoved == 1 || field_58_flags_bf.bOnSlope == true)
                    {

                        gCharB4_GradientSlopeBelow_6FDB54 =
                            gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_80_sprite_ptr->field_14_xy.x.ToInt(),
                                                                        field_80_sprite_ptr->field_14_xy.y.ToInt(),
                                                                        (field_80_sprite_ptr->field_1C_zpos - kFP16One_6FD9E8).ToInt());
                        Char::ManageZCoordAndSlopes_548590();
                    }
                }
            }
        }
        else
        {
            field_69_is_colliding_with_sprite = false;
            field_2A_slide_turn = kAng0_6FDB34;
            field_5C_jump_over_timer = 10;
        }
    }

    if (field_6A_repulsion_timer)
    {
        field_40_rotation = saved_rotation;
    }
    if (field_58_flags_bf.bReverseVelocity)
    {
        field_40_rotation = Ang16(field_40_rotation.rValue + kAng180_6FD936.rValue, static_cast<u8>(0));
        field_38_velocity = -field_38_velocity;
    }
    Char::CheckCollisionsAndUpdateAnim_54DD70();
}

// https://decomp.me/scratch/4eLOQ
MATCH_FUNC(0x550f60)
Ang16 Char::GetNextRotationToward_550F60(Ang16 inputAng)
{
    u8 side_curr = field_40_rotation.ToAng4_405680();
    u8 side_input_ang = inputAng.ToAng4_405680();

    // 9.6f has no trace of this; 10.5 constructs the step angle from it and then overwrites it.
    // The product is a temporary inside the inline: the step angle sits in the temporaries area.
    Ang16 max_turn = kAng4_6FDB2E.MultiplyByFix16_401CB0(field_38_velocity);
    Ang16 result;

    if (field_10_char_state == Char_B4_state::Avoiding_Ped_10)
    {
        return inputAng;
    }

    if (field_38_velocity > kZeroVelocity_6FD7C0)
    {
        if (field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC)
        {
            max_turn = kAng12_6FD892;
        }
        else
        {
            max_turn = kAng24_6FD890;
        }
    }
    else
    {
        max_turn = kAng45_6FD89C;
    }

    switch (side_input_ang)
    {
        case 3: // west
            if (side_curr <= 1)
            {
                if (ComputeShortestAngleDelta_4056C0(inputAng, field_40_rotation) > kAng180_6FD936)
                {
                    result = field_40_rotation + max_turn;
                }
                else
                {
                    result = field_40_rotation - max_turn;
                }
                return result;
            }
            break;
        case 2: // north
            if (side_curr == 0)
            {
                if (ComputeShortestAngleDelta_4056C0(inputAng, field_40_rotation) > kAng180_6FD936)
                {
                    result = field_40_rotation + max_turn;
                }
                else
                {
                    result = field_40_rotation - max_turn;
                }
                return result;
            }
            break;
        case 1: // east
            if (side_curr == 3)
            {
                if (ComputeShortestAngleDelta_4056C0(field_40_rotation, inputAng) < kAng180_6FD936)
                {
                    result = field_40_rotation - max_turn;
                }
                else
                {
                    result = field_40_rotation + max_turn;
                }
                return result;
            }
            break;
        case 0: // south
            switch (side_curr)
            {
                case 2:
                    if (ComputeShortestAngleDelta_4056C0(field_40_rotation, inputAng) < kAng180_6FD936)
                    {
                        result = field_40_rotation - max_turn;
                    }
                    else
                    {
                        result = field_40_rotation + max_turn;
                    }
                    return result;
                    break;
                case 3:
                    if (ComputeShortestAngleDelta_4056C0(inputAng, field_40_rotation) < max_turn)
                    {
                        return inputAng;
                    }
                    else
                    {
                        return field_40_rotation + max_turn;
                    }
                    break;
            }
            break;
    }

    // Close enough to the target: snap to it (the original returns inputAng here, not field_40_rotation)
    if (inputAng > field_40_rotation)
    {
        if (ComputeShortestAngleDelta_4056C0(inputAng, field_40_rotation) > max_turn)
        {
            result = field_40_rotation + max_turn;
        }
        else
        {
            result = inputAng;
        }
        return result;
    }
    else
    {
        if (ComputeShortestAngleDelta_4056C0(field_40_rotation, inputAng) > max_turn)
        {
            result = field_40_rotation - max_turn;
        }
        else
        {
            result = inputAng;
        }
    }
    return result;
}

MATCH_FUNC(0x551350)
bool Char::CanStepInDirection_551350(Ang16 ang)
{
    Fix16 old_x = field_80_sprite_ptr->field_14_xy.x;
    Fix16 old_y = field_80_sprite_ptr->field_14_xy.y;
    Fix16 x_pos;
    Fix16 y_pos;

    if (field_C_ped_state_2 == ped_state_2::running_away_3)
    {
        Ang16::PolarToCartesian_41FC20(ang, kFpHalf_6FDB18, x_pos, y_pos);
    }
    else
    {
        Ang16::PolarToCartesian_41FC20(ang, kFpThreeSixteenths_6FDB08, x_pos, y_pos);
    }
    x_pos += old_x;
    y_pos += old_y;
    return Char::CanReachTile_550090(x_pos.ToInt(), y_pos.ToInt());
}

MATCH_FUNC(0x551400)
void Char::ChooseNextMovementTile_551400()
{
    s32 saved_direction = gCharB4_PathDirection_623F44;
    switch (gCharB4_PathDirection_623F44)
    {
        case path_direction::up_1:
        case path_direction::down_2:
            if (field_7C_pPed->GetMoveTargetX_492CE0() <= gCharB4_Saved_Xpos_6FD7F8)
            {
                gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 + 1, gCharB4_Saved_TileY_6FDAD9))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                    field_60_unused = 3;
                    field_55_next_tile_timer = 40;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                    if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 - 1, gCharB4_Saved_TileY_6FDAD9))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                        field_60_unused = 4;
                        field_55_next_tile_timer = 40;
                        return;
                    }
                }
            }
            else
            {
                gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 - 1, gCharB4_Saved_TileY_6FDAD9))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                    field_60_unused = 4;
                    field_55_next_tile_timer = 40;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                    if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 + 1, gCharB4_Saved_TileY_6FDAD9))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                        field_60_unused = 3;
                        field_55_next_tile_timer = 40;
                        return;
                    }
                }
            }

            if (saved_direction == path_direction::up_1)
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

        case path_direction::right_3:
        case path_direction::left_4:

            if (field_7C_pPed->GetMoveTargetY_492CF0() <= gCharB4_Saved_Ypos_6FD800)
            {
                gAng16_AngleOfCollision_6FD808 = kAng0_6FDB3C;
                if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 + 1))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
                    field_55_next_tile_timer = 40;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                    if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 - 1))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
                        field_55_next_tile_timer = 40;
                        return;
                    }
                }
            }
            else
            {
                gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 - 1))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
                    field_55_next_tile_timer = 40;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng0_6FDB3C;
                    if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 + 1))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
                        field_55_next_tile_timer = 40;
                        return;
                    }
                }
            }

            if (saved_direction == path_direction::right_3)
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
    field_55_next_tile_timer = 40;
}

MATCH_FUNC(0x5516f0)
void Char::SelectNextTileFast_5516F0()
{
    s32 saved_direction = gCharB4_PathDirection_623F44;
    switch (gCharB4_PathDirection_623F44)
    {
        case path_direction::up_1:
        case path_direction::down_2:
            if (field_7C_pPed->GetMoveTargetX_492CE0() >= gCharB4_Saved_Xpos_6FD7F8)
            {
                gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 + 1, gCharB4_Saved_TileY_6FDAD9))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng270_6FD904;
                    if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 - 1, gCharB4_Saved_TileY_6FDAD9))
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
                if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 - 1, gCharB4_Saved_TileY_6FDAD9))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 - 1;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng90_6FDA64;
                    if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8 + 1, gCharB4_Saved_TileY_6FDAD9))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8 + 1;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9;
                        return;
                    }
                }
            }

            if (saved_direction == path_direction::up_1)
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

        case path_direction::right_3:
        case path_direction::left_4:

            if (field_7C_pPed->GetMoveTargetY_492CF0() >= gCharB4_Saved_Ypos_6FD800)
            {
                gAng16_AngleOfCollision_6FD808 = kAng0_6FDB3C;
                if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 + 1))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng180_6FD8E8;
                    if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 - 1))
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
                if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 - 1))
                {
                    field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                    field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 - 1;
                    return;
                }
                else
                {
                    gAng16_AngleOfCollision_6FD808 = kAng0_6FDB3C;
                    if (Char::CanReachTile_550090(gCharB4_Saved_TileX_6FDAD8, gCharB4_Saved_TileY_6FDAD9 + 1))
                    {
                        field_72_next_tile_x = gCharB4_Saved_TileX_6FDAD8;
                        field_73_next_tile_y = gCharB4_Saved_TileY_6FDAD9 + 1;
                        return;
                    }
                }
            }

            if (saved_direction == path_direction::right_3)
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
void Char::state_2_5519F0()
{
    Char::state_1_5504F0();
}

MATCH_FUNC(0x551A00)
void Char::state_3_551A00()
{
    if (field_C_ped_state_2 == ped_state_2::ped2_following_a_car_4)
    {
        field_58_flags_bf.bFlag7 = false;
        Char::state_1_5504F0();
        if (!field_7C_pPed->GetAttacking_433CA0() && field_10_char_state != Char_B4_state::Jumping_15)
        {
            if (field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC)
            {
                field_6C_animation_state = Char_Anim_state::Running_1;
            }
            else
            {
                if (field_38_velocity != kZeroVelocity_6FD7C0)
                {
                    field_6C_animation_state = Char_Anim_state::Walking_0;
                }
                else
                {
                    field_6C_animation_state = Char_Anim_state::Standing_2;
                    if (field_84_target_car)
                    {
                        field_40_rotation = field_84_target_car->field_50_car_sprite->field_0_rotation;
                    }
                }
            }
        }
    }
    else if (field_C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
    {
        field_10_char_state = Char_B4_state::Interacting_Car_Door_36;
        Sprite* pNearestSprite = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_80_sprite_ptr, sprite_types_enum::none_0);
        if (!pNearestSprite || pNearestSprite->GetType_416B40() != sprite_types_enum::car_2 ||
            (pNearestSprite->field_8_car_bc_ptr == field_7C_pPed->get_target_to_enter_403B10()) ||
            pNearestSprite->field_8_car_bc_ptr->IsTrailerCar_421720() || field_7C_pPed->CloseDoorIfCarApproaching_45BD20(pNearestSprite->field_8_car_bc_ptr))
        {
            if (field_6C_animation_state != Char_Anim_state::Entering_Car_6)
            {
                field_40_rotation = field_84_target_car->field_50_car_sprite->field_0_rotation;
                field_6C_animation_state = Char_Anim_state::Entering_Car_6;
                field_68_animation_frame = 0;
                if (field_84_target_car->IsDoorOffsetLarge_43B540(field_7C_pPed->get_target_car_door_403A60()))
                {
                    field_58_flags_bf.bFlag4 = true;
                }
                else
                {
                    field_58_flags_bf.bFlag4 = false;
                }
                field_70_frame_timer = 0;
            }
        }
    }
}

MATCH_FUNC(0x551B30)
void Char::state_4_551B30()
{
    if (field_6C_animation_state != Char_Anim_state::Exiting_Car_7)
    {
        field_6C_animation_state = Char_Anim_state::Exiting_Car_7;
        field_68_animation_frame = 0;
        s8 target_door = field_7C_pPed->get_target_car_door_403A60();
        if (field_84_target_car->IsDoorOffsetLarge_43B540(target_door))
        {
            field_58_flags_bf.bFlag4 = true;
        }
        else
        {
            field_58_flags_bf.bFlag4 = false;
        }
        field_70_frame_timer = 0;
    }
    if (field_10_char_state == Char_B4_state::Jumping_15)
    {
        field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::walking_0);
        field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
    }
    if (static_cast<u8>(Char::IsOnWater_545570()))
    {
        field_7C_pPed->PutOutFire();
        Char::DrownPed_5459E0();
    }
}

MATCH_FUNC(0x551BB0)
void Char::state_5_551BB0()
{
    if (field_C_ped_state_2 == ped_state_2::ped2_following_a_car_4)
    {
        this->field_58_flags_bf.bFlag7 = false;

        state_1_5504F0();

        if (!this->field_7C_pPed->GetAttacking_433CA0() && this->field_10_char_state != Char_B4_state::Jumping_15)
        {
            if (field_38_velocity > k_CollisionRepulsionSpeed_6FD7BC)
            {

                this->field_6C_animation_state = Char_Anim_state::Running_1;
            }
            else if (field_38_velocity != kZeroVelocity_6FD7C0)
            {
                this->field_6C_animation_state = Char_Anim_state::Walking_0;
            }
            else
            {
                this->field_6C_animation_state = Char_Anim_state::Standing_2;
                this->field_40_rotation = field_84_target_car->field_50_car_sprite->field_0_rotation;
            }
        }
    }
    else if (field_C_ped_state_2 == ped_state_2::ped2_entering_a_car_6 && this->field_6C_animation_state != Char_Anim_state::Entering_Car_6)
    {
        this->field_40_rotation = field_84_target_car->field_50_car_sprite->field_0_rotation;
        field_84_target_car->GetDoorWorldPosition_43B5A0(field_7C_pPed->get_target_car_door_403A60(),
                                              &field_80_sprite_ptr->field_14_xy.x,
                                              &field_80_sprite_ptr->field_14_xy.y);
        this->field_6C_animation_state = Char_Anim_state::Entering_Car_6;
        this->field_68_animation_frame = 0;
        if (field_84_target_car->IsDoorOffsetLarge_43B540(field_7C_pPed->get_target_car_door_403A60()))
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
void Char::state_7_551CB0()
{
    u8 block_type;
    gmp_block_info* pBlockAtPos;
    field_38_velocity = kZeroVelocity_6FD7C0;
    if (field_10_char_state != Char_B4_state::Jumping_15)
    {
        Char::CheckAndHandleCollisions_5459C0();
    }
    if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) == true)
    {
        field_40_rotation += field_7C_pPed->GetTurnSpeed_45C900();

        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            Char::state_0_54DDF0();
            SetPedState1_433910(0);
            SetPedState2_433A50(0);
            return;
        }
        if (field_58_flags_bf.bOnSlope || gCharB4_Saved_Zpos_6FD7FC.GetFracValue() != kFP16Zero_6FD9E4)
        {
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());
            pBlockAtPos = gMap_0x370_6F6268->GetBlock_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt());
        }
        else
        {
            block_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
            pBlockAtPos = gMap_0x370_6F6268->GetBlock_4DFE10(gCharB4_Saved_Xpos_6FD7F8.ToInt(), gCharB4_Saved_Ypos_6FD800.ToInt(), gCharB4_Saved_Zpos_6FD7FC.ToInt() - 1);
        }
        if (pBlockAtPos)
        {
            if (gGtx_0x106C_703DD4->IsElectrifiedFloorType_491F80(pBlockAtPos->field_8_lid & block_face::tile_idx_mask))
            {
                if (!field_7C_pPed->field_21C_bf.bLeftVehicle)
                {
                    field_7C_pPed->field_210_shock_counter += 3;
                    if (field_7C_pPed->field_204_killer_id != 0)
                    {
                        if (gPedManager_6787BC->PedById(field_7C_pPed->field_204_killer_id))
                        {
                            field_7C_pPed->field_290_death_cause = ped_death_cause::electrocuted_2;
                            field_7C_pPed->field_264_killer_id_timer = 50;
                        }
                    }
                }
            }
        }
        if (field_6C_animation_state != Char_Anim_state::Punching_4)
        {
            field_6C_animation_state = Char_Anim_state::Standing_2;
        }
    }
    else
    {
        field_40_rotation = field_7C_pPed->GetTargetFacingAngle_492CC0();
    }
    if (field_10_char_state == Char_B4_state::Jumping_15)
    {
        field_38_velocity = gRunOrJumpSpeed_6FD7D0;
    }
    if (field_6A_repulsion_timer > 0 || field_10_char_state == Char_B4_state::Jumping_15 || field_4C_conveyor_dx != kFP16Zero_6FD9E4 || field_50_conveyor_dy != kFP16Zero_6FD9E4)
    {
        Set_F8_ped_state_1_433910(ped_state_1::walking_0);
        field_C_ped_state_2 = ped_state_2::ped2_walking_0;
        Char::state_0_54DDF0();
        Set_F8_ped_state_1_433910(ped_state_1::standing_still_7);
        field_C_ped_state_2 = ped_state_2::ped2_staying_14;
        if (field_10_char_state == Char_B4_state::Jumping_15)
        {
            return;
        }
    }
    else
    {
        // Raw compares: the Fix16 operator== inlines push this function over VC6's inline budget
        if (!field_58_flags_bf.bOnSlope &&
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

        if (static_cast<u8>(Char::IsOnWater_545570()))
        {
            field_7C_pPed->PutOutFire();
            Char::DrownPed_5459E0();
            return;
        }
        if (block_type == AIR && !field_58_flags_bf.bOnSlope && field_10_char_state != Char_B4_state::Jumping_15)
        {
            field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
            field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::falling_19);
            field_16_state_init_pending = true;
            return;
        }
    }

    switch (field_C_ped_state_2)
    {
        case ped_state_2::ped2_staying_14:
            if (field_7C_pPed->GetAttacking_433CA0() == 1)
            {
                if (field_7C_pPed->field_21C_bf.bNoWeapon)
                {
                    if (field_6C_animation_state != Char_Anim_state::Punching_4)
                    {
                        field_6C_animation_state = Char_Anim_state::Punching_4;
                        field_68_animation_frame = 0;
                    }
                }
                else
                {
                    if (field_6C_animation_state == Char_Anim_state::Punching_4)
                    {
                        if (field_68_animation_frame == 0)
                        {
                            field_6C_animation_state = Char_Anim_state::Standing_2;
                        }
                    }
                    else
                    {
                        field_6C_animation_state = Char_Anim_state::Standing_2;
                    }
                }
            }
            else
            {
                if (field_6C_animation_state == Char_Anim_state::Punching_4)
                {
                    if (field_68_animation_frame == 0)
                    {
                        field_6C_animation_state = Char_Anim_state::Standing_2;
                    }
                }
                else
                {
                    if ((field_7C_pPed->PedTypeIs_45EDE0(ped_type::script_created_5) || field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2)) && !field_4A_timeout)
                    {
                        if (gRng_6F6784.get_int_4F7AE0(600) < 4u && field_10_char_state != Char_B4_state::Smoking_35)
                        {
                            field_10_char_state = Char_B4_state::Smoking_35;
                            field_68_animation_frame = 0;
                            field_70_frame_timer = 0;
                        }
                    }
                    field_6C_animation_state = Char_Anim_state::Standing_2;
                }
            }
            break;
        case ped_state_2::carjack_opening_door_8:
            field_6C_animation_state = Char_Anim_state::Carjacking_9;
            break;
        case ped_state_2::carjack_pulling_out_9:
            field_6C_animation_state = Char_Anim_state::Carjacking_9;
            break;
        default:
            field_6C_animation_state = Char_Anim_state::Standing_2;
            break;
    }
}

// https://decomp.me/scratch/x9UcS
MATCH_FUNC(0x5520A0)
void Char::state_8_5520A0()
{
    Fix16 nudge_x;
    Fix16 nudge_y;
    Fix16 ground_z;

    Sprite* pNearestSprite;
    Fix16 fall_x;
    Fix16 fall_y;
    Sprite* pAttachSprite;
    Sprite* pPedObjSprite;

    Ang16 rotation;

    s16 tile_frac_x = 0;
    s16 tile_frac_y = 0;

    field_7C_pPed->ClearAttacking_403A40();
    if (field_16_state_init_pending == 1)
    {
        field_16_state_init_pending = false;
        switch (field_C_ped_state_2)
        {
            case ped_state_2::lying_on_floor_22:
                if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2))
                {
                    field_48_lying_on_floor_timer = 20;
                }
                else
                {
                    field_48_lying_on_floor_timer = 60;
                }

                switch (field_10_char_state)
                {
                    case Char_B4_state::Hit_From_Front_33:
                        if (field_6C_animation_state != Char_Anim_state::Knocked_Down_Hit_Front_15)
                        {
                            field_6C_animation_state = Char_Anim_state::Knocked_Down_Hit_Front_15;
                            field_68_animation_frame = 0;
                        }
                        break;
                    case Char_B4_state::Hit_From_Behind_34:
                        if (field_6C_animation_state != Char_Anim_state::Knocked_Down_Hit_Behind_16)
                        {
                            field_40_rotation = field_40_rotation + kAng180_6FD936;
                            field_6C_animation_state = Char_Anim_state::Knocked_Down_Hit_Behind_16;
                            field_68_animation_frame = 0;
                        }
                        break;
                    default:
                        field_6C_animation_state = Char_Anim_state::Lying_On_Floor_10;
                        break;
                }
                field_80_sprite_ptr->SetDrawLayer_40F7B0(6);
                break;

            case ped_state_2::falling_19:
                field_6C_animation_state = Char_Anim_state::Normal_Fall_11;
                field_90_fall_speed = field_38_velocity;
                field_94_fall_z_speed = kFP16Zero_6FD9E4;
                field_68_animation_frame = 0;
                GetTileFracX64_545640(gCharB4_Saved_Xpos_6FD7F8, &tile_frac_x);
                GetTileFracY64_545670(gCharB4_Saved_Ypos_6FD800, &tile_frac_y);
                nudge_x = kFP16Zero_6FD9E4;
                nudge_y = kFP16Zero_6FD9E4;
                if (tile_frac_x < 10)
                {
                    nudge_x = kFpOneSixteenth_6FD87C;
                }
                if (tile_frac_x > 54)
                {
                    nudge_x = -kFpOneSixteenth_6FD87C;
                }
                if (tile_frac_y < 10)
                {
                    nudge_y = kFpOneSixteenth_6FD87C;
                }
                if (tile_frac_y > 54)
                {
                    nudge_y = -kFpOneSixteenth_6FD87C;
                }
                field_80_sprite_ptr->set_xyz_lazy_420600(nudge_x + field_80_sprite_ptr->field_14_xy.x,
                                                         nudge_y + field_80_sprite_ptr->field_14_xy.y,
                                                         field_80_sprite_ptr->field_1C_zpos);
                break;

            case ped_state_2::sinking_20:
                field_6C_animation_state = Char_Anim_state::Standing_2;
                field_46_timer = 10;
                break;

            case ped_state_2::knocked_recover_24:
            case ped_state_2::knocked_down_25:
            case ped_state_2::knocked_fatal_26:
                if (!field_7C_pPed->field_184_pObj2C)
                {
                    field_7C_pPed->Kill_46F9D0();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    field_C_ped_state_2 = ped_state_2::ped2_dead_15;
                    field_7C_pPed->field_278_ped_state_1 = ped_state_1::dead_9;
                    field_7C_pPed->field_27C_ped_state_2 = ped_state_2::ped2_dead_15;
                    return;
                }
                if (!field_7C_pPed->field_184_pObj2C->field_10_obj_3c)
                {
                    field_7C_pPed->Kill_46F9D0();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    field_C_ped_state_2 = ped_state_2::ped2_dead_15;
                    field_7C_pPed->field_278_ped_state_1 = ped_state_1::dead_9;
                    field_7C_pPed->field_27C_ped_state_2 = ped_state_2::ped2_dead_15;
                    return;
                }
                if (!field_7C_pPed->field_184_pObj2C->field_4_sprite)
                {
                    field_7C_pPed->Kill_46F9D0();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    field_C_ped_state_2 = ped_state_2::ped2_dead_15;
                    field_7C_pPed->field_278_ped_state_1 = ped_state_1::dead_9;
                    field_7C_pPed->field_27C_ped_state_2 = ped_state_2::ped2_dead_15;
                    return;
                }
                field_6C_animation_state = Char_Anim_state::Lethal_Fall_12;
                field_80_sprite_ptr->set_xyz_lazy_420600(field_7C_pPed->field_184_pObj2C->field_4_sprite->field_14_xy.x,
                                                         field_7C_pPed->field_184_pObj2C->field_4_sprite->field_14_xy.y,
                                                         field_7C_pPed->field_184_pObj2C->field_4_sprite->field_1C_zpos);
                if (field_C_ped_state_2 != ped_state_2::knocked_recover_24)
                {
                    field_40_rotation = field_7C_pPed->field_184_pObj2C->field_4_sprite->field_0_rotation;
                }
                if (!field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_34_sprite_collide_mode)
                {
                    field_80_sprite_ptr->SetDrawLayer_40F7B0(6);
                }
                field_4A_timeout = 300;
                break;

            case ped_state_2::ped2_at_car_door_17:
                field_6C_animation_state = Char_Anim_state::Getting_pulled_out_of_car_8;
                field_68_animation_frame = 0;
                break;

            case ped_state_2::electrocuted_27:
                field_6C_animation_state = Char_Anim_state::Electrocuted_17;
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
                rotation = kAng180_6FD936 + field_80_sprite_ptr->field_0_rotation;
                gParticle_8_6FD5E8->EmitWaterSplash_53F060(field_80_sprite_ptr->field_14_xy.x,
                                                           field_80_sprite_ptr->field_14_xy.y,
                                                           field_80_sprite_ptr->field_1C_zpos,
                                                           rotation,
                                                           1);
                if ((field_46_timer % 2) == 0)
                {
                    field_80_sprite_ptr->ShrinkSprite_59E390(kFpOneThirtySecond_6FDB24, kFpOneThirtySecond_6FDB24, 0);
                }
                if (field_46_timer == 0)
                {
                    // Ped sunk
                    field_7C_pPed->Kill_46F9D0();
                    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::dead_9);
                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_dead_15);
                    field_7C_pPed->RestorePreviousPedState_45C5A0();
                    field_80_sprite_ptr->set_z_lazy_420660(kFP16Zero_6FD9E4);
                    field_7C_pPed->field_224_bf.bDropsWeaponOnDeath = false;
                }
                break;

            case ped_state_2::lying_on_floor_22:

                if (field_48_lying_on_floor_timer == 0)
                {
                    if (!field_7C_pPed->field_21C_bf.bBusted)
                    {
                        field_7C_pPed->field_210_shock_counter = 0;
                        if (field_7C_pPed->field_280_stored_ped_state_1 == ped_state_1::dead_9)
                        {
                            field_7C_pPed->RestorePreviousPedState_45C5A0();
                            if (!field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) && !field_7C_pPed->PedTypeIs_45EDE0(ped_type::script_created_5))
                            {
                                field_7C_pPed->SetObjective(objectives_enum::dead_patient_28);
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
                                field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::walking_0);
                                field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                                field_7C_pPed->RestorePreviousPedState_45C5A0();
                            }

                            field_6C_animation_state = Char_Anim_state::Walking_0;
                        }
                        gSpriteGrid_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);
                        if (field_4C_conveyor_dx != kFP16Zero_6FD9E4 || field_50_conveyor_dy != kFP16Zero_6FD9E4)
                        {
                            field_80_sprite_ptr->set_xyz_lazy_420600(field_4C_conveyor_dx + field_80_sprite_ptr->field_14_xy.x,
                                                                     field_50_conveyor_dy + field_80_sprite_ptr->field_14_xy.y,
                                                                     field_80_sprite_ptr->field_1C_zpos);
                        }
                    }
                }

                break;

            case ped_state_2::ped2_at_car_door_17:
                if (!field_84_target_car->IsDoorAccessible_43AFE0(field_7C_pPed->field_24C_target_car_door))
                {
                    field_68_animation_frame = 3;
                }
                if (field_68_animation_frame == 3)
                {

                    if (field_84_target_car->field_58_physics)
                    {
                        field_7C_pPed->field_184_pObj2C =
                            gObject_5C_6F8F84->NewMovingObj_52A240(objects::ped_pushed_110,
                                                                   field_80_sprite_ptr->field_14_xy.x,
                                                                   field_80_sprite_ptr->field_14_xy.y,
                                                                   field_80_sprite_ptr->field_1C_zpos,
                                                                   field_84_target_car->field_50_car_sprite->field_0_rotation,
                                                                   field_80_sprite_ptr->field_0_rotation,
                                                                   field_84_target_car->field_58_physics->vec_len_552DE0(),
                                                                   -kFpOne64th_6FD868,
                                                                   0);
                    }
                    else
                    {
                        field_7C_pPed->field_184_pObj2C =
                            gObject_5C_6F8F84->NewMovingObj_52A240(objects::ped_pushed_110,
                                                                   field_80_sprite_ptr->field_14_xy.x,
                                                                   field_80_sprite_ptr->field_14_xy.y,
                                                                   field_80_sprite_ptr->field_1C_zpos,
                                                                   field_84_target_car->field_50_car_sprite->field_0_rotation,
                                                                   field_80_sprite_ptr->field_0_rotation,
                                                                   kFpOneSixteenth_6FD87C,
                                                                   -kFpOne64th_6FD868,
                                                                   0);
                    }

                    field_7C_pPed->field_280_stored_ped_state_1 = ped_state_1::walking_0;
                    field_7C_pPed->field_284_stored_ped_state_2 = ped_state_2::ped2_walking_0;
                    field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                    gParticle_8_6FD5E8->EmitBloodBurst_53E450(field_80_sprite_ptr->field_14_xy.x,
                                                              field_80_sprite_ptr->field_14_xy.y,
                                                              field_80_sprite_ptr->field_1C_zpos,
                                                              field_80_sprite_ptr->field_0_rotation);
                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_down_25);
                    field_C_ped_state_2 = ped_state_2::knocked_recover_24;
                    field_6C_animation_state = Char_Anim_state::Lying_On_Floor_10;
                }
                break;
            case ped_state_2::falling_19:
                ground_z = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(gCharB4_Saved_Xpos_6FD7F8, gCharB4_Saved_Ypos_6FD800, gCharB4_Saved_Zpos_6FD7FC);
                if (gCharB4_Saved_Zpos_6FD7FC < ground_z || gCharB4_Saved_Zpos_6FD7FC >= ground_z + kFP16Eighth_6FDB04)
                {
                    Ang16::PolarToCartesian_41FC20(field_40_rotation, field_90_fall_speed, fall_x, fall_y);
                    fall_x += gCharB4_Saved_Xpos_6FD7F8;
                    fall_y += gCharB4_Saved_Ypos_6FD800;
                    if (Char::CanMoveToTile_54C500(fall_x.ToInt(), fall_y.ToInt()))
                    {
                        field_80_sprite_ptr->set_xyz_lazy_420600(fall_x, fall_y, field_80_sprite_ptr->field_1C_zpos);
                    }
                    if (field_94_fall_z_speed < kFpPoint1_6FD824)
                    {
                        field_94_fall_z_speed += kFpPoint01_6FD9A0;
                    }
                    field_80_sprite_ptr->field_1C_zpos -= field_94_fall_z_speed;
                    if (field_90_fall_speed > kFP16One_6FD9E8)
                    {
                        field_90_fall_speed -= kFpPoint2_6FD82C;
                    }
                }
                else
                {
                    field_80_sprite_ptr->field_1C_zpos = ground_z;
                    field_7C_pPed->RestorePreviousPedState_45C5A0();
                    Set_F8_ped_state_1_433910(field_7C_pPed->GetPedState_403990());
                    field_C_ped_state_2 = field_7C_pPed->GetPedState2_433B60();
                    pNearestSprite = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_80_sprite_ptr, sprite_types_enum::none_0);
                    if (field_6C_animation_state == Char_Anim_state::Lethal_Fall_12)
                    {
                        if (!field_7C_pPed->field_21C_bf.bOnFire)
                        {
                            field_7C_pPed->field_250_voice_event = 27;
                        }
                        if (pNearestSprite)
                        {
                            field_7C_pPed->field_184_pObj2C = gObject_5C_6F8F84->NewMovingObj_52A240(objects::ped_pushed_110,
                                                                                                     field_80_sprite_ptr->field_14_xy.x,
                                                                                                     field_80_sprite_ptr->field_14_xy.y,
                                                                                                     field_80_sprite_ptr->field_1C_zpos,
                                                                                                     field_80_sprite_ptr->field_0_rotation,
                                                                                                     field_80_sprite_ptr->field_0_rotation,
                                                                                                     kFpPoint1_6FD824,
                                                                                                     -kFpPoint01_6FD9A0,
                                                                                                     kFP16Zero_6FD9E4);

                            field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                            field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_fatal_26);
                            // 9.6f: Ped::SetStateInitPending_433B50 here and below (inlined, using it makes the diff worse)
                            field_7C_pPed->field_168_game_object->field_16_state_init_pending = true;
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
                        field_7C_pPed->SetVoiceEventIfNotOnFire_433DD0(26);
                        if (pNearestSprite)
                        {
                            Char::DoJump_5454D0();
                        }
                    }
                }
                break;

            case ped_state_2::knocked_recover_24:
            case ped_state_2::knocked_down_25:
            case ped_state_2::knocked_fatal_26:

                if (field_4A_timeout > 0)
                {
                    field_4A_timeout--;
                }
                if (field_4A_timeout == 0)
                {
                    field_C_ped_state_2 = ped_state_2::knocked_fatal_26;
                }
                if (!field_7C_pPed->field_184_pObj2C)
                {
                    field_7C_pPed->Kill_46F9D0();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    field_C_ped_state_2 = ped_state_2::ped2_dead_15;
                    field_7C_pPed->field_278_ped_state_1 = ped_state_1::dead_9;
                    field_7C_pPed->field_27C_ped_state_2 = ped_state_2::ped2_dead_15;
                    return;
                }
                if (field_7C_pPed->field_184_pObj2C->GetEffectiveSpeed_5290F0() == kFP16Zero_6FD9E4 || !field_4A_timeout)
                {
                    switch (field_C_ped_state_2)
                    {
                        case ped_state_2::knocked_recover_24:
                            field_7C_pPed->RestorePreviousPedState_45C5A0();
                            Set_F8_ped_state_1_433910(field_7C_pPed->field_278_ped_state_1);
                            field_C_ped_state_2 = field_7C_pPed->field_27C_ped_state_2;
                            break;
                        case ped_state_2::knocked_down_25:
                            field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                            field_7C_pPed->field_27C_ped_state_2 = ped_state_2::lying_on_floor_22;
                            field_7C_pPed->field_168_game_object->field_16_state_init_pending = true;
                            Set_F8_ped_state_1_433910(ped_state_1::immobilized_8);
                            field_C_ped_state_2 = ped_state_2::lying_on_floor_22;
                            break;
                        case ped_state_2::knocked_fatal_26:
                            field_7C_pPed->RestorePreviousPedState_45C5A0();
                            field_7C_pPed->Kill_46F9D0();
                            field_7C_pPed->SetVoiceEventIfNotOnFire_433DD0(27);
                            break;
                    }
                    pPedObjSprite = field_7C_pPed->field_184_pObj2C->field_4_sprite;

                    field_80_sprite_ptr->set_xyz_lazy_420600(pPedObjSprite->field_14_xy.x, pPedObjSprite->field_14_xy.y, pPedObjSprite->field_1C_zpos);

                    field_80_sprite_ptr->set_ang_lazy_420690(field_7C_pPed->field_184_pObj2C->field_4_sprite->field_0_rotation);

                    if (!field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_34_sprite_collide_mode)
                    {
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(6);
                    }
                    field_7C_pPed->field_184_pObj2C->RequestRemoval_5290A0();
                    field_7C_pPed->field_184_pObj2C = NULL;
                    Char::ManageZCoordAndSlopes_548590();
                    if (static_cast<u8>(Char::IsOnWater_545570()))
                    {
                        field_7C_pPed->PutOutFire();
                        Char::DrownPed_5459E0();
                        return;
                    }

                    gSpriteGrid_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);

                    if (field_4C_conveyor_dx != kFP16Zero_6FD9E4 || field_50_conveyor_dy != kFP16Zero_6FD9E4)
                    {

                        field_80_sprite_ptr->set_xyz_lazy_420600(field_4C_conveyor_dx + field_80_sprite_ptr->field_14_xy.x,
                                                                 field_50_conveyor_dy + field_80_sprite_ptr->field_14_xy.y,
                                                                 field_80_sprite_ptr->field_1C_zpos);
                    }
                }
                else
                {
                    pAttachSprite = field_7C_pPed->field_184_pObj2C->field_4_sprite;
                    field_80_sprite_ptr->set_xyz_lazy_420600(pAttachSprite->field_14_xy.x, pAttachSprite->field_14_xy.y, pAttachSprite->field_1C_zpos);

                    if (field_C_ped_state_2 != ped_state_2::knocked_recover_24)
                    {
                        field_40_rotation = field_7C_pPed->field_184_pObj2C->field_4_sprite->field_0_rotation;
                    }
                    if (!field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_34_sprite_collide_mode)
                    {
                        field_80_sprite_ptr->SetDrawLayer_40F7B0(6);
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
                    Set_F8_ped_state_1_433910(ped_state_1::walking_0);
                    field_C_ped_state_2 = ped_state_2::ped2_walking_0;
                    field_7C_pPed->field_27C_ped_state_2 = ped_state_2::ped2_walking_0;
                    field_7C_pPed->field_278_ped_state_1 = ped_state_1::walking_0;
                    field_7C_pPed->field_224_bf.bDropsWeaponOnDeath = false;
                    if (field_7C_pPed->field_15C_player)
                    {
                        gRoot_sound_66B038.PlayVoice_40F090(voice_line::shocking_25);
                        field_7C_pPed->field_15C_player->SetDeathType_434950(4);
                    }
                    field_7C_pPed->Kill_46F9D0();
                    Set_F8_ped_state_1_433910(ped_state_1::dead_9);
                    field_C_ped_state_2 = ped_state_2::ped2_dead_15;
                    field_6C_animation_state = Char_Anim_state::Charred_Corpse_21;
                    field_10_char_state = Char_B4_state::Walking_1;
                }

                break;
            default:
                break;
        }
    }
    if (field_7C_pPed->field_210_shock_counter < static_cast<u32>(field_7C_pPed->field_212_electrocution_threshold))
    {
        --field_48_lying_on_floor_timer;
    }
}

// https://decomp.me/scratch/4c8aU
MATCH_FUNC(0x552DE0)
Fix16 CarPhysics::vec_len_552DE0() // Weird location, I'm putting this here to preserve ordering
{
    return field_40_linvel_1.GetLength_41E260();
}

MATCH_FUNC(0x552E90)
void Char::state_9_552E90()
{
    s32 rng;

    field_7C_pPed->ClearAttacking_403A40();
    if (field_16_state_init_pending == 1)
    {
        switch (field_10_char_state)
        {
            case Char_B4_state::Hit_From_Front_33:
                if (field_6C_animation_state != Char_Anim_state::Knocked_Down_Hit_Front_15)
                {
                    field_6C_animation_state = Char_Anim_state::Knocked_Down_Hit_Front_15;
                    field_68_animation_frame = 0;
                }
                break;
            case Char_B4_state::Hit_From_Behind_34:
                if (field_6C_animation_state != Char_Anim_state::Knocked_Down_Hit_Behind_16)
                {
                    field_40_rotation = field_40_rotation + kAng180_6FD936;
                    field_6C_animation_state = Char_Anim_state::Knocked_Down_Hit_Behind_16;
                    field_68_animation_frame = 0;
                }
                break;
            default:
                if (field_6C_animation_state != Char_Anim_state::Charred_Corpse_21)
                {
                    rng = gRng_6F6784.get_int_4F7AE0(3);

                    switch (rng)
                    {
                        case 0:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_14;
                            break;
                        case 1:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_19;
                            break;
                        case 2:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_20;
                            break;
                    }
                }
                break;
        }

        field_16_state_init_pending = false;
        if (field_7C_pPed->field_164_ped_group)
        {
            if (field_7C_pPed->field_164_ped_group->field_0_remove_dead_members)
            {
                field_7C_pPed->field_164_ped_group->RemovePed_4C9970(field_7C_pPed);
                field_7C_pPed->field_164_ped_group = NULL;
            }
        }

        field_34_unused = 0;
        gSpriteGrid_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);
        Char::ManageZCoordAndSlopes_548590();
        field_80_sprite_ptr->set_xyz_lazy_420600(field_4C_conveyor_dx + field_80_sprite_ptr->field_14_xy.x,
                                                 field_50_conveyor_dy + field_80_sprite_ptr->field_14_xy.y,
                                                 field_80_sprite_ptr->field_1C_zpos);
    }
    else
    {
        switch (field_6C_animation_state)
        {
            case Char_Anim_state::Knocked_Down_Hit_Front_15:
                if (field_80_sprite_ptr->field_1C_zpos.ToUInt8() != 0 && field_80_sprite_ptr->field_14_xy.x.ToUInt8() > 1 &&
                    field_80_sprite_ptr->field_14_xy.y.ToUInt8() > 1 && field_80_sprite_ptr->field_14_xy.x.ToUInt8() < 254 &&
                    field_80_sprite_ptr->field_14_xy.y.ToUInt8() < 254)
                {
                    field_38_velocity = -k_CollisionRepulsionSpeed_6FD7BC;
                    Char::state_0_54DDF0();
                }
                else
                {
                    field_6C_animation_state = Char_Anim_state::Lying_Pose_14;
                }
                break;
            case Char_Anim_state::Knocked_Down_Hit_Behind_16:
                if (field_80_sprite_ptr->field_1C_zpos.ToUInt8() != 0 && field_80_sprite_ptr->field_14_xy.x.ToUInt8() > 1 &&
                    field_80_sprite_ptr->field_14_xy.y.ToUInt8() > 1 && field_80_sprite_ptr->field_14_xy.x.ToUInt8() < 254 &&
                    field_80_sprite_ptr->field_14_xy.y.ToUInt8() < 254)
                {
                    field_38_velocity = -k_CollisionRepulsionSpeed_6FD7BC;
                    Char::state_0_54DDF0();
                }
                else
                {
                    field_6C_animation_state = Char_Anim_state::Lying_Pose_14;
                }
                break;
            case Char_Anim_state::Lying_Pose_13:
            case Char_Anim_state::Lying_Pose_14:
            case Char_Anim_state::Lying_Pose_19:
            case Char_Anim_state::Lying_Pose_20:
                if (field_4C_conveyor_dx != kFP16Zero_6FD9E4 || field_50_conveyor_dy != kFP16Zero_6FD9E4)
                {
                    field_4C_conveyor_dx = kFP16Zero_6FD9E4;
                    field_50_conveyor_dy = kFP16Zero_6FD9E4;
                    gSpriteGrid_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);

                    field_80_sprite_ptr->set_xyz_lazy_420600(field_4C_conveyor_dx + field_80_sprite_ptr->field_14_xy.x,
                                                             field_50_conveyor_dy + field_80_sprite_ptr->field_14_xy.y,
                                                             field_80_sprite_ptr->field_1C_zpos);
                }

                field_80_sprite_ptr->field_28_draw_layer = 6;
                break;
            default:
                if (field_6C_animation_state != Char_Anim_state::Charred_Corpse_21)
                {
                    rng = gRng_6F6784.get_int_4F7AE0(3);
                    switch (rng)
                    {
                        case 0:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_14;
                            break;
                        case 1:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_19;
                            break;
                        case 2:
                            field_6C_animation_state = Char_Anim_state::Lying_Pose_20;
                            break;
                    }
                }
                field_68_animation_frame = 0;
                field_80_sprite_ptr->SetDrawLayer_40F7B0(6);
                break;
        }
    }
}

MATCH_FUNC(0x5532C0)
bool Char::IsNearTileCentre_5532C0() const
{
    s16 frac_x = 0;
    s16 frac_y = 0;
    Char::GetTileFracX64_545640(gCharB4_Saved_Xpos_6FD7F8, &frac_x);
    Char::GetTileFracY64_545670(gCharB4_Saved_Ypos_6FD800, &frac_y);
    if (frac_x > 20 && frac_x < 44 && frac_y > 20 && frac_y < 44)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x553330)
bool Char::IsThreatToSearchingPed_553330() const
{
    return field_7C_pPed->IsThreatToSearchingPed_4661F0();
}

// 9.6f 0x497480
MATCH_FUNC(0x553340)
bool Char::ShouldCollideWithSprite_553340(Sprite* pSprite) const
{
    Ped* pPed;

    if (pSprite)
    {
        switch (pSprite->GetType_416B40())
        {
            case sprite_types_enum::car_2:
                break;

            case sprite_types_enum::ped_3:
                break;

            default:
            {
                Object* pObj = pSprite->AsObject2C_40FEC0();
                if (pObj)
                {
                    u8 idx = field_7C_pPed->get_ped_ref_idx_420B50();
                    if (pObj->IsOwnedByPedRef_4973E0(idx))
                    {
                        return false;
                    }

                    if (pObj->IsSpriteGrid2Region3_4210B0())
                    {
                        return false;
                    }
                }
            }
            break;
        }
    }

    switch (this->field_C_ped_state_2)
    {
        case ped_state_2::ped2_entering_a_car_6:
        case ped_state_2::ped2_getting_out_a_car_7:
        case ped_state_2::carjack_pulling_out_9:
        case ped_state_2::ped2_driving_10:
        case ped_state_2::ped2_at_car_door_17:
        case ped_state_2::falling_19:
        case ped_state_2::knocked_recover_24:
        case ped_state_2::knocked_down_25:
        case ped_state_2::knocked_fatal_26:
        case ped_state_2::electrocuted_27:
            return false;

        case ped_state_2::carjack_opening_door_8:
            if (pSprite == NULL)
            {
                return true;
            }
            return false;

        case ped_state_2::lying_on_floor_22:
            break;

        default:
            pPed = this->field_7C_pPed;
            if (pPed->GetPedState_403990() == ped_state_1::dead_9 &&
                (!pPed->IsOnFire_475B50() || !pSprite || !pSprite->IsObjectModelEqual_59E930(objects::water_hitting_198)))
            {
                return false;
            }

            if (this->field_10_char_state == Char_B4_state::Jumping_15 && pSprite)
            {
                switch (pSprite->GetType_416B40())
                {
                    case sprite_types_enum::car_2:
                        return false;
                    case sprite_types_enum::code_obj1_4:
                        break;
                    default:
                        return false;
                }
            }
            break;
    }
    return true;
}

MATCH_FUNC(0x5535B0)
bool Char::PhoneTouched_5535B0(Object* pObject)
{
    Ped* pPed = field_7C_pPed;
    if (pPed->field_15C_player)
    {
        return gScriptManager_6F8060->AnswerPhone_5129F0(pPed->get_id(), pObject->field_14_id);
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x553640)
bool Char::OnObjectTouched_553640(Object* pObject)
{
    s8 conveyor_x;
    s8 conveyor_y;

    if (pObject->IsPickup_421060())
    {
        return get_ped_433A20()->HandlePickupCollision_45DE80(pObject);
    }

    switch (pObject->field_18_model)
    {
        case objects::secret_token_266: 
            return get_ped_433A20()->HandlePickupCollision_45DE80(pObject);

        case 257:
        case objects::ped_crossing_trigger_258:
            if (field_8_ped_state_1 != ped_state_1::dead_9 && field_8_ped_state_1 != ped_state_1::immobilized_8)
            {
                get_ped_433A20()->HandlePedCrossingTrigger_45CF20(pObject);
            }
            break;

        case objects::savepoint_161:
            gCarThreadTriggers_214_705F20->CheckThreadTrigger_5C8780(pObject->field_27_thread_trigger_idx, this->field_80_sprite_ptr);
            break;

        case objects::blue_phone_164:
        case objects::red_phone_177:
        case objects::yellow_phone_179:
        case objects::green_phone_181:
            return PhoneTouched_5535B0(pObject);

        case objects::door_trigger_167:
            gDoor_4D4_67BD2C->TryOpenDoorForPed_49D370(get_ped_433A20(), pObject->GetParam_421050());
            break;

        case objects::destructor_141:
            get_ped_433A20()->Kill_46F9D0();
            break;

        case objects::conveyor_139:
            pObject->GetConveyorDirection_493090(&conveyor_x, &conveyor_y);
            this->field_4C_conveyor_dx = kFpOne64th_6FDA9C * conveyor_x;
            this->field_50_conveyor_dy = kFpOne64th_6FDA9C * conveyor_y;
            break;

        default:
            break;
    }
    return false;
}

MATCH_FUNC(0x5537F0)
bool Char::HandlePedObjectHit_5537F0(Object* pObject)
{
    const u8 idx = pObject->get_field_26_420FF0();
    const s32 pedId = gPedRefTable_7F8_703398->GetPedId_420F10(idx);
    if (pObject->field_18_model == objects::rocket_bullet_128 || pObject->field_18_model == objects::moving_molotov_138 || pObject->field_18_model == objects::mine_10 && !gCharB4_HitByMine_6FDB59)
    {
        gObject_5C_6F8F84->CreateExplosion_52A3D0(field_80_sprite_ptr->field_14_xy.x,
                                                  field_80_sprite_ptr->field_14_xy.y,
                                                  field_80_sprite_ptr->field_1C_zpos,
                                                  kAng0_6FDB34,
                                                  explosion_type::small_18,
                                                  pedId);
        if (pObject->field_18_model == objects::mine_10)
        {
            gCharB4_HitByMine_6FDB59 = 1;
        }
    }

    if (pObject->get_field_26_420FF0() != field_7C_pPed->get_ped_ref_idx_420B50())
    {
        return field_7C_pPed->HandlePedHitByObject_45D000(pObject);
    }
    else
    {
        return false;
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

MATCH_FUNC(0x5538A0)
#define SPAWN_KNOCKDOWN_OBJ(speed)                                                      \
    gObject_5C_6F8F84->NewMovingObj_52A240(objects::ped_pushed_110,                     \
                                           this->field_80_sprite_ptr->field_14_xy.x,    \
                                           this->field_80_sprite_ptr->field_14_xy.y,    \
                                           this->field_80_sprite_ptr->field_1C_zpos,    \
                                           impact_angle,                                \
                                           this->field_80_sprite_ptr->field_0_rotation, \
                                           speed,                                       \
                                           -kFpPoint1_6FD824,                           \
                                           kFP16Zero_6FD9E4)
void Char::HandleCarImpact_5538A0(Car* pCar, u8 bUnknown, Fix16 x, Fix16 y)
{
    Fix16_Point p(x, y);
    Fix16 vecLen = GetLength_inline_5538A0(p);
    Ang16 impact_angle;
    impact_angle = p.atan2_40F790();

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

        if (!this->field_7C_pPed->field_267_ped_ref_idx)
        {
            this->field_7C_pPed->field_267_ped_ref_idx = gPedRefTable_7F8_703398->AllocForPed_59B060(this->field_7C_pPed->field_200_id);
        }

        this->field_6C_animation_state = Char_Anim_state::Lethal_Fall_12;
        this->field_68_animation_frame = 0;

        if (pCar->field_54_driver)
        {
            if (!pCar->field_54_driver->field_21C_bf.bDriveAggressively && pCar->field_54_driver->PedTypeIs_45EDE0(ped_type::dummy_3))
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
                this->field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(this->field_7C_pPed->field_267_ped_ref_idx);
                this->field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                this->field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_recover_24);
                this->field_C_ped_state_2 = ped_state_2::knocked_recover_24;
            }
        }
        else if (this->field_7C_pPed->PedTypeIs_45EDE0(ped_type::dummy_3))
        {
            this->field_7C_pPed->Kill_46F9D0();
            gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                      this->field_80_sprite_ptr->field_14_xy.y,
                                                      this->field_80_sprite_ptr->field_1C_zpos,
                                                      impact_angle);
        }
        else if (vecLen < kFpPoint3_6FD830)
        {
            if (vecLen < kFpPoint2_6FD82C)
            {
                if (this->field_C_ped_state_2 == ped_state_2::lying_on_floor_22)
                {
                    this->field_7C_pPed->Kill_46F9D0();
                }
                else
                {
                    this->field_7C_pPed->field_184_pObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two_6FD9EC);
                    this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = kFpPoint05_6FD9B0;
                    this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_2A_bAirborne = 1;
                    this->field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(this->field_7C_pPed->field_267_ped_ref_idx);
                    this->field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);

                    if (this->field_7C_pPed->field_208_invulnerability > 0)
                    {
                        this->field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_recover_24);
                    }
                    else if (pCar->field_7C_ped_type == ped_type::player_2)
                    {
                        this->field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_fatal_26);
                    }
                    else
                    {
                        this->field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_down_25);
                    }

                    this->field_C_ped_state_2 = ped_state_2::knocked_recover_24;

                    gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                              this->field_80_sprite_ptr->field_14_xy.y,
                                                              this->field_80_sprite_ptr->field_1C_zpos,
                                                              impact_angle);
                }
            }
            else
            {
                if (this->field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2) && pCar->field_7C_ped_type != ped_type::player_2)
                {
                    if (this->field_C_ped_state_2 == ped_state_2::lying_on_floor_22)
                    {
                        this->field_7C_pPed->Kill_46F9D0();
                        return;
                    }
                    this->field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                    this->field_7C_pPed->field_184_pObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two_6FD9EC);
                    this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = kFpPoint1_6FD824;
                    this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_2A_bAirborne = 1;
                    this->field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(this->field_7C_pPed->field_267_ped_ref_idx);
                    if (field_7C_pPed->field_208_invulnerability > 0)
                    {
                        field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_recover_24);
                    }
                    else
                    {
                        field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_down_25);
                    }

                    this->field_C_ped_state_2 = ped_state_2::knocked_recover_24;
                }
                else
                {
                    this->field_7C_pPed->Kill_46F9D0();
                }
                gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                          this->field_80_sprite_ptr->field_14_xy.y,
                                                          this->field_80_sprite_ptr->field_1C_zpos,
                                                          impact_angle);
            }
        }
        else
        {
            if (this->field_7C_pPed->field_208_invulnerability > 0)
            {
                this->field_7C_pPed->field_184_pObj2C = SPAWN_KNOCKDOWN_OBJ(vecLen / gFix16_Two_6FD9EC);
                this->field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = kFP16Zero_6FD9E4;
                this->field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(this->field_7C_pPed->field_267_ped_ref_idx);
                this->field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                this->field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_recover_24);
                this->field_C_ped_state_2 = ped_state_2::knocked_recover_24;
            }
            else
            {
                this->field_7C_pPed->Kill_46F9D0();
            }
            gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                      this->field_80_sprite_ptr->field_14_xy.y,
                                                      this->field_80_sprite_ptr->field_1C_zpos,
                                                      impact_angle);
        }
    }
}
#undef SPAWN_KNOCKDOWN_OBJ

MATCH_FUNC(0x553E00)
void Char::HandleGenericImpact_553E00(Ang16 ang, Fix16 impact_x, Fix16 impact_z, char_type impact_type)
{

    if (field_7C_pPed->field_208_invulnerability > 0)
    {
        impact_type = 0;
    }

    if (field_7C_pPed->GetPedState_403990() != ped_state_1::dead_9)
    {
        this->field_16_state_init_pending = true;
        this->field_7C_pPed->field_184_pObj2C = gObject_5C_6F8F84->NewMovingObj_52A240(objects::ped_pushed_110,
                                                                                       field_80_sprite_ptr->field_14_xy.x,
                                                                                       field_80_sprite_ptr->field_14_xy.y,
                                                                                       field_80_sprite_ptr->field_1C_zpos,
                                                                                       ang,
                                                                                       field_80_sprite_ptr->field_0_rotation,
                                                                                       impact_x,
                                                                                       -kFpPoint02_6FD9AC,
                                                                                       impact_z);
        if (!field_7C_pPed->field_267_ped_ref_idx)
        {
            field_7C_pPed->field_267_ped_ref_idx = gPedRefTable_7F8_703398->AllocForPed_59B060(field_7C_pPed->field_200_id);
        }
        field_7C_pPed->field_184_pObj2C->SetDamageOwner_529080(field_7C_pPed->field_267_ped_ref_idx);
        field_7C_pPed->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);

        switch (impact_type)
        {
            case 0:
                field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = kFP16Zero_6FD9E4;
                field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_recover_24);
                field_C_ped_state_2 = ped_state_2::knocked_recover_24;
                break;

            case 1:
                field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = impact_z;
                if (field_7C_pPed->field_208_invulnerability > 0)
                {

                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_recover_24);
                }
                else
                {
                    if (field_7C_pPed->PedTypeIs_45EDE0(ped_type::player_2))
                    {
                        field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_down_25);
                    }
                    else
                    {
                        field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_fatal_26);
                    }
                }

                this->field_C_ped_state_2 = ped_state_2::knocked_recover_24;
                gParticle_8_6FD5E8->EmitBloodBurst_53E450(this->field_80_sprite_ptr->field_14_xy.x,
                                                          this->field_80_sprite_ptr->field_14_xy.y,
                                                          this->field_80_sprite_ptr->field_1C_zpos,
                                                          ang);
                break;

            case 2:
                field_7C_pPed->field_184_pObj2C->field_10_obj_3c->field_10_z_speed = impact_z;
                if (field_7C_pPed->field_208_invulnerability > 0)
                {
                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_recover_24);
                }
                else
                {
                    field_7C_pPed->ChangeNextPedState2_45C540(ped_state_2::knocked_fatal_26);
                }

                this->field_C_ped_state_2 = ped_state_2::knocked_recover_24;
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

MATCH_FUNC(0x551CA0)
EXPORT void Char::nullsub_28()
{
    // State 6 deliberately does nothing in the original game.
}
