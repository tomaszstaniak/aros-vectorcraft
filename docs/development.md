# Building VectorCraft for AROS

Targets, named as `<system>-<arch>-<abi>-<platform>`:

| Target | System |
|---|---|
| `aros-x86_64-abiv11-pc` | ABIv11 distributions (AROS One), x86_64 PC |
| `aros-x86_64-abiv1-pc` | mainline AROS, x86_64 PC |
| `aros-aarch64-abiv1-raspi` | native AROS on Raspberry Pi, AArch64 |

A binary built for one target does not run on another. Each target has its own toolchain,
SDK, Cargo home (`deps/<target>/`) and build directory (`build/<target>/`).

## What you need

- Python 3.11 or later, Git, rustup.
- Rust nightly `nightly-2026-09-01` (it reports `rustc 1.100.0-nightly (0dfb098f3 2026-08-31)`)
  with `rust-src`, and a private toolchain alias whose sysroot holds the AROS standard library
  source. The source archive carries that source as `sources/rust-std-*.tar.xz`; install it as
  a new alias with

  ```
  python3 scripts/install-rust-snapshot.py sources/rust-std-<id>.tar.xz /new/sysroot/dir aros-vectorcraft
  ```

  The destination and the alias must be new. All three targets use the same standard library
  source; the source tree hash is recorded in each build manifest.
- Per target, an AROS C toolchain and the SDK it was made for:
  - ABIv11: x86_64 AROS GCC 10.5 and the ABIv11 SDK, plus a deadwood2/AROS checkout that
    contains commit `5376f09bc1ed0c36a12fbb096406013f8a046e89` (its `compiler/pthread` is
    rebuilt with `patches/aros-pthread`).
  - mainline ABIv1: x86_64 AROS GCC 10.5 and an SDK built from mainline AROS (the release was
    built against `9802c8c03c4797946b5a55621a23e2d904c5e126`).
  - Raspberry Pi: the AROS-patched Clang 20.1 for `aarch64-unknown-aros` with compiler-rt, the
    SDK of the Raspberry Pi system image of 2026-08-22, a mainline AROS checkout that contains
    `cc39c62f33d90b842540e79d5e47357590c014ac`, and a pomaros-hosted checkout at
    `3bc9359d439f9fb5655cf48924413ffe670931cc` (the C bridge of the AArch64 standard library).
- The rust-aros files the release used (target templates, profiles, `libc-aros`, the std
  patches): `sources/rust-aros-<commit>.tar.xz` in the source archive, one per rust-aros commit
  in the local files (`rust_aros_commit`). These commits are on a rust-aros branch that is not
  public yet; the archives are the published copy.
- An egui-aros checkout containing the commit pinned in `upstreams.json`, with its upstream
  sources prepared by its own `python3 scripts/ea.py fetch`.

## Configuration

Copy `local.example.json` (an ABIv11 example) to one ignored local file per target and fill in the paths:
`local.json` (ABIv11, the default), for example `local.v1.json` and `local.raspi.json` for the
others, each with its `"target"`. Select a file with `VECTORCRAFT_LOCAL`:

```
VECTORCRAFT_LOCAL=local.v1.json python3 scripts/vc.py config
```

`vc.py config` shows where every setting comes from; environment variables `VECTORCRAFT_*`
override the file. Missing paths are reported, never guessed.

For the Raspberry Pi target, `aros_gcc` is the `aarch64-aros-clang` driver (with `aros_ar` and
`aros_strip` beside it), `aros_src` a mainline checkout and `pomaros_hosted` the pomaros-hosted
checkout. The mainline x86_64 target needs no `aros_src`.

## Steps

1. Fetch the pinned upstreams and create the work copy:

   ```
   python3 scripts/vc.py fetch
   python3 scripts/vc.py materialize
   python3 scripts/vc.py verify
   ```

2. Fetch the locked dependencies into the target's private Cargo home (once per target):

   ```
   VECTORCRAFT_LOCAL=<file> python3 scripts/vc.py deps
   ```

