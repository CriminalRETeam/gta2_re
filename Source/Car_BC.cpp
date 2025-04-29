#include "Car_BC.hpp"
#include "Car_14_18.hpp"
#include "Car_B0.hpp"
#include "Game_0x40.hpp"
#include "Garox_2B00.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "PurpleDoom.hpp"
#include "RouteFinder.hpp"
#include "Sero_181C.hpp"
#include "Weapon_8.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "text_0x14.hpp"

EXPORT_VAR Car_214* gCar_214_705F20;
GLOBAL(gCar_214_705F20, 0x705F20);

EXPORT_VAR Car_6C* gCar_6C_677930;
GLOBAL(gCar_6C_677930, 0x677930);

EXPORT_VAR Car_E0C4* gCar_E0C4_67792C;
GLOBAL(gCar_E0C4_67792C, 0x67792C);

EXPORT_VAR Sprite* gSprite_6F61E8;
GLOBAL(gSprite_6F61E8, 0x6F61E8);

EXPORT_VAR Sprite* gSprite_6791A8;
GLOBAL(gSprite_6791A8, 0x6791A8);

EXPORT_VAR Car_8F74* gCar_8F74_677CF8;
GLOBAL(gCar_8F74_677CF8, 0x677CF8);

EXPORT_VAR Car_A4* gCar_A4_66AC80;
GLOBAL(gCar_A4_66AC80, 0x66AC80);

EXPORT_VAR Car_14* gCar_14_677934;
GLOBAL(gCar_14_677934, 0x677934);

// This is not used outside this file.
// In fact, it's only allocated and deallocated, it's never used.
EXPORT_VAR Sprite* gSprite_Unused_677938;
GLOBAL(gSprite_Unused_677938, 0x677938);

EXPORT_VAR s32 dword_679188;
GLOBAL(dword_679188, 0x679188);

EXPORT_VAR Fix16 dword_6F6850[256];
GLOBAL(dword_6F6850, 0x6F6850);

EXPORT_VAR Ang16 gAng16_703804;
GLOBAL(gAng16_703804, 0x703804);

EXPORT_VAR Fix16 gFix16_6777CC;
GLOBAL(gFix16_6777CC, 0x6777CC);

EXPORT_VAR Fix16 gFix16_7035C0;
GLOBAL(gFix16_7035C0, 0x7035C0);

EXPORT_VAR CarInfo_2C* gCarInfo_2C_66AB78;
GLOBAL(gCarInfo_2C_66AB78, 0x66AB78);

EXPORT_VAR CarInfo_48* gCarInfo_48_66AB70;
GLOBAL(gCarInfo_48_66AB70, 0x66AB70);

EXPORT_VAR s16 DAT_677CFC;
GLOBAL(DAT_677CFC, 0x677CFC);

// This is a pointer to something
EXPORT_VAR s32 DAT_0067727c;
GLOBAL(DAT_0067727c, 0x67727c);

// This is a pointer to something
EXPORT_VAR s32 DAT_0067737c;
GLOBAL(DAT_0067737c, 0x67737c);

// Indicates if Car_2 is initialised
// It can probably turned into a static variable inside Car_2
EXPORT_VAR char_type byte_679C0A;
GLOBAL(byte_679C0A, 0x679C0A);

// Array of values used by Car_2.
// It can probably turned into a static variable inside Car_2
EXPORT_VAR s16 DAT_00679320[1000];
GLOBAL(DAT_00679320, 0x679320);

EXPORT_VAR s32 dword_6777D0;
GLOBAL(dword_6777D0, 0x6777D0);

EXPORT_VAR s32 dword_6778D0;
GLOBAL(dword_6778D0, 0x6778D0);

EXPORT_VAR Fix16 DAT_006FF744; // 0x147
GLOBAL(DAT_006FF744, 0x6FF744);

EXPORT_VAR Fix16 DAT_006FF774; // 0x0
GLOBAL(DAT_006FF774, 0x6FF774);

EXPORT_VAR Fix16 DAT_006FF570;
GLOBAL(DAT_006FF570, 0x6FF570);

EXPORT_VAR Fix16 DAT_006FF7E8;
GLOBAL(DAT_006FF7E8, 0x6FF7E8);

EXPORT_VAR s8 DAT_006FF8C4;
GLOBAL(DAT_006FF8C4, 0x6FF8C4);

EXPORT_VAR s8 DAT_006FF8C5;
GLOBAL(DAT_006FF8C5, 0x6FF8C5);

EXPORT_VAR Fix16 DAT_006FF778;
GLOBAL(DAT_006FF778, 0x6ff778);

MATCH_FUNC(0x5639c0)
void sub_5639C0()
{
    gCarInfo_2C_66AB78 = NULL;
    gCarInfo_48_66AB70 = NULL;
}

MATCH_FUNC(0x447640)
void sub_447640()
{
    DAT_677CFC = 0;
}

STUB_FUNC(0x4476f0)
void Car_78::sub_4476F0()
{
}

STUB_FUNC(0x447710)
char_type Car_78::sub_447710()
{
    return 0;
}

STUB_FUNC(0x447970)
void Car_78::sub_447970()
{
}

STUB_FUNC(0x447ca0)
char_type Car_78::sub_447CA0(u8 a2, u8 a3, u8 a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x447d40)
char_type Car_78::sub_447D40(s32 a2)
{
    return 0;
}

STUB_FUNC(0x448270)
char_type Car_78::sub_448270()
{
    return 0;
}

STUB_FUNC(0x4482c0)
char_type Car_78::sub_4482C0()
{
    return 0;
}

STUB_FUNC(0x448770)
char_type Car_78::sub_448770()
{
    return 0;
}

STUB_FUNC(0x448ce0)
char_type Car_78::sub_448CE0()
{
    return 0;
}

STUB_FUNC(0x44a1f0)
char_type Car_78::sub_44A1F0()
{
    return 0;
}

STUB_FUNC(0x44af00)
s16 Car_78::sub_44AF00()
{
    return 0;
}

STUB_FUNC(0x44d1d0)
char_type Car_78::sub_44D1D0()
{
    return 0;
}

STUB_FUNC(0x44e0c0)
void Car_78::sub_44E0C0()
{
}

STUB_FUNC(0x44e560)
void Car_78::sub_44E560()
{
}

STUB_FUNC(0x451980)
void Car_78::sub_451980()
{
}

STUB_FUNC(0x451fa0)
void Car_78::sub_451FA0()
{
}

STUB_FUNC(0x451ff0)
s32 Car_78::sub_451FF0()
{
    return 0;
}

STUB_FUNC(0x452060)
char_type Car_78::sub_452060()
{
    return 0;
}

STUB_FUNC(0x452a20)
s16 Car_78::sub_452A20()
{
    return 0;
}

STUB_FUNC(0x452df0)
s16 Car_78::sub_452DF0()
{
    return 0;
}

STUB_FUNC(0x453470)
s16 Car_78::sub_453470()
{
    return 0;
}

STUB_FUNC(0x453590)
s32* Car_78::sub_453590(s32* a2)
{
    return 0;
}

STUB_FUNC(0x4537d0)
char_type Car_78::sub_4537D0()
{
    return 0;
}

STUB_FUNC(0x4538b0)
Car_BC* Car_78::sub_4538B0()
{
    return 0;
}

STUB_FUNC(0x453990)
s32 Car_78::sub_453990(s32 a2)
{
    return 0;
}

STUB_FUNC(0x4539b0)
Car_BC* Car_78::sub_4539B0()
{
    return 0;
}

STUB_FUNC(0x4539d0)
u8* Car_78::sub_4539D0()
{
    return 0;
}

STUB_FUNC(0x453a40)
s32 Car_78::sub_453A40()
{
    return 0;
}

STUB_FUNC(0x453bb0)
s16 Car_78::sub_453BB0()
{
    return 0;
}

MATCH_FUNC(0x453bf0)
void Car_78::sub_453BF0(Car_BC* a2)
{
    field_0 = a2;
}

STUB_FUNC(0x453c00)
u8* Car_78::sub_453C00()
{
    return 0;
}

STUB_FUNC(0x453d80)
s32 Car_78::sub_453D80()
{
    return 0;
}

STUB_FUNC(0x453cb0)
Car_78::Car_78()
{
}

MATCH_FUNC(0x451950)
void Sprite::sub_451950(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (field_14_xpos != xpos || field_18_ypos != ypos || field_1C_zpos != zpos)
    {
        field_14_xpos = xpos;
        field_18_ypos = ypos;
        field_1C_zpos = zpos;
        sub_59E7B0();
    }
}

