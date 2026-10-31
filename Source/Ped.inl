#pragma once

inline s32 Ped::get_car_model() const
{
    u32 m;
    if (mCar)
    {
        m = mCar->mCarInfoIdx;
    }
    else
    {
        m = car_model_enum::none;
    }
    return m;
}
