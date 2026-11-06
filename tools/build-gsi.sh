#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
AOSP_DIR="${AOSP_DIR:-$ROOT/.aosp}"
AOSP_BRANCH="${AOSP_BRANCH:-android14-gsi}"
JOBS="${JOBS:-$(nproc)}"

if [[ "$(uname -s)" != "Linux" || "$(uname -m)" != "x86_64" ]]; then
  echo "AOSP GSI builds require Linux x86_64." >&2
  exit 2
fi
command -v repo >/dev/null 2>&1 || { echo "Install the AOSP repo launcher first." >&2; exit 2; }

FREE_GB="$(df -BG "$ROOT" | awk 'NR==2 {gsub(/G/,"",$4); print $4}')"
if [[ "${FREE_GB:-0}" -lt 350 ]]; then
  echo "At least 350 GB free disk required; detected ${FREE_GB:-unknown} GB." >&2
  exit 2
fi

mkdir -p "$AOSP_DIR"
if [[ ! -d "$AOSP_DIR/.repo" ]]; then
  repo init -u https://android.googlesource.com/platform/manifest -b "$AOSP_BRANCH" --partial-clone
fi
cd "$AOSP_DIR"
repo sync -c -j"$JOBS" --force-sync

mkdir -p vendor/ios27
cp "$ROOT/vendor/ios27/ios27_gsi.mk" vendor/ios27/ios27_gsi.mk

GSI_PRODUCT="device/generic/common/gsi_product.mk"
if [[ ! -f "$GSI_PRODUCT" ]]; then
  echo "Expected GSI product makefile not found: $GSI_PRODUCT" >&2
  echo "AOSP branch layout changed; refusing to patch an unknown file." >&2
  exit 3
fi
if ! grep -q 'vendor/ios27/ios27_gsi.mk' "$GSI_PRODUCT"; then
  printf '\n# iOS 27.3 product integration\n$(call inherit-product, vendor/ios27/ios27_gsi.mk)\n' >> "$GSI_PRODUCT"
fi

source build/envsetup.sh
lunch gsi_arm64-userdebug
m -j"$JOBS" systemimage

OUT="out/target/product/generic_arm64"
[[ -s "$OUT/system.img" ]] || { echo "system.img not found at $OUT" >&2; exit 4; }
mkdir -p "$ROOT/out"
cp "$OUT/system.img" "$ROOT/out/iOS27-KL5n-GSI-arm64-system.img"
cp "$OUT/installed-files.txt" "$ROOT/out/installed-files.txt" 2>/dev/null || true
sha256sum "$ROOT/out/iOS27-KL5n-GSI-arm64-system.img" | tee "$ROOT/out/SHA256SUMS"
echo "GSI image created: $ROOT/out/iOS27-KL5n-GSI-arm64-system.img"
