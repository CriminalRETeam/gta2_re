#pragma once

inline s32 Ped::get_car_model()
{
    u32 m;
    if (field_16C_car)
    {
        m = field_16C_car->field_84_car_info_idx;
    }
    else
    {
        m = car_model_enum::none;
    }
    return m;
}
