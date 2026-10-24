"""
What a build address stands for in 10.5.exe, shared by check_match_eh.py, check_match_consts.py and
check_global_values.py.

An address has an identity when check_match_refs.load() maps it (function markers, DEFINE_GLOBAL addresses,
_ADDRESS name suffixes), when it is a file-local object named after its address (found in the object files,
since the map only lists public data), when its map symbol is a CRT name listed in reccmp/library.csv, or
when it is an import slot (compared by imported name). Everything else (string literals, float constants,
vtables, COMDAT copies of inlines, GUIDs, unnamed tables) is compared by what it holds: the bytes, the
string, each vtable entry, or the function's code with absolute addresses left out.
"""
import bisect
import glob
import re
import struct

import pefile
from iced_x86 import Decoder, FlowControl, OpKind

import check_match_refs as cmr

ROOT = cmr.ROOT
MAP = ROOT + '/build_vc6/output.map'
LIBRARY_CSV = ROOT + '/reccmp/library.csv'
OBJ_DIR = ROOT + '/build_vc6/CMakeFiles/gta2_lib.dir/Source'
MAX_DATA = 0x400  # bytes compared for a referenced object of unknown size


class Image(cmr.Pe):
    def __init__(self, path):
        super().__init__(path)
        pe = struct.unpack_from('<I', self.data, 0x3C)[0]
        nsec = struct.unpack_from('<H', self.data, pe + 6)[0]
        optsize = struct.unpack_from('<H', self.data, pe + 20)[0]
        self.code = []
        for i in range(nsec):
            off = pe + 24 + optsize + i * 40
            if struct.unpack_from('<I', self.data, off + 36)[0] & 0x20000000:  # IMAGE_SCN_MEM_EXECUTE
                self.code.append(self.secs[i][:2])
        self.imports = {}
        pef = pefile.PE(path, fast_load=True)
        pef.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_IMPORT']])
        for dll in getattr(pef, 'DIRECTORY_ENTRY_IMPORT', []):
            for imp in dll.imports:
                name = imp.name.decode() if imp.name else '#%d' % imp.ordinal
                self.imports[imp.address] = '%s!%s' % (dll.dll.decode().lower(), re.sub(r'^_|@\d+$', '', name))

    def is_code(self, va):
        return any(s <= va < s + n for s, n in self.code)

    def dword(self, va):
        raw = self.read(va, 4)
        return struct.unpack('<I', raw)[0] if raw else None


def map_entries(path=MAP):
    """(address, name, object) of every public and static symbol in the build's map file."""
    out = []
    for line in open(path, errors='replace'):
        p = line.split()
        if len(p) >= 4 and re.match(r'^[0-9a-f]{4}:[0-9a-f]{8}$', p[0]) and re.match(r'^[0-9a-f]{8}$', p[2]):
            out.append((int(p[2], 16), p[1], p[-1].split(':')[-1]))
    return out


def map_symbols(path=MAP):
    """(address, name) of every public and static symbol in the build's map file, sorted."""
    return sorted({(va, name) for va, name, _ in map_entries(path)})


def read_coff(path):
    """Sections [(name, size, raw offset, characteristics, [(offset, symbol index, type)])] and symbols
    {index: (name, value, section number, storage class, type)} of an object file."""
    d = open(path, 'rb').read()
    _, nsec, _, symptr, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0)
    strtab = symptr + 18 * nsym
    secs = []
    for i in range(nsec):
        o = 20 + opt + 40 * i
        name = d[o:o + 8].rstrip(b'\0').decode(errors='replace')
        _, _, rawsz, rawptr, relptr, _, nrel, _, chars = struct.unpack_from('<IIIIIIHHI', d, o + 8)
        relocs = [struct.unpack_from('<IIH', d, relptr + 10 * r) for r in range(nrel)]
        secs.append((name, rawsz, rawptr, chars, relocs))
    syms = {}
    i = 0
    while i < nsym:
        o = symptr + 18 * i
        raw = d[o:o + 8]
        if raw[:4] == b'\0\0\0\0':
            k = strtab + struct.unpack_from('<I', raw, 4)[0]
            name = d[k:d.index(b'\0', k)].decode(errors='replace')
        else:
            name = raw.rstrip(b'\0').decode(errors='replace')
        value, sec, typ, cls, naux = struct.unpack_from('<IhHBB', d, o + 8)
        syms[i] = (name, value, sec, cls, typ)
        i += 1 + naux
    return d, secs, syms


