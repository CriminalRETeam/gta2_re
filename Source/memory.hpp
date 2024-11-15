#pragma once

#include "Function.hpp"

class Memory
{
public:
    EXPORT static void* __stdcall malloc_4FE4D0(size_t Size);

    EXPORT static void* __stdcall Aligned_malloc_4FE510(s32 requestedSize, void** pAlignedSize);
};
