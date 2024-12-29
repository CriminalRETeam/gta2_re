#pragma once

class BitSet32 {
  public:
    void clear_bit(int bit) {
        m_var &= ~(1 << bit);
    }
    BitSet32& operator=(u32 value)
    {
        m_var = value;
        return *this;
    }
  private:
    u32 m_var;
};