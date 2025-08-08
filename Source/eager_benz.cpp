#include "eager_benz.hpp"
#include "CarInfo_808.hpp"
#include "Car_B0.hpp"
#include "Car_BC.hpp"
#include "ExplodingScore_100.hpp"
#include "Game_0x40.hpp"
#include "Garox_2B00.hpp"
#include "Globals.hpp"
#include "Ped.inl"
#include "Player.hpp"
#include "Shooey_CC.hpp"
#include "Zones_CA8.hpp"
#include "debug.hpp"
#include "gtx_0x106C.hpp"
#include "lucid_hamilton.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "text_0x14.hpp"
#include <string.h>

// TODO: move
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);
EXTERN_GLOBAL(Shooey_CC*, gShooey_CC_67A4B8);

// GLOBAL: 105 0x7028BC
DEFINE_GLOBAL_INIT(Fix16, dword_7028BC, 1638);

// FUNCTION: 105 0x591bd0
eager_benz::eager_benz()
{
    field_368_pObj = 0;

    field_74 = 1;
    field_75_score_mult = 1;

    field_18C = 0;
    field_190_fly_car_count = 0;
    field_70 = 0;
    field_78 = 0;
    field_7C_e_execution_count = 0;
    field_86_total_kills = 0;
    field_88_killed_cops = 0;
    field_8A_cars_stolen_count = 0;
    field_80 = 0;
    field_84_num_elvis_killed = 0;
    field_194 = 0;
    field_198_accuracy_count = 0;
    field_19C_reverse_count = 0;
    field_1A0 = 0;
    field_1A4_killed_cars_flags = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_8C); i++)
    {
        field_8C[i] = 0;
    }
}

// STRING: 105 0x6252c8
#define EXCUTIN_STRING "excutin"

// STRING: 105 0x6252C0
#define ELVIS_D_STRING "elvis_d"

// STRING: 105 0x6252b8
#define EM_DEST_STRING "em_dest"

// STRING: 105 0x6252b0
#define GENCIDE_STRING "gencide"

// STRING: 105 0x6252a8
#define COPKILL_STRING "copkill"

// STRING: 105 0x625298
#define ACCURCY_STRING "accurcy"

// STRING: 105 0x625290
#define WRNGWAY_STRING "wrngway"

// STRING: 105 0x625288
#define FLY_CAR_STRING "fly_car"

// STRING: 105 0x620d84
#define G_OVER_STRING "g_over"

// STRING: 105 0x6252A0
#define CARJACKA_STRING "carjacka"

