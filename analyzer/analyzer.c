#include "analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser/parser.h"
#include "render.h"
#include "utils/tcp.h"

// cleanup TotalRewards to be used with HashMap
void cleanup_total_rewards(void *rewards) {
    TotalRewards *r = rewards;
    hashmap_free(r->saturations);
    for (size_t i = 0; i < MAX_ITEM_TYPE; i++)
        array_destroy(r->items[i]);
}

// cleanup char** to be used in DynArray
void cleanup_str_pointer(void *str_pointer) {
    char **p_str = str_pointer;
    free(*p_str);
}

// initalize new total rewards
TotalRewards new_total_rewards() {
    HashMap *saturations = hashmap_new(sizeof(const char *), sizeof(int), hash_str, compare_strs, NULL, NULL);
    // clang-format off
    TotalRewards rewards = {
        .exp = 0,
        .gold = 0,
        .psycho = 0,
        .saturations = saturations
    };
    // clang-format on
    for (size_t i = 0; i < MAX_ITEM_TYPE; i++)
        rewards.items[i] = array_new(sizeof(char **), 16, cleanup_str_pointer);
    return rewards;
}

// initalize file analysis state
FileAnalState init_file_anal_state() {
    HashMap *characters = hashmap_new(sizeof(char *), sizeof(TotalRewards), hash_str, compare_strs, NULL, cleanup_total_rewards);
    FileAnalState state = {.fight_count = 0, .curr_character = {0}, .characters = characters};
    return state;
}

// destroy FileAnalState
void destroy_file_anal_state(FileAnalState state) {
    hashmap_free(state.characters);
}

// add data from FriendlyResults into TotalRewards
void add_to_total_rewards(TotalRewards *rewards, FriendlyResults *results) {
    rewards->exp += results->exp;
    rewards->gold += results->gold;
    rewards->psycho += results->psycho;

    Item *it = NULL;
    while ((it = array_next(results->items, it))) {
        char *data = strdup(it->data);
        array_append(&rewards->items[it->type], &data);
    }

    if (results->saturation > 0) {
        int *old_sat = hashmap_get(rewards->saturations, (char *)results->saturation_type);
        int new_sat = results->saturation + (old_sat ? *old_sat : 0);
        hashmap_put(rewards->saturations, (char *)results->saturation_type, &new_sat);
    }
}

// file analyzer parsed data handler
void file_parsed_data_handler(void *state, DynArray parsed_data) {
    FileAnalState *st = state;
    ParsedData *data = NULL;
    while ((data = array_next(parsed_data, data))) {
        switch (data->data_type) {
            case TYPE_FIGHT_RESULTS: {
                FightResults *res = data->data;

                // if only one friendly - it's the player
                if (res->friendly_results.count == 1) {
                    FriendlyResults *player = array_get(res->friendly_results, 0);
                    strcpy(st->curr_character, player->name);
                }

                // find the current player
                FriendlyResults *friendly = NULL;
                int found = 0;
                while (!found && (friendly = array_next(res->friendly_results, friendly))) {
                    if (!strcmp(st->curr_character, friendly->name))
                        found = 1;
                }

                // if found - save the rewards
                if (found) {
                    TotalRewards *r = hashmap_get(st->characters, st->curr_character);
                    if (r) {
                        add_to_total_rewards(r, friendly);
                    }
                    else {
                        TotalRewards rewards = new_total_rewards();
                        add_to_total_rewards(&rewards, friendly);
                        hashmap_put(st->characters, st->curr_character, &rewards);
                    }
                }

                // print fight results
                printf("\nFIGHT %zu:\n", ++st->fight_count);
                render_fight_results(*(FightResults *)data->data);
                break;
            }
        }
    }
}

// loop through packets received from pcap_handle and analyze the TCP streams
void analyzer_loop(
    TCPStream *server_stream, TCPStream *client_stream, pcap_t *pcap_handle, void *state, void (*parsed_data_handler)(void *state, DynArray parsed_data)
) {
    // read packets from handle
    struct pcap_pkthdr *packet_data;
    const byte_t *packet;
    while (pcap_next_ex(pcap_handle, &packet_data, &packet) == 1) {
        // get new segment
        TCPSegment segment = get_segment_from_packet(packet, packet_data);

        // handle the new segment if it's correct one and has data in it (ignore SYN)
        if (segment.len <= 0)
            continue;

        TCPStream *stream = segment.src == SERVER ? server_stream : client_stream;
        if (handle_segment(stream, segment)) {
            // if it was added to the stream - parse it
            size_t parsed_len;
            DynArray parsed_data = parse_stream(stream, &parsed_len);

            // if some bytes were parsed - remove them from the stream
            if (parsed_len > 0)
                remove_parsed_data(stream, parsed_len);

            // if there's parsed data - call the parsed data handler
            if (parsed_data.count > 0)
                parsed_data_handler(state, parsed_data);

            // destroy parsed data
            array_destroy(parsed_data);
        }
    }
}

void analyze_file(char *path) {
    // open the file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(path, errbuf);

    // handle errors
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open the specified file:\n%s\n", errbuf);
        return;
    }

    // analyze the file
    TCPStream server_stream = new_stream(SERVER);
    TCPStream client_stream = new_stream(CLIENT);
    FileAnalState state = init_file_anal_state();
    analyzer_loop(&server_stream, &client_stream, handle, &state, file_parsed_data_handler);

    // TODO: replace this
    TotalRewards *r = hashmap_get(state.characters, "Denem");
    if (r)
        render_total_rewards(*r);

    // free everything
    destroy_stream(server_stream);
    destroy_stream(client_stream);
    destroy_file_anal_state(state);
    pcap_close(handle);
}
