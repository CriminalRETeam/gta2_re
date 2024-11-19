#include "sharp_bose_0x54.hpp"
#include "Function.hpp"
#include <windows.h>

STUB_FUNC(0x5BEC30)
Garox_C4* ShowFps_5BEC30()
{
    return 0;
}

STUB_FUNC(0x5BEC70)
sharp_bose_0x54::sharp_bose_0x54()
{
    // todo
}

STUB_FUNC(0x5BECC0)
sharp_bose_0x54::~sharp_bose_0x54()
{
    // todo
}

MATCH_FUNC(0x5BECF0)
void sharp_bose_0x54::sub_5BECF0(char_type a2, char_type a3)
{
    if (a3)
    {
        field_0++;

        if (field_0 == 100)
        {
            u32 v5 = timeGetTime() - field_4;
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
