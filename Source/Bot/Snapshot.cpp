#include "Snapshot.hpp"
#include "HeapTracker.hpp"
#include "BotLog.hpp"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// FNV-1a over 32 bit words, bytes for the tail
u32 BotHash_Begin()
{
    return 2166136261u;
}

u32 BotHash_Update(u32 h, const u8* data, u32 size)
{
    const u32* words = reinterpret_cast<const u32*>(data);
    const u32 word_count = size >> 2;
    for (u32 i = 0; i < word_count; i++)
    {
        h = (h ^ words[i]) * 16777619u;
    }
    for (u32 j = word_count << 2; j < size; j++)
    {
        h = (h ^ data[j]) * 16777619u;
    }
    return h;
}

static int __cdecl CompareRanges(const void* a, const void* b)
{
    const MemRange* ra = static_cast<const MemRange*>(a);
    const MemRange* rb = static_cast<const MemRange*>(b);
    if (ra->start < rb->start)
    {
        return -1;
    }
    if (ra->start > rb->start)
    {
        return 1;
    }
    return 0;
}

static void GrowRanges(MemRange*& arr, u32& cap, u32 needed)
{
    if (needed <= cap)
    {
        return;
    }
    u32 new_cap = cap ? cap : 256;
    while (new_cap < needed)
    {
        new_cap *= 2;
    }
    arr = static_cast<MemRange*>(BotHeap_Realloc(arr, sizeof(MemRange) * new_cap));
    cap = new_cap;
}

MemoryMap::MemoryMap()
    : mRaw(NULL)
    , mRawCount(0)
    , mRawCap(0)
    , mRanges(NULL)
    , mCount(0)
    , mCap(0)
    , mTotal(0)
    , mExcl(NULL)
    , mExclCount(0)
    , mExclCap(0)
    , mSectionBytes(0)
    , mHeapBytes(0)
    , mHeapBlocks(0)
    , mSectionCount(0)
{
}

MemoryMap::~MemoryMap()
{
    BotHeap_Free(mRaw);
    BotHeap_Free(mRanges);
    BotHeap_Free(mExcl);
}

void MemoryMap::Clear()
{
    mRawCount = 0;
    mCount = 0;
    mTotal = 0;
    mSectionBytes = 0;
    mHeapBytes = 0;
    mHeapBlocks = 0;
    mSectionCount = 0;
}

void MemoryMap::AddExclusion(const void* start, u32 size, const char* name)
{
    if (!start || !size)
    {
        return;
    }
    GrowRanges(mExcl, mExclCap, mExclCount + 1);
    mExcl[mExclCount].start = static_cast<u8*>(const_cast<void*>(start));
    mExcl[mExclCount].size = size;
    mExclCount++;
    BotLog("snapshot: exclude %08x+%x %s", reinterpret_cast<u32>(start), size, name ? name : "");
}

u32 MemoryMap::LoadExclusionFile(const char* path)
{
    u32 size = 0;
    u8* text = BotFile_ReadAll(path, &size);
    if (!text)
    {
        BotLog("snapshot: no exclusion file %s", path);
        return 0;
    }
    u8* base = reinterpret_cast<u8*>(GetModuleHandleA(NULL));
    u32 count = 0;
    char* line = reinterpret_cast<char*>(text);
    while (line && *line)
    {
        char* next = strchr(line, '\n');
        if (next)
        {
            *next++ = 0;
        }
        char* end = line + strlen(line);
        while (end > line && (end[-1] == '\r' || end[-1] == ' '))
        {
            *--end = 0;
        }
        if (*line && *line != '#')
        {
            char* p = line;
            const u32 rva = strtoul(p, &p, 16);
            const u32 len = strtoul(p, &p, 0);
            while (*p == ' ' || *p == '\t')
            {
                p++;
            }
            if (len)
            {
                AddExclusion(base + rva, len, p);
                count++;
            }
        }
        line = next;
    }
    BotHeap_Free(text);
    return count;
}

