#include "HeapTracker.hpp"
#include "BotHeap.hpp"
#include "BotLog.hpp"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "3rdParty/Detours/include/detours.h"

namespace
{
struct Entry
{
    u8* p;    // NULL empty, kTombstone deleted
    u32 size;
};

u8* const kTombstone = reinterpret_cast<u8*>(1);

Entry* gTable = NULL;
u32 gCapacity = 0; // power of two
u32 gUsed = 0;     // live + tombstones
u32 gLive = 0;
u32 gLiveBytes = 0;

bool gInPlay = false;
u32 gPlayAllocs = 0;
u32 gPlayFrees = 0;
u32 gPlayAllocBytes = 0;
const u32 kMaxLoggedPlayAllocs = 64;

u32 HashPtr(const u8* p)
{
    u32 h = reinterpret_cast<u32>(p) >> 3;
    h *= 2654435761u;
    return h ^ (h >> 15);
}

void Rehash(u32 new_capacity)
{
    Entry* old = gTable;
    const u32 old_capacity = gCapacity;
    gTable = static_cast<Entry*>(BotHeap_Alloc(sizeof(Entry) * new_capacity));
    gCapacity = new_capacity;
    gUsed = 0;
    for (u32 i = 0; i < old_capacity; i++)
    {
        if (old[i].p && old[i].p != kTombstone)
        {
            u32 idx = HashPtr(old[i].p) & (gCapacity - 1);
            while (gTable[idx].p)
            {
                idx = (idx + 1) & (gCapacity - 1);
            }
            gTable[idx] = old[i];
            gUsed++;
        }
    }
    BotHeap_Free(old);
}

void Insert(u8* p, u32 size)
{
    if (!p)
    {
        return;
    }
    if (!gTable)
    {
        Rehash(1 << 14);
    }
    if ((gUsed + 1) * 2 > gCapacity)
    {
        Rehash(gCapacity * 2);
    }
    u32 idx = HashPtr(p) & (gCapacity - 1);
    while (gTable[idx].p && gTable[idx].p != kTombstone)
    {
        if (gTable[idx].p == p)
        {
            // Should not happen: the block is live. Keep the newer size.
            gLiveBytes += size - gTable[idx].size;
            gTable[idx].size = size;
            return;
        }
        idx = (idx + 1) & (gCapacity - 1);
    }
    if (!gTable[idx].p)
    {
        gUsed++;
    }
    gTable[idx].p = p;
    gTable[idx].size = size;
    gLive++;
    gLiveBytes += size;
}

// Returns the size of the removed block, or 0 when unknown
u32 Remove(u8* p)
{
    if (!p || !gTable)
    {
        return 0;
    }
    u32 idx = HashPtr(p) & (gCapacity - 1);
    while (gTable[idx].p)
    {
        if (gTable[idx].p == p)
        {
            const u32 size = gTable[idx].size;
            gTable[idx].p = kTombstone;
            gTable[idx].size = 0;
            gLive--;
            gLiveBytes -= size;
            return size;
        }
        idx = (idx + 1) & (gCapacity - 1);
    }
    return 0;
}

void NotePlayAlloc(u32 size, void* ret_addr, const char* what)
{
    gPlayAllocs++;
    gPlayAllocBytes += size;
    if (gPlayAllocs <= kMaxLoggedPlayAllocs)
    {
        BotLog("heap: %s of %u bytes during play, caller %08x", what, size, reinterpret_cast<u32>(ret_addr));
    }
}

void NotePlayFree(u32 size, void* ret_addr)
{
    gPlayFrees++;
    if (gPlayFrees <= kMaxLoggedPlayAllocs)
    {
        BotLog("heap: free of %u bytes during play, caller %08x", size, reinterpret_cast<u32>(ret_addr));
    }
}

void*(__cdecl* Real_malloc)(size_t) = malloc;
void*(__cdecl* Real_calloc)(size_t, size_t) = calloc;
void*(__cdecl* Real_realloc)(void*, size_t) = realloc;
void(__cdecl* Real_free)(void*) = free;

// The hooks keep a frame pointer so that [ebp+4] is the caller's address
#pragma optimize("y", off)

void* __cdecl Hook_malloc(size_t size)
{
    void* ret_addr;
    __asm mov eax, [ebp + 4]
    __asm mov ret_addr, eax
    void* p = Real_malloc(size);
    Insert(static_cast<u8*>(p), size);
    if (gInPlay)
    {
        NotePlayAlloc(size, ret_addr, "malloc");
    }
    return p;
}

void* __cdecl Hook_calloc(size_t count, size_t size)
{
    void* ret_addr;
    __asm mov eax, [ebp + 4]
    __asm mov ret_addr, eax
    void* p = Real_calloc(count, size);
    Insert(static_cast<u8*>(p), count * size);
    if (gInPlay)
    {
        NotePlayAlloc(count * size, ret_addr, "calloc");
    }
    return p;
}

void* __cdecl Hook_realloc(void* old, size_t size)
{
    void* ret_addr;
    __asm mov eax, [ebp + 4]
    __asm mov ret_addr, eax
    void* p = Real_realloc(old, size);
    if (size == 0 || p)
    {
        Remove(static_cast<u8*>(old));
    }
    if (p && size)
    {
        Insert(static_cast<u8*>(p), size);
    }
    if (gInPlay)
    {
        NotePlayAlloc(size, ret_addr, "realloc");
    }
    return p;
}

void __cdecl Hook_free(void* p)
{
    void* ret_addr;
    __asm mov eax, [ebp + 4]
    __asm mov ret_addr, eax
    const u32 size = Remove(static_cast<u8*>(p));
    if (gInPlay && p)
    {
        NotePlayFree(size, ret_addr);
    }
    Real_free(p);
}

#pragma optimize("", on)

} // namespace

