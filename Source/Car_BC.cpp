#include "Car_BC.hpp"
#include "CarInfo_808.hpp"
#include "Car_B0.hpp"
#include "Fix16_Rect.hpp"
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
#include "Taxi_4.hpp"
#include "Weapon_8.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "text_0x14.hpp"

DEFINE_GLOBAL(Car_214*, gCar_214_705F20, 0x705F20);
DEFINE_GLOBAL(Car_6C*, gCar_6C_677930, 0x677930);
DEFINE_GLOBAL(Car_E0C4*, gCar_E0C4_67792C, 0x67792C);
DEFINE_GLOBAL(Car_8F74*, gCar_8F74_677CF8, 0x677CF8);
DEFINE_GLOBAL(Car_A4*, gCar_A4_66AC80, 0x66AC80);
DEFINE_GLOBAL(Car_14*, gCar_14_677934, 0x677934);
DEFINE_GLOBAL(s32, dword_6772AC, 0x6772AC);

// This is not used outside this file.
// In fact, it's only allocated and deallocated, it's never used.
DEFINE_GLOBAL(Sprite*, gSprite_Unused_677938, 0x677938);
DEFINE_GLOBAL(Fix16, gFix16_6777CC, 0x6777CC);
DEFINE_GLOBAL(CarInfo_2C*, gCarInfo_2C_66AB78, 0x66AB78);
DEFINE_GLOBAL(CarInfo_48*, gCarInfo_48_66AB70, 0x66AB70);
DEFINE_GLOBAL(s16, DAT_677CFC, 0x677CFC);
DEFINE_GLOBAL(struct_4, stru_67737C, 0x67737c);

DEFINE_GLOBAL(Fix16, dword_6771F0, 0x6771F0);
DEFINE_GLOBAL(Ang16, word_677326, 0x677326);
DEFINE_GLOBAL(Fix16, unk_6772A4, 0x6772A4);

// Indicates if Car_2 is initialised
// It can probably turned into a static variable inside Car_2
DEFINE_GLOBAL(char_type, byte_679C0A, 0x679C0A);

// Array of values used by Car_2.
// It can probably turned into a static variable inside Car_2
DEFINE_GLOBAL_ARRAY(s16, DAT_00679320, 1000, 0x679320);
DEFINE_GLOBAL(Fix16, dword_6777D0, 0x6777D0);
DEFINE_GLOBAL(s32, dword_677888, 0x677888);
DEFINE_GLOBAL(s32, dword_6778D0, 0x6778D0);
DEFINE_GLOBAL(Fix16, DAT_006FF744, 0x006FF744); //, 0x147, 0xUNKNOWN);
DEFINE_GLOBAL(Fix16, DAT_006FF774, 0x006FF774); //, 0x0, 0xUNKNOWN);
DEFINE_GLOBAL(Fix16, DAT_006FF570, 0x6FF570);
DEFINE_GLOBAL(Fix16, DAT_006FF7E8, 0x6FF7E8);
DEFINE_GLOBAL(s8, DAT_006FF8C4, 0x6FF8C4);
DEFINE_GLOBAL(s8, DAT_006FF8C5, 0x6FF8C5);
DEFINE_GLOBAL(Fix16, DAT_006FF778, 0x6ff778);
DEFINE_GLOBAL(Fix16, dword_6F7690, 0x6F7690);
DEFINE_GLOBAL(Fix16, dword_6F77D4, 0x6F77D4);
DEFINE_GLOBAL(Ang16, dword_6F804C, 0x6F804C);
DEFINE_GLOBAL(Ang16, word_6F771E, 0x6F771E);
DEFINE_GLOBAL(Ang16, word_67791C, 0x67791C);

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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x447710)
char_type Car_78::sub_447710()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x447970)
void Car_78::sub_447970()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x447ca0)
char_type Car_78::sub_447CA0(u8 a2, u8 a3, u8 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x447d40)
char_type Car_78::sub_447D40(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448270)
char_type Car_78::sub_448270()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4482c0)
char_type Car_78::sub_4482C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448770)
char_type Car_78::sub_448770()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448ce0)
char_type Car_78::sub_448CE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44a1f0)
char_type Car_78::sub_44A1F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44af00)
s16 Car_78::sub_44AF00()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44d1d0)
char_type Car_78::sub_44D1D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44e0c0)
void Car_78::sub_44E0C0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x44e560)
void Car_78::sub_44E560()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451980)
void Car_78::sub_451980()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451fa0)
void Car_78::sub_451FA0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451ff0)
s32 Car_78::sub_451FF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452060)
char_type Car_78::sub_452060()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452a20)
s16 Car_78::sub_452A20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452df0)
s16 Car_78::sub_452DF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453470)
s16 Car_78::sub_453470()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453590)
s32* Car_78::sub_453590(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4537d0)
char_type Car_78::sub_4537D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4538b0)
Car_BC* Car_78::sub_4538B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453990)
s32 Car_78::sub_453990(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4539b0)
Car_BC* Car_78::sub_4539B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4539d0)
u8* Car_78::sub_4539D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453a40)
s32 Car_78::sub_453A40()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453bb0)
s16 Car_78::sub_453BB0()
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453d80)
s32 Car_78::sub_453D80()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453cb0)
Car_78::Car_78()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5A4D90)
void Sprite_4C::SetCurrentRect_5A4D90()
{
    field_30.DoSetCurrentRect_59DD60();
}

