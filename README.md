# iOS 27.3 for TECNO KL5n

An AOSP Android 14 ARM64 Generic System Image project. The deliverable is a native Android `system.img`, not an APK launcher.

## Build target

- AOSP GSI branch: `android14-gsi`
- Product: `ios27_gsi_arm64-userdebug`
- Device path: `generic_arm64`
- Architecture: ARM64, 64-bit Binder
- Output: `out/iOS27-KL5n-GSI-arm64-system.img`

The custom product inherits the upstream `device/generic/common/gsi_arm64.mk` and changes product identity only. It does not patch upstream AOSP makefiles.

## Build host

Use a dedicated Linux x86_64 machine. Allocate at least 350 GB free disk; 64 GB RAM is recommended. The script chooses a conservative parallel job count based on available RAM (maximum 24).

Install the standard AOSP host packages and the Google `repo` launcher, then run:

```bash
bash tools/build-gsi.sh
```

The GitHub Actions workflow is manual and requires a self-hosted runner labelled `aosp` with the same storage and memory capacity. GitHub-hosted runners are not suitable for a full AOSP source checkout and build.

## Device boundary

The GSI build produces the generic `system.img` only. It does not replace KL5n-specific `vendor`, `odm`, `boot`, `init_boot`, `vendor_boot`, `dtbo`, `vbmeta`, or super metadata.

Device facts currently recorded: TECNO KL5n, Android 14/API 34, ARM64, MediaTek MT6768, vendor API 31, Treble and dynamic partitions enabled, Virtual A/B enabled.

A successful generic AOSP build proves that the GSI image was produced. It does not by itself prove KL5n hardware compatibility. VINTF, vendor interfaces, partition capacity, audio, camera, radio, fingerprint, sensors, power HAL and SELinux still require device validation.

## Current implementation status

The product registration and reproducible build entry point are in place. The iOS-style SystemUI, Keyguard, framework overlays and native system apps are separate implementation work and are not falsely represented as completed.
