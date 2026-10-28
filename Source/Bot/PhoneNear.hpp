#pragma once

// With no arrow on the screen and the player on foot, is there a ringing phone close by that the script may be waiting for (the
// tutorial's last marker is its phone)? Pure, tested natively in Scripts/bot/sim/phone_near_sim.cpp.
struct PhoneSpot
{
    float x, y;
    bool ringing;
};

// The index of the nearest ringing phone within `near_dist` tiles of (px, py), or -1
inline int PhoneNear_Pick(const PhoneSpot* phones, int n, float px, float py, float near_dist)
{
    int best = -1;
    float best_d2 = near_dist * near_dist;
    for (int i = 0; i < n; i++)
    {
        const float dx = phones[i].x - px;
        const float dy = phones[i].y - py;
        const float d2 = dx * dx + dy * dy;
        if (phones[i].ringing && d2 < best_d2)
        {
            best_d2 = d2;
            best = i;
        }
    }
    return best;
}
