# iOS 27.3 for TECNO KL5n

AOSP-based Android 14 project. Development uses **Cuttlefish**, the AOSP virtual Android device, so the source-built OS can be booted and interacted with directly from the build tree instead of exporting and manually flashing a standalone `system.img`.

## Run the OS from source

The development target is:

- AOSP branch: `android14-gsi`
- Cuttlefish product: `aosp_cf_x86_64_phone-userdebug`
- Host: Linux x86_64 with KVM
- Runtime: Cuttlefish + browser-based WebRTC viewer
- Browser UI: `https://localhost:8443`
- Required free disk: at least 400 GB

### One-time host setup

Install the Cuttlefish host packages using the official AOSP procedure:

```bash
sudo apt install -y git devscripts equivs config-package-dev debhelper-compat golang curl
git clone https://github.com/google/android-cuttlefish
cd android-cuttlefish
tools/buildutils/build_packages.sh
sudo apt install -y ./cuttlefish-base_*.deb ./cuttlefish-user_*.deb
sudo usermod -aG kvm,cvdnetwork,render "$USER"
sudo reboot
```

After reboot, install the AOSP host dependencies and the `repo` launcher. The build host needs working `/dev/kvm`.

### Build and boot

From the project checkout:

```bash
AOSP_DIR=/mnt/aosp JOBS=16 bash tools/run-cuttlefish.sh
```

The script syncs Android 14 sources, applies the project product identity, selects the Cuttlefish lunch target, builds the OS and launches it. No standalone GSI artifact is exported. AOSP still generates internal build partitions because Cuttlefish needs them to boot, but the workflow goes straight from source build to running virtual device.

Use the AOSP-built `adb` from `ANDROID_HOST_OUT/bin/adb` for the device shell. Open `https://localhost:8443` on the Linux host to interact with the virtual phone. Stop it with `stop_cvd`.

## Why the emulator target is x86_64

The Cuttlefish host runs on a Linux x86_64 machine with KVM. The virtual Android target is therefore x86_64 for efficient native virtualization. This is the interactive UI/framework development target, not the final KL5n firmware architecture. The release target remains ARM64 and must be validated separately against the device's vendor interfaces and partition layout.

## TECNO KL5n device boundary

The final GSI supplies the generic Android system side only. Device-specific `vendor`, `odm`, `boot`, `init_boot`, `vendor_boot`, `dtbo`, `vbmeta`, and super metadata remain tied to the exact KL5n firmware.

Known target facts: TECNO KL5n, Android 14/API 34, ARM64, MediaTek MT6768, vendor API 31, Treble and dynamic partitions enabled, Virtual A/B enabled.

A successful Cuttlefish boot proves that the source-built virtual target starts. It does not prove KL5n hardware compatibility.

## Current implementation status

The repository now has a source-to-Cuttlefish development path. The iOS-style SystemUI, Keyguard, framework overlays, boot animation and native system apps remain implementation tasks; they are not represented as completed.
