## Problem

`strgclass` copies `STRINGA_TextVal` into the gadget's edit buffer with `strcpy()`, both at OM_NEW and at OM_SET. The buffer holds `STRINGA_MaxChars` bytes including the terminating NUL, whether the class allocated it or the caller passed it with `STRINGA_Buffer`. A text of MaxChars characters or more is therefore written past the end of the buffer.

asl.library creates its file requester's pattern, drawer and file gadgets this way; the pattern gadget has MaxChars 257 and gets `ASLFR_InitialPattern` as its initial text. A program that opens a file requester with a longer pattern (for example a long list of file name extensions) overwrites the memory after that buffer. On pc-x86_64 a requester with a 300-character pattern ended in an "Illegal instruction" Software Failure in Kickstart code when it was closed. On the native Raspberry Pi system the next allocation stopped with "TLSF free-list corruption at REMOVE_HEADER", and the damaged block header held bytes of the pattern. This was found with VectorCraft, whose Open pattern lists many formats.

## Change

`rom/intuition/strgclass.c`: a small helper copies the text and cuts it to MaxChars - 1 characters, the same limit `set_buffer_str()` already applies to `STRINGA_LongVal`. OM_NEW and OM_SET use it instead of `strcpy()`. Texts that fit are stored exactly as before. If MaxChars is not positive the buffer is left alone.

New test `developer/debug/test/intuition/strgtextval` (added to the directory's mmakefile): for texts of 0, 5, 15, 16 and 300 characters it creates a gadget with `STRINGA_Buffer`, `STRINGA_MaxChars` 16 and the text as `STRINGA_TextVal`, sets the text again with `SetAttrs()`, and checks that `STRINGA_TextVal` returns the caller's buffer, that the stored length is min(length, 15) and that the guard bytes after the buffer are untouched. The guard area is longer than the longest text, so the unfixed class is detected without damaging other memory.

## Tests run

pc-x86_64, GCC 10.5 mainline toolchain. The base package (`aros-base.pkg`, which contains intuition.library) was built in a separate build directory from master d68c0a784c (before) and with this change (after), and booted through extra GRUB entries on a mainline system installed from 9802c8c0 (the only intuition change between the two is in dogadgetmethoda.c; strgclass.c and asl.library are the same). QEMU, TCG.

| run | strgtextval | ASL requester, 12-character pattern | ASL requester, 300-character pattern |
|---|---|---|---|
| installed system (9802c8c0) | 0, 5, 15 characters pass; 16 and 300 characters overwrite the guard at OM_NEW and OM_SET (FAIL) | file name typed, OK, results returned | Software Failure (Illegal instruction in Kickstart code) after OK |
| base package from master d68c0a784c | same as the installed system | not run | not run |
| base package with this change | all 10 checks pass | file name typed, OK, results returned | file name typed, OK; `fr_Pattern` is the first 256 characters; 2000 allocations afterwards complete |

The test program was also built in the tree (`make test-intuition`) without warnings.

raspi-aarch64: the changed `strgclass.c` compiles without warnings with the Clang 20.1 AROS toolchain against the native Pi SDK (object file only, intuition.library was not linked), and the test compiles. Neither was run with the change. Before the change, the test printed FAIL for the long text on the native Pi system (2026-08-22 image). i386, arm and m68k were not built.
