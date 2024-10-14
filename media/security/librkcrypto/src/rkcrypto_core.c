/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "cryptodev.h"
#include "rk_cryptodev.h"
#include "rkcrypto_mem.h"
#include "rkcrypto_core.h"
#include "rkcrypto_core_int.h"
#include "rkcrypto_trace.h"
#include "rkcrypto_rsa_helper.h"
#include "rk_list.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({                      \
        const typeof(((type *) 0)->member) *__mptr = (ptr);     \
        (type *) ((char *) __mptr - offsetof(type, member));})
#endif

#define rkop_to_cop(rkop)	\
	((rkop == RK_OP_CIPHER_ENC) ? COP_ENCRYPT : COP_DECRYPT)

enum RK_CRYPTO_CONFIG_TYPE {
	RK_CONFIG_TYPE_CIPHER = 0,
	RK_CONFIG_TYPE_AE,
	RK_CONFIG_TYPE_HASH,
};

struct algo_map_info {
	uint32_t	algo;
	uint32_t	mode;
	uint32_t	crypto_id;
};

struct hash_result {
	uint8_t		hash[AALG_MAX_RESULT_LEN];
	uint32_t	len;
};

struct sess_id_node {
	uint32_t		sess_id;
	uint32_t		config_type;
	void			*priv;

	union {
		rk_cipher_config	cipher;
		rk_ae_config		ae;
		rk_hash_config		hash;
	} config;

	struct list_head	list;
};

struct ae_node_priv {
	void		*iv;
	uint8_t		*aad_virt;
	int		aad_fd;
};

static int cryptodev_fd = -1;
static int cryptodev_refcnt;

static struct list_head sess_id_list;
pthread_mutex_t sess_mutex = PTHREAD_MUTEX_INITIALIZER;

#define CHECK_CRYPTO_INITED()	do {\
					if (cryptodev_fd < 0) {\
						E_TRACE("RK_CRYPTO is uninitialized\n");\
						return RK_CRYPTO_ERR_UNINITED;\
					}\
				} while (0)

static const struct {
	const uint32_t kernel_code;
	const uint32_t rk_crypto_code;
} kernel_crypto_code[] = {
	{0,			RK_CRYPTO_SUCCESS},
	{-EINVAL,		RK_CRYPTO_ERR_PARAMETER},
	{-ENOENT,		RK_CRYPTO_ERR_NOT_SUPPORTED},
	{-ENOMEM,		RK_CRYPTO_ERR_OUT_OF_MEMORY},
	{-EACCES,		RK_CRYPTO_ERR_ACCESS_DENIED},
	{-EBUSY,		RK_CRYPTO_ERR_BUSY},
	{-ETIMEDOUT,		RK_CRYPTO_ERR_TIMEOUT},
	{-ENOKEY,		RK_CRYPTO_ERR_KEY},
	{-EBADMSG,		RK_CRYPTO_ERR_MAC_INVALID},
};

static RK_RES kernel_to_crypto_code(uint32_t tee_code)
{
	uint32_t i;

	for (i = 0; i < ARRAY_SIZE(kernel_crypto_code); i++) {
		if (tee_code == kernel_crypto_code[i].kernel_code)
			return kernel_crypto_code[i].rk_crypto_code;
	}

	/* Others convert to RK_CRYPTO_ERR_GENERIC. */
	return RK_CRYPTO_ERR_GENERIC;
}

