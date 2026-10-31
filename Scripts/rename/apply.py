"""
Apply the rename maps (maps/*.tsv, old<TAB>new) to a branch: whole identifiers in code and comments,
string literals left alone (ImGui labels and other .rdata text keep their bytes).

Usage (from the repo root):
  python3 Scripts/rename/apply.py                 # Source/*.cpp|hpp|h|inl, test.cpp, test.h, StdAfx.*
  python3 Scripts/rename/apply.py --docs          # docs/*.md, *.md and Scripts/**/*.md|py|sh instead (all text)
  python3 Scripts/rename/apply.py --map maps/fields.tsv path ...   # chosen maps and files
  python3 Scripts/rename/apply.py --csv Scripts/bin_comp/og_function_data_v105.csv   # its name column
"""
import glob
import os
import sys

from lex import rename

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, '..', '..'))


def load(paths):
    mp = {}
    for p in paths:
        for line in open(p):
            old, new = line.rstrip('\n').split('\t')
            mp[old] = new
    return mp


def rename_csv(path, mp):
    """Name column only; a row keeps its name when the new one would duplicate another row's."""
    rows = [line.split(',', 1) for line in open(path).read().split('\n') if line]
    new = [rename(r[0], mp)[0] for r in rows]
    count = {}
    for n in new:
        count[n] = count.get(n, 0) + 1
    out = [(n if count[n] == 1 else r[0]) + ',' + r[1] for r, n in zip(rows, new)]
    open(path, 'w').write('\n'.join(out) + '\n')
    print('csv rows renamed:', sum(1 for r, n in zip(rows, new) if n != r[0] and count[n] == 1),
          'kept because the name would repeat:', sum(1 for r, n in zip(rows, new) if n != r[0] and count[n] > 1))


def main():
    args = sys.argv[1:]
    maps, files, docs, csv_path = [], [], False, None
    while args:
        a = args.pop(0)
        if a == '--map':
            maps.append(os.path.join(HERE, args.pop(0)) if not os.path.isfile(args[0]) else args.pop(0))
        elif a == '--docs':
            docs = True
        elif a == '--csv':
            csv_path = args.pop(0)
        else:
            files.append(a)
    mp = load(maps or [p for p in sorted(glob.glob(os.path.join(HERE, 'maps', '*.tsv'))) if not p.endswith('kept.tsv')])
    if csv_path:
        return rename_csv(csv_path, mp)
    if not files:
        files = [f for f in sorted(glob.glob(os.path.join(ROOT, 'Source', '*'))) if f.endswith(('.cpp', '.hpp', '.h', '.inl'))]
        files += [os.path.join(ROOT, f) for f in ('test.cpp', 'test.h', 'StdAfx.cpp', 'StdAfx.h')]
        if docs:
            files = sorted(glob.glob(os.path.join(ROOT, 'docs', '*.md'))) + sorted(glob.glob(os.path.join(ROOT, '*.md')))
            for ext in ('md', 'py', 'sh'):
                files += [f for f in sorted(glob.glob(os.path.join(ROOT, 'Scripts', '**', '*.' + ext), recursive=True))
                          if not f.startswith(HERE)]
    nf = nr = 0
    for p in files:
        if not os.path.isfile(p):
            continue
        t = open(p, errors='surrogateescape').read()
        u, n = rename(t, mp, in_strings=not p.endswith(('.cpp', '.hpp', '.h', '.inl')))
        if u != t:
            open(p, 'w', errors='surrogateescape').write(u)
            nf += 1
            nr += n
    print('identifiers in the maps:', len(mp), 'files changed:', nf, 'replacements:', nr)


if __name__ == '__main__':
    main()
