# Reference implementations and KL5n porting notes

This document records what can be reused as engineering reference. It does not claim that a binary built for another phone can boot on KL5n.

## Projects are separate layers

### Project Silicium / Mu-Silicium

Repository: https://github.com/Project-Silicium/Mu-Silicium

Mu-Silicium is a Project Mu-based ARM64 UEFI port for supported Android devices. It is not an iOS port and does not provide an iOS kernel or iOS userland.

Useful reference for this project:

- `Silicon/MediaTek/MT6768Pkg` contains MediaTek MT6768-family platform support.
- `Platforms/Xiaomi/lancelotPkg` is a board-specific Redmi 9 implementation.
- The upstream status page lists Redmi 9 (`lancelot`) under the MediaTek P65/G80 family. Its UEFI display, storage, side-button and SD-card entries are marked working, while Linux boot is not.
- The repository does not list TECNO KL5n as a supported device.

The MT6768 package and lancelot board files are useful source references, not a ready-made KL5n firmware. The board's memory layout, display path, storage wiring, PMIC, GPIO, clocks, touch controller and boot handoff must be independently verified from KL5n data.

### ios-sweet

Repository: https://github.com/dipa-studio/ios-sweet

This is an iOS 16.7.16 userland + XNU 8796 bring-up for Redmi Note 10 Pro (`sweet`, Qualcomm SM7150). Its public documentation describes a boot shim, patched XNU, Apple device tree generation, framebuffer, touch and button drivers. It is a useful end-to-end reference for the XNU/iOS 16 generation, but its Qualcomm addresses and drivers are not KL5n drivers.

### iOS 27 kernel research: ios-fuxi

Repository: https://github.com/bangprovn/ios-fuxi

Its `xnu27/` lane targets XNU 12377.121.6 / iOS 27.0.1 with an arm64e profile, based on work for Xiaomi 13 (`fuxi`, Snapdragon 8 Gen 2 / SM8550) and UniXNU. The repository's own iOS 27 lane notes explicitly state that this lane had not yet run on the device at the time those notes were written; its iOS 27 userland is a separate later milestone. The main Fuxi README still describes the iOS 16.7 userland path.

This is a source-level reference for the newer XNU generation, not a ready-made iOS 27 image for KL5n.

## KL5n bring-up boundary

The current QEMU target is a generic ARM64 UEFI application. It proves only that the EFI image can start under QEMU and draw through GOP.

The phone target is separate. Before selecting a loader or writing a device profile, collect and verify:

1. Exact SoC identity and silicon revision from the live device tree and firmware.
2. The live flattened device tree and reserved-memory map.
3. Bootloader chain and supported temporary boot interface.
4. Display controller, panel timing, framebuffer handoff and backlight.
5. Touch controller and bus topology.
6. GIC, timer, PSCI/SMP and memory map.
7. eMMC/UFS controller, partition layout and DMA constraints.
8. Clocks, regulators, PMIC and required firmware blobs.

No lancelot address, partition offset or driver is copied into the KL5n profile without matching evidence.

## Current implementation boundary

The current EFI application now contains a small interactive visual shell for QEMU testing: welcome page, home grid, placeholder app pages and a software mouse cursor driven by UEFI Simple Pointer when firmware exposes it. This is a boot/UI prototype only. It is not SpringBoard, XNU, Darwin, or an iOS userland.
