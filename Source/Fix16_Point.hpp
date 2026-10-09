#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

EXTERN_GLOBAL(Fix16, gFix16_6777CC);
EXTERN_GLOBAL(Fix16, kFP16Zero_6FE20C);
EXTERN_GLOBAL(Fix16, kFP16One256th_6FE07C);
EXTERN_GLOBAL(Fix16, kFpZero_6F77C0);

// GetLength_41E260 compares against a zero constant that each TU of the original has its own copy of
// (a header static, like 9.6f's single 0x5E3DC8 in vec_len 0x41E260). A TU whose copy isn't
// gFix16_6777CC defines FIX16_POINT_ZERO to it before its first include.
#ifndef FIX16_POINT_ZERO
    #define FIX16_POINT_ZERO gFix16_6777CC
#endif
EXTERN_GLOBAL(Fix16, kFpZero_6F610C);
EXTERN_GLOBAL(Fix16, kF16Zero_677B90);
EXTERN_GLOBAL(Fix16, dword_706EB8);
EXTERN_GLOBAL(Fix16, kFP16Zero_6FD9E4);
EXTERN_GLOBAL(Fix16, kZero_676818);
EXTERN_GLOBAL(Fix16, kFpZero_7064C0);
EXTERN_GLOBAL(Fix16, kZero_679E70);

class Fix16_Point;

// A point without a destructor. 10.5 has points that don't take part in EH unwinding: struct members
// (~Sprite is a bare jmp, the Char_B4 ctor's EH states leave out field_98_velocity_vector), a by-value
// parameter the caller copies as two dwords (sub_5DE910) and a few locals (CarPhysics_B0 0x55F280,
// 0x55F800, Camera_0xBC::WorldToScreen_40CFC0's result). Everything else is a Fix16_Point. It is a
// separate type, not a base of Fix16_Point: 10.5 charges Fix16_Point's ctor as one size-42 inline
// site at the top level, which a derived class's ctor (base ctor nested in it) doesn't reproduce.
// 9.6f doesn't tell the two apart.
struct Fix16_Point_POD
{
    Fix16_Point_POD()
    {
    }

    Fix16_Point_POD(Fix16& a1, Fix16& a2)
    {
        x = a1;
        y = a2;
    }

    void SetXY_432860(Fix16& a2, Fix16& a3)
    {
        this->x = a2;
        this->y = a3;
    }

    inline void SetFromPolar_41E210(Fix16& radius, Ang16& angle)
    {
        x = Ang16::sine_40F500(angle) * radius;
        y = Ang16::cosine_40F520(angle) * radius;
    }

    // Matching impl at RotateVelocity_562C20
    inline void RotateByAngle_40F6B0(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        Fix16 x_old = x;

        x = (x * cos) + (y * sin);
        y = ((-x_old) * sin) + (y * cos);
    }

    Fix16_Point_POD& operator+=(Fix16_Point& other);

    // Same layout: a POD passed where a Fix16_Point is expected (a free inline site, like the other way)
    operator Fix16_Point&()
    {
        return *(Fix16_Point*)this;
    }

    Fix16 x;
    Fix16 y;
};

class Fix16_Point
{
  public:
    void SetXY_432860(Fix16& a2, Fix16& a3)
    {
        this->x = a2;
        this->y = a3;
    }

    // Reads the TU's FIX16_POINT_ZERO copy (Weapon_30::throwable_5DDFC0 compares with 0x706EB8)
    inline bool IsNull_420360() const
    {
        return x == FIX16_POINT_ZERO && y == FIX16_POINT_ZERO;
    }

    // For some reason uses another constant
    inline bool IsNull() const
    {
        return x == kFP16Zero_6FE20C && y == kFP16Zero_6FE20C;
    }

    // 9.6f 0x49E450
    inline bool HasZeroComponent_49E450() const
    {
        return x == kFP16Zero_6FE20C || y == kFP16Zero_6FE20C;
    }

