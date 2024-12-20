//
// Created by lennart on 12/20/24.
//

#include <aoc.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256

#define LIST_NAME towels
#define LIST_ELEMENT char*
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

static bool solve(char *layout) {
    printf("%s\n", layout);
    char c = *layout;
    if (c == '\0') {
        return true;
    }

    towels_t *towels = towels_az[c - 'a'];
    if (towels == nullptr) {
        return false;
    }

    for (int i = 0; i < towels->size; i++) {
        char *towel = towels_get(towels, i);
        unsigned int stripes = strlen(towel);

        if (strncmp(towel, layout, stripes) == 0) {
            if (stripes == strlen(layout) || solve(layout + stripes)) {
                return true;
            }
        }
    }

    return false;
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
        printf("Solving towels for: %s\n", layout);
        if (strlen(layout) == 0) continue;

        if (solve(layout)) {
            printf("Solved\n");
            result++;
        }
    }
    printf("Result: %d\n", result);

    aoc_end();
}
