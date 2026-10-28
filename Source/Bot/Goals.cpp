#include "Goals.hpp"
#include "CarSwitch.hpp"
#include "WalkAction.hpp"
#include "NavFoot.hpp"
#include "NavCar.hpp"
#include "CarActions.hpp"
#include "ChaseActions.hpp"
#include "TrainActions.hpp"
#include "CollectActions.hpp"
#include "MissionActions.hpp"
#include "Goap.hpp"
#include "Snapshot.hpp"
#include "BotLog.hpp"
#include "BotRng.hpp"
#include "input_bits.hpp"
#include <math.h>
#include <string.h>
#include "BotCompat.hpp"

// ---------------------------------------------------------------------------------------------
// Weapons

namespace Weapons
{
static const s32 kPedScore[11] = {3, 8, 4, 2, 3, 3, 7, 1, 8, 8, 5};
static const s32 kCarScore[11] = {1, 3, 10, 0, 7, 6, 2, 0, 8, 3, 1};

s32 Score(s32 t, s32 purpose)
{
    if (t < 0 || t > 10)
    {
        return 0;
    }
    return purpose == VsCar ? kCarScore[t] : kPedScore[t];
}

s32 MinAmmo(s32 t)
{
    switch (t)
    {
        case 2:
        case 4:
        case 5:
            return 3;
        case 8:
            return 20;
        default:
            return 10;
    }
}

s32 BestHeld(const WorldModel& w, s32 purpose, s32* out_score)
{
    s32 best = -1;
    s32 best_score = 0;
    for (s32 t = 0; t <= 10; t++)
    {
        if (w.weapon_ammo[t] >= MinAmmo(t) && Score(t, purpose) > best_score)
        {
            best = t;
            best_score = Score(t, purpose);
        }
    }
    if (out_score)
    {
        *out_score = best_score;
    }
    return best;
}
} // namespace Weapons

// Cost, in frames, of a way that does not exist or does not work: so large that any real way beats it
static const float kHopeless = 1e9f;

s32 Goals_PhoneTier(s32 phone_model)
{
    s32 tier = (phone_model - 176) / 2;
    return tier < 0 ? 0 : (tier > 2 ? 2 : tier);
}

// Every child of a goal starts under a snapshot of its own
static void BeginChild(BotContext& ctx, Snapshot** snap, Action* child)
{
    if (ctx.map)
    {
        if (!*snap)
        {
            *snap = new Snapshot(ctx.map);
        }
        (*snap)->Capture();
        ctx.action_snap = *snap;
    }
    ctx.action_start_frame = ctx.frame;
    child->Start(ctx);
}

// ---------------------------------------------------------------------------------------------
SelectWeaponAction::SelectWeaponAction(s32 weapon_type)
    : mType(weapon_type)
    , mStart(0)
{
}

void SelectWeaponAction::Start(BotContext& ctx)
{
    mStart = ctx.frame;
}

Action::Status SelectWeaponAction::Step(BotContext& ctx)
{
    ctx.input = 0;
    if (ctx.world->player.weapon == mType)
    {
        return Done;
    }
    if (ctx.frame - mStart > 80)
    {
        return Fail("action_failed", "select weapon %d: still on %d after 80 frames", mType, ctx.world->player.weapon);
    }
    if ((ctx.frame - mStart) % 4 == 0)
    {
        ctx.input = input_bit::NextWeapon;
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
SequenceGoal::SequenceGoal(const char* name)
    : mName(name)
    , mCount(0)
    , mIdx(0)
    , mChildStarted(false)
    , mSnap(NULL)
{
    for (int i = 0; i < kMax; i++)
    {
        mChild[i] = NULL;
        mOptional[i] = false;
    }
}

SequenceGoal::~SequenceGoal()
{
    for (int i = 0; i < mCount; i++)
    {
        delete mChild[i];
    }
    delete mSnap;
}

void SequenceGoal::Add(Action* child, bool optional)
{
    if (mCount < kMax)
    {
        mChild[mCount] = child;
        mOptional[mCount] = optional;
        mCount++;
    }
}

void SequenceGoal::Start(BotContext& ctx)
{
    mIdx = 0;
    mChildStarted = false;
}

Action::Status SequenceGoal::Step(BotContext& ctx)
{
    ctx.input = 0;
    while (mIdx < mCount)
    {
        if (!mChildStarted)
        {
            BeginChild(ctx, &mSnap, mChild[mIdx]);
            mChildStarted = true;
        }
        const Status s = mChild[mIdx]->Step(ctx);
        if (s == Running)
        {
            return Running;
        }
        if (s == Failed && !mOptional[mIdx])
        {
            return Fail(mChild[mIdx]->fail_condition, "%s step %d (%s): %s", mName, mIdx, mChild[mIdx]->Name(), mChild[mIdx]->fail_detail);
        }
        mIdx++;
        mChildStarted = false;
        ctx.input = 0;
    }
    return Done;
}

// ---------------------------------------------------------------------------------------------
PrepareGoal::PrepareGoal(s32 purpose, float max_range, s32 max_frames)
    : mPurpose(purpose)
    , mRange(max_range)
    , mMaxFrames(max_frames)
    , mStart(0)
    , mChild(NULL)
    , mChildId(0)
    , mSelect(NULL)
    , mSnap(NULL)
    , mBanCount(0)
    , mPicked(0)
{
}

PrepareGoal::~PrepareGoal()
{
    delete mChild;
    delete mSelect;
    delete mSnap;
}

void PrepareGoal::Start(BotContext& ctx)
{
    delete mChild;
    mChild = NULL;
    delete mSelect;
    mSelect = NULL;
    mStart = ctx.frame;
    mBanCount = 0;
    mPicked = 0;
}

bool PrepareGoal::Banned(s32 id) const
{
    for (int i = 0; i < mBanCount; i++)
    {
        if (mBan[i] == id)
        {
            return true;
        }
    }
    return false;
}

// Plans the pickups worth fetching and starts the first; false when there is nothing to fetch
bool PrepareGoal::Choose(BotContext& ctx)
{
    const WorldModel& w = *ctx.world;
    const WorldModel::PlayerInfo& pl = w.player;
    Goap::Entity ents[Goap::kMaxEntities];
    const WorldModel::ObjInfo* objs[Goap::kMaxEntities];
    s32 count = 0;
    s32 listed[3] = {0, 0, 0};
    const bool need_health = pl.hp < 85;
    const bool need_armour = pl.armour < 6;
    s32 best_score = 0;
    Weapons::BestHeld(w, mPurpose, &best_score);
    const s32 want_score = mPurpose == Weapons::VsCar ? 8 : 7;
    const bool need_weapon = best_score < want_score;
    for (int i = 0; i < w.pickup_count && count < Goap::kMaxEntities; i++)
    {
        const WorldModel::ObjInfo& o = w.pickups[i];
        const float dx = o.x - pl.x;
        const float dy = o.y - pl.y;
        if (dx * dx + dy * dy > mRange * mRange)
        {
            break; // nearest first
        }
        if (Banned(o.id))
        {
            continue;
        }
        s32 kind = -1;
        float cost = 20.0f;
        if (o.model == 230 && need_health)
        {
            kind = 0;
        }
        else if (o.model == 231 && need_armour)
        {
            kind = 1;
        }
        else if (o.model >= 200 && o.model <= 227 && need_weapon)
        {
            const s32 sc = Weapons::Score(o.model - 200, mPurpose);
            if (sc > best_score && listed[2] < 4)
            {
                kind = 2;
                cost = 20.0f + static_cast<float>(10 - sc) * 30.0f; // the better gun is worth a longer walk
            }
        }
        if (kind < 0)
        {
            continue;
        }
        ents[count].id = o.id;
        ents[count].kind = kind;
        ents[count].x = o.x;
        ents[count].y = o.y;
        ents[count].cost = cost;
        objs[count] = &o;
        listed[kind]++;
        count++;
    }
    if (count == 0)
    {
        return false;
    }
    Goap::Goal goal;
    memset(&goal, 0, sizeof(goal));
    goal.need[0] = listed[0] ? 1 : 0;
    goal.need[1] = listed[1] ? 1 : 0;
    goal.need[2] = listed[2] ? 1 : 0;
    Goap::Plan plan;
    Goap::MakePlan(ents, count, goal, pl.x, pl.y, 22.0f, ctx.rng->Next(), &plan);
    if (!plan.found || plan.length == 0)
    {
        return false;
    }
    const Goap::Entity& first = ents[plan.order[0]];
    BotDecision("f=%d prepare: hp %d armour %d weapon %d (best score %d); %d pickups in reach, plan %d, first %s #%d model %d at (%.0f,%.0f)", ctx.frame, pl.hp, pl.armour, pl.weapon, best_score, count, plan.length,
                first.kind == 0 ? "health" : (first.kind == 1 ? "armour" : "weapon"), first.id, objs[plan.order[0]]->model, first.x, first.y);
    mChildId = first.id;
    mChild = new PickUpAction(first.id, first.x, first.y);
    BeginChild(ctx, &mSnap, mChild);
    return true;
}

Action::Status PrepareGoal::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid || pl.dead)
    {
        return Running;
    }
    if (pl.in_car)
    {
        return Done; // the pickups are walked onto
    }
    if (ctx.frame - mStart > mMaxFrames)
    {
        BotDecision("f=%d prepare: out of time, %d pickups taken", ctx.frame, mPicked);
        return Done;
    }
    if (mSelect)
    {
        const Status s = mSelect->Step(ctx);
        return s == Running ? Running : Done;
    }
    for (;;)
    {
        if (!mChild)
        {
            if (Choose(ctx))
            {
                continue;
            }
            // Nothing more to fetch: use the best gun held
            s32 sc = 0;
            const s32 best = Weapons::BestHeld(*ctx.world, mPurpose, &sc);
            if (best >= 0 && best != pl.weapon && sc > Weapons::Score(pl.weapon, mPurpose))
            {
                BotDecision("f=%d prepare: selecting weapon %d (score %d) over %d", ctx.frame, best, sc, pl.weapon);
                mSelect = new SelectWeaponAction(best);
                mSelect->Start(ctx);
                return Running;
            }
            return Done;
        }
        const Status s = mChild->Step(ctx);
        if (s == Running)
        {
            return Running;
        }
        if (s == Done)
        {
            mPicked++;
        }
        else if (mBanCount < kMaxBan)
        {
            BotDecision("f=%d prepare: pickup #%d failed (%s): %s", ctx.frame, mChildId, mChild->fail_condition, mChild->fail_detail);
            mBan[mBanCount++] = mChildId;
        }
        delete mChild;
        mChild = NULL;
        ctx.input = 0;
        return Running; // plan again next frame, with the world as it is now
    }
}