// FUNCTION: 105 0x591c70
void eager_benz::sub_591C70()
{
    field_1A8_unk.sub_431E30();
    Ped* field_2C4_player_ped = this->field_368_pObj->field_2C4_player_ped;
    field_18C += gGame_0x40_67E008->sub_4B8BB0();

    if (field_18C >= 1000)
    {
        this->field_18C -= 1000;
        Car_BC* field_16C_car = field_2C4_player_ped->field_16C_car;

        if (field_16C_car)
        {
            if (field_2C4_player_ped->field_248_enter_car_as_passenger != 1 &&
                (gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_16C_car->field_84_car_info_idx)->info_flags & 0x20) == 0x20)
            {
                if (field_16C_car->field_4.field_0_pOwner)
                {
                    field_368_pObj->field_2D4_unk.AddCash_592620(this->field_368_pObj->field_6BC_multpliers.field_0);
                }
            }
        }
        if (field_2C4_player_ped->field_20A_wanted_points >= 5000)
        {
            this->field_368_pObj->field_2D4_unk.AddCash_592620(this->field_368_pObj->field_6BC_multpliers.field_0);
        }

        field_368_pObj->field_644_unk.sub_484FB0(field_2C4_player_ped->get_wanted_star_count_46EF00());
    }

    if (this->field_7C_e_execution_count >= 20u)
    {
        this->field_7C_e_execution_count = 0;
        field_368_pObj->field_2D4_unk.AddCash_592620(100000 * field_368_pObj->field_6BC_multpliers.field_0);

        if (this->field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(EXCUTIN_STRING), 1);
            gRoot_sound_66B038.sub_40F090(4);
        }
    }

    if (this->field_84_num_elvis_killed >= 6u)
    {
        this->field_84_num_elvis_killed = 0;
        field_368_pObj->field_2D4_unk.AddCash_592620(30000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (this->field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(ELVIS_D_STRING), 1);
            gRoot_sound_66B038.sub_40F090(8);
        }
    }

    if (this->field_1A4_killed_cars_flags == 7)
    {
        this->field_1A4_killed_cars_flags = 0;
        field_368_pObj->field_2D4_unk.AddCash_592620(10000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (this->field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(EM_DEST_STRING), 1);
            gRoot_sound_66B038.sub_40F090(11);
        }
    }

    if (this->field_86_total_kills >= 1000)
    {
        this->field_86_total_kills = 0;
        field_368_pObj->field_2D4_unk.AddCash_592620(30000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (this->field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(GENCIDE_STRING), 1);
            gRoot_sound_66B038.sub_40F090(5);
        }
    }

    if (this->field_88_killed_cops >= 20u)
    {
        this->field_88_killed_cops = 0;
        field_368_pObj->field_2D4_unk.AddCash_592620(5000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (this->field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(COPKILL_STRING), 1);
            gRoot_sound_66B038.sub_40F090(6);
        }
    }

    if (this->field_8A_cars_stolen_count >= 100)
    {
        this->field_8A_cars_stolen_count = 0;
        field_368_pObj->field_2D4_unk.AddCash_592620(10000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (this->field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(CARJACKA_STRING), 1);
            gRoot_sound_66B038.sub_40F090(7);
        }
    }

    if (this->field_198_accuracy_count >= 25)
    {
        this->field_198_accuracy_count = 0;
        field_368_pObj->field_2D4_unk.AddCash_592620(5000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (this->field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(ACCURCY_STRING), 1);
            gRoot_sound_66B038.sub_40F090(9);
        }
    }

    //Car_B0* tmp4;
    Car_BC* tmp3;
    if (field_2C4_player_ped->get_wanted_points_433DC0() > 3000 && (field_2C4_player_ped->has_car_403B80()) &&
        field_2C4_player_ped->not_enter_car_as_passenger_4A5040() &&
        (tmp3 = field_2C4_player_ped->get_car_416B60()) != 0 && // null check optimized away ??
        (tmp3->field_58_physics) != 0 &&

        tmp3->field_58_physics->is_backward_gas_on_411810())
    {
        this->field_19C_reverse_count += gGame_0x40_67E008->sub_4B8BB0();
    }
    else
    {
        this->field_19C_reverse_count = 0;
    }

    if (this->field_19C_reverse_count >= 60000u)
    {
        this->field_19C_reverse_count = 0;
        field_368_pObj->field_2D4_unk.AddCash_592620(1000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (this->field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(WRNGWAY_STRING), 1);
            gRoot_sound_66B038.sub_40F090(10);
        }
    }

    Car_BC* v24 = field_2C4_player_ped->get_car_416B60();
    if (v24 && field_2C4_player_ped->not_enter_car_as_passenger_4A5040() && (v24->field_58_physics) != 0 &&
        v24->field_58_physics->sub_55A0B0() // TODO: Wrong stack
        && v24->sub_43A4C0() > dword_7028BC)
    {
        this->field_190_fly_car_count += gGame_0x40_67E008->sub_4B8BB0();
    }
    else
    {
        this->field_190_fly_car_count = 0;
    }

    // v26 = this->field_190_fly_car_count;
    if (field_190_fly_car_count >= 1250 && field_190_fly_car_count < 2250)
    {
        this->field_368_pObj->field_2D4_unk.AddCash_592620(1000 * this->field_368_pObj->field_6BC_multpliers.field_0);
        this->field_190_fly_car_count = 2250;
        if (field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(FLY_CAR_STRING), 1);
            gRoot_sound_66B038.sub_40F090(1);
        }
    }

    if (bStartNetworkGame_7081F0)
    {
        s32 v30; // edi
        u8 a2 = this->field_368_pObj->field_2E_idx;
        u8 v29 = gLucid_hamilton_67E8E0.sub_4C5BC0();
        s32 v34 = gLucid_hamilton_67E8E0.sub_4C5BD0();

        if (v29 == 1) // di vs bl
        {
            s16 t = gLucid_hamilton_67E8E0.sub_4C5D60(a2);
            v30 = t;
            sub_5935C0();
        }
        else if (v29 == 2)
        {
            v30 = gLucid_hamilton_67E8E0.sub_4C5CB0(a2);
            sub_592370();
        }

        if (v29 != 3)
        {
            if (v30 >= v34) // TODO: di vs edi
            {
                gLucid_hamilton_67E8E0.sub_4C5C00(a2);
                if (gGame_0x40_67E008->field_28_timer == -1)
                {
                    gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(G_OVER_STRING), 3);
                }
                gGame_0x40_67E008->sub_4B8C00(2, 5);
            }
        }
    }

    // Handle the previous LABEL_63 section
    const s32 field_0_rng = rng_dword_67AB34->field_0_rng; // TODO: inline

    if ((u32)(rng_dword_67AB34->field_0_rng - this->field_70) > 15)
    {
        this->field_74 = 1;
    }

    if ((u32)(field_0_rng - field_78) > 15)
    {
        field_75_score_mult = 1;
    }
}

