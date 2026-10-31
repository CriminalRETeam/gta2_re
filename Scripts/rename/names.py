"""
Build maps/funcs.tsv and maps/globals.tsv: drop the _ADDRESS suffix from function and global names
(Car::SetFlag_421890 -> Car::SetFlag, gFoo_6F8F88 -> gFoo). The address stays in the MATCH_FUNC/WIP_FUNC/
STUB_FUNC marker or the DEFINE_GLOBAL, so the verification is unaffected.

A base name is only used when the result can't change what a name refers to:
- it isn't a macro, nor a VC98 or GTA2Hax header name given to something other than a class member;
- several entities may share it only if all are members of classes without inheritance between them;
- a plain (unsuffixed) use of it that already exists must be a member of such a class, or a use of one.
Everything else is listed in maps/kept.tsv with the reason, and keeps its suffix.

Usage (from the repo root): python3 Scripts/rename/names.py [--helpers [--comment]]
--helpers also maps unmarked functions named after an address (inlines recovered from 9.6f and the like),
--comment then writes that address in a comment above their definitions.
"""
import collections
import os
import re
import sys

from lex import IDENT, mask

ROOT = os.path.normpath(os.environ.get('RENAME_ROOT') or os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
VC = os.path.join(ROOT, '3rdParty', 'gta2_re_compile_tools', 'VC98', 'Include')
HAX = [os.path.join(ROOT, '3rdParty', 'GTA2Hax', p) for p in ('d3ddll/d3ddll.hpp', 'DmaVideo/DmaVideo.hpp', 'Common/Types.hpp',
                                                           'Common/ddraw_vc6.h', '3rdParty/imgui/imgui.h')]
INC = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)
SUF = re.compile(r'^(\w+?)_([0-9A-Fa-f]{6})$')
IDA = re.compile(r'^(sub|nullsub|j|loc|unknown|byte|word|dword|stru|unk|off|flt|dbl|qword|asc|xmmword|unk)$')
MARK = re.compile(r'\b(?:MATCH|WIP|STUB)_FUNC\s*\(\s*0x([0-9A-Fa-f]+)\s*\)\s*;?')
GDEF = re.compile(r'\b(DEFINE_GLOBAL\w*|DEFINE_STATIC_GLOBAL|EXTERN_GLOBAL\w*)\s*\(\s*([^,]+?)\s*,\s*(\w+)\s*(?:,|\))')
VARIANT = re.compile(r'(_ool|OOL|[Ii]nlined?|Inl|OutOfLine|_local)$|^inline_|_ool_')
# renaming these moves them (or a function's local static, whose mangled name holds the function's) in .bss,
# which changes matched code (bucket order, see docs/matching_quirks.md "Even a global's name can change code")
CODE_CHANGING = {'gCurrCarAI_HasChaseUpdate_677CA8', 'gCharB4_HitByMine_6FDB59', 'gElectroChainDepth_706C94',
                 'kShotgunInnerSpread_707002', 'GetCollisionVolume_413A10', 'GenerateRadioVehicleDescription_426F20',
                 'PoliceRadioMessageGeneration_426790', 'ProcessType2_Frontend_412490',
                 'ProcessType6_CollisionSoundQueue_413760', 'gMessageFont_7062F0'}
KEYW = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'operator', 'defined', 'catch'}


def sources():
    out = {}
    for f in sorted(os.listdir(os.path.join(ROOT, 'Source'))):
        if f.endswith(('.cpp', '.hpp', '.h', '.inl')):
            out['Source/' + f] = open(os.path.join(ROOT, 'Source', f), errors='surrogateescape').read()
    for f in ('test.cpp', 'test.h', 'StdAfx.cpp', 'StdAfx.h'):
        if os.path.isfile(os.path.join(ROOT, f)):
            out[f] = open(os.path.join(ROOT, f), errors='surrogateescape').read()
    return out


def func_name(h):
    """Qualified name declared by a function header or prototype (text before its parameter list)."""
    h = MARK.sub(' ', h)
    h = re.sub(r'\b(EXPORT|__stdcall|__cdecl|__fastcall|inline|static|virtual|explicit|extern)\b', ' ', h)
    m = re.search(r'((?:\w+\s*::\s*)*~?\w+)\s*\(', h)
    if not m or m.group(1).split('::')[-1].strip() in KEYW:
        return None
    return re.sub(r'\s', '', m.group(1))


