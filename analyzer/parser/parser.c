#include "parser.h"

#include <stdlib.h>

ParsedData *parse_stream(TCPStream *stream, size_t *parsed_len, size_t *n_parsed_data) {
    // TODO: Need to do parsing of the entire thing (loop) and return all of the parsed data
    int length;
    byte_t *start = find_fight_results(stream, &length);
    if (start == NULL) {
        *parsed_len = stream->len;
    }
    else if (length == -1) {
        *parsed_len = 0;
    }
    else {
        parse_fight_results(start, length);
        *parsed_len = start - stream->data + length;
    }
    return NULL;
}
