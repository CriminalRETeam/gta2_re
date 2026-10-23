"""
For every MATCH function, checks what the verifier's normalisation can't see: that the build and 10.5.exe
reference the same globals at the same instructions, call the same functions, and route the same switch
values to the same blocks (jump tables and the byte index tables of merged cases).

compare_all_functions.py renames absolute addresses to stable_name_N in order of first use and leaves
out jump tables, so a wrong `case` label, two swapped constants or a call to the wrong function still
"match". Found this way: Ped::Deallocate_45EB60 (case 28 for 29), Char_B4::sub_54C090 (two constants
swapped between cases), Car_BC::IsAreaDamaged_43D1C0, the script compares SCRCMD_I_OP_S/S_OP_S,
Network_20324::SetSetting_51B9C0 and sound_obj::HandleTruckCorneringAudio_417FD0 (case labels).

Switch blocks laid out in another order pass when each value's block does the same (calls and globals
mapped to 10.5.exe's). Differences reviewed as equivalent are in REVIEWED and left out; check a new one
against 10.5.exe's code before adding it there.

Build addresses are mapped to original ones through build_vc6/output.map and new_data.json (globals by
their DEFINE_GLOBAL address or _ADDRESS name suffix, functions by their marker); an address the build has
no original for (strings, float constants, vtables, ...) is not checked.

Usage (from Scripts/bin_comp, after build.py): python3 check_match_refs.py [address ...]
Exit code 1 when something differs.
"""
import bisect, json, re, struct, sys
import compare_function, post_process_asm
from iced_x86 import Decoder, FlowControl, Mnemonic, OpKind

ROOT = '../..'
# Differences that do the same as 10.5.exe (reviewed by reading both)
REVIEWED = {
    0x4e0130: 'Map_0x370::CanMoveOntoSlopeTile_4E0130: 10.5.exe\'s block is a copy of the first instructions of ours',
    0x546360: 'Char_B4::UpdateAnimState_546360: 10.5.exe skips reloading al, which holds the same value',
    0x453590: 'Fix16_Point_POD::GetLength_453590: another Fix16(0) constant',
    0x55dc00: 'CarPhysics_B0::UpdateWheelSkidEffects_55DC00: another Fix16(1) constant',
    0x4785d0: 'SpriteGrid_400::CheckRowForRectCollisions_4785D0: a copy of the same 0xCCC constant',
    0x45bd20: 'Ped::CloseDoorIfCarApproaching_45BD20: two addends in the other order',
    0x454680: 'ModelPhysics_48::ConvertMass_454680: two addends in the other order',
    0x53a280: 'Particle_4C::UpdateSkidOrScrapeSpark_state_40_41_53A280: two addends in the other order',
    0x542060: 'Explosion_30::EmitExplosion_19_32_542060: two addends in the other order',
    0x520de0: 'NetPlay::ClearPlayersAndSession_520DE0: free, where 10.5.exe calls delete_5ED46D (which frees)',
    0x4bae30: 'Game_0x40::dtor_4BAE30: dtor_484FD0 is a jmp to dtor_484DB0',
    0x4dad50: 'Net_Set_Local_Player_Inputs_4DAD50: network only, not reviewed further',
}
NEW = ROOT + '/build_vc6/decomp_main.exe'
OG = '10.5.exe'


class Pe:
    def __init__(self, path):
        self.data = open(path, 'rb').read()
        pe = struct.unpack_from('<I', self.data, 0x3C)[0]
        nsec = struct.unpack_from('<H', self.data, pe + 6)[0]
        optsize = struct.unpack_from('<H', self.data, pe + 20)[0]
        self.base = struct.unpack_from('<I', self.data, pe + 24 + 28)[0]
        self.secs = []
        for i in range(nsec):
            off = pe + 24 + optsize + i * 40
            vsize, va, rawsize, raw = struct.unpack_from('<IIII', self.data, off + 8)
            self.secs.append((self.base + va, max(vsize, rawsize), raw, rawsize))

    def read(self, va, n):
        for start, size, raw, rawsize in self.secs:
            if start <= va < start + size:
                o = va - start
                return self.data[raw + o:raw + min(o + n, rawsize)].ljust(n, b'\0') if o < rawsize else b'\0' * n
        return None

    def inside(self, va):
        return any(s <= va < s + n for s, n, _, _ in self.secs)


