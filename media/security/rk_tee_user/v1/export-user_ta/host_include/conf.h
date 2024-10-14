#ifndef _out_arm_plat_rockchip_include_generated_conf_h_
#define _out_arm_plat_rockchip_include_generated_conf_h_
#define CFG_ARM32_ta_arm32 1 /* 'y' */
#define CFG_ARM64_core 1 /* 'y' */
#define CFG_ARM64_ta_arm64 1 /* 'y' */
#define CFG_BCH_SUPPORT 1 /* 'y' */
#define CFG_CORE_CLUSTER_SHIFT 2 /* '2' */
#define CFG_CORE_WORKAROUND_SPECTRE_BP 1 /* 'y' */
#define CFG_CORE_WORKAROUND_SPECTRE_BP_SEC 1 /* 'y' */
#define CFG_CRYPTO 1 /* 'y' */
#define CFG_CRYPTO_AES 1 /* 'y' */
#define CFG_CRYPTO_CBC 1 /* 'y' */
#define CFG_CRYPTO_CBC_MAC 1 /* 'y' */
#define CFG_CRYPTO_CCM 1 /* 'y' */
#define CFG_CRYPTO_CMAC 1 /* 'y' */
#define CFG_CRYPTO_CONCAT_KDF 1 /* 'y' */
#define CFG_CRYPTO_CTR 1 /* 'y' */
#define CFG_CRYPTO_CTS 1 /* 'y' */
#define CFG_CRYPTO_DES 1 /* 'y' */
#define CFG_CRYPTO_DH 1 /* 'y' */
#define CFG_CRYPTO_DSA 1 /* 'y' */
#define CFG_CRYPTO_ECB 1 /* 'y' */
#define CFG_CRYPTO_ECC 1 /* 'y' */
#define CFG_CRYPTO_GCM 1 /* 'y' */
#define CFG_CRYPTO_HKDF 1 /* 'y' */
#define CFG_CRYPTO_HMAC 1 /* 'y' */
#define CFG_CRYPTO_MD5 1 /* 'y' */
#define CFG_CRYPTO_PBKDF2 1 /* 'y' */
#define CFG_CRYPTO_RSA 1 /* 'y' */
#define CFG_CRYPTO_SHA1 1 /* 'y' */
#define CFG_CRYPTO_SHA224 1 /* 'y' */
#define CFG_CRYPTO_SHA256 1 /* 'y' */
#define CFG_CRYPTO_SHA384 1 /* 'y' */
#define CFG_CRYPTO_SHA512 1 /* 'y' */
#define CFG_CRYPTO_XTS 1 /* 'y' */
/* CFG_DEBUG is not set ('n') */
#define CFG_ENC_FS 1 /* 'y' */
#define CFG_GENERIC_BOOT 1 /* 'y' */
#define CFG_GIC 1 /* 'y' */
#define CFG_HWSUPP_MEM_PERM_PXN 1 /* 'y' */
#define CFG_KERN_LINKER_ARCH aarch64 /* 'aarch64' */
#define CFG_KERN_LINKER_FORMAT elf64-littleaarch64 /* 'elf64-littleaarch64' */
#define CFG_LIBUTILS_WITH_ISOC 1 /* 'y' */
#define CFG_LTC_OPTEE_THREAD 1 /* 'y' */
#define CFG_NUM_THREADS 2 /* '2' */
#define CFG_OPTEE_REVISION_MAJOR 1 /* '1' */
#define CFG_OPTEE_REVISION_MINOR 1 /* '1' */
#define CFG_OTP_SUPPORT 1 /* 'y' */
#define CFG_PM_STUBS 1 /* 'y' */
#define CFG_REE_FS 1 /* 'y' */
/* CFG_REE_FS_BLOCK_CACHE is not set ('n') */
#define CFG_RK_BOOT 1 /* 'y' */
#define CFG_RK_CORE_RKWVM 1 /* 'y' */
#define CFG_RK_CORE_SECURE_VIDEO 1 /* 'y' */
/* CFG_RK_CRYPTO_AES_DES is not set ('n') */
#define CFG_RK_CRYPTO_BORINGSSL 1 /* 'y' */
/* CFG_RK_CRYPTO_HASH is not set ('n') */
/* CFG_RK_CRYPTO_RSA is not set ('n') */
#define CFG_RK_DEFAULT_RPMB_FS 1 /* 'y' */
#define CFG_RK_EFUSE 1 /* 'y' */
/* CFG_RK_HW_CRYPTO is not set ('n') */
#define CFG_RK_MASK_NATIVE_INTR 1 /* 'y' */
#define CFG_RK_OS_SERVICE 1 /* 'y' */
#define CFG_RK_OTP 1 /* 'y' */
#define CFG_RK_OTP_WITH_DEVICEINFO 1 /* 'y' */
/* CFG_RK_RPMB_READ_ONEBLOCK is not set ('n') */
/* CFG_RK_SEC_EFUSE_OPERATION is not set ('n') */
/* CFG_RK_SEC_OTP_OPERATION is not set ('n') */
/* CFG_RK_TIMER is not set ('n') */
#define CFG_RK_UART 1 /* 'y' */
#define CFG_RK_UBOOT_STORE 1 /* 'y' */
#define CFG_RK_UBOOT_STORE_EFUSE 1 /* 'y' */
#define CFG_RK_UBOOT_WRITE_TOYBRICK_SEED 1 /* 'y' */
#define CFG_RPMB_FS 1 /* 'y' */
/* CFG_RPMB_FS_DEBUG_DATA is not set ('n') */
#define CFG_RPMB_FS_DEV_ID 0 /* '0' */
#define CFG_RPMB_SW_KEY 1 /* 'y' */
#define CFG_SECURE_TIME_SOURCE_CNTPCT 1 /* 'y' */
#define CFG_TA 1 /* 'y' */
/* CFG_TA_DRM is not set ('n') */
#define CFG_TA_FLOAT_SUPPORT 1 /* 'y' */
#define CFG_TEE_API_VERSION GPD-1.1-dev /* 'GPD-1.1-dev' */
#define CFG_TEE_CORE_DEBUG 0 /* '0' */
#define CFG_TEE_CORE_EMBED_INTERNAL_TESTS 1 /* 'y' */
#define CFG_TEE_CORE_LOG_LEVEL 2 /* '2' */
/* CFG_TEE_CORE_MALLOC_DEBUG is not set ('n') */
#define CFG_TEE_CORE_TA_TRACE 1 /* 'y' */
#define CFG_TEE_CORE_USER_MEM_DEBUG 1 /* '1' */
#define CFG_TEE_FS_KEY_MANAGER_TEST 1 /* 'y' */
#define CFG_TEE_FW_IMPL_VERSION FW_IMPL_UNDEF /* 'FW_IMPL_UNDEF' */
#define CFG_TEE_FW_MANUFACTURER FW_MAN_UNDEF /* 'FW_MAN_UNDEF' */
#define CFG_TEE_IMPL_DESCR OPTEE /* 'OPTEE' */
#define CFG_TEE_MANUFACTURER LINARO /* 'LINARO' */
#define CFG_TEE_PANIC_DEBUG 1 /* 'y' */
#define CFG_TEE_TA_LOG_LEVEL 1 /* '1' */
/* CFG_TEE_TA_MALLOC_DEBUG is not set ('n') */
#define CFG_VCODEC_SERVICE 1 /* 'y' */
#define CFG_WITH_ARM_TRUSTED_FW 1 /* 'y' */
#define CFG_WITH_LPAE 1 /* 'y' */
#define CFG_WITH_SOFTWARE_PRNG 1 /* 'y' */
#define CFG_WITH_STACK_CANARIES 1 /* 'y' */
#define CFG_WITH_STATS 1 /* 'y' */
#define CFG_WITH_VFP 1 /* 'y' */
#define _CFG_CRYPTO_WITH_ACIPHER 1 /* 'y' */
#define _CFG_CRYPTO_WITH_ASN1 1 /* 'y' */
#define _CFG_CRYPTO_WITH_AUTHENC 1 /* 'y' */
#define _CFG_CRYPTO_WITH_CBC 1 /* 'y' */
#define _CFG_CRYPTO_WITH_CIPHER 1 /* 'y' */
#define _CFG_CRYPTO_WITH_FORTUNA_PRNG 1 /* 'y' */
#define _CFG_CRYPTO_WITH_HASH 1 /* 'y' */
#define _CFG_CRYPTO_WITH_MAC 1 /* 'y' */
#endif
