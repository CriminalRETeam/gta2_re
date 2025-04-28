#include "Montana.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "error.hpp"

EXPORT_VAR Montana* gMontana_67B580;
GLOBAL(gMontana_67B580, 0x67B580);

EXPORT_VAR Montana_2EE4* gMontana_2EE4_705BBC;
GLOBAL(gMontana_2EE4_705BBC, 0x705BBC);

EXPORT_VAR Montana_FA4* gMontana_FA4_705BC0;
GLOBAL(gMontana_FA4_705BC0, 0x705BC0);

EXPORT_VAR s32 gDisplayDraw_67B57C;
GLOBAL(gDisplayDraw_67B57C, 0x67B57C);

EXPORT_VAR s32 gDisplayAdd_67B578;
GLOBAL(gDisplayAdd_67B578, 0x67B578);

EXPORT_VAR Fix16 dword_67B434; // =  0x4000; TODO
GLOBAL(dword_67B434, 0x67B434);

MATCH_FUNC(0x5c5f60)
Montana_2EE4::Montana_2EE4()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        field_0[i].field_0 = 0;
    }
    field_2EE0_free_indx = 0;
}

MATCH_FUNC(0x5c5f90)
Montana_2EE4::~Montana_2EE4()
{
}

STUB_FUNC(0x5c5cf0)
void Montana_4::AddSprite_5C5CF0(Sprite* pSprite)
{
}

STUB_FUNC(0x5C5DF0)
void Montana_4::Draw_5C5DF0()
{
    Montana_C* pIter = this->field_0_pFirst;
    for (;;)
    {
        while (pIter)
        {
            gMontana_FA4_705BC0->Push_4C4B80(pIter);
            pIter = pIter->field_4_pNext;
        }
        if (gMontana_FA4_705BC0->IsEnd_4C4BC0())
        {
            break;
        }

        pIter = gMontana_FA4_705BC0->Pop_4C4BA0();
        pIter->field_0->sub_59EFF0();
        pIter = pIter->field_8;
    }
}

MATCH_FUNC(0x5c5e50)
void Montana_4::Reset_5C5E50()
{
    gMontana_2EE4_705BBC->field_2EE0_free_indx = 0;
    field_0_pFirst = 0;
}

STUB_FUNC(0x5c5e70)
Montana_4::Montana_4()
{
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
STUB_FUNC(0x5BEE90)
EXPORT unsigned __int64 get_rdtsc_5BEE90()
{
    return 0;
}

MATCH_FUNC(0x495510)
void Montana::DisplayAdd_495510(Sprite* pSprite)
{
    const s32 rdtsc = get_rdtsc_5BEE90();
    if (pSprite->field_1C_zpos >= dword_67B434)
    {
        field_0_cols[pSprite->sub_5A1BD0()]->AddSprite_5C5CF0(pSprite);
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
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\display.cpp", 121);
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
