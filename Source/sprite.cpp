#include "sprite.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "PurpleDoom.hpp"
#include "Rozza_C88.hpp"
#include "char.hpp"
#include "crt_stubs.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "memory.hpp"
#include "root_sound.hpp"

DEFINE_GLOBAL(Sprite_8*, gSprite_8_703820, 0x703820);
DEFINE_GLOBAL(Sprite_4C_Pool*, gSprite_4C_Pool_70381C, 0x70381C);
DEFINE_GLOBAL(Sprite_Pool*, gSprite_Pool_703818, 0x703818);
DEFINE_GLOBAL(Sprite_3CC*, gSprite_3CC_67AF1C, 0x67AF1C);
DEFINE_GLOBAL(Sprite_18_Pool*, gSprite_18_Pool_703B80, 0x703B80);
DEFINE_GLOBAL(Sprite*, gSprite_703814, 0x703814);
DEFINE_GLOBAL(Sprite*, gSprite_6F61E8, 0x6F61E8);
DEFINE_GLOBAL(Fix16, dword_7035C4, 0x7035C4);
DEFINE_GLOBAL(Fix16, gFix16_7035C0, 0x7035C0);
DEFINE_GLOBAL(Ang16, gAng16_703804, 0x703804);
DEFINE_GLOBAL_ARRAY(Fix16, dword_6F6850, 256, 0x6F6850);
DEFINE_GLOBAL_INIT(Fix16, dword_703424, Fix16(0xCCC, 0), 0x703424);

Ang16 word_703804 = 0; //DEFINE_GLOBAL_INIT(Ang16, word_703804, Ang16(0), 0x703804);

MATCH_FUNC(0x443580)
Fix16_Point Sprite::get_x_y_443580()
{
    // TODO: HACK fix me - cast to derived object else no match - but using non POD field_14 breaks Sprite::dtor match
    return *(Fix16_Point*)&field_14_xpos;
}

MATCH_FUNC(0x451950)
void Sprite::sub_451950(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (field_14_xpos.x != xpos || field_14_xpos.y != ypos || field_1C_zpos != zpos)
    {
        field_14_xpos.x = xpos;
        field_14_xpos.y = ypos;
        field_1C_zpos = zpos;
        sub_59E7B0();
    }
}

MATCH_FUNC(0x54EC80)
void Sprite::sub_54EC80(Fix16 xpos, Fix16 ypos)
{
    if (this->field_14_xpos.x != xpos || this->field_14_xpos.y != ypos)
    {
        this->field_14_xpos.x = xpos;
        this->field_14_xpos.y = ypos;
        sub_59E7B0();
    }
}

STUB_FUNC(0x59E170)
bool Sprite::sub_59E170()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x59E1B0)
Ped* Sprite::sub_59E1B0()
{
    if (this->field_30_sprite_type_enum == sprite_types_enum::ped && (this->field_8_char_b4_ptr) != 0)
    {
        return field_8_char_b4_ptr->field_7C_pPed;
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x59E1D0)
s32 Sprite::IsOnWater_59E1D0() // IsWater?
{
    gmp_block_info* pBlock;

    const Fix16 zpos = field_1C_zpos;
    if (zpos > gFix16_7035C0)
    {
        pBlock = gMap_0x370_6F6268->get_block_4DFE10(field_14_xpos.x.ToInt(), field_14_xpos.y.ToInt(), (zpos - dword_7035C4).ToInt());
    }
    else
    {
        pBlock = gMap_0x370_6F6268->get_block_4DFE10(field_14_xpos.x.ToInt(), field_14_xpos.y.ToInt(), zpos.ToInt());
    }

    if (pBlock)
    {
        const u16 lid_idx = pBlock->field_8_lid & 1023;
        if (gGtx_0x106C_703DD4->field_6C_spec[lid_idx] == tile_spec::water && gGtx_0x106C_703DD4->sub_5AA850(lid_idx))
        {
            return true;
        }
    }
    return false;
}

STUB_FUNC(0x59E250)
char Sprite::sub_59E250()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x59e2e0)
void Sprite::sub_59E2E0(void)
{
    sub_59F990();
    memcpy(field_4_0x4C_len, field_C_sprite_4c_ptr, sizeof(Sprite_4C));
}