// FUNCTION: 105 0x5922f0
void eager_benz::sub_5922F0(Player* a2, s16 a3, s32 a4, s16 a5, u16 a6)
{
    field_368_pObj = a2;
    field_0_unk.sub_492110(a3, a4, a5);
    field_38_unk.sub_492110(a3, a6, a5);
}

// FUNCTION: 105 0x592330
void eager_benz::sub_592330()
{
    field_0_unk.sub_492150();
    field_38_unk.sub_492150();
    field_1A8_unk.sub_431E10(this);
    sub_592380(3);
}

// FUNCTION: 105 0x592360
thirsty_lamarr* eager_benz::sub_592360()
{
    return &field_0_unk;
}

// FUNCTION: 105 0x592370
s32 eager_benz::sub_592370()
{
    return field_0_unk.field_0;
}

// FUNCTION: 105 0x592380
void eager_benz::sub_592380(char_type bits)
{
    if ((bits & 1) != 0)
    {
        for (u16 i = 0; i < 256; i++)
        {
            if (gGtx_0x106C_703DD4->does_car_exist(i) && gGtx_0x106C_703DD4->sub_5AB380(i))
            {
                field_8C[i] &= ~1;
            }
            else
            {
                field_8C[i] |= 1;
            }
        }
    }

    if ((bits & 2) != 0)
    {
        for (u16 i = 0; i < 256; i++)
        {
            if (gGtx_0x106C_703DD4->does_car_exist(i) && gGtx_0x106C_703DD4->sub_5AB380(i))
            {
                const u8 wreck = gGtx_0x106C_703DD4->get_car_info_5AA3B0(i)->wreck;

                if (wreck == 99)
                {
                    field_8C[i] |= 2;
                }
                else
                {
                    field_8C[i] &= ~2;
                }
            }
            else
            {
                field_8C[i] |= 2;
            }
        }
    }
}
// STRING: 105 0x6252d8
#define STL_ALL_STRING "stl_all"

// STRING: 105 0x6252D0
#define DST_ALL_STRING "dst_all"

// FUNCTION: 105 0x592430
void eager_benz::sub_592430(char_type bits)
{
    u16 i;

    if ((bits & 1) != 0)
    {
        for (i = 0; i < GTA2_COUNTOF(field_8C); i++)
        {
            if ((field_8C[i] & 1) == 0)
            {
                return;
            }
        }

        field_368_pObj->field_2D4_unk.AddCash_592620(30000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(STL_ALL_STRING), 1);
            gRoot_sound_66B038.sub_40F090(2);
        }
        sub_592380(1);
    }
    else if ((bits & 2) != 0)
    {
        for (i = 0; i < GTA2_COUNTOF(field_8C); i++)
        {
            if ((field_8C[i] & 2) == 0)
            {
                return;
            }
        }

        field_368_pObj->field_2D4_unk.AddCash_592620(50000 * field_368_pObj->field_6BC_multpliers.field_0);
        if (field_368_pObj->field_0)
        {
            gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90(DST_ALL_STRING), 1);
            gRoot_sound_66B038.sub_40F090(3);
        }
        sub_592380(2);
    }
}

