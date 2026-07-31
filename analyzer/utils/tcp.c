#include "utils/tcp.h"

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <string.h>

#include "headers/server.h"
#include "utils/memory.h"
#include "utils/tcp_seq_math.h"

// initalize default TCP segment
TCPSegment new_segment() {
    return (TCPSegment){
        .payload = NULL,
        .len = 0,
        .seq = 0,
        .port = 0,
    };
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

    // get source
    segment.src = ip_header->ip_src.s_addr == inet_addr(SERVER_IP) ? SERVER : CLIENT;

    // check if it's TCP
    if (ip_header->ip_p != IPPROTO_TCP)
        return segment;

    // get TCP header
    const struct tcphdr *tcp_header = (const struct tcphdr *)(packet + eth_header_len + ip_header_len);
    int tcp_header_len = tcp_header->th_off * 4;

    // get TCP sequence number and client port
    segment.seq = ntohl(tcp_header->seq);
    segment.port = ntohs(segment.src == SERVER ? tcp_header->th_dport : tcp_header->th_sport);

    // get payload
    int total_header_len = eth_header_len + ip_header_len + tcp_header_len;
    segment.len = packet_data->caplen - total_header_len;
    segment.payload = safe_malloc(segment.len);
    memcpy(segment.payload, packet + total_header_len, segment.len);
    return segment;
}

TCPStream new_stream(ConnectionSource src) {
    return (TCPStream){
        .bytes = array_new(1, INIT_STREAM_CAPACITY, NULL),
        .seq = 0,
        .port = 0,
        .src = src,
        .pending = NULL,
    };
}

void destroy_stream(TCPStream stream) {
    array_destroy(stream.bytes);
    heap_free(stream.pending);
}

void remove_parsed_data(TCPStream *stream, size_t parsed_len) {
    array_remove_front(&stream->bytes, parsed_len);
    stream->seq += parsed_len;
}

// get the index of the first relevant byte in segment (its tcp sequence number is the one stream is expecting)
// return the index of the first relevant byte in payload or -1 if segment isn't directly connected to stream
int get_first_relevant_byte_index(TCPStream stream, TCPSegment segment) {
    // expected tcp sequence numbers for stream and segment
    uint32_t stream_expected = stream.seq + stream.bytes.count;
    uint32_t segment_expected = segment.seq + segment.len;
    if (seq_in_range_ie(stream_expected, segment.seq, segment_expected))
        return stream_expected - segment.seq;
    return -1;
}

// check if the segment is in acceptable range to the stream
int in_acceptable_range(TCPStream stream, TCPSegment segment) {
    // expected tcp sequence number for stream and max acceptable sequence number for segment
    uint32_t stream_expected = stream.seq + stream.bytes.count;
    uint32_t max_acceptable_seq = stream_expected + MAX_SEQ_OFFSET;
    return seq_in_range_ei(segment.seq, stream_expected, max_acceptable_seq);
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
            array_concat(&stream->bytes, top->payload + data_start, top->len - data_start);
            top_changed = 1;
        }

        // remove top if it was just added or no longer is in acceptable range (e.g. was contained within another one pending)
        if (top_changed || !in_acceptable_range(*stream, *top)) {
            heap_top_remove(stream->pending);
        }
    }
}

int handle_segment(TCPStream *stream, TCPSegment segment) {
    // add the segment if port is different (new connection)
    if (stream->port != segment.port) {
        // create new heap for new pending segments
        heap_free(stream->pending);
        stream->pending = heap_new(sizeof(TCPSegment), segments_compare, segment_cleanup);

        array_remove_front(&stream->bytes, stream->bytes.count);
        stream->seq = segment.seq;
        stream->port = segment.port;

        array_concat(&stream->bytes, segment.payload, segment.len);
        free(segment.payload);
        return 1;
    }

    // add segment directly to the stream if it's directly connected
    int data_start = get_first_relevant_byte_index(*stream, segment);
    if (data_start != -1) {
        array_concat(&stream->bytes, segment.payload + data_start, segment.len - data_start);
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