def load():
    nd = json.load(open('new_data.json'))
    sym = {}  # mangled name -> build address
    for line in open(ROOT + '/build_vc6/output.map', errors='replace'):
        p = line.split()
        if len(p) >= 3 and re.match(r'^[0-9a-f]{4}:[0-9a-f]{8}$', p[0]) and re.match(r'^[0-9a-f]{8}$', p[2]):
            sym[p[1]] = int(p[2], 16)
    # build address -> original address, for everything with a known original
    known = {}
    for v in nd['variables']:
        if v['mangled_name'] in sym:
            known[sym[v['mangled_name']]] = int(v['og_address'], 16)
    for name, addr in sym.items():
        m = re.search(r'_([0-9A-Fa-f]{6})@', name)
        if m and addr not in known and 0x400000 <= int(m.group(1), 16) < 0x800000:
            known[addr] = int(m.group(1), 16)
    for f in nd['functions']:
        if f.get('og_addr', '?').startswith('0x'):
            known[int(f['func_va'], 16)] = int(f['og_addr'], 16)
    return nd, known


def decode(pe, va, size):
    return list(Decoder(32, pe.read(va, size), ip=va))


def table_len(insns, k):
    """Entries of the table used at insns[k]: from the closest bounds check before it (cmp reg, N; ja)."""
    for j in range(k - 1, max(-1, k - 8), -1):
        if insns[j].mnemonic == Mnemonic.CMP and insns[j].op1_kind in (OpKind.IMMEDIATE8TO32, OpKind.IMMEDIATE32, OpKind.IMMEDIATE8):
            return insns[j].immediate(1) + 1
    return None


def switch_map(pe, insns, k, start, size):
    """For the jmp [table + reg*4] at insns[k]: the block offset each switch index reaches."""
    jmp = insns[k]
    def entry(e):
        raw = pe.read(jmp.memory_displacement + 4 * e, 4)
        return struct.unpack('<I', raw)[0] - start if raw else None
    n = table_len(insns, k)
    if not n or n > 1024:
        return None
    for j in range(k - 1, max(-1, k - 4), -1):  # a byte index table first: mov dl, [idx + eax]
        i = insns[j]
        one_reg = (i.memory_index != 0) != (i.memory_base != 0)  # [table + reg], either encoding
        if i.op_count == 2 and i.op1_kind == OpKind.MEMORY and one_reg and i.memory_index_scale == 1 and \
                i.memory_displacement >= pe.base:
            idx = pe.read(i.memory_displacement, n)
            return [entry(b) for b in idx] if idx else None
    return [entry(e) for e in range(n)]


def block_sig(insns, start, off, mapaddr):
    """What the block at a function offset does, up to its ret/jmp: instructions with their absolute
    addresses as 10.5.exe ones (mapaddr) and jumps inside the function left out, to compare permuted blocks."""
    by_ip = {i.ip: n for n, i in enumerate(insns)}
    n = by_ip.get(start + off)
    out = []
    while n is not None and n < len(insns) and len(out) < 16:
        i = insns[n]
        if i.op0_kind == OpKind.NEAR_BRANCH32 and i.flow_control != FlowControl.CALL:
            out.append((i.mnemonic,))
        else:
            ops = []
            for o in range(i.op_count):
                k = i.op_kind(o)
                if k == OpKind.MEMORY:
                    ops.append(('m', mapaddr(i.memory_displacement)))  # registers may be allocated differently
                elif k in (OpKind.IMMEDIATE32, OpKind.IMMEDIATE8TO32, OpKind.IMMEDIATE8, OpKind.IMMEDIATE16):
                    ops.append(('i', mapaddr(i.immediate(o))))
                elif k == OpKind.NEAR_BRANCH32:
                    ops.append(('c', mapaddr(i.near_branch_target)))
            out.append((i.mnemonic, tuple(ops)))
        if i.flow_control in (FlowControl.RETURN, FlowControl.UNCONDITIONAL_BRANCH, FlowControl.INDIRECT_BRANCH):
            break
        n += 1
    return out


def same_code(og, sizes, a, b):
    """Two functions of 10.5.exe with the same code (an operator and its COMDAT copy): calling either is fine."""
    if a not in sizes or b not in sizes or sizes[a][1] != sizes[b][1]:
        return False
    # the same instructions, with relative branches compared by where they land (calls by target)
    key = lambda f: [(i.code, i.near_branch_target if i.flow_control == FlowControl.CALL else
                      i.near_branch_target - f if i.op0_kind == OpKind.NEAR_BRANCH32 else str(i)) for i in decode(og, f, sizes[f][1])]
    return key(a) == key(b)


def same_as_build(og, new, sizes, a, b):
    """10.5.exe's function at a and the build's at b normalise to the same code (the verifier's rule): the build
    calls its own copy of an inline (a COMDAT named after another address) that does what 10.5.exe calls."""
    if a not in sizes:
        return False
    n = sizes[a][1]
    pa = post_process_asm.post_process_asm(compare_function.dism_func(og.read(a, n)))
    pb = post_process_asm.post_process_asm(compare_function.dism_func(new.read(b, n)))
    return pa == pb


