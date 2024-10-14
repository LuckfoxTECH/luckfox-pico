# Note that it is important that that $(BINARY) is set before we include
# ta_dev_kit.mk. In the optee_test this is done in the individual TA Makefiles.
include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk
