#include "Car_BC.hpp"
#include "Garox_2B00.hpp"
#include "Globals.hpp"
#include "PurpleDoom.hpp"
#include "cool_nash_0x294.hpp"
#include "debug.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "text_0x14.hpp"

EXPORT_VAR Car_214* gCar_214_705F20;
GLOBAL(gCar_214_705F20, 0x705F20);

EXPORT_VAR Car_6C* gCar_6C_677930;
GLOBAL(gCar_6C_677930, 0x677930);

EXPORT_VAR Car_E0C4* gCar_E0C4_67792C;
GLOBAL(gCar_E0C4_67792C, 0x67792C);

EXPORT_VAR Car_3C* gCar_3C_6F61E8;
GLOBAL(gCar_3C_6F61E8, 0x6F61E8);

EXPORT_VAR Car_3C* gCar_3C_6791A8;
GLOBAL(gCar_3C_6791A8, 0x6791A8);

EXPORT_VAR s32 dword_679188;
GLOBAL(dword_679188, 0x679188);

MATCH_FUNC(0x451950)
void Car_3C::sub_451950(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (field_14_xpos != xpos || field_18_ypos != ypos || field_1C_zpos != zpos)
    {
        field_14_xpos = xpos;
        field_18_ypos = ypos;
        field_1C_zpos = zpos;
        sub_59E7B0();
    }
}

MATCH_FUNC(0x59e2e0)
void Car_3C::sub_59E2E0(void)
{
    sub_59F990();
    memcpy(field_4_0x4C_len, sprite_4c_ptr, sizeof(Sprite_4C));
}

MATCH_FUNC(0x59e300)
void Car_3C::sub_59E300()
{
    memcpy(sprite_4c_ptr, field_4_0x4C_len, sizeof(Sprite_4C));
}

STUB_FUNC(0x59e320)
s32 Car_3C::sub_59E320(char_type a2)
{
    return 0;
}

STUB_FUNC(0x59e390)
bool Car_3C::sub_59E390(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x59e4c0)
s32 Car_3C::sub_59E4C0(s32 a2, s32 a3)
{
    return 0;
}

MATCH_FUNC(0x59e7b0)
void Car_3C::sub_59E7B0()
{
    field_39 = -1;
    if (sprite_4c_ptr != NULL)
    {
        sprite_4c_ptr->field_48 = 0;
    }
    if (field_4_0x4C_len != NULL)
    {
        field_4_0x4C_len->field_48 = 0;
    }
}

MATCH_FUNC(0x59e7d0)
Car_3C* Car_3C::sub_59E7D0(s32 a2)
{
    Car_3C* result;

    sub_59E9C0();
    next_ptr->sub_5A4D90();
    gCar_3C_6F61E8 = this;
    if (gMap_0x370_6F6268->sub_4E1520(field_1C_zpos.ToInt()))
    {
        return gCar_3C_6791A8;
    }
    result = (Car_3C*)gPurpleDoom_1_679208->sub_477E60(this, a2);
    if (result)
    {
        dword_679188 = 3;
        gCar_3C_6791A8 = result;
    }
    return result;
}

STUB_FUNC(0x59e960)
s32 Car_3C::sub_59E960()
{
    return 0;
}

STUB_FUNC(0x59e9c0)
Car_3C* Car_3C::sub_59E9C0()
{
    return 0;
}

STUB_FUNC(0x59ea00)
s16 Car_3C::sub_59EA00(s16 a2)
{
    return 0;
}

STUB_FUNC(0x59eaa0)
s16 Car_3C::sub_59EAA0()
{
    return 0;
}

STUB_FUNC(0x59eae0)
char_type Car_3C::sub_59EAE0()
{
    return 0;
}

STUB_FUNC(0x59eb30)
void Car_3C::sub_59EB30(s32 a2, f32* a3)
{
}

STUB_FUNC(0x59ee40)
void Car_3C::ShowHorn_59EE40(s32 a2, s32 a3)
{
}

STUB_FUNC(0x59eff0)
void Car_3C::sub_59EFF0()
{
}

