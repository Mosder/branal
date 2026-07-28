#include "structures/hashmap.h"

#include <stdlib.h>
#include <string.h>

#include "utils/memory.h"

HashMap *hashmap_new(
    size_t key_size,
    size_t val_size,
    size_t (*hash)(void *key),
    int (*compare)(void *key1, void *key2),
    void (*key_cleanup)(void *key),
    void (*val_cleanup)(void *val)
) {
    HashMap *map = safe_malloc(sizeof(HashMap));
    // initialize NULLs
    memset(map->table, 0, HASHMAP_TABLE_SIZE * sizeof(HashMapNode *));
    map->key_size = key_size;
    map->val_size = val_size;
    map->hash = hash;
    map->compare = compare;
    map->key_cleanup = key_cleanup;
    map->val_cleanup = val_cleanup;
    return map;
}

void hashmap_put(HashMap *map, void *key, void *val) {
    size_t key_hash = map->hash(key);
    HashMapNode *node = map->table[key_hash];

    // check if it exists
    while (node) {
        // if it exists - update node
        if (map->compare(key, node->key)) {
            // ensure no hanging memory allocation
            if (map->val_cleanup) {
                map->val_cleanup(node->val);
            }
            memcpy(node->val, val, map->val_size);
            return;
        }
        node = node->next;
    }

    // if it doesn't exist - create new node
    HashMapNode *new_node = safe_malloc(sizeof(HashMapNode));
    new_node->key = safe_malloc(map->key_size);
    new_node->val = safe_malloc(map->val_size);
    memcpy(new_node->key, key, map->key_size);
    memcpy(new_node->val, val, map->val_size);

    // put in front of the linked list
    new_node->next = map->table[key_hash];
    map->table[key_hash] = new_node;
}

void *hashmap_get(HashMap *map, void *key) {
    size_t key_hash = map->hash(key);
    HashMapNode *node = map->table[key_hash];

    // check if it exists
    while (node) {
        // if it exists - return the value pointer
        if (map->compare(key, node->key)) {
            return node->val;
        }
        node = node->next;
    }

    // if it doesn't exist - return NULL
    return NULL;
}

void free_node_without_next(HashMap *map, HashMapNode *node) {
    if (map->key_cleanup) {
        map->key_cleanup(node->key);
    }
    if (map->val_cleanup) {
        map->val_cleanup(node->val);
    }
    free(node->key);
    free(node->val);
    free(node);
}

void hashmap_delete(HashMap *map, void *key) {
    size_t key_hash = map->hash(key);
    HashMapNode **p_node = &map->table[key_hash];

    // check if it exists
    while (*p_node) {
        // if it exists - delete it
        if (map->compare(key, (*p_node)->key)) {
            HashMapNode *next = (*p_node)->next;
            free_node_without_next(map, *p_node);
            *p_node = next;
            return;
        }
        p_node = &(*p_node)->next;
    }
}

void hashmap_free(HashMap *map) {
    if (!map)
        return;
    for (size_t i = 0; i < HASHMAP_TABLE_SIZE; i++) {
        HashMapNode *curr = map->table[i];
        while (curr) {
            HashMapNode *next = curr->next;
            free_node_without_next(map, curr);
            curr = next;
        }
    }
    free(map);
}
