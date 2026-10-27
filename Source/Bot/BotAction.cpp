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
    , no_evade_frame(-1000)
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
{
}

void Calibrator::Start(bool in_car)
{
    mCore.Start(in_car);
}

Action::Status Calibrator::Step(BotContext& ctx, Steering* out, char* fail, u32 fail_size)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    unsigned input = 0;
    CalSteering cs;
    memset(&cs, 0, sizeof(cs));
    const CalibrateCore::Result r = mCore.Step(ctx.frame, pl.x, pl.y, pl.rot_deg, &input, &cs, fail, fail_size);
    ctx.input = input;
    if (r == CalibrateCore::Done && cs.valid)
    {
        out->sign = cs.sign;
        out->offset = cs.offset;
        out->turn_sign = cs.turn_sign;
        out->valid = true;
    }
    return r == CalibrateCore::Running ? Action::Running : (r == CalibrateCore::Done ? Action::Done : Action::Failed);
}
