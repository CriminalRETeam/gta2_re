#pragma once

#include "types.hpp"
#include "BotHeap.hpp"

struct MemRange
{
    u8* start;
    u32 size;
};

// The set of memory a Snapshot covers: the writable sections of the exe (every DEFINE_GLOBAL)
// and the live CRT heap blocks (HeapTracker), minus an exclusion list of ranges that must
// survive a restore (video, sound and input device state, frame pacing, the CRT allocator's
// own bookkeeping). Exclusions come from bot_exclude.txt in the out dir (written by
// Scripts/bot from output.map: "rva size name") and from AddExclusion for heap blocks.
class MemoryMap : public BotObject
{
  public:
    MemoryMap();
    ~MemoryMap();

    void AddExclusion(const void* start, u32 size, const char* name);
    u32 LoadExclusionFile(const char* path); // returns the number of ranges read

    // (Re)builds the range list from the process state. Call when the game has booted.
    void Build();

    u32 Count() const
    {
        return mCount;
    }

    const MemRange& At(u32 i) const
    {
        return mRanges[i];
    }

    u32 TotalBytes() const
    {
        return mTotal;
    }

    u32 SectionBytes() const
    {
        return mSectionBytes;
    }

    u32 HeapBytes() const
    {
        return mHeapBytes;
    }

    u32 HeapBlocks() const
    {
        return mHeapBlocks;
    }

    u32 ExclusionCount() const
    {
        return mExclCount;
    }

    // Hash of the current contents of every range
    u32 Hash() const;

  private:
    void Clear();
    void PushRaw(u8* start, u32 size);
    void PushFinal(u8* start, u32 size);
    void AddSections();
    static void AddBlockCb(void* ctx, u8* p, u32 size);

    MemRange* mRaw;
    u32 mRawCount;
    u32 mRawCap;

    MemRange* mRanges;
    u32 mCount;
    u32 mCap;
    u32 mTotal;

    MemRange* mExcl;
    u32 mExclCount;
    u32 mExclCap;

    u32 mSectionBytes;
    u32 mHeapBytes;
    u32 mHeapBlocks;
};

// A copy of every range of a MemoryMap. Capture and Restore are plain memcpy loops, so they
// must run when nothing of the game is live on the stack: the bot's frame hook.
class Snapshot : public BotObject
{
  public:
    Snapshot(MemoryMap* pMap);
    ~Snapshot();

    void Capture();
    void Restore() const;

    // Hash of the stored copy (same function as MemoryMap::Hash on live memory)
    u32 Hash() const;

    // Calls fn for each run of bytes where the stored copy differs from live memory
    typedef void (*DiffFn)(void* ctx, u8* addr, u32 size, const u8* old_bytes, const u8* new_bytes);
    u32 Diff(DiffFn fn, void* ctx, u32 max_runs) const;

    bool SaveToFile(const char* path) const;
    // Loads a file written by SaveToFile into this snapshot. The ranges must match the map
    // (same build, same boot), which the header checks.
    bool LoadFromFile(const char* path);

    const MemoryMap* Map() const
    {
        return mMap;
    }

  private:
    MemoryMap* mMap;
    u8* mData;
    u32 mSize;
};

u32 BotHash_Begin();
u32 BotHash_Update(u32 h, const u8* data, u32 size);
