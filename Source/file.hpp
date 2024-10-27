#pragma once

#include <stdio.h>
#include <windows.h>
#include "types.hpp"

class File
{
public:
    // or static with explicit __stdcall
    EXPORT static s32 __stdcall GetFileSize_4A6B10(FILE *Stream);

    EXPORT static bool __stdcall IsCdRomDrive_4A6BB0(char_type a1);

    EXPORT static void *__stdcall ReadFileToBuffer_4A6C80(const char_type *FileName, size_t *a2);

    EXPORT static size_t __stdcall Read_4A6D90(void *Buffer, size_t ElementSize, size_t ElementCount, FILE *Stream);

    EXPORT static void __stdcall WriteBufferToFile_4A6E80(const char_type *FileName, void *Buffer, size_t *a3);

    EXPORT static size_t __stdcall Write_4A6F30(void *Buffer, size_t ElementSize, size_t ElementCount, FILE *Stream);

    EXPORT static void __stdcall AppendBufferToFile_4A6F50(const char_type *FileName, void *Buffer, size_t *a3);

    EXPORT static void __stdcall CreateFile_4A7000(const char_type *FileName);

    EXPORT static void __stdcall Global_Open_4A7060(const char_type *FileName);

    EXPORT static void __stdcall Global_Close_4A70C0();

    EXPORT static void __stdcall Global_Close_UnChecked_4A7110();

    EXPORT static void __stdcall Global_Seek_4A7140(u32 *pOffset);

    EXPORT static void __stdcall File_Error_4A7190(s32 Code, s32 a2, s32 a3);

    EXPORT static void __stdcall Global_Read_4A71C0(void *pBuffer, u32 *pBufferSize);

    EXPORT static bool __stdcall Global_Read_4A7210(void *Buffer, u32 *pSize);

    EXPORT static size_t __stdcall GetRemainderSize_4A7250(void *Buffer, u32 *pMaxFileSize);

    EXPORT static char_type __stdcall SkipWhitespace_4A7340(FILE *Stream);


};