const struct algo_map_info algo_map_tbl[] = {
	{RK_ALGO_DES,  RK_CIPHER_MODE_ECB,     CRYPTO_RK_DES_ECB},
	{RK_ALGO_DES,  RK_CIPHER_MODE_CBC,     CRYPTO_RK_DES_CBC},
	{RK_ALGO_DES,  RK_CIPHER_MODE_CFB,     CRYPTO_RK_DES_CFB},
	{RK_ALGO_DES,  RK_CIPHER_MODE_OFB,     CRYPTO_RK_DES_OFB},

	{RK_ALGO_TDES, RK_CIPHER_MODE_ECB,     CRYPTO_RK_3DES_ECB},
	{RK_ALGO_TDES, RK_CIPHER_MODE_CBC,     CRYPTO_RK_3DES_CBC},
	{RK_ALGO_TDES, RK_CIPHER_MODE_CFB,     CRYPTO_RK_3DES_CFB},
	{RK_ALGO_TDES, RK_CIPHER_MODE_OFB,     CRYPTO_RK_3DES_OFB},

	{RK_ALGO_SM4,  RK_CIPHER_MODE_ECB,     CRYPTO_RK_SM4_ECB},
	{RK_ALGO_SM4,  RK_CIPHER_MODE_CBC,     CRYPTO_RK_SM4_CBC},
	{RK_ALGO_SM4,  RK_CIPHER_MODE_CFB,     CRYPTO_RK_SM4_CFB},
	{RK_ALGO_SM4,  RK_CIPHER_MODE_OFB,     CRYPTO_RK_SM4_OFB},
	{RK_ALGO_SM4,  RK_CIPHER_MODE_CTS,     CRYPTO_RK_SM4_CTS},
	{RK_ALGO_SM4,  RK_CIPHER_MODE_CTR,     CRYPTO_RK_SM4_CTR},
	{RK_ALGO_SM4,  RK_CIPHER_MODE_XTS,     CRYPTO_RK_SM4_XTS},
	{RK_ALGO_SM4,  RK_CIPHER_MODE_CCM,     CRYPTO_RK_SM4_CCM},
	{RK_ALGO_SM4,  RK_CIPHER_MODE_GCM,     CRYPTO_RK_SM4_GCM},

	{RK_ALGO_AES,  RK_CIPHER_MODE_ECB,     CRYPTO_RK_AES_ECB},
	{RK_ALGO_AES,  RK_CIPHER_MODE_CBC,     CRYPTO_RK_AES_CBC},
	{RK_ALGO_AES,  RK_CIPHER_MODE_CFB,     CRYPTO_RK_AES_CFB},
	{RK_ALGO_AES,  RK_CIPHER_MODE_OFB,     CRYPTO_RK_AES_OFB},
	{RK_ALGO_AES,  RK_CIPHER_MODE_CTS,     CRYPTO_RK_AES_CTS},
	{RK_ALGO_AES,  RK_CIPHER_MODE_CTR,     CRYPTO_RK_AES_CTR},
	{RK_ALGO_AES,  RK_CIPHER_MODE_XTS,     CRYPTO_RK_AES_XTS},
	{RK_ALGO_AES,  RK_CIPHER_MODE_CCM,     CRYPTO_RK_AES_CCM},
	{RK_ALGO_AES,  RK_CIPHER_MODE_GCM,     CRYPTO_RK_AES_GCM},

	{RK_ALGO_MD5,         0, CRYPTO_RK_MD5},
	{RK_ALGO_SHA1,        0, CRYPTO_RK_SHA1},
	{RK_ALGO_SHA224,      0, CRYPTO_RK_SHA224},
	{RK_ALGO_SHA256,      0, CRYPTO_RK_SHA256},
	{RK_ALGO_SHA384,      0, CRYPTO_RK_SHA384},
	{RK_ALGO_SHA512,      0, CRYPTO_RK_SHA512},
	{RK_ALGO_SHA512_224,  0, CRYPTO_RK_SHA512_224},
	{RK_ALGO_SHA512_256,  0, CRYPTO_RK_SHA512_256},
	{RK_ALGO_SM3,         0, CRYPTO_RK_SM3},

	{RK_ALGO_HMAC_MD5,    0, CRYPTO_RK_MD5_HMAC},
	{RK_ALGO_HMAC_SHA1,   0, CRYPTO_RK_SHA1_HMAC},
	{RK_ALGO_HMAC_SHA256, 0, CRYPTO_RK_SHA256_HMAC},
	{RK_ALGO_HMAC_SHA512, 0, CRYPTO_RK_SHA512_HMAC},
	{RK_ALGO_HMAC_SM3,    0, CRYPTO_RK_SM3_HMAC},
	{RK_ALGO_CMAC_SM4,    0, CRYPTO_RK_SM4_CMAC},
	{RK_ALGO_CBCMAC_SM4,  0, CRYPTO_RK_SM4_CBC_MAC},
	{RK_ALGO_CMAC_AES,    0, CRYPTO_RK_AES_CMAC},
	{RK_ALGO_CBCMAC_AES,  0, CRYPTO_RK_AES_CBC_MAC},
};

static uint32_t rk_get_hash_len(uint32_t algo)
{
	switch (algo) {
	case RK_ALGO_MD5:
	case RK_ALGO_HMAC_MD5:
		return MD5_HASH_SIZE;
	case RK_ALGO_SHA1:
	case RK_ALGO_HMAC_SHA1:
		return SHA1_HASH_SIZE;
	case RK_ALGO_SHA224:
	case RK_ALGO_SHA512_224:
		return SHA224_HASH_SIZE;
	case RK_ALGO_SHA256:
	case RK_ALGO_SHA512_256:
	case RK_ALGO_HMAC_SHA256:
		return SHA256_HASH_SIZE;
	case RK_ALGO_SHA384:
		return SHA384_HASH_SIZE;
	case RK_ALGO_SHA512:
	case RK_ALGO_HMAC_SHA512:
		return SHA512_HASH_SIZE;
	case RK_ALGO_SM3:
	case RK_ALGO_HMAC_SM3:
		return SM3_HASH_SIZE;
	default:
		return 0;
	}
}

static RK_RES xioctl(int fd, unsigned long int request, void *arg)
{
	return ioctl(fd, request, arg) ? kernel_to_crypto_code(-errno) : RK_CRYPTO_SUCCESS;
}

static RK_RES rk_get_crypto_id(uint32_t algo, uint32_t mode, uint32_t *crypto_id)
{
	uint32_t i;

	for (i = 0; i < ARRAY_SIZE(algo_map_tbl); i++) {
		if (algo == algo_map_tbl[i].algo && mode == algo_map_tbl[i].mode) {
			*crypto_id = algo_map_tbl[i].crypto_id;
			return RK_CRYPTO_SUCCESS;
		}
	}

	return RK_CRYPTO_ERR_GENERIC;
}

