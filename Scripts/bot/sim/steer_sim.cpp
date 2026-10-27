// Native test of the driver's steering (Source/Bot/DriveSteer.cpp) on a map with no game: a kinematic car follows
// waypoints the way DriveAction does (pure pursuit, the arcs, the speed limit from the wall ahead) and the run is
// judged: did it arrive, and how many frames did it spend scraping a wall.
//
//   g++ -O1 -I../../../Source/Bot -o steer_sim steer_sim.cpp ../../../Source/Bot/DriveSteer.cpp && ./steer_sim
#include "DriveSteer.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

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

struct Pt
{
    float x, y;
};

struct Result
{
    bool arrived;
    int frames;
    int scrape_frames;
    float end_x, end_y;
};

// hdir -1: a right key press lowers the heading, like the game's cars' calibration says "Right turns -1" for rot
static Result Run(const Grid& g, float sx, float sy, float heading, const std::vector<Pt>& wps, int max_frames, bool trace)
{
    Result r = {false, 0, 0, sx, sy};
    float x = sx, y = sy, h = heading, v = 0.0f;
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
        // pure pursuit: a little way on along the route
        const float look = 3.5f + v * 22.0f > 8.0f ? 8.0f : 3.5f + v * 22.0f;
        Pt aim = wps[wi];
        {
            float rest = look;
            float px = x, py = y;
            for (size_t k = wi; k < wps.size(); k++)
            {
                const float dx = wps[k].x - px, dy = wps[k].y - py;
                const float d = sqrtf(dx * dx + dy * dy);
                if (d >= rest)
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
        }
        SteerWorld w = {SolidCb, const_cast<Grid*>(&g), NULL, 0};
        const SteerChoice c = DriveSteer_Choose(w, x, y, h, hdir, v, aim.x, aim.y);
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
        // steering (a half key press is every other frame)
        float key = c.s;
        if (fabsf(key) < 0.9f && key != 0.0f && (f % 2))
        {
            key = 0.0f;
        }
        h += hdir * key * 1.8f;
        h = fmodf(h + 540.0f, 360.0f) - 180.0f;
        const float rad = h * 3.14159265f / 180.0f;
        const float fx = sinf(rad), fy = -cosf(rad);
        const float nx = x + fx * v, ny = y + fy * v;
        const float px = -fy * 0.5f, py = fx * 0.5f;
        const bool was = g.Solid(x + fx * 0.8f + px, y + fy * 0.8f + py) || g.Solid(x + fx * 0.8f - px, y + fy * 0.8f - py) || g.Solid(x + px, y + py) || g.Solid(x - px, y - py);
        const bool overlap = g.Solid(nx + fx * 0.8f + px, ny + fy * 0.8f + py) || g.Solid(nx + fx * 0.8f - px, ny + fy * 0.8f - py) || g.Solid(nx + px, ny + py) || g.Solid(nx - px, ny - py);
        const bool hit = overlap && !was; // a car that starts overlapping a wall may move away
        if (hit)
        {
            r.scrape_frames++;
            v = 0.0f; // the car stops dead against it
        }
        else
        {
            x = nx;
            y = ny;
        }
        if (trace && f % 10 == 0)
        {
            printf("f=%d at (%.1f,%.1f) h %.0f v %.3f wp %zu aim (%.1f,%.1f) key %+.1f wall %.1f cap %.2f%s\n", f, x, y, h, v, wi, aim.x, aim.y, c.s, c.wall_d, cap, hit ? " HIT" : "");
        }
        r.end_x = x;
        r.end_y = y;
        r.frames = f;
    }
    return r;
}

static int Report(const char* name, const Result& r, int max_scrape)
{
    const bool ok = r.arrived && r.scrape_frames <= max_scrape;
    printf("%-34s %s  frames %4d  scraping %3d frames  ends (%.1f,%.1f)\n", name, ok ? "PASS" : "FAIL", r.frames, r.scrape_frames, r.end_x, r.end_y);
    return ok ? 0 : 1;
}

int main(int argc, char** argv)
{
    const bool trace = argc > 1 && strcmp(argv[1], "-v") == 0;
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
            fails += Report("real junction (94.0,212.5) south-west", Run(g, 94.0f, 212.5f, 200.0f, wps, 1500, trace), 3);
            fails += Report("real junction (95.3,215.4) beside wall", Run(g, 95.3f, 215.4f, 180.0f, wps, 1500, trace), 3);
        }
        else
        {
            printf("junction map missing\n");
            fails++;
        }
    }
    printf("%d failed\n", fails);
    return fails ? 1 : 0;
}
