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
    bool hn;
    bool he;
    bool hs;
    bool hw;
} Visited;

typedef struct {
    char **tiles;
    Visited* visited;
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
    map->visited = calloc(map->width * map->height, sizeof(Visited));
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

bool cycles_at(Map *map, Position guard, Direction direction) {
    // printf("Checking for cycles at %d, %d\n", guard.x, guard.y);
    const Position paradox = guard;
    const char t = map->tiles[paradox.y][paradox.x];
    map->tiles[paradox.y][paradox.x] = '#';
    memset(map->visited, 0, map->width * map->height * sizeof(Visited)); // Reset visited state

    // while (map->tiles[guard.y][guard.x] == '#') {
    //     // Change direction and go back
    //     switch (direction) {
    //     case NORTH: direction = EAST; break;
    //     case EAST: direction = SOUTH; break;
    //     case SOUTH: direction = WEST; break;
    //     case WEST: direction = NORTH; break;
    //     }
    // }

    // do our check
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

        Visited *visited = &map->visited[guard.y * map->width + guard.x];
        if (
            (direction == NORTH && visited->hn)
            || (direction == EAST && visited->he)
            || (direction == SOUTH && visited->hs)
            || (direction == WEST && visited->hw)
        ) {
            map->tiles[paradox.y][paradox.x] = t;
            return true;
        } else {
            if (direction == NORTH) visited->hn = true;
            if (direction == EAST) visited->he = true;
            if (direction == SOUTH) visited->hs = true;
            if (direction == WEST) visited->hw = true;
        }

        // Move forward
        switch (direction) {
        case NORTH: guard.y--; break;
        case EAST: guard.x++; break;
        case SOUTH: guard.y++; break;
        case WEST: guard.x--; break;
        }
    }

    map->tiles[paradox.y][paradox.x] = t;
    return false;
}

void run_day6() {
    FILE *f = fopen("input.txt", "r");
    Map *map = load_map(f);

    Position guard = find_guard(map);
    Direction direction = NORTH;

    int result = 1;
    int cycles = 0;
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
        } else if (map->tiles[guard.y][guard.x] == '.') {
            if (cycles_at(map, guard, direction)) {
                cycles++;
                if (map->tiles[guard.y][guard.x] == '.') {
                    result++;
                }
                map->tiles[guard.y][guard.x] = 'O';
            }
        }

        if (map->tiles[guard.y][guard.x] == '.') {
            // Mark position as visited
            if (direction == NORTH || direction == SOUTH) {
                map->tiles[guard.y][guard.x] = '|';
            } else {
                map->tiles[guard.y][guard.x] = '-';
            }
            result++;
        } else if (
            (map->tiles[guard.y][guard.x] == '|' && (direction == EAST || direction == WEST))
            || (map->tiles[guard.y][guard.x] == '-' && (direction == NORTH || direction == SOUTH))
        ) {
            map->tiles[guard.y][guard.x] = '+';
        }

        // Move forward
        switch (direction) {
        case NORTH: guard.y--; break;
        case EAST: guard.x++; break;
        case SOUTH: guard.y++; break;
        case WEST: guard.x--; break;
        }
    }
    printf("Result 1: %d\n", result);
    printf("Result 2: %d\n", cycles);

    for (int y = 0; y < map->height; y++) {
        free(map->tiles[y]);
    }
    free(map->tiles);
    free(map->visited);
    free(map);
    fclose(f);
}
