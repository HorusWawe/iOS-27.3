# Graphics subsystem

Planned modules:

- `gop.h` / `gop.c` — GOP discovery, framebuffer dimensions, pixel format and drawing primitives.
- `image_loader.h` / `image_loader.c` — read image assets from the EFI system partition and decode PNG into GOP-compatible pixels.

The current GOP implementation still lives in `../main.c`. It will be moved here when the interfaces are extracted; this directory is scaffolding for that refactor.
