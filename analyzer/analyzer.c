#include "analyzer.h"

#include <stdio.h>

#include "parser/parser.h"
#include "render.h"
#include "utils/tcp.h"

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

// file analyzer parsed data handler
void file_parsed_data_handler(void *state, DynArray parsed_data) {
    int *fights_count = state;
    ParsedData *data = NULL;
    while ((data = array_next(parsed_data, data))) {
        switch (data->data_type) {
            case TYPE_FIGHT_RESULTS:
                printf("\nFIGHT %d:\n", ++*fights_count);
                render_fight_results(*(FightResults *)data->data);
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
        fprintf(stderr, "Couldn't open the specified file:\n%s\n", errbuf);
        return;
    }

    // analyze the file
    TCPStream server_stream = new_stream(SERVER);
    TCPStream client_stream = new_stream(CLIENT);
    int state = 0;
    analyzer_loop(&server_stream, &client_stream, handle, &state, file_parsed_data_handler);
    destroy_stream(server_stream);
    destroy_stream(client_stream);
    pcap_close(handle);
}
