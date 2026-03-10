#include "CarAI_78.hpp"
#include "Car_BC.hpp"
#include "Hamburger_500.hpp"
#include "RouteFinder.hpp"
#include "rng.hpp"
#include "CarPhysics_B0.hpp"

DEFINE_GLOBAL(Ang16, word_677CE8, 0x677CE8);
DEFINE_GLOBAL(Fix16, dword_677B90, 0x677B90);
DEFINE_GLOBAL(Fix16, dword_677C30, 0x677C30);
DEFINE_GLOBAL(Fix16, dword_677C38, 0x677C38);
DEFINE_GLOBAL(Fix16, dword_677C48, 0x677C48);
DEFINE_GLOBAL(Fix16, dword_6779F8, 0x6779F8);

DEFINE_GLOBAL_INIT(Fix16, dword_677BA0, Fix16(0x10000, 0), 0x677BA0);
DEFINE_GLOBAL_INIT(Fix16, dword_677B5C, Fix16(0x147, 0), 0x677B5C);
DEFINE_GLOBAL_INIT(Fix16, dword_677AC4, dword_677B5C, 0x677AC4);

DEFINE_GLOBAL_INIT(Fix16, dword_677B94, Fix16(0x4000, 0), 0x677B94);
DEFINE_GLOBAL_INIT(Fix16, dword_6779B8, Fix16(0x666, 0), 0x6779B8);
DEFINE_GLOBAL_INIT(Fix16, dword_677B50, Fix16(163, 0), 0x677B50);
DEFINE_GLOBAL_INIT(Fix16, dword_677CB4, dword_6779B8, 0x677CB4);
DEFINE_GLOBAL_INIT(Fix16, dword_6779A4, dword_677B50, 0x6779A4);

DEFINE_GLOBAL_INIT(Fix16, dword_6779D4, Fix16(0x2CCC, 0), 0x6779D4);
DEFINE_GLOBAL(CarAI_78_Pool*, gCarAI_78_Pool_677CF8, 0x677CF8);

STUB_FUNC(0x4476f0)
void CarAI_78::sub_4476F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x447710)
char_type CarAI_78::sub_447710()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x447970)
void CarAI_78::sub_447970()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x447ca0)
bool CarAI_78::GoToBlock_447CA0(u8 x, u8 y, u8 z, s32 maybe_direction)
{
    field_28_junc_idx = gRouteFinder_6FFDC8->StartRoute_58A190(dword_677C38.ToInt(),
                                                               dword_677C30.ToInt(),
                                                               (dword_677C48 - dword_677B94).ToInt(),
                                                               x,
                                                               y,
                                                               z,
                                                               maybe_direction);
    if (field_28_junc_idx > 0)
    {
        field_0->field_60->field_14_target_x = Fix16(x);
        field_0->field_60->field_18_target_y = Fix16(y);
        field_0->field_60->field_1C_target_z = dword_6779F8;
        field_56 = 0;
        return true;
    }
    return false;
}

STUB_FUNC(0x447d40)
char_type CarAI_78::sub_447D40(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448270)
char_type CarAI_78::sub_448270()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4482c0)
char_type CarAI_78::sub_4482C0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448770)
char_type CarAI_78::sub_448770()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x448ce0)
char_type CarAI_78::sub_448CE0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44a1f0)
char_type CarAI_78::sub_44A1F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44af00)
s16 CarAI_78::sub_44AF00()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44d1d0)
char_type CarAI_78::sub_44D1D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x44e0c0)
void CarAI_78::sub_44E0C0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x44e560)
void CarAI_78::sub_44E560()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451980)
void CarAI_78::sub_451980()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451fa0)
void CarAI_78::sub_451FA0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x451ff0)
s32 CarAI_78::sub_451FF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452060)
char_type CarAI_78::sub_452060()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452a20)
s16 CarAI_78::sub_452A20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x452df0)
s16 CarAI_78::sub_452DF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453470)
s16 CarAI_78::sub_453470()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x453590)
s32* CarAI_78::sub_453590(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x4537d0)
void CarAI_78::sub_4537D0()
{
    WIP_IMPLEMENTED;

    if ((this->field_0->field_A6 & 2) != 2 && (this->field_0->field_A6 & 1) != 1)
    {
        if (stru_6F6784.get_int_4F7AE0(2) <= 0)
        {
            field_0->field_A6 |= 1;
        }
        else
        {
            field_0->field_A6 |= 2;
        }
    }
}

MATCH_FUNC(0x4538b0)
void CarAI_78::sub_4538B0()
{
    if (this->field_24_flags & 0x80)
    {
        this->field_0->field_A6 &= ~2u;
        this->field_0->field_A6 &= ~1u;
    }
}

MATCH_FUNC(0x453990)
void CarAI_78::sub_453990(Fix16 a2)
{
    if (this->field_14 < a2)
    {
        this->field_14 = a2;
    }
}