def system_headers(texts):
    """VC98 headers reachable from Source's includes (ignoring #if), plus the third-party headers Source uses."""
    files = {}
    for dirpath, _, fs in os.walk(VC):
        for f in fs:
            files[os.path.relpath(os.path.join(dirpath, f), VC).lower()] = os.path.join(dirpath, f)
    third = os.path.join(ROOT, 'Source', '3rdParty')
    todo = list(HAX) + [os.path.join(third, f) for f in os.listdir(third) if f.endswith('.h')]
    for t in texts.values():
        for inc in INC.findall(t):
            k = inc.lower().replace('vc98/include/', '')
            if k in files:
                todo.append(files[k])
    seen = set()
    while todo:
        p = todo.pop()
        if p in seen or not os.path.isfile(p):
            continue
        seen.add(p)
        for inc in INC.findall(open(p, errors='replace').read()):
            k = inc.lower().replace('\\', '/')
            if k in files:
                todo.append(files[k])
            elif os.path.isfile(os.path.join(os.path.dirname(p), inc)):
                todo.append(os.path.join(os.path.dirname(p), inc))
    return sorted(seen)


class Inventory:
    def __init__(self, texts):
        self.decl = collections.defaultdict(set)   # name -> {(kind, scope)}: kind method/field/func/var/type
        self.bases = collections.defaultdict(set)  # class -> base classes
        self.bodies = []                            # (file, start, end, owner class or '')
        self.marked = {}                            # qualified name -> (address, file)
        self.gdefs = {}                             # global name -> (address or '', file)
        for f, t in texts.items():
            self.scan(f, t, mask(t))

    def scan(self, f, t, m):
        for g in GDEF.finditer(m):
            a = re.search(r'0x([0-9A-Fa-f]{6})', t[g.end():t.find(';', g.end())]) if 'DEFINE' in g.group(1) else None
            self.gdefs.setdefault(g.group(3), (a.group(1).upper() if a else '', f))
            self.decl[g.group(3)].add(('var', ''))
        for mk in MARK.finditer(m):
            q = func_name(m[mk.end():m.find('{', mk.end())])
            if q:
                self.marked[q] = (mk.group(1).upper(), f)
        stack, st = [], 0
        for i, c in enumerate(m):
            if c == '{':
                h = m[st:i].strip()
                top = stack[-1] if stack else ('ns', '')
                cls = ([''] + [s[1] for s in stack if s[0] == 'class'])[-1]
                if top[0] in ('func', 'init', 'enum'):
                    stack.append(('func' if top[0] == 'func' else 'init', None))
                elif re.search(r'\benum\b', h):
                    stack.append(('enum', None))
                    mm = re.search(r'\benum\s+(\w+)', h)
                    if mm:
                        self.decl[mm.group(1)].add(('type', cls))
                    self.enum_start = i
                elif re.search(r'\bnamespace\b', h) or re.search(r'\bextern\s*$', h):
                    mm = re.search(r'\bnamespace\s+(\w+)', h)
                    if mm:
                        self.decl[mm.group(1)].add(('type', cls))
                    stack.append(('ns', mm.group(1) if mm else None))
                elif (re.search(r'\b(class|struct|union)\b', h) and '=' not in h
                      and ')' not in re.sub(r'__declspec\s*\([^)]*\)', '', h.split(':')[0])):
                    mm = re.search(r'\b(?:class|struct|union)\b\s*(?:__declspec\s*\([^)]*\)\s*)?(\w+)?\s*(?::(.*))?$', h, re.S)
                    name = mm.group(1) if mm and mm.group(1) else '<anon@%s:%d>' % (f, i)
                    if mm and mm.group(1):
                        self.decl[name].add(('type', cls))
                        bl = mm.group(2) or ''
                        while re.search(r'<[^<>]*>', bl):
                            bl = re.sub(r'<[^<>]*>', '', bl)
                        for b in re.findall(r'(\w+)\s*(?:,|$)', bl.strip()):
                            if b not in ('public', 'private', 'protected', 'virtual'):
                                self.bases[name].add(b)
                    stack.append(('class', name))
                elif '=' in h and top[0] != 'func':
                    stack.append(('init', None))
                else:
                    q = func_name(h)
                    owner = cls
                    if q:
                        parts = q.split('::')
                        if len(parts) > 1:
                            owner = parts[-2]
                            self.decl[parts[-1]].add(('method', owner))
                        else:
                            self.decl[q].add(('method', cls) if cls else ('func', ''))
                    stack.append(('func', owner))
                    self.bodies.append([f, i, None, owner])
                st = i + 1
            elif c == '}':
                if stack:
                    k = stack.pop()
                    if k[0] == 'func':
                        for b in reversed(self.bodies):
                            if b[0] == f and b[2] is None:
                                b[2] = i
                                break
                    if k[0] == 'enum':
                        cls = ([''] + [s[1] for s in stack if s[0] == 'class'])[-1]
                        ns = [s[1] for s in stack if s[0] == 'ns' and s[1]]
                        if not cls and ns:
                            cls = 'ns:' + ns[-1]  # only ever used qualified: Source has no using-directives
                        for e in re.findall(r'(?:^|,)\s*(\w+)', m[self.enum_start + 1:i]):
                            self.decl[e].add(('enumerator', cls))
                st = i + 1
            elif c == ';':
                top = stack[-1] if stack else ('ns', '')
                if top[0] in ('class', 'ns'):
                    s = MARK.sub(' ', m[st:i]).strip()
                    cls = top[1] if top[0] == 'class' else ''
                    if s.startswith(('typedef', 'friend', 'using')):
                        mm = re.search(r'(\w+)\s*(?:\[[^\]]*\]\s*)?$', s)
                        if s.startswith('typedef') and mm:
                            self.decl[mm.group(1)].add(('type', cls))
                    elif '(' in s and not GDEF.search(s):
                        q = func_name(s)
                        if q:
                            parts = q.split('::')
                            if cls:
                                self.decl[parts[-1]].add(('method', cls))
                            elif len(parts) > 1:
                                self.decl[parts[-1]].add(('method', parts[-2]))
                            else:
                                self.decl[q].add(('func', ''))
                    elif not GDEF.search(s):
                        for d in re.finditer(r'([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?::\s*\w+\s*)?(?:=[^,]*)?(?=,|$)', s):
                            if d.group(1) not in ('public', 'private', 'protected'):
                                self.decl[d.group(1)].add(('field', cls) if cls else ('var', ''))
                    st = i + 1

    def related(self, a, b):
        def up(c, seen):
            for x in self.bases.get(c, ()):
                if x not in seen:
                    seen.add(x)
                    up(x, seen)
            return seen
        return a == b or b in up(a, set()) or a in up(b, set())