STUB_FUNC(0x59E170)
bool Sprite::sub_59E170()
{
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

STUB_FUNC(0x59E1D0)
s32 Sprite::sub_59E1D0()
{
    return 0;
}

STUB_FUNC(0x59E250)
char Sprite::sub_59E250()
{
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
s32 Sprite::sub_59E320(char_type a2)
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
    return result;
}

STUB_FUNC(0x59e390) // https://decomp.me/scratch/dijmx
bool Sprite::sub_59E390(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x59e4c0)
s32 Sprite::sub_59E4C0(s32 a2, s32 a3)
{
    return 0;
}

MATCH_FUNC(0x59e7b0)
void Sprite::sub_59E7B0()
{
    field_39 = -1;
    if (field_C_sprite_4c_ptr != NULL)
    {
        field_C_sprite_4c_ptr->field_48 = 0;
    }
    if (field_4_0x4C_len != NULL)
    {
        field_4_0x4C_len->field_48 = 0;
    }
}

MATCH_FUNC(0x59e7d0)
Sprite* Sprite::sub_59E7D0(s32 a2)
{
    Sprite* result;

    sub_59E9C0();
    field_C_sprite_next_ptr->sub_5A4D90();
    gSprite_6F61E8 = this;
    if (gMap_0x370_6F6268->sub_4E1520(field_1C_zpos.ToInt()))
    {
        return gSprite_6791A8;
    }
    result = (Sprite*)gPurpleDoom_1_679208->sub_477E60(this, a2);
    if (result)
    {
        dword_679188 = 3;
        gSprite_6791A8 = result;
    }
    return result;
}

STUB_FUNC(0x59e960)
s32 Sprite::sub_59E960()
{
    return 0;
}

STUB_FUNC(0x59e9c0)
Sprite* Sprite::sub_59E9C0()
{
    return 0;
}

STUB_FUNC(0x59ea00)
void Sprite::SetRemap(s16 remap)
{
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

STUB_FUNC(0x59eae0)
char_type Sprite::sub_59EAE0()
{
    return 0;
}

STUB_FUNC(0x59eb30)
void Sprite::sub_59EB30(s32 a2, f32* a3)
{
}

STUB_FUNC(0x59ee40)
void Sprite::ShowHorn_59EE40(s32 a2, s32 a3)
{
}

STUB_FUNC(0x59eff0)
void Sprite::sub_59EFF0()
{
}

MATCH_FUNC(0x59f950)
void Sprite::sub_59F950(Fix16 a2, Fix16 a3, Fix16 a4)
{
    if (field_C_sprite_4c_ptr == NULL)
    {
        field_C_sprite_4c_ptr = gSprite_5D598_70381C->Allocate();
    }

    Sprite_4C* pSprite4C = field_C_sprite_4c_ptr;
    pSprite4C->field_0_width = a2;
    pSprite4C->field_4_height = a3;
    pSprite4C->field_8 = a4;
}

STUB_FUNC(0x59f990)
void Sprite::sub_59F990()
{
    if (this->field_4_0x4C_len == NULL)
    {
        this->field_4_0x4C_len = gSprite_5D598_70381C->Allocate();

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
            t->field_48 = 0;
        }
    }
}

MATCH_FUNC(0x59fad0)
void Sprite::sub_59FAD0()
{
    if (field_C_sprite_4c_ptr)
    {
        gSprite_5D598_70381C->Remove(field_C_sprite_4c_ptr);
        field_C_sprite_4c_ptr = 0;
    }

    if (field_4_0x4C_len)
    {
        gSprite_5D598_70381C->Remove(field_4_0x4C_len);
        field_4_0x4C_len = 0;
    }
}

STUB_FUNC(0x5a0150)
char_type Sprite::sub_5A0150(s32 a2, u8* a3, u8* a4)
{
    return 0;
}

MATCH_FUNC(0x5a0320)
char_type Sprite::CollisionCheck_5A0320(Fix16* pXY1, Fix16* pXY2, u8* pCollisionIdx1, u8* pCollisionIdx2)
{
    u8 overlapCount = 0;
    const Car_8* pBoundingBox = &field_C_sprite_4c_ptr->field_C_b_box[0];

    for (u8 i = 0; i < 4; i++)
    {
        if ((pBoundingBox[i].field_0 > pXY1[0] && pBoundingBox[i].field_4 > pXY1[1]) &&
            (pBoundingBox[i].field_0 < pXY2[0] && pBoundingBox[i].field_4 < pXY2[1]))
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
    return 0;
}

STUB_FUNC(0x5a1030)
char_type Sprite::sub_5A1030(Sprite* a2, Sprite** a3, u8* a4)
{
    return 0;
}

STUB_FUNC(0x5a1490)
bool Sprite::sub_5A1490(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5a19c0)
char_type Sprite::sub_5A19C0()
{
    return 0;
}

STUB_FUNC(0x5a1a60)
char_type Sprite::sub_5A1A60()
{
    return 0;
}

STUB_FUNC(0x5a1b30)
char_type Sprite::sub_5A1B30(Sprite* a2)
{
    return 0;
}

STUB_FUNC(0x5a1bd0)
char_type Sprite::sub_5A1BD0()
{
    return 0;
}

STUB_FUNC(0x5a1ca0)
char_type Sprite::sub_5A1CA0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x5a2500)
char_type Sprite::sub_5A2500()
{
    return 0;
}

STUB_FUNC(0x5a2710)
s32* Sprite::sub_5A2710(s32* a2, Sprite* a3, s32* a4, s32 a5, u8* a6, u8* a7, char_type* a8)
{
    return 0;
}

MATCH_FUNC(0x5a29d0)
void Sprite::sub_5A29D0()
{
    if (!field_10)
    {
        if (!bSkip_audio_67D6BE)
        {
            field_10 = gRoot_sound_66B038.CreateSoundObject_40EF40(this, 1);
        }
    }
}

MATCH_FUNC(0x5a2a00)
void Sprite::FreeSound_5A2A00()
{
    if (field_10)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_10);
        field_10 = 0;
    }
}

STUB_FUNC(0x5a2a30)
void Sprite::sub_5A2A30()
{
}

STUB_FUNC(0x5a2cf0)
s32 Sprite::sub_5A2CF0()
{
    return 0;
}

MATCH_FUNC(0x5a3030)
void Sprite::sub_5A3030()
{
    sub_59FAD0();
    this->field_20_id = 0;
    FreeSound_5A2A00();
}

STUB_FUNC(0x5a3100)
Car_BC* Sprite::sub_5A3100(Sprite* a2, s32 a3, s32 a4, Ang16 a5)
{
    return 0;
}

MATCH_FUNC(0x5a3540)
Sprite::~Sprite()
{
    FreeSound_5A2A00();
}

STUB_FUNC(0x5A4D90)
void Sprite::sub_5A4D90()
{
}

MATCH_FUNC(0x5c8680)
void Car_214::sub_5C8680(u8 idx)
{
    field_0[idx].field_8 = 0;
    field_0[idx].field_C = 0;
    field_0[idx].field_0 = 0;
    field_0[idx].field_14 = 2;
    field_0[idx].field_4_O2C->sub_5291B0();
    field_0[idx].field_4_O2C = 0;
}

STUB_FUNC(0x5c86c0)
char_type Car_214::sub_5C86C0(s32* a2, u32* a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9)
{
    return 0;
}

MATCH_FUNC(0x5c8750)
void Car_214::sub_5C8750()
{
    field_210_count = 0;
    Car_18* pOff = &field_0[0];
    for (u8 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        pOff->field_10_idx = i;
        pOff->field_8 = 0;
        pOff->field_C = 0;
        pOff->field_0 = 0;
        pOff->field_14 = 1;
        pOff++;
    }
}

STUB_FUNC(0x5c8780)
u16* Car_214::sub_5C8780(u8 a2, Sprite* pCarSprite)
{
    return 0;
}

MATCH_FUNC(0x47bd00)
Car_2::Car_2()
{
    if (byte_679C0A == false)
    {
        byte_679C0A = true;
        for (u16 i = 0; i < 1000; i++)
        {
            DAT_00679320[i] = i;
        }

        for (u16 j = 0; j < 1000; j++)
        {
            s16 tmp = 1000;
            u16 idx = stru_6F6784.get_int_4F7AE0(&tmp);
            s16 next = DAT_00679320[j];
            DAT_00679320[j] = DAT_00679320[idx];
            DAT_00679320[idx] = next;
        }
    }
    field_0 = 0;
}

