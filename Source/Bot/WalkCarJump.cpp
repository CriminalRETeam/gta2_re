#include "WalkCarJump.hpp"
#include <math.h>

bool WalkCarJump_Decide(float x, float y, float face_x, float face_y, const JumpCar* cars, int car_count)
{
    for (int k = 0; k < car_count; k++)
    {
        const JumpCar& c = cars[k];
        if (c.speed >= 0.12f)
        {
            continue; // coming fast: stepping aside, not hopping
        }
        const float along = (c.x - x) * face_x + (c.y - y) * face_y;
        const float across = (c.x - x) * -face_y + (c.y - y) * face_x;
        // a standing car is hopped from further off than one creeping along, which may well be gone by the time he arrives
        const float reach = c.speed < 0.03f ? 1.9f : 1.2f;
        if (along > 0.0f && along < reach && fabsf(across) < 1.0f)
        {
            return true;
        }
    }
    return false;
}

bool WalkCarJump_StealNow(int frames_into_jump, float car_dist, bool police, bool locked)
{
    // the game's own enter code holds the key four frames and lets it go four: a single frame press is not seen
    return frames_into_jump >= 4 && frames_into_jump <= 26 && ((frames_into_jump / 4) % 2) == 1 && car_dist < 1.3f && !police && !locked;
}
