# Patch workflow

`upstream/vectorcraft/` is the clean upstream checkout at the commit pinned in
`upstreams.json`. Never edit it. `work/vectorcraft/` is the editable copy, created from the
pin and `patches/vectorcraft/series` by `python3 scripts/vc.py materialize`; it is never
recreated or reset by the tools.

To change VectorCraft for AROS:

1. Commit the change in `work/vectorcraft/`, one commit per logical change. The commit message
   becomes the patch description: say what the change does on AROS, why, which targets it
   affects and how it was checked.
2. `python3 scripts/vc.py export` writes the work copy's commits since the pin into
   `patches/vectorcraft/` and checks that the new series rebuilds the work copy's tree before
   it replaces anything.
3. Review `git diff patches/`, then commit the patches and the series in this repository.
4. `python3 scripts/vc.py verify` rebuilds the series in a temporary directory and compares
   it with the work copy.

Other external sources that need changes have their own series:

- `patches/crates/`: crates.io crates built from a patched copy. `crates.json` records each
  crate's registry checksum and the tree hash of the reviewed result; `vc.py crates` prepares
  them under `build/<target>/crates/`, never in the Cargo registry.
- `patches/aros-pthread/`: the change to AROS `compiler/pthread` linked into the ABIv11 and
  Raspberry Pi programs (a thread detached after it finished no longer blocks the exit).
- `patches/libc-aros/`: changes to rust-aros's `libc-aros` (currently none).

Do not edit Cargo registry sources, a shared toolchain or an SDK. Preserve local commits,
stashes and untracked files before moving a work copy aside; `verify` compares committed
trees and is not a backup.
