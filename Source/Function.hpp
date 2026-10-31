#pragma once

#include "Globals.hpp"
#include "types.hpp"

#if _MSC_VER > 1200
    #include <stdio.h> // printf
#endif

// The empty Marker_<addr>_fpo() after each naked marker matters: C2 sizes [esp+x] operands (when deciding
// to copy a small tail block or jump to it, <= 20 bytes is copied) using flags left by the previously
// compiled function. A naked/asm function leaves them off, an FPO function leaves them on as in the
// original. See "How VC6 copies and merges block tails" in docs/matching_quirks.md.
// Pattern: 0x90, 0x90 0xB8 [addr bytes x4] 0xB8 [status bytes x4] 0x90 0x90
#define FUNC_MARKER_ASM(addr, status) __asm nop __asm nop __asm mov eax, addr __asm mov eax, status __asm nop __asm nop

// Variadic macros were added in C99, but this is not supported by VC6.
// We need to use variadic args for the DEFINE_GLOBAL_ARRAY_INIT macro.
// If we use a regular parameter instead of the ellipsis and ignore the
// C4002 "too many parameters for macro" warning, this will not properly
// initialize the array. An array of structs will fail to compile at all.
// This leads us to this ugly hack to include commas in macro expansion
// but not confuse the compiler with extra parameters.
#define COMMA ,

