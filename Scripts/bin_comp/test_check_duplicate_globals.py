import contextlib
import io
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import check_duplicate_globals as checker


class GlobalDeclarationTests(unittest.TestCase):
    def parse(self, text):
        return list(checker.iter_globals(text, "fixture.cpp"))

    def run_checker(self, files):
        with tempfile.TemporaryDirectory() as directory:
            folder = Path(directory)
            for name, text in files.items():
                (folder / name).write_text(text)
            output = io.StringIO()
            with patch.object(checker, "SOURCE_FOLDER", folder), contextlib.redirect_stdout(output):
                with self.assertRaises(SystemExit) as result:
                    checker.main()
            return result.exception.code, output.getvalue()

    def test_multiline_plain_and_nested_initializer(self):
        declarations = self.parse("""DEFINE_GLOBAL(
    s32,
    gPlain,
    0x600000);
DEFINE_GLOBAL_INIT(
    Fix16,
    gNested,
    Fix16(compute(1, nested(2, 3)), 0),
    0x600004);
""")
        self.assertEqual([(d.name, d.address, d.src_line_num) for d in declarations],
                         [("gPlain", 0x600000, 1), ("gNested", 0x600004, 5)])

    def test_multiline_guid_aggregate_with_comma_macro(self):
        declaration, = self.parse("""DEFINE_GLOBAL_INIT(
    GUID, gGuid,
    {0x561E8920 COMMA 0x5C70 COMMA 0x11D2 COMMA {0xAD COMMA 0}},
    0x5FE928);
""")
        self.assertEqual((declaration.var_type, declaration.name, declaration.address),
                         ("GUID", "gGuid", 0x5FE928))

    def test_quotes_url_char_and_comments_do_not_end_calls(self):
        declarations = self.parse(r'''// DEFINE_GLOBAL(s32, ignored, 0x600000);
const char* words = "DEFINE_GLOBAL(s32, ignoredToo, 0x600000)";
/* DEFINE_GLOBAL_INIT(s32, ignoredBlock, 1, 0x600000); */
DEFINE_GLOBAL_INIT(
    char*, gUrl, choose("https://example.test/a),b//c/*d*/", ')'),
    /* comments may contain ), commas, and quotes: " */
    0x600008);
''')
        self.assertEqual([(d.name, d.address, d.src_line_num) for d in declarations],
                         [("gUrl", 0x600008, 4)])

    def test_escaped_quotes_and_backslashes(self):
        declaration, = self.parse(r'''DEFINE_GLOBAL_INIT(char*, gText, "a\\b\"),//still a string", 0x600000);''')
        self.assertEqual(declaration.name, "gText")

    def test_arrays_remain_excluded(self):
        code, output = self.run_checker({"a.cpp": """DEFINE_GLOBAL_ARRAY(s32, same, 3, 0x600000);
DEFINE_GLOBAL_ARRAY_INIT(
    char*, same, 2, 0x600000,
    "DEFINE_GLOBAL(s32, fake, 0x600000)" COMMA "https://x/(y)");
DEFINE_GLOBAL(s32, same, 0x600000);
"""})
        self.assertEqual((code, output), (0, ""))

    def test_duplicate_address_across_multiline_and_single_line(self):
        code, output = self.run_checker({"a.cpp": "\nDEFINE_GLOBAL(\n s32, first,\n 0x600000);\n",
                                         "b.cpp": "DEFINE_GLOBAL_INIT(s32, second, nested(1, 2), 0x600000);\n"})
        self.assertEqual(code, 1)
        self.assertIn("first on a.cpp(2)", output)
        self.assertIn("second on b.cpp(1)", output)

    def test_duplicate_name_across_initializer_and_plain_declaration(self):
        code, output = self.run_checker({"a.cpp": "DEFINE_GLOBAL_INIT(\n s32, repeated,\n 1, 0x600000);\n",
                                         "b.cpp": "\n\nDEFINE_GLOBAL(s32, repeated, 0x600004);\n"})
        self.assertEqual(code, 1)
        self.assertIn("a.cpp(1)", output)
        self.assertIn("b.cpp(3)", output)

    def test_unterminated_invocation_fails_clearly(self):
        code, output = self.run_checker({"broken.cpp": "\nDEFINE_GLOBAL_INIT(\n s32, value, nested(1, 2),\n 0x600000\n"})
        self.assertEqual(code, 2)
        self.assertIn("broken.cpp(2): unterminated DEFINE_GLOBAL_INIT invocation", output)
        self.assertNotIn("Traceback", output)

    def test_malformed_arguments_or_address_fail_clearly(self):
        for text, message in [
            ("DEFINE_GLOBAL(s32, value);", "expects 3 nonempty arguments"),
            ("DEFINE_GLOBAL(s32, value, 1, 0x600000);", "expects 3 nonempty arguments"),
            ("DEFINE_GLOBAL_INIT(s32, value, 0x600000);", "expects 4 nonempty arguments"),
            ("DEFINE_GLOBAL_INIT(s32, value, , 0x600000);", "expects 4 nonempty arguments"),
            ("DEFINE_GLOBAL(s32, value, 0xNOPE);", "invalid global address"),
        ]:
            with self.subTest(text=text):
                code, output = self.run_checker({"broken.cpp": text})
                self.assertEqual(code, 2)
                self.assertIn("broken.cpp(1): ", output)
                self.assertIn(message, output)
                self.assertNotIn("Traceback", output)

    def test_unterminated_comment_or_literal_fails_clearly(self):
        for text, message in [("/* never closed", "unterminated block comment"),
                              ('DEFINE_GLOBAL_INIT(char*, x, "never closed', "unterminated quoted literal")]:
            with self.subTest(text=text):
                code, output = self.run_checker({"broken.cpp": text})
                self.assertEqual(code, 2)
                self.assertIn("broken.cpp(1): " + message, output)

    def test_comments_between_macro_and_open_parenthesis_preserve_line(self):
        declaration, = self.parse("\nDEFINE_GLOBAL_INIT /* ) not a delimiter */\n(\n s32, value, 1, 0x600000);\n")
        self.assertEqual(declaration.src_line_num, 2)

    def test_temporary_tu_copies_and_headers_stay_excluded(self):
        code, output = self.run_checker({"a.cpp": "DEFINE_GLOBAL(s32, first, 0x600000);\n",
                                         "_tu_harness_tmp1.cpp": "DEFINE_GLOBAL(s32, first, 0x600000);\n",
                                         "a.hpp": "DEFINE_GLOBAL(s32, first, 0x600000);\n"})
        self.assertEqual((code, output), (0, ""))


if __name__ == "__main__":
    unittest.main()