MATCH_FUNC(0x47bd90)
void Car_2::sub_47BD90()
{
    field_0++;
    if (field_0 == 1000)
    {
        field_0 = 0;
    }
}

STUB_FUNC(0x444980)
void Car_6C::sub_444980()
{
}

STUB_FUNC(0x444ab0)
u32 Car_6C::sub_444AB0(s32 a2, u16* a3, s32 a4, u16* a5)
{
    return 0;
}

STUB_FUNC(0x444cf0)
Car_BC* Car_6C::sub_444CF0(s32 a1, s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x444f80)
Car_BC* Car_6C::sub_444F80(s32 a1, s32 a2, s32 a3, Ped* a4)
{
    return 0;
}

STUB_FUNC(0x444fa0)
Car_BC* Car_6C::sub_444FA0(Fix16 x, Fix16 y, Fix16 z, Ped* pPed)
{
    return 0;
}

STUB_FUNC(0x4458b0)
Car_BC* Car_6C::sub_4458B0(s32 arg0, s32 a3, s32 a4, s32 a2)
{
    return 0;
}

STUB_FUNC(0x446230)
Car_BC* Car_6C::sub_446230(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, Fix16 maybe_w_scale)
{
    return 0;
}

STUB_FUNC(0x446530)
Car_A4_10* Car_6C::sub_446530(s32 a2, s32 a3, Car_BC* a4, s32 a5, s32 a6)
{
    return 0;
}

STUB_FUNC(0x4466c0)
s32 Car_6C::sub_4466C0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x446760)
void Car_6C::sub_446760()
{
}

STUB_FUNC(0x446790)
void Car_6C::sub_446790()
{
}

STUB_FUNC(0x446870)
bool Car_6C::sub_446870(s32 a2)
{
    return 0;
}

STUB_FUNC(0x446930)
u32 Car_6C::sub_446930(s32 a2)
{
    return 0;
}

STUB_FUNC(0x4469f0)
// There are still something missing here.
// But the structure it seems to be complete
Car_6C::Car_6C()
{
    if (!gCar_E0C4_67792C)
    {
        gCar_E0C4_67792C = new Car_E0C4();
        if (!gCar_E0C4_67792C)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8318);
        }
    }

    if (!gCar_D264_6FE3E0)
    {
        gCar_D264_6FE3E0 = new Car_D264();
        if (!gCar_D264_6FE3E0)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8323);
        }
    }

    if (!gCar_14_677934)
    {
        gCar_14_677934 = new Car_14();
        if (!gCar_14_677934)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8329);
        }
    }

    if (!gCar_8F74_677CF8)
    {
        gCar_8F74_677CF8 = new Car_8F74();
        if (!gCar_8F74_677CF8)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8335);
        }
    }

    if (!gCar_A4_66AC80)
    {
        gCar_A4_66AC80 = new Car_A4();
        if (!gCar_A4_66AC80)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8341);
        }
    }

    field_64 = gFix16_6777CC;
    field_60 = 0;
    field_4 = 0;
    field_14 = 1;
    field_9 = 0;
    field_8 = 0;
    field_A = 0;
    field_B = 0;
    field_1A = 0;
    field_1C = 0;
    field_28_recycled_cars = 0;
    field_40_proto_recycled_cars = 0;
    field_30 = 0;
    field_2C = 0;
    field_34_unit_cars = 0;
    field_38 = 0;
    field_3C_mission_cars = 0;
    field_44 = 0;

    // On version 9.6f this is call to a class method
    DAT_0067727c = 0;
    // On version 9.6f this is call to a class method
    DAT_0067737c = 0;
    sub_5639C0();
    sub_447640();

    field_20 = 4;
    field_24 = 1;

    if (!gSprite_Unused_677938)
    {
        gSprite_Unused_677938 = new Sprite();
        if (!gSprite_Unused_677938)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\car.cpp", 8375);
        }
    }

    // On version 9.6f this is call to a class method
    // that initialises the next two fields
    field_4C = 0;
    field_50_tv_van_dir = 0;
    field_54 = 0;
    field_55 = 0;
    field_58 = 0x57;
    field_5C = 0;
    field_C = 87;
    field_69_do_free_shopping = bDo_free_shopping_67D6CD;
}

STUB_FUNC(0x446dc0)
Car_6C::~Car_6C()
{
}

MATCH_FUNC(0x439ec0)
bool Car_BC::sub_439EC0()
{
    bool b = field_84_car_info_idx == 12 || field_84_car_info_idx == 52 || field_84_car_info_idx == 84;
    return b;
}

STUB_FUNC(0x439ee0)
u32* Car_BC::sub_439EE0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x439f30)
u32* Car_BC::sub_439F30(u32* a2)
{
    return 0;
}

MATCH_FUNC(0x439f80)
wchar_t* Car_BC::GetCarStr_439F80()
{
    sprintf(byte_67CE50, "car%d%c%c", field_84_car_info_idx, 0, 0);
    return gText_0x14_704DFC->Find_5B5F90(byte_67CE50);
}

STUB_FUNC(0x439fb0)
u32* Car_BC::sub_439FB0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x43a0e0)
u32* Car_BC::sub_43A0E0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x43a120)
u32* Car_BC::sub_43A120(u32* a2)
{
    return 0;
}

STUB_FUNC(0x43a1d0)
s32* Car_BC::sub_43A1D0(s32* a2)
{
    return 0;
}

MATCH_FUNC(0x43a1f0)
bool Car_BC::is_bus_43A1F0()
{
    if (gSero_181C_6FF1D4->is_bus_579AA0(this))
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x43a230)
bool Car_BC::sub_43A230()
{
    return field_50_car_sprite->field_38 != 0;
}

STUB_FUNC(0x43a240)
s32* Car_BC::sub_43A240(s32* a2)
{
    return 0;
}

STUB_FUNC(0x43a3c0) // ??? stupidly easy function that doesn't match :)
bool Car_BC::sub_43A3C0()
{
    Car_B0* pObj; // eax

    pObj = this->field_58_uni_Car78_or_Car_B0;
    if (!pObj)
    {
        return false;
    }
    s32 type = pObj->field_98_surface_type;
    return (type == 6) ? true : false;
}

STUB_FUNC(0x43a3e0)
s16* Car_BC::sub_43A3E0(s16* a2)
{
    return 0;
}

STUB_FUNC(0x43a450)
u32* Car_BC::sub_43A450(u32* a2)
{
    return 0;
}

STUB_FUNC(0x43a4c0)
s32* Car_BC::sub_43A4C0(s32* a2)
{
    return 0;
}

STUB_FUNC(0x43a590)
u32* Car_BC::sub_43A590(u32* a2)
{
    return 0;
}

STUB_FUNC(0x43a5b0)
u32* Car_BC::sub_43A5B0(u32* a2)
{
    return 0;
}

MATCH_FUNC(0x43a600)
void Car_BC::RemoveAllDamage()
{
    sub_43D400();
    Car_A4_10* v2 = this->field_64_pTrailer;
    if (v2)
    {
        v2->field_C->sub_43D400();
    }
}

STUB_FUNC(0x43a680)
bool Car_BC::AllowResprayOrPlates()
{
    // TODO: which object is sub_5A71A0 part of?
    s32 info_idx; // eax
    s32 info_idx_; // esi

    info_idx = this->field_84_car_info_idx;

    if (info_idx != 59 && info_idx != 60 && info_idx != 61 && info_idx != 6 &&
        (gGtx_0x106C_703DD4->get_car_info_5AA3B0(info_idx)->info_flags & 2) != 2 && this->field_84_car_info_idx != 84 /*&& !sub_5A71A0()*/)
    {
        info_idx_ = this->field_84_car_info_idx;
        if (info_idx_ != 54 && info_idx_ != 22 && info_idx_ != 30 && info_idx_ != 3)
        {
            return true;
        }
    }
    return false;
}

STUB_FUNC(0x43a6f0)
bool Car_BC::IsNotCurrentRemap(u8 remap)
{
    // Does this car info have remaps and is it not the current remap?
    return gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->num_remaps > 1u && field_50_car_sprite->field_24_remap != remap;
}

MATCH_FUNC(0x43a730)
bool Car_BC::IsNotCurrentRemapOfCarAndTrailerCar(u8 remap)
{
    if (field_64_pTrailer)
    {
        // Check trailer car
        return IsNotCurrentRemap(remap) || field_64_pTrailer->field_C->IsNotCurrentRemap(remap);
    }
    return IsNotCurrentRemap(remap);
}