STUB_FUNC(0x5A3550)
void Sprite_4C::sub_5A3550(Fix16 x, Fix16 y, Fix16 z, Ang16 ang)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5c8680)
void Car_214::sub_5C8680(u8 idx)
{
    field_0[idx].field_8 = 0;
    field_0[idx].field_C = 0;
    field_0[idx].field_0 = 0;
    field_0[idx].field_14 = 2;
    field_0[idx].field_4_O2C->Dealloc_5291B0();
    field_0[idx].field_4_O2C = 0;
}

STUB_FUNC(0x5c86c0)
char_type Car_214::sub_5C86C0(s32* a2, u32* a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x444ab0)
u32 Car_6C::sub_444AB0(s32 a2, u16* a3, s32 a4, u16* a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x444cf0)
Car_BC* Car_6C::sub_444CF0(s32 a1, s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x444f80)
Car_BC* Car_6C::sub_444F80(s32 a1, s32 a2, s32 a3, Ped* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x444fa0)
Car_BC* Car_6C::sub_444FA0(Fix16 x, Fix16 y, Fix16 z, Ped* pPed)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x445210)
Car_BC* Car_6C::sub_445210(Sprite *a1, u8 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4458b0)
Car_BC* Car_6C::sub_4458B0(s32 arg0, s32 a3, s32 a4, s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x446230)
Car_BC* Car_6C::sub_446230(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, Fix16 maybe_w_scale)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x446530)
Trailer* Car_6C::sub_446530(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_idx, s32 trailer_idx)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x446730)
void Car_6C::sub_446730(Car_BC* pCar)
{
    gPurpleDoom_3_679210->Remove_477B00(pCar->field_50_car_sprite);
    gCar_E0C4_67792C->Remove(pCar);
}

MATCH_FUNC(0x4466c0)
void Car_6C::sub_4466C0(s32 car_kind)
{
    switch (car_kind)
    {
        case 1:
            --this->field_28_recycled_cars;
            break;

        case 2:
            --this->field_40_proto_recycled_cars;
            break;

        case 10:
            --this->field_48;
            break;

        case 4:
            --this->field_2C;
            break;

        case 5:
            --this->field_30;
            break;

        case 6:
            --this->field_34_unit_cars;
            break;

        case 7:
            --this->field_38;
            break;

        case 8:
            --this->field_3C_mission_cars;
            break;
        case 9:
            --this->field_44;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x446760)
void Car_6C::sub_446760()
{
    // Enable free shopping once all KF's are passed
    u32* pSecretsPassed = gfrosty_pasteur_6F8060->field_338_secrets_passed;
    if (pSecretsPassed)
    {
        const u32 total_secrets = gfrosty_pasteur_6F8060->field_318_total_secrets;
        if (total_secrets > 0)
        {
            if (!this->field_69_do_free_shopping && *pSecretsPassed >= total_secrets)
            {
                this->field_69_do_free_shopping = 1;
            }
        }
    }
}

STUB_FUNC(0x446790)
void Car_6C::sub_446790()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x446870)
bool Car_6C::sub_446870(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x446930)
u32 Car_6C::sub_446930(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4469f0)
// There are still something missing here.
// But the structure it seems to be complete
Car_6C::Car_6C()
{
    NOT_IMPLEMENTED;
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

    stru_67727C.sub_4207E0();
    stru_67737C.sub_4207E0();

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
    //field_4C = 0;
    //field_50_tv_van_dir = 0;
    field_4C_tv_van_dir = Fix16_Point(Fix16(0), Fix16(0));
    field_54 = 0;
    field_55 = 0;
    field_58 = 0x57;
    field_5C = 0;
    field_C = 87;
    field_69_do_free_shopping = bDo_free_shopping_67D6CD;
}

