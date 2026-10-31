// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO kFpZero_706EB8
// This TU's copy of the Fix16_Rect::ComputeCollisionPrism half height (see Fix16_Rect.hpp)
#define FIX16_RECT_HALF_HEIGHT kFpOneEighth_706CC8
#include "PlayerCrimeStats.hpp"
#include "explosion_type.hpp"
#include "ped_death_cause.hpp"
#include "Function.hpp"
#include "fix16.hpp"
EXTERN_GLOBAL(Fix16, kFpOneEighth_706CC8);

#include "Weapon.hpp"
#include "CarPhysics.hpp"
#include "Object_3C.hpp"
#include "Object_5C.hpp"
#include "Object_8.hpp"
#include "ParticleManager.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "CrimeReportQueue.hpp"
#include "WeaponManager.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "map_0x370.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "CollisionSoundQueue.hpp"
#include "Police.hpp"
#include "Car.hpp"
#include "PlayerScoreTracker.hpp"
#include "Fix16_Rect.hpp"
#include "rng.hpp"
#include "ScriptManager.hpp"
#include "SpriteGrid.hpp"

DEFINE_GLOBAL_INIT(Fix16, kFP16Quarter_706CF4, Fix16(0x1000, 0), 0x706CF4);
DEFINE_GLOBAL_INIT(Fix16, kFP16Two_706EC0, Fix16(2), 0x706EC0);
DEFINE_STATIC_MEMBER(bool, Weapon, sbAllowFlameSegment, 0x706D60);

DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_706FF4, Fix16(0x100, 0), 0x706FF4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpNineThirtySeconds, Fix16(0x1200, 0), 0x706FEC);
DEFINE_GLOBAL(Fix16, kFpZero_706EB8, 0x706EB8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_706EBC, Fix16(1), 0x706EBC);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_706EC4, Fix16(3), 0x706EC4);
DEFINE_GLOBAL_INIT(Ang16, kAng45_706D6C, Ang16(180), 0x706D6C);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng315, Ang16(1260), 0x706E28);
DEFINE_GLOBAL(u8, gElectroChainDepth_706C94, 0x706C94);
DEFINE_STATIC_GLOBAL(Ang16, sElectroShooterRotation, 0x707004);
DEFINE_STATIC_GLOBAL(Fix16_Point_POD, sElectroShooterVelocity, 0x706E58);
DEFINE_STATIC_GLOBAL(Fix16_Point, kZeroPoint, 0x706F90);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_706CC8, Fix16(0x800, 0), 0x706CC8);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_706EDC, Fix16(8), 0x706EDC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_706F70, Fix16(0x100, 0), 0x706F70);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpMinusThreeSixtyFourths, Fix16(0xFFFFFD00, 0), 0x706DCC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_706FD0, Fix16(0x100, 0), 0x706FD0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeThirtySeconds, Fix16(0x600, 0), 0x706EA4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpMinusNineThirtySeconds, Fix16(0xFFFFEE00, 0), 0x706EE8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint03, Fix16(0x1EB, 0), 0x706E7C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_706CF0, Fix16(0x666, 0), 0x706CF0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint02, Fix16(0x147, 0), 0x706E80);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_706DA8, Fix16(0.5), 0x706DA8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_706E74, Fix16(0xA3, 0), 0x706E74);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne512th, Fix16(0x20, 0), 0x706F64);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kThrowSpeedBonus, Fix16(0x340, 0), 0x706C8C);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeEighths, kFpOne64th_706F70 * 24, 0x706EB4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveThirtySeconds, kFpOne64th_706F70 * 10, 0x706E6C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, sTankCannonLength, kFpOne64th_706F70 * 30, 0x706E20);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_706D88, kFpOne64th_706F70 * 8, 0x706D88);

// Defined in WeaponManager.cpp: with the definition (and its dynamic initialiser) in this TU, VC6
// loads it with a 32-bit mov in dual_pistol instead of the original's 16-bit mov/sub.
EXTERN_GLOBAL(Ang16, kAng12_706D5E);
// Keeps its old name (6 degrees): the name sets its .bss offset, which shotgun depends on (docs/matching_quirks.md)
DEFINE_GLOBAL_INIT(Ang16, kShotgunInnerSpread_707002, Ang16(24), 0x707002);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng24, Ang16(96), 0x706D5C);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_707006, Ang16(0), 0x707006);

MATCH_FUNC(0x5DCD10)
Weapon::Weapon()
{
    mAmmo = 0;
    mpPed = NULL;
    mCar = NULL;
    mReloadTimer = 0;
    mbTargetHit = 0;
    mpNext = NULL;
    mWeaponType = 0;
    mUnused10 = 0;
    mUnused8 = 0;
    mUnusedC = -1;
    mbAiTestShotFired = 0;
    mbThrown = 0;
    mShotFired = false;
    mpSound = NULL;
}

MATCH_FUNC(0x5DCD50)
Weapon::~Weapon()
{
    mpPed = NULL;
    mpNext = NULL;
    mCar = NULL;
    mUnused8 = 0;
    if (mpSound)
    {
        gRoot_sound.DestroySoundObj(mpSound);
        mpSound = NULL;
    }
}

MATCH_FUNC(0x5DCD90)
void Weapon::init()
{
    mpPed = NULL;
    mCar = NULL;
    mWeaponType = 0;
    mAmmo = 0;
    mReloadTimer = 0;
    mbTargetHit = 0;
    mbThrown = 0;
    mUnused8 = 0;
    mUnusedC = -1;
    mbAiTestShotFired = 0;
    mShotFired = false;
    if (!mpSound && !bSkip_audio)
    {
        mpSound = gRoot_sound.CreateSoundObject(this, SoundObjectTypeEnum::Weapon_30_7);
    }
}

MATCH_FUNC(0x5DCDE0)
void Weapon::PoolDeallocate()
{
    init();

    mUnused8 = 0;

    if (mpSound)
    {
        gRoot_sound.DestroySoundObj(mpSound);
        mpSound = NULL;
    }
}

MATCH_FUNC(0x5dce20)
void Weapon::set_ammo_clips(u8 clips)
{
    mAmmo = clips * 10;
}

MATCH_FUNC(0x5dce40)
bool Weapon::add_ammo_capped(u8 to_add)
{
    s32 cap_total = max_ammo_capacity[mWeaponType] * 10;
    if (is_infinite_ammo())
    {
        return false;
    }

    u16 cur_amount = mAmmo;
    if (cur_amount == cap_total)
    {
        return false;
    }

    s32 new_amount = cur_amount + (to_add * 10);
    if (new_amount > cap_total)
    {
        mAmmo = cap_total;
    }
    else
    {
        mAmmo = new_amount;
    }
    return true;
}

MATCH_FUNC(0x5dcea0)
bool Weapon::is_max_capacity() const
{
    return this->mAmmo == 10 * max_ammo_capacity[this->mWeaponType];
}

MATCH_FUNC(0x5dcef0)
bool Weapon::CountsAsArmedAttack() const
{
    bool result;
    switch (mWeaponType)
    {
        case weapon_type::car_bomb:
        case weapon_type::oil_stain:
        case weapon_type::car_mines:
        case weapon_type::fire_truck_gun:
        case weapon_type::weapon_0x17:
            result = false;
            break;
        case weapon_type::car_smg:
        case weapon_type::tank_main_gun:
        case weapon_type::fire_truck_flamethrower:
        case weapon_type::army_gun_jeep:
            result = true;
            break;
        default:
            result = mpPed->mCar == NULL;
            break;
    }
    return result;
}

MATCH_FUNC(0x5dcf40)
void Weapon::ApplyFastReload()
{
    Player* pPlayer = mpPed->mPlayer;
    if (pPlayer)
    {
        if (pPlayer->mPowerUpTimers[power_up_indices::FastReload_8])
        {
            mReloadTimer /= 2;
        }
    }
}

// 9.6f 0x4CDA90
MATCH_FUNC(0x5dcf60)
Object* Weapon::spawn_bullet(s32 bullet_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rot, Fix16_Point& speed)
{
    // probe_x/probe_y are assigned, not initialised: initialised, VC6 folds them into the call
    // arguments and computes y first
    Fix16 probe_x;
    Fix16 probe_y;

    Sprite* pProbeSprite = gObject_5C->mCollisionProbeSprite;
    Object* pNewBullet = gObject_5C->NewPhysicsObj(bullet_type, xpos, ypos, zpos, rot);

    probe_x = mpPed->get_cam_x() + (xpos - mpPed->get_cam_x()) / kFP16Two_706EC0;
    probe_y = mpPed->get_cam_y() + (ypos - mpPed->get_cam_y()) / kFP16Two_706EC0;
    pProbeSprite->set_xyz_lazy(probe_x, probe_y, zpos);

    pProbeSprite->set_ang_lazy(pNewBullet->mSprite->mRotation);

    pProbeSprite->AllocCollisionShape(pNewBullet->mDefinition->mWidth, kFP16Quarter_706CF4, pNewBullet->mDefinition->mDepth);
    pProbeSprite->SetType(pNewBullet->mSprite->GetType());
    pProbeSprite->SetObject2C(pNewBullet->mSprite->mObject2CPtr);

    pNewBullet->SetDamageOwner(mpPed->get_ped_ref_idx());

    if (bullet_type == objects::machine_gun_bullet_254 || bullet_type == objects::pistol_bullet_265)
    {
        pNewBullet->SetSpriteIdxOffset(mpPed->GetBulletSpriteOffset());
    }

    if (pProbeSprite->CheckSpriteMovementRegion())
    {
        pNewBullet->RequestRemoval();
        pNewBullet = NULL;
        sbAllowFlameSegment = false;
    }
    else
    {
        sbAllowFlameSegment = true;
        pNewBullet->SetMovementVector(speed);
    }
    return pNewBullet;
}

