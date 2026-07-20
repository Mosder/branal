// structures/heap.h
// a simple implementation of heap

#include <stddef.h>

#ifndef HEAP_H
#define HEAP_H

#define INIT_HEAP_DEPTH 7
#define INIT_HEAP_CAPACITY (1 << INIT_HEAP_DEPTH) - 1

typedef struct {
    void **data;
    size_t n_elements;
    size_t element_size;
    size_t capacity;
    int (*compare)(void *element1, void *element2);
    void (*element_cleanup)(void *element);
} Heap;

// create a new heap
// params:
//      - element_size - size of the heap element
//      - compare - function for comparing elements
//        params:
//             - element1 - pointer to the first of the elements to compare
//             - element2 - pointer to the second of the elements to compare
//        returns:
//             truthy if element1 should be element2's parent (e.g. element1 > element2 for max heap), falsy otherwise
//      - element_cleanup - function for cleaning up any internal allocations inside element (not the element itself)
//                          if it has no heap allocations, this should be NULL
//        params:
//             - element - pointer to the element to cleanup
// returns:
//      pointer to the new heap
extern Heap *heap_new(size_t element_size, int (*compare)(void *element1, void *element2), void (*element_cleanup)(void *element));

// insert new element into the heap
// params:
//      - heap - heap to insert to
//      - element - pointer to the element to insert
extern void heap_insert(Heap *heap, void *element);

// get the top element from the heap
// params:
//      - heap - heap to get the top element from
// returns:
//      pointer to the top element in the heap or NULL if heap is empty
extern void *heap_top_get(Heap *heap);

// remove the top element from the heap
// params:
//      - heap - heap to remove the top element from
extern void heap_top_remove(Heap *heap);

// free the heap
//      - heap - heap to free
extern void heap_free(Heap *heap);

#endif
