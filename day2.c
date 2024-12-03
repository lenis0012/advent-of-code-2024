//
// Created by lennart on 12/2/24.
//

#include <string.h>

#include "aoc.h"

struct Array {
    int *values;
    unsigned int length;
};

struct Array parse_line(char *line) {
    struct Array array = {
        .values = calloc(1024, sizeof(int)),
        .length = 0
    };

    char *next = line;
    while (true) {
        line = next;
        int value = strtod(line, &next);
        if (line == next) {
            break;
        }

        array.values[array.length++] = value;
    }

    return array;
}

int is_increasing(struct Array levels, int exclude) {
    int last = levels.values[0], start = 1;
    if (exclude == 0) {
        last = levels.values[1];
        start += 1;
    }

    for (int i = start; i < levels.length; i++) {
        if (i == exclude) continue;
        if (last >= levels.values[i]) {
            return i;
        }
        last = levels.values[i];
    }

    return 0;
}

int is_decreasing(struct Array levels, int exclude) {
    int last = levels.values[0], start = 1;
    if (exclude == 0) {
        last = levels.values[1];
        start += 1;
    }

    for (int i = start; i < levels.length; i++) {
        if (i == exclude) continue;
        if (last <= levels.values[i]) {
            return i;
        }
        last = levels.values[i];
    }

    return 0;
}

int is_similar(struct Array levels, int exclude) {
    int last = levels.values[0], start = 1;
    if (exclude == 0) {
        last = levels.values[1];
        start += 1;
    }

    for (int i = start; i < levels.length; i++) {
        if (i == exclude) continue;
        int distance = abs(last - levels.values[i]);
        if (distance == 0 || distance > 3) {
            return i;
        }
        last = levels.values[i];
    }

    return 0;
}

bool is_safe(struct Array levels, int exclude) {
    return (is_increasing(levels, exclude) == 0 || is_decreasing(levels, exclude) == 0) && is_similar(levels, exclude) == 0;
}

void run_day2() {
    FILE *f = fopen("input.txt", "r");
    char *input = aoc_read_file(f);

    int result1 = 0, result2 = 0;
    input = strtok(input, "\n");
    while (input != NULL) {
        struct Array levels = parse_line(input);

        if (is_safe(levels, -1)) {
            result1++;
            result2++;
        } else {
            int fIncreasing = is_increasing(levels, -1);
            int fDecreasing = is_decreasing(levels, -1);
            int fSimilar = is_similar(levels, -1);
            if (is_safe(levels, fIncreasing) || is_safe(levels, fDecreasing) || is_safe(levels, fSimilar)) {
                result2++;
            }
        }

        // go to next line
        free(levels.values);
        input = strtok(nullptr, "\n");
    }
    printf("Result 1: %d\n", result1);
    printf("Result 2: %d\n", result2);

    fclose(f);
}
