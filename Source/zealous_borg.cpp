#include "zealous_borg.hpp"
#include "Function.hpp"

MATCH_FUNC(0x484ED0)
zealous_borg::zealous_borg() // 0x484ED0
{
    sub_484EF0();
}

MATCH_FUNC(0x484EE0)
zealous_borg::~zealous_borg() // 0x484EE0
{
    // Empty
}

MATCH_FUNC(0x484EF0)
void zealous_borg::sub_484EF0()
{
    u8* pByteIter = field_28;
    u32* pIntIter = field_0;
    for (s32 i = GTA2_COUNTOF(field_28) - 1; i >= 0; i--)
    {
        *pIntIter = 0;
        *pByteIter = 1;
        ++pIntIter;
        ++pByteIter;
    }
    field_34 = 0;
    field_38 = 0;
}

MATCH_FUNC(0x484F20)
void zealous_borg::sub_484F20()
{
    sub_484F30();
}

STUB_FUNC(0x484F30)
void zealous_borg::sub_484F30()
{
    *(int*)&field_28[1] = 0x1010101;
    *(int*)&field_28[5] = 0x1010101;
    field_28[9] = 1;
}

MATCH_FUNC(0x484F50)
void zealous_borg::sub_484F50(int a2)
{
    switch (a2)
    {
        case 2:
            if (field_28[a2])
            {
                field_0[a2]++;
                field_28[a2] = 0;
            }
            break;
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            field_0[a2]++;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x484FA0)
void zealous_borg::sub_484FA0(int a2)
{
    field_34 += a2;
}

MATCH_FUNC(0x484FB0)
void zealous_borg::sub_484FB0(int amount)
{
    field_38 += amount;
}
