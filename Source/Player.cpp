#include "Player.hpp"
#include "car_shop_type.hpp"
#include "hud_message_priority.hpp"
#include "hud_brief_priority.hpp"
#include "voice_line.hpp"
#include "InputRecorder_67F8B0.hpp"
#include "input_bits.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "RouteFinder.hpp"
#include "Firefighters.hpp"
#include "CarAI.hpp"
#include "Char_Pool.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "NetPlay.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Police.hpp"
#include "Weapon.hpp"
#include "WeaponManager.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "ScriptManager.hpp"
#include "SoundObject.hpp"
#include "GameSession.hpp"
#include "map_0x370.hpp"
#include "registry.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "TextTable.hpp"
#include "winmain.hpp"
#include "TagGame.hpp"
#include "PlayerCrimeStats.hpp"
#include <DINPUT.H>

// Forward declarations: the functions below are in address order
EXTERN_GLOBAL(s16, gTestRouteIdx_675F14);
EXTERN_GLOBAL(Car*, gTestFireCar_675C30);

EXTERN_GLOBAL(bool, gCheatUnlimitedFlameThrower_67D6CC);
EXTERN_GLOBAL(bool, gCheatInvisibility_67D539);
EXTERN_GLOBAL(bool, gCheatUnlimitedDoubleDamage_67D57C);

DEFINE_GLOBAL_INIT(Fix16, kZero_6FE610, Fix16(0), 0x6FE610);
DEFINE_GLOBAL_INIT(Fix16, kTwo_6FE618, Fix16(2), 0x6FE618);
DEFINE_GLOBAL_INIT(Fix16, kZero_6FE41C, kZero_6FE610, 0x6FE41C);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_6FE754, Ang16(0), 0x6FE754);
DEFINE_GLOBAL_INIT(Ang16, kJumpTurnAccel_6FE74C, Ang16(4), 0x6FE74C);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng3, Ang16(12), 0x6FE750);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAttackTurnAccel, Ang16(12), 0x6FE488);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kMaxJumpTurnSpeed, Ang16(24), 0x6FE450);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kMaxTurnSpeed, Ang16(48), 0x6FE700);
DEFINE_GLOBAL_INIT(Fix16, kOne_6FE614, Fix16(1), 0x6FE614);
DEFINE_GLOBAL_INIT(Fix16, k_instant_gang_radius_6FE634, Fix16(8), 0x6FE634);

struct DebugTest
{
    EXPORT void DoTest(u16 action);
    EXPORT void DoBrianTest(u16 action);
};

DEFINE_GLOBAL(Car*, gTestArrowCar_7043F0, 0x7043F0);

MATCH_FUNC(0x42D870)
void DebugTest::DoBrianTest(u16 action)
{
    switch (action)
    {
        case 8:
            gGame_0x40_67E008->ExitGame(0, GameExitType::AreaCompleted_4, 3);
            break;
        case 11:
            gGame_0x40_67E008->ExitGameNoBonus(0, GameExitType::GameOverRIP_3);
            break;
        case 9:
            gScriptManager_6F8060->SaveGame(gGameSession_67E8E0.GetSaveFileName_4C5970());
            break;
        case 6:
            gTestRouteIdx_675F14 = gRouteFinder_6FFDC8->DoStartRoute(3, 3, 1, 0xD5, 0xAB, 1, 1);
            gRouteFinder_6FFDC8->CancelRoute(gTestRouteIdx_675F14);
            gTestRouteIdx_675F14 = gRouteFinder_6FFDC8->DoStartRoute(0xD5, 0xF9, 1, 3, 3, 1, 1);
            break;
        case 7:
        {
            Ped* pPed = gGame_0x40_67E008->IterateFirstPlayer()->GetPlayerPed_41D020();
            if (pPed->mCar)
            {
                pPed->mCar->GoToBlockTest(3, 3, 1, 1);
                pPed->mCar->InitCarAIControl();
                gRouteFinder_6FFDC8->DebugPrintRoute(pPed->mCar->mAI->mRouteIdx);
            }
            break;
        }
        case 10:
            gFirefighterPool_54_67D4C0->TryDispatchFirefightersToCar(gTestFireCar_675C30);
            break;
    }
}

DEFINE_GLOBAL(s16, gTestRouteIdx_675F14, 0x675F14);
DEFINE_GLOBAL(Car*, gTestFireCar_675C30, 0x675C30);

MATCH_FUNC(0x443CB0)
EXPORT s32 Player::ObjectTypeToWeaponType(u8 shop_type)
{
    s32 weapon_kind;
    switch (shop_type)
    {
        case car_shop_type::mines_252:
            weapon_kind = weapon_type::car_mines;
            break;
        case car_shop_type::oil_stain_251:
            weapon_kind = weapon_type::oil_stain;
            break;
        case car_shop_type::car_smg_250:
            weapon_kind = weapon_type::car_smg;
            break;
        case car_shop_type::change_plates_253:
        case car_shop_type::car_bomb_254:
            weapon_kind = weapon_type::car_bomb;
            break;
        default:
            weapon_kind = weapon_type::car_bomb;
            break;
    }
    return weapon_kind;
}

MATCH_FUNC(0x4881E0)
u8 Player::GetIdx()
{
    return mPlayerIdx;
}

MATCH_FUNC(0x5645B0)
void Player::AddCarToHistory(Car* pNewCar)
{
    Car** pIter = mCarHistory;
    if (!bStartNetworkGame_7081F0)
    {
        u8 i = PromoteCarInHistory(pNewCar, false);
        if (!i)
        {
            for (; i < 3; pIter++, i++)
            {
                if (!*pIter)
                {
                    *pIter = pNewCar;
                    return;
                }
            }

            // History full: recycle the oldest car and shift the others down. The same pointer is
            // reset here: VC6 then keeps the array address in esi and the walking pointer in edi, a
            // second pointer variable swaps them (VC7 against 9.6f too).
            pIter = mCarHistory;
            (*pIter)->MarkRecycled();
            for (u8 j = 0; j < 2; j++, pIter++)
            {
                *pIter = pIter[1];
            }
            *pIter = pNewCar;
        }
    }
}

MATCH_FUNC(0x564610)
bool Player::PromoteCarInHistory(Car* pCar, bool dont_modify)
{
    Car** ppCarIter = &mCarHistory[1];
    if (!bStartNetworkGame_7081F0)
    {
        if (pCar == mCarHistory[2]) // check 2
        {
            return true;
        }

        for (char_type count = 1; count >= 0; count--)
        {
            if (*ppCarIter == pCar)
            {
                Car** pCarValue = ppCarIter + 1;
                if (count < 2)
                {
                    // shift the array
                    s32 subCount = 2 - count;
                    do
                    {
                        if (!dont_modify)
                        {
                            *ppCarIter++ = *pCarValue++;
                        }
                        --subCount;
                    } while (subCount);
                }
                if (!dont_modify)
                {
                    *ppCarIter = pCar;
                }
                return true;
            }
            *ppCarIter--;
        }
    }
    return false;
}

MATCH_FUNC(0x564680)
void Player::RemoveCarFromHistory(Car* pCar)
{
    Car** pIter = mCarHistory;
    if (!bStartNetworkGame_7081F0)
    {
        u8 idx;
        for (idx = 0; idx < 2; pIter++, idx++)
        {
            if (*pIter == pCar)
            {
                *pIter = pIter[1];
                for (; idx < 2; idx++, pIter++)
                {
                    *pIter = pIter[1];
                }
                *pIter = NULL;
                return;
            }
        }

        Car** pCurrent = &mCarHistory[idx];
        if (*pCurrent == *pIter)
        {
            *pCurrent = NULL;
        }
    }
}

MATCH_FUNC(0x564710)
void Player::SetKFCarWeapon(Car* pCar, s32 weapon_kind)
{
    this->mPreKfWeaponKind = this->mCurrWeaponIdx;

    Weapon* pWeapon = gWeapon_8_707018->find(pCar, weapon_kind);
    if (pWeapon)
    {
        this->mPreKfAmmo = pWeapon->get_ammo_4A4FE0();
    }
    else
    {
        this->mPreKfAmmo = 0;
        pWeapon = gWeapon_8_707018->allocate_5E3CE0(weapon_kind, pCar, 0);
    }

    this->mKfWeaponKind = weapon_kind;
    this->mKfCar = pCar;
    this->mKfCarId = pCar->mCarId;

    pWeapon->set_infinite_ammo_4A4F90();

    Ped* pDriver = pCar->mDriver;
    if (pDriver)
    {
        if (pDriver->mPlayer == this)
        {
            this->mWeapons[weapon_kind] = pWeapon;
            this->mCurrWeaponIdx = this->mKfWeaponKind;
        }
    }
}

MATCH_FUNC(0x564790)
void Player::SetKFWeapon(s32 weapon_idx)
{
    this->mPreKfWeaponKind = this->mCurrWeaponIdx;
    this->mKfWeaponKind = weapon_idx;
    this->mPreKfAmmo = this->mWeapons[weapon_idx]->get_ammo_4A4FE0();
    this->mWeapons[weapon_idx]->set_infinite_ammo_4A4F90();
    this->mCurrWeaponIdx = this->mKfWeaponKind;
    EnableKFMode();
}

MATCH_FUNC(0x5647D0)
void Player::ClearKFWeapon()
{
    if (this->mPreKfWeaponKind != -2)
    {
        if (gWeapon_8_707018->is_car_weapon_433820(mKfWeaponKind))
        {
            if (mKfCar->mCarId == this->mKfCarId)
            {
                Weapon* pWeapon = gWeapon_8_707018->find(mKfCar, this->mKfWeaponKind);
                pWeapon->SetAmmo_4A4FF0(this->mPreKfAmmo);
                if (!this->mPreKfAmmo)
                {
                    gWeapon_30_Pool_707014->DeAllocate_4A4F20(pWeapon);
                    Ped* pDriver = this->mKfCar->mDriver;
                    if (pDriver)
                    {
                        if (pDriver->mPlayer == this)
                        {
                            this->mWeapons[this->mKfWeaponKind] = NULL;
                            this->mCurrWeaponIdx = this->mPreKfWeaponKind;
                        }
                    }
                }
            }
        }
        else
        {
            if (gCheatUnlimitedElectroGun_67D4F7 && mKfWeaponKind == weapon_type::shocker ||
                gCheatUnlimitedFlameThrower_67D6CC && mKfWeaponKind == weapon_type::flamethrower)
            {
                this->mPreKfWeaponKind = -2;
                return;
            }
            else
            {
                DisableKFMode();
                this->mWeapons[this->mKfWeaponKind]->SetAmmo_4A4FF0(this->mPreKfAmmo);
                this->mCurrWeaponIdx = this->mPreKfWeaponKind;
            }
        }
        this->mPreKfWeaponKind = -2;
    }
}

MATCH_FUNC(0x5648F0)
Weapon* Player::GetCurrPlayerWeapon()
{
    return mCurrWeaponIdx == -1 ? NULL : mWeapons[mCurrWeaponIdx];
}

MATCH_FUNC(0x564910)
void Player::SetWeapon(Weapon* pWeapon)
{
    s16 idx = pWeapon->mWeaponType;
    this->mWeapons[idx] = pWeapon;
    if (!this->mKfWeaponMode)
    {
        this->mCurrWeaponIdx = idx;
    }
}

