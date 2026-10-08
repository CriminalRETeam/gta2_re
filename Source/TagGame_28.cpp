#include "TagGame_28.hpp"
#include "Frontend.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "GameSession_578.hpp"
#include "error.hpp"
#include "text_0x14.hpp"
#include <string>

DEFINE_GLOBAL(TagGame_28, gTagGame_6F8450, 0x6F8450);
DEFINE_GLOBAL(Fix16_Point, stru_6F8720, 0x6F8720);
DEFINE_GLOBAL(s32, gNetTimeLimitEnabled_6F58A4, 0x6F58A4);

MATCH_FUNC(0x516560)
void TagGame_28::Init_516560() // For some reason, it's a function instead of a proper ctor
{
    field_0_fugitive = NULL;
    field_1C_tick_timer = 0;
    memset(field_4_it_time_secs, 0, sizeof(field_4_it_time_secs));
    memset(field_20_bHasQuit, 0, sizeof(field_20_bHasQuit));
}

MATCH_FUNC(0x516590)
void TagGame_28::SetNewFugitive_516590(Player* pNewFugitive)
{
    if (pNewFugitive == NULL)
    {
        pNewFugitive = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0();
    }
    field_0_fugitive = pNewFugitive;
    gHud_2B00_706620->field_1F18_arrows.ReleaseAllArrows_5D10B0();
    field_0_fugitive->UnloadCarWeapons_564C00();
    field_0_fugitive->RemovePlayerWeapons_564C50();
    field_0_fugitive->ClearPowerUps_564CC0();

    if (field_0_fugitive->GetPlayerPed_41D020() != NULL)
    {
        field_0_fugitive->GetPlayerPed_41D020()->SetVisible();
        field_0_fugitive->GetPlayerPed_41D020()->ClearInvulnerable_45C050();
        field_0_fugitive->GetPlayerPed_41D020()->clear_bit_26_482080();
    }

    if (!field_0_fugitive->IsUser_41DC70())
    {
        Hud_Arrow_7C* pArrow = gHud_2B00_706620->field_1F18_arrows.AllocArrow_5D1050();
        pArrow->field_18.field_18_primary_target.SetTargetPlayer_4820A0(field_0_fugitive);

        if (field_0_fugitive->field_2C4_player_ped != NULL)
        {
            pArrow->SetPlayerArrowColour_5D0DC0(field_0_fugitive->field_2C4_player_ped);
        }
    }
    else
    {
        gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("yourit"), 3);
    }
}

MATCH_FUNC(0x516660)
void TagGame_28::ExecuteGamemodeTick_516660()
{
    switch (gGameSession_67E8E0.GetMultiplayerGamemode_4C5BC0())
    {
        case NO_GAME_0:
            FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\multip.cpp", 108, 0);
        case FRAG_GAME_1:
        case POINTS_GAME_2:
            return;
        case TAG_GAME_3:
            field_1C_tick_timer++;
            if (field_1C_tick_timer >= 30)
            {
                field_1C_tick_timer = 0;
                if (field_0_fugitive && gNetTimeLimitEnabled_6F58A4 != 0)
                {
                    field_4_it_time_secs[field_0_fugitive->get_idx_4219D0()]++;
                }
            }
            if (!field_0_fugitive)
            {
                SetNewFugitive_516590(NULL);
            }

            if (field_0_fugitive && field_0_fugitive->field_2C4_player_ped &&
                field_0_fugitive->field_2C4_player_ped->field_16C_car)
            {
                field_0_fugitive->GetPlayerPed_41D020()->get_car_416B60()->AccumulateDamage_43DA90(17, &stru_6F8720);
            }

            break;
        default:
            FatalError_4A38C0(Gta2Error::InvalidLineInfo, "C:\\Splitting\\Gta2\\Source\\multip.cpp", 144, 0);
    }
}

MATCH_FUNC(0x516740)
void TagGame_28::UpdateFugitive_516740(Player* pFormerPlayerFugitive, Player* pPlayer_killer)
{
    Fix16 threshold_distance(65535); // 65535.0 (raw 0x3FFFC000): start value of the nearest player search
    if (gGameSession_67E8E0.GetMultiplayerGamemode_4C5BC0() == TAG_GAME_3
        && field_0_fugitive == pFormerPlayerFugitive)
    {
        if (pPlayer_killer != NULL)
        {
            TagGame_28::SetNewFugitive_516590(pPlayer_killer); //  Make him "IT"
        }
        else
        {
            Player* PlayerIter = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0();
            Player* pChosenPlayer = NULL;
            Fix16 fugitive_x = field_0_fugitive->field_2C4_player_ped->field_1AC_cam.x;
            Fix16 fugitive_y = field_0_fugitive->field_2C4_player_ped->field_1AC_cam.y;

            for (; PlayerIter != NULL; PlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
            {
                if (PlayerIter != field_0_fugitive)
                {
                    Fix16 iter_y = PlayerIter->field_2C4_player_ped->field_1AC_cam.y;
                    Fix16 y_abs = Fix16::Abs(iter_y - fugitive_y);
                    Fix16 iter_x = PlayerIter->field_2C4_player_ped->field_1AC_cam.x;
                    Fix16 x_abs = Fix16::Abs(iter_x - fugitive_x);
                    Fix16 taxicab_distance = x_abs + y_abs; //  taxicab geometry
                    if (taxicab_distance < threshold_distance)
                    {
                        threshold_distance = taxicab_distance;
                        pChosenPlayer = PlayerIter;
                    }
                }
            }
            /*
            the player closest to the former fugitive (in taxicab geometry)
            will be the "pChosenPlayer"
            */
            if (pChosenPlayer != NULL)
            {
                TagGame_28::SetNewFugitive_516590(pChosenPlayer); //  Make him "IT"
            }
        }
    }
}
