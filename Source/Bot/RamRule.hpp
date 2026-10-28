#pragma once

// When is a ramming car stuck against its target? Pushing a parked truck at a crawl is not ramming: the car must back off and come at
// it again for speed. Pure, tested natively in Scripts/bot/sim/ram_rule_sim.cpp.
inline bool RamRule_Still(float speed, float dist_to_target)
{
    // right up against the target the car creeps (0.005 to 0.02 a frame) as it pushes, and that counts as standing still
    return speed < (dist_to_target < 2.5f ? 0.02f : 0.004f);
}
