# iOS 27.3 ARM64 GSI product.
# Keep the official GSI partition layout and inherit its complete configuration.
$(call inherit-product, device/generic/common/gsi_arm64.mk)

PRODUCT_NAME := ios27_gsi_arm64
PRODUCT_DEVICE := generic_arm64
PRODUCT_BRAND := HorusWawe
PRODUCT_MODEL := iOS 27.3
PRODUCT_MANUFACTURER := HorusWawe
PRODUCT_CHARACTERISTICS := default
