"""Unsupported switch evidence must not waive independent reference discrepancies."""
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest

from iced_x86 import Decoder, FlowControl
import check_match_refs as checker


START, TABLE, SELECTOR = 0x401000, 0x402000, 0x402100


class Image(checker.Pe):
    """Use production PE readers over explicitly declared raw/virtual ranges."""
    def __init__(self, regions):
        self.base, self.data, self.secs = 0x400000, b'', []
        for address, data, rawsize, virtual in regions:
            self.secs.append((address, max(rawsize, virtual), len(self.data), rawsize))
            self.data += data


def region(address, data, rawsize=None, virtual=None):
    return address, data, len(data) if rawsize is None else rawsize, len(data) if virtual is None else virtual


def direct(bound=1, jump=b'\xff\x24\x85', neutral=b''):
    return b'\x83\xf8' + bytes([bound]) + neutral + b'\x77\x0d' + jump + struct.pack('<I', TABLE) + b'\xa1\0\x21\x40\0\xc3\xc3'


def remapped(bound=2, mov=b'\x8a\x90'):
    return b'\x83\xf8' + bytes([bound]) + b'\x77\x15\x33\xd2' + mov + struct.pack('<I', SELECTOR) + b'\xff\x24\x95' + struct.pack('<I', TABLE) + b'\xa1\0\x21\x40\0\xc3\xc3'


def dwords(*values):
    return b''.join(struct.pack('<I', value) for value in values)


def mapped(code, entries, selector=None):
    regions = [region(TABLE, dwords(*entries))]
    if selector is not None:
        regions.append(region(SELECTOR, selector))
    regions.append(region(START, code))
    return Image(regions)


def switch(image, code, size=None):
    insns = list(Decoder(32, code, ip=START))
    index = next(k for k, ins in enumerate(insns) if ins.flow_control == FlowControl.INDIRECT_BRANCH)
    return checker.switch_map(image, insns, index, START, len(code) if size is None else size)


