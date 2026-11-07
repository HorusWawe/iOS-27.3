# Input subsystem

Planned modules:

- `pointer.h` / `pointer.c` — UEFI Simple Pointer Protocol discovery and relative movement.
- `input.h` / `input.c` — normalized pointer/button events and UI hit testing.

The current prototype polls Simple Pointer directly in `../main.c`. This directory will own that code after extraction. QEMU testing should attach a USB tablet or mouse so pointer movement is available.
