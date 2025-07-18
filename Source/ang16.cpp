#include "Function.hpp"
#include "ang16.hpp"
#include <cmath>

MATCH_FUNC(0x406C20)
void Ang16::sub_406C20()
{
    Normalize();
}

MATCH_FUNC(0x409300)
Ang16* Ang16::sub_409300(Ang16* a2, s32 a3)
{
    rValue = a2->rValue;
    Normalize();
    return this;
}

MATCH_FUNC(0x409340)
Ang16* Ang16::sub_409340(Ang16* pRet, Ang16* toSub)
{
    pRet->rValue = rValue - toSub->rValue;
    pRet->Normalize();
    return pRet;
}

MATCH_FUNC(0x4516B0)
Ang16* Ang16::sub_4516B0(s32* a2, s32 a3)
{
    rValue = *a2 >> 14;
    Normalize();
    return this;
}

MATCH_FUNC(0x482740)
Ang16* Ang16::sub_482740(Ang16* a1, s32* a2)
{
    a1->rValue = (s32)*a2 / 71;
    a1->Normalize();
    return a1;
}