#include <windows.h>

#include "3rdParty/Detours/include/detours.h"
#include "3rdParty/Manual-DLL-Loader/Source/Manual Loader/Loader.h"
#include "Globals.hpp"
#include <map>
#include <string>

// patch 10.5.exe -> imports to load HookLoader.dll
// HookLoader.dll -> manual load of gta2_dll_imports.dll via DllMain which will never return - hooked at executable entry that does CRT
// start up so that control of the global static init list can be controlled in the future if needed.
// - redirects imported vars to resolve to locations within 10.5.exe (TODO: How to get vars table?)
// - jmp hooks 10.5.exe functions to call matched functions (via exports enumeration) (TODO: How to get function table?)
// - jmp hooks exported functions that are stubbed to jmp to 10.5.exe (via exports enumeration) (TODO: How to get function table?)

HMODULE hFake = (HMODULE)0xdeadbabe;

class GTA2LoaderCallBacks : public ILoaderCallBacks
{
  public:
    GTA2LoaderCallBacks(const std::map<std::string, u32>& funcMap) : mFuncMap(funcMap)
    {
    }

    virtual HMODULE VLoadLibraryA(const char* pDll)
    {
        if (stricmp(pDll, "gta2_dll_exports.dll") == 0)
        {
            return hFake;
        }
        else
        {
            return ::LoadLibraryA(pDll);
        }
    }

    virtual FARPROC VGetProcAddress(HMODULE hModule, const char* pFuncName)
    {
        if (hModule == hFake)
        {
            std::map<std::string, u32>::const_iterator it = mFuncMap.find(pFuncName);
            if (it == mFuncMap.end())
            {
                printf("Function not found %s!!\n", pFuncName);
                return 0;
            }
            else
            {
                printf("Resolved %s to %X\n", pFuncName, it->second);
                return (FARPROC)it->second;
            }
        }
        else
        {
            return ::GetProcAddress(hModule, pFuncName);
        }
    }

  private:
    const std::map<std::string, u32>& mFuncMap;
};

typedef bool (*TExportCb)(LPVOID, LPVOID, const char*);
static void EnumExports(LPVOID pContext, LPVOID dllAlloc, TExportCb cb)
{
    PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)((BYTE*)dllAlloc + ((PIMAGE_DOS_HEADER)dllAlloc)->e_lfanew);
    //assert(header->Signature == IMAGE_NT_SIGNATURE);
    //assert(header->OptionalHeader.NumberOfRvaAndSizes > 0);
    PIMAGE_EXPORT_DIRECTORY exports =
        (PIMAGE_EXPORT_DIRECTORY)((BYTE*)dllAlloc + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    //assert(exports->AddressOfNames != 0);
    BYTE** names = (BYTE**)((int)dllAlloc + exports->AddressOfNames);
    for (int i = 0; i < exports->NumberOfNames; i++)
    {
        char* pName = (char*)((BYTE*)dllAlloc + (int)names[i]);
        if (!cb(pContext, dllAlloc, pName))
        {
            return;
        }
    }
}

class HookLoader
{
    struct FuncMeta
    {
        u32 mOgAddr;
        u32 mStatus;
    };

  private:
    static bool OnExportsExport(LPVOID pContext, LPVOID hDll, const char* pName)
    {
        if (stricmp(pName, "GetGlobalsRegistry") == 0)
        {
            // Skip - its a function, everything else is a var
        }
        else
        {
            FARPROC varAddr = GetProcAddress((HINSTANCE)hDll, pName);

            HookLoader* hl = reinterpret_cast<HookLoader*>(pContext);

            std::map<const void*, u32>::iterator it = hl->mGlobalEntryToOgAddrMap.find(varAddr);
            if (it != hl->mGlobalEntryToOgAddrMap.end())
            {
                printf("Found! %s\n", pName);
                hl->mExportNameToOgAddrMap[std::string(pName)] = it->second;
            }
            else
            {
                printf("Didn't find %s (its over fr)\n", pName);
            }
        }
        return true;
    }

