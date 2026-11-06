#!/usr/bin/env python3
"""Collect a privacy-filtered hardware inventory from a rooted TECNO KL5n via ADB."""

from __future__ import annotations

import argparse
import re
import shutil
import subprocess
import sys
import zipfile
from datetime import datetime
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "build" / "kl5n-device-report"

PROPERTIES = (
    "ro.product.manufacturer",
    "ro.product.brand",
    "ro.product.model",
    "ro.product.device",
    "ro.product.name",
    "ro.board.platform",
    "ro.hardware",
    "ro.boot.hardware",
    "ro.soc.manufacturer",
    "ro.soc.model",
    "ro.build.version.release",
    "ro.build.version.sdk",
    "ro.vendor.api_level",
    "ro.boot.slot_suffix",
    "ro.boot.dynamic_partitions",
    "ro.virtual_ab.enabled",
)

COMMANDS = {
    "device-tree.txt": (
        "printf 'model: '; tr '\\000' ' ' < /proc/device-tree/model 2>/dev/null; "
        "printf '\\ncompatible:\\n'; tr '\\000' '\\n' < /proc/device-tree/compatible 2>/dev/null"
    ),
    "memory-map.txt": "cat /proc/iomem 2>/dev/null",
    "memory-info.txt": "cat /proc/meminfo 2>/dev/null",
    "interrupts.txt": "cat /proc/interrupts 2>/dev/null",
    "partitions.txt": "cat /proc/partitions 2>/dev/null",
    "input-devices.txt": "cat /proc/bus/input/devices 2>/dev/null",
    "block-links.txt": (
        "ls -la /dev/block/by-name /dev/block/bootdevice/by-name "
        "/dev/block/platform/*/by-name 2>/dev/null"
    ),
    "graphics-backlight.txt": (
        "ls -la /sys/class/graphics /sys/class/backlight /sys/class/input 2>/dev/null"
    ),
    "i2c-spi-devices.txt": (
        "for d in /sys/bus/i2c/devices /sys/bus/spi/devices; do "
        "echo === $d; ls -la $d 2>/dev/null; done"
    ),
    "platform-devices.txt": "ls -la /sys/bus/platform/devices 2>/dev/null",
    "boot-command-line.txt": "cat /proc/cmdline 2>/dev/null",
}

SENSITIVE = re.compile(
    r"(?i)(androidboot\.(?:serialno|serial|imei|wifimacaddr|btmacaddr|macaddr)"
    r"|(?:serialno|imei|meid|macaddr|wifi_mac|bt_mac)=)[^\s]+"
)


def run(args: list[str], *, binary: bool = False) -> subprocess.CompletedProcess:
    return subprocess.run(
        args,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
        text=not binary,
        errors="replace" if not binary else None,
    )


def adb_shell(command: str) -> str:
    result = run(["adb", "shell", "su", "-c", command])
    output = result.stdout or ""
    if result.returncode != 0 and not output.strip():
        output = result.stderr or f"command exited with {result.returncode}"
    return output


def sanitize(value: str) -> str:
    return SENSITIVE.sub(lambda m: m.group(0).split("=")[0] + "=<redacted>", value)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Collect a sanitized KL5n hardware report. No partitions are read or modified."
    )
    parser.add_argument(
        "--include-raw-dtb",
        action="store_true",
        help="also save /sys/firmware/fdt as a separate raw file; it may contain device-specific calibration data",
    )
    args = parser.parse_args()

    if not shutil.which("adb"):
        print("adb was not found in PATH. Install Android platform-tools first.", file=sys.stderr)
        return 2

    devices = run(["adb", "devices"])
    lines = [line.split()[0] for line in (devices.stdout or "").splitlines()[1:] if "\tdevice" in line]
    if not lines:
        print("No authorized ADB device found. Check USB debugging and adb devices.", file=sys.stderr)
        return 2
    if len(lines) > 1:
        print("More than one ADB device is connected. Leave only the KL5n connected.", file=sys.stderr)
        return 2

    root_check = adb_shell("id")
    if "uid=0" not in root_check:
        print("Root shell unavailable. Magisk su did not return uid=0.", file=sys.stderr)
        return 2

    OUT.mkdir(parents=True, exist_ok=True)
    for old in OUT.iterdir():
        if old.is_file():
            old.unlink()

    summary = [
        "TECNO KL5n hardware inventory",
        f"Collected: {datetime.now().astimezone().isoformat(timespec='seconds')}",
        "Collection mode: read-only, allowlisted system metadata",
        "",
        "Android partitions are not read, written, mounted, or modified.",
        "Serial numbers, IMEI/MEID and common MAC-address command-line fields are redacted.",
        "",
        "[Selected properties]",
    ]
    for prop in PROPERTIES:
        value = adb_shell(f"getprop {prop}").strip()
        summary.append(f"{prop}={sanitize(value) if value else '<empty>'}")

    (OUT / "summary.txt").write_text("\n".join(summary) + "\n", encoding="utf-8")

    for filename, command in COMMANDS.items():
        content = sanitize(adb_shell(command))
        (OUT / filename).write_text(content, encoding="utf-8", errors="replace")

    if args.include_raw_dtb:
        check = adb_shell("test -r /sys/firmware/fdt && echo PRESENT || echo MISSING").strip()
        if check == "PRESENT":
            result = run(
                ["adb", "exec-out", "su", "-c", "cat /sys/firmware/fdt"],
                binary=True,
            )
            if result.returncode == 0 and result.stdout:
                (OUT / "device-tree.raw.dtb").write_bytes(result.stdout)
                print("Raw DTB saved separately. Inspect it locally before sharing.")
            else:
                print("Could not read /sys/firmware/fdt; continuing without raw DTB.")
        else:
            print("The kernel does not expose /sys/firmware/fdt; raw DTB was not collected.")

    archive = ROOT / "build" / "kl5n-device-report.zip"
    with zipfile.ZipFile(archive, "w", compression=zipfile.ZIP_DEFLATED) as zf:
        for path in OUT.iterdir():
            if path.is_file():
                zf.write(path, path.name)

    print(f"Report created: {archive}")
    print("Review the report before sharing it. Raw DTB is optional and may contain calibration data.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
