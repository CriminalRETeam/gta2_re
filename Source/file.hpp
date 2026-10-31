#pragma once

#include "Function.hpp"
#include <stdio.h>
#include <windows.h>

class File
{
  public:
    STATIC_MEMBER(s32, sbGlobalFileOpen);
    EXPORT static s32 __stdcall GetFileSize(FILE* Stream);

    EXPORT static bool __stdcall IsCdRomDrive(char_type driveLetter);

    EXPORT static void __stdcall CheckReadOnlyFile(const char_type* FileName, s32 expectedSize);
    EXPORT static void* __stdcall ReadFileToBuffer(const char_type* FileName, size_t* pAllocatedBufferSize);
    EXPORT static size_t __stdcall ReadFileToFixedBuffer(const char_type* FileName, void* pBuffer, size_t* pMaxSize);

    EXPORT static size_t __stdcall Read(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream);

    EXPORT static void __stdcall WriteBufferToFile(const char_type* FileName, void* Buffer, size_t* pBufferSize);

    EXPORT static size_t __stdcall Write(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream);

    EXPORT static void __stdcall AppendBufferToFile(const char_type* FileName, void* Buffer, size_t* pBufferSize);

    EXPORT static void __stdcall CreateEmptyFile(const char_type* FileName);

    EXPORT static void __stdcall Global_Open(const char_type* FileName);

    EXPORT static void __stdcall Global_Close();

    EXPORT static void __stdcall Global_Close_UnChecked();

    EXPORT static void __stdcall Global_Seek(u32* pOffset);

    EXPORT static void __stdcall File_Error(s32 Code, s32 arg1, s32 arg2);

    EXPORT static void __stdcall Global_Read(void* pBuffer, const u32& bufferSize);

    EXPORT static bool __stdcall Global_TryRead(void* Buffer, u32* pSize);

    EXPORT static size_t __stdcall Global_ReadRemainder(void* Buffer, u32* pMaxFileSize);

    EXPORT static char_type __stdcall SkipWhitespace(FILE* Stream);
};

// Copy protection: a file of the game CD with its expected size
struct CdCheckFile
{
    char_type mName[128]; // 0x0
    u32 mSize; // 0x80
};
GTA2_ASSERT_SIZEOF_ALWAYS(CdCheckFile, 0x84);

EXTERN_GLOBAL(u16, gCdCheckFileCount);
EXTERN_GLOBAL_ARRAY(CdCheckFile, gCdCheckFiles, 22);
