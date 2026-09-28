#pragma once

#include "types.hpp"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class GlobalRef;

// Defined in GlobalsRegistry.hpp. Not included here because <vector> pulls in <new>, whose
// throw() operator delete makes VC6 drop the EH frames that the original has in destructors.
class GlobalsRegistry;

// Export as plain C function with no name mangling so that
// the HookLoader can easily find and special case it.
extern "C"
{
    __declspec(dllexport) GlobalsRegistry* __cdecl GetGlobalsRegistry();
}

typedef GlobalsRegistry*(__cdecl* TGetGlobalsRegistry)();

class GlobalRef
{
  public:
    GlobalRef(void* pVar, u32 addr, u32 size);
    const void* mVar;
    const u32 mOgAddr;
    const u32 mSize;
  };

#define GLOBAL(var, addr) const GlobalRef gRef_##var##_##addr(&var, addr, sizeof(var));