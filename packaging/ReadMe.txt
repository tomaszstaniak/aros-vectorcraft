VectorCraft @VERSION@ for AROS
================================

VectorCraft is a vector drawing program: shapes, paths, pen and brush
tools, text, gradients, effects, layers, symbols and artboards, with import
and export of SVG, PDF, EPS, PNG and other formats. This is its AROS port,
based on VectorCraft @UPSTREAM_VERSION@.

This package is for: @SYSTEM@.
It does not run on other AROS systems; use the package made for yours.

The drawer holds two programs:

  VectorCraft      the drawing program
  vectorcraft-cli  the command-line program: shows a document's details,
                   converts between formats, runs scripts and serves an MCP
                   automation interface over standard input and output

VectorCraft.portable keeps the settings in the program drawer (see Settings
and log); leave it there.

Installation
------------

Unpack the archive anywhere writable, for example to Work:. It creates the
drawer VectorCraft and its icon. Both programs use only libraries that come
with AROS; nothing in the system is replaced or added.

To upgrade, unpack the new version to a new place (not over the old drawer),
then move the drawer VectorCraftData from the old VectorCraft drawer into the
new one to keep your settings, libraries and Data Recovery copies.

Starting
--------

Workbench: double-click VectorCraft. The program takes the stack it needs
itself; it also works from a Shell with the default stack.

Shell:

  VectorCraft [--gui-renderer CPU|AUTO|GPU] [--window WxH] [file ...]

Files named on the command line are opened at start. Open, Save As, Place
and the other file choices use the standard file requester. To close
VectorCraft, use the window's close gadget; with unsaved changes it asks
whether to save them. Unsaved changes are kept for Data Recovery while you
work and offered again after VectorCraft stopped unexpectedly.

  vectorcraft-cli info drawing.svg
  vectorcraft-cli convert drawing.svg drawing.pdf
  vectorcraft-cli convert drawing.svg drawing.png --scale 2
  vectorcraft-cli --help

vectorcraft-cli mcp runs an MCP server on standard input and output for a
client that starts it as a child process. It works on its own documents,
not on the ones open in the VectorCraft window.

Settings and log
----------------

Settings, the user's swatch, style, symbol and brush libraries, Data
Recovery copies and the log files (logs/vectorcraft.log and the two before
it) are kept in the drawer VectorCraftData inside the VectorCraft drawer; it
is created at the first start. Earlier test releases for AROS kept their
settings in the same place, so they are used as they are.

Clipboard
---------

Text is exchanged with other programs through the system clipboard. Within
VectorCraft, Copy and Paste carry whole objects.

Interface renderer
------------------

The ToolType GUI_RENDERER in the program's icon, or the Shell argument
--gui-renderer, chooses how the interface is drawn. A Shell argument takes
priority; the icon is not read when the program is started from the Shell.
Values ignore letter case.

  AUTO  OpenGL if it opens and is not Mesa's software renderer, otherwise
        CPU; the reason is written to the log (default)
  CPU   the processor draws the interface; OpenGL is not opened
  GPU   OpenGL (the software renderer too), or the program stops with an
        error

OpenGL draws the interface and shows the drawing. Documents are always
rendered by the processor.

Known limitations
-----------------

@LIMITATIONS@

Licences
--------

VectorCraft is available under the MIT license or the Apache License 2.0,
at your option. The texts, the notices for fonts, icons and other bundled
material, and the licences of everything linked into the programs are in the
Licenses drawer. BUILD.txt identifies this build.

VectorCraft: https://github.com/storytold/vectorcraft
AROS port: https://github.com/tomaszstaniak/aros-vectorcraft
