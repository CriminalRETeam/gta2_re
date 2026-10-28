// Native test of the driver's steering (Source/Bot/DriveSteer.cpp) on a map with no game: a kinematic car follows
// waypoints the way DriveAction does (pure pursuit, the arcs, the speed limit from the wall ahead) and the run is
// judged: did it arrive, and how many frames did it spend scraping a wall.
//
//   g++ -O1 -I../../../Source/Bot -o steer_sim steer_sim.cpp ../../../Source/Bot/DriveSteer.cpp && ./steer_sim
#include "DriveSteer.hpp"
#include "LocalPlan.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

static bool g_overtake = true;
static bool g_local = false; // aim along the detailed A* route, as the game does
static int g_trace_every = 10;
static float kTraffic = 0.15f;
static const float kCarHl = 0.56f; // every car in the sim, ours too: the game's own size, 1.1 tiles long and 0.66 wide
static const float kCarHw = 0.33f;

struct Grid
{
    int x0, y0, w, h;
    std::vector<char> cell;
    bool Solid(float x, float y) const
    {
        const int ix = static_cast<int>(floorf(x)) - x0;
        const int iy = static_cast<int>(floorf(y)) - y0;
        if (ix < 0 || iy < 0 || ix >= w || iy >= h)
        {
            return true;
        }
        const char c = cell[iy * w + ix];
        return c == 'B' || c == ' ';
    }
};

static bool SolidCb(void* user, float x, float y)
{
    return static_cast<const Grid*>(user)->Solid(x, y);
}

// the bot_map.txt dump: "map around ... x 72..120, y 190..238", two ruler lines, then "yyy cells"
static bool LoadGrid(const char* path, Grid* g)
{
    FILE* f = fopen(path, "r");
    if (!f)
    {
        return false;
    }
    char line[512];
    int x1 = 0, y1 = 0;
    if (!fgets(line, sizeof(line), f) || sscanf(strstr(line, "x ") + 2, "%d..%d, y %d..%d", &g->x0, &x1, &g->y0, &y1) != 4)
    {
        return false;
    }
    g->w = x1 - g->x0 + 1;
    g->h = y1 - g->y0 + 1;
    g->cell.assign(g->w * g->h, ' ');
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f))
    {
        const int y = atoi(line);
        if (y < g->y0 || y > y1)
        {
            continue;
        }
        for (int x = 0; x < g->w && line[4 + x] && line[4 + x] != '\n'; x++)
        {
            g->cell[(y - g->y0) * g->w + x] = line[4 + x];
        }
    }
    fclose(f);
    return true;
}

// DriveAction's Drive_CorridorClear: is the strip (half width 0.75) from the car to the aim point free of buildings?
static bool CorridorClear(const Grid& g, float x0, float y0, float x1, float y1)
{
    const float ex = x1 - x0, ey = y1 - y0;
    const float el = sqrtf(ex * ex + ey * ey);
    if (el < 2.0f) return true;
    const float px = -ey / el * 0.75f, py = ex / el * 0.75f;
    for (float d = 1.5f; d <= el; d += 0.5f)
    {
        const float qx = x0 + ex / el * d, qy = y0 + ey / el * d;
        if (g.Solid(qx + px, qy + py) || g.Solid(qx - px, qy - py)) return false;
    }
    return true;
}

struct Pt
{
    float x, y;
};

struct SimCar
{
    float x, y, vx, vy;
};

struct Result
{
    bool arrived;
    int frames;
    int scrape_frames;
    float end_x, end_y;
    float min_car_dist;
    int car_bumps;
    int reversals;      // times the wheel went from one side to the other
    float mean_speed;
};

