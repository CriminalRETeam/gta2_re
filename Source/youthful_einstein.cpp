#include "youthful_einstein.hpp"
#include "Globals.hpp"
#include <string>

EXPORT_VAR youthful_einstein gYouthful_einstein_6F8450;
GLOBAL(gYouthful_einstein_6F8450, 0x6F8450);

MATCH_FUNC(0x516560)
void youthful_einstein::ctor_516560() // For some reason, it's a function instead of a proper ctor
{
    field_0_fugitive = NULL;
    field_1C = 0;
    memset(field_4_time, 0, sizeof(field_4_time));
    memset(field_20, 0, sizeof(field_20));
}

STUB_FUNC(0x516590)
void youthful_einstein::sub_516590(angry_lewin_0x85C* a2)
{
}

STUB_FUNC(0x516660)
s16 youthful_einstein::sub_516660()
{
    return 0;
}

STUB_FUNC(0x516740)
void youthful_einstein::sub_516740(angry_lewin_0x85C* a2, angry_lewin_0x85C* a3)
{
}
