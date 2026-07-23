#include "parser/parser.h"

#include <stdlib.h>

#include "parser/find_parse_functions.h"
#include "utils/memory.h"

// find earliest data to parse in stream
byte_t *find_earliest_data(TCPStream *stream, int stream_offset, int *earliest_length, parse_function_t **earliest_parse_fun) {
    byte_t *earliest_data = NULL;
    for (size_t i = 0; i < ARR_LEN(all_find_parse_functions); i++) {
        int length;
        byte_t *data = all_find_parse_functions[i].find_fn(stream, stream_offset, &length);
        // if it's the first found or earlier than earliest - save it
        if (data && (!earliest_data || data < earliest_data)) {
            earliest_data = data;
            *earliest_length = length;
            *earliest_parse_fun = all_find_parse_functions[i].parse_fn;
        }
    }
    return earliest_data;
}

ParsedData *parse_stream(TCPStream *stream, size_t *parsed_len, size_t *n_parsed_data) {
    size_t parsed_data_capacity = INIT_PARSED_DATA_CAPACITY;
    ParsedData *parsed_data = safe_malloc(parsed_data_capacity * sizeof(ParsedData));
    *n_parsed_data = 0;

    int stream_offset = 0;
    byte_t *earliest_data;
    do {
        // find earliest data to parse
        int earliest_length;
        parse_function_t *earliest_parse_fun;
        earliest_data = find_earliest_data(stream, stream_offset, &earliest_length, &earliest_parse_fun);

        // if start of data to parse was found, but not the end, then finish here
        // set parsed_len, so that the beginning of that data is at the beginning of the stream
        if (earliest_data && earliest_length == -1) {
            *parsed_len = earliest_data - stream->data;
            return parsed_data;
        }

        // if data to parse was found, parse it and update the offset
        if (earliest_data) {
            ParsedData new_parsed = earliest_parse_fun(earliest_data, earliest_length);
            parsed_data = array_append(parsed_data, sizeof(ParsedData), n_parsed_data, &parsed_data_capacity, &new_parsed);
            stream_offset = earliest_data - stream->data + earliest_length;
        }
    } while (earliest_data);

    // if all was parsed - set parsed_len to stream_len and return the data
    *parsed_len = stream->len;
    return parsed_data;
}

void free_parsed_data(ParsedData *parsed_data, size_t n_parsed_data) {
    for (size_t i = 0; i < n_parsed_data; i++) {
        if (parsed_data[i].data_cleanup)
            parsed_data[i].data_cleanup(parsed_data[i].data);
        free(parsed_data[i].data);
    }
    free(parsed_data);
}
