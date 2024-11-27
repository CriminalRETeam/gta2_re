#pragma once

#include "Function.hpp"

class Frismo_25C;
class Frismo_C;

class miss2_8
{
  public:
    miss2_8(); // 503120
    ~miss2_8(); // 503130

    void add_503160(Frismo_C* a2);
    Frismo_C* remove_503180();
    void remove_5031E0(u8 count);

    Frismo_C* field_0_current;
    u8 field_4_count;
    u8 field_5_pad;
    u8 field_6_pad;
    u8 field_7_pad;
};
GTA2_ASSERT_SIZEOF_ALWAYS(miss2_8, 0x8)