def obj_data_symbols(new, entries=None, obj_dir=OBJ_DIR):
    """Every data symbol of the game's objects, static ones too (the map lists only public data):
    {address: (name, size)}. A section is placed by a symbol the map lists, or else by a relocation
    to it from a placed section (the exe holds the relocated address). The size runs to the next
    symbol of the section, so it can include alignment padding."""
    entries = map_entries() if entries is None else entries
    public = {name: va for va, name, _ in entries}
    static = {(obj, name): va for va, name, obj in entries}
    out = {}
    for path in sorted(glob.glob(obj_dir + '/*.obj')):
        obj = path.replace('\\', '/').split('/')[-1]
        d, secs, syms = read_coff(path)
        delta = {}
        for name, value, sec, cls, _ in syms.values():
            if sec > 0 and sec not in delta:
                va = static.get((obj, name)) if cls == 3 else public.get(name)
                if va is not None:
                    delta[sec] = va - value
        changed = True
        while changed:
            changed = False
            for sec, (_, _, rawptr, _, relocs) in enumerate(secs, 1):
                if sec not in delta:
                    continue
                for offset, index, kind in relocs:
                    target = syms.get(index)
                    if kind != 6 or not target or target[2] <= 0 or target[2] in delta:
                        continue  # IMAGE_REL_I386_DIR32 to a section not placed yet
                    final = new.dword(delta[sec] + offset)
                    if final is None:
                        continue
                    addend = struct.unpack_from('<I', d, rawptr + offset)[0]
                    delta[target[2]] = (final - addend - target[1]) & 0xFFFFFFFF
                    changed = True
        by_sec = {}
        for name, value, sec, cls, typ in syms.values():
            if sec > 0 and cls in (2, 3) and typ != 0x20 and not name.startswith(('.', '$')):
                by_sec.setdefault(sec, []).append((value, name))
        for sec, items in by_sec.items():
            secname, size, _, chars, _ = secs[sec - 1]
            if sec not in delta or chars & 0x20000000 or not secname.startswith(('.data', '.bss', '.rdata')):
                continue
            items.sort()
            for k, (value, name) in enumerate(items):
                end = items[k + 1][0] if k + 1 < len(items) else size
                if end > value:
                    out[delta[sec] + value] = (name, end - value)
    return out


def og_function_sizes():
    sizes = {}
    for line in open('og_function_data_v105.csv'):
        p = line.strip().split(',')
        if len(p) >= 4 and p[1].startswith('0x'):
            sizes[int(p[1], 16)] = (p[0], int(p[3], 16))
    return sizes


def library_names():
    """10.5.exe address of each CRT function and variable in reccmp/library.csv, by its name and symbol."""
    out = {}
    for line in open(LIBRARY_CSV):
        p = line.rstrip('\n').split(',')
        if len(p) >= 4 and re.match(r'^[0-9a-fA-F]+$', p[0]):
            for name in p[2:4]:
                if name:
                    out[name] = int(p[0], 16)
    return out


def code_until_return(pe, va, limit=0x400):
    """The instructions from va up to the first ret or jmp, for functions with no size in the csv."""
    out = []
    for i in Decoder(32, pe.read(va, limit) or b'', ip=va):
        if i.is_invalid:
            return None
        out.append(i)
        if i.flow_control in (FlowControl.RETURN, FlowControl.UNCONDITIONAL_BRANCH, FlowControl.INDIRECT_BRANCH):
            return out
    return None


SCALARS = {'C': 1, 'D': 1, 'E': 1, 'F': 2, 'G': 2, 'H': 4, 'I': 4, 'J': 4, 'K': 4, 'M': 4, 'N': 8, '_N': 1, '_J': 8,
           '_K': 8, 'VFix16@@': 4, 'VAng16@@': 2, 'VFix16_Point@@': 8, 'UFix16_Point_POD@@': 8}


