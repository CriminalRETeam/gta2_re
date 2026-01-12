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

//#define HOOK_VERBOSE

typedef bool (*TExportCb)(LPVOID, HMODULE, const char*);
static void EnumExports(LPVOID pContext, HMODULE dllAlloc, TExportCb cb)
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

template<class T, class Y>
static void SetFuncField(T& toSet, Y value)
{
    memcpy(&toSet, &value, 4);
}

static void RewriteImports(HMODULE hImports, GlobalsRegistry* pGlobals)
{
    const PIMAGE_DOS_HEADER lpImageDOSHeader = (PIMAGE_DOS_HEADER)hImports;
    const PIMAGE_NT_HEADERS lpImageNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpImageDOSHeader + lpImageDOSHeader->e_lfanew);
    const PIMAGE_SECTION_HEADER lpImageSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)lpImageNTHeader + 4 + sizeof(IMAGE_FILE_HEADER) +
                                                                               lpImageNTHeader->FileHeader.SizeOfOptionalHeader);

    PIMAGE_IMPORT_DESCRIPTOR lpImageImportDescriptor =
        (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)hImports +
                                   lpImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    if (lpImageImportDescriptor == NULL)
    {
        printf("[-] An error is occured when tying to get the import descriptor !\n");
        return;
    }

    while (lpImageImportDescriptor->Name != 0)
    {
        const LPSTR lpLibraryName = (LPSTR)((DWORD_PTR)hImports + lpImageImportDescriptor->Name);

        printf("[+] Loading %s\n", lpLibraryName);

        if (stricmp(lpLibraryName, "gta2_dll_exports.dll") == 0)
        {
            PIMAGE_THUNK_DATA lpThunkData = (PIMAGE_THUNK_DATA)((DWORD_PTR)hImports + lpImageImportDescriptor->FirstThunk);
            while (lpThunkData->u1.AddressOfData != 0)
            {
                if (IMAGE_SNAP_BY_ORDINAL(lpThunkData->u1.Ordinal))
                {
                    //const UINT functionOrdinal = (UINT)IMAGE_ORDINAL(lpThunkData->u1.Ordinal);
                    //lpThunkData->u1.Function = (DWORD)callBacks.VGetProcAddress(hModule, MAKEINTRESOURCEA(functionOrdinal));
                    //printf("[+]\tFunction Ordinal %u\n", functionOrdinal);
                }
                else
                {
                    // Because lpThunkData->u1.XYZ no longer points to a PIMAGE_IMPORT_BY_NAME we can't use it.
                    // Instead it points to a resolved var in our exports dll, so we can get the function name
                    // that way.

                    //const PIMAGE_IMPORT_BY_NAME lpData =
                    //    (PIMAGE_IMPORT_BY_NAME)((unsigned char*)hMod + (unsigned int)lpThunkData->u1.AddressOfData);

                    bool replaced = false;
                    for (size_t i = 0; i < pGlobals->mGlobals.size(); i++)
                    {
                        if ((void*)lpThunkData->u1.Function == pGlobals->mGlobals[i]->mVar)
                        {
                            DWORD functionAddress = pGlobals->mGlobals[i]->mOgAddr;
                            //(unsigned long*)callBacks.VGetProcAddress(hModule, (const char*)lpData->Name);
                            DWORD dwOld = 0;
                            VirtualProtect(&lpThunkData->u1.Function, 4, PAGE_EXECUTE_READWRITE, &dwOld);
                            SetFuncField(lpThunkData->u1.Function, functionAddress);
                            replaced = true;
                            break;
                        }
                    }

                    if (!replaced)
                    {
                        printf("Var couldn't be found, I guess we die now\n");
                        abort();
                    }

                   
                    //printf("[+]\tFunction %s\n", (LPSTR)lpData->Name);
                }

                lpThunkData++;
            }
        }
        lpImageImportDescriptor++;
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
    static bool OnExportsExport(LPVOID pContext, HMODULE hDll, const char* pName)
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
                #ifdef HOOK_VERBOSE
                printf("Found! %s\n", pName);
                #endif
                hl->mExportNameToOgAddrMap[std::string(pName)] = it->second;
            }
            else
            {
                if (strstr(pName, "Marker") == NULL) // ignore Markers
                {
                    printf("Didn't find %s (its over fr)\n", pName);
                }
            }
        }
        return true;
    }

    static bool GetFuncMeta(FuncMeta& meta, HMODULE hDll, const char* pName)
    {
        u8* pFun = reinterpret_cast<u8*>(::GetProcAddress(hDll, pName));
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

    static bool OnImportsExport(LPVOID pContext, HMODULE hDll, const char* pName)
    {
        FuncMeta meta;
        if (GetFuncMeta(meta, hDll, pName))
        {
            reinterpret_cast<HookLoader*>(pContext)->mFunctionsToHookMap[pName] = meta;
        }
        else
        {
            if (strstr(pName, "Marker") == NULL) // ignore Markers
            {
                printf("WARNING: No meta for %s\n", pName);
            }
        }
        return true;
    }

    void MapExportedVarsToOgAddrs(HMODULE hMod)
    {
        printf("Enumerating exports..\n");
        EnumExports(this, hMod, OnExportsExport);
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
        // Both DLL export functions, only exports dll exports vars and only imports dll imports var
        printf("Load dll\n");
        HMODULE hImports = LoadLibraryA("gta2_dll_imports.dll");

        // Implicitly loaded as imports links to exports
        HMODULE hExports = GetModuleHandle("gta2_dll_exports.dll");

        // Build a map of exported var records and their OG executable address
        printf("Get globals registry %X\n", hExports);
        TGetGlobalsRegistry pFnGlobalsRegistry = (TGetGlobalsRegistry)GetProcAddress(hExports, "GetGlobalsRegistry");
        printf("Collect export entries %X\n", pFnGlobalsRegistry);
        GlobalsRegistry* pGlobalsRegistry = pFnGlobalsRegistry();

        printf("Enumerate entries %X count %d\n", pGlobalsRegistry, pGlobalsRegistry->mGlobals.size());

        for (size_t i = 0; i < pGlobalsRegistry->mGlobals.size(); i++)
        {
            mGlobalEntryToOgAddrMap[pGlobalsRegistry->mGlobals[i]->mVar] = pGlobalsRegistry->mGlobals[i]->mOgAddr;
        }


        // Get the actual exported vars and link it to an OG addr
        MapExportedVarsToOgAddrs(hExports);

        // TODO: rewrite the address of imports addr of the exported var to the OG executable addr
        RewriteImports(hImports, pGlobalsRegistry);


        // mExportNameToOgAddrMap

        /*
        printf("Unload exports dll\n");
        FreeLibrary(hMod);

        printf("Load imports dll\n");
        GTA2LoaderCallBacks cb(mExportNameToOgAddrMap);
        LPVOID lpModule = MemoryLoader::LoadDLL("gta2_dll_imports.dll", cb);
        if (lpModule == NULL)
        {
            printf("Loading gta2_dll_imports.dll failed\n");
        }
        */

        // Replicate what start() does enough to make things work
        printf("crt inits\n");
        crt_inits();

        EnumExports(this, hImports, OnImportsExport);

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
                #ifdef HOOK_VERBOSE
                printf("Look up %s\n", it->first.c_str());
                #endif

                LPVOID addr = ::GetProcAddress(hImports, it->first.c_str());
                const FuncMeta& meta = it->second;
                
                #ifdef HOOK_VERBOSE
                printf("Doing %s\n", it->first.c_str());
                #endif

                if (meta.mStatus == 0 || meta.mStatus == 2)
                {
                    // stubbed or wip - hook reimpl func to og
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
        LPVOID pGameMain = ::GetProcAddress(hImports, "GameMain");
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
    void __declspec(dllexport) __cdecl HookMain()
    {
            AllocConsole();
            freopen("CONOUT$", "w", stdout);
            SetConsoleTitleA("GTA2 debug console");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
        

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