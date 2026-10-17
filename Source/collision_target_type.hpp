#pragma once

// CollisionTarget_28::field_0_type: what the sprite that is being moved collided with.
// Kept in its own header so that the enum does not land in a header many TUs share.
namespace collision_target_type
{
enum
{
    none_0 = 0,
    horizontal_edge_1 = 1, // wall segment running along x (field_4_hseg_x_min .. field_8_hseg_x_max at field_18_hseg_y)
    vertical_edge_2 = 2,   // wall segment running along y (field_C_vseg_y_min .. field_10_vseg_y_max at field_14_vseg_x)
    sprite_3 = 3,          // another sprite (car, ped or object), see field_20_pHitSprite
    floor_below_4 = 4,     // the block just below the sprite (SetType4), queued as CollisionSoundQueue_C88::AddFloorCollision_40BC40
    block_at_z_5 = 5,      // the block at the sprite's own height (SetType5), queued as AddBlockCollision_40BD10
};
} // namespace collision_target_type
