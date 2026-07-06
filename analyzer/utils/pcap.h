// utils/pcap.h
// helper functions for usage of libpcap

#include <pcap.h>

// get payload from a TCP/IP packet
// params:
//      - packet - captured packet
//      - packet_data - metadata about packet
//      - payload_len - pointer to save the payload length to or -1 if not a correct packet
//      - tcp_seq - pointer to save the TCP sequence number
// returns:
//      pointer to the payload or NULL if not a correct packet
extern const unsigned char *get_payload_from_packet(const unsigned char *packet, const struct pcap_pkthdr *packet_data, int *payload_len, uint32_t *tcp_seq);