// ---------------------------------------------------------------------------------------------
TravelGoal::TravelGoal(float x, float y, float arrive_dist, bool keep_car)
    : mX(x)
    , mY(y)
    , mArrive(arrive_dist)
    , mStage(Decide)
    , mChild(NULL)
    , mSnap(NULL)
    , mCarId(0)
    , mBanCount(0)
    , mCarFailures(0)
    , mWalkOnly(false)
    , mKeepCar(keep_car)
    , mReclaims(0)
    , mCarSwitches(0)
    , mWalkOnlyUntil(0)
    , mNoTrain(false)
    , mGrabBanCount(0)
    , mGrabId(0)
    , mGrabX(0.0f)
    , mGrabY(0.0f)
    , mAways(0)
    , mRoadTries(0)
    , mRoadX(0.0f)
    , mRoadY(0.0f)
    , mWaitStart(0)
    , mAwayX(0.0f)
    , mAwayY(0.0f)
{
}

TravelGoal::~TravelGoal()
{
    delete mChild;
    delete mSnap;
}

void TravelGoal::Start(BotContext& ctx)
{
    delete mChild;
    mChild = NULL;
    mStage = Decide;
    mBanCount = 0;
    mCarFailures = 0;
    mWalkOnly = false;
    mGrabBanCount = 0;
    mAways = 0;
    mRoadTries = 0;
}

bool Goals_WantedPickup(const WorldModel& w, float range, const s32* ban, s32 ban_count, WorldModel::ObjInfo* out)
{
    const WorldModel::PlayerInfo& pl = w.player;
    s32 held = 0;
    Weapons::BestHeld(w, Weapons::VsPed, &held);
    for (int i = 0; i < w.pickup_count; i++)
    {
        const WorldModel::ObjInfo& o = w.pickups[i];
        const float dx = o.x - pl.x;
        const float dy = o.y - pl.y;
        if (dx * dx + dy * dy > range * range)
        {
            break; // nearest first
        }
        bool banned = false;
        for (int b = 0; b < ban_count; b++)
        {
            banned = banned || ban[b] == o.id;
        }
        if (banned)
        {
            continue;
        }
        bool want = false;
        if (o.model == 230)
        {
            want = pl.hp < 90;
        }
        else if (o.model == 231)
        {
            want = pl.armour < 10;
        }
        else if (o.model >= 200 && o.model <= 227)
        {
            const s32 t = o.model - 200;
            const s32 sc = Weapons::Score(t, Weapons::VsPed);
            want = sc > held || (sc >= 3 && w.weapon_ammo[t] < Weapons::MinAmmo(t) * 3);
        }
        if (want)
        {
            *out = o;
            return true;
        }
    }
    return false;
}

// A walkable spot about dist tiles from the player, away from the nearest cop (on foot or in a police car)
static bool Goals_AwayPoint(const WorldModel& w, float dist, float* out_x, float* out_y)
{
    const WorldModel::PlayerInfo& pl = w.player;
    float cx = pl.x + 1.0f;
    float cy = pl.y;
    float best = 1e9f;
    for (int i = 0; i < w.ped_count; i++)
    {
        if (w.peds[i].cop && !w.peds[i].dead && w.peds[i].dist < best)
        {
            best = w.peds[i].dist;
            cx = w.peds[i].x;
            cy = w.peds[i].y;
        }
    }
    for (int i = 0; i < w.car_count; i++)
    {
        if (w.cars[i].police && w.cars[i].dist < best)
        {
            best = w.cars[i].dist;
            cx = w.cars[i].x;
            cy = w.cars[i].y;
        }
    }
    float ax = pl.x - cx;
    float ay = pl.y - cy;
    const float al = sqrtf(ax * ax + ay * ay);
    ax = al > 0.01f ? ax / al : 1.0f;
    ay = al > 0.01f ? ay / al : 0.0f;
    for (int a = 0; a < 8; a++)
    {
        const float ang = (a % 2 == 0 ? 1 : -1) * (a / 2) * 0.6f;
        const float px = pl.x + (ax * cosf(ang) - ay * sinf(ang)) * dist;
        const float py = pl.y + (ax * sinf(ang) + ay * cosf(ang)) * dist;
        if (px < 3 || py < 3 || px > 250 || py > 250)
        {
            continue;
        }
        const int tt = WorldModel::TileType(static_cast<int>(px), static_cast<int>(py));
        if (tt == 0 || tt == 4)
        {
            continue;
        }
        *out_x = px;
        *out_y = py;
        return true;
    }
    return false;
}

float TravelGoal::Remaining(const WorldModel& w) const
{
    const float dx = mX - w.player.x;
    const float dy = mY - w.player.y;
    return sqrtf(dx * dx + dy * dy);
}

// A car worth walking to: parked and unlocked, or waiting in traffic (hijacked); never a police
// car, a mission car or a wreck. dist is the path length on foot.
bool TravelGoal::ChooseCar(BotContext& ctx, s32* out_id, bool* out_hijack, float* out_dist, float max_dist)
{
    CarPick ban;
    ban.banned_count = mBanCount;
    for (int i = 0; i < mBanCount; i++)
    {
        ban.banned[i] = mBan[i];
    }
    ban.has_target = true;
    ban.target_x = mX;
    ban.target_y = mY;
    return Cars_Pick(ctx, ban, max_dist, false, out_id, out_hijack, out_dist);
}

void TravelGoal::BeginStage(BotContext& ctx, Stage st)
{
    delete mChild;
    mChild = NULL;
    mStage = st;
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    switch (st)
    {
        case GetCar:
        {
            WorldModel::CarInfo c;
            const bool hijack = ctx.world->FindCar(mCarId, &c) && c.has_driver;
            mChild = new EnterCarAction(mCarId, hijack);
            break;
        }
        case Drive:
            mChild = new DriveAction(mX, mY, mArrive > 6.0f ? mArrive : 6.0f);
            break;
        case Dismount:
            mChild = new ExitCarAction();
            break;
        case Walk:
        {
            WalkAction* walk = new WalkAction(mX, mY, false, mArrive);
            // a long way to go and no police about: a car jumped over on the way is taken, not left
            walk->SetStealOnJump(!mWalkOnly && Remaining(*ctx.world) > 25.0f && !ctx.world->CopsNear(14.0f));
            mChild = walk;
            break;
        }
        case Train:
            mChild = new RideTrainGoal(mX, mY);
            break;
        case Grab:
            mChild = new PickUpAction(mGrabId, mGrabX, mGrabY);
            break;
        case Away:
            mChild = new WalkAction(mAwayX, mAwayY, false, 4.0f);
            break;
        case ToRoad:
            mChild = new WalkAction(mRoadX, mRoadY, false, 0.8f); // onto the lane itself: traffic stops for a pedestrian in front of it
            break;
        case WaitCar:
            mWaitStart = ctx.frame;
            return; // no child: Step waits at the kerb

        default:
            return;
    }
    BeginChild(ctx, &mSnap, mChild);
    (void)pl;
}

