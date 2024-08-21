ifneq ($O,)
	out-dir := $O
else
	# If no build folder has been specified, then create all build files in
	# the current directory under a folder named out.
	out-dir := $(CURDIR)/out
endif

-include $(TA_DEV_KIT_DIR)/host_include/conf.mk

ifneq ($V,1)
	q := @
else
	q :=
endif

.PHONY: all
ifneq ($(wildcard $(TA_DEV_KIT_DIR)/host_include/conf.mk),)
ifeq "$(BUILD_CA)" "y"
all: rk_test ta
else
all: ta
endif
else
all:
	$(q)echo "TA_DEV_KIT_DIR is not correctly defined" && false
endif

.PHONY: ta
ta:
	$(q)$(MAKE) -C ta CROSS_COMPILE="$(CROSS_COMPILE_TA)" \
			  q=$(q) \
			  O=$(out-dir)/ta \
			  $@

.PHONY: rk_test
rk_test:
	$(q)$(MAKE) -C host/rk_test CROSS_COMPILE="$(CROSS_COMPILE_HOST)" \
				--no-builtin-variables \
				q=$(q) \
				O=$(out-dir)/rk_test  \
				$@

.PHONY: clean
ifneq ($(wildcard $(TA_DEV_KIT_DIR)/host_include/conf.mk),)
clean:
	$(q)$(MAKE) -C host/rk_test O=$(out-dir) $@
	$(q)$(MAKE) -C ta O=$(out-dir)/ta q=$(q) $@
	$(q)find ta/ \( -name "*.ta" -o -name "*.dmp" -o -name "*.elf" -o -name "*.map" -o -name "*.d" \) -exec rm -rf {} \;
else
clean:
	$(q)echo "TA_DEV_KIT_DIR is not correctly defined"
	$(q)echo "You can remove manually $(out-dir)"
endif

.PHONY: patch
patch:
ifdef CFG_GP_PACKAGE_PATH
CFG_GP_API?=1.0
CFG_GP_XSL_PACKAGE_PATH?=$(CURDIR)/package/testsuite/global_platform/api_1.0/GP_XSL_TEE_Initial_Configuration-Test_Suite_v1_0_0-2014-12-03-STM

ifeq "$(wildcard $(CFG_GP_XSL_PACKAGE_PATH) )" ""
$(error CFG_GP_XSL_PACKAGE_PATH must contain the xsl package)
endif

ifeq "$(wildcard $(CFG_GP_PACKAGE_PATH) )" ""
$(error CFG_GP_PACKAGE_PATH must contain the xml package from GP)
endif

ifeq "$(wildcard /usr/include/openssl )" ""
$(error openssl must be installed)
endif

# Note that only TEE_Initial_Configuration-Test_Suite_v1_1_0_4-2014_11_07 is supported

GP_XTEST_OUT_DIR=$(CURDIR)/host/xtest
GP_XTEST_IN_DIR=${GP_XTEST_OUT_DIR}/global_platform/${CFG_GP_API}
GP_USERTA_DIR=$(CURDIR)/ta

define patch-file
	@if [ ! -e ${1} ]; then \
		echo "Error: File to patch is unknown: $1"; \
		return 1; \
	fi
	@if [ ! -e ${2} ]; then \
		echo "Error: Patch to apply is unknown: $2"; \
		return 1; \
	fi
	@if [ ! -e ${1}.orig ]; then \
		patch -N -b ${1} < ${2}; \
	else \
		echo "Warning: Patch already applied on `basename $1`"; \
	fi
endef