void HeapTracker::Init()
{
    if (!gTable)
    {
        Rehash(1 << 14);
    }
}

bool HeapTracker::AttachDetours()
{
    Init();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(reinterpret_cast<PVOID*>(&Real_malloc), reinterpret_cast<PVOID>(Hook_malloc));
    DetourAttach(reinterpret_cast<PVOID*>(&Real_calloc), reinterpret_cast<PVOID>(Hook_calloc));
    DetourAttach(reinterpret_cast<PVOID*>(&Real_realloc), reinterpret_cast<PVOID>(Hook_realloc));
    DetourAttach(reinterpret_cast<PVOID*>(&Real_free), reinterpret_cast<PVOID>(Hook_free));
    const LONG err = DetourTransactionCommit();
    if (err != NO_ERROR)
    {
        BotLog("heap: DetourTransactionCommit failed: %ld", err);
        return false;
    }
    return true;
}

void HeapTracker::BeginPlay()
{
    gInPlay = true;
}

bool HeapTracker::InPlay()
{
    return gInPlay;
}

u32 HeapTracker::LiveCount()
{
    return gLive;
}

u32 HeapTracker::LiveBytes()
{
    return gLiveBytes;
}

u32 HeapTracker::PlayAllocs()
{
    return gPlayAllocs;
}

u32 HeapTracker::PlayFrees()
{
    return gPlayFrees;
}

u32 HeapTracker::PlayAllocBytes()
{
    return gPlayAllocBytes;
}

u32 HeapTracker::BlockSize(const u8* p)
{
    if (!p || !gTable)
    {
        return 0;
    }
    u32 idx = HashPtr(p) & (gCapacity - 1);
    while (gTable[idx].p)
    {
        if (gTable[idx].p == p)
        {
            return gTable[idx].size;
        }
        idx = (idx + 1) & (gCapacity - 1);
    }
    return 0;
}

void HeapTracker::ForEachLive(BlockFn fn, void* ctx)
{
    for (u32 i = 0; i < gCapacity; i++)
    {
        if (gTable[i].p && gTable[i].p != kTombstone)
        {
            fn(ctx, gTable[i].p, gTable[i].size);
        }
    }
}
