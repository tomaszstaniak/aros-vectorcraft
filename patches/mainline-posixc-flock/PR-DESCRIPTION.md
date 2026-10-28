## Problem

`flock()` in posixc implements a lock as a public exec `SignalSemaphore` named `FLOCK(path)`. Semaphores belong to a task, while flock() locks belong to an open file description, which gives three wrong results:

- A second `open()` of a locked file in the same task gets an exclusive lock again (the semaphore nests for its owner) instead of `EWOULDBLOCK`.
- `LOCK_UN` releases the semaphore whatever the descriptor holds, so unlocking a descriptor that holds no lock removes the lock of another descriptor of the same file (the autodoc listed this as a BUG for other processes).
- Nothing releases a lock when its descriptor is closed or the program ends. The file stays locked until posixc.library is expunged; the semaphore is owned by a task that no longer exists.

Programs that use flock() to detect a second running instance, or to protect a file between processes, see wrong answers. The cases were found with a Rust program (std's `File::lock`/`try_lock` are built on flock()).

## Change

Locks now belong to the open file description (`fcb`), as in BSD and POSIX:

- `flock.c`: a registry, shared by all bases of the library (as the old `file_locks` list was), lists the locked files by the name `NameFromFH()` returns (`NameFromLock()` for directories), with the descriptions holding a lock and the requests waiting for one. A conflicting request from another description fails with `EWOULDBLOCK` under `LOCK_NB` and otherwise waits on a private signal bit, not `SIGF_SINGLE`, which exec's semaphores use. `LOCK_UN` removes only the lock of the given description and does nothing for a description without one. When every conflicting lock was obtained by the calling task, a blocking request returns `EDEADLK`: only that task could release them, and the wait cannot be interrupted, so it would otherwise hang for good. Conversions keep BSD behaviour: a downgrade is immediate; an upgrade is immediate for the only holder, keeps the shared lock and fails with `LOCK_NB`, and otherwise releases the shared lock before waiting.
- `__fdesc.h`: the `fcb` gets the lock fields; `close()` and `closedir()` call `__flock_release()` when the last descriptor of a description is closed. `__exit_fd()` already closes every descriptor when a program closes the library, so program exit releases its locks too.
- `__posixc_intbase.h`: the `file_locks` list is replaced by the registry pointer.
- The autodoc describes the semantics and errors; the old BUG entry is removed.

Tests:

- `developer/debug/test/crt/posixc/flock_description` (new, in `POSIXCTESTFILES`) checks, each case on its own file in `RAM:`: two descriptions in one task (`EWOULDBLOCK` for exclusive and shared `LOCK_NB`, `EDEADLK` for a blocking request); `LOCK_UN` on a descriptor without a lock leaves the other lock in place; a lock survives `close()` while a `dup()`ed descriptor is open and goes with the last one; the lock of a process that exits without unlocking or closing is released; shared locks of two descriptions; upgrade and downgrade. The other process is the test program itself, started through `SystemTags()`. Existing files are refused and only the files it created are deleted.
- `developer/debug/test/crt/posixc/flock` opened its file with `O_CREAT` alone, which posixc rejects with `EINVAL`, so it never reached `flock()`. It now uses `O_RDWR | O_CREAT`; its ten processes then contend for one exclusive lock 500 times, which exercises the blocking path.

## Tests run

pc-x86_64, GCC 10.5 mainline toolchain, posixc.library built from master 67f7b77542 (before) and with this change (after) in a separate build directory, `make compiler-posixc`. Runtime on QEMU (TCG, uniprocessor) with a mainline system installed from AROS 9802c8c0 (no posixc changes between that and 67f7b77542); each library was loaded by path for the test only, one boot per library.

| test | before | after |
|---|---|---|
| flock_description | 9 of 28 checks fail: second description gets EX and SH under LOCK_NB and EX blocking; LOCK_UN on the other descriptor releases the lock; lock kept after the last close; lock kept after the holder exited; three conversion checks | 28 of 28 pass |
| flock (with the O_RDWR fix) | passes | passes |

raspi-aarch64: the posixc.library with this change builds with the Clang 20.1 AROS toolchain (`--with-toolchain=llvm`) and the two tests compile; not run. i386, arm and m68k were not built. Not run on an SMP build or on real hardware.
