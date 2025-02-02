#include "youthful_einstein.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "lucid_hamilton.hpp"
#include <string>

EXPORT_VAR youthful_einstein gYouthful_einstein_6F8450;
GLOBAL(gYouthful_einstein_6F8450, 0x6F8450);

MATCH_FUNC(0x516560)
void youthful_einstein::ctor_516560() // For some reason, it's a function instead of a proper ctor
{
    field_0_fugitive = NULL;
    field_1C = 0;
    memset(field_4_time, 0, sizeof(field_4_time));
    memset(field_20, 0, sizeof(field_20));
}

STUB_FUNC(0x516590)
void youthful_einstein::sub_516590(Player* a2)
{
}

STUB_FUNC(0x516660)
s16 youthful_einstein::sub_516660()
{
    return 0;
}

MATCH_FUNC(0x516740)
void youthful_einstein::sub_516740(Player* pFormerPlayerFugitive, Player* pPlayer_killer)
{
    Player* Player_4B9D10;
    Player* pChosenPlayer;
    Ped* pFugitive;

    Fix16 taxicab_distance;
    Fix16 threshold_distance;

    threshold_distance = 0x3FFFC000;
    if (gLucid_hamilton_67E8E0.sub_4C5BC0() == 3 //  3 = tag mode
        && this->field_0_fugitive == pFormerPlayerFugitive)
    {
        if (pPlayer_killer != NULL)
        {
            youthful_einstein::sub_516590(pPlayer_killer); //  Make him "IT"
        }
        else
        {
            Player_4B9D10 = gGame_0x40_67E008->sub_4B9CD0();
            pChosenPlayer = NULL;
            pFugitive = this->field_0_fugitive->field_2C4_player_ped;

            Fix16 fugitive_x = pFugitive->field_1AC_cam.x;
            Fix16 fugitive_y = pFugitive->field_1AC_cam.y;

            Fix16 x1, x2;
            Fix16 y1, y2;
            Fix16 x_abs, y_abs;
            Fix16 x_diff, y_diff;

            for (; Player_4B9D10 != NULL; Player_4B9D10 = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
            {
                if (Player_4B9D10 != this->field_0_fugitive)
                {
                    y1 = Player_4B9D10->field_2C4_player_ped->field_1AC_cam.y;
                    y2 = fugitive_y;
                    y_diff = y1 - y2;
                    x_abs.inline_abs_403840(y_diff);

                    x1 = Player_4B9D10->field_2C4_player_ped->field_1AC_cam.x;
                    x2 = fugitive_x;
                    x_diff = x1 - x2;
                    y_abs.inline_abs_403840(x_diff);

                    taxicab_distance = x_abs + y_abs; //  taxicab geometry
                    if (taxicab_distance < threshold_distance)
                    {
                        threshold_distance = taxicab_distance;
                        pChosenPlayer = Player_4B9D10;
                    }
                }
            }
            /*
            the most far away player from the former fugitive (in taxicab geometry)
            will be the "pChosenPlayer"
            */
            if (pChosenPlayer != NULL)
            {
                youthful_einstein::sub_516590(pChosenPlayer); //  Make him "IT"
            }
        }
    }
}
