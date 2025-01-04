#pragma once

#include "fix16.hpp"
#include "types.hpp"

struct angle
{
    s16 field_0_value;

    angle(): field_0_value(0)
    {
    }

    angle(const s16 &param_1, s32)
    {
        this->field_0_value = param_1;
        clamp();
    }

    angle(const Fix16 &param_1, s32)
    {
        field_0_value = param_1.ToInt();
        clamp();
    }

    void clamp();

    s32 operator!=(const angle &right) const
    {
        return field_0_value != right.field_0_value;
    }

    s32 operator>(const angle &right) const
    {
        return field_0_value > right.field_0_value;
    }

    s32 operator<(const angle &right) const
    {
        return field_0_value < right.field_0_value;
    }

    s32 operator<=(const angle &right) const
    {
        return field_0_value <= right.field_0_value;
    }

    angle operator+(const angle &right) const
    {
        return angle(field_0_value + right.field_0_value, 0);
    }

    angle operator-(const angle &right) const
    {
        return angle(field_0_value - right.field_0_value, 0);
    }

    angle operator-()
    {
        return angle(-field_0_value, 0);
    }

    angle &operator+=(const angle &right)
    {
        field_0_value = field_0_value + right.field_0_value;
        clamp();
        return *this;
    }

    angle &operator-=(const angle &right)
    {
        field_0_value = field_0_value - right.field_0_value;
        clamp();
        return *this;
    }
};

//static_assert(sizeof(angle) == 0x2);
