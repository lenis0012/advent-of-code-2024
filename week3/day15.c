//
// Created by lennart on 12/16/24.
//

#include <assert.h>

#include "aoc.h"

typedef struct {
    int x, y;
} Position;

static Map *map;
static Map *wide_map;
static Position robot;

static void find_robot() {
    for (int x = 0; x < map->width; x++) {
        for (int y = 0; y < map->height; y++) {
            if (aoc_map(map, x, y) == '@') {
                robot = (Position) { x, y };
                return;
            }
        }
    }
}

static bool robot_push(Position pos, char direction) {
    Position next = pos;
    switch (direction) {
    case '^': next.y--; break;
    case '>': next.x++; break;
    case 'v': next.y++; break;
    case '<': next.x--; break;
    }

    char obstacle = aoc_map(map, next.x, next.y);
    if (obstacle == '#') {
        return false; // Failed
    }
    if (obstacle == 'O'&& !robot_push(next, direction)) {
        return false; // Failed downstream
    }
    if (direction == '^' || direction == 'v') {
        if (obstacle == '[' && (!robot_push(next, direction) || !robot_push((Position) { next.x + 1, next.y }, direction))) {
            return false;
        }
        if (obstacle == ']' && (!robot_push(next, direction) || !robot_push((Position) { next.x - 1, next.y }, direction))) {
            return false;
        }
    } else if ((obstacle == '[' || obstacle == ']') && !robot_push(next, direction)) {
        return false;
    }

    char origin = aoc_map(map, pos.x, pos.y);
    if (origin == '@') {
        robot = next;
    }
    aoc_map_set(map, pos.x, pos.y, '.');
    aoc_map_set(map, next.x, next.y, origin);
    return true;
}

static void solve_part1() {
    // Process instructions
    while (!aoc_eof()) {
        char c = aoc_char();
        if (c == EOF || c == '\n' || c == '\r') {
            continue;
        }

        assert(c == '^' || c == '>' || c == 'v' || c == '<');
        robot_push(robot, c);
    }

    int result = 0;
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            char c = aoc_map(map, x, y);
            if (c == 'O') {
                result += 100 * y + x;
            }
        }
    }
    printf("Result: %d\n", result);
}

static void solve_part2() {
    Map *wide_map = malloc(sizeof(Map));
    *wide_map = (Map) {
        .cells = malloc(sizeof(char) * map->width * 2 * map->height),
        .width = map->width * 2,
        .height = map->height
    };

    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            char c = aoc_map(map, x, y);
            switch (c) {
            case '.':
            case '#':
                aoc_map_set(wide_map, x * 2 + 0, y, c);
                aoc_map_set(wide_map, x * 2 + 1, y, c);
                break;
            case 'O':
                aoc_map_set(wide_map, x * 2 + 0, y, '[');
                aoc_map_set(wide_map, x * 2 + 1, y, ']');
                break;
            case '@':
                aoc_map_set(wide_map, x * 2 + 0, y, '@');
                aoc_map_set(wide_map, x * 2 + 1, y, '.');
                break;
            }
        }
    }
    map = wide_map;
    find_robot();

    // Process instructions
    while (!aoc_eof()) {
        char c = aoc_char();
        if (c == EOF || c == '\n' || c == '\r') {
            continue;
        }

        assert(c == '^' || c == '>' || c == 'v' || c == '<');
        robot_push(robot, c);
    }

    int result = 0;
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            char c = aoc_map(map, x, y);
            if (c == '[') {
                result += 100 * y + x;
            }
            printf("%c", c);
        }
        printf("\n");
    }
    printf("Result: %d\n", result);
}

void run_day15() {
    aoc_begin();
    map = aoc_map_load();
    find_robot();

    solve_part1();

    aoc_rewind();
    map = aoc_map_load();
    solve_part2();

    aoc_end();
}
