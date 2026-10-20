#pragma once

#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "Object_3C.hpp"
#include "Object_8.hpp"
#include "ObjectDefinitions_8CA8.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "sprite.hpp"

EXTERN_GLOBAL(Fix16_Point, gZeroVector_6F8EF0);
EXTERN_GLOBAL(s32, gObj2C_id_623EC0);

EXPORT void __stdcall UnpackSignedNibbles_529050(u8 a1, s8* a2, s8* a3);

class Object_2C;
class Sprite;
class SpriteShape_4C;
class Car_BC;
class ObjectDefinition_74;
class Object_3C;
class Object_2C_Pool;
class Explosion_30;
class Object_8;
class Light_28;

namespace object_pool_state
{
enum
{
    none_0 = 0,
    in_pool_list_1 = 1, // linked into the pool's list of live objects (PoolGive_522340)
    out_of_pool_list_2 = 2, // allocated but not linked (PoolTake_522360, AllocateUntracked)
};
} // namespace object_pool_state

namespace object_removal_state
{
enum
{
    none_0 = 0,
    requested_1 = 1, // RequestRemoval_5290A0
    when_off_screen_2 = 2, // RequestRemovalWhenOffScreen_5290B0
};
} // namespace object_removal_state

class Object_2C
{
  public:
    // 9.6f 0x4C4F10
    inline void SetThreadTriggerIdx_4C4F10(char_type v)
    {
        field_27_thread_trigger_idx = v;
    }