MATCH_FUNC(0x59e300)
void Sprite::sub_59E300()
{
    memcpy(field_C_sprite_4c_ptr, field_4_0x4C_len, sizeof(Sprite_4C));
}

MATCH_FUNC(0x59e320)
void Sprite::sub_59E320(char_type a2)
{
    u16 v3 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
    sprite_index* sprite_index_5AA440 = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v3);
    u32 field_5_height = sprite_index_5AA440->field_5_height;
    u32 field_4_width = sprite_index_5AA440->field_4_width;

    s32 result;
    if ((s32)(field_4_width & 0xFFFFFFFE) < (s32)(field_5_height & 0xFFFFFFFE))
    {
        result = field_4_width >> 1;
    }
    else
    {
        result = field_5_height >> 1;
    }
    field_38 = a2 + field_38;
    if ((field_38 & 0xFF) > result)
    {
        field_38 = result;
    }
}

STUB_FUNC(0x59e390) // https://decomp.me/scratch/dijmx
bool Sprite::sub_59E390(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x59e4c0)
s32 Sprite::sub_59E4C0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x59E590)
char_type Sprite::sub_59E590(Sprite* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x59E680)
char_type Sprite::sub_59E680(s32 a2, s16* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x59e7b0)
void Sprite::sub_59E7B0()
{
    field_39_z_col = -1;
    if (field_C_sprite_4c_ptr != NULL)
    {
        field_C_sprite_4c_ptr->field_48_bUnknown = 0;
    }
    if (field_4_0x4C_len != NULL)
    {
        field_4_0x4C_len->field_48_bUnknown = 0;
    }
}

MATCH_FUNC(0x59e7d0)
Sprite* Sprite::sub_59E7D0(s32 a2)
{
    Sprite* result;

    sub_59E9C0();
    field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    gSprite_6F61E8 = this;
    if (gMap_0x370_6F6268->sub_4E1520(field_1C_zpos.ToInt()))
    {
        return gRozza_679188.field_20_pSprite;
    }
    result = gPurpleDoom_1_679208->sub_477E60(this, a2);
    if (result)
    {
        gRozza_679188.field_0_type = 3;
        gRozza_679188.field_20_pSprite = result;
    }
    return result;
}

STUB_FUNC(0x59E830)
char_type Sprite::sub_59E830(Sprite* a1, Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x59E850)
char_type Sprite::sub_59E850(Sprite* pSprite)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x59E8C0)
void Sprite::sub_59E8C0(Sprite* pSprite)
{
    s32 sprite_type = this->field_30_sprite_type_enum;
    if (sprite_type == sprite_types_enum::unknown_1 || sprite_type > sprite_types_enum::ped && sprite_type <= sprite_types_enum::map_obj)
    {
        field_8_object_2C_ptr->sub_528E50(pSprite);
        s32 type = pSprite->field_30_sprite_type_enum;
        if (type == sprite_types_enum::code_obj1 || type == sprite_types_enum::map_obj || type == sprite_types_enum::unknown_1)
        {
            if (pSprite->field_8_object_2C_ptr)
            {
                field_8_object_2C_ptr->sub_529000(pSprite->field_8_object_2C_ptr);
            }
        }
    }
}

