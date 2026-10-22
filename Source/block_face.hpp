#pragma once

// Masks for the gmp_block_info face words (left, right, top, bottom, lid), laid out as BlockSideWord in
// MapRenderer.hpp. The lid has no wall bits: its bits 10-11 are the lighting level.
namespace block_face
{
enum
{
    tile_idx_mask = 0x3FF,
    wall = 0x400,
    bullet_wall = 0x800,
    flat = 0x1000,
    flip = 0x2000,
    flip_and_rotation_mask = 0xE000,
};
} // namespace block_face
