include $(TA_DEV_KIT_DIR)/mk/conf.mk

BINARY = 5b9e0e40-2636-11e1-ad9e-0002a5d5c51b

# Hack: to locate the os_test library, we assume that its output path
# follows the same scheme as the output path of the os_test application.
# Therefore, simply replacing os_test by os_test_lib in the absolute path
# should get us a valid path
LDADD = -L$(subst os_test,os_test_lib,$(abspath $(link-out-dir))) -los_test

LDADD += -ldl

include ../ta_common.mk
