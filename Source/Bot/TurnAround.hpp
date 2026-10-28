#pragma once

// Turning a car round when the way to go is behind it. Go forward with the wheel over towards the way out; when something
// blocks the nose, back up with the wheel over the other way (a car backing swings its nose the other way to the wheel);
// when something blocks the tail, forward again; until the car faces the way to go. The usual three point turn, decided by the
// room ahead and behind. A pure function (no game types), tested natively in Scripts/bot/sim/turn_around_sim.cpp.

struct TurnState
{
    bool active;
    bool reverse; // backing up now
    int phase;    // frames in this direction
};

struct TurnIn
{
    float err;        // degrees from the nose to the way to go; positive: the Right key swings the nose towards it (going forward)
    float fwd_clear;  // free tiles in front of the nose
    float rear_clear; // free tiles behind the tail
};

struct TurnOut
{
    bool active; // false: the car faces the way well enough, drive as usual
    int gas;     // +1 forward, -1 backwards
    bool right;  // hold the Right key (else the Left key)
};

const float kTurnEngageDeg = 100.0f; // the way to go is this far off the nose: turn round
const float kTurnReleaseDeg = 45.0f; // until it is this near
const float kTurnRoom = 0.8f;        // tiles of room a direction needs
const int kTurnMinPhase = 10;        // frames in one direction before switching

void TurnAround_Reset(TurnState* s);
TurnOut TurnAround_Step(TurnState* s, const TurnIn& in);
