# VectorCraft for AROS

An AROS port of [VectorCraft](https://github.com/storytold/vectorcraft), a vector drawing program: shapes, paths, pen and brush tools, text, gradients, effects, layers, symbols and artboards, with SVG, PDF, EPS and PNG import and export, and a command-line program for conversions and scripts.

## Download and run

[Download 0.24.1](https://github.com/tomaszstaniak/aros-vectorcraft/releases/tag/v0.24.1-r1) for one of these systems, or install `vectorcraft` with arospkg:

| Package | System |
|---|---|
| `VectorCraft-0.24.1-r1-aros-x86_64-abiv11-pc.zip` | AROS x86_64, ABIv11 distributions |
| `VectorCraft-0.24.1-r1-aros-x86_64-abiv1-pc.zip` | AROS x86_64, mainline ABIv1 (nightly 2026-11-24 or later) |
| `VectorCraft-0.24.1-r1-aros-aarch64-abiv1-raspi.zip` | AROS on Raspberry Pi, AArch64 ABIv1 (nightly 2026-11-24 or later) |

Each package runs only on the system it was built for and includes VectorCraft and `vectorcraft-cli`. Unpack the archive anywhere writable and double-click the VectorCraft icon; from a Shell, run `VectorCraft [file]`. The program takes the stack it needs itself. `ReadMe.txt` in the drawer describes installation, the command line and the interface renderer.

## Known limitations

There is no printing (File > Print saves a PDF), and only text goes through the clipboard to other programs. Changes of the whole window, such as scrolling and zooming, redraw all of it and follow with a delay on slow machines and in emulators. The Raspberry Pi build was tested only in an emulator. The [release notes](packaging/RELEASE-NOTES.md) list the rest.

## Build from source

This repository contains source revisions, AROS patches and build scripts. See [Building](docs/development.md), [Patch workflow](docs/patching.md) and [Source revisions](upstreams.json). The window, clipboard, file requesters and start-up code use [egui-aros](https://github.com/tomaszstaniak/egui-aros), pinned in `upstreams.json`, which also holds the build tools shared by the Craft ports; the Rust standard library for AROS comes from [rust-aros](https://github.com/tomaszstaniak/rust-aros).

The repository holds the build recipe and patches; the source archive of each release adds the pinned upstream inputs. Licenses and author notices are in the source and application archives.

## Licence

VectorCraft is MIT OR Apache-2.0. The port's scripts and patches use the same terms.
