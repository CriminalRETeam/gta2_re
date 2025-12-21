#pragma once

#include "Function.hpp"
#include "fix16.hpp"

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

    bool operator!=(const Ang16& other)
    {
        return rValue != other.rValue;
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

    bool operator==(const Ang16& other)
    {
        return rValue == other.rValue;
    }

    Ang16 operator*(const Ang16& other)
    {
        return Ang16((Fix16(rValue) * Fix16(other.rValue)).ToInt());
    }

    // 9.6f 0x40E5A0
    // https://decomp.me/scratch/Rc5ql
    Ang16 operator+(const Ang16& rhs)
    {
        return Ang16( rValue + rhs.rValue, 0);
    }

    Ang16 operator-(const Ang16& other)
    {
        return Ang16(rValue - other.rValue, 0);
    }

    Ang16 operator-()
    {
        return Ang16(-rValue, 0);
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

    inline void ConvertAndMultiply(Ang16* a1, Ang16* a2)
    {
        Fix16 fix_1;
        fix_1.mValue = a2->FromUnsignedToFloat();
        Fix16 fix_2;
        fix_2.mValue = a1->ToFloat();
        Fix16 fix_3;
        fix_3.mValue = fix_2.MultiplyInt64(fix_1);

        rValue = fix_3.ToInt();
    }

    EXPORT void sub_406C20();

    // 9.6f 0x401C10
    // https://decomp.me/scratch/bB2VJ
    void Normalize()
    {
        for (; rValue < 0; rValue += 1440)
        {
            ;
        }
        for (; rValue >= 1440; rValue -= 1440)
        {
            ;
        }
    }

    EXPORT Ang16* sub_409300(Ang16* a2, s32 a3);
    EXPORT Ang16* sub_409340(Ang16* pRet, Ang16* toSub);
    EXPORT Ang16* sub_482740(Ang16* a1, s32* a2);
    EXPORT Ang16* sub_4516B0(s32* a2, s32 a3);

    inline static Fix16 __stdcall cosine_40F520(const Ang16& angle)
    {
        return gCos_table_669260[angle.rValue];
    }

    inline static Fix16 __stdcall sine_40F500(const Ang16& angle)
    {
        return gSin_table_667A80[angle.rValue];
    }

    EXPORT static s32 __stdcall Ang16::GetAngleFace_4F78F0(Ang16& a1);

    // inlined from 0x40f580 on v9.6f
    // I was tempted to put this on the Fix16 header, but that will create a circular dependency...
    EXPORT static Fix16 Ang16_to_Fix16(const Ang16 &v)
    {
        s32 value = v.rValue * 71;
        return Fix16(value, 0);
    }

    // 9.6f 0x401C60
    // https://decomp.me/scratch/0qgcp
    Ang16(const s16& value, s32 not_used)
     : rValue(value)
    {
        this->Normalize();
    }


    // 9.6f 0x40E590
    Ang16() : rValue(0)
    {
    }

    Ang16(s16 value, u8 null2) : rValue(value)
    {
        Normalize();
    }

    Ang16(s32 value) : rValue(value)
    {
    }

    Ang16(Fix16 value) : rValue(value.mValue >> 14) // value.ToInt() breaks Map_0x370::sub_4DFB90
    {
    }

    s16 rValue;
};

class Ang8
{
  public:
    //  maybe an Ang8 inline at 0x462EF0 in 9.6f
    Ang16 convert_to_ang16()
    {
        return Ang16((Fix16(rValue) / 256) * 1440);
    }

    u8 rValue;
};