void MemoryMap::PushRaw(u8* start, u32 size)
{
    if (!size)
    {
        return;
    }
    GrowRanges(mRaw, mRawCap, mRawCount + 1);
    mRaw[mRawCount].start = start;
    mRaw[mRawCount].size = size;
    mRawCount++;
}

void MemoryMap::PushFinal(u8* start, u32 size)
{
    if (!size)
    {
        return;
    }
    GrowRanges(mRanges, mCap, mCount + 1);
    mRanges[mCount].start = start;
    mRanges[mCount].size = size;
    mCount++;
    mTotal += size;
}

void MemoryMap::AddSections()
{
    u8* base = reinterpret_cast<u8*>(GetModuleHandleA(NULL));
    const IMAGE_DOS_HEADER* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    const IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);
    for (u32 i = 0; i < nt->FileHeader.NumberOfSections; i++)
    {
        if (sec[i].Characteristics & IMAGE_SCN_MEM_WRITE)
        {
            char name[9];
            memcpy(name, sec[i].Name, 8);
            name[8] = 0;
            BotLog("snapshot: section %s at %08x size %x", name, reinterpret_cast<u32>(base + sec[i].VirtualAddress), sec[i].Misc.VirtualSize);
            PushRaw(base + sec[i].VirtualAddress, sec[i].Misc.VirtualSize);
            mSectionBytes += sec[i].Misc.VirtualSize;
            mSectionCount++;
        }
    }
}

void MemoryMap::AddBlockCb(void* ctx, u8* p, u32 size)
{
    MemoryMap* self = static_cast<MemoryMap*>(ctx);
    self->PushRaw(p, size);
    self->mHeapBytes += size;
    self->mHeapBlocks++;
}

void MemoryMap::Build()
{
    Clear();
    AddSections();
    HeapTracker::ForEachLive(AddBlockCb, this);

    if (mExclCount > 1)
    {
        qsort(mExcl, mExclCount, sizeof(MemRange), CompareRanges);
    }

    // Cut every raw range around the exclusions
    for (u32 i = 0; i < mRawCount; i++)
    {
        u8* cur = mRaw[i].start;
        u8* end = mRaw[i].start + mRaw[i].size;
        for (u32 e = 0; e < mExclCount && cur < end; e++)
        {
            u8* ex_start = mExcl[e].start;
            u8* ex_end = mExcl[e].start + mExcl[e].size;
            if (ex_end <= cur || ex_start >= end)
            {
                continue;
            }
            if (ex_start > cur)
            {
                PushFinal(cur, ex_start - cur);
            }
            cur = ex_end > cur ? ex_end : cur;
        }
        if (cur < end)
        {
            PushFinal(cur, end - cur);
        }
    }

    if (mCount > 1)
    {
        qsort(mRanges, mCount, sizeof(MemRange), CompareRanges);
    }

    BotLog("snapshot: map built: %u ranges, %u bytes (sections %u, heap %u in %u blocks), %u exclusions",
           mCount,
           mTotal,
           mSectionBytes,
           mHeapBytes,
           mHeapBlocks,
           mExclCount);
}

namespace
{
struct BlockRef
{
    u8* start;
    u32 size;
    u32 ref_count;
    u32 refs[4];
};

int __cdecl CompareBlockRefs(const void* a, const void* b)
{
    const BlockRef* ra = static_cast<const BlockRef*>(a);
    const BlockRef* rb = static_cast<const BlockRef*>(b);
    if (ra->start < rb->start)
    {
        return -1;
    }
    if (ra->start > rb->start)
    {
        return 1;
    }
    return 0;
}

void CollectBlockCb(void* ctx, u8* p, u32 size)
{
    BlockRef** cursor = static_cast<BlockRef**>(ctx);
    (*cursor)->start = p;
    (*cursor)->size = size;
    (*cursor)->ref_count = 0;
    (*cursor)++;
}
} // namespace

