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

    Fix16 operator-(const Fix16& in)
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

    bool operator>(const Fix16& other)
    {
        return mValue > other.mValue;
    }

    bool operator<(const Fix16& other)
    {
        return mValue < other.mValue;
    }

    bool operator!=(const Fix16& other)
    {
        return mValue != other.mValue;
    }

    bool operator>=(const Fix16& other)
    {
        return mValue >= other.mValue;
    }

    bool operator<=(const Fix16& other)
    {
        return mValue <= other.mValue;
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
        return mValue * 0.0000610351562f;
    }

    Fix16()
    {
    }

    Fix16(s32 value, u8) : mValue(value)
    {
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

    inline Fix16 ConcatenateWord(Fix16 a2)
    {
        Fix16 result;
        result.mValue = mValue + (a2.mValue & 0xFFFFC000);
        return result;
    }

    inline s32 MultiplyInt64(Fix16 a2)
    {
        __int64 t = (mValue * (__int64)a2.mValue) >> 14;
        return (s32)t;
    }

    //  inline div_401B90
    inline Fix16& inline_divide_by(Fix16& a1) 
    {
        mValue = ((__int64)mValue << 14) / a1.mValue;
        return *this;
    }

    EXPORT Fix16& FromInt_4369F0(s32 a2);
    EXPORT Fix16& FromInt_45C4E0(u8 a2);
    EXPORT Fix16& FromU16_4AE970(u16 a2);
   
    EXPORT Fix16 Max_44E540(Fix16& pLhs, Fix16& pRhs);
    EXPORT Fix16 Abs_436A50(Fix16& a2);
    EXPORT Fix16 SquareRoot_436A70(Fix16& a2);
    EXPORT Fix16 operator-();
    EXPORT Fix16 operator+(const Fix16& rhs) const;
    EXPORT Fix16 operator/(const Fix16& in);

  public:
    s32 mValue;
};

struct Fix16_Vec
{
    Fix16 x, y, z;
};

struct Fix16_2
{
    Fix16 field_0_full;
    Fix16 field_4_frac;
};