class SwitchMapTests(unittest.TestCase):
    def test_direct_table_and_duplicate_targets(self):
        code = direct()
        self.assertEqual(switch(mapped(code, [START + 12, START + 12]), code), [12, 12])

    def test_neutral_instructions_between_cmp_and_ja_remain_supported(self):
        code = direct(neutral=b'\x89\xc9')
        self.assertEqual(switch(mapped(code, [START + 14, START + 20]), code), [14, 20])

    def test_byte_remap_preserves_order_and_duplicate_selectors(self):
        code = remapped()
        self.assertEqual(switch(mapped(code, [START + 20, START + 26], b'\1\0\1'), code), [26, 20, 26])

    def test_remap_may_select_a_slot_above_the_guard_count(self):
        code = remapped()
        entries = [START + 20] * 7 + [START + 26]
        self.assertEqual(switch(mapped(code, entries, b'\7\0\7'), code), [26, 20, 26])

    def test_remap_requires_all_bytes_through_the_highest_selected_slot(self):
        code = remapped()
        self.assertIsNone(switch(mapped(code, [START + 20] * 7, b'\7\0\7'), code))

    def test_missing_partial_and_virtual_dwords_are_not_evidence(self):
        code = direct(bound=0)
        for data, rawsize, virtual in [(b'', 0, 4), (dwords(START + 12)[:3], 3, 4),
                                       (dwords(START + 12)[:3], 4, 4)]:
            with self.subTest(data=data, rawsize=rawsize):
                # Put the table last so an overstated raw range crosses physical EOF.
                image = Image([region(START, code), region(TABLE, data, rawsize, virtual)])
                self.assertIsNone(switch(image, code))

    def test_a_later_short_dword_cannot_supply_a_partial_table(self):
        code = direct()
        image = Image([region(START, code), region(TABLE, dwords(START + 12, START + 18)[:7], 7, 8)])
        self.assertIsNone(switch(image, code))

    def test_dword_span_cannot_cross_raw_sections(self):
        code = direct()
        image = Image([region(START, code), region(TABLE, dwords(START + 12)),
                       region(TABLE + 4, dwords(START + 18))])
        self.assertIsNone(switch(image, code))

    def test_selector_bytes_cannot_be_missing_partial_or_virtual(self):
        code = remapped()
        for data, rawsize, virtual in [(b'', 0, 3), (b'\1\0', 2, 3), (b'\1\0', 3, 3)]:
            with self.subTest(data=data, rawsize=rawsize):
                image = Image([region(START, code), region(TABLE, dwords(START + 20, START + 26)),
                               region(SELECTOR, data, rawsize, virtual)])
                self.assertIsNone(switch(image, code))

    def test_caller_window_must_be_complete_and_valid_through_its_end(self):
        code = direct()
        for data, rawsize, virtual in [(code[:-1], len(code) - 1, len(code)),
                                       (code[:-1], len(code), len(code)),
                                       (code + b'\xe8', len(code) + 1, len(code) + 1)]:
            with self.subTest(data=data, rawsize=rawsize):
                image = Image([region(TABLE, dwords(START + 12, START + 18)),
                               region(START, data, rawsize, virtual)])
                self.assertIsNone(switch(image, data, virtual))

    def test_nonlocal_end_and_mid_instruction_targets_are_not_evidence(self):
        code = direct(bound=0)
        for target in (0, START - 1, START + 1, START + len(code), START + 0x100):
            with self.subTest(target=target):
                self.assertIsNone(switch(mapped(code, [target]), code))

    def test_only_near32_dword_absolute_indexed_jumps_are_supported(self):
        for jump in (b'\x66\xff\x24\x85', b'\xff\x2c\x85', b'\xff\x24\x45',
                     b'\xff\x24\x05', b'\xff\x25'):
            with self.subTest(jump=jump):
                code = direct(bound=0, jump=jump)
                self.assertIsNone(switch(mapped(code, [START + len(code) - 1]), code))

    def test_jump_segments_are_declined_on_each_side(self):
        for segment in (b'\x26', b'\x2e', b'\x36', b'\x3e', b'\x64', b'\x65'):
            with self.subTest(segment=segment):
                code = direct(bound=0, jump=segment + b'\xff\x24\x85')
                self.assertIsNone(switch(mapped(code, [START + len(code) - 1]), code))

    def test_unsupported_selected_remap_cannot_fall_through_to_direct_inference(self):
        for mov in (b'\x8d\x90', b'\x8b\x90', b'\x8a\x88', b'\x64\x8a\x90'):
            with self.subTest(mov=mov):
                code = remapped(mov=mov)
                # A direct-table fallback would otherwise find enough valid local targets.
                target = START + len(code) - 1
                self.assertIsNone(switch(mapped(code, [target] * 3, b'\0\0\0'), code))

    def test_legacy_negative_lea_omission_does_not_expand_coverage(self):
        code = b'\x8d\x42\xff' + direct(bound=0)
        self.assertIsNone(switch(mapped(code, [START + len(code) - 1]), code))

    def test_absent_or_oversized_legacy_length_remains_unsupported(self):
        for code in (direct()[3:], b'\x3d\0\x04\0\0' + direct()[3:]):
            with self.subTest(code=code):
                self.assertIsNone(switch(mapped(code, [START + len(code) - 1]), code))


def pe_image(base, code, targets):
    """Minimal real PE32 with independent .text and .data file-backed sections."""
    data = bytearray(0x800)
    data[:2] = b'MZ'
    struct.pack_into('<I', data, 0x3C, 0x80)
    data[0x80:0x84] = b'PE\0\0'
    struct.pack_into('<HHIIIHH', data, 0x84, 0x14C, 2, 0, 0, 0, 0xE0, 0x10F)
    opt = 0x98
    struct.pack_into('<H', data, opt, 0x10B)
    for offset, value in [(16, 0x1000), (28, base), (32, 0x1000), (36, 0x200),
                          (56, 0x3000), (60, 0x200), (92, 16)]:
        struct.pack_into('<I', data, opt + offset, value)
    for offset, name, va, raw, length, flags in [(0x178, b'.text', 0x1000, 0x200, 0x200, 0x60000020),
                                               (0x1A0, b'.data', 0x2000, 0x400, 0x400, 0xC0000040)]:
        data[offset:offset + 8] = name.ljust(8, b'\0')
        struct.pack_into('<IIIIIIHHI', data, offset + 8, length, va, length, raw, 0, 0, 0, 0, flags)
    data[0x200:0x200 + len(code)] = code
    data[0x400:0x400 + 4 * len(targets)] = dwords(*targets)
    return bytes(data)


def caller(base, globals_, swapped=False, segment=b''):
    body = b''.join(b'\xa1' + struct.pack('<I', value) + b'\xc3' for value in globals_) + b'\xc3'
    header = b'\x83\xf8' + bytes([len(globals_) - 1]) + b'\x77' + bytes([7 + len(segment) + len(body) - 1])
    header += segment + b'\xff\x24\x85' + struct.pack('<I', base + 0x2000)
    offsets = [len(header) + 6 * index for index in range(len(globals_))]
    return header + body, offsets[::-1] if swapped else offsets