void MemoryMap::WriteBlockIndex(const char* path) const
{
    const u32 block_count = HeapTracker::LiveCount();
    BlockRef* blocks = static_cast<BlockRef*>(BotHeap_Alloc(sizeof(BlockRef) * (block_count + 1)));
    BlockRef* cursor = blocks;
    HeapTracker::ForEachLive(CollectBlockCb, &cursor);
    const u32 count = cursor - blocks;
    if (count > 1)
    {
        qsort(blocks, count, sizeof(BlockRef), CompareBlockRefs);
    }

    // Every aligned word of the sections that equals a block start is a reference to that block
    u8* base = reinterpret_cast<u8*>(GetModuleHandleA(NULL));
    for (u32 s = 0; s < mSectionCount && s < mRawCount; s++)
    {
        u8** words = reinterpret_cast<u8**>(mRaw[s].start);
        const u32 word_count = mRaw[s].size / sizeof(u8*);
        for (u32 w = 0; w < word_count; w++)
        {
            u8* target = words[w];
            if (!target || !HeapTracker::BlockSize(target))
            {
                continue;
            }
            u32 lo = 0;
            u32 hi = count;
            while (lo < hi)
            {
                const u32 mid = (lo + hi) / 2;
                if (blocks[mid].start < target)
                {
                    lo = mid + 1;
                }
                else
                {
                    hi = mid;
                }
            }
            if (lo < count && blocks[lo].start == target && blocks[lo].ref_count < 4)
            {
                blocks[lo].refs[blocks[lo].ref_count++] = reinterpret_cast<u8*>(&words[w]) - base;
            }
        }
    }

    BotTextFile file;
    if (file.Open(path))
    {
        file.Printf("# block_addr size referencing_global_rva...\n");
        for (u32 b = 0; b < count; b++)
        {
            file.Printf("%08x %u", reinterpret_cast<u32>(blocks[b].start), blocks[b].size);
            for (u32 r = 0; r < blocks[b].ref_count; r++)
            {
                file.Printf(" %08x", blocks[b].refs[r]);
            }
            file.Printf("\n");
        }
    }
    BotHeap_Free(blocks);
}

u32 MemoryMap::Hash() const
{
    u32 h = BotHash_Begin();
    for (u32 i = 0; i < mCount; i++)
    {
        h = BotHash_Update(h, mRanges[i].start, mRanges[i].size);
    }
    return h;
}

Snapshot::Snapshot(MemoryMap* pMap)
    : mMap(pMap)
    , mData(NULL)
    , mSize(0)
{
    mSize = pMap->TotalBytes();
    mData = static_cast<u8*>(BotHeap_Alloc(mSize ? mSize : 1));
}

Snapshot::~Snapshot()
{
    BotHeap_Free(mData);
}

void Snapshot::Capture()
{
    if (mSize != mMap->TotalBytes())
    {
        mSize = mMap->TotalBytes();
        mData = static_cast<u8*>(BotHeap_Realloc(mData, mSize ? mSize : 1));
    }
    u8* dst = mData;
    for (u32 i = 0; i < mMap->Count(); i++)
    {
        const MemRange& r = mMap->At(i);
        memcpy(dst, r.start, r.size);
        dst += r.size;
    }
}

void Snapshot::Restore() const
{
    const u8* src = mData;
    for (u32 i = 0; i < mMap->Count(); i++)
    {
        const MemRange& r = mMap->At(i);
        memcpy(r.start, src, r.size);
        src += r.size;
    }
}

u32 Snapshot::Hash() const
{
    u32 h = BotHash_Begin();
    const u8* src = mData;
    for (u32 i = 0; i < mMap->Count(); i++)
    {
        const MemRange& r = mMap->At(i);
        h = BotHash_Update(h, src, r.size);
        src += r.size;
    }
    return h;
}