MATCH_FUNC(0x564940)
bool Player::HasAnyAmmo()
{
    for (s32 i = 0; i < 15; i++)
    {
        if (mWeapons[i]->mAmmo)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x564960)
bool Player::AddWeaponWithAmmo(s32 weapon_kind, u8 ammo)
{
    bool bHasAnyAmmo;
    bool bAmmoAdded;

    if (GetPlayerPed_4A5130()->get_car_416B60())
    {
        bHasAnyAmmo = false;
    }
    else
    {
        bHasAnyAmmo = !HasAnyAmmo();
    }

    bAmmoAdded = mWeapons[weapon_kind]->add_ammo_capped(ammo);
    if (!this->mKfWeaponMode)
    {
        if (bAmmoAdded)
        {
            if (bHasAnyAmmo)
            {
                this->mCurrWeaponIdx = weapon_kind;
            }
        }
    }

    return bAmmoAdded;
}

MATCH_FUNC(0x5649D0)
void Player::SelectNextOrPrevWeapon(bool go_forwards, bool go_backwards)
{
    Ped* pPlayerPed = GetPlayerPed_4A5130();

    if (!mKfWeaponMode)
    {
        s32 max_idx;
        if (!pPlayerPed || !pPlayerPed->get_car_416B60())
        {
            max_idx = weapon_type::car_bomb; // 15
        }
        else
        {
            max_idx = 28;
        }

        if (mCurrWeaponIdx < -1 || mCurrWeaponIdx >= max_idx)
        {
            mCurrWeaponIdx = 0;
        }

        Weapon* pWeapon;

        if (go_forwards)
        {
            while (1)
            {
                if (++mCurrWeaponIdx == max_idx)
                {
                    mCurrWeaponIdx = -1;
                }
                if (mCurrWeaponIdx == -1)
                {
                    break;
                }
                pWeapon = mWeapons[mCurrWeaponIdx];
                if (pWeapon)
                {
                    if (pWeapon->HasAmmo_4A4F80())
                    {
                        break;
                    }
                }
            }
        }

        if (!go_backwards)
        {
            if (mCurrWeaponIdx != -1)
            {
                pWeapon = mWeapons[mCurrWeaponIdx];
                if (!pWeapon || !pWeapon->HasAmmo_4A4F80())
                {
                    mBlockAttack = true;
                }
                else
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }
        while (1)
        {
            s16 dec_idx;
            if (mCurrWeaponIdx == -1)
            {
                dec_idx = max_idx - 1;
            }
            else
            {
                dec_idx = mCurrWeaponIdx - 1;
            }
            mCurrWeaponIdx = dec_idx;
            if (dec_idx == -1)
            {
                break;
            }
            pWeapon = mWeapons[dec_idx];
            if (pWeapon)
            {
                if (pWeapon->HasAmmo_4A4F80())
                {
                    break;
                }
            }
        }
    }
}

MATCH_FUNC(0x564AD0)
void Player::LoadCarWeapons(Car* pCar)
{
    s16 last_weapon_kind = -1;
    for (s32 weapon_kind = weapon_type::car_bomb; weapon_kind < 28; weapon_kind++)
    {
        Weapon* pWeapon = gWeapon_8_707018->find(pCar, weapon_kind);
        mWeapons[weapon_kind] = pWeapon;
        if (pWeapon)
        {
            last_weapon_kind = weapon_kind;
        }
    }

    if (!this->mKfWeaponMode)
    {
        this->mSavedPedWeaponIdx = this->mCurrWeaponIdx;
        if (this->mWeapons[mSavedCarWeaponIdx])
        {
            this->mCurrWeaponIdx = mSavedCarWeaponIdx;
        }
        else if (last_weapon_kind != -1)
        {
            this->mCurrWeaponIdx = last_weapon_kind;
        }
        SelectNextOrPrevWeapon(false, false);
    }
}

MATCH_FUNC(0x564B60)
void Player::ClearCarWeapons()
{
    for (u32 i = 15; i < GTA2_COUNTOF(mWeapons); i++)
    {
        mWeapons[i] = NULL;
    }
}

MATCH_FUNC(0x564B80)
void Player::CleanupEmptyAmmoWeapons()
{
    for (s32 i = 15; i < 28; i++)
    {
        if (mWeapons[i])
        {
            if (!mWeapons[i]->mAmmo)
            {
                gWeapon_30_Pool_707014->DeAllocate_4A4F20(mWeapons[i]);
            }
        }
        mWeapons[i] = NULL;
    }
}

MATCH_FUNC(0x564C00)
void Player::UnloadCarWeapons()
{
    CleanupEmptyAmmoWeapons();

    if (mCurrWeaponIdx >= weapon_type::car_bomb)
    {
        mSavedCarWeaponIdx = mCurrWeaponIdx;
        mCurrWeaponIdx = mSavedPedWeaponIdx;
    }

    if (mPreKfWeaponKind >= weapon_type::car_bomb)
    {
        mSavedCarWeaponIdx = mPreKfWeaponKind;
        mPreKfWeaponKind = mSavedPedWeaponIdx;
    }

    SelectNextOrPrevWeapon(false, false);
}

MATCH_FUNC(0x564C50)
void Player::RemovePlayerWeapons()
{
    s32 i = 15;
    Weapon* pWeapon = this->mWeapons[0];
    do
    {
        if ((!gCheatUnlimitedElectroGun_67D4F7 || pWeapon->mWeaponType != weapon_type::shocker) &&
            (!gCheatUnlimitedFlameThrower_67D6CC || pWeapon->mWeaponType != weapon_type::flamethrower))
        {
            if (pWeapon->HasAmmo_4A4F80())
            {
                pWeapon->set_ammo_clips(0);
            }
        }
        ++pWeapon;
        --i;
    } while (i);

    s16 idx = this->mCurrWeaponIdx;
    if (idx < 15)
    {
        this->mSavedPedWeaponIdx = idx;
        this->mCurrWeaponIdx = 0;
    }
    SelectNextOrPrevWeapon(false, false);
}

MATCH_FUNC(0x564CC0)
void Player::ClearPowerUps()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(mPowerUpTimers); i++)
    {
        if (i == power_up_indices::Invisibility_11 && gCheatInvisibility_67D539)
        {
            continue;
        }

        if (i == power_up_indices::DoubleDamage_7 && gCheatUnlimitedDoubleDamage_67D57C)
        {
            continue;
        }

        mPowerUpTimers[i] = 0;
    }
}

MATCH_FUNC(0x564CF0)
void Player::ClearPowerUpsExceptJailCard()
{
    u16 jail_card_timer = mPowerUpTimers[power_up_indices::JailCard_4];
    if (mPowerUpTimers[power_up_indices::Invulnerability_6])
    {
        mPlayerPed->ClearInvulnerable();
    }
    if (mPowerUpTimers[power_up_indices::Electrofingers_9])
    {
        mPlayerPed->ClearElectrofingers_482080();
    }
    if (mPowerUpTimers[power_up_indices::Invisibility_11])
    {
        if (!gCheatInvisibility_67D539)
        {
            mPlayerPed->SetVisible();
        }
    }
    ClearPowerUps();
    mPowerUpTimers[power_up_indices::JailCard_4] = jail_card_timer;
}

MATCH_FUNC(0x564D60)
bool Player::CollectPowerUp(s32 power_up_idx)
{
    switch (power_up_idx)
    {
        case power_up_indices::Multiplier_0:
            if (mMultipliers.get_value() == 99)
            {
                return false;
            }
            ChangeMultipliers_4766B0(1);
            break;

        case power_up_indices::ExtraLife_1:
            if (this->mLives.get_value() == 99)
            {
                return false;
            }
            ChangeLifeCountByAmount(1);
            break;

        case power_up_indices::Health_2:
            if (mPlayerPed->get_health_433B70() >= 100)
            {
                return false;
            }
            mPlayerPed->SetFullHealth_4A5050();
            break;

        case power_up_indices::Armor_3:
            if (this->mPowerUpTimers[power_up_indices::Armor_3] == 10)
            {
                return false;
            }
            this->mPowerUpTimers[power_up_indices::Armor_3] = 10;
            break;

        case power_up_indices::CopBribe_5:
            if (mPlayerPed->get_wanted_points_433DC0() == 0)
            {
                return false;
            }
            mPlayerPed->ClearWantedPoints_420B80();
            break;

        case power_up_indices::Invulnerability_6:
            if (this->mPowerUpTimers[power_up_indices::Invulnerability_6] == 1200)
            {
                return false;
            }
            this->mPowerUpTimers[power_up_idx] = 1200;
            mPlayerPed->SetInvulnerable();
            break;

        case power_up_indices::DoubleDamage_7:
            if (this->mPowerUpTimers[power_up_indices::DoubleDamage_7] == 1800)
            {
                return false;
            }
            this->mPowerUpTimers[power_up_indices::DoubleDamage_7] = 1800;
            break;

        case power_up_indices::FastReload_8:
            if (this->mPowerUpTimers[power_up_indices::FastReload_8] == 1800)
            {
                return false;
            }
            this->mPowerUpTimers[power_up_indices::FastReload_8] = 1800;
            break;

        case power_up_indices::Electrofingers_9:
            if (this->mPowerUpTimers[power_up_indices::Electrofingers_9] == 2100)
            {
                return false;
            }
            this->mPowerUpTimers[power_up_indices::Electrofingers_9] = 2100;
            this->mPlayerPed->SetElectrofingers_4A5060();
            break;

        case power_up_indices::Invisibility_11:
            if (this->mPowerUpTimers[power_up_indices::Invisibility_11] == 1800)
            {
                return false;
            }
            this->mPowerUpTimers[power_up_idx] = 1800;
            mPlayerPed->SetInvisible();
            break;

        case power_up_indices::Respect_10:
            if (!mGangCurrLocation)
            {
                return false;
            }
            if (mGangCurrLocation->GetRespectForPlayer(this->mPlayerIdx) == 100)
            {
                return false;
            }
            mGangCurrLocation->IncrementRespect(this->mPlayerIdx, 20);
            break;

        case power_up_indices::InstantGang_12:
            if (!mPlayerPed->get_game_object_4A5020())
            {
                return false;
            }
            mPlayerPed->RecruitNearbyPeds(4, k_instant_gang_radius_6FE634);
            break;
        default:
            if (this->mPowerUpTimers[power_up_idx] == 1)
            {
                return false;
            }
            this->mPowerUpTimers[power_up_idx] = 1;
            break;
    }
    return true;
}

MATCH_FUNC(0x565070)
void Player::tick_down_powerups()
{
    if (mPowerUpTimers[power_up_indices::Invulnerability_6])
    {
        mPowerUpTimers[power_up_indices::Invulnerability_6]--;
        if (!mPowerUpTimers[power_up_indices::Invulnerability_6])
        {
            mPlayerPed->ClearInvulnerable();
        }
    }

    if (mPowerUpTimers[power_up_indices::DoubleDamage_7])
    {
        if (!gCheatUnlimitedDoubleDamage_67D57C)
        {
            mPowerUpTimers[power_up_indices::DoubleDamage_7]--;
        }
    }

    if (mPowerUpTimers[power_up_indices::FastReload_8])
    {
        mPowerUpTimers[power_up_indices::FastReload_8]--;
    }

    if (mPowerUpTimers[power_up_indices::Electrofingers_9])
    {
        mPowerUpTimers[power_up_indices::Electrofingers_9]--;
        if (!mPowerUpTimers[power_up_indices::Electrofingers_9])
        {
            mPlayerPed->ClearElectrofingers_482080();
        }
    }

    // invisiblity
    if (mPowerUpTimers[power_up_indices::Invisibility_11])
    {
        if (!gCheatInvisibility_67D539)
        {
            mPowerUpTimers[power_up_indices::Invisibility_11]--;
            if (!mPowerUpTimers[power_up_indices::Invisibility_11])
            {
                mPlayerPed->SetVisible();
            }
        }
    }
}

