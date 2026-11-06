# TECNO KL5n target

| Property | Value |
|---|---|
| Device | TECNO KL5n |
| Android product | Android 14 / API 34 |
| CPU ABI | arm64-v8a |
| SoC | MediaTek MT6768 |
| Vendor API | 31 |
| Treble | enabled |
| Dynamic partitions | enabled |
| Virtual A/B | enabled |

## Partition ownership
The GSI build supplies the generic `system` image only. Device-specific partitions must come from the exact KL5n firmware: vendor, odm (if present), boot, init_boot (if present), vendor_boot (if present), dtbo, vbmeta / vbmeta_system, and super metadata / sizing.

Do not substitute images from another MediaTek model. The MT6768 name alone does not make vendor blobs interchangeable.

## Compatibility work remaining
- Record the exact partition map and sizes from KL5n firmware.
- Verify VNDK / vendor namespace compatibility before selecting a GSI release.
- Check VINTF manifests and compatibility matrix.
- Validate audio, camera, RIL, fingerprint, sensors, power HAL and SELinux.

Unknown partition sizes and hardware features are deliberately not guessed.
