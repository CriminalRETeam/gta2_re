// Same as probe_slot_tiebreak_pod.cpp but the final reads are three separate statements
// instead of one combined `a.x + b.x + c.x` expression. That alone changes the creation
// order to c, a, b (neither source order nor the other probes' b, c, a), so the shape of
// the expression tree that last references a tied-size/tied-refs local also affects the
// tie-break, not just its reference count.
struct Point
{
    int x, y;
};

extern void ext(Point&);
extern int g;

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
    g = a.x;
    g = b.x;
    g = c.x;
    return g;
}
