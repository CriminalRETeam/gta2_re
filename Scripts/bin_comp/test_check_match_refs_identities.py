"""Referenced declaration conflicts must not select identities by input order."""
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest import mock
import warnings

import check_match_refs as checker


def variable(name, original):
    return {'mangled_name': name, 'og_address': hex(original)}


def function(name, compiled, original):
    return {'mangled_name': name, 'func_va': hex(compiled), 'og_addr': hex(original), 'func_status': '0x1'}


def map_text(rows):
    return ''.join(' 0001:00000000 %s %08x f fixture.obj\n' % row for row in rows)


class IdentityLoadTests(unittest.TestCase):
    def setUp(self):
        self.enterContext(warnings.catch_warnings())
        warnings.simplefilter('ignore', ResourceWarning)  # Existing loader open() iterators.
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.root = Path(directory.name)
        (self.root / 'build_vc6').mkdir()
        previous = Path.cwd()
        os.chdir(self.root)
        self.addCleanup(os.chdir, previous)
        self.enterContext(mock.patch.object(checker, 'ROOT', str(self.root)))

    def load(self, rows=(), variables=(), functions=()):
        (self.root / 'new_data.json').write_text(json.dumps({'variables': list(variables), 'functions': list(functions)}))
        (self.root / 'build_vc6/output.map').write_text(map_text(rows))
        return checker.load()[1]

    def test_explicit_variable_conflict_rejected_independently_of_row_order(self):
        claims = [variable('?alpha@@3HA', 0x402000), variable('?beta@@3HA', 0x402004)]
        errors = []
        for order in (claims, claims[::-1]):
            with self.assertRaises(ValueError) as error:
                self.load([('?alpha@@3HA', 0x452000), ('?beta@@3HA', 0x452000)], order)
            errors.append(str(error.exception))
        self.assertEqual(errors[0], errors[1])
        self.assertIn('Conflicting explicit identities at 0x452000: 0x402000, 0x402004', errors[0])

    def test_referenced_variable_map_name_cannot_select_one_of_two_addresses(self):
        rows = [('?value@@3HA', 0x452000), ('?value@@3HA', 0x452004)]
        errors = []
        for order in (rows, rows[::-1]):
            with self.assertRaises(ValueError) as error:
                self.load(order, [variable('?value@@3HA', 0x402000)])
            errors.append(str(error.exception))
        self.assertEqual(errors[0], errors[1])
        self.assertIn('multiple MAP addresses: 0x452000, 0x452004', errors[0])

    def test_referenced_function_map_name_must_also_be_unambiguous(self):
        with self.assertRaisesRegex(ValueError, 'Referenced symbol'):
            self.load([('?callee@@YAXXZ', 0x451000), ('?callee@@YAXXZ', 0x451010)],
                      functions=[function('?callee@@YAXXZ', 0x451000, 0x401000)])

    def test_function_metadata_must_agree_with_its_present_map_name(self):
        with self.assertRaisesRegex(ValueError, 'metadata address 0x451000 disagrees with MAP address 0x451010'):
            self.load([('?callee@@YAXXZ', 0x451010)], functions=[function('?callee@@YAXXZ', 0x451000, 0x401000)])

    def test_function_claims_cannot_overwrite_conflicting_globals_or_functions(self):
        f = function('?callee@@YAXXZ', 0x451000, 0x401000)
        for variables, functions in [([variable('?value@@3HA', 0x402000)], [f]),
                                     ([], [f, function('?other@@YAXXZ', 0x451000, 0x401010)])]:
            with self.subTest(variables=variables):
                with self.assertRaisesRegex(ValueError, 'Conflicting explicit identities'):
                    self.load([('?value@@3HA', 0x451000), ('?callee@@YAXXZ', 0x451000), ('?other@@YAXXZ', 0x451000)],
                              variables, functions)

    def test_consistent_duplicate_claims_and_map_entries_are_supported(self):
        rows = [('?value@@3HA', 0x452000), ('?value@@3HA', 0x452000), ('?alias@@3HA', 0x452000)]
        v = variable('?value@@3HA', 0x402000)
        self.assertEqual(self.load(rows, [v, v.copy(), variable('?alias@@3HA', 0x402000)]), {0x452000: 0x402000})

    def test_one_original_identity_can_have_multiple_compiled_copies(self):
        functions = [function('?copyA@@YAXXZ', 0x451000, 0x401000), function('?copyB@@YAXXZ', 0x451010, 0x401000)]
        self.assertEqual(self.load([('?copyA@@YAXXZ', 0x451000), ('?copyB@@YAXXZ', 0x451010)], functions=functions),
                         {0x451000: 0x401000, 0x451010: 0x401000})

    def test_unrelated_repeated_static_labels_are_ignored(self):
        self.assertEqual(self.load([('_$E1', 0x451000), ('_$E1', 0x451010), ('LeadUp1', 0x451100), ('LeadUp1', 0x451110)]), {})

    def test_missing_map_names_keep_existing_omission_and_function_marker_behavior(self):
        omitted = {'mangled_name': '?absent@@3HA', 'og_address': 'not parsed when absent'}
        self.assertEqual(self.load(variables=[omitted, {'mangled_name': None, 'og_address': 'also absent'}],
                                   functions=[function('?missing@@YAXXZ', 0x451000, 0x401000)]), {0x451000: 0x401000})

    def test_unknown_original_function_marker_does_not_create_a_new_requirement(self):
        unknown = {'mangled_name': '?unknown@@YAXXZ', 'func_va': 'not parsed', 'og_addr': '?', 'func_status': '0x0'}
        self.assertEqual(self.load([('?unknown@@YAXXZ', 0x451000), ('?unknown@@YAXXZ', 0x451010)], functions=[unknown]), {})

    def test_input_recorder_constructor_explicit_marker_wins_over_class_suffix(self):
        name = '??0InputRecorder_67F8B0@@QAE@XZ'
        self.assertEqual(self.load([(name, 0x46FF50)], functions=[function(name, 0x46FF50, 0x4CDD30)]), {0x46FF50: 0x4CDD30})

    def test_explicit_variable_outranks_even_conflicting_suffix_hints(self):
        rows = [('?value@@3HA', 0x452000), ('?hint_401000@@3HA', 0x452000), ('?hint_402000@@3HA', 0x452000)]
        for order in (rows, rows[::-1]):
            self.assertEqual(self.load(order, [variable('?value@@3HA', 0x403000)]), {0x452000: 0x403000})

    def test_inferred_conflict_without_explicit_identity_cannot_choose_by_map_order(self):
        rows = [('?hint_401000@@3HA', 0x452000), ('?hint_402000@@3HA', 0x452000)]
        errors = []
        for order in (rows, rows[::-1]):
            with self.assertRaises(ValueError) as error:
                self.load(order)
            errors.append(str(error.exception))
        self.assertEqual(errors[0], errors[1])
        self.assertIn('Conflicting inferred identities at 0x452000: 0x401000, 0x402000', errors[0])

    def test_consistent_suffix_hint_supports_multiple_compiled_copies(self):
        self.assertEqual(self.load([('?copyA_401000@@3HA', 0x451000), ('?copyB_401000@@3HA', 0x451010)]),
                         {0x451000: 0x401000, 0x451010: 0x401000})

    def test_multivalued_suffix_name_cannot_add_inferred_copies(self):
        rows = [('?hint_401000@@3HA', 0x451000), ('?hint_401000@@3HA', 0x451010)]
        errors = []
        for order in (rows, rows[::-1]):
            with self.assertRaises(ValueError) as error:
                self.load(order)
            errors.append(str(error.exception))
        self.assertEqual(errors[0], errors[1])
        self.assertIn('Inferred symbol ?hint_401000@@3HA has multiple MAP addresses: 0x451000, 0x451010', errors[0])

    def test_multivalued_suffix_name_cannot_infer_a_partly_explicit_range(self):
        rows = [('?hint_401000@@3HA', 0x451000), ('?hint_401000@@3HA', 0x451010), ('?value@@3HA', 0x451000)]
        with self.assertRaisesRegex(ValueError, 'Inferred symbol'):
            self.load(rows, [variable('?value@@3HA', 0x402000)])

    def test_fully_explicit_addresses_ignore_multivalued_lower_priority_suffix(self):
        rows = [('?hint_401000@@3HA', 0x451000), ('?hint_401000@@3HA', 0x451010),
                ('?first@@3HA', 0x451000), ('?second@@3HA', 0x451010)]
        self.assertEqual(self.load(rows, [variable('?first@@3HA', 0x402000), variable('?second@@3HA', 0x402004)]),
                         {0x451000: 0x402000, 0x451010: 0x402004})