MATCH_FUNC(0x5651F0)
void Player::RestorePowerUpsFromSave(SavedPlayerStats* pSave)
{
    for (u8 idx = 0; idx < 17; idx++)
    {
        if (pSave->mPowerUps[idx] > 0)
        {
            mPowerUpTimers[idx] = pSave->mPowerUps[idx];
            switch (idx)
            {
                case power_up_indices::Invulnerability_6:
                    mPlayerPed->SetInvulnerable();
                    break;
                case power_up_indices::Electrofingers_9:
                    mPlayerPed->SetElectrofingers_4A5060();
                    break;
                case power_up_indices::Invisibility_11:
                    mPlayerPed->SetInvisible();
                    break;
            }
        }
    }
}

MATCH_FUNC(0x565310)
void Player::TeleportToDebugCam()
{
    mPlayerPed->TeleportToCoord(this->mViewCamera.mCurPos.mX,
                                                 this->mViewCamera.mCurPos.mY);
    memcpy(&this->mGameCamera, &this->mViewCamera, sizeof(this->mGameCamera));
    this->mInDebugCamMode = 0;
    this->mCameraMode = 0;
    this->mGameCamera.SetSmoothFollow_475B60();
    gHud_2B00_706620->mMessage.ShowMessage(gText_0x14_704DFC->Find("tport"), hud_message_priority::important_3);
}

MATCH_FUNC(0x5653E0)
void Player::DebugWatchNearestCar()
{
    Car* pCar = gCarManager_6C_677930->GetNearestEnterableCarFromCoord(mViewCamera.mCurPos.mX,
                                                                          mViewCamera.mCurPos.mY,
                                                                          mViewCamera.mCurPos.mZ,
                                                                          NULL);
    if (pCar)
    {
        ResetAuxCamera();
        WatchCar(pCar);
    }
}

MATCH_FUNC(0x565460)
void Player::OnVideoModeChange()
{
    // Empty
}

MATCH_FUNC(0x565490)
void Player::InitPlayerPed(Ped* pPed)
{
    mWastedOrBusted = false;
    mBusted = false;
    mAuxPedDying = false;
    mDeathCountdown = 0;
    mPlayerPed = pPed;
    mTurnSpeed = kAngZero_6FE754;
    mTurnAccel = kJumpTurnAccel_6FE74C + (kAng3 + kJumpTurnAccel_6FE74C);
    mMoveDirection = kZero_6FE610;
    mUnused10 = 0;
    mTrafficSpawnCounter = 0;
    mTrafficSpawnThreshold = 1000;
    pPed->SetPlayer(this, false);
    mCameraMode = 0;

    u8 ammo;

    for (s32 weapon_idx = weapon_type::pistol; weapon_idx < weapon_type::car_bomb; ++weapon_idx)
    {
        if (!bGet_all_weapons_67D684 || weapon_idx >= weapon_type::weapon_11 || weapon_idx == weapon_type::electro_batton)
        {
            ammo = 0;
        }
        else
        {
            ammo = gWeapon_8_707018->get_max_ammo_capacity(weapon_idx);
        }
        mWeapons[weapon_idx] = gWeapon_8_707018->allocate_5E3C10(weapon_idx, pPed, ammo);
    }

    Player::ClearCarWeapons();
    mCurrWeaponIdx = -1;
    mSavedPedWeaponIdx = -1;
    mSavedCarWeaponIdx = 27;
    Player::ClearPowerUps();
}

MATCH_FUNC(0x565740)
void Player::SetInputs(u32 inputs)
{
    mInputs = inputs;
}

MATCH_FUNC(0x565770)
void Player::IncrementGangRespectFromDebugKeys(u8 count)
{
    Gang* pZone = gGangPool_CA8_67E274->FirstGang();
    for (u8 i = count; i > 0; i--)
    {
        pZone = gGangPool_CA8_67E274->NextGang();
    }

    if (pZone)
    {
        if (pZone->GetRespectForPlayer(this->mPlayerIdx) >= 100)
        {
            pZone->SetRespect(this->mPlayerIdx, -100);
        }
        else
        {
            pZone->IncrementRespect(this->mPlayerIdx, 20);
        }
    }
}

MATCH_FUNC(0x565860)
void Player::IncreaseWantedLevelFromDebugKeys()
{
    mPlayerPed->IncreaseWantedLevelFromDebugKeys();
}

MATCH_FUNC(0x565890)
void Player::Hud_Controls(u16 action)
{
    LOCAL_STATIC(DebugTest*, sBrianTestObj, 0x675F74);
    LOCAL_STATIC(DebugTest*, sTestObj, 0x7044A0);
    s32 vol;

    if (!gHud_2B00_706620->HandleInputKey(action, this))
    {
        switch (action)
        {
            case DIK_ESCAPE:
                if (bSkip_quit_confirm_67D4E8)
                {
                    if (this->IsUser_41DC70())
                    {
                        gGame_0x40_67E008->ExitGameNoBonus(1, GameExitType::PlayerQuit_2);
                    }
                }
                else
                {
                    gHud_2B00_706620->mQuitMessage.ShowQuitMessage(this);
                }
                break;
            case DIK_HOME:
                if (bDo_debug_keys_67D6CF)
                {
                    mGameCamera.ResetElevation();
                    mAuxGameCamera.ResetElevation();
                }
                break;
            case DIK_NEXT:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mDbgZoomIn = true;
                }
                break;
            case DIK_PRIOR:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mDbgZoomOut = true;
                }
                break;
            case DIK_F1:
                gRoot_sound_66B038.CycleRadioStation(0);
                break;
            case DIK_F2:
                gRoot_sound_66B038.CycleRadioStation(1);
                break;

            // ordered above

            // ordered below
            case DIK_F3:
                if (gRoot_sound_66B038.GetCDVol() - 10 > 0)
                {
                    vol = gRoot_sound_66B038.GetCDVol() - 10;
                }
                else
                {
                    vol = 0;
                }
                gRoot_sound_66B038.SetCDVol(vol);
                gRegistry_6FF968.Set_Sound_Setting("CDVol", gRoot_sound_66B038.GetCDVol());
                break;
            case DIK_F4:
                if (gRoot_sound_66B038.GetCDVol() + 10 < 127)
                {
                    vol = gRoot_sound_66B038.GetCDVol() + 10;
                }
                else
                {
                    vol = 127;
                }
                gRoot_sound_66B038.SetCDVol(vol);
                gRegistry_6FF968.Set_Sound_Setting("CDVol", gRoot_sound_66B038.GetCDVol());
                break;
            case DIK_F6:
                gGame_0x40_67E008->TogglePause();
                break;
            case DIK_F7:
                if (this->IsUser_41DC70())
                {
                    gHud_2B00_706620->mBrief.ShowBrief();
                }
                break;
            case DIK_F8:
                if (this->IsUser_41DC70())
                {
                    Start_GTA2Manager();
                }
                break;
            case DIK_F9:
                if (this->IsUser_41DC70())
                {
                    gHud_2B00_706620->mZoneName.clear_zones();
                }
                break;
            case DIK_F10:
                if (bStartNetworkGame_7081F0)
                {
                    gHud_2B00_706620->mChatInput.StartChatting(this);
                }
                break;
            case DIK_F11:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mCameraMode = 0;
                    this->mGameCamera.SetSmoothFollow_475B60();
                }
                break;
            case DIK_F12:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mCameraMode = 1;
                    this->mGameCamera.SetSnapFollow_4A5070();
                }
                break;
            case DIK_ADD:
                if (bDo_debug_keys_67D6CF)
                {
                    gGame_0x40_67E008->SinglePlayerStepFrame();
                }
                break;
            case DIK_SUBTRACT:
                if (bDo_debug_keys_67D6CF)
                {
                    gGame_0x40_67E008->ToggleFastForward_4A4750();
                }
                break;
            case DIK_NUMPAD2:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mInDebugCamMode = 1;
                    this->mDbgCamSouth = true;
                }
                break;
            case DIK_NUMPAD8:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mInDebugCamMode = 1;
                    this->mDbgCamNorth = true;
                }
                break;
            case DIK_NUMPAD4:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mInDebugCamMode = 1;
                    this->mDbgCamWest = true;
                }
                break;
            case DIK_NUMPAD6:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mInDebugCamMode = 1;
                    this->mDbgCamEast = true;
                }
                break;
            case DIK_NUMPAD7:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mInDebugCamMode = 1;
                    this->mDbgCamZoomingOut = true;
                }
                break;
            case DIK_NUMPAD9:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mInDebugCamMode = 1;
                    this->mDbgCamZoomingIn = true;
                }
                break;
            case DIK_NUMPAD5:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mInDebugCamMode = 0;
                }
                break;
            case DIK_NUMPAD0:
                if (bDo_debug_keys_67D6CF)
                {
                    Player::TeleportToDebugCam(); // teleport?
                }
                break;
            case DIK_NUMPAD1:
                if (bDo_debug_keys_67D6CF)
                {
                    this->mDbgCamFollowPlayer = !this->mDbgCamFollowPlayer;
                }
                break;
            case DIK_DECIMAL:
                if (bDo_debug_keys_67D6CF)
                {
                    if (mPlayerPed)
                    {
                        mPlayerPed->RestoreCarOrPedHealth();
                    }
                }
                break;
            case DIK_DIVIDE:
                if (bDo_debug_keys_67D6CF)
                {
                    Player::DebugWatchNearestCar();
                }
                break;
            case DIK_PERIOD:
                if (bDo_debug_keys_67D6CF)
                {
                    gHud_2B00_706620->mBriefSelector.ShowNextNumberedBrief();
                }
                break;
            case DIK_COMMA:
                if (bDo_debug_keys_67D6CF)
                {
                    gHud_2B00_706620->mBriefSelector.ShowPrevNumberedBrief();
                }
                break;
            case DIK_C:
                if (bDo_debug_keys_67D6CF)
                {
                    if (this->IsUser_41DC70())
                    {
                        gHud_2B00_706620->mShowCoords.ToggleShowCoords_4A4760();
                    }
                }
                break;
            case DIK_1:
                if (bDo_debug_keys_67D6CF)
                {
                    Player::IncrementGangRespectFromDebugKeys(0);
                }
                break;
            case DIK_2:
                if (bDo_debug_keys_67D6CF)
                {
                    Player::IncrementGangRespectFromDebugKeys(1);
                }
                break;
            case DIK_3:
                if (bDo_debug_keys_67D6CF)
                {
                    Player::IncrementGangRespectFromDebugKeys(2);
                }
                break;

            case DIK_4:
                if (bDo_debug_keys_67D6CF)
                {
                    Player::IncreaseWantedLevelFromDebugKeys();
                }
                break;
            case DIK_5:
                if (bDo_debug_keys_67D6CF)
                {
                    if (mPlayerPed)
                    {
                        mPlayerPed->ClearWantedPoints_420B80();
                    }
                }
                break;
            case DIK_6:
            case DIK_7:
            case DIK_8:
            case DIK_9:
            case DIK_0:
                if (bDo_test_67D4F8)
                {
                    sTestObj->DoTest(action);
                }
                if (bDo_brian_test_67D544)
                {
                    sBrianTestObj->DoBrianTest(action);
                }
                else if (bDo_iain_test_67D4E9)
                {
                    gPedManager_6787BC->DoIanTest(action);
                }
                break;

            default:
                return;
        }
    }
}