void TravelGoal::Decided(BotContext& ctx)
{
    const WorldModel& w = *ctx.world;
    const WorldModel::PlayerInfo& pl = w.player;
    const float remaining = Remaining(w);
    if (remaining <= mArrive)
    {
        mStage = Walk; // arrived: the walk finishes at once
        BeginStage(ctx, Walk);
        return;
    }
    if (pl.in_car)
    {
        BotDecision("f=%d travel: in car #%d, %.0f tiles to (%.0f,%.0f)", ctx.frame, pl.car_id, remaining, mX, mY);
        BeginStage(ctx, remaining > mArrive + 6.0f ? Drive : Dismount);
        return;
    }
    // Police about: no car is taken in front of them (unless the clock is running); walk away first
    if (!w.TimeLimited() && w.CopsNear(14.0f) && mAways < 2 && remaining > 15.0f && Goals_AwayPoint(w, 25.0f, &mAwayX, &mAwayY))
    {
        mAways++;
        BotDecision("f=%d travel: police close by: moving away to (%.0f,%.0f) before taking a car (%d)", ctx.frame, mAwayX, mAwayY, mAways);
        BeginStage(ctx, Away);
        return;
    }
    // on foot: cost of walking against the cost of getting a car and driving
    float walk_cost = remaining * 22.0f * 1.4f;
    if (ctx.nav->FindPath(static_cast<u8>(pl.x), static_cast<u8>(pl.y), static_cast<u8>(pl.z + 0.5f), static_cast<u8>(mX), static_cast<u8>(mY)))
    {
        walk_cost = static_cast<float>(ctx.nav->PathLength()) * 28.0f;
    }
    else
    {
        walk_cost = kHopeless; // no way on foot
    }
    s32 car = 0;
    bool hijack = false;
    float to_car = 0.0f;
    float best_other = walk_cost;
    // Frames to get there by car; only meaningful when a car was found (have_car)
    bool have_car = false;
    float car_cost = kHopeless;
    if (!mWalkOnly && remaining > 20.0f && ChooseCar(ctx, &car, &hijack, &to_car, remaining > 80.0f ? 70.0f : 30.0f))
    {
        have_car = true;
        car_cost = to_car * 22.0f + remaining * 1.3f * 9.0f + 200.0f;
        best_other = car_cost < best_other ? car_cost : best_other;
    }
    // A train: the tracks are long loops across the map and the trains run about three times as fast as a walker
    if (!mNoTrain && !mWalkOnly && remaining > 60.0f)
    {
        s32 bid = -1;
        s32 aid = -1;
        const float train_cost = RideTrainGoal::Estimate(w, pl.x, pl.y, mX, mY, &bid, &aid);
        BotDecision("f=%d travel: train estimate %.0f (stations %d, board %d, alight %d) against %.0f", ctx.frame, train_cost, w.station_count, bid, aid, best_other);
        if (train_cost > 0.0f && train_cost < best_other * 0.8f)
        {
            BotDecision("f=%d travel: %.0f tiles to (%.0f,%.0f): by train from station %d to %d (estimate %.0f frames, against %.0f on foot and %.0f by car)", ctx.frame, remaining, mX, mY, bid, aid, train_cost, walk_cost, car_cost);
            BeginStage(ctx, Train);
            return;
        }
    }
    if (have_car)
    {
        if (car_cost < walk_cost * (remaining > 80.0f ? 0.85f : 0.7f))
        {
            mCarId = car;
            BotDecision("f=%d travel: %.0f tiles to (%.0f,%.0f); taking car #%d%s %.0f tiles away (cost %.0f against %.0f on foot)", ctx.frame, remaining, mX, mY, car, hijack ? " (hijack)" : "", to_car, car_cost, walk_cost);
            BeginStage(ctx, GetCar);
            return;
        }
    }
    // No car to be had here on a long trip: go to the road, where the cars are, and wait for one at the kerb
    if (!have_car && !mWalkOnly && remaining > 50.0f && mRoadTries < 2 && ctx.road && ctx.road->NearestLane(pl.x, pl.y, mX, mY, 40.0f, &mRoadX, &mRoadY))
    {
        const float rdx = mRoadX - pl.x;
        const float rdy = mRoadY - pl.y;
        if (rdx * rdx + rdy * rdy > 4.0f * 4.0f && ctx.nav->FindPath(static_cast<u8>(pl.x), static_cast<u8>(pl.y), static_cast<u8>(pl.z + 0.5f), static_cast<u8>(mRoadX), static_cast<u8>(mRoadY)) && !ctx.nav->Approximate())
        {
            mRoadTries++;
            BotDecision("f=%d travel: %.0f tiles to (%.0f,%.0f) and no car near: going to the road at (%.0f,%.0f), where the cars are (%d)", ctx.frame, remaining, mX, mY, mRoadX, mRoadY, mRoadTries);
            BeginStage(ctx, ToRoad);
            return;
        }
        if (rdx * rdx + rdy * rdy <= 4.0f * 4.0f && mRoadTries < 2)
        {
            mRoadTries++;
            BotDecision("f=%d travel: at the road already: waiting for a car (%d)", ctx.frame, mRoadTries);
            BeginStage(ctx, WaitCar);
            return;
        }
    }
    BotDecision("f=%d travel: walking %.0f tiles to (%.0f,%.0f) (%.0f frames on foot; %d cars known, walk only %d)", ctx.frame, remaining, mX, mY, walk_cost, w.car_count, mWalkOnly);
    BeginStage(ctx, Walk);
}

