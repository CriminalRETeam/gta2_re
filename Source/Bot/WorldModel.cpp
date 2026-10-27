#include "WorldModel.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#include "Game.hpp"
#include "Player.hpp"
#include "Ped.hpp"
#include "Car.hpp"
#include "char.hpp"
#include "sprite.hpp"
#include "Char_Pool.hpp"
#include "Hud.hpp"
#include "Gang.hpp"
#include "CarInfoTable.hpp"
#include "map_0x370.hpp"
#include "ScriptManager.hpp"
#include "BotLog.hpp"
#include "ScriptThread.hpp"
#include "rng.hpp"
#include "enums.hpp"
#include "Weapon.hpp"
#include "TextTable.hpp"
#include "BonusTracker.hpp"
#include "PlayerScoreTracker.hpp"
#include "ObjectPool.hpp"
#include "Object_5C.hpp"
#include "ObjectDefinitions.hpp"
#include "BotCompat.hpp"

static float F(const Fix16& v)
{
    return v.mValue / 16384.0f;
}

WorldModel::WorldModel()
{
    memset(&player, 0, sizeof(player));
    memset(&arrow, 0, sizeof(arrow));
    memset(peds, 0, sizeof(peds));
    memset(cars, 0, sizeof(cars));
    memset(respect, 0, sizeof(respect));
    memset(gang_names, 0, sizeof(gang_names));
    frame = 0;
    rng = 0;
    on_mission = false;
    missions_passed = 0;
    phones_ringing = 0;
    brief_key[0] = 0;
    peds_total = cops_total = cars_total = ped_count = car_count = gang_count = 0;
    exit_pending = false;
    exit_type = 0;
    token_count = frenzy_count = phone_count = tokens_listed = pickup_count = 0;
    memset(weapon_ammo, 0, sizeof(weapon_ammo));
    memset(gang_index, 0, sizeof(gang_index));
    memset(kill_reaction, 0, sizeof(kill_reaction));
    memset(kill_respect_change, 0, sizeof(kill_respect_change));
    gang_zone_count = 0;
    gang_zones_read = false;
    kill_frenzy_active = false;
    memset(&bonus, 0, sizeof(bonus));
    track_ped_id = 0;
    tracked_valid = false;
    memset(&tracked, 0, sizeof(tracked));
    secrets_passed = secrets_failed = match_count = 0;
    memset(match_peds, 0, sizeof(match_peds));
    finish_score = 0;
    arrow.ped_id = -1;
    arrow.car_idx = -1;
}

namespace
{
struct ScrName
{
    int id;
    const char* name;
};
const ScrName kScriptNames[] = {
#include "ScriptNames.inc"
    {-1, 0}};
} // namespace

void WorldModel::DumpZones(const char* path)
{
    BotTextFile f;
    if (!gMap_0x370_6F6268 || !gMap_0x370_6F6268->field_32C_pZones || !f.Open(path))
    {
        return;
    }
    const s32 zones = gMap_0x370_6F6268->field_32C_pZones->field_0_num_zones;
    for (s32 i = 0; i < zones; i++)
    {
        gmp_map_zone* z = gMap_0x370_6F6268->GetZone_4DFB30(static_cast<u16>(i));
        char name[40];
        const int len = z->field_5_name_length < 30 ? z->field_5_name_length : 30;
        memcpy(name, z->field_6_name, len);
        name[len] = 0;
        f.Printf("%d type=%d x=%d y=%d w=%d h=%d name=%s\n", i, z->field_0_zone_type, z->field_1_x, z->field_2_y, z->field_3_w, z->field_4_h, name);
    }
    f.Close();
}

void WorldModel::DumpScript(const char* path)
{
    BotTextFile f;
    if (!gScriptManager_6F8060 || !f.Open(path))
    {
        return;
    }
    for (int idx = 0; idx < 6000; idx++)
    {
        SCR_CMD_HEADER* p = gScriptManager_6F8060->GetBasePointer_512770(static_cast<u16>(idx));
        if (!p)
        {
            continue;
        }
        const char* pn = "?";
        for (int i = 0; kScriptNames[i].name; i++)
        {
            if (kScriptNames[i].id == p->field_2_type)
            {
                pn = kScriptNames[i].name;
                break;
            }
        }
        const u16* w = reinterpret_cast<const u16*>(p);
        f.Printf("%d %s(%d) next=%d | %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n", idx, pn, p->field_2_type, static_cast<s16>(p->field_4_cmd_next), w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11]);
    }
    f.Close();
}

