#pragma once

// CreditsLine_6C::field_6_string_category: set by CreditsText_FD22::LoadCredits_483F20 from the "#X" tag in the credits
// text, drawn by Frontend::DrawCredits_4B7AE0. Kept in its own header, like car_despawn_status.hpp, so that the enum does
// not land in a header many TUs share.
namespace credits_line_category
{
enum
{
    normal_0 = 0,     // no tag, or #W: white
    unknown_1 = 1,    // any other tag
    dev_names_2 = 2,  // #B: blue
    department_3 = 3, // #G: green (DMA, T2 etc)
    game_name_4 = 4,  // #C: yellow ("GTA2")
};
} // namespace credits_line_category