u32 Snapshot::Diff(DiffFn fn, void* ctx, u32 max_runs) const
{
    u32 runs = 0;
    const u8* src = mData;
    for (u32 i = 0; i < mMap->Count(); i++)
    {
        const MemRange& r = mMap->At(i);
        u32 j = 0;
        while (j < r.size)
        {
            if (src[j] == r.start[j])
            {
                j++;
                continue;
            }
            // A run ends after 8 equal bytes so that neighbouring fields stay in one run
            u32 k = j + 1;
            u32 equal = 0;
            while (k < r.size && equal < 8)
            {
                equal = (src[k] == r.start[k]) ? equal + 1 : 0;
                k++;
            }
            const u32 run_end = k - equal;
            runs++;
            if (runs <= max_runs)
            {
                fn(ctx, r.start + j, run_end - j, src + j, r.start + j);
            }
            j = k;
        }
        src += r.size;
    }
    return runs;
}

namespace
{
struct SnapFileHeader
{
    char magic[8]; // "GTA2SNAP"
    u32 version;
    u32 range_count;
    u32 total_bytes;
    u32 image_base;
};
} // namespace

bool Snapshot::SaveToFile(const char* path) const
{
    SnapFileHeader hdr;
    memcpy(hdr.magic, "GTA2SNAP", 8);
    hdr.version = 1;
    hdr.range_count = mMap->Count();
    hdr.total_bytes = mSize;
    hdr.image_base = reinterpret_cast<u32>(GetModuleHandleA(NULL));

    HANDLE h = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    DWORD written = 0;
    BOOL ok = WriteFile(h, &hdr, sizeof(hdr), &written, NULL);
    for (u32 i = 0; ok && i < mMap->Count(); i++)
    {
        ok = WriteFile(h, &mMap->At(i), sizeof(MemRange), &written, NULL);
    }
    if (ok)
    {
        ok = WriteFile(h, mData, mSize, &written, NULL);
    }
    CloseHandle(h);
    return ok == TRUE;
}

bool Snapshot::LoadFromFile(const char* path)
{
    u32 size = 0;
    u8* data = BotFile_ReadAll(path, &size);
    if (!data)
    {
        BotLog("snapshot: cannot read %s", path);
        return false;
    }
    bool ok = false;
    const SnapFileHeader* hdr = reinterpret_cast<const SnapFileHeader*>(data);
    if (size >= sizeof(SnapFileHeader) && memcmp(hdr->magic, "GTA2SNAP", 8) == 0 && hdr->version == 1 &&
        hdr->range_count == mMap->Count() && hdr->total_bytes == mMap->TotalBytes() &&
        size == sizeof(SnapFileHeader) + hdr->range_count * sizeof(MemRange) + hdr->total_bytes)
    {
        const MemRange* ranges = reinterpret_cast<const MemRange*>(data + sizeof(SnapFileHeader));
        ok = true;
        for (u32 i = 0; i < hdr->range_count; i++)
        {
            if (ranges[i].start != mMap->At(i).start || ranges[i].size != mMap->At(i).size)
            {
                BotLog("snapshot: range %u differs (%08x+%x vs %08x+%x)",
                       i,
                       reinterpret_cast<u32>(ranges[i].start),
                       ranges[i].size,
                       reinterpret_cast<u32>(mMap->At(i).start),
                       mMap->At(i).size);
                ok = false;
                break;
            }
        }
        if (ok)
        {
            mSize = hdr->total_bytes;
            mData = static_cast<u8*>(BotHeap_Realloc(mData, mSize ? mSize : 1));
            memcpy(mData, data + sizeof(SnapFileHeader) + hdr->range_count * sizeof(MemRange), mSize);
        }
    }
    else
    {
        BotLog("snapshot: %s does not match this build/boot (ranges %u vs %u, bytes %u vs %u)",
               path,
               hdr->range_count,
               mMap->Count(),
               hdr->total_bytes,
               mMap->TotalBytes());
    }
    BotHeap_Free(data);
    return ok;
}
