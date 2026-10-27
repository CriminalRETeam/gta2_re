#include "BotAction.hpp"
#include "WorldModel.hpp"
#include "BotOptions.hpp"
#include "input_bits.hpp"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "BotCompat.hpp"

float Bot_WrapDeg(float d)
{
    while (d > 180.0f)
    {
        d -= 360.0f;
    }
    while (d < -180.0f)
    {
        d += 360.0f;
    }
    return d;
}

float Bot_HeadingDeg(float dx, float dy)
{
    return atan2f(dx, -dy) * (180.0f / 3.14159265f);
}

BotContext::BotContext()
    : world(NULL)
    , nav(NULL)
    , road(NULL)
    , rng(NULL)
    , action_snap(NULL)
    , map(NULL)
    , opts(NULL)
    , frame(0)
    , action_start_frame(0)
    , input(0)
    , nav_dump(false)
    , out_dir("")
{
    memset(&foot, 0, sizeof(foot));
    memset(&car, 0, sizeof(car));
}

float BotContext::Steer(const Steering& st, float rot, float px, float py, float aim_x, float aim_y, bool /*unused*/)
{
    const float want = st.sign * Bot_WrapDeg(Bot_HeadingDeg(aim_x - px, aim_y - py) - st.offset);
    const float err = Bot_WrapDeg(want - rot);
    if (fabsf(err) > 8.0f)
    {
        input |= (err * st.turn_sign > 0) ? input_bit::Right : input_bit::Left;
    }
    return err;
}

bool Bot_LineClear(float x0, float y0, float x1, float y1)
{
    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float len = sqrtf(dx * dx + dy * dy);
    const int steps = static_cast<int>(len * 2.0f) + 1;
    for (int i = 1; i < steps; i++)
    {
        const float t = static_cast<float>(i) / steps;
        if (WorldModel::BuildingAt(x0 + dx * t, y0 + dy * t))
        {
            return false;
        }
    }
    return true;
}

Action::Action()
{
    fail_condition[0] = 0;
    fail_detail[0] = 0;
}

Action::Status Action::Fail(const char* condition, const char* fmt, ...)
{
    strncpy(fail_condition, condition, sizeof(fail_condition) - 1);
    fail_condition[sizeof(fail_condition) - 1] = 0;
    va_list args;
    va_start(args, fmt);
    _vsnprintf(fail_detail, sizeof(fail_detail) - 1, fmt, args);
    va_end(args);
    fail_detail[sizeof(fail_detail) - 1] = 0;
    return Failed;
}

Calibrator::Calibrator()
    : mInCar(false)
    , mState(Finished)
    , mStateFrame(0)
    , mX(0)
    , mY(0)
    , mRot(0)
    , mH1(0)
    , mRot1(0)
    , mTurnSign(1)
{
}

void Calibrator::Start(bool in_car)
{
    mInCar = in_car;
    mState = Move1;
    mStateFrame = -1;
}

Action::Status Calibrator::Step(BotContext& ctx, Steering* out, char* fail, u32 fail_size)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    const s32 move1 = mInCar ? 22 : 10;
    const s32 turn = mInCar ? 14 : 8;
    const s32 move2 = mInCar ? 16 : 10;

    if (mStateFrame < 0)
    {
        mStateFrame = ctx.frame;
        mX = pl.x;
        mY = pl.y;
        mRot = pl.rot_deg;
    }

    switch (mState)
    {
        case Move1:
            if (ctx.frame - mStateFrame < move1)
            {
                ctx.input = input_bit::Forward;
                return Action::Running;
            }
            {
                const float dx = pl.x - mX;
                const float dy = pl.y - mY;
                if (dx * dx + dy * dy < 0.04f)
                {
                    _snprintf(fail, fail_size, "calibration: moving forward did not move the %s (%.2f tiles)", mInCar ? "car" : "ped", sqrtf(dx * dx + dy * dy));
                    return Action::Failed;
                }
                mH1 = Bot_HeadingDeg(dx, dy);
                mRot1 = mRot;
                mState = Turn;
                mStateFrame = ctx.frame;
                mRot = pl.rot_deg;
                mX = pl.x;
                mY = pl.y;
            }
            // fall through to Turn on this frame
        case Turn:
            if (ctx.frame - mStateFrame < turn)
            {
                ctx.input = input_bit::Right | (mInCar ? input_bit::Forward : 0);
                return Action::Running;
            }
            mTurnSign = Bot_WrapDeg(pl.rot_deg - mRot) >= 0 ? 1.0f : -1.0f;
            mState = Move2;
            mStateFrame = ctx.frame;
            mRot = pl.rot_deg;
            mX = pl.x;
            mY = pl.y;
            // fall through
        case Move2:
            if (ctx.frame - mStateFrame < move2)
            {
                ctx.input = input_bit::Forward;
                if (ctx.frame - mStateFrame == 4)
                {
                    // measure from here on: the car has settled on its new heading
                    mX = pl.x;
                    mY = pl.y;
                    mRot = pl.rot_deg;
                }
                return Action::Running;
            }
            {
                const float dx = pl.x - mX;
                const float dy = pl.y - mY;
                if (dx * dx + dy * dy < 0.02f)
                {
                    _snprintf(fail, fail_size, "calibration: second move did not move the %s", mInCar ? "car" : "ped");
                    return Action::Failed;
                }
                const float h2 = Bot_HeadingDeg(dx, dy);
                const float drot = Bot_WrapDeg(mRot - mRot1);
                const float dhead = Bot_WrapDeg(h2 - mH1);
                if (fabsf(drot) < 10.0f)
                {
                    _snprintf(fail, fail_size, "calibration: the turn changed the rotation by only %.1f deg", drot);
                    return Action::Failed;
                }
                out->sign = (drot * dhead >= 0) ? 1.0f : -1.0f;
                out->offset = Bot_WrapDeg(mH1 - out->sign * mRot1);
                out->turn_sign = mTurnSign;
                out->valid = true;
                mState = Finished;
                return Action::Done;
            }
        default:
            return Action::Done;
    }
}
