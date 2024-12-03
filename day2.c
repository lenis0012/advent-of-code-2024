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

struct Array copy_levels_without(struct Array original, int index) {
    struct Array array = {
        .values = calloc(1024, sizeof(int)),
        .length = original.length - 1
    };

    memcpy(array.values, original.values, index * sizeof(int));
    memcpy(&array.values[index], &original.values[index + 1], (original.length - index - 1) * sizeof(int));
    return array;
}

int is_increasing(struct Array levels) {
    int last = levels.values[0];

    for (int i = 1; i < levels.length; i++) {
        if (last >= levels.values[i]) {
            return i;
        }
        last = levels.values[i];
    }

    return 0;
}

int is_decreasing(struct Array levels) {
    int last = levels.values[0];

    for (int i = 1; i < levels.length; i++) {
        if (last <= levels.values[i]) {
            return i;
        }
        last = levels.values[i];
    }

    return 0;
}

int is_similar(struct Array levels) {
    int last = levels.values[0];

    for (int i = 1; i < levels.length; i++) {
        int distance = abs(last - levels.values[i]);
        if (distance == 0 || distance > 3) {
            return i;
        }
        last = levels.values[i];
    }

    return 0;
}

bool levels_valid(struct Array levels) {
    return is_similar(levels) == 0 && (is_increasing(levels) == 0 || is_decreasing(levels) == 0);
}

void run_day2() {
    FILE *f = fopen("input.txt", "r");
    char *input = aoc_read_file(f);

    int result1 = 0, result2 = 0;
    input = strtok(input, "\n");
    while (input != NULL) {
        struct Array levels = parse_line(input);

        bool valid = levels_valid(levels);
        result1 += valid ? 1 : 0;

        if (
            valid
            || levels_valid(copy_levels_without(levels, is_increasing(levels)))
            || levels_valid(copy_levels_without(levels, is_decreasing(levels)))
            || levels_valid(copy_levels_without(levels, is_similar(levels)))
        ) {
            result2 += 1;
        }

        // go to next line
        free(levels.values);
        input = strtok(nullptr, "\n");
    }
    printf("Result 1: %d\n", result1);
    printf("Result 2: %d\n", result2);

    fclose(f);
}
