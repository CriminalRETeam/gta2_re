"""Optional alias witnesses must not accept missing or invalid function bytes."""
import contextlib
import io
import json
import os
from pathlib import Path
import struct
import tempfile
import unittest
from unittest import mock
import warnings

import check_match_refs as checker


class Image(checker.Pe):
    def __init__(self, regions):
        self.data, self.secs = b'', []
        self.base = min((start for start, _, _ in regions), default=0x400000) & ~0xFFFF
        for start, code, virtual_size in regions:
            self.secs.append((start, virtual_size, len(self.data), len(code)))
            self.data += code


def image(address, code, virtual_size=None):
    return Image([(address, code, len(code) if virtual_size is None else virtual_size)])


class EquivalenceWitnessTests(unittest.TestCase):
    def compare(self, original, compiled, size):
        return checker.same_as_build(original, compiled, {0x402000: ('example', size)}, 0x402000, 0x502000)

    def test_valid_exact_bodies_still_establish_a_witness(self):
        self.assertTrue(self.compare(image(0x402000, b'\xc3'), image(0x502000, b'\xc3'), 1))

    def test_valid_relocated_calls_and_globals_still_normalize(self):
        for a, b in [(b'\xe8\x10\0\0\0\xc3', b'\xe8\x20\0\0\0\xc3'),
                     (b'\xa1\0\0\x60\0\xc3', b'\xa1\0\x10\x60\0\xc3')]:
            with self.subTest(code=a):
                self.assertTrue(self.compare(image(0x402000, a), image(0x502000, b), len(a)))

    def test_valid_different_instructions_do_not_establish_a_witness(self):
        self.assertFalse(self.compare(image(0x402000, b'\xc3'), image(0x502000, b'\x90'), 1))

    def test_zero_or_negative_original_sizes_are_inconclusive(self):
        for size in (0, -1):
            with self.subTest(size=size):
                self.assertFalse(self.compare(image(0x402000, b'\xc3'), image(0x502000, b'\xc3'), size))

    def test_unmapped_original_metadata_or_compiled_range_is_inconclusive(self):
        self.assertFalse(checker.same_as_build(Image([]), Image([]), {}, 0x402000, 0x502000))
        self.assertFalse(self.compare(image(0x402000, b'\xc3'), Image([]), 1))

    def test_virtual_zero_fill_cannot_be_an_equivalence_witness(self):
        a, b = image(0x402000, b'\xc3', 3), image(0x502000, b'\xc3', 3)
        self.assertEqual(a.read(0x402000, 3), b'\xc3\0\0')
        self.assertFalse(self.compare(a, b, 3))

    def test_truncated_raw_files_are_inconclusive(self):
        for truncate_original in (False, True):
            a, b = image(0x402000, b'\x90\xc3'), image(0x502000, b'\x90\xc3')
            target = a if truncate_original else b
            target.data = target.data[:-1]
            with self.subTest(original=truncate_original):
                self.assertFalse(self.compare(a, b, 2))

    def test_range_crossing_raw_sections_is_not_guessed(self):
        a = Image([(0x402000, b'\x90', 1), (0x402001, b'\xc3', 1)])
        self.assertFalse(self.compare(a, image(0x502000, b'\x90\xc3'), 2))

    def test_invalid_or_incomplete_bytes_on_either_side_are_inconclusive(self):
        for a, b in [(b'\xe8', b'\xe8'), (b'\xe8', b'\xc3'), (b'\xc3', b'\xe8'),
                     (b'\xc3\xe8', b'\xc3\xe9')]:
            with self.subTest(original=a, compiled=b):
                with mock.patch.object(checker.compare_function, 'dism_func') as normalize:
                    self.assertFalse(self.compare(image(0x402000, a), image(0x502000, b), len(a)))
                normalize.assert_not_called()

    def test_original_aliases_reject_invalid_empty_or_unbacked_bodies(self):
        for code, size, virtual in [(b'\xe8', 1, 1), (b'\xc3', 0, 1), (b'\xc3', 3, 3)]:
            a = Image([(0x402000, code, virtual), (0x403000, code, virtual)])
            with self.subTest(code=code, size=size):
                self.assertFalse(checker.same_code(a, {0x402000: ('a', size), 0x403000: ('b', size)},
                                                   0x402000, 0x403000))

    def test_original_aliases_keep_local_branch_relocation(self):
        a = Image([(0x402000, b'\xeb\0\xc3', 3), (0x403000, b'\xeb\0\xc3', 3)])
        self.assertTrue(checker.same_code(a, {0x402000: ('a', 3), 0x403000: ('b', 3)}, 0x402000, 0x403000))

    def test_original_alias_calls_compare_absolute_destinations(self):
        direct = lambda start, target: b'\xe8' + struct.pack('<i', target - start - 5) + b'\xc3'
        for target, equal in [(0x405000, True), (0x405010, False)]:
            a = Image([(0x402000, direct(0x402000, 0x405000), 6),
                       (0x403000, direct(0x403000, target), 6)])
            with self.subTest(target=target):
                self.assertEqual(checker.same_code(a, {0x402000: ('a', 6), 0x403000: ('b', 6)},
                                                  0x402000, 0x403000), equal)

    def test_original_aliases_keep_size_and_metadata_gates(self):
        a = image(0x402000, b'\xc3')
        self.assertFalse(checker.same_code(a, {0x402000: ('a', 1)}, 0x402000, 0x403000))
        self.assertFalse(checker.same_code(a, {0x402000: ('a', 1), 0x403000: ('b', 2)}, 0x402000, 0x403000))


