#include "ang16.hpp"
#include "Function.hpp"
#include <cmath>

// TODO: init these angles
DEFINE_GLOBAL(Ang16, dword_6F677C, 0x6F677C);
DEFINE_GLOBAL(Ang16, word_6F680C, 0x6F680C);
DEFINE_GLOBAL(Ang16, dword_6F6800, 0x6F6800);
DEFINE_GLOBAL(Ang16, word_6F67DC, 0x6F67DC);

EXTERN_GLOBAL(Ang16, word_669156);

WIP_FUNC(0x405C60)
bool __stdcall Ang16::IsAngleAhead_405C60(Ang16* a1, Ang16* a2);
{
    WIP_IMPLEMENTED;
    return a2->field_0 - a1->field_0 <= word_669156;
}

WIP_FUNC(0x405640)
void Ang16::SnapToAng4_405640()
{
    WIP_IMPLEMENTED;
    if (rValue < 180 || rValue > 1260)
    {
        rValue = 0;
    }
    else if (rValue >= 540)
    {
        rValue = rValue >= 900 ? 1080 : 720;
    }
    else
    {
        rValue = 360;
    }
}

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

MATCH_FUNC(0x4F78F0)
s32 __stdcall Ang16::GetAngleFace_4F78F0(Ang16& a1)
{
    if (a1 <= dword_6F677C || a1 > word_6F680C) //  45° and 315°
    {
        return 2; //  North
    }
    if (a1 < dword_6F6800) // dword_6F6800 = 135°
    {
        return 3; //  East
    }
    return a1 < word_6F67DC ? 1 : 4; // word_6F67DC = 225°  ,  1 = South, 4 = West
}
