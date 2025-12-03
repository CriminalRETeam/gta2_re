#include "CarPhysics_B0.hpp"
#include "CarInfo_808.hpp"
#include "Globals.hpp"

DEFINE_GLOBAL(CarPhyisicsPool*, gCarPhysicsPool_6FE3E0, 0x6FE3E0);
DEFINE_GLOBAL(Ang16, DAT_0066AC08, 0x66AC08);
DEFINE_GLOBAL(Fix16, kFP16Zero_6FE20C, 0x6FE20C);
DEFINE_GLOBAL(Fix16, DAT_006FE290, 0x6FE290);
DEFINE_GLOBAL(s32, DAT_006FE200, 0x6FE200);
DEFINE_GLOBAL(ModelPhysics_48*, dword_6FE258, 0x6FE258);
DEFINE_GLOBAL(Fix16, dword_6FE1B0, 0x6FE1B0);

// TODO: Part of a global object? Inline static ctor @ crt_init_477990() ? check 9.6f
DEFINE_GLOBAL(Sprite*, dword_6791AC, 0x6791AC);

STUB_FUNC(0x4403a0)
s16* CarPhysics_B0::sub_4403A0(s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x446ee0)
CarPhysics_B0::~CarPhysics_B0()
{
}

// https://decomp.me/scratch/xqLh0
STUB_FUNC(0x559430)
void CarPhysics_B0::ShowPhysicsDebug_559430()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5597b0)
void CarPhysics_B0::sub_5597B0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5599d0)
bool CarPhysics_B0::sub_5599D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x559a40)
void CarPhysics_B0::sub_559A40()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x559b40)
void CarPhysics_B0::sub_559B40()
{
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        pTrailer->sub_407CE0();
    }
}

MATCH_FUNC(0x559b50)
void CarPhysics_B0::sub_559B50()
{
    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        if (pTrailer->field_0)
        {
            if (this->field_94_is_backward_gas_on)
            {
                this->field_94_is_backward_gas_on = 0;
                this->field_91_is_foot_brake_on = 1;
            }
            if (!this->field_93_is_forward_gas_on)
            {
                this->field_AD_turn_direction = 0;
            }
        }
    }
}

MATCH_FUNC(0x559b90)
void CarPhysics_B0::sub_559B90(const Fix16& a2)
{
    field_A0 = a2;
}

STUB_FUNC(0x559ba0)
u32 CarPhysics_B0::sub_559BA0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x559c30)
void CarPhysics_B0::sub_559C30()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x559dd0)
void CarPhysics_B0::sub_559DD0()
{
    if (this->field_5C_pCar->field_54_driver)
    {
        if (field_A0.mValue == 1)
        {
            this->field_95 = 0;
            this->field_93_is_forward_gas_on = 1;
            this->field_AD_turn_direction = -1;
        }
        else if (field_A0.mValue == 2)
        {
            this->field_95 = 0;
            this->field_93_is_forward_gas_on = 1;
            this->field_AD_turn_direction = 1;
        }
    }
}

STUB_FUNC(0x559e20)
u32 CarPhysics_B0::sub_559E20(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x559ec0)
s32* CarPhysics_B0::sub_559EC0(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/KYfsp
STUB_FUNC(0x559ff0)
u32* CarPhysics_B0::CalculateMass_559FF0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55a050)
u32* CarPhysics_B0::sub_55A050(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x55a0b0)
u8 CarPhysics_B0::IsInAir_55A0B0()
{
    // TODO: The surface checks are likely inlines
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        return pTrailer->field_8_truck_cab->field_58_physics->field_98_surface_type == 6 &&
            pTrailer->field_C_pCarOnTrailer->field_58_physics->field_98_surface_type == 6;
    }
    return field_98_surface_type == 6;
}

MATCH_FUNC(0x55a100)
Fix16 CarPhysics_B0::GetTrailerAwareTurnRatio_55A100()
{
    // TODO: This check is very likely an inline
    if (field_5C_pCar->field_64_pTrailer != NULL && field_5C_pCar->field_64_pTrailer->field_8_truck_cab == field_5C_pCar)
    {
        return dword_6FE1B0 * dword_6FE258->field_18_turn_ratio;
    }
    else
    {
        return dword_6FE258->field_18_turn_ratio;
    }
}