// hdir -1: a right key press lowers the heading, like the game's cars' calibration says "Right turns -1" for rot
static Result Run(const Grid& g, float sx, float sy, float heading, const std::vector<Pt>& wps, int max_frames, bool trace, std::vector<SimCar> cars = std::vector<SimCar>())
{
    Result r = {false, 0, 0, sx, sy, 1e9f, 0, 0, 0.0f};
    float last_side = 0.0f;
    float prev_s = 0.0f;
    float speed_sum = 0.0f;
    float ot_x = 0, ot_y = 0;
    int ot_until = -1;
    float x = sx, y = sy, h = heading, v = 0.0f;
    const Pt start_pt = {sx, sy};
    const float hdir = 1.0f;
    size_t wi = 0;
    const float cap0 = 0.20f;
    for (int f = 0; f < max_frames; f++)
    {
        const Pt goal = wps.back();
        if ((x - goal.x) * (x - goal.x) + (y - goal.y) * (y - goal.y) < 3.0f * 3.0f)
        {
            r.arrived = true;
            r.frames = f;
            break;
        }
        while (wi + 1 < wps.size() && (wps[wi].x - x) * (wps[wi].x - x) + (wps[wi].y - y) * (wps[wi].y - y) < 3.5f * 3.5f)
        {
            wi++;
        }
        // pure pursuit like DriveAction: the car is projected onto the leg it is on (from the previous waypoint, or the
        // start), and the aim point is a little way on along the route from there
        float look = 4.0f + v * 40.0f > 12.0f ? 12.0f : 4.0f + v * 40.0f;
        Pt aim = wps[wi];
        {
            const Pt from = wi > 0 ? wps[wi - 1] : start_pt;
            float sx2 = wps[wi].x - from.x, sy2 = wps[wi].y - from.y;
            const float sl2 = sx2 * sx2 + sy2 * sy2;
            float t = sl2 > 0.01f ? ((x - from.x) * sx2 + (y - from.y) * sy2) / sl2 : 1.0f;
            t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
            float px = from.x + sx2 * t, py = from.y + sy2 * t;
            const float base_px = px, base_py = py;
            for (int shorten = 0; shorten < 6; shorten++)
            {
                px = base_px; py = base_py;
                float rest = look;
            for (size_t k = wi; k < wps.size(); k++)
            {
                const float dx = wps[k].x - px, dy = wps[k].y - py;
                const float d = sqrtf(dx * dx + dy * dy);
                if (d >= rest && d > 0.001f)
                {
                    aim.x = px + dx / d * rest;
                    aim.y = py + dy / d * rest;
                    rest = 0;
                    break;
                }
                rest -= d;
                px = wps[k].x;
                py = wps[k].y;
                aim = wps[k];
            }
                if (CorridorClear(g, x, y, aim.x, aim.y)) break;
                look *= 0.6f;
            }
        }
        std::vector<SteerCar> sc;
        for (size_t i = 0; i < cars.size(); i++)
        {
            SteerCar s2;
            s2.x = cars[i].x;
            s2.y = cars[i].y;
            s2.vx = cars[i].vx;
            s2.vy = cars[i].vy;
            s2.speed = sqrtf(cars[i].vx * cars[i].vx + cars[i].vy * cars[i].vy);
            s2.police = false;
            const SteerBox cb = {cars[i].x, cars[i].y, 1.0f, 0.0f, kCarHl, kCarHw}; // the cars drive along x
            s2.box = cb;
            sc.push_back(s2);
        }
        SteerWorld w = {SolidCb, const_cast<Grid*>(&g), sc.empty() ? NULL : &sc[0], static_cast<int>(sc.size()), kCarHl + 0.25f, kCarHw + 0.25f};
        if (g_local)
        {
            static LocalPlanOut lp;
            static int next_plan = 0;
            if (f == 0)
            {
                lp.found = false;
                next_plan = 0;
            }
            LocalPlanWorld lw0;
            lw0.solid = SolidCb;
            lw0.step_cost = NULL;
            lw0.user = const_cast<Grid*>(&g);
            lw0.cars = sc.empty() ? NULL : &sc[0];
            lw0.car_count = static_cast<int>(sc.size());
            lw0.own_hl = kCarHl + 0.25f;
            lw0.own_hw = kCarHw + 0.25f;
            lw0.own_speed = 0.12f;
            lw0.route_x = 0;
            lw0.route_y = 0;
            lw0.route_n = 0;
            if (f >= next_plan || !LocalPlan_StillGood(lw0, lp, x, y))
            {
                // 18 tiles along the route from where the car is
                float wxs[64], wys[64];
                int wn = 0;
                wxs[wn] = start_pt.x; wys[wn] = start_pt.y; wn++; // (the game's route starts with the car's own start)
                for (size_t k = 0; k < wps.size() && wn < 64; k++) { wxs[wn] = wps[k].x; wys[wn] = wps[k].y; wn++; }
                static float rxs[kLocalRouteMax], rys[kLocalRouteMax];
                int rn = 0;
                LocalPlan_RouteAhead(wxs, wys, wn, static_cast<int>(wi) + 1, x, y, 24.0f, rxs, rys, &rn);
                const float tx = rxs[rn - 1], ty = rys[rn - 1];
                LocalPlanWorld lw;
                lw.edge_ok = NULL;
                lw.ramp_zone = NULL;
                lw.solid = SolidCb;
                lw.step_cost = NULL;
                lw.user = const_cast<Grid*>(&g);
                lw.cars = sc.empty() ? NULL : &sc[0];
                lw.car_count = static_cast<int>(sc.size());
                lw.own_hl = kCarHl + 0.25f;
                lw.own_hw = kCarHw + 0.25f;
                lw.own_speed = 0.12f;
                lw.route_x = rxs;
                lw.route_y = rys;
                lw.route_n = rn;
                LocalPlan_Find(lw, x, y, h, tx, ty, &lp);
                next_plan = f + 60;
                if (trace)
                {
                    printf("  plan f=%d from (%.1f,%.1f) to (%.1f,%.1f): %d nodes:", f, x, y, tx, ty, lp.count);
                    for (int q = 0; q < lp.count && q < 8; q++) printf(" (%.1f,%.1f)", lp.x[q], lp.y[q]);
                    printf("\n");
                }
            }
            if (lp.found)
            {
                float lk = 4.0f + v * 40.0f;
                lk = lk > 12.0f ? 12.0f : lk;
                LocalPlan_AimPoint(lp, x, y, lk, &aim.x, &aim.y);
            }
        }
        // overtake: go round a slower car; the aim point is kept for a while so that the choice does not flip
        if (f < ot_until)
        {
            aim.x = ot_x;
            aim.y = ot_y;
        }
        else
        {
            float ox, oy;
            if (g_overtake && !g_local && DriveSteer_Overtake(w, x, y, v, aim.x, aim.y, &ox, &oy, NULL))
            {
                ot_x = ox;
                ot_y = oy;
                ot_until = f + 45;
                aim.x = ox;
                aim.y = oy;
            }
        }
        const SteerChoice c = DriveSteer_Choose(w, x, y, h, hdir, v, aim.x, aim.y, prev_s, NULL);
        prev_s = c.s;
        float cap = cap0;
        const float wall_cap = DriveSteer_SpeedFor(c.wall_d);
        cap = wall_cap < cap ? wall_cap : cap;
        // throttle
        float target = v;
        bool back = false;
        const float err = fmodf(atan2f(aim.x - x, -(aim.y - y)) * 57.29578f - h + 540.0f, 360.0f) - 180.0f;
        if (fabsf(err) > 100.0f)
        {
            back = true; // the real driver backs up or turns; the sim just reverses slowly
        }
        if (v < cap)
        {
            target = v + 0.004f;
        }
        else if (v > cap * 1.5f + 0.01f)
        {
            target = v - 0.012f;
        }
        v = back ? -0.05f : (target < 0.0f ? 0.0f : target);
        // steering, as the driver presses the keys: a key is down or up, so a half is every other frame, a quarter every fourth
        float key = c.s;
        if (key != 0.0f && fabsf(key) < 0.9f)
        {
            const bool down = fabsf(key) > 0.4f ? (f % 2 == 0) : (f % 4 == 0);
            key = down ? (key > 0.0f ? 1.0f : -1.0f) : 0.0f;
        }
        if (key != 0.0f)
        {
            const float side = key > 0.0f ? 1.0f : -1.0f;
            if (last_side != 0.0f && side != last_side)
            {
                r.reversals++;
            }
            last_side = side;
        }
        speed_sum += v > 0 ? v : 0;
        h += hdir * key * 1.8f;
        h = fmodf(h + 540.0f, 360.0f) - 180.0f;
        const float rad = h * 3.14159265f / 180.0f;
        const float fx = sinf(rad), fy = -cosf(rad);
        const float nx = x + fx * v, ny = y + fy * v;
        const SteerBox mine_now = {x, y, fx, fy, kCarHl, kCarHw};
        const SteerBox mine_next = {nx, ny, fx, fy, kCarHl, kCarHw};
        float pxs[8], pys[8];
        SteerBox_Points(mine_now, pxs, pys);
        bool was = false;
        for (int i = 0; i < 8; i++) was = was || g.Solid(pxs[i], pys[i]);
        SteerBox_Points(mine_next, pxs, pys);
        bool overlap = false;
        for (int i = 0; i < 8; i++) overlap = overlap || g.Solid(pxs[i], pys[i]);
        const bool hit = overlap && !was; // a car that starts overlapping a wall may move away
        if (hit)
        {
            r.scrape_frames++;
            // the real car slides along a wall and loses most of its speed: try each axis alone
            bool moved = false;
            const float tries[2][2] = {{nx, y}, {x, ny}};
            for (int q = 0; q < 2 && !moved; q++)
            {
                const SteerBox m2 = {tries[q][0], tries[q][1], fx, fy, kCarHl, kCarHw};
                float qxs[8], qys[8];
                SteerBox_Points(m2, qxs, qys);
                bool ov = false;
                for (int i = 0; i < 8; i++) ov = ov || g.Solid(qxs[i], qys[i]);
                if (!ov)
                {
                    x = tries[q][0];
                    y = tries[q][1];
                    moved = true;
                }
            }
            v *= 0.3f;
        }
        else
        {
            x = nx;
            y = ny;
        }
        for (size_t i = 0; i < cars.size(); i++)
        {
            cars[i].x += cars[i].vx;
            cars[i].y += cars[i].vy;
            const float cd = sqrtf((cars[i].x - x) * (cars[i].x - x) + (cars[i].y - y) * (cars[i].y - y));
            if (cd < r.min_car_dist)
            {
                r.min_car_dist = cd;
            }
            const SteerBox mine = {x, y, sinf(h * 3.14159265f / 180.0f), -cosf(h * 3.14159265f / 180.0f), kCarHl, kCarHw};
            const SteerBox theirs = {cars[i].x, cars[i].y, 1.0f, 0.0f, kCarHl, kCarHw};
            if (SteerBox_Overlap(mine, theirs))
            {
                r.car_bumps++;
            }
        }
        if (trace && f % g_trace_every == 0)
        {
            printf("f=%d at (%.1f,%.1f) h %.0f v %.3f wp %zu aim (%.1f,%.1f) key %+.1f wall %.1f cap %.2f%s\n", f, x, y, h, v, wi, aim.x, aim.y, c.s, c.wall_d, cap, hit ? " HIT" : "");
        }
        r.end_x = x;
        r.end_y = y;
        r.frames = f;
    }
    r.mean_speed = r.frames > 0 ? speed_sum / r.frames : 0.0f;
    return r;
}

