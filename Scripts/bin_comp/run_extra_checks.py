"""
Runs the checks that look past compare_all_functions.py's normalisation and compares what they find with
the known differences in extra_checks_known.txt:

- check_match_refs.py: the same globals, callees and switch targets as 10.5.exe;
- check_match_eh.py: the same EH unwind data (frame slots and destructors of each state);
- check_match_consts.py: the same float constants, strings, vtables, GUIDs, CRT callees and imports;
- check_global_values.py: the same value in every global with an original address after static init.

extra_checks_known.txt lists the differences master still has: known, not fixed yet. A run reports what
differs from it:
- a new difference (a function or global not in the list) fails;
- a changed one (in the list, with other details) fails too, as it can hide a new error in a function that
  was already wrong;
- a fixed one (in the list, no longer found) is reported, to remove from the list in the same change;
- the known ones still present are only counted (--all lists them).
The REVIEWED dict of each check is another thing: differences read and found to do the same as 10.5.exe,
which the check leaves out for good.

An entry is the check, the original address, the name and the check's details with the build's addresses
left out, so moving code elsewhere doesn't change it. Instruction offsets stay: in a MATCH function they
are 10.5.exe's.

Usage (from Scripts/bin_comp, after build.py; `python3 build.py --extra_checks` runs it too):
    python3 run_extra_checks.py [--all] [--update-known] [--known FILE] [--github]
--update-known rewrites the list from this run (review its diff before committing it). --github also
writes GitHub annotations and appends the report to $GITHUB_STEP_SUMMARY.
Exit code 1 when there is a new or changed difference, 2 when a check failed to run.
"""
import argparse
import os
import re
import sys
import traceback

import check_global_values
import check_match_consts
import check_match_eh
import check_match_refs
import ref_values

KNOWN = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'extra_checks_known.txt')
KNOWN_IN_REPO = 'Scripts/bin_comp/extra_checks_known.txt'
KNOWN_HEADER = '''\
# Differences found by run_extra_checks.py that master still has (known, not fixed yet): see its docstring.
# Each entry: the check, the original address and the name, then the details. Remove an entry when you fix it.
# `python3 run_extra_checks.py --update-known` rewrites this file from a run; review the diff.
'''
MAX_ANNOTATIONS = 9  # GitHub shows 10 warnings and 10 notices per step


def refs_check(refs):
    checked, found = check_match_refs.find_differences()
    # check_match_refs.py prints the build address, which moves with any code change
    strip = lambda p: re.sub(r'the build [0-9a-f]+ \(= ', 'the build (= ', p)
    return ['%d MATCH functions checked, %d differ' % (checked, len(found))], \
        [(oa, name, [strip(p) for p in problems]) for name, oa, problems in found]


def eh_check(refs):
    summary, found = check_match_eh.find_differences(refs)
    return [summary], [(d['address'], d['name'], d['details']) for d in found]


def consts_check(refs):
    summary, found = check_match_consts.find_differences(refs)
    return [summary], [(d['address'], d['name'], d['details']) for d in found]


def globals_check(refs):
    summary, found, fails = check_global_values.find_differences(refs)
    out = [(d['address'], d['name'].lstrip('?_').split('@@')[0], d['details']) for d in found]
    if fails['build']:
        out.append((0, 'static initialisers', ['the build\'s %s fails in the emulator: %s' % (refs.new_name(f), e)
                                               for _, f, e in fails['build']]))
    notes = ['10.5.exe\'s initialiser %x fails in the emulator: %s' % (f, e) for _, f, e in fails['10.5.exe']]
    return [summary] + notes, out


CHECKS = [('refs', 'check_match_refs.py', refs_check), ('eh', 'check_match_eh.py', eh_check),
          ('consts', 'check_match_consts.py', consts_check), ('globals', 'check_global_values.py', globals_check)]
ORDER = {kind: n for n, (kind, _, _) in enumerate(CHECKS)}


