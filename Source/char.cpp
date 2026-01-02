#include "char.hpp"
#include "Car_BC.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "Particle_8.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "Varrok_7F8.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "sprite.hpp"
#include <DINPUT.H>

DEFINE_GLOBAL(PedManager*, gPedManager_6787BC, 0x6787BC);
DEFINE_GLOBAL(PedPool*, gPedPool_6787B8, 0x6787B8);
DEFINE_GLOBAL(Char_B4_Pool*, gChar_B4_Pool_6FDB44, 0x6FDB44);
DEFINE_GLOBAL(Char_8_Pool*, gChar_8_Pool_678b50, 0x678b50);

DEFINE_GLOBAL(s8, byte_6FDB48, 0x6FDB48);
DEFINE_GLOBAL(s8, byte_6FDB49, 0x6FDB49);
DEFINE_GLOBAL(u32, gB4_id_6FDB4C, 0x6FDB4C);

DEFINE_GLOBAL(Fix16, dword_623F44, 0x623F44);
DEFINE_GLOBAL(Fix16, dword_6FD7F8, 0x6FD7F8);
DEFINE_GLOBAL(Fix16, dword_6FD800, 0x6FD800);
DEFINE_GLOBAL(Fix16, dword_6FD80C, 0x6FD80C);
DEFINE_GLOBAL(Fix16, dword_6FD7FC, 0x6FD7FC);

DEFINE_GLOBAL_INIT(Ang16, word_6FD936, Ang16(720), 0x6FD936);

DEFINE_GLOBAL(u8, byte_6FDB55, 0x6FDB55);

DEFINE_GLOBAL(u8, byte_6FDB58, 0x6FDB58);
DEFINE_GLOBAL(u8, byte_6FDB59, 0x6FDB59);

DEFINE_GLOBAL_INIT(Fix16, dword_6FD9E4, Fix16(0), 0x6FD9E4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9F4, Fix16(65536, 0), 0x6FD9F4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7A4, Fix16(0x1000, 0), 0x6FD7A4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7B0, dword_6FD9E4, 0x6FD7B0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7C0, dword_6FD9E4, 0x6FD7C0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD7DC, dword_6FD9E4, 0x6FD7DC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD868, Fix16(256, 0), 0x6FD868);
DEFINE_GLOBAL_INIT(Fix16, gRunOrJumpSpeed_6FD7D0, dword_6FD9F4* dword_6FD868, 0x6FD7D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8B4, dword_6FD9E4, 0x6FD8B4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8B8, dword_6FD9E4, 0x6FD8B8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8BC, dword_6FD9E4, 0x6FD8BC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD8D8, Fix16(0xCCC, 0), 0x6FD8D8);

DEFINE_GLOBAL(u16, gNumPedsOnScreen_6787EC, 0x6787EC);
DEFINE_GLOBAL(u16, word_6787F0, 0x6787F0);
DEFINE_GLOBAL(u8, byte_61A8A1, 0x61A8A1);
DEFINE_GLOBAL(u8, byte_61A8A2, 0x61A8A2);
DEFINE_GLOBAL(u8, unk_6787EE, 0x6787EE);
DEFINE_GLOBAL(u8, unk_6787EF, 0x6787EF);
DEFINE_GLOBAL(u8, byte_6787DA, 0x6787DA);

EXTERN_GLOBAL(u16, word_6787E0);
EXTERN_GLOBAL(u8, byte_6787E2);
EXTERN_GLOBAL(u8, byte_6787E4);
EXTERN_GLOBAL(u8, byte_6787E3);
EXTERN_GLOBAL(u8, byte_6787D8);
EXTERN_GLOBAL(u8, byte_6787D9);
EXTERN_GLOBAL(u8, byte_6787D2);

DEFINE_GLOBAL(u8, byte_6FDB51, 0x6FDB51);
DEFINE_GLOBAL(u8, byte_6FDB52, 0x6FDB52);
DEFINE_GLOBAL(u8, byte_6FDB53, 0x6FDB53);
DEFINE_GLOBAL(u8, byte_6FDB54, 0x6FDB54);

DEFINE_GLOBAL(u8, byte_6FDB56, 0x6FDB56);
DEFINE_GLOBAL(u8, byte_623F48, 0x623F48);
DEFINE_GLOBAL(u8, byte_6FDAD8, 0x6FDAD8);
DEFINE_GLOBAL(u8, byte_6FDAD9, 0x6FDAD9);
DEFINE_GLOBAL(u8, byte_6FDB57, 0x6FDB57);

EXTERN_GLOBAL(Ang16, word_6FDB34);
EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);
EXTERN_GLOBAL(Ped_List_4, gThreateningPedsList_678468);

