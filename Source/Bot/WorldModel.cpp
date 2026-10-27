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
#include "ScriptManager.hpp"
#include "rng.hpp"
#include "enums.hpp"
#include "Weapon.hpp"

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
    finish_score = 0;
    arrow.ped_id = -1;
    arrow.car_idx = -1;
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
    ped_count = car_count = 0;
    peds_total = cops_total = cars_total = 0;
    brief_key[0] = 0;
    phones_ringing = 0;
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
        player.dead = pPlayer->field_28_wasted_or_busted != 0;
        Weapon* pWeapon = pPlayer->GetCurrPlayerWeapon_5648F0();
        if (pWeapon)
        {
            player.weapon = pWeapon->field_1C_idx;
            player.ammo = pWeapon->field_0_ammo;
        }
    }

    // Script state
    ScriptManager* pScript = gScriptManager_6F8060;
    if (pScript)
    {
        on_mission = pScript->field_344_mission_flag && *pScript->field_344_mission_flag == 1;
        missions_passed = pScript->field_328_passed_flag ? *pScript->field_328_passed_flag : 0;
        finish_score = pScript->field_310_finish_score;
        for (i = 0; i < pScript->field_184_phone_trigger_count && i < 20; i++)
        {
            if (pScript->field_188_phone_triggers[i].field_8_cmd_line != 0)
            {
                phones_ringing++;
            }
        }
    }

    // Brief
    if (gHud_2B00_706620)
    {
        Hud_BriefEntry* pBrief = gHud_2B00_706620->field_DC_brief.field_6F8_curr_briefs;
        if (pBrief)
        {
            strncpy(brief_key, pBrief->field_0_brief_id_str, sizeof(brief_key) - 1);
            brief_key[sizeof(brief_key) - 1] = 0;
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
            gang_count++;
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
        info.cop = p->IsLawEnforcement_45B4E0();
        info.in_car = p->field_16C_car != NULL;
        info.dead = p->field_278_ped_state_1 == 9;
        if (info.cop && !info.dead)
        {
            cops_total++;
        }
        const float dx = info.x - player.x;
        const float dy = info.y - player.y;
        info.dist = sqrtf(dx * dx + dy * dy);
        if (!info.dead)
        {
            InsertPed(this, info);
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
        Append(&buf, "brief \"%s\"\n", brief_key);
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
    Append(&buf, "peds %d (cops %d) cars %d\n", peds_total, cops_total, cars_total);
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
