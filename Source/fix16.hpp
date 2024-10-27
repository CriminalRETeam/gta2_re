#pragma once

#include "types.hpp"

struct Fix16
{
    Fix16() : mValue(0) { }

    Fix16& operator = (s32 value)
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

    f32 AsFloat() const
    {
        return mValue / 16384.0f;
    }

    inline s32 ToInt()
    {
        return mValue >> 14;
    }

    EXPORT Fix16& FromInt_4369F0(s32 a2);

public:
    s32 mValue;
};
