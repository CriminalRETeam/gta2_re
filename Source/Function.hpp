#pragma once

#include "Globals.hpp"
#include "types.hpp"

#if _MSC_VER > 1200
    #include <stdio.h> // printf
#endif

// Pattern: 0x90, 0x90 0xB8 [addr bytes x4] 0xB8 [status bytes x4] 0x90 0x90
#define FUNC_MARKER_ASM(addr, status) __asm nop __asm nop __asm mov eax, addr __asm mov eax, status __asm nop __asm nop

#if defined(_MSC_VER)
    #define MATCH_FUNC(addr)                   \
        __declspec(naked) __declspec(dllexport) void Marker_##addr() \
        {                                      \
            FUNC_MARKER_ASM(addr, 1)           \
        }
    #define STUB_FUNC(addr)                    \
        __declspec(naked) __declspec(dllexport) void Marker_##addr() \
        {                                      \
            FUNC_MARKER_ASM(addr, 0)           \
        }

    #if defined(EXPORT_FUNCS)
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT
    #endif

    #if defined(EXPORT_VARS) && defined(IMPORT_VARS)
        #error "EXPORT_VARS and IMPORT_VARS should not be defined together!"
    #endif

    #if defined(EXPORT_VARS) || defined(IMPORT_VARS)
        #if defined(EXPORT_VARS)
            #define DEFINE_GLOBAL(type, name) \
                __declspec(dllexport) type name;    

            #define DEFINE_GLOBAL_INIT(type, name, value) \
                __declspec(dllexport) type name = value;        

            #define DEFINE_GLOBAL_ARRAY(type, name, size) \
                __declspec(dllexport) type name[size];          

            //#define DEFINE_GLOBAL_ARRAY_INIT(type, name, size, ...) \
            //   __declspec(dllexport) type name[size] = {__VA_ARGS__};

            #define EXTERN_GLOBAL(type, name) __declspec(dllexport) extern type name
            #define EXTERN_GLOBAL_ARRAY(type, name, size) __declspec(dllexport) extern type name[size]

        #elif defined(IMPORT_VARS)
            // define
            #define DEFINE_GLOBAL(type, name) __declspec(dllimport) type name
            #define DEFINE_GLOBAL_INIT(type, name, value) __declspec(dllimport) type name;

            #define DEFINE_GLOBAL_ARRAY(type, name, size) __declspec(dllimport) type name[size]
           // #define DEFINE_GLOBAL_ARRAY_INIT(type, name, size, ...) __declspec(dllimport) type name[size];
            // extern
            #define EXTERN_GLOBAL(type, name) __declspec(dllimport) extern type name
            #define EXTERN_GLOBAL_ARRAY(type, name, size) __declspec(dllimport) extern type name[size]
        #endif
    #else
        // Static build
        #define DEFINE_GLOBAL(type, name) \
            type name;                          

        #define DEFINE_GLOBAL_INIT(type, name, value) \
            type name = value;                              

        #define DEFINE_GLOBAL_ARRAY(type, name, size) \
            type name[size];                                

        //#define DEFINE_GLOBAL_ARRAY_INIT(type, name, size, ...) \
        //    type name[size] = {__VA_ARGS__};                          

        #define EXTERN_GLOBAL(type, name) extern type name
        #define EXTERN_GLOBAL_ARRAY(type, name, size) extern type name[size]
    #endif

    #if defined(__clang__) || (_MSC_VER <= 1200)
void __stdcall LogNotImplemented(u32 codeAddr);

        #define GET_IP(var)   \
            __asm call $ + 5; \
            __asm pop var;

        #define NOT_IMPLEMENTED                                                                                                       \
            do                                                                                                                        \
            {                                                                                                                         \
                static bool logged = false;                                                                                           \
                if (!logged)                                                                                                          \
                {                                                                                                                     \
                    unsigned int ip;                                                                                                  \
                    __asm { _emit 0xE8 }                                                                                                \
                    __asm {_emit 0x00} __asm {_emit 0x00} __asm {_emit 0x00} __asm {_emit 0x00} __asm {pop ip} LogNotImplemented(ip); \
                    logged = true;                                                                                                    \
                }                                                                                                                     \
            } while (0)
    #else
        #define NOT_IMPLEMENTED                               \
            static bool done___ = false;                      \
            if (!done___)                                     \
            {                                                 \
                done___ = true;                               \
                printf("NOT IMPLEMENTED %s\n", __FUNCTION__); \
            }
    #endif

#else
    #define MATCH_FUNC(addr)
    #define STUB_FUNC(addr)
    #define EXPORT
    #define NOT_IMPLEMENTED
    #define __stdcall

    #define DEFINE_GLOBAL(type, name) type name
    #define DEFINE_GLOBAL_INIT(type, name, value) type name = value

    #define DEFINE_GLOBAL_ARRAY(type, name, size) type name[size]
//#define DEFINE_GLOBAL_ARRAY_INIT(type, name, size, ...)  type name[size] = { __VA_ARGS__ }

    #define EXTERN_GLOBAL(type, name) extern type name
    #define EXTERN_GLOBAL_ARRAY(type, name, size) extern type name[size]
#endif

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
#define GTA2_COUNTOF_S(x) ((s32)GTA2_COUNTOF(x))

#define GTA2_DELETE_AND_NULL(x) \
    delete x;                   \
    x = 0;