STUB_FUNC(0x59E910)
void Sprite::sub_59E910(Sprite* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x59e960)
void Sprite::sub_59E960()
{
    switch (this->field_30_sprite_type_enum)
    {
        case sprite_types_enum::car:
            this->field_28_num = 15;
            break;
        case sprite_types_enum::ped:
            this->field_28_num = 23;
            break;
        case sprite_types_enum::code_obj1:
            this->field_28_num = 2;
            break;
        case sprite_types_enum::map_obj:
            this->field_28_num = 2;
            break;
        case sprite_types_enum::code_obj2:
            this->field_28_num = 33;
            break;
        case sprite_types_enum::unknown_0:
            this->field_28_num = 0;
            break;
        case sprite_types_enum::unknown_1:
            this->field_28_num = 0;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x59e9c0)
void Sprite::sub_59E9C0()
{
    if (!field_C_sprite_4c_ptr->field_48_bUnknown)
    {
        if (field_C_sprite_4c_ptr->IsZeroWidth_41E390())
        {
            field_C_sprite_4c_ptr->sub_5A3550(field_14_xpos.x, field_14_xpos.y, field_1C_zpos, word_703804);
        }
        else
        {
            field_C_sprite_4c_ptr->sub_5A3550(field_14_xpos.x, field_14_xpos.y, field_1C_zpos, field_0);
        }
    }
}

STUB_FUNC(0x59ea00)
void Sprite::SetRemap(s16 remap)
{
    NOT_IMPLEMENTED;

    switch (this->field_30_sprite_type_enum)
    {
        case 2:
            this->field_34 = 3;
            this->field_24_remap = remap;
            break;
        case 3:
            this->field_34 = 4;
            this->field_24_remap = remap;
            break;
        case 4:
        case 8:
            this->field_34 = 5;
            this->field_24_remap = remap;
            break;
        case 5:
            this->field_34 = 6;
            this->field_24_remap = remap;
            break;
        case 6:
            this->field_34 = 7;
            this->field_24_remap = remap;
            break;
        case 7:
            this->field_34 = 8;
            this->field_24_remap = remap;
        default:
            this->field_24_remap = remap;
            break;
    }
}

MATCH_FUNC(0x59eaa0)
s16 Sprite::sub_59EAA0()
{
    if (field_34 == 2)
    {
        s16 v2 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
        return gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(2, v2);
    }
    return gGtx_0x106C_703DD4->convert_pal_type_5AA5F0(field_34, field_24_remap);
}

MATCH_FUNC(0x59eae0)
char_type Sprite::has_shadows_59EAE0()
{
    switch (field_30_sprite_type_enum)
    {
        case 3:
            return field_8_char_b4_ptr->sub_5451C0();
        case 2:
            return 1;
        case 1:
        case 4:
        case 5:
            return field_8_object_2C_ptr->field_8->field_62_has_shadows;
        case 6:
        case 7:
        case 8:
            return 0;
        default:
            break;
    }
    return 0;
}

STUB_FUNC(0x59eb30)
void Sprite::sub_59EB30(s32 a2, f32* a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x59ee40)
void Sprite::ShowHorn_59EE40(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x59eff0)
void Sprite::Draw_59EFF0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x59f950)
void Sprite::AllocInternal_59F950(Fix16 a2, Fix16 a3, Fix16 a4)
{
    if (field_C_sprite_4c_ptr == NULL)
    {
        field_C_sprite_4c_ptr = gSprite_4C_Pool_70381C->Allocate();
    }

    Sprite_4C* pSprite4C = field_C_sprite_4c_ptr;
    pSprite4C->field_0_width = a2;
    pSprite4C->field_4_height = a3;
    pSprite4C->field_8 = a4;
}

// https://decomp.me/scratch/ZqbRh
STUB_FUNC(0x59f990)
void Sprite::sub_59F990()
{
    NOT_IMPLEMENTED;
    if (this->field_4_0x4C_len == NULL)
    {
        this->field_4_0x4C_len = gSprite_4C_Pool_70381C->Allocate();

        const u16 sprite_pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
        const sprite_index* sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_pal);

        if (this->field_30_sprite_type_enum == sprite_types_enum::code_obj2)
        {
            field_4_0x4C_len->field_0_width = dword_6F6850[sprite_index->field_4_width].mValue / 2;
            field_4_0x4C_len->field_4_height = dword_6F6850[sprite_index->field_5_height].mValue / 2;
            field_4_0x4C_len->field_8 = 0;
        }
        else
        {
            field_4_0x4C_len->field_0_width = dword_6F6850[sprite_index->field_4_width].mValue;
            field_4_0x4C_len->field_4_height = dword_6F6850[sprite_index->field_5_height].mValue;
            field_4_0x4C_len->field_8 = 0;
        }
    }
}

MATCH_FUNC(0x59fa40)
void Sprite::sub_59FA40()
{
    if (field_4_0x4C_len)
    {
        const u16 idx = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_30_sprite_type_enum, field_22_sprite_id);
        sprite_index* pSprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(idx);

        const Fix16 height = dword_6F6850[pSprite_index->field_5_height];
        const Fix16 width = dword_6F6850[pSprite_index->field_4_width];

        if (width != field_4_0x4C_len->field_0_width || height != field_4_0x4C_len->field_4_height)
        {
            Sprite_4C* t = field_4_0x4C_len;
            t->field_0_width = width;
            t->field_4_height = height;
            t->field_48_bUnknown = 0;
        }
    }
}

