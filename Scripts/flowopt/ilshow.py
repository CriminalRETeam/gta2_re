"""
ilshow.py <last.log> <first line> <last line> [function substring]

Prints, from an ildump log, the instruction list of a function after each pass that changed it: only the
instructions whose source line (relative to the function's first line) is in [first, last], plus jumps
(op/17) and labels (430/26). Plain instructions print as their op name, others as op/kind.
"""
import os, sys

def main():
    ops = {}
    for l in open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'x87_sched', 'optab.txt')):
        a = l.split()
        ops[int(a[0])] = a[2] if len(a) > 2 else a[0]
    log, lo, hi = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
    fn = sys.argv[4] if len(sys.argv) > 4 else None
    prev = None
    for l in open(log, errors='replace'):
        if not l.startswith('@S'):
            continue
        hd, rest = l.split(': ', 1)
        if fn and fn not in hd:
            continue
        items = []
        for t in rest.split():
            k, o, ln = (int(v) for v in t.split('.'))
            ln &= 0xffff
            if lo <= ln <= hi or k in (17, 26):
                items.append(f"{ops.get(o, o)}{'' if k == 12 else '/' + str(k)}@{ln}")
        s = ' '.join(items)
        if s != prev:
            print(hd.split()[1], s)
        prev = s

if __name__ == '__main__':
    main()
