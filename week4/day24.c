//
// Created by lennart on 12/24/24.
//

#include <string.h>

#include "aoc.h"

#define FNV_BASIS 2166136261
#define FNV_PRIME 16777619

typedef enum {
    UNKNOWN = 0,
    TRUE,
    FALSE
} Value;

static int hash_str(char *str) {
    int hash = FNV_BASIS;
    while (*str != '\0') {
        hash = hash ^ *(str++);
        hash = hash * FNV_PRIME;
    }
    return hash;
}

typedef struct {
    char *inputA;
    char *inputB;
    char *output;
    enum Operation {
        AND,
        OR,
        XOR
    } operation;
} Gate;

#define MAP_NAME wires
#define MAP_K char*
#define MAP_V Value
#define MAP_EQ(ka, kb) (strcmp(ka, kb) == 0)
#define MAP_HASH(v) hash_str(v)
#include "hashmap.h"

#define LIST_NAME gates
#define LIST_ELEMENT Gate*
#include "list.h"

#define LIST_NAME inputs
#define LIST_ELEMENT char*
#include "list.h"

static wires_t *wires;
static gates_t *gates;

static int wire_compare(void *a, void *b) {
    char **astr = a, **bstr = b;
    return strcmp(*astr, *bstr);
}

void run_day24() {
    aoc_begin();

    printf("Hello, day 24\n");
    wires = wires_create(1 << 6);
    gates = gates_new();

    // Read wires
    char line[32];
    while (!aoc_eof()) {
        char name[4];
        unsigned int value, count = 0;

        // constrain scanning to single line
        aoc_line(line, 32);
        if (sscanf(line, "%3s: %u%n\n", name, &value, &count) == 0 || count == 0) {
            break; // empty line
        }

        wires_put(wires, strdup(name), value == 1 ? TRUE : FALSE);
    }

    // Read gates
    while (!aoc_eof()) {
        char inputA[4], inputB[4], operation[4], output[4];
        if (aoc_scan("%3s %3s %3s -> %3s\n", inputA, operation, inputB, output) == 0) {
            break; // empty line
        }

        Gate *gate = malloc(sizeof(Gate));
        gate->inputA = strdup(inputA);
        gate->inputB = strdup(inputB);
        gate->output = strdup(output);
        if (strcmp(operation, "AND") == 0) gate->operation = AND;
        if (strcmp(operation, "OR") == 0) gate->operation = OR;
        if (strcmp(operation, "XOR") == 0) gate->operation = XOR;

        gates_add(gates, gate);
    }

    // Read initial inputs
    inputs_t *inputs = inputs_new();
    wires_node_t *node = nullptr;
    while ((node = wires_next(wires, node)) != nullptr) {
        inputs_add(inputs, node->key);
    }

    // Process inputs and gates
    for (int i = 0; i < inputs->size; i++) {
        char *input = inputs_get(inputs, i);

        for (int g = 0; g < gates->size; g++) {
            Gate *gate = gates_get(gates, g);
            if (strcmp(gate->inputA, input) != 0 && strcmp(gate->inputB, input) != 0) {
                continue; // Not a member
            }

            Value value = wires_get(wires, gate->output);
            if (value != UNKNOWN) {
                continue; // Already computed
            }

            Value va = wires_get(wires, gate->inputA);
            Value vb = wires_get(wires, gate->inputB);
            if (va == UNKNOWN || vb == UNKNOWN) {
                continue; // One output still unknown
            }

            // Process gate value
            Value result = UNKNOWN;
            switch (gate->operation) {
            case AND: result = va == TRUE && vb == TRUE ? TRUE : FALSE; break;
            case OR: result = va == TRUE || vb == TRUE ? TRUE : FALSE; break;
            case XOR:
                result = ((va == TRUE && vb == FALSE) || (va == FALSE && vb == TRUE))
                    ? TRUE : FALSE;
                break;
            }

            // Write result to output wire
            wires_put(wires, gate->output, result);
            inputs_add(inputs, gate->output);
        }
    }

    // Process output Z values into decimal number
    unsigned long result = 0, bit = 0;
    qsort(inputs->elements, inputs->size, sizeof(char*), (__compar_fn_t) wire_compare);
    for (int i = 0; i < inputs->size; i++) {
        char *wire = inputs_get(inputs, i);
        if (wire[0] == 'z') {
            result += (wires_get(wires, wire) == TRUE ? 1L : 0L) << bit++;
        }
    }
    printf("Result: %lu\n", result);

    aoc_end();
}
