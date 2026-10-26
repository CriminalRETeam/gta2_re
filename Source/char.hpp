#pragma once

#include "char_b4_flags.hpp"
#include "BitSet32.hpp"
#include "Function.hpp"
#include "Object_3C.hpp"
#include "Pool.hpp"
#include "ang16.hpp"
#include "sprite.hpp"

class Sprite_3C;
class Ped;
class Car;
class Sprite;

EXTERN_GLOBAL(Fix16, kFpOne256th_678620);
EXTERN_GLOBAL(Fix16, gCharB4_Saved_Xpos_6FD7F8);
EXTERN_GLOBAL(Fix16, gCharB4_Saved_Ypos_6FD800);
EXTERN_GLOBAL(Fix16, gCharB4_Saved_Zpos_6FD7FC);
EXTERN_GLOBAL(Fix16, kFpOne128th_6784BC);

EXTERN_GLOBAL(Ang16, kAng16_6FD940);
EXTERN_GLOBAL(Ang16, kAng344_6FD8F8);

EXPORT void __stdcall UnpackSignedNibbles_529050(u8 packed, s8* pX, s8* pY);
EXPORT Ang16 __stdcall ComputeShortestAngleDelta_4056C0(Ang16& angle1, Ang16& angle2);

// SpriteList with a (trivial) destructor: the original Char ctor has an EH frame for this member,
// while other owners of a SpriteList (Car, Object_5C, WeaponManager) have none.
class SpriteList_4_dtor : public SpriteList
{
  public:
    ~SpriteList_4_dtor()
    {
    }
};

class Char
{
  public:
    // 9.6f 0x41B080
    inline Fix16 get_velocity_41B080() const
    {
        return field_38_velocity;
    }

    // 9.6f 0x433930
    inline void UseRunOrJumpSpeed_433930()
    {
        field_38_velocity = field_3C_run_or_jump_speed;
    }

    // 9.6f 0x4338F0
    inline void SetSpriteNum_4338F0(s32 num)
    {
        field_80_sprite_ptr->SetDrawLayer_40F7B0(num);
    }

    // 9.6f 0x41B090
    inline s32 get_ped_state_2_41B090() const
    {
        return field_C_ped_state_2;
    }

    // 9.6f 0x4338D0
    inline Sprite* get_sprite_ptr_4338D0() const
    {
        return field_80_sprite_ptr;
    }

    // 9.6f 0x4338E0
    inline void set_pPed_4338E0(Ped* v)
    {
        field_7C_pPed = v;
    }

    s32 field_0_id;
    s8 field_4_unused;
    u8 field_5_remap;
    s8 field_6_unused;
    s8 field_7_unused;
    s32 field_8_ped_state_1;
    s32 field_C_ped_state_2;
    s32 field_10_char_state;
    Ang16 field_14_target_rotation;
    bool field_16_state_init_pending;
    s8 field_17_unused;
    void* field_18_collided_entity;
    void* field_1C_prev_collided_entity;
    s32 field_20_collided_entity_type;
    s32 field_24;
    Ang16 field_28_slide_rotation;
    Ang16 field_2A_slide_turn;
    Ang16 field_2C_ang;
    s8 field_2E_unused;
    s8 field_2F_unused;
    s32 field_30_unused;
    s16 field_34_unused;
    s8 field_36_unused;
    s8 field_37_unused;
    Fix16 field_38_velocity;
    Fix16 field_3C_run_or_jump_speed;
    Ang16 field_40_rotation;
    Ang16 field_42_rotation_jitter;
    s8 field_44_block_type;
    u8 field_45_slope_gradient_direction;
    u16 field_46_timer;
    s8 field_48_lying_on_floor_timer;
    s8 field_49_unused;
    u16 field_4A;
    Fix16 field_4C_conveyor_dx;
    Fix16 field_50_conveyor_dy;
    s8 field_54_jump_scale_counter;
    u8 field_55_next_tile_timer;
    s8 field_56_unused;
    s8 field_57_unused;
    union
    {
        u32 field_58_flags;
        CharB4Flags32 field_58_flags_bf;
    };
    u8 field_5C_jump_over_timer;
    s8 field_5D_unused;
    s8 field_5E_unused;
    s8 field_5F_unused;
    s32 field_60_unused;
    s32 field_64_unused;
    u8 field_68_animation_frame;
    s8 field_69_is_colliding_with_sprite;
    u8 field_6A_repulsion_timer;
    s8 field_6b_unused;
    s32 field_6C_animation_state;
    s8 field_70_frame_timer;
    s8 field_71_frame_delay;
    u8 field_72_next_tile_x;
    u8 field_73_next_tile_y;
    Ang16 field_74_repulsion_rotation;
    s8 field_76_unused;
    s8 field_77_unused;
    Char* mpNext;
    Ped* field_7C_pPed;
    Sprite* field_80_sprite_ptr; // TODO: Or sprite_3c, are they the same type ??
    Car* field_84_target_car;
    SpriteList_4_dtor field_88_obj_2c;
    Fix16 field_8C_jump_base_z;
    Fix16 field_90_fall_speed;
    Fix16 field_94_fall_z_speed;
    Fix16_Point_POD field_98_velocity_vector;
    bool field_A0_force_top_layer;
    s8 field_A1_unused;
    s8 field_A2_unused;
    s8 field_A3_unused;
    Fix16 field_A4_xpos;
    Fix16 field_A8_ypos;
    Fix16 field_AC_zpos;
    s32 field_B0_scream_timer;

