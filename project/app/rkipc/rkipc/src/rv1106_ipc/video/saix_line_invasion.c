// Import libraries

#include <stdint.h>
#include <stdbool.h>

// Define constants
#define ROCKIVA_AREA_NUM_MAX 10
#define ROCKIVA_BA_MAX_RULE_NUM 10
#define MAX_LINE_INVASION_RULES 10

// Define structures

/* Point coordinates */
typedef struct {
    int16_t x; /* Horizontal coordinate, represented as ten-thousandths, range 0~9999 */
    int16_t y; /* Vertical coordinate, represented as ten-thousandths, range 0~9999 */
} RockIvaPoint;

/* Line coordinates */
typedef struct {
    RockIvaPoint head; /* Head coordinate (top of the line in the vertical direction) */
    RockIvaPoint tail; /* Tail coordinate (bottom of the line in the vertical direction) */
} RockIvaLine;

/* Area coordinates */
typedef struct {
    RockIvaPoint points[4]; /* Four points defining an area */
} RockIvaArea;

/* Area configuration */
typedef struct {
    uint32_t areaNum;
    RockIvaArea areas[ROCKIVA_AREA_NUM_MAX];
} RockIvaAreas;

/* Area Invasion Rule */
typedef struct {
    RockIvaArea area;  // Area coordinates
    bool ruleEnable;   // Enable/Disable flag for the rule
} AreaInvasionRule;

/* Task Rule */
typedef struct {
    LineInvasionAreaRule lineInvasionRule[MAX_LINE_INVASION_RULES];
    LineInvasionAreaRule tripWireRule[ROCKIVA_BA_MAX_RULE_NUM];
    AreaInvasionRule areaInRule[ROCKIVA_BA_MAX_RULE_NUM];
    AreaInvasionRule areaOutRule[ROCKIVA_BA_MAX_RULE_NUM];
    AreaInvasionRule areaInBreakRule[ROCKIVA_BA_MAX_RULE_NUM];
} TaskRule;

// Global task rule
TaskRule g_task_rule;

// Function declarations
void normalize_point(RockIvaPoint* point);
void normalize_line(RockIvaLine* line);
void normalize_area(RockIvaArea* area);
void set_line_invasion_rule(int index, RockIvaLine* line, bool enable);
void clear_all_line_invasion_rules();

// Function definitions

void normalize_point(RockIvaPoint* point) {
    if (!point) return;
    // Normalize the point (logic to be implemented)
}

void normalize_line(RockIvaLine* line) {
    if (!line) return;
    normalize_point(&line->head);
    normalize_point(&line->tail);
}

void normalize_area(RockIvaArea* area) {
    if (!area) return;
    for (int i = 0; i < 4; i++) {
        normalize_point(&area->points[i]);
    }
}

void set_line_invasion_rule(int index, RockIvaLine* line, bool enable) {
    if (index < 0 || index >= MAX_LINE_INVASION_RULES) return;
    g_task_rule.lineInvasionRule[index].line = *line;
    g_task_rule.lineInvasionRule[index].ruleEnable = enable;
}

void clear_all_line_invasion_rules() {
    for (int i = 0; i < MAX_LINE_INVASION_RULES; i++) {
        g_task_rule.lineInvasionRule[i].ruleEnable = false;
    }
}

// Main logic for normalizing rules
void normalize_rules() {
    for (int i = 0; i < ROCKIVA_BA_MAX_RULE_NUM; i++) {
        if (g_task_rule.tripWireRule[i].ruleEnable) {
            normalize_line(&g_task_rule.tripWireRule[i].line);
        }
        if (g_task_rule.areaInRule[i].ruleEnable) {
            normalize_area(&g_task_rule.areaInRule[i].area);
        }
        if (g_task_rule.areaOutRule[i].ruleEnable) {
            normalize_area(&g_task_rule.areaOutRule[i].area);
        }
        if (g_task_rule.areaInBreakRule[i].ruleEnable) {
            normalize_area(&g_task_rule.areaInBreakRule[i].area);
        }
    }

    for (int i = 0; i < MAX_LINE_INVASION_RULES; i++) {
        if (g_task_rule.lineInvasionRule[i].ruleEnable) {
            normalize_line(&g_task_rule.lineInvasionRule[i].line);
        }
    }
}