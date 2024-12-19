//
// Created by lennart on 12/17/24.
//

#include <stdint.h>
#include <tgmath.h>

#include "aoc.h"

typedef enum : uint8_t {
    ADV,
    BXL,
    BST,
    JNZ,
    BXC,
    OUT,
    BDV,
    CDV
} Opcode;

#define LIST_ELEMENT unsigned int
#include "list.h"

list_t *instructions;
unsigned long A, B, C;

static unsigned long combo(unsigned int combo) {
    if (combo <= 3) {
        return combo;
    }

    if (combo == 4) return A;
    if (combo == 5) return B;
    if (combo == 6) return C;

    return 0;
}

static void simulate(list_t *output) {
    unsigned int ip = 0;

    while (ip < instructions->size) {
        Opcode opcode = list_get(instructions, ip++);
        unsigned int operand = list_get(instructions, ip++);

        switch (opcode) {
        case ADV:
            // printf("ADV %lu\n", combo(operand));
            A = A >> combo(operand);
            break;
        case BXL:
            // printf("BXL %lu\n", (operand));
            B = B ^ operand;
            break;
        case BST:
            // printf("BST %lu\n", combo(operand));
            B = combo(operand) % 8;
            break;
        case JNZ:
            // printf("JNZ %lu\n", (operand));
            if (A != 0) ip = operand;
            break;
        case BXC:
            // printf("BXC %lu\n", (operand));
            B = B ^ C;
            break;
        case OUT:
            // printf("OUT %lu\n", combo(operand));
            list_add(output, combo(operand) % 8);
            break;
        case BDV:
            // printf("BDV %lu\n", combo(operand));
            B = A >> combo(operand);
            break;
        case CDV:
            // printf("CDV %lu\n", combo(operand));
            C = A >> combo(operand);
            break;
        }
    }
}

static void print_list(list_t *list) {
    for (int i = 0; i < list->size; i++) {
        printf(i == 0 ? "%u" : ", %u", list_get(list, i));
    }
    printf("\n");
}

static bool list_eq(list_t *a, list_t *b) {
    if (a->size != b->size) return false;

    for (int i = 0; i < a->size; i++) {
        if (list_get(a, i) != list_get(b, i)) {
            return false;
        }
    }

    return true;
}

void run_day17() {
    aoc_begin();

    aoc_scan("Register A: %u\n", &A);
    aoc_scan("Register B: %u\n", &B);
    aoc_scan("Register C: %u\n", &C);

    aoc_eat("\r\n");
    aoc_expect("Program: ");
    char *input = aoc_line(nullptr, 0);

    instructions = list_new();
    char *cursor = input;
    while (*cursor != '\0') {
        if (*cursor == ',') cursor++;
        list_add(instructions, strtoul(cursor, &cursor, 10));
    }

    list_t *output = list_new();
    simulate(output);
    printf("Result 1: "); print_list(output);

    unsigned long i = pow(8, 15);
    for (int j = 14; j >= 0; j--) {
        unsigned long inc = pow(8, j);

        next:
        A = i;
        output->size = 0;
        simulate(output);
        if (list_get(output, j) != list_get(instructions, j) || list_get(output, j+1) != list_get(instructions, j+1)) {
            i += inc;
            goto next;
        }
    }
    printf("Result 2: "); print_list(output);
    printf("Result 2: %lu\n", i);

    aoc_end();
}
