#pragma once

#include "BitSet32.hpp"
#include "Fix16.hpp"
#include "Function.hpp"
#include "Pool.hpp"
#include "ang16.hpp"

class Car_BC;
class Sprite;

class CarAI_78
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
    EXPORT void sub_451FF0();
    EXPORT char_type sub_452060();
    EXPORT s16 sub_452A20();
    EXPORT s16 sub_452DF0();
    EXPORT void sub_453470();
    EXPORT void sub_4537D0();
    EXPORT void sub_4538B0();
    EXPORT void sub_453990(Fix16 a2);
    EXPORT void sub_4539B0();
    EXPORT void sub_4539D0();
    EXPORT s32 sub_453A40();
    EXPORT void sub_453BB0();
    EXPORT void SetCar_453BF0(Car_BC* a2);
    EXPORT u8* sub_453C00();
    EXPORT void PoolAllocate();
    EXPORT CarAI_78();

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
    CarAI_78* mpNext;
    Ang16 field_10;
    s16 field_12;
    Fix16 field_14;
    Fix16 field_18;
    Fix16 field_1C;
    s32 field_20;

    union
    {
        u32 field_24_flags;
        CompilerBitField32 field_24_bf;
    };

    char_type field_28_junc_idx;
    char_type field_29;
    char_type field_2A;
    u8 field_2B;
    char_type field_2C;
    char_type field_2D;
    char_type field_2E;
    char_type field_2F;
    u8 field_30;
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
    Fix16 field_5C;
    Fix16 field_60;
    Fix16 field_64;
    s32 field_68;
    s32 field_6C;
    Sprite* field_70;
    Fix16 field_74;
};

class CarAI_78_Pool
{
  public:
    //Inlined in Car_6C constructor 9.6f -> 0x420eb0
    CarAI_78_Pool()
    {
    }

    ~CarAI_78_Pool()
    {
    }

    CarAI_78* Allocate()
    {
        return field_0_pool.Allocate();
    }

    // TODO: get 9.6f inline addr
    void DeAllocate(CarAI_78* p78)
    {
        field_0_pool.DeAllocate(p78);
    }

    PoolBasic<CarAI_78, 306> field_0_pool;
};

EXTERN_GLOBAL(CarAI_78_Pool*, gCarAI_78_Pool_677CF8);
