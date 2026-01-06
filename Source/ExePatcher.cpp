#include "types.hpp"

#if defined(__clang__) || (_MSC_VER <= 1200)
#define _Lockit MyStubLockit
#endif

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <windows.h>


#if defined(__clang__)
namespace std
{
    MyStubLockit::MyStubLockit()
    {

    }

    MyStubLockit::~MyStubLockit()
    {

    }
} // namespace std
#endif

void __declspec(naked) WinMain_Hooked_Start()
{
}

#ifdef _MSC_VER
#define SPACE_FOR_JMP_ASM() __asm test eax, ebx __asm nop __asm nop __asm nop __asm nop
#else
    #define SPACE_FOR_JMP_ASM() __asm__( \
        "test %ebx, %eax\n\t" \
        "nop\n\t" \
        "nop\n\t" \
        "nop\n\t" \
        "nop\n\t")
#endif

HMODULE WINAPI LoadLibraryA_Proxy(LPCSTR lpLibFileName);
FARPROC WINAPI GetProcAddress_Proxy(HMODULE hModule, LPCSTR lpProcName);
int WINAPI MessageBoxA_Proxy(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
BOOL WINAPI FreeLibrary_Proxy(HMODULE hLibModule);

// As this hook functions instruction are copied over the real WinMain and start() is skipped this
// function must be PiC (position independant code) and not depend on the CRT (c-run-time)
s32 APIENTRY WinMain_Hooked(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, s32 nShowCmd)
{
    char dllName[256] = {'H', 'o', 'o', 'k', 'L', 'o', 'a', 'd', 'e', 'r', '.', 'd', 'l', 'l'};
    char msgBoxCaption[256] = {'G', 'T', 'A', '2', ' ', 'h', 'o', 'o', 'k', 's', ' ',
                               'f', 'a', 't', 'a', 'l', ' ', 'e', 'r', 'r', 'o', 'r'};
    const HMODULE hLib = LoadLibraryA_Proxy(dllName);
    if (!hLib)
    {
        char errMsg[256] = {'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'A',
                            ' ', 'h', 'a', 's', ' ', 'f', 'a', 'i', 'l', 'e', 'd'};
        MessageBoxA_Proxy(NULL, errMsg, msgBoxCaption, MB_OK | MB_ICONERROR);
        return 1;
    }

    char funcName[256] = {'H', 'o', 'o', 'k', 'M', 'a', 'i', 'n'};
    const FARPROC entryPoint = GetProcAddress_Proxy(hLib, funcName);
    if (!entryPoint)
    {
        char errMsg[256] = {'N', 'o', ' ', 'e', 'n', 't', 'r', 'y', ' ', 'p', 'o', 'i', 'n', 't'};
        MessageBoxA_Proxy(NULL, errMsg, msgBoxCaption, MB_OK | MB_ICONERROR);
        return 1;
    }

    const s32 ret = entryPoint();

    if (!FreeLibrary_Proxy(hLib))
    {
        char errMsg[256] = {'F', 'r', 'e', 'e', 'L', 'i', 'b', 'r', 'a', 'r', 'y', ' ', 'f', 'a', 'i', 'l', 'e', 'd'};
        MessageBoxA_Proxy(NULL, errMsg, msgBoxCaption, MB_OK | MB_ICONERROR);
        return 1;
    }
    return ret;
}

HMODULE __declspec(naked) WINAPI LoadLibraryA_Proxy(LPCSTR lpLibFileName)
{
    SPACE_FOR_JMP_ASM();
}

FARPROC __declspec(naked) WINAPI GetProcAddress_Proxy(HMODULE hModule, LPCSTR lpProcName)
{
    SPACE_FOR_JMP_ASM();
}

int __declspec(naked) WINAPI MessageBoxA_Proxy(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    SPACE_FOR_JMP_ASM();
}

BOOL __declspec(naked) WINAPI FreeLibrary_Proxy(HMODULE hLibModule)
{
    SPACE_FOR_JMP_ASM();
}

void __declspec(naked) WinMain_Hooked_End()
{
}

static std::vector<u8> CopyHookEntryPointCode()
{
    const u32 len = reinterpret_cast<const u8*>(WinMain_Hooked_End) - reinterpret_cast<const u8*>(WinMain_Hooked_Start);
    printf("Hook code size is %d\n", len);
    std::vector<u8> buf;
    buf.resize(len);
    memcpy(&buf[0], (void *)WinMain_Hooked, buf.size());
    return buf;
}

static std::vector<u8> ReadFileToVector(const char* fileName)
{
    // open the file:
    std::streampos fileSize;
    std::ifstream file(fileName, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<u8> fileData(fileSize);
    file.read((char*)&fileData[0], fileSize);
    return fileData;
}

static void SaveVectorToFile(const std::vector<u8>& data, const char* fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    file.write((const char*)&data[0], data.size());
}

static const u32 kJmpOpCodeLen = 5;
static const u32 kJmpOpCode = 0xE9;
static const u32 kOgWinMainVA = 0x5E53F0;
static const u32 kOgWinMainFA = 0x1E53F0;

static void WinMainFwdJmpHookFunc(std::vector<u8>& exeData, u32 destinationFunctionVA, u32 sourceFunctionOffset)
{
    const u32 jmpAddr2 = destinationFunctionVA - (kOgWinMainVA + sourceFunctionOffset + kJmpOpCodeLen);
    exeData[kOgWinMainFA + sourceFunctionOffset + 0] = kJmpOpCode;
    memcpy(&exeData[kOgWinMainFA + sourceFunctionOffset + 1], &jmpAddr2, sizeof(u32));
}

static int PatchOgExecutableToLoadHooks()
{
    // Load the executable into a buffer
    printf("Read exe...\n");
    std::vector<u8> exeData = ReadFileToVector("10.5.exe");
    if (exeData.size() == 0)
    {
        printf("Reading exe failed\n");
        return 1;
    }

    const u32 kOgStartFuncVA = 0x5EFE3D;
    const u32 kOgStartFuncFA = 0x1EFE3D;

    printf("Jmp hook start() -> WinMain()...\n");
    const u32 jmpAddr = (kOgWinMainVA - kJmpOpCodeLen) - kOgStartFuncVA;
    exeData[kOgStartFuncFA + 0] = kJmpOpCode;
    memcpy(&exeData[kOgStartFuncFA + 1], &jmpAddr, sizeof(u32));

    printf("Overwrite WinMain code with WinMain_Hooked code\n");
    std::vector<u8> functionInstructions = CopyHookEntryPointCode();
    printf("Copying instructions\n");
    memcpy(&exeData[kOgWinMainFA], &functionInstructions[0], functionInstructions.size());
    
    const u32 LoadLibraryA_offset = (const u8*)LoadLibraryA_Proxy - (const u8*)WinMain_Hooked_Start;
    const u32 GetProcAddress_offset = (const u8*)GetProcAddress_Proxy - (const u8*)WinMain_Hooked_Start;
    const u32 MessageBoxA_offset = (const u8*)MessageBoxA_Proxy - (const u8*)WinMain_Hooked_Start;
    const u32 FreeLibrary_offset = (const u8*)FreeLibrary_Proxy - (const u8*)WinMain_Hooked_Start;

    // Check for conflicting offsets
    if (LoadLibraryA_offset == GetProcAddress_offset 
        || GetProcAddress_offset == MessageBoxA_offset
        || MessageBoxA_offset == FreeLibrary_offset
        || FreeLibrary_offset == LoadLibraryA_offset
        || LoadLibraryA_offset == MessageBoxA_offset
        || GetProcAddress_offset == FreeLibrary_offset)
    {
        printf("Failed to differentiate proxy functions.\n");
        return 2;
    }

    // Patch the proxy funcs to call ones that'll actually work
    const u32 kOgLoadLibVA = 0x5E8F2C;
    WinMainFwdJmpHookFunc(exeData, kOgLoadLibVA, LoadLibraryA_offset);

    const u32 kOgGetProcAddressVA = 0x5E8F62;
    WinMainFwdJmpHookFunc(exeData, kOgGetProcAddressVA, GetProcAddress_offset);

    const u32 kOgMessageBoxAVA = 0x5E8FBC;
    WinMainFwdJmpHookFunc(exeData, kOgMessageBoxAVA, MessageBoxA_offset);

    const u32 kFreeLibraryVA = 0x5E8F26;
    WinMainFwdJmpHookFunc(exeData, kFreeLibraryVA, FreeLibrary_offset);

    printf("Save back new exe\n");
    SaveVectorToFile(exeData, "10.5.new.exe");
    printf("Done\n");
    return 0;
}

int main(int argc, const char* argv[])
{
    return PatchOgExecutableToLoadHooks();
}
