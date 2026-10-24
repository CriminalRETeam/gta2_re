"""Binary-only fixtures for complete, unambiguous COFF global symbol extraction."""
from pathlib import Path
import struct
import tempfile
import unittest

from msvc_dump_new_data import parse_lib


def member_header(name, size):
    return (name.ljust(16) + b'0'.ljust(12) + b'0'.ljust(6) + b'0'.ljust(6)
            + b'100644'.ljust(8) + str(size).encode().ljust(10) + b'`\n')


def archive(symbols, registrations=()):
    # Only the archive index and marker strings are relevant to this reader;
    # the object member is deliberately not a linkable COFF object.
    names = b''.join(symbol + b'\0' for symbol in symbols)
    size = 4 + 4 * len(symbols) + len(names)
    object_offset = 68 + size + size % 2
    index = (struct.pack('>I', len(symbols))
             + struct.pack('>I', object_offset) * len(symbols) + names)
    markers = b'\0'.join(f'gRef_{name}_{address}'.encode('ascii')
                         for name, address in registrations) + b'\0'
    return (b'!<arch>\n' + member_header(b'/', len(index)) + index
            + (b'\n' if size % 2 else b'')
            + member_header(b'fixture.obj/', len(markers)) + markers)


class GlobalSymbolMetadataTests(unittest.TestCase):
    def parse(self, data):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'fixture.lib'
            path.write_bytes(data)
            return sorted((row['name'], row['mangled_name'], row['og_address'])
                          for row in parse_lib(path))

    def test_ordinary_global(self):
        self.assertEqual(self.parse(archive([b'?value@@3HA'], [('value', '0x401000')])),
                         [('value', '?value@@3HA', '0x401000')])

    def test_complete_template_names(self):
        symbols = [b'?pool@@3PAV?$Pool@VElement@@$01@@A',
                   b'?nested@@3V?$Box@V?$Inner@$01@@@@A']
        self.assertEqual(self.parse(archive(symbols, [('pool', '0x401000'), ('nested', '0x401004')])),
                         [('nested', symbols[1].decode(), '0x401004'),
                          ('pool', symbols[0].decode(), '0x401000')])

    def test_prefix_names_keep_their_own_addresses_in_either_order(self):
        symbols = [b'?pgbh_DrawTile@@3HA', b'?pgbh_DrawTilePart@@3HA']
        markers = [('pgbh_DrawTile', '0x626C68'), ('pgbh_DrawTilePart', '0x626C6C')]
        expected = [('pgbh_DrawTile', symbols[0].decode(), '0x626C68'),
                    ('pgbh_DrawTilePart', symbols[1].decode(), '0x626C6C')]
        for order in (markers, markers[::-1]):
            with self.subTest(order=order):
                self.assertEqual(self.parse(archive(symbols, order)), expected)

    def test_nested_decoration_is_not_another_symbol(self):
        symbols = [b'?holder@@3V?$Box@$1?value@@3HA@@A', b'?value@@3HA']
        self.assertEqual(self.parse(archive(symbols, [('value', '0x401000')])),
                         [('value', '?value@@3HA', '0x401000')])

    def test_prefix_only_is_not_a_match(self):
        self.assertEqual(self.parse(archive([b'?valueExtra@@3HA'], [('value', '0x401000')])), [])

    def test_scoped_symbol_preserved(self):
        symbol = b'?value@Scope@@3HA'
        self.assertEqual(self.parse(archive([symbol], [('value', '0x401000')])),
                         [('value', symbol.decode(), '0x401000')])

    def test_duplicate_symbols_and_markers(self):
        self.assertEqual(self.parse(archive([b'?value@@3HA'] * 2, [('value', '0x401000')] * 2)),
                         [('value', '?value@@3HA', '0x401000')])

    def test_equal_numeric_addresses_with_different_spelling(self):
        rows = self.parse(archive([b'?value@@3HA'], [('value', '0x401000'), ('value', '0x00401000')]))
        self.assertEqual(len(rows), 1)
        self.assertEqual(int(rows[0][2], 16), 0x401000)

    def test_conflicting_addresses_rejected_in_either_order(self):
        markers = [('value', '0x401000'), ('value', '0x401004')]
        for order in (markers, markers[::-1]):
            with self.subTest(order=order), self.assertRaisesRegex(ValueError, 'Ambiguous original addresses'):
                self.parse(archive([b'?value@@3HA'], order))

    def test_ambiguous_scoped_symbols_rejected(self):
        with self.assertRaisesRegex(ValueError, 'Ambiguous decorated public names'):
            self.parse(archive([b'?value@A@@3HA', b'?value@B@@3HA'], [('value', '0x401000')]))

    def test_private_c_style_omission_is_unchanged(self):
        self.assertEqual(self.parse(archive([b'_privateValue'], [('privateValue', '0x401000')])), [])

    def test_unrelated_non_ascii_symbol_does_not_change_matching(self):
        rows = self.parse(archive([b'\xffunrelated', b'?value@@3HA'], [('value', '0x401000')]))
        self.assertEqual(rows, [('value', '?value@@3HA', '0x401000')])

    def test_non_ascii_registered_symbol_is_explicitly_unsupported(self):
        with self.assertRaisesRegex(ValueError, 'Non-ASCII decorated public name'):
            self.parse(archive([b'?value@@3V\xff@@A'], [('value', '0x401000')]))

    def test_invalid_utf8_does_not_join_registration_name(self):
        data = archive([b'?value@@3HA'], [('vaxlue', '0x401000')])
        data = data.replace(b'gRef_vaxlue_', b'gRef_va\xfflue_')
        self.assertEqual(self.parse(data), [])

    def test_empty_index(self):
        self.assertEqual(self.parse(archive([])), [])

    def test_malformed_archives_do_not_fall_back_to_substring_search(self):
        valid = archive([b'?value@@3HA'], [('value', '0x401000')])
        malformed = {
            'magic': b'notar!!!' + valid[8:],
            'header': valid[:24],
            'member_name': valid[:8] + b'//'.ljust(16) + valid[24:],
            'header_terminator': valid[:66] + b'xx' + valid[68:],
            'size_text': valid[:56] + b'invalid'.ljust(10) + valid[66:],
            'negative_size': valid[:56] + b'-1'.ljust(10) + valid[66:],
            'payload': valid[:56] + b'99999999'.ljust(10) + valid[66:],
            'count': valid[:68] + struct.pack('>I', 0xFFFFFFFF) + valid[72:],
            'unterminated': b'!<arch>\n' + member_header(b'/', 9) + struct.pack('>II', 1, 0) + b'x',
            'empty_symbol': b'!<arch>\n' + member_header(b'/', 9) + struct.pack('>II', 1, 0) + b'\0',
            'trailing_data': b'!<arch>\n' + member_header(b'/', 5) + struct.pack('>I', 0) + b'x',
        }
        for name, data in malformed.items():
            with self.subTest(name=name), self.assertRaises(ValueError):
                self.parse(data)


if __name__ == '__main__':
    unittest.main()
