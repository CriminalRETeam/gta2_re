#include "Tango_28.hpp"
#include "debug.hpp"

MATCH_FUNC(0x4a85f0)
void Tango_54::sub_4A85F0()
{
    Tango_28* p = field_0;
    if (!bSkip_fire_engines_67D53A)
    {
        for (u32 i = 0; i < 2; i++)
        {
            if (p->field_4)
            {
                p->sub_4A81F0();
            }
            p++;
        }
    }
}

STUB_FUNC(0x4a8620)
Tango_28* Tango_54::sub_4A8620(Car_BC* a2, s32 a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4a8800)
Tango_28* Tango_54::sub_4A8800()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4a8820)
char_type Tango_54::sub_4A8820(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4a88d0)
void Tango_54::sub_4A88D0()
{
    field_50 = 0;
}

STUB_FUNC(0x4a7fc0)
char_type Tango_28::sub_4A7FC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4a81a0)
void Tango_28::sub_4A81A0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4a81f0)
void Tango_28::sub_4A81F0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4a85c0)
void Tango_28::init_4A85C0()
{
    sub_450C10();
}

STUB_FUNC(0x4a85e0)
s32 Tango_28::sub_4A85E0()
{
    NOT_IMPLEMENTED;
    return 0;
}