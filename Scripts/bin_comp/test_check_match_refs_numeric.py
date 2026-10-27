"""Original-free regression tests for the optional numeric diagnostic."""
import contextlib
import io
import json
import struct
from pathlib import Path
import tempfile
import unittest
import warnings
from unittest import mock

import check_match_refs as checker


class Image:
    def __init__(self, code, base):
        self.data = bytes.fromhex(code)
        self.base = base
        self.image_size = 0x2000
        self.secs = [(base + 0x1000, len(self.data), 0, len(self.data))]

    def read(self, address, size):
        for start, span, raw, rawsize in self.secs:
            if start <= address < start + span:
                offset = address - start
                return self.data[raw + offset:raw + min(offset + size, rawsize)].ljust(size, b'\0')
        return None

    def inside(self, address):
        return any(start <= address < start + size for start, size, _, _ in self.secs)


class NumericOperandTests(unittest.TestCase):
    def compare(self, left, right, size=None, images=None):
        a, b = images or (Image(left, 0x400000), Image(right, 0x500000))
        return checker.numeric_operands(a, b, 0x401000, 0x501000,
                                        len(a.data) if size is None else size)

    def test_small_pushed_argument_and_raw_evidence(self):
        rows, stats = self.compare('6a0fc3', '6a10c3')
        self.assertEqual(len(rows), 1)
        self.assertEqual((rows[0]['original_value_u32'], rows[0]['compiled_value_u32']), ('0xf', '0x10'))
        self.assertEqual((rows[0]['original_bytes'], rows[0]['compiled_bytes']), ('6a0f', '6a10'))
        self.assertEqual((rows[0]['instruction_index'], rows[0]['operand_index']), (0, 0))
        self.assertEqual(stats['non_image_operand_pairs_compared'], 1)

    def test_sign_extended_bounds_are_reported_as_u32(self):
        rows, _ = self.compare('83f8f2c3', '83f8f4c3')
        self.assertEqual((rows[0]['original_value_u32'], rows[0]['compiled_value_u32']),
                         ('0xfffffff2', '0xfffffff4'))

    def test_large_flag_and_float_bit_patterns(self):
        for left, right in [('b800000010c3', 'b800000020c3'),
                            ('b80000803fc3', 'b800000040c3')]:
            with self.subTest(left=left):
                rows, _ = self.compare(left, right)
                self.assertEqual(len(rows), 1)

    def test_16_bit_explicit_push(self):
        rows, _ = self.compare('66680100c3', '66680200c3')
        self.assertEqual(len(rows), 1)

    def test_equal_numbers_are_compared_without_candidates(self):
        rows, stats = self.compare('6a01c3', '6a01c3')
        self.assertEqual(rows, [])
        self.assertEqual(stats['non_image_operand_pairs_compared'], 1)

    def test_numbers_visible_to_normalizer_remain_outside_scope(self):
        rows, stats = self.compare('83f801c3', '83f802c3')
        self.assertEqual(rows, [])
        self.assertEqual(stats['normalization_unequal_excluded'], 1)

    def test_equal_formatted_assembly_with_different_encodings_is_not_aligned(self):
        rows, stats = self.compare('31c0c3', '33c0c3')  # xor eax,eax; ret, opposite operand forms
        self.assertEqual(rows, [])
        self.assertEqual(stats['instruction_alignment_excluded'], 1)

    def test_pointer_shaped_values_in_either_image_headers_or_gaps_are_excluded(self):
        for left, right in [('6810004000c3', '6810005000c3'),
                            ('6810005000c3', '680000007fc3')]:
            with self.subTest(left=left):
                rows, stats = self.compare(left, right)
                self.assertEqual(rows, [])
                self.assertEqual(stats['image_span_operand_pairs_excluded'], 1)

    def test_sizeofimage_includes_final_section_alignment_padding(self):
        def parsed_image(code, base, folder):
            data = bytearray(0x200) + bytes.fromhex(code)
            struct.pack_into('<I', data, 0x3C, 0x40)
            struct.pack_into('<H', data, 0x40 + 6, 1)
            struct.pack_into('<H', data, 0x40 + 20, 0x60)
            struct.pack_into('<I', data, 0x40 + 24 + 28, base)
            struct.pack_into('<I', data, 0x40 + 24 + 56, 0x3000)
            struct.pack_into('<IIII', data, 0x40 + 24 + 0x60 + 8, 6, 0x1000, 6, 0x200)
            path = folder / (hex(base) + '.exe')
            path.write_bytes(data)
            with warnings.catch_warnings():
                warnings.simplefilter('ignore', ResourceWarning)  # Existing Pe reader uses open().read().
                return checker.Pe(path)
        with tempfile.TemporaryDirectory() as directory:
            a = parsed_image('68ff2f4000c3', 0x400000, Path(directory))
            b = parsed_image('68ff2f5000c3', 0x500000, Path(directory))
            self.assertEqual(a.image_size, 0x3000)
            self.assertFalse(a.inside(0x402FFF))  # Outside sections, inside SizeOfImage.
            rows, stats = self.compare('', '', size=6, images=(a, b))
            self.assertEqual(rows, [])
            self.assertEqual(stats['image_span_operand_pairs_excluded'], 1)

    def test_bytes_after_return_or_skipped_by_jump_are_not_candidates(self):
        for left, right in [('c36a01', 'c36a02'), ('eb026a01c3', 'eb026a02c3')]:
            with self.subTest(left=left):
                rows, stats = self.compare(left, right)
                self.assertEqual(rows, [])
                self.assertGreater(stats['unreached_instruction_pairs_excluded'], 0)

    def test_both_direct_conditional_paths_are_checked(self):
        rows, _ = self.compare('74036a01c36a03c3', '74036a02c36a04c3')
        self.assertEqual([row['instruction_index'] for row in rows], [1, 3])

    def test_opaque_indirect_jump_is_a_traversal_boundary(self):
        rows, stats = self.compare('ff206a01c3', 'ff206a02c3')
        self.assertEqual(rows, [])
        self.assertEqual(stats['unreached_instruction_pairs_excluded'], 2)

    def test_branch_into_instruction_interior_is_not_followed(self):
        rows, _ = self.compare('eb016a01c3', 'eb016a02c3')
        self.assertEqual(rows, [])

    def test_complete_file_backing_is_required(self):
        a, b = Image('c3', 0x400000), Image('c3', 0x500000)
        a.secs = [(0x401000, 2, 0, 1)]
        b.secs = [(0x501000, 2, 0, 1)]
        self.assertEqual(a.read(0x401000, 2), b'\xc3\0')
        for size in (0, -1, 2):
            with self.subTest(size=size):
                rows, stats = self.compare('', '', size=size, images=(a, b))
                self.assertEqual(rows, [])
                self.assertEqual(stats['incomplete_file_backing_excluded'], 1)

    def test_truncated_raw_file_does_not_inherit_section_size(self):
        a, b = Image('6a01c3', 0x400000), Image('6a02c3', 0x500000)
        a.data = a.data[:-1]
        rows, stats = self.compare('', '', size=3, images=(a, b))
        self.assertEqual(rows, [])
        self.assertEqual(stats['incomplete_file_backing_excluded'], 1)

    def test_invalid_decode_on_either_side_is_excluded(self):
        for left, right in [('e8', 'e9'), ('e8', 'c3'), ('c3', 'e8')]:
            with self.subTest(left=left):
                rows, stats = self.compare(left, right)
                self.assertEqual(rows, [])
                self.assertEqual(stats['invalid_decode_excluded'], 1)


