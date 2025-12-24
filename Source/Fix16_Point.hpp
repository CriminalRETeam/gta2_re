#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "ang16.hpp"

EXTERN_GLOBAL(Fix16, gFix16_6777CC);
EXTERN_GLOBAL(Fix16, kFP16Zero_6FE20C);

// TODO: Some functions like Camera_0xBC::sub_435A70 won't match unless this is a POD
// but 9.6f leads me to believe both the POD and non-POD type are the same
struct Fix16_Point_POD
{
    // None inline exists in 10.5 at 0x453590
    inline Fix16 GetLength()
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
            return Fix16::SquareRoot(x*x + y*y);
        }
    }

    inline void sub_41E210(Fix16& unk, Ang16& angle)
    {
        x = Ang16::sine_40F500(angle) * unk;
        y = Ang16::cosine_40F520(angle) * unk;
    }

    inline void RotateByAngle_40F6B0(Ang16& angle)
    {
        Fix16 sin = Ang16::sine_40F500(angle);
        Fix16 cos = Ang16::cosine_40F520(angle);
        
        Fix16 x_old = x;
        
        x = (x * cos) + (y * sin);
        y = ((-x_old) * sin) + (y * cos);
    }

    Fix16_Point_POD Fix16_Point_POD::operator+(Fix16_Point_POD& in)
    {
        return Fix16_Point_POD(x + in.x, y + in.y);
    }

    Fix16_Point_POD& Fix16_Point_POD::operator+=(Fix16_Point_POD& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    EXPORT Fix16_Point_POD Multiply_438FE0(Fix16& a1);
    EXPORT Fix16_Point_POD Divide_442CB0(Fix16& a1);

    Fix16_Point_POD()
    {
    }

    Fix16_Point_POD(Fix16& a1, Fix16& a2)
    {
        x = a1;
        y = a2;
    }

    // Inlined, on version 9.6f 0x41E1E0
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

    // 0x40AC80
    Fix16_Point operator-(const Fix16_Point& rhs);

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
            return Fix16::SquareRoot(x*x + y*y);
        }
    }
};

struct Fix16_Vec
{
    Fix16 x, y, z;
};
