"""sv.py LOG ASM [function substring] [-r]

The VC6 scheduler's decisions for the matching functions, per window, from a sched.sh log and its /FAs
listing. Columns: cycle, seq (pre-schedule IL order), priority (latency-weighted height . low flag
bits), earliest cycle, instruction (from the listing; `nop-node` for op 354, which emits nothing), and
successors as s<seq>/<latency>. -r also prints the ready list before each cycle as
s<seq>:<prio>/e<earliest>. The highest priority ready node goes first, ties in seq order; an x87
instruction issues alone, two integer ones can pair in one cycle.
"""
import os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ops = {354: 'nop-node'}
for l in open(os.path.join(HERE, 'optab.txt')):
    i, _, n = l.split()
    ops[int(i)] = n


def listing(asmf):
    """function name -> [(mnemonic, text)] from a /FAs listing."""
    funcs, cur = {}, None
    for l in open(asmf, errors='replace'):
        l = l.rstrip('\n')
        if ' PROC ' in l:
            cur = []
            funcs.setdefault(l.split(';')[-1].split(',')[0].strip(), cur)
            continue
        if ' ENDP' in l:
            cur = None
            continue
        if cur is None:
            continue
        m = re.match(r'^\t([a-z][a-z0-9]*)\b(.*)', l)
        if m:
            cur.append((m.group(1), re.sub(r'\s+', ' ', l.strip().split(';')[0]).strip()))
    return funcs


def fields(e):
    return dict(re.findall(r'(\w+)=([0-9a-f]+)', e.split('succ:')[0]))


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('-')]
    showready = '-r' in sys.argv
    log, asmf = args[0], args[1]
    filt = args[2:]
    fd = listing(asmf)
    F = []
    for l in open(log, errors='replace'):
        if l.startswith('@F '):
            F.append({'name': l[3:].strip(), 'ev': [], 'E': []})
        elif l.startswith('@E ') and F:
            F[-1]['E'].append((l.split()[1], int(l.split('op=')[1])))
        elif l.startswith(('@S', '@R', '@L')) and F:
            F[-1]['ev'].append(l.strip())
    for f in F:
        if filt and not any(s in f['name'] for s in filt):
            continue
        best = None
        for n in fd:
            if n and (n + '(') in f['name'] and (best is None or len(n) > len(best)):
                best = n
        lst = fd.get(best, [])
        # align the emitted instructions with the listing
        text, j = {}, 0
        for a, op in f['E']:
            n = ops.get(op, '?')
            k = j
            while k < len(lst) and lst[k][0] != n and not (n.startswith('j') and lst[k][0].startswith('j')):
                k += 1
            if k < len(lst) and k - j <= 3:
                text[a] = lst[k][1]
                j = k + 1
        print('=' * 20, f['name'])
        seqof = {}
        for e in f['ev']:
            if e.startswith('@S'):
                d = fields(e)
                seqof[d['node']] = int(d['seq'])
        win = 0
        for e in f['ev']:
            if e.startswith('@L'):
                win += 1
                w = e.split()[1:]
                if len(w) >= 3:
                    print('-' * 10, 'window %d: %d nodes, %d nop' % (win, len(w), sum(x.startswith('354/') for x in w)))
                continue
            if e.startswith('@R'):
                if showready:
                    c = int(re.search(r'cyc=(\d+)', e).group(1))
                    r = re.findall(r'([0-9a-f]{8})\(op=(\d+),p=(\d+),e=(\d+),s=(\d+)\)', e)
                    print('   ready@%d: ' % c + ' '.join('s%s:%d.%d/e%s' % (s, int(p) >> 12, int(p) & 4095, ee)
                                                     for nd, o, p, ee, s in r))
                continue
            d = fields(e)
            succ = re.findall(r'([0-9a-f]{8})/(\d+)', e.split('succ:')[1])
            pr = int(d['prio'])
            t = text.get(d['ins'], ops.get(int(d['op']), d['op']))
            ss = ' '.join('s%s/%s' % (seqof[n], lat) for n, lat in succ if n in seqof)
            print('%3s s%-3s p%4d.%-4d e%-3s %-46s -> %s' % (d['cyc'], d['seq'], pr >> 12, pr & 4095, d['earliest'],
                                                            t[:46], ss))


if __name__ == '__main__':
    main()
