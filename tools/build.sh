#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

CLANG="${CLANG:-clang}"
LLD_LINK="${LLD_LINK:-lld-link}"

mkdir -p build

"$CLANG" \
  --target=aarch64-pc-windows-msvc \
  -std=c11 -O2 \
  -ffreestanding -fshort-wchar -fno-stack-protector \
  -fno-builtin -fno-exceptions -fno-unwind-tables \
  -fno-asynchronous-unwind-tables \
  -Wall -Wextra -Werror \
  -c src/boot/main.c -o build/main.obj

"$LLD_LINK" \
  /subsystem:efi_application \
  /entry:efi_main \
  /nodefaultlib \
  /machine:arm64 \
  /out:build/BOOTAA64.EFI \
  build/main.obj

if ! file build/BOOTAA64.EFI | grep -qi 'PE32+.*Aarch64'; then
  echo "Output is not an AArch64 PE32+ EFI application." >&2
  file build/BOOTAA64.EFI >&2
  exit 1
fi

dd if=/dev/zero of=build/ios27.3-arm64.img bs=1M count=64 status=none
mkfs.vfat build/ios27.3-arm64.img >/dev/null
mmd -i build/ios27.3-arm64.img ::/EFI
mmd -i build/ios27.3-arm64.img ::/EFI/BOOT
mcopy -i build/ios27.3-arm64.img build/BOOTAA64.EFI ::/EFI/BOOT/BOOTAA64.EFI

echo "Built AArch64 EFI application."
echo "Built build/ios27.3-arm64.img"
