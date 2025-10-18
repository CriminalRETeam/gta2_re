#include "Varrok_7F8.hpp"

DEFINE_GLOBAL(Varrok_7F8*, gVarrok_7F8_703398, 0x703398);

MATCH_FUNC(0x59b060)
u8 Varrok_7F8::sub_59B060(s32 ped_id)
{
    Varrok_8* pIter = &this->field_0[1];
    for (u8 i = 1; i < 255; pIter++, i++)
    {
        if (!pIter->field_0_ped_id && !pIter->field_4)
        {
            field_0[i].field_0_ped_id = ped_id;
            return i;
        }
    }
    return 0;
}

MATCH_FUNC(0x59b0b0)
void Varrok_7F8::sub_59B0B0(u8 a2)
{
    field_0[a2].field_4++;
}

MATCH_FUNC(0x59b0d0)
void Varrok_7F8::sub_59B0D0(u8 idx)
{
    if (field_0[idx].field_4 > 0)
    {
        field_0[idx].field_4--;
    }
}

MATCH_FUNC(0x59b0f0)
Varrok_7F8::Varrok_7F8()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        field_0[i].field_0_ped_id = 0;
        field_0[i].field_4 = 0;
    }
}

MATCH_FUNC(0x59b110)
Varrok_7F8::~Varrok_7F8()
{
}