u32 WorldModel::ScriptThreads(char* out, u32 out_size)
{
    u32 n = 0;
    out[0] = 0;
    if (!miss2_0x11C_Pool_6F8064 || !gScriptManager_6F8060)
    {
        return 0;
    }
    for (ScriptThread* t = miss2_0x11C_Pool_6F8064->field_0_pool.field_4_pPrev; t && n + 100 < out_size; t = t->mpNext)
    {
        SCR_CMD_HEADER* h = gScriptManager_6F8060->GetBasePointer_512770(t->field_4_next_cmd);
        const char* name = "?";
        for (int i = 0; kScriptNames[i].name; i++)
        {
            if (h && kScriptNames[i].id == h->field_2_type)
            {
                name = kScriptNames[i].name;
                break;
            }
        }
        n += _snprintf(out + n, out_size - n, "thread %d: cmd@%d %s (%d) cond=%d stopped=%d\n", t->field_11A_thread_id, t->field_4_next_cmd, name, h ? h->field_2_type : -1, t->field_8_cond_result, t->field_10_stopped);
        // the next few commands with their raw words: enough to read what a wait is waiting for
        SCR_CMD_HEADER* p = h;
        for (int k = 0; k < 6 && p && n + 200 < out_size; k++)
        {
            const char* pn = "?";
            for (int i = 0; kScriptNames[i].name; i++)
            {
                if (kScriptNames[i].id == p->field_2_type)
                {
                    pn = kScriptNames[i].name;
                    break;
                }
            }
            const u16* w = reinterpret_cast<const u16*>(p);
            n += _snprintf(out + n, out_size - n, "    @%d %s next=%d words: %04x %04x %04x %04x | %04x %04x %04x %04x | %04x %04x %04x %04x\n", p->field_0_cmd_this, pn, p->field_4_cmd_next, w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11]);
            if (static_cast<u16>(p->field_4_cmd_next) == 0xFFFF)
            {
                break;
            }
            p = gScriptManager_6F8060->GetBasePointer_512770(static_cast<u16>(p->field_4_cmd_next));
        }
    }
    return n;
}

u32 WorldModel::PedTable(char* out, u32 out_size, float radius)
{
    u32 n = 0;
    out[0] = 0;
    if (!gPedPool_6787B8 || !gGame_0x40_67E008 || !gGame_0x40_67E008->field_38_local_player || !gGame_0x40_67E008->field_38_local_player->field_2C4_player_ped)
    {
        return 0;
    }
    Ped* me = gGame_0x40_67E008->field_38_local_player->field_2C4_player_ped;
    const float mx = F(me->field_1AC_cam.x);
    const float my = F(me->field_1AC_cam.y);
    for (Ped* p = gPedPool_6787B8->GetFirstPed_435530(); p && n + 160 < out_size; p = p->mpNext)
    {
        const float dx = F(p->field_1AC_cam.x) - mx;
        const float dy = F(p->field_1AC_cam.y) - my;
        if (p == me || dx * dx + dy * dy > radius * radius)
        {
            continue;
        }
        n += _snprintf(out + n, out_size - n, "ped#%d type=%d occ=%d(%s) obj=%d/%d state=%d/%d hp=%d at (%.1f,%.1f) d=%.1f car=%d script=%d\n", p->field_200_id, p->field_238_ped_type, p->field_240_occupation, WorldModel_OccupationName(p->field_240_occupation),
                       p->field_258_objective, p->field_25C_internal_objective, p->field_278_ped_state_1, p->field_27C_ped_state_2, p->field_216_health, F(p->field_1AC_cam.x), F(p->field_1AC_cam.y), sqrtf(dx * dx + dy * dy),
                       p->field_16C_car ? p->field_16C_car->field_6C_car_id : 0, p->field_168_game_object != NULL);
    }
    return n;
}

const char* WorldModel_OccupationName(s32 o)
{
    switch (o)
    {
        case 0: return "player";
        case 1: return "empty";
        case 3: return "dummy";
        case 5: return "driver";
        case 15: return "mugger";
        case 16: return "car_thief";
        case 19: return "gang";
        case 22: return "elvis";
        case 23: return "paramedic";
        case 24: return "cop";
        case 25: return "swat";
        case 26: return "fbi";
        case 27: return "army";
        case 28: return "guard";
        case 29: return "walk_guard";
        case 37: return "roadblock";
        case 38: return "fireman";
        case 42: return "gang_driver";
        default: return "other";
    }
}

