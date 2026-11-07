# Boot and system UI

Planned modules:

- `boot_screen.h` / `boot_screen.c` — Apple-style startup screen.
- `lock_screen.h` / `lock_screen.c` — dynamic lock screen, clock/date and wallpaper composition.
- `status_bar.h` / `status_bar.c` — status indicators.
- `clock.h` / `clock.c` — time/date formatting from UEFI Runtime Services.
- `progress_bar.h` / `progress_bar.c` — boot progress state and rendering.
- `cursor.h` / `cursor.c` — cursor sprite composition and redraw.

UI elements must be drawn independently; full-screen PNG mockups are not used as the actual interface. Existing prototype drawing code remains in `../main.c` until the module split.
