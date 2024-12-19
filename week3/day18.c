//
// Created by lennart on 12/19/24.
//

#include <limits.h>
#include <string.h>
#include <tgmath.h>

#include "aoc.h"

#define WIDTH 71
#define HEIGHT 71
#define FALLING_COUNT 1024

typedef struct {
    int x, y;
} Position;

typedef struct Node_struct {
    struct Node_struct *parent;
    Position position;
    unsigned int score;
} Node;

#define LIST_NAME frontier
#define LIST_ELEMENT Node*
#include "list.h"

static Node *nodes;
static Position finish;
static frontier_t *frontier;

static Node* get_next_node() {
    Node *best_node = nullptr;
    unsigned int best_score = UINT_MAX;
    int best_index = -1;
    for (int i = 0; i < frontier->size; i++) {
        Node *node = frontier_get(frontier, i);
        if (node->score == -1) continue;

        int cost = node->score + sqrt((finish.x - node->position.x) * (finish.y - node->position.y));
        if (cost >= best_score) continue;

        best_score = cost;
        best_node = node;
        best_index = i;
    }

    frontier_remove_at(frontier, best_index);
    return best_node;
}

static void process_neighbor(Node *parent, int x, int y) {
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) {
        return; // Out of bounds
    }

    Node *node = &nodes[y * WIDTH + x];
    if (node->score != 0) {
        return; // Already used
    }

    frontier_add(frontier, node);
    node->score = parent->score + 1;
    node->parent = parent;
}

static Node *solve() {
    // A* Search
    Node *end = nullptr;
    while (frontier->size > 0) {
        Node *node = get_next_node();
        if (node->position.x == WIDTH - 1 && node->position.y == HEIGHT - 1) {
            // Finished
            end = node;
            break;
        }

        int x = node->position.x, y = node->position.y;
        process_neighbor(node, x, y-1); // north
        process_neighbor(node, x+1, y); // east
        process_neighbor(node, x, y+1); // south
        process_neighbor(node, x-1, y); // west
    }

    return end;
}

static void print_map(Node *end) {
    char map[WIDTH * HEIGHT];
    memset(map, '.', WIDTH * HEIGHT);
    Node *parent = end->parent;
    while (parent != nullptr) {
        map[parent->position.y * WIDTH + parent->position.x] = (char) '0' + (char) (parent->score % 10);
        parent = parent->parent;
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Node *node = &nodes[y * WIDTH + x];
            if (node->score == -1) {
                printf("#");
            } else {
                printf("%c", map[y * WIDTH + x]);
            }
        }
        printf("\n");
    }
}

void run_day18() {
    aoc_begin();

    // Create memory grid
    nodes = malloc(sizeof(Node) * WIDTH  * HEIGHT);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        nodes[i] = (Node) {
            .position = (Position) { i % WIDTH, i / WIDTH },
            .score = 0
        };
    }
    finish = (Position) { WIDTH - 1, HEIGHT - 1 };

    // Read corrupted RAM
    for (int i = 0; i < FALLING_COUNT; i++) {
        int x, y;
        aoc_scan("%u,%u\n", &x, &y);
        nodes[y * WIDTH + x].score = -1;
    }

    // Setup search space
    Node *start = &nodes[0];
    start->score = 1;
    start->parent = nullptr;
    frontier = frontier_new();
    frontier_add(frontier, start);

    // Part 1
    Node *end = solve();
    printf("Result: %d\n", end == nullptr ? 0 : end->score - 1);

    // Part 2
    Position corrupted = {};
    while (!aoc_eof()) {
        unsigned int x, y;
        aoc_scan("%u,%u\n", &x, &y);
        nodes[y * WIDTH + x].score = -1;

        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            nodes[i].score = nodes[i].score == -1 ? -1 : 0;
        }
        start->score = 1;
        frontier->size = 0;
        frontier_add(frontier, start);

        Node *result = solve();
        if (result == nullptr) {
            corrupted.x = x;
            corrupted.y = y;
            break;
        }
    }
    printf("Result 2: %u,%u\n", corrupted.x, corrupted.y);

    frontier_destroy(frontier);
    aoc_end();
}
