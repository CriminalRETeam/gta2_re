"""Reject incomplete or invalid function bytes before assembly normalization."""
import os
from pathlib import Path
import tempfile
import unittest

import compare_function


class FunctionByteRangeTests(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.root = Path(directory.name)
        previous_cwd = os.getcwd()
        os.chdir(self.root)
        self.addCleanup(os.chdir, previous_cwd)
        self.original = self.root / "original.exe"
        self.compiled = self.root / "compiled.exe"

    def compare(self, original, compiled, size, original_offset=0, compiled_offset=0):
        self.original.write_bytes(original)
        self.compiled.write_bytes(compiled)
        return compare_function.compare_function(
            str(self.compiled), str(self.original), "example",
            compiled_offset, original_offset, size
        )

    def test_exact_read_at_file_end_is_allowed(self):
        self.original.write_bytes(b"\x90\xC3")
        self.assertEqual(compare_function.get_bytes_from_file(str(self.original), 1, 1), b"\xC3")

    def test_valid_matching_instructions_still_match(self):
        self.assertTrue(self.compare(b"\xC3", b"\xC3", 1))

    def test_valid_differing_instructions_still_fail(self):
        self.assertFalse(self.compare(b"\xC3", b"\x90", 1))

    def test_relocated_direct_calls_still_match(self):
        original = b"\xE8\x10\x00\x00\x00\xC3"
        compiled = b"\xE8\x20\x00\x00\x00\xC3"
        self.assertTrue(self.compare(original, compiled, len(original)))

    def test_relocated_global_loads_still_match(self):
        original = b"\xA1\x00\x00\x60\x00\xC3"
        compiled = b"\xA1\x00\x10\x60\x00\xC3"
        self.assertTrue(self.compare(original, compiled, len(original)))

    def test_zero_or_negative_size_is_rejected(self):
        for size in (0, -1):
            with self.subTest(size=size):
                with self.assertRaisesRegex(ValueError, "Function size must be positive"):
                    self.compare(b"\xC3", b"\xC3", size)

    def test_negative_offset_is_rejected(self):
        for offsets in ((-1, 0), (0, -1)):
            with self.subTest(offsets=offsets):
                with self.assertRaisesRegex(ValueError, "Negative function offset"):
                    self.compare(b"\xC3", b"\xC3", 1, *offsets)

    def test_offsets_at_or_past_eof_cannot_compare_empty_strings(self):
        for offset in (1, 10):
            with self.subTest(offset=offset):
                with self.assertRaisesRegex(ValueError, "expected 1 bytes, got 0"):
                    self.compare(b"\xC3", b"\xC3", 1, offset, offset)

    def test_identical_partial_reads_are_rejected(self):
        with self.assertRaisesRegex(ValueError, "expected 2 bytes, got 1"):
            self.compare(b"\xC3", b"\xC3", 2)

    def test_partial_original_or_compiled_read_is_rejected(self):
        for original, compiled, filename in (
            (b"\xC3", b"\xC3\x90", "original.exe"),
            (b"\xC3\x90", b"\xC3", "compiled.exe"),
        ):
            with self.subTest(filename=filename):
                with self.assertRaisesRegex(ValueError, "Short function read in .*" + filename):
                    self.compare(original, compiled, 2)

    def test_invalid_original_or_compiled_instruction_is_rejected(self):
        for original, compiled in ((b"\xE8", b"\xC3"), (b"\xC3", b"\xE8"),
                                   (b"\xE8", b"\xE9")):
            with self.subTest(original=original, compiled=compiled):
                with self.assertRaisesRegex(ValueError, "Invalid x86 instruction"):
                    self.compare(original, compiled, 1)

    def test_incomplete_trailing_instruction_reports_offset(self):
        with self.assertRaisesRegex(ValueError, "function offset 0x1"):
            compare_function.dism_func(b"\xC3\xE8")


if __name__ == "__main__":
    unittest.main()
