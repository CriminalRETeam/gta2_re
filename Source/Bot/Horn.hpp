#pragma once

// The horn: a pedestrian in the way ahead flees from a car that sounds it. Short bursts with gaps (a held horn is a drone), and
// never when the car is out to run the pedestrian down. A pure function (no game types), tested natively in
// Scripts/bot/sim/horn_sim.cpp.

struct HornIn
{
    bool ped_ahead;       // a living pedestrian on foot in the car's way
    float ped_dist;       // tiles ahead along the car's heading
    bool running_over;    // the drive's aim is to run pedestrians down (a gang kill, a frenzy)
};

const int kHornPeriod = 48; // frames from the start of one burst to the next
const int kHornBurst = 8;   // frames the horn is down in a burst
const float kHornNear = 1.5f; // closer than this the car is on them: nothing to warn
const float kHornFar = 12.0f; // further than this they are not in the way yet

bool Horn_Press(const HornIn& in, int frame);
