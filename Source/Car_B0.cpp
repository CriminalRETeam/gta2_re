#include "Car_B0.hpp"
#include "Globals.hpp"

EXPORT_VAR Car_D264* gCar_D264_6FE3E0;
GLOBAL(gCar_D264_6FE3E0, 0x6FE3E0);

EXPORT_VAR Ang16 DAT_0066AC08;
GLOBAL(DAT_0066AC08, 0x66AC08);

EXPORT_VAR Fix16 DAT_006FE20C;
GLOBAL(DAT_006FE20C, 0x6FE20C);

EXPORT_VAR Fix16 DAT_006FE290;
GLOBAL(DAT_006FE290, 0x6FE290);

EXPORT_VAR s32 DAT_006FE200;
GLOBAL(DAT_006FE200, 0x6FE200);

STUB_FUNC(0x4403a0)
s16* Car_B0::sub_4403A0(s16* a2)
{
    return 0;
}

STUB_FUNC(0x446ee0)
Car_B0::~Car_B0()
{
}

STUB_FUNC(0x559430)
void Car_B0::sub_559430()
{
}

STUB_FUNC(0x5597b0)
void Car_B0::sub_5597B0()
{
}

STUB_FUNC(0x5599d0)
bool Car_B0::sub_5599D0()
{
    return 0;
}

STUB_FUNC(0x559a40)
void Car_B0::sub_559A40()
{
}

MATCH_FUNC(0x559b40)
void Car_B0::sub_559B40()
{
    Car_A4_10* p10 = this->field_5C_pPrev->field_64_pTrailer;
    if (p10)
    {
        p10->sub_407CE0();
    }
}

MATCH_FUNC(0x559b50)
void Car_B0::sub_559B50()
{
    Car_A4_10* pTrailer = this->field_5C_pPrev->field_64_pTrailer;
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
void Car_B0::sub_559B90(const Fix16& a2)
{
    field_A0 = a2;
}

STUB_FUNC(0x559ba0)
u32 Car_B0::sub_559BA0()
{
    return 0;
}

STUB_FUNC(0x559c30)
void Car_B0::sub_559C30()
{
}

MATCH_FUNC(0x559dd0)
void Car_B0::sub_559DD0()
{
    if (this->field_5C_pPrev->field_54_driver)
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
u32 Car_B0::sub_559E20(s32 a2)
{
    return 0;
}

STUB_FUNC(0x559ec0)
s32* Car_B0::sub_559EC0(s32* a2)
{
    return 0;
}

STUB_FUNC(0x559ff0)
u32* Car_B0::sub_559FF0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x55a050)
u32* Car_B0::sub_55A050(u32* a2)
{
    return 0;
}

STUB_FUNC(0x55a0b0)
s32 Car_B0::sub_55A0B0()
{
    return 0;
}

STUB_FUNC(0x55a100)
u32* Car_B0::sub_55A100(u32* a2)
{
    return 0;
}

STUB_FUNC(0x55a150)
char_type Car_B0::sub_55A150()
{
    return 0;
}

STUB_FUNC(0x55a180)
char_type Car_B0::sub_55A180()
{
    return 0;
}

STUB_FUNC(0x55a1d0)
s32 Car_B0::sub_55A1D0(s32 a2, s32 a3, s32 a4, u32* a5)
{
    return 0;
}

STUB_FUNC(0x55a400)
void Car_B0::sub_55A400()
{
}

STUB_FUNC(0x55a4b0)
void Car_B0::sub_55A4B0()
{
}

STUB_FUNC(0x55a550)
void Car_B0::sub_55A550()
{
}

STUB_FUNC(0x55a600)
void Car_B0::sub_55A600()
{
}

STUB_FUNC(0x55a6a0)
u32* Car_B0::sub_55A6A0(u32* a2)
{
    return 0;
}

MATCH_FUNC(0x55a840)
void Car_B0::sub_55A840()
{
    this->field_48 = 0;
    this->field_4C = 0;
    this->field_50 = 0;
    this->field_54 = 0;
    this->field_7C = 0;
    this->field_80 = 0;
}

STUB_FUNC(0x55a860)
char_type Car_B0::sub_55A860(char_type bForwardGasOn, char_type bFootBrakeOn, char_type a4, char_type a5, char_type bHandBrakeOn)
{
    return 0;
}

STUB_FUNC(0x55aa00)
void Car_B0::sub_55AA00()
{
}

STUB_FUNC(0x55ab50)
s32* Car_B0::sub_55AB50(s32* a2, Sprite_4C** a3)
{
    return 0;
}

STUB_FUNC(0x55ad90)
s32 Car_B0::sub_55AD90(Sprite_4C* a2)
{
    return 0;
}

STUB_FUNC(0x55b3f0)
s32 Car_B0::sub_55B3F0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x55b4f0)
s32 Car_B0::sub_55B4F0(Sprite_4C* a2)
{
    return 0;
}

