#pragma once

#include "types.hpp"

// Dumb name to avoid conflict with BitField32
struct CompilerBitField32
{
    u32 b0 : 1;
    u32 b1 : 1;
    u32 b2 : 1;
    u32 b3 : 1;
    u32 b4 : 1;
    u32 b5 : 1;
    u32 b6 : 1;
    u32 b7 : 1;
    u32 b8 : 1;
    u32 b9 : 1;
    u32 b10 : 1;
    u32 b11 : 1;
    u32 b12 : 1;
    u32 b13 : 1;
    u32 b14 : 1;
    u32 b15 : 1;
    u32 b16 : 1;
    u32 b17 : 1;
    u32 b18 : 1;
    u32 b19 : 1;
    u32 b20 : 1;
    u32 b21 : 1;
    u32 b22 : 1;
    u32 b23 : 1;
    u32 b24 : 1;
    u32 b25 : 1;
    u32 b26 : 1;
    u32 b27 : 1;
    u32 b28 : 1;
    u32 b29 : 1;
    u32 b30 : 1;
    u32 b31 : 1;
};

class BitSet32
{
  public:
    void clear_bit(int bit)
    {
        m_var &= ~(1 << bit);
    }
    void set_bit(int bit)
    {
        m_var |= (1 << bit);
    }
    bool check_bit(s32 bit)
    {
        return (m_var & (1 << bit)) ? true : false;
    }

    // TODO: don't know if matches, added for Car_BC::sub_43BF10
    bool mask_bit(u32 bit)
    {
        return (m_var & (1u << bit)) == (1u << bit);
    }

    BitSet32& operator=(u32& value) // Note: arg as reference
    {
        m_var = value;
        return *this;
    }

    // 0x420D80
    BitSet32()
     : m_var(0)
    {
    }

    BitSet32(s32 value) : m_var(value)
    {
    }

    u32 m_var;
};