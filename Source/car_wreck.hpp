#pragma once

// car_info::wreck: which wreck model the car turns into, as an offset from car_model_enum::WRECK0 (0..9). Kept in its own header, like
// car_despawn_status.hpp, so that the enum does not land in a header many TUs share.
namespace car_wreck
{
enum
{
    none_99 = 99, // the model has no wreck: it can't be destroyed, so ResetCarModelFlags_592380 treats it as already destroyed
};
} // namespace car_wreck
