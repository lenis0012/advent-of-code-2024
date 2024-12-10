//
// Created by lennart on 12/7/24.
//

#define LINEBUF_SIZE 256
#define MAX_NUMBERS 64

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

typedef enum {
    OP_ADD,
    OP_MUL,
    OP_CAT,
    OP__Count
} Operation;

typedef struct {
    int value;
    Operation operation;
    uint32_t shift;
} Operand;

static uint64_t eval(const Operand *operands, int noperands) {
    uint64_t value = operands[0].value;
    for (int i = 1; i < noperands; i++) {
        switch (operands[i].operation) {
        case OP_ADD: value += operands[i].value; break;
        case OP_MUL: value *= operands[i].value; break;
        case OP_CAT: {
            // uint32_t appending = operands[i].value;
            // uint32_t d = 1;
            // while (appending >= d) {
            //     value *= 10;
            //     d *= 10;
            // }
            // value += appending;
            value = value * operands[i].shift + operands[i].value;
        }
        }
    }

    return value;
}

static bool solve(uint64_t test, Operand *operands, int coperand, int noperands) {
    Operand *opr = &operands[coperand];

    for (Operation op = OP_ADD; op < OP__Count; op++) {
        opr->operation = op;
        if (eval(operands, noperands) == test) {
            return true;
        }
        if (noperands > coperand + 1 && solve(test, operands, coperand + 1, noperands)) {
            return true;
        }
    }

    return false;
}

void run_day7() {
    FILE *f = fopen("input.txt", "r");

    Operand operands[MAX_NUMBERS];
    char line[LINEBUF_SIZE];

    uint64_t result = 0;

    while (fgets(line, LINEBUF_SIZE, f) != nullptr) {
        char *cursor;
        uint64_t test = strtoull(line, &cursor, 10);
        assert(*cursor == ':');

        int nvalues = 0;
        while (*cursor != '\n' && *cursor != 0) {
            if (*cursor == ' ') cursor++;
            char *next;
            int value = strtod(cursor, &next);
            if (next == cursor) {
                cursor++;
                continue;
            }
            uint16_t digits = next - cursor;
            cursor = next;
            operands[nvalues++] = (Operand) {
                .value = value,
                .operation = OP_ADD,
                .shift = (uint64_t) pow(10, digits)
            };
        }

        if (solve(test, operands, 1, nvalues)) {
            result += test;
        }
    }

    printf("Result: %lu\n", result);

    fclose(f);
}
