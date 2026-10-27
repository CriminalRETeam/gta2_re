#include "NavCar.hpp"
#include "BotLog.hpp"
#include <string.h>
#include <stdio.h>

#include "map_0x370.hpp"
#include "BotCompat.hpp"

namespace
{
const u32 kTiles = 256 * 256;
const u32 kNone = 0xFFFFFFFFu;

struct HeapItem
{
    u32 f;
    u32 idx;
};

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

NavCar::NavCar()
    : mPathTileCount(0)
    , mCount(0)
    , mExpanded(0)
    , mBuilt(false)
    , mSx(0)
    , mSy(0)
    , mTx(0)
    , mTy(0)
{
    mGrid = static_cast<u8*>(BotHeap_Alloc(kTiles));
    mArrows = static_cast<u8*>(BotHeap_Alloc(kTiles));
    mCost = static_cast<u32*>(BotHeap_Alloc(sizeof(u32) * kTiles));
    mParent = static_cast<u32*>(BotHeap_Alloc(sizeof(u32) * kTiles));
    mState = static_cast<u8*>(BotHeap_Alloc(kTiles));
    mPathTiles = static_cast<u16*>(BotHeap_Alloc(sizeof(u16) * kTiles));
    memset(mX, 0, sizeof(mX));
    memset(mY, 0, sizeof(mY));
}

NavCar::~NavCar()
{
    BotHeap_Free(mGrid);
    BotHeap_Free(mArrows);
    BotHeap_Free(mCost);
    BotHeap_Free(mParent);
    BotHeap_Free(mState);
    BotHeap_Free(mPathTiles);
}

void NavCar::Build()
{
    memset(mGrid, 0, kTiles);
    memset(mArrows, 0, kTiles);
    for (int y = 1; y < 255; y++)
    {
        for (int x = 1; x < 255; x++)
        {
            s32 z = 0;
            gmp_block_info* b = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x, y, &z);
            if (!b || z >= 3)
            {
                continue;
            }
            const int type = b->field_B_slope_type & 3;
            u8 cell = 1;
            if (type == 1)
            {
                cell = 3;
            }
            else if (type == 2)
            {
                cell = 2;
            }
            else if (type == 3)
            {
                cell = 1;
            }
            else
            {
                continue; // air
            }
            mGrid[y * 256 + x] = cell;
            mArrows[y * 256 + x] = b->field_A_arrows & 0x0F;
        }
    }
    mBuilt = true;
}

bool NavCar::Open(int x, int y) const
{
    return x >= 1 && x <= 254 && y >= 1 && y <= 254 && mGrid[y * 256 + x] != 0;
}

bool NavCar::Safe(int x, int y, int clearance) const
{
    if (!Open(x, y))
    {
        return false;
    }
    if (clearance > 0)
    {
        return Open(x - 1, y) && Open(x + 1, y) && Open(x, y - 1) && Open(x, y + 1);
    }
    return true;
}

bool NavCar::LineSafe(int x0, int y0, int x1, int y1) const
{
    // Walk the segment in half tile steps; every tile touched needs clearance
    const float dx = static_cast<float>(x1 - x0);
    const float dy = static_cast<float>(y1 - y0);
    const float len = (dx < 0 ? -dx : dx) > (dy < 0 ? -dy : dy) ? (dx < 0 ? -dx : dx) : (dy < 0 ? -dy : dy);
    const int steps = static_cast<int>(len * 2.0f) + 1;
    for (int i = 0; i <= steps; i++)
    {
        const float t = static_cast<float>(i) / steps;
        const int x = static_cast<int>(x0 + 0.5f + dx * t);
        const int y = static_cast<int>(y0 + 0.5f + dy * t);
        if (!Safe(x, y, 1))
        {
            return false;
        }
    }
    return true;
}