static float TopSpeed(s32 model)
{
    if (!gCarInfo_808_678098 || model < 0 || model > 255)
    {
        return 0.0f;
    }
    ModelPhysics* p = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(static_cast<u8>(model));
    return p ? F(p->field_28_max_speed) : 0.0f;
}

static void InsertObj(WorldModel::ObjInfo* arr, s32* count, const WorldModel::ObjInfo& info, float px, float py)
{
    const float dist2 = (info.x - px) * (info.x - px) + (info.y - py) * (info.y - py);
    s32 pos = *count;
    if (pos == WorldModel::kMaxObjs)
    {
        const WorldModel::ObjInfo& last = arr[pos - 1];
        if (dist2 >= (last.x - px) * (last.x - px) + (last.y - py) * (last.y - py))
        {
            return;
        }
        pos--;
    }
    else
    {
        (*count)++;
    }
    while (pos > 0)
    {
        const WorldModel::ObjInfo& prev = arr[pos - 1];
        if ((prev.x - px) * (prev.x - px) + (prev.y - py) * (prev.y - py) <= dist2)
        {
            break;
        }
        arr[pos] = arr[pos - 1];
        pos--;
    }
    arr[pos] = info;
}

static void InsertPed(WorldModel* w, const WorldModel::PedInfo& info)
{
    int pos = w->ped_count;
    if (pos == WorldModel::kMaxPeds)
    {
        if (info.dist >= w->peds[pos - 1].dist)
        {
            return;
        }
        pos--;
    }
    else
    {
        w->ped_count++;
    }
    while (pos > 0 && w->peds[pos - 1].dist > info.dist)
    {
        w->peds[pos] = w->peds[pos - 1];
        pos--;
    }
    w->peds[pos] = info;
}

static void InsertCar(WorldModel* w, const WorldModel::CarInfo& info)
{
    int pos = w->car_count;
    if (pos == WorldModel::kMaxCars)
    {
        if (info.dist >= w->cars[pos - 1].dist)
        {
            return;
        }
        pos--;
    }
    else
    {
        w->car_count++;
    }
    while (pos > 0 && w->cars[pos - 1].dist > info.dist)
    {
        w->cars[pos] = w->cars[pos - 1];
        pos--;
    }
    w->cars[pos] = info;
}

