// parser/fight_results.h
// utilities for parsing the fight results data

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

// values of field "team"
#define TEAM_FRIEND 1
#define TEAM_ENEMY 2

// fight results data for friendly
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

// fight results data for enemy
typedef struct {
    char *name;
    int level;
} EnemyResults;

// entire fight resutls
typedef struct {
    int num_friendly;
    FriendlyResults *friendly_results;
    int num_enemy;
    EnemyResults *enemy_results;
} FightResults;

// find the first fight results data in stream
// params:
//      - stream - stream to find the fight results in
//      - stream_len - length of the stream
//      - length - length of the fight results data or -1 if there's no end yet
// returns:
//      pointer to the beggining of fight results data or NULL if there's none
extern char *find_fight_results(char *stream, int stream_len, int *length);

// parse given fight results
// params:
//      - data - fight results data to parse
//      - length - length of fight results data
// returns:
//      FightResults struct containing parsed data
extern FightResults parse_fight_results(char *data, int length);