// Some SEH stuff, prob needs some dtors moving to or from another file
STUB_FUNC(0x446dc0)
Car_6C::~Car_6C()
{
    NOT_IMPLEMENTED;

    if (gCar_E0C4_67792C)
    {
        GTA2_DELETE_AND_NULL(gCar_E0C4_67792C);
    }

    if (gCar_D264_6FE3E0)
    {
        GTA2_DELETE_AND_NULL(gCar_D264_6FE3E0);
    }

    if (gCar_14_677934)
    {
        GTA2_DELETE_AND_NULL(gCar_14_677934);
    }

    if (gCar_8F74_677CF8)
    {
        GTA2_DELETE_AND_NULL(gCar_8F74_677CF8);
    }

    if (gCar_A4_66AC80)
    {
        GTA2_DELETE_AND_NULL(gCar_A4_66AC80);
    }

    if (gSprite_Unused_677938)
    {
        GTA2_DELETE_AND_NULL(gSprite_Unused_677938);
    }

    field_4 = 0;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x439f30)
u32* Car_BC::sub_439F30(u32* a2)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43a0e0)
u32* Car_BC::sub_43A0E0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43a120)
u32* Car_BC::sub_43A120(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43a1d0)
Fix16 Car_BC::get_anti_strngth_43A1D0()
{
    return gCarInfo_808_678098->sub_4546B0(field_84_car_info_idx)->field_2C_anti_strngth;
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
Fix16 Car_BC::sub_43A240()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43a3c0)
bool Car_BC::sub_43A3C0()
{
    if (!field_58_physics)
    {
        return false;
    }
    return field_58_physics->field_98_surface_type == 6; // 6 = air surface (or no surface)
}

STUB_FUNC(0x43a3e0)
s16* Car_BC::sub_43A3E0(s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43a450)
u32* Car_BC::sub_43A450(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43a4c0)
Fix16 Car_BC::sub_43A4C0()
{
    if (!field_58_physics)
    {
        return 0;
    }
    else
    {
        return field_58_physics->field_0_vel_read_only.GetLength();
    }
}

STUB_FUNC(0x43a590)
u32* Car_BC::sub_43A590(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43a5b0)
u32* Car_BC::sub_43A5B0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43a600)
void Car_BC::RemoveAllDamage()
{
    sub_43D400();
    //Trailer* v2 = field_64_pTrailer;
    if (field_64_pTrailer)
    {
        field_64_pTrailer->field_C_trailer_carObj->sub_43D400();
    }
}

STUB_FUNC(0x43a680)
bool Car_BC::AllowResprayOrPlates()
{
    NOT_IMPLEMENTED;
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

MATCH_FUNC(0x43a6f0)
bool Car_BC::IsNotCurrentRemap(u8 remap)
{
    return gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->has_remaps() && field_50_car_sprite->field_24_remap != remap;
}

MATCH_FUNC(0x43a730)
bool Car_BC::IsNotCurrentRemapOfCarAndTrailerCar(u8 remap)
{
    if (field_64_pTrailer)
    {
        // Check trailer car
        return IsNotCurrentRemap(remap) || field_64_pTrailer->field_C_trailer_carObj->IsNotCurrentRemap(remap);
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
        if (field_64_pTrailer->field_C_trailer_carObj->IsNotCurrentRemap(remap))
        {
            field_64_pTrailer->field_C_trailer_carObj->field_50_car_sprite->SetRemap(remap);
        }
    }
}

STUB_FUNC(0x43a7d0)
void Car_BC::sub_43A7D0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43a850)
char_type Car_BC::sub_43A850()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43a950)
void Car_BC::sub_43A950()
{
    Car_B0* pB0 = field_58_physics;
    pB0->field_91_is_foot_brake_on = 1;
    pB0->field_93_is_forward_gas_on = 0;
    pB0->field_94_is_backward_gas_on = 0;
    pB0->field_95 = 0;
}

MATCH_FUNC(0x43a970)
void Car_BC::sub_43A970()
{
    field_58_physics->field_92_is_hand_brake_on = 1;
    Car_B0* pB0 = field_58_physics;
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
        Car_B0* pB0 = this->field_58_physics;
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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43aa60)
Car_BC* Car_BC::sub_43AA60()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43aaf0)
char_type Car_BC::sub_43AAF0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43adc0)
void Car_BC::sub_43ADC0(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43af10)
bool Car_BC::sub_43AF10()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43af40)
char_type Car_BC::sub_43AF40()
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43b140)
bool Car_BC::sub_43B140(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43b2b0)
bool Car_BC::sub_43B2B0(Ped* a2)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43b3d0)
char_type Car_BC::sub_43B3D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43b420)
s32 Car_BC::sub_43B420(s32 a2, u32* a3, u32* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43b5a0)
s32* Car_BC::sub_43B5A0(s32 a2, u32* a3, s32* a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43b730)
char_type Car_BC::sub_43B730()
{
    NOT_IMPLEMENTED;
    return gGame_0x40_67E008->sub_4B97E0(field_50_car_sprite, dword_6778D0);
}

STUB_FUNC(0x43b750)
char_type Car_BC::sub_43B750()
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43b850)
bool Car_BC::sub_43B850(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43b870)
void Car_BC::sub_43B870(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43bb90)
s32 Car_BC::sub_43BB90(char_type a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43bbc0)
char_type Car_BC::sub_43BBC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x43bc30)
void Car_BC::sub_43BC30()
{
    if (!field_58_physics)
    {
        sub_4419E0();
        field_58_physics->sub_5638C0(this);
        field_58_physics->sub_563560(this->field_50_car_sprite);
    }
    else
    {
        if (field_84_car_info_idx == 59 || field_84_car_info_idx == 60 || field_84_car_info_idx == 61 || field_84_car_info_idx == 6)
        {
            field_58_physics->sub_563560(this->field_50_car_sprite);
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43bf10)
void Car_BC::sub_43BF10()
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c0c0)
char_type Car_BC::sub_43C0C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c1c0)
u32 Car_BC::sub_43C1C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c260)
car_info* Car_BC::sub_43C260()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c310)
car_info* Car_BC::sub_43C310()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c3c0)
car_info* Car_BC::sub_43C3C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c470)
void Car_BC::sub_43C470()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43c500)
s32 Car_BC::sub_43C500()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c650)
s32 Car_BC::sub_43C650()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c700)
s32 Car_BC::sub_43C700()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c840)
void Car_BC::sub_43C840()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43c920)
s32 Car_BC::ActivateEmergencyLights_43C920()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43c9d0)
s16 Car_BC::DeactivateEmergencyLights_43C9D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43ca80)
s16 Car_BC::sub_43CA80()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43cbe0)
s32 Car_BC::sub_43CBE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43cdf0)
Car_BC* Car_BC::sub_43CDF0(char_type a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43cf30)
void Car_BC::DamageArea_43CF30(s32 damage_area)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43d1c0)
bool Car_BC::IsAreaDamaged_43D1C0(s32 damage_area)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43d400)
s32 Car_BC::sub_43D400()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43d690)
void Car_BC::sub_43D690(s32 a2, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43d7b0)
void Car_BC::sub_43D7B0(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43d840)
void Car_BC::sub_43D840(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43da90)
s16 Car_BC::sub_43DA90(s16 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43db80)
Char_8** Car_BC::sub_43DB80()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43dbd0)
void Car_BC::sub_43DBD0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43dc00)
bool Car_BC::sub_43DC00()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43dc80)
bool Car_BC::sub_43DC80(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43dd50)
char_type Car_BC::sub_43DD50()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43dd60)
void Car_BC::sub_43DD60()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x43e560)
char_type Car_BC::sub_43E560()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43e8d0)
Car_BC* Car_BC::sub_43E8D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43e990)
Ped* Car_BC::sub_43E990()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43ea60)
char_type Car_BC::sub_43EA60(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x43f130)
char_type Car_BC::sub_43F130(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x440510)
u32* Car_BC::sub_440510(u32* a2)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4407f0)
void Car_BC::sub_4407F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x440840)
Sprite* Car_BC::sub_440840()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x440ac0)
void Car_BC::sub_440AC0()
{
    Object_2C* p2C =
        gObject_5C_6F8F84->sub_5299B0(objects::moving_collect_18_114, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, word_67791C);
    field_50_car_sprite->sub_5A3100(p2C->field_4, gFix16_6777CC, dword_6771F0, word_677326);
}

MATCH_FUNC(0x440b10)
void Car_BC::sub_440B10()
{
    Object_2C* p2C = gObject_5C_6F8F84->sub_5299B0(objects::small_brown_skid_148, gFix16_6777CC, gFix16_6777CC, gFix16_6777CC, word_67791C);
    field_50_car_sprite->sub_5A3100(p2C->field_4, gFix16_6777CC, unk_6772A4, word_67791C);
}

STUB_FUNC(0x440b60)
s32 Car_BC::sub_440B60()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x440bb0)
s32 Car_BC::sub_440BB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x440c10)
char_type Car_BC::sub_440C10(char_type a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x440d90)
char_type Car_BC::sub_440D90(char_type a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x440f90)
void Car_BC::sub_440F90(char_type a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x441030)
char_type Car_BC::sub_441030(u8 a2, u8 a3, u8 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x441080)
char_type Car_BC::sub_441080(u8 a2, u8 a3, u8 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4410d0)
char_type Car_BC::sub_4410D0(s16 a2, u8* a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x441360)
void Car_BC::sub_441360()
{
    if (field_A9)
    {
        field_A9--;
        if (field_A9 == 0)
        {
            sub_4436A0(); // jmp to function chunk
        }
    }
}

STUB_FUNC(0x4436A0)
void Car_BC::sub_4436A0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x441380)
void Car_BC::sub_441380()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4413b0)
char_type Car_BC::sub_4413B0(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x441520)
void Car_BC::sub_441520()
{
    Car_B0* pB0;
    switch (this->field_9C)
    {
        case 1:
            pB0 = this->field_58_physics;
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
    Car_B0* pB0 = this->field_58_physics;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4416d0)
char_type Car_BC::sub_4416D0(s32 a2)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4419e0)
void Car_BC::sub_4419E0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x441a10)
Car_B0* Car_BC::sub_441A10()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x441a40)
char_type Car_BC::sub_441A40()
{
    NOT_IMPLEMENTED;
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

MATCH_FUNC(0x441a70)
void Car_BC::sub_441A70()
{
    const u8* pRemapCount = gGtx_0x106C_703DD4->get_car_remap_5AA3D0(field_84_car_info_idx);
    u8 i;
    for (i = 0; i < *pRemapCount; i++)
    {
        field_C_doors[i].sub_439E40(i);
        field_C_doors[i].field_8_pObj = 0;
    }

    for (i = *pRemapCount; i < 4; i++)
    {
        field_C_doors[i].field_4_state = 0;
    }
}

MATCH_FUNC(0x441b00)
void Car_BC::sub_441B00()
{
    Car_Door_10* p = field_C_doors;
    for (s32 i = 0; i < 4; i++)
    {
        p->sub_439DA0(&field_8_damaged_areas.m_var);
        p++;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x441c00)
void Car_BC::sub_441C00()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x441d40)
s16 Car_BC::sub_441D40()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x441e70)
void Car_BC::sub_441E70()
{
    if ((field_A4 & 4) != 0)
    {
        Car_BC::sub_441B50();
    }
    else if ((field_A4 & 8) != 0)
    {
        field_A5--;
        if (field_A5 == 0)
        {
            field_8E--;
            if (field_8E == 0)
            {
                Car_BC::sub_43CBE0();
            }
            else
            {
                sub_425590();
                sub_4213D0();
                field_A5 = 12;
            }
        }
        else
        {
            if (field_A5 == 6)
            {
                sub_425650();
                sub_421430();
            }
        }
    }
    else if ((field_A4 & 0x10) != 0)
    {
        field_A5--;
        if (field_A5 == 0)
        {
            if ((field_A4 & 2) != 0)
            {
                sub_425590();
            }
            else
            {
                Car_BC::sub_447360();
            }
            field_A4 &= ~0x10u;
        }
    }

    if (sub_421700())
    {
        Car_BC::sub_441C00();
    }
    if (sub_4216C0())
    {
        Car_BC::sub_441B20();
    }
    if (field_84_car_info_idx == car_model_enum::EDSELFBI)
    {
        Car_BC::sub_441D40();
    }
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
    if (field_58_physics->sub_562FE0())
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x442310)
void Car_BC::sub_442310()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4424c0)
char_type Car_BC::sub_4424C0()
{
    switch (this->field_88)
    {
        case 3:
            this->field_88 = 5;
            return 0;

        case 4:
            this->field_88 = 2;
            return 0;

        case 2:
            if (!sub_442170())
            {
                return 0;
            }
            this->field_88 = 5;
            return 0;

        case 5:
            this->field_88 = 6;
            return 1;

        default:
            return 0;
    }
}

