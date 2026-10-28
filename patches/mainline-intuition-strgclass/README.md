# Mainline intuition: bound STRINGA_TextVal by MaxChars

Upstream: aros-development-team/AROS, component `rom/intuition` (strgclass).
Base: master `d68c0a784c79834622cb9069adbaf64e005b1151` (2026-10-28).
Branch: `intuition-strgclass-textval-bound`, commit `b75e72e5ea753c4000d3f7de0651f383e719eac1`
(fork tomaszstaniak/AROS).
`git am` of the patch in `series` onto the base reproduces that tree (checked).

Status: submitted 2026-10-28 as https://github.com/aros-development-team/AROS/pull/1927 (head b75e72e5ea, open).

## Problem

strgclass copied `STRINGA_TextVal` into the edit buffer with `strcpy()` at OM_NEW
and OM_SET without bounding it by MaxChars (buffer size including NUL). ASL's
pattern gadget has MaxChars 257, so `ASLFR_InitialPattern` longer than 256
characters overwrote the heap. Found with VectorCraft's Open pattern (~300
characters): TLSF corruption on the native Pi (2026-08-22), and on mainline
x86_64 a Software Failure (Illegal instruction in Kickstart code) after the
requester closes. Same code in ABIv11; this patch is for mainline v1 only.
Upstream check 2026-10-28: master's strgclass.c unchanged since 2021; no PR or
issue about strgclass, TextVal, MaxChars or ASL patterns (gh search).

## Change

`set_buffer_text()` copies at most MaxChars - 1 characters plus NUL (does
nothing if MaxChars <= 0), as `set_buffer_str()` already does for LongVal.
Used at OM_NEW and OM_SET. STRINGA_LongVal paths were already bounded; the
UndoBuffer `strcpy()` copies from the (now bounded) buffer into a buffer of the
same size. Test `developer/debug/test/intuition/strgtextval`: lengths 0, 5,
15, 16, 300 with MaxChars 16, guard bytes, OM_NEW and OM_SET.

## Tests run

- Build pc-x86_64 (GCC 10.5 mainline toolchain), separate build directory
  (removed afterwards): `make kernel-package-base`
  at the base (before), then at the commit (after; `kernel-intuition-quick`,
  package, compress). `libatomic.a` copied from the main build's SDK (needed by
  m68kemu, not built by the target); acpica built first (missing dependency of
  hidd-pc in a fresh tree). Test also built in-tree (`make test-intuition-quick`), no warnings.
- Runtime: pool slot v1-x, mainline installed from AROS 9802c8c0, QEMU TCG.
  Two non-default GRUB entries added for the test (`harness/strg-entries.cfg`),
  loading `SYS:boot/aros-base-strg-{before,after}.pkg.xz`; boot identity
  recorded by `bootargs` (kernel argument `strgtest=...`). Three boots:
  - stock entry (`evidence/stock`): strgtextval FAIL (16 and 300 characters,
    guard overwritten, rc 20); ASL short pattern OK; ASL 300-character pattern:
    Software Failure after OK (`evidence/stock-asl-long-crash.png`).
  - after (`evidence/after`): strgtextval PASS, 10/10 (rc 0); ASL short and long
    OK, long pattern returned as 256 characters, allocation loop completes.
  - before, same build as after minus the patch (`evidence/before`): same FAIL
    as stock.
  - Input hashes: `evidence/SHA256SUMS-inputs`.
- raspi-aarch64: patched `strgclass.c` compiled (`-c`, Clang 20.1 AROS
  toolchain, Pi SDK headers plus intuition's generated x86_64 libdefs header),
  no warnings; test compiles. Not linked, not run.
- Not built: i386, arm, m68k. Not run: SMP, hardware.

## Changes to v1-x (recorded in the slot notes)

SYS:boot/grub/grub.cfg extended, then restored from the copy (byte-identical
to `evidence/grub.cfg.orig`, date changed); the two test packages and the
backup deleted in boot 3. Nothing else written to SYS:. RESULTS:strg-1028 left.

## Disposition

Submitted: https://github.com/aros-development-team/AROS/pull/1927, revision b75e72e5ea75. Remaining: review; i386/arm/m68k builds, an aarch64 link and run of the patched intuition, and SMP runs not done.
