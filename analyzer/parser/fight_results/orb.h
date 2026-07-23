// parser/fight_results/orb.h
// Orb generic struct data

#include "parser/structs.h"

// Orb structure:
// field (SEPARATOR_ORB_FIELD field)*
// fields are defined in order in the Orb struct
#define SEPARATOR_ORB_FIELD ","

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

static const StructField orb_fields[] = {
    {STRING, offsetof(Orb, field1)},
    {INT, offsetof(Orb, field2)},
    {INT, offsetof(Orb, field3)},
    {STRING, offsetof(Orb, field4)},
    {INT, offsetof(Orb, field5)},
    {INT, offsetof(Orb, id)},
    {INT, offsetof(Orb, size)},
    {FLOAT, offsetof(Orb, value)},
    {STRING, offsetof(Orb, field9)},
};