    EXPORT Object_2C();
    EXPORT void PoolDeallocate();
    EXPORT bool CanCollideWithSpriteByOwnerRef_522250(Sprite* pSprite);
    EXPORT s32 GetMovementRegionSize_5222B0();
    EXPORT void UpdatePhysics_5222D0();
    EXPORT void PoolGive_522340();
    EXPORT void PoolTake_522360();
    EXPORT bool ShouldCollideWith_5223C0(Sprite* pSprite);
    EXPORT bool ShouldStopAtTrafficLight_525290(Sprite* pSprite);
    EXPORT bool ShouldCollideWithSprite_522430(Sprite* pSprite);
    EXPORT bool SelectCollisionSprite_522460(Sprite* pProbe);
    EXPORT void SetMovementVector_5224E0(Fix16_Point& speed);
    EXPORT void SetMovementVectorWithRandomState_522640(Fix16_Point& point);
    EXPORT void SetConveyorPush_5226A0(char_type conveyor_speed);
    EXPORT void ResolveCollisionWithObject_522710(Object_2C* pObj, Fix16_Point* pPoint);
    EXPORT void ResolveCollisionWithPed_5229B0(Char_B4* pB4, Fix16_Point* pPoint, u8 not_used);
    EXPORT void ResolveCollisionWithWorld_522B20(Fix16_Point* f18, Fix16_Point* pPoint, Fix16_Point* speed);
    EXPORT void ResolveCollisionWithMapTile_522BE0(Fix16_Point* pPoint);
    EXPORT void ResolveCollisionWithMapTileHorizontal_522D00(Fix16_Point* pPoint);
    EXPORT void HandleCollision_522E10(Fix16_Point* pPoint);
    EXPORT bool TryStartSlide_5233A0(Fix16 value);
    EXPORT void HandleCollisionOutcome_523440(Fix16_Point point, char_type bUnknown1, char_type bUnknown2);
    EXPORT bool HandleSpriteGroundAndCollision_5235B0(Sprite* pSprite, Fix16_Point* pPoint, u8* pHit, Fix16 found_z);
    EXPORT bool HandleSpriteGroundAndCollisionSimple_523770(Sprite* pSprite, Fix16_Point* pPoint, u8* pHit, u8* pHit2);
    EXPORT bool HandleSpriteZCollision_5238B0(Sprite* pSprite, Fix16_Point* pPoint, u8* pHit, u8* pHit2, Fix16 found_z);
    EXPORT void ComputeWallHitSide_524550();
    EXPORT void IntegrateHorizontalMovementAndCollisions_524630(Fix16 move_len, Ang16 move_angle);
    EXPORT void OnExplosionNearby_525190(u8 ped_ref_idx);
    EXPORT void UpdateAnimation_5257D0();
    EXPORT bool DispatchFrameAction_525910();
    EXPORT char ShouldCollideWithSprite_525370(Sprite* pSprite);
    EXPORT void CheckCollisionForModel_139_And_141_525AE0();
    EXPORT void SpawnSpriteParticlesForRocketBullet_525B40();
    EXPORT bool CheckWaterDeath_525B60();
    EXPORT void UpdatePhysicsAndMovement_525B80();
    EXPORT void UpdatePhysicsMovementAndAnimation_525D90();
    EXPORT void Update_525F30();
    EXPORT bool PoolUpdate();
    EXPORT void TriggerCarExplosionIfApplicable_526790(Sprite* pSprite);
    EXPORT static s32 __stdcall GetExplosionTypeForItemDefinition_526830(s32 type);
    EXPORT void InheritMotionFromSprite_526B40(Sprite* pSprite);
    EXPORT bool UpdateMovementAndEffects_527070(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot);
    EXPORT void InitializeObject_527630(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT void TurnOffLight_527990();
    EXPORT void AssignToBucket_527AE0();
    EXPORT void RemoveFromCollisionBuckets_527D00();
    EXPORT void ReleaseSubObjects_527F10();
    EXPORT void NewObj3C_528130(Fix16_Point& speed);
    EXPORT bool HandleRotationStateTransition_528240(s32 current, s32 desired);
    EXPORT void TickObject_5283C0(s32 obj_type);
    EXPORT char_type OnObjectTouched_5288B0(Sprite* pSprite);
    EXPORT void HandleWaterDeath_528900();
    EXPORT static s32 __stdcall GetExplosionTypeForWallSide_528E00(s32 wall_side); // TODO: Check if thiscall
    EXPORT bool HandleObjectHitIfExplosive_528960(Object_2C* pOther);
    EXPORT char_type HandleObjectHit_528990(Sprite* pSprite);
    EXPORT void ProcessObjectExplosionImpact_528A20(Object_2C* pObj);
    EXPORT void HandleImpactNoSprite_528BA0();
    EXPORT void HandleImpact_528E50(Sprite* pSprite);
    EXPORT void HandleCollisionWithObject_529000(Object_2C* pObj);
    EXPORT void SetConveyorSpeed_529030(s8 speed_x, s8 speed_y);
    EXPORT void ApplyConveyorSpeedFrom_529070(Object_2C* pObj);
    EXPORT s32 GetExplosionSideFromDiagonalWall_529210();
    EXPORT s32 GetMapBlockSoundSpec_529240();
    EXPORT void SetDefaultWeaponAmmo_5292D0();
    EXPORT void SetDamageOwner_529080(u8 ped_ref_idx);
    EXPORT void RequestRemoval_5290A0();
    EXPORT void RequestRemovalWhenOffScreen_5290B0();
    EXPORT void Dealloc_5291B0();
    EXPORT void PoolGiveAndMarkDone_5291D0();
    EXPORT void PoolGiveAndSetDone_5291E0(s32 bDone);
    EXPORT bool IsNotModel_174_529200();
    EXPORT ~Object_2C();
    EXPORT void EnsureObject3C_52A650();
    EXPORT void ReactivateObjectAfterImpact_52A6D0(Sprite* pSprite);
    EXPORT Fix16_Point GetXY_52AE70();
    EXPORT Fix16_Point GetSpeedVector_52AE90();
    // 9.6f 0x482C50; GetSpeedVector_52AE90 is its out-of-line copy. It returns a Fix16_Point (which has
    // a destructor), so VC6 never expands it: it is charged to the caller's inline budget and called
    // out of line (SetMovementVector_5224E0)
    inline Fix16_Point GetSpeedVector_482C50()
    {
        if (field_10_obj_3c)
        {
            return field_10_obj_3c->GetSpeedVector_482BA0();
        }
        return gZeroVector_6F8EF0;
    }

    // TODO: ordering
    EXPORT void SetSpriteIdxOffset_5290C0(u8 offset);
    EXPORT Fix16 GetEffectiveSpeed_5290F0();
    EXPORT bool IsFireExplosion_525AC0();
    EXPORT void UpdateEffectPool_525B20();
    EXPORT void UpdateLight_527A30();
    EXPORT void OnAttachedSpriteDestroyed_525100();

    EXPORT void IntegrateMovementAndCollisions_523BF0(Fix16 mov_speed, Ang16 angle);
    EXPORT void Sprite_UpdateZFromSlopeAndTile_522FA0(Sprite* pSprite);

    // 9.6f 0x475A70
    inline bool IsStaticCollisionCheck_475A70()
    {
        return field_8_definition->field_34_behavior_type == object_behavior_type::static_collision_check_1;
    }

    // 9.6f 0x475A60
    inline bool IsShop_475A60()
    {
        s32 v1 = field_8_definition->field_34_behavior_type;
        return v1 == object_behavior_type::shop_10;
    }

    bool IsAnimatedBehavior_475A80()
    {
        s32 t = this->field_8_definition->field_34_behavior_type;
        return t == object_behavior_type::animated_2 || t == object_behavior_type::physics_animated_4 || t == object_behavior_type::pickup_animated_8 || t == object_behavior_type::pickup_physics_animated_9;
    }

    inline bool IsDestroyedPowergen_434140()
    {
        if (field_18_model == objects::destroyed_invisible_powergen_285 || field_18_model == objects::destroyed_powergen_282)
        {
            return true;
        }
        return false;
    }

    bool CanBePushed_482C90() // matched
    {
        s32 f34 = field_8_definition->field_34_behavior_type;
        if (f34 == object_behavior_type::physics_3 || f34 == object_behavior_type::physics_animated_4 ||
            ((f34 == object_behavior_type::static_object_0 || f34 == object_behavior_type::static_collision_check_1 || f34 == object_behavior_type::animated_2) && field_8_definition->field_44_touch_behavior == 2))
        {
            return true;
        }
        return false;
    }

    bool UsesParamAsPedRef_421080()
    {
        s32 type = this->field_8_definition->field_34_behavior_type;
        return type != object_behavior_type::pickup_6 && type != object_behavior_type::pickup_physics_7 &&
            type != object_behavior_type::pickup_animated_8 && type != object_behavior_type::pickup_physics_animated_9 &&
            type != object_behavior_type::shop_10 && type != object_behavior_type::static_collision_check_1 &&
            type != object_behavior_type::effect_only_12;
    }

    // Sucky names but better than just "sub" for now
    bool IsOwnedByPedRef_4973E0(u8 a2)
    {
        return UsesParamAsPedRef_421080() && field_26_ped_ref_idx == a2;
    }

    bool IsSpriteGrid2Region3_4210B0()
    {
        return field_8_definition->field_40_collision_bucket_category == collision_bucket_category::sprite_grid_2_region_3;
    }

    // Inlined on version 9.6f 0x447e90 (also 9.6f copies 0x45E0A0 and 0x482C00)
    inline void set_field_26(u8 v)
    {
        field_26_ped_ref_idx = v;
    }

    // Inlined on version 9.6f 0x420FF0
    inline u8 get_field_26_420FF0()
    {
        return field_26_ped_ref_idx;
    }

    // 9.6f 0x434130
    inline void SetO8Timer_434130(s16 timer)
    {
        field_C_pAny.o8->field_4_timer = timer;
    }

    // 9.6f 0x40FEF0
    s32 GetModel_40FEF0()
    {
        return field_18_model;
    }

    // 9.6f 0x40FF00
    inline char_type GetDefHasShadowsField_40FF00()
    {
        return field_8_definition->field_63;
    }

    // 9.6f 0x475AA0
    inline bool IsModel176To181_475AA0()
    {
        s32 model = field_18_model;
        return model == 176 || model == 177 || model == 178 || model == 179 || model == 180 || model == 181;
    }

    bool IsPickup_421060()
    {
        return field_8_definition->field_34_behavior_type == object_behavior_type::pickup_6 ||
            field_8_definition->field_34_behavior_type == object_behavior_type::pickup_physics_7 ||
            field_8_definition->field_34_behavior_type == object_behavior_type::pickup_animated_8 ||
            field_8_definition->field_34_behavior_type == object_behavior_type::pickup_physics_animated_9;
    }

    // 9.6f 0x482490
    inline void PoolAllocate()
    {
        field_14_id = gObj2C_id_623EC0++;
        field_25_removal_state = object_removal_state::none_0;
        field_26_ped_ref_idx = 99;
        field_10_obj_3c = 0;
        field_C_pAny.o8 = 0;
        field_1C_has_explosion = 0;
        field_4_sprite = 0;
        field_18_model = 0;
    }

    inline Fix16 GetX_4340D0()
    {
        return field_4_sprite->field_14_xy.x;
    }

    inline Fix16 GetY_4340E0()
    {
        return field_4_sprite->field_14_xy.y;
    }

    inline Fix16 GetZ_4340F0()
    {
        return field_4_sprite->field_1C_zpos;
    }

    // 9.6f 0x4BE850
    bool IsGangIcon_4BE850()
    {
        return field_18_model >= 287 && field_18_model <= 293; // loonies_icon_287 .. russian_mafia_icon_293
    }

    bool IsFlameObject_4BE830()
    {
        if (this->field_18_model == 197)
        {
            return 1;
        }

        if (IsFireExplosion_525AC0())
        {
            return 1;
        }

        return 0;
    }

    // 9.6f 0x482400
    static inline bool IsExplosiveItemDefinition_482400(s32 idx)
    {
        return idx >= 39 && idx <= 42;
    }

    // 9.6f 0x482C80
    inline Fix16 GetMass_482C80()
    {
        return field_8_definition->field_18_mass;
    }

    void ApplyDefinitionDrawLayer_482BE0()
    {
        field_4_sprite->SetDrawLayer_40F7B0(field_8_definition->field_2C_draw_layer);
    }

    // 9.6f 0x482C10, defined in Object_5C.cpp (needs Object_8)
    inline bool IsAnimFinished_482C10();

    // 9.6f 0x493090
    inline void GetConveyorDirection_493090(s8* pX, s8* pY)
    {
        UnpackSignedNibbles_529050(get_field_26_420FF0(), pX, pY);
    }

    // 9.6f 0x482BF0
    void SetTopDrawLayer_482BF0()
    {
        field_4_sprite->SetDrawLayer_40F7B0(29);
    }

    Object_2C* mpNext;
    Sprite* field_4_sprite;
    ObjectDefinition_74* field_8_definition;
    union TAny
    {
        Explosion_30* pExplosion;
        Object_8* o8;
        Light_28* pLight;
    };
    TAny field_C_pAny;
    Object_3C* field_10_obj_3c;
    s32 field_14_id;
    s32 field_18_model;
    char_type field_1C_has_explosion;
    char_type field_1D_unused;
    char_type field_1E_unused;
    char_type field_1F_unused;
    s32 field_20_pool_list_state;
    u8 field_24_touched_result;
    u8 field_25_removal_state;
    u8 field_26_ped_ref_idx; // Seems to be a generic index. Sometimes it's the current idx of its object type. For many objects, it's unused (equal to 99).
    char_type field_27_thread_trigger_idx;
    char_type field_28;
    char_type field_29_unused;
    char_type field_2A;
    char_type field_2B_unused;
};

struct SavedObjectData_164;

class Object_5C
{
  public:
    EXPORT void TrimSpriteList_529300();
    EXPORT void ObjectsService_5293A0();
    EXPORT Object_5C();
    EXPORT ~Object_5C();
    EXPORT void CreateDiagonalWallObjects_5297F0();
    EXPORT Object_2C* GetDirectionalObject_5298E0(s32 slope_type);
    EXPORT Object_2C* NewTouchPoint_529950(s32 object_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16 w, Fix16 h, Fix16 depth);
    EXPORT Object_2C* NewPhysicsObj_5299B0(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT Object_2C* NewWithPedRefIdx_5299F0(s32 object_type, u32 ped_ref_idx, Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT Object_2C* NewLight_529A40(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity);
    EXPORT Object_2C* NewLight_529AB0(s32 light_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 argb, Fix16 radius, u8 intensity);
    EXPORT Object_2C* NewFlashingLight_529B20(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity, u8 on_time, u8 off_time, u8 shape);
    EXPORT Object_2C* NewPhysicsObjUnlessBlocked_529BC0(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation);
    EXPORT Object_2C* CreateObject_529C00(int object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, char fail_if_blocked);
    EXPORT char_type SetPendingDamageOwner_52A210(char_type owner);
    EXPORT Object_2C* NewMovingObj_52A240(s32 object_type,
                                        Fix16 xpos,
                                        Fix16 ypos,
                                        Fix16 zpos,
                                        Ang16 move_angle,
                                        Ang16 rotation,
                                        Fix16 speed,
                                        Fix16 friction,
                                        Fix16 z_speed);
    EXPORT Object_2C* NewMovingObjUnlessBlocked_52A280(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 move_angle,
                                 Ang16 rotation,
                                 Fix16 speed,
                                 Fix16 friction,
                                 Fix16 z_speed);
    EXPORT Object_2C* CreateMovingObj_52A2C0(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 move_angle,
                                 Ang16 rotation,
                                 Fix16 speed,
                                 Fix16 friction,
                                 Fix16 z_speed,
                                 char_type fail_if_blocked);
    EXPORT Object_2C* CreateExplosion_52A3D0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 kind, s32 ped_id);

    EXPORT void SaveObjects_52A500(SavedObjectData_164* pUnknownObj);
    EXPORT void RestoreObjects_52A590(SavedObjectData_164* pUnknownObj);
    EXPORT void RemoveAndFree_52A610(Object_2C* p2C);

    Object_2C* field_0_diagonal_wall_ang315;
    Object_2C* field_4_diagonal_wall_ang135;
    Object_2C* field_8_diagonal_wall_ang225;
    Object_2C* field_C_diagonal_wall_ang45;
    s32 field_10_rotating_object_count;
    s32 field_14_listed_sprite_count;
    u8 field_18_pending_damage_owner;
    u8 field_19_unused;
    u16 field_1A_unused;
    SpriteList_4 field_1C_sprite_list;
    u8 field_20_uncollected_tokens[50];
    u16 field_52_unused; // pad?
    s32 field_54_uncollected_token_index;
    Sprite* field_58_collision_probe_sprite;
};

EXTERN_GLOBAL(Object_5C*, gObject_5C_6F8F84);