void WorldModel::Observe()
{
    memset(&player, 0, sizeof(player));
    memset(&arrow, 0, sizeof(arrow));
    arrow.ped_id = -1;
    arrow.car_idx = -1;
    ped_count = car_count = match_count = 0;
    tracked_valid = false;
    memset(&bonus, 0, sizeof(bonus));
    peds_total = cops_total = cars_total = 0;
    brief_key[0] = 0;
    brief_text[0] = 0;
    phones_ringing = 0;
    token_count = frenzy_count = phone_count = tokens_listed = pickup_count = 0;
    memset(weapon_ammo, 0, sizeof(weapon_ammo));
    rng = gpRng_67AB34->get_cur_rng_41CFE0();

    int i;
    Game* pGame = gGame_0x40_67E008;
    if (!pGame)
    {
        return;
    }
    exit_pending = pGame->field_28_exit_timer != -1;
    exit_type = pGame->field_2C_game_exit_type;

    Player* pPlayer = pGame->field_38_local_player;
    Ped* me = pPlayer ? pPlayer->field_2C4_player_ped : NULL;
    if (pPlayer && me)
    {
        player.valid = true;
        player.x = F(me->field_1AC_cam.x);
        player.y = F(me->field_1AC_cam.y);
        player.z = F(me->field_1AC_cam.z);
        player.rot_deg = me->GetRotation().rValue * (360.0f / 1440.0f);
        player.hp = me->field_216_health;
        player.lives = pPlayer->field_684_lives.field_0_value;
        player.cash = pPlayer->field_2D4_scores.field_0_money.field_0_value;
        player.wanted_stars = me->get_wanted_star_count_46EF00();
        player.in_car = me->field_16C_car != NULL;
        player.car_id = me->field_16C_car ? me->field_16C_car->field_6C_car_id : 0;
        player.car_damage = me->field_16C_car ? me->field_16C_car->field_74_damage : 0;
        player.dead = pPlayer->field_28_wasted_or_busted != 0;
        player.attack_pressed = pPlayer->field_7C_now_attack_pressed;
        player.ped_attacking = me->field_21C_bf.bAttacking != 0;
        player.ped_state = me->field_278_ped_state_1;
        player.armour = pPlayer->field_6F4_power_up_timers[3];
        for (i = 0; i < 28; i++)
        {
            Weapon* pHeld = pPlayer->field_718_weapons[i];
            weapon_ammo[i] = pHeld ? pHeld->field_0_ammo : 0;
        }
        Weapon* pWeapon = pPlayer->GetCurrPlayerWeapon_5648F0();
        if (pWeapon)
        {
            player.weapon = pWeapon->field_1C_idx;
            player.ammo = pWeapon->field_0_ammo;
            player.reload = pWeapon->field_2_reload_timer;
            player.shot_fired = pWeapon->field_2C_shot_fired;
        }
    }

    // Script state
    ScriptManager* pScript = gScriptManager_6F8060;
    if (pScript)
    {
        on_mission = pScript->field_344_mission_flag && *pScript->field_344_mission_flag == 1;
        missions_passed = pScript->field_328_passed_flag ? *pScript->field_328_passed_flag : 0;
        finish_score = pScript->field_310_finish_score;
        kill_frenzy_active = pScript->field_C1E2D_bKillFrenzyActive != 0;
        secrets_passed = pScript->field_338_secrets_passed ? *pScript->field_338_secrets_passed : 0;
        secrets_failed = pScript->field_33C_secrets_failed ? *pScript->field_33C_secrets_failed : 0;
        for (i = 0; i < pScript->field_184_phone_trigger_count && i < 20; i++)
        {
            if (pScript->field_188_phone_triggers[i].field_8_cmd_line != 0)
            {
                phones_ringing++;
            }
        }
    }

    // Brief
    for (i = 0; i < 4; i++)
    {
        pager_timer[i] = -1;
        pager_value[i] = -1;
    }
    if (gHud_2B00_706620)
    {
        for (i = 0; i < 4; i++)
        {
            Hud_Pager& pg = gHud_2B00_706620->field_620_pagers.field_0_pagers_array[i];
            pager_timer[i] = pg.field_0_timer;
            pager_value[i] = pg.field_4_ptr_counter ? *pg.field_4_ptr_counter : -1;
        }
        Hud_BriefEntry* pBrief = gHud_2B00_706620->field_DC_brief.field_6F8_curr_briefs;
        if (pBrief)
        {
            strncpy(brief_key, pBrief->field_0_brief_id_str, sizeof(brief_key) - 1);
            brief_key[sizeof(brief_key) - 1] = 0;
            const wchar_t* pText = gText_0x14_704DFC->Find_5B5F90(brief_key);
            if (pText)
            {
                int k = 0;
                while (pText[k] && k < static_cast<int>(sizeof(brief_text)) - 1)
                {
                    brief_text[k] = pText[k] < 128 ? static_cast<char>(pText[k]) : '?';
                    k++;
                }
                brief_text[k] = 0;
            }
        }

        // The mission arrow: first arrow in use, not a gang arrow, with a target
        for (i = 0; i < 17; i++)
        {
            Hud_Arrow& a = gHud_2B00_706620->field_1F18_arrows.field_0_array[i];
            Hud_ArrowTargets& t = a.field_18_targets;
            if (!t.field_10_gang_info.field_6_in_use || a.IsType0_4C6F80())
            {
                continue;
            }
            if (t.field_10_gang_info.field_30_gang != NULL)
            {
                continue;
            }
            ArrowTrace& tr = t.field_18_primary_target;
            if (tr.field_10_target_type == 0)
            {
                continue;
            }
            arrow.valid = true;
            arrow.type = tr.field_10_target_type;
            arrow.colour = t.field_28_arrow_colour;
            arrow.x = F(tr.field_14_aim_x);
            arrow.y = F(tr.field_18_aim_y);
            arrow.z = F(tr.field_1C_aim_z);
            if (tr.field_0_ped)
            {
                arrow.ped_id = tr.field_0_ped->field_200_id;
            }
            if (tr.field_4_car)
            {
                arrow.car_idx = tr.field_4_car->field_6C_car_id;
            }
            break;
        }
    }

    // Objects: full scan of the pool, the active list leaves out pickups
    for (i = 0; i < 3825; i++)
    {
        Object* o = &gObject_2C_Pool_6F8F80->field_8_pool[i];
        if (!o->field_4_sprite || !o->field_18_model)
        {
            continue;
        }
        const s32 model = o->field_18_model;
        ObjInfo info;
        info.id = o->field_14_id;
        info.model = model;
        info.x = F(o->field_4_sprite->field_14_xy.x);
        info.y = F(o->field_4_sprite->field_14_xy.y);
        info.z = F(o->field_4_sprite->field_1C_zpos);
        if (model == 266)
        {
            token_count++;
            InsertObj(tokens, &tokens_listed, info, player.x, player.y);
        }
        else if (model == 286)
        {
            InsertObj(frenzies, &frenzy_count, info, player.x, player.y);
        }
        else if (model >= 176 && model <= 181)
        {
            InsertObj(phones, &phone_count, info, player.x, player.y);
        }
        else if ((model >= 200 && model <= 227) || model == 230 || model == 231)
        {
            InsertObj(pickups, &pickup_count, info, player.x, player.y);
        }
    }

    // Gangs: read the array directly, FirstGang/NextGang share a cursor with the game
    gang_count = 0;
    if (gGangPool_CA8_67E274 && pPlayer)
    {
        for (i = 0; i < kMaxGangs; i++)
        {
            Gang* g = &gGangPool_CA8_67E274->field_0_gang_list[i];
            if (!g->field_0_used)
            {
                continue;
            }
            respect[gang_count] = g->field_11C_respect[pPlayer->field_2E_idx];
            memcpy(gang_names[gang_count], g->field_2_name, 4);
            gang_names[gang_count][4] = 0;
            gang_index[gang_count] = g->field_1_gang_idx;
            if (g->field_1_gang_idx < kMaxGangs)
            {
                kill_respect_change[g->field_1_gang_idx] = g->field_139_kill_respect_change;
                for (int k = 0; k < kMaxGangs; k++)
                {
                    kill_reaction[g->field_1_gang_idx][k] = g->field_122_gang_kill_reaction[k];
                }
            }
            gang_count++;
        }
        if (!gang_zones_read && gMap_0x370_6F6268 && gMap_0x370_6F6268->field_32C_pZones)
        {
            gang_zones_read = true;
            const s32 zones = gMap_0x370_6F6268->field_32C_pZones->field_0_num_zones;
            for (i = 0; i < zones && gang_zone_count < kMaxGangZones; i++)
            {
                gmp_map_zone* z = gMap_0x370_6F6268->GetZone_4DFB30(static_cast<u16>(i));
                if (z->field_0_zone_type != 14)
                {
                    continue;
                }
                for (int k = 0; k < kMaxGangs; k++)
                {
                    Gang* g = &gGangPool_CA8_67E274->field_0_gang_list[k];
                    if (g->field_0_used && strncmp(g->field_2_name, z->field_6_name, 4) == 0)
                    {
                        GangZone& gz = gang_zones[gang_zone_count++];
                        gz.gang_idx = g->field_1_gang_idx;
                        gz.x = z->field_1_x;
                        gz.y = z->field_2_y;
                        gz.w = z->field_3_w;
                        gz.h = z->field_4_h;
                        break;
                    }
                }
            }
        }
    }

    // The active bonus rule
    BonusTracker* pTracker = pPlayer ? &pPlayer->field_2D4_scores.field_1A8_bonuses : NULL;
    if (pTracker)
    {
        for (i = 0; i < BonusTracker::k_max_rules; i++)
        {
            BonusRule& r = pTracker->field_0_bonuses[i];
            if (r.field_2A_bUsed && r.field_2B_bActive)
            {
                bonus.active = true;
                bonus.event_type = r.field_4_event_type;
                bonus.occupation = r.field_C_occupation;
                bonus.gang_idx = r.field_10_gang_idx;
                bonus.remap = r.field_12_remap;
                bonus.death_cause = r.field_14_death_cause;
                bonus.car_model = r.field_8_car_model;
                bonus.zone_limited = r.field_0_pZone != NULL;
                bonus.target = r.field_25_target_count;
                bonus.progress = r.field_26_progress_count;
                bonus.cancel_on_mismatch = r.field_24_check_mode == 1;
                break;
            }
        }
    }

    // Peds on the active list
    for (Ped* p = gPedPool_6787B8->GetFirstPed_435530(); p; p = p->mpNext)
    {
        if (p == me)
        {
            continue;
        }
        peds_total++;
        PedInfo info;
        memset(&info, 0, sizeof(info));
        info.id = p->field_200_id;
        info.occupation = p->field_240_occupation;
        info.objective = p->field_258_objective;
        info.hp = p->field_216_health;
        info.x = F(p->field_1AC_cam.x);
        info.y = F(p->field_1AC_cam.y);
        info.z = F(p->field_1AC_cam.z);
        info.cop = p->IsLawEnforcement_45B4E0();
        info.in_car = p->field_16C_car != NULL;
        info.dead = p->field_278_ped_state_1 == 9;
        info.remap = p->field_244_remap;
        info.state1 = p->field_278_ped_state_1;
        info.state2 = p->field_27C_ped_state_2;
        info.gang_idx = p->field_17C_pGang ? p->field_17C_pGang->field_1_gang_idx : (p->field_19C_dummy_gang ? p->field_19C_dummy_gang->field_1_gang_idx : -1);
        info.matches_bonus = false;
        if (bonus.active && bonus.event_type == 0 && pTracker)
        {
            const bool occ_ok = bonus.occupation == info.occupation || bonus.occupation == 51 || pTracker->IsOccupationInGroup_432240(info.occupation, bonus.occupation);
            const bool gang_ok = bonus.gang_idx == -1 || bonus.gang_idx == info.gang_idx;
            const bool remap_ok = bonus.remap == -2 || bonus.remap == info.remap;
            info.matches_bonus = occ_ok && gang_ok && remap_ok;
        }
        if (info.cop && !info.dead)
        {
            cops_total++;
        }
        const float dx = info.x - player.x;
        const float dy = info.y - player.y;
        info.dist = sqrtf(dx * dx + dy * dy);
        if (track_ped_id != 0 && info.id == track_ped_id)
        {
            tracked = info;
            tracked_valid = true;
        }
        if (!info.dead)
        {
            InsertPed(this, info);
            if (info.matches_bonus)
            {
                int pos = match_count;
                if (pos == kMaxPeds)
                {
                    pos = info.dist < match_peds[pos - 1].dist ? pos - 1 : -1;
                }
                else
                {
                    match_count++;
                }
                if (pos >= 0)
                {
                    while (pos > 0 && match_peds[pos - 1].dist > info.dist)
                    {
                        match_peds[pos] = match_peds[pos - 1];
                        pos--;
                    }
                    match_peds[pos] = info;
                }
            }
        }
    }

    // Cars: full scan of the pool array, deactivated cars are off the active list
    for (i = 0; i < 306; i++)
    {
        Car* c = &gCar_BC_Pool_67792C->field_0_pool.field_8_pool[i];
        if (!c->field_50_car_sprite || !c->field_6C_car_id)
        {
            continue;
        }
        cars_total++;
        CarInfo info;
        memset(&info, 0, sizeof(info));
        info.exists = true;
        info.idx = c->field_6C_car_id;
        info.model = c->field_84_car_info_idx;
        info.damage = c->field_74_damage;
        info.x = F(c->field_50_car_sprite->field_14_xy.x);
        info.y = F(c->field_50_car_sprite->field_14_xy.y);
        info.has_driver = c->field_54_driver != NULL;
        info.locked = c->field_98_door_lock == 1 || c->field_98_door_lock == 4;
        info.mission = c->IsMissionRelated_4421B0();
        info.police = c->field_A0_car_kind == 6;
        info.speed = c->field_58_physics ? F(c->GetVelocity_43A4C0()) : 0.0f;
        info.top_speed = TopSpeed(c->field_84_car_info_idx);
        const float dx = info.x - player.x;
        const float dy = info.y - player.y;
        info.dist = sqrtf(dx * dx + dy * dy);
        InsertCar(this, info);
    }
}

