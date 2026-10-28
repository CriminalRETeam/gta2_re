// Native test of SelfDefence: muggers are hit once and left; with police about they are left alone.
#include "SelfDefence.hpp"
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

int main()
{
    Check("a mugger on us, no police: fight", SelfDefence_ShouldFight({true, false, 0}));
    Check("... still fighting after 10 frames", SelfDefence_ShouldFight({true, false, 10}));
    Check("... one burst is enough: left alone after 14 frames", !SelfDefence_ShouldFight({true, false, kMuggerBurstFrames}));
    Check("... and for good (200 frames in)", !SelfDefence_ShouldFight({true, false, 200}));
    Check("a mugger with police about: never fought", !SelfDefence_ShouldFight({true, true, 0}));
    Check("anyone else beating us: fought however long it takes", SelfDefence_ShouldFight({false, false, 500}));
    Check("a gang member with police about: still fought (they are not after him)", SelfDefence_ShouldFight({false, true, 5}));
    if (fails == 0)
    {
        printf("all passed\n");
    }
    return fails ? 1 : 0;
}