// FUNCTION: 105 0x592570
void eager_benz::sub_592570(char_type a2, s32 a3)
{
    field_8C[a3] |= a2;
    sub_592430(a2);
}

// FUNCTION: 105 0x5925b0
s32 eager_benz::sub_5925B0(u32 car_info_idx, u8 arg4)
{
    u32 result = gCarInfo_808_678098->sub_4546B0(car_info_idx)->field_2_value;

    switch (arg4)
    {
        case 0:
            return result;
        case 1:
            return 2 * result;
        case 2:
            return 5 * result;
        default:
            return 0;
    }
}

// FUNCTION: 105 0x592620
void eager_benz::AddCash_592620(s32 cash)
{
    field_0_unk.sub_4921B0(cash);

    if (bStartNetworkGame_7081F0)
    {
        gLucid_hamilton_67E8E0.sub_4C5C80(field_368_pObj->field_2E_idx, cash);
    }
}

// STUB: 105 0x592660
void eager_benz::sub_592660(Ped* pPed1, Ped* pPed2)
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x592dd0
void eager_benz::sub_592DD0(Car_BC* pCar, Ped* pPed)
{
    const s32 multipler = field_368_pObj->field_6BC_multpliers.field_0;
    gmp_map_zone* pZone = gMap_0x370_6F6268->sub_4DF6A0(pPed->get_cam_x().ToInt(), pPed->get_cam_y().ToInt());

    u32 car_info_idx = pPed->get_car_model();

    u16 bIsGangCar = gZones_CA8_67E274->sub_4BF2F0(pCar->field_84_car_info_idx);

    field_1A8_unk.sub_4320D0(1,
                             pCar->field_84_car_info_idx,
                             51,
                             bIsGangCar,
                             pCar->field_50_car_sprite->field_24_remap,
                             pCar->field_90,
                             car_info_idx,
                             pZone);

    u8 bCopSwatOrFbiCar = 1;
    if (bIsFrench_67D53C)
    {
        if (pCar->sub_439EC0())
        {
            bCopSwatOrFbiCar = 0;
        }
    }

    s32 cur_rng_2 = rng_dword_67AB34->field_0_rng;
    if (pCar->field_84_car_info_idx == car_model_enum::FIRETRUK || pCar->field_84_car_info_idx == car_model_enum::COPCAR ||
        pCar->field_84_car_info_idx == car_model_enum::MEDICAR || pCar->field_84_car_info_idx == car_model_enum::SWATVAN ||
        pCar->field_84_car_info_idx == car_model_enum::EDSELFBI)
    {

        if ((unsigned int)(rng_dword_67AB34->field_0_rng - field_1A0) > 150)
        {
            field_1A4_killed_cars_flags = 0;
        }

        u32 car_type = pCar->field_84_car_info_idx;

        if (car_type == car_model_enum::MEDICAR)
        {
            field_1A4_killed_cars_flags |= 1;
        }
        else if (car_type == car_model_enum::COPCAR || car_type == car_model_enum::SWATVAN || car_type == car_model_enum::EDSELFBI)
        {
            field_1A4_killed_cars_flags |= 2;
        }
        else if (car_type == car_model_enum::FIRETRUK)
        {
            field_1A4_killed_cars_flags |= 4;
        }

        field_1A0 = cur_rng_2;
    }
    if (pCar->field_84_car_info_idx == car_model_enum::COPCAR && bCopSwatOrFbiCar)
    {
        field_88_killed_cops++;
    }

    u32 tt = sub_5925B0(pCar->field_84_car_info_idx, 2);
    u8 t = field_74;

    u32 kill_car_score = tt * t;
    if (!bExplodingOff_67D4FB)
    {
        if (bCopSwatOrFbiCar)
        {
            if (field_368_pObj->field_0)
            {
                gExplodingScore_100_702F34->sub_596890(pCar->field_50_car_sprite->GetXPos(),
                                                       pCar->field_50_car_sprite->GetYPos(),
                                                       pCar->field_50_car_sprite->GetZPos(),
                                                       multipler * kill_car_score);
            }
        }
    }

    if (bCopSwatOrFbiCar)
    {
        field_368_pObj->field_2D4_unk.AddCash_592620(kill_car_score * this->field_368_pObj->field_6BC_multpliers.field_0);
    }

    field_70 = cur_rng_2;
    if (field_74 < 5u)
    {
        field_74++;
    }

    if (gShooey_CC_67A4B8->sub_485090(pCar, this->field_368_pObj))
    {
        gShooey_CC_67A4B8->ReportCrimeForPed(3u, field_368_pObj->Get_Field_68_Ped());
    }
    field_368_pObj->field_644_unk.sub_484FA0(multipler * kill_car_score);
    sub_592570(2, pCar->field_84_car_info_idx);
}