struct TextBuf
{
    char* p;
    char* end;
};

static void Append(TextBuf* b, const char* fmt, ...)
{
    if (b->p >= b->end)
    {
        return;
    }
    va_list args;
    va_start(args, fmt);
    int n = _vsnprintf(b->p, b->end - b->p, fmt, args);
    va_end(args);
    if (n < 0 || n > b->end - b->p)
    {
        n = b->end - b->p;
    }
    b->p += n;
}

void WorldModel::DumpMap(const char* path, float cx, float cy, int radius) const
{
    FILE* f = fopen(path, "w");
    if (!f)
    {
        return;
    }
    const int x0 = static_cast<int>(cx) - radius;
    const int y0 = static_cast<int>(cy) - radius;
    fprintf(f, "map around (%.1f,%.1f) radius %d; x %d..%d, y %d..%d\n", cx, cy, radius, x0, x0 + 2 * radius, y0, y0 + 2 * radius);
    fprintf(f, "    ");
    int x;
    for (x = x0; x <= x0 + 2 * radius; x++)
    {
        fprintf(f, "%d", (x / 10) % 10);
    }
    fprintf(f, "\n    ");
    for (x = x0; x <= x0 + 2 * radius; x++)
    {
        fprintf(f, "%d", x % 10);
    }
    fprintf(f, "\n");
    int y;
    for (y = y0; y <= y0 + 2 * radius; y++)
    {
        fprintf(f, "%3d ", y);
        for (x = x0; x <= x0 + 2 * radius; x++)
        {
            char c = ' ';
            s32 z = 0;
            gmp_block_info* b = (x > 0 && x < 255 && y > 0 && y < 255) ? gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x, y, &z) : NULL;
            if (b)
            {
                const int type = b->field_B_slope_type & 3;
                if (z >= 3)
                {
                    c = 'B';
                }
                else if (type == 1)
                {
                    c = 'R';
                }
                else if (type == 2)
                {
                    c = 'P';
                }
                else if (type == 3)
                {
                    c = 'F';
                }
                else
                {
                    c = '.';
                }
                // green arrows: bit0 left, bit1 right, bit2 up, bit3 down (gmp_arrow_direction)
                if (b->field_A_arrows & 0x4)
                {
                    c = '^';
                }
                else if (b->field_A_arrows & 0x8)
                {
                    c = 'v';
                }
                else if (b->field_A_arrows & 0x1)
                {
                    c = '<';
                }
                else if (b->field_A_arrows & 0x2)
                {
                    c = '>';
                }
            }
            int i;
            for (i = 0; i < 306; i++)
            {
                Car* pc = &gCar_BC_Pool_67792C->field_0_pool.field_8_pool[i];
                if (pc->field_50_car_sprite && pc->field_6C_car_id && static_cast<int>(F(pc->field_50_car_sprite->field_14_xy.x)) == x &&
                    static_cast<int>(F(pc->field_50_car_sprite->field_14_xy.y)) == y)
                {
                    c = 'c';
                }
            }
            if (static_cast<int>(player.x) == x && static_cast<int>(player.y) == y)
            {
                c = '@';
            }
            fputc(c, f);
        }
        fputc('\n', f);
    }
    fclose(f);
}