MATCH_FUNC(0x43a780)
void Car_BC::SetCarRemap(u8 remap)
{
    if (IsNotCurrentRemap(remap))
    {
        field_50_car_sprite->SetRemap(remap);
    }

    // trailer ?
    if (field_64_pTrailer)
    {
        if (field_64_pTrailer->field_C->IsNotCurrentRemap(remap))
        {
            field_64_pTrailer->field_C->field_50_car_sprite->SetRemap(remap);
        }
    }
}

STUB_FUNC(0x43a7d0)
void Car_BC::sub_43A7D0()
{
}

STUB_FUNC(0x43a850)
char_type Car_BC::sub_43A850()
{
    return 0;
}

MATCH_FUNC(0x43a950)
void Car_BC::sub_43A950()
{
    Car_B0* pB0 = field_58_uni_Car78_or_Car_B0;
    pB0->field_91_is_foot_brake_on = 1;
    pB0->field_93_is_forward_gas_on = 0;
    pB0->field_94_is_backward_gas_on = 0;
    pB0->field_95 = 0;
}

MATCH_FUNC(0x43a970)
void Car_BC::sub_43A970()
{
    field_58_uni_Car78_or_Car_B0->field_92_is_hand_brake_on = 1;
    Car_B0* pB0 = field_58_uni_Car78_or_Car_B0;
    pB0->field_91_is_foot_brake_on = 1;
    pB0->field_93_is_forward_gas_on = 0;
    pB0->field_94_is_backward_gas_on = 0;
    pB0->field_95 = 0;
}

MATCH_FUNC(0x43a9a0)
void Car_BC::SetDriver(Ped* pNewDriver)
{
    char hand_brake_on; // [esp+Ch] [ebp+4h]
    if (!pNewDriver)
    {
        Car_B0* pB0 = this->field_58_uni_Car78_or_Car_B0;
        if (pB0)
        {
            Ped* pOldDriver = this->field_54_driver;
            if (pOldDriver && pOldDriver->field_15C_player)
            {
                hand_brake_on = pB0->field_92_is_hand_brake_on;
            }
            else
            {
                hand_brake_on = 1;
            }
            pB0->sub_55A860(0, 0, 0, 0, hand_brake_on);
        }
    }
    this->field_54_driver = pNewDriver;
}

STUB_FUNC(0x43a9f0)
void Car_BC::sub_43A9F0()
{
}

STUB_FUNC(0x43aa60)
Car_BC* Car_BC::sub_43AA60()
{
    return 0;
}

STUB_FUNC(0x43aaf0)
char_type Car_BC::sub_43AAF0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x43adc0)
void Car_BC::sub_43ADC0(s32 a2)
{
}

STUB_FUNC(0x43af10)
bool Car_BC::sub_43AF10()
{
    return 0;
}

STUB_FUNC(0x43af40)
char_type Car_BC::sub_43AF40()
{
    return 0;
}

MATCH_FUNC(0x43af60)
void Car_BC::sub_43AF60()
{
    if (field_5C)
    {
        field_A6 |= 0x20u;
    }
}

STUB_FUNC(0x43afe0)
char_type Car_BC::sub_43AFE0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x43b140)
bool Car_BC::sub_43B140(s32 a2)
{
    return 0;
}

STUB_FUNC(0x43b2b0)
bool Car_BC::sub_43B2B0(Ped* a2)
{
    return 0;
}

MATCH_FUNC(0x43b340)
Car_Door_10* Car_BC::GetDoor(u8 door_idx)
{
    return &field_C_doors[door_idx];
}

MATCH_FUNC(0x43b360)
char_type Car_BC::GetRemap()
{
    return *gGtx_0x106C_703DD4->get_car_remap_5AA3D0(field_84_car_info_idx);
}

STUB_FUNC(0x43b380)
void Car_BC::sub_43B380()
{
}

STUB_FUNC(0x43b3d0)
char_type Car_BC::sub_43B3D0()
{
    return 0;
}

STUB_FUNC(0x43b420)
s32 Car_BC::sub_43B420(s32 a2, u32* a3, u32* a4)
{
    return 0;
}

STUB_FUNC(0x43b5a0)
s32* Car_BC::sub_43B5A0(s32 a2, u32* a3, s32* a4)
{
    return 0;
}

STUB_FUNC(0x43b730)
char_type Car_BC::sub_43B730()
{
    return gGame_0x40_67E008->sub_4B97E0(field_50_car_sprite, dword_6778D0);
}

STUB_FUNC(0x43b750)
char_type Car_BC::sub_43B750()
{
    return 0;
}

MATCH_FUNC(0x43b770)
void Car_BC::sub_43B770()
{
    Ped* pDriver = this->field_54_driver;
    if (pDriver)
    {
        if (pDriver->field_240_occupation != 4)
        {
            pDriver->field_204 = this->field_70;
            this->field_54_driver->field_290 = this->field_90;
            this->field_54_driver->field_264 = 50;
        }
    }
    field_4.sub_4716D0(this);
}

STUB_FUNC(0x43b7b0)
char_type Car_BC::sub_43B7B0(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x43b850)
bool Car_BC::sub_43B850(s32 a2)
{
    return 0;
}

STUB_FUNC(0x43b870)
void Car_BC::sub_43B870(s32 a2, s32 a3)
{
}

STUB_FUNC(0x43bb90)
s32 Car_BC::sub_43BB90(char_type a1)
{
    return 0;
}

STUB_FUNC(0x43bbc0)
char_type Car_BC::sub_43BBC0()
{
    return 0;
}

MATCH_FUNC(0x43bc30)
void Car_BC::sub_43BC30()
{
    if (!field_58_uni_Car78_or_Car_B0)
    {
        sub_4419E0();
        field_58_uni_Car78_or_Car_B0->sub_5638C0(this);
        field_58_uni_Car78_or_Car_B0->sub_563560(this->field_50_car_sprite);
    }
    else
    {
        if (field_84_car_info_idx == 59 || field_84_car_info_idx == 60 || field_84_car_info_idx == 61 || field_84_car_info_idx == 6)
        {
            field_58_uni_Car78_or_Car_B0->sub_563560(this->field_50_car_sprite);
        }
    }
}

MATCH_FUNC(0x43bca0)
void Car_BC::sub_43BCA0()
{
    if (field_64_pTrailer)
    {
        field_64_pTrailer->sub_408190();
    }
    else
    {
        sub_43BC30();
    }
}

MATCH_FUNC(0x43bd00)
void Car_BC::sub_43BD00()
{
    if (field_64_pTrailer)
    {
        field_64_pTrailer->sub_4081B0();
    }
    else
    {
        sub_441A10();
    }
}

STUB_FUNC(0x43bd40)
char_type Car_BC::sub_43BD40()
{
    return 0;
}

STUB_FUNC(0x43bf10)
void Car_BC::sub_43BF10()
{
    if ((this->field_A4 & 8) != 0)
    {
        this->field_A4 |= 1u;
    }
    else
    {
        if (!this->field_8_damaged_areas.mask_bit(2))
        {
            this->field_8_damaged_areas.set_bit(5);
        }
        if (!this->field_8_damaged_areas.mask_bit(1))
        {
            this->field_8_damaged_areas.set_bit(22);
        }
        this->field_A4 |= 1u;
    }
}

MATCH_FUNC(0x43bf70)
void Car_BC::sub_43BF70()
{
    if ((this->field_A4 & 8) == 0)
    {
        this->field_8_damaged_areas.clear_bit(5);
        this->field_8_damaged_areas.clear_bit(22);
    }
    this->field_A4 &= ~1u;
}

STUB_FUNC(0x43bfe0)
char_type Car_BC::sub_43BFE0()
{
    return 0;
}

STUB_FUNC(0x43c0c0)
char_type Car_BC::sub_43C0C0()
{
    return 0;
}

STUB_FUNC(0x43c1c0)
u32 Car_BC::sub_43C1C0()
{
    return 0;
}

STUB_FUNC(0x43c260)
car_info* Car_BC::sub_43C260()
{
    return 0;
}

STUB_FUNC(0x43c310)
car_info* Car_BC::sub_43C310()
{
    return 0;
}

STUB_FUNC(0x43c3c0)
car_info* Car_BC::sub_43C3C0()
{
    return 0;
}

STUB_FUNC(0x43c470)
void Car_BC::sub_43C470()
{
}

STUB_FUNC(0x43c500)
s32 Car_BC::sub_43C500()
{
    return 0;
}

STUB_FUNC(0x43c650)
s32 Car_BC::sub_43C650()
{
    return 0;
}

STUB_FUNC(0x43c700)
s32 Car_BC::sub_43C700()
{
    return 0;
}