def run_checks():
    """({(kind, address): (name, details)}, summary lines, {kind: why} of the checks that failed to run)."""
    refs = ref_values.Refs()
    refs.build_addresses = False
    found, lines, errors = {}, [], {}
    for kind, script, check in CHECKS:
        try:
            summary, results = check(refs)
        except Exception:
            traceback.print_exc()
            errors[kind] = '%s failed to run: %s' % (script, traceback.format_exc().strip().splitlines()[-1])
            continue
        lines += ['%s: %s' % (script, s) for s in summary]
        for address, name, details in results:
            found[kind, address] = (name, list(dict.fromkeys(details)))
    return found, lines, errors


def entry_header(key, name):
    return '%s 0x%x %s' % (key[0], key[1], name)


def sorted_keys(entries):
    return sorted(entries, key=lambda k: (ORDER.get(k[0], len(ORDER)), k[1]))


def read_known(path):
    known, key = {}, None
    for n, line in enumerate(open(path, encoding='utf-8'), 1):
        line = line.rstrip('\n')
        if not line.strip() or line.startswith('#'):
            continue
        m = re.fullmatch(r'(\w+) 0x([0-9a-f]+) (.+)', line)
        if m and m.group(1) in ORDER:
            key = (m.group(1), int(m.group(2), 16))
            known[key] = (m.group(3), [])
        elif line.startswith('    ') and key:
            known[key][1].append(line[4:])
        else:
            raise ValueError('%s:%d: not an entry (check 0xaddress name) or an indented detail: %r' % (path, n, line))
    return known


def write_known(path, found):
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.write(KNOWN_HEADER)
        for key in sorted_keys(found):
            name, details = found[key]
            f.write('\n%s\n' % entry_header(key, name))
            f.writelines('    %s\n' % d for d in details)


def entry_text(key, name, details, prefix=''):
    return '\n'.join([prefix + entry_header(key, name)] + [prefix + '    ' + d for d in details])


def changed_lines(old, new):
    return ['- ' + d for d in old if d not in new] + ['+ ' + d for d in new if d not in old]


def compare(found, known, errors):
    new = [k for k in sorted_keys(found) if k not in known]
    fixed = [k for k in sorted_keys(known) if k not in found and k[0] not in errors]
    changed = [k for k in sorted_keys(found) if k in known and found[k][1] != known[k][1]]
    still = [k for k in sorted_keys(found) if k in known and k not in changed]
    return new, changed, fixed, still


def headline(new, changed, fixed, still, errors):
    parts = []
    if new:
        parts.append('%d new difference%s' % (len(new), 's' if len(new) > 1 else ''))
    if changed:
        parts.append('%d changed' % len(changed))
    if not new and not changed:
        parts.append('no new difference')
    if fixed:
        parts.append('%d fixed' % len(fixed))
    if errors:
        parts.append('%d check%s failed to run' % (len(errors), 's' if len(errors) > 1 else ''))
    return '%s (%d known still present)' % (', '.join(parts), len(still))


def print_report(found, known, lines, errors, result, show_all, known_name):
    new, changed, fixed, still = result
    for line in lines + list(errors.values()):
        print(line)
    if new:
        print('\nNew differences (not in %s):' % known_name)
        for k in new:
            print(entry_text(k, *found[k], prefix='  '))
    if changed:
        print('\nChanged known differences (- in %s, + found now):' % known_name)
        for k in changed:
            print(entry_text(k, found[k][0], changed_lines(known[k][1], found[k][1]), prefix='  '))
    if fixed:
        print('\nFixed, remove them from %s (or run --update-known):' % known_name)
        for k in fixed:
            print('  ' + entry_header(k, known[k][0]))
    if show_all and still:
        print('\nKnown differences still present:')
        for k in still:
            print(entry_text(k, *found[k], prefix='  '))
    head = 'Extra checks: ' + headline(*result, errors)
    print('\n' + (head if show_all or not still else head[:-1] + '; --all lists them)'))


