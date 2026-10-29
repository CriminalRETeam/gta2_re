#include "BoxSweep.hpp"
#include <stdio.h>
static int fails = 0;
static void Check(const char* n, bool ok) { printf("%-70s %s\n", n, ok ? "PASS" : "FAIL"); fails += ok ? 0 : 1; }
static bool Near(float a, float b) { return fabsf(a - b) < 0.0001f; }
int main()
{
    float cx, cy, hl, hw;
    BoxSweep(10.0f, 10.0f, 1.0f, 0.0f, 0.55f, 0.33f, 0.0f, 0.0f, 3, &cx, &cy, &hl, &hw);
    Check("standing: the same box", Near(cx, 10.0f) && Near(cy, 10.0f) && Near(hl, 0.55f) && Near(hw, 0.33f));
    BoxSweep(10.0f, 10.0f, 1.0f, 0.0f, 0.55f, 0.33f, 0.2f, 0.0f, 3, &cx, &cy, &hl, &hw);
    Check("driving along its axis: longer by the travel, centre half-way", Near(cx, 10.3f) && Near(cy, 10.0f) && Near(hl, 0.85f) && Near(hw, 0.33f));
    BoxSweep(10.0f, 10.0f, 1.0f, 0.0f, 0.55f, 0.33f, -0.2f, 0.0f, 3, &cx, &cy, &hl, &hw);
    Check("reversing: the same growth the other way", Near(cx, 9.7f) && Near(hl, 0.85f));
    BoxSweep(10.0f, 10.0f, 0.0f, -1.0f, 0.55f, 0.33f, 0.0f, -0.1f, 3, &cx, &cy, &hl, &hw);
    Check("facing -Y, going -Y: grows along its own length", Near(cy, 9.85f) && Near(hl, 0.7f) && Near(hw, 0.33f));
    BoxSweep(10.0f, 10.0f, 1.0f, 0.0f, 0.55f, 0.33f, 0.0f, 0.1f, 3, &cx, &cy, &hl, &hw);
    Check("sliding sideways: wider, not longer", Near(hw, 0.48f) && Near(hl, 0.55f) && Near(cy, 10.15f));
    printf("%s\n", fails ? "FAILED" : "all passed");
    return fails ? 1 : 0;
}
