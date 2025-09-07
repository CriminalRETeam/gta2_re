#include "memory.hpp"
#include "Function.hpp"
#include "error.hpp"
#include <stdlib.h>

// STRING: 105 0x6210b8
#define MEMORY_CPP_STRING "C:\\Splitting\\Gta2\\Source\\Memory.cpp"

// FUNCTION: 105 0x4FE4D0
void* __stdcall Memory::malloc_4FE4D0(size_t Size)
{
    void* result = ::malloc(Size);
    if (!result)
    {
        FatalError_4A38C0(1029, MEMORY_CPP_STRING, 44, Size);
    }

    return result;
}

// FUNCTION: 105 0x4FE510
void* __stdcall Memory::Aligned_malloc_4FE510(s32 requestedSize, void** pUnAlignedPtr)
{
    // Allocate with extra space for alignment
    void* pAlloc = ::malloc(requestedSize + 0x10000);

    // Return the raw unaligned allocation (since we can't free the aligned one)
    *pUnAlignedPtr = pAlloc;

    // Fail if no alloc
    if (!pAlloc)
    {
        FatalError_4A38C0(1029, MEMORY_CPP_STRING, 76, requestedSize + 0x10000);
    }

    // Return aligned pointer
    return (void*)(((u32)*pUnAlignedPtr + 0xFFFF) & 0xFFFF0000);
}