def main():
    nd, known = load()
    og, new = Pe(OG), Pe(NEW)
    sizes = {}
    for line in open('og_function_data_v105.csv'):
        p = line.strip().split(',')
        if len(p) >= 4 and p[1].startswith('0x'):
            sizes[int(p[1], 16)] = (p[0], int(p[3], 16))
    known_sorted = sorted(known)
    only = {int(a, 16) for a in sys.argv[1:]}
    bad = 0
    checked = 0
    for f in nd['functions']:
        if f.get('func_status') != '0x1' or not f.get('og_addr', '?').startswith('0x'):
            continue
        oa, na = int(f['og_addr'], 16), int(f['func_va'], 16)
        if only and oa not in only or oa not in sizes or (oa in REVIEWED and not only):
            continue
        name, size = sizes[oa]
        a, b = decode(og, oa, size), decode(new, na, size)
        if len(a) != len(b):
            continue  # compare_all_functions.py reports it
        checked += 1
        problems = []
        permuted = set()  # offsets inside switch blocks that are only laid out in another order

        def to_og(v):
            i = bisect.bisect_right(known_sorted, v) - 1
            if i >= 0 and known_sorted[i] == v:
                return known[v]
            return v if not (na <= v < na + size) else ('f', v - na)
        for k, (x, y) in enumerate(zip(a, b)):
            # absolute addresses in operands: memory displacements, immediates, call/jmp targets
            pairs = []
            for opn in range(x.op_count):
                kx, ky = x.op_kind(opn), y.op_kind(opn)
                if kx == OpKind.MEMORY and ky == OpKind.MEMORY and x.memory_base == 0 and y.memory_base == 0:
                    pairs.append((x.memory_displacement, y.memory_displacement))
                elif kx == OpKind.IMMEDIATE32 and ky == OpKind.IMMEDIATE32:
                    pairs.append((x.immediate(opn), y.immediate(opn)))
                elif kx == OpKind.NEAR_BRANCH32 and ky == OpKind.NEAR_BRANCH32 and x.flow_control == FlowControl.CALL:
                    pairs.append((x.near_branch_target, y.near_branch_target))
            for xo, yo in pairs:
                if xo == yo:
                    continue  # the same value: a constant that only looks like an address
                if not (new.inside(yo) and og.inside(xo)) or oa <= xo < oa + size:
                    continue
                # the build's address as an original one: the known symbol at or before it, plus the offset
                i = bisect.bisect_right(known_sorted, yo) - 1
                if i < 0:
                    continue
                base = known_sorted[i]
                if yo - base > 0x10000:
                    continue
                want = known[base] + (yo - base)
                if want != xo and yo == base and not same_code(og, sizes, xo, want) and not \
                        (x.flow_control == FlowControl.CALL and same_as_build(og, new, sizes, xo, yo)):
                    problems.append('%x: 10.5 references %x, the build %x (= %x)' % (x.ip - oa, xo, yo, want))
            # switches: each value (as the table index) must reach the same block, as an offset into the function;
            # through a byte index table when there is one (merged cases)
            if x.op_count and x.flow_control == FlowControl.INDIRECT_BRANCH and x.op0_kind == OpKind.MEMORY and \
                    x.memory_index_scale == 4 and x.memory_base == 0 and y.op0_kind == OpKind.MEMORY:
                ma, mb = switch_map(og, a, k, oa, size), switch_map(new, b, k, na, size)
                if ma and mb and ma != mb and len(ma) == len(mb):
                    # blocks laid out in another order are fine when each value's block does the same
                    sa = [block_sig(a, oa, o, lambda v: ('f', v - oa) if oa <= v < oa + size else v) for o in ma]
                    sb = [block_sig(b, na, o, to_og) for o in mb]
                    if sa == sb:
                        permuted.update(range(min(ma), max(ma) + 0x40))
                        continue
                if ma and mb and ma != mb:
                    diff = ['%d: %x/%x' % (v, ma[v], mb[v]) for v in range(min(len(ma), len(mb))) if ma[v] != mb[v]]
                    problems.append('%x: switch index -> block (10.5/build) differs: %s' % (x.ip - oa, ', '.join(diff[:12])))
        problems = [p for p in problems if 'switch' in p or int(p.split(':')[0], 16) not in permuted]
        if problems:
            bad += 1
            print('%s (0x%x):' % (name, oa))
            for p in problems[:10]:
                print('   ' + p)
    print('%d MATCH functions checked, %d differ' % (checked, bad))
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
