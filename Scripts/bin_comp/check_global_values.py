"""
Compares the value every global with an original address has when WinMain starts, in the build and in
10.5.exe: the data in the exe plus what the static initialisers write.

Both exes' CRT startup (_ioinit, the __xi_a..__xi_z and __xc_a..__xc_z initialisers) runs under unicorn
(static_init_emu.py), then each global the build maps to a 10.5.exe address (check_match_refs.load():
DEFINE_GLOBAL addresses and _ADDRESS name suffixes) is compared byte for byte. Its size is the type's
when the mangled name gives it (scalars, Fix16, Ang16, Fix16_Point), else up to the next symbol in the
build and the next mapped global in 10.5.exe. A pointer into the image is fine when it stands for the
same thing (ref_values.py: the same global, function, string, vtable...), a pointer into the heap when
both are heap pointers.

check_global_inits.py reads 10.5.exe's initialiser code and checks the simple Fix16(n)/Ang16(n) forms
and store widths of the DEFINE_GLOBAL declarations without running anything; this checks the values
themselves, of every global, however they are computed (tables, copies, constructors, compile-time data).

Globals reviewed as equivalent or not comparable (a value computed from the machine) are in REVIEWED and
left out; check a new one against 10.5.exe before adding it there.

Usage (from Scripts/bin_comp, after build.py): python3 check_global_values.py [address ...]
Exit code 1 when something differs.
"""
import bisect
import glob
import re
import struct
import sys

import check_global_inits
import ref_values
import static_init_emu

# Globals whose value can't or needn't match (reviewed)
REVIEWED = {
    0x705334: 'dword_705334: 10.5.exe measures cycles per unit with rdtsc at startup, the build uses a constant',
}
MAX_GLOBAL = 0x200000
TYPE_SIZES = dict(check_global_inits.SIZES, s64=8, u64=8, f64=8, double=8, int=4, long=4, short=2,
                  wchar_t=2, BYTE=1, WORD=2, DWORD=4, BOOL=4, HWND=4, HANDLE=4, HINSTANCE=4, )
DECL = re.compile(r'\b(DEFINE_GLOBAL(?:_INIT|_ARRAY|_ARRAY_INIT)?)\(\s*([^,()]+?)\s*,\s*'
                  r'(\w+_[0-9A-Fa-f]{6})\s*,([^;]*)\)\s*;'
                  r'|^[ \t]*(?:static\s+|const\s+|extern\s+|volatile\s+)*([\w:]+(?:\s*\*)*)\s*(\**)\s*'
                  r'(\w+_[0-9A-Fa-f]{6})\s*((?:\[[^\]]*\])*)\s*[=;]', re.M)


def declared_sizes():
    """Size of each global named NAME_ADDRESS in Source/, from its declaration, when the type is a scalar,
    Fix16/Ang16/Fix16_Point or a pointer, or an array of those with a numeric count."""
    out = {}
    for path in glob.glob(ref_values.ROOT + '/Source/*.[ch]pp'):
        for m in DECL.finditer(open(path, errors='ignore').read()):
            if m.group(1):
                ty, name, rest = m.group(2), m.group(3), m.group(4)
                dims = [rest.split(',')[0].strip()] if 'ARRAY' in m.group(1) else []
            else:
                ty, name = m.group(5) + m.group(6), m.group(7)
                dims = re.findall(r'\[([^\]]*)\]', m.group(8))
            ty = re.sub(r'\b(const|static|volatile|struct|class)\b', '', ty).strip()
            size = 4 if '*' in ty else TYPE_SIZES.get(ty)
            for d in dims:
                size = size * int(d, 0) if size and re.fullmatch(r'0x[0-9a-fA-F]+|\d+', d.strip()) else None
            if size:
                out.setdefault(name, size)
    return out


def startup_memory(exe, names):
    emu, fails = static_init_emu.run_static_init(exe, names['___xc_a'], names['___xc_z'], names['___xi_a'],
                                                 names['___xi_z'], names['__ioinit'])
    return emu, fails


