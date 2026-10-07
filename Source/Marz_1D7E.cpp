#include "Marz_1D7E.hpp"

DEFINE_GLOBAL(Marz_1D7E*, gMarz_1D7E_6FD784, 0x6FD784);

MATCH_FUNC(0x463F90)
Marz_3::Marz_3()
{
    field_0_x = 0;
    field_1_y = 0;
    field_2_z = 0;
}

MATCH_FUNC(0x463FA0)
Marz_3::~Marz_3()
{
}

MATCH_FUNC(0x4bdf70)
Marz_1D7E::~Marz_1D7E()
{
}

MATCH_FUNC(0x4bdf90)
Marz_96::~Marz_96()
{
}

MATCH_FUNC(0x543ec0)
void Marz_96::sub_543EC0()
{
}

MATCH_FUNC(0x543ed0)
Marz_1D7E::Marz_1D7E()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_1D4C_used); i++)
    {
        field_1D4C_used[i] = 0;
    }
}

MATCH_FUNC(0x543f10)
Marz_96* Marz_1D7E::AllocPatrolList_543F10(u8* pRet)
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_1D4C_used); i++)
    {
        if (!field_1D4C_used[i])
        {
            field_0_lists[i].sub_543EC0();
            *pRet = i;
            field_1D4C_used[i] = 1;
            return &field_0_lists[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x544bf0)
Marz_96::Marz_96()
{
}