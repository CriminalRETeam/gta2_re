#pragma once

#include "BitSet32.hpp"
#include "Car_10.hpp"
#include "Fix16.hpp"
#include "Function.hpp"
#include "gtx_0x106C.hpp"
#include "Ped_Unknown_4.hpp"
#include "ang16.hpp"
#include "sprite.hpp"
#include "miss2_xyz.hpp"
#include "Object_3C.hpp"
#include "Pool.hpp"
#include <wchar.h>

struct gmp_zone_info;
class Camera_0xBC;
class Object_2C;
class Fix16_Rect;
class Car_78;
class CarPhysics_B0;
class Trailer;
//class Sprite;
class Ped;
class Char_8;
class Hamburger_40;
class Ped_Unknown_4;
class car_info;
class infallible_turing;
//class Sprite_4C;
//class Sprite_18;

// TODO: Move
class Car_78
{
  public:
    EXPORT void sub_4476F0();
    EXPORT char_type sub_447710();
    EXPORT void sub_447970();
    EXPORT char_type sub_447CA0(u8 a2, u8 a3, u8 a4, s32 a5);
    EXPORT char_type sub_447D40(s32 a2);
    EXPORT char_type sub_448270();
    EXPORT char_type sub_4482C0();
    EXPORT char_type sub_448770();
    EXPORT char_type sub_448CE0();
    EXPORT char_type sub_44A1F0();
    EXPORT s16 sub_44AF00();
    EXPORT char_type sub_44D1D0();
    EXPORT void sub_44E0C0();
    EXPORT void sub_44E560();
    EXPORT void sub_451980();
    EXPORT void sub_451FA0();
    EXPORT s32 sub_451FF0();
    EXPORT char_type sub_452060();
    EXPORT s16 sub_452A20();
    EXPORT s16 sub_452DF0();
    EXPORT s16 sub_453470();
    EXPORT s32* sub_453590(s32* a2);
    EXPORT char_type sub_4537D0();
    EXPORT Car_BC* sub_4538B0();
    EXPORT s32 sub_453990(s32 a2);
    EXPORT Car_BC* sub_4539B0();
    EXPORT u8* sub_4539D0();
    EXPORT s32 sub_453A40();
    EXPORT s16 sub_453BB0();
    EXPORT void sub_453BF0(Car_BC* a2);
    EXPORT u8* sub_453C00();
    EXPORT s32 sub_453D80();
    EXPORT Car_78();

    Car_BC* field_0;
    s32 field_4;
    char_type field_8;
    char_type field_9;
    char_type field_A;
    char_type field_B;
    Car_78* field_C;
    s16 field_10;
    s16 field_12;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24_flags;
    char_type field_28_junc_idx;
    char_type field_29;
    char_type field_2A;
    char_type field_2B;
    char_type field_2C;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
    char_type field_30;
    char_type field_31;
    char_type field_32;
    char_type field_33;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s16 field_54;
    s16 field_56;
    s16 field_58;
    s16 field_5A;
    s32 field_5C;
    s32 field_60;
    s32 field_64;
    s32 field_68;
    s32 field_6C;
    s32 field_70;
    s32 field_74;
};

class Car_8F74
{
  public:
    //Inlined in Car_6C constructor 9.6f -> 0x420eb0
    EXPORT Car_8F74()
    {
        Car_78* pIter = &field_4[0];
        for (s32 i = 0; i < 305; i++)
        {
            pIter->field_C = pIter + 1;
            pIter++;
        }

        field_4[305].field_C = NULL;
        field_0 = field_4;
    }

    ~Car_8F74()
    {
        field_0 = 0;
    }

    // TODO: get 9.6f inline addr
    void Remove(Car_78* p78)
    {
        p78->field_0 = 0;
        p78->field_C = field_0;
        field_0 = p78;
    }

    Car_78* field_0;
    Car_78 field_4[306];
};

class Car_18
{
  public:
    s32 field_0;
    Object_2C* field_4_O2C;
    s32 field_8;
    s32 field_C;
    u8 field_10_idx;
    u8 field_11_pad[3];
    s32 field_14;
};

