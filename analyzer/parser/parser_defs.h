// parser/parser_defs.h
// useful definitions for parser files

#include "utils/tcp.h"

#ifndef PARSER_DEFS_H
#define PARSER_DEFS_H

typedef enum { TYPE_FIGHT_RESULTS } ResultsEnum;

// "generic" type for parsed data
typedef struct {
    ResultsEnum data_type;            // type of data represented by an enum
    void *data;                       // pointer to the parsed data
    void (*data_cleanup)(void *data); // function to cleanup internal allocations inside data
                                      // if no internal allocations - this should be NULL
} ParsedData;

// find the first results data of specific type in stream
// params:
//      - stream - stream to find the data in
//      - offset - offset to start finding data from
//      - length - length of the data or -1 if there's no end yet
// returns:
//      pointer to the beginning of data or NULL if there's none
typedef byte_t *find_function_t(TCPStream *stream, int offset, int *length);

// parse given data of specifc type
// params:
//      - data - data to parse
//      - length - length of data
// returns:
//      ParsedData struct including the parsed data
typedef ParsedData parse_function_t(byte_t *data, size_t length);

// struct for holding the find and parse function for every result type
typedef struct {
    find_function_t *find_fn;
    parse_function_t *parse_fn;
} FindParseFunctions;

#endif
