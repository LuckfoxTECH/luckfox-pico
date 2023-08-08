#
# Overlay cfg-common.mk
#
export CONFIG_RK_CHIP=rv1106
export CONFIG_RK_MEDIA_CROSS=arm-rockchip830-linux-uclibcgnueabihf

#------------------------------------------------
# Enable libdrm
export CONFIG_LIBDRM=y

#------------------------------------------------
# Rockchip's MPP(Multimedia Processing Platform)
#
# Enable MPP Build
export CONFIG_RK_MPP=y
# Rockchip MPP use drm allocator
export CONFIG_RK_MPP_ALLOCATOR_DRM=n
# Install test programs
export CONFIG_RK_MPP_TESTS=y

#------------------------------------------------
# Rockchip's RKAIQ(Rockchip Auto Image Quality)
#
# Enable RKAIQ Build
export CONFIG_RK_RKAIQ=y
# ISP Hardware version
export CONFIG_RK_ISP_HW_VERSION_V32=y
# ISP build demo
export CONFIG_RK_ISP_BUILD_DEMO=y

#------------------------------------------------
# Rockchip's RGA2 (Raster Graphic Acceleration)
#
# Enable RGA2 Build
export CONFIG_RK_RGA2=y
# RGA2 build with libdrm
export CONFIG_RK_RGA2_USE_LIBDRM=n

#------------------------------------------------
# Rockchip's common algorithm
#
# Enable common_algorithm Build
export CONFIG_RK_COMMON_ALGORITHM=y

#------------------------------------------------
# Rockchip's rockit
#
# Enable Rockit Build
export CONFIG_RK_ROCKIT=y

#------------------------------------------------
# Rockchip's iva
#
# Enable iva Build
export CONFIG_RK_IVA=y

#------------------------------------------------
# Rockchip's ive
#
# Enable ive Build
export CONFIG_RK_IVE=y

#------------------------------------------------
# Rockchip's hardware crypto
#
# Enable librkcrypto Build
export CONFIG_RK_CRYPTO=n

# Enable alsa-lib
# export CONFIG_ALSA_LIB=y

# Enable libv4l
# export CONFIG_LIBV4L=y

# liblvgl
export CONFIG_LVGL=n

export CONFIG_RK_AVS=y
