"""
For every MATCH function, checks what the verifier's normalisation can't see: that the build and 10.5.exe
reference the same globals at the same instructions, call the same functions, and route the same switch
values to the same blocks (jump tables and the byte index tables of merged cases).

compare_all_functions.py renames absolute addresses to stable_name_N in order of first use and leaves
out jump tables, so a wrong `case` label, two swapped constants or a call to the wrong function still
"match". Found this way: Ped::Deallocate_45EB60 (case 28 for 29), Char::sub_54C090 (two constants
swapped between cases), Car::IsAreaDamaged_43D1C0, the script compares SCRCMD_I_OP_S/S_OP_S,
NetUi::SetSetting_51B9C0 and sound_obj::HandleTruckCorneringAudio_417FD0 (case labels).

Switch blocks laid out in another order pass when each value's block does the same (calls and globals
mapped to 10.5.exe's). Differences reviewed as equivalent are in REVIEWED and left out; check a new one
against 10.5.exe's code before adding it there.

Build addresses are mapped to original ones through build_vc6/output.map and new_data.json (globals by
their DEFINE_GLOBAL address or _ADDRESS name suffix, functions by their marker); an address the build has
no original for (strings, float constants, vtables, ...) is not checked.

Usage (from Scripts/bin_comp, after build.py): python3 check_match_refs.py [address ...]
Exit code 1 when something differs. Optional --numeric-json PATH writes a diagnostic
for differing non-address immediates hidden by normalization; its candidates do not
change that exit status. The JSON records input hashes and explicit coverage limits.
"""
import argparse, bisect, collections, hashlib, json, re, struct, sys
from pathlib import Path
import compare_function, post_process_asm
from iced_x86 import Code, Decoder, FlowControl, MemorySizeExt, Mnemonic, OpKind, Register

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
        self.image_size = struct.unpack_from('<I', self.data, pe + 24 + 56)[0]
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

    def read_backed(self, va, n):
        """Optional equivalence witnesses require one complete file-backed range."""
        if n > 0:
            for start, _, raw, rawsize in self.secs:
                offset = va - start
                if 0 <= offset and offset + n <= rawsize and raw + offset + n <= len(self.data):
                    return self.data[raw + offset:raw + offset + n]
        return None

    def inside(self, va):
        return any(s <= va < s + n for s, n, _, _ in self.secs)