STUB_FUNC(0x442520)
s16* Car_BC::sub_442520(s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4425d0)
void Car_BC::sub_4425D0()
{
    NOT_IMPLEMENTED;
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
Trailer* Car_BC::sub_442760()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4427a0)
Car_BC* Car_BC::sub_4427A0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x442810)
s32 Car_BC::sub_442810()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x442d10)
s32* Car_BC::sub_442D10()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x442d70)
char_type Car_BC::sub_442D70()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x443130)
char_type Car_BC::sub_443130()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x443170)
char_type Car_BC::sub_443170()
{
    if (!this->field_76)
    {
        gCar_6C_677930->field_55++;
    }

    sub_444020();

    if (this->field_64_pTrailer)
    {
        return sub_443130();
    }

    // IsTrain inline ?
    if (is_train_model())
    {
        return sub_442D70();
    }

    if (this->field_88 != 5)
    {
        gPurpleDoom_1_679208->sub_477B60(this->field_50_car_sprite);
    }

    if (sub_4424C0())
    {
        return 1;
    }

    if (this->field_58_physics)
    {
        Car_78* v4 = this->field_5C;
        if (v4)
        {
            Ped* pDriver = this->field_54_driver;
            if (pDriver)
            {
                if (pDriver->sub_420B70() == 2 && this->field_84_car_info_idx == car_model_enum::TRAINCAB || pDriver->sub_420B70() != 2)
                {
                    v4->sub_453BB0();
                }
            }
            field_5C->field_68 = 0;
            field_5C->field_24_flags &= ~0x1000u;
        }
        sub_442190();
    }

    if ((this->field_78_flags & 0x2000) != 0)
    {
        stru_67737C.sub_5A72B0(this->field_50_car_sprite, 1);
        stru_67737C.sub_5A6E10();
    }

    if (this->field_0_qq.field_0_p18)
    {
        this->field_0_qq.sub_5A6F70(this->field_50_car_sprite);
        this->field_0_qq.sub_5A72B0(this->field_50_car_sprite, 0);
    }

    if (this->field_58_physics)
    {
        sub_43E560();
    }

    if (!this->field_64_pTrailer && (gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->info_flags & 8) == 8)
    {
        sub_442810();
    }

    sub_4426D0();
    sub_442310();
    sub_441360();

    if (this->field_88 != 5)
    {
        if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->info_flags & 0x20) == 0x20)
        {
            if (this->field_54_driver)
            {
                if (this->field_9C == 3 && !this->field_4.field_0_pOwner && this->field_8C < 3u)
                {
                    gTaxi_4_704130->PushTaxi_457BA0(this);
                }
            }
        }
        gPurpleDoom_1_679208->sub_477B20(this->field_50_car_sprite);
    }
    return 0;
}

STUB_FUNC(0x443330)
char_type Car_BC::sub_443330(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x443360)
char_type Car_BC::sub_443360(s32 a2, s32 a3, s32 a4, s16 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4435a0)
void Car_BC::sub_4435A0()
{
    sub_441A70();
}

MATCH_FUNC(0x4435b0)
s32 Car_BC::sub_4435B0()
{
    if ((field_78_flags & 0x10) != 0)
    {
        return 14;
    }

    if ((field_78_flags & 0x40) != 0)
    {
        return 20;
    }

    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & 1) != 1)
    {
        return 17;
    }
    return 22;
}

MATCH_FUNC(0x4435f0)
void Car_BC::sub_4435F0()
{
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 0x10) == 0x10)
    {
        this->field_50_car_sprite->field_28_num = 16;
    }
    else
    {
        if ((field_78_flags & 0x10) != 0)
        {
            this->field_50_car_sprite->field_28_num = 13;
        }
        else if ((field_78_flags & 0x40) != 0)
        {
            this->field_50_car_sprite->field_28_num = 19;
        }
        else if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & 1) == 1)
        {
            this->field_50_car_sprite->field_28_num = 21;
        }
        else
        {
            this->field_50_car_sprite->field_28_num = 15;
        }
    }
}

STUB_FUNC(0x443710)
Car_6C* Car_BC::sub_443710(s32 a2)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
    Player* pPlayer = this->field_54_driver->field_15C_player;
    s32 cost = gCar_6C_677930->field_69_do_free_shopping != 0 ? 0 : 5000;
    if (cost <= pPlayer->field_2D4_unk.GetScore_592370())
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
            field_0_qq.sub_5A7110();
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
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x443d00)
void Car_BC::sub_443D00(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    gPurpleDoom_1_679208->sub_477B60(field_50_car_sprite);
    Sprite* pCarSprite = field_50_car_sprite;
    if (pCarSprite->field_14_xpos.x != xpos || pCarSprite->field_14_xpos.y != ypos || pCarSprite->field_1C_zpos != zpos)
    {
        pCarSprite->field_14_xpos.x = xpos;
        pCarSprite->field_14_xpos.y = ypos;
        pCarSprite->field_1C_zpos = zpos;
        pCarSprite->sub_59E7B0();
    }
    Car_B0* field_58_uni = field_58_physics;
    if (field_58_uni)
    {
        field_58_uni->sub_563560(field_50_car_sprite);
    }
    gPurpleDoom_1_679208->sub_477B20(field_50_car_sprite);
}

