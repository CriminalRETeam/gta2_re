#pragma once

#include "types.hpp"
#include "Function.hpp"

EXTERN_GLOBAL(u32, kGlobalMask0_61A9AC);
EXTERN_GLOBAL(u32, kGlobalMask1_61A9A8);
EXTERN_GLOBAL(u32, kGlobalMask2_61A9A4);
EXTERN_GLOBAL(u32, gFlags_67ACF8);

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
    // 9.6f 0x420DE0
    void clear_bit(int bit)
    {
        m_var &= ~(1 << bit);
    }

    // 9.6f 0x420DC0
    void set_bit(int bit)
    {
        m_var |= (1 << bit);
    }

    // TODO: Find in 9.6f
    bool check_bit(s32 bit)
    {
        return (m_var & (1 << bit)) ? true : false;
    }

    // 9.6f 0x420E20 TODO: Check if matches
    bool mask_bit(u32 bit)
    {
        return (m_var & (1u << bit)) == (1u << bit);
    }

    // 9.6f 0x4A51A0
    BitSet32& operator=(u32& value) // Note: arg as reference
    {
        // TODO: Value is probably another BitSet32
        m_var = value;
        return *this;
    }

    inline u32 GetRaw_4A5190()
    {
        return m_var;
    }

    // 9.6f 0x4BA360
    inline void SetGlobal0_4BA360()
    {
        gFlags_67ACF8 = kGlobalMask0_61A9AC;
    }

    // 9.6f 0x4BA370
    inline void SetGlobal1_4BA370()
    {
        gFlags_67ACF8 = kGlobalMask1_61A9A8;
    }

    // 9.6f 0x4BA350
    inline void SetGlobal2_4BA350()
    {
        gFlags_67ACF8 = kGlobalMask2_61A9A4;
    }

    inline void sub_4BA330()
    {
        m_var &= gFlags_67ACF8;
    }

    inline void sub_4BA340()
    {
        m_var &= ~gFlags_67ACF8;
    }

    // 9.6f 0x420D90
    void ClearAllBits_420D90()
    {
        m_var = 0;
    }

    // 9.6f 0x420D80
    BitSet32()
     : m_var(0)
    {
    }

    // TODO: Find in 9.6f
    BitSet32(s32 value) : m_var(value)
    {
    }

    u32 m_var;
};