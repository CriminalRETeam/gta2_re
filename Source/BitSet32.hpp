#pragma once

#include "types.hpp"
#include "Function.hpp"

EXTERN_GLOBAL(u32, kGlobalMask0);
EXTERN_GLOBAL(u32, kGlobalMask1);
EXTERN_GLOBAL(u32, kGlobalMask2);
EXTERN_GLOBAL(u32, gFlags);

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

// Byte-sized bit field: clearing a bit through it is a byte `and` whose mask VC6 does not share with
// the same mask on a 32 bit field (Ped::Reset)
struct CompilerBitField8
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

class BitSet32
{
  public:
    // 9.6f 0x420DE0
    void clear_bit(s32 bit)
    {
        m_var &= ~(1 << bit);
    }

    // 9.6f 0x420DC0
    void set_bit(s32 bit)
    {
        m_var |= (1 << bit);
    }

    // Unused, but removing it changes MapRenderer code (see the shared header names note in
    // docs/matching_quirks.md)
    bool check_bit(s32 bit)
    {
        return (m_var & (1 << bit)) ? true : false;
    }

    // 9.6f 0x420E20, where the bit is passed by reference; by reference here changes most of the code
    bool mask_bit(u32 bit)
    {
        return (m_var & (1u << bit)) == (1u << bit);
    }

    // 9.6f 0x4A51A0. Unused, kept for MapRenderer like check_bit
    BitSet32& operator=(u32& value) // Note: arg as reference
    {
        m_var = value;
        return *this;
    }

    // 9.6f 0x4A5190
    inline u32 GetRaw()
    {
        return m_var;
    }

    // 9.6f 0x4BA360
    inline void SetGlobal0()
    {
        gFlags = kGlobalMask0;
    }

    // 9.6f 0x4BA370
    inline void SetGlobal1()
    {
        gFlags = kGlobalMask1;
    }

    // 9.6f 0x4BA350
    inline void SetGlobal2()
    {
        gFlags = kGlobalMask2;
    }

    // 9.6f 0x4BA330
    inline void MaskWithGlobalFlags()
    {
        m_var &= gFlags;
    }

    // 9.6f 0x4BA340
    inline void ClearGlobalFlags()
    {
        m_var &= ~gFlags;
    }

    // 9.6f 0x420D90
    void ClearAllBits()
    {
        m_var = 0;
    }

    // 9.6f 0x420D80
    BitSet32()
     : m_var(0)
    {
    }

    // Unused, kept for MapRenderer like check_bit
    BitSet32(s32 value) : m_var(value)
    {
    }

    u32 m_var;
};

struct DamageAreaFlags : BitSet32
{
    EXPORT s16 GetOrBuildDamageMask(u16& sprite_idx, u8& bRet, const u16& palette, const u32& bClearMask);
    EXPORT void ApplyDamageDeltas(u16& sprite_idx, u8* pArray, u32& delta_mask, u8& width);
};
