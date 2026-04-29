#include "Fix16_Point.hpp"
#include "Function.hpp"

// https://decomp.me/scratch/qQwG3
STUB_FUNC(0x438FE0)
Fix16_Point_POD Fix16_Point_POD::Multiply_438FE0(Fix16& in)
{
    return Fix16_Point_POD(x * in, y * in);
}

MATCH_FUNC(0x442C80)
Fix16_Point Fix16_Point::MultBy_442C80(const s32& factor)
{
    return Fix16_Point(x * factor, y * factor);
}

// https://decomp.me/scratch/nFSYS
STUB_FUNC(0x442CB0)
Fix16_Point_POD Fix16_Point_POD::Divide_442CB0(Fix16& in)
{
    return Fix16_Point_POD(x / in, y / in);
}

Fix16_Point Fix16_Point::operator-(const Fix16_Point& rhs)
{
    return Fix16_Point(x - rhs.x, y - rhs.y);
}

Fix16_Point Fix16_Point::operator/(Fix16& in)
{
    return Fix16_Point(x / in, y / in);
}

WIP_FUNC(0x442AD0)
Fix16_Point Fix16_Point::NormalizeSafe_442AD0()
{
    WIP_IMPLEMENTED;
    Fix16 length = GetLength_41E260();
    if (length == gFix16_6777CC)
    {
        Fix16_Point scaled = MultBy_442C80(128);
        return scaled / scaled.GetLength_41E260();
    }
    else
    {
        return *this / length;
    }
}

MATCH_FUNC(0x40ACD0)
Ang16 Fix16_Point::atan2_40ACD0()
{
    return Fix16::atan2_fixed_405320(y, x);
}

WIP_FUNC(0x5605E0)
Fix16_Point Fix16_Point::Rotate90CCW_5605E0()
{
    WIP_IMPLEMENTED;
    // TODO: Mov instruction is encoded wrongly ??
    return Fix16_Point(-y, x);
}

MATCH_FUNC(0x55F9E0)
Fix16_Point Fix16_Point::operator/(const s32& a3)
{
    return Fix16_Point(x / a3, y / a3);
}

MATCH_FUNC(0x453590)
Fix16 Fix16_Point_POD::GetLength_453590()
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
