#pragma once

#include "Function.hpp"

class Frismo_C_Pool;
class Frismo_C;

class miss2_8
{
  public:
    EXPORT miss2_8(); // 503120
    EXPORT ~miss2_8(); // 503130

    EXPORT void add_503160(Frismo_C* a2);
    EXPORT Frismo_C* remove_503180();

    EXPORT Frismo_C* sub_5031A0();

    EXPORT void sub_5031C0(Frismo_C* a2);

    EXPORT void remove_5031E0(u8 count);

    Frismo_C* field_0_current;
    u8 field_4_count;
    u8 field_5_pad;
    u8 field_6_pad;
    u8 field_7_pad;
};
GTA2_ASSERT_SIZEOF_ALWAYS(miss2_8, 0x8)