MATCH_FUNC(0x59f950)
void Car_3C::sub_59F950(Fix16 a2, Fix16 a3, Fix16 a4)
{
    if (sprite_4c_ptr == NULL)
    {
        Sprite_4C* pOldFree = gSprite_5D598_70381C->field_0_pFree;
        gSprite_5D598_70381C->field_0_pFree = gSprite_5D598_70381C->field_0_pFree->field_2C_pNext;
        pOldFree->sub_5A57A0();
        sprite_4c_ptr = pOldFree;
    }

    Sprite_4C* pSprite4C = sprite_4c_ptr;
    pSprite4C->field_0_width = a2;
    pSprite4C->field_4_height = a3;
    pSprite4C->field_8 = a4;
}

STUB_FUNC(0x59f990)
Sprite_4C* Car_3C::sub_59F990()
{
    return 0;
}

STUB_FUNC(0x59fa40)
Sprite_4C* Car_3C::sub_59FA40()
{
    return 0;
}

STUB_FUNC(0x59fad0)
Sprite_4C* Car_3C::sub_59FAD0()
{
    return 0;
}

STUB_FUNC(0x5a0150)
char_type Car_3C::sub_5A0150(s32 a2, u8* a3, u8* a4)
{
    return 0;
}

STUB_FUNC(0x5a0320)
char_type Car_3C::sub_5A0320(u32* a2, u32* a3, u8* a4, u8* a5)
{
    return 0;
}

STUB_FUNC(0x5a1030)
char_type Car_3C::sub_5A1030(Car_3C* a2, Car_3C** a3, u8* a4)
{
    return 0;
}

STUB_FUNC(0x5a1490)
bool Car_3C::sub_5A1490(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5a19c0)
char_type Car_3C::sub_5A19C0()
{
    return 0;
}

STUB_FUNC(0x5a1a60)
char_type Car_3C::sub_5A1A60()
{
    return 0;
}

STUB_FUNC(0x5a1b30)
char_type Car_3C::sub_5A1B30(Car_3C* a2)
{
    return 0;
}

STUB_FUNC(0x5a1bd0)
char_type Car_3C::sub_5A1BD0()
{
    return 0;
}

STUB_FUNC(0x5a1ca0)
char_type Car_3C::sub_5A1CA0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x5a2500)
char_type Car_3C::sub_5A2500()
{
    return 0;
}

STUB_FUNC(0x5a2710)
s32* Car_3C::sub_5A2710(s32* a2, Car_3C* a3, s32* a4, s32 a5, u8* a6, u8* a7, char_type* a8)
{
    return 0;
}

MATCH_FUNC(0x5a29d0)
void Car_3C::sub_5A29D0()
{
    if (!field_10)
    {
        if (!bSkip_audio_67D6BE)
        {
            field_10 = gRoot_sound_66B038.CreateSoundObject_40EF40(this, 1);
        }
    }
}

STUB_FUNC(0x5a2a00)
infallible_turing* Car_3C::sub_5A2A00()
{
    return 0;
}

STUB_FUNC(0x5a2a30)
void Car_3C::sub_5A2A30()
{
}

STUB_FUNC(0x5a2cf0)
s32 Car_3C::sub_5A2CF0()
{
    return 0;
}

STUB_FUNC(0x5a3030)
infallible_turing* Car_3C::sub_5A3030()
{
    return 0;
}

STUB_FUNC(0x5a3100)
Car_BC* Car_3C::sub_5A3100(Car_3C* a2, s32 a3, s32 a4, s16 a5)
{
    return 0;
}

STUB_FUNC(0x5a3540)
void Car_3C::dtor_5A3540()
{
}

STUB_FUNC(0x5A4D90)
void Car_3C::sub_5A4D90()
{
}

STUB_FUNC(0x5a5e50)
void Car_3C::ctor_5A5E50()
{
}

STUB_FUNC(0x5c8680)
s32 Car_214::sub_5C8680(u8 a2)
{
    return 0;
}

STUB_FUNC(0x5c86c0)
char_type Car_214::sub_5C86C0(s32* a2, u32* a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9)
{
    return 0;
}