// https://decomp.me/scratch/73olU
MATCH_FUNC(0x5dd0f0)
void Weapon::flamethrower()
{
    Ang16 ped_rot;
    Fix16_Point cartesian_offset;
    Fix16_Point ped_velocity;

    Fix16 cam_x = mpPed->get_cam_x();
    Fix16 cam_y = mpPed->get_cam_y();
    Fix16 cam_z = mpPed->get_cam_z();

    ped_rot = mpPed->GetRotation();

    ped_velocity = mpPed->GetVelocityVector();

    cartesian_offset.FromPolar_41E210(kFP16Quarter_706CF4, ped_rot);

    Fix16 xpos = cam_x + cartesian_offset.x;
    Fix16 ypos = cam_y + cartesian_offset.y;

    if (!mbTargetHit)
    {
        SetShotFired(true);
        sbAllowFlameSegment = false;
        Weapon::spawn_bullet(objects::invisible_bullet_154, xpos, ypos, cam_z, ped_rot, ped_velocity);
        if (sbAllowFlameSegment)
        {
            gParticle_8->EmitFlameStreamSegment(mpPed->mGameObject->mSpritePtr);

            if (mpPed->PedTypeIs(ped_type::player_2))
            {
                DecreaseAmmo();
            }
            mpPed->AddThreateningPedToList();
            if (mpPed->is_player())
            {
                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
            }
        }
    }
    else
    {
        Weapon::spawn_bullet(objects::flamethrower_bullet_195, cam_x, cam_y, cam_z, ped_rot, ped_velocity);
    }
}

// https://decomp.me/scratch/3qEdg
// The spread angles are built three ways: Ang16(s16, u8) gives the 16-bit add/sub + jns, operator+
// (the const s16& ctor) gives the 32-bit lea + test/jge of the first tank shot, and the last tank shot
// normalizes out of line through Normalize_406C20.
MATCH_FUNC(0x5dd290)
void Weapon::shotgun()
{
    Ang16 ped_rotation;
    Fix16_Point vector;
    if (mReloadTimer == 0)
    {
        Fix16 x = mpPed->get_cam_x();
        Fix16 y = mpPed->get_cam_y();
        Fix16 z = mpPed->get_cam_z();
        ped_rotation = mpPed->GetRotation();
        vector = mpPed->GetVelocityVector();
        SetShotFired(true);
        if (!mbTargetHit)
        {
            Object* pBullet_1 = Weapon::spawn_bullet(objects::shotgun_bullet_192, x, y, z, Ang16(kAng12_706D5E.rValue + ped_rotation.rValue, static_cast<u8>(0)), vector);
            Object* pBullet_2 = Weapon::spawn_bullet(objects::shotgun_bullet_192, x, y, z, Ang16(kShotgunInnerSpread_707002.rValue + ped_rotation.rValue, static_cast<u8>(0)), vector);
            Object* pBullet_3 = Weapon::spawn_bullet(objects::shotgun_bullet_192, x, y, z, ped_rotation, vector);
            Object* pBullet_4 = Weapon::spawn_bullet(objects::shotgun_bullet_192, x, y, z, Ang16(ped_rotation.rValue - kShotgunInnerSpread_707002.rValue, static_cast<u8>(0)), vector);
            Object* pBullet_5 = Weapon::spawn_bullet(objects::shotgun_bullet_192, x, y, z, Ang16(ped_rotation.rValue - kAng12_706D5E.rValue, static_cast<u8>(0)), vector);
            if ((pBullet_1 || pBullet_2 || pBullet_3 || pBullet_4 || pBullet_5) && mpPed->PedTypeIs(ped_type::player_2))
            {
                decrement_ammo();
            }
            mReloadTimer = 40;
            mpPed->AddThreateningPedToList();
            gParticle_8->GunMuzzelFlash(mpPed->mGameObject->mSpritePtr);

            if (mpPed->is_player())
            {
                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
            }
        }
        else
        {
            Weapon::spawn_bullet(objects::invisible_shotgun_bullet_193, x, y, z, kAng24 + ped_rotation, vector);
            Weapon::spawn_bullet(objects::invisible_shotgun_bullet_193, x, y, z, Ang16(kAng12_706D5E.rValue + ped_rotation.rValue, static_cast<u8>(0)), vector);
            Weapon::spawn_bullet(objects::invisible_shotgun_bullet_193, x, y, z, ped_rotation, vector);
            Weapon::spawn_bullet(objects::invisible_shotgun_bullet_193, x, y, z, Ang16(ped_rotation.rValue - kAng12_706D5E.rValue, static_cast<u8>(0)), vector);
            Weapon::spawn_bullet(objects::invisible_shotgun_bullet_193, x, y, z, Ang16(ped_rotation.rValue - kAng24.rValue).Normalized(), vector);
            mReloadTimer = 5;
        }
        Weapon::ApplyFastReload();
    }
    else
    {
        mReloadTimer--;
    }
}

// Fix16_Point::FromPolar_41E210 (9.6f 0x41E210) as it comes out in pistol: the x line
// reads the sine table directly (through Ang16::sine VC6 loads the radius first), the
// y line is the out of line radius * cos
static inline void FromPolar_41E210_sin_table(Fix16_Point& p, const Fix16& radius, const Ang16& angle)
{
    p.x = radius * gSin_table[angle.rValue];
    p.y = radius * Ang16::cosine(angle);
}

// 9.6f 0x4CE070
MATCH_FUNC(0x5dd860)
void Weapon::pistol()
{
    Ang16 pedRot;
    Fix16_Point offset;
    Fix16_Point velocity;

    if (mReloadTimer == 0)
    {
        SetShotFired(true);
        if (!mbTargetHit)
        {
            const s32 bullet_type = mpPed->PedTypeIs(ped_type::player_2) ? 265 : 254;

            Fix16 x = mpPed->get_cam_x();
            Fix16 y = mpPed->get_cam_y();
            Fix16 z = mpPed->get_cam_z();
            pedRot = mpPed->GetRotation();
            velocity = mpPed->GetVelocityVector();
            FromPolar_41E210_sin_table(offset, kFP16Quarter_706CF4, pedRot);
            Fix16 xx = x + offset.x;
            Fix16 yy = y + offset.y;
            if (spawn_bullet(bullet_type, xx, yy, z, pedRot, velocity))
            {
                if (mpPed->PedTypeIs(ped_type::player_2))
                {
                    decrement_ammo();
                }
            }

            mReloadTimer = 20;

            gParticle_8->GunMuzzelFlash(mpPed->mGameObject->mSpritePtr);
            mpPed->AddThreateningPedToList();

            if (mpPed->is_player())
            {
                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, this->mpPed);
            }
        }
        else
        {
            spawn_bullet(objects::invisible_bullet_154,
                                mpPed->get_cam_x(),
                                mpPed->get_cam_y(),
                                mpPed->get_cam_z(),
                                mpPed->GetAimAngle(),
                                mpPed->GetVelocityVector());
            mReloadTimer = 5;
        }
        ApplyFastReload();
    }
    else
    {
        mReloadTimer--;
    }
}

// It matches on decompme: https://decomp.me/scratch/dAQ5C
MATCH_FUNC(0x5dda70)
void Weapon::dual_pistol()
{
    Ang16 ped_rotation;
    Fix16_Point vector;
    Fix16_Point vector2;
    Fix16_Point vector3;
    Fix16_Point vector_esp1; // not used, but required to match
    Fix16_Point vector_esp2; // not used, but required to match

    if (mReloadTimer == 0)
    {
        vector = mpPed->GetVelocityVector();

        Fix16 x = mpPed->get_cam_x();
        Fix16 y = mpPed->get_cam_y();
        Fix16 z = mpPed->get_cam_z();
        ped_rotation = mpPed->GetRotation();
        vector3 = mpPed->GetVelocityVector(); // not used, but required to match

        vector2.FromPolar_41E210(kFP16Quarter_706CF4, ped_rotation);
        Fix16 point_x = x + vector2.x;
        Fix16 point_y = y + vector2.y;

        SetShotFired(true);
        if (!mbTargetHit)
        {
            s32 weapon_bullet_model;
            if (mpPed->PedTypeIs(ped_type::player_2))
            {
                weapon_bullet_model = objects::pistol_bullet_265;
            }
            else
            {
                weapon_bullet_model = objects::machine_gun_bullet_254;
            }

            Object* pBullet_1 =
                Weapon::spawn_bullet(weapon_bullet_model, point_x, point_y, z, ped_rotation - kAng12_706D5E, vector);
            Object* pBullet_2 =
                Weapon::spawn_bullet(weapon_bullet_model, point_x, point_y, z, ped_rotation + kAng12_706D5E, vector);
            if ((pBullet_1 || pBullet_2) && mpPed->PedTypeIs(ped_type::player_2))
            {
                decrement_ammo();
            }
            mReloadTimer = 10;
            mpPed->AddThreateningPedToList();
            gParticle_8->GunMuzzelFlash(mpPed->mGameObject->mSpritePtr);

            if (mpPed->is_player())
            {
                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
            }
        }
        else
        {
            Weapon::spawn_bullet(objects::invisible_bullet_154, point_x, point_y, z, ped_rotation - kAng12_706D5E, vector);
            Weapon::spawn_bullet(objects::invisible_bullet_154, point_y, point_y, z, ped_rotation + kAng12_706D5E, vector);
            mReloadTimer = 5;
        }
        Weapon::ApplyFastReload();
    }
    else
    {
        mReloadTimer--;
    }
}

