#pragma once

// Ped::field_22C_ped_class. Kept in its own header, like the other ped enums, so that it does not land in a header
// many TUs share.
namespace ped_class
{
enum
{
    gang_dummy_0 = 0,
    gang_member_1 = 1,
    mugger_or_car_thief_2 = 2,
};
} // namespace ped_class
