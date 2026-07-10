#include "fight_results.h"

#include <stdio.h>
#include <string.h>

byte_t *find_fight_results(TCPStream *stream, int *length) {
    byte_t *start = memmem(stream->data, stream->len, FIGHT_RESULTS_START, strlen(FIGHT_RESULTS_START));
    byte_t *end = memmem(stream->data, stream->len, FIGHT_RESULTS_END, strlen(FIGHT_RESULTS_END));
    *length = start != NULL && end != NULL ? end - start + strlen(FIGHT_RESULTS_END) : -1;
    return start;
}

FightResults parse_fight_results(byte_t *data, size_t length) {
    // TODO: actually parse that data
    printf("len=%zu\n%.*s\n----------------\n", length, (int)length, data);
    FightResults tmp;
    return tmp;
}