// https://decomp.me/scratch/lAo1H
MATCH_FUNC(0x5ddd20)
void Weapon::smg()
{
    Fix16_Point point;
    if (mReloadTimer == 0)
    {
        SetShotFired(true);
        if (!mbTargetHit)
        {
            Ang16 AimAngle;
            AimAngle = mpPed->ComputeAimAngle();

            point.x = -kFpPoint03;
            point.y = kFpPoint1_706CF0 + kFpPoint02;

            point.RotateByAngle(mpPed->mGameObject->mSpritePtr->mRotation);

            point = point.Add(mpPed->GetVelocityVector());

            if (Weapon::spawn_bullet(objects::machine_gun_bullet_254,
                                            mpPed->get_cam_x() + point.x,
                                            mpPed->get_cam_y() + point.y,
                                            mpPed->get_cam_z(),
                                            AimAngle,
                                            mpPed->GetVelocityVector()))
            {
                if (mpPed->PedTypeIs(ped_type::player_2))
                {
                    DecreaseAmmo();
                }
            }

            mReloadTimer = 2;
            gParticle_8->GunMuzzelFlash(mpPed->mGameObject->mSpritePtr);

            if (mWeaponType != weapon_type::silence_smg)
            {
                mpPed->AddThreateningPedToList();
                if (mpPed->is_player())
                {
                    gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
                }
            }
        }
        else
        {
            Weapon::spawn_bullet(objects::invisible_bullet_154,
                                        mpPed->get_cam_x(),
                                        mpPed->get_cam_y(),
                                        mpPed->get_cam_z(),
                                        mpPed->GetAimAngle(),
                                        mpPed->GetVelocityVector());
            mReloadTimer = 1;
        }
        Weapon::ApplyFastReload();
    }
    else
    {
        mReloadTimer--;
    }
}

// https://decomp.me/scratch/OrmRn
MATCH_FUNC(0x5ddfc0)
void Weapon::throwable(s32 obj_idx, s32 throw_strength, s32 throw_charge)
{
    Fix16_Point vector;

    if (throw_strength)
    {
        mbThrown = 0;
        if (mReloadTimer == 0)
        {
            SetShotFired(true);
            if (!mbTargetHit)
            {
                if (!mpPed->PedTypeIs(ped_type::player_2) && !mbAiTestShotFired)
                {
                    spawn_bullet(objects::invisible_rocket_159,
                                        mpPed->get_cam_x(),
                                        mpPed->get_cam_y(),
                                        mpPed->get_cam_z(),
                                        mpPed->GetAimAngle(),
                                        mpPed->GetVelocityVector());
                    mReloadTimer = 5;
                    mbAiTestShotFired = 1;
                    if (mpPed->is_player())
                    {
                        gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
                    }
                }
                else
                {
                    if (obj_idx == objects::grenade_obj_183 && throw_charge == 96)
                    {
                        // maybe holding the grenade for too long
                        gObject_5C->CreateExplosion(mpPed->get_cam_x(),
                                                                  mpPed->get_cam_y(),
                                                                  mpPed->get_cam_z(),
                                                                  mpPed->GetAimAngle(),
                                                                  explosion_type::small_18,
                                                                  mpPed->mId);
                        if (mpPed->PedTypeIs(ped_type::player_2))
                        {
                            decrement_ammo();
                        }
                        if (mpPed->is_player())
                        {
                            gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
                        }
                    }
                    else
                    {
                        Fix16 speed;
                        if (obj_idx == objects::grenade_obj_183)
                        {
                            speed = kFpPoint01_706E74 + (Fix16(throw_strength) / Fix16(60)) * (kFpPoint1_706CF0 + kFpPoint02);
                        }
                        else
                        {
                            speed = kFpPoint02 + (Fix16(throw_strength) / Fix16(60)) * kFpPoint1_706CF0;
                        }
                        gObject_5C->SetPendingDamageOwner(mpPed->get_ped_ref_idx());

                        Object* pProjectile = gObject_5C->NewMovingObjUnlessBlocked(obj_idx,
                                                                               mpPed->get_cam_x(),
                                                                               mpPed->get_cam_y(),
                                                                               mpPed->get_cam_z() + kFP16Half_706DA8,
                                                                               mpPed->GetAimAngle(),
                                                                               mpPed->GetAimAngle(),
                                                                               speed,
                                                                               -kFpOne512th,
                                                                               kFpPoint1_706CF0);
                        if (pProjectile)
                        {
                            if (!mpPed->mGameObject->mFlagsBf.bReverseVelocity)
                            {
                                vector = mpPed->GetVelocityVector();
                                pProjectile->SetMovementVector(vector);
                                if (!vector.IsNull_420360())
                                {
                                    pProjectile->mObj3c->mSpeed += kThrowSpeedBonus;
                                }
                            }
                            if (obj_idx == objects::moving_molotov_138)
                            {
                                Object* pLightObj = gObject_5C->NewLight(94, 138, 2, 0xFF8000, 3, 255);
                                pProjectile->mSprite->AttachSprite(pLightObj->mSprite, 0, 0, kAngZero_707006);
                                Object* pExplosionObj =
                                    gObject_5C->CreateExplosion(Fix16(113), Fix16(145), 2, kAngZero_707006, explosion_type::molotov_fire_5, mpPed->mId);
                                if (pExplosionObj)
                                {
                                    pProjectile->mSprite->AttachSprite(pExplosionObj->mSprite, 0, 0, kAngZero_707006);
                                }
                            }
                            else
                            {
                                pProjectile->SetO8Timer((96 - throw_charge) / 8);
                            }

                            if (mpPed->PedTypeIs(ped_type::player_2))
                            {
                                decrement_ammo();
                            }
                            mbThrown = 1;
                            if (mpPed->is_player())
                            {
                                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
                            }
                            mpPed->AddThreateningPedToList();
                        }
                    }
                    if (mpPed->mPlayer)
                    {
                        mReloadTimer = 4;
                        mpPed->mBf21C.bHoldPosition = true;
                    }
                    else
                    {
                        mReloadTimer = 50;
                        mpPed->mBf21C.bHoldPosition = true;
                    }
                    mbThrown = 1;
                    Weapon::ApplyFastReload();
                }
            }
            else
            {
                spawn_bullet(objects::invisible_rocket_159,
                                    mpPed->get_cam_x(),
                                    mpPed->get_cam_y(),
                                    mpPed->get_cam_z(),
                                    mpPed->GetAimAngle(),
                                    mpPed->GetVelocityVector());
                mReloadTimer = 5;
                mbAiTestShotFired = 0;
                Weapon::ApplyFastReload();
            }
        }
        else
        {
            if (mbAiTestShotFired == 0)
            {
                mpPed->mBf21C.bHoldPosition = false;
            }
            --mReloadTimer;
            if (mReloadTimer < 30 && mbThrown)
            {
                mbThrown = 0;
                mpPed->mBf21C.bHoldPosition = false;
            }
        }
    }
}

EXPORT void __stdcall DrawElectricArc(Fix16_Point_POD start_pos, Fix16_Point& end_pos, Fix16 z_pos);

// GetLength_41E260 with both Abs and the x multiply, the add and the square root out of line
static inline Fix16 BeamLength_5DE4F0(Fix16_Point& d)
{
    if (d.x == kFpZero_706EB8)
    {
        return Fix16::Abs_436A50(d.y);
    }
    else if (d.y == kFpZero_706EB8)
    {
        return Fix16::Abs_436A50(d.x);
    }
    else
    {
        return Fix16::SquareRoot_436A70(d.x.Multiply(d.x).Add(d.y * d.y));
    }
}

// PolarToCartesian_41FC20 with the cosine multiply out of line. The radius is a by-value copy: the original
// stores it once after the joins and its slot is shared with step_y, which is not address-taken.
static inline void BeamStep_5DE4F0(Ang16& angle, Fix16 radius, Fix16& ret1, Fix16& ret2)
{
    ret1 = Ang16::sine(angle) * radius;
    ret2 = Ang16::cosine(angle).Multiply(radius);
}

