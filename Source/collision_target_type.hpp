#pragma once

// CollisionTarget::mType: what the sprite that is being moved collided with.
// Kept in its own header so that the enum does not land in a header many TUs share.
namespace collision_target_type
{
enum
{
    none_0 = 0,
    horizontal_edge_1 = 1, // wall segment running along x (mHsegXMin .. mHsegXMax at mHsegY)
    vertical_edge_2 = 2,   // wall segment running along y (mVsegYMin .. mVsegYMax at mVsegX)
    sprite_3 = 3,          // another sprite (car, ped or object), see mpHitSprite
    floor_below_4 = 4,     // the block just below the sprite, queued as CollisionSoundQueue::AddFloorCollision
    block_at_z_5 = 5,      // the block at the sprite's own height, queued as AddBlockCollision
};
} // namespace collision_target_type
