# Boot and interface assets

Place user-provided graphics in this directory. The firmware currently does not load these files yet; image decoding and filesystem access will be implemented before they are wired into the UI.

Expected resources:

- `apple-logo.png` — genuine Apple silhouette, transparent background; recommended 512x512 or larger.
- `cursor.png` — pointer image, transparent background; recommended 32x32 or 48x48.
- `wallpaper.png` — lock-screen wallpaper; recommended 16:9 or the target display aspect ratio, preferably 1920x1080 or larger.

Keep source images as PNG with alpha where appropriate. Do not commit screenshots of the entire UI as a replacement for these individual assets.
