#include "utils/tcp_seq_math.h"

int seq_after(uint32_t a, uint32_t b) {
    return (int32_t)(a - b) > 0;
}

int seq_before(uint32_t a, uint32_t b) {
    return (int32_t)(a - b) < 0;
}

int seq_after_eq(uint32_t a, uint32_t b) {
    return (int32_t)(a - b) >= 0;
}

int seq_before_eq(uint32_t a, uint32_t b) {
    return (int32_t)(a - b) <= 0;
}

int seq_in_range_ii(uint32_t value, uint32_t range_start, uint32_t range_end) {
    int after_eq = seq_after_eq(value, range_start);
    int before_eq = seq_before_eq(value, range_end);
    return after_eq && before_eq;
}

int seq_in_range_ee(uint32_t value, uint32_t range_start, uint32_t range_end) {
    int after = seq_after(value, range_start);
    int before = seq_before(value, range_end);
    return after && before;
}

int seq_in_range_ie(uint32_t value, uint32_t range_start, uint32_t range_end) {
    int after_eq = seq_after_eq(value, range_start);
    int before = seq_before(value, range_end);
    return after_eq && before;
}

int seq_in_range_ei(uint32_t value, uint32_t range_start, uint32_t range_end) {
    int after = seq_after(value, range_start);
    int before_eq = seq_before_eq(value, range_end);
    return after && before_eq;
}
