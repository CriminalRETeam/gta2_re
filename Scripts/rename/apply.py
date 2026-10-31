"""
Apply the rename maps (maps/*.tsv, old<TAB>new) to a branch: whole identifiers in code and comments,
string literals left alone (ImGui labels and other .rdata text keep their bytes).

Usage (from the repo root):
  python3 Scripts/rename/apply.py                 # Source/*.cpp|hpp|h|inl, test.cpp, test.h, StdAfx.*
  python3 Scripts/rename/apply.py --docs          # also docs/*.md and Scripts/**/*.md
  python3 Scripts/rename/apply.py --map maps/fields.tsv path ...   # chosen maps and files
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


def main():
    args = sys.argv[1:]
    maps, files, docs = [], [], False
    while args:
        a = args.pop(0)
        if a == '--map':
            maps.append(os.path.join(HERE, args.pop(0)) if not os.path.isfile(args[0]) else args.pop(0))
        elif a == '--docs':
            docs = True
        else:
            files.append(a)
    mp = load(maps or sorted(glob.glob(os.path.join(HERE, 'maps', '*.tsv'))))
    if not files:
        files = [f for f in sorted(glob.glob(os.path.join(ROOT, 'Source', '*'))) if f.endswith(('.cpp', '.hpp', '.h', '.inl'))]
        files += [os.path.join(ROOT, f) for f in ('test.cpp', 'test.h', 'StdAfx.cpp', 'StdAfx.h')]
        if docs:
            files += sorted(glob.glob(os.path.join(ROOT, 'docs', '*.md')))
            files += sorted(glob.glob(os.path.join(ROOT, 'Scripts', '**', '*.md'), recursive=True))
    nf = nr = 0
    for p in files:
        if not os.path.isfile(p):
            continue
        t = open(p, errors='surrogateescape').read()
        u, n = rename(t, mp, in_strings=p.endswith('.md'))
        if u != t:
            open(p, 'w', errors='surrogateescape').write(u)
            nf += 1
            nr += n
    print('identifiers in the maps:', len(mp), 'files changed:', nf, 'replacements:', nr)


if __name__ == '__main__':
    main()