STUB_FUNC(0x55b970)
char_type Car_B0::sub_55B970(char_type* a2)
{
    return 0;
}

STUB_FUNC(0x55bfe0)
void Car_B0::sub_55BFE0()
{
}

STUB_FUNC(0x55c150)
char_type Car_B0::sub_55C150()
{
    return 0;
}

STUB_FUNC(0x55c3b0)
char_type Car_B0::sub_55C3B0(Sprite_4C** a2, Sprite_4C** a3)
{
    return 0;
}

STUB_FUNC(0x55c560)
s32 Car_B0::sub_55C560(s32* a2, s32* a3)
{
    return 0;
}

STUB_FUNC(0x55c5c0)
s32 Car_B0::sub_55C5C0(u32* a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x55c820)
s32 Car_B0::sub_55C820(u32* a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x55ca70)
void Car_B0::sub_55CA70(s32 a2, s32 a3)
{
}

STUB_FUNC(0x55cbb0)
void Car_B0::sub_55CBB0(s32 a2, s32 a3)
{
}

STUB_FUNC(0x55d200)
void Car_B0::sub_55D200(s32 a2, Sprite_4C* a3, s32 a4, s32 a5)
{
}

STUB_FUNC(0x55dc00)
char_type Car_B0::sub_55DC00()
{
    return 0;
}

STUB_FUNC(0x55e260)
void Car_B0::sub_55E260()
{
}

STUB_FUNC(0x55e470)
char_type Car_B0::sub_55E470()
{
    return 0;
}

STUB_FUNC(0x55eb80)
char_type Car_B0::sub_55EB80()
{
    return 0;
}

STUB_FUNC(0x55ec30)
s32 Car_B0::sub_55EC30()
{
    return 0;
}

STUB_FUNC(0x55ef20)
s32 Car_B0::sub_55EF20()
{
    return 0;
}

STUB_FUNC(0x55f020)
s32 Car_B0::sub_55F020()
{
    return 0;
}

STUB_FUNC(0x55f240)
char_type Car_B0::sub_55F240()
{
    return 0;
}

STUB_FUNC(0x55f280)
s32 Car_B0::sub_55F280()
{
    return 0;
}

STUB_FUNC(0x55f330)
void Car_B0::sub_55F330()
{
}

STUB_FUNC(0x55f360)
char_type Car_B0::sub_55F360()
{
    return 0;
}

STUB_FUNC(0x55f740)
s32 Car_B0::sub_55F740(s32* a2, s32* a3)
{
    return 0;
}

STUB_FUNC(0x55f7a0)
s32 Car_B0::sub_55F7A0(s32* a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x55f800)
s32 Car_B0::sub_55F800(s32* a2, s32* a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x55f930)
s32 Car_B0::sub_55F930(s32* a2)
{
    return 0;
}

STUB_FUNC(0x55f970)
__int64 Car_B0::sub_55F970(s32 a2)
{
    return 0;
}

STUB_FUNC(0x55f9a0)
s32 Car_B0::sub_55F9A0(s32* a2)
{
    return 0;
}

STUB_FUNC(0x55fa10)
s32 Car_B0::sub_55FA10(s32* a2)
{
    return 0;
}

STUB_FUNC(0x55fa60)
u32* Car_B0::sub_55FA60(u32* a2, s32* a3, s32* a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x55fc30)
Ped* Car_B0::sub_55FC30(s32* a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x55fd00)
s32 Car_B0::sub_55FD00(s32 a2)
{
    return 0;
}

STUB_FUNC(0x55ff20)
Car_78* Car_B0::sub_55FF20(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x5606c0)
void Car_B0::sub_5606C0(s32 a2, char_type a3)
{
}

STUB_FUNC(0x560b40)
void Car_B0::sub_560B40(s32 a2, s32 a3)
{
}

STUB_FUNC(0x560eb0)
__int64 Car_B0::sub_560EB0()
{
    return 0;
}

STUB_FUNC(0x560f20)
void Car_B0::sub_560F20(s32 a2)
{
}

STUB_FUNC(0x5610b0)
s32 Car_B0::sub_5610B0()
{
    return 0;
}

STUB_FUNC(0x561130)
u32* Car_B0::sub_561130(u32* a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x561350)
u32* Car_B0::sub_561350(u32* a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x561380)
u32* Car_B0::sub_561380(u32* a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x5615d0)
s32* Car_B0::sub_5615D0(s32* a2, s32* a3, s32 a4, u32* a5, s32 a6)
{
    return 0;
}

