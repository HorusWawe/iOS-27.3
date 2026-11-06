#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

QEMU="${QEMU_AARCH64:-qemu-system-aarch64}"
FIRMWARE="${UEFI_FIRMWARE:-}"

if [[ -z "$FIRMWARE" ]]; then
  FIRMWARE="$(find /usr/share -type f \( -name 'QEMU_EFI.fd' -o -name 'AAVMF_CODE.fd' \) -print -quit)"
fi

if [[ -z "$FIRMWARE" || ! -f "$FIRMWARE" ]]; then
  echo "AArch64 UEFI firmware was not found." >&2
  exit 1
fi

if ! command -v "$QEMU" >/dev/null 2>&1; then
  echo "qemu-system-aarch64 is not installed." >&2
  exit 1
fi

LOG="build/qemu-boot.log"
set +e
timeout 18s "$QEMU" \
  -machine virt -cpu cortex-a72 -m 512M \
  -bios "$FIRMWARE" \
  -drive if=none,id=esp,format=raw,file=build/ios27.3-arm64.img \
  -device virtio-blk-device,drive=esp,romfile= \
  -display none -serial stdio -monitor none -no-reboot \
  2>&1 | tee "$LOG"
QEMU_STATUS=${PIPESTATUS[0]}
set -e

if ! grep -q 'IOS27_BOOT_OK' "$LOG"; then
  echo "QEMU did not report the iOS 27.3 boot marker (exit: $QEMU_STATUS)." >&2
  exit 1
fi

echo "QEMU boot smoke test passed."
