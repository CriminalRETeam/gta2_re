// Native tests of the kiting decision (Source/Bot/KiteCops.cpp), and a small chase: a ped backing away from police
// that walk faster than he does must keep facing them and keep the distance from growing short.
//   g++ -O1 -I../../../Source/Bot -o kite_sim kite_sim.cpp ../../../Source/Bot/KiteCops.cpp && ./kite_sim
#include "KiteCops.hpp"
#include <math.h>
#include <stdio.h>
#include <vector>

static bool NoWalls(void*, float, float)
{
    return false;
}

static bool WallBelow(void*, float, float y)
{
    return y > 20.0f; // everything south of y = 20 is a building
}

static int fails = 0;

static void Check(const char* name, bool ok)
{
    printf("%-52s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

int main()
{
    const KiteCop three_ahead[3] = {{10, 5}, {11, 6}, {9, 7}}; // north of the player at (10,15)
    {
        KiteWorld w = {10, 15, 0, -1, three_ahead, 3, NoWalls, 0}; // facing north, at them
        KiteOut o = Kite_Decide(w);
        Check("crowd ahead: walk backwards, shoot the nearest", o.active && o.move == -1 && o.target_y > 6.5f);
    }
    {
        KiteWorld w = {10, 15, 0, 1, three_ahead, 3, NoWalls, 0}; // facing south, the crowd behind
        KiteOut o = Kite_Decide(w);
        Check("crowd behind: run forwards", o.active && o.move == 1);
    }
    {
        KiteWorld w = {10, 15, 1, 0, three_ahead, 3, NoWalls, 0}; // facing east, the crowd to the north
        KiteOut o = Kite_Decide(w);
        Check("crowd to the side: stand and shoot, no step", o.active && o.move == 0);
    }
    {
        KiteWorld w = {10, 15, 0, -1, three_ahead, 2, NoWalls, 0};
        Check("two police are not a crowd", !Kite_Decide(w).active);
    }
    {
        KiteWorld w = {10, 19.5f, 0, -1, three_ahead, 3, WallBelow, 0}; // the wall is behind the player
        KiteOut o = Kite_Decide(w);
        Check("wall behind: does not back into it", o.active && o.move == 0);
    }
    {
        // a chase: the player faces the nearest cop, backs away at 0.04 a frame; the police close at 0.06
        std::vector<KiteCop> cops;
        const KiteCop start[4] = {{10, 8}, {12, 9}, {8, 9}, {10, 10}};
        for (int i = 0; i < 4; i++) cops.push_back(start[i]);
        float px = 10, py = 16;
        float nearest0 = 0, nearest1 = 0;
        bool faced = true;
        for (int f = 0; f < 120; f++)
        {
            float fx = 0, fy = -1;
            KiteWorld w = {px, py, fx, fy, &cops[0], (int)cops.size(), NoWalls, 0};
            KiteOut o = Kite_Decide(w);
            // face the target at once (the real ped turns in a few frames)
            const float dx = o.target_x - px, dy = o.target_y - py;
            const float l = sqrtf(dx * dx + dy * dy);
            fx = dx / l;
            fy = dy / l;
            KiteWorld w2 = {px, py, fx, fy, &cops[0], (int)cops.size(), NoWalls, 0};
            o = Kite_Decide(w2);
            faced = faced && o.active;
            px += fx * 0.04f * o.move;
            py += fy * 0.04f * o.move;
            float best = 1e9f;
            for (size_t i = 0; i < cops.size(); i++)
            {
                const float cx = px - cops[i].x, cy = py - cops[i].y;
                const float d = sqrtf(cx * cx + cy * cy);
                cops[i].x += cx / d * 0.03f; // police come at 0.03, slower than the backing player? no: closing speed is what matters
                cops[i].y += cy / d * 0.03f;
                if (d < best) best = d;
            }
            if (f == 0) nearest0 = best;
            nearest1 = best;
        }
        // the player backs at 0.04 and the police close at 0.03: the gap must not shrink
        Check("chase: backing away keeps the gap from shrinking", faced && nearest1 >= nearest0 - 0.01f);
    }
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
