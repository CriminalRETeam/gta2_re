#pragma once

#include "Fix16_Rect.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

EXTERN_GLOBAL(Ang16, gAng16_703804);
EXTERN_GLOBAL(Fix16, gFix16_7035C0);

class Car_BC;
class Char_B4;
class Object_2C;
class infallible_turing;
class Ped;

class Sprite_4C
{
  public:
    EXPORT s32 sub_5A57A0();
    EXPORT void SetCurrentRect_5A4D90();
    EXPORT void sub_5A3550(Fix16 x, Fix16 y, Fix16 z, Ang16 ang);

    EXPORT Sprite_4C();
    EXPORT ~Sprite_4C();
    Fix16 field_0_width;
    Fix16 field_4_height;
    Fix16 field_8;
    Fix16_Point field_C_b_box[4];
    Sprite_4C* field_2C_pNext;
    Fix16_Rect field_30;
    char_type field_48;
    char_type field_49;
    char_type field_4A;
    char_type field_4B;
};

class Sprite
{
  public:
    EXPORT Fix16_Point get_x_y_443580();
    EXPORT void sub_451950(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void sub_54EC80(Fix16 xpos, Fix16 ypos);
    EXPORT bool sub_59E170();
    EXPORT Ped* sub_59E1B0();
    EXPORT s32 sub_59E1D0();
    EXPORT char_type sub_59E250();
    EXPORT void sub_59E2E0();
    EXPORT void sub_59E300();
    EXPORT void sub_59E320(char_type a2);
    EXPORT bool sub_59E390(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_59E4C0(s32 a2, s32 a3);
    EXPORT char_type sub_59E590(Sprite* a2);
    EXPORT char_type sub_59E680(s32 a2, s16* a3);
    EXPORT void sub_59E7B0();
    EXPORT Sprite* sub_59E7D0(s32 a2);
    EXPORT char_type sub_59E830(Sprite* a1, Ped* a2);
    EXPORT char_type sub_59E850(Sprite* pSprite);
    EXPORT void sub_59E8C0(Sprite* pSprite);
    EXPORT void sub_59E910(Sprite* a2);
    EXPORT void sub_59E960();
    EXPORT Sprite* sub_59E9C0();
    EXPORT void SetRemap(s16 remap);
    EXPORT s16 sub_59EAA0();
    EXPORT char_type sub_59EAE0();
    EXPORT void sub_59EB30(s32 a2, f32* a3);
    EXPORT void ShowHorn_59EE40(s32 a2, s32 a3);
    EXPORT void sub_59EFF0();
    EXPORT void AllocInternal_59F950(Fix16 width, Fix16 height, Fix16 a4);
    EXPORT void sub_59F990();
    EXPORT void sub_59FA40();
    EXPORT void sub_59FAD0();
    EXPORT bool sub_59FB10(s32* a2);
    EXPORT char_type sub_5A0150(s32 a2, u8* a3, u8* a4);
    EXPORT char_type CollisionCheck_5A0320(Fix16* pXY1, Fix16* pXY2, u8* pCollisionIdx1, u8* pCollisionIdx2);
    EXPORT bool sub_5A0380(Sprite* a2);
    EXPORT char_type sub_5A0970(s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_5A0A70(Sprite_4C* a2, Sprite** a3, u8* a4);
    EXPORT char_type sub_5A0EF0(s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_5A1030(Sprite* a2, Sprite** a3, u8* a4);
    EXPORT bool sub_5A1490(s32 a2, s32 a3);
    EXPORT char_type sub_5A19C0();
    EXPORT char sub_5A1A60();
    EXPORT char_type sub_5A1B30(Sprite* a2);
    EXPORT char_type sub_5A1BD0();
    EXPORT char_type sub_5A1CA0(u32* a2);
    EXPORT char_type sub_5A1EB0();
    EXPORT char_type sub_5A21F0();
    EXPORT u32* sub_5A22B0(u32* a2, Sprite* a3);
    EXPORT char_type sub_5A2440();
    EXPORT char_type sub_5A2500();
    EXPORT s16* sub_5A26E0(s16* a2);
    EXPORT s32* sub_5A2710(s32* a2, Sprite* a3, s32* a4, s32 a5, u8* a6, u8* a7, char_type* a8);
    EXPORT void CreateSoundObj_5A29D0();
    EXPORT void FreeSound_5A2A00();
    EXPORT void sub_5A2A30();
    EXPORT void Init_5A2CF0();
    EXPORT void sub_5A3030();
    EXPORT void sub_5A3100(Sprite* a2, Fix16 a3, Fix16 a4, Ang16 a5);

    EXPORT ~Sprite(); // 0x5a3540

    // This function matches, but the marker doesn't seem to work on header files.
    // Leave it here commented for future reference
    //MATCH_FUNC(0x5a5e50)
    EXPORT Sprite() : field_0(gAng16_703804)
    {
        field_4_0x4C_len = NULL;
        field_14_xpos.x = gFix16_7035C0;
        field_14_xpos.y = gFix16_7035C0;
        field_1C_zpos = gFix16_7035C0;
        field_20_id = 0;
        field_22_sprite_id = 0;
        field_24_remap = 0;
        field_28_num = NULL;
        field_2C = 0;
        field_30_sprite_type_enum = 0;
        field_34 = 0;
        field_38 = 0;
        field_39_z_col = -1;
        field_8_car_bc_ptr = NULL;
        field_C_sprite_next_ptr = NULL;
        field_10_sound = NULL;
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
    // As far as I understand, the field_C_sprite_next_ptr is only enabled while the instance is being handled by Sprite_49B28.
    // Otherwise, the field_C_sprite_4c_ptr is active. i.e. the instance belongs to another object.
    union
    {
        Sprite* field_C_sprite_next_ptr;
        Sprite_4C* field_C_sprite_4c_ptr;
        class Object_5C* field_C_o5c;
    };
    infallible_turing* field_10_sound;

    // 9.6f inline 0x420690
    inline void sub_420690(Ang16 a1)
    {
        if (a1 != field_0)
        {
            field_0 = a1;
            sub_59E7B0();
        }
    }

    inline Fix16 GetXPos()
    {
        return field_14_xpos.x;
    }

    inline Fix16 GetYPos()
    {
        return field_14_xpos.y;
    }

    inline Fix16 GetZPos()
    {
        return field_1C_zpos;
    }

    Fix16_Point_POD field_14_xpos;
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
    u8 field_39_z_col;
    char_type field_3A;
    char_type field_3B;
};

class Sprite_14
{
  public:
    // Inlined, from 9.6f at 0x44af30
    EXPORT Sprite_14()
    {
        field_C = 0;
        field_8 = 0;
        field_4 = -1;
        field_12 = 0;
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
    EXPORT s16* sub_48F6E0(u16* a2);
    EXPORT void sub_48F710();
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

    // TODO: ordering ?
    EXPORT Sprite* sub_5A6CA0(s32 a2);

    EXPORT void sub_5A6A20();

    EXPORT void sub_5A69E0();

    Sprite* field_0;
    Sprite_18* field_4_next;
    Fix16_Point field_8;
    Ang16 field_10;
    s16 field_12;
    s32 field_14_rng;
};

class Sprite_1C24
{
  public:
    // Inlined, from 9.6f at 0x4bdcf0
    EXPORT Sprite_1C24()
    {
        Sprite_18* pIter = field_4;
        for (s32 i = 0; i < GTA2_COUNTOF(field_4) - 1; i++)
        {
            pIter->field_4_next = pIter + 1;
            pIter++;
        }

        field_4[299].field_4_next = NULL;
        field_0 = field_4;
    }

    // 0x4BEC40
    Sprite_18* Alloc()
    {
        Sprite_18* p18 = field_0;
        field_0 = field_0->field_4_next;
        return p18;
    }

    void DeAlloc(Sprite_18* pNext)
    {
        pNext->field_4_next = field_0;
        field_0 = pNext;
    }

    EXPORT ~Sprite_1C24();
    Sprite_18* field_0;
    Sprite_18 field_4[300];
};

class Sprite_5D598
{
  public:
    // Inlined, from 9.6f at 0x4bc9a0
    EXPORT Sprite_5D598()
    {
        Sprite_4C* pIter = field_4;
        for (s32 i = 0; i < GTA2_COUNTOF(field_4) - 1; i++)
        {
            pIter->field_2C_pNext = pIter + 1;
            pIter++;
        }

        field_4[0x13a6].field_2C_pNext = NULL;
        field_0_pFree = field_4;
    }
    EXPORT ~Sprite_5D598();

    // TODO: Get 9.6f inline addr
    void Remove(Sprite_4C* pToRemove)
    {
        pToRemove->field_2C_pNext = field_0_pFree;
        field_0_pFree = pToRemove;
    }

    // TODO: Get 9.6f inline addr
    Sprite_4C* Allocate()
    {
        Sprite_4C* pSprite4C = field_0_pFree;
        field_0_pFree = field_0_pFree->field_2C_pNext;
        pSprite4C->sub_5A57A0();
        return pSprite4C;
    }

    Sprite_4C* field_0_pFree;
    Sprite_4C field_4[5031];
};

class Sprite_49B28
{
  public:
    // Inlined, from 9.6f at 0x4bca20
    EXPORT Sprite_49B28()
    {
        Sprite* pIter = field_4;
        for (s32 i = 0; i < GTA2_COUNTOF(field_4) - 1; i++)
        {
            pIter->field_C_sprite_next_ptr = pIter + 1;
            pIter++;
        }

        field_4[0x13a6].field_C_sprite_next_ptr = NULL;
        field_0_first_free = field_4;
    }
    EXPORT ~Sprite_49B28();

    // Inlined, from 9.6f at 0x421000
    Sprite* get_new_sprite()
    {
        Sprite* this_00 = this->field_0_first_free;
        this->field_0_first_free = this_00->field_C_sprite_next_ptr;
        this_00->Init_5A2CF0();
        return this_00;
    }

    // TODO: get 9.6f addr
    void remove(Sprite* pSprite)
    {
        pSprite->sub_5A3030();
        pSprite->field_C_sprite_next_ptr = field_0_first_free;
        field_0_first_free = pSprite;
    }

    Sprite* field_0_first_free;
    Sprite field_4[5031];
};
GTA2_ASSERT_SIZEOF_ALWAYS(Sprite_49B28, 0x49B28)

EXTERN_GLOBAL(Sprite_8*, gSprite_8_703820);

EXTERN_GLOBAL(Sprite_49B28*, gSprite_49B28_703818);

EXTERN_GLOBAL(Sprite_5D598*, gSprite_5D598_70381C);

EXTERN_GLOBAL(Sprite_3CC*, gSprite_3CC_67AF1C);

EXTERN_GLOBAL(Sprite_1C24*, gSprite_1C24_703B80);

EXTERN_GLOBAL(Sprite*, gSprite_6F61E8);
