# Mainline posixc: flock() locks per open file description

Upstream: aros-development-team/AROS, component `compiler/crt/posixc`.
Base: master `67f7b775421aca4ea8ee609c975ff459ebf69815` (2026-10-28).
Branch: `posixc-flock-per-description`, commit `a265e734459ef4cf2762fd30e6467c5b967603d7`
(fork tomaszstaniak/AROS).

Test logs in `evidence/` show source paths relative to the AROS checkout.

Status: submitted 2026-10-28 as https://github.com/aros-development-team/AROS/pull/1919 (head a265e73445, open).

## Problem

posixc `flock()` was a named exec SignalSemaphore per path (`FLOCK(path)`), so:

1. a second descriptor of the same file in the same task got an exclusive lock
   again (the semaphore nests for its owner);
2. `LOCK_UN` released the semaphore whatever the descriptor held (documented
   BUG; unlocking a descriptor with no lock released another's lock);
3. locks were never released on close or program exit.

Found by rust-aros (`File::lock`/`try_lock`, commit da569a6, which works around
it in `libc-aros/csrc/compat.c` `rs_flock`). Same code in ABIv11; this patch is
for mainline v1 only. No upstream issue or PR mentioned flock (gh search
2026-10-28).

## Change

Lock ownership moves to the fcb (open file description); a registry in the
library base lists locked files, holders and waiters; close()/closedir()
release on the last descriptor, which covers exit (`__exit_fd`). Blocking waits
use a private signal bit; EDEADLK when every conflicting lock was taken by the
calling task. The old `flock` test used `open(path, O_CREAT)` (EINVAL in
posixc) and never reached flock(); fixed to `O_RDWR | O_CREAT`. New test
`flock_description` (28 checks). Details: `PR-DESCRIPTION.md`.

## Tests run

- Build pc-x86_64 (GCC 10.5 mainline toolchain, separate build directory,
  `make compiler-posixc`), before = base, after = commit.
- Runtime: pool slot v1-x, mainline installed from AROS 9802c8c0 (no crt
  difference to the base), QEMU TCG. Library loaded by path for the test only
  (`harness/loadposixc.c`, from mainline-pr-opendir), one boot per library,
  everything in RAM:.
  - before (`evidence/before`): flock_description 9 FAIL / 19 pass (rc 5);
    flock (fixed test) passes.
  - after (`evidence/after`): flock_description 28/28 pass; flock passes
    (10 processes x 50 exclusive locks, blocking path).
  - Input hashes: `evidence/SHA256SUMS-inputs`.
  - The first after-run found a use-after-free in my first version (a woken
    waiter's file record was freed by the releaser); fixed before the commit,
    the runs above are of the committed code.
- raspi-aarch64: posixc.library with the patch built with the Clang 20.1 AROS
  toolchain (`--with-toolchain=llvm`; final link done by hand with the Pi
  toolchain's compiler-rt builtins path); both tests compile with
  `aarch64-aros-clang`. Not run.
- Not built: i386, arm, m68k (32-bit, big-endian). Not run: SMP, hardware.

## Disposition

Submitted: https://github.com/aros-development-team/AROS/pull/1919, revision a265e734459e. Remaining: review; i386/arm/m68k builds and SMP runs not done. The rust-aros `rs_flock` wrapper stays until the fix is in the systems we ship for.
