// This updated version removes INI parsing from file and uses rk_param_get_* for rule setup.
// Only required parameters are extracted from assumed keys like: rule.0:type, rule.0:line_head etc.
#include "rockiva_common.h"

int configure_rules_from_rkparam(RockIvaHandle handle) {
    RockIvaBaTaskParams taskParams;
    memset(&taskParams, 0, sizeof(taskParams));

    int width = rk_param_get_int("rule.common:normalized_screen_width", 704);
    int height = rk_param_get_int("rule.common:normalized_screen_height", 480);

    // Rule 0: Tripwire
    if (rk_param_get_int("rule.0:enabled", 0)) {
        int rule_id = rk_param_get_int("rule.0:rule_id", 0);
        taskParams.baRules.tripWireRule[rule_id].ruleEnable = 1;
        taskParams.baRules.tripWireRule[rule_id].ruleID = rule_id;
        taskParams.baRules.tripWireRule[rule_id].rulePriority = rk_param_get_int("rule.0:priority", 0);
        taskParams.baRules.tripWireRule[rule_id].sense = rk_param_get_int("rule.0:sense", 80);
        taskParams.baRules.tripWireRule[rule_id].trigerMode = rk_param_get_int("rule.0:trigger_mode", 1);
        taskParams.baRules.tripWireRule[rule_id].event = ROCKIVA_BA_TRIP_EVENT_BOTH;
        taskParams.baRules.tripWireRule[rule_id].objType = rk_param_get_int("rule.0:object_type", 0);

        int head_x = 0, head_y = 0;
        sscanf(rk_param_get_string("rule.0:line_head", "0,0"), "%d,%d", &head_x, &head_y);
        taskParams.baRules.tripWireRule[rule_id].line.head.x = head_x;
        taskParams.baRules.tripWireRule[rule_id].line.head.y = head_y;

        int tail_x = 0, tail_y = 0;
        sscanf(rk_param_get_string("rule.0:line_tail", "0,0"), "%d,%d", &tail_x, &tail_y);
        taskParams.baRules.tripWireRule[rule_id].line.tail.x = tail_x;
        taskParams.baRules.tripWireRule[rule_id].line.tail.y = tail_y;
    }

    // Rule 1: Area In
    if (rk_param_get_int("rule.1:enabled", 0)) {
        int rule_id = rk_param_get_int("rule.1:rule_id", 1);
        taskParams.baRules.areaInRule[rule_id].ruleEnable = 1;
        taskParams.baRules.areaInRule[rule_id].ruleID = rule_id;
        taskParams.baRules.areaInRule[rule_id].sense = rk_param_get_int("rule.1:sense", 70);
        taskParams.baRules.areaInRule[rule_id].alertTime = rk_param_get_int("rule.1:alert_time", 10);
        taskParams.baRules.areaInRule[rule_id].checkEnter = rk_param_get_int("rule.1:check_enter", 1);
        taskParams.baRules.areaInRule[rule_id].objType = rk_param_get_int("rule.1:object_type", 0);

        const char* area_str = rk_param_get_string("rule.1:area", "");
        int x[4], y[4];
        if (sscanf(area_str, "%d,%d,%d,%d,%d,%d,%d,%d", &x[0], &y[0], &x[1], &y[1], &x[2], &y[2], &x[3], &y[3]) == 8) {
            taskParams.baRules.areaInRule[rule_id].area.pointNum = 4;
            for (int i = 0; i < 4; i++) {
                taskParams.baRules.areaInRule[rule_id].area.points[i].x = x[i];
                taskParams.baRules.areaInRule[rule_id].area.points[i].y = y[i];
            }
        }
    }

    RockIvaRetCode ret = ROCKIVA_BA_Init(handle, &taskParams, ba_result_callback);
    if (ret != ROCKIVA_RET_SUCCESS) {
        printf("Failed to initialize BA rules, ret=%d\n", ret);
        return -1;
    }

    return 0;
}

const char* get_object_type_string(uint32_t obj_type) {
    switch (obj_type) {
        case ROCKIVA_OBJECT_TYPE_PERSON:        return "person";
        case ROCKIVA_OBJECT_TYPE_VEHICLE:       return "vehicle";
        case ROCKIVA_OBJECT_TYPE_NON_VEHICLE:   return "non_vehicle";
        case ROCKIVA_OBJECT_TYPE_FACE:          return "face";
        case ROCKIVA_OBJECT_TYPE_HEAD:          return "head";
        case ROCKIVA_OBJECT_TYPE_PET:           return "pet";
        case ROCKIVA_OBJECT_TYPE_MOTORCYCLE:    return "motorcycle";
        case ROCKIVA_OBJECT_TYPE_BICYCLE:       return "bicycle";
        case ROCKIVA_OBJECT_TYPE_PLATE:         return "plate";
        case ROCKIVA_OBJECT_TYPE_BABY:          return "baby";
        case ROCKIVA_OBJECT_TYPE_PACKAGE:       return "package";
        default:                                return "unknown";
    }
}

// Map the RockIvaBaTripEvent direction to a human-readable string
const char* map_tripwire_direction(RockIvaBaTripEvent direction) {
    switch (direction) {
        case ROCKIVA_BA_TRIP_EVENT_BOTH:
            return "bidirectional";
        case ROCKIVA_BA_TRIP_EVENT_DEASIL:
            return "head_to_tail";
        case ROCKIVA_BA_TRIP_EVENT_WIDDERSHINES:
            return "tail_to_head";
        default:
            return "unknown";
    }
}


