// parser/parser.h
// main parser file

#include "fight_results.h"

#include "../utils/tcp.h"

// "generic" type for parsed data
typedef struct {
    enum { FIGHT_RESULTS } data_type;
    union {
        FightResults fight_results;
    } data;
} ParsedData;

// parse a stream of TCP data
// params:
//      - stream - the stream to parse
//      - parsed_len - pointer to inform how much of the stream was parsed (not all if e.g. start of data is there, but the end isn't yet)
//      - n_parsed_data - pointer to the count of parsed data that was retrieved from the stream
// returns:
//      pointer to the array of parsed data
extern ParsedData *parse_stream(TCPStream *stream, size_t *parsed_len, size_t *n_parsed_data);
