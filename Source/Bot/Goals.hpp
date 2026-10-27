#pragma once

#include "BotAction.hpp"
#include "WorldModel.hpp"

class Snapshot;
class PickUpAction;
class PhoneAction;
class MissionAction;

// The goal tree (docs/BOT_DESIGN.md, "Goal tree"). A goal is an Action that owns child actions,
// so a node can be a leaf (walk) or a whole subtree (a mission = prepare, phone, follow the
// arrows). Each child starts under a snapshot of its own, so its retries rewind to its start and
// not to the start of the tree.
//
//   MissionsGoal            picks the next phone (tier, distance, what it learned), repeats
//     MissionGoal           one phone
//       PrepareGoal         health, armour, a decent weapon, planned with GOAP
//       PhoneAction
//       MissionAction       the arrow loop, which makes PrepareGoal / DestroyCarGoal children

namespace Weapons
{
enum Purpose
{
    VsPed,
    VsCar
};

// How good a weapon type is for the purpose (0 = useless), and how much ammo makes it worth using
s32 Score(s32 weapon_type, s32 purpose);
s32 MinAmmo(s32 weapon_type);

// The best weapon the player holds with enough ammo; returns its type or -1, and its score in *out_score
s32 BestHeld(const WorldModel& w, s32 purpose, s32* out_score);
} // namespace Weapons

// Makes the weapon of the given type the selected one (cycles with the next weapon control)
class SelectWeaponAction : public Action
{
  public:
    explicit SelectWeaponAction(s32 weapon_type);

    virtual const char* Name() const
    {
        return "select_weapon";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    s32 mType;
    s32 mStart;
};

// Runs its children in order. A child marked optional that fails is skipped.
class SequenceGoal : public Action
{
  public:
    explicit SequenceGoal(const char* name);
    ~SequenceGoal();

    void Add(Action* child, bool optional);

    virtual const char* Name() const
    {
        return mName;
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    s32 Index() const
    {
        return mIdx;
    }

  private:
    enum
    {
        kMax = 8
    };
    const char* mName;
    Action* mChild[kMax];
    bool mOptional[kMax];
    s32 mCount;
    s32 mIdx;
    bool mChildStarted;
    Snapshot* mSnap;
};

// Gets the player to within arrive_dist tiles of (x, y), on foot: by car when that is much cheaper
// (take a parked car, or one waiting in traffic, drive, get out), on foot otherwise. Falls back to
// walking when no car can be got.
class TravelGoal : public Action
{
  public:
    // keep_car: after a drive stay in the car instead of getting out at the end
    TravelGoal(float x, float y, float arrive_dist, bool keep_car = false);
    ~TravelGoal();

    virtual const char* Name() const
    {
        return "travel";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    enum Stage
    {
        Decide,
        GetCar,
        Drive,
        Dismount,
        Walk,
        Train,
        Grab
    };
    enum
    {
        kMaxBan = 8
    };
    void BeginStage(BotContext& ctx, Stage st);
    void Decided(BotContext& ctx);
    bool ChooseCar(BotContext& ctx, s32* out_id, bool* out_hijack, float* out_dist, float max_dist);
    float Remaining(const WorldModel& w) const;

    float mX, mY, mArrive;
    Stage mStage;
    Action* mChild;
    Snapshot* mSnap;
    s32 mCarId;
    s32 mBan[kMaxBan];
    s32 mBanCount;
    s32 mCarFailures;
    bool mWalkOnly;
    bool mKeepCar;
    s32 mReclaims;
    bool mNoTrain;
    s32 mGrabBan[kMaxBan];
    s32 mGrabBanCount;
    s32 mGrabId;
    float mGrabX, mGrabY;
};

// A pickup lying close that is worth a few steps: health or armour when short of it, a better gun,
// or ammo for a gun held low. Not one in ban[]. False when there is none.
bool Goals_WantedPickup(const WorldModel& w, float range, const s32* ban, s32 ban_count, WorldModel::ObjInfo* out);

// Gang peds to kill when a phone refuses: the brief names whom ("kill some Zaibatsu")
s32 Goals_ParseKillHint(const WorldModel& w, const char* brief); // Gang::field_1_gang_idx or -1
s32 Goals_FirstGang(const WorldModel& w, const char* text);

// Kills pedestrians of one gang: hunts those in sight, otherwise wanders their territory
class KillGangGoal : public Action
{
  public:
    // owner_gang / target_respect: also done as soon as that gang's respect for the player reaches the target
    KillGangGoal(s32 gang_idx, s32 kills, s32 max_frames, s32 owner_gang = -1, s32 target_respect = 0);
    ~KillGangGoal();

