// structures/hashmap.h
// a simple implementation of hashmap using linked list

#include <stddef.h>

#define HASHMAP_TABLE_SIZE 128

typedef struct HashMapNode {
    void *key;
    void *val;
    struct HashMapNode *next;
} HashMapNode;

typedef struct {
    HashMapNode *table[HASHMAP_TABLE_SIZE];
    size_t key_size;
    size_t val_size;
    size_t (*hash)(void *key);
    int (*compare)(void *key1, void *key2);
    void (*key_cleanup)(void *key);
    void (*val_cleanup)(void *val);
} HashMap;

// create a new hash map
// params:
//      - key_size - size of the key type
//      - val_size - size of the value type
//      - hash - function for hashing keys
//        params:
//             - key - pointer to the key to hash
//        returns:
//             hash of the key (size_t in [0; HASHMAP_TABLE_SIZE))
//      - compare - function for comparing keys
//        params:
//             - key1 - pointer to the first of the keys to compare
//             - key2 - pointer to the second of the keys to compare
//        returns:
//             truthy if the keys are identical, falsy if they are different
//      - key_cleanup - function for cleaning up any internal allocations inside node->key (not the key itself)
//                      if it has no heap allocations, this should be NULL
//        params:
//             - key - pointer to the key to cleanup
//      - val_cleanup - function for cleaning up any internal allocations inside node->val (not the value itself)
//                      if it has no heap allocations, this should be NULL
//        params:
//             - val - pointer to the value to cleanup
// returns:
//      pointer to the new hash map
extern HashMap *hashmap_new(
    size_t key_size,
    size_t val_size,
    size_t (*hash)(void *key),
    int (*compare)(void *key1, void *key2),
    void (*key_cleanup)(void *key),
    void (*val_cleanup)(void *val)
);

// create/update an entry in a hash map
// params:
//      - map - hash map to insert to
//      - key - pointer to the key
//      - val - pointer to the value
extern void hashmap_put(HashMap *map, void *key, void *val);

// get the value of given key from a hash map
// params:
//      - map - hash map to get from
//      - key - pointer to the key of the value to get
// returns:
//      pointer to the value of specified key or NULL if it doesn't exist
extern void *hashmap_get(HashMap *map, void *key);

// delete an entry from a hash map
// params:
//      - map - hash map to delete from
//      - key - pointer to the key of the entry to delete
extern void hashmap_delete(HashMap *map, void *key);

// free the hash map
//      - map - hash map to free
extern void hashmap_free(HashMap *map);
