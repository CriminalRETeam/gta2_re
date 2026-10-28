#pragma once

// On the way to a car, a closer one turns up (traffic stops next to the walker): drop the first for the second? A pure
// function (no game types), tested natively in Scripts/bot/sim/car_switch_sim.cpp. Changing for a car that is barely
// nearer, or one that has to be hijacked (the driver dragged out: it fails often and costs 160 frames when it does),
// or more than once, is the car swapping for no reason.

// cur_dist: tiles to the car being walked to; new_dist: to the one on offer; new_hijack: it has a driver in it;
// switches: how many times the goal has changed car already
bool CarSwitch_ShouldSwitch(float cur_dist, float new_dist, bool new_hijack, int switches);