class Car_214
{
  public:
    EXPORT void sub_5C8680(u8 idx);
    EXPORT char_type sub_5C86C0(s32* a2, u32* a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
    EXPORT void sub_5C8750();
    EXPORT u16* sub_5C8780(u8 a2, Sprite* pCarSprite);
    Car_18 field_0[22];
    s16 field_210_count;
    s16 field_212;
};

EXTERN_GLOBAL(Car_214*, gCar_214_705F20);


class Car_2
{
  public:
    EXPORT Car_2();
    EXPORT void sub_47BD90();
    u16 field_0;
};

EXTERN_GLOBAL(Fix16, dword_6F7690);

EXTERN_GLOBAL(Fix16, dword_6F77D4);

EXTERN_GLOBAL(Ang16, dword_6F804C);

EXTERN_GLOBAL(Ang16, word_6F771E);


class Car_6C
{
  public:
    EXPORT void sub_444980();
    EXPORT u32 sub_444AB0(s32 a2, u16* a3, s32 a4, u16* a5);
    EXPORT Car_BC* sub_444CF0(s32 a1, s32 a2, s32 a3, s32 a4);
    EXPORT Car_BC* sub_444F80(s32 a1, s32 a2, s32 a3, Ped* a4);
    EXPORT Car_BC* GetNearestCarFromCoord_444FA0(Fix16 x, Fix16 y, Fix16 z, Ped* pPed);
    EXPORT Car_BC* sub_445210(Sprite *a1, u8 a2);
    EXPORT Car_BC* sub_4458B0(s32 arg0, s32 a3, s32 a4, s32 a2);
    EXPORT Car_BC* SpawnCarAt_446230(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, Fix16 maybe_w_scale);
    EXPORT Trailer* sub_446530(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_idx, s32 trailer_idx);
    EXPORT void sub_446730(Car_BC *pCar);

    EXPORT void sub_4466C0(s32 a2);
    EXPORT void sub_446760();
    EXPORT void CarsService_446790();
    EXPORT bool sub_446870(s32 a2);
    EXPORT u32 sub_446930(s32 a2);
    EXPORT Car_6C();
    EXPORT ~Car_6C();
    
    // 9.6f inlined
    inline Car_BC* sub_426E10(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx)
    {
        return SpawnCarAt_446230(xpos, ypos, zpos, rotation, car_info_idx, dword_6F77C4);
    }

    // 9.6f inlined
    inline Car_BC* sub_4764A0(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx)
    {
        return SpawnCarAt_446230(xpos, ypos, zpos, rotation, car_info_idx, dword_6F7690);
    }

    // unknown inlined function
    inline Car_BC* sub_446230_shortened(s32 car_info_idx)
    {
        return SpawnCarAt_446230(dword_6F77D4, dword_6F77D4, dword_6F77C0, dword_6F804C, car_info_idx, dword_6F77C4);
    }

    Car_2 field_0;
    s16 field_2;
    Ped* field_4;
    char_type field_8;
    u8 field_9;
    u8 field_A;
    u8 field_B;
    s32 field_C_model_unk;
    Car_2 field_10_idx;
    Car_2 field_12;
    s32 field_14;
    char_type field_18;
    char_type field_19;
    char_type field_1A;
    char_type field_1B;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28_recycled_cars;
    s32 field_2C;
    s32 field_30;
    s32 field_34_unit_cars;
    s32 field_38;
    s32 field_3C_mission_cars;
    s32 field_40_proto_recycled_cars;
    s32 field_44;
    s32 field_48;
    Fix16_Point field_4C_tv_van_dir;
    char_type field_54;
    char_type field_55;
    char_type field_56;
    char_type field_57;
    s32 field_58_model_to_check_destroy;
    char_type field_5C;
    char_type field_5D;
    char_type field_5E;
    char_type field_5F;
    s32 field_60;
    Fix16 field_64;
    char_type field_68;
    char_type field_69_do_free_shopping;
    char_type field_6A;
    char_type field_6B;
};

EXTERN_GLOBAL(Car_6C*, gCar_6C_677930);


class Trailer
{
  public:
    EXPORT Car_BC* sub_407B90(Car_BC* a2);
    EXPORT void sub_407BB0(Car_BC* a2, Car_BC* a3);
    EXPORT s32* sub_407BD0(s32* a2);
    EXPORT s32* sub_407CE0();
    EXPORT char_type sub_408140();
    EXPORT void sub_408190();
    EXPORT void sub_4081B0();
    EXPORT char_type sub_4081D0();
    EXPORT s32 sub_408220();

