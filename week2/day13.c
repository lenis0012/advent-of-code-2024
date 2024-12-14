//
// Created by lennart on 12/13/24.
//

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <tgmath.h>

#include "aoc.h"

typedef struct {
    int A, B;
} Input;

#define LIST_ELEMENT Input
#define LIST_EQ(a, b) (a.A == b.A && a.B == b.B)
#include "list.h"

static void parse_claw(int *AX, int *AY, int *BX, int *BY, int *RX, int *RY) {
    aoc_expect("Button A: X+");
    *AX = aoc_int();
    aoc_expect(", Y+");
    *AY = aoc_int();
    aoc_eat("\r\n");

    aoc_expect("Button B: X+");
    *BX = aoc_int();
    aoc_expect(", Y+");
    *BY = aoc_int();
    aoc_eat("\r\n");

    // Prize: X=8400, Y=5400
    aoc_expect("Prize: X=");
    *RX = aoc_int();
    aoc_expect(", Y=");
    *RY = aoc_int();
    aoc_eat("\r\n");
}

void run_day13() {
    aoc_begin();

    list_t *solutions_x = list_new();
    list_t *solutions_y = list_new();

    // Part 1
    uint64_t result = 0;
    int AX, AY, BX, BY, RX, RY;
    while (!aoc_eof()) {
        parse_claw(&AX, &AY, &BX, &BY, &RX, &RY);
        solutions_x->size = 0;
        solutions_y->size = 0;

        int range1 = MAX(RX / AX, RX / BX);
        for (int i = 0; i < range1; i++) {
            int vx = AX * i;
            int tb = 0;
            while (vx < RX) {
                vx += BX;
                tb += 1;
            }
            if (vx == RX) {
                list_add(solutions_x, (Input) { i, tb });
            }
        }

        int range2 = MAX(RY / AY, RY / BY);
        for (int i = 0; i < range2; i++) {
            int vy = AY * i;
            int tb = 0;
            while (vy < RY) {
                vy += BY;
                tb += 1;
            }
            if (vy == RY) {
                list_add(solutions_y, (Input) { i, tb });
            }
        }

        Input best_input = {};
        int best = INT_MAX;
        for (int i = 0; i < solutions_x->size; i++) {
            if (list_index_of(solutions_y, list_get(solutions_x, i)) == -1) {
                continue;
            }

            Input input = list_get(solutions_x, i);
            if (input.A * 3 + input.B * 1 < best) {
                best_input = input;
                best = input.A * 3 + input.B * 1;
            }
        }

        if (best < INT_MAX) {
            result += best;
        }
    }
    printf("Result 1: %llu\n", result);
    list_destroy(solutions_x);
    list_destroy(solutions_y);

    // part 2 - https://www.reddit.com/r/adventofcode/comments/1hd7irq/2024_day_13_an_explanation_of_the_mathematics/
    result = 0;
    aoc_rewind();
    while (!aoc_eof()) {
        parse_claw(&AX, &AY, &BX, &BY, &RX, &RY);

        // A = (p_x*b_y - prize_y*b_x) / (a_x*b_y - a_y*b_x)
        // B = (a_x * p_y - a_y * p_x) / (a_x * b_y - a_y * b_x)

        uint64_t PX = RX + 10000000000000L, PY = RY + 10000000000000L;
        double A = ((double) PX * (double) BY - (double) PY * (double) BX) / (AX * BY - AY * BX);
        double B = ((double) AX * (double) PY - (double) AY * (double) PX) / (AX * BY - AY * BX);

        if (fabs(A - round(A)) < 0.0004 && fabs(B - round(B)) < 0.0004) {
            result += ((uint64_t) round(A)) * 3 + ((uint64_t) round(B)) * 1;
        }
    }
    printf("Result 2: %llu\n", result);

    aoc_end();
}
