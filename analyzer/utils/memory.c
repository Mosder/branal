#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *safe_malloc(size_t size) {
    for (size_t i = 0; i < MAX_ALLOC_ATTEMPTS; i++) {
        void *p = malloc(size);
        if (p)
            return p;
        if (i < MAX_ALLOC_ATTEMPTS - 1) {
            fprintf(stderr, "Failed malloc. Retrying in 1s...\n");
            sleep(1);
        }
    }

    fprintf(stderr, "FATAL: Failed malloc %d times. Exiting...", MAX_ALLOC_ATTEMPTS);
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
            fprintf(stderr, "Failed realloc. Retrying in 1s...\n");
            sleep(1);
        }
    }

    fprintf(stderr, "FATAL: Failed realloc %d times. Exiting...", MAX_ALLOC_ATTEMPTS);
    exit(EXIT_FAILURE);
}
