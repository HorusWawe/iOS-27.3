# iOS 27.3 platform roadmap

## Stage 0 - reproducible ARM64 boot

- [x] Remove the previous AOSP/GSI/Cuttlefish project files.
- [x] Establish an AArch64 UEFI build target.
- [x] Draw a basic GOP framebuffer boot screen.
- [x] Build a FAT EFI System Partition image in CI.
- [x] Boot the image in QEMU and check a boot marker.
- [ ] Confirm the first clean GitHub Actions run.

## Stage 1 - platform core

- [ ] Define the platform ABI and firmware handoff contract.
- [ ] Add a standalone kernel image and a UEFI loader.
- [ ] Add physical/virtual memory management and exception handling.
- [ ] Add interrupt controller and timer support.
- [ ] Add serial logging, panic handling and a basic test framework.

## Stage 2 - system runtime

- [ ] Define process, thread, IPC and system-call interfaces.
- [ ] Implement a platform libc and filesystem layer.
- [ ] Add storage and input abstractions.
- [ ] Add a compositor, window server and graphics API.
- [ ] Add package metadata and a system service manager.

## Stage 3 - iOS compatibility

- [ ] Specify the Mach-O executable loader.
- [ ] Implement the required Darwin/POSIX ABI surface.
- [ ] Add Objective-C runtime compatibility.
- [ ] Implement framework APIs incrementally, starting with Foundation.
- [ ] Add graphics, text, networking and media framework subsets.
- [ ] Build IPA inspection, install and launch tooling.

IPA compatibility is a long-term runtime and framework project. Reading an IPA archive is not equivalent to executing its application binary.

## Stage 4 - KL5n hardware bring-up

- [ ] Document the exact bootloader chain and partition map.
- [ ] Obtain and inspect device tree and firmware components.
- [ ] Bring up serial, memory, interrupts and timer on the physical device.
- [ ] Bring up display, touch, storage and power management.
- [ ] Add remaining hardware drivers and validate stability.

## Stage 5 - system experience

- [ ] Lock screen, home screen, status bar and control center.
- [ ] System settings and core system applications.
- [ ] Notifications, permissions, accounts and application lifecycle.
- [ ] App installation/update and system recovery.

## Compatibility boundary

Android APK support is not part of the core platform. If added, it will be isolated behind a separate compatibility runtime. It must not turn the system into an Android userspace.