#if defined(_MSC_VER)
    #define WIP_FUNC(addr)                                           \
        __declspec(naked) __declspec(dllexport) void Marker_##addr() \
        {                                                            \
            FUNC_MARKER_ASM(addr, 2)                                 \
        } \
        __declspec(dllexport) void Marker_##addr##_fpo() {}
    #define MATCH_FUNC(addr)                                         \
        __declspec(naked) __declspec(dllexport) void Marker_##addr() \
        {                                                            \
            FUNC_MARKER_ASM(addr, 1)                                 \
        } \
        __declspec(dllexport) void Marker_##addr##_fpo() {}
    #define STUB_FUNC(addr)                                          \
        __declspec(naked) __declspec(dllexport) void Marker_##addr() \
        {                                                            \
            FUNC_MARKER_ASM(addr, 0)                                 \
        } \
        __declspec(dllexport) void Marker_##addr##_fpo() {}

    #if defined(EXPORT_FUNCS)
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT
    #endif

    #if defined(EXPORT_VARS) && defined(IMPORT_VARS)
        #error "EXPORT_VARS and IMPORT_VARS should not be defined together!"
    #endif

    // Statics keep the original address in their macro. File statics (DEFINE_STATIC_GLOBAL*) and LOCAL_STATIC* are
    // private to each build, so the hook DLLs don't redirect them to 10.5.exe; class statics are exported like globals.
    #if defined(EXPORT_VARS) || defined(IMPORT_VARS)
        #if defined(EXPORT_VARS)
            #define DEFINE_GLOBAL(type, name, addr) \
                __declspec(dllexport) type name;    \
                GLOBAL(name, addr)

            #define DEFINE_STATIC_GLOBAL(type, name, addr) \
                static type name;                          \
                GLOBAL_STATIC(name, addr)

            #define DEFINE_STATIC_GLOBAL_INIT(type, name, value, addr) \
                static type name = value;                              \
                GLOBAL_STATIC(name, addr)

            #define DEFINE_STATIC_GLOBAL_ARRAY(type, name, size, addr) \
                static type name[size];                                \
                GLOBAL_STATIC(name, addr)

            #define DEFINE_STATIC_GLOBAL_ARRAY_INIT(type, name, size, addr, va_args) \
                static type name[size] = {va_args};                                 \
                GLOBAL_STATIC(name, addr)

            #define DEFINE_GLOBAL_INIT(type, name, value, addr) \
                __declspec(dllexport) type name = value;        \
                GLOBAL(name, addr)

            #define DEFINE_GLOBAL_ARRAY(type, name, size, addr) \
                __declspec(dllexport) type name[size];          \
                GLOBAL(name, addr)

            #define DEFINE_GLOBAL_ARRAY_INIT(type, name, size, addr, va_args) \
                __declspec(dllexport) type name[size] = {va_args};            \
                GLOBAL(name, addr)

            #define EXTERN_GLOBAL(type, name) __declspec(dllexport) extern type name
            #define EXTERN_GLOBAL_ARRAY(type, name, size) __declspec(dllexport) extern type name[size]

            #define STATIC_MEMBER(type, name) static __declspec(dllexport) type name
            #define STATIC_MEMBER_ARRAY(type, name, size) static __declspec(dllexport) type name[size]
            #define DEFINE_STATIC_MEMBER(type, cls, name, addr) \
                type cls::name;                             \
                GLOBAL_MEMBER(cls, name, addr)
            #define DEFINE_STATIC_MEMBER_INIT(type, cls, name, value, addr) \
                type cls::name = value;                                 \
                GLOBAL_MEMBER(cls, name, addr)
            #define DEFINE_STATIC_MEMBER_ARRAY(type, cls, name, size, addr) \
                type cls::name[size];                                   \
                GLOBAL_MEMBER(cls, name, addr)
            #define DEFINE_STATIC_MEMBER_ARRAY_INIT(type, cls, name, size, addr, va_args) \
                type cls::name[size] = {va_args};                                     \
                GLOBAL_MEMBER(cls, name, addr)

        #elif defined(IMPORT_VARS)
            // define
            #define DEFINE_GLOBAL(type, name, addr) __declspec(dllimport) type name
            #define DEFINE_STATIC_GLOBAL(type, name, addr) static type name
            #define DEFINE_STATIC_GLOBAL_INIT(type, name, value, addr) static type name = value
            #define DEFINE_STATIC_GLOBAL_ARRAY(type, name, size, addr) static type name[size]
            #define DEFINE_STATIC_GLOBAL_ARRAY_INIT(type, name, size, addr, va_args) static type name[size] = {va_args}
            #define DEFINE_GLOBAL_INIT(type, name, value, addr) __declspec(dllimport) type name;

            #define DEFINE_GLOBAL_ARRAY(type, name, size, addr) __declspec(dllimport) type name[size]
            #define DEFINE_GLOBAL_ARRAY_INIT(type, name, size, addr, va_args) __declspec(dllimport) type name[size];
            // extern
            #define EXTERN_GLOBAL(type, name) __declspec(dllimport) extern type name
            #define EXTERN_GLOBAL_ARRAY(type, name, size) __declspec(dllimport) extern type name[size]

            // The member is defined in gta2_dll_exports; a dllimport member can't be defined here
            #define STATIC_MEMBER(type, name) static __declspec(dllimport) type name
            #define STATIC_MEMBER_ARRAY(type, name, size) static __declspec(dllimport) type name[size]
            #define DEFINE_STATIC_MEMBER(type, cls, name, addr)
            #define DEFINE_STATIC_MEMBER_INIT(type, cls, name, value, addr)
            #define DEFINE_STATIC_MEMBER_ARRAY(type, cls, name, size, addr)
            #define DEFINE_STATIC_MEMBER_ARRAY_INIT(type, cls, name, size, addr, va_args)
        #endif
    #else
        // Static build
        #define DEFINE_GLOBAL(type, name, addr) \
            type name;                          \
            GLOBAL(name, addr)

        #define DEFINE_STATIC_GLOBAL(type, name, addr) \
            static type name;                          \
            GLOBAL_STATIC(name, addr)

        #define DEFINE_STATIC_GLOBAL_INIT(type, name, value, addr) \
            static type name = value;                              \
            GLOBAL_STATIC(name, addr)

        #define DEFINE_STATIC_GLOBAL_ARRAY(type, name, size, addr) \
            static type name[size];                                \
            GLOBAL_STATIC(name, addr)

        #define DEFINE_STATIC_GLOBAL_ARRAY_INIT(type, name, size, addr, va_args) \
            static type name[size] = {va_args};                                 \
            GLOBAL_STATIC(name, addr)

        #define DEFINE_GLOBAL_INIT(type, name, value, addr) \
            type name = value;                              \
            GLOBAL(name, addr)

        #define DEFINE_GLOBAL_ARRAY(type, name, size, addr) \
            type name[size];                                \
            GLOBAL(name, addr)

        #define DEFINE_GLOBAL_ARRAY_INIT(type, name, size, addr, va_args) \
            type name[size] = {va_args};                                  \
            GLOBAL(name, addr)

        #define EXTERN_GLOBAL(type, name) extern type name
        #define EXTERN_GLOBAL_ARRAY(type, name, size) extern type name[size]

        #define STATIC_MEMBER(type, name) static type name
        #define STATIC_MEMBER_ARRAY(type, name, size) static type name[size]
        #define DEFINE_STATIC_MEMBER(type, cls, name, addr) \
            type cls::name;                             \
            GLOBAL_MEMBER(cls, name, addr)
        #define DEFINE_STATIC_MEMBER_INIT(type, cls, name, value, addr) \
            type cls::name = value;                                 \
            GLOBAL_MEMBER(cls, name, addr)
        #define DEFINE_STATIC_MEMBER_ARRAY(type, cls, name, size, addr) \
            type cls::name[size];                                   \
            GLOBAL_MEMBER(cls, name, addr)
        #define DEFINE_STATIC_MEMBER_ARRAY_INIT(type, cls, name, size, addr, va_args) \
            type cls::name[size] = {va_args};                                     \
            GLOBAL_MEMBER(cls, name, addr)
    #endif

    #if defined(__clang__) || (_MSC_VER <= 1200)
