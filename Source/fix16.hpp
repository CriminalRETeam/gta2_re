#pragma once

#include "Function.hpp"

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

    explicit Fix16(f32 v) : mValue(static_cast<s32>(v * 16384.0))
    {
    }

    void FromU8(u8 v)
    {
        mValue = v << 14;
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
