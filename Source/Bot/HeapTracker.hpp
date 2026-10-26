#pragma once

#include "types.hpp"

// Detours on the CRT allocator (malloc, calloc, realloc, free) that keep a table of the live
// heap blocks. The Snapshot copies these blocks: the pools and everything else the game new'ed
// at init live there. The table also logs allocations that happen during play, which the
// snapshot design assumes do not happen (the pools are allocated once and reused).
//
// The table lives in the bot heap. Blocks allocated before the detours were attached (CRT
// start up, static constructors) are not in it; nothing of the game's state is allocated then.
namespace HeapTracker
{
void Init();
bool AttachDetours(); // own Detours transaction

// From now on every allocation and free counts as "during play" and the first ones are logged
void BeginPlay();
bool InPlay();

u32 LiveCount();
u32 LiveBytes();
u32 PlayAllocs();
u32 PlayFrees();
u32 PlayAllocBytes();

typedef void (*BlockFn)(void* ctx, u8* p, u32 size);
void ForEachLive(BlockFn fn, void* ctx);
} // namespace HeapTracker
