#pragma once

#include "BitSet32.hpp"
#include "Car_10.hpp"
#include "Fix16.hpp"
#include "Function.hpp"
#include "Object_3C.hpp"
#include "Ped.hpp"
#include "Ped_List_4.hpp"
#include "Pool.hpp"
#include "ang16.hpp"
#include "gtx_0x106C.hpp"
#include "miss2_xyz.hpp"
#include "sprite.hpp"
#include <wchar.h>

struct gmp_zone_info;
class Camera_0xBC;
class Object_2C;
class Fix16_Rect;
class Car_78;
class CarPhysics_B0;
class Trailer;
class Ped;
class Char_8;
class Hamburger_40;
class Ped_List_4;
class car_info;
class infallible_turing;
class Gang_144;
class Player;

EXTERN_GLOBAL(Fix16, dword_6777D0);

EXPORT Ang16 __stdcall sub_4F7940(s32* a2);

// TODO: Move
class Car_78
{
  public:
    EXPORT void sub_4476F0();
    EXPORT char_type sub_447710();
    EXPORT void sub_447970();
    EXPORT bool GoToBlock_447CA0(u8 x, u8 y, u8 z, s32 maybe_direction);
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
    EXPORT void sub_4537D0();
    EXPORT void sub_4538B0();
    EXPORT void sub_453990(s32 a2);
    EXPORT Car_BC* sub_4539B0();
    EXPORT u8* sub_4539D0();
    EXPORT s32 sub_453A40();
    EXPORT void sub_453BB0();
    EXPORT void SetCar_453BF0(Car_BC* a2);
    EXPORT u8* sub_453C00();
    EXPORT s32 PoolAllocate();
    EXPORT Car_78();

    void PoolDeallocate()
    {
        field_0 = 0;
    }

    Car_BC* field_0;
    s32 field_4;
    char_type field_8;
    char_type field_9;
    char_type field_A;
    char_type field_B;
    Car_78* mpNext;
    Ang16 field_10;
    s16 field_12;
    s32 field_14;
    Fix16 field_18;
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
    Fix16 field_74;
};

class Car_78_Pool
{
  public:
    //Inlined in Car_6C constructor 9.6f -> 0x420eb0
    Car_78_Pool()
    {
    }

    ~Car_78_Pool()
    {
    }

    Car_78* Allocate()
    {
        return field_0_pool.Allocate();
    }

    // TODO: get 9.6f inline addr
    void DeAllocate(Car_78* p78)
    {
        field_0_pool.DeAllocate(p78);
    }

    PoolBasic<Car_78, 306> field_0_pool;
};

class Car_18
{
  public:
    s32 field_0;
    Object_2C* field_4_O2C;
    s32 field_8_type;
    s32 field_C;
    u8 field_10_remap_rng;
    u8 field_11_pad[3];
    s32 field_14;
};

class Car_214
{
  public:
    EXPORT void sub_5C8680(u8 idx);
    EXPORT char_type sub_5C86C0(s32* pType, s32* f_C, s32 f_0, Fix16 xpos, Fix16 ypos, Fix16 zpos, Fix16 a8, Fix16 a9);
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
    EXPORT void IncNextRngRemapIdx_47BD90();
    u16 field_0;
};

EXTERN_GLOBAL(Fix16, dword_6F7690);

EXTERN_GLOBAL(Fix16, dword_6F77D4);

EXTERN_GLOBAL(Ang16, dword_6F804C);

EXTERN_GLOBAL(Ang16, word_6F771E);

EXTERN_GLOBAL(Fix16, dword_6F77C0);
EXTERN_GLOBAL(Fix16, dword_6F77C4);

class Car_6C
{
  public:
    void sub_4C39F0()
    {
        if (field_20 != 4)
        {
            field_20 = 4;
        }
        else
        {
            field_20 = 3;
        }

        if (field_24 != 1)
        {
            field_24 = 1;
        }
        else
        {
            field_24 = 2;
        }
    }

