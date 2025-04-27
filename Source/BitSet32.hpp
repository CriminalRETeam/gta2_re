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

    // TODO: don't know if matches, added for Car_BC::sub_43BF10
    bool mask_bit(u32 bit)
    {
        return (m_var & (1u << bit)) == (1u << bit);
    }

    BitSet32& operator=(u32 value)
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
