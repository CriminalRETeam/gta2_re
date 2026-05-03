#include "TrafficLights_194.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Object_5C.hpp"
#include "map_0x370.hpp"
#include "nostalgic_ellis_0x28.hpp"
#include "Light_1D4CC.hpp"
#include "Car_BC.hpp"
#include "debug.hpp"

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

DEFINE_GLOBAL(TrafficLights_194*, gTrafficLights_194_705958, 0x705958);

DEFINE_GLOBAL_INIT(Fix16, dword_70580C, Fix16(1), 0x70580C);
DEFINE_GLOBAL_INIT(Fix16, dword_705810, Fix16(2), 0x705810);
DEFINE_GLOBAL_INIT(Fix16, dword_705674, Fix16(0.5), 0x705674);
DEFINE_GLOBAL_INIT(Fix16, dword_705714, Fix16(0.5), 0x705714);

DEFINE_GLOBAL_INIT(Fix16, dword_705678, Fix16(0x2666, 0), 0x705678);

DEFINE_GLOBAL_INIT(Fix16, dword_7055E4, dword_70580C, 0x7055E4);
DEFINE_GLOBAL_INIT(Fix16, dword_7055E0, dword_705810, 0x7055E0);
DEFINE_GLOBAL_INIT(Fix16, dword_705660, dword_705678, 0x705660);

DEFINE_GLOBAL_INIT(Fix16, dword_7057C0, Fix16(256, 0), 0x7057C0);
DEFINE_GLOBAL_INIT(Fix16, dword_7058C4, dword_7057C0, 0x7058C4);
DEFINE_GLOBAL_INIT(Fix16, dword_7055FC, dword_7058C4 * 6, 0x7055FC);

DEFINE_GLOBAL_INIT(Fix16, dword_705850, dword_70580C + dword_705674, 0x705850);

DEFINE_GLOBAL_INIT(Ang16, word_705788, Ang16(1080), 0x705788);
DEFINE_GLOBAL_INIT(Ang16, word_705690, Ang16(360), 0x705690);
DEFINE_GLOBAL_INIT(Ang16, word_705762, Ang16(720), 0x705762);
DEFINE_GLOBAL_INIT(Ang16, word_705948, Ang16(0), 0x705948);

// TODO: Is this too big?
u8 traffic_light_phase_timers_626840[] = {0x00, 0x64, 0x18, 0x18, 0x64, 0x18, 0x18, 0x96, 0x50, 0x00, 0x00, 0x00};