MATCH_FUNC(0x59fad0)
void Sprite::sub_59FAD0()
{
    if (field_C_sprite_4c_ptr)
    {
        gSprite_4C_Pool_70381C->Remove(field_C_sprite_4c_ptr);
        field_C_sprite_4c_ptr = 0;
    }

    if (field_4_0x4C_len)
    {
        gSprite_4C_Pool_70381C->Remove(field_4_0x4C_len);
        field_4_0x4C_len = 0;
    }
}

STUB_FUNC(0x59FB10)
bool Sprite::sub_59FB10(s32* a2)
{
    NOT_IMPLEMENTED;
    return false;
}

STUB_FUNC(0x5a0150)
char_type Sprite::sub_5A0150(s32 a2, u8* a3, u8* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a0320)
char_type Sprite::CollisionCheck_5A0320(Fix16* pXY1, Fix16* pXY2, u8* pCollisionIdx1, u8* pCollisionIdx2)
{
    u8 overlapCount = 0;
    const Fix16_Point* pBoundingBox = &field_C_sprite_4c_ptr->field_C_b_box[0];

    for (u8 i = 0; i < 4; i++)
    {
        if ((pBoundingBox[i].x > pXY1[0] && pBoundingBox[i].y > pXY1[1]) && (pBoundingBox[i].x < pXY2[0] && pBoundingBox[i].y < pXY2[1]))
        {
            // If we find the first valid match, store index in pCollisionIdx1
            overlapCount++;

            if (overlapCount == 1)
            {
                *pCollisionIdx1 = i;
            }
            // If we find the second valid match, store index in pCollisionIdx2
            else if (overlapCount == 2)
            {
                *pCollisionIdx2 = i;
            }
        }
    }

    // Return the result count (0, 1, or 2)
    return overlapCount;
}

STUB_FUNC(0x5a0380)
bool Sprite::sub_5A0380(Sprite* a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A0970)
char_type Sprite::sub_5A0970(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A0A70)
char_type Sprite::sub_5A0A70(Sprite_4C* a2, Sprite** a3, u8* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A0EF0)
char_type Sprite::sub_5A0EF0(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a1030)
char_type Sprite::sub_5A1030(Sprite* a2, Sprite** a3, u8* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a1490)
bool Sprite::sub_5A1490(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a19c0)
char_type Sprite::sub_5A19C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a1a60)
char Sprite::sub_5A1A60()
{
    sub_59F990();

    Sprite_4C* p4C = this->field_4_0x4C_len;

    if (!p4C->field_48_bUnknown)
    {
        p4C->sub_5A3550(this->field_14_xpos.x, this->field_14_xpos.y, this->field_1C_zpos, this->field_0);
    }

    field_4_0x4C_len->SetCurrentRect_5A4D90();

    if (gPurple_left_6F5FD4 < 0)
    {
        gPurple_left_6F5FD4 = 0;
    }

    if (gPurple_right_6F5B80 > 255)
    {
        gPurple_right_6F5B80 = 255;
    }

    if (gPurple_top_6F6108 < 0)
    {
        gPurple_top_6F6108 = 0;
    }

    if (gPurple_bottom_6F5F38 > 255)
    {
        gPurple_bottom_6F5F38 = 255;
    }

    return gMap_0x370_6F6268->sub_4E4630(field_1C_zpos);
}

STUB_FUNC(0x5a1b30)
char_type Sprite::sub_5A1B30(Sprite* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a1bd0)
char_type Sprite::sub_5A1BD0()
{
    if (this->field_39_z_col == 0xFF)
    {
        if (this->field_30_sprite_type_enum == sprite_types_enum::car && field_8_car_bc_ptr->is_train_model())
        {
            this->field_39_z_col = (this->field_1C_zpos - dword_7035C4).ToInt();
        }
        else
        {
            this->field_39_z_col = (this->field_1C_zpos - dword_7035C4).ToInt() + sub_5A1A60();
        }
    }
    return field_39_z_col;
}

