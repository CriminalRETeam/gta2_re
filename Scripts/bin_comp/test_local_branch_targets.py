"""Structural local branch fidelity after relocation-normalized asm equality."""
import contextlib
import io
import os
from pathlib import Path
import tempfile
import unittest

import compare_function
import post_process_asm


JUMP_BEFORE = bytes.fromhex('eb00 b801000000 c3 b803000000 c3')
JUMP_AFTER = bytes.fromhex('eb06 b801000000 c3 b803000000 c3')
CONDITIONAL_BEFORE = bytes.fromhex('85c9 7406 b800000000 c3 b801000000 c3 b802000000 c3 ebf2')
CONDITIONAL_AFTER = bytes.fromhex('85c9 740c b800000000 c3 b801000000 c3 b802000000 c3 ebf8')


class LocalBranchTargetTests(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.root = Path(directory.name)
        previous_cwd = os.getcwd()
        os.chdir(self.root)
        self.addCleanup(os.chdir, previous_cwd)

    def compare(self, before, after, original_offset=0, compiled_offset=0):
        original = self.root / 'original.exe'
        compiled = self.root / 'compiled.exe'
        original.write_bytes(bytes(original_offset) + before)
        compiled.write_bytes(bytes(compiled_offset) + after)
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            result = compare_function.compare_function(str(compiled), str(original), 'example',
                compiled_offset, original_offset, len(before))
        return result, output.getvalue()

    def assert_normalization_equal(self, before, after):
        normalize = lambda code: post_process_asm.post_process_asm(compare_function.dism_func(code))
        self.assertEqual(normalize(before), normalize(after))

    def test_wrong_local_jump_is_rejected_despite_normalized_equality(self):
        self.assert_normalization_equal(JUMP_BEFORE, JUMP_AFTER)
        result, output = self.compare(JUMP_BEFORE, JUMP_AFTER)
        self.assertFalse(result)
        self.assertIn('target instructions 1 and 3', output)
        self.assertIn('original offset 0x0 target 0x2', output)
        self.assertIn('compiled offset 0x0 target 0x8', output)
        self.assertEqual((self.root / 'diff/example_branch_targets.txt').read_text(),
                         output.splitlines()[-1] + '\n')

    def test_wrong_conditional_target_hidden_by_unreachable_jump_is_rejected(self):
        self.assert_normalization_equal(CONDITIONAL_BEFORE, CONDITIONAL_AFTER)
        result, output = self.compare(CONDITIONAL_BEFORE, CONDITIONAL_AFTER)
        self.assertFalse(result)
        self.assertIn('instruction 1', output)
        self.assertIn('original offset 0x2 target 0xa', output)
        self.assertIn('compiled offset 0x2 target 0x10', output)

    def test_equal_behavior_duplicate_leaf_retarget_is_not_original_structure(self):
        before = bytes.fromhex('eb00 b801000000 c3 b801000000 c3')
        after = bytes.fromhex('eb06 b801000000 c3 b801000000 c3')
        self.assert_normalization_equal(before, after)
        result, output = self.compare(before, after)
        self.assertFalse(result)
        self.assertIn('Local branch target instruction differs', output)

    def test_same_local_bindings_at_relocated_file_offsets_match(self):
        result, output = self.compare(CONDITIONAL_BEFORE, CONDITIONAL_BEFORE, 0x20, 0x60)
        self.assertTrue(result)
        self.assertEqual(output, '')

    def test_same_target_ordinal_with_different_instruction_encoding_length(self):
        before = bytes.fromhex('eb00 b801000000 c3')
        after = bytes.fromhex('e900000000 b801000000 c3')
        self.assert_normalization_equal(before, after)
        self.assertIsNone(compare_function.local_branch_target_mismatch(before, after))

    def test_both_external_relocated_tail_jumps_still_match(self):
        before, after = bytes.fromhex('e9fb0f0000'), bytes.fromhex('e9fb1f0000')
        self.assert_normalization_equal(before, after)
        self.assertTrue(self.compare(before, after)[0])

    def test_target_at_function_end_is_external(self):
        before = bytes.fromhex('eb06 b801000000 c3')
        after = bytes.fromhex('eb26 b801000000 c3')
        self.assert_normalization_equal(before, after)
        self.assertTrue(self.compare(before, after)[0])

    def test_calls_remain_outside_local_branch_guard(self):
        before, after = bytes.fromhex('e8fb0f0000 c3'), bytes.fromhex('e8fb1f0000 c3')
        self.assert_normalization_equal(before, after)
        self.assertTrue(self.compare(before, after)[0])

    def test_operand_size_16_branches_retain_existing_normalization(self):
        # A 16-bit near destination truncates actual EIP, so zero-based decode
        # offsets cannot establish whether it is inside the real function.
        before = bytes.fromhex('66e90000 9090c3 e8f8ffffff')
        after = bytes.fromhex('66e90100 9090c3 e8f9ffffff')
        self.assert_normalization_equal(before, after)
        for code, address, target in ((before, 0x401000, 0x1004),
                                      (after, 0x502000, 0x2005)):
            branch = next(iter(compare_function.Decoder(32, code, ip=address)))
            self.assertEqual(branch.op0_kind, compare_function.OpKind.NEAR_BRANCH16)
            self.assertEqual(branch.near_branch_target, target)
            self.assertFalse(address <= target < address + len(code))
        self.assertIsNone(compare_function.local_branch_target_mismatch(before, after))
        self.assertTrue(self.compare(before, after)[0])

    def test_changed_local_external_shape_is_rejected_in_both_directions(self):
        local = bytes.fromhex('eb00 b801000000 c3')
        external = bytes.fromhex('eb7e b801000000 c3')
        self.assert_normalization_equal(local, external)
        for before, after in ((local, external), (external, local)):
            with self.subTest(before=before):
                result, output = self.compare(before, after)
                self.assertFalse(result)
                self.assertIn('Local/external branch target shape differs', output)

    def test_backward_out_of_function_target_is_external(self):
        before = bytes.fromhex('eb00 b801000000 c3')
        after = bytes.fromhex('ebfc b801000000 c3')
        self.assert_normalization_equal(before, after)
        result, output = self.compare(before, after)
        self.assertFalse(result)
        self.assertIn('target 0xfffffffe', output)

    def test_mid_instruction_target_is_rejected_in_both_directions(self):
        valid = bytes.fromhex('eb00 b801000000 c3')
        invalid = bytes.fromhex('eb01 b801000000 c3')
        self.assert_normalization_equal(valid, invalid)
        for before, after in ((valid, invalid), (invalid, valid), (invalid, invalid)):
            with self.subTest(before=before, after=after):
                result, output = self.compare(before, after)
                self.assertFalse(result)
                self.assertIn('not a decoded instruction boundary', output)

    def test_valid_backward_local_loop_binding(self):
        code = bytes.fromhex('90 ebfd')
        self.assertTrue(self.compare(code, code)[0])

    def test_ordinary_asm_mismatch_still_fails(self):
        result, output = self.compare(b'\xC3', b'\x90')
        self.assertFalse(result)
        self.assertNotIn('branch target', output)

    def test_existing_exact_read_validation_is_preserved(self):
        with self.assertRaisesRegex(ValueError, 'Short function read'):
            self.compare(b'\x90\xC3', b'\xC3')

    def test_existing_invalid_instruction_validation_is_preserved(self):
        with self.assertRaisesRegex(ValueError, 'Invalid x86 instruction'):
            self.compare(b'\xE8', b'\xE9')


if __name__ == '__main__':
    unittest.main()
