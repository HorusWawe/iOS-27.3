# iOS 27.3 system integration

This directory is part of the AOSP source build, not an Android application.

Actual interface work belongs in AOSP source packages:
- `frameworks/base/packages/SystemUI`: status bar, lock screen, quick settings and notification shade.
- `frameworks/base/core/res`: framework resources and configuration.
- `packages/apps/Launcher3` or a dedicated system launcher package: home screen and recents integration.
- `packages/apps/Settings`: system settings experience.
- `frameworks/base/services`: platform services and system behavior, only where required.

All changes must be compiled into the system image. A regular APK cannot replace Keyguard or SystemUI.