# openssl .h file installation
forgpdir=${CURDIR}/host/xtest/for_gp
.PHONY: patch-openssl
patch-openssl:
	$(q)mkdir -p ${forgpdir}/include/openssl ${forgpdir}/lib
	$(q)if [ -d /usr/include/x86_64-linux-gnu/openssl ]; then \
		cp -r /usr/include/x86_64-linux-gnu/openssl ${forgpdir}/include ; \
	fi
	$(q)cp /usr/include/openssl/*.h $f ${forgpdir}/include/openssl

define mv-package
	@if [ -d ${1} ]; then \
		mv ${1} ${CFG_GP_PACKAGE_PATH}/packages ;\
	fi
endef

define patch-xalan
	$(q)rm -f ${GP_XTEST_OUT_DIR}/${3} ${GP_XTEST_OUT_DIR}/${3}.orig
	$(q)xalan -in ${GP_XTEST_IN_DIR}/${1} -xsl ${GP_XTEST_IN_DIR}/${2} -out ${GP_XTEST_OUT_DIR}/${3}
endef

# Generate host files
define patch-cp-ta
	$(q)rm -rf $(GP_USERTA_DIR)/${3}
	$(q)mkdir -p $(GP_USERTA_DIR)/${3}
	$(q)cp -p $(CFG_GP_PACKAGE_PATH)/${1}/* $(GP_USERTA_DIR)/${3}
	$(q)cp -p $(CFG_GP_XSL_PACKAGE_PATH)/${2}/* $(GP_USERTA_DIR)/${3}
endef

.PHONY: patch-generate-host
patch-generate-host: patch-package
	@echo "INFO: Generate host tests"
	$(q) mkdir -p ${GP_XTEST_IN_DIR} ${GP_XTEST_IN_DIR}
	$(q)find ${CFG_GP_PACKAGE_PATH}/packages -type f -name "*.xml" -exec cp -p {} ${GP_XTEST_IN_DIR} \;
	$(q)find ${CFG_GP_XSL_PACKAGE_PATH}/packages -type f -name "*.xsl" -exec cp -p {} ${GP_XTEST_IN_DIR} \;
	$(call patch-xalan,TEE.xml,adbg_case_declare.xsl,adbg_case_declare.h)
	$(call patch-xalan,TEE.xml,adbg_entry_declare.xsl,adbg_entry_declare.h)
	$(call patch-xalan,TEE.xml,TEE.xsl,xtest_7000_gp.c)
	$(call patch-xalan,TEE_DataStorage_API.xml,TEE_DataStorage_API.xsl,xtest_7500.c)
	$(call patch-xalan,TEE_Internal_API.xml,TEE_Internal_API.xsl,xtest_8000.c)
	$(call patch-xalan,TEE_TimeArithm_API.xml,TEE_TimeArithm_API.xsl,xtest_8500.c)
	$(call patch-xalan,TEE_Crypto_API.xml,TEE_Crypto_API.xsl,xtest_9000.c)
	@echo "INFO: Patch host tests"
	# $(q)sed -i '752 c\    xtest_tee_deinit();\n' ${GP_XTEST_OUT_DIR}/xtest_7000.c
	# $(q)sed -i '1076 c\    xtest_tee_deinit();\n' ${GP_XTEST_OUT_DIR}/xtest_8000.c
	# $(q)sed -i '2549 c\    xtest_tee_deinit();\n' ${GP_XTEST_OUT_DIR}/xtest_8500.c
	# $(q)sed -i '246 c\    xtest_tee_deinit();\n' ${GP_XTEST_OUT_DIR}/xtest_9000.c
	$(call patch-file,host/xtest/xtest_9000.c,${CFG_GP_XSL_PACKAGE_PATH}/host/xtest/xtest_9000.c.patch)

.PHONY: patch-generate-ta
patch-generate-ta: patch-package
	@echo "INFO: Generate TA"
	$(call patch-cp-ta,TTAs/TTA_Arithmetical/TTA_Arithmetical/code_files,TTAs/TTA_Arithmetical/code_files,GP_TTA_Arithmetical)
	$(call patch-cp-ta,TTAs/TTA_DS/TTA_DS/code_files,TTAs/TTA_DS/code_files,GP_TTA_DS)
	$(call patch-cp-ta,TTAs/TTA_ClientAPI/TTA_answerErrorTo_Invoke/code_files,TTAs/TTA_ClientAPI/TTA_answerErrorTo_Invoke/code_files,GP_TTA_answerErrorTo_Invoke)
	$(call patch-cp-ta,TTAs/TTA_ClientAPI/TTA_check_OpenSession_with_4_parameters/code_files,TTAs/TTA_ClientAPI/TTA_check_OpenSession_with_4_parameters/code_files,GP_TTA_check_OpenSession_with_4_parameters)
	$(q) cp $(CFG_GP_PACKAGE_PATH)/TTAs/TTA_ClientAPI/ta_check_OpenSession_with_4_parameters/code_files/TTA_check_OpenSession_with_4_parameters_protocol.h $(GP_USERTA_DIR)/GP_TTA_check_OpenSession_with_4_parameters
	$(call patch-cp-ta,TTAs/TTA_ClientAPI/TTA_answerErrorTo_OpenSession/code_files,TTAs/TTA_ClientAPI/TTA_answerErrorTo_OpenSession/code_files,GP_TTA_answerErrorTo_OpenSession)
	$(call patch-cp-ta,TTAs/TTA_ClientAPI/TTA_testingClientAPI/code_files,TTAs/TTA_ClientAPI/TTA_testingClientAPI/code_files,GP_TTA_testingClientAPI)
	$(call patch-cp-ta,TTAs/TTA_ClientAPI/TTA_answerSuccessTo_OpenSession_Invoke/code_files,TTAs/TTA_ClientAPI/TTA_answerSuccessTo_OpenSession_Invoke/code_files,GP_TTA_answerSuccessTo_OpenSession_Invoke)
	$(call patch-cp-ta,TTAs/TTA_Crypto/TTA_Crypto/code_files,TTAs/TTA_Crypto/code_files,GP_TTA_Crypto)
	$(call patch-cp-ta,TTAs/TTA_Time/TTA_Time/code_files,TTAs/TTA_Time/code_files,GP_TTA_Time)
	$(call patch-cp-ta,TTAs/TTA_TCF/TTA_TCF_SingleInstanceTA/code_files,TTAs/TTA_TCF/TTA_TCF_SingleInstanceTA/code_files,GP_TTA_TCF_SingleInstanceTA)
	$(call patch-cp-ta,TTAs/TTA_TCF/TTA_TCF_ICA/code_files,TTAs/TTA_TCF/TTA_TCF_ICA/code_files,GP_TTA_TCF_ICA)
	$(call patch-cp-ta,TTAs/TTA_TCF/TTA_TCF_MultipleInstanceTA/code_files,TTAs/TTA_TCF/TTA_TCF_MultipleInstanceTA/code_files,GP_TTA_TCF_MultipleInstanceTA)
	$(call patch-cp-ta,TTAs/TTA_TCF/TTA_TCF_ICA2/code_files,TTAs/TTA_TCF/TTA_TCF_ICA2/code_files,GP_TTA_TCF_ICA2)
	$(call patch-cp-ta,TTAs/TTA_TCF/TTA_TCF/code_files,TTAs/TTA_TCF/TTA_TCF/code_files,GP_TTA_TCF)

# Patch the GP package
.PHONY: patch-package
patch-package:
	@echo "INFO: Patch provided tests"
	$(q)mkdir -p ${CFG_GP_PACKAGE_PATH}/packages
	$(call mv-package,${CFG_GP_PACKAGE_PATH}/ClientAPI)
	$(call mv-package,${CFG_GP_PACKAGE_PATH}/Crypto)
	$(call mv-package,${CFG_GP_PACKAGE_PATH}/DataStorage)
	$(call mv-package,${CFG_GP_PACKAGE_PATH}/Time_Arithmetical)
	$(call mv-package,${CFG_GP_PACKAGE_PATH}/TrustedCoreFw)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/packages/ClientAPI/xmlStable/TEE.xml,${CFG_GP_XSL_PACKAGE_PATH}/packages/ClientAPI/xmlpatch/v1_1_0_4-2014_11_07/TEE.xml.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/packages/Crypto/xmlStable/TEE_Crypto_API.xml,${CFG_GP_XSL_PACKAGE_PATH}/packages/Crypto/xmlpatch/v1_1_0_4-2014_11_07/TEE_Crypto_API.xml.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/packages/DataStorage/xmlStable/TEE_DataStorage_API.xml,${CFG_GP_XSL_PACKAGE_PATH}/packages/DataStorage/xmlpatch/v1_1_0_4-2014_11_07/TEE_DataStorage_API.xml.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/packages/Time_Arithmetical/xmlStable/TEE_TimeArithm_API.xml,${CFG_GP_XSL_PACKAGE_PATH}/packages/Time_Arithmetical/xmlpatch/v1_1_0_4-2014_11_07/TEE_TimeArithm_API.xml.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/packages/TrustedCoreFw/xmlStable/TEE_Internal_API.xml,${CFG_GP_XSL_PACKAGE_PATH}/packages/TrustedCoreFw/xmlpatch/v1_1_0_4-2014_11_07/TEE_Internal_API.xml.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_Arithmetical/TTA_Arithmetical/code_files/TTA_Arithmetical.c,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_Arithmetical/code_patches/v1_1_0_4-2014_11_07/TTA_Arithmetical.c.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_Arithmetical/TTA_Arithmetical/code_files/TTA_Arithmetical_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_Arithmetical/code_patches/v1_1_0_4-2014_11_07/TTA_Arithmetical_protocol.h.patch)
	# $(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_ClientAPI/ta_check_OpenSession_with_4_parameters/code_files/TTA_check_OpenSession_with_4_parameters_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_check_OpenSession_with_4_parameters/code_patches/v1_1_0_4-2014_11_07/TTA_check_OpenSession_with_4_parameters_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_testingClientAPI/code_files/TTA_testingClientAPI_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_testingClientAPI/code_patches/v1_1_0_4-2014_11_07/TTA_testingClientAPI_protocol.h.patch)
	# $(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_answerSuccessTo_OpenSession_Invoke/code_files/TTA_answerSuccessTo_OpenSession_Invoke_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_answerSuccessTo_OpenSession_Invoke/code_patches/v1_1_0_4-2014_11_07/TTA_answerSuccessTo_OpenSession_Invoke_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_answerErrorTo_OpenSession/code_files/TTA_answerErrorTo_OpenSession_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_answerErrorTo_OpenSession/code_patches/v1_1_0_4-2014_11_07/TTA_answerErrorTo_OpenSession_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_answerErrorTo_Invoke/code_files/TTA_answerErrorTo_Invoke_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_ClientAPI/TTA_answerErrorTo_Invoke/code_patches/v1_1_0_4-2014_11_07/TTA_answerErrorTo_Invoke_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_Crypto/TTA_Crypto/code_files/TTA_Crypto.c,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_Crypto/code_patches/v1_1_0_4-2014_11_07/TTA_Crypto.c.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_Crypto/TTA_Crypto/code_files/TTA_Crypto_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_Crypto/code_patches/v1_1_0_4-2014_11_07/TTA_Crypto_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_DS/TTA_DS/code_files/TTA_DS_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_DS/code_patches/v1_1_0_4-2014_11_07/TTA_DS_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_DS/TTA_DS/code_files/TTA_DS.c,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_DS/code_patches/v1_1_0_4-2014_11_07/TTA_DS.c.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TEE_include/tee_internal_api.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TEE_include/code_patches/v1_1_0_4-2014_11_07/tee_internal_api.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_ICA/code_files/TTA_TCF_ICA_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_ICA/code_patches/v1_1_0_4-2014_11_07/TTA_TCF_ICA_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_MultipleInstanceTA/code_files/TTA_TCF_MultipleInstanceTA_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_MultipleInstanceTA/code_patches/v1_1_0_4-2014_11_07/TTA_TCF_MultipleInstanceTA_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_MultipleInstanceTA/code_files/TTA_TCF_MultipleInstanceTA.c,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_MultipleInstanceTA/code_patches/v1_1_0_4-2014_11_07/TTA_TCF_MultipleInstanceTA.c.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_SingleInstanceTA/code_files/TTA_TCF_SingleInstanceTA.c,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_SingleInstanceTA/code_patches/v1_1_0_4-2014_11_07/TTA_TCF_SingleInstanceTA.c.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF/code_files/TTA_TCF.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF/code_patches/v1_1_0_4-2014_11_07/TTA_TCF.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_SingleInstanceTA/code_files/TTA_TCF_SingleInstanceTA_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_TCF/TTA_TCF_SingleInstanceTA/code_patches/v1_1_0_4-2014_11_07/TTA_TCF_SingleInstanceTA_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_Time/TTA_Time/code_files/TTA_Time_protocol.h,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_Time/code_patches/v1_1_0_4-2014_11_07/TTA_Time_protocol.h.patch)
	$(call patch-file,${CFG_GP_PACKAGE_PATH}/TTAs/TTA_Time/TTA_Time/code_files/TTA_Time.c,${CFG_GP_XSL_PACKAGE_PATH}/TTAs/TTA_Time/code_patches/v1_1_0_4-2014_11_07/TTA_Time.c.patch)

define patch-filter-one
	$(q)sed -i 's|^ADBG_SUITE_ENTRY(XTEST_TEE_'${1}', NULL)|/\*ADBG_SUITE_ENTRY(XTEST_TEE_'${1}', NULL)\*/|g' ${GP_XTEST_OUT_DIR}/xtest_main.c
	$(q)sed -i 's|    ADBG_SUITE_ENTRY(XTEST_TEE_'${1}', NULL)\\|    /\*ADBG_SUITE_ENTRY(XTEST_TEE_'${1}', NULL)\*/\\|g' ${GP_XTEST_OUT_DIR}/adbg_entry_declare.h
