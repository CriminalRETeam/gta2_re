#pragma once

#include "Function.hpp"
#include <math.h>
#include <windows.h>

#ifndef INLINE_MODE
#define INLINE_MODE __forceinline
#endif

class Fix16
{
    public:
    // 9.6f 0x40E570
    // https://decomp.me/scratch/5BHO3
    s32 operator==(const Fix16& value) const
    {
        return mValue == value.mValue;
    }
    
    // 9.6f 0x40CE50
    // https://decomp.me/scratch/MBAm6
    s32 operator<=(const Fix16& other) const
    {
        return mValue <= other.mValue;
    }

    Fix16& operator=(s32 value)
    {
        mValue = value;
        return *this;
    }

    Fix16 operator-(const Fix16& in) const
    {
        s32 value = mValue - in.mValue;
        return Fix16(value, 0);
    }

    Fix16 operator-=(const Fix16& other)
    {
        mValue -= other.mValue;
        return *this;
    }

    Fix16 operator+(const Fix16& in)
    {
        s32 value = mValue + in.mValue;
        return Fix16(value, 0);
    }

    void operator+=(const Fix16& other)
    {
        mValue += other.mValue;
    }

    Fix16 operator*(const Fix16& in) const
    {
        s32 value = (s32)((mValue * (__int64)in.mValue) >> 14);
        return Fix16(value, 0);
    }

    // Inlined from 9.6f from 0x401bd0
    Fix16 operator*(const s32& in) const
    {
        s32 value = mValue * in;
        return Fix16(value, 0);
    }

    //MATCH_FUNC(0x4086A0)
    Fix16 operator-() const
    {
        return Fix16(-mValue, 0);
    }

    s32 operator>(const Fix16& other) const
    {
        return mValue > other.mValue;
    }

    s32 operator<(const Fix16& other) const
    {
        return mValue < other.mValue;
    }

    s32 operator!=(const Fix16& other) const
    {
        return mValue != other.mValue;
    }

    s32 operator>=(const Fix16& other) const
    {
        return mValue >= other.mValue;
    }


    // MATCH_FUNC(0x509990)
    bool operator>=(const s32 value) const
    {
        return mValue >= value << 14;
    }

    // MATCH_FUNC(0x509990)
    bool operator<(const s32 value) const
    {
        return mValue < value << 14;
    }

    f32 AsFloat() const
    {
        return mValue / 16384.0f;
    }

    inline f64 AsDouble() const
    {
        return mValue / 16384.0;
    }

    inline s32 ToInt() const
    {
        return mValue >> 14;
    }

    inline u8 ToUInt8() const
    {
        return mValue >> 14;
    }

    inline void FromInt(s32 a1)
    {
        mValue = a1 << 14;
    }

    inline void FromShort(s16 a1)
    {
        mValue = a1 << 14;
    }

    inline void FromUnsignedShort(u16 a1)
    {
        mValue = a1 << 14;
    }

    float ToFloat() const
    {
        return (mValue / 16384.0f);
    }

    Fix16()
    {
    }

    Fix16(s32 value, u8) : mValue(value)
    {
    }

    Fix16(u8 value)
    {
        mValue = value << 14;
    }

    Fix16(u16 value)
    {
        mValue = value << 14;
    }

    Fix16(s16 value)
    {
        mValue = value << 14;
    }

    Fix16(u32 value)
    {
        mValue = value << 14;
    }

    Fix16(s32 value)
    {
        mValue = value << 14;
    }

    explicit Fix16(f32 v) : mValue(static_cast<s32>(v * 16384.0))
    {
    }

    explicit Fix16(f64 v) : mValue(static_cast<s32>(v * 16384.0))
    {
    }

    void FromU8(u8 v)
    {
        mValue = v << 14;
    }

    inline Fix16& Negate()
    {
        mValue = -mValue;
        return *this;
    }

    inline static Fix16 __stdcall Abs(Fix16& input)
    {
        if (input.mValue > 0)
        {
            return input;
        }
        else
        {
            return -input;
        }
    }