    //Inlined in Car_6C constructor 9.6f -> 0x4212d0
    Trailer()
    {
        field_4 = NULL;
        field_8_truck_cab = NULL;
        field_C_trailer_carObj = NULL;
        field_0 = NULL;
        0;
    }

    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    Trailer* field_4;
    Car_BC* field_8_truck_cab;
    Car_BC* field_C_trailer_carObj;
};

class Car_BC
{
  public:
    EXPORT bool IsPoliceCar_439EC0();
    EXPORT u32* GetDamageFactorOnSpeed_439EE0(u32* a2);
    EXPORT u32* GetMaxSpeed_439F30(u32* a2);
    EXPORT wchar_t* GetCarStr_439F80();
    EXPORT u32* sub_439FB0(u32* a2);
    EXPORT u32* sub_43A0E0(u32* a2);
    EXPORT u32* sub_43A120(u32* a2);
    EXPORT Fix16 get_anti_strngth_43A1D0();
    EXPORT bool is_bus_43A1F0();
    EXPORT bool sub_43A230();
    EXPORT Fix16 sub_43A240();
    EXPORT bool IsCarInAir_43A3C0();
    EXPORT s16* sub_43A3E0(s16* a2);
    EXPORT u32* sub_43A450(u32* a2);
    EXPORT Fix16 sub_43A4C0();
    EXPORT u32* sub_43A590(u32* a2);
    EXPORT u32* sub_43A5B0(u32* a2);
    EXPORT void RemoveAllDamage();
    EXPORT bool AllowResprayOrPlates();
    EXPORT bool IsNotCurrentRemap(u8 remap);
    EXPORT bool IsNotCurrentRemapOfCarAndTrailerCar(u8 remap);
    EXPORT void SetCarRemap(u8 remap);
    EXPORT void sub_43A7D0();
    EXPORT char_type sub_43A850();
    EXPORT void sub_43A950();
    EXPORT void sub_43A970();
    EXPORT void SetDriver(Ped* a2);
    EXPORT void sub_43A9F0();
    EXPORT Car_BC* sub_43AA60();
    EXPORT char_type sub_43AAF0(s32 a2);
    EXPORT void sub_43ADC0(s32 a2);
    EXPORT bool sub_43AF10();
    EXPORT char_type sub_43AF40();
    EXPORT void sub_43AF60();
    EXPORT char_type sub_43AFE0(s32 a2);
    EXPORT bool sub_43B140(s32 a2);
    EXPORT bool sub_43B2B0(Ped* a2);
    EXPORT Car_Door_10* GetDoor(u8 door_idx);
    EXPORT char_type GetRemap();
    EXPORT void sub_43B380();
    EXPORT char_type sub_43B3D0();
    EXPORT s32 sub_43B420(s32 a2, u32* a3, u32* a4);
    EXPORT s32* sub_43B5A0(s32 a2, u32* a3, s32* a4);
    EXPORT char_type sub_43B730();
    EXPORT char_type sub_43B750();
    EXPORT void sub_43B770();
    EXPORT char_type sub_43B7B0(Car_BC* a2);
    EXPORT bool sub_43B850(s32 a2);
    EXPORT void sub_43B870(s32 a2, s32 a3);
    EXPORT s32 sub_43BB90(char_type a1);
    EXPORT char_type sub_43BBC0();
    EXPORT void sub_43BC30();
    EXPORT void sub_43BCA0();
    EXPORT void sub_43BD00();
    EXPORT char_type sub_43BD40();
    EXPORT void sub_43BF10();
    EXPORT void sub_43BF70();
    EXPORT char_type sub_43BFE0();
    EXPORT char_type sub_43C0C0();
    EXPORT u32 sub_43C1C0();
    EXPORT car_info* sub_43C260();
    EXPORT car_info* sub_43C310();
    EXPORT car_info* sub_43C3C0();
    EXPORT void sub_43C470();
    EXPORT s32 sub_43C500();
    EXPORT s32 sub_43C650();
    EXPORT s32 sub_43C700();
    EXPORT void sub_43C840();
    EXPORT void ActivateEmergencyLights_43C920();
    EXPORT void DeactivateEmergencyLights_43C9D0();
    EXPORT void sub_43CA80();
    EXPORT s32 sub_43CBE0();
    EXPORT Car_BC* sub_43CDF0(char_type a2);
    EXPORT void DamageArea_43CF30(s32 damage_area);
    EXPORT bool IsAreaDamaged_43D1C0(s32 damage_area);
    EXPORT void sub_43D2C0(char_type a2, s32 a3);
    EXPORT s32 sub_43D400();
    EXPORT void sub_43D690(s32 a3, s32 a4, s32 a5);
    EXPORT void sub_43D7B0(s32 a2);
    EXPORT void sub_43D840(s32 a2);
    EXPORT s16 sub_43DA90(s16 a2, s32 a3);
    EXPORT Char_8** sub_43DB80();
    EXPORT void sub_43DBD0();
    EXPORT bool sub_43DC00();
    EXPORT bool sub_43DC80(s32 a2, s32 a3);
    EXPORT char_type sub_43DD50();
    EXPORT void sub_43DD60();
    EXPORT char_type sub_43E560();
    EXPORT Car_BC* sub_43E8D0();
    EXPORT Ped* sub_43E990();
    EXPORT char_type sub_43EA60(s32 a2);
    EXPORT char_type sub_43F130(s32 a2);
    EXPORT u32* sub_440510(u32* a2);
    EXPORT char_type GetPassengersCount_440570();
    EXPORT void sub_440590();
    EXPORT void SpawnDriverPed();
    EXPORT void AddGangDriver_440630(Gang_144* pGang);
    EXPORT s32 AttachGangIcon_440660(u8 a2);
    EXPORT void ShowCarName_4406B0(Ped* pPed);
    EXPORT void sub_4406E0(Ped* a2);
    EXPORT void sub_4407F0();
    EXPORT Sprite* sub_440840();
    EXPORT void sub_440AC0();
    EXPORT void sub_440B10();
    EXPORT s32 sub_440B60();
    EXPORT s32 sub_440BB0();
    EXPORT char_type sub_440C10(char_type a2);
    EXPORT char_type sub_440D90(char_type a2);
    EXPORT void sub_440F90(char_type a2);
    EXPORT char_type sub_441030(u8 a2, u8 a3, u8 a4, s32 a5);
    EXPORT char_type sub_441080(u8 a2, u8 a3, u8 a4, s32 a5);
    EXPORT char_type sub_4410D0(s16 a2, u8* a3, s32 a4, s32 a5);
    EXPORT void sub_441360();
    EXPORT void sub_4436A0();
    EXPORT void sub_441380();
    EXPORT char_type sub_4413B0(s32 a2, s32 a3, s32 a4);
    EXPORT void sub_441520();
    EXPORT void sub_4415C0();
    EXPORT u32* sub_441600(u32* a2);
    EXPORT char_type sub_4416D0(s32 a2);
    EXPORT void sub_4417D0();
    EXPORT void sub_4417F0();
    EXPORT char_type sub_441800(char_type a2);
    EXPORT void sub_4418A0();
    EXPORT void sub_4418B0();
    EXPORT char_type
    sub_4418D0(char_type a2, char_type a3, char_type a4, char_type a5, char_type a6, char_type a7, char_type a8, char_type a9);
    EXPORT void sub_4419E0();
    EXPORT CarPhysics_B0* sub_441A10();
    EXPORT char_type sub_441A40();
    EXPORT void sub_441A70();
    EXPORT void sub_441B00();
    EXPORT void sub_441B20();
    EXPORT char_type sub_441B50();
    EXPORT void sub_441C00();
    EXPORT s16 sub_441D40();
    EXPORT void sub_441E70();
    EXPORT bool sub_442170();
    EXPORT void sub_442190();
    EXPORT char_type sub_4421B0();
    EXPORT bool sub_442200();
    EXPORT void sub_442310();
    EXPORT char_type sub_4424C0();
    EXPORT s16* sub_442520(s16* a2);
    EXPORT void sub_4425D0();
    EXPORT void sub_4426D0();
    EXPORT Trailer* sub_442760();
    EXPORT Car_BC* sub_4427A0(Car_BC* a2);
    EXPORT s32 sub_442810();
    EXPORT s32* sub_442D10();
    EXPORT char_type sub_442D70();
    EXPORT char_type sub_443130();
    EXPORT char_type sub_443170();
    EXPORT char_type sub_443330(s32 a2);
    EXPORT char_type sub_443360(s32 a2, s32 a3, s32 a4, s16 a5);
    EXPORT void sub_4435A0();
    EXPORT s32 sub_4435B0();
    EXPORT void sub_4435F0();
    EXPORT Car_6C* sub_443710(s32 a2);
    EXPORT static s32 __stdcall get_car_weapon_cost_443A50(s32 weapon_kind);
    EXPORT static void __stdcall sub_443AB0(Player* pPlayer, s32 weapon_cost);
    EXPORT void ResprayOrChangePlates(s32 remap);
    EXPORT void ResprayOrCleanPlates(s32 remap);
    EXPORT void sub_443C40(s32 a2);
    EXPORT void sub_443D00(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void IncrementCarStats_443D70(s32 car_type);
    EXPORT void IncrementAllocatedCarType_443DA0(s32 a2);
    EXPORT void sub_443E50();
    EXPORT void sub_443E80();
    EXPORT void sub_443EB0(s32 a2);
    EXPORT void sub_443EE0(s32 a2);
    EXPORT void sub_443F30(s32 object_type, s32 argb, s32 a4, s32 a5);
    EXPORT void sub_444020();
    EXPORT u32 sub_444090(Car_BC* a1);
    EXPORT void sub_4441B0();
    EXPORT void sub_444490();
    EXPORT void sub_4446E0();
    EXPORT void PoolDeallocate();

    Car_BC(); // 0x444860
    ~Car_BC(); // 0x444960

    EXPORT void sub_447360();
    EXPORT bool sub_564300();

    // Inlined 0x4118d0
    EXPORT bool is_driven_by_player() const
    {
        if (field_54_driver != NULL && field_54_driver->field_15C_player != NULL)
        {
            return true;
        }
        return false;
    }

    // Inlined 0x421720
    bool sub_421720()
    {
        return field_64_pTrailer && field_64_pTrailer->field_C_trailer_carObj == this;
    }

    // Inlined 0x41E460
    bool sub_41E460()
    {
        return field_64_pTrailer && field_64_pTrailer->field_8_truck_cab == this;
    }

    // Inlined 0x475E60
    bool sub_475E60(Car_BC* a1)
    {
        return field_64_pTrailer && field_64_pTrailer->field_C_trailer_carObj == a1 && this != a1;
    }

    inline bool IsWithinArea(SCR_Rect_f* rect)
    {
        Fix16 x_pos = field_50_car_sprite->field_14_xpos.x;
        Fix16 width = rect->field_C_size.field_0_x;
        Fix16 y_pos, z_pos;
        Fix16 height;
        Fix16 z_target;
        return (
            x_pos >= rect->field_0_pos.field_0_x - width && x_pos <= rect->field_0_pos.field_0_x + width &&
            (y_pos = field_50_car_sprite->field_14_xpos.y,
             height = rect->field_C_size.field_4_y,
             y_pos >= rect->field_0_pos.field_4_y - height) &&
            field_50_car_sprite->field_14_xpos.y <= rect->field_0_pos.field_4_y + height &&
            (z_pos = field_50_car_sprite->field_1C_zpos, z_target = rect->field_0_pos.field_8_z, z_pos.ToUInt8() == z_target.ToUInt8()));
    }

    inline bool IsWithinBlock(SCR_XYZ_f* pos)
    {
        Sprite* pSprite = field_50_car_sprite;
        Fix16 car_z_pos;
        return (pSprite->field_14_xpos.x >= pos->field_0_x.ToUInt8() && pSprite->field_14_xpos.x < (pos->field_0_x.ToUInt8() + 1)

                && pSprite->field_14_xpos.y >= pos->field_4_y.ToUInt8() && pSprite->field_14_xpos.y < (pos->field_4_y.ToUInt8() + 1)

                && (car_z_pos = pSprite->field_1C_zpos, car_z_pos.ToUInt8() == pos->field_8_z.ToUInt8()));
    }

    bool is_train_model()
    {
        return field_84_car_info_idx == car_model_enum::TRAIN || field_84_car_info_idx == car_model_enum::TRAINCAB ||
               field_84_car_info_idx == car_model_enum::TRAINFB || field_84_car_info_idx == car_model_enum::boxcar;
    }

    // 9.6f inline 0x421560
    void sub_421560(s32 a1)
    {
        field_7C_uni_num = a1;
        field_76 = 0;
    }

    // 9.6f inline 0x421700
    inline bool sub_421700()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & 2) == 2;
    }

