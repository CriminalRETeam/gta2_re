#pragma once

#include "types.hpp"

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
    BitSet32& operator=(u32 value)
    {
        m_var = value;
        return *this;
    }

    BitSet32()
    {
    }

    BitSet32(s32 value) : m_var(value)
    {
    }

    u32 m_var;
};