    // 9.6f 0x48A4C0
    inline s32 get_ped_state_1_48A4C0() const
    {
        return field_8_ped_state_1;
    }

    // 9.6f 0x492180
    inline void Set_F8_ped_state_1_433910(s32 ped_state)
    {
        field_8_ped_state_1 = ped_state;
    }

    inline Ang16 get_rotation_433A40() const
    {
        return field_40_rotation;
    }

    void set_rotation_433A30(Ang16 rotation)
    {
        field_40_rotation = rotation;
    }

    // 9.6f 0x4339C0
    inline Fix16 get_sprite_xpos() const
    {
        return field_80_sprite_ptr->field_14_xy.x;
    }

    // 9.6f 0x4339E0
    inline Fix16 get_sprite_ypos() const
    {
        return field_80_sprite_ptr->field_14_xy.y;
    }

    // 9.6f 0x433A00
    inline Fix16 get_sprite_zpos() const
    {
        return field_80_sprite_ptr->field_1C_zpos;
    }

    inline void RegulateVelocity_433970(Fix16 threshold)
    {
        if (field_38_velocity < threshold)
        {
            field_38_velocity += kFpOne256th_678620;
        }
        else if (field_38_velocity > threshold)
        {
            field_38_velocity -= kFpOne256th_678620;
        }
    }

    // strange, the same 9.6f func but Ped::ChaseTargetStateMachine_46B170 only matches if it pass by ref
    inline void RegulateVelocityByRef_433970(Fix16& threshold)
    {
        if (field_38_velocity < threshold)
        {
            field_38_velocity += kFpOne256th_678620;
        }
        else if (field_38_velocity > threshold)
        {
            field_38_velocity -= kFpOne256th_678620;
        }
    }

    inline void SetVelocity_433920(Fix16 max_speed)
    {
        field_38_velocity = max_speed;
    }

    // strange, the same 9.6f func but Ped::ChaseTargetStateMachine_46B170 only matches if it pass by ref
    inline void SetVelocityByRef_433920(Fix16& max_speed)
    {
        field_38_velocity = max_speed;
    }

    inline Ped* get_ped_433A20() const
    {
        return field_7C_pPed;
    }

    inline void SetPedState1_433910(s32 new_state)
    {
        field_8_ped_state_1 = new_state;
    }

    inline void SetPedState2_433A50(s32 new_state)
    {
        field_C_ped_state_2 = new_state;
    }

    inline void sub_4923D0()
    {
        field_40_rotation.SnapToAng4_405640();
        field_40_rotation += kAng16_6FD940;
        field_10_char_state = 9;
        field_46_timer = 10;
    }

    inline void sub_4923A0()
    {
        field_40_rotation.SnapToAng4_405640();
        field_40_rotation += kAng344_6FD8F8;
        field_10_char_state = 8;
        field_46_timer = 10;
    }

    inline s32 GetCharState_433A80() const
    {
        return field_10_char_state;
    }

    inline void SetCharState_433A60(s32 char_state)
    {
        field_10_char_state = char_state;
    }

    // 9.6f 0x433A90
    inline s8 Get_F44_433A90() const
    {
        return field_44_block_type;
    }

    // 9.6f 0x403900
    inline Car* Get_F84_403900() const
    {
        return field_84_target_car;
    }

    inline void Set_F84_433900(Car* pCar)
    {
        field_84_target_car = pCar;
    }

    inline void IncreaseSpeedIfAllowed_433940()
    {
        field_38_velocity += kFpOne128th_6784BC;
        
        if (field_38_velocity > field_3C_run_or_jump_speed)
        {
            field_38_velocity = field_3C_run_or_jump_speed;
        }
    }

    inline void AddAngle_4928A0(Ang16 ang)
    {
        field_40_rotation += ang;
    }

    inline bool IsCollidingWithASprite_433AA0() const
    {
        if (field_69_is_colliding_with_sprite)
        {
            return true;
        }
        return false;
    }

