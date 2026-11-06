#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
AOSP_DIR="${AOSP_DIR:-$ROOT/.aosp-cuttlefish}"
AOSP_BRANCH="${AOSP_BRANCH:-android14-gsi}"
JOBS="${JOBS:-$(nproc)}"
PRODUCT="aosp_cf_x86_64_phone"
PRODUCT_MK="device/google/cuttlefish/vsoc_x86_64/phone/aosp_cf.mk"

fail() { echo "ERROR: $*" >&2; exit 1; }
info() { printf '\n==> %s\n' "$*"; }

[[ "$(uname -s)" == Linux && "$(uname -m)" == x86_64 ]] || fail "Cuttlefish host must be Linux x86_64."
for bin in repo git python3 make gcc g++ curl; do
  command -v "$bin" >/dev/null 2>&1 || fail "Missing required command: $bin"
done
[[ -c /dev/kvm && -r /dev/kvm && -w /dev/kvm ]] || fail "/dev/kvm is unavailable. Enable KVM/nested virtualization and grant the current user access."
command -v launch_cvd >/dev/null 2>&1 || fail "Cuttlefish host packages are missing. Install android-cuttlefish host packages first."

FREE_GB="$(df -BG "$AOSP_DIR" 2>/dev/null | awk 'NR==2 {gsub(/G/,"",$4); print $4}')"
if [[ ! "$FREE_GB" =~ ^[0-9]+$ ]]; then
  mkdir -p "$AOSP_DIR"
  FREE_GB="$(df -BG "$AOSP_DIR" | awk 'NR==2 {gsub(/G/,"",$4); print $4}')"
fi
(( FREE_GB >= 350 )) || fail "Need at least 350 GB free for AOSP source and build output; found ${FREE_GB} GB."

info "Sync Android 14 source"
mkdir -p "$AOSP_DIR"
cd "$AOSP_DIR"
if [[ ! -d .repo ]]; then
  repo init -u https://android.googlesource.com/platform/manifest -b "$AOSP_BRANCH" --partial-clone
else
  repo init -u https://android.googlesource.com/platform/manifest -b "$AOSP_BRANCH" --partial-clone
fi
repo sync -c -j"$JOBS" --force-sync

[[ -f "$PRODUCT_MK" ]] || fail "Cuttlefish product makefile not found: $PRODUCT_MK"
mkdir -p vendor/ios27
cp "$ROOT/vendor/ios27/ios27_cuttlefish.mk" vendor/ios27/ios27_cuttlefish.mk
if ! grep -qF 'vendor/ios27/ios27_cuttlefish.mk' "$PRODUCT_MK"; then
  printf '\n# iOS 27.3 Cuttlefish product identity\n$(call inherit-product, vendor/ios27/ios27_cuttlefish.mk)\n' >> "$PRODUCT_MK"
fi

info "Configure and build Android 14 Cuttlefish"
source build/envsetup.sh
lunch "${PRODUCT}-userdebug"
m -j"$JOBS"

info "Boot the source-built OS in Cuttlefish"
launch_cvd --start_webrtc=true --daemon
ADB_BIN="${ANDROID_HOST_OUT}/bin/adb"
[[ -x "$ADB_BIN" ]] || ADB_BIN="$(command -v adb || true)"
[[ -n "$ADB_BIN" && -x "$ADB_BIN" ]] || fail "adb was not found in ANDROID_HOST_OUT or PATH after the build."
"$ADB_BIN" wait-for-device

echo
echo "Cuttlefish is booted from this AOSP source tree."
echo "Open https://localhost:8443 in a browser on this host."
echo "Use '$ADB_BIN shell' to access the running Android system."
echo "Stop it with: stop_cvd"
