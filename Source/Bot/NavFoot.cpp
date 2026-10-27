#include "NavFoot.hpp"
#include "BotLog.hpp"
#include <string.h>
#include <stdio.h>

#include "map_0x370.hpp"
#include "PathFinder.hpp"
#include "BotCompat.hpp"

// PathFinder.cpp, no header declaration
EXTERN_GLOBAL(u8, gPathFinder_SlopeZDelta_6FDEEC);

namespace
{
const u32 kZLevels = 8;
const u32 kStates = 256 * 256 * kZLevels;
const u32 kNone = 0xFFFFFFFFu;

inline u32 Pack(u32 x, u32 y, u32 z)
{
    return ((y * 256 + x) * kZLevels) + z;
}

struct HeapItem
{
    u32 f;
    u32 idx;
};

// Binary min heap in the bot heap
struct Heap
{
    HeapItem* items;
    u32 count;
    u32 cap;

    Heap()
        : items(NULL)
        , count(0)
        , cap(0)
    {
    }

    ~Heap()
    {
        BotHeap_Free(items);
    }

    void Push(u32 f, u32 idx)
    {
        if (count == cap)
        {
            cap = cap ? cap * 2 : 4096;
            items = static_cast<HeapItem*>(BotHeap_Realloc(items, sizeof(HeapItem) * cap));
        }
        u32 i = count++;
        while (i > 0)
        {
            const u32 parent = (i - 1) / 2;
            if (items[parent].f <= f)
            {
                break;
            }
            items[i] = items[parent];
            i = parent;
        }
        items[i].f = f;
        items[i].idx = idx;
    }

    HeapItem Pop()
    {
        HeapItem top = items[0];
        const HeapItem last = items[--count];
        u32 i = 0;
        while (true)
        {
            u32 child = i * 2 + 1;
            if (child >= count)
            {
                break;
            }
            if (child + 1 < count && items[child + 1].f < items[child].f)
            {
                child++;
            }
            if (items[child].f >= last.f)
            {
                break;
            }
            items[i] = items[child];
            i = child;
        }
        if (count)
        {
            items[i] = last;
        }
        return top;
    }
};
} // namespace

NavFoot::NavFoot()
    : mPathLen(0)
    , mExpanded(0)
    , mBanCount(0)
    , mStartX(0)
    , mStartY(0)
    , mTargetX(0)
    , mTargetY(0)
{
    mCost = static_cast<u32*>(BotHeap_Alloc(sizeof(u32) * kStates));
    mParent = static_cast<u32*>(BotHeap_Alloc(sizeof(u32) * kStates));
    mState = static_cast<u8*>(BotHeap_Alloc(kStates));
    memset(mBanX, 0, sizeof(mBanX));
    memset(mBanY, 0, sizeof(mBanY));
}

NavFoot::~NavFoot()
{
    BotHeap_Free(mCost);
    BotHeap_Free(mParent);
    BotHeap_Free(mState);
}

void NavFoot::Ban(u8 x, u8 y)
{
    if (mBanCount < kMaxBans && !IsBanned(x, y))
    {
        mBanX[mBanCount] = x;
        mBanY[mBanCount] = y;
        mBanCount++;
    }
}

void NavFoot::ClearBans()
{
    mBanCount = 0;
}

bool NavFoot::IsBanned(u8 x, u8 y) const
{
    for (u32 i = 0; i < mBanCount; i++)
    {
        if (mBanX[i] == x && mBanY[i] == y)
        {
            return true;
        }
    }
    return false;
}