MATCH_FUNC(0x443d70)
void Car_BC::IncrementCarStats_443D70(s32 a2)
{
    IncrementAllocatedCarType_443DA0(a2);

    if (field_64_pTrailer)
    {
        if (field_64_pTrailer->field_8_truck_cab == this)
        {
            field_64_pTrailer->field_C_trailer_carObj->IncrementAllocatedCarType_443DA0(a2);
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

MATCH_FUNC(0x443f30)
void Car_BC::sub_443F30(s32 object_type, s32 argb, s32 a4, s32 a5)
{
    Object_2C* pObj = gObject_5C_6F8F84->sub_529AB0(object_type, 0, 0, 0, argb, dword_6772AC, 200);
    pObj->sub_527990();
    field_50_car_sprite->sub_5A3100(pObj->field_4, Fix16(a4 * dword_677888, 0), Fix16(a5 * dword_677888, 0), word_67791C);
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
    NOT_IMPLEMENTED;
    return 0;
}

/*
OBS: it is not a switch case :)
*/
MATCH_FUNC(0x4441b0)
void Car_BC::sub_4441B0()
{
    // car sirens?
    s32 idx = this->field_84_car_info_idx;
    if (idx == car_model_enum::MEDICAR)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 10, 16);
        Car_BC::sub_443F30(171, 0xFF2010, -10, 16);
        Car_BC::sub_443F30(172, 0xFF2010, 10, -32);
        Car_BC::sub_443F30(173, 0xFF2010, -10, -32);
    }
    else if (idx == car_model_enum::COPCAR)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 10, -16);
        Car_BC::sub_443F30(171, 255, -10, -16);
    }
    else if (idx == car_model_enum::SWATVAN)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 10, 32);
        Car_BC::sub_443F30(171, 255, -10, 32);
        Car_BC::sub_443F30(172, 0xFF2010, 10, -32);
        Car_BC::sub_443F30(173, 255, -10, -32);
    }
    else if (idx == car_model_enum::FIRETRUK)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 16, 48);
        Car_BC::sub_443F30(171, 0xFF2010, -16, 48);
        Car_BC::sub_443F30(172, 0xFF2010, 16, -16);
        Car_BC::sub_443F30(173, 0xFF2010, -16, -26);
    }
    else if (idx == car_model_enum::EDSELFBI)
    {
        Car_BC::sub_443F30(165, 0xFF2010, 0, 16);
    }
}

MATCH_FUNC(0x444490)
void Car_BC::sub_444490()
{
    this->field_6C_maybe_id = gCar_6C_677930->field_14++;
    this->field_74_damage = 0;
    this->field_8C = 0;
    this->field_8_damaged_areas = 0;
    this->field_4.field_0_pOwner = 0;
    this->field_54_driver = 0;
    this->field_98 = 3;
    this->field_58_physics = 0;
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

    this->field_0_qq.sub_5A7010();

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
            gRouteFinder_6FFDC8->CancelRoute_589930(field_5C->field_28_junc_idx);
        }

        gCar_8F74_677CF8->Remove(field_5C);
        field_5C = 0;
    }
}

MATCH_FUNC(0x4447d0)
void Car_BC::sub_4447D0()
{
    this->field_0_qq.sub_5A7010();

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
    field_58_physics = 0;
    field_5C = 0;
    field_88 = 0;
    field_6C_maybe_id = 0xFFFF;
    field_64_pTrailer = 0;
    field_78_flags = 0;
    this->field_0_qq.sub_5A7010();
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

MATCH_FUNC(0x407b90)
Car_BC* Trailer::sub_407B90(Car_BC* a2)
{
    if (a2 == field_8_truck_cab)
    {
        return field_C_trailer_carObj;
    }
    return field_8_truck_cab;
}

MATCH_FUNC(0x407bb0)
void Trailer::sub_407BB0(Car_BC* a2, Car_BC* a3)
{
    this->field_8_truck_cab = a2;
    this->field_C_trailer_carObj = a3;
    a2->field_64_pTrailer = this;
    this->field_C_trailer_carObj->field_64_pTrailer = this;
    this->field_0 = 0;
}

STUB_FUNC(0x407bd0)
s32* Trailer::sub_407BD0(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x407ce0)
s32* Trailer::sub_407CE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x408140)
char_type Trailer::sub_408140()
{
    if (!field_8_truck_cab->field_58_physics && !field_C_trailer_carObj->field_58_physics)
    {
        return 0;
    }

    sub_408190();

    if (!field_8_truck_cab->field_58_physics->sub_562FE0() || field_8_truck_cab->field_54_driver)
    {
        return field_8_truck_cab->sub_43E560();
    }
    sub_4081B0();
    return 0;
}