def symbol_size(name):
    """The size a mangled name gives: scalar and Fix16/Ang16 globals, string literals, float constants."""
    m = re.match(r'\?[^@]+@@3(.+)[AB]$', name)
    if m and m.group(1) in SCALARS:
        return SCALARS[m.group(1)]
    m = re.match(r'\?\?_C@_([01])([0-9]|[A-P]+@)', name)
    if m:
        n = m.group(2)
        return int(n) + 1 if n.isdigit() else int(''.join('%x' % (ord(c) - 65) for c in n[:-1]), 16)
    m = re.match(r'__real@(\d+)@', name)
    return int(m.group(1)) if m else None


def data_refs(pe, va, size):
    """Absolute data addresses used by the code at va (memory operands and 32-bit immediates)."""
    out = []
    for i in Decoder(32, pe.read(va, size) or b'', ip=va):
        for o in range(i.op_count):
            k = i.op_kind(o)
            v = i.memory_displacement if k == OpKind.MEMORY else i.immediate(o) if k == OpKind.IMMEDIATE32 else 0
            if v >= pe.base and pe.inside(v) and not pe.is_code(v):
                out.append(v)
    return out


def same_shape(og, new, a, b, xa, ya):
    """The same instructions, absolute addresses aside (both in their image), branches inside compared
    by offset: compare_function.py's normalisation without its first-use renaming."""
    if len(a) != len(b):
        return False
    end_a, end_b = a[-1].next_ip, b[-1].next_ip
    for x, y in zip(a, b):
        if x.code != y.code or x.op_count != y.op_count:
            return False
        for o in range(x.op_count):
            k = x.op_kind(o)
            if k != y.op_kind(o):
                return False
            if k == OpKind.REGISTER and x.op_register(o) != y.op_register(o):
                return False
            if k == OpKind.MEMORY:
                if (x.memory_base, x.memory_index, x.memory_index_scale, x.memory_segment) != \
                        (y.memory_base, y.memory_index, y.memory_index_scale, y.memory_segment):
                    return False
                dx, dy = x.memory_displacement, y.memory_displacement
                if dx != dy and not (og.inside(dx) and new.inside(dy)):
                    return False
            if k in (OpKind.IMMEDIATE8, OpKind.IMMEDIATE16, OpKind.IMMEDIATE32, OpKind.IMMEDIATE8TO32):
                vx, vy = x.immediate(o), y.immediate(o)
                if vx != vy and not (og.inside(vx) and new.inside(vy)):
                    return False
            if k == OpKind.NEAR_BRANCH32:
                tx, ty = x.near_branch_target, y.near_branch_target
                if (xa <= tx < end_a) != (ya <= ty < end_b) or xa <= tx < end_a and tx - xa != ty - ya:
                    return False
    return True


