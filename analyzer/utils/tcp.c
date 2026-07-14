#include "utils/tcp.h"

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <string.h>

#include "utils/tcp_seq_math.h"

// initalize default TCP segment
TCPSegment new_segment() {
    TCPSegment segment;
    segment.payload = NULL;
    segment.len = -1;
    segment.seq = 0;
    return segment;
}

// segments compare function to use with heap
int segments_compare(void *segment1, void *segment2) {
    TCPSegment *seg1 = segment1;
    TCPSegment *seg2 = segment2;
    return seq_before(seg1->seq, seg2->seq);
}

// segment cleanup function to use with heap
void segment_cleanup(void *segment) {
    TCPSegment *seg = segment;
    free(seg->payload);
}

TCPSegment get_segment_from_packet(const byte_t *packet, const struct pcap_pkthdr *packet_data) {
    TCPSegment segment = new_segment();

    // get Ethernet header
    const struct ether_header *eth_header = (const struct ether_header *)packet;
    int eth_header_len = sizeof(struct ether_header);

    // check if it's IP
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP)
        return segment;

    // get IP header
    const struct ip *ip_header = (const struct ip *)(packet + eth_header_len);
    int ip_header_len = ip_header->ip_hl * 4;

    // check if it's TCP
    if (ip_header->ip_p != IPPROTO_TCP)
        return segment;

    // get TCP header
    const struct tcphdr *tcp_header = (const struct tcphdr *)(packet + eth_header_len + ip_header_len);
    int tcp_header_len = tcp_header->th_off * 4;

    // get TCP sequence number
    segment.seq = ntohl(tcp_header->seq);

    // get payload
    int total_header_len = eth_header_len + ip_header_len + tcp_header_len;
    segment.len = packet_data->caplen - total_header_len;
    segment.payload = malloc(segment.len);
    memcpy(segment.payload, packet + total_header_len, segment.len);
    return segment;
}

TCPStream new_stream() {
    TCPStream stream;
    stream.data = NULL; // NULL to differentiate between first and other segments
    stream.len = 0;
    stream.capacity = INIT_STREAM_CAPACITY;
    stream.seq = 0;
    stream.pending = heap_new(sizeof(TCPSegment), segments_compare, segment_cleanup);
    return stream;
}

void destroy_stream(TCPStream stream) {
    free(stream.data);
    heap_free(stream.pending);
}

void remove_parsed_data(TCPStream *stream, size_t parsed_len) {
    memmove(stream->data, stream->data + parsed_len, stream->len - parsed_len);
    stream->len -= parsed_len;
    stream->seq += parsed_len;
}

// get the index of the first relevant byte in segment (its tcp sequence number is the one stream is expecting)
// return the index of the first relevant byte in payload or -1 if segment isn't directly connected to stream
int get_first_relevant_byte_index(TCPStream stream, TCPSegment segment) {
    // expected tcp sequence numbers for stream and segment
    uint32_t stream_expected = stream.seq + stream.len;
    uint32_t segment_expected = segment.seq + segment.len;
    if (seq_in_range_ie(stream_expected, segment.seq, segment_expected))
        return stream_expected - segment.seq;
    return -1;
}

// check if the segment is in acceptable range to the stream
int in_acceptable_range(TCPStream stream, TCPSegment segment) {
    // expected tcp sequence number for stream and max acceptable sequence number for segment
    uint32_t stream_expected = stream.seq + stream.len;
    uint32_t max_acceptable_seq = stream_expected + MAX_SEQ_OFFSET;
    return seq_in_range_ei(segment.seq, stream_expected, max_acceptable_seq);
}

// expand stream capacity exponentially to fit new_len
void expand_stream_capacity(TCPStream *stream, size_t new_len) {
    while (stream->capacity < new_len)
        stream->capacity <<= 1;
    stream->data = realloc(stream->data, stream->capacity);
}

// add segment directly to stream
void add_segment(TCPStream *stream, TCPSegment segment, size_t data_start) {
    size_t new_stream_len = stream->len + segment.len - data_start;
    if (new_stream_len > stream->capacity)
        expand_stream_capacity(stream, new_stream_len);
    memcpy(stream->data + stream->len, segment.payload + data_start, segment.len - data_start);
    stream->len = new_stream_len;
}

// add pending segments to the stream
void add_pending_segments(TCPStream *stream) {
    int top_changed = 1;
    TCPSegment *top;
    while (top_changed && (top = heap_top_get(stream->pending))) {
        top_changed = 0;

        // add pending segment it it became directly connected
        int data_start = get_first_relevant_byte_index(*stream, *top);
        if (data_start != -1) {
            add_segment(stream, *top, data_start);
            top_changed = 1;
        }

        // remove top if it was just added or no longer is in acceptable range (e.g. was contained within another one pending)
        if (top_changed || !in_acceptable_range(*stream, *top)) {
            heap_top_remove(stream->pending);
        }
    }
}

int handle_segment(TCPStream *stream, TCPSegment segment) {
    // add the segment if it's the first one
    if (stream->data == NULL) {
        stream->data = malloc(stream->capacity);
        stream->seq = segment.seq;
        add_segment(stream, segment, 0);
        free(segment.payload);
        return 1;
    }

    // add segment directly to the stream if it's directly connected
    int data_start = get_first_relevant_byte_index(*stream, segment);
    if (data_start != -1) {
        add_segment(stream, segment, data_start);
        free(segment.payload);
        add_pending_segments(stream);
        return 1;
    }

    // add to pending if segment is in acceptable range:
    if (in_acceptable_range(*stream, segment)) {
        heap_insert(stream->pending, &segment);
        return 0;
    }

    // if neither, discard entirely
    free(segment.payload);
    return 0;
}