def pe_fixture(path, base, code):
    data = bytearray(0x500)
    struct.pack_into('<I', data, 0x3C, 0x40)
    data[0x40:0x44] = b'PE\0\0'
    struct.pack_into('<HH', data, 0x44, 0x14C, 2)
    struct.pack_into('<H', data, 0x40 + 20, 0xE0)
    struct.pack_into('<H', data, 0x40 + 24, 0x10B)
    struct.pack_into('<I', data, 0x40 + 24 + 28, base)
    struct.pack_into('<I', data, 0x40 + 24 + 56, 0x3000)
    section = 0x40 + 24 + 0xE0
    struct.pack_into('<IIII', data, section + 8, 0x100, 0x1000, 0x100, 0x200)
    struct.pack_into('<IIII', data, section + 40 + 8, 0x100, 0x2000, 0x100, 0x400)
    data[0x200:0x200 + len(code)] = code
    path.write_bytes(data)


class IdentityCliTests(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.root = Path(directory.name)
        self.scripts = self.root / 'Scripts/bin_comp'
        self.scripts.mkdir(parents=True)
        (self.root / 'build_vc6').mkdir()
        pe_fixture(self.scripts / '10.5.exe', 0x400000, b'\xa1\x04\x20\x40\0\xc3')
        pe_fixture(self.root / 'build_vc6/decomp_main.exe', 0x450000, b'\xa1\0\x20\x45\0\xc3')
        (self.scripts / 'og_function_data_v105.csv').write_text('caller,0x401000,0x200,0x6\n')
        self.functions = [function('?caller@@YAXXZ', 0x451000, 0x401000)]
        self.rows = [('?caller@@YAXXZ', 0x451000), ('?alpha@@3HA', 0x452000), ('?beta@@3HA', 0x452000)]

    def run_checker(self, variables, rows=None):
        (self.scripts / 'new_data.json').write_text(json.dumps({'variables': variables, 'functions': self.functions}))
        (self.root / 'build_vc6/output.map').write_text(map_text(self.rows if rows is None else rows))
        return subprocess.run([sys.executable, '-B', str(Path(checker.__file__).resolve())],
                              cwd=self.scripts, capture_output=True, text=True)

    def test_row_order_false_green_is_rejected_in_both_orders_with_same_error(self):
        claims = [variable('?alpha@@3HA', 0x402000), variable('?beta@@3HA', 0x402004)]
        runs = [self.run_checker(order) for order in (claims, claims[::-1])]
        self.assertEqual([run.returncode for run in runs], [1, 1])
        self.assertEqual(runs[0].stderr, runs[1].stderr)
        self.assertTrue(all(not run.stdout for run in runs))
        self.assertIn('Conflicting explicit identities at 0x452000: 0x402000, 0x402004', runs[0].stderr)

    def test_valid_aliases_and_unrelated_statics_preserve_success(self):
        claims = [variable('?alpha@@3HA', 0x402004), variable('?beta@@3HA', 0x402004)]
        run = self.run_checker(claims, self.rows + [('_$E1', 0x451010), ('_$E1', 0x451020)])
        self.assertEqual((run.returncode, run.stdout, run.stderr), (0, '1 MATCH functions checked, 0 differ\n', ''))

    def test_referenced_map_ambiguity_is_rejected_before_a_green_verdict(self):
        run = self.run_checker([variable('?beta@@3HA', 0x402004)], self.rows + [('?beta@@3HA', 0x452004)])
        self.assertEqual(run.returncode, 1)
        self.assertFalse(run.stdout)
        self.assertIn('Referenced symbol ?beta@@3HA has multiple MAP addresses', run.stderr)

    def test_function_map_disagreement_is_rejected_before_a_green_verdict(self):
        rows = [('?caller@@YAXXZ', 0x451010), ('?beta@@3HA', 0x452000)]
        run = self.run_checker([variable('?beta@@3HA', 0x402004)], rows)
        self.assertEqual(run.returncode, 1)
        self.assertFalse(run.stdout)
        self.assertIn('metadata address 0x451000 disagrees with MAP address 0x451010', run.stderr)


if __name__ == '__main__':
    unittest.main()