    // 9.6f inline 0x421660
    inline bool sub_421660()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 4) == 4;
    }

    // 9.6f inline 0x4216C0
    inline bool sub_4216C0()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 0x20) == 0x20;
    }
    
    // 9.6f inline 0x425650
    inline void sub_425650()
    {
        if (sub_421700())
        {
            // clear left and right rear door frames
            field_8_damaged_areas.clear_bit(11);
            field_8_damaged_areas.clear_bit(12);
            field_8_damaged_areas.clear_bit(13);
            field_8_damaged_areas.clear_bit(14);

            field_8_damaged_areas.clear_bit(28);
            field_8_damaged_areas.clear_bit(29);
            field_8_damaged_areas.clear_bit(30);
            field_8_damaged_areas.clear_bit(31);
        }
        field_8_damaged_areas.clear_bit(6); // clear left front headlight
        field_8_damaged_areas.clear_bit(23); // clear right front headlight
        if (sub_421660())
        {
            field_8_damaged_areas.clear_bit(15); // clear right siren light or roof light
        }
    }

    // 9.6f inline 0x421430
    inline void sub_421430()
    {
        field_8_damaged_areas.clear_bit(5); // clear left rear brake light
        field_8_damaged_areas.clear_bit(22); // clear right rear brake light
    }

    // 9.6f inline 0x4213D0
    inline void sub_4213D0()
    {
        if (!field_8_damaged_areas.mask_bit(1)) // if not rear left damage
        {
            field_8_damaged_areas.set_bit(5); // set left rear brake light
        }
        if (!field_8_damaged_areas.mask_bit(0)) // if not rear right damage
        {
            field_8_damaged_areas.set_bit(22); // set right rear brake light
        }
    }

    // 9.6f inline 0x425590
    inline void sub_425590()
    {
        if (!field_8_damaged_areas.mask_bit(2)) // if not front left damage
        {
            if (sub_421700())
            {
                field_8_damaged_areas.set_bit(11); // set left rear door open/close frame 1 ????
            }
            else
            {
                field_8_damaged_areas.set_bit(6); // set left front headlight
            }
        }

        if (!field_8_damaged_areas.mask_bit(3)) // if not front right damage
        {
            if (sub_421700())
            {
                field_8_damaged_areas.set_bit(28); // set right rear door open/close frame 1 ????
            }
            else
            {
                field_8_damaged_areas.set_bit(23); // set right front headlight
            }
        }
        if (sub_421660())
        {
            field_8_damaged_areas.set_bit(15); // set right siren light or roof light
        }
    }

    bool is_FBI_car_411920()
    {
        return field_84_car_info_idx == car_model_enum::EDSELFBI;
    }
    
    struct_4 field_0_qq;
    Ped_Unknown_4 field_4;
    BitSet32 field_8_damaged_areas; // TODO: check if it's a bitset
    Car_Door_10 field_C_doors[4];
    Car_BC* mpNext;
    Sprite* field_50_car_sprite;
    Ped* field_54_driver;
    CarPhysics_B0* field_58_physics;
    Car_78* field_5C;
    Hamburger_40* field_60;
    Trailer* field_64_pTrailer;
    Fix16 field_68;
    s32 field_6C_maybe_id;
    s32 field_70;
    s16 field_74_damage;
    s16 field_76;
    u16 field_78_flags;
    s16 field_7A;
    s32 field_7C_uni_num;
    char_type field_80;
    char_type field_81;
    char_type field_82;
    char_type field_83;
    s32 field_84_car_info_idx;
    s32 field_88;
    char_type field_8C;
    char_type field_8D;
    char_type field_8E;
    char_type field_8F;
    s32 field_90;
    u8 field_94;
    char_type field_95;
    char_type field_96;
    char_type field_97;
    s32 field_98;
    s32 field_9C;
    s32 field_A0_car_kind; // police car, fire engine, mission car etc
    char_type field_A4;
    char_type field_A5;
    char_type field_A6;
    u8 field_A7_horn;
    char_type field_A8;
    char_type field_A9;
    char_type field_AA;
    char_type field_AB;
    s32 field_AC;
    s32 field_B0;
    s32 field_B4;
    char_type field_B8;
    char_type field_B9;
    char_type field_BA;
    char_type field_BB;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_BC, 0xBC)

