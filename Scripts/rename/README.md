# Rename maps

`maps/*.tsv` hold the renames that dropped addresses from names: `fields.tsv` (`field_5C_pCar` ->
`mpCar`), `funcs.tsv` (marked functions), `globals.tsv`, `helpers.tsv` (unmarked functions named
after an address, mostly inlines recovered from 9.6f), `scrcmd.tsv` (`ScriptThread::Handle_X`) and
`statics.tsv` (globals that became file, class or function statics). `kept.tsv` lists the names
that keep their `_ADDRESS` suffix and why: clashes, inline variants and out-of-line copies, per-TU
constant copies, and the globals and functions whose name sets a `.bss` slot (their own or a local
static's) that matched code depends on.

To move an open branch over, merge or rebase it onto the rename commits and run, from the repo root,
`python3 Scripts/rename/apply.py` (Source/ and test.cpp), then again with `--docs` (docs/ and Scripts/)
and with `--csv Scripts/bin_comp/og_function_data_v105.csv` (the csv name column). It replaces whole
identifiers in code and comments and leaves string literals alone, so it is safe to run twice. Then
build. A compile error on `mX`/`mFlags` means a renamed name was also a member of a GTA2Hax struct
(`SLight`, `SVideo`, ...): put the `field_` name back there. One on a `s...` name means the branch uses
a global that became a class or function static: qualify it or move the use. `compare_builds.py` must
show no changed function. `realign.py` then re-indents the continuation lines that were aligned to a
parenthesis after a renamed name (whitespace only); it needs a copy of the branch from before
`apply.py`.

`fields.py` and `names.py` built the maps (`names.py` explains the rules); they read the tree, so they
only reproduce the maps on a tree from before the renames.
