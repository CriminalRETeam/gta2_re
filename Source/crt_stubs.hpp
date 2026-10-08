#pragma once
#include "Function.hpp"
#include <stdio.h>
#include <stdlib.h>

// crt proxy functions - required so that the hook build has fopen/fread/any crt function
// that we call using the same "copy" of the crt. I.e the one in the executable, not the one
// linked into the hook dll, else ka boom m8.
// Mixing VC6+ CRT structs with VC65CRT structs will likely crash also, howwever if all CRT funcs are matched or stubbed
// they'll use the same copy of the structure and it will be fine...

namespace crt
{
EXPORT long __cdecl ftell(FILE*);
EXPORT int __cdecl fseek(FILE*, long, int);
EXPORT FILE* __cdecl fopen(const char* FileName, const char* Mode);
EXPORT int __cdecl fclose(FILE*);
EXPORT size_t __cdecl fread(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream);
EXPORT int __cdecl fgetc(FILE*);
EXPORT size_t __cdecl fwrite(const void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream);

EXPORT void __cdecl free(void* Block);
EXPORT void* __cdecl malloc(size_t Size);

// TODO: crt::malloc, crt::free

} // namespace crt

// Out-of-line copies of the empty Iostream_init constructor (VC98 iostream.h owns that name), one per translation unit that
// pulled <iostream> in. Each is `mov %ecx,%eax; ret`: a constructor returning this.
struct Iostream_init_ool
{
    EXPORT Iostream_init_ool* ctor_copy_419DF0();
    EXPORT Iostream_init_ool* ctor_copy_531940();
};
