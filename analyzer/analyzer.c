#include "analyzer.h"

#include <stdlib.h>
#include <string.h>

#include "parser/parser.h"
#include "utils/tcp.h"

// loop through packets received from pcap_handle and analyze the TCP stream
void analyzer_loop(TCPStream *stream, pcap_t *pcap_handle, void (*parsed_data_handler)(ParsedData *parsed_data, size_t n_parsed_data)) {
    // read packets from handle
    struct pcap_pkthdr *packet_data;
    const byte_t *packet;
    while (pcap_next_ex(pcap_handle, &packet_data, &packet) == 1) {
        // get new segment
        TCPSegment segment = get_segment_from_packet(packet, packet_data);

        // handle the new segment if it's correct one and has data in it (ignore SYN)
        if (segment.len > 0 && handle_segment(stream, segment)) {
            // if it was added to the stream - parse it
            size_t parsed_len;
            size_t n_parsed_data;
            ParsedData *parsed_data = parse_stream(stream, &parsed_len, &n_parsed_data);

            // if some bytes were parsed - remove them from the stream
            if (parsed_len > 0)
                remove_parsed_data(stream, parsed_len);

            // if there's parsed data - call the parsed data handler
            if (n_parsed_data > 0)
                parsed_data_handler(parsed_data, n_parsed_data);

            // free the entire parsed data memory
            free_parsed_data(parsed_data, n_parsed_data);
        }
    }
}

// prints fight resutls
void print_fight_results(FightResults results) {
    printf("Enemies:\n\t");
    for (size_t i = 0; i < results.num_enemy; i++) {
        EnemyResults enemy = results.enemy_results[i];
        printf("%s (%d)%s", enemy.name, enemy.level, i < results.num_enemy - 1 ? ", " : "\n");
    }
    printf("Player rewards:\n");
    for (size_t i = 0; i < results.num_friendly; i++) {
        FriendlyResults friendly = results.friendly_results[i];
        printf("\t%s (%d):\n", friendly.name, friendly.level);
        printf("\t\texp: %d\n", friendly.exp);
        printf("\t\tgold: %d\n", friendly.gold);
        if (friendly.psycho > 0)
            printf("\t\tpsycho: %d\n", friendly.psycho);
        if (strlen(friendly.items) > 0)
            printf("\t\titems: %s\n", friendly.items);
        if (strlen(friendly.gear) > 0)
            printf("\t\tgear: %s\n", friendly.gear);
        if (strlen(friendly.drifs) > 0)
            printf("\t\tdrifs: %s\n", friendly.drifs);
        if (friendly.splinters > 0)
            printf("\t\tsplinters: %d\n", friendly.splinters);
        if (friendly.saturation > 0)
            printf("\t\tsaturation: %d (%s)\n", friendly.saturation, friendly.saturation_type);
    }
    printf("\n");
}

// file analyzer parsed data handler
void file_parsed_data_handler(ParsedData *parsed_data, size_t n_parsed_data) {
    for (size_t i = 0; i < n_parsed_data; i++) {
        switch (parsed_data[i].data_type) {
            case TYPE_FIGHT_RESULTS:
                print_fight_results(*(FightResults *)parsed_data[i].data);
                break;
        }
    }
}

void analyze_file(char *path) {
    // open the file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(path, errbuf);

    // handle errors
    if (handle == NULL) {
        printf("Couldn't open the specified file:\n%s\n", errbuf);
        return;
    }

    // analyze the file
    TCPStream stream = new_stream();
    analyzer_loop(&stream, handle, file_parsed_data_handler);
    destroy_stream(stream);
    pcap_close(handle);
}
