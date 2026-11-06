# TECNO KL5n hardware target

This directory records the first physical-device target for iOS 27.3.

| Property | Known value |
|---|---|
| Device | TECNO KL5n |
| CPU ABI | ARM64 / arm64-v8a |
| SoC | MediaTek MT6768 |
| Factory software family | Android 14 / API 34 |
| Vendor API | 31 |
| Treble | Enabled |
| Dynamic partitions | Enabled |
| Virtual A/B | Enabled |

These values describe the existing device firmware and hardware. They do **not** mean the new platform is Android.

## Bring-up work

Before a phone image can be produced, the project needs verified information for:

- Boot chain, bootloader handoff and image signing/verification requirements.
- Exact partition table, slot layout and image formats.
- Device tree and memory map.
- Display controller, touch controller and backlight.
- Interrupt controller, clocks, regulators and power management.
- Storage, USB, audio, camera, sensors, Wi-Fi, Bluetooth and modem.
- Required firmware and vendor hardware interfaces.

No partition sizes, addresses, kernel sources or hardware support are invented here. Record them only after extracting and checking the exact KL5n firmware/device data.

## Current status

The first CI target is generic ARM64 UEFI under QEMU. It is deliberately separate from the phone target. A successful QEMU boot is not evidence of KL5n hardware support.
