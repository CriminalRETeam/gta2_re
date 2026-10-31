#pragma once

#include "Function.hpp"
#include <math.h>
#include <windows.h>
#include <assert.h>

#ifndef INLINE_MODE
    #define INLINE_MODE __forceinline
#endif

class Fix16;

EXTERN_GLOBAL(Fix16, kFP16Zero_6FE20C);
EXTERN_GLOBAL(Fix16, kFPZero_6691B0);
EXTERN_GLOBAL(Fix16, kFpZero_6F8E10);
EXTERN_GLOBAL(Fix16, kFP16One256th_6FE07C);
EXTERN_GLOBAL(Fix16, kFpOne256th_6F8CF0);

class Fix16
{
  public:
    // 9.6f func
    static inline s32 __stdcall Round_To_Int_410BF0(Fix16& input)
    {
        s32 v = input.mValue;
        return (v + 0x2000) >> 14;
    }


    // TODO: BIG HACK!!! Makes no sense for this to be a method but its the only way to force certain inlining behaviour
    // ideally we'll figure out how to get the inlining we need without doing this in the future.
    bool __stdcall IntervalIntersectsRange_438FB0_inline(const Fix16& intervalEnd, const Fix16& rangeMin, const Fix16& rangeMax) const
    {
        if (*this < rangeMin)
        {
            if (intervalEnd < rangeMin)
            {
                return false;
            }
            return true;
        }
        else
        {
            if (*this <= rangeMax)
            {
                return true;
            }
            return false;
        }
    }

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

    // The compiled-out asserts and the casts count in VC6's inline size: keep them (Scripts/usm_44e560/README.md)
    Fix16 operator-(const Fix16& in) const
    {
        assert(static_cast<__int64>(mValue) - in.mValue <= 0x7FFFFFFF);
        assert(static_cast<__int64>(mValue) - in.mValue >= -0x7FFFFFFF - 1);
        return Fix16(static_cast<s32>(mValue - in.mValue), 0);
    }

    Fix16& operator-=(const Fix16& other)
    {
        mValue -= other.mValue;
        return *this;
    }

    Fix16 operator+(const Fix16& in) const
    {
        assert(static_cast<__int64>(mValue) + in.mValue <= 0x7FFFFFFF);
        assert(static_cast<__int64>(mValue) + in.mValue >= -0x7FFFFFFF - 1);
        return Fix16(static_cast<s32>(mValue + in.mValue), 0);
    }

    Fix16& operator+=(const Fix16& other)
    {
        assert(static_cast<__int64>(mValue) + other.mValue <= 0x7FFFFFFF);
        assert(static_cast<__int64>(mValue) + other.mValue >= -0x7FFFFFFF - 1);
        mValue += other.mValue;
        return *this;
    }

    Fix16 operator*(const Fix16& in) const
    {
        assert(((mValue * static_cast<__int64>(in.mValue)) >> 14) <= 0x7FFFFFFF);
        assert(((mValue * static_cast<__int64>(in.mValue)) >> 14) >= -0x7FFFFFFF - 1);
        return Fix16(static_cast<s32>(static_cast<s32>((mValue * static_cast<__int64>(in.mValue)) >> 14)), 0);
    }

    // 10.5 non inline addr is 0x539F90
    Fix16& operator/=(const Fix16& rhs)
    {
        mValue = static_cast<s32>((static_cast<__int64>(mValue) << 14) / rhs.mValue);
        return *this;
    }

    // 10.5 non inline addr is 0x562430
    Fix16& operator*=(const Fix16& rhs)
    {
        mValue = static_cast<s32>((mValue * static_cast<__int64>(rhs.mValue)) >> 14);
        return *this;
    }

    // 10.5 is 0x561DB0
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

    // 9.6f 0x410BA0, defined in CarPhysics.cpp (AsDouble with the parentheses of the original: each
    // pair is a no-op node for VC6's x87 scheduler, see Scripts/x87_sched/README.md)
    inline f64 to_float_410BA0() const;

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

    inline void FromInt(s32 value)
    {
        mValue = value << 14;
    }

    inline void FromShort(s16 value)
    {
        mValue = value << 14;
    }

