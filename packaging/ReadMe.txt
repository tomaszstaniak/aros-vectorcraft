VectorCraft @VERSION@ for AROS
================================

VectorCraft is a vector drawing program. This is its AROS port, based on
VectorCraft @UPSTREAM_VERSION@.

This package is for: @SYSTEM@.
It does not run on other AROS systems; use the package made for yours.


Installation
------------

Unpack the archive anywhere writable, for example to Work:. It creates the
drawer VectorCraft and its icon.

To upgrade, unpack the new version to a new place (not over the old drawer),
then move the drawer VectorCraftData from the old VectorCraft drawer into the
new one to keep your settings.

Starting
--------

Workbench: double-click VectorCraft. The icon gives the program the stack it
needs.

Shell:

  Stack 16777216
  VectorCraft [--gui-renderer CPU|AUTO|GPU] [file]

vectorcraft-cli is the command-line program; `vectorcraft-cli --help` lists
its commands. Give it the same stack.

Settings and log
----------------

Settings and the log file VectorCraft.log are kept in the drawer
VectorCraftData inside the VectorCraft drawer. It is created at the first
start.

Interface renderer
------------------

The ToolType GUI_RENDERER in the program's icon, or the Shell argument
--gui-renderer, chooses how the interface is drawn. A Shell argument takes
priority; the icon is not read when the program is started from the Shell.
Values ignore letter case.

  CPU   the processor draws the interface; OpenGL is not opened (default)
  AUTO  OpenGL if it opens, otherwise CPU; the reason is written to
        VectorCraft.log
  GPU   OpenGL, or the program stops with an error

OpenGL draws the interface and shows the drawing. Documents are always
rendered by the processor. On systems where OpenGL is Mesa's software
renderer (softpipe), it is slower than CPU, which is why CPU is the default.

Known limitations
-----------------

TODO(maintainer): list the limitations confirmed on @SYSTEM@.
TODO(maintainer): file formats that do not open or save on AROS, if any.
TODO(maintainer): OpenGL (AUTO/GPU) result on this system.
TODO(maintainer): memory and speed notes for large documents.

Licences
--------

VectorCraft is available under the MIT license or the Apache License 2.0,
at your option. The texts, the notices for fonts, icons and other bundled
material, and the licences of everything linked into the programs are in the
Licenses drawer. BUILD.txt identifies this build.

VectorCraft: https://github.com/storytold/vectorcraft
