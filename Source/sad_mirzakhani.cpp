#include "sad_mirzakhani.hpp"
#include "Function.hpp"
#include "Player.hpp" // eager_benz
#include "rng.hpp"

MATCH_FUNC(0x431D30);
silly_saha_0x2C::silly_saha_0x2C()
{
    sub_431D50();
}

MATCH_FUNC(0x431D40);
silly_saha_0x2C::~silly_saha_0x2C()
{
}

MATCH_FUNC(0x431D50);
void silly_saha_0x2C::sub_431D50()
{
    field_0_pZone = 0;
    field_4 = -1;
    field_8 = 87;
    field_C = 51;
    field_10 = -1;
    field_12 = -2;
    field_14 = 0;
    field_18 = 87;
    field_1C = 0;
    field_20_counterVal = 0;
    field_24 = 0;
    field_25 = 0;
    field_26 = 0;
    field_28 = 0;
    field_2A_bUsed = 0;
    field_2B = 0;
}

MATCH_FUNC(0x431DA0);
void silly_saha_0x2C::sub_431DA0()
{
    sub_431D50();
}

MATCH_FUNC(0x431DB0);
void silly_saha_0x2C::sub_431DB0()
{
    field_2B = 0;
}

// ============

MATCH_FUNC(0x431DC0);
sad_mirzakhani::sad_mirzakhani()
{
    field_1B8 = 0;
    field_1BC = 0;
}

MATCH_FUNC(0x431DF0);
sad_mirzakhani::~sad_mirzakhani()
{
    field_1B8 = 0;
}

MATCH_FUNC(0x431E10);
void sad_mirzakhani::sub_431E10(eager_benz* a2)
{
    field_1BC = 0;
    field_1B8 = a2;
}

MATCH_FUNC(0x431E30);
void sad_mirzakhani::sub_431E30()
{
    field_1BC = rng_dword_67AB34->field_0_rng;

    silly_saha_0x2C* pIter = &field_0[0];
    for (s32 i = GTA2_COUNTOF(field_0) - 1; i >= 0; i--)
    {
        if (pIter->field_2A_bUsed)
        {
            if (pIter->field_2B)
            {
                const u32 f1c = pIter->field_1C;
                if (f1c != -1 && field_1BC - pIter->field_20_counterVal > f1c)
                {
                    pIter->field_26 = 0;
                    pIter->sub_431DB0();
                }
            }
        }
        pIter++;
    }
}

MATCH_FUNC(0x431E90);
u16 sad_mirzakhani::next_free_idx_431E90()
{
    for (u16 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        if (!field_0[i].field_2A_bUsed)
        {
            return i;
        }
    }
    return GTA2_COUNTOF(field_0);
}

WIP_FUNC(0x431EC0);
u16 sad_mirzakhani::find_431EC0(u16 idx,
                                s16 f_4,
                                s32 car_info_idx,
                                s32 occupation,
                                s16 f_10,
                                s16 f_12,
                                s32 f_14,
                                s32 f_18,
                                gmp_map_zone* pZone)
{
    WIP_IMPLEMENTED;

    u16 local_idx; // bp
    silly_saha_0x2C* pItem; // esi
    s16 l_4; // ax
    s32 l_c; // eax
    s32 l_8; // eax
    s16 l_10; // ax
    s16 l_12; // ax
    s32 l_14; // eax
    s32 l_18; // eax

    local_idx = idx;
    if (idx >= 10u)
    {
        return 10;
    }
    while (1)
    {
        pItem = &this->field_0[local_idx];
        if (!pItem->field_2A_bUsed)
        {
            goto inc_idx;
        }
        if (!pItem->field_2B)
        {
            goto inc_idx;
        }
        l_4 = pItem->field_4;
        if (l_4 != f_4 && l_4 != -1)
        {
            goto inc_idx;
        }
        l_c = pItem->field_C;
        if (l_c == occupation || l_c == 51 || sub_432240(occupation, pItem->field_C))
        {
            l_8 = pItem->field_8;
            if (l_8 == car_info_idx || l_8 == 87 || sub_432300(car_info_idx, pItem->field_8))
            {
                l_10 = pItem->field_10;
                if (l_10 == f_10 || l_10 == -1)
                {
                    l_12 = pItem->field_12;
                    if (l_12 == f_12 || l_12 == -2)
                    {
                        l_14 = pItem->field_14;
                        if (l_14 == f_14 || l_14 == 23 || sub_432170(f_14, pItem->field_14))
                        {
                            l_18 = pItem->field_8;
                            if ((l_18 == f_18 || l_18 == 87) && (pItem->field_0_pZone == pZone || !pItem->field_0_pZone))
                            {
                                return local_idx;
                            }
                        }
                    }
                }
            }
        }
        if (pItem->field_24 == 1)
        {
            field_0[local_idx].sub_431DB0();
        }
    inc_idx:
        if (++local_idx >= 10u)
        {
            return 10;
        }
    }
}

