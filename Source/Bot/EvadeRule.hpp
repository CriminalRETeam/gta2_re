#pragma once

// Is a car about to run the pedestrian over, and which way to step. A pure function (no game types), tested natively in
// Scripts/bot/sim/evade_sim.cpp. A police car pushing in at walking pace squashes a ped as surely as a fast one, so the slow ones count
// (the reach shrinks with the speed); and the frenzies, which used to be left out, are not.

struct EvadeCar
{
    float x, y;
    float vx, vy; // tiles per frame
};

const float kEvadeMinSpeed = 0.04f; // slower than this a car is not moving into us
const float kEvadeNear = 6.0f;      // tiles; cars further off are not yet a threat

// The index of the car that threatens a pedestrian at (px, py), or -1. *side is +1 / -1: the side of its path the pedestrian stands on
// (step that way); *along the tiles ahead of the car
int Evade_Threat(float px, float py, const EvadeCar* cars, int n, float* side, float* along);
