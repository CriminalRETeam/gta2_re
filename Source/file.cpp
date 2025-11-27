#include "file.hpp"
#include "Function.hpp"
#include "error.hpp"
#include "memory.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "enums.hpp"

DEFINE_GLOBAL(s32, gbGlobalFileOpen_67D160, 0x67D160);
DEFINE_GLOBAL(FILE*, ghFile_67CFEC, 0x67CFEC);

MATCH_FUNC(0x4A6B10)
s32 __stdcall File::GetFileSize_4A6B10(FILE* Stream)
{
    s32 oldPos = crt::ftell(Stream);
    if (oldPos == -1)
    {
        FatalError_4A38C0(Gta2Error::FtellError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 56);
    }

    if (crt::fseek(Stream, 0, SEEK_END))
    {
        FatalError_4A38C0(Gta2Error::FseekError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 58);
    }

    s32 endPos = crt::ftell(Stream);
    if (endPos == -1)
    {
        FatalError_4A38C0(Gta2Error::FtellError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 60);
    }

    if (crt::fseek(Stream, oldPos, SEEK_SET))
    {
        FatalError_4A38C0(Gta2Error::FseekError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 62);
    }

    return endPos;
}

MATCH_FUNC(0x4A6BB0)
bool __stdcall File::IsCdRomDrive_4A6BB0(char_type driveLetter)
{
    sprintf(gTmpBuffer_67C598, "%c:", driveLetter);
    // Silly return structure but needed to match (and somehow produces less code)
    if (GetDriveTypeA(gTmpBuffer_67C598) == DRIVE_CDROM)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x4A6C80)
void* __stdcall File::ReadFileToBuffer_4A6C80(const char_type* FileName, size_t* pAllocatedBufferSize)
{
    Error_SetName_4A0770(FileName);
    FILE* hFileRead1 = crt::fopen(FileName, "rb");
    if (!hFileRead1)
    {
        FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown, "C:\\Splitting\\Gta2\\Source\\File.cpp", 141);
    }

    *pAllocatedBufferSize = GetFileSize_4A6B10(hFileRead1);
    if (crt::fclose(hFileRead1))
    {
        FatalError_4A38C0(Gta2Error::FileCloseError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 145);
    }

    void* pBuffer = Memory::malloc_4FE4D0(*pAllocatedBufferSize);

    FILE* hFileRead2 = crt::fopen(FileName, "rb");
    if (!hFileRead2)
    {
        crt::free(pBuffer);
        FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown, "C:\\Splitting\\Gta2\\Source\\File.cpp", 151);
    }

    if (Read_4A6D90(pBuffer, *pAllocatedBufferSize, 1u, hFileRead2) != 1)
    {
        crt::free(pBuffer);
        crt::fclose(hFileRead2);
        FatalError_4A38C0(Gta2Error::FileReadFailure, "C:\\Splitting\\Gta2\\Source\\File.cpp", 158);
    }

    if (crt::fclose(hFileRead2))
    {
        crt::free(pBuffer);
        FatalError_4A38C0(Gta2Error::FileCloseError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 164);
    }

    return pBuffer;
}

MATCH_FUNC(0x4A6D90)
size_t __stdcall File::Read_4A6D90(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream)
{
    size_t ret = crt::fread(Buffer, ElementSize, ElementCount, Stream);
    return ret;
}

MATCH_FUNC(0x4A6E80)
void __stdcall File::WriteBufferToFile_4A6E80(const char_type* FileName, void* Buffer, size_t* pBufferSize)
{
    Error_SetName_4A0770(FileName);
    if (!*pBufferSize)
    {
        FatalError_4A38C0(Gta2Error::WritingZeroBytesToFile, "C:\\Splitting\\Gta2\\Source\\File.cpp", 228);
    }

    FILE* hFile = crt::fopen(FileName, "wb");
    if (!hFile)
    {
        FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown, "C:\\Splitting\\Gta2\\Source\\File.cpp", 231);
    }

    if (Write_4A6F30(Buffer, *pBufferSize, 1u, hFile) != 1)
    {
        crt::fclose(hFile);
        FatalError_4A38C0(Gta2Error::FileWriteFailure, "C:\\Splitting\\Gta2\\Source\\File.cpp", 237);
    }

    if (crt::fclose(hFile))
    {
        FatalError_4A38C0(Gta2Error::FileCloseError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 241);
    }
}

MATCH_FUNC(0x4A6F30)
size_t __stdcall File::Write_4A6F30(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream)
{
    return crt::fwrite(Buffer, ElementSize, ElementCount, Stream);
}

MATCH_FUNC(0x4A6F50)
void __stdcall File::AppendBufferToFile_4A6F50(const char_type* FileName, void* pBuffer, size_t* pBufferSize)
{
    Error_SetName_4A0770(FileName);
    if (!*pBufferSize)
    {
        FatalError_4A38C0(Gta2Error::WritingZeroBytesToFile, "C:\\Splitting\\Gta2\\Source\\File.cpp", 261);
    }

    FILE* hFile = crt::fopen(FileName, "ab"); // TODO: check
    if (!hFile)
    {
        FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown, "C:\\Splitting\\Gta2\\Source\\File.cpp", 264);
    }

    if (Write_4A6F30(pBuffer, *pBufferSize, 1u, hFile) != 1)
    {
        crt::fclose(hFile);
        FatalError_4A38C0(Gta2Error::FileWriteFailure, "C:\\Splitting\\Gta2\\Source\\File.cpp", 270);
    }

    if (crt::fclose(hFile))
    {
        FatalError_4A38C0(Gta2Error::FileCloseError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 274);
    }
}