// https://decomp.me/scratch/JogBj
WIP_FUNC(0x5C1D00)
void TrafficLight_20::sub_5C1D00(u8 x, u8 y, u8 w, u8 h)
{
    WIP_IMPLEMENTED;

    Fix16 zpos;
    gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&zpos, x, y);
    u8 zTmp = (zpos.ToInt()) - 1;

    // West side
    if (gMap_0x370_6F6268->IsBlockRoadType_42A8C0(x - 1, h + y - 1, zTmp))
    {
        // OK
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(170, Fix16(x) - dword_7055E4, Fix16(y + (h / 2)), dword_7055E0 + zpos, word_705788);
        field_8 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(255,
                                                          Fix16(x) - dword_7055FC - dword_7055E4,
                                                          Fix16(y + (h / 2)),
                                                          zpos + dword_7055E0,
                                                          word_705788);
        field_8->sub_5290C0(0);
        field_18 = gLight_1D4CC_6F5520->Init_469010(Fix16(x) - dword_7055E4, Fix16(y + (h / 2)), zpos, 0xFF0000, dword_705850, 200);
    }
    else
    {
        field_8 = 0;
    }

    // East side
    if (gMap_0x370_6F6268->IsBlockRoadType_42A8C0(x + w, y, zTmp))
    {
        // OK
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(170, Fix16(x + w) + dword_7055E4, Fix16(y + (h / 2)), dword_7055E0 + zpos, word_705690);
        field_C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(255,
                                                          Fix16(x + w) + dword_7055FC + dword_7055E4,
                                                          Fix16(y + (h / 2)),
                                                          zpos + dword_7055E0,
                                                          word_705690);
        field_C->sub_5290C0(0);
        field_1C = gLight_1D4CC_6F5520->Init_469010(Fix16(x + w) + dword_7055E4, Fix16(y + (h / 2)), zpos, 0xFF0000, dword_705850, 200);
    }
    else
    {
        field_C = 0;
    }

    // North side
    if (gMap_0x370_6F6268->IsBlockRoadType_42A8C0(x, y - 1, zTmp))
    {
        // OK
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(170, Fix16(x + (w / 2)), Fix16(y) - dword_7055E4, dword_7055E0 + zpos, word_705762);
        field_0 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(255,
                                                          Fix16(x + (w / 2)) - dword_7055FC - dword_7055E4,
                                                          Fix16(y),
                                                          zpos + dword_7055E0,
                                                          word_705762);
        field_0->sub_5290C0(0);
        field_10 = gLight_1D4CC_6F5520->Init_469010(Fix16(x + (w / 2)), Fix16(y) - dword_7055E4, zpos, 0x00FF00, dword_705850, 200);
    }
    else
    {
        field_0 = 0;
    }

    // South side
    if (gMap_0x370_6F6268->IsBlockRoadType_42A8C0(x + w - 1, y + h, zTmp))
    {
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(170, Fix16(x + (w / 2)), Fix16(y + h) + dword_7055E4, dword_7055E0 + zpos, word_705948);
        field_4 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(255,
                                                          Fix16(x + (w / 2)),
                                                          Fix16(y + h) + dword_7055E4 + dword_7055FC,
                                                          zpos + dword_7055E0,
                                                          word_705948);
        field_4->sub_5290C0(0);
        field_14 = gLight_1D4CC_6F5520->Init_469010(Fix16(x + (w / 2)), Fix16(y + h) + dword_7055E4, zpos, 0x00FF00, dword_705850, 200);
    }
    else
    {
        field_4 = 0;
    }

    if ((field_8 || field_0) && gMap_0x370_6F6268->IsBlockPavementType_42A8C0(x - 1, y - 1, zTmp))
    {
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(258, Fix16(x) - dword_705714, Fix16(y) - dword_705714, zpos, word_705948);
    }

    if ((field_C || field_0) && gMap_0x370_6F6268->IsBlockPavementType_42A8C0(x + w, y - 1, zTmp))
    {
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(258, Fix16(w + x) + dword_705714, Fix16(y) - dword_705714, zpos, word_705948);
    }

    if ((field_C || field_4) && gMap_0x370_6F6268->IsBlockPavementType_42A8C0(x + w, h + y, zTmp))
    {
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(258, Fix16(w + x) + dword_705714, Fix16(y + h) + dword_705714, zpos, word_705948);
    }

    if ((field_8 || field_4) && gMap_0x370_6F6268->IsBlockPavementType_42A8C0(x - 1, y + h, zTmp))
    {
        gObject_5C_6F8F84->NewPhysicsObj_5299B0(258, Fix16(x) - dword_705714, Fix16(y + h) + dword_705714, zpos, word_705948);
    }

    if (field_0)
    {
        Fix16 xIter1(x);

        for (u8 i = 0; i < w; i++)
        {
            gmp_block_info* pBlock1 = gMap_0x370_6F6268->get_block_4DFE10((xIter1 + dword_705714).ToInt(),
                                                                          (Fix16(y) - dword_705660).ToInt(),
                                                                          (zpos - dword_70580C).ToInt());
            if (pBlock1 && (pBlock1->field_A_arrows & 8) != 0)
            {
                gObject_5C_6F8F84->NewPhysicsObj_5299B0(122, xIter1 + dword_705714, Fix16(y) - dword_705660, zpos, word_705948);
            }
            xIter1 += Fix16(1);
        }
    }

    if (field_4)
    {
        Fix16 xIter2(x);
        for (u8 i = 0; i < w; i++)
        {
            gmp_block_info* pBlock2 = gMap_0x370_6F6268->get_block_4DFE10((xIter2 + dword_705714).ToInt(),
                                                                          (Fix16(y + h) + dword_705660).ToInt(),
                                                                          (zpos - dword_70580C).ToInt());
            if (pBlock2 && (pBlock2->field_A_arrows & 4) != 0)
            {
                gObject_5C_6F8F84->NewPhysicsObj_5299B0(122, xIter2 + dword_705714, Fix16(y + h) + dword_705660, zpos, word_705948);
            }
            xIter2 += Fix16(1);
        }
    }

    if (field_C)
    {
        Fix16 yOff(y);
        for (u8 i = 0; i < h; h++)
        {
            gmp_block_info* pBlock3 = gMap_0x370_6F6268->get_block_4DFE10((Fix16(x + w) + dword_705660).ToInt(),
                                                                          (yOff + dword_705714).ToInt(),
                                                                          (zpos - dword_70580C).ToInt());
            if (pBlock3 && (pBlock3->field_A_arrows & 1) != 0)
            {
                gObject_5C_6F8F84->NewPhysicsObj_5299B0(122, Fix16(x + w) + dword_705660, yOff + dword_705714, zpos, word_705690);
            }
            yOff += Fix16(1);
        }
    }

    if (field_8)
    {
        Fix16 yOff(y);
        for (u8 i = 0; i < h; h++)
        {
            gmp_block_info* pBlock4 = gMap_0x370_6F6268->get_block_4DFE10((Fix16(x) - (dword_705660)).ToInt(),
                                                                          (yOff + dword_705714).ToInt(),
                                                                          (zpos - dword_70580C).ToInt());
            if (pBlock4 && (pBlock4->field_A_arrows & 2) != 0)
            {
                gObject_5C_6F8F84->NewPhysicsObj_5299B0(122, Fix16(x) - dword_705660, yOff + dword_705714, zpos, word_705690);
            }
            yOff += Fix16(1);
        }
    }
}

