#pragma once

#include "Fix16_Rect.hpp"
#include "Function.hpp"
#include "Pool.hpp"
#include "ang16.hpp"
#include "enums.hpp"
#include "fix16.hpp"
#include "gbh_graphics.hpp"

EXTERN_GLOBAL(Ang16, gAng16_703804);
EXTERN_GLOBAL(Fix16, gFix16_7035C0);

EXTERN_GLOBAL(u32, gLightingDrawFlag_7068F4);
EXTERN_GLOBAL_ARRAY(Vert, gTileVerts_7036D0, 8);

class Car_BC;
class Char_B4;
class Object_2C;
class infallible_turing;
class Ped;

class Sprite_4C
{
  public:
    EXPORT s32 PoolAllocate();

    void PoolDeallocate()
    {
    }

    // 9.6f 0x41E390
    // https://decomp.me/scratch/YTMyx
    bool IsZeroWidth_41E390() const
    {
        return (field_0_width == field_4_height && field_0_width <= kSmallWidthEpslion_703450) ? true : false;
    }

    EXPORT void SetCurrentRect_5A4D90();
    EXPORT void UpdateRotatedBoundingBox_5A3550(Fix16 x, Fix16 y, Fix16 z, Ang16 ang);

    EXPORT Sprite_4C();
    EXPORT ~Sprite_4C();

    Fix16 field_0_width;
    Fix16 field_4_height;
    Fix16 field_8;
    Fix16_Point field_C_renderingRect[4];
    Sprite_4C* mpNext;
    Fix16_Rect field_30_boundingBox;
    bool field_48_bBoxUpToDate;
    char_type field_49;
    char_type field_4A;
    char_type field_4B;
};

class Sprite
{
  public:
    // TODO: Ordering
    EXPORT Fix16_Point GetBoundingBoxCorner_562450(s32 idx);

