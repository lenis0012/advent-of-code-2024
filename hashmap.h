//
// Created by lennart on 12/5/24.
//

#include <stdlib.h>
#include <assert.h>
#include <stdbit.h>

#ifndef MAP_NAME
#define MAP_NAME map
#endif

#ifndef MAP_K
#define MAP_K int
#endif

#ifndef MAP_V
#define MAP_V int
#endif

#ifndef MAP_HASH
#define MAP_HASH(v) v
#endif

#ifndef MAP_EQ
#define MAP_EQ(a, b) (a == b)
#endif

#define MAP_CONCAT_(a, b) a ## _ ## b
#define MAP_CONCAT(a, b) MAP_CONCAT_(a, b)
#define MAP_FUNC(name, rt) static rt MAP_CONCAT(MAP_NAME, name)

#define MAP_LF 0.7
#define MAP_T MAP_CONCAT(MAP_NAME, t)
#define MAP_NT MAP_CONCAT(MAP_NAME, node_t)

typedef struct {
    MAP_K key;
    MAP_V value;
    bool set;
} MAP_NT;

typedef struct {
    MAP_NT *nodes;
    unsigned int size;
    unsigned int capacity;
} MAP_T;

MAP_FUNC(create, MAP_T*)(unsigned int initial_capacity) {
    assert(stdc_count_ones(initial_capacity) == 1);
    MAP_T *map = malloc(sizeof(MAP_T));
    map->capacity = initial_capacity;
    map->nodes = calloc(map->capacity, sizeof(MAP_NT));
    map->size = 0;
    // map->keys = calloc(map->capacity, sizeof(MAP_K));
    // map->values = calloc(map->capacity, sizeof(MAP_V));
    return map;
}

MAP_FUNC(destroy, void)(MAP_T *map) {
    free(map->nodes);
    free(map);
}

MAP_FUNC(put, void)(MAP_T *map, MAP_K key, MAP_V value) {
    int hash = MAP_HASH(key);
    int pos = hash % map->capacity;
    while (true) {
        MAP_NT *node = &map->nodes[pos];
        if (!node->set) {
            if (map->size + 1 >= map->capacity * MAP_LF) {
                MAP_NT *onodes = map->nodes;
                unsigned int ocapacity = map->capacity;
                map->capacity *= 2;
                map->nodes = calloc(map->capacity, sizeof(MAP_NT));
                for (int i  = 0; i < ocapacity; i++) {
                    if (!onodes[i].set) continue;
                    MAP_CONCAT(MAP_NAME, put)(map, onodes[i].key, onodes[i].value);
                }
                free(onodes);
                MAP_CONCAT(MAP_NAME, put)(map, key, value);
                return;
            }
            map->size++;
            node->key = key;
            node->value = value;
            node->set = true;
            return;
        } if (MAP_EQ(node->key, key)) {
            node->key = key;
            node->value = value;
            return;
        }

        pos = ++pos % map->capacity;
    }
}

MAP_FUNC(get, MAP_V)(MAP_T *map, MAP_K key) {
    int hash = MAP_HASH(key);
    int pos = hash % map->capacity;
    while (true) {
        MAP_NT node = map->nodes[pos];
        if (!node.set) {
            return (MAP_V) 0;
        } if (MAP_EQ(node.key, key)) {
            return node.value;
        }

        pos = (++pos) % map->capacity;
    }
}

MAP_FUNC(next, MAP_NT*)(MAP_T *map, MAP_NT *prev) {
    if (prev == nullptr) {
        for (int i = 0; i < map->capacity; i++) {
            MAP_NT node = map->nodes[i];
            if (node.set) {
                return &map->nodes[i];
            }
        }
        return nullptr;
    }

    while (++prev < &map->nodes[map->capacity]) {
        if (prev->set) {
            return prev;
        }
    }

    return nullptr;
}

#undef MAP_NAME
#undef MAP_K
#undef MAP_V
#undef MAP_HASH
