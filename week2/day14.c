//
// Created by lennart on 12/15/24.
//

#include <aoc.h>

typedef struct {
    int pos_x, pos_y;
    int vel_x, vel_y;
} Robot;

#define LIST_NAME robots
#define LIST_ELEMENT Robot
#include "list.h"

#define LIST_NAME quadrant
#define LIST_ELEMENT Robot*
#define LIST_UNORDERED
#define LIST_EQ(a, b) (a == b)
#include "list.h"

#define MAP_WIDTH 101
#define MAP_HEIGHT 103
#define MAP_CX (MAP_WIDTH / 2)
#define MAP_CY (MAP_HEIGHT / 2)

quadrant_t quadrants[4];

static quadrant_t* get_quadrant(const Robot *robot) {
    if (robot->pos_x < MAP_CX && robot->pos_y < MAP_CY) {
        return &quadrants[0];
    } if (robot->pos_x > MAP_CX && robot->pos_y < MAP_CY) {
        return &quadrants[1];
    } if (robot->pos_x > MAP_CX && robot->pos_y > MAP_CY) {
        return &quadrants[2];
    } if (robot->pos_x < MAP_CX && robot->pos_y > MAP_CY) {
        return &quadrants[3];
    }

    return nullptr;
}

static int wrap_pos_x(int pos) {
    pos %= MAP_WIDTH;
    return pos < 0 ? pos + MAP_WIDTH : pos;
}

static int wrap_pos_y(int pos) {
    pos %= MAP_HEIGHT;
    return pos < 0 ? pos + MAP_HEIGHT : pos;
}

void run_day14() {
    aoc_begin();

    // Parse input
    robots_t *robots = robots_new();
    while (!aoc_eof()) {
        Robot robot;
        aoc_expect("p="); robot.pos_x = aoc_int();
        aoc_expect(","); robot.pos_y = aoc_int();

        aoc_expect(" v="); robot.vel_x = aoc_int();
        aoc_expect(","); robot.vel_y = aoc_int();
        aoc_eat("\r\n");

        robots_add(robots, robot);
    }

    // Initialize
    for (int i = 0; i < 4; i++) {
        quadrant_init(&quadrants[i]);
    }
    for (int r = 0; r < robots->size; r++) {
        Robot *robot = &robots->elements[r];
        quadrant_t *quadrant = get_quadrant(robot);
        if (quadrant != nullptr) {
            quadrant_add(quadrant, robot);
        }
    }

    // Iterate
    for (int i = 0; i < 100; i++) {
        for (int r = 0; r < robots->size; r++) {
            Robot *robot = &robots->elements[r];
            quadrant_t *previous_quadrant = get_quadrant(robot);
            robot->pos_x = wrap_pos_x(robot->pos_x + robot->vel_x);
            robot->pos_y = wrap_pos_y(robot->pos_y + robot->vel_y);
            quadrant_t *new_quadrant = get_quadrant(robot);

            if (previous_quadrant != new_quadrant) {
                if (previous_quadrant != nullptr) {
                    quadrant_remove_at(previous_quadrant, quadrant_index_of(previous_quadrant, robot));
                }
                if (new_quadrant != nullptr) {
                    quadrant_add(new_quadrant, robot);
                }
            }
        }
    }

    // Count
    unsigned int result = 0;
    for (int i = 0; i < 4; i++) {
        quadrant_t *quad = &quadrants[i];
        result = i == 0 ? quad->size : result * quad->size;
    }
    printf("Result: %u\n", result);


    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            printf(".");
        }
        printf("\n");
    }

    robots_destroy(robots);
    for (int i = 0; i < 4; i++) quadrant_fini(&quadrants[i]);
    aoc_end();
}