    EXPORT Fix16_Point get_x_y_443580();
    EXPORT void sub_451950(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void sub_54EC80(Fix16 xpos, Fix16 ypos);
    EXPORT bool IsControlledByActivePlayer_59E170();
    EXPORT Ped* GetPed_59E1B0();
    EXPORT s32 IsOnWater_59E1D0();
    EXPORT char_type sub_59E250();
    EXPORT void sub_59E2E0();
    EXPORT void sub_59E300();
    EXPORT void sub_59E320(char_type a2);
    EXPORT bool sub_59E390(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_59E4C0(s32 a2, s32 a3);
    EXPORT char_type CollisionCheck_59E590(Sprite* a2);
    EXPORT char_type sub_59E680(s32 a2, s16* a3);
    EXPORT void ResetZCollisionAndDebugBoxes_59E7B0();
    EXPORT Sprite* sub_59E7D0(s32 a2);
    EXPORT char_type IsThreatToSearchingPed_59E830();
    EXPORT char_type sub_59E850(Sprite* pSprite);
    EXPORT void HandleObjectCollision_59E8C0(Sprite* pSprite);
    EXPORT void ProcessCarToCarImpactIfCar_59E910(Sprite* a2);
    EXPORT void sub_59E960();
    EXPORT void UpdateCollisionBoundsIfNeeded_59E9C0();
    EXPORT void SetRemap(s16 remap);
    EXPORT s16 sub_59EAA0();
    EXPORT char_type has_shadows_59EAE0();
    EXPORT void sub_59EB30(f32& a2, f32& a3);
    EXPORT void ShowHorn_59EE40(f32 a2, f32 a3);
    EXPORT void Draw_59EFF0();
    EXPORT void AllocInternal_59F950(Fix16 width, Fix16 height, Fix16 a4);
    EXPORT void Update_4C_59F990();
    EXPORT void sub_59FA40();
    EXPORT void FreeSprite4CChildren_59FAD0();
    EXPORT bool sub_59FB10(s32* a2);
    EXPORT char_type FindOverlappingBoundingBoxCorners_5A0150(s32 a2, u8* a3, u8* a4);
    EXPORT char_type CollisionCheck_5A0320(Fix16* pXY1, Fix16* pXY2, u8* pCollisionIdx1, u8* pCollisionIdx2);
    EXPORT bool RotatedRectCollisionSAT_5A0380(Sprite* a2);
    EXPORT char_type sub_5A0970(s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_5A0A70(Sprite_4C* a2, Sprite** a3, u8* a4);
    EXPORT char_type sub_5A0EF0(s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_5A1030(Sprite* a2, Sprite** a3, u8* a4);
    EXPORT bool sub_5A1490(s32 a2, s32 a3);
    EXPORT char_type sub_5A19C0();
    EXPORT char sub_5A1A60();
    EXPORT void ResolveZOrder_5A1B30(Sprite* pOther);
    EXPORT char_type ComputeZLayer_5A1BD0();
    EXPORT char_type CheckCornerZCollisions_5A1CA0(u32* a2);
    EXPORT char_type IsTouchingSlopeBlock_5A1EB0();
    EXPORT char_type sub_5A21F0();
    EXPORT u32* sub_5A22B0(u32* a2, Sprite* a3);
    EXPORT char_type sub_5A2440();
    EXPORT char_type sub_5A2500();
    EXPORT s16* sub_5A26E0(s16* a2);
    EXPORT s32* sub_5A2710(s32* a2, Sprite* a3, s32* a4, s32 a5, u8* a6, u8* a7, char_type* a8);
    EXPORT void CreateSoundObj_5A29D0();
    EXPORT bool IsObjectModelEqual_59E930(s32 model);
    EXPORT void FreeSound_5A2A00();
    EXPORT void sub_5A2A30();
    EXPORT void PoolAllocate();
    EXPORT void PoolDeallocate();
    EXPORT void DispatchCollisionEvent_5A3100(Sprite* a2, Fix16 a3, Fix16 a4, Ang16 a5);

    EXPORT void set_angle_4833B0(Ang16 ang);

    EXPORT ~Sprite(); // 0x5a3540

    EXPORT Sprite();

    Car_BC* AsCar_40FEB0()
    {
        if (field_30_sprite_type_enum == sprite_types_enum::car)
        {
            return field_8_car_bc_ptr;
        }
        else
        {
            return NULL;
        }
    }

    Char_B4* AsCharB4_40FEA0()
    {
        if (field_30_sprite_type_enum == sprite_types_enum::ped)
        {
            return field_8_char_b4_ptr;
        }
        else
        {
            return NULL;
        }
    }

    bool Is2C_40FE80()
    {
        return field_30_sprite_type_enum == 4 || field_30_sprite_type_enum == 5 || field_30_sprite_type_enum == 1;
    }

    Object_2C* As2C_40FEC0()
    {
        if (Is2C_40FE80())
        {
            return field_8_object_2C_ptr;
        }
        else
        {
            return NULL;
        }
    }

    inline s32 sub_4BA200()
    {
        return field_2C & 3;
    }

    inline u32 sub_4BA210()
    {
        //return field_2C >> 3;
        return field_2C & 0xFFFFFFF8;
    }

    inline s32 sub_4B9BA0()
    {
        if ((field_2C & 4) == 0)
        {
            return gLightingDrawFlag_7068F4;
        }
        return 0;
    }

    // matched on 9.6f but slight different on 10.5: https://decomp.me/scratch/iNjwT
    inline u32 sub_4BAC60()
    {
        u32 flags;
        switch (sub_4BA200())
        {
            case 0:
                return sub_4B9BA0() | 0x80;
            case 1:
                flags = sub_4BA210();
                gTileVerts_7036D0[0].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[1].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[2].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[3].diff = (flags << 24) | 0xFFFFFF;
                return sub_4B9BA0() | 0x2180;
                break;
            case 2:
                flags = sub_4BA210();
                gTileVerts_7036D0[0].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[1].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[2].diff = (flags << 24) | 0xFFFFFF;
                gTileVerts_7036D0[3].diff = (flags << 24) | 0xFFFFFF;
                return sub_4B9BA0() | 0x2280;
                break;
            default:
                return 0;
        }
    }

    Ang16 field_0;
    s8 field_2_pad;
    s8 field_3_pad;
    Sprite_4C* field_4_0x4C_len;
    // Only one field is enable at the same time.
    // The field that is enabled is decided by field_30_sprite_type_enum
    // The following list may not be complete.
    // At this moment, it comes from the functions(version 9.6f) at 4b9a10, 4b9a30, 4b9d50
    union
    {
        Car_BC* field_8_car_bc_ptr; // field_30_sprite_type_enum == sprite_types_enum::car
        Char_B4* field_8_char_b4_ptr; // field_30_sprite_type_enum == sprite_types_enum::ped
        Object_2C* field_8_object_2C_ptr; // field_30_sprite_type_enum == sprite_types_enum::map_obj or code_obj1 or unknown_1
        Sprite* field_8_pSprite;
        class Object_5C* field_8_o5C;
    };
    // Only one field is enable at the same time.
    // As far as I understand, the mpNext is only enabled while the instance is being handled by Sprite_Pool.
    // Otherwise, the field_C_sprite_4c_ptr is active. i.e. the instance belongs to another object.
    union
    {
        Sprite* mpNext;
        Sprite_4C* field_C_sprite_4c_ptr;
        class Object_5C* field_C_o5c;
    };
    infallible_turing* field_10_sound;

    // 9.6f inline 0x420690
    inline void set_ang_lazy_420690(Ang16 a1)
    {
        if (a1 != field_0)
        {
            field_0 = a1;
            ResetZCollisionAndDebugBoxes_59E7B0();
        }
    }

    // 9.6f inline 0x420600
    inline void set_xyz_lazy_420600(Fix16 xpos, Fix16 ypos, Fix16 zpos)
    {
        if (field_14_xy.x != xpos || field_14_xy.y != ypos || field_1C_zpos != zpos)
        {
            field_14_xy.x = xpos;
            field_14_xy.y = ypos;
            field_1C_zpos = zpos;
            ResetZCollisionAndDebugBoxes_59E7B0();
        }
    }

    // 9.6f inline 0x447E20
    void set_xy_lazy_447E20(Fix16 x_target, Fix16 y_target)
    {
        if (field_14_xy.x != x_target || field_14_xy.y != y_target)
        {
            field_14_xy.x = x_target;
            field_14_xy.y = y_target;
            ResetZCollisionAndDebugBoxes_59E7B0();
        }
    }

    // TODO: get 9.6f addr
    void set_z_lazy(Fix16 zpos)
    {
        if (field_1C_zpos != zpos)
        {
            field_1C_zpos = zpos;
            ResetZCollisionAndDebugBoxes_59E7B0();
        }
    }

    inline Fix16 GetXPos()
    {
        return field_14_xy.x;
    }

    inline Fix16 GetYPos()
    {
        return field_14_xy.y;
    }

    inline Fix16 GetZPos()
    {
        return field_1C_zpos;
    }

    Fix16_Point_POD field_14_xy;
    Fix16 field_1C_zpos;
    s16 field_20_id;
    s16 field_22_sprite_id;
    s16 field_24_remap;
    char_type field_26_pad;
    char_type field_27_pad;
    s32 field_28_num;
    char_type field_2C;
    char_type field_2D_pad;
    char_type field_2E_pad;
    char_type field_2F_pad;
    s32 field_30_sprite_type_enum; // Uses the enum defined in the namespace sprite_types_enum
    s32 field_34;
    char_type field_38;
    char_type field_39_z_col;
    char_type field_3A;
    char_type field_3B;
};

class Sprite_14
{
  public:
    EXPORT void sub_48F5A0();

    EXPORT void sub_48F5C0(u8 xCount, u8 yCount);

    // 9.6f 0x44af30
    EXPORT Sprite_14()
    {
        field_C = 0;
        field_8 = 0;
        field_4 = -1;
        field_12 = 0;
    }

    // 9.6f 0x44AF70
    void Invalidate_44AF70()
    {
        field_4 = -1;
    }

    u8* field_0;
    s16 field_4;
    char_type field_6;
    char_type field_7;
    s32 field_8;
    s32 field_C;
    u16 field_10;
    u16 field_12;
};

class Sprite_3CC
{
  public:
    EXPORT Sprite_14* sub_48F600(u16* a2, u32* a3, u32* a4, u16* a5);
    EXPORT Sprite_14* sub_48F690(u32* a2);
    EXPORT void InvalidateMasksByType_48F6E0(u16* a2);
    EXPORT void InvalidateAllMasks_48F710();
    EXPORT Sprite_3CC();
    EXPORT ~Sprite_3CC();

    u8* get_s14(u16 idx)
    {
        return field_0[idx].field_0;
    }

    Sprite_14 field_0[48];
    s32 field_3C0;
    u8* field_3C4;
    s32* field_3C8;
};

class Sprite_8
{
  public:
    EXPORT void sub_5A5860();
    EXPORT void sub_5A5870();
    EXPORT Sprite_8();
    EXPORT ~Sprite_8();
    s32 field_0;
    s16 field_4_id_base;
    s16 field_6;
};

class Sprite_18
{
  public:
    EXPORT ~Sprite_18();
    EXPORT Sprite_18();

    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    // TODO: ordering ?

    EXPORT void sub_5A6A20();

    EXPORT void sub_5A69E0();

    Sprite* field_0;
    Sprite_18* mpNext;
    Fix16_Point field_8;
    Ang16 field_10;
    s16 field_12;
    s32 field_14_rng;
};

class Sprite_18_Pool
{
  public:
    // Inlined, from 9.6f at 0x4bdcf0
    EXPORT Sprite_18_Pool()
    {
    }

    EXPORT ~Sprite_18_Pool();

    Sprite_18* Allocate()
    {
        return field_0_pool.Allocate();
    }

    void DeAllocate(Sprite_18* pItem)
    {
        field_0_pool.DeAllocate(pItem);
    }

    PoolBasic<Sprite_18, 300> field_0_pool;
};

class Sprite_4C_Pool
{
  public:
    // Inlined, from 9.6f at 0x4bc9a0
    Sprite_4C_Pool()
    {
    }

    ~Sprite_4C_Pool();

    // TODO: Get 9.6f inline addr
    void Remove(Sprite_4C* pToRemove)
    {
        field_0_pool.DeAllocate(pToRemove);
    }

    // TODO: Get 9.6f inline addr
    Sprite_4C* Allocate()
    {
        return field_0_pool.Allocate();
    }

    PoolBasic<Sprite_4C, 5031> field_0_pool;
};

class Sprite_Pool
{
  public:
    // Inlined, from 9.6f at 0x4bca20
    Sprite_Pool()
    {
    }

    EXPORT ~Sprite_Pool();

    // Inlined, from 9.6f at 0x421000
    Sprite* get_new_sprite()
    {
        return field_0_pool.Allocate();
    }

    // Inlined, from 9.6f at 0x421030
    void remove(Sprite* pSprite)
    {
        field_0_pool.DeAllocate(pSprite);
    }

    PoolBasic<Sprite, 5031> field_0_pool;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Sprite_Pool, 0x49B28)

EXTERN_GLOBAL(Sprite_8*, gSprite_8_703820);

EXTERN_GLOBAL(Sprite_Pool*, gSprite_Pool_703818);

EXTERN_GLOBAL(Sprite_4C_Pool*, gSprite_4C_Pool_70381C);

EXTERN_GLOBAL(Sprite_3CC*, gSprite_3CC_67AF1C);

EXTERN_GLOBAL(Sprite_18_Pool*, gSprite_18_Pool_703B80);

EXTERN_GLOBAL(Sprite*, gSprite_6F61E8);
