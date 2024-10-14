# SPDX-License-Identifier: BSD-3-Clause */

# Copyright (c) 2020 Rockchip Electronics Co., Ltd.

#-------------------------------------------------------------------------
# Set HAL_PATH and SOC
# Due to the serious customization of startup file, it is not provided as a general part
#-------------------------------------------------------------------------
HAL_LIB_INC += \
-I"$(HAL_PATH)/lib/hal/inc" \
-I"$(HAL_PATH)/lib/bsp/$(SOC)" \
-I"$(HAL_PATH)/lib/CMSIS/Device/$(SOC)/Include" \
-I"$(HAL_PATH)/lib/CMSIS/Device" \

HAL_LIB_SRC += \
    $(HAL_PATH)/lib/hal/src \
    $(HAL_PATH)/lib/hal/src/* \
    $(HAL_PATH)/lib/bsp/$(SOC) \
    $(HAL_PATH)/lib/CMSIS/Device/$(SOC)/Source/Templates \
