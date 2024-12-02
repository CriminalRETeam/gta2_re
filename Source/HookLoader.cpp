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

void LoadHooks()
{
    printf("Load exports dll\n");
    HMODULE hMod = LoadLibraryA("gta2_dll_exports.dll");
    printf("Get globals registry %X\n", hMod);
    TGetGlobalsRegistry pFnGlobalsRegistry = (TGetGlobalsRegistry)GetProcAddress(hMod, "GetGlobalsRegistry");
    printf("Collect export entries %X\n", pFnGlobalsRegistry);
    GlobalsRegistry* pGlobalsRegistry = pFnGlobalsRegistry();

    printf("Enumerate entries %X count %d\n", pGlobalsRegistry, pGlobalsRegistry->mGlobals.size());
    std::map<const void*, u32> funcMap;
    for (size_t i = 0; i < pGlobalsRegistry->mGlobals.size(); i++)
    {
        funcMap[pGlobalsRegistry->mGlobals[i]->mVar] = pGlobalsRegistry->mGlobals[i]->mOgAddr;
    }

    printf("Load imports dll\n");
    HMODULE hMod2 = LoadLibraryA("gta2_dll_exports.dll");

    std::map<std::string, u32> funcMap2;
    {
        printf("Enumerating exports..\n");
        {
            LPVOID dllAlloc = (LPVOID)hMod2;
            // TODO: Pretty much a copy paste of GetFunctionAddress
            PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)((BYTE*)dllAlloc + ((PIMAGE_DOS_HEADER)dllAlloc)->e_lfanew);
            //assert(header->Signature == IMAGE_NT_SIGNATURE);
            //assert(header->OptionalHeader.NumberOfRvaAndSizes > 0);
            PIMAGE_EXPORT_DIRECTORY exports =
                (PIMAGE_EXPORT_DIRECTORY)((BYTE*)dllAlloc +
                                          header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
            //assert(exports->AddressOfNames != 0);
            BYTE** names = (BYTE**)((int)dllAlloc + exports->AddressOfNames);
            for (int i = 0; i < exports->NumberOfNames; i++)
            {
                char* pName = (char*)((BYTE*)dllAlloc + (int)names[i]);
                if (stricmp(pName, "GetGlobalsRegistry") == 0)
                {
                    // Skip - its a function, everything else is a var
                }
                else
                {
                    FARPROC varAddr = GetProcAddress(hMod2, pName);

                    std::map<const void*, u32>::iterator it = funcMap.find(varAddr);
                    if (it != funcMap.end())
                    {
                        printf("Found! %s\n", pName);
                        funcMap2[std::string(pName)] = it->second;
                    }
                    else
                    {
                        printf("Didn't find %s (its over fr)\n", pName);
                    }
                }
            }
        }
    }

    FreeLibrary(hMod2);

    printf("Unload exports dll\n");
    FreeLibrary(hMod);

    printf("Load imports dll\n");
    GTA2LoaderCallBacks cb(funcMap2);
    LPVOID lpModule = MemoryLoader::LoadDLL("gta2_dll_imports.dll", cb);
    if (lpModule == NULL)
    {
    }

    LPVOID pGameMain = MemoryLoader::GetFunctionAddress(lpModule, "GameMain");
    typedef void(__cdecl * TGameMain)();

    printf("GameMain = %X\n", pGameMain);

    TGameMain pTypedGameMain = (TGameMain)pGameMain;
    pTypedGameMain();

    // TODO: Enumerate matched functions and make the exe jmp to those
    // TODO: Enumerate stub functions and make them jump to the exe function

    // TODO: Replicate what start() does
    // TODO: Bin off static init funcs that are impl'd
}

// The OG patched executable will call this function (outside of DllMain/OS loader locks etc)
extern "C"
{
    __declspec(dllexport) __cdecl HookMain()
    {
        LoadHooks();
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
