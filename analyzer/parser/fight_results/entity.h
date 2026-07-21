// parser/fight_results/entity.h
// EntityResults generic struct data

#include "parser/structs.h"

// EntityResults structure:
// field (SEPARATOR_ENTITY_FIELD field)*
// fields are defined in order in the EntityResults struct
#define SEPARATOR_ENTITY "[--]"
#define SEPARATOR_ENTITY_FIELD "&"

// values of field "team" in EntityResults
#define TEAM_FRIEND 1
#define TEAM_ENEMY 2

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

static const StructField entity_fields[] = {
    {INT, offsetof(EntityResults, team)},       {STRING, offsetof(EntityResults, name)},       {INT, offsetof(EntityResults, exp)},
    {FLOAT, offsetof(EntityResults, field4)},   {INT, offsetof(EntityResults, gold)},          {FLOAT, offsetof(EntityResults, field6)},
    {STRING, offsetof(EntityResults, field7)},  {STRING, offsetof(EntityResults, gear)},       {INT, offsetof(EntityResults, field9)},
    {STRING, offsetof(EntityResults, items)},   {INT, offsetof(EntityResults, field11)},       {INT, offsetof(EntityResults, field12)},
    {FLOAT, offsetof(EntityResults, field13)},  {FLOAT, offsetof(EntityResults, field14)},     {FLOAT, offsetof(EntityResults, field15)},
    {INT, offsetof(EntityResults, level)},      {DOUBLE, offsetof(EntityResults, field17)},    {FLOAT, offsetof(EntityResults, field18)},
    {INT, offsetof(EntityResults, field19)},    {INT, offsetof(EntityResults, field20)},       {FLOAT, offsetof(EntityResults, field21)},
    {FLOAT, offsetof(EntityResults, field22)},  {INT, offsetof(EntityResults, field23)},       {INT, offsetof(EntityResults, field24)},
    {INT, offsetof(EntityResults, psycho)},     {STRING, offsetof(EntityResults, drifs)},      {INT, offsetof(EntityResults, splinters)},
    {INT, offsetof(EntityResults, field28)},    {STRING, offsetof(EntityResults, field29)},    {INT, offsetof(EntityResults, field30)},
    {INT, offsetof(EntityResults, field31)},    {INT, offsetof(EntityResults, field32)},       {INT, offsetof(EntityResults, field33)},
    {STRING, offsetof(EntityResults, field34)}, {STRING, offsetof(EntityResults, saturation)},
};
