// utils/memory.h
// helper functions for dealing with memory management

#include <stddef.h>

#ifndef MEMORY_H
#define MEMORY_H

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define MAX_ALLOC_ATTEMPTS 3

// tries malloc MAX_ALLOC_ATTEMPTS times, if all fail - exits the program
// params:
//      - size - count of bytes to allocate
// returns:
//      pointer to the allocated memory
extern void *safe_malloc(size_t size);

// tries realloc MAX_ALLOC_ATTEMPTS times, if all fail - exits the program
// params:
//      - ptr - pointer to the memory to reallocate
//      - size - new size of the memory
// returns:
//      pointer to the reallocated memory
extern void *safe_realloc(void *ptr, size_t size);

#endif
