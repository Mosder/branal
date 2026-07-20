#include "parser/fight_results.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv/id_to_item_name.h"
#include "csv/id_to_saturation_type.h"

// function to cleanup fight results to be used in ParsedData struct
void cleanup_fight_results(void *data) {
    FightResults *fight_results = data;
    for (size_t i = 0; i < fight_results->num_friendly; i++) {
        FriendlyResults friendly = fight_results->friendly_results[i];
        free(friendly.name);
        for (size_t j = 0; j < friendly.num_items; j++)
            free(friendly.items[j]);
        free(friendly.items);
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

void destroy_entity_results(EntityResults results) {
    free(results.name);
    free(results.field7);
    free(results.gear);
    free(results.items);
    free(results.drifs);
    free(results.field29);
    free(results.field34);
    free(results.saturation);
}

// get entity results from the string
EntityResults get_entity_results(byte_t *data, size_t length) {
    EntityResults results;
    TypeEnum field_types[ENTITY_FIELD_COUNT] = ENTITY_FIELD_TYPES;
    void *field_pointers[ENTITY_FIELD_COUNT] = ENTITY_FIELD_ADDRESSES(results);

    for (size_t i = 0; i < ENTITY_FIELD_COUNT; i++) {
        // get string buffer of given result
        byte_t *result_separator = memmem(data, length, SEPARATOR_RESULT, strlen(SEPARATOR_RESULT));
        size_t len = result_separator ? result_separator - data : length;
        char *str = malloc(len + 1);
        memcpy(str, data, len);
        str[len] = '\0';

        // parse depending on variable type
        switch (field_types[i]) {
            case INT:
                sscanf(str, "%d", (int *)field_pointers[i]);
                free(str);
                break;
            case FLOAT:
                sscanf(str, "%f", (float *)field_pointers[i]);
                free(str);
                break;
            case DOUBLE:
                sscanf(str, "%lf", (double *)field_pointers[i]);
                free(str);
                break;
            case STRING:
                *(char **)field_pointers[i] = str;
                break;
        }

        // update data pointer and length
        if (result_separator) {
            length -= result_separator - data + strlen(SEPARATOR_RESULT);
            data = result_separator + strlen(SEPARATOR_RESULT);
        }
    }

    return results;
}

char *get_item_name(int item_id) {
    return item_id < id_to_item_name_len ? id_to_item_name[item_id] : "null";
}

char **expand_items(char **items, size_t *items_capacity, size_t to_fit) {
    while (*items_capacity < to_fit)
        *items_capacity <<= 1;
    return realloc(items, *items_capacity * sizeof(char *));
}

void add_gear(FriendlyResults *results, size_t *items_capacity, char *gear_str) {
    if (strlen(gear_str) == 0)
        return;

    // TODO: parse
    if (results->num_items >= *items_capacity)
        results->items = expand_items(results->items, items_capacity, results->num_items + 1);

    results->items[results->num_items++] = strdup(gear_str);
}

void add_drifs(FriendlyResults *results, size_t *items_capacity, char *drifs_str) {
    if (strlen(drifs_str) == 0)
        return;

    // TODO: parse
    if (results->num_items >= *items_capacity)
        results->items = expand_items(results->items, items_capacity, results->num_items + 1);

    results->items[results->num_items++] = strdup(drifs_str);
}

void add_items(FriendlyResults *results, size_t *items_capacity, char *items_str) {
    if (strlen(items_str) == 0)
        return;

    char *items_separator;
    do {
        char buffer[SINGLE_ITEM_BUFFER_LEN];
        items_separator = strstr(items_str, SEPARATOR_ITEMS);

        int item_id, item_count;
        sscanf(items_str, "%d,%d", &item_id, &item_count);
        sprintf(buffer, "%dx %s", item_count, get_item_name(item_id));

        // if there are still items - modify items_str pointer
        if (items_separator)
            items_str = items_separator + strlen(SEPARATOR_ITEMS);

        // expand if needed
        if (results->num_items >= *items_capacity)
            results->items = expand_items(results->items, items_capacity, results->num_items + 1);

        results->items[results->num_items++] = strdup(buffer);
    } while (items_separator);
}

char *get_saturation_type(int saturation_id) {
    return saturation_id < id_to_saturation_type_len ? id_to_saturation_type[saturation_id] : "null";
}

// parse EntityResults to FriendlyResults
FriendlyResults parse_to_friendly(EntityResults entity_results) {
    FriendlyResults results;

    results.name = strdup(entity_results.name);
    results.exp = entity_results.exp;
    results.gold = entity_results.gold;
    results.level = entity_results.level;
    results.psycho = entity_results.psycho;
    results.splinters = entity_results.splinters;

    // create items array and add to it gear, drifs and items
    results.num_items = 0;
    results.items = malloc(INIT_ITEMS_CAPACITY * sizeof(char *));
    size_t items_cap = INIT_ITEMS_CAPACITY;
    add_gear(&results, &items_cap, entity_results.gear);
    add_drifs(&results, &items_cap, entity_results.drifs);
    add_items(&results, &items_cap, entity_results.items);

    results.saturation = 0;
    if (strlen(entity_results.saturation) > 0) {
        int saturation_id;
        sscanf(entity_results.saturation, "%d,%d", &saturation_id, &results.saturation);
        results.saturation_type = get_saturation_type(saturation_id);
    }

    return results;
}

// parse EntityResults to EnemyResults
EnemyResults parse_to_enemy(EntityResults entity_results) {
    EnemyResults results;

    results.name = strdup(entity_results.name);
    results.level = entity_results.level;

    return results;
}

ParsedData parse_fight_results(byte_t *data, size_t length) {
    ParsedData parsed_data;
    parsed_data.data_type = TYPE_FIGHT_RESULTS;

    // ignore start and end indicators
    data += strlen(FIGHT_RESULTS_START);
    length -= strlen(FIGHT_RESULTS_START) + strlen(FIGHT_RESULTS_END);

    FightResults *results = malloc(sizeof(FightResults));
    results->num_friendly = 0;
    results->num_enemy = 0;

    byte_t *entity_separator;
    do {
        // get generic entity results
        entity_separator = memmem(data, length, SEPARATOR_ENTITY, strlen(SEPARATOR_ENTITY));
        EntityResults entity_results = get_entity_results(data, entity_separator ? entity_separator - data : length);

        // depending on team - parse it into friendly or enemy results
        switch (entity_results.team) {
            case TEAM_FRIEND:
                results->friendly_results[results->num_friendly++] = parse_to_friendly(entity_results);
                break;
            case TEAM_ENEMY:
                results->enemy_results[results->num_enemy++] = parse_to_enemy(entity_results);
                break;
        }
        destroy_entity_results(entity_results);

        // if there are still entities - modify data pointer and length
        if (entity_separator) {
            length -= entity_separator - data + strlen(SEPARATOR_ENTITY);
            data = entity_separator + strlen(SEPARATOR_ENTITY);
        }
    } while (entity_separator);

    parsed_data.data = results;
    parsed_data.data_cleanup = cleanup_fight_results;
    return parsed_data;
}
