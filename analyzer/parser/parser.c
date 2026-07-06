#include "parser.h"

#include <stdlib.h>

ParsedData *parse_stream(char *stream, int stream_len, int *parsed_len) {
    // TODO: Need to do parsing of the entire thing (loop) and return all of the parsed data
    if (stream_len > 0) {
        int length;
        char *start = find_fight_results(stream, stream_len, &length);
        if (start != NULL) {
            parse_fight_results(start, length);
        }
    }
    return NULL;
}
