#include "analyzer.h"

#include "parser/parser.h"
#include "utils/pcap.h"

void analyze_file(char *path) {
    // open the file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(path, errbuf);

    // handle errors
    if (handle == NULL) {
        printf("Couldn't open the specified file:\n%s\n", errbuf);
        return;
    }

    struct pcap_pkthdr *packet_data;
    const unsigned char *packet;
    const unsigned char *payload;

    // read packets from the file
    while (pcap_next_ex(handle, &packet_data, &packet) == 1) {
        int payload_len;
        uint32_t tcp_seq;
        payload = get_payload_from_packet(packet, packet_data, &payload_len, &tcp_seq);
        // TODO: Stream reassembly using TCP sequence number
        int parsed_len;
        parse_stream((char *)payload, payload_len, &parsed_len);
    }

    pcap_close(handle);
}
