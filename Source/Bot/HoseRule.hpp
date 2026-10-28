#pragma once
#include <math.h>

// The fire truck's hose in a kill frenzy: it fires straight ahead from the cab, so the truck has to be turned at the pedestrian and
// brought within reach of the jet. Pure, tested natively in Scripts/bot/sim/hose_rule_sim.cpp.
struct HoseDecision
{
    bool forward;
    bool backward;
    bool attack;
};

const float kHoseFireDist = 7.0f;   // tiles the jet is taken to reach
const float kHoseCloseDist = 3.0f;  // no nearer needed: stand off
const float kHoseAimDeg = 12.0f;    // how far off the nose the pedestrian may be for the jet to hit

inline HoseDecision HoseRule_Decide(float dist, float err_deg)
{
    HoseDecision d;
    const bool aligned = fabsf(err_deg) <= kHoseAimDeg;
    d.attack = aligned && dist <= kHoseFireDist;
    d.backward = !aligned && dist < 1.5f; // too near to turn: back off and swing round
    d.forward = !d.backward && (dist > kHoseCloseDist || !aligned);
    return d;
}
