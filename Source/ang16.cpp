#include "ang16.hpp"
#include "Function.hpp"
#include <cmath>

DEFINE_GLOBAL_INIT(Ang16, dword_6F677C, Ang16(0xB4), 0x6F677C);
DEFINE_GLOBAL_INIT(Ang16, word_6F680C, Ang16(0x4EC), 0x6F680C);
DEFINE_GLOBAL_INIT(Ang16, dword_6F6800, Ang16(0x21C), 0x6F6800);
DEFINE_GLOBAL_INIT(Ang16, word_6F67DC, Ang16(0x384), 0x6F67DC);

EXTERN_GLOBAL(Ang16, word_669156);

WIP_FUNC(0x405C60)
bool __stdcall Ang16::IsAngleAhead_405C60(Ang16* a1, Ang16* a2)
{
    WIP_IMPLEMENTED;
    return *a2 - *a1 <= word_669156;
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

WIP_FUNC(0x4056A0)
u8 Ang16::GetOctant_4056A0()
{
    return rValue / 180;
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
Ang16* Ang16::sub_4516B0(Fix16* a2, s32 a3)
{
    rValue = a2->ToInt();
    Normalize();
    return this;
}

MATCH_FUNC(0x482740)
Ang16 Ang16::Fix16_To_Ang16_482740(Fix16& a2)
{
    return Ang16(a2.mValue / 71, 0);
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

MATCH_FUNC(0x405B60)
Fix16 __stdcall Ang16::NormalizeAngleDeltaScaled_405B60(Ang16& a2, Ang16& a3, Ang16& a4)
{
    Ang16 d = a3 - a2;
    if (d > word_669156)
    {
        d = -d;
        return d.divideBy_40E640(a4);
    }
    else
    {
        return d.divideBy_40E640(a4);
    }
}

// TODO: move
WIP_FUNC(0x451730)
void __stdcall Ang16::PolarToCartesian_451730(Ang16& angle, Fix16& radius, Fix16& ret1, Fix16& ret2)
{
    ret1 = radius * Ang16::sine_40F500(angle);
    ret2 = radius * Ang16::cosine_40F520(angle);
}