    EXPORT void sub_444980();
    EXPORT u32 sub_444AB0(s32 a2, u16* a3, s32 a4, u16* a5);
    EXPORT Car_BC* sub_444CF0(s32 car_model_type, Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT Car_BC* GetNearestCarFromCoord_444F80(Fix16 x, Fix16 y, Fix16 z, Ped* pPed);
    EXPORT Car_BC* GetNearestEnterableCarFromCoord_444FA0(Fix16 x, Fix16 y, Fix16 z, Ped* pPed);
    EXPORT Car_BC* DoGetNearestCarFromCoord_444FC0(Fix16 xpos,
                                                   Fix16 ypos,
                                                   Fix16 zpos,
                                                   s32 bMatchDriverless,
                                                   Ped* pPed,
                                                   char_type bIgnorePedRestrictions);

    EXPORT Car_BC* sub_445210(Sprite* a1, u8 a2);
    EXPORT Car_BC* sub_4458B0(s32 arg0, s32 a3, s32 a4, s32 a2);
    EXPORT Car_BC* SpawnCarAt_446230(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx, Fix16 maybe_w_scale);
    EXPORT Trailer* sub_446530(Fix16 xpos, Fix16 ypos, Ang16 rotation, s32 car_idx, s32 trailer_idx);
    EXPORT void sub_446730(Car_BC* pCar);

    EXPORT void sub_4466C0(s32 a2);
    EXPORT void sub_446760();
    EXPORT void CarsService_446790();
    EXPORT bool CanAlloc_446870(s32 a2);
    EXPORT u32 CanAllocateOfType_446930(s32 a2);
    EXPORT Car_6C();
    EXPORT ~Car_6C();

    // 9.6f inlined
    inline Car_BC* SpawnCar_426E10(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx)
    {
        return SpawnCarAt_446230(xpos, ypos, zpos, rotation, car_info_idx, dword_6F77C4);
    }

    // 9.6f inlined
    inline Car_BC* SpawnCar_4764A0(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, s32 car_info_idx)
    {
        return SpawnCarAt_446230(xpos, ypos, zpos, rotation, car_info_idx, dword_6F7690);
    }

    // unknown inlined function
    inline Car_BC* SpawnCar_shortened(s32 car_info_idx)
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
    Car_2 field_10_remap_rng;
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
    u8 field_54;
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
    EXPORT void SetTruckCabAndTrailerCar_407BB0(Car_BC* a2, Car_BC* a3);
    EXPORT s32* sub_407BD0(s32* a2);
    EXPORT s32* sub_407CE0();
    EXPORT char_type sub_408140();
    EXPORT void SetupCarPhysicsAndSpriteBinding_408190();
    EXPORT void DeAllocateCarPhysics_4081B0();
    EXPORT char_type sub_4081D0();
    EXPORT s32 sub_408220();

    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    //Inlined in Car_6C constructor 9.6f -> 0x4212d0
    Trailer()
    {
        mpNext = NULL;
        field_8_truck_cab = NULL;
        field_C_pCarOnTrailer = NULL;
        field_0 = NULL;
        0;
    }

    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    Trailer* mpNext;
    Car_BC* field_8_truck_cab;
    Car_BC* field_C_pCarOnTrailer;
};

static inline bool IsArmyModel(s32 idx1)
{
    return idx1 == car_model_enum::TANK || idx1 == 22 || idx1 == 30 || idx1 == 3;
}

class Car_BC
{
  public:
    EXPORT Ang16 sub_4403A0();

    EXPORT s16 sub_43D5D0(Fix16 a2);


