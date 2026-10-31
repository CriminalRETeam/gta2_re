"""
Re-indent continuation lines after apply.py: a line aligned to an open parenthesis (or to the first
argument after it) in the old tree moves with that column when a name before it got shorter. Whitespace
only, the line count stays.

Usage: python3 Scripts/rename/realign.py <tree before apply.py> <tree after> [--write]
(for example a `git worktree add` of the branch before the rename, and the repo root)
"""
import difflib
import os
import re
import sys

from lex import mask
OLD, NEW = sys.argv[1], sys.argv[2]; WRITE = '--write' in sys.argv
def open_cols(m):
    """columns just after each unmatched '(' of the line (masked), innermost last"""
    st = []
    for i, c in enumerate(m):
        if c == '(':
            st.append(i + 1)
        elif c == ')' and st:
            st.pop()
    return st
tot = files = 0
for f in sorted(os.listdir(os.path.join(NEW, 'Source'))):
    if not f.endswith(('.cpp', '.hpp', '.inl', '.h')) or not os.path.isfile(os.path.join(OLD, 'Source', f)): continue
    a = open(os.path.join(OLD, 'Source', f), errors='surrogateescape').read().split('\n')
    b = open(os.path.join(NEW, 'Source', f), errors='surrogateescape').read().split('\n')
    ma, mb = mask('\n'.join(a)).split('\n'), mask('\n'.join(b)).split('\n')
    # pair lines: same masked shape once identifiers are blanked, skipping lines only one side has
    norm = lambda s: re.sub(r'\b[A-Za-z_]\w*\b', 'X', s).strip()
    sm = difflib.SequenceMatcher(None, [norm(x) for x in ma], [norm(x) for x in mb], autojunk=False)
    pair = {}
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal' or (tag == 'replace' and i2 - i1 == j2 - j1):
            for k in range(i2 - i1): pair[i1 + k] = j1 + k
    changed = 0
    for i in range(len(a) - 1):
        if i not in pair: continue
        ca, cb = open_cols(ma[i]), open_cols(mb[pair[i]])
        if not ca or len(ca) != len(cb) or ca == cb: continue
        # the continuation lines of this line's innermost open group
        ka, kb = ca[-1], cb[-1]
        # first argument on the same line: alignment column is where it starts
        rest_a = ma[i][ka:].lstrip(); rest_b = mb[pair[i]][kb:].lstrip()
        if rest_a:
            ka = ka + len(ma[i][ka:]) - len(ma[i][ka:].lstrip()); kb = kb + len(mb[pair[i]][kb:]) - len(mb[pair[i]][kb:].lstrip())
        j = i + 1
        depth = 0
        while j < len(a) and j in pair:
            la = a[j]; ind = len(la) - len(la.lstrip(' '))
            if not la.strip() or ind != ka: break
            jb = pair[j]; lb = b[jb]
            if lb.strip() != lb.strip() or len(lb) - len(lb.lstrip(' ')) != ka: break
            b[jb] = ' ' * kb + lb.lstrip(' '); changed += 1
            depth += mb[jb].count('(') - mb[jb].count(')')
            if depth < 0 or mb[jb].rstrip().endswith((';', '{')): break
            j += 1
    if changed:
        files += 1; tot += changed
        if WRITE: open(os.path.join(NEW, 'Source', f), 'w', errors='surrogateescape').write('\n'.join(b))
print('files', files, 'lines re-indented', tot)
