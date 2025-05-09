/*
 * Copyright (c) 2025 NeuralSense AI Private Limited
 * Trading as swatah.ai. All rights reserved.
 *
 * This file is part of the swatah.ai software stack and is licensed under
 * the terms defined in the accompanying LICENSE file. Unauthorized copying,
 * distribution, or modification of this file, via any medium, is strictly prohibited.
 *
 * For more information, visit: https://swatah.ai
*/

#ifndef _SAIX_TRIPWIRE_H_
#define _SAIX_TRIPWIRE_H_

#include "rockiva_common.h"
#include "rockiva_ba_api.h"

#ifdef __cplusplus
extern "C" {
#endif

// Event callback typedef
typedef void (*SaixEventCallback)(int rule_id, const char* event_type, const char* json_payload);

// Register callback for events
void saix_register_event_callback(SaixEventCallback cb);

// Send event through the registered callback
void saix_send_event(int rule_id, const char* type, const char* message);

// Get ISO format timestamp
//const char* saix_get_iso_timestamp(void);

// Initialize tripwire functionality - takes no parameters
int init_tripwire(void);

// Update tripwire configuration
int update_tripwire_config(void);

// De-initialize tripwire functionality
int deinit_tripwire(void);

#ifdef __cplusplus
}
#endif

#endif /* _SAIX_TRIPWIRE_H_ */