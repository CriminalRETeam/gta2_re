"""
For every MATCH function, compares what each absolute address it uses holds with what 10.5.exe uses at
the same instruction, for the addresses check_match_refs.py can't map: float constants, string literals,
vtables, GUIDs and other data with no original address, calls to COMDAT copies of inlines and to CRT
functions, import slots, and offsets into a known global.

compare_all_functions.py renames every absolute address, so `fld [__real@4@3ff...]` matches whatever float
it loads, `push offset "..."` whatever string, and `call ??0Fix16@@QAE@H@Z` whatever function sits there;
check_match_refs.py only checks addresses with an original identity. Here each pair of instructions (paired
as check_match_refs.py does) is checked through ref_values.py:

- an address with an identity (marker, DEFINE_GLOBAL, reccmp/library.csv CRT name) at an offset into the
  object must be 10.5.exe's object at the same offset;
- an import slot must import the same function;
- a float constant must hold the same bytes (the operand's size), a string literal the same string, a
  vtable the same functions;
- a call or code address without a marker must be code that compare_function.py's normalisation finds
  identical to 10.5.exe's (the callee's size from og_function_data_v105.csv, else up to its ret);
- any other data the bytes up to the next map symbol (at most 1 KB), pointers in it compared the same way.

It also checks what the normaliser renames that isn't an address: each jump inside the function must go
to the same offset as 10.5.exe's (a cross-jump into the wrong copy of a block still matches), and each
immediate must be the same number (`push 43EF0000h`, a float argument, or any `push` constant, is renamed
like an address and only fails when it breaks the first-use numbering).

Jump and index tables after the code are left to check_match_refs.py, the EH handler to check_match_eh.py.
Differences reviewed as equivalent are in REVIEWED and left out; check a new one against 10.5.exe before
adding it there.

Usage (from Scripts/bin_comp, after build.py): python3 check_match_consts.py [address ...]
Exit code 1 when something differs.
"""
import bisect
import sys

from iced_x86 import MemorySizeExt, Register

import check_match_eh
import check_match_refs as cmr
import ref_values

# Differences that do the same as 10.5.exe (reviewed by reading both)
REVIEWED = {
    0x4c8e60: 'PedGroup::DestroyGroupArray_4C8E60: a stand-in for compiler-generated code; the real static destructor ($E2) calls the CRT ??_M as 10.5.exe does',
}


def biased_targets(refs, yo):
    """10.5.exe addresses yo can stand for when an index or base register is added to it: an offset into
    the object holding yo, or into the next object with an identity (`table - 0x100` for `table[i - 0x80]`)."""
    out = [refs.og_of(yo)]
    i = bisect.bisect_right(refs.sym_vas, yo)
    while i < len(refs.sym_vas) and refs.sym_vas[i] - yo < 0x1000:
        start = refs.sym_vas[i]
        if refs.og_of(start) is not None:
            out.append(refs.og_of(start) - (start - yo))
            break
        i += 1
    return [a for a in out if a is not None]


def find_differences(refs, only=()):
    """(summary line, [{'address', 'name', 'details'}] of each MATCH function, or each one in only, that differs).
    A detail repeated at another offset is listed once."""
    og, new = refs.og, refs.new
    checked = refs_checked = unchecked = 0
    found = []
    for f in refs.nd['functions']:
        if f.get('func_status') != '0x1' or not f.get('og_addr', '?').startswith('0x'):
            continue
        oa, na = int(f['og_addr'], 16), int(f['func_va'], 16)
        if only and oa not in only or oa not in refs.sizes or (oa in REVIEWED and not only):
            continue
        name, size = refs.sizes[oa]
        a, b = cmr.decode(og, oa, size), cmr.decode(new, na, size)
        if len(a) != len(b):
            continue  # compare_all_functions.py reports it
        checked += 1
        handlers = {check_match_eh.eh_handler(og, oa, size), check_match_eh.eh_handler(new, na, size)}
        problems = []
        for x, y in zip(a, b):
            for kind, xo, yo, msize in ref_values.image_operands(x, y):
                if xo == yo:
                    continue
                if not (og.inside(xo) and new.inside(yo)):
                    if kind != 'branch':
                        problems.append('%x: %s 0x%x, the build 0x%x' % (x.ip - oa, 'immediate' if kind == 'imm' else
                                                                         'displacement', xo, yo))
                    continue
                if kind == 'branch' and (oa <= xo < oa + size or na <= yo < na + size):
                    if xo - oa != yo - na:
                        problems.append('%x: jumps to +%x, the build to +%x' % (x.ip - oa, xo - oa, yo - na))
                    continue
                if yo in handlers or yo in refs.known:
                    continue  # check_match_eh.py / check_match_refs.py
                if kind == 'mem' and new.is_code(yo):
                    continue  # a jump or index table: check_match_refs.py
                n = MemorySizeExt.size(msize) if kind == 'mem' else 0
                if not n or kind == 'mem' and (x.memory_index != Register.NONE or x.memory_base != Register.NONE):
                    n = None  # indexed or address only: the whole object
                    if kind != 'branch' and yo not in refs.known and refs.symbol(yo)[0] != yo:
                        # biased by an index, or an end bound: the object can start before or after yo
                        wants = biased_targets(refs, yo) if kind == 'mem' else \
                            [a for a in [refs.og_of(yo)] if a is not None]
                        if xo in wants:
                            continue
                        if not wants:
                            unchecked += 1
                            continue
                refs_checked += 1
                e = refs.diff(xo, yo, n)
                if e:
                    problems.append('%x: %s' % (x.ip - oa, e))
        if problems:
            seen, details = set(), []
            for p in problems:
                if p.split(': ', 1)[1] not in seen:
                    seen.add(p.split(': ', 1)[1])
                    details.append(p)
            found.append({'address': oa, 'name': name, 'details': details})
    return ('%d MATCH functions checked, %d references without an original identity compared, %d left out '
            '(no object start), %d functions differ' % (checked, refs_checked, unchecked, len(found))), found


def main():
    summary, found = find_differences(ref_values.Refs(), {int(a, 16) for a in sys.argv[1:]})
    for d in found:
        print('%s (0x%x):' % (d['name'], d['address']))
        for p in d['details']:
            print('   ' + p)
    print(summary)
    sys.exit(1 if found else 0)


if __name__ == '__main__':
    main()
