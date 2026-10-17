#pragma once

// CollisionEvent_28::field_0_type: the pair of things that collided, which picks the impact sound
// (see sound_obj::Type6_413A10 / Type6_412C90). Pairs are symmetrical: a car hitting a ped and a ped hitting a car are
// both car_ped_4. A diagonal wall object counts as a wall.
// Kept in its own header so that the enum does not land in a header many TUs share.
namespace collision_event_type
{
enum
{
    object_floor_1 = 1,  // AddFloorCollision / AddBlockCollision with an object
    car_car_2 = 2,
    car_object_3 = 3,
    car_ped_4 = 4,
    car_wall_5 = 5,
    ped_ped_6 = 6,
    ped_object_7 = 7,
    ped_wall_8 = 8,
    object_object_9 = 9,
    object_wall_10 = 10,
    ped_floor_11 = 11,
    car_floor_12 = 12,
};
} // namespace collision_event_type
