#include "BotHeap.hpp"
#include <windows.h>
#include "BotCompat.hpp"

static HANDLE gBotHeap = NULL;

void BotHeap_Init()
{
    if (!gBotHeap)
    {
        gBotHeap = HeapCreate(0, 4 * 1024 * 1024, 0);
    }
}

void* BotHeap_Alloc(size_t size)
{
    if (!gBotHeap)
    {
        BotHeap_Init();
    }
    void* p = HeapAlloc(gBotHeap, HEAP_ZERO_MEMORY, size ? size : 1);
    if (!p)
    {
        OutputDebugStringA("bot: out of memory\n");
        ExitProcess(3);
    }
    return p;
}

void* BotHeap_Realloc(void* p, size_t size)
{
    if (!p)
    {
        return BotHeap_Alloc(size);
    }
    void* q = HeapReAlloc(gBotHeap, HEAP_ZERO_MEMORY, p, size ? size : 1);
    if (!q)
    {
        OutputDebugStringA("bot: out of memory\n");
        ExitProcess(3);
    }
    return q;
}

void BotHeap_Free(void* p)
{
    if (p && gBotHeap)
    {
        HeapFree(gBotHeap, 0, p);
    }
}