Action::Status TravelGoal::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (mWalkOnly && mWalkOnlyUntil > 0 && ctx.frame >= mWalkOnlyUntil)
    {
        BotDecision("f=%d travel: walk only has run its 900 frames: cars are allowed again", ctx.frame);
        mWalkOnly = false;
        mWalkOnlyUntil = 0;
        mCarFailures = 0;
    }
    if (mStage == Decide)
    {
        if (ctx.frame - ctx.action_start_frame >= 30) // the player may still be falling into the map at the start of a run
        {
            Decided(ctx);
        }
        return Running;
    }
    // Stood on a car on the way and took it (the walker's jump): drive on from here
    if (mStage == Walk && pl.in_car && !mWalkOnly)
    {
        BotDecision("f=%d travel: in car #%d, taken on the way: driving", ctx.frame, pl.car_id);
        mCarId = pl.car_id;
        BeginStage(ctx, Drive);
        return Running;
    }
    if (mStage == WaitCar)
    {
        ctx.no_evade_frame = ctx.frame; // we stand in the lane on purpose
        // At the kerb: a car that slows or stops next to us is taken; after a while we go on on foot
        if (ctx.frame % 10 == 0)
        {
            s32 wc = 0;
            bool wh = false;
            float wd = 0.0f;
            if (ChooseCar(ctx, &wc, &wh, &wd, 25.0f))
            {
                mCarId = wc;
                BotDecision("f=%d travel: a car at the kerb: #%d%s %.0f tiles away, taking it", ctx.frame, wc, wh ? " (hijack)" : "", wd);
                BeginStage(ctx, GetCar);
                return Running;
            }
        }
        if (ctx.frame - mWaitStart > 420)
        {
            BotDecision("f=%d travel: no car came in 420 frames", ctx.frame);
            mStage = Decide;
        }
        return Running;
    }
    // On foot and a long way to go: a car that stops next to us (traffic at a crossing) or sits
    // parked close by is worth more than the rest of the walk
    // A car standing in the walker's way (traffic stops for a pedestrian in front of it) is the car to take, not to jump over
    // The same while walking to a car that is still far off (GetCar): traffic passes and stops on the way, and a car
    // close by is better than the far one. It has to be clearly nearer, or the choice flips between two cars
    bool far_target = false;
    float target_dist = 0.0f;
    if (mStage == GetCar && !pl.in_car)
    {
        WorldModel::CarInfo cur;
        if (ctx.world->FindCar(mCarId, &cur) && cur.dist > 14.0f)
        {
            far_target = true;
            target_dist = cur.dist;
        }
    }
    bool stopped_near = false;
    for (int k = 0; k < ctx.world->car_count && (mStage == Walk || far_target); k++)
    {
        const WorldModel::CarInfo& sc = ctx.world->cars[k];
        stopped_near = stopped_near || (sc.speed < 0.03f && sc.dist < 3.5f && sc.dist > 0.5f && !sc.police);
    }
    if ((mStage == Walk || far_target) && !mWalkOnly && (ctx.frame % 20 == 0 || (stopped_near && ctx.frame % 5 == 0)) && !pl.in_car && Remaining(*ctx.world) > 25.0f)
    {
        s32 car = 0;
        bool hijack = false;
        float to_car = 0.0f;
        if (ChooseCar(ctx, &car, &hijack, &to_car, 20.0f) && (!far_target || (car != mCarId && CarSwitch_ShouldSwitch(target_dist, to_car, hijack, mCarSwitches))))
        {
            if (far_target)
            {
                mCarSwitches++;
            }
            mCarId = car;
            BotDecision("f=%d travel: %.0f tiles to go; car #%d%s is %.0f tiles away: taking it", ctx.frame, Remaining(*ctx.world), car, hijack ? " (hijack)" : "", to_car);
            BeginStage(ctx, GetCar);
            return Running;
        }
    }
    // On foot with the police closing in (a wanted level and a cop within a few tiles): get away from them
    // (and before taking a car, police that turn up while we walk to it make the grab a theft in front of them)
    if ((mStage == Walk || mStage == GetCar) && !pl.in_car && ctx.frame % 10 == 0 && mAways < 3 && !ctx.world->TimeLimited() &&
        (pl.wanted_stars > 0 && ctx.world->CopsNear(8.0f)))
    {
        // A car close by is the way out (the wanted level is up, so taking one costs nothing more); otherwise run
        s32 escape = 0;
        bool escape_hijack = false;
        float escape_dist = 0.0f;
        if (mStage == Walk && ChooseCar(ctx, &escape, &escape_hijack, &escape_dist, 14.0f))
        {
            mCarId = escape;
            BotDecision("f=%d travel: wanted %d with police close: getting into car #%d%s %.0f tiles away to get away", ctx.frame, pl.wanted_stars, escape, escape_hijack ? " (hijack)" : "", escape_dist);
            BeginStage(ctx, GetCar);
            return Running;
        }
        if (mStage == Walk && Goals_AwayPoint(*ctx.world, 25.0f, &mAwayX, &mAwayY))
        {
            mAways++;
            BotDecision("f=%d travel: wanted %d with police %.0f tiles away: running to (%.0f,%.0f) (%d)", ctx.frame, pl.wanted_stars, 8.0f, mAwayX, mAwayY, mAways);
            BeginStage(ctx, Away);
            return Running;
        }
    }
    // On foot: a gun, health or armour lying close is taken on the way
    if (mStage == Walk && !pl.in_car && ctx.frame % 10 == 0 && Remaining(*ctx.world) > mArrive + 3.0f)
    {
        WorldModel::ObjInfo o;
        if (Goals_WantedPickup(*ctx.world, 5.0f, mGrabBan, mGrabBanCount, &o))
        {
            mGrabId = o.id;
            mGrabX = o.x;
            mGrabY = o.y;
            if (mGrabBanCount < kMaxBan)
            {
                mGrabBan[mGrabBanCount++] = o.id; // once
            }
            BotDecision("f=%d travel: pickup #%d (model %d) is close: taking it on the way", ctx.frame, o.id, o.model);
            BeginStage(ctx, Grab);
            return Running;
        }
    }
    const Status s = mChild->Step(ctx);
    if (s == Running)
    {
        return Running;
    }
    ctx.input = 0;
    switch (mStage)
    {
        case GetCar:
            if (s == Done)
            {
                BeginStage(ctx, Drive);
            }
            else
            {
                BotDecision("f=%d travel: could not get car #%d (%s): %s", ctx.frame, mCarId, mChild->fail_condition, mChild->fail_detail);
                if (mBanCount < kMaxBan)
                {
                    mBan[mBanCount++] = mCarId;
                }
                if (++mCarFailures >= 3)
                {
                    mWalkOnly = true;
                    mWalkOnlyUntil = ctx.frame + 900; // not for good: cars about change
                }
                delete mChild;
                mChild = NULL;
                mStage = Decide;
            }
            return Running;
        case Drive:
            if (s == Failed)
            {
                BotDecision("f=%d travel: the drive failed (%s): %s", ctx.frame, mChild->fail_condition, mChild->fail_detail);
                // thrown out of the car (its owner took it back): get it back, the thief is next to it
                WorldModel::CarInfo lost;
                if (strstr(mChild->fail_detail, "not in a car") != NULL && mReclaims < 4 && mCarId != 0 && ctx.world->FindCar(mCarId, &lost) && lost.dist < 25.0f)
                {
                    mReclaims++;
                    BotDecision("f=%d travel: car #%d is %.1f tiles away: taking it back (%d)", ctx.frame, mCarId, lost.dist, mReclaims);
                    BeginStage(ctx, GetCar);
                    return Running;
                }
                BotDecision("f=%d travel: on foot from here", ctx.frame);
                mWalkOnly = true;
                mWalkOnlyUntil = ctx.frame + 900;
            }
            else if (mKeepCar)
            {
                return Done;
            }
            BeginStage(ctx, Dismount);
            return Running;
        case Dismount:
            if (s == Failed)
            {
                return Fail(mChild->fail_condition, "travel: %s", mChild->fail_detail);
            }
            delete mChild;
            mChild = NULL;
            mStage = Decide;
            // The car got as near as the roads go (water, a wall of buildings): walk on, and take another car on the
            // far side where one can drive the rest. This one is no use for it, so it is not taken again. A drive
            // that failed has already set walk only
            if (mCarId != 0 && mBanCount < kMaxBan)
            {
                mBan[mBanCount++] = mCarId;
            }
            if (Remaining(*ctx.world) <= mArrive)
            {
                return Done;
            }
            return Running;
        case Train:
            if (s == Failed)
            {
                BotDecision("f=%d travel: the train did not work (%s): %s", ctx.frame, mChild->fail_condition, mChild->fail_detail);
                mNoTrain = true;
            }
            delete mChild;
            mChild = NULL;
            mStage = Decide;
            if (Remaining(*ctx.world) <= mArrive)
            {
                return Done;
            }
            mNoTrain = true; // one ride; the rest by car or on foot
            return Running;
        case ToRoad:
            delete mChild;
            mChild = NULL;
            BeginStage(ctx, WaitCar); // at the road: wait for a car
            return Running;
        case Away:
        case Grab:
            delete mChild;
            mChild = NULL;
            mStage = Decide;
            return Running;
        case Walk:
            if (s == Failed)
            {
                return Fail(mChild->fail_condition, "travel: %s", mChild->fail_detail);
            }
            return Done;
        default:
            return Done;
    }
}

// ---------------------------------------------------------------------------------------------
static bool IEqualPrefix(const char* text, const char* name4)
{
    for (int i = 0; i < 4; i++)
    {
        char a = text[i];
        char b = name4[i];
        if (a >= 'A' && a <= 'Z')
        {
            a = static_cast<char>(a + 32);
        }
        if (b >= 'A' && b <= 'Z')
        {
            b = static_cast<char>(b + 32);
        }
        if (a != b || a == 0)
        {
            return false;
        }
    }
    return true;
}

// The first gang named after "kill" / "eliminate" in the brief
s32 Goals_ParseKillHint(const WorldModel& w, const char* brief)
{
    const char* from = NULL;
    for (const char* p = brief; *p && !from; p++)
    {
        if (IEqualPrefix(p, "kill") || IEqualPrefix(p, "elim"))
        {
            from = p;
        }
    }
    if (!from)
    {
        return -1;
    }
    for (const char* p = from; *p; p++)
    {
        for (int g = 0; g < w.gang_count; g++)
        {
            if (IEqualPrefix(p, w.gang_names[g]))
            {
                return w.gang_index[g];
            }
        }
    }
    return -1;
}

KillGangGoal::KillGangGoal(s32 gang_idx, s32 kills, s32 max_frames, s32 owner_gang, s32 target_respect)
    : mGang(gang_idx)
    , mWant(kills)
    , mMaxFrames(max_frames)
    , mStart(0)
    , mKilled(0)
    , mChild(NULL)
    , mHunting(false)
    , mTargetId(0)
    , mSnap(NULL)
    , mBanCount(0)
    , mWander(0)
    , mZone(0)
    , mFailures(0)
    , mChildSince(0)
    , mRunOver(false)
    , mNoCar(false)
    , mCarTries(0)
    , mCarBanCount(0)
    , mOwner(owner_gang)
    , mTargetRespect(target_respect)
    , mFlees(0)
    , mBurnFlees(0)
    , mFleeing(false)
{
}

KillGangGoal::~KillGangGoal()
{
    delete mChild;
    delete mSnap;
}

void KillGangGoal::Start(BotContext& ctx)
{
    mStart = ctx.frame;
    mKilled = 0;
    mBanCount = 0;
    mWander = 0;
    mZone = 0;
    mFailures = 0;
    delete mChild;
    mChild = NULL;
}

bool KillGangGoal::Banned(s32 id) const
{
    for (int i = 0; i < mBanCount; i++)
    {
        if (mBan[i] == id)
        {
            return true;
        }
    }
    return false;
}

