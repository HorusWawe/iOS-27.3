# iOS 27.3

An independent ARM64 platform project inspired by the iOS software architecture and user experience, with TECNO KL5n as the first hardware target.

This repository is **not an Android GSI, Android ROM, launcher, or Cuttlefish build**. The old AOSP/GSI/Cuttlefish implementation has been removed.

## First milestone: a real ARM64 boot target

The initial milestone is a UEFI-bootable ARM64 prototype:

- C and GNU-EFI for the first firmware entry point.
- ARM64 / AArch64 target.
- QEMU \`virt\` machine for repeatable boot testing.
- GitHub Actions builds the EFI executable, creates a FAT EFI System Partition image, boots it in QEMU, and uploads the result.
- The prototype draws a basic graphical boot screen through GOP and reports a machine-readable boot marker.

The first image is a **development boot image**, not a flashable KL5n ROM. It proves the ARM64 build and boot path before kernel, runtime, drivers, and phone integration are added.

## Build from GitHub Actions

1. Open the repository's **Actions** tab.
2. Select **iOS 27.3 ARM64 build**.
3. Run the workflow with **Run workflow**, or push a commit to \`main\`.
4. Download the \`ios27.3-arm64-uefi\` artifact from the completed run.

The workflow also runs a QEMU boot smoke test. A successful build must produce the EFI application and pass the boot-marker check.

## Local build on Windows

The intended developer host is Windows. The CI build currently uses Ubuntu runners because the cross-compiler, GNU-EFI and QEMU packages are readily available there. Windows-native build scripts/toolchain support can be added without changing the ARM64 target.

For local reproduction, use the same commands documented in \`.github/workflows/ios27.yml\` inside an Ubuntu environment. No Linux installation is required as the user's main operating system.

## Repository layout

\`\`\`text
.github/workflows/ios27.yml    CI build, QEMU boot test, artifact upload
src/boot/main.c                ARM64 UEFI entry point and GOP boot screen
tools/build.sh                 Cross-compile and create EFI disk image
tools/run-qemu.sh              QEMU boot smoke test
device/tecno/kl5n/README.md    Hardware target notes
docs/ROADMAP.md                Platform milestones and compatibility plan
\`\`\`

## Platform direction

The long-term target is an iOS-compatible platform implementation, not an Android userspace with a reskinned interface. IPA support requires a Mach-O loader, compatible ABI, Objective-C runtime, and implementations of the frameworks used by applications. APK support, if pursued, will be a separate compatibility subsystem. Neither runtime is claimed to exist in this boot prototype.

## TECNO KL5n

Known target: TECNO KL5n, ARM64, MediaTek MT6768, Android 14-era vendor firmware. The QEMU image is only the development target. Booting QEMU does not mean the phone's boot chain, display, touch, modem, camera, or power hardware is supported yet.

See [device notes](device/tecno/kl5n/README.md) and [roadmap](docs/ROADMAP.md).
