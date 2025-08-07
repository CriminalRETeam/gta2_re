#include "Marz_1D7E.hpp"

// FUNCTION: 105 0x463F90
Marz_3::Marz_3()
{
    field_0 = 0;
    field_1 = 0;
    field_2 = 0;
}

// FUNCTION: 105 0x463FA0
Marz_3::~Marz_3()
{
}

// FUNCTION: 105 0x4bdf90
Marz_96::~Marz_96()
{
}

// FUNCTION: 105 0x543ec0
void Marz_96::sub_543EC0()
{
}

// FUNCTION: 105 0x544bf0
Marz_96::Marz_96()
{
}

// FUNCTION: 105 0x4bdf70
Marz_1D7E::~Marz_1D7E()
{
}

// FUNCTION: 105 0x543ed0
Marz_1D7E::Marz_1D7E()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_1D4C); i++)
    {
        field_1D4C[i] = 0;
    }
}

// FUNCTION: 105 0x543f10
Marz_96* Marz_1D7E::sub_543F10(u8* pRet)
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_1D4C); i++)
    {
        if (!field_1D4C[i])
        {
            field_0[i].sub_543EC0();
            *pRet = i;
            field_1D4C[i] = 1;
            return &field_0[i];
        }
    }
    return 0;
}