MATCH_FUNC(0x566380)
void Player::HandleKeyRelease(u16 key)
{
    switch (key)
    {
        case DIK_NEXT:
            if (bDo_debug_keys_67D6CF)
            {
                this->mDbgZoomIn = false;
            }
            break;

        case DIK_PRIOR:
            if (bDo_debug_keys_67D6CF)
            {
                this->mDbgZoomOut = false;
            }
            break;
        case DIK_NUMPAD2:
            this->mDbgCamSouth = false;
            break;

        case DIK_NUMPAD8:
            this->mDbgCamNorth = false;
            break;

        case DIK_NUMPAD4:
            this->mDbgCamWest = false;
            break;

        case DIK_NUMPAD6:
            this->mDbgCamEast = false;
            break;

        case DIK_NUMPAD7:
            this->mDbgCamZoomingOut = false;
            break;

        case DIK_NUMPAD9:
            this->mDbgCamZoomingIn = false;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x566520)
void Player::CharacterControls()
{
    s32 input = this->mInputs;

    // up arrow
    bool forwardGas = (input & input_bit::Forward) == input_bit::Forward;
    bool same = (forwardGas == this->mNowForwardPressed);
    this->mForwardChanged = !same;
    this->mNowForwardPressed = forwardGas;

    // down arrow
    bool footBrake = (input & input_bit::Backward) == input_bit::Backward;
    same = (footBrake == this->mNowDownPressed);
    this->mDownChanged = !same;
    this->mNowDownPressed = footBrake;

    // left arrow
    // right arrow
    this->mNowLeftPressed = (input & input_bit::Left) == input_bit::Left;
    this->mNowRightPressed = (input & input_bit::Right) == input_bit::Right;

    // ctrl (attack)
    const bool ctrl10 = (input & input_bit::Attack) == input_bit::Attack;
    same = (ctrl10 == this->mNowAttackPressed);
    this->mAttackChanged = !same;
    this->mNowAttackPressed = ctrl10;

    // space (handbrake, jump)
    const bool prevHandBrake = this->mNowHandBrakeOrJumpPressed;
    const bool handBrake = (input & input_bit::HandbrakeJump) == input_bit::HandbrakeJump;
    this->mNowHandBrakeOrJumpPressed = handBrake;
    this->mHandBrakeOrJumpChanged = (handBrake != prevHandBrake);

    // enter (enter/exit car)
    const bool prev20 = this->mNowEnterExitPressed;
    const bool ctrl20 = (input & input_bit::EnterExit) == input_bit::EnterExit;
    this->mNowEnterExitPressed = ctrl20;
    this->mEnterExitChanged = (ctrl20 != prev20);

    // y (prev weapon)
    const bool prev80 = this->mNowPrevWeaponPressed;
    const bool ctrl80 = (input & input_bit::PrevWeapon) == input_bit::PrevWeapon;
    this->mNowPrevWeaponPressed = ctrl80;
    this->mPrevWeaponChanged = (ctrl80 != prev80);

    // x (next weapon)
    const bool prev100 = this->mNowNextWeaponPressed;
    this->mNowNextWeaponPressed = (input & input_bit::NextWeapon) == input_bit::NextWeapon;
    this->mNextWeaponChanged = (mNowNextWeaponPressed != prev100);

    // tab (hold for turret movement, horn, siren, burp, fart on press)
    const bool prev200 = this->mNowSpecial1Pressed;
    bool ctrl200 = (input & input_bit::Special1) == input_bit::Special1;
    this->mNowSpecial1Pressed = ctrl200;
    this->mSpecial1Changed = (ctrl200 != prev200);

    // alt (special2, hold for camera pan)
    const bool ctrl400 = (input & input_bit::Special2) == input_bit::Special2;
    same = (ctrl400 == this->mNowSpecial2Pressed);
    this->mSpecial2Changed = !same;
    this->mNowSpecial2Pressed = ctrl400;

    // right shift (special 3)
    const bool prev800 = this->mNowSpecial3Pressed;
    bool ctrl800 = (input & input_bit::Special3) == input_bit::Special3;
    this->mNowSpecial3Pressed = ctrl800;
    this->mSpecial3Changed = (ctrl800 != prev800);
}

MATCH_FUNC(0x566820)
void Player::ControlInputs()
{
    CharacterControls();

    if (mInputs)
    {
        const u16 dxKey = (mInputs >> input_bit::KeyShift) & input_bit::KeyMask;
        if (dxKey != 0)
        {
            if ((mInputs & input_bit::KeyDown) != 0)
            {
                Hud_Controls(dxKey);
            }
            else
            {
                HandleKeyRelease(dxKey);
            }
        }
        mInputs = 0;
    }
}

MATCH_FUNC(0x5668D0)
void Player::HandleControls(Ped* pPed)
{
    bool bNoPed = (pPed == NULL);
    Car* pPedCar = pPed->get_car_416B60();

    if (bNoPed)
    {
        return;
    }
    if (pPed->GetPedState_403990() == ped_state_1::dead_9)
    {
        return;
    }
    if (mNowEnterExitPressed && mEnterExitChanged)
    {
        if (!pPed->has_car_403B80())
        {
            if (pPed->get_objective_403A80() != objectives_enum::leave_car_36)
            {
                Car* pCar = gCarManager_6C_677930->GetNearestFrontVehicle(pPed->mGameObject->mSpritePtr, 3u);
                if (pCar)
                {
                    pPed->SetObjective2(objectives_enum::no_obj_0);

                    if (pCar->IsTrainModel_403BA0())
                    {
                        pPed->SetObjective(objectives_enum::enter_train_37);
                    }
                    else
                    {
                        pPed->SetObjective(objectives_enum::enter_car_as_driver_35);
                    }

                    pPed->SetTargetObjectiveCar(pCar);
                    pPed->set_enter_car_as_passenger_4039B0(0);
                    pPed->set_target_car_door_403A70(0);
                }
            }
        }
        else
        {
            if (pPedCar->CanExitCar())
            {
                pPed->SetObjective2(objectives_enum::no_obj_0);
                if (pPed->get_car_416B60()->mCarInfoIdx == car_model_enum::TRAIN)
                {
                    pPed->SetObjective(objectives_enum::leave_train_38);
                }
                else
                {
                    pPed->SetObjective(objectives_enum::leave_car_36);
                }
                pPed->SetTargetObjectiveCar(pPed->get_car_416B60());
            }
        }
    }
    else
    {
        s32 objective = pPed->get_objective_403A80();
        if (objective == objectives_enum::enter_car_as_driver_35 || objective == objectives_enum::enter_train_37)
        {
            if (pPed->GetPedState_403990() != ped_state_1::in_car_10)
            {
                if (pPed->GetObjectiveStatus_450CB0() == objective_status::failed_2 || !pPed->get_target_objective_car_403AB0())
                {
                    pPed->SetObjective(objectives_enum::no_obj_0);
                    pPed->SetObjective2(objectives_enum::no_obj_0);
                    pPed->mGameObject->mVelocity = kZero_6FE41C;
                }
            }
            else
            {
                if (pPed->GetObjectiveStatus_450CB0() != objective_status::not_finished_0)
                {
                    pPed->SetObjective(objectives_enum::no_obj_0);
                    pPed->SetObjective2(objectives_enum::no_obj_0);
                    pPed->ChangeNextPedState1(ped_state_1::in_car_10);
                    pPed->ChangeNextPedState2(ped_state_2::ped2_driving_10);
                }
            }
        }
    }

    if (mNowAttackPressed != 1 || mWastedOrBusted)
    {
        pPed->ClearAttacking_403A40();
    }
    else
    {
        if (!mAttackChanged)
        {
            if (mCurrWeaponIdx == -1)
            {
                pPed->SetAttacking_4A5010();
            }
            else
            {
                bNoPed = !mWeapons[mCurrWeaponIdx]->CanFireWhileHeld();
                if (!bNoPed)
                {
                    pPed->SetAttacking_4A5010();
                }
                else
                {
                    pPed->ClearAttacking_403A40();
                }
            }
        }
        else
        {
            pPed->SetAttacking_4A5010();
        }
    }

    if (pPedCar)
    {
        if (pPed->not_enter_car_as_passenger_4A5040())
        {
            if (pPedCar->IsTrainModel_403BA0())
            {
                Player::DoPedControlInputs(pPed);
            }
            else
            {
                Player::DoCarControlInputs(pPedCar);
            }
        }
    }
    else
    {
        Player::DoPedControlInputs(pPed);
    }
}

MATCH_FUNC(0x566C30)
void Player::DoCarControlInputs(Car* pCar)
{
    char bAttackPressed;

    if (this->mCurrWeaponIdx >= 15)
    {
        // Not a car weapon
        bAttackPressed = 0;
    }
    else
    {
        bAttackPressed = this->mNowAttackPressed;
    }

    pCar->HandleUserInput(this->mNowForwardPressed,
                                 this->mNowDownPressed,
                                 this->mNowLeftPressed,
                                 this->mNowRightPressed,
                                 this->mNowHandBrakeOrJumpPressed,
                                 this->mNowSpecial1Pressed,
                                 this->mSpecial1Changed,
                                 bAttackPressed);
}

MATCH_FUNC(0x566C80)
void Player::DoPedControlInputs(Ped* pPed)
{
    Ang16 turn_accel = mTurnAccel;

    // clear flag
    pPed->mBf21C.bSpottedPlayer = 0;

    // --- Right pressed ---
    if (mNowRightPressed == 1)
    {
        pPed->CancelEnterCarObjective();

        if (mNowAttackPressed && !mNowForwardPressed)
        {
            turn_accel = kAttackTurnAccel;
        }

        if (mJumping)
        {
            mTurnSpeed = mTurnSpeed - kJumpTurnAccel_6FE74C;
            if (mTurnSpeed < -kMaxJumpTurnSpeed)
            {
                mTurnSpeed = -kMaxJumpTurnSpeed;
            }
        }
        else
        {
            mTurnSpeed = mTurnSpeed - turn_accel;
            if (mTurnSpeed < -kMaxTurnSpeed)
            {
                mTurnSpeed = -kMaxTurnSpeed;
            }
        }
    }

    // --- Left pressed ---
    if (mNowLeftPressed == 1)
    {
        pPed->CancelEnterCarObjective();

        if (mNowAttackPressed && !mNowForwardPressed)
        {
            turn_accel = kAttackTurnAccel;
        }

        if (mJumping)
        {
            Ang16 new_turn_speed = mTurnSpeed + kJumpTurnAccel_6FE74C;
            mTurnSpeed = new_turn_speed;
            if (new_turn_speed > kMaxJumpTurnSpeed)
            {
                mTurnSpeed = kMaxJumpTurnSpeed;
            }
        }
        else
        {
            Ang16 new_turn_speed = mTurnSpeed + turn_accel;
            mTurnSpeed = new_turn_speed;
            if (new_turn_speed > kMaxTurnSpeed)
            {
                mTurnSpeed = kMaxTurnSpeed;
            }
        }
    }

    // --- Neutralize angle if no left/right ---
    if (!mNowLeftPressed && !mNowRightPressed)
    {
        mTurnSpeed = kAngZero_6FE754;
    }

    // --- Forward/backward movement ---
    if (mNowForwardPressed == 1)
    {
        pPed->CancelEnterCarObjective();
        mMoveDirection = kOne_6FE614;
    }
    else if (mNowDownPressed == 1)
    {
        pPed->CancelEnterCarObjective();
        mMoveDirection = -kOne_6FE614;
    }
    else
    {
        mMoveDirection = kZero_6FE610;
    }

    // --- Jump / handbrake ---
    if (mNowHandBrakeOrJumpPressed == 1 && mHandBrakeOrJumpChanged)
    {
        if (pPed->mGameObject && pPed->mGameObject->GetCharState_433A80() != Char_B4_state::Jumping_15 &&
            pPed->mBf21C.bLeftVehicle == 0)
        {
            pPed->DoJump_433C40();
        }
    }

    // --- Special action ---
    if (pPed->mGameObject)
    {
        if (mNowSpecial1Pressed && mSpecial1Changed && !mNowAttackPressed)
        {
            pPed->SetVoiceEventIfNotOnFire_433DD0(20);
        }
    }
}

MATCH_FUNC(0x566EE0)
void Player::ShowDebugInfo(bool skip)
{
    if (!skip)
    {
        Ped* pPed = GetPlayerPed_4A5130();
        Car* pCar = pPed->get_car_416B60();

        if (pCar)
        {
            CarPhysics* pPhysics = pCar->mPhysics;
            if (pPhysics)
            {
                pPhysics->ShowPhysicsDebug();
                pCar->mPhysics->ShowSpeedRevsDamage();
            }
        }

        if (bDo_show_camera_67D58A)
        {
            double x = this->mGameCamera.mCurPos.mX.AsDouble();
            double y = this->mGameCamera.mCurPos.mY.AsDouble();
            double z = this->mGameCamera.mCurPos.mZ.AsDouble();
            swprintf(tmpBuff_67BD9C, L"game camera: (%3.3f,%3.3f,%3.3f)", x, y, z);
            gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 64, gDebugFont_706600, 1);

            swprintf(tmpBuff_67BD9C,
                     L"aux game camera: (%3.3f,%3.3f,%3.3f)",
                     this->mAuxGameCamera.mCurPos.mX.AsDouble(),
                     this->mAuxGameCamera.mCurPos.mY.AsDouble(),
                     this->mAuxGameCamera.mCurPos.mZ.AsDouble());
            gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 80, gDebugFont_706600, 1);

            swprintf(tmpBuff_67BD9C,
                     L"view camera: (%3.3f,%3.3f,%3.3f)",
                     this->mViewCamera.mCurPos.mX.AsDouble(),
                     this->mViewCamera.mCurPos.mY.AsDouble(),
                     this->mViewCamera.mCurPos.mZ.AsDouble());
            gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 96, gDebugFont_706600, 1);
        }

        if (gShow_cycle_67D6BD)
        {
            gpRng_67AB34->ShowCycle();
        }

        if (gDo_show_input_67D576)
        {
            gInputRecorder_67F8B0.ShowInput(); // input
        }

        gGame_0x40_67E008->ShowDebugOverlay();
    }
}

