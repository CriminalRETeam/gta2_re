int g1, g2, g3;
void f(int* p, int n)
{
    int a = 0, b = 0, c = 0;
    for (int i = 0; i < n; i++) { a += p[i]; b ^= p[i] * 3; c |= p[i]; }
    g1 = a; g2 = b; g3 = c;
}