STUB_FUNC(0x561940)
s32 Car_B0::get_revs_561940()
{
    return 0;
}

STUB_FUNC(0x561970)
u32* Car_B0::sub_561970(u32* a2)
{
    return 0;
}

STUB_FUNC(0x561dd0)
u32* Car_B0::sub_561DD0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x561e50)
Sprite_4C** Car_B0::sub_561E50(Sprite_4C** a2)
{
    return 0;
}

STUB_FUNC(0x5620d0)
u32* Car_B0::sub_5620D0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x562450)
u32* Car_B0::sub_562450(u32* a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x562480)
s32 Car_B0::sub_562480()
{
    return 0;
}

STUB_FUNC(0x5624f0)
void Car_B0::sub_5624F0()
{
}

STUB_FUNC(0x562560)
s32 Car_B0::sub_562560()
{
    return 0;
}

STUB_FUNC(0x5626a0)
s32 Car_B0::sub_5626A0()
{
    return 0;
}

STUB_FUNC(0x5626c0)
u32* Car_B0::sub_5626C0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x5626f0)
char_type Car_B0::sub_5626F0()
{
    return 0;
}

STUB_FUNC(0x562910)
void Car_B0::sub_562910()
{
}

STUB_FUNC(0x562c20)
__int64 Car_B0::sub_562C20(s16* a2)
{
    return 0;
}

STUB_FUNC(0x562d00)
void Car_B0::sub_562D00()
{
}

STUB_FUNC(0x562eb0)
CarInfo_48* Car_B0::sub_562EB0()
{
    return 0;
}

STUB_FUNC(0x562ed0)
CarInfo_2C* Car_B0::sub_562ED0()
{
    return 0;
}

STUB_FUNC(0x562ef0)
s32 Car_B0::sub_562EF0()
{
    return 0;
}

STUB_FUNC(0x562f30)
s32 Car_B0::sub_562F30()
{
    return 0;
}

MATCH_FUNC(0x562fa0)
char_type Car_B0::sub_562FA0()
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
bool Car_B0::sub_562FE0()
{
    return 0;
}

STUB_FUNC(0x563280)
s32* Car_B0::sub_563280()
{
    return 0;
}

STUB_FUNC(0x563350)
s32* Car_B0::sub_563350()
{
    return 0;
}

STUB_FUNC(0x563460)
s32* Car_B0::sub_563460()
{
    return 0;
}

MATCH_FUNC(0x563560)
void Car_B0::sub_563560(Sprite* a2)
{
    this->field_38_cp1.field_0 = a2->field_14_xpos;
    this->field_38_cp1.field_4 = a2->field_18_ypos;
    this->field_6C_cp3 = a2->field_1C_zpos;
    this->field_58_theta = a2->field_0;
    this->field_78_pointing_ang_rad = 0;
    sub_563350();
}

STUB_FUNC(0x563590)
void Car_B0::sub_563590(Sprite* a2)
{
}

STUB_FUNC(0x563670)
s32 Car_B0::sub_563670()
{
    return 0;
}

MATCH_FUNC(0x5636c0)
void Car_B0::sub_5636C0()
{
    sub_563670();

    Car_A4_10* p10 = this->field_5C_pPrev->field_64_pTrailer;
    if (p10)
    {
        p10->field_C->field_58_uni_Car78_or_Car_B0->sub_563670();
    }
}

STUB_FUNC(0x5636e0)
bool Car_B0::sub_5636E0()
{
    return 0;
}

MATCH_FUNC(0x5637a0)
void Car_B0::Init_5637A0()
{
    field_40_linvel_1.reset();
    field_74_ang_vel_rad = Fix16(0);
    field_70 = DAT_006FE20C;
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
    sub_559B90(0);
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
    field_68_z_pos = DAT_006FE20C;
    field_84_front_skid = DAT_006FE20C;
    field_88_rear_skid = DAT_006FE20C;
    field_AA_sbw = 0;
    field_AB_tpa = 0;
}

MATCH_FUNC(0x563890)
void Car_B0::Reset_563890()
{
    field_30_cm1.reset();
    field_58_theta = DAT_0066AC08;
    field_38_cp1.reset();
    field_5C_pPrev = NULL;
    Init_5637A0();
    field_0_vel_read_only.reset();
}

MATCH_FUNC(0x5638c0)
void Car_B0::sub_5638C0(Car_BC* pCar)
{
    this->field_5C_pPrev = pCar;
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
Car_B0::Car_B0()
{
    field_C_pNext = NULL;
    Reset_563890();
}
