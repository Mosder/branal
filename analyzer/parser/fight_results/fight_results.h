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

// values of field "team" in EntityResults
#define TEAM_FRIEND 1
#define TEAM_ENEMY 2

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
    /* saturation */ STRING, \
}
#define ENTITY_FIELD_ADDRESSES(entity_results) {\
    &entity_results.team,       \
    &entity_results.name,       \
    &entity_results.exp,        \
    &entity_results.field4,     \
    &entity_results.gold,       \
    &entity_results.field6,     \
    &entity_results.field7,     \
    &entity_results.gear,       \
    &entity_results.field9,     \
    &entity_results.items,      \
    &entity_results.field11,    \
    &entity_results.field12,    \
    &entity_results.field13,    \
    &entity_results.field14,    \
    &entity_results.field15,    \
    &entity_results.level,      \
    &entity_results.field17,    \
    &entity_results.field18,    \
    &entity_results.field19,    \
    &entity_results.field20,    \
    &entity_results.field21,    \
    &entity_results.field22,    \
    &entity_results.field23,    \
    &entity_results.field24,    \
    &entity_results.psycho,     \
    &entity_results.drifs,      \
    &entity_results.splinters,  \
    &entity_results.field28,    \
    &entity_results.field29,    \
    &entity_results.field30,    \
    &entity_results.field31,    \
    &entity_results.field32,    \
    &entity_results.field33,    \
    &entity_results.field34,    \
    &entity_results.saturation, \
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
#define SEPARATOR_GEARS "???" // TODO: change it to correct one which idk at this moment
// Item struct - for keeping item string and type
typedef enum { NORMAL, DRIF, ORB, SYNG, SET, RARE, EPIC } ItemType;
typedef struct {
    char *data;
    ItemType type;
} Item;

// Drif structure:
// field (SEPARATOR_DRIF_FIELDS field)*
// fields are defined in order in the Drif struct
#define DRIF_FIELD_COUNT 18
#define SEPARATOR_DRIF_FIELDS ","
// clang-format off
#define DRIF_FIELD_TYPES { \
    /* field1 */   STRING, \
    /* field2 */   INT,    \
    /* size */     INT,    \
    /* field4 */   STRING, \
    /* field5 */   INT,    \
    /* id */       INT,    \
    /* value1 */   FLOAT,  \
    /* value2 */   FLOAT,  \
    /* field9 */   INT,    \
    /* field10 */  INT,    \
    /* field11 */  STRING, \
    /* size_str */ STRING, \
    /* field13 */  INT,    \
    /* power */    INT,    \
    /* field15 */  INT,    \
    /* field16 */  INT,    \
    /* name */     STRING, \
    /* field18 */  STRING, \
}
#define DRIF_FIELD_ADDRESSES(drif) {\
    &drif.field1,   \
    &drif.field2,   \
    &drif.size,     \
    &drif.field4,   \
    &drif.field5,   \
    &drif.id,       \
    &drif.value1,   \
    &drif.value2,   \
    &drif.field9,   \
    &drif.field10,  \
    &drif.field11,  \
    &drif.size_str, \
    &drif.field13,  \
    &drif.power,    \
    &drif.field15,  \
    &drif.field16,  \
    &drif.name,     \
    &drif.field18,  \
}
// clang-format on
typedef struct {
    char *field1; // just says "drif"
    int field2;
    int size; // 1 - sub, 2 - bi, 3 - magni, 4 - arcy
    char *field4;
    int field5;
    int id;       // id of orb type
    float value1; // value1, value2 (in %) - one is start value, the other is value per lvl (both are the same)
    float value2; // value1, value2 (in %) - one is start value, the other is value per lvl (both are the same)
    int field9;
    int field10;
    char *field11;
    char *size_str; // sub - small, medium - bi, big - magni, ??? - arcy
    int field13;
    int power; // how much space it takes
    int field15;
    int field16;
    char *name; // <size>drif <name>
    char *field18;
} Drif;