    EXPORT bool IsPoliceCar_439EC0();
    EXPORT Fix16 GetDamageFactorOnSpeed_439EE0();
    EXPORT Fix16 GetMaxSpeed_439F30();
    EXPORT wchar_t* GetCarStr_439F80();
    EXPORT Fix16_Point sub_439FB0();
    EXPORT Fix16_Point get_rear_wheel_offset_43A0E0();
    EXPORT Fix16 get_mass_43A120();
    EXPORT Fix16 get_anti_strngth_43A1D0();
    EXPORT bool is_bus_43A1F0();
    EXPORT bool sub_43A230();
    EXPORT Fix16 sub_43A240();
    EXPORT bool IsCarInAir_43A3C0();
    EXPORT Ang16 GetOrientationAngle_43A3E0();
    EXPORT Fix16_Point get_linvel_43A450();
    EXPORT Fix16 GetVelocity_43A4C0();
    EXPORT Fix16 sub_43A590();
    EXPORT Fix16 sub_43A5B0();
    EXPORT void RemoveAllDamage();
    EXPORT bool AllowResprayOrPlates();
    EXPORT bool IsNotCurrentRemap(u8 remap);
    EXPORT bool IsNotCurrentRemapOfCarAndTrailerCar(u8 remap);
    EXPORT void SetCarRemap(u8 remap);
    EXPORT void AssignRandomRemap_43A7D0();
    EXPORT char_type sub_43A850();
    EXPORT void sub_43A950();
    EXPORT void sub_43A970();
    EXPORT void SetDriver(Ped* a2);
    EXPORT void sub_43A9F0();
    EXPORT void Deactivate_43AA60();
    EXPORT char_type IsThreatToSearchingPed_43AAE0();
    EXPORT char_type sub_43AAF0(Sprite* a2);
    EXPORT void ProcessCarToCarImpact_43ADC0(Sprite* a2);
    EXPORT bool CanExitCar_43AF10();
    EXPORT void sub_43AF40();
    EXPORT void sub_43AF60();
    EXPORT char_type sub_43AFE0(s32 target_door);
    EXPORT bool sub_43B140(s32 a2);
    EXPORT bool sub_43B2B0(Ped* a2);
    EXPORT Car_Door_10* GetDoor(u8 door_idx);
    EXPORT char_type GetRemap();
    EXPORT void sub_43B380();
    EXPORT void sub_43B3D0();
    EXPORT s32 sub_43B420(s32 a2, u32* a3, u32* a4);
    EXPORT bool sub_43B540(u8 targetDoor);
    EXPORT s32* sub_43B5A0(s32 a2, Fix16* a3, Fix16* a4);
    EXPORT char_type sub_43B730();
    EXPORT char_type sub_43B750();
    EXPORT void sub_43B770();
    EXPORT void sub_43B7B0(Car_BC* a2);
    EXPORT bool sub_43B850(s32 a2);
    EXPORT void sub_43B870(s32 a2, Fix16_Point* a3);
    EXPORT s32 sub_43BB90(u8 a1);
    EXPORT void sub_43BBC0();
    EXPORT void SetupCarPhysicsAndSpriteBinding_43BC30();
    EXPORT void SetupCarPhysicsAndSpriteBinding_43BCA0();
    EXPORT void DeAllocateCarPhysics_43BD00();
    EXPORT char_type sub_43BD40();
    EXPORT void BrakeLightsOn_43BF10();
    EXPORT void BrakeLightsOff_43BF70();
    EXPORT char_type sub_43BFE0();
    EXPORT void sub_43C0C0();
    EXPORT void sub_43C1C0();
    EXPORT void sub_43C260();
    EXPORT void sub_43C310();
    EXPORT void sub_43C3C0();
    EXPORT void sub_43C470();
    EXPORT s32 sub_43C500();
    EXPORT void sub_43C650();
    EXPORT s32 sub_43C700();
    EXPORT void sub_43C840();
    EXPORT void ActivateEmergencyLights_43C920();
    EXPORT void DeactivateEmergencyLights_43C9D0();
    EXPORT void SyncEmergencyLightState_43CA80();
    EXPORT s32 sub_43CBE0();
    EXPORT Car_BC* sub_43CDF0(char_type a2);
    EXPORT void DamageArea_43CF30(s32 damage_area);
    EXPORT bool IsAreaDamaged_43D1C0(s32 damage_area);
    EXPORT void TryDamageArea_43D2C0(u8 damage_area, s32 damageAmount);
    EXPORT s32 sub_43D400();
    EXPORT void ExplodeCar_43D690(s32 a3, Fix16 x, Fix16 y);
    EXPORT void sub_43D7B0(s32 a2);
    EXPORT void ExplodeCar_Unknown_43D840(s32 a2);
    EXPORT s16 sub_43DA90(s16 a2, Fix16_Point* a3);
    EXPORT void sub_43DB80();
    EXPORT void sub_43DBD0();
    EXPORT bool sub_43DC00();
    EXPORT bool sub_43DC80(s32 a2, s32 a3);
    EXPORT bool IsBeingCrushed_43DD50();
    EXPORT void sub_43DD60();
    EXPORT char_type ManageDrowning_43E560();
    EXPORT Car_BC* GetCabOrSelf_43E8D0();
    EXPORT Ped* GetEffectiveDriver_43E990();
    EXPORT bool OnObjectTouched_43EA60(Object_2C* a2);
    EXPORT char_type HandleCarHitByObject_43F130(Object_2C* a2);
    EXPORT Fix16 sub_440510();
    EXPORT char_type GetPassengersCount_440570();
    EXPORT void InitCarAIControl_440590();
    EXPORT void SpawnDriverPed();
    EXPORT void AddGangDriver_440630(Gang_144* pGang);
    EXPORT void AttachGangIcon_440660(u8 arrow_colour);
    EXPORT void ShowCarName_4406B0(Ped* pPed);
    EXPORT void sub_4406E0(Ped* a2);
    EXPORT void ClearDriver_4407F0();
    EXPORT Sprite* GetSprite_440840();
    EXPORT void PutWaterCannonOnRoof_440AC0();
    EXPORT void PutTankCannonOnRoof_440B10();
    EXPORT void PutMachineGunOnRoof_440B60();
    EXPORT void PutTV_Antenna_440BB0();
    EXPORT char_type RotateRoofObjectTowardTarget_440C10(Ang16 a2);
    EXPORT char_type HandleRoofTurretRotation_440D90(char_type a2);
    EXPORT void sub_440F90(char_type a2);
    EXPORT void GoToBlockTest_441030(u8 x, u8 y, u8 z, s32 maybe_direction);
    EXPORT void GotoBlock_441080(u8 x, u8 y, u8 z, s32 maybe_direction);
    EXPORT char_type sub_4410D0(s16 a2, u8* a3, s32 a4, s32 a5);
    EXPORT Fix16 GetZPos_441330();
    EXPORT void sub_441360();
    EXPORT void sub_4436A0();
    EXPORT void sub_441380();
    EXPORT void UpdateTrainCarriagesOnTrack_4413B0(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void sub_441520();
    EXPORT void UpdateBrakeLights_4415C0();
    EXPORT u32* sub_441600(u32* a2);
    EXPORT void sub_4416D0(s32 a2);
    EXPORT void sub_4417D0();
    EXPORT void sub_4417F0();
    EXPORT void HandleSpecialInput_441800(char_type a2);
    EXPORT void DoDetachTrailer_4418A0();
    EXPORT void DetachTrailerAndUpdateDamage_4418B0();
    EXPORT void HandleUserInput_4418D0(char_type bForwardGasOn,
                                       char_type bFootBrakeOn,
                                       char_type bLeftOn,
                                       char_type bRightOn,
                                       char_type bHandBrakeOn,
                                       char_type bNowSpecialPressed,
                                       char_type bWasSpecialPressed,
                                       char_type bAttack);
    EXPORT void AllocCarPhysics_4419E0();
    EXPORT void DeAllocateCarPhysics_441A10();
    EXPORT char_type sub_441A40();
    EXPORT void sub_441A70();
    EXPORT void sub_441B00();
    EXPORT void sub_441B20();
    EXPORT void sub_441B50();
    EXPORT void sub_441C00();
    EXPORT s16 sub_441D40();
    EXPORT void sub_441E70();
    EXPORT bool sub_442170();
    EXPORT void sub_442190();
    EXPORT char_type sub_4421B0();
    EXPORT bool sub_442200();
    EXPORT void sub_442310();
    EXPORT char_type sub_4424C0();
    EXPORT Ang16 GetRadioTowerAngle_442520();
    EXPORT void ManageTVAntenna_4425D0();
    EXPORT void sub_4426D0();
    EXPORT void DetachTrailer_442760();
    EXPORT void AttachTrailer_4427A0(Car_BC* pToFind);
    EXPORT s32 sub_442810();
    EXPORT void LightUpdate_442D10();
    EXPORT char_type sub_442D70();
    EXPORT char_type TrailerUpdate_443130();
    EXPORT char_type PoolUpdate();
    EXPORT void sub_443330();
    EXPORT bool sub_443360(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot);
    EXPORT void sub_4435A0();
    EXPORT s32 GetCrashSoundCategory_4435B0();
    EXPORT void sub_4435F0();
    EXPORT void sub_443710(Fix16_Point* a2);
    EXPORT static s32 __stdcall get_car_weapon_cost_443A50(s32 weapon_kind);
    EXPORT void BuyCarWeapon_4438C0(s32 weapon_kind);
    EXPORT static void __stdcall sub_443AB0(Player* pPlayer, s32 weapon_cost);
    EXPORT void ResprayOrChangePlates(u8 remap);
    EXPORT void ResprayOrCleanPlates(u8 remap);
    EXPORT void HandleShops_443C40(Object_2C* pObj);
    EXPORT void sub_443D00(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT void IncrementCarStats_443D70(s32 car_type);
    EXPORT void IncrementAllocatedCarType_443DA0(s32 a2);
    EXPORT void sub_443E50();
    EXPORT void sub_443E80();
    EXPORT void sub_443EB0(s32 a2);
    EXPORT void sub_443EE0(s32 a2);
    EXPORT void sub_443F30(s32 object_type, s32 argb, s32 a4, s32 a5);
    EXPORT void sub_444020();
    EXPORT u32 GetEffectiveDriverPedId_444090();
    EXPORT void SetSirens_4441B0();
    EXPORT void PoolAllocate();
    EXPORT void sub_4446E0();
    EXPORT void PoolDeallocate();

    Car_BC(); // 0x444860
    ~Car_BC(); // 0x444960

    EXPORT void sub_447360();
    EXPORT bool sub_564300();

    // Inlined 0x4118d0
    s32 is_driven_by_player() const
    {
        if (field_54_driver != NULL && field_54_driver->field_15C_player != NULL)
        {
            return true;
        }
        return false;
    }

    // Inlined 0x421720
    bool is_on_trailer_421720()
    {
        // We are on the trailer
        return field_64_pTrailer && field_64_pTrailer->field_C_pCarOnTrailer == this;
    }

    Ped* get_driver_4118B0()
    {
        return field_54_driver;
    }

    // Inlined 0x41E460
    bool is_trailer_cab_41E460()
    {
        // We are the cab of the trailer
        return field_64_pTrailer && field_64_pTrailer->field_8_truck_cab == this;
    }

    // Inlined 0x475E60
    bool HasOtherCarOnTrailer_475E60(Car_BC* pExcludeCar)
    {
        // A car is on the trailer but must not be pExcludeCar
        return field_64_pTrailer && field_64_pTrailer->field_C_pCarOnTrailer == pExcludeCar && this != pExcludeCar;
    }

    bool IsCargoCarOf_4BA390(Car_BC* pOther) const
    {
        return field_64_pTrailer && field_64_pTrailer->field_8_truck_cab == pOther && this != pOther;
    }

    inline bool IsWithinArea(SCR_Rect_f* rect)
    {
        Fix16 x_pos = field_50_car_sprite->field_14_xy.x;
        Fix16 width = rect->field_C_size.field_0_x;
        Fix16 y_pos, z_pos;
        Fix16 height;
        Fix16 z_target;
        return (
            x_pos >= rect->field_0_pos.field_0_x - width && x_pos <= rect->field_0_pos.field_0_x + width &&
            (y_pos = field_50_car_sprite->field_14_xy.y,
             height = rect->field_C_size.field_4_y,
             y_pos >= rect->field_0_pos.field_4_y - height) &&
            field_50_car_sprite->field_14_xy.y <= rect->field_0_pos.field_4_y + height &&
            (z_pos = field_50_car_sprite->field_1C_zpos, z_target = rect->field_0_pos.field_8_z, z_pos.ToUInt8() == z_target.ToUInt8()));
    }

    inline bool IsWithinBlock(SCR_XYZ_f* pos)
    {
        Sprite* pSprite = field_50_car_sprite;
        Fix16 car_z_pos;
        return (pSprite->field_14_xy.x >= pos->field_0_x.ToUInt8() && pSprite->field_14_xy.x < (pos->field_0_x.ToUInt8() + 1)

                && pSprite->field_14_xy.y >= pos->field_4_y.ToUInt8() && pSprite->field_14_xy.y < (pos->field_4_y.ToUInt8() + 1)

                && (car_z_pos = pSprite->field_1C_zpos, car_z_pos.ToUInt8() == pos->field_8_z.ToUInt8()));
    }

    bool IsTrainModel_403BA0()
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

    // TODO: Find 9.6f addr
    inline bool inline_info_flags_bit2()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 2) == 2;
    }