// Function to fetch camera and client ID

// Callback function for behavior analysis results
void ba_result_callback(const RockIvaBaResult* result, const RockIvaExecuteStatus status, void* userdata) {
    if (status != ROCKIVA_SUCCESS || !g_event_callback || !result) {
        return;
    }

    char camera_id[128];
    char client_id[128];
    fetch_camera_and_client_id(camera_id, sizeof(camera_id), client_id, sizeof(client_id));

    // Process each triggered object
    for (uint32_t i = 0; i < result->objNum; i++) {
        const RockIvaBaObjectInfo* obj = &result->triggerObjects[i];
        
        // If object has triggered rules
        if (obj->triggerRulesNum > 0) {
            const RockIvaBaTrigger* firstTrigger = &obj->firstTrigger;
            
            // Check trigger type and generate appropriate event
            if (firstTrigger->triggerType == ROCKIVA_BA_RULE_CROSS) {
                // Get tripwire details for this rule ID
                RockIvaBaWireRule wireRule;
                memset(&wireRule, 0, sizeof(wireRule));
                
                // We need to retrieve the actual rule parameters from the loaded configuration
                RockIvaBaTaskParams* taskParams = (RockIvaBaTaskParams*)userdata;
                
                // Find matching rule by ID if we have access to the taskParams
                if (taskParams) {
                    for (int r = 0; r < ROCKIVA_BA_MAX_RULE_NUM; r++) {
                        if (taskParams->baRules.tripWireRule[r].ruleID == firstTrigger->ruleID) {
                            wireRule = taskParams->baRules.tripWireRule[r];
                            break;
                        }
                    }
                }
                
                // Determine direction using the rule configuration
                const char* direction;
                
                // Get the direction from the wire rule event parameter
                direction = map_tripwire_direction(wireRule.event);
                
                // If we couldn't get specific rule info, fallback to a simple determination
                if (strcmp(direction, "unknown") == 0) {
                    // Simple direction detection based on object movement
                    // This is a fallback if we can't get the actual rule configuration
                    direction = "head_to_tail"; // Default
                }
                
                // Tripwire event
                char json_message[512];
                snprintf(json_message, sizeof(json_message),
                         "{\n"
                         "  \"timestamp\": \"%s\",\n"
                         "  \"camera_id\": \"%s\",\n"
                         "  \"client_id\": \"%s\",\n"
                         "  \"event_type\": \"tripwire_event\",\n"
                         "  \"sequence_number\": %d,\n"
                         "  \"tripwire_event\": {\n"
                         "    \"direction\": \"%s\",\n"
                         "    \"object\": {\n"
                         "      \"class\": \"%s\",\n"
                         "      \"tracking_id\": \"%u\"\n"
                         "    }\n"
                         "  }\n"
                         "}\n",
                         saix_get_current_timestamp(),
                         camera_id,
                         client_id,
                         firstTrigger->ruleID,
                         direction,  // Use the properly mapped direction
                         get_object_type_string(obj->objInfo.type),
                         obj->objInfo.objId);

                g_event_callback(firstTrigger->ruleID, "Tripwire", json_message);
                printf(">>> ba_result_callback() triggered with %u objects\n", result->objNum);

            }
            else if (firstTrigger->triggerType == ROCKIVA_BA_RULE_INAREA || 
                     firstTrigger->triggerType == ROCKIVA_BA_RULE_STAY) {
                // Area invasion event
                char json_message[512];
                snprintf(json_message, sizeof(json_message),
                         "{\n"
                         "  \"timestamp\": \"%s\",\n"
                         "  \"camera_id\": \"%s\",\n"
                         "  \"client_id\": \"%s\",\n"
                         "  \"event_type\": \"area_invasion_event\",\n"
                         "  \"sequence_number\": %d,\n"
                         "  \"area_invasion_event\": {\n"
                         "    \"area_id\": %d,\n"
                         "    \"object\": {\n"
                         "      \"class\": \"%s\",\n"
                         "      \"tracking_id\": \"%u\"\n"
                         "    }\n"
                         "  }\n"
                         "}\n",
                         saix_get_current_timestamp(),
                         camera_id,
                         client_id,
                         firstTrigger->ruleID,
                         firstTrigger->ruleID,
                         get_object_type_string(obj->objInfo.type),
                         obj->objInfo.objId);

                g_event_callback(firstTrigger->ruleID, "Area Invasion", json_message);
                printf(">>> ba_result_callback() triggered with %u objects\n", result->objNum);

            }
        }
    }

}

// Initialization function
int init_tripwire(RockIvaHandle handle) {
    printf("Initializing Tripwire functionality...\n");

    // Configure rules from rk_param directly
    if (configure_rules_from_rkparam(handle) != 0) {
        printf("Failed to configure rules from rk_param.\n");
        return -1;
    }

    printf("Tripwire initialization complete.\n");
    return 0;
}

// Deinitialization function
int deinit_tripwire(RockIvaHandle handle) {
    printf("Deinitializing Tripwire functionality...\n");

    RockIvaRetCode ret = ROCKIVA_BA_Release(handle);
    if (ret != ROCKIVA_RET_SUCCESS) {
        printf("Failed to release RockIva BA: %d\n", ret);
        return -1;
    }
    
    // Free any allocated memory for taskParams
    // (implementation would depend on how you store it)

    printf("Tripwire deinitialization complete.\n");
    return 0;
}