void KillGangGoal::NextChild(BotContext& ctx)
{
    const WorldModel& w = *ctx.world;
    const WorldModel::PlayerInfo& pl = w.player;
    // A car that is about to burn (and blow up) next to a player on foot: walk clear of it before anything else
    if (!pl.in_car && mBurnFlees < 3)
    {
        for (int i = 0; i < w.car_count; i++)
        {
            const WorldModel::CarInfo& bc = w.cars[i];
            if (bc.damage >= 25000 && bc.dist < 8.0f)
            {
                float ax = pl.x - bc.x;
                float ay = pl.y - bc.y;
                const float al = sqrtf(ax * ax + ay * ay);
                ax = al > 0.01f ? ax / al : 1.0f;
                ay = al > 0.01f ? ay / al : 0.0f;
                for (int a = 0; a < 8; a++)
                {
                    const float ang = (a % 2 == 0 ? 1 : -1) * (a / 2) * 0.6f;
                    const float cx = pl.x + (ax * cosf(ang) - ay * sinf(ang)) * 14.0f;
                    const float cy = pl.y + (ax * sinf(ang) + ay * cosf(ang)) * 14.0f;
                    if (cx < 3 || cy < 3 || cx > 250 || cy > 250 || WorldModel::TileType(static_cast<int>(cx), static_cast<int>(cy)) == 0 || WorldModel::TileType(static_cast<int>(cx), static_cast<int>(cy)) == 4)
                    {
                        continue;
                    }
                    mBurnFlees++;
                    mHunting = false;
                    mRunOver = false;
                    mChildSince = ctx.frame;
                    BotDecision("f=%d kill_gang: car #%d (damage %d) is about to burn %.1f tiles away: walking clear to (%.0f,%.0f)", ctx.frame, bc.idx, bc.damage, bc.dist, cx, cy);
                    mChild = new TravelGoal(cx, cy, 3.0f, false);
                    BeginChild(ctx, &mSnap, mChild);
                    return;
                }
            }
        }
    }
    // The police are after us: no more killing here, get away from them first
    bool cop_close = false;
    for (int i = 0; i < w.ped_count; i++)
    {
        cop_close = cop_close || (w.peds[i].cop && !w.peds[i].dead && w.peds[i].dist < 14.0f);
    }
    // one star with a cop close by ends in an arrest, as the cops walk up to a player in sight
    if (((pl.wanted_stars >= 2 && w.cops_total > 0) || (pl.wanted_stars >= 1 && cop_close)) && mFlees < 10)
    {
        const WorldModel::PedInfo* cop = NULL;
        for (int i = 0; i < w.ped_count && !cop; i++)
        {
            if (w.peds[i].cop && !w.peds[i].dead)
            {
                cop = &w.peds[i];
            }
        }
        float ax = cop ? pl.x - cop->x : 1.0f;
        float ay = cop ? pl.y - cop->y : 0.0f;
        const float al = sqrtf(ax * ax + ay * ay);
        ax = al > 0.01f ? ax / al : 1.0f;
        ay = al > 0.01f ? ay / al : 0.0f;
        for (int a = 0; a < 8; a++)
        {
            const float ang = (a % 2 == 0 ? 1 : -1) * (a / 2) * 0.6f;
            const float cx = pl.x + (ax * cosf(ang) - ay * sinf(ang)) * 40.0f;
            const float cy = pl.y + (ax * sinf(ang) + ay * cosf(ang)) * 40.0f;
            if (cx < 3 || cy < 3 || cx > 250 || cy > 250 || WorldModel::TileType(static_cast<int>(cx), static_cast<int>(cy)) == 0 || WorldModel::TileType(static_cast<int>(cx), static_cast<int>(cy)) == 4)
            {
                continue;
            }
            mFlees++;
            mHunting = false;
            mRunOver = false;
            mChildSince = ctx.frame;
            BotDecision("f=%d kill_gang: wanted level %d, %d cops about: getting away to (%.0f,%.0f) (%d)", ctx.frame, pl.wanted_stars, w.cops_total, cx, cy, mFlees);
            mFleeing = true;
            mChild = new TravelGoal(cx, cy, 6.0f, true);
            BeginChild(ctx, &mSnap, mChild);
            return;
        }
    }
    // Without a gun (it is lost when the player dies) the gang is no use punched one by one: a gun lying within a short
    // walk is taken, or a car, which is a weapon too
    const bool unarmed = !pl.in_car && (pl.weapon == 0 || pl.ammo <= 0);
    // On foot, a gun, health or armour lying close (dropped by the ones just killed) is worth a few steps
    if (!pl.in_car && mBanCount < kMaxBan)
    {
        WorldModel::ObjInfo o;
        if (Goals_WantedPickup(w, unarmed ? 25.0f : 8.0f, mBan, mBanCount, &o))
        {
            mBan[mBanCount++] = o.id; // once
            mHunting = false;
            mRunOver = false;
            mChildSince = ctx.frame;
            BotDecision("f=%d kill_gang: picking up #%d (model %d) close by", ctx.frame, o.id, o.model);
            mChild = new PickUpAction(o.id, o.x, o.y);
            BeginChild(ctx, &mSnap, mChild);
            return;
        }
    }
    for (int i = 0; i < w.ped_count; i++)
    {
        const WorldModel::PedInfo& p = w.peds[i];
        // unarmed on foot with a car to try for: not a fight with the fists, the car code below goes first
        if (unarmed && !mNoCar && mCarTries < 4 && p.gang_idx == mGang)
        {
            continue;
        }
        if (p.gang_idx == mGang && !p.dead && !p.cop && !p.in_car && p.dist < 35.0f && !Banned(p.id))
        {
            mHunting = true;
            mTargetId = p.id;
            mRunOver = false;
            if (pl.in_car && !mNoCar)
            {
                // in a car: run them over, one after the other, as long as any are in sight
                mRunOver = true;
                BotDecision("f=%d kill_gang: running down gang %d peds from a car (nearest %.0f tiles, %d of %d killed)", ctx.frame, mGang, p.dist, mKilled, mWant);
                mChild = new RunOverAction(mGang, mWant - mKilled);
            }
            else
            {
                BotDecision("f=%d kill_gang: hunting ped #%d of gang %d at %.0f tiles (%d of %d killed)", ctx.frame, p.id, mGang, p.dist, mKilled, mWant);
                mChild = new KillPedAction(p.id, p.x, p.y);
            }
            BeginChild(ctx, &mSnap, mChild);
            return;
        }
    }
    // Nobody in sight: walk about the gang's territory, nearest zone first
    mHunting = false;
    mRunOver = false;
    mChildSince = ctx.frame;
    // Nobody in sight and no car: a car makes the rounds of the territory (and the kills) much faster
    if (!pl.in_car && !mNoCar && mCarTries < 4)
    {
        CarPick ban;
        ban.has_target = false;
        ban.banned_count = 0;
        for (int i = 0; i < mCarBanCount && i < 8; i++)
        {
            ban.banned[ban.banned_count++] = mCarBan[i];
        }
        s32 car = 0;
        bool hijack = false;
        float path = 0.0f;
        if (Cars_Pick(ctx, ban, 30.0f, true, &car, &hijack, &path))
        {
            mCarTries++;
            if (mCarBanCount < 8)
            {
                mCarBan[mCarBanCount++] = car;
            }
            BotDecision("f=%d kill_gang: taking car #%d%s %.0f tiles away to cover the territory", ctx.frame, car, hijack ? " (hijack)" : "", path);
            mChild = new EnterCarAction(car, hijack);
            BeginChild(ctx, &mSnap, mChild);
            return;
        }
    }
    const WorldModel::GangZone* best[WorldModel::kMaxGangZones];
    float best_d[WorldModel::kMaxGangZones];
    int n = 0;
    for (int i = 0; i < w.gang_zone_count; i++)
    {
        const WorldModel::GangZone& z = w.gang_zones[i];
        if (z.gang_idx != mGang)
        {
            continue;
        }
        const float dx = z.x + z.w * 0.5f - pl.x;
        const float dy = z.y + z.h * 0.5f - pl.y;
        const float d = dx * dx + dy * dy;
        int pos = n++;
        while (pos > 0 && best_d[pos - 1] > d)
        {
            best[pos] = best[pos - 1];
            best_d[pos] = best_d[pos - 1];
            pos--;
        }
        best[pos] = &z;
        best_d[pos] = d;
    }
    if (n == 0)
    {
        mFailures++;
        return;
    }
    const WorldModel::GangZone& z = *best[mZone % n];
    // a random reachable tile of the zone
    for (int attempt = 0; attempt < 24; attempt++)
    {
        const s32 tx = z.x + static_cast<s32>(ctx.rng->Below(static_cast<u32>(z.w > 0 ? z.w : 1)));
        const s32 ty = z.y + static_cast<s32>(ctx.rng->Below(static_cast<u32>(z.h > 0 ? z.h : 1)));
        if (WorldModel::TileType(tx, ty) == 0 || WorldModel::TileType(tx, ty) == 4)
        {
            continue;
        }
        if (ctx.nav->FindPath(static_cast<u8>(pl.x), static_cast<u8>(pl.y), static_cast<u8>(pl.z + 0.5f), static_cast<u8>(tx), static_cast<u8>(ty)) && !ctx.nav->Approximate())
        {
            BotDecision("f=%d kill_gang: nobody of gang %d in sight; walking to (%d,%d) in its zone %d", ctx.frame, mGang, tx, ty, mZone % n);
            // by car when the zone is far, and stay in it: a gang is run down faster than it is chased on foot
            mChild = new TravelGoal(tx + 0.5f, ty + 0.5f, 2.0f, !mNoCar);
            BeginChild(ctx, &mSnap, mChild);
            if (++mWander % 4 == 0)
            {
                mZone++;
            }
            return;
        }
    }
    mZone++;
    mFailures++;
}

