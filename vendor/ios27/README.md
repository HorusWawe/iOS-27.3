# iOS 27.3 product integration

The AOSP build imports `ios27_gsi_arm64.mk` through `AndroidProducts.mk` and selects the dedicated lunch target:

`ios27_gsi_arm64-userdebug`

The product inherits the official Android 14 ARM64 GSI configuration from `device/generic/common/gsi_arm64.mk`. The resulting output remains the standard generic ARM64 system image at `out/target/product/generic_arm64/system.img`.

No Android APK, launcher-only package, or Magisk module is used as the operating-system deliverable.
