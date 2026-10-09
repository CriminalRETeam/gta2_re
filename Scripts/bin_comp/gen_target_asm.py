"""Write target_asm.json for every WIP/STUB function straight from 10.5.exe.

The "Dump target asm" workflow publishes this file on the claude/target-asm branch. When that
branch isn't reachable, this builds the same thing locally: the original asm of each marked
function, disassembled the way compare_function.py does it, so compare_target_asm.py,
regsearch.py and compare_callees.py all work offline.

    python3 Scripts/bin_comp/gen_target_asm.py [-o target_asm.json]
"""
import argparse
import csv
import json
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
sys.path.insert(0, HERE)
import compare_function  # noqa: E402


def marked():
    """{address: status} for every WIP_FUNC (0x2) and STUB_FUNC (0x0) in Source/."""
    out = subprocess.run(['rg', '-o', r'(?:WIP|STUB)_FUNC\(0x[0-9a-fA-F]+\)', os.path.join(ROOT, 'Source')],
                         capture_output=True, text=True).stdout
    return {int(a, 16): ('0x2' if k == 'WIP' else '0x0')
            for k, a in re.findall(r'(WIP|STUB)_FUNC\(0x([0-9a-fA-F]+)\)', out)}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('-o', '--out', default=os.path.join(HERE, 'target_asm.json'))
    ap.add_argument('--exe', default=os.path.join(HERE, '10.5.exe'))
    a = ap.parse_args()
    want = marked()
    og = open(a.exe, 'rb').read()
    res = {}
    with open(os.path.join(HERE, 'og_function_data_v105.csv')) as f:
        for name, addr, off, size in csv.reader(f):
            va = int(addr, 16)
            if va not in want:
                continue
            off, size = int(off, 16), int(size, 16)
            res[hex(va)] = {'name': name, 'status': want[va],
                            'asm': compare_function.dism_func(og[off:off + size])}
    json.dump(res, open(a.out, 'w'))
    print('%d functions -> %s' % (len(res), a.out))


if __name__ == '__main__':
    main()