Action::Status KillGangGoal::Step(BotContext& ctx)
{
    const WorldModel::PlayerInfo& pl = ctx.world->player;
    ctx.input = 0;
    if (!pl.valid)
    {
        return Running;
    }
    if (mKilled >= mWant)
    {
        return Done;
    }
    if (mOwner >= 0)
    {
        for (int g = 0; g < ctx.world->gang_count; g++)
        {
            if (ctx.world->gang_index[g] == mOwner && ctx.world->respect[g] >= mTargetRespect)
            {
                BotDecision("f=%d kill_gang: respect of gang %d is %d (wanted %d) after %d kills", ctx.frame, mOwner, ctx.world->respect[g], mTargetRespect, mKilled);
                return Done;
            }
        }
    }
    if (ctx.frame - mStart > mMaxFrames)
    {
        return Fail("no_progress", "kill_gang: %d of %d gang %d peds killed in %d frames", mKilled, mWant, mGang, mMaxFrames);
    }
    if (mFailures > 30)
    {
        return Fail("no_plan", "kill_gang: nowhere to look for gang %d (%d of %d killed)", mGang, mKilled, mWant);
    }
    if (!mChild)
    {
        NextChild(ctx);
        return Running;
    }
    // A zone that cannot be reached in a reasonable time: try the next one
    if (!mHunting && ctx.frame - mChildSince > 2500)
    {
        BotDecision("f=%d kill_gang: could not reach the zone in 2500 frames; trying another", ctx.frame);
        delete mChild;
        mChild = NULL;
        mZone++;
        mFailures++;
        return Running;
    }
    // A gang ped came into sight while walking: switch to hunting it
    if (!mHunting && !mFleeing && ctx.frame % 10 == 0)
    {
        for (int i = 0; i < ctx.world->ped_count; i++)
        {
            const WorldModel::PedInfo& p = ctx.world->peds[i];
            if (p.gang_idx == mGang && !p.dead && !p.cop && !p.in_car && p.dist < 20.0f && !Banned(p.id))
            {
                delete mChild;
                mChild = NULL;
                NextChild(ctx);
                return Running;
            }
        }
    }
    const Status s = mChild->Step(ctx);
    if (s == Running)
    {
        return Running;
    }
    if (mHunting && mRunOver)
    {
        mKilled += static_cast<RunOverAction*>(mChild)->Kills();
        if (s == Failed && strcmp(mChild->fail_condition, "car_damaged") == 0)
        {
            mNoCar = true; // the car is spent: finish on foot (KillPed gets out first)
        }
        else if (s == Failed && strstr(mChild->fail_detail, "not in a car") != NULL && mCarBanCount > 0)
        {
            mCarBanCount--; // thrown out: its owner took it back; it may be taken again (the ones next to it die first)
        }
        BotDecision("f=%d kill_gang: run over ended (%s), %d of %d killed", ctx.frame, s == Done ? "done" : mChild->fail_detail, mKilled, mWant);
    }
    else if (mHunting)
    {
        if (s == Done)
        {
            mKilled++;
        }
        else if (mBanCount < kMaxBan)
        {
            mBan[mBanCount++] = mTargetId;
        }
    }
    else if (s == Failed)
    {
        mFailures++;
    }
    delete mChild;
    mChild = NULL;
    mFleeing = false;
    ctx.input = 0;
    return Running;
}

// ---------------------------------------------------------------------------------------------
DestroyCarGoal::DestroyCarGoal(s32 car_id)
    : mCarId(car_id)
    , mStage(Prep)
    , mChild(NULL)
    , mSnap(NULL)
    , mTriedShoot(false)
    , mRamTries(0)
    , mPickId(0)
    , mPickFailures(0)
    , mBanCount(0)
    , mStart(0)
    , mReclaims(0)
    , mStageStart(0)
    , mApproaches(0)
    , mFlees(0)
    , mFleeX(0.0f)
    , mFleeY(0.0f)
{
}

DestroyCarGoal::~DestroyCarGoal()
{
    delete mChild;
    delete mSnap;
}

void DestroyCarGoal::Start(BotContext& ctx)
{
    mStart = ctx.frame;
    mTriedShoot = false;
    mRamTries = 0;
    mPickFailures = 0;
    mBanCount = 0;
    BeginStage(ctx, Prep);
}

void DestroyCarGoal::BeginStage(BotContext& ctx, Stage st)
{
    delete mChild;
    mChild = NULL;
    mStage = st;
    mStageStart = ctx.frame;
    switch (st)
    {
        case Prep:
            mChild = new PrepareGoal(Weapons::VsCar, 45.0f, 900);
            break;
        case Shoot:
            mTriedShoot = true;
            mChild = new DestroyCarAction(mCarId);
            break;
        case GetCar:
        {
            WorldModel::CarInfo c;
            const bool hijack = ctx.world->FindCar(mPickId, &c) && c.has_driver;
            mChild = new EnterCarAction(mPickId, hijack);
            break;
        }
        case Ram:
        {
            RamCarAction* ram = new RamCarAction(mCarId);
            s32 score = 0;
            Weapons::BestHeld(*ctx.world, Weapons::VsCar, &score);
            ram->SetStay(score < 6); // nothing worth getting out for: keep hitting it
            mChild = ram;
            break;
        }
        case Leave:
            mChild = new ExitCarAction();
            break;
        case Finish:
            mTriedShoot = true;
            mChild = new DestroyCarAction(mCarId);
            break;
        case Flee:
            mChild = new WalkAction(mFleeX, mFleeY, false, 4.0f);
            break;
        case Approach:
        {
            WorldModel::CarInfo t;
            const float tx = ctx.world->FindCar(mCarId, &t) ? t.x : ctx.world->player.x;
            const float ty = ctx.world->FindCar(mCarId, &t) ? t.y : ctx.world->player.y;
            mChild = new TravelGoal(tx, ty, 25.0f, true); // by car, train or on foot, whichever is quickest
            break;
        }
    }
    BeginChild(ctx, &mSnap, mChild);
}

// Shoot it or ram it, whichever is cheaper (frames, roughly)
void DestroyCarGoal::Decide(BotContext& ctx)
{
    const WorldModel& w = *ctx.world;
    const WorldModel::PlayerInfo& pl = w.player;
    WorldModel::CarInfo t;
    if (!w.FindCar(mCarId, &t))
    {
        delete mChild; // gone: Step reports Done
        mChild = NULL;
        return;
    }
    if (t.dist > 45.0f && mApproaches < 3)
    {
        // a long way off: get near first (in a car if one can be had), the choice of weapon waits
        mApproaches++;
        BotDecision("f=%d destroy_car: car #%d is %.0f tiles away: travelling to it first (%d)", ctx.frame, mCarId, t.dist, mApproaches);
        BeginStage(ctx, Approach);
        return;
    }
    s32 score = 0;
    Weapons::BestHeld(w, Weapons::VsCar, &score);
    float shoot_cost = 1e9f;
    // A car that is proof against bullets and rockets (the mission truck is) takes nothing from a gun: shooting only
    // draws the police
    const bool shootable = !(t.bulletproof && t.rocketproof);
    if (!mTriedShoot && shootable)
    {
        shoot_cost = t.dist * 22.0f + 200.0f;
        if (score >= 8 && w.weapon_ammo[2] >= 10 && t.dist < 30.0f && t.speed < 0.05f)
        {
            // a rocket held and the car close and standing still: one that reaches it finishes it, which beats ramming it about
            // (and the damage to our own car). A car that is driving off cannot be chased on foot, that is for the ram
            shoot_cost = t.dist * 6.0f + 150.0f;
        }
        else if (t.speed > 0.08f)
        {
            shoot_cost += 2500.0f; // it will not stand still for the rocket
        }
        if (score < 6)
        {
            shoot_cost += 4000.0f; // a pistol takes a minute to wreck a car
        }
        else if (score < 8)
        {
            shoot_cost += 600.0f;
        }
    }
    float ram_cost = 1e9f;
    s32 pick = 0;
    bool hijack = false;
    float path = 0.0f;
    if (mRamTries < 14)
    {
        if (pl.in_car)
        {
            ram_cost = t.dist * 7.0f + 500.0f;
            pick = pl.car_id;
        }
        else
        {
            CarPick ban;
            ban.has_target = false;
            ban.banned_count = mBanCount;
            for (int i = 0; i < mBanCount; i++)
            {
                ban.banned[i] = mBan[i];
            }
            if (Cars_Pick(ctx, ban, 60.0f, true, &pick, &hijack, &path))
            {
                ram_cost = path * 22.0f + 150.0f + t.dist * 7.0f + 500.0f;
            }
        }
        if (t.damage >= 6000)
        {
            ram_cost *= 0.7f; // already hurt: a few more knocks
        }
    }
    BotDecision("f=%d destroy_car: car #%d model %d %.0f tiles away (speed %.2f, damage %d, proof: bullets %d rockets %d flames %d); shoot %.0f (weapon score %d) against ram %.0f%s", ctx.frame, mCarId, t.model, t.dist, t.speed, t.damage, t.bulletproof, t.rocketproof, t.flameproof, shoot_cost, score, ram_cost,
                pl.in_car ? " (already in a car)" : "");
    if (shoot_cost >= 1e9f && ram_cost >= 1e9f)
    {
        delete mChild; // nothing left to try: Step fails
        mChild = NULL;
        return;
    }
    if (shoot_cost <= ram_cost)
    {
        BeginStage(ctx, pl.in_car ? Leave : Shoot);
        return;
    }
    mRamTries++;
    if (pl.in_car)
    {
        mPickId = pl.car_id; // remembered so that being thrown out can be answered by going back for it
        BeginStage(ctx, Ram);
    }
    else
    {
        mPickId = pick;
        BeginStage(ctx, GetCar);
    }
}

