#include "pcap.h"

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

const unsigned char *get_payload_from_packet(const unsigned char *packet, const struct pcap_pkthdr *packet_data, int *payload_len, uint32_t *tcp_seq) {
    // get Ethernet header
    const struct ether_header *eth_header = (const struct ether_header *)packet;
    int eth_header_len = sizeof(struct ether_header);

    // check if it's IP
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        *payload_len = -1;
        return NULL;
    }

    // get IP header
    const struct ip *ip_header = (const struct ip *)(packet + eth_header_len);
    int ip_header_len = ip_header->ip_hl * 4;

    // check if it's TCP
    if (ip_header->ip_p != IPPROTO_TCP) {
        *payload_len = -1;
        return NULL;
    }

    // get TCP header
    const struct tcphdr *tcp_header = (const struct tcphdr *)(packet + eth_header_len + ip_header_len);
    int tcp_header_len = tcp_header->th_off * 4;

    // get TCP sequence number
    *tcp_seq = ntohl(tcp_header->seq);

    // get payload
    int total_header_len = eth_header_len + ip_header_len + tcp_header_len;
    *payload_len = packet_data->caplen - total_header_len;
    return packet + total_header_len;
}

PendingPayload new_pending(const unsigned char *payload, int payload_len) {
    // TODO: impement
    PendingPayload tmp;
    return tmp;
}

unsigned char *free_parsed_data(unsigned char *stream, uint32_t stream_seq, int stream_len, int parsed_len) {
    // TODO: impement
    return NULL;
}

int add_segment_to_stream(
    unsigned char *stream, uint32_t stream_seq, int stream_len, const unsigned char *payload, uint32_t payload_seq, int payload_len, HashMap *pending
) {
    // TODO: impement
    return 0;
}
