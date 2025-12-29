#include "Montana.hpp"
#include "Globals.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "sprite.hpp"

DEFINE_GLOBAL(Montana*, gMontana_67B580, 0x67B580);
DEFINE_GLOBAL(Montana_2EE4*, gMontana_2EE4_705BBC, 0x705BBC);
DEFINE_GLOBAL(Montana_FA4*, gMontana_FA4_705BC0, 0x705BC0);
DEFINE_GLOBAL(s32, gDisplayDraw_67B57C, 0x67B57C);
DEFINE_GLOBAL(s32, gDisplayAdd_67B578, 0x67B578);
DEFINE_GLOBAL(Fix16, dword_67B434, 0x67B434); // = 0x4000, TODO, 0xUNKNOWN);
DEFINE_GLOBAL_INIT(Fix16, dword_705B80, Fix16(60), 0x705B80);
DEFINE_GLOBAL_INIT(Fix16, dword_705AC4, Fix16(0), 0x705AC4);

MATCH_FUNC(0x5c5f60)
Montana_2EE4::Montana_2EE4()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        field_0[i].field_0_sprt = 0;
    }
    field_2EE0_free_indx = 0;
}

MATCH_FUNC(0x5c5f90)
Montana_2EE4::~Montana_2EE4()
{
}

// https://decomp.me/scratch/qyVgM reg swap
STUB_FUNC(0x5c5cf0)
void Montana_4::AddSprite_5C5CF0(Sprite* pSprite)
{
    Montana_C* pFirst = field_0_pFirst;
    Fix16 a2_1;
    Fix16 z_pos;

    if (pSprite->field_28_num > 9)
    {
        if (pSprite->field_28_num == 34)
        {
            z_pos = dword_705B80;
        }
        else
        {
            z_pos = pSprite->field_1C_zpos;
        }
    }
    else
    {
        z_pos = dword_705AC4;
    }

    Montana_C* pLastNonNull; // TODO: not initialized before 'for' loop
    for (Montana_C* pIter = pFirst; pIter;)
    {
        Sprite* pSprt = pIter->field_0_sprt;
        pLastNonNull = pIter;
        s32 num = pIter->field_0_sprt->field_28_num;
        if (num > 9)
        {
            a2_1 = pSprt->field_1C_zpos;
        }
        else
        {
            a2_1 = dword_705AC4;
        }

        if (z_pos < a2_1)
        {
            pIter = pIter->mpNext;
        }
        else
        {
            if (z_pos == a2_1)
            {
                if (pSprite->field_28_num < num)
                {
                    pIter = pIter->mpNext;
                }
                else
                {
                    if (pSprt == pSprite)
                    {
                        return;
                    }
                    pIter = pIter->field_8;
                }
            }
            else
            {
                pIter = pIter->field_8;
            }
        }
    }

    Montana_C* pAllocated = gMontana_2EE4_705BBC->sub_4C4B40();
    pAllocated->field_0_sprt = pSprite;
    pAllocated->mpNext = NULL;
    pAllocated->field_8 = NULL;

    if (!field_0_pFirst)
    {
        field_0_pFirst = pAllocated;
    }
    else
    {
        if (z_pos >= a2_1)
        {
            if (z_pos == a2_1)
            {
                if (pSprite->field_28_num >= pLastNonNull->field_0_sprt->field_28_num)
                {
                    pLastNonNull->field_8 = pAllocated;
                }
                else
                {
                    pLastNonNull->mpNext = pAllocated;
                }
            }
            else
            {
                pLastNonNull->field_8 = pAllocated;
            }
        }
        else
        {
            pLastNonNull->mpNext = pAllocated;
        }
    }
}

MATCH_FUNC(0x5C5DF0)
void Montana_4::Draw_5C5DF0()
{
    Montana_C* pIter = this->field_0_pFirst;
    for (;;)
    {
        while (pIter)
        {
            gMontana_FA4_705BC0->Push_4C4B80(pIter);
            pIter = pIter->mpNext;
        }
        if (gMontana_FA4_705BC0->IsEnd_4C4BC0())
        {
            break;
        }

        pIter = gMontana_FA4_705BC0->Pop_4C4BA0();
        pIter->field_0_sprt->Draw_59EFF0();
        pIter = pIter->field_8;
    }
}

MATCH_FUNC(0x5c5e50)
void Montana_4::Reset_5C5E50()
{
    gMontana_2EE4_705BBC->field_2EE0_free_indx = 0;
    field_0_pFirst = 0;
}

// TODO: Doesn't match due to SEH stuff
STUB_FUNC(0x5c5e70)
Montana_4::Montana_4()
{
    NOT_IMPLEMENTED;
    if (!gMontana_2EE4_705BBC)
    {
        gMontana_2EE4_705BBC = new Montana_2EE4();
    }

    if (!gMontana_FA4_705BC0)
    {
        gMontana_FA4_705BC0 = new Montana_FA4();
    }
    Reset_5C5E50();
}

STUB_FUNC(0x5c5f10)
Montana_4::~Montana_4()
{
    NOT_IMPLEMENTED;
    GTA2_DELETE_AND_NULL(gMontana_2EE4_705BBC);
    GTA2_DELETE_AND_NULL(gMontana_FA4_705BC0);
}

MATCH_FUNC(0x4954f0)
void Montana::ResetAll_4954F0()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_cols); i++)
    {
        field_0_cols[i]->Reset_5C5E50();
    }
}

// TODO: move
// https://decomp.me/scratch/qe97a
STUB_FUNC(0x5BEE90)
EXPORT unsigned __int64 get_rdtsc_5BEE90()
{
    // NOTE: Actually is inline assembly, surprisingly
    unsigned __int64 t;
    __asm 
    { 
        pushad 
        rdtsc 
        mov DWORD PTR t, eax 
        mov DWORD PTR t+4, edx 
        popad
    }
    return static_cast<int>(t);
}

MATCH_FUNC(0x495510)
void Montana::DisplayAdd_495510(Sprite* pSprite)
{
    const s32 rdtsc = get_rdtsc_5BEE90();
    if (pSprite->field_1C_zpos >= dword_67B434)
    {
        field_0_cols[pSprite->ComputeZLayer_5A1BD0()]->AddSprite_5C5CF0(pSprite);
    }
    gDisplayAdd_67B578 += get_rdtsc_5BEE90() - rdtsc;
}

MATCH_FUNC(0x495560)
void Montana::Draw_495560(s32 col_idx)
{
    const s32 rdtsc = get_rdtsc_5BEE90();
    field_0_cols[col_idx - 1]->Draw_5C5DF0();
    gDisplayDraw_67B57C += get_rdtsc_5BEE90() - rdtsc;
}

MATCH_FUNC(0x4955a0)
Montana::Montana()
{
    for (s32 i = 0; i < 7; i++)
    {
        field_0_cols[i] = new Montana_4();
        if (!field_0_cols[i])
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\display.cpp", 121);
        }
    }
}

MATCH_FUNC(0x495630)
Montana::~Montana()
{
    for (s32 i = 0; i < 7; i++)
    {
        delete field_0_cols[i];
    }
}