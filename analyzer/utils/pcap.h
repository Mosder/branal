// utils/pcap.h
// helper functions for usage of libpcap and for handling TCP segments in general

#include <pcap.h>

#include "hashmap.h"

// get payload from a TCP/IP packet
// params:
//      - packet - captured packet
//      - packet_data - metadata about packet
//      - payload_len - pointer to save the payload length to or -1 if not a correct packet
//      - tcp_seq - pointer to save the TCP sequence number
// returns:
//      pointer to the payload or NULL if not a correct packet
extern const unsigned char *get_payload_from_packet(const unsigned char *packet, const struct pcap_pkthdr *packet_data, int *payload_len, uint32_t *tcp_seq);

// struct to persistently store pending payloads
typedef struct {
    unsigned char *payload;
    int payload_len;
} PendingPayload;

// store persistently and return a PendingPayload, the payload is malloced and needs to be freed
// params:
//      - payload - payload to store
//      - payload_len - length of the payload to store
// returns:
//      PendingPayload struct with stored payload
extern PendingPayload new_pending(const unsigned char *payload, int payload_len);

// remove parsed part of the stream from memory and return only unparsed part
// params:
//      - stream - stream to free a part of
//      - stream_seq - pointer to the current TCP sequence number at the beginning of the stream, updated to the new one in the function
//      - stream_len - pointer to the current stream length, updated to the new length in the function
//      - parsed_len - count of n first bytes of the stream that were parsed
// returns:
//      pointer to the new, reduced stream
extern unsigned char *free_parsed_data(unsigned char *stream, uint32_t stream_seq, int stream_len, int parsed_len);

// add TCP segment to stream
// params:
//      - stream - current stream
//      - stream_seq - TCP sequence number at the beginning of the stream
//      - stream_len - current length of the stream
//      - payload - payload to add to stream
//      - payload_seq - payload TCP sequence number
//      - payload_len - length of the payload
//      - pending - hashmap (seq -> PendingPayload) of pending payloads to be added to stream
// returns:
//      1 if the payload was added to the stream, 0 if it was discarded or saved to pending
extern int add_segment_to_stream(
    unsigned char *stream, uint32_t stream_seq, int stream_len, const unsigned char *payload, uint32_t payload_seq, int payload_len, HashMap *pending
);
