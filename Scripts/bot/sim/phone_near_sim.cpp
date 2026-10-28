// Native test of PhoneNear_Pick: which phone the bot walks onto when the script shows no arrow
#include "PhoneNear.hpp"
#include <stdio.h>

static int fails = 0;
static void Check(const char* name, bool ok)
{
    printf("%-70s %s\n", name, ok ? "PASS" : "FAIL");
    fails += ok ? 0 : 1;
}

int main()
{
    // the tutorial's last marker: the phone at (188.5, 37.5), the player standing at (188.1, 37.3)
    PhoneSpot a[3] = {{188.5f, 37.5f, true}, {75.5f, 40.5f, true}, {44.5f, 150.5f, true}};
    Check("the phone under us is picked", PhoneNear_Pick(a, 3, 188.1f, 37.3f, 6.0f) == 0);
    Check("phones far away are not", PhoneNear_Pick(a + 1, 2, 188.1f, 37.3f, 6.0f) == -1);
    PhoneSpot b[2] = {{190.0f, 37.0f, true}, {188.5f, 37.5f, true}};
    Check("the nearer of two is picked", PhoneNear_Pick(b, 2, 188.1f, 37.3f, 6.0f) == 1);
    PhoneSpot c[1] = {{188.5f, 37.5f, false}};
    Check("a phone that is not ringing is left alone", PhoneNear_Pick(c, 1, 188.1f, 37.3f, 6.0f) == -1);
    Check("no phones: none", PhoneNear_Pick(a, 0, 188.1f, 37.3f, 6.0f) == -1);
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
