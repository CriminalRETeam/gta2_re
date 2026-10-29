#include "SpriteRenderer.hpp"
#include "Globals.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "sprite.hpp"

// Forward declarations: the functions below are in address order
EXPORT s32 get_rdtsc_5BEE90();
EXTERN_GLOBAL(u32, gCyclesPerMs_705334);

DEFINE_GLOBAL(SpriteRenderer*, gSpriteRenderer_67B580, 0x67B580);
DEFINE_GLOBAL(SpriteTreeNodePool*, gSpriteTreeNodePool_705BBC, 0x705BBC);
DEFINE_GLOBAL(SpriteTreeStack*, gSpriteTreeStack_705BC0, 0x705BC0);
DEFINE_GLOBAL(s32, gDisplayDraw_67B57C, 0x67B57C);
DEFINE_GLOBAL(s32, gDisplayAdd_67B578, 0x67B578);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_67B434, Fix16(1), 0x67B434);
DEFINE_GLOBAL_INIT(Fix16, kFp96_705B80, Fix16(96), 0x705B80);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_705AC4, Fix16(0), 0x705AC4);

MATCH_FUNC(0x4954f0)
void SpriteRenderer::ResetAll_4954F0()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_layers); i++)
    {
        field_0_layers[i]->Reset_5C5E50();
    }
}

MATCH_FUNC(0x495510)
void SpriteRenderer::DisplayAdd_495510(Sprite* pSprite)
{
    const s32 rdtsc = get_rdtsc_5BEE90();
    if (pSprite->field_1C_zpos >= kFpOne_67B434)
    {
        field_0_layers[pSprite->ComputeZLayer_5A1BD0()]->AddSprite_5C5CF0(pSprite);
    }
    gDisplayAdd_67B578 += get_rdtsc_5BEE90() - rdtsc;
}

MATCH_FUNC(0x495560)
void SpriteRenderer::Draw_495560(s32 layer)
{
    const s32 rdtsc = get_rdtsc_5BEE90();
    field_0_layers[layer - 1]->Draw_5C5DF0();
    gDisplayDraw_67B57C += get_rdtsc_5BEE90() - rdtsc;
}

MATCH_FUNC(0x4955a0)
SpriteRenderer::SpriteRenderer()
{
    for (s32 i = 0; i < k_num_layers; i++)
    {
        field_0_layers[i] = new SpriteTree();
        if (!field_0_layers[i])
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
        delete field_0_layers[i];
    }
}

// https://decomp.me/scratch/qe97a
MATCH_FUNC(0x5BEE90)
EXPORT s32 get_rdtsc_5BEE90()
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

// Converts a cycle count from get_rdtsc_5BEE90 to milliseconds for the profiler display
MATCH_FUNC(0x5BEED0)
EXPORT s32 __stdcall ConvertCycleCount_5BEED0(s32 cycles)
{
    return static_cast<u32>(cycles) / gCyclesPerMs_705334;
}