STUB_FUNC(0x43c840)
void Car_BC::sub_43C840()
{
}

STUB_FUNC(0x43c920)
s32 Car_BC::sub_43C920()
{
    return 0;
}

STUB_FUNC(0x43c9d0)
s16 Car_BC::sub_43C9D0()
{
    return 0;
}

STUB_FUNC(0x43ca80)
s16 Car_BC::sub_43CA80()
{
    return 0;
}

STUB_FUNC(0x43cbe0)
s32 Car_BC::sub_43CBE0()
{
    return 0;
}

STUB_FUNC(0x43cdf0)
Car_BC* Car_BC::sub_43CDF0(char_type a2)
{
    return 0;
}

STUB_FUNC(0x43cf30)
void Car_BC::DamageArea_43CF30(s32 damage_area)
{
}

STUB_FUNC(0x43d1c0)
bool Car_BC::IsAreaDamaged_43D1C0(s32 damage_area)
{
    bool bDamaged;
    switch (damage_area)
    {
        case CarDamageAreas::FrontLeft_0:
            bDamaged = (this->field_8_damaged_areas.m_var & CarDamageAreasFlags::FlagsFrontLeft_4) == CarDamageAreasFlags::FlagsFrontLeft_4;
            break;
        case CarDamageAreas::FrontRight_1:
            bDamaged =
                (this->field_8_damaged_areas.m_var & CarDamageAreasFlags::FlagsFrontRight_8) == CarDamageAreasFlags::FlagsFrontRight_8;
            break;
        case CarDamageAreas::BackLeft_2:
            bDamaged = (this->field_8_damaged_areas.m_var & CarDamageAreasFlags::FlagsBackLeft_2) == CarDamageAreasFlags::FlagsBackLeft_2;
            break;
        case CarDamageAreas::BackRight_3:
            bDamaged = (this->field_8_damaged_areas.m_var & CarDamageAreasFlags::FlagsBackRight_1) == CarDamageAreasFlags::FlagsBackRight_1;
            break;
        case CarDamageAreas::Window_4:
            bDamaged = (this->field_8_damaged_areas.m_var & CarDamageAreasFlags::FlagsWindow_10) == CarDamageAreasFlags::FlagsWindow_10;
            break;
        default:
            bDamaged = 0;
            break;
    }
    return bDamaged;
}

STUB_FUNC(0x43d2c0)
void Car_BC::sub_43D2C0(char_type a2, s32 a3)
{
}

STUB_FUNC(0x43d400)
s32 Car_BC::sub_43D400()
{
    return 0;
}

STUB_FUNC(0x43d690)
void Car_BC::sub_43D690(s32 a2, s32 a4, s32 a5)
{
}

STUB_FUNC(0x43d7b0)
void Car_BC::sub_43D7B0(s32 a2)
{
}

STUB_FUNC(0x43d840)
void Car_BC::sub_43D840(s32 a2)
{
}

STUB_FUNC(0x43da90)
s16 Car_BC::sub_43DA90(s16 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x43db80)
Char_8** Car_BC::sub_43DB80()
{
    return 0;
}

STUB_FUNC(0x43dbd0)
void Car_BC::sub_43DBD0()
{
}

STUB_FUNC(0x43dc00)
bool Car_BC::sub_43DC00()
{
    return 0;
}

STUB_FUNC(0x43dc80)
bool Car_BC::sub_43DC80(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x43dd50)
char_type Car_BC::sub_43DD50()
{
    return 0;
}

STUB_FUNC(0x43dd60)
void Car_BC::sub_43DD60()
{
}

STUB_FUNC(0x43e560)
char_type Car_BC::sub_43E560()
{
    return 0;
}

STUB_FUNC(0x43e8d0)
Car_BC* Car_BC::sub_43E8D0()
{
    return 0;
}

STUB_FUNC(0x43e990)
Ped* Car_BC::sub_43E990()
{
    return 0;
}

STUB_FUNC(0x43ea60)
char_type Car_BC::sub_43EA60(s32 a2)
{
    return 0;
}

STUB_FUNC(0x43f130)
char_type Car_BC::sub_43F130(s32 a2)
{
    return 0;
}

STUB_FUNC(0x440510)
u32* Car_BC::sub_440510(u32* a2)
{
    return 0;
}

MATCH_FUNC(0x440570)
char_type Car_BC::GetPassengersCount_440570()
{
    if ((field_78_flags & 4) != 0)
    {
        return 0;
    }
    else
    {
        return gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->passengers;
    }
}

STUB_FUNC(0x440590)
void Car_BC::sub_440590()
{
}

MATCH_FUNC(0x4405f0)
void Car_BC::SpawnDriverPed()
{
    Ped* pNextPed = gCar_6C_677930->field_4;
    if (!pNextPed)
    {
        pNextPed = gChar_C_6787BC->sub_470CC0(this);
        gCar_6C_677930->field_4 = pNextPed;
    }
    SetDriver(pNextPed);
    this->field_7C_uni_num = 3;
}

MATCH_FUNC(0x440630)
void Car_BC::AddGangDriver_440630(Gang_144* pGang)
{
    Ped* pNewPed = gChar_C_6787BC->SpawnGangDriver_470BA0(this, pGang);
    SetDriver(pNewPed);
    field_7C_uni_num = 3;
}

STUB_FUNC(0x440660)
s32 Car_BC::sub_440660(u8 a2)
{
    return 0;
}

MATCH_FUNC(0x4406b0)
void Car_BC::ShowCarName_4406B0(Ped* pPed)
{
    Player* pPlayer = pPed->field_15C_player;
    if (pPlayer)
    {
        if (pPlayer->field_0)
        {
            gGarox_2B00_706620->sub_5D5240(GetCarStr_439F80());
        }
    }
}

STUB_FUNC(0x4406e0)
void Car_BC::sub_4406E0(Ped* a2)
{
}

STUB_FUNC(0x4407f0)
void Car_BC::sub_4407F0()
{
}

STUB_FUNC(0x440840)
Sprite* Car_BC::sub_440840()
{
    return 0;
}

STUB_FUNC(0x440ac0)
s32 Car_BC::sub_440AC0()
{
    return 0;
}

STUB_FUNC(0x440b10)
s32 Car_BC::sub_440B10()
{
    return 0;
}

STUB_FUNC(0x440b60)
s32 Car_BC::sub_440B60()
{
    return 0;
}

STUB_FUNC(0x440bb0)
s32 Car_BC::sub_440BB0()
{
    return 0;
}

STUB_FUNC(0x440c10)
char_type Car_BC::sub_440C10(char_type a2)
{
    return 0;
}

STUB_FUNC(0x440d90)
char_type Car_BC::sub_440D90(char_type a2)
{
    return 0;
}

STUB_FUNC(0x440f90)
void Car_BC::sub_440F90(char_type a2)
{
}

STUB_FUNC(0x441030)
char_type Car_BC::sub_441030(u8 a2, u8 a3, u8 a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x441080)
char_type Car_BC::sub_441080(u8 a2, u8 a3, u8 a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x4410d0)
char_type Car_BC::sub_4410D0(s16 a2, u8* a3, s32 a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x441360)
char_type Car_BC::sub_441360()
{
    return 0;
}

STUB_FUNC(0x441380)
void Car_BC::sub_441380()
{
}