endef

.PHONY: patch-filter
patch-filter:
	@echo "INFO: Filter some tests"
	$(call patch-filter-one,7038)
	$(call patch-filter-one,7522)
	$(call patch-filter-one,7538)
	$(call patch-filter-one,7540)
	$(call patch-filter-one,7546)
	$(call patch-filter-one,7557)
	$(call patch-filter-one,7522)
	$(call patch-filter-one,7538)
	$(call patch-filter-one,7540)
	$(call patch-filter-one,7546)
	$(call patch-filter-one,7557)
	$(call patch-filter-one,7559)
	$(call patch-filter-one,7577)
	$(call patch-filter-one,7641)
	$(call patch-filter-one,7642)
	$(call patch-filter-one,7643)
	$(call patch-filter-one,7644)
	$(call patch-filter-one,7686)
	$(call patch-filter-one,8025)
	$(call patch-filter-one,8058)
	$(call patch-filter-one,8059)
	$(call patch-filter-one,8030)
	$(call patch-filter-one,8066)
	$(call patch-filter-one,8614)
	$(call patch-filter-one,8643)
	$(call patch-filter-one,8644)
	$(call patch-filter-one,8673)
	$(call patch-filter-one,8674)
	$(call patch-filter-one,9001)
	$(call patch-filter-one,9072)
	$(call patch-filter-one,9073)
	$(call patch-filter-one,9075)
	$(call patch-filter-one,9079)
	$(call patch-filter-one,9080)
	$(call patch-filter-one,9082)
	$(call patch-filter-one,9085)
	$(call patch-filter-one,9086)
	$(call patch-filter-one,9088)
	$(call patch-filter-one,9090)
	$(call patch-filter-one,9091)
	$(call patch-filter-one,9093)
	$(call patch-filter-one,9095)
	$(call patch-filter-one,9096)
	$(call patch-filter-one,9098)
	$(call patch-filter-one,9099)
	$(call patch-filter-one,9109)
	$(call patch-filter-one,9110)
	$(call patch-filter-one,9160)
	$(call patch-filter-one,9174)
	$(call patch-filter-one,9195)
	$(call patch-filter-one,9196)
	$(call patch-filter-one,9204)
	$(call patch-filter-one,9239)

.PHONY: patch
patch: patch-openssl patch-generate-host patch-generate-ta
	$(MAKE) patch-filter

else
.PHONY: patch
patch:
	$(q) echo "Please define CFG_GP_PACKAGE_PATH" && false
endif
