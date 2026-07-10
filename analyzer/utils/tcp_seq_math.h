// utils/tcp_seq_math.h
// helper functions for math regarding tcp sequence numbers which correctly handle overflow

#include <stdint.h>

// check if a is after b
extern int seq_after(uint32_t a, uint32_t b);

// check if a is before b
extern int seq_before(uint32_t a, uint32_t b);

// check if a is after or equal to b
extern int seq_after_eq(uint32_t a, uint32_t b);

// check if a is before or equal to b
extern int seq_before_eq(uint32_t a, uint32_t b);

// check if value is in [range_start; range_end]
extern int seq_in_range_ii(uint32_t value, uint32_t range_start, uint32_t range_end);

// check if value is in (range_start; range_end)
extern int seq_in_range_ee(uint32_t value, uint32_t range_start, uint32_t range_end);

// check if value is in [range_start; range_end)
extern int seq_in_range_ie(uint32_t value, uint32_t range_start, uint32_t range_end);

// check if value is in (range_start; range_end]
extern int seq_in_range_ei(uint32_t value, uint32_t range_start, uint32_t range_end);
