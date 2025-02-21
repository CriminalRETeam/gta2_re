#include "Miss2_25C.hpp"
#include "Globals.hpp"

EXPORT_VAR Miss2_25C* gMiss2_25C_6F805C;
GLOBAL(gMiss2_25C_6F805C, 0x6F805C);

MATCH_FUNC(0x502d90)
Miss2_25C::Miss2_25C()
{
    field_258_count = 0;
    for (s32 i = 0; i < 50; i++)
    {
        field_0[i].field_0_uni1 = 0;
        field_0[i].field_8_uni2 = 0;
        field_0[i].field_4_type = 0;
    }
}

STUB_FUNC(0x502dc0)
s32 Miss2_25C::sub_502DC0()
{
    return 0;
}

MATCH_FUNC(0x502f60)
Miss2_C* Miss2_25C::allocate_next_502F60()
{
    Miss2_C* pIter = field_0;
    for (u16 idx = 0; idx < 50; idx++)
    {
        if (!pIter->field_0_uni1)
        {
            return pIter;
        }
        pIter++;
    }
    return NULL;
}

STUB_FUNC(0x502f80)
void Miss2_25C::sub_502F80(s32 a2)
{
}

STUB_FUNC(0x502fb0)
char_type Miss2_25C::sub_502FB0(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x502ff0)
void Miss2_25C::sub_502FF0(s32 a2, char_type a3)
{
}

STUB_FUNC(0x503050)
void Miss2_25C::sub_503050(s32 a2)
{
}
