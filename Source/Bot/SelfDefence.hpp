#pragma once

// Whether the self-defence reflex fights the ped that is on top of the player. A mugger is hit once (a burst of punches or shots)
// and then left: it takes the cash and runs, and a long fight with it costs more than it saves. With police about it is left
// alone from the start: they get the mugger. Everyone else who is beating the player is fought until it stops. A pure function (no
// game types), tested natively in Scripts/bot/sim/self_defence_sim.cpp.

struct SelfDefenceIn
{
    bool mugger;          // the ped's occupation is a mugger
    bool cops_near;       // police on foot or in cars within 14 tiles
    int frames_on_target; // frames already spent fighting this ped
};

const int kMuggerBurstFrames = 14; // about two bursts of the attack key

bool SelfDefence_ShouldFight(const SelfDefenceIn& in);