// FUNCTION: 105 0x593030
void eager_benz::sub_593030(Car_BC* pCar, s16 score_default)
{
    bool bAddScore = true;
    s32 mutipler = this->field_368_pObj->field_6BC_multpliers.field_0;

    if (bIsFrench_67D53C)
    {
        if (pCar->sub_439EC0())
        {
            bAddScore = false;
        }
    }

    u32 score_default_2 = score_default;
    if (score_default_2 > 0)
    {
        int base_score;
        if (score_default_2 < 300)
        {
            base_score = 1;
        }
        else
        {
            base_score = score_default_2 < 400 ? 10 : 100;
        }

        if (!bExplodingOff_67D4FB)
        {
            if (bAddScore)
            {
                if (this->field_368_pObj->field_0)
                {
                    gExplodingScore_100_702F34->sub_596890(pCar->field_50_car_sprite->GetXPos(),
                                                           pCar->field_50_car_sprite->GetYPos(),
                                                           pCar->field_50_car_sprite->GetZPos(),
                                                           mutipler * base_score);
                }
            }
        }

        if (bAddScore)
        {
            field_368_pObj->field_2D4_unk.AddCash_592620(base_score * field_368_pObj->field_6BC_multpliers.field_0);
        }

        field_368_pObj->field_644_unk.sub_484FA0(mutipler * base_score);
        if (gShooey_CC_67A4B8->sub_485090(pCar, field_368_pObj))
        {
            gShooey_CC_67A4B8->ReportCrimeForPed(1u, field_368_pObj->Get_Field_68_Ped());
        }
    }
}

// FUNCTION: 105 0x593150
void eager_benz::sub_593150(Car_BC* pCar, s16 a3)
{
    if (pCar->field_74_damage != 32001)
    {
        const s32 multipler = field_368_pObj->field_6BC_multpliers.field_0;
        u32 t = a3;
        if (t > 0)
        {
            s32 base_score;
            if (t < 300)
            {
                base_score = 1;
            }
            else
            {
                base_score = t < 400 ? 10 : 100;
            }
            if (!bIsFrench_67D53C || !pCar->sub_439EC0())
            {
                field_368_pObj->field_2D4_unk.AddCash_592620(base_score * field_368_pObj->field_6BC_multpliers.field_0);
            }
            field_368_pObj->field_644_unk.sub_484FA0(multipler * base_score);

            gShooey_CC_67A4B8->ReportCrimeForPed(1u, field_368_pObj->Get_Field_68_Ped());
        }
    }
}

// FUNCTION: 105 0x593220
void eager_benz::sub_593220()
{
    field_368_pObj->field_2D4_unk.AddCash_592620(field_368_pObj->field_6BC_multpliers.field_0 * 20);
}

