#pragma once

#include "Function.hpp"

class GosubFramePool_25C;
class GosubFrame_C;

class miss2_8
{
  public:
    EXPORT miss2_8() throw(); // 503120
    EXPORT ~miss2_8(); // 503130

    EXPORT void add_503160(GosubFrame_C* pFrame);
    EXPORT GosubFrame_C* remove_503180();

    EXPORT GosubFrame_C* AllocFrame_5031A0();

    EXPORT void FreeFrame_5031C0(GosubFrame_C* pFrame);

    EXPORT void remove_5031E0(u8 count);

    GosubFrame_C* field_0_current;
    u8 field_4_count;
    u8 field_5_pad;
    u8 field_6_pad;
    u8 field_7_pad;
};
GTA2_ASSERT_SIZEOF_ALWAYS(miss2_8, 0x8)