    static bool GetFuncMeta(FuncMeta& meta, LPVOID hDll, const char* pName)
    {
        u8* pFun = reinterpret_cast<u8*>(MemoryLoader::GetFunctionAddress(hDll, pName));
        if (!pFun)
        {
            return false;
        }

        /*
        Looking backwards for the pattern:
        90
        90
        B8 XX XX XX XX (og addr)
        B8 XX XX XX XX (status)
        90
        90
        */
        u8* pFunStart = pFun;
        for (u32 loopBackPos = 1; loopBackPos <= 10; loopBackPos++)
        {
            // find double nop
            if (*pFun == 0x90 && *(pFun - 1) == 0x90)
            {
                // find double mov eax, xyz, mov eax xyz
                if (*(pFun - 6) == 0xB8 && *(pFun - 11) == 0xB8)
                {
                    // find final double nop
                    if (*(pFun - 12) == 0x90 && *(pFun - 13) == 0x90)
                    {
                        meta.mOgAddr = *reinterpret_cast<u32*>(&(pFun[-10]));
                        meta.mStatus = *reinterpret_cast<u32*>(&(pFun[-5]));
                        //printf("og addr %X status %d for %s\n", meta.mOgAddr, meta.mStatus, pName);
                        return true;
                    }
                }
            }
            pFun--;
        }

        return false;
    }

    static bool OnImportsExport(LPVOID pContext, LPVOID hDll, const char* pName)
    {
        FuncMeta meta;
        if (GetFuncMeta(meta, hDll, pName))
        {
            reinterpret_cast<HookLoader*>(pContext)->mFunctionsToHookMap[pName] = meta;
        }
        else
        {
            printf("WARNING: No meta for %s\n", pName);
        }
        return true;
    }

    void MapImportedVarsToOgAddrs()
    {
        printf("Load imports dll\n");
        HMODULE hMod2 = LoadLibraryA("gta2_dll_exports.dll");
        printf("Enumerating exports..\n");
        EnumExports(this, (LPVOID)hMod2, OnExportsExport);
        FreeLibrary(hMod2);
    }

    typedef int(__cdecl* PT1)(int);
    typedef int (*TP2)();
    typedef void (*TP3)();

    void crt_inits()
    {
        printf("Heap\n");
        if (!((PT1)(0x5F116B))(0))
        {
            printf("Heap failed!\n");
        }
        printf("Heap done\n");

        printf("_ioinit\n");
        ((TP3)(0x5F0527))();
        printf("_ioinit done\n");

        *(char**)0x7099E4 = GetCommandLineA();

        *((char**)0x7082F8) = (char*)((TP2)(0x5F6D61))(); // __crtGetEnvironmentStringsA

        printf("_setargv\n");
        ((TP2)(0x5F6B14))();
        printf("_setargv done\n");

        printf("_setenvp\n");
        ((TP3)(0x5F6A5B))();
        printf("_setenvp done\n");

        printf("_cinit\n");
        ((TP3)(0x5EE16A))();
        printf("_cinit done\n");

        // TODO: Bin off static init funcs that are impl'd
        // leaving till we get to a point where double static init calls actually cause a crash etc
    }

