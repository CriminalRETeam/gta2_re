#pragma once

// Kept out of enums.hpp on purpose: adding this enum there changed the code of
// MapRenderer::Draw4SidedDiagonalUpLeft_4EF880 (see "Adding an enum breaks a match" in docs/matching_quirks.md)
// Ped::field_26C_graphic_type: which set of ped sprites is used (Char_B4 picks a base sprite id of 0, 158 or 316)
namespace ped_graphic_type
{
enum
{
    civilian_0 = 0,  // pedestrians, paramedics
    character_1 = 1, // the default; the player, gang members, FBI, script created chars
    cop_2 = 2,       // police, SWAT, army
};
} // namespace ped_graphic_type