    bool IsMaxDamage_40F890()
    {
        return field_74_damage == 32001;
    }

    inline bool sub_414F80()
    {
        return sub_414F20() && (field_A4 & 4) != 0;
    }

    inline s32 GetCarInfoIdx_411940()
    {
        return field_84_car_info_idx;
    }

    bool sub_414F20()
    {
        return inline_info_flags_bit2() || is_FBI_car_411920();
    }

    // 9.6f inline 0x421700
    inline bool inline_check_0x2_info_421700()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags_2 & 2) == 2;
    }

    // 9.6f inline 0x421660
    inline bool inline_check_0x4_info_421660()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 4) == 4;
    }

    inline bool inline_check_0x10_info_421640()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 0x10) == 0x10;
    }

    // 9.6f inline 0x4216C0
    inline bool inline_check_0x20_info_4216C0()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 0x20) == 0x20;
    }

    // 9.6f inline 0x421680
    inline bool inline_check_0x40_info_421680()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 0x40) == 0x40;
    }

    // 9.6f inline 0x4216A0
    inline bool inline_check_0x80_info_4216A0()
    {
        return (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 0x80) == 0x80;
    }

    void set_damage_bit15_421810()
    {
        field_8_damaged_areas.set_bit(CarDeltaBitsEnum::BottomLeftRoofLight_15);
    }

    void add_f78_bits_421890(u16 bits)
    {
        this->field_78_flags |= bits;
    }

    void set_f78_0x100_4218C0()
    {
        Car_BC::add_f78_bits_421890(0x100);
    }

    void set_f78_0x400_4218D0()
    {
        Car_BC::add_f78_bits_421890(0x400);
    }

    void set_f78_0x8_4218A0()
    {
        Car_BC::add_f78_bits_421890(8);
    }

    void set_f78_0x40_4218B0()
    {
        Car_BC::add_f78_bits_421890(0x40);
    }

    inline s32 GetCarKind_4343B0()
    {
        return field_A0_car_kind;
    }

    // 9.6f inline 0x425650
    inline void sub_425650()
    {
        if (inline_check_0x2_info_421700())
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
        if (inline_check_0x4_info_421660())
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
            if (inline_check_0x2_info_421700())
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
            if (inline_check_0x2_info_421700())
            {
                field_8_damaged_areas.set_bit(28); // set right rear door open/close frame 1 ????
            }
            else
            {
                field_8_damaged_areas.set_bit(23); // set right front headlight
            }
        }
        if (inline_check_0x4_info_421660())
        {
            field_8_damaged_areas.set_bit(15); // set right siren light or roof light
        }
    }

    Fix16 get_car_width()
    {
        return field_50_car_sprite->field_C_sprite_4c_ptr->field_0_width;
    }

    Fix16 get_car_height()
    {
        return field_50_car_sprite->field_C_sprite_4c_ptr->field_4_height;
    }

    bool sub_4214B0()
    {
        return field_88 == 2 || field_88 == 4 || field_88 == 3;
    }

    void sub_421470()
    {
        if (field_88 != 5 && field_88 != 2 && field_88 != 3)
        {
            field_88 = 4;
        }
    }

    bool sub_4214D0()
    {
        return field_88 == 7;
    }

    void SetF_88_4214E0()
    {
        field_88 = 7;
    }

    bool IsGunJeep_411910()
    {
        return field_84_car_info_idx == car_model_enum::GUNJEEP;
    }

    bool IsTank_411900()
    {
        return field_84_car_info_idx == car_model_enum::TANK;
    }

    bool IsSwatVan_4217A0()
    {
        return field_84_car_info_idx == car_model_enum::SWATVAN;
    }

    bool IsFireTruck_4118F0()
    {
        return field_84_car_info_idx == car_model_enum::FIRETRUK;
    }

    bool IsCopCar_421790()
    {
        return field_84_car_info_idx == car_model_enum::COPCAR;
    }

    bool is_FBI_car_411920()
    {
        return field_84_car_info_idx == car_model_enum::EDSELFBI;
    }

    bool IsGt24640_4217D0()
    {
        return field_84_car_info_idx == car_model_enum::GT24640;
    }

    bool IsTvVan_4217E0()
    {
        return this->field_84_car_info_idx == car_model_enum::TVVAN;
    }

    bool sub_4215C0()
    {
        return field_54_driver && field_54_driver->field_15C_player &&
            field_54_driver->get_occupation_403980() != ped_ocupation_enum::empty;
    }

    Fix16 sub_421910(Fix16 value)
    {
        if (field_68 != dword_6777D0)
        {
            return value * field_68;
        }
        else
        {
            return value;
        }
    }

    Fix16 get_x_41E430()
    {
        return field_50_car_sprite->field_14_xy.x;
    }

    Fix16 get_y_41E440()
    {
        return field_50_car_sprite->field_14_xy.y;
    }

    Fix16 get_z_41E450()
    {
        return field_50_car_sprite->field_1C_zpos;
    }

    inline bool IsEmittingHorn_411970()
    {
        return field_A7_horn > 0 && field_A7_horn <= 0xF8u;
    }

    inline bool IsEmittingHorn_411990()
    {
        // Not sure what is going on here, seems to same as 0x411970 ??
        return field_A7_horn <= 0xF8u && field_A7_horn > 0;
    }

    EXPORT char sub_444E40(Fix16 xpos, Fix16 ypos, Fix16 zpos);
    EXPORT char sub_445EC0(Fix16 xpos, Fix16 ypos, s32 maybe_direction);

    struct_4 field_0_qq;
    Ped_List_4 field_4_passengers_list;
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
    s32 field_70_exploder_ped_id;
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
    u8 field_A8;
    char_type field_A9;
    char_type field_AA;
    char_type field_AB;
    s32 field_AC;
    s32 field_B0;
    s32 field_B4_weapon_kind;
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

    void UpdateNextPrev(Car_BC* pItem)
    {
        field_0_pool.UpdateNextPrev(pItem);
    }

    Pool<Car_BC, 306> field_0_pool;
};

