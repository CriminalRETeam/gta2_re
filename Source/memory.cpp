#include "memory.hpp"
#include "Function.hpp"
#include "error.hpp"
#include "crt_stubs.hpp"
#include <stdlib.h>
#include "enums.hpp"

MATCH_FUNC(0x4FE4D0)
void* __stdcall Memory::malloc_4FE4D0(size_t size)
{
    void* result = crt::malloc(size);
    if (!result)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemory, "C:\\Splitting\\Gta2\\Source\\Memory.cpp", 44, size);
    }

    return result;
}

MATCH_FUNC(0x4FE510)
void* __stdcall Memory::Aligned_malloc_4FE510(s32 requestedSize, void** pUnAlignedPtr)
{
    // 64 KB extra so that the block can be aligned to 64 KB
    void* pAlloc = crt::malloc(requestedSize + 0x10000);

    // The caller frees this one, not the aligned pointer
    *pUnAlignedPtr = pAlloc;

    if (!pAlloc)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemory, "C:\\Splitting\\Gta2\\Source\\Memory.cpp", 76, requestedSize + 0x10000);
    }

    return reinterpret_cast<void*>((reinterpret_cast<u32>(*pUnAlignedPtr) + 0xFFFF) & 0xFFFF0000);
}