class NumericCliTests(unittest.TestCase):
    def setUp(self):
        # The accepted checker's CSV iterator predates this optional feature.
        self.enterContext(warnings.catch_warnings())
        warnings.simplefilter('ignore', ResourceWarning)
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        (self.root / 'build_vc6').mkdir()
        (self.root / 'build_vc6/output.map').write_text('')
        self.scripts = self.root / 'Scripts/bin_comp'
        self.scripts.mkdir(parents=True)
        self.a, self.b = Image('6a01c3', 0x400000), Image('6a02c3', 0x500000)
        self.og = self.scripts / '10.5.exe'
        self.new = self.root / 'build_vc6/decomp_main.exe'
        self.og.write_bytes(self.a.data)
        self.new.write_bytes(self.b.data)
        self.functions = [{'og_addr': '0x401000', 'func_va': '0x501000', 'func_status': '0x1'}]
        self.known = {}
        (self.scripts / 'new_data.json').write_text(json.dumps({'functions': self.functions}))
        (self.scripts / 'og_function_data_v105.csv').write_text('fixture,0x401000,0,0x3\n')
        previous = Path.cwd()
        import os
        os.chdir(self.scripts)
        self.addCleanup(os.chdir, previous)
        for name, value in [('ROOT', str(self.root)), ('OG', str(self.og)), ('NEW', str(self.new))]:
            patch = mock.patch.object(checker, name, value)
            patch.start()
            self.addCleanup(patch.stop)

    def run_main(self, *arguments):
        output = io.StringIO()
        with mock.patch.object(checker, 'load', return_value=({'functions': self.functions}, self.known)), \
             mock.patch.object(checker, 'Pe', side_effect=lambda path: self.a if path == str(self.og) else self.b), \
             mock.patch('sys.argv', ['check_match_refs.py', *arguments]), contextlib.redirect_stdout(output):
            with self.assertRaises(SystemExit) as result:
                checker.main()
        return result.exception.code, output.getvalue()

    def test_default_verdict_output_and_no_report_io_are_preserved(self):
        with mock.patch.object(checker, 'numeric_report') as report:
            status, output = self.run_main()
        self.assertEqual((status, output), (0, '1 MATCH functions checked, 0 differ\n'))
        report.assert_not_called()

    def test_numeric_findings_do_not_turn_into_reference_failures(self):
        path = self.root / 'numeric.json'
        status, output = self.run_main('--numeric-json', str(path))
        report = json.loads(path.read_text())
        self.assertEqual(status, 0)
        self.assertTrue(output.startswith('1 MATCH functions checked, 0 differ\n'))
        self.assertTrue(report['diagnostic_only'])
        self.assertEqual(report['candidate_count'], 1)
        self.assertEqual(set(report['input_sha256']), {'original', 'compiled', 'metadata', 'function_csv', 'map'})

    def test_report_bytes_are_repeatable_and_independent_of_output_filename(self):
        a, b = self.root / 'a.json', self.root / 'b.json'
        self.run_main('--numeric-json', str(a))
        self.run_main('--numeric-json', str(b))
        self.assertEqual(a.read_bytes(), b.read_bytes())

    def test_address_filter_applies_to_optional_diagnostic(self):
        path = self.root / 'selected.json'
        status, output = self.run_main('0x402000', '--numeric-json', str(path))
        self.assertEqual(status, 0)
        self.assertTrue(output.startswith('0 MATCH functions checked, 0 differ\n'))
        self.assertEqual(json.loads(path.read_text())['candidate_count'], 0)

    def test_duplicate_metadata_pair_does_not_duplicate_candidates(self):
        self.functions.append(self.functions[0].copy())
        (self.scripts / 'new_data.json').write_text(json.dumps({'functions': self.functions}))
        path = self.root / 'duplicate.json'
        self.run_main('--numeric-json', str(path))
        report = json.loads(path.read_text())
        self.assertEqual(report['candidate_count'], 1)
        self.assertEqual(report['stats']['duplicate_function_pairs_excluded'], 1)

    def test_existing_reference_failure_still_exits_one_with_numeric_report(self):
        self.a, self.b = Image('a100006000c3', 0x400000), Image('a100007000c3', 0x500000)
        self.a.secs.append((0x600000, 16, 0, 0))
        self.b.secs.append((0x700000, 16, 0, 0))
        self.known = {0x700000: 0x600004}
        (self.scripts / 'og_function_data_v105.csv').write_text('fixture,0x401000,0,0x6\n')
        path = self.root / 'failure.json'
        self.assertEqual(self.run_main()[0], 1)
        self.assertEqual(self.run_main('--numeric-json', str(path))[0], 1)
        self.assertEqual(json.loads(path.read_text())['candidate_count'], 0)

    def test_reviewed_reference_waivers_are_not_modified(self):
        reviewed = checker.REVIEWED.copy()
        self.functions[0]['og_addr'] = '0x453590'
        self.a.secs = [(0x453590, 3, 0, 3)]
        (self.scripts / 'og_function_data_v105.csv').write_text('reviewed,0x453590,0,0x3\n')
        path = self.root / 'reviewed.json'
        self.assertEqual(self.run_main()[1], '0 MATCH functions checked, 0 differ\n')
        self.assertEqual(self.run_main('--numeric-json', str(path))[0], 0)
        self.assertEqual(checker.REVIEWED, reviewed)


if __name__ == '__main__':
    unittest.main()
