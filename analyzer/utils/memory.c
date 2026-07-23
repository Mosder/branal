#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *safe_malloc(size_t size) {
    for (size_t i = 0; i < MAX_ALLOC_ATTEMPTS; i++) {
        void *p = malloc(size);
        if (p)
            return p;
        if (i < MAX_ALLOC_ATTEMPTS - 1) {
            printf("Failed malloc. Retrying in 1s...\n");
            sleep(1);
        }
    }

    printf("ERROR: Failed malloc %d times. Exiting...", MAX_ALLOC_ATTEMPTS);
    exit(EXIT_FAILURE);
}

void *safe_realloc(void *ptr, size_t size) {
    if (!ptr)
        return safe_malloc(size);

    for (size_t i = 0; i < MAX_ALLOC_ATTEMPTS; i++) {
        void *p = realloc(ptr, size);
        if (p)
            return p;
        if (i < MAX_ALLOC_ATTEMPTS - 1) {
            printf("Failed realloc. Retrying in 1s...\n");
            sleep(1);
        }
    }

    printf("ERROR: Failed realloc %d times. Exiting...", MAX_ALLOC_ATTEMPTS);
    exit(EXIT_FAILURE);
}

// expand an array
// returns pointer to the expanded array
void *array_expand(void *arr, size_t el_size, size_t *cap, size_t to_fit) {
    while (*cap < to_fit)
        *cap *= 2;
    return safe_realloc(arr, *cap * el_size);
}

void *array_append(void *arr, size_t el_size, size_t *len, size_t *cap, void *val) {
    if (*len >= *cap)
        arr = array_expand(arr, el_size, cap, *len + 1);
    memcpy((char *)arr + (*len)++ * el_size, val, el_size);
    return arr;
}

void *array_concat(void *arr1, size_t el_size, size_t *len1, size_t *cap, void *arr2, size_t len2) {
    if (*len1 + len2 > *cap)
        arr1 = array_expand(arr1, el_size, cap, *len1 + len2);
    memcpy((char *)arr1 + *len1 * el_size, arr2, len2 * el_size);
    *len1 += len2;
    return arr1;
}