//https://decomp.me/scratch/iQH9l 
MATCH_FUNC(0x544F70)
void __stdcall sub_544F70()
{
  dword_6FD7F8 = dword_6FD9E4;
  dword_6FD800 = dword_6FD9E4;
  dword_6FD7FC = dword_6FD9E4;
  dword_6FD7DC = dword_6FD9E4;
  dword_6FD7B0 = dword_6FD9E4;
  dword_6FD8B8 = dword_6FD9E4;
  dword_6FD8BC = dword_6FD9E4;
  dword_6FD8B4 = dword_6FD9E4;
  byte_6FDB51 = 0;
  byte_6FDB52 = 0;
  byte_6FDB53 = 0;
  byte_6FDB54 = 0;
  byte_6FDB55 = 0;
  dword_623F44 = 1;
  byte_6FDB56 = 0;
  byte_623F48 = 1;
  byte_6FDAD8 = dword_6FD9E4.ToUInt8();
  byte_6FDAD9 = dword_6FD9E4.ToUInt8();
  byte_6FDB57 = 0;
  byte_6FDB58 = 0;
}

// https://decomp.me/scratch/ZsDjc
STUB_FUNC(0x544ff0)
Char_B4::Char_B4()
{
    field_0_id = 0;
    field_4 = 0;
    field_5_remap = -1;
    field_6 = 0;
    field_8_ped_state_1 = 11;
    field_C_ped_state_2 = 28;
    field_10 = 36;
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
    field_38_velocity = dword_6FD7C0;
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
    field_68 = 0;
    field_69 = 0;
    field_58_flags_bf.b0 = 0;
    field_74 = word_6FDB34;
    field_6A = 0;
    field_84 = 0;
    field_88_obj_2c.sub_5A7010();
    field_8C = dword_6FD9E4;
    field_58_flags_bf.b2 = 0;
    field_6C = 18;
    field_70 = 0;
    field_71 = 0;
    field_90 = dword_6FD9E4;
    field_94 = dword_6FD9E4;
    field_58_flags_bf.b1 = 0;
    field_98.x = dword_6FD9E4;
    field_98.y = dword_6FD9E4;
    field_58_flags_bf.b3 = 0;
    field_58_flags_bf.b5 = 0;
    field_A4_xpos = dword_6FD9E4;
    field_A8_ypos = dword_6FD9E4;
    field_AC_zpos = dword_6FD9E4;
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
    field_10 = 36;
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
    field_38_velocity = dword_6FD7C0;
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
    field_68 = 0;
    field_69 = 0;
    field_74 = word_6FDB34;
    field_6A = 0;
    field_84 = 0;
    field_58_flags_bf.b0 = 0;
    field_88_obj_2c.sub_5A7010();
    field_8C = dword_6FD9E4;
    field_58_flags_bf.b2 = 0;
    field_6C = 18;
    field_70 = 0;
    field_71 = 0;
    field_90 = dword_6FD9E4;
    field_94 = dword_6FD9E4;
    field_58_flags_bf.b1 = 0;
    field_98.x = dword_6FD9E4;
    field_98.y = dword_6FD9E4;
    field_58_flags_bf.b5 = 0;
    field_58_flags_bf.b3 = 0;
    field_A4_xpos = dword_6FD9E4;
    field_A8_ypos = dword_6FD9E4;
    field_AC_zpos = dword_6FD9E4;
    field_58_flags_bf.b4 = 0;
    field_4C = dword_6FD9E4;
    field_50 = dword_6FD9E4;
    field_72 = dword_6FD9E4.ToInt();
    field_73 = dword_6FD9E4.ToInt();
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
        gPurpleDoom_1_679208->sub_477B60(field_80_sprite_ptr);
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

    if (field_C_ped_state_2 != 22 && field_10 != 15 && field_C_ped_state_2 != 27 && !field_7C_pPed->sub_433DA0())
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
    field_80_sprite_ptr->sub_5A3100(p2C->field_4, 0, 0, word_6FDB34);
    field_B0 = 10; // Start screaming timer
}

MATCH_FUNC(0x5454B0)
void Char_B4::sub_5454B0()
{
    field_B0 = -1;
    field_88_obj_2c.sub_5A7080();
}

