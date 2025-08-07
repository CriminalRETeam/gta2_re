#include "zealous_borg.hpp"
#include "Function.hpp"

// FUNCTION: 105 0x484ED0
zealous_borg::zealous_borg() // 0x484ED0
{
    sub_484EF0();
}

// FUNCTION: 105 0x484EE0
zealous_borg::~zealous_borg() // 0x484EE0
{
    // Empty
}

// FUNCTION: 105 0x484EF0
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

// FUNCTION: 105 0x484F20
void zealous_borg::sub_484F20()
{
    sub_484F30();
}

// FUNCTION: 105 0x484F30
void zealous_borg::sub_484F30()
{
    for (u8 i = 1; i < 10; i++)
    {
        field_28[i] = 1;
    }
}

// FUNCTION: 105 0x484F50
void zealous_borg::report_crime(int crime_type)
{
    switch (crime_type)
    {
        case 2:
            if (field_28[crime_type])
            {
                field_0[crime_type]++;
                field_28[crime_type] = 0;
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
            field_0[crime_type]++;
            break;

        default:
            return;
    }
}

// FUNCTION: 105 0x484FA0
void zealous_borg::sub_484FA0(int a2)
{
    field_34 += a2;
}

// FUNCTION: 105 0x484FB0
void zealous_borg::sub_484FB0(int amount)
{
    field_38 += amount;
}