// Gear structure:
// field (SEPARATOR_GEAR_FIELDS field)*
// fields are defined in order in the Gear struct
#define GEAR_FIELD_COUNT 19
#define SEPARATOR_GEAR_FIELDS "$"
// values of field "type" in Gear
#define TYPE_SYNG_NORMAL 0
#define TYPE_SET 1
#define TYPE_RARE 2
// clang-format off
#define GEAR_FIELD_TYPES { \
    /* stats */        STRING, \
    /* resistances */  STRING, \
    /* info */         STRING, \
    /* reqs */         STRING, \
    /* field5 */       STRING, \
    /* psycho */       STRING, \
    /* field7 */       STRING, \
    /* field8 */       STRING, \
    /* field9 */       STRING, \
    /* syng_tier */    STRING, \
    /* field11 */      INT,    \
    /* field12 */      INT,    \
    /* field13 */      INT,    \
    /* field14 */      STRING, \
    /* type */         INT,    \
    /* field16 */      INT,    \
    /* orb */          STRING, \
    /* field18 */      STRING, \
    /* field19 */      STRING, \
}
#define GEAR_FIELD_ADDRESSES(gear) {\
    &gear.stats,       \
    &gear.resistances, \
    &gear.info,        \
    &gear.reqs,        \
    &gear.field5,      \
    &gear.psycho,      \
    &gear.field7,      \
    &gear.field8,      \
    &gear.field9,      \
    &gear.syng_tier,   \
    &gear.field11,     \
    &gear.field12,     \
    &gear.field13,     \
    &gear.field14,     \
    &gear.type,        \
    &gear.field16,     \
    &gear.orb,         \
    &gear.field18,     \
    &gear.field19,     \
}
// clang-format on
typedef struct {
    char *stats;       // strength, dexterity, power, knowledge, hp, mana, stamina
    char *resistances; // slashing, crushing, piercing, fire, cold, energy, mental
    char *info;        // GearInfo struct data
    char *reqs;        // requirements (str, dex, pow, know), weight, something else
    char *field5;
    char *psycho; // psycho effects (id,%) for syngs
    char *field7;
    char *field8;
    char *field9;
    char *syng_tier; // lvl 20 - 1, lvl 30 - 2, etc..., null for non-syngs
    int field11;
    int field12;
    int field13;
    char *field14;
    int type;    // 0 - normal item or syng, 1 - set, 2 - rare
    int field16; // 10 for rare, -1 for others ???
    char *orb;   // orb info
    char *field18;
    char *field19;
} Gear;

// GearInfo structure:
// field (SEPARATOR_GEAR_INFO_FIELDS field)*
// fields are defined in order in the GearInfo struct
#define GEAR_INFO_FIELD_COUNT 23
#define SEPARATOR_GEAR_INFO_FIELDS ","
// clang-format off
#define GEAR_INFO_FIELD_TYPES { \
    /* name */       STRING, \
    /* field2 */     INT,    \
    /* lvl */        INT,    \
    /* price */      INT,    \
    /* field5 */     INT,    \
    /* field6 */     INT,    \
    /* field7 */     INT,    \
    /* rank */       INT,    \
    /* field9 */     INT,    \
    /* field10 */    INT,    \
    /* field11 */    INT,    \
    /* durability */ INT,    \
    /* field13 */    STRING, \
    /* field14 */    INT,    \
    /* field15 */    INT,    \
    /* charges */    FLOAT,  \
    /* field17 */    STRING, \
    /* field18 */    INT,    \
    /* stars */      INT,    \
    /* field20 */    INT,    \
    /* field21 */    INT,    \
    /* field22 */    INT,    \
    /* field23 */    INT,    \
}
#define GEAR_INFO_FIELD_ADDRESSES(gear_info) {\
    &gear_info.name,       \
    &gear_info.field2,     \
    &gear_info.lvl,        \
    &gear_info.price,      \
    &gear_info.field5,     \
    &gear_info.field6,     \
    &gear_info.field7,     \
    &gear_info.rank,       \
    &gear_info.field9,     \
    &gear_info.field10,    \
    &gear_info.field11,    \
    &gear_info.durability, \
    &gear_info.field13,    \
    &gear_info.field14,    \
    &gear_info.field15,    \
    &gear_info.charges,    \
    &gear_info.field17,    \
    &gear_info.field18,    \
    &gear_info.stars,      \
    &gear_info.field20,    \
    &gear_info.field21,    \
    &gear_info.field22,    \
    &gear_info.field23,    \
}
// clang-format on
typedef struct {
    char *name; // item name
    int field2;
    int lvl;   // level requirement
    int price; // gold price value
    int field5;
    int field6;
    int field7;
    int rank; // I-XII
    int field9;
    int field10;
    int field11;
    int durability; // in %
    char *field13;
    int field14;
    int field15;
    float charges; // charge count
    char *field17;
    int field18;
    int stars; // incrustation level: 0 - b1, 1 - b2, ... (-100 for syngs)
    int field20;
    int field21;
    int field22;
    int field23;
} GearInfo;

// Orb structure:
// field (SEPARATOR_ORB_FIELDS field)*
// fields are defined in order in the Orb struct
#define ORB_FIELD_COUNT 9
#define SEPARATOR_ORB_FIELDS ","
// clang-format off
#define ORB_FIELD_TYPES { \
    /* field1 */ STRING, \
    /* field2 */ INT,    \
    /* field3 */ INT,    \
    /* field4 */ STRING, \
    /* field5 */ INT,    \
    /* id */     INT,    \
    /* size */   INT,    \
    /* value */  FLOAT,  \
    /* field9 */ STRING, \
}
#define ORB_FIELD_ADDRESSES(orb) {\
    &orb.field1, \
    &orb.field2, \
    &orb.field3, \
    &orb.field4, \
    &orb.field5, \
    &orb.id,     \
    &orb.size,   \
    &orb.value,  \
    &orb.field9, \
}
// clang-format on
typedef struct {
    char *field1; // just says "orb"
    int field2;
    int field3;
    char *field4;
    int field5;
    int id;      // id of orb type
    int size;    // 1 - sub, 2 - bi, 3 - magni, 4 - arcy
    float value; // value in % you gain from the orb
    char *field9;
} Orb;

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
    const char *saturation_type;
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