MATCH_FUNC(0x408190)
void Trailer::sub_408190()
{
    field_8_truck_cab->sub_43BC30();
    field_C_trailer_carObj->sub_43BC30();
}

MATCH_FUNC(0x4081b0)
void Trailer::sub_4081B0()
{
    field_8_truck_cab->sub_441A10();
    field_C_trailer_carObj->sub_441A10();
}

MATCH_FUNC(0x4081d0)
char_type Trailer::sub_4081D0()
{
    if (field_8_truck_cab->field_74_damage == 32001)
    {
        if (field_C_trailer_carObj->field_74_damage != 32001)
        {
            field_C_trailer_carObj->field_74_damage = 32000;
            field_C_trailer_carObj->sub_43D840(18);
        }
        return 1;
    }
    else if (field_C_trailer_carObj->field_74_damage == 32001)
    {
        field_8_truck_cab->field_74_damage = 32000;
        field_8_truck_cab->sub_43D840(18);
        return 1;
    }
    else
    {
        return 0;
    }
}

STUB_FUNC(0x408220)
s32 Trailer::sub_408220()
{
    NOT_IMPLEMENTED;
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
    field_0_cam = NULL;
    field_C_player = NULL;
    field_10_zone = NULL;
    field_4_rect = new Fix16_Rect();
    sub_5822E0();
}

MATCH_FUNC(0x582340)
Car_14::~Car_14()
{
    delete field_4_rect;
    field_4_rect = NULL;
    field_0_cam = NULL;
    field_C_player = NULL;
    field_10_zone = NULL;
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
                if (param_2 < (field_0_cam->field_78_boundaries_non_neg.field_4_right - DAT_006FF778))
                {
                    return 1;
                }
            }
            else
            {
                if (param_2 > (field_0_cam->field_78_boundaries_non_neg.field_0_left + DAT_006FF778))
                {
                    return 1;
                }
            }
            break;
        case 3:
        case 4:
            if (field_8 == 0)
            {
                if (param_3 < (field_0_cam->field_78_boundaries_non_neg.field_C_bottom - DAT_006FF778))
                {
                    return 1;
                }
            }
            else
            {
                if (param_3 > (field_0_cam->field_78_boundaries_non_neg.field_8_top + DAT_006FF778))
                {
                    return 1;
                }
            }
            break;
    }
    return 0;
}

STUB_FUNC(0x5832C0)
void Car_14::MakeTrafficForCurrCamera_5832C0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x583670)
void Car_14::GenerateTraffic_583670()
{
    if (!bSkip_recycling_67D575)
    {
        field_0_cam = gGame_0x40_67E008->IteratePlayerCamera_4B9BC0();
        field_C_player = gGame_0x40_67E008->sub_4B9CD0();
        if (field_C_player)
        {
            field_10_zone = gMap_0x370_6F6268->get_nav_zone_unknown_4DF890(field_C_player->field_2C4_player_ped->get_cam_x().ToInt(),
                                                                           field_C_player->field_2C4_player_ped->field_1AC_cam.y.ToInt());
        }

        for (; field_0_cam;)
        {
            if (field_0_cam->has_camera_car_or_ped_433E90())
            {
                Car_14::MakeTrafficForCurrCamera_5832C0();
            }
            field_0_cam = gGame_0x40_67E008->sub_4B9C50();

            Ped* pPed;
            if (field_0_cam == &field_C_player->field_208_aux_game_camera)
            {
                pPed = field_C_player->field_2C8_unkq;
            }
            else
            {
                field_C_player = gGame_0x40_67E008->IterateNextPlayer_4B9D10();
                if (field_C_player)
                {
                    pPed = field_C_player->field_2C4_player_ped;
                }
                else
                {
                    continue;
                }
            }
            if (pPed)
            {
                field_10_zone = gMap_0x370_6F6268->get_nav_zone_unknown_4DF890(pPed->get_cam_x().ToInt(), pPed->field_1AC_cam.y.ToInt());
            }
        }
    }
}
