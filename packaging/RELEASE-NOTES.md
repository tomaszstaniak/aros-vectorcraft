# VectorCraft 0.17.0-510-g9997ff02, test 4, for AROS

Test 4 is test 3 rebuilt for mainline x86_64 AROS nightly builds of 2026-11-24 or later; the program has no other changes. That nightly changed a system structure that programs are built against: test 3 does not work reliably on it, and test 4 does not run on earlier nightly builds, where test 3 stays the one to use. The ABIv11 and Raspberry Pi packages are not rebuilt and stay at test 3. Checked on the nightly of 2026-11-24 in an emulator: installation and verification with the package manager, a start from Workbench and a new document.

The text below is that of test 3.

First public test release of the AROS port of VectorCraft, a vector drawing program. It is built from VectorCraft's main branch at commit 9997ff02 of 2026-10-27, 510 commits after the 0.17.0 release, so it contains changes that 0.17.0 itself does not have.

## Downloads

| File | System |
|---|---|
| `VectorCraft-0.17.0-510-g9997ff02-test3-aros-x86_64-abiv11-pc.zip` | AROS x86_64, ABIv11 (AROS One 1.3 / ABIv11 2026.09); from the test 3 release |
| `VectorCraft-0.17.0-510-g9997ff02-test4-aros-x86_64-abiv1-pc.zip` | AROS x86_64, mainline ABIv1 (nightly 2026-11-24 or later) |
| `VectorCraft-0.17.0-510-g9997ff02-test3-aros-aarch64-abiv1-raspi.zip` | AROS on Raspberry Pi, AArch64 ABIv1 (system image 2026-08-22); not for hosted Macaros |
| `VectorCraft-0.17.0-510-g9997ff02-test4-source.zip` | Port sources, patches, pinned inputs and build recipe |

Each package runs only on the system it was built for. `SHA256SUMS` lists the checksums. The x86_64 packages can also be installed with arospkg 0.6 (`apkg install vectorcraft`) once the catalogue lists them. The Raspberry Pi package is not in the catalogue: its program is larger than apkg 0.5.0, the newest Pi client, accepts.

## Features

- The VectorCraft editor in one AROS window, with Workbench and Shell start, ASL file requesters, clipboard text and opening links through OpenURL.
- `vectorcraft-cli`, the command-line program, in the same drawer.
- The interface renderer is chosen with the `GUI_RENDERER` ToolType or the `--gui-renderer` Shell argument; the Shell argument wins:
  - `CPU` (default): the processor draws the interface; OpenGL is not opened.
  - `AUTO`: OpenGL when it opens and is not Mesa's software renderer, otherwise CPU. The reason is written to `VectorCraft.log`.
  - `GPU`: OpenGL, including the software renderer, or the program stops with an error.

  OpenGL draws the interface and shows the drawing; documents are always rendered by the processor.

## Installation

Unpack the archive for your system anywhere writable and double-click VectorCraft. From the Shell, run `Stack 16777216` first, then `VectorCraft [--gui-renderer CPU|AUTO|GPU] [file]`. Settings and `VectorCraft.log` are kept in `VectorCraftData` inside the program drawer; to upgrade, unpack the new version to a new place and move `VectorCraftData` into it. The archive also holds a `.arospkg` drawer with the arospkg description; after unpacking by hand it can be deleted.

`VectorCraft --version` and the first line of `VectorCraft.log` name the exact build (`0.17.0-510-g9997ff02-test3`); please include it when you report a problem.

## Known limitations

- Test release: every change redraws the whole window, so on slow machines and in emulators the interface follows with a delay.
- One window: floating panels open inside the main window.
- OpenGL: AROS One 1.3, ABIv11 2026.09 and mainline have only Mesa's software renderer, so `AUTO` uses CPU there. `GPU` works but draws about ten times slower than CPU: in an emulator one redraw took 15 to 30 seconds, so a click or key can take up to a minute to show. Hardware OpenGL drivers have not been tested.
- Raspberry Pi: tested in an emulator only, not on a physical Pi. OpenGL has not been tested on Pi hardware, so `AUTO` uses CPU. `GPU` opens OpenGL; in the emulator (Mesa's software renderer) that stopped the whole system. Save your work before you try it.
- On mainline AROS, the first use of OpenGL (`AUTO` or `GPU`) since the system started leaves the VectorCraft drawer in use until a reboot when the program was started from Workbench or from a Shell inside its drawer. Starts with `CPU` are not affected.
- On ABIv11 2026.09 the drawer once stayed in use until a reboot after a session with CPU, GPU and AUTO starts from it; it did not happen again in later tests. If `Delete` says "object is in use", restart the system and delete it then.
- Letters typed with a keymap other than ISO 8859-1 can arrive wrong; file and drawer names must be plain ASCII or UTF-8.
- Clipboard text only. File > Print saves a PDF. No automatic updates. The automation server and plug-ins have not been tested on AROS.
- `vectorcraft-cli` cannot write a file at the top of a volume named the AROS way (`RAM:x.png`); use `/RAM/x.png` or a drawer.
- Shell starts need a 16 MiB stack; the Workbench icon sets it.
- If VectorCraft stops on an internal error, on ABIv11 it closes its window and keeps your settings. On mainline it shows a "Caught Signal" requester and ends without saving settings; its window stays on screen until you restart the system. The Raspberry Pi build is made the same way and is expected to behave like mainline (not tested there). Save your work often.

Licences and notices for VectorCraft and everything built into it are in the `Licenses` drawer of each package.
