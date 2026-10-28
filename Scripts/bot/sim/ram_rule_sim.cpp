// Native test of RamRule_Still
#include "RamRule.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-70s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

int main()
{
    Check("pushing a parked truck at 0.01 a frame, 1 tile from it: still", RamRule_Still(0.01f, 1.0f));
    Check("pushing at 0.0 right against it: still", RamRule_Still(0.0f, 1.0f));
    Check("driving at it at 0.1 from 1 tile: not still", !RamRule_Still(0.1f, 1.0f));
    Check("creeping at 0.01 far from the target: not stuck (just slow)", !RamRule_Still(0.01f, 12.0f));
    Check("stopped far from the target: still", RamRule_Still(0.0f, 12.0f));
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