MATCH_FUNC(0x55a150)
char_type CarPhysics_B0::IsFootBrakeOn_55A150()
{
    char_type bFootBrakeOn;

    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pPhysics = pTrailer->field_8_truck_cab->field_58_physics;
        if (pPhysics)
        {
            bFootBrakeOn = pPhysics->field_91_is_foot_brake_on;
        }
        else
        {
            bFootBrakeOn = 0;
        }
    }
    else
    {
        return this->field_91_is_foot_brake_on;
    }
    return bFootBrakeOn;
}

MATCH_FUNC(0x55a180)
char_type CarPhysics_B0::IsAccelerationOrReverseOn_55A180()
{
    Trailer* pTrailer = field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        CarPhysics_B0* pCarPhysics = pTrailer->field_8_truck_cab->field_58_physics;
        if (pCarPhysics)
        {
            return (pCarPhysics->field_93_is_forward_gas_on || pCarPhysics->field_94_is_backward_gas_on);
        }
    }
    else
    {
        return (field_93_is_forward_gas_on || field_94_is_backward_gas_on);
    }
    return 0;
}

STUB_FUNC(0x55a1d0)
s32 CarPhysics_B0::sub_55A1D0(s32 a2, s32 a3, s32 a4, u32* a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55a400)
void CarPhysics_B0::restore_saved_physics_state_55A400()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55a4b0)
void CarPhysics_B0::save_physics_state_55A4B0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55a550)
void CarPhysics_B0::sub_55A550()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55a600)
void CarPhysics_B0::sub_55A600()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55a6a0)
u32* CarPhysics_B0::sub_55A6A0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x55a840)
void CarPhysics_B0::sub_55A840()
{
    this->field_48 = 0;
    this->field_4C = 0;
    this->field_50 = 0;
    this->field_54 = 0;
    this->field_7C = 0;
    this->field_80 = 0;
}

