#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

EXTERN_GLOBAL(Fix16, gFix16_6777CC);
EXTERN_GLOBAL(Fix16, kFP16Zero_6FE20C);
EXTERN_GLOBAL(Fix16, kFP16One256th_6FE07C);
EXTERN_GLOBAL(Fix16, kFpZero_6F77C0);

// TODO: Some functions like Camera_0xBC::sub_435A70 won't match unless this is a POD
// but 9.6f leads me to believe both the POD and non-POD type are the same
class Fix16_Point;

struct Fix16_Point_POD
{
    void SetXY_432860(Fix16& a2, Fix16& a3)
    {
        this->x = a2;
        this->y = a3;
    }

    inline bool IsNull_420360() const
    {
        return x == gFix16_6777CC && y == gFix16_6777CC;
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

    // None inline exists in 10.5 at 0x453590
    inline Fix16 GetLength_41E260()
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
            return Fix16::SquareRoot(x * x + y * y);
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
    // the Fix16 operators are the out-of-line copies (Weapon_30::fire_truck_flamethrower_5E0B10)
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

    // As above, with y * sin inlined (Particle_8::EmitFlameStreamSegment_53F4C0)
    inline void RotateByAngle_MixOOL_40F6B0(const Ang16& angle)
    {
        Fix16 x_old = x;
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        x = x.Multiply_408680(cos).Add_408660(y * sin);
        y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(y.Multiply_408680(cos));
    }

    // As RotateByAngle_40F6B0 with the x line inline and the y line out of line (Car_BC::GetHitchPoint_439FB0)
    inline void RotateByAngle_YOOL_40F6B0(const Ang16& angle)
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);

        Fix16 x_old = x;

