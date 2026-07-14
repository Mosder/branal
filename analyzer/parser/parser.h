// parser/parser.h
// main parser file

#include "parser/fight_results.h"
#include "parser/parser_defs.h"
#include "utils/tcp.h"

#define INIT_PARSED_DATA_CAPACITY 4

// parse a stream of TCP data
// params:
//      - stream - the stream to parse
//      - parsed_len - pointer to inform how much of the stream was parsed (not all if e.g. start of data is there, but the end isn't yet)
//      - n_parsed_data - pointer to the count of parsed data that was retrieved from the stream
// returns:
//      pointer to the array of parsed data
extern ParsedData *parse_stream(TCPStream *stream, size_t *parsed_len, size_t *n_parsed_data);

// free the entire parsed data array from parse_stream function
// params:
//      - parsed_data - parsed data to free
//      - n_parsed_data - count of parsed data
extern void free_parsed_data(ParsedData *parsed_data, size_t n_parsed_data);
