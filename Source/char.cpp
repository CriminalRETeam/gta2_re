#include "char.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Object_3C.hpp"
#include "Particle_8.hpp"
#include "PurpleDoom.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL(Char_C*, gChar_C_6787BC, 0x6787BC);
DEFINE_GLOBAL(PedPool*, gPedPool_6787B8, 0x6787B8);
DEFINE_GLOBAL(Char_B4_Pool*, gChar_B4_Pool_6FDB44, 0x6FDB44);
DEFINE_GLOBAL(Char_8_Pool*, gChar_8_Pool_678b50, 0x678b50);

DEFINE_GLOBAL(s8, byte_6FDB48, 0x6FDB48);
DEFINE_GLOBAL(Fix16, dword_6FD80C, 0x6FD80C);

DEFINE_GLOBAL(Fix16, dword_6FD7F8, 0x6FD7F8);
DEFINE_GLOBAL(Fix16, dword_6FD800, 0x6FD800);
DEFINE_GLOBAL(Fix16, dword_6FD7FC, 0x6FD7FC);
DEFINE_GLOBAL(Fix16, dword_6FD7B0, 0x6FD7B0);

DEFINE_GLOBAL_INIT(Ang16, dword_6FD936, Ang16(720), 0x6FD936);

DEFINE_GLOBAL(u8, byte_6FDB55, 0x6FDB55);
DEFINE_GLOBAL(u8, byte_6FDB58, 0x6FDB58);

STUB_FUNC(0x544F70)
void __stdcall sub_544F70()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x544ff0)
Char_B4::Char_B4()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5451A0)
Char_B4::~Char_B4()
{
    this->field_18 = 0;
    this->field_1C = 0;
    this->mpNext = 0;
    this->field_7C_pPed = 0;
    this->field_80_sprite_ptr = 0;
    this->field_84 = 0;
}

STUB_FUNC(0x5453d0)
void Char_B4::PoolDeallocate()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5454B0)
void Char_B4::sub_5454B0()
{
    field_B0 = -1;
    field_88_obj_2c.sub_5A7080();
}

STUB_FUNC(0x5454d0)
void Char_B4::sub_5454D0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x545530)
void Char_B4::sub_545530(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x545570)
s32 Char_B4::sub_545570()
{
    return field_80_sprite_ptr->sub_59E1D0();
}

STUB_FUNC(0x5455f0)
void Char_B4::sub_5455F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x545600)
s16 Char_B4::sub_545600()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5456a0)
void Char_B4::sub_5456A0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x545700)
s32 Char_B4::sub_545700()
{
    NOT_IMPLEMENTED;
    return 0;
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
    dword_6FD7F8 = field_80_sprite_ptr->field_14_xpos.x;
    dword_6FD800 = field_80_sprite_ptr->field_14_xpos.y;
    dword_6FD7FC = field_80_sprite_ptr->field_1C_zpos;
    dword_6FD7B0 = Fix16(field_45);
    byte_6FDB55 = 0;
    byte_6FDB58 = 0;

    gPurpleDoom_1_679208->sub_477B60(field_80_sprite_ptr);

    if (field_58_flags_bf.b5)
    {
        field_80_sprite_ptr->sub_420600(field_A4_xpos, field_A8_ypos, field_AC_zpos);
        field_58_flags_bf.b5 = 0;
    }
    else
    {
        switch (field_8_ped_state_1)
        {
            case 0:
                Char_B4::state_0_54DDF0();
                break;
            case 1:
                Char_B4::state_1_5504F0();
                break;
            case 2:
                Char_B4::state_1_5519F0();
                break;
            case 3:
                Char_B4::state_3_551A00();
                break;
            case 4:
                Char_B4::state_4_551B30();
                break;
            case 5:
                Char_B4::state_5_551BB0();
                break;
            case 6:
                nullsub_28();
                break;
            case 7:
                Char_B4::state_7_551CB0();
                break;
            case 8:
                Char_B4::state_8_5520A0();
                break;
            case 9:
                Char_B4::state_9_552E90();
                break;
            default:
                break;
        }
        if (field_A0)
        {
            if (field_8_ped_state_1 != 9)
            {
                field_80_sprite_ptr->field_28_num = 34;
            }
        }
        if (field_10 == 15)
        {
            if (field_6C != 5 && field_8_ped_state_1 != 9)
            {
                Char_B4::sub_5454D0();
            }
        }
        else
        {
            field_A0 = 0;
        }
        Char_B4::sub_546360();

        field_80_sprite_ptr->sub_420690(field_40_rotation);

        if (field_58_flags_bf.b3)
        {
            // clockwise?
            field_98.sub_41E210(-field_38, field_40_rotation);
        }
        else
        {
            // anti-clockwise?
            field_98.sub_41E210(field_38, field_40_rotation);
        }
    }
    gPurpleDoom_1_679208->sub_477B20(field_80_sprite_ptr);
    if (field_88_obj_2c.field_0_p18)
    {
        field_88_obj_2c.sub_5A6F70(field_80_sprite_ptr);
    }
    if (field_6A > 0)
    {
        field_6A--;
    }
}

STUB_FUNC(0x5459c0)
char_type Char_B4::sub_5459C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/0bMp2 it matches on decompme
STUB_FUNC(0x5459e0)
void Char_B4::sub_5459E0()
{
    field_7C_pPed->sub_45C500(8);
    field_7C_pPed->sub_45C540(20);
    field_16 = 1;
    Ang16 rotation = dword_6FD936 + field_80_sprite_ptr->field_0;
    rotation.Normalize();

    gParticle_8_6FD5E8->sub_53F060(field_80_sprite_ptr->field_14_xpos.x,
                                   field_80_sprite_ptr->field_14_xpos.y,
                                   field_80_sprite_ptr->field_1C_zpos,
                                   rotation,
                                   1);

    if ((field_7C_pPed->field_21C & 0x1000000) == 0)
    {
        field_7C_pPed->field_250 = 28;
    }

    if (field_7C_pPed->field_204)
    {
        if (gChar_C_6787BC->PedById(field_7C_pPed->field_204))
        {
            field_7C_pPed->field_290 = 5;
            field_7C_pPed->field_264 = 50;
        }
    }
}

