//
// Created by lennart on 12/4/24.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_ELEMENT char*
#include "list.h"

#include "aoc.h"

#define LINE_BUFSIZE 2048

enum Direction {
    NORTH,
    NE,
    EAST,
    SE,
    SOUTH,
    SW,
    WEST,
    NW,
    Direction_Size
};

static int puzzle_offsetst[Direction_Size][2] = {
    { 0, -1 }, // NORTH
    { 1, -1 }, // NE
    { 1, 0 }, // EAST
    { 1, 1 }, // SE
    { 0, 1 }, // SOUTH
    { -1, 1 }, // SW
    { -1, 0 }, // WEST
    { -1, -1 } //  NW
};

typedef struct {
    char *data;
    unsigned int width;
    unsigned int height;
} Puzzle;

int puzzle_at(Puzzle *puzzle, int x, int y) {
    if (x < 0 || x >= puzzle->width) {
        return 0; // out of bounds
    } if (y < 0 || y >= puzzle->height) {
        return 0; // out of bounds
    }

    return puzzle->data[y * puzzle->width + x];
}

int is_xmas(Puzzle *puzzle, int x, int y) {
    if (puzzle_at(puzzle, x, y) != 'X') {
        return 0;
    }

    int amount = 0;
    for (enum Direction d = NORTH; d < Direction_Size; d++) {
        int *offset = puzzle_offsetst[d];
        for (int i = 0; i < strlen("XMAS"); i++) {
            int c = puzzle_at(puzzle, x + offset[0] * i, y + offset[1] * i);
            if (c != "XMAS"[i]) {
                goto next;
            }
        }

        amount++;
        next:
    }

    return amount;
}

int is_mas_x(Puzzle *puzzle, int x, int y) {
    if (puzzle_at(puzzle, x, y) != 'A') {
        return 0;
    }

    if (
        (
            (puzzle_at(puzzle, x - 1, y - 1) == 'M' && puzzle_at(puzzle, x + 1, y + 1) == 'S')
            || (puzzle_at(puzzle, x - 1, y - 1) == 'S' && puzzle_at(puzzle, x + 1, y + 1) == 'M')
        )
        && (
            (puzzle_at(puzzle, x + 1, y - 1) == 'M' && puzzle_at(puzzle, x - 1, y + 1) == 'S')
            || (puzzle_at(puzzle, x + 1, y - 1) == 'S' && puzzle_at(puzzle, x - 1, y + 1) == 'M')
        )
    ) {
        return 1;
    }

    return 0;
}

void run_day4() {
    FILE *f = fopen("input.txt", "r");
    Puzzle puzzle;
    puzzle.data = calloc(sizeof(char), aoc_file_size(f));

    char *buffer = malloc(LINE_BUFSIZE);
    char *line = buffer;
    line = fgets(line, LINE_BUFSIZE, f);
    puzzle.width = strlen(line) - 1;
    puzzle.height = 0;
    while (line != nullptr) {
        assert(line != nullptr);
        strncpy(&puzzle.data[puzzle.width * puzzle.height], line, puzzle.width);
        puzzle.height++;
        line = fgets(line, LINE_BUFSIZE, f);
    }
    fclose(f);

    // Solve
    int result1 = 0;
    int result2 = 0;
    for (int x = 0; x < puzzle.width; x++) {
        for (int y = 0; y < puzzle.height; y++) {
            result1 += is_xmas(&puzzle, x, y);
            result2 += is_mas_x(&puzzle, x, y);
        }
    }
    printf("Result 1: %d\n", result1);
    printf("Result 2: %d\n", result2);

    free(buffer);
    free(puzzle.data);
}