def main():
    helpers = '--helpers' in sys.argv
    texts = sources()
    inv = Inventory(texts)
    macros, system = set(), set()
    for p in system_headers(texts):
        if os.path.isfile(p):
            t = open(p, errors='replace').read()
            system.update(IDENT.findall(t))
            macros.update(re.findall(r'^\s*#\s*define\s+(\w+)', t, re.M))
    for t in texts.values():
        macros.update(re.findall(r'^\s*#\s*define\s+(\w+)', t, re.M))

    # every suffixed identifier and every plain token use, per file position
    toks = collections.defaultdict(list)  # name -> [(file, pos, qualified/member access)]
    for f, t in texts.items():
        m = mask(t)
        for x in IDENT.finditer(m):
            pre = m[max(0, x.start() - 3):x.start()].rstrip()
            toks[x.group(0)].append((f, x.start(), pre.endswith(('::', '.', '->'))))

    entities = {}  # old name -> (kind, scopes, address)
    for name, d in inv.decl.items():
        s = SUF.match(name)
        if not s or IDA.match(s.group(1)):
            continue
        entities[name] = (d, s.group(2).upper())
    marked_names = {q.split('::')[-1]: a for q, (a, f) in inv.marked.items()}
    rows = []  # (old, new, category)
    for old, (d, addr) in sorted(entities.items()):
        base = SUF.match(old).group(1)
        if old in marked_names:
            cat = 'funcs' if marked_names[old] == addr else 'funcs_addr_differs'
        elif old in inv.gdefs:
            cat = 'globals' if inv.gdefs[old][0] in (addr, '') else 'globals_addr_differs'
        elif any(k in ('method', 'func') for k, _ in d):
            cat = 'helpers'
        else:
            cat = 'other'
        rows.append((old, base, cat))

    # inline variants and out-of-line copies keep the address, their only link to the original function
    by_addr = collections.defaultdict(list)
    for q, (a, f) in sorted(inv.marked.items()):
        by_addr[a.upper().zfill(6)].append(q.split('::')[-1])
    for old, base, cat in rows:
        if cat == 'helpers':
            by_addr[entities[old][1]].append(old)
    variants = {}
    for old, base, cat in rows:
        if cat == 'helpers':
            if VARIANT.search(base):
                variants[old] = 'variant name'
            elif len(by_addr[entities[old][1]]) > 1:
                variants[old] = 'address shared with ' + ' '.join(o for o in by_addr[entities[old][1]] if o != old)
    vscopes = collections.defaultdict(set)
    for o in variants:
        vscopes[SUF.match(o).group(1)] |= set(sc for k, sc in entities[o][0])
    for old, base, cat in rows:
        if cat == 'funcs':
            if VARIANT.search(base):
                variants[old] = 'variant name'
            elif vscopes[base] & set(sc for k, sc in entities[old][0]):
                variants[old] = 'variant of ' + ' '.join(o for o in variants if SUF.match(o).group(1) == base)
    use = {'funcs', 'globals'} | ({'helpers'} if helpers else set())
    by_base = collections.defaultdict(list)
    for old, base, cat in rows:
        if cat in use and old not in variants:
            by_base[base].append(old)
    owners = {}
    for b in inv.bodies:
        owners.setdefault(b[0], []).append(b)

    def owner_of(f, pos):
        best = None
        for b in owners.get(f, ()):
            if b[1] < pos and (b[2] is None or pos < b[2]):
                if best is None or b[1] > best[1]:
                    best = b
        return best[3] if best else None

    def members_of(c):
        out, todo = set(), [c]
        while todo:
            x = todo.pop()
            if x in out:
                continue
            out.add(x)
            todo += list(inv.bases.get(x, ()))
        return out

    def decide(base, olds):
        ents = set()
        for o in olds:
            ents |= entities[o][0]
        existing = set(e for e in inv.decl.get(base, set()) if not e[1].startswith('ns:'))
        allents = ents | existing
        nonmember = any(not s for k, s in allents)
        if base in macros:
            return 'macro name'
        if base in system and nonmember:
            return 'VC98 or GTA2Hax name for a non-member'
        if nonmember and len(allents) + len(olds) - len(ents) > 1:
            return 'shared with a non-member: ' + ', '.join(sorted(set(k + ' ' + (s or '::') for k, s in allents)))
        if len(olds) > 1 and len(set(s for k, s in ents)) < len(olds):
            return 'same scope'
        if set(s for k, s in ents) & set(s for k, s in existing):
            return 'same class as an existing ' + base
        classes = sorted(set(s for k, s in allents))
        for i, a in enumerate(classes):
            for b in classes[i + 1:]:
                if inv.related(a, b):
                    return 'inheritance %s / %s' % (a, b)
        if len(classes) < len(allents):
            return 'same class: ' + ', '.join(sorted(k + ' ' + s for k, s in allents))
        mclasses = set(s for k, s in existing if s)
        for f, pos, qual in toks.get(base, ()):
            if qual:
                continue
            o = owner_of(f, pos)
            if o is None and inv.decl.get(base) and all(s for k, s in inv.decl[base]):
                continue  # the declaration of an existing member or namespace-scope name
            if o is None or not (members_of(o) & mclasses):
                return 'plain use in %s' % f
        return None

    cats0 = dict((o, c) for o, b, c in rows)
    mp, kept = {}, []
    for base, olds in sorted(by_base.items()):
        why = decide(base, olds)
        if why == 'same scope':
            sc = collections.Counter(sorted(entities[o][0])[0][1] for o in olds)
            single = [o for o in olds if sc[sorted(entities[o][0])[0][1]] == 1 and len(entities[o][0]) == 1]
            for o in olds:
                if o not in single:
                    kept.append((o, base, 'same scope: ' + ' '.join(x for x in olds if x not in single)))
            olds = single
            why = decide(base, olds) if olds else None
        members = [o for o in olds if all(s for k, s in entities[o][0])]
        if why and members and len(members) < len(olds) and not decide(base, members):
            for o in olds:
                if o not in members:
                    kept.append((o, base, why))
            olds, why = members, None
        for o in olds:
            if why:
                kept.append((o, base, why))
            else:
                mp[o] = base

    for o in CODE_CHANGING:
        if o in mp:
            del mp[o]
            kept.append((o, SUF.match(o).group(1), 'renaming it changes matched code'))
    for o, why in variants.items():
        if helpers or cats0[o] != 'helpers':
            kept.append((o, SUF.match(o).group(1), why))
    for o, b, c in rows:
        if c not in ('funcs', 'globals', 'helpers'):
            kept.append((o, b, 'suffix is not the marker or DEFINE_GLOBAL address' if 'differs' in c else 'not a function or global'))
    cats = dict((o, c) for o, b, c in rows)
    d = os.path.join(ROOT, 'Scripts', 'rename', 'maps')
    for name in ('funcs', 'globals') + (('helpers',) if helpers else ()):
        with open(os.path.join(d, name + '.tsv'), 'w') as out:
            for o in sorted(mp):
                if cats[o] == name:
                    out.write(o + '\t' + mp[o] + '\n')
    with open(os.path.join(d, 'kept.tsv'), 'w') as out:
        for o, b, why in sorted(kept):
            out.write('%s\t%s\t%s\n' % (o, cats[o], why))
    print('suffixed names:', collections.Counter(c for o, b, c in rows))
    print('mapped:', collections.Counter(cats[o] for o in mp))
    print('kept:', collections.Counter(why.split(':')[0].split(' ')[0] for o, b, why in kept))
    if '--comment' in sys.argv:
        comment_helpers(texts, [o for o in mp if cats[o] == 'helpers'])