STUB_FUNC(0x5a1ca0)
char_type Sprite::sub_5A1CA0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A1EB0)
char_type Sprite::sub_5A1EB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A21F0)
char_type Sprite::sub_5A21F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A22B0)
u32* Sprite::sub_5A22B0(u32* a2, Sprite* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A2440)
char_type Sprite::sub_5A2440()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a2500)
char_type Sprite::sub_5A2500()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5A26E0)
s16* Sprite::sub_5A26E0(s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a2710)
s32* Sprite::sub_5A2710(s32* a2, Sprite* a3, s32* a4, s32 a5, u8* a6, u8* a7, char_type* a8)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a29d0)
void Sprite::CreateSoundObj_5A29D0()
{
    if (!field_10_sound)
    {
        if (!bSkip_audio_67D6BE)
        {
            field_10_sound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, 1);
        }
    }
}

MATCH_FUNC(0x5a2a00)
void Sprite::FreeSound_5A2A00()
{
    if (field_10_sound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_10_sound);
        field_10_sound = 0;
    }
}

STUB_FUNC(0x5a2a30)
void Sprite::sub_5A2A30()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5a2cf0)
void Sprite::PoolAllocate()
{
    this->field_2C = 0;
    this->field_28_num = 0;
    this->field_8_car_bc_ptr = 0;
    this->field_14_xpos.x = gFix16_7035C0;
    this->field_14_xpos.y = gFix16_7035C0;
    this->field_1C_zpos = gFix16_7035C0;
    this->field_39_z_col = -1;
    this->field_0 = gAng16_703804;
    this->field_22_sprite_id = 0;
    this->field_24_remap = 0;
    this->field_34 = 2;
    this->field_C_sprite_4c_ptr = 0;
    this->field_4_0x4C_len = 0;
    this->field_38 = 0;
    this->field_10_sound = 0;

    // TODO: Probably an inline
    this->field_20_id = gSprite_8_703820->field_4_id_base++;
    if (!gSprite_8_703820->field_4_id_base)
    {
        gSprite_8_703820->field_4_id_base = 1;
    }
}

MATCH_FUNC(0x5a3030)
void Sprite::PoolDeallocate()
{
    sub_59FAD0();
    this->field_20_id = 0;
    FreeSound_5A2A00();
}

MATCH_FUNC(0x5a3100)
void Sprite::sub_5A3100(Sprite* a2, Fix16 a3, Fix16 a4, Ang16 a5)
{
    Object_2C* o2c;

    switch (field_30_sprite_type_enum)
    {
        case sprite_types_enum::ped:
            field_8_char_b4_ptr->field_88_obj_2c.sub_5A6D00(a2, a3, a4, a5);
            break;
        case sprite_types_enum::car:
            field_8_car_bc_ptr->field_0_qq.sub_5A6D00(a2, a3, a4, a5);
            break;
        case 1: // sprite_type_1_Object_5C
        case 4: // sprite_type_4_Object_5C
        case 5: // sprite_type_5_Object_5C
            o2c = field_8_object_2C_ptr;
            if (!o2c->field_10_obj_3c)
            {
                o2c->sub_52A650();
            }
            field_8_object_2C_ptr->field_10_obj_3c->field_0.sub_5A6D00(a2, a3, a4, a5);
            break;
        default:
            break;
    }

    switch (a2->field_30_sprite_type_enum)
    {
        case sprite_types_enum::car:
            a2->field_8_car_bc_ptr->sub_43AA60();
            break;
        case 1: // sprite_type_1_Object_5C
        case 4: // sprite_type_4_Object_5C
        case 5: // sprite_type_5_Object_5C
            a2->field_8_object_2C_ptr->sub_52A6D0(this);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4833B0)
void Sprite::set_angle_4833B0(Ang16 ang)
{
    if (ang != field_0)
    {
        field_0 = ang;
        sub_59E7B0();
    }
}

MATCH_FUNC(0x5a3540)
Sprite::~Sprite()
{
    FreeSound_5A2A00();
}

MATCH_FUNC(0x5a5e50)
Sprite::Sprite() : field_0(gAng16_703804)
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
    mpNext = NULL;
    field_10_sound = NULL;
}

MATCH_FUNC(0x48F5A0)
void Sprite_14::sub_48F5A0()
{
    Sprite_3CC* pSprt = gSprite_3CC_67AF1C;
    s32 new_idx = ++pSprt->field_3C0;
    field_C = new_idx;
}

MATCH_FUNC(0x48F5C0)
EXPORT void Sprite_14::sub_48F5C0(u8 xCount, u8 yCount)
{
    u8* pData = this->field_0;
    for (s32 y = 0; y < yCount; y++)
    {
        memset(pData, 0, xCount);
        pData += 256;
    }
}