def annotation(level, message):
    message = message.replace('%', '%25').replace('\r', '%0D').replace('\n', '%0A')
    print('::%s title=Extra checks::%s' % (level, message))


def one_line(key, name, details):
    more = ' (+%d more)' % (len(details) - 1) if len(details) > 1 else ''
    return '%s %s (0x%x): %s%s' % (key[0], name, key[1], details[0] if details else '', more)


def github_report(found, known, lines, errors, result):
    new, changed, fixed, still = result
    head = headline(*result, errors)
    warnings = ['new difference: ' + one_line(k, *found[k]) for k in new]
    for k in changed:
        now = [d for d in found[k][1] if d not in known[k][1]]
        was = [d for d in known[k][1] if d not in found[k][1]]
        warnings.append('changed known difference: %s (was: %s)' % (one_line(k, found[k][0], now), was[0] if was else '-'))
    warnings += errors.values()
    notices = ['fixed: %s %s (0x%x), remove it from %s' % (k[0], known[k][0], k[1], KNOWN_IN_REPO) for k in fixed]
    if not warnings:
        notices.insert(0, head)
    for level, messages in (('warning', warnings), ('notice', notices)):
        for m in messages[:MAX_ANNOTATIONS]:
            annotation(level, m)
        if len(messages) > MAX_ANNOTATIONS:
            annotation(level, '%d more, see the job summary' % (len(messages) - MAX_ANNOTATIONS))

    path = os.environ.get('GITHUB_STEP_SUMMARY')
    if not path:
        return
    md = ['### Extra checks: ' + head, '']
    if new:
        md += ['#### New differences', '```'] + [entry_text(k, *found[k]) for k in new] + ['```', '']
    if changed:
        md += ['#### Changed known differences (- known, + now)', '```']
        md += [entry_text(k, found[k][0], changed_lines(known[k][1], found[k][1])) for k in changed] + ['```', '']
    if errors:
        md += ['#### Checks that failed to run', '```'] + list(errors.values()) + ['```', '']
    if fixed:
        md += ['#### Fixed: remove them from `%s`' % KNOWN_IN_REPO, '']
        md += ['- `%s`' % entry_header(k, known[k][0]) for k in fixed] + ['']
    if still:
        md += ['<details><summary>%d known differences still present</summary>' % len(still), '', '```']
        md += [entry_text(k, *found[k]) for k in still] + ['```', '</details>', '']
    md += ['<details><summary>Checks run</summary>', '', '```'] + lines + ['```', '</details>', '']
    with open(path, 'a') as f:
        f.write('\n'.join(md) + '\n')


def main():
    parser = argparse.ArgumentParser(description='Run the extra checks and compare them with the known differences.')
    parser.add_argument('--all', action='store_true', help='also list the known differences still present')
    parser.add_argument('--update-known', action='store_true', help='rewrite the known list from this run')
    parser.add_argument('--known', default=KNOWN, help='known differences file (default: %(default)s)')
    parser.add_argument('--github', action='store_true', help='GitHub annotations and $GITHUB_STEP_SUMMARY report')
    args = parser.parse_args()

    try:
        known = read_known(args.known) if os.path.exists(args.known) else {}
    except ValueError as e:
        print(e)
        return 2
    found, lines, errors = run_checks()
    result = compare(found, known, errors)
    print_report(found, known, lines, errors, result, args.all, os.path.basename(args.known))
    if args.github:
        sys.stdout.flush()
        github_report(found, known, lines, errors, result)
    if args.update_known:
        if errors:
            print('Not updating %s: a check failed to run' % args.known)
            return 2
        write_known(args.known, found)
        print('Wrote %d known differences to %s' % (len(found), args.known))
        return 0
    if errors:
        return 2
    return 1 if result[0] or result[1] else 0


if __name__ == '__main__':
    sys.exit(main())