void DestroyCarGoal::Next(BotContext& ctx, bool ok)
{
    switch (mStage)
    {
        case Flee:
            BeginStage(ctx, Finish); // away from the police: the next shot
            break;
        case Prep:
        case Approach:
            Decide(ctx);
            break;
        case Shoot:
        case Finish:
            // Done is reported by Step; a failed shot tries the other way
            Decide(ctx);
            break;
        case GetCar:
            if (ok)
            {
                BeginStage(ctx, Ram);
            }
            else
            {
                if (mBanCount < 8)
                {
                    mBan[mBanCount++] = mPickId;
                }
                mPickFailures++;
                Decide(ctx);
            }
            break;
        case Ram:
        {
            // Thrown out of the car (its owner took it back): go back for it while it is near, rather than walk
            WorldModel::CarInfo lost;
            if (!ok && strstr(mChild->fail_detail, "not in a car") != NULL && mReclaims < 4 && mPickId != 0 && ctx.world->FindCar(mPickId, &lost) && lost.dist < 25.0f)
            {
                mReclaims++;
                BotDecision("f=%d destroy_car: thrown out of car #%d, %.1f tiles away: taking it back (%d)", ctx.frame, mPickId, lost.dist, mReclaims);
                BeginStage(ctx, GetCar);
                break;
            }
            // Ran its course (target stopped / burning / gone) or our car is nearly dead: get out
            BeginStage(ctx, Leave);
            break;
        }
        case Leave:
        {
            // on foot: the target is stopped or burning, or our car is spent. Finish it with the gun
            // if it is near, otherwise decide again
            WorldModel::CarInfo t;
            if (ctx.world->FindCar(mCarId, &t) && t.damage < 32000 && t.dist < 30.0f && !mTriedShoot)
            {
                BeginStage(ctx, Finish);
            }
            else
            {
                Decide(ctx);
            }
            break;
        }
    }
}

Action::Status DestroyCarGoal::Step(BotContext& ctx)
{
    ctx.input = 0;
    if (!ctx.world->player.valid)
    {
        return Running;
    }
    WorldModel::CarInfo t;
    if (!ctx.world->FindCar(mCarId, &t) || t.damage >= 32000)
    {
        BotDecision("f=%d destroy_car: car #%d destroyed after %d frames", ctx.frame, mCarId, ctx.frame - mStart);
        return Done;
    }
    if (ctx.frame - mStart > 15000)
    {
        return Fail("no_progress", "destroy_car: car #%d still alive after 15000 frames", mCarId);
    }
    if (!mChild)
    {
        return Fail("no_plan", "destroy_car: no way left to destroy car #%d (shoot tried %d, ram tries %d)", mCarId, mTriedShoot, mRamTries);
    }
    // On foot with the police beside the player after a shot (the rocket draws them): get away from them first. Every
    // death so far was cops punching the player to death while the action went on with the next shot
    if ((mStage == Shoot || mStage == Finish) && !ctx.world->player.in_car && ctx.world->player.wanted_stars > 0 && ctx.world->CopsNear(5.0f) && mFlees < 4 &&
        Goals_AwayPoint(*ctx.world, 22.0f, &mFleeX, &mFleeY))
    {
        mFlees++;
        BotDecision("f=%d destroy_car: wanted %d with police beside us: moving away to (%.0f,%.0f) before the next shot (%d)", ctx.frame, ctx.world->player.wanted_stars, mFleeX, mFleeY, mFlees);
        BeginStage(ctx, Flee);
        return Running;
    }
    if ((mStage == Shoot || mStage == Finish) && ctx.frame - mStageStart > 900)
    {
        // a target that drives off cannot be chased on foot
        BotDecision("f=%d destroy_car: 900 frames of shooting at car #%d (damage %d) got nowhere: trying the other way", ctx.frame, mCarId, t.damage);
        ctx.input = 0;
        Next(ctx, false);
        return Running;
    }
    const Status s = mChild->Step(ctx);
    if (s == Running)
    {
        return Running;
    }
    ctx.input = 0;
    if (s == Failed)
    {
        BotDecision("f=%d destroy_car: stage %d (%s) failed (%s): %s", ctx.frame, static_cast<int>(mStage), mChild->Name(), mChild->fail_condition, mChild->fail_detail);
    }
    Next(ctx, s == Done);
    return Running;
}

// ---------------------------------------------------------------------------------------------
MissionGoal::MissionGoal(const WorldModel::ObjInfo& phone)
    : mPhone(phone)
    , mStage(Prep)
    , mChild(NULL)
    , mSnap(NULL)
    , mRefused(false)
{
    mRefusalBrief[0] = 0;
}

MissionGoal::~MissionGoal()
{
    delete mChild;
    delete mSnap;
}

void MissionGoal::BeginStage(BotContext& ctx, Stage st)
{
    delete mChild;
    mStage = st;
    mChild = NULL;
    switch (st)
    {
        case Prep:
            mChild = new PrepareGoal(Weapons::VsPed, 40.0f, 1500);
            break;
        case Travel:
            mChild = new TravelGoal(mPhone.x, mPhone.y, 8.0f);
            break;
        case Phone:
            mChild = new PhoneAction(mPhone.id, mPhone.x, mPhone.y);
            break;
        case Play:
            mChild = new MissionAction();
            break;
        default:
            return;
    }
    BeginChild(ctx, &mSnap, mChild);
}

void MissionGoal::Start(BotContext& ctx)
{
    mRefused = false;
    BeginStage(ctx, Prep);
}

Action::Status MissionGoal::Step(BotContext& ctx)
{
    ctx.input = 0;
    if (!mChild)
    {
        return Done;
    }
    const Status s = mChild->Step(ctx);
    if (s == Running)
    {
        return Running;
    }
    if (s == Failed)
    {
        if (mStage == Phone)
        {
            PhoneAction* phone = static_cast<PhoneAction*>(mChild);
            mRefused = phone->Refused();
            strncpy(mRefusalBrief, phone->RefusalBrief(), sizeof(mRefusalBrief) - 1);
            mRefusalBrief[sizeof(mRefusalBrief) - 1] = 0;
        }
        return Fail(mChild->fail_condition, "phone #%d stage %d (%s): %s", mPhone.id, static_cast<int>(mStage), mChild->Name(), mChild->fail_detail);
    }
    ctx.input = 0;
    if (mStage == Prep)
    {
        BeginStage(ctx, Travel);
    }
    else if (mStage == Travel)
    {
        BeginStage(ctx, Phone);
    }
    else if (mStage == Phone)
    {
        BeginStage(ctx, Play);
    }
    else
    {
        mStage = Over;
        return Done;
    }
    return Running;
}

// ---------------------------------------------------------------------------------------------
MissionsGoal::MissionsGoal(s32 want_missions)
    : mWant(want_missions)
    , mPassed(0)
    , mStart(0)
    , mChild(NULL)
    , mRaise(NULL)
    , mRaisePhone(0)
    , mPreferPhone(0)
    , mFavourGang(-2)
    , mSnap(NULL)
    , mMemoryCount(0)
    , mFailures(0)
    , mRaiseFailures(0)
{
}

MissionsGoal::~MissionsGoal()
{
    delete mChild;
    delete mRaise;
    delete mSnap;
}

void MissionsGoal::Start(BotContext& ctx)
{
    mStart = ctx.frame;
}

// The first gang whose name appears in the text
s32 Goals_FirstGang(const WorldModel& w, const char* text)
{
    for (const char* p = text; *p; p++)
    {
        for (int g = 0; g < w.gang_count; g++)
        {
            if (IEqualPrefix(p, w.gang_names[g]))
            {
                return w.gang_index[g];
            }
        }
    }
    return -1;
}

// The gang whose territory holds the point, -1 if none
static s32 OwnerAt(const WorldModel& w, float x, float y)
{
    for (int z = 0; z < w.gang_zone_count; z++)
    {
        const WorldModel::GangZone& gz = w.gang_zones[z];
        if (x >= gz.x && x < gz.x + gz.w && y >= gz.y && y < gz.y + gz.h)
        {
            return gz.gang_idx;
        }
    }
    return -1;
}

static s32 RespectOf(const WorldModel& w, s32 gang_idx)
{
    for (int g = 0; g < w.gang_count; g++)
    {
        if (w.gang_index[g] == gang_idx)
        {
            return w.respect[g];
        }
    }
    return 0;
}

MissionsGoal::PhoneMemory* MissionsGoal::Memory(s32 id, bool create)
{
    for (int i = 0; i < mMemoryCount; i++)
    {
        if (mMemory[i].id == id)
        {
            return &mMemory[i];
        }
    }
    if (!create || mMemoryCount == kMaxPhones)
    {
        return NULL;
    }
    PhoneMemory* m = &mMemory[mMemoryCount++];
    memset(m, 0, sizeof(*m));
    m->id = id;
    m->hint_gang = -1;
    return m;
}

// Has the respect of any gang changed since the vector was taken?
static bool RespectMoved(const WorldModel& w, const s32* before)
{
    for (int i = 0; i < w.gang_count; i++)
    {
        if (w.respect[i] != before[i])
        {
            return true;
        }
    }
    return false;
}

s32 MissionsGoal::RespectSum(const WorldModel& w) const
{
    s32 sum = 0;
    for (int i = 0; i < w.gang_count; i++)
    {
        sum += w.respect[i];
    }
    return sum;
}

