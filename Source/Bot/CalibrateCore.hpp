#pragma once

// The steering calibration as a pure state machine (no game types), so Scripts/bot/sim/ can test it with a fake ped
// and car: it drives forward, turns, drives forward again, and works out how the rotation relates to the heading and
// which key turns which way. A ped that does not respond (the first frames of a run, a ped held by something) is
// waited out and measured again, not reported as a failure at once.

struct CalSteering
{
    bool valid;
    float sign;
    float offset;
    float turn_sign;
};

class CalibrateCore
{
  public:
    enum Result
    {
        Running,
        Done,
        Failed
    };
    CalibrateCore();
    void Start(bool in_car);
    // input gets the control bits to press this frame
    Result Step(int frame, float x, float y, float rot_deg, unsigned* input, CalSteering* out, char* fail, unsigned fail_size);

  private:
    enum State
    {
        Move1,
        Turn,
        Move2,
        Wait,
        Finished
    };
    bool Retry(int frame, const char* what, char* fail, unsigned fail_size);
    bool mInCar;
    State mState;
    int mStateFrame;
    int mRetries;
    float mX, mY, mRot;
    float mH1, mRot1;
    float mTurnSign;
    char mWhy[96];
};
