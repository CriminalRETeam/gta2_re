// Same as probe_slot_tiebreak_eh.cpp but with a POD struct (no ctor/dtor, no EH frame at
// all). Gives the same creation order (b, c, a) as the EH version, which rules out EH
// bookkeeping as the cause: the tie-break anomaly is a plain IL/front-end phenomenon.
struct Point
{
    int x, y;
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
