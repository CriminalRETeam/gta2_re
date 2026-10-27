#include "KiteCops.hpp"
#include <math.h>

KiteOut Kite_Decide(const KiteWorld& w)
{
    KiteOut out;
    out.active = false;
    out.target_x = w.x;
    out.target_y = w.y;
    out.move = 0;
    if (w.cop_count < kKiteCrowd)
    {
        return out;
    }
    // the nearest to shoot at, the middle of the crowd to get away from
    float nearest = 1e9f;
    float cx = 0.0f;
    float cy = 0.0f;
    for (int i = 0; i < w.cop_count; i++)
    {
        const float dx = w.cops[i].x - w.x;
        const float dy = w.cops[i].y - w.y;
        const float d = dx * dx + dy * dy;
        if (d < nearest)
        {
            nearest = d;
            out.target_x = w.cops[i].x;
            out.target_y = w.cops[i].y;
        }
        cx += w.cops[i].x;
        cy += w.cops[i].y;
    }
    cx /= w.cop_count;
    cy /= w.cop_count;
    out.active = true;
    float rx = w.x - cx;
    float ry = w.y - cy;
    float rl = sqrtf(rx * rx + ry * ry);
    if (rl < 0.01f)
    {
        rx = w.x - out.target_x;
        ry = w.y - out.target_y;
        rl = sqrtf(rx * rx + ry * ry);
    }
    if (rl < 0.01f)
    {
        return out;
    }
    rx /= rl;
    ry /= rl;
    // walking backwards takes the player along -face, forwards along face
    const float fwd = w.face_x * rx + w.face_y * ry; // cos of the angle between the facing and the way out
    const float cos_back = -fwd;
    int move = 0;
    if (cos_back > 0.5f)
    {
        move = -1;
    }
    else if (fwd > 0.5f)
    {
        move = 1;
    }
    if (move != 0)
    {
        // a wall (or the water) in the way of the step: stand and shoot instead
        const float sx = w.x + w.face_x * 1.2f * move;
        const float sy = w.y + w.face_y * 1.2f * move;
        if (w.solid(w.user, sx, sy))
        {
            move = 0;
        }
    }
    out.move = move;
    return out;
}
