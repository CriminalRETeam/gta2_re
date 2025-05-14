#pragma once

#include "types.hpp"
#include <stdio.h>

// Pattern: 0x90, 0x90 0xB8 [addr bytes x4] 0xB8 [status bytes x4] 0x90 0x90
#define FUNC_MARKER_ASM(addr, status) __asm nop __asm nop __asm mov eax, addr __asm mov eax, status __asm nop __asm nop

#if defined(_MSC_VER)
    #define MATCH_FUNC(addr)                   \
        __declspec(naked) void Marker_##addr() \
        {                                      \
            FUNC_MARKER_ASM(addr, 1)           \
        }
    #define STUB_FUNC(addr)                    \
        __declspec(naked) void Marker_##addr() \
        {                                      \
            FUNC_MARKER_ASM(addr, 0)           \
        }

    #if defined(EXPORT_FUNCS)
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT
    #endif

    #if defined(EXPORT_VARS) || defined(IMPORT_VARS)
        #if defined(EXPORT_VARS)
            #define EXPORT_VAR __declspec(dllexport)
        #elif defined(IMPORT_VARS)
            #define EXPORT_VAR __declspec(dllimport)
        #endif
    #else
        #define EXPORT_VAR
    #endif

    // Searches backward from the given code address for a metadata signature.
    // The assumed signature is: two NOPs (0x90, 0x90) followed by a MOV EAX immediate opcode (0xB8).
    // If found, returns the address (as u32) where the pattern starts; otherwise returns 0.
    inline u32 find_func_meta_data(u32 codeAddr)
    {
        // Search backwards up to 10 bytes.
        for (u32 offset = 1; offset <= 100; ++offset)
        {
            u32 candidateAddr = codeAddr - offset;
            // Cast the candidate address into a pointer to bytes.
            const u8* candidate = reinterpret_cast<const u8*>(candidateAddr);
            // Check for the pattern.
            // (Note: Make sure that reading these 3 bytes is safe in your environment.)
            if (candidate[0] == 0x90 &&
                candidate[1] == 0x90 &&
                candidate[2] == 0xB8)
            {
                return (candidate[3]) |
                (candidate[4] << 8) |
                (candidate[5] << 16) |
                (candidate[6] << 24);
            }
        }
        return 0;  // Not found.
    }

    inline void LogNotImplemented(u32 codeAddr)
    {
        printf("NOT IMPLEMENTED: 0x%X\n", find_func_meta_data(codeAddr));
    }
    
    // Call the lable next: to get the ip
    #define GET_IP(var) __asm call $+5; __asm pop var;  
    #define NOT_IMPLEMENTED do {                \
        static bool logged = false;             \
        if (!logged) {                           \
        unsigned int ip;                        \
        __asm { _emit 0xE8 }                     \
        __asm { _emit 0x00 }                     \
        __asm { _emit 0x00 }                     \
        __asm { _emit 0x00 }                     \
        __asm { _emit 0x00 }                     \
        __asm { pop ip }                         \
        LogNotImplemented(ip);                  \
        logged = true;                          \
        }                                       \
    } while (0)
    
#else
    #define MATCH_FUNC(addr)
    #define STUB_FUNC(addr)
    #define EXPORT
    #define EXPORT_VAR
    #define NOT_IMPLEMENTED
    #define __stdcall
#endif

#if defined(_MSC_VER)
    #if _MSC_VER > 1200
        #define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) \
            static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize);
    #else
        #define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) \
            typedef int static_assert_##structureName[sizeof(structureName) == expectedSize ? 1 : -1];
    #endif
#else
    #define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) \
        static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize);
#endif

#define GTA2_COUNTOF(x) (sizeof(x) / sizeof(*(x)))
#define GTA2_COUNTOF_S(x) ((s32)GTA2_COUNTOF(x))

#define GTA2_DELETE_AND_NULL(x) \
    delete x;                   \
    x = 0;
