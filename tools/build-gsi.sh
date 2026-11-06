#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
AOSP_DIR="${AOSP_DIR:-$ROOT/.aosp}"
AOSP_BRANCH="${AOSP_BRANCH:-android14-gsi}"
JOBS="${JOBS:-$(python3 - <<'PY'
import os
cpu = os.cpu_count() or 4
try:
    mem_gb = int(open("/proc/meminfo").read().split("MemTotal:")[1].split()[0]) // 1048576
except Exception:
    mem_gb = 16
print(max(1, min(cpu, max(1, mem_gb // 3), 24)))
PY
)}"
PRODUCT="ios27_gsi_arm64"
DEVICE="generic_arm64"
OUT="$AOSP_DIR/out/target/product/$DEVICE"
IMAGE="$OUT/system.img"

fail() { echo "ERROR: $*" >&2; exit 1; }

[[ "$(uname -s)" == Linux && "$(uname -m)" == x86_64 ]] || fail "AOSP GSI builds require Linux x86_64."
for bin in repo git curl python3 make gcc g++; do
  command -v "$bin" >/dev/null 2>&1 || fail "Missing required command: $bin"
done
[[ -x "$(command -v repo)" ]] || fail "The repo launcher is not executable."

FREE_GB="$(df -BG "$ROOT" | awk 'NR==2 {gsub(/G/,"",$4); print $4}')"
[[ "${FREE_GB:-0}" =~ ^[0-9]+$ ]] || fail "Could not determine free disk space."
(( FREE_GB >= 350 )) || fail "Need at least 350 GB free; found ${FREE_GB} GB."
echo "Build configuration: branch=$AOSP_BRANCH product=$PRODUCT jobs=$JOBS free_disk=${FREE_GB}G"

mkdir -p "$AOSP_DIR"
if [[ -e "$AOSP_DIR/.repo" ]]; then
  (cd "$AOSP_DIR" && repo init -u https://android.googlesource.com/platform/manifest -b "$AOSP_BRANCH" --partial-clone)
else
  (cd "$AOSP_DIR" && repo init -u https://android.googlesource.com/platform/manifest -b "$AOSP_BRANCH" --partial-clone)
fi
cd "$AOSP_DIR"
repo sync -c -j"$JOBS" --force-sync

# Register a separate product. Do not patch upstream GSI makefiles: the custom
# product inherits the official ARM64 GSI and overrides identity afterwards.
mkdir -p vendor/ios27
cp "$ROOT/vendor/ios27/ios27_gsi_arm64.mk" vendor/ios27/ios27_gsi_arm64.mk
cp "$ROOT/vendor/ios27/AndroidProducts.mk" vendor/ios27/AndroidProducts.mk

[[ -f device/generic/common/gsi_arm64.mk ]] || fail "AOSP tree lacks device/generic/common/gsi_arm64.mk."
[[ -f device/generic/common/gsi_product.mk ]] || fail "AOSP tree lacks device/generic/common/gsi_product.mk."
grep -q 'ios27_gsi_arm64' vendor/ios27/AndroidProducts.mk || fail "Custom product registration is missing."

source build/envsetup.sh
lunch "${PRODUCT}-userdebug"
m -j"$JOBS" systemimage

[[ -s "$IMAGE" ]] || fail "Build completed without producing $IMAGE."
mkdir -p "$ROOT/out"
cp "$IMAGE" "$ROOT/out/iOS27-KL5n-GSI-arm64-system.img"
cp "$OUT/installed-files.txt" "$ROOT/out/installed-files.txt" 2>/dev/null || true
sha256sum "$ROOT/out/iOS27-KL5n-GSI-arm64-system.img" | tee "$ROOT/out/SHA256SUMS"
file "$ROOT/out/iOS27-KL5n-GSI-arm64-system.img" | tee "$ROOT/out/IMAGE-INFO.txt"
echo "SUCCESS: $ROOT/out/iOS27-KL5n-GSI-arm64-system.img"
