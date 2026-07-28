// analyzer.h
// functions for analyzing the packets received from the game server

#include "structures/hashmap.h"
#include "structures/array.h"
#include "parser/fight_results/fight_results.h"

#ifndef ANALYZER_H
#define ANALYZER_H

#define MAX_NAME_LENGTH 11

typedef struct {
    int exp;
    int gold;
    DynArray items[MAX_ITEM_TYPE]; // item type -> dynamic array of items
    int psycho;
    HashMap *saturations; // saturation type -> total gained
} TotalRewards;

typedef struct {
    size_t fight_count;
    char curr_character[MAX_NAME_LENGTH+1];
    HashMap *characters; // nick -> TotalRewards
} FileAnalState;

// analyze file
// params:
//      - path - path to the file to analyze
extern void analyze_file(char *path);

#endif
