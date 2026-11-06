#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "\${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

CC="\${CROSS_COMPILE:-aarch64-linux-gnu-}gcc"
OBJCOPY="\${CROSS_COMPILE:-aarch64-linux-gnu-}objcopy"
LD="\${CROSS_COMPILE:-aarch64-linux-gnu-}ld"

EFI_CRT="$(dpkg -L gnu-efi | grep '/crt0-efi-aarch64\.o$' | head -n 1 || true)"
EFI_LDS="$(dpkg -L gnu-efi | grep '/elf_aarch64_efi\.lds$' | head -n 1 || true)"

if [[ -z "$EFI_CRT" || -z "$EFI_LDS" ]]; then
  echo "GNU-EFI AArch64 linker files were not found." >&2
  dpkg -L gnu-efi >&2
  exit 1
fi

EFI_LIB_DIR="$(dirname "$EFI_CRT")"
mkdir -p build

"$CC" \
  -I/usr/include/efi \
  -I/usr/include/efi/aarch64 \
  -I/usr/include/efi/protocol \
  -fpic -ffreestanding -fno-stack-protector -fshort-wchar \
  -fno-strict-aliasing -fno-builtin -Wall -Wextra \
  -c src/boot/main.c -o build/main.o

"$LD" \
  -nostdlib -znocombreloc \
  -T "$EFI_LDS" -shared -Bsymbolic \
  "$EFI_CRT" build/main.o \
  -L"$EFI_LIB_DIR" -lefi -lgnuefi \
  -o build/boot.so

"$OBJCOPY" \
  -j .text -j .sdata -j .data -j .dynamic \
  -j .dynsym -j .rel -j .rela -j .reloc \
  --target=efi-app-aarch64 \
  build/boot.so build/BOOTAA64.EFI

if ! file build/BOOTAA64.EFI | grep -qi 'PE32+.*EFI'; then
  echo "The output is not recognized as an AArch64 EFI application." >&2
  file build/BOOTAA64.EFI >&2
  exit 1
fi

dd if=/dev/zero of=build/ios27.3-arm64.img bs=1M count=64 status=none
mkfs.vfat build/ios27.3-arm64.img >/dev/null
mmd -i build/ios27.3-arm64.img ::/EFI
mmd -i build/ios27.3-arm64.img ::/EFI/BOOT
mcopy -i build/ios27.3-arm64.img build/BOOTAA64.EFI ::/EFI/BOOT/BOOTAA64.EFI

echo "Built build/BOOTAA64.EFI"
echo "Built build/ios27.3-arm64.img"
