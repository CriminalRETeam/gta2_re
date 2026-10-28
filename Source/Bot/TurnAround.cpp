#include "TurnAround.hpp"
#include <math.h>

void TurnAround_Reset(TurnState* s)
{
    s->active = false;
    s->reverse = false;
    s->phase = 0;
}

TurnOut TurnAround_Step(TurnState* s, const TurnIn& in)
{
    TurnOut out = {false, 1, false};
    const float a = fabsf(in.err);
    if (!s->active)
    {
        if (a <= kTurnEngageDeg)
        {
            return out;
        }
        s->active = true;
        s->phase = 0;
        s->reverse = in.fwd_clear < kTurnRoom && in.rear_clear > in.fwd_clear;
    }
    else if (a < kTurnReleaseDeg)
    {
        TurnAround_Reset(s);
        return out;
    }
    s->phase++;
    const float room = s->reverse ? in.rear_clear : in.fwd_clear;
    if (s->phase >= kTurnMinPhase && room < kTurnRoom)
    {
        const float other = s->reverse ? in.fwd_clear : in.rear_clear;
        if (other >= kTurnRoom || other > room)
        {
            s->reverse = !s->reverse;
            s->phase = 0;
        }
    }
    const bool towards_right = in.err > 0.0f;
    out.active = true;
    out.gas = s->reverse ? -1 : 1;
    out.right = s->reverse ? !towards_right : towards_right;
    return out;
}
