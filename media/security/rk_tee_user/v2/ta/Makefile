# Normally this makefile shouldn't be called directly and we expect the output
# path to be on a certain location to fit together with the other OP-TEE
# gits and helper scripts.

include ../scripts/common.mk
out-dir := $(call strip-trailing-slashes-and-dots,$(O))
ifeq ($(out-dir),)
$(error invalid output directory (O=$(O)))
endif

include $(TA_DEV_KIT_DIR)/mk/conf.mk

# Prevent use of LDFLAGS from the environment. For example, yocto exports
# LDFLAGS that are suitable for the client applications, not for TAs
LDFLAGS=

TA_DIRS := create_fail_test \
	   crypt \
	   os_test_lib \
	   os_test_lib_dl \
	   os_test \
	   rpc_test \
	   sims \
	   miss \
	   sims_keepalive \
	   storage \
	   storage2 \
	   concurrent \
	   concurrent_large \
	   storage_benchmark \
	   sha_perf \
	   aes_perf \
	   socket \
	   supp_plugin \
	   rk_test \
	   extra_app \
	   rk_anti_copy_board

ifeq ($(CFG_SECURE_DATA_PATH),y)
TA_DIRS += sdp_basic
endif

.PHONY: all
all: ta

.PHONY: ta
ta:
	$(q)$(foreach dir,$(TA_DIRS), [ ! -d $(dir) ] || $(MAKE) -C $(dir) O=$(out-dir)/ta/$(dir) &&) true

# remove build directories including ta/<ta-name>/ directories.
# Note: $(out-dir) may be a relative path.
RMDIR := rmdir --ignore-fail-on-non-empty
define rm-build-subdir
echo `[ -d $1 ] && { cd $1; $(RMDIR) $(out-dir)/ta $(out-dir) 2> /dev/null; true; }` > /dev/null
endef

.PHONY: clean
clean:
	$(q)$(foreach dir,$(TA_DIRS), [ ! -d $(dir) ] || $(MAKE) -C $(dir) O=$(out-dir)/ta/$(dir) $@ &&) true
	$(q)$(foreach dir,$(TA_DIRS), $(call rm-build-subdir,$(dir));)
