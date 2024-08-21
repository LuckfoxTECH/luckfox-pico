# Normally this makefile shouldn't be called directly and we expect the output
# path to be on a certain location to fit together with the other OP-TEE
# gits and helper scripts.
ifeq ($O,)
$(error output path should be specified when calling this makefile)
endif

# Prevent use of LDFLAGS from the environment. For example, yocto exports
# LDFLAGS that are suitable for the client applications, not for TAs
LDFLAGS=

TA_DIRS := rk_test  \


ifdef CFG_GP_PACKAGE_PATH
TA_DIRS += GP_TTA_Arithmetical \
	   GP_TTA_Crypto \
	   GP_TTA_DS \
	   GP_TTA_TCF \
	   GP_TTA_TCF_ICA \
	   GP_TTA_TCF_ICA2 \
	   GP_TTA_TCF_MultipleInstanceTA \
	   GP_TTA_TCF_SingleInstanceTA \
	   GP_TTA_Time \
	   GP_TTA_answerErrorTo_Invoke \
	   GP_TTA_answerErrorTo_OpenSession \
	   GP_TTA_answerSuccessTo_OpenSession_Invoke \
	   GP_TTA_check_OpenSession_with_4_parameters \
	   GP_TTA_testingClientAPI
endif

.PHONY: all
all: ta

.PHONY: ta
ta: 
	$(q)$(foreach dir,$(TA_DIRS), $(MAKE) -C $(dir) O=$(O)/$(dir) &&) true

.PHONY: clean
clean:
	$(q)$(foreach dir,$(TA_DIRS), $(MAKE) -C $(dir) O=$(O)/$(dir) $@ &&) true
