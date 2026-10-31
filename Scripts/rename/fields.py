"""
Build maps/fields.tsv: field_XX_name members -> mName, and add the offset comment to their declarations.

  field_5C_pCar -> mpCar                 (a lower-case Hungarian prefix stays: p, b, h, n, s, ...)
  field_92_is_hand_brake_on -> mIsHandBrakeOn
  field_45_unused -> mUnused45           (placeholders keep the offset: unused, pad, padding, bf, ...)
  field_45 -> mField45
Two fields of one class (anonymous unions included) that would get the same name both keep their offset.
With --comment, each declaration line also gets `// 0x5C` (or `// 0x5C: ` before an existing comment).

Usage (from the repo root, on a tree that still has field_ names):
  python3 Scripts/rename/fields.py [--comment]
"""
import collections
import os
import re
import sys

from lex import IDENT, class_statements, mask

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
VC = os.path.join(ROOT, '3rdParty', 'gta2_re_compile_tools', 'VC98', 'Include')
FT = re.compile(r'\bfield_([0-9A-Fa-f]+)(?:_(\w+))?\b')
HUNG = re.compile(r'^(p|pp|b|h|n|ab|an|as|s|str|sz)[A-Z]')
PLACE = re.compile(r'^(unused|pad|padding|bf|not_used|unk)\d*$')
# names already used, but only as members of classes unrelated to the ones that get them
MEMBER_ONLY = {'mValue', 'mSize', 'mStatus', 'mpNext'}


def camel(s):
    parts = [p for p in s.split('_') if p]
    first = parts[0] if HUNG.match(parts[0]) else parts[0][0].upper() + parts[0][1:]
    return first + ''.join(p[0].upper() + p[1:] for p in parts[1:])


def new_name(old, keep_offset):
    m = FT.fullmatch(old)
    off, name = m.group(1).upper(), m.group(2)
    if name is None:
        return 'mField' + off
    if PLACE.match(name):
        base, num = re.match(r'^(.*?)(\d*)$', name).groups()
        return 'm' + camel(base) + off + ('_' + num if keep_offset and num else '')
    return 'm' + camel(name) + (off if keep_offset else '')


def sources():
    out = {}
    for f in sorted(os.listdir(os.path.join(ROOT, 'Source'))):
        if f.endswith(('.cpp', '.hpp', '.h', '.inl')):
            out['Source/' + f] = None
    for f in ('test.cpp', 'test.h', 'StdAfx.cpp', 'StdAfx.h'):
        if os.path.isfile(os.path.join(ROOT, f)):
            out[f] = None
    for f in out:
        out[f] = open(os.path.join(ROOT, f), errors='surrogateescape').read()
    return out


def main():
    texts = sources()
    taken = set()
    for t in texts.values():
        taken.update(IDENT.findall(t))
    for f in os.listdir(VC):
        p = os.path.join(VC, f)
        if os.path.isfile(p):
            taken.update(IDENT.findall(open(p, errors='replace').read()))

    decls = []  # (file, line, scope, old)
    for f, t in texts.items():
        m = mask(t)
        for path, s, e in class_statements(m):
            parts = path.split('::')
            while len(parts) > 1 and parts[-1].startswith('<anon@'):
                parts.pop()
            scope = f + ':' + parts[0] if parts[0].startswith('<anon@') else '::'.join(parts)
            for tok in FT.finditer(m[s:e]):
                decls.append((f, t.count('\n', 0, s + tok.start()) + 1, scope, tok.group(0)))

    scopes = collections.defaultdict(set)
    for f, line, scope, old in decls:
        scopes[scope].add(old)
    clash = set()
    for olds in scopes.values():
        by_new = collections.defaultdict(list)
        for o in olds:
            by_new[new_name(o, False)].append(o)
        for v in by_new.values():
            if len(v) > 1:
                clash.update(v)
    mp = {}
    for o in sorted(set(d[3] for d in decls)):
        n = new_name(o, o in clash)
        if n in taken and n not in MEMBER_ONLY:
            print('skipped, name already used:', o, n)
            continue
        mp[o] = n
    for scope, olds in scopes.items():
        c = collections.Counter(mp.get(o, o) for o in olds)
        for o in olds:
            if c[mp.get(o, o)] > 1:
                sys.exit('two fields of %s get %s' % (scope, mp.get(o, o)))
    with open(os.path.join(ROOT, 'Scripts', 'rename', 'maps', 'fields.tsv'), 'w') as out:
        for k, v in sorted(mp.items()):
            out.write(k + '\t' + v + '\n')
    print('fields:', len(mp), 'keeping the offset:', sum(1 for o in mp if o in clash or not FT.fullmatch(o).group(2)
                                                          or PLACE.match(FT.fullmatch(o).group(2))))
    if '--comment' in sys.argv:
        comment(texts, decls, mp)


def comment(texts, decls, mp):
    by_line = collections.defaultdict(list)
    for f, line, scope, old in decls:
        if old in mp:
            by_line[(f, line)].append(old)
    stats = collections.Counter()
    for f in sorted(set(k[0] for k in by_line)):
        t = texts[f]
        lines, masked = t.split('\n'), mask(t).split('\n')
        for (g, n), olds in sorted(by_line.items()):
            if g != f:
                continue
            tag = ', '.join(sorted(set('0x' + FT.fullmatch(o).group(1).upper() for o in olds)))
            src, code_end = lines[n - 1], len(masked[n - 1].rstrip())
            rest = src[code_end:]
            cm = re.match(r'^(\s*)//\s?(.*)$', rest)
            if not masked[n - 1].rstrip().endswith(';'):
                print('not commented, declaration spans lines: %s:%d' % (f, n))
            elif rest.strip() == '':
                lines[n - 1] = src[:code_end] + ' // ' + tag
                stats['new comment'] += 1
            elif cm and not cm.group(2).upper().startswith(tag.upper()):
                lines[n - 1] = src[:code_end] + cm.group(1) + '// ' + tag + ': ' + cm.group(2)
                stats['before an existing comment'] += 1
            elif not cm:
                print('not commented, other trailing text: %s:%d' % (f, n))
        open(os.path.join(ROOT, f), 'w', errors='surrogateescape').write('\n'.join(lines))
    print(dict(stats))


if __name__ == '__main__':
    main()
