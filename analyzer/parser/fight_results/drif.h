// parser/fight_results/drif.h
// Drif generic struct data

#include "parser/structs.h"

// Drif structure:
// field (SEPARATOR_DRIF_FIELD field)*
// fields are defined in order in the Drif struct
#define SEPARATOR_DRIF "???" // TODO: change it to correct one which idk at this moment
#define SEPARATOR_DRIF_FIELD ","

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

static const StructField drif_fields[] = {
    {STRING, offsetof(Drif, field1)},
    {INT, offsetof(Drif, field2)},
    {INT, offsetof(Drif, size)},
    {STRING, offsetof(Drif, field4)},
    {INT, offsetof(Drif, field5)},
    {INT, offsetof(Drif, id)},
    {FLOAT, offsetof(Drif, value1)},
    {FLOAT, offsetof(Drif, value2)},
    {INT, offsetof(Drif, field9)},
    {INT, offsetof(Drif, field10)},
    {STRING, offsetof(Drif, field11)},
    {STRING, offsetof(Drif, size_str)},
    {INT, offsetof(Drif, field13)},
    {INT, offsetof(Drif, power)},
    {INT, offsetof(Drif, field15)},
    {INT, offsetof(Drif, field16)},
    {STRING, offsetof(Drif, name)},
    {STRING, offsetof(Drif, field18)},
};