class Refs:
    def __init__(self):
        self.nd, self.known = cmr.load()
        self.nd['functions'].sort(key=lambda f: int(f['og_addr'], 16) if f.get('og_addr', '?').startswith('0x') else 0)
        self.og, self.new = Image(cmr.OG), Image(cmr.NEW)
        self.sizes = og_function_sizes()
        entries = map_entries()
        self.data_syms = obj_data_symbols(self.new, entries)
        self.syms = sorted({(va, name) for va, name, _ in entries} |
                           {(va, name) for va, (name, _) in self.data_syms.items()})
        self.sym_vas = [va for va, _ in self.syms]
        for va, (name, _) in self.data_syms.items():
            m = re.search(r'_([0-9A-Fa-f]{6})$', name.lstrip('?').split('@@')[0])
            if m and va not in self.known and 0x5FE000 <= int(m.group(1), 16) < 0x70B000:
                self.known[va] = int(m.group(1), 16)  # a file-local object named after its address
        self.by_name = {}
        for va, name in self.syms:
            self.by_name.setdefault(name, va)
        lib = library_names()
        self.lib = {va: lib[name] for va, name in self.syms if name in lib}
        self.og_lib_names = {a: n for n, a in lib.items()}
        self.cache = {}

    # names, for reports
    def og_name(self, xo):
        if xo in self.sizes:
            return '%x %s' % (xo, self.sizes[xo][0])
        if xo in self.og_lib_names:
            return '%x %s' % (xo, self.og_lib_names[xo])
        if xo in self.og.imports:
            return '%x [%s]' % (xo, self.og.imports[xo])
        return '%x' % xo

    def new_name(self, yo):
        if yo in self.new.imports:
            return '%x [%s]' % (yo, self.new.imports[yo])
        start, name, _ = self.symbol(yo)
        if name is None:
            return '%x' % yo
        return '%x %s%s' % (yo, name, '+0x%x' % (yo - start) if yo != start else '')

    def symbol(self, yo):
        """The build symbol (map or object file) at or before yo: (start, name, size). The size is the type size
        when the name gives it, else up to the next symbol; yo can be past it (an object with no symbol)."""
        i = bisect.bisect_right(self.sym_vas, yo) - 1
        if i < 0:
            return None, None, 0
        start, name = self.syms[i]
        size = symbol_size(name)
        if size is None and start in self.data_syms:
            size = self.data_syms[start][1]
        if size is None:
            j = bisect.bisect_right(self.sym_vas, start)
            size = (self.sym_vas[j] if j < len(self.sym_vas) else start + MAX_DATA) - start
        return start, name, size

    def og_of(self, yo):
        """10.5.exe address that the build address yo stands for, when it has an identity."""
        if yo in self.known:
            return self.known[yo]
        if yo in self.lib:
            return self.lib[yo]
        start, _, size = self.symbol(yo)
        if start is not None and yo - start < size:
            for table in (self.known, self.lib):
                if start in table:
                    return table[start] + (yo - start)
        return None

    def labels(self):
        """Addresses where 10.5.exe's and the build's objects start, as far as code and symbols tell: data
        addresses the code uses, mapped globals, map symbols. They bound objects of unknown size."""
        if not hasattr(self, '_labels'):
            og = set(self.known.values()) | set(self.og_lib_names)
            for a, (_, n) in self.sizes.items():
                og.update(data_refs(self.og, a, n))
            new = set(self.sym_vas)
            for (a, _), (b, _) in zip(self.syms, self.syms[1:]):
                if self.new.is_code(a) and b - a < 0x10000:
                    new.update(data_refs(self.new, a, b - a))
            self._labels = sorted(og), sorted(new)
        return self._labels

    def extent(self, xo, yo):
        """Bytes from xo and yo up to the next object start on either side (at most MAX_DATA)."""
        og, new = self.labels()
        i, j = bisect.bisect_right(og, xo), bisect.bisect_right(new, yo)
        n = MAX_DATA
        if i < len(og):
            n = min(n, og[i] - xo)
        if j < len(new):
            n = min(n, new[j] - yo)
        return max(n, 1)

    def diff(self, xo, yo, size=None, depth=0):
        """None when 10.5.exe's address xo and the build's yo stand for the same thing, else why not."""
        key = (xo, yo, size)
        if key not in self.cache:
            self.cache[key] = None  # breaks cycles between objects that point at each other
            self.cache[key] = self._diff(xo, yo, size, depth)
        return self.cache[key]

    def _diff(self, xo, yo, size, depth):
        if xo == yo:
            return None
        if yo in self.new.imports or xo in self.og.imports:
            a, b = self.og.imports.get(xo), self.new.imports.get(yo)
            return None if a and a == b else 'import %s, the build %s' % (a, b)
        want = self.og_of(yo)
        if want is not None:
            if want == xo or (self.new.is_code(yo) and self.same_function(xo, yo)):
                return None
            return '%s, the build %s (= %s)' % (self.og_name(xo), self.new_name(yo), self.og_name(want))
        if self.new.is_code(yo):
            if not self.og.is_code(xo):
                return '%s is data, the build %s is code' % (self.og_name(xo), self.new_name(yo))
            return None if self.same_function(xo, yo) else \
                'code of %s and the build %s differs' % (self.og_name(xo), self.new_name(yo))
        if not self.og.inside(xo):
            return '%x is outside 10.5.exe, the build %s' % (xo, self.new_name(yo))
        start, name, ssize = self.symbol(yo)
        off = yo - start if start is not None else 0
        name = name or ''
        if off == 0 and name.startswith(('??_C@_0', '??_C@_1')):
            width = 2 if name.startswith('??_C@_1') else 1
            a, b = self.c_string(self.og, xo, width), self.c_string(self.new, yo, width)
            return None if a == b else 'string %r, the build %r' % (a, b)
        if off == 0 and name.startswith('??_7'):
            for k in range(self.extent(xo, yo) // 4):
                ex, ey = self.og.dword(xo + 4 * k), self.new.dword(yo + 4 * k)
                if not self.new.is_code(ey) and not self.og.is_code(ex):
                    break
                e = self.diff(ex, ey, None, depth + 1)
                if e:
                    return 'vtable %s entry %d: %s' % (name, k, e)
            return None
        if size is None:
            size = ssize if off == 0 and name.startswith('__real@') else self.extent(xo, yo)
        return self.bytes_diff(xo, yo, size, depth)

    def bytes_diff(self, xo, yo, size, depth=0):
        a, b = self.og.read(xo, size), self.new.read(yo, size)
        if a is None or b is None:
            return 'unreadable %s / %s' % (self.og_name(xo), self.new_name(yo))
        if a == b:
            return None
        k = 0
        while k < size:
            if k % 4 == 0 and k + 4 <= size and depth < 2:
                va, vb = struct.unpack_from('<I', a, k)[0], struct.unpack_from('<I', b, k)[0]
                if va != vb and self.og.inside(va) and self.new.inside(vb):
                    e = self.diff(va, vb, None, depth + 1)
                    if e:
                        return '+%d: pointer: %s' % (k, e)
                    k += 4
                    continue
            if a[k] != b[k]:
                return '%s holds %s, the build %s holds %s (+%d, %d bytes)' % (
                    self.og_name(xo), a[k:k + 8].hex(), self.new_name(yo), b[k:k + 8].hex(), k, size)
            k += 1
        return None

    @staticmethod
    def c_string(pe, va, width):
        out = b''
        while len(out) < 0x1000:
            ch = pe.read(va + len(out), width)
            if not ch or ch == b'\0' * width:
                break
            out += ch
        return out.decode('utf-16-le' if width == 2 else 'latin-1', errors='replace')

    def same_function(self, xo, yo):
        """10.5.exe's function at xo and the build's at yo are the same code once normalised."""
        want = self.og_of(yo)
        if want is not None and (want == xo or cmr.same_code(self.og, self.sizes, xo, want)):
            return True
        if xo in self.sizes:
            if cmr.same_as_build(self.og, self.new, self.sizes, xo, yo):
                return True
            n = self.sizes[xo][1]
            a, b = cmr.decode(self.og, xo, n), cmr.decode(self.new, yo, n)
        else:
            a, b = code_until_return(self.og, xo), code_until_return(self.new, yo)
        return a is not None and b is not None and same_shape(self.og, self.new, a, b, xo, yo)


def image_operands(x, y):
    """(kind, 10.5 value, build value, operand size) for each operand pair of two paired instructions
    that can hold an absolute address: memory displacements, immediates and branch targets."""
    out = []
    if x.op_count != y.op_count:
        return out
    for o in range(x.op_count):
        kx, ky = x.op_kind(o), y.op_kind(o)
        if kx == OpKind.MEMORY and ky == OpKind.MEMORY:
            out.append(('mem', x.memory_displacement, y.memory_displacement, x.memory_size))
        elif kx == ky and kx in (OpKind.IMMEDIATE8, OpKind.IMMEDIATE16, OpKind.IMMEDIATE32, OpKind.IMMEDIATE8TO32):
            out.append(('imm', x.immediate(o) & 0xFFFFFFFF, y.immediate(o) & 0xFFFFFFFF, None))
        elif kx == OpKind.NEAR_BRANCH32 and ky == OpKind.NEAR_BRANCH32:
            out.append(('branch', x.near_branch_target, y.near_branch_target, None))
    return out