MATCH_FUNC(0x5454d0)
void Char_B4::sub_5454D0()
{
    if (field_8_ped_state_1 != 8)
    {
        if (field_10 == 15 && field_6C == 5)
        {
            if (field_68 >= 5u)
            {
                field_68 = 5;
                field_71 = 2;
                field_70 = 0;
            }
        }
        else
        {
            field_10 = 15;
            field_6C = 5;
            field_68 = 0;
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
s32 Char_B4::IsOnScreen_545700()
{
    return gGame_0x40_67E008->sub_4B97E0(this->field_80_sprite_ptr, dword_6FD9E4) == 1;
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
                Char_B4::state_7_551CB0(); // Repulsion/Repel Ending?
                break;
            case 8:
                Char_B4::state_8_5520A0(); // Repulsion/Repel
                break;
            case 9:
                Char_B4::state_9_552E90(); // Dying?
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
            field_98.sub_41E210(-field_38_velocity, field_40_rotation);
        }
        else
        {
            // anti-clockwise?
            field_98.sub_41E210(field_38_velocity, field_40_rotation);
        }
    }
    gPurpleDoom_1_679208->sub_477B20(field_80_sprite_ptr);
    if (field_88_obj_2c.field_0_p18)
    {
        field_88_obj_2c.PoolUpdate_5A6F70(field_80_sprite_ptr);
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

MATCH_FUNC(0x5459e0)
void Char_B4::DrownPed_5459E0()
{
    field_7C_pPed->sub_45C500(8);
    field_7C_pPed->sub_45C540(20);
    field_16 = 1;

    gParticle_8_6FD5E8->EmitWaterSplash_53F060(field_80_sprite_ptr->field_14_xpos.x,
                                               field_80_sprite_ptr->field_14_xpos.y,
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

MATCH_FUNC(0x551B30)
void Char_B4::state_4_551B30()
{
    if (field_6C != 7)
    {
        field_6C = 7;
        field_68 = 0;
        s8 target_door = field_7C_pPed->field_24C_target_car_door;
        if (field_84->sub_43B540(target_door))
        {
            field_58_flags_bf.b4 = true;
        }
        else
        {
            field_58_flags_bf.b4 = false;
        }
        field_70 = 0;
    }
    if (field_10 == 15)
    {
        field_7C_pPed->sub_45C500(0);
        field_7C_pPed->sub_45C540(0);
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

STUB_FUNC(0x5532C0)
bool Char_B4::sub_5532C0()
{
    NOT_IMPLEMENTED;
    return false;
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
bool Char_B4::sub_553640(Object_2C* p2c)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/UYcej
STUB_FUNC(0x5537F0)
char_type Char_B4::sub_5537F0(Object_2C* p2c)
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
        return pPed->sub_45D000(p2c);
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

STUB_FUNC(0x46eb60)
void PedManager::sub_46EB60(u32* a2)
{
    NOT_IMPLEMENTED;
}

// https://decomp.me/scratch/dQf8H
STUB_FUNC(0x4703f0)
void PedManager::PedsService_4703F0()
{
    NOT_IMPLEMENTED;

    ++word_6787F0;
    word_6787E0 = 0;
    byte_6787E2 = 0;
    byte_6787E4 = 0;
    byte_6787E3 = 0;
    gNumPedsOnScreen_6787EC = 0;
    byte_61A8A1 = 1;
    byte_61A8A2 = 1;
    byte_6787D2 = 0;
    unk_6787EE = 0;

    gPedPool_6787B8->field_0_pool.UpdatePool();

    if (unk_6787EF) // 11d: je 128
    {
        byte_6787DA = 1;
    }
    else
    {
        byte_6787DA = 0;
        gThreateningPedsList_678468.ClearPeds_4712F0();
    }

    byte_6787D8 = byte_61A8A1 == 1;
    byte_6787D9 = byte_61A8A2 == 1;

    if (!bSkip_dummies_67D4EF)
    {
        Dummies_470330();
    }
    field_3 = word_6787E0;
    field_2 = byte_6787E2;
    field_4 = byte_6787E3;
    field_5_fbi_army_count = byte_6787E4;
    field_6_num_peds_on_screen = gNumPedsOnScreen_6787EC;
    if (gPolice_7B8_6FEE40)
    {
        gPolice_7B8_6FEE40->field_7AD_police_peds_in_range_screen = unk_6787EE;
    }
    unk_6787EF = 0;
    if (bDo_iain_test_67D4E9)
    {
        u16 num_peds = *(u32*)&gNumPedsOnScreen_6787EC; //  TODO: fix me
        swprintf(tmpBuff_67BD9C, L"num peds on screen : %d",
                 num_peds); // num peds on screen : %d
        gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 64, word_706600, 1);

        if (gPolice_7B8_6FEE40)
        {
            swprintf(tmpBuff_67BD9C,
                     L"num police peds in range screen : %d", // num police peds in range screen : %d
                     (u8)gPolice_7B8_6FEE40->field_7AD_police_peds_in_range_screen);
            gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 80, word_706600, 1);
        }
    }
}

// https://decomp.me/scratch/P1OvR
STUB_FUNC(0x470650)
PedManager::PedManager()
{
    field_8 = 0;
    if (!gPedPool_6787B8)
    {
        gPedPool_6787B8 = new PedPool();
        if (!gPedPool_6787B8)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15827); // OutOfMemoryNewOperator
        }
    }

    if (!gChar_B4_Pool_6FDB44)
    {
        gChar_B4_Pool_6FDB44 = new Char_B4_Pool();
        if (!gChar_B4_Pool_6FDB44)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15834); // OutOfMemoryNewOperator
        }
    }

    if (!gChar_8_Pool_678b50)
    {
        gChar_8_Pool_678b50 = new Char_8_Pool();
        if (!gChar_8_Pool_678b50)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\char.cpp", 15841); // OutOfMemoryNewOperator
        }
    }

    field_8 = gSprite_Pool_703818->get_new_sprite();

    field_2 = 0;
    field_3 = 0;
    field_4 = 0;
    field_6_num_peds_on_screen = 0;
    field_0 = 50;
    field_7_make_all_muggers = false;
    /*
    gPedId_61A89C = 7;
    dword_6787C0 = 0;
    word_6787C6 = 0;
    byte_6787C8 = 0;
    byte_6787C9 = 0;
    gNumberMuggersSpawned_6787CA = 0;
    gNumberCarThiefsSpawned_6787CB = 0;
    gNumberElvisLeadersSpawned_6787CC = 0;
    gNumberWalkingCopsSpawned_6787CD = 0;
    byte_6787CE = 0;
    word_6787D0 = 0;
    this->field_5_fbi_army_count = 0;
    HIWORD(dword_678654) = word_61A898;
    word_6787F0 = 0;
    byte_6787D2 = 0;
    byte_6787D3 = 0;
    HIWORD(dword_6784EE) = word_6787A8;
    byte_6787D4 = 0;
    byte_6787D5 = 0;
    byte_6787D6 = 0;
    byte_6787D7 = 0;
    byte_61A8A0 = 1;
    byte_61A8A1 = 1;
    byte_6787D8 = 0;
    byte_61A8A2 = 1;
    byte_6787D9 = 0;
    byte_6787DA = 0;
    dword_678750 = dword_678660;
    dword_6787DC = 0;
    word_678760 = word_6787A8;
    byte_61A8A3 = 1;
    byte_61A8A4 = 1;
    word_6787E0 = 0;
    byte_6787E2 = 0;
    byte_6787E3 = 0;
    gNumPedsOnScreen_6787EC = 0;
    unk_6787EF = 0;
    */
    sub_553F90();
    gThreateningPedsList_678468.ClearList_420E90();
}

STUB_FUNC(0x4709b0)
PedManager::~PedManager()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x470a50)
Ped* PedManager::SpawnPedAt(Fix16 xpos, Fix16 ypos, Fix16 zpos, u8 remap, Ang16 rotation)
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
Ped* PedManager::SpawnDriver_470B00(Car_BC* pCar)
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
Ped* PedManager::SpawnGangDriver_470BA0(Car_BC* pCar, Gang_144* pGang)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x470cc0)
Ped* PedManager::sub_470CC0(Car_BC* pCar)
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_244_remap = -1;
    pNewPed->field_26C_graphic_type = 0;
    pNewPed->field_238 = 3;
    pNewPed->field_240_occupation = 4; //unknown_2;
    pNewPed->field_16C_car = pCar;
    pNewPed->field_168_game_object = 0;
    pNewPed->sub_45C500(10);
    pNewPed->sub_45C540(10);
    pNewPed->field_24C_target_car_door = 0;
    pNewPed->field_288_threat_search = 2; //area_2;
    pNewPed->field_28C_threat_reaction = 3; //run_away_3;
    pNewPed->field_216_health = 100;
    pNewPed->field_26C_graphic_type = 0;
    return pNewPed;
}

STUB_FUNC(0x470d60)
Ped* PedManager::sub_470D60()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x470e30)
Ped* PedManager::sub_470E30()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x470f30)
Ped* PedManager::sub_470F30()
{
    Ped* pNewPed = gPedPool_6787B8->Allocate();
    pNewPed->field_216_health = 100;
    return pNewPed;
}

MATCH_FUNC(0x470f90)
Ped* PedManager::sub_470F90(Ped* pSrc)
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
void PedManager::DoIanTest_471060(s16 key)
{
    NOT_IMPLEMENTED;

    switch (key)
    {

        case DIK_9:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetOnFire();
            break;
        case DIK_8:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetVisible();
            break;

        case DIK_7:
            gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->SetInvisible();
            break;
    }
}

MATCH_FUNC(0x4710c0)
Ped* PedManager::PedById(s32 pedId)
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

STUB_FUNC(0x470330)
void PedManager::Dummies_470330()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x471110)
PedPool::~PedPool()
{
}
