#!/usr/bin/env python3
"""Pretty-print the VC6 inliner log (patched C2.DLL, see patch_c2.py) as a tree.

usage: inltree.py LOG [needle] [-a]
  needle: substring of the top-level function name (default: all functions)
  -a: also show free (size <= 40) sites
"""
import re, sys

args = [a for a in sys.argv[1:] if not a.startswith('-')]
show_all = '-a' in sys.argv
log = open(args[0], errors='replace').read().splitlines()
needle = args[1] if len(args) > 1 else None

def short(n):
    n = re.sub(r'\b(class|struct|static|__thiscall|__stdcall|__cdecl|const)\b', '', n)
    m = re.search(r'([\w:~<>=!+\-*/\[\]]+(?:operator\s*\S+?)?)\s*\(', n)
    return (m.group(1) if m else n).strip()

kv = lambda s: dict(re.findall(r'(\w+)=(-?\w+)', s))

out = []
active = False
pending = None
for ln in log:
    m = re.search(r'([^\\/]+)\((\d+)\) : warning C4711: function \'(INLINE|OUTLINE|SKIP)', ln)
    if m and active and pending is not None and pending.get('shown'):
        pending['src'] = '%s:%s' % (m.group(1), m.group(2))
        p = pending
        out[p['idx']] = out[p['idx']].replace('@@', '%-24s' % p['src'])
        continue
    if not ln.startswith('@I '):
        continue
    body = ln[3:]
    kind = body.split()[0]
    if kind == 'ENTER':
        d = kv(body)
        caller = re.search(r'caller=(.*) callersize=', body).group(1)
        if d['depth'] == '1':
            active = needle is None or needle in caller
            if active:
                out.append('')
                out.append('%s  size=%s budget=%s' % (caller, d['callersize'], d['budget']))
        elif active and pending is not None:
            pending['nb'] = d['budget']
    elif not active:
        continue
    elif kind == 'SITE':
        d = kv(body)
        callee = re.search(r'callee=(.*) size=', body).group(1)
        pending = dict(d=int(d['depth']), name=short(callee), size=int(d['size']),
                       budget=int(d['budget']), count=int(d['count']), force=int(d['flags73'], 16) & 0x2000,
                       argok=d['argok'], line=d['line'])
    elif kind in ('ACCEPT', 'REJECT', 'SKIP'):
        p = pending
        free = p['size'] <= 40 or p['force']
        if kind == 'ACCEPT':
            tag = 'force' if p['force'] else ('free' if p['size'] <= 40 else 'INLINE')
        elif kind == 'SKIP':
            tag = 'SKIP(argctor)'
        else:
            tag = 'OUT-OF-LINE'
        p['tag'] = tag
        if kind == 'REJECT' or not free or show_all:
            p['idx'] = len(out)
            out.append(None)
            p['shown'] = True
        else:
            p['shown'] = False
        p['nb'] = None
        stack_top = p
        pending = p
    elif kind == 'NESTED':
        pass
    # render lazily
    if kind in ('ACCEPT', 'REJECT', 'SKIP', 'ENTER') and pending is not None and pending.get('shown'):
        p = pending
        cost = '' if p['tag'] != 'INLINE' else ' -> %d' % (p['budget'] - p['size'])
        nb = '' if p.get('nb') is None else '  (nested budget %s)' % p['nb']
        out[p['idx']] = ('@@ ' if not p.get('src') else '%-24s ' % p['src']) + '%s%-12s %-40s size=%-4d budget=%-5d sites_left=%-3d%s%s' % (
            '  ' * p['d'], p['tag'], p['name'], p['size'], p['budget'], p['count'], cost, nb)
print('\n'.join(o.replace('@@', ' '*24) for o in out if o is not None))
