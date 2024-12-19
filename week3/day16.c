//
// Created by lennart on 12/16/24.
//

#include <aoc.h>
#include <limits.h>
#include <list.h>
#include <stdint.h>
#include <time.h>

typedef struct {
    int x, y;
} Position;

typedef enum : uint8_t {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    Direction__Count
} Direction;

typedef struct Node_struct {
    struct Node_struct *parent;
    Position position;
    int score;
    Direction direction;
    char type;
} Node;

#define LIST_NAME nodes
#define LIST_ELEMENT Node*
#include "list.h"

#include <stdbit.h>

Map *map;
nodes_t *frontier;
int *visited;
bool *is_on_best;

static Node* find_start() {
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->height; x++) {
            if (aoc_map(map, x, y) == 'S') {
                Node *node = malloc(sizeof(Node));
                *node = (Node) {
                    .position =  { x, y },
                    .score = 0,
                    .direction = EAST,
                    .type = 'S',
                    .parent = nullptr
                };
                return node;
            }
        }
    }

    return nullptr;
}

static Node* find_best(nodes_t *frontier) {
    int best_cost = INT_MAX, best_index = -1;
    for (int i = 0; i < frontier->size; i++) {
        Node *node = frontier->elements[i];
        if (node->score < best_cost) {
            best_cost = node->score;
            best_index = i;
        }
    }

    Node *node = nodes_get(frontier, best_index);
    nodes_remove_at(frontier, best_index);
    return node;
}

static void get_neighbor(Node *parent, Direction direction) {
    Position position = parent->position;
    switch (direction) {
    case NORTH: position.y--; break;
    case EAST: position.x++; break;
    case SOUTH: position.y++; break;
    case WEST: position.x--; break;
    }

    int score = direction == parent->direction ? parent->score + 1 : parent->score + 1001;
    char type = aoc_map(map, position.x , position.y);
    unsigned int vkey = direction * map->width * map->height + position.y * map->width + position.x;
    if (type == '#' || (visited[vkey] != 0 && visited[vkey] < score)) {
        return;
    }
    visited[vkey] = score;

    Node *neighbor = malloc(sizeof(Node));
    *neighbor = (Node) {
        .position = position,
        .direction = direction,
        .score = score,
        .type = type,
        .parent = parent
    };

    nodes_add(frontier, neighbor);
}

void run_day16() {
    aoc_begin();

    map = aoc_map_load();
    visited = calloc(Direction__Count * map->width * map->height, sizeof(int));
    is_on_best = calloc(map->width * map->height, sizeof(bool));

    frontier = nodes_new();
    nodes_add(frontier, find_start());

    // Solve
    int result = 0;
    int i = 0;
    while (!frontier->size == 0) {
        Node *best = find_best(frontier);
        if (best->type == 'E') {
            if (result == 0) {
                result = best->score;
            }

            if (best->score == result) {
                Node *parent = best->parent;
                while (parent->type != 'S') {
                    is_on_best[parent->position.y * map->width + parent->position.x] = true;
                    parent = parent->parent;
                }
            }
        }

        get_neighbor(best, best->direction);
        get_neighbor(best, (best->direction + 1) % Direction__Count);
        get_neighbor(best, (best->direction + 3) % Direction__Count);
    }
    printf("Result 1: %d\n", result);

    // Part 2
    result = 2; // Start and End = 2
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (is_on_best[y * map->width + x]) {
                result++;
            }
        }
    }
    printf("Result 2: %d\n", result);

    // for (int y = 0; y < map->height; y++) {
    //     for (int x = 0; x < map->width; x++) {
    //         if (visited[NORTH * map->width * map->height + y * map->width + x]) {
    //             printf("N");
    //         } else if (visited[EAST * map->width * map->height + y * map->width + x]) {
    //             printf("E");
    //         }else if (visited[SOUTH * map->width * map->height + y * map->width + x]) {
    //             printf("S");
    //         }else if (visited[WEST * map->width * map->height + y * map->width + x]) {
    //             printf("W");
    //         } else {
    //             printf("%c", aoc_map(map, x, y));
    //         }
    //     }
    //     printf("\n");
    // }

    nodes_destroy(frontier);
    aoc_end();
}