    void ApplyDeadZone_49E3C0()
    {
        Fix16 total = (Fix16::Abs(x) + Fix16::Abs(y));
        if (total < kFP16One256th_6FE07C)
        {
            x = kFP16Zero_6FE20C;
            y = kFP16Zero_6FE20C;
        }
    }

    // But also 0x40ACD0 non inlined in 10.5
    Ang16 atan2_40F790()
    {
        return Fix16::atan2_fixed_405320(y, x);
    }

    Fix16 GetLength_453590();

    // 9.6f 0x41E260; the out-of-line copy is GetLength_453590. The nested if/else (front-end size 162, the
    // else-if chain is 160) gives the original's cut-offs in Crane_15C::ComputeHookPolar_47F6C0 with
    // SquareRoot at 48; matched functions allow 159..162.
    inline Fix16 GetLength_41E260()
    {
        if (x == FIX16_POINT_ZERO)
        {
            return Fix16::Abs(y);
        }
        else
        {
            if (y == FIX16_POINT_ZERO)
            {
                return Fix16::Abs(x);
            }
            else
            {
                return Fix16::SquareRoot(x * x + y * y);
            }
        }
    }

    // GetLength_41E260 as inlined twice into Car_BC::GetCarLinearSpeed_43A240: the original inlines both
    // multiplies, the add and SquareRoot but calls Negate_4086A0 for all four Abs. No helper/caller shape found
    // that gives that with the plain inline (unexplained, no 9.6f copy of the function).
    inline Fix16 GetLength_SqrtForced_43A240()
    {
        if (x == FIX16_POINT_ZERO)
        {
            return Fix16::Abs_negate_out_of_line(y);
        }
        else if (y == FIX16_POINT_ZERO)
        {
            return Fix16::Abs_negate_out_of_line(x);
        }
        else
        {
            return Fix16::SquareRoot_forced(x * x + y * y);
        }
    }

    inline void SetFromPolar_41E210(Fix16& radius, Ang16& angle)
    {
        x = Ang16::sine_40F500(angle) * radius;
        y = Ang16::cosine_40F520(angle) * radius;
    }

    // RotateByAngle_40F6B0 in a function that ran out of inline expansions: every operator but
    // the first + is called out of line (CarPhysics_B0::SpawnSkidSegment_55D200)
    inline void RotateByAngle_40F6B0_out_of_line(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        Fix16 x_old = x;

        x = x.Multiply_408680(cos) + y.Multiply_408680(sin);
        y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(y.Multiply_408680(cos));
    }

    // RotateByAngle_40F6B0 in a function whose inline budget ran out after the first multiply
    // (y * sin, evaluated first): the rest are the out-of-line copies (Car_BC::SpawnDamageFireEffect_43B870)
    inline void RotateByAngle_OneMulInline_40F6B0(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        Fix16 x_old = x;

        x = x.Multiply_408680(cos).Add_408660(y * sin);
        y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(y.Multiply_408680(cos));
    }

    // RotateByAngle_40F6B0 with every operator called out of line (Particle_8::EmitImpactParticles_53FE40)
    inline void RotateByAngle_40F6B0_all_out_of_line(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        Fix16 x_old = x;

        x = x.Multiply_408680(cos).Add_408660(y.Multiply_408680(sin));
        y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(y.Multiply_408680(cos));
    }

    // Matching impl at RotateVelocity_562C20
    inline void RotateByAngle_40F6B0(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        Fix16 x_old = x;

        x = (x * cos) + (y * sin);
        y = ((-x_old) * sin) + (y * cos);
    }

    // RotateByAngle_40F6B0 as big functions get it once they run out of inline expansions:
    // the Fix16 operators are the out-of-line copies (Car_BC::HandleCarHitByObject_43F130)
    inline void RotateByAngle_OOL_40F6B0(const Ang16& angle)
    {
        Fix16 x_old = x;
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        x = x.Multiply_408680(cos).Add_408660(y.Multiply_408680(sin));
        y = (-x_old).Multiply_408680(sin).Add_408660(y.Multiply_408680(cos));
    }