MATCH_FUNC(0x5670B0)
void Player::RespawnPlayer()
{
    if (!IsNetworkGame_434B10() && !mBusted)
    {
        Player::ChangeLifeCountByAmount(-1);
    }
    if (mLives.get_value() > 0)
    {
        gmp_map_zone* pZone;
        if (IsNetworkGame_434B10())
        {
            pZone = gMap_0x370_6F6268->GetNextZoneOfTypeRoundRobin(0x10u);
        }
        else
        {
            s32 y_pos = mPlayerPed->get_cam_y().ToInt();
            s32 x_pos = mPlayerPed->get_cam_x().ToInt();
            pZone = gMap_0x370_6F6268->GetNearestZoneOfType(x_pos, y_pos, Restart_16);
        }
        mPlayerPed->RespawnPed(pZone);
    }
}

MATCH_FUNC(0x567130)
void Player::Wasted()
{
    Ped* pPed_killer;
    Player* player_killer; //  this function seems to handle the player death

    if (bStartNetworkGame_7081F0)
    {
        // Ped: field_204 = the ID of the ped who killed this player/Player
        if (mPlayerPed->mKillerId == 0)
        {
            player_killer = NULL;
        }
        else if ((pPed_killer = gPedManager_6787BC->PedById(mPlayerPed->mKillerId)) == NULL)
        {
            player_killer = NULL;
        }
        else if (!pPed_killer->PedTypeIs(ped_type::player_2))
        {
            player_killer = NULL;
        }
        else if ((player_killer = pPed_killer->mPlayer) == NULL)
        {
            player_killer = NULL;
        }

        (&gTagGame_6F8450)
            ->UpdateFugitive( //  tag mode death handler
                gGame_0x40_67E008->GetPlayer_4219E0(get_idx_4219D0()),
                player_killer); //  if player_killer != NULL then 'player_killer' now is "IT"
    }

    mAuxPedDying = false;
    mPlayerPed->ClearAttacking_403A40();
    if (!mWastedOrBusted)
    {
        if (IsUser_41DC70())
        {
            if (mLives.get_value() > 1 || bStartNetworkGame_7081F0)
            {
                gHud_2B00_706620->mMessage.ShowMessage( //  It's really ugly, it's probably inlined
                    gText_0x14_704DFC->Find(GetDeathText()),
                    hud_message_priority::normal_1);
                gRoot_sound_66B038.PlayVoice(voice_line::wasted_29);
            }
            else
            {
                gHud_2B00_706620->mMessage.ShowMessage(gText_0x14_704DFC->Find("g_over"), hud_message_priority::important_3);
                gRoot_sound_66B038.PlayVoice(voice_line::game_over_21);
            }
        }
        SetDeathType_434950(0);
        if (mAuxCamActive)
        {
            Player::ResetAuxCamera();
        }
        mWastedOrBusted = true;
        mDeathCountdown = 70;
        if (mLives.get_value() > 1)
        {
            mAuxPed = gPedManager_6787BC->ClonePed(mPlayerPed);
            mAuxPed->mSelectedWeapon = NULL;
            mAuxPed->mId = 0;
            mAuxPed->ClearAttacking_403A40();
            mAuxPed->mPedRefIdx = 0;
            mCameraMode = 2;
            memcpy(&mAuxGameCamera, &mGameCamera, sizeof(mAuxGameCamera));
            mAuxCamActive = true;
        }
        Player::RespawnPlayer();
    }
    else
    {
        if (mAuxPed)
        {
            mAuxPed->ClearAttacking_403A40();
        }

        if (mDeathCountdown == 0)
        {
            if (mLives.get_value() > 0 || bStartNetworkGame_7081F0)
            {
                Player::ClearKFWeapon();
                if (!bKeep_weapons_after_death_67D54D)
                {
                    Player::UnloadCarWeapons();
                    Player::RemovePlayerWeapons(); //  remove weapons from dead player
                    Player::ClearPowerUpsExceptJailCard();
                }
                mCameraMode = 0;
                mGameCamera.ResetCameraSmoothing();
                mGameCamera.SetSmoothFollow_475B60();

                if (mAuxPed != NULL)
                {
                    mAuxPed->Deallocate();
                    mAuxPed = NULL;
                    mAuxCamActive = false;
                }
                mPlayerPed->ResetForPlayerRespawn();
                mWastedOrBusted = false;
            }
            else
            {
                gGame_0x40_67E008->ExitGameNoBonus(0, GameExitType::GameOverRIP_3);
            }
        }
        else
        {
            mDeathCountdown--;
        }
    }
}

MATCH_FUNC(0x567850)
void Player::UpdateAuxPedDeath()
{
    if (!mAuxPedDying)
    {
        mAuxPedDying = true;
        mDeathCountdown = 70;
        return;
    }

    if (--mDeathCountdown == 0)
    {
        mAuxPedDying = false;
        ResetAuxCamera();
    }
}

MATCH_FUNC(0x5679E0)
void Player::Busted()
{
    mAuxPedDying = false;
    mPlayerPed->ClearAttacking_403A40();
    if (!mWastedOrBusted)
    {
        gRoot_sound_66B038.PlayVoice(voice_line::busted_17);
        SetDeathType_434950(3);
        if (IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage(gText_0x14_704DFC->Find(Player::GetDeathText()), hud_message_priority::normal_1);
        }
        SetDeathType_434950(0);
        mWastedOrBusted = true;
        mDeathCountdown = 70;
        if (mAuxCamActive)
        {
            Player::ResetAuxCamera();
        }
        if (mPlayerPed->mPedGroup)
        {
            mPlayerPed->mPedGroup->DestroyGroup();
        }
        mAuxPed = gPedManager_6787BC->ClonePed(mPlayerPed);
        mAuxPed->mSelectedWeapon = NULL;
        mAuxPed->mId = 0;
        mAuxPed->ClearAttacking_403A40();
        mAuxPed->mPedRefIdx = 0;
        gPolice_7B8_6FEE40->SetArrestedPed(mPlayerPed, mAuxPed);
        mCameraMode = 2;
        memcpy(&mAuxGameCamera, &mGameCamera, sizeof(Camera));
        mAuxCamActive = true;
        Player::RespawnPlayer();
        mPlayerPed->mShockCounter = 0;
        mPlayerPed->ClearWantedPoints_420B80();
        mPlayerPed->mBf21C.bBusted = 0;
    }
    else
    {
        --mDeathCountdown;
        mAuxPed->ClearAttacking_403A40();

        if (!mDeathCountdown)
        {
            if (mLives.get_value() <= 0 || gGameSession_67E8E0.IsBonusStage() == 1)
            {
                gGame_0x40_67E008->ExitGameNoBonus(0, GameExitType::GameOverRIP_3);
            }
            else
            {
                gHud_2B00_706620->mMessage.ClearTimeToShow();
                Player::ClearKFWeapon();
                if (HasPowerUp_434920(power_up_indices::JailCard_4))
                {
                    if (!gCheatUnlimitedJailCard_67D4F6)
                    {
                        DecPowerUp_434940(power_up_indices::JailCard_4);
                    }
                }
                else
                {
                    if (!bKeep_weapons_after_death_67D54D)
                    {
                        Player::UnloadCarWeapons();
                        Player::RemovePlayerWeapons();
                        Player::ClearPowerUpsExceptJailCard();
                    }
                    s32 multiplers = mMultipliers.mValue / 2;
                    if (multiplers == 0)
                    {
                        multiplers = 1;
                    }
                    mMultipliers.SetValueClamped_4A50B0(multiplers);
                }
                mCameraMode = 0;
                mGameCamera.ResetCameraSmoothing();
                mGameCamera.SetSmoothFollow_475B60();
                mAuxPed->mShockCounter = 0;
                mAuxPed->mShockCounter = 0;
                mAuxPed->mWantedPoints = cop_level_ped_enum::cop_0_stars;
                mAuxPed->Deallocate();
                mAuxPed = NULL;
                mAuxCamActive = false;
                mBusted = false;
                mWastedOrBusted = false;
            }
        }
        else
        {
            if (mDeathCountdown == 2 && mLives.mValue > 0 && gGameSession_67E8E0.IsBonusStage() != 1)
            {
                mPlayerPed->mShockCounter = 0;
                mPlayerPed->ClearWantedPoints_420B80();
                mPlayerPed->SetObjective(objectives_enum::call_police_car_54, 60);
                mPlayerPed->SetTargetObjectiveCar(NULL);
            }
        }
    }
}