    inline void FromUnsignedShort(u16 value)
    {
        mValue = value << 14;
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

    explicit Fix16(f32 v) : mValue(static_cast<s32>(v * 16384.0f))
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

    // 9.6f 0x403840, out-of-line copy Abs_436A50. No braces and a const& parameter on purpose: VC6's
    // inline budget sees the front-end size, 57 here (each brace pair adds 2, the else 2 more, a non-const
    // parameter 1 less). 57 is the only size that gives the original's cut-offs both in MaxAbsDistance_42A6B0
    // (Ped::NotifyWeaponHit_46FF00 needs <= 57) and in GetLength_41E260 (Car::ManageDrowning_43E560 >= 57).
    inline static Fix16 __stdcall Abs(const Fix16& input)
    {
        if (input.mValue > 0)
            return input;
        return -input;
    }

    inline Fix16 ZeroIfNegligible_482730() const
    {
        if (Fix16::Abs(*this) < kFpOne256th_6F8CF0)
        {
            return kFpZero_6F8E10;
        }
        return *this;
    }

    inline Fix16 GetFloorValue() const
    {
        // get the "integer part" of Fix16, since everything less than 0x3FFF is decimal in float
        return Fix16(mValue & 0xFFFFC000, 0); // 0xFFFFC000 = 0xFFFFFFFF - Fix16(1)
    }

    // 9.6f 0x42A630 as a static taking the value by reference; defined in CarPhysics.cpp (UpdateZPhysics_55AD90)
    static Fix16 __stdcall GetFracValue_42A630(const Fix16& v);

    // 9.6f func: 0x42A630
    inline Fix16 GetFracValue() const
    {
        // get the "fractional part" of Fix16
        return Fix16(mValue & 0x3FFF, 0); // 0x4000 = Fix16(1)
    }

    inline s32 MultiplyInt64(Fix16 rhs) const
    {
        __int64 t = (mValue * static_cast<__int64>(rhs.mValue)) >> 14;
        return static_cast<s32>(t);
    }

    // 9.6f 0x4B9E10
    inline static bool IsBetween_4B9E10(Fix16& min, Fix16& max, Fix16& input)
    {
        return input >= min && input <= max;
    }

    // 9.6f 0x462ED0
    static Fix16 ctor_462ED0(s16 value)
    {
        return Fix16(value << 7, 0);
    }

    // Non-member style add of two references. Unlike the member operator+ it loads the left
    // operand into the result register first (CarPhysics::ComputeEngineTorque_561970's length).
    inline static Fix16 Add_ref(const Fix16& lhs, const Fix16& rhs)
    {
        return Fix16(lhs.mValue + rhs.mValue, 0);
    }

    inline static Fix16 Max(const Fix16& diff_x, const Fix16& diff_y)
    {
        return (diff_x > diff_y) ? diff_x : diff_y;
    }

    // 9.6f 0x410C10, out-of-line copy SquareRoot_436A70. A plain inline: big functions call 0x436A70 once
    // they run out of inline budget (Car::ManageDrowning_43E560). The const local is on purpose: VC6's
    // inline budget charges the front-end size, 48 here (41 without the local, 46 with a non-const one).
    // Car::ApplyExplosionImpulse_443710 needs >= 48 and CarPhysics::EnforceGearSensitiveMaxSpeed_562D00
    // <= 49; with GetLength_41E260 at 162, Crane::ComputeHookPolar_47F6C0 needs >= 48 too.
    inline static Fix16 __stdcall SquareRoot(Fix16& input)
    {
        const f64 value = input.AsDouble();
        return Fix16(sqrt(value));
    }

    // SquareRoot forced inline, only for Fix16_Point::GetLength_SqrtForced_43A240 (unexplained)
    __forceinline static Fix16 __stdcall SquareRoot_forced(Fix16& input)
    {
        return Fix16(sqrt(input.AsDouble()));
    }

    EXPORT static Fix16 __stdcall Max_44E540(const Fix16& lhs, const Fix16& rhs);
    // 9.6f 0x41E130, out-of-line copy Max_44E540. Small functions inline it (Ped_List::FindClosestActivePed_471340),
    // and as a site after the two Abs it sets their nested budget in MaxAbsDistance_42A6B0
    inline static Fix16 __stdcall Max_41E130(Fix16& a, Fix16& b)
    {
        if (a > b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }
    EXPORT static Fix16 __stdcall Abs_436A50(const Fix16& input);
    EXPORT static Fix16 __stdcall SquareRoot_436A70(const Fix16& input) throw();
    // throw(): the original calls these out-of-line copies without an EH frame (their inline
    // bodies were visible there), see CarPhysics::UpdateReferencePoint_563460
    // Out-of-line copy of operator+ (0x408660)
    EXPORT Fix16 Add_408660(const Fix16& rhs) const throw();
    EXPORT Fix16 Multiply_408680(const Fix16& in) const throw();
    // Out-of-line copies of operators, which big functions call once they run out of inline
    // expansions (SpriteShape::DrawCollisionBox_5A4DA0)
    EXPORT Fix16 Subtract_436A00(const Fix16& in) const;
    EXPORT Fix16 Divide_436A20(const Fix16& in) const;
    EXPORT s32 IsLess_451670(const Fix16& other) const;
    EXPORT s32 IsGreater_451690(const Fix16& other) const;
    EXPORT Fix16& DivideAssign_539F90(const Fix16& rhs);
    EXPORT Fix16 MultiplyInt_561DB0(const s32& in) const;
    EXPORT Fix16 DivideInt_53E860(const s32& in) const;
    EXPORT Fix16& MultiplyAssign_562430(const Fix16& rhs);
    EXPORT Fix16 Negate_4086A0() const throw();

    // Needed this for a GetLength variant used by ScriptThread::GetSpeed_50E190.
    inline static Fix16 __stdcall Abs_negate_out_of_line(Fix16& input)
    {
        if (input.mValue > 0)
        {
            return input;
        }

        return input.Negate_4086A0();
    }

    //MATCH_FUNC(0x436A20)
    Fix16 operator/(const Fix16& in) const
    {
        s32 value = static_cast<s32>((static_cast<__int64>(mValue) << 14) / in.mValue);
        return Fix16(value, 0);
    }

    // Inlined from 9.6f at 0x401bf0
    // I am not fully sure if this is right, i.e. the s32 parameter, instead of Fix16.
    // But I couldn't match ObjectDefinition::SetDimensionsFromSprite_533090 without this overload.
    EXPORT Fix16 operator/(const s32& in) const
    {
        s32 value = mValue / in;
        return Fix16(value, 0);
    }

    inline s32 get_value_4754D0() const
    {
        return mValue;
    }

    inline s32 GetRaw_40F4B0() const
    {
        return mValue;
    }

    Fix16 ApplyDeadZone_482730(Fix16 to_abs) const
    {
        if (!(Fix16::Abs(to_abs) < kFP16One256th_6FE07C))
        {
            return to_abs;
        }
        else
        {
            return kFP16Zero_6FE20C;
        }
    }

    // https://decomp.me/scratch/MqQPJ
    inline static Fix16 __stdcall MaxAbsDistance_42A6B0(const Fix16& x1, const Fix16& y1, const Fix16& x2, const Fix16& y2)
    {
        Fix16 diff_x;
        diff_x = x2 - x1;
        Fix16 diff_y;
        diff_y = y2 - y1;
        Fix16 result;
        result = Fix16::Max_41E130(Fix16::Abs(diff_x), Fix16::Abs(diff_y));
        return result;
    }

    // Out-of-line copy of the inline below, at its address (callers use the inline, which VC6 emits as
    // its own COMDAT copy). Defined in CarPhysics.cpp.
    EXPORT static Fix16 __stdcall Max3_ool_55EEE0(Fix16& first, Fix16& second, Fix16& third);

    // NOTE: 10.5 function - matched but inlined
    static inline Fix16 __stdcall Max3_55EEE0(Fix16& first, Fix16& second, Fix16& third)
    {
        if (first > second)
        {
            if ((first <= third))
            {
                return third;
            }
            else
            {
                return first;
            }
        }
        else
        {
            if (second > third)
            {
                return second;
            }
            else
            {
                return third;
            }
        }
    }

    inline Fix16* subtract_one_491F00()
    {
        mValue -= 0x4000;
        return this;
    }

    inline Fix16* add_one_491EF0()
    {
        mValue += 0x4000;
        return this;
    }

    // 9.6f 0x4824E0
    inline Fix16 operator++(int)
    {
        mValue += 0x4000;
        return Fix16(mValue - 0x4000, 0);
    }

    // 9.6f 0x482510
    inline Fix16 operator--(int)
    {
        mValue -= 0x4000;
        return Fix16(mValue + 0x4000, 0);
    }

    EXPORT static class Ang16 __stdcall atan2_fixed_405320(Fix16& y, Fix16& x);

  public:
    s32 mValue;
};

EXPORT bool __stdcall IntervalIntersectsRange_438FB0(const Fix16& intervalStart,
                                                     const Fix16& intervalEnd,
                                                     const Fix16& rangeMin,
                                                     const Fix16& rangeMax);

class Ang16;
EXPORT Ang16 __stdcall ArcTanLookup_405500(const Fix16& targetTan);

EXPORT void __stdcall FindMinMax_5A57E0(Fix16& minOut, Fix16& maxOut, const Fix16& a, const Fix16& b, const Fix16& c, const Fix16& d);

EXPORT Fix16 __stdcall TurnAngleToward_405DA0(Fix16 cur, const Fix16* pTarget, const Fix16* pSpeed);
EXPORT s32 __stdcall ClampAngleAroundTarget_405E80(const Fix16* pTarget, Fix16* pCur);
EXTERN_GLOBAL_ARRAY(Fix16, gSin_table_667A80, 1440);
EXTERN_GLOBAL_ARRAY(Fix16, gCos_table_669260, 1440);