class ReferenceVerdictTests(unittest.TestCase):
    def setUp(self):
        self.enterContext(warnings.catch_warnings())
        warnings.simplefilter('ignore', ResourceWarning)  # Existing CSV iterator.
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.root = Path(directory.name)
        previous = Path.cwd()
        os.chdir(self.root)
        self.addCleanup(os.chdir, previous)
        self.functions = [{'og_addr': '0x401000', 'func_va': '0x501000', 'func_status': '0x1'}]
        self.known = {0x502000: 0x403000}
        self.csv = self.root / 'og_function_data_v105.csv'
        self.csv.write_text('caller,0x401000,0,0x6\ndesired,0x402000,0,0x1\nother,0x403000,0,0x1\n')

    def run_checker(self, helper):
        direct = b'\xe8\xfb\x0f\0\0\xc3'
        original = Image([(0x401000, direct, 6), (0x402000, b'\xc3', 1), (0x403000, b'\xcc', 1)])
        compiled = Image([(0x501000, direct, 6), (0x502000, helper, len(helper))])
        output = io.StringIO()
        with mock.patch.object(checker, 'load', return_value=({'functions': self.functions}, self.known)), \
             mock.patch.object(checker, 'Pe', side_effect=[original, compiled]), \
             mock.patch('sys.argv', ['check_match_refs.py']), contextlib.redirect_stdout(output):
            with self.assertRaises(SystemExit) as result:
                checker.main()
        return result.exception.code, output.getvalue()

    def test_real_wrong_callee_with_invalid_body_is_reported(self):
        status, output = self.run_checker(b'\xe8')
        self.assertEqual(status, 1)
        self.assertIn('1 MATCH functions checked, 1 differ', output)
        self.assertIn('10.5 references 402000', output)

    def test_real_wrong_callee_with_valid_different_body_is_reported(self):
        self.assertEqual(self.run_checker(b'\x90')[0], 1)

    def test_unknown_original_callee_is_not_silently_waived(self):
        self.csv.write_text('caller,0x401000,0,0x6\n')
        self.assertEqual(self.run_checker(b'\xc3')[0], 1)

    def test_valid_normalized_alias_still_preserves_reference_verdict(self):
        status, output = self.run_checker(b'\xc3')
        self.assertEqual((status, output), (0, '1 MATCH functions checked, 0 differ\n'))


if __name__ == '__main__':
    unittest.main()
