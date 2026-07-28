// structures/array.h
// implementation of dynamic array

#include <stddef.h>

#ifndef ARRAY_H
#define ARRAY_H

typedef struct {
    void *data;                   // array elements
    size_t el_size;               // element size
    size_t count;                 // element count
    size_t capacity;              // array capacity
    void (*el_cleanup)(void *el); // element cleanup function
} DynArray;

// create new dynamic array of given el_size and capacity
// params:
//      - el_size - size of the element to be in array
//      - capacity - initial capacity of the array
//      - cleanup - element cleanup function (if there are any internal allocations)
//                  if no internal allocations - should be NULL
// returns:
//      newly created DynArray
extern DynArray array_new(size_t el_size, size_t capacity, void (*el_cleanup)(void *el));

// get (the pointer to) an element in the array
// params:
//      - array - array to get from
//      - index - index of the elment to get
// returns:
//      pointer to the element to get or NULL if it doesn't exist
extern void *array_get(DynArray array, size_t index);

// get the next element in the array
// params:
//      - array - array to get next element from
//      - prev - previous element of the array, if NULL gets first
// returns:
//      pointer to the next element or NULL prev was the last one
extern void *array_next(DynArray array, void *prev);

// put an element into the array
// params:
//      - array - array to put to
//      - index - index to put to
//      - val - pointer to the value to put
extern void array_put(DynArray *array, size_t index, void *val);

// append to array, expand if necessary
// params:
//      - array - array to append to
//      - val - pointer to the value to append to the array
extern void array_append(DynArray *array, void *val);

// concatenate 2 arrays, expand if necessary
// params:
//      - array - first array (destination)
//      - arr2 - second array (doesn't have to be dynamic)
//      - len2 - length of arr2
extern void array_concat(DynArray *array, void *arr2, size_t len2);

// remove given count of elements from the front of the array
// params:
//      - array - array to remove from
//      - to_remove - count of elements to remove
extern void array_remove_front(DynArray *array, size_t to_remove);

// remove given count of elements from the back of the array
// params:
//      - array - array to remove from
//      - to_remove - count of elements to remove
extern void array_remove_back(DynArray *array, size_t to_remove);

// destroy the array
// params:
//      - array - array to destroy
extern void array_destroy(DynArray array);

#endif