// The cheapest phone not yet passed: tier first (green, amber, red), then distance. A phone that
// refused is left alone until the respect of some gang has changed since.
bool MissionsGoal::ChoosePhone(BotContext& ctx, WorldModel::ObjInfo* out)
{
    const WorldModel& w = *ctx.world;
    if (mFavourGang == -2 && w.gang_count > 0)
    {
        mFavourGang = w.gang_index[ctx.rng->Below(static_cast<u32>(w.gang_count))];
        BotDecision("f=%d missions: favourite gang this run: %d", ctx.frame, mFavourGang);
    }
    float best = 1e9f;
    bool found = false;
    for (int i = 0; i < w.phone_count; i++)
    {
        const WorldModel::ObjInfo& p = w.phones[i];
        const PhoneMemory* m = Memory(p.id, false);
        float penalty = 0.0f;
        if (m)
        {
            if (m->passed || m->tries >= 2)
            {
                continue;
            }
            if (m->refused && !RespectMoved(w, m->respect_at_refusal_v))
            {
                continue;
            }
            penalty = static_cast<float>(m->tries) * 600.0f;
        }
        const float dx = p.x - w.player.x;
        const float dy = p.y - w.player.y;
        float cost = sqrtf(dx * dx + dy * dy) + static_cast<float>(Goals_PhoneTier(p.model)) * 400.0f + penalty;
        // Phones of a gang that has already refused for want of respect, while the respect is still short of
        // what that took, are not worth the walk yet: do the other gangs' phones first
        const s32 owner = OwnerAt(w, p.x, p.y);
        cost += static_cast<float>(ctx.rng->Below(300)); // not always the same order
        if (owner >= 0 && owner == mFavourGang)
        {
            cost -= 450.0f;
        }
        if (owner >= 0)
        {
            for (int k = 0; k < mMemoryCount; k++)
            {
                if (mMemory[k].refused && mMemory[k].owner_gang == owner && RespectOf(w, owner) < mMemory[k].target_respect)
                {
                    cost += 6000.0f;
                    break;
                }
            }
        }
        if (p.id == mPreferPhone)
        {
            cost -= 10000.0f; // we just raised the respect it wanted: go back to it
        }
        if (cost < best)
        {
            best = cost;
            *out = p;
            found = true;
        }
    }
    return found;
}

Action::Status MissionsGoal::Step(BotContext& ctx)
{
    ctx.input = 0;
    if (!ctx.world->player.valid)
    {
        return Running;
    }
    if (mRaise)
    {
        const Status s = mRaise->Step(ctx);
        if (s == Running)
        {
            return Running;
        }
        if (s == Done)
        {
            BotDecision("f=%d missions: respect raised for phone #%d: respect now %d %d %d", ctx.frame, mRaisePhone, ctx.world->respect[0], ctx.world->respect[1], ctx.world->respect[2]);
            mPreferPhone = mRaisePhone;
            PhoneMemory* rm = Memory(mRaisePhone, false);

        }
        else
        {
            mRaiseFailures++;
            BotDecision("f=%d missions: raising respect for phone #%d failed (%s): %s", ctx.frame, mRaisePhone, mRaise->fail_condition, mRaise->fail_detail);
        }
        delete mRaise;
        mRaise = NULL;
        // whatever the kills did, the phone may be asked again; if the respect did not move it is not
        ctx.input = 0;
        if (mRaiseFailures > 6)
        {
            return Fail("action_failed", "missions: raising respect failed %d times (%d of %d missions passed)", mRaiseFailures, mPassed, mWant);
        }
        return Running;
    }
    if (mChild)
    {
        const Status s = mChild->Step(ctx);
        if (s == Running)
        {
            return Running;
        }
        PhoneMemory* m = Memory(mChild->PhoneId(), true);
        const bool refused = s == Failed && mChild->Refused();
        if (s == Done)
        {
            mPassed++;
            if (m)
            {
                m->passed = true;
            }
            BotDecision("f=%d missions: phone #%d done, %d of %d passed, cash %d", ctx.frame, mChild->PhoneId(), mPassed, mWant, ctx.world->player.cash);
        }
        else if (refused)
        {
            // Nothing happened in the world but a brief: stay where we are, and do what it says
            BotDecision("f=%d missions: phone #%d refused: %s", ctx.frame, mChild->PhoneId(), mChild->RefusalBrief());
            if (m)
            {
                m->refused = true;
                for (int r = 0; r < WorldModel::kMaxGangs; r++)
                {
                    m->respect_at_refusal_v[r] = ctx.world->respect[r];
                }
                // Whom to kill: the brief says so when it can; otherwise the gang whose death pleases the
                // owner of the phone most (the game's own kill reaction table)
                const WorldModel& w = *ctx.world;
                s32 owner = -1;
                for (int z = 0; z < w.gang_zone_count && owner < 0; z++)
                {
                    const WorldModel::GangZone& gz = w.gang_zones[z];
                    if (mChild->PhoneObj().x >= gz.x && mChild->PhoneObj().x < gz.x + gz.w && mChild->PhoneObj().y >= gz.y && mChild->PhoneObj().y < gz.y + gz.h)
                    {
                        owner = gz.gang_idx;
                    }
                }
                if (owner < 0)
                {
                    owner = Goals_FirstGang(w, mChild->RefusalBrief());
                }
                m->hint_gang = Goals_ParseKillHint(w, mChild->RefusalBrief());
                s32 best_reaction = 0;
                if (m->hint_gang < 0 && owner >= 0)
                {
                    for (int j = 0; j < WorldModel::kMaxGangs; j++)
                    {
                        if (j != owner && w.kill_reaction[j][owner] > best_reaction)
                        {
                            best_reaction = w.kill_reaction[j][owner];
                            m->hint_gang = j;
                        }
                    }
                    // any gang nearly as pleasing will do: pick one at random
                    s32 pool[WorldModel::kMaxGangs];
                    s32 pool_n = 0;
                    for (int j = 0; j < WorldModel::kMaxGangs; j++)
                    {
                        if (j != owner && best_reaction > 0 && w.kill_reaction[j][owner] * 4 >= best_reaction * 3)
                        {
                            pool[pool_n++] = j;
                        }
                    }
                    if (pool_n > 1)
                    {
                        m->hint_gang = pool[ctx.rng->Below(static_cast<u32>(pool_n))];
                    }
                }
                if (m->hint_gang >= 0)
                {
                    // Kill until the owner's respect reaches a target: 30 above where it is, or 25 above the last target
                    // (a refusal at 46 means the threshold is higher than that)
                    s32 now_respect = 0;
                    for (int g = 0; g < w.gang_count; g++)
                    {
                        if (w.gang_index[g] == owner)
                        {
                            now_respect = w.respect[g];
                        }
                    }
                    m->owner_gang = owner;
                    if (now_respect >= 60 || m->kills_asked >= 2)
                    {
                        // refused at high respect, or after two rounds of killing: respect is not what it wants
                        BotDecision("f=%d missions: phone #%d still refuses at respect %d after %d rounds: giving it up for now", ctx.frame, m->id, now_respect, m->kills_asked);
                        m->tries = 2;
                        delete mChild;
                        mChild = NULL;
                        return Running;
                    }
                    m->kills_asked++; // rounds of raising respect for this phone
                    m->target_respect = (m->target_respect > now_respect ? m->target_respect : now_respect) + 30;
                    if (m->target_respect > 100)
                    {
                        m->target_respect = 100;
                    }
                    const s32 asked = 60;
                    BotDecision("f=%d missions: phone owner gang %d, kill gang %d (one kill changes the owner's respect by %d, the victim's by -%d)", ctx.frame, owner, m->hint_gang,
                                owner >= 0 ? w.kill_reaction[m->hint_gang][owner] : 0, w.kill_respect_change[m->hint_gang]);
                    mRaisePhone = m->id;
                    SequenceGoal* seq = new SequenceGoal("raise_respect");
                    seq->Add(new PrepareGoal(Weapons::VsPed, 40.0f, 1500), true);
                    seq->Add(new KillGangGoal(m->hint_gang, asked, 9000, owner, m->target_respect), false);
                    mRaise = seq;
                    BotDecision("f=%d missions: raising respect: kill gang %d until gang %d has respect %d (now %d)", ctx.frame, m->hint_gang, owner, m->target_respect, now_respect);
                    BeginChild(ctx, &mSnap, mRaise);
                }
            }
        }
        else
        {
            mFailures++;
            BotDecision("f=%d missions: phone #%d failed (%s): %s", ctx.frame, mChild->PhoneId(), mChild->fail_condition, mChild->fail_detail);
            // the attempt is undone: the world goes back to before it
            if (mSnap)
            {
                mSnap->Restore();
                ctx.world->Observe();
            }
            if (m)
            {
                m->tries++;
            }
        }
        delete mChild;
        mChild = NULL;
        ctx.input = 0;
        if (mPassed >= mWant)
        {
            return Done;
        }
        if (mFailures > 16)
        {
            return Fail("action_failed", "missions: %d failed attempts, %d of %d missions passed", mFailures, mPassed, mWant);
        }
        return Running;
    }
    if (mPassed >= mWant)
    {
        return Done;
    }
    WorldModel::ObjInfo phone;
    if (!ChoosePhone(ctx, &phone))
    {
        return Fail("no_plan", "missions: no phone left to try (%d of %d passed, %d failed attempts)", mPassed, mWant, mFailures);
    }
    BotDecision("f=%d missions: next phone #%d model %d (tier %d) at (%.0f,%.0f), I am at (%.0f,%.0f)", ctx.frame, phone.id, phone.model, Goals_PhoneTier(phone.model), phone.x, phone.y, ctx.world->player.x, ctx.world->player.y);
    mPreferPhone = 0;
    mChild = new MissionGoal(phone);
    BeginChild(ctx, &mSnap, mChild);
    return Running;
}
