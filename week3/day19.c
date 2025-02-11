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

static long solve(const char *layout) {
    towels_t *open = towels_new();
    towels_add(open, (char*) layout);

    long reoccurrences[strlen(layout) + 1] = {};
    reoccurrences[0] = 1;

    long patterns = 0;
    while (open->size > 0) {
        char *cursor = get_lowest(open);
        if (*cursor == '\0') {
            patterns += reoccurrences[cursor - layout];
            continue;
        }

        towels_t *towels = towels_az[*cursor - 'a'];
        if (towels == nullptr) continue;

        for (int i = 0; i < towels->size; i++) {
            char *towel = towels->elements[i];
            unsigned int tlen = strlen(towel);
            if (strncmp(towel, cursor, tlen) == 0) {
                if (towels_index_of(open, cursor + tlen) == -1) {
                    towels_add(open, cursor + tlen);
                }
                reoccurrences[cursor - layout + tlen] += reoccurrences[cursor - layout];
            }
        }
    }

    towels_destroy(open);
    return patterns;
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

    long result = 0, result2 = 0;
    while (!aoc_eof()) {
        char *layout = aoc_line(towel, BUFFER_SIZE);
        if (strlen(layout) == 0) continue;

        long patterns = solve(layout);
        result2 += patterns;
        if (patterns > 0) {
            result++;
        }
    }
    printf("Result 1: %ld\n", result);
    printf("Result 2: %ld\n", result2);

    aoc_end();
}
