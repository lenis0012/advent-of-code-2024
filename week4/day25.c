//
// Created by lennart on 12/28/24.
//

#include "aoc.h"
#include <stdint.h>

// typedef uint8_t pins_t[5];

typedef struct {
    uint8_t values[5];
} pins_t;

#define LIST_NAME locks
#define LIST_ELEMENT pins_t
#include "list.h"

static locks_t *locks;
static locks_t *keys;

void run_day25() {
    aoc_begin();
    printf("Hello, day 25!\n");
    locks = locks_new();
    keys = locks_new();

    // Read patterns
    while (!aoc_eof()) {
        char line[8];
        aoc_line(line, 8);
        locks_t *dest = keys;
        if (strncmp(line, "#####", 5) == 0) {
            dest = locks;
        }

        pins_t pins = {};
        for (int i = 0; i < 5; i++) {
            aoc_line(line, 8);
            for (int j = 0; j < 5; j++) {
                if (line[j] == '#') {
                    pins.values[j] += 1;
                }
            }
        }

        // Skip next 2 lines
        aoc_line(line, 8);
        aoc_line(line, 8);

        locks_add(dest, pins);
    }

    // Find matching keys and locks
    unsigned int result = 0;
    for (int l = 0; l < locks->size; l++) {
        pins_t lock = locks_get(locks, l);
        for (int k = 0; k < keys->size; k++) {
            pins_t key = locks_get(keys, k);

            unsigned int faults = 0;
            for (int p = 0; p < 5; p++) {
                if (lock.values[p] + key.values[p] > 5) {
                    faults++;
                }
            }

            if (faults == 0) {
                result++;
            }
        }
    }
    printf("Result: %u\n", result);

    locks_destroy(locks);
    locks_destroy(keys);
    aoc_end();
}
