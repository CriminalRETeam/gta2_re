#pragma once

#include "types.hpp"

struct Fix16
{
    Fix16() : mValue(0) {}

    Fix16 &operator=(s32 value)
    {
        mValue = value;
        return *this;
    }

    Fix16 operator-(const Fix16 &in)
    {
        Fix16 t;
        t.mValue = mValue - in.mValue;
        return t;
    }

    Fix16 operator-=(const Fix16 &other)
    {
        mValue -= other.mValue;
        return *this;
    }

    Fix16 operator+(const Fix16 &in)
    {
        Fix16 t;
        t.mValue = mValue + in.mValue;
        return t;
    }

    Fix16 operator+=(const Fix16 &other)
    {
        mValue += other.mValue;
        return *this;
    }

    bool operator>(const Fix16 &other)
    {
        return mValue > other.mValue;
    }

    bool operator<(const Fix16 &other)
    {
        return mValue < other.mValue;
    }

    f32 AsFloat() const
    {
        return mValue / 16384.0f;
    }

    inline s32 ToInt()
    {
        return mValue >> 14;
    }

    EXPORT Fix16 &FromInt_4369F0(s32 a2);
    EXPORT Fix16 &FromInt_45C4E0(u8 a2);

    Fix16 operator*(const Fix16 &in);

public:
    s32 mValue;
};
