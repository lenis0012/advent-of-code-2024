//
// Created by lennart on 12/3/24.
//

#include "aoc.h"
#include <stdio.h>
#include <string.h>

bool parse_expect(char *data, int *bytesread, const char *word) {
    for (int i = 0; i < strlen(word); i++) {
        char c = data[i];
        if (bytesread != nullptr) *bytesread += 1;
        if (c == 0) {
            return false;
        }

        if (c != word[i]) {
            return false;
        }
    }

    return true;
}

bool parse_number(char *data, int *bytesread, int *numout) {
    char *endptr;
    int value = strtod(data, &endptr);

    if (endptr == data) {
        return false;
    }

    *bytesread += (int) (endptr - data);
    *numout = value;
    return true;
}

void parse_eval(char *data, int *nbytesread, int *accumulator) {
    int pos = 0;
    if (parse_expect(&data[pos], &pos, "mul(")) {
        int a, b;
        if (parse_number(&data[pos], &pos, &a)
            && parse_expect(&data[pos], &pos, ",")
            && parse_number(&data[pos], &pos, &b)
        ) {
            if (parse_expect(&data[pos], &pos, ")")) {
                *accumulator += a * b;
            }
        }
    }

    *nbytesread += pos;
}

void run_day3() {
    FILE *f = fopen("input.txt", "r");
    char *data = aoc_read_file(f);
    uint data_len = strlen(data);
    fclose(f);

    // Part 1
    int cursor = 0, result = 0;
    while (cursor < data_len) {
        parse_eval(&data[cursor], &cursor, &result);
    }
    printf("Result 1: %d\n", result);

    // Part 2
    cursor = 0; result = 0;
    bool do_eval = true;
    while (cursor < data_len) {
        if (do_eval && parse_expect(&data[cursor], nullptr, "don't()")) {
            // stop evaluating
            do_eval = false;
            cursor += strlen("don't()");
        } else if (!do_eval && parse_expect(&data[cursor], nullptr, "do()")) {
            // resume evaluating
            do_eval = true;
            cursor += strlen("do()");
        } else if (!do_eval) {
            // not evaluating, continue on next character
            cursor += 1;
        } else {
            // evaluate like in part 1
            parse_eval(&data[cursor], &cursor, &result);
        }
    }
    printf("Result 2: %d\n", result);

    free(data);
}