static RK_RES rk_add_sess_node(uint32_t sess_id, uint32_t config_type, const void *config, void *priv)
{
	struct sess_id_node *node;

	node = malloc(sizeof(*node));
	if (!node) {
		E_TRACE("malloc node error!\n");
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;
	}

	switch (config_type) {
	case RK_CONFIG_TYPE_CIPHER:
		memcpy(&node->config.cipher, config, sizeof(node->config.cipher));
		break;
	case RK_CONFIG_TYPE_AE:
		memcpy(&node->config.ae, config, sizeof(node->config.ae));
		break;
	case RK_CONFIG_TYPE_HASH:
		memcpy(&node->config.hash, config, sizeof(node->config.hash));
		break;
	default:
		free(node);
		return RK_CRYPTO_ERR_PARAMETER;
	}

	node->sess_id     = sess_id;
	node->priv        = priv;
	node->config_type = config_type;

	pthread_mutex_lock(&sess_mutex);

	list_add_tail(&node->list, &sess_id_list);

	pthread_mutex_unlock(&sess_mutex);

	return RK_CRYPTO_SUCCESS;
}

static struct sess_id_node *rk_get_sess_node(uint32_t sess_id)
{
	struct list_head *pos = NULL;
	struct sess_id_node *node = NULL;

	pthread_mutex_lock(&sess_mutex);

	list_for_each(pos, &sess_id_list) {
		node = list_entry(pos, struct sess_id_node, list);

		if (node->sess_id == sess_id) {
			goto exit;
		}
	}

exit:
	pthread_mutex_unlock(&sess_mutex);
	return node;
}

static void *rk_get_sess_config(uint32_t sess_id)
{
	struct sess_id_node *node;

	node = rk_get_sess_node(sess_id);

	return node ? &node->config : NULL;
}

static RK_RES rk_del_sess_node(uint32_t sess_id)
{
	struct list_head *pos = NULL, *n = NULL;
	RK_RES res = RK_CRYPTO_ERR_GENERIC;

	pthread_mutex_lock(&sess_mutex);

	list_for_each_safe(pos, n, &sess_id_list) {
		struct sess_id_node *node;

		node = list_entry(pos, struct sess_id_node, list);

		if (node->sess_id == sess_id) {
			list_del(pos);
			free(node);
			res = RK_CRYPTO_SUCCESS;
			goto exit;
		}
	}

exit:
	pthread_mutex_unlock(&sess_mutex);
	return res;
}

static RK_RES rk_create_session(struct session_op *sess, uint32_t config_type,
				const void *config, void *priv, rk_handle *handle)
{
	RK_RES res;

	res = xioctl(cryptodev_fd, CIOCGSESSION, sess);
	if (res) {
		if (res != RK_CRYPTO_ERR_NOT_SUPPORTED)
			E_TRACE("CIOCGSESSION error %d!\n", errno);

		goto exit;
	}

	res = rk_add_sess_node(sess->ses, config_type, config, priv);
	if (res) {
		E_TRACE("rk_add_sess_node error[%x]!\n", res);
		xioctl(cryptodev_fd, CIOCFSESSION, &sess->ses);
	} else {
		*handle = sess->ses;
	}
exit:
	return res;
}

static RK_RES rk_destroy_session(rk_handle handle)
{
	RK_RES res;

	res = xioctl(cryptodev_fd, CIOCFSESSION, &handle);
	if (res) {
		E_TRACE("CIOCFSESSION error!");
		return res;
	}

	return rk_del_sess_node(handle);
}

static RK_RES rk_update_user_iv(const void *cfg)
{
	struct sess_id_node *node;
	struct ae_node_priv *ae_priv = NULL;

	node = container_of(cfg, struct sess_id_node, config.cipher);
	if (!node)
		return RK_CRYPTO_ERR_STATE;

	if (node->priv) {
		V_TRACE("update user iv.");

		switch (node->config_type) {
		case RK_CONFIG_TYPE_CIPHER:
			memcpy(node->priv, node->config.cipher.iv, sizeof(node->config.cipher.iv));
			break;
		case RK_CONFIG_TYPE_AE:
			ae_priv = node->priv;
			memcpy(ae_priv->iv, node->config.ae.iv, node->config.ae.iv_len);
			break;
		default:
			break;
		}
	}

	return RK_CRYPTO_SUCCESS;
}

static size_t rk_mpi_bitlen(const uint8_t *data, uint32_t data_len)
{
	uint32_t i;

	while (data_len--) {
		if (*data) {
			uint8_t tmp = *data;

			for (i = 0; i < 8; i++) {
				if (!tmp)
					break;

				tmp = tmp >> 1;
			}

			return data_len * 8 + i;
		}

		data++;
	}

	return 0;
}