    inline Fix16 GetRoundValue() const
    {
        // get the "integer part" of Fix16, since everything less than 0x3FFF is decimal in float
        return Fix16(mValue & 0xFFFFC000, 0); // 0xFFFFC000 = 0xFFFFFFFF - Fix16(1)
    }

    inline Fix16 GetFracValue() const
    {
        // get the "fractional part" of Fix16
        return Fix16(mValue & 0x3FFF, 0); // 0x4000 = Fix16(1)
    }

    inline s32 MultiplyInt64(Fix16 a2)
    {
        __int64 t = (mValue * (__int64)a2.mValue) >> 14;
        return (s32)t;
    }

    //  inline sub_4B9E10 in 9.6f
    inline static bool IsBetween_4B9E10(Fix16& min, Fix16& max, Fix16& input)
    {
        return input >= min && input <= max;
    }

    //  inline sub_462ED0 in 9.6f
    static Fix16 ctor_462ED0(s16 a1)
    {
        return Fix16(a1 << 7, 0);
    }

    inline static Fix16 Max(const Fix16& diff_x, const Fix16& diff_y)
    {
        return (diff_x > diff_y) ? diff_x : diff_y;
    }

    
    // NOTE: Force required for sub_43A240 else 2nd call doesn't get inlined
    // miss2_0x11C.cpp switches this back to regular inlining mode. In 9.6f it seems like
    // that file actually does have other inline settings as it actually has inlined way more things in the
    // opcode switch case compared to 10.5 which has inlined nothing.
    INLINE_MODE static Fix16 __stdcall SquareRoot(Fix16& input)
    {
        return Fix16(sqrt(input.AsDouble()));
    }

    EXPORT Fix16& FromInt_4369F0(s32 a2);
    EXPORT Fix16& FromInt_45C4E0(u8 a2);
    EXPORT Fix16& FromU16_4AE970(u16 a2);

    EXPORT Fix16 Max_44E540(Fix16& pLhs, Fix16& pRhs);
    EXPORT inline static Fix16 __stdcall Abs_436A50(Fix16& a2);
    EXPORT inline static Fix16 __stdcall SquareRoot_436A70(Fix16& a2);
    EXPORT Fix16 operator+(const Fix16& rhs) const;

    //MATCH_FUNC(0x436A20)
    Fix16 operator/(const Fix16& in)
    {
        s32 value = (s32)(((__int64)mValue << 14) / in.mValue);
        return Fix16(value, 0);
    }

    // Inlined from 9.6f at 0x401bf0
    // I am not fully sure if this is right, i.e. the s32 parameter, instead of Fix16.
    // But I couldn't match Phi_74::sub_533090 without this overload.
    EXPORT Fix16 operator/(const s32& in)
    {
        s32 value = mValue / in;
        return Fix16(value, 0);
    }

    inline s32 get_value_4754D0() const
    {
        return mValue;
    }

    EXPORT static class Ang16 __stdcall atan2_fixed_405320(Fix16& pMaybeX_FP16, Fix16& pMaybeY_FP16);

  public:
    s32 mValue;
};

EXPORT bool __stdcall IntervalIntersectsRange_438FB0(const Fix16& intervalStart,
                                                     const Fix16& intervalEnd,
                                                     const Fix16& rangeMin,
                                                     const Fix16& rangeMax);

class Ang16;
EXPORT Ang16 __stdcall ArcTanLookup_405500(const Fix16& targetTan);

EXPORT void __stdcall FindMinMax_5A57E0(Fix16& minOut, Fix16& maxOut, const Fix16& v1, const Fix16& v2, const Fix16& v3, const Fix16& v4);

EXTERN_GLOBAL_ARRAY(Fix16, gSin_table_667A80, 1440);
EXTERN_GLOBAL_ARRAY(Fix16, gCos_table_669260, 1440);

EXPORT void Init_trigonometry_tables();