static int Report(const char* name, const Result& r, int max_scrape)
{
    const bool ok = r.arrived && r.scrape_frames <= max_scrape;
    printf("%-34s %s  frames %4d  scraping %3d frames  bumps %3d  ends (%.1f,%.1f)\n", name, ok ? "PASS" : "FAIL", r.frames, r.scrape_frames, r.car_bumps, r.end_x, r.end_y);
    return ok ? 0 : 1;
}

int main(int argc, char** argv)
{
    const bool trace = argc > 1 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-vv") == 0);
    if (argc > 1 && strcmp(argv[1], "-vv") == 0) g_trace_every = 1;
    for (int i = 1; i < argc; i++) if (strncmp(argv[i], "-traffic=", 9) == 0) kTraffic = static_cast<float>(atof(argv[i] + 9));
    for (int i = 1; i < argc; i++) if (strcmp(argv[i], "-local") == 0) g_local = true;
    g_overtake = !(argc > 1 && strcmp(argv[argc - 1], "-noovertake") == 0); // to prove the tests can fail
    int fails = 0;

    // 1. an open field: straight to the goal
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 60;
        g.cell.assign(g.w * g.h, 'F');
        std::vector<Pt> wps;
        Pt a = {40.5f, 40.5f};
        wps.push_back(a);
        fails += Report("open field", Run(g, 10.5f, 10.5f, 90.0f, wps, 2000, trace), 0);
    }
    // 2. a right-angle corner in a road 4 wide with buildings either side
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 60; g.h = 60;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 10; y < 14; y++) for (int x = 5; x < 40; x++) g.cell[y * g.w + x] = 'P';
        for (int y = 10; y < 50; y++) for (int x = 36; x < 40; x++) g.cell[y * g.w + x] = 'P';
        std::vector<Pt> wps;
        Pt a = {37.5f, 12.0f}, b = {37.5f, 45.0f};
        wps.push_back(a);
        wps.push_back(b);
        fails += Report("road corner, 4 wide", Run(g, 8.5f, 12.0f, 90.0f, wps, 2000, trace), 3);
    }
    // 3. the junction that was a deadlock in the game (real map): starts beside the building at the corner
    {
        Grid g;
        if (LoadGrid("junction_96_214.map", &g))
        {
            std::vector<Pt> wps;
            Pt a = {96.5f, 214.5f}, b = {98.5f, 215.5f}, c = {98.5f, 223.5f};
            wps.push_back(a);
            wps.push_back(b);
            wps.push_back(c);
            fails += Report("real junction (95.6,209.4) south", Run(g, 95.6f, 209.4f, 180.0f, wps, 1500, trace), 3);
            fails += Report("real junction (94.0,212.5) south-west", Run(g, 94.0f, 212.5f, 200.0f, wps, 1500, trace), 200); // pressed against a corner, a 140 degree turn: it slides along the wall for a while (181 with the square route, 102 with the 8 direction one; the K-turn in the game gets a car off a wall sooner)
            fails += Report("real junction (95.3,215.4) beside wall", Run(g, 95.3f, 215.4f, 180.0f, wps, 1500, trace), 3);
        }
        else
        {
            printf("junction map missing\n");
            fails++;
        }
    }
    // 4. a slow car pottering along the lane in front: a road 4 wide with a pavement 3 wide on each side and buildings
    //    beyond. Following it for the whole road is the failure; the car should go round by the pavement
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 120; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 14; y < 26; y++) for (int x = 2; x < 118; x++) g.cell[y * g.w + x] = (y < 17 || y >= 23) ? 'P' : '>';
        std::vector<Pt> wps;
        Pt a = {110.0f, 20.5f};
        wps.push_back(a);
        std::vector<SimCar> cars;
        SimCar c = {22.0f, 20.5f, 0.04f, 0.0f};
        cars.push_back(c);
        const Result r = Run(g, 8.5f, 20.5f, 90.0f, wps, 2500, trace, cars);
        const bool fast = r.frames < 800; // behind it at 0.04 the road takes 2500 frames
        fails += Report("slow car ahead: round it by the pavement", r, 3);
        if (r.car_bumps > 0 || !fast)
        {
            printf("   expected no bump and under 800 frames: bumps %d frames %d\n", r.car_bumps, r.frames);
            fails++;
        }
    }
    // 5. the same with the pavement blocked on one side by a wall: it must use the other
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 120; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 17; y < 26; y++) for (int x = 2; x < 118; x++) g.cell[y * g.w + x] = (y >= 23) ? 'P' : '>';
        std::vector<Pt> wps;
        Pt a = {110.0f, 20.5f};
        wps.push_back(a);
        std::vector<SimCar> cars;
        SimCar c = {22.0f, 19.5f, 0.04f, 0.0f};
        cars.push_back(c);
        const Result r = Run(g, 8.5f, 19.5f, 90.0f, wps, 2500, trace, cars);
        fails += Report("slow car ahead, pavement on one side only", r, 3);
        if (r.car_bumps > 0 || r.frames >= 800)
        {
            printf("   expected no bump and under 800 frames: bumps %d frames %d\n", r.car_bumps, r.frames);
            fails++;
        }
    }
    // 6. no room to pass (a one lane alley): it follows, and does not scrape the walls trying
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 120; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 19; y < 22; y++) for (int x = 2; x < 118; x++) g.cell[y * g.w + x] = '>';
        std::vector<Pt> wps;
        Pt a = {60.0f, 20.5f};
        wps.push_back(a);
        std::vector<SimCar> cars;
        SimCar c = {22.0f, 20.5f, 0.04f, 0.0f};
        cars.push_back(c);
        const Result r = Run(g, 8.5f, 20.5f, 90.0f, wps, 2500, trace, cars);
        fails += Report("alley too narrow to pass: follows", r, 3);
    }
    // 7. traffic at nearly our speed (the real case: NPC cars drive at 0.13 to 0.20 and the driver's cap is 0.20): going
    //    round at once saves the 20 percent we would lose following it, and must not clip it
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 160; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 14; y < 26; y++) for (int x = 2; x < 158; x++) g.cell[y * g.w + x] = (y < 17 || y >= 23) ? 'P' : '>';
        std::vector<Pt> wps;
        Pt a = {150.0f, 20.5f};
        wps.push_back(a);
        std::vector<SimCar> cars;
        SimCar c = {20.0f, 20.5f, kTraffic, 0.0f};
        cars.push_back(c);
        SimCar c2 = {44.0f, 20.5f, kTraffic, 0.0f}; // and one more, further on
        cars.push_back(c2);
        const Result r = Run(g, 8.5f, 20.5f, 90.0f, wps, 3000, trace, cars);
        fails += Report("traffic at 0.15, cap 0.20: overtakes", r, 3);
        // behind a car that does 0.15 the 140 tiles take 930 frames; going round, 700 to 780
        if (r.car_bumps > 0 || r.frames > 850)
        {
            printf("   expected no bump and under 850 frames: bumps %d frames %d\n", r.car_bumps, r.frames);
            fails++;
        }
    }
    // 8. a long straight road with nothing on it: no weaving, and the speed up to the cap
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 200; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 14; y < 26; y++) for (int x = 2; x < 198; x++) g.cell[y * g.w + x] = (y < 17 || y >= 23) ? 'P' : '>';
        std::vector<Pt> wps;
        Pt a = {190.0f, 20.5f};
        wps.push_back(a);
        const Result r = Run(g, 8.5f, 20.5f, 90.0f, wps, 3000, trace);
        fails += Report("empty straight road: no weaving", r, 0);
        printf("   wheel changed sides %d times, mean speed %.3f\n", r.reversals, r.mean_speed);
        if (r.reversals > 25 || r.mean_speed < 0.17f)
        {
            printf("   expected at most 25 changes (the path is re-planned now and then) and a mean speed of 0.17 or more\n");
            fails++;
        }
    }
    // 9. the same road with traffic ahead and a stopped car: a few clean moves, not a weave
    {
        Grid g;
        g.x0 = 0; g.y0 = 0; g.w = 200; g.h = 40;
        g.cell.assign(g.w * g.h, 'B');
        for (int y = 14; y < 26; y++) for (int x = 2; x < 198; x++) g.cell[y * g.w + x] = (y < 17 || y >= 23) ? 'P' : '>';
        std::vector<Pt> wps;
        Pt a = {190.0f, 20.5f};
        wps.push_back(a);
        std::vector<SimCar> cars;
        SimCar c1 = {30.0f, 20.5f, 0.12f, 0.0f};
        SimCar c2 = {70.0f, 19.5f, 0.0f, 0.0f};
        SimCar c3 = {110.0f, 20.5f, 0.10f, 0.0f};
        cars.push_back(c1);
        cars.push_back(c2);
        cars.push_back(c3);
        const Result r = Run(g, 8.5f, 20.5f, 90.0f, wps, 3000, trace, cars);
        fails += Report("road with traffic and a stopped car", r, 3);
        printf("   wheel changed sides %d times, mean speed %.3f, bumps %d\n", r.reversals, r.mean_speed, r.car_bumps);
        if (r.reversals > 45 || r.car_bumps > 0)
        {
            printf("   expected at most 45 changes (going round a stopped car takes a few) and no bump\n");
            fails++;
        }
    }
    // 10. the plaza the van starts in (real map): out by the first waypoints of the real route
    {
        Grid g;
        if (LoadGrid("plaza_192_184.map", &g))
        {
            std::vector<Pt> wps;
            Pt a = {186.5f, 181.5f}, b = {178.5f, 183.5f}, c = {177.5f, 193.5f};
            wps.push_back(a);
            wps.push_back(b);
            wps.push_back(c);
            fails += Report("real plaza: out by the route", Run(g, 198.5f, 188.5f, 0.0f, wps, 1500, trace), 6);
        }
    }
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
