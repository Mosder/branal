// parser/fight_results.h
// utilities for parsing the fight results data

#include "../utils/tcp.h"
#include "parser_defs.h"

// Payload structure:
// FIGHT_RESULTS_START EntityResults (SEPARATOR_ENTITY EntityResults)* FIGHT_RESULTS_END
//
// EntityResults structure:
// field (SEPARATOR_RESULT field)*
// fields are defined in order in the EntityResults struct

#define FIGHT_RESULTS_START "3;19;"
#define FIGHT_RESULTS_END "[--];"
#define SEPARATOR_ENTITY "[--]"
#define SEPARATOR_RESULT "&"

#define ENTITY_FIELD_COUNT 35
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
    &entity_results.name,       \
    &entity_results.exp,       \
    &entity_results.field4,    \
    &entity_results.gold,      \
    &entity_results.field6,    \
    &entity_results.field7,     \
    &entity_results.gear,       \
    &entity_results.field9,    \
    &entity_results.items,      \
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
    &entity_results.drifs,      \
    &entity_results.splinters, \
    &entity_results.field28,   \
    &entity_results.field29,    \
    &entity_results.field30,   \
    &entity_results.field31,   \
    &entity_results.field32,   \
    &entity_results.field33,   \
    &entity_results.field34,    \
    &entity_results.saturation  \
}

#ifndef ENTITY_RESULTS_DEFINED
#define ENTITY_RESULTS_DEFINED
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
    int field31; // possibly information who is the player - 1 for current player, 0 for others (need to double check)
    int field32;
    int field33;
    char *field34;
    char *saturation;
} EntityResults;
#endif

// values of field "team"
#define TEAM_FRIEND 1
#define TEAM_ENEMY 2

// fight results data for friendly
#ifndef FRIENDLY_RESULTS_DEFINED
#define FRIENDLY_RESULTS_DEFINED
typedef struct {
    char *name;
    int exp;
    int gold;
    char *gear;
    char *items;
    int level;
    int psycho;
    char *drifs;
    int splinters;
    char *saturation_type;
    int saturation;
} FriendlyResults;
#endif

// fight results data for enemy
#ifndef ENEMY_RESULTS_DEFINED
#define ENEMY_RESULTS_DEFINED
typedef struct {
    char *name;
    int level;
} EnemyResults;
#endif

#define MAX_ENTITIES_PER_SIDE 8

// entire fight resutls
#ifndef FIGHT_RESULTS_DEFINED
#define FIGHT_RESULTS_DEFINED
typedef struct {
    size_t num_friendly;
    FriendlyResults friendly_results[MAX_ENTITIES_PER_SIDE];
    size_t num_enemy;
    EnemyResults enemy_results[MAX_ENTITIES_PER_SIDE];
} FightResults;
#endif

extern find_function_t find_fight_results;
extern parse_function_t parse_fight_results;
