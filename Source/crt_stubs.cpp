#include "crt_stubs.hpp"
#include "Function.hpp"

namespace crt
{
STUB_FUNC(0x5EE316);
long __cdecl ftell(FILE* f)
{
    return ::ftell(f);
}

STUB_FUNC(0x5EE28A);
int __cdecl fseek(FILE* a1, long a2, int a3)
{
    return ::fseek(a1, a2, a3);
}

STUB_FUNC(0x5EE4E4);
FILE* __cdecl fopen(const char* FileName, const char* Mode)
{
    return ::fopen(FileName, Mode);
}

STUB_FUNC(0x5EE46E);
int __cdecl fclose(FILE* f)
{
    return ::fclose(f);
}

STUB_FUNC(0x5EE4F7);
size_t __cdecl fread(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream)
{
    return ::fread(Buffer, ElementSize, ElementCount, Stream);
}

STUB_FUNC(0x5EE6E9);
int __cdecl fgetc(FILE* f)
{
    return ::fgetc(f);
}

} // namespace crt