STUB_FUNC(0x4413b0)
char_type Car_BC::sub_4413B0(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

MATCH_FUNC(0x441520)
void Car_BC::sub_441520()
{
    Car_B0* pB0;
    switch (this->field_9C)
    {
        case 1:
            pB0 = this->field_58_uni_Car78_or_Car_B0;
            if (pB0)
            {
                if (pB0->sub_55A180())
                {
                    this->field_9C = 4;
                }
            }
            break;
        case 4:
            sub_43BFE0();
            this->field_9C = 3;
            break;
        case 2:
            sub_43C0C0();
            this->field_9C = 1;
            break;
        case 5:
            sub_43C0C0();
            this->field_9C = 6;
            break;
        case 7:
            field_50_car_sprite->sub_59E300();
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4415c0)
void Car_BC::sub_4415C0()
{
    Car_B0* pB0 = this->field_58_uni_Car78_or_Car_B0;
    if (pB0 && pB0->IsFootBrakeOn_55A150())
    {
        sub_43BF10();
    }
    else
    {
        sub_43BF70();
    }
}

STUB_FUNC(0x441600)
u32* Car_BC::sub_441600(u32* a2)
{
    return 0;
}

STUB_FUNC(0x4416d0)
char_type Car_BC::sub_4416D0(s32 a2)
{
    return 0;
}

MATCH_FUNC(0x4417d0)
void Car_BC::sub_4417D0()
{
    if (field_A7_horn <= 0)
    {
        return;
    }

    if (field_A7_horn != 248)
    {
        field_A7_horn--;
    }
}

MATCH_FUNC(0x4417f0)
void Car_BC::sub_4417F0()
{
    field_A7_horn = 45;
}

STUB_FUNC(0x441800)
char_type Car_BC::sub_441800(char_type a2)
{
    return 0;
}

MATCH_FUNC(0x4418a0)
void Car_BC::sub_4418A0()
{
    if (field_64_pTrailer)
    {
        sub_4418B0();
    }
}

MATCH_FUNC(0x4418b0)
void Car_BC::sub_4418B0()
{
    sub_43BD40();
    sub_442760();
    field_78_flags |= 1;
}

STUB_FUNC(0x4418d0)
char_type Car_BC::sub_4418D0(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7, char_type a8, char_type a9)
{
    return 0;
}

STUB_FUNC(0x4419e0)
void Car_BC::sub_4419E0()
{
}

STUB_FUNC(0x441a10)
Car_B0* Car_BC::sub_441A10()
{
    return 0;
}

STUB_FUNC(0x441a40)
char_type Car_BC::sub_441A40()
{
    for (s32 i = 0; i < 4; i++)
    {
        /*
        if (field_C[i].field_4_state !=0 && field_C[i].field_4_state !=6)
        {
            return 0;
        }*/
        switch (field_C_doors[i].field_4_state)
        {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                return 0;
            default:
                break;
        }
    }
    return 1;
}

STUB_FUNC(0x441a70)
char_type Car_BC::sub_441A70()
{
    return 0;
}

STUB_FUNC(0x441b00)
void Car_BC::sub_441B00()
{
    Car_Door_10* pIter = field_C_doors;
    s32 tmp;
    for (s32 i = 0; i < GTA2_COUNTOF(field_C_doors); i++)
    {
        pIter->sub_439DA0(&tmp);
        pIter++;
    }
}

MATCH_FUNC(0x441b20)
void Car_BC::sub_441B20()
{
    if (this->field_9C != 3 || this->field_4.field_0_pOwner)
    {
        this->field_8_damaged_areas.clear_bit(15);
    }
    else
    {
        this->field_8_damaged_areas.set_bit(15);
    }
}

STUB_FUNC(0x441b50)
char_type Car_BC::sub_441B50()
{
    return 0;
}

STUB_FUNC(0x441c00)
void Car_BC::sub_441C00()
{
}

STUB_FUNC(0x441d40)
s16 Car_BC::sub_441D40()
{
    return 0;
}

STUB_FUNC(0x441e70)
void Car_BC::sub_441E70()
{
}

MATCH_FUNC(0x442170)
bool Car_BC::sub_442170()
{
    Ped* pDriver = this->field_54_driver;
    if ((!pDriver || !pDriver->field_15C_player) && !sub_43B750())
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x442190)
void Car_BC::sub_442190()
{
    if (field_58_uni_Car78_or_Car_B0->sub_562FE0())
    {
        if (!this->field_54_driver)
        {
            sub_43BD00();
        }
    }
}

MATCH_FUNC(0x4421b0)
char_type Car_BC::sub_4421B0()
{
    if (field_A0_car_kind != 8 && field_7C_uni_num != 5 && !field_4.sub_471710())
    {
        return 0;
    }

    return 1;
}

STUB_FUNC(0x442200)
bool Car_BC::sub_442200()
{
    return 0;
}

STUB_FUNC(0x442310)
void Car_BC::sub_442310()
{
}

STUB_FUNC(0x4424c0)
char_type Car_BC::sub_4424C0()
{
    return 0;
}

STUB_FUNC(0x442520)
s16* Car_BC::sub_442520(s16* a2)
{
    return 0;
}

STUB_FUNC(0x4425d0)
void Car_BC::sub_4425D0()
{
}

MATCH_FUNC(0x4426d0)
void Car_BC::sub_4426D0()
{
    sub_441B00();
    sub_441520();
    if (field_74_damage != 32001)
    {
        sub_4415C0();
        sub_441E70();
    }
    sub_4417D0();
    if (field_84_car_info_idx == 67 && field_9C == 3)
    {
        sub_4425D0();
    }
}

STUB_FUNC(0x442760)
Car_A4_10* Car_BC::sub_442760()
{
    return 0;
}

STUB_FUNC(0x4427a0)
Car_BC* Car_BC::sub_4427A0(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x442810)
s32 Car_BC::sub_442810()
{
    return 0;
}

STUB_FUNC(0x442d10)
s32* Car_BC::sub_442D10()
{
    return 0;
}

STUB_FUNC(0x442d70)
char_type Car_BC::sub_442D70()
{
    return 0;
}

STUB_FUNC(0x443130)
char_type Car_BC::sub_443130()
{
    return 0;
}

STUB_FUNC(0x443170)
char_type Car_BC::sub_443170()
{
    return 0;
}

STUB_FUNC(0x443330)
char_type Car_BC::sub_443330(s32 a2)
{
    return 0;
}

STUB_FUNC(0x443360)
char_type Car_BC::sub_443360(s32 a2, s32 a3, s32 a4, s16 a5)
{
    return 0;
}

MATCH_FUNC(0x4435a0)
char_type Car_BC::sub_4435A0()
{
    return sub_441A70();
}

STUB_FUNC(0x4435b0)
s32 Car_BC::sub_4435B0()
{
    return 0;
}

STUB_FUNC(0x4435f0)
void Car_BC::sub_4435F0()
{
}

STUB_FUNC(0x443710)
Car_6C* Car_BC::sub_443710(s32 a2)
{
    return 0;
}

MATCH_FUNC(0x443A50)
s32 __stdcall Car_BC::get_car_weapon_cost_443A50(s32 weapon_kind)
{
    if (gCar_6C_677930->field_69_do_free_shopping)
    {
        return 0;
    }

    switch (weapon_kind)
    {
        case weapon_type::car_bomb:
            return 5000;
        case weapon_type::oil_stain:
            return 10000;
        case weapon_type::car_smg:
            return 25000;
        case weapon_type::car_mines:
            return 50000;
        default:
            return 0;
    }
}

MATCH_FUNC(0x443AB0)
void __stdcall Car_BC::sub_443AB0(Player* pPlayer, s32 weapon_cost)
{
    if (pPlayer->field_0)
    {
        gGarox_2B00_706620->field_DC.sub_5D3F10(1, "nspraya", weapon_cost);
    }
}

STUB_FUNC(0x443ae0)
void Car_BC::ResprayOrChangePlates(s32 remap)
{
    Player* pPlayer = this->field_54_driver->field_15C_player;
    s32 cost = gCar_6C_677930->field_69_do_free_shopping != 0 ? 0 : 5000;
    if (cost <= pPlayer->field_2D4_unk.sub_592370())
    {
        if (pPlayer->field_0)
        {
            if (remap == 0xFD) // clean plates only
            {
                gGarox_2B00_706620->field_DC.sub_5D3F10(1, "cdone", cost);
                this->field_B4 = 2;
            }
            else
            {
                gGarox_2B00_706620->field_DC.sub_5D3F10(1, "sdone", cost);
            }
        }

        if (remap != 0xFD) // respray
        {
            SetCarRemap(remap);
            ((Object_3C*)this)->sub_5A7110();
            this->field_B4 = 1;
        }

        pPlayer->field_2D4_unk.AddCash_592620(-cost);
        this->field_54_driver->field_20A_wanted_points = 0;
        RemoveAllDamage();
        return;
    }
    sub_443AB0(pPlayer, cost);
    this->field_B4 = 8;
}

MATCH_FUNC(0x443bd0)
void Car_BC::ResprayOrCleanPlates(s32 remap)
{
    if (AllowResprayOrPlates())
    {
        if (IsNotCurrentRemapOfCarAndTrailerCar(remap))
        {
            ResprayOrChangePlates(remap);
        }
        else
        {
            ResprayOrChangePlates(0xFD); // change plates
        }
    }
    else if (field_54_driver->field_15C_player->field_0)
    {
        // I ain't touching that get outta here!
        gGarox_2B00_706620->field_DC.sub_5D4400(1, "nespray");
    }
}

STUB_FUNC(0x443c40)
void Car_BC::sub_443C40(s32 a2)
{
}

MATCH_FUNC(0x443d00)
s32 Car_BC::sub_443D00(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    gPurpleDoom_1_679208->sub_477B60(field_50_car_sprite);
    Sprite* pCarSprite = field_50_car_sprite;
    if (pCarSprite->field_14_xpos != xpos || pCarSprite->field_18_ypos != ypos || pCarSprite->field_1C_zpos != zpos)
    {
        pCarSprite->field_14_xpos = xpos;
        pCarSprite->field_18_ypos = ypos;
        pCarSprite->field_1C_zpos = zpos;
        pCarSprite->sub_59E7B0();
    }
    Car_B0* field_58_uni = field_58_uni_Car78_or_Car_B0;
    if (field_58_uni)
    {
        field_58_uni->sub_563560(field_50_car_sprite);
    }
    return gPurpleDoom_1_679208->sub_477B20(field_50_car_sprite);
}

MATCH_FUNC(0x443d70)
void Car_BC::IncrementCarStats_443D70(s32 a2)
{
    IncrementAllocatedCarType_443DA0(a2);

    if (field_64_pTrailer)
    {
        if (field_64_pTrailer->field_8 == this)
        {
            field_64_pTrailer->field_C->IncrementAllocatedCarType_443DA0(a2);
        }
    }
}

MATCH_FUNC(0x443da0)
void Car_BC::IncrementAllocatedCarType_443DA0(s32 a2)
{
    this->field_A0_car_kind = a2;
    switch (a2)
    {
        case 1:
            ++gCar_6C_677930->field_28_recycled_cars;
            break;
        case 2:
            ++gCar_6C_677930->field_40_proto_recycled_cars;
            break;
        case 5:
            ++gCar_6C_677930->field_30;
            break;
        case 10:
            ++gCar_6C_677930->field_48;
            break;
        case 4:
            ++gCar_6C_677930->field_2C;
            break;
        case 6:
            ++gCar_6C_677930->field_34_unit_cars;
            break;
        case 7:
            ++gCar_6C_677930->field_38;
            break;
        case 8:
            ++gCar_6C_677930->field_3C_mission_cars;
            break;
        case 9:
            ++gCar_6C_677930->field_44;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x443e50)
void Car_BC::sub_443E50()
{
    if (field_A0_car_kind == 1)
    {
        gCar_6C_677930->field_28_recycled_cars--;
        gCar_6C_677930->field_40_proto_recycled_cars++;
        field_A0_car_kind = 2;
    }
}

MATCH_FUNC(0x443e80)
void Car_BC::sub_443E80()
{
    if (field_A0_car_kind == 2)
    {
        gCar_6C_677930->field_28_recycled_cars++;
        gCar_6C_677930->field_40_proto_recycled_cars--;
        field_A0_car_kind = 1;
    }
}

MATCH_FUNC(0x443eb0)
void Car_BC::sub_443EB0(s32 a2)
{
    gCar_6C_677930->sub_4466C0(field_A0_car_kind);
    IncrementCarStats_443D70(a2);
}

MATCH_FUNC(0x443ee0)
void Car_BC::sub_443EE0(s32 a2)
{
    gCar_6C_677930->sub_4466C0(field_A0_car_kind);
    IncrementAllocatedCarType_443DA0(a2);
}

STUB_FUNC(0x443f30)
s32 Car_BC::sub_443F30(s32 a2, s32 a3, s32 a4, s32 a5)
{
    return 0;
}

MATCH_FUNC(0x444020)
void Car_BC::sub_444020()
{
    if (field_94 > 0)
    {
        field_94--;
        if (field_94 == 0)
        {
            field_70 = 0;
            field_90 = 0;
        }
    }
}

STUB_FUNC(0x444090)
u32 Car_BC::sub_444090(Car_BC* a1)
{
    return 0;
}

/*
One of these combinations of the cases will get a match :)
34,12,52,17,84
 34,12,52,84,17
 34,12,17,52,84
 34,12,17,84,52
 34,12,84,52,17
 34,12,84,17,52
 34,52,12,84,17
 34,52,12,17,84
 34,52,17,84,12
 34,52,17,12,84
 34,52,84,17,12
 34,52,84,12,17
 34,17,12,52,84
 34,17,12,84,52
 34,17,52,12,84
 34,17,52,84,12
 34,17,84,12,52
 34,17,84,52,12
 34,84,12,17,52
 34,84,12,52,17
 34,84,52,17,12
 34,84,52,12,17
 34,84,17,52,12
 34,84,17,12,52
 12,34,84,52,17
 12,34,84,17,52
 12,34,52,84,17
 12,34,52,17,84
 12,34,17,84,52
 12,34,17,52,84
 12,52,84,17,34
 12,52,84,34,17
 12,52,34,17,84
 12,52,34,84,17
 12,52,17,34,84
 12,52,17,84,34
 12,17,84,34,52
 12,17,84,52,34
 12,17,34,84,52
 12,17,34,52,84
 12,17,52,84,34
 12,17,52,34,84
 12,84,17,52,34
 12,84,17,34,52
 12,84,34,52,17
 12,84,34,17,52
 12,84,52,34,17
 12,84,52,17,34
 52,34,17,84,12
 52,34,17,12,84
 52,34,84,17,12
 52,34,84,12,17
 52,34,12,17,84
 52,34,12,84,17
 52,12,17,34,84
 52,12,17,84,34
 52,12,84,34,17
 52,12,84,17,34
 52,12,34,84,17
 52,12,34,17,84
 52,17,12,84,34
 52,17,12,34,84
 52,17,84,12,34
 52,17,84,34,12
 52,17,34,12,84
 52,17,34,84,12
 52,84,12,34,17
 52,84,12,17,34
 52,84,17,34,12
 52,84,17,12,34
 52,84,34,17,12
 52,84,34,12,17
 17,34,12,52,84
 17,34,12,84,52
 17,34,52,12,84
 17,34,52,84,12
 17,34,84,12,52
 17,34,84,52,12
 17,12,34,84,52
 17,12,34,52,84
 17,12,52,84,34
 17,12,52,34,84
 17,12,84,52,34
 17,12,84,34,52
 17,52,34,12,84
 17,52,34,84,12
 17,52,12,34,84
 17,52,12,84,34
 17,52,84,34,12
 17,52,84,12,34
 17,84,34,52,12
 17,84,34,12,52
 17,84,12,52,34
 17,84,12,34,52
 17,84,52,12,34
 17,84,52,34,12
 84,34,17,12,52
 84,34,17,52,12
 84,34,12,17,52
 84,34,12,52,17
 84,34,52,17,12
 84,34,52,12,17
 84,12,17,52,34
 84,12,17,34,52
 84,12,34,52,17
 84,12,34,17,52
 84,12,52,34,17
 84,12,52,17,34
 84,52,17,34,12
 84,52,17,12,34
 84,52,34,17,12
 84,52,34,12,17
 84,52,12,17,34
 84,52,12,34,17
 84,17,52,12,34
 84,17,52,34,12
 84,17,34,12,52
 84,17,34,52,12
 84,17,12,34,52
 84,17,12,52,34
*/
STUB_FUNC(0x4441b0)
void Car_BC::sub_4441B0()
{
    s32 info_idx = field_84_car_info_idx;
    switch (info_idx)
    {
        case 34:
            sub_443F30(165, 16719888, 10, 16);
            sub_443F30(171, 16719888, -10, 16);
            sub_443F30(172, 16719888, 10, -32);
            sub_443F30(173, 16719888, -10, -32);
            break;
        case 12:
            sub_443F30(165, 16719888, 10, -16);
            sub_443F30(171, 255, -10, -16);
            break;
        case 52:
            sub_443F30(165, 16719888, 10, 32);
            sub_443F30(171, 255, -10, 32);
            sub_443F30(172, 16719888, 10, -32);
            sub_443F30(173, 255, -10, -32);
            break;
        case 17:
            sub_443F30(165, 16719888, 16, 48);
            sub_443F30(171, 16719888, -16, 48);
            sub_443F30(172, 16719888, 16, -16);
            sub_443F30(173, 16719888, -16, -26);
            break;
        case 84:
            sub_443F30(165, 16719888, 0, 16);
            break;
    }
}

// TODO: matches on decomp.me
STUB_FUNC(0x444490) // https://decomp.me/scratch/Mt1bU
void Car_BC::sub_444490()
{
    this->field_6C_maybe_id = gCar_6C_677930->field_14++;
    this->field_74_damage = 0;
    this->field_8C = 0;
    this->field_8_damaged_areas = 0;
    this->field_4.field_0_pOwner = 0;
    this->field_54_driver = 0;
    this->field_98 = 3;
    this->field_58_uni_Car78_or_Car_B0 = 0;
    this->field_A4 = 0;
    this->field_A5 = 0;
    this->field_76 = 0;
    this->field_7C_uni_num = 3;
    this->field_50_car_sprite = 0;
    this->field_9C = 1;
    this->field_A6 = 0;
    this->field_80 = 0;
    this->field_78_flags = 0;
    //clear();

    ((Object_3C*)this)->sub_5A7010(); // base?
    this->field_A7_horn = 0;
    IncrementCarStats_443D70(0);
    this->field_8D = 0;
    this->field_60 = 0;
    this->field_70 = 0;
    this->field_90 = 0;
    this->field_94 = 0;
    this->field_95 = 0;
    //v3 = dword_6777D0;
    this->field_68 = dword_6777D0;

    this->field_8E = 0;
    this->field_A8 = 0;
    this->field_A9 = 0;
    this->field_B4 = 0;
    this->field_B8 = 0;
    this->field_B0 = 0;
}

MATCH_FUNC(0x4446e0)
void Car_BC::sub_4446E0()
{
    if (field_5C)
    {
        if (field_5C->field_28_junc_idx > 0)
        {
            gRouteFinder_6FFDC8->sub_589930(field_5C->field_28_junc_idx);
        }

        gCar_8F74_677CF8->Remove(field_5C);
        field_5C = 0;
    }
}

MATCH_FUNC(0x4447d0)
void Car_BC::sub_4447D0()
{
    ((Object_3C*)this)->sub_5A7010();

    sub_441A10();

    if (field_50_car_sprite)
    {
        gSprite_49B28_703818->remove(field_50_car_sprite);
        field_50_car_sprite = 0;
    }

    sub_4446E0();

    if (field_60)
    {
        gHamburger_500_678E30->sub_474CC0(field_60);
        field_60 = 0;
    }

    sub_43DB80();

    gWeapon_8_707018->alloc_car_weapon_5E3DF0(this);
    gCar_6C_677930->sub_4466C0(field_A0_car_kind);

    field_6C_maybe_id = 0;
}

MATCH_FUNC(0x444860)
Car_BC::Car_BC()
{
    field_54_driver = 0;
    field_74_damage = 0;
    field_8C = 0;
    field_98 = 0;
    field_9C = 0;
    field_7C_uni_num = 0;
    field_76 = 0;
    field_A4 = 0;
    field_A5 = 0;
    field_A6 = 0;
    field_4C_next = 0;
    field_84_car_info_idx = 87;
    field_50_car_sprite = 0;
    field_58_uni_Car78_or_Car_B0 = 0;
    field_5C = 0;
    field_88 = 0;
    field_6C_maybe_id = 0xFFFF;
    field_64_pTrailer = 0;
    field_78_flags = 0;
    // TODO
    ((Object_3C*)this)->sub_5A7010();
    field_A7_horn = 0;
    field_80 = 0;
    field_A0_car_kind = 0;
    field_8D = 0;
    field_70 = 0;
    field_90 = 0;
    field_94 = 0;
    field_95 = 0;
    field_68.mValue = 0x4000;
    field_8E = 0;
}

MATCH_FUNC(0x444960)
Car_BC::~Car_BC()
{
    field_4C_next = 0;
    field_50_car_sprite = 0;
    field_5C = 0;
}

STUB_FUNC(0x446730)
s32 Car_BC::sub_446730(Car_BC* a1)
{
    return 0;
}

MATCH_FUNC(0x447360)
void Car_BC::sub_447360()
{
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & 2) == 2)
    {
        field_8_damaged_areas.clear_bit(0x0b);
        field_8_damaged_areas.clear_bit(0x0c);
        field_8_damaged_areas.clear_bit(0x0d);
        field_8_damaged_areas.clear_bit(0x0e);
        field_8_damaged_areas.clear_bit(0x1c);
        field_8_damaged_areas.clear_bit(0x1d);
        field_8_damaged_areas.clear_bit(0x1e);
        field_8_damaged_areas.clear_bit(0x1f);
    }
    field_8_damaged_areas.clear_bit(0x06);
    field_8_damaged_areas.clear_bit(0x17);
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 4) == 4)
    {
        field_8_damaged_areas.clear_bit(0x0f);
    }
}

