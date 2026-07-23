#include "parser/fight_results/fight_results.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv/id_to_artifact_size.h"
#include "csv/id_to_item_name.h"
#include "csv/id_to_orb_name.h"
#include "csv/id_to_saturation_type.h"
#include "parser/fight_results/drif.h"
#include "parser/fight_results/entity.h"
#include "parser/fight_results/gear.h"
#include "parser/fight_results/gear_info.h"
#include "parser/fight_results/orb.h"
#include "parser/structs.h"
#include "utils/memory.h"
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
    free(fight_results->friendly_results);
    free(fight_results->enemy_results);
}

byte_t *find_fight_results(TCPStream *stream, int offset, int *length) {
    byte_t *start = memmem(stream->data + offset, stream->len - offset, FIGHT_RESULTS_START, strlen(FIGHT_RESULTS_START));
    byte_t *end = NULL;
    if (start)
        end = memmem(start, stream->len - (start - stream->data), FIGHT_RESULTS_END, strlen(FIGHT_RESULTS_END));
    *length = end ? end - start + strlen(FIGHT_RESULTS_END) : -1;
    return start;
}

void get_stars(char *buffer, int incr_above_b1) {
    char star_types[] = {'B', 'S', 'G'};
    sprintf(buffer, " (%c%d)", star_types[incr_above_b1 / 3], incr_above_b1 % 3 + 1);
}

void add_gear(FriendlyResults *results, size_t *items_capacity, char *gears_str) {
    if (strlen(gears_str) == 0)
        return;

    char *gears_separator;
    do {
        gears_separator = strstr(gears_str, SEPARATOR_GEAR);
        size_t len = gears_separator ? gears_separator - gears_str : strlen(gears_str);
        Gear gear = {0};
        fill_struct(&gear, gear_fields, ARR_LEN(gear_fields), gears_str, len, SEPARATOR_GEAR_FIELD);

        // if there are still gears - modify gears_str pointer
        if (gears_separator)
            gears_str = gears_separator + strlen(SEPARATOR_GEAR);

        // get gear info for rank, name and stars / syng level
        GearInfo gear_info = {0};
        fill_struct(&gear_info, gear_info_fields, ARR_LEN(gear_info_fields), gear.info, strlen(gear.info), SEPARATOR_GEAR_INFO_FIELD);

        // create the buffer of information about gear
        char buffer[SINGLE_ITEM_BUFFER_LEN];
        char roman[5];
        to_roman(roman, gear_info.rank);
        sprintf(buffer, "[%s] %s", roman, gear_info.name);
        switch (gear.type) {
            case TYPE_SYNG_NORMAL:
                // atoi should be fine, since syng_tier starts from 1 for syngs
                if (atoi(gear.syng_tier)) {
                    char lvl[7];
                    sprintf(lvl, " (%d)", gear_info.lvl);
                    strcat(buffer, lvl);
                }
                break;
            default: {
                char stars[6];
                get_stars(stars, gear_info.stars);
                strcat(buffer, stars);
                break;
            }
        }

        // add gear with specific type
        Item item = {.data = strdup(buffer)};
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
        results->items = array_append(results->items, sizeof(Item), &results->num_items, items_capacity, &item);

        cleanup_struct(&gear_info, gear_info_fields, ARR_LEN(gear_info_fields));

        // add orb to items if it exists
        if (strlen(gear.orb)) {
            Orb orb = {0};
            fill_struct(&orb, orb_fields, ARR_LEN(orb_fields), gear.orb, strlen(gear.orb), SEPARATOR_ORB_FIELD);

            char buffer[SINGLE_ITEM_BUFFER_LEN];
            sprintf(buffer, "%sorb %s", get_artifact_size_from_id(orb.size), get_orb_name_from_id(orb.id));

            Item item = {.data = strdup(buffer), .type = ORB};
            results->items = array_append(results->items, sizeof(Item), &results->num_items, items_capacity, &item);

            cleanup_struct(&orb, orb_fields, ARR_LEN(orb_fields));
        }

        cleanup_struct(&gear, gear_fields, ARR_LEN(gear_fields));
    } while (gears_separator);
}

