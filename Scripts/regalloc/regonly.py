"""
Lists WIP/STUB functions whose post processed asm matches the original once registers are
renamed, i.e. functions that differ only in register assignment. Run from Scripts/bin_comp after
a build + msvc_dump_new_data.py (same inputs as compare_target_asm.py):

    python3 ../regalloc/regonly.py          # perm: consistent register permutation, shape: same
                                            # instructions with any registers
"""
import difflib, re, sys, os
sys.path.insert(0, os.getcwd())
import compare_target_asm as cta, post_process_asm

REG = re.compile(r'%(e?[abcd]x|[abcd][lh]|e?si|e?di|e?bp|e?sp)\b')
FAM = {}
for f, names in {'a': 'eax ax al ah', 'b': 'ebx bx bl bh', 'c': 'ecx cx cl ch', 'd': 'edx dx dl dh',
                 'si': 'esi si', 'di': 'edi di', 'bp': 'ebp bp', 'sp': 'esp sp'}.items():
    for n in names.split():
        FAM[n] = f

def canon(asm):
    m = {}
    def sub(mo):
        r = mo.group(1); f = FAM[r]
        if f == 'sp':
            return '%' + r
        m.setdefault(f, 'R%d' % len(m))
        return '%' + m[f] + ':' + r.replace('e', '', 1) if r[0] == 'e' and len(r) == 3 else '%' + m[f] + ':' + r
    return REG.sub(sub, asm)

def shape(asm):
    return REG.sub(lambda mo: '%sp' if FAM[mo.group(1)] == 'sp' else '%R', asm)

def main():
    targets = cta.load_json('target_asm.json')
    nd = cta.load_json('new_data.json')
    for rec in nd['functions']:
        if not (rec['og_addr'] or '').startswith('0x') or rec['func_status'] == '0x1':
            continue
        t = targets.get(hex(int(rec['og_addr'], 16)))
        if not t or t['pp'] is None:
            continue
        mine = post_process_asm.post_process_asm(cta.build_asm(rec, t['size']))
        if mine == t['pp']:
            continue
        kind = 'perm' if canon(mine) == canon(t['pp']) else 'shape' if shape(mine) == shape(t['pp']) else None
        if kind:
            n = sum(1 for a, b in zip(mine.split('\n'), t['pp'].split('\n')) if a != b)
            print(f"{kind:5} {rec['og_addr']} {t['name']} lines={n}")

if __name__ == '__main__':
    main()
