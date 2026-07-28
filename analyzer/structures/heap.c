#include "structures/heap.h"

#include <stdlib.h>
#include <string.h>

#include "utils/memory.h"

Heap *heap_new(size_t el_size, int (*compare)(void *el1, void *el2), void (*el_cleanup)(void *el)) {
    Heap *heap = safe_malloc(sizeof(Heap));
    heap->data = safe_malloc(INIT_HEAP_CAPACITY * el_size);
    heap->count = 0;
    heap->el_size = el_size;
    heap->capacity = INIT_HEAP_CAPACITY;
    heap->compare = compare;
    heap->el_cleanup = el_cleanup;
    return heap;
}

// return index of parent in heap
size_t parent(size_t i) {
    return (i - 1) / 2;
}

// return index of left child in heap
size_t left(size_t i) {
    return 2 * i + 1;
}

// return index of right child in heap
size_t right(size_t i) {
    return 2 * i + 2;
}

// swap two elements in heap
void swap(Heap *heap, size_t i1, size_t i2) {
    char tmp[heap->el_size];
    memcpy(tmp, heap->data + i1, heap->el_size);
    memcpy(heap->data + i1, heap->data + i2, heap->el_size);
    memcpy(heap->data + i2, tmp, heap->el_size);
}

// expand heap exponentially to fit el_count elements
void expand_heap(Heap *heap, size_t el_count) {
    while (heap->capacity < el_count)
        heap->capacity = (heap->capacity << 1) | 1;
    heap->data = safe_realloc(heap->data, heap->capacity * heap->el_size);
}

void heap_insert(Heap *heap, void *element) {
    // expand heap if capacity is too small
    if (heap->count >= heap->capacity)
        expand_heap(heap, heap->count + 1);

    // insert into the last position
    size_t i = heap->count++;
    memcpy(heap->data + i, element, heap->count);

    // heapify up
    while (i > 0 && heap->compare(heap->data + i, heap->data + parent(i))) {
        swap(heap, i, parent(i));
        i = parent(i);
    }
}

void *heap_top_get(Heap *heap) {
    return heap->count <= 0 ? NULL : heap->data;
}

void heap_top_remove(Heap *heap) {
    if (heap->count <= 0)
        return;

    if (heap->el_cleanup)
        heap->el_cleanup(heap->data);

    // move last element to root
    memcpy(heap->data, heap->data + --heap->count, heap->el_size);

    // heapify down
    size_t i = 0;
    int changed;
    do {
        size_t l = left(i);
        size_t r = right(i);
        size_t new_par = i;

        if (l < heap->count && heap->compare(heap->data + l, heap->data + new_par))
            new_par = l;
        if (r < heap->count && heap->compare(heap->data + r, heap->data + new_par))
            new_par = r;

        if ((changed = new_par != i)) {
            swap(heap, new_par, i);
            i = new_par;
        }
    } while (changed);
}

void heap_free(Heap *heap) {
    if (!heap)
        return;
    if (heap->el_cleanup) {
        for (size_t i = 0; i < heap->count; i++)
            heap->el_cleanup(heap->data + i);
    }
    free(heap->data);
    free(heap);
}
