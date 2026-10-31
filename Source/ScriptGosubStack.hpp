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
    EXPORT ScriptStackFrame* PopFrame();

    EXPORT ScriptStackFrame* AllocFrame();

    EXPORT void FreeFrame(ScriptStackFrame* pFrame);

    EXPORT void PopToDepth(u8 count);

    ScriptStackFrame* mCurrent; // 0x0
    u8 mCount; // 0x4
};
GTA2_ASSERT_SIZEOF_ALWAYS(ScriptGosubStack, 0x8)