RK_RES rk_crypto_init(void)
{
	I_TRACE("%s\n", RK_CRYPTO_API_FULL_VERSION);

	if (cryptodev_fd < 0) {
		rk_crypto_mem_init();

		INIT_LIST_HEAD(&sess_id_list);

		/* Open the crypto device */
		cryptodev_fd = open("/dev/crypto", O_RDWR, 0);
		if (cryptodev_fd < 0) {
			E_TRACE("open cryptodev error!\n");
			return kernel_to_crypto_code(-errno);
		}

		/* Set close-on-exec (not really neede here) */
		if (fcntl(cryptodev_fd, F_SETFD, 1) == -1) {
			E_TRACE("cryptodev F_SETFD error!\n");
			goto error;
		}
	}

	cryptodev_refcnt++;

	return RK_CRYPTO_SUCCESS;
error:
	if (cryptodev_fd >= 0)
		close(cryptodev_fd);

	return RK_CRYPTO_ERR_GENERIC;
}

void rk_crypto_deinit(void)
{
	if (--cryptodev_refcnt == 0 && cryptodev_fd >= 0) {
		/* free sess_id list */
		struct list_head *pos = NULL, *n = NULL;

		pthread_mutex_lock(&sess_mutex);

		list_for_each_safe(pos, n, &sess_id_list) {
			struct sess_id_node *node;

			node = list_entry(pos, struct sess_id_node, list);
			list_del(pos);
			free(node);
		}

		pthread_mutex_unlock(&sess_mutex);

		close(cryptodev_fd);
		cryptodev_fd = -1;
		rk_crypto_mem_deinit();

	}

	if (cryptodev_refcnt < 0)
		cryptodev_refcnt = 0;
}

RK_RES rk_cipher_init(const rk_cipher_config *config, rk_handle *handle)
{
	RK_RES res;
	struct session_op sess;
	uint32_t crypto_id = 0;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(!config || !handle);

	memset(&sess, 0, sizeof(sess));

	res = rk_get_crypto_id(config->algo, config->mode, &crypto_id);
	if (res) {
		E_TRACE("rk_get_crypto_id error!\n");
		return res;
	}

	sess.cipher = crypto_id;
	sess.key    = (__u8 *)config->key;
	sess.keylen = config->key_len;

	return rk_create_session(&sess, RK_CONFIG_TYPE_CIPHER,
				 config, (void *)config->iv, handle);
}

RK_RES rk_cipher_crypt(rk_handle handle, int in_fd, int out_fd, uint32_t len)
{
	struct crypt_fd_op cryp;
	rk_cipher_config *cipher_cfg;
	RK_RES res = RK_CRYPTO_ERR_GENERIC;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(len == 0);

	cipher_cfg = rk_get_sess_config(handle);
	if (!cipher_cfg) {
		E_TRACE("rk_get_sess_config error!\n");
		return RK_CRYPTO_ERR_STATE;
	}

	memset(&cryp, 0, sizeof(cryp));

	/* Encrypt data.in to data.encrypted */
	cryp.ses    = handle;
	cryp.len    = len;
	cryp.src_fd = in_fd;
	cryp.dst_fd = out_fd;
	cryp.iv     = (void *)cipher_cfg->iv;
	cryp.op     = rkop_to_cop(cipher_cfg->operation);
	cryp.flags  = COP_FLAG_WRITE_IV;

	res = xioctl(cryptodev_fd, RIOCCRYPT_FD, &cryp);
	if (res) {
		E_TRACE("RIOCCRYPT_FD error!\n");
		goto exit;
	}

	res = rk_update_user_iv(cipher_cfg);
	if (res) {
		E_TRACE("rk_update_user_iv error!\n");
		goto exit;
	}

exit:
	return res;
}

RK_RES rk_cipher_crypt_virt(rk_handle handle, const uint8_t *in, uint8_t *out, uint32_t len)
{
	struct crypt_op cryp;
	rk_cipher_config *cipher_cfg;
	RK_RES res = RK_CRYPTO_ERR_GENERIC;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(!in || !out || len == 0);

	cipher_cfg = rk_get_sess_config(handle);
	if (!cipher_cfg) {
		E_TRACE("rk_get_sess_config error!\n");
		return RK_CRYPTO_ERR_STATE;
	}

	memset(&cryp, 0, sizeof(cryp));

	/* Encrypt data.in to data.encrypted */
	cryp.ses   = handle;
	cryp.len   = len;
	cryp.src   = (void *)in;
	cryp.dst   = out;
	cryp.iv    = (void *)cipher_cfg->iv;
	cryp.op    = rkop_to_cop(cipher_cfg->operation);
	cryp.flags = COP_FLAG_WRITE_IV;

	res = xioctl(cryptodev_fd, CIOCCRYPT, &cryp);
	if (res) {
		E_TRACE("CIOCCRYPT error!\n");
		goto exit;
	}

	res = rk_update_user_iv(cipher_cfg);
	if (res) {
		E_TRACE("rk_update_user_iv error!\n");
		goto exit;
	}

exit:
	return res;
}