void WorldModel::DumpObjects(const char* path) const
{
    FILE* f = fopen(path, "w");
    if (!f)
    {
        return;
    }
    fprintf(f, "# id model x y z pool_state behaviour\n");
    for (int i = 0; i < 3825; i++)
    {
        Object* o = &gObject_2C_Pool_6F8F80->field_8_pool[i];
        if (!o->field_4_sprite || !o->field_18_model)
        {
            continue;
        }
        fprintf(f, "%d %d %.1f %.1f %.1f %d %d\n", o->field_14_id, o->field_18_model, F(o->field_4_sprite->field_14_xy.x), F(o->field_4_sprite->field_14_xy.y),
                F(o->field_4_sprite->field_1C_zpos), o->field_20_pool_list_state, o->field_8_definition ? o->field_8_definition->field_34_behavior_type : -1);
    }
    fclose(f);
}

int WorldModel::TileType(int x, int y)
{
    if (x < 1 || x > 254 || y < 1 || y > 254)
    {
        return 0;
    }
    s32 z = 0;
    gmp_block_info* b = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x, y, &z);
    if (!b)
    {
        return 0;
    }
    return z >= 3 ? 4 : (b->field_B_slope_type & 3);
}

bool WorldModel::BuildingAt(float x, float y)
{
    const int ix = static_cast<int>(x);
    const int iy = static_cast<int>(y);
    if (ix < 1 || ix > 254 || iy < 1 || iy > 254)
    {
        return true;
    }
    s32 z = 0;
    gmp_block_info* b = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(ix, iy, &z);
    return b != NULL && z >= 3;
}