STUB_FUNC(0x5c8750)
void Car_214::sub_5C8750()
{
}

STUB_FUNC(0x5c8780)
u16* Car_214::sub_5C8780(u8 a2, Car_3C* pCarSprite)
{
    return 0;
}

STUB_FUNC(0x47bd00)
Car_2* Car_2::sub_47BD00()
{
    return 0;
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
Car_BC* Car_6C::sub_444F80(s32 a1, s32 a2, s32 a3, cool_nash_0x294* a4)
{
    return 0;
}

STUB_FUNC(0x444fa0)
s32 Car_6C::sub_444FA0(s32 x, s32 y, s32 z, s32 a5)
{
    return 0;
}

STUB_FUNC(0x4458b0)
Car_BC* Car_6C::sub_4458B0(s32 arg0, s32 a3, s32 a4, s32 a2)
{
    return 0;
}

STUB_FUNC(0x446230)
Car_BC* Car_6C::sub_446230(s32 arg0, s32 a3, s32 a4, s16 a5, s32 a2, s32 a7)
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
Car_6C::Car_6C()
{
}

STUB_FUNC(0x446dc0)
Car_6C::~Car_6C()
{
}

STUB_FUNC(0x563560)
s32 Car_B0::sub_563560(Car_3C* a2)
{
    return 0;
}

STUB_FUNC(0x439ec0)
bool Car_BC::sub_439EC0()
{
    return 0;
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
    sprintf(byte_67CE50, "car%d%c%c", this->field_84_car_info_idx, 0, 0);
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

STUB_FUNC(0x43a1f0)
bool Car_BC::is_bus_43A1F0()
{
    return 0;
}

MATCH_FUNC(0x43a230)
bool Car_BC::sub_43A230()
{
    return this->field_50_car_sprite->field_38 != 0;
}

STUB_FUNC(0x43a240)
s32* Car_BC::sub_43A240(s32* a2)
{
    return 0;
}

STUB_FUNC(0x43a3c0)
bool Car_BC::sub_43A3C0()
{
    return 0;
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

STUB_FUNC(0x43a600)
void Car_BC::sub_43A600()
{
}

STUB_FUNC(0x43a680)
s32 Car_BC::sub_43A680()
{
    return 0;
}

STUB_FUNC(0x43a6f0)
bool Car_BC::sub_43A6F0(u8 a2)
{
    return gGtx_0x106C_703DD4->get_car_info_5AA3B0(this->field_84_car_info_idx)->num_remaps > 1u &&
        this->field_50_car_sprite->field_24_remap != a2;
}

STUB_FUNC(0x43a730)
s32 Car_BC::sub_43A730(u8 a2)
{
    return 0;
}

MATCH_FUNC(0x43a780)
void Car_BC::sub_43A780(u8 a2)
{

    if (sub_43A6F0(a2))
    {
        field_50_car_sprite->sub_59EA00(a2);
    }

    if (field_64)
    {
        if (field_64->field_C->sub_43A6F0(a2))
        {
            field_64->field_C->field_50_car_sprite->sub_59EA00(a2);
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

STUB_FUNC(0x43a950)
Car_B0* Car_BC::sub_43A950()
{
    return 0;
}

STUB_FUNC(0x43a970)
Car_B0* Car_BC::sub_43A970()
{
    return 0;
}

STUB_FUNC(0x43a9a0)
void Car_BC::sub_43A9A0(cool_nash_0x294* a2)
{
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
s32 Car_BC::sub_43ADC0(s32 a2)
{
    return 0;
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

STUB_FUNC(0x43af60)
Car_78* Car_BC::sub_43AF60()
{
    return 0;
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
bool Car_BC::sub_43B2B0(cool_nash_0x294* a2)
{
    return 0;
}

STUB_FUNC(0x43b340)
Car_10* Car_BC::sub_43B340(u8 a2)
{
    return 0;
}

STUB_FUNC(0x43b360)
char_type Car_BC::sub_43B360()
{
    return 0;
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
    return 0;
}

STUB_FUNC(0x43b750)
char_type Car_BC::sub_43B750()
{
    return 0;
}

STUB_FUNC(0x43b770)
Char_8* Car_BC::sub_43B770()
{
    return 0;
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

STUB_FUNC(0x43bc30)
void Car_BC::sub_43BC30()
{
}

STUB_FUNC(0x43bca0)
void Car_BC::sub_43BCA0()
{
}

STUB_FUNC(0x43bd00)
Car_B0* Car_BC::sub_43BD00()
{
    return 0;
}

STUB_FUNC(0x43bd40)
char_type Car_BC::sub_43BD40()
{
    return 0;
}

STUB_FUNC(0x43bf10)
s32 Car_BC::sub_43BF10()
{
    return 0;
}

STUB_FUNC(0x43bf70)
u32 Car_BC::sub_43BF70()
{
    return 0;
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
char_type Car_BC::sub_43CF30(s32 a2)
{
    return 0;
}

STUB_FUNC(0x43d1c0)
bool Car_BC::sub_43D1C0(s32 a2)
{
    return 0;
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
s32* Car_BC::sub_43D690(Car_BC* a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return 0;
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
cool_nash_0x294* Car_BC::sub_43E990()
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

STUB_FUNC(0x4405f0)
void Car_BC::sub_4405F0()
{
}

STUB_FUNC(0x440630)
void Car_BC::sub_440630(s32 a2)
{
}

STUB_FUNC(0x440660)
s32 Car_BC::sub_440660(u8 a2)
{
    return 0;
}

STUB_FUNC(0x4406b0)
u8* Car_BC::sub_4406B0(cool_nash_0x294* a2)
{
    return 0;
}

STUB_FUNC(0x4406e0)
void Car_BC::sub_4406E0(cool_nash_0x294* a2)
{
}

STUB_FUNC(0x4407f0)
void Car_BC::sub_4407F0()
{
}

STUB_FUNC(0x440840)
Car_3C* Car_BC::sub_440840()
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

STUB_FUNC(0x441520)
void Car_BC::sub_441520()
{
}

STUB_FUNC(0x4415c0)
void Car_BC::sub_4415C0()
{
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
    if (field_A7 <= 0)
    {
        return;
    }

    if (field_A7 != 248)
    {
        field_A7--;
    }
}

STUB_FUNC(0x4417f0)
void Car_BC::sub_4417F0()
{
}

STUB_FUNC(0x441800)
char_type Car_BC::sub_441800(char_type a2)
{
    return 0;
}

STUB_FUNC(0x4418a0)
Car_A4_10* Car_BC::sub_4418A0()
{
    return 0;
}

STUB_FUNC(0x4418b0)
Car_A4_10* Car_BC::sub_4418B0()
{
    return 0;
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
    return 0;
}

STUB_FUNC(0x441a70)
char_type Car_BC::sub_441A70()
{
    return 0;
}

STUB_FUNC(0x441b00)
void Car_BC::sub_441B00()
{
    Car_10* pIter = field_C;
    s32 tmp;
    for (s32 i = 0; i < GTA2_COUNTOF(field_C); i++)
    {
        pIter->sub_439DA0(&tmp);
        pIter++;
    }
}

STUB_FUNC(0x441b20)
s16 Car_BC::sub_441B20()
{
    return 0;
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

STUB_FUNC(0x442170)
bool Car_BC::sub_442170()
{
    return 0;
}

STUB_FUNC(0x442190)
void Car_BC::sub_442190()
{
}

STUB_FUNC(0x4421b0)
char_type Car_BC::sub_4421B0()
{
    return 0;
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

STUB_FUNC(0x4435a0)
char_type Car_BC::sub_4435A0()
{
    return 0;
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

STUB_FUNC(0x443ae0)
void Car_BC::sub_443AE0(s32 a2)
{
}

STUB_FUNC(0x443bd0)
void Car_BC::sub_443BD0(s32 a2)
{
}

STUB_FUNC(0x443c40)
void Car_BC::sub_443C40(s32 a2)
{
}

MATCH_FUNC(0x443d00)
s32 Car_BC::sub_443D00(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    gPurpleDoom_1_679208->sub_477B60(field_50_car_sprite);
    Car_3C* pCarSprite = field_50_car_sprite;
    if (pCarSprite->field_14_xpos != xpos || pCarSprite->field_18_ypos != ypos || pCarSprite->field_1C_zpos != zpos)
    {
        pCarSprite->field_14_xpos = xpos;
        pCarSprite->field_18_ypos = ypos;
        pCarSprite->field_1C_zpos = zpos;
        pCarSprite->sub_59E7B0();
    }
    Car_B0* field_58_uni = (Car_B0*)field_58_uni_Car78_or_Car_B0;
    if (field_58_uni)
    {
        field_58_uni->sub_563560(field_50_car_sprite);
    }
    return gPurpleDoom_1_679208->sub_477B20(field_50_car_sprite);
}

STUB_FUNC(0x443d70)
void Car_BC::sub_443D70(s32 a2)
{
}

STUB_FUNC(0x443da0)
Car_6C* Car_BC::sub_443DA0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x443e50)
Car_6C* Car_BC::sub_443E50()
{
    return 0;
}

STUB_FUNC(0x443e80)
void Car_BC::sub_443E80()
{
}

STUB_FUNC(0x443eb0)
void Car_BC::sub_443EB0(s32 a2)
{
}

STUB_FUNC(0x443ee0)
Car_6C* Car_BC::sub_443EE0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x443f30)
s32 Car_BC::sub_443F30(s32 a2, s32 a3, s32 a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x444020)
char_type Car_BC::sub_444020()
{
    return 0;
}

STUB_FUNC(0x444090)
u32 Car_BC::sub_444090(Car_BC* a1)
{
    return 0;
}

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

STUB_FUNC(0x444490)
Car_6C* Car_BC::sub_444490()
{
    return 0;
}

STUB_FUNC(0x4446e0)
void Car_BC::sub_4446E0()
{
}

STUB_FUNC(0x4447d0)
s32 Car_BC::sub_4447D0()
{
    return 0;
}

STUB_FUNC(0x444860)
Car_BC::Car_BC()
{
    this->field_0_qq = 0;
    this->field_4.field_0_pOwner = 0;
    this->field_8 = 0;
    this->field_54_driver = 0;
    this->field_74_damage = 0;
    this->field_8C = 0;
    this->field_98 = 0;
    this->field_9C = 0;
    this->field_7C_uni_num = 0;
    this->field_76 = 0;
    this->field_A4 = 0;
    this->field_A5 = 0;
    this->field_A6 = 0;
    this->field_4C_next = 0;
    this->field_84_car_info_idx = 87;
    this->field_50_car_sprite = 0;
    this->field_58_uni_Car78_or_Car_B0 = 0;
    this->field_5C = 0;
    this->field_88 = 0;
    this->field_6C_maybe_id = 0xFFFF;
    this->field_64 = 0;
    this->field_78_flags = 0;
    // TODO
    // Object_3C::sub_5A7010((Object_3C *)this);
    this->field_A7 = 0;
    this->field_80 = 0;
    this->field_A0 = 0;
    this->field_8D = 0;
    this->field_70 = 0;
    this->field_90 = 0;
    this->field_94 = 0;
    this->field_95 = 0;
    this->field_68 = 0x4000;
    this->field_8E = 0;
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
        field_8.clear_bit(0x0b);
        field_8.clear_bit(0x0c);
        field_8.clear_bit(0x0d);
        field_8.clear_bit(0x0e);
        field_8.clear_bit(0x1c);
        field_8.clear_bit(0x1d);
        field_8.clear_bit(0x1e);
        field_8.clear_bit(0x1f);
    }
    field_8.clear_bit(0x06);
    field_8.clear_bit(0x17);
    if ((gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_84_car_info_idx)->info_flags & 4) == 4)
    {
        field_8.clear_bit(0x0f);
    }
}

STUB_FUNC(0x52a6d0)
Car_3C* Car_BC::sub_52A6D0(Car_3C* a2)
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

STUB_FUNC(0x40ac40)
void Car_8::dtor_40AC40()
{
}

STUB_FUNC(0x563970)
Car_8* Car_8::ctor_563970()
{
    return 0;
}