        x = (x * cos) + (y * sin);
        y = x_old.Negate_4086A0().Multiply_408680(sin).Add_408660(y.Multiply_408680(cos));
    }

    void FromPolar_41E210(const Fix16& radius, const Ang16& angle)
    {

        x = radius * Ang16::sine_40F500(angle);
        y = radius * Ang16::cosine_40F520(angle);
    }

    // 9.6f 0x40F5C0
    Fix16_Point_POD Fix16_Point_POD::operator+(const Fix16_Point_POD& in)
    {
        return Fix16_Point_POD(x + in.x, y + in.y);
    }

    Fix16_Point_POD& Fix16_Point_POD::operator+=(Fix16_Point_POD& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // FUNCTION: 96f 0x4828c0
    Fix16_Point_POD& Fix16_Point_POD::operator-=(Fix16_Point_POD& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Operator* for Fix16 ?
    Fix16_Point_POD& MultiplyByFix16_49E3A0(const Fix16& factor)
    {
        x *= factor;
        y *= factor;
        return *this;
    }

    // Out-of-line copy of RotateByAngle_40F6B0, emitted in CarPhysics_B0.cpp
    EXPORT void RotateVelocity_562C20(const Ang16& angle);

    EXPORT Fix16_Point Multiply_438FE0(Fix16& a1);
    EXPORT Fix16_Point Divide_442CB0(Fix16& a1);
    inline Fix16_Point DivideInl_442CB0(Fix16& in) throw();
    inline Fix16_Point MultiplyInl_438FE0(Fix16& in) throw();

    // Out-of-line copies emitted in Weapon_30.cpp (used by sub_5DE910).
    EXPORT Fix16_Point_POD& AddAssign_5E40C0(const Fix16_Point_POD& other);
    EXPORT Fix16_Point_POD& DivAssign_5E40E0(const Fix16& v);
    EXPORT Fix16 MaxAbs_5E4140();

    Fix16_Point_POD()
    {
    }

    Fix16_Point_POD(Fix16& a1, Fix16& a2)
    {
        x = a1;
        y = a2;
    }

    // FUNCTION: 96f 0x41e1e0
    void reset()
    {
        x = Fix16(0);
        y = Fix16(0);
    }

    Fix16 x;
    Fix16 y;
};

class Fix16_Point : public Fix16_Point_POD
{
  public:
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
    Fix16_Point(const Fix16& a1, const Fix16& a2)
    {
        x = a1;
        y = a2;
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
    Fix16_Point operator+(const Fix16_Point_POD& in)
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

    // Out of line operator+ (CarPhysics_B0::SpawnSkidSegment_55D200; Weapon_30::fire_truck_flamethrower_5E0B10 keeps the EH state of
    // the get_x_y_443580 temporary around this call)
    EXPORT Fix16_Point Add_40AC50(const Fix16_Point_POD& in);

    // operator+ 0x40AC50 as a nothrow inline that VC6 still calls out of line: no EH state for the
    // temporaries alive across the call (Car_BC::TryHitchTrailer_442810)
    inline Fix16_Point AddInl_40AC50(const Fix16_Point_POD& in) throw()
    {
        return Fix16_Point(x + in.x, y + in.y);
    }

    // Out of line unary minus (Object_2C::ResolveCollisionWithPed_5229B0)
    EXPORT Fix16_Point Negate_40ACB0() const;

    // The same function of GetLength but using another cutoff
    inline Fix16 GetLength_2()
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
            return Fix16::SquareRoot(x * x + y * y);
        }
    }

    // OBS: needed for matching Crane_15C::ComputeHookPolar_47F6C0
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

    // Needed for miss2_0x11C::SCRCMD_CHECK_CAR_SPEED_50E360.
    inline Fix16 GetLength_no_sqrt_inline_abs_y_negate()
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
            return Fix16::SquareRoot_436A70(x * x + y * y);
        }
    }

    // Needed for miss2_0x11C::GetSpeed_50E190.
    inline Fix16 GetLength_453590_inline_wrap()
    {
        return GetLength_453590();
    }

    // Needed for miss2_0x11C::GetSpeed_50E190.
    inline Fix16 GetLength_all_out_of_line_abs_y_negate()
    {
        if (x == kFpZero_6F77C0)
        {
            return Fix16::Abs_negate_out_of_line(y);
        }
        else if (y == kFpZero_6F77C0)
        {
            return Fix16::Abs_436A50(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y.Multiply_408680(y)));
        }
    }

    // Needed for CarPhysics_B0::ShowSpeedRevsDamage_5597B0.
    inline Fix16 GetLength_all_out_of_line_abs_negate()
    {
        if (x == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_negate_out_of_line(y);
        }
        else if (y == kFP16Zero_6FE20C)
        {
            return Fix16::Abs_negate_out_of_line(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y.Multiply_408680(y)));
        }
    }

    // Needed for CarPhysics_B0::ShowSpeedRevsDamage_5597B0.
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

    // Needed for CarPhysics_B0::ComputeEngineTorque_561970: out-of-line Abs and multiplies, inline add.
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

    // Needed for CarPhysics_B0::ApplyImpactForcesAndDamage_55FA60.
    inline Fix16 GetLength_out_of_line_abs_x_squared()
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
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y * y));
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

    Fix16_Point operator+(Fix16_Point& in)
    {
        return Fix16_Point(x + in.x, y + in.y);
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

    // GetLength_41E260 as inlined into Car_BC::ApplyExplosionImpulse_443710 (out of line helpers)
    inline Fix16 GetLength_inline_443710()
    {
        if (x == gFix16_6777CC)
        {
            return Fix16::Abs_436A50(y);
        }
        else if (y == gFix16_6777CC)
        {
            return Fix16::Abs_436A50(x);
        }
        else
        {
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(y.Multiply_408680(y)));
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

    // GetLength_2 as inlined into CarPhysics_B0::ProcessPedImpact_560B40 (out of line helpers)
    inline Fix16 GetLength_inline_560B40()
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

    // GetLength_41E260 as inlined into Car_BC::TryHitchTrailer_442810 (out of line helpers)
    inline Fix16 GetLength_inline_442810()
    {
        if (x == gFix16_6777CC)
        {
            return Fix16::Abs_436A50(y);
        }
        else if (y == gFix16_6777CC)
        {
            return Fix16::Abs_436A50(x);
        }
        else
        {
            return Fix16::SquareRoot(x.Multiply_408680(x).Add_408660(y.Multiply_408680(y)));
        }
    }

    // MultiplyByFix16_49E3A0 as inlined into CarPhysics_B0::CalculateRearWheelForce_5620D0: the
    // second *= is the out-of-line copy
    void MultiplyByFix16_inline_5620D0(const Fix16& factor)
    {
        x *= factor;
        y.MultiplyAssign_562430(factor);
    }

    // GetLength_2 as inlined into CarPhysics_B0::CalculateRearWheelForce_5620D0: Abs out of line,
    // x*x out of line, y*y inline
    inline Fix16 GetLength_inline_5620D0()
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
            return Fix16::SquareRoot_436A70(x.Multiply_408680(x).Add_408660(Fix16((s32)((y.mValue * (__int64)y.mValue) >> 14), 0)));
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
};

// Divide_442CB0 as a nothrow inline (see DivideInl_55F9E0; CarPhysics_B0::HandleObjectCollision_5606C0,
// Car_BC::ApplyExplosionImpulse_443710)
inline Fix16_Point Fix16_Point_POD::DivideInl_442CB0(Fix16& in) throw()
{
    return Fix16_Point(x / in, y / in);
}

// Multiply_438FE0 as a nothrow inline (see DivideInl_55F9E0; Car_BC::TryHitchTrailer_442810)
inline Fix16_Point Fix16_Point_POD::MultiplyInl_438FE0(Fix16& in) throw()
{
    return Fix16_Point(x * in, y * in);
}

struct Fix16_Vec
{
    Fix16 x, y, z;
};
