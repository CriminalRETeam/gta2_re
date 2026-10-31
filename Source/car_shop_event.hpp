#pragma once

// Car::field_B4_shop_event: what a car shop just did to the car, read once by sound_obj::PlayCarShopEventSound_415480.
namespace car_shop_event
{
enum
{
    none_0 = 0,
    respray_1 = 1,
    change_plates_2 = 2,
    car_bomb_4 = 4,
    car_mines_5 = 5,
    oil_stain_6 = 6,
    car_smg_7 = 7,
    cant_afford_8 = 8,
};
} // namespace car_shop_event
