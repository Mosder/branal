#include "heap.h"

#include <stdlib.h>
#include <string.h>

Heap *heap_new(size_t element_size, int (*compare)(void *element1, void *element2), void (*element_cleanup)(void *element)) {
    Heap *heap = malloc(sizeof(Heap));
    heap->data = malloc(INIT_HEAP_CAPACITY * sizeof(void *));
    heap->n_elements = 0;
    heap->element_size = element_size;
    heap->capacity = INIT_HEAP_CAPACITY;
    heap->compare = compare;
    heap->element_cleanup = element_cleanup;
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

// swap two elements
void swap(void **p_el1, void **p_el2) {
    void *tmp = *p_el1;
    *p_el1 = *p_el2;
    *p_el2 = tmp;
}

// expand heap exponentially to fit el_count elements
void expand_heap(Heap *heap, size_t el_count) {
    while (heap->capacity < el_count)
        heap->capacity = (heap->capacity << 1) | 1;
    heap->data = realloc(heap->data, heap->capacity * sizeof(void *));
}

void heap_insert(Heap *heap, void *element) {
    // expand heap if capacity is too small
    if (heap->n_elements >= heap->capacity)
        expand_heap(heap, heap->n_elements + 1);

    // insert into the last position
    size_t i = heap->n_elements;
    heap->data[i] = malloc(heap->element_size);
    memcpy(heap->data[i], element, heap->element_size);
    heap->n_elements++;

    // heapify up
    while (i > 0 && heap->compare(heap->data[i], heap->data[parent(i)])) {
        swap(&heap->data[i], &heap->data[parent(i)]);
        i = parent(i);
    }
}

void *heap_top_get(Heap *heap) {
    return heap->n_elements <= 0 ? NULL : heap->data[0];
}

void free_heap_element(Heap *heap, size_t index) {
    if (heap->element_cleanup)
        heap->element_cleanup(heap->data[index]);
    free(heap->data[index]);
}

void heap_top_remove(Heap *heap) {
    if (heap->n_elements <= 0)
        return;

    free_heap_element(heap, 0);

    // move last element to root
    heap->data[0] = heap->data[--heap->n_elements];

    // heapify down
    size_t i = 0;
    int changed;
    do {
        size_t l = left(i);
        size_t r = right(i);
        size_t to_be_parent = i;

        if (l < heap->n_elements && heap->compare(heap->data[l], heap->data[to_be_parent]))
            to_be_parent = l;
        if (r < heap->n_elements && heap->compare(heap->data[r], heap->data[to_be_parent]))
            to_be_parent = r;

        if ((changed = to_be_parent != i)) {
            swap(&heap->data[to_be_parent], &heap->data[i]);
            i = to_be_parent;
        }
    } while (changed);
}

void heap_free(Heap *heap) {
    for (size_t i = 0; i < heap->n_elements; i++) {
        free_heap_element(heap, i);
    }
    free(heap->data);
    free(heap);
}
