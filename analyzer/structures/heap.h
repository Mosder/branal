// structures/heap.h
// a simple implementation of heap

#include <stddef.h>

#ifndef HEAP_H
#define HEAP_H

#define INIT_HEAP_DEPTH 7
#define INIT_HEAP_CAPACITY (1 << INIT_HEAP_DEPTH) - 1

typedef struct {
    void *data;
    size_t count;
    size_t el_size;
    size_t capacity;
    int (*compare)(void *el1, void *el2);
    void (*el_cleanup)(void *el);
} Heap;

// create a new heap
// params:
//      - el_size - size of the heap element
//      - compare - function for comparing elements
//        params:
//             - el1 - pointer to the first of the elements to compare
//             - el2 - pointer to the second of the elements to compare
//        returns:
//             truthy if el1 should be el2's parent (e.g. el1 > el2 for max heap), falsy otherwise
//      - el_cleanup - function for cleaning up any internal allocations inside element (not the element itself)
//                     if it has no heap allocations, this should be NULL
//        params:
//             - el - pointer to the element to cleanup
// returns:
//      pointer to the new heap
extern Heap *heap_new(size_t el_size, int (*compare)(void *el1, void *el2), void (*el_cleanup)(void *el));

// insert new element into the heap
// params:
//      - heap - heap to insert to
//      - el - pointer to the element to insert
extern void heap_insert(Heap *heap, void *el);

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