RK_RES rk_cipher_final(rk_handle handle)
{
	CHECK_CRYPTO_INITED();

	return rk_destroy_session(handle);
}

RK_RES rk_ae_init(const rk_ae_config *config, rk_handle *handle)
{
	RK_RES res;
	struct session_op sess;
	uint32_t crypto_id = 0;
	struct ae_node_priv *ae_priv = NULL;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(!config || !handle);

	memset(&sess, 0, sizeof(sess));

	res = rk_get_crypto_id(config->algo, config->mode, &crypto_id);
	if (res) {
		E_TRACE("rk_get_crypto_id error!\n");
		return res;
	}

	sess.cipher = crypto_id;
	sess.key    = (__u8 *)config->key;
	sess.keylen = config->key_len;

	ae_priv = calloc(1, sizeof(*ae_priv));
	if (!ae_priv) {
		E_TRACE("malloc ae_priv error!\n");
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;
	}

	ae_priv->iv = (void *)config->iv;

	return rk_create_session(&sess, RK_CONFIG_TYPE_AE,
				 config, (void *)ae_priv, handle);
}

static RK_RES ae_set_aad(rk_handle handle, int aad_fd, uint8_t *aad_virt)
{
	struct sess_id_node *node = NULL;
	struct ae_node_priv *ae_priv = NULL;

	node = rk_get_sess_node(handle);
	if (!node) {
		E_TRACE("rk_get_sess_node error!\n");
		return RK_CRYPTO_ERR_STATE;
	}

	ae_priv = node->priv;

	if (aad_fd)
		ae_priv->aad_fd = aad_fd;
	if (aad_virt)
		ae_priv->aad_virt = aad_virt;

	return RK_CRYPTO_SUCCESS;
}

RK_RES rk_ae_set_aad(rk_handle handle, int aad_fd)
{
	return ae_set_aad(handle, aad_fd, NULL);
}

RK_RES rk_ae_set_aad_virt(rk_handle handle, uint8_t *aad_virt)
{
	return ae_set_aad(handle, 0, aad_virt);
}

RK_RES rk_ae_crypt(rk_handle handle, int in_fd, int out_fd, uint32_t len, uint8_t *tag)
{
	struct crypt_auth_fd_op op;
	rk_ae_config *ae_cfg = NULL;
	struct sess_id_node *node = NULL;
	struct ae_node_priv *ae_priv = NULL;
	RK_RES res = RK_CRYPTO_ERR_GENERIC;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(len == 0);

	node = rk_get_sess_node(handle);
	if (!node) {
		E_TRACE("rk_get_sess_node error!\n");
		return RK_CRYPTO_ERR_STATE;
	}

	ae_priv =  node->priv;

	/* make sure rk_ae_set_aad has been called */
	RK_CRYPTO_CHECK_PARAM(ae_priv->aad_fd == 0);

	ae_cfg = rk_get_sess_config(handle);
	if (!ae_cfg) {
		E_TRACE("rk_get_sess_config error!\n");
		return RK_CRYPTO_ERR_STATE;
	}

	if (ae_cfg->tag_len)
		RK_CRYPTO_CHECK_PARAM(!tag);

	memset(&op, 0, sizeof(op));

	/* Encrypt data.in to data.encrypted */
	op.ses      = handle;
	op.op       = rkop_to_cop(ae_cfg->operation);
	op.flags    = COP_FLAG_WRITE_IV | COP_FLAG_AEAD_RK_TYPE;
	op.len      = len;
	op.src_fd   = in_fd;
	op.dst_fd   = out_fd;
	op.iv       = (unsigned long)ae_cfg->iv | (__u64)0;
	op.iv_len   = ae_cfg->iv_len;
	op.auth_fd  = ae_priv->aad_fd;
	op.auth_len = ae_cfg->aad_len;
	op.tag_len  = ae_cfg->tag_len;
	op.tag      = (unsigned long)tag | (__u64)0;

	V_TRACE("in and out are %s address.", (in_fd == out_fd) ? "the same" : "different");
	V_TRACE("cipher data len = %d, aad_len = %d, tag_len = %d",
		len, ae_cfg->aad_len, ae_cfg->tag_len);

	res = xioctl(cryptodev_fd, RIOCAUTHCRYPT_FD, &op);
	if (res) {
		E_TRACE("RIOCCRYPT_FD error!\n");
		goto exit;
	}

	res = rk_update_user_iv(ae_cfg);
	if (res) {
		E_TRACE("rk_update_user_iv error[0x%x]!\n", res);
		goto exit;
	}

exit:
	return res;
}

