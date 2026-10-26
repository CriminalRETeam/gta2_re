#pragma once

#include <stddef.h>
#include "types.hpp"

// All bot memory lives in a private Win32 heap so that a Snapshot restore (the exe's writable
// sections and the CRT heap blocks) never touches it: the bot's memory survives, the world rewinds.
// Bot classes derive from BotObject to allocate from it. The bot never calls the CRT allocator.
void BotHeap_Init();
void* BotHeap_Alloc(size_t size); // zero filled, never returns NULL
void* BotHeap_Realloc(void* p, size_t size);
void BotHeap_Free(void* p);

class BotObject
{
  public:
    void* operator new(size_t n)
    {
        return BotHeap_Alloc(n);
    }

    void operator delete(void* p)
    {
        BotHeap_Free(p);
    }
};
