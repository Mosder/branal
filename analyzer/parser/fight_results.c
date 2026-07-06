#include "fight_results.h"

#include <stdio.h>
#include <string.h>

char *find_fight_results(char *stream, int stream_len, int *length) {
    char *start = memmem(stream, stream_len, FIGHT_RESULTS_START, strlen(FIGHT_RESULTS_START));
    char *end = memmem(stream, stream_len, FIGHT_RESULTS_END, strlen(FIGHT_RESULTS_END));
    *length = start != NULL && end != NULL ? end - start + strlen(FIGHT_RESULTS_END) : -1;
    return start;
}

FightResults parse_fight_results(char *data, int length) {
    // TODO: actually parse that data
    printf("len=%d\n%.*s\n----------------\n", length, length, data);
    FightResults tmp;
    return tmp;
}
