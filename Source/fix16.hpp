#pragma once

#include "Function.hpp"

struct Fix16
{
    Fix16& operator=(s32 value)
    {
        mValue = value;
        return *this;
    }

    Fix16 operator-(const Fix16& in)
    {
        Fix16 t;
        t.mValue = mValue - in.mValue;
        return t;
    }

    Fix16 operator-=(const Fix16& other)
    {
        mValue -= other.mValue;
        return *this;
    }

    Fix16 operator+(const Fix16& in)
    {
        Fix16 t;
        t.mValue = mValue + in.mValue;
        return t;
    }

    Fix16 operator+=(const Fix16& other)
    {
        mValue += other.mValue;
        return *this;
    }

    bool operator>(const Fix16& other)
    {
        return mValue > other.mValue;
    }

    bool operator<(const Fix16& other)
    {
        return mValue < other.mValue;
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

    EXPORT Fix16& FromInt_4369F0(s32 a2);
    EXPORT Fix16& FromInt_45C4E0(u8 a2);
    EXPORT Fix16 Max_44E540(Fix16& pLhs, Fix16& pRhs);
    EXPORT Fix16 Abs_436A50(Fix16& a2);
    EXPORT Fix16 SquareRoot_436A70(Fix16& a2);

    EXPORT Fix16 operator*(const Fix16& in);

    EXPORT Fix16 operator-();

    EXPORT Fix16 operator+(const Fix16& rhs) const;
    EXPORT Fix16 operator/(const Fix16& in);

  public:
    s32 mValue;
};


struct Fix16_Vec
{
  Fix16 x,y,z;
};