STUB_FUNC(0x52a6d0)
Sprite* Car_BC::sub_52A6D0(Sprite* a2)
{
    return 0;
}

MATCH_FUNC(0x564300)
bool Car_BC::sub_564300()
{
    if (field_54_driver)
    {
        bool result = field_54_driver->sub_45EDE0(2) == 0;
        return result;
    }
    return false;
}

Car_E0C4::~Car_E0C4()
{
    field_0 = NULL;
    field_4_firstCar = NULL;
}

MATCH_FUNC(0x40ac40)
Car_8::~Car_8()
{
}

MATCH_FUNC(0x407b90)
Car_BC* Car_A4_10::sub_407B90(Car_BC* a2)
{
    if (a2 == field_8)
    {
        return field_C;
    }
    return field_8;
}

MATCH_FUNC(0x407bb0)
void Car_A4_10::sub_407BB0(Car_BC* a2, Car_BC* a3)
{
    this->field_8 = a2;
    this->field_C = a3;
    a2->field_64_pTrailer = this;
    this->field_C->field_64_pTrailer = this;
    this->field_0 = 0;
}

STUB_FUNC(0x407bd0)
s32* Car_A4_10::sub_407BD0(s32* a2)
{
    return 0;
}

STUB_FUNC(0x407ce0)
s32* Car_A4_10::sub_407CE0()
{
    return 0;
}

