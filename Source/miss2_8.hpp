#pragma once

#include "Function.hpp"

class ScriptStackFramePool_25C;
class ScriptStackFrame_C;

class miss2_8
{
  public:
    EXPORT miss2_8() throw(); // 503120
    EXPORT ~miss2_8(); // 503130

    EXPORT void add_503160(ScriptStackFrame_C* pFrame);
    EXPORT ScriptStackFrame_C* remove_503180();

    EXPORT ScriptStackFrame_C* AllocFrame_5031A0();

    EXPORT void FreeFrame_5031C0(ScriptStackFrame_C* pFrame);

    EXPORT void remove_5031E0(u8 count);

    ScriptStackFrame_C* field_0_current;
    u8 field_4_count;
    u8 field_5_pad;
    u8 field_6_pad;
    u8 field_7_pad;
};
GTA2_ASSERT_SIZEOF_ALWAYS(miss2_8, 0x8)