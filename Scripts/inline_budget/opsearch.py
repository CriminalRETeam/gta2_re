"""Scores Fix16 operator sizes against the original's out-of-line operator calls.

OPSEARCH_DIR/base/<TU>.log: inl.sh logs of every TU as it is; OPSEARCH_DIR/alias/<TU>.log: the same with the
named Fix16 calls (Add_408660, Multiply_408680, ...) turned into inline sites with the operators' bodies
(AddAlias, MulAlias, ...). For each MATCH_FUNC in the logs the target is the original's count of calls to
0x408660/0x408680/0x4086A0/0x436A00/0x436A20 (from 10.5.exe) plus the other out-of-line calls of the base
log; score() counts the functions an inlsim run with the given size deltas reproduces.
See docs/matching_quirks.md, "Searching the budget inputs".
"""
import sys,re,glob,csv,collections,pickle,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
import inlsim
import pefile
from iced_x86 import *
S=os.environ.get('OPSEARCH_DIR','opsearch')  # holds base/*.log, alias/*.log (LOG=... inl.sh per TU), data.pkl
ROOT=os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GROUPS={ # group: (substrings in callee name, 10.5 OOL address)
 'ADD':(['Fix16::operator+(class Fix16 const &)','Fix16::AddAlias'],0x408660),
 'MUL':(['Fix16::operator*(class Fix16 const &)','Fix16::MulAlias'],0x408680),
 'NEG':(['Fix16::operator-(void)','Fix16::NegAlias'],0x4086A0),
 'SUB':(['Fix16::operator-(class Fix16 const &)','Fix16::SubAlias'],0x436A00),
 'DIV':(['Fix16::operator/(class Fix16 const &)','Fix16::DivAlias'],0x436A20),
}
def grp(name):
    for g,(subs,_) in GROUPS.items():
        if any(x in name for x in subs): return g
    return None
def parse(path):
    inlsim.last_acc.clear(); inlsim.UDT=set()
    funcs=inlsim.parse(path)
    return funcs,inlsim.catalog(funcs),set(inlsim.UDT)
# matched functions: address -> name
names={int(r[1],16):r[0] for r in csv.reader(open(ROOT+'/Scripts/bin_comp/og_function_data_v105.csv')) if len(r)>3 and r[1].startswith('0x')}
sizes={int(r[1],16):int(r[3],16) for r in csv.reader(open(ROOT+'/Scripts/bin_comp/og_function_data_v105.csv')) if len(r)>3 and r[1].startswith('0x')}
matched={}
for f in glob.glob(ROOT+'/Source/*.cpp'):
    for m in re.finditer(r'MATCH_FUNC\((0x[0-9a-fA-F]+)\)',open(f,errors='ignore').read()):
        a=int(m.group(1),16)
        if a in names: matched[names[a]]=a
pe=pefile.PE(ROOT+'/Scripts/bin_comp/10.5.exe');b=pe.OPTIONAL_HEADER.ImageBase;mem=pe.get_memory_mapped_image()
ADDR2G={v[1]:g for g,v in GROUPS.items()}
def asm_counts(a):
    c=collections.Counter()
    for i in Decoder(32,mem[a-b:a-b+sizes[a]],ip=a):
        if i.mnemonic==Mnemonic.CALL and i.op0_kind==OpKind.NEAR_BRANCH32 and i.near_branch32 in ADDR2G: c[ADDR2G[i.near_branch32]]+=1
    return c
def fname(caller):
    m=re.findall(r'([\w:~]+)\(',caller)
    return m[-1] if m else None
def ool_split(out,udt):
    op=collections.Counter();other=collections.Counter()
    for p,s,d,bb,l in out:
        if d=='INLINE': continue
        g=grp(s.name)
        if g: op[g]+=1
        else: other[s.name]+=1
    return op,other
def build():
    data=[]
    for lp in sorted(glob.glob(S+'/alias/*.log')):
        bp=S+'/base/'+os.path.basename(lp)
        bf,bcat,budt=parse(bp)
        bmap={f.name:f for f in bf}
        af,acat,audt=parse(lp)
        for f in af:
            n=fname(f.name)
            if n not in matched or f.name not in bmap: continue
            inlsim.UDT=budt
            bo=inlsim.Sim(bcat).run(bmap[f.name])
            _,other=ool_split(bo,budt)
            tgt=asm_counts(matched[n])
            data.append((n,f,acat,audt,other,tgt,os.path.basename(lp)))
    return data
def score(data,delta,verbose=False):
    sd={}
    for g,dv in delta.items():
        for sub in GROUPS[g][0]: sd[sub]=dv
    ok=0;bad=[]
    for n,f,cat,udt,other,tgt,tu in data:
        inlsim.UDT=udt
        out=inlsim.Sim(cat,sd).run(f)
        op,oth=ool_split(out,udt)
        if op==+tgt and oth==other: ok+=1
        else: bad.append((n,tu,dict(op),dict(tgt)))
    return ok,bad
if __name__=='__main__':
    if not os.path.exists(S+'/data.pkl'):
        data=build(); pickle.dump(data,open(S+'/data.pkl','wb'))
    data=pickle.load(open(S+'/data.pkl','rb'))
    ok,bad=score(data,{})
    print('functions',len(data),'ok at current sizes',ok)
    for x in bad[:60]: print(x)
