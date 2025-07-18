#include "crt_stubs.hpp"
#include "Function.hpp"
#include <memory>

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

STUB_FUNC(0x5EE5DF);
EXPORT size_t __cdecl fwrite(const void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream)
{
    return ::fwrite(Buffer, ElementSize, ElementCount, Stream);
}

STUB_FUNC(0x5ED478)
void __cdecl free(void* Block)
{
    ::free(Block);
}

STUB_FUNC(0x5ED3BB)
void* __cdecl malloc(size_t Size)
{
    return ::malloc(Size);
}

} // namespace crt

// WARNING: This might cause some mixed new/delete of 2 crt versions due to hooks 
// getting applied mid-way. However since after WinMain is called exit() is also
// called we never "return" to the hook manager code nor ever remove any hooks therefore
// this shouldn't be a problem but is something to be aware of.
void* operator new(size_t n) throw(std::bad_alloc)
{
    //printf("new %d\n", n);
    return crt::malloc(n);
}
void operator delete(void* p) throw()
{
    crt::free(p);
}

void* operator new[](size_t s) throw(std::bad_alloc)
{
    //printf("new[] %d\n", s);
    return crt::malloc(s);
}

void operator delete[](void* p) throw()
{
    crt::free(p);
}