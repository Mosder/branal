// parser/parser.h
// main parser file

#include "structures/array.h"
#include "utils/tcp.h"

#define INIT_PARSED_DATA_CAPACITY 4

// parse a stream of TCP data
// params:
//      - stream - the stream to parse
//      - parsed_len - pointer to inform how much of the stream was parsed (not all if e.g. start of data is there, but the end isn't yet)
// returns:
//      array of parsed data
extern DynArray parse_stream(TCPStream *stream, size_t *parsed_len);