MATCH_FUNC(0x4A7000)
void __stdcall File::CreateFile_4A7000(const char_type* FileName)
{
    Error_SetName_4A0770(FileName);

    FILE* hFile = crt::fopen(FileName, "wb");
    if (!hFile)
    {
        FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown, "C:\\Splitting\\Gta2\\Source\\File.cpp", 296);
    }

    if (crt::fclose(hFile))
    {
        FatalError_4A38C0(Gta2Error::FileCloseError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 300);
    }
}

MATCH_FUNC(0x4A7060)
void __stdcall File::Global_Open_4A7060(const char_type* FileName)
{
    if (gbGlobalFileOpen_67D160)
    {
        Global_Close_4A70C0();
    }

    Error_SetName_4A0770(FileName);

    ghFile_67CFEC = crt::fopen(FileName, "rb");
    if (!ghFile_67CFEC)
    {
        FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown, "C:\\Splitting\\Gta2\\Source\\File.cpp", 323);
    }

    gbGlobalFileOpen_67D160 = 1;
}

MATCH_FUNC(0x4A70C0)
void __stdcall File::Global_Close_4A70C0()
{
    if (gbGlobalFileOpen_67D160)
    {
        s32 v0 = crt::fclose(ghFile_67CFEC);
        gbGlobalFileOpen_67D160 = 0;
        if (v0)
        {
            FatalError_4A38C0(Gta2Error::FileCloseError, "C:\\Splitting\\Gta2\\Source\\File.cpp", 345);
        }
    }
}

MATCH_FUNC(0x4A7110)
void __stdcall File::Global_Close_UnChecked_4A7110()
{
    if (gbGlobalFileOpen_67D160)
    {
        crt::fclose(ghFile_67CFEC);
        gbGlobalFileOpen_67D160 = 0;
    }
}

MATCH_FUNC(0x4A7140)
void __stdcall File::Global_Seek_4A7140(u32* pOffset)
{
    if (!gbGlobalFileOpen_67D160)
    {
        FatalError_4A38C0(Gta2Error::FileReadingNotOpenedFile, "C:\\Splitting\\Gta2\\Source\\File.cpp", 416);
    }

    if (crt::fseek(ghFile_67CFEC, *pOffset, 1))
    {
        File_Error_4A7190(14, 0, 0);
    }
}

MATCH_FUNC(0x4A7190)
void __stdcall File::File_Error_4A7190(s32 Code, s32 a2, s32 a3)
{
    Global_Close_UnChecked_4A7110();
    FatalError_4A38C0(Code, "C:\\Splitting\\Gta2\\Source\\File.cpp", 398, a2, a3);
}

MATCH_FUNC(0x4A71C0)
void __stdcall File::Global_Read_4A71C0(void* pBuffer, const u32& pBufferSize)
{
    if (!gbGlobalFileOpen_67D160)
    {
        FatalError_4A38C0(Gta2Error::FileReadingNotOpenedFile, "C:\\Splitting\\Gta2\\Source\\File.cpp", 438);
    }

    if (Read_4A6D90(pBuffer, pBufferSize, 1u, ghFile_67CFEC) != 1)
    {
        File_Error_4A7190(15, 0, 0);
    }
}

MATCH_FUNC(0x4A7210)
bool __stdcall File::Global_Read_4A7210(void* Buffer, u32* pSize)
{
    if (!gbGlobalFileOpen_67D160)
    {
        FatalError_4A38C0(Gta2Error::FileReadingNotOpenedFile, "C:\\Splitting\\Gta2\\Source\\File.cpp", 460);
    }
    return (Read_4A6D90(Buffer, *pSize, 1u, ghFile_67CFEC) == 1) ? true : false;
}

MATCH_FUNC(0x4A7250)
size_t __stdcall File::GetRemainderSize_4A7250(void* Buffer, u32* pMaxFileSize)
{
    if (!gbGlobalFileOpen_67D160)
    {
        FatalError_4A38C0(Gta2Error::FileReadingNotOpenedFile, "C:\\Splitting\\Gta2\\Source\\File.cpp", 487);
    }

    s32 curPos = crt::ftell(ghFile_67CFEC);
    if (curPos == -1)
    {
        File_Error_4A7190(13, 0, 0);
    }

    if (crt::fseek(ghFile_67CFEC, 0, SEEK_END))
    {
        File_Error_4A7190(14, 0, 0);
    }

    s32 endPos = crt::ftell(ghFile_67CFEC);
    if (endPos == -1)
    {
        File_Error_4A7190(13, 0, 0);
    }

    if (crt::fseek(ghFile_67CFEC, curPos, 0))
    {
        File_Error_4A7190(14, 0, 0);
    }

    size_t remainderSize = endPos - curPos;
    if (remainderSize > *pMaxFileSize)
    {
        File_Error_4A7190(1022, remainderSize - *pMaxFileSize, 0);
    }

    if (Read_4A6D90(Buffer, remainderSize, 1u, ghFile_67CFEC) != 1)
    {
        File_Error_4A7190(15, 0, 0);
    }

    return remainderSize;
}

MATCH_FUNC(0x4A7340)
char_type __stdcall File::SkipWhitespace_4A7340(FILE* Stream)
{
    char_type next_char = 0;

    while (1)
    {
        next_char = crt::fgetc(Stream);
        // note: feof = Stream->_flag & 0x10
        if (feof(Stream) || next_char == '\n')
        {
            next_char = 0;
            break;
        }

        if (next_char == ' ' || next_char == '\r' || next_char == '\t')
        {
            next_char = 0;
        }
        else
        {
            break;
        }
    }
    return next_char;
}