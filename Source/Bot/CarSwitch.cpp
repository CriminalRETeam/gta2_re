#include "CarSwitch.hpp"

bool CarSwitch_ShouldSwitch(float cur_dist, float new_dist, bool new_hijack, int switches)
{
    if (switches >= 1)
    {
        return false; // once: no ping-pong between two cars
    }
    if (cur_dist <= 14.0f)
    {
        return false; // nearly there
    }
    if (new_dist >= cur_dist - 8.0f)
    {
        return false; // not clearly nearer
    }
    if (new_hijack && cur_dist <= 40.0f)
    {
        return false; // dragging a driver out is only worth it to save a long walk
    }
    return true;
}
