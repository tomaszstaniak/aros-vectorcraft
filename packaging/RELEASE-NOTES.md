# VectorCraft 0.17.0-510-g9997ff02, test 3, for AROS

First public test release of the AROS port of VectorCraft, a vector drawing program. It is built from VectorCraft's main branch at commit 9997ff02 of 2026-10-27, 510 commits after the 0.17.0 release, so it contains changes that 0.17.0 itself does not have.

## Downloads

| File | System |
|---|---|
| `VectorCraft-0.17.0-510-g9997ff02-test3-aros-x86_64-abiv11-pc.zip` | AROS x86_64, ABIv11 (AROS One 1.3 / ABIv11 2026.09) |
| `VectorCraft-0.17.0-510-g9997ff02-test3-aros-x86_64-abiv1-pc.zip` | AROS x86_64, mainline ABIv1 (built against AROS 9802c8c0, 2026-10-28) |
| `VectorCraft-0.17.0-510-g9997ff02-test3-aros-aarch64-abiv1-raspi.zip` | AROS on Raspberry Pi, AArch64 ABIv1 (system image 2026-08-22); not for hosted Macaros |
| `VectorCraft-0.17.0-510-g9997ff02-test3-source.zip` | Port sources, patches, pinned inputs and build recipe |

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
- Letters typed with a keymap other than ISO 8859-1 can arrive wrong; file and drawer names must be plain ASCII or UTF-8.
- Clipboard text only. File > Print saves a PDF. No automatic updates. The automation server and plug-ins have not been tested on AROS.
- `vectorcraft-cli` cannot write a file at the top of a volume named the AROS way (`RAM:x.png`); use `/RAM/x.png` or a drawer.
- Shell starts need a 16 MiB stack; the Workbench icon sets it.
- If VectorCraft stops on an internal error, on ABIv11 it closes its window and keeps your settings; on mainline and the Raspberry Pi it ends at once. Save your work often.

Licences and notices for VectorCraft and everything built into it are in the `Licenses` drawer of each package.
