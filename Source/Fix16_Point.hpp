#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "Fix16_Vec.hpp"

EXTERN_GLOBAL(Fix16, gFix16_6777CC);
EXTERN_GLOBAL(Fix16, kFP16Zero_6FE20C);
EXTERN_GLOBAL(Fix16, kFP16One256th);
EXTERN_GLOBAL(Fix16, kFpZero_6F77C0);

// GetLength_41E260 compares against a zero constant that each TU of the original has its own copy of
// (a header static, like 9.6f's single 0x5E3DC8 in vec_len 0x41E260). A TU whose copy isn't
// gFix16_6777CC defines FIX16_POINT_ZERO to it before its first include.
#ifndef FIX16_POINT_ZERO
    #define FIX16_POINT_ZERO gFix16_6777CC
#endif
EXTERN_GLOBAL(Fix16, kFpZero_6F610C);
EXTERN_GLOBAL(Fix16, kF16Zero);
EXTERN_GLOBAL(Fix16, kFpZero_706EB8);
EXTERN_GLOBAL(Fix16, kFP16Zero_6FD9E4);
EXTERN_GLOBAL(Fix16, kZero_676818);
EXTERN_GLOBAL(Fix16, kFpZero_7064C0);
EXTERN_GLOBAL(Fix16, kZero_679E70);

class Fix16_Point;

// A point without a destructor. 10.5 has points that don't take part in EH unwinding: struct members
// (~Sprite is a bare jmp, the Char ctor's EH states leave out mVelocityVector), a by-value
// parameter the caller copies as two dwords (DrawElectricArc) and a few locals (CarPhysics 0x55F280,
// 0x55F800, Camera::WorldToScreen_40CFC0's result). Everything else is a Fix16_Point. It is a
// separate type, not a base of Fix16_Point: 10.5 charges Fix16_Point's ctor as one size-42 inline
// site at the top level, which a derived class's ctor (base ctor nested in it) doesn't reproduce.
// 9.6f doesn't tell the two apart.
struct Fix16_Point_POD
{
    Fix16_Point_POD()
    {
    }

    Fix16_Point_POD(Fix16& new_x, Fix16& new_y)
    {
        x = new_x;
        y = new_y;
    }

    // 9.6f 0x432860
    void SetXY(Fix16& new_x, Fix16& new_y)
    {
        this->x = new_x;
        this->y = new_y;
    }

    inline void SetFromPolar_41E210(Fix16& radius, Ang16& angle)
    {
        x = Ang16::sine(angle) * radius;
        y = Ang16::cosine(angle) * radius;
    }

    // Matching impl at RotateVelocity
    inline void RotateByAngle_40F6B0(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine(angle);
        Fix16 cos = Ang16::cosine(angle);

        Fix16 x_old = x;

        x = (x * cos) + (y * sin);
        y = ((-x_old) * sin) + (y * cos);
    }

    Fix16_Point_POD& operator+=(Fix16_Point& other);

    // Same layout: a POD passed where a Fix16_Point is expected (a free inline site, like the other way)
    operator Fix16_Point&()
    {
        return *reinterpret_cast<Fix16_Point*>(this);
    }

    Fix16 x;
    Fix16 y;
};

class Fix16_Point
{
  public:
    void SetXY(Fix16& new_x, Fix16& new_y)
    {
        this->x = new_x;
        this->y = new_y;
    }

    // Reads the TU's FIX16_POINT_ZERO copy (Weapon::throwable compares with 0x706EB8)
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
    inline bool HasZeroComponent() const
    {
        return x == kFP16Zero_6FE20C || y == kFP16Zero_6FE20C;
    }

    // 9.6f 0x49E3C0
    void ApplyDeadZone()
    {
        Fix16 total = (Fix16::Abs(x) + Fix16::Abs(y));
        if (total < kFP16One256th)
        {
            x = kFP16Zero_6FE20C;
            y = kFP16Zero_6FE20C;
        }
    }

    // But also 0x40ACD0 non inlined in 10.5
    Ang16 atan2_40F790()
    {
        return Fix16::atan2_fixed(y, x);
    }

    Fix16 GetLength_453590() const;

    // 9.6f 0x41E260; the out-of-line copy is GetLength_453590. The nested if/else (front-end size 162, the
    // else-if chain is 160) gives the original's cut-offs in Crane::ComputeHookPolar with
    // SquareRoot at 48; matched functions allow 159..162.
    inline Fix16 GetLength_41E260() const
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