    Char();
    ~Char();

    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();

    EXPORT void DrawFlamesAndStartScreamTimer_545430();
    EXPORT bool HasShadows_5451C0() const;

    EXPORT Fix16_Point GetRunVelocity_545580() const;
    EXPORT void SetRemap_46DD50(u8 remap);

    // Inlined copy of SetRemap_46DD50
    inline void SetRemap_Inline(u8 remap)
    {
        this->field_5_remap = remap;
        if (remap != 0xFF)
        {
            field_80_sprite_ptr->SetRemap(remap);
        }
    }

    EXPORT void RemoveFireSprites_5454B0();
    EXPORT void DoJump_5454D0();
    EXPORT void Teleport_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT s32 IsOnWater_545570() const;
    EXPORT void KillPed_5455F0();
    EXPORT void ClearCollisionState_545600();
    EXPORT void GetTileFracX64_545640(Fix16 coord, s16* output) const;
    EXPORT void GetTileFracY64_545670(Fix16 coord, s16* output) const;
    EXPORT void InitSprite_5456A0();
    EXPORT bool IsOnScreen_545700() const;
    EXPORT void Update_545720(Fix16 distance_to_target);
    EXPORT void CheckAndHandleCollisions_5459C0();
    EXPORT void DrownPed_5459E0();
    EXPORT void UpdateAnimState_546360();
    EXPORT void ManageZCoordAndSlopes_548590();
    EXPORT void DispatchCollision_548670(char_type bFlag);
    EXPORT void HandleObjectCollision_548840(Object* pObj);
    EXPORT void HandlePedCollision_548BD0(Char* pOther);
    EXPORT void HandleGenericCollision_54A530(Car* pCar, Object* pObj, Char* pChar);
    EXPORT bool ContinueMovementAfterCollision_54B8F0();
    EXPORT void SnapRotationToFace_54C090();
    EXPORT bool CanMoveOntoSlope_54C1A0(s32 path_direction);
    inline void SetTurnTarget_492400(Ang16 target_rotation);
    EXPORT void TurnToFreeAdjacentFace_54C3E0();
    EXPORT bool CanMoveToTile_54C500(char_type x, char_type y);
    EXPORT void SelectRandomIdleBehavior_54C580();
    EXPORT void ApplyRandomRotationJitter_54C6C0();
    EXPORT void TickMovementStateMachine_54C900();
    EXPORT void TurnTowardsAngle_54CAE0();
    EXPORT void ApplyMovement_54CC40();
    EXPORT void CheckCollisionsAndUpdateAnim_54DD70();
    EXPORT void state_0_54DDF0();
    EXPORT bool CanStepForwardWithRegionCheck_54ECB0(s32 direction);
    EXPORT bool CanStepToTile_54EF60(char_type target_x, char_type target_y);
    EXPORT bool CanStepForward_54FEC0(s32 direction);
    EXPORT bool CanReachTile_550090(u8 xpos, u8 ypos);
    EXPORT void state_1_5504F0();
    EXPORT Ang16 GetNextRotationToward_550F60(Ang16 inputAng);
    EXPORT bool CanStepInDirection_551350(Ang16 angle);
    EXPORT void ChooseNextMovementTile_551400();
    EXPORT void SelectNextTileFast_5516F0();
    EXPORT void state_2_5519F0();
    EXPORT void state_3_551A00();
    EXPORT void state_4_551B30();
    EXPORT void state_5_551BB0();
    EXPORT void state_7_551CB0();
    EXPORT void state_8_5520A0();
    EXPORT void state_9_552E90();

    EXPORT bool IsNearTileCentre_5532C0() const;
    EXPORT bool IsThreatToSearchingPed_553330() const;
    EXPORT bool ShouldCollideWithSprite_553340(Sprite* pSprite) const;
    EXPORT bool PhoneTouched_5535B0(Object* pObject);
    EXPORT bool OnObjectTouched_553640(Object* pObject);
    EXPORT bool HandlePedObjectHit_5537F0(Object* pObject);
    EXPORT void HandleCarImpact_5538A0(Car* pCar, u8 bUnknown, Fix16 x, Fix16 y);
    EXPORT void HandleGenericImpact_553E00(Ang16 ang, Fix16 impact_x, Fix16 impact_z, char_type impact_type);

    EXPORT void nullsub_28();
};
GTA2_ASSERT_SIZEOF_ALWAYS(Char, 0xB4);

EXPORT void __stdcall ResetCharUpdateGlobals_544F70();
EXPORT void __stdcall ResetCharStatics_553F90();

EXTERN_GLOBAL(u8, bThreateningPedAdded_6787EF);

EXTERN_GLOBAL(u16, gNumPedsOnScreen_6787EC);
