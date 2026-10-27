#include "CalibrateCore.hpp"
#include <math.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif

namespace
{
const unsigned kForward = 0x1; // input_bits.hpp
const unsigned kRight = 0x8;
const int kMaxRetries = 14;

float WrapDeg(float d)
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

float HeadingDeg(float dx, float dy)
{
    return atan2f(dx, -dy) * 180.0f / 3.14159265f;
}
} // namespace

CalibrateCore::CalibrateCore()
    : mInCar(false)
    , mState(Finished)
    , mStateFrame(0)
    , mRetries(0)
    , mX(0)
    , mY(0)
    , mRot(0)
    , mH1(0)
    , mRot1(0)
    , mTurnSign(1)
{
    mWhy[0] = 0;
}

void CalibrateCore::Start(bool in_car)
{
    mInCar = in_car;
    mState = Move1;
    mStateFrame = -1;
    mRetries = 0;
}

// The measurement did not work: wait a little and measure again, a number of times, then give up
bool CalibrateCore::Retry(int frame, const char* what, char* fail, unsigned fail_size)
{
    if (mRetries >= kMaxRetries)
    {
        SNPRINTF(fail, fail_size, "calibration: %s (after %d tries)", what, mRetries + 1);
        return false;
    }
    mRetries++;
    SNPRINTF(mWhy, sizeof(mWhy), "%s", what);
    mState = Wait;
    mStateFrame = frame;
    return true;
}

CalibrateCore::Result CalibrateCore::Step(int frame, float x, float y, float rot_deg, unsigned* input, CalSteering* out, char* fail, unsigned fail_size)
{
    const int move1 = mInCar ? 22 : 10;
    const int turn = mInCar ? 14 : 8;
    const int move2 = mInCar ? 16 : 10;
    *input = 0;

    if (mStateFrame < 0)
    {
        mStateFrame = frame;
        mX = x;
        mY = y;
        mRot = rot_deg;
    }

    switch (mState)
    {
        case Wait:
            if (frame - mStateFrame < 20)
            {
                return Running; // no keys: whatever held the ped may let go
            }
            mState = Move1;
            mStateFrame = -1;
            return Running;
        case Move1:
            if (frame - mStateFrame < move1)
            {
                *input = kForward;
                return Running;
            }
            {
                const float dx = x - mX;
                const float dy = y - mY;
                if (dx * dx + dy * dy < 0.04f)
                {
                    char what[96];
                    SNPRINTF(what, sizeof(what), "moving forward did not move the %s (%.2f tiles)", mInCar ? "car" : "ped", sqrtf(dx * dx + dy * dy));
                    return Retry(frame, what, fail, fail_size) ? Running : Failed;
                }
                mH1 = HeadingDeg(dx, dy);
                mRot1 = mRot;
                mState = Turn;
                mStateFrame = frame;
                mRot = rot_deg;
                mX = x;
                mY = y;
            }
            // fall through to Turn on this frame
        case Turn:
            if (frame - mStateFrame < turn)
            {
                *input = kRight | (mInCar ? kForward : 0);
                return Running;
            }
            mTurnSign = WrapDeg(rot_deg - mRot) >= 0 ? 1.0f : -1.0f;
            mState = Move2;
            mStateFrame = frame;
            mRot = rot_deg;
            mX = x;
            mY = y;
            // fall through
        case Move2:
            if (frame - mStateFrame < move2)
            {
                *input = kForward;
                if (frame - mStateFrame == 4)
                {
                    // measure from here on: the car has settled on its new heading
                    mX = x;
                    mY = y;
                    mRot = rot_deg;
                }
                return Running;
            }
            {
                const float dx = x - mX;
                const float dy = y - mY;
                if (dx * dx + dy * dy < 0.02f)
                {
                    return Retry(frame, mInCar ? "second move did not move the car" : "second move did not move the ped", fail, fail_size) ? Running : Failed;
                }
                const float h2 = HeadingDeg(dx, dy);
                const float drot = WrapDeg(mRot - mRot1);
                const float dhead = WrapDeg(h2 - mH1);
                if (fabsf(drot) < 10.0f)
                {
                    char what[96];
                    SNPRINTF(what, sizeof(what), "the turn changed the rotation by only %.1f deg", drot);
                    return Retry(frame, what, fail, fail_size) ? Running : Failed;
                }
                out->sign = (drot * dhead >= 0) ? 1.0f : -1.0f;
                out->offset = WrapDeg(mH1 - out->sign * mRot1);
                out->turn_sign = mTurnSign;
                out->valid = true;
                mState = Finished;
                return Done;
            }
        default:
            return Done;
    }
}