MATCH_FUNC(0x568520)
void Player::UpdateCurrentZones()
{
    const Ped* pPed = mCameraMode == 2 ? mAuxPed : mPlayerPed;
    const Fix16 cam_x_fp = pPed->mCam.x;
    const Fix16 cam_y_fp = pPed->mCam.y;
    mLocalNavigationZone =
        gMap_0x370_6F6268->GetZoneAtPosOfType(cam_x_fp.ToInt(), cam_y_fp.ToInt(), Local_Navigation_15);
    mNavigationZone = gMap_0x370_6F6268->GetZoneAtPosOfType(cam_x_fp.ToInt(), cam_y_fp.ToInt(), Navigation_1);
    mArrowBlockerZone = gMap_0x370_6F6268->GetZoneAtPosOfType(cam_x_fp.ToInt(), cam_y_fp.ToInt(), Arrow_Blocker_5);
    mGangCurrLocation = gMap_0x370_6F6268->GetGangAtCoords(cam_x_fp, cam_y_fp);
}

MATCH_FUNC(0x568630)
void Player::UpdateSoundListener()
{
    SoundObject* pSoundObj = mSoundObj;
    if (pSoundObj)
    {
        pSoundObj->mpAny.pCamera_0xBC = &mViewCamera;
    }
}

MATCH_FUNC(0x568670)
void Player::HandleDebugZoom()
{
    if (this->mDbgZoomIn)
    {
        mGameCamera.DecreaseElevation();
        mAuxGameCamera.DecreaseElevation();
    }

    if (this->mDbgZoomOut)
    {
        mGameCamera.IncreaseElevation();
        mAuxGameCamera.IncreaseElevation();
    }
}

MATCH_FUNC(0x5686D0)
void Player::UpdateCamera(Camera* pCam)
{
    // Camera panning
    if (this->mNowSpecial2Pressed)
    {
        pCam->HandlePanning(this->mNowForwardPressed,
                                   this->mNowDownPressed,
                                   this->mNowLeftPressed,
                                   this->mNowRightPressed);
        this->mNowLeftPressed = false;
        this->mNowRightPressed = false;
        this->mNowForwardPressed = false;
        this->mNowDownPressed = false;
        this->mForwardChanged = false;
        this->mDownChanged = false;
    }
    else
    {
        pCam->ResetPanning();
    }
}

MATCH_FUNC(0x568730)
void Player::Disconnect()
{
    Ped* pPlayerPed = mPlayerPed;
    if (pPlayerPed)
    {
        pPlayerPed->BecomeDummyOnPlayerDisconnect();
    }
    Ped* pPed = mAuxPed;
    if (pPed)
    {
        pPed->BecomeDummyOnPlayerDisconnect();
    }
    set_in_use_4A5170(false);
}

MATCH_FUNC(0x5687F0)
void Player::Service()
{
    const bool bNowEnterExitPressed = this->mNowEnterExitPressed;
    const bool bWasEnterExitPressed = this->mEnterExitChanged;
    const bool bNowSpecial_1_Pressed = this->mNowSpecial1Pressed;
    const bool bWasSpecial_1_Pressed = this->mSpecial1Changed;

    if (mTrafficSpawnCounter < 100u)
    {
        this->mTrafficSpawnCounter++;
    }

    Player::tick_down_powerups();
    Player::ShowDebugInfo(false);

    if (this->mDisableEnterVehicles)
    {
        this->mNowEnterExitPressed = false;
        this->mEnterExitChanged = false;
    }

    mScores.Service();
    mCrimeStats.Service();

    Player::SelectNextOrPrevWeapon(this->mNextWeaponChanged && this->mNowNextWeaponPressed,
                                          this->mPrevWeaponChanged && this->mNowPrevWeaponPressed);

    if (this->mBlockAttack)
    {
        if (this->mNowAttackPressed)
        {
            this->mNowAttackPressed = false;
        }
        else
        {
            this->mBlockAttack = false;
        }
    }

    if (bDo_debug_keys_67D6CF)
    {
        Player::HandleDebugZoom();
    }

    switch (this->mCameraMode)
    {

        case 1:
            mGameCamera.MoveTarget(this->mNowForwardPressed,
                                            this->mNowDownPressed,
                                            this->mNowLeftPressed,
                                            this->mNowRightPressed,
                                            this->mNowPrevWeaponPressed,
                                            this->mNowNextWeaponPressed);
            mGameCamera.Update();
            if (!this->mInDebugCamMode)
            {
                memcpy(&this->mViewCamera, &mGameCamera, sizeof(this->mViewCamera));
            }
            break;

        case 0:
            Player::UpdateCamera(&this->mGameCamera);
            if (!this->mDisableAllControls)
            {
                Player::HandleControls(this->mPlayerPed);
            }

            mGameCamera.UpdateFollowPedCamera(this->mPlayerPed);
            mGameCamera.Update();

            if (this->mAuxCamActive)
            {
                if (this->mAuxPed)
                {
                    mAuxGameCamera.UpdateFollowPedCamera(this->mAuxPed);
                }
                else if (this->mWatchedCar)
                {
                    mAuxGameCamera.UpdateFollowCarCamera(this->mWatchedCar);
                }
                mAuxGameCamera.Update();
            }

            if (!this->mInDebugCamMode)
            {
                memcpy(&this->mViewCamera, &mGameCamera, sizeof(this->mViewCamera));
            }
            break;

        case 2:
            this->mNowEnterExitPressed = false;
            this->mEnterExitChanged = false;

            Player::UpdateCamera(&this->mAuxGameCamera);

            if (this->mAuxPed)
            {
                if (!this->mDisableAllControls)
                {
                    Player::HandleControls(this->mAuxPed);
                }
                mAuxGameCamera.UpdateFollowPedCamera(this->mAuxPed);
            }
            mGameCamera.UpdateFollowPedCamera(this->mPlayerPed);
            mGameCamera.Update();
            mAuxGameCamera.Update();
            if (!this->mInDebugCamMode)
            {
                memcpy(&this->mViewCamera, &mAuxGameCamera, sizeof(this->mViewCamera));
            }
            break;

        case 3:
            mGameCamera.UpdateFollowPedCamera(this->mPlayerPed);
            if (this->mAuxPed)
            {
                mAuxGameCamera.UpdateFollowPedCamera(this->mAuxPed);
            }
            else if (this->mWatchedCar)
            {
                mAuxGameCamera.UpdateFollowCarCamera(this->mWatchedCar);
            }
            mGameCamera.Update();
            mAuxGameCamera.Update();
            if (!this->mInDebugCamMode)
            {
                memcpy(&this->mViewCamera, &mAuxGameCamera, sizeof(this->mViewCamera));
            }
            break;

        default:
            break;
    }

    Player::UpdateCurrentZones();

    if (this->mInDebugCamMode == 1)
    {
        this->mViewCamera.SetSnapFollow_4A5070();

        mViewCamera.MoveTarget(this->mDbgCamNorth,
                                         this->mDbgCamSouth,
                                         this->mDbgCamWest,
                                         this->mDbgCamEast,
                                         this->mDbgCamZoomingOut,
                                         this->mDbgCamZoomingIn);
        if (this->mDbgCamFollowPlayer)
        {
            mViewCamera.ApplyMovementDeltaFrom(&this->mGameCamera);
        }
        mViewCamera.Update();
    }

    Player::UpdateSoundListener();

    if (mPlayerPed)
    {
        if (mPlayerPed->mBf21C.bBusted)
        {
            this->mBusted = true;
        }

        if (mPlayerPed->GetPedState_403990() == ped_state_1::dead_9 && !mBusted)
        {
            Player::Wasted();
        }
        else if (mBusted)
        {
            Player::Busted();
        }
    }

    if (this->mAuxCamActive)
    {
        if (mAuxPed)
        {
            if (mAuxPed->GetPedState_403990() == ped_state_1::dead_9 && !this->mWastedOrBusted)
            {
                Player::UpdateAuxPedDeath();
            }
        }
    }
    this->mNowEnterExitPressed = bNowEnterExitPressed;
    this->mEnterExitChanged = bWasEnterExitPressed;
    this->mNowSpecial1Pressed = bNowSpecial_1_Pressed;
    this->mSpecial1Changed = bWasSpecial_1_Pressed;
    Player::SetScoreTextColour();
}

MATCH_FUNC(0x569410)
void Player::UpdatePaused()
{
    Camera* pCam;
    Camera* pCam2;

    Player::ShowDebugInfo(true);
    switch (mCameraMode)
    {
        case 1:
            pCam = &mGameCamera;
            pCam->RefreshBoundaries();
            if (!mInDebugCamMode)
            {
                memcpy(&mViewCamera, pCam, sizeof(mViewCamera));
            }
            break;
        case 0:
            pCam = &mGameCamera;
            pCam->RefreshBoundaries();
            if (mAuxCamActive)
            {
                pCam2 = &mAuxGameCamera;
                pCam2->RefreshBoundaries();
            }
            if (!mInDebugCamMode)
            {
                memcpy(&mViewCamera, pCam, sizeof(mViewCamera));
            }
            break;

        case 2:
        case 3:
            mGameCamera.RefreshBoundaries();
            pCam = &mAuxGameCamera;
            mAuxGameCamera.RefreshBoundaries();
            if (!mInDebugCamMode)
            {
                memcpy(&mViewCamera, pCam, sizeof(mViewCamera));
            }
            break;
        default:
            break;
    }
    if (mInDebugCamMode == 1)
    {
        mViewCamera.SetSnapFollow_4A5070();
        mViewCamera.MoveTarget(mDbgCamNorth,
                                         mDbgCamSouth,
                                         mDbgCamWest,
                                         mDbgCamEast,
                                         mDbgCamZoomingOut,
                                         mDbgCamZoomingIn);
        mViewCamera.Update();
    }
}

MATCH_FUNC(0x569530)
void Player::EndRemoteControl()
{
    Car* pCar = mAuxPed->get_car_416B60();
    if (pCar->mDriver)
    {
        pCar->ClearDriver();
    }

    pCar->UnlockUnlessPermanent_475C80();

    mAuxPed->Kill();
    mAuxPed = NULL;

    UnloadCarWeapons();

    if (mPlayerPed->get_car_416B60())
    {
        LoadCarWeapons(mPlayerPed->get_car_416B60());
    }
}

MATCH_FUNC(0x5695A0)
void Player::ResetAuxCamera()
{
    if (!mWastedOrBusted)
    {
        mCameraMode = 0;
        if (IsRemoteControlActive_4A5100())
        {
            EndRemoteControl();
        }
        mAuxPed = NULL;
        mWatchedCar = NULL;
        mAuxCamActive = false;
        mGameCamera.SetSmoothFollow_475B60();
    }
}

MATCH_FUNC(0x569600)
void Player::StartRemoteControl(Car* pCar)
{
    mAuxPed = gPedManager_6787BC->SpawnDriver(pCar);
    mAuxPed->SetPedType_403920(ped_type::player_2);
    mAuxPed->set_occupation_403970(ped_ocupation_enum::empty);
    mAuxPed->SetPlayer(this, true);
    mAuxPed->UpdatePositionFromCar();
    pCar->AssignDriver(mAuxPed);
    pCar->LockUnlessPermanent_475C10();
    mCameraMode = 2;
    mAuxGameCamera.UpdateFollowPedCamera(mAuxPed);
    mAuxGameCamera.CommitPendingTarget_41E410();
    mAuxGameCamera.ResetCameraSmoothing();
    mAuxCamActive = true;
    Player::UnloadCarWeapons();
    Player::LoadCarWeapons(pCar);
}

