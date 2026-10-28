#pragma once

// A walker treats cars as if they were not there and hops over the one in his way. Which car is "in his way"? A pure
// function (no game types), tested natively in Scripts/bot/sim/walk_car_jump_sim.cpp.

struct JumpCar
{
    float x, y;
    float speed; // tiles per frame
};

// Should the walker jump now? A car close ahead and in the path, standing or creeping along (a queue); one coming fast
// is jumped aside from (Bot::Evade), not over
bool WalkCarJump_Decide(float x, float y, float face_x, float face_y, const JumpCar* cars, int car_count);
