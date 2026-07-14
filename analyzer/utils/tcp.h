// utils/tcp.h
// functions for handling TCP streams/segments

#include <pcap.h>

#include "structures/heap.h"

typedef unsigned char byte_t;

// struct to persistently store TCP segments
// payload field is malloced and needs to be freed
#ifndef TCP_SEGMENT_DEFINED
#define TCP_SEGMENT_DEFINED
typedef struct {
    byte_t *payload; // payload in the segment
    size_t len;      // length of the payload
    uint32_t seq;    // TCP sequence number of the segment
    uint16_t port;   // destination (client) port of connection the segment was received from
} TCPSegment;
#endif

// get TCP segment from captured packet
// params:
//      - packet - captured packet
//      - packet_data - metadata about packet
// returns:
//      TCPSegment from packet, payload is NULL if the received packet was incorrect
extern TCPSegment get_segment_from_packet(const byte_t *packet, const struct pcap_pkthdr *packet_data);

// struct that stores all the information about stream
#ifndef TCP_STREAM_DEFINED
#define TCP_STREAM_DEFINED
typedef struct {
    byte_t *data;    // bytes in the stream
    size_t len;      // length of the stream
    size_t capacity; // capacity of the data buffer
    uint32_t seq;    // TCP sequence number of the first byte in the stream
    uint16_t port;   // destination (client) port of current analyzed connection
    Heap *pending;   // min heap of segments yet to be added to stream (sorted by TCP sequence number)
} TCPStream;
#endif

#define INIT_STREAM_CAPACITY 256

// initialize new stream
// returns:
//      newly created TCPStream structure with initial values
extern TCPStream new_stream();

// cleanup memory allocations within stream
// params:
//      stream - stream to destroy
extern void destroy_stream(TCPStream stream);

// remove parsed data from the stream
// params:
//      - stream - pointer to the stream to remove a part of
//      - parsed_len - count of n first bytes of the stream that were parsed
extern void remove_parsed_data(TCPStream *stream, size_t parsed_len);

// max difference between expected TCP seq and received TCP seq to still accept it into pending
#define MAX_SEQ_OFFSET 100000

// add TCP segment to stream, pending or completely discard it
// params:
//      - stream - pointer to the current stream
//      - segment - segment to handle
// returns:
//      1 if the segment was added to the stream, 0 if it was discarded or saved to pending
extern int handle_segment(TCPStream *stream, TCPSegment segment);