def comment_helpers(texts, helpers):
    """Unmarked helpers lose the address from their name: keep it in a comment above the definition
    (`// 9.6f 0x4928A0` or `// 10.5 0x5DE4F0`, the build the address belongs to), unless one is already there."""
    import csv
    exe = {}
    for v, f in (('10.5', 'og_function_data_v105.csv'), ('9.6f', 'og_function_data_v96f.csv')):
        for r in csv.reader(open(os.path.join(ROOT, 'Scripts', 'bin_comp', f))):
            if len(r) > 1 and r[1].startswith('0x'):
                exe.setdefault(int(r[1], 16), []).append(v)
    edits = collections.defaultdict(list)  # file -> [(line index, text)]
    masked = dict((f, mask(t)) for f, t in texts.items())
    for old in sorted(helpers):
        addr = old[-6:].upper()
        found = None
        for f, t in texts.items():
            m = masked[f]
            if old not in m:
                continue
            for x in re.finditer(r'\b' + old + r'\s*\(', m):
                ls = m.rfind('\n', 0, x.start()) + 1
                head = m[ls:x.start()]
                if not re.match(r'^\s*(template\s*<[^>]*>\s*)?[A-Za-z_][\w:<>,\s]*[\s*&]+(\w+\s*::\s*)?$', head) \
                        or re.search(r'\b(return|new|delete|else|case)\s', head):
                    continue
                rest = m[x.end():]
                body = rest.find('{') < rest.find(';') if '{' in rest else False
                if found is None or (body and not found[2]):
                    found = (f, t.count('\n', 0, x.start()), body)
        if found is None:
            print('no definition found:', old)
            continue
        f, n, _ = found
        lines = texts[f].split('\n')
        ctx = ' '.join(lines[max(0, n - 3):n + 1])
        if re.search(r'0x0*' + addr.lstrip('0') + r'\b', ctx, re.I):
            continue
        while n > 0 and re.match(r'^\s*(template\b.*|[\w:<>*&\s]+)$', lines[n - 1]) and lines[n - 1].strip() \
                and not lines[n - 1].strip().endswith((';', '{', '}', ':')) and not lines[n - 1].strip().startswith('//'):
            n -= 1
        where = exe.get(int(addr, 16), [])
        tag = where[0] if len(where) == 1 else '9.6f' if '9.6f' in ctx or '96f' in ctx else None
        if tag is None:
            print('address in %s, not commented: %s' % (where or 'neither exe', old))
            continue
        indent = re.match(r'^\s*', lines[n]).group(0)
        edits[f].append((n, indent + '// %s 0x%s' % (tag, addr)))
    for f, es in edits.items():
        lines = texts[f].split('\n')
        for n, c in sorted(es, reverse=True):
            lines.insert(n, c)
        texts[f] = '\n'.join(lines)
        open(os.path.join(ROOT, f), 'w', errors='surrogateescape').write(texts[f])
    print('helpers commented:', sum(len(v) for v in edits.values()))


if __name__ == '__main__':
    main()