MATCH_FUNC(0x5696D0)
void Player::WatchCar(Car* pCar)
{
    if (!mAuxCamActive && !mAuxPed && !mWatchedCar)
    {
        mWatchedCar = pCar;
        mAuxGameCamera.UpdateFollowCarCamera(pCar);
        mAuxGameCamera.CommitPendingTarget_41E410();
        mAuxGameCamera.ResetCameraSmoothing();
        mCameraMode = 3;
        mAuxCamActive = true;
    }
}

MATCH_FUNC(0x569840)
void Player::GetPosU8(u8& x, u8& y, u8& z)
{
    Ped* pPed;

    if (mCameraMode == 2 || mCameraMode == 3)
    {
        pPed = mAuxPed;
    }
    else
    {
        pPed = mPlayerPed;
    }
    if (pPed)
    {
        x = pPed->get_cam_x().ToUInt8();
        y = pPed->get_cam_y().ToUInt8();
        z = pPed->get_cam_z().ToUInt8();
    }
    else
    {
        Camera* pCam;
        if (mCameraMode == 2 || mCameraMode == 3)
        {
            pCam = &mAuxGameCamera;
        }
        else
        {
            pCam = &mGameCamera;
        }
        x = pCam->mCurPos.mX.ToUInt8();
        y = pCam->mCurPos.mY.ToUInt8();
        z = (kTwo_6FE618 + pCam->mCurPos.mZ).ToUInt8();
    }
}

MATCH_FUNC(0x5698E0)
Car* Player::GetPlayerCar()
{
    Ped* pPed;
    if (mCameraMode == 2 || mCameraMode == 3)
    {
        pPed = this->mAuxPed;
    }
    else
    {
        pPed = this->mPlayerPed;
    }

    if (pPed)
    {
        Car* pCar = pPed->mCar;
        if (!pCar)
        {
            pCar = pPed->GetCarBeingEnteredOrExited();
        }
        return pCar;
    }
    return NULL;
}

MATCH_FUNC(0x569920)
void Player::get_pos(Fix16* out_x, Fix16* out_y, Fix16* out_z)
{
    Ped* pPed = GetActivePed_4A5150();
    if (pPed)
    {
        Car* pCar = pPed->GetCarBeingEnteredOrExited();
        if (pCar)
        {
            *out_x = pCar->GetX_41E430();
            *out_y = pCar->GetY_41E440();
            *out_z = pCar->GetZ_41E450();
        }
        else
        {
            *out_x = pPed->get_cam_x();
            *out_y = pPed->get_cam_y();
            *out_z = pPed->get_cam_z();
        }
    }
    else
    {
        Camera* pCam = get_camera_434900();
        *out_x = pCam->mCurPos.mX;
        *out_y = pCam->mCurPos.mY;
        *out_z = pCam->mCurPos.mZ + kTwo_6FE618;
    }
}

MATCH_FUNC(0x5699F0)
void Player::ChangeLifeCountByAmount(s32 amount)
{
    if (!gDo_infinite_lives_67D4C9 || amount > 0)
    {
        mLives.ChangeStatByAmount(amount);
    }
}

MATCH_FUNC(0x569A10)
void Player::ColorScoreFromRemap()
{
    switch (mPlayerPed->mRemap)
    {
        case ped_remap_enum::ped_remap_yakuza:
            mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 2);
            mScores.GetMultiplayerFragDigits()->ColorDigits(palette_types_enum::user_remaps_7, 2);
            mHudPalette = 2;
            mHudPaletteType = palette_types_enum::user_remaps_7;
            break;

        case ped_remap_enum::ped_remap_lonny:
            mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 3);
            mScores.GetMultiplayerFragDigits()->ColorDigits(palette_types_enum::user_remaps_7, 3);
            mHudPalette = 3;
            mHudPaletteType = palette_types_enum::user_remaps_7;
            break;

        case ped_remap_enum::ped_remap_russian:
            mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 6);
            mScores.GetMultiplayerFragDigits()->ColorDigits(palette_types_enum::user_remaps_7, 6);
            mHudPalette = 6;
            mHudPaletteType = palette_types_enum::user_remaps_7;
            break;

        case ped_remap_enum::ped_remap_kristna:
            mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 5);
            mScores.GetMultiplayerFragDigits()->ColorDigits(palette_types_enum::user_remaps_7, 5);
            mHudPalette = 5;
            mHudPaletteType = palette_types_enum::user_remaps_7;
            break;

        case ped_remap_enum::ped_remap_zaibatsu:
            mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 7);
            mScores.GetMultiplayerFragDigits()->ColorDigits(palette_types_enum::user_remaps_7, 7);
            mHudPalette = 7;
            mHudPaletteType = palette_types_enum::user_remaps_7;
            break;

        case ped_remap_enum::ped_remap_yellow_scientist:
            mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 8);
            mScores.GetMultiplayerFragDigits()->ColorDigits(palette_types_enum::user_remaps_7, 8);
            mHudPalette = 8;
            mHudPaletteType = palette_types_enum::user_remaps_7;
            break;

        case ped_remap_enum::ped_remap_red_head_redneck:
        case ped_remap_enum::ped_remap_blond_head_redneck:
            mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 4);
            mScores.GetMultiplayerFragDigits()->ColorDigits(palette_types_enum::user_remaps_7, 4);
            mHudPalette = 4;
            mHudPaletteType = palette_types_enum::user_remaps_7;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x569C20)
void Player::SetScoreTextColour()
{
    if (bStartNetworkGame_7081F0 == false && gScriptManager_6F8060 != NULL)
    {
        if (mFinishScoreReached == 0)
        {
            u32 score = mScores.GetScore();
            if (score >= static_cast<u32>(gScriptManager_6F8060->mFinishScore))
            {
                mFinishScoreReached = 1;
                // Red when map "beaten"
                mScores.GetScoreDigits()->ColorDigits(palette_types_enum::user_remaps_7, 6);
            }
        }
        else
        {
            u32 score = mScores.GetScore();
            if (score < static_cast<u32>(gScriptManager_6F8060->mFinishScore))
            {
                mFinishScoreReached = 0;
                mScores.GetScoreDigits()->ColorDigits(palette_types_enum::sprites_2, 0);
            }
        }
    }
}

MATCH_FUNC(0x569CB0)
void Player::InitializePlayerState()
{
    if (!gMap_0x370_6F6268->GetFirstZoneOfType(Restart_16))
    {
        FatalError_4A38C0(Gta2Error::NoRestartZone, "C:\\Splitting\\Gta2\\Source\\player.cpp",
                          2905); // No Restart Zone
    }
    mFinishScoreReached = 0;
    mAuxPedDying = false;
    mWastedOrBusted = false;
    mBusted = false;
    mTrafficSpawnCounter = 0;
    mTrafficSpawnThreshold = 1000;
    mScores.Reset();
    mLives.InitDigitSprites();
    mMultipliers.InitDigitSprites();
    mJumping = false;
    mPreKfWeaponKind = -2;
    if (gScriptManager_6F8060->IsLoadedFromSave_475A20())
    {
        Player::UpdateGameFromSave(&gGameSave_6F78C8.mPlayerAndWorldStats);
    }
    else
    {
        if (!gGameSession_67E8E0.IsBonusStage()) // bonus level?
        {
            mLives.ChangeStatByAmount(5);
        }
        else
        {
            mLives.ChangeStatByAmount(1);
        }
        mMultipliers.ChangeStatByAmount(1);
    }

    mGameCamera.UpdateFollowPedCamera(mPlayerPed);
    mGameCamera.SetSmoothFollow_475B60();
    mGameCamera.CommitPendingTarget_41E410();
    mGameCamera.ResetCameraSmoothing();

    mAuxGameCamera.UpdateFollowPedCamera(mPlayerPed);
    mAuxGameCamera.SetSmoothFollow_475B60();
    mAuxGameCamera.CommitPendingTarget_41E410();
    mAuxGameCamera.ResetCameraSmoothing();

    mAuxCamActive = false;

    if (IsUser_41DC70())
    {
        if (!bSkip_audio_67D6BE)
        {
            mSoundObj = gRoot_sound_66B038.CreateSoundObject(&mGameCamera, SoundObjectTypeEnum::Camera_0xBC_5);
        }
    }
    mHudPaletteType = palette_types_enum::sprites_2;
    mHudPalette = 0;
    if (bStartNetworkGame_7081F0)
    {
        Player::ColorScoreFromRemap();
        gNetPlay_7071E8.GetPlayerName(mPlayerName, mPlayerIdx);
        gText_0x14_704DFC->RemapExtendedCharsIfNotJapanese(mPlayerName);
        gGameSession_67E8E0.SetPlayerName(mPlayerIdx, mPlayerName);
    }
}

MATCH_FUNC(0x569E70)
void Player::DebugToggleRemoteControl()
{
    if (IsRemoteControlActive_4A5100())
    {
        if (mCameraMode == 2)
        {
            Player::DisableInputs();
        }
        Player::ResetAuxCamera();
    }
    else if (!mAuxCamActive)
    {
        Fix16 zpos;
        Fix16 ypos;
        Fix16 xpos;
        Player::get_pos(&xpos, &ypos, &zpos);
        Car* pCar = gCarManager_6C_677930->GetNearestCarFromCoord(xpos, ypos, zpos, NULL);
        if (pCar)
        {
            Player::DisableInputs();
            Player::StartRemoteControl(pCar);
        }
    }
}

MATCH_FUNC(0x569F00)
char* Player::GetDeathText()
{
    switch (mDeathType)
    {
        case 0:
        case 1:
            return "wasted";
            break;
        case 2:
            return "fried";
            break;
        case 3:
            return "nicked";
            break;
        case 4:
            return "shocked";
            break;
        default:
            return NULL;
            break;
    }
}

MATCH_FUNC(0x569F40)
void Player::DisableInputs()
{
    Ped* pPed;
    if (!mCameraMode)
    {
        pPed = this->mPlayerPed;
    }
    else
    {
        if (mCameraMode != 2)
        {
            pPed = NULL;
        }
        else
        {
            pPed = this->mAuxPed;
        }
    }

    if (pPed)
    {
        pPed->ClearAttacking_403A40();

        Car* pCar = pPed->get_car_416B60();
        if (pCar)
        {
            if (pPed->not_enter_car_as_passenger_4A5040())
            {
                if (pCar->IsTrainModel_403BA0())
                {
                    this->mTurnSpeed = kAngZero_6FE754;
                    this->mMoveDirection = kZero_6FE610;
                }
                else if (pCar->mPhysics)
                {
                    pCar->HandleUserInput(0, 0, 0, 0, 0, 0, 0, 0);
                }
            }
        }
    }

    this->mTurnSpeed = kAngZero_6FE754;
    this->mMoveDirection = kZero_6FE610;
}

MATCH_FUNC(0x569FF0)
void Player::DisableAllControls()
{
    mDisableAllControls = true;
    DisableInputs();
}

MATCH_FUNC(0x56A000)
void Player::EnableAllControls()
{
    mDisableAllControls = false;
}

MATCH_FUNC(0x56A010)
void Player::EnableKFMode()
{
    mKfWeaponMode = true;
}

MATCH_FUNC(0x56A020)
void Player::DisableKFMode()
{
    mKfWeaponMode = false;
}

MATCH_FUNC(0x56A030)
void Player::DisableEnterVehicles()
{
    mDisableEnterVehicles = true;
}

MATCH_FUNC(0x56A040)
void Player::EnableEnterVehicles()
{
    mDisableEnterVehicles = false;
}