STUB_FUNC(0x546360)
void Char_B4::sub_546360()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x548590)
void Char_B4::sub_548590()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x548670)
void Char_B4::sub_548670(char_type a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x548840)
void Char_B4::sub_548840(Object_2C* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x548bd0)
void Char_B4::sub_548BD0(s32 a2)
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

STUB_FUNC(0x54c500)
char_type Char_B4::sub_54C500(char_type a2, char_type a3)
{
    NOT_IMPLEMENTED;
    return 0;
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
s16 Char_B4::sub_54C900(s32 a2)
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

STUB_FUNC(0x54dd70)
void Char_B4::sub_54DD70()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x54ddf0)
void Char_B4::state_0_54DDF0()
{
    NOT_IMPLEMENTED;
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
    return 0;
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

STUB_FUNC(0x5519F0)
void Char_B4::state_1_5519F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x551A00)
void Char_B4::state_3_551A00()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x551B30)
void Char_B4::state_4_551B30()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x551BB0)
void Char_B4::state_5_551BB0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x551CB0)
void Char_B4::state_7_551CB0()
{
    NOT_IMPLEMENTED;
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

EXPORT void Char_B4::nullsub_28()
{
    NOT_IMPLEMENTED;
}

//STUB_FUNC(0x5519f0)
//void j_Char_B4::state_1_5504F0()
//{
//    NOT_IMPLEMENTED;
//}

STUB_FUNC(0x46eb60)
void Char_C::sub_46EB60(u32* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4703f0)
void Char_C::PedsService_4703F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x470650)
Char_C::Char_C()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4709b0)
Char_C::~Char_C()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x470a50)
Ped* Char_C::SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation)
{
    Ped* pPed = gPedPool_6787B8->Allocate();

    if (!pPed->sub_45C830(xpos, ypos, zpos))
    {
        return 0;
    }
    pPed->field_168_game_object->field_40_rotation.rValue = rotation.rValue;
    pPed->field_244_remap = remap;

    Char_B4* pB4 = pPed->field_168_game_object;
    pB4->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pB4->field_80_sprite_ptr->SetRemap(remap);
    }
    pPed->field_134 = rotation;
    pPed->field_288_threat_search = 2;
    pPed->field_28C_threat_reaction = 3;
    pPed->field_216_health = 100;
    pPed->field_26C_graphic_type = 0;
    return pPed;
}

MATCH_FUNC(0x470b00)
Ped* Char_C::SpawnDriver_470B00(Car_BC* pCar)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_238 = 3;
    pNewPed->field_240_occupation = 4; //unknown_2;
    pNewPed->field_16C_car = pCar;
    pNewPed->field_168_game_object = 0;
    pNewPed->sub_45C500(10);
    pNewPed->sub_45C540(10);
    pNewPed->field_248_enter_car_as_passenger = 0;
    pNewPed->field_24C_target_car_door = 0;
    pNewPed->field_288_threat_search = 2; //area_2;
    pNewPed->field_28C_threat_reaction = 3; //run_away_3;
    pNewPed->field_216_health = 100;
    pNewPed->field_26C_graphic_type = 0;
    pCar->SetDriver(pNewPed);
    return pNewPed;
}

STUB_FUNC(0x470ba0)
Ped* Char_C::SpawnGangDriver_470BA0(Car_BC* pCar, Gang_144* pGang)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x470cc0)
Ped* Char_C::sub_470CC0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x470d60)
Ped* Char_C::sub_470D60()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x470e30)
Ped* Char_C::sub_470E30()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x470f30)
Ped* Char_C::sub_470F30()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x470f90)
Ped* Char_C::sub_470F90(Ped* pSrc)
{
    Ped* pDst = gPedPool_6787B8->Allocate();
    Ped* pNext = pDst->mpNext;
    memcpy(pDst, pSrc, sizeof(Ped));
    pDst->mpNext = pNext;

    if (pSrc->field_168_game_object)
    {
        pDst->sub_45C830(pSrc->field_1AC_cam.x, pSrc->field_1AC_cam.y, pSrc->field_1AC_cam.z);
        Char_B4* pCharObj = pDst->field_168_game_object;
        u8 remap = pSrc->field_244_remap;
        pCharObj->field_5_remap = remap;
        if (remap != 0xFF)
        {
            pCharObj->field_80_sprite_ptr->SetRemap(remap);
        }
        pDst->field_168_game_object->set_rotation_433A30(pSrc->GetRotation());
        pDst->field_168_game_object->field_16 = 1;
        pDst->field_168_game_object->field_84 = pSrc->field_168_game_object->field_84;
    }
    return pDst;
}

STUB_FUNC(0x471060)
void Char_C::DoIanTest_471060(s16 a1)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4710c0)
Ped* Char_C::PedById(s32 pedId)
{
    for (Ped* pPedIter = gPedPool_6787B8->field_0_pool.field_4_pPrev; pPedIter; pPedIter = pPedIter->mpNext)
    {
        if (pPedIter->field_200_id == pedId)
        {
            return pPedIter;
        }
    }
    return NULL;
}

MATCH_FUNC(0x471110)
PedPool::~PedPool()
{
}