class TrailerPool
{
  public:
    //Inlined in Car_6C constructor 9.6f -> 0x425500
    TrailerPool()
    {
    }

    ~TrailerPool()
    {
    }

    PoolBasic<Trailer, 10> field_0_pool;
};

struct Car_14
{
    EXPORT Car_14();
    EXPORT ~Car_14();
    EXPORT char sub_582360(int param_1, Fix16 param_2, Fix16 param_3);
    EXPORT void MakeTrafficForCurrCamera_5832C0();
    EXPORT char_type sub_583260(s32 angle_face);
    EXPORT void GenerateTraffic_583670();

    // TODO: Prob just returns Fix16 and isn't static or stdcall
    EXPORT static Fix16* __stdcall sub_583750(Fix16* pRetF16, Fix16 a2, u8* pOut);

    EXPORT char_type SpawnTrafficCar_582480(s32 xpos, s32 ypos, s32 zpos);

    EXPORT char_type sub_583870(Fix16 xpos, Fix16 ypos);

    EXPORT s32 sub_5838E0(u8 a1);
    EXPORT char_type sub_583930(char_type a1, char_type a2);

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

EXTERN_GLOBAL(Car_78_Pool*, gCar_78_Pool_677CF8);

EXTERN_GLOBAL(TrailerPool*, gTrailerPool_66AC80);

EXTERN_GLOBAL(Car_14*, gCar_14_677934);

EXTERN_GLOBAL(char_type, gbRngRemapTableDone_679C0A);
