"""A green verifier must account for every function claimed as a match."""
import contextlib
import io
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest import mock

import compare_all_functions
import compare_builds


def function(addr="0x401000", status="0x1"):
    return {
        "mangled_name": "?example@@YAXXZ",
        "og_addr": addr,
        "func_fo": "0x1000",
        "func_status": status,
    }


def write_metadata(path, functions):
    path.write_text(json.dumps({"functions": functions}))


class TemporaryVerifierTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        previous_cwd = os.getcwd()
        os.chdir(self.root)
        self.addCleanup(os.chdir, previous_cwd)
        (self.root / "og_function_data_v105.csv").write_text(
            "example,0x401000,0x1000,0x1\n"
            "another,0x402000,0x2000,0x1\n"
        )


class VerifierCoverageTests(TemporaryVerifierTests):
    def verify(self, functions, comparison_result=True):
        write_metadata(self.root / "new_data.json", functions)
        with mock.patch.object(compare_all_functions.compare_function, "compare_function",
                               return_value=comparison_result) as comparison:
            output = io.StringIO()
            with contextlib.redirect_stdout(output):
                result = compare_all_functions.check_funcs_match()
        return result, output.getvalue(), comparison

    def test_known_match_is_compared(self):
        result, output, comparison = self.verify([function()])
        self.assertTrue(result)
        self.assertIn("[1/1] funcs OK", output)
        comparison.assert_called_once_with(
            "../../build_vc6/decomp_main.exe", "10.5.exe", "example", 0x1000, 0x1000, 1
        )

    def test_known_mismatch_still_fails(self):
        result, output, comparison = self.verify([function()], comparison_result=False)
        self.assertFalse(result)
        self.assertIn("example FAIL!", output)
        comparison.assert_called_once()

    def test_unknown_match_cannot_hide_behind_a_known_match(self):
        result, output, comparison = self.verify([function(), function("0x403000")])
        self.assertFalse(result)
        self.assertIn("?example@@YAXXZ at 0x403000", output)
        self.assertIn("og_function_data_v105.csv", output)
        self.assertNotIn("funcs OK", output)
        comparison.assert_not_called()

    def test_no_claimed_matches_cannot_succeed(self):
        for functions in ([], [function(status="0x0")], [function(status="0x2")],
                          [function(status="?")]):
            with self.subTest(functions=functions):
                result, output, comparison = self.verify(functions)
                self.assertFalse(result)
                self.assertIn("No MATCH_FUNC functions", output)
                comparison.assert_not_called()


class BaselineCoverageTests(TemporaryVerifierTests):
    # Use the same synthetic build metadata, without reading game executables.
    def setUp(self):
        super().setUp()
        self.current_data = self.root / "new_data.json"
        self.current_exe = self.root / "current.exe"
        self.current_exe.write_bytes(b"\xC3")
        self.baseline = self.root / "baseline"
        self.baseline.mkdir()
        write_metadata(self.baseline / "new_data.json", [function()])
        (self.baseline / "decomp_main.exe").write_bytes(b"baseline sentinel")
        for name, value in (("NEW_DATA", str(self.current_data)),
                            ("NEW_EXE", str(self.current_exe))):
            patch = mock.patch.object(compare_builds, name, value)
            patch.start()
            self.addCleanup(patch.stop)

    def compare(self, current, baseline=None, save=False, asm=None):
        write_metadata(self.current_data, current)
        if baseline is not None:
            write_metadata(self.baseline / "new_data.json", baseline)
        argv = ["compare_builds.py", str(self.baseline)]
        if save:
            argv.append("--save")
        output = io.StringIO()
        with mock.patch("sys.argv", argv), \
                mock.patch.object(compare_builds, "func_asm", return_value="ret",
                                  side_effect=asm) as comparison, \
                contextlib.redirect_stdout(output), contextlib.redirect_stderr(output):
            result = compare_builds.main()
        return result, output.getvalue(), comparison

    def test_baseline_known_functions_are_compared(self):
        result, output, comparison = self.compare([function()])
        self.assertEqual(result, 0)
        self.assertIn("changed: 0, removed: 0", output)
        self.assertEqual(comparison.call_count, 2)

    def test_baseline_changed_match_still_fails(self):
        result, output, _ = self.compare([function()], asm=["ret", "nop"])
        self.assertEqual(result, 1)
        self.assertIn("CHANGED: example", output)

    def test_baseline_removed_match_still_fails(self):
        result, output, _ = self.compare([function("0x402000")])
        self.assertEqual(result, 1)
        self.assertIn("NO LONGER MARKED MATCH: example", output)

    def test_unknown_current_or_baseline_match_fails(self):
        for current, baseline, description in (
            ([function(), function("0x403000")], [function()], "Current build"),
            ([function()], [function(), function("0x403000")], "Baseline build"),
        ):
            with self.subTest(description=description):
                result, output, comparison = self.compare(current, baseline)
                self.assertEqual(result, 1)
                self.assertIn(description + " MATCH_FUNC missing", output)
                self.assertIn("0x403000", output)
                comparison.assert_not_called()

    def test_empty_current_or_baseline_fails(self):
        for current, baseline, description in (
            ([], [function()], "Current build"),
            ([function()], [], "Baseline build"),
        ):
            with self.subTest(description=description):
                result, output, comparison = self.compare(current, baseline)
                self.assertEqual(result, 1)
                self.assertIn(description + " has no MATCH_FUNC functions", output)
                comparison.assert_not_called()

    def test_invalid_save_does_not_overwrite_existing_baseline(self):
        original_data = (self.baseline / "new_data.json").read_bytes()
        original_exe = (self.baseline / "decomp_main.exe").read_bytes()
        for functions in ([], [function(), function("0x403000")]):
            with self.subTest(functions=functions):
                result, output, comparison = self.compare(functions, save=True)
                self.assertEqual(result, 1)
                self.assertIn("Verification error:", output)
                self.assertEqual((self.baseline / "new_data.json").read_bytes(), original_data)
                self.assertEqual((self.baseline / "decomp_main.exe").read_bytes(), original_exe)
                comparison.assert_not_called()

    def test_valid_save_copies_build_without_comparing(self):
        result, output, comparison = self.compare([function()], save=True)
        self.assertEqual(result, 0)
        self.assertIn("Saved baseline", output)
        self.assertEqual((self.baseline / "new_data.json").read_bytes(), self.current_data.read_bytes())
        self.assertEqual((self.baseline / "decomp_main.exe").read_bytes(), self.current_exe.read_bytes())
        comparison.assert_not_called()


if __name__ == "__main__":
    unittest.main()
