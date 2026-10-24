"""
Runs the checks that look past compare_all_functions.py's normalisation, one after the other:

- check_match_refs.py: the same globals, callees and switch targets as 10.5.exe;
- check_match_eh.py: the same EH unwind data (frame slots and destructors of each state);
- check_match_consts.py: the same float constants, strings, vtables, GUIDs, CRT callees and imports;
- check_global_values.py: the same value in every global with an original address after static init.

Usage (from Scripts/bin_comp, after build.py; `python3 build.py --extra_checks` runs it too):
    python3 run_extra_checks.py
Exit code 1 when any of them reports a difference.
"""
import subprocess
import sys
import time

CHECKS = ['check_match_refs.py', 'check_match_eh.py', 'check_match_consts.py', 'check_global_values.py']


def main():
    failed = []
    for check in CHECKS:
        print('==== %s' % check)
        sys.stdout.flush()
        start = time.time()
        result = subprocess.run([sys.executable, check])
        print('==== %s: %s (%.1f s)' % (check, 'OK' if result.returncode == 0 else 'differences', time.time() - start))
        if result.returncode != 0:
            failed.append(check)
    print('Extra checks with differences: %s' % (', '.join(failed) if failed else 'none'))
    return 1 if failed else 0


if __name__ == '__main__':
    sys.exit(main())