MATCH_FUNC(0x431FE0);
u16 sad_mirzakhani::alloc_next_431FE0(s16 f_4,
                                      s32 f_8,
                                      s32 f_c,
                                      s16 f_10,
                                      s16 f_12,
                                      s32 f_14,
                                      s32 f_18,
                                      s32 f_1c,
                                      s8 f_24,
                                      s8 f_25,
                                      u16 f_28,
                                      gmp_map_zone* pZone)
{
    const s16 idx = next_free_idx_431E90();
    if (idx == 10)
    {
        return idx;
    }

    field_0[idx].field_4 = f_4;
    field_0[idx].field_8 = f_8;
    field_0[idx].field_C = f_c;
    field_0[idx].field_10 = f_10;
    field_0[idx].field_12 = f_12;
    field_0[idx].field_14 = f_14;
    field_0[idx].field_18 = f_18;
    field_0[idx].field_0_pZone = pZone;
    field_0[idx].field_20_counterVal = rng_dword_67AB34->field_0_rng;
    field_0[idx].field_1C = f_1c;
    field_0[idx].field_24 = f_24;
    field_0[idx].field_25 = f_25;
    field_0[idx].field_26 = 0;
    field_0[idx].field_28 = f_28;
    field_0[idx].field_2A_bUsed = 1;
    field_0[idx].field_2B = 1;

    return idx;
}

MATCH_FUNC(0x432080);
s16 sad_mirzakhani::sub_432080(u16 idx)
{
    silly_saha_0x2C* pItem = &field_0[idx];
    if (!pItem->field_2A_bUsed)
    {
        return -1;
    }

    if (pItem->field_2B)
    {
        return -2;
    }

    if (pItem->field_26 == pItem->field_25)
    {
        pItem->sub_431DA0();
        return -3;
    }
    else
    {
        pItem->sub_431DA0();
        return -4;
    }
}

WIP_FUNC(0x4320D0);
void sad_mirzakhani::sub_4320D0(s16 f_4, s32 f_8, s32 f_c, s16 f_10, s16 f_12, s32 f_14, s32 f_18, gmp_map_zone* pZone)
{
    WIP_IMPLEMENTED;
    u16 found_idx = 0;
    for (s16 i = 0; i < 10u; i = found_idx + 1)
    {
        found_idx = find_431EC0(i, f_4, f_8, f_c, f_10, f_12, f_14, f_18, pZone);
        // _found_idx = found_idx;
        if (found_idx >= 10u)
        {
            break;
        }
        silly_saha_0x2C* pFound = &field_0[found_idx];
        pFound->field_26++;
        if (pFound->field_26 == pFound->field_25)
        {
            field_1B8->field_368_player->field_2D4_scores.AddCash_592620(field_1B8->field_368_player->field_6BC_multpliers.field_0_value *
                                                                         pFound->field_28);
            pFound->sub_431DB0();
        }
    }
}

WIP_FUNC(0x432170);
s8 sad_mirzakhani::sub_432170(int a2, int a3)
{
    WIP_IMPLEMENTED;
    switch (a3)
    {
        case 1:
            return a2 == 3;
        case 12:
            return a2 == 4;
        case 21:
            return a2 == 4;
        case 15:
            return a2 == 4;
        case 16:
            return a2 == 4;
        case 17:
            return a2 == 4;
        case 14:
            return a2 == 13;
    }

    if (a3 != 22)
    {
        return 0;
    }

    switch (a2)
    {
        case 4:
        case 10:
        case 11:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
            break;
        default:
            return 0;
    }
    return 1;
}

WIP_FUNC(0x432240);
s8 sad_mirzakhani::sub_432240(int occupation, int a3)
{
    WIP_IMPLEMENTED;
    switch (a3)
    {
        case 46:
            switch (occupation)
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
                    return 1;
                default:
                    return 0;
            }
        case 47:
            switch (occupation)
            {
                case ped_ocupation_enum::unknown_13:
                case ped_ocupation_enum::police:
                case ped_ocupation_enum::swat:
                case ped_ocupation_enum::fbi:
                case ped_ocupation_enum::army_army:
                case ped_ocupation_enum::unknown_14:
                case ped_ocupation_enum::unknown_15:
                case ped_ocupation_enum::unknown_16:
                case ped_ocupation_enum::tank_driver:
                case ped_ocupation_enum::unknown_17:
                case ped_ocupation_enum::fireman:
                case ped_ocupation_enum::road_block_tank_man:
                    return 1;
                default:
                    return 0;
            }
        case 48:
            if (occupation == ped_ocupation_enum::unknown_10 || occupation == ped_ocupation_enum::guard || occupation == ped_ocupation_enum::unknown_19)
            {
                return 1;
            }
            break;
        default:
            if (a3 == 49 && (occupation == ped_ocupation_enum::elvis || occupation == ped_ocupation_enum::elvis_leader))
            {
                return 1;
            }
            break;
    }
    return 0;
}

MATCH_FUNC(0x432300);
bool sad_mirzakhani::sub_432300(int car_info_idx_1, int car_info_idx_2)
{
    bool is_fed_car_1;
    bool is_fed_car_2;

    if ((car_info_idx_1 == car_model_enum::TAXI || car_info_idx_1 == car_model_enum::STYPECAB) &&
        (car_info_idx_2 == car_model_enum::TAXI || car_info_idx_2 == car_model_enum::STYPECAB))
    {
        return 1;
    }
    switch (car_info_idx_1)
    {
        case car_model_enum::apc:
        case car_model_enum::COPCAR:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::SWATVAN:
        case car_model_enum::TANK:
        case car_model_enum::EDSELFBI:
            is_fed_car_1 = 1;
            break;
        default:
            is_fed_car_1 = 0;
            break;
    }
    switch (car_info_idx_2)
    {
        case car_model_enum::apc:
        case car_model_enum::COPCAR:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::SWATVAN:
        case car_model_enum::TANK:
        case car_model_enum::EDSELFBI:
            is_fed_car_2 = 1;
            break;
        default:
            is_fed_car_2 = 0;
            break;
    }
    if (is_fed_car_1 && is_fed_car_2)
    {
        return true;
    }
    return false;
}