#include "analyzer.h"

#include "parser/parser.h"
#include "utils/tcp.h"

// loop through packets received from pcap_handle and analyze the TCP stream
void analyzer_loop(TCPStream *stream, pcap_t *pcap_handle) {
    // read packets from handle
    struct pcap_pkthdr *packet_data;
    const byte_t *packet;
    while (pcap_next_ex(pcap_handle, &packet_data, &packet) == 1) {
        // get new segment
        TCPSegment segment = get_segment_from_packet(packet, packet_data);

        // handle the new segment
        if (handle_segment(stream, segment)) {
            // if it was added to the stream - parse it
            size_t parsed_len;
            size_t n_parsed_data;
            ParsedData *parsed_data = parse_stream(stream, &parsed_len, &n_parsed_data);

            // if some bytes were parsed - remove them from the stream
            if (parsed_len > 0)
                remove_parsed_data(stream, parsed_len);

            // TODO: handle parsed data
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
    analyzer_loop(&stream, handle);
    destroy_stream(stream);
    pcap_close(handle);
}
