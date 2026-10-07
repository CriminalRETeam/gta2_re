"""
Compares the branch targets (offsets from the function start) of one function in an .obj with the original's
(Scripts/bin_comp/target_asm.json). The verifier's normalisation renames forward `jmp` targets, so a build
that jumps into the wrong copy of identical code can still score 0; this catches that.

    venv/bin/python3 Scripts/flowopt/jumps.py <file.obj> <addr> <symbol substring>
"""
import json, os, re, sys

BIN = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'bin_comp')

def obj_jumps(obj, needle):
    sys.path.insert(0, BIN)
    import permuter_score as ps
    coff = ps.Coff(open(obj, 'rb').read())
    asm = ps.function_asm(coff, ps.find_function(coff, needle))
    return [(m.group(1), int(m.group(2), 16)) for l in asm.split('\n')
            for m in [re.match(r'(j(?!mpl)\w+) +0x([0-9A-Fa-f]+)', l.strip())] if m]

def main():
    obj, addr, needle = sys.argv[1], int(sys.argv[2], 16), sys.argv[3]
    raw = json.load(open(os.path.join(BIN, 'target_asm.json')))[hex(addr)]['asm'].split('\n')
    tj = [(m.group(1), int(m.group(2), 16)) for l in raw for m in [re.match(r'(j(?!mpl)\w+) +0x([0-9A-Fa-f]+)', l)] if m]
    oj = obj_jumps(obj, needle)
    bad = 0
    for i in range(max(len(tj), len(oj))):
        a = oj[i] if i < len(oj) else None
        b = tj[i] if i < len(tj) else None
        if a is None or b is None or a[1] != b[1]:
            bad += 1
            print(f'#{i}: ours {a and (a[0], hex(a[1]))}  original {b and (b[0], hex(b[1]))}')
    print('jump targets: %s (%d jumps)' % ('same' if not bad else f'{bad} differ', len(tj)))

if __name__ == '__main__':
    main()
