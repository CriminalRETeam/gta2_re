#pragma once

#include "types.hpp"

// Pattern: 0x90, 0x90 0xB8 [addr bytes x4] 0xB8 [status bytes x4] 0x90 0x90
#define FUNC_MARKER_ASM(addr, status)\
        __asm nop\
        __asm nop\
        __asm mov eax, addr\
        __asm mov eax, status\
        __asm nop\
        __asm nop\

#define MATCH_FUNC(addr) __declspec(naked) void Marker_##addr() { FUNC_MARKER_ASM(addr, 1) }
#define STUB_FUNC(addr) __declspec(naked) void Marker_##addr() { FUNC_MARKER_ASM(addr, 0) }

#define GTA2_COUNTOF(x) (sizeof(x) / sizeof(*(x)))
#define GTA2_COUNTOF_S(x) ((s32)GTA2_COUNTOF(x))

#if defined(EXPORT_FUNCS)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#if defined(EXPORT_VARS)||defined(IMPORT_VARS)
        #if defined(EXPORT_VARS)
                #define EXPORT_VAR __declspec(dllexport) 
        #elif defined(IMPORT_VARS)
                #define EXPORT_VAR __declspec(dllimport) 
        #endif
#else
        #define EXPORT_VAR
#endif

#if _MSC_VER > 1200
#define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize);
#else
#define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) typedef int static_assert_##structureName[sizeof(structureName) == expectedSize ? 1 : -1];
#endif