bool NavFoot::FindPath(u8 sx, u8 sy, u8 sz, u8 tx, u8 ty)
{
    mPathLen = 0;
    mExpanded = 0;
    mStartX = sx;
    mStartY = sy;
    mTargetX = tx;
    mTargetY = ty;
    if (sz >= kZLevels)
    {
        return false;
    }

    memset(mState, 0, kStates);

    PathFinder* pf = gPathFinder_6FDEF0;
    // The game's path finder keeps its test position in these fields; a ped may be in the middle of
    // a search that spans frames, so leave them as they were
    const u8 save_x = pf->field_25_xpos;
    const u8 save_y = pf->field_26_ypos;
    const u8 save_z = pf->field_27_zpos;
    const u8 save_delta = gPathFinder_SlopeZDelta_6FDEEC;
    const char_type save_blocked = gMap_0x370_6F6268->field_36E_bBlockedByTerrain;

    Heap heap;
    const u32 start = Pack(sx, sy, sz);
    mCost[start] = 0;
    mParent[start] = kNone;
    mState[start] = 1;
    heap.Push(0, start);

    static const s32 dxs[8] = {0, 1, 0, -1, 1, 1, -1, -1};
    static const s32 dys[8] = {-1, 0, 1, 0, -1, 1, 1, -1};

    u32 goal = kNone;
    while (heap.count && mExpanded < 80000)
    {
        const HeapItem item = heap.Pop();
        const u32 idx = item.idx;
        if (mState[idx] == 2)
        {
            continue;
        }
        mState[idx] = 2;
        mExpanded++;

        const u32 z = idx % kZLevels;
        const u32 xy = idx / kZLevels;
        const s32 x = xy % 256;
        const s32 y = xy / 256;
        if (x == tx && y == ty)
        {
            goal = idx;
            break;
        }

        for (u32 d = 0; d < 8; d++)
        {
            const s32 nx = x + dxs[d];
            const s32 ny = y + dys[d];
            if (nx < 2 || nx > 253 || ny < 2 || ny > 253)
            {
                continue;
            }
            if (IsBanned(static_cast<u8>(nx), static_cast<u8>(ny)) && !(nx == tx && ny == ty))
            {
                continue;
            }
            gPathFinder_SlopeZDelta_6FDEEC = 0;
            if (!pf->TestMoveToAdjacentTile_5540E0(static_cast<u8>(x), static_cast<u8>(y), static_cast<u8>(z), static_cast<u8>(nx), static_cast<u8>(ny)))
            {
                continue;
            }
            const u8 nz = static_cast<u8>(z + gPathFinder_SlopeZDelta_6FDEEC);
            if (nz >= kZLevels)
            {
                continue;
            }
            const u32 nidx = Pack(nx, ny, nz);
            if (mState[nidx] == 2)
            {
                continue;
            }
            u32 step = (dxs[d] != 0 && dys[d] != 0) ? 14 : 10;
            if (nz != z)
            {
                step += 20;
            }
            const u32 cost = mCost[idx] + step;
            if (mState[nidx] == 1 && mCost[nidx] <= cost)
            {
                continue;
            }
            mCost[nidx] = cost;
            mParent[nidx] = idx;
            mState[nidx] = 1;
            const u32 hx = nx > tx ? nx - tx : tx - nx;
            const u32 hy = ny > ty ? ny - ty : ty - ny;
            const u32 diag = hx < hy ? hx : hy;
            const u32 h = (hx + hy - 2 * diag) * 10 + diag * 14;
            heap.Push(cost + h, nidx);
        }
    }

    pf->field_25_xpos = save_x;
    pf->field_26_ypos = save_y;
    pf->field_27_zpos = save_z;
    gPathFinder_SlopeZDelta_6FDEEC = save_delta;
    gMap_0x370_6F6268->field_36E_bBlockedByTerrain = save_blocked;

    if (goal == kNone)
    {
        return false;
    }

    // Walk the parents back, then reverse
    u32 count = 0;
    for (u32 cur = goal; cur != kNone && count < kMaxPath; cur = mParent[cur])
    {
        const u32 xy = cur / kZLevels;
        mPath[count].x = static_cast<u8>(xy % 256);
        mPath[count].y = static_cast<u8>(xy / 256);
        mPath[count].z = static_cast<u8>(cur % kZLevels);
        count++;
    }
    for (u32 i = 0; i < count / 2; i++)
    {
        NavNode t = mPath[i];
        mPath[i] = mPath[count - 1 - i];
        mPath[count - 1 - i] = t;
    }
    mPathLen = count;
    return true;
}

void NavFoot::Dump(const char* path, const char* title) const
{
    // Bounding box of the explored tiles, capped
    s32 min_x = 255, max_x = 0, min_y = 255, max_y = 0;
    for (u32 i = 0; i < kStates; i++)
    {
        if (mState[i] == 2)
        {
            const u32 xy = i / kZLevels;
            const s32 x = xy % 256;
            const s32 y = xy / 256;
            if (x < min_x) min_x = x;
            if (x > max_x) max_x = x;
            if (y < min_y) min_y = y;
            if (y > max_y) max_y = y;
        }
    }
    if (min_x > max_x)
    {
        BotFile_WriteAll(path, "nothing explored\n", 17);
        return;
    }
    if (max_x - min_x > 120)
    {
        max_x = min_x + 120;
    }
    if (max_y - min_y > 90)
    {
        max_y = min_y + 90;
    }

    BotTextFile file;
    if (!file.Open(path))
    {
        return;
    }
    file.Printf("%s\nexplored %u tiles, path %u tiles, %u banned, x %d..%d y %d..%d\n", title, mExpanded, mPathLen, mBanCount, min_x, max_x, min_y, max_y);
    char row[260];
    for (s32 y = min_y; y <= max_y; y++)
    {
        for (s32 x = min_x; x <= max_x; x++)
        {
            char c = ' ';
            for (u32 z = 0; z < kZLevels; z++)
            {
                if (mState[Pack(x, y, z)] == 2)
                {
                    c = '.';
                }
            }
            if (IsBanned(static_cast<u8>(x), static_cast<u8>(y)))
            {
                c = '#';
            }
            row[x - min_x] = c;
        }
        // path overlay
        for (u32 i = 0; i < mPathLen; i++)
        {
            if (mPath[i].y == y && mPath[i].x >= min_x && mPath[i].x <= max_x)
            {
                row[mPath[i].x - min_x] = 'o';
            }
        }
        if (mStartY == y && mStartX >= min_x && mStartX <= max_x)
        {
            row[mStartX - min_x] = 'S';
        }
        if (mTargetY == y && mTargetX >= min_x && mTargetX <= max_x)
        {
            row[mTargetX - min_x] = 'G';
        }
        row[max_x - min_x + 1] = 0;
        file.Printf("%3d %s\n", y, row);
    }
}
