# VC6 register allocation (C2.DLL), work in progress

Goal: the rule VC6 (`C2.DLL`, SP4 12.00.8804) uses to give values registers, so near misses
that differ only in register choice (see "Still unexplained" in `docs/matching_quirks.md`) can be
predicted instead of permuted. Same approach as `Scripts/inline_budget/`: find the code, add
logging hooks with a byte patch, then write a model that reproduces the log.

Status: the allocator is located; nothing is hooked or modelled yet.

## Tools

`c2dis.py` disassembles C2.DLL (needs `iced-x86` and `pefile` in the venv):

```bash
python3 Scripts/regalloc/c2dis.py d 1071acdf 40              # disassemble 40 instructions
python3 Scripts/regalloc/c2dis.py x 1078e69f 1078e6ae        # who branches/calls to these addresses
```

## What is known

- C2's asserts pass the source file name, so `E:\8799\vc98\p2\src\P2\<file>.c` strings locate each
  source file. The assert calls sit in cold blocks (`mov edx,line; mov ecx,file; jmp 0x1076AB67`)
  after `0x1078xxxx`; find the hot code with `c2dis.py x <cold block>`.
- Allocator files: `color.c` (string `0x107A97DC`, graph colouring, asserts up to line 5616) and
  `regasg.c` (string `0x107A95CC`, register assignment). `color.c` code is at about
  `0x1071B000-0x10723400`.
- Per-function pass pipeline: `0x107657E2`. It calls each pass in turn with `0x107034AB` between
  them. The inliner (`0x1073B588`) is one of the first passes. The `color.c` phase is the call to
  `0x1071ACDF`, near the middle, after `0x10717684` and `0x10711F93` and before `0x10723B05`.
- `0x1071ACDF` calls, among others, `0x1071B48F`, `0x1071B848`, `0x1071BBC7`, `0x1071B079`,
  `0x1071BC3E` (asserts at `color.c` lines 0x331/0x3F3), `0x107203FD`, `0x10721578`, `0x1072243B`
  and `0x10722650` (0xD66 bytes, asserts at lines 0xE44/0xF17; most likely the select/colour step).
  `0x1072035D` (12 callers) holds the assert at line 0x15F0.
- Tables used in that range: `0x107A09BC` (.rdata, small int table, compared against the current
  value in several loops), `0x10799034` (.bssbe, writable, indexed like a register number: a
  "who holds register r" table is the first guess), `0x107A0494` (.rdata, flag word per entry,
  bit 0x40 tested). Their meaning is not confirmed.

## Next steps

1. Work out C2's register numbering and what `0x10799034` holds (dump it at the end of
   `0x10722650` from a hook on a small test function).
2. Hook the point in `0x10722650` where a value gets its register and log
   (value/symbol, chosen register, order, priority). Reuse the code cave and `logf` from
   `Scripts/inline_budget/patch_c2.py`.
3. Compare the log for `BurgerKing_67F8B0::modify_inputs_4CDF30` (ebx/edi swap) and
   `sound_obj::HandlePedVoiceEvent_423080` against the original's allocation.