MATCH_FUNC(0x408140)
char_type Car_A4_10::sub_408140()
{
    if (!this->field_8->field_58_uni_Car78_or_Car_B0 && !this->field_C->field_58_uni_Car78_or_Car_B0)
    {
        return 0;
    }

    sub_408190();

    if (!this->field_8->field_58_uni_Car78_or_Car_B0->sub_562FE0() || this->field_8->field_54_driver)
    {
        return this->field_8->sub_43E560();
    }
    sub_4081B0();
    return 0;
}

MATCH_FUNC(0x408190)
void Car_A4_10::sub_408190()
{
    field_8->sub_43BC30();
    field_C->sub_43BC30();
}

MATCH_FUNC(0x4081b0)
void Car_A4_10::sub_4081B0()
{
    field_8->sub_441A10();
    field_C->sub_441A10();
}

MATCH_FUNC(0x4081d0)
char_type Car_A4_10::sub_4081D0()
{
    if (field_8->field_74_damage == 32001)
    {
        if (field_C->field_74_damage != 32001)
        {
            field_C->field_74_damage = 32000;
            field_C->sub_43D840(18);
        }
        return 1;
    }
    else if (this->field_C->field_74_damage == 32001)
    {
        field_8->field_74_damage = 32000;
        field_8->sub_43D840(18);
        return 1;
    }
    else
    {
        return 0;
    }
}

STUB_FUNC(0x408220)
s32 Car_A4_10::sub_408220()
{
    return 0;
}

MATCH_FUNC(0x5822E0)
void sub_5822E0()
{
    DAT_006FF7E8 = DAT_006FF774;
    DAT_006FF570 = DAT_006FF744;
    DAT_006FF8C4 = 0;
    DAT_006FF8C5 = 0;
}

MATCH_FUNC(0x582310)
Car_14::Car_14()
{
    field_9 = 0;
    field_A = 0;
    field_8 = 0;
    field_0 = NULL;
    field_C = NULL;
    field_10 = NULL;
    field_4 = new Car_14_18();
    sub_5822E0();
}

MATCH_FUNC(0x582340)
Car_14::~Car_14()
{
    delete field_4;
    field_4 = NULL;
    field_0 = NULL;
    field_C = NULL;
    field_10 = NULL;
}

MATCH_FUNC(0x582360)
char Car_14::sub_582360(int param_1, Fix16 param_2, Fix16 param_3)
{
    switch (param_1)
    {
        case 1:
        case 2:
            if (field_8 == 0)
            {
                if (param_2 < (field_0->field_7C_win_right - DAT_006FF778))
                {
                    return 1;
                }
            }
            else
            {
                if (param_2 > (field_0->field_78_win_left + DAT_006FF778))
                {
                    return 1;
                }
            }
            break;
        case 3:
        case 4:
            if (field_8 == 0)
            {
                if (param_3 < (field_0->field_84_win_bottom - DAT_006FF778))
                {
                    return 1;
                }
            }
            else
            {
                if (param_3 > (field_0->field_80_win_top + DAT_006FF778))
                {
                    return 1;
                }
            }
            break;
    }
    return 0;
}
