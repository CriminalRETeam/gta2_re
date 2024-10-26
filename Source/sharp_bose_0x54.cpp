#include "Function.hpp"
#include "sharp_bose_0x54.hpp"
#include <windows.h>

// stub 0x5BEC70
sharp_bose_0x54::sharp_bose_0x54()
{
    // todo
}

// stub 0x5BECC0
sharp_bose_0x54::~sharp_bose_0x54()
{
    // todo
}

MATCH_FUNC(0x5BECF0)
void sharp_bose_0x54::sub_5BECF0(char a2, char a3)
{
    if (a3)
    {
        field_0++;

        if (field_0 == 100)
        {
            DWORD v5 = timeGetTime() - field_4;
            field_8 = 100000 / v5;
            field_18.field_0 = 100 * field_18.field_8 / v5;
            field_24.field_0 = 100 * field_24.field_8 / v5;
            field_30.field_0 = 100 * field_30.field_8 / v5;
            field_3C.field_0 = 100 * field_3C.field_8 / v5;
            field_48.field_0 = 100 * field_48.field_8 / v5;
            field_4 = timeGetTime();
            field_0 = 0;
            field_18.field_8 = 0;
            field_24.field_8 = 0;
            field_30.field_8 = 0;
            field_3C.field_8 = 0;
            field_48.field_8 = 0;
        }
    }

    if (a2)
    {
        field_C++;
        if (field_C == 100)
        {
            this->field_14 = 100000 / (timeGetTime() - this->field_10);
            this->field_10 = timeGetTime();
            this->field_C = 0;
        }
    }
}
