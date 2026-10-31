#include "SpriteRenderer.hpp"
#include "Globals.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "sprite.hpp"

// Forward declarations: the functions below are in address order
EXPORT s32 get_rdtsc();
EXTERN_GLOBAL(u32, gCyclesPerMs);

DEFINE_GLOBAL(SpriteRenderer*, gSpriteRenderer, 0x67B580);
DEFINE_GLOBAL(SpriteTreeNodePool*, gSpriteTreeNodePool, 0x705BBC);
DEFINE_GLOBAL(SpriteTreeStack*, gSpriteTreeStack, 0x705BC0);
DEFINE_GLOBAL(s32, gDisplayDraw, 0x67B57C);
DEFINE_GLOBAL(s32, gDisplayAdd, 0x67B578);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_67B434, Fix16(1), 0x67B434);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFp96, Fix16(96), 0x705B80);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpZero, Fix16(0), 0x705AC4);

MATCH_FUNC(0x4954f0)
void SpriteRenderer::ResetAll()
{
    for (s32 i = 0; i < GTA2_COUNTOF(mLayers); i++)
    {
        mLayers[i]->Reset();
    }
}

MATCH_FUNC(0x495510)
void SpriteRenderer::DisplayAdd(Sprite* pSprite)
{
    const s32 rdtsc = get_rdtsc();
    if (pSprite->mZpos >= kFpOne_67B434)
    {
        mLayers[pSprite->ComputeZLayer()]->AddSprite(pSprite);
    }
    gDisplayAdd += get_rdtsc() - rdtsc;
}

MATCH_FUNC(0x495560)
void SpriteRenderer::Draw(s32 layer)
{
    const s32 rdtsc = get_rdtsc();
    mLayers[layer - 1]->Draw();
    gDisplayDraw += get_rdtsc() - rdtsc;
}

MATCH_FUNC(0x4955a0)
SpriteRenderer::SpriteRenderer()
{
    for (s32 i = 0; i < k_num_layers; i++)
    {
        mLayers[i] = new SpriteTree();
        if (!mLayers[i])
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\display.cpp", 121);
        }
    }
}

MATCH_FUNC(0x495630)
SpriteRenderer::~SpriteRenderer()
{
    for (s32 i = 0; i < k_num_layers; i++)
    {
        delete mLayers[i];
    }
}

// https://decomp.me/scratch/qe97a
MATCH_FUNC(0x5BEE90)
EXPORT s32 get_rdtsc()
{
    // NOTE: Actually is inline assembly, surprisingly
    unsigned __int64 t;
    __asm
    {
        // The original has the 16-bit pushaw/popaw. Prefix pushad/popad with an operand-size
        // override so the compiler still sees them and saves ebx/esi/edi.
        _emit 0x66
        pushad
        rdtsc
        mov DWORD PTR t, eax
        mov DWORD PTR t+4, edx
        _emit 0x66
        popad
    }
    return static_cast<s32>(t);
}

// Converts a cycle count from get_rdtsc to milliseconds for the profiler display
MATCH_FUNC(0x5BEED0)
EXPORT s32 __stdcall ConvertCycleCount(s32 cycles)
{
    return static_cast<u32>(cycles) / gCyclesPerMs;
}

// https://decomp.me/scratch/qyVgM
MATCH_FUNC(0x5c5cf0)
void SpriteTree::AddSprite(Sprite* pSprite)
{
    SpriteTreeNode* pRoot = mpRoot;
    Fix16 node_z;
    Fix16 z_pos;

    if (pSprite->mDrawLayer > 9)
    {
        if (pSprite->mDrawLayer == 34)
        {
            z_pos = kFp96;
        }
        else
        {
            z_pos = pSprite->mZpos;
        }
    }
    else
    {
        z_pos = kFpZero;
    }

    SpriteTreeNode* pParent; // set by the loop, read below only when the tree is not empty
    for (SpriteTreeNode* pIter = pRoot; pIter;)
    {
        Sprite* pNodeSprite = pIter->mpSprite;
        pParent = pIter;
        s32 node_layer = pIter->mpSprite->mDrawLayer;
        if (node_layer > 9)
        {
            node_z = pNodeSprite->mZpos;
        }
        else
        {
            node_z = kFpZero;
        }

        if (z_pos < node_z)
        {
            pIter = pIter->mpLeft;
        }
        else
        {
            if (z_pos == node_z)
            {
                if (pSprite->mDrawLayer < node_layer)
                {
                    pIter = pIter->mpLeft;
                }
                else
                {
                    if (pNodeSprite == pSprite)
                    {
                        return;
                    }
                    pIter = pIter->mpRight;
                }
            }
            else
            {
                pIter = pIter->mpRight;
            }
        }
    }

    SpriteTreeNode* pAllocated = gSpriteTreeNodePool->Alloc_4C4B40();
    pAllocated->mpSprite = pSprite;
    pAllocated->mpLeft = NULL;
    pAllocated->mpRight = NULL;

    if (!mpRoot)
    {
        mpRoot = pAllocated;
    }
    else
    {
        if (z_pos < node_z || (z_pos == node_z && pSprite->mDrawLayer < pParent->mpSprite->mDrawLayer))
        {
            pParent->mpLeft = pAllocated;
        }
        else
        {
            pParent->mpRight = pAllocated;
        }
    }
}

MATCH_FUNC(0x5C5DF0)
void SpriteTree::Draw()
{
    SpriteTreeNode* pIter = this->mpRoot;
    for (;;)
    {
        while (pIter)
        {
            gSpriteTreeStack->Push_4C4B80(pIter);
            pIter = pIter->mpLeft;
        }
        if (gSpriteTreeStack->IsEnd_4C4BC0())
        {
            break;
        }

        pIter = gSpriteTreeStack->Pop_4C4BA0();
        pIter->mpSprite->Draw();
        pIter = pIter->mpRight;
    }
}

DEFINE_GLOBAL_INIT(u32, gCyclesPerMs, 1701493, 0x705334);

MATCH_FUNC(0x5c5e50)
void SpriteTree::Reset()
{
    gSpriteTreeNodePool->Reset_4C4B70();
    mpRoot = NULL;
}

MATCH_FUNC(0x5c5e70)
SpriteTree::SpriteTree()
{
    if (!gSpriteTreeNodePool)
    {
        gSpriteTreeNodePool = new SpriteTreeNodePool();
    }

    if (!gSpriteTreeStack)
    {
        gSpriteTreeStack = new SpriteTreeStack();
    }
    Reset();
}

MATCH_FUNC(0x5c5f10)
SpriteTree::~SpriteTree()
{
    // An explicit dtor call through the global plus operator delete on a saved copy gives the original's
    // test in ecx with esi pushed and loaded inside the if; `delete` keeps the pointer in esi throughout
    if (gSpriteTreeNodePool != NULL)
    {
        SpriteTreeNodePool* p = gSpriteTreeNodePool;
        gSpriteTreeNodePool->~SpriteTreeNodePool();
        operator delete(p);
        gSpriteTreeNodePool = NULL;
    }

    if (gSpriteTreeStack)
    {
        GTA2_DELETE_AND_NULL(gSpriteTreeStack);
    }
}

MATCH_FUNC(0x5c5f60)
SpriteTreeNodePool::SpriteTreeNodePool()
{
    for (s32 i = 0; i < GTA2_COUNTOF(mEntries); i++)
    {
        mEntries[i].mpSprite = NULL;
    }
    Reset_4C4B70();
}

MATCH_FUNC(0x5c5f90)
SpriteTreeNodePool::~SpriteTreeNodePool()
{
}