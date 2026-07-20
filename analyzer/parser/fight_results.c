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
            free(friendly.items[j].data);
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

// save to a field in struct depending on the type
void save_to_struct(TypeEnum type, void *field, char *src, size_t src_len) {
    char *str = malloc(src_len + 1);
    memcpy(str, src, src_len);
    str[src_len] = '\0';

    // parse depending on variable type
    switch (type) {
        case INT:
            sscanf(str, "%d", (int *)field);
            free(str);
            break;
        case FLOAT:
            sscanf(str, "%f", (float *)field);
            free(str);
            break;
        case DOUBLE:
            sscanf(str, "%lf", (double *)field);
            free(str);
            break;
        case STRING:
            *(char **)field = str;
            break;
    }
}

// fill struct fields from given data of given length
void get_struct_data(size_t field_count, TypeEnum *field_types, void **field_pointers, char *data_separator, char *data, size_t data_len) {
    for (size_t i = 0; i < field_count; i++) {
        char *sep = memmem(data, data_len, data_separator, strlen(data_separator));
        size_t len = sep ? sep - data : data_len;
        save_to_struct(field_types[i], field_pointers[i], (char *)data, len);

        // update data pointer and length
        if (sep) {
            data_len -= sep - data + strlen(SEPARATOR_RESULT);
            data = sep + strlen(SEPARATOR_RESULT);
        }
    }
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
    get_struct_data(ENTITY_FIELD_COUNT, field_types, field_pointers, SEPARATOR_RESULT, (char *)data, length);
    return results;
}

Item *expand_items(Item *items, size_t *items_capacity, size_t to_fit) {
    while (*items_capacity < to_fit)
        *items_capacity <<= 1;
    return realloc(items, *items_capacity * sizeof(Item));
}

void add_gear(FriendlyResults *results, size_t *items_capacity, char *gear_str) {
    if (strlen(gear_str) == 0)
        return;

    // TODO: parse
    if (results->num_items >= *items_capacity)
        results->items = expand_items(results->items, items_capacity, results->num_items + 1);

    Item item;
    item.data = strdup(gear_str);
    item.type = RARE;
    results->items[results->num_items++] = item;
}

void destroy_drif(Drif drif) {
    free(drif.field1);
    free(drif.field4);
    free(drif.field11);
    free(drif.field12);
    free(drif.name);
    free(drif.field18);
    free(drif.field19);
}

Drif get_drif(char *data, size_t length) {
    Drif drif;
    TypeEnum field_types[DRIF_FIELD_COUNT] = DRIF_FIELD_TYPES;
    void *field_pointers[DRIF_FIELD_COUNT] = DRIF_FIELD_ADDRESSES(drif);
    get_struct_data(DRIF_FIELD_COUNT, field_types, field_pointers, SEPARATOR_DRIF_INFO, data, length);
    return drif;
}

void add_drifs(FriendlyResults *results, size_t *items_capacity, char *drifs_str) {
    if (strlen(drifs_str) == 0)
        return;

    char *drifs_separator;
    do {
        drifs_separator = strstr(drifs_str, SEPARATOR_DRIFS);
        Drif drif = get_drif(drifs_str, drifs_separator ? drifs_separator - drifs_str : strlen(drifs_str));

        // if there are still drifs - modify drifs_str pointer
        if (drifs_separator)
            drifs_str = drifs_separator + strlen(SEPARATOR_DRIFS);

        // expand if needed
        if (results->num_items >= *items_capacity)
            results->items = expand_items(results->items, items_capacity, results->num_items + 1);

        Item item;
        item.data = strdup(drif.name);
        item.type = DRIF;
        results->items[results->num_items++] = item;

        destroy_drif(drif);
    } while (drifs_separator);
}

char *get_item_name(int item_id) {
    return item_id < id_to_item_name_len ? id_to_item_name[item_id] : "null";
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

        Item item;
        item.data = strdup(buffer);
        item.type = NORMAL;
        results->items[results->num_items++] = item;
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
    results.items = malloc(INIT_ITEMS_CAPACITY * sizeof(Item));
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