def load():
    nd = json.load(open('new_data.json'))
    sym = {}  # mangled name -> all build addresses, including repeated static labels
    for line in open(ROOT + '/build_vc6/output.map', errors='replace'):
        p = line.split()
        if len(p) >= 3 and re.match(r'^[0-9a-f]{4}:[0-9a-f]{8}$', p[0]) and re.match(r'^[0-9a-f]{8}$', p[2]):
            sym.setdefault(p[1], set()).add(int(p[2], 16))
    functions = [f for f in nd['functions'] if f.get('og_addr', '?').startswith('0x')]
    required = {v['mangled_name'] for v in nd['variables']}
    required.update(f['mangled_name'] for f in functions if f.get('mangled_name'))
    conflicts = []
    for name in sorted(required.intersection(sym)):
        addresses = sym.get(name, set())
        if len(addresses) > 1:
            conflicts.append('Referenced symbol %s has multiple MAP addresses: %s' %
                             (name, ', '.join(hex(a) for a in sorted(addresses))))

    # Explicit registrations outrank inferred suffixes, but cannot contradict each other.
    explicit = {}
    for v in nd['variables']:
        addresses = sym.get(v['mangled_name'], set())
        if len(addresses) == 1:
            explicit.setdefault(next(iter(addresses)), set()).add(int(v['og_address'], 16))
    for f in functions:
        address, original = int(f['func_va'], 16), int(f['og_addr'], 16)
        addresses = sym.get(f.get('mangled_name'), set())
        if len(addresses) == 1 and address not in addresses:
            conflicts.append('Function %s metadata address %s disagrees with MAP address %s' %
                             (f['mangled_name'], hex(address), hex(next(iter(addresses)))))
        explicit.setdefault(address, set()).add(original)
    for address, originals in explicit.items():
        if len(originals) > 1:
            conflicts.append('Conflicting explicit identities at %s: %s' %
                             (hex(address), ', '.join(hex(a) for a in sorted(originals))))

    inferred = {}
    for name, addresses in sym.items():
        m = re.search(r'_([0-9A-Fa-f]{6})@', name)
        if m and 0x400000 <= int(m.group(1), 16) < 0x800000:
            if len(addresses) > 1:
                if any(address not in explicit for address in addresses):
                    conflicts.append('Inferred symbol %s has multiple MAP addresses: %s' %
                                     (name, ', '.join(hex(a) for a in sorted(addresses))))
                continue
            for address in addresses:
                if address not in explicit:
                    inferred.setdefault(address, set()).add(int(m.group(1), 16))
    for address, originals in inferred.items():
        if len(originals) > 1:
            conflicts.append('Conflicting inferred identities at %s: %s' %
                             (hex(address), ', '.join(hex(a) for a in sorted(originals))))
    if conflicts:
        raise ValueError('Reference identity conflicts: ' + '; '.join(sorted(conflicts)))
    known = {address: next(iter(originals)) for address, originals in explicit.items()}
    known.update((address, next(iter(originals))) for address, originals in inferred.items())
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
    """Map a supported table to backed, valid local instruction starts, or decline its evidence.

    The existing nearest-CMP length heuristic is retained; this does not prove guard dominance.
    """
    code = equivalence_bytes(pe, start, size)
    if code is None:
        return None
    insns = list(Decoder(32, code, ip=start))
    if not 0 <= k < len(insns):
        return None
    jmp = insns[k]
    if jmp.code != Code.JMP_RM32 or jmp.op0_kind != OpKind.MEMORY or \
            jmp.memory_base != Register.NONE or jmp.memory_index == Register.NONE or \
            jmp.memory_index_scale != 4 or MemorySizeExt.size(jmp.memory_size) != 4 or \
            jmp.segment_prefix != Register.NONE:
        return None
    n = table_len(insns, k)
    if not n or n > 1024:
        return None
    entries = list(range(n))
    for j in range(k - 1, max(-1, k - 4), -1):  # a byte index table first: mov dl, [idx + eax]
        i = insns[j]
        one_reg = (i.memory_index != 0) != (i.memory_base != 0)  # [table + reg], either encoding
        if i.op_count == 2 and i.op1_kind == OpKind.MEMORY and one_reg and i.memory_index_scale == 1 and \
                i.memory_displacement >= pe.base:
            low_byte = {Register.EAX: Register.AL, Register.ECX: Register.CL,
                        Register.EDX: Register.DL, Register.EBX: Register.BL}.get(jmp.memory_index)
            if i.mnemonic != Mnemonic.MOV or i.op0_kind != OpKind.REGISTER or i.op0_register != low_byte or \
                    MemorySizeExt.size(i.memory_size) != 1 or i.segment_prefix != Register.NONE:
                return None  # Unsupported legacy candidates must not fall through to direct-table inference.
            idx = pe.read_backed(i.memory_displacement, n)
            if idx is None:
                return None
            entries = list(idx)
            break
    raw = pe.read_backed(jmp.memory_displacement, 4 * (max(entries) + 1))
    if raw is None:
        return None
    targets = [struct.unpack_from('<I', raw, 4 * entry)[0] for entry in entries]
    boundaries = {i.ip for i in insns}
    if any(target not in boundaries for target in targets):
        return None
    return [target - start for target in targets]


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


def equivalence_bytes(pe, address, size):
    code = pe.read_backed(address, size)
    if code is None or any(i.is_invalid for i in Decoder(32, code)):
        return None
    return code


