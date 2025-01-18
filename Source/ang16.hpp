#pragma once

#include "Function.hpp"

class Ang16
{
  public:
    //   Ang16  and  Ang16
    bool operator>(const Ang16& other)
    {
        return rValue > other.rValue;
    }

    bool operator<(const Ang16& other)
    {
        return rValue < other.rValue;
    }

    bool operator>=(const Ang16& other)
    {
        return rValue >= other.rValue;
    }

    bool operator<=(const Ang16& other)
    {
        return rValue <= other.rValue;
    }

    Ang16 operator-=(const Ang16& other)
    {
        rValue -= other.rValue;
        return *this;
    }

    Ang16 operator+=(const Ang16& other)
    {
        rValue += other.rValue;
        return *this;
    }

    Ang16 operator=(const Ang16& other)
    {
        rValue = other.rValue;
        return *this;
    }

    //   Ang16  and  const int

    bool operator>(const s32 other)
    {
        return rValue > other;
    }

    bool operator<(const s32 other)
    {
        return rValue < other;
    }

    bool operator>=(const s32 other)
    {
        return rValue >= other;
    }

    bool operator<=(const s32 other)
    {
        return rValue <= other;
    }

    Ang16 operator-=(const s32 other)
    {
        rValue -= other;
        return *this;
    }

    Ang16 operator+=(const s32 other)
    {
        rValue += other;
        return *this;
    }

    Ang16 operator<<(const s32 other)
    {
        rValue = rValue << other;
        return *this;
    }

    Ang16 operator>>(const s32 other)
    {
        rValue = rValue >> other;
        return *this;
    }

    Ang16 operator*(const s32 other)
    {
        rValue *= other;
        return *this;
    }

    Ang16 operator=(const s32 other)
    {
        rValue = other;
        return *this;
    }

    Ang16 operator-(const s32 other)
    {
        rValue -= other;
        return *this;
    }
    
    inline s32 ToFloat()
    {
        return rValue << 14;
    }

    inline s32 FromUnsignedToFloat()
    {
        return (u16)rValue << 14;
    }

    EXPORT void sub_406C20();
    inline void Normalize()
    {
        for (; *this < 0; *this += 1440)
        {
            ;
        }
        for (; *this >= 1440; *this -= 1440)
        {
            ;
        }
    };

    EXPORT Ang16* sub_409300(Ang16* a2, s32 a3);
    EXPORT Ang16* sub_409340(Ang16* pRet, Ang16* toSub);
    EXPORT Ang16* sub_482740(Ang16* a1, s32* a2);
    EXPORT Ang16* sub_4516B0(s32* a2, s32 a3);

    Ang16() : rValue(0)
    {
    }

    Ang16(s32 value) : rValue(value)
    {
    }

    s16 rValue;
};