MATCH_FUNC(0x5C27A0)
void TrafficLight_20::UpdateLightsFromPhase_5C27A0(u8 phase)
{
    switch (phase)
    {
        case 1u:
            if (field_0)
            {
                field_0->sub_5290C0(1u);
                field_10->field_10_argb = 0xFF00;
            }

            if (field_4)
            {
                field_4->sub_5290C0(1u);
                field_14->field_10_argb = 0xFF00;
            }
            break;

        case 2u:
            if (field_0)
            {
                field_0->sub_5290C0(2u);
                field_10->field_10_argb = 0xFF8000;
            }

            if (field_4)
            {
                field_4->sub_5290C0(2u);
                field_14->field_10_argb = 0xFF8000;
            }
            break;

        case 3u:
            if (field_0)
            {
                field_0->sub_5290C0(0);
                field_10->field_10_argb = 0xFF0000;
            }

            if (field_4)
            {
                field_4->sub_5290C0(0);
                field_14->field_10_argb = 0xFF0000;
            }
            break;

        case 4u:
            if (field_C)
            {
                field_C->sub_5290C0(1u);
                field_1C->field_10_argb = 0xFF00;
            }

            if (field_8)
            {
                field_8->sub_5290C0(1u);
                field_18->field_10_argb = 0xFF00;
            }
            break;

        case 5u:
            if (field_C)
            {
                field_C->sub_5290C0(2u);
                field_1C->field_10_argb = 0xFF8000;
            }

            if (field_8)
            {
                field_8->sub_5290C0(2u);
                field_18->field_10_argb = 0xFF8000;
            }
            break;

        case 6u:
            if (field_C)
            {
                field_C->sub_5290C0(0);
                field_1C->field_10_argb = 0xFF0000;
            }

            if (field_8)
            {
                field_8->sub_5290C0(0);
                field_18->field_10_argb = 0xFF0000;
            }
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5c2910)
void TrafficLights_194::sub_5C2910(u8 x, u8 y, u8 w, u8 h)
{
    TrafficLight_20* pMem = new TrafficLight_20();
    field_0_traffic_lights[field_190_array_used_count++] = pMem;
    pMem->sub_5C1D00(x, y, w, h);
}

MATCH_FUNC(0x5c2950)
void TrafficLights_194::TrafficLightsService_5C2950()
{
    field_193_timer--;

    if (field_193_timer == 0)
    {
        if (field_192_phase == 5)
        {
            gCar_6C_677930->sub_4C39F0();
        }

        field_192_phase++;

        if (field_192_phase > 8) // TODO: Why is  GTA2_COUNTOF_S(traffic_light_phase_timers_626840) wrong?
        {
            field_192_phase = 1;
        }

        field_193_timer = traffic_light_phase_timers_626840[field_192_phase];

        for (u32 i = 0; i < field_190_array_used_count; i++)
        {
            field_0_traffic_lights[i]->UpdateLightsFromPhase_5C27A0(field_192_phase);
        }
    }

    if (bDo_show_traffic_lights_info_67D4FA)
    {
        ShowTrafficLightsInfo_5C2A10();
    }
}

MATCH_FUNC(0x5c2a10)
void TrafficLights_194::ShowTrafficLightsInfo_5C2A10()
{
    swprintf(tmpBuff_67BD9C, L"timer: %d", field_193_timer);
    gHud_2B00_706620->field_650.DisplayText_5D1F50(tmpBuff_67BD9C, -1, 0, word_706600, 1);

    swprintf(tmpBuff_67BD9C, L"phase: %d", field_192_phase);
    gHud_2B00_706620->field_650.DisplayText_5D1F50(tmpBuff_67BD9C, -1, 16, word_706600, 1);

    if (field_192_phase == 7)
    {
        gHud_2B00_706620->field_650.DisplayText_5D1F50(L"peds crossing now!", -1, 32, word_706600, 1);
    }
}

MATCH_FUNC(0x5c2ac0)
void TrafficLights_194::sub_5C2AC0()
{
    gmp_map_zone* pZoneIter = gMap_0x370_6F6268->first_zone_by_type_4DF1D0(Traffic_light_2);
    while (pZoneIter)
    {
        sub_5C2910(pZoneIter->field_1_x, pZoneIter->field_2_y, pZoneIter->field_3_w, pZoneIter->field_4_h);
        pZoneIter = gMap_0x370_6F6268->next_zone_4DF770();
    }
}

MATCH_FUNC(0x5c2b00)
TrafficLights_194::TrafficLights_194()
{
    field_190_array_used_count = 0;
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_traffic_lights); i++)
    {
        field_0_traffic_lights[i] = 0;
    }
    field_192_phase = 1;
    field_193_timer = traffic_light_phase_timers_626840[1];
}

MATCH_FUNC(0x5c2b30)
TrafficLights_194::~TrafficLights_194()
{
    for (u32 i = 0; i < field_190_array_used_count; i++)
    {
        if (field_0_traffic_lights[i])
        {
            GTA2_DELETE_AND_NULL(field_0_traffic_lights[i]);
        }
    }
}