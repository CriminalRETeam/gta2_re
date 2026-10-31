#pragma once

#include "error.hpp"
#include "types.hpp"
#include "enums.hpp"

struct chunk_base
{
    char_type mType[4]; // 0x0

    // inline 0x460EE0
    void verify_type(const char_type* pExpected)
    {
        if (strncmp(pExpected, mType, sizeof(mType)) != 0)
        {
            FatalError(Gta2Error::InvalidFileType, "C:\\Splitting\\Gta2\\Source\\chunk.h", 37);
        }
    }
};

struct file_header : public chunk_base
{
    u16 mVersion; // 0x4

    // inline 460EC0
    void verify_version(u32 expectedVersion)
    {
        if (mVersion != expectedVersion)
        {
            FatalError(Gta2Error::InvalidFileVersion, "C:\\Splitting\\Gta2\\Source\\chunk.h", 33);
        }
    }
};

struct chunk_header : public chunk_base
{
    s32 mSize; // 0x4
};