    // As above, with the unary minus out of line too
    inline void RotateByAngle_NegOOL_40F6B0(const Ang16& angle)
    {
        Fix16 x_old = x;
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        x = x.Multiply_408680(cos).Add_408660(y.Multiply_408680(sin));
        y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(y.Multiply_408680(cos));
    }

    void FromPolar_41E210(const Fix16& radius, const Ang16& angle)
    {

        x = radius * Ang16::sine_40F500(angle);
        y = radius * Ang16::cosine_40F520(angle);
    }


    Fix16_Point& operator+=(Fix16_Point& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // FUNCTION: 96f 0x4828c0
    Fix16_Point& operator-=(Fix16_Point& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Operator* for Fix16 ?
    Fix16_Point& MultiplyByFix16_49E3A0(const Fix16& factor)
    {
        x *= factor;
        y *= factor;
        return *this;
    }

    // Out-of-line copy of RotateByAngle_40F6B0, emitted in CarPhysics_B0.cpp
    EXPORT void RotateVelocity_562C20(const Ang16& angle);

    EXPORT Fix16_Point Multiply_438FE0(Fix16& a1);
    EXPORT Fix16_Point Divide_442CB0(Fix16& a1);

    // Divide_442CB0 as a nothrow inline (see DivideInl_55F9E0; CarPhysics_B0::HandleObjectCollision_5606C0,
    // Car_BC::ApplyExplosionImpulse_443710)
    inline Fix16_Point DivideInl_442CB0(Fix16& in) throw()
    {
        return Fix16_Point(x / in, y / in);
    }

    // Multiply_438FE0 as a nothrow inline (see DivideInl_55F9E0; Car_BC::TryHitchTrailer_442810)
    inline Fix16_Point MultiplyInl_438FE0(Fix16& in) throw()
    {
        return Fix16_Point(x * in, y * in);
    }

    // Out-of-line copies emitted in Weapon_30.cpp (used by sub_5DE910).
    EXPORT Fix16_Point& AddAssign_5E40C0(const Fix16_Point& other);
    EXPORT Fix16_Point& DivAssign_5E40E0(const Fix16& v);
    EXPORT Fix16 MaxAbs_5E4140();

    // 9.6f 0x48A270 (the inline of MaxAbs_5E4140)
    inline Fix16 MaxAbs_48A270()
    {
        Fix16 ax = Fix16::Abs_436A50(x);
        Fix16 ay = Fix16::Abs_436A50(y);
        if (ax > ay)
        {
            return ax;
        }
        return ay;
    }

    // 9.6f 0x48A250: both components through Fix16::DivideAssign (10.5 0x539F90)
    inline void DivideAssign_48A250(const Fix16& d)
    {
        x.DivideAssign_539F90(d);
        y.DivideAssign_539F90(d);
    }

    // FUNCTION: 96f 0x41e1e0
    void reset()
    {
        x = Fix16(0);
        y = Fix16(0);
    }

    // Both inlined and exists as a function... some strange array init behaviour??
    ~Fix16_Point()
    {
    }

    // It needs to be in the header
    // MATCH_FUNC(0x563970)

    Fix16_Point()
    {
    }

    // No user-defined copy ctor: 9.6f copies points with plain movs (no call even at /Ob0), and the
    // implicit one does not use up VC6's inline budget (SpawnCabAndTrailerHelper_408370 needs that).

    // 9.6f 0x401D20
    Fix16_Point(const Fix16& a1, const Fix16& a2) : x(a1), y(a2)
    {
    }

    void ClampTowardsZero_49E480(const Fix16_Point& limit)
    {
        if (x >= kFP16Zero_6FE20C)
        {
            if (x > limit.x)
            {
                x = limit.x;
            }
        }
        else if (x < limit.x)
        {
            x = limit.x;
        }

        if (y >= kFP16Zero_6FE20C)
        {
            if (y > limit.y)
            {
                y = limit.y;
            }
        }
        else if (y < limit.y)
        {
            y = limit.y;
        }
    }

    // MATCH_FUNC(0x40AC50)
    Fix16_Point operator+(const Fix16_Point& in)
    {
        return Fix16_Point(x + in.x, y + in.y);
    }

    // MATCH_FUNC(0x40AC80)
    Fix16_Point operator-(const Fix16_Point& rhs)
    {
        return Fix16_Point(x - rhs.x, y - rhs.y);
    }

    // The out-of-line copy of the inline operator-. Called by name where the original keeps an EH state
    // for temporaries around the call (Crane_15C::HookPickupCar_47EF80, ComputeHookPolar_47F6C0); the
    // inline operator- called out of line gets none (CarPhysics_B0::HandleCarCollision_55FF20)
    EXPORT Fix16_Point Sub_40AC80(const Fix16_Point& rhs);

    // operator+/operator- with their bodies defined at the end of Car_BC.cpp (HandleCarHitByObject_43F130)
    inline Fix16_Point AddLate_40AC50(const Fix16_Point& in);
    inline Fix16_Point SubLate_40AC80(const Fix16_Point& rhs);

    // Out of line operator+ (CarPhysics_B0::SpawnSkidSegment_55D200)
    EXPORT Fix16_Point Add_40AC50(const Fix16_Point& in);

    // operator+ 0x40AC50 as a nothrow inline that VC6 still calls out of line: no EH state for the
    // temporaries alive across the call (Car_BC::TryHitchTrailer_442810)
    inline Fix16_Point AddInl_40AC50(const Fix16_Point& in) throw()
    {
        return Fix16_Point(x + in.x, y + in.y);
    }

    // Out of line unary minus (Object_2C::ResolveCollisionWithPed_5229B0)
    EXPORT Fix16_Point Negate_40ACB0() const;

    // Unused (ComputeHookPolar_47F6C0 calls GetLength_41E260 now). Kept: deleting any inline from this header
    // moves register tie-breaks in six matched MapRenderer functions (4EEE60..4F0030)
    inline Fix16 GetLength_no_sqrt_inline()
    {
        if (x == kFP16Zero_6FE20C)
        {
            return Fix16::Abs(y);
        }
        else if (y == kFP16Zero_6FE20C)
        {
            return Fix16::Abs(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x * x + y * y);
        }
    }

    // Unused (GetSpeed_50E190 matches with the plain GetLength_41E260), but don't remove it: without it
    // MapRenderer::Draw4SidedDiagonalUpLeft_4EF880 and Draw3SidedDiagonalDownRight_4EF520 stop matching
    // (see "Adding unused inline methods to a header" in docs/matching_quirks.md).
    inline Fix16 GetLength_all_out_of_line_abs_y_negate()
    {
        if (x == kFpZero_6F77C0)
        {
            return Fix16::Abs_negate_out_of_line(y);
        }
        else if (y == kFpZero_6F77C0)
        {
            return Fix16::Abs(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y.Multiply_408680(y)));
        }
    }

    // Needed for CarPhysics_B0::HandleCarCollision_55FF20.
    inline Fix16 GetLength_all_out_of_line_abs_y_negate_2()
    {
        if (x == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_negate_out_of_line(y);
        }
        else if (y == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_436A50(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y.Multiply_408680(y)));
        }
    }