// FUNCTION: 105 0x593240
void eager_benz::sub_593240(Car_BC* pCar)
{
    const s32 multipler = field_368_pObj->field_6BC_multpliers.field_0;
    gmp_map_zone* pMapZone = gMap_0x370_6F6268->sub_4DF6A0(field_368_pObj->field_2C4_player_ped->get_cam_x().ToInt(),
                                                           field_368_pObj->field_2C4_player_ped->get_cam_y().ToInt());

    const u16 zone_ret = gZones_CA8_67E274->sub_4BF2F0(pCar->field_84_car_info_idx);
    field_1A8_unk.sub_4320D0(2, pCar->field_84_car_info_idx, 51, zone_ret, pCar->field_50_car_sprite->field_24_remap, 23, 87, pMapZone);

    field_8A_cars_stolen_count++;

    const s32 base_score = sub_5925B0(pCar->field_84_car_info_idx, 0);
    if (!bExplodingOff_67D4FB && field_368_pObj->field_0)
    {
        gExplodingScore_100_702F34->sub_596890(pCar->field_50_car_sprite->GetXPos(),
                                               pCar->field_50_car_sprite->GetYPos(),
                                               pCar->field_50_car_sprite->GetZPos(),
                                               multipler * base_score);
    }
    field_368_pObj->Add_2D4(base_score);

    gShooey_CC_67A4B8->ReportCrimeForPed(5u, field_368_pObj->Get_Field_68_Ped());
    sub_592570(1, pCar->field_84_car_info_idx);
}

// FUNCTION: 105 0x593370
void eager_benz::sub_593370(Car_BC* pCar)
{
    if (!bExplodingOff_67D4FB && field_368_pObj->field_0)
    {
        gExplodingScore_100_702F34->sub_596890(pCar->field_50_car_sprite->GetXPos(),
                                               pCar->field_50_car_sprite->GetYPos(),
                                               pCar->field_50_car_sprite->GetZPos(),
                                               field_368_pObj->field_6BC_multpliers.field_0 * 10);
    }

    field_368_pObj->field_2D4_unk.AddCash_592620(field_368_pObj->field_6BC_multpliers.field_0 * 10);
    gShooey_CC_67A4B8->ReportCrimeForPed(4u, field_368_pObj->Get_Field_68_Ped());
}

// FUNCTION: 105 0x593410
void eager_benz::sub_593410(Car_BC* pCar)
{
    const s32 multpliers = field_368_pObj->field_6BC_multpliers.field_0;
    if (!bExplodingOff_67D4FB)
    {
        if (field_368_pObj->field_0)
        {
            gExplodingScore_100_702F34->sub_596890(pCar->field_50_car_sprite->GetXPos(),
                                                   pCar->field_50_car_sprite->GetYPos(),
                                                   pCar->field_50_car_sprite->GetZPos(),
                                                   100 * multpliers);
        }
    }

    field_368_pObj->Add_2D4(100);
    field_368_pObj->field_644_unk.sub_484FA0(100 * multpliers);

    if (gShooey_CC_67A4B8->sub_485090(pCar, field_368_pObj))
    {
        gShooey_CC_67A4B8->ReportCrimeForPed(3u, field_368_pObj->Get_Field_68_Ped());
    }
}

// FUNCTION: 105 0x5934f0
void eager_benz::UpdateAccuracyCount_5934F0(u32 a2, s32 model, Ped* pPed)
{
    field_194 = a2;
    if (pPed && bIsFrench_67D53C)
    {
        switch (pPed->field_240_occupation)
        {
            case ped_ocupation_enum::police:
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::fbi:
            case ped_ocupation_enum::army_army:
            case ped_ocupation_enum::unknown_14:
            case ped_ocupation_enum::unknown_15:
            case ped_ocupation_enum::unknown_16:
            case ped_ocupation_enum::tank_driver:
            case ped_ocupation_enum::unknown_17:
            case ped_ocupation_enum::road_block_tank_man:
                field_198_accuracy_count = 0;
                return;
            default:
                break;
        }
    }

    if (model == 194 || model == 198 || model == 154 || model == 193 || model == 195 || model == 159 || model == 199 || a2 == 0 ||
        a2 == 1 || a2 == 3)
    {
        field_198_accuracy_count = 0;
    }
    else if (a2 == 2)
    {
        field_198_accuracy_count++;
    }
}

// FUNCTION: 105 0x5935b0
thirsty_lamarr* eager_benz::sub_5935B0()
{
    return &field_38_unk;
}

// FUNCTION: 105 0x5935c0
s32 eager_benz::sub_5935C0()
{
    return field_38_unk.field_0;
}

// FUNCTION: 105 0x5935d0
void eager_benz::sub_5935D0(s32 a2)
{
    field_38_unk.sub_4921B0(a2);
}