MATCH_FUNC(0x48f600)
Sprite_14* Sprite_3CC::sub_48F600(u16* a2, u32* a3, u32* a4, u16* a5)
{
    s32 final_idx;
    s32 start_idx = 0;
    Sprite_14* pSprt = NULL;
    if (!*a3)
    {
        final_idx = 32;
    }
    else
    {
        start_idx = 32;
        final_idx = 48;
    }

    s32 count = start_idx;
    for (Sprite_14* pIter = &field_0[start_idx]; count < final_idx; count++, ++pIter)
    {
        s32 unk = *a2;
        if (pIter->field_4 == unk && pIter->field_12 == *a5)
        {
            if (pIter->field_8 == *a4)
            {
                return pIter;
            }
            if (pIter->field_8 < *a4 && (pIter->field_8 | (*a4 - pIter->field_8)) == *a4)
            {
                pSprt = pIter;
            }
        }
    }
    return pSprt;
}

MATCH_FUNC(0x48f690)
Sprite_14* Sprite_3CC::sub_48F690(u32* a2)
{
    u32 min_value = -1;
    s32 start_idx;
    s32 final_idx;
    Sprite_14* pSprt = NULL;
    if (!*a2)
    {
        start_idx = 0;
        final_idx = 32;
    }
    else
    {
        start_idx = 32;
        final_idx = 48;
    }

    s32 count = start_idx;
    for (Sprite_14* pIter = &field_0[start_idx]; count < final_idx; count++, ++pIter)
    {
        if (pIter->field_C < min_value)
        {
            min_value = pIter->field_C;
            pSprt = pIter;
        }
    }
    return pSprt;
}

MATCH_FUNC(0x48f6e0)
void Sprite_3CC::sub_48F6E0(u16* a2)
{
    s32 count = 0;
    for (Sprite_14* pIter = &this->field_0[0]; count < 48; count++, ++pIter)
    {
        if (pIter->field_4 == *a2)
        {
            pIter->field_4 = -1;
        }
    }
}

MATCH_FUNC(0x48f710)
void Sprite_3CC::sub_48F710()
{
    Sprite_14* psVar1 = field_0;
    s32 iVar2 = 0x30;
    do
    {
        psVar1->field_4 = -1;
        psVar1++;
        iVar2--;
    } while (iVar2 != 0);
}

MATCH_FUNC(0x48f730)
Sprite_3CC::Sprite_3CC()
{
    u32 iVar4;
    field_3C0 = 0;
    field_3C4 = NULL;
    field_3C8 = NULL;

    // By the way this is later used, it seems to be an array of a structure of size 0x40.
    // For now, it's a s32* as it make the code to match.
    u8* pvVar2 = (u8*)Memory::Aligned_malloc_4FE510(0x40000, (void**)(&field_3C8));
    field_3C4 = pvVar2;

    Sprite_14* tmp = field_0;
    for (iVar4 = 8; iVar4 != 0; iVar4--, pvVar2 += 0x4000, tmp += 4)
    {
        tmp[0].field_0 = pvVar2;
        tmp[1].field_0 = (pvVar2 + 0x40);
        tmp[2].field_0 = (pvVar2 + 0x80);
        tmp[3].field_0 = (pvVar2 + 0xC0);
    }

    tmp = field_0 + 33;
    for (iVar4 = 4; iVar4 != 0; iVar4--, pvVar2 += 0x8000, tmp += 4)
    {
        // I don't know why this one starts at -1...
        // Maybe an artifact of the decompilation
        tmp[-1].field_0 = pvVar2;
        tmp[0].field_0 = (pvVar2 + 0x40);
        tmp[1].field_0 = (pvVar2 + 0x80);
        tmp[2].field_0 = (pvVar2 + 0xC0);
    }

    for (u16 uVar1 = 0; uVar1 < 48; uVar1++)
    {
        field_0[uVar1].field_10 = uVar1;
    }
}

MATCH_FUNC(0x48F7F0)
Sprite_3CC::~Sprite_3CC()
{
    if (this->field_3C8)
    {
        crt::free(this->field_3C8);
    }
    this->field_3C4 = 0;
    this->field_3C8 = 0;
}

MATCH_FUNC(0x5a5860)
void Sprite_8::sub_5A5860()
{
    field_0 = 0;
}

