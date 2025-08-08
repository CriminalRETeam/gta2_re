#pragma once

#include "Function.hpp"
#include <windows.h>

class Fix16
{
  public:
    bool operator==(const Fix16& value) const
    {
        return mValue == value.mValue;
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

    Fix16 operator+=(const Fix16& other)
    {
        mValue += other.mValue;
        return *this;
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

    // Inlined from 9.6f from 0x4086A0
    Fix16 operator-()
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

    s32 operator<=(const Fix16& other) const
    {
        return mValue <= other.mValue;
    }

    // Inlined from 9.6f from 0x509990
    bool operator>=(const s32 value) const
    {
        return mValue >= value << 14;
    }

    // Inlined from 9.6f from 0x509990
    bool operator<(const s32 value) const
    {
        return mValue < value << 14;
    }

    f32 AsFloat() const
    {
        return mValue / 16384.0f;
    }

    f64 AsDouble() const
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
        return mValue / 16384.0f;
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

    void FromU8(u8 v)
    {
        mValue = v << 14;
    }

    inline Fix16& Negate()
    {
        mValue = -mValue;
        return *this;
    }

    inline Fix16& inline_abs_403840(Fix16& input)
    {
        if (input.mValue <= 0)
        {
            *this = input.Negate();
        }
        else
        {
            *this = input;
        }
        return *this;
    }

    // OBS: obsolete, I will remove it soon
    inline Fix16 ConcatenateWord(Fix16 a2)
    {
        Fix16 result;
        result.mValue = mValue + (a2.mValue & 0xFFFFC000);
        return result;
    }

    inline Fix16 GetRoundValue()
    {
        // get the "integer part" of Fix16, since everything less than 0x3FFF is decimal in float
        return Fix16(mValue & 0xFFFFC000, 0); // 0xFFFFC000 = 0xFFFFFFFF - Fix16(1)
    }

    inline Fix16 GetFracValue()
    {
        // get the "fractional part" of Fix16
        return Fix16(mValue & 0x3FFF, 0); // 0x4000 = Fix16(1)
    }

    inline s32 MultiplyInt64(Fix16 a2)
    {
        __int64 t = (mValue * (__int64)a2.mValue) >> 14;
        return (s32)t;
    }

    //  inline div_401B90
    inline Fix16& inline_divide_by(Fix16& a1) 
    {
        mValue = (s32)(((__int64)mValue << 14) / a1.mValue);
        return *this;
    }

    //  inline sub_4B9E10 in 9.6f
    inline static bool inline_is_between(Fix16& a1, Fix16& a2, Fix16& a3)
    {
        return a3 >= a1 && a3 <= a2;
    }

    //  inline sub_462ED0 in 9.6f
    static Fix16 ctor_462ED0(s16 a1)
    {
        return Fix16(a1 << 7, 0);
    }

    EXPORT Fix16& FromInt_4369F0(s32 a2);
    EXPORT Fix16& FromInt_45C4E0(u8 a2);
    EXPORT Fix16& FromU16_4AE970(u16 a2);
   
    EXPORT Fix16 Max_44E540(Fix16& pLhs, Fix16& pRhs);
    EXPORT Fix16 Abs_436A50(Fix16& a2);
    EXPORT Fix16 SquareRoot_436A70(Fix16& a2);
    EXPORT Fix16 operator+(const Fix16& rhs) const;

    // Inlined from 9.6f from 0x436A20
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

  public:
    s32 mValue;
};

// TODO: Some functions like DrawUnk_0xBC::sub_435A70 won't match unless this is a POD
// but 9.6f leads me to believe both the POD and non-POD type are the same
struct Fix16_Point_POD
{
    Fix16 x;
    Fix16 y;
}; 

class Fix16_Point : public Fix16_Point_POD
{
public:
    // Both inlined and exists as a function... some strange array init behaviour??
    ~Fix16_Point() {}

    // It needs to be in the header
    // Inlined from 9.6f from 0x563970
    Fix16_Point()
    {
    }

    // 9.6f 0x401D20
    Fix16_Point(const Fix16_Point& rhs)
    {
        x = (rhs.x);
        y = (rhs.y);
    }

    Fix16_Point(Fix16& a1, Fix16& a2)
    {
        x = a1;
        y = a2;
    }

    // MATCH_FUNC(0x40AC50)
    Fix16_Point operator+(const Fix16_Point& in)
    {
        return Fix16_Point(x + in.x, y + in.y);
    }

    // Inlined, on version 9.6f 0x41E1E0
    void reset()
    {
        x = Fix16(0);
        y = Fix16(0);
    }
};

struct Fix16_Vec
{
    Fix16 x, y, z;
};

EXTERN_GLOBAL_ARRAY(Fix16, gSin_table_667A80, 1440);
EXTERN_GLOBAL_ARRAY(Fix16, gCos_table_669260, 1440);