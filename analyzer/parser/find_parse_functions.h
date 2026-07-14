// parser/find_parse_functions.h
// array of find and parse functions for every result type

#include "parser/fight_results.h"

#define RESULT_TYPE_COUNT 1

FindParseFunctions all_find_parse_functions[RESULT_TYPE_COUNT] = {
    {.find_fn = find_fight_results, .parse_fn = parse_fight_results}
};
