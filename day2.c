//
// Created by lennart on 12/2/24.
//

#include <string.h>

#include "aoc.h"

void run_day2() {
    FILE *f = fopen("input.txt", "r");
    char *input = aoc_read_file(f);

    int result1 = 0, result2 = 0;
    input = strtok(input, "\n");
    while (input != NULL) {
        // plusmid's hybrid parser - parse data and compare immediately
        bool increasing = true, decreasing = true;
        int failures = 0;

        int lastValue = strtod(input, &input); // Start with first value
        char *next = input;
        while (true) {
            input = next;
            int value = strtod(input, &next);
            if (input == next) {
                break;
            }

            // Check if always increasing or decreasing
            if (increasing && value <= lastValue) increasing = false;
            if (decreasing && value >= lastValue) decreasing = false;

            // If ordered and not too different
            if ((!increasing && !decreasing) || (abs(value - lastValue) > 3)) {
                failures++;
            }

            // Update & go to next
            lastValue = value;
        }

        // go to next line
        input = strtok(nullptr, "\n");
    }
    printf("Result: %d\n", result);

    fclose(f);
}
