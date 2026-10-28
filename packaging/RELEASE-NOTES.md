# VectorCraft 0.17.0 for AROS

First AROS release of VectorCraft, a vector drawing program, based on VectorCraft 0.17.0 (main of 2026-10-27, commit 9997ff02).

## Downloads

| File | System |
|---|---|
| `VectorCraft-0.17.0-aros-x86_64-abiv11-pc.zip` | AROS x86_64, ABIv11 (AROS One 1.3 / ABIv11 2026.09) |
| `VectorCraft-0.17.0-aros-x86_64-abiv1-pc.zip` | AROS x86_64, mainline ABIv1 (built against AROS 9802c8c0, 2026-10-28) |
| `VectorCraft-0.17.0-aros-aarch64-abiv1-raspi.zip` | AROS on Raspberry Pi, AArch64 ABIv1 (system image 2026-08-22) |
| `VectorCraft-0.17.0-source.zip` | Port sources, patches and build recipe |

Each package runs only on the system it was built for. `SHA256SUMS` lists the checksums.

## Features

- The VectorCraft editor in one AROS window, with Workbench and Shell start, ASL file requesters, clipboard text and opening links through OpenURL.
- `vectorcraft-cli`, the command-line program, in the same drawer.
- The interface renderer is chosen with the `GUI_RENDERER` ToolType or the `--gui-renderer` Shell argument: `CPU` (default, no OpenGL), `AUTO` (OpenGL if it opens, otherwise CPU, with the reason in `VectorCraft.log`) or `GPU` (OpenGL or an error). OpenGL draws the interface and shows the drawing; documents are always rendered by the processor. Where OpenGL is Mesa's software renderer, CPU is faster, which is why it is the default.

## Installation

Unpack the archive for your system anywhere writable and double-click VectorCraft. From the Shell, run `Stack 16777216` first, then `VectorCraft [--gui-renderer CPU|AUTO|GPU] [file]`. Settings and `VectorCraft.log` are kept in `VectorCraftData` inside the program drawer; to upgrade, unpack the new version to a new place and move `VectorCraftData` into it.

## Known limitations

- One window: floating panels open inside the main window.
- Every change redraws the whole window, so on slow machines and in emulators the interface follows with a delay.
- OpenGL: on AROS One 1.3, ABIv11 2026.09 and mainline it is Mesa's software renderer, slower than CPU; hardware OpenGL drivers have not been tested. On the Raspberry Pi OpenGL is off (not tested on Pi hardware); `VECTORCRAFT_EXPERIMENTAL_GL=1` turns it on at your own risk.
- The Raspberry Pi package has been tested in an emulator only, not on a physical Pi.
- Letters typed with a keymap other than ISO 8859-1 can arrive wrong; file and drawer names must be plain ASCII or UTF-8.
- Clipboard text only. File > Print saves a PDF. No automatic updates. The automation server and plug-ins have not been tested on AROS.
- On mainline AROS, after the first start from Workbench with `GUI_RENDERER=AUTO` since the system started, the VectorCraft drawer can stay in use until a reboot.
- `vectorcraft-cli` cannot write a file at the top of a volume named the AROS way (`RAM:x.png`); use `/RAM/x.png` or a drawer.
- Shell starts need a 16 MiB stack; the Workbench icon sets it.

Licences and notices for VectorCraft and everything built into it are in the `Licenses` drawer of each package.
