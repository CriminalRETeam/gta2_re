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
};
