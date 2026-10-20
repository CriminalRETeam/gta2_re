# UpdateStateMachine_44E560 kit

`CarAI_78::UpdateStateMachine_44E560` matches (see `docs/match_attempts.md`, the UpdateStateMachine_44E560
sections). These tools score it alone and build the patched C2s used to find the last differences.

```bash
python3 Scripts/inline_budget/setup.py              # once: build_vc6/inline_c2
Scripts/usm_44e560/score.sh                         # stock compiler: 0
python3 Scripts/usm_44e560/c2cap.py 0               # build_vc6/inline_c2_cap0 (stock 0x400 limit)
python3 Scripts/usm_44e560/c2pred.py P661 661       # needs keystone-engine
Scripts/usm_44e560/score.sh build_vc6/inline_c2_P661
```

- The source holds reconstructions the match needs: an unused `s32 unused;` in `Ang16::PolarToCartesian_41FC20`
  and `CarPhysics_B0::TurnClockwise_42ABA0` (+34 optimizer entries, 1 scope marker per expansion, 0 nodes; any
  set of helpers giving 34 or 35 works), the two cluster-C spellings at the left alongside site, and the
  `bOvertake` hand-off flag in follow_behind. The compiled-out asserts (in the function and two per `Fix16`
  operator) are part of the match: each costs 2 front-end nodes in the inline budget and nothing else.
- `c2cap.py K`: C2 that tracks 0x400 + K entries per function (the global optimizer limit).
- `c2pred.py NAME LINE`: C2 that leaves the 0 of the compare at IL line `LINE` (relative to the function's first
  line) as an immediate.
