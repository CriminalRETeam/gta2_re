#pragma once

#include "Function.hpp"

class ScriptStackFramePool;
class ScriptStackFrame;

class ScriptGosubStack
{
  public:
    EXPORT ScriptGosubStack() throw(); // 503120
    EXPORT ~ScriptGosubStack(); // 503130

    EXPORT void add_503160(ScriptStackFrame* pFrame);
    EXPORT ScriptStackFrame* remove_503180();

    EXPORT ScriptStackFrame* AllocFrame_5031A0();

    EXPORT void FreeFrame_5031C0(ScriptStackFrame* pFrame);

    EXPORT void remove_5031E0(u8 count);

    ScriptStackFrame* field_0_current;
    u8 field_4_count;
};
GTA2_ASSERT_SIZEOF_ALWAYS(ScriptGosubStack, 0x8)