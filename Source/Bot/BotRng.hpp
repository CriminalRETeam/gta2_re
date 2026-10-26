#pragma once

#include "types.hpp"

// The bot's own random numbers (tie breaks, retry variant order), seeded from -bot_seed. The
// game's rng is never touched, so a run is reproducible from the seed and the inputs.
class BotRng
{
  public:
    explicit BotRng(u32 seed)
        : mState(seed ? seed : 0x9E3779B9u)
    {
    }

    // xorshift32
    u32 Next()
    {
        u32 x = mState;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        mState = x;
        return x;
    }

    // 0 <= result < n (n > 0)
    u32 Below(u32 n)
    {
        return Next() % n;
    }

    u32 State() const
    {
        return mState;
    }

  private:
    u32 mState;
};
