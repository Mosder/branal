// parser/fight_results/gear_info.h
// GearInfo generic struct data

#include "parser/structs.h"

// GearInfo structure:
// field (SEPARATOR_GEAR_INFO_FIELD field)*
// fields are defined in order in the GearInfo struct
#define SEPARATOR_GEAR_INFO_FIELD ","

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

static const StructField gear_info_fields[] = {
    {STRING, offsetof(GearInfo, name)},    {INT, offsetof(GearInfo, field2)},  {INT, offsetof(GearInfo, lvl)},     {INT, offsetof(GearInfo, price)},
    {INT, offsetof(GearInfo, field5)},     {INT, offsetof(GearInfo, field6)},  {INT, offsetof(GearInfo, field7)},  {INT, offsetof(GearInfo, rank)},
    {INT, offsetof(GearInfo, field9)},     {INT, offsetof(GearInfo, field10)}, {INT, offsetof(GearInfo, field11)}, {INT, offsetof(GearInfo, durability)},
    {STRING, offsetof(GearInfo, field13)}, {INT, offsetof(GearInfo, field14)}, {INT, offsetof(GearInfo, field15)}, {FLOAT, offsetof(GearInfo, charges)},
    {STRING, offsetof(GearInfo, field17)}, {INT, offsetof(GearInfo, field18)}, {INT, offsetof(GearInfo, stars)},   {INT, offsetof(GearInfo, field20)},
    {INT, offsetof(GearInfo, field21)},    {INT, offsetof(GearInfo, field22)}, {INT, offsetof(GearInfo, field23)},
};
