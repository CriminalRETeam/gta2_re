#include "Fix16_Point.hpp"
#include "Function.hpp"

// The out-of-line copy of the inline operator+
MATCH_FUNC(0x40AC50)
Fix16_Point Fix16_Point::Add_40AC50(const Fix16_Point& in)
{
    return Fix16_Point(x + in.x, y + in.y);
}

// The out-of-line copy of the inline operator-
MATCH_FUNC(0x40AC80)
Fix16_Point Fix16_Point::Sub_40AC80(const Fix16_Point& rhs)
{
    return Fix16_Point(x - rhs.x, y - rhs.y);
}

MATCH_FUNC(0x40ACB0)
Fix16_Point Fix16_Point::Negate_40ACB0() const
{
    return Fix16_Point(-x, -y);
}

MATCH_FUNC(0x40ACD0)
Ang16 Fix16_Point::atan2_40ACD0()
{
    return Fix16::atan2_fixed_405320(y, x);
}

// https://decomp.me/scratch/qQwG3
MATCH_FUNC(0x438FE0)
Fix16_Point Fix16_Point::Multiply_438FE0(Fix16& in)
{
    return Fix16_Point(x * in, y * in);
}

WIP_FUNC(0x442AD0)
Fix16_Point Fix16_Point::NormalizeSafe_442AD0()
{
    WIP_IMPLEMENTED;
    Fix16 length = GetLength_inline_442AD0();
    if (length == gFix16_6777CC)
    {
        Fix16_Point scaled = MultBy_442C80(128);
        length = scaled.GetLength_scaled_inline_442AD0();
        return scaled / length;
    }
    else
    {
        return *this / length;
    }
}

Fix16_Point Fix16_Point::operator/(Fix16& in)
{
    return Fix16_Point(x / in, y / in);
}

MATCH_FUNC(0x442C80)
Fix16_Point Fix16_Point::MultBy_442C80(const s32& factor)
{
    return Fix16_Point(x * factor, y * factor);
}

// https://decomp.me/scratch/nFSYS
MATCH_FUNC(0x442CB0)
Fix16_Point Fix16_Point::Divide_442CB0(Fix16& in)
{
    return Fix16_Point(x / in, y / in);
}

MATCH_FUNC(0x453590)
Fix16 Fix16_Point::GetLength_453590()
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

MATCH_FUNC(0x55F9E0)
Fix16_Point Fix16_Point::operator/(const s32& a3)
{
    return Fix16_Point(x / a3, y / a3);
}

MATCH_FUNC(0x5605E0)
Fix16_Point Fix16_Point::Rotate90CCW_5605E0()
{
    // TODO: Mov instruction is encoded wrongly ??
    return Fix16_Point(-y, x);
}

MATCH_FUNC(0x5E40C0)
Fix16_Point& Fix16_Point::AddAssign_5E40C0(const Fix16_Point& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

MATCH_FUNC(0x5E40E0)
Fix16_Point& Fix16_Point::DivAssign_5E40E0(const Fix16& v)
{
    x /= v;
    y /= v;
    return *this;
}

// The larger of |x| and |y|, a cheap stand-in for the length.
MATCH_FUNC(0x5E4140)
Fix16 Fix16_Point::MaxAbs_5E4140()
{
    Fix16 ax = Fix16::Abs(x);
    Fix16 ay = Fix16::Abs(y);
    if (ax > ay)
    {
        return ax;
    }
    return ay;
}