    // Needed for CarPhysics_B0::ComputeEngineTorque_561970: named out-of-line multiplies, inline add (Abs goes
    // out of line by the inline budget).
    inline Fix16 GetLength_ool_abs_mul()
    {
        if (x == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_436A50(y);
        }
        else if (y == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_436A50(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(Fix16::Add_ref(x.Multiply_408680(x), y.Multiply_408680(y)));
        }
    }

    // Needed for CarPhysics_B0::SpawnSkidSegment_55D200.
    inline Fix16 GetLength_all_out_of_line_abs()
    {
        if (x == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_436A50(y);
        }
        else if (y == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_436A50(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y.Multiply_408680(y)));
        }
    }

    // Needed for CarPhysics_B0::ScarePedsOnDrivingFast_559C30 and UpdateSteeringAngle_562560.
    inline Fix16 GetLength_out_of_line_x_squared()
    {
        if (x == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_negate_out_of_line(y);
        }
        else if (y == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_436A50(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y * y));
        }
    }

    Fix16_Point operator*(Fix16& in)
    {
        return Fix16_Point(x * in, y * in);
    }

    Fix16_Point operator-() const
    {
        return Fix16_Point(-x, -y);
    }

    EXPORT Fix16_Point MultBy_442C80(const s32& factor);

    // 10.0 0x442CB0
    EXPORT Fix16_Point operator/(Fix16& in);

    // Unused (ApplyExplosionImpulse_443710 calls GetLength_41E260 now), kept for the same reason as
    // GetLength_no_sqrt_inline
    inline Fix16 GetLength_inline_443710()
    {
        if (x == gFix16_6777CC)
        {
            return Fix16::Abs(y);
        }
        else if (y == gFix16_6777CC)
        {
            return Fix16::Abs(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x * x + y * y);
        }
    }

    // GetLength_41E260 as inlined into NormalizeSafe_442AD0 (out of line helpers where the inline budget ran out)
    inline Fix16 GetLength_inline_442AD0()
    {
        if (x == gFix16_6777CC)
        {
            return Fix16::Abs_negate_out_of_line(y);
        }
        else if (y == gFix16_6777CC)
        {
            return Fix16::Abs_436A50(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y * y));
        }
    }

    // MultiplyByFix16_49E3A0 as inlined into CarPhysics_B0::CalculateRearWheelForce_5620D0: the
    // second *= is the out-of-line copy
    void MultiplyByFix16_inline_5620D0(const Fix16& factor)
    {
        x *= factor;
        y.MultiplyAssign_562430(factor);
    }

    // GetLength_41E260 as inlined into CarPhysics_B0::CalculateRearWheelForce_5620D0: Abs and x*x out of line
    // by the inline budget, y*y written out inline (the plain y * y changes the function)
    inline Fix16 GetLength_inline_5620D0()
    {
        if (x == kFP16Zero_6FE20C)
        {
            return Fix16::Abs(y);
        }
        else if (y == kFP16Zero_6FE20C)
        {
            return Fix16::Abs(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x * x + Fix16((s32)((y.mValue * (__int64)y.mValue) >> 14), 0));
        }
    }

    // Same, for the scaled point in NormalizeSafe_442AD0
    inline Fix16 GetLength_scaled_inline_442AD0()
    {
        if (x == gFix16_6777CC)
        {
            return Fix16::Abs_negate_out_of_line(y);
        }
        else if (y == gFix16_6777CC)
        {
            return Fix16::Abs_negate_out_of_line(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y.Multiply_408680(y)));
        }
    }

    EXPORT Fix16_Point NormalizeSafe_442AD0();

    EXPORT Ang16 atan2_40ACD0();

    EXPORT Fix16_Point Rotate90CCW_5605E0();

    void clear_41E1E0()
    {
        x = 0;
        y = 0;
    }

    EXPORT Fix16_Point operator/(const s32& a3);

    // operator/ 0x55F9E0 as a nothrow inline that VC6 still calls out of line: the original sets no EH
    // state for the temporaries that live across this call (CarPhysics_B0::HandleCarCollision_55FF20,
    // HandleObjectCollision_5606C0). throw() on the real operator/ changes 6 matched functions.
    inline Fix16_Point DivideInl_55F9E0(const s32& a3) throw()
    {
        return Fix16_Point(x / a3, y / a3);
    }

    // Same layout: a point sliced into a Fix16_Point_POD (one free inline site, see sub_5DF270)
    operator Fix16_Point_POD&()
    {
        return *(Fix16_Point_POD*)this;
    }

    Fix16 x;
    Fix16 y;
};

inline Fix16_Point_POD& Fix16_Point_POD::operator+=(Fix16_Point& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

struct Fix16_Vec
{
    Fix16 x, y, z;
};
