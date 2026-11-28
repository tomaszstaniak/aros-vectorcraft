# VectorCraft 0.24.1 for AROS

The AROS port of VectorCraft 0.24.1, a vector drawing program. It is built from the VectorCraft 0.24.1 release (tag `v0.24.1`), for three AROS systems. This is the first full release of the port; it follows the test releases built from VectorCraft's main branch after 0.17.0.

## Downloads

| File | System |
|---|---|
| `VectorCraft-0.24.1-r1-aros-x86_64-abiv11-pc.zip` | AROS x86_64, ABIv11 distributions |
| `VectorCraft-0.24.1-r1-aros-x86_64-abiv1-pc.zip` | AROS x86_64, mainline ABIv1 (nightly 2026-11-24 or later) |
| `VectorCraft-0.24.1-r1-aros-aarch64-abiv1-raspi.zip` | AROS on Raspberry Pi, AArch64 ABIv1 (nightly 2026-11-24 or later) |
| `VectorCraft-0.24.1-r1-source.zip` | Port sources, patches, pinned inputs and build recipe |

Each package runs only on the system it was built for. `SHA256SUMS` lists the checksums. The packages can also be installed with the AROS package manager: `apkg install vectorcraft`.

## What is new

This release moves from VectorCraft's main branch of October 2026 (test 4) to VectorCraft 0.24.1. For the changes in the program itself, see the release notes of VectorCraft upstream. On the AROS side:

- The window runs on the same engine as the other Craft programs: only the parts that changed are drawn again, so most changes no longer redraw the whole window.
- `GUI_RENDERER` defaults to `AUTO`: OpenGL when it is not Mesa's software renderer, otherwise the CPU.
- The program takes the stack it needs itself; `Stack` is no longer needed before a Shell start.
- Settings, the user's libraries, Data Recovery and the logs stay in `VectorCraftData` in the program drawer, where the test releases kept them, so they are used as they are. The log is now `VectorCraftData/logs/vectorcraft.log`.
- The Raspberry Pi package is built for current mainline nightlies and is in the package catalogue.

The package manager does not upgrade from the test releases: it cannot order their version names (`0.17.0-510-g9997ff02-test3`) against `0.24.1`. If you installed a test release with `apkg`:

1. `apkg remove vectorcraft`. The `VectorCraft` drawer stays, holding `VectorCraftData`.
2. Rename that drawer, for example to `VectorCraft-old`.
3. `apkg install vectorcraft`.
4. Move `VectorCraftData` from `VectorCraft-old` into the new `VectorCraft` drawer, then delete `VectorCraft-old`.

Later releases upgrade with `apkg upgrade vectorcraft`.

## Installation

Unpack the archive for your system anywhere writable and double-click VectorCraft. From a Shell, run `VectorCraft [--gui-renderer CPU|AUTO|GPU] [file]`. To upgrade by hand, unpack the new version to a new place and move `VectorCraftData` into it. Leave `VectorCraft.portable` in the drawer: it keeps the settings there. The archive also holds a `.arospkg` drawer with the arospkg description; after unpacking by hand it can be deleted.

`VectorCraft --version` and the first lines of the log name the build (`0.24.1` and the target); please include them when you report a problem.

## Known limitations

- One window: floating panels open inside the main window, not in windows of their own.
- Changes of the whole window (scrolling, zooming, opening a dialog) redraw all of it; on slow machines and in emulators these follow with a delay. Opening a large file takes a while in an emulator; a progress window shows it.
- Fonts for Japanese, Korean and Chinese are not built in. Text in these scripts is shown as boxes unless a font for it is installed in `FONTS:`.
- The clipboard carries text to and from other programs; pictures and VectorCraft objects stay inside VectorCraft. File > Print saves a PDF.
- No automatic updates: install new versions with the AROS package manager or by unpacking the new archive. The MCP server (`vectorcraft-cli mcp`) has been tested only for its handshake and tool list on AROS; plug-ins and scripts have not been tested.
- ABIv11: AROS One 1.3 has only Mesa's software renderer, so `AUTO` uses the CPU there. `GPU` works with it but draws a frame in seconds.
- Mainline x86_64: `AUTO` uses the CPU, because a program that has opened OpenGL there leaves its log file in use after it ends. `GPU` still opens OpenGL. Hardware OpenGL drivers have not been tested.
- Raspberry Pi: tested in an emulator only. OpenGL has not been tested on Pi hardware, so `AUTO` uses the CPU; save your work before you try `GPU`.

Licences and notices for VectorCraft, the fonts built into it and everything else linked into it are in the `Licenses` drawer of each package.
