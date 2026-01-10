#include "Function.hpp"
#include "Fix16_Point.hpp"

// https://decomp.me/scratch/qQwG3
STUB_FUNC(0x438FE0)
Fix16_Point_POD Fix16_Point_POD::Multiply_438FE0(const Fix16& in)
{
    return Fix16_Point_POD(x * in, y * in);
}

// https://decomp.me/scratch/nFSYS
STUB_FUNC(0x442CB0)
Fix16_Point_POD Fix16_Point_POD::Divide_442CB0(const Fix16& in)
{
    return Fix16_Point_POD(x / in, y / in);
}

Fix16_Point Fix16_Point::operator-(const Fix16_Point& rhs)
{
    return Fix16_Point(x - rhs.x, y - rhs.y);
}