// https://decomp.me/scratch/qyVgM
MATCH_FUNC(0x5c5cf0)
void SpriteTree::AddSprite_5C5CF0(Sprite* pSprite)
{
    SpriteTreeNode* pRoot = field_0_pRoot;
    Fix16 node_z;
    Fix16 z_pos;

    if (pSprite->field_28_draw_layer > 9)
    {
        if (pSprite->field_28_draw_layer == 34)
        {
            z_pos = kFp96_705B80;
        }
        else
        {
            z_pos = pSprite->field_1C_zpos;
        }
    }
    else
    {
        z_pos = kFpZero_705AC4;
    }

    SpriteTreeNode* pParent; // TODO: not initialized before 'for' loop
    for (SpriteTreeNode* pIter = pRoot; pIter;)
    {
        Sprite* pNodeSprite = pIter->field_0_pSprite;
        pParent = pIter;
        s32 node_layer = pIter->field_0_pSprite->field_28_draw_layer;
        if (node_layer > 9)
        {
            node_z = pNodeSprite->field_1C_zpos;
        }
        else
        {
            node_z = kFpZero_705AC4;
        }

        if (z_pos < node_z)
        {
            pIter = pIter->field_4_pLeft;
        }
        else
        {
            if (z_pos == node_z)
            {
                if (pSprite->field_28_draw_layer < node_layer)
                {
                    pIter = pIter->field_4_pLeft;
                }
                else
                {
                    if (pNodeSprite == pSprite)
                    {
                        return;
                    }
                    pIter = pIter->field_8_pRight;
                }
            }
            else
            {
                pIter = pIter->field_8_pRight;
            }
        }
    }

    SpriteTreeNode* pAllocated = gSpriteTreeNodePool_705BBC->Alloc_4C4B40();
    pAllocated->field_0_pSprite = pSprite;
    pAllocated->field_4_pLeft = NULL;
    pAllocated->field_8_pRight = NULL;

    if (!field_0_pRoot)
    {
        field_0_pRoot = pAllocated;
    }
    else
    {
        if (z_pos < node_z || (z_pos == node_z && pSprite->field_28_draw_layer < pParent->field_0_pSprite->field_28_draw_layer))
        {
            pParent->field_4_pLeft = pAllocated;
        }
        else
        {
            pParent->field_8_pRight = pAllocated;
        }
    }
}

MATCH_FUNC(0x5C5DF0)
void SpriteTree::Draw_5C5DF0()
{
    SpriteTreeNode* pIter = this->field_0_pRoot;
    for (;;)
    {
        while (pIter)
        {
            gSpriteTreeStack_705BC0->Push_4C4B80(pIter);
            pIter = pIter->field_4_pLeft;
        }
        if (gSpriteTreeStack_705BC0->IsEnd_4C4BC0())
        {
            break;
        }

        pIter = gSpriteTreeStack_705BC0->Pop_4C4BA0();
        pIter->field_0_pSprite->Draw_59EFF0();
        pIter = pIter->field_8_pRight;
    }
}

DEFINE_GLOBAL_INIT(u32, gCyclesPerMs_705334, 1701493, 0x705334);

MATCH_FUNC(0x5c5e50)
void SpriteTree::Reset_5C5E50()
{
    gSpriteTreeNodePool_705BBC->Reset_4C4B70();
    field_0_pRoot = NULL;
}

MATCH_FUNC(0x5c5e70)
SpriteTree::SpriteTree()
{
    if (!gSpriteTreeNodePool_705BBC)
    {
        gSpriteTreeNodePool_705BBC = new SpriteTreeNodePool();
    }

    if (!gSpriteTreeStack_705BC0)
    {
        gSpriteTreeStack_705BC0 = new SpriteTreeStack();
    }
    Reset_5C5E50();
}

MATCH_FUNC(0x5c5f10)
SpriteTree::~SpriteTree()
{
    // An explicit dtor call through the global plus operator delete on a saved copy gives the original's
    // test in ecx with esi pushed and loaded inside the if; `delete` keeps the pointer in esi throughout
    if (gSpriteTreeNodePool_705BBC != NULL)
    {
        SpriteTreeNodePool* p = gSpriteTreeNodePool_705BBC;
        gSpriteTreeNodePool_705BBC->~SpriteTreeNodePool();
        operator delete(p);
        gSpriteTreeNodePool_705BBC = NULL;
    }

    if (gSpriteTreeStack_705BC0)
    {
        GTA2_DELETE_AND_NULL(gSpriteTreeStack_705BC0);
    }
}

MATCH_FUNC(0x5c5f60)
SpriteTreeNodePool::SpriteTreeNodePool()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_entries); i++)
    {
        field_0_entries[i].field_0_pSprite = NULL;
    }
    Reset_4C4B70();
}

MATCH_FUNC(0x5c5f90)
SpriteTreeNodePool::~SpriteTreeNodePool()
{
}