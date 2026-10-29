"""Indexed accesses are compared from the first entry their bounds checks let them reach, as in
TextTable::ToUpper_5B5AD0's `mov ax, [eax*2 + table - 100h]`."""
import struct
import unittest

import check_match_consts
import check_match_refs as cmr
import ref_values

OG_CODE, NEW_CODE = 0x401000, 0x40B000
OG_TABLE, NEW_TABLE = 0x626590, 0x5313D8
NEW_SLOT = NEW_TABLE - 0x100  # an import slot where the build's biased displacement lands


def to_upper(table, zext=True):
    """mov eax, [esp+4]; cmp ax, 80h; jb ret; cmp ax, 0FFh; ja ret; [and eax, 0FFFFh];
    mov ax, [eax*2 + table - 100h]; ret 4"""
    tail = (b'\x25\xff\xff\x00\x00' if zext else b'') + b'\x66\x8b\x04\x45' + struct.pack('<I', table - 0x100)
    return (b'\x8b\x44\x24\x04' + b'\x66\x3d\x80\x00' + bytes([0x72, 0x06 + len(tail)]) +
            b'\x66\x3d\xff\x00' + bytes([0x77, len(tail)]) + tail + b'\xc2\x04\x00')


class Image(ref_values.Image):
    def __init__(self, regions, imports=()):
        self.data, self.secs, self.code = b'', [], []
        self.base = 0x400000
        for start, data in regions:
            self.secs.append((start, len(data), len(self.data), len(data)))
            self.data += data
        self.code = [(OG_CODE, 0x1000), (NEW_CODE, 0x1000)]
        self.imports = dict(imports)


def words(base):
    return b''.join(struct.pack('<H', base + i) for i in range(128))


def refs(new_table=words(0x80), identity=False, code=to_upper):
    r = object.__new__(ref_values.Refs)
    og_data = b'\x11' * 0x100 + words(0x80) + b'\x22' * 0x100
    r.og = Image([(OG_CODE, code(OG_TABLE)), (OG_TABLE - 0x100, og_data)])
    r.new = Image([(NEW_CODE, code(NEW_TABLE)), (NEW_SLOT, b'\0' * 0x100 + new_table + b'\x33' * 0x100)],
                  {NEW_SLOT: 'dear_imgui.dll!Button'})
    size = len(code(OG_TABLE))
    r.nd = {'functions': [{'func_status': '0x1', 'og_addr': hex(OG_CODE), 'func_va': hex(NEW_CODE)}]}
    r.sizes = {OG_CODE: ('TextTable::ToUpper_5B5AD0', size)}
    r.known = {NEW_TABLE: OG_TABLE} if identity else {}
    r.syms = [(NEW_CODE, 'f'), (NEW_SLOT, '__imp_Button'), (NEW_TABLE, '?word_626590@@3QBGB'),
              (NEW_TABLE + 0x100, 'next')]
    r.sym_vas = [va for va, _ in r.syms]
    r.data_syms, r.lib, r.og_lib_names, r.cache, r.build_addresses = {}, {}, {}, {}, True
    return r


class IndexRangeTests(unittest.TestCase):
    def range_of(self, code):
        insns = cmr.decode(Image([(OG_CODE, code)]), OG_CODE, len(code))
        return cmr.index_range(insns, len(insns) - 2, cmr.predecessors(None, insns, OG_CODE, len(code)))

    def test_bounds_checks_give_the_range(self):
        self.assertEqual(self.range_of(to_upper(OG_TABLE)), (0x80, 0xFF))

    def test_a_narrow_compare_without_zero_extension_gives_nothing(self):
        self.assertIsNone(self.range_of(to_upper(OG_TABLE, zext=False)))

    def test_a_path_around_the_checks_gives_nothing(self):
        # test ecx, ecx; jne straight to the access, past the checks
        code = to_upper(OG_TABLE)
        self.assertIsNone(self.range_of(b'\x85\xc9\x75' + bytes([len(code) - 11]) + code))


class BiasedTableTests(unittest.TestCase):
    def differences(self, r):
        return check_match_consts.find_differences(r)[1]

    def test_a_biased_displacement_on_an_import_slot_is_not_an_import(self):
        self.assertEqual(self.differences(refs(identity=True)), [])
        self.assertEqual(self.differences(refs()), [])

    def test_a_different_entry_is_reported_against_the_table_reached(self):
        table = bytearray(words(0x80))
        table[0x20] ^= 1
        found = self.differences(refs(bytes(table)))
        self.assertEqual(len(found), 1)
        detail = found[0]['details'][0]
        self.assertTrue(detail.startswith('15: 626590 holds'), detail)
        self.assertIn('%x ?word_626590@@3QBGB holds' % NEW_TABLE, detail)

    def test_an_unbounded_index_on_an_import_slot_is_left_out(self):
        unbounded = lambda table: b'\x0f\xb7\x44\x24\x04' + b'\x66\x8b\x04\x4d' + struct.pack('<I', table - 0x100) + \
            b'\xc2\x04\x00'  # movzx eax, word [esp+4]; mov ax, [ecx*2 + table - 100h]; ret 4
        summary, found = check_match_consts.find_differences(refs(code=unbounded))
        self.assertEqual(found, [])
        self.assertIn('1 left out', summary)


if __name__ == '__main__':
    unittest.main()
