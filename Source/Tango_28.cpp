#include "Tango_28.hpp"
#include "Car_BC.hpp"
#include "Game_0x40.hpp"
#include "Hamburger_500.hpp"
#include "debug.hpp"

// FUNCTION: 105 0x4a85f0
void Tango_54::sub_4A85F0()
{
    Tango_28* p = field_0;
    if (!bSkip_fire_engines_67D53A)
    {
        for (u32 i = 0; i < 2; i++)
        {
            if (p->field_4_bActive)
            {
                p->sub_4A81F0();
            }
            p++;
        }
    }
}

STUB_FUNC(0x4a8620)
Tango_28* Tango_54::sub_4A8620(Car_BC* a2, Fix16 x, Fix16 y, Fix16 z)
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x4a8800
Tango_28* Tango_54::New28_4A8800()
{
    Tango_28* p = field_0;
    for (s16 i = 0; i < 2; i++)
    {
        if (!p->field_4_bActive)
        {
            return p;
        }
        p++;
    }
    return 0;
}

// FUNCTION: 105 0x4a8820
char_type Tango_54::sub_4A8820(Car_BC* pCar)
{
    if (!pCar)
    {
        return 0;
    }
    const s32 f88 = pCar->field_88;
    if (f88 == 6 || f88 == 7 || f88 == 5)
    {
        return 0;
    }

    if (pCar->is_train_model())
    {
        return 0;
    }

    Tango_28* pFoundCar =
        sub_4A8620(pCar, pCar->field_50_car_sprite->GetXPos(), pCar->field_50_car_sprite->GetYPos(), pCar->field_50_car_sprite->GetZPos());

    if (!pFoundCar)
    {
        return 0;
    }
    pFoundCar->field_C_target_car = pCar;
    return 1;
}

// FUNCTION: 105 0x4a88d0
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

// FUNCTION: 105 0x4a81a0
void Tango_28::deinit_4A81A0()
{
    Car_BC* pCar = this->field_1C_car;
    if (pCar)
    {
        Hamburger_40* pRoute = pCar->field_60;
        if (pRoute)
        {
            gHamburger_500_678E30->sub_474CC0(pRoute);
            this->field_1C_car->field_60 = 0;
        }
    }
    Ped* pPed = this->field_20_ped;
    if (pPed)
    {
        pPed->SetObjective(0, 9999);
    }

    Car_BC* pCar2 = this->field_C_target_car;
    this->field_8_state = 5;
    if (pCar2)
    {
        pCar2->field_0_qq.sub_5A7080();
    }
}

STUB_FUNC(0x4a81f0)
void Tango_28::sub_4A81F0()
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x4a85c0
void Tango_28::init_4A85C0()
{
    sub_450C10();
}

// FUNCTION: 105 0x4a85e0
void Tango_28::Reset_4A85E0()
{
    this->field_C_target_car = 0;
    this->field_8_state = 0;
    this->field_4_bActive = 0;
    this->field_1C_car = 0;
}