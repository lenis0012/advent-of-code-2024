//
// Created by lennart on 12/8/24.
//

#define LINEBUF_SIZE 64

#include <stdio.h>

#define LIST_NAME lines
#define LIST_ELEMENT char*
#include "list.h"

typedef struct {
    char **cells;
    unsigned int width;
    unsigned int height;
} Map;

typedef struct {
    unsigned int x;
    unsigned int y;
} Position;

#define LIST_NAME antennas
#define LIST_ELEMENT Position
#include "list.h"
antennas_t antennas[128] = {}; // By ascii character

static void find_antennas(Map *map) {
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            char c = map->cells[y][x];
            if (c != '.') {
                antennas_t *ant = &antennas[c];
                if (ant->capacity == 0) antennas_init(ant);
                antennas_add(ant, (Position) { x, y });
            }
        }
    }
}

static int solve(Map *map, bool resonance, char mark) {
    int result = 0;

    for (unsigned char freq = 0; freq < 128; freq++) {
        if (antennas[freq].size <= 1) continue;

        antennas_t *ant = &antennas[freq];
        for (int i = 0; i < ant->size; i++) {
            for (int j = 0; j < ant->size; j++) {
                if (i == j) continue;

                Position from = antennas_get(ant, i);
                Position to = antennas_get(ant, j);
                Position delta = {
                    to.x - from.x,
                    to.y - from.y
                };

                Position antinode = {
                    from.x - delta.x,
                    from.y - delta.y
                };

                do {
                    if (antinode.x >= 0 && antinode.x < map->width && antinode.y >= 0 && antinode.y < map->height) {
                        if (map->cells[antinode.y][antinode.x] != mark) {
                            map->cells[antinode.y][antinode.x] = mark;
                            result++;
                        }
                        antinode.x -= delta.x;
                        antinode.y -= delta.y;
                    } else {
                        break;
                    }
                } while (resonance);

                if (resonance && map->cells[from.y][from.x] != mark) {
                    map->cells[from.y][from.x] = mark;
                    result++;
                }
            }
        }
    }

    return result;
}

void run_day8() {
    FILE *f = fopen("input.txt", "r");

    lines_t lines; lines_init(&lines);
    char line[LINEBUF_SIZE];
    while (fgets(line, LINEBUF_SIZE, f) != nullptr) {
        int length = strlen(line);
        if (line[length - 1] == '\n') line[length - 1] = 0;
        lines_add(&lines, strdup(line));
    }
    fclose(f);

    Map map = {
        .cells = lines.elements,
        .height = lines.size,
        .width = strlen(lines_get(&lines, 0))
    };

    find_antennas(&map);
    printf("Result 1: %d\n", solve(&map, false, '#'));
    printf("Result 2: %d\n", solve(&map, true, '*'));

    for (int y = 0; y < map.height; y++) {
        free(map.cells[y]);
    }
    for (unsigned char freq= 0 ; freq < 128; freq++) {
        if (antennas[freq].capacity == 0) continue;
        antennas_fini(&antennas[freq]);
    }
    free(map.cells);
}