3. Build both programs for each target. The release profile of each target is in
   `packaging/release.json`:

   ```
   python3 scripts/vc.py build --lto fat
   VECTORCRAFT_LOCAL=local.v1.json python3 scripts/vc.py build --lto fat
   VECTORCRAFT_LOCAL=local.raspi.json python3 scripts/vc.py build
   ```

   Add `--online` if Cargo still has to download crates. Each build goes to a new
   `build/<target>/out/<date>-<id>/` with the unstripped binaries, `build.log`,
   `cargo-messages.jsonl` and `manifest.json`. The manifest records the work copy commit, the
   patch series, compiler and SDK hashes, the command, every linked package and the binary
   hashes, and checks that no symbol is left undefined. `--worktree` builds uncommitted files
   too and is recorded as such; such builds cannot be packaged. A build is not a runtime test.

   The build passes `release` from `release.json` to the programs as `VECTORCRAFT_RELEASE`;
   `--version` and the log banner print it, and packaging refuses a build made under another
   release name. Test builds only: `--features panic-test` makes the editor panic in its third
   frame when `VECTORCRAFT_TEST_PANIC` is set, and `VECTORCRAFT_FRAME_LOG=each` at run time logs
   every frame (events, waits, timings; any other value logs averages per 20 frames).

4. Commit this repository, then package each build and the sources. `APKG_PACK` names
   arospkg's `tools/apkg-pack` (or `apkg-pack.pyz`), which writes each binary archive:

   ```
   export APKG_PACK=/path/to/arospkg/tools/apkg-pack
   python3 scripts/package.py binary build/aros-x86_64-abiv11-pc/out/<id>
   python3 scripts/package.py binary build/aros-x86_64-abiv1-pc/out/<id>
   python3 scripts/package.py binary build/aros-aarch64-abiv1-raspi/out/<id>
   python3 scripts/package.py source build/aros-x86_64-abiv11-pc/out/<id> \
       build/aros-x86_64-abiv1-pc/out/<id> build/aros-aarch64-abiv1-raspi/out/<id>
   ```

   `package.py` finds each target's local file by its `target`. It writes
   `dist/VectorCraft-<release>-<target>.zip` and `dist/VectorCraft-<release>-source.zip`
   (`release` in `release.json`) and adds them to `dist/SHA256SUMS`. Binary archives go
   through `apkg-pack check`, `build` and `check` with the target's catalogue description from
   `packaging/catalogue/`; it adds `.arospkg/manifest.toml` (read by `apkg-pack submit`) and
   `VectorCraft/application.json` (read by `apkg run`), and every other member must come out
   byte for byte as staged. It refuses an existing or listed file, a dirty repository, a
   build from a dirty work copy, a profile other than the target's in `release.json`, binaries
   that differ from the manifest and undefined symbols. The programs are stripped with
   `strip --strip-unneeded --remove-section .comment` only: a full strip removes relocations
   AROS applies when it loads a program. The program icon is generated from VectorCraft's
   64x64 application icon with the stack and ToolTypes from `release.json`.

5. Check each archive on the host: unpack it into a new directory, compare the programs with
   the packaged hashes in `VectorCraft/BUILD.txt` and run the target's `nm -u` on them (it
   must print nothing). Then test the unpacked package on the target system itself.

## Source archive

`VectorCraft-<version>-source.zip` holds this repository at the packaged commit and, under
`sources/`, the pinned inputs every packaged build used: VectorCraft's upstream source,
egui-aros, the rust-aros files used (target templates, `libc-aros`), the AROS
`compiler/pthread` sources, the pomaros-hosted bridge, the Rust standard library source and the
crates.io archives of the patched crates. `sources/INDEX.json` lists them with their hashes and,
under `origins`, the repository, commit and licence of each. The rust-aros snapshots carry its
`LICENSE`; the Rust std snapshots carry Rust's `COPYRIGHT-library.html` and licence texts from
the rustup toolchain whose `rustc -V` matches the build.
The rust-aros commits named there (da569a68146b for x86_64, bf47cdfb6008 for the mainline x86_64 build of test 4, 79324c2dbfa0 for the Pi)
are on a branch that is not published; the snapshots in `sources/` are the source of those files.
The storytold egui and winit forks are public Git repositories pinned in `upstreams.json`.

## Host checks

`python3 scripts/vc.py host test -p vectorcraft-aros` runs the AROS program's host tests
(start-up argument and ToolType parsing) with the pinned egui-aros.
