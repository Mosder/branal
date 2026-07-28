#include "structures/array.h"

#include <stdlib.h>
#include <string.h>

#include "utils/memory.h"

DynArray array_new(size_t el_size, size_t capacity, void (*el_cleanup)(void *el)) {
    // clang-format off
    DynArray array = {
        .data = safe_malloc(capacity * el_size),
        .el_size = el_size,
        .count = 0,
        .capacity = capacity,
        .el_cleanup = el_cleanup
    };
    // clang-format on
    return array;
}

void *array_get(DynArray array, size_t index) {
    if (index >= array.count)
        return NULL;
    return (char *)array.data + index * array.el_size;
}

void *array_next(DynArray array, void *prev) {
    // if NULL - return first or NULL if count is 0
    if (!prev)
        return array.count == 0 ? NULL : array.data;

    ptrdiff_t byte_diff = (char *)prev - (char *)array.data;

    // if outside of range / prev was last - return NULL
    if (byte_diff < 0 || (size_t)byte_diff >= (array.count - 1) * array.el_size)
        return NULL;

    return (char *)prev + array.el_size;
}

// expand an array
void array_expand(DynArray *array, size_t to_fit) {
    while (array->capacity < to_fit)
        array->capacity *= 2;
    array->data = safe_realloc(array->data, array->capacity * array->el_size);
}

void array_put(DynArray *array, size_t index, void *val) {
    if (index >= array->capacity)
        array_expand(array, index + 1);

    if (index < array->count && array->el_cleanup)
        array->el_cleanup(array_get(*array, index));

    memcpy((char *)array->data + index * array->el_size, val, array->el_size);

    if (index >= array->count)
        array->count = index + 1;
}

void array_append(DynArray *array, void *val) {
    if (array->count >= array->capacity)
        array_expand(array, array->count + 1);
    memcpy((char *)array->data + array->count++ * array->el_size, val, array->el_size);
}

void array_concat(DynArray *array, void *arr2, size_t len2) {
    if (array->count + len2 > array->capacity)
        array_expand(array, array->count + len2);
    memcpy((char *)array->data + array->count * array->el_size, arr2, len2 * array->el_size);
    array->count += len2;
}

void array_remove_front(DynArray *array, size_t to_remove) {
    if (to_remove >= array->count) {
        array->count = 0;
        return;
    }

    if (array->el_cleanup) {
        for (size_t i = 0; i < to_remove; i++)
            array->el_cleanup(array_get(*array, i));
    }

    array->count -= to_remove;
    memmove(array->data, (char *)array->data + to_remove * array->el_size, array->count * array->el_size);
}

void array_remove_back(DynArray *array, size_t to_remove) {
    if (to_remove >= array->count) {
        array->count = 0;
        return;
    }

    if (array->el_cleanup) {
        for (size_t i = 0; i < to_remove; i++)
            array->el_cleanup(array_get(*array, array->count - i - 1));
    }

    array->count -= to_remove;
}

void array_destroy(DynArray array) {
    if (array.el_cleanup) {
        void *el = NULL;
        while ((el = array_next(array, el)))
            array.el_cleanup(el);
    }
    free(array.data);
}