void add_drifs(FriendlyResults *results, size_t *items_capacity, char *drifs_str) {
    if (strlen(drifs_str) == 0)
        return;

    char *drifs_separator;
    do {
        drifs_separator = strstr(drifs_str, SEPARATOR_DRIF);
        size_t len = drifs_separator ? drifs_separator - drifs_str : strlen(drifs_str);
        Drif drif = {0};
        fill_struct(&drif, drif_fields, ARR_LEN(drif_fields), drifs_str, len, SEPARATOR_DRIF_FIELD);

        // if there are still drifs - modify drifs_str pointer
        if (drifs_separator)
            drifs_str = drifs_separator + strlen(SEPARATOR_DRIF);

        Item item = {.data = strdup(drif.name), .type = DRIF};
        results->items = array_append(results->items, sizeof(Item), &results->num_items, items_capacity, &item);

        cleanup_struct(&drif, drif_fields, ARR_LEN(drif_fields));
    } while (drifs_separator);
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
        sprintf(buffer, "%dx %s", item_count, get_item_name_from_id(item_id));

        // if there are still items - modify items_str pointer
        if (items_separator)
            items_str = items_separator + strlen(SEPARATOR_ITEMS);

        Item item = {.data = strdup(buffer), .type = NORMAL};
        results->items = array_append(results->items, sizeof(Item), &results->num_items, items_capacity, &item);
    } while (items_separator);
}

// parse EntityResults to FriendlyResults
FriendlyResults parse_to_friendly(EntityResults entity_results) {
    FriendlyResults results = {
        .name = strdup(entity_results.name),
        .exp = entity_results.exp,
        .gold = entity_results.gold,
        .level = entity_results.level,
        .psycho = entity_results.psycho,
        .splinters = entity_results.splinters,
        .num_items = 0,
        .saturation = 0
    };

    // create items array and add to it gear, drifs and items
    results.items = safe_malloc(INIT_ITEMS_CAPACITY * sizeof(Item));
    size_t items_cap = INIT_ITEMS_CAPACITY;
    add_gear(&results, &items_cap, entity_results.gear);
    add_drifs(&results, &items_cap, entity_results.drifs);
    add_items(&results, &items_cap, entity_results.items);

    if (strlen(entity_results.saturation) > 0) {
        int saturation_id;
        sscanf(entity_results.saturation, "%d,%d", &saturation_id, &results.saturation);
        results.saturation_type = get_saturation_type_from_id(saturation_id);
    }

    return results;
}

// parse EntityResults to EnemyResults
EnemyResults parse_to_enemy(EntityResults entity_results) {
    EnemyResults results = {.name = strdup(entity_results.name), .level = entity_results.level};
    return results;
}

ParsedData parse_fight_results(byte_t *data, size_t length) {
    ParsedData parsed_data = {.data_type = TYPE_FIGHT_RESULTS, .data_cleanup = cleanup_fight_results};

    // ignore start and end indicators
    data += strlen(FIGHT_RESULTS_START);
    length -= strlen(FIGHT_RESULTS_START) + strlen(FIGHT_RESULTS_END);

    FightResults *results = safe_malloc(sizeof(FightResults));

    results->friendly_results = safe_malloc(INIT_ENTITIES_PER_SIDE * sizeof(FriendlyResults));
    results->num_friendly = 0;
    size_t friendly_cap = INIT_ENTITIES_PER_SIDE;

    results->enemy_results = safe_malloc(INIT_ENTITIES_PER_SIDE * sizeof(EnemyResults));
    results->num_enemy = 0;
    size_t enemy_cap = INIT_ENTITIES_PER_SIDE;

    byte_t *entity_separator;
    do {
        // get generic entity results
        entity_separator = memmem(data, length, SEPARATOR_ENTITY, strlen(SEPARATOR_ENTITY));
        size_t len = entity_separator ? entity_separator - data : length;
        EntityResults entity_results = {0};
        fill_struct(&entity_results, entity_fields, ARR_LEN(entity_fields), (char *)data, len, SEPARATOR_ENTITY_FIELD);

        // depending on team - parse it into friendly or enemy results
        switch (entity_results.team) {
            case TEAM_FRIEND: {
                FriendlyResults res = parse_to_friendly(entity_results);
                results->friendly_results = array_append(results->friendly_results, sizeof(FriendlyResults), &results->num_friendly, &friendly_cap, &res);
                break;
            }
            case TEAM_ENEMY: {
                EnemyResults res = parse_to_enemy(entity_results);
                results->enemy_results = array_append(results->enemy_results, sizeof(EnemyResults), &results->num_enemy, &enemy_cap, &res);
                break;
            }
        }

        cleanup_struct(&entity_results, entity_fields, ARR_LEN(entity_fields));

        // if there are still entities - modify data pointer and length
        if (entity_separator) {
            length -= entity_separator - data + strlen(SEPARATOR_ENTITY);
            data = entity_separator + strlen(SEPARATOR_ENTITY);
        }
    } while (entity_separator);

    parsed_data.data = results;
    return parsed_data;
}
