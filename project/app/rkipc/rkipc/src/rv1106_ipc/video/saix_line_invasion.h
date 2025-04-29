#ifndef SAIX_LINE_INVASION_H
#define SAIX_LINE_INVASION_H

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
extern TaskRule g_task_rule;

// Function declarations
void normalize_point(RockIvaPoint* point);
void normalize_line(RockIvaLine* line);
void normalize_area(RockIvaArea* area);
void set_line_invasion_rule(int index, RockIvaLine* line, bool enable);
void clear_all_line_invasion_rules();
void normalize_rules();

#endif /* SAIX_LINE_INVASION_H */