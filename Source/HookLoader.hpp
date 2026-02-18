#pragma once

#include <windows.h>
#include <string>
#include <map>
#include "types.hpp"

class FunctionCollector
{
  public:
    struct FuncMeta
    {
        u32 mOgAddr;
        u32 mStatus;
    };

    static bool GetFuncMeta(FuncMeta& meta, HMODULE hDll, const char* pName);
    static bool OnImportsExport(LPVOID pContext, HMODULE hDll, const char* pName);
    FunctionCollector(HMODULE hImports);

    std::map<std::string, FuncMeta> mFunctionsToHookMap;

    struct HookEntry
    {
      void* trampoline; // detour trampoline
      void* target;     // original function
      void* detour;     // replacement function
    };
    std::map<std::string, HookEntry*> mHooks;
};

// Dynamically loaded API
extern "C"
{
  typedef void (*TOnEntryFn)(const char* funcName, u32 ogAddr, u32 status);
  typedef void (*TEnumerateFuncsFn)(TOnEntryFn callback);
  __declspec(dllexport) void EnumerateFuncs(TOnEntryFn callback);

  typedef void (*TChangeHookFn)(const char* pFuncName, u32 ogAddr, bool hook);
  __declspec(dllexport) void ChangeHook(const char* pFuncName, u32 ogAddr, bool hook);
}