STUB_FUNC(0x55a860)
char_type CarPhysics_B0::sub_55A860(char_type bForwardGasOn, char_type bFootBrakeOn, char_type a4, char_type a5, char_type bHandBrakeOn)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55aa00)
void CarPhysics_B0::sub_55AA00()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55ab50)
s32* CarPhysics_B0::sub_55AB50(s32* a2, Sprite_4C** a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ad90)
s32 CarPhysics_B0::sub_55AD90(Sprite_4C* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55b3f0)
s32 CarPhysics_B0::sub_55B3F0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55b4f0)
s32 CarPhysics_B0::sub_55B4F0(Sprite_4C* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55b970)
char_type CarPhysics_B0::sub_55B970(char_type* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55bfe0)
void CarPhysics_B0::sub_55BFE0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x55c150)
char_type CarPhysics_B0::sub_55C150()
{
    Sprite* pCarSprite = this->field_5C_pCar->field_50_car_sprite;
    if (!pCarSprite->sub_5A2500() && !pCarSprite->sub_59E7D0(0))
    {
        Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
        if (pTrailer)
        {
            pCarSprite = pTrailer->field_C_pCarOnTrailer->field_50_car_sprite;
            if (!pCarSprite->sub_5A2500() && !pCarSprite->sub_59E7D0(0))
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    dword_6791AC = pCarSprite;
    return 1;
}

STUB_FUNC(0x55c3b0)
char_type CarPhysics_B0::sub_55C3B0(Sprite_4C** a2, Sprite_4C** a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55c560)
s32 CarPhysics_B0::sub_55C560(s32* a2, s32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55c5c0)
s32 CarPhysics_B0::sub_55C5C0(u32* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55c820)
s32 CarPhysics_B0::sub_55C820(u32* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ca70)
void CarPhysics_B0::sub_55CA70(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55cbb0)
void CarPhysics_B0::sub_55CBB0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55d200)
void CarPhysics_B0::sub_55D200(s32 a2, Sprite_4C* a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55dc00)
char_type CarPhysics_B0::sub_55DC00()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55e260)
void CarPhysics_B0::sub_55E260()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55e470)
char_type CarPhysics_B0::sub_55E470()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55eb80)
char_type CarPhysics_B0::sub_55EB80()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ec30)
s32 CarPhysics_B0::sub_55EC30()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ef20)
s32 CarPhysics_B0::sub_55EF20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f020)
s32 CarPhysics_B0::sub_55F020()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f240)
char_type CarPhysics_B0::sub_55F240()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f280)
s32 CarPhysics_B0::sub_55F280()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f330)
void CarPhysics_B0::sub_55F330()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x55f360)
char_type CarPhysics_B0::sub_55F360()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f740)
s32 CarPhysics_B0::sub_55F740(s32* a2, s32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f7a0)
s32 CarPhysics_B0::sub_55F7A0(s32* a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f800)
s32 CarPhysics_B0::sub_55F800(s32* a2, s32* a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f930)
s32 CarPhysics_B0::sub_55F930(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f970)
__int64 CarPhysics_B0::sub_55F970(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55f9a0)
s32 CarPhysics_B0::sub_55F9A0(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55fa10)
s32 CarPhysics_B0::sub_55FA10(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55fa60)
u32* CarPhysics_B0::sub_55FA60(u32* a2, s32* a3, s32* a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55fc30)
Ped* CarPhysics_B0::sub_55FC30(s32* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55fd00)
s32 CarPhysics_B0::sub_55FD00(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x55ff20)
Car_78* CarPhysics_B0::sub_55FF20(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5606c0)
void CarPhysics_B0::sub_5606C0(s32 a2, char_type a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x560b40)
void CarPhysics_B0::sub_560B40(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x560eb0)
__int64 CarPhysics_B0::sub_560EB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x560f20)
void CarPhysics_B0::sub_560F20(s32 a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5610b0)
s32 CarPhysics_B0::sub_5610B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561130)
u32* CarPhysics_B0::sub_561130(u32* a2, u32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561350)
u32* CarPhysics_B0::sub_561350(u32* a2, u32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561380)
u32* CarPhysics_B0::sub_561380(u32* a2, u32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5615d0)
s32* CarPhysics_B0::sub_5615D0(s32* a2, s32* a3, s32 a4, u32* a5, s32 a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561940)
s32 CarPhysics_B0::get_revs_561940()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561970)
u32* CarPhysics_B0::sub_561970(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561dd0)
u32* CarPhysics_B0::sub_561DD0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x561e50)
Sprite_4C** CarPhysics_B0::CalculateFrontSkid_561E50(Sprite_4C** a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5620d0)
u32* CarPhysics_B0::CalculateRearSkid_5620D0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x562450)
u32* CarPhysics_B0::sub_562450(u32* a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x562480)
s32 CarPhysics_B0::sub_562480()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5624f0)
void CarPhysics_B0::sub_5624F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x562560)
s32 CarPhysics_B0::sub_562560()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5626a0)
s32 CarPhysics_B0::IsGasPedalPressedEnough_5626A0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5626c0)
u32* CarPhysics_B0::sub_5626C0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5626f0)
char_type CarPhysics_B0::sub_5626F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x562910)
void CarPhysics_B0::sub_562910()
{
    NOT_IMPLEMENTED;
}

// TODO: Actually Fix16_Point method
MATCH_FUNC(0x562c20)
void CarPhysics_B0::RotateVelocity_562C20(const Ang16& angle)
{
    const Fix16 sin = Ang16::sine_40F500(angle);
    const Fix16 cos = Ang16::cosine_40F520(angle);

    const Fix16 x_old = field_0_vel_read_only.x;

    field_0_vel_read_only.x = (sin * field_0_vel_read_only.y) + (cos * field_0_vel_read_only.x);
    field_0_vel_read_only.y = (cos * field_0_vel_read_only.y) + ((-x_old) * sin);
}

STUB_FUNC(0x562d00)
void CarPhysics_B0::sub_562D00()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x562eb0)
ModelPhysics_48* CarPhysics_B0::sub_562EB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x562ed0)
CarInfo_2C* CarPhysics_B0::sub_562ED0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x562ef0)
s32 CarPhysics_B0::SetCurrentCarInfoAndModelPhysics_562EF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x562f30)
s32 CarPhysics_B0::sub_562F30()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x562fa0)
char_type CarPhysics_B0::sub_562FA0()
{
    if (sub_5599D0())
    {
        if (field_90_timer_since_last_move < 255)
        {
            this->field_90_timer_since_last_move++;
        }
        if (this->field_90_timer_since_last_move >= 20u)
        {
            return 1;
        }
    }
    else
    {
        this->field_90_timer_since_last_move = 0;
    }
    return 0;
}