def describe(data, size):
    if size == 4:
        v = struct.unpack('<i', data)[0]
        return '0x%x (%d, %.4f as Fix16)' % (v & 0xFFFFFFFF, v, v / 16384.0)
    if size in (1, 2, 8):
        return data.hex()
    return data[:16].hex() + ('...' if len(data) > 16 else '')


def compare(refs, og_emu, new_emu, oa, na, n):
    """None when the n bytes at 10.5.exe's oa and the build's na hold the same, else the first difference."""
    a, b = og_emu.read(oa, n), new_emu.read(na, n)
    if a == b:
        return None
    heap = range(static_init_emu.HEAP, static_init_emu.HEAP + static_init_emu.HEAP_SIZE)
    k = 0
    while k < n:
        if (oa + k) % 4 == 0 and k + 4 <= n:
            x, y = struct.unpack_from('<I', a, k)[0], struct.unpack_from('<I', b, k)[0]
            if x != y and x in heap and y in heap:
                k += 4
                continue
            if x != y and refs.og.inside(x) and refs.new.inside(y):
                e = refs.diff(x, y)
                if e:
                    return k, 'pointer: %s' % e
                k += 4
                continue
        if a[k] != b[k]:
            return k, None
        k += 1
    return None


def globals_to_compare(refs, og_emu):
    """(build address, 10.5.exe address, size, bytes up to 10.5.exe's next object, name) of each global."""
    names = dict((va, name) for va, name in refs.syms)
    declared = declared_sizes()
    data = sorted((oa, na) for na, oa in refs.known.items() if not refs.new.is_code(na) and refs.new.inside(na)
                  and refs.og.inside(oa) and not refs.og.is_code(oa))
    og_starts = sorted(set(refs.labels()[0]) | og_emu.write_starts)
    out = []
    for oa, na in data:
        name = names.get(na, '%x' % na)
        i = bisect.bisect_right(og_starts, oa)
        og_extent = og_starts[i] - oa if i < len(og_starts) else MAX_GLOBAL
        n = ref_values.symbol_size(name) or declared.get(name.lstrip('?_').split('@@')[0])
        if n is None:
            n = max(1, min(refs.symbol(na)[2], og_extent, MAX_GLOBAL))
        out.append((na, oa, n, og_extent, name))
    return out


def main():
    only = {int(a, 16) for a in sys.argv[1:]}
    refs = ref_values.Refs()
    og_emu, og_fails = startup_memory(ref_values.cmr.OG, ref_values.library_names())
    new_emu, new_fails = startup_memory(ref_values.cmr.NEW, refs.by_name)
    for which, fails in (('10.5.exe', og_fails), ('build', new_fails)):
        for slot, f, e in fails:
            print('%s: initialiser %x (slot %x) failed in the emulator: %s' % (which, f, slot, e))

    checked = bad = overlaps = 0
    for na, oa, n, og_extent, name in globals_to_compare(refs, og_emu):
        if only and oa not in only or (oa in REVIEWED and not only):
            continue
        checked += 1
        d = compare(refs, og_emu, new_emu, oa, na, n)
        if not d:
            continue
        bad += 1
        k, why = d
        if why is None:
            w = n if n in (1, 2, 4, 8) else 4 if k % 4 == 0 and k + 4 <= n else 1
            start = 0 if w == n else k
            why = '10.5 %s, the build %s' % (describe(og_emu.read(oa + start, w), w),
                                             describe(new_emu.read(na + start, w), w))
        if k >= og_extent:
            overlaps += 1
            why += ' (past %d bytes, where 10.5 has another object)' % og_extent
        print('%s (0x%x, %d bytes): +%d: %s' % (name, oa, n, k, why))
    print('%d globals compared, %d differ (%d of them only where 10.5 has another object)' % (checked, bad, overlaps))
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
