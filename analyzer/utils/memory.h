// utils/memory.h
// helper functions for dealing with memory management

#include <stddef.h>

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

// append to array, expand if necessary
// params:
//      - arr - array to append to
//      - el_size - size of an array element
//      - len - pointer to the length of the array
//      - cap - pointer to the capacity of the array
//      - val - pointer to the value to append to the array
// returns:
//      new pointer to the array (different if it was realloced)
extern void *array_append(void *arr, size_t el_size, size_t *len, size_t *cap, void *val);

// concatenate 2 arrays, expand if necessary
// params:
//      - arr1 - first array (destination)
//      - el_size - size of an array element
//      - len1 - pointer to the length of arr1
//      - cap - pointer to the capacity of arr1
//      - arr2 - second array
//      - len2 - length of arr2
// returns:
//      new pointer to the array (different if it was realloced)
extern void *array_concat(void *arr1, size_t el_size, size_t *len1, size_t *cap, void *arr2, size_t len2);
