#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "ang16.hpp"

// TODO: Some functions like DrawUnk_0xBC::sub_435A70 won't match unless this is a POD
// but 9.6f leads me to believe both the POD and non-POD type are the same
struct Fix16_Point_POD
{
    Fix16 x;
    Fix16 y;
}; 

class Fix16_Point : public Fix16_Point_POD
{
public:
    // Both inlined and exists as a function... some strange array init behaviour??
    ~Fix16_Point() {}

    // It needs to be in the header
    // MATCH_FUNC(0x563970)
    
    Fix16_Point()
    {
    }

    // 9.6f 0x401D20
    Fix16_Point(const Fix16_Point& rhs)
    {
        x = (rhs.x);
        y = (rhs.y);
    }

    Fix16_Point(Fix16& a1, Fix16& a2)
    {
        x = a1;
        y = a2;
    }

    // MATCH_FUNC(0x40AC50)
    Fix16_Point operator+(const Fix16_Point& in)
    {
        return Fix16_Point(x + in.x, y + in.y);
    }

    // 9.6f inline 0x40F6B0
    inline void RotateByAngle_40F6B0(Ang16& angle)
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);
        
        Fix16 x_old = x;
        
        x = (x * cos) + (y * sin);
        y = ((-x_old) * sin) + (y * cos);
    }

    // Inlined, on version 9.6f 0x41E1E0
    void reset()
    {
        x = Fix16(0);
        y = Fix16(0);
    }
};

struct Fix16_Vec
{
    Fix16 x, y, z;
};