class SwitchCliTests(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.root = Path(directory.name)
        self.scripts = self.root / 'Scripts/bin_comp'
        self.scripts.mkdir(parents=True)
        (self.root / 'build_vc6').mkdir()

    def run_pair(self, old_code, new_code, old_targets, new_targets, variables):
        (self.scripts / '10.5.exe').write_bytes(pe_image(0x400000, old_code, old_targets))
        (self.root / 'build_vc6/decomp_main.exe').write_bytes(pe_image(0x450000, new_code, new_targets))
        (self.scripts / 'og_function_data_v105.csv').write_text('switch_fixture,0x401000,0x200,0x%x\n' % len(old_code))
        metadata = {'functions': [{'mangled_name': '?caller@@YAHXZ', 'func_va': '0x451000',
                                   'og_addr': '0x401000', 'func_status': '0x1'}],
                    'variables': [{'mangled_name': name, 'og_address': hex(original)} for name, _, original in variables]}
        (self.scripts / 'new_data.json').write_text(json.dumps(metadata))
        rows = [('?caller@@YAHXZ', 0x451000)] + [(name, address) for name, address, _ in variables]
        (self.root / 'build_vc6/output.map').write_text(''.join(' 0001:00000000 %s %08x f fixture.obj\n' % row for row in rows))
        return subprocess.run([sys.executable, '-B', str(Path(checker.__file__).resolve())],
                              cwd=self.scripts, capture_output=True, text=True)

    def wrong_global(self, offsets, segment=b''):
        old, _ = caller(0x400000, [0x402100], segment=segment)
        new, _ = caller(0x450000, [0x452100], segment=segment)
        return self.run_pair(old, new, [0x401000 + offsets[0]], [0x451000 + offsets[1]],
                             [('?table@@3HA', 0x452000, 0x402000), ('?wrong@@3HA', 0x452100, 0x402104)])

    def test_mid_instruction_tables_cannot_waive_an_independent_wrong_global(self):
        run = self.wrong_global((1, 2))
        self.assertEqual((run.returncode, run.stderr), (1, ''))
        self.assertIn('10.5 references 402100, the build 452100 (= 402104)', run.stdout)
        self.assertIn('1 MATCH functions checked, 1 differ', run.stdout)

    def test_nonlocal_targets_cannot_waive_an_independent_wrong_global(self):
        run = self.wrong_global((-1, -2))
        self.assertEqual((run.returncode, run.stderr), (1, ''))
        self.assertIn('10.5 references 402100, the build 452100 (= 402104)', run.stdout)

    def test_invalid_caller_tail_cannot_supply_a_permutation_witness(self):
        old, _ = caller(0x400000, [0x402100])
        new, _ = caller(0x450000, [0x452100])
        run = self.run_pair(old + b'\xe8', new + b'\xe8', [0x401001], [0x451002],
                            [('?table@@3HA', 0x452000, 0x402000), ('?wrong@@3HA', 0x452100, 0x402104)])
        self.assertEqual((run.returncode, run.stderr), (1, ''))
        self.assertIn('10.5 references 402100, the build 452100 (= 402104)', run.stdout)

    def test_valid_local_targets_keep_the_wrong_global_failure(self):
        run = self.wrong_global((12, 12))
        self.assertEqual((run.returncode, run.stderr), (1, ''))
        self.assertIn('10.5 references 402100, the build 452100 (= 402104)', run.stdout)

    def test_segmented_tables_cannot_waive_an_independent_wrong_global(self):
        run = self.wrong_global((1, 2), segment=b'\x64')
        self.assertEqual((run.returncode, run.stderr), (1, ''))
        self.assertIn('10.5 references 402100, the build 452100 (= 402104)', run.stdout)

    def test_a_genuine_local_case_layout_permutation_remains_successful(self):
        old, old_offsets = caller(0x400000, [0x402100, 0x402104])
        new, new_offsets = caller(0x450000, [0x452104, 0x452100], swapped=True)
        run = self.run_pair(old, new, [0x401000 + offset for offset in old_offsets],
                            [0x451000 + offset for offset in new_offsets],
                            [('?table@@3HA', 0x452000, 0x402000), ('?first@@3HA', 0x452100, 0x402100),
                             ('?second@@3HA', 0x452104, 0x402104)])
        self.assertEqual((run.returncode, run.stdout, run.stderr), (0, '1 MATCH functions checked, 0 differ\n', ''))


if __name__ == '__main__':
    unittest.main()
