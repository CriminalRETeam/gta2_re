// Unresolved multi-ref stack-slot tie-break probe, EH variant: see slotsim.py's docstring
// (PROBES_UNRESOLVED). Compile with Scripts/x87_sched/sched.sh -l <this file> and read the
// _a$/_b$/_c$ lines in last.asm. All three locals have equal size and equal reference count
// (3 each), so the (size, refs) sort ties completely; only the tie-break decides their
// slot creation order. Observed: b, c, a (not source order a, b, c).
struct Point
{
    int x, y;
    Point() {}
    ~Point() {}
};

extern void ext(Point&);

int test_func()
{
    Point a;
    Point b;
    Point c;
    ext(a);
    ext(b);
    ext(c);
    ext(a);
    ext(b);
    ext(c);
    return a.x + b.x + c.x;
}
