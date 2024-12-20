//
// Created by lennart on 12/20/24.
//

#include <aoc.h>
#include <limits.h>

static Map *map;

typedef struct {
    int x, y;
} Position;

typedef struct Node_struct {
    Position pos;
    int score;
} Node;

// #define LIST_NAME frontier
// #define LIST_ELEMENT Node*
// #include "list.h"

Node *nodes;

static Position find_start() {
    for (int y = 0 ; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            char cell = aoc_map(map, x, y);
            if (cell == 'S') {
                return (Position) { x, y };
            }
        }
    }

    fprintf(stderr, "Could not find start");
    return (Position) { 0, 0 };
}

static Node* next_node(Node *node) {
    Node *nn = &nodes[node->pos.x + (node->pos.y - 1) * map->width];
    Node *ne = &nodes[node->pos.x + 1 + (node->pos.y) * map->width];
    Node *ns = &nodes[node->pos.x + (node->pos.y + 1) * map->width];
    Node *nw = &nodes[node->pos.x - 1 + (node->pos.y) * map->width];

    if (nn->score == 0 || nn->score > node->score) {
        return nn;
    } if (ne->score == 0 || ne->score > node->score) {
        return ne;
    } if (ns->score == 0 || ns->score > node->score) {
        return ns;
    } if (nw->score == 0 || nw->score > node->score) {
        return nw;
    }

    fprintf(stderr, "No next node");
    return nullptr;
}

static int solve(Position start, int range, int threshold) {
    int result = 0;
    Node *node = &nodes[start.x + start.y * map->width];
    while (aoc_map(map, node->pos.x, node->pos.y) != 'E') {
        for (int rx = -range; rx <= range; rx++) {
            for (int ry = -range; ry <= range; ry++) {
                int x = node->pos.x + rx, y = node->pos.y + ry;
                int mhd = abs(rx) + abs(ry); // manhattan distance
                if (mhd == 0 || mhd > range ||  x < 0 || x >= map->width || y < 0 || y >= map->height) {
                    continue;
                }

                Node *candidate = &nodes[x + y * map->width];
                if (candidate->score - node->score >= threshold + mhd) {
                    result++;
                }
            }
        }

        node = next_node(node);
    }

    return result;
}

void run_day20() {
    aoc_begin();

    // Load grid
    map = aoc_map_load();
    nodes = malloc(sizeof(Node) * map->width * map->height);
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            nodes[x + y * map->width] = (Node) {
                .pos = (Position) { x, y },
                .score = aoc_map(map, x, y) == '#' ? -1 : 0
            };
        }
    }

    // Find normal path
    Position start = find_start();
    int score = 0;
    Node *node = &nodes[start.x + start.y * map->width];
    while (aoc_map(map, node->pos.x, node->pos.y) != 'E') {
        Node *next = next_node(node);
        next->score = ++score;
        if (next != nullptr) {
            node = next;
        }
    }

    // Find cheats
    printf("Result 1: %d\n", solve(start, 2, 100));
    printf("Result 2: %d\n", solve(start, 20, 100));

    aoc_end();
}
