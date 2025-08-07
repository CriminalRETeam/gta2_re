#include "Hamburger_500.hpp"
#include "Globals.hpp"
#include "Ped.hpp"

DEFINE_GLOBAL(s32, dword_678D0C, 0x678D0C);

// FUNCTION: 105 0x4747b0
void Hamburger_40::sub_4747B0()
{
    field_0 = 0;
    field_10 = 0;
    field_8 = 0;
    field_14_target_x = dword_678D0C;
    field_18_target_y = dword_678D0C;
    field_1C_target_z = dword_678D0C;
    field_20 = 0;
    field_21 = 0;
    field_22 = 1;
    field_23 = 1;
    field_24 = 0;
    field_25 = 0;
    field_30 = 0;
    field_26 = 0;
    field_2A = 0;
    field_2C = 0;
    field_2E = 0;
    field_C = 0;
    field_34 = 0;
    field_4 = 0;
    field_38 = 0;
    field_3C = 0;
}

// FUNCTION: 105 0x474ca0
Hamburger_40::Hamburger_40()
{
    sub_4747B0();
}

// FUNCTION: 105 0x474cb0
Hamburger_40::~Hamburger_40()
{
}

// FUNCTION: 105 0x474810
Hamburger_40* Hamburger_500::sub_474810()
{
    for (u8 i = 0; i < 20; i++)
    {
        if (!field_0[i].field_0)
        {
            field_0[i].field_0 = 1;
            return &field_0[i];
        }
    }
    return 0;
}

// FUNCTION: 105 0x474850
char_type Hamburger_500::sub_474850(Ped* pPed1, Ped* pPed2)
{
    if (pPed1->field_240_occupation < 24 || pPed1->field_240_occupation > 27)
    {
        if (pPed2->field_17C_pZone == pPed1->field_17C_pZone)
        {
            return 1;
        }
    }
    else
    {
        if (pPed2->field_240_occupation >= 24 && pPed2->field_240_occupation <= 27)
        {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: 105 0x4748a0
Ped* Hamburger_500::sub_4748A0(s32 a2, Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1 && field_0[i].field_30 == pPed && sub_474850(pPed, field_0[i].field_4) && a2 == field_0[i].field_C)
        {
            return field_0[i].field_4;
        }
    }
    return 0;
}

// FUNCTION: 105 0x474920
char_type Hamburger_500::sub_474920(Ped* a2, Ped* a3)
{
    u8 total = 0;
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1 && field_0[i].field_30 == a2)
        {
            if (sub_474850(a3, field_0[i].field_4))
            {
                total++;
            }
        }
    }
    return total;
}

// FUNCTION: 105 0x474970
char_type Hamburger_500::sub_474970(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1 && field_0[i].field_30 == pPed)
        {
            return 1;
        }
    }
    return 0;
}

STUB_FUNC(0x4749b0)
char_type Hamburger_500::sub_4749B0(Ped* a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x474a20
char_type Hamburger_500::sub_474A20(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (sub_474850(pPed, field_0[i].field_4) && field_0[i].field_C == 8)
            {
                return 1;
            }
        }
    }
    return 0;
}

STUB_FUNC(0x474a80)
char_type Hamburger_500::sub_474A80(Ped* pPed)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x474af0
char_type Hamburger_500::sub_474AF0(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (sub_474850(pPed, field_0[i].field_4) && field_0[i].field_C == 9)
            {
                return 1;
            }
        }
    }
    return 0;
}

STUB_FUNC(0x474b50)
char_type Hamburger_500::sub_474B50(Ped* pPed)
{
    NOT_IMPLEMENTED;
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (sub_474850(pPed, field_0[i].field_4))
            {
                if (field_0[i].field_C == 7)
                {
                    return 1;
                }

                if (field_0[i].field_C == 2 || field_0[i].field_C == 0)
                {
                    return 1;
                }

                // TODO: Some sort switch case annoyance?
                /*
                f_c = field_0[i].field_C - 7;
                if (!f_c)
                {
                    return 1;
                }

                f_c_m2 = f_c - 2;
                if (!f_c_m2 || f_c_m2 == 2)
                {
                    return 1;
                }
                */
            }
        }
    }
    return 0;
}

// FUNCTION: 105 0x474bc0
char_type Hamburger_500::sub_474BC0(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (sub_474850(pPed, field_0[i].field_4))
            {
                if (field_0[i].field_C >= 6 && (field_0[i].field_C <= 9 || field_0[i].field_C == 13))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: 105 0x474c30
char_type Hamburger_500::sub_474C30(Ped* pPed)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (field_0[i].field_0 == 1)
        {
            if (sub_474850(pPed, field_0[i].field_4))
            {
                if (field_0[i].field_C < 4)
                {
                    continue;
                }

                if (field_0[i].field_C <= 5)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: 105 0x474cc0
void Hamburger_500::sub_474CC0(Hamburger_40* toFind)
{
    for (u8 i = 0; i < 20; i++)
    {
        if (&field_0[i] == toFind)
        {
            field_0[i].field_0 = 0;
            field_0[i].sub_4747B0();
            return;
        }
    }
}

// FUNCTION: 105 0x474d10
Hamburger_500::Hamburger_500()
{
}

// FUNCTION: 105 0x474d30
Hamburger_500::~Hamburger_500()
{
}