struct Car_BC_Pool
{
    //Inlined in Car_6C constructor 9.6f -> 0x426db0
    Car_BC_Pool()
    {

    }

    ~Car_BC_Pool()
    {

    }

    // TODO: 9.6f addr/check
    void Remove(Car_BC* pCar)
    {
        field_0_pool.DeAllocate(pCar);
    }

    Pool<Car_BC, 306> field_0_pool;
};

class Car_A4
{
  public:
    //Inlined in Car_6C constructor 9.6f -> 0x425500
    EXPORT Car_A4()
    {
        Trailer* it = field_4;
        for (u32 i = 0; i < 9; i++)
        {
            it->field_4 = it + 1;
            it++;
        }
        field_4[9].field_4 = NULL;
        field_0 = field_4;
    }

    ~Car_A4()
    {
        field_0 = 0;
    }

    Trailer* field_0;
    Trailer field_4[10];
};

struct Car_14
{
    EXPORT Car_14();
    EXPORT ~Car_14();
    EXPORT char sub_582360(int param_1, Fix16 param_2, Fix16 param_3);
    EXPORT void MakeTrafficForCurrCamera_5832C0();
    EXPORT void GenerateTraffic_583670();

    Camera_0xBC* field_0_cam;
    Fix16_Rect* field_4_rect;
    s8 field_8;
    s8 field_9;
    s8 field_A;
    s8 field_B;
    Player* field_C_player;
    gmp_zone_info* field_10_zone;
};

EXTERN_GLOBAL(Car_BC_Pool*, gCar_BC_Pool_67792C);

EXTERN_GLOBAL(Sprite*, gSprite_6F61E8);

EXTERN_GLOBAL(Car_8F74*, gCar_8F74_677CF8);

EXTERN_GLOBAL(Car_A4*, gCar_A4_66AC80);

EXTERN_GLOBAL(Car_14*, gCar_14_677934);

EXTERN_GLOBAL(char_type, byte_679C0A);
