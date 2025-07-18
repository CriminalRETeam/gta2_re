#include "Car_10.hpp"
#include "Ped.hpp"

const u8 byte_61A808[] = {7, 24, 11, 28};

MATCH_FUNC(0x439CD0)
void Car_Door_10::sub_439CD0(u32* pRet)
{
    if (field_0_animation_frame != 4)
    {
        if (field_1 == 0)
        {
            if (field_0_animation_frame != 0)
            {
                *pRet &= ~(1 << (field_2 + field_0_animation_frame - 1));
            }
            *pRet |= 1 << (field_0_animation_frame + field_2);
            field_1 = 2;
            field_0_animation_frame++;
        }
        else
        {
            field_1--;
        }
    }

    if (field_0_animation_frame == 4)
    {
        field_4_state = 1;
    }
}

MATCH_FUNC(0x439D40)
void Car_Door_10::sub_439D40(u32* a3)
{
    if (field_0_animation_frame)
    {
        *a3 &= ~(1 << (field_2 + field_0_animation_frame - 1));
        field_0_animation_frame--;
    }

    if (!field_0_animation_frame)
    {
        field_4_state = 6;
    }
    else
    {
        *a3 |= 1 << (field_2 + field_0_animation_frame - 1);
    }
}

STUB_FUNC(0x439DA0)
void Car_Door_10::sub_439DA0(u32* a3a)
{
    NOT_IMPLEMENTED;
    switch (field_4_state)
    {
        case 1:
            if (field_C > 0)
            {
                field_C--;
            }
            else
            {
                sub_439EA0();
            }
            break;
        case 2:
            sub_439CD0(a3a);
            break;
        case 3:
            sub_439D40(a3a);
            break;
        case 4:
            field_4_state = 3;
            break;
        case 5:
            field_4_state = 4;
            break;
        default:
            break;
    }

    Ped* v4 = field_8_pObj;
    if (v4)
    {
        if (v4->field_278 == 9 || (v4->field_21C & 0x8000000) == 0)
        {
            field_8_pObj = 0;
        }
    }
}

MATCH_FUNC(0x439E40)
void Car_Door_10::sub_439E40(u8 a2)
{
    field_4_state = 6;
    field_0_animation_frame = 0;
    field_2 = byte_61A808[a2];
}

MATCH_FUNC(0x439E60)
void Car_Door_10::sub_439E60()
{
    switch (field_4_state)
    {
        case 1:
        case 2:
            field_C = 50;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            field_1 = 4;
            field_4_state = 2;
            break;
        default:
            break;
    }
}

MATCH_FUNC(0x439EA0)
void Car_Door_10::sub_439EA0()
{
    if (field_4_state > 0 && field_4_state <= 2)
    {
        field_4_state = 5;
    }
}

MATCH_FUNC(0x447330)
Car_Door_10::Car_Door_10()
{
    field_8_pObj = 0;
    field_2 = 0;
    field_0_animation_frame = 0;
    field_1 = 3;
    field_4_state = 0;
    field_C = 0;
}

MATCH_FUNC(0x447350)
Car_Door_10::~Car_Door_10()
{
    field_8_pObj = 0;
}