RK_RES rk_ae_crypt_virt(rk_handle handle, const uint8_t *in, uint8_t *out, uint32_t len,
			uint8_t *tag)
{
	struct crypt_auth_op op;
	rk_ae_config *ae_cfg = NULL;
	struct sess_id_node *node = NULL;
	struct ae_node_priv *ae_priv = NULL;
	RK_RES res = RK_CRYPTO_ERR_GENERIC;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(!in || !out || len == 0);

	node = rk_get_sess_node(handle);
	if (!node) {
		E_TRACE("rk_get_sess_node error!\n");
		return RK_CRYPTO_ERR_STATE;
	}

	ae_priv =  node->priv;

	/* make sure rk_ae_set_aad_virt has been called */
	RK_CRYPTO_CHECK_PARAM(ae_priv->aad_virt == 0);

	ae_cfg = rk_get_sess_config(handle);
	if (!ae_cfg) {
		E_TRACE("rk_get_sess_config error!\n");
		return RK_CRYPTO_ERR_STATE;
	}

	memset(&op, 0, sizeof(op));

	/* Encrypt data.in to data.encrypted */
	op.ses      = handle;
	op.op       = rkop_to_cop(ae_cfg->operation);
	op.flags    = COP_FLAG_WRITE_IV | COP_FLAG_AEAD_RK_TYPE;
	op.len      = len;
	op.src      = (void *)in;
	op.dst      = out;
	op.iv       = (void *)ae_cfg->iv;
	op.iv_len   = ae_cfg->iv_len;
	op.auth_src = ae_priv->aad_virt;
	op.auth_len = ae_cfg->aad_len;
	op.tag_len  = ae_cfg->tag_len;
	op.tag      = tag;

	V_TRACE("in and out are %s address.", (in == out) ? "the same" : "different");
	V_TRACE("cipher data len = %d, aad_len = %d, tag_len = %d",
		len, ae_cfg->aad_len, ae_cfg->tag_len);

	res = xioctl(cryptodev_fd, CIOCAUTHCRYPT, &op);
	if (res) {
		E_TRACE("CIOCAUTHCRYPT error!\n");
		goto exit;
	}

	res = rk_update_user_iv(ae_cfg);
	if (res) {
		E_TRACE("rk_update_user_iv error[0x%x]!\n", res);
		goto exit;
	}

exit:
	return res;
}

RK_RES rk_ae_final(rk_handle handle)
{
	struct sess_id_node *node = NULL;

	CHECK_CRYPTO_INITED();

	node = rk_get_sess_node(handle);
	if (!node) {
		E_TRACE("rk_get_sess_node error!\n");
		return RK_CRYPTO_ERR_STATE;
	}

	free(node->priv);

	return rk_destroy_session(handle);
}

RK_RES rk_hash_init(const rk_hash_config *config, rk_handle *handle)
{
	RK_RES res;
	struct session_op sess;
	uint32_t crypto_id = 0;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(!config || !handle);

	memset(&sess, 0, sizeof(sess));

	res = rk_get_crypto_id(config->algo, 0, &crypto_id);
	if (res) {
		E_TRACE("rk_get_crypto_id error!\n");
		return res;
	}

	sess.mac = crypto_id;
	if (config->key && config->key_len) {
		sess.mackey    = config->key;
		sess.mackeylen = config->key_len;
	}

	return rk_create_session(&sess, RK_CONFIG_TYPE_HASH, config, NULL, handle);
}

RK_RES rk_hash_update(rk_handle handle, int data_fd, uint32_t data_len)
{
	struct crypt_fd_op cryp;
	RK_RES res;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(data_len == 0);

	memset(&cryp, 0, sizeof(cryp));

	cryp.ses    = handle;
	cryp.len    = data_len;
	cryp.src_fd = data_fd;
	cryp.mac    = NULL;
	cryp.flags  = COP_FLAG_UPDATE;

	res = xioctl(cryptodev_fd, RIOCCRYPT_FD, &cryp);
	if (res) {
		E_TRACE("RIOCCRYPT_FD error!\n");
		return res;
	}

	return RK_CRYPTO_SUCCESS;
}

RK_RES rk_hash_update_virt(rk_handle handle, const uint8_t *data, uint32_t data_len)
{
	struct crypt_op cryp;
	RK_RES res;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(!data || data_len == 0);

	memset(&cryp, 0, sizeof(cryp));

	cryp.ses   = handle;
	cryp.len   = data_len;
	cryp.src   = (void *)data;
	cryp.mac   = NULL;
	cryp.flags = COP_FLAG_UPDATE;

	res = xioctl(cryptodev_fd, CIOCCRYPT, &cryp);
	if (res) {
		E_TRACE("CIOCCRYPT error!\n");
		return res;
	}

	return RK_CRYPTO_SUCCESS;
}

RK_RES rk_hash_final(rk_handle handle, uint8_t *hash)
{
	RK_RES res = RK_CRYPTO_SUCCESS;
	struct crypt_op cryp;
	rk_hash_config *hash_cfg;
	uint8_t hash_tmp[SHA512_HASH_SIZE];
	uint32_t hash_tmp_len;

	CHECK_CRYPTO_INITED();

	hash_cfg = rk_get_sess_config(handle);
	if (!hash_cfg) {
		E_TRACE("handle[%u] rk_get_sess_config  error!\n", handle);
		goto exit;
	}

	hash_tmp_len = rk_get_hash_len(hash_cfg->algo);

	/* final update 0 Byte */
	memset(&cryp, 0, sizeof(cryp));

	cryp.ses   = handle;
	cryp.mac   = hash_tmp;
	cryp.flags = COP_FLAG_FINAL;

	res = xioctl(cryptodev_fd, CIOCCRYPT, &cryp);
	if (res) {
		E_TRACE("CIOCCRYPT error!\n");
		goto exit;
	}

	if (hash)
		memcpy(hash, hash_tmp, hash_tmp_len);

exit:
	return rk_destroy_session(handle);
}

