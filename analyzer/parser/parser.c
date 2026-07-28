#include "parser/parser.h"

#include <stdlib.h>

#include "parser/find_parse_functions.h"
#include "utils/memory.h"

// function to cleanup ParsedData in DynArray
void cleanup_parsed_data(void *parsed_data) {
    ParsedData *data = parsed_data;
    if (data->data_cleanup)
        data->data_cleanup(data->data);
    free(data->data);
}

// find earliest data to parse in stream
byte_t *find_earliest_data(TCPStream *stream, size_t stream_offset, int *earliest_length, parse_function_t **earliest_parse_fun) {
    byte_t *earliest_data = NULL;
    for (size_t i = 0; i < ARR_LEN(all_find_parse_functions); i++) {
        // ignore if not correct stream
        if (stream->src != all_find_parse_functions[i].stream_src)
            continue;

        int length;
        byte_t *data = all_find_parse_functions[i].find_fn(stream->bytes.data + stream_offset, stream->bytes.count - stream_offset, &length);
        // if it's the first found or earlier than earliest - save it
        if (data && (!earliest_data || data < earliest_data)) {
            earliest_data = data;
            *earliest_length = length;
            *earliest_parse_fun = all_find_parse_functions[i].parse_fn;
        }
    }
    return earliest_data;
}

DynArray parse_stream(TCPStream *stream, size_t *parsed_len) {
    DynArray parsed_data = array_new(sizeof(ParsedData), INIT_PARSED_DATA_CAPACITY, cleanup_parsed_data);

    size_t stream_offset = 0;
    byte_t *earliest_data;
    do {
        // find earliest data to parse
        int earliest_length;
        parse_function_t *earliest_parse_fun;
        earliest_data = find_earliest_data(stream, stream_offset, &earliest_length, &earliest_parse_fun);

        // if start of data to parse was found, but not the end, then finish here
        // set parsed_len, so that the beginning of that data is at the beginning of the stream
        if (earliest_data && earliest_length == -1) {
            *parsed_len = earliest_data - (byte_t *)stream->bytes.data;
            return parsed_data;
        }

        // if data to parse was found, parse it and update the offset
        if (earliest_data) {
            ParsedData new_parsed = earliest_parse_fun(earliest_data, earliest_length);
            array_append(&parsed_data, &new_parsed);
            stream_offset = earliest_data - (byte_t *)stream->bytes.data + earliest_length;
        }
    } while (earliest_data);

    // if all was parsed - set parsed_len to stream_len and return the data
    *parsed_len = stream->bytes.count;
    return parsed_data;
}
