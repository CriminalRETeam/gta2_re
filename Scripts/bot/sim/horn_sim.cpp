// Native test of Horn: bursts, not a drone; only for a pedestrian in the way; never when running them down.
#include "Horn.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-72s %s\n", name, ok ? "PASS" : "FAIL");
    if (!ok)
    {
        fails++;
    }
}

static int Down(const HornIn& in, int frames, int* longest)
{
    int total = 0, run = 0;
    *longest = 0;
    for (int f = 0; f < frames; f++)
    {
        if (Horn_Press(in, f))
        {
            total++;
            run++;
            *longest = run > *longest ? run : *longest;
        }
        else
        {
            run = 0;
        }
    }
    return total;
}

int main()
{
    int longest;
    HornIn in = {true, 6.0f, false};
    int total = Down(in, 480, &longest);
    Check("a pedestrian 6 tiles ahead: bursts, 8 frames in every 48", total == 80 && longest == kHornBurst);
    Check("... never held longer than one burst", longest <= kHornBurst);
    in.running_over = true;
    Check("running pedestrians down: silent", Down(in, 480, &longest) == 0);
    in = HornIn{false, 6.0f, false};
    Check("nobody in the way: silent", Down(in, 480, &longest) == 0);
    in = HornIn{true, 0.8f, false};
    Check("already on them: silent", Down(in, 480, &longest) == 0);
    in = HornIn{true, 20.0f, false};
    Check("20 tiles off: not yet", Down(in, 480, &longest) == 0);
    if (fails == 0)
    {
        printf("all passed\n");
    }
    return fails ? 1 : 0;
}
