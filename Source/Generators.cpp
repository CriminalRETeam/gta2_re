#include "Generators.hpp"
#include "Globals.hpp"
#include "rng.hpp"
#include "Object_5C.hpp"

DEFINE_GLOBAL(GeneratorPool_14AC*, gGeneratorPool_14AC_67E5D0, 0x67E5D0);
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

MATCH_FUNC(0x4C1A70)
void Generator_2C::sub_4C1A70()
{
    field_1E_kill_timer = -1;

    if (!bStartNetworkGame_7081F0)
    {
        field_18_cycle = next_cycle_4C1AB0();
        gGeneratorPool_14AC_67E5D0->field_14A4_kill_count++;
    }
    else
    {
        field_18_cycle = rng_dword_67AB34->field_0_rng + 1;
        gGeneratorPool_14AC_67E5D0->field_14A4_kill_count++;
    }
}

WIP_FUNC(0x4C1AB0)
s32 Generator_2C::next_cycle_4C1AB0()
{
    WIP_IMPLEMENTED;

    const s16 min = field_12_min_delay;
    const s16 max = field_14_max_delay;
    if (min == max)
    {
        return min + rng_dword_67AB34->field_0_rng;
    }
    s16 mix_max_delta_m4 = 4 * (max - min);
    s16 rng = stru_6F6784.get_int_4F7AE0(mix_max_delta_m4);
    return rng + 4 * min + rng_dword_67AB34->field_0_rng;
}

WIP_FUNC(0x4C1B10)
EXPORT void Generator_2C::sub_4C1B10()
{
    WIP_IMPLEMENTED;

    if (field_21 != 2)
    {
        Object_2C* pObj = gObject_5C_6F8F84->sub_529BC0(field_0_gen_type, field_4_x, field_8_y, field_C_z, field_10_rot);
        if (pObj)
        {
            if (field_1E_kill_timer != -1)
            {
                field_1E_kill_timer--;
            }

            if (pObj->check_is_shop_421060())
            {
                if (field_1C_ammo)
                {
                    pObj->field_26_varrok_idx = field_1C_ammo;
                }
            }
            field_24_obj = pObj;
            field_28_id = pObj->field_14_id;
        }
        field_18_cycle = next_cycle_4C1AB0();
    }
    else
    {
        if (field_24_obj)
        {
            if (field_24_obj->field_14_id == field_28_id && field_24_obj->field_18_model)
            {
                field_18_cycle += 4;
            }
            else
            {
                field_24_obj = 0;
                field_28_id = 0;
                field_18_cycle = next_cycle_4C1AB0();
            }
        }
        else
        {
            Object_2C* pObj = gObject_5C_6F8F84->sub_529BC0(field_0_gen_type, field_4_x, field_8_y, field_C_z, field_10_rot);
            if (pObj)
            {
                field_24_obj = pObj;
                field_28_id = pObj->field_14_id;
                if (field_1E_kill_timer != -1)
                {
                    field_1E_kill_timer--;
                }

                if (pObj->check_is_shop_421060())
                {
                    if (field_1C_ammo)
                    {
                        pObj->field_26_varrok_idx = field_1C_ammo;
                    }
                }
                field_24_obj = pObj;
                field_28_id = pObj->field_14_id;
            }
            field_18_cycle += 4;
        }
    }

    if (field_1E_kill_timer == 0)
    {
        gGeneratorPool_14AC_67E5D0->field_14A4_kill_count--;
    }
}

MATCH_FUNC(0x4c1c50)
void Generator_2C::Service_4C1C50()
{
    if (rng_dword_67AB34->field_0_rng >= field_18_cycle)
    {
        if (field_1E_kill_timer > 0)
        {
            sub_4C1B10();
        }
    }
}

MATCH_FUNC(0x4c1c70)
void Generator_2C::sub_4C1C70(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 generator_type, s16 min_delay, s16 max_delay)
{
    field_4_x = x.mValue;
    field_8_y = y.mValue;
    field_C_z = z.mValue;
    field_10_rot = rot.rValue;
    field_0_gen_type = generator_type;
    field_12_min_delay = min_delay;
    field_14_max_delay = max_delay;
    field_18_cycle = rng_dword_67AB34->field_0_rng + 1;
    field_20 = 1;
    field_1E_kill_timer = 0;
    field_1C_ammo = 0;
    field_24_obj = NULL;
    field_28_id = 0;
    field_21 = 1;
}

MATCH_FUNC(0x4c1cd0)
void GeneratorPool_14AC::sub_4C1CD0()
{
    s32 count = field_14A0_count;
    field_14A8_count_d3 = count / 3;

    if (!(u8)(count / 3))
    {
        field_14A8_count_d3 = field_14A0_count;
    }

    if (count > 4)
    {
        Generator_2C* pIter = &field_0_pool[0];
        for (u8 v2 = 0; v2 < field_14A0_count; v2++)
        {
            pIter->field_12_min_delay = pIter->field_12_min_delay >> 2;
            pIter->field_14_max_delay = pIter->field_14_max_delay >> 2;
            pIter++;
        }
    }
    if (bStartNetworkGame_7081F0)
    {
        Generator_2C* pIter_2 = &field_0_pool[0];
        for (u8 v4 = 0; v4 < field_14A0_count; v4++)
        {
            pIter_2->field_18_cycle = 1;
            pIter_2++;
        }
    }
}

MATCH_FUNC(0x4c1d70)
void GeneratorPool_14AC::GeneratorsService_4C1D70()
{
    s32 start_idx = field_14A8_count_d3 * (rng_dword_67AB34->field_0_rng & 3);
    s32 end_idx = start_idx + field_14A8_count_d3;
    if (end_idx > field_14A0_count)
    {
        end_idx = field_14A0_count;
    }

    Generator_2C* pIter = &field_0_pool[start_idx];
    for (s32 i = start_idx; i < end_idx; i++)
    {
        pIter->Service_4C1C50();
        pIter++;
    }
}

MATCH_FUNC(0x4c1dc0)
Generator_2C* GeneratorPool_14AC::CreateGenerator_4C1DC0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 type, s16 min_delay, s16 max_delay)
{
    Generator_2C* pMaccies = &field_0_pool[field_14A0_count];
    pMaccies->sub_4C1C70(x, y, z, rot, type, min_delay, max_delay);
    field_14A0_count++;
    return pMaccies;
}

MATCH_FUNC(0x4c1e20)
GeneratorPool_14AC::GeneratorPool_14AC()
{
    field_14A0_count = 0;
    field_14A4_kill_count = 0;
    field_14A8_count_d3 = 0;
}