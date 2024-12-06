//
// Created by lennart on 12/6/24.
//

#define LINEBUF_SIZE 256

#include <stdio.h>

#define LIST_NAME lines
#define LIST_ELEMENT char*
#include "list.h"

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

typedef struct {
    char **tiles;
    int height;
    int width;
} Map;

typedef struct {
    int x;
    int y;
} Position;

Map *load_map(FILE *f) {
    lines_t lines; lines_init(&lines);
    char line[LINEBUF_SIZE];
    while (fgets(line, LINEBUF_SIZE, f) != nullptr) {
        char *nline = strdup(line);
        int length = strlen(nline);
        if (nline[length-1] == '\n') {
            nline[--length] = 0;
        }
        lines_add(&lines, nline);
    }

    Map *map = malloc(sizeof(Map));
    map->tiles = lines.elements;
    map->height = lines.size;
    map->width = strlen(map->tiles[0]);
    return map;
}

Position find_guard(Map *map) {
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            char c = map->tiles[y][x];
            if (c == '^') {
                return (Position) { x, y };
            }
        }
    }
    fprintf(stderr, "Could not find guard position\n");
    exit(1);
}

void run_day6() {
    FILE *f = fopen("input.txt", "r");
    Map *map = load_map(f);

    Position guard = find_guard(map);
    Direction direction = NORTH;

    int result = 0;
    // While within bounds
    while (guard.x >= 0 && guard.x < map->width && guard.y >= 0 && guard.y < map->height) {
        // If obstructed
        if (map->tiles[guard.y][guard.x] == '#') {
            // Change direction and go back
            switch (direction) {
            case NORTH: direction = EAST; guard.y++; break;
            case EAST: direction = SOUTH; guard.x--; break;
            case SOUTH: direction = WEST; guard.y--; break;
            case WEST: direction = NORTH; guard.x++; break;
            }
        }

        if (map->tiles[guard.y][guard.x] != 'X') {
            // Mark position as visited
            map->tiles[guard.y][guard.x] = 'X';
            result++;
        }

        // Move forward
        switch (direction) {
        case NORTH: guard.y--; break;
        case EAST: guard.x++; break;
        case SOUTH: guard.y++; break;
        case WEST: guard.x--; break;
        }
    }
    printf("Result: %d\n", result);

    for (int y = 0; y < map->height; y++) {
        printf("%s\n", map->tiles[y]);
    }

    fclose(f);
}