def same_code(og, sizes, a, b):
    """Two functions of 10.5.exe with the same code (an operator and its COMDAT copy): calling either is fine."""
    if a not in sizes or b not in sizes or sizes[a][1] != sizes[b][1]:
        return False
    left, right = equivalence_bytes(og, a, sizes[a][1]), equivalence_bytes(og, b, sizes[b][1])
    if left is None or right is None:
        return False  # Missing/invalid bytes cannot establish an alias witness.
    # the same instructions, with relative branches compared by where they land (calls by target)
    key = lambda code, f: [(i.code, i.near_branch_target if i.flow_control == FlowControl.CALL else
                      i.near_branch_target - f if i.op0_kind == OpKind.NEAR_BRANCH32 else str(i)) for i in Decoder(32, code, ip=f)]
    return key(left, a) == key(right, b)


def same_as_build(og, new, sizes, a, b):
    """10.5.exe's function at a and the build's at b normalise to the same code (the verifier's rule): the build
    calls its own copy of an inline (a COMDAT named after another address) that does what 10.5.exe calls."""
    if a not in sizes:
        return False
    n = sizes[a][1]
    left, right = equivalence_bytes(og, a, n), equivalence_bytes(new, b, n)
    if left is None or right is None:
        return False  # An inconclusive optional witness must not waive a discrepancy.
    pa = post_process_asm.post_process_asm(compare_function.dism_func(left))
    pb = post_process_asm.post_process_asm(compare_function.dism_func(right))
    return pa == pb



NUMERIC_KINDS = (OpKind.IMMEDIATE8, OpKind.IMMEDIATE16, OpKind.IMMEDIATE32,
                 OpKind.IMMEDIATE8TO16, OpKind.IMMEDIATE8TO32)


def numeric_bytes(pe, va, size):
    """Require one complete file-backed range; Pe.read's virtual zero fill is not evidence."""
    if size > 0:
        for start, _, raw, rawsize in pe.secs:
            offset = va - start
            if 0 <= offset and offset + size <= rawsize and raw + offset + size <= len(pe.data):
                return pe.data[raw + offset:raw + offset + size]
    return None


def numeric_reachable(insns):
    """Syntactic overapproximation: both direct conditional edges, calls only fall through."""
    at = {i.ip: i for i in insns}
    pending, seen = [insns[0].ip], set()
    while pending:
        address = pending.pop()
        if address in seen or address not in at:
            continue
        seen.add(address)
        i = at[address]
        if i.flow_control in (FlowControl.RETURN, FlowControl.INTERRUPT,
                              FlowControl.EXCEPTION, FlowControl.INDIRECT_BRANCH):
            continue
        if i.flow_control in (FlowControl.CONDITIONAL_BRANCH, FlowControl.UNCONDITIONAL_BRANCH):
            if i.op0_kind == OpKind.NEAR_BRANCH32:
                pending.append(i.near_branch_target)
            if i.flow_control == FlowControl.UNCONDITIONAL_BRANCH:
                continue
        pending.append(i.next_ip)
    return seen


def numeric_operands(og, new, oa, na, size):
    """Review candidates, not a verdict: aligned normalized-equal x86 and non-image values only."""
    stats = collections.Counter(functions_considered=1)
    left, right = numeric_bytes(og, oa, size), numeric_bytes(new, na, size)
    if left is None or right is None:
        stats['incomplete_file_backing_excluded'] += 1
        return [], stats
    a, b = list(Decoder(32, left, ip=oa)), list(Decoder(32, right, ip=na))
    if not a or any(i.is_invalid for i in a + b):
        stats['invalid_decode_excluded'] += 1
        return [], stats
    norm = lambda code: post_process_asm.post_process_asm(compare_function.dism_func(code))
    if norm(left) != norm(right):
        stats['normalization_unequal_excluded'] += 1
        return [], stats
    if len(a) != len(b) or any(x.code != y.code or x.op_count != y.op_count or
            any(x.op_kind(o) != y.op_kind(o) for o in range(x.op_count)) for x, y in zip(a, b)):
        stats['instruction_alignment_excluded'] += 1
        return [], stats
    stats['functions_aligned'] += 1
    live_a, live_b = numeric_reachable(a), numeric_reachable(b)
    # Include headers and gaps in these spans to conservatively exclude address-shaped values.
    spans = [(p.base, p.base + p.image_size) for p in (og, new)]
    records = []
    for index, (x, y) in enumerate(zip(a, b)):
        if x.ip not in live_a or y.ip not in live_b:
            stats['unreached_instruction_pairs_excluded'] += 1
            continue
        for operand in range(x.op_count):
            if x.op_kind(operand) not in NUMERIC_KINDS:
                continue
            stats['immediate_operand_pairs_reached'] += 1
            xo, yo = x.immediate(operand) & 0xFFFFFFFF, y.immediate(operand) & 0xFFFFFFFF
            if any(lo <= value < hi for value in (xo, yo) for lo, hi in spans):
                stats['image_span_operand_pairs_excluded'] += 1
                continue
            stats['non_image_operand_pairs_compared'] += 1
            if xo != yo:
                records.append({'original_function': hex(oa), 'compiled_function': hex(na),
                    'instruction_index': index, 'operand_index': operand,
                    'original_instruction': hex(x.ip), 'compiled_instruction': hex(y.ip),
                    'original_bytes': left[x.ip - oa:x.next_ip - oa].hex(),
                    'compiled_bytes': right[y.ip - na:y.next_ip - na].hex(),
                    'original_asm': str(x), 'compiled_asm': str(y),
                    'original_value_u32': hex(xo), 'compiled_value_u32': hex(yo)})
    return records, stats


