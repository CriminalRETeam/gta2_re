#include "TagGame.hpp"
#include "hud_message_priority.hpp"
#include "Frontend.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "GameSession.hpp"
#include "error.hpp"
#include "TextTable.hpp"
#include <string>

DEFINE_GLOBAL(TagGame, gTagGame, 0x6F8450);
DEFINE_STATIC_GLOBAL(Fix16_Point, kZeroPoint, 0x6F8720);
DEFINE_GLOBAL(s32, gNetTimeLimitEnabled, 0x6F58A4);

MATCH_FUNC(0x516560)
void TagGame::Init() // For some reason, it's a function instead of a proper ctor
{
    mFugitive = NULL;
    mTickTimer = 0;
    memset(mItTimeSecs, 0, sizeof(mItTimeSecs));
    memset(mbHasQuit, 0, sizeof(mbHasQuit));
}

MATCH_FUNC(0x516590)
void TagGame::SetNewFugitive(Player* pNewFugitive)
{
    if (pNewFugitive == NULL)
    {
        pNewFugitive = gGame_0x40->IterateFirstPlayer();
    }
    mFugitive = pNewFugitive;
    gHud_2B00->mArrows.ReleaseAllArrows();
    mFugitive->UnloadCarWeapons();
    mFugitive->RemovePlayerWeapons();
    mFugitive->ClearPowerUps();

    if (mFugitive->GetPlayerPed_41D020() != NULL)
    {
        mFugitive->GetPlayerPed_41D020()->SetVisible();
        mFugitive->GetPlayerPed_41D020()->ClearInvulnerable();
        mFugitive->GetPlayerPed_41D020()->ClearElectrofingers_482080();
    }

    if (!mFugitive->IsUser_41DC70())
    {
        Hud_Arrow* pArrow = gHud_2B00->mArrows.AllocArrow();
        pArrow->mTargets.mPrimaryTarget.SetTargetPlayer_4820A0(mFugitive);

        if (mFugitive->mPlayerPed != NULL)
        {
            pArrow->SetPlayerArrowColour(mFugitive->mPlayerPed);
        }
    }
    else
    {
        gHud_2B00->mMessage.ShowMessage(gText_0x14->Find("yourit"), hud_message_priority::important_3);
    }
}

MATCH_FUNC(0x516660)
void TagGame::ExecuteGamemodeTick()
{
    switch (gGameSession.GetMultiplayerGamemode())
    {
        case NO_GAME_0:
            FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\multip.cpp", 108, 0);
        case FRAG_GAME_1:
        case POINTS_GAME_2:
            return;
        case TAG_GAME_3:
            mTickTimer++;
            if (mTickTimer >= 30)
            {
                mTickTimer = 0;
                if (mFugitive && gNetTimeLimitEnabled != 0)
                {
                    mItTimeSecs[mFugitive->get_idx_4219D0()]++;
                }
            }
            if (!mFugitive)
            {
                SetNewFugitive(NULL);
            }

            if (mFugitive && mFugitive->mPlayerPed &&
                mFugitive->mPlayerPed->mCar)
            {
                mFugitive->GetPlayerPed_41D020()->get_car_416B60()->AccumulateDamage(17, &kZeroPoint);
            }

            break;
        default:
            FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\multip.cpp", 144, 0);
    }
}

MATCH_FUNC(0x516740)
void TagGame::UpdateFugitive(Player* pFormerPlayerFugitive, Player* pPlayer_killer)
{
    Fix16 threshold_distance(65535); // 65535.0 (raw 0x3FFFC000): start value of the nearest player search
    if (gGameSession.GetMultiplayerGamemode() == TAG_GAME_3
        && mFugitive == pFormerPlayerFugitive)
    {
        if (pPlayer_killer != NULL)
        {
            TagGame::SetNewFugitive(pPlayer_killer); //  Make him "IT"
        }
        else
        {
            Player* pIter = gGame_0x40->IterateFirstPlayer();
            Player* pChosenPlayer = NULL;
            Fix16 fugitive_x = mFugitive->mPlayerPed->mCam.x;
            Fix16 fugitive_y = mFugitive->mPlayerPed->mCam.y;

            for (; pIter != NULL; pIter = gGame_0x40->IterateNextPlayer())
            {
                if (pIter != mFugitive)
                {
                    Fix16 iter_y = pIter->mPlayerPed->mCam.y;
                    Fix16 y_abs = Fix16::Abs(iter_y - fugitive_y);
                    Fix16 iter_x = pIter->mPlayerPed->mCam.x;
                    Fix16 x_abs = Fix16::Abs(iter_x - fugitive_x);
                    Fix16 taxicab_distance = x_abs + y_abs; //  taxicab geometry
                    if (taxicab_distance < threshold_distance)
                    {
                        threshold_distance = taxicab_distance;
                        pChosenPlayer = pIter;
                    }
                }
            }
            /*
            the player closest to the former fugitive (in taxicab geometry)
            will be the "pChosenPlayer"
            */
            if (pChosenPlayer != NULL)
            {
                TagGame::SetNewFugitive(pChosenPlayer); //  Make him "IT"
            }
        }
    }
}