RK_RES rk_crypto_fd_ioctl(uint32_t request, struct crypt_fd_map_op *mop)
{
	RK_RES res;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(request != RIOCCRYPT_FD_MAP &&
			   request != RIOCCRYPT_FD_UNMAP);
	RK_CRYPTO_CHECK_PARAM(!mop);

	res = xioctl(cryptodev_fd, request, mop);
	if (res) {
		E_TRACE("ioctl cryptodev_fd failed!");
		return res;
	}

	return RK_CRYPTO_SUCCESS;
}

static RK_RES rk_rsa_crypt_common(void *key, uint16_t flag, uint16_t op,
				  const uint8_t *in, uint32_t in_len,
				  uint8_t *out, uint32_t *out_len)
{
	RK_RES res = RK_CRYPTO_ERR_GENERIC;
	uint8_t *asn1_key = NULL;
	uint16_t asn1_key_len = RK_RSA_BER_KEY_MAX, key_bits;
	struct crypt_rsa_op rop;

	CHECK_CRYPTO_INITED();

	RK_CRYPTO_CHECK_PARAM(!key);

	asn1_key = malloc(asn1_key_len);
	if (!asn1_key) {
		E_TRACE("ioctl cryptodev_fd failed!");
		return RK_CRYPTO_ERR_OUT_OF_MEMORY;
	}

	memset(asn1_key, 0x00, asn1_key_len);

	/* Encode the key in ASN1 format */
	if ((flag & COP_FLAG_RSA_PRIV) == COP_FLAG_RSA_PRIV)
		res = rk_rsa_privkey_encode((rk_rsa_priv_key_pack *)key,
					    asn1_key, &asn1_key_len, &key_bits);
	else
		res = rk_rsa_pubkey_encode((rk_rsa_pub_key_pack *)key,
					   asn1_key, &asn1_key_len, &key_bits);

	if (res) {
		E_TRACE("asn1 encode failed %x!", res);
		res = RK_CRYPTO_ERR_KEY;
		goto exit;
	}

	if (key_bits / 8 != in_len) {
		E_TRACE("RIOCCRYPT_RSA_CRYPT length not match!");
		res = RK_CRYPTO_ERR_PARAMETER;
		goto exit;
	}

	memset(&rop, 0x00, sizeof(rop));

	rop.op      = op;
	rop.flags   = flag;
	rop.key     = (unsigned long)asn1_key | (__u64)0;
	rop.key_len = asn1_key_len;
	rop.in      = (unsigned long)in | (__u64)0;
	rop.in_len  = in_len;
	rop.out     = (unsigned long)out | (__u64)0;

	res = xioctl(cryptodev_fd, RIOCCRYPT_RSA_CRYPT, &rop);
	if (res) {
		E_TRACE("ioctl cryptodev_fd failed! [%d]", -errno);
		goto exit;
	}

	*out_len = rop.out_len;
	res = RK_CRYPTO_SUCCESS;
exit:
	memset(asn1_key, 0x00, asn1_key_len);

	free(asn1_key);

	return res;
}

RK_RES rk_rsa_pub_encrypt(const rk_rsa_pub_key_pack *pub, enum RK_RSA_CRYPT_PADDING padding,
			  const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len)
{
	RK_RES res;
	uint8_t data_pad[MAX_RSA_KEY_BITS / 8];
	uint32_t data_pad_len = sizeof(data_pad);

	RK_CRYPTO_CHECK_PARAM(!pub || !in || !out || !out_len);
	RK_CRYPTO_CHECK_PARAM(in_len == 0);

	/* deal with padding */
	res = rk_rsa_crypt_do_padding(padding, pub->key.n_len, false,
				      in, in_len, data_pad, &data_pad_len);
	if (res) {
		D_TRACE("rsa padding %d error!", padding);
		return res;
	}

	return rk_rsa_crypt_common((void *)pub, COP_FLAG_RSA_PUB, AOP_ENCRYPT,
				   data_pad, data_pad_len, out, out_len);
}

RK_RES rk_rsa_priv_decrypt(const rk_rsa_priv_key_pack *priv, enum RK_RSA_CRYPT_PADDING padding,
			   const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len)
{
	RK_RES res;
	uint8_t data_pad[MAX_RSA_KEY_BITS / 8];
	uint32_t data_pad_len = sizeof(data_pad);

	RK_CRYPTO_CHECK_PARAM(!priv || !in || !out || !out_len);
	RK_CRYPTO_CHECK_PARAM(priv->key.n_len != in_len);

	res = rk_rsa_crypt_common((void *)priv, COP_FLAG_RSA_PRIV, AOP_DECRYPT,
				   in, in_len, data_pad, &data_pad_len);
	if (res) {
		D_TRACE("rk_rsa_crypt_common error! %x", res);
		return res;
	}

	return rk_rsa_crypt_undo_padding(padding, priv->key.n_len, true,
					 data_pad, data_pad_len, out, out_len);
}

