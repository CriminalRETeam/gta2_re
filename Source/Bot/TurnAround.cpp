#include "TurnAround.hpp"
#include <math.h>

void TurnAround_Reset(TurnState* s)
{
    s->active = false;
    s->reverse = false;
    s->phase = 0;
    s->spin = 0;
    s->blocked = 0;
}

TurnOut TurnAround_Step(TurnState* s, const TurnIn& in)
{
    TurnOut out = {false, 1, false, false};
    const float a = fabsf(in.err);
    if (!s->active)
    {
        if (a <= kTurnEngageDeg)
        {
            return out;
        }
        s->active = true;
        s->phase = 0;
        // the way to go is straight behind: back towards it while there is room, the tail leads
        s->reverse = (a > kTurnTailDeg && in.rear_clear >= kTurnRoom) || (in.fwd_clear < kTurnRoom && in.rear_clear > in.fwd_clear);
    }
    else if (a < kTurnReleaseDeg)
    {
        TurnAround_Reset(s);
        return out;
    }
    s->phase++;
    const bool towards_right = in.err > 0.0f;
    // Room neither way: brake and accelerate with the wheel over turns the car about itself; let go for a moment and again
    // (only after a good while: whether the spin turns the car at all has not been seen yet, a bump is the usual way out)
    s->blocked = (in.fwd_clear < kTurnRoom && in.rear_clear < kTurnRoom) ? s->blocked + 1 : 0;
    if (s->blocked >= kTurnSpinAfter)
    {
        const int cycle = kTurnSpinHold + kTurnSpinLetGo;
        const int at = s->spin % cycle;
        s->spin++;
        out.active = true;
        out.right = towards_right;
        out.spin = at < kTurnSpinHold;
        out.gas = 0;
        return out;
    }
    s->spin = 0;
    const float room = s->reverse ? in.rear_clear : in.fwd_clear;
    // Backing towards the way to go does not go on for ever: after a while the nose is swung round instead
    const bool tail_leads = s->reverse && a > kTurnTailDeg;
    if (tail_leads && s->phase >= kTurnTailFrames && in.fwd_clear >= kTurnRoom)
    {
        s->reverse = false;
        s->phase = 0;
    }
    else if (room < kTurnRoom && (s->phase >= kTurnMinPhase || room < kTurnRoom * 0.5f))
    {
        const float other = s->reverse ? in.fwd_clear : in.rear_clear;
        if (other >= kTurnRoom || other > room)
        {
            s->reverse = !s->reverse;
            s->phase = 0;
        }
    }
    out.active = true;
    out.gas = s->reverse ? -1 : 1;
    // backing: with the way to go behind, the wheel towards it brings the tail round to it; otherwise the opposite wheel swings the nose
    out.right = (s->reverse && a <= kTurnTailDeg) ? !towards_right : towards_right;
    return out;
}
