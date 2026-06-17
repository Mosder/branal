#include "analyzer.h"

#include <unistd.h>

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
        payload = get_payload_from_packet(packet, packet_data, &payload_len);
        if (payload_len > 0) {
            printf("\n----------------\n");
            printf("Payload length: %d\n", payload_len);
            printf("Payload:\n");
            fflush(stdout);
            write(STDOUT_FILENO, payload, payload_len);
        }
    }

    pcap_close(handle);
}
