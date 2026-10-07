import pefile,sys,iced_x86 as ix
import os
p=os.path.join(os.path.dirname(os.path.abspath(__file__)),"..","..","3rdParty","gta2_re_compile_tools","VC98","Bin","C2.DLL")
pe=pefile.PE(p); t=pe.sections[0]; base=pe.OPTIONAL_HEADER.ImageBase
code=t.get_data(); tva=base+t.VirtualAddress
def dis(start,n):
    dec=ix.Decoder(32,code[start-tva:],ip=start); f=ix.Formatter(ix.FormatterSyntax.INTEL)
    for i,ins in enumerate(dec):
        if i>=n: break
        print(f"{ins.ip:08x} {f.format(ins)}")
def xrefs(targets):
    # linear sweep for branch/call targets
    dec=ix.Decoder(32,code,ip=tva); res={}
    for ins in dec:
        if ins.flow_control in (ix.FlowControl.CONDITIONAL_BRANCH,ix.FlowControl.UNCONDITIONAL_BRANCH,ix.FlowControl.CALL):
            if ins.near_branch_target in targets: res.setdefault(ins.near_branch_target,[]).append(ins.ip)
    return res
if __name__=='__main__':
    if sys.argv[1]=='d': dis(int(sys.argv[2],16),int(sys.argv[3]))
    else: print({hex(k):[hex(x) for x in v] for k,v in xrefs({int(a,16) for a in sys.argv[2:]}).items()})
