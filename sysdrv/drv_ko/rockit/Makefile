SHELL = /bin/bash
ifeq ($(SYSDRV_PARAM), )
	SYSDRV_PARAM:=../../Makefile.param
	include $(SYSDRV_PARAM)
endif

ROCKIT_CURRENT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ifeq ($(RK_PROJECT_OUTPUT),)
-include $(ROCKIT_CURRENT_DIR)/../../../.BoardConfig.mk
endif

ifeq ($(ARCH), arm)
MPP_KO=ko_32_rel
RELEASE_KO=arm
endif
ifeq ($(ARCH), arm64)
MPP_KO=ko_64_rel
RELEASE_KO=arm64
endif

PKG_NAME_ROCKIT_KO := rockit-ko
PKG_OUT := out
TOP_DRV_KO_OUT := $(ROCKIT_CURRENT_DIR)/../out

PKG_TARGET:=rockit-build

ifeq ($(RK_ENABLE_FASTBOOT),y)
PKG_TARGET:=
endif

all: $(PKG_TARGET)

rockit-build: rockit_ko_release
	@test ! -d $(ROCKIT_CURRENT_DIR)/$(PKG_NAME_ROCKIT_KO) || (\
		mkdir -p $(ROCKIT_CURRENT_DIR)/$(PKG_OUT); \
		pushd $(ROCKIT_CURRENT_DIR)/$(PKG_NAME_ROCKIT_KO)/; \
			make -j$(SYSDRV_JOBS) BUILD_IN_IPC_SDK=YES || exit $$?; \
			make linux_release BUILD_IN_IPC_SDK=YES; \
		popd; \
		cp $(ROCKIT_CURRENT_DIR)/$(PKG_NAME_ROCKIT_KO)/install/ko/*.ko $(ROCKIT_CURRENT_DIR)/$(PKG_OUT); \
		cp $(ROCKIT_CURRENT_DIR)/$(PKG_NAME_ROCKIT_KO)/install/ko/*_hpmcu_wrap_*.bin $(ROCKIT_CURRENT_DIR)/$(PKG_OUT)/hpmcu_wrap.bin; \
	);
	@mkdir -p $(TOP_DRV_KO_OUT) && \
		cp -fra $(PKG_OUT)/* $(TOP_DRV_KO_OUT) ;
	@echo "build $(PKG_NAME_ROCKIT_KO) done"

rockit_ko_release:
	$(call MAROC_PKG_CHECK_PREBUILT, $(ROCKIT_CURRENT_DIR),$(PKG_NAME_ROCKIT_KO),$(PKG_OUT))

clean:
	@test ! -d $(ROCKIT_CURRENT_DIR)/$(PKG_NAME_ROCKIT_KO) || (\
		cd $(ROCKIT_CURRENT_DIR)/$(PKG_NAME_ROCKIT_KO); \
		make clean ;\
	);
	@rm -rf $(PKG_OUT)
	@echo "$(PKG_NAME_ROCKIT_KO) clean done"
################################################################################
__RELEASE__: # DO NOT RUN THIS TARGET
ifeq ($(RK_ENABLE_FASTBOOT),y)
	$(call MAROC_PKG_RELEASE_ASM, $(ROCKIT_CURRENT_DIR),$(PKG_NAME_ROCKIT_KO))
else
	$(call MAROC_PKG_RELEASE, $(ROCKIT_CURRENT_DIR),$(PKG_NAME_ROCKIT_KO),$(PKG_OUT))
endif