RK_RES rk_rsa_priv_encrypt(const rk_rsa_priv_key_pack *priv, enum RK_RSA_CRYPT_PADDING padding,
			   const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len)
{
	RK_RES res;
	uint8_t data_pad[MAX_RSA_KEY_BITS / 8];
	uint32_t data_pad_len = sizeof(data_pad);

	RK_CRYPTO_CHECK_PARAM(!priv || !in || !out || !out_len);
	RK_CRYPTO_CHECK_PARAM(in_len == 0);

	/* deal with padding */
	res = rk_rsa_crypt_do_padding(padding, priv->key.n_len, true,
				      in, in_len, data_pad, &data_pad_len);
	if (res) {
		D_TRACE("rsa padding %d error!", padding);
		return res;
	}

	return rk_rsa_crypt_common((void *)priv, COP_FLAG_RSA_PRIV, AOP_DECRYPT,
				   data_pad, data_pad_len, out, out_len);
}

RK_RES rk_rsa_pub_decrypt(const rk_rsa_pub_key_pack *pub, enum RK_RSA_CRYPT_PADDING padding,
			  const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len)
{
	RK_RES res;
	uint8_t data_pad[MAX_RSA_KEY_BITS / 8];
	uint32_t data_pad_len = sizeof(data_pad);

	RK_CRYPTO_CHECK_PARAM(!pub || !in || !out || !out_len);
	RK_CRYPTO_CHECK_PARAM(pub->key.n_len != in_len);

	res = rk_rsa_crypt_common((void *)pub, COP_FLAG_RSA_PUB, AOP_ENCRYPT,
				   in, in_len, data_pad, &data_pad_len);
	if (res) {
		D_TRACE("rk_rsa_crypt_common error! %x", res);
		return res;
	}

	return rk_rsa_crypt_undo_padding(padding, pub->key.n_len, false,
					 data_pad, data_pad_len, out, out_len);
}

RK_RES rk_rsa_sign(const rk_rsa_priv_key_pack *priv, enum RK_RSA_SIGN_PADDING padding,
		   const uint8_t *in, uint32_t in_len, const uint8_t *hash,
		   uint8_t *out, uint32_t *out_len)
{
	RK_RES res;
	uint8_t data_pad[MAX_RSA_KEY_BITS / 8];
	uint32_t data_pad_len = sizeof(data_pad);

	RK_CRYPTO_CHECK_PARAM(!in && !hash);
	RK_CRYPTO_CHECK_PARAM(in && in_len == 0);
	RK_CRYPTO_CHECK_PARAM(!priv || !out || !out_len);

	/* deal with padding */
	res = rk_rsa_sign_do_padding(padding, priv->key.n_len,
				     rk_mpi_bitlen(priv->key.n, priv->key.n_len),
				     in, in_len, hash, data_pad, &data_pad_len);
	if (res) {
		D_TRACE("rsa padding %d error!", padding);
		return res;
	}

	return rk_rsa_crypt_common((void *)priv, COP_FLAG_RSA_PRIV, AOP_DECRYPT,
				   data_pad, data_pad_len, out, out_len);
}

RK_RES rk_rsa_verify(const rk_rsa_pub_key_pack *pub, enum RK_RSA_SIGN_PADDING padding,
		     const uint8_t *in, uint32_t in_len, const uint8_t *hash,
		     uint8_t *sign, uint32_t sign_len)
{
	RK_RES res;
	uint8_t dec_pad[MAX_RSA_KEY_BITS / 8];
	uint32_t dec_pad_len = sizeof(dec_pad);

	RK_CRYPTO_CHECK_PARAM(!in && !hash);
	RK_CRYPTO_CHECK_PARAM(in && in_len == 0);
	RK_CRYPTO_CHECK_PARAM(!pub || !sign);
	RK_CRYPTO_CHECK_PARAM(pub->key.n_len != sign_len);

	res = rk_rsa_crypt_common((void *)pub, COP_FLAG_RSA_PUB, AOP_ENCRYPT,
				  sign, sign_len, dec_pad, &dec_pad_len);
	if (res) {
		D_TRACE("rsa rk_rsa_crypt_common error[%x]!", res);
		goto exit;
	}

	/* deal with padding */
	res = rk_rsa_sign_undo_padding(padding, pub->key.n_len,
				       rk_mpi_bitlen(pub->key.n, pub->key.n_len),
				       in, in_len, hash, dec_pad);
	if (res) {
		D_TRACE("rsa padding %d error!", padding);
		goto exit;
	}

exit:
	return res ? RK_CRYPTO_ERR_VERIFY : RK_CRYPTO_SUCCESS;
}
