#include "char.hpp"
#include "Char_Pool.hpp"
#include "Car_BC.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "Particle_8.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "rng.hpp"
#include "Varrok_7F8.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "sprite.hpp"

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
WIP_FUNC(0x544ff0)
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
        gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_80_sprite_ptr);
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
    field_80_sprite_ptr->DispatchCollisionEvent_5A3100(p2C->field_4, 0, 0, word_6FDB34);
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
bool Char_B4::IsOnScreen_545700()
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
    dword_6FD7F8 = field_80_sprite_ptr->field_14_xy.x;
    dword_6FD800 = field_80_sprite_ptr->field_14_xy.y;
    dword_6FD7FC = field_80_sprite_ptr->field_1C_zpos;
    dword_6FD7B0 = Fix16(field_45);
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

        field_80_sprite_ptr->set_ang_lazy_420690(field_40_rotation);

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
void Char_B4::sub_5459C0()
{
    byte_6FDB59 = 0;
    gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_80_sprite_ptr);
}

MATCH_FUNC(0x5459e0)
void Char_B4::DrownPed_5459E0()
{
    field_7C_pPed->sub_45C500(8);
    field_7C_pPed->sub_45C540(20);
    field_16 = 1;

    gParticle_8_6FD5E8->EmitWaterSplash_53F060(field_80_sprite_ptr->field_14_xy.x,
                                               field_80_sprite_ptr->field_14_xy.y,
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

STUB_FUNC(0x553330)
char_type Char_B4::IsThreatToSearchingPed_553330()
{
    NOT_IMPLEMENTED;
    return field_7C_pPed->IsThreatToSearchingPed_4661F0();
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
bool Char_B4::OnObjectTouched_553640(Object_2C* p2c)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/UYcej
STUB_FUNC(0x5537F0)
char_type Char_B4::HandlePedObjectHit_5537F0(Object_2C* p2c)
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
        return pPed->HandlePedHitByObject_45D000(p2c);
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
