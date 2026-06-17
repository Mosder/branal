#include "pcap.h"

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

const unsigned char *get_payload_from_packet(const unsigned char *packet, const struct pcap_pkthdr *packet_data, int *payload_len) {
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

    // get payload
    int total_header_len = eth_header_len + ip_header_len + tcp_header_len;
    *payload_len = packet_data->caplen - total_header_len;
    return packet + total_header_len;
}
