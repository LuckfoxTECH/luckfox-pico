/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */

#ifndef CRYP_TAF_H
#define CRYP_TAF_H

#include <tee_api.h>

TEE_Result ta_entry_allocate_operation(uint32_t param_type, TEE_Param params[4]
);

TEE_Result ta_entry_free_operation(uint32_t param_type, TEE_Param params[4]
);

TEE_Result ta_entry_get_operation_info(uint32_t param_type, TEE_Param params[4]
);

TEE_Result ta_entry_reset_operation(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_set_operation_key(uint32_t param_type, TEE_Param params[4]
);

TEE_Result ta_entry_set_operation_key2(uint32_t param_type, TEE_Param params[4]
);

TEE_Result ta_entry_copy_operation(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_digest_update(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_digest_do_final(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_cipher_init(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_cipher_update(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_cipher_do_final(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_mac_init(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_mac_update(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_mac_final_compute(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_mac_final_compare(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_allocate_transient_object(uint32_t param_type,
					      TEE_Param params[4]);

TEE_Result ta_entry_free_transient_object(uint32_t param_type,
					  TEE_Param params[4]);

TEE_Result ta_entry_reset_transient_object(uint32_t param_type,
					   TEE_Param params[4]);

TEE_Result ta_entry_populate_transient_object(uint32_t param_type,
					      TEE_Param params[4]);

TEE_Result ta_entry_copy_object_attributes(uint32_t param_type,
					   TEE_Param params[4]);

TEE_Result ta_entry_generate_key(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_asymmetric_encrypt(uint32_t param_type,
				       TEE_Param params[4]);

TEE_Result ta_entry_asymmetric_decrypt(uint32_t param_type,
				       TEE_Param params[4]);

TEE_Result ta_entry_asymmetric_sign_digest(uint32_t param_type,
					   TEE_Param params[4]);

TEE_Result ta_entry_asymmetric_verify_digest(uint32_t param_type,
					     TEE_Param params[4]);

TEE_Result ta_entry_derive_key(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_random_number_generate(uint32_t param_type,
					   TEE_Param params[4]);

TEE_Result ta_entry_ae_init(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_ae_update_aad(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_ae_update(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_ae_encrypt_final(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_ae_decrypt_final(uint32_t param_type, TEE_Param params[4]);

TEE_Result ta_entry_get_object_buffer_attribute(uint32_t param_type,
						TEE_Param params[4]);

TEE_Result ta_entry_get_object_value_attribute(uint32_t param_type,
					       TEE_Param params[4]);

TEE_Result ta_entry_is_algo_supported(uint32_t param_type, TEE_Param params[4]);
#endif /*CRYP_TAF_H */