bool WorldModel::FindObject(s32 object_id, ObjInfo* out) const
{
    for (int i = 0; i < 3825; i++)
    {
        Object* o = &gObject_2C_Pool_6F8F80->field_8_pool[i];
        if (o->field_4_sprite && o->field_18_model && o->field_14_id == object_id)
        {
            out->id = object_id;
            out->model = o->field_18_model;
            out->x = F(o->field_4_sprite->field_14_xy.x);
            out->y = F(o->field_4_sprite->field_14_xy.y);
            out->z = F(o->field_4_sprite->field_1C_zpos);
            return true;
        }
    }
    return false;
}

bool WorldModel::FindCar(s32 car_id, CarInfo* out) const
{
    for (int i = 0; i < 306; i++)
    {
        Car* c = &gCar_BC_Pool_67792C->field_0_pool.field_8_pool[i];
        if (!c->field_50_car_sprite || c->field_6C_car_id != car_id)
        {
            continue;
        }
        memset(out, 0, sizeof(*out));
        out->exists = true;
        out->idx = c->field_6C_car_id;
        out->model = c->field_84_car_info_idx;
        out->damage = c->field_74_damage;
        out->x = F(c->field_50_car_sprite->field_14_xy.x);
        out->y = F(c->field_50_car_sprite->field_14_xy.y);
        out->has_driver = c->field_54_driver != NULL;
        out->locked = c->field_98_door_lock == 1 || c->field_98_door_lock == 4;
        out->mission = c->IsMissionRelated_4421B0();
        out->police = c->field_A0_car_kind == 6;
        out->speed = c->field_58_physics ? F(c->GetVelocity_43A4C0()) : 0.0f;
        out->top_speed = TopSpeed(c->field_84_car_info_idx);
        const float dx = out->x - player.x;
        const float dy = out->y - player.y;
        out->dist = sqrtf(dx * dx + dy * dy);
        return true;
    }
    return false;
}

