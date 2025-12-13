#include "Function.hpp"
#include "Fix16_Point.hpp"

// https://decomp.me/scratch/qQwG3
STUB_FUNC(0x438FE0)
Fix16_Point_POD Fix16_Point_POD::Multiply_438FE0(Fix16& in)
{
    return Fix16_Point_POD(x * in, y * in);
}
