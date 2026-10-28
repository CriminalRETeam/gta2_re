#pragma once

// Where to leave the car when the goal cannot be reached by car: the road point that makes the whole trip shortest, by the time of the
// drive to it plus the walk from it, not the road point nearest the goal in a straight line (that may be on the wrong side of a block and
// leave a walk three times the gap). A pure function (no game types), tested natively in Scripts/bot/sim/drop_off_sim.cpp.

struct DropOffCand
{
    float x, y;
    float car_len;  // tiles of road to drive to it, < 0: the car cannot get there
    float walk_len; // tiles of walking from it to the goal, < 0: no way on foot
};

// Frames a tile takes, driving and walking, and getting out of the car
const float kDropCarFramesPerTile = 7.0f;
const float kDropWalkFramesPerTile = 28.0f;
const float kDropExitFrames = 60.0f;

// The cost of a candidate in frames; huge when it cannot be used
float DropOff_Cost(const DropOffCand& c);

// The index of the cheapest candidate, -1 when none can be used. A candidate has to beat the first one (the plain nearest road point) by
// the margin to be chosen over it, so that the choice does not hop between near equals.
int DropOff_Choose(const DropOffCand* c, int n);

const float kDropBeatBy = 0.9f; // a candidate must cost under this fraction of the first one's cost
