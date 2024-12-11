//
// Created by lennart on 12/11/24.
//

#include "aoc.h"
#include <stdint.h>

#define LIST_NAME stones
#define LIST_ELEMENT uint64_t
#include "list.h"

#define MAP_NAME cache
#define MAP_K uint64_t
#define MAP_V uint64_t
#define MAP_HASH(v) (((int) (v >> 8)) * 2166136261 + (int) (v & 0Xff))
#include "hashmap.h"

cache_t *cache;

static void parse_stones(stones_t *stones) {
    int value;
    while (aoc_parse_int(&value)) {
        stones_add(stones, value);
    }
}

static inline unsigned int get_digits(uint64_t value) {
    unsigned int d = 1;
    uint64_t v = 10;
    while (value >= v) {
        v *= 10;
        d++;
    }

    return d;
}

static uint64_t stone_key(uint64_t stone, int iterations) {
    return (stone << 8) | ((unsigned int) iterations & 0xFF);
}

static uint64_t blink(uint64_t stone, int iterations) {
    if (iterations == 0) {
        return 1;
    }

    uint64_t key = stone_key(stone, iterations);
    uint64_t cv = cache_get(cache, key);
    if (cv != 0) {
        return cv;
    }

    unsigned int digits;
    uint64_t result;
    if (stone == 0) {
        result = blink(1, iterations - 1);
    } else if ((digits = get_digits(stone)) % 2 == 0) {
        unsigned int half = digits / 2;
        uint64_t scale = 1;
        for (int j = 0 ; j < half; j++) {
            scale *= 10;
        }

        uint64_t rhs = stone % scale;
        uint64_t lhs = stone / scale;

        result = blink(lhs, iterations - 1) + blink(rhs, iterations - 1);
    } else {
        result = blink(stone * 2024, iterations - 1);
    }

    cache_put(cache, key, result);
    return result;
}

void run_day11() {
    aoc_begin();

    stones_t stones; stones_init(&stones);
    parse_stones(&stones);

    cache = cache_create(1048576);

    uint64_t result = 0;
    for (int i = 0; i < stones.size; i++) {
        result += blink(stones_get(&stones, i), 75);
    }
    printf("Result: %llu\n", result);

    cache_destroy(cache);
    aoc_end();
}