  public:
    void LoadHooks()
    {
        printf("Load exports dll\n");
        HMODULE hMod = LoadLibraryA("gta2_dll_exports.dll");
        printf("Get globals registry %X\n", hMod);
        TGetGlobalsRegistry pFnGlobalsRegistry = (TGetGlobalsRegistry)GetProcAddress(hMod, "GetGlobalsRegistry");
        printf("Collect export entries %X\n", pFnGlobalsRegistry);
        GlobalsRegistry* pGlobalsRegistry = pFnGlobalsRegistry();

        printf("Enumerate entries %X count %d\n", pGlobalsRegistry, pGlobalsRegistry->mGlobals.size());

        for (size_t i = 0; i < pGlobalsRegistry->mGlobals.size(); i++)
        {
            mGlobalEntryToOgAddrMap[pGlobalsRegistry->mGlobals[i]->mVar] = pGlobalsRegistry->mGlobals[i]->mOgAddr;
        }

        MapImportedVarsToOgAddrs();
        printf("Unload exports dll\n");
        FreeLibrary(hMod);

        printf("Load imports dll\n");
        GTA2LoaderCallBacks cb(mExportNameToOgAddrMap);
        LPVOID lpModule = MemoryLoader::LoadDLL("gta2_dll_imports.dll", cb);
        if (lpModule == NULL)
        {
        }

        // Replicate what start() does enough to make things work
        printf("crt inits\n");
        crt_inits();

        EnumExports(this, lpModule, OnImportsExport);

        LONG err = DetourTransactionBegin();
        if (err != NO_ERROR)
        {
            printf("DetourTransactionBegin failed\n");
        }

        err = DetourUpdateThread(GetCurrentThread());
        if (err != NO_ERROR)
        {
            printf("DetourUpdateThread failed");
        }

        printf("Apply hooks..\n");
        for (std::map<std::string, FuncMeta>::iterator it = mFunctionsToHookMap.begin(); it != mFunctionsToHookMap.end(); it++)
        {
            {
                printf("Look up %s\n",  it->first.c_str());
                LPVOID addr = MemoryLoader::GetFunctionAddress(lpModule, it->first.c_str());
                const FuncMeta& meta = it->second;
                printf("Doing %s\n",  it->first.c_str());
                if (meta.mStatus == 0)
                {
                    // stubbed - hook reimpl func to og
                    //printf("STUB %s\n", it->first.c_str());

                    err = DetourAttach(&(PVOID&)addr, reinterpret_cast<void*>(meta.mOgAddr));
                }
                else if (meta.mStatus == 1)
                {
                    // matched - hook og func to reimpl
                    //printf("MATCH %s\n", it->first.c_str());
                    LPVOID addr2 = reinterpret_cast<void*>(meta.mOgAddr);
                    err = DetourAttach(&(PVOID&)addr2, addr);
                }
                if (err != NO_ERROR)
                {
                    printf("DetourAttach failed\n");
                }
            }
        }

        printf("commit hooks\n");
        err = DetourTransactionCommit();
        if (err != NO_ERROR)
        {
            printf("DetourTransactionCommit failed\n");
        }

        err = DetourTransactionBegin();

        if (err != NO_ERROR)
        {
            printf("DetourTransactionBegin failed\n");
        }

        err = DetourUpdateThread(GetCurrentThread());

        if (err != NO_ERROR)
        {
            printf("DetourUpdateThread failed\n");
        }

        printf("Look up GameMain...\n");
        LPVOID pGameMain = MemoryLoader::GetFunctionAddress(lpModule, "GameMain");
        typedef void(__cdecl * TGameMain)();

        printf("GameMain = %X\n", pGameMain);

        TGameMain pTypedGameMain = (TGameMain)pGameMain;
        pTypedGameMain();
        printf("Game main returned\n");
    }

  private:
    std::map<const void*, u32> mGlobalEntryToOgAddrMap;
    std::map<std::string, u32> mExportNameToOgAddrMap;
    std::map<std::string, FuncMeta> mFunctionsToHookMap;
};

// The OG patched executable will call this function (outside of DllMain/OS loader locks etc)
extern "C"
{
    __declspec(dllexport) __cdecl HookMain()
    {
        HookLoader hl;
        hl.LoadHooks();
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, // handle to DLL module
                    DWORD fdwReason, // reason for calling function
                    LPVOID lpvReserved) // reserved
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE; // Successful DLL_PROCESS_ATTACH.
}
