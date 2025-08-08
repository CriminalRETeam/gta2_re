#pragma once

#include "error.hpp"
#include "types.hpp"

// STRING: 105 0x620c04
#define CHUNK_H_STRING "C:\\Splitting\\Gta2\\Source\\chunk.h"

struct chunk_base
{
    char_type field_0_type[4];

    // inline 0x460EE0
    void verify_type(const char_type* pExpected)
    {
        if (strncmp(pExpected, field_0_type, sizeof(field_0_type)) != 0)
        {
            FatalError_4A38C0(93, CHUNK_H_STRING, 37);
        }
    }
};

struct file_header : public chunk_base
{
    u16 field_4_version;

    // inline 460EC0
    void verify_version(u32 expectedVersion)
    {
        if (field_4_version != expectedVersion)
        {
            FatalError_4A38C0(94, CHUNK_H_STRING, 33);
        }
    }
};

struct chunk_header : public chunk_base
{
    s32 field_4_size;
};