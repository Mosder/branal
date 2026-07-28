// parser/fight_results.h
// utilities for parsing the fight results data

#include "parser/parser_defs.h"
#include "structures/array.h"

#ifndef FIGHT_RESULTS_H
#define FIGHT_RESULTS_H

// Payload structure:
// FIGHT_RESULTS_START EntityResults (SEPARATOR_ENTITY EntityResults)* FIGHT_RESULTS_END
#define FIGHT_RESULTS_START "3;19;"
#define FIGHT_RESULTS_END "[--];"

// items parsing info
#define INIT_ITEMS_CAPACITY 8
#define SINGLE_ITEM_BUFFER_LEN 128
#define SEPARATOR_ITEMS "[-]"

// Item struct - for keeping item string and type
typedef enum { NORMAL, DRIF, ORB, SYNG, SET, RARE, EPIC, MAX_ITEM_TYPE } ItemType;
typedef struct {
    char *data;
    ItemType type;
} Item;

// fight results data for friendly
typedef struct {
    char *name;
    int exp;
    int gold;
    DynArray items;
    int level;
    int psycho;
    int splinters;
    const char *saturation_type;
    int saturation;
} FriendlyResults;

// fight results data for enemy
typedef struct {
    char *name;
    int level;
} EnemyResults;

#define INIT_ENTITIES_PER_SIDE 6

// entire fight resutls
typedef struct {
    DynArray friendly_results;
    DynArray enemy_results;
} FightResults;

extern find_function_t find_fight_results;
extern parse_function_t parse_fight_results;

#endif
