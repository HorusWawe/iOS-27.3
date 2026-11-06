# iOS 27.3 product integration

The repository currently has two distinct targets:

- **Cuttlefish development target:** `aosp_cf_x86_64_phone-userdebug`, launched directly from the AOSP build tree for interactive development and testing.
- **TECNO KL5n release target:** Android 14 ARM64 GSI, which remains a separate device-compatibility and packaging task.

The Cuttlefish product identity is defined in `ios27_cuttlefish.mk`. The project does not treat a launcher APK as an operating system. Native SystemUI, Keyguard, framework overlays, boot animation and system apps still need implementation in AOSP.