void __stdcall LogFuncAddr(u32 codeAddr, s32 mode);

        #define GET_IP(var)   \
            __asm call $ + 5; \
            __asm pop var;

        #define NOT_IMPLEMENTED                                                                                                    \
            do                                                                                                                     \
            {                                                                                                                      \
                static bool logged = false;                                                                                        \
                if (!logged)                                                                                                       \
                {                                                                                                                  \
                    u32 ip;                                                                                                        \
                    __asm { _emit 0xE8 }                                                                                             \
                    __asm {_emit 0x00} __asm {_emit 0x00} __asm {_emit 0x00} __asm {_emit 0x00} __asm {pop ip} LogFuncAddr(ip, 0); \
                    logged = true;                                                                                                 \
                }                                                                                                                  \
            } while (0)

        #define WIP_IMPLEMENTED                                                                                                    \
            do                                                                                                                     \
            {                                                                                                                      \
                static bool logged = false;                                                                                        \
                if (!logged)                                                                                                       \
                {                                                                                                                  \
                    u32 ip;                                                                                                        \
                    __asm { _emit 0xE8 }                                                                                             \
                    __asm {_emit 0x00} __asm {_emit 0x00} __asm {_emit 0x00} __asm {_emit 0x00} __asm {pop ip} LogFuncAddr(ip, 1); \
                    logged = true;                                                                                                 \
                }                                                                                                                  \
            } while (0)

        // build.py --no_wip_logging: no logging code in WIP/stub functions, so their asm can be
        // compared with the original's (Scripts/report_progress_to_discord.py scores the WIPs on it)
        #ifdef NO_WIP_LOGGING
            #undef NOT_IMPLEMENTED
            #define NOT_IMPLEMENTED
            #undef WIP_IMPLEMENTED
            #define WIP_IMPLEMENTED
        #endif
    #else
        #define NOT_IMPLEMENTED                               \
            static bool done___ = false;                      \
            if (!done___)                                     \
            {                                                 \
                done___ = true;                               \
                printf("NOT IMPLEMENTED %s\n", __FUNCTION__); \
            }

        #define WIP_IMPLEMENTED                               \
            static bool done___ = false;                      \
            if (!done___)                                     \
            {                                                 \
                done___ = true;                               \
                printf("WIP IMPLEMENTED %s\n", __FUNCTION__); \
            }
    #endif

#else
    #define WIP_FUNC(addr)
    #define MATCH_FUNC(addr)
    #define STUB_FUNC(addr)
    #define EXPORT
    #define NOT_IMPLEMENTED
    #define WIP_IMPLEMENTED
    #define __stdcall

    #define DEFINE_GLOBAL(type, name, addr) type name
    #define DEFINE_STATIC_GLOBAL(type, name, addr) static type name
    #define DEFINE_STATIC_GLOBAL_INIT(type, name, value, addr) static type name = value
    #define DEFINE_STATIC_GLOBAL_ARRAY(type, name, size, addr) static type name[size]
    #define DEFINE_GLOBAL_INIT(type, name, value, addr) type name = value

    #define DEFINE_GLOBAL_ARRAY(type, name, size, addr) type name[size]

    #define EXTERN_GLOBAL(type, name) extern type name
    #define EXTERN_GLOBAL_ARRAY(type, name, size) extern type name[size]

    #define STATIC_MEMBER(type, name) static type name
    #define STATIC_MEMBER_ARRAY(type, name, size) static type name[size]
    #define DEFINE_STATIC_MEMBER(type, cls, name, addr) type cls::name
    #define DEFINE_STATIC_MEMBER_INIT(type, cls, name, value, addr) type cls::name = value
    #define DEFINE_STATIC_MEMBER_ARRAY(type, cls, name, size, addr) type cls::name[size]
#endif

// A static inside the one function that uses it. Only for constant initializers: a constructor would add an init
// guard the original doesn't have.
#define LOCAL_STATIC(type, name, addr) static type name
#define LOCAL_STATIC_INIT(type, name, value, addr) static type name = value
#define LOCAL_STATIC_ARRAY(type, name, size, addr) static type name[size]
#define LOCAL_STATIC_ARRAY_INIT(type, name, size, addr, va_args) static type name[size] = {va_args}

#if defined(_MSC_VER)
    #if defined(__clang__) || (_MSC_VER <= 1200)
        #define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) \
            typedef int static_assert_##structureName[sizeof(structureName) == expectedSize ? 1 : -1];
    #else
        #define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) \
            static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize);
    #endif
#else
    #define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) \
        static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize);
#endif

#define GTA2_COUNTOF(x) (sizeof(x) / sizeof(*(x)))
#define GTA2_COUNTOF_S(x) (static_cast<s32>(GTA2_COUNTOF(x)))

#define GTA2_DELETE_AND_NULL(x) \
    delete x;                   \
    x = NULL;