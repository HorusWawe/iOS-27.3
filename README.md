# iOS 27.3 for TECNO KL5n

AOSP-based Android 14 ARM64 Generic System Image project. The OS deliverable is a native Android `system.img`, not an APK launcher.

## Build target

- AOSP branch: `android14-gsi`
- Official GSI lunch target: `gsi_arm64-userdebug`
- Device path: `generic_arm64`
- Architecture: ARM64, 64-bit Binder
- Output: `out/iOS27-KL5n-GSI-arm64-system.img`

The build uses the upstream AOSP GSI target and adds the project product identity through `vendor/ios27/ios27_gsi.mk`. It does not invent a separate lunch target.

## Build without a self-hosted runner

GitHub Actions starts an ephemeral Google Compute Engine VM, builds the image there, downloads the artifact, and deletes the VM even when the job fails.

The temporary builder is configured as:
- Ubuntu 22.04 x86_64
- `n2-highmem-16` (16 vCPU, 128 GB RAM)
- 600 GB SSD boot disk
- zone `us-central1-a`

One-time Google Cloud configuration is required. Enable Compute Engine API and configure Workload Identity Federation for this GitHub repository. Add these GitHub Actions repository secrets:

- `GCP_PROJECT_ID`: Google Cloud project ID with billing enabled
- `GCP_WIF_PROVIDER`: full Workload Identity Provider resource name
- `GCP_SERVICE_ACCOUNT`: service account email

The service account needs permission to create, inspect, SSH into, and delete Compute Engine instances, plus `iam.serviceAccountUser` on the VM service account. The project needs quota for `n2-highmem-16` and a 600 GB persistent SSD in the selected zone. The workflow is manually started from Actions.

This is an ephemeral managed cloud VM, not a self-hosted GitHub runner. VM compute and disk usage are billed by Google Cloud while the build runs.

## Device boundary

The GSI build produces the generic `system.img` only. It does not replace KL5n-specific `vendor`, `odm`, `boot`, `init_boot`, `vendor_boot`, `dtbo`, `vbmeta`, or super metadata.

Known target facts: TECNO KL5n, Android 14/API 34, ARM64, MediaTek MT6768, vendor API 31, Treble and dynamic partitions enabled, Virtual A/B enabled.

A successful AOSP build proves image generation, not full KL5n hardware compatibility. VINTF, vendor interfaces, partition capacity, audio, camera, radio, fingerprint, sensors, power HAL and SELinux still require device validation.

## Current implementation status

The build pipeline creates the native Android 14 GSI. The iOS-style SystemUI, Keyguard, framework overlays, boot animation and native system apps remain separate AOSP implementation tasks. They are not represented as completed.