// The direct mCam.x read in `delta` (instead of get_cam_x()) drops one register candidate from the
// first block, so `angle` loses ebx to the `dist << 14` temp and is spilled like in the original.
MATCH_FUNC(0x5de4f0)
void Weapon::TraceBeamToHitTarget()
{
    Sprite* pBeam = gObject_5C->mCollisionProbeSprite;
    Fix16_Point delta(mpPed->mHitTargetPed->mCam.x - mpPed->get_cam_x(),
                      mpPed->mHitTargetPed->get_cam_y() - mpPed->get_cam_y());
    gCollisionTarget.Reset();

    Ang16 angle;
    angle = Fix16::atan2_fixed(mpPed->mHitTargetPed->get_cam_y() - mpPed->get_cam_y(),
                                            mpPed->mHitTargetPed->get_cam_x() - mpPed->get_cam_x());

    // Block scopes from the original frame: dist and i share a slot, and the atan2 argument temporaries
    // share theirs with angle and the radius.
    {
        Fix16 steps;
        Fix16 step_x;
        Fix16 step_y;
        {
            Fix16 dist = BeamLength_5DE4F0(delta);

            if (dist > kFpThree_706EC4)
            {
                mpPed->mHitTargetPed = NULL;
                return;
            }

            pBeam->set_xyz_lazy(mpPed->mCam.x, mpPed->mCam.y, mpPed->mCam.z);
            pBeam->set_ang_lazy(angle);
            pBeam->AllocCollisionShape(kFpPoint1_706CF0, kFpPoint1_706CF0, kFpPoint1_706CF0);

            Fix16 step_len;
            if (dist != kFpZero_706EB8)
            {
                steps = dist / kFpPoint1_706CF0;
                step_len = dist / steps;
            }
            else
            {
                steps = kFpZero_706EB8;
                step_len = kFpZero_706EB8;
            }

            if (steps < kFpOne_706EBC)
            {
                steps = kFpOne_706EBC;
                step_len = dist;
            }

            BeamStep_5DE4F0(angle, step_len, step_x, step_y);
        }
        for (u8 i = 1; i <= steps.ToInt(); i++)
        {
            gMap_0x370->FindGroundZBelowCoord(pBeam->mXy.x, pBeam->mXy.y, pBeam->mZpos);
            pBeam->set_xy_lazy(pBeam->mXy.x + step_x, pBeam->mXy.y + step_y);
            if (pBeam->CheckBulletWallCollision())
            {
                break;
            }

            Sprite* pHit = pBeam->QuerySpriteCollision(sprite_types_enum::car_2);
            if (pHit)
            {
                switch (pHit->mSpriteTypeEnum)
                {
                    case sprite_types_enum::car_2:
                        mpPed->mSelectedWeapon->SetTargetHit(1);
                        mpPed->mHitTargetPed = NULL;
                        return;

                    case sprite_types_enum::ped_3:
                    {
                        if (pHit->mCharB4Ptr->mpPed != mpPed->mHitTargetPed &&
                            pHit->mCharB4Ptr->mpPed != mpPed)
                        {
                            s32 state = pHit->mCharB4Ptr->mpPed->mPedState1;
                            if (state < 8 || state > 9)
                            {
                                mpPed->mSelectedWeapon->SetTargetHit(1);
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    mpPed->mHitTargetPed->SetAttacker(mpPed);
    mpPed->mHitTargetPed->mKillerId = mpPed->mId;
    mpPed->mHitTargetPed->mBf21C.bHitByAttacker = 1;
    if (mpPed->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
    {
        gPolice_7B8->mLastFiringEmergencyPed = mpPed;
    }

    DrawElectricArc(mpPed->mGameObject->mSpritePtr->get_x_y(),
               mpPed->mHitTargetPed->mGameObject->mSpritePtr->get_x_y(),
               mpPed->get_cam_z());
}

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_706CF8, Fix16(0xCCC, 0), 0x706CF8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_706D34, Fix16(0x100, 0), 0x706D34);

// Length of `d`, with kFpZero_706EB8 as the zero (the multiplies, the add and the square root are the named
// out-of-line copies; Abs follows the inline budget).
static inline Fix16 BeamLength_5DE910(Fix16_Point& d)
{
    if (d.x == kFpZero_706EB8)
    {
        return Fix16::Abs(d.y);
    }
    else if (d.y == kFpZero_706EB8)
    {
        return Fix16::Abs(d.x);
    }
    else
    {
        return Fix16::SquareRoot_436A70(d.x.Multiply(d.x).Add(d.y.Multiply(d.y)));
    }
}

// Draws the electro beam from `start_pos` (or the gun muzzle when gElectroChainDepth_706C94 is clear) to `end_pos` at height
// `z_pos`: kFP16Quarter_706CF4 long segments with a random kink each, then straight segments for the rest.
// `start_pos` is the base type: callers pass a get_x_y() temporary, which is sliced into it
// (Fix16_Point here breaks the caller ChainShockTargets). 9.6f: DrawElectricArc_4CCBD0.
// The EH state is 0xA at entry and never changes: 11 Fix16_Point locals, two of them unused
// (likely the original's by-value start_pos is one of them). `d` is reused for the rest of the way and
// `len` for the straight segment count, as the original's frame shows.
MATCH_FUNC(0x5de910)
void __stdcall DrawElectricArc(Fix16_Point_POD start_pos, Fix16_Point& end_pos, Fix16 z_pos)
{
    Fix16_Point d;
    Fix16_Point start;
    Fix16_Point cur;
    Fix16_Point mid;
    Fix16_Point next;
    Fix16_Point step;
    Fix16_Point d3;
    Fix16_Point from;
    Fix16_Point to;
    Fix16_Point unused_eh_1;
    Fix16_Point unused_eh_2;

    Fix16 seg_len = kFP16Quarter_706CF4;
    if (gElectroChainDepth_706C94 > 0)
    {
        start.x = start_pos.x;
        start.y = start_pos.y;
    }
    else
    {
        start.x = -kFpPoint03;
        start.y = kFpPoint2_706CF8 + kFpPoint02;
        start.RotateByAngle_40F6B0_all_out_of_line(sElectroShooterRotation);
        start = start.Add(start_pos);
        start.x += sElectroShooterVelocity.x;
        start.y += sElectroShooterVelocity.y;
    }

    Fix16 len;
    Ang16 angle;
    Ang16 seg_angle;
    // The first length and angle are dead, like 9.6f's (it only kept the atan2 call)
    d = end_pos.Sub(start);
    len = BeamLength_5DE910(d);
    d.GetAngle();

    to = end_pos;
    from = start;
    d = end_pos.Sub(start);
    len = BeamLength_5DE910(d);
    gRng.get_int(2);

    from = start;
    to = end_pos;
    d3 = to.Sub(from);
    len = BeamLength_5DE910(d3);
    u8 count = (len / seg_len).ToInt();
    angle = d3.GetAngle();

    cur = from;
    for (u8 i = 0; i < count; i++)
    {
        // 9.6f: Fix16(s16) 0x401AE0 minus Fix16(u16) 0x41F990, times kFpOne64th_706D34. The kink stays a
        // temporary (a named local moves the frame slots) and `-=` evaluates the half first.
        u16 spread = (gRng.get_int(4) + 1) * 32;
        Ang16 jitter = Ang16::Fix16_To_Ang16_ool_40F540(
            (Fix16(gRng.get_int(spread)) -= Fix16(static_cast<u16>(spread >> 1))) * kFpOne64th_706D34);

        step.x = seg_len.Multiply(Ang16::sine(angle));
        step.y = seg_len.Multiply(Ang16::cosine(angle));
        step.RotateByAngle_40F6B0_all_out_of_line(jitter);
        seg_angle = step.GetAngle();

        next = cur.Add(step);
        mid = next.Sub(cur);
        mid.x.DivideAssign(kFP16Two_706EC0);
        mid.y.DivideAssign(kFP16Two_706EC0);
        mid.x += cur.x;
        mid.y += cur.y;
        gParticle_8->EmitElectricArcParticle(mid.x, mid.y, z_pos, seg_angle);
        cur = next;
    }

    d = end_pos.Sub(cur);
    seg_angle = d.GetAngle();
    len = d.MaxAbs_5E4140() / seg_len;
    if (len != kFpZero_706EB8)
    {
        d.DivAssign(len);
        cur = next;
        for (s32 j = 1; j <= len.ToInt(); j++)
        {
            next.AddAssign(d);
            mid = next.Sub(cur);
            mid.DivAssign(kFP16Two_706EC0);
            mid.AddAssign(cur);
            gParticle_8->EmitElectricArcParticle(mid.x, mid.y, z_pos, seg_angle);
            cur = next;
        }
    }
}

// MaxAbsDistance as ShockPedsAroundSprite inlines it: past the inline budget, Max is the named out-of-line copy
// Max_44E540 (the Abs calls and the y subtraction go out of line by themselves)
static inline Fix16 MaxAbsDistance_5DF270(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2)
{
    Fix16 diff_x;
    diff_x = x2 - x1;
    Fix16 diff_y;
    diff_y = y2 - y1;
    Fix16 result;
    result = Fix16::Max_44E540(Fix16::Abs(diff_x), Fix16::Abs(diff_y));
    return result;
}

// The pTargetSprite check is a do-while under an if (the first loop's exit jumps straight to the tail, which stays last),
// and the !a4 (shock counter) branch comes first in the source: its get_x_y temporaries take EH states 0/1.
MATCH_FUNC(0x5DF270)
void __stdcall ShockPedsAroundSprite(Sprite* pSprite, Fix16 search_size, bool bCheckAngle, bool bShockAllPeds, Ped* pAttacker, Sprite* pTargetSprite)
{
    // 9.6f 0x4CEF40
    SpriteList hits;
    Fix16 xpos = pSprite->mXy.x;
    Fix16 ypos = pSprite->mXy.y;
    Fix16 zpos = pSprite->mZpos;
    Ang16 angle = pSprite->mRotation;
    Ang16 diff;

    Fix16_Rect rect;
    rect.ComputeCollisionPrism(xpos, ypos, search_size, zpos);

    if (gSpriteGrid_1->CollectRectCollisions(&rect, false, sprite_types_enum::none_0, pSprite, &hits))
    {
        Sprite* pHit;
        if (pTargetSprite)
        {
            // Every hit nearer than pTargetSprite must be inside the angle window
            pHit = hits.TakeClosestSprite(xpos, ypos);
            if (pHit)
            {
                do
                {
                    if (!pHit->AsCharB4())
                    {
                        diff = Fix16::atan2_fixed(pHit->mXy.y - ypos, pHit->mXy.x - xpos) - angle;
                        if (diff < kAng45_706D6C || diff > kAng315)
                        {
                            hits.ClearList();
                            if (pAttacker->mSelectedWeapon)
                            {
                                pAttacker->mSelectedWeapon->SetTargetHit(1);
                            }
                            return;
                        }
                    }
                    else
                    {
                        if (pHit == pTargetSprite)
                        {
                            break;
                        }
                        diff = Fix16::atan2_fixed(pHit->mXy.y - ypos, pHit->mXy.x - xpos) - angle;
                        if (diff < kAng45_706D6C || diff > kAng315)
                        {
                            hits.ClearList();
                            if (pAttacker->mSelectedWeapon)
                            {
                                pAttacker->mSelectedWeapon->SetTargetHit(1);
                            }
                            return;
                        }
                    }
                    pHit = hits.TakeClosestSprite(xpos, ypos);
                } while (pHit);
            }
        }
        else
        {
            pHit = hits.TakeClosestSprite(xpos, ypos);
        }

        while (pHit)
        {
            Char* pChar = pHit->AsCharB4();
            if (pChar)
            {
                bool bOutside;
                if (bCheckAngle)
                {
                    diff = Fix16::atan2_fixed(pHit->mXy.y - ypos, pHit->mXy.x - xpos) - angle;
                    if (diff < kAng45_706D6C || diff > kAng315)
                    {
                        bOutside = true;
                    }
                    else
                    {
                        bOutside = false;
                    }
                }
                else
                {
                    bOutside = true;
                }

                // The result is unused
                MaxAbsDistance_5DF270(pAttacker->get_cam_x(), pAttacker->get_cam_y(), pChar->mSpritePtr->mXy.x, pChar->mSpritePtr->mXy.y);

                if (bOutside)
                {
                    if (gMap_0x370->IsProbePathClear(kFpPoint1_706CF0,
                                                      kFpPoint1_706CF0,
                                                      kFpPoint1_706CF0,
                                                      xpos,
                                                      ypos,
                                                      zpos,
                                                      pHit->mXy.x,
                                                      pHit->mXy.y,
                                                      pHit->mZpos))
                    {
                        pChar->mpPed->SetAttacker(pAttacker);
                        pChar->mpPed->mKillerId = pAttacker->mId;
                        pChar->mpPed->mDeathCause = ped_death_cause::rocket_18;
                        pChar->mpPed->mKillerIdTimer = 50;
                        if (!bShockAllPeds)
                        {
                            Fix16 z;
                            if (pHit->mZpos > zpos)
                            {
                                z = pHit->mZpos;
                            }
                            else
                            {
                                z = zpos;
                            }
                            DrawElectricArc(pSprite->get_x_y(), pHit->get_x_y(), z);
                            pChar->mpPed->mShockCounter += 3;
                            if (pAttacker->mSelectedWeapon)
                            {
                                pAttacker->mSelectedWeapon->SetTargetHit(0);
                            }
                            hits.ClearList();
                            return;
                        }
                        else
                        {
                            Fix16 z;
                            if (pHit->mZpos > zpos)
                            {
                                z = pHit->mZpos;
                            }
                            else
                            {
                                z = zpos;
                            }
                            DrawElectricArc(pSprite->get_x_y(), pHit->get_x_y(), z);
                            pChar->mpPed->TakeDamage(3);
                        }
                    }
                }
                else if (pAttacker->mHitTargetPed)
                {
                    pAttacker->mHitTargetPed = NULL;
                }
            }
            pHit = hits.TakeClosestSprite(xpos, ypos);
        }
    }

    pAttacker->mHitTargetPed = NULL;
    if (pAttacker->mSelectedWeapon)
    {
        pAttacker->mSelectedWeapon->SetTargetHit(1);
    }
}

MATCH_FUNC(0x5dfb60)
void Weapon::ChainShockTargets(u8 chain_depth, Sprite* pSprite, Ang16 target_angle)
{
    SpriteList hits;
    bool bHit = false;

    // Both branches compute the four bounds into registers and share the stores.
    Fix16_Rect rect;
    Fix16 left;
    Fix16 right;
    Fix16 top;
    Fix16 bottom;
    if (!chain_depth)
    {
        left = pSprite->mXy.x - kFP16Two_706EC0;
        right = pSprite->mXy.x + kFP16Two_706EC0;
        top = pSprite->mXy.y - kFP16Two_706EC0;
        bottom = pSprite->mXy.y + kFP16Two_706EC0;
    }
    else
    {
        left = pSprite->mXy.x - kFpOne_706EBC;
        right = pSprite->mXy.x + kFpOne_706EBC;
        top = pSprite->mXy.y - kFpOne_706EBC;
        bottom = pSprite->mXy.y + kFpOne_706EBC;
    }
    rect.SetBounds(left, right, top, bottom);
    rect.SetHiLowZ(pSprite->mZpos - kFpOne_706EBC, pSprite->mZpos + kFpOne_706EBC);

    sElectroShooterRotation = mpPed->mGameObject->mSpritePtr->mRotation;
    // The original copies the returned point through the return pointer, as a struct assignment
    reinterpret_cast<Fix16_Point&>(sElectroShooterVelocity) = mpPed->GetVelocityVector();

    if (gSpriteGrid_1->CollectRectCollisions(&rect, false, sprite_types_enum::none_0, pSprite, &hits) && hits.mP18)
    {
        do
        {
            Sprite* pHit = hits.PopFrontSprite();
            // Declared here (9.6f constructs them at the top): both branches share the slots.
            // The ped branch's `angle - target_angle` is past the inline budget, so its Normalize is the
            // out-of-line Normalize_406C20 while the car branch's is inlined.
            Ang16 angle;
            Ang16 diff;
            switch (pHit->GetType())
            {
                case sprite_types_enum::ped_3:
                    if (pSprite != pHit && !gWeapon_8->mHitSprites.SpriteExists(pHit))
                    {
                        angle = Fix16::atan2_fixed(pHit->mXy.y - pSprite->mXy.y, pHit->mXy.x - pSprite->mXy.x);
                        Fix16::MaxAbsDistance(pHit->mXy.x, pHit->mXy.y, pSprite->mXy.x, pSprite->mXy.y);

                        diff = angle - target_angle;
                        if (diff < kAng45_706D6C || diff > kAng315)
                        {
                            if (gMap_0x370->IsProbePathClear(kFpPoint1_706CF0,
                                                              kFpPoint1_706CF0,
                                                              kFpPoint1_706CF0,
                                                              pSprite->mXy.x,
                                                              pSprite->mXy.y,
                                                              pSprite->mZpos,
                                                              pHit->mXy.x,
                                                              pHit->mXy.y,
                                                              pHit->mZpos))
                            {
                                gElectroChainDepth_706C94 = chain_depth;
                                DrawElectricArc(pSprite->get_x_y(),
                                           pHit->get_x_y(),
                                           pHit->mZpos > pSprite->mZpos ? pHit->mZpos : pSprite->mZpos);
                                gWeapon_8->mHitSprites.PushSprite(pHit);
                                if (chain_depth < 1)
                                {
                                    ChainShockTargets(chain_depth + 1, pHit, angle);
                                }
                                pHit->mCharB4Ptr->mpPed->SetAttacker(mpPed);
                                pHit->mCharB4Ptr->mpPed->mKillerId = mpPed->mId;
                                pHit->mCharB4Ptr->mpPed->mDeathCause = ped_death_cause::rocket_18;
                                pHit->mCharB4Ptr->mpPed->mKillerIdTimer = 50;
                                pHit->mCharB4Ptr->mpPed->mShockCounter += 5;
                                if (mpPed->is_player())
                                {
                                    gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
                                }
                                mpPed->AddThreateningPedToList();
                                gScriptManager->RecordWeaponHit(pHit->mCharB4Ptr->mpPed->mId, 160, 1);
                            }
                            bHit = true;
                        }
                    }
                    break;

                case sprite_types_enum::car_2:
                    if (pSprite != pHit && !gWeapon_8->mHitSprites.SpriteExists(pHit))
                    {
                        angle = Fix16::atan2_fixed(pHit->mXy.y - pSprite->mXy.y, pHit->mXy.x - pSprite->mXy.x);
                        Fix16::MaxAbsDistance(pHit->mXy.x, pHit->mXy.y, pSprite->mXy.x, pSprite->mXy.y);

                        diff = angle - target_angle;
                        if (diff < kAng45_706D6C || diff > kAng315)
                        {
                            if (gMap_0x370->IsProbePathClear(kFpPoint1_706CF0,
                                                              kFpPoint1_706CF0,
                                                              kFpPoint1_706CF0,
                                                              pSprite->mXy.x,
                                                              pSprite->mXy.y,
                                                              pSprite->mZpos,
                                                              pHit->mXy.x,
                                                              pHit->mXy.y,
                                                              pHit->mZpos))
                            {
                                gElectroChainDepth_706C94 = chain_depth;
                                DrawElectricArc(pSprite->get_x_y(),
                                           pHit->get_x_y(),
                                           pHit->mZpos > pSprite->mZpos ? pHit->mZpos : pSprite->mZpos);
                                gWeapon_8->mHitSprites.PushSprite(pHit);
                                if (chain_depth < 1)
                                {
                                    ChainShockTargets(chain_depth + 1, pHit, angle);
                                }
                                if (!pHit->mCarBcPtr->IsFlameproof())
                                {
                                    pHit->mCarBcPtr->mExploderPedId = mpPed->mId;
                                    pHit->mCarBcPtr->mDeathCause = ped_death_cause::rocket_18;
                                    pHit->mCarBcPtr->mExploderTimer = 50;
                                    s16 damage = pHit->mCarBcPtr->AccumulateDamage(300, &kZeroPoint);
                                    pHit->mCarBcPtr->TriggerCarAlarm();
                                    if (mpPed->PedTypeIs(ped_type::player_2) && damage > 0)
                                    {
                                        mpPed->mPlayer->mScores.AwardCarDamageScoreHit(pHit->mCarBcPtr, 1);
                                    }
                                }
                                if (mpPed->is_player())
                                {
                                    gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
                                }
                                mpPed->AddThreateningPedToList();
                            }
                            bHit = true;
                            gScriptManager->RecordWeaponHit(pHit->mCarBcPtr->mCarId, 160, 0);
                        }
                    }
                    break;
            }
        } while (hits.mP18);

        if (bHit && !chain_depth)
        {
            SetShotFired(true);
            if (mpPed->PedTypeIs(ped_type::player_2) && (gpRng->get_cur_rng() & 1))
            {
                DecreaseAmmo();
            }
        }
    }
}

MATCH_FUNC(0x5e06b0)
void Weapon::shocker()
{
    gWeapon_8->mHitSprites.ClearList();

    ChainShockTargets(0,
               this->mpPed->mGameObject->mSpritePtr,
               this->mpPed->mGameObject->mSpritePtr->mRotation);
    Ped* pPed = this->mpPed;
    if (pPed->mPlayer)
    {
        gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, pPed);
    }
}

MATCH_FUNC(0x5e0740)
void Weapon::electro_batton()
{
    if (!mpPed->mHitTargetPed)
    {
        if (!mReloadTimer)
        {
            mShotFired = true;
            if (!mbTargetHit)
            {
                Object* pBullet = spawn_bullet(objects::electrobaton_bullet_277,
                                                      mpPed->get_cam_x(),
                                                      mpPed->get_cam_y(),
                                                      mpPed->get_cam_z(),
                                                      mpPed->GetAimAngle(),
                                                      mpPed->GetVelocityVector());
                if (pBullet && mpPed->PedTypeIs(ped_type::player_2))
                {
                    decrement_ammo();
                }
                mReloadTimer = 20;
            }
            else
            {
                spawn_bullet(objects::invisible_bullet_154,
                                    mpPed->get_cam_x(),
                                    mpPed->get_cam_y(),
                                    mpPed->get_cam_z(),
                                    mpPed->GetAimAngle(),
                                    mpPed->GetVelocityVector());
                mReloadTimer = 5;
            }
            ApplyFastReload();
        }
        else
        {
            mReloadTimer--;
        }
    }
    else if (mpPed->mHitTargetPed->mGameObject)
    {
        if (mpPed->PedTypeIs(ped_type::player_2))
        {
            s32 target_state = mpPed->mHitTargetPed->mPedState1;
            if (target_state >= 8 && target_state <= 9)
            {
                mpPed->mHitTargetPed = NULL;
                return;
            }
        }
        TraceBeamToHitTarget();
    }
    else
    {
        mpPed->mHitTargetPed = NULL;
    }
}

MATCH_FUNC(0x5e0ab0)
void Weapon::car_bomb(bool instant_bomb)
{
    mpPed = mCar->GetDriver();

    SetShotFired(true);

    decrement_ammo();

    if (mCar->IsTrailerCab())
    {
        mCar->mpTrailer->mTrailerCar->FireCarBomb(instant_bomb);
        mCar->FlashAndDetachTrailer();
    }
    else
    {
        mCar->FireCarBomb(instant_bomb);
    }
}

DEFINE_GLOBAL_INIT(Ang16, kAng180_706DFA, Ang16(720), 0x706DFA);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpSevenThirtySeconds, Fix16(0xE00, 0), 0x706CDC);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_706CD8, Fix16(0x800, 0), 0x706CD8);

MATCH_FUNC(0x5e0b10)
void Weapon::fire_truck_flamethrower()
{
    Ang16 gun_ang;
    Fix16_Point bullet_pos;
    Fix16_Point offset;
    Fix16_Point velocity;

    AttachedSprite* pTurret;
    Ped* pDriver = mCar->mDriver;
    mpPed = pDriver;

    pTurret = mCar->mAttachments.GetSpriteForModel(objects::water_cannon_114);
    if (pTurret)
    {
        gun_ang = pTurret->mpSprite->mRotation + kAng180_706DFA;

        bullet_pos.SetXY(Fix16(0), kFpSevenThirtySeconds);
        bullet_pos.RotateByAngle(gun_ang);
        offset.SetXY(Fix16(0), kFpOneEighth_706CD8);
    }
    else
    {
        gun_ang = mCar->mAttachments.GetSpriteForModel(248)->mpSprite->mRotation;

        bullet_pos.SetXY(Fix16(0), kFpThreeThirtySeconds);
        bullet_pos.RotateByAngle(gun_ang);
        offset.SetXY(Fix16(0), kFpMinusNineThirtySeconds);
    }

    offset.RotateByAngle(mCar->mCarSprite->mRotation);

    bullet_pos += offset.Add(mCar->mCarSprite->get_x_y_443580());

    velocity = mCar->mPhysics->GetPointVelocity(&bullet_pos);

    SetShotFired(true);

    if (!IsTargetHit())
    {
        gParticle_8->EmitFlameStreamSegment(mCar->mCarSprite);
    }
    else
    {
        spawn_bullet(objects::flamethrower_bullet_195, bullet_pos.x, bullet_pos.y, mCar->mCarSprite->mZpos, gun_ang, velocity);
    }
}

// Both fire truck guns keep the EH state stores around their Fix16_Point add: once VC6 has compiled the
// out-of-line copy of the inline operator+ it knows the call can't throw and drops them. So only this
// function uses operator+, and fire_truck_flamethrower above calls Add. The inline getters
// (GetDriver, IsTargetHit) are free inline sites that set the inline budget split, so the
// second rotation calls Negated out of line as in 10.5.
MATCH_FUNC(0x5e0e70)
void Weapon::fire_truck_gun()
{
    Ang16 gun_ang;
    Fix16_Point bullet_pos;
    Fix16_Point offset;
    Fix16_Point velocity;

    mpPed = mCar->GetDriver();

    AttachedSprite* pTurret = mCar->mAttachments.GetSpriteForModel(objects::water_cannon_114);
    gun_ang = pTurret->mpSprite->mRotation + kAng180_706DFA;

    bullet_pos.SetXY(Fix16(0), kFpSevenThirtySeconds);
    bullet_pos.RotateByAngle(gun_ang);
    offset.SetXY(Fix16(0), kFpOneEighth_706CD8);

    offset.RotateByAngle(mCar->mCarSprite->mRotation);

    bullet_pos += offset + mCar->mCarSprite->get_x_y();

    velocity = mCar->mPhysics->GetPointVelocity(&bullet_pos);

    SetShotFired(true);

    if (!IsTargetHit())
    {
        gParticle_8->EmitFireTruckSprayParticle(mCar->mCarSprite);
    }
    else
    {
        spawn_bullet(objects::fire_truck_gun_bullet_199, bullet_pos.x, bullet_pos.y, mCar->mCarSprite->mZpos, gun_ang, velocity);
    }
}

// Fix16 operator* with the __int64 cast on the left operand: VC6 then keeps the left value (the point's y)
// in eax for the imul as the original does; with Fix16::operator* sin goes to eax
// (tank_main_gun, army_gun_jeep)
static inline Fix16 MultiplyLeftWide(const Fix16& a, const Fix16& b)
{
    return Fix16(static_cast<s32>((static_cast<__int64>(a.mValue) * b.mValue) >> 14), 0);
}

// Fix16 sum as a free function: the sum lands in ecx (operator+ gives eax)
static inline Fix16 AddFree(const Fix16& a, const Fix16& b)
{
    s32 v = a.mValue + b.mValue;
    return Fix16(v, 0);
}

// Fix16_Point::RotateByAngle as VC6 emits it in a caller that has run out of inline
// expansions: the multiplies, the negate and the y line's add are the out of line operator
// copies, and the x line's sum is AddFree (tank_main_gun, army_gun_jeep)
static inline void RotateByAngle_40F6B0_no_budget3(Fix16_Point& p, const Ang16& angle)
{
    Fix16 sin = Ang16::sine(angle);
    Fix16 cos = Ang16::cosine(angle);
    Fix16 x_old = p.x;
    p.x = AddFree(p.x.Multiply(cos), p.y.Multiply(sin));
    p.y = x_old.Negated().Multiply(sin).Add(p.y.Multiply(cos));
}

// https://decomp.me/scratch/QliaE
MATCH_FUNC(0x5e10e0)
void Weapon::tank_main_gun()
{
    Ang16 cannon_angle;
    Fix16_Point cannon_pos;
    Fix16_Point offset;
    Fix16_Point velocity;

    if (mReloadTimer == 0)
    {
        mpPed = mCar->GetDriver();
        cannon_angle =
            mCar->mAttachments.GetSpriteForModel(objects::tank_cannon_148)->mpSprite->mRotation;
        cannon_pos.SetXY(Fix16(0), sTankCannonLength);
        {
            // RotateByAngle_MixOOL_40F6B0 written out, y * sin with the left-wide multiply
            Fix16 x_old = cannon_pos.x;
            Fix16 sin = Ang16::sine(cannon_angle);
            Fix16 cos = Ang16::cosine(cannon_angle);
            cannon_pos.x = cannon_pos.x.Multiply(cos).Add(MultiplyLeftWide(cannon_pos.y, sin));
            cannon_pos.y = x_old.Negated().Multiply(sin).Add(cannon_pos.y.Multiply(cos));
        }

        offset.SetXY(Fix16(0), kFpOneEighth_706D88);
        RotateByAngle_40F6B0_no_budget3(offset, mCar->mCarSprite->mRotation);

        cannon_pos += offset.Add(mCar->mCarSprite->get_x_y());

        if (mCar->mPhysics)
        {
            velocity = mCar->mPhysics->GetPointVelocity(&cannon_pos);
        }
        else
        {
            velocity.reset();
        }
        SetShotFired(true);
        if (!mbTargetHit)
        {
            if (Weapon::spawn_bullet(objects::rocket_bullet_128,
                                               cannon_pos.x,
                                               cannon_pos.y,
                                               mCar->mCarSprite->mZpos,
                                               cannon_angle,
                                               velocity))
            {
                if (mpPed->PedTypeIs(ped_type::player_2))
                {
                    decrement_ammo();
                }
            }
            mReloadTimer = 50;
            mpPed->AddThreateningPedToList();
            if (mpPed->is_player())
            {
                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
            }
        }
        else
        {
            Weapon::spawn_bullet(objects::invisible_rocket_159,
                                        cannon_pos.x,
                                        cannon_pos.y,
                                        mCar->mCarSprite->mZpos,
                                        cannon_angle,
                                        velocity);
            mReloadTimer = 5;
        }
        Weapon::ApplyFastReload();
    }
    else
    {
        --mReloadTimer;
    }
}

MATCH_FUNC(0x5e13e0)
void Weapon::army_gun_jeep()
{
    Ang16 gun_ang;
    Fix16_Point bullet_pos;
    Fix16_Point muzzle_offset;
    Fix16_Point muzzle_vel;
    if (mReloadTimer == 0)
    {
        mpPed = mCar->GetDriver();

        gun_ang = mCar->mAttachments.GetSpriteForModel(248)->mpSprite->mRotation;

        bullet_pos.SetXY(Fix16(0), kFpThreeThirtySeconds);
        {
            // RotateByAngle_MixOOL_40F6B0 written out, y * sin with the left-wide multiply
            Fix16 x_old = bullet_pos.x;
            Fix16 sin = Ang16::sine(gun_ang);
            Fix16 cos = Ang16::cosine(gun_ang);
            bullet_pos.x = bullet_pos.x.Multiply(cos).Add(MultiplyLeftWide(bullet_pos.y, sin));
            bullet_pos.y = x_old.Negated().Multiply(sin).Add(bullet_pos.y.Multiply(cos));
        }

        muzzle_offset.SetXY(Fix16(0), kFpMinusNineThirtySeconds);
        RotateByAngle_40F6B0_no_budget3(muzzle_offset, mCar->mCarSprite->mRotation);

        bullet_pos += muzzle_offset.Add(mCar->mCarSprite->get_x_y());

        muzzle_vel = mCar->mPhysics->GetPointVelocity(&bullet_pos);

        SetShotFired(true);

        if (!mbTargetHit)
        {
            if (spawn_bullet(objects::machine_gun_bullet_254,
                                    bullet_pos.x,
                                    bullet_pos.y,
                                    mCar->mCarSprite->mZpos,
                                    gun_ang,
                                    muzzle_vel))
            {
                if (mpPed->PedTypeIs(ped_type::player_2) && !is_infinite_ammo())
                {
                    mAmmo--;
                }
            }
            mReloadTimer = 2;

            mpPed->AddThreateningPedToList();
            if (mpPed->mPlayer)
            {
                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
            }
        }
        else
        {
            spawn_bullet(objects::invisible_bullet_154,
                                bullet_pos.x,
                                bullet_pos.y,
                                mCar->mCarSprite->mZpos,
                                gun_ang,
                                muzzle_vel);
            mReloadTimer = 1;
        }
        ApplyFastReload();
    }
    else
    {
        mReloadTimer--;
    }
}

// https://decomp.me/scratch/75934
MATCH_FUNC(0x5e1dc0)
void Weapon::oil_stain()
{
    Fix16_Point vector;
    Sprite* pCarSprite = mCar->GetSprite();
    vector.y = (-(pCarSprite->mpCollisionShape->GetHeight() + kFpThreeEighths)) / kFP16Two_706EC0;

    if (get_ammo_clips() % 2 != 0)
    {
        vector.x = -kFpFiveThirtySeconds;
    }
    else
    {
        vector.x = kFpFiveThirtySeconds;
    }

    vector.RotateByAngle(pCarSprite->mRotation);
    vector += pCarSprite->get_x_y_443580();

    Fix16 half_depth = pCarSprite->mpCollisionShape->GetDepth() / 2;
    Fix16 zpos_lower = pCarSprite->mZpos - half_depth;
    Fix16 zpos_upper = pCarSprite->mZpos + half_depth;

    if (zpos_upper >= kFpEight_706EDC)
    {
        zpos_upper = kFpEight_706EDC - kFpOne64th_706F70;
    }
    Fix16 found_z;
    if (gMap_0x370->CanPlaceOilOrMine(vector.x, vector.y, zpos_lower, zpos_upper, &found_z))
    {
        gObject_5C->NewPhysicsObj(objects::animating_oil_8, vector.x, vector.y, found_z, pCarSprite->mRotation);
        decrement_ammo();
        SetShotFired(true);
    }
}

MATCH_FUNC(0x5e2550)
void Weapon::car_mine()
{
    Fix16_Point p;

    mpPed = mCar->GetDriver();

    Sprite* pCarSprite = mCar->GetSprite();

    p.y = -(kFpOne64th_706FF4 + (kFpNineThirtySeconds + pCarSprite->mpCollisionShape->GetHeight()) / kFP16Two_706EC0);
    p.x = kFpZero_706EB8;

    p.RotateByAngle(pCarSprite->mRotation);

    p += pCarSprite->get_x_y();

    Fix16 half_depth = pCarSprite->mpCollisionShape->mDepth / 2;
    Fix16 z_low = pCarSprite->mZpos - half_depth;
    Fix16 z_high = pCarSprite->mZpos + half_depth;
    if (z_high >= kFpEight_706EDC)
    {
        z_high = kFpEight_706EDC - kFpOne64th_706F70;
    }

    Fix16 newZ;
    if (gMap_0x370->CanPlaceOilOrMine(p.x, p.y, z_low, z_high, &newZ))
    {
        Object* pMine = gObject_5C->NewPhysicsObj(objects::mine_10, p.x, p.y, newZ, pCarSprite->mRotation);
        pMine->SetDamageOwner(mpPed->mPedRefIdx);

        decrement_ammo();
        SetShotFired(true);
    }
}

// 9.6f 0x4D0230
// 10.5 https://decomp.me/scratch/odtu0
MATCH_FUNC(0x5e2940)
void Weapon::car_smg()
{
    // The original has a fifth destructible local up front (EH entry state 4), hence unused_5.
    // Declaration order and the Ang16 copy below are needed to match. gun_offset_x/gun_offset_y are initialised where
    // they are computed: declared up front and assigned, the caller is too big for the rotations'
    // inline budget (the first + of each RotateByAngle is inlined, its other operators aren't).
    Ang16 sprite_ang;
    Fix16_Point unused_5;
    Fix16_Point left;
    Fix16_Point right;
    Fix16_Point left_point_velocity;
    Fix16_Point right_point_velocity;
    if (mReloadTimer == 0)
    {
        mpPed = mCar->GetDriver();

        Sprite* pCarSprite = mCar->mCarSprite;
        sprite_ang = mCar->mCarSprite->mRotation;

        Fix16 gun_offset_x = kFpMinusThreeSixtyFourths + mCar->GetCarWidth() / 2;
        Fix16 gun_offset_y = kFpOne64th_706FD0 + mCar->GetCarHeight() / 2;

        left.SetXY(gun_offset_x, gun_offset_y);
        left.RotateByAngle(sprite_ang);
        left += pCarSprite->get_x_y_443580();

        right.SetXY(-gun_offset_x, gun_offset_y);
        right.RotateByAngle(sprite_ang);
        right += pCarSprite->get_x_y_443580();

        left_point_velocity = mCar->mPhysics->GetPointVelocity(&left);
        right_point_velocity = mCar->mPhysics->GetPointVelocity(&right);

        if (!mbTargetHit)
        {
            Object* pLeft = spawn_bullet(objects::machine_gun_bullet_254,
                                                   left.x,
                                                   left.y,
                                                   pCarSprite->mZpos,
                                                   sprite_ang,
                                                   left_point_velocity);
            Object* pRight = spawn_bullet(objects::machine_gun_bullet_254,
                                                    right.x,
                                                    right.y,
                                                    pCarSprite->mZpos,
                                                    sprite_ang,
                                                    right_point_velocity);
            if ((pLeft || pRight) && mpPed->PedTypeIs(ped_type::player_2))
            {
                DecreaseAmmo();
            }

            gParticle_8->GunMuzzelFlash(mCar->mCarSprite);

            mReloadTimer = 2;

            mpPed->AddThreateningPedToList();
            if (mpPed->is_player())
            {
                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
            }
        }
        else
        {
            Ang16 tmp = sprite_ang;
            spawn_bullet(objects::invisible_bullet_154, left.x, left.y, pCarSprite->mZpos, tmp, left_point_velocity);
            spawn_bullet(objects::invisible_bullet_154,
                                right.x,
                                right.y,
                                pCarSprite->mZpos,
                                sprite_ang,
                                right_point_velocity);
            mReloadTimer = 1;
        }

        ApplyFastReload();
        SetShotFired(true);
    }
    else
    {
        mReloadTimer--;
    }
}

MATCH_FUNC(0x5e33c0)
bool Weapon::CanFireWhileHeld() const
{
    bool result;
    switch (this->mWeaponType)
    {
        case weapon_type::car_bomb:
        case weapon_type::oil_stain:
        case weapon_type::car_mines:
        case weapon_type::tank_main_gun:
        case weapon_type::weapon_0x17:
            result = false;
            break;
        default:
            result = true;
            break;
    }
    return result;
}

MATCH_FUNC(0x5e34b0)
void Weapon::ChuckThrowable()
{
    if (mReloadTimer > 0)
    {
        mReloadTimer--;
    }
    else
    {
        if (mpPed)
        {
            if (mpPed->mPlayer)
            {
                if (mWeaponType == weapon_type::molotov || mWeaponType == weapon_type::grenade)
                {
                    s32 obj_type = (mWeaponType != weapon_type::molotov ? objects::grenade_obj_183 : objects::moving_molotov_138);
                    if (mpPed->mPlayer->IsThrowCharging())
                    {
                        s32 throw_strength = mpPed->mPlayer->GetThrowStrength();
                        s32 throw_charge = mpPed->mPlayer->GetThrowCharge();
                        throwable(obj_type, throw_strength, throw_charge);
                    }
                    mpPed->mPlayer->ResetThrowCharge();
                }
            }
        }
    }
}

MATCH_FUNC(0x5e3670)
void Weapon::pull_trigger()
{
    switch (mWeaponType)
    {
        case weapon_type::pistol:
            pistol();
            break;

        case weapon_type::smg:
        case weapon_type::silence_smg:
            smg();
            break;

        case weapon_type::rocket:
            rocket();
            break;

        case weapon_type::car_bomb:
            car_bomb(false);
            break;

        case weapon_type::oil_stain:
            oil_stain();
            break;

        case weapon_type::car_mines:
            car_mine();
            break;

        case weapon_type::tank_main_gun:
            tank_main_gun();
            break;

        case weapon_type::army_gun_jeep:
            army_gun_jeep();
            break;

        case weapon_type::electro_batton:
            electro_batton();
            break;

        case weapon_type::shocker:
            shocker();
            break;

        case weapon_type::molotov:
            if (mpPed && mpPed->mPlayer)
            {
                mpPed->mPlayer->IncrementThrowCharge();
            }
            else
            {
                throwable(objects::moving_molotov_138, 0x1E, 45);
            }
            break;

        case weapon_type::grenade:
            if (mpPed && mpPed->mPlayer)
            {
                Player* p = mpPed->mPlayer;
                p->IncrementThrowCharge();

                // This is really whacky, using p results in most of these inlines being optimized out
                Player* pp = mpPed->mPlayer;

                if (pp->GetThrowCharge() == 0x60)
                {
                    throwable(objects::grenade_obj_183, mpPed->mPlayer->GetThrowStrength(), pp->GetThrowCharge());
                    this->mpPed->mPlayer->mThrowCharge = -1;
                }
            }
            else
            {
                throwable(objects::grenade_obj_183, 0x1E, 45);
            }
            break;

        case weapon_type::dual_pistol:
            dual_pistol();
            break;

        case weapon_type::shotgun:
            shotgun();
            break;

        case weapon_type::car_smg:
            car_smg();
            break;

        case weapon_type::flamethrower:
            flamethrower();
            break;

        case weapon_type::fire_truck_gun:
            fire_truck_gun();
            break;

        case weapon_type::fire_truck_flamethrower:
            fire_truck_flamethrower();
            break;

        case weapon_type::weapon_0x17:
            car_bomb(true);
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5e3850)
void Weapon::rocket()
{
    if (mReloadTimer == 0)
    {
        SetShotFired(true);

        if (!mbTargetHit)
        {
            Object* pBullet;
            if (mpPed->PedTypeIs(ped_type::player_2))
            {
                pBullet = spawn_bullet(objects::rocket_bullet_128,
                                              mpPed->get_cam_x(),
                                              mpPed->get_cam_y(),
                                              mpPed->get_cam_z(),
                                              mpPed->GetAimAngle(),
                                              mpPed->GetVelocityVector());
            }
            else
            {
                if (!mbAiTestShotFired)
                {
                    spawn_bullet(objects::invisible_rocket_159,
                                        mpPed->get_cam_x(),
                                        mpPed->get_cam_y(),
                                        mpPed->get_cam_z(),
                                        mpPed->GetAimAngle(),
                                        mpPed->GetVelocityVector());
                    mReloadTimer = 5;
                    mbAiTestShotFired = 1;
                    return;
                }

                pBullet = spawn_bullet(objects::rocket_bullet_128,
                                              mpPed->get_cam_x(),
                                              mpPed->get_cam_y(),
                                              mpPed->get_cam_z(),
                                              mpPed->GetAimAngle(),
                                              mpPed->GetVelocityVector());
            }

            // People get scared when someone starts firing off rockets
            mpPed->AddThreateningPedToList();

            if (pBullet)
            {
                if (mpPed->PedTypeIs(ped_type::player_2))
                {
                    decrement_ammo();
                }
            }

            mReloadTimer = 50;
            gParticle_8->GunMuzzelFlash(mpPed->mGameObject->mSpritePtr);
            mbAiTestShotFired = 0;

            if (mpPed->is_player())
            {
                gCrimeReportQueue->ReportCrimeForPed(crime_stats_type::weapon_fired_2, mpPed);
            }
        }
        else
        {
            spawn_bullet(objects::invisible_rocket_159,
                                mpPed->get_cam_x(),
                                mpPed->get_cam_y(),
                                mpPed->get_cam_z(),
                                mpPed->GetAimAngle(),
                                mpPed->GetVelocityVector());
            mReloadTimer = 5;
            mbAiTestShotFired = 0;
        }
        ApplyFastReload();
    }
    else
    {
        if (!mbAiTestShotFired)
        {
            mpPed->mBf21C.bHoldPosition = false;
        }
        mReloadTimer--;
    }
}

MATCH_FUNC(0x5e3bd0)
bool Weapon::IsExplosiveWeapon() const
{
    bool result;
    switch (this->mWeaponType)
    {
        case weapon_type::rocket:
        case weapon_type::molotov:
        case weapon_type::car_bomb:
        case weapon_type::car_mines:
        case weapon_type::tank_main_gun:
            result = true;
            break;
        default:
            result = false;
            break;
    }
    return result;
}

// 9.6f 0x4CD8C0
MATCH_FUNC(0x5E3F90)
void Weapon::GetSoundPos(Fix16* pX, Fix16* pY, Fix16* pZ) const
{
    if (mpPed)
    {
        *pX = mpPed->get_cam_x();
        *pY = mpPed->get_cam_y();
        *pZ = mpPed->get_cam_z();
    }
    else if (mCar)
    {
        *pX = mCar->mCarSprite->GetXPos();
        *pY = mCar->mCarSprite->GetYPos();
        *pZ = mCar->mCarSprite->GetZPos();
    }
}