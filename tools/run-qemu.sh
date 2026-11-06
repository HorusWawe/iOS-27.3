#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

QEMU="${QEMU_AARCH64:-qemu-system-aarch64}"
FIRMWARE="${UEFI_FIRMWARE:-}"

if [[ -z "$FIRMWARE" ]]; then
  FIRMWARE="$(find /usr/share -type f -name 'QEMU_EFI.fd' -print -quit)"
fi
if [[ -z "$FIRMWARE" ]]; then
  FIRMWARE="$(find /usr/share -type f -name 'AAVMF_CODE.fd' -print -quit)"
fi

if [[ -z "$FIRMWARE" || ! -f "$FIRMWARE" ]]; then
  echo "AArch64 UEFI firmware was not found." >&2
  exit 1
fi
if ! command -v "$QEMU" >/dev/null 2>&1; then
  echo "qemu-system-aarch64 is not installed." >&2
  exit 1
fi

VIRTIO_ROM="$(find /usr/share /usr/lib -type f -name 'efi-virtio.rom' -print -quit)"
if [[ -z "$VIRTIO_ROM" ]]; then
  echo "efi-virtio.rom was not found. Ensure the ipxe-qemu package is installed." >&2
  exit 1
fi
ROM_DIR="$(dirname "$VIRTIO_ROM")"
echo "Using QEMU ROM directory: $ROM_DIR"
echo "Using UEFI firmware: $FIRMWARE"

LOG="build/qemu-boot.log"
set +e
timeout 18s "$QEMU" -L "$ROM_DIR" -machine virt -cpu cortex-a72 -m 512M -bios "$FIRMWARE" -device ramfb -device qemu-xhci -device usb-mouse -device usb-kbd -drive if=none,id=esp,format=raw,file=build/ios27.3-arm64.img -device virtio-blk-device,drive=esp -display none -serial stdio -monitor none -no-reboot 2>&1 | tee "$LOG"
QEMU_STATUS=${PIPESTATUS[0]}
set -e

if ! grep -q 'IOS27_BOOT_OK' "$LOG"; then
  echo "QEMU did not report the iOS 27.3 boot marker (exit: $QEMU_STATUS)." >&2
  exit 1
fi
if ! grep -q 'GOP_READY' "$LOG"; then
  echo "EFI started, but GOP was not available. Check the QEMU display device." >&2
  exit 1
fi

if grep -q 'POINTER_READY' "$LOG"; then
  echo "QEMU USB pointer protocol detected."
else
  echo "::warning::QEMU booted, but UEFI did not expose Simple Pointer. Check USB input support."
fi

if ! grep -q 'INTERACTIVE_UI_READY' "$LOG"; then
  echo "Interactive boot UI did not initialize." >&2
  exit 1
fi

echo "QEMU boot, GOP and interactive UI smoke tests passed."
