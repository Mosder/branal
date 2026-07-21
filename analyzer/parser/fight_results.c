#include "parser/fight_results.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv/id_to_artifact_size.h"
#include "csv/id_to_item_name.h"
#include "csv/id_to_orb_name.h"
#include "csv/id_to_saturation_type.h"
#include "utils/roman.h"

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

// free struct fields which are STRING
void destroy_struct(size_t field_count, TypeEnum *field_types, void **field_pointers) {
    for (size_t i = 0; i < field_count; i++) {
        if (field_types[i] == STRING)
            free(*(char **)field_pointers[i]);
    }
}

void destroy_entity_results(EntityResults results) {
    TypeEnum field_types[ENTITY_FIELD_COUNT] = ENTITY_FIELD_TYPES;
    void *field_pointers[ENTITY_FIELD_COUNT] = ENTITY_FIELD_ADDRESSES(results);
    destroy_struct(ENTITY_FIELD_COUNT, field_types, field_pointers);
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

void destroy_gear(Gear gear) {
    TypeEnum field_types[GEAR_FIELD_COUNT] = GEAR_FIELD_TYPES;
    void *field_pointers[GEAR_FIELD_COUNT] = GEAR_FIELD_ADDRESSES(gear);
    destroy_struct(GEAR_FIELD_COUNT, field_types, field_pointers);
}

Gear get_gear(char *data, size_t length) {
    Gear gear;
    TypeEnum field_types[GEAR_FIELD_COUNT] = GEAR_FIELD_TYPES;
    void *field_pointers[GEAR_FIELD_COUNT] = GEAR_FIELD_ADDRESSES(gear);
    get_struct_data(GEAR_FIELD_COUNT, field_types, field_pointers, SEPARATOR_GEAR_FIELDS, data, length);
    return gear;
}

void destroy_gear_info(GearInfo gear_info) {
    TypeEnum field_types[GEAR_INFO_FIELD_COUNT] = GEAR_INFO_FIELD_TYPES;
    void *field_pointers[GEAR_INFO_FIELD_COUNT] = GEAR_INFO_FIELD_ADDRESSES(gear_info);
    destroy_struct(GEAR_INFO_FIELD_COUNT, field_types, field_pointers);
}

GearInfo get_gear_info(char *data, size_t length) {
    GearInfo gear_info;
    TypeEnum field_types[GEAR_INFO_FIELD_COUNT] = GEAR_INFO_FIELD_TYPES;
    void *field_pointers[GEAR_INFO_FIELD_COUNT] = GEAR_INFO_FIELD_ADDRESSES(gear_info);
    get_struct_data(GEAR_INFO_FIELD_COUNT, field_types, field_pointers, SEPARATOR_GEAR_INFO_FIELDS, data, length);
    return gear_info;
}

void destroy_orb(Orb orb) {
    TypeEnum field_types[ORB_FIELD_COUNT] = ORB_FIELD_TYPES;
    void *field_pointers[ORB_FIELD_COUNT] = ORB_FIELD_ADDRESSES(orb);
    destroy_struct(ORB_FIELD_COUNT, field_types, field_pointers);
}

Orb get_orb(char *data, size_t length) {
    Orb orb;
    TypeEnum field_types[ORB_FIELD_COUNT] = ORB_FIELD_TYPES;
    void *field_pointers[ORB_FIELD_COUNT] = ORB_FIELD_ADDRESSES(orb);
    get_struct_data(ORB_FIELD_COUNT, field_types, field_pointers, SEPARATOR_ORB_FIELDS, data, length);
    return orb;
}

char *get_artifact_size(int artifact_size_id) {
    return artifact_size_id < id_to_artifact_size_len ? id_to_artifact_size[artifact_size_id] : "null";
}

char *get_orb_name(int orb_name_id) {
    return orb_name_id < id_to_orb_name_len ? id_to_orb_name[orb_name_id] : "null";
}

void get_stars(char *buffer, int incr_above_b1) {
    char star_types[] = {'B', 'S', 'G'};
    buffer[0] = star_types[incr_above_b1 / 3];
    buffer[1] = incr_above_b1 % 3 + 1 + '0';
    buffer[2] = '\0';
}

void add_gear(FriendlyResults *results, size_t *items_capacity, char *gears_str) {
    if (strlen(gears_str) == 0)
        return;

    char *gears_separator;
    do {
        gears_separator = strstr(gears_str, SEPARATOR_GEARS);
        Gear gear = get_gear(gears_str, gears_separator ? gears_separator - gears_str : strlen(gears_str));

        // if there are still gears - modify gears_str pointer
        if (gears_separator)
            gears_str = gears_separator + strlen(SEPARATOR_DRIFS);

        // expand if needed
        if (results->num_items >= *items_capacity)
            results->items = expand_items(results->items, items_capacity, results->num_items + 1);

        Item item;

        GearInfo gear_info = get_gear_info(gear.info, strlen(gear.info));
        char buffer[SINGLE_ITEM_BUFFER_LEN];
        char roman[5];
        to_roman(roman, gear_info.rank);
        sprintf(buffer, "[%s] %s", roman, gear_info.name);
        switch (gear.type) {
            case TYPE_SYNG_NORMAL:
                // atoi should be fine, since syng_tier starts from 1 for syngs
                if (atoi(gear.syng_tier))
                    sprintf(buffer, "%s (%d)", buffer, gear_info.lvl);
                break;
            default: {
                char stars[3];
                get_stars(stars, gear_info.stars);
                sprintf(buffer, "%s (%s)", buffer, stars);
                break;
            }
        }
        item.data = strdup(buffer);
        destroy_gear_info(gear_info);

        switch (gear.type) {
            case TYPE_SYNG_NORMAL:
                // atoi should be fine, since syng_tier starts from 1 for syngs
                item.type = atoi(gear.syng_tier) ? SYNG : NORMAL;
                break;
            case TYPE_SET:
                item.type = SET;
                break;
            case TYPE_RARE:
                item.type = RARE;
                break;
            default:
                item.type = EPIC;
                break;
        }

        results->items[results->num_items++] = item;

        // add orb to items if it exists
        if (strlen(gear.orb)) {
            Item item;
            char buffer[SINGLE_ITEM_BUFFER_LEN];
            Orb orb = get_orb(gear.orb, strlen(gear.orb));
            sprintf(buffer, "%sorb %s", get_artifact_size(orb.size), get_orb_name(orb.id));
            item.data = strdup(buffer);
            destroy_orb(orb);
            item.type = ORB;
            results->items[results->num_items++] = item;
        }

        destroy_gear(gear);
    } while (gears_separator);
}

void destroy_drif(Drif drif) {
    TypeEnum field_types[DRIF_FIELD_COUNT] = DRIF_FIELD_TYPES;
    void *field_pointers[DRIF_FIELD_COUNT] = DRIF_FIELD_ADDRESSES(drif);
    destroy_struct(DRIF_FIELD_COUNT, field_types, field_pointers);
}

Drif get_drif(char *data, size_t length) {
    Drif drif;
    TypeEnum field_types[DRIF_FIELD_COUNT] = DRIF_FIELD_TYPES;
    void *field_pointers[DRIF_FIELD_COUNT] = DRIF_FIELD_ADDRESSES(drif);
    get_struct_data(DRIF_FIELD_COUNT, field_types, field_pointers, SEPARATOR_DRIF_FIELDS, data, length);
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
