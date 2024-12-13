//
// Created by lennart on 12/5/24.
//

#define LINEBUF_SIZE 128

#include <stdio.h>

typedef struct {
    int before;
    int after;
} Rule;

#define LIST_NAME rules
#define LIST_ELEMENT Rule
#include "list.h"

#define LIST_NAME update
#define LIST_ELEMENT int
#define LIST_EQ(a, b) a == b
#include "list.h"

rules_t *rules;

void read_expect(FILE *f, char expected) {
    assert(getc(f) == expected);
}

void run_day5() {
    FILE *f = fopen("input.txt", "r");
    char line[LINEBUF_SIZE];

    // Reading all rules
    rules = rules_new();
    fgets(line, LINEBUF_SIZE, f);
    while (strlen(line) > 1) {
        int x, y;
        char *cursor = line;
        x = strtod(cursor, &cursor);
        char c = *(cursor++); assert(c == '|');
        y = strtod(cursor, &cursor);
        rules_add(rules, (Rule) { x, y });
        fgets(line, LINEBUF_SIZE, f);
    }
    fpos_t data_position;
    fgetpos(f, &data_position);

    // Read all updates
    int result = 0;
    update_t *update = update_new();
    while (!feof(f)) {
        update_clear(update);
        fgets(line, LINEBUF_SIZE, f);
        char *cursor = line;
        while (*cursor != '\n' && *cursor != 0) {
            int page = strtod(cursor, &cursor);
            update_add(update, page);
            if (*cursor == ',') cursor++;
        }

        // Check against rules
        for (int r = 0; r < rules->size; r++) {
            Rule rule = rules_get(rules, r);
            int beforeAt = update_index_of(update, rule.before);
            int afterAt = update_index_of(update, rule.after);
            if (beforeAt != -1 && afterAt != -1 && beforeAt > afterAt) {
                goto next_update;
            }
        }

        // Sum
        result += update_get(update, update->size / 2);
        next_update:
    }
    printf("Result 1: %d\n", result);

    // Part 2
    result = 0;
    fsetpos(f, &data_position);
    int updi = 0;
    while (!feof(f)) {
        update_clear(update);
        fgets(line, LINEBUF_SIZE, f);
        char *cursor = line;
        while (*cursor != '\n' && *cursor != 0) {
            int page = strtod(cursor, &cursor);
            update_add(update, page);
            if (*cursor == ',') cursor++;
        }

        // Correct the update
        bool correct = true;
        // printf("Processing update %d\n", updi++);
        for (int r = 0; r < rules->size; r++) {
            Rule rule = rules_get(rules, r);
            int beforeAt = update_index_of(update, rule.before);
            int afterAt = update_index_of(update, rule.after);
            if (beforeAt != -1 && afterAt != -1 && beforeAt > afterAt) {
                // printf("Swapping page %d with %d at index [%d, %d]\n", rule.before, rule.after, beforeAt, afterAt);
                update->elements[afterAt] = rule.before;
                update->elements[beforeAt] = rule.after;
                r = -1;
                correct = false;
            }
        }

        // printf("Final order:");
        // for (int p = 0; p < update->size; p++) {
        //     printf(" %d", update->elements[p]);
        // }
        // printf("\n");

        // Sum
        if (!correct) {
            result += update_get(update, update->size / 2);
        }
    }
    printf("Result 2: %d\n", result);

    fclose(f);
    update_destroy(update);
    rules_destroy(rules);
}
