SHELL = /bin/bash
ifeq ($(SYSDRV_PARAM), )
	SYSDRV_PARAM:=../../Makefile.param
	include $(SYSDRV_PARAM)
endif

KMPP_CURRENT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ifeq ($(RK_PROJECT_OUTPUT),)
-include $(KMPP_CURRENT_DIR)/../../../.BoardConfig.mk
endif

ifeq ($(ARCH), arm)
MPP_KO=ko_32_rel
RELEASE_KO=arm
endif
ifeq ($(ARCH), arm64)
MPP_KO=ko_64_rel
RELEASE_KO=arm64
endif

PKG_NAME_KMPP := kmpp
PKG_OUT := out
TOP_DRV_KO_OUT := $(KMPP_CURRENT_DIR)/../out

PKG_TARGET:=kmpp-build

ifeq ($(RK_ENABLE_FASTBOOT),y)
PKG_TARGET:=
endif

all: $(PKG_TARGET)

kmpp-build: kmpp_release
	@test ! -d $(KMPP_CURRENT_DIR)/$(PKG_NAME_KMPP) || (\
		rm -rf $(KMPP_CURRENT_DIR)/$(PKG_OUT); \
		mkdir -p $(KMPP_CURRENT_DIR)/$(PKG_OUT); \
		pushd $(KMPP_CURRENT_DIR)/$(PKG_NAME_KMPP)/; \
			make -j$(SYSDRV_JOBS) || exit $$?; \
			make linux_release; \
		popd; \
		cp $(KMPP_CURRENT_DIR)/$(PKG_NAME_KMPP)/prebuild/$(MPP_KO)/*.ko $(KMPP_CURRENT_DIR)/$(PKG_OUT); \
	);
	@mkdir -p $(TOP_DRV_KO_OUT) && \
		cp -fra $(PKG_OUT)/* $(TOP_DRV_KO_OUT) ;
	@echo "build $(PKG_NAME_KMPP) done"

kmpp_release:
	$(call MAROC_PKG_CHECK_PREBUILT, $(KMPP_CURRENT_DIR),$(PKG_NAME_KMPP),$(PKG_OUT))

clean:
	@test ! -d $(KMPP_CURRENT_DIR)/$(PKG_NAME_KMPP) || (\
		cd $(KMPP_CURRENT_DIR)/$(PKG_NAME_KMPP) ; \
		make clean; \
	);
	@rm -rf $(PKG_OUT)
	@echo "$(PKG_NAME_KMPP) clean done"
################################################################################
__RELEASE__: # DO NOT RUN THIS TARGET
ifeq ($(RK_ENABLE_FASTBOOT),y)
	$(call MAROC_PKG_RELEASE_ASM, $(KMPP_CURRENT_DIR),$(PKG_NAME_KMPP))
else
	$(call MAROC_PKG_RELEASE, $(KMPP_CURRENT_DIR),$(PKG_NAME_KMPP),$(PKG_OUT))
endif