MATCH_FUNC(0x5a5870)
void Sprite_8::sub_5A5870()
{
    gSprite_703814 = gSprite_Pool_703818->get_new_sprite();
    gSprite_703814->AllocInternal_59F950(gFix16_7035C0, gFix16_7035C0, gFix16_7035C0);
}

MATCH_FUNC(0x5a58a0)
Sprite_8::Sprite_8()
{
    gSprite_Pool_703818 = new Sprite_Pool();
    if (gSprite_Pool_703818 == NULL)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 5039);
    }

    gSprite_4C_Pool_70381C = new Sprite_4C_Pool();
    if (gSprite_4C_Pool_70381C == NULL)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 5041);
    }

    gSprite_3CC_67AF1C = new Sprite_3CC();
    if (gSprite_3CC_67AF1C == NULL)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 5043);
    }

    gSprite_18_Pool_703B80 = new Sprite_18_Pool();
    if (gSprite_18_Pool_703B80 == NULL)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\sprite.cpp", 5046);
    }

    field_0 = 0;
    field_4_id_base = 1;
}

MATCH_FUNC(0x5a5b50)
Sprite_8::~Sprite_8()
{
    GTA2_DELETE_AND_NULL(gSprite_Pool_703818);
    GTA2_DELETE_AND_NULL(gSprite_4C_Pool_70381C);
    GTA2_DELETE_AND_NULL(gSprite_3CC_67AF1C);
    GTA2_DELETE_AND_NULL(gSprite_18_Pool_703B80);
    gSprite_703814 = 0;
}

MATCH_FUNC(0x5a5c40)
Sprite_18::~Sprite_18()
{
}

MATCH_FUNC(0x5a5c50)
Sprite_18::Sprite_18()
{
    field_10 = 0;
}

MATCH_FUNC(0x5a5c20)
Sprite_18_Pool::~Sprite_18_Pool()
{
}

MATCH_FUNC(0x5a57a0)
s32 Sprite_4C::PoolAllocate()
{
    mpNext = NULL;
    field_48_bUnknown = 0;
    return 0;
}

MATCH_FUNC(0x5a57b0)
Sprite_4C::Sprite_4C()
{
    PoolAllocate();
}

MATCH_FUNC(0x5a5840)
Sprite_4C::~Sprite_4C()
{
}

MATCH_FUNC(0x5a5be0)
Sprite_4C_Pool::~Sprite_4C_Pool()
{
}

MATCH_FUNC(0x5A5C00)
Sprite_Pool::~Sprite_Pool()
{
}

MATCH_FUNC(0x5A6ca0)
Sprite* Sprite_18::sub_5A6CA0(s32 a2)
{
    if (field_0 != NULL)
    {
        for (Sprite_18* pNext = (Sprite_18*)field_0; pNext != NULL; pNext = pNext->mpNext)
        {
            if (pNext->field_0->field_30_sprite_type_enum == a2)
            {
                return pNext->field_0;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x5A6A20)
void Sprite_18::sub_5A6A20()
{
    if (this->field_0->field_30_sprite_type_enum == sprite_types_enum::car)
    {
        Car_BC* cBC = this->field_0->field_8_car_bc_ptr;
        if (cBC)
        {
            if (cBC->field_88 != 2 && cBC->field_88 != 4 && cBC->field_88 != 3)
            {
                cBC->sub_43DD60();
            }
        }
    }
}

STUB_FUNC(0x5A69E0)
void Sprite_18::sub_5A69E0()
{
    Sprite* pSprite = this->field_0;
    if ((pSprite->field_30_sprite_type_enum == sprite_types_enum::code_obj1 ||
         pSprite->field_30_sprite_type_enum == sprite_types_enum::map_obj ||
         pSprite->field_30_sprite_type_enum == sprite_types_enum::unknown_1) &&
        (pSprite->field_8_object_2C_ptr) != 0)
    {
        Object_2C* pO2c = pSprite->field_8_object_2C_ptr;
        if (pO2c->field_8->field_34 <= 1u)
        {
            if (pO2c->field_18_model == 148)
            {
                pO2c->sub_5290C0(1u);
            }
            else
            {
                pO2c->sub_5290A0();
            }
        }
    }
    else if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car)
    {
        Car_BC* pBC = pSprite->field_8_car_bc_ptr;
        if (pBC)
        {
            pBC->sub_43D7B0(19);
        }
    }
}