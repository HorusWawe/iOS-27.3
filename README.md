# iOS 27.3 for TECNO KL5n

An AOSP-based Android 14 ARM64 Generic System Image project for TECNO KL5n, with an iOS-inspired native system interface.

## Project status

The repository is being pivoted from the initial APK prototype to an AOSP-based Android 14 ARM64 GSI. The APK is retained only as a legacy UI prototype and is not the OS deliverable.

## GSI build foundation

- AOSP branch: `android14-gsi`
- Product: `gsi_arm64-userdebug`
- Target device: TECNO KL5n, ARM64, MT6768
- Product integration: `vendor/ios27/ios27_gsi.mk`
- Build entry point: `tools/build-gsi.sh`
- Manual CI: `.github/workflows/gsi.yml` on a self-hosted Linux x86_64 runner

The GSI supplies `system.img`; the device-specific vendor and boot partitions are not replaced.

## Device target

- TECNO KL5n
- Android 14 / API 34
- ARM64
- MediaTek MT6768

## Build

AOSP requires a dedicated Linux x86_64 build host with at least 350 GB free disk and 64 GB RAM recommended. Run `bash tools/build-gsi.sh`. See the root README for details.
