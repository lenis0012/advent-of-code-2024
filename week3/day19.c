//
// Created by lennart on 12/20/24.
//

#include <aoc.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256

#define LIST_NAME towels
#define LIST_ELEMENT char*
#define LIST_UNORDERED
#define LIST_EQ(a, b) (a == b)
#include <stdint.h>

#include "list.h"

towels_t* towels_az['z' - 'a'] = {};

static bool parse_towel(char *towel) {
    aoc_eat(", ");
    if (aoc_eof()) {
        return false;
    }

    char c;
    while ((c = aoc_char()) >= 'a' && c <= 'z') {
        *(towel++) = c;
    }
    *towel = '\0';
    return true;
}

static char* get_lowest(towels_t *towels) {
    char *lowest = (char*) SIZE_MAX;
    int lowest_idx = 0;
    for (int i = 0; i < towels->size; i++) {
        char *towel = towels_get(towels, i);
        if (towel < lowest) {
            lowest = towel;
            lowest_idx = i;
        }
    }

    towels_remove_at(towels, lowest_idx);
    return lowest;
}

static bool solve(char *layout) {
    towels_t *open = towels_new();
    towels_add(open, layout);

    bool success = false;
    while (open->size > 0) {
        char *cursor = get_lowest(open);
        if (*cursor == '\0') {
            success = true;
            break;
        }

        towels_t *towels = towels_az[*cursor - 'a'];
        if (towels == nullptr) continue;
        for (int i = 0; i < towels->size; i++) {
            char *towel = towels->elements[i];

            unsigned int tlen = strlen(towel);
            if (strncmp(towel, cursor, tlen) == 0 && towels_index_of(open, cursor + tlen) == -1) {
                towels_add(open, cursor + tlen);
            }
        }
    }

    towels_destroy(open);
    return success;
}

void run_day19() {
    aoc_begin();

    // Read towels
    char towel[BUFFER_SIZE] = {};
    while (parse_towel(towel) && strlen(towel) > 0) {
        towels_t *towels = towels_az[*towel - 'a'];
        if (towels == nullptr) {
            towels = towels_az[*towel - 'a'] = towels_new();
        }
        towels_add(towels, strdup(towel));
    }

    int result = 0;
    while (!aoc_eof()) {
        char *layout = aoc_line(towel, BUFFER_SIZE);
        if (strlen(layout) == 0) continue;

        if (solve(layout)) {
            result++;
        }
    }
    printf("Result: %d\n", result);

    aoc_end();
}
