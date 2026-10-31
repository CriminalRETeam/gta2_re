"""Small C++ lexer and class scanner shared by the rename scripts."""
import re

TOKEN = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|\b[A-Za-z_]\w*\b', re.S)
IDENT = re.compile(r'\b[A-Za-z_]\w*\b')


def rename(text, mp, in_strings=False):
    """Replace whole identifiers found in mp, in code and comments; string and char literals are left alone."""
    count = [0]

    def ident(m):
        s = m.group(0)
        if s in mp:
            count[0] += 1
            return mp[s]
        return s

    def tok(m):
        s = m.group(0)
        if s[0] in '"\'' and not in_strings:
            return s
        if s[0] in '/"\'':
            return IDENT.sub(ident, s)
        return ident(m)

    return TOKEN.sub(tok, text), count[0]


def mask(t):
    """Blank comments, literals and preprocessor lines (same length, newlines kept)."""
    out = list(t)
    for m in TOKEN.finditer(t):
        s = m.group(0)
        if s[0] in '/"\'':
            a = m.start() + (1 if s[0] in '"\'' else 0)
            b = m.end() - (1 if s[0] in '"\'' else 0)
            for k in range(a, b):
                if out[k] != '\n':
                    out[k] = ' '
    for m in re.finditer(r'^[ \t]*#(?:[^\n]*\\\n)*[^\n]*', ''.join(out), re.M):
        for k in range(m.start(), m.end()):
            if out[k] != '\n':
                out[k] = ' '
    return ''.join(out)


def class_statements(m):
    """Statements ending in ';' at class scope of masked text m: (class path, start, end)."""
    stack, res, st = [], [], 0
    for i, c in enumerate(m):
        if c == '{':
            h = m[st:i].strip()
            if stack and stack[-1][0] in ('func', 'init', 'enum'):
                kind = 'func' if stack[-1][0] == 'func' else 'init'
            elif re.search(r'\benum\b', h):
                kind = 'enum'
            elif re.search(r'\bnamespace\b', h) or re.search(r'\bextern\s*$', h):
                kind = 'ns'
            elif (re.search(r'\b(class|struct|union)\b', h) and '=' not in h
                  and ')' not in re.sub(r'__declspec\s*\([^)]*\)', '', h.split(':')[0])):
                mm = re.search(r'\b(?:class|struct|union)\b\s*(?:__declspec\s*\([^)]*\)\s*)?(\w+)?', h)
                stack.append(('class', mm.group(1) if mm.group(1) else '<anon@%d>' % i))
                st = i + 1
                continue
            elif '=' in h and stack and stack[-1][0] != 'func':
                kind = 'init'
            else:
                kind = 'func'
            stack.append((kind, None))
            st = i + 1
        elif c == '}':
            if stack:
                stack.pop()
            st = i + 1
        elif c == ';':
            if stack and stack[-1][0] == 'class':
                res.append(('::'.join(s[1] for s in stack if s[0] == 'class'), st, i))
            if not stack or stack[-1][0] in ('class', 'ns'):
                st = i + 1
    return res