    virtual const char* Name() const
    {
        return "kill_gang";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    s32 Kills() const
    {
        return mKilled;
    }

  private:
    enum
    {
        kMaxBan = 24
    };
    bool Banned(s32 id) const;
    void NextChild(BotContext& ctx);

    s32 mGang;
    s32 mWant;
    s32 mMaxFrames;
    s32 mStart;
    s32 mKilled;
    Action* mChild;
    bool mHunting;
    s32 mTargetId;
    Snapshot* mSnap;
    s32 mBan[kMaxBan];
    s32 mBanCount;
    s32 mWander;
    s32 mZone;
    s32 mFailures;
    s32 mChildSince;
    bool mRunOver;
    bool mNoCar; // the car failed: on foot from now on
    s32 mCarTries;
    s32 mCarBan[8];
    s32 mCarBanCount;
    s32 mOwner;
    s32 mTargetRespect;
    s32 mFlees;
    bool mFleeing;
};

// Picks up what makes the next fight survivable: full health, armour and a better weapon than the
// one held, whichever are within reach, in the cheapest order (the GOAP planner). Never fails:
// when nothing is worth fetching it is done at once.
class PrepareGoal : public Action
{
  public:
    PrepareGoal(s32 purpose, float max_range, s32 max_frames);
    ~PrepareGoal();

    virtual const char* Name() const
    {
        return "prepare";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    enum
    {
        kMaxBan = 16
    };
    bool Choose(BotContext& ctx);
    bool Banned(s32 id) const;

    s32 mPurpose;
    float mRange;
    s32 mMaxFrames;
    s32 mStart;
    Action* mChild;
    s32 mChildId;
    SelectWeaponAction* mSelect;
    Snapshot* mSnap;
    s32 mBan[kMaxBan];
    s32 mBanCount;
    s32 mPicked;
};

// Destroys a car. Two ways to do it, chosen by estimated cost and tried in turn if one fails:
// shoot it from the ground (the best explosive / fire weapon held), or take a fast car and ram it
// until it stops or burns, then get out and finish it with the gun.
class DestroyCarGoal : public Action
{
  public:
    explicit DestroyCarGoal(s32 car_id);
    ~DestroyCarGoal();

    virtual const char* Name() const
    {
        return "destroy_car_goal";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

  private:
    enum Stage
    {
        Prep,
        Shoot,
        GetCar,
        Ram,
        Leave,
        Finish
    };
    void Decide(BotContext& ctx);
    void BeginStage(BotContext& ctx, Stage st);
    void Next(BotContext& ctx, bool ok);

    s32 mCarId;
    Stage mStage;
    Action* mChild;
    Snapshot* mSnap;
    bool mTriedShoot;
    s32 mRamTries;
    s32 mPickId;
    s32 mPickFailures;
    s32 mBan[8];
    s32 mBanCount;
    s32 mStart;
    s32 mReclaims;
};

// One phone: prepare, answer it, play the mission it starts
class MissionGoal : public Action
{
  public:
    MissionGoal(const WorldModel::ObjInfo& phone);
    ~MissionGoal();

    virtual const char* Name() const
    {
        return "mission_goal";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    // After Failed: the phone gave no mission (the script refused: usually too little respect)
    bool Refused() const
    {
        return mRefused;
    }

    // After a refusal: the brief that came up
    const char* RefusalBrief() const
    {
        return mRefusalBrief;
    }

    s32 PhoneId() const
    {
        return mPhone.id;
    }

    const WorldModel::ObjInfo& PhoneObj() const
    {
        return mPhone;
    }

  private:
    enum Stage
    {
        Prep,
        Travel,
        Phone,
        Play,
        Over
    };
    void BeginStage(BotContext& ctx, Stage st);

    WorldModel::ObjInfo mPhone;
    Stage mStage;
    Action* mChild;
    Snapshot* mSnap;
    bool mRefused;
    char mRefusalBrief[400];
};

// The root: pass missions until want_missions have passed
class MissionsGoal : public Action
{
  public:
    explicit MissionsGoal(s32 want_missions);
    ~MissionsGoal();

    virtual const char* Name() const
    {
        return "missions";
    }
    virtual void Start(BotContext& ctx);
    virtual Status Step(BotContext& ctx);

    s32 Passed() const
    {
        return mPassed;
    }

  private:
    enum
    {
        kMaxPhones = 64
    };
    struct PhoneMemory
    {
        s32 id;
        s32 tries;           // mission attempts that ended badly
        bool passed;
        bool refused;
        s32 respect_at_refusal_v[WorldModel::kMaxGangs];
        s32 hint_gang;       // whom to kill for respect, -1 unknown
        s32 kills_asked;     // kills asked for so far (doubles each refusal)
        s32 owner_gang;      // the gang whose territory the phone is in, -1 unknown
        s32 target_respect;  // the respect asked for the last time it refused
    };

    PhoneMemory* Memory(s32 id, bool create);
    s32 RespectSum(const WorldModel& w) const;
    bool ChoosePhone(BotContext& ctx, WorldModel::ObjInfo* out);

    s32 mWant;
    s32 mPassed;
    s32 mStart;
    MissionGoal* mChild;
    Action* mRaise;        // killing a gang for respect
    s32 mRaisePhone;
    s32 mPreferPhone; // the phone whose respect was just raised: try it first
    Snapshot* mSnap;
    PhoneMemory mMemory[kMaxPhones];
    s32 mMemoryCount;
    s32 mFailures;
    s32 mRaiseFailures;
};

// Difficulty of a phone from its model: 0 green, 1 amber, 2 red
s32 Goals_PhoneTier(s32 phone_model);