STUB_FUNC(0x562fe0)
bool CarPhysics_B0::sub_562FE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x563280)
s32* CarPhysics_B0::sub_563280()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x563350)
s32* CarPhysics_B0::sub_563350()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x563460)
s32* CarPhysics_B0::sub_563460()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x563560)
void CarPhysics_B0::SetSprite_563560(Sprite* a2)
{
    this->field_38_cp1.x = a2->field_14_xpos.x;
    this->field_38_cp1.y = a2->field_14_xpos.y;
    this->field_6C_cp3 = a2->field_1C_zpos;
    this->field_58_theta = a2->field_0;
    this->field_78_pointing_ang_rad = 0;
    sub_563350();
}

STUB_FUNC(0x563590)
void CarPhysics_B0::sub_563590(Sprite* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x563670)
void CarPhysics_B0::sub_563670()
{
    Sprite* car_sprite = field_5C_pCar->field_50_car_sprite;
    car_sprite->sub_420600(field_38_cp1.x, field_38_cp1.y, field_6C_cp3);
    car_sprite->sub_420690(field_58_theta);
}

MATCH_FUNC(0x5636c0)
void CarPhysics_B0::sub_5636C0()
{
    sub_563670();

    Trailer* pTrailer = this->field_5C_pCar->field_64_pTrailer;
    if (pTrailer)
    {
        pTrailer->field_C_pCarOnTrailer->field_58_physics->sub_563670();
    }
}

STUB_FUNC(0x5636e0)
bool CarPhysics_B0::IsNearlyStopped_5636E0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5637a0)
void CarPhysics_B0::Init_5637A0()
{
    field_40_linvel_1.reset();
    field_74_ang_vel_rad = Fix16(0);
    field_70 = kFP16Zero_6FE20C;
    sub_55A840();
    field_91_is_foot_brake_on = 0;
    field_92_is_hand_brake_on = 0;
    field_93_is_forward_gas_on = 0;
    field_94_is_backward_gas_on = 0;
    field_95 = 0;
    field_AD_turn_direction = 0;
    field_A9_car_model = -1;
    field_A8_hand_brake_force = 0;
    field_90_timer_since_last_move = 0;
    sub_559B90(Fix16(0));
    field_A4 = 0;
    field_98_surface_type = 0;
    field_9C = 0;
    field_A5_current_slope_length = 0;
    field_A6_current_slope_left_tiles = 0;
    field_10[0].reset();
    field_10[1].reset();
    field_10[2].reset();
    field_10[3].reset();
    field_8C = 1;
    field_8_total_damage_q = 0;
    field_60_gas_pedal = DAT_006FE290;
    field_AC_drive_wheels_locked_q = 0;
    field_64 = DAT_006FE200;
    field_68_z_pos = kFP16Zero_6FE20C;
    field_84_front_skid = kFP16Zero_6FE20C;
    field_88_rear_skid = kFP16Zero_6FE20C;
    field_AA_sbw = 0;
    field_AB_tpa = 0;
}

MATCH_FUNC(0x563890)
void CarPhysics_B0::PoolAllocate()
{
    field_30_cm1.reset();
    field_58_theta = DAT_0066AC08;
    field_38_cp1.reset();
    field_5C_pCar = NULL;
    Init_5637A0();
    field_0_vel_read_only.reset();
}

MATCH_FUNC(0x447010)
EXPORT Fix16_Point CarPhysics_B0::get_linvel_447010()
{
    return field_40_linvel_1;
}

MATCH_FUNC(0x5638c0)
void CarPhysics_B0::SetCar_5638C0(Car_BC* pCar)
{
    this->field_5C_pCar = pCar;
    Ped* pDriver = pCar->field_54_driver;
    if (pDriver && pDriver->field_15C_player)
    {
        this->field_8C = 2;
    }
    else
    {
        this->field_8C = 1;
    }
}

MATCH_FUNC(0x563900)
CarPhysics_B0::CarPhysics_B0()
{
    mpNext = NULL;
    PoolAllocate();
}