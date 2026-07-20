// parser/fight_results.h
// utilities for parsing the fight results data

#include "parser/parser_defs.h"
#include "utils/tcp.h"

#ifndef FIGHT_RESULTS_H
#define FIGHT_RESULTS_H

// Payload structure:
// FIGHT_RESULTS_START EntityResults (SEPARATOR_ENTITY EntityResults)* FIGHT_RESULTS_END
#define FIGHT_RESULTS_START "3;19;"
#define FIGHT_RESULTS_END "[--];"
#define SEPARATOR_ENTITY "[--]"
#define SEPARATOR_RESULT "&"

// EntityResults structure:
// field (SEPARATOR_RESULT field)*
// fields are defined in order in the EntityResults struct
#define ENTITY_FIELD_COUNT 35
// clang-format off
#define ENTITY_FIELD_TYPES { \
    /* team */       INT,    \
    /* name */       STRING, \
    /* exp */        INT,    \
    /* field4 */     FLOAT,  \
    /* gold */       INT,    \
    /* field6 */     FLOAT,  \
    /* field7 */     STRING, \
    /* gear */       STRING, \
    /* field9 */     INT,    \
    /* items */      STRING, \
    /* field11 */    INT,    \
    /* field12 */    INT,    \
    /* field13 */    FLOAT,  \
    /* field14 */    FLOAT,  \
    /* field15 */    FLOAT,  \
    /* level */      INT,    \
    /* field17 */    DOUBLE, \
    /* field18 */    FLOAT,  \
    /* field19 */    INT,    \
    /* field20 */    INT,    \
    /* field21 */    FLOAT,  \
    /* field22 */    FLOAT,  \
    /* field23 */    INT,    \
    /* field24 */    INT,    \
    /* psycho */     INT,    \
    /* drifs */      STRING, \
    /* splinters */  INT,    \
    /* field28 */    INT,    \
    /* field29 */    STRING, \
    /* field30 */    INT,    \
    /* field31 */    INT,    \
    /* field32 */    INT,    \
    /* field33 */    INT,    \
    /* field34 */    STRING, \
    /* saturation */ STRING  \
}
#define ENTITY_FIELD_ADDRESSES(entity_results) {\
    &entity_results.team,      \
    &entity_results.name,      \
    &entity_results.exp,       \
    &entity_results.field4,    \
    &entity_results.gold,      \
    &entity_results.field6,    \
    &entity_results.field7,    \
    &entity_results.gear,      \
    &entity_results.field9,    \
    &entity_results.items,     \
    &entity_results.field11,   \
    &entity_results.field12,   \
    &entity_results.field13,   \
    &entity_results.field14,   \
    &entity_results.field15,   \
    &entity_results.level,     \
    &entity_results.field17,   \
    &entity_results.field18,   \
    &entity_results.field19,   \
    &entity_results.field20,   \
    &entity_results.field21,   \
    &entity_results.field22,   \
    &entity_results.field23,   \
    &entity_results.field24,   \
    &entity_results.psycho,    \
    &entity_results.drifs,     \
    &entity_results.splinters, \
    &entity_results.field28,   \
    &entity_results.field29,   \
    &entity_results.field30,   \
    &entity_results.field31,   \
    &entity_results.field32,   \
    &entity_results.field33,   \
    &entity_results.field34,   \
    &entity_results.saturation \
}
// clang-format on
typedef struct {
    int team; // friend or enemy
    char *name;
    int exp;
    float field4;
    int gold;
    float field6;
    char *field7;
    char *gear; // rares, sets, syngs, orbs
    int field9;
    char *items; // regular items
    int field11;
    int field12;
    float field13;
    float field14; // some multiplier most likely exp/gold/psycho/something else idk
    float field15;
    int level;
    double field17;
    float field18;
    int field19;
    int field20;
    float field21;
    float field22;
    int field23;
    int field24;
    int psycho;
    char *drifs;
    int splinters;
    int field28;
    char *field29;
    int field30;
    int field31;
    int field32;
    int field33;
    char *field34;
    char *saturation;
} EntityResults;

// items/gear/drifs fields parsing info
#define INIT_ITEMS_CAPACITY 8
#define SINGLE_ITEM_BUFFER_LEN 128
#define SEPARATOR_ITEMS "[-]"
#define SEPARATOR_DRIFS "???" // TODO: change it to correct one which idk at this moment
#define SEPARATOR_DRIF_INFO ","
// Item struct - for keeping item string and type
typedef enum { NORMAL, DRIF, SYNG, SET, RARE, EPIC } ItemType;
typedef struct {
    char *data;
    ItemType type;
} Item;

// Drif structure:
// field (SEPARATOR_DRIF field)*
// fields are defined in order in the Drif struct
#define DRIF_FIELD_COUNT 19
// clang-format off
#define DRIF_FIELD_TYPES { \
    /* field1 */  STRING, \
    /* field2 */  INT,    \
    /* field3 */  INT,    \
    /* field4 */  STRING, \
    /* field5 */  INT,    \
    /* field6 */  INT,    \
    /* field7 */  FLOAT,  \
    /* field8 */  FLOAT,  \
    /* field9 */  INT,    \
    /* field10 */ INT,    \
    /* field11 */ STRING, \
    /* field12 */ STRING, \
    /* field13 */ INT,    \
    /* field14 */ INT,    \
    /* field15 */ INT,    \
    /* field16 */ INT,    \
    /* name */    STRING, \
    /* field18 */ STRING, \
    /* field19 */ STRING, \
}
#define DRIF_FIELD_ADDRESSES(drif) {\
    &drif.field1,  \
    &drif.field2,  \
    &drif.field3,  \
    &drif.field4,  \
    &drif.field5,  \
    &drif.field6,  \
    &drif.field7,  \
    &drif.field8,  \
    &drif.field9,  \
    &drif.field10, \
    &drif.field11, \
    &drif.field12, \
    &drif.field13, \
    &drif.field14, \
    &drif.field15, \
    &drif.field16, \
    &drif.name,    \
    &drif.field18, \
    &drif.field19, \
}
// clang-format on
typedef struct {
    char *field1;
    int field2;
    int field3;
    char *field4;
    int field5;
    int field6;
    float field7;
    float field8;
    int field9;
    int field10;
    char *field11;
    char *field12;
    int field13;
    int field14;
    int field15;
    int field16;
    char *name; // <size>drif <name>
    char *field18;
    char *field19;
} Drif;

// values of field "team"
#define TEAM_FRIEND 1
#define TEAM_ENEMY 2

// fight results data for friendly
typedef struct {
    char *name;
    int exp;
    int gold;
    Item *items;
    size_t num_items;
    int level;
    int psycho;
    int splinters;
    char *saturation_type;
    int saturation;
} FriendlyResults;

// fight results data for enemy
typedef struct {
    char *name;
    int level;
} EnemyResults;

#define MAX_ENTITIES_PER_SIDE 8

// entire fight resutls
typedef struct {
    size_t num_friendly;
    FriendlyResults friendly_results[MAX_ENTITIES_PER_SIDE];
    size_t num_enemy;
    EnemyResults enemy_results[MAX_ENTITIES_PER_SIDE];
} FightResults;

extern find_function_t find_fight_results;
extern parse_function_t parse_fight_results;

#endif
