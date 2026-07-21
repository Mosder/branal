// parser/fight_results/gear.h
// Gear generic struct data

#include "parser/structs.h"

// Gear structure:
// field (SEPARATOR_GEAR_FIELD field)*
// fields are defined in order in the Gear struct
#define SEPARATOR_GEAR "???" // TODO: change it to correct one which idk at this moment
#define SEPARATOR_GEAR_FIELD "$"

// values of field "type" in Gear
#define TYPE_SYNG_NORMAL 0
#define TYPE_SET 1
#define TYPE_RARE 2

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

static const StructField gear_fields[] = {
    {STRING, offsetof(Gear, stats)},  {STRING, offsetof(Gear, resistances)}, {STRING, offsetof(Gear, info)},    {STRING, offsetof(Gear, reqs)},
    {STRING, offsetof(Gear, field5)}, {STRING, offsetof(Gear, psycho)},      {STRING, offsetof(Gear, field7)},  {STRING, offsetof(Gear, field8)},
    {STRING, offsetof(Gear, field9)}, {STRING, offsetof(Gear, syng_tier)},   {INT, offsetof(Gear, field11)},    {INT, offsetof(Gear, field12)},
    {INT, offsetof(Gear, field13)},   {STRING, offsetof(Gear, field14)},     {INT, offsetof(Gear, type)},       {INT, offsetof(Gear, field16)},
    {STRING, offsetof(Gear, orb)},    {STRING, offsetof(Gear, field18)},     {STRING, offsetof(Gear, field19)},
};
