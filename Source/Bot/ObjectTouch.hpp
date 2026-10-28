#pragma once

// Touching an object. The game answers a phone, takes a pickup or a secret token when the player's collision box overlaps the
// object's (Char::OnObjectTouched). Standing "on" one is standing well inside that box, so the walk aims at the middle of the box and
// is over when the player is inside the middle of it, not at its edge. A pure function (no game types), tested natively in
// Scripts/bot/sim/object_touch_sim.cpp.

struct TouchBox
{
    bool valid;
    float cx, cy; // the middle
    float ax, ay; // unit vector along the long side
    float hl, hw; // half length, half width
};

struct TouchAim
{
    float x, y;  // where to walk to
    float reach; // how near to it counts as there (tiles)
};

// The spot to walk to for an object at (ox, oy) with the given box (which may be missing: valid false)
TouchAim ObjectTouch_Aim(const TouchBox& box, float ox, float oy);

// Is the player's centre inside the middle part of the box (the middle half of each side), or within reach of the object's spot
// when it has no box
bool ObjectTouch_Inside(const TouchBox& box, float ox, float oy, float px, float py);

const float kTouchMinReach = 0.08f; // tiles: never asked to stand closer to the middle than this
const float kTouchMaxReach = 0.30f; // tiles: nor allowed to stop further from it than this
const float kTouchNoBoxReach = 0.25f;