u32 WorldModel::Text(char* out, u32 out_size, const char* header) const
{
    TextBuf buf;
    buf.p = out;
    buf.end = out + out_size - 1;
    int i;
Append(&buf, "bot f=%d rng=%d %s\n", frame, rng, header ? header : "");
    if (brief_key[0])
    {
        Append(&buf, "brief \"%s\": %s\n", brief_key, brief_text);
    }
    Append(&buf, "player x=%.1f y=%.1f z=%.0f rot=%.0f hp=%d lives=%d cash=%d wanted=%d incar=%d dead=%d weapon=%d/%d\n",
         player.x,
         player.y,
         player.z,
         player.rot_deg,
         player.hp,
         player.lives,
         player.cash,
         player.wanted_stars,
         player.in_car,
         player.dead,
         player.weapon,
         player.ammo);
    Append(&buf, "script on_mission=%d passed=%d phones_ringing=%d finish_score=%d exit=%d/%d\n",
         on_mission,
         missions_passed,
         phones_ringing,
         finish_score,
         exit_pending,
         exit_type);
    if (gang_count)
    {
        Append(&buf, "respect");
        for (i = 0; i < gang_count; i++)
        {
            Append(&buf, " %s=%d", gang_names[i], respect[i]);
        }
        Append(&buf, "\n");
    }
    if (arrow.valid)
    {
        const float dx = arrow.x - player.x;
        const float dy = arrow.y - player.y;
        Append(&buf, "arrow type=%d colour=%d x=%.1f y=%.1f (dist %.0f)\n", arrow.type, arrow.colour, arrow.x, arrow.y, sqrtf(dx * dx + dy * dy));
    }
    else
    {
        Append(&buf, "arrow none\n");
    }
    if (bonus.active)
    {
        Append(&buf, "bonus rule: event %d occupation %d gang %d remap %d death_cause %d car %d zone %d progress %d/%d cancel_on_mismatch %d; matching peds %d; frenzies passed %d failed %d\n", bonus.event_type, bonus.occupation, bonus.gang_idx, bonus.remap, bonus.death_cause, bonus.car_model, bonus.zone_limited, bonus.progress, bonus.target, bonus.cancel_on_mismatch, match_count, secrets_passed, secrets_failed);
    }
    Append(&buf, "pagers t=%d,%d,%d,%d v=%d,%d,%d,%d\n", pager_timer[0], pager_timer[1], pager_timer[2], pager_timer[3], pager_value[0], pager_value[1], pager_value[2], pager_value[3]);
    Append(&buf, "peds %d (cops %d) cars %d; tokens left %d, frenzy icons %d, phones %d, frenzy active %d\n", peds_total, cops_total, cars_total, token_count, frenzy_count, phone_count, kill_frenzy_active);
    for (i = 0; i < ped_count && i < 5; i++)
    {
        const PedInfo& d = peds[i];
        Append(&buf, "ped#%d %s x=%.0f y=%.0f hp=%d obj=%d d=%.0f%s\n", d.id, WorldModel_OccupationName(d.occupation), d.x, d.y, d.hp, d.objective, d.dist, d.in_car ? " incar" : "");
    }
    for (i = 0; i < car_count && i < 4; i++)
    {
        const CarInfo& d = cars[i];
        Append(&buf, "car#%d model=%d x=%.0f y=%.0f dmg=%d driver=%d locked=%d mission=%d d=%.0f\n", d.idx, d.model, d.x, d.y, d.damage, d.has_driver, d.locked, d.mission, d.dist);
    }
    *buf.p = 0;
    return buf.p - out;
}