def numeric_report(nd, og, new, sizes, only):
    records, stats, seen = [], collections.Counter(), set()
    for f in nd['functions']:
        if f.get('func_status') != '0x1' or not f.get('og_addr', '?').startswith('0x'):
            continue
        oa, na = int(f['og_addr'], 16), int(f['func_va'], 16)
        if only and oa not in only:
            continue
        if (oa, na) in seen:
            stats['duplicate_function_pairs_excluded'] += 1
            continue
        seen.add((oa, na))
        if oa not in sizes:
            stats['missing_original_size_excluded'] += 1
            continue
        found, counts = numeric_operands(og, new, oa, na, sizes[oa][1])
        for row in found:
            row['function_name'] = sizes[oa][0]
        records.extend(found)
        stats.update(counts)
    records.sort(key=lambda r: (int(r['original_function'], 16), r['instruction_index'], r['operand_index']))
    paths = {'original': OG, 'compiled': NEW, 'metadata': 'new_data.json',
             'function_csv': 'og_function_data_v105.csv', 'map': ROOT + '/build_vc6/output.map'}
    return {'format_version': 1, 'diagnostic_only': True, 'candidate_count': len(records),
        'scope': 'Normalized-equal, aligned 32-bit x86; complete file-backed function ranges; direct local control flow only. '
                 'Both conditional edges and call fallthrough are an overapproximation; branch conditions and callees are not executed. '
                 'Switch/opaque targets, image-span values and other operand kinds are excluded. Unknown runtime pointers can be candidates. '
                 'Differing numeric operands require review and do not change the reference-check verdict.',
        'input_sha256': {name: hashlib.sha256(Path(path).read_bytes()).hexdigest() for name, path in paths.items()},
        'implementation_sha256': {Path(path).name: hashlib.sha256(Path(path).read_bytes()).hexdigest()
            for path in (__file__, compare_function.__file__, post_process_asm.__file__)},
        'stats': dict(stats), 'candidates': records}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addresses", nargs="*", type=lambda value: int(value, 16))
    parser.add_argument("--numeric-json", type=Path, help="write optional non-image immediate evidence; findings do not change the verdict")
    args = parser.parse_args()
    nd, known = load()
    og, new = Pe(OG), Pe(NEW)
    sizes = {}
    for line in open('og_function_data_v105.csv'):
        p = line.strip().split(',')
        if len(p) >= 4 and p[1].startswith('0x'):
            sizes[int(p[1], 16)] = (p[0], int(p[3], 16))
    known_sorted = sorted(known)
    only = set(args.addresses)
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
    if args.numeric_json is not None:
        report = numeric_report(nd, og, new, sizes, only)
        args.numeric_json.write_text(json.dumps(report, indent=2, sort_keys=True) + '\n')
        print('Numeric diagnostic: %d operand candidates; wrote %s' % (report['candidate_count'], args.numeric_json))
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
