#include "Fix16_Point.hpp"
#include "Function.hpp"

// https://decomp.me/scratch/qQwG3
STUB_FUNC(0x438FE0)
Fix16_Point_POD Fix16_Point_POD::Multiply_438FE0(Fix16& in)
{
    return Fix16_Point_POD(x * in, y * in);
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

STUB_FUNC(0x442AD0)
Fix16_Point Fix16_Point::NormalizeSafe_442AD0()
{
    NOT_IMPLEMENTED;
    return Fix16_Point(x, y);
}

STUB_FUNC(0x40ACD0)
Ang16 Fix16_Point::atan2_40ACD0()
{
    return Fix16::atan2_fixed_405320(y, x);
}

WIP_FUNC(0x5605E0)
Fix16_Point Fix16_Point::Rotate90CCW_5605E0()
{
    WIP_IMPLEMENTED;
    
    Fix16_Point t;
    t.x = -this->y;
    t.y = this->x;
    return t;
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