STUB_FUNC(0x4539b0)
Car_BC* CarAI_78::sub_4539B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4539d0)
void CarAI_78::sub_4539D0()
{
    Fix16 f14 = this->field_14;
    Fix16 f18 = this->field_18;
    if (f14 < f18)
    {
        this->field_14 = field_1C + f14;
        if (field_1C < dword_677B90)
        {
            sub_453990(this->field_18);
        }
    }
    else
    {
        if (f14 > f18)
        {
            this->field_14 = f14 - ((dword_677BA0 * dword_677AC4));
            sub_453990(field_18);
        }
    }

    CarPhysics_B0* pPhysics = this->field_0->field_58_physics;
    pPhysics->field_91_is_foot_brake_on = 0;
    pPhysics->field_93_is_forward_gas_on = 0;
    pPhysics->field_94_is_backward_gas_on = 0;
    pPhysics->field_95 = 0;
}

STUB_FUNC(0x453a40)
s32 CarAI_78::sub_453A40()
{
    NOT_IMPLEMENTED;
    return 0;
}

// TODO: Move
STUB_FUNC(0x447650)
void __stdcall sub_447650()
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x453bb0)
void CarAI_78::sub_453BB0()
{
    WIP_IMPLEMENTED;

    sub_447650();
    this->field_10 = this->field_0->field_50_car_sprite->field_0;
    this->field_4C = Ang16::GetAngleFace_4F78F0(field_10);
    if (field_2B != -1)
    {
        field_2B++;
    }
    sub_453470();
}

MATCH_FUNC(0x453bf0)
void CarAI_78::SetCar_453BF0(Car_BC* a2)
{
    field_0 = a2;
}

STUB_FUNC(0x453c00)
u8* CarAI_78::sub_453C00()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x453d80)
void CarAI_78::PoolAllocate()
{
    this->field_0 = 0;
    this->field_4 = 0;
    this->field_8 = 0;
    this->field_9 = 0;
    this->field_A = 0;
    this->field_10 = word_677CE8;
    this->field_14 = dword_677B90;
    this->field_18 = dword_677CB4;
    this->field_1C = dword_6779A4;
    this->field_20 = 0;
    this->field_56 = 0;
    this->field_28_junc_idx = -1;
    this->field_29 = 0;
    this->field_58 = 0;
    this->field_24_flags &= ~0x2DFEFu;
    this->field_24_flags |= 0x12010u;
    this->field_2D = 0;
    this->field_2E = 0;
    this->field_2F = 0;
    this->field_5C = dword_677B90;
    this->field_60 = dword_677B90;
    this->field_64 = dword_677B90;
    this->field_68 = 0;
    this->field_6C = 0;
    this->field_34 = 0;
    this->field_38 = 0;
    this->field_3C = 0;
    this->field_40 = 0;
    this->field_44 = 0;
    this->field_48 = 0;
    this->field_4C = 0;
    this->field_70 = 0;
    this->field_74 = dword_6779D4;
    this->field_54 = 0;
    this->field_2A = 0;
    this->field_2B = 0;
    this->field_2C = 0;
    this->field_24_flags &= ~0x3C0000u;
    this->field_50 = 0;
    this->field_5A = 0;
    this->field_30 = 0;
}

MATCH_FUNC(0x453cb0)
CarAI_78::CarAI_78()
{
    this->field_10 = 0;
    this->field_0 = 0;
    this->field_4 = 0;
    this->field_8 = 0;
    this->field_9 = 0;
    this->field_A = 0;
    this->mpNext = 0;
    this->field_10 = word_677CE8;
    this->field_14 = dword_677B90;
    this->field_18 = dword_677CB4;
    this->field_1C = dword_6779A4;
    this->field_20 = 0;
    this->field_56 = 0;
    this->field_28_junc_idx = -1;
    this->field_29 = 0;
    this->field_58 = 0;
    this->field_24_flags &= ~0x2DFEFu;
    this->field_24_flags |= 0x12010u;
    this->field_2D = 0;
    this->field_2E = 0;
    this->field_2F = 0;
    this->field_5C = dword_677B90;
    this->field_60 = dword_677B90;
    this->field_64 = dword_677B90;
    this->field_68 = 0;
    this->field_6C = 0;
    this->field_34 = 0;
    this->field_38 = 0;
    this->field_3C = 0;
    this->field_40 = 0;
    this->field_44 = 0;
    this->field_48 = 0;
    this->field_4C = 0;
    this->field_70 = 0;
    this->field_74 = dword_677B90;
    this->field_54 = 0;
    this->field_2A = 0;
    this->field_2B = 0;
    this->field_2C = 0;
    this->field_5A = 0;
    this->field_50 = 0;
    this->field_24_flags &= ~0x3C0000u;
}