    // GetLength_41E260 as inlined twice into Car::GetCarLinearSpeed: the original inlines both
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
        x = Ang16::sine(angle) * radius;
        y = Ang16::cosine(angle) * radius;
    }

    // RotateByAngle_40F6B0 in a function that ran out of inline expansions: every operator but
    // the first + is called out of line (CarPhysics::SpawnSkidSegment)
    inline void RotateByAngle_40F6B0_out_of_line(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine(angle);
        Fix16 cos = Ang16::cosine(angle);

        Fix16 x_old = x;

        x = x.Multiply(cos) + y.Multiply(sin);
        y = x_old.Negate_4086A0().Multiply(sin).Add(y.Multiply(cos));
    }

    // RotateByAngle_40F6B0 in a function whose inline budget ran out after the first multiply
    // (y * sin, evaluated first): the rest are the out-of-line copies (Car::SpawnDamageFireEffect)
    inline void RotateByAngle_OneMulInline_40F6B0(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine(angle);
        Fix16 cos = Ang16::cosine(angle);

        Fix16 x_old = x;

        x = x.Multiply(cos).Add(y * sin);
        y = x_old.Negate_4086A0().Multiply(sin).Add(y.Multiply(cos));
    }

    // RotateByAngle_40F6B0 with every operator called out of line (ParticleManager::EmitImpactParticles)
    inline void RotateByAngle_40F6B0_all_out_of_line(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine(angle);
        Fix16 cos = Ang16::cosine(angle);

        Fix16 x_old = x;

        x = x.Multiply(cos).Add(y.Multiply(sin));
        y = x_old.Negate_4086A0().Multiply(sin).Add(y.Multiply(cos));
    }

    // Matching impl at RotateVelocity
    inline void RotateByAngle_40F6B0(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine(angle);
        Fix16 cos = Ang16::cosine(angle);

        Fix16 x_old = x;

        x = (x * cos) + (y * sin);
        y = ((-x_old) * sin) + (y * cos);
    }

    // RotateByAngle_40F6B0 as big functions get it once they run out of inline expansions:
    // the Fix16 operators are the out-of-line copies (Car::HandleCarHitByObject)
    inline void RotateByAngle_OOL_40F6B0(const Ang16& angle)
    {
        Fix16 x_old = x;
        Fix16 sin = Ang16::sine(angle);
        Fix16 cos = Ang16::cosine(angle);

        x = x.Multiply(cos).Add(y.Multiply(sin));
        y = (-x_old).Multiply(sin).Add(y.Multiply(cos));
    }

    // As above, with the unary minus out of line too
    inline void RotateByAngle_NegOOL_40F6B0(const Ang16& angle)
    {
        Fix16 x_old = x;
        Fix16 sin = Ang16::sine(angle);
        Fix16 cos = Ang16::cosine(angle);

        x = x.Multiply(cos).Add(y.Multiply(sin));
        y = x_old.Negate_4086A0().Multiply(sin).Add(y.Multiply(cos));
    }

    void FromPolar_41E210(const Fix16& radius, const Ang16& angle)
    {

        x = radius * Ang16::sine(angle);
        y = radius * Ang16::cosine(angle);
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

    // 9.6f 0x49E3A0
    Fix16_Point& MultiplyByFix16(const Fix16& factor)
    {
        x *= factor;
        y *= factor;
        return *this;
    }

    // Out-of-line copy of RotateByAngle_40F6B0, emitted in CarPhysics.cpp
    EXPORT void RotateVelocity(const Ang16& angle);

    EXPORT Fix16_Point Multiply(Fix16& in) const;
    EXPORT Fix16_Point Divide(Fix16& in) const;

    // Divide as a nothrow inline (see DivideInl_55F9E0; CarPhysics::HandleObjectCollision,
    // Car::ApplyExplosionImpulse)
    inline Fix16_Point DivideInl_442CB0(Fix16& in) const throw()
    {
        return Fix16_Point(x / in, y / in);
    }

    // Multiply as a nothrow inline (see DivideInl_55F9E0; Car::TryHitchTrailer)
    inline Fix16_Point MultiplyInl_438FE0(Fix16& in) const throw()
    {
        return Fix16_Point(x * in, y * in);
    }

    // Out-of-line copies emitted in Weapon.cpp (used by DrawElectricArc).
    EXPORT Fix16_Point& AddAssign(const Fix16_Point& other);
    EXPORT Fix16_Point& DivAssign(const Fix16& v);
    EXPORT Fix16 MaxAbs_5E4140() const;

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
    inline void DivideAssign(const Fix16& d)
    {
        x.DivideAssign(d);
        y.DivideAssign(d);
    }

    // FUNCTION: 96f 0x41e1e0
    void reset()
    {
        x = Fix16(0);
        y = Fix16(0);
    }

    // Out of line at 0x40AC40 only as the destructor the eh vector constructor iterator (??_L) gets for
    // Fix16_Point arrays (CarPhysics::ctor_563900, SpriteShape::ctor_5A57B0)
    ~Fix16_Point()
    {
    }

    // Out of line at 0x563970 for the same ??_L calls: a header inline, so it has no marker
    Fix16_Point()
    {
    }

    // No user-defined copy ctor: 9.6f copies points with plain movs (no call even at /Ob0), and the
    // implicit one does not use up VC6's inline budget (SpawnCabAndTrailerHelper needs that).

    // 9.6f 0x401D20
    Fix16_Point(const Fix16& new_x, const Fix16& new_y) : x(new_x), y(new_y)
    {
    }

    // 9.6f 0x49E480
    void ClampTowardsZero(const Fix16_Point& limit)
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
    Fix16_Point operator+(const Fix16_Point& in) const
    {
        return Fix16_Point(x + in.x, y + in.y);
    }

    // MATCH_FUNC(0x40AC80)
    Fix16_Point operator-(const Fix16_Point& rhs) const
    {
        return Fix16_Point(x - rhs.x, y - rhs.y);
    }

    // The out-of-line copy of the inline operator-. Called by name where the original keeps an EH state
    // for temporaries around the call (Crane::HookPickupCar, ComputeHookPolar); the
    // inline operator- called out of line gets none (CarPhysics::HandleCarCollision)
    EXPORT Fix16_Point Sub(const Fix16_Point& rhs) const;

    // operator+/operator- with their bodies defined at the end of Car.cpp (HandleCarHitByObject)
    inline Fix16_Point AddLate_40AC50(const Fix16_Point& in);
    inline Fix16_Point SubLate_40AC80(const Fix16_Point& rhs);

    // Out of line operator+ (CarPhysics::SpawnSkidSegment)
    EXPORT Fix16_Point Add(const Fix16_Point& in) const;

    // operator+ 0x40AC50 as a nothrow inline that VC6 still calls out of line: no EH state for the
    // temporaries alive across the call (Car::TryHitchTrailer)
    inline Fix16_Point AddInl_40AC50(const Fix16_Point& in) const throw()
    {
        return Fix16_Point(x + in.x, y + in.y);
    }

    // Out of line unary minus (Object::ResolveCollisionWithPed)
    EXPORT Fix16_Point Negate_40ACB0() const;

    // Unused (ComputeHookPolar calls GetLength_41E260 now). Kept: deleting any inline from this header
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

    // Unused (GetSpeed matches with the plain GetLength_41E260), but don't remove it: without it
    // MapRenderer::Draw4SidedDiagonalUpLeft and Draw3SidedDiagonalDownRight stop matching
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
            return Fix16::SquareRoot_436A70(x.Multiply(x).Add(y.Multiply(y)));
        }
    }

    // Needed for CarPhysics::HandleCarCollision.
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
            return Fix16::SquareRoot_436A70(x.Multiply(x).Add(y.Multiply(y)));
        }
    }

    // Needed for CarPhysics::ComputeEngineTorque: named out-of-line multiplies, inline add (Abs goes
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
            return Fix16::SquareRoot_436A70(Fix16::Add_ref(x.Multiply(x), y.Multiply(y)));
        }
    }

    // Needed for CarPhysics::SpawnSkidSegment.
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
            return Fix16::SquareRoot_436A70(x.Multiply(x).Add(y.Multiply(y)));
        }
    }

    // Needed for CarPhysics::ScarePedsOnDrivingFast and UpdateSteeringAngle.
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
            return Fix16::SquareRoot_436A70(x.Multiply(x).Add(y * y));
        }
    }

    Fix16_Point operator*(Fix16& in) const
    {
        return Fix16_Point(x * in, y * in);
    }

    Fix16_Point operator-() const
    {
        return Fix16_Point(-x, -y);
    }

    EXPORT Fix16_Point MultBy(const s32& factor) const;

    // 10.0 0x442CB0
    EXPORT Fix16_Point operator/(Fix16& in) const;

    // Unused (ApplyExplosionImpulse calls GetLength_41E260 now), kept for the same reason as
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

    // GetLength_41E260 as inlined into NormalizeSafe (out of line helpers where the inline budget ran out)
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
            return Fix16::SquareRoot_436A70(x.Multiply(x).Add(y * y));
        }
    }

    // MultiplyByFix16 as inlined into CarPhysics::CalculateRearWheelForce: the
    // second *= is the out-of-line copy
    void MultiplyByFix16_inline_5620D0(const Fix16& factor)
    {
        x *= factor;
        y.MultiplyAssign(factor);
    }

    // GetLength_41E260 as inlined into CarPhysics::CalculateRearWheelForce: Abs and x*x out of line
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
            return Fix16::SquareRoot_436A70(x * x + Fix16(static_cast<s32>((y.mValue * static_cast<__int64>(y.mValue)) >> 14), 0));
        }
    }

    // Same, for the scaled point in NormalizeSafe
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
            return Fix16::SquareRoot_436A70(x.Multiply(x).Add(y.Multiply(y)));
        }
    }

    EXPORT Fix16_Point NormalizeSafe();

    EXPORT Ang16 atan2_40ACD0();

    EXPORT Fix16_Point Rotate90CCW() const;

    void clear_41E1E0()
    {
        x = 0;
        y = 0;
    }

    EXPORT Fix16_Point operator/(const s32& divisor) const;

    // operator/ 0x55F9E0 as a nothrow inline that VC6 still calls out of line: the original sets no EH
    // state for the temporaries that live across this call (CarPhysics::HandleCarCollision,
    // HandleObjectCollision). throw() on the real operator/ changes 6 matched functions.
    inline Fix16_Point DivideInl_55F9E0(const s32& divisor) const throw()
    {
        return Fix16_Point(x / divisor, y / divisor);
    }

    // Same layout: a point sliced into a Fix16_Point_POD (one free inline site, see ShockPedsAroundSprite)
    operator Fix16_Point_POD&()
    {
        return *reinterpret_cast<Fix16_Point_POD*>(this);
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