MATCH_FUNC(0x56a0f0)
void Player::RestoreCarsFromSave()
{
    for (u8 i = 0; i < 3; i++)
    {
        if (gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mX[i].mValue > kZero_6FE610 &&
            gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mY[i].mValue > kZero_6FE610)
        {
            Car* pNewCar = gCarManager_6C_677930->SpawnCar_426E10(gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mX[i].mValue,
                                                                gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mY[i].mValue,
                                                                gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mZ[i].mValue,
                                                                gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mAng[i],
                                                                gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mCarModel[i],
                                                                kOne_6FE614);
            AddCarToHistory(pNewCar);
            pNewCar->mDamagedAreas.m_var = gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mDamagedAreas[i];
            pNewCar->SetDamage_4A51B0(gGameSave_6F78C8.mCarAndScriptData.mSavedCars.mDamage[i]);
        }
    }
    memset(&gGameSave_6F78C8.mCarAndScriptData.mSavedCars, 0, 68u);
}

MATCH_FUNC(0x56A1A0)
void Player::CopyPlayerDataToSave(SavedPlayerStats* pSave)
{
    pSave->mX = mPlayerPed->get_cam_x();
    pSave->mY = mPlayerPed->get_cam_y();
    pSave->mZ = mPlayerPed->get_cam_z();
    pSave->mRotation = mPlayerPed->GetRotation();
    pSave->mMoney = GetScore_421980();
    pSave->mMultipliers = get_multiplier_4766A0();
    pSave->mHealth = mPlayerPed->get_health_433B70();
    pSave->mPlayerPedRemap = mPlayerPed->get_remap_433BA0();
    pSave->mLives = get_lives_4766C0();
    pSave->mCurrWeaponIdx = mCurrWeaponIdx;
    pSave->mWantedLevel = mPlayerPed->mWantedPoints;

    for (u8 gang_idx = 0; gang_idx < 10; gang_idx++)
    {
        Gang* pGang = gGangPool_CA8_67E274->GangByIdx(gang_idx);
        pSave->mGangRespect[gang_idx] = pGang->GetRespectForPlayer(mPlayerIdx);
    }

    for (u16 weapon_idx = 0; weapon_idx < 15; weapon_idx++)
    {
        pSave->mWeaponAmmo[weapon_idx] = mWeapons[weapon_idx]->get_ammo_4A4FB0();
    }

    for (u16 crime_idx = 0; crime_idx < crime_stats_type::count_10; crime_idx++)
    {
        pSave->mCrimeCounts[crime_idx] = mCrimeStats.mCrimeCountList[crime_idx];
    }
    pSave->mCarDamageCost = mCrimeStats.mCarDamageCost;
    pSave->mEvasionRating = mCrimeStats.mEvasionRating;
}

MATCH_FUNC(0x56A310)
void Player::UpdateGameFromSave(SavedPlayerStats* pSave)
{
    mLives.SetValueClamped_4A50B0(pSave->mLives);
    mMultipliers.SetValueClamped_4A50B0(pSave->mMultipliers);
    mScores.SetMoney_4A50E0(pSave->mMoney);

    mCurrWeaponIdx = pSave->mCurrWeaponIdx;
    mPlayerPed->set_health_4039A0(pSave->mHealth);

    mPlayerPed->ClearWantedPoints_420B80();
    mPlayerPed->add_wanted_points(pSave->mWantedLevel);

    for (u8 gang_idx = 0; gang_idx < 10; gang_idx++)
    {
        Gang* pGang = gGangPool_CA8_67E274->GangByIdx(gang_idx);
        pGang->SetRespect(mPlayerIdx, pSave->mGangRespect[gang_idx]);
    }

    for (u16 weapon_idx = 0; weapon_idx < 15; weapon_idx++)
    {
        mWeapons[weapon_idx]->set_ammo_clips(pSave->mWeaponAmmo[weapon_idx]);
    }

    for (u16 crime_idx = 0; crime_idx < crime_stats_type::count_10; crime_idx++)
    {
        mCrimeStats.mCrimeCountList[crime_idx] = pSave->mCrimeCounts[crime_idx];
    }
    mCrimeStats.mCarDamageCost = pSave->mCarDamageCost;
    mCrimeStats.mEvasionRating = pSave->mEvasionRating;

    Player::RestorePowerUpsFromSave(pSave);
    Player::RestoreCarsFromSave();
}

MATCH_FUNC(0x56A490)
void Player::ApplyCheats()
{
    if (gCheatGetPlayerPoints_67D4C8)
    {
        mScores.SetMoney_4A50E0(200000);
    }
    if (gCheatGet10MillionMoney_67D6CE)
    {
        mScores.SetMoney_4A50E0(9999999);
    }
    if (gCheatUnlimitedElectroGun_67D4F7)
    {
        mWeapons[weapon_type::shocker]->set_infinite_ammo_4A4F90();
    }
    if (gCheatUnlimitedFlameThrower_67D6CC)
    {
        mWeapons[weapon_type::flamethrower]->set_infinite_ammo_4A4F90();
    }
    if (gCheatAllGangMaxRespect_67D587)
    {
        for (Gang* pIter = gGangPool_CA8_67E274->FirstGang(); pIter; pIter = gGangPool_CA8_67E274->NextGang())
        {
            pIter->SetRespect(mPlayerIdx, 100);
        }
    }
    if (gCheatGet99Lives_67D4F1)
    {
        mLives.SetValueClamped_4A50B0(99);
    }
    if (gCheat10xMultiplier_67D589)
    {
        mMultipliers.ChangeStatByAmount(9);
    }

    if (gCheatOnlyMuggerPeds_67D5A4)
    {
        gPedManager_6787BC->mMakeAllMuggers = true;
    }

    if (gCheatUnlimitedJailCard_67D4F6)
    {
        Player::CollectPowerUp(power_up_indices::JailCard_4);
    }

    if (gCheatInvisibility_67D539)
    {
        Player::CollectPowerUp(power_up_indices::Invisibility_11);
    }

    if (gCheatUnlimitedDoubleDamage_67D57C)
    {
        Player::CollectPowerUp(power_up_indices::DoubleDamage_7);
    }

    if (gCheatJailCardSmgRespect_67D56B)
    {
        Player::CollectPowerUp(power_up_indices::JailCard_4);
        Player::AddWeaponWithAmmo(weapon_type::smg, 50u);
        for (Gang* pIter2 = gGangPool_CA8_67E274->FirstGang(); pIter2; pIter2 = gGangPool_CA8_67E274->NextGang())
        {
            pIter2->SetRespect(mPlayerIdx, 80);
        }
    }
}

MATCH_FUNC(0x56A6D0)
void Player::ClearInputs()
{
    mNowForwardPressed = false;
    mNowDownPressed = false;
    mNowLeftPressed = false;
    mNowRightPressed = false;
    mNowAttackPressed = false;
    mNowEnterExitPressed = false;
    mNowHandBrakeOrJumpPressed = false;
    mNowPrevWeaponPressed = false;
    mNowNextWeaponPressed = false;
    mNowSpecial1Pressed = false;
    mNowSpecial2Pressed = false;
    mNowSpecial3Pressed = false;

    mSpecial1Changed = false;
    mPrevWeaponChanged = false;
    mNextWeaponChanged = false;
    mEnterExitChanged = false;
    mAttackChanged = false;
    mHandBrakeOrJumpChanged = false;
    mForwardChanged = false;
    mDownChanged = false;
}

MATCH_FUNC(0x56A740)
Player::Player(u8 player_idx)
{
    mIsChatting = false;
    mChatTextIdx = 0;
    mChatText[0] = 0;
    mPlayerIdx = player_idx;
    mWastedOrBusted = false;
    mBusted = false;
    mTrafficSpawnCounter = 0;
    mTrafficSpawnThreshold = 1000;
    mAuxPedDying = false;
    mInputs = 0;
    mInUse = true;

    mPlayerPed = 0;
    mAuxPed = 0;
    mWatchedCar = 0;
    mGangCurrLocation = 0;
    mLocalNavigationZone = 0;
    mNavigationZone = 0;

    Player::ClearInputs();
    mTurnSpeed = kAngZero_6FE754;
    mTurnAccel = kAngZero_6FE754;
    mMoveDirection = kZero_6FE610;
    mUnused10 = 0;
    mCurrWeaponIdx = 0;
    mCameraMode = 1;
    mInDebugCamMode = 0;
    mDbgCamFollowPlayer = false;
    mBlockAttack = false;
    mDisableAllControls = false;
    mDisableEnterVehicles = false;
    mKfWeaponMode = false;
    mDbgCamNorth = false;
    mDbgCamSouth = false;
    mDbgCamWest = false;
    mDbgCamEast = false;
    mDbgCamZoomingIn = false;
    mDbgCamZoomingOut = false;
    mDbgZoomOut = false;
    mDbgZoomIn = false;
    memset(mWeapons, 0, sizeof(mWeapons));

    for (s32 i = 0; i < 3; i++)
    {
        mCarHistory[i] = NULL;
    }

    mIsUser = false;
    mAuxCamActive = false;
    mDeathCountdown = 0;
    mSoundObj = NULL;
    mPlayerIdx = player_idx;
    mScores.Init(this, 2, 999999999, 158, 999u);
    mLives.SetupDigitsParams(1, 99, 115);
    mMultipliers.SetupDigitsParams(1, 99, 116);
    Player::ClearPowerUps();
    ResetThrowCharge_4A5180();
    SetDeathType_434950(0);
    mPlayerName[0] = 0;
    mShowQuitMessage = false;
}

MATCH_FUNC(0x56A940)
Player::~Player()
{
    mPlayerPed = NULL;
    mAuxPed = NULL;
    mWatchedCar = NULL;
    mGangCurrLocation = NULL;
    mLocalNavigationZone = NULL;
    mNavigationZone = NULL;
    memset(mCarHistory, 0, sizeof(mCarHistory));

    if (mSoundObj)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(mSoundObj);
        mSoundObj = NULL;
    }

    /*
	PlayerCrimeStats::dtor_484EE0(&mCrimeStats);
	BonusTracker::dtor_431DF0(&field_2D4_unk.mBonuses);
	Camera::dtor_4369E0(&mAuxGameCamera);
	Camera::dtor_4369E0(&mViewCamera);
	Camera::dtor_4369E0(&mGameCamera);
	*/
}

MATCH_FUNC(0x5B2640)
void DebugTest::DoTest(u16 action)
{
    LOCAL_STATIC(Hud_Arrow*, sTestArrow, 0x70416C);
    LOCAL_STATIC(s32, sTestTimer, 0x7042D8);
    switch (action)
    {
        case 3:
            gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::normal_1, "8011");
            break;
        case 4:
            gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::now_3, "8012");
            break;
        case 5:
            gHud_2B00_706620->mBrief.SetHudBrief_5D4400(hud_brief_priority::now_3, "8013");
            break;
        case 6:
            gGame_0x40_67E008->mLocalPlayer->ChangeMultipliers_4766B0(1);
            break;
        case 7:
            sTestArrow = gHud_2B00_706620->mArrows.AllocArrow();
            sTestArrow->SetArrowTargetCar_476860(gTestArrowCar_7043F0);
            sTestArrow->SetArrowColour(5);
            break;
        case 8:
            gGame_0x40_67E008->mLocalPlayer->DebugToggleRemoteControl();
            break;
        case 10:
            sTestTimer = gHud_2B00_706620->mPagers.CreateTimer(40);
            break;
    }
}