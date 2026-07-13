#include "fight_results.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function to cleanup fight results to be used in ParsedData struct
void cleanup_fight_results(void *data) {
    FightResults *fight_results = data;
    for (size_t i = 0; i < fight_results->num_friendly; i++) {
        free(fight_results->friendly_results[i].name);
        free(fight_results->friendly_results[i].gear);
        free(fight_results->friendly_results[i].items);
        free(fight_results->friendly_results[i].drifs);
    }
    for (size_t i = 0; i < fight_results->num_enemy; i++) {
        free(fight_results->enemy_results[i].name);
    }
}

byte_t *find_fight_results(TCPStream *stream, int offset, int *length) {
    byte_t *start = memmem(stream->data + offset, stream->len, FIGHT_RESULTS_START, strlen(FIGHT_RESULTS_START));
    byte_t *end = memmem(stream->data + offset, stream->len, FIGHT_RESULTS_END, strlen(FIGHT_RESULTS_END));
    *length = start != NULL && end != NULL ? end - start + strlen(FIGHT_RESULTS_END) : -1;
    return start;
}

ParsedData parse_fight_results(byte_t *data, size_t length) {
    ParsedData parsed_data;
    parsed_data.data_type = TYPE_FIGHT_RESULTS;

    // TODO: actually parse that data
    printf("len=%zu\n%.*s\n----------------\n", length, (int)length, data);

    parsed_data.data = NULL;
    parsed_data.data_cleanup = NULL;
    // parsed_data.data_cleanup = cleanup_fight_results;
    return parsed_data;
}