bool NavCar::Search(int sx, int sy, int tx, int ty, int clearance)
{
    memset(mState, 0, kTiles);
    mExpanded = 0;
    Heap heap;
    const u32 start = sy * 256 + sx;
    mCost[start] = 0;
    mParent[start] = kNone;
    mState[start] = 1;
    heap.Push(0, start);

    static const int dxs[8] = {0, 1, 0, -1, 1, 1, -1, -1};
    static const int dys[8] = {-1, 0, 1, 0, -1, 1, 1, -1};
    u32 goal = kNone;
    while (heap.count && mExpanded < 70000)
    {
        const HeapItem item = heap.Pop();
        const u32 idx = item.idx;
        if (mState[idx] == 2)
        {
            continue;
        }
        mState[idx] = 2;
        mExpanded++;
        const int x = idx % 256;
        const int y = idx / 256;
        if (x == tx && y == ty)
        {
            goal = idx;
            break;
        }
        for (int d = 0; d < 8; d++)
        {
            const int nx = x + dxs[d];
            const int ny = y + dys[d];
            if (!Open(nx, ny))
            {
                continue;
            }
            const bool near_end = (nx - sx) * (nx - sx) + (ny - sy) * (ny - sy) <= 9 || (nx - tx) * (nx - tx) + (ny - ty) * (ny - ty) <= 9;
            if (!Safe(nx, ny, near_end ? 0 : clearance))
            {
                continue;
            }
            if (dxs[d] != 0 && dys[d] != 0 && (!Open(x + dxs[d], y) || !Open(x, y + dys[d])))
            {
                continue; // no cutting corners
            }
            const u32 nidx = ny * 256 + nx;
            if (mState[nidx] == 2)
            {
                continue;
            }
            // terrain and lane cost
            u32 step = (dxs[d] != 0 && dys[d] != 0) ? 14 : 10;
            const u8 cell = mGrid[nidx];
            if (cell == 3)
            {
                const u8 arrows = mArrows[nidx];
                if (arrows)
                {
                    const bool with = ((arrows & 1) && dxs[d] < 0) || ((arrows & 2) && dxs[d] > 0) || ((arrows & 4) && dys[d] < 0) || ((arrows & 8) && dys[d] > 0);
                    const bool against = ((arrows & 1) && dxs[d] > 0) || ((arrows & 2) && dxs[d] < 0) || ((arrows & 4) && dys[d] > 0) || ((arrows & 8) && dys[d] < 0);
                    step = with ? step * 8 / 10 : (against ? step * 18 / 10 : step);
                }
            }
            else if (cell == 2)
            {
                step = step * 13 / 10;
            }
            else
            {
                step = step * 15 / 10;
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
            heap.Push(cost + ((hx + hy - 2 * diag) * 10 + diag * 14) * 8 / 10, nidx);
        }
    }
    if (goal == kNone)
    {
        return false;
    }
    u32 count = 0;
    for (u32 cur = goal; cur != kNone && count < kTiles; cur = mParent[cur])
    {
        mPathTiles[count++] = static_cast<u16>(cur);
    }
    for (u32 i = 0; i < count / 2; i++)
    {
        const u16 t = mPathTiles[i];
        mPathTiles[i] = mPathTiles[count - 1 - i];
        mPathTiles[count - 1 - i] = t;
    }
    mPathTileCount = count;
    return true;
}

bool NavCar::FindPath(float sx, float sy, float tx, float ty)
{
    mCount = 0;
    mPathTileCount = 0;
    if (!mBuilt)
    {
        Build();
    }
    mSx = static_cast<int>(sx);
    mSy = static_cast<int>(sy);
    mTx = static_cast<int>(tx);
    mTy = static_cast<int>(ty);
    if (!Open(mSx, mSy) || !Open(mTx, mTy))
    {
        return false;
    }
    if (!Search(mSx, mSy, mTx, mTy, 1) && !Search(mSx, mSy, mTx, mTy, 0))
    {
        return false;
    }

    // String pulling: from each kept tile, jump to the furthest tile with a safe straight line
    u32 cur = 0;
    mX[mCount] = (mPathTiles[0] % 256) + 0.5f;
    mY[mCount] = (mPathTiles[0] / 256) + 0.5f;
    mCount++;
    while (cur < mPathTileCount - 1 && mCount < kMaxWp - 1)
    {
        u32 best = cur + 1;
        const int cx = mPathTiles[cur] % 256;
        const int cy = mPathTiles[cur] / 256;
        for (u32 j = mPathTileCount - 1; j > cur + 1; j--)
        {
            // limit leg length so that the car corrects its heading now and then
            const int jx = mPathTiles[j] % 256;
            const int jy = mPathTiles[j] / 256;
            if ((jx - cx) * (jx - cx) + (jy - cy) * (jy - cy) > 30 * 30)
            {
                continue;
            }
            if (LineSafe(cx, cy, jx, jy))
            {
                best = j;
                break;
            }
        }
        cur = best;
        mX[mCount] = (mPathTiles[cur] % 256) + 0.5f;
        mY[mCount] = (mPathTiles[cur] / 256) + 0.5f;
        mCount++;
    }
    return true;
}

void NavCar::Dump(const char* path, const char* title) const
{
    BotTextFile file;
    if (!file.Open(path))
    {
        return;
    }
    file.Printf("%s\n%u waypoints from %u path tiles, %u explored\n", title, mCount, mPathTileCount, mExpanded);
    for (u32 i = 0; i < mCount; i++)
    {
        file.Printf("%3u %.1f %.1f\n", i, mX[i], mY[i]);
    }
}
