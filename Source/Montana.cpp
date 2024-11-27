#include "Montana.hpp"

Montana* gMontana_67B580;
Montana_2EE4* gMontana_2EE4_705BBC;
Montana_FA4* gMontana_FA4_705BC0;

STUB_FUNC(0x5c5f60)
Montana_2EE4::Montana_2EE4()
{
}

STUB_FUNC(0x5c5f90)
s32 Montana_2EE4::dtor_5C5F90()
{
    return 0;
}

STUB_FUNC(0x5c5cf0)
void Montana_4::sub_5C5CF0(s32 a2)
{
}

STUB_FUNC(0x5c5df0)
void Montana_4::Draw_5C5DF0()
{
}

STUB_FUNC(0x5c5e50)
void Montana_4::Reset_5C5E50()
{
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
void Montana_4::dtor_5C5F10()
{
}

MATCH_FUNC(0x4954f0)
void Montana::ResetAll_4954F0()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_cols); i++)
    {
        field_0_cols[i]->Reset_5C5E50();
    }
}

STUB_FUNC(0x495510)
s32 Montana::sub_495510(Car_3C* a2)
{
    return 0;
}

STUB_FUNC(0x495560)
void Montana::Draw_495560(s32 col_idx)
{
}

STUB_FUNC(0x4955a0)
Montana::Montana()
{
}

STUB_FUNC(0x495630)
Montana::~Montana()
{
}
