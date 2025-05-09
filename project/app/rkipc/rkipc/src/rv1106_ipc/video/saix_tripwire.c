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
