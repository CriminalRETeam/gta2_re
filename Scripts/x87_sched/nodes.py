"""nodes.py LOG [function substring...]

Per scheduled function of a sched.sh log (`#ordinal name`): every scheduling window with its node count,
how many of those are no-op nodes (op 354 = 0x162: a parenthesised float subexpression, an (f32) cast
of a float expression, a store to an f32 local) and where they are. `FULL` marks a window that was cut
by the size limit (81 nodes at the stock limit 80), so its last node decides where the next window
starts. Windows of fewer than 3 nodes are folded into a count.
"""
import sys

NOP = 354


def parse(log):
    F = []
    for l in open(log, errors='replace'):
        if l.startswith('@F '):
            parts = l[3:].strip().split(' ', 1)
            F.append({'ord': int(parts[0].lstrip('#')), 'name': parts[1] if len(parts) > 1 else '', 'win': []})
        elif l.startswith('@L') and F:
            F[-1]['win'].append([int(x.split('/')[0]) for x in l.split()[1:]])
    return F


def main():
    log, filt = sys.argv[1], [a for a in sys.argv[2:] if not a.startswith('-')]
    for f in parse(log):
        if filt and not any(s in f['name'] for s in filt):
            continue
        print('#%d %s' % (f['ord'], f['name'][:110]))
        small = 0
        for i, w in enumerate(f['win']):
            if len(w) < 3:
                small += 1
                continue
            nops = [k for k, op in enumerate(w) if op == NOP]
            print('   window %-3d n=%-3d nop=%-2d %s%s' % (i + 1, len(w), len(nops), 'FULL ' if len(w) >= 81 else '',
                                                        'at %s' % nops if nops else ''))
        if small:
            print('   (%d windows of 1-2 nodes not shown)' % small)


if __name__ == '__main__':
    main()
