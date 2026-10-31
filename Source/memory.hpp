#pragma once

#include "Function.hpp"

class Memory
{
  public:
    EXPORT static void* __stdcall Checked_malloc(size_t size);

    EXPORT static void* __stdcall Aligned_malloc(s32 requestedSize